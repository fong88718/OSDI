#include "mailbox.h"
#include "mmio.h"
#include "uart.h"

#define MAILBOX_BASE    (MMIO_BASE + 0xB880)

#define MAILBOX_READ    ((volatile unsigned int*)MAILBOX_BASE)
#define MAILBOX_STATUS  ((volatile unsigned int*)MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE   ((volatile unsigned int*)MAILBOX_BASE + 0x20)

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define GET_BOARD_REVISION  0x00010002
#define REQUEST_CODE        0x00000000
#define REQUEST_SUCCEED     0x80000000
#define REQUEST_FAILED      0x80000001
#define TAG_REQUEST_CODE    0x00000000
#define END_TAG             0x00000000

void get_board_revision()
{
  unsigned int __attribute__((aligned(16))) mailbox[7];
  mailbox[0] = 7 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_BOARD_REVISION; // tag identifier
  mailbox[3] = 4; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // value buffer
  // tags end
  mailbox[6] = END_TAG;

  mailbox_call(mailbox, 7); // message passing procedure call, you should implement it following the 6 steps provided above.

  uart_printf("0x%x\n", mailbox[5]); // it should be 0xa020d3 for rpi3 b+
}

int mailbox_call(unsigned int *mailbox, int channel)
{
    unsigned long p = (unsigned long)mailbox;
    unsigned int r = p & ~(0xF)| (channel & (0xF));
    while(*MAILBOX_STATUS & MAILBOX_FULL)
        asm volatile("nop");
    *MAILBOX_WRITE = r;

    while(*MAILBOX_READ & MAILBOX_EMPTY)
        asm volatile("nop");
    if(*MAILBOX_READ  == r)
        return mailbox[1] == TAG_REQUEST_CODE;
    return 0;
}