// =============================================================================
//  Automation/AGV_E84.h  --  E84 LOADER/UNLOADER HANDSHAKE CORE (W5-Final unit)
//
//  Faithful translation of golden TfAGV member functions + file-scope globals,
//  from  HT9011UC_Code_V3.33.906.0_20260618/Automation/AGV.cpp  (BCB6, Big5).
//  Verified exact ranges (grep + manual read against the golden, cp950-decoded):
//    task-init helpers (file-scope, already free funcs in golden) :171-222
//    TfAGV::DoE84Loader()                                          :224-585
//    TfAGV::DoE84Unloader()                                        :587-926
//    TfAGV::ShowE84Log(AnsiString,int,int)                         :1067-1100
//    TfAGV::E84StatusLog(AnsiString,bool)                          :1102-1126
//  Golden decl (member methods only): Automation/AGV.h:178-179,185,196.
//  Translation wave: W5-Final (AGV_E84 unit)
//  Translator: AI(W5-Final-AGV_E84) 20260711
//
//  SCOPE -- deliberately narrow.  This is NOT a translation of AGV.cpp/.h as a
//  whole (see Automation/AGV_predicates.h for the same disclaimer re: the 3
//  AMR-mode predicates already translated in W5 Wave 1).  Still DEFERRED in
//  this same golden file: ShowE84Sensor/NowE84Status/E84StatusChange (:35-169,
//  ALed-widget-heavy + only reachable from Timer2Timer, not from our 4
//  functions), DoE84LoaderScan/DoE84UnloaderScan (:1146-1291, read/write the
//  SAME bE84Loaderflag/bE84Unloaderflag/iLoaderScanTask/iUnloaderScanTask
//  globals this unit defines -- see "SHARED STATE" note below), ScanLoadPort
//  and the bScanLoadPortState_SPIL/_Analog/_ATK families, spbSaveClick/
//  ReadFile/DoIniDataToForm/edXXXMouseDown (VCL TEdit/TComboBox-heavy).  None
//  of those were verified low-risk by the feasibility pass and are explicitly
//  OUT of this unit's scope -- left untouched in golden, no gate needed since
//  no translated file references them.
//
//  FREE-FUNCTION DEMOTION (judgment call, no ODR collision) -- same convention
//  as acarry.h's Do_Auto_SHT1/Do_Auto_SHT2 (golden TfMain member -> bare free
//  function; FormsFacade's TfAGV facade (see FormsFacade.h) does NOT declare
//  DoE84Loader/DoE84Unloader/ShowE84Log/E84StatusLog, so no prefix is needed
//  (contrast AGV_predicates.h's AGV_ prefix, needed there because FormsFacade's
//  TfAGV DOES already declare IsSPIL_AMR/IsATK_AMR/Use_AMR).
//
//  SHARED STATE (judgment call, flag for review) -- golden AGV.h:188-192
//  declares bE84LoaderFinishflag/bE84UnloaderFinishflag/bE84Loaderflag/
//  bE84Unloaderflag as TfAGV *member* fields (not file-scope globals like
//  iE84LoadTask/iPlaceWhichBuffer).  Since exactly ONE TfAGV instance (fAGV)
//  ever exists, and since FormsFacade's TfAGV is only a minimal facade (no
//  real per-instance storage), per-instance member == process-wide state in
//  practice here -- so bE84Loaderflag[3]/bE84Unloaderflag[3] are translated
//  as ordinary extern globals (verified: read/written ONLY inside AGV.cpp,
//  by our DoE84Loader/DoE84Unloader AND by the out-of-scope DoE84LoaderScan/
//  DoE84UnloaderScan -- grep-verified zero references anywhere else in the
//  golden tree).  Whoever translates DoE84LoaderScan/DoE84UnloaderScan in a
//  later wave MUST reuse these exact extern globals, not redeclare local
//  copies (would silently desync the handshake state).  bE84LoaderFinishflag/
//  bE84UnloaderFinishflag are verified DEAD (declared in AGV.h, referenced
//  NOWHERE in the whole golden tree, not even in AGV.cpp itself) -- NOT
//  translated here; add them only if/when a real consumer surfaces.
//
//  GATED DEPENDENCIES (see AGV_E84.cpp file head for the full list + golden
//  citations): fAGV->mmE84Log (FormsFacade wiring -- RESOLVED, AI(W5-Final-
//  Integrate) 20260711: TfAGV::mmE84Log added, gate lifted, see AGV_E84.cpp),
//  WriteDataToFile (common.h identifier itself gated, TODO(wave-file)),
//  ShowMyMessageBox_YES_NO (mymessbox.h, W7-UI modal dialog, untranslated).
//
//  Big5: every Chinese comment/string literal decoded via cp950 and preserved
//  as correct UTF-8.  Final gate: ZERO U+FFFD bytes (verified before hand-off).
// =============================================================================
#ifndef AGV_E84H
#define AGV_E84H

#include "vclcompat/vcl_compat.h"   // AnsiString, __fastcall no-op

// ---------------------------------------------------------------------------
//  Task-init helpers (golden Automation/AGV.cpp:176-218 -- already file-scope
//  free functions in golden, not TfAGV members; translated verbatim, no
//  demotion needed).
// ---------------------------------------------------------------------------
void InitialE84LoadTask();          // golden :176-179   -- iE84LoadTask=1
void InitialE84UnLoaderTask();      // golden :181-184   -- iE84UnloadTask=1
void InitialLoaderScanTask();       // golden :186-191   -- iLoaderScanTask[0..2]=1
void InitialUnLoaderScanTask();     // golden :193-198   -- iUnloaderScanTask[0..2]=1
void InitialE84LoadSensor();        // golden :200-208   -- SW E84_1 outputs Off()
void InitialE84UnloadSensor();      // golden :210-218   -- SW E84_2 outputs Off()

// ---------------------------------------------------------------------------
//  Core E84 handshake state machines (golden TfAGV::DoE84Loader/DoE84Unloader,
//  Automation/AGV.h:178-179) -- demoted to free functions (see file-head note).
// ---------------------------------------------------------------------------
void DoE84Loader();      // golden Automation/AGV.cpp:224-585
void DoE84Unloader();    // golden Automation/AGV.cpp:587-926

// ---------------------------------------------------------------------------
//  Logging (golden TfAGV::ShowE84Log/E84StatusLog, Automation/AGV.h:185,196)
//  -- demoted to free functions (see file-head note).
// ---------------------------------------------------------------------------
void ShowE84Log(AnsiString str, int iFunction, int iLoader);   // golden :1067-1100
void E84StatusLog(AnsiString str, bool bflag);                 // golden :1102-1126

// ---------------------------------------------------------------------------
//  Task cursors + per-call persistent state (golden Automation/AGV.cpp:171-222,
//  file-scope, non-static in golden -- external linkage preserved verbatim).
// ---------------------------------------------------------------------------
extern int iE84LoadTask;            // golden :171 -- DoE84Loader Task cursor
extern int iE84UnloadTask;          // golden :172 -- DoE84Unloader Task cursor
extern int iLoaderScanTask[3];      // golden :173 -- DoE84LoaderScan cursor (consumer: future wave)
extern int iUnloaderScanTask[3];    // golden :174 -- DoE84UnloaderScan cursor (consumer: future wave)
extern int iPlaceWhichBuffer[2];    // golden :222 -- [0]=Loader buffer(0/1/2=Loader/Empty/Color), [1]=Unloader buffer(0/1/2=Auto1/2/3)

// ---------------------------------------------------------------------------
//  golden TfAGV member fields (Automation/AGV.h:191-192) -- demoted to extern
//  globals.  See file-head "SHARED STATE" note: DoE84LoaderScan/DoE84UnloaderScan
//  (a later, out-of-scope wave) read/write these SAME arrays.
// ---------------------------------------------------------------------------
extern bool bE84Loaderflag[3];      // golden AGV.h:191
extern bool bE84Unloaderflag[3];    // golden AGV.h:192

#endif // AGV_E84H
