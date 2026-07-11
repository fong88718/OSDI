#include "shell.h"
#include "uart.h"
#include "my_string.h"
#include "util.h"
#include "mailbox.h"

enum ANSI_ESC {
    Unknown,
    CursorForward,
    CursorBackward,
    Delete
};

enum ANSI_ESC decode_csi_key() 
{
    char c = uart_recv();
    if (c == 'C') 
        return CursorForward;
    else if (c == 'D') 
        return CursorBackward;
    else if (c == '3') 
    {
        c = uart_recv();
        if (c == '~') 
            return Delete;
    }
    return Unknown;
}

enum ANSI_ESC decode_ansi_escape() 
{
    char c = uart_recv();
    if (c == '[') 
        return decode_csi_key();
    return Unknown;
}


void shell_init()
{
    uart_init();
    uart_printf("Hello RASPI3b+\r\n");
}

void shell_read(char *cmd)
{
restart:
    uart_printf("\r# ");
    int idx = 0, end = 0;
    cmd[0] = '\0';

    while (1) 
    {
        char c = uart_recv();
        if(c == '\r' || c == '\n')
            break;
        // Decode CSI key sequences
        if (c == 27) 
        {
            enum ANSI_ESC key = decode_ansi_escape();
            switch (key) 
            {
                case CursorForward:
                    if (idx < end) idx++;
                    break;
                case CursorBackward:
                    if (idx > 0) idx--;
                    break;
                case Delete: // left shift command
                    if(idx < end)
                    {
                        for (int i = idx; i < end; i++) 
                            cmd[i] = cmd[i + 1];
                        cmd[--end] = '\0';
                    }
                    
                    break;
                case Unknown:
                    uart_flush();
                    break;
            }
        }
        // CTRL-C
        else if (c == 3) 
        {
            uart_printf("\r\n");
            goto restart;
        }
        // Backspace
        else if (c == 8 || c == 127) 
        {
            if (idx > 0) 
            {
                idx--;
                // left shift command
                for (int i = idx; i < end; i++) 
                    cmd[i] = cmd[i + 1];
                cmd[--end] = '\0';
            }
        }
        else 
        {
            // normal char
            if (idx < end) 
            {
                for (int i = end; i > idx; i--) 
                    cmd[i] = cmd[i - 1];
            }
            cmd[idx++] = c;
            cmd[++end] = '\0';
        }
        uart_printf("\r\e[2K# %s\r\e[%dC", cmd, idx+2);
    }
    uart_printf("\r\n");
}

void shell_parse(char *cmd)
{
    if(strcmp(cmd, "hello") == 0)
        uart_printf("Hello World!\n");
    else if(strcmp(cmd, "help") == 0)
    {
        uart_printf("command      | description\n");
        uart_printf("-------------------------------------\n");
        uart_printf("help         | print all available commands\n");
        uart_printf("hello        | print Hello World!\n");
        uart_printf("timestamp    | get current timestamp\n");
        uart_printf("reboot       | reboot raspi3b+\n");
    }
    else if(strcmp(cmd, "timestamp") == 0)
    {
        uart_printf("[%f]\n", getTimeStamp());
    }
    else if(strcmp(cmd, "reboot") == 0)
    {
        uart_printf("rebooting...\n");
        reset(100);
        while(1)
            asm volatile("nop");
    }
    else if(strcmp(cmd, "test") == 0)
    { 
        get_board_revision();
    }
    else
    {
        uart_printf("ERR : command \"%s\" not found, try <help>\n", cmd);
    } 
}