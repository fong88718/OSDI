ENTRY(_start)

SECTIONS
{
  	. = 0x80000;
	.text : 
	{ 
		KEEP(*(.text.boot))
		*(.text*) 
	}
	.rodata :
	{
		*(.rodata*)
	}
	.data :
	{
		*(.data*)
	}
	.bss :
	{
		. = ALIGN(8);
		__bss_start = .;
		*(.bss*)
		. = ALIGN(8);
		__bss_end = .;
	}
	/* AAPCS64 ABI 規定stack pointer要對齊16 */
	. = ALIGN(16);
	__stack_top = . + 0x4000;
}
