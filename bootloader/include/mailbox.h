#ifndef MAILBOX_H
#define MAILBOX_H

#include "mmio.h"

#define MAILBOX_BASE    (MMIO_BASE + 0xB880)

// address map
#define MAILBOX_READ    ((volatile unsigned int*)MAILBOX_BASE)
#define MAILBOX_STATUS  ((volatile unsigned int*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE   ((volatile unsigned int*)(MAILBOX_BASE + 0x20))

// flag
#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

// code
#define REQUEST_CODE                0x00000000
#define REQUEST_SUCCEED             0x80000000
#define REQUEST_FAILED              0x80000001

// tag
#define GET_BOARD_REVISION          0x00010002
#define GET_VC_CORE_BASE_ADDRESS    0x00010006
#define SET_UART_CLOCK_RATE         0x00038002
#define TAG_REQUEST_CODE            0x00000000
#define END_TAG                     0x00000000

#define UART_ID                     0x00000002
#define UART_RATE                   4000000

void get_board_revision();
int mailbox_call(unsigned int *mailbox, int channel);
void get_VC_Core_base_address();
#endif