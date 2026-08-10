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
	// set stack pointer
	ldr x0, =__stack_top
	mov sp, x0

	// 允許 EL0 讀 CNTPCT/CNTFRQ,否則 getTimeStamp 會 trap
    mrs x0, CNTKCTL_EL1
    orr x0, x0, #1            // EL0PCTEN = 1 (bit0)
    msr CNTKCTL_EL1, x0

	// enable FP in EL0 and EL1
	mrs x0, cpacr_el1
	orr x0, x0, #(3 << 20)
	msr cpacr_el1, x0
	isb

	// load exception_table to VBAR_EL2
	ldr x0, =exception_table
	msr VBAR_EL2, x0

	// set HCR_EL2.IMO to routing exception to EL2
	// mrs x0, HCR_EL2
	// orr x0, x0, #(1 << 4)
	// msr HCR_EL2, x0

	// set HCR_EL2.RW to specify EL1 is running at aarch64
	mrs x0, HCR_EL2
	orr x0, x0, #(1 << 31)
	msr HCR_EL2, x0

	mov x0, #0x3c5 // EL1h (SPSel = 1) with interrupt disabled
	msr SPSR_EL2, x0 
	adr x0, rest_initialization // load exception return address
	msr elr_el2, x0
	adr x0, __stack_top // init sp for el1 option 1
	msr sp_el1, x0
	eret // return to EL1

rest_initialization:
	ldr x0, =exception_table
	msr VBAR_EL1, x0


	// enable irq interrupt
	msr DAIFclr, #2

	

	// jump to main function in c
	bl from_el1_to_el0
	// halt this core if return
	b secondary
	

from_el1_to_el0:

	ldr x0, =__user_stack_top
	msr sp_el0, x0
	mov x0, 0 // EL0 with interrupt enabled
	msr spsr_el1, x0
	adr x0, main // return to shell run in EL0
	msr elr_el1, x0
	eret


