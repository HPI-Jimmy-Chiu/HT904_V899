//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "ATC_Handler_Side.h"
#include "uLotInfo.h"
#include "uTemp_Set.h"
#include "cprod.h"
#include "cMydef.h"
#include "cMyDB.h"
#include "common.h"
#include "database.h"
#include "aseTest.h"
#include "MessageDef.h"
#include "main.h"
#include "FileTransfer.h"
#include "FTPClient.h"
#include "ExternFunction.h"    //Eastsun 20260522 整合: DeleteDirectory
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TATC_InterfaceForm *ATC_InterfaceForm;
//---------------------------------------------------------------------------
//Control Command
//---------------------------------------------------------------------------
#define ATC_NO_THIS_COMMAND         0001                                        //ATC not Support this command
#define ATC_RECIPE_FILE             1001                                        //ATC Change Recipe File
#define ATC_SET_TEMP                1002                                        //Set ATC Run Temperature
#define ATC_SET_TOFS                1003                                        //Temp. Offset
#define ATC_SITE_ENABLED            1004                                        //Set Use Site
#define ATC_SET_TRM                 1005                                        //Temp. Reading Mode
#define ATC_SET_MAX_TEMP            1006                                        //Set ATC MAX Working Temperature
#define ATC_RUN_STOP                1007                                        //ATC Run/Stop
#define ATC_INITIAL_SET             1008                                        //ATC Initial all Setting
#define ATC_MODE_TYPE               1009                                        //ATC Mode Type
#define ATC_READ_TEMP               1010                                        //Read ATC Temperature
#define ATC_HANDLER_ARM             1011                                        //Inform Handler Use Arm
#define ATC_TEST_START              1012                                        //Inform Handler Testing or Test finish
#define ATC_SEND_TJ                 1013                                        //Send TJ Temperature to ATC            //Not been used
#define ATC_ERROR                   1014                                        //ATC Error Message
#define ATC_SET_SINGLE_TEMP         1015                                        //Set Single Site Temperature
#define ATC_SET_SINGLE_OFFSET       1016                                        //Set Single Site Offset
#define ATC_CANCEL_MESSAGE          1017                                        //Inform ATC Close Alarm Form
#define ATC_CHILLER_STATUS          1018                                        //Query Chiller ststus                  //For ATC 3.0/6.0
#define ATC_SET_PID                 1019                                        //Set ATC PID
#define ATC_READ_PID                1020                                        //Read ATC PID
#define ATC_USE_TSD                 1021                                        //Use TSD Function                      //For ATC 7.0
#define ATC_EMG_UP_DOWN             1022                                        //Handler EMG Up or Down                //For ATC 7.0
#define ATC_READ_TEMP_2             1023                                        //Read ATC Second Temperature
#define ATC_READ_SETTEMP            1024                                        //Read ATC Set Temp          //ChungHung 20150826 add
#define ATC_STATE                   1025                                        //Read ATC State             //ChungHung 20150826 add
#define ATC_RUN_MODULE              1026                                        //Run ATC Module                        //For ATC 5.0
#define ATC_STOP_MODULE             1027                                        //Stop ATC Module                       //For ATC 5.0
#define ATC_SET_2ND_RANGE_P         1028                                        //Setting second point temperature Positive range.  //通知ATC設定第二感溫點Alarm 範圍正極限
#define ATC_SET_2ND_RANGE_N         1029                                        //Setting second point temperature Negative range.  //通知ATC設定第二感溫點Alarm 範圍負極限
#define ATC_SET_2ND_TIME            1030                                        //Setting second point temperature interval time.   //通知ATC設定第二感溫點Alarm 時間
#define ATC_SW_VER                  1031                                        //ATC Software Version                  //Eliot 2015_1007
#define ATC_FW_VER                  1032                                        //ATC Firmware Version                  //Eliot 2015_1007
#define ATC_TIME_SYNC               1033                                        //Time synchronization to Handler       //Eliot 2015_1017
#define ATC_SET_2ND_ENABLED         1034                                        //Setting second point temperature Function enabled.//通知ATC設定第二感溫點功能
#define ATC_READ_OFFSET             1035                                        //Read ATC Set Offset                   //Eliot 2015_1124
#define ATC_GET_NOW_RECIPE          1036                                        //Get Now Recipe File                   //Eliot 2015_1126
#define ATC_GET_RECIPE_LIST         1037                                        //Get Recipe File List                  //Eliot 2015_1126
#define ATC_USE_TJ_MODE             1038                                        //SET TJ MODE for AMD   kevin 20160425 add
#define ATC_SET_CHILLER_TEMP        1039                                        //SET chiller temp     kevin 20160425 add
#define ATC_SEND_TEMP_READY         1040                                        //send temperature ready                //2016-04-27    Dell    Add
#define ATC_SITE_2ND_CHECK          1041                                        //Site Check Second Temperature         //Eliot 2016_0503
#define ATC_SET_TJ_OFFSET           1042
#define ATC_SET_SINGLE_TJ_OFFSET    1043
#define ATC_RUN_SELFTEST            1044                                        //Run ATC selt-test functin             //2016/7/17 Dell    Add
#define ATC_GET_2ND_STATUS          1045                                        //Get second sensor function enabled.   //2016-08-01    Dell    Add
#define ATC_MANUAL_SELFTEST         1046                                        //Manual ATC selt-test functin          //Eliot 2016_0816
#define ATC_SELFTEST_STATUS         1047                                        //Get self-test status                  //Eliot 2016_0816
#define HANDLER_STATUS              1048                                        //Check Handler Status                  //Eliot 2016_0816
#define ATC_AIRMACHINE_STATUS       1049                                        //Query Chiller ststus                  //20160922 Zek For ATC 5.1
#define ATC_LOT_START               1050                                        //Lot Start, Lot ID                     //Eliot 2016_1020
#define ATC_LOT_END                 1051                                        //Lot End                               //Eliot 2016_1020
#define ATC_SELFTEST_RESULT         1052                                        //Query Self-Test Result                //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
#define HANDLER_ABNORMAL_SITE       1053                                        //Handler send abnormal site to ATC     //Eliot 20170116 Handler傳送有異常的Site給ATC
#define ATC_GET_SN                  1054                                        //Get ATC power supply serial number    //20170202W05   Dell    Add Get ATC power supply serial number
#define ATC_SHUTDOWN                1055                                        //ATC SHUTDOWN                          //2017-08-03    Dell for 1028c
#define ATC_51_FREONRECOVER         1056                                        //ATC FREON RECOVER                     //2017-09-14    Dell for 勝麗
#define HANDLER_CAN_SELFTEST        1057                                        //ATC Query Handler can self-test?          //Eliot 20171211
#define ATC_51_SET_REF_ENABLED      1058                                        //Setting refrigerator enabled.         //2018-06-06    Dell for HT3012CT
#define ATC_51_GET_REF_STATUS       1059                                        //Query refrigerator Status.            //2018-06-06    Dell for HT3012CT
#define ATC_51_SET_DEFROST          1060                                        //Setting ATC defrost.                  //2018-07-16    Dell for ASE_CL
#define ATC_60_SET_AIRVALVE         1061                                        //Setting open/close air valve          //2018-10-31    Dell for Qualcomm //Steven 20181102 : Control ATC6.0氣流開關
#define ATC_READ_SOCKETTEMP         1062
#define HANDLER_FFC_SETTING         1063                                        //Ifor 20190730 : add FFC Temperature Offset
#define HANDLER_ATO_RECORD          1064                                        //Ztex 2023.04.18 Add HT-1032 ATC CMD
#define HANDLER_FFC_ENABLED         1065                                        //Ifor 20190730 : add FFC Temperature Offset
#define HANDLER_FFC_TRIGGER         1066                                        //Ifor 20190730 : add FFC Temperature Offset
#define ATC_GET_CONTROL_MODE        1067                                        //Controller Control Mode Status        //Eliot 20190619
#define HANDLER_PFC_ENABLED         1068                                        //Power Following Control ENABLED       //Eliot 20190916
#define ATC_GET_PFC_PARAMETER       1069                                        //讀取單一溫度的PF參數值
#define ATC_SET_PFC_PARAMETER       1070                                        //設定單一溫度的PF參數值
#define ATC_READ_WATER_VALVE        1071                                        //Read ATC Water valve value            //Eliot 20200406
#define ATC_CMD_READ_FUNCTION_STATUS 1072                                        //Read ATC Function Status
                                                                                /* Handler ' ATC    @1071,1,n,#
                                                                                                                說明:n代表要回傳多少Channel的資料
                                                                                ATC ' Handler    @1071,4,100,100,50,50,#
                                                                                                                說明:@指令,資料數量,Ch1開度, Ch2開度, Ch3開度, Ch4開度,#
                                                                                                                        開度0~100代表0~100%
                                                                                */
#define ATC_GET_HEAT_OUTPUT         1073                                        //Read ATC Heat output value            //2020-07-16    Dell
#define ATC_AUTO_TCTS_ENABLED       1074                                        //Handler send site mapping             //2021-07-20    Dell
#define HANDLER_TEST_SITEMAPPING    1075                                        //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Handler send site mapping
#define ATC_SET_TJ_ENABLED          1076                                        //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ Function enabled               //2021-07-20    Dell
#define ATC_SET_TJ_PARAMETER        1077                                        //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ slope and offset               //2021-07-20    Dell
#define ATC_51_SET_ExtraHeatGun     1078
#define ATC_51_READ_ExtraHeatGun    1079
#define ATC_SWAP_CH2CH3_DATALOG     1080                                        //Swap Ch2 And Ch3 Data Pos In The Log
#define ATC_READ_TCTJ_WATER_VALVE   1081                                        //Read ATC TC/TJ Water valve value
#define ATC_READ_TJ_PARAMETER       1082                                        //Read TJ slope and offset
#define ATC_READ_HEATING            1083                                        //Read ATC Heating
#define ATC_GET_SLOPE_OFFSET        1084                                        //Get the Slope/Offset value            //Eliot 20210412
#define ATC_SET_SLOPE_OFFSET        1085                                        //Set the Slope/Offset value            //Eliot 20210412
#define ATC_FEEDBACK_VOLTAGE        1086                                        //Get the Tj Voltage                    //Eliot 20210412
#define ATC_RECORD_TJ_TEMP          1087                                        //Record Tj Temp(Max/Min/Avg)           //Eliot 20220411
#define ATC_QUERY_TJ_TEMP           1088                                        //Query Tj Temp(Max/Min/Avg)            //Eliot 20220411
#define ATC_51_SET_REF_ADJUST       1089
#define ATC_51_AUTOLOAD_RECIPE      1090
#define HANDLER_SLK_LAYOUT          1091                                        //Excel: Handler send the SLK layout  //Ztex 2023.04.19
#define ATC_GET_HEATER_POWER        1092
#define ATC_KL_SV                   1093
#define ATC_KL_OFFSET               1094
#define ATC_KL_TRIGGER              1095
#define ATC_KL_TRIGGER_STATUS       1096
#define ATC_READ_TEMP_3             1097
#define ATC_LOAD_RECIPE_SINGLE      1098                                        //Eliot 20230809
#define ATC_GETDEWPOINTTEMP         1099                                        //Ztex 2023.04.19 Add HT-1032 TriTemp Function
#define ATC_51_SET_TJ_WATCHDOG      1100                                        //             //2023-11-07    Dell
#define ATC_AIRMACHINE_TEMP         1101
#define ATC_SAVE_RECIPE_PROHIBIT    1102
#define ATC_READ_CHILLER_TEMP       1103
#define ATC_SET_PF_PARAMETER        1104                                        //Hmy 20240207 Add Send Pf Par to Atc
#define ATC_SET_T2OFS               1105
#define ATC_SET_SINGLE_T2OFS        1106
#define ATC_SET_HEAT_OUTPUT_LIMIT   1107
#define ATC_GET_CTRL_MODE           1108
#define ATC_READ_HYGON_STATUS       1109
#define ATC_READ_WATER_FLOW         1110
#define ATC_SET_CoolingValue        1111
#define ASIF_TJ_EFUSED              1112                                        //Steven 20240903 : for MTK ASIF data
#define ASIF_TJ_REQUEST             1113
#define ASIF_TJ_FB                  1114
#define ASIF_TJ_EOT                 1115
#define ASIF_TJ_CPU                 1116
#define ASIF_TJ_SOC                 1117
#define ASIF_TJ_GPU                 1118
#define ASIF_TJ_FPC                 1119
#define HANDLER_2DID                1120
#define ATC_SET_WValve_ENABLED      1121                                        //Set Water Valve Enabled               //2024/10/08 Cheng
#define ATC_Recipe_By_Channel       1122                                        //ATC Recipe by Channel parameters      //2024/10/25 Eliot
#define ATC_Recipe_By_Public        1123                                        //ATC Recipe by public parameters       //2024/10/25 Eliot
#define ATC_SET_CHILLER_ENABLED     1124                                        //Set Chiller Enabled                   //2025/01/16 Cheng

#define ATC_SET_TC_WATER_VALVE      1125
#define HANDLER_ALARM               1126
#define ATC_SET_DYNAMIC_PID         1127
#define ATC_READ_DYNAMIC_PID        1128

#define ATC_SET_MULTI_TC_OFFSET     1129                                        //Ifor 20251203 add: ATC MultiSensor
#define ATC_CHILLER_WATERWARNING    1130                                        //ATC CHILLER WATERWARNING              //2025/09/02 Victor
#define ATC_Multi_Temperature_Control 1131                                      //Ifor 20251203 add: ATC MultiSensor
#define ATC_Handler_Transmit_Recipe 1132                                        //Transmit recipe file                  //20251021 Evan
#define ATC_DEFROSTING              1133                                        //ATC DEFROSTING                        //20251217 Victor
#define ATC_Star_Transmit_Recipe    1134                                        //Handler requests to send RECIPE       //20260122 Evan
#define ATC_SET_SINGLE_OFFSET_MTP   1135                                        //SET SINGLE OFFSET MTP                 //20260226 Victor
#define ATC_51_ATO_AdjustValve      1136                                        //Auto K-Temp adjust valve              //20260211 Allen
#define ATC_HulkMode                1137                                        //On/Off HulkMode for TSMC              //20260402
//#define ATC_MAX_COMMAND  150   <--超過1150的話要記得去改

//---------------------------------------------------------------------------
//ATC 3.1 Chiller Status <<CER=XXXX>>
#define CHILLER_RUN             "0021"                                          //Chiller Run
#define CHILLER_STOP            "0020"                                          //Chiller Not Run
#define CHILLER_WATER_LEVEL_LOW "0001"                                          //Water Level Over Low
#define CHILLER_WATER_TEMP_HIGH "0004"                                          //Water Temperature Over High
#define CHILLER_WATER_TEMP_LOW  "0008"                                          //Water Temperature Over Low
#define CHILLER_PRESSURE_RISE   "0080"                                          //Circulating fluid discharge pressure rise (水壓過高).
#define CHILLER_PRESSURE_DROP   "0100"                                          //Circulating fluid discharge pressure drop (水壓過低).
//---------------------------------------------------------------------------
//ATC 3.1 Temp. Reading Mode
#define TEMP_FROM_TC        0
#define TEMP_FROM_TJ        1
#define TEMP_FROM_AD_TC     2
#define TEMP_FROM_AD_TJ     3
#define TEMP_FROM_INT_TC    4
TATC_Data ATC_Data;                                                             //kevin 20200409 add ATX DATA read
bool bATCBusy=false;                                                            //Ztex 2023.04.19 Add HT-1032 TriTemp Function
int TriTemp_Ch[ATC_MAX_SITE] = {tcAa1, tcAb1, tcAc1, tcAd1, tcBa1, tcBb1, tcBc1, tcBd1,
                                tcAe1, tcAf1, tcAg1, tcAh1, tcBe1, tcBf1, tcBg1, tcBh1,
                                tcAa2, tcAb2, tcAc2, tcAd2, tcBa2, tcBb2, tcBc2, tcBd2,
                                tcAe2, tcAf2, tcAg2, tcAh2, tcBe2, tcBf2, tcBg2, tcBh2,
                                tcShuttle1, tcShuttle2, tcHotPlate1, tcHotPlate2,
                                tcShuttle3, tcShuttle4, tcHotPlate3, tcHotPlate4
                               };
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetCommandString()
{
    asCommString[ATC_RECIPE_FILE            -1000]="ATC Change Recipe File(1001)";
    asCommString[ATC_SET_TEMP               -1000]="Set ATC Run Temperature(1002)";
    asCommString[ATC_SET_TOFS               -1000]="Set Temp. Offset(1003)";
    asCommString[ATC_SITE_ENABLED           -1000]="Set Use Site(1004)";
    asCommString[ATC_SET_TRM                -1000]="Set Temp. Reading Mode(1005)";              //not Ready
    asCommString[ATC_SET_MAX_TEMP           -1000]="Set ATC MAX Working Temperature(1006)";
    asCommString[ATC_RUN_STOP               -1000]="ATC Run/Stop(1007)";
    asCommString[ATC_INITIAL_SET            -1000]="ATC Initial all Setting(1008)";
    asCommString[ATC_MODE_TYPE              -1000]="ATC Mode Type(1009)";
    asCommString[ATC_READ_TEMP              -1000]="Read ATC Temperature(1010)";
    asCommString[ATC_HANDLER_ARM            -1000]="Inform Handler Use Arm(1011)";
    asCommString[ATC_TEST_START             -1000]="Inform Handler Testing or Test finish(1012)";
    asCommString[ATC_SEND_TJ                -1000]="Send TJ Temperature to ATC(1013)";          //not Ready
    asCommString[ATC_ERROR                  -1000]="ATC Error Message(1014)";
    asCommString[ATC_SET_SINGLE_TEMP        -1000]="Set Single Site Temperature(1015)";
    asCommString[ATC_SET_SINGLE_OFFSET      -1000]="Set Single Site Offset(1016)";
    asCommString[ATC_CANCEL_MESSAGE         -1000]="Inform ATC Close Alarm Form(1017)";
    asCommString[ATC_CHILLER_STATUS         -1000]="Query Chiller ststus(1018)";                //not Ready
    asCommString[ATC_SET_PID                -1000]="Set ATC PID(1019)";
    asCommString[ATC_READ_PID               -1000]="Read ATC PID(1020)";                        //not Ready
    asCommString[ATC_USE_TSD                -1000]="Use TSD Function(1021)";
    asCommString[ATC_EMG_UP_DOWN            -1000]="Handler EMG Up or Down(1022)";
    asCommString[ATC_READ_TEMP_2            -1000]="Read ATC Second Temperature(1023)";
    asCommString[ATC_READ_SETTEMP           -1000]="Read ATC Set Temp(1024)";
    asCommString[ATC_STATE                  -1000]="Read ATC State(1025)";
    asCommString[ATC_RUN_MODULE             -1000]="Run ATC Module(1026)";
    asCommString[ATC_STOP_MODULE            -1000]="Stop ATC Module(1027)";
    asCommString[ATC_SET_2ND_RANGE_P        -1000]="Set 2nd temperature Positive range(1028)";
    asCommString[ATC_SET_2ND_RANGE_N        -1000]="Set 2nd temperature Negative range(1029)";
    asCommString[ATC_SET_2ND_TIME           -1000]="Set 2nd temperature Interval time(1030)";
    asCommString[ATC_SW_VER                 -1000]="ATC Software Version(1031)";
    asCommString[ATC_FW_VER                 -1000]="ATC Firmware Version(1032)";
    asCommString[ATC_TIME_SYNC              -1000]="Time synchronization to Handler(1033)";
    asCommString[ATC_SET_2ND_ENABLED        -1000]="Set 2nd temperature Function enabled(1034)";
    asCommString[ATC_READ_OFFSET            -1000]="Read ATC Set Offset(1035)";
    asCommString[ATC_GET_NOW_RECIPE         -1000]="Read Now Recipe File(1036)";
    asCommString[ATC_GET_RECIPE_LIST        -1000]="Get Recipe File List(1037)";
    asCommString[ATC_USE_TJ_MODE            -1000]="ATC Use TJ Mode(1038)";
    asCommString[ATC_SET_CHILLER_TEMP       -1000]="ATC Set Chiller Temp(1039)";
    asCommString[ATC_SEND_TEMP_READY        -1000]="ATC Send Temp Ready(1040)";         //Steven 20160604 : by site TSD
    asCommString[ATC_SITE_2ND_CHECK         -1000]="ATC_SITE_2ND_CHECK(1041)";
    asCommString[ATC_SET_TJ_OFFSET          -1000]="ATC_SET_TJ_OFFSET(1042)";
    asCommString[ATC_SET_SINGLE_TJ_OFFSET   -1000]="ATC_SET_SINGLE_TJ_OFFSET(1043)";
    asCommString[ATC_RUN_SELFTEST           -1000]="ATC_RUN_SELFTEST(1044)";            //2016/7/17 Dell    Add
    asCommString[ATC_GET_2ND_STATUS         -1000]="Get_Second_Sensor_Function(1045)";  //Eliot 2016_0802
    asCommString[ATC_MANUAL_SELFTEST        -1000]="Manual ATC selt-test functin(1046)";//Eliot 2016_0816
    asCommString[ATC_SELFTEST_STATUS        -1000]="Get self-test status(1047)";        //Eliot 2016_0816
    asCommString[HANDLER_STATUS             -1000]="Check Handler Status(1048)";        //Eliot 2016_0816
    asCommString[ATC_AIRMACHINE_STATUS      -1000]="Query AirMachine ststus(1049)";     //Hmy 20161129 add air Machine//Ztex 2023.04.19 Add HT-1032 TriTemp Function
    asCommString[ATC_LOT_START              -1000]="ATC_LOT_START(1050)";               //Lot Start, Lot ID                     //Eliot 2016_1020
    asCommString[ATC_LOT_END                -1000]="ATC_LOT_END(1051)";                 //Lot End                               //Eliot 2016_1020
    asCommString[ATC_SELFTEST_RESULT        -1000]="ATC_SELFTEST_RESULT(1052)";         //Query Self-Test Result                //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
    asCommString[HANDLER_ABNORMAL_SITE      -1000]="HANDLER_ABNORMAL_SITE(1053)";       //Handler send abnormal site to ATC     //Eliot 20170116 Handler傳送有異常的Site給ATC
    asCommString[ATC_GET_SN                 -1000]="ATC_GET_SN(1054)";                  //Get ATC power supply serial number    //20170202W05   Dell    Add Get ATC power supply serial number
    asCommString[ATC_SHUTDOWN               -1000]="ATC_SHUTDOWN(1055)";                //ATC SHUTDOWN                          //2017-08-03    Dell for 1028c
    asCommString[ATC_51_FREONRECOVER        -1000]="ATC51 FREONRE COVER(1056)";         //Hmy 20170919 Copy Dell     //2017-09-14
    asCommString[HANDLER_CAN_SELFTEST       -1000]="HANDLER_CAN_SELFTEST(1057)";        //ATC Query Handler can self-test?          //Eliot 20171211
    asCommString[ATC_51_SET_REF_ENABLED     -1000]="ATC51 SET REF ENABLED(1058)";       //Hmy20180730 Add ATC Refrigerator Status  //2018-06-06    Dell for HT3012CT                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    asCommString[ATC_51_GET_REF_STATUS      -1000]="ATC51 GET REF STATUS(1059)";        //Hmy20180730 Add ATC Refrigerator Status  //2018-06-06    Dell for HT3012CT   Dell for 勝麗 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    asCommString[ATC_51_SET_DEFROST         -1000]="ATC_51_SET_DEFROST(1060)";          //Setting ATC defrost.                  //2018-07-16    Dell for ASE_CL
    asCommString[ATC_60_SET_AIRVALVE        -1000]="Setting open/close air valve(1061)";//Steven 20181102 : Control ATC6.0氣流開關
    asCommString[ATC_READ_SOCKETTEMP        -1000]="Read ATC socket temperature(1062)";
    asCommString[HANDLER_FFC_SETTING        -1000]="HANDLER FFC SETTING(1063)";         //Ifor 20190730 : add FFC Temperature Offset
    asCommString[HANDLER_ATO_RECORD         -1000]="Record Auto Temperature Offset data(1064)";         //Ifor 20190730 : add FFC Temperature Offset
    asCommString[HANDLER_FFC_ENABLED        -1000]="HANDLER FFC ENABLED(1065)";         //Ifor 20190730 : add FFC Temperature Offset
    asCommString[HANDLER_FFC_TRIGGER        -1000]="HANDLER FFC TRIGGER(1066)";         //Ifor 20190730 : add FFC Temperature Offset
    asCommString[ATC_GET_CONTROL_MODE       -1000]="ATC_GET_CONTROL_MODE(1067)";        //Controller Control Mode Status        //Eliot 20190619
    asCommString[HANDLER_PFC_ENABLED        -1000]="Power Following Control ENABLED(1068)"; //Eliot 20190916
    asCommString[ATC_GET_PFC_PARAMETER      -1000]="ATC_GET_PFC_PARAMETER(1069)";       //讀取單一溫度的PF參數值
    asCommString[ATC_SET_PFC_PARAMETER      -1000]="ATC_SET_PFC_PARAMETER(1070)";       //設定單一溫度的PF參數值
    asCommString[ATC_READ_WATER_VALVE       -1000]="ATC_READ_WATER_VALVE(1071)";        //kevin 20200409
    asCommString[ATC_CMD_READ_FUNCTION_STATUS-1000]="ATC_CMD_READ_FUNCTION_STATUS(1072)"; //kevin 20200409
    asCommString[ATC_GET_HEAT_OUTPUT        -1000]="ATC_GET_HEAT_OUTPUT(1073)";         //Read ATC Heat output value            //2020-07-16    Dell
    asCommString[ATC_AUTO_TCTS_ENABLED      -1000]="ATC_AUTO_TCTS_ENABLED(1074)";       //Auto Tc/Ts switch function //Eliot 20220411
    asCommString[ATC_GETDEWPOINTTEMP        -1000]="ATC_GETDEWPOINTTEMP(1099)";         //Ztex 2023.04.19 Add HT-1032 TriTemp Function

    asCommString[HANDLER_TEST_SITEMAPPING   -1000]="HANDLER_TEST_SITEMAPPING(1075)";    //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Handler send site mapping             //2021-07-20    Dell
    asCommString[ATC_SET_TJ_ENABLED         -1000]="ATC_SET_TJ_ENABLED(1076)";          //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ Function enabled               //2021-07-20    Dell
    asCommString[ATC_SET_TJ_PARAMETER       -1000]="ATC_SET_TJ_PARAMETER(1077)";        //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ slope and offset               //2021-07-20    Dell
    asCommString[ATC_51_SET_ExtraHeatGun    -1000]="ATC_51_SET_ExtraHeatGun(1078)";
    asCommString[ATC_51_READ_ExtraHeatGun   -1000]="ATC_51_READ_ExtraHeatGun(1079)";
    asCommString[ATC_SWAP_CH2CH3_DATALOG    -1000]="ATC_SWAP_CH2CH3_DATALOG(1080)";     //Swap Ch2 And Ch3 Data Pos In The Log
    asCommString[ATC_READ_TCTJ_WATER_VALVE  -1000]="ATC_READ_TCTJ_WATER_VALVE(1081)";   //Read ATC TC/TJ Water valve value
    asCommString[ATC_READ_TJ_PARAMETER      -1000]="ATC_READ_TJ_PARAMETER(1082)";       //Read TJ slope and offset
    asCommString[ATC_READ_HEATING           -1000]="ATC_READ_HEATING(1083)";            //Read ATC Heating
    asCommString[ATC_GET_SLOPE_OFFSET       -1000]="ATC_GET_SLOPE_OFFSET(1084)";        //Eliot 20210412
    asCommString[ATC_SET_SLOPE_OFFSET       -1000]="ATC_SET_SLOPE_OFFSET(1085)";        //Eliot 20210412
    asCommString[ATC_FEEDBACK_VOLTAGE       -1000]="ATC_FEEDBACK_VOLTAGE(1086)";        //Eliot 20210412
    asCommString[ATC_RECORD_TJ_TEMP         -1000]="ATC_RECORD_TJ_TEMP(1087)";          //Record Tj Temp(Max/Min/Avg)//Eliot 20220411
    asCommString[ATC_QUERY_TJ_TEMP          -1000]="ATC_QUERY_TJ_TEMP(1088)";           //Query Tj Temp(Max/Min/Avg) //Eliot 20220411
    asCommString[ATC_51_SET_REF_ADJUST      -1000]="Read ATC5.1 set refrigerator valve(1089)";
    asCommString[ATC_51_AUTOLOAD_RECIPE     -1000]="ATC5.1 Autoload golden recipe by Temp(1090)";
    asCommString[HANDLER_SLK_LAYOUT         -1000]="HANDLER_SLK_LAYOUT(1091)";           //Excel: Handler send the SLK layout
    asCommString[ATC_GET_HEATER_POWER       -1000]="Get the Heater Output Power(1092)";
    asCommString[ATC_KL_SV                  -1000]="KL set temp to Handler(1093)";
    asCommString[ATC_KL_OFFSET              -1000]="KL set offset to Handler(1094)";
    asCommString[ATC_KL_TRIGGER             -1000]="KL set start KL control by Handler(1095)";
    asCommString[ATC_KL_TRIGGER_STATUS      -1000]="KL set start Handler read status(1096)";
    asCommString[ATC_READ_TEMP_3            -1000]="Read ATC Multi Temperature(1097)";
    asCommString[ATC_LOAD_RECIPE_SINGLE     -1000]="ATC_LOAD_RECIPE_SINGLE(1098)";      //Eliot 20230809
    asCommString[ATC_GETDEWPOINTTEMP        -1000]="ATC_GETDEWPOINTTEMP(1099)";         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    asCommString[ATC_51_SET_TJ_WATCHDOG     -1000]="ATC_51_SET_TJ_WATCHDOG(1100)";
    asCommString[ATC_AIRMACHINE_TEMP        -1000]="Read Air Machine Temperature(1101)";
    asCommString[ATC_SAVE_RECIPE_PROHIBIT   -1000]="Saving recipe data is prohibited(1102)";
    asCommString[ATC_READ_CHILLER_TEMP      -1000]="Read chiller temperature(1103)";
    asCommString[ATC_SET_PF_PARAMETER       -1000]="Set PF slope and offset (1104)";
    asCommString[ATC_SET_T2OFS              -1000]="Set Tc2 Offset(1105)";
    asCommString[ATC_SET_SINGLE_T2OFS       -1000]="Set Single Site Tc2 Offset(1106)";
    asCommString[ATC_SET_HEAT_OUTPUT_LIMIT  -1000]="Set Heat Output Limit(1107)";
    asCommString[ATC_GET_CTRL_MODE          -1000]="Get ATC Controller Control Mode(1108)";
    asCommString[ATC_READ_HYGON_STATUS      -1000]="READ HYGON_STATUS(1109)";
    asCommString[ATC_READ_WATER_FLOW        -1000]="Read water flow value(1110)";
    asCommString[ATC_SET_CoolingValue       -1000]="SET Cooling Value(1111)";
    asCommString[ASIF_TJ_EFUSED             -1000]="ASIF_TJ_EFUSED(1112)";              //Steven 20240903 : for MTK ASIF data
    asCommString[ASIF_TJ_REQUEST            -1000]="ASIF_TJ_REQUEST(1113)";
    asCommString[ASIF_TJ_FB                 -1000]="ASIF_TJ_FB(1114)";
    asCommString[ASIF_TJ_EOT                -1000]="ASIF_TJ_EOT(1115)";
    asCommString[ASIF_TJ_CPU                -1000]="ASIF_TJ_CPU(1116)";
    asCommString[ASIF_TJ_SOC                -1000]="ASIF_TJ_SOC(1117)";
    asCommString[ASIF_TJ_GPU                -1000]="ASIF_TJ_GPU(1118)";
    asCommString[ASIF_TJ_FPC                -1000]="ASIF_TJ_FPC(1119)";

    asCommString[HANDLER_2DID               -1000]="HANDLER_2DID(1120)";
    asCommString[ATC_Recipe_By_Channel      -1000]  ="ATC_Recipe_By_Channel(1122)";     //Ifor 20241119 add: Handler 可以取得ATC各Channel的PID/Water Valve….等參數.
    asCommString[ATC_Recipe_By_Public       -1000]  ="ATC_Recipe_By_Public(1123)";      //Ifor 20241119 add: Handler 可以取得ATC共用參數的設定值(ATC Recipe Function-2 page).
    asCommString[ATC_SET_TC_WATER_VALVE     -1000]="ATC_SET_TC_WATER_VALVE(1125)";
    asCommString[HANDLER_ALARM              -1000]="Handler To ATC Alarm Message(1126)";
    asCommString[ATC_SET_DYNAMIC_PID        -1000]="Set ATC Dynamic PID(1127)";
    asCommString[ATC_READ_DYNAMIC_PID       -1000]="ATC_READ_DYNAMIC_PID(1128)";
    asCommString[ATC_SET_MULTI_TC_OFFSET    -1000]="ATC_SET_MULTI_TC_OFFSET(1129)";     //Ifor 20251203 add: ATC MultiSensor

    asCommString[ATC_Multi_Temperature_Control -1000]="ATC_Multi_Temperature_Control(1131)"; //Ifor 20251203 add: ATC MultiSensor
    asCommString[ATC_Handler_Transmit_Recipe-1000]  ="ATC_Handler_Transmit_Recipe(1132)";     //Ifor 20251229 add: ATC Rrcipe 傳送與接收
    asCommString[ATC_Star_Transmit_Recipe   -1000]  ="ATC_Star_Transmit_Recipe(1134)";//Ifor 20251229 add: ATC Rrcipe 傳送與接收
}
//---------------------------------------------------------------------------
__fastcall TATC_InterfaceForm::TATC_InterfaceForm(TComponent* Owner)
    : TForm(Owner)
{
    IsConnectFlag=false;
    bConnecting=false;
    bOpenFlag=false;                                                            //Eliot 2015_1008
    dCommLogFileSize=512*1024;                                                  //Eliot 2015_1008  size:521Kb
    asData="";
    asDataTemp="";
    asATC_SW_Ver="";                                                            //Eliot 2015_1007
    asHandlerVer="unknow";                                                      //Eliot 2015_1126
    iATC_MODE_TYPE=ATC_TYPE_UNSET;
    A31_Temp_Reading_Mode->ItemIndex=0;
    Temp_Panel->Top =8;
    Temp_Panel->Left=8;
    Temp_Panel->Width=673;
    Temp_Panel->Height=201;
    ReadTempInterval=1000;
    iHandlerSiteCount=4;
    iATC_ModuleCount=1;
    iATCTempIndex=0;                                                            //Eliot 2016_0802
    iATCTempIndexCount=0;                                                       //Eliot 2016_0802
    bATC2ndFunctionFlag=false;                                                  //Eliot 2016_0802
    //AlarmMessage.clear();                                                     //Eliot 2016_0722
    bGetATC_ModuleStateFlag=false;                                              //Eliot 2015_1022
    bHandlerOnlineFlag=false;                                                   //Eliot 2015_1027
    bGetNowRecipeFlag=false;                                                    //Eliot 2015_1126
    bGetRecipeListFlag=false;                                                   //Eliot 2015_1126
    bGetATC_SW_VerFlag=false;                                                   //Eliot 2016_0503
    iATC_DataCount=5000;                                                        //kevin 20160127    //kevin 20151127
    flagATCSelfTestResult=0;                                                    //2016-07-17    Dell    Add

    for(int i=0; i<iATC_DataCount; i++)                                         //kevin 20160127
    {
        ATC_WorkFileList[i]="";
    }

    for(int i=0; i<8; i++)
    {
        asATC_ErrorMsg[i]="";
    }

    for(int i=0; i<ATC_MAX_COMMAND; i++)
    {
        iCommCount[i]=0;                                                        //Eliot 2015_1027
        bCommFlag[i]=true;                                                      //Eliot 2015_1027
        asCommString[i]="";                                                     //Eliot 2015_1027
    }

    csReceiveString = new TCriticalSection();
    slReveiveStringList = new TStringList;
    sDataList = new TStringList();
    sAlarmMessageList = new TStringList();                                      //Eliot 2016_0722
    sAlarmMessageList->Clear();                                                 //Eliot 2016_0722

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        ATC_Channel[i] = new TCheckBox(this);
        ATC_Channel[i]->Parent = ChannelGroupBox;
        ATC_Channel[i]->Visible=false;
        ATC_Channel[i]->Width=65;

        ChPanel[i] = new TPanel(this);
        ChPanel[i]->Parent = Temp_Panel;
        ChPanel[i]->Visible=false;
        ChPanel[i]->Width=41;
        ChPanel[i]->Height=25;

        ChTempTC[i] = new TPanel(this);
        ChTempTC[i]->Parent = Temp_Panel;
        ChTempTC[i]->Visible=false;
        ChTempTC[i]->Width=41;
        ChTempTC[i]->Height=25;

        ChTempTC2[i] = new TPanel(this);
        ChTempTC2[i]->Parent = Temp_Panel;
        ChTempTC2[i]->Visible=false;
        ChTempTC2[i]->Width=41;
        ChTempTC2[i]->Height=25;

        ChTempTJ[i] = new TPanel(this);
        ChTempTJ[i]->Parent = Temp_Panel;
        ChTempTJ[i]->Visible=false;
        ChTempTJ[i]->Width=41;
        ChTempTJ[i]->Height=25;

        P_Value[i]=1000;                                                        //Eliot 2015_0914
        I_Value[i]=10;                                                          //Eliot 2015_0914
        D_Value[i]=100;                                                         //Eliot 2015_0914

        ATC_Module[i]=false;                                                    //Eliot 2015_0930
        asATC_FW_Ver[i]="";                                                     //Eliot 2015_1007
        bATC_ModuleState[i]=false;                                              //Eliot 2015_1022
        asControllerSN[i]="";                                                   //Eliot 20170221 Get ATC power supply serial number
    }
    bGetControllerSN_Flag=false;                                                //Eliot 20170221 Get ATC power supply serial number
    bGetControllerFW_Flag=false;                                                //Ifor 20170230 Get ATC power supply firmware number

    PageControl->Visible = false;
    TabSheet20->TabVisible=false;
    TabSheet21->TabVisible=false;
    TabSheet30->TabVisible=false;
    TabSheet31->TabVisible=false;
    TabSheet50->TabVisible=false;
    TabSheet51->TabVisible=false;
    TabSheet60->TabVisible=false;
    TabSheet70->TabVisible=false;

    ATC_Data.dChillerSetTemp=-999.0;

    LastMsg="";                                                                 //ChungHung 20150826 add fix for continue error message
    SetCommandString();                                                         //Eliot 2015_1027

    for(int i=0;i<iATC_Refrigerator_Num+1;i++)                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
    {
        iATC_RefrigeratorUserMode[0][i] = -1;
        iATC_RefrigeratorUserMode[1][i] = 1;
    }

    for(int i=0;i<iATC_Refrigerator_Num;i++)
    {
        for(int j=0;j<9;j++)
        {
            dATC_RefrigeratorAllStatus[i][j]=-9999.0;
        }
    }
    dATC_RefrigerantMachineHighLimit    = -9999.0;
    dATC_RefrigerantMachineLowLimit     = -9999.0;
    bReadRefrigerantMode_AllStatus      = false;
    bReadRefrigerantMode_Send           = false;
    bReadRefrigerantMode_Recv           = false;                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FormShow(TObject *Sender)
{
    Left=100;                                                                   //Steven 20220407 : 避免跑到畫面外
    Top =10;
    #ifdef SOFT_SIMULTE
        InitialATCSet();                                                        //Initial ATC Set
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::InitialATCSet()
{
    SetChannelCount(4);                                                         //設定目前Channel Count
    SetReadTempInterval(1000);                                                  //自動讀取ATC溫度間格時間(ms)
    SetAddressPort("127.0.0.1", 1234);                                          //設定IP Address and port
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FormDestroy(TObject *Sender)
{
    try
    {
        if(COMM_Log!=NULL && bOpenFlag==true)
            fclose(COMM_Log);                                                   //Eliot 2015_1008

        if(csReceiveString!=NULL)
            delete csReceiveString;

        if(slReveiveStringList!=NULL)                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        {
            slReveiveStringList->Clear();
            delete slReveiveStringList;
        }

        if(sDataList!=NULL)
        {
            sDataList->Clear();
            delete sDataList;
        }

        if(sAlarmMessageList != NULL)
        {
            sAlarmMessageList->Clear();
            delete sAlarmMessageList;
        }

        for(int i=0; i<ATC_MAX_SITE; i++)
        {
            if(ATC_Channel[i]!=NULL)
                delete ATC_Channel[i];
            if(ChPanel[i]!=NULL)
                delete ChPanel[i];
            if(ChTempTC[i]!=NULL)
                delete ChTempTC[i];
            if(ChTempTC2[i]!=NULL)
                delete ChTempTC2[i];
            if(ChTempTJ[i]!=NULL)
                delete ChTempTJ[i];
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TATC_InterfaceForm::FormDestroy");
    }
    LogSoftwareOffTime("TATC_InterfaceForm, FormDestroy");                      //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    IsConnectFlag               =true;
    bConnecting                 =false;
    bHandlerOnlineFlag          =true;                                          //Eliot 2016_0218
    ATC_InterfaceForm->Caption  ="ON-Line";                                     //Eliot 2015_1130

    for(int i=0; i<ATC_MAX_COMMAND; i++)
    {
        iCommCount[i]=0;                                                        //Eliot 2015_1027
        bCommFlag[i]=true;                                                      //Eliot 2015_1027
    }
    bGetATC_SW_VerFlag=false;                                                   //Eliot 2016_0503
    SendCommand(ATC_SW_VER);                                                    //Eliot 2015_1126 ClientSocketConnect
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::PushBackMessage(AnsiString aMsg)            //ChungHung 20150826 add fix for continue error message
{
    for(int i=0; i<sAlarmMessageList->Count; i++)                               //Elito 2016_0722
    {
        if(LastMsg==aMsg || sAlarmMessageList->Strings[i]==aMsg)
            return;
    }
    sAlarmMessageList->Add(aMsg);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    IsConnectFlag               =false;
    bConnecting                 =false;
    bHandlerOnlineFlag          =false;                                         //Eliot 2016_0218
    ATC_ConnectBtn->Down        =false;
    ATC_ConnectBtn->Caption     ="Connect";
    ATC_ConnectBtn->Font->Color =clGreen;

    if(ClientSocket->Active!=false)                                             //ChungHung 20150125 fix Disconnect
    {
        sAlarmMessageList->Add("Disconnect...");                                //Eliot 2016_0722
    }
    bATCRun=false;                                                              //Eliot 2015_1019
    ATC_InterfaceForm->Caption="OFF-Line";                                      //Eliot 2015_1130
    for(int i=0; i<ATC_MAX_SITE; i++)                                           //Eliot 2015_1019
    {
        dTC[i]=9999;
        dTC2[i]=9999;
        dTJ[i]=9999;
    }

    for(int i=0; i<ATC_MAX_COMMAND; i++)                                        //Ifor 20160722 斷線時必須清除 Count 避免一連線就發出Alarm 訊息
    {
        iCommCount[i]=0;
        bCommFlag[i]=true;
    }
    bReSendATCLotEven=true;                                                     //Ifor 20161121 ATC 斷線需重新送出 ATC Lot Even 給ATC
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ClientSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int iReceiveLength;
    AnsiString asData="";

    iReceiveLength = Socket->ReceiveLength();
    if(iReceiveLength<=0)
        return;

    asData=Socket->ReceiveText();

    csReceiveString->Acquire();
    try
    {
        slReveiveStringList->Add(asData);
    }
    __finally
    {
        csReceiveString->Release();
    }

    if(A31_Memo->Lines->Count>500)                                              //Ifor 20170522 (wei) ATC Log Count 50 =>500
    {
        SaveCommLog(!cBox_RecordComm->Checked, A31_Memo);                       //Eliot 2015_1008
    }
    A31_Memo->Lines->Insert(0, AnsiString(Now().FormatString("yyyy/m/d hh:nn:ss"))+" [<-Read] "+asData);
    ProcessReceiveString_ATC();                                                 //kevin 20160128  Eliot???
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SendCommand(int iCommIndex)
{
    int i=0;    //Eastsun 20260522 整合
    AnsiString asValue, asFileBuffer;    //Eastsun 20260522 整合: ATC FileTransfer
    AnsiString asData="", asTemp="";

    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    switch (iCommIndex)
    {
        case ATC_SET_TRM:                                                       //1005 Temp. Reading Mode
        case ATC_SET_MAX_TEMP:                                                  //1006 Set ATC MAX Working Temperature
        case ATC_RUN_STOP:                                                      //1007 ATC Run/Stop
        case ATC_INITIAL_SET:                                                   //1008 ATC Initial all Setting
        case ATC_MODE_TYPE:                                                     //1009 ATC Mode Type
        case ATC_READ_TEMP:                                                     //1010 Read ATC Temperature
        case ATC_HANDLER_ARM:                                                   //1011 Inform Handler Use Arm
        case ATC_SEND_TJ:                                                       //1013 Send TJ Temperature to ATC            //Not been used
        case ATC_CHILLER_STATUS:                                                //1018 Query Chiller ststus                  //For ATC 3.0/6.0
        case ATC_READ_PID:                                                      //1020 Read ATC PID
        case ATC_EMG_UP_DOWN:                                                   //1022 Handler EMG Up or Down                //For ATC 7.0
        case ATC_READ_TEMP_2:                                                   //1023 Read ATC Second Temperature
        case ATC_READ_SETTEMP:                                                  //1024 Read ATC Set Temp          //ChungHung 20150826 add
        case ATC_STATE:                                                         //1025 Read ATC State             //ChungHung 20150826 add
        case ATC_RUN_MODULE:                                                    //1026 Run ATC Module                        //For ATC 5.0
        case ATC_STOP_MODULE:                                                   //1027 Stop ATC Module                       //For ATC 5.0
        case ATC_SET_2ND_TIME:                                                  //1030 Setting second point temperature interval time.   //通知ATC設定第二感溫點Alarm 時間
        case ATC_FW_VER:                                                        //1032 ATC Firmware Version                  //Eliot 2015_1007
        case ATC_SET_2ND_ENABLED:                                               //1034 Setting second point temperature Function enabled.//通知ATC設定第二感溫點功能
        case ATC_READ_OFFSET:                                                   //1035 Read ATC Set Offset                   //Eliot 2015_1124
        case ATC_GET_NOW_RECIPE:                                                //1036 Get Now Recipe File                   //Eliot 2015_1126
        case ATC_GET_RECIPE_LIST:                                               //1037 Get Recipe File List                  //Eliot 2015_1126
        case ATC_SET_CHILLER_TEMP:                                              //1039 SET chiller temp     kevin 20160425 add
        case ATC_RUN_SELFTEST:                                                  //1044 Run ATC selt-test functin             //2016/7/17 Dell    Add
        case ATC_GET_2ND_STATUS:                                                //1045 Get second sensor function enabled.   //2016-08-01    Dell    Add
        case ATC_MANUAL_SELFTEST:                                               //1046 Manual ATC selt-test functin          //Eliot 2016_0816
        case ATC_SELFTEST_STATUS:                                               //1047 Get self-test status                  //Eliot 2016_0816
        case HANDLER_STATUS:                                                    //1048 Check Handler Status                  //Eliot 2016_0816
        case ATC_SELFTEST_RESULT:                                               //1052 Query Self-Test Result                //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
        case ATC_GET_SN:                                                        //1054 Get ATC power supply serial number    //20170202W05   Dell    Add Get ATC power supply serial number
        case ATC_51_SET_DEFROST:                                                //1060 Setting ATC defrost.                  //2018-07-16    Dell for ASE_CL
        case ATC_READ_WATER_VALVE:                                              //1071 Read ATC Water valve value            //Eliot 20200406
        case ATC_CMD_READ_FUNCTION_STATUS:                                      //1072
        case ATC_SET_TJ_ENABLED:                                                //1076 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ Function enabled               //2021-07-20    Dell
        case ATC_GET_CONTROL_MODE:                                              //1067
            sATC_COMMAND.sprintf("@%d,1,%d,#", iCommIndex, iSendData[0]);
            break;
        case ATC_RECIPE_FILE:                                                   //1001 ATC Change Recipe File
            sATC_COMMAND.sprintf("@%d,%d,%s,%d#", iCommIndex, 2, asRecipeFile, iSendData[0]);   //Eliot 2016_0324
            break;
        case ATC_SET_TEMP:                                                      //1002 Set ATC Run Temperature
        case ATC_SET_TOFS:                                                      //1003 Temp. Offset
        case ATC_SITE_ENABLED:                                                  //1004 Set Use Site
        case ATC_TEST_START:                                                    //1012 Inform Handler Testing or Test finish
        case ATC_USE_TSD:                                                       //1021 Use TSD Function                      //For ATC 7.0
        case ATC_SET_2ND_RANGE_P:                                               //1028 Setting second point temperature Positive range.  //通知ATC設定第二感溫點Alarm 範圍正極限
        case ATC_SET_2ND_RANGE_N:                                               //1029 Setting second point temperature Negative range.  //通知ATC設定第二感溫點Alarm 範圍負極限
        case ATC_USE_TJ_MODE:                                                   //1038 SET TJ MODE for AMD   kevin 20160425 add
        case ATC_SEND_TEMP_READY:                                               //1040 send temperature ready                //2016-04-27    Dell    Add
        case ATC_SITE_2ND_CHECK:                                                //1041 Site Check Second Temperature         //Eliot 2016_0503
        case ATC_SET_TJ_OFFSET:                                                 //1042
        case ATC_SET_SINGLE_TJ_OFFSET:                                          //1043
        case HANDLER_ABNORMAL_SITE:                                             //1053 Handler send abnormal site to ATC     //Eliot 20170116 Handler傳送有異常的Site給ATC
        case ATC_60_SET_AIRVALVE:                                               //1061 Setting open/close air valve          //2018-10-31    Dell for Qualcomm //Steven 20181102 : Control ATC6.0氣流開關
        case HANDLER_TEST_SITEMAPPING:                                          //1075 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Handler send site mapping //JerryYang 20231128 : 傳送ATC site map
        case ATC_SET_T2OFS:                                                     //KenHsieh 20240311 : add Tc2 Offse
        case ATC_SET_DYNAMIC_PID:
        case ATC_Multi_Temperature_Control:                                     //Ifor 20251203 add: ATC MultiSensor
            for(int i=0; i<iDataCount; i++)
            {
                asData+=AnsiString(iSendData[i])+",";
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iDataCount, asData);
            break;
        case ATC_ERROR:                                                         //1014 ATC Error Message
            sATC_COMMAND.sprintf("@%d,1,ATC_ERROR_OK,#", iCommIndex);
            break;
        case ATC_SET_SINGLE_TEMP:                                               //1015 Set Single Site Temperature
        case ATC_SET_SINGLE_OFFSET:                                             //1016 Set Single Site Offset
            sATC_COMMAND.sprintf("@%d,%d,%d,%d,#", iCommIndex, 2, iSendData[0], iSendData[1]);
            break;
        case ATC_CANCEL_MESSAGE:                                                //1017 Inform ATC Close Alarm Form
            sATC_COMMAND.sprintf("@%d,%d#",iCommIndex, 0);
            break;
        case ATC_SET_PID:                                                       //1019 Set ATC PID
            for(int i=0; i<(iDataCount*3); i++)
            {
                asData+=AnsiString(iSendData[i])+",";
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iDataCount*3, asData);
            break;
        case ATC_SW_VER:                                                        //1031 ATC Software Version                 //Eliot 2015_1007
            sATC_COMMAND.sprintf("@%d,1,%s#", iCommIndex, asHandlerVer);
            break;
        case ATC_TIME_SYNC:                                                     //1033 Time synchronization to Handler      //Eliot 2015_1017
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, 1, FormatDateTime("MM/DD/YYYY HH:NN:SS AM/PM", Now()));          //2016-04-26    Dell    fixed for windows time format
            break;
        case ATC_AIRMACHINE_STATUS:                                             //1049 Query Chiller ststus                 //20160922 Zek For ATC 5.1
            if(iATC_MODE_TYPE==61)
                sATC_COMMAND.sprintf("@%d,%d,%d,%d,%d,%d,%d,%d,#", iCommIndex, 6, iSendData[0], iSendData[1], iSendData[2], iSendData[3], iSendData[4], iSendData[5]);
            else
                sATC_COMMAND.sprintf("@%d,%d,%d,%d,%d,#", iCommIndex, 3, bUseAirMachine, iSendData[0], iSendData[1]);       //Ztex 2023.04.19 Add HT-1032 TriTemp Function  //設定/讀取AirStream溫度
            break;
        case ATC_LOT_START:                                                     //1050 Lot Start, Lot ID                    //Eliot 2016_1020
        case ATC_LOT_END:                                                       //1051 Lot End                              //Eliot 2016_1020
            sATC_COMMAND.sprintf("@%d,1,%s#", iCommIndex, asLotID);
        case ATC_51_SET_REF_ENABLED:                                            //Add ATC Refrigerator Status //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
            for(int i=0;i<Total_Compressor;i++)                                     //ATC 收/發壓縮機狀態命令
            {
                asData += AnsiString(iSendData[i])+",";
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#",iCommIndex,Total_Compressor,asData);
            break;
        case ATC_51_GET_REF_STATUS:                                             //Add ATC Refrigerator Status  //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
            if(bReadRefrigerantMode_AllStatus ==false)                          //ATC 收/發壓縮機狀態命令
                sATC_COMMAND.sprintf("@%d,1,%d,#",iCommIndex,iSendData[0]);
            else
                sATC_COMMAND.sprintf("@%d,2,%d,%s#",iCommIndex,iSendData[0],asRefrigeratorStatus);
            bReadRefrigerantMode_Send=true;
            bReadRefrigerantMode_Recv=false;
            break;                                                              //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==
        case HANDLER_FFC_SETTING:                                               //1063 Ifor 20190730 : add FFC Temperature Offset
            for(int i=0; i<(iDataCount); i++)
            {
                if(i==iDataCount-1)
                {
                    asData+=AnsiString(iSendData[i]);
                }
                else
                {
                    asData+=AnsiString(iSendData[i])+",";
                }
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iDataCount, asData);
            break;
        case HANDLER_ATO_RECORD:                                                //1064 Ztex 2023.04.18 Add HT-1032 ATC CMD  //Hmy 20200901 Add Auto K Temp Auto Save//Record Auto Temperature Offset data
            for(int i=0; i<iDataCount; i++)
            {
                asData+=AnsiString(iSendData[i])+",";
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#",iCommIndex, iDataCount, asData);
            break;
        case HANDLER_FFC_ENABLED:                                               //1065 Ifor 20190730 : add FFC Temperature Offset
            asData=AnsiString(iSendData[1]);
            sATC_COMMAND.sprintf("@%d,1,%s#", iCommIndex, asData);
            break;
        case HANDLER_FFC_TRIGGER:                                               //1066 Ifor 20190730 : add FFC Temperature Offset
            for(int i=0; i<16; i++)                                             //16CH
            {
                asData+=AnsiString(iSendData[i])+",";
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, 16, asData);
            break;
        case ATC_SET_TJ_PARAMETER:                                              //1077 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ slope and offset //2021-07-20    Dell
            sATC_COMMAND.sprintf("@%d,2,%s,%s#", iCommIndex, asSendData[0], asSendData[1]);
            break;
        case ATC_51_SET_TJ_WATCHDOG:                                            //JerryYang 20231205 : ATC Tj WatchDog
            sATC_COMMAND.sprintf("@%d,5,%s,%s,%s,%s,%s#", iCommIndex, asSendData[0], asSendData[1], asSendData[2], asSendData[3], asSendData[4]);  //JerryYang 20251201 : ATC連動watchdog新增continue欄位
            break;
        case ATC_SET_PF_PARAMETER:                                              //1104 //Hmy 20240207 Add Send Pf Par to Atc
            sATC_COMMAND.sprintf("@%d,5,%d,%d,%s,%s,%d#",iCommIndex,iSendData[0],iSendData[1],asSendData_PF[0],asSendData_PF[1],iSendData[2]);
//            sATC_COMMAND.sprintf("@%d,4,%d,%d,%s,%s#",iCommIndex,iSendData[0],iSendData[1],asSendData_PF[0],asSendData_PF[1]);
            break;
        case HANDLER_SLK_LAYOUT:                                               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            sATC_COMMAND.sprintf("@%d,%d,%d,%d,%d,#", iCommIndex, 3, iSendData[0], iSendData[1], iSendData[2]);
            break;
        case ATC_GETDEWPOINTTEMP:
            sATC_COMMAND.sprintf("@%d,%d,%d,%d,%d,#", iCommIndex, 3, iSendData[0], iSendData[1], iSendData[2]);
            break;
        case ASIF_TJ_EFUSED:                                                    //1112 //Steven 20240903 : for MTK ASIF data
            sATC_COMMAND.sprintf("@1112,%d,ASIF_TJ_EFUSED", iDataCount);
            for(int i=1; i<iDataCount; i++)
            {
                sATC_COMMAND=sATC_COMMAND+AnsiString(",\"")+asSendData[i]+AnsiString("\"");
            }
            sATC_COMMAND=sATC_COMMAND+AnsiString("#");
            break;
        case ASIF_TJ_REQUEST:                                                   //@1113,1,ASIF_TJ_REQUEST#
            sATC_COMMAND.sprintf("@%d,1,ASIF_TJ_REQUEST#", iCommIndex);
            break;
        case ASIF_TJ_FB:                                                        //@1114,1,ASIF_TJ_FB#
            sATC_COMMAND.sprintf("@%d,1,ASIF_TJ_FB#", iCommIndex);
            break;
        case HANDLER_2DID:                                                        //@1114,1,ASIF_TJ_FB#
            if(iSendData[1]==0 || iSendData[1]==1)                              //Single Site || Dual Site 2DID
            {
                sATC_COMMAND.sprintf("@%d,%d,%d,%s#",iCommIndex,iSendData[0],iSendData[1],asHandler2DID); //[0]:use site(1,2,3) ,[1]:arm
            }
            else                                                                //停止所有Thermo profile記錄並存檔
            {
                sATC_COMMAND.sprintf("@%d,%d,%d#",iCommIndex,1,-1);
            }
            break;
        case ATC_GET_PFC_PARAMETER:                                             //1069 讀取單一溫度的PF參數值    送 @1069,1,80#  回傳 @1069,21,80,100,80,70,62,61,60,59,57,55,55,53,51,49,48,44,61,61,30,20,10#
            sATC_COMMAND.sprintf("@%d,%d,%d#", iCommIndex, 1, 1);
            break;
        case ATC_SET_PFC_PARAMETER:                                             //設定單一溫度的PF參數值    送 @1070,21,80,100,80,70,62,61,60,59,57,55,55,53,51,49,48,44,61,61,30,20,10#   回傳 @1070,1,1#
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, 21, asSetPFCData);
            break;
        case ATC_GET_SLOPE_OFFSET:
        case ATC_FEEDBACK_VOLTAGE:                                              //ATC_FEEDBACK_VOLTAGE //wei 20211206 Get the Tj Voltage //1071 Read ATC Water valve value
        case ATC_READ_DYNAMIC_PID:                                              //1128
            sATC_COMMAND.sprintf("@%d,%d,%d#", iCommIndex, 1, iDataCount);
            break;
        case ATC_SET_SLOPE_OFFSET:
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iATC_Use_Heat_Count*2, asSetPFCData);
            break;
        case ATC_RECORD_TJ_TEMP:
            sATC_COMMAND.sprintf("@%d,2,%d,%d#", iCommIndex, iSendData[0], iRecordTjTemp); //@1087,2,m,n#     //m:-1:none 0:Arm1 1:Arm2 2:Both Arm         n:0(Stop)  1(record)
            break;
        case ATC_QUERY_TJ_TEMP:
            sATC_COMMAND.sprintf("@%d,1,%d#", iCommIndex, iDataCount);          //@1088,1,1#     讀取1組
            break;
        case ATC_KL_TRIGGER:
            sATC_COMMAND.sprintf("@%d,3,%s,%d,%d#", iCommIndex, asChangeFile, iSendData[1], iSendData[0]);
            break;
        case ATC_SET_TC_WATER_VALVE:
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iDataCount, asTCWaterValue);
            break;
        case ATC_SET_MULTI_TC_OFFSET:                                           //Ifor 20251203 add: ATC MultiSensor
            for(int i=0; i<(iDataCount); i++)
            {
                if(i==iDataCount-1)
                {
                    asData+=AnsiString(iSendData[i]);
                }
                else
                {
                    asData+=AnsiString(iSendData[i])+",";
                }
            }
            sATC_COMMAND.sprintf("@%d,%d,%s#", iCommIndex, iDataCount, asData);
            break;
        case ATC_Recipe_By_Channel:                                             //1122 Eastsun 20260521 整合 //Ifor 20241119 add: Handler 可以取得ATC各Channel的PID/Water Valve….等參數.
            for(i=0; i<iDataCount; i++)
            {
                asValue=asValue+sDataList->Strings[3+i]+",";
            }
            iDataCount = atoi(sDataList->Strings[1].c_str());
            if(iDataCount<=0)
                iDataCount=0;
            else
                iDataCount=iDataCount-1;
            RunInfo.ATCParametersCh[iDataCount-1]=asValue;
            break;
        case ATC_Recipe_By_Public:                                              //1123 Eastsun 20260521 //Ifor 20241119 add: Handler 可以取得ATC共用參數的設定值(ATC Recipe Function-2 page).
            for(i=0; i<iDataCount; i++)
            {
                asValue=asValue+sDataList->Strings[2+i]+",";
            }
            RunInfo.ATCParameters=asValue;
            break;
        case ATC_Handler_Transmit_Recipe:                                       //1132 Eastsun 20260521 //Ifor 20241119 add: Handler 可以取得ATC共用參數的設定值(ATC Recipe Function-2 page).
            if(CosFunction.bUseATCFileTransfer==true)
            {
                asFileBuffer="@"+asDataTemp+"#";

                if(asFileBuffer.Pos("@1132,2")==1)
                {
                    iATC_RecipeFileTransfer=6;                           //Ifor 20251229 add: ATC Rrcipe 傳送與接收 0:無需傳送 1:需要傳送 2: 傳送中 6:檔案接收中
                    fLotInfo->ATCTransferFileTime->Enabled=true;
                    iTransferFileDleayCount=0;
//                            frmFileTransfer->bStartFileTransfer=true;
                    frmFileTransfer->OnCommandSocketRead(ClientSocket->Socket, probRecipe, asFileBuffer, true);
                }
                else
                {                                                       //Ifor 20251229 add: ATC Rrcipe 傳送與接收 0:無需傳送 1:需要傳送 2: 傳送中 6:檔案接收中
                    if(iATC_RecipeFileTransfer==2)
                    {
                        iATC_RecipeFileTransfer=3;                      //Ifor 20251229 add: ATC Rrcipe 傳送與接收 0:無需傳送 1:需要傳送 2: 傳送中 6:檔案接收中
                        iTransferFileDleayCount=0;
                        frmFileTransfer->OnCommandSocketRead(ClientSocket->Socket, probRecipe, asFileBuffer, false);
                    }
                }
            }
            break;
    }

    try
    {
        if(ClientSocket->Socket->Connected)
        {
            ClientSocket->Socket->SendBuf(sATC_COMMAND.c_str(), sATC_COMMAND.Length());
            if(A31_Memo->Lines->Count>500)                                      //Ifor 20170522 (wei) ATC Log Count 50 =>500
            {
                SaveCommLog(!cBox_RecordComm->Checked, A31_Memo);               //Eliot 2015_1008
            }
            A31_Memo->Lines->Insert(0, AnsiString(Now().FormatString("yyyy/m/d hh:nn:ss"))+" [Send->] "+sATC_COMMAND);
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TATC_InterfaceForm::SendCommand");
        if(A31_Memo->Lines->Count>500)                                          //Ifor 20170522 (wei) ATC Log Count 50 =>500
        {
            SaveCommLog(!cBox_RecordComm->Checked, A31_Memo);                   //Eliot 2015_1008
        }
        A31_Memo->Lines->Insert(0, AnsiString(Now().FormatString("yyyy/m/d hh:nn:ss"))+" [Fail Send->] "+sATC_COMMAND);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_RunStopsBtnClick(TObject *Sender)
{
    if(A31_RunStopsBtn->Down==true)
    {
        A31_RunStopsBtn->Caption="STOP";
        A31_RunStopsBtn->Font->Color=clRed;
        Run();
    }
    else
    {
        A31_RunStopsBtn->Caption="RUN";
        A31_RunStopsBtn->Font->Color=clGreen;
        Stop();
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Set_TempClick(TObject *Sender)
{
    A31_SetTempLabel->Font->Color=clBlack;
    double dTemp=atof(A31_Temperature->Text.c_str());
    SetAllTemp(dTemp);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Set_Temp_OFSClick(TObject *Sender)
{
    double dOffset[ATC_MAX_SITE];
    A31_TempOffsetLabel->Font->Color=clBlack;
    for(int i=0; i<iHandlerSiteCount; i++)
        dOffset[i]=atof(A31_Temp_OFS->Text.c_str());

    SetOffset(iHandlerSiteCount, dOffset);                                      //A31_Set_Temp_OFSClick not using
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Set_Temp_Reading_ModeClick(TObject *Sender)
{
    //A31_Set_Temp_Reading_Mode->Font->Color = clBlack;
    //iSendData[0] = A31_Temp_Reading_Mode->ItemIndex ;                         //Temperature Read Mode
    //SendCommand(ATC_SET_TRM);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_Recipe_FileClick(TObject *Sender)
{
    double dTemp=atof(A31_Temperature->Text.c_str());                           //Eliot 2016_0324
    ChangeRecipe(ATC_RecipeFile->Text, dTemp);                                  //Eliot 2016_0324
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Initial_SetClick(TObject *Sender)
{
    //A31_Initial_Set->Font->Color = clBlack;
    //iSendData[0] = 1;
    //SendCommand(ATC_INITIAL_SET);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_SetChannelEnabledClick(TObject *Sender)
{
    A31_Initial_Set->Font->Color = clBlack;
    bool bEnabled[ATC_MAX_SITE];

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        bEnabled[i] = false;                                                    //Invalid data.
        if(i<iHandlerSiteCount)
        {
            bEnabled[i] = ATC_Channel[i]->Checked;
        }
    }
    EnablesChannel(iHandlerSiteCount, bEnabled);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ProcessReceiveString_ATC()
{
    AnsiString Str1, Msg;
    int i, iStartPos=0,iEndPos=0, iPos1;
    int iCommand=9999, iDataCt=0;
    bool bFlag=false;
    bool bHasTempIndex=false;                                                   //Ifor 20161006 判斷ATC 版本並是否支援溫度索引值
    int iCheckATCVer=0;
//    double fValue=0;
    AnsiString asResult, asValue, asTemp, str, sBuffer, asFileBuffer;    //Eastsun 20260522 整合: ATC FileTransfer

    TLabel *FW_Label[12]={labFW1, labFW2, labFW3, labFW4, labFW5, labFW6,
                          labFW7, labFW8, labFW9, labFW10, labFW11, labFW12};

    if(asData.Length()!=0)
    {
        iStartPos=asData.Pos("@");
        iEndPos  =asData.Pos("#");
        if(iStartPos==0 && iEndPos!=0)
        {
            asData=asData.SubString(iEndPos+1, asData.Length());
            asDataTemp="";
        }
        else if(iStartPos==0 && iEndPos==0)
        {
            asData="";
        }
        else if(iStartPos!=0 && iEndPos==0)
        {
            asDataTemp=asData.Trim();
            asData="";
        }
        else if(iStartPos!=0 && iEndPos!=0)
        {
            asDataTemp=asData.SubString(iStartPos+1, iEndPos-iStartPos-1);      //Eliot 2016_0720
            asData=asData.SubString(iEndPos+1, asData.Length());
            bFlag=true;
        }
    }
    else if(slReveiveStringList->Count!=0)
    {
        asData=slReveiveStringList->Strings[0];
        asData=asDataTemp+asData;
        asDataTemp="";
        csReceiveString->Acquire();
        try
        {
            slReveiveStringList->Delete(0);
        }
        __finally
        {
            csReceiveString->Release();
        }
    }
    int b=0;
    if(bFlag==true)
    {
        try                                                                     //kevin 20160423
        {
            sDataList->Clear();
            sDataList->CommaText=asDataTemp;
            iCommand=atoi(sDataList->Strings[0].c_str());
            iDataCt=atoi(sDataList->Strings[1].c_str());

            if(iDataCt>ATC_MAX_SITE*2)                                          //kevin 20160507
                iDataCt=ATC_MAX_SITE*2;                                         //Ifor 20170615 (wei) 32Site 超過32筆資料資料會有問題
            for(i=0; i<ATC_MAX_SITE*2; i++)                                     //Ifor 20160527 ATC_MAX_SITE*2
            {
                dATC_Result[i]=9999;
                if(i<iDataCt)
                {
                    OutputDebugString(sDataList->Strings[i+2].c_str());         //Data Result
                    dATC_Result[i]=atof(sDataList->Strings[i+2].c_str());
                }
            }

            //if(iCommand>1000)
            //if(iCommand==ATC_STATE)                                           //ChungHung 20160125 delete
            {
                bCommFlag[ATC_STATE-1000]=true;                                 //ChungHung 20160218 add //Eliot 2015_1027
                iCommCount[ATC_STATE-1000]=0;                                   //ChungHung 20160218 add
            }

            switch(iCommand)
            {
                case ATC_NO_THIS_COMMAND:                                       //0001 ATC not Support this command
                    ShowMessage("ATC not Support this command!!");
                    break;
                case ATC_RECIPE_FILE:                                           //1001 ATC Change Recipe File
                    if(dATC_Result[0]==0)
                        ShowMessage("Recipe file not exist!!");
                    break;
                case ATC_SET_TEMP:                                              //1002 Set ATC Run Temperature
                    if(dATC_Result[0]!=0)
                        A31_SetTempLabel->Font->Color=clGreen;
                    else
                        A31_SetTempLabel->Font->Color=clRed;
                    break;
                case ATC_SET_TOFS:                                              //1003 Temp. Offset
                    if(dATC_Result[0]!=0)
                        A31_TempOffsetLabel->Font->Color=clGreen;
                    else
                        A31_TempOffsetLabel->Font->Color=clRed;
                    break;
                case ATC_SET_TRM:                                               //1005 Temp. Reading Mode
                    if(dATC_Result[0]!=0)
                        A31_Set_Temp_Reading_Mode->Font->Color=clGreen;
                    else
                        A31_Set_Temp_Reading_Mode->Font->Color=clRed;
                    break;
                case ATC_SET_MAX_TEMP:                                          //1006 Set ATC MAX Working Temperature
                    //if(dATC_Result[0]!=0)
                    //    A31_MaxTempLabel->Font->Color = clGreen;
                    //else
                    //    A31_MaxTempLabel->Font->Color = clRed;
                    break;
                case ATC_RUN_STOP:                                              //1007 ATC Run/Stop
                    break;
                case ATC_INITIAL_SET:                                           //1008 ATC Initial all Setting
                    if(dATC_Result[0]!=0)
                        A31_Initial_Set->Font->Color=clGreen;
                    else
                        A31_Initial_Set->Font->Color=clRed;
                    break;
                case ATC_MODE_TYPE:                                             //1009 ATC Mode Type
                    if(ATC_SYSTEM==eNewATCSystem &&                             //JerryYang 20250729 : ATC 3.3+6.0
                       ATC_MixMode==eMixATC60_ATC33)
                    {
                        if(dATC_Result[0]==60 &&
                           iATC_Use_Heat_Count!=32)
                        {
                            iATC_Use_Heat_Count=32;
                            HSys.asATCSYSTEMUSEHEAT=32;
                        }
                        else if(dATC_Result[0]==33 &&
                           iATC_Use_Heat_Count==32)
                        {
                            HSys.asATCSYSTEMUSEHEAT=CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_USEHEAT", 4);
                            iATC_Use_Heat_Count=HSys.asATCSYSTEMUSEHEAT;
                        }

                        fTemp_Set->InitialAddrToATC();

                        fTemp_Set->ReadTempFile(true);
                        bSetTempChange=true;
                    }
                    iATC_MODE_TYPE = dATC_Result[0];
                    fTemp_Set->ControlATC60AirFlow();                           //Steven 20190924 : Add for ATC6.0
                    ShowATC_Page(iATC_MODE_TYPE);
                    if(ATC_SYSTEM==eNewATCSystem &&                             //JerryYang 20250729 : ATC 3.3+6.0
                       ATC_MixMode==eMixATC60_ATC33)
                    {
                        if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60)
                        {
                            iATC_Use_Heat_Count=32;
                            HSys.asATCSYSTEMUSEHEAT=32;
                        }
                        else
                        {
                            iATC_Use_Heat_Count=HSys.asATCSYSTEMUSEHEAT;
                        }
                        ATC_InterfaceForm->SetChannelCount(HSys.asATCSYSTEMUSEHEAT);            //設定目前Channel Count
                        fLotInfo->ShowATCTempPanel();                                           //Steven 20241112 : 調整ATC溫度顯示
                    }

                    break;
                case ATC_READ_TEMP:                                             //1010 Read ATC Temperature
                    for(i=0; i<(iDataCt/2); i++)
                    {
                        dTC[i] = dATC_Result[0+(i*2)];
                        dTJ[i] = dATC_Result[1+(i*2)];
                        ChTempTC[i]->Caption = dTC[i];
                        ChTempTJ[i]->Caption = dTJ[i];
                    }

                    if(iATCForHSMode==1)                                        //海思ATC版本
                    {                                                           //Ifor 20161006 Add 判斷New ATC版本是否支援 Temp Index Check 機制
                        if(iATC_MODE_TYPE==ATC_TYPE_31)                         //Ifor 20161006 預留其他ATC資料格式 "HISI_ATC31_V02.00.K005"                      //Ifor 20200914 Fix HISI_ATC2X_V02.00 -> KL_ATC2X_V02.00
                        {
                            iCheckATCVer=atoi(sATCVerRead.SubString(11, 2).c_str())*100;             //Ifor 20161108 add 取得ATC3.1大版號   //Ifor 20161108 add 取得ATC2.0小版號    //Ifor 20200914 Fix: HISI -> KL 長度減2 13>11
                            iCheckATCVer=iCheckATCVer+atoi(sATCVerRead.SubString(14, 2).c_str());//Ifor 20161108 add 取得ATC3.1小版號       //Ifor 20161108 add 取得ATC2.0小版號    //Ifor 20200914 Fix: HISI -> KL 長度減2 16>14
                            if(iCheckATCVer>=200)                                                   //Ifor 20161108 2 -> 200
                                bHasTempIndex=true;
                        }
                    }
                    else if(iATCForHSMode==2)                                   //非海思ATC版本
                    {
                        if(iATC_MODE_TYPE==ATC_TYPE_31)                         //Ifor 20161006 預留其他ATC資料格式 "2.0.0"
                        {
                            iCheckATCVer= atoi(sATCVerRead.SubString(1,1).c_str())*100;                 //Ifor 20161108 add 取ATC 第一碼版號
                            iCheckATCVer= iCheckATCVer + atoi(sATCVerRead.SubString(3,1).c_str())*10;   //Ifor 20161108 add 取ATC 第二碼版號
                            iCheckATCVer= iCheckATCVer + atoi(sATCVerRead.SubString(5,1).c_str());      //Ifor 20161108 add 取ATC 第三碼版號
                            if(iCheckATCVer>=200)
                                bHasTempIndex=true;
                        }
                    }

                    if(iATC_MODE_TYPE==ATC_TYPE_32)
                    {
                        bHasTempIndex=true;
                    }

                    if(bHasTempIndex==true)
                    {
                        if(iATCTempIndex==dATC_Result[iDataCt-1])               //Ifor 20160719 add ATC first Temp difference Count
                        {
                            iATCTempIndexCount++;
                        }
                        else
                        {
                            iATCTempIndex=dATC_Result[iDataCt-1];
                            iATCTempIndexCount=0;
                        }

                        if(iATCTempIndexCount>=3)
                        {
                            //溫度索引值無改變的Alarm訊息統一                   //Eliot 2016_0802
                            //各位請自動過濾掉(ALM499)不顯示出來,只顯示後面統一的訊息ATC alarm: ATC temperature Index always same error
                            PushBackMessage("(ALM499)ATC alarm: ATC temperature Index always same error");
                        }
                    }
                    bCommFlag[ATC_READ_TEMP-1000]=true;                         //ChungHung 20160218 add //Eliot 2015_1027
                    iCommCount[ATC_READ_TEMP-1000]=0;                           //ChungHung 20160218 add
                    break;
                case ATC_HANDLER_ARM:                                           //1011 Inform Handler Use Arm
                    //dATC_Result[0];
                    break;
                case ATC_TEST_START:                                            //1012 Inform Handler Testing or Test finish
                    //dATC_Result[0];
                    break;
                case ATC_SEND_TJ:                                               //1013 Send TJ Temperature to ATC            //Not been used
                    //dATC_Result[0];
                    break;
                case ATC_ERROR:                                                 //1014 ATC Error Message
                    AlarmMsg->Lines->Clear();
                    iPos1 = asDataTemp.Pos(",");
                    asDataTemp = asDataTemp.SubString(iPos1+1, asDataTemp.Length());//去除Command
                    iPos1 = asDataTemp.Pos(",");
                    asDataTemp = asDataTemp.SubString(iPos1+1, asDataTemp.Length());//去除Count
                    PushBackMessage(asDataTemp);
                    SendCommand(ATC_ERROR);
                    break;
                case ATC_SET_SINGLE_TEMP:                                       //1015 Set Single Site Temperature

                    //dATC_Result[0];
                    break;
                case ATC_SET_SINGLE_OFFSET:                                     //1016 Set Single Site Offset
                    //dATC_Result[0];
                    break;
                case ATC_CANCEL_MESSAGE:                                        //1017 Inform ATC Close Alarm Form
                    //iATC_Result[0];
                    break;
                case ATC_CHILLER_STATUS:                                        //1018 Query Chiller ststus                  //For ATC 3.0/6.0
                    /*Chiller 水溫設定部份 請使用 1018 (ATC_CHILLER_STATUS)
                    Handler ' ATC    @1018,1,1,#
                    ATC ' Handler    @1018,5,1,0,150,149,0,#
                    說明:@指令,資料數量,冰水機編號,冰水機狀態,設定溫度,現在溫度,Alarm Code,#
                    設定溫度及現在溫度的數值請除以10倍才是真正值(例149=14.9度) */
                    dNowATCSetOffset[0]=(double)dATC_Result[2]/10;              //Hmy 20200407 Copy//Hmy 20190426 Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點//kevin 20180620 ??
                    ATC_Data.dChillerSetTemp= dNowATCSetOffset[0];              //kevin 20200409 add
                    break;
                case ATC_SET_PID:                                               //1019 Set ATC PID
                    if(dATC_Result[0]==0)
                        ShowMessage("Set ATC PID Fail!!");
                    break;
                case ATC_READ_PID:                                              //1020 Read ATC PID
                    for(i=0; i<iDataCt; i++)                                    //kevin 20200702
                    {
                        sBuffer= sDataList->Strings[2+i];
                        if(iDataCt%4==0)
                        {
                            if(b>=3)
                                b=0;
                            ATC_Data.iPID[i/3][b]=StrToFloat(sBuffer);
                            b++;
                        }
                    }
                    break;
                case ATC_READ_TEMP_2:                                           //1023 Read ATC Second Temperature
                    for(i=0; i<(iDataCt); i++)
                    {
                        dTC2[i] = dATC_Result[i];
                        ChTempTC2[i]->Caption = dTC2[i];
                    }
                    bCommFlag[ATC_READ_TEMP_2-1000]=true;                       //ChungHung 20160218 add //Eliot 2015_1027
                    iCommCount[ATC_READ_TEMP_2-1000]=0;                         //ChungHung 20160218 add
                    break;
                case ATC_READ_SETTEMP:                                          //1024 Read ATC Set Temp          //ChungHung 20150826 add
                    for(i=0; i<(iDataCt); i++)                                  //kirin 20151102
                    {
                        dNowATCSetTemp[i]=dATC_Result[i];
                    }
                    break;
                case ATC_STATE:                                                 //1025 Read ATC State             //ChungHung 20150826 add
                    if(dATC_Result[0]==1)                                       //ATC程式Run/Stop控溫
                        bATCRun=true;
                    else
                        bATCRun=false;

                    for(i=1; i<(iDataCt); i++)                                  //Eliot 2015_1022
                    {
                        bATC_ModuleState[i-1]=dATC_Result[i];                   //ATC個模組Run/Stop
                    }
                    break;
                case ATC_SW_VER:                                                //1031 ATC Software Version                 //Eliot 2015_1007
                    asATC_SW_Ver=sDataList->Strings[2];
                    sprintf(ATC_Data.SWVer, "%s", asATC_SW_Ver);
                    lab_SW->Caption=asATC_SW_Ver;
                    bGetATC_SW_VerFlag=true;                                    //Eliot 2016_0503
                    break;
                case ATC_FW_VER:                                                //1032 ATC Firmware Version                  //Eliot 2015_1007
                    for(i=0; i<iDataCt; i++)
                    {
                        asATC_FW_Ver[i]=sDataList->Strings[2+i];
                    }

                    for(i=0; i<12; i++)
                    {
                        FW_Label[i]->Caption=asATC_FW_Ver[i];
                    }
                    bGetControllerFW_Flag=true;                                 //Ifor 20170230 Get ATC power supply firmware number
                    break;
                case ATC_SET_2ND_ENABLED:                                       //1034 Setting second point temperature Function enabled.//通知ATC設定第二感溫點功能
                    dATC_Result[0];
                    break;
                case ATC_READ_OFFSET:                                           //1035 Read ATC Set Offset                   //Eliot 2015_1124
                    for(i=0; i<(iDataCt); i++)
                    {
                        dNowATCSetOffset[i]=dATC_Result[i];
                        ATC_Data.iOffset[i]=dNowATCSetOffset[i];                //kevin 20200702 add ARC dta
                    }
                    break;
                case ATC_GET_NOW_RECIPE:                                        //1036 Get Now Recipe File                   //Eliot 2015_1126
                    ATC_Now_RecipeFile->Caption = sDataList->Strings[2];
                    fLotInfo->lblATC_Now_RecipeFile->Caption="ATC Now RecipeFile : "+sDataList->Strings[2];
                    bGetNowRecipeFlag=true;
                    break;
                case ATC_GET_RECIPE_LIST:                                       //1037 Get Recipe File List                  //Eliot 2015_1126
                    fTemp_Set->cbbATC_RecipeFile->Items->Clear();

                    iATC_DataCount=iDataCt;                                     //kevin 20160127
                    for(i=0; i<iATC_DataCount; i++)                             //kevin 20160127
                    {
                        ATC_RecipeFile->Items->Add(sDataList->Strings[2+i]);
                        ATC_WorkFileList[i]=sDataList->Strings[2+i];            //kevin 20151127
                        fTemp_Set->cbbATC_RecipeFile->Items->Add(sDataList->Strings[2+i]);
                    }

                    if(ATC_RecipeFile->ItemIndex!=0)
                        ATC_RecipeFile->ItemIndex=0;
                    bGetRecipeListFlag=true;
                    break;
                case ATC_SET_CHILLER_TEMP:                                      //1039 SET chiller temp     kevin 20160425 add
                    //dATC_Result[0];
                    break;
                case ATC_SEND_TEMP_READY:                                       //1040 send temperature ready                //2016-04-27    Dell    Add
                    iATC_DataCount=iDataCt;
                    bGetATC_SEND_TEMP_READY=true;
                    break;
                case ATC_SITE_2ND_CHECK:                                        //1041 Site Check Second Temperature         //Eliot 2016_0503
                    //dATC_Result[0];
                    break;
                case ATC_RUN_SELFTEST:                                          //1044 Run ATC selt-test functin             //2016/7/17 Dell    Add
                    flagATCSelfTestResult=dATC_Result[0];
                    Edit1->Text=flagATCSelfTestResult;

                    for(i=0; i<ATC_MAX_SITE; i++)                               //Eliot 20161130 增加Fail Site資料 ==>
                    {
                        iSelfTestFailSite[i]=-1;
                        if(i<(iDataCt-1))
                        {
                            iSelfTestFailSite[i]=dATC_Result[i+1];
                        }
                    }
                    break;
                case ATC_GET_2ND_STATUS:                                        //1045 Get second sensor function enabled.   //2016-08-01    Dell    Add
                    bATC2ndFunctionFlag=dATC_Result[0];                         //Eliot 2016_0802
                    ATC_Data.GET_2ND_STATUS=dATC_Result[0];
                    for(i=0; i<iDataCt; i++)                                    //kevin 20200410 add
                    {
                        dNowATCSetOffset[i]=dATC_Result[i+2];                   //Hmy 20200407 Copy//Hmy 20190426 Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點//kevin 20180620 ??
                        sBuffer=dNowATCSetOffset[i];
                        ATC_Data.ATC_GET_GrandBand[i]=dNowATCSetOffset[i];
                    }
                    break;
                case ATC_MANUAL_SELFTEST:                                       //1046 Manual ATC selt-test functin          //Eliot 2016_0816
                    flagATCSelfTestResult=dATC_Result[0];
                    Edit1->Text=flagATCSelfTestResult;
                    break;
                case ATC_SELFTEST_STATUS:                                       //1047 Get self-test status                  //Eliot 2016_0816
                    iATCSelfTestStatus=dATC_Result[0];
                    Edit2->Text=iATCSelfTestStatus;
                    break;
                case HANDLER_STATUS:                                            //1048 Check Handler Status                  //Eliot 2016_0816
                    SendHandlerStatus();
                    break;
                case ATC_AIRMACHINE_STATUS:                                     //add air Machine    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
//                  @1049,8,asStatus,iDefro,dTemp,dTemp2,dSet,sAlm,dWindSpeed);
                    if(iATC_MODE_TYPE==61)
                    {
                        AirMachineInfo.asATC_AirMachineStatus                  = sDataList->Strings[2];    ////Run、Stop、Disconnect
                        AirMachineInfo.iDefrostSec                             = atoi(sDataList->Strings[3].c_str());
                        AirMachineInfo.iATC_AirMachineSocketChTemp[0]          = atof(sDataList->Strings[6].c_str());
                        AirMachineInfo.iATC_AirMachineSocketChTemp[1]          = atof(sDataList->Strings[7].c_str());
                        AirMachineInfo.asATC_AirMachineSetTemp                 = sDataList->Strings[9];
                        AirMachineInfo.asATC_AirMachineAlarm                   = sDataList->Strings[10];
                        AirMachineInfo.iATC_AirVolume                          = atoi(sDataList->Strings[12].c_str());

                        AirMachineInfo_Index.asATC_AirMachineStatus            = sDataList->Strings[2];
                        AirMachineInfo_Index.iDefrostSec                       = atoi(sDataList->Strings[3].c_str());
                        AirMachineInfo_Index.iATC_AirMachineSocketChTemp[0]    = atof(sDataList->Strings[4].c_str());
                        AirMachineInfo_Index.iATC_AirMachineSocketChTemp[1]    = atof(sDataList->Strings[5].c_str());
                        AirMachineInfo_Index.asATC_AirMachineNowTemp           = sDataList->Strings[8];
                        AirMachineInfo_Index.asATC_AirMachineAlarm             = sDataList->Strings[10];
                        AirMachineInfo_Index.iATC_AirVolume                    = atoi(sDataList->Strings[11].c_str());
                    }
                    else
                    {
                        AirMachineInfo.asATC_AirMachineStatus                  = sDataList->Strings[2];    ////Run、Stop、Disconnect
                        AirMachineInfo.iDefrostSec                             = atoi(sDataList->Strings[3].c_str());
                        AirMachineInfo.iATC_AirMachineSocketChTemp[0]          = atof(sDataList->Strings[4].c_str());
                        AirMachineInfo.iATC_AirMachineSocketChTemp[1]          = atof(sDataList->Strings[5].c_str());
                        AirMachineInfo.asATC_AirMachineSetTemp                 = sDataList->Strings[6];
                        AirMachineInfo.asATC_AirMachineAlarm                   = sDataList->Strings[7];
                        AirMachineInfo.iATC_AirVolume                          = atoi(sDataList->Strings[8].c_str());
                    }
                    Send_Rec_AirMachineStatus.iA2H_RecStatus   = 1;             //設定/讀取AirStream溫度
                    break;
                case ATC_LOT_START:                                             //1050 Lot Start, Lot ID                    //Eliot 2016_1020
                    bATCLotStartStatus=dATC_Result[0];                          //Ifor 20161121 add 接收ATC_LOT_START回傳值
                    break;
                case ATC_LOT_END:                                               //1051 Lot End                              //Eliot 2016_1020
                    bATCLotEndStatus=dATC_Result[0];                            //Ifor 20161121 add 接收ATC_LOT_End回傳值
                    break;
                case ATC_SELFTEST_RESULT:                                       //1052 Query Self-Test Result                //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
                    for(i=0; i<ATC_MAX_SITE; i++)
                    {
                        iSelfTestResult[i]=-1;
                        if(i<iDataCt)
                        {
                            iSelfTestResult[i]=dATC_Result[i];                  //==> Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
                        }
                    }
                    break;
                case ATC_GET_SN:                                                //1054 Get ATC power supply serial number    //20170202W05   Dell    Add Get ATC power supply serial number
                    for(i=0; i<ATC_MAX_SITE; i++)
                    {
                        asControllerSN[i]="";
                        if(i<iDataCt)
                        {
                            asControllerSN[i]=sDataList->Strings[2+i];          //==> Eliot 20170221 Get ATC power supply serial number
                        }
                    }
                    bGetControllerSN_Flag=true;
                    break;
                case ATC_51_SET_REF_ENABLED :                                   //Hmy20180730 Add ATC Refrigerator Status //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                    break;                                                      //ATC 收/發壓縮機狀態命令
                case ATC_51_GET_REF_STATUS  :                                   //Hmy20180730 Add ATC Refrigerator Status
//                    bReadRefrigerantMode_AllStatus = false;                     //ATC 收/發壓縮機狀態命令
//                    for(int i=0;i<(iDataCount);i++)
//                    {
//                        iATC_RefrigeratorUserMode[0][i]=dATC_Result[i]; // Modify iATC_Result-> dATC_Result_Offset
//                    }
//                    iATC_RefrigeratorUserMode[0][Total_Compressor] = 2;

                    if(bReadRefrigerantMode_AllStatus ==false || iDataCount==8)  //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
                    {
                        bReadRefrigerantMode_AllStatus = false;
                        for(int i=0;i<(iDataCount);i++)
                        {
                            iATC_RefrigeratorUserMode[0][i]=dATC_Result[i]; //Modify iATC_Result-> dATC_Result_Offset
                        }
                        iATC_RefrigeratorUserMode[0][iATC_Refrigerator_Num] = 2;
                    }
                    else
                    {
                        for(int i=0;i<Total_Compressor;i++)    //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
                        {
                            for(int j=0;j<9;j++)
                            {
                                dATC_RefrigeratorAllStatus[i][j]=dATC_Result[i*9+j];
                            }
                            iATC_RefrigeratorUserMode[0][i]=dATC_Result[i*9];
                        }
                        dATC_RefrigerantMachineHighLimit = dATC_Result[Total_Compressor*9+1];
                        dATC_RefrigerantMachineLowLimit  = dATC_Result[Total_Compressor*9+2];
                        iATC_RefrigeratorUserMode[0][iATC_Refrigerator_Num] = 2;
                    }
                    bReadRefrigerantMode_Send=false;
                    bReadRefrigerantMode_Recv=true;
                    break;                                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==

                case HANDLER_FFC_SETTING:                                       //1063 Ifor 20190730 : add FFC Temperature Offset
                    break;
                case HANDLER_ATO_RECORD:                                        //1064 Ztex 2023.04.18 Add HT-1032 ATC CMD  //Hmy 20200901 Add Auto K Temp Auto Save//Record Auto Temperature Offset data
                    break;
                case HANDLER_FFC_ENABLED:                                       //1065 Ifor 20190730 : add FFC Temperature Offset
                    break;
                case HANDLER_FFC_TRIGGER:                                       //1066 Ifor 20190730 : add FFC Temperature Offset
                    break;
                case ATC_GET_CONTROL_MODE:                                      //1067
                    for(i=0; i<ATC_MAX_SITE; i++)
                    {
                        asControlMode[i]="";
                        if(iDataCount>8)                                        //Ifor 20251203 Fix 溢位
                            iDataCount=8;
                        if(i<iDataCount)
                        {
                            asControlMode[i]=sDataList->Strings[2+i];
                        }
                    }
                    ChangeTJMode();
                    break;
                case ATC_GET_PFC_PARAMETER:                                             //1069 讀取單一溫度的PF參數值    送 @1069,1,80#  回傳 @1069,21,80,100,80,70,62,61,60,59,57,55,55,53,51,49,48,44,61,61,30,20,10#
                    for(int i=0; i<14; i++)
                    {
                        for(int j=0; j<20; j++)
                        {
                            asALLGETPFCPARAMETER[i][j]=iPFCPARAMETERData[j+i*20];
                        }
                    }
                    break;
                case 1070:       //@1070,1,1#
                    if(dATC_Result[0]==1)
                        bSETPFCPARAMETER=true;
                    else
                        bSETPFCPARAMETER=false;
                    break;
                case ATC_READ_WATER_VALVE:                                      //1071 Read ATC Water valve value            //Eliot 20200406
                    for(i=0; i<(iDataCt); i++)
                    {
                        if(dATC_Result[i]==9999)                                //kevin 20201202 add error
                        {
                            dNowATCSetOffset[i]=ATC_Data.ATC_READ_WATER_Value[i];       //Hmy 20200407 Copy//Hmy 20190426 Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點//kevin 20180620 ??
                            ATC_Data.ATC_READ_WATER_Value[i]=dNowATCSetOffset[i];       //kevin 20200409 add
                        }
                        else
                        {
                            dNowATCSetOffset[i]=dATC_Result[i];                 //Hmy 20200407 Copy//Hmy 20190426 Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點//kevin 20180620 ??
                            ATC_Data.ATC_READ_WATER_Value[i]=dNowATCSetOffset[i];      //kevin 20200409 add
                        }
                    }
                    break;
                case ATC_CMD_READ_FUNCTION_STATUS:                             //1072
                    dNowATCSetOffset[0]=dATC_Result[0];                         //Hmy 20200407 Copy//Hmy 20190426 Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點//kevin 20180620 ??
                    ATC_Data.ATC_READ_FUNCTION_STATUS=dNowATCSetOffset[0];      //kevin 20200409 add
                    iReceiveATCData=0;                                          //kevin 20200608 讀取ATC 參數資料=0
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 20200702 write ATC 參數
                        ASESendMessage->WriteATCDataToASE();                    //kevin 20200414 ATC data
                    break;
                case HANDLER_TEST_SITEMAPPING:                                  //1075 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Handler send site mapping   //JerryYang 20231128 : 傳送ATC site map
                    break;
                case ATC_SET_TJ_ENABLED:                                        //1076 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ Function enabled     //2021-07-20    Dell
                    break;
                case ATC_SET_TJ_PARAMETER:                                      //1077 Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)//Set TJ slope and offset     //2021-07-20    Dell
                    break;
                case ATC_SET_PF_PARAMETER:                                      //1104 //Hmy 20240207 Add Send Pf Par to Atc
                    break;
                case ASIF_TJ_EFUSED:                                            //1112  //Steven 20240903 : for MTK ASIF data
                    break;
                case ASIF_TJ_REQUEST:                                           //@1113,3,ASIF_TJ_REQUEST,"LVTS1_A:[1:99999,2:99999,3:99999]","LVTS2_A:[1:99999,2:99999,3:99999]"#
                case ASIF_TJ_FB:                                                //@1114,3,ASIF_TJ_FB,"LVTS1_A:[1:99999,2:99999,3:99999]","LVTS1_B:[1:99999,2:99999,3:99999]"#
                    Msg="";

                    for(int i=3; i<iDataCt; i++)
                    {
                        Str1=sDataList->Strings[i];
                        Str1=StringReplace(Str1, ":[", "\":\"[", TReplaceFlags()<<rfReplaceAll);
                        Str1=StringReplace(Str1, "]", "]\"", TReplaceFlags()<<rfReplaceAll);
                        Str1=AnsiString("\"")+Str1;
                        Msg=Msg+AnsiString(",")+Str1;
                    }
                    Msg=AnsiString("ASIF_TJ_FB,{")+Msg+AnsiString("}");
                    fMain->SendMSG_CMD(MSG_CMD_ASIF_TJ_FB, Msg);
                    break;
                case HANDLER_2DID:
                    break;
                case ATC_GET_SLOPE_OFFSET:
                    for(int i=0; i<iDataCount/2; i++)
                    {
                        asALLGETSLOPEOFFSET[0][i]=asSLOPEOFFSET[i*2];
                        asALLGETSLOPEOFFSET[1][i]=asSLOPEOFFSET[i*2+1];
                        asALLSETSLOPEOFFSET[0][i]=asSLOPEOFFSET[i*2];
                        asALLSETSLOPEOFFSET[1][i]=asSLOPEOFFSET[i*2+1];
                    }
                    break;
                case ATC_SET_SLOPE_OFFSET:
                    if(dATC_Result[0]==1)
                        bSETSLOPEOFFSET=true;
                    else
                        bSETSLOPEOFFSET=false;
                    break;
                case ATC_FEEDBACK_VOLTAGE:
                    for(i=0; i<(iDataCount); i++)         //wei 20211206 Get the Tj Voltage
                    {
                        asTJVoltage[i]=dATC_Result[i];
                    }
                    break;
                case ATC_QUERY_TJ_TEMP:  //Query Tj Temp(Max/Min/Avg)
                    for(i=0; i<ATC_MAX_SITE; i++)
                    {
                        asQueryTJTemp[i][0]=dATC_Result[i*3];
                        asQueryTJTemp[i][1]=dATC_Result[i*3+1];
                        asQueryTJTemp[i][2]=dATC_Result[i*3+2];
                    }
                    break;
                case ATC_READ_DYNAMIC_PID:
                    for(i=0; i<iDataCount; i++)
                    {
                        asReadDynamicPID[i]=dATC_Result[i];
                    }
                    break;
                case ATC_SET_T2OFS:                                             //KenHsieh 20240311 : add Tc2 Offse
                    break;
                case ATC_SET_SINGLE_T2OFS:                                      //KenHsieh 20240311 : add Tc2 Offse
                    break;
                case ATC_SET_MULTI_TC_OFFSET:                                  //Ifor 20251203 add: ATC MultiSensor
                case ATC_Multi_Temperature_Control:                            //Ifor 20251203 add: ATC MultiSensor
                    asDataTemp="";
                    break;
            }
            asDataTemp="";
            if(CosFunction.bUseATCFileTransfer==true)  //Eastsun 20260522 add: ATC Recipe FileTransfer timeout monitoring
            {
                asFileBuffer="";
                if(iATC_RecipeFileTransfer==3)  //download finish check
                {
                    iTransferFileDleayCount++;
                    if(probRecipe->Position==probRecipe->Max)
                    {
                        RecordProcess("FTP Download ATC PID FileTransfer Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Download ATC PID FileTransfer Finish!!");
                        iATC_RecipeFileTransfer=0;
                    }
                    else if(iTransferFileDleayCount>=20)  //10Sec
                    {
                        iTransferFileDleayCount=0;
                        iATC_RecipeFileTransfer=0;
                        RecordProcess("FTP Download PID File Transfer from ATC Not Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Download PID File Transfer from ATC Not Finish!!");
                    }
                }

                if(iATC_RecipeFileTransfer==2)
                {
                    iTransferFileDleayCount++;
                    if(iTransferFileDleayCount>=25)  //10Sec
                    {
                        iTransferFileDleayCount=0;
                        iATC_RecipeFileTransfer=0;
                        RecordProcess("FTP Download PID File Transfer from ATC Not Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Download PID File Transfer from ATC Not Finish!!");
                    }
                }

                if(iATC_RecipeFileTransfer==6)
                {
                    iTransferFileDleayCount++;
                    if(probRecipe->Position==probRecipe->Max)
                    {
                        iATC_RecipeFileTransfer=0;
                        iTransferFileDleayCount=0;
                        RecordProcess("FTP Upload ATC PID FileTransfer Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Upload ATC PID FileTransfer Finish!!");
                    }
                    else if(iTransferFileDleayCount>=10)  //10Sec
                    {
                        iTransferFileDleayCount=0;
                        iATC_RecipeFileTransfer=0;
                        RecordProcess("FTP Upload PID File Transfer from ATC Not Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Upload PID File Transfer from ATC Not Finish!!");
                    }
                }

                if(iATC_RecipeFileTransfer==4)
                {
                    iTransferFileDleayCount++;
                    if(iTransferFileDleayCount>=20)  //10Sec
                    {
                        iTransferFileDleayCount=0;
                        iATC_RecipeFileTransfer=0;
                        fLotInfo->ATCTransferFileTime->Enabled=true;
                        RecordProcess("FTP Upload PID File Transfer from ATC Not Finish!!");
                        fFTPClient->memoFTP->Lines->Add("FTP Upload PID File Transfer from ATC Not Finish!!");
                    }
                }
            }
        }
        catch(...)                                                              //kevin 20160423
        {
            MyDBIProcess("Exception", "TATC_InterfaceForm::ProcessReceiveString_ATC");
            bATCCommuateError= true;                                            //kevin 20160423
            aATCCommuateError=asDataTemp+":"+sDataList->Strings[0]+":"+":"+sDataList->Strings[1]+":"+sDataList->Strings[2];//kevin 20160423         //kevin 20160423
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::TimerTimer(TObject *Sender)
{
    static bool bRunTimer=false;
    if(InitialOK==false || bRunTimer==true)
        return;

    bRunTimer=true;

    ProcessReceiveString_ATC();
    Label7->Caption = slReveiveStringList->Count;
    if(IsConnect()==true)
    {
        SendReadTempComm();
    }

    bRunTimer=false;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Read_TempClick(TObject *Sender)
{
    double dTemp[ATC_MAX_SITE];
    ReadTC(ATC_MAX_SITE, dTemp);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SendReadTempComm()
{
    static double Time1=0, Time2=0;
    static int iCount=9;
    Time2=MyTickCount();
    if(abs(Time2-Time1)>=ReadTempInterval)                                      //Ifor 20180514 : add 避免Time2 49天後歸0導致條件永遠無法成立(負數)
    {
        Time1=Time2;
        if(Tri_Temp_Machine==1)                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            iSendData[0]=TriTemperature_TotalChannel;                           //設定 TotalChannel
        }
        else
        {
            iSendData[0]=iHandlerSiteCount;//1;
        }

        if(bCommFlag[ATC_READ_TEMP-1000]==true)
        {
            SendCommand(ATC_READ_TEMP);
            bCommFlag[ATC_READ_TEMP-1000]=false;
            iCommCount[ATC_READ_TEMP-1000]=0;
        }

        if(Temperature.bUseReferTempSensor==true)                               //TriTemp 使用第二點溫度
        {
            if(bCommFlag[ATC_READ_TEMP_2-1000]==true)
            {
                SendCommand(ATC_READ_TEMP_2);
                bCommFlag[ATC_READ_TEMP_2-1000]=false;
                iCommCount[ATC_READ_TEMP_2-1000]=0;
            }
        }

/*        if(Tri_Temp_Machine==1)                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            if(Temperature.bUseReferTempSensor==true)                               //TriTemp 使用第二點溫度
            {
                if(bCommFlag[ATC_READ_TEMP_2-1000]==true)
                {
                    SendCommand(ATC_READ_TEMP_2);
                    bCommFlag[ATC_READ_TEMP_2-1000]=false;
                    iCommCount[ATC_READ_TEMP_2-1000]=0;
                }
            }
            else
            {
                if(bCommFlag[ATC_READ_TEMP_2-1000]==true)
                {
                    SendCommand(ATC_READ_TEMP_2);
                    bCommFlag[ATC_READ_TEMP_2-1000]=false;
                    iCommCount[ATC_READ_TEMP_2-1000]=0;
                }
            }
        }*/

        //========================================================================
        // Steven 20260423 : ATC 命令分頻化（質數錠峰策略）
        // Reporter: Matthew Han <matthew@teratechkorea.com> ATK HT-9046LS Hybrid
        // 原因：ATC 端指令密度過高 ~3.5/s，%9 %11 %13 公倍數 tick 炸量
        // 策略：使用不同質數 + offset 錠峰，最大同 tick = 3 個，穩態 ~1.7/s
        //========================================================================
        if(bCommFlag[ATC_STATE-1000]==true && (iCount%2)==1)                    //Steven 20260423 : 1025 每 2 秒，offset 1，與 1010 錯峰
        {
            iSendData[0]=1;
            SendCommand(ATC_STATE);
            bCommFlag[ATC_STATE-1000]=false;
            iCommCount[ATC_STATE-1000]=0;
        }
        iCount++;

        // ----- Tier B (9 秒)：1024 / 1035 拆兩個 offset、不同 tick -----
        if(iCount%9==2)
            SendCommand(ATC_READ_SETTEMP);                                      //Steven 20260423 : 1024 9秒 offset 2

        if(iCount%9==5)
            SendCommand(ATC_READ_OFFSET);                                       //Steven 20260423 : 1035 9秒 offset 5

        // ----- Tier C (30 秒質數)：1067 與 1128 錯峰 -----
        if(iCount%30==7)
        {
            if(iATC_MODE_TYPE!=ATC_TYPE_60 &&
               iATC_MODE_TYPE!=ATC_TYPE_70)                                     //ATC 6.0/7.0 不支援 1067
                SendCommand(ATC_GET_CONTROL_MODE);                              //Steven 20260423 : 1067 30秒 offset 7
        }

        if(iCount%30==19)
        {
            if(iATC_MODE_TYPE==ATC_TYPE_35 ||
               iATC_MODE_TYPE==ATC_TYPE_36)                                     //Steven 20260423 : 1128 僅 ATC 3.5/3.6 支援 Dynamic PID
                SendCommand(ATC_READ_DYNAMIC_PID);                              //Steven 20260423 : 1128 30秒 offset 19
        }

        // ----- Tier C (60 秒)：1071 水閥、 1036 Now Recipe 監控 -----
        if(iCount%60==23)
        {
            if(iATC_MODE_TYPE!=ATC_TYPE_60 &&
               iATC_MODE_TYPE!=ATC_TYPE_70)                                     //ATC 6.0/7.0 不支援 1071
                SendCommand(ATC_READ_WATER_VALVE);                              //Steven 20260423 : 1071 60秒 offset 23
        }

        if(iCount%60==41)
            GetNowRecipeFile();                                                 //Steven 20260423 : 1036 60秒 offset 41，避免 uLotInfo case 2 連續輪詢

        // ----- Tier D (300 秒)：1033 Time Sync -----
        if(iCount%300==137)
            SendCommand(ATC_TIME_SYNC);                                         //Steven 20260423 : 1033 改為 5 分鐘 offset 137

        // ----- iCount 防溢位（1 小時重設，LCM(2,9,30,60,300)=900 安全倍數）-----
        if(iCount>=3600)
            iCount=0;

        if(bQUERYTJ && iATC_MODE_TYPE!=ATC_TYPE_60 && iATC_MODE_TYPE!=ATC_TYPE_70)  //Steven 20260415 : ATC 6.0/7.0 unsupported (1088)
            SendCommand(ATC_QUERY_TJ_TEMP);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetReadTempInterval(int iInterval)
{
    ReadTempInterval = iInterval;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Handler_ArmClick(TObject *Sender)
{
    HandlerArm(0);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_Set_Single_Temp_OFSClick(TObject *Sender)
{
    int iCH=atoi(A31_Single_Temp_OFS_Site->Text.c_str());
    double dTemp=atof(A31_Single_Temp_OFS->Text.c_str());

    SetSingleOffset(iCH, dTemp);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_Set_Single_TempClick(TObject *Sender)
{
    int iCH=atoi(A31_Single_Temp_Site->Text.c_str());
    double dTemp=atof(A31_Single_Temp->Text.c_str());

    SetSingleTemp(iCH, dTemp);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Test_StartClick(TObject *Sender)
{
    StartTesting();
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ClearATCMessage()
{
    SendCommand(ATC_CANCEL_MESSAGE);

    LastMsg="";                                                                 //ChungHung 20150826 add fix for continue error message
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Button2Click(TObject *Sender)
{
//    SendCommand(ATC_CANCEL_MESSAGE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Chiller_StatusClick(TObject *Sender)
{
//    iSendData[0] =1;
//    SendCommand(ATC_CHILLER_STATUS);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Connect()
{
    if(bConnecting == true)
        return;
    if(IsConnectFlag==false)
    {
        ClientSocket->Active = true;
        bConnecting = true;
        if(CosFunction.bUseATCFileTransfer==true)  //Eastsun 20260522 add: ATC Recipe FileTransfer
        {
            Sleep(100);
            FileSocket->Active = true;
            sheRecipe->Brush->Color = clYellow;
            labRecipe->Caption = "嘗試連線...";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Disconnet()
{
    ClientSocket->Active = false;
    bConnecting = false;
    IsConnectFlag=false;
    if(CosFunction.bUseATCFileTransfer==true)  //Eastsun 20260522 add: ATC Recipe FileTransfer
        FileSocket->Active = false;
    ATC_InterfaceForm->Caption = "OFF-Line";                                    //Eliot 2015_1130
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::IsConnect()
{
    return IsConnectFlag;
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::IsConnecting()                              //ChungHung 20150125 add try to fix disconnect
{
    return bConnecting;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ChangeRecipe(AnsiString RecipeName)
{
    asRecipeFile=RecipeName.Trim();                                             //Set Recipe File
    ATC_RecipeFile->Text=asRecipeFile;
    SendCommand(ATC_RECIPE_FILE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ChangeRecipe(AnsiString RecipeName, double dTemperature) //Eliot 2016_0324
{
    asRecipeFile = RecipeName.Trim();                                           //Set Recipe File
    ATC_RecipeFile->Text = asRecipeFile;

    iSendData[0] = 9999;                                                        //Invalid data.     //Eliot 2016_0324
    iSendData[0] = (dTemperature*10+0.5);                                       //Use Temperature   //Eliot 2016_0324
    SendCommand(ATC_RECIPE_FILE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetChannelCount(int iCount)
{
    iHandlerSiteCount = iCount;
    TColor pnlColor[2]={clAqua, clLime};

    int iIndex=0;
    AnsiString asName;
    //Channel Enabled
    for(int j=0; j<4; j++)
    {
        for(int i=0; i<8; i++)
        {
            ATC_Channel[iIndex]->Caption=asName.sprintf("CH%d", iIndex+1);
            ATC_Channel[iIndex]->Visible=false;
            if(iIndex<iHandlerSiteCount)
            {
                ATC_Channel[iIndex]->Visible=true;
                ATC_Channel[iIndex]->Left= 8 + (i*72);
                ATC_Channel[iIndex]->Top =24 + (j*24);
            }
            iIndex++;
        }
    }
    //Show Temperature
    iIndex=0;
    for(int j=0; j<2; j++)
    {
        for(int i=0; i<16; i++)
        {
            ChPanel[iIndex]->Caption=asName.sprintf("CH%d",iIndex+1);
            ChPanel[iIndex]->Visible=false;
            if(iIndex<iHandlerSiteCount)
            {
                ChPanel[iIndex]->Visible    =true;
                ChPanel[iIndex]->Left       =32 + (i*40);
                ChPanel[iIndex]->Top        = 0 + (j*104);
                ChPanel[iIndex]->Color      =pnlColor[iIndex%2];

                ChTempTC[iIndex]->Visible   =true;
                ChTempTC[iIndex]->Left      =32 + (i*40);
                ChTempTC[iIndex]->Top       =24 + (j*104);
                ChTempTC[iIndex]->Color     =pnlColor[iIndex%2];

                ChTempTC2[iIndex]->Visible  =true;
                ChTempTC2[iIndex]->Left     =32 + (i*40);
                ChTempTC2[iIndex]->Top      =48 + (j*104);
                ChTempTC2[iIndex]->Color    =pnlColor[iIndex%2];

                ChTempTJ[iIndex]->Visible   =true;
                ChTempTJ[iIndex]->Left      =32 + (i*40);
                ChTempTJ[iIndex]->Top       =72 + (j*104);
                ChTempTJ[iIndex]->Color     =pnlColor[iIndex%2];
            }
            iIndex++;
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::SetAddressPort(AnsiString asAddress, int iPost)
{
    if(IsConnectFlag==true)
        return false;

    ClientSocket->Active=false;
    ClientSocket->Address=asAddress.Trim();
    ClientSocket->Port=iPost;
    if(CosFunction.bUseATCFileTransfer==true)  //Eastsun 20260522 add: ATC Recipe FileTransfer
    {
        FileSocket->Active=false;
        FileSocket->Address = asAddress.Trim();
        FileSocket->Port = 5678;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Run()
{
    A31_RunStopsBtn->Down=true;
    A31_RunStopsBtn->Caption="STOP";
    A31_RunStopsBtn->Font->Color=clRed;
    iSendData[0]=1;
    SendCommand(ATC_RUN_STOP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Stop()
{
    A31_RunStopsBtn->Down=false;
    A31_RunStopsBtn->Caption="RUN";
    A31_RunStopsBtn->Font->Color=clGreen;
    iSendData[0]=0;
    if(iATC_MODE_TYPE==ATC_TYPE_50)                                             //Eliot 2015_0930
    {
        for(int i=0; i<iATC_ModuleCount; i++)
        {
            iSendData[0]=i+1;
            if(ATC_Module[i]==true)
            {
                SendCommand(ATC_RUN_MODULE);
            }
            else
            {
                SendCommand(ATC_STOP_MODULE);
            }
        }
    }
    else
    {
        SendCommand(ATC_RUN_STOP);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetAllTemp(double dTemperature)
{
    A31_Temperature->Text = dTemperature;
    if(Tri_Temp_Machine==1)                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        iDataCount=TriTemperature_TotalChannel;
    else
        iDataCount=iHandlerSiteCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i]=9999;                                                      //Invalid data.
        if(i<iDataCount)
        {
            if(dTemperature<0)                                                  //pig 2016.01.14 ATC 8CHANNEL
                iSendData[i]=(dTemperature*10.0-0.5);                           //Single Temperature
            else
                iSendData[i]=(dTemperature*10.0+0.5);                           //Single Temperature
        }
    }

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        if(bUT150HasUse[TriTemp_Ch[i]]==false && bUT150Install[TriTemp_Ch[i]]==true)
            iSendData[i]=dTemperature*10.0+0.5;
    }
    SendCommand(ATC_SET_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetMultiZoneTemp(double dTemperature)
{
    iDataCount = iHandlerSiteCount;
    int iChannel=0;
    if(iDataCount>ATC_MAX_SITE)      //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;    //Invalid data.
        if(i<iHandlerSiteCount)
        {
            iChannel=i%4;
            if(Temperature.bZoneTempEnable[iChannel])
            {
                if(Temperature.dZoneTempSetting[iChannel]<0)                        //pig 2016.01.14 ATC 8CHANNEL
                    iSendData[i] = (Temperature.dZoneTempSetting[iChannel]*10.0-0.5);   //Single Temperature
                else
                    iSendData[i] = (Temperature.dZoneTempSetting[iChannel]*10.0+0.5);   //Single Temperature
            }
            else
            {
                if(Temperature.dZoneTempSetting[iChannel]<0)                        //pig 2016.01.14 ATC 8CHANNEL
                    iSendData[i] = (dTemperature*10.0-0.5);   //Single Temperature
                else
                    iSendData[i] = (dTemperature*10.0+0.5);   //Single Temperature
            }
        }
    }
    SendCommand(ATC_SET_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetAirValve(bool bControl, bool bOnOff)     //Steven 20181102 : Control ATC6.0氣流開關
{
    iDataCount=9;
    iSendData[0]=1;

    if(bOnOff)
    {
        iSendData[1]=1;
        iSendData[2]=1;
        iSendData[3]=1;
        iSendData[4]=1;
        iSendData[5]=1;
        iSendData[6]=1;
        iSendData[7]=1;
        iSendData[8]=1;
//        iSendData[1]=1;
//        iSendData[2]=1;
//        iSendData[3]=0;
//        iSendData[4]=0;
//        iSendData[5]=0;
//        iSendData[6]=0;
//        iSendData[7]=1;
//        iSendData[8]=1;
    }
    else
    {
        iSendData[1]=0;
        iSendData[2]=0;
        iSendData[3]=0;
        iSendData[4]=0;
        iSendData[5]=0;
        iSendData[6]=0;
        iSendData[7]=0;
        iSendData[8]=0;
    }
    SendCommand(ATC_60_SET_AIRVALVE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTemp(int iChCount, double *dTemp)
{
    A31_Temperature->Text = dTemp[0];
    iDataCount=iChCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i]=9999;                                                      //Invalid data.
        if(i<iChCount)
        {
            if(dTemp<0)                                                         //pig 2016.01.14 ATC 8CHANNEL
                iSendData[i]=(dTemp[i]*10.0-0.5);                               //Single Temperature
            else
                iSendData[i]=(dTemp[i]*10.0+0.5);                               //Single Temperature
        }
    }
    SendCommand(ATC_SET_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetSingleTemp(int iChannel, double dTemp)
{
    A31_Single_Temp_Site->Text = iChannel;
    A31_Single_Temp->Text = dTemp;
    iSendData[0]=iChannel;
    if(dTemp<0)                                                                 //pig 2016.01.14 ATC 8CHANNEL
        iSendData[1]=(dTemp*10.0-0.5);                                          //Single Temperature
    else
        iSendData[1]=(dTemp*10.0+0.5);                                          //Single Temperature

    if(iSendData[0]<0)
    {
        iSendData[0]=0;
    }
    SendCommand(ATC_SET_SINGLE_TEMP);                                           //ATC 設定單點溫度
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetOffset(int iChCount, double *dOffset)
{
    A31_Temp_OFS->Text = dOffset[0];
    iDataCount = iChCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i]=9999;                                                      //Invalid data.
        if(Tri_Temp_Machine==1)                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
             if(i<=TriTemperature_TotalChannel)                                 //ATC 送Offset命令
             {
                if(dOffset[i]>=0)
                    iSendData[i] = (dOffset[i]*10+0.5);                         //Site 1~n Temperature Offset
                else
                    iSendData[i] = (dOffset[i]*10-0.5);                         //Site 1~n Temperature Offset
             }
        }
        else
        {
            if(i<iHandlerSiteCount)
            {
                //Eliot 2015_1123 start
                if(dOffset[i]>=0)
                    iSendData[i] = (dOffset[i]*10+0.5);                         //Site 1~n Temperature Offset
                else
                    iSendData[i] = (dOffset[i]*10-0.5);                         //Site 1~n Temperature Offset
                //Eliot 2015_1123 end
            }
        }
    }
    SendCommand(ATC_SET_TOFS);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetFFCOffset(int iChCount, double *dOffset)
{
    iDataCount=iChCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i]=9999;                                                      //Invalid data.
        if(i<iDataCount)
        {
            if(i==0)
            {
                iSendData[i]=dOffset[i];                                        //Test Arm 位置
            }
            else
            {
                if(dOffset[i]>=0)
                    iSendData[i]=(dOffset[i]*10+0.5);                           //Site 1~n Temperature Offset
                else
                    iSendData[i]=(dOffset[i]*10-0.5);                           //Site 1~n Temperature Offset
            }
        }
    }
    SendCommand(HANDLER_FFC_SETTING);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetSingleOffset(int iChannel, double Offset)
{
    A31_Single_Temp_OFS_Site->Text = iChannel;
    A31_Single_Temp_OFS->Text = Offset;
    iSendData[0] = iChannel;

    if(Offset>=0)
        iSendData[1] = (Offset*10+0.5);                                         //Single Temperature Offset
    else
        iSendData[1] = (Offset*10-0.5);                                         //Single Temperature Offset

    if(iSendData[0]<0)
    {
        iSendData[0] = 0;
    }
    SendCommand(ATC_SET_SINGLE_OFFSET);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::EnablesChannel(int iChCount, bool *Enabled)
{
    iDataCount = iChCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        ATC_Channel[i]->Checked = false;
        if(i<iChCount)
        {
            iSendData[i]=Enabled[i];                                            //Site 1~n Enabled
            ATC_Channel[i]->Checked = Enabled[i];
        }
    }

    if(iATC_MODE_TYPE==ATC_TYPE_50)                                             //Eliot 2015_1002
    {
        for(int i=0; i<(ATC_MAX_SITE/4); i++)
        {
            ATC_Module[i]=false;
            if(iSendData[0+i*4]==1 || iSendData[1+i*4]==1 ||
               iSendData[2+i*4]==1 || iSendData[3+i*4]==1 )
            {
                ATC_Module[i]=true;
            }
        }
    }
    else
    {
        SendCommand(ATC_SITE_ENABLED);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::StartTesting()
{
    iDataCount  =1;                                                             //Eliot 2016_0503
    iSendData[0]=1;                                                             //Start Test
    SendCommand(ATC_TEST_START);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::TestFinish()
{
    iDataCount  =1;                                                             //Eliot 2016_0503
    iSendData[0]=0;                                                             //Test Finish
    SendCommand(ATC_TEST_START);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SiteTesting(int iCount, bool *bTesting)     //Eliot 2016_0503
{
    iDataCount=iCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i]=false;                                                     //Invalid data.
        if(i<iCount)
        {
            iSendData[i]=bTesting[i];
        }
    }
    SendCommand(ATC_SITE_2ND_CHECK);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::HandlerArm(int iArm)                        //ATC6.0 ==> 0:Arm-1  1:Arm-2 2:Not Contact 3:NN mode
{                                                                               //ATC3.X ==> 0:Arm-1  1:Arm-2 -1:Not Contact 3:NN mode
    if(iArm==-1)                                                                //Steven 20240913 : fixed for ATC [1011]Handler Arm
    {
        if(iATC_MODE_TYPE==ATC_TYPE_60)                                         //Ifor20230720 add: QualComm ATC 6.0 需送2
        {
            iSendData[0]=2;
        }
        else
        {
            iSendData[0]=-1;
        }
    }
    else
    {
        if(bUseTwoArm32Site==true)
        {
            iSendData[0]=3;
        }
        else
        {
            iSendData[0]=iArm;
        }
    }
    SendCommand(ATC_HANDLER_ARM);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadTC(int iChCount, double *TempTC)
{
    if(iChCount<0 || iChCount>ATC_MAX_SITE)
    {
        iChCount=4;
    }
    for(int i=0; i<iChCount; i++)
        TempTC[i]=dTC[i];
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadTC2(int iChCount, double *TempTC2)
{
    if(iChCount<0 || iChCount>ATC_MAX_SITE)
    {
        iChCount=4;
    }
    for(int i=0; i<iChCount; i++)
        TempTC2[i]=dTC2[i];
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadTJ(int iChCount, double *TempTJ)
{
    if(iChCount<0 || iChCount>ATC_MAX_SITE)
    {
        iChCount=4;
    }
    for(int i=0; i<iChCount; i++)
        TempTJ[i]=dTJ[i];
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadPID(int *kP, double *kI, int *kD)
{
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetPID(int kP, double kI, int kD)
{
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_ConnectBtnClick(TObject *Sender)
{
    if(ATC_ConnectBtn->Down==true)
    {
        ATC_ConnectBtn->Caption     ="Disconnect";
        ATC_ConnectBtn->Font->Color =clRed;
        Connect();
    }
    else
    {
        ATC_ConnectBtn->Caption     ="Connect";
        ATC_ConnectBtn->Font->Color =clGreen;
        Disconnet();
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ShowATC_Page(int iType)
{
    PageControl->Visible = false;
    TabSheet20->TabVisible=false;
    TabSheet21->TabVisible=false;
    TabSheet30->TabVisible=false;
    TabSheet31->TabVisible=false;
    TabSheet50->TabVisible=false;
    TabSheet60->TabVisible=false;
    TabSheet70->TabVisible=false;
    switch(iType)
    {
        case ATC_TYPE_20:   TabSheet20->TabVisible=true;    break;
        case ATC_TYPE_21:   TabSheet21->TabVisible=true;    break;
        case ATC_TYPE_30:   TabSheet30->TabVisible=true;    break;
        case ATC_TYPE_31:   TabSheet31->TabVisible=true;    break;
        case ATC_TYPE_50:   TabSheet50->TabVisible=true;    break;
        case ATC_TYPE_51:   TabSheet51->TabVisible=true;    break;
        case ATC_TYPE_60:   TabSheet60->TabVisible=true;    break;
        case ATC_TYPE_70:   TabSheet70->TabVisible=true;    break;
    }

    TIniFile *ATCINIFile;                                                       //Steven 20160707 : 將ATC MODE存檔
    ATCINIFile=new TIniFile(asATCFilePath);
    ATCINIFile->WriteInteger("System", "iATC_MODE_TYPE", iType);
    delete ATCINIFile;

    PageControl->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadATCModeType()                           //Steven 20160707 : 將ATC MODE存檔
{
    TIniFile *ATCINIFile;
    ATCINIFile=new TIniFile(asATCFilePath);
    iATC_MODE_TYPE=ATCINIFile->ReadInteger("System", "iATC_MODE_TYPE", ATC_TYPE_UNSET);
    delete ATCINIFile;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetMaxTemperature(double dTemperature)
{
    iSendData[0]=(dTemperature*10+0.5);                                         //Max Temperature
    SendCommand(ATC_SET_MAX_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    RecordProcess("ATC Socket Error Code = "+IntToStr(ErrorCode));
    ErrorCode=0;
    IsConnectFlag=false;
    bConnecting=false;
    ATC_InterfaceForm->Caption = "OFF-Line";                                    //Eliot 2015_1130

    ATC_ConnectBtn->Down=false;
    ATC_ConnectBtn->Caption = "Connect";
    ATC_ConnectBtn->Font->Color = clGreen;
    for(int i=0; i<ATC_MAX_COMMAND; i++)                                        //Ifor 20160722 斷線時必須清除 Count 避免一連線就發出Alarm 訊息
    {
        iCommCount[i]=0;
        bCommFlag[i]=true;
    }
    Disconnet();                                                                //Ifor 20160825 add 避免 Socket Error 造成斷線 重新連線會被ATC踢出(ATC3.1 僅允許一個人連線)
    bReSendATCLotEven=true;                                                     //Ifor 20161121 ATC 斷線需重新送出 ATC Lot Even 給ATC
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::HasAlarmMsg()
{
    if(sAlarmMessageList->Count<1)                                              //Eliot 2016_0722
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TATC_InterfaceForm::GetAlarmMsg()
{
    AnsiString asMessage="", asTemp="";

    if(sAlarmMessageList->Count<1)                                              //Eliot 2016_0722
        return asMessage;

    AlarmMsg->Lines->Clear();
    asMessage=sAlarmMessageList->Strings[0];                                    //Eliot 2016_0722
    AlarmMsg->Lines->CommaText=asMessage;
    for(int i=0; i<8; i++)
    {
        asATC_ErrorMsg[i]=AlarmMsg->Lines->Strings[i];
    }
    LastMsg = sAlarmMessageList->Strings[0];
    sAlarmMessageList->Delete(0);
    return asMessage;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_GetAlarmMsgClick(TObject *Sender)
{
    AnsiString asMsg;
    if(HasAlarmMsg()==true)
    {
        asMsg=GetAlarmMsg();
    }
}
//---------------------------------------------------------------------------
double __fastcall TATC_InterfaceForm::GetSetTemp(int iChannel)                  //ChungHung 20150826 add
{
    if(iChannel<ATC_MAX_SITE)                                                   //kirn 20151102
    {
        return dNowATCSetTemp[iChannel];
    }
    else
    {
        return dNowATCSetTemp[0];
    }
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::CheckATCSetTemp(double dSettemp)            //wei 20230202 判斷ATC所有控制器設定溫度，一個溫度不一樣就要重新設定溫度
{
    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        if(i<ATC_MAX_SITE)
        {
            if(dNowATCSetTemp[i]!=dSettemp)
            {
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool   __fastcall TATC_InterfaceForm::GetRunState()                             //ChungHung 20150826 add
{
    return bATCRun;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetATC_ModuleCount(int iCount)              //for ATC 5.0 Eliot 2015_0930
{
    iATC_ModuleCount = iCount;
    if(iATC_ModuleCount<=0 || iATC_ModuleCount>8)
        iATC_ModuleCount=1;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetModuleRunStop(bool *Enabled)             //for ATC 5.0 Eliot 2015_0930
{                                                                               //設定那一個模組開啟或關閉
    for(int i=0; i<iATC_ModuleCount; i++)
        ATC_Module[i]=Enabled[i];
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Set2ndRange_P(int iCount, double *Range)    //Eliot 2015_0930
{
    iDataCount = iCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        if(i<iCount)
        {
            iSendData[i] = (Range[i]*10.0+0.5);
        }
    }
    SendCommand(ATC_SET_2ND_RANGE_P);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Set2ndRange_N(int iCount, double *Range)    //Eliot 2015_0930
{
    iDataCount = iCount;
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        if(i<iCount)
        {
            iSendData[i] = (Range[i]*10.0-0.5);                                 //Eliot 2015_1109
        }
    }
    SendCommand(ATC_SET_2ND_RANGE_N);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Set2ndTime(int iTime)                       //Eliot 2015_0930
{
    iSendData[0]=iTime;                                                         //unit : sec
    SendCommand(ATC_SET_2ND_TIME);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Set2ndFunction(bool bEnabled)               //Eliot 2015_1020
{
    iSendData[0]=bEnabled;                                                      //True:Use , False:No Use
    SendCommand(ATC_SET_2ND_ENABLED);
}
//---------------------------------------------------------------------------
int __fastcall TATC_InterfaceForm::GetRunSelfTestResult()                       //ChungHung 20160719 add
{
    return flagATCSelfTestResult;
}
//---------------------------------------------------------------------------
int __fastcall TATC_InterfaceForm::GetRunSelfTestStatus()                       //Ifor 20160824 add Get ATC Self Test Status
{
    return iATCSelfTestStatus;
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::GetATCLotStartStatus()                      //Ifor 20161121 回傳 ATC Lot Start 狀態
{
    return bATCLotStartStatus;
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::GetATCLotEndStatus()                        //Ifor 20161121 回傳 ATC Lot End 狀態
{
    return bATCLotEndStatus;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetRunSelfTest()                            //2016/7/17 Dell    Add
{
    iSendData[0]=1;
    flagATCSelfTestResult=0;
    iATCSelfTestStatus=0;                                                       //Ifor 20160825 add Self Status Flag
    SendCommand(ATC_RUN_SELFTEST);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetManualSelfTest()                         //Eliot 2016_0816
{
    iSendData[0]=1;
    flagATCSelfTestResult=0;
    iATCSelfTestStatus=0;                                                       //Ifor 20160825 add Self Status Flag
    SendCommand(ATC_MANUAL_SELFTEST);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::QuerySelfTestStatus()                       //Eliot 2016_0816
{
    iSendData[0]=1;
    //iATCSelfTestStatus=0;                                                     //Ifor 20160825 add Mark Self Status Flag 避免判斷異常
    SendCommand(ATC_SELFTEST_STATUS);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SendHandlerStatus()                         //Eliot 2016_0816
{
    iSendData[0]=1;
    SendCommand(HANDLER_STATUS);
}
//---------------------------------------------------------------------------

void __fastcall TATC_InterfaceForm::PID_SetClick(TObject *Sender)
{
    iDataCount = atoi(cBox_PID->Text.c_str());
    if(iDataCount>ATC_MAX_SITE)                                                 //kevin 20160507
        iDataCount=ATC_MAX_SITE;

    if(iDataCount<=0)
        iDataCount = 1;
    double dP, dI, dD;

    dP = atof(ed_P->Text.c_str());
    dI = atof(ed_I->Text.c_str());
    dD = atof(ed_D->Text.c_str());

    for(int i=0; i<iDataCount; i++)
    {
        iSendData[0+(i*3)] = (dP*10+0.5);
        iSendData[1+(i*3)] = (dI*10+0.5);
        iSendData[2+(i*3)] = (dD*10+0.5);
    }
    SendCommand(ATC_SET_PID);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::EMG_UP_DOWNClick(TObject *Sender)
{
    EMG_UpDown(EMG_UP_DOWN->Checked);                                           //0:EMG_Up  1:EMG_Down
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::UseTSD_Function(int iCount, int *iSiteOn)   //Steven 20160604 : by site TSD
{
    iDataCount=iCount;
    for(int i=0; i<iDataCount; i++)
        iSendData[i]=iSiteOn[i]?1:0;

    bGetATC_SEND_TEMP_READY=false;
    SendCommand(ATC_USE_TSD);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::UseTJ_MODE(int iChCount, int *iMode)        //Eliot 2015_1005
{
    for(int i=0; i<iChCount; i++)
    {
        iSendData[i] = iMode[i];
    }
    SendCommand(ATC_USE_TJ_MODE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::EMG_UpDown(bool bDown)                      //Eliot 2015_1005
{
    iSendData[0] = bDown;                                                       //False:EMG_Up  True:EMG_Down
    SendCommand(ATC_EMG_UP_DOWN);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::CheckFolderExist()
{
    AnsiString asFolderPath;
    Word tempSystemYear,tempSystemMonth,tempSystemDate;
    DecodeDate(Now(), tempSystemYear, tempSystemMonth, tempSystemDate);

    asFolderPath="D:\\ATC_Recoder";
    MyForceDirectories(asFolderPath);
    asFolderPath="D:\\ATC_Recoder\\CommLog";
    MyForceDirectories(asFolderPath);
    asFolderPath.sprintf("D:\\ATC_Recoder\\CommLog\\%04d_%02d", tempSystemYear, tempSystemMonth);
    MyForceDirectories(asFolderPath);
    asFolderPath.sprintf("D:\\ATC_Recoder\\CommLog\\%04d_%02d\\%04d_%02d_%02d", tempSystemYear, tempSystemMonth, tempSystemYear, tempSystemMonth, tempSystemDate);
    MyForceDirectories(asFolderPath);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SaveCommLog(bool bCloseFile, TMemo *CommMemo)
{
    if(bCloseFile==true && bOpenFlag==false)
    {
        CommMemo->Lines->Clear();
        return;
    }

    int iDataSize;
    CheckFolderExist();
    AnsiString asFileName;
    Word tempSystemYear,tempSystemMonth,tempSystemDate;
    Word tempSystemHour,tempSystemMin,tempSystemSec,tempSystemMSec;

    if(COMM_Log==NULL || bOpenFlag==false)
    {
        DecodeDate(Now(), tempSystemYear, tempSystemMonth, tempSystemDate);
        DecodeTime(Now(), tempSystemHour, tempSystemMin, tempSystemSec, tempSystemMSec);
        asFileName.sprintf("D:\\ATC_Recoder\\CommLog\\%04d_%02d\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d_%02d.TXT",
                            tempSystemYear, tempSystemMonth,
                            tempSystemYear, tempSystemMonth, tempSystemDate,
                            tempSystemYear, tempSystemMonth, tempSystemDate, tempSystemHour, tempSystemMin);
        COMM_Log=fopen(asFileName.c_str(), "a+");

        if(COMM_Log!=NULL)
            bOpenFlag=true;
        else
            RecordProcess("fopen error, TATC_InterfaceForm::SaveCommLog");      //JerryYang 20250505 : add log
    }

    if(COMM_Log!=NULL)
    {
        for(int i=0; i<CommMemo->Lines->Count; i++)
        {
            fputs(CommMemo->Lines->Strings[i].c_str(), COMM_Log);
            fputs("\n", COMM_Log);
        }
        CommMemo->Lines->Clear();
        fseek(COMM_Log, 0, SEEK_END);
        iDataSize=ftell(COMM_Log);
        if(iDataSize>=dCommLogFileSize || bCloseFile==true)
        {
            bOpenFlag=fclose(COMM_Log);
            if(bOpenFlag!=0)
            {
                RecordProcess("fclose error, TATC_InterfaceForm::SaveCommLog"); //JerryYang 20250505 : add log
            }
        }
    }
    else
    {
//        bOpenFlag = fclose(COMM_Log);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::cBox_RecordCommClick(TObject *Sender)
{
    SaveCommLog(!cBox_RecordComm->Checked,A31_Memo);                            //Eliot 2015_1008
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetSW_Ver()                                 //Eliot 2015_1015
{
    bGetATC_SW_VerFlag=false;                                                   //Eliot 2016_0503
    iSendData[0] = 1;
    SendCommand(ATC_SW_VER);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetFW_Ver()                                 //Eliot 2015_1015
{
    iSendData[0] = 1;
    SendCommand(ATC_FW_VER);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SecondSensorFunctionClick(TObject *Sender)
{
    Set2ndFunction(SecondSensorFunction->Down);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::CommFlagTimerTimer(TObject *Sender)
{
    static bool bRunTimer=false;
    if(InitialOK==false || bRunTimer==true)
        return;

    bRunTimer=true;
    AnsiString asAlarmMes="";
    for(int i=0; i<ATC_MAX_COMMAND; i++)                                        //Eliot 2015_1027
    {
        if(bCommFlag[i]==false && bHandlerOnlineFlag==true)                     //Ifor 20160926 修改New ATC未連線時清除計數避免發生例外Alarm
        {
            iCommCount[i]++;
            if(iCommCount[i]>=10)                                               //2016/7/17 Dell    modify
            {                                                                   //Ifor 20160926 修改未收到New ATC 命令10次 將溫度寫成9999
                if(i==(ATC_READ_TEMP-1000) || i==(ATC_READ_TEMP_2-1000))
                {
                    iCommCount[i]=0;
                    for(int j=0; j<ATC_MAX_SITE; j++)
                    {
                        dTC[j]=9999;
                        dTC2[j]=9999;
                        dTJ[j]=9999;
                        ChTempTC[j]->Caption = dTC[j];
                        ChTempTC2[j]->Caption = dTC2[j];
                        ChTempTJ[j]->Caption = dTJ[j];
                    }

                    if(i==(ATC_READ_TEMP-1000))
                        bCommFlag[ATC_READ_TEMP-1000]=true;
                    else if(i==(ATC_READ_TEMP_2-1000))
                        bCommFlag[ATC_READ_TEMP_2-1000]=true;
                }
            }
        }
        else
        {
            iCommCount[i]=0;
        }
    }

    bRunTimer=false;
}
//---------------------------------------------------------------------------
double __fastcall TATC_InterfaceForm::GetSetOffset(int iChannel)                //Eliot 2015_1124
{
    if(iChannel<ATC_MAX_SITE) //kirn 20151102
    {
        return dNowATCSetOffset[iChannel];
    }
    else
    {
        return dNowATCSetOffset[0];
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetNowRecipeFile()                          //Eliot 2015_1027
{
    bGetNowRecipeFlag=false;
//    ATC_Now_RecipeFile->Caption="";                                           //Ifor 20190308 : Mark 清掉會造成Handler 重新設定工作檔
    iSendData[0]=1;
    SendCommand(ATC_GET_NOW_RECIPE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetRecipeFileList()                         //Eliot 2015_1027
{
    bGetRecipeListFlag=false;
    ATC_RecipeFile->Items->Clear();
    ATC_RecipeFile->Text="";
    iSendData[0]=1;
    SendCommand(ATC_GET_RECIPE_LIST);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::btnGetNowRecipeFileClick(TObject *Sender)
{
    GetNowRecipeFile();
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::btnGetRecipeFileListClick(TObject *Sender)
{
    GetRecipeFileList();
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetHandlerVersion(AnsiString asVersion)     //Eliot 2015_1126
{
    bGetControllerFW_Flag=false;                                                //Ifor 20170230 Get ATC power supply firmware number
    asHandlerVer = asVersion;
    SendCommand(ATC_SW_VER);                                                    //沒用到
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::AutoCheckList()
{
    if(A31_Temperature->Hint!=A31_Temperature->Text)
    {
        A31_Temperature->Hint=A31_Temperature->Text;
        A31_Set_TempClick(this);
    }

    if(A31_Temp_OFS->Hint!=A31_Temp_OFS->Text)
    {
        A31_Temp_OFS->Hint=A31_Temp_OFS->Text;
        A31_Set_Temp_OFSClick(this);
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetChillerTemp(double dTemp)                //kevin 20160425
{
    if(dTemp<0)                                                                 //Ifor 20220210 : Fix 負溫設定異常問題
    {
        iSendData[0] = dTemp*10.0-0.5;
    }
    else
    {
        iSendData[0] = dTemp*10.0+0.5;
    }

    SendCommand(ATC_SET_CHILLER_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::BtnSetTSDClick(TObject *Sender)
{
    int UseTSD[4];
    UseTSD[0] = cBox_CH1->Checked;
    UseTSD[1] = cBox_CH2->Checked;
    UseTSD[2] = cBox_CH3->Checked;
    UseTSD[3] = cBox_CH4->Checked;

    UseTSD_Function(4, UseTSD);                                                 //Eliot 2016_0802
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTempReadMode(int iChCount, int *iMode)   //2016-02-01    Dell    Add     SetTempReadMode     (Mode:0 Tc  1:Tj)
{
    iDataCount=iChCount;
    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        if(i<iChCount)
        {
            iSendData[i] = iMode[i];                                            //Site 1~n Enabled
        }
    }
    SendCommand(ATC_SET_TRM);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTJOffset(int iChCount, double *dOffset)
{
    iDataCount = iChCount;
    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        if(i<iHandlerSiteCount)
        {
            if(dOffset[i]>=0)                                                   //Eliot 2015_1123
                iSendData[i] = dOffset[i];                                      //Site 1~n Temperature Offset
            else
                iSendData[i] = dOffset[i];                                      //Site 1~n Temperature Offset

            iSendData[i] = iSendData[i]*10;                                     // 2016.07.04 , Joye , AMD Tj Offset
        }
    }
    SendCommand(ATC_SET_TJ_OFFSET);                                             // 2016.07.01 , Joye , AMD Tj Offset
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SpeedButton1Click(TObject *Sender)
{
    SetRunSelfTest();
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetATC_2ND_Status()                         //Eliot 2016_0802
{
    iSendData[0] = 1;
    SendCommand(ATC_GET_2ND_STATUS);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SpeedButton2Click(TObject *Sender)
{
    SetManualSelfTest();
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::LotStart(String sLotID)                     //Eliot 2016_1020
{
    asLotID = sLotID;
    SendCommand(ATC_LOT_START);
    A31_LotID->Text = sLotID;
    bATCLotStartStatus=false;                                                   //Ifor 20170124 (Steven) : ATC3.1送出Lot Start 需清除旗標
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::LotEnd(String sLotID)                       //Eliot 2016_1020
{
    asLotID = sLotID;
    SendCommand(ATC_LOT_END);
    A31_LotID->Text = sLotID;
    bATCLotEndStatus=false;                                                     //Ifor 20170124 (Steven) : ATC3.1送出 Lot End 需清除旗標
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::QuerySelfTestResult(int iChCount)           //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
{
    iSendData[0]=iChCount;
    SendCommand(ATC_SELFTEST_RESULT);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::btnGetSelfTestResultClick(
      TObject *Sender)
{
    QuerySelfTestResult(iHandlerSiteCount);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SendAbnormalSite(int iChCount, int *AbnormalSite)   //Eliot 20170116 Handler傳送有異常的Site給ATC
{
    iDataCount=iChCount;
    for(int i=0; i<iDataCount; i++)
    {
        iSendData[i] = AbnormalSite[i];
    }
    SendCommand(HANDLER_ABNORMAL_SITE);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SpeedButton3Click(TObject *Sender)
{
    int iAbnormalSite[1];
    iAbnormalSite[0]=atoi(Edit3->Text.c_str());
    SendAbnormalSite(1, iAbnormalSite);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetControllerSN(int iChCount)               //Eliot 20170221 Get ATC power supply serial number
{                                                                               //iChCount : Get how many serial number data count.
    bGetControllerSN_Flag=false;
    iSendData[0] = iChCount;
    SendCommand(ATC_GET_SN);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SpeedButton4Click(TObject *Sender)
{
    GetControllerSN(iHandlerSiteCount);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::btnFFCTriggerClick(TObject *Sender)
{
    static bool bChange=true;

    bool bEnabled[16];

    for(int i=0; i<16; i++)
    {
        bEnabled[i]=false;                                                      //Invalid data.
        if(i<16)
        {
            if(bChange)
                bEnabled[i] = ATC_Channel[i]->Checked;
            else
                bEnabled[i] = false;
        }
    }
    ChannelFFCTrigger(16, 0, bEnabled);
    if(bChange)
    {
        bChange=false;
        btnFFCTrigger->Caption="Trigger On";
    }
    else
    {
        bChange=true;
        btnFFCTrigger->Caption="Trigger Off";
    }
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ChannelFFCTrigger(int iChCount, int Arm, bool Enabled)
{
    for(int i=0; i<iChCount; i++)                                               //iChCount =16
    {
        if(Enabled)
        {
            if(i<iChCount)
            {
                if(Arm==0)
                {
                    if(i>=0 && i<=7)
                    {
                        if(ATC_Channel[i]->Checked)
                        {
                            iSendData[i] = 1;
                        }
                        else
                        {
                            iSendData[i] = 0;
                        }
                    }
                    else
                    {
                        iSendData[i] = 0;
                    }
                }
                else
                {
                    if(i>=8 && i<=16)
                    {
                        if(ATC_Channel[i]->Checked)
                        {
                            iSendData[i] = 1;
                        }
                        else
                        {
                            iSendData[i] = 0;
                        }
                    }
                    else
                    {
                        iSendData[i] = 0;
                    }
                }
            }
        }
        else
        {
             iSendData[i] = 0;
        }
    }
    SendCommand(HANDLER_FFC_TRIGGER);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FFCOffsetEnable( bool Enabled)
{
    iSendData[0] = 1;
    if(Enabled)
    {
        iSendData[1] = 1;
    }
    else
    {
        iSendData[1] = 0;
    }

    SendCommand(HANDLER_FFC_ENABLED);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::btnFFCEnableClick(TObject *Sender)
{
    static bool bChange=true;
    if(bChange)
    {
        bChange=false;
        btnFFCEnable->Caption = "Enable";
        FFCOffsetEnable(true);
    }
    else
    {
        bChange=true;
        btnFFCEnable->Caption = "Disable";
        FFCOffsetEnable(false);
    }
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::AskATCDateToHandle()
{
    if(Temperature.bATCActiveCooling==false)                                    //kevin 20221011
        return;

    iSendData[0] =1;
    //SendCommand(ATC_CHILLER_STATUS);
    bGetATC_SW_VerFlag=false;                                                   //Eliot 2016_0503
    SendCommand(ATC_SW_VER);                                                    //CC_ASE_KaohSiung
    iSendData[0] = 8;                                                           //kevin 20200702
    SendCommand(ATC_READ_PID);                                                  //kevin 20200409 add
    SendCommand(ATC_READ_OFFSET);                                               //kevin 20200409 add

    if((ATC_SYSTEM==eNewATCSystem &&
       (iATC_MODE_TYPE==ATC_TYPE_60 ||
        iATC_MODE_TYPE==ATC_TYPE_70)))                                          //kevin 20221011 ATC 6.0 沒有資料無法回
    {
    }
    else
    {
        if(iATC_MODE_TYPE==ATC_TYPE_31 ||
           iATC_MODE_TYPE==ATC_TYPE_32 ||
           IS_ATC33())
        {
            SendCommand(ATC_READ_WATER_VALVE);                                  //kevin 20221028 add    水閥開度 ATC 6.0 7.0 沒有
        }
        SendCommand(ATC_CHILLER_STATUS);                                        //kevin 20221011 add 讀取 ATC 資料給 eKeep 判斷
        SendCommand(ATC_GET_2ND_STATUS);                                        //kevin 20221011 add 讀取 ATC 資料給 eKeep 判斷
        SendCommand(ATC_CMD_READ_FUNCTION_STATUS);                              //kevin 20221011 add 讀取 ATC 資料給 eKeep 判斷
    }
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_TemperatureClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, dTempMax, dTempMin);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::A31_Temp_OFSClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, (double)InputLimit.iTempHigh, (double)InputLimit.iTempLow);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ed_PClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 99999, 0);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetChillerStatus()                          //Ifor 20230515 add:取得Chiller 狀態
{
    iSendData[0] =1;
    SendCommand(ATC_CHILLER_STATUS);
}
//---------------------------------------------------------------------------
//ben 20240927 : add CCD Bar Code to ATC
//==>
void __fastcall TATC_InterfaceForm::SendHandler2DID(int iArm, bool bStartRecord, AnsiString as2DData, int iSiteCount)
{
    if(ATC_SYSTEM==eNewATCSystem &&
       Temperature.bATCActiveCooling==true &&
       TestIF_File.bEnableBarCode)
    {
        if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32 ||
           ATC_InterfaceForm->IS_ATC33())
        {
            iSendData[0] = 1;
            iSendData[1] = -1;

            if(bStartRecord==true)
            {
                iSendData[0] = iSiteCount+1;
                iSendData[1] = iArm;
                asHandler2DID =as2DData;
            }
            SendCommand(HANDLER_2DID);
        }
    }
}
//---------------------------------------------------------------------------
//<==
//ben 20240927 : add CCD Bar Code to ATC
//Ztex 2023.04.19 Add HT-1032 TriTemp Function
//Add ATC Refrigerator Status
void __fastcall TATC_InterfaceForm::SendSwitchRefrigerator(int iRefrigeratorNo, int iRefrigeratorOnOff)
{
    iDataCount = Total_Compressor;
    iATC_RefrigeratorUserMode[1][iRefrigeratorNo] = iRefrigeratorOnOff;         //Hmy 20190614 Mark and Move pos
    for(int i=0;i<iDataCount;i++)
    {
        iSendData[i] = iATC_RefrigeratorUserMode[1][i];
    }

    SendCommand(ATC_51_SET_REF_ENABLED);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Get_ATCRefrigeratorAllStatus(bool bAllStatus)  //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
{
    if(bAllStatus ==false)
    {
        bReadRefrigerantMode_AllStatus = false;                                 //ATC 收/發壓縮機狀態命令
        iSendData[0] = 1;
        SendCommand(ATC_51_GET_REF_STATUS);
    }
    else
    {
        bReadRefrigerantMode_AllStatus = true;
        iSendData[0] = Total_Compressor;
        asRefrigeratorStatus = "ALL";
        SendCommand(ATC_51_GET_REF_STATUS);
    }
}
//---------------------------------------------------------------------------  //Add ATC Refrigerator Status
int  __fastcall TATC_InterfaceForm::Get_ATCRefrigeratorAtcSetMode(int iRefrigeratorNum, int *iTempRefrigeratorUserMode)
{
    int iReturnStatus =false;
    if(iRefrigeratorNum<0 || iRefrigeratorNum>Total_Compressor)                 //ATC 收/發壓縮機狀態命令
    {
        iRefrigeratorNum = 8;
    }

    if(iATC_RefrigeratorUserMode[0][Total_Compressor] !=2)
    {
        iReturnStatus =iATC_RefrigeratorUserMode[0][Total_Compressor];
        return iReturnStatus;
    }
    for(int i=0;i<iRefrigeratorNum+1;i++)
    {
        iTempRefrigeratorUserMode[i] = iATC_RefrigeratorUserMode[0][i];
    }
    iReturnStatus =1;
    return iReturnStatus;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_AtcDefrostStatus(int iOpenClose)   //Modify //Add DEFROST Command
{
    iSendData[0] = iOpenClose;                                                  //ATC 除霜命令
    SendCommand(ATC_51_SET_DEFROST);
}
//Add ATC Refrigerator Status
//---------------------------------------------------------------------------
//add air Machine ->
void __fastcall TATC_InterfaceForm::SendAirMachineStatus(int iEnableAirMachine,double dSetSocketTempature,double dSetIndexTempature)
{
    bool bUse=false;

    if(iATC_MODE_TYPE==61)
    {
        bUse = Temperature.bEnableArm_1_Air || Temperature.bEnableArm_2_Air || Temperature.bEnableSocket_Air;
        if(bCloseAirMachine==true)
            bUse=false;

        if(IsConnect()==true)
        {
            iSendData[0]=bUse;
            iSendData[1]=Temperature.bEnableArm_1_Air;
            iSendData[2]=Temperature.bEnableArm_2_Air;
            iSendData[3]=Temperature.bEnableSocket_Air;
            if(bDefrostKeepATCTemp==true)
            {
                if(iEnableAirMachine==1)
                {
                    iSendData[1]=true;
                    iSendData[2]=true;
                    iSendData[3]=true;
                }
                else
                {
                    iSendData[1]=false;
                    iSendData[2]=false;
                    iSendData[3]=false;
                }
            }
            else
            {
                if(iEnableAirMachine==0)
                {
                    iSendData[1]=false;
                    iSendData[2]=false;
                    iSendData[3]=false;
                }
            }

            iSendData[4]=dSetIndexTempature;
            iSendData[5]=dSetSocketTempature;
            SendCommand(ATC_AIRMACHINE_STATUS);
            Send_Rec_AirMachineStatus.iH2A_SendStatus=1;
            return;                                                                 //設定/讀取AirStream溫度
        }
        Send_Rec_AirMachineStatus.iH2A_SendStatus=2;
        Send_Rec_AirMachineStatus.iA2H_RecStatus=2;
    }
    else
    {
        if(IsConnect()==true)
        {
            iSendData[0]=iEnableAirMachine;
            iSendData[1]=dSetSocketTempature;//Modify(add Handler Set Air Machine Temp)
            SendCommand(ATC_AIRMACHINE_STATUS);
            Send_Rec_AirMachineStatus.iH2A_SendStatus=1;
            return;                                                                 //設定/讀取AirStream溫度
        }
        Send_Rec_AirMachineStatus.iH2A_SendStatus=2;
        Send_Rec_AirMachineStatus.iA2H_RecStatus=2;
    }
}
//add air machine <-
//-----------------------------------------------------------------------------
void TATC_InterfaceForm::Check_ATC_Busy_State()                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
{
    if(ATC_InterfaceForm->bATC_ModuleState[TriTemperature_TotalChannel]==true)
    {
        bATCBusy = true;                             //檢查ATC 忙碌狀態
    }
    else
    {
        bATCBusy = false;
    }
}
//-----------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_ATC_DewPoint(double dDewPoint_Index,double dDewPoint_InSht,double dDewPointOutSht)
{
    iSendData[0]=dDewPoint_Index*10;
    iSendData[1]=dDewPoint_InSht*10;
    iSendData[2]=dDewPointOutSht*10;
    SendCommand(ATC_GETDEWPOINTTEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SendEnableAtcAtcFreonRecover(int iOpenClose)//Hmy 20170920 Add  Atc 5.1 冷媒回收功能
{
    iSendData[0] = iOpenClose;
    SendCommand(ATC_51_FREONRECOVER);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_ATCSETTJENABLED(int iOpenClose)        //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)
{
    iSendData[0] = iOpenClose;
    SendCommand(ATC_SET_TJ_ENABLED);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_ATCSETTJPARAMETER(AnsiString asSlope, AnsiString asOffset)   //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)
{
    asSendData[0] = asSlope;
    asSendData[1] = asOffset;
    SendCommand(ATC_SET_TJ_PARAMETER);
}
//-----------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_HANDLERTESTSITEMAPPING(int iChCount, int *iSiteMap)   //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)  //JerryYang 20231128 : 傳送ATC site map
{
    iDataCount=iChCount;
    for(int i=0; i<ATC_MAX_SITE/2; i++)
    {
        iSendData[i]=0;
        if(i<iChCount)
        {
            iSendData[i]=iSiteMap[i];                                           //Site 1~n Enabled
        }
    }
    SendCommand(HANDLER_TEST_SITEMAPPING);
}
//-------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetAllTemp_List(double *dTemperature)       //Ztex 2024.08.24 Add Tri Temp Set All Temp By List
{
    if(Tri_Temp_Machine==1)
        iDataCount=TriTemperature_TotalChannel;
    else
        iDataCount=iHandlerSiteCount;

    if(iDataCount>ATC_MAX_SITE)
        iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;
        if(bUT150Install[TriTemp_Ch[i]]==false && bUT150HasUse[TriTemp_Ch[i]]==false)
            dTemperature[i]=25.0;

        if(i<iDataCount)
        {
            if(dTemperature[i]<0)
                iSendData[i] = (dTemperature[i]*10.0-0.5);
            else
                iSendData[i] = (dTemperature[i]*10.0+0.5);
        }
    }
    SendCommand(ATC_SET_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_ATCWATCHDOG(bool bTjWatchdog, double dDelaySOT, double dInputVLow, double dInputVHigh, double dContinueSec)   //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)  //JerryYang 20231205 : ATC Tj WatchDog  //JerryYang 20251201 : ATC連動watchdog新增continue欄位
{
    if(bTjWatchdog)
        asSendData[0]=1;
    else
        asSendData[0]=0;

    asSendData[1]=FloatToStr(dDelaySOT);
    asSendData[2]=FloatToStr(dInputVLow);
    asSendData[3]=FloatToStr(dInputVHigh);
    asSendData[4]=FloatToStr(dContinueSec);  //JerryYang 20251201 : ATC連動watchdog新增continue欄位

    SendCommand(ATC_51_SET_TJ_WATCHDOG);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Set_ASIF_TJ_EFUSED(AnsiString Str)          //Steven 20240903 : for MTK ASIF data
{                                                                               //@1112,3,ASIF_TJ_EFUSED,"EFUSE_DATA1:[1:99999,2:99999,3:99999]","EFUSE_DATA2:[1:99999,2:99999,3:99999]"#
    AnsiString StBuffer=Str;
    StBuffer=StringReplace(StBuffer, "\"", "", TReplaceFlags()<<rfReplaceAll);
    int iLeft =Str.AnsiPos("{")+1;
    int iRight=Str.Length();
    StBuffer=StBuffer.SubString(iLeft, iRight);
    StBuffer=StringReplace(StBuffer, "}", "", TReplaceFlags()<<rfReplaceAll);
    StBuffer=StringReplace(StBuffer, "],", "]\",\"", TReplaceFlags()<<rfReplaceAll);
    StBuffer=AnsiString("\"")+StBuffer+AnsiString("\"");

    TStringList *List=new TStringList();
    List->CommaText=StBuffer;

    iDataCount=List->Count+1;
    asSendData[0]="ASIF_TJ_EFUSED";
    for(int i=0; i<List->Count; i++)
    {
        asSendData[i+1]=List->Strings[i];
    }
    SendCommand(ASIF_TJ_EFUSED);

    List->Clear();
    delete List;
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Get_ASIF_TJ_REQUEST()                       //Steven 20240903 : for MTK ASIF data
{
    SendCommand(ASIF_TJ_REQUEST);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Get_ASIF_TJ_FB()                            //Steven 20240903 : for MTK ASIF data
{
    SendCommand(ASIF_TJ_FB);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTC2Offset(int iChCount, double *dOffset) //KenHsieh 20240311 : add Tc2 Offset
{
    iDataCount = iChCount;
    if(iDataCount>ATC_MAX_SITE)      //kevin 20160507
       iDataCount=ATC_MAX_SITE;

    for(int i=0; i<ATC_MAX_SITE; i++)
    {
        iSendData[i] = 9999;    //Invalid data.
        if(i<iHandlerSiteCount)
        {
            //Eliot 2015_1123 start
            if(dOffset[i]>=0)
                iSendData[i] = (dOffset[i]*10+0.5); //Site 1~n Temperature Offset
            else
                iSendData[i] = (dOffset[i]*10-0.5); //Site 1~n Temperature Offset
            //Eliot 2015_1123 end
        }
    }
    SendCommand(ATC_SET_T2OFS);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::Send_ATCSetPFParameter(int iOnOff, int iFullPower, AnsiString asPFSlope, AnsiString asWGain, int iMany2one)  //Hmy 20240207 Add Send Pf Par to Atc
{
    iSendData[0] =iOnOff;
    iSendData[1] =iFullPower;
    asSendData_PF[0]=asPFSlope;
    asSendData_PF[1]=asWGain;
    iSendData[2] =iMany2one;
    SendCommand(ATC_SET_PF_PARAMETER);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::PFMode(bool bOnOff) //Power Following Control ENABLED       //Eliot 20190916
{
    int i=0;

    iDataCount=iATC_Use_Heat_Count;
    if(bOnOff==1 &&
       (IndexStatus==Z1Down_Z2Up || IndexStatus==Z1Up_Z2Down || IndexStatus==Z1_Z2_Down ||
        iGPIBIndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1_Z2_Down))
    {
        for(i=0; i<iDataCount; i++)
        {
            if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
            {
                if(i>=iDataCount/2)
                    iSendData[i]=0;
                else
                    iSendData[i]=bATC_EnablesChannel[i]?1:0;
            }
            else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
            {
                if(i<iDataCount/2)
                    iSendData[i]=0;
                else
                    iSendData[i]=bATC_EnablesChannel[i]?1:0;
            }
            else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
            {
                iSendData[i]=bATC_EnablesChannel[i]?1:0;
            }
        }
    }
    else
    {
        for(i=0; i<iDataCount; i++)
        {
            iSendData[i]=0;
        }
    }

    SendCommand(HANDLER_PFC_ENABLED);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATCCONTROLMODEMode(AnsiString asSetData) //Power Following Control ENABLED       //Eliot 20190916
{
    int i=0, iLen=0;
    AnsiString asOnOff="";
    iDataCount=iATC_Use_Heat_Count;
    int iSiteMapping_QualSite[]    ={0,2,1,3,4,6,5,7};
    int iMap;
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;

    for(i=0; i<iDataCount; i++)
    {
        iSendData[i]=0;
    }

    if(asSetData.Pos(1)!=0)
    {
        bTJControlMode=true;
    }
    else
    {
        bTJControlMode=false;
        bTJtoTCWait=true;                                                       //wei 20251002 : TJ轉回TC等待
    }

    if(TestIF.iTestMode==SingleSite && Temperature.bMultiZoneEnable)
    {
        for(int i=0; i<4; i++)
        {
            asOnOff="";
            iMap=0;
            iLen=asSetData.Pos(",");
            if(iLen==0)
            {
                iLen=asSetData.Pos(";");
            }

            if(iLen==0)
            {
                asOnOff="0";
            }
            else
            {
                asOnOff=asSetData.SubString(1, iLen-1);
                asSetData.Delete(1, iLen);
            }

            if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
            {
                iMap=i;
            }
            else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
            {
                iMap=i+iATC_Use_Heat_Count/2;
            }

            if(asOnOff=="1")
                iSendData[iMap]=1;
            else
                iSendData[iMap]=0;
        }
    }
    else
    {
        for(int k=0; k<iRow*iCol; k++)
        {
            for(int i=0; i<iRow; i++)
            {
                for(int j=0; j<iCol; j++)
                {
                    if(TestIF.iSiteMap[i][j]==k+1)
                    {
                        asOnOff="";
                        iMap=0;
                        iLen=asSetData.Pos(",");
                        if(iLen==0)
                        {
                            iLen=asSetData.Pos(";");
                        }
                        asOnOff=asSetData.SubString(1, iLen-1);
                        asSetData.Delete(1, iLen);

                        if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
                        {
                            if(iRow==2)
                                iMap=iSiteMapping_QualSite[i*iRow+j];
                            else
                                iMap=i+j;
                        }
                        else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
                        {
                            if(iRow==2)
                                iMap=iSiteMapping_QualSite[i*iRow+j+iATC_Use_Heat_Count/2];
                            else
                                iMap=i+j+iATC_Use_Heat_Count/2;
                        }
                        else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
                        {
                            if(i==0)
                                iMap=i+j+iATC_Use_Heat_Count/2;
                            else
                                iMap=i+j-1;
                        }

                        if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
                        {
                            if(asOnOff=="1" &&
                               (i==1 && LastSet.bUseTestSocket[0][0][j] ||
                                i==0 && LastSet.bUseTestSocket[1][0][j]))
                            {
                                iSendData[iMap]=1;
                            }
                            else
                            {
                                iSendData[iMap]=0;
                            }
                        }
                        else
                        {
                            if(asOnOff=="1" && LastSet.bUseTestSocket[0][i][j])
                            {
                                iSendData[iMap]=1;
                            }
                            else
                            {
                                iSendData[iMap]=0;
                            }
                        }
                    }
                }
            }
        }
    }

    if(iATC_MODE_TYPE!=ATC_TYPE_60 && iATC_MODE_TYPE!=ATC_TYPE_70)              //Steven 20260415 : ATC 6.0/7.0 unsupported (1038/1067)
    {
        SendCommand(ATC_USE_TJ_MODE);
        SendCommand(ATC_GET_CONTROL_MODE);
    }
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetPFCPARAMETER() //讀取單一溫度的PF參數值
{
    SendCommand(ATC_GET_PFC_PARAMETER);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetPFCPARAMETER(AnsiString asSetData)    //設定單一溫度的PF參數值
{
    asSetPFCData=StringReplace(asSetData, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
    SendCommand(ATC_SET_PFC_PARAMETER);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetSLOPEOFFSET() //Get the Slope/Offset value   //Eliot 20210412
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1084)
    SendCommand(ATC_GET_SLOPE_OFFSET);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetSLOPEOFFSET(AnsiString asSetData)    //Set the Slope/Offset value    //Eliot 20210412
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1085)
    asSetPFCData=StringReplace(asSetData, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
    SendCommand(ATC_SET_SLOPE_OFFSET);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GetVOLTAGE() //Get the Slope/Offset value   //Eliot 20210412
{
    SendCommand(ATC_FEEDBACK_VOLTAGE);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::RECORDTJTEMP(int iStart)  //Record Tj Temp(Max/Min/Avg)//Eliot 20220411
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1087)
    iRecordTjTemp=iStart;
    if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
    {
        iSendData[0]=0;
    }
    else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
    {
        iSendData[0]=1;
    }
    else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
    {
        iSendData[0]=2;
    }
    else
    {
        iSendData[0]=-1;
    }
//    iSendData[0]=0;
    SendCommand(ATC_RECORD_TJ_TEMP);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::QUERYTJTEMP()               //Record Tj Temp(Max/Min/Avg)//Eliot 20220411
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1088)
    SendCommand(ATC_QUERY_TJ_TEMP);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATCGETCONTROLMODE()
{
    SendCommand(ATC_GET_CONTROL_MODE);
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::TesterChangeTemp(int iTemp)     //wei 20230525
{
    int iSiteMapping_QualSite[]    ={0,2,1,3,4,6,5,7};
    int iMap;
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;

    for(int k=0; k<iRow*iCol; k++)
    {
        for(int i=0; i<iRow; i++)
        {
            for(int j=0; j<iCol; j++)
            {
                if(TestIF.iSiteMap[i][j]==k+1)
                {
                    iMap=0;

                    if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
                    {
                        if(iRow==2)
                            iMap=iSiteMapping_QualSite[i*iRow+j];
                        else
                            iMap=i+j;
                    }
                    else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
                    {
                        if(iRow==2)
                            iMap=iSiteMapping_QualSite[i*iRow+j+iATC_Use_Heat_Count/2];
                        else
                            iMap=i+j+iATC_Use_Heat_Count/2;
                    }
                    else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
                    {
                        if(i==0)
                            iMap=i+j+iATC_Use_Heat_Count/2;
                        else
                            iMap=i+j-1;
                    }

                    ATC_InterfaceForm->SetSingleTemp(iMap,iTemp);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTempPID(double dTemp, bool bChange, double dTemp1)
{
/*    AnsiString asFileName,asFileNameOld,asFileNameNew;
//    int iLen=0;
    int iSiteMapping_QualSite[]    ={0,2,1,3,4,6,5,7};
    int iMap;
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;
    iDataCount=iATC_Use_Heat_Count;

//    asFileName="BGA1957-25CTC-BORAS-TMTB53-6-ENG";
    fMain->TSMCFileName();
//    iLen=asTSMCFileName[1].Pos("C");
    asFileNameOld=asTSMCFileName[1].c_str();
//    asFileName=AnsiString(dTemp)+asFileNameOld.SubString(iLen,asTSMCFileName[1].Length());
    asFileName=asFileNameOld;
    for(int i=0; i<9; i++)
    {
        if(i==1)
        {
            asFileNameNew+=asFileName;
            asFileNameNew+="-";
        }
        else
        {
            if(asTSMCFileName[i].Length()!=0)
            {
                asFileNameNew+=asTSMCFileName[i];
                asFileNameNew+="-";
            }

            if(i==8)
            {
                asFileNameNew.Delete(asFileNameNew.Length(), 1);
            }
        }
    }

    if(bChange)
        asFileNameNew=asFileNameNew+"-"+AnsiString(dTemp1);
//    asFileNameNew=asFileNameNew;
    asChangeFile=asFileNameNew.c_str();
    iSendData[1] = dTemp;

    for(int k=0; k<iRow*iCol; k++)
    {
        for(int i=0; i<iRow; i++)
        {
            for(int j=0; j<iCol; j++)
            {
                if(TestIF.iSiteMap[i][j]==k+1)
                {
                    iMap=0;

                    if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
                    {
                        if(iRow==2)
                            iMap=iSiteMapping_QualSite[i*iRow+j];
                        else
                            iMap=i+j;
                    }
                    else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
                    {
                        if(iRow==2)
                            iMap=iSiteMapping_QualSite[i*iRow+j+iATC_Use_Heat_Count/2];
                        else
                            iMap=i+j+iATC_Use_Heat_Count/2;
                    }
                    else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
                    {
                        if(i==0)
                            iMap=i+j+iATC_Use_Heat_Count/2;
                        else
                            iMap=i+j-1;
                    }

                    iSendData[0] = iMap;
                    SendCommand(ATC_LOAD_RECIPE_SINGLE);
                }
            }
        }
    }  */
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetTCWaterValve(AnsiString asWaterValueGPIB, int iWaterValue)
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1125)
    if(asWaterValueGPIB=="0")
    {
        asTCWaterValue="";
        for(int i=0; i<iATC_Use_Heat_Count; i++)
        {
            asTCWaterValue+=AnsiString(iWaterValue)+",";
        }
    }
    else
    {
        asTCWaterValue=asWaterValueGPIB;
    }

    SendCommand(ATC_SET_TC_WATER_VALVE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadTCWaterValue()
{
    if(iATC_MODE_TYPE==ATC_TYPE_60 || iATC_MODE_TYPE==ATC_TYPE_70) return;      //Steven 20260415 : ATC 6.0/7.0 unsupported (1071)
    SendCommand(ATC_READ_WATER_VALVE);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetDynamicPID(AnsiString asSetData)
{
    int iLen=0;
    AnsiString asOnOff="";

    for(int k=0; k<iATC_Use_Heat_Count; k++)
    {
        iSendData[k] = atoi(asReadDynamicPID[k].c_str());
    }

    for(int k=0; k<iATC_Use_Heat_Count/2; k++)
    {
        asOnOff="";
        iLen=asSetData.Pos(",");
        if(iLen==0)
        {
            iLen=asSetData.Pos(";");
        }
        asOnOff=asSetData.SubString(1, iLen-1);
        asSetData.Delete(1, iLen);

        if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
        {
            iSendData[k] = atoi(asOnOff.c_str());
        }
        else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
        {
            iSendData[k+iATC_Use_Heat_Count/2] = atoi(asOnOff.c_str());
        }
        else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
        {
        }
    }

    if(iATC_MODE_TYPE!=ATC_TYPE_35 &&
       iATC_MODE_TYPE!=ATC_TYPE_36) return;                                     //Steven 20260423 : 1127 Dynamic PID 僅 ATC 3.5/3.6 支援
    SendCommand(ATC_SET_DYNAMIC_PID);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ReadDynamicPID()
{
    if(iATC_MODE_TYPE!=ATC_TYPE_35 &&
       iATC_MODE_TYPE!=ATC_TYPE_36) return;                                     //Steven 20260423 : 1128 Dynamic PID 僅 ATC 3.5/3.6 支援
    SendCommand(ATC_READ_DYNAMIC_PID);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ChangeTJMode()
{
    asControlModeGPIB="";

    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        asControlModeGPIB+=ATC_InterfaceForm->asControlMode[i];
        if(i==iATC_Use_Heat_Count-1)
            asControlModeGPIB+=";";
        else
            asControlModeGPIB+=",";
    }

    if(asControlModeGPIB.Pos(1)!=0)
    {
        bTJControlMode=true;
    }
    else
    {
        if(bTJControlMode==true)
            bTJtoTCWait=true;                                                   //wei 20251002 : TJ轉回TC等待
        bTJControlMode=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::SetMultiSensorOffset(int iChCount, double *dOffset) //Ifor 20251203 : add Multi Sensor Offset
{
    if(iATC_MODE_TYPE!=ATC_TYPE_35) return;                                     //MultiSensor Offset 僅 ATC 3.5 支援
//    iDataCount = iChCount;
//        if(iDataCount>32)      //kevin 20160507
       iDataCount=32;

    for(int i=0; i<32; i++)
    {
        iSendData[i] = 9999;    //Invalid data.
        if(i<32)
        {
            //Eliot 2015_1123 start
            if(dOffset[i]>=0)
                iSendData[i] = (dOffset[i]*10+0.5); //Site 1~n Temperature Offset
            else
                iSendData[i] = (dOffset[i]*10-0.5); //Site 1~n Temperature Offset
            //Eliot 2015_1123 end
        }
    }
    SendCommand(ATC_SET_MULTI_TC_OFFSET);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::EnablesMultiSensor(bool *Enabled)
{
    if(iATC_MODE_TYPE!=ATC_TYPE_35) return;                                     //MultiSensor Enabled 僅 ATC 3.5 支援
    iDataCount = 4;

    for(int i=0; i<4; i++)
    {
        iSendData[i] = 9999;                                                    //Invalid data.
        iSendData[i] = Enabled[i];                                                //Site 1~n Enabled
    }

    SendCommand(ATC_Multi_Temperature_Control);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FileSocketConnect(TObject *Sender,          //Eastsun 20260522 整合
      TCustomWinSocket *Socket)
{
    // 只有當兩個 Socket 都連上時，才顯示成功
    if(ClientSocket->Socket->Connected)
    {
        sheRecipe->Brush->Color = clLime;
        labRecipe->Caption = "成功連線到 Server!";
    }
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FileSocketDisconnect(TObject *Sender,       //Eastsun 20260522 整合
      TCustomWinSocket *Socket)
{
    // 任何一個 Socket 斷線，都視為整體斷線
    ClientSocket->Active = false; // 關閉另一個 Socket
    sheRecipe->Brush->Color = clRed;
    labRecipe->Caption = "FileSocket 中斷連線";
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FileSocketError(TObject *Sender,            //Eastsun 20260522 整合
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AnsiString Str;
//    ShowMessage("File socket error: " + IntToStr(ErrorCode));
    Str.sprintf("File socket error: %s " ,IntToStr(ErrorCode));
    RecordProcess(Str);
    ErrorCode = 0;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::FileSocketRead(TObject *Sender,             //Eastsun 20260522 整合
      TCustomWinSocket *Socket)
{
    // 交給 Form2 的檔案資料處理函式
    frmFileTransfer->OnDataSocketRead(Socket, probRecipe);
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::GET_ATC_Recipe()                            //Eastsun 20260522 整合
{
    SendCommand(ATC_Star_Transmit_Recipe);

    DeleteDirectory(asATCFileTransferPath);
    CreateDir(asATCFileTransferPath);

    probRecipe->Position=0;

    iATC_RecipeFileTransfer=4;
}
//---------------------------------------------------------------------------
void __fastcall TATC_InterfaceForm::ATC_Recipe_FileTransfer(AnsiString asFileSend) //Eastsun 20260522 整合 //Ifor 20251229 add: ATC Rrcipe 傳送與接收
{
    // 確保兩個 Socket 都已連線
    if (ClientSocket->Socket->Connected && FileSocket->Socket->Connected)
    {
        // 呼叫InitiateFileTransfer，傳入兩個 Socket
        frmFileTransfer->InitiateFileTransfer(ClientSocket->Socket, FileSocket->Socket, asFileSend, probRecipe);
    }
    else
    {
        RecordProcess("Socket 未完全連線，無法傳送檔案。");
    }
}
//---------------------------------------------------------------------------
bool __fastcall TATC_InterfaceForm::IS_ATC33()                                  //Steven 20260716 : 整合ATC 3.3 / 3.5 / 3.6
{
    if(iATC_MODE_TYPE==ATC_TYPE_33 ||
       iATC_MODE_TYPE==ATC_TYPE_35 ||
       iATC_MODE_TYPE==ATC_TYPE_36)
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
