/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

/*ENTRY(start)*/
SECTIONS
{

  /*. = 0x0000;*/
  PROVIDE( kernel_start = . );
  .text :
  {
    *(.text)
    *(.rodata*)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss)
  }
  PROVIDE( kernel_end = . );
  PROVIDE( kernel_size = kernel_end - kernel_start );

}
