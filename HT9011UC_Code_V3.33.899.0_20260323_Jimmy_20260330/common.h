//---------------------------------------------------------------------------

#ifndef commonH
#define commonH

#include <Classes.hpp>
#include <Controls.hpp>
#include <Buttons.hpp>
//#include <vector>
#include "MachineType.h"
//---------------------------------------------------------------------------
void InitCommonString();                                                        //jou 2016-08-24 Initial Common String
//---------------------------------------------------------------------------
AnsiString EncodeStr(AnsiString sourceStr);                                     //jou 2013-01-04 Password Txt 加密
AnsiString DecodeStr(AnsiString sourceStr);                                     //jou 2013-01-04 Password Txt 加密
//---------------------------------------------------------------------------
bool        __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name);
bool        __fastcall CheckIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name);  //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
//---------------------------------------------------------------------------
//Steven 20110711 : 把General.ini的存取改成獨立的
//---------------------------------------------------------------------------
void OpenGeneralIniFile();                                                      //Steven 20141120 : Add Read/Write IniFile Speed
void CloseGeneralIniFile();                                                     //Steven 20141120 : Add Read/Write IniFile Speed

AnsiString __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);
int        __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
bool       __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value);
double     __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);

void __fastcall WriteIniData1(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value); //kevin 20220623 add write
//---------------------------------------------------------------------------
//Steven 20090810 : 先檢查有沒有資料,沒有就會存入ini檔並回傳取得的資料
//---------------------------------------------------------------------------
bool OpenIniFile(AnsiString FileName);                                          //Steven 20141120 : Add Read/Write IniFile Speed
void CloseIniFile();                                                            //Steven 20141120 : Add Read/Write IniFile Speed
bool OpenIniFileMem(AnsiString FileName);                                       //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
void CloseIniFileMem();                                                         //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制

TDateTime   __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);
AnsiString  __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
int         __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
bool        __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value);
double      __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);
//---------------------------------------------------------------------------

AnsiString      __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value, AnsiString DefaultValue, bool bIsRead);
bool            __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value, bool DefaultValue, bool bIsRead);
TDateTime       __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value, TDateTime DefaultValue, bool bIsRead, bool bCheckRange=false, TDateTime Maximum=NULL, TDateTime Minimum=NULL);
int             __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, int Value, int DefaultValue, bool bIsRead, bool bCheckRange=false, int Maximum=NULL, int Minimum=NULL);
double          __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, double Value, double DefaultValue, bool bIsRead, bool bCheckRange=false, double Maximum=NULL, double Minimum=NULL);
unsigned long   __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value, unsigned long DefaultValue, bool bIsRead, bool bCheckRange=false, unsigned long Maximum=NULL, unsigned long Minimum=NULL);
//---------------------------------------------------------------------------
//讀取ini資料,若無附值則以傳入值回傳
//---------------------------------------------------------------------------
bool        __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);
int         __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
double      __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
AnsiString  __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
bool        __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);        //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
int         __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);          //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
double      __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);       //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
AnsiString  __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);   //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
AnsiString  __fastcall GetLastOpenFN();
AnsiString  __fastcall ChangeSaveFileName(AnsiString asFileName);               //ben 20240927 : add CCD Bar Code to ATC
//---------------------------------------------------------------------------
//檢查該群組是否存在
//---------------------------------------------------------------------------
bool        __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group);
bool        __fastcall CheckKeyExist(AnsiString FileName, AnsiString Group, AnsiString Name);

void __fastcall WriteLastDataFN(AnsiString SName);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);  //Steven 20090731
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
//---------------------------------------------------------------------------
//Steven 20090803
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int i);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom);

//---------------------------------------------------------------------------
//Steven 20090811 : 用Name2的值去取代Name1的值
//---------------------------------------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2);

void __fastcall AddSpace(AnsiString File);
bool __fastcall OnlyMakeFileDataInPut(int iKey);

extern bool __fastcall OnlyNumberInPut(int iKey);
extern bool __fastcall OnlyNumberAndDotInPut(int iKey);                         //only for Number and char '.'
extern AnsiString DefaultPath;
extern AnsiString asSystemPath;
extern AnsiString asGeneralPath;
extern AnsiString asBarCodeLogPath;
extern AnsiString asBarCodeCommLogPath;                                         //Steven 20160421 : Add 2D CCD Log
extern AnsiString asTrayMapCommLogPath;                                         //wei 20161219 Tray Mapping
extern AnsiString asKeyenceCommLogPath;                                         //wei 20161219 Tray Mapping
extern AnsiString asTrayMapLogPath;                                             //wei 20161219 Tray Mapping
extern AnsiString asTrayMapDataPath;                                            //wei 20161219 Tray Mapping
extern AnsiString asBarCodeLot;
extern AnsiString asDBPath;
extern AnsiString asSummaryPath;
extern AnsiString AuthPath;
extern AnsiString DataPath;
extern AnsiString OffsetPath;
extern AnsiString BmpPath;
extern AnsiString IoTablePath;
extern AnsiString MotTablePath;
extern AnsiString TrayTablePath;
extern AnsiString PlateTablePath;
extern AnsiString GrapicPath;
extern AnsiString DIOCFGPath;
extern AnsiString asErrNotePath;
extern AnsiString ConfigMemoPath;
//extern AnsiString RunPath;
extern AnsiString LastDataPath;
extern AnsiString SecsGemPath;
extern AnsiString as9045UPH;                                                    //Frank 20150515 新增UPH資料夾
extern AnsiString as9045LogPath;                                                //Steven 20141229 : 修改Log Path
extern AnsiString asEPLogPath  ;
extern AnsiString asHiSiLogPath;
extern AnsiString asLbTempLogPath;                                              //Steven 20250110 : LB溫度log
extern AnsiString asShtLogPath ;
extern AnsiString asHomeLogPath;
extern AnsiString asTorqLogPath;
extern AnsiString asACSmartIntervalLogPath;                                     //Sam 20240726 : AI Clean
extern AnsiString asTrayLogPath;
extern AnsiString asTempLogPath;
extern AnsiString asEJ1NLogPath;
extern AnsiString asLaserLogPath;
extern AnsiString asOLPLogPath;
extern AnsiString asASELogPath;
extern AnsiString asTravelingLogPath;
extern AnsiString JOBFILEDataPath;                                              //kevin 20140105  高雄福雷
extern AnsiString JOBFILEOffsetPath;                                            //kevin 20140105  高雄福雷
extern AnsiString sBarcodeFile;                                                 //wei 20160728 Barcode File切換
extern AnsiString asBarCodeDownLot;
extern AnsiString asSQLLogPath;                                                 //Steven 20150426 : 紀錄SQL存取速度
extern AnsiString asGAILLogPath;                                                //kevin 20150909 : 紀錄Gail 通訊
extern AnsiString asBarCodeLogPath1;                                            //wei 20150720 OCR BarCode_LOG
extern AnsiString asHeaterOnLogPath;                                            //Steven 20151123 : Log for Heater Relay
extern AnsiString asESDLogPath;                                                 //kevin 20191212 Handle sens HISI ESD path
extern AnsiString asPadCommLogPath;                                             //Ken 20210702 AddPadInterface
extern AnsiString asUDPLogPath;                                                 //kevin 20211020 UDP
extern AnsiString asVibrateMotCommLogPath;                                      //JerryYang 20230814 : add震動馬達通訊調速版本
extern AnsiString asYieldRecordPath;                                            //Ifor 20151221 :Log for Yield
extern AnsiString asProductionLogPath;                                          //JerryYang 20151225 Production Log for SPIL蘇州
extern AnsiString asPMAlarmPath;                                                //wei 20160225 PMAlarmFunction
extern AnsiString asProductDataPath;                                            //kevin 20160724 :新增Auto 123 Fix 123 數量Record
extern AnsiString asHandlePath;                                                 //kevin 20190411 handle log
extern AnsiString sJamRatePath;
extern AnsiString sDailyJamPath;
extern AnsiString asQtyDataPath;
extern AnsiString asProductRecordPath;                                          //JerryYang 20230721 : Analog要求production record
extern AnsiString asSearch2DIDByLot;                                            //Frank 20170316 (wei) add Search 2DID By Lot
extern AnsiString asSortingBy2DID;                                              //JerryYang 20190313 : 2D sorting
extern AnsiString as2DIDSortToError;                                            //JerryYang 20231218 : 2DID黑名單功能
extern AnsiString sPMList_Month            ;
extern AnsiString sPMList_Quarter          ;
extern AnsiString sPMList_Year             ;
extern AnsiString sPMList_Temperature      ;
extern AnsiString sPMList_ESD              ;
extern AnsiString sPMList_IonFan           ;
extern AnsiString sPMItem_List             ;
extern AnsiString sPMItem_Check            ;
extern AnsiString sPMItem_ListTemperature  ;
extern AnsiString sPMItem_CheckTemperature ;
extern AnsiString sPMItem_ListESD          ;
extern AnsiString sPMItem_CheckESD         ;
extern AnsiString sPMItem_ListIonFan       ;
extern AnsiString sPMItem_CheckIonFan      ;
extern AnsiString sPMSetting               ;
extern SHFILEOPSTRUCT oFile;
extern int CheckPtrItem;
extern AnsiString asIndexZphasePath;                                            //kevin 20170511 (wei) add Zphase
extern AnsiString asARSMParaPath ;                                              //Ifor 20170621 (wei) add ARMS Function
extern AnsiString sProductionInfoFilePath;                                      //Sam 20170603 (Steven) 移植超豐 OEE 功能 form HT-7045
extern AnsiString asLightScalePath;
extern AnsiString asTCPIPPath;                                                  //Isaac 20170809 (wei) TCPIP log
extern AnsiString asRenesasCommPath;                                            //RogerYang 20250812 : 瑞薩FT-CT
extern AnsiString asRenesasTaskPath;                                            //RogerYang 20250812 : 瑞薩FT-CT
extern AnsiString aESDSetDataFileName;                                          //kevin 20180907 (Steven) : add ESD Data
extern AnsiString asGalilCmdPath;                                               //JerryYang 20181130 (Steven) : add galil command log
extern AnsiString asASECLFTPPath;                                               //Steven 20181224 : For ASE-CL
extern AnsiString asChipMosFTPPath;                                             //Steven 20210413 : 南茂的自動Start功能
extern AnsiString asChipMosFTPPath2;                                            //Steven 20210413 : 南茂的自動Start功能
extern AnsiString asSaveEventLogPath;
extern AnsiString asTeachPath;                                                  //kevin 20190305 add teach.ini
extern AnsiString asOCRSaveMane;
extern AnsiString asProduct_LoaderPath;                                         //Sam 20210107 : Summary Report fuction
extern AnsiString asVacuumOnOffLogPath;                                         //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
extern AnsiString asFix2AICCDDataPath;                                          //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
extern AnsiString asAlarmLogXmlPath;
extern AnsiString sFTPSetupFileLogPath;                                         //Sam 20210803 : FTP SetFile Change Log
extern AnsiString sSaveByMachine;                                               //JimmyChiu 20220618 : save by machine

extern AnsiString asTestTCPIPLogPath;                                           //wei 20211022 Tester TCP/IP
extern AnsiString sGroundESDLogPath;                                            //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
extern AnsiString aAutoTempureOffsetFTP;                                        //Sam 20220406 : 溫度自動補償功能 By FTP
extern AnsiString aAutoTempureOffsetFTPBackup;                                  //Sam 20220406 : 溫度自動補償功能 By FTP
extern AnsiString aDownloadUpdateAutomaticallyPath;                             //Sam 20220824 : FTP 自動下載安裝更新包
extern AnsiString asShtSenLogPath;                                              //kevin 20220912 add shuttle sensor log
extern AnsiString asInArmAutoAlignmentPath;                                     //KenHsieh 20220111 : AOA add Save Log
extern AnsiString asOutArmAutoAlignmentPath;                                    //KenHsieh 20220111 : AOA add Save Log
extern AnsiString asCheckIndexYLogPath;                                         //Ztex 2023.05.11 Add CheckIndexY
extern AnsiString abHandlerStateChangeUploadServerPath;                         //Sam 20230511 : 機台資料變更後須上傳 FTP
extern AnsiString asCleanPadLogPath;                                            //Sam 20230616 : Add Auto Clean Record
extern AnsiString asBackup2DSortListPath;                                       //JerryYang 20230822 : 佳熹要求備份
extern AnsiString asTesterReportPath;                                           //Sam 20231115 : PTI 新增 Tester report
extern AnsiString asBackup2DSummaryPath;                                        //JerryYang 20231205 : 備份2DID summary
extern AnsiString asJamStatPath;                                                //Ifor 20220310 add:JamStatList Path
extern AnsiString asPowerSavePath;                                              //Ifor 20220927 add:Power Save Path
extern AnsiString asATCPowerSavePath;                                           //Ifor 20240410 add:ATC Power Save Path
extern AnsiString asDupBundleID;
extern AnsiString aslDupUnloadBundlID;
extern AnsiString asTempLog_HS;
extern AnsiString asEPLog_HS;
extern AnsiString asESDLog_HS;
extern AnsiString asATCEventLog_HS;
extern AnsiString asArmTestLog_HS;
//typedef struct                                                                //JimmyChiu 20220124 沒用到
//{
//    TCheckBox *ChkPtr;
//    bool *Parameter;
//} CHECT_PTR;

typedef struct
{
    TSpeedButton *SButton;
    bool Visible;
} SPEEDBUTTON_PTR;

//extern CHECT_PTR CheckPtr[300];                                               //Ifor 20161213 CheckPtr 200 -> 300
//extern std::vector<CHECT_PTR *> myCheckPtr;                                   //Steven 20120528 : 改用Vector
extern void WriteDataToFile(char* cFilePath, char* cData, bool bOverWrite=false);
extern void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite=false);
extern char* ReadDataFromFile(AnsiString cFilePath);                            //JimmyChiu 20230428 : add Read file data
extern bool CheckFileIsEmpty(AnsiString cFilePath);
extern DWORD MyTickCount();
extern DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable);
extern void MySleep(DWORD dwMilliseconds);
extern int MyForceDirectories(AnsiString Directory, AnsiString Function="");
extern AnsiString __fastcall TempChangeLog(AnsiString Group, AnsiString Name);  //Ifor 20190930 : add Display temperature switch Site Name
extern AnsiString as3SigmaLogPath;                                              //kevin 20200521 add 3 SIGMA  Hmy 20200515 Copy//Hmy 20200510 Add Enable 3 Sigma Temp Monitor
extern AnsiString asTrayStepSpeedByMachinePatch;                                //Sam 20201221 : Tray y step motor by machine
extern AnsiString asHPCardPath;                                                 //kevin 202111111 MCU Command log
extern AnsiString asDefaultRecipeChangeLogPath;                                 //Sam 20201209 : Default Recipe ChangeLog
extern AnsiString GetRecipePath();                                              //JimmyChiu 20220601 : config儲存跟隨recipe
extern AnsiString GetRecipeFileName(AnsiString FileName);                       //Steven 20241030 : 取得檔案的完整路徑與名稱
extern AnsiString asGroundManPath;                                              //KenHsieh 20220728 : 新增GroundMan Value Log
extern AnsiString asOCRDownLoadLotPath;                                         //KenHsieh 20220825 : 新增OCR比對功能
extern AnsiString asOCRLotPath;                                                 //KenHsieh 20220825 : 新增OCR比對功能
extern AnsiString asProductionByFilePath;                                       //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
extern void SGDToCSV(TStringGrid *strGrid, AnsiString sSplit, AnsiString sReplaceSplit, AnsiString Path);      //Sam 20240104 : 文字裡若有 CSV 分隔號會導致資料分割錯誤需要用其他分隔號來取代 //Sam 20231106 : 新增 SGD 轉 CSV
typedef struct _VersionKeyInfo                                                  //Sam 20230328 : 自動更新增加版本檢查
{
    TCHAR const *szKey;
    TCHAR *szValue;
}VKINFO, *LPVKINFO;
bool IsFileInUse(const char* filePath);                                         //Jimmychiu 20241121 : Verify whether the file is currently in use
extern AnsiString as2DWhiteListLog;                                             //JerryYang 20241104 : 支援2DID白名單功能
extern AnsiString as2DWhiteListLogName;
extern AnsiString SVNRevision;
extern AnsiString sGroundESDInterValPath;                                       //Sam 20250609 : Record Ground and ESD at intervals and upload
extern AnsiString sWhite2DIDListLoc;                                            //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能

int __fastcall MySecondsBetween(TDateTime DT1, TDateTime DT2);                  //Sam 20251031 : Add 時間差計算 DT2-DT1
#endif
