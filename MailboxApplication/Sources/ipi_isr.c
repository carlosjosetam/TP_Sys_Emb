#include <Private/Itoa.h>

#include <Platform/Platform.h>
#include <Processor/Processor.h>

int32_t ipi_isr (void * data)
{
  char buffer[32];
   int32_t command = 0; // value,
			
  /*
   * Reading command and value fields.
   */
   //cpu_read(UINT32, data
  

  /*
   * Reseting the Mailbox.
   */

  

  /*
   * Write info.
   */

  platform_debug_puts ("Processeur: ");
  itoa (cpu_mp_id (), buffer);
  platform_debug_puts (buffer);
  platform_debug_puts (" received IPI: ");
  itoa (command, buffer);
  platform_debug_puts (buffer);
  platform_debug_puts ("\r\n");

  return 0;
}
