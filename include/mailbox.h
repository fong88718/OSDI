#ifndef MAILBOX_H
#define MAILBOX_H
void get_board_revision();
int mailbox_call(unsigned int *mailbox, int channel);
void get_VC_Core_base_address();
#endif