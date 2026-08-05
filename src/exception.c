#include "exception_table.h"
#include "uart0.h"

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
    uart_printf("\rException return address : 0x%x\n", ELR);
    uart_printf("\rException Class(EC) : 0x%x\n", EC);
    uart_printf("\rInstruction Specific Syndrome(ISS) : 0x%x\n", ISS);
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