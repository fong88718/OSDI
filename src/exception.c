#include "exception_table.h"
#include "uart0.h"

void required_1()
{

    unsigned long long ESR, ELR;
    asm volatile("mrs %0, esr_el2" : "=r"(ESR));
    asm volatile("mrs %0, elr_el2" : "=r"(ELR));
    unsigned long long EC, ISS;
    EC = (ESR >> 26) & 0x3F;
    ISS = ESR & 0x1FFFFFF;
    uart_printf("\rException return address : 0x%x\n", ELR);
    uart_printf("\rException Class(EC) : 0x%x\n", EC);
    uart_printf("\rInstruction Specific Syndrome(ISS) : 0x%x\n", ISS);
    
}