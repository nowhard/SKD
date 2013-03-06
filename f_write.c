#include "ADUC847.H"
#include "f_adi.h"

/*---------------------------------------------------------
---------------------------------------------------------*/
unsigned long flash_write (
  const void *buffer,      /* Buffer to save */
  unsigned long len,       /* Buffer length */
  unsigned long address)   /* FLASH address to write to */
{
const unsigned char *s = buffer;
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
Write the appropriate EE byte.
--------------------------------------*/
  switch (addr & 0x03)
    {
    case 0: EDATA1 = s[i]; break;
    case 1: EDATA2 = s[i]; break;
    case 2: EDATA3 = s[i]; break;
    case 3: EDATA4 = s[i]; break;
    }
  
  ++addr;

  if ((addr & 0x03) == 0)
    {
    ECON = ADI_EE_ERASE_PAGE;
    ECON = ADI_EE_WRITE_PAGE;
    }

  if (addr >= ADI_EEMEM_SIZE)
    break;

  if ((addr & 0x03) == 0)
    {
    EADRL = addr >> 2;
    ECON = ADI_EE_READ_PAGE;
    }
  }

if ((addr & 0x03) != 0)
  {
  ECON = ADI_EE_ERASE_PAGE;
  ECON = ADI_EE_WRITE_PAGE;
  }

return (i);
}

/*---------------------------------------------------------
---------------------------------------------------------*/
