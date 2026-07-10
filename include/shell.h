#ifndef SHELL_H
#define SHELL_H

void shell_init();
void shell_read(char *cmd);
void shell_parse(char *cmd);
enum ANSI_ESC decode_csi_key();
enum ANSI_ESC decode_ansi_escape();

#endif