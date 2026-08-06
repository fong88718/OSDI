#include "mailbox.h"
#include "uart0.h"

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

    int ok = mailbox_call(mailbox, 8); // message passing procedure call, you should implement it following the 6 steps provided above.
    if(ok)
        uart_printf("\r0x%x\n", mailbox[5]); // it should be 0xa020d3 for rpi3 b+
    else    
        uart_printf("Fail to get board revision!\n");
}

void get_VC_Core_base_address()
{
    unsigned int __attribute__((aligned(16))) mailbox[8];
    mailbox[0] = 8 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE; 
    // tags begin
    mailbox[2] = GET_VC_CORE_BASE_ADDRESS; // tag identifier
    mailbox[3] = 8; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer, base address in bytes
    mailbox[6] = 0; // value buffer, size in bytes
    // tags end
    mailbox[7] = END_TAG;

    int ok = mailbox_call(mailbox, 8); // message passing procedure call, you should implement it following the 6 steps provided above.
    if(ok)
        uart_printf("\rbase 0x%x + size 0x%x\n", mailbox[5], mailbox[6]);
    else
        uart_printf("Fail to get VC Core base address!\n");
}

int mailbox_call(unsigned int *mailbox, int channel)
{
    unsigned int r = (unsigned int)((((unsigned long)mailbox) & ~(0xF)) | (channel & (0xF)));

    // wait until full flag unset
    while(*MAILBOX_STATUS & MAILBOX_FULL)
        asm volatile("nop");
    *MAILBOX_WRITE = r;

    while(1)
    {
        // wait until empty flag unset
        while(*MAILBOX_STATUS & MAILBOX_EMPTY)
            asm volatile("nop");
        // is it a response to our msg?
        if(*MAILBOX_READ  == r)
            return mailbox[1] == REQUEST_SUCCEED; // check is response success
    }
    return 0;
}