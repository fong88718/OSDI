#ifndef EXCEPTION_TABLE_H
#define EXCEPTION_TABLE_H


/* Current EL, using SP_EL0 */
#define  SYNC_CURRENT_SP0       0
#define  IRQ_CURRENT_SP0        1
#define  FIQ_CURRENT_SP0        2
#define  SERROR_CURRENT_SP0     3

/* Current EL, using SP_ELx */
#define  SYNC_CURRENT_SPX       4
#define  IRQ_CURRENT_SPX        5
#define  FIQ_CURRENT_SPX        6
#define  SERROR_CURRENT_SPX     7

/* Lower EL, AArch64 */
#define  SYNC_LOWER_AARCH64     8
#define  IRQ_LOWER_AARCH64      9   
#define  FIQ_LOWER_AARCH64      10
#define  SERROR_LOWER_AARCH64   11

/* Lower EL, AArch32 */
#define  SYNC_LOWER_AARCH32     12
#define  IRQ_LOWER_AARCH32      13
#define  FIQ_LOWER_AARCH32      14
#define  SERROR_LOWER_AARCH32   15

#endif /* VECTOR_TABLE_H */