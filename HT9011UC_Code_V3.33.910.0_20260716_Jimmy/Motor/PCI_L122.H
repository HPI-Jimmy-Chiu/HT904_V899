
#ifndef _PCIL122ENG_H

#define _PCIL122ENG_H

#include "type_def.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (PASCAL *GpioChangeUserCbk)(U16);
    //======= Constant definition ===============
    //=====Int Setting constant ==========
    #define C_INT_NONE_USED   0x0000
    #define C_INT_RING0_IRQ 0x0001
    #define C_INT_RING1_IRQ  0x0002
    #define C_INT_GPIO_IRQ   0x0004
    //======= Baud Rate Of Ring ==========
    #define C_BAUD_RATE_2P5MHZ 0x00
    #define    C_BAUD_RATE_5MHZ 0x01
    #define    C_BAUD_RATE_10MHZ 0x02
    #define    C_BAUD_RATE_20MHZ 0x03
    //======= Baud Rate Of Ring ==========
    #define PCIL122_INT_DISABLE 0x00
    #define    PCIL122_INT_ENABLE  0x01
    //======= End of Constant Definition ========

    //======= Card Resource functions =======
    I16 PASCAL _l122_open(I16 *existcards);
    I16 PASCAL _l122_close(U16 CardSwitchNo);
    I16 PASCAL _l122_set_ring_config( U16 CardSwitchNo , U16 RingOfCard , U16 BaudRate);
    I16 PASCAL _l122_get_start_ring_num( U16 CardSwitchNo , U16 *RingNo);

    //======= Local Io functions =========
    I16 PASCAL _l122_lio_input_read( U16 CardSwitchNo , U16 *data);
    I16 PASCAL _l122_lio_output_write( U16 CardSwitchNo , U16 data);
    I16 PASCAL _l122_lio_output_read( U16 CardSwitchNo , U16 *data);

    //======= Interrupt control functions ==========
    I16 PASCAL _l122_int_control( U16 CardSwitchNo,U16 IntType ,U16 Enable);
    I16 PASCAL _l122_set_lio_read_int_mask( U16 CardSwitchNo , U16 data);
    void PASCAL _l122_GPIO_change_UserCbk(U16 CardSwitchNo);
    I16 PASCAL _l122_int_link( U16 CardSwitchNo, GpioChangeUserCbk CbkFunc );
    I16 PASCAL _l122_get_int_status(U16 CardSwitchNo, U16 *intsrc, U16 *G91_1, U16 *G91_2);

#ifdef __cplusplus
}
#endif

#endif