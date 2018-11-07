#include <stdint.h>
#include <stdbool.h>
#include <Private/XPuts.h>
#include <Private/Itoa.h>
#include <Private/Application.h>
#include <stdio.h>
#include <Platform/Platform.h>
#include <Processor/Processor.h>

uint32_t TTY_DEVICE = 0xC1000000;




void xputs (char * s);

void system_kickstart (void)
{
  xputs("begining of the main\r\n");
  bigtime_t cpu_time; //représente le tps en ns
  interrupt_handler_t interrupt_handler;
  int32_t cpuid = cpu_mp_id();
  //soclib_timer_port_t timer = & PLATFORM_TIMER_BASE[cpuid];
  interrupt_handler=timer_isr;

  // Enabling the AICU.
  cpu_write(UINT32, &(PLATFORM_AICU_BASE -> control), 1);

  // Attaching the timer handler.
  cpu_trap_attach_isr (0, 1, 0, timer_isr);  //This interruption (with ID 1)will activate timer_isr
  cpu_trap_enable (1);   		     //enable the interruption with ID 1.


  //Starting the timer.
  xputs("starting timer\r\n");
  cpu_timer_get(cpuid, &cpu_time);	     //It uses the id of CPU and his time is received at cpu_time 
  cpu_timer_set(cpuid, cpu_time);	     //It sees if the time of the interruption has already over


  //Reading some values.
  
  int i;
  while (true){
    xputs("Out of interruption \n");
    for(i=0; i<1000; i++);
  }
}










