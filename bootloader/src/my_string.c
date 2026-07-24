#include "my_string.h"

int strcmp(const char *a, const char *b)
{
    while(*a)
    {
        if(*a != *b)
            break;
        a++;
        b++;
    }
    return *a - *b;
}

char* strcat(char *dest, const char *src)
{
    char *cur = dest;
    while(*cur != 0)
        cur++;
    while((*cur++ = *src++) != 0);
    return dest;
}
char* strcpy(char *dest, const char *src)
{
    char *cur = dest;
    while((*cur++ = *src++) != 0);
    return dest;
}


char* itoa(int n, char *s)
{
    int i = 0;
    if(n < 0)
    {
        s[i++] = '-';
        n = -n;
    }
    while(1)
    {
        s[i++] = n % 10 + '0';
        n /= 10;
        if(n == 0)
            break;
    }
    s[i] = 0;

    for(int st = (s[0] == '-'), ed = i-1; st < ed ; st++, ed--)
    {
        char tmp = s[st];
        s[st] = s[ed];
        s[ed] = tmp;
    }
    return s;
}

char *dtoa(double value, char *buffer)
{
    const unsigned int precision = 7;
    const unsigned long long scale = 10000000ULL;

    char *result = buffer;
    char *out = buffer;

    if (value < 0.0) 
    {
        *out++ = '-';
        value = -value;
    }

    unsigned long long integer_part = value;
    double fraction = value - (double)integer_part;
    unsigned long long fraction_part = fraction * (double)scale + 0.5; // rounding

    /* 處理小數四捨五入後向整數進位 */
    if (fraction_part >= scale) {
        integer_part++;
        fraction_part = 0;
    }

    char integer_buffer[32];
    int integer_length = 0;
  
    do
    {
        integer_buffer[integer_length++] = '0' + integer_part % 10;
        integer_part /= 10;
    } while(integer_part > 0);

    while (integer_length > 0) 
        *out++ = integer_buffer[--integer_length];
    *out++ = '.';

    if (fraction_part == 0) 
    {
        *out++ = '0';
        *out = '\0';
        return result;
    }

    unsigned long long divisor = scale / 10;

    for (unsigned int i = 0; i < precision; i++) 
    {
        *out++ = '0' + fraction_part / divisor;
        fraction_part %= divisor;
        if (fraction_part == 0) 
            break;
        divisor /= 10;
    }

    *out = '\0';
    return result;
}

char* hextoa(unsigned int arg, char *buf)
{
    unsigned char *byte = (unsigned char*)&arg;
    char *p = buf;

    for(int i = 0 ; i < 4 ; i++)
    {
        int low = *byte & 0xF;
        int high = (*byte >> 4) & 0xF;

        
        if(low < 10)
            *p++ = '0' + low;
        else
            *p++ = 'a' + low - 10;
        
        if(high < 10)
            *p++ = '0' + high;
        else
            *p++ = 'a' + high - 10;
        
        byte++;
    }
    for(int i = 0 ; i < 4 ; i++)
    {
        char tmp = buf[i];
        buf[i] = buf[7-i];
        buf[7-i] = tmp;
    }
    *p = 0;
    while(buf != 0 && *buf == '0')
        buf++;
    return buf;
}
unsigned int vsprintf(char *dst, char *fmt, __builtin_va_list args) 
{
    char *dst_orig = dst;
    while (*fmt) 
    {
        if (*fmt == '%') 
        {
            fmt++;
            // escape %
            if (*fmt == '%') 
            {
                goto put;
            }
            // string
            else if (*fmt == 's') 
            {
                char *p = __builtin_va_arg(args, char *);
                while (*p) 
                    *dst++ = *p++;
            }
            // number
            else if (*fmt == 'd') 
            {
                int arg = __builtin_va_arg(args, int);
                char buf[12];
                char *p = itoa(arg, buf);
                while (*p) {
                    *dst++ = *p++;
                }
            }
            // float
            else if (*fmt == 'f') 
            {
                double arg = (double) __builtin_va_arg(args, double);
                char buf[20];  // sign + 10 int + dot + 7 float
                char *p = dtoa(arg, buf);
                while (*p) 
                    *dst++ = *p++;
            }
            else if(*fmt == 'x')
            {
                unsigned int arg = __builtin_va_arg(args, unsigned int);
                char buf[9];
                char *p = hextoa(arg, buf);
                while (*p) 
                    *dst++ = *p++;
            }
        } 
        else 
        {
        put:
            *dst++ = *fmt;
        }
        fmt++;
    }
    *dst = '\0';

    return dst - dst_orig;  // return written bytes
}

unsigned int sprintf(char *dst, char *fmt, ...) 
{
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    return vsprintf(dst, fmt, args);
}