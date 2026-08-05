#ifndef TIMER_H
#define TIMER_H
#include "mmio.h"

#define EXPIRE_PERIOD               0x7ffffff

#define CORE0_TIMER_IRQ_CTRL        ((volatile unsigned int*)(0x40000040))
#define CORE0_INTERRUPT_SOURCE      ((volatile unsigned int*)(0x40000060))

#define LOCAL_TIMER_CONTROL_REG     ((volatile unsigned int*)(0x40000034))
#define LOCAL_TIMER_IRQ_CLR         ((volatile unsigned int*)(0x40000038))

#define SYSTEM_TIMER_BASE           0x3000

#define SYSTEM_TIMER_CS             ((volatile unsigned int*)(MMIO_BASE + SYSTEM_TIMER_BASE + 0x00))
#define SYSTEM_TIMER_CLO            ((volatile unsigned int*)(MMIO_BASE + SYSTEM_TIMER_BASE + 0x04))
#define SYSTEM_TIMER_COMPARE1       ((volatile unsigned int*)(MMIO_BASE + SYSTEM_TIMER_BASE + 0x10))

#define INTERRUPT_BASE              0xB000
#define IRQ_ENABLE0                 ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x210))      


#define ARM_TIMER_LOAD              ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x400)) 
#define ARM_TIMER_CONTROL           ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x408)) 
#define ARM_TIMER_IRQ_CLR           ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x40c)) 
#define IRQ_ENABLE2                 ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x218)) 
#define IRQ_PENDING0                ((volatile unsigned int*)(MMIO_BASE + INTERRUPT_BASE + 0x200))  

void local_timer_init();
void local_timer_handler();
void core_timer_enable();
void core_timer_handler();
void IRQ_handler();
void sys_timer_init();
void sys_timer_handler();
void arm_timer_init();
void arm_timer_hanler();
#endif