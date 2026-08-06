#include "loadimg.h"
#include "uart0.h"

enum ANSI_ESC {
    Unknown,
    CursorForward,
    CursorBackward,
    Delete
};

static enum ANSI_ESC decode_csi_key() 
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

static enum ANSI_ESC decode_ansi_escape() 
{
    char c = uart_recv();
    if (c == '[') 
        return decode_csi_key();
    return Unknown;
}

long long address_input()
{
    uart_printf("\rPlease enter kernel loaded address (default : 0x80000) : 0x");
    char cmd[128];
    int idx = 0, end = 0;
    cmd[0] = 0;

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
            uart_printf("\n");
           return -1;
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
            // if(idx >= 8 || end >= 8)
            //     continue;
            // normal char
            if (idx < end) 
            {
                for (int i = end; i > idx; i--) 
                    cmd[i] = cmd[i - 1];
            }
            cmd[idx++] = c;
            cmd[++end] = '\0';
        }
        uart_printf("\r\e[2KPlease enter kernel loaded address (default : 0x80000) : 0x%s\r\e[%dC", cmd, idx+59);
    }
    long long address = 0;
    
    for(int i = 0 ; cmd[i] != 0 ; i++)
    {   
        int val;
        if(cmd[i] >= 'A' && cmd[i] <= 'F')
            val = cmd[i] - 'A' + 10;
        else if(cmd[i] >= 'a' && cmd[i] <= 'f')
            val = cmd[i] - 'a' + 10;
        else if(cmd[i] >= '0' && cmd[i] <= '9')
            val = cmd[i] - '0';
        else    
        {
            uart_printf("address error\n");
            return -1;
        }
        address = (address << 4) + val;
    }
        
    if(address == 0)
    {
        uart_printf("\r\e[2KPlease enter kernel loaded address (default : 0x80000) : 0x80000");
        address = 0x80000;
    }
    uart_printf("\n");
    return address;
}



void init_loadimg()
{
    long long _new_kernel_addr = address_input();
    if(_new_kernel_addr < 0)
        return;

    uart_printf("\rstart sending by uart!\n");

    unsigned int img_size = 0;
    for(int i = 0 ; i < 4 ; i++)
        img_size = img_size << 8 | uart_recv_raw();
    unsigned int check_sum = 0;
    for(int i = 0 ; i < 4 ; i++)
        check_sum = check_sum << 8 | uart_recv_raw();
  
    unsigned char* kernel = (unsigned char*)_new_kernel_addr;
    for(int i = 0 ; i < img_size ; i++)
    {
        unsigned char c = uart_recv_raw();
        *(kernel + i) = c;
        check_sum -= c;
    }
    if(check_sum != 0)
    {
        uart_printf("\rload image failed!\n");
        return;
    }
    void (*start_os)(void) = (void*)kernel;
    start_os();
}
