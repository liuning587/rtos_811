SECTIONS
{
	. = 0x20000000;
	. = ALIGN(2);
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
		*(.rodata*)
	    . = ALIGN(4);
    }
    .data :
    {
        *(.data*)
		. = ALIGN(4);
    }
    .bss :
    {
        _bss = .;
        *(.bss*)
		*(COMMON)
	   . = ALIGN(4);
        _ebss = .;
    }
}
