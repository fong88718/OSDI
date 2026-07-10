#include "uart.h"
#include "shell.h"
#include "my_string.h"

#define CMD_SIZE 128

enum shellStatus {
	Read,
	Parse
};

int main()
{
	shell_init();
	uart_send_string("Hello RASPI3b+\n");
	char cmd[CMD_SIZE];
	enum shellStatus status = Read;
	while(1)
	{
		switch(status)
		{
			case Read : 
				shell_read(cmd);
				status = Parse;
				break;
			case Parse :
				shell_parse(cmd);
				status = Read;
				break;
			default :
				break;
		}
	};
	
	return 0;
}
