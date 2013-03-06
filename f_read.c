#include "ADUC847.H"
#include "f_adi.h"

/*---------------------------------------------------------
---------------------------------------------------------*/
//#pragma NOAR
unsigned long flash_read (
  void *buffer,            /* Buffer to fill */
  unsigned long len,       /* Bytes to read */
  unsigned long address)   /* FLASH address to read from */
{
unsigned char *s = buffer;
unsigned long i;
unsigned int addr;

/*-------------------------------------
If the address is too big, exit.
--------------------------------------*/
if (address >= ADI_EEMEM_SIZE)
  return (0UL);

/*-------------------------------------
Otherwise, read the starting page.
--------------------------------------*/
addr = (unsigned int) address;

EADRL = addr >> 2;
ECON = ADI_EE_READ_PAGE;

for (i = 0; i < len; i++)
  {

/*-------------------------------------
Read the appropriate EE byte.
--------------------------------------*/
  switch (addr & 0x03)
    {
    case 0: s[i] = EDATA1; break;
    case 1: s[i] = EDATA2; break;
    case 2: s[i] = EDATA3; break;
    case 3: s[i] = EDATA4; break;
    }
  

  if (++addr >= ADI_EEMEM_SIZE)
    break;

  if ((addr & 0x03) == 0)
    {
    EADRL = addr >> 2;
    ECON = ADI_EE_READ_PAGE;
    }
  }

return (i);
}

/*---------------------------------------------------------
---------------------------------------------------------*/
