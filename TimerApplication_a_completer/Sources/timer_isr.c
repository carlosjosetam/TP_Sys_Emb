#include <Platform/Platform.h>
#include <Processor/Processor.h>
#include <Private/XPuts.h>

extern soclib_timer_port_t PLATFORM_TIMER_BASE; 

int32_t timer_isr (void * data)
{
 
  int32_t cpuid = cpu_mp_id(); 
  soclib_timer_port_t timer = & PLATFORM_TIMER_BASE[cpuid];
  /*
   * Acknowledge the timer.
   */
  xputs("on est dans linterrupt");
  cpu_write(UINT32, &(timer -> irq_ack), 1);  //quando temos setamos o ack, nosso timer vai para 0 para comecar o timer da interrupcao dnv?
  cpu_write(UINT32, &(timer -> period), 200000);  //timer period of 2s = 200000

  /*
   * Write a little something..
   */
  xputs("In the interupt, waiting for timer...");

  /*
   * Reset the timer.
   */
  cpu_write(UINT32, &(timer -> value ), 0);
  xputs("on sort de l'interruption");

  return 0;
}
