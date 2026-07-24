#include "uart1.h"
#include "auxi.h"
#include "gpio.h"
#include "my_string.h"
#include "util.h"

void uart1_init(void) 
{
    /* Initialized GPIO */
    // GPIO pins should be set up first before enable UART. (Page 10)

    // 1. Change GPIO 14, 15 to alternate function 5
    unsigned int r = *GPFSEL1;
    r &= ~((7 << 12) | (7 << 15));          // Reset GPIO 14, 15
    r |= (2 << 12) | (2 << 15);             // Set GPIO 14, 15 to ALT 5, i.e. mini UART 
    *GPFSEL1 = r;                          
    // 2. Disable GPIO pull up/down (Because these GPIO pins use alternate functions, not basic input-output)
    // Set control signal to disable
    *GPPUD = 0;                             // disable pull-up/down control
    delay(150);                             // delay 150 cycles
    *GPPUDCLK0 = (1 << 14) | (1 << 15);     // apply to pin 14, 15
    delay(150);                             // delay 150 cycles
    *GPPUD = 0;                             // clear register
    *GPPUDCLK0 = 0;                         // clear register


    /* Initialize mini UART */
    *AUX_ENABLES |=0x1;         // enable mini UART
    *AUX_MU_CNTL_REG = 0;       // disable tx/rx during configuration
    *AUX_MU_IER_REG = 0;        // disable mini UART interrupt
    *AUX_MU_IIR_REG = 6;        // clear tx/rx FIFO
    *AUX_MU_LCR_REG = 3;        // set 8-bit mode
    *AUX_MU_BAUD_REG = 270;     // set baudrate to 115200
    *AUX_MU_CNTL_REG = 3;       // enable tx/rx
}

void uart1_send(char c)
{
    if(c == '\n') 
        uart1_send('\r');
    while((*AUX_MU_LSR_REG & (1 << 5)) == 0) // tx queue is full
        asm volatile("nop");
    *AUX_MU_IO_REG = c;
}

char uart1_recv(void) 
{
    while((*AUX_MU_LSR_REG & 1) == 0) // rx queue is full
        asm volatile("nop");
    char c = (char)*AUX_MU_IO_REG;
    return c == '\r' ? '\n' : c;
}



void uart1_flush() 
{
    while (*AUX_MU_LSR_REG & 0x01) 
        *AUX_MU_IO_REG;
}

void uart1_printf(char *fmt, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    char output[100], *s = output;
    vsprintf(s, fmt, args);
    while(*s != 0)
        uart1_send(*s++);
}