.section .text.relocate
.global _relocate

_relocate:
	// get cpu ID
	mrs x0, mpidr_el1
	and x0, x0, #3
	cbz x0, clear_bss
	// if cpu ID > 0, stop
	// if cpu ID == 0, clear bss, set stack pointer and jump to main function in c

secondary:
	wfe
	b secondary

clear_bss:
	ldr x0, =__bss_start
	ldr x1, =__bss_end
1:
	cmp x0, x1
	b.ge clear_done
	str xzr, [x0], #8
	b 1b	

clear_done:
	// set stack pointer
	ldr x0, =_boot_loader
	mov sp, x0

	// jump to main function in c
	bl relocate
	// halt this core if return
	b secondary

.section .text.boot
.global _start

_start:
	bl main
1:
	wfe
	b 1b
	


