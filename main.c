#include "uart.h"

int main()
{
	uart_init();
	uart_send('O');
	uart_send('K');
	

	while(1)
	{
		uart_send(uart_recv());
	};
	
	return 0;
}
