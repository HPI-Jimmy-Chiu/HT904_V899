#ifdef MN200DLL_EXPORTS
#define MOTIONNETDLL_API __declspec(dllexport)
#else
#define MOTIONNETDLL_API __declspec(dllimport)
#endif

#ifndef _MN200DLL_h_
#define _MN200DLL_h_
//================================================================================
// Communication Speed Configuration
#define COMMSPEED_2_5M  0
#define COMMSPEED_5M    1
#define COMMSPEED_10M   2
#define COMMSPEED_20M   3

// Output Pulse Configuration
#define PULSE_MODE_PULSE_LOGIC_LOW_DIR_FORWARD_HIGH     0
#define PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_HIGH    1
#define PULSE_MODE_PULSE_LOGIC_LOW_DIR_FORWARD_LOW      2
#define PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_LOW     3

#define PULSE_MODE_CW_LOGIC_LOW                         4
#define PULSE_MODE_CW_LOGIC_HIGH                        7

#define PULSE_MODE_A_LEAD_B                             5
#define PULSE_MODE_A_LAG_B                              6

//Encoder Configuration
#define ENCODER_MODE_AB             0  // Quadrature pulse input
#define ENCODER_MODE_AB_MULT_2      1
#define ENCODER_MODE_AB_MULT_4      2
#define ENCODER_MODE_CW_CCW         3  // Up/Down pulse input
#define ENCODER_MODE_PULSE_DIR      4  // Pulse/Direction pulse input, for mn200_set_enc_cfg only

// Motion related IO Configuration
typedef enum
{
    PULSE_MODE                      =   0,
    SDLTC_LOGIC                     =   2,
    EL_PROC                         =   3,
    SD_ENA                          =   31,
    SD_PROC                         =   4,
    SDLTC_ENA                       =   5,
    SD_LOGIC                        =   6,
    ORG_LOGIC                       =   7,
    ALM_PROC                        =   8,
    ALM_LOGIC                       =   9,
    ERC_ERR_ENA                     =   10,
    ERC_ORG_ENA                     =   11,
    ERC_LEN                         =   12,
    ERC_LOGIC                       =   15,
    MANUAL_SD_ENA                   =   16,
    MANUAL_TRI_DRV_ELIMINATE_ENA    =   17,
    INP_ENA                         =   18,
    INP_LOGIC                       =   19,
    RESET_OUTPLS_AFTER_ORG_ENA      =   20,
    RESET_ENC_AFTER_ORG_ENA         =   21,
    RESET_CNT3_AFTER_ORG_ENA        =   22,
    ENC_MODE                        =   23,
    ENC_REV_ENA                     =   25,
    ENC_Z_LOGIC                     =   26,
    OUTPLS_REV_ENA                  =   27,
    CNT3_MODE                       =   28,
    ALL_DATA                        =   32,
}
MotionConfig;

#define SLOWDOWN        0
#define SUDDEN_STOP     0
#define SLOWDOWN_STOP   1

#define DISABLE_FEATURE 0
#define ENABLE_FEATURE  1

#define LOGIC_ACTIVE_LOW    0
#define LOGIC_ACTIVE_HIGH   1

#define ERC_LEN_12us            0
#define ERC_LEN_93us            1
#define ERC_LEN_371us           2
#define ERC_LEN_1500us          3
#define ERC_LEN_12ms            4
#define ERC_LEN_48ms            5
#define ERC_LEN_95ms            6
#define ERC_LEN_LEVEL_OUTPUT    7

// Filter Selection
#define FILTER_DATA_EMG_EL_SD_ORG_ALM_INP   0
#define FILTER_DATA_ENCODER                 1

// Cmp/Ring Source Selection
#define PULSE_COMMAND       0
#define ENCODER_POSITION    1

// SERVO/ERC/ALM_RESET ON/OFF
#define TURN_OFF    0
#define TURN_ON     1

// Move Direction
#define MOVE_DIRECTION_REVERSE  0
#define MOVE_DIRECTION_FORWARD  1

#define MOVE_DIRECTION_CW       0
#define MOVE_DIRECTION_CCW      1

// DIO FILE config
#define BACK_UP_DO_STATUS       0
#define RECOVER_DO_STATUS       1

// Speed Parameter
typedef enum
{
    MAXSPEED_DO_NOT_CHANGE,
    MAXSPEED_10K,               // 0.1 ~ 10,000 PPS
    MAXSPEED_20K,               // 0.2 ~ 20,000 PPS
    MAXSPEED_50K,               // 0.5 ~ 50,000 PPS
    MAXSPEED_100K,              //  1 ~ 100,000 PPS
    MAXSPEED_200K,              //  2 ~ 200,000 PPS
    MAXSPEED_500K,              //  5 ~ 500,000 PPS
    MAXSPEED_1M,                // 10 ~ 1,000,000 PPS
    MAXSPEED_2M,                // 20 ~ 2,000,000 PPS
    MAXSPEED_5M,                // 50 ~ 5,000,000 PPS
    MAXSPEED_6M                 // 66 ~ 6,666,666 PPS
}
MaxSpeed;

typedef struct _SPEED_PAR
{
//#ifdef MN200DLL_VB
//  BYTE        Max_Speed;              // selecting MaxSpeed to set the speed magnification rate
//#endif
    double      Start_Speed;            // Initial & Stop Speed (PPS)
    double      Drive_Speed;            // Operation Speed (PPS)
    double      Correction_Speed;       // Correction Speed (PPS)
    double      Acc;                    // Acceleration data (s/ PPS/S)
    double      Dec;                    // Deceleration data (s/ PPS/S)
#ifdef MN200DLL_VB
    //BYTE      Max_Speed;              // selecting MaxSpeed to set the speed magnification rate
    MaxSpeed    Max_Speed;              // selecting MaxSpeed to set the speed magnification rate
#endif
    BYTE        AccDec_Mode;            // ADC_MODE_RATE / ADC_MODE_TIME
    BYTE        SCurve_Enable;          // DISABLE / Enable
    double      SCurveAcc_Sect;         // Acceleration S-curve range (PPS)
    double      SCurveDec_Sect;         // Deceleration S-curve range (PPS)
//#ifdef MN200DLL_VB
//  BYTE        Max_Speed;              // selecting MaxSpeed to set the speed magnification rate
//#else
#ifndef MN200DLL_VB
    MaxSpeed    Max_Speed;              // selecting MaxSpeed to set the speed magnification rate
#endif

}SPEED_PAR, *PSPEED_PAR;

#define ADC_MODE_RATE       0   // Specify the rate (number of pulses per second)
#define ADC_MODE_TIME       1   // Specify the time (seconds)
//
#define ADC_MODE_RATE_NOACC 2   // Only do acceleration without acceleration
#define ADC_MODE_TIME_NOACC 3
#define ADC_MODE_RATE_NODEC 4   // Only do deceleration without deceleration
#define ADC_MODE_TIME_NODEC 5

//Fix Move Mode
#define FIX_MOVE_MODE_REL                   0x41
#define FIX_MOVE_MODE_ABS_BY_OUTPLS         0x42
#define FIX_MOVE_MODE_ABS_BY_ENC            0x43
#define FIX_MOVE_MODE_ZERO_RETURN_BY_OUTPLS 0x44
#define FIX_MOVE_MODE_ZERO_RETURN_BY_ENC    0x45

//20/03/13
//Absoluted interpolation
#define ABS_MOVE_BY_OUTPLS                  0x1
#define ABS_MOVE_BY_ENC                     0x2

#define FIRST_SEGMENT                       0x1
#define INTERP_SEGMENT                      0x2
#define LAST_SEGMENT                        0x3

// Compare Trigger Direction
#define CMPTRIG_DIRECTION_BOTH              8
#define CMPTRIG_DIRECTION_FORWARD           9
#define CMPTRIG_DIRECTION_REVERSE           10

// Device Information
#define DEV_INF_NO_DEV                  0x00
#define DEV_INF_MOTION_DEV              0x8b
#define DEV_INF_IO_32OUT_DEV            0x80
#define DEV_INF_IO_16IN_16OUT_DEV       0x82
#define DEV_INF_IO_32IN_DEV             0x84
#define DEV_INF_AI_8IN_DEV              0x8f// assigned manually
#define CPU_EMU_DEV_PORT23_DATA         0x0033//G9004A cpu emulation(for AI module)

// Status
#define MOTION_NOT_DONE 0
#define MOTION_DONE     1

#define CONTINUE_INTERPOLATION_NEXT_READY       1
#define CONTINUE_INTERPOLATION_NEXT_NOT_READY   0

#define ERR_STATUS_SW_PEL_STOP          0x00000001
#define ERR_STATUS_SW_MEL_STOP          0x00000002
#define ERR_STATUS_PEL_STOP             0x00000008
#define ERR_STATUS_MEL_STOP             0x00000010
#define ERR_STATUS_ALM_STOP             0x00000020
#define ERR_STATUS_EMG_STOP             0x00000080
#define ERR_STATUS_SD_STOP              0x00000100

typedef struct _MOTION_DEV_IO
{
    BYTE    SVON;
    BYTE    RESET_ALM;
    BYTE    RDY;
    BYTE    ALM;
    BYTE    PEL;
    BYTE    MEL;
    BYTE    ORG;
    BYTE    SDLTC;
    BYTE    SDIN;
    BYTE    INP;
    BYTE    EMG;
    BYTE    EZ;
    BYTE    ERC;
}MOTION_IO, *PMOTION_IO;

//For mn_process_cpu_emu_register, mn_process_cpu_emu_register_read_after_write,
#define CPU_EMU_CMD_ADDR_REFRESH_ADD        0x0080
#define CPU_EMU_CMD_ADDR_REFRESH_SUB        0x00c0
#define CPU_EMU_CMD_PROCESS_WRITE           0x0100
#define CPU_EMU_CMD_PROCESS_READ            0x0200
#define CPU_EMU_CMD_COMM_WAIT               0x0800

//For mn_set_customization
#define CUST_REPLACE_SPEED_PAR              0x2
#define CUST_FIXED_MAX_SPEED                0x3
#define CUST_ACC_DEC_TIME_PRIORITY          0x4

//For mn_save_cal, mn_load_cal
enum{CAL_ADR_REF_5V = 0,
    CAL_ADR_OFFSET,
    CAL_ADR_GAIN,
    CAL_ADR_DEFAULT_REF_5V,
    CAL_ADR_DEFAULT_OFFSET,
    CAL_ADR_DEFAULT_GAIN
};

//For mn_set_cal(),mn_save_cal(),mn_load_cal()
enum{CAL_ITEM_REF_5V = 0,
    CAL_ITEM_OFFSET,
    CAL_ITEM_GAIN,
    CAL_ITEM_ALL,//for mn_save_cal() & mn_load_cal()
    CAL_ITEM_DEFAULT//only for mn_load_cal()
};

//For mn_set_cal_src()
enum{CAL_SRC_NONE = 0,
    CAL_SRC_AGND,
    CAL_SRC_REF_5V
};

//For mn_get_speed_range, mn_get_tcurve_acc_range, mn_get_scurve_acc_range
typedef struct _AXIS_RANGE_SETTING_
{
    double minSpeed;
    double maxSpeed;
    double minAccTime;
    double maxAccTime;
    double minAccRates;
    double maxAccRates;

}AXIS_RANGE_SETTINGS, *PAXIS_RANGE_SETTINGS;

#ifdef __cplusplus
extern "C"
{
#endif
//============================================================================
//====================== MN200 DLL (V1.0) Release Functions===================
//============================================================================

// OS Configuration Routines
#ifdef MN200DLL_VB
// OS Configuration Routines
extern short __stdcall mn_open_all(BYTE* pNumLine);
extern short __stdcall mn200_get_lineinfo(BYTE bScannedIndex, BYTE* pLineNo);
extern short __stdcall mn200_get_cardinfo(BYTE bScannedIndex, BYTE* pCardID);
extern short __stdcall mn_close_all();
extern short __stdcall mn_reset(BYTE bLineNo);
extern short __stdcall mn_set_comm_speed(BYTE bLineNo, BYTE bCommSpeed);
extern short __stdcall mn_start_line(BYTE bLineNo, BYTE* pNumDev);
extern short __stdcall mn_stop_line(BYTE bLineNo);
extern short __stdcall mn_get_dll_version(WORD* wFileVersion, WORD* wProductVersion);

// Hardware Configuration Routines (Optional)
extern short __stdcall mn_set_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD dwData);
extern short __stdcall mn_get_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD* pData);
extern short __stdcall mn_set_filter(BYTE bLineNo, BYTE bDevNo, BYTE bFilterSelection, BYTE bFilterEnable);
extern short __stdcall mn_set_softlimit(BYTE bLineNo, BYTE bDevNo, BYTE bSWLimitEnable, BYTE bCmpSource, BYTE bStopMode, long LimitPositive, long LimitNegaitive);
extern short __stdcall mn_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bServoOn);
extern short __stdcall mn_set_erc(BYTE bLineNo, BYTE bDevNo, BYTE bErcOn);
extern short __stdcall mn_alarm_reset(BYTE bLineNo, BYTE bDevNo, BYTE bAlmRstOn);
extern short __stdcall mn_load_ezgo_cfg(BYTE bLineNo, char *FileName);

// AUTOMATIC HOME SEARCH
extern short __stdcall mn_home_start(BYTE bLineNo, BYTE bDevNo, SPEED_PAR *pSpeedPar, BYTE bDirection, BYTE bHomeMode=0, BYTE bEZcount=0);
extern short __stdcall mn_leave_home(BYTE bLineNo, BYTE bDevNo, SPEED_PAR* pSpeedPar, BYTE bDirection, BYTE bHomeMode = 0, BYTE bEZcount = 0);
extern short __stdcall mn_home_search(BYTE bLineNo, BYTE bDevNo, SPEED_PAR* pSpeedPar, BYTE bDirection, long OrgWidth, BYTE bHomeMode = 0, BYTE bEZcount = 0);

// INDEPENDENT MOVING FUNCTIONS
extern short __stdcall mn_velocity_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR *pSpeedPar, BYTE bDirection);
extern short __stdcall mn_fix_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR *pSpeedPar, long Position, BYTE bMoveType=FIX_MOVE_MODE_REL);
extern short __stdcall mn_stop_move(BYTE bLineNo, BYTE bDevNo, BYTE bStopMode=SUDDEN_STOP);

// INTERPOLATION MOVING FUNCTIONS
extern short __stdcall mn_set_group(BYTE bLineNo, BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]);
extern short __stdcall mn_get_group(BYTE bLineNo, BYTE bGrpNo, BYTE* pNumDev, BYTE bDevNo[]);
extern short __stdcall mn_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern short __stdcall mn_arc2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR *pSpeedPar, BYTE bDirection, long Dev1CenterPos, long Dev2CenterPos, long Dev1FinishPos, long Dev2FinishPos, DWORD Low32BitDummyDevNo=0, DWORD High32BitDummyDevNo=0, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern short __stdcall mn_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos);
extern short __stdcall mn_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR *pSpeedPar, long DevPos[], BYTE bNumDev);

// ABSOLUTED INTERPOLATION
extern MOTIONNETDLL_API short __stdcall mn_abs_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable, BYTE bMoveType, BYTE bSegConfig);
extern MOTIONNETDLL_API short __stdcall mn_abs_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos, BYTE bMoveType, BYTE bSegConfig);
extern MOTIONNETDLL_API short __stdcall mn_abs_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR *pSpeedPar, long DevPos[], BYTE bNumDev, BYTE bMoveType, BYTE bSegConfig);

// CONTINUOUS INTERPOLATION MOVING
extern short __stdcall mn_conti_interp_next_ready(BYTE bLineNo, BYTE bDevNo, BYTE *pReady);

// OTHER MOTION FUNCTIONS
extern short __stdcall mn_group_stop_move(BYTE bLineNo, BYTE bGrpNo, BYTE bStopMode=SUDDEN_STOP);
extern short __stdcall mn_group_hold_move(BYTE bLineNo, BYTE bGrpNo);
extern short __stdcall mn_group_start_move(BYTE bLineNo, BYTE bGrpNo);

// ADVANCED MOTION CONFIGURATIONS
extern short __stdcall mn_set_vring(BYTE bLineNo, BYTE bDevNo, BYTE bVRingEnable, BYTE bVRingSource, DWORD dwVRingValue);
extern short __stdcall mn_cnst_cmptrig_config(BYTE bLineNo, BYTE bDevNo, BYTE bCmpTrigEnable, BYTE bCmpSource, BYTE bDirection, WORD wCnstPitch);
extern short __stdcall mn_set_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD wValue);
extern short __stdcall mn_get_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD *wValue);

// MISCELLANEOUS FUNCTIONS
extern short __stdcall mn_change_v(BYTE bLineNo, BYTE bDevNo, SPEED_PAR *pSpeedPar, BYTE bWaitCmpEnable=DISABLE_FEATURE);
extern short __stdcall mn_change_p(BYTE bLineNo, BYTE bDevNo, long Position);
extern short __stdcall mn_set_cmdcounter(BYTE bLineNo, BYTE bDevNo, long Data);
extern short __stdcall mn_set_enccounter(BYTE bLineNo, BYTE bDevNo, long Data);

// STATUS
extern short __stdcall mn_get_dev_info(BYTE bLineNo, BYTE bDevNo, BYTE* pData);
extern short __stdcall mn_motion_done(BYTE bLineNo, BYTE bDevNo, BYTE* pDone);
extern short __stdcall mn_get_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_speed(BYTE bLineNo, BYTE bDevNo, double *pData);
extern short __stdcall mn_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO);
extern short __stdcall mn_get_error_status(BYTE bLineNo, BYTE bDevNo, DWORD* pData);
extern short __stdcall mn_get_latch_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_latch_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_speed_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_tcurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_scurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, double dScurveSect, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_comm_error_counter(BYTE bLineNo, WORD* pData);
extern short __stdcall mn_clear_comm_error_counter(BYTE bLineNo);

// Parallel I/O
extern short __stdcall mn200_get_di(BYTE bCardID, BYTE* pData);
extern short __stdcall mn200_set_do(BYTE bCardID, BYTE bData);
extern short __stdcall mn200_get_do(BYTE bCardID, BYTE* pData);

// Serial I/O
extern short __stdcall  mn_get_di_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
extern short __stdcall  mn_set_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
extern short __stdcall  mn_get_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);

extern short __stdcall  mn_get_di_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);
extern short __stdcall  mn_set_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE bData);
extern short __stdcall  mn_get_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);

extern short __stdcall  mn_get_di_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
extern short __stdcall  mn_set_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD wData);
extern short __stdcall  mn_get_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);

// ADVANCED IO FUNCTIONS
extern short __stdcall  mn_get_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE* pData);
extern short __stdcall  mn_set_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE bData);
extern short __stdcall  mn_get_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE* pData);
extern short __stdcall  mn_set_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bData);
extern short __stdcall mn_get_line_status(BYTE bLineNo, WORD* pData);
extern short __stdcall mn_get_slave_error_table(BYTE bLineNo, DWORD ErrorTable[]);
extern short __stdcall mn_clear_slave_error_flag(BYTE bLineNo, DWORD *ErrorTable);

// ANALOG IO EXTENSION
extern short __stdcall  mn_set_ao(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fData);
extern short __stdcall  mn_set_ao_offset(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, short OffsetLSB);
extern short __stdcall  mn_set_ao_gain(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fGainValue);
extern short __stdcall  mn_get_ai(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float* pData);
extern short __stdcall  mn_get_ai_all(BYTE bLineNo, BYTE bDevNo, float fData[]);
extern short __stdcall  mn_set_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem, BYTE bValue);
extern short __stdcall  mn_set_cal_src(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern short __stdcall  mn_save_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern short __stdcall  mn_load_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);

#elif MN200DLL_LABVIEW
// OS Configuration Routines
extern short __stdcall mn_open_all(BYTE* pNumLine);
extern short __stdcall mn200_get_lineinfo(BYTE bScannedIndex, BYTE* pLineNo);
extern short __stdcall mn200_get_cardinfo(BYTE bScannedIndex, BYTE* pCardID);
extern short __stdcall mn_close_all();
extern short __stdcall mn_reset(BYTE bLineNo);
extern short __stdcall mn_set_comm_speed(BYTE bLineNo, BYTE bCommSpeed);
extern short __stdcall mn_start_line(BYTE bLineNo, BYTE* pNumDev);
extern short __stdcall mn_stop_line(BYTE bLineNo);
extern short __stdcall mn_get_dll_version(WORD* wFileVersion, WORD* wProductVersion);

// Hardware Configuration Routines (Optional)
extern short __stdcall mn_set_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD dwData);
extern short __stdcall mn_get_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD* pData);
extern short __stdcall mn_set_filter(BYTE bLineNo, BYTE bDevNo, BYTE bFilterSelection, BYTE bFilterEnable);
extern short __stdcall mn_set_softlimit(BYTE bLineNo, BYTE bDevNo, BYTE bSWLimitEnable, BYTE bCmpSource, BYTE bStopMode, long LimitPositive, long LimitNegaitive);
extern short __stdcall mn_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bServoOn);
extern short __stdcall mn_set_erc(BYTE bLineNo, BYTE bDevNo, BYTE bErcOn);
extern short __stdcall mn_alarm_reset(BYTE bLineNo, BYTE bDevNo, BYTE bAlmRstOn);
extern short __stdcall mn_load_ezgo_cfg(BYTE bLineNo, char *FileName);

// AUTOMATIC HOME SEARCH
extern short __stdcall mn_home_start(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, BYTE bDirection, BYTE bHomeMode=0, BYTE bEZcount=0);
extern short __stdcall mn_leave_home(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, BYTE bDirection, BYTE bHomeMode = 0, BYTE bEZcount = 0);
extern short __stdcall mn_home_search(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, BYTE bDirection, long OrgWidth, BYTE bHomeMode = 0, BYTE bEZcount = 0);

// INDEPENDENT MOVING FUNCTIONS
extern short __stdcall mn_velocity_move(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, BYTE bDirection);
extern short __stdcall mn_fix_move(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, long Position, BYTE bMoveType=FIX_MOVE_MODE_REL);
extern short __stdcall mn_stop_move(BYTE bLineNo, BYTE bDevNo, BYTE bStopMode=SUDDEN_STOP);

// INTERPOLATION MOVING FUNCTIONS
extern short __stdcall mn_set_group(BYTE bLineNo, BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]);
extern short __stdcall mn_get_group(BYTE bLineNo, BYTE bGrpNo, BYTE* pNumDev, BYTE bDevNo[]);
extern short __stdcall mn_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern short __stdcall mn_arc2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR *pSpeedPar, BYTE bDirection, long Dev1CenterPos, long Dev2CenterPos, long Dev1FinishPos, long Dev2FinishPos, DWORD Low32BitDummyDevNo=0, DWORD High32BitDummyDevNo=0, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern short __stdcall mn_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR *pSpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos);
extern short __stdcall mn_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR *pSpeedPar, long DevPos[], BYTE bNumDev);

// ABSOLUTED INTERPOLATION
extern short __stdcall mn_abs_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, double* dSpeedPar, BYTE* dSpeedPar_Opt, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable, BYTE bMoveType, BYTE bSegConfig);
extern short __stdcall mn_abs_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, double* dSpeedPar, BYTE* dSpeedPar_Opt, long Dev1Pos, long Dev2Pos, long Dev3Pos, BYTE bMoveType, BYTE bSegConfig);
extern short __stdcall mn_abs_linen_move(BYTE bLineNo, BYTE bDevNo[], double* dSpeedPar, BYTE* dSpeedPar_Opt, long DevPos[], BYTE bNumDev, BYTE bMoveType, BYTE bSegConfig);

// CONTINUOUS INTERPOLATION MOVING
extern short __stdcall mn_conti_interp_next_ready(BYTE bLineNo, BYTE bDevNo, BYTE *pReady);

// OTHER MOTION FUNCTIONS
extern short __stdcall mn_group_stop_move(BYTE bLineNo, BYTE bGrpNo, BYTE bStopMode=SUDDEN_STOP);
extern short __stdcall mn_group_hold_move(BYTE bLineNo, BYTE bGrpNo);
extern short __stdcall mn_group_start_move(BYTE bLineNo, BYTE bGrpNo);

// ADVANCED MOTION CONFIGURATIONS
extern short __stdcall mn_set_vring(BYTE bLineNo, BYTE bDevNo, BYTE bVRingEnable, BYTE bVRingSource, DWORD dwVRingValue);
extern short __stdcall mn_cnst_cmptrig_config(BYTE bLineNo, BYTE bDevNo, BYTE bCmpTrigEnable, BYTE bCmpSource, BYTE bDirection, WORD wCnstPitch);
extern short __stdcall mn_set_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD wValue);
extern short __stdcall mn_get_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD *wValue);

// MISCELLANEOUS FUNCTIONS
extern short __stdcall mn_change_v(BYTE bLineNo, BYTE bDevNo, double* dSpeedPar, BYTE* dSpeedPar_Opt, BYTE bWaitCmpEnable=DISABLE_FEATURE);
extern short __stdcall mn_change_p(BYTE bLineNo, BYTE bDevNo, long Position);
extern short __stdcall mn_set_cmdcounter(BYTE bLineNo, BYTE bDevNo, long Data);
extern short __stdcall mn_set_enccounter(BYTE bLineNo, BYTE bDevNo, long Data);

// STATUS
extern short __stdcall mn_get_dev_info(BYTE bLineNo, BYTE bDevNo, BYTE* pData);
extern short __stdcall mn_motion_done(BYTE bLineNo, BYTE bDevNo, BYTE* pDone);
extern short __stdcall mn_get_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_speed(BYTE bLineNo, BYTE bDevNo, double *pData);
extern short __stdcall mn_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO);
extern short __stdcall mn_get_error_status(BYTE bLineNo, BYTE bDevNo, DWORD* pData);
extern short __stdcall mn_get_latch_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_latch_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern short __stdcall mn_get_speed_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_tcurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_scurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, double dScurveSect, PAXIS_RANGE_SETTINGS pAxisRange);
extern short __stdcall mn_get_comm_error_counter(BYTE bLineNo, WORD* pData);
extern short __stdcall mn_clear_comm_error_counter(BYTE bLineNo);

// Parallel I/O
extern short __stdcall mn200_get_di(BYTE bCardID, BYTE* pData);
extern short __stdcall mn200_set_do(BYTE bCardID, BYTE bData);
extern short __stdcall mn200_get_do(BYTE bCardID, BYTE* pData);

// Serial I/O
extern short __stdcall  mn_get_di_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
extern short __stdcall  mn_set_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
extern short __stdcall  mn_get_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);

extern short __stdcall  mn_get_di_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);
extern short __stdcall  mn_set_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE bData);
extern short __stdcall  mn_get_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);

extern short __stdcall  mn_get_di_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
extern short __stdcall  mn_set_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD wData);
extern short __stdcall  mn_get_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);

// ADVANCED IO FUNCTIONS
extern short __stdcall  mn_get_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE* pData);
extern short __stdcall  mn_set_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE bData);
extern short __stdcall  mn_get_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE* pData);
extern short __stdcall  mn_set_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bData);
extern short __stdcall mn_get_line_status(BYTE bLineNo, WORD* pData);
extern short __stdcall mn_get_slave_error_table(BYTE bLineNo, DWORD ErrorTable[]);
extern short __stdcall mn_clear_slave_error_flag(BYTE bLineNo, DWORD *ErrorTable);

// ANALOG IO EXTENSION
extern short __stdcall  mn_set_ao(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fData);
extern short __stdcall  mn_set_ao_offset(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, short OffsetLSB);
extern short __stdcall  mn_set_ao_gain(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fGainValue);
extern short __stdcall  mn_get_ai(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float* pData);
extern short __stdcall  mn_get_ai_all(BYTE bLineNo, BYTE bDevNo, float fData[]);
extern short __stdcall  mn_set_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem, BYTE bValue);
extern short __stdcall  mn_set_cal_src(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern short __stdcall  mn_save_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern short __stdcall  mn_load_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);

#else
// OS Configuration Routines
extern MOTIONNETDLL_API short __stdcall mn_open_all(BYTE* pNumLine);
extern MOTIONNETDLL_API short __stdcall mn200_get_lineinfo(BYTE bScannedIndex, BYTE* pLineNo);
extern MOTIONNETDLL_API short __stdcall mn200_get_cardinfo(BYTE bScannedIndex, BYTE* pCardID);
extern MOTIONNETDLL_API short __stdcall mn_close_all();
extern MOTIONNETDLL_API short __stdcall mn_reset(BYTE bLineNo);
extern MOTIONNETDLL_API short __stdcall mn_set_comm_speed(BYTE bLineNo, BYTE bCommSpeed);
extern MOTIONNETDLL_API short __stdcall mn_start_line(BYTE bLineNo, BYTE* pNumDev);
extern MOTIONNETDLL_API short __stdcall mn_stop_line(BYTE bLineNo);
extern MOTIONNETDLL_API short __stdcall mn_get_dll_version(WORD* wFileVersion, WORD* wProductVersion);

// Hardware Configuration Routines (Optional)
extern MOTIONNETDLL_API short __stdcall mn_set_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD dwData);
extern MOTIONNETDLL_API short __stdcall mn_get_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_filter(BYTE bLineNo, BYTE bDevNo, BYTE bFilterSelection, BYTE bFilterEnable);
extern MOTIONNETDLL_API short __stdcall mn_set_softlimit(BYTE bLineNo, BYTE bDevNo, BYTE bSWLimitEnable, BYTE bCmpSource, BYTE bStopMode, long LimitPositive, long LimitNegaitive);
extern MOTIONNETDLL_API short __stdcall mn_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bServoOn);
extern MOTIONNETDLL_API short __stdcall mn_set_erc(BYTE bLineNo, BYTE bDevNo, BYTE bErcOn);
extern MOTIONNETDLL_API short __stdcall mn_alarm_reset(BYTE bLineNo, BYTE bDevNo, BYTE bAlmRstOn);
extern MOTIONNETDLL_API short __stdcall mn_load_ezgo_cfg(BYTE bLineNo, char *FileName);

// AUTOMATIC HOME SEARCH
extern MOTIONNETDLL_API short __stdcall mn_home_start(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, BYTE bHomeMode=0, BYTE bEZcount=0);
extern MOTIONNETDLL_API short __stdcall mn_leave_home(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, BYTE bHomeMode = 0, BYTE bEZcount = 0);
extern MOTIONNETDLL_API short __stdcall mn_home_search(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, long OrgWidth, BYTE bHomeMode = 0, BYTE bEZcount = 0);

// INDEPENDENT MOVING FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_velocity_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection);
extern MOTIONNETDLL_API short __stdcall mn_fix_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, long Position, BYTE bMoveType=FIX_MOVE_MODE_REL);
extern MOTIONNETDLL_API short __stdcall mn_stop_move(BYTE bLineNo, BYTE bDevNo, BYTE bStopMode=SUDDEN_STOP);

// INTERPOLATION MOVING FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_set_group(BYTE bLineNo, BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]);
extern MOTIONNETDLL_API short __stdcall mn_get_group(BYTE bLineNo, BYTE bGrpNo, BYTE* pNumDev, BYTE bDevNo[]);
extern MOTIONNETDLL_API short __stdcall mn_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern MOTIONNETDLL_API short __stdcall mn_arc2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, BYTE bDirection, long Dev1CenterPos, long Dev2CenterPos, long Dev1FinishPos, long Dev2FinishPos, DWORD Low32BitDummyDevNo=0, DWORD High32BitDummyDevNo=0, BYTE bCnstSpdEnable=DISABLE_FEATURE);
extern MOTIONNETDLL_API short __stdcall mn_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos);
extern MOTIONNETDLL_API short __stdcall mn_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR SpeedPar, long DevPos[], BYTE bNumDev);

// ABSOLUTED INTERPOLATION
extern MOTIONNETDLL_API short __stdcall mn_abs_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable, BYTE bMoveType, BYTE bSegConfig);
extern MOTIONNETDLL_API short __stdcall mn_abs_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos, BYTE bMoveType, BYTE bSegConfig);
extern MOTIONNETDLL_API short __stdcall mn_abs_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR SpeedPar, long DevPos[], BYTE bNumDev, BYTE bMoveType, BYTE bSegConfig);


// CONTINUOUS INTERPOLATION MOVING
extern MOTIONNETDLL_API short __stdcall mn_conti_interp_next_ready(BYTE bLineNo, BYTE bDevNo, BYTE *pReady);

// OTHER MOTION FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_group_stop_move(BYTE bLineNo, BYTE bGrpNo, BYTE bStopMode=SUDDEN_STOP);
extern MOTIONNETDLL_API short __stdcall mn_group_hold_move(BYTE bLineNo, BYTE bGrpNo);
extern MOTIONNETDLL_API short __stdcall mn_group_start_move(BYTE bLineNo, BYTE bGrpNo);

// ADVANCED MOTION CONFIGURATIONS
extern MOTIONNETDLL_API short __stdcall mn_set_vring(BYTE bLineNo, BYTE bDevNo, BYTE bVRingEnable, BYTE bVRingSource, DWORD dwVRingValue);
extern MOTIONNETDLL_API short __stdcall mn_cnst_cmptrig_config(BYTE bLineNo, BYTE bDevNo, BYTE bCmpTrigEnable, BYTE bCmpSource, BYTE bDirection, WORD wCnstPitch);
extern MOTIONNETDLL_API short __stdcall mn_set_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD wValue);
extern MOTIONNETDLL_API short __stdcall mn_get_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD *wValue);

// MISCELLANEOUS FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_change_v(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bWaitCmpEnable=DISABLE_FEATURE);
extern MOTIONNETDLL_API short __stdcall mn_change_p(BYTE bLineNo, BYTE bDevNo, long Position);
extern MOTIONNETDLL_API short __stdcall mn_set_cmdcounter(BYTE bLineNo, BYTE bDevNo, long Data);
extern MOTIONNETDLL_API short __stdcall mn_set_enccounter(BYTE bLineNo, BYTE bDevNo, long Data);

// STATUS
extern MOTIONNETDLL_API short __stdcall mn_get_dev_info(BYTE bLineNo, BYTE bDevNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_motion_done(BYTE bLineNo, BYTE bDevNo, BYTE* pDone);
extern MOTIONNETDLL_API short __stdcall mn_get_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_speed(BYTE bLineNo, BYTE bDevNo, double *pData);
extern MOTIONNETDLL_API short __stdcall mn_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO);
extern MOTIONNETDLL_API short __stdcall mn_get_error_status(BYTE bLineNo, BYTE bDevNo, DWORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_latch_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_latch_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_speed_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, PAXIS_RANGE_SETTINGS pAxisRange);
extern MOTIONNETDLL_API short __stdcall mn_get_tcurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, PAXIS_RANGE_SETTINGS pAxisRange);
extern MOTIONNETDLL_API short __stdcall mn_get_scurve_acc_range(BYTE bLineNo, BYTE bDevNo, MaxSpeed Max_Speed, double dStartSpeed, double dDriveSpeed, double dScurveSect, PAXIS_RANGE_SETTINGS pAxisRange);
extern MOTIONNETDLL_API short __stdcall mn_get_comm_error_counter(BYTE bLineNo, WORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_clear_comm_error_counter(BYTE bLineNo);

// Parallel I/O
extern MOTIONNETDLL_API short __stdcall mn200_get_di(BYTE bCardID, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn200_set_do(BYTE bCardID, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn200_get_do(BYTE bCardID, BYTE* pData);

// Serial I/O
extern MOTIONNETDLL_API short __stdcall mn_get_di_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_get_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);

extern MOTIONNETDLL_API short __stdcall mn_get_di_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_get_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);

extern MOTIONNETDLL_API short __stdcall mn_get_di_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD wData);
extern MOTIONNETDLL_API short __stdcall mn_get_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);

// ADVANCED IO FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_get_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_get_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_set_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_get_line_status(BYTE bLineNo, WORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_slave_error_table(BYTE bLineNo, DWORD ErrorTable[]);
extern MOTIONNETDLL_API short __stdcall mn_clear_slave_error_flag(BYTE bLineNo, DWORD *ErrorTable);

// ANALOG IO EXTENSION
extern MOTIONNETDLL_API short __stdcall mn_set_ao(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fData);
extern MOTIONNETDLL_API short __stdcall mn_set_ao_offset(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, short OffsetLSB);
extern MOTIONNETDLL_API short __stdcall mn_set_ao_gain(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float fGainValue);
extern MOTIONNETDLL_API short __stdcall mn_get_ai(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float* pData);
extern MOTIONNETDLL_API short __stdcall mn_get_ai_all(BYTE bLineNo, BYTE bDevNo, float fData[]);
extern MOTIONNETDLL_API short __stdcall mn_set_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem, BYTE bValue);
extern MOTIONNETDLL_API short __stdcall mn_set_cal_src(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern MOTIONNETDLL_API short __stdcall mn_save_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
extern MOTIONNETDLL_API short __stdcall mn_load_cal(BYTE bLineNo, BYTE bDevNo, BYTE bItem);
#endif

#ifdef __cplusplus
}
#endif

// Return error code
#define SUCCESS                                    0        //No error
#define ERROR_NO_CARD_FOUND                     -100        //Board not connected.The board is not identified.
#define ERROR_IOCTL_FAILED                      -101        //DeviceIoControl fail!
#define ERROR_INVALID_LINE_NO                   -102        //Line number assignment error
#define ERROR_COMM_NOT_START                    -103        //I/O communication failed to start error
#define ERROR_INVALID_DEV_NO                    -104        //Device number assignment error
#define ERROR_NO_DEV_FOUND                      -105        //Device not connected.The device is not identified.
#define ERROR_SET_IO_DEV                        -106
#define ERROR_SET_MOTION_DEV                    -107
#define ERROR_START_SPEED_EXCEED_DRIVING_SPEED  -108
#define ERROR_INVALID_MAX_SPEED_SELECTION       -109
#define ERROR_SET_START_SPEED_OUT_RANGE         -110
#define ERROR_SET_DRIVING_SPEED_OUT_RANGE       -111
#define ERROR_INVALID_SCURVE_ENABLE             -112
#define ERROR_INVALID_ADC_MODE                  -113
#define ERROR_INVALID_ACC_DATA                  -114
#define ERROR_SET_ACC_DOUBLE_DEC                -115
#define ERROR_SET_ACC_OUT_RANGE                 -116
#define ERROR_SET_DEC_OUT_RANGE                 -117
#define ERROR_SET_ACC_SECT_OUT_RANGE            -118
#define ERROR_SET_DEC_SECT_OUT_RANGE            -119
#define ERROR_SET_CORRECTION_SPD_OUT_RANGE      -120
#define ERROR_SET_SCAN_INDEX_OUT_RANGE          -121
#define ERROR_INVALID_COMM_SPEED                -122
#define ERROR_COMM_NOT_STOP                     -123
#define ERROR_SET_DATA                          -124
#define ERROR_INVALID_CONFIG_ITEM               -125
#define ERROR_INVALID_FILTER_ITEM               -126
#define ERROR_INVALID_SOFTWARE_LIMIT_SOURCE     -127
#define ERROR_INVALID_STOP_MODE                 -128
#define ERROR_CONFLICT_WITH_VRING               -129
#define ERROR_INVALID_MOVE_DIRECTION            -130
#define ERROR_INVALID_HOME_MODE                 -131
#define ERROR_INVALID_EZ_COUNT                  -132
#define ERROR_MOVE_HOLD                         -133
#define ERROR_EMG_SIGNAL_ON                     -134
#define ERROR_ALM_SIGNAL_ON                     -135
#define ERROR_MEL_SIGNAL_ON                     -136
#define ERROR_PEL_SIGNAL_ON                     -137
#define ERROR_WAIT_INP                          -138
#define ERROR_WAIT_ERC                          -139
#define ERROR_WAIT_BACKLASH_CORRECT             -140
#define ERROR_WAIT_PULSE_IN                     -141
#define ERROR_MOTION_NOT_COMPLETE               -142
#define ERROR_INVALID_FIX_MOVE_MODE             -143
#define ERROR_REGISTER_FULL                     -144
#define ERROR_INVALID_POSITION                  -145
#define ERROR_INVALID_GROUPNO                   -146
#define ERROR_INVALID_NUM_DEV                   -147
#define ERROR_GROUP_ALREADY_HOLD                -148
#define ERROR_SET_ARC_FINISH_POS                -149
#define ERROR_SET_BIT_DUMMY_DEV                 -150
#define ERROR_INVALID_DEV_HOLD                  -151
#define ERROR_GROUP_NOT_HOLD                    -152
#define ERROR_INVALID_VRING_SOURCE              -153
#define ERROR_INVALID_VRING_VALUE               -154
#define ERROR_INVALID_CMPTRIG_SOURCE            -155
#define ERROR_INVALID_CMPTRIG_DIRECTION         -156
#define ERROR_INVALID_CMPTRIG_PITCH             -157
#define ERROR_MOTION_IS_COMPLETED               -158
#define ERROR_INVALID_CARD_ID                   -159
#define ERROR_INVALID_OUTPUT_DATA               -160
#define ERROR_INVALID_BITNO                     -161
#define ERROR_SET_BITNO                         -162
#define ERROR_INVALID_BYTENO                    -163
#define ERROR_SET_BYTENO                        -164
#define ERROR_CARD_ID_DUPLICATED                -165
#define ERROR_CONFIG_FILE_LOAD                  -166
#define ERROR_CONFIG_FILE_MATCH                 -167
#define ERROR_COMM_DISCONNECT                   -168
#define ERROR_INVALID_WORDNO                    -169
#define ERROR_SET_WORDNO                        -170
#define ERROR_STEP_HOME_FAILED                  -171
#define ERROR_CONFLICT_WITH_CMPTRIG             -172
#define ERROR_SET_CPU_EMU_DEV                   -173
#define ERROR_INVALID_CPU_EMU_START_ADDRESS     -174
#define ERROR_MATCH_CPU_EMU_CMD                 -175
#define ERROR_SET_CPU_EMU_ADDRESS_REFRESH       -176
#define ERROR_SET_CPU_EMU_COMM_WAIT             -177
#define ERROR_SET_CPU_EMU_PROCESS_MODE          -178
#define ERROR_INVALID_CHANNELNO                 -179
#define ERROR_SET_AO_VALUE_OUT_RANGE            -180
#define ERROR_SET_AI_DEV                        -181
#define ERROR_SET_AO_OFFSET_OUT_RANGE           -182
#define ERROR_SET_AO_GAIN_OUT_RANGE             -183
#define ERROR_INVALID_CALIBRATION_ITEM          -184
#define ERROR_INVALID_CALIBRATION_SOURCE        -185
#define ERROR_INVALID_AXIS_NO                   -186
#define ERROR_INVALID_INTERP_TYPE               -187
#define ERROR_INVALID_DECELERATE_POINT          -188
#define ERROR_INVALID_TERMINATE_THREAD_OPT      -189
#define ERROR_INVALID_ENC_MODE_OPT              -190
#define ERROR_NO_DO_BACKUP_FILE_FOUND           -191
#define ERROR_NO_DLL_VERSION_FOUND              -192
#define ERROR_DUPLICATE_DLL_LOAD                -193
#define ERROR_LINE_EDTE_FAULT                   -194
#define ERROR_LINE_ERAE_FAULT                   -195
#define ERROR_LINE_CAER_FAULT                   -196

//For mn_conti_linen_move
#define CONTINUE_INTERPOLATION_START            0
#define CONTINUE_INTERPOLATION_NEXT             1
#define CONTINUE_INTERPOLATION_END              2

// For mn_set_gdio_dir(bDir)
#define GPIO_DIR_INPUT  0
#define GPIO_DIR_OUTPUT 1

// For mn_step6_set_home_check(bHomeLogic)
#define STEP6_HOME_NORMAL_CLOSE     1
#define STEP6_HOME_NORMAL_OPEN      0

//For mn_step6_set_micro_step(bMicroStep)
#define STEP6_MICRO_STEP_FULL               0
#define STEP6_MICRO_STEP_ONE_HALF           1
#define STEP6_MICRO_STEP_ONE_QUARTER        2
#define STEP6_MICRO_STEP_ONE_EIGHTH         4
#define STEP6_MICRO_STEP_ONE_SIXTEENTH      5
#define STEP6_MICRO_STEP_ONE_THIRTY_SECOND  6

// For mn_step6_set_current
#define STEP6_SET_CURRENT_ALL_NORMAL        0
#define STEP6_SET_CURRENT_AXIS_4_LOW        1
#define STEP6_SET_CURRENT_AXIS_4_5_LOW      2
#define STEP6_SET_CURRENT_ALL_NORMAL_       3

// For mn_cmp_stop
#define CMP_STOP_METHOD_CNT_EQUAL           1
#define CMP_STOP_METHOD_CNT_FWD_EQUAL       2
#define CMP_STOP_METHOD_CNT_REV_EQUAL       3
#define CMP_STOP_METHOD_CNT_EXCEED          4
#define CMP_STOP_METHOD_CNT_LESS            5

//For mn200_get_enc_di_bit
#define ENC_DI_BIT_A 0
#define ENC_DI_BIT_B 1
#define ENC_DI_BIT_C 2

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************
 *--------- NON-STANDARD FUNCTIONS-------
 ****************************************/
extern MOTIONNETDLL_API short __stdcall mn_reset_init(BYTE bLineNo, BYTE bDevNum);
extern MOTIONNETDLL_API short __stdcall mn_reset_slave(BYTE bLineNo, BYTE bDevNo);
extern MOTIONNETDLL_API short __stdcall mn_recover_dio_status(BYTE bLineNo, BYTE bConfig);
extern MOTIONNETDLL_API short __stdcall mn_cmp_stop(BYTE bLineNo, BYTE bDevNo, BYTE bCmpStopEnable, BYTE bCmpSource, BYTE bCmpMethod, long wCmpPos, BYTE bStopMode=SUDDEN_STOP);
extern MOTIONNETDLL_API short __stdcall mn_set_cmp_counter(BYTE bLineNo, BYTE bDevNo, WORD Data);
extern MOTIONNETDLL_API short __stdcall mn_get_cmp_counter(BYTE bLineNo, BYTE bDevNo, WORD *wCount3);
extern MOTIONNETDLL_API short __stdcall mn_set_cmp(BYTE bLineNo, BYTE bDevNo, BYTE bCmpOn);
extern MOTIONNETDLL_API short __stdcall mn_set_gdio_dir(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bDir);
extern MOTIONNETDLL_API short __stdcall mn_set_gdo_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_get_gdio_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_step6_set_home_check(BYTE bLineNo, BYTE bFirstDevNo, BYTE bHomeLogic, BYTE bEnableDevIndexBits);
extern MOTIONNETDLL_API short __stdcall mn_step6_set_micro_step(BYTE bLineNo, BYTE FirstDevNo, BYTE bMicroStep);
extern MOTIONNETDLL_API short __stdcall mn_step6_set_current(BYTE bLineNo, BYTE FirstDevNo, BYTE bCurrentMode);
extern MOTIONNETDLL_API short __stdcall mn_step_home_start(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, long* pOffsetPulse);
extern MOTIONNETDLL_API short __stdcall mn_do_cfg_save();
extern MOTIONNETDLL_API short __stdcall mn_do_cfg_load();

// INDEPENDENT MOVING TEST FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_home_start_t(BYTE bLineNo, BYTE bDevNo, SPEED_PAR* pSpeedPar, BYTE bDirection, BYTE bHomeMode=0, BYTE bEZcount=0);
extern MOTIONNETDLL_API short __stdcall mn_velocity_move_t(BYTE bLineNo, BYTE bDevNo, SPEED_PAR* pSpeedPar, BYTE bDirection);
extern MOTIONNETDLL_API short __stdcall mn_fix_move_t(BYTE bLineNo, BYTE bDevNo, SPEED_PAR* pSpeedPar, long Position, BYTE bMoveType=FIX_MOVE_MODE_REL);

// INTERPOLATION MOVING FUNCTIONS
extern MOTIONNETDLL_API short __stdcall mn_conti_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR SpeedPar, long DevPos[], BYTE bNumDev, BYTE bContiType = 0);
extern MOTIONNETDLL_API short __stdcall mn_conti_arc2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, BYTE bDirection, long Dev1CenterPos, long Dev2CenterPos, long Dev1FinishPos, long Dev2FinishPos, DWORD Low32BitDummyDevNo=0, DWORD High32BitDummyDevNo=0, BYTE bCnstSpdEnable=DISABLE_FEATURE, BYTE bContiType = 0);
// CONTINUOUS INTERPOLATION MOVING
extern MOTIONNETDLL_API short __stdcall mn_conti_linen_interp_next_ready(BYTE bLineNo, BYTE bDevNo[], BYTE bDevNum,  BYTE *pReady);
// STATUS
extern MOTIONNETDLL_API short __stdcall mn_get_comm_err_dev_list(BYTE bLineNo, DWORD *ErrorTable);

// MN200 Encoder
extern MOTIONNETDLL_API short __stdcall mn200_get_enccounter(BYTE bCardID, long* pData);
extern MOTIONNETDLL_API short __stdcall mn200_reset_enccounter(BYTE bCardID);

extern MOTIONNETDLL_API short __stdcall mn200_set_enc_cfg(BYTE bCardID, WORD wEncMode, WORD wEncLogic);
extern MOTIONNETDLL_API short __stdcall mn200_get_enc_cfg(BYTE bCardID, WORD* wEncMode, WORD* wEncLogic);
extern MOTIONNETDLL_API short __stdcall mn200_get_enc_status(BYTE bCardID, BYTE* bEncASts, BYTE* bEncBSts, BYTE* bEncCSts);
extern MOTIONNETDLL_API short __stdcall mn200_get_enc_di_bit(BYTE bCardID, BYTE bBitNo, BYTE* pData);

//MN200 Parallel I/O
extern MOTIONNETDLL_API short __stdcall mn200_get_di_bit(BYTE bCardID, BYTE bBitNo, BYTE* pData);

// Serial AI for calibration
//extern MOTIONNETDLL_API short __stdcall mn_ai_cal_set_ref_5v(BYTE bLineNo, BYTE bDevNo, BYTE bValue);
//extern MOTIONNETDLL_API short __stdcall mn_ai_cal_set_offset_0v(BYTE bLineNo, BYTE bDevNo, BYTE bValue, float* pData);
//extern MOTIONNETDLL_API short __stdcall mn_ai_cal_set_gain_5v(BYTE bLineNo, BYTE bDevNo, BYTE bValue, float* pData);
//extern MOTIONNETDLL_API short __stdcall mn_ai_cal_reset(BYTE bLineNo, BYTE bDevNo);
//extern MOTIONNETDLL_API short __stdcall mn_ai_cal_load(BYTE bLineNo, BYTE bDevNo);

// Serial AI for calibration
//extern MOTIONNETDLL_API short __stdcall mn_verify_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_ai_ver_get(BYTE bLineNo, BYTE bDevNo, WORD *pVer);

// Basic Functions to Access Register
extern MOTIONNETDLL_API void __stdcall mn_write_motion_register(BYTE bLineNo, BYTE bDevNo, WORD wCmd, DWORD dwData);
extern MOTIONNETDLL_API DWORD __stdcall mn_read_motion_register(BYTE bLineNo, BYTE bDevNo, WORD rCmd);

// Get firmware version
extern MOTIONNETDLL_API short __stdcall mn200_firmware_ver_get(BYTE bCardID, WORD *pFirmwareVer);

// Advanced Interrupt function
typedef short (CALLBACK *UserCbk)(BYTE bLineNo, BYTE bDevNo, DWORD dwFactor);
//extern MOTIONNETDLL_API short __stdcall   mn_set_int_link(BYTE bLineNo, BYTE bDevNo, DWORD dwFactor, UserCbk CbkFunc);
extern MOTIONNETDLL_API short __stdcall mn_set_int_link(BYTE bLineNo[], BYTE bDevNo[], DWORD dwFactor[], BYTE bNumDev, UserCbk CbkFunc);

// Read cycle time
extern MOTIONNETDLL_API short __stdcall mn_read_cycle_time(BYTE bLineNo, BYTE bDevNo, WORD* wTime);
// Read CRC error times
extern MOTIONNETDLL_API short __stdcall mn_read_CRC_error_time(BYTE bLineNo, BYTE bDevNo, WORD* wTimes);
extern MOTIONNETDLL_API short __stdcall mn_clear_CRC_error(BYTE bLineNo, BYTE bDevNo);

// G9004A
extern MOTIONNETDLL_API short __stdcall mn_process_cpu_emu_register(BYTE bLineNo, BYTE bDevNo, WORD wProcessMode, WORD wStartAddress, WORD wData[], BYTE bNumContiTimes=0, WORD wAddressRefreshContiMode=0, WORD wIsWaitComm=0);
extern MOTIONNETDLL_API short __stdcall mn_process_cpu_emu_register_read_after_write(BYTE bLineNo, BYTE bDevNo, WORD wWriteAddress, WORD wWriteData[], WORD wReadStartAddress, WORD wReadData[], BYTE bNumContiTimesWrite=0, BYTE bNumContiTimesRead=0, WORD wAddressRefreshContiMode=0, WORD wIsWaitComm=0);

// GPM Function
extern MOTIONNETDLL_API short __stdcall mn_gpm01_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bAxisNo, BYTE bServoOn);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_config(BYTE bLineNo, BYTE bDevNo, BYTE bGPIOEnable);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO);

extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_di_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_set_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);

extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_di_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_set_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE bData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);

extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_di_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_set_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD wData);
extern MOTIONNETDLL_API short __stdcall mn_gpm01_get_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);


#ifdef __cplusplus
}
#endif


#endif