#include <Platform/Platform.h>
#include <stdio.h>

uint32_t TTY_DEVICE=0xC1000000;

//static const volatile void* tty_address = "0xC1000000";

void xputs (char * s);
char xgets ();


void system_kickstart (void)
{
  platform_debug_puts ("Hello, World !\r\n");
  xputs ("Hello, World !\r\n");
  for (int i = 0 ;; i++) {
	xgets ();
//    platform_debug_puts ("Boucle: \r\n");
//    soclib_io_set( tty_address, 0, 'c' );
  };

}



