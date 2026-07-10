#ifndef UART_H
#define UART_H
#include "mmio.h"
void uart_init(void);
void uart_send(char c);
char uart_recv();
void uart_send_string(char *s);
void delay(int time);
void uart_flush();

#endif