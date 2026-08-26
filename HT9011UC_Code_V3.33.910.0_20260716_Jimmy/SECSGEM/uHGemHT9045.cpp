#include "MachineDefine.h"
#pragma hdrstop
#include <sstream>
#include "uHGemHT9045.h"

#include "MachineType.h"
#include "database.h"
#include "main.h"
#include "cSortCT.h"
#include "cObserver.h"
#include "cprod.h"
#include "mysensor.h"
#include "uShowMessage.h"
#include "cTemperFrom.h"
#include "csystem.h"
#include "rs232.h"
#include "uLotInfo.h"
#include "common.h"
#include "atester.h"
#include "mymessbox.h"
#include "INPUT.h"
#include "cMyDB.h"
#include "cTowerLight.h"
#include "cmydef.h"
#include "note.h"
#include "cSocket.h"
#include "cContactCT.h"
#include "MyKitSuck.h"
#include "cSpeed.h"
#include "cContact.h"
#include "cTrayAssignment.h"
#include "cTesterIF.h"
//#include "ArmOffsetData.h"
#include "cOffSet.h"
#include "uCleaning.h"
#include "uTemp_Set.h"
#include "cConfiguration.h"
#include "cSecurity.h"
#include "cShowBinSelect.h"
#include "uCleaning.h"
#include "fRotate.h"
#include "cBinSel.h"
#include "MyMotor.h"                                                            //ChungHung 20150511 modify
#include "FTPClient.h"                                                          //ChungHung 20150515 add FTP control by SECS GEM
#include "AutoRetest.h"
#include "cBuilder.h"
#include "cCounterClear.h"
#include "InterfaceSYS.h"
#include "BarcodeReader.h"
#include "Password.h"
#include "ProductionInfo.h"
#include "SCK_ART.h"
#include "TesterTCP.h"
#include "BarCode.h"
//#include "BarcodeXML.h"
#include "HS_Function.h"
#include "cSetUp.h"
#include "AGV.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

struct ETypeStruct SECS_EVENT;
char GEM_MachineName[16];
//---------------------------------------------------------------------------
HT9045Gem::HT9045Gem(AnsiString Path, THGem *HGemTmp)                           // Tmp
{
    HGemPtr=HGemTmp;
    HandlerPath=Path;

    EventDescription[SECS_EVENT.DoStart]            ="1 Start Pressed";
    EventDescription[SECS_EVENT.DoPause]            ="2 Pause Pressed";
    EventDescription[SECS_EVENT.DoOneCycle]         ="3 OneCycle Pressed";
    EventDescription[SECS_EVENT.DoCleanOut]         ="4 CleanOut Pressed";
    EventDescription[SECS_EVENT.DoClearCount]       ="5 ClearCount Pressed";
    EventDescription[SECS_EVENT.DoLotStart]         ="6 Lot Start";
    EventDescription[SECS_EVENT.DoLot]              ="7 Lot";
    EventDescription[SECS_EVENT.DoLotEnd]           ="8 Lot End";
    EventDescription[SECS_EVENT.SwitchRunMode]      ="9 Switch Real Dummy Mode";
    EventDescription[SECS_EVENT.SwitchTesterMode]   ="10 Switch Tester Online";
    EventDescription[SECS_EVENT.SwitchProduction]   ="11 Switch Production Mode";
    EventDescription[SECS_EVENT.SwitchEngineer]     ="12 Switch Engineer Mode";
    EventDescription[SECS_EVENT.SwitchTemperature]  ="13 Switch Temperature Mode";
    EventDescription[SECS_EVENT.SwitchStartMode]    ="14 Switch StartMode";
    EventDescription[SECS_EVENT.SwitchSetupFile]    ="15 Switch Setup File";
    EventDescription[SECS_EVENT.SwitchUser]         ="16 Switch UserLevel";
    EventDescription[SECS_EVENT.EnterTool]          ="17 Enter Tool Page";
    EventDescription[SECS_EVENT.EnterConfig]        ="18 Enter Maintenance Page";
    EventDescription[SECS_EVENT.EnterOffset]        ="19 Enter Offset Page";
    EventDescription[SECS_EVENT.EnterSpeed]         ="20 Enter Speed Page";
    EventDescription[SECS_EVENT.EnterIO]            ="21 Enter IO Page";
    EventDescription[SECS_EVENT.EnterMessage]       ="22 Enter Message Page";
    EventDescription[SECS_EVENT.EnterDebug]         ="23 Enter Debug Page";
    EventDescription[SECS_EVENT.DoExit]             ="24 Exit Pressed";
    EventDescription[SECS_EVENT.DoHome]             ="25 Home Pressed";
    EventDescription[SECS_EVENT.GetTestResult]      ="26 Get Test Result";
    EventDescription[SECS_EVENT.RunStatus]          ="27 Change Machine State";
    EventDescription[SECS_EVENT.DoRetry]            ="28 Retry Pressed";
    EventDescription[SECS_EVENT.DoSkip]             ="29 Skip Pressed";
    EventDescription[SECS_EVENT.DoAlarmReset]       ="30 Alarm Reset Pressed";
    EventDescription[SECS_EVENT.DoTrayEnd]          ="31 Tray End Pressed";
    EventDescription[SECS_EVENT.DoTrayFeed]         ="32 Tray Feed Pressed";
    EventDescription[SECS_EVENT.DoReset]            ="33 Reset Pressed";
    EventDescription[SECS_EVENT.DoAutoClean]        ="34 Auto Clean Start";
    EventDescription[SECS_EVENT.Auto1Full]          ="35 Auto1 Full";
    EventDescription[SECS_EVENT.Auto2Full]          ="36 Auto2 Full";
    EventDescription[SECS_EVENT.Auto3Full]          ="37 Auto3 Full";
    EventDescription[SECS_EVENT.Fix1Full]           ="38 Fix1 Full";
    EventDescription[SECS_EVENT.Fix2Full]           ="39 Fix2 Full";
    EventDescription[SECS_EVENT.Fix3Full]           ="40 Fix3 Full";
    EventDescription[SECS_EVENT.OneCycleFinish]     ="41 One Cycle Finish";
    EventDescription[SECS_EVENT.CleanOutFinish]     ="42 Clean Out Finish";
    EventDescription[SECS_EVENT.DownloadRecipe]     ="43 DownLoadRecipe";
    EventDescription[SECS_EVENT.SiteOnOff]          ="44 Site On Off";
    EventDescription[SECS_EVENT.ArmOnOff]           ="45 Arm On Off";
    EventDescription[SECS_EVENT.SwitchTempData]     ="46 Change Temp Defaultand Soak Time";
    EventDescription[SECS_EVENT.SwitchSpeed]        ="47 Change HandlerSpeed";
    EventDescription[SECS_EVENT.ChangeEC]           ="48 Change EC";
    EventDescription[SECS_EVENT.TrayFeedFinish]     ="49 Tray Feed Finish";
    EventDescription[SECS_EVENT.AutoCleanFinish]    ="50 Auto Clean Finish";
    EventDescription[SECS_EVENT.SiteMappingStart]   ="51 Site Mapping Start";
    EventDescription[SECS_EVENT.SiteMappingEnd]     ="52 Site Mapping End";
    EventDescription[SECS_EVENT.UPHRecordStart]     ="53 UPH Record Start";
    EventDescription[SECS_EVENT.UPHRecordEnd]       ="54 UPH Record End";
    EventDescription[SECS_EVENT.InitialArtStart]    ="55 Initial ART Start";
    EventDescription[SECS_EVENT.TesterFT]           ="56 Change Tester Program to FT";
    EventDescription[SECS_EVENT.TesterRT]           ="57 Change Tester Program to RT";
    EventDescription[SECS_EVENT.ReadyForArt]        ="58 Ready for ART";

    EventDescription[SECS_EVENT.ArtReceiveTrayOK]       ="59 ART Receive Tray OK";
    EventDescription[SECS_EVENT.ArtReceiveTraySTART]    ="60 ART Receive Tray START";
    EventDescription[SECS_EVENT.ArtRTFinish]            ="61 RT Finish";
    EventDescription[SECS_EVENT.ArtTrayFeedFinish]      ="62 ART Finish";
    EventDescription[SECS_EVENT.ArtFTFinish]            ="63 FT Finish";
    EventDescription[SECS_EVENT.DownLoadRecipeByFTPOK]  ="64 DownLoad Recipe by FTP OK";
    EventDescription[SECS_EVENT.DownLoadRecipeByFTPNG]  ="65 DownLoad Recipe by FTP NG";
    EventDescription[SECS_EVENT.LoadTrayFinish]         ="66 Load Tray Finish";
    EventDescription[SECS_EVENT.TrayTestFinish]         ="67 Tray Test Finish";
    EventDescription[SECS_EVENT.AutoCleanClearCount]    ="68 Auto Clean Clear Count";
    EventDescription[SECS_EVENT.SiteMappingStop]        ="69 Site Mapping Stop";
    EventDescription[SECS_EVENT.BarcodeReaderEnter]     ="70 Barcode Reader Enter";
    EventDescription[SECS_EVENT.OTDLock]                ="71 OTD Lock";
    EventDescription[SECS_EVENT.OTDUnLock]              ="72 OTD UnLock";
    EventDescription[SECS_EVENT.MymessboxOK]            ="73 Mymessbox OK";
    EventDescription[SECS_EVENT.RemoteProgramClose]     ="74";
    EventDescription[SECS_EVENT.ChangeTesterPrgToEQC]   ="75";
    EventDescription[SECS_EVENT.DoStartHasIC]           ="76 Start Pressed HasIC";                                      //Ifor 20151118 非第一次啟動
    EventDescription[SECS_EVENT.ReadCurrentESDData]     ="77 Read Current ESD Data";                                    //Ifor 20160302 Time event, after setting time will sent report automatically
    EventDescription[SECS_EVENT.JamSkipICCount]         ="78 Jam Skip IC Count";                                        //wei 20160503 Jam Skip IC Count
    EventDescription[SECS_EVENT.REVERSED79]             ="79 ";
    EventDescription[SECS_EVENT.ReadNowHandlerData]     ="80 Read Now Handler Data";                                    //Ifor 20160330 KYEC 要求一個Even 即可
    EventDescription[SECS_EVENT.ReadATCTemperature]     ="81 Read ATC Temperature";
    EventDescription[SECS_EVENT.ReadATCRefTemperature]  ="82 Read ATC Ref Temperature";
    EventDescription[SECS_EVENT.ReadNowEPPenconder]     ="83 Read Now EP Penconder";
    EventDescription[SECS_EVENT.RunStatus_FT]           ="84 Run Status FT";    //wei 20170104
    EventDescription[SECS_EVENT.RunStatus_RT]           ="85 Run Status RT";    //wei 20170104
    EventDescription[SECS_EVENT.MapNoArmHasIC]          ="86 Map No Device Arm Has Device";                             //wei 20170104
    EventDescription[SECS_EVENT.MapHasICArmRetry]       ="87 Map Has Device Arm Error Retry";                           //wei 20170104
    EventDescription[SECS_EVENT.MapHasICArmSkip]        ="88 Map Has Device Arm Error Skip";                            //wei 20170104
    EventDescription[SECS_EVENT.PreAlarmMessage]        ="89 Pre Alarm Message";                                        //Ifor 20171024 : add Pre Alarm Position

    EventDescription[SECS_EVENT.GetTestResultAndBarcode]="90 Get TestResult And Barcode";                               //7045
    EventDescription[SECS_EVENT.SECSOffline]            ="91 SECS/GEM Offline";                                         //7045
    EventDescription[SECS_EVENT.SECSOnline]             ="92 SECS/GEM Online";  //7045
    EventDescription[SECS_EVENT.SECSOnlineRemote]       ="93 SECS/GEM Online Remote";                                   //7045
    EventDescription[SECS_EVENT.TransferBlocked]        ="94 Transfer Blocked";
    EventDescription[SECS_EVENT.CassetteLoadComplete]   ="95 Cassette Load Complete";
    EventDescription[SECS_EVENT.CassetteIDReadComplete] ="96 Cassette ID Read Complete";
    EventDescription[SECS_EVENT.ReadyToProcessComplete] ="97 Ready To Process Complete";
    EventDescription[SECS_EVENT.ReadyToCarrierOutLot]   ="98 Ready To Carrier Out Lot";
    EventDescription[SECS_EVENT.CassetteOutComplete]    ="99 Cassette Out Complete";
    EventDescription[SECS_EVENT.CassetteUnclamped]      ="100 Cassette Unclamped";
    EventDescription[SECS_EVENT.ReadyToUnload]          ="101 Ready To Unload";
    EventDescription[SECS_EVENT.UnloadComplete]         ="102 Unload Complete";
    EventDescription[SECS_EVENT.ReadyToCarrierOutTray]  ="103 Ready To Carrier Out Tray";
    EventDescription[SECS_EVENT.ReadyToCombinePass]     ="104 Ready To Combine Pass";
    EventDescription[SECS_EVENT.ReadyToCombineFail]     ="105 Ready To Combine Fail";
    EventDescription[SECS_EVENT.MachineNoStart]         ="106 Machine No Start";
    EventDescription[SECS_EVENT.ReadyToCombinePassLotEnd]="107 Ready To Combine Pass Lot End";
    EventDescription[SECS_EVENT.DoCSTLotStart]          ="108 Cassette Lot Start";
    EventDescription[SECS_EVENT.DieCountFailMessageClose]="109 Die Count Fail Message Close";
    EventDescription[SECS_EVENT.CleanOutTrayFeedFinish] ="110 Clean Out Tray Feed Finish";
    EventDescription[SECS_EVENT.MapNoICArmAutoSkip]     ="111 Map No Device Arm Auto Skip";
    EventDescription[SECS_EVENT.MRRunModeChange]        ="112 MR Run Mode Change";
    EventDescription[SECS_EVENT.AccessModeChange]       ="113 Access Mode Change";
    EventDescription[SECS_EVENT.SoftwareBin]            ="114 Software Bin";
    EventDescription[SECS_EVENT.TrayIDChange]           ="115 Tray ID Change";  //wei 20180130
    EventDescription[SECS_EVENT.ReadyToLoadNoLot]       ="116 Ready To Load No Lot";
    EventDescription[SECS_EVENT.ReadyToLoadNoTray]      ="117 Ready To Load No Tray";
    EventDescription[SECS_EVENT.ReadyToLoadNoCassette]  ="118 Ready To Load No Cassette";
    EventDescription[SECS_EVENT.ART_SRQKIND2_FTLOTSTART]  ="119 ART SRQKIND2 FT LOTSTART";
    EventDescription[SECS_EVENT.ART_SRQKIND4_RTLOTSTART]  ="120 ART SRQKIND4 RT LOTSTART";
    EventDescription[SECS_EVENT.ART_SRQKIND8_LOTEND]      ="121 ART SRQKIND6 LOTEND";
    EventDescription[SECS_EVENT.ART_SRQKIND10_FINALLOTEND]="122 FINAL LOTEND";
    EventDescription[SECS_EVENT.SafeDoorOnOff]            ="123 Safe Door On Off";
    EventDescription[SECS_EVENT.SaveRecipe]               ="124 Save Recipe";   //JerryYang 20200527 新增Save Recipe Event

    EventDescription[SECS_EVENT.EESUGOffestSelect]        ="125 EESUG Offest Select";
    EventDescription[SECS_EVENT.EESUGOffestModify]        ="126 EESUG Offest Modify";
    EventDescription[SECS_EVENT.Backtonormal]             ="127 Back To Normal";
    EventDescription[SECS_EVENT.TestStart]                ="128 Test Start";
    EventDescription[SECS_EVENT.TestFinish]               ="129 Test Finish";
    EventDescription[SECS_EVENT.MaterialReceive]          ="130 Material Receive";

    EventDescription[SECS_EVENT.SlotMapCountOK]           ="131 Slot Map Count OK";                                     //KaiChen 20200716 ：OHT
    EventDescription[SECS_EVENT.CHECK_IN]                 ="132 CHECK IN";      //KaiChen 20200716 ：OHT
    EventDescription[SECS_EVENT.CHECK_OUT]                ="133 CHECK OUT";     //KaiChen 20200716 ：OHT
    EventDescription[SECS_EVENT.ReadyToCombineFailLotEnd] ="134 Ready To Combine Fail Lot End";                         //KaiChen 20210222 ：OHT
    EventDescription[SECS_EVENT.ReadyToOHTLotEnd]         ="135 Ready To OHT Lot End";                                  //KaiChen 20210303 ：OHT

    EventDescription[SECS_EVENT.Auto1Unloadtray]          ="136 Auto 1 Unloading tray";                                 //Steven 20210716 : Auto 退盤的事件
    EventDescription[SECS_EVENT.Auto2Unloadtray]          ="137 Auto 2 Unloading tray";                                 //Steven 20210716 : Auto 退盤的事件
    EventDescription[SECS_EVENT.Auto3Unloadtray]          ="138 Auto 3 Unloading tray";                                 //Steven 20210716 : Auto 退盤的事件
    EventDescription[SECS_EVENT.DoVisualSortLotStart]     ="139 Click lot start button for visual sort mode";           //JerryYang 20220927 : for SPIL Visual sorting Lot start
    EventDescription[SECS_EVENT.PreLoadTray]              ="140 Prepare Load Tray";                                     //KenHsieh 20220923 : add Tray Map Throw IC Function
    EventDescription[SECS_EVENT.GemControlStateChange]    ="141 GEM Control State Change";                              //Ifor 20221018 add:GEM Control State Change Report

    EventDescription[SECS_EVENT.Auto4Unloadtray]          ="145 Auto 4 Unloading tray";                                 //Steven 20230907 : For HT-9011UC
    EventDescription[SECS_EVENT.Auto5Unloadtray]          ="146 Auto 5 Unloading tray";
    EventDescription[SECS_EVENT.Auto6Unloadtray]          ="147 Auto 6 Unloading tray";
    EventDescription[SECS_EVENT.Auto4Full]                ="148 Auto 4 Full";
    EventDescription[SECS_EVENT.Auto5Full]                ="149 Auto 5 Full";
    EventDescription[SECS_EVENT.Auto6Full]                ="150 Auto 6 Full";
    EventDescription[SECS_EVENT.Fix4Full]                 ="151 Fix 4 Full";
    EventDescription[SECS_EVENT.Fix5Full]                 ="152 Fix 5 Full";
    EventDescription[SECS_EVENT.Fix6Full]                 ="153 Fix 6 Full";

    EventDescription[SECS_EVENT.LoadNoTray]               ="154 Loader hasn't tray";                                    //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Loader空盤事件
    EventDescription[SECS_EVENT.LoadFullTray]             ="155 Loader full of tray";                                   //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Loader滿盤事件
    EventDescription[SECS_EVENT.LoadOnlyOneTray]          ="156 Loader only one tray";                                  //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Loader剩一盤事件
    EventDescription[SECS_EVENT.Loader_ReadyToUnload]     ="157 Loader ready to unload";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Loader 退 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Loader_FinishUnload]      ="158 Loader finish unload";                                  //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Loader 退 TRAY 事件(結束)
    EventDescription[SECS_EVENT.Empty_PreLoadTray]        ="159 Empty Prepare Load Tray";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 入料事件
    EventDescription[SECS_EVENT.EmptyOnlyOneTray]         ="160 Empty only one tray";                                   //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 剩一盤事件
    EventDescription[SECS_EVENT.EmptyNoTray]              ="161 Empty hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 空盤事件
    EventDescription[SECS_EVENT.EmptyFullTray]            ="162 Empty full of tray";                                    //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 滿盤事件
    EventDescription[SECS_EVENT.Color_PreLoadTray]        ="163 Color Prepare Load Tray";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 入料事件
    EventDescription[SECS_EVENT.ColorOnlyOneTray]         ="164 Color only one tray";                                   //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 剩一盤事件
    EventDescription[SECS_EVENT.ColorNoTray]              ="165 Color hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 空盤事件
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto1]     ="166 Empty put tray to Auto1";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto1 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto2]     ="167 Empty put tray to Auto2";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto2 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto3]     ="168 Empty put tray to Auto3";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto3 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto4]     ="169 Empty put tray to Auto4";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto1 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto5]     ="170 Empty put tray to Auto5";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto2 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutTrayToAuto6]     ="171 Empty put tray to Auto6";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto3 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto1]    ="172 Empty put cover to Auto1";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto1 上蓋事件
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto2]    ="173 Empty put cover to Auto2";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto2 上蓋事件
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto3]    ="174 Empty put cover to Auto3";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto3 上蓋事件
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto4]    ="175 Empty put cover to Auto4";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto1 上蓋事件
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto5]    ="176 Empty put cover to Auto5";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto2 上蓋事件
    EventDescription[SECS_EVENT.Empty_PutCoverToAuto6]    ="177 Empty put cover to Auto6";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 補Auto3 上蓋事件
    EventDescription[SECS_EVENT.Color_PutTrayToAuto1]     ="178 Color put tray to Auto1";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto1 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutTrayToAuto2]     ="179 Color put tray to Auto2";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto2 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutTrayToAuto3]     ="180 Color put tray to Auto3";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto3 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutTrayToAuto4]     ="181 Color put tray to Auto4";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto1 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutTrayToAuto5]     ="182 Color put tray to Auto5";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto2 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutTrayToAuto6]     ="183 Color put tray to Auto6";                               //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto3 TRAY 事件(開始)
    EventDescription[SECS_EVENT.Color_PutCoverToAuto1]    ="184 Color put cover to Auto1";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto1 上蓋事件
    EventDescription[SECS_EVENT.Color_PutCoverToAuto2]    ="185 Color put cover to Auto2";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto2 上蓋事件
    EventDescription[SECS_EVENT.Color_PutCoverToAuto3]    ="186 Color put cover to Auto3";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto3 上蓋事件
    EventDescription[SECS_EVENT.Color_PutCoverToAuto4]    ="187 Color put cover to Auto4";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto1 上蓋事件
    EventDescription[SECS_EVENT.Color_PutCoverToAuto5]    ="188 Color put cover to Auto5";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto2 上蓋事件
    EventDescription[SECS_EVENT.Color_PutCoverToAuto6]    ="189 Color put cover to Auto6";                              //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 補Auto3 上蓋事件
    EventDescription[SECS_EVENT.Auto1_LoadTrayFinish]     ="190 Auto1 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto2_LoadTrayFinish]     ="191 Auto2 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto3_LoadTrayFinish]     ="192 Auto3 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto4_LoadTrayFinish]     ="193 Auto4 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto5_LoadTrayFinish]     ="194 Auto5 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto6_LoadTrayFinish]     ="195 Auto6 load tray finish";                                //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 定位TRAY事件(結束)
    EventDescription[SECS_EVENT.Auto1_ReadyToUnload]      ="196 Auto1 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 退TRAY事件
    EventDescription[SECS_EVENT.Auto2_ReadyToUnload]      ="197 Auto2 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 退TRAY事件
    EventDescription[SECS_EVENT.Auto3_ReadyToUnload]      ="198 Auto3 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 退TRAY事件
    EventDescription[SECS_EVENT.Auto4_ReadyToUnload]      ="199 Auto4 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 退TRAY事件
    EventDescription[SECS_EVENT.Auto5_ReadyToUnload]      ="200 Auto5 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 退TRAY事件
    EventDescription[SECS_EVENT.Auto6_ReadyToUnload]      ="201 Auto6 ready to unload";                                 //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 退TRAY事件
    EventDescription[SECS_EVENT.Auto1NoTray]              ="202 Auto1 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 空盤事件
    EventDescription[SECS_EVENT.Auto2NoTray]              ="203 Auto2 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 空盤事件
    EventDescription[SECS_EVENT.Auto3NoTray]              ="204 Auto3 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 空盤事件
    EventDescription[SECS_EVENT.Auto4NoTray]              ="205 Auto4 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto1 空盤事件
    EventDescription[SECS_EVENT.Auto5NoTray]              ="206 Auto5 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto2 空盤事件
    EventDescription[SECS_EVENT.Auto6NoTray]              ="207 Auto6 hasn't tray";                                     //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Auto3 空盤事件
    EventDescription[SECS_EVENT.ColorFullTray]            ="208 Color full of tray";                                    //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 滿盤事件
    EventDescription[SECS_EVENT.TrayEndFinish]            ="209 Tray End Finish";                                       //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Tray End Finish
    EventDescription[SECS_EVENT.Empty_FinishUnload]       ="210 Empty finish unload";                                   //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Empty 退Tray 事件(結束)
    EventDescription[SECS_EVENT.Color_FinishUnload]       ="211 Color finish unload";                                   //KenHsieh 20230918 : ASEKH_K1 & K3 事件新增_Color 退Tray 事件(結束)

    EventDescription[SECS_EVENT.PowerSavingStart]         ="212 Energy Saving Start";                                   //Ifor 20240402 add:SECS/GEM Power Saving Mode
    EventDescription[SECS_EVENT.PowerSavingEnd]           ="213 Energy Saving End";                                     //Ifor 20240402 add:SECS/GEM Power Saving Mode
    EventDescription[SECS_EVENT.Reserved_03]              ="214 Reserved_03";
    EventDescription[SECS_EVENT.Reserved_04]              ="215 Reserved_04";
    EventDescription[SECS_EVENT.Reserved_05]              ="216 Reserved_05";

    EventDescription[SECS_EVENT.LoadPortStatusChanged]    ="217 LoadPortStatusChanged";
    EventDescription[SECS_EVENT.EmptyPortStatusChanged ]  ="218 EmptyPortStatusChanged";
    EventDescription[SECS_EVENT.ColorPortStatusChanged]   ="219 ColorPortStatusChanged";
    EventDescription[SECS_EVENT.Auto1PortStatusChanged]   ="220 Auto1PortStatusChanged";
    EventDescription[SECS_EVENT.Auto2PortStatusChanged]   ="221 Auto2PortStatusChanged";
    EventDescription[SECS_EVENT.Auto3PortStatusChanged]   ="222 Auto3PortStatusChanged";
    EventDescription[SECS_EVENT.Fix1PortStatusChanged]    ="223 Fix1PortStatusChanged";
    EventDescription[SECS_EVENT.Fix2PortStatusChanged]    ="224 Fix2PortStatusChanged";
    EventDescription[SECS_EVENT.Fix3PortStatusChanged]    ="225 Fix3PortStatusChanged";
    EventDescription[SECS_EVENT.Auto4PortStatusChanged]   ="226 Auto4PortStatusChanged";
    EventDescription[SECS_EVENT.Auto5PortStatusChanged]   ="227 Auto5PortStatusChanged";
    EventDescription[SECS_EVENT.Auto6PortStatusChanged]   ="228 Auto6PortStatusChanged";
    EventDescription[SECS_EVENT.Fix4PortStatusChanged]    ="229 Fix4PortStatusChanged";
    EventDescription[SECS_EVENT.Fix5PortStatusChanged]    ="230 Fix5PortStatusChanged";
    EventDescription[SECS_EVENT.Fix6PortStatusChanged]    ="231 Fix6PortStatusChanged";
    EventDescription[SECS_EVENT.Reserved_21]              ="232 Reserved_21";
    EventDescription[SECS_EVENT.Reserved_22]              ="233 Reserved_22";
    EventDescription[SECS_EVENT.SafetyDoorOpen]           ="234 SafetyDoorOpen";
    EventDescription[SECS_EVENT.SafetyDoorClosed]         ="235 SafetyDoorClosed";
    EventDescription[SECS_EVENT.LoadPortBundleArrived]    ="236 LoadPortBundleArrived";
    EventDescription[SECS_EVENT.LoadPortBundleRead]       ="237 LoadPortBundleRead";
    EventDescription[SECS_EVENT.RemoteStart]              ="238 RemoteStart]";
    EventDescription[SECS_EVENT.UnexpectedBundleIDRead]   ="239 UnexpectedBundleIDRead";
    EventDescription[SECS_EVENT.UnexpectedUNITIDRead]     ="240 UnexpectedUNITIDRead";
    EventDescription[SECS_EVENT.BundleCompleteProcessed]  ="241 BundleCompleteProcessed";
    EventDescription[SECS_EVENT.BundleCompleteIDRead]     ="242 BundleCompleteIDRead";
    EventDescription[SECS_EVENT.NoCoverTray_2DID]         ="243 NoCoverTray_2DID";
    EventDescription[SECS_EVENT.NoCoverTray_Normal]       ="244 NoCoverTray_Normal";
    EventDescription[SECS_EVENT.BundleEnd_Auto1]          ="245 BundleEnd_Auto1";                                       //Auto1
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto1]   ="246 BundleEnd_IDREAD_Auto1";
    EventDescription[SECS_EVENT.BundleEnd_Auto2]          ="247 BundleEnd_Auto2";                                       //Auto2
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto2]   ="248 BundleEnd_IDREAD_Auto2";
    EventDescription[SECS_EVENT.ProcessEnd]               ="249 ProcessEnd";    //Process end
    EventDescription[SECS_EVENT.DoStartAutoHeight]        ="250 START Auto contact height";                             //Ifor 20240311 add: START Auto contact height
    EventDescription[SECS_EVENT.BundleEnd_Auto3]          ="251 BundleEnd_Auto3";                                       //Auto3
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto3]   ="252 BundleEnd_IDREAD_Auto3";
    EventDescription[SECS_EVENT.BundleEnd_Auto4]          ="253 BundleEnd_Auto4";                                       //Auto4
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto4]   ="254 BundleEnd_IDREAD_Auto4";
    EventDescription[SECS_EVENT.BundleEnd_Auto5]          ="255 BundleEnd_Auto5";                                       //Auto5
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto5]   ="256 BundleEnd_IDREAD_Auto5";
    EventDescription[SECS_EVENT.BundleEnd_Auto6]          ="257 BundleEnd_Auto6";                                       //Auto6
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Auto6]   ="258 BundleEnd_IDREAD_Auto6";
    EventDescription[SECS_EVENT.BundleEnd_Fix1]           ="259 BundleEnd_Fix1";                                        //Fix1
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix1]    ="260 BundleEnd_IDREAD_Fix1";
    EventDescription[SECS_EVENT.BundleEnd_Fix2]           ="261 BundleEnd_Fix2";                                        //Fix2
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix2]    ="262 BundleEnd_IDREAD_Fix2";
    EventDescription[SECS_EVENT.BundleEnd_Fix3]           ="263 BundleEnd_Fix3";                                        //Fix3
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix3]    ="264 BundleEnd_IDREAD_Fix3";
    EventDescription[SECS_EVENT.BundleEnd_Fix4]           ="265 BundleEnd_Fix4";                                        //Fix4
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix4]    ="266 BundleEnd_IDREAD_Fix4";
    EventDescription[SECS_EVENT.BundleEnd_Fix5]           ="267 BundleEnd_Fix5";                                        //Fix5
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix5]    ="268 BundleEnd_IDREAD_Fix5";
    EventDescription[SECS_EVENT.BundleEnd_Fix6]           ="269 BundleEnd_Fix6";                                        //Fix6
    EventDescription[SECS_EVENT.BundleEnd_IDREAD_Fix6]    ="270 BundleEnd_IDREAD_Fix6";
    EventDescription[SECS_EVENT.LoaderTrayState]            ="271 LoaderTrayState";
    EventDescription[SECS_EVENT.AGVSupplement]              ="272 AGVSupplement";
    EventDescription[SECS_EVENT.AGVLDUnLDStatus]            ="273 AGVLDUnLDStatus";
    EventDescription[SECS_EVENT.SECSGEMConsecutiveFailure]  ="274 SECSGEMConsecutiveFailure";
    EventDescription[SECS_EVENT.Loader_Buffer_HasTray]      ="275 Loader_Buffer_HasTray";                               //Sam 20250605 : Loader buffer status change event
    EventDescription[SECS_EVENT.Loader_Buffer_NoTray]       ="276 Loader_Buffer_NoTray";
    EventDescription[SECS_EVENT.OutputPort1BinCode]         ="277 OutputPort1BinCode";
    EventDescription[SECS_EVENT.OutputPort2BinCode]         ="278 OutputPort2BinCode";
    EventDescription[SECS_EVENT.OutputPort3BinCode]         ="279 OutputPort3BinCode";
    EventDescription[SECS_EVENT.OutputPort4BinCode]         ="280 OutputPort4BinCode";
    EventDescription[SECS_EVENT.OutputPort5BinCode]         ="281 OutputPort5BinCode";
    EventDescription[SECS_EVENT.OutputPort6BinCode]         ="282 OutputPort6BinCode";
    EventDescription[SECS_EVENT.MaterialModeChange]         ="283 MaterialModeChange";
    EventDescription[SECS_EVENT.PortStateUpdated]           ="284 PortStateUpdated";
    EventDescription[SECS_EVENT.UnloaderTrayIDReadOK]       ="285 UnloaderTrayIDReadOK";
    EventDescription[SECS_EVENT.UnloaderTrayIDReadFail]     ="286 UnloaderTrayIDReadFail";
    EventDescription[SECS_EVENT.LoaderTrayIDReadFail]       ="287 LoaderTrayIDReadFail";
    EventDescription[SECS_EVENT.MaximumOutputPortReport]    ="288 MaximumOutputPortReport";
    EventDescription[SECS_EVENT.RunCheckRequest]            ="289 RunCheckRequest";//Eastsun 20260515 F018
    EventDescription[SECS_EVENT.AGVLDUnLDFinish]            ="290 AGVLDUnLDFinish";//Eastsun 20260512 F009 KYEC AMR LDUnLD Finish
    EventDescription[SECS_EVENT.AGVLdID]                    ="291 AGVLdID";//Eastsun 20260513 F016 KYEC AMR cover-tray ID
    EventDescription[SECS_EVENT.DoSecsGemIndexFail]         ="292 DoSecsGemIndexFail";//Eastsun 20260515 F018 KYEC Index Fail
};
//------------------------------------------------------------------------------
void HT9045Gem::ReloadParameter()
{
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 2018905 add
        SaveAllFile(GetLastOpenFN());                                           //kevin 20181114

    fMain->LoadTestModePicture();
    fMain->UpdateMainOperateMode();
    fMain->LoadRunModePicture();
    fMain->LoadStartModePicture();
}
//------------------------------------------------------------------------------
void HT9045Gem::AddAlarmList()
{
    if(CosFunction.bEnable_SECS_GEM==true)                                      //Ifor 20220719 add 有SECS GEM 功能即載入資料避免後續開啟造成異常
    {
        AnsiString Code;
        int AlarmCount;

//Steven 20200331 : Alarm code list改用文字檔
//==>
//        asQuery="SELECT  ID_AlarmList,      "
//                "        AlarmCode,         "
//                "        UnitNo,            "
//                "        UnitName.UnitName, "
//                "        Type,              "
//                "        Message            "
//                "FROM AlarmList             "
//                "INNER JOIN UnitName ON     "
//                "(AlarmList.UnitNo = UnitName.ID_UnitNo)";
//        MyDBVProcess(asQuery, HGem->strGrdAlarm);

        GetAlarmCodeList(HGem->strGrdAlarm);
        HGem->strGrdAlarm->ColWidths[7]=50;                                     //Enable
        HGem->strGrdAlarm->Cells[7][0]="Enable";
        HGem->strGrdAlarm->Cells[8][0]="AlarmID";
        HGem->strGrdAlarm->Cells[9][0]="Class";
        HGem->strGrdAlarm->Cells[10][0]="Position";

        AlarmCount=HGem->strGrdAlarm->RowCount;
//<==
//Steven 20200331 : Alarm code list改用文字檔

        HGem->ReadAlamData();

        for(int i=1; i<AlarmCount; i++)                                         //20140213  wei   KYEC SECS/GEM
        {
            int iType, iPos;
            int iAlarmCode;

            iType=atoi(HGem->strGrdAlarm->Cells[5][i].c_str());
            if(iType<0)
                iType=0;

            iPos=atoi(HGem->strGrdAlarm->Cells[3][i].c_str());
            iAlarmCode=atoi(HGem->strGrdAlarm->Cells[2][i].SubString(4, HGem->strGrdAlarm->Cells[2][i].Length()).c_str());

            Code.sprintf("%d%02d%06d", iType, iPos, iAlarmCode);

            HGem->SetAlamData(i, Code, AnsiString(iType), HGem->strGrdAlarm->Cells[6][i], HGem->strGrdAlarm->Cells[4][i]);
        }

        HGemPtr->WriteAlamData();
        HGemPtr->ReadEventReportData();
    }
}
//------------------------------------------------------------------------------
void HT9045Gem::AddCEID()
{
    if(CosFunction.bEnable_SECS_GEM==true)                                      //Ifor 20220719 add 有SECS GEM 功能即載入資料避免後續開啟造成異常
    {
        int EquDefault=1;
        for(int i=SECS_EVENT.DoStart; i<SECS_EVENT.TotalEvent; i++)
        {
            unsigned CEDIDContent[]={i};
            HGemPtr->SetCEIDContent(i, EventDescription[i], sizeof(CEDIDContent)/sizeof(unsigned), CEDIDContent, EquDefault);                                   // Ceid=1 ,有 1個 ReportID ,分別是 1
        }
    }
}
//------------------------------------------------------------------------------
void HT9045Gem::AddReprot()
{
    if(CosFunction.bEnable_SECS_GEM==true)                                      //Ifor 20220719 add 有SECS GEM 功能即載入資料避免後續開啟造成異常
    {
        int EquDefault=1;
        unsigned ReportIDContent[]={1027};                                      //System Time

        for(int i=SECS_EVENT.DoStart; i<SECS_EVENT.TotalEvent; i++)
        {
            HGemPtr->SetReportIDContent(1, sizeof(ReportIDContent)/sizeof(unsigned), ReportIDContent, EquDefault);
        }

        HGem->SaveEventReportData();
    }
}
//------------------------------------------------------------------------------
void HT9045Gem::LookForFile()
{
    AnsiString Str=fMain->cbSetupFileName->Text;
    fMain->cbSetupFileName->Clear();
    fMain->LookForFile();
    fMain->cbSetupFileName->Text=Str;
}
//==============================================================================
//  V 1.0
//     ret
//     0   = Acknowledge
//     1   = Denied. At least one constant does not exist
//     2   = Denied. Busy
//     3   = Denied. At least one constant out of range
//    >3   = Other equipment-specific error
//    4-63   Reserved
//==============================================================================
int HT9045Gem::S2F15_CheckNewEquipmentConstant()                                //wei 20170417 (Steven) add S2F15
{
    int EClen, i, len, ret=0;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64 uint8EC;                                                   //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC;
    AnsiString ECID;
    int iECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
//    void *P;

    /*
        L,n
        1. L,2
        1. <ECID1>
        2. <ECV1>
        2. L,2
        .
        .
        n. L,2
        1. <ECIDn>
        2. <ECVn>
    */

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)!=1)
        return -1;
    if(Type!=HType.LIST_TYPE)
        return -1;
    if(EClen<1)
        return -1;
    for(i=0; i<EClen; i++)
    {
        if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)!=1)
            return -1;
        if(Type!=HType.LIST_TYPE || len!=2)
            return -1;

        ret=1;
        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(HGem->DataItemIn(len, Type, ECID)!=1)
                return -1;
        }
        else
        {
            return -1;
        }

        iECID=atoi(ECID.c_str());

        if(iECID==1530 || iECID==1531)
        {
            if((LastSet.iTemperature==Tempture_Hot && fMain->CanChangeSite(true)==false) ||
               (LastSet.iTemperature!=Tempture_Hot && fMain->CanChangeSite(false)==false))
            {
                return 2;
            }
        }
        else if(iECID==1513)
        {
            if(BAR_CODE_INSTALL!=ebctUninstall &&
               (TestIF_File.bEnableBarCode ||
               (BOTTOM_2DID && TestIF_File.bEnableBottom2D)) &&
                TestIF_File.bSortingBy2DIDList==true)                           //Steven 20190422 : Bottom 2DID
            {
            }
            else
            {
                return -1;
            }
        }

        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(Type==HType.UINT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                    ret=CheckECValue(ECID, &uint1EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                    ret=CheckECValue(ECID, &uint2EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                    ret=CheckECValue(ECID, &uint4EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                    ret=CheckECValue(ECID, &uint8EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int2EC)==1)
                    ret=CheckECValue(ECID, &int2EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int4EC)==1)
                    ret=CheckECValue(ECID, &int4EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &int8EC)==1)
                    ret=CheckECValue(ECID, &int8EC);
                else
                    return -1;
            }
            else if(Type==HType.ASCII_TYPE)
            {
                char *Str;
                Str=new char [len+100];
                if(HGem->DataItemIn(len, Type, Str)==1)
                {
                    ret=CheckECValue(ECID, Str);
                }
                else
                {
//                    delete[] Str;                                             //Steven 20160912 : Add delete for save memory, 先mark
                    return -1;
                }
            }
            else if(Type==HType.BINARY_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &floatEC)==1)
                    ret=CheckECValue(ECID, &floatEC);
                else
                    return -1;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                {
                    ret=CheckECValue(ECID, &doubleEC);
                }
                else
                {
                    return -1;
                }
            }
            else                                                                // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
            {
                HGem->SendInvalidDataMessageToHost("error format");
                return -1;
            }

            if(ret!=0)
            {
                return ret;
            }
        }
        else
        {
            HGem->SendInvalidDataMessageToHost("GetDataItemLenAndType Error");
            return -1;
        }
    }

    if(iECID>=37800 && iECID<=37887)                                            //kevin 20181206  20180914 (Steven) : add read ESD data D:\ESD_Program\System\General.ini
    {
        ReadESDDataFile();
    }
    return 0;
}
//------------------------------------------------------------------------------
int HT9045Gem::S2F15_UpdateNewEquipmentConstant()
{
    int EClen, i, x, y, len, z;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64 uint8EC;                                                   //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC, ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
    AnsiString S, FileName="", SitMapData="", SitMapDataBuffer="", SData="", sIni="";                                   //kevin 20181107 add
//    int iArry[64]={0};
//    void *P;
    bool b151920=false;
    bool bspeed=false;                                                          //kevin 20181210 add speed
    FileName=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString str;

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            for(i=0; i<EClen; i++)
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)==1)
                {
                    if(Type!=HType.LIST_TYPE || len!=2)
                        break;
                    // get ECID

                    HGem->GetDataItemLenAndType(len, Type);
                    HGem->DataItemIn(len, Type, S);
                    ECID=atoi(S.c_str());

                    HGem->GetDataItemLenAndType(len, Type);

                    if(Type==HType.UINT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                            SetECValue(ECID, &uint1EC);
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                            SetECValue(ECID, &uint2EC);
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                            SetECValue(ECID, &uint4EC);
                    }
                    else if(Type==HType.UINT_8_TYPE)                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                            SetECValue(ECID, &uint8EC);
                    }
                    else if(Type==HType.INT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int2EC)==1)
                            SetECValue(ECID, &int2EC);
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int4EC)==1)
                            SetECValue(ECID, &int4EC);
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &int8EC)==1)
                            SetECValue(ECID, &int8EC);
                    }
                    else if(Type==HType.ASCII_TYPE)
                    {
                        char *Str;
                        Str=new char [len+100];
                        if(HGem->DataItemIn(len, Type, Str)==1)
                        {
                            SetECValue(ECID, Str);
                            SData= Str;                                         //kevin 20181116 add
                        }

                        if(ECID==3540)                                          //kevin 20181107 add  SITE MAP
                        {
                            SitMapData=Str;
                            SitMapDataBuffer=Str;
                        }
                        delete[] Str;                                           //kevin 20181116 add
                    }
                    else if(Type==HType.BINARY_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.BOOLEAN_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                        {
//                            if(int1EC==0x00)                                  //Steven 20230815 : 將ASCII轉成CHAR避免0x00或0x01造成不可視字元
//                                int1EC='0';
//                            else
//                                int1EC='1';
                            SetECValue(ECID, &int1EC);
                        }
                    }
                    else if(Type==HType.FT_4_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &floatEC)==1)
                            SetECValue(ECID, &floatEC);
                    }
                    else if(Type==HType.FT_8_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                            SetECValue(ECID, &doubleEC);
                    }
                    else                                                        // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
                    {
                        HGem->SendInvalidDataMessageToHost("error format");
                        return -1;
                    }
                    int ia=0, iArry[40];
                    int temp, k=0;
                    AnsiString sPath="";                                        //JerryYang 20250613 Add

                    switch(ECID)
                    {
                        case 1006:
                            if(CUSTOMER_CODE==CC_SJ_Semiconductor_OS)
                            {
                                if(TestIF.iTestType==TCP_IP_MODE)                                                       //Steven 20230213 : For SJSemi OS Tester
                                {
                                    if(iRunStartMode==RT)
                                        str.sprintf("LOTNUMBER,%s,RT,", fLotInfo->edtSysLotID->Text);
                                    else
                                        str.sprintf("LOTNUMBER,%s,FT,", fLotInfo->edtSysLotID->Text);
                                    fTesterTCP->SendTCPIPCommand(0, "LOTNUMBER", str);
                                    fTesterTCP->SendTCPIPCommand(0, "LOTSTART", "LOTSTART");
                                    fTesterTCP->SendTCPIPCommand(0, "Get OS Setup", "GETOSSETUP");                      //Steven 20230505 : 取得OS Tester資訊
                                }
                            }
                            break;
                        case 1007:
                            if(CUSTOMER_CODE==CC_SJ_Semiconductor_OS)
                            {
                                if(TestIF.iTestType==TCP_IP_MODE)               //Steven 20230213 : For SJSemi OS Tester
                                {
                                    str.sprintf("OPERATORID,%s,", fLotInfo->edtSysOperatorID->Text);
                                    fTesterTCP->SendTCPIPCommand(0, "OPERATORID", str);
                                }
                            }
                            break;
                        case 1513:
                            if(BAR_CODE_INSTALL!=ebctUninstall &&
                               (TestIF_File.bEnableBarCode ||                   //Ifor 20190129 : add Cognex EtherNet 通訊
                               (BOTTOM_2DID && TestIF_File.bEnableBottom2D)) &&
                                TestIF_File.bSortingBy2DIDList==true)           //Steven 20190422 : Bottom 2DID
                            {
                                fMain->ChangeTesterConnect(LastSet.iTester, false, true);
                                fMain->LoadRunModePicture();
                                fMain->SetStartModeData();
                            }
                            else
                            {
                                return -1;
                            }
                            break;
                        case 1514:
                            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                 //kevin 20181116 add
                            {
                                if(LastSet.iTemperature==0)
                                    Temperature.iMachineTempMode=1;             //Ambient
                                else if(LastSet.iTemperature==1)
                                    Temperature.iMachineTempMode=0;             //Hot;
                                else if(LastSet.iTemperature==3)
                                    Temperature.iMachineTempMode=3;             //Ambient_HOT
                            }
                            break;
                        case 1519:                                              //kevin 20181206 add
                            fMain->edWorkTemperBase->Text=doubleEC;
                            b151920=true;
                            break;
                        case 1520:
                            fMain->edSoakTime->Text=doubleEC;
                            b151920=true;
                            break;
                        case 1530:
                            for(x=0; x<MAX_SOCKET_ROW; x++)
                            {
                                for(y=0; y<MAX_SOCKET_COL; y++)
                                {
                                    z=x*MAX_SOCKET_COL+y;
                                    if(z<fMain->tSiteOnOff[0]->Count)
                                        LastSet.bUseTestSocket[0][x][y]=atoi(fMain->tSiteOnOff[0]->Strings[z].c_str());
                                    else
                                        LastSet.bUseTestSocket[0][x][y]=false;
                                    bTestSiteUse[0][x][y]=LastSet.bUseTestSocket[0][x][y];                              //wei 20170417 (Steven) 顯示問題
                                    if(IniConfig.bA09_ByArmCloseSite==false)
                                    {
                                        LastSet.bUseTestSocket[1][x][y]=LastSet.bUseTestSocket[0][x][y];
                                        bTestSiteUse[1][x][y]=LastSet.bUseTestSocket[0][x][y];
                                    }
                                }
                            }
                            fMain->ShowTestHeadComp(false);
                            break;
                        case 1531:
                            for(x=0; x<MAX_SOCKET_ROW; x++)
                            {
                                for(y=0; y<MAX_SOCKET_COL; y++)                                                         //Eliot 2009_12_25 4->8
                                {
                                    z=x*MAX_SOCKET_COL+y;
                                    if(z<fMain->tSiteOnOff[1]->Count)
                                        LastSet.bUseTestSocket[1][x][y]=atoi(fMain->tSiteOnOff[1]->Strings[z].c_str());
                                    else
                                        LastSet.bUseTestSocket[1][x][y]=false;

                                    bTestSiteUse[1][x][y]=LastSet.bUseTestSocket[1][x][y];                              //wei 20170417 (Steven) 顯示問題
                                }
                            }
                            fMain->ShowTestHeadComp(false);
                            break;
                        case 1701:                                              //JerryYang 20250320 : add ECID
                            sIni=GetLastOpenFN();
                            szDir.sprintf("%s%s", DataPath, sIni);
                            MyForceDirectories(szDir);
                            szDir+="\\HandlerCondition.Data";

                            WriteIniData(szDir, "Configuration", "Check 2DID Allow List Function", int1EC);
                            fBarCode->ReadFile();
                            break;
                        case 2501:                                              //Input Arm vacuum wait time
                        case 2502:                                              //Index Arm vacuum wait time
                        case 2503:                                              //Output Arm vacuum wait time
                        case 2504:                                              //Tray Arm vacuum wait time
                        case 2511:                                              //Input Arm destroy wait time
                        case 2512:                                              //Index Arm destroy wait time
                        case 2513:                                              //Output Arm destroy wait time
                        case 2514:                                              //Tray Arm destroy wait time
                        case 8501:                                              //In Arm Speed
                        case 8502:                                              //Shuttle Speed
                        case 8503:                                              //Index Arm Speed
                        case 8504:                                              //Out Arm Speed
                        case 8505:                                              //Tray Arm Speed
                        case 8506:                                              //Elevator Speed
                        case 8507:                                              //Shuttle 2 Speed
                        case 8508:                                              //In Rotate Speed
                        case 8509:                                              //Out Rotate Speed
                        case 8510:                                              //In Arm Z Speed
                        case 8511:                                              //In Arm X Pitch Speed
                        case 8512:                                              //In Arm Y Pitch Speed
                        case 8513:                                              //Out Arm Z Speed
                        case 8514:                                              //Out Arm X Pitch Speed
                        case 8515:                                              //Out Arm Y Pitch Speed
                        case 8527:                                              //Step Shuttle
                        case 8528:                                              //Tray Arm Retry Count
                        case 8529:                                              //Tray Arm Head Down Time
                        case 8530:                                              //Auto Speed
                        case 8534:                                              //Index Retry Count
                        case 8535:                                              //Index Retry Down Distance
                        case 8536:                                              //Index Destroy Again Interval Time
                        case 8537:                                              //Index Destroy Again Count
                        case 8538:                                              //Socket Device Floating Check
                        case 8540:                                              //In Arm ADC Speed
                        case 8541:                                              //Shuttle ADC Speed
                        case 8542:                                              //Index Arm ADC Speed
                        case 8543:                                              //Out Arm ADC Speed
                        case 8544:                                              //Tray Arm ADC Speed
                        case 8545:                                              //Elevator ADC Speed
                        case 8546:                                              //Shuttle 2 ADC Speed
                        case 8547:                                              //In Rotate ADC Speed
                        case 8548:                                              //Out Rotate ADC Speed
                        case 8549:                                              //In Arm Z ADC Speed
                        case 8550:                                              //In Arm X Pitch ADC Speed
                        case 8551:                                              //In Arm Y Pitch ADC Speed
                        case 8552:                                              //Out Arm Z ADC Speed
                        case 8553:                                              //Out Arm X Pitch ADC Speed
                        case 8560:                                              //In Arm Retry Count
                        case 8561:                                              //In Arm Retry Down Distance
                        case 8562:                                              //In Arm Destroy Again Interval Time
                        case 8563:                                              //In Arm Destroy Again Count
                        case 8564:                                              //In Arm Pitch Function
                        case 8565:                                              //In Arm Destroy Check Time
                        case 8566:                                              //In Arm Destroy Check Need Pause
                        case 8567:                                              //In Arm Two Speed Move Down
                        case 8568:                                              //In Arm Two Speed Move Down Speed
                        case 8569:                                              //In Arm Two Speed Move Down ADC
                        case 8570:                                              //In Arm Two Speed Move Down Distance
                        case 8571:                                              //In Arm Auto Skip
                        case 8572:                                              //In Arm Auto Skip Count
                        case 8573:                                              //In Arm Wait On Shuttle Time
                        case 8574:                                              //Out Arm Retry Count
                        case 8575:                                              //Out Arm Retry Down Distance
                        case 8576:                                              //Out Arm Destroy Again Interval Time
                        case 8577:                                              //Out Arm Destroy Again Count
                        case 8578:                                              //Out Arm Pitch Function
                        case 8579:                                              //Out Arm Destroy Check Time
                        case 8580:                                              //Out Arm Destroy Check Need Pause
                        case 8581:                                              //Out Arm Two Speed Move Down
                        case 8582:                                              //Out Arm Two Speed Move Down Speed
                        case 8583:                                              //Out Arm Two Speed Move Down ADC
                        case 8584:                                              //Out Arm Two Speed Move Down Distance
                            bspeed=true;                                        //kevin 20181210 add speed
                            szDir.sprintf("%s%s", DataPath, FileName);
                            MyForceDirectories(szDir);
                            break;
                        case 2622:
                            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                 //kevin 20181116 add
                            {
                                DeviceForm_File.ForcePerPinN=DeviceForm_File.ForcePerPinG/1000.0*9.8;
                            }
                            break;
                        case 3616:
                            fBinSel->Save(3616, eBinFT);                        //kevin 20181120       //error bin
                            break;
                        case 3617:
                            fBinSel->Save(3617, eBinFT);                        //kevin 20181120
                            break;
                        case 3677:
                            fBinSel->Save(3677, eBinFT);                        //kevin 20181120
                            break;
                        case 35032:                                             //JerryYang 20250613 Add
                            sPath=AuthPath+"config.ini";
                            WriteIniData(sPath, "Function", "bA68_AutoLoadUnload", IniConfig.bA68_AutoLoadUnload);
                            break;
                        case 3540:                                                                                      //kevin 20181107 add sitemap
                            szDir.sprintf("%s%s", DataPath, S);
                            MyForceDirectories(szDir);
                            szDir+="\\HandlerCondition.Data";

                            string sData= SitMapData.c_str();
                            std::replace(sData.begin(), sData.end(), ',', ' ');                                         // replace ':' by ' '
                            stringstream ss(sData);

                            while(ss >> temp)
                            {
                                iArry[k]=temp;
                                k++;
                            }
                            for(int i=0; i<MAX_SOCKET_ROW; i++)
                            {
                                for(int j=0; j<MAX_SOCKET_COL; j++)
                                {
                                    TestIF_File.iSiteMap[i][j]= iArry[ia];
                                    ia++;
                                    str.sprintf("Site %c%c", i+'A', j+'a');                                             //kevin 20180312 j+'A' ==> j+'a'
                                    WriteIniData(szDir, "Configuration", str, TestIF_File.iSiteMap[i][j]);
                                }
                            }

                            if(HasICUnderMachine()==false)                                                              //kevin 20181108有開SITE 才恢復畫面關SITE -> 開SITE
                                fMain->ShowTestHeadComp(true);
                            else
                                fMain->ShowTestHeadComp(false);
                            break;
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    if(b151920)
    {
        bRefreshFunction=true;                                                  //kevin 20181101
        fMain->SetTemp(true, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));
    }

    if(bspeed)                                                                  //kevin 20181210 add speed
    {
        fSpeed->DoIniDataToForm();
        //fSpeed->SaveSetupFile(szDir);
    }

    ReloadParameter();
    if(ECID>=4801 && ECID<=4832)                                                //kevin 20180914 (Steven) : add read ESD data
    {
        fTemp_Set->DoIniDataToForm(true);                                       //Steven 20110930 : 得在fSetup後面
        fBuilder->bSaveAllFillOrFile("Temperature",0);
    }
    else if(ECID>=37800 && ECID<=37887)                                         //kevin 20181206 //kevin 20180914 add read ESD data
    {
        WriteESDDataFile();
        SendCommand_ESD(ESD_Read_ESD_SettDATA);                                 //kevin 20180914
    }
    else if(ECID==3545)
    {
        fSetup->ScrollBar1Change(NULL);
        fSetup->DoIniDataToForm();
        fSetup->sbUpdateClick(NULL);
    }
    return 0;
}
//------------------------------------------------------------------------------
// CheckAndExecuteAsyncRCMD()
// 說明：供機台進入 Idle（SystemStart==false 且 !HasICUnderMachine()）後呼叫。
//       若有先前以 HCACK=4 接受的非同步指令，則在此執行並透過 S6F11 通知主機完成。
// 呼叫時機：建議放在 Stop() / OneCycleFinish / Idle 狀態切換點。（Ifor 20260423）
//------------------------------------------------------------------------------
void HT9045Gem::CheckAndExecuteAsyncRCMD()
{
    if(!m_AsyncRCMD.bPending)
        return;
    if(SystemStart || HasICUnderMachine())
        return;

    m_AsyncRCMD.bPending = false;

    // --- PP_SELECT 非同步：等待機台 Idle 後切換工作檔 ---
    if(m_AsyncRCMD.sCommand.AnsiPos("PP_SELECT") == 1 ||
       m_AsyncRCMD.sCommand.AnsiPos("PP-SELECT") == 1)
    {
        AnsiString PPID = m_AsyncRCMD.sParam;
        bool bHasFile = false;
        for(int i = 0; i < fMain->cbSetupFileName->Items->Count; i++)
        {
            if(PPID != "" && PPID == fMain->cbSetupFileName->Items->Strings[i])
                bHasFile = true;
        }

        if(bHasFile)
        {
            fMain->cbSetupFileName->Text = PPID;
            fMain->cbSetupFileNameChange(fMain);
            // 以 S6F11（CEID=SwitchSetupFile）通知主機非同步指令已完成
            HGemPtr->EventReport(1, SECS_EVENT.SwitchSetupFile);
            RecordProcess("[S2F42 Async] PP_SELECT executed: " + PPID);
        }
    }
    // --- 其他非同步指令可在此繼續 else if 擴充 ---
}
//------------------------------------------------------------------------------
extern bool ProcessLotInfo(AnsiString S2);
extern int S2F42_SET_2DID_BIN_CODE(AnsiString s2DID_Source);
int HT9045Gem::S2F42_Host_Command_Acknowledge()
{
    unsigned char HCACK=1;
//    char CommandStr[1024];
    char *CommandStr;
//    CommandStr=new char [10000000];                                           //JerryYang 20241205 : 字串陣列大小改成動態

    char str[256];
    AnsiString PPID, sLotNo, sLOTCount, str2="";
    int ret, SVlen, i, j, len, iHead, iArm, iLoadPort=0;
    double dOffset=0.0;
    bool bHasFile;
    unsigned char Type;
    AnsiString S, S1, S2, S3, sLotInfo, strLotID, sLotCnt, str1, sDCC, sOP, sTrayID, sYield, sHBin, sTrayQty;
//    ZeroMemory(CommandStr, sizeof(CommandStr));
    ZeroMemory(str, sizeof(str));

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)                        // 需要補充多重 Command
    {
        HGemPtr->GetDataItemLenAndType(len, Type);
        CommandStr=new char [len+1];                                            //JerryYang 20241205 : 字串陣列大小改成動態
//        ZeroMemory(CommandStr, sizeof(CommandStr));
        memset(CommandStr, 0, len+1);                                           //JerryYang 20250325 : fix

        if(Type==HType.ASCII_TYPE)
        {
            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
            if(ret==-1)
                S="";
        }
        else
        {
            S="";
        }
        S=CommandStr;
        S=S.UpperCase();

        delete[] CommandStr;                                                    //Steven 20160912 : Add delete for save memory
        CommandStr=NULL;

        if(S.AnsiPos("CANCEL_INPUT_TRAY")==1)                                   //Command to discharge input tray
        {
            HCACK=0;
        }
        else if(S.AnsiPos("TRY_RFID_READ")==1)                                  //Command to retry reading input cover tray ID
        {
            HCACK=0;                                                        //Steven 20260415 : stub, acknowledge
        }
        else if(CUSTOMER_CODE==CC_AMKOR_Korea &&
                S.AnsiPos("LOT_START")==1)                                      //Command to process loaded tray
        {
//        <L:2
//            <A:9 LOT_START>
//            <L:8
//                <L:2
//                    <A:6 LOT_NO>
//                    <A:38 AZ1FC5337LT-C5337M4.0101#SL3@k3tv93368>
//                >
//                <L:2
//                    <A:3 DCC>
//                    <A:0>
//                >
//                <L:2
//                    <A:14 OPERATION_CODE>
//                    <A:4 7582>
//                >
//                <L:2
//                    <A:8 UNIT_QTY>
//                    <A:4 5849>
//                >
//                <L:2
//                    <A:7 TRAY_ID>
//                    <A:9 RT0000658>
//                >
//                <L:2
//                    <A:9 LOT_YIELD>
//                    <A:4 99.5>
//                >
//                <L:2
//                    <A:13 HARD_BIN_INFO>
//                    <A:56 BIN01,BIN02,BIN03,BIN04,BIN05,BIN06,BIN22,CM,CV,OS,OTHER>
//                >
//                <L:2
//                    <A:15 OUTPUT_TRAY_QTY>
//                    <A:2 32>
//                >
//            >
//        >

//            unsigned char  uint1EC;
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)                                         //wei 20150630
            {
                HCACK=0;
                for(i=0; i<SVlen; i++)
                {
                    if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                    {
                        HGem->GetDataItemLenAndType(len, Type);
                        HGem->DataItemIn(len, Type, str);
                        S1=str;
                        if(S1=="LOT_NO")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                strLotID=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="DCC")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sDCC=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="OPERATION_CODE")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sOP=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="UNIT_QTY")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sLotCnt=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="TRAY_ID")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sTrayID=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="LOT_YIELD")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sYield=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="HARD_BIN_INFO")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sHBin=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else if(S1=="OUTPUT_TRAY_QTY")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                sTrayQty=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;
                            }
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                    else
                    {
                        HCACK=2;
                    }
                }

                if(HCACK==0)
                {
//                    fLotInfo->edtSysLotID->Text     =strLotID;                  //RogerYang 20260401 : add
//                    fLotInfo->edtDevice->Text       =sDCC;
//                    sDCC_ATK                        =sDCC;
                    fLotInfo->edtSysOperatorID->Text=sOP;                       //RogerYang 20260401 : add
                    fLotInfo->cbRunMode->Text       =strLotID;                  //RogerYang 20260401 : add
//                    fLotInfo->lbledtTestBinNo->Text =sHBin;
//                    fSCKART->iLotCount              =atoi(sLotCnt.c_str());

//                    int iLotCnt=atoi(sLotCnt.c_str());
//                    fSCKART->DoARTLotStart(strLotID, "", iLotCnt);
                    fLotInfo->sbSECSLotStartClick(fLotInfo);
                    RecordProcess("SECS/GEM LOTSTART!");
                    fAGV->bATK_AMR_DoHostLotStart=true;                         //RogerYang 20260401 : add
                    if(fAGV->bATKAMR_GET_LOTORDER0_Ready==true)
                    {
                        LastSet.bWaitStartLotAutoRetestGPIB=true;
                        fAGV->bATKAMR_GET_LOTORDER0_Ready=false;
                    }

                    if(SystemStart==false)
                    {
                        fMain->Start("SECS GEM RCMD : START");
//                        HCACK=0;
                    }
                    else
                    {
//                        HCACK=3;                                              //RogerYang 20260402 : no return 3 when start
                    }
                    HCACK=0;
                }
            }
        }
        else if(CUSTOMER_CODE==CC_AMKOR_Korea &&
                S.AnsiPos("LOT_END")==1)                                        //End signal command to discharge all trays after Lot processing
        {
            HCACK=0;                                                        //Steven 20260415 : stub, acknowledge
        }
        else if(S.AnsiPos("LOT_PRE_END")==1)                                    //Command to discharge trays when max tray quantity is reached during Lot processing
        {
//        <L:2
//            <A:11 LOT_PRE_END>
//            <L:1
//                <L:2
//                    <A:14 OUTPUT_PORT_NO>
//                    <A:1 3>
//                >
//            >
//        >
            HCACK=0;                                                        //Steven 20260415 : stub, acknowledge
        }
        else if(S.AnsiPos("DISCHARGE_OUTPUT_PORT")==1)                          //DISCHARGE_OUTPUT_PORT
        {
            HCACK=0;                                                        //Steven 20260415 : stub, acknowledge
        }
        else if(S.AnsiPos("DISCHARGE_OUTPUT_ALL_PORT")==1)                      //All discharge commands for the Output Port that must be discharged after Lot End (CEID 5000 occurs for each port)
        {
//      <L:2
//          <A:25 DISCHARGE_OUTPUT_ALL_PORT>
//          <L:0>
//      >
            HCACK=0;                                                        //Steven 20260415 : stub, acknowledge
        }
        else if(S.AnsiPos("ALARM_NOTIFY")==1)                                   //Command to display alarm from host on equipment
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
            {
                for(i=0; i<SVlen; i++)
                {
                    if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                    {
                        HGem->GetDataItemLenAndType(len, Type);
                        CommandStr=new char [len+1];
                        memset(CommandStr, 0, len+1);
                        ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                        if(ret==-1)
                            S2="";
                        S2=CommandStr;
                        delete[] CommandStr;
                        CommandStr=NULL;

                        HGem->GetDataItemLenAndType(len, Type);
                        CommandStr=new char [len+1];
                        memset(CommandStr, 0, len+1);
                        ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                        if(ret==-1)
                            S3="";
                        S3=CommandStr;
                        delete[] CommandStr;
                        CommandStr=NULL;

                        if(S2=="HOST_ALARM_DESCRIPTION")
                        {
                            SecsAlarmMessage->Add(S3);
                            HCACK=0;
                        }
                    }
                }
            }
        }
        else if(S.AnsiPos("STOP_LOAD_PORT")==1)                                 //JerryYang 20250505 : AGV
        {
            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
            {
                HGem->GetDataItemLenAndType(len, Type);
                CommandStr=new char [len+1];                                    //JerryYang 20241205 : 字串陣列大小改成動態
//                ZeroMemory(CommandStr, sizeof(CommandStr));
                memset(CommandStr, 0, len+1);                                   //JerryYang 20250325 : fix
                ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                if(ret==-1)
                    S2="";
                S2=CommandStr;
                delete[] CommandStr;                                            //Steven 20160912 : Add delete for save memory
                CommandStr=NULL;

//                int iPort=0;

                if(S2=="PortID")
                {
                    HGem->GetDataItemLenAndType(len, Type);
                    CommandStr=new char [len+1];                                //JerryYang 20241205 : 字串陣列大小改成動態
//                    ZeroMemory(CommandStr, sizeof(CommandStr));
                    memset(CommandStr, 0, len+1);                               //JerryYang 20250325 : fix
                    HGem->GetDataItemLenAndType(len,Type);                      //取得資料長度與格式
                    if(Type==HType.INT_4_TYPE)
                    {
                        ret=HGem->DataItemIn(len, HType.INT_4_TYPE, &iLoadPort);
                        if(ret==-1)
                            iLoadPort=0;
                        S3=CommandStr;

//                        iLoadPort=atoi(S3.c_str());

                        delete[] CommandStr;                                    //Steven 20160912 : Add delete for save memory
                        CommandStr=NULL;
                        if(iLoadPort>=1 || iLoadPort<=15)
                        {
                            if(bPortIsBusy[iLoadPort]==false && fNote->fShow==false)                                    //RogerYang 20250617 add do not ready if Alarm Shown
                            {
                                bAskStopPort[iLoadPort]=true;
                                if(iLoadPort==ePortLoader)
                                {
                                    iPortStatus[iLoadPort]=eLoadReadyToLoad;
                                }
                                else if(iLoadPort==ePortAuto1 || iLoadPort==ePortAuto2 ||iLoadPort==ePortAuto3)
                                {
                                    iPortStatus[iLoadPort]=eReadyToUnload;
                                }
                                HCACK=0;
                            }
                            else
                            {
                                HCACK=1;
                            }
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("RESTART_LOAD_PORT")==1)                              //JerryYang 20250505 : AGV
        {
            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
            {
                HGem->GetDataItemLenAndType(len, Type);
                CommandStr=new char [len+1];                                    //JerryYang 20241205 : 字串陣列大小改成動態
//                ZeroMemory(CommandStr, sizeof(CommandStr));
                memset(CommandStr, 0, len+1);                                   //JerryYang 20250325 : fix
                ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                if(ret==-1)
                    S2="";
                S2=CommandStr;
                delete[] CommandStr;                                            //Steven 20160912 : Add delete for save memory
                CommandStr=NULL;

                if(S2=="PortID")
                {
                    HGem->GetDataItemLenAndType(len, Type);
                    CommandStr=new char [len+1];                                //JerryYang 20241205 : 字串陣列大小改成動態

                    memset(CommandStr, 0, len+1);                               //JerryYang 20250325 : fix

                    HGem->GetDataItemLenAndType(len,Type);                      //取得資料長度與格式
                    if(Type==HType.INT_4_TYPE)
                    {
                        ret=HGem->DataItemIn(len, HType.INT_4_TYPE, &iLoadPort);
                        if(ret==-1)
                            iLoadPort=0;
                        S3=CommandStr;

                        delete[] CommandStr;                                    //Steven 20160912 : Add delete for save memory
                        CommandStr=NULL;
                        if(iLoadPort>=1 || iLoadPort<=15)
                        {
                            bAskStopPort[iLoadPort]=false;
                            HCACK=0;
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=1;
            }
        }
//        else if(S.AnsiPos("PAUSE")==1 || S.AnsiPos("STOP")==1)                  //Steven 20221218 : S2F41字串判斷從原本的S=="" 改成 S.AnsiPos()==1
//        {
//            if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND)
//            {
//                if(fNote->fShow)
//                {
//                    fNote->BtnPauseClick(fNote);
//                    fNote->Close();
//                }
//
//                fMain->BtnPauseClick(fMain);
//                HCACK=0;
//            }
//            else
//            {
//                fMain->BtnPauseClick(fMain);
//                if(IniConfig.bEnable_SECS_GEM==true &&
//                   IniConfig.bRCMDStart==true &&
//                   bPhysicalStart==true)                                        //Wei 20150815 : Fixed for  RCMD Start
//                {
//                    bPhysicalStart=false;
//                }
//
//                if(CUSTOMER_CODE==CC_ASE_CL)                                    //RogerYang 20260413 Add
//                {
//                    bSECSPause=true;                                            //JerryYang 20250120 : modify
//                }
//
//                if(CosFunction.RunCheckWhenRecPause )                           //JerryYang 20250120 : modify
//                {
//                    bNeedDoRunCheck=true;
//                }
//
//                if(SystemStart==false)
//                {
//                    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                      //KenHsieh 20220913 : ASEKH K1要求回傳2
//                        HCACK=2;
//                    else if(IniConfig.bVTESTFunction)                           //RogerYang 20260608 : Add for VTEST SECS
//                        HCACK=5;                                                //5 = Rejected, Already inDesired Condition
//                    else
//                        HCACK=1;
//                }
//                else
//                {
//                    HCACK=0;
//                }
//            }
//        }
        else if(S.AnsiPos("ONE_CYCLE")==1 || S.AnsiPos("ONE CYCLE")==1)         //20140124 wei
        {
            bSECSOneCycleComm=true;                                             //RogerYang 20170705 (wei) add SECS/GEN OneCycle命令旗標
            fMain->BtnOneCycleClick(fMain);
            RecordProcess("SECS/GEM One Cycle!");
            HCACK=0;
        }
        else if(S.AnsiPos("RESET")==1)                                          //Steven 20220714 : Add RCMD RESET
        {
            fMain->Reset("RESET by SECS GEM S2F42");
            HCACK=0;
        }
        else if(S.AnsiPos("TRAYCHKOK")==1)                                      //JerryYang 20250207 : add
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                HCACK=1;
            }
            else
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
                {
                    if(SVlen==4)
                    {
                        fSCKART->ClearLotInfo();                                //JerryYang 20250731 : add
                        for(i=0; i<SVlen; i++)
                        {
                            if(HGem->DataItemIn(2,HType.LIST_TYPE,NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len,Type);          //取得資料長度與格式
                                if(Type==HType.ASCII_TYPE)
                                {
                                    HGem->DataItemIn(len,Type,str);             //取值
                                    S1=str;
                                    if(i==0)
                                    {
                                        if(S1=="COVERTRAYLIST")
                                        {
                                            HGem->GetDataItemLenAndType(len, Type);                                     //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值

                                                S2=StringReplace(S2, ";", ",", TReplaceFlags()<<rfReplaceAll);
                                                slBundlID->Clear();
                                                slBundlID->CommaText=S2;

                                                fSCKART->sBundleList=slBundlID->CommaText;
                                                fObserver->labBundleID->Caption=fSCKART->sBundleList;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==1)
                                    {
                                        if(S1=="LOTID")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3);                                         //取值

                                                fSCKART->sLotID.sprintf("%s", S3);
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==2)
                                    {
                                        if(S1=="STEP")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len, Type, S3);                                        //取值
                                                fSCKART->sInfo_Step=S3;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==3)
                                    {
                                        if(S1=="QTY")
                                        {
                                            HGem->GetDataItemLenAndType(len, Type);                                     //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3);                                         //取值
                                                fSCKART->sInfo_CurrQty=S3;

                                                fSCKART->iLotCount  =atoi(S3.c_str());
                                                fSCKART->iInputCount=atoi(S3.c_str());

                                                str1.sprintf("Lot start: %s, %d", fSCKART->sLotID, fSCKART->iLotCount);
                                                RecordProcess(str1);            //Steven 20190722 : add TSV log
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }

                                    fMain->Clarn_Data(8, "Clean by TRAY CHECK");                                        //JerryYang 20250307

                                    if(TrayForm.bSpecTrayCnt)
                                    {
                                        fSCKART->iBundleInCnt=TrayForm.iFullTrayCount;
                                        fSCKART->iBundleOutCnt=TrayForm.iInputTrayCount;
                                        iBundleIn=TrayForm.iFullTrayCount;
                                        iBundleOut=TrayForm.iInputTrayCount;
                                    }
                                    fSCKART->AccessFile(false, 1);
                                }
                            }
                            else
                            {
                                HCACK=1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        HCACK=2;
                    }
                }
            }
        }
        else if(S.AnsiPos("TRAYCHKNG")==1)                                      //JerryYang 20250207 : add
        {
            bTRAYCHKNG=true;
            HCACK=0;
        }
        else if(S.AnsiPos("BINDCHKNG")==1)                                      //JerryYang 20250207 : add
        {
            bBindChkNG=true;
            HCACK=0;
        }
        else if(S.AnsiPos("UNBINDCHKNG")==1)                                    //JerryYang 20250207 : add
        {
            bUnbindChkNG=true;
            HCACK=0;
        }
        else if(S.AnsiPos("CLEAR_SOCKET_COUNT")==1)                                   //JerryYang 20250825 : add
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<8; j++)
                {
                    LastSet.iSocketContactCount[i][j]=0;
                }
            }
            HCACK=0;
            bWaitSECS=true;                                                      //KevinC 20250919 : Wait SECS
        }
        else if(S.AnsiPos("CONTINUE_RETEST_ART")==1)                            //ChungHung 20150511 modify
        {
            if(MOT[MMPlate1].HasIC()       || MOT[MMPlate2].HasIC()         ||
                InArmSuck.HasIC()          || OutArmSuck.HasIC()            ||
                ShuttleHasIC()             || IndexHasIC()                  ||
                MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()   )    //2013-04-12    Dell
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmContinuRetest_ART);
                HCACK=0;
            }
        }
        else if(S.AnsiPos("CONTINUE_START_ART")==1)                             //ChungHung 20150511 modify
        {
            if(MOT[MMPlate1].HasIC()       || MOT[MMPlate2].HasIC()         ||
                InArmSuck.HasIC()          || OutArmSuck.HasIC()            ||
                ShuttleHasIC()             || IndexHasIC()                  ||
                MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()   )    //2013-04-12    Dell
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmContinuStart_ART);
                HCACK=0;
            }
        }
        else if(S.AnsiPos("CLEAN_AUTO_SORT_COUNT")==1 || S.AnsiPos("CLEAN_SORT_COUNT")==1)                              //Ifor 20171016 : add RETEST AUTO SORT COUNT
        {
            if(CUSTOMER_CODE==CC_ASE_CL)                                        //JerryYang 20250120 : modify
            {
                if(InArmSuck.HasIC()          || OutArmSuck.HasIC()         ||
                   ShuttleHasIC()             || IndexHasIC()               ||
                   MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC() )   //2013-04-12    Dell
                {
                    HCACK=2;
                }
//                else if(SystemStart==true)
//                {
//                    HCACK=4;
//                }
                else
                {
//                    bRefreshFunction=true;  //kevin 20181220 add
//                    fMain->Clarn_Data(8, "S2F42 Clear Count");

                    for(i=0; i<TEST_MAX_BIN; i++)
                    {
                        iSVByBinCount[i]=0;
                    }
                    iSV_ErrBinCnt=0;
                    fShowBinSelect->ShowCategoryBin();

                    HCACK=0;
                }
            }
            else
            {
                if(InArmSuck.HasIC()          || OutArmSuck.HasIC()         ||
                    ShuttleHasIC()             || IndexHasIC()               ||
                    MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC() )  //2013-04-12    Dell
                {
                    HCACK=2;
                }
                else
                {
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2018905 add
                    {
                        bRefreshFunction=true;                                  //kevin 20181220 add
                        fMain->Clarn_Data(8, "S2F42 Clear Count");
                    }
                    else
                    {
                        if(CUSTOMER_CODE==CC_JCET)                              //JerryYang 20210127 : 長電葉宏偉要求清除sort count跟yield計數
                        {
                            fMain->Clarn_Data(8, "btnClearCountClick");
                            fContactCT->sgYield->Refresh();

                            fProductionInfo->CalculateNowArmSiteBinQty(true);
                            for(int k=0; k<3; k++)
                            {
                                ArmData[k]->ClearALLCT();                       //2012-01-03    Dell fix 當按下Count Clear,在Tester Category的I/F Error數值錯誤
                            }

                            //Site Yield Alarm(%)
                            for(i=0; i<32; i++)
                                fYieldMonitoring->bShowSiteYield[i]=false;
                            fProductionInfo->UpdateControlBinCount(true);       //Sam 20200525 : Control Bin
                            fYieldMonitoring->iFailAlarmSiteMaxYieldIntervalCount=0;                                    //jou 2014-08-14 Site Compare Low Yield alarm
                            fYieldMonitoring->iFailAlarmSiteYieldIntervalCount=0;
                            fYieldMonitoring->iAutoClean_FailAlarmSiteYieldIntervalCount=0;
                            fYieldMonitoring->ClearYieldCount();                //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                            fYieldMonitoring->ClearAutoSiteOffStatus();         //Steven 20200409 : 修正清除count之後,不能開site的問題

                            LastSet.iIndexCount=0;                              //wei 20141201 Low Yield Auto Clean 重置
                            iLowYieldCloseCount=0;
                            bStandardYield=false;
                            for(i=0; i<4; i++)                                  //KEVIN 201050424 FIX
                            {
                                for(j=0; j<8; j++)
                                {
                                    iStandardYield[i][j]=0;
                                }
                            }
                            fContactCT->sgYield->Refresh();
                        }
                        for(int k=0; k<4; k++)
                        {
                            if(k==1)                                            //Steven 20190715 : 第1組為每天24hr清一次
                            {
                            }
                            else
                            {
                                LastSet.SendCT_ART[k]=0;
                                for(int i=0; i<eTrayCount; i++)
                                {
                                    LastSet.BinCT_ART[k][iTo3Unload[i]]=0;
                                }
                                for(int j=0; j<TEST_MAX_BIN; j++)
                                {
                                    LastSet.iBinData32_ART[k][j]=0;
                                }
                            }
                        }
                        for(int i=1; i<eTrayCount; i++)                         //Ifor 20240313 21 => eTrayCount
                        {
                            fShowBinSelect->StrARTSkipICCount->Cells[1][i]="";
                        }
                        fSortCT->ShowLoadingIC_ART();
                        fSortCT->ShowSortIC();
                        fShowBinSelect->ShowCategoryBin();
                    }
                HCACK=0;
                RecordProcess("SECS/GEM CLEAN AUTO SORT COUNT");
                }
            }
        }
        else if(S.AnsiPos("RETEST_MRT")==1)                                     //Ifor 20170425 add RETEST_MRT Remote Command
        {
            if(MOT[MMPlate1].HasIC()       || MOT[MMPlate2].HasIC()         ||
                InArmSuck.HasIC()          || OutArmSuck.HasIC()            ||
                ShuttleHasIC()             || IndexHasIC()                  ||
                MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()   )    //2013-04-12    Dell
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmRetest_MRT);
                HCACK=0;
            }
        }
        else if(S.AnsiPos("INITIAL_START_MRT")==1)                              //Ifor 20170425 add INITIAL_START_MRT Remote Command
        {
            if(MOT[MMPlate1].HasIC()       || MOT[MMPlate2].HasIC()         ||
                InArmSuck.HasIC()          || OutArmSuck.HasIC()            ||
                ShuttleHasIC()             || IndexHasIC()                  ||
                MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()   )    //2013-04-12    Dell
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmInitial_MRT);

                if(TrayForm.bEnableAMR) //Eastsun 20260515 F019 AMR
                    bSameSetupFileNoDownload=true;
                else
                    bSameSetupFileNoDownload=false;

                HCACK=0;
            }
        }
        else if(S.AnsiPos("CONTINUE_START_MRT")==1)                             //Ifor 20170425 add CONTINUE_START_MRT Remote Command
        {
            if(MOT[MMPlate1].HasIC()       || MOT[MMPlate2].HasIC()         ||
                InArmSuck.HasIC()          || OutArmSuck.HasIC()            ||
                ShuttleHasIC()             || IndexHasIC()                  ||
                MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()   )    //2013-04-12    Dell
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmContinuStart_MRT);
                HCACK=0;
            }
        }
        else if(S.AnsiPos("INITIAL_START")==1)                                  //kevin 20180806 (wei) add Initial startR 新增S2F41 回盤
        {
            if(MOT[MMPlate1].HasIC()      || MOT[MMPlate2].HasIC()          ||
               InArmSuck.HasIC()          || OutArmSuck.HasIC()             ||
               ShuttleHasIC()             || IndexHasIC()                   ||
               MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()    )
            {
                HCACK=2;
            }
            else
            {
                SetRunStartMode(rsmInitialStart);

                if(TrayForm.bEnableAMR) //Eastsun 20260515 F019 AMR
                    bSameSetupFileNoDownload=true;
                else
                    bSameSetupFileNoDownload=false;

                HCACK=0;
            }
        }
        else if(S.AnsiPos("REMOTE_SAVE")==1)                                    //kevin 20180914 (Steven) : add read ESD data
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                HCACK=4;
            }
            else
            {
                HCACK=0;
            }
        }
        else if(S.AnsiPos("AUTOSITEMAP")==1)                                    //kevin 20180806 (wei) add AutoSiteMap 新增S2F41 回盤
        {
            if(MOT[MMPlate1].HasIC()      || MOT[MMPlate2].HasIC()          ||
               InArmSuck.HasIC()          || OutArmSuck.HasIC()             ||
               ShuttleHasIC()             || IndexHasIC()                   ||
               MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()    )
            {
                HCACK=2;
            }
            else
            {
                RecordProcess("Trigger Auto Site Map by SECS GEM");
                SetRunStartMode(rsmAutoSiteMap);
                HCACK=0;
            }
        }
        else if(S.AnsiPos("AUTO_RETEST")==1)                                    //wei 20170217 (Steven) TSMC ATR 新增S2F41 回盤
        {
            if(MOT[MMPlate1].HasIC()      || MOT[MMPlate2].HasIC()          ||
               InArmSuck.HasIC()          || OutArmSuck.HasIC()             ||
               ShuttleHasIC()             || IndexHasIC()                   ||
               MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()    )
            {
                HCACK=2;
            }
            else
            {
                DoAutoRetest(true);
                SetRunStartMode(rsmAutoRetest);
                SoftStart=true;
                HCACK=0;
            }
        }
        else if(S.AnsiPos("TRAY_FEED")==1 || S.AnsiPos("TRAY FEED")==1)         //wei 20170217 (Steven) TSMC ATR 新增S2F41 Tray Feed
        {
            if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND)     //Ifor 20251018 add:Analog 泰國客戶要求Onecycle後需要可以Tray Feed
            {
                if(InArmSuck.HasIC()          || OutArmSuck.HasIC()             ||
                   ShuttleHasIC()             || IndexHasIC()                   ||
                   MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()    )
                {
                    HCACK=2;
                }
                else
                {
                    fMain->BtnTrayEndClick(fMain);
                    HCACK=0;
                }
            }
            else
            {
                if(MOT[MMPlate1].HasIC()      || MOT[MMPlate2].HasIC()          ||
                   InArmSuck.HasIC()          || OutArmSuck.HasIC()             ||
                   ShuttleHasIC()             || IndexHasIC()                   ||
                   MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC()    )
                {
                    HCACK=2;
                }
                else
                {
                    NewRecordProcess("MES2118", "TRAY FEED pressed", "S2F42 1");
                    fMain->BtnTrayEndClick(fMain);
                    if(!(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND))
                        SoftStart=true;
                    HCACK=0;
                }
            }
        }
        else if (S.AnsiPos("RETRY")==1)
        {
            if(fNote->fShow)
            {
                fNote->ReturnCode=K_RETRY;
                fNote->Close();
            }
            HCACK=0;
        }
        else if(S.AnsiPos("TRAY END")==1)
        {
            if(fNote->fShow)
            {
                fNote->ReturnCode=K_TRAY_END;
                fNote->Close();
            }
            HCACK=0;
        }
        else if(S.AnsiPos("INITIAL_START_ART")==1)                              //ChungHung 20150511 modify
        {
            if(HasICUnderMachine()==false)
            {
                SetRunStartMode(rsmInitial_ART);

                if(TrayForm.bEnableAMR) //Eastsun 20260515 F019 AMR
                    bSameSetupFileNoDownload=true;
                else
                    bSameSetupFileNoDownload=false;

                HCACK=0;
            }
            else
            {
                HCACK=2;
            }
        }
        else if(S.Pos("DOWNLOAD_RECIPE_BY_FTP")!=0)                             //ChungHung 20150511 modify
        {
            if(fFTPClient->bControlBySECSGEM==true)
            {
                HCACK=7;                                                        //Steven 20240923 : S2F42 HCACK 1 --> 7
            }
            else if(HasICUnderMachine()==true)
            {
                HCACK=2;
            }
            else
            {
                //Eastsun 20260710 Merge - comment out original Start
//                if(HGem->GetDataItemLenAndType(len, HType.LIST_TYPE))
//                {
//                    if(len==1)
//                        HGem->DataItemIn(len, HType.LIST_TYPE, NULL);           //Steven 20231226 : 舊的版本少一個L
//
//                    if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
//                    {
//                        HGem->GetDataItemLenAndType(len, Type);
//                        CommandStr=new char [len+1];                            //JerryYang 20241205 : 字串陣列大小改成動態
////                        ZeroMemory(CommandStr, sizeof(CommandStr));
//                        memset(CommandStr, 0, len+1);                           //JerryYang 20250325 : fix
//                        ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
//
//                        if(ret==-1)
//                            S2="";
//                        S2=CommandStr;
//                        delete[] CommandStr;                                    //Steven 20160912 : Add delete for save memory
//                        CommandStr=NULL;
//
//                        if(S2=="Setup_File")
//                        {
//                            HGem->GetDataItemLenAndType(len, Type);
//                            CommandStr=new char [len+1];                        //JerryYang 20241205 : 字串陣列大小改成動態
////                            ZeroMemory(CommandStr, sizeof(CommandStr));
//                            memset(CommandStr, 0, len+1);                       //JerryYang 20250325 : fix
//                            ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
//                            if(ret==-1)
//                                S3="";
//                            S3=CommandStr;
//                            delete[] CommandStr;                                //Steven 20160912 : Add delete for save memory
//                            CommandStr=NULL;
//                            fFTPClient->bControlBySECSGEM=true;
//                            fFTPClient->aSetUpNameBySECSGEM=S3;
//                            fFTPClient->ShowFTPModal(0);
//                            HCACK=0;
//                            bSecsGemDownloadFTP=true;                           //wei 20170119 (Steven) DownLoad 沒有馬上按掉會Time Out
//                        }
//                        else
//                        {
//                            HCACK=9;                                            //Steven 20240923 : S2F42 HCACK 1 --> 9
//                        }
//                    }
//                    else
//                    {
//                        HCACK=8;                                                //Steven 20240923 : S2F42 HCACK 1 --> 8
//                    }
//                }
//                else
//                {
//                    HCACK=10;                                                   //Steven 20240923 : S2F42 HCACK 1 --> 10
//                }
//            }
//
                //Eastsun 20260710 Merge - comment out original End
                //Eastsun 20260710 Merge - New code Start
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
                {
                    if(SVlen==0)
                    {
                        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                        HCACK=0;
                    }
                    else
                    {
                        for(i=0; i<SVlen; i++)
                        {
                            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len, Type);
                                CommandStr=new char [len+1];
                                memset(CommandStr, 0, len+1);
                                ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                if(ret==-1)
                                    S2="";
                                S2=CommandStr;
                                delete[] CommandStr;
                                CommandStr=NULL;

                                if(S2=="Setup_File")
                                {
                                    HGem->GetDataItemLenAndType(len, Type);
                                    CommandStr=new char [len+1];
                                    memset(CommandStr, 0, len+1);
                                    ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                    if(ret==-1)
                                        S3="";
                                    S3=CommandStr;
                                    delete[] CommandStr;
                                    CommandStr=NULL;
                                    fFTPClient->bControlBySECSGEM=true;
                                    fFTPClient->aSetUpNameBySECSGEM=S3;
                                    fFTPClient->ShowFTPModal(0);
                                    HCACK=0;
                                    bSecsGemDownloadFTP=true;
                                }
                                else
                                {
                                    HCACK=9;
                                }
                            }
                            else
                            {
                                HCACK=8;
                            }
                        }
                    }
                }
                else
                {
                    HCACK=10;
                }
                //Eastsun 20260710 Merge - New code End
            }        }
        else if(S.AnsiPos("CLEAN_OUT")==1 || S.AnsiPos("CLEAN OUT")==1)
        {
            if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND)
            {
                if(fNote->fShow)
                {
                    fNote->ReturnCode=K_CLEAN_OUT;
                    fNote -> Close();
                    HCACK=0;
                }
                else
                {
                    fMain->CleanOut("S2F42 1");
                    HCACK=0;
                }
            }
            else
            {
                fMain->CleanOut("S2F42 1");
                HCACK=0;
            }
        }
        else if(S.AnsiPos("SWITCH_TO_FT")==1)                                   //Steven 20141016 : Add RCMD SWITCH_TO_FT
        {
            iSecsGemSwitchFTRT=1;                                               //Steven 20210202 : 透過SECS/GEM切換動作狀態 0:無動作, 1:切換中, 2:切換成功
            HCACK=fMain->FTClick();                                             //Steven 20210423 :修改FT/RT Click回覆動作
            if(HCACK!=0)
            {
                if(HCACK==1)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (SystemStart)", HCACK);
                else if(HCACK==2)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (Picker has IC)", HCACK);
                else if(HCACK==3)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (RunStartMode is disable)", HCACK);
                else if(HCACK==4)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (Tray has IC)", HCACK);
                else
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d", HCACK);
                HGemPtr->StringOut(S2);
            }

            if(SystemStart || iSecsGemSwitchFTRT==1 || HCACK!=0)
                HCACK=2;
            else
                HCACK=0;
            iSecsGemSwitchFTRT=0;
        }
        else if(S.AnsiPos("SWITCH_TO_RT")==1)                                   //Steven 20141016 : Add RCMD SWITCH_TO_RT
        {
            iSecsGemSwitchFTRT=1;                                               //Steven 20210202 : 透過SECS/GEM切換動作狀態 0:無動作, 1:切換中, 2:切換成功
            HCACK=fMain->RTClick();                                             //Steven 20210423 :修改FT/RT Click回覆動作
            if(HCACK!=0)
            {
                if(HCACK==1)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_RT Fail! HCACK=%d (SystemStart)", HCACK);
                else if(HCACK==2)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_RT Fail! HCACK=%d (Tray has IC)", HCACK);
                else if(HCACK==3)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_RT Fail! HCACK=%d (Auto Site Map)", HCACK);
                else if(HCACK==4)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_RT Fail! HCACK=%d (Picker has IC)", HCACK);
                else if(HCACK==5)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (RunStartMode is disable)", HCACK);
                else if(HCACK==6)
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_FT Fail! HCACK=%d (Tray has IC)", HCACK);
                else
                    S2.sprintf("[S2F42]RCMD SWITCH_TO_RT Fail! HCACK=%d", HCACK);
                HGemPtr->StringOut(S2);
            }

            if(SystemStart || iSecsGemSwitchFTRT==1 || HCACK!=0)
                HCACK=2;
            else
                HCACK=0;
            iSecsGemSwitchFTRT=0;
        }
        else if(S.AnsiPos("ONLINE_LOCAL")==1)                                   //Steven 20150604 : Add ONLINE_LOCAL for SECS GEM
        {
            HGem->GemBtnOnlineLocal->Click();
            HCACK=0;
        }
        else if(S.AnsiPos("ONLINE_REMOTE")==1)                                  //Steven 20150604 : Add ONLINE_REMOTE for SECS GEM
        {
            HGem->GemBtnOnlineRemote->Click();
            HCACK=0;
        }
        else if(S=="RESUME")                                                    //JerryYang 20250120 : modify
        {
            if(IniConfig.bEnable_SECS_GEM==true)
            {
                bSECSGEMAlarm=false;
                bSECSPause=false;
            }
            HCACK=0;
        }
        else if(S.AnsiPos("REMOTE_START")==1)                                   //20180817 add
        {
            if(SystemStart==false && CosFunction.bCanRemoteStart)               //Steven 20250712 : 有通過風險告知可以遠端start的客戶
            {
                HCACK=0;
                if(TrayForm.bEnableAMR) //Eastsun 20260515 F019 AMR
                {
                    if(RunInfo.bLotStart==true)
                    {
                        bSameSetupFileNoDownload=true;
                        fMain->Start("SECS GEM RCMD : REMOTE_START");
                    }
                }
                else
                {
                    fMain->Start("SECS GEM RCMD : REMOTE_START");
                }
            }
            else
            {
                HCACK=1;
            }
        }
//        else if(S.AnsiPos("START")==1)                                          //Steven 20141016 : Add RCMD START
//        {
//            if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND)
//            {
//                if(SystemStart==false)
//                {
//                    HCACK=0;
//                    fMain->Start("SECS GEM RCMD : START");
//                }
//                else
//                {
//                    HCACK=1;
//                }
//            }
//            else if(IniConfig.bEnable_SECS_GEM==true &&
//                    CosFunction.bCanRemoteStart)                                //Steven 20250712 : 有通過風險告知可以遠端start的客戶
//            {
//                HCACK=0;
//                bSECSGEMAlarm=false;                                            //Ifor 20151208 :解除按鍵Lock
//                bHasSaveSet=false;                                              //Ifor 20151208 :清除設定檔變更旗標
//                fMain->Start("SECS GEM RCMD : START");
//            }
//            else if(IniConfig.bEnable_SECS_GEM==true &&                         //Steven 20141006 : SECS GEM使用Remote Start功能
//                    ((IniConfig.bRCMDStart==true && bPhysicalStart==true) ||
//                    bSecsGemCanStart))                                          //wei 20150630  SecsGem Can Start
//            {
//                SoftStart=true;
//                RecordProcess("SECS GEM RCMD : RUN CHECK START");
//                bSecsGemCanStart=false;
//                HCACK=0;
//                bSECSGEMAlarm=false;                                            //Ifor 20151208 :解除按鍵Lock
//                bHasSaveSet=false;                                              //Ifor 20151208 :清除設定檔變更旗標
//            }
//            else
//            {
//                HCACK=1;
//            }
//            bPhysicalStart=false;
//            bNeedDoRunCheck=false;                                              //JerryYang 20250120 : modify
//        }
        else if(S.AnsiPos("HOME")==1)                                           //Steven 20250712 : Add RCMD HOME
        {
            if(IniConfig.bEnable_SECS_GEM==true &&
               CosFunction.bCanRemoteStart)                                     //Steven 20250712 : 有通過風險告知可以遠端start的客戶
            {
                HCACK=0;
                bSECSGEMAlarm=false;                                            //Ifor 20151208 :解除按鍵Lock
                bHasSaveSet=false;                                              //Ifor 20151208 :清除設定檔變更旗標
                RecordProcess("SECS GEM RCMD : Home");
                fMain->Home("S2F42");
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("HALT")==1)                                           //Steven 20141016 : Add RCMD HALT
        {
            if(IniConfig.bEnable_SECS_GEM==true &&
               IniConfig.bRCMDStart==true &&
               bPhysicalStart==true)                                            //Steven 20141006 : SECS GEM使用Remote Start功能
            {
                SoftStart=false;
                RecordProcess("SECS GEM RCMD : Handler halt by Host");
//                HSys.MyGem->SecsAlarmMessage->Add("Handler halt by Host");    //Steven 20150519 : AdanYu說不要顯示
                HCACK=0;
            }
            else
            {
                HCACK=2;
            }
            bPhysicalStart=false;
        }
        else if(S.AnsiPos("PP_MUSIC")==1)                                       //Steven 20150604 : Add PP_MUSIC for SECS GEM
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)                                         //wei 20150630
            {
                if(SVlen==0)
                {
                    SECS_GEM_PPMUSIC_CONTROL_flag=false;
                    HCACK=0;
                }
                else
                {
                    if(HGem->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
                    {
                        HGem->GetDataItemLenAndType(len, Type);                 //取得資料長度與格式
                        HGem->DataItemIn(len, Type,str);                        //取值
                        HGem->GetDataItemLenAndType(len, Type);                 //取得資料長度與格式
                        HGem->DataItemIn(len, Type, &ret);                      //取值

                        HCACK=0;
                        iSECS_GEM_PPMUSIC_CONTROL_CLASS=ret;
                        SECS_GEM_PPMUSIC_CONTROL_flag=true;
                    }
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("PP_SIGNALTOWER")==1)                                 //Steven 20150604 : Add PP_SIGNALTOWER for SECS GEM
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)                                         //wei 20150630
            {
                if(SVlen==0)
                {
                    SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                    HCACK=0;
                }
                else
                {
                    for(i=0; i<SVlen; i++)
                    {
                        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len,Type);              //取得資料長度與格式
                            HGem->DataItemIn( len,Type,str);                    //取值

                            HGem->GetDataItemLenAndType(len,Type);              //取得資料長度與格式
                            HGem->DataItemIn( len,Type,&ret);                   //取值
                            S1=str;
                            HCACK=0;
                            if(S1=="RED")
                                iSECS_GEM_PPSIGNALTOWER_CONTROL_RED=ret;
                            else if(S1=="GREEN")
                                iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN=ret;
                            else if(S1=="YELLOW")
                                iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW=ret;
                            else
                                HCACK=1;
                            SECS_GEM_PPSIGNALTOWER_CONTROL_flag=true;
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("AUTO_CLEAN")==1)
        {
            if(IniConfig.bEnableAutoCleanFunction &&
               TestIF.iAutoClean_Function==true &&
               bRunAutoClean==false)                                            //Ifor 20220512 add:SECS GEM Auto Clean啟動中或無開啟回覆1
            {
                bSECSGEMAutoclean=true;                                         //Ifor 20220414 add SECS/GEM AUTO Clean
                RecordProcess("Start ONE CYCLE by SECS/GEM Auto Clean...");
                fShowBinSelect->btnAutoCleanClick(fShowBinSelect);
                HCACK=0;
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("PP_PASSWORD")==1)                                    //wei 20150803 SECSGEM更改PASSWORD
        {
            if(HGem->GetDataItemLenAndType(len, HType.LIST_TYPE))
            {
                if(len==1)
                    HGem->DataItemIn(len, HType.LIST_TYPE, NULL);               //Steven 20231226 : 舊的版本少一個L

                if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                {
                    HGem->GetDataItemLenAndType(len, Type);                     //取得資料長度與格式
                    HGem->DataItemIn(len, Type, str);                           //取值

                    HGem->GetDataItemLenAndType(len, Type);                     //取得資料長度與格式
                    HGem->DataItemIn(len, Type, &ret);                          //取值

                    asSECSGEMChangeName=str;
                    asSECSGEMChangePassword=ret;
                    fMain->ChangePassword();                                    //wei 20150803
                    HCACK=0;
                }
                else
                {
                    HCACK=1;
                }
            }
        }
         else if(S.AnsiPos("REMOTE_SAVE")==1)                                   //kevin 20180823 add
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                HCACK=4;
            }
            else
            {
                  HCACK=0;
            }
        }
        else if(S.AnsiPos("PP_SELECT")==1 || S.AnsiPos("PP-SELECT")==1)         //Steven 20140929 : 新增PP-Select的Remote Command
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20180821 add
            {
                HGemPtr->GetDataItemLenAndType(len, Type);
                if(Type==HType.ASCII_TYPE)
                {
                    ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
                    if(ret==-1)
                        PPID="";
                }
                else
                {
                    PPID="";
                }

                if(fMain->cbSetupFileName->Items->Text.AnsiPos(PPID)==0)
                {
                    HCACK=6;                                                    //Steven 20160425 : 改成HCACK=5-->6
                }
                else
                {
                    if(HasICUnderMachine()==true || SystemStart==true)
                    {
                        HCACK=4;
                    }
                    else
                    {
                        fMain->cbSetupFileName->Text=PPID;
                        fMain->cbSetupFileNameChange(fMain);
                        HCACK=0;
                        if(CosFunction.bKeepOnly1SetupFile)
                            fLotInfo->ClearAllSetupFile(PPID);                  //Steven 20231209 : Add for 下載完工作檔後, 只留一個就好
                    }
                }
            }
            else if(CUSTOMER_CODE==CC_ONSEMI_M ||
                    CUSTOMER_CODE==CC_IFXTH_Thai ||
                    CUSTOMER_CODE==CC_ASE_CL ||                                 //JerryYang 20250120 : modify
                    IniConfig.bVTESTFunction==true)                             //RogerYang 20260608 : Add for VTEST SECS
            {
                if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
                {
                    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                    {
                        HGem->GetDataItemLenAndType(len, Type);                 //取得資料長度與格式
                        HGem->DataItemIn(len, Type, str);                       //取值
                        S1=str;
                        if(S1=="PPID" || CUSTOMER_CODE==CC_ASE_CL)              //JerryYang 20250120 : modify
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
                                if(ret==-1)
                                    PPID="";
                            }
                            else
                            {
                                PPID="";
                            }
                        }
                        else
                        {
                            PPID="";
                        }

                        bHasFile=false;
                        for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
                        {
                            if(PPID!="" && PPID==fMain->cbSetupFileName->Items->Strings[i])
                                bHasFile=true;
                        }

                        if(bHasFile==false)
                        {
                            HCACK=6;                                            //Steven 20160425 : 改成HCACK=5-->6
                        }
                        else
                        {
                            if(HasICUnderMachine()==true || SystemStart==true)
                            {
                                HCACK=4;
                            }
                            else
                            {
                                fMain->cbSetupFileName->Text=PPID;
                                fMain->cbSetupFileNameChange(fMain);
                                HCACK=0;
                            }
                        }
                    }
                    else
                    {
                        HCACK=3;
                    }
                }
                else
                {
                    HCACK=2;
                }
            }
            // ==================================================================
            // CC_IFXTH_Thai - PP_SELECT (SEMI E5 compliant)        Ifor 20260618
            //   S2F41 request format:
            //     <L,2 <A "PP-SELECT">
            //          <L,n                            ; CPLIST, n>=1
            //              <L,2 <A "PPID"> <A value>>  ; CPNAME / CPVAL pair
            //              ...                         ; additional CPs ignored
            //          >
            //     >
            //   S2F42 HCACK (per SEMI E5 10.4):
            //     0 = success, switched synchronously
            //     2 = CPLIST missing or empty
            //     3 = CPNAME/CPVAL invalid (wrong type, length, or PPID missing)
            //     4 = accepted, will switch when machine becomes idle
            //         (CheckAndExecuteAsyncRCMD will fire S6F11 SwitchSetupFile)
            //     5 = already in desired condition (current PPID matches request)
            //     6 = no such object (PPID not in setup file list)
            // ==================================================================
            else if(CUSTOMER_CODE==CC_IFXTH_Thai)                               //Ifor 20260618 : SEMI E5 PP-SELECT compliant
            {
                AnsiString sNewPPID = "";
                bool       bParamOK = false;
                bool       bFound   = false;
                int        iCPCount = 0;

                if(HGemPtr->GetDataItemLenAndTypeAndDelete(iCPCount, HType.LIST_TYPE) == 1)
                {
                    if(iCPCount <= 0)
                    {
                        HCACK = 2;                                              // empty CPLIST
                    }
                    else
                    {
                        bParamOK = true;                                        // optimistic; downgrade on any error
                        for(int k = 0; k < iCPCount; k++)
                        {
                            if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL) != 1)
                            {
                                bParamOK = false;
                                break;
                            }

                            // ---- CPNAME ----
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type != HType.ASCII_TYPE || len <= 0 || len > 256)
                            {
                                bParamOK = false;
                                break;
                            }
                            CommandStr = new char[len + 1];
                            memset(CommandStr, 0, len + 1);
                            HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                            S1 = CommandStr;
                            delete[] CommandStr;
                            CommandStr = NULL;

                            // ---- CPVAL ----
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type != HType.ASCII_TYPE || len <= 0 || len > 256)
                            {
                                bParamOK = false;
                                break;
                            }
                            CommandStr = new char[len + 1];
                            memset(CommandStr, 0, len + 1);
                            HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);

                            if(S1 == "PPID")
                            {
                                sNewPPID = CommandStr;
                            }
                            // other CPNAMEs ignored: SEMI E5 PP-SELECT only mandates PPID

                            delete[] CommandStr;
                            CommandStr = NULL;
                        }

                        if(bParamOK == false || sNewPPID.Length() == 0)
                        {
                            HCACK = 3;                                          // invalid CP or PPID missing
                            RecordProcess("[S2F42] PP_SELECT(IFXTH) HCACK=3 invalid CP");
                        }
                        else
                        {
                            for(int k = 0; k < fMain->cbSetupFileName->Items->Count; k++)
                            {
                                if(sNewPPID == fMain->cbSetupFileName->Items->Strings[k])
                                {
                                    bFound = true;
                                    break;
                                }
                            }

                            if(bFound == false)
                            {
                                HCACK = 6;                                      // no such object
                                RecordProcess("[S2F42] PP_SELECT(IFXTH) HCACK=6 PPID not found: " + sNewPPID);
                            }
                            else if(sNewPPID == fMain->cbSetupFileName->Text)
                            {
                                HCACK = 5;                                      // already in desired condition
                                RecordProcess("[S2F42] PP_SELECT(IFXTH) HCACK=5 already selected: " + sNewPPID);
                            }
                            else if(HasICUnderMachine() == true || SystemStart == true)
                            {
                                m_AsyncRCMD.bPending = true;
                                m_AsyncRCMD.sCommand = "PP_SELECT";             // matched by CheckAndExecuteAsyncRCMD
                                m_AsyncRCMD.sParam   = sNewPPID;
                                HCACK = 4;                                      // accepted, will switch when idle
                                RecordProcess("[S2F42] PP_SELECT(IFXTH) HCACK=4 async pending: " + sNewPPID);
                            }
                            else
                            {
                                fMain->cbSetupFileName->Text = sNewPPID;
                                fMain->cbSetupFileNameChange(fMain);
                                HGemPtr->EventReport(1, SECS_EVENT.SwitchSetupFile);  // CEID=15 notify host
                                HCACK = 0;
                                RecordProcess("[S2F42] PP_SELECT(IFXTH) HCACK=0 switched: " + sNewPPID);
                            }
                        }
                    }
                }
                else
                {
                    HCACK = 2;                                                  // CPLIST not present
                }
            }
            else if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                         //Sam 20250325 : 矽格中興 PPID
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
                {
                    if(SVlen==0)
                    {
                        HCACK=2;
                    }
                    else
                    {
                        if(SVlen==2)
                        {
                            for(i=0; i<SVlen; i++)
                            {
                                if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                                {
                                    HGem->GetDataItemLenAndType(len, Type);     //取得資料長度與格式
                                    HGem->DataItemIn(len, Type, str);           //取值
                                    S1=str;
                                    if(i==0)
                                    {
                                        if(S1=="PPID")
                                        {
                                            HGem->GetDataItemLenAndType(len, Type);
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                ret=HGem->DataItemIn(len, HType.ASCII_TYPE, PPID);
                                                if(ret==-1)
                                                    PPID="";
                                            }
                                            else
                                            {
                                                PPID="";
                                            }
                                        }
                                        else
                                        {
                                            PPID="";
                                        }

                                        bHasFile=false;
                                        for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
                                        {
                                            if(PPID!="" && PPID==fMain->cbSetupFileName->Items->Strings[i])
                                                bHasFile=true;
                                        }

                                        if(bHasFile==false)
                                        {
                                            HCACK=6;
                                        }
                                        else
                                        {
                                            if(HasICUnderMachine()==true || SystemStart==true)
                                            {
                                                HCACK=4;
                                            }
                                            else
                                            {
                                                fMain->cbSetupFileName->Text=PPID;
                                                fMain->cbSetupFileNameChange(fMain);
                                                HCACK=0;
                                            }
                                        }
                                    }
                                    else if(i==1)
                                    {
                                        if(S1=="ICQUANTITY")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3);                                         //取值
                                                iHead=atoi(S3.c_str());
                                                if(iHead>0)
                                                {
                                                    LastSet.iP57_InputCT=iHead;                                         //Sam 20250605 : Loader Count AutoCleanOut
                                                    HCACK=0;
                                                }
                                                else
                                                {
                                                    HCACK=2;
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    HCACK=3;
                                }
                            }
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)           // 需要補充多重 Command
            {
                if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                {
                    HGemPtr->GetDataItemLenAndType(len, Type);

                    if(Type==HType.ASCII_TYPE)                                  //Steven 20250725
                    {
                        ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                        S1=str;
                        if(S1=="PPID")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
                                if(ret==-1)
                                    PPID="";
                            }
                            else
                            {
                                PPID="";
                            }
                        }
                        else
                        {
                            PPID=str;
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                            }
                        }
                    }
                    else
                    {
                        PPID="";
                        HGemPtr->GetDataItemLenAndType(len, Type);
                        if(Type==HType.ASCII_TYPE)
                        {
                            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                        }
                    }

                    bHasFile=false;
                    for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
                    {
                        if(PPID!="" && PPID==fMain->cbSetupFileName->Items->Strings[i])
                            bHasFile=true;
                    }

                    if(bHasFile==false)
                    {
                        HCACK=6;                                                //Steven 20160425 : 改成HCACK=5-->6
                    }
                    else
                    {
                        if(HasICUnderMachine()==true || SystemStart==true)
                        {
                            HCACK=4;
                        }
                        else
                        {
                            fMain->cbSetupFileName->Text=PPID;
                            fMain->cbSetupFileNameChange(fMain);
                            HCACK=0;
                        }
                    }

//                    HGemPtr->GetDataItemLenAndType(len, Type);        //JerryYang 20250725 : 多做的, MARK掉
//                    if(Type==HType.ASCII_TYPE)
//                    {
//                        ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
//                    }

                    #ifndef FOR_NVIDIA_2D_SORT                                  //JerryYang 20230222 : 中科佳毅要求切換工作檔時開啟run check
                    if(IniConfig.bSPILFunction && IniConfig.bRCMDStart==false)
                    {
                        AnsiString sPath=AuthPath+"config.ini";
                        IniConfig.bRCMDStart=true;
                        IniConfig.bRCMDStart=ReadWriteIni(sPath, "SECS GEM", "Enable RCMD START", IniConfig.bRCMDStart, false, false);
                        fConfiguration->cbN07_EnableHostStart->Checked=true;    //JerryYang 20250826 : 元件也要強制打勾避免進溫度頁面按存檔又被關閉
                    }
                    #endif
                }
                else
                {
                    HCACK=3;
                }
            }
            else
            {
                HCACK=2;
            }
        }
        else if(S.AnsiPos("LOTSTART")==1)                                       //wei 20160517 TSMC lot卡關
        {
            if(CUSTOMER_CODE==CC_XINYUN)                                        //AI(ht9045-secs-sem) 20260615 (RogerYang) : LOTSTART 改非同步,避免 sbSECSLotStartClick
            {                                                                   //(含 DoPassword 模態框/檔案下載) 在 SECS thread 同步 block 造成 S2F42 逾時斷線
                AnsiString sName, sVal, sLotID="";                              //LOTSTART 帶 LOTID;批號同步設,開批改非同步避免 block SECS thread
                int  iParamCnt=0;
                unsigned char tType;

                if(HGem->GetDataItemLenAndTypeAndDelete(iParamCnt, tType)==1 && tType==HType.LIST_TYPE)
                {
                    for(i=0; i<iParamCnt; i++)
                    {
                        if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len, Type);             // 取 name 實際長度/型別
                            HGem->DataItemIn(len, Type, sName);
                            HGem->GetDataItemLenAndType(len, Type);             // 取 value 實際長度/型別
                            HGem->DataItemIn(len, Type, sVal);
                            if(sName.UpperCase()=="LOTID")
                                sLotID = sVal;

                        }
                    }
                }
                if(sLotID!="")
                    fLotInfo->edtSysLotID->Text = sLotID.Trim();                // 批號「現在」就設好
                fLotInfo->sbSECSLotStartClick(fLotInfo);
                RecordProcess("[LOTSTART] executed, LotID="+fLotInfo->edtSysLotID->Text);
                HCACK=0;
            }
            else
            {
                fLotInfo->sbSECSLotStartClick(fLotInfo);
                RecordProcess("SECS/GEM LOTSTART!");                                //Ifor 20200908 add:記錄SECS/GEM LOTSTART
                HCACK=0;
            }
        }
        else if(S.AnsiPos("AUTHORITY_CHECK")==1)
        {
            unsigned char  uint1EC;
            if(IniConfig.bN07_EnableEmployeeIdCheak==true)
            {
                if(FormBarcodeReader->bShow)
                {
                    FormBarcodeReader->Close();
                }

                if(fPassword->Visible==true)
                {
                    fPassword->Close();
                }

                if(MyMessageBox->fShow && bSECSGEMAlarm==false)                 //Ifor 20180911 :Add 啟動 Employee ID Check
                {
                    MyMessageBox->Close();
                }

                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)                                     //wei 20150630
                {
                    if(SVlen==0)
                    {
                        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                        HCACK=0;
                    }
                    else
                    {
                        for(i=0; i<SVlen; i++)
                        {
                            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len, Type);         //取得資料長度與格式
                                HGem->DataItemIn(len, Type, str);               //取值
                                S1=str;
                                HCACK=0;
                                if(S1=="Action")
                                {
                                    if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, &uint1EC)==1)
                                    {
                                        ret=uint1EC;
                                    }
                                }
                                else if(S1=="Message")
                                {
                                    HGemPtr->GetDataItemLenAndType(len, Type);
                                    if(Type==HType.ASCII_TYPE)
                                    {
                                        CommandStr=new char [len+1];            //JerryYang 20241205 : 字串陣列大小改成動態
//                                        ZeroMemory(CommandStr, sizeof(CommandStr));
                                        memset(CommandStr, 0, len+1);           //JerryYang 20250325 : fix
                                        ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                        str2=CommandStr;
                                        delete[] CommandStr;                    //Steven 20160912 : Add delete for save memory
                                        CommandStr=NULL;
                                    }
                                }
                                else
                                {
                                    HCACK=1;
                                }
                            }
                            else
                            {
                                HCACK=1;
                            }
                        }
                    }
                    bWaitSecsGemReply=false;                                    //Ifor 20180302 Time Out 判斷旗標
                    if(MyMessageBox->fShow || fNote->fShow)
                    {
                        if(uint1EC==0)
                        {
                            HCACK=0;
                            iShowAUTHORITY=2;                                   //Ifor 20180302 顯示判斷
                            bWaitSecsGemReply=true;
                        }
                        else if(uint1EC==1 || uint1EC==2)
                        {
                            iShowAUTHORITY=1;
                            bWaitSecsGemReply=true;
                            strShowAUTHORITY=str2;                              //JerryYang 20241205 : 字串陣列大小改成動態
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        iShowAUTHORITY=0;
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=0;
            }
        }
        else if(S=="SET_TEST_FLOW")                                             //JerryYang 20250120 : modify
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                HCACK=3;
            }
            else
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE)==1)
                {
                    if(SVlen==0)
                    {
                        HCACK=2;
                    }
                    else
                    {
                        if(SVlen>=4)
                        {
                            for(i=0; i<SVlen; i++)
                            {
                                if( HGem->DataItemIn(2,HType.LIST_TYPE,NULL)==1)
                                {
                                    HGem->GetDataItemLenAndType(len,Type);      //取得資料長度與格式
                                    if(Type==HType.ASCII_TYPE)
                                    {
                                        HGem->DataItemIn(len,Type,str);         //取值
                                        S1=str;

                                        if(S1=="AO")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值
                                                fLotInfo->edtASECL_LotID->Text=S2;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                            }
                                        }
                                        else if(S1=="FLOW_ID")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值
                                                fLotInfo->edFlowID->Text=S2;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                            }
                                        }
                                        else if(S1=="INSERTION")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值
                                                fLotInfo->edInsertion->Text=S2;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                            }
                                        }
                                        else if(S1=="CUSTOMER_DEVICE")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值
                                                fLotInfo->edCustomerDevice->Text=S2;
                                                HCACK=0;
                                                //JerryYang 20250411 : 這裡要按Lot start
                                                fLotInfo->btnASECL_LotStartClick(fLotInfo->btnASECL_LotStart);
                                            }
                                            else
                                            {
                                                HCACK=2;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                        }
                                    }
                                }
                                else
                                {
                                    HCACK=1;
                                }
                            }
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
        }
        else if(S.AnsiPos("SET_LOT_INFO")==1)                                   //JerryYang 20190409
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                if(IniConfig.bSPILFunction==true)
                {
                    if(IniConfig.bA37LotStartLotEnd==false &&
                       fSCKART->iCurrent93KARTStep!=8)                          //JerryYang 20220923 : 修改SPIL LOT INFO
                    {
                        HCACK=4;
                    }
                }
                else
                {
                    HCACK=4;
                }
            }

            if(HCACK!=4)
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
                {
                    if(SVlen==0)
                    {
                        HCACK=2;
                    }
                    else
                    {
                        if(SVlen==2)                                            //JerryYang 20220923 : 修改SPIL LOT INFO
                        {
                            for(i=0; i<SVlen; i++)
                            {
                                if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                                {
                                    HGem->GetDataItemLenAndType(len, Type);     //取得資料長度與格式
                                    if(Type==HType.ASCII_TYPE)
                                    {
                                        HGem->DataItemIn(len, Type, str);       //取值
                                        S1=str;
                                        if(i==0)
                                        {
                                            if(S1=="LOT_INFO")                  //JerryYang 20220923 : 修改SPIL LOT INFO
                                            {
                                                HGem->GetDataItemLenAndType(len, Type);                                 //取得資料長度與格式
                                                if(Type==HType.ASCII_TYPE)
                                                {
                                                    HGem->DataItemIn(len, Type, S2);                                    //取值
                                                    bHasFile=ProcessLotInfo(S2);
                                                    if(bHasFile==false)
                                                    {
                                                        HCACK=3;
                                                    }
                                                    else
                                                    {
                                                        bReadLotInfoFromART=true;
                                                    }
                                                }
                                                else
                                                {
                                                    HCACK=7;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=5;
                                            }
                                        }
                                        else if(i==1)
                                        {
                                            if(S1=="DISPLAY")
                                            {
                                                HGem->GetDataItemLenAndType(len, Type);                                 //取得資料長度與格式
                                                if(Type==HType.ASCII_TYPE)
                                                {
                                                    HGem->DataItemIn(len, Type, S3);                                    //取值
                                                    if(HCACK==1)
                                                        HCACK=0;
                                                }
                                                else
                                                {
                                                    HCACK=6;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=2;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    HCACK=8;
                                }
                            }
                        }
                        else
                        {
                            HCACK=9;
                        }
                    }
                }
                else
                {
                    HCACK=10;
                }
            }
        }
        else if(S.AnsiPos("SET_BUNDLE_INFO")==1)                                //JerryYang 20240321 : add
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
            {
                if(SVlen==3 || SVlen==2)
                {
                    for(i=0; i<SVlen; i++)
                    {
                        if(HGem->DataItemIn(2,HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len,Type);              //取得資料長度與格式
                            if(Type==HType.ASCII_TYPE)
                            {
                                HGem->DataItemIn(len,Type,str);                 //取值
                                S1=str;
                                if(i==0)
                                {
                                    if(S1=="BUNDLE_LIST")
                                    {
                                        HGem->GetDataItemLenAndType(len, Type);                                         //取得資料長度與格式
                                        if(Type==HType.ASCII_TYPE)
                                        {
                                            HGem->DataItemIn(len,Type, S2);     //取值
                                            slBundlID->Clear();
                                            slBundlID->CommaText=S2;

                                            fSCKART->sBundleList=slBundlID->CommaText;
                                            fObserver->labBundleID->Caption=fSCKART->sBundleList;
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        HCACK=2;
                                        break;
                                    }
                                }
                                else if(i==1)
                                {
                                    if(S1=="BUNDLE_IN")
                                    {
                                        HGem->GetDataItemLenAndType(len,Type);  //取得資料長度與格式
                                        if(Type==HType.ASCII_TYPE)
                                        {
                                            HGem->DataItemIn(len,Type, S3);     //取值
                                            iBundleIn=StrToIntDef(S3, 0);

                                            fSCKART->iBundleInCnt=iBundleIn;
                                            fObserver->labBundlIn->Caption=fSCKART->iBundleInCnt;

                                            if(iBundleIn>2)
                                            {
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                iBundleIn=12;
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(S1=="BUNDLE_OUT")
                                    {
                                        HGem->GetDataItemLenAndType(len,Type);  //取得資料長度與格式
                                        if(Type==HType.ASCII_TYPE)
                                        {
                                            HGem->DataItemIn(len,Type, S3);     //取值
                                            iBundleOut=StrToIntDef(S3, 0);
                                            fSCKART->iBundleOutCnt=iBundleOut;
                                            if(iBundleOut>2)
                                            {
                                                fObserver->labBundOut->Caption=fSCKART->iBundleOutCnt;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                HCACK=1;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        HCACK=2;
                                        break;
                                    }
                                }
                                else if(i==2)
                                {
                                    if(S1=="BUNDLE_OUT")
                                    {
                                        HGem->GetDataItemLenAndType(len,Type);  //取得資料長度與格式
                                        if(Type==HType.ASCII_TYPE)
                                        {
                                            HGem->DataItemIn(len,Type, S3);     //取值
                                            iBundleOut=StrToIntDef(S3, 0);
                                            fSCKART->iBundleOutCnt=iBundleOut;
                                            if(iBundleOut>2)
                                            {
                                                fObserver->labBundOut->Caption=iBundleOut;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                iBundleOut=12;
                                                HCACK=1;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        HCACK=2;
                                        break;
                                    }
                                }
                                fSCKART->AccessFile(false, 1);
                            }
                        }
                        else
                        {
                            HCACK=1;
                            break;
                        }
                    }
                }
                else
                {
                    HCACK=2;
                }
            }
        }
        else if(S.AnsiPos("DEVTEMPOFFSETADJUST")==1)                            //JerryYang 20190812 For Qualcomm 溫度offset funciton
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
            {
                if(SVlen==0)
                {
                    HCACK=2;
                }
                else
                {
                    if(SVlen==3)
                    {
                        for(i=0; i<SVlen; i++)
                        {
                            if(HGem->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len, Type);         //取得資料長度與格式
                                if(Type==HType.ASCII_TYPE)
                                {
                                    HGem->DataItemIn(len,Type,str);             //取值
                                    S1=str;
                                    if(i==0)
                                    {
                                        if(S1=="INDEX_ARM")
                                        {
                                            HGem->GetDataItemLenAndType(len, Type);                                     //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2);                                         //取值
                                                if(S2!="F" && S2!="R")
                                                {
                                                    HCACK=2;
                                                    break;
                                                }
                                                else
                                                {
                                                    if(S2=="F")
                                                    {
                                                        iArm=1;
                                                    }
                                                    else
                                                    {
                                                        iArm=2;
                                                    }
                                                    HCACK=0;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==1)
                                    {
                                        if(S1=="THERMAL_HEAD")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);                                      //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3);                                         //取值
                                                iHead=StrToIntDef(S3, 0);
                                                if(iHead>0 && iHead<=16)
                                                {
                                                    HCACK=0;
                                                }
                                                else
                                                {
                                                    HCACK=2;
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==2)
                                    {
                                        if(S1=="TEMP_OFFSET")
                                        {
                                            HGem->GetDataItemLenAndType(len, Type);                                     //取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3);                                         //取值
                                                dOffset=StrToFloatDef(S3, 0.0);
                                                if(fTemp_Set->SaveRemoteTempOffset(iArm, iHead, dOffset)==0)
                                                {
                                                    fTemp_Set->ReadRemoteTempOffset();
                                                    HCACK=0;
                                                }
                                                else
                                                {
                                                    HCACK=1;
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                HCACK=1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        HCACK=1;
                    }
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("CLEAR_LOT_INFO")==1)                                 //JerryYang 20200330 : 修改SPIL LOT INFO
        {
            if(SystemStart==false)
            {
                fSCKART->iWaitGPIBLotR=0;
                ZeroMemory(iAutoTrayCount, sizeof(iAutoTrayCount));             //Sam 20191113 : TCP ART
                if(HasICUnderMachine()==false)
                {
                    fMain->Clarn_Data(1, "ART_LOTCLEARED");
                    fSCKART->ClearLotInfo();
                    RecordProcess("ART LOTCLEARED.");
                    fLotInfo->btClearBarcodeList->Click();                      //Steven 20190214 : 統一清除2DID方式
                    slDupBundlID->Clear();
                    slDupBundlID->SaveToFile(asDupBundleID);
                    slDupUnloadBundlID->Clear();
                    slDupUnloadBundlID->SaveToFile(aslDupUnloadBundlID);
                    HCACK=0;
                }
                else
                {
                    HCACK=2;
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("LOTORDER")==1)                                       //JerryYang 20200330 : 修改SPIL LOT ORDER
        {
            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
            {
                HGem->GetDataItemLenAndType(len, Type);
                CommandStr=new char [len+1];                                    //JerryYang 20241205 : 字串陣列大小改成動態
//                ZeroMemory(CommandStr, sizeof(CommandStr));
                memset(CommandStr, 0, len+1);                                   //JerryYang 20250325 : fix
                ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                if(ret==-1)
                    S2="";
                S2=CommandStr;
                delete[] CommandStr;                                            //Steven 20160912 : Add delete for save memory
                CommandStr=NULL;

                if(S2=="ORDER")
                {
                    HGem->GetDataItemLenAndType(len, Type);
                    CommandStr=new char [len+1];                                //JerryYang 20241205 : 字串陣列大小改成動態
//                    ZeroMemory(CommandStr, sizeof(CommandStr));
                    memset(CommandStr, 0, len+1);                               //JerryYang 20250325 : fix
                    ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                    if(ret==-1)
                        S3="";
                    S3=CommandStr;
                    delete[] CommandStr;                                        //Steven 20160912 : Add delete for save memory
                    CommandStr=NULL;
                    if(S3==0 || S3==1 || S3==2 || S3==3)
                    {
                        HCACK=0;
                        if(S3.Pos("0")!=0 || S3.Pos("1")!=0)
                        {
                            if(fSCKART->iFTRTCount==0)
                            {
                                fSCKART->iCurrent93KARTStep=3;
                            }
                            else
                            {
                                fSCKART->iCurrent93KARTStep=9;
                            }
                            LastSet.bWaitStartLotAutoRetestGPIB=true;           //SECS GEM ART
                        }
                        else if(S3.Pos("2")!=0 || S3.Pos("3")!=0)
                        {
                            if(fSCKART->iCurrent93KARTStep>=10)
                                fSCKART->iCurrent93KARTStep=12;
                            LastSet.bWaitEndLotAutoRetestGPIB=true;
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                    else
                    {
                        HCACK=1;
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("TRAY_MAP")==1)                                       //KenHsieh 20220912 : add AOSH 1600LT Tray map CCD Command
        {
            HCACK=0;
        }
        else if(S.AnsiPos("SET_2DID_BIN_CODE")==1)                              //KenHsieh 20220912 : add AOSH 1600LT Tray map CCD Command
        {
            if(HasICUnderMachine()==true || SystemStart==true)
            {
                HCACK=4;
            }
            else
            {
                sTotalLotID="";

                HGem->GetDataItemLenAndType(len, Type);
                if(Type==HType.ASCII_TYPE)
                {
                    CommandStr=new char [len+1];                                //JerryYang 20241205 : 字串陣列大小改成動態
//                    ZeroMemory(CommandStr, sizeof(CommandStr));
                    memset(CommandStr, 0, len+1);                               //JerryYang 20250325 : fix
                    ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                    if(ret==-1)
                    {
                        HCACK=3;
                    }
                    else
                    {
                        HCACK=S2F42_SET_2DID_BIN_CODE(CommandStr);              //Steven 20241008 : S2F42的XML換位置
                    }
                    delete[] CommandStr;                                        //Steven 20160912 : Add delete for save memory
                    CommandStr=NULL;
                }
                else
                {
                    HCACK=3;
                }
            }
        }
        else if(S=="START_LOT")                                                 //Ifor 20221019 add: Onsemi START_LOT
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE)==1)
            {
                if(SVlen==0)
                {
                    SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                    HCACK=0;
                }
                else
                {
                    for(i=0; i<SVlen; i++)
                    {
                        if(HGem->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len,Type);              //取得資料長度與格式
                            HGem->DataItemIn( len,Type,str);                    //取值

                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, strLotID);
                                S1=str;
                                HCACK=0;
                                if(S1=="LOTID")
                                {
                                    fLotInfo->edtSysLotID->Text=strLotID;
                                }
                                else if(S1=="DEVICEID")
                                {
                                    fLotInfo->edtDevice->Text=strLotID;
                                }
                                else if(S1=="OPERATORID")
                                {
                                    fLotInfo->edtSysOperatorID->Text=strLotID;
                                }
                                else if(S1=="RUN_MODE")                         //RogerYang 20260608 : Add for VTEST SECS (LotID、OperatorID、Mode(FT0-FT15、RT0-RT5))
                                {
                                    if(IniConfig.bVTESTFunction==true)
                                    {
                                        if(fLotInfo->cbRunMode->Items->IndexOf(strLotID)>=0)
                                        {
                                            fLotInfo->cbRunMode->Text=strLotID;
                                        }
                                        else
                                        {
                                            AnsiString sLog;
                                            sLog.sprintf("[S2F42] START_LOT RUN_MODE value '%s' not in list", strLotID);
                                            HGemPtr->StringOut(sLog);
                                            HCACK=3;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        fLotInfo->cbRunMode->Text=strLotID;
                                    }
                                }
                                else if(S1=="TEST_TIMES")                       //RogerYang 20260608 : Add for VTEST SECS (LotID、OperatorID、Mode(FT0-FT15、RT0-RT5))
                                {
                                    if(fLotInfo->cbTestTimes->Items->IndexOf(strLotID)>=0)
                                    {
                                        fLotInfo->cbTestTimes->Text=strLotID;
                                    }
                                    else
                                    {
                                        AnsiString sLog;
                                        sLog.sprintf("[S2F42] START_LOT TEST_TIMES value '%s' not in list", strLotID);
                                        HGemPtr->StringOut(sLog);
                                        HCACK=3;
                                        break;
                                    }
                                }
                                else
                                {
                                    HCACK=1;
                                }
                            }
                            else
                            {
                                HCACK=1;
                            }
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }

                    if(HCACK==0)
                        fLotInfo->sbSECSLotStartClick(fLotInfo);
                }
            }
            else
            {
                HCACK=1;
            }
        }
        //==> Eastsun 20260520 整合
        else if(S.AnsiPos("START_AQL")==1)                                      //Ifor 20240311 "START_AQL"移至"START"上方避免直接進入"START"命令
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE)==1)  //wei 20150630
            {
                if(HasICUnderMachine()==false && IniConfig.bI52_bAQLSortMode==true)
                {
                    if(SVlen==0)
                    {
                        HCACK=1;
                    }
                    else
                    {
                        for(i=0;i<SVlen;i++)
                        {
                            if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                HGem->DataItemIn( len,Type,str); //取值

                                HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                HGem->DataItemIn( len,Type,&ret); //取值
                                S1=str;
//                                HCACK=0;          //判斷後再回覆
                                if(S1=="COUNT")
                                    iAQLCount=ret;
                                else if(S1=="BIN")
                                    iAQLBin=ret;
                            }
                            else
                            {
                                HCACK=1;
                            }
                        }

                        if(fLotInfo->SetAQLMode(true)==false)
                        {
                             HCACK=1;
                        }
                        else
                        {
                            HCACK=0;
                        }
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=1;
            }
        }
        //<== Eastsun 20260520 整合
        //==> Eastsun 20260520 整合
        else if(S.AnsiPos("START_AGV")==1)
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE)==1)    //wei 20150630
            {
                if(SVlen==0)
                {
                    SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                    HCACK=0;
                }
                else
                {
                    for(i=0;i<SVlen;i++)
                    {
                        if(HGem->DataItemIn(2,HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                            HGem->DataItemIn(len,Type,S1); //取值

                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                            HGem->DataItemIn(len,Type,S2); //取值

                            if(S1=="Loader" && S2=="Action")
                            {
                                bLoaderSECSActionFlag[0]=true;
                                fLotInfo->InitialLoaderTask(0);
                            }
                            else if(S1=="LoaderTrayCount")
                            {
                                iSECSSetTrayCount=atoi(S2.c_str());
                            }
                            else if(S1=="Empty" && S2=="Action")
                            {
                                bLoaderSECSActionFlag[1]=true;
                                fLotInfo->InitialLoaderTask(1);
                            }
                            else if(S1=="Color" && S2=="Action")
                            {
                                bLoaderSECSActionFlag[2]=true;
                                fLotInfo->InitialLoaderTask(2);
                            }
                            else if(S1=="AUTO1" && S2=="Action")
                            {
                                bUnLoaderSECSActionFlag[0]=true;
                                fLotInfo->InitialUnLoaderTask(0);
                            }
                            else if(S1=="AUTO2" && S2=="Action")
                            {
                                bUnLoaderSECSActionFlag[1]=true;
                                fLotInfo->InitialUnLoaderTask(1);
                            }
                            else if(S1=="AUTO3" && S2=="Action")
                            {
                                bUnLoaderSECSActionFlag[2]=true;
                                fLotInfo->InitialUnLoaderTask(2);
                            }

                            HCACK=0;
                        }
                        else
                        {
                            HCACK=1;
                        }
                    }
                }
            }
            else
            {
                HCACK=1;
            }
        }
        //<== Eastsun 20260520 整合
        else if(S=="STOP_LOT")                                                  //Ifor 20221019 add: Onsemi STOP_LOT
        {
            if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
            {
                if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                {
                    HGem->GetDataItemLenAndType(len,Type);                      //取得資料長度與格式
                    HGem->DataItemIn( len,Type,str);                            //取值
                    S1=str;
                    HCACK=0;
                    if(S1=="LOTID")
                    {
                        HGemPtr->GetDataItemLenAndType(len, Type);
                        if(Type==HType.ASCII_TYPE)
                        {
                            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, strLotID);
                            if(ret==-1)
                                strLotID="";
                        }
                        else
                        {
                            strLotID="";
                        }
                    }
                    else
                    {
                        strLotID="";
                    }

                    sLotNo=fLotInfo->edtSysLotID->Text;
                    if(sLotNo.Pos(strLotID)>=1)
                    {
                        fMain->CleanOut("S2F42 2");
                        HCACK=0;
                    }
                    else
                    {
                        HCACK=1;
                    }
                }
                else
                {
                    HCACK=1;
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("SKIP")==1)
        {
            if(fNote->fShow)
            {
                fNote->ReturnCode=K_SKIP;
                fNote->Close();
            }
            HCACK=0;                                                        //Steven 20260415 : align with RETRY
        }
        else if(S.AnsiPos("TERMINAL_DISPLAY")==1)                               //Ifor 20251018 add:Analog 泰國客戶要求新增同1028 SECS GEM 命令關閉視窗
        {
            if(MyMessageBox->Visible==true)
            {
                MyMessageBox->Close();
            }
            HCACK=0;
        }
        // ======================================================================
        // SET_RECIPE — HCACK 完整示範 RCMD                         Ifor 20260423
        // S2F41 格式：
        //   <L2
        //     <A "SET_RECIPE">
        //     <L1
        //       <L2 <A "PPID"> <A "RecipeName">>
        //     >
        //   >
        // HCACK 回傳規則：
        //   0 = 執行完成（同步）
        //   1 = 指令不存在（不應到此，防禦性保留）
        //   2 = 目前狀態不允許（機台運行中且不支援非同步）
        //   3 = 參數不合法（PPID 格式錯誤）
        //   4 = 接受，非同步執行（機台運行中，待 Idle 後切換，S6F11 通知）
        //   5 = 已在目標狀態（當前工作檔與要求相同）
        //   6 = 找不到物件（PPID 不在工作檔清單中）
        // ======================================================================
        else if(S.AnsiPos("SET_RECIPE")==1)                                     //Ifor 20260423 : HCACK 0-6 完整示範 RCMD
        {
            AnsiString sNewPPID = "";
            bool bParamValid    = false;
            bool bFoundInList   = false;

            // ---- 解析參數：讀取 CPNAME="PPID" / CPVAL="RecipeName" ----
            if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL) == 1)
            {
                HGem->GetDataItemLenAndType(len, Type);
                CommandStr = new char[len + 1];
                memset(CommandStr, 0, len + 1);
                ret = HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                S1 = CommandStr;
                delete[] CommandStr;
                CommandStr = NULL;

                if(S1 == "PPID")
                {
                    HGem->GetDataItemLenAndType(len, Type);
                    if(Type == HType.ASCII_TYPE && len > 0 && len <= 256)       // 防禦：長度範圍合法
                    {
                        CommandStr = new char[len + 1];
                        memset(CommandStr, 0, len + 1);
                        ret = HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                        sNewPPID    = CommandStr;
                        bParamValid = (sNewPPID.Length() > 0);                  // 非空字串才合法
                        delete[] CommandStr;
                        CommandStr = NULL;
                    }
                    else
                    {
                        bParamValid = false;                                     // Type 或長度不合法
                    }
                }
            }

            if(!bParamValid)
            {
                // HCACK=3：CPNAME/CPVAL 不合法
                HCACK = 3;
            }
            else
            {
                // 確認 PPID 是否存在於工作檔清單
                for(int k = 0; k < fMain->cbSetupFileName->Items->Count; k++)
                {
                    if(sNewPPID == fMain->cbSetupFileName->Items->Strings[k])
                    {
                        bFoundInList = true;
                        break;
                    }
                }

                if(!bFoundInList)
                {
                    // HCACK=6：找不到指定的工作檔物件
                    HCACK = 6;
                }
                else if(sNewPPID == fMain->cbSetupFileName->Text)
                {
                    // HCACK=5：設備已在目標工作檔狀態，拒絕重複切換
                    HCACK = 5;
                }
                else if(SystemStart || HasICUnderMachine())
                {
                    // HCACK=4：接受指令，非同步執行（機台 Idle 後再切換）
                    // 儲存非同步指令，待 CheckAndExecuteAsyncRCMD() 執行後
                    // 透過 S6F11 CEID=SwitchSetupFile 通知主機
                    m_AsyncRCMD.bPending = true;
                    m_AsyncRCMD.sCommand = "SET_RECIPE";
                    m_AsyncRCMD.sParam   = sNewPPID;
                    HCACK = 4;
                    RecordProcess("[S2F42] SET_RECIPE accepted async, PPID=" + sNewPPID);
                }
                else
                {
                    // HCACK=0：同步執行，切換工作檔完成
                    fMain->cbSetupFileName->Text = sNewPPID;
                    fMain->cbSetupFileNameChange(fMain);
                    HGemPtr->EventReport(1, SECS_EVENT.SwitchSetupFile);        // CEID=15 通知 Host
                    RecordProcess("[S2F42] SET_RECIPE done, PPID=" + sNewPPID);
                    HCACK = 0;
                }
            }
        }
        else if(S=="RECIPEVERIFYACK" || S=="RUNCHECKEND")                       //RogerYang 20260610 : XINYUN runcheck 結果回覆
        {
            AnsiString asName="", asValue="", asResult="", asReason="";         //AI(ht9045-secs-sem) 20260622 (RogerYang) : 改 AnsiString+動態長度, 根除固定buffer off-by-one(X7)與reason截斷(X5), 並以名稱對應不綁順序
            int iParamCnt=0;
            unsigned char tType;
            if(HGem->GetDataItemLenAndTypeAndDelete(iParamCnt, tType)==1 && tType==HType.LIST_TYPE)
            {
                for(i=0; i<iParamCnt; i++)                                      // 逐組 <A name><A value>
                {
                    if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                    {
                        HGem->GetDataItemLenAndType(len, Type);                 // name 實際長度/型別
                        HGem->DataItemIn(len, Type, asName);
                        HGem->GetDataItemLenAndType(len, Type);                 // value 實際長度/型別
                        HGem->DataItemIn(len, Type, asValue);
                        if(asName.UpperCase()=="RESULT")
                            asResult=asValue.Trim();                            //AI(ht9045-secs-sem) 20260622 (RogerYang) : 依名稱取值
                        else if(asName.UpperCase()=="REASON")
                            asReason=asValue;
                    }
                }
            }
            if(asResult=="0")                                                   // 0=pass 1=fail
            {
                if(IniConfig.bEnable_SECS_GEM==true &&
                   ((IniConfig.bRCMDStart==true && bPhysicalStart==true) ||
                    bSecsGemCanStart))                                          // 與 START case 條件一致
                {
                    SoftStart=true;
                    bSecsGemCanStart=false;
                    bSECSGEMAlarm=false;
                    bHasSaveSet=false;
                    RecordProcess("SECS/GEM RunCheck Pass, Start!");
                }
                HCACK=0;
            }
            else                                                                // fail
            {
                RecordProcess("SECS/GEM RunCheck Fail, Reason="+asReason);
                if(asReason!="")
                {
                    if(CUSTOMER_CODE==CC_XINYUN)                                 // XINYUN reason 為長 JSON, 插換行讓 memo word-wrap
                    {
                        AnsiString asWrap="";
                        int iCol=0;
                        for(int k=1; k<=asReason.Length(); k++)
                        {
                            asWrap+=asReason[k];
                            iCol++;
                            if(asReason[k]==',' || iCol>=80)                     // JSON 逗號或滿 80 字就換行
                            {
                                asWrap+="\r\n";
                                iCol=0;
                            }
                        }
                        asReason=asWrap;
                    }
                    SecsAlarmMessage->Add("RunCheck Failed#" + asReason);
                }
                HCACK=0;
            }
            bPhysicalStart=false;
            bNeedDoRunCheck=false;
        }
        else if(S=="START")                                                     //RogerYang 20260610 : 改強制相等，與"PAUSE"都搬到下面     //Steven 20141016 : Add RCMD START
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE && CosFunction.bUseSECSGEMAutoStart==true) //Eastsun 20260515 F019 AMR SECS/GEM Auto Start
            {
                if(TrayForm.bEnableAMR)
                {
                    if(RunInfo.bLotStart==true)
                    {
                        bSameSetupFileNoDownload=true;
                        RecordProcess("SECS/GEM Auto Start!");
                        bAMRReceiveStart=true;
                        fLotInfo->CheckActionFlag();
                    }
                }
                else
                {
                    RecordProcess("SECS/GEM Auto Start!");
                    fMain->Start("SECS GEM RCMD : START Auto Start");
                }

                if(IniConfig.bI52_bAQLSortMode==true && bSetAQLSortMode==true)
                {
                    bStartAQLSortMode=true;
                    bSetAQLSortMode=false;
                }
                bSecsGemCanStart=false;
                HCACK=0;
                bSECSGEMAlarm=false;
                bHasSaveSet=false;
            }
            else if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND || CUSTOMER_CODE==CC_IFXTH_Thai)
            {
                if(SystemStart==false)
                {
                    HCACK=0;
                    fMain->Start("SECS GEM RCMD : START");
                }
                else
                {
                    HCACK=1;
                }
            }
            else if(IniConfig.bEnable_SECS_GEM==true &&
                    CosFunction.bCanRemoteStart)                                //Steven 20250712 : 有通過風險告知可以遠端start的客戶
            {
                HCACK=0;
                bSECSGEMAlarm=false;                                            //Ifor 20151208 :解除按鍵Lock
                bHasSaveSet=false;                                              //Ifor 20151208 :清除設定檔變更旗標
                fMain->Start("SECS GEM RCMD : START");
            }
            else if(IniConfig.bEnable_SECS_GEM==true &&                         //Steven 20141006 : SECS GEM使用Remote Start功能
                    ((IniConfig.bRCMDStart==true && bPhysicalStart==true) ||
                    bSecsGemCanStart))                                          //wei 20150630  SecsGem Can Start
            {
                SoftStart=true;
                RecordProcess("SECS GEM RCMD : RUN CHECK START");
                bSecsGemCanStart=false;
                HCACK=0;
                bSECSGEMAlarm=false;                                            //Ifor 20151208 :解除按鍵Lock
                bHasSaveSet=false;                                              //Ifor 20151208 :清除設定檔變更旗標
            }
            else
            {
                HCACK=1;
            }
            bPhysicalStart=false;
            bNeedDoRunCheck=false;                                              //JerryYang 20250120 : modify
        }
        else if(S.AnsiPos("PAUSE")==1 || S=="STOP")                             //Steven 20221218 : S2F41字串判斷從原本的S=="" 改成 S.AnsiPos()==1
        {
            if(CUSTOMER_CODE==CC_MAXIM || CUSTOMER_CODE==CC_MAXIM_THAILAND)
            {
                if(fNote->fShow)
                {
                    fNote->BtnPauseClick(fNote);
                    fNote->Close();
                }

                fMain->BtnPauseClick(fMain);
                HCACK=0;
            }
            else
            {
                fMain->BtnPauseClick(fMain);
                if(IniConfig.bEnable_SECS_GEM==true &&
                   IniConfig.bRCMDStart==true &&
                   bPhysicalStart==true)                                        //Wei 20150815 : Fixed for  RCMD Start
                {
                    bPhysicalStart=false;
                }

                if(CUSTOMER_CODE==CC_ASE_CL)                                    //RogerYang 20260413 Add
                {
                    bSECSPause=true;                                            //JerryYang 20250120 : modify
                }

                if(CosFunction.RunCheckWhenRecPause )                           //JerryYang 20250120 : modify
                {
                    bNeedDoRunCheck=true;
                }

                if(SystemStart==false)
                {
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                      //KenHsieh 20220913 : ASEKH K1要求回傳2
                        HCACK=2;
                    else if(IniConfig.bVTESTFunction)                           //RogerYang 20260608 : Add for VTEST SECS
                        HCACK=5;                                                //5 = Rejected, Already inDesired Condition
                    else
                        HCACK=1;
                }
                else
                {
                    HCACK=0;
                }
            }
        }
        // ======================================================================
        else                                                                    //20140124 wei
        {
            HCACK=1;
        }

//        delete[] CommandStr;                                                  //JerryYang 20241205 : 字串陣列大小改成動態
//        CommandStr=NULL;

        HGemPtr->InitLocalHead(2, 42, 0);

//        if(HCACK==0)
//        {
//            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
//            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
//            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
//        }
//        else
//        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
//        }

        HGemPtr->SendLocalData();
        return 1;
    }
    else
    {
        HCACK=3;
        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
        HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
    }
    return HCACK;
}
//------------------------------------------------------------------------------
void HT9045Gem::S5F6_ListAlarmData()                                            //Steven 20150520 : 修正S5F6 ListAlarmData
{
    int SVlen, ret;
    unsigned char Type;
    AnsiString S;

    unsigned char ALCD=0x80;
    unsigned int  ALID;
    AnsiString    ALTX;

    if(HGemPtr->GetDataItemLenAndType(SVlen, Type)==1)
    {
        if(SVlen!=0)
        {
            if(Type==HType.UINT_8_TYPE || Type==HType.UINT_4_TYPE || Type==HType.UINT_2_TYPE ||
               Type==HType.INT_8_TYPE  || Type==HType.INT_4_TYPE  || Type==HType.INT_2_TYPE)                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64 *uint8Ptr;
                unsigned *uint4Ptr;
                unsigned short *uint2Ptr;
                __int64  *int8Ptr;
                int      *int4Ptr;
                short    *int2Ptr;

                ret=HGem->GetDataItemLenAndType(SVlen, Type);
                if(ret!=1)
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    return;
                }

                if(Type==HType.UINT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    uint8Ptr=new unsigned __int64 [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_8_TYPE, uint8Ptr);
                }
                else if(Type==HType.UINT_4_TYPE)
                {
                    uint4Ptr=new unsigned [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_4_TYPE, uint4Ptr);
                }
                else if(Type==HType.UINT_2_TYPE)
                {
                    uint2Ptr=new unsigned short [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_2_TYPE, uint2Ptr);
                }
                else if(Type==HType.INT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    int8Ptr=new __int64[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_8_TYPE, int8Ptr);
                }
                else if(Type==HType.INT_4_TYPE)
                {
                    int4Ptr=new int[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_4_TYPE, int4Ptr);
                }
                else if(Type==HType.INT_2_TYPE)
                {
                    int2Ptr=new short[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_2_TYPE, int2Ptr);
                }

                if(ret==1)
                {
                    HGem->InitLocalHead(5, 6, 0);
                    HGem->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                    for(int i=0; i<SVlen; i++)
                    {
                        if(Type==HType.UINT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=uint8Ptr[i];
                        else if(Type==HType.UINT_4_TYPE)
                            S=uint4Ptr[i];
                        else if(Type==HType.UINT_2_TYPE)
                            S=uint2Ptr[i];
                        else if(Type==HType.INT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=int8Ptr[i];
                        else if(Type==HType.INT_4_TYPE)
                            S=int4Ptr[i];
                        else if(Type==HType.INT_2_TYPE)
                            S=int2Ptr[i];

                        int j=HGem->GetAlarmIndex(S);

                        if(j>=0)
                        {
                            HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                            ALCD=atoi(HGem->strGrdAlarm->Cells[7][j].c_str())+0x80;
                            HGem->DataItemOut(1, HType.BINARY_TYPE, &ALCD);     // mode

                            ALID=_atoi64(HGem->strGrdAlarm->Cells[8][j].c_str());
                            HGem->DataItemOut(1, HType.INT_8_TYPE, &ALID);      // ID

                            ALTX=HGem->strGrdAlarm->Cells[6][j];                // message
                            HGem->DataItemOut(HType.ASCII_TYPE, ALTX);
                        }
                        else
                        {
                            HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                            ALCD=0x80;
                            HGem->DataItemOut(0, HType.BINARY_TYPE, &ALCD);     // mode

                            ALID=42;
                            HGem->DataItemOut(1, HType.INT_8_TYPE, &ALID);      // ID

                            ALTX="Unknown Alarm Code";                          // message
                            HGem->DataItemOut(HType.ASCII_TYPE, ALTX);
                        }
                    }
                    HGem->SendLocalData();
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }

                    return;
                }
                else
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }
                    return;
                }
            }
            else
            {
                S9F7_IllegalData("S5,F5 Data Format error !!!");
            }
        }
        else
        {
            HGem->InitLocalHead(5, 6, 0);
            HGem->DataItemOut(HGem->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
            for(int i=1; i<HGem->strGrdAlarm->RowCount; i++)
            {
                HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                ALCD=atoi(HGem->strGrdAlarm->Cells[7][i].c_str())+0x80;
                HGem->DataItemOut(1, HType.BINARY_TYPE, &ALCD);                 // mode

                ALID=atoi(HGem->strGrdAlarm->Cells[8][i].c_str());
                HGem->DataItemOut(1, HType.UINT_4_TYPE, &ALID);                 // ID

                ALTX=HGem->strGrdAlarm->Cells[6][i];                            // message
                HGem->DataItemOut(HType.ASCII_TYPE, ALTX);
            }
        }
    }

    HGem->SendLocalData();
    return;
}
//------------------------------------------------------------------------------
int HT9045Gem::S7F2_ProcessProgramLoadGrant()
{
    unsigned char HCACK=1;
    int ret, len;
    unsigned char Type;
    AnsiString PPID;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            HGemPtr->DataItemIn(len, Type, PPID);
            HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type);
        }
    }

    TRegExpr * regex    =new TRegExpr;
    regex->Expression   ="[/|\\*|\\?|:|\"|>|<|\\|]";                            //不等於裡面指定的文字
    regex->InputString=PPID;

    //Steven 20220422 : 加強S7F2_ProcessProgramLoadGrant的回覆
    if(SystemStart==true)                                                       //20140124 wei
    {
        HCACK=6;
    }
    else if(PPID.Length()<=0)
    {
        HCACK=7;
    }
    else if(regex->Exec())
    {
        HCACK=8;
    }
    else if(MOT[MMTrayZ].fHasTray || MOT[MMTrayY].fHasTray)
    {
        HCACK=9;
    }
    else if(MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())
    {
        HCACK=10;
    }
    else if(InArmSuck.HasIC())
    {
        HCACK=11;
    }
    else if(OutArmSuck.HasIC())
    {
        HCACK=12;
    }
    else if(FLCarryKit.UseSiteHasIC() || BLCarryKit.UseSiteHasIC())
    {
        HCACK=13;
    }
    else if(FRCarryKit.UseSiteHasIC() || BRCarryKit.UseSiteHasIC())
    {
        HCACK=14;
    }
    else if(IndexHasIC())
    {
        HCACK=15;
    }
    else if(MOT[MInRotateKit].HasIC())
    {
        HCACK=16;
    }
    else if(MOT[MOutRotateKit].HasIC())
    {
        HCACK=17;
    }
    else
    {
        HCACK=0;
    }

    HGemPtr->InitLocalHead(7, 2, 0);
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
    HGemPtr->SendLocalData();
    return HCACK;
}
//-----------------------------------------------------------------------------
void HT9045Gem::S7F4_ProcessProgramAcknowledge()                                //Steven 20140911 : Modify S7F4 for HT9045
{
//L,2
//    1. <PPID>
//    2. <PPBODY>    int ret,len;
    unsigned char Type;
    char *PPBODY;
    FILE *stream;
    HINSTANCE hInstance;
    AnsiString PPID, S, Str, PPIDFolder, str1, str2, str3, sDir, strSetupfile, sOffsetOrg, sOffset, sLastSetup;
    AnsiString NewFolder, NewFile, OrgFolder, OrgFile;

    AnsiString sConfigPath=FileInfo().PathCombin(AuthPath, "Security_new.def");
    bool b2Dsort=false;
    int ret, len;
    int iStart, iEnd;                                                           //, iRNPos;
    bool bHasSetUpFile=false, bNeedCover=false;                                 //JerryYang 20170126 (Steven) 是否已存在此工作檔
    double fTempUserOffset[tcTotalCount], fContactHeight[20], dB2DOffsetX, dB2DOffsetY, dATCOffsetBackup[32];
    int iIndexHeatingMode;
    int iShuttleMode[2]={0, 0},iAutoClean[24];                                  //JerryYang 20191024 15->24
    AnsiString strCleanCnt[20][10];                                             //KenHsieh 20230518 : Auto Clean count不覆蓋
    AnsiString asCleanCt;                                                       //KenHsieh 20230518 : Auto Clean count不覆蓋

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            if(CUSTOMER_CODE==CC_MAXIM_THAILAND)                                //Ifor 20251018 add:Analog 泰國客戶要求PPID 需要多一層L
            {
                HGemPtr->GetDataItemLenAndType(len, Type);
                ret=HGemPtr->DataItemIn(len, Type, PPID);                       // PPID=filename;
            }
            else
            {
                HGemPtr->DataItemIn(len, Type, PPID);                           // PPID=filename;
                ret=HGemPtr->GetDataItemLenAndType(len, Type);
            }

            if(ret==1 && (Type==HType.ASCII_TYPE ||
                          Type==HType.BINARY_TYPE))                             //JerryYang 20230221 : S7F4可以接收Binary
            {
                PPBODY=new char [len+1];                                        //JerryYang 20190411 因為有結束位元，長度+1

                if(HGemPtr->DataItemIn(len, Type, PPBODY))
                {
                    RecordProcess("S7F4_ProcessProgramAcknowledge");
                    PPIDFolder.sprintf("D:\\HT9045\\IniData\\Data\\%s", PPID);

                    if(DirectoryExists(PPIDFolder)==false)
                    {
                        HGemPtr->StringOut("S7F4, Make folder :"+PPID);
                        MyForceDirectories(PPIDFolder, "S7F4_ProcessProgramAcknowledge");
                        bHasSetUpFile=false;                                    //JerryYang 20170126 (Steven) 本機不存在欲Download的setup file
                    }
                    else
                    {
                        HGemPtr->StringOut("S7F4, Has folder :"+PPID);
                        bHasSetUpFile=true;                                     //JerryYang 20170126 (Steven) 已存在此工作檔
                    }

                    if(bHasSetUpFile)                                           //JerryYang 20170126 (Steven) 把要還原的資料先備份
                    {
                        strSetupfile=PPID;
                    }
                    else
                    {
                        strSetupfile=GetLastOpenFN();
                    }

                    //----------------------
                    //檢查Offset檔
                    //----------------------
                    if(IniConfig.bE45_AllSetupFileUseOneFile==false)            //JerryYang 20220324 : 如果沒有offset就從當下的工作檔複製過來
                    {
                        NewFolder=FileInfo().PathCombin(fOffSet->GetOffsetPath(PPID), PPID);
                        MyForceDirectories(NewFolder);
                        NewFile  =FileInfo().PathCombin(NewFolder, "Position Offset.Data");
                        if(!FileExists(NewFile))                                //如果沒有就從當下的工作檔複製過來
                        {
                            OrgFile=FileInfo().PathCombin(fOffSet->GetOffsetPath(strSetupfile), "Position Offset.Data");
                            CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        }

                        NewFolder=FileInfo().PathCombin(fOffSet->GetOffsetPath(PPID), PPID);
                        NewFile  =FileInfo().PathCombin(NewFolder, "Position Offset Hot.Data");
                        if(!FileExists(NewFile))                                //如果沒有就從當下的工作檔複製過來
                        {
                            OrgFile=FileInfo().PathCombin(fOffSet->GetOffsetPath(strSetupfile), "Position Offset Hot.Data");
                            CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        }
                    }
                    //----------------------
                    //把溫度Offset資料備份
                    //----------------------
                    OrgFolder=FileInfo().PathCombin(DataPath, strSetupfile);
                    NewFolder=DataPath;

                    str1=FileInfo().PathCombin(OrgFolder, "Temperature.Data");
                    for(int i=0; i<tcTotalCount; i++)
                    {
                        str2.printf("CH%d", i+1);
                        fTempUserOffset[i]=ReadIniData(str1, "User OffSet", str2, 0.0);
                    }
                    iIndexHeatingMode=CheckAndReadIniData(str1, "Index",   "Heating Mode",   0);                        //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式

                    if(ATC_SYSTEM>eATC60)                                       //JerryYang 20220324 : 複製ATC溫度offset
                    {
                        if(ATC_SYSTEM!=eNonChamber)
                        {
                            if(CosFunction.bUseOldATCTempOffset==true)          //JerryYang 20190123 ATC Offset UI使用舊版
                            {
                            }
                            else
                            {
                                for(int i=0; i<32; i++)
                                {
                                    str2.sprintf("ATCTempOffset[%d]", i);
                                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)         //kevin 20180215 (Steven) ADD atc
                                        dATCOffsetBackup[i]=CheckRange(ReadIniData(str1, "ATC", str2, 0.0), 60.0, -60.0);
                                    else
                                        dATCOffsetBackup[i]=CheckRange(ReadIniData(str1, "ATC", str2, 0.0), (double)InputLimit.iTempHigh, (double)InputLimit.iTempLow);
                                }
                            }
                        }
                    }
                    MySleep(100);

                    //----------------------
                    //把HotPlate資料備份
                    //----------------------
                    OrgFile=FileInfo().PathCombin(OrgFolder, "HotPlate.Data");
                    NewFile=FileInfo().PathCombin(NewFolder, "HotPlate.Data");
                    ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                    MySleep(100);

                    //----------------------
                    //把Contact高度備份
                    //----------------------
                    str1=FileInfo().PathCombin(OrgFolder, "Contact.Data");
                    fContactHeight[0]=ReadIniData(str1, "Test Arm1", "Pick Up", 0.0);
                    fContactHeight[1]=ReadIniData(str1, "Test Arm1", "Contact", 0.0);
                    fContactHeight[2]=ReadIniData(str1, "Test Arm1", "Drop",    0.0);
                    fContactHeight[3]=ReadIniData(str1, "Test Arm1", "Place",   0.0);
                    fContactHeight[4]=ReadIniData(str1, "Test Arm2", "Pick Up", 0.0);
                    fContactHeight[5]=ReadIniData(str1, "Test Arm2", "Contact", 0.0);
                    fContactHeight[6]=ReadIniData(str1, "Test Arm2", "Drop",    0.0);
                    fContactHeight[7]=ReadIniData(str1, "Test Arm2", "Place",   0.0);
                    fContactHeight[8]=ReadIniData(str1, "Torque Control", "Pin Number",    0.0);
                    fContactHeight[9]=ReadIniData(str1, "Torque Control", "Force Per Pin", 0.0);
                    fContactHeight[19]=ReadIniData(str1,"Torque Control", "Torque",        0.0);

                    fContactHeight[10]=ReadIniData(str1, "Wait Time", "Drop Wait",   1.0);
                    fContactHeight[11]=ReadIniData(str1, "Wait Time", "Drop Speed",  1.0);
                    fContactHeight[12]=ReadIniData(str1, "Mode", "Contact",                          0.0);
                    fContactHeight[13]=ReadIniData(str1, "Mode", "Vacuum",                           0.0);
                    fContactHeight[14]=ReadIniData(str1, "Mode", "Dummy Contact",                    0.0);
                    fContactHeight[15]=ReadIniData(str1, "Mode", "Head Device Mode",                 0.0);
                    fContactHeight[16]=ReadIniData(str1, "Mode", "Kit Diameter",                     3.0);
                    fContactHeight[17]=ReadIniData(str1, "Mode", "Suck Shuttle Device After Tested", 0.0);
                    fContactHeight[18]=ReadIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber", 0.0);

                    //----------------------
                    //把Ld/Uld速度備份
                    //----------------------
                    OrgFile=FileInfo().PathCombin(OrgFolder, "UdUld.Data");
                    NewFile=FileInfo().PathCombin(NewFolder, "UdUld.Data");
                    ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                    MySleep(100);

                    //----------------------
                    //把速度設定備份
                    //----------------------
                    OrgFile=FileInfo().PathCombin(OrgFolder, "ArmCondition.Data");
                    NewFile=FileInfo().PathCombin(NewFolder, "ArmCondition.Data");
                    ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                    MySleep(100);

                    //----------------------
                    //備份開啟單一Shuttle選項
                    //----------------------
                    str1=FileInfo().PathCombin(OrgFolder, "HandlerCondition.Data");
                    iShuttleMode[0]=ReadIniData(str1, "Configuration", "Shuttle Mode", 0);
                    iShuttleMode[1]=ReadIniData(str1, "Configuration", "Shuttle1 Cancel", 0);

                    //----------------------
                    //備份Auto Clean選項
                    //----------------------                                    //Steven 20161116 : ATC說要加上Auto Clean
                    str1=FileInfo().PathCombin(OrgFolder, "HandlerCondition.Data");
                    iAutoClean[0]=ReadIniData(str1,  "Configuration",  "iAutoClean_Function",            0);
                    iAutoClean[1]=ReadIniData(str1,  "Configuration",  "iAutoClean_SelectArm",           0);
                    iAutoClean[2]=ReadIniData(str1,  "Configuration",  "iAutoClean_ContactShiftHeight",  1000);
                    iAutoClean[3]=ReadIniData(str1,  "Configuration",  "iAutoClean_ContactCleanHeight",  0);
                    iAutoClean[4]=ReadIniData(str1,  "Configuration",  "iAutoClean_IndexPickOffset",     0);            //wei 20150318 Auto clean Index Pick Offset
                    iAutoClean[5]=ReadIniData(str1,  "Configuration",  "iAutoClean_IndexReleaseOffset",  0);            //Jou 2015-08-22 Auto clean Index Release Offset
                    iAutoClean[6]=ReadIniData(str1,  "Configuration",  "HotplatlXOffset",                0.0);          //JerryYang 20161122 補上Auto Clean參數
                    iAutoClean[7]=ReadIniData(str1,  "Configuration",  "HotplatlYOffset",                0.0);
                    iAutoClean[8]=ReadIniData(str1,  "Configuration",  "HotplatlPickOffset",             0.0);
                    iAutoClean[9]=ReadIniData(str1,  "Configuration",  "HotplatlPlaceOffset",            0.0);
                    iAutoClean[10]=ReadIniData(str1, "Configuration",  "HotplatlPitchOffset",            0.0);
                    iAutoClean[11]=ReadIniData(str1, "Configuration",  "iAutoClean_MotorSpeed[0]",       10);
                    iAutoClean[12]=ReadIniData(str1, "Configuration",  "iAutoClean_MotorSpeed[1]",       10);
                    iAutoClean[13]=ReadIniData(str1, "Configuration",  "iAutoClean_MotorSpeed[2]",       10);
                    iAutoClean[14]=ReadIniData(str1, "Configuration",  "iAutoClean_MotorSpeed[3]",       10);
                    iAutoClean[15]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle1PickOffset",  -200);         //JerryYang 20191003 add auto clean offset
                    iAutoClean[16]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle1PlaceOffset", 0);
                    iAutoClean[17]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle1XOffset",     0);
                    iAutoClean[18]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle1YOffset",     0);
                    iAutoClean[19]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle2PickOffset",  -200);
                    iAutoClean[20]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle2PlaceOffset", 0);
                    iAutoClean[21]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle2XOffset",     0);
                    iAutoClean[22]=ReadIniData(str1, "Configuration",  "iAutoClean_Shuttle2YOffset",     0);
                    iAutoClean[23]=ReadIniData(str1, "Configuration",  "ShuttlePitchOffset"        ,     0.0);

                    for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)         //KenHsieh 20230518 : Auto Clean count不覆蓋
                    {
                        for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
                        {
                            asCleanCt.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                            strCleanCnt[X][Y]=ReadIniData(str1, "Configuration", asCleanCt, AnsiString(0));
                        }
                    }
                    //----------------------
                    //備份Bottom 2D offset
                    //----------------------
                    str1=FileInfo().PathCombin(OrgFolder, "HandlerCondition.Data");
                    dB2DOffsetX=ReadIniData(str1, "Configuration", "Bottom 2D Offset X",  0.0);
                    dB2DOffsetY=ReadIniData(str1, "Configuration", "Bottom 2D Offset Y",  0.0);

                    if(IniConfig.bN07_7SendRecipeAsBinary==false)               //Steven 20230710 : 工作檔使用二進制上傳下載
                    {
                        S=FileInfo().PathCombin(PPIDFolder, PPID+AnsiString(".ini"));
                        TStringList *TempPPBody;
                        TempPPBody= new TStringList();
                        TempPPBody->Clear();
                        TempPPBody->Text=PPBODY;
                        TempPPBody->SaveToFile(S);

                        AnsiString TempString,TempString3;
                        TStringList *TempString2;
                        TempString2= new TStringList();

                        Str="";
                        TempString=TempPPBody->Text;

                        int ilength =0;
                        AnsiString sTemp;
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                     //kevin 20181101 add fast Get file 10 sec
                        {
                            TempString3=TempString;

                            while(TempString3.Length()!=0)
                            {
                                ilength=TempString3.Length();
                                iEnd   =TempString3.AnsiPos(";");
                                if(iEnd==0)                                     //last data
                                {
                                    sTemp =TempString3;
                                    TempString3="";
                                }
                                else
                                {
                                    sTemp =TempString3.SubString(0, iEnd-1);
                                    TempString3=TempString3.SubString(iEnd+1,ilength);
                                }
                                iStart=sTemp.AnsiPos("[");                      //先找到第一筆
                                iEnd  =sTemp.AnsiPos("]");
                                Str   =sTemp.SubString(iStart+1, iEnd-iStart-1);
                                sTemp.Delete(1, iEnd);                          //取出內容資料砍掉[檔名.Data]
                                TempString2->Text = StringReplace(sTemp, "\\r\\n", "\r\n", TReplaceFlags()<<rfReplaceAll);
                                if(Str.UpperCase()=="POSITION OFFSET.DATA" || Str.UpperCase()=="POSITION OFFSET HOT.DATA")
                                    S.sprintf("D:\\HT9045\\IniData\\Offset\\%s\\%s", PPID, Str);                        //kevin 20181107
                                else
                                    S.sprintf("%s\\%s", PPIDFolder, Str);

                                TempString2->SaveToFile(S);                     //TReplaceFlags
                            }
                        }
                        else
                        {
                            iStart=TempString.AnsiPos("[");                     //先找到第一筆
                            iEnd  =TempString.AnsiPos("]");
                            Str   =TempString.SubString(iStart+1, iEnd-iStart-1);
                            TempString.Delete(1, iEnd);

                            while(TempString.Length()!=0)
                            {
                                iEnd  =TempString.AnsiPos(".Data]");
                                if(iEnd!=0)                                     //找出檔案名稱
                                {
                                    iStart=iEnd-1;
                                    while(TempString.SubString(iStart, 1)!="[")
                                    {
                                        iStart--;
                                        if(iStart==1)
                                            break;
                                    }
                                    TempString2->Text=TempString.SubString(1, iStart-1);
                                }
                                else
                                {
                                    TempString2->Text=TempString.SubString(1, TempString.Length());
                                }

                                S=FileInfo().PathCombin(PPIDFolder, Str);

                                int iSize=0;
                                char *buff;
                                AnsiString asInsterData1;
                                AnsiString asInsterData2;
                                TMemoryStream* pms = new TMemoryStream();
                                TMemoryStream* newpms = new TMemoryStream();
                                pms->Write((void*)TempString2->Text.c_str(), TempString2->Text.Length());
                                iSize=pms->Size;
                                pms->Position=0;
                                buff=new char[iSize+1];
                                pms->Read(buff, iSize);
                                newpms->Position=0;
                                asInsterData1="\r";
                                asInsterData2="\n";
                                for(int i=0; i<iSize; i++)
                                {
                                    if(i+1<iSize && buff[i]=='\\' && buff[i+1]=='r')
                                    {
                                        newpms->Write(asInsterData1.c_str(), asInsterData1.Length());
                                        i++;
                                    }
                                    else if(i+1<iSize && buff[i]=='\\' && buff[i+1]=='n')
                                    {
                                        newpms->Write(asInsterData2.c_str(), asInsterData2.Length());
                                        i++;
                                    }
                                    else
                                    {
                                        newpms->Write(buff+i,1);
                                    }
                                }
                                TempString2->Text=AnsiString((char*)newpms->Memory).SubString(1, newpms->Size);

                                try                                             //AI(Copilot) 20260320 (RogerYang) : 捕捉寫檔例外，僅記錄失敗log，不中斷原流程
                                {
                                    TempString2->SaveToFile(S);
                                }
                                catch(Exception &exception)
                                {
                                    HGemPtr->StringOut("S7F4, SaveToFile fail :"+S);
                                    HGemPtr->StringOut("S7F4, SaveToFile err  :"+exception.Message);
                                }

                                delete pms;
                                delete newpms;
                                delete[] buff;                                  //JerryYang 20240821 : modify
                                buff=NULL;
                                TempString2->Clear();
                                Str=TempString.SubString(iStart+1, iEnd-iStart-1+5);
                                if(iEnd!=0)
                                    TempString.Delete(1, iEnd+5);
                                else
                                    break;
                            }
                        }
                        bRefreshFunction=true;                                  //kevin 20181101
                        LookForFile();
                        TempString2->Clear();                                   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                        delete TempString2;
                        TempString2 =NULL;

                        TempPPBody->Clear();                                    //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                        delete TempPPBody;
                        TempPPBody =NULL;
                    }
                    else
                    {
                        NewFolder=FileInfo().PathCombin(DataPath, PPID);
                        str1=FileInfo().PathCombin(DataPath, PPID+AnsiString(".zip"));
                        DeleteFile(str1);
                        HGemPtr->StringOut("S7F4, DeleteFile :"+str1);
                        if((stream=fopen(str1.c_str(), "wb"))==NULL)            //Steven 20240809 : S --> str1
                        {
                            HGemPtr->StringOut("S7F4, fopen fail :"+str1);
                            HGemPtr->LocalAcknowledge(7, 4, 1);
                            delete[] PPBODY;
                            PPBODY=NULL;                                        //kevin 20180517
                            return;
                        }

                        fwrite((char *)&PPBODY[0], 1, len, stream);
                        fclose(stream);
                        HGemPtr->StringOut("S7F4, fwrite done :"+str1);

                        str1="e \""+NewFolder+".zip\" -o\""+NewFolder+"\\\" -y";
                        hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);
                        if(int(hInstance)<=32)
                        {
                            ;
                        }
                        MySleep(500);                                           //Steven 20140609 : 1000 --> 100
                        str1=FileInfo().PathCombin(DataPath, PPID+AnsiString(".zip"));
                        HGemPtr->StringOut("S7F4, DeleteFile :"+str1);
                        DeleteFile(str1.c_str());
                    }
                    bRefreshFunction=true;                                      //kevin 20181101
                    LookForFile();
//                    delete[] PPBODY;                                          //Steven 20160912 : Add delete for save memory
//                    PPBODY=NULL;                                              //kevin 20180517

                    if(IniConfig.bN07_6EnableUploadOSRecipe)
                    {
                        if(IniConfig.bN07_6CompressedFile)                      //JimmyChiu 20250214 : For Hana ART
                        {
                            NewFolder=FileInfo().PathCombin(DataPath, PPID);
                            str1=FileInfo().PathCombin(NewFolder, "OS_Setting.zip");                                    //Steven 20230710 : OS測試機的工作檔也要上傳
                            str2=FileInfo().PathCombin(IniConfig.sN07_6OSRecipePath, PPID+"ini");
                            if(FileExists(str1))
                            {
                                str3="e \""+str1+"\" -o\""+str2+"\\\" -y";
                                hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str3.c_str(), NULL, SW_HIDE);
                                if(int(hInstance)<=32)
                                {
                                    ;
                                }
                                MySleep(500);
                            }
                        }
                        else
                        {
                            OrgFolder=FileInfo().PathCombin(DataPath, PPID);
                            NewFolder=FileInfo().PathCombin(IniConfig.sN07_6OSRecipePath, PPID);
                            OrgFile=FileInfo().PathCombin(OrgFolder, "OS_Setting.Data");
                            NewFile=FileInfo().PathCombin(NewFolder, "OS_Setting.ini");                                 //Steven 20230710 : OS測試機的工作檔也要上傳
                            ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        }
                    }

                    NewFolder=FileInfo().PathCombin(DataPath, PPID);
                    OrgFolder=DataPath;

                    if(IniConfig.bA30SetupTeachFunction)                        //JerryYang 20180921 Setup Teach功能
                    {
                        int iTempPinNum=0;
                        double dXDeimen=0.0, dYDeimen=0.0;
                        str1=FileInfo().PathCombin(NewFolder, "Contact.Data");

                        iTempPinNum=ReadIniData(str1, "Torque Control", "Pin Number",    0);
                        dXDeimen=ReadIniData(str1, "Torque Control", "X Dimension",   0.0);
                        dYDeimen=ReadIniData(str1, "Torque Control", "Y Dimension",   0.0);

                        if(DeviceForm_File.iPinCT!=iTempPinNum || DeviceForm_File.XDimension!=dXDeimen || DeviceForm_File.YDimension!=dYDeimen)
                        {
                            LastSet.bNeedSetupTeach=true;
                            WriteLastDataFile();
                            for(int i=0; i<5; i++)
                            {
                                iInArmPickPlaceCnt[i]=0;
                                bInArmStop[i]=false;
                            }

                            for(int i=0; i<8; i++)                              //Out arm部分，預設全部檢查
                            {
//                                iOutArmPickPlaceCnt[i]=0;                     //JerryYang 20230523 : 沒在用, Mark掉
                                bOutArmStop[i]=false;
                            }
                        }
                    }

                    if(IniConfig.bSPILFunction)                                 //JerryYang 20230322 : //矽品中山廠封裝是by機台上傳工作檔,要download
                    {
                        str1=FileInfo().PathCombin(NewFolder, "HandlerCondition.Data");
                        b2Dsort=ReadIniData(str1, "Configuration", "Sorting By 2DID List", false);
                    }
                    //JerryYang 20170126 (Steven) 不覆蓋的要進行還原
                    //==>
                    //----------------------
                    //不覆蓋就是必須要還原
                    // 0 : 不覆蓋
                    // 1 : 要覆蓋
                    //----------------------

                    //----------------------
                    //把溫度Offset資料還原
                    //----------------------
                    if(CosFunction.bDownloadRecipeLevelMode)                    //jou 2016-01-06 download recipe 增加權限模式選擇
                    {
                        if(fLotInfo->coLevelMode->Text!="Normal")
                            bNeedCover=true;
                        else
                            bNeedCover=false;
                    }
                    else
                    {
                        bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Temp Offset", true);
                    }

                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 20180831 add
                    {
                        bNeedCover=true;
                    }
                    else if(IniConfig.bSPILFunction || CUSTOMER_CODE==CC_ASE_CL)                                        //JerryYang 20250120 : modify                            //JerryYang 20220923 : add SPIL
                    {
                        bNeedCover=false;
                    }

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "Temperature.Data");
                        for(int i=0; i<tcTotalCount; i++)                       //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset
                        {
                            str2.printf("CH%d", i+1);
                            WriteIniData(str1, "User OffSet", str2, fTempUserOffset[i]);
                        }
                    }

                    if(ATC_SYSTEM>eATC60)                                       //JerryYang 20220324 : 複製ATC溫度offset
                    {
                        if(ATC_SYSTEM!=eNonChamber)
                        {
                            if(CosFunction.bUseOldATCTempOffset==true)          //JerryYang 20190123 ATC Offset UI使用舊版
                            {
                            }
                            else
                            {
                                for(int i=0; i<32; i++)
                                {
                                    str2.sprintf("ATCTempOffset[%d]", i);
                                    WriteIniData(str1, "ATC", str2, dATCOffsetBackup[i]);
                                }
                            }
                        }
                    }

                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);                    //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
                    if(bNeedCover==false)
                    {
                        str1=FileInfo().PathCombin(NewFolder, "Temperature.Data");
                        WriteIniData(str1, "Index",   "Heating Mode",   iIndexHeatingMode);
                    }

                    //----------------------
                    //把Contact高度還原
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact High", false);                      //預設不覆蓋
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 20180831 add
                    {
                        bNeedCover=true;
                    }
                    else if(IniConfig.bSPILFunction)                            //JerryYang 20220923 : add SPIL
                    {
                        bNeedCover=false;
                        if(b2Dsort==true)                                       //JerryYang 20230322 : //矽品中山廠封裝是by機台上傳工作檔,要download
                        {
                            bNeedCover=true;
                        }
                    }

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "Contact.Data");
                        WriteIniData(str1, "Test Arm1", "Pick Up", fContactHeight[0]);
                        WriteIniData(str1, "Test Arm1", "Contact", fContactHeight[1]);
                        if(IniConfig.bSPILFunction==false)                      //JerryYang 20200812 : SPIL緯平要求將drop offset移除,不要當機差參數
                        {
                            WriteIniData(str1, "Test Arm1", "Drop",    fContactHeight[2]);
                            WriteIniData(str1, "Test Arm2", "Drop",    fContactHeight[6]);
                        }
                        WriteIniData(str1, "Test Arm1", "Place",   fContactHeight[3]);
                        WriteIniData(str1, "Test Arm2", "Pick Up", fContactHeight[4]);
                        WriteIniData(str1, "Test Arm2", "Contact", fContactHeight[5]);
                        WriteIniData(str1, "Test Arm2", "Place",   fContactHeight[7]);
                    }

                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Force", true);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                    {
                        bNeedCover=true;
                    }

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "Contact.Data");
                        WriteIniData(str1, "Torque Control", "Pin Number",    fContactHeight[8]);
                        WriteIniData(str1, "Torque Control", "Force Per Pin", fContactHeight[9]);
                        WriteIniData(str1, "Torque Control", "Torque"       , fContactHeight[19]);
                    }

                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Mode", true);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                        bNeedCover=true;

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "Contact.Data");
                        WriteIniData(str1, "Wait Time", "Drop Wait",                    fContactHeight[10]);
                        WriteIniData(str1, "Wait Time", "Drop Speed",                   fContactHeight[11]);
                        WriteIniData(str1, "Mode", "Contact",                           fContactHeight[12]);
                        WriteIniData(str1, "Mode", "Vacuum",                            fContactHeight[13]);
                        WriteIniData(str1, "Mode", "Dummy Contact",                     fContactHeight[14]);
                        WriteIniData(str1, "Mode", "Head Device Mode",                  fContactHeight[15]);
                        WriteIniData(str1, "Mode", "Kit Diameter",                      fContactHeight[16]);
                        WriteIniData(str1, "Mode", "Suck Shuttle Device After Tested",  fContactHeight[17]);
                        WriteIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber",  fContactHeight[18]);
                    }

                    //----------------------
                    //把HotPlate資料還原
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "HotPlate", false);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                        bNeedCover=true;

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        OrgFile=FileInfo().PathCombin(OrgFolder, "HotPlate.Data");
                        NewFile=FileInfo().PathCombin(NewFolder, "HotPlate.Data");
                        ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        MySleep(100);
                    }

                    OrgFile=FileInfo().PathCombin(OrgFile, "HotPlate.Data");
                    DeleteFile(OrgFile.c_str());
                    MySleep(100);

                    //----------------------
                    //把Ld/Uld速度還原
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Load Unload", false);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                    {
                        bNeedCover=true;
                    }

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        OrgFile=FileInfo().PathCombin(OrgFolder, "UdUld.Data");
                        NewFile=FileInfo().PathCombin(NewFolder, "UdUld.Data");
                        ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        MySleep(100);
                    }
                    OrgFile=FileInfo().PathCombin(OrgFolder, "UdUld.Data");
                    DeleteFile(OrgFile.c_str());
                    MySleep(100);

                    //----------------------
                    //把速度設定還原
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Speed Setting", true);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                        bNeedCover=true;

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        OrgFile=FileInfo().PathCombin(OrgFolder, "ArmCondition.Data");
                        NewFile=FileInfo().PathCombin(NewFolder, "ArmCondition.Data");
                        ret=CopyFile(OrgFile.c_str(), NewFile.c_str(), false);
                        MySleep(100);
                    }

                    OrgFile=FileInfo().PathCombin(OrgFolder, "ArmCondition.Data");
                    DeleteFile(OrgFile.c_str());
                    MySleep(100);

                    //----------------------
                    //還原開啟單一Shuttle選項
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode", false);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20180831 add
                       IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                        bNeedCover=true;

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "HandlerCondition.Data");
                        WriteIniData(str1, "Configuration", "Shuttle Mode", iShuttleMode[0]);
                        WriteIniData(str1, "Configuration", "Shuttle1 Cancel", iShuttleMode[1]);
                    }
                    //----------------------
                    //還原Auto Clean選項
                    //----------------------                                    //Steven 20161116 : ATC說要加上Auto Clean
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean", true);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 20180831 add
                        bNeedCover=true;
                    else if(CUSTOMER_CODE==CC_SCC || IniConfig.bSPILFunction)   //Steven 20190712 : 李國旗說要寫死不覆蓋  //JerryYang 20220923 : add SPIL
                        bNeedCover=false;

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "HandlerCondition.Data");
                        if(IniConfig.bSPILFunction==true)                       //JerryYang 20191003 矽品只還原auto clean offset
                        {
                            WriteIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",  iAutoClean[2]);

                            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean Contact Height", false);                                         //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋
                            if(bNeedCover==false)
                            {
                                WriteIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",  iAutoClean[3]);
                            }
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",     iAutoClean[4]);
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",  iAutoClean[5]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",  iAutoClean[15]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset", iAutoClean[16]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",     iAutoClean[17]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",     iAutoClean[18]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",  iAutoClean[19]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset", iAutoClean[20]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",     iAutoClean[21]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",     iAutoClean[22]);
                            WriteIniData(str1, "Configuration", "ShuttlePitchOffset",             iAutoClean[23]);
                        }
                        else if(CUSTOMER_CODE==CC_SCC)                          //JerryYang 20190919 : 曹沖說除了Height，其他的都需要正常download覆蓋本機參數
                        {
                            WriteIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",  iAutoClean[2]);
                            WriteIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",  iAutoClean[3]);
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",     iAutoClean[4]);       //wei 20150318 Auto clean Index Pick Offset
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",  iAutoClean[5]);       //Jou 2015-08-22 Auto clean Index Release Offset
                            WriteIniData(str1, "Configuration", "HotplatlXOffset",                iAutoClean[6]);       //JerryYang 20161122 補上Auto Clean參數
                            WriteIniData(str1, "Configuration", "HotplatlYOffset",                iAutoClean[7]);
                            WriteIniData(str1, "Configuration", "HotplatlPickOffset",             iAutoClean[8]);
                            WriteIniData(str1, "Configuration", "HotplatlPlaceOffset",            iAutoClean[9]);
                            WriteIniData(str1, "Configuration", "HotplatlPitchOffset",            iAutoClean[10]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",  iAutoClean[15]);      //JerryYang 20170411 (Steven) 補上auto clean offset
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset", iAutoClean[16]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",     iAutoClean[17]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",     iAutoClean[18]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",  iAutoClean[19]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset", iAutoClean[20]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",     iAutoClean[21]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",     iAutoClean[22]);
                            WriteIniData(str1, "Configuration", "ShuttlePitchOffset",             iAutoClean[23]);
                        }
                        else
                        {
                            WriteIniData(str1, "Configuration", "iAutoClean_Function",            iAutoClean[0]);
                            WriteIniData(str1, "Configuration", "iAutoClean_SelectArm",           iAutoClean[1]);
                            WriteIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",  iAutoClean[2]);
                            WriteIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",  iAutoClean[3]);
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",     iAutoClean[4]);       //wei 20150318 Auto clean Index Pick Offset
                            WriteIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",  iAutoClean[5]);       //Jou 2015-08-22 Auto clean Index Release Offset
                            WriteIniData(str1, "Configuration", "HotplatlXOffset",                iAutoClean[6]);       //JerryYang 20161122 補上Auto Clean參數
                            WriteIniData(str1, "Configuration", "HotplatlYOffset",                iAutoClean[7]);
                            WriteIniData(str1, "Configuration", "HotplatlPickOffset",             iAutoClean[8]);
                            WriteIniData(str1, "Configuration", "HotplatlPlaceOffset",            iAutoClean[9]);
                            WriteIniData(str1, "Configuration", "HotplatlPitchOffset",            iAutoClean[10]);
                            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[0]",       iAutoClean[11]);
                            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[1]",       iAutoClean[12]);
                            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[2]",       iAutoClean[13]);
                            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[3]",       iAutoClean[14]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",  iAutoClean[15]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset", iAutoClean[16]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",     iAutoClean[17]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",     iAutoClean[18]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",  iAutoClean[19]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset", iAutoClean[20]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",     iAutoClean[21]);
                            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",     iAutoClean[22]);
                            WriteIniData(str1, "Configuration", "ShuttlePitchOffset",             iAutoClean[23]);
                        }
                    }

                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Cleaning Count", bNeedCover);               //KenHsieh 20230518 : Auto Clean count不覆蓋
                    if(bNeedCover==false)                                       //KenHsieh 20230518 : Auto Clean count不覆蓋
                    {
                        str1=FileInfo().PathCombin(NewFolder, "HandlerCondition.Data");
                        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
                        {
                            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
                            {
                                asCleanCt.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                                WriteIniData(str1, "Configuration", asCleanCt, strCleanCnt[X][Y]);
                            }
                        }
                    }

                    //----------------------
                    //還原Bottom 2D offset
                    //----------------------
                    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Bottom 2D offset", true);
                    if(IniConfig.bSPILFunction)                                 //JerryYang 20220923 : add SPIL
                    {
                        bNeedCover=false;
                        if(b2Dsort==true)                                       //JerryYang 20230322 : //矽品中山廠封裝是by機台上傳工作檔,要download
                        {
                            bNeedCover=true;
                        }
                    }

                    if(bNeedCover==false)                                       //不覆蓋就要還原
                    {
                        str1=FileInfo().PathCombin(NewFolder, "HandlerCondition.Data");
                        WriteIniData(str1, "Configuration", "Bottom 2D Offset X",     dB2DOffsetX);
                        WriteIniData(str1, "Configuration", "Bottom 2D Offset Y",     dB2DOffsetY);
                    }
                    //----------------------
                    //檢查Offset檔
                    //----------------------
                    str1.sprintf("%s%s\\Position Offset.Data", OffsetPath, PPID);
                    if(!FileExists(str1))                                       //如果沒有就從當下的工作檔複製過來  //JerryYang 20210119 : fix機台點位offset問題
                    {
                        str2.sprintf("%s%s\\Position Offset.Data", OffsetPath, GetLastOpenFN());
                        sDir.sprintf("%s%s\\", OffsetPath, PPID);
                        MyForceDirectories(sDir);
                        CopyFile(str2.c_str(), str1.c_str(), false);
                    }

                    str1.sprintf("%s%s\\Position Offset Hot.Data", OffsetPath, PPID);
                    if(!FileExists(str1))                                       //如果沒有就從當下的工作檔複製過來
                    {
                        str2.sprintf("%s%s\\Position Offset Hot.Data", OffsetPath, GetLastOpenFN());
                        sDir.sprintf("%s%s\\", OffsetPath, PPID);
                        MyForceDirectories(sDir);
                        CopyFile(str2.c_str(), str1.c_str(), false);
                    }
                    MySleep(100);
                }
                delete[] PPBODY;                                                //Steven 20160912 : Add delete for save memory
                PPBODY=NULL;                                                    //kevin 20180517
                if(strSetupfile==PPID)                                          //Steven 20220818 : Fixed for bD31RTCChangeRecipeNeedreCreateModel
                    fSetup->bFirstTime=true;
                else
                    fSetup->bFirstTime=false;

                if(CosFunction.bKeepOnly1SetupFile)
                {
                    try
                    {
                        fLotInfo->ClearAllSetupFile(PPID);                      //Steven 20210917 : Add for 下載完工作檔後, 只留一個就好
                    }
                    catch(...)
                    {
                    };
//                    fMain->PPID=PPID;
//                    fMain->bNeedClearFile=true;
                }
                LastSet.bHasDownloadFile=true;                                  //Steven 20200415 : 下載工作檔的Flag
//                if(CUSTOMER_CODE==CC_XINYUN)                                    //AI(ht9045-secs-sem) 20260616 (RogerYang) : XINYUN PPID下載成功後彈窗請OP確認配方(經SecsAlarmMessage非同步, 避免block SECS thread斷線)
//                    SecsAlarmMessage->Add("Download Recipe Success, Please Confirm Recipe!");        //暫時不卡

                HGemPtr->LocalAcknowledge(7, 4, 0);                             //Steven 20211013 : 換位置
                return;
            }
        }
    }
    S9F7_IllegalData("S7,F3 Data Format error !!!");
}
// ---------------------------------------------------------------------------
// V 1.0
// S7,F6 This message is used to transfer a process program.
// ---------------------------------------------------------------------------
void HT9045Gem::S7F6_ProcessProgramData()                                       //Steven 20140911 : Modify S7F6 for HT9045
{
    unsigned char Type;
    AnsiString S, Str, str1, str2, str3;
    char PPID[1024];
    int ret, len;
    FILE *stream;
    HINSTANCE hInstance;
    int handle, length;
    char *PPBODY;

    TStringList *TempString2;                                                   //JerryYang 20241118 : 優化S7F5時間
    TempString2= new TStringList();
    TempString2->Clear();

    ret=HGemPtr->GetDataItemLenAndType(len, Type);
    if(ret==1 && Type==HType.ASCII_TYPE)
    {
        HGemPtr->DataItemIn(1024, Type, PPID);                                  // PPID=filename;
        S.sprintf("D:\\HT9045\\IniData\\Data\\%s", PPID);

        if(DirectoryExists(S)==false)
        {
            S9F7_IllegalData("S7,F5 PPID Not Exists!!!");
            return;
        }

        if(IniConfig.bN07_6EnableUploadOSRecipe)
        {
            if(IniConfig.bN07_6CompressedFile)                                  //JimmyChiu 20250214 : For Hana ART
            {
                str1.sprintf("%s%s\\OS_Setting.zip", DataPath, PPID);           //Steven 20230710 : OS測試機的工作檔也要上傳
                str2.sprintf("%s%s.ini", IniConfig.sN07_6OSRecipePath, PPID);
                if(FileExists(str2))
                {
                    Str.sprintf("S7F6, step 2 : Copy OS file from %s to %s   ", str2, str1);
                    HGemPtr->StringOut(Str);
                    str3.sprintf("a -tzip \"%s\" \"%s\"", str1, str2);
                    hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str3.c_str(), NULL, SW_HIDE);
                    HGemPtr->StringOut("S7F6, Zip File");
                    MySleep(IniConfig.dN07_7_DelayTime*1000);                   //Steven 20240105 : add delay
                    HGemPtr->StringOut("S7F6, step 3 : Zip File Done");
                }
                else
                {
                    Str.sprintf("S7F6, step 1 : OS File %s does not exists!", str2);
                    HGemPtr->StringOut(Str);
                }
            }
            else
            {
                str1.sprintf("%s%s\\OS_Setting.data", DataPath, PPID);          //Steven 20230710 : OS測試機的工作檔也要上傳
                str2.sprintf("%s%s.ini", IniConfig.sN07_6OSRecipePath, PPID);
                if(FileExists(str2))
                {
                    Str.sprintf("S7F6, step 2 : Copy OS file from %s to %s   ", str2, str1);
                    HGemPtr->StringOut(Str);
                    CopyFile(str2.c_str(), str1.c_str(), false);
                    MySleep(IniConfig.dN07_7_DelayTime*1000);                   //Steven 20240105 : add delay
                    LogSoftwareOnTime("S7F6, step 3 : Copy File Done");
                }
                else
                {
                    Str.sprintf("S7F6, step 1 : OS File %s does not exists!", str2);
                    HGemPtr->StringOut(Str);
                }
            }

            if(FileExists(str1))
            {
                HGemPtr->StringOut("S7F6, copy OS file ok");
            }
            else
            {
                HGemPtr->StringOut("S7F6, copy OS file fail");
            }
            HGemPtr->SaveSECSGEMTextToLog();
        }

        HGemPtr->FileListBox3->Directory=S;
        HGemPtr->memoPPBody->Clear();
        TStringList *TempString;
        TempString= new TStringList();
        AnsiString sBuffer="", sBuffer1="";                                     //kevin 20180823 add ASE KH

        if(IniConfig.bN07_7SendRecipeAsBinary==false)                           //Steven 20230710 : 工作檔使用二進制上傳下載
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_ASE_CL)     //JerryYang 20250120 : modify                                 //kevin 20180830 add
            {
                for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)
                {
                    TempString->Clear();
                    TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                    Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                    sBuffer.sprintf("%s%s;", Str,TempString->Text);
                    sBuffer1+=sBuffer;
                }
                S.sprintf("D:\\HT9045\\IniData\\Offset\\%s", PPID);
                if(DirectoryExists(S)==false)
                {
                    S9F7_IllegalData("S7,F5 PPID OFFSET Not Exists!!!");
                    return;
                }

                HGemPtr->FileListBox3->Directory=S;
                for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)        //kevin 20180830 add copy offset data
                {
                    TempString->Clear();
                    TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                    Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                    if(i==HGemPtr->FileListBox3->Items->Count-1)
                        sBuffer.sprintf("%s%s", Str,TempString->Text);
                    else
                        sBuffer.sprintf("%s%s;", Str,TempString->Text);
                    sBuffer1+=sBuffer;
                }

                HGemPtr->memoPPBody->Lines->Add(sBuffer1);
            }
            else
            {
                for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)
                {
                    TempString->Clear();
                    TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                    Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                    TempString2->Add(Str);                                      //JerryYang 20241118 : 優化S7F5時間
                    TempString2->Add(TempString->Text);
                    TempString2->Add("");
                }
            }

            if(TempString2->Count<1000)                                         //JerryYang 20241118 : 優化S7F5時間
            {
                for(int i=0; i<TempString2->Count; i++)
                {
                    Str=TempString2->Strings[i];
                    Str=Str.Trim();
                    if(Str=="")
                    {
                        TempString2->Delete(i);
                        i--;
                        if(i<0)
                            i=0;
                    }
                }
            }
            str1.sprintf("S7F6, Trim() %d", HGemPtr->memoPPBody->Lines->Count);
            HGemPtr->StringOut(str1);

            char *buff;                                                         //JerryYang 20241118 : 優化S7F5時間
            int iSize=0;
            AnsiString asInsterData1;
            AnsiString asInsterData2;
            Str=TempString2->Text;
            TMemoryStream* pms = new TMemoryStream();
            TMemoryStream* newpms = new TMemoryStream();
            pms->Write((void*)Str.c_str(), Str.Length());
            iSize=pms->Size;
            pms->Position=0;
            buff=new char[iSize+1];
            pms->Read(buff,iSize);
            newpms->Position=0;
            asInsterData1="\\r";
            asInsterData2="\\n";
            for(int i=0; i<iSize; i++)
            {
                if(buff[i]=='\r' )
                {
                    newpms->Write(asInsterData1.c_str(), asInsterData1.Length());
                }
                else if(buff[i]=='\n' )
                {
                    newpms->Write(asInsterData2.c_str(), asInsterData2.Length());
                }
                else
                {
                    newpms->Write(buff+i,1);
                }
            }
            Str=AnsiString((char*)newpms->Memory).SubString(1, newpms->Size);
            delete pms;
            delete newpms;
            delete[] buff;                                                      //JerryYang 20240821 : modify
            buff=NULL;

            HGemPtr->InitLocalHead(7, 6, 0);
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
            HGemPtr->DataItemOut(HType.ASCII_TYPE, Str);
            HGemPtr->SendLocalData();
            HGemPtr->StringOut("S7F6, DataItemOut");
        }
        else
        {
            //先刪除舊的壓縮檔
            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%s%s.zip", DataPath, PPID);
            DeleteFile(str1.c_str());
            MySleep(IniConfig.dN07_7_DelayTime*1000);                           //Steven 20240105 : add delay

            //把7Zip準備好
            if(FileExists("d:\\HT9045\\7z.exe")==false)
            {
                CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , false);                             //Ifor 20170321 FALSE => false
            }

            //把工作檔壓縮
            //jou 2012-12-14 system改採用ShellExecute
            str1.sprintf("a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", DataPath, PPID, DataPath, PPID);
            hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);
            MySleep(IniConfig.dN07_7_DelayTime*1000);                           //Steven 20240105 : add delay
            HGemPtr->StringOut("S7F6, Zip File");
            if(int(hInstance)<=32)
            {
                //可能是壓縮失敗
                HGemPtr->StringOut("Zip recipe fail!");
                HGemPtr->InitLocalHead(7, 6, 0);
                HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
                HGemPtr->StringOut("S7F6, Zip File fail");
            }

            S.sprintf("%s%s.zip", DataPath, PPID);

            MySleep(IniConfig.dN07_7_DelayTime*1000);                           //Steven 20240105 : add delay
            if(FileExists(S))
            {
                HGemPtr->StringOut(AnsiString("S7F6, Has Zip File:")+S);
            }
            else
            {
                HGemPtr->StringOut(AnsiString("S7F6, Can not found Zip File")+S);
            }

            handle=open(S.c_str(), O_RDONLY);
            if(handle==-1)
            {
                HGemPtr->InitLocalHead(7, 6, 0);
                HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
                HGemPtr->SendLocalData();
                HGemPtr->StringOut("S7F6, open File fail");
            }
            else
            {
                length=filelength(handle);
                HGemPtr->StringOut(AnsiString("S7F6, File Length:")+AnsiString(length));
                PPBODY=new char [length];
                close(handle);
                if((stream=fopen(S.c_str(), "rb"))==NULL)
                {
                    delete[] PPBODY;
                }
                else
                {
                    fread(PPBODY, 1, length, stream);
                    fclose(stream);
                    HGemPtr->InitLocalHead(7, 6, 0);
                        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
                            HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
                        HGemPtr->DataItemOut(length, HType.BINARY_TYPE, PPBODY);
                    HGemPtr->SendLocalData();
                    delete[] PPBODY;
                    HGemPtr->StringOut("S7F6, DataItemOut");
                }
            }
            str1.sprintf("%s%s.zip", DataPath, PPID);                           //最後要把壓縮檔給刪除。
            DeleteFile(str1.c_str());
        }
        LookForFile();
        TempString->Clear();                                                    //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete TempString;
        TempString2->Clear();                                                   //JerryYang 20241118 : 優化S7F5時間
        delete TempString2;

        return;
    }
    S9F7_IllegalData("S7,F5 Data Format error !!!");
}
//------------------------------------------------------------------------------
void HT9045Gem::S7F6_ProcessProgramData(AnsiString FileName)                    //Steven 20210510 : S7F6上傳工作檔
{
//    unsigned char Type=HType.ASCII_TYPE;
    AnsiString S, Str, str1, str2, str3;
    char PPID[1024];
    FILE *stream;
    HINSTANCE hInstance;
    int handle, length;
    char *PPBODY;
//    int ret, len;

    sprintf(PPID, "%s", FileName.c_str());

//    HGemPtr->DataItemIn(1024, Type, PPID);  // PPID=filename;
    S.sprintf("D:\\HT9045\\IniData\\Data\\%s", PPID);

    if(DirectoryExists(S)==false)
    {
        S9F7_IllegalData("S7,F5 PPID Not Exists!!!");
        return;
    }

    HGemPtr->FileListBox3->Directory=S;
    HGemPtr->memoPPBody->Clear();
    TStringList *TempString;
    TempString= new TStringList();
    AnsiString sBuffer="", sBuffer1="";                                         //kevin 20180823 add ASE KH

    if(IniConfig.bN07_6EnableUploadOSRecipe)
    {
        if(IniConfig.bN07_6CompressedFile)                                      //JimmyChiu 20250214 : For Hana ART
        {
            str1.sprintf("%s%s\\OS_Setting.zip", DataPath, PPID);               //Steven 20230710 : OS測試機的工作檔也要上傳
            str2.sprintf("%s%s.ini", IniConfig.sN07_6OSRecipePath, PPID);
            if(FileExists(str2))
            {
                Str.sprintf("S7F6, step 2 : Copy OS file from %s to %s   ", str2, str1);
                HGemPtr->StringOut(Str);
                str3.sprintf("a -tzip \"%s\" \"%s\"", str1, str2);
                hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str3.c_str(), NULL, SW_HIDE);
                HGemPtr->StringOut("S7F6, Zip File");
                MySleep(IniConfig.dN07_7_DelayTime*1000);                       //Steven 20240105 : add delay
                HGemPtr->StringOut("S7F6, step 3 : Zip File Done");
            }
            else
            {
                Str.sprintf("S7F6, step 1 : OS File %s does not exists!", str2);
                HGemPtr->StringOut(Str);
            }
        }
        else
        {
            str1.sprintf("%s%s\\OS_Setting.data", DataPath, PPID);              //Steven 20230710 : OS測試機的工作檔也要上傳
            str2.sprintf("%s%s.ini", IniConfig.sN07_6OSRecipePath, PPID);
            if(FileExists(str2))
            {
                Str.sprintf("S7F6, step 2 : Copy OS file from %s to %s   ", str2, str1);
                HGemPtr->StringOut(Str);
                CopyFile(str2.c_str(), str1.c_str(), false);
                MySleep(IniConfig.dN07_7_DelayTime*1000);                       //Steven 20240105 : add delay
                LogSoftwareOnTime("S7F6, step 3 : Copy OS File Done");
            }
            else
            {
                Str.sprintf("S7F6, step 1 : OS File %s does not exists!", str2);
                HGemPtr->StringOut(Str);
            }
        }

        if(FileExists(str1))
        {
            HGemPtr->StringOut("S7F6, copy OS file ok");
        }
        else
        {
            HGemPtr->StringOut("S7F6, copy OS file fail");
        }
        HGemPtr->SaveSECSGEMTextToLog();
    }

    if(IniConfig.bN07_7SendRecipeAsBinary==false)                               //Steven 20230710 : 工作檔使用二進制上傳下載
    {
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20180830 add
        {
            for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)
            {
                TempString->Clear();
                TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                sBuffer.sprintf("%s%s;", Str,TempString->Text);
                sBuffer1+=sBuffer;
            }
            S.sprintf("D:\\HT9045\\IniData\\Offset\\%s", PPID);
            if(DirectoryExists(S)==false)
            {
                S9F7_IllegalData("S7,F6 PPID OFFSET Not Exists!!!");
                return;
            }

            HGemPtr->FileListBox3->Directory=S;
            for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)            //kevin 20180830 add copy offset data
            {
                TempString->Clear();
                TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                if(i==HGemPtr->FileListBox3->Items->Count-1)
                    sBuffer.sprintf("%s%s", Str,TempString->Text);
                else
                    sBuffer.sprintf("%s%s;", Str,TempString->Text);
                sBuffer1+=sBuffer;
            }

             HGemPtr->memoPPBody->Lines->Add(sBuffer1);
        }
        else
        {
            for(int i=0; i<HGemPtr->FileListBox3->Items->Count; i++)
            {
                TempString->Clear();
                TempString->LoadFromFile(S+"\\"+HGemPtr->FileListBox3->Items->Strings[i]);
                Str.sprintf("[%s]", HGemPtr->FileListBox3->Items->Strings[i]);
                HGemPtr->memoPPBody->Lines->Add(Str);
                HGemPtr->memoPPBody->Lines->Add(TempString->Text);
                //HGemPtr->memoPPBody->Lines->Add("");
                HGemPtr->memoPPBody->Lines->Add("");
            }
        }

        if(HGemPtr->memoPPBody->Lines->Count<1000)
        {
            for(int i=0; i<HGemPtr->memoPPBody->Lines->Count; i++)
            {
                Str=HGemPtr->memoPPBody->Lines->Strings[i];
                Str=Str.Trim();
                if(Str=="")
                {
                    HGemPtr->memoPPBody->Lines->Delete(i);
                    i--;
                    if(i<0)
                        i=0;
                }
            }
        }

        Str=HGemPtr->memoPPBody->Lines->Text;
        Str=StringReplace(Str, "\r\n", "\\r\\n", TReplaceFlags()<<rfReplaceAll);
        HGemPtr->InitLocalHead(7, 6, 0);
        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
        HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
        HGemPtr->DataItemOut(Str.Length(), HType.ASCII_TYPE, Str.c_str());
        HGemPtr->SendLocalData();
    }
    else
    {
        //先刪除舊的壓縮檔
        //jou 2012-12-14 system改採用DeleteFile
        str1.sprintf("%s%s.zip", DataPath, PPID);
        DeleteFile(str1.c_str());
        MySleep(50);

        //把7Zip準備好
        if(FileExists("d:\\HT9045\\7z.exe")==false)
        {
            CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , false);                                 //Ifor 20170321 FALSE => false
        }

        //把工作檔壓縮
        //jou 2012-12-14 system改採用ShellExecute
        str1.sprintf("a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", DataPath, PPID, DataPath, PPID);
        hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);
        MySleep(500);                                                           //jou 2016-01-06 100->500 Unload 上傳失敗修正
        HGemPtr->StringOut("S7F6, Zip File");
        if(int(hInstance)<=32)
        {
            //可能是壓縮失敗
            HGemPtr->StringOut("Zip recipe fail!");
            HGemPtr->InitLocalHead(7, 6, 0);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
            HGemPtr->StringOut("S7F6, Zip File fail");
        }

        S.sprintf("%s%s.zip", DataPath, PPID);
        handle=open(S.c_str(), O_RDONLY);
        if(handle==-1)
        {
            HGemPtr->InitLocalHead(7, 6, 0);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
            HGemPtr->SendLocalData();
            HGemPtr->StringOut("S7F6, open File fail");
        }
        else
        {
            length=filelength(handle);
            PPBODY=new char [length];
            close(handle);
            if((stream=fopen(S.c_str(), "rb"))==NULL)
            {
                delete[] PPBODY;
            }
            else
            {
                fread(PPBODY, 1, length, stream);
                fclose(stream);
                HGemPtr->InitLocalHead(7, 6, 0);
                    HGemPtr->DataItemOut(2, HType.LIST_TYPE,NULL);
                        HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
                    HGemPtr->DataItemOut(length, HType.BINARY_TYPE, PPBODY);
                HGemPtr->SendLocalData();
                delete[] PPBODY;
                HGemPtr->StringOut("S7F6, DataItemOut");
            }
        }
        str1.sprintf("%s%s.zip", DataPath, PPID);                               //最後要把壓縮檔給刪除。
        DeleteFile(str1.c_str());
    }
    LookForFile();
    TempString->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete TempString;
    return;
}
//------------------------------------------------------------------------------
int HT9045Gem::ProcessS7F23FromatReceipe()                                      //寫入到檔案
{
//    double dData;
//    int    iData;
    int ret, SVlen;                                                             //,i, j,k,ret1,ret2,len;

//    char PPID[256];
//    char GemSoft[256];
//    char GemMDSN[256];
//    char CCODE[256];
//    AnsiString S,S1;
//    bool bSearchOK=false;

    ret=HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE);
    if(ret!=1)
        return 2;
    if(SVlen!=4)
        return 2;                                                               // format error
/*
    HGemPtr->DataItemIn(256, HType.ASCII_TYPE,PPID);                            //
    HGemPtr->DataItemIn(256, HType.ASCII_TYPE,GemMDSN);                         //
    HGemPtr->DataItemIn(256, HType.ASCII_TYPE,GemSoft);                         //
    S=PPID;
    S=S.UpperCase();
    for(i=0; i<fMain->cbSetupFileName->Items->Count; i++)
    {
        S1=fMain->cbSetupFileName->Items->Strings[i];
        S1=S1.UpperCase();
        if(S==S1)
        {
            bSearchOK=true;
            break;
        }
    }

    if(bSearchOK==false)
        return 4;                                                               // PPID not found

    ret=HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen, HType.LIST_TYPE);
    if(ret!=1)
        return 2;

    S1=CurrentDir+AnsiString("\\data\\")+S+AnsiString(".ini");
    TIniFile *IniFile = new TIniFile(S1);

    for(i=0; i<SVlen; i++)
    {
        ret=HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL);
        if(ret!=1)
        {
            delete IniFile;
            return 2;
        }

        ret=HGemPtr->DataItemIn(256, HType.ASCII_TYPE, CCODE);                  //
        if(ret!=1)
        {
            delete IniFile;
            return 2;
        }

        if(     strcmp(CCODE, "Molding")==0)
        {
            ret=HGemPtr->DataItemIn(13, HType.LIST_TYPE, NULL);
            if(ret!=1)
            {
                delete IniFile;
                return 2;
            }

            HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iData);
            IniFile->WriteInteger("TabMolding", "edMotorMoldingSpeed", iData);
            HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iData);
            IniFile->WriteInteger("TabMolding", "edMotorDownDelay", iData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldCynDelay1", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldCynDelay2", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldCynDelay3", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldCynDelay4", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldCynDelay5", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMoldingTime", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMold1_1_SetTorque", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMold1_2_SetTorque", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMold2_1_SetTorque", dData);
            HGemPtr->DataItemIn(1, HType.FT_8_TYPE,  &dData);
            IniFile->WriteFloat("TabMolding", "edMold2_2_SetTorque", dData);
            HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iData);
            IniFile->WriteInteger("TabMolding", "edtMoldCount", iData);
        }
    }
    delete IniFile;
*/
    return 0;
}
//------------------------------------------------------------------------------
int HT9045Gem::S7F24_FormattedProcessProgramSendAcknowledge()
{
    unsigned char HCACK=1;
    if(HasICUnderMachine()==true || SystemStart==true)                          //20140124 wei
        HCACK=1;
    else
        HCACK=ProcessS7F23FromatReceipe();

    HGemPtr->InitLocalHead(7, 24, 0);
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
    HGemPtr->SendLocalData();
    return HCACK;
}
//------------------------------------------------------------------------------
int HT9045Gem::ProcessS7F25FromatReceipe()                                      //從檔案讀取
{
//    double dData;
//    int    iData;
//    int ret,SVlen,i, j,k,ret1,ret2,len;
//
//    char PPID[256];
//    char GemSoft[256];
//    char GemMDSN[256];
//    char CCODE[256];
//    AnsiString S,S1;
//    bool bSearchOK=false;

//    ret=HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE);
//    if( ret!=1)
//        return 2;
//    if(SVlen!=4)
//        return 2;    // format error
/*
    HGemPtr->DataItemIn(256   ,HType.ASCII_TYPE,PPID);                          //

    S=PPID;
    S=S.UpperCase();
    for(i=0; i<fMain->cbSetupFileName->Items->Count; i++)                       //cb_WorkFile
    {
        S1=fMain->cbSetupFileName->Items->Strings[i];
        S1=S1.UpperCase();
        if(S==S1)
        {
            bSearchOK=true;
            break;
        }
    }

    if(bSearchOK==false)
        return 4;                                                               // PPID not found

    S1=CurrentDir+AnsiString("\\data\\")+S+AnsiString(".ini");
    TIniFile *IniFile = new TIniFile(S1);

    HGemPtr->InitLocalHead(7, 26, 2);
    HGemPtr->DataItemOut(4, HType.LIST_TYPE,NULL);                              // L,4
    HGemPtr->DataItemOut(S.Length(),          HType.ASCII_TYPE, S.c_str()         );                                    //    1. <PPID>
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN   );                //    1. <MDLN>
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);                //    2. <SOFTREV>
    HGemPtr->DataItemOut(1, HType.LIST_TYPE, NULL);                             // L,1
    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);                             // L,4
    HGemPtr->DataItemOut(strlen("Molding"), HType.ASCII_TYPE,"Molding");
    HGemPtr->DataItemOut(13, HType.LIST_TYPE,NULL);                             // L,13
    iData=IniFile->ReadInteger("TabMolding", "edMotorMoldingSpeed", 0);
    HGemPtr->DataItemOut(1, HType.INT_4_TYPE, &iData);                          //fSetup->edMotorMoldingSpeed
    iData=IniFile->ReadInteger("TabMolding", "edMotorDownDelay", 0);
    HGemPtr->DataItemOut(1, HType.INT_4_TYPE, &iData);                          //fSetup->edMotorDownDelay
    dData=IniFile->ReadFloat("TabMolding", "edMoldCynDelay1", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldCynDelay1
    dData=IniFile->ReadFloat("TabMolding", "edMoldCynDelay2", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldCynDelay2
    dData=IniFile->ReadFloat("TabMolding", "edMoldCynDelay3", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldCynDelay3
    dData=IniFile->ReadFloat("TabMolding", "edMoldCynDelay4", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldCynDelay4
    dData=IniFile->ReadFloat("TabMolding", "edMoldCynDelay5", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldCynDelay5
    dData=IniFile->ReadFloat("TabMolding", "edMoldingTime", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMoldingTime
    dData=IniFile->ReadFloat("TabMolding", "edMold1_1_SetTorque", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMold1_1_SetTorque
    dData=IniFile->ReadFloat("TabMolding", "edMold1_2_SetTorque", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMold1_2_SetTorque
    dData=IniFile->ReadFloat("TabMolding", "edMold2_1_SetTorque", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMold2_1_SetTorque
    dData=IniFile->ReadFloat("TabMolding", "edMold2_2_SetTorque", 0.0);
    HGemPtr->DataItemOut(1, HType.FT_8_TYPE,  &dData);                          //fSetup->edMold2_2_SetTorque
    iData=IniFile->ReadInteger("TabMolding", "edtMoldCount", 0);
    HGemPtr->DataItemOut(1, HType.INT_4_TYPE, &iData);                          //fSetup->edtMoldCount
    HGemPtr->SendLocalData();

    delete IniFile;
*/
    return 0;
}
//------------------------------------------------------------------------------
int HT9045Gem::S7F26_FormattedProcessProgramData()
{
    unsigned char HCACK=1;
    HCACK=HSys.MyGem->ProcessS7F25FromatReceipe();

    if(HCACK!=0)
    {
        HGemPtr->InitLocalHead(7, 26, 0);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        HGemPtr->SendLocalData();
    }
    return HCACK;
}

//---------------------------------------------------------------------------
extern int Process_2DID_BinCode(AnsiString s2DID_Source);                       //Steven 20231111 : 調整XML讀取
//------------------------------------------------------------------------------
void HT9045Gem::S14F4_Get2DID_BinCode()                                         //JerryYang 20230322 : Get 2DID bin map
{
    unsigned char HCACK=1;
    char *CommandStr;
    AnsiString PPID, S;
    int ret, len;
    unsigned char Type;

    CommandStr=new char [10000000];                                             //JerryYang 20221020 test

    if(HasICUnderMachine()==true || SystemStart==true)
    {
        HCACK=4;
    }
    else
    {
        if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
        {
            if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
            {
                HGemPtr->GetDataItemLenAndType(len, Type);
                if(Type==HType.ASCII_TYPE)
                {
                    ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                    if(ret==-1)
                        S="";
                    else
                        S=CommandStr;
                }
                else
                {
                    S="";
                }

                if(S!="")
                {
                    if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
                    {
                        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                S=CommandStr;
                                if(ret==-1)
                                {
                                    S="";
                                }
                                else
                                {
                                    S=CommandStr;
                                    S=S.UpperCase();

                                    if(S=="SUBSTRATETYPE")                      //JerryYang 20200330
                                    {
                                        HGem->GetDataItemLenAndType(len, Type);
                                        if(Type==HType.ASCII_TYPE)
                                        {
                                            ret=HGem->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                            if(ret==-1)
                                            {
                                                HCACK=3;
                                            }
                                            else
                                            {
                                                HCACK=Process_2DID_BinCode(CommandStr);                                 //Steven 20231111 : 調整XML讀取
                                            }
                                        }
                                        else
                                        {
                                            HCACK=3;
                                        }
                                    }
                                    else
                                    {
                                        HCACK=3;
                                    }
                                }
                            }
                            else
                            {
                                HCACK=3;
                            }
                        }
                    }
                }
            }
        }
        delete[] CommandStr;                                                    //Steven 20160912 : Add delete for save memory
        CommandStr=NULL;
        HGemPtr->InitLocalHead(14, 4, 0);

        if(HCACK==0)
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }
        else
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }

        HGemPtr->SendLocalData();
        return;
    }
    return;
}
//------------------------------------------------------------------------------
//pig 2014.07.04 ASEM SECS GEM start
void HT9045Gem::S110F5_RequestCustomerNameList()
{
    HGemPtr->InitLocalHead(110, 5, 1);
    HGemPtr->DataItemOut(1, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    HGemPtr->SendLocalData();
}
//------------------------------------------------------------------------------
//L,n
//    1. L,2
//        1. <LSID1>
//        2. <LEVEL1>
//    2. L,2
//        1. <LSID2>
//        2. <LEVEL2>
//    .
//    .
//    n. L,2
//        1. <LSIDn>
//        2. <LEVELn>
//------------------------------------------------------------------------------
void HT9045Gem::S125F4_LevelSettingChangeAcknowledge()                          //Steven 20150605 : S125F3 LevelSettingChangeRequest
{
    int SVLen, len;
    unsigned char HCACK=0;
    unsigned char Type;
    int LSID, LEVEL;

    if(HGem->GetDataItemLenAndTypeAndDelete(SVLen, Type)==1)                    //wei 20150630
    {
        if(SVLen!=0 && Type==HType.LIST_TYPE)
        {
            for(int i=0; i<SVLen; i++)
            {
                if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                {
                    HGem->GetDataItemLenAndType(len,Type);                      //取得資料長度與格式
                    HGem->DataItemIn( len,Type,&LSID);                          //取值
                    HGem->GetDataItemLenAndType(len,Type);                      //取得資料長度與格式
                    HGem->DataItemIn( len,Type,&LEVEL);                         //取值
                    SetECValue(LSID, &LEVEL);
                }
                else
                {
                    HCACK=1;
                }
            }
        }
        else
        {
            HCACK=1;
        }
        HGemPtr->LocalAcknowledge(125, 4, HCACK);
    }

    if(HCACK==1)
        S9F7_IllegalData("S125,F3 Data Format error !!!");
    else
        fSecurity->SetLevelSet();
}
//------------------------------------------------------------------------------
