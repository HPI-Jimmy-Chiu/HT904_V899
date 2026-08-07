#ifndef _L122ERR_H

#define _L122ERR_H
typedef enum {
    ERR_NoError=0,            //0
    ERR_NoCardFound=-1,        //
    ERR_lioReadIntMask=-2,
    ERR_RingNo=-3,
    ERR_CommSpeed=-4,
    ERR_DisableInt=-5,
    ERR_EnableInt=-6,
    ERR_LioRead=-7,
    ERR_SetLioReadIntMask=-8,
    ERR_LioReadOutput=-9,
    ERR_SetRingConfig=-10,
    ERR_FunctionNotSupportThisAsic=-11,
    ERR_LinkIntError=-12,
    ERR_PCIL122CardOpenError=-13,
    ERR_GetCardSwitchID=-14,
    ERR_FailRing0G91ARegisterWrite=-15,
    ERR_FailRing1G91ARegisterWrite=-16,
    ERR_FailRing0G91ARegisterRead=-17,
    ERR_FailRing1G91ARegisterRead=-18,
    ERR_CardSwitchNOoutstrip=-19,
    ERR_OpenCardRpt=-20,
}EN_PCI_L122_ERR;

#endif