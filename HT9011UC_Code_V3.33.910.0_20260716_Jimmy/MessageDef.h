//---------------------------------------------------------------------------
//MV & VM 結構請不要任意加東西
//就算要加也是加在最下面
//只要有改就是 Handler, GPIB, RS232三隻程式要一起動, 然後一起進版號
//不能塞VCL元件在裡面, 只能用純C
//---------------------------------------------------------------------------
#ifndef MessageDefH
#define MessageDefH
//---------------------------------------------------------------------------

//#define VERSION_5_DEBUG
#include "MachineType.h"

extern AnsiString GPIBVersion;
extern double GPIBVersionCheck;
extern AnsiString RS232Version;
extern double TTLRS232VerCheck;                                                 //Isaac 20210511 : TTLRS232板子版本檢查
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
//MSG_CMD single-source table  (Steven 20260702)
//  只要有改就是 Handler, GPIB, RS232三隻程式要一起動, 然後一起進版號
//----------------------------------------------------------------------------
#define MSG_CMD_LIST_A(X) \
  X(  0, MSG_CMD_NONE                , "NONE"                    , "") \
  X(  1, MSG_CMD_CatalystSimpleGPIB  , "CatalystSimpleGPIB"      , "") \
  X(  2, MSG_CMD_SwitchArm           , "SwitchArm"               , "") \
  X(  3, MSG_CMD_SwitchArmOK         , "SwitchArmOK"             , "") \
  X(  4, MSG_CMD_AbortTest           , "AbortTest"               , "") \
  X(  5, MSG_CMD_AskArmTestMode      , "AskArmTestMode"          , "") \
  X(  6, MSG_CMD_2ArmTestMode        , "2ArmTestMode"            , "") \
  X(  7, MSG_CMD_1ArmTestMode        , "1ArmTestMode"            , "") \
  X(  8, MSG_CMD_NoFullSiteRespon    , "NoFullSiteRespon"        , "Steven 20141016 : FullSite的Test Time Out") \
  X(  9, MSG_CMD_ECHONG              , "ECHONG Error!"           , "Steven 20250915 : 資料錯誤") \
  X( 10, MSG_CMD_DoubleContact       , "DoubleContact"           , "") \
  X( 11, MSG_CMD_TimeOutSkip         , "TimeOutSkip"             , "Steven 20150304 : Add GPIB LOG") \
  X( 12, MSG_CMD_TimeOutRetryWait    , "TimeOutRetryWait"        , "") \
  X( 13, MSG_CMD_TimeOutRetrySend    , "TimeOutRetrySend"        , "") \
  X( 14, MSG_CMD_HandlerHomeStart    , "HandlerHomeStart"        , "") \
  X( 15, MSG_CMD_HandlerHomeFinish   , "HandlerHomeFinish"       , "") \
  X( 16, MSG_CMD_Arm1Down            , "Arm1Down"                , "") \
  X( 17, MSG_CMD_Arm2Down            , "Arm2Down"                , "") \
  X( 18, MSG_CMD_ContactTestArm1     , "ContactTestArm1"         , "") \
  X( 19, MSG_CMD_ContactTestArm2     , "ContactTestArm2"         , "") \
  X( 20, MSG_CMD_ContactTestAbort    , "ContactTestAbort"        , "") \
  X( 21, MSG_CMD_CloseGpib           , "CloseGpib"               , "wei 20150408 Add Close GPIB Command") \
  X( 22, MSG_CMD_LotStatus           , "LotStatus"               , "wei 20150409 Add Close GPIB Command") \
  X( 23, MSG_CMD_ChangeGpib          , "ChangeGpib"              , "wei 20150409 Add Close GPIB Command") \
  X( 24, MSG_CMD_Version             , "Version"                 , "wei 20150617 Add version control") \
  X( 25, MSG_CMD_RCMD                , "RCMD"                    , "jou 2015-09-21 Auto Retest function") \
  X( 26, MSG_CMD_SVID                , "SVID"                    , "jou 2015-09-21 Auto Retest function") \
  X( 27, MSG_CMD_ECID                , "ECID"                    , "jou 2015-09-21 Auto Retest function") \
  X( 28, MSG_CMD_RetestFlag          , "RetestFlag"              , "jou 2015-09-21 Auto Retest function") \
  X( 29, MSG_CMD_CEIDON              , "CEIDON"                  , "Steven 20150901 : For GPIB V1.08 Command") \
  X( 30, MSG_CMD_CEIDOFF             , "CEIDOFF"                 , "Steven 20150901 : For GPIB V1.08 Command") \
  X( 31, MSG_CMD_EnableBarCode       , "EnableBarCode"           , "Steven 20150713 : Add 2D code wei 20150924") \
  X( 32, MSG_CMD_DisableBarCode      , "DisableBarCode"          , "Steven 20150713 : Add 2D code wei 20150924") \
  X( 33, MSG_CMD_BarCodeFlowErr      , "BarCodeFlowErr"          , "Steven 20150713 : Add 2D code wei 20150924") \
  X( 34, MSG_CMD_MachineState        , "MachineState"            , "JerryYang 20151109 add for 力成 回覆機台狀態") \
  X( 35, MSG_CMD_OverDrive           , "OverDrive"               , "Steven 20151207 : OverDrive for TSMC") \
  X( 36, MSG_CMD_ReContact           , "ReContact"               , "Steven 20151207 : Recontact for TSMC") \
  X( 37, MSG_CMD_TesterBin           , "TesterBin"               , "JerryYang 20151207 : BinCount for Maxim Philippine") \
  X( 38, MSG_CMD_SoakTime            , "SoakTime"                , "JerryYang 20160308 : SoakTime for Maxim Philippine") \
  X( 39, MSG_CMD_JamCode             , "JamCode"                 , "JerryYang 20160316 : Jam Code for Maxim Philippine") \
  X( 40, MSG_CMD_SiteMap             , "SiteMap"                 , "JerryYang 20160316 : Get site map for Maxim Philippine") \
  X( 41, MSG_CMD_AllMassTemp         , "AllMassTemp"             , "JerryYang 20160325 : Get all mess temp for Maxim Philippine") \
  X( 42, MSG_CMD_BarcodeOFF          , "BarcodeOFF"              , "wei 20161024 No Open Barcode Function") \
  X( 43, MSG_CMD_SCKART_LOTCLEAR     , "ART_LOTCLEAR"            , "Steven 20161025 : SCK ART function") \
  X( 44, MSG_CMD_SCKART_LOTRTCLEAR   , "ART_LOTRTCLEAR"          , "") \
  X( 45, MSG_CMD_SCKART_INPUTQTY     , "ART_INPUTQTY"            , "") \
  X( 46, MSG_CMD_SCKART_LOTSTATUS    , "ART_LOTSTATUS"           , "") \
  X( 47, MSG_CMD_SCKART_Alarm        , "ART_Alarm"               , "") \
  X( 48, MSG_CMD_SCKART_QTY          , "ART_QTY"                 , "") \
  X( 49, MSG_CMD_SCKART_INITIAL      , "ART_INITIAL"             , "") \
  X( 50, MSG_CMD_SCKART_SRQMASK      , "ART_SRQMASK"             , "") \
  X( 51, MSG_CMD_TesterMode          , "TesterMode"              , "") \
  X( 52, MSG_CMD_State_Record        , "State_Record"            , "wei 20170911 (steven) State Record") \
  X( 53, MSG_CMD_SCKART_RunDummy     , "ART_RunDummy"            , "Steven 20180824 : Semi ART") \
  X( 54, MSG_CMD_Auto_Clean          , "Auto_Clean"              , "wei 20180309") \
  X( 55, MSG_CMD_Pause               , "Pause"                   , "wei 20180309") \
  X( 56, MSG_CMD_TempArm             , "TempArm"                 , "") \
  X( 57, MSG_CMD_TestArm             , "TestArm"                 , "") \
  X( 58, MSG_CMD_ContactForce        , "ContactForce"            , "") \
  X( 59, MSG_CMD_ActualTemp          , "ActualTemp"              , "") \
  X( 60, MSG_CMD_Assign              , "Assign"                  , "") \
  X( 61, MSG_CMD_StartMode           , "StartMode"               , "") \
  X( 62, MSG_CMD_HandlerID           , "HandlerID"               , "") \
  X( 63, MSG_CMD_HandlerSiteMap      , "HandlerSiteMap"          , "") \
  X( 64, MSG_CMD_HandlerSoakTime     , "HandlerSoakTime"         , "") \
  X( 65, MSG_CMD_HandlerTemperature  , "HandlerTemperature"      , "") \
  X( 66, MSG_CMD_Force               , "Force"                   , "") \
  X( 67, MSG_CMD_BinMap              , "BinMap"                  , "")
#define MSG_CMD_LIST_B(X) \
  X( 68, MSG_CMD_TestMode            , "TestMode"                , "") \
  X( 69, MSG_CMD_GetNowAllTemp       , "GetNowAllTemp"           , "") \
  X( 70, MSG_CMD_ChkSetup            , "ChkSetup"                , "") \
  X( 71, MSG_CMD_GetTestArmPos       , "GetTestArmPos"           , "") \
  X( 72, MSG_CMD_GetTestArmEP        , "GetTestArmEP"            , "") \
  X( 73, MSG_CMD_SetTemp             , "SetTemp"                 , "") \
  X( 74, MSG_CMD_SetSoakTime         , "SetSoakTime"             , "") \
  X( 75, MSG_CMD_SetTJ               , "SetTJ"                   , "") \
  X( 76, MSG_CMD_SetSiteMapData      , "SetSiteMapData"          , "") \
  X( 77, MSG_CMD_SetAlarmSetup       , "SetAlarmSetup"           , "") \
  X( 78, MSG_CMD_EnableAMDFunction   , "EnableAMDFunction"       , "V3.30.649 (GPIB V12.03) 以後不使用") \
  X( 79, MSG_CMD_DisableAMDFunction  , "DisableAMDFunction"      , "V3.30.649 (GPIB V12.03) 以後不使用") \
  X( 80, MSG_CMD_AMDNextStep1        , "AMDNextStep1"            , "") \
  X( 81, MSG_CMD_AMDNextStep2        , "AMDNextStep2"            , "") \
  X( 82, MSG_CMD_HanderIDRS232       , "HandlerIDRS232"          , "") \
  X( 83, MSG_CMD_GetSiteOnOff        , "GetSiteOnOff"            , "") \
  X( 84, MSG_CMD_GetNumOfSites       , "GetNumOfSites"           , "") \
  X( 85, MSG_CMD_DeviceMapSRQ        , "DeviceMapSRQ"            , "") \
  X( 86, MSG_CMD_PickLoad            , "PickLoad"                , "") \
  X( 87, MSG_CMD_PlaceLoad           , "PlaceLoad"               , "") \
  X( 88, MSG_CMD_TrayFeed            , "TrayFeed"                , "") \
  X( 89, MSG_CMD_SamSung_Tmp         , "TMP?"                    , "Steven 20191112 : 三星格式") \
  X( 90, MSG_CMD_SamSung_Map         , "MAP?"                    , "") \
  X( 91, MSG_CMD_SamSung_Soak        , "SOAK?"                   , "") \
  X( 92, MSG_CMD_AMDRS232Connect     , "AMDRS232Connect"         , "Ifor 20200220 : add AMD Rs232 Connect Error Alarm") \
  X( 93, MSG_CMD_2DIDFormat          , "2DIDFormat"              , "JerryYang 20200422 2DID format") \
  X( 94, MSG_CMD_State_TTL           , "State_TTL"               , "Isaac 20200903 :TTL RS232通訊") \
  X( 95, MSG_CMD_Command_TTL         , "Command_TTL"             , "Isaac 20200903 :TTL RS232通訊") \
  X( 96, MSG_CMD_ESC                 , "ESC Function"            , "Steven 20201022 : For RFMD Empty Socket Check Funstion.") \
  X( 97, MSG_CMD_RESUME              , "RESUME"                  , "Steven 20201022 : For RFMD") \
  X( 98, MSG_CMD_TestAlarm           , "TestAlarm"               , "Steven 20201022 : For RFMD") \
  X( 99, MSG_CMD_POWERFOLLOWING      , "POWERFOLLOWING"          , "") \
  X(100, MSG_CMD_SetPID              , "SetPID"                  , "") \
  X(101, MSG_CMD_GET_PFC_PARAMETER   , "GET_PFC_PARAMETER"       , "讀取單一溫度的PF參數值") \
  X(102, MSG_CMD_SET_PFC_PARAMETER   , "SET_PFC_PARAMETER"       , "設定單一溫度的PF參數值") \
  X(103, MSG_CMD_DoubleContactCount  , "DoubleContactCount"      , "Isaac 20210706 : add MSG_CMD_DoubleContactCount指令，詢問handler doublecontact次數") \
  X(104, MSG_CMD_SetTestTemp         , "Set Test Temp"           , "Ifor 20210623 add: Test Temp Change") \
  X(105, MSG_CMD_SIGURD_CHKSTATUS    , "CHKSTATUS?"              , "KaiChen 20180910 ：Add GPIB CHKSTATUS?") \
  X(106, MSG_CMD_ONECYCLE            , "ONECYCLE"                , "KaiChen 20180910 ：Add GPIB ONECYCLE") \
  X(107, MSG_CMD_ECHOOK_ONECYCLE     , "ECHOOK_ONECYCLE"         , "KaiChen 20181114 ：Add GPIB ECHOOK:ONECYCLE") \
  X(108, MSG_CMD_GETBINCATEGORY      , "GETBINCATEGORY?"         , "KaiChen 20180913 ：Add GPIB GETBINCATEGORY?") \
  X(109, MSG_CMD_SETUPFILENAME       , "SETUPFILENAME?"          , "KaiChen 20181022 ：Add GPIB GETSETUPFILENAME?") \
  X(110, MSG_CMD_SIGURD_HANDLERID    , "HANDLERID?"              , "KaiChen 20200507 ：Add GPIB HANDLERID?") \
  X(111, MSG_CMD_SGSETUP             , "SGSETUP_"                , "KaiChen 20190613 ：Add GPIB SGSETUP_") \
  X(112, MSG_CMD_SETSTARTMODE        , "SETSTARTMODE_"           , "KaiChen 20180910 ：Add GPIB SetStartMode_") \
  X(113, MSG_CMD_CHECKLIST           , "CHECKLIST?"              , "KaiChen 20190613 ：Add GPIB CHECKLIST?") \
  X(114, MSG_CMD_BINPOS              , "BINPOS_"                 , "KaiChen 20190706 ：Add GPIB BINPOS_") \
  X(115, MSG_CMD_GetSGFTP_STATUS     , "GetSGFTP_STATUS"         , "Sam 20210329 : Add GPIB SGFTP_STATUS") \
  X(116, MSG_CMD_SetSGFTP            , "SetSGFTP_STATUS"         , "Sam 20210329 : Add GPIB SGFTP_ SGFTP_ON/SGFTP_OFF") \
  X(117, MSG_CMD_SetNONDOUBLEBIN     , "Non-DoubleBin"           , "Sam 20210329 : Add GPIB NONDOUBLEBIN_") \
  X(118, MSG_CMD_SetBINCOUNT         , "BINCOUNT_"               , "Sam 20210329 : Add GPIB BINCOUNT_") \
  X(119, MSG_CMD_SetSGOSBIN          , "SGOSBIN_"                , "Sam 20210406 : Add GPIB SGOSBIN_") \
  X(120, MSG_CMD_SetSGCONTFAIL       , "SGCONTFAIL_"             , "Sam 20210422 : Add GPIB SGCONTFAIL_") \
  X(121, MSG_CMD_SETTESTERID         , "SETTESTERID_"            , "Sam 20210617 : Add GPIB SETTESTERID") \
  X(122, MSG_CMD_GETTESTERID         , "SETTESTERID?"            , "Sam 20210617 : Add GPIB SETTESTERID") \
  X(123, MSG_CMD_SBIN                , "SOFTBIN"                 , "Steven 20220120 : Amlogic需要收SBIN") \
  X(124, MSG_CMD_Pause01             , "PAUSE_01"                , "Steven 20220517 : Add for GIGA") \
  X(125, MSG_CMD_Stop_01             , "STOP_01"                 , "Steven 20220517 : Add for GIGA") \
  X(126, MSG_CMD_GetAutClean         , "AUTOCLEAN?"              , "Sam 20220408 : Novatek 新增 AUTOCLEAN?") \
  X(127, MSG_CMD_ForcePerPinN        , "DEVICEFORCEPERPIN?"      , "Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?") \
  X(128, MSG_CMD_ContactHeight       , "ARMCONTACTHIGHVALUE?"    , "Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?") \
  X(129, MSG_CMD_YieldContinusFail   , "YIELDCONTINUESFAIL?"     , "Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?") \
  X(130, MSG_CMD_YieldSiteCompare    , "YIELDSITEUNBALANCE?"     , "Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?") \
  X(131, MSG_CMD_DUTStatus           , "DUTSTATUS?"              , "Sam 20220408 : Novatek 新增 DUTSTATUS?") \
  X(132, MSG_CMD_UPH                 , "UPH?"                    , "Sam 20220408 : Novatek 新增 UPH?") \
  X(133, MSG_CMD_IndexCycleTime      , "INDEXCYCLETIME?"         , "Sam 20220408 : Novatek 新增 INDEXCYCLETIME?") \
  X(134, MSG_CMD_TempOfs             , "GETTEMPOFFSET?"          , "Sam 20220408 : Novatek 新增 GETTEMPOFFSET?") \
  X(135, MSG_CMD_TempRange           , "GETTEMPERATURETOLERANCE?", "Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?")
#define MSG_CMD_LIST_C(X) \
  X(136, MSG_CMD_VACUUMAIR           , "VACUUMAIR?"              , "Sam 20220408 : Novatek 新增 VACUUMAIR?") \
  X(137, MSG_CMD_Get_All             , "SET_ALL?"                , "Sam 20220408 : Novatek 新增 SET_ALL?") \
  X(138, MSG_CMD_HandlerVersion      , "HANDLERVERSION?"         , "Sam 20220408 : Novatek 新增 HANDLERVERSION?") \
  X(139, MSG_CMD_PPSELECT            , "PPSELECT"                , "Richard 20220929 :Add for UTAC 讀檔") \
  X(140, MSG_CMD_ASKPPSELECT         , "ASKPPSELECT"             , "Richard 20220929 :Add for UTAC 讀檔 詢問Handler當前檔名") \
  X(141, MSG_CMD_SetBinMap           , "Set Bin Map"             , "Steven 20230210 : Set Bin Map.") \
  X(142, MSG_CMD_GETSHUTTLEMODE      , "GETSHUTTLEMODE?"         , "Sam 20230130 : Add GPIB GETSHUTTLEMODE?") \
  X(143, MSG_CMD_SETMAXTEST          , "SETMAXTEST_"             , "Sam 20230201 : Add GPIB SETMAXTEST_") \
  X(144, MSG_CMD_GETMAXTEST          , "GETMAXTEST?"             , "Sam 20230201 : Add GPIB GETMAXTEST") \
  X(145, MSG_CMD_SETINITIALMAXTEST   , "SETINITIALMAXTEST_"      , "Sam 20230201 : Add GPIB SETINITIALMAXTEST_") \
  X(146, MSG_CMD_GETINITIALMAXTEST   , "GETINITIALMAXTEST?"      , "Sam 20230201 : Add GPIB GETINITIALMAXTEST") \
  X(147, MSG_CMD_READYNEXTSHOT       , "READYNEXTSHOT"           , "Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information") \
  X(148, MSG_CMD_NEXT2DID            , "NEXT2DID"                , "Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information") \
  X(149, MSG_CMD_CloseSiteHaveBin    , "CloseSiteHaveBin"        , "Steven 20231017 : GPIB flow error need alarm") \
  X(150, MSG_CMD_BinonWithout0x41    , "BinonWithout0x41"        , "Steven 20231017 : GPIB flow error need alarm") \
  X(151, MSG_CMD_BinonWithoutFullsite, "BinonWithoutFullsite"    , "Steven 20231017 : GPIB flow error need alarm") \
  X(152, MSG_CMD_SETAICCD            , "SETAICCD_"               , "Sam 20231108 : Add GPIB SETAICCD_") \
  X(153, MSG_CMD_ASIF_TJ_EFUSED      , "ASIF_TJ_EFUSED"          , "Steven 20240903 : for MTK ASIF data") \
  X(154, MSG_CMD_ASIF_TJ_REQUEST     , "ASIF_TJ_REQUEST"         , "Steven 20240903 : for MTK ASIF data") \
  X(155, MSG_CMD_ASIF_TJ_FB          , "ASIF_TJ_FB"              , "Steven 20240903 : for MTK ASIF data") \
  X(156, MSG_CMD_GETAICCD            , "GETAICCD?"               , "Sam 20240826 : Add GPIB GETAICCD?") \
  X(157, MSG_CMD_QRA                 , "ART Enabled?"            , "Steven 20241004 : Qorvo check ART enable") \
  X(158, MSG_CMD_RUN_HANA_ART        , "RUN_HANA_ART"            , "JimmyChiu 20241023 HANA ART Function") \
  X(159, MSG_CMD_HANA_ART            , "HANA_ART"                , "JimmyChiu 20241023 HANA ART Function") \
  X(160, MSG_CMD_SETOSBIN            , "SETOSBIN_"               , "Sam 20250115 : Add GPIB SETOSBIN_") \
  X(161, MSG_CMD_GETOSBIN            , "GETOSBIN?"               , "Sam 20250115 : Add GPIB GETOSBIN?") \
  X(162, MSG_CMD_DUTCHK              , "DUTCHK?"                 , "Steven 20250701 : for DOOSAN TESNA") \
  X(163, MSG_CMD_GetFFC              , "GetFFC?"                 , "Steven 20250701 : for Ampere") \
  X(164, MSG_CMD_GetTJFunction       , "GetTJFunction?"          , "") \
  X(165, MSG_CMD_GetPowerFollowing   , "GetPowerFollowing?"      , "") \
  X(166, MSG_CMD_SetSiteOnOff        , "SetSiteOnOff"            , "") \
  X(167, MSG_CMD_EnableFTPFunction   , "EnableFTPFunction"       , "Ifor 20231101 add:FTP Function") \
  X(168, MSG_CMD_DisableFTPFunction  , "DisableFTPFunction"      , "Ifor 20231101 add:FTP Function") \
  X(169, MSG_CMD_SetupFileChange     , "SetupFileChange"         , "Ifor 20231101 add:FTP Function") \
  X(170, MSG_CMD_GetContactCount     , "GetContactCount"         , "Ifor 20240510 add: Get Head Contact Count") \
  X(171, MSG_CMD_EnablePin1Function  , "EnablePin1Function"      , "Ifor 20240528 add:Pin1 Function") \
  X(172, MSG_CMD_DisablePin1Function , "DisablePin1Function"     , "Ifor 20240528 add:Pin1 Function") \
  X(173, MSG_CMD_BarcodePin1ON       , "BarcodePin1ON"           , "Ifor 20240528 add:Pin1 Function") \
  X(174, MSG_CMD_GetSocketCounter    , "GetSocketCounter"        , "Ifor 20250607 add:Get Socket Counter") \
  X(175, MSG_CMD_GetTIMCounter       , "GetTIMCounter"           , "Ifor 20250607 add:Get TIM Counter") \
  X(176, MSG_CMD_FTPDownLoad         , "FTPDownLoad"             , "Ifor 20231101 add:FTP Function") \
  X(177, MSG_CMD_SetSocketToBinR     , "SetSocketToBinR"         , "Ifor 20210911: Add KLT 要求BINON WITHOUT 0x41 ERROR直接分ErrBin") \
  X(178, MSG_CMD_MultiZoneTemp       , "MultiZoneTemp"           , "wei 20240617 Multi Zone Temp") \
  X(179, MSG_CMD_MultiZoneEnable     , "MultiZoneEnable"         , "wei 20240617 Multi Zone Temp") \
  X(180, MSG_CMD_GET_SLOPE_OFFSET    , "GET_SLOPE_OFFSET"        , "Get the Slope/Offset value Eliot 20210412") \
  X(181, MSG_CMD_SET_SLOPE_OFFSET    , "SET_SLOPE_OFFSET"        , "Set the Slope/Offset value Eliot 20210412") \
  X(182, MSG_CMD_READTJ              , "READTJ"                  , "Set the Slope/Offset value Eliot 20210412") \
  X(183, MSG_CMD_GET_VOLTAGE         , "GET_VOLTAGE"             , "Set the Slope/Offset value Eliot 20210412") \
  X(184, MSG_CMD_SET_ATCCONTROLMODE  , "SET_ATCCONTROLMODE"      , "Set the Slope/Offset value Eliot 20210412") \
  X(185, MSG_CMD_SET_ATC_TEMP        , "SET_ATC_TEMP"            , "Set ATC Temp") \
  X(186, MSG_CMD_RECODETJ            , "RECODETJ"                , "") \
  X(187, MSG_CMD_QUERYTJ             , "QUERYTJ"                 , "") \
  X(188, MSG_CMD_GET_ATCCONTROLMODE  , "GET_ATCCONTROLMODE"      , "") \
  X(189, MSG_CMD_GET_ATCERROR        , "GET_ATCERROR"            , "") \
  X(190, MSG_CMD_RESET_ATCALARM      , "RESET_ATCALARM"          , "") \
  X(191, MSG_CMD_READ_WATER_VALVE    , "READ_WATER_VALVE"        , "wei 20250512") \
  X(192, MSG_CMD_SET_WATER_VALVE     , "SET_WATER_VALVE"         , "wei 20250512") \
  X(193, MSG_CMD_SET_DYNAMIC_PID     , "SET_DYNAMIC_PID"         , "wei 20250512") \
  X(194, MSG_CMD_READ_DYNAMIC_PID    , "READ_DYNAMIC_PID"        , "wei 20250512") \
  X(195, MSG_CMD_AUTOZSTART          , "AUTOZSTART"              , "wei 20250512") \
  X(196, MSG_CMD_AUTOZMOVE           , "AUTOZMOVE"               , "wei 20250512") \
  X(197, MSG_CMD_AUTOZMOVEOK         , "AUTOZMOVEOK"             , "wei 20250512") \
  X(198, MSG_CMD_AUTOZPASS           , "AUTOZPASS"               , "wei 20250512") \
  X(199, MSG_CMD_AUTOZREACHLIMIT     , "AUTOZREACHLIMIT"         , "wei 20250512") \
  X(200, MSG_CMD_AUTOZOK             , "AUTOZOK"                 , "wei 20250512") \
  X(201, MSG_CMD_READAUTOZLIMIT      , "READAUTOZLIMIT"          , "wei 20250512") \
  X(202, MSG_CMD_READZPOS            , "READZPOS"                , "wei 20250625") \
  X(203, MSG_CMD_READZTORQUE         , "READZTORQUE"             , "wei 20250625")
#define MSG_CMD_LIST(X) MSG_CMD_LIST_A(X) MSG_CMD_LIST_B(X) MSG_CMD_LIST_C(X)

#define X(idx,name,str,note) extern const unsigned int name;
MSG_CMD_LIST(X)
#undef X
extern const char * const MSG_CMD_NAMES[];
extern const int          MSG_CMD_COUNT;
typedef struct
{
    unsigned int iCommand;                                                      //Steven 20141008 : iTemp --> iCommand
    unsigned int Result[32];
    bool bError;
    bool bEchoStop;                                                             //ChungHung 20130326 add
    char cReturn[256];                                                          //kevin 20160307
    char GpibStatus[32];                                                        // kevin 20130425
    char GpibData[256];                                                         // kevin 20130425
    int  GPIBBin;                                                               //kevin 20140305 gpib bin 數量   //Steven 20151219 : 沒在用,拿來傳輸簡單的資料
    bool bOneCycle;                                                             //jou 2014-09-23 Tester Low Yield Handler need One Cycle & Alarm
}VM;
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iSendCommand;                                                  //Steven 20141008 : iTimeOutSecond --> iSendCommand
    unsigned int Site[32];
    bool bSimulate;
    bool bSupport32Bin;                                                         //Steven 20121112 : RS232支援32Bin  b8080 --> bSupport32Bin
    bool bCloseGpib;
    bool bTimeOutProcess;
    int  GpibAddress;
    bool MachineISRun;
    bool IsTest;
    bool bGpibMode;
    int  iLotStatus;
    HWND HandlerHwnd;
    HWND GpibHwnd;
    int  GPIBBin;                                                               //kevin 20140305 gpib bin 數量
    int  iStatus[17];                                                           //JerryYang 20151109 add for 力成 回覆tester機台狀態
    char Message[2048];                                                         //Steven 20150713 : for 2D Code
//    #ifdef AMD_Version
    char UseSiteMapData[256];                                                   //Ifor 20201030 add:送Site Mapping 資料給GPIB
    char asATC_TYPE[32];                                                        //Ifor 20230828 add:送ATC TYPE 資料給GPIB
    char MultiMessage[4096];                                                    //Ifor 20241003 add: for Multi 2D Code
    char sMulti2DIDStringSeparator[10];                                         //JerryYang 20260504 : add
//    #endif
}MV;
//------------------------------------------------------------------------------
//  從Refresh程式收訊息  kevin 20110317
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iType;                                                         // 0 : normal model
    char cSendData[300];
    bool bError;
}VM1;
//------------------------------------------------------------------------------
//  送訊息給Refresh程式   kevin 20110317
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iType;                                                         // 0 : normal model
    char cSendData[300];
    bool bError;
}MV1;
//---------------------------------------------------------------------------
//  從CCD程式收訊息  kevin 20110811
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iType;                                                         // 0 : normal model
    char cSendData[300];
    int  iCustomer;                                                             //客戶碼
    int  iRespondState;                                                         //回應狀態  CDHT_EXISTENCE  CDHT_FILE_READY
    bool bError;
}VM2;
//------------------------------------------------------------------------------
//  送訊息給CCD程式   kevin 20110811
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iType;                                                         // 0 : normal model
    char cSendData[300];
    int  iCustomer;
    int  iCCDDoState;                                                           //讓CCD作事狀態   CDHT_EXISTENCE  CDHT_FILE_READY
    bool bError;
}MV2;
//---------------------------------------------------------------------------
//For Handler----------
extern VM *HGpib2Handler;
extern MV HHandler2Gpib;

//For GPIB-------------
extern VM GGpib2Handler;
extern MV *GHandler2Gpib;
//---------------------
extern VM1 *RefeshReceve;
extern MV1 RefeshSend;
extern VM2 *CCDReceve;
extern MV2 CCDSend;
//---------------------------------------------------------------------------

#endif
