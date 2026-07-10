#include "util.h"

#define PM_PASSWORD 0x5a000000
#define PM_RSTC ((volatile unsigned int*)0x3F10001c)
#define PM_WDOG ((volatile unsigned int*)0x3F100024)

void reset(int tick) // reboot after watchdog timer expire
{    
    *PM_RSTC = PM_PASSWORD | 0x20; // full reset
    *PM_WDOG = PM_PASSWORD | tick; // number of watchdog tick
}

void cancel_reset() 
{
    *PM_RSTC = PM_PASSWORD | 0; // full reset
    *PM_WDOG = PM_PASSWORD | 0; // number of watchdog tick
}

double getTimeStamp()
{
    unsigned int cnt, freq;
    asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq));
    asm volatile("mrs %0, CNTPCT_EL0" : "=r"(cnt));
    return 1.0 * cnt / freq;
}