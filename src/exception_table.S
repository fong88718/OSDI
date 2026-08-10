#include "exception_table.h"

// Simple vector table
.align 11 // vector table should be aligned to 0x800

.macro E_handler_entry type
    save_all
    mov x0, #\type
    mrs x1, ELR_EL1
    mrs x2, ESR_EL1
    bl show_exception_status
    load_all
    eret
.endm

.macro ventry label
    b \label
    .align 7
.endm

.macro save_all
    sub sp, sp, 34 * 8
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]
    str x30, [sp, 16 * 15]
    
    mrs x0, ELR_EL1
    mrs x1, SPSR_EL1
    stp x0, x1, [sp, 16 * 16]
.endm

    // load general registers from stack
.macro load_all
    // 防止復原的時候被interrupt
    msr DAIFset, #2
    isb

    ldp x0, x1, [sp ,16 * 16]
    msr ELR_EL1, x0
    msr SPSR_EL1, x1

    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    ldr x30, [sp, 16 * 15]
    add sp, sp, 34 * 8
.endm

.section .text

.align 11
.global exception_table

exception_table:
    ventry L_SYNC_CURRENT_SP0
    ventry L_IRQ_CURRENT_SP0
    ventry L_FIQ_CURRENT_SP0
    ventry L_SERROR_CURRENT_SP0

    ventry L_SYNC_CURRENT_SPX
    ventry L_IRQ_CURRENT_SPX
    ventry L_FIQ_CURRENT_SPX
    ventry L_SERROR_CURRENT_SPX

    ventry L_SYNC_LOWER_AARCH64
    ventry L_IRQ_LOWER_AARCH64
    ventry L_FIQ_LOWER_AARCH64
    ventry L_SERROR_LOWER_AARCH64

    ventry L_SYNC_LOWER_AARCH32
    ventry L_IRQ_LOWER_AARCH32
    ventry L_FIQ_LOWER_AARCH32
    ventry L_SERROR_LOWER_AARCH32


L_SYNC_CURRENT_SP0:
    E_handler_entry SYNC_CURRENT_SP0
L_IRQ_CURRENT_SP0:
    E_handler_entry IRQ_CURRENT_SP0
L_FIQ_CURRENT_SP0:
    E_handler_entry FIQ_CURRENT_SP0
L_SERROR_CURRENT_SP0:
    E_handler_entry SERROR_CURRENT_SP0

L_SYNC_CURRENT_SPX:
    b SYNC_HANDLER
L_IRQ_CURRENT_SPX:
    b IRQ_HANDLER
L_FIQ_CURRENT_SPX:
    E_handler_entry FIQ_CURRENT_SPX
L_SERROR_CURRENT_SPX:
    E_handler_entry SERROR_CURRENT_SPX

L_SYNC_LOWER_AARCH64:
    b SYNC_HANDLER
L_IRQ_LOWER_AARCH64:
    b IRQ_HANDLER
L_FIQ_LOWER_AARCH64:
    E_handler_entry FIQ_LOWER_AARCH64
L_SERROR_LOWER_AARCH64:
    E_handler_entry SERROR_LOWER_AARCH64

L_SYNC_LOWER_AARCH32:
    E_handler_entry SYNC_LOWER_AARCH32
L_IRQ_LOWER_AARCH32:
    E_handler_entry IRQ_LOWER_AARCH32
L_FIQ_LOWER_AARCH32:
    E_handler_entry FIQ_LOWER_AARCH32
L_SERROR_LOWER_AARCH32:
    E_handler_entry SERROR_LOWER_AARCH32
    

SYNC_HANDLER:
    save_all
    mrs x1, ELR_EL1
    mrs x2, ESR_EL1
    bl sync_exception_handler
    load_all
    eret

IRQ_HANDLER:
    save_all
    bl IRQ_handler
    load_all
    eret

