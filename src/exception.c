#include "exception_table.h"
#include "uart0.h"
#include "timer.h"
#include "queue.h"

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64",
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32",
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "ERROR_INVALID_EL0_32",
};

void sync_exception_handler(unsigned long long type, unsigned long long ELR, unsigned long long ESR)
{
    unsigned long long EC, ISS;
    EC = (ESR >> 26) & 0x3F;
    ISS = ESR & 0x1FFFFFF;
    if(EC == 0x15) // system call
    {
        if(ISS == 1)
        {
            uart_printf("\rException return address : 0x%x\n", ELR);
            uart_printf("\rException Class(EC) : 0x%x\n", EC);
            uart_printf("\rInstruction Specific Syndrome(ISS) : 0x%x\n", ISS);
        }
        if(ISS == 2)
        {
            core_timer_enable();
            local_timer_init();
            sys_timer_init();
            arm_timer_init();
        }
    }
    else
    {
        uart_printf("\rException return address : 0x%x\n", ELR);
        uart_printf("\rException Class(EC) : 0x%x\n", EC);
        uart_printf("\rInstruction Specific Syndrome(ISS) : 0x%x\n", ISS);
    }   
}

void show_exception_status(unsigned int type, unsigned long long ELR, unsigned long long ESR)
{
    unsigned long long EC, ISS;
    EC = (ESR >> 26) & 0x3F;
    ISS = ESR & 0x1FFFFFF;
    
    uart_printf("\rException type : %s\n", entry_error_messages[type]);
    uart_printf("\rException return address : 0x%x\n", ELR);
    uart_printf("\rException Class(EC) : 0x%x\n", EC);
    uart_printf("\rInstruction Specific Syndrome(ISS) : 0x%x\n", ISS);
}

void IRQ_handler()
{
    unsigned int src = *CORE0_INTERRUPT_SOURCE;
    if(src & (1 << 11))
        local_timer_handler();
    if(src & (1 << 1))
        core_timer_handler();
    if(src & (1 << 8)) // gpu interrupt
    {
        if(*IRQ_PENDING2 & (1 << 25)) // uart interrupt
            uart0_irq_handler();
        if(*SYSTEM_TIMER_CS & (1 << 1))
            sys_timer_handler();
        if(*IRQ_PENDING0 & (1 << 0))
            arm_timer_hanler();
    }
}

void uart0_irq_handler()
{
    unsigned int mis = *UART0_MIS;
    if(mis & (1 << 4)) // rx interrupt
    {
        while((*UART0_FR & (1 << 4)) == 0 )// rx FIFO isn't empty 
        {
            char c = *UART0_DR;
            queue_push(&rx_buf, c);
        }
    }
    if(mis & (1 << 5)) // tx interrupt
    {
        while(!queue_empty(&tx_buf) && (*UART0_FR & (1<<5)) == 0) // 還有資料要傳 and tx FIFO isn't full
            *UART0_DR = queue_pop(&tx_buf);
        
        tx_active = 0; // 送完
        *UART0_IMSC &= ~(1 << 5); // disable tx interrupt
    }
}