// ===========================================================================
//  common.cpp  -- PARTIAL translation of BCB6 common.cpp
//  Golden ref: D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\common.cpp
//
//  THIS BATCH (W3 TranslateHelpers): INI-helper family only.
//  All other common.cpp regions are gated with #if 0 // TODO(wave-N).
//
//  INCLUDES
//  --------
//  #include "vclcompat/vcl_compat.h" replaces the BCB6 #include <vcl.h> /
//  #include "MachineDefine.h" / #pragma hdrstop trio.
//
//  FAITHFULNESS NOTES
//  ------------------
//  * Signatures are VERBATIM copies of the BCB6 originals.
//  * __fastcall is neutralized to empty by vcl_compat.h.
//  * BCB6 dangling-pointer bugs in CloseIniFile / CloseIniFileMem are
//    FAITHFULLY preserved (INIFile / INIFileMem not set to NULL after delete;
//    see notes at each function).
//  * RecordProcess / RecordChangeLogProcess / TempChangeLog / InitialOK /
//    bSysLotStart / CosFunction / FormHS are all out-of-batch dependencies
//    and MUST NOT be called from code compiled in this wave.  In the
//    WriteIniData* bodies the change-log side-effect blocks are gated with
//    #if 0 // TODO(wave-logging/UI) and the RecordProcess open-fail paths
//    are also gated.  RecordProcess FORWARD STUB below provides a no-op so
//    common.cpp itself compiles as a standalone TU; the real RecordProcess
//    (from cMyDB) replaces it at link time in a later wave.
//  * TryStrToFloat: BCB6 SysUtils function; provided in vclcompat/SysUtils.h.
//  * CheckRange: BCB6 template in MachineType.h:1520 (T CheckRange(T,T,T)).
//    The ReadWriteIni overloads that use it are translated here; CheckRange
//    is already visible via MachineType.h.
// ===========================================================================

// ---------------------------------------------------------------------------
//  BCB6 preamble (common.cpp:1-16)
//  Was:  #include "MachineDefine.h"
//        #pragma hdrstop
//        #include "common.h"
//        #include "mymessbox.h"
//        ... (other BCB6-specific includes)
//        #pragma package(smart_init)
//  The BCB6 pragmas and VCL-specific includes are dropped; the compat umbrella
//  provides everything the in-batch body needs.
// ---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   // was: #include <vcl.h> / MachineDefine.h
#include "common.h"                 // common.h:4 (translated header)
#include "MachineType.h"            // common.cpp:8 -- for CheckRange template

#include <cstdio>
#include <cstdlib>
#include <cstring>

// ---------------------------------------------------------------------------
//  Forward stub for RecordProcess (common.cpp:374,402,...).
//  cMyDB.h declares RecordProcess(AnsiString, AnsiString="").  That header
//  pulls untranslated BCB6 UI forms and is out of scope this wave.  Provide
//  a weak no-op here so this TU compiles standalone.  At final link time the
//  real RecordProcess from cMyDB.cpp resolves and overrides this stub.
//
//  TODO(wave-cMyDB): remove this stub once cMyDB is translated and linked.
// ---------------------------------------------------------------------------
static void RecordProcess(AnsiString /*S*/, AnsiString /*S2*/ = AnsiString("")) {}

// ---------------------------------------------------------------------------
//  Forward stub for ShowMyMessage (golden mymessbox.h:58; called from
//  GetLastOpenFN common.cpp:1262/1266/1275 and MyForceDirectories :1709/1715).
//  mymessbox.h pulls untranslated BCB6 UI forms and is out of scope this wave.
//  ht9045_core (this library) must NOT depend on ht9045_sm -- ht9045_sm already
//  has a real sim stand-in for this in canary_support.h, but ht9045_sm
//  transitively depends on ht9045_core, so linking the other way would create
//  a CMake link cycle. Mirror the RecordProcess stub immediately above instead:
//  a local no-op with the same signature golden's call sites need.
//
//  AI(W906-CommonWaveFile) 20260721: added, mirroring the RecordProcess
//  forward-stub precedent above.
//  TODO(wave-UI): remove this stub once mymessbox / a real message-box shim is
//  translated and linked into ht9045_core (or ht9045_core no longer needs it).
// ---------------------------------------------------------------------------
static void ShowMyMessage(AnsiString /*S1*/, AnsiString /*S2*/ = AnsiString(""), AnsiString /*S3*/ = AnsiString(""), bool /*Ok*/ = false, bool /*bServoOff*/ = false) {}

// ---------------------------------------------------------------------------
//  Global path strings (common.cpp:18-174)
//  Big5 source comments are preserved as-is (raw bytes; ASCII portions only
//  shown where needed for provenance).
//
//  NOTE: asKeyStr is file-local in the BCB6 source (no extern in common.h);
//  kept static here so it does not clash with a future cMyDB link unit.
// ---------------------------------------------------------------------------
AnsiString asGeneralPath            ="D:\\HT9045\\system\\Gerneral.ini";             // common.cpp:18
AnsiString asBarCodeLogPath         ="D:\\HT9045_Log\\2DBarCode";                    // common.cpp:19
AnsiString asBarCodeLot             ="D:\\HT9045_Log\\2DBarCode\\LotData.txt";       // common.cpp:20
AnsiString asTrayIDByLot            ="D:\\HT9045_Log\\2DBarCode\\TrayIDByLot.txt";   // common.cpp:21
AnsiString asBarCodeCommLogPath     ="D:\\HT9045_Log\\2DBarCodeComm";                // common.cpp:22
AnsiString asTrayMapCommLogPath     ="D:\\HT9045_Log\\TrayMapComm";                  // common.cpp:23
AnsiString asKeyenceCommLogPath     ="D:\\HT9045_Log\\KeyenceComm";                  // common.cpp:24
AnsiString asTrayMapLogPath         ="D:\\HT9045_Log\\TrayMapLog";                   // common.cpp:25
AnsiString asTrayMapDataPath        ="D:\\HT9045_Log\\TrayMapLog\\TrayData";         // common.cpp:26
AnsiString asSummaryPath            ="D:\\HT9045_Log\\Summary";                      // common.cpp:27
AnsiString asFix2AICCDDataPath      ="D:\\HT9045_Log\\Fix2AICCDLog\\Fix2AICCDData";  // common.cpp:28
AnsiString asSystemPath             ="d:\\HT9045\\system\\";                         // common.cpp:29
AnsiString asDBPath                 ="D:\\HT9045\\MDB\\Handler.db3";                 // common.cpp:30
AnsiString AuthPath                 ="D:\\HT9045\\config\\";                         // common.cpp:31
AnsiString DefaultPath              ="D:\\HT9045\\IniData\\";                        // common.cpp:32
AnsiString DataPath                 ="D:\\HT9045\\IniData\\Data\\";                  // common.cpp:33
AnsiString OffsetPath               ="D:\\HT9045\\IniData\\Offset\\";                // common.cpp:34
AnsiString DIOCFGPath               ="D:\\HT9045\\iniData\\DioCfg\\";               // common.cpp:35
AnsiString asErrNotePath            ="D:\\HT9045\\system\\SpecialErrNote.ini";        // common.cpp:36
AnsiString ConfigMemoPath           ="D:\\HT9045\\config\\Description.ini";          // common.cpp:37
AnsiString LastDataPath             ="D:\\HT9045\\SetUp.inf";                        // common.cpp:38
AnsiString BmpPath                  ="D:\\HT9045\\IMG\\BMP\\";                       // common.cpp:39
AnsiString IoTablePath              ="D:\\HT9045\\System\\IO_Table.csv";             // common.cpp:40
AnsiString MotTablePath             ="D:\\HT9045\\System\\Mot_Table.csv";            // common.cpp:41
AnsiString TrayTablePath            ="D:\\HT9045\\System\\TrayForm.csv";             // common.cpp:42
AnsiString PlateTablePath           ="D:\\HT9045\\System\\PlateForm.csv";            // common.cpp:43
AnsiString GrapicPath               ="D:\\HT9045\\IMG\\Graphic\\";                   // common.cpp:44
AnsiString SecsGemPath              ="D:\\HT9045\\SECS\\SECS\\SYSTEM\\Gerneral.ini"; // common.cpp:45
static AnsiString asKeyStr          ="HontechPassword";                              // common.cpp:46 (file-local)
AnsiString as9045UPH                ="D:\\HT9045_Log\\UPH";                          // common.cpp:47
AnsiString as9045LogPath            ="D:\\HT9045_Log";                               // common.cpp:48
AnsiString asEPLogPath              ="D:\\HT9045_Log\\EP";                           // common.cpp:49
AnsiString asHiSiLogPath            ="D:\\HT9045_Log\\Temperature";                  // common.cpp:50
AnsiString asLbTempLogPath          ="D:\\HT9045_Log\\LB_Temperature";               // common.cpp:51
AnsiString asShtLogPath             ="D:\\HT9045_Log\\ShuttleLog";                   // common.cpp:52
AnsiString asHomeLogPath            ="D:\\HT9045_Log\\HomeLog";                      // common.cpp:53
AnsiString asTorqLogPath            ="D:\\HT9045_Log\\TorqueLog";                    // common.cpp:54
AnsiString asACSmartIntervalLogPath ="D:\\HT9045_Log\\ACSmartIntervalLog";           // common.cpp:55
AnsiString asTrayLogPath            ="D:\\HT9045_Log\\EditTray";                     // common.cpp:56
AnsiString asTempLogPath            ="D:\\HT9045_Log\\Temp";                         // common.cpp:57
AnsiString asEJ1NLogPath            ="D:\\HT9045_Log\\EJ1N_Log";                     // common.cpp:58
AnsiString asLaserLogPath           ="D:\\HT9045_Log\\LaserLog";                     // common.cpp:59
AnsiString asASELogPath             ="D:\\SaveRecord";                               // common.cpp:60
AnsiString asTravelingLogPath       ="D:\\HT9045_Log\\Production_Log";               // common.cpp:61
AnsiString asOLPLogPath             ="D:\\HT9045_Log\\Automation";                   // common.cpp:62
AnsiString asBarCodeLogPath1        ="D:\\HT9045_Log\\BarCode_LOG";                  // common.cpp:65
AnsiString asHeaterOnLogPath        ="D:\\HT9045_Log\\Heater_On_Off_LOG";            // common.cpp:66
AnsiString asYieldRecordPath        ="D:\\HT9045_Log\\Yield";                        // common.cpp:67
AnsiString asESDLogPath             ="D:\\HT9045_Log\\ESD";                          // common.cpp:68
AnsiString asAlarmLogXmlPath        ="D:\\HT9045_Log\\AlarmLogXml";                  // common.cpp:69
AnsiString asPadCommLogPath         ="D:\\HT9045_Log\\PadCommLog";                   // common.cpp:71
AnsiString asUDPLogPath             ="D:\\HT9045_Log\\UDP";                          // common.cpp:72
AnsiString asVibrateMotCommLogPath  ="D:\\HT9045_Log\\VibrateMotorCommLog";          // common.cpp:73
AnsiString asProductionLogPath      ="D:\\RMS";                                      // common.cpp:74
AnsiString asPMAlarmPath            ="D:\\HT9045\\PMAlarm";                          // common.cpp:75
AnsiString asProductDataPath        ="D:\\HT9045_Log\\ProductData\\";                // common.cpp:76
AnsiString asProductRecordPath      ="D:\\HT9045_Log\\ProductRecord";                // common.cpp:77
AnsiString asSocketIDLogPath        ="D:\\HT9045_Log\\SocketIDLog\\";                // common.cpp:78
AnsiString asQtyDataPath            ="D:\\HT9045_Log\\QtyData\\";                    // common.cpp:79
AnsiString sPMList_Month            ="D:\\HT9045\\PMAlarm\\PM_Month.ini";            // common.cpp:80
AnsiString sPMList_Quarter          ="D:\\HT9045\\PMAlarm\\PM_Quarter.ini";          // common.cpp:81
AnsiString sPMList_Year             ="D:\\HT9045\\PMAlarm\\PM_Year.ini";             // common.cpp:82
AnsiString sPMList_Temperature      ="D:\\HT9045\\PMAlarm\\PM_Temperature.ini";      // common.cpp:83
AnsiString sPMList_ESD              ="D:\\HT9045\\PMAlarm\\PM_ESD.ini";              // common.cpp:84
AnsiString sPMList_IonFan           ="D:\\HT9045\\PMAlarm\\PM_IonFan.ini";           // common.cpp:85
AnsiString sPMItem_List             ="D:\\HT9045\\PMAlarm\\PM_List.ini";             // common.cpp:86
AnsiString sPMItem_Check            ="D:\\HT9045\\PMAlarm\\PM_Check.ini";            // common.cpp:87
AnsiString sPMItem_ListTemperature  ="D:\\HT9045\\PMAlarm\\PM_ListTemperature.ini";  // common.cpp:88
AnsiString sPMItem_CheckTemperature ="D:\\HT9045\\PMAlarm\\PM_CheckTemperature.ini"; // common.cpp:89
AnsiString sPMItem_ListESD          ="D:\\HT9045\\PMAlarm\\PM_ListESD.ini";          // common.cpp:90
AnsiString sPMItem_CheckESD         ="D:\\HT9045\\PMAlarm\\PM_CheckESD.ini";         // common.cpp:91
AnsiString sPMItem_ListIonFan       ="D:\\HT9045\\PMAlarm\\PM_ListIonFan.ini";       // common.cpp:92
AnsiString sPMItem_CheckIonFan      ="D:\\HT9045\\PMAlarm\\PM_CheckIonFan.ini";      // common.cpp:93
AnsiString sPMSetting               ="D:\\HT9045\\PMAlarm\\PM_Setting.ini";          // common.cpp:94
AnsiString JOBFILEDataPath          ="JOBFILE";                                      // common.cpp:95
AnsiString JOBFILEOffsetPath        ="JOBFILE";                                      // common.cpp:96
AnsiString sBarcodeFile             ="D:\\HT9045\\Barcode_File\\";                   // common.cpp:97
AnsiString asBarCodeDownLot         ="D:\\HT9045_Log\\2DBarCode\\LotCheckData.txt";  // common.cpp:98
AnsiString asSearch2DIDByLot        ="D:\\HT9045_Log\\2DBarCode\\Search2DIDByLot.txt"; // common.cpp:99
AnsiString asSortingBy2DID          ="D:\\HT9045_Log\\2DBarCode\\SortingBy2DID.txt"; // common.cpp:100
AnsiString as2DIDSortToError        ="D:\\HT9045_Log\\2DBarCode\\2DID_list.txt";     // common.cpp:101
AnsiString asIndexZphasePath        ="D:\\HT9045_Log\\Zphase";                       // common.cpp:102
AnsiString asARSMParaPath           ="D:\\HT9045\\system\\ARMS.ini";                 // common.cpp:103
AnsiString sProductionInfoFilePath  ="D:\\HT9045_log\\ProductionInfo";               // common.cpp:104
AnsiString asLightScalePath         ="D:\\HT9045_Log\\LightScale\\";                 // common.cpp:105
AnsiString asTCPIPPath              ="D:\\HT9045_Log\\TCPIP_Log";                    // common.cpp:106
AnsiString asRenesasCommPath        ="D:\\HT9045_Log\\RenesasComm_Log";              // common.cpp:107
AnsiString asRenesasTaskPath        ="D:\\HT9045_Log\\RenesasTask_Log";              // common.cpp:108
AnsiString aESDSetDataFileName      ="D:\\ESD_Program\\system\\General.ini";         // common.cpp:109
AnsiString asGalilCmdPath           ="D:\\HT9045_Log\\Galil_Log";                    // common.cpp:110
AnsiString asASECLFTPPath           ="D:\\ASECLFTP";                                 // common.cpp:111
AnsiString asChipMosFTPPath         ="D:\\HT9045_Log\\FTP";                          // common.cpp:112
AnsiString asChipMosFTPPath2        ="D:\\HT9045_Log\\ChipMosFTP";                   // common.cpp:113
AnsiString asSaveEventLogPath       ="D:\\HT9045_Log\\SaveEventLog";                 // common.cpp:114
AnsiString asOCRSaveMane            ="D:\\HT9045_Log\\OCR\\Alarm\\Bad";              // common.cpp:115
AnsiString asTeachPath              ="D:\\HT9045\\system\\teach.ini";                // common.cpp:117
AnsiString asHandlePath             ="D:\\HT9045_Log\\Handlelog";                    // common.cpp:118
AnsiString sJamRatePath             ="D:\\HT9045_Log\\JamRate";                      // common.cpp:119
AnsiString sDailyJamPath            ="D:\\HT9045_Log\\JamRate_Daily";                // common.cpp:120
AnsiString sFTPSetupFileLogPath     ="D:\\HT9045_Log\\FTPSetupFileChangeLog";        // common.cpp:121
AnsiString sSaveByMachine           ="D:\\HT9045\\IniData\\SaveByMachine\\";         // common.cpp:122
AnsiString asBarCodeInspReportPath  ="D:\\HT9045_Log\\2DIDParameterReport";          // common.cpp:123
AnsiString as3SigmaLogPath          ="D:\\HT9045_log\\TemperatureLog_3Sigma";        // common.cpp:125
AnsiString asTrayStepSpeedByMachinePatch ="D:\\HT9045\\system\\TrayStepSpeed.ini";   // common.cpp:126
AnsiString asProduct_LoaderPath     ="D:\\HT9045_Log\\Production_Loader";            // common.cpp:127
AnsiString asVacuumOnOffLogPath     ="D:\\HT9045_log\\VacuumOnOffLog";               // common.cpp:128
AnsiString asHPCardPath             ="D:\\HT9045_log\\HPCARD";                       // common.cpp:129
AnsiString asDefaultRecipeChangeLogPath ="D:\\HT9045_Log\\DefaultRecipeChangeLog";   // common.cpp:130
AnsiString asTestTCPIPLogPath       ="D:\\HT9045_Log\\Test_TCPIP";                   // common.cpp:131
AnsiString sGroundESDLogPath        ="D:\\HT9045_Log\\GroundESDLog";                 // common.cpp:132
AnsiString aAutoTempureOffsetFTP    ="D:\\HT9045_Log\\AutoTempureOffsetFTP";         // common.cpp:133
AnsiString aAutoTempureOffsetFTPBackup ="D:\\HT9045_Log\\AutoTempureOffsetFTPBackup"; // common.cpp:134
AnsiString aDownloadUpdateAutomaticallyPath ="D:\\HT9045_Log\\Installer";            // common.cpp:135
AnsiString asGroundManPath          ="D:\\HT9045_log\\GroundManLog\\GroundManValue";  // common.cpp:136
AnsiString asShtSenLogPath          ="D:\\HT9045_Log\\ShuttleSensorLog\\";           // common.cpp:137
AnsiString asInArmAutoAlignmentPath ="D:\\HT9045_log\\AOA_Log\\InArm";               // common.cpp:138
AnsiString asOutArmAutoAlignmentPath ="D:\\HT9045_log\\AOA_Log\\OutArm";             // common.cpp:139
AnsiString abHandlerStateChangeUploadServerPath ="D:\\HT9045_Log\\HandlerStateChangeUploadServer"; // common.cpp:140
AnsiString asCleanPadLogPath        ="D:\\HT9045_Log\\CleanPad_Log";                 // common.cpp:141
AnsiString asBackup2DSortListPath   ="D:\\HT9045_log\\2D_SortList_Backup\\";         // common.cpp:142
AnsiString asOCRDownLoadLotPath     ="D:\\HT9045_Log\\OCR\\CompareFile";             // common.cpp:143
AnsiString asOCRLotPath             ="D:\\HT9045_Log\\OCR\\OCRLot.txt";              // common.cpp:144
AnsiString asProductionByFilePath   ="D:\\HT9045_Log\\Production_ByFile";            // common.cpp:145
AnsiString asTesterReportPath       ="C:\\HontechTestLogs";                          // common.cpp:146
AnsiString asMachineLifePath        ="D:\\HT9045\\system\\MachineLife.ini";          // common.cpp:147

// INI singleton globals (common.cpp:148-151)
TIniFile    *INIFileGeneral = nullptr;   // common.cpp:148
TIniFile    *INIFile        = nullptr;   // common.cpp:149
TMemIniFile *INIFileMem     = nullptr;   // common.cpp:151

AnsiString asCheckIndexYLogPath     ="D:\\HT9045_log\\CheckIndexYLog";               // common.cpp:153
AnsiString asBackup2DSummaryPath    ="D:\\HT9045_log\\Summary_Backup\\";             // common.cpp:154
AnsiString asDupBundleID            ="D:\\HT9045_Log\\BundleID\\DupBundleID.txt";    // common.cpp:155
AnsiString aslDupUnloadBundlID      ="D:\\HT9045_Log\\BundleID\\DupUnloadBundleID.txt"; // common.cpp:156
AnsiString as2DWhiteListLog         ="D:\\HT9045_Log\\2DBarCode\\CheckResult\\";     // common.cpp:157
AnsiString as2DWhiteListLogName     ="";                                             // common.cpp:158
AnsiString SVNRevision              ="";                                             // common.cpp:159
AnsiString asSWVersionPath          ="D:\\HT9045_log\\SW_Version\\";                 // common.cpp:160
AnsiString asJamStatPath            ="D:\\HT9045_Log\\JamStatList";                  // common.cpp:164
AnsiString asPowerSavePath          ="D:\\HT9045_Log\\Power_EnergySaving";           // common.cpp:165
AnsiString asATCPowerSavePath       ="D:\\HT9045_Log\\ATC_EnergySaving";             // common.cpp:166
AnsiString asTempLog_HS             ="D:\\HandlerLog\\Temperature_Log\\";            // common.cpp:167
AnsiString asEPLog_HS               ="D:\\HandlerLog\\EP_Log\\";                     // common.cpp:168
AnsiString asESDLog_HS              ="D:\\HandlerLog\\ESD_Log\\";                    // common.cpp:169
AnsiString asATCEventLog_HS         ="D:\\HandlerLog\\ATCEven_Log\\";               // common.cpp:170
AnsiString asArmTestLog_HS          ="D:\\HandlerLog\\ArmTestInfo\\";                // common.cpp:171
AnsiString sGroundESDInterValPath   ="D:\\HT9045_Log\\RecordGroundESDInterval";      // common.cpp:172
AnsiString asATCFileTransferPath    ="D:\\ATC\\Data\\SaveFile\\";                    // common.cpp:173
AnsiString sWhite2DIDListLoc        ="D:\\HT9045_Log\\White2DIDListTmp\\";           // common.cpp:174

// ===========================================================================
//  GATED: InitCommonString (common.cpp:177-265)
//  WAVE: misc-strings -- depends on same path globals but also reassigns
//  SecsGemPath to a shorter form and resets asKeyStr; no external coupling
//  otherwise.  Translated body deferred until misc-strings wave because the
//  function is only called at startup and the defaults above already match
//  the reset values for the majority of paths.
// ===========================================================================
#if 0 // TODO(wave-misc-strings): InitCommonString
void InitCommonString()
{
    // ... (common.cpp:177-265) -- reassign all path globals at runtime.
    // Defer: the runtime reset is only needed for machines that mount HT9045
    // from a non-default drive; the global initialisers above are the same
    // values that InitCommonString assigns, so omitting this body is safe for
    // the compile/test wave.
}
#endif // TODO(wave-misc-strings)

// ===========================================================================
//  GATED: EncodeStr / DecodeStr (common.cpp:267-321)
//  WAVE: misc-strings -- pure XOR password codec; no external dependency.
// ===========================================================================
#if 0 // TODO(wave-misc-strings): EncodeStr / DecodeStr
AnsiString EncodeStr(AnsiString sourceStr) { /* common.cpp:267 */ return sourceStr; }
AnsiString DecodeStr(AnsiString sourceStr) { /* common.cpp:295 */ return sourceStr; }
#endif // TODO(wave-misc-strings)

// ===========================================================================
//  OpenIniFile / CloseIniFile (common.cpp:323-343)
// ===========================================================================

// ---------------------------------------------------------------------------
//  OpenIniFile (common.cpp:323-334)
//  Caching singleton: if INIFile is NULL or bound to a different path, close
//  and reopen.  Returns false (without opening) if FileName is empty.
//  Steven 20141120 : Add Read/Write IniFile Speed
// ---------------------------------------------------------------------------
bool OpenIniFile(AnsiString FileName)   // common.cpp:323
{
    if (FileName == "")                 // common.cpp:325 -- empty-path guard
        return false;

    if (INIFile == nullptr || INIFile->FileName != FileName)  // common.cpp:328
    {
        CloseIniFile();
        INIFile = new TIniFile(FileName);
    }
    return true;
}

// ---------------------------------------------------------------------------
//  CloseIniFile (common.cpp:336-343)
//  FAITHFUL BUG preserved: INIFile is NOT set to NULL after delete.
//  The next OpenIniFile with the same path will see a non-NULL stale pointer
//  whose FileName still matches, and will reuse the freed object.
//  This matches BCB6 behaviour exactly (common.cpp:336-343).
//  Steven 20141120 : Add Read/Write IniFile Speed
// ---------------------------------------------------------------------------
void CloseIniFile()                     // common.cpp:336
{
    if (INIFile != nullptr)
    {
        INIFile->UpdateFile();
        delete INIFile;
        // NOTE: INIFile intentionally NOT set to nullptr here -- faithful bug.
    }
}

// ===========================================================================
//  OpenIniFileMem / CloseIniFileMem (common.cpp:345-365)
//  RogerYang 20260214 : load entire file into memory to bypass 32KB/section cap
// ===========================================================================

// ---------------------------------------------------------------------------
//  OpenIniFileMem (common.cpp:345-356)
// ---------------------------------------------------------------------------
bool OpenIniFileMem(AnsiString FileName)    // common.cpp:345
{
    if (FileName == "")
        return false;

    if (INIFileMem == nullptr || INIFileMem->FileName != FileName)  // common.cpp:350
    {
        CloseIniFileMem();
        INIFileMem = new TMemIniFile(FileName);
    }
    return true;
}

// ---------------------------------------------------------------------------
//  CloseIniFileMem (common.cpp:358-365)
//  FAITHFUL BUG preserved: INIFileMem NOT set to nullptr after delete.
// ---------------------------------------------------------------------------
void CloseIniFileMem()                      // common.cpp:358
{
    if (INIFileMem != nullptr)
    {
        INIFileMem->UpdateFile();
        delete INIFileMem;
        // NOTE: INIFileMem intentionally NOT set to nullptr -- faithful bug.
    }
}

// ===========================================================================
//  CheckIniData / CheckIniDataMem (common.cpp:367-395)
// ===========================================================================

// ---------------------------------------------------------------------------
//  CheckIniData (common.cpp:367-380)
// ---------------------------------------------------------------------------
bool __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name)  // common.cpp:367
{
    bool bTemp = false;
    AnsiString Str;
    if (OpenIniFile(FileName) == false)    // common.cpp:371
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);                // TODO(wave-cMyDB): real cMyDB::RecordProcess
        return bTemp;
    }

    bTemp = INIFile->ValueExists(Group, Name);   // common.cpp:378
    return bTemp;
}

// ---------------------------------------------------------------------------
//  CheckIniDataMem (common.cpp:382-395)
//  RogerYang 20260214
// ---------------------------------------------------------------------------
bool __fastcall CheckIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name)  // common.cpp:382
{
    bool bTemp = false;
    AnsiString Str;
    if (OpenIniFileMem(FileName) == false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return bTemp;
    }

    bTemp = INIFileMem->ValueExists(Group, Name);   // common.cpp:393
    return bTemp;
}

// ===========================================================================
//  CheckAndReadIniData overloads (common.cpp:397-508)
// ===========================================================================

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(double) (common.cpp:397-413)
//  Missing key -> seed with sprintf "%0.4f" text via WriteString, return Value.
//  Present key -> ReadFloat; return updated Value.
// ---------------------------------------------------------------------------
double __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  // common.cpp:397
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    Str.sprintf("%0.4f", Value);                                  // common.cpp:407
    if (!INIFile->ValueExists(Group, Name))
        INIFile->WriteString(Group, Name, Str);                   // common.cpp:409 -- seed default
    else
        Value = INIFile->ReadFloat(Group, Name, Value);           // common.cpp:411
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(unsigned long) (common.cpp:415-430)
//  ODD/FAITHFUL: missing key path calls ReadInteger with result DISCARDED --
//  no seed write, unlike every other CheckAndReadIniData overload.
// ---------------------------------------------------------------------------
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)  // common.cpp:415
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    if (!INIFile->ValueExists(Group, Name))
        INIFile->ReadInteger(Group, Name, static_cast<int>(Value));  // common.cpp:426 -- READ, result discarded
    else
        Value = static_cast<unsigned long>(INIFile->ReadInteger(Group, Name, static_cast<int>(Value))); // common.cpp:428
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(int) (common.cpp:432-447)
//  Missing key -> WriteInteger (seed); present -> ReadInteger.
// ---------------------------------------------------------------------------
int __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  // common.cpp:432
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    if (!INIFile->ValueExists(Group, Name))
        INIFile->WriteInteger(Group, Name, Value);               // common.cpp:443 -- seed
    else
        Value = INIFile->ReadInteger(Group, Name, Value);        // common.cpp:445
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(bool) (common.cpp:449-464)
//  Missing key -> WriteBool (seed); present -> ReadBool.
// ---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value)  // common.cpp:449
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    if (!INIFile->ValueExists(Group, Name))
        INIFile->WriteBool(Group, Name, Value);                  // common.cpp:460 -- seed
    else
        Value = INIFile->ReadBool(Group, Name, Value);           // common.cpp:462
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(AnsiString) (common.cpp:466-491)
//  Missing key -> WriteString (seed), Str=Value.
//  Present key -> ReadString; EXTRA REPAIR: if stored value is "" and default
//  is not "", re-seed with WriteString (Steven 20160323 + JerryYang 20170711).
// ---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:466
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    if (!INIFile->ValueExists(Group, Name))
    {
        INIFile->WriteString(Group, Name, Value);                // common.cpp:478 -- seed
        Str = Value;                                             // common.cpp:479 -- JerryYang 20170711
    }
    else
    {
        Str = INIFile->ReadString(Group, Name, Value);           // common.cpp:483
        if (Str == "" && Value != "")                            // common.cpp:484 -- Steven 20160323
        {
            Str = Value;
            INIFile->WriteString(Group, Name, Value);            // re-seed empty stored value
        }
    }
    return Str;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniData(TDateTime) (common.cpp:493-508)
//  Missing key -> WriteDateTime (seed); present -> ReadDateTime.
// ---------------------------------------------------------------------------
TDateTime __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)  // common.cpp:493
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }

    if (!INIFile->ValueExists(Group, Name))
        INIFile->WriteDateTime(Group, Name, Value);              // common.cpp:504 -- seed
    else
        Value = INIFile->ReadDateTime(Group, Name, Value);       // common.cpp:506
    return Value;
}

// ===========================================================================
//  ReadIniData overloads (common.cpp:510-564)
//  Pure reads: no seeding/writes; default = passed value.
// ===========================================================================

// ---------------------------------------------------------------------------
//  ReadIniData(bool) (common.cpp:510-522)
// ---------------------------------------------------------------------------
bool __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  // common.cpp:510
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return bValue;
    }
    return INIFile->ReadBool(Group, Name, bValue);               // common.cpp:520
}

// ---------------------------------------------------------------------------
//  ReadIniData(double) (common.cpp:524-536)
// ---------------------------------------------------------------------------
double __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  // common.cpp:524
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFile->ReadFloat(Group, Name, Value);               // common.cpp:534
}

// ---------------------------------------------------------------------------
//  ReadIniData(int) (common.cpp:538-550)
// ---------------------------------------------------------------------------
int __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  // common.cpp:538
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFile->ReadInteger(Group, Name, Value);             // common.cpp:548
}

// ---------------------------------------------------------------------------
//  ReadIniData(AnsiString) (common.cpp:552-564)
//  No empty-string repair here (unlike CheckAndReadIniData(AnsiString)).
// ---------------------------------------------------------------------------
AnsiString __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:552
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Read NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFile->ReadString(Group, Name, Value);              // common.cpp:562
}

// ===========================================================================
//  ReadIniDataMem overloads (common.cpp:566-620)
//  Identical to ReadIniData but via OpenIniFileMem / INIFileMem.
//  RogerYang 20260214
// ===========================================================================

// ---------------------------------------------------------------------------
//  ReadIniDataMem(bool) (common.cpp:566-578)
// ---------------------------------------------------------------------------
bool __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  // common.cpp:566
{
    AnsiString Str;
    if (OpenIniFileMem(FileName) == false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return bValue;
    }
    return INIFileMem->ReadBool(Group, Name, bValue);            // common.cpp:576
}

// ---------------------------------------------------------------------------
//  ReadIniDataMem(double) (common.cpp:580-592)
// ---------------------------------------------------------------------------
double __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  // common.cpp:580
{
    AnsiString Str;
    if (OpenIniFileMem(FileName) == false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFileMem->ReadFloat(Group, Name, Value);            // common.cpp:590
}

// ---------------------------------------------------------------------------
//  ReadIniDataMem(int) (common.cpp:594-606)
// ---------------------------------------------------------------------------
int __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  // common.cpp:594
{
    AnsiString Str;
    if (OpenIniFileMem(FileName) == false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFileMem->ReadInteger(Group, Name, Value);          // common.cpp:604
}

// ---------------------------------------------------------------------------
//  ReadIniDataMem(AnsiString) (common.cpp:608-620)
// ---------------------------------------------------------------------------
AnsiString __fastcall ReadIniDataMem(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:608
{
    AnsiString Str;
    if (OpenIniFileMem(FileName) == false)
    {
        Str.sprintf("Read NULL INI_Mem on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return Value;
    }
    return INIFileMem->ReadString(Group, Name, Value);           // common.cpp:618
}

// ===========================================================================
//  WriteIniData overloads (common.cpp:622-1120)
//  INI-CORE only translated this wave.
//  Change-log side-effects (TempChangeLog / RecordChangeLogProcess /
//  FormHS->RecordChangeLogByLot / ConvertToMMType / UI deref fContact /
//  FTestIF / fCleaning + globals InitialOK / bSysLotStart /
//  CosFunction.bUseChangeLogByLot) are gated with
//    #if 0 // TODO(wave-logging/UI)
//  The ini-core (Read-old / try{Write*} catch RecordProcess) is active.
// ===========================================================================

// ---------------------------------------------------------------------------
//  WriteIniData(bool) (common.cpp:622-689)
//  Core: ReadBool old value; try{ WriteBool } catch.
//  Change-log block gated (InitialOK/TempChangeLog/RecordChangeLogProcess/
//  FormHS->RecordChangeLogByLot / fCleaning UI deref).
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  // common.cpp:622
{
    AnsiString Str, Str1, Str2;
    bool ret;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    ret = INIFile->ReadBool(Group, Name, bValue);               // common.cpp:636

#if 0 // TODO(wave-logging/UI): change-log block (common.cpp:637-688)
    // if (ret != bValue && InitialOK == true) {
    //     ... TempChangeLog / RecordChangeLogProcess / fCleaning UI / RecordChangeLogByLot
    // }
#endif // TODO(wave-logging/UI)

    try
    {
        INIFile->WriteBool(Group, Name, bValue);                 // common.cpp:675
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }

#if 0 // TODO(wave-logging/UI): RecordChangeLogByLot (common.cpp:683-688)
    // if (CosFunction.bUseChangeLogByLot && bSysLotStart && bHasChange)
    //     FormHS->RecordChangeLogByLot(Str1, Str2);
#endif // TODO(wave-logging/UI)

    (void)ret;   // suppress unused-variable warning while change-log is gated
    (void)Str2;
}

// ---------------------------------------------------------------------------
//  WriteIniData(int) (common.cpp:691-873)
//  Core: ReadInteger old value; try{ WriteInteger } catch.
//  Change-log block gated (large UI-coupled block + ConvertToMMType).
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  // common.cpp:691
{
    AnsiString Str, Str1, Str2;
    int ret;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    ret = INIFile->ReadInteger(Group, Name, Value);              // common.cpp:705

#if 0 // TODO(wave-logging/UI): change-log block (common.cpp:706-855)
    // if (ret != Value && InitialOK == true) {
    //     ... TempChangeLog / fContact->cbContactMode->Items / FTestIF->rgInterfaceType->Items
    //     ... fCleaning->rgAutoCleanSelectArm->Items / ConvertToMMType / RecordChangeLogProcess
    // }
#endif // TODO(wave-logging/UI)

    try                                                          // common.cpp:857
    {
        INIFile->WriteInteger(Group, Name, Value);
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }

#if 0 // TODO(wave-logging/UI): RecordChangeLogByLot (common.cpp:867-872)
#endif // TODO(wave-logging/UI)

    (void)ret; (void)Str2;
}

// ---------------------------------------------------------------------------
//  WriteIniData(double) (common.cpp:875-972)
//  Core: sprintf "%0.4f" -> Str; ReadFloat old; try{ WriteString(Str) } catch.
//  SUBTLETY (common.cpp:891): BCB6 compares `ret` (double) with `Str`
//  (AnsiString) -- BCB6 resolves via AnsiString implicit cast from double
//  (effectively comparing the 4-decimal formatted string of ret with Str).
//  The change-log check is fully gated, so only the ini-core (WriteString)
//  matters here.
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  // common.cpp:875
{
    AnsiString Str, Str1, Str2;
    double ret;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }
    Str.sprintf("%0.4f", Value);                                 // common.cpp:888

    ret = INIFile->ReadFloat(Group, Name, Value);                // common.cpp:890

#if 0 // TODO(wave-logging/UI): change-log block (common.cpp:891-954)
    // if (ret != Str && InitialOK == true) { ... ConvertToMMType / RecordChangeLogProcess }
    // NOTE: `ret != Str` in BCB6 coerces Str to double via AnsiString operator double
    // (effectively sprintf("%0.4f", ret) != Str, i.e. the stored value differs from new).
#endif // TODO(wave-logging/UI)

    try                                                          // common.cpp:956
    {
        INIFile->WriteString(Group, Name, Str);                  // common.cpp:958 -- NOT WriteFloat
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }

#if 0 // TODO(wave-logging/UI): RecordChangeLogByLot (common.cpp:966-971)
#endif // TODO(wave-logging/UI)

    (void)ret; (void)Str2;
}

// ---------------------------------------------------------------------------
//  WriteIniData(unsigned long) (common.cpp:974-1019)
//  Core: ReadInteger old; try{ WriteInteger } catch.
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)  // common.cpp:974
{
    AnsiString Str, Str1, Str2;
    unsigned long ret;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    ret = static_cast<unsigned long>(INIFile->ReadInteger(Group, Name, static_cast<int>(Value)));  // common.cpp:987

#if 0 // TODO(wave-logging/UI): change-log block (common.cpp:988-1001)
    // if (ret != Value && InitialOK == true) { TempChangeLog / RecordChangeLogProcess }
#endif // TODO(wave-logging/UI)

    try                                                          // common.cpp:1003
    {
        INIFile->WriteInteger(Group, Name, static_cast<int>(Value));  // common.cpp:1005
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }

#if 0 // TODO(wave-logging/UI): RecordChangeLogByLot (common.cpp:1013-1018)
#endif // TODO(wave-logging/UI)

    (void)ret; (void)Str2;
}

// ---------------------------------------------------------------------------
//  WriteIniData(AnsiString) (common.cpp:1021-1099)
//  Core: ReadString old; TryStrToFloat to decide numeric vs string compare;
//  try{ WriteString } catch.
//  CHANGE-DETECT: if both old/new parse as float -> compare atof() != atof();
//  else string compare ret != Value (common.cpp:1057-1058).
//  Change-log action (RecordChangeLogProcess / FormHS) is gated; the detect
//  logic is also gated since all its outputs feed only the log block.
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:1021
{
    AnsiString Str, Str1, Str2;
    AnsiString ret;

#if 0 // TODO(wave-logging/UI): change-log variables (common.cpp:1025-1029)
    bool bStrIsFloat1 = false;
    bool bStrIsFloat2 = false;
    double freg       = 0;
#endif // TODO(wave-logging/UI)

    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    ret = INIFile->ReadString(Group, Name, Value);               // common.cpp:1037

#if 0 // TODO(wave-logging/UI): float-detect change-log block (common.cpp:1053-1081)
    // bStrIsFloat1 = TryStrToFloat(ret.c_str(), freg);
    // bStrIsFloat2 = TryStrToFloat(Value.c_str(), freg);
    // if (InitialOK == true) {
    //     if ((bStrIsFloat1 && bStrIsFloat2 && atof(ret.c_str()) != atof(Value.c_str())) ||
    //         (!bStrIsFloat1 && !bStrIsFloat2 && ret != Value)) {
    //         ... TempChangeLog / RecordChangeLogProcess
    //     }
    // }
#endif // TODO(wave-logging/UI)

    try                                                          // common.cpp:1083
    {
        INIFile->WriteString(Group, Name, Value);                // common.cpp:1085
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }

#if 0 // TODO(wave-logging/UI): RecordChangeLogByLot (common.cpp:1093-1098)
#endif // TODO(wave-logging/UI)

    (void)ret; (void)Str2;
}

// ---------------------------------------------------------------------------
//  WriteIniData(TDateTime) (common.cpp:1101-1120)
//  No change-log.  try{ WriteDateTime } catch.
// ---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)  // common.cpp:1101
{
    AnsiString Str, Str1;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    try                                                          // common.cpp:1111
    {
        INIFile->WriteDateTime(Group, Name, Value);              // common.cpp:1113
    }
    catch (...)
    {
        Str1.sprintf("WriteIniData:%s", FileName.c_str());
        RecordProcess("Exception", Str1);
    }
}

// ===========================================================================
//  WriteIniDataNoLog overloads (common.cpp:1122-1184)
//  No change-log, no RecordChangeLog.  Open -> Write inside try/catch.
//  Steven 20200526 : Add Write ini without event log
// ===========================================================================

// ---------------------------------------------------------------------------
//  WriteIniDataNoLog(int) (common.cpp:1122-1141)
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  // common.cpp:1122
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    try                                                          // common.cpp:1132
    {
        INIFile->WriteInteger(Group, Name, Value);               // common.cpp:1134
    }
    catch (...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName.c_str());
        RecordProcess("Exception", Str);
    }
}

// ---------------------------------------------------------------------------
//  WriteIniDataNoLog(AnsiString) (common.cpp:1143-1162)
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:1143
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }

    try                                                          // common.cpp:1153
    {
        INIFile->WriteString(Group, Name, Value);                // common.cpp:1155
    }
    catch (...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName.c_str());
        RecordProcess("Exception", Str);
    }
}

// ---------------------------------------------------------------------------
//  WriteIniDataNoLog(double) (common.cpp:1164-1184)
//  sprintf "%0.4f" -> Str; WriteString (NOT WriteFloat).
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataNoLog(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  // common.cpp:1164
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Write NULL INI on [%s] %s", Group.c_str(), Name.c_str());
        RecordProcess(Str);
        return;
    }
    Str.sprintf("%0.4f", Value);                                 // common.cpp:1173

    try
    {
        INIFile->WriteString(Group, Name, Str);                  // common.cpp:1177
    }
    catch (...)
    {
        Str.sprintf("WriteIniDataNoLog:%s", FileName.c_str());
        RecordProcess("Exception", Str);
    }
}

// ===========================================================================
//  WriteIniData1 (common.cpp:1186-1237)
//  kevin 20220623
//  Like WriteIniData(AnsiString) but:
//    * open-fail returns SILENTLY (no RecordProcess) (common.cpp:1194-1196)
//    * builds change strings but does NOT call RecordChangeLogProcess
//    * does NOT call RecordChangeLogByLot
//  The change-detection block is gated (it only feeds the discarded strings).
// ===========================================================================
void __fastcall WriteIniData1(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:1186
{
    AnsiString Str, Str1, Str2;
    AnsiString ret;

    if (OpenIniFile(FileName) == false)                          // common.cpp:1194
    {
        return;                                                  // common.cpp:1196 -- silent, no RecordProcess
    }

    ret = INIFile->ReadString(Group, Name, Value);               // common.cpp:1199

#if 0 // TODO(wave-logging/UI): change-detect block (common.cpp:1200-1225)
    // bool bStrIsFloat1 = false, bStrIsFloat2 = false;
    // double freg = 0;
    // bStrIsFloat1 = TryStrToFloat(ret.c_str(), freg);
    // bStrIsFloat2 = TryStrToFloat(Value.c_str(), freg);
    // if (InitialOK == true) {
    //     if ((bStrIsFloat1 && bStrIsFloat2 && atof(ret.c_str()) != atof(Value.c_str())) ||
    //         (!bStrIsFloat1 && !bStrIsFloat2 && ret != Value)) {
    //         StrChangeName = TempChangeLog(Group, Name);
    //         if (FileName.Pos("Offset") == 0)
    //             Str1.sprintf("%s_%s change Value", Group, StrChangeName);
    //         else
    //             Str1.sprintf("%s_%s Offset change Value", Group, StrChangeName);
    //         if (FileName.Pos("Contact.Data") != 0 && Group == "Mode" &&
    //             Name == "fSocketInitialICCheckPositionOffset")
    //             Str2.sprintf("%0.2fmm ==> %0.2fmm", atof(ret.c_str()), atof(Value.c_str()));
    //         else
    //             Str2.sprintf("%s==>%s", ret, Value);
    //         // NOTE: unlike WriteIniData(AnsiString), NO RecordChangeLogProcess call here.
    //     }
    // }
#endif // TODO(wave-logging/UI)

    try
    {
        INIFile->WriteString(Group, Name, Value);                // common.cpp:1230
    }
    catch (...)
    {
        Str.sprintf("WriteIniData1:%s", FileName.c_str());
        RecordProcess("Exception", Str);
    }

    (void)ret; (void)Str1; (void)Str2;
}

// ===========================================================================
//  ReplaceIniData (common.cpp:1239-1250)
//  Copies the stored value of key Name2 into key Name1.
// ===========================================================================
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2)  // common.cpp:1239
{
    AnsiString Str;
    if (OpenIniFile(FileName) == false)
    {
        Str.sprintf("Replace NULL INI on [%s] %s", Group.c_str(), Name1.c_str());
        RecordProcess(Str);
        return;
    }
    AnsiString ret = INIFile->ReadString(Group, Name2, " ");     // common.cpp:1248 -- default single space
    INIFile->WriteString(Group, Name1, ret);                     // common.cpp:1249
}

// ===========================================================================
//  GetLastOpenFN (common.cpp:1252-1281)
// ===========================================================================
// AI(W906-CommonWaveFile) 20260721: un-gated -- TStringList::LoadFromFile /
// FileExists are already-active vclcompat primitives; ShowMyMessage resolves
// to the local no-op forward-stub declared near RecordProcess above.
AnsiString __fastcall GetLastOpenFN()                                        // common.cpp:1252
{
    AnsiString Str="Fail Open";
    TStringList *MyList= new TStringList();

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        if(MyList->Count==0)                                                 // Jimmychiu 20250426 : content of setup is empty
        {
            ShowMyMessage("The content of setup.inf is empty!");
        }
        else if(AnsiString(MyList->Strings[0]).Trim()=="")                   // StringsProxy has no Trim() -- explicit AnsiString cast (established idiom, e.g. Automation/auto9045.cpp:84)
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
    MyList->Clear();                                                         // Ifor 20170603 : clear TStringList before delete
    delete MyList;

    return Str;
}

// ===========================================================================
//  CheckSectionExist / CheckKeyExist (common.cpp:1283-1309)
// ===========================================================================

// ---------------------------------------------------------------------------
//  CheckSectionExist (common.cpp:1283-1295)
//  Returns false if Group=="" || FileName=="".
// ---------------------------------------------------------------------------
bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group)  // common.cpp:1283
{
    if (Group == "" || FileName == "")                           // common.cpp:1285
        return false;

    if (OpenIniFile(FileName) == false)
        return false;

    return INIFile->SectionExists(Group);                        // common.cpp:1293
}

// ---------------------------------------------------------------------------
//  CheckKeyExist (common.cpp:1297-1309)
//  Returns false if Group=="" || FileName=="".
// ---------------------------------------------------------------------------
bool __fastcall CheckKeyExist(AnsiString FileName, AnsiString Group, AnsiString Name)  // common.cpp:1297
{
    if (Group == "" || FileName == "")                           // common.cpp:1299
        return false;

    if (OpenIniFile(FileName) == false)
        return false;

    return INIFile->ValueExists(Group, Name);                    // common.cpp:1307
}

// ===========================================================================
//  WriteLastDataFN (common.cpp:1311-1331)
// ===========================================================================
// AI(W906-CommonWaveFile) 20260721: un-gated. Golden's bootstrap (else,
// LastDataPath does not yet exist) branch used raw BCB6 int-handle
// FileCreate/FileWrite/FileClose (common.cpp:1325-1327) -- no vclcompat shim
// for that trio exists (confirmed via grep across this tree), and adding one
// is deliberately out of scope this wave (avoid growing new Win32 shim
// surface for a single bootstrap call). DELIBERATE SUBSTITUTION: reuse the
// same TStringList Add()+SaveToFile() path as the if-branch instead --
// functionally equivalent to golden's "create the file with SName as its
// sole line" bootstrap behavior. `iFileHandle` (golden's local var for the
// removed FileCreate/FileWrite/FileClose calls) is dropped as unused.
void __fastcall WriteLastDataFN(AnsiString SName)                            // common.cpp:1311
{
    TStringList *MyList= new TStringList();

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        MyList->Clear();
        MyList->Add(SName);
        MyList->SaveToFile(LastDataPath);
    }
    else
    {
        MyList->Add(SName);
        MyList->SaveToFile(LastDataPath);
    }
    MyList->Clear();                                                         // Ifor 20170603 : clear TStringList before delete
    delete MyList;
}

// ===========================================================================
//  GATED: AddSpace (common.cpp:1333-1336) -- body is a no-op return; still
//  gated to keep the header clean for this wave (it depends on nothing but
//  the signature is UI-flavoured).
// ===========================================================================
#if 0 // TODO(wave-ui-input): AddSpace (common.cpp:1333)
void __fastcall AddSpace(AnsiString /*File*/) { return; }
#endif // TODO(wave-ui-input)

// ===========================================================================
//  GATED: OnlyNumberInPut / OnlyNumberAndDotInPut / OnlyMakeFileDataInPut
//  (common.cpp:1338-1358) -- key-filter helpers; pure logic, no deps.
//  WAVE: ui-input (tiny, could pull early if needed).
// ===========================================================================
#if 0 // TODO(wave-ui-input): keyboard filter helpers (common.cpp:1338-1358)
bool __fastcall OnlyNumberInPut(int iKey)          { return (iKey>='0'&&iKey<='9')||iKey==8||iKey=='-'; }
bool __fastcall OnlyNumberAndDotInPut(int iKey)    { return (iKey>='0'&&iKey<='9')||iKey=='.'||iKey==8||iKey=='-'; }
bool __fastcall OnlyMakeFileDataInPut(int iKey)    { return !(iKey=='\''||iKey=='/'||iKey==':'||iKey=='*'||iKey=='?'||iKey=='"'||iKey=='<'||iKey=='|'); }
#endif // TODO(wave-ui-input)

// ===========================================================================
//  GATED: MyDrawText overloads (common.cpp:1360-1404)
//  WAVE: canvas/UI -- TCanvas / TRect / TColor VCL GDI types.
// ===========================================================================
#if 0 // TODO(wave-canvas): MyDrawText x6 (common.cpp:1360-1404)
#endif // TODO(wave-canvas)

// ===========================================================================
//  OpenGeneralIniFile / CloseGeneralIniFile (common.cpp:1408-1421)
// ===========================================================================

// ---------------------------------------------------------------------------
//  OpenGeneralIniFile (common.cpp:1408-1412)
//  UNCONDITIONALLY calls CloseGeneralIniFile() then creates a new TIniFile
//  bound to asGeneralPath.  No caching guard, no return value.
//  Steven 20141120 : Add Read/Write IniFile Speed
// ---------------------------------------------------------------------------
void OpenGeneralIniFile()                                        // common.cpp:1408
{
    CloseGeneralIniFile();
    INIFileGeneral = new TIniFile(asGeneralPath);
}

// ---------------------------------------------------------------------------
//  CloseGeneralIniFile (common.cpp:1414-1421)
//  FAITHFUL BUG: INIFileGeneral NOT set to nullptr after delete.
// ---------------------------------------------------------------------------
void CloseGeneralIniFile()                                       // common.cpp:1414
{
    if (INIFileGeneral != nullptr)
    {
        INIFileGeneral->UpdateFile();
        delete INIFileGeneral;
        // NOTE: INIFileGeneral intentionally NOT set to nullptr -- faithful bug.
    }
}

// ===========================================================================
//  CheckAndReadIniDataGeneral overloads (common.cpp:1423-1464)
//  Operate DIRECTLY on global INIFileGeneral -- NO open/close guard, NO NULL
//  check.  Caller MUST have called OpenGeneralIniFile() first; otherwise NULL
//  deref is faithful BCB6 behaviour.
// ===========================================================================

// ---------------------------------------------------------------------------
//  CheckAndReadIniDataGeneral(AnsiString) (common.cpp:1423-1430)
// ---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:1423
{
    if (!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteString(Group, Name, Value);         // common.cpp:1426 -- seed
    else
        Value = INIFileGeneral->ReadString(Group, Name, Value);  // common.cpp:1428
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniDataGeneral(double) (common.cpp:1432-1442)
//  Seed via WriteString (NOT WriteFloat), 4-decimal text.
// ---------------------------------------------------------------------------
double __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value)  // common.cpp:1432
{
    AnsiString Str;
    Str.sprintf("%0.4f", Value);                                 // common.cpp:1435

    if (!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteString(Group, Name, Str);           // common.cpp:1438 -- seed as 4-decimal text
    else
        Value = INIFileGeneral->ReadFloat(Group, Name, Value);   // common.cpp:1440
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniDataGeneral(int) (common.cpp:1444-1455)
// ---------------------------------------------------------------------------
int __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value)  // common.cpp:1444
{
    if (!INIFileGeneral->ValueExists(Group, Name))
    {
        INIFileGeneral->WriteInteger(Group, Name, Value);        // common.cpp:1448 -- seed
    }
    else
    {
        Value = INIFileGeneral->ReadInteger(Group, Name, Value); // common.cpp:1452
    }
    return Value;
}

// ---------------------------------------------------------------------------
//  CheckAndReadIniDataGeneral(bool) (common.cpp:1457-1464)
// ---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value)  // common.cpp:1457
{
    if (!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteBool(Group, Name, Value);           // common.cpp:1460 -- seed
    else
        Value = INIFileGeneral->ReadBool(Group, Name, Value);    // common.cpp:1462
    return Value;
}

// ===========================================================================
//  WriteIniDataGeneral overloads (common.cpp:1466-1487)
//  Operate directly on INIFileGeneral.  No guard, no log.
//  Steven 20090731
// ===========================================================================

// ---------------------------------------------------------------------------
//  WriteIniDataGeneral(bool) (common.cpp:1466-1469)
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue)  // common.cpp:1466
{
    INIFileGeneral->WriteBool(Group, Name, bValue);
}

// ---------------------------------------------------------------------------
//  WriteIniDataGeneral(int) (common.cpp:1471-1474)
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value)  // common.cpp:1471
{
    INIFileGeneral->WriteInteger(Group, Name, Value);
}

// ---------------------------------------------------------------------------
//  WriteIniDataGeneral(double) (common.cpp:1476-1482)
//  sprintf "%0.4f" -> WriteString (NOT WriteFloat). Steven 20150723.
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value)  // common.cpp:1476
{
    AnsiString Str;
    Str.sprintf("%0.4f", Value);                                 // common.cpp:1479
    INIFileGeneral->WriteString(Group, Name, Str);
}

// ---------------------------------------------------------------------------
//  WriteIniDataGeneral(AnsiString) (common.cpp:1484-1487)
// ---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value)  // common.cpp:1484
{
    INIFileGeneral->WriteString(Group, Name, Value);
}

// ===========================================================================
//  ReadWriteIni overloads (common.cpp:1489-1605)
//  Unified read-or-write facade.
//  bIsRead==true:  Value = CheckAndReadIniData(FileName,Group,Name,DefaultValue)
//                  [NOTE: DefaultValue, not Value, is the seed/default arg]
//                  then clamp via CheckRange if bCheckRange.
//  bIsRead==false: clamp Value via CheckRange if bCheckRange;
//                  WriteIniData(FileName,Group,Name,clamped-Value).
//  ASYMMETRY (double/unsigned long): extra pre-clamp of Value before the
//  if(bIsRead) branch (common.cpp:1564-1565, 1587-1588).
//
//  Depends on: CheckAndReadIniData / WriteIniData (this batch) +
//              CheckRange (MachineType.h template, already in scope).
// ===========================================================================

// ---------------------------------------------------------------------------
//  ReadWriteIni(bool) (common.cpp:1489-1503)
//  No bCheckRange / Max / Min params (common.h:52 signature).
// ---------------------------------------------------------------------------
bool __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value, bool DefaultValue, bool bIsRead)  // common.cpp:1489
{
    bool tValue = Value;

    if (bIsRead)
    {
        Value  = CheckAndReadIniData(FileName, Group, Name, DefaultValue);  // common.cpp:1495
        tValue = Value;
    }
    else
    {
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1500
    }
    return tValue;
}

// ---------------------------------------------------------------------------
//  ReadWriteIni(AnsiString) (common.cpp:1505-1518)
//  No bCheckRange / Max / Min params (common.h:51 signature).
// ---------------------------------------------------------------------------
AnsiString __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value, AnsiString DefaultValue, bool bIsRead)  // common.cpp:1505
{
    AnsiString tValue = Value;

    if (bIsRead)
    {
        Value  = CheckAndReadIniData(FileName, Group, Name, DefaultValue);  // common.cpp:1510
        tValue = Value;
    }
    else
    {
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1515
    }
    return tValue;
}

// ---------------------------------------------------------------------------
//  ReadWriteIni(TDateTime) (common.cpp:1520-1538)
// ---------------------------------------------------------------------------
TDateTime __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value, TDateTime DefaultValue, bool bIsRead, bool bCheckRange, TDateTime Maximum, TDateTime Minimum)  // common.cpp:1520
{
    TDateTime tValue = Value;

    if (bIsRead)
    {
        Value = CheckAndReadIniData(FileName, Group, Name, DefaultValue);  // common.cpp:1525
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1527
        else
            tValue = Value;
    }
    else
    {
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1534
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1535
    }
    return tValue;
}

// ---------------------------------------------------------------------------
//  ReadWriteIni(int) (common.cpp:1540-1559)
// ---------------------------------------------------------------------------
int __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, int Value, int DefaultValue, bool bIsRead, bool bCheckRange, int Maximum, int Minimum)  // common.cpp:1540
{
    int tValue = Value;

    if (bIsRead)                                                 // common.cpp:1544
    {
        Value = CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1548
        else
            tValue = Value;
    }
    else
    {
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1555
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1556
    }
    return tValue;
}

// ---------------------------------------------------------------------------
//  ReadWriteIni(double) (common.cpp:1561-1582)
//  ASYMMETRY: extra pre-clamp before if(bIsRead) (common.cpp:1564-1565).
// ---------------------------------------------------------------------------
double __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, double Value, double DefaultValue, bool bIsRead, bool bCheckRange, double Maximum, double Minimum)  // common.cpp:1561
{
    double tValue = Value;
    if (bCheckRange)
        Value = CheckRange(Value, Maximum, Minimum);             // common.cpp:1564-1565 -- pre-clamp

    if (bIsRead)
    {
        Value = CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1571
        else
            tValue = Value;
    }
    else
    {
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1578
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1579
    }
    return tValue;
}

// ---------------------------------------------------------------------------
//  ReadWriteIni(unsigned long) (common.cpp:1584-1605)
//  ASYMMETRY: extra pre-clamp before if(bIsRead) (common.cpp:1587-1588).
// ---------------------------------------------------------------------------
unsigned long __fastcall ReadWriteIni(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value, unsigned long DefaultValue, bool bIsRead, bool bCheckRange, unsigned long Maximum, unsigned long Minimum)  // common.cpp:1584
{
    unsigned long tValue = Value;
    if (bCheckRange)
        Value = CheckRange(Value, Maximum, Minimum);             // common.cpp:1587-1588 -- pre-clamp

    if (bIsRead)
    {
        Value = CheckAndReadIniData(FileName, Group, Name, DefaultValue);
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1593
        else
            tValue = Value;
    }
    else
    {
        if (bCheckRange)
            tValue = CheckRange(Value, Maximum, Minimum);        // common.cpp:1600
        WriteIniData(FileName, Group, Name, tValue);             // common.cpp:1601
    }
    return tValue;
}

// ===========================================================================
//  GATED: most remaining common.cpp regions (lines 1607-end) -- EXCEPT the
//  wave-file cluster immediately below, un-gated this wave (AI(W906-
//  CommonWaveFile) 20260721).
// ===========================================================================

// ===========================================================================
//  WriteDataToFile x2 / CheckFileIsEmpty / ReadDataFromFile / MyForceDirectories
//  (common.cpp:1607-1721)
// ===========================================================================
// AI(W906-CommonWaveFile) 20260721: un-gated -- all five functions translated
// below. See each function's own note for golden bugs/quirks preserved
// verbatim.

// ---------------------------------------------------------------------------
//  WriteDataToFile(char*,char*,bool) (common.cpp:1607-1625)
//  Pure CRT: fopen(mode "w"/"a") -> fputs(cData) -> fputs("\n") -> fclose.
//  Silent no-op if fopen fails (pFile==NULL) -- no error signalled to caller.
//  DEVIATION from golden's literal `char*` params: `const char*` here (see
//  common.h's declaration-site note for why -- fixes a real infinite-
//  recursion bug found via a build+gdb backtrace, not a stylistic choice).
// ---------------------------------------------------------------------------
void WriteDataToFile(const char* cFilePath, const char* cData, bool bOverWrite) // common.cpp:1607
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

// ---------------------------------------------------------------------------
//  CheckFileIsEmpty(AnsiString) (common.cpp:1627-1643)
//  GOLDEN BUG, preserved verbatim: fclose(pFile) is called UNCONDITIONALLY,
//  even when pFile==NULL (fopen failed) -- passing a NULL FILE* to fclose is
//  UB in ISO C. Do NOT add a NULL guard; this is golden's actual behavior.
//
//  GOLDEN BUG (found and VERIFIED empirically this wave via tests/
//  test_common.cpp -- NOT just a static reading; the naive expectation below
//  was wrong the first time round too), preserved verbatim: the return value
//  is effectively INVERTED from what the function's name suggests. `bResult`
//  starts `true` and is only ever forced to `false` in the one case where the
//  file opens AND the very first fgetc() immediately hits EOF. The three
//  real outcomes are:
//    - file does not exist            -> returns true
//    - file exists, has content       -> returns true
//    - file exists, genuinely empty   -> returns false
//  In other words: true means "missing OR non-empty", false means "exists
//  and is empty" -- the opposite of an intuitive "IsEmpty" predicate for two
//  of the three cases. Do not "fix" this; callers elsewhere in golden
//  presumably already accommodate (or never depend on) this inversion.
// ---------------------------------------------------------------------------
bool CheckFileIsEmpty(AnsiString cFilePath)                                  // common.cpp:1627
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
    fclose(pFile);                                                          // golden bug: unconditional, even if pFile==NULL
    return bResult;
}

// ---------------------------------------------------------------------------
//  WriteDataToFile(AnsiString,AnsiString,bool) (common.cpp:1645-1648)
//  1-line forwarder to the char* overload via .c_str().
// ---------------------------------------------------------------------------
void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite) // common.cpp:1645
{
    WriteDataToFile(cFilePath.c_str(), cData.c_str(), bOverWrite);
}

// ---------------------------------------------------------------------------
//  ReadDataFromFile(AnsiString) (common.cpp:1650-1665)
//  OWNERSHIP CONTRACT (golden, preserved verbatim -- this is a deliberate
//  C-style raw pointer hand-off, not a bug to "modernize" away): returns a
//  raw malloc()'d buffer on success; the CALLER is responsible for free()'ing
//  it. Signature kept exactly as golden has it (raw char*/malloc, no smart
//  pointer, no signature change).
//
//  GOLDEN BUG (found and VERIFIED empirically this wave via a standalone gdb/
//  diagnostic repro, not just inferred): `fopen(...,"r")` here is TEXT mode
//  (golden's literal mode string, no "b"), so on Windows the CRLF<->LF
//  translation applies. `file_size` is the ON-DISK byte count (from ftell()),
//  used as (a) the malloc() size, (b) the fread() request size, AND (c) the
//  index where the forced '\0' terminator is written. A text-mode fread()
//  DELIVERS FEWER bytes than requested whenever the file contains any "\r\n"
//  (each collapses to one delivered "\n"), stopping at EOF -- so for a file
//  with N embedded newlines, exactly N bytes at the TAIL of the malloc'd
//  buffer (immediately before the forced '\0') are LEFT UNINITIALIZED
//  (whatever malloc() happened to hand back), not zeroed and not part of the
//  real file content. Any file written by WriteDataToFile ALWAYS has this
//  problem, since it unconditionally appends one "\n" per call. Preserved
//  verbatim (golden's fopen mode strings are exactly "r"/"w"/"a", no "b" --
//  see also WriteDataToFile above); documented here rather than silently
//  switched to binary mode. Callers must not assume the returned buffer's
//  tail bytes (near the end, before the final NUL) are meaningful when the
//  source file has embedded newlines -- see tests/test_common.cpp for a
//  reproduction and the safe (prefix-only) way to check round-tripped content.
// ---------------------------------------------------------------------------
char* ReadDataFromFile(AnsiString cFilePath)                                 // common.cpp:1650
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

// ---------------------------------------------------------------------------
//  MyForceDirectories(AnsiString,AnsiString) (common.cpp:1667-1721)
//  GOLDEN QUIRK, preserved verbatim: the directory-vs-filename auto-detect
//  only strips a trailing filename component for exactly ".txt"/".csv"
//  extensions (hardcoded) when the path does not exist and has no trailing
//  '\\' -- any other extension is (mis)treated as a directory name to create.
//  catch(Exception&)/catch(...) MERGED into a single generic catch(...) --
//  there is no vclcompat Exception base class to catch. Follows the
//  established project precedent in Interface/TesterTCP.cpp:79
//  (Gated_MyForceDirectories) and its sibling copy in
//  Interface/TesterTCP_Socket.cpp. Documented accepted behavior-diff: loses
//  golden's distinct return-code/e.Message text split (-2 with e.Message vs
//  -3 with a fixed string) -- both call sites currently in this project
//  discard the return value, so this is harmless today; -3 with the fixed
//  message is returned uniformly instead.
// ---------------------------------------------------------------------------
int MyForceDirectories(AnsiString Directory, AnsiString Function)            // common.cpp:1667
{
    AnsiString Str;
    if(Directory=="")
    {
        RecordProcess("Directory value is NULL!", Function);
        return -1;
    }

    if(Directory[Directory.Length()]!='\\')
    {
        DWORD attr=GetFileAttributes(Directory.c_str());
        if(attr!=INVALID_FILE_ATTRIBUTES)
        {
            if(!(attr & FILE_ATTRIBUTE_DIRECTORY))
                Directory=ExtractFilePath(Directory);                       // path exists: strip filename only if it's a file
        }
        else
        {
            AnsiString sExt=ExtractFileExt(Directory).LowerCase();          // only these 2 known file extensions get stripped (golden quirk, see banner above)
            if(sExt==".txt" || sExt==".csv")
                Directory=ExtractFilePath(Directory);
        }
    }

    if(Directory=="")
    {
        RecordProcess("Directory value is NULL!", Function);
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
        catch(...)
        {
            Str.sprintf("%s -- %s", Directory, Function);
            ShowMyMessage("Create directory fail!", Str, Str);
            return -3;
        }
    }

    return 1;
}

#if 0 // TODO(wave-timing): MyTickCount / MySleepEx / MySleep (common.cpp:1726-1801)
#endif // TODO(wave-timing)

#if 0 // TODO(wave-logging): TempChangeLog (common.cpp:1802)
#endif // TODO(wave-logging)

// AI(W906-CommonWaveFile) 20260721: GetLastOpenFN / WriteLastDataFN un-gated
// above (common.cpp:1252-1331 real bodies now live earlier in this file) --
// dropped from this index placeholder. GetRecipePath / GetRecipeFileName /
// ChangeSaveFileName remain gated (still out of scope; common.cpp:2039-2130).
#if 0 // TODO(wave-path): GetRecipePath / GetRecipeFileName / ChangeSaveFileName (common.cpp:2039-2130)
#endif // TODO(wave-path)

#if 0 // TODO(wave-grid): SGDToCSV (common.cpp:2050)
#endif // TODO(wave-grid)

#if 0 // TODO(wave-file): IsFileInUse / CopyAndCompressFile (common.cpp:2066-2120)
#endif // TODO(wave-file)

#if 0 // TODO(wave-datetime): MySecondsBetween (common.cpp:2188)
#endif // TODO(wave-datetime)
