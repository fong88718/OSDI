#ifndef MY_STRING_H
#define MY_STRING_H

int strcmp(const char *a, const char *b);
char* strcat(char *dest, const char *src);
char* strcpy(char *dest, const char *src);
char* dtoa(double d, char *s);
char* itoa(int n, char *s);
char* hextoa(unsigned int arg, char *buf);

unsigned int vsprintf(char *dst, char *fmt, __builtin_va_list args);
unsigned int sprintf(char *dst, char *fmt, ...); 
#endif