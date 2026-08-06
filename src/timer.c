#include "timer.h"
#include "uart0.h"


void arm_timer_init()
{
    *ARM_TIMER_CONTROL = (1 << 7) | (1 << 5) | (1 << 1);
    *ARM_TIMER_LOAD = 5000000;
    *IRQ_ENABLE2 = 1;
}

void arm_timer_hanler()
{
    static int jiffies4 = 0;
    uart_printf("\rarm timer interrupt, jiffies %d\n", jiffies4++);
    *ARM_TIMER_IRQ_CLR = 1;
}

void sys_timer_init()
{
    
    unsigned int t;
    t = *SYSTEM_TIMER_CLO;
    *SYSTEM_TIMER_COMPARE1 = t + 2500000;
    *IRQ_ENABLE0 = 1 << 1;
}

void sys_timer_handler()
{
    static int jiffies3 = 0;
    uart_printf("\rsystem timer interrupt, jiffies %d\n", jiffies3++);
    unsigned int t;
    t = *SYSTEM_TIMER_CLO;
    *SYSTEM_TIMER_COMPARE1 = t + 2500000;
    *SYSTEM_TIMER_CS = 0xF;
}


void core_timer_enable()
{
    int x = 1;
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(x)); // enable timer
    asm volatile("msr cntp_tval_el0, %0" :: "r"(EXPIRE_PERIOD)); // set expired time
    *CORE0_TIMER_IRQ_CTRL = 2; // enable timer interrupt
}
    
void core_timer_handler()
{
    static int jiffies1 = 0;
    
    asm volatile("msr cntp_tval_el0, %0" :: "r"(EXPIRE_PERIOD));
    uart_printf("\rcore timer interrupt, jiffies %d\n", jiffies1++);

    // bottom half simulation
    
    // asm volatile("msr DAIFclr, #2"); // enable irq
    // volatile unsigned long long  x = 0;
    // while(x++ < 10000000);
        
}

void local_timer_init()
{
    unsigned int flag = 0x30000000; // enable timer and interrupt.
    unsigned int reload = 250000000;
    *LOCAL_TIMER_CONTROL_REG = flag | reload;
}

void local_timer_handler()
{
    static int jiffies2 = 0;
    *LOCAL_TIMER_IRQ_CLR =  0xc0000000; // clear interrupt and reload.

    

    uart_printf("\rlocal timer interrupt, jiffies %d\n", jiffies2++);
}

void IRQ_handler()
{
    unsigned int src = *CORE0_INTERRUPT_SOURCE;
    if(src & (1 << 11))
        local_timer_handler();
    if(src & (1 << 1))
        core_timer_handler();
    if(src & (1 << 8))
    {
        if(*SYSTEM_TIMER_CS & (1 << 1))
            sys_timer_handler();
        if(*IRQ_PENDING0 & (1 << 0))
            arm_timer_hanler();
    }
}