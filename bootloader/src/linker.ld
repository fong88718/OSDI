ENTRY(_relocate)

SECTIONS
{
  	. = 0x80000;
	.relocate :
	{
		KEEP(*(.text.relocate))
	}

	. = ALIGN(4096);
	_begin = .;
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
	_end = .;
}
_boot_loader = 0x3B3FC000; /*VC Core Base 位址: 0x3B400000 - 16KB*/