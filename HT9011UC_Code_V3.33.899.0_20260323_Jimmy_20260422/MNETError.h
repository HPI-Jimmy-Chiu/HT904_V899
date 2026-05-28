

#define ERR_NoError                       0
#define ERR_EventError                   -1
#define ERR_LinkError                    -2
#define ERR_MNET_Ring_Used               -3
#define ERR_Invalid_Ring                 -4
#define ERR_Invalid_Slave                -5
#define ERR_Invalid_Hardware             -6
#define ERR_Ring_Communication           -7
#define ERR_Value_Out_Range              -8
#define ERR_Invalid_Setting              -9
#define ERR_RingCommunicationTimeOut    -10

#define ERR_Axis_Communication          -11  //FIFO Communication error
#define ERR_Axis_command                -12
#define ERR_Axis_Receive                -13
#define ERR_Invalid_Operating_Velocity  -14
#define ERR_PosOutOfRange               -15
#define ERR_Invalid_MaxVel              -16
#define ERR_Speed_Change                -17
#define ERR_SlowDown_Point              -18
#define ERR_Invalid_DIO                 -19
#define ERR_Invalid_Comparator          -20
#define ERR_Comparator_Config           -21
#define ERR_CompareSourceError          -22
#define ERR_CompareActionError          -23
#define ERR_CompareMethodError          -24
#define ERR_ComparatorRead              -25
#define ERR_LimitOutOfRange             -26
#define ERR_DeviceTable_NotMatch        -27
#define ERR_Invalid_speed_pattern       -28
#define ERR_SetAutoTrigger              -29

#define ERR_Latch_Config                -30
#define ERR_LatchError                  -31
#define ERR_LatchRead                   -32
#define ERR_Invalid_AxisNo              -33
#define ERR_HomeConfig                  -35


/////////////////////G94 BUS ERROR///////////////////
#define ERR_G94_RECEIVE_TimeOut         -36
#define ERR_G94_CPURead                 -37

/////////////////////M4 ERROR///////////////////
#define ERR_M4_Buffer_Full                   -43
#define ERR_M4_move_Wait_Motiondone          -44
#define ERR_M4_InvalidParameter              -45
#define ERR_M4_CPLDRead                      -46
#define ERR_M4_RegisterRead                  -47
#define ERR_M4_CPLDWrite                     -48
#define ERR_M4_RegisterWrite                 -49
#define ERR_M4_InvalidAxisNo                 -50
#define ERR_M4_MOFStatusErr                  -51
#define ERR_M4_InvalidAxisSelect             -52
#define ERR_M4_MPGmode                       -53
#define ERR_M4_InvalidMpgEnable              -54
#define ERR_M4_MOFConfigmode                 -55
#define ERR_M4_SpeedError                    -56
#define ERR_M4_AxisArrayError                -57
#define ERR_M4_InvalidSrcAxisNo              -58
#define ERR_M4_InvalidTriggeredTiming        -59

//==============================================
#define ERR_M1_LoadAxisDataError            -60
#define ERR_M4_LoadAxisDataError            -61
#define ERR_BacklashValueError              -62
#define ERR_BacklashEnableError             -63
#define ERR_BacklashCntSrcError             -64
#define ERR_BacklashCorrectionError         -65
#define ERR_FixSpeedBeforeMotionDone        -66
#define ERR_NotPermitToWorkAtSameAxis       -67
#define ERR_Invalid_Type                    -68
#define ERR_SV_OutOfRange                   -69

#define ERR_Invalid_Action                  -70
#define ERR_InvalidFile                     -71
#define ERR_ErrLogOpened                    -72
#define ERR_Invalid_BaudRate                -73
#define ERR_FailGetDeviceTable              -74
#define ERR_NoErrorDevice                   -75
#define ERR_Invalid_PortNo                  -76
#define ERR_FailToEnableSoftLimit           -77
#define ERR_FailToDisableSoftLimit          -78
#define ERR_FailToGetCurrentSpeed           -79

#define ERR_M1_RegisterWrite                -80
#define ERR_M1_RegisterRead                 -81
#define ERR_ComparatorNoError               -82
#define ERR_M1_FailToInitialize             -83
#define ERR_M1_FailToSetServoOn             -84
#define ERR_M1_FailToSetResetAlarm          -85
#define ERR_M1_CommandWrite                 -86
#define ERR_M1_FailToSetSpeed               -87
#define ERR_M1_RelativeMove                 -88
#define ERR_M1_AbsoluteMove                 -89

#define ERR_M1_FixSpeedRange                -90
#define ERR_M1_ContinousMove                -91
#define ERR_M1_VelocityChange               -92
#define ERR_M1_SlowdownStop                 -93
#define ERR_M1_EmergecyStop                 -94
#define ERR_M1_ImediateStop                 -95
#define ERR_M1_SynchRelativeMove            -96
#define ERR_M1_SynchAbsoluteMove            -97
#define ERR_M1_SynchContinousMove           -98
#define ERR_M1_TCurveSynchLineMove          -99
#define ERR_M1_SCurveSynchLineMove          -100
#define ERR_M1_PChangeSlowDownPointError    -101
#define ERR_M1_DChangeSlowDownPointError    -102
#define ERR_M1_CommandTimeOut               -103

#define ERR_AI8_RegisterWriteFail           -111
#define ERR_AI8_RegisterReadFail            -112
#define ERR_AI8_DeviceTypeNotCorrect        -113
#define ERR_AI8_IncorrectChannelNo          -114
#define ERR_AI8_IncorrectValueOfGain        -115
#define ERR_AI8_DeviceNotEnable             -116
#define ERR_AI8_ChannelNotEnable            -117
#define ERR_AI8_MOSI_Busy                   -118
#define ERR_AI8_MISO_Busy                   -119
#define ERR_AI8_Read_EEPROM                 -120
#define ERR_AI8_AdjustFail                  -121

#define ERR_AO4_RegisterWriteFail           -131
#define ERR_AO4_RegisterReadFail            -132
#define ERR_AO4_RegisterReadTimeout         -133
#define ERR_AO4_DeviceTypeNotCorrect        -134
#define ERR_AO4_ChannelNoNotCorrect         -135
#define ERR_AO4_OutputValueNotCorrect       -136
#define ERR_AO4_CoarseGainValueNotCorrect   -137
#define ERR_AO4_FineGainValueNotCorrect     -138
#define ERR_AO4_OffsetValueNotCorrect       -139
#define ERR_AO4_OutputVoltageNotCorrect     -140
#define ERR_AO4_FlashReadFail               -141
#define ERR_AO4_FlashWriteFail              -142
#define ERR_AO4_FlashEraseFail              -143

#define ERR_GetDataDeviceOffsetAddress      -150
#define ERR_FunctionNotSupportThisDevice    -151
#define ERR_InvalidDeviceType               -152
#define ERR_InvalidAxisNO                   -153
#define ERR_InvalidSetAxisGpio              -154
#define ERR_InvalidGetAxisGpio              -155
#define ERR_InvalidSpeed                    -156

#define ERR_Write6045RegisterInvalid        -160
#define ERR_Read6045RegisterInvalid         -161
#define ERR_GetDLLPath                      -170
#define ERR_GetDLLVersion                   -171
#define ERR_M204_SlowDownPointError         -200
#define ERR_M204_SpeedChange                -201
#define ERR_M204_AxisAlreadyStop            -202
#define ERR_M204_AxisArrayError             -203
