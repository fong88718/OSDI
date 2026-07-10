#ifndef MY_STRING_H
#define MY_STRING_H

int strcmp(const char *a, const char *b);
char* strcat(char *dest, const char *src);
char* strcpy(char *dest, const char *src);
char* dtoa(double d, char *s);
char* itoa(int n, char *s);
#endif