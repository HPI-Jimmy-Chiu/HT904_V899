//*******************************************************
//      API-SMC(WDM)
//       FILE NAME       CSMCDEF.H
//********************************************************

#define CSMC_PTP            1
#define CSMC_JOG            2
#define CSMC_ORG            3
#define CSMC_SINGLE            4
#define CSMC_LOOP            5
#define CSMC_ZMOVE            6

#define CSMC_CW                0
#define CSMC_CCW            1

#define CSMC_ABS            0
#define CSMC_INC            1

#define CSMC_STOP            0
#define CSMC_PASS            1

#define CSMC_OUT1            1
#define CSMC_OUT2            2
#define CSMC_OUT3            4
#define CSMC_ALL            0XFF

#define CSMC_IN1            1
#define CSMC_IN2            2
#define CSMC_IN3            4
#define CSMC_IN4            8
#define CSMC_IN5            0x10
#define CSMC_IN6            0x20
#define CSMC_IN7            0x40

#define CSMC_STEP_0            0
#define CSMC_STEP_1            1

#define CSMC_HOLD            0
#define CSMC_HOLDOFF        1

#define CSMC_PLS_STOP        0
#define CSMC_PLS_FLCONST    1
#define CSMC_PLS_FHCONST    2
#define CSMC_PLS_READY        3
#define CSMC_PLS_ERCW        4
#define CSMC_PLS_DTMW        5
#define CSMC_PLS_ACCEL        6
#define CSMC_PLS_DECEL        7
#define CSMC_PLS_INPW        8

#define CSMC_MOVE            0
#define CSMC_STOP_COMMAND    1
#define CSMC_SD_COMMAND        2
#define CSMC_STOP_OTHER        3
#define CSMC_STOP_ALARM        4
#define CSMC_STOP_PLIM        5
#define CSMC_STOP_MLIM        6
#define CSMC_STOP_SD        7
#define CSMC_ERROR_ORG        8
#define CSMC_STOP_NORMAL    0XFF

#define CSMC_MESSAGE        0X8700

#define CSMC_DISABLE        0
#define CSMC_ENABLE            1

#define CSMC_OUTPULSE        0
#define CSMC_ENCODER        1

#define CSMC_LTC            0

#define CSMC_TIMER            1

#define CSMC_ALM            0x1
#define CSMC_PLIM            0x2
#define CSMC_MLIM            0x4
#define CSMC_ORGLIM            0x8
#define CSMC_SD                0x10

#define CSMC_2PULSE            0
#define CSMC_1PULSE            1

#define CSMC_AB                0
#define CSMC_UD                1

#define CSMC_HOFF            1
#define CSMC_ALMCLR            2
#define CSMC_CNTCLR            4

#define CSMC_DISTANCE        0
#define CSMC_ANGLE            1

#define CSMC_ALLTIME        0
#define CSMC_LIM            1

#define CSMC_CTRL_PCS        0x1
#define CSMC_CTRL_ERC        0x2
#define CSMC_CTRL_EZ        0x4
#define CSMC_CTRL_CLR        0x8
#define CSMC_CTRL_LTC        0x10
#define CSMC_CTRL_SD        0x20
#define CSMC_CTRL_INP        0x40
#define CSMC_CTRL_DIRCCW    0x80

