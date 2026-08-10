#include "exception_table.h"
#include "uart0.h"
#include "timer.h"

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
    if(ISS == 3)
    {
        int currEL = -1;
        asm volatile("mrs %0, currentEL" : "=r"(currEL));
        uart_printf("\rcurrent EL = %d\n", currEL >> 2);
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