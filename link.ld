/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

/*ENTRY(start)*/
SECTIONS
{
  . = 0x00100000;
  PROVIDE( kernel_start = . );
  .text :
  {
    *(.text)
  }

  .data :
  {
    *(.data)
    *(.rodata*)
  }

  .bss :
  {
    *(.bss)
  }
  PROVIDE( kernel_end = . );

}
