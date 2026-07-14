#ifndef UART1_H
#define UART1_H
#include "mmio.h"

void uart1_init(void);
void uart1_send(char c);
char uart1_recv();

void uart1_flush();
void uart1_printf(char *fmt, ...); 

#endif