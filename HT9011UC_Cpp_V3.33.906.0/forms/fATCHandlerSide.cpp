// =============================================================================
//  forms/fATCHandlerSide.cpp  --  ACTIVE bodies for forms/fATCHandlerSide.h's
//  TATC_InterfaceForm facade.  See forms/fATCHandlerSide.h for the full
//  safety posture / gate register / deviation ledger.  This file defines
//  ONLY the 28 methods marked ACTIVE there; every GATED method declared in
//  the header is deliberately left undefined here.
//
//  AI(W906-FW3-ATC1) 20260827: new file, FW wave FW3-ATC1.
// =============================================================================
#include "forms/fATCHandlerSide.h"

#include "cmydef.h"    // Total_Compressor (cmydef.cpp, ht9045_globals)

// ===========================================================================
//  golden ATC_Handler_Side.cpp:26-171 command-index macros, reproduced
//  verbatim (plain file-scope #defines in golden, not class members -- 0
//  hits anywhere else in the port tree, re-verified 20260827).  These feed
//  SetCommandString's `asCommString[<ID>-1000]` array below; kept as the
//  COMPLETE golden set (not just the subset SetCommandString references)
//  so a future wave un-gating SendCommand/ProcessReceiveString_ATC finds
//  every ID already in place, and so an accidental miss here fails loudly
//  as an undefined-macro compile error rather than a silent gap.  Chinese
//  inline comments from golden are not reproduced (this file was decoded
//  cp950->UTF-8 this wave; dropping a comment changes no behaviour, unlike
//  dropping a numeric value, which would).
// ===========================================================================
#define ATC_NO_THIS_COMMAND            0001
#define ATC_RECIPE_FILE                1001
#define ATC_SET_TEMP                   1002
#define ATC_SET_TOFS                   1003
#define ATC_SITE_ENABLED               1004
#define ATC_SET_TRM                    1005
#define ATC_SET_MAX_TEMP               1006
#define ATC_RUN_STOP                   1007
#define ATC_INITIAL_SET                1008
#define ATC_MODE_TYPE                  1009
#define ATC_READ_TEMP                  1010
#define ATC_HANDLER_ARM                1011
#define ATC_TEST_START                 1012
#define ATC_SEND_TJ                    1013
#define ATC_ERROR                      1014
#define ATC_SET_SINGLE_TEMP            1015
#define ATC_SET_SINGLE_OFFSET          1016
#define ATC_CANCEL_MESSAGE             1017
#define ATC_CHILLER_STATUS             1018
#define ATC_SET_PID                    1019
#define ATC_READ_PID                   1020
#define ATC_USE_TSD                    1021
#define ATC_EMG_UP_DOWN                1022
#define ATC_READ_TEMP_2                1023
#define ATC_READ_SETTEMP               1024
#define ATC_STATE                      1025
#define ATC_RUN_MODULE                 1026
#define ATC_STOP_MODULE                1027
#define ATC_SET_2ND_RANGE_P            1028
#define ATC_SET_2ND_RANGE_N            1029
#define ATC_SET_2ND_TIME               1030
#define ATC_SW_VER                     1031
#define ATC_FW_VER                     1032
#define ATC_TIME_SYNC                  1033
#define ATC_SET_2ND_ENABLED            1034
#define ATC_READ_OFFSET                1035
#define ATC_GET_NOW_RECIPE             1036
#define ATC_GET_RECIPE_LIST            1037
#define ATC_USE_TJ_MODE                1038
#define ATC_SET_CHILLER_TEMP           1039
#define ATC_SEND_TEMP_READY            1040
#define ATC_SITE_2ND_CHECK             1041
#define ATC_SET_TJ_OFFSET              1042
#define ATC_SET_SINGLE_TJ_OFFSET       1043
#define ATC_RUN_SELFTEST               1044
#define ATC_GET_2ND_STATUS             1045
#define ATC_MANUAL_SELFTEST            1046
#define ATC_SELFTEST_STATUS            1047
#define HANDLER_STATUS                 1048
#define ATC_AIRMACHINE_STATUS          1049
#define ATC_LOT_START                  1050
#define ATC_LOT_END                    1051
#define ATC_SELFTEST_RESULT            1052
#define HANDLER_ABNORMAL_SITE          1053
#define ATC_GET_SN                     1054
#define ATC_SHUTDOWN                   1055
#define ATC_51_FREONRECOVER            1056
#define HANDLER_CAN_SELFTEST           1057
#define ATC_51_SET_REF_ENABLED         1058
#define ATC_51_GET_REF_STATUS          1059
#define ATC_51_SET_DEFROST             1060
#define ATC_60_SET_AIRVALVE            1061
#define ATC_READ_SOCKETTEMP            1062
#define HANDLER_FFC_SETTING            1063
#define HANDLER_ATO_RECORD             1064
#define HANDLER_FFC_ENABLED            1065
#define HANDLER_FFC_TRIGGER            1066
#define ATC_GET_CONTROL_MODE           1067
#define HANDLER_PFC_ENABLED            1068
#define ATC_GET_PFC_PARAMETER          1069
#define ATC_SET_PFC_PARAMETER          1070
#define ATC_READ_WATER_VALVE           1071
#define ATC_CMD_READ_FUNCTION_STATUS   1072
#define ATC_GET_HEAT_OUTPUT            1073
#define ATC_AUTO_TCTS_ENABLED          1074
#define HANDLER_TEST_SITEMAPPING       1075
#define ATC_SET_TJ_ENABLED             1076
#define ATC_SET_TJ_PARAMETER           1077
#define ATC_51_SET_ExtraHeatGun        1078
#define ATC_51_READ_ExtraHeatGun       1079
#define ATC_SWAP_CH2CH3_DATALOG        1080
#define ATC_READ_TCTJ_WATER_VALVE      1081
#define ATC_READ_TJ_PARAMETER          1082
#define ATC_READ_HEATING               1083
#define ATC_GET_SLOPE_OFFSET           1084
#define ATC_SET_SLOPE_OFFSET           1085
#define ATC_FEEDBACK_VOLTAGE           1086
#define ATC_RECORD_TJ_TEMP             1087
#define ATC_QUERY_TJ_TEMP              1088
#define ATC_51_SET_REF_ADJUST          1089
#define ATC_51_AUTOLOAD_RECIPE         1090
#define HANDLER_SLK_LAYOUT             1091
#define ATC_GET_HEATER_POWER           1092
#define ATC_KL_SV                      1093
#define ATC_KL_OFFSET                  1094
#define ATC_KL_TRIGGER                 1095
#define ATC_KL_TRIGGER_STATUS          1096
#define ATC_READ_TEMP_3                1097
#define ATC_LOAD_RECIPE_SINGLE         1098
#define ATC_GETDEWPOINTTEMP            1099
#define ATC_51_SET_TJ_WATCHDOG         1100
#define ATC_AIRMACHINE_TEMP            1101
#define ATC_SAVE_RECIPE_PROHIBIT       1102
#define ATC_READ_CHILLER_TEMP          1103
#define ATC_SET_PF_PARAMETER           1104
#define ATC_SET_T2OFS                  1105
#define ATC_SET_SINGLE_T2OFS           1106
#define ATC_SET_HEAT_OUTPUT_LIMIT      1107
#define ATC_GET_CTRL_MODE              1108
#define ATC_READ_HYGON_STATUS          1109
#define ATC_READ_WATER_FLOW            1110
#define ATC_SET_CoolingValue           1111
#define ASIF_TJ_EFUSED                 1112
#define ASIF_TJ_REQUEST                1113
#define ASIF_TJ_FB                     1114
#define ASIF_TJ_EOT                    1115
#define ASIF_TJ_CPU                    1116
#define ASIF_TJ_SOC                    1117
#define ASIF_TJ_GPU                    1118
#define ASIF_TJ_FPC                    1119
#define HANDLER_2DID                   1120
#define ATC_SET_WValve_ENABLED         1121
#define ATC_Recipe_By_Channel          1122
#define ATC_Recipe_By_Public           1123
#define ATC_SET_CHILLER_ENABLED        1124
#define ATC_SET_TC_WATER_VALVE         1125
#define HANDLER_ALARM                  1126
#define ATC_SET_DYNAMIC_PID            1127
#define ATC_READ_DYNAMIC_PID           1128
#define ATC_SET_MULTI_TC_OFFSET        1129
#define ATC_CHILLER_WATERWARNING       1130
#define ATC_Multi_Temperature_Control  1131
#define ATC_Handler_Transmit_Recipe    1132
#define ATC_DEFROSTING                 1133
#define ATC_Star_Transmit_Recipe       1134
#define ATC_SET_SINGLE_OFFSET_MTP      1135
#define ATC_51_ATO_AdjustValve         1136
#define ATC_HulkMode                   1137

// ---------------------------------------------------------------------------
//  TATC_InterfaceForm ctor -- golden :338-469, PORT SPLIT (DEVIATION D-2 in
//  forms/fATCHandlerSide.h -- see there for the full kept/dropped line
//  ledger).  Kept: fields no widget/occupied-global touches, needed by an
//  ACTIVE method or NAME RESERVED.
// ---------------------------------------------------------------------------
TATC_InterfaceForm::TATC_InterfaceForm()
{
    IsConnectFlag = false;                                                     // golden :341
    bConnecting = false;                                                       // golden :342
    asATC_SW_Ver = "";                                                         // golden :347 (NAME RESERVED)
    iATC_MODE_TYPE = ATC_TYPE_UNSET;                                           // golden :349 (NAME RESERVED)
    ReadTempInterval = 1000;                                                   // golden :355
    iATC_ModuleCount = 1;                                                      // golden :357

    sAlarmMessageList = new TStringList();                                     // golden :390
    sAlarmMessageList->Clear();                                                // golden :391

    for (int i = 0; i < ATC_MAX_SITE; i++)                                     // golden :393-432, trimmed to the one
        ATC_Module[i] = false;                                                 // ACTIVE-needed field (SetModuleRunStop)

    LastMsg = "";                                                              // golden :448
    SetCommandString();                                                        // golden :449 (this class's own ACTIVE method)

    for (int i = 0; i < iATC_Refrigerator_Num + 1; i++)                        // golden :451-455
    {
        iATC_RefrigeratorUserMode[0][i] = -1;
        iATC_RefrigeratorUserMode[1][i] = 1;
    }

    for (int i = 0; i < iATC_Refrigerator_Num; i++)                            // golden :457-463
        for (int j = 0; j < 9; j++)
            dATC_RefrigeratorAllStatus[i][j] = -9999.0;

    dATC_RefrigerantMachineHighLimit = -9999.0;                                // golden :464
    dATC_RefrigerantMachineLowLimit = -9999.0;                                 // golden :465
    bReadRefrigerantMode_AllStatus = false;                                    // golden :466
    bReadRefrigerantMode_Send = false;                                         // golden :467
    bReadRefrigerantMode_Recv = false;                                         // golden :468
}

// ---------------------------------------------------------------------------
//  SetCommandString -- golden :200-336.  Pure lookup-table builder: every
//  statement is `asCommString[<ID>-1000]="<literal>";`.  Translated in
//  FULL -- no widget, no I/O, no SendCommand.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::SetCommandString()
{
    asCommString[ATC_RECIPE_FILE               -1000] = "ATC Change Recipe File(1001)";
    asCommString[ATC_SET_TEMP                  -1000] = "Set ATC Run Temperature(1002)";
    asCommString[ATC_SET_TOFS                  -1000] = "Set Temp. Offset(1003)";
    asCommString[ATC_SITE_ENABLED               -1000] = "Set Use Site(1004)";
    asCommString[ATC_SET_TRM                    -1000] = "Set Temp. Reading Mode(1005)";
    asCommString[ATC_SET_MAX_TEMP               -1000] = "Set ATC MAX Working Temperature(1006)";
    asCommString[ATC_RUN_STOP                   -1000] = "ATC Run/Stop(1007)";
    asCommString[ATC_INITIAL_SET                -1000] = "ATC Initial all Setting(1008)";
    asCommString[ATC_MODE_TYPE                  -1000] = "ATC Mode Type(1009)";
    asCommString[ATC_READ_TEMP                  -1000] = "Read ATC Temperature(1010)";
    asCommString[ATC_HANDLER_ARM                -1000] = "Inform Handler Use Arm(1011)";
    asCommString[ATC_TEST_START                 -1000] = "Inform Handler Testing or Test finish(1012)";
    asCommString[ATC_SEND_TJ                    -1000] = "Send TJ Temperature to ATC(1013)";
    asCommString[ATC_ERROR                      -1000] = "ATC Error Message(1014)";
    asCommString[ATC_SET_SINGLE_TEMP            -1000] = "Set Single Site Temperature(1015)";
    asCommString[ATC_SET_SINGLE_OFFSET          -1000] = "Set Single Site Offset(1016)";
    asCommString[ATC_CANCEL_MESSAGE             -1000] = "Inform ATC Close Alarm Form(1017)";
    asCommString[ATC_CHILLER_STATUS             -1000] = "Query Chiller ststus(1018)";
    asCommString[ATC_SET_PID                    -1000] = "Set ATC PID(1019)";
    asCommString[ATC_READ_PID                   -1000] = "Read ATC PID(1020)";
    asCommString[ATC_USE_TSD                    -1000] = "Use TSD Function(1021)";
    asCommString[ATC_EMG_UP_DOWN                -1000] = "Handler EMG Up or Down(1022)";
    asCommString[ATC_READ_TEMP_2                -1000] = "Read ATC Second Temperature(1023)";
    asCommString[ATC_READ_SETTEMP               -1000] = "Read ATC Set Temp(1024)";
    asCommString[ATC_STATE                      -1000] = "Read ATC State(1025)";
    asCommString[ATC_RUN_MODULE                 -1000] = "Run ATC Module(1026)";
    asCommString[ATC_STOP_MODULE                -1000] = "Stop ATC Module(1027)";
    asCommString[ATC_SET_2ND_RANGE_P            -1000] = "Set 2nd temperature Positive range(1028)";
    asCommString[ATC_SET_2ND_RANGE_N            -1000] = "Set 2nd temperature Negative range(1029)";
    asCommString[ATC_SET_2ND_TIME               -1000] = "Set 2nd temperature Interval time(1030)";
    asCommString[ATC_SW_VER                     -1000] = "ATC Software Version(1031)";
    asCommString[ATC_FW_VER                     -1000] = "ATC Firmware Version(1032)";
    asCommString[ATC_TIME_SYNC                  -1000] = "Time synchronization to Handler(1033)";
    asCommString[ATC_SET_2ND_ENABLED            -1000] = "Set 2nd temperature Function enabled(1034)";
    asCommString[ATC_READ_OFFSET                -1000] = "Read ATC Set Offset(1035)";
    asCommString[ATC_GET_NOW_RECIPE             -1000] = "Read Now Recipe File(1036)";
    asCommString[ATC_GET_RECIPE_LIST            -1000] = "Get Recipe File List(1037)";
    asCommString[ATC_USE_TJ_MODE                -1000] = "ATC Use TJ Mode(1038)";
    asCommString[ATC_SET_CHILLER_TEMP           -1000] = "ATC Set Chiller Temp(1039)";
    asCommString[ATC_SEND_TEMP_READY            -1000] = "ATC Send Temp Ready(1040)";
    asCommString[ATC_SITE_2ND_CHECK             -1000] = "ATC_SITE_2ND_CHECK(1041)";
    asCommString[ATC_SET_TJ_OFFSET              -1000] = "ATC_SET_TJ_OFFSET(1042)";
    asCommString[ATC_SET_SINGLE_TJ_OFFSET       -1000] = "ATC_SET_SINGLE_TJ_OFFSET(1043)";
    asCommString[ATC_RUN_SELFTEST               -1000] = "ATC_RUN_SELFTEST(1044)";
    asCommString[ATC_GET_2ND_STATUS             -1000] = "Get_Second_Sensor_Function(1045)";
    asCommString[ATC_MANUAL_SELFTEST            -1000] = "Manual ATC selt-test functin(1046)";
    asCommString[ATC_SELFTEST_STATUS            -1000] = "Get self-test status(1047)";
    asCommString[HANDLER_STATUS                 -1000] = "Check Handler Status(1048)";
    asCommString[ATC_AIRMACHINE_STATUS          -1000] = "Query AirMachine ststus(1049)";
    asCommString[ATC_LOT_START                  -1000] = "ATC_LOT_START(1050)";
    asCommString[ATC_LOT_END                    -1000] = "ATC_LOT_END(1051)";
    asCommString[ATC_SELFTEST_RESULT            -1000] = "ATC_SELFTEST_RESULT(1052)";
    asCommString[HANDLER_ABNORMAL_SITE          -1000] = "HANDLER_ABNORMAL_SITE(1053)";
    asCommString[ATC_GET_SN                     -1000] = "ATC_GET_SN(1054)";
    asCommString[ATC_SHUTDOWN                   -1000] = "ATC_SHUTDOWN(1055)";
    asCommString[ATC_51_FREONRECOVER            -1000] = "ATC51 FREONRE COVER(1056)";
    asCommString[HANDLER_CAN_SELFTEST           -1000] = "HANDLER_CAN_SELFTEST(1057)";
    asCommString[ATC_51_SET_REF_ENABLED         -1000] = "ATC51 SET REF ENABLED(1058)";
    asCommString[ATC_51_GET_REF_STATUS          -1000] = "ATC51 GET REF STATUS(1059)";
    asCommString[ATC_51_SET_DEFROST             -1000] = "ATC_51_SET_DEFROST(1060)";
    asCommString[ATC_60_SET_AIRVALVE            -1000] = "Setting open/close air valve(1061)";
    asCommString[ATC_READ_SOCKETTEMP            -1000] = "Read ATC socket temperature(1062)";
    asCommString[HANDLER_FFC_SETTING            -1000] = "HANDLER FFC SETTING(1063)";
    asCommString[HANDLER_ATO_RECORD             -1000] = "Record Auto Temperature Offset data(1064)";
    asCommString[HANDLER_FFC_ENABLED            -1000] = "HANDLER FFC ENABLED(1065)";
    asCommString[HANDLER_FFC_TRIGGER            -1000] = "HANDLER FFC TRIGGER(1066)";
    asCommString[ATC_GET_CONTROL_MODE           -1000] = "ATC_GET_CONTROL_MODE(1067)";
    asCommString[HANDLER_PFC_ENABLED            -1000] = "Power Following Control ENABLED(1068)";
    asCommString[ATC_GET_PFC_PARAMETER          -1000] = "ATC_GET_PFC_PARAMETER(1069)";
    asCommString[ATC_SET_PFC_PARAMETER          -1000] = "ATC_SET_PFC_PARAMETER(1070)";
    asCommString[ATC_READ_WATER_VALVE           -1000] = "ATC_READ_WATER_VALVE(1071)";
    asCommString[ATC_CMD_READ_FUNCTION_STATUS   -1000] = "ATC_CMD_READ_FUNCTION_STATUS(1072)";
    asCommString[ATC_GET_HEAT_OUTPUT            -1000] = "ATC_GET_HEAT_OUTPUT(1073)";
    asCommString[ATC_AUTO_TCTS_ENABLED          -1000] = "ATC_AUTO_TCTS_ENABLED(1074)";
    asCommString[ATC_GETDEWPOINTTEMP            -1000] = "ATC_GETDEWPOINTTEMP(1099)";

    asCommString[HANDLER_TEST_SITEMAPPING       -1000] = "HANDLER_TEST_SITEMAPPING(1075)";
    asCommString[ATC_SET_TJ_ENABLED             -1000] = "ATC_SET_TJ_ENABLED(1076)";
    asCommString[ATC_SET_TJ_PARAMETER           -1000] = "ATC_SET_TJ_PARAMETER(1077)";
    asCommString[ATC_51_SET_ExtraHeatGun        -1000] = "ATC_51_SET_ExtraHeatGun(1078)";
    asCommString[ATC_51_READ_ExtraHeatGun       -1000] = "ATC_51_READ_ExtraHeatGun(1079)";
    asCommString[ATC_SWAP_CH2CH3_DATALOG        -1000] = "ATC_SWAP_CH2CH3_DATALOG(1080)";
    asCommString[ATC_READ_TCTJ_WATER_VALVE      -1000] = "ATC_READ_TCTJ_WATER_VALVE(1081)";
    asCommString[ATC_READ_TJ_PARAMETER          -1000] = "ATC_READ_TJ_PARAMETER(1082)";
    asCommString[ATC_READ_HEATING               -1000] = "ATC_READ_HEATING(1083)";
    asCommString[ATC_GET_SLOPE_OFFSET           -1000] = "ATC_GET_SLOPE_OFFSET(1084)";
    asCommString[ATC_SET_SLOPE_OFFSET           -1000] = "ATC_SET_SLOPE_OFFSET(1085)";
    asCommString[ATC_FEEDBACK_VOLTAGE           -1000] = "ATC_FEEDBACK_VOLTAGE(1086)";
    asCommString[ATC_RECORD_TJ_TEMP             -1000] = "ATC_RECORD_TJ_TEMP(1087)";
    asCommString[ATC_QUERY_TJ_TEMP              -1000] = "ATC_QUERY_TJ_TEMP(1088)";
    asCommString[ATC_51_SET_REF_ADJUST          -1000] = "Read ATC5.1 set refrigerator valve(1089)";
    asCommString[ATC_51_AUTOLOAD_RECIPE         -1000] = "ATC5.1 Autoload golden recipe by Temp(1090)";
    asCommString[HANDLER_SLK_LAYOUT             -1000] = "HANDLER_SLK_LAYOUT(1091)";
    asCommString[ATC_GET_HEATER_POWER           -1000] = "Get the Heater Output Power(1092)";
    asCommString[ATC_KL_SV                      -1000] = "KL set temp to Handler(1093)";
    asCommString[ATC_KL_OFFSET                  -1000] = "KL set offset to Handler(1094)";
    asCommString[ATC_KL_TRIGGER                 -1000] = "KL set start KL control by Handler(1095)";
    asCommString[ATC_KL_TRIGGER_STATUS          -1000] = "KL set start Handler read status(1096)";
    asCommString[ATC_READ_TEMP_3                -1000] = "Read ATC Multi Temperature(1097)";
    asCommString[ATC_LOAD_RECIPE_SINGLE         -1000] = "ATC_LOAD_RECIPE_SINGLE(1098)";
    asCommString[ATC_GETDEWPOINTTEMP            -1000] = "ATC_GETDEWPOINTTEMP(1099)";
    asCommString[ATC_51_SET_TJ_WATCHDOG         -1000] = "ATC_51_SET_TJ_WATCHDOG(1100)";
    asCommString[ATC_AIRMACHINE_TEMP            -1000] = "Read Air Machine Temperature(1101)";
    asCommString[ATC_SAVE_RECIPE_PROHIBIT       -1000] = "Saving recipe data is prohibited(1102)";
    asCommString[ATC_READ_CHILLER_TEMP          -1000] = "Read chiller temperature(1103)";
    asCommString[ATC_SET_PF_PARAMETER           -1000] = "Set PF slope and offset (1104)";
    asCommString[ATC_SET_T2OFS                  -1000] = "Set Tc2 Offset(1105)";
    asCommString[ATC_SET_SINGLE_T2OFS           -1000] = "Set Single Site Tc2 Offset(1106)";
    asCommString[ATC_SET_HEAT_OUTPUT_LIMIT      -1000] = "Set Heat Output Limit(1107)";
    asCommString[ATC_GET_CTRL_MODE              -1000] = "Get ATC Controller Control Mode(1108)";
    asCommString[ATC_READ_HYGON_STATUS          -1000] = "READ HYGON_STATUS(1109)";
    asCommString[ATC_READ_WATER_FLOW            -1000] = "Read water flow value(1110)";
    asCommString[ATC_SET_CoolingValue           -1000] = "SET Cooling Value(1111)";
    asCommString[ASIF_TJ_EFUSED                 -1000] = "ASIF_TJ_EFUSED(1112)";
    asCommString[ASIF_TJ_REQUEST                -1000] = "ASIF_TJ_REQUEST(1113)";
    asCommString[ASIF_TJ_FB                     -1000] = "ASIF_TJ_FB(1114)";
    asCommString[ASIF_TJ_EOT                    -1000] = "ASIF_TJ_EOT(1115)";
    asCommString[ASIF_TJ_CPU                    -1000] = "ASIF_TJ_CPU(1116)";
    asCommString[ASIF_TJ_SOC                    -1000] = "ASIF_TJ_SOC(1117)";
    asCommString[ASIF_TJ_GPU                    -1000] = "ASIF_TJ_GPU(1118)";
    asCommString[ASIF_TJ_FPC                    -1000] = "ASIF_TJ_FPC(1119)";

    asCommString[HANDLER_2DID                   -1000] = "HANDLER_2DID(1120)";
    asCommString[ATC_Recipe_By_Channel          -1000] = "ATC_Recipe_By_Channel(1122)";
    asCommString[ATC_Recipe_By_Public           -1000] = "ATC_Recipe_By_Public(1123)";
    asCommString[ATC_SET_TC_WATER_VALVE         -1000] = "ATC_SET_TC_WATER_VALVE(1125)";
    asCommString[HANDLER_ALARM                  -1000] = "Handler To ATC Alarm Message(1126)";
    asCommString[ATC_SET_DYNAMIC_PID            -1000] = "Set ATC Dynamic PID(1127)";
    asCommString[ATC_READ_DYNAMIC_PID           -1000] = "ATC_READ_DYNAMIC_PID(1128)";
    asCommString[ATC_SET_MULTI_TC_OFFSET        -1000] = "ATC_SET_MULTI_TC_OFFSET(1129)";

    asCommString[ATC_Multi_Temperature_Control  -1000] = "ATC_Multi_Temperature_Control(1131)";
    asCommString[ATC_Handler_Transmit_Recipe    -1000] = "ATC_Handler_Transmit_Recipe(1132)";
    asCommString[ATC_Star_Transmit_Recipe       -1000] = "ATC_Star_Transmit_Recipe(1134)";
}

// ---------------------------------------------------------------------------
//  IsConnect / IsConnecting -- golden :1910-1918.  Pure getters.
// ---------------------------------------------------------------------------
bool TATC_InterfaceForm::IsConnect()
{
    return IsConnectFlag;
}

bool TATC_InterfaceForm::IsConnecting()
{
    return bConnecting;
}

// ---------------------------------------------------------------------------
//  SetReadTempInterval -- golden :1833-1836.  Pure field setter.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::SetReadTempInterval(int iInterval)
{
    ReadTempInterval = iInterval;
}

// ---------------------------------------------------------------------------
//  HasAlarmMsg -- golden :2479-2485.  Pure query over own sAlarmMessageList.
// ---------------------------------------------------------------------------
bool TATC_InterfaceForm::HasAlarmMsg()
{
    if (sAlarmMessageList->Count < 1)
        return false;
    else
        return true;
}

// ---------------------------------------------------------------------------
//  PushBackMessage -- golden :553-561.  Pure de-dup insert into own
//  sAlarmMessageList / LastMsg -- no widget, no SendCommand.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::PushBackMessage(AnsiString aMsg)
{
    for (int i = 0; i < sAlarmMessageList->Count; i++)
    {
        if (LastMsg == aMsg || sAlarmMessageList->Strings[i] == aMsg)
            return;
    }
    sAlarmMessageList->Add(aMsg);
}

// ---------------------------------------------------------------------------
//  GetSetTemp / GetSetOffset -- golden :2515-2525 / :2847-2857.  Pure
//  bounds-checked reads of own dNowATCSetTemp[] / dNowATCSetOffset[].
// ---------------------------------------------------------------------------
double TATC_InterfaceForm::GetSetTemp(int iChannel)
{
    if (iChannel < ATC_MAX_SITE)
        return dNowATCSetTemp[iChannel];
    else
        return dNowATCSetTemp[0];
}

double TATC_InterfaceForm::GetSetOffset(int iChannel)
{
    if (iChannel < ATC_MAX_SITE)
        return dNowATCSetOffset[iChannel];
    else
        return dNowATCSetOffset[0];
}

// ---------------------------------------------------------------------------
//  CheckATCSetTemp -- golden :2527-2540.  Pure comparison loop over own
//  dNowATCSetTemp[].
// ---------------------------------------------------------------------------
bool TATC_InterfaceForm::CheckATCSetTemp(double dSettemp)
{
    for (int i = 0; i < ATC_MAX_SITE; i++)
    {
        if (i < ATC_MAX_SITE)
        {
            if (dNowATCSetTemp[i] != dSettemp)
            {
                return true;
            }
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
//  GetRunState -- golden :2542-2545.  Pure getter (NAME RESERVED).
// ---------------------------------------------------------------------------
bool TATC_InterfaceForm::GetRunState()
{
    return bATCRun;
}

// ---------------------------------------------------------------------------
//  SetATC_ModuleCount / SetModuleRunStop -- golden :2547-2558.  Pure
//  bounds-clamped setter / copy loop over own fields.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::SetATC_ModuleCount(int iCount)
{
    iATC_ModuleCount = iCount;
    if (iATC_ModuleCount <= 0 || iATC_ModuleCount > 8)
        iATC_ModuleCount = 1;
}

void TATC_InterfaceForm::SetModuleRunStop(bool *Enabled)
{
    for (int i = 0; i < iATC_ModuleCount; i++)
        ATC_Module[i] = Enabled[i];
}

// ---------------------------------------------------------------------------
//  GetRunSelfTestResult / GetRunSelfTestStatus / GetATCLotStartStatus /
//  GetATCLotEndStatus -- golden :2606-2624.  Pure getters.
// ---------------------------------------------------------------------------
int TATC_InterfaceForm::GetRunSelfTestResult()
{
    return flagATCSelfTestResult;
}

int TATC_InterfaceForm::GetRunSelfTestStatus()
{
    return iATCSelfTestStatus;
}

bool TATC_InterfaceForm::GetATCLotStartStatus()
{
    return bATCLotStartStatus;
}

bool TATC_InterfaceForm::GetATCLotEndStatus()
{
    return bATCLotEndStatus;
}

// ---------------------------------------------------------------------------
//  ReadTC / ReadTC2 / ReadTJ -- golden :2360-2388.  Pure bounds-checked
//  copy from own dTC[] / dTC2[] / dTJ[] into the caller's buffer.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::ReadTC(int iChCount, double *TempTC)
{
    if (iChCount < 0 || iChCount > ATC_MAX_SITE)
    {
        iChCount = 4;
    }
    for (int i = 0; i < iChCount; i++)
        TempTC[i] = dTC[i];
}

void TATC_InterfaceForm::ReadTC2(int iChCount, double *TempTC2)
{
    if (iChCount < 0 || iChCount > ATC_MAX_SITE)
    {
        iChCount = 4;
    }
    for (int i = 0; i < iChCount; i++)
        TempTC2[i] = dTC2[i];
}

void TATC_InterfaceForm::ReadTJ(int iChCount, double *TempTJ)
{
    if (iChCount < 0 || iChCount > ATC_MAX_SITE)
    {
        iChCount = 4;
    }
    for (int i = 0; i < iChCount; i++)
        TempTJ[i] = dTJ[i];
}

// ---------------------------------------------------------------------------
//  ReadPID / SetPID -- golden :2390-2396.  Golden's ENTIRE body is `{}` in
//  both cases (GOLDEN NOTE G-2) -- translated verbatim.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::ReadPID(int * /*kP*/, double * /*kI*/, int * /*kD*/)
{
}

void TATC_InterfaceForm::SetPID(int /*kP*/, double /*kI*/, int /*kD*/)
{
}

// ---------------------------------------------------------------------------
//  Get_ATCRefrigeratorAtcSetMode -- golden :3267-3286.  Pure read/copy over
//  own iATC_RefrigeratorUserMode[][], bounded by Total_Compressor
//  (cmydef.cpp, ht9045_globals).  NAME RESERVED.
// ---------------------------------------------------------------------------
int TATC_InterfaceForm::Get_ATCRefrigeratorAtcSetMode(int iRefrigeratorNum, int *iTempRefrigeratorUserMode)
{
    int iReturnStatus = false;
    if (iRefrigeratorNum < 0 || iRefrigeratorNum > Total_Compressor)
    {
        iRefrigeratorNum = 8;
    }

    if (iATC_RefrigeratorUserMode[0][Total_Compressor] != 2)
    {
        iReturnStatus = iATC_RefrigeratorUserMode[0][Total_Compressor];
        return iReturnStatus;
    }
    for (int i = 0; i < iRefrigeratorNum + 1; i++)
    {
        iTempRefrigeratorUserMode[i] = iATC_RefrigeratorUserMode[0][i];
    }
    iReturnStatus = 1;
    return iReturnStatus;
}

// ---------------------------------------------------------------------------
//  A31_Read_TempClick -- golden :1710-1714.  Calls this class's own ACTIVE
//  ReadTC; `Sender` unused matching golden (DEVIATION D-5).
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::A31_Read_TempClick(TObject * /*Sender*/)
{
    double dTemp[ATC_MAX_SITE];
    ReadTC(ATC_MAX_SITE, dTemp);
}

// ---------------------------------------------------------------------------
//  Four click handlers whose ENTIRE golden body is commented-out statements
//  (GOLDEN NOTE G-1 in forms/fATCHandlerSide.h) -- translated as empty
//  functions, which is golden's ACTUAL runtime behaviour, not a degraded
//  translation.
// ---------------------------------------------------------------------------
void TATC_InterfaceForm::A31_Set_Temp_Reading_ModeClick(TObject * /*Sender*/)                  // golden :959-964
{
}

void TATC_InterfaceForm::A31_Initial_SetClick(TObject * /*Sender*/)                            // golden :972-977
{
}

void TATC_InterfaceForm::Button2Click(TObject * /*Sender*/)                                    // golden :1871-1874
{
}

void TATC_InterfaceForm::A31_Chiller_StatusClick(TObject * /*Sender*/)                         // golden :1876-1880
{
}
