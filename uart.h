#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send(char c);
char uart_recv();

void uart_send_string(char *s);
// char* uart_recv_string();
void delay(int time);


#endif