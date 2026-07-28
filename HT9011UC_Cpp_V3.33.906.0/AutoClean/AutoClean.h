// =============================================================================
//  AutoClean/AutoClean.h  --  W906-AutoCleanFoundation + W906-AutoCleanCluster waves
//
//  Translation wave: W906-AutoCleanFoundation (foundation, 20260721) followed by
//  W906-AutoCleanCluster (20260722, the FIRST of two natural dependency
//  clusters covering golden's remaining 19 absent functions) followed by the
//  SECOND (and final) cluster (this wave, 20260728): DoIndexAutoClean +
//  DoIndexAutoClean_Arm1PickArm2Test (golden AutoClean.cpp:6465-9106). Every
//  function in golden AutoClean.cpp/.h is now translated -- no further
//  AutoClean-cluster waves remain.
//  Translator: AI(W906-AutoCleanFoundation) 20260721 / AI(W906-AutoCleanCluster) 20260722
//              / AI(W906-AutoCleanCluster) 20260728 (DoIndexAutoClean cluster)
//
//  Golden: D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\AutoClean\AutoClean.h/.cpp
//  Mirrors golden's own file layout (golden keeps ALL of AutoClean.cpp/.h in one
//  AutoClean/ subfolder rather than per-function files) -- this wave creates
//  that same AutoClean/ subfolder in the target tree for the first time.
//
//  SCOPE (W906-AutoCleanCluster, 20260722): 9 small helpers + the 4 named core
//  pick/place engines (DoAutoCleanPickfromCleanKit / DoPlaceToShuttle /
//  DoPickFromShuttle / DoAutoCleanPlaceToCleanKit) + the 3 shuttle-clean state
//  machines (DoShuttle1AutoClean(+_Arm1PickArm2Test variant) / DoShuttle2AutoClean)
//  + the master orchestrator DoAutoCleanKit.
//
//  SCOPE (this wave, 20260728): DoIndexAutoClean_Arm1PickArm2Test + DoIndexAutoClean
//  (golden AutoClean.cpp:6465-9106, 2642 lines) -- REPLACES the TEMPORARY
//  placeholder stub that used to sit near the top of AutoClean.cpp (an empty
//  `void DoIndexAutoClean(){}`; see git history). Both are now real, faithful
//  translations.
// =============================================================================
#ifndef AutoCleanFoundationH
#define AutoCleanFoundationH

#include "vclcompat/vcl_compat.h"   // AnsiString
// AI(W906-AutoCleanCluster) 20260728: MAX_SOCKET_ROW/MAX_SOCKET_COL (#define,
// MachineType.h:391-392) are needed by DoIndexAutoClean_Arm1PickArm2Test's own
// declaration below. Included here (not left to the includer) because this
// header must be self-sufficient regardless of caller include order --
// verified test_AutoClean.cpp includes THIS header first, before cprod.h/
// cmydef.h/MachineType.h would otherwise have defined these macros.
#include "MachineType.h"            // MAX_SOCKET_ROW / MAX_SOCKET_COL

// golden AutoClean.h:4-5 -- shuttle selector enum.  Verified (grepped) this
// exact enum/these exact enumerators do not exist anywhere else in the target
// tree yet (the one other hit, ainarm9045_2x8_32.cpp, is entirely inside that
// file's own `#if 0 // TODO(W7)` dead-variant gate -- see that file's banner).
enum eWhichShuttle { euShuttle1=0, euShuttle2=1 };

// golden AutoClean.cpp:44-47 -- file-scope globals declared right at the top
// of golden's own file (mirrored here at the header so both this file and
// the .cpp agree on one definition).
extern int iAutoCleanPlaceToShuttleTask;
extern int iInXPos;
extern int iInYPos;
// AI(W906-AutoCleanCluster) 20260722: golden AutoClean.cpp:44/48/49/51 -- the
// remaining file-scope globals DoAutoCleanPickfromCleanKit/DoAutoCleanKit (now
// in-scope) read/write. `SHT_Kit` (golden :49, `int SHT_Kit=0;`) is DELIBERATELY
// NOT declared here -- verified by grep: it is written once at its own
// declaration and never read or written anywhere else in the whole of golden
// AutoClean.cpp/uCleaning.cpp (dead global).
// AI(W906-AutoCleanCluster) 20260728 UPDATE: `DoTestYRearDelayAC` (golden :51,
// a TQPF_Timer) is NOW in scope (its sole readers/writers, DoIndexAutoClean/
// DoIndexAutoClean_Arm1PickArm2Test, landed this wave) -- it is defined as a
// plain (non-extern) file-scope TQPF_Timer directly in AutoClean.cpp, right
// beside the pre-existing DoIndexAutoCleanDelay, since nothing outside this
// one TU reads or writes it (same posture as DoIndexAutoCleanDelay itself,
// which was never header-declared either). No header declaration needed.
extern int iAutoCleanPickFromCleanKitStageTask;    // golden :44 (jou 2012-05-22)
extern bool bFullViewCheckFinish;                  // golden :48 (JerryYang 20160331)
extern bool bInedxCleanFinish[2];                  // golden :50 (kevin 20170520)

// ---------------------------------------------------------------------------
//  Part A -- pure calc / config (zero VCL, zero HAL beyond already-real globals)
// ---------------------------------------------------------------------------
int  GetAutoCleanPickCount();                     // golden :163 (27 lines)
int  GetAutoCleanPickStep(int iCol);               // golden :190 (37 lines)
int  CalculateAutoCleanXPitch();                   // golden :227 (35 lines)
int  GetXPitchOfCleanKit_HP();                     // golden :262 (77 lines)
int  GetXPitchOfCleanKit_Kit();                    // golden :339 (209 lines)
int  GetXPitchOfCleanKit();                        // golden :548 (9 lines)
int  GetYPitchOfCleanKit();                        // golden :557 (26 lines)
bool RunAutoCleanByArmPickArm2Test();              // golden :1183 (7 lines) -- Jimmychiu 20230710
bool Special_2X6_Tray_XItem7();                    // golden :9107 (15 lines) -- Sam 20250712
void GetInarmSuckRow(int iShtRowKit, int &isuckRow, int &ikitStep); // golden :9122 (15 lines)

// ---------------------------------------------------------------------------
//  Part A -- HAL-only state-machine helpers (operate on already-real
//  MOT[MMAutoCleanKit].Tray / FTestSuck / BTestSuck / FLCarryKit / BLCarryKit /
//  InArmSuck / Motor Sim -- every dependency re-verified via grep this wave,
//  see the .cpp banner + the final wave report for the handful that were
//  genuinely missing and additively filled in (aHotPlateSubstrate.h/.cpp,
//  Public/MyProductionRecord.h)).
//
//  "Init task family" -- golden's ACTUAL names (the task brief's names were
//  approximate; confirmed by reading golden AutoClean.cpp:689-737 directly):
//    InitialAutoCleanAllTask   (golden :689, 14 lines) -- Sam 20230504 orchestrator
//    InitialAutoCleanTask      (golden :703, 17 lines)
//    InitialShuttleAutoCleanTask (golden :720, 7 lines)  -- brief's "ShuttleAutoCleanTask"
//    InitialIndexAutoCleanTask   (golden :727, 5 lines)  -- brief's "IndexAutoCleanTask"
//    InitPickFromShuttleTask   (golden :732, 5 lines)
//    InitPlaceToShuttleTask    (golden :3158, 5 lines)
// ---------------------------------------------------------------------------
void InitialAutoCleanAllTask();
void InitialAutoCleanTask();
void InitialShuttleAutoCleanTask();
void InitialIndexAutoCleanTask();
void InitPickFromShuttleTask();
void InitPlaceToShuttleTask();

void CleanSetSpeed(bool bBackup);                                          // golden :737 (83 lines)
int  GetShuttleState(eWhichShuttle iSht, bool bPick);                      // golden :949 (87 lines)
bool MoveInArmZToShuttlePlace(eWhichShuttle iSht, int iRowSel);            // golden :827 (122 lines)
bool MoveInOutArmZToKitPickPlace(int Pick, bool bReset, int iSht, int iShuttleRow); // golden :1036 (114 lines)
bool MoveInArmZ_Shuttle_Pick(eWhichShuttle iSht, int iSelRow);             // golden :3163 (165 lines)
bool MoveInArmXYPickCleanKit(int iPick, int iShuttleRow, eWhichShuttle iSht); // golden :2022 (163 lines)
bool TrayHasCleanIC();                                                     // golden :2185 (22 lines)
int  TrayHasCleanICCount();                                                // golden :2207 (23 lines) -- wei 2015090
bool CheckCleaningCount();                                                 // golden :2230 (113 lines)
bool PickFromCleanKit(int iRowKit);                                        // golden :2482 (109 lines)
bool PlaceToCleanKit();                                                    // golden :4143 (82 lines)
void SearchCleanKitUpDown(int iRow, eWhichShuttle iSht);                   // golden :1789 (233 lines)
bool SearchCleanKitRowCol(int &iKRow, int &iKCol);                         // golden :1683 (overload 1/2, 38 lines)
void SearchCleanKitRowCol(eWhichShuttle iSht);                             // golden :1721 (overload 2/2, 68 lines)
void DoPlaceToKitSwapData(bool bPick, int iIC_Type, int iSuckRow, int iSuckCol,
                          int iKitRow, int iKitCol);                       // golden :2460 (22 lines)
// NOTE: golden AutoClean.h:45 declares a DIFFERENT (older, 5-arg, no iIC_Type)
// signature `extern void DoPlaceToKitSwapData(bool,int,int,int,int);` -- that
// header/body pair went stale; the LIVE body (golden .cpp:2460, translated
// above) takes 6 args with iIC_Type. Declared here to match the real body.
bool CheckInArmSuckFromCleanKitICFallDown(bool bRetry);                    // golden :2343 (117 lines)
bool CheckInSuckICFallDown(int KCode);                                     // golden :3328 (86 lines)
bool CleanPad_PlaceToShuttle(int iSht);                                    // golden :3414 (140 lines)
bool CleanPad_PickFromShuttle(int iSht, int iSelRow);                      // golden :3771 (92 lines)
bool RestoreCleanKitData();                                                // golden :1465 (218 lines) -- ChungHung 20130628
void SetShuttleIcForSpecialMode(eWhichShuttle iSht, int iType);            // golden :1306 (159 lines) -- Steven 20221006
void InOutArmSuckActiveSet();                                              // golden :820 (7 lines)
bool DoInArmPineRelease();                                                 // golden :5800 (77 lines)
bool DoInArmMoveToWaitPosByAutoClean();                                    // golden :6386 (38 lines) -- ChungHung 20150129
void RecDebug(class TMyKitSuck &tray, int iTask);                          // golden :6424 (11 lines)
bool DoSocketSensorAlarm(AnsiString sFunc, int Task);                      // golden :6435 (30 lines)
int  CheckShuttleSensor_Clean(eWhichShuttle iSht, bool alarmflag);         // golden :4361 (45 lines)
bool CheckAutoCleanCloseSite(int iSht);                                    // golden :4406 (11 lines) -- Steven 20220929

// AI(W906-AutoCleanFoundation) 20260721: NOT in the task brief's Part A/HAL
// list, but RestoreCleanKitData (in-scope, above) calls it as its LAST
// statement -- translated as a required implicit dependency (same treatment
// as SetAutoCleanStringGrid). golden :1190 (55 lines) -- Steven 20180524.
void ReadWriteAutoCleanCount(bool bRead, bool bReset=false);

// AI(W906-AutoCleanFoundation) 20260721: golden AutoClean.h:30 -- required
// dependency of RestoreCleanKitData/CheckCleaningCount (both in-scope, above).
// golden :1171 (3 lines) -- Steven 20180524.
void SetAutoCleanStringGrid(int X, int Y, AnsiString Str);

// ---------------------------------------------------------------------------
//  Part D -- TfCleaning free-function translations (golden AutoClean/uCleaning.cpp;
//  translated as free functions here per the task brief -- the rest of
//  uCleaning.cpp's 2,916-line VCL settings-panel form stays OUT of scope).
//  golden TfCleaning::eSetCleanPadMode (uCleaning.h:350-354) -- the "which sink"
//  selector SetDeviceInTray's iMode arg (and its SetCleanCellValue helper,
//  file-local in AutoClean.cpp) switches on.
// ---------------------------------------------------------------------------
enum eSetCleanPadMode { eUcleanUsed=0, eAutoCleanUsed=1 };
void SetDeviceInTray(int iXItem, int iYItem, int iDeviceNum, int iMode);   // golden uCleaning.cpp:1934
bool CleanPadCountCanSupport2Arm();                                        // golden uCleaning.cpp:1325

// =============================================================================
//  W906-AutoCleanCluster (20260722) -- 9 small helpers + 4 core pick/place
//  engines + 3 shuttle-clean state machines + the master orchestrator.
// =============================================================================

// ---------------------------------------------------------------------------
//  Part E -- 9 small helpers
// ---------------------------------------------------------------------------
void CleanOnlyHasNullInShuttle();                                          // golden :55-78 (jimmychiu 20220624)
void InitialSet();                                                         // golden :80-129
void EnableAutoclean(bool Manual);                                         // golden :131-152
void __fastcall AutoCleanWriteData(AnsiString Str, int Data);              // golden :154-161 (kevin 20120710)
void SetAutoCleanICCount(bool Work);                                       // golden :583-668 (ChungHung 20141027)
void SetAutoCleanTrayPosition();                                           // golden :670-687 (Steven 20210825)
void SearchiAutoCleanNum();                                                // golden :1150-1169 (Steven 20171212)
AnsiString GetMotFunc(AnsiString asFunc, int iTask);                       // golden :6457-6460
void ResetAutoClean();                                                     // golden :5877-5930

// ---------------------------------------------------------------------------
//  Part F -- 4 core pick/place engines
// ---------------------------------------------------------------------------
int  DoAutoCleanPickfromCleanKit(eWhichShuttle iSht, bool Restart);        // golden :2591-3157 (567 lines)
bool DoPlaceToShuttle(eWhichShuttle iSht);                                 // golden :3554-3770 (217 lines)
bool DoPickFromShuttle(eWhichShuttle iSht, int iSelRow);                   // golden :3863-4142 (280 lines)
bool DoAutoCleanPlaceToCleanKit(bool Reset);                               // golden :4225-4360 (136 lines)

// ---------------------------------------------------------------------------
//  Part G -- 3 shuttle-clean state machines
// ---------------------------------------------------------------------------
void DoShuttle1AutoClean_Arm1PickArm2Test();                               // golden :5932-6043 (Jimmychiu 20230710)
void DoShuttle1AutoClean();                                                // golden :6044-6199
void DoShuttle2AutoClean();                                                // golden :6200-6385

// ---------------------------------------------------------------------------
//  Part H -- master orchestrator
// ---------------------------------------------------------------------------
void DoAutoCleanKit();                                                     // golden :4417-5799 (ChungHung 20130701)

// ---------------------------------------------------------------------------
//  Part I -- DoIndexAutoClean cluster (this wave, 20260728). Golden's own
//  header (AutoClean.h) does NOT declare the _Arm1PickArm2Test variant either
//  (verified by grep) -- it is called only from DoIndexAutoClean, defined
//  lexically before it in the SAME .cpp, so no forward declaration is
//  strictly required for that call site. Declared here anyway (same
//  precedent as DoShuttle1AutoClean_Arm1PickArm2Test above, which golden's
//  own header also omits) purely for test-visibility, so a unit test can
//  drive it in isolation with its own local arrays.
// ---------------------------------------------------------------------------
void DoIndexAutoClean_Arm1PickArm2Test(int iSpeed, int iSpeedSY, int iSpeedSZ, int &iContactCount,
                                       bool bDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       AnsiString ErrPart);                // golden :6465-7358 (Jimmychiu 20230710)
void DoIndexAutoClean();                                                   // golden :7360-9105

#endif // AutoCleanFoundationH
