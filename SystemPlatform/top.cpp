/*
 * SoCLib is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 of the License.
 * 
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "mips32.h"
#include "mapping_table.h"
#include "cc_id_table.h"
#include "gdbserver.h"
#include "vci_cc_cache.h"
#include "vci_cc_ram.h"
#include "vci_multi_tty.h"
#include "vci_timer.h"
#include "vci_mailbox.h"
#include "vci_aicu.h"
#include "vci_vgmn.h"
#include "vci_log_console.h"

#include "segmentation.h"

#define DATA_BLOCK_SIZE     8 
#define CACHE_N_LINE      256

#define N_EXTRA_TARGETS     6
#define N_EXTRA_INITIATORS  1

#define N_GLOBAL_IRQS       1
#define N_LOCAL_IRQS        2

using namespace std;
using namespace sc_core;
using namespace soclib::caba;
using namespace soclib::common;

typedef VciParams<4,8,32,1,1,1,8,1,1,1> vci_param;
typedef	GdbServer<Mips32ElIss> iss_t;

int _main(int argc, char *argv[])
{
  char name[32];
  char * simulation_ncycles_var = getenv ("SIMULATION_N_CYCLES");
  char * simulation_ncpus_var = getenv ("SIMULATION_N_CPUS");
  long int n_cycles = 0, n_cpus = 0;

  /*
   * Check the parameters : param 1 is the name of the simulated application.
   */

  if (argc != 2)
  {
    cout << "[PLATFORM] Wrong number of arguments." << endl;
    cout << "Usage : ./simulation application_name" << endl;
    exit (-1);
  }

  /*
   * Simulation cycles.
   */

  if (simulation_ncycles_var == NULL)
  {
    n_cycles = -1;
  }
  else
  {
    n_cycles = strtol (simulation_ncycles_var, (char **)NULL, 10);
    if (n_cycles == 0)
    {
      cout << "[PLATFORM] Invalid SIMULATION_N_CYCLES value, ";
      cout << "defaulting to infinite." << endl;
      n_cycles = -1;
    }
  }

  if (n_cycles == -1)
  {
    cout << "[PLATFORM] Infinite simulation." << endl;
  }
  else
  {
    cout << "[PLATFORM] Simulating " << n_cycles << " cycle(s)." << endl;
  }

  /*
   * Number of CPUs.
   */

  if (simulation_ncpus_var == NULL)
  {
    cout << "[PLATFORM] SIMULATION_N_CPUS not defined." << endl;
    n_cpus = 1;
  }
  else
  {
    n_cpus = strtol (simulation_ncpus_var, (char **)NULL, 10);
    if (n_cpus == 0)
    {
      cout << "[PLATFORM] Invalid SIMULATION_N_CPUS value, defaulting to 1.";
      cout << endl;
      n_cpus = 1;
    }
  }

  cout << "[PLATFORM] Using " << n_cpus << " CPU(s)." << endl;

  /*
   * Mapping table.
   */

	CcIdTable * cctab = new CcIdTable ();
	MappingTable maptab (32, IntTab (8), IntTab (8), 0xC0000000);

  /*
   * Configure the segments.
   */

	maptab . add (Segment (
        "reset",
        RESET_BASE,
        RESET_SIZE,
        IntTab (0),
        true
        ));

	maptab . add (Segment (
        "excep",
        EXCEP_BASE,
        EXCEP_SIZE,
        IntTab (0),
        true
        ));

	maptab . add (Segment (
        "memory",
        MEMORY_BASE,
        MEMORY_SIZE ,
        IntTab (0),
        true));

	maptab . add (Segment (
        "log",
        LOG_BASE,
        LOG_SIZE ,
        IntTab (1),
        false));

	maptab . add (Segment (
        "tty",
        TTY_BASE,
        TTY_SIZE ,
        IntTab (2),
        false));

	maptab . add (Segment (
        "timer",
        TIMER_BASE,
        TIMER_SIZE ,
        IntTab (3),
        false));

	maptab . add (Segment (
        "mailbox",
        MAILBOX_BASE,
        MAILBOX_SIZE ,
        IntTab (4),
        false));

	maptab . add (Segment (
        "aicu",
        AICU_BASE,
        AICU_SIZE ,
        IntTab (5),
        false));

	for (int i = 0 ; i < n_cpus; i++)
	{
		sprintf (name,"CPU%d",i);	
		maptab.add (Segment (
          name,
          CACHE_BASE + i * 0x1000000,
          CACHE_SIZE,
          IntTab (i + N_EXTRA_TARGETS),
          false
          ));
	}

  /*
   * Clock and reset signals.
   */

	sc_clock signal_clk ("signal_clk");
	sc_signal<bool> signal_resetn ("signal_resetn");

  /*
   * Processor signals.
   */

	sc_signal<bool> * signal_cpu_it[n_cpus][iss_t::n_irq];
	VciSignals<vci_param> * signal_cpu_vci_initiator[n_cpus];
	VciSignals<vci_param> * signal_cpu_vci_target[n_cpus];

  for (int i = 0; i < n_cpus; i++)
  {
    sprintf (name, "signal_cpu_vci_initiator%d", i);
    signal_cpu_vci_initiator[i] = new VciSignals<vci_param> (name);

    sprintf (name, "signal_cpu_vci_target%d", i);
    signal_cpu_vci_target[i] = new VciSignals<vci_param> (name);

    for (unsigned int j = 0; j < iss_t::n_irq; j++)
    {
      sprintf (name, "signal_cpu%d_it%d", i, j);
      signal_cpu_it[i][j] = new sc_signal<bool> (name);
    }
  }

  /*
   * Memory signals.
   */

	VciSignals<vci_param> signal_ram_vci_target ("signal_ram_vci_target");
	VciSignals<vci_param> signal_ram_vci_initiator ("signal_ram_vci_initiator");

  /*
   * Log signal.
   */

	VciSignals<vci_param> signal_log_vci_target ("signal_log_vci_target");

  /*
   * TTY signals.
   */

	sc_signal<bool> signal_tty_it ("signal_tty_it");
  VciSignals<vci_param> signal_tty_vci_target("signal_tty_vci_target");

  /*
   * Timer signals.
   */

	sc_signal<bool> * signal_timer_it[n_cpus];
  VciSignals<vci_param> signal_timer_vci_target("signal_timer_vci_target");


  for (int i = 0; i < n_cpus; i += 1)
  {
    sprintf (name, "signal_timer_it%d", i);
    signal_timer_it[i] = new sc_signal<bool> (name);
  }

  /*
   * Mailbox signals.
   */

  sc_signal<bool> * signal_mailbox_it[n_cpus];
  VciSignals<vci_param> signal_mailbox_vci_target ("signal_mailbox_vci_target");

  for (int i = 0; i < n_cpus; i += 1)
  {
    sprintf (name, "signal_mailbox_it%d", i);
    signal_mailbox_it[i] = new sc_signal<bool> (name);
  }

  /*
   * AICU signals.
   */

  VciSignals<vci_param> signal_aicu_vci_target ("signal_aicu_vci_target");

  /*
   * Registering Processor IDs
   */

	for (int k = 0; k < n_cpus; k ++)
	{
		cctab -> register_coherent_initiator (
        maptab . indexForId (k),
        IntTab (k + N_EXTRA_TARGETS)
        );
	}

  /*
   * Registering Master device IDs.
   */

  /*
   * Loader.
   */
  
	Loader loader (argv[1]);
  iss_t::set_loader (& loader);

  /*
   * Processors.
   */

  VciCcCache<vci_param, iss_t> * cache[n_cpus];

  for (int i = 0; i < n_cpus; i++)
  {
    sprintf (name, "CPU%d", i);
	  cache[i] =  new VciCcCache<vci_param, iss_t> (
        name,
        IntTab (i),
        IntTab (i + N_EXTRA_TARGETS),
        CACHE_N_LINE,
        DATA_BLOCK_SIZE,
        CACHE_N_LINE,
        DATA_BLOCK_SIZE,
        i,
        & maptab
        );

	  cache[i] -> p_clk (signal_clk);
	  cache[i] -> p_resetn (signal_resetn);
	  cache[i] -> p_i_vci (*signal_cpu_vci_initiator[i]);
	  cache[i] -> p_t_vci (*signal_cpu_vci_target[i]);

    for (unsigned int j = 0; j < iss_t::n_irq; j ++)
    {
	    cache[i] -> p_irq[j] (*signal_cpu_it[i][j]); 
    }
  }

  /*
   * Memory.
   */

	VciCcRam<vci_param> ram (
      "ram",
      IntTab (n_cpus),
      IntTab (0),
      cctab,
      n_cpus,
      loader,
      DATA_BLOCK_SIZE,
      & maptab
      );

  ram . p_clk (signal_clk);
	ram . p_resetn (signal_resetn);
	ram . p_t_vci (signal_ram_vci_target);
	ram . p_i_vci (signal_ram_vci_initiator);

  /*
   * Log.
   */

	VciLogConsole<vci_param> log (
      "log",
      IntTab (1),
      maptab
      );

  log . p_clk (signal_clk);
	log . p_resetn (signal_resetn);
	log . p_vci (signal_log_vci_target);

  /*
   * TTY.
   */

  VciMultiTty<vci_param> tty (
      "tty",
      IntTab (2),
      maptab,
      "terminal",
      NULL);

  tty . p_clk (signal_clk);
  tty . p_resetn (signal_resetn);
  tty . p_vci (signal_tty_vci_target);
  tty . p_irq[0] (signal_tty_it); 

  /*
   * Timer
   */

	VciTimer<vci_param> timer (
      "vcittimer",
      IntTab (3),
      maptab,
      n_cpus 
      );

  timer . p_clk (signal_clk);
  timer . p_resetn (signal_resetn);
  timer . p_vci (signal_timer_vci_target);

  for (int i = 0; i < n_cpus; i += 1)
  {
    timer . p_irq[i] (*signal_timer_it[i]); 
  }

  /*
   * Mailbox.
   */

  VciMailbox<vci_param> mailbox (
      "mailbox",
      maptab,
      IntTab (4),
      n_cpus
      );

  mailbox . p_clk (signal_clk);
  mailbox . p_resetn (signal_resetn);
  mailbox . p_vci (signal_mailbox_vci_target);

  for (int i = 0; i < n_cpus; i += 1)
  {
    mailbox . p_irq[i] (*signal_mailbox_it[i]); 
  }

  /*
   * Advanced ICU.
   */

  VciAicu<vci_param> aicu (
      "aicu",
      maptab,
      IntTab (5),
      n_cpus,
      N_GLOBAL_IRQS,
      N_LOCAL_IRQS
      );

  aicu . p_clk (signal_clk);
  aicu . p_resetn (signal_resetn);
  aicu . p_vci (signal_aicu_vci_target);

  /*
   * Connect the local interrupt sources.
   */

  for (int i = 0; i < n_cpus; i += 1)
  {
    aicu . p_irq_in[i] (*signal_mailbox_it[i]); 
    aicu . p_irq_in[n_cpus + i] (*signal_timer_it[i]); 
  }

  /*
   * Connect the global interrupt sources.
   */

  aicu . p_irq_in[N_LOCAL_IRQS * n_cpus + 0] (signal_tty_it);

  /*
   * Connect output interrupts.
   */

  for (int i = 0; i < n_cpus; i += 1)
  {
    aicu . p_irq_out[i] (*signal_cpu_it[i][0]);
  }

  /*
   * Network.
   */

	VciVgmn<vci_param> vgmn (
      "vgmn",
      maptab,
      n_cpus + N_EXTRA_INITIATORS,
      n_cpus + N_EXTRA_TARGETS,
      1,
      8
      );

	vgmn . p_clk (signal_clk);
	vgmn . p_resetn (signal_resetn);

  /*
   * Connecting initiators.
   */

  for (int i = 0; i < n_cpus; i++)
  {
	  vgmn . p_to_initiator[i] (*signal_cpu_vci_initiator[i]);
  }

  vgmn . p_to_initiator[n_cpus] (signal_ram_vci_initiator);

  /*
   * Connecting targets.
   */

	vgmn . p_to_target[0] (signal_ram_vci_target);
	vgmn . p_to_target[1] (signal_log_vci_target);
  vgmn . p_to_target[2] (signal_tty_vci_target);
  vgmn . p_to_target[3] (signal_timer_vci_target);
  vgmn . p_to_target[4] (signal_mailbox_vci_target);
  vgmn . p_to_target[5] (signal_aicu_vci_target);

  for (int i = 0; i < n_cpus; i++)
  {
	  vgmn . p_to_target[i + N_EXTRA_TARGETS] (*signal_cpu_vci_target[i]);
  }

  /*
   * Start of simulation
   */

	sc_start(sc_core::sc_time(0, SC_NS));
	signal_resetn = false;

	sc_start(sc_core::sc_time(1, SC_NS));
	signal_resetn = true;

	if (n_cycles == -1)
  {
    sc_start();
  }
  else
  {
    sc_start(sc_core::sc_time(n_cycles, SC_NS));
  }

  /*
   * Delete new'd objects.
   */

  for (int i = 0; i < n_cpus; i++)
  {
    delete signal_cpu_vci_initiator[i];
    delete signal_cpu_vci_target[i];

    delete signal_timer_it[i];
    delete signal_mailbox_it[i];

    for (unsigned int j = 0; j < iss_t::n_irq; j++)
    {
      delete signal_cpu_it[i][j];
    }

    delete cache[i];
  }
 
	return EXIT_SUCCESS;
}

int sc_main (int argc, char *argv[])
{
	try
  {
		return _main(argc, argv);
	}
  catch (exception &e)
  {
		cout << e.what() << endl;
	}
  catch (...)
  {
		cout << "Unknown exception occured" << endl;
		throw;
	}

	return 1;
}
