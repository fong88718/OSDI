#include "frame_buffer.h"
#include "mailbox.h"
#include "uart0.h"

unsigned int width, height, pitch, isrgb; // dimensions and channel order 
unsigned char *fb;                        // raw frame buffer address 

void fb_init()
{
    unsigned int __attribute__((aligned(16))) mailbox[35];
    mailbox[0] = 35 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin

    // set physical width / height
    mailbox[2] = MBOX_TAG_SET_PHY_WIDTH_HEIGHT; // tag identifier
    mailbox[3] = 8; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 960; // width in pixels
    mailbox[6] = 720; // heigh in pixels

    // set virtual width / height
    mailbox[7] = MBOX_TAG_SET_VTL_WIDTH_HEIGHT; // tag identifier
    mailbox[8] = 8; // maximum of request and response value buffer's length.
    mailbox[9] = TAG_REQUEST_CODE;
    mailbox[10] = 960; // width in pixels
    mailbox[11] = 720; // heigh in pixels


    // set virtual offset
    mailbox[12] = MBOX_TAG_SET_VTL_OFFSET; // tag identifier
    mailbox[13] = 8; // maximum of request and response value buffer's length.
    mailbox[14] = TAG_REQUEST_CODE;
    mailbox[15] = 0; // X in pixels
    mailbox[16] = 0; // Y in pixels

    // set depth
    mailbox[17] = MBOX_TAG_SET_DEPTH; // tag identifier
    mailbox[18] = 4; // maximum of request and response value buffer's length.
    mailbox[19] = TAG_REQUEST_CODE;
    mailbox[20] = 32; // bits per pixel

    // set pixel order
    mailbox[21] = MBOX_TAG_SET_PIXEL_ORDER; // tag identifier
    mailbox[22] = 4; // maximum of request and response value buffer's length.
    mailbox[23] = TAG_REQUEST_CODE;
    mailbox[24] = 1; // state. (0x0: BGR, 0x1: RGB)

    // allocate buffer
    mailbox[25] = MBOX_TAG_ALLOCATE_BUFFER; // tag identifier
    mailbox[26] = 8; // maximum of request and response value buffer's length.
    mailbox[27] = TAG_REQUEST_CODE;
    mailbox[28] = 4096; // frame buffer base address in bytes
    mailbox[29] = 0;  // frame buffer size in bytes

    // get pitch
    mailbox[30] = MBOX_TAG_GET_PITCH; // tag identifier
    mailbox[31] = 4; // maximum of request and response value buffer's length.
    mailbox[32] = TAG_REQUEST_CODE;
    mailbox[33] = 0; // bytes per line
    // tags end
    mailbox[34] = END_TAG;

    int ok = mailbox_call(mailbox, 8); // message passing procedure call, you should implement it following the 6 steps provided above.

    if(ok && mailbox[20] == 32 && mailbox[28] != 0)
    {
        fb = (unsigned char*)(unsigned long)(mailbox[28] & 0x3FFFFFFF);  // convert GPU address to ARM address
        width = mailbox[5];
        height = mailbox[6];
        pitch = mailbox[33];
        isrgb = mailbox[24];
    }
    else
    {
        uart_printf("Unable to set screen resolution to 1024x768x24\n");
    }
}

void fb_splash()
{
    unsigned int white = 255 << 16 | 255 << 8 | 255;
    unsigned int black = 0;
    unsigned int current = white, start = white, space = 50;
    unsigned char *p = fb;

    for(int i = 0 ; i < height ; i++)
    {
        if(i % space == 0)
            current = start = (start == white) ? black : white;
        for(int j = 0 ; j < width ; j += space)
        {
            for(int k = 0 ; k < space && k+j < width ; k++)
            {
                *((unsigned int*)p) = current;
                p += 4;
            }
            current = (current == white) ? black : white;
        }
    }
}