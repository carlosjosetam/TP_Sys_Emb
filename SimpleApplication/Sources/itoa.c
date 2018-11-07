#include <stdint.h>
#include <stdbool.h>

int32_t itoa (int32_t integer, char * buffer)
{
  char ascii[64];
  int32_t value = integer, index = 0, rem = 0;
  bool negative = false;

  if(value < 0)
  {
    value = -1 * value;
    negative = true;
  }

  do
  {
    rem = value % 10;
    value = value / 10;
    ascii[index ++] = rem + '0';
  }
  while(value > 0);

  if (negative) ascii[index ++] = '-';

  for(int32_t i = index - 1; i >= 0; i--) *buffer ++ = ascii[i];
  *buffer = '\0';

  return integer;
}

