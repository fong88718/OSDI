ENTRY(_start)

SECTIONS
{
  	. = 0x80000;
	.text : 
	{ 
		KEEP(*(.text.boot))
		*(.text) 
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
	/* 每次function call都要存frame pointer 跟return addr這兩個pointer, 1個pointer 8byte所以要對其16 */
	. = ALIGN(16);
	__stack_top = . + 0x4000
}
