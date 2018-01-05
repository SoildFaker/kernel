/* kernel_link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(kmain)
SECTIONS
{
  . = 0xC0100000;
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

  .stab :
  {
    *(.stab)
    . = ALIGN(4096);
  }

  .stabstr :
  {
   *(.stabstr)
   . = ALIGN(4096);
  }

  PROVIDE( kernel_end = . );
  /DISCARD/ : { *(.comment) *(.eh_frame) }
}
