/* user_link.ld -- Linker script for the users - ensure everything goes in the */

ENTRY(main)
SECTIONS
{
    . = 0x0;

    .text :
    {
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        *(.rodata*)
        . = ALIGN(4096);
    }

    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
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

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
