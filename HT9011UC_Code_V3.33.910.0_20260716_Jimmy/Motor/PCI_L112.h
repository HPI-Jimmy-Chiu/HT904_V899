#ifndef __PCI_L112_H__
#define __PCI_L112_H__

#include "Type_def.h"

#ifdef __cplusplus
extern "C" {
#endif
//
I16 PASCAL _l112_open(I16* existcard);
I16 PASCAL _l112_close(I16 CardNo);
//
// Local I/O
I16 PASCAL _l112_lio_read(I16 CardNo);
I16 PASCAL _l112_lio_write(I16 CardNo, U8 Value);
//
// Interrupt
I16 PASCAL _l112_int_control(I16 CardNo, U16 Enable);
//
#ifdef __cplusplus
}
#endif

#endif //__PCI_L112_H__

