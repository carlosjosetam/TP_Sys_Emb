#include <stdint.h>
//#include <string>

extern uint32_t TTY_DEVICE;


char xgets ()
{
  volatile char * tty_port = (volatile char *) TTY_DEVICE;

  char s;
	s = *tty_port;
	return s;
  for (int32_t i = 0; *tty_port != '\0'; i += 1)
  {
  ///  s.push_back(*tty_port);
    tty_port++;
  }
  return s;
}
