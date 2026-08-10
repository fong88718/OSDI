#ifndef UART0_H
#define UART0_H

#include "mmio.h"
#include "queue.h"

#define UART0_BASE   (MMIO_BASE + 0x201000)

#define UART0_DR     ((volatile unsigned int*)(UART0_BASE + 0x0))
#define UART0_FR     ((volatile unsigned int*)(UART0_BASE + 0x18))
#define UART0_IBRD   ((volatile unsigned int*)(UART0_BASE + 0x24))
#define UART0_FBRD   ((volatile unsigned int*)(UART0_BASE + 0x28))
#define UART0_LCRH   ((volatile unsigned int*)(UART0_BASE + 0x2c))
#define UART0_CR     ((volatile unsigned int*)(UART0_BASE + 0x30))

#define UART0_IMSC   ((volatile unsigned int*)(UART0_BASE + 0x38))
#define UART0_MIS    ((volatile unsigned int*)(UART0_BASE + 0x40))
#define UART0_ICR    ((volatile unsigned int*)(UART0_BASE + 0x44))

#define Q_SIZE 1024
extern Queue tx_buf, rx_buf;
extern volatile int tx_active;

void uart_init(void);
void uart_send(char c);
char uart_recv_raw(void);
char uart_recv();
void uart_flush();
void uart_printf(char *fmt, ...); 
void uart0_irq_handler();
#endif