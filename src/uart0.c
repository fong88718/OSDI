#include "uart0.h"
#include "mailbox.h"
#include "gpio.h"
#include "my_string.h"
#include "util.h"



void uart_init(void) 
{
    // disable UART
    *UART0_CR = 0; 

    unsigned int __attribute__((aligned(16))) mailbox[9];
    mailbox[0] = 9 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = SET_UART_CLOCK_RATE; // tag identifier
    mailbox[3] = 12; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = UART_ID; // 
    mailbox[6] = UART_RATE; // set 4MHz
    mailbox[7] = 0; // clear turbo
    // tags end
    mailbox[8] = END_TAG;

    mailbox_call(mailbox, 8); // message passing procedure call, you should implement it following the 6 steps provided above.


    /* Initialized GPIO */
    // GPIO pins should be set up first before enable UART. (Page 10)

    // 1. Change GPIO 14, 15 to alternate function 0
    unsigned int r = *GPFSEL1;
    r &= ~((7 << 12) | (7 << 15));          // Reset GPIO 14, 15
    r |= (4 << 12) | (4 << 15);             // Set GPIO 14, 15 to ALT 0, i.e. UART 
    *GPFSEL1 = r;                          
    // 2. Disable GPIO pull up/down (Because these GPIO pins use alternate functions, not basic input-output)
    // Set control signal to disable
    *GPPUD = 0;                             // disable pull-up/down control
    delay(150);                             // delay 150 cycles
    *GPPUDCLK0 = (1 << 14) | (1 << 15);     // apply to pin 14, 15
    delay(150);                             // delay 150 cycles
    *GPPUD = 0;                             // clear register
    *GPPUDCLK0 = 0;                         // clear register



    /* Initialize UART */
    *UART0_IBRD = 0x2;                   // set 115200 Baudrate
    *UART0_FBRD = 0xB;                   // set 115200 Baudrate
    *UART0_LCRH = 0b11 << 5;             // 8 bit mode
    *UART0_CR = 1 | (1 << 8) | (1 << 9); // enable, tx, rx
}

void uart_send(char c)
{
    while((*UART0_FR & (1 << 5))) // tx queue is full
        asm volatile("nop");
    *UART0_DR = c;
}

char uart_recv(void) 
{
    while(*UART0_FR & (1 << 4)) // rx queue is empty
        asm volatile("nop");
    char c = (char)*UART0_DR;
    return c == '\r' ? '\n' : c;
}

void uart_flush() 
{
    while((*UART0_FR & (1 << 4)) == 0)
        *UART0_DR;
}

void uart_printf(char *fmt, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    char output[200], *s = output;
    vsprintf(s, fmt, args);
    while(*s != 0)
        uart_send(*s++);
}