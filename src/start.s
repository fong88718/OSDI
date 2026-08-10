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
	// 允許 EL0 讀 CNTPCT/CNTFRQ,否則 getTimeStamp 會 trap
    mrs x0, CNTKCTL_EL1
    orr x0, x0, #1            // EL0PCTEN = 1 (bit0)
    msr CNTKCTL_EL1, x0

	// enable FP in EL0 and EL1
	mrs x0, cpacr_el1
	orr x0, x0, #(3 << 20)
	msr cpacr_el1, x0
	isb

	// set HCR_EL2.RW to specify EL1 running at aarch64
	mrs x0, HCR_EL2
	orr x0, x0, #(1 << 31)
	msr HCR_EL2, x0

	// EL1h (SPSel = 1) with interrupt disabled
	mov x0, #0x3c5 
	msr SPSR_EL2, x0 

	// load exception return address
	adr x0, EL1_start 
	msr elr_el2, x0

	// return to EL1
	eret 

EL1_start:
	// init sp for el1
	mov sp, #0x80000

	// set exception table
	ldr x0, =exception_table
	msr VBAR_EL1, x0

	// enable irq interrupt
	msr DAIFclr, #2

	// EL0 with interrupt enabled
	mov x0, 0 
	msr spsr_el1, x0

	// load exception return address
	ldr x0, =EL0_start
	msr ELR_EL1, x0

	// switch to EL0
	eret
	// halt this core if return
	b secondary
	

EL0_start:
	// set stack pointer
	mov sp, #0x60000
	bl main
	b secondary