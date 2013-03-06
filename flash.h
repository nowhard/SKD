/*---------------------------------------------------------
FLASH functions.
---------------------------------------------------------*/
//char flash_erase_all (void);

unsigned long flash_size (void);

unsigned long flash_read (
  void *buffer,             /* Buffer to fill */
  unsigned long len,        /* Bytes to read */
  unsigned long address);   /* FLASH address to read from */

unsigned long flash_write (
  const void *buffer,       /* Buffer to save */
  unsigned long len,        /* Buffer length */
  unsigned long address);   /* FLASH address to write to */
unsigned long  flash_erase_page (unsigned long address);
/*---------------------------------------------------------
---------------------------------------------------------*/
