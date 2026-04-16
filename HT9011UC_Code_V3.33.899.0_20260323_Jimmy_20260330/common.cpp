#include "MachineDefine.h"
#pragma hdrstop

#include "common.h"

#include "mymessbox.h"
#include "MachineType.h"
#include "cmydef.h"
#include "cMyDB.h"
#include "HS_Function.h"
#include "uCleaning.h"
#include "uHGemEquipment.h"
#include "FileInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString asGeneralPath            ="D:\\HT9045\\system\\Gerneral.ini";
AnsiString asBarCodeLogPath         ="D:\\HT9045_Log\\2DBarCode";
AnsiString asBarCodeLot             ="D:\\HT9045_Log\\2DBarCode\\LotData.txt";
AnsiString asBarCodeCommLogPath     ="D:\\HT9045_Log\\2DBarCodeComm";           //Steven 20160421 : Add 2D CCD Log
AnsiString asTrayMapCommLogPath     ="D:\\HT9045_Log\\TrayMapComm";             //wei 20161219 Tray Mapping
AnsiString asKeyenceCommLogPath     ="D:\\HT9045_Log\\KeyenceComm";             //wei 20161219 Tray Mapping
AnsiString asTrayMapLogPath         ="D:\\HT9045_Log\\TrayMapLog";              //wei 20161219 Tray Mapping
AnsiString asTrayMapDataPath        ="D:\\HT9045_Log\\TrayMapLog\\TrayData";    //wei 20161219 Tray Mapping
AnsiString asSummaryPath            ="D:\\HT9045_Log\\Summary";
AnsiString asFix2AICCDDataPath      ="D:\\HT9045_Log\\Fix2AICCDLog\\Fix2AICCDData"; //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
AnsiString asSystemPath             ="d:\\HT9045\\system\\";
AnsiString asDBPath                 ="D:\\HT9045\\MDB\\Handler.db3";            //Steven 20090817
AnsiString AuthPath                 ="D:\\HT9045\\config\\";
AnsiString DefaultPath              ="D:\\HT9045\\IniData\\";
AnsiString DataPath                 ="D:\\HT9045\\IniData\\Data\\";
AnsiString OffsetPath               ="D:\\HT9045\\IniData\\Offset\\";
AnsiString DIOCFGPath               ="D:\\HT9045\\iniData\\DioCfg\\";
AnsiString asErrNotePath            ="D:\\HT9045\\system\\SpecialErrNote.ini";
AnsiString ConfigMemoPath           ="D:\\HT9045\\config\\Description.ini";
AnsiString LastDataPath             ="D:\\HT9045\\SetUp.inf";
AnsiString BmpPath                  ="D:\\HT9045\\IMG\\BMP\\";
AnsiString IoTablePath              ="D:\\HT9045\\System\\IO_Table.csv";
AnsiString MotTablePath             ="D:\\HT9045\\System\\Mot_Table.csv";
AnsiString TrayTablePath            ="D:\\HT9045\\System\\TrayForm.csv";
AnsiString PlateTablePath           ="D:\\HT9045\\System\\PlateForm.csv";
AnsiString GrapicPath               ="D:\\HT9045\\IMG\\Graphic\\";
AnsiString SecsGemPath              ="D:\\HT9045\\SECS\\SECS\\SYSTEM\\Gerneral.ini";
AnsiString asKeyStr                 ="HontechPassword";                         //Steven 20240105 : 密碼本的金鑰, 不能改
AnsiString as9045UPH                ="D:\\HT9045_Log\\UPH";                     //Frank 20150515 新增UPH資料夾
AnsiString as9045LogPath            ="D:\\HT9045_Log";                          //Steven 20141229 : 修改Log Path
AnsiString asEPLogPath              ="D:\\HT9045_Log\\EP";
AnsiString asHiSiLogPath            ="D:\\HT9045_Log\\Temperature";
AnsiString asLbTempLogPath          ="D:\\HT9045_Log\\LB_Temperature";          //Steven 20250110 : LB溫度log
AnsiString asShtLogPath             ="D:\\HT9045_Log\\ShuttleLog";
AnsiString asHomeLogPath            ="D:\\HT9045_Log\\HomeLog";
AnsiString asTorqLogPath            ="D:\\HT9045_Log\\TorqueLog";
AnsiString asACSmartIntervalLogPath ="D:\\HT9045_Log\\ACSmartIntervalLog";      //Sam 20240726 : AI Clean
AnsiString asTrayLogPath            ="D:\\HT9045_Log\\EditTray";
AnsiString asTempLogPath            ="D:\\HT9045_Log\\Temp";
AnsiString asEJ1NLogPath            ="D:\\HT9045_Log\\EJ1N_Log";
AnsiString asLaserLogPath           ="D:\\HT9045_Log\\LaserLog";
AnsiString asASELogPath             ="D:\\SaveRecord";
AnsiString asTravelingLogPath       ="D:\\HT9045_Log\\Production_Log";
AnsiString asOLPLogPath             ="D:\\HT9045_Log\\Automation";
AnsiString asSQLLogPath             ="D:\\HT9045_Log\\SQL_LOG";                 //Steven 20150426 : 紀錄SQL存取速度
AnsiString asGAILLogPath            ="D:\\HT9045_Log\\GAIL_LOG";                //kevin 20150909 : 紀錄Gail 通訊
AnsiString asBarCodeLogPath1        ="D:\\HT9045_Log\\BarCode_LOG";             //wei 20150720 OCR BarCode_LOG
AnsiString asHeaterOnLogPath        ="D:\\HT9045_Log\\Heater_On_Off_LOG";       //Steven 20151123 : Log for Heater Relay
AnsiString asYieldRecordPath        ="D:\\HT9045_Log\\Yield";                   //Ifor 20151221 :新增Yield Record
AnsiString asESDLogPath             ="D:\\HT9045_Log\\ESD";                     //kevin 20191212 Handle sens HISI ESD path
AnsiString asAlarmLogXmlPath        ="D:\\HT9045_Log\\AlarmLogXml";

AnsiString asPadCommLogPath         ="D:\\HT9045_Log\\PadCommLog";              //Ken 20210702 AddPadInterface
AnsiString asUDPLogPath             ="D:\\HT9045_Log\\UDP";                     //kevin 20211020 UDP
AnsiString asVibrateMotCommLogPath  ="D:\\HT9045_Log\\VibrateMotorCommLog";     //JerryYang 20230814 : add震動馬達通訊調速版本
AnsiString asProductionLogPath      ="D:\\RMS";                                 //JerryYang 20151225 Production Log for SPIL蘇州
AnsiString asPMAlarmPath            ="D:\\HT9045\\PMAlarm";                     //wei 20160225 PMAlarmFunction
AnsiString asProductDataPath        ="D:\\HT9045_Log\\ProductData\\";           //kevin 20160724 :新增Auto 123 Fix 123 數量Record
AnsiString asProductRecordPath      ="D:\\HT9045_Log\\ProductRecord";           //JerryYang 20230721 : Analog要求production record
AnsiString asQtyDataPath            ="D:\\HT9045_Log\\QtyData\\";
AnsiString sPMList_Month            ="D:\\HT9045\\PMAlarm\\PM_Month.ini";
AnsiString sPMList_Quarter          ="D:\\HT9045\\PMAlarm\\PM_Quarter.ini";
AnsiString sPMList_Year             ="D:\\HT9045\\PMAlarm\\PM_Year.ini";
AnsiString sPMList_Temperature      ="D:\\HT9045\\PMAlarm\\PM_Temperature.ini";
AnsiString sPMList_ESD              ="D:\\HT9045\\PMAlarm\\PM_ESD.ini";
AnsiString sPMList_IonFan           ="D:\\HT9045\\PMAlarm\\PM_IonFan.ini";
AnsiString sPMItem_List             ="D:\\HT9045\\PMAlarm\\PM_List.ini";
AnsiString sPMItem_Check            ="D:\\HT9045\\PMAlarm\\PM_Check.ini";
AnsiString sPMItem_ListTemperature  ="D:\\HT9045\\PMAlarm\\PM_ListTemperature.ini";
AnsiString sPMItem_CheckTemperature ="D:\\HT9045\\PMAlarm\\PM_CheckTemperature.ini";
AnsiString sPMItem_ListESD          ="D:\\HT9045\\PMAlarm\\PM_ListESD.ini";
AnsiString sPMItem_CheckESD         ="D:\\HT9045\\PMAlarm\\PM_CheckESD.ini";
AnsiString sPMItem_ListIonFan       ="D:\\HT9045\\PMAlarm\\PM_ListIonFan.ini";
AnsiString sPMItem_CheckIonFan      ="D:\\HT9045\\PMAlarm\\PM_CheckIonFan.ini";
AnsiString sPMSetting               ="D:\\HT9045\\PMAlarm\\PM_Setting.ini";
AnsiString JOBFILEDataPath          ="JOBFILE";                                 //kevin 20140105  高雄福雷
AnsiString JOBFILEOffsetPath        ="JOBFILE";                                 //kevin 20140105  高雄福雷
AnsiString sBarcodeFile             ="D:\\HT9045\\Barcode_File\\";              //wei 20160728 Barcode File切換
AnsiString asBarCodeDownLot         ="D:\\HT9045_Log\\2DBarCode\\LotCheckData.txt";  //wei 20160824 更改檔案名稱
AnsiString asSearch2DIDByLot        ="D:\\HT9045_Log\\2DBarCode\\Search2DIDByLot.txt";   //Frank 20170316 (wei) add Search 2DID By Lot
AnsiString asSortingBy2DID          ="D:\\HT9045_Log\\2DBarCode\\SortingBy2DID.txt";     //JerryYang 20190313 : 2D sorting
AnsiString as2DIDSortToError        ="D:\\HT9045_Log\\2DBarCode\\2DID_list.txt";         //JerryYang 20231218 : 2DID黑名單功能
AnsiString asIndexZphasePath        ="D:\\HT9045_Log\\Zphase";                  //kevin 20170511 (wei) add Zphase
AnsiString asARSMParaPath           ="D:\\HT9045\\system\\ARMS.ini";            //Ifor 20170621 (wei) add ARMS Function
AnsiString sProductionInfoFilePath = "D:\\HT9045_log\\ProductionInfo";          //Sam 20170603 (Steven) 移植超豐功能 form HT-7045
AnsiString asLightScalePath         ="D:\\HT9045_Log\\LightScale\\";
AnsiString asTCPIPPath              ="D:\\HT9045_Log\\TCPIP_Log";               //Isaac 20170809 (wei) TCPIP log
AnsiString asRenesasCommPath        ="D:\\HT9045_Log\\RenesasComm_Log";         //RogerYang 20250812 : 瑞薩FT-CT
AnsiString asRenesasTaskPath        ="D:\\HT9045_Log\\RenesasTask_Log";         //RogerYang 20250812 : 瑞薩FT-CT
AnsiString aESDSetDataFileName      ="D:\\ESD_Program\\system\\General.ini";    //kevin 20180907 (Steven) : add ESD Data
AnsiString asGalilCmdPath           ="D:\\HT9045_Log\\Galil_Log";               //JerryYang 20181130 (Steven) : add galil command log
AnsiString asASECLFTPPath           ="D:\\ASECLFTP";                            //Steven 20181224 : For ASE-CL  //JerryYang 20220215 : 松諭要求修改路徑
AnsiString asChipMosFTPPath         ="D:\\HT9045_Log\\FTP";                     //Steven 20210413 : 南茂的自動Start功能
AnsiString asChipMosFTPPath2        ="D:\\HT9045_Log\\ChipMosFTP";              //Steven 20210413 : 南茂的自動Start功能
AnsiString asSaveEventLogPath       ="D:\\HT9045_Log\\SaveEventLog";
AnsiString asOCRSaveMane            ="D:\\HT9045_Log\\OCR\\Alarm\\Bad";
char ConverStr[16];
AnsiString asTeachPath              ="D:\\HT9045\\system\\teach.ini";           //kevin 20190305 add teach.ini
AnsiString asHandlePath             ="D:\\HT9045_Log\\Handlelog";               //kevin 20190411 handle log
AnsiString sJamRatePath             ="D:\\HT9045_Log\\JamRate";
AnsiString sDailyJamPath            ="D:\\HT9045_Log\\JamRate_Daily";
AnsiString sFTPSetupFileLogPath     ="D:\\HT9045_Log\\FTPSetupFileChangeLog";   //Sam 20210803 : FTP SetFile Change Log
AnsiString sSaveByMachine           ="D:\\HT9045\\IniData\\SaveByMachine\\";    //JimmyChiu 20220618 : save by machine

AnsiString as3SigmaLogPath          ="D:\\HT9045_log\\TemperatureLog_3Sigma";   //kevin 20200521 add 3 SIGMA  Hmy 20200515 Copy//Hmy 20200510 Add Enable 3 Sigma Temp Monitor
AnsiString asTrayStepSpeedByMachinePatch ="D:\\HT9045\\system\\TrayStepSpeed.ini";      //Sam 20201221 : Tray y step motor by machine
AnsiString asProduct_LoaderPath      ="D:\\HT9045_Log\\Production_Loader";      //Sam 20210107 : Summary Report fuction
AnsiString asVacuumOnOffLogPath     ="D:\\HT9045_log\\VacuumOnOffLog";          //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
AnsiString asHPCardPath             ="D:\\HT9045_log\\HPCARD";                  //kevin 202111111 MCU Command log
AnsiString asDefaultRecipeChangeLogPath  ="D:\\HT9045_Log\\DefaultRecipeChangeLog";     //Sam 20201209 : Default Recipe ChangeLog
AnsiString asTestTCPIPLogPath       ="D:\\HT9045_Log\\Test_TCPIP";              //wei 20211022 Tester TCP/IP
AnsiString sGroundESDLogPath        ="D:\\HT9045_Log\\GroundESDLog";                    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
AnsiString aAutoTempureOffsetFTP    ="D:\\HT9045_Log\\AutoTempureOffsetFTP";            //Sam 20220406 : 溫度自動補償功能 By FTP
AnsiString aAutoTempureOffsetFTPBackup="D:\\HT9045_Log\\AutoTempureOffsetFTPBackup";    //Sam 20220406 : 溫度自動補償功能 By FTP
AnsiString aDownloadUpdateAutomaticallyPath="D:\\HT9045_Log\\Installer";        //Sam 20220824 : FTP 自動下載安裝更新包
AnsiString asGroundManPath           ="D:\\HT9045_log\\GroundManLog\\GroundManValue";   //KenHsieh 20220728 : 新增GroundMan Value Log
AnsiString asShtSenLogPath           ="D:\\HT9045_Log\\ShuttleSensorLog\\";     //kevin 20220912 add shuttle sensor log
AnsiString asInArmAutoAlignmentPath  ="D:\\HT9045_log\\AOA_Log\\InArm";         //KenHsieh 20220111 : AOA add Save Log
AnsiString asOutArmAutoAlignmentPath ="D:\\HT9045_log\\AOA_Log\\OutArm";        //KenHsieh 20220111 : AOA add Save Log
AnsiString abHandlerStateChangeUploadServerPath="D:\\HT9045_Log\\HandlerStateChangeUploadServer";   //Sam 20230511 : 機台資料變更後須上傳 FTP
AnsiString asCleanPadLogPath         ="D:\\HT9045_Log\\CleanPad_Log";           //Sam 20230616 : Add Auto Clean Record
AnsiString asBackup2DSortListPath    ="D:\\HT9045_log\\2D_SortList_Backup\\";   //JerryYang 20230822 : 佳熹要求備份
AnsiString asOCRDownLoadLotPath     ="D:\\HT9045_Log\\OCR\\CompareFile";        //KenHsieh 20220825 : 新增OCR比對功能
AnsiString asOCRLotPath             ="D:\\HT9045_Log\\OCR\\OCRLot.txt";         //KenHsieh 20220825 : 新增OCR比對功能
AnsiString asProductionByFilePath   ="D:\\HT9045_Log\\Production_ByFile";       //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
AnsiString asTesterReportPath        ="C:\\HontechTestLogs";                    //Sam 20240930 : 客戶彭鴻昌要求改 //Sam 20231115 : PTI 新增 Tester report
TIniFile *INIFileGeneral ;                                                      //Steven 20141120 : Add Read/Write IniFile Speed
TIniFile *INIFile ;                                                             //Steven 20141120 : Add Read/Write IniFile Speed

TMemIniFile *INIFileMem;                                                        //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制

AnsiString asCheckIndexYLogPath="D:\\HT9045_log\\CheckIndexYLog";               //Ztex 2023.05.11 Add CheckIndexY
AnsiString asBackup2DSummaryPath     ="D:\\HT9045_log\\Summary_Backup\\";       //JerryYang 20231205 : 備份2DID summary
AnsiString asDupBundleID             ="D:\\HT9045_Log\\BundleID\\DupBundleID.txt";
AnsiString aslDupUnloadBundlID       ="D:\\HT9045_Log\\BundleID\\DupUnloadBundleID.txt";
AnsiString as2DWhiteListLog          ="D:\\HT9045_Log\\2DBarCode\\CheckResult\\";  //JerryYang 20241104 : 支援2DID白名單功能
AnsiString as2DWhiteListLogName      ="";
AnsiString SVNRevision               ="";

SHFILEOPSTRUCT oFile;
int CheckPtrItem=0;
AnsiString asJamStatPath             ="D:\\HT9045_Log\\JamStatList";            //Ifor 20220310 add:JamStatList Path
AnsiString asPowerSavePath           ="D:\\HT9045_Log\\Power_EnergySaving";     //Ifor 20220927 add:Power Save Path
AnsiString asATCPowerSavePath        ="D:\\HT9045_Log\\ATC_EnergySaving";       //Ifor 20240410 add:ATC Power Save Path
AnsiString asTempLog_HS              ="D:\\HandlerLog\\Temperature_Log\\";
AnsiString asEPLog_HS                ="D:\\HandlerLog\\EP_Log\\";
AnsiString asESDLog_HS               ="D:\\HandlerLog\\ESD_Log\\";
AnsiString asATCEventLog_HS          ="D:\\HandlerLog\\ATCEven_Log\\";
AnsiString asArmTestLog_HS           ="D:\\HandlerLog\\ArmTestInfo\\";
AnsiString sGroundESDInterValPath    ="D:\\HT9045_Log\\RecordGroundESDInterval";    //Sam 20250609 : Record Ground and ESD at intervals and upload
AnsiString sWhite2DIDListLoc         ="D:\\HT9045_Log\\White2DIDListTmp\\";     //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能

//---------------------------------------------------------------------------
void InitCommonString()                                                         //jou 2016-08-24 Initial Common String
{
    asGeneralPath        ="d:\\HT9045\\system\\Gerneral.ini";
    asBarCodeLogPath     ="D:\\HT9045_Log\\2DBarCode";
    asBarCodeLot         ="D:\\HT9045_Log\\2DBarCode\\LotData.txt";
    asBarCodeCommLogPath ="D:\\HT9045_Log\\2DBarCodeComm";                      //Steven 20160421 : Add 2D CCD Log
    asTrayMapCommLogPath ="D:\\HT9045_Log\\TrayMapComm";                        //wei 20161219 Tray Mapping
    asKeyenceCommLogPath ="D:\\HT9045_Log\\KeyenceComm";                        //wei 20161219 Tray Mapping
    asTrayMapLogPath     ="D:\\HT9045_Log\\TrayMapLog";                         //wei 20161219 Tray Mapping
    asTrayMapDataPath    ="D:\\HT9045_Log\\TrayMapLog\\TrayData";               //wei 20161219 Tray Mapping
    asFix2AICCDDataPath  ="D:\\HT9045_Log\\Fix2AICCDLog\\Fix2AICCDData";        //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    asSystemPath         ="d:\\HT9045\\system\\";
    asDBPath             ="D:\\HT9045\\MDB\\Handler.db3";                       //Steven 20090817
    AuthPath             ="D:\\HT9045\\config\\";
    DefaultPath          ="D:\\HT9045\\IniData\\";
    DataPath             ="D:\\HT9045\\IniData\\Data\\";
    OffsetPath           ="D:\\HT9045\\IniData\\Offset\\";
    DIOCFGPath           ="D:\\HT9045\\iniData\\DioCfg\\";
    asErrNotePath        ="D:\\HT9045\\system\\SpecialErrNote.ini";
    ConfigMemoPath       ="D:\\HT9045\\config\\Description.ini";
    LastDataPath         ="D:\\HT9045\\SetUp.inf";
    BmpPath              ="D:\\HT9045\\IMG\\BMP\\";
    GrapicPath           ="D:\\HT9045\\IMG\\Graphic\\";
    SecsGemPath          ="D:\\HT9045\\SECS\\SYSTEM\\Gerneral.ini";
    asKeyStr             ="HontechPassword";                                    //Steven 20240105 : 密碼本的金鑰, 不能改
    as9045UPH            ="D:\\HT9045_Log\\UPH";                                //Frank 20150515 新增UPH資料夾
    as9045LogPath        ="D:\\HT9045_Log";                                     //Steven 20141229 : 修改Log Path
    asEPLogPath          ="D:\\HT9045_Log\\EP";
    asHiSiLogPath        ="D:\\HT9045_Log\\Temperature";
    asShtLogPath         ="D:\\HT9045_Log\\ShuttleLog";
    asHomeLogPath        ="D:\\HT9045_Log\\HomeLog";
    asTorqLogPath        ="D:\\HT9045_Log\\TorqueLog";
    asACSmartIntervalLogPath ="D:\\HT9045_Log\\ACSmartIntervalLog";             //Sam 20240726 : AI Clean
    asTrayLogPath        ="D:\\HT9045_Log\\EditTray";
    asTempLogPath        ="D:\\HT9045_Log\\Temp";
    asEJ1NLogPath        ="D:\\HT9045_Log\\EJ1N_Log";
    asLaserLogPath       ="D:\\HT9045_Log\\LaserLog";
    asASELogPath         ="D:\\SaveRecord";
    asTravelingLogPath   ="D:\\HT9045_Log\\Production_Log";
    asOLPLogPath         ="D:\\HT9045_Log\\Automation";
    asSQLLogPath         ="D:\\HT9045_Log\\SQL_LOG";                            //Steven 20150426 : 紀錄SQL存取速度
    asGAILLogPath        ="D:\\HT9045_Log\\GAIL_LOG";                           //kevin 20150909 : 紀錄Gail 通訊
    asBarCodeLogPath1    ="D:\\HT9045_Log\\BarCode_LOG";                        //wei 20150720 OCR BarCode_LOG
    asHeaterOnLogPath    ="D:\\HT9045_Log\\Heater_On_Off_LOG";                  //Steven 20151123 : Log for Heater Relay
    asYieldRecordPath    ="D:\\HT9045_Log\\Yield";                              //Ifor 20151221 :新增Yield Record
    asProductionLogPath  ="D:\\RMS";                                            //JerryYang 20151225 Production Log for SPIL蘇州
    asPMAlarmPath        ="D:\\HT9045\\PMAlarm";                                //wei 20160225 PMAlarmFunction
    asProductDataPath    ="D:\\HT9045_Log\\ProductData\\";                      //kevin 20160724 :新增Auto 123 Fix 123 數量Record
    sPMList_Month            ="D:\\HT9045\\PMAlarm\\PM_Month.ini";
    sPMList_Quarter          ="D:\\HT9045\\PMAlarm\\PM_Quarter.ini";
    sPMList_Year             ="D:\\HT9045\\PMAlarm\\PM_Year.ini";
    sPMList_Temperature      ="D:\\HT9045\\PMAlarm\\PM_Temperature.ini";
    sPMList_ESD              ="D:\\HT9045\\PMAlarm\\PM_ESD.ini";
    sPMList_IonFan           ="D:\\HT9045\\PMAlarm\\PM_IonFan.ini";
    sPMItem_List             ="D:\\HT9045\\PMAlarm\\PM_List.ini";
    sPMItem_Check            ="D:\\HT9045\\PMAlarm\\PM_Check.ini";
    sPMItem_ListTemperature  ="D:\\HT9045\\PMAlarm\\PM_ListTemperature.ini";
    sPMItem_CheckTemperature ="D:\\HT9045\\PMAlarm\\PM_CheckTemperature.ini";
    sPMItem_ListESD          ="D:\\HT9045\\PMAlarm\\PM_ListESD.ini";
    sPMItem_CheckESD         ="D:\\HT9045\\PMAlarm\\PM_CheckESD.ini";
    sPMItem_ListIonFan       ="D:\\HT9045\\PMAlarm\\PM_ListIonFan.ini";
    sPMItem_CheckIonFan      ="D:\\HT9045\\PMAlarm\\PM_CheckIonFan.ini";
    sPMSetting               ="D:\\HT9045\\PMAlarm\\PM_Setting.ini";
    JOBFILEDataPath          ="JOBFILE";                                        //kevin 20140105  高雄福雷
    JOBFILEOffsetPath        ="JOBFILE";                                        //kevin 20140105  高雄福雷
    sBarcodeFile             ="D:\\HT9045\\Barcode_File\\";                     //wei 20160728 Barcode File切換
    asBarCodeDownLot         ="D:\\HT9045_Log\\2DBarCode\\LotData1.txt";
    asSearch2DIDByLot        ="D:\\HT9045_Log\\2DBarCode\\Search2DIDByLot.txt"; //Frank 20170316 (wei) add Search 2DID By Lot
    asSortingBy2DID          ="D:\\HT9045_Log\\2DBarCode\\";                    //JerryYang 20190313 : 2D sorting
    asGalilCmdPath           ="D:\\HT9045_Log\\Galil_Log";                      //JerryYang 20181130 (Steven) : add galil command log
    asProduct_LoaderPath     ="D:\\HT9045_Log\\Production_Loader";              //Sam 20210107 : Summary Report fuction
    asAlarmLogXmlPath        ="D:\\HT9045_Log\\AlarmLogXml";
    sFTPSetupFileLogPath     ="D:\\HT9045_Log\\FTPSetupFileChangeLog";
    asTestTCPIPLogPath       ="D:\\HT9045_Log\\Test_TCPIP";                     //wei 20211022 Tester TCP/IP
    asShtSenLogPath          ="D:\\HT9045_Log\\ShuttleSensorLog\\";             //kevin 20220912 add shuttle sensor log
    asCleanPadLogPath        ="D:\\HT9045_Log\\CleanPad_Log";                   //Sam 20230616 : Add Auto Clean Record
    asBackup2DSortListPath   ="D:\\HT9045_log\\2D_SortList_Backup\\";           //JerryYang 20230822 : 佳熹要求備份
    asOCRDownLoadLotPath     ="D:\\HT9045_Log\\OCR\\CompareFile";               //KenHsieh 20220825 : 新增OCR比對功能
    asOCRLotPath             ="D:\\HT9045_Log\\OCR\\OCRLot.txt";                //KenHsieh 20220825 : 新增OCR比對功能
    asProductionByFilePath   ="D:\\HT9045_Log\\Production_ByFile";              //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
    asBackup2DSortListPath   ="D:\\HT9045_log\\2D_SortList_Backup\\";           //JerryYang 20231205 : 備份2DID summary
    asBackup2DSummaryPath    ="D:\\HT9045_log\\Summary_Backup\\";
    asPowerSavePath          ="D:\\HT9045_Log\\Power_EnergySaving";             //Ifor 20220927 add:Power Save Path
    asATCPowerSavePath       ="D:\\HT9045_Log\\ATC_EnergySaving";               //Ifor 20240410 add:ATC Power Save Path
    asDupBundleID            ="D:\\HT9045_Log\\BundleID\\DupBundleID.txt";
    aslDupUnloadBundlID      ="D:\\HT9045_Log\\BundleID\\DupUnloadBundleID.txt";
}
//---------------------------------------------------------------------------
AnsiString EncodeStr(AnsiString sourceStr)                                      //jou 2013-01-04 Password Txt 加密
{   //編碼
    int l1, l2, p1, p2;
    byte ch, ctemp;
    AnsiString Result;
    l1=sourceStr.Length();
    p1=1;
    l2=asKeyStr.Length();
    p2=1;
    Result="";
    while(p1<=l1)
    {
        ctemp=(byte(sourceStr[p1])-1)^byte(asKeyStr[p2]);
        if(ctemp==0 || ctemp=='\0')
            ch=(byte(asKeyStr[p2]));
        else
            ch=ctemp;

        Result=Result+char(ch);
        p1++;
        if(p2>=l2)
            p2=1;
        else
            p2++;
    }
    return Result;
}
//---------------------------------------------------------------------------
AnsiString DecodeStr(AnsiString sourceStr)                                      //jou 2013-01-04 Password Txt 解碼
{
    int l1, l2, p1, p2;
    byte ch;
    AnsiString Result;
    l1=sourceStr.Length();
    p1=1;
    l2=asKeyStr.Length();
    p2=1;
    Result="";
    while(p1<=l1)
    {
        ch=byte(sourceStr[p1]);
        if(ch==byte(asKeyStr[p2]))                                              //Steven 20210330 : 修正有可能加密出來剛好等於\0的狀態
            ch=byte(asKeyStr[p2]);
        else
            ch=ch^byte(asKeyStr[p2]);

        Result=Result+char(ch+1);
        p1++;
        if(p2>=l2)
            p2=1;
        else
            p2++;
    }
    return Result;
}
//---------------------------------------------------------------------------
bool OpenIniFile(AnsiString FileName)                                           //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(FileName=="")                                                            //Steven 20160606 : Add protection for inifile
        return false;

    if(INIFile==NULL || INIFile->FileName!=FileName)
    {
        CloseIniFile();
        INIFile=new TIniFile(FileName);
    }
    return true;
}
//---------------------------------------------------------------------------
void CloseIniFile()                                                             //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(INIFile!=NULL)
    {
        INIFile->UpdateFile();
        delete INIFile;
    }
}
//---------------------------------------------------------------------------
bool OpenIniFileMem(AnsiString FileName)                                        //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    if(FileName=="")
        return false;

    if(INIFileMem==NULL || INIFileMem->FileName!=FileName)
    {
        CloseIniFileMem();
        INIFileMem=new TMemIniFile(FileName);
    }
    return true;
}
//---------------------------------------------------------------------------
void CloseIniFileMem()                                                          //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    if(INIFileMem!=NULL)
    {
        INIFileMem->UpdateFile();
        delete INIFileMem;
    }
}
//---------------------------------------------------------------------------
bool __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name)
{
    bool bTemp=false;
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return bTemp;
    }

    bTemp = INIFile->ValueExists(Group, Name);
    return bTemp;
}
//---------------------------------------------------------------------------
bool __fastcall CheckIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name)   //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    bool bTemp=false;
    AnsiString Str;
    if(OpenIniFileMem(FileName)==false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group, Name);
        RecordProcess(Str);
        return bTemp;
    }

    bTemp=INIFileMem->ValueExists(Group, Name);
    return bTemp;
}
//---------------------------------------------------------------------------
double __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    Str.sprintf("%0.4f", Value);
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteString(Group, Name, Str);                                 //Steven 20150723 : double資料存檔前都補成4個0
    else
        Value=INIFile->ReadFloat(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    if(!INIFile->ValueExists(Group, Name))
        INIFile->ReadInteger(Group, Name, Value);
    else
        Value=INIFile->ReadInteger(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteInteger(Group, Name, Value);
    else
        Value=INIFile->ReadInteger(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteBool(Group, Name, Value);
    else
        Value=INIFile->ReadBool(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    if(!INIFile->ValueExists(Group, Name))
    {
        INIFile->WriteString(Group, Name, Value);
        Str=Value;                                                              //JerryYang 20170711 (Steven) 修正read ini file初始值異常問題
    }
    else
    {
        Str=INIFile->ReadString(Group, Name, Value);
        if(Str=="" && Value!="")                                                //Steven 20160323 : Fixed when value is NULL
        {
            Str=Value;
            INIFile->WriteString(Group, Name, Value);
        }
    }
    return Str;
}
//---------------------------------------------------------------------------
TDateTime __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }

    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteDateTime(Group, Name, Value);
    else
        Value=INIFile->ReadDateTime(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)
{
    bool ret;
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return bValue;
    }
    ret=INIFile->ReadBool(Group, Name, bValue);
    return ret;
}
//---------------------------------------------------------------------------
double __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    double ret;
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFile->ReadFloat(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
int __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    int ret;
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFile->ReadInteger(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString ret;
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFile->ReadString(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)             //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    bool ret;
    AnsiString Str;
    if(OpenIniFileMem(FileName)==false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group, Name);
        RecordProcess(Str);
        return bValue;
    }
    ret=INIFileMem->ReadBool(Group, Name, bValue);
    return ret;
}
//---------------------------------------------------------------------------
double __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)          //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    double ret;
    AnsiString Str;
    if(OpenIniFileMem(FileName)==false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFileMem->ReadFloat(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
int __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)                //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    int ret;
    AnsiString Str;
    if(OpenIniFileMem(FileName)==false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFileMem->ReadInteger(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
{
    AnsiString ret;
    AnsiString Str;
    if(OpenIniFileMem(FileName)==false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group, Name);
        RecordProcess(Str);
        return Value;
    }
    ret=INIFileMem->ReadString(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  //Steven 20090731
{
    AnsiString Str, Str1, Str2;
    bool ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    bool bStr2HasFind=false;                                                    //Ifor 20191021 : 整理Even Log
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    ret=INIFile->ReadBool(Group, Name, bValue);
    if(ret!=bValue && InitialOK==true)                                          //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
    {
        StrChangeName=TempChangeLog(Group,Name);                                //Ifor 20190930 : add Display temperature switch Site Name
        if(FileName.Pos("Offset")==0)                                           //Ifor 20191004 : add Change Log 是否為Offset 資料
        {
            Str1.sprintf("%s_%s change Value",Group , StrChangeName);
        }
        else
        {
            Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
        }

        if(FileName.Pos("HandlerCondition.Data")!=0)
        {
            if(Group=="Configuration")
            {
                bStr2HasFind=true;
                if(Name=="bAutoClean_UseTray")
                {
                    Str2.sprintf("%s ==> %s", fCleaning->rgCleanKitType->Items->Strings[ret], fCleaning->rgCleanKitType->Items->Strings[bValue]);
                }
                else
                {
                    bStr2HasFind=false;
                }
            }
        }

        if(bStr2HasFind==false)
        {
            Str2.sprintf("%d==>%d", ret, bValue);
        }
        RecordChangeLogProcess(Str1.c_str(), Str2.c_str());                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }

    try
    {
        INIFile->WriteBool(Group, Name, bValue);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  //Steven 20090731
{
    AnsiString Str, Str1, Str2;
    int  ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    bool bStr2HasFind=false;                                                    //Ifor 20191021 : 整理Even Log
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    ret=INIFile->ReadInteger(Group, Name, Value);
    if(ret!=Value && InitialOK==true)                                           //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
    {
        if(FileName.Pos("config.ini")>1 && Group=="O_Count" && Name.Pos("O_1")==1)
        {
                                                                                //Ifor 20191017 : Head Contact Count Change 不記錄
        }
        else
        {
            StrChangeName=TempChangeLog(Group,Name);                            //Ifor 20190930 : add Display temperature switch Site Name

            if(FileName.Pos("Offset")==0)                                       //Ifor 20191004 : add Change Log 是否為Offset 資料
            {
                Str1.sprintf("%s_%s change Value",Group , StrChangeName);
            }
            else
            {
                Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
            }

            if(FileName.Pos("Contact.Data")!=0)
            {
                if(Group=="Mode")
                {
                    bStr2HasFind=true;
                    if(Name=="Contact")
                    {
                        Str2.sprintf("%s ==> %s", fContact->cbContactMode->Items->Strings[ret], fContact->cbContactMode->Items->Strings[Value]);
                    }
                    else if(Name=="iSocketInitialICCheckPosition")
                    {
                        Str2.sprintf("%s ==> %s", fContact->coD41->Items->Strings[ret], fContact->coD41->Items->Strings[Value]);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
            }
            else if(FileName.Pos("Tester.Data")!=0)
            {
                if(Group=="Mode")
                {
                    bStr2HasFind=true;
                    if(Name=="Tester Type")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->rgInterfaceType->Items->Strings[ret], FTestIF->rgInterfaceType->Items->Strings[Value]);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
                else if(Group=="GP-IB")
                {
                    bStr2HasFind=true;
                    if(Name=="Type")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->cbGPIBType->Items->Strings[ret], FTestIF->cbGPIBType->Items->Strings[Value]);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
                else if(Group=="DIO")
                {
                    bStr2HasFind=true;
                    if(Name=="Type")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->cbDIOType->Items->Strings[ret], FTestIF->cbDIOType->Items->Strings[Value]);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
                else if(Group=="RS-232C")
                {
                    bStr2HasFind=true;
                    if(Name=="Baud Rate")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->rgBaudRate->Items->Strings[ret], FTestIF->rgBaudRate->Items->Strings[Value]);
                    }
                    else if(Name=="Bit Length")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->rgBitLength->Items->Strings[ret], FTestIF->rgBitLength->Items->Strings[Value]);
                    }
                    else if(Name=="Parity")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->rgParity->Items->Strings[ret], FTestIF->rgParity->Items->Strings[Value]);
                    }
                    else if(Name=="Stop Bit")
                    {
                        Str2.sprintf("%s ==> %s", FTestIF->rgStopBit->Items->Strings[ret], FTestIF->rgStopBit->Items->Strings[Value]);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
            }
            else if(FileName.Pos("HandlerCondition.Data")!=0)
            {
                if(Group=="Configuration")
                {
                    bStr2HasFind=true;
                    if(Name=="iAutoClean_SelectArm")
                    {
                        Str2.sprintf("%s ==> %s", fCleaning->rgAutoCleanSelectArm->Items->Strings[ret], fCleaning->rgAutoCleanSelectArm->Items->Strings[Value]);
                    }
                    else if(Name=="iAutoClean_iPadThickness"        ||
                            Name=="iAutoClean_DropHigh"             ||
                            Name=="iAutoClean_ContactShiftHeight"   ||
                            Name=="iAutoClean_ContactCleanHeight"   ||
                            Name=="iAutoClean_IndexPickOffset"      ||
                            Name=="iAutoClean_IndexReleaseOffset"   ||
                            Name=="iAutoClean_Shuttle1PickOffset"   ||
                            Name=="iAutoClean_Shuttle1PlaceOffset"  ||
                            Name=="iAutoClean_Shuttle1XOffset"      ||
                            Name=="iAutoClean_Shuttle1YOffset"      ||
                            Name=="iAutoClean_Shuttle2PickOffset"   ||
                            Name=="iAutoClean_Shuttle2PlaceOffset"  ||
                            Name=="iAutoClean_Shuttle2XOffset"      ||
                            Name=="iAutoClean_Shuttle2YOffset"      )
                    {
                        Str2.sprintf("%smm ==> %smm", AnsiString(ConvertToMMType(ret)), AnsiString(ConvertToMMType(Value)));
                    }
                    else if(Name=="iAutoClean_ContactMode")
                    {
                        Str2.sprintf("%s ==> %s", fCleaning->ContactMode->Items->Strings[ret], fCleaning->ContactMode->Items->Strings[Value]);
                    }
                    else if(Name=="iAutoClean_ContactTime")
                    {
                        Str2.sprintf("%0.1fs ==> %0.1fs", (double)ret/10, (double)Value/10);
                    }
                    else
                    {
                        bStr2HasFind=false;
                    }
                }
            }

            if(bStr2HasFind==false)
            {
                Str2.sprintf("%d==>%d", ret, Value);
            }
            RecordChangeLogProcess(Str1.c_str(), Str2.c_str());                 //wei 20180625 offset Change log紀錄
            bHasChange=true;
        }
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteInteger(Group, Name, Value);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  //Steven 20090731
{
    AnsiString Str, Str1, Str2;
    double ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    bool bStr2HasFind=false;                                                    //Ifor 20191021 : 整理Even Log
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }
    Str.sprintf("%0.4f", Value);                                                //Steven 20150723 : double資料存檔前都補成4個0

    ret=INIFile->ReadFloat(Group, Name, Value);
    if(ret!=Str && InitialOK==true)                                             //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
    {
        StrChangeName=TempChangeLog(Group,Name);                                //Ifor 20190930 : add Display temperature switch Site Name
        if(FileName.Pos("Offset")==0)                                           //Ifor 20191004 : add Change Log 是否為Offset 資料
        {
            Str1.sprintf("%s_%s change Value",Group , StrChangeName);
        }
        else
        {
            Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
        }

        if(StrChangeName=="Contact" &&
           (Group=="Test Arm1" || Group=="Test Arm2"))                          //kevin 20210519 Add : For ASE 高雄 扭力值存Log 修改 INDEX OFFSET
        {
            if(TestIF_File.bEnableReadAndCheckTorque)                           //kevin 20210804
            {
                bResetArm1Value=true;                                           //修改index contact offset 參數 重新設定扭力標準值
                bResetArm2Value=true;
            }
        }

        if(FileName.Pos("Contact.Data")!=0)                                     //Ifor 20191015 : add Kit Diameter switch Name
        {
            if(Group=="Mode")
            {
                bStr2HasFind=true;
                if(Name=="Kit Diameter")
                {
                    Str2.sprintf("%0.1fmm ==>%0.1fmm", ret*10, Value*10);
                }
                else
                {
                    bStr2HasFind=false;
                }
            }
        }
        else if(FileName.Pos("HandlerCondition.Data")!=0)
        {
            if(Group=="Configuration")
            {
                bStr2HasFind=true;
                if(Name=="ShuttlePitchOffset"       ||
                   Name=="dAutoClean_XStart_Kit"    ||
                   Name=="dAutoClean_XPitch_Kit"    ||
                   Name=="dAutoClean_YStart_Kit"    ||
                   Name=="dAutoClean_YPitch_Kit"    )
                {
                    Str2.sprintf("%smm ==> %smm", AnsiString(ConvertToMMType(ret)), AnsiString(ConvertToMMType(Value)));
                }
                else
                {
                    bStr2HasFind=false;
                }
            }
        }

        if(bStr2HasFind==false)
        {
            Str2.sprintf("%0.4f==>%0.4f", ret, Value);
        }
        RecordChangeLogProcess(Str1.c_str(), Str2.c_str());                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteString(Group, Name, Str);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)
{
    AnsiString Str, Str1, Str2;
    unsigned long ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    ret=INIFile->ReadInteger(Group, Name, Value);
    if(ret!=Value && InitialOK==true)                                           //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
    {
        StrChangeName=TempChangeLog(Group, Name);                               //Ifor 20190930 : add Display temperature switch Site Name
        if(FileName.Pos("Offset")==0)                                           //Ifor 20191004 : add Change Log 是否為Offset 資料
        {
            Str1.sprintf("%s_%s change Value",Group , StrChangeName);
        }
        else
        {
            Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
        }
        Str2.sprintf("%d==>%d", ret, Value);
        RecordChangeLogProcess(Str1.c_str(), Str2.c_str());                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteInteger(Group, Name, Value);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value) //Steven 20090731
{
    AnsiString Str, Str1, Str2;
    AnsiString ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    bool bStrIsFloat1=false;
    bool bStrIsFloat2=false;
    double freg=0;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    ret=INIFile->ReadString(Group, Name, Value);
//    if(atof(ret.c_str())!=atof(Value.c_str()) && InitialOK==true)             //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
//    {
//        StrChangeName=TempChangeLog(Group,Name);                              //Ifor 20190930 : add Display temperature switch Site Name
//        if(FileName.Pos("Offset")==0)                                         //Ifor 20191004 : add Change Log 是否為Offset 資料
//        {
//            Str1.sprintf("%s_%s change Value",Group , StrChangeName);
//        }
//        else
//        {
//            Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
//        }
//        Str2.sprintf("%s==>%s", ret, Value);
//        RecordChangeLogProcess(Str1.c_str(), Str2.c_str());                   //wei 20180625 offset Change log紀錄
//        bHasChange=true;
//    }
    bStrIsFloat1=TryStrToFloat(ret.c_str(), freg);
    bStrIsFloat2=TryStrToFloat(Value.c_str(), freg);
    if(InitialOK==true)                                                         //Ifor 20190919 : add 避免程式開啟時因客戶要求強制開啟功能寫入時發生異常
    {
        if((bStrIsFloat1==true && bStrIsFloat2==true && atof(ret.c_str())!=atof(Value.c_str())) ||
           (bStrIsFloat1==false && bStrIsFloat2==false && ret!=Value))
        {
            StrChangeName=TempChangeLog(Group,Name);                            //Ifor 20190930 : add Display temperature switch Site Name
            if(FileName.Pos("Offset")==0)                                       //Ifor 20191004 : add Change Log 是否為Offset 資料
            {
                Str1.sprintf("%s_%s change Value",Group , StrChangeName);
            }
            else
            {
                Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
            }

            if(FileName.Pos("Contact.Data")!=0 && Group=="Mode" && Name=="fSocketInitialICCheckPositionOffset")
            {
                Str2.sprintf("%0.2fmm ==> %0.2fmm", atof(ret.c_str()), atof(Value.c_str()));
            }
            else
            {
                Str2.sprintf("%s==>%s", ret, Value);
            }
            RecordChangeLogProcess(Str1, Str2);                                 //wei 20180625 offset Change log紀錄
            bHasChange=true;
        }
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteString(Group, Name, Value);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    AnsiString Str, Str1;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteDateTime(Group, Name, Value);
    }
    catch(...)
    {
        Str1.sprintf("WriteIniData:%s", FileName);
        RecordProcess("Exception", Str1);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)    //Steven 20200526 : 新增不加入event log的ini存檔
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteInteger(Group, Name, Value);
    }
    catch(...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName);
        RecordProcess("Exception", Str);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }

    try                                                                         //JerryYang 20230204 : add 例外處理
    {
        INIFile->WriteString(Group, Name, Value);
    }
    catch(...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName);
        RecordProcess("Exception", Str);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group, Name);
        RecordProcess(Str);
        return;
    }
    Str.sprintf("%0.4f", Value);

    try
    {
        INIFile->WriteString(Group, Name, Str);
    }
    catch(...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName);
        RecordProcess("Exception", Str);
    }
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData1(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value) //kevin 20220623 add write
{
    AnsiString Str, Str1, Str2;
    AnsiString ret;
    AnsiString StrChangeName="";                                                //Ifor 20190930 : add Display temperature switch Site Name
    bool bStrIsFloat1=false;
    bool bStrIsFloat2=false;
    double freg=0;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        return;
    }

    ret=INIFile->ReadString(Group, Name, Value);
    bStrIsFloat1=TryStrToFloat(ret.c_str(), freg);
    bStrIsFloat2=TryStrToFloat(Value.c_str(), freg);
    if(InitialOK==true)
    {
        if((bStrIsFloat1==true && bStrIsFloat2==true && atof(ret.c_str())!=atof(Value.c_str())) ||
           (bStrIsFloat1==false && bStrIsFloat2==false && ret!=Value))
        {
            StrChangeName=TempChangeLog(Group,Name);                            //Ifor 20190930 : add Display temperature switch Site Name
            if(FileName.Pos("Offset")==0)                                       //Ifor 20191004 : add Change Log 是否為Offset 資料
            {
                Str1.sprintf("%s_%s change Value",Group , StrChangeName);
            }
            else
            {
                Str1.sprintf("%s_%s Offset change Value",Group , StrChangeName);
            }

            if(FileName.Pos("Contact.Data")!=0 && Group=="Mode" && Name=="fSocketInitialICCheckPositionOffset")
            {
                Str2.sprintf("%0.2fmm ==> %0.2fmm", atof(ret.c_str()), atof(Value.c_str()));
            }
            else
            {
                Str2.sprintf("%s==>%s", ret, Value);
            }
        }
    }

    try
    {
        INIFile->WriteString(Group, Name, Value);
    }
    catch(...)
    {
        Str.sprintf("WriteIniData1:%s", FileName);
        RecordProcess("Exception", Str);
    }
}
//---------------------------------------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2) //Steven 20090731
{
    AnsiString Str;
    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        Str.sprintf("Replace NULL INI on [%s] %s", Group, Name1);
        RecordProcess(Str);
        return;
    }
    AnsiString  ret=INIFile->ReadString(Group, Name2, " ");
    INIFile->WriteString(Group, Name1, ret);
}
//---------------------------------------------------------------------------
AnsiString __fastcall GetLastOpenFN()
{
    AnsiString Str="Fail Open";
    TStringList *MyList= new TStringList();                                     //Steven 20141120 : delete MyList

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        if(MyList->Count==0)                                                    //Jimmychiu 20250426 : fixed for the content of setup is empty
        {
            ShowMyMessage("The content of setup.inf is empty!");
        }
        else if(MyList->Strings[0].Trim()=="")
        {
            ShowMyMessage("The content of setup.inf is NULL!");
        }
        else
        {
            Str=MyList->Strings[0];
        }
    }
    else
    {
        ShowMyMessage("LastData does not exist");
    }
    MyList->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete MyList;

    return Str;
}
//---------------------------------------------------------------------------
bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group)
{
    if(Group=="" || FileName=="")                                               //Steven 20160606 : Add protection for inifile
        return false;

    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        return false;
    }

    bool  ret=INIFile->SectionExists(Group);
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall CheckKeyExist(AnsiString FileName, AnsiString Group, AnsiString Name)
{
    if(Group=="" || FileName=="")                                               //Steven 20160606 : Add protection for inifile
        return false;

    if(OpenIniFile(FileName)==false)                                            //Steven 20141120 : Add Read/Write IniFile Speed
    {
        return false;
    }

    bool  ret=INIFile->ValueExists(Group, Name);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall WriteLastDataFN(AnsiString SName)
{
    int iFileHandle;
    TStringList *MyList= new TStringList();                                     //Steven 20141120 : delete MyList

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        MyList->Clear();
        MyList->Add(SName);
        MyList->SaveToFile(LastDataPath);
    }
    else
    {
        iFileHandle=FileCreate(LastDataPath);
        FileWrite(iFileHandle, SName.c_str(), SName.Length());
        FileClose(iFileHandle);
    }
    MyList->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete MyList;
}
//---------------------------------------------------------------------------
void __fastcall AddSpace(AnsiString File)
{
    return;
}
//---------------------------------------------------------------------------
bool __fastcall OnlyNumberInPut(int iKey)                                       //only for Number
{
    if((iKey>='0' && iKey<='9') || iKey==8 || iKey=='-')                        //iKey==8 back space
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall OnlyNumberAndDotInPut(int iKey)                                 //only for Number and char '.'
{
    if((iKey>='0' && iKey<='9') || iKey=='.' || iKey==8|| iKey=='-')            //iKey==8 back space
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall OnlyMakeFileDataInPut(int iKey)
{
    if(iKey=='\'' ||  iKey=='/' || iKey==':'  || iKey=='*' ||
       iKey=='?'  ||  iKey=='"' ||  iKey=='<' || iKey=='|')
            return false;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str)
{
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor)
{
    pCanvas->Brush->Color=BrushColor;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor)
{
    pCanvas->Brush->Color=BrushColor;
    pCanvas->Font->Color =FontColor;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int str)
{
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, AnsiString(str).c_str(), -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom)
{
    Rect.Left=left;
    Rect.Top=top;
    Rect.Right=right;
    Rect.Bottom=bottom;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom)
{
    Rect.Left=left;
    Rect.Top=top;
    Rect.Right=right;
    Rect.Bottom=bottom;
    pCanvas->FillRect(Rect);
}
//---------------------------------------------------------------------------
//Steven 20110711 : 把General.ini的存取改成獨立的
//---------------------------------------------------------------------------
void OpenGeneralIniFile()                                                       //Steven 20141120 : Add Read/Write IniFile Speed
{
    CloseGeneralIniFile();
    INIFileGeneral=new TIniFile(asGeneralPath);
}
//---------------------------------------------------------------------------
void CloseGeneralIniFile()                                                      //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(INIFileGeneral!=NULL)
    {
        INIFileGeneral->UpdateFile();
        delete INIFileGeneral;
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteString(Group, Name, Value);
    else
        Value=INIFileGeneral->ReadString(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
double __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value)
{
    AnsiString Str;
    Str.sprintf("%0.4f", Value);

    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteString(Group, Name, Str);                          //Steven 20150723 : double資料存檔前都補成4個0
    else
        Value=INIFileGeneral->ReadFloat(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
    {
        INIFileGeneral->WriteInteger(Group, Name, Value);
    }
    else
    {
        Value=INIFileGeneral->ReadInteger(Group, Name, Value);
    }
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteBool(Group, Name, Value);
    else
        Value=INIFileGeneral->ReadBool(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue)     //Steven 20090731
{
    INIFileGeneral->WriteBool(Group, Name, bValue);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value)       //Steven 20090731
{
    INIFileGeneral->WriteInteger(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value)    //Steven 20090731
{
    AnsiString Str;
    Str.sprintf("%0.4f", Value);                                                //Steven 20150723 : double資料存檔前都補成4個0

    INIFileGeneral->WriteString(Group, Name, Str);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value)//Steven 20090731
{
    INIFileGeneral->WriteString(Group, Name, Value);
}
//---------------------------------------------------------------------------
bool __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value, bool DefaultValue, bool bIsRead)
{
    bool tValue=Value;

    if(bIsRead)
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        tValue=Value;
    }
    else
    {
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
AnsiString __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value, AnsiString DefaultValue, bool bIsRead)
{
    AnsiString tValue=Value;
    if(bIsRead)
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        tValue=Value;
    }
    else
    {
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
TDateTime __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value, TDateTime DefaultValue, bool bIsRead, bool bCheckRange, TDateTime Maximum, TDateTime Minimum)
{
    TDateTime tValue=Value;
    if(bIsRead)
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        else
            tValue=Value;
    }
    else
    {
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
int __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, int Value, int DefaultValue, bool bIsRead, bool bCheckRange, int Maximum, int Minimum)
{
    int tValue=Value;

    if(bIsRead)                                                                 //Steven 20160906 : rewrite range check
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        else
            tValue=Value;
    }
    else
    {
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
double __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, double Value, double DefaultValue, bool bIsRead, bool bCheckRange, double Maximum, double Minimum)
{
    double tValue=Value;
    if(bCheckRange)
        Value=CheckRange(Value, Maximum, Minimum);

    if(bIsRead)
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        else
            tValue=Value;
    }
    else
    {
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
unsigned long __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value, unsigned long DefaultValue, bool bIsRead, bool bCheckRange, unsigned long Maximum, unsigned long Minimum)
{
    unsigned long tValue=Value;
    if(bCheckRange)
        Value=CheckRange(Value, Maximum, Minimum);

    if(bIsRead)
    {
        Value=CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        else
            tValue=Value;
    }
    else
    {
        if(bCheckRange)
            tValue=CheckRange(Value, Maximum, Minimum);
        WriteIniData(FileName, Group, Name, tValue);
    }
    return tValue;
}
//---------------------------------------------------------------------------
void WriteDataToFile(char* cFilePath, char* cData, bool bOverWrite)
{
    FILE *pFile;
    if(bOverWrite)
    {
        pFile=fopen(cFilePath, "w");
    }
    else
    {
        pFile=fopen(cFilePath, "a");
    }

    if(pFile!=NULL)
    {
        fputs(cData, pFile);
        fputs("\n", pFile);
        fclose(pFile);
    }
}
//---------------------------------------------------------------------------
bool CheckFileIsEmpty(AnsiString cFilePath)                                     // 檢查檔案是否為空
{
    bool bResult=true;
    FILE *pFile;

    pFile=fopen(cFilePath.c_str(), "r");
    if(pFile!=NULL)
    {
        int first_char=fgetc(pFile);
        if(first_char==EOF)
        {
            bResult=false;
        }
    }
    fclose(pFile);
    return bResult;
}
//---------------------------------------------------------------------------
void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite)
{
    WriteDataToFile(cFilePath.c_str(), cData.c_str(), bOverWrite);
}
//---------------------------------------------------------------------------
char* ReadDataFromFile(AnsiString cFilePath)
{
    FILE* pFile=fopen(cFilePath.c_str(), "r");
    if(pFile!=NULL)
    {
        fseek(pFile, 0, SEEK_END);
        long file_size=ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        char* file_buf=(char*)malloc(file_size + 1);
        fread(file_buf, file_size, 1, pFile);
        file_buf[file_size]='\0';
        fclose(pFile);
        return file_buf;
    }
    return NULL;
}
//---------------------------------------------------------------------------
int MyForceDirectories(AnsiString Directory, AnsiString Function)               //Steven 20210112 : 針對資料夾加上保護
{
    AnsiString Str;
    if(Directory=="")
    {
        ShowMyMessage("Directory value is NULL!", Function, Function);
        return -1;
    }
    else
    {
        try
        {
            if(DirectoryExists(Directory)==false)
            {
                ForceDirectories(Directory);
            }
        }
        catch(Exception& e)                                                     //Steven 20140505 : 試著抓出連線異常的訊息
        {
            Str.sprintf("%s -- %s", Directory, Function);
            ShowMyMessage(e.Message, Str, Str);
            return -2;
        }
        catch(...)
        {
            Str.sprintf("%s -- %s", Directory, Function);
            ShowMyMessage("Create directory fail!", Str, Str);
            return -3;
        }
    }

    return 1;
}
//---------------------------------------------------------------------------
//Steven 20190918 : 避免Compile異常, 把STD Function包起來
//==>
//---------------------------------------------------------------------------
DWORD MyTickCount()
{
    return GetTickCount();
}
//---------------------------------------------------------------------------
int iSleepCount=0;
bool bRunTimer=false;
bool bAlreadySleep=false;
TQPF_Timer MySpeelTimer;
DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable)                          //Steven 20200807 : 趁著Sleep的時候去檢查EC change report
{
    #ifdef USE_EC_CHANGE
    int iFlag;
    bool bStopWhile=false;
    static int iInputCnt=0;
    int iLeftTime;

    if(bAlreadySleep==false)                                                    //避免多執行緒進來
    {
        bAlreadySleep=true;
    }
    else
    {
        return SleepEx(dwMilliseconds, bAlertable);
    }

    if(bRunTimer==false)
    {
        MySpeelTimer.SetMSAndOn(dwMilliseconds);
        MySpeelTimer.LatchCycleTime(true);
        bRunTimer=true;
    }

    iInputCnt=iSleepCount;
    do
    {
        iFlag=HGem->DoReportECChange(iSleepCount);
        if(iFlag==1)
        {
            iSleepCount++;
            if(iSleepCount>=HGem->EC_ID->Count)
                iSleepCount=0;
        }

        if(iSleepCount==iInputCnt || iFlag!=1)                                  //假設已經繞了一圈, 或者不需要執行EC check, 就停止while loop
        {
            bStopWhile=true;
            break;
        }
    }
    while(MySpeelTimer.Off()==false && bStopWhile==false);

    if(bStopWhile==true)
    {
        iLeftTime=dwMilliseconds-MySpeelTimer.LatchCycleTime();

        if(iLeftTime>0)
            SleepEx(iLeftTime, bAlertable);
    }
    bRunTimer=false;
    bAlreadySleep=false;
    return 1;
    #else
    return SleepEx(dwMilliseconds, bAlertable);
    #endif
}
//---------------------------------------------------------------------------
void MySleep(DWORD dwMilliseconds)
{
    ::Sleep(dwMilliseconds);
}
//---------------------------------------------------------------------------
//<==
//Steven 20190918 : 避免Compile異常, 把STD Function包起來
//---------------------------------------------------------------------------
//extern AnsiString asTempCtrl[tcTotalCount];
AnsiString __fastcall TempChangeLog(AnsiString Group, AnsiString Name)          //Ifor 20190930 : add Display temperature switch Site Name
{
    int iSiteAdd=0;
    AnsiString asATCTempName[32]=   {"Aa1", "Ab1", "Ac1", "Ad1",
                                     "Ae1", "Af1", "Ag1", "Ah1",
                                     "Ba1", "Bb1", "Bc1", "Bd1",
                                     "Be1", "Bf1", "Bg1", "Bh1",
                                     "Aa2", "Ab2", "Ac2", "Ad2",
                                     "Ae2", "Af2", "Ag2", "Ah2",
                                     "Ba2", "Bb2", "Bc2", "Bd2",
                                     "Be2", "Bf2", "Bg2", "Bh2"
                                    };

    AnsiString asIniDelayName[10]=  {"Every first devices",
                                     "After ShowAlarm Message",
                                     "After Auto Clean Function",
                                     "When happen tested time below",
                                     "After Open Heat Door",
                                     "When Press Stop Over",
                                     "When No Full Site",
                                     "EOT monitor time",
                                     "OTD unlock",
                                     "SOT monitor time"
                                    };

    AnsiString asAutoCleanSpeed[4]= {"Auto Clean Input Arm Speed",
                                     "Auto Clean Shuttle Speed",
                                     "Auto Clean Index Arm Speed",
                                     "Auto Clean Input Arm Z Speed"
                                    };

    if(Group.Pos("AmbientHotLowOffSet")     ==1     ||                          //Ifor 20190930 : add Temp Change Site
       Group.Pos("AmbientHotMidOffSet")     ==1     ||
       Group.Pos("Low OffSet")              ==1     ||
       Group.Pos("Mid. OffSet")             ==1     ||
       Group.Pos("High OffSet")             ==1     ||
       Group.Pos("User OffSet")             ==1     ||
       Group.Pos("SingleTempLimit")         ==1     ||
       Group.Pos("Init Temp OffSet")        ==1     ||
       Group.Pos("TestOverTime Temp OffSet")==1     )
    {
        if(Name.Pos("CH")==1)                                                   //Ifor 20191015 : Fix 溫度校正曲線中非CH參數會發生異常導致無法寫入檔案
        {
            iSiteAdd=atoi(Name.SubString(3,Name.Length()-2).c_str());
            if(Group.Pos("SingleTempLimit")==1)                                 //Ifor 20190930 : add Single Temp Limit 不需-1
            {
                Name=asTempCtrl[iSiteAdd];
            }
            else
            {
                Name=asTempCtrl[iSiteAdd-1];
            }
        }
    }
    else if(Group.Pos("ATC")==1 && Name.Pos("ATCTempOffset")==1)
    {
        iSiteAdd=atoi(Name.SubString(15,Name.Length()-15).c_str());

        if(TestIF_File.iTestMode==_8Site2X4 && TestIF_File.bOctal_16Kit==true)  //JerryYang 20230828 : fix change log
        {
            if(iSiteAdd>=2)                                                     //避免小於0
            {
                iSiteAdd-=2;
            }
        }

        Name="OffSet_"+asATCTempName[iSiteAdd];
    }
    else if(Group.Pos("InitialMode")==1)
    {
        if(Name.Pos("iInitialDelay")==1)
        {
            if(Name.Pos("iInitialDelay_")==1)
            {
                iSiteAdd=atoi(Name.SubString(15,Name.Length()-14).c_str())-1;
            }
            else
            {
                iSiteAdd=0;
            }
            Name=asIniDelayName[iSiteAdd];
        }
        else if(Name.Pos("dInitialDelay_10")==1)
        {
            iSiteAdd=atoi(Name.SubString(15,Name.Length()-14).c_str())-1;
            Name=asIniDelayName[iSiteAdd];
        }
        else if(Name=="bEveryFirstDeviceUseInitialDelay")
        {
            Name="bEvery First Device Use Initial Delay";
        }
        else if(Name=="bAfterShowAlarmMessageUseInitialDelay")
        {
            Name="bAfter Show Alarm Message Use Initial Delay";
        }
        else if(Name=="bWhenHappenTestedTimeBelowUseInitialDelay")
        {
            Name="bWhen Happen Tested Time Below Use Initial Delay";
        }
        else if(Name=="bAfterAutoCleanFunctionUseInitialDelay")
        {
            Name="bAfter Auto Clean Function Use Initial Delay";
        }
        else if(Name=="bAfterOpenHeatDoorUseInitialDelay")
        {
            Name="bAfter Open Heat Door Use Initial Delay";
        }
        else if(Name=="bWhenPressStopOverUseInitialDelay")
        {
            Name="bWhen Press Stop Over Use Initial Delay";
        }
        else if(Name=="bTestFinishToNextTestOver")
        {
            Name="bEOT monitor time Use Initial Delay";
        }
        else if(Name=="bTestStartToNextTestStart")
        {
            Name="bSOT monitor time Use Initial Delay";
        }
        else if(Name=="bOTDUnlockDelay")
        {
            Name="bOTD Unlock Use Initial Delay";
        }
        else if(Name=="iEveryFirstDeviceUseInitialDelay")
        {
            Name="When happen tested time below Trigger time (Sec)";
        }
        else if(Name=="iWhenPressStopOver")
        {
            Name="When Press Stop Over Trigger time (Sec)";
        }
        else if(Name=="iTestFinishToNextTestOver")
        {
            Name="EOT monitor time Trigger time (Sec)";
        }
        else if(Name=="dTeststartToNextTestStart")
        {
            Name="SOT monitor time Trigger time (Sec)";
        }
//        else if(Name=="")
//        {
//            Name="";
//        }
    }
    else if(Group.Pos("Time")==1)
    {
        if(Name=="Stary Delay")
        {
            Name="Start Delay";
        }
        else if(Name=="Initial Stary Delay")
        {
            Name="Initial Start Delay";
        }
        else if(Name=="Initial Stary Delay CT")
        {
            Name="Initial Start Delay Count";
        }
    }
    else if(Group.Pos("Mode")==1)
    {
        if(Name=="fSocketInitialICCheckPositionOffset")
        {
            Name="fSocket Initial IC Check Position Offset";
        }
        else if(Name=="iSocketInitialICCheckPosition")
        {
            Name="iSocket Initial IC Check Position";
        }
    }
    else if(Group.Pos("Configuration")==1)
    {
        if(Name.Pos("iAutoClean_MotorSpeed[")==1)
        {
            iSiteAdd=atoi(Name.SubString(23,1).c_str());
            Name=asAutoCleanSpeed[iSiteAdd];
        }
        else if(Name=="iAutoClean_AlarmCount")
        {
            Name="Auto Clean Alarm Count";
        }
        else if(Name=="iAutoClean_iPadThickness")
        {
            Name="Auto Clean Clean Pad Deviation";
        }
        else if(Name=="iAutoClean_ContactCleanHeight")
        {
            Name="Auto Clean Socket Position Offset";
        }
        else if(Name=="iAutoClean_IndexPickOffset")
        {
            Name="Auto Clean Index to Shuttle Pick Offset";
        }
        else if(Name=="iAutoClean_IndexReleaseOffset")
        {
            Name="Auto Clean Index to Shuttle Release Offset";
        }
        else if(Name=="iAutoClean_Shuttle1PickOffset")
        {
            Name="Auto Clean In Arm to Shuttle1 Pick Offset";
        }
        else if(Name=="iAutoClean_Shuttle1PlaceOffset")
        {
            Name="Auto Clean In Arm to Shuttle1 Place Offset";
        }
        else if(Name=="iAutoClean_Shuttle1XOffset")
        {
            Name="Auto Clean In Arm to Shuttle1 X Offset";
        }
        else if(Name=="iAutoClean_Shuttle1YOffset")
        {
            Name="Auto Clean In Arm to Shuttle1 Y Offset";
        }
        else if(Name=="iAutoClean_Shuttle2PickOffset")
        {
            Name="Auto Clean In Arm to Shuttle2 Pick Offset";
        }
        else if(Name=="iAutoClean_Shuttle2PlaceOffset")
        {
            Name="Auto Clean In Arm to Shuttle2 Place Offset";
        }
        else if(Name=="iAutoClean_Shuttle2XOffset")
        {
            Name="Auto Clean In Arm to Shuttle2 X Offset";
        }
        else if(Name=="iAutoClean_Shuttle2YOffset")
        {
            Name="Auto Clean In Arm to Shuttle2 Y Offset";
        }
        else if(Name=="ShuttlePitchOffset")
        {
            Name="Auto Clean In Arm Shuttle Pitch Offset";
        }
    }
    return Name;
}
//---------------------------------------------------------------------------
AnsiString GetRecipePath()                                                      //JimmyChiu 20220601 : config儲存跟隨recipe
{
    return AnsiString().sprintf("%s%s\\", DataPath, GetLastOpenFN());
}
//---------------------------------------------------------------------------
AnsiString GetRecipeFileName(AnsiString FileName)                               //Steven 20241030 : 取得檔案的完整路徑與名稱
{
    return FileInfo().PathCombin(AnsiString().sprintf("%s%s", DataPath, GetLastOpenFN()), FileName);
//    return AnsiString().sprintf("%s%s\\%s", DataPath, GetLastOpenFN(), FileName);
}
//---------------------------------------------------------------------------
void SGDToCSV(TStringGrid *strGrid, AnsiString sSplit, AnsiString sReplaceSplit, AnsiString Path)        //Sam 20231106 : 新增 SGD 轉 CSV
{
    AnsiString sTemp="", sStr="";

    for(int iRow=0; iRow<strGrid->RowCount;iRow++)
    {
        sTemp="";
        for(int iCol=0; iCol<strGrid->ColCount;iCol++)
        {
            sStr=StringReplace(strGrid->Cells[iCol][iRow], sSplit, sReplaceSplit, TReplaceFlags());     //Sam 20240104 : 文字裡若有 CSV 分隔號會導致資料分割錯誤需要用其他分隔號來取代
            sTemp+=sStr+sSplit;
        }
         WriteDataToFile(Path, sTemp);
    }
}
//---------------------------------------------------------------------------
bool IsFileInUse(const char* filePath)                                          //Jimmychiu 20241121 : Verify whether the file is currently in use
{
    HANDLE hFile=CreateFile(
        filePath,                // 文件路徑
        GENERIC_READ,            // 讀取權限
        FILE_SHARE_READ,         // 共享讀取模式
        NULL,                    // 安全屬性
        OPEN_EXISTING,           // 僅在文件存在時打開
        FILE_ATTRIBUTE_NORMAL,   // 文件屬性
        NULL                     // 模板文件句柄
    );

    if(hFile==INVALID_HANDLE_VALUE)
    {
        DWORD error=GetLastError();
        if(error==ERROR_SHARING_VIOLATION || error==ERROR_LOCK_VIOLATION)
        {
            return true;
        }
    } else
    {
        CloseHandle(hFile);
    }
    return false;
}
//---------------------------------------------------------------------------
bool CopyAndCompressFile(const AnsiString &sourcePath, const AnsiString &targetPath, const AnsiString &sSourFileName, const AnsiString &sTarFileName, const int &iDelayMS, AnsiString &sMsg)
{
    AnsiString sSourceFilePath=FileInfo().PathCombin(sourcePath, sSourFileName);
    AnsiString sTargetFilePath=FileInfo().PathCombin(targetPath, sTarFileName);
    if(FileExists(sSourceFilePath))
    {
        if(FileExists("d:\\HT9045\\7z.exe")==false)                             //把7Zip準備好
        {
            CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , false); //Ifor 20170321 FALSE => false
        }
        AnsiString Str=AnsiString().sprintf("a -tzip \"%s\" \"%s\"", sTargetFilePath, sSourceFilePath);
        ShellExecute(NULL, "open", "d:\\HT9045\\7z.exe", Str.c_str(), NULL, SW_HIDE);
        MySleep(iDelayMS);
        if(FileExists(sTargetFilePath))
        {
            sMsg=AnsiString().sprintf("Copy file from %s to %s .", sSourceFilePath, sTargetFilePath);
            return true;
        }
        else
        {
            sMsg=AnsiString().sprintf("Failed to copy file from %s to %s .", sSourceFilePath, sTargetFilePath);
        }
    }
    else
    {
        sMsg=AnsiString().sprintf("File %s does not exists!", sSourceFilePath);
    }
    return false;
}
//---------------------------------------------------------------------------
//ben 20240927 : add CCD Bar Code to ATC
//==>
AnsiString __fastcall ChangeSaveFileName(AnsiString asFileName)
{
    AnsiString Str1;

    if(asFileName.Pos("\\")!=0)
    {
        Str1=StringReplace(asFileName, "\\", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("/")!=0)
    {
        Str1=StringReplace(asFileName, "/", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos(":")!=0)
    {
        Str1=StringReplace(asFileName, ":", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("*")!=0)
    {
        Str1=StringReplace(asFileName, "*", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("?")!=0)
    {
        Str1=StringReplace(asFileName, "?", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("""")!=0)
    {
        Str1=StringReplace(asFileName, """", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("<")!=0)
    {
        Str1=StringReplace(asFileName, "<", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos(">")!=0)
    {
        Str1=StringReplace(asFileName, ">", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    if(asFileName.Pos("|")!=0)
    {
        Str1=StringReplace(asFileName, "|", "_", TReplaceFlags()<<rfReplaceAll);
        asFileName=Str1;
    }

    return asFileName;
}
//---------------------------------------------------------------------------
//<==
//ben 20240927 : add CCD Bar Code to ATC
//---------------------------------------------------------------------------
int __fastcall MySecondsBetween(TDateTime DT1, TDateTime DT2)                   //Sam 20251031 : Add 時間差計算 DT2-DT1
{
    int iTempSec=0;
    Word year, month, day;
    Word hour, min, sec, msec;
    TDateTime TempDT;
    TempDT=DT2-DT1;
    DecodeDate(TempDT, year, month, day);
    DecodeTime(TempDT, hour, min, sec, msec);
    iTempSec=day*24*60*60+hour*60*60+min*60+sec;
    return iTempSec;

}
//---------------------------------------------------------------------------
