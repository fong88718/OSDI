#include "shell.h"
#include "uart.h"
#include "my_string.h"
#include "util.h"

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
}

void shell_read(char *cmd)
{
    uart_send_string("\r# ");

    int idx = 0, end = 0;
    cmd[0] = '\0';
    char c;
    while ((c = uart_recv()) != '\n') 
    {
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
                case Delete:
                    // left shift command
                    for (int i = idx; i < end; i++) 
                        cmd[i] = cmd[i + 1];
                    cmd[--end] = '\0';
                    break;
                case Unknown:
                    uart_flush();
                    break;
            }
        }
        // CTRL-C
        else if (c == 3) 
        {
            cmd[0] = '\0';
            break;
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
            // right shift command
            if (idx < end) 
            {
                for (int i = end; i > idx; i--) 
                    cmd[i] = cmd[i - 1];
            }
            cmd[idx++] = c;
            cmd[++end] = '\0';
        }
        uart_send_string(cmd);
        uart_send_string("\n");
    }
    uart_send_string("\n");
}

void shell_parse(char *cmd)
{
    if(strcmp(cmd, "hello") == 0)
        uart_send_string("Hello World!\n");
    else if(strcmp(cmd, "help") == 0)
    {
        uart_send_string("command      | description\n");
        uart_send_string("-------------------------------------\n");
        uart_send_string("help         | print all available commands\n");
        uart_send_string("hello        | print Hello World!\n");
        uart_send_string("timestamp    | get current timestamp\n");
        uart_send_string("reboot       | reboot raspi3b+\n");
    }
    else if(strcmp(cmd, "timestamp") == 0)
    {
        double time = getTimeStamp();
        char res[50];
        strcpy(res, dtoa(time, res));
        uart_send_string("[");
        uart_send_string(res);
        uart_send_string("]");
        uart_send('\n');

    }
    else if(strcmp(cmd, "reboot") == 0)
    {
        uart_send_string("rebooting...\n");
        reset(100);
        while(1)
            asm volatile("nop");
    }
    else
    {
        uart_send_string("ERR : command \"");
        uart_send_string(cmd);
        uart_send_string("\" not found, try <help>\n");
    } 
}