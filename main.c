#include "uart.h"

int main()
{
	uart_init();
	uart_send_string("Hello world!");

	while(1){};
	
	return 0;
}
