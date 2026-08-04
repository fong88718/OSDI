.section .text.boot
.global _start

_start:
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
	// enable FP in EL0 and EL1
	mrs x0, cpacr_el1
	orr x0, x0, #(3 << 20)
	msr cpacr_el1, x0
	isb

	// load exception_table to VBAR_EL2
	ldr x0, =exception_table
	msr VBAR_EL2, x0

	

	// set stack pointer
	ldr x0, =__stack_top
	mov sp, x0

	// jump to main function in c
	bl main
	// halt this core if return
	b secondary
	


