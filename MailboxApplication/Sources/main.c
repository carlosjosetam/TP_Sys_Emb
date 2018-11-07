#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <Private/Itoa.h>
#include <Private/Application.h>
#include <Processor/Processor.h>

void system_kickstart (void)
{
  /*
   * Enabling the AICU.
   */

  if (cpu_mp_id () == 0)
  {
    cpu_write (UINT32, & (PLATFORM_AICU_BASE -> control), 0x1);

    for (int32_t i = 0; i < cpu_mp_count (); i += 1)			//to handle with all processors
    {
      cpu_trap_attach_isr (i, 0, 0, ipi_isr);				//call ipi_isr as a handler
    }

    cpu_mp_proceed ();							// to syncronize the processor
    cpu_trap_enable (0);
  }
  else
  {
    cpu_mp_wait ();
    cpu_trap_enable (0);
  }

  /*
   * Depending on the processor, send an IPI
   */

  switch (cpu_mp_id ())
  {
    case 0:
      cpu_mp_send_ipi (2, 1234, NULL);
      break;

    case 1:
      cpu_mp_send_ipi (3, 5678, NULL);
      break;

    default: break;
  }

  while (true);
}
