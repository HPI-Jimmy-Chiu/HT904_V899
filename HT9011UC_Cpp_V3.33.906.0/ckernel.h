// =============================================================================
//  ckernel.h  --  KERNEL free-function interface (W7-L2 translation)
//
//  Mirrored from : HT9011UC_Code_V3.33.906.0_20260618/ckernel.h  (READ-ONLY
//                  golden, 27 lines, BCB6 / cp950)
//  Translation wave : W7-L2 (golden ckernel.cpp -- 2588 lines, 17 functions)
//  Translator : AI(W906-W7-L2) 20260803
//
//  WHAT THIS HEADER IS
//  -------------------
//  A flat list of free-function declarations -- no class, no macro, no data.
//  Golden lines 1-27 are reproduced BELOW IN GOLDEN ORDER, verbatim in name,
//  signature and default-argument terms.  Nothing was reordered, renamed or
//  merged.
//
//  THE ONE STRUCTURAL ADDITION vs GOLDEN: the `#include "vclcompat/vcl_compat.h"`
//  immediately after the guard.  Golden ckernel.h names `AnsiString` (golden
//  :25) with no include of its own -- BCB6 supplied it through the <vcl.h>
//  precompiled header that MachineDefine.h pulled in before every use site.
//  This tree has no precompiled header, so the type has to be named.  Same
//  device, same reason, as csystem.h:36 (`#include "vclcompat/vcl_compat.h"
//  // AnsiString, TDateTime`).
//
//  DECLARED-BUT-NEVER-DEFINED -- KEPT DELIBERATELY
//  -----------------------------------------------
//  NINE distinct names in golden ckernel.h have NO definition in golden
//  ckernel.cpp -- verified this pass by scanning all 2588 cp950-decoded lines
//  of golden ckernel.cpp for each identifier, which returned ZERO occurrences
//  for every one of them (not "no definition": no occurrence at all):
//        InitialSensor              (golden ckernel.h:6)
//        CheckReceiveTrayBuildFull  (:7)
//        CheckEmptyTrayBuildIsNull  (:8)
//        CheckHasOtherEmptyTray     (:9)
//        SendTrayBufferIsFull       (:10 and again :13 -- see below)
//        SetWarn                    (:11)
//        ClearWarn                  (:12)
//        ClearAllWarn               (:14)
//        ProcessRunStatus           (:16)
//  (The W7-L2 brief said EIGHT; the measured figure is NINE distinct names
//  across TEN declarations.  Recorded here rather than argued with.)
//  A declaration without a definition is legal C++ and costs nothing until
//  somebody calls it; DROPPING them would be a silent deviation from golden's
//  interface, so all ten declarations stay.
//
//  GOLDEN ODDITY PRESERVED
//  -----------------------
//  `bool SendTrayBufferIsFull();` is declared TWICE -- golden ckernel.h:10 and
//  golden ckernel.h:13, a literal duplicate line.  Both are kept.  Re-declaring
//  the same function is legal and has no effect; removing one would be a
//  "tidy-up" of golden, which this wave does not do.
//
//  NOT DECLARED HERE (and deliberately so)
//  ---------------------------------------
//  ckernel.cpp defines SEVEN functions that golden ckernel.h does NOT declare:
//  CheckThermo (:151), CheckBinSet (:211), ChangeUseSuckMode (:253),
//  ShowRunLed (:704), ShowRunLabel (:935), DoPanelLamp (:1728) and
//  GetMotorAlarmCode (:2432).  An exhaustive cp950 scan of EVERY .h/.hpp in the
//  golden tree this pass found ZERO declarations of any of the seven, in any
//  header -- they are file-local by convention, and every intra-file call site
//  (:370, :405, :1613, :1905, :1906, :1910, :2514) sits BELOW the definition it
//  calls, so no forward declaration is needed.  Adding them here would invent
//  interface golden does not have.
//
//  Big5: golden's two Chinese trailing comments (:25, :26) are decoded via
//  cp950 and carried over as real UTF-8.  ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ckernelH
#define ckernelH
//---------------------------------------------------------------------------

//AI(W906-W7-L2) 20260803: added -- golden :25 names AnsiString but golden
// ckernel.h includes nothing; BCB6 got the type from the <vcl.h> precompiled
// header.  Same fix as csystem.h:36.
#include "vclcompat/vcl_compat.h"   // AnsiString

void InitialSensor();
bool CheckReceiveTrayBuildFull(int pos,int flag);
bool CheckEmptyTrayBuildIsNull(int pos,int flag);
bool CheckHasOtherEmptyTray();
bool SendTrayBufferIsFull();
void SetWarn(int code);
void ClearWarn(int code);
//AI(W906-W7-L2) 20260803: GOLDEN ODDITY PRESERVED -- golden ckernel.h:13 is a
// literal duplicate of golden ckernel.h:10 (`bool SendTrayBufferIsFull();`
// declared twice in one header).  Harmless and legal; kept because deleting it
// would silently "fix" golden.
bool SendTrayBufferIsFull();
void ClearAllWarn();
//void CheckManualTrayChange();
void ProcessRunStatus(int);
int ScanPannelKey();
bool ScanSystemSensor();
void ProcessAlarm();
void StopAllDestroy();
bool WaitManualStartKey();
bool WaitManualStepKey();
bool WaitManualRetryKey();
void DoSystemMessage();
AnsiString InitialTestDelayStatus();    //wei 20171020 (jou) 延遲狀態顯示
int DoScanLevelSensor(); //Steven 20190503 : 指紋辨識權限

// ---------------------------------------------------------------------------
//  W906 TEST SEAM -- NOT IN GOLDEN.  AI(W906-W7-L2) 20260803.
//
//  ShowRunLed (golden ckernel.cpp:704-932) and ShowRunLabel (golden
//  ckernel.cpp:935-1726) -- 1,021 lines between them -- are DEFERRED to W7-U by
//  this wave's charter: they are almost pure VCL form painting (tower-light
//  RunState/LED_* selection, fMain led/panel/caption writes, fLotInfo FTP button
//  enables, fNote error-code text reads).  They land in ckernel.cpp as
//  explicitly-labelled deferred stubs, NOT as bodies.
//
//  WHY THE COUNTERS EXIST.  DoSystemMessage (golden :1899-1915) is NOT deferred,
//  and its entire content is scheduling: it decides, per tick, whether to call
//  ShowRunLed() (:1905), ShowRunLabel() (:1906) and DoPanelLamp() (:1910).  With
//  empty stubs and no counter, a faithful DoSystemMessage and one that never
//  calls anything are indistinguishable -- exactly the unfalsifiable shape this
//  file family has already paid for.  A call counter per deferred stub makes
//  DoSystemMessage's scheduling genuinely testable while the painting stays
//  unwritten.
//
//  THESE ARE CALL COUNTERS, NOT DISPLAY STATE.  Nothing may read them as if the
//  tower light or the run label had actually been painted; no painting happens.
//  They retire when W7-U lands the real bodies.
//
//  Naming follows this tree's established seam idiom -- `W906_<subject>_<field>`
//  externs plus a reset, as in canary_support.h:303-311
//  (W906_ShowErrorMessage_SimReturn / _LastCode / _Count / _Reset) and
//  aHotPlateSubstrate.h:356-358 (W906_TMySucker_OffDestroyEpoch / _Total /
//  _ResetAll).  ONE reset clears the pair, following the _ResetAll precedent.
//
//  DEFINED IN: ckernel.cpp, alongside the two deferred stubs themselves.
// ---------------------------------------------------------------------------
extern int W906_ShowRunLed_Count;      // ShowRunLed()   calls since the last reset
extern int W906_ShowRunLabel_Count;    // ShowRunLabel() calls since the last reset
void W906_ShowRunLedLabel_ResetCounts();   // zero both counters

#endif
