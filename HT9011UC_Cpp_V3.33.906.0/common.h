// ===========================================================================
//  common.h  -- PARTIAL translation of BCB6 common.h
//  Golden ref: D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\common.h
//
//  THIS BATCH (W3 TranslateHelpers): INI-helper family only.
//    OpenIniFile / CloseIniFile / OpenIniFileMem / CloseIniFileMem
//    OpenGeneralIniFile / CloseGeneralIniFile
//    CheckIniData / CheckIniDataMem
//    CheckAndReadIniData* (6 overloads)
//    CheckAndReadIniDataGeneral* (4 overloads)
//    ReadIniData* (4 overloads)
//    ReadIniDataMem* (4 overloads)
//    WriteIniData* (6 overloads)
//    WriteIniDataNoLog (3 overloads)
//    WriteIniData1
//    WriteIniDataGeneral (4 overloads)
//    ReplaceIniData
//    CheckSectionExist / CheckKeyExist
//    ReadWriteIni (6 overloads)
//
//  All other declarations from the BCB6 common.h are gated below
//  with #if 0 // TODO(wave-N) and a WAVE note.
//
//  RULES
//  -----
//  * #include "vclcompat/vcl_compat.h" replaces #include <vcl.h>.
//  * __fastcall is neutralized to empty by vcl_compat.h.
//  * Signatures are VERBATIM copies of the BCB6 originals (names, types,
//    default args) so call-site code needs zero changes.
//  * extern globals defined in common.cpp are re-declared here so other
//    translation units that previously included common.h keep compiling.
// ===========================================================================
#ifndef COMMON_H
#define COMMON_H

#include "vclcompat/vcl_compat.h"   // was: #include <vcl.h> + BCB6 RTL headers
#include "MachineType.h"

// ---------------------------------------------------------------------------
//  IniFile singletons (defined in common.cpp, common.h:38-41 / common.cpp:148-151)
// ---------------------------------------------------------------------------
extern TIniFile    *INIFile;          // common.cpp:149
extern TIniFile    *INIFileGeneral;   // common.cpp:148
extern TMemIniFile *INIFileMem;       // common.cpp:151

// ---------------------------------------------------------------------------
//  Path globals (declared extern here; defined in common.cpp:18-174).
//  Exact list mirrors common.h:108-287.
// ---------------------------------------------------------------------------
extern AnsiString DefaultPath;
extern AnsiString asSystemPath;
extern AnsiString asGeneralPath;
extern AnsiString asBarCodeLogPath;
extern AnsiString asBarCodeCommLogPath;
extern AnsiString asTrayMapCommLogPath;
extern AnsiString asKeyenceCommLogPath;
extern AnsiString asTrayMapLogPath;
extern AnsiString asTrayMapDataPath;
extern AnsiString asBarCodeLot;
extern AnsiString asTrayIDByLot;
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
extern AnsiString LastDataPath;
extern AnsiString SecsGemPath;
extern AnsiString as9045UPH;
extern AnsiString as9045LogPath;
extern AnsiString asEPLogPath;
extern AnsiString asHiSiLogPath;
extern AnsiString asLbTempLogPath;
extern AnsiString asShtLogPath;
extern AnsiString asHomeLogPath;
extern AnsiString asTorqLogPath;
extern AnsiString asACSmartIntervalLogPath;
extern AnsiString asTrayLogPath;
extern AnsiString asTempLogPath;
extern AnsiString asEJ1NLogPath;
extern AnsiString asLaserLogPath;
extern AnsiString asOLPLogPath;
extern AnsiString asASELogPath;
extern AnsiString asTravelingLogPath;
extern AnsiString JOBFILEDataPath;
extern AnsiString JOBFILEOffsetPath;
extern AnsiString sBarcodeFile;
extern AnsiString asBarCodeDownLot;
extern AnsiString asBarCodeLogPath1;
extern AnsiString asHeaterOnLogPath;
extern AnsiString asESDLogPath;
extern AnsiString asPadCommLogPath;
extern AnsiString asUDPLogPath;
extern AnsiString asVibrateMotCommLogPath;
extern AnsiString asYieldRecordPath;
extern AnsiString asProductionLogPath;
extern AnsiString asPMAlarmPath;
extern AnsiString asProductDataPath;
extern AnsiString asSocketIDLogPath;
extern AnsiString asHandlePath;
extern AnsiString sJamRatePath;
extern AnsiString sDailyJamPath;
extern AnsiString asQtyDataPath;
extern AnsiString asProductRecordPath;
extern AnsiString asSearch2DIDByLot;
extern AnsiString asSortingBy2DID;
extern AnsiString as2DIDSortToError;
extern AnsiString sPMList_Month;
extern AnsiString sPMList_Quarter;
extern AnsiString sPMList_Year;
extern AnsiString sPMList_Temperature;
extern AnsiString sPMList_ESD;
extern AnsiString sPMList_IonFan;
extern AnsiString sPMItem_List;
extern AnsiString sPMItem_Check;
extern AnsiString sPMItem_ListTemperature;
extern AnsiString sPMItem_CheckTemperature;
extern AnsiString sPMItem_ListESD;
extern AnsiString sPMItem_CheckESD;
extern AnsiString sPMItem_ListIonFan;
extern AnsiString sPMItem_CheckIonFan;
extern AnsiString sPMSetting;
extern AnsiString asIndexZphasePath;
extern AnsiString asARSMParaPath;
extern AnsiString sProductionInfoFilePath;
extern AnsiString asLightScalePath;
extern AnsiString asTCPIPPath;
extern AnsiString asRenesasCommPath;
extern AnsiString asRenesasTaskPath;
extern AnsiString aESDSetDataFileName;
extern AnsiString asGalilCmdPath;
extern AnsiString asASECLFTPPath;
extern AnsiString asChipMosFTPPath;
extern AnsiString asChipMosFTPPath2;
extern AnsiString asSaveEventLogPath;
extern AnsiString asTeachPath;
extern AnsiString asOCRSaveMane;
extern AnsiString asProduct_LoaderPath;
extern AnsiString asVacuumOnOffLogPath;
extern AnsiString asFix2AICCDDataPath;
extern AnsiString asAlarmLogXmlPath;
extern AnsiString sFTPSetupFileLogPath;
extern AnsiString sSaveByMachine;
extern AnsiString asBarCodeInspReportPath;
extern AnsiString asTestTCPIPLogPath;
extern AnsiString sGroundESDLogPath;
extern AnsiString aAutoTempureOffsetFTP;
extern AnsiString aAutoTempureOffsetFTPBackup;
extern AnsiString aDownloadUpdateAutomaticallyPath;
extern AnsiString asShtSenLogPath;
extern AnsiString asInArmAutoAlignmentPath;
extern AnsiString asOutArmAutoAlignmentPath;
extern AnsiString asCheckIndexYLogPath;
extern AnsiString abHandlerStateChangeUploadServerPath;
extern AnsiString asCleanPadLogPath;
extern AnsiString asBackup2DSortListPath;
extern AnsiString asTesterReportPath;
extern AnsiString asBackup2DSummaryPath;
extern AnsiString asJamStatPath;
extern AnsiString asPowerSavePath;
extern AnsiString asATCPowerSavePath;
extern AnsiString asDupBundleID;
extern AnsiString aslDupUnloadBundlID;
extern AnsiString asTempLog_HS;
extern AnsiString asEPLog_HS;
extern AnsiString asESDLog_HS;
extern AnsiString asATCEventLog_HS;
extern AnsiString asArmTestLog_HS;
extern AnsiString asSWVersionPath;
extern AnsiString asMachineLifePath;
extern AnsiString as3SigmaLogPath;
extern AnsiString asTrayStepSpeedByMachinePatch;
extern AnsiString asHPCardPath;
extern AnsiString asDefaultRecipeChangeLogPath;
extern AnsiString asGroundManPath;
extern AnsiString asOCRDownLoadLotPath;
extern AnsiString asOCRLotPath;
extern AnsiString asProductionByFilePath;
extern AnsiString as2DWhiteListLog;
extern AnsiString as2DWhiteListLogName;
extern AnsiString SVNRevision;
extern AnsiString sGroundESDInterValPath;
extern AnsiString sWhite2DIDListLoc;
extern AnsiString asATCFileTransferPath;

// ---------------------------------------------------------------------------
//  Ini-helper family (W3 batch -- all translated in common.cpp this wave)
// ---------------------------------------------------------------------------

// ---- open / close singletons (common.cpp:323-365) -------------------------
bool OpenIniFile(AnsiString FileName);                       // common.h:38
void CloseIniFile();                                         // common.h:39
bool OpenIniFileMem(AnsiString FileName);                    // common.h:40
void CloseIniFileMem();                                      // common.h:41

// ---- General.ini shortcut (common.h:22-23) ---------------------------------
void OpenGeneralIniFile();                                   // common.h:22
void CloseGeneralIniFile();                                  // common.h:23

// ---- existence checks (common.h:17-18) -------------------------------------
bool        __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name);
bool        __fastcall CheckIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name);

// ---- CheckAndReadIniData (common.h:43-48) ----------------------------------
TDateTime   __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);
AnsiString  __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
int         __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
bool        __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value);
double      __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);

// ---- CheckAndReadIniDataGeneral (common.h:25-28) ---------------------------
AnsiString __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);
int        __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
bool       __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value);
double     __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value);

// ---- ReadIniData (common.h:60-63) ------------------------------------------
bool        __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);
int         __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
double      __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
AnsiString  __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);

// ---- ReadIniDataMem (common.h:64-67) ----------------------------------------
bool        __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);
int         __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
double      __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
AnsiString  __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);

// ---- WriteIniData (common.h:77-82) -----------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);

// ---- WriteIniDataNoLog (common.h:83-85) ------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);

// ---- WriteIniData1 (common.h:34) -------------------------------------------
void __fastcall WriteIniData1(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);

// ---- WriteIniDataGeneral (common.h:29-32) ----------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);

// ---- ReplaceIniData (common.h:99) ------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2);

// ---- CheckSectionExist / CheckKeyExist (common.h:73-74) --------------------
bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group);
bool __fastcall CheckKeyExist(AnsiString FileName, AnsiString Group, AnsiString Name);

// ---- ReadWriteIni (common.h:51-56) -----------------------------------------
AnsiString    __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value, AnsiString DefaultValue, bool bIsRead);
bool          __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value, bool DefaultValue, bool bIsRead);
TDateTime     __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value, TDateTime DefaultValue, bool bIsRead, bool bCheckRange=false, TDateTime Maximum=TDateTime(0.0), TDateTime Minimum=TDateTime(0.0));
int           __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, int Value, int DefaultValue, bool bIsRead, bool bCheckRange=false, int Maximum=0, int Minimum=0);
double        __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, double Value, double DefaultValue, bool bIsRead, bool bCheckRange=false, double Maximum=0.0, double Minimum=0.0);
unsigned long __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value, unsigned long DefaultValue, bool bIsRead, bool bCheckRange=false, unsigned long Maximum=0, unsigned long Minimum=0);

// ===========================================================================
//  GATED DECLARATIONS  -- out of this batch; kept here so the header remains
//  a faithful structural mirror of the BCB6 original.  Translated code that
//  includes this header will NOT see these names at compile-time until they
//  are translated in a later wave.
// ===========================================================================
#if 0 // TODO(wave-misc-strings): InitCommonString / EncodeStr / DecodeStr
void InitCommonString();                                     // common.h:12
AnsiString EncodeStr(AnsiString sourceStr);                  // common.h:14
AnsiString DecodeStr(AnsiString sourceStr);                  // common.h:15
#endif // TODO(wave-misc-strings)

#if 0 // TODO(wave-path): GetLastOpenFN / WriteLastDataFN / ChangeSaveFileName / GetRecipePath / GetRecipeFileName
AnsiString  __fastcall GetLastOpenFN();                      // common.h:68
void __fastcall WriteLastDataFN(AnsiString SName);           // common.h:76
AnsiString  __fastcall ChangeSaveFileName(AnsiString asFileName); // common.h:69
extern AnsiString GetRecipePath();                           // common.h:268
extern AnsiString GetRecipeFileName(AnsiString FileName);    // common.h:269
#endif // TODO(wave-path)

#if 0 // TODO(wave-file): WriteDataToFile / ReadDataFromFile / CheckFileIsEmpty / MyForceDirectories / IsFileInUse / CopyAndCompressFile / SGDToCSV
extern void WriteDataToFile(char* cFilePath, char* cData, bool bOverWrite=false);     // common.h:255
extern void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite=false); // common.h:256
extern char* ReadDataFromFile(AnsiString cFilePath);         // common.h:257
extern bool CheckFileIsEmpty(AnsiString cFilePath);          // common.h:258
extern int MyForceDirectories(AnsiString Directory, AnsiString Function=""); // common.h:262
bool IsFileInUse(const char* filePath);                      // common.h:280
#endif // TODO(wave-file)

#if 0 // TODO(wave-timing): MyTickCount / MySleepEx / MySleep / MySecondsBetween
extern DWORD MyTickCount();                                  // common.h:259
extern DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable); // common.h:260
extern void MySleep(DWORD dwMilliseconds);                   // common.h:261
int __fastcall MySecondsBetween(TDateTime DT1, TDateTime DT2); // common.h:288
#endif // TODO(wave-timing)

#if 0 // TODO(wave-logging): TempChangeLog
extern AnsiString __fastcall TempChangeLog(AnsiString Group, AnsiString Name); // common.h:263
#endif // TODO(wave-logging)

#if 0 // TODO(wave-canvas): MyDrawText (6 overloads, TCanvas/TRect/TColor VCL GDI)
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str);              // common.h:89
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor); // common.h:90
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor); // common.h:91
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int i);                 // common.h:92
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom); // common.h:93
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom); // common.h:94
#endif // TODO(wave-canvas)

#if 0 // TODO(wave-ui-input): OnlyNumberInPut / OnlyNumberAndDotInPut / OnlyMakeFileDataInPut / AddSpace
bool __fastcall OnlyMakeFileDataInPut(int iKey);             // common.h:102
extern bool __fastcall OnlyNumberInPut(int iKey);            // common.h:104
extern bool __fastcall OnlyNumberAndDotInPut(int iKey);      // common.h:105
void __fastcall AddSpace(AnsiString File);                   // common.h (stub)
#endif // TODO(wave-ui-input)

// ---------------------------------------------------------------------------
//  Misc non-function declarations from common.h that may be needed by callers
//  (gated: SPEEDBUTTON_PTR depends on TSpeedButton which is a VCL form type).
// ---------------------------------------------------------------------------
#if 0 // TODO(wave-ui): SPEEDBUTTON_PTR (TSpeedButton is a VCL form component)
typedef struct
{
    TSpeedButton *SButton;
    bool Visible;
} SPEEDBUTTON_PTR;
#endif // TODO(wave-ui)

#endif // COMMON_H
