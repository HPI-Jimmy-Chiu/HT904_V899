// MainCalcCore.h
// Standard C++ translation of 17 pure calc-core functions from BCB6 main.cpp
// (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/main.cpp (bodies) and
//                   HT9011UC_Code_V3.33.906.0_20260618/main.h (TfMain declarations).
//
// AI(ht9045-v899) 20260722: new file pair (MainCalcCore.h/.cpp). golden main.cpp is a
// 34972-line VCL form (TfMain) with NOTHING translated from it yet in this tree -- almost
// every one of its 374 functions is VCL-orchestration-coupled (widget reads/writes, global
// mutation).  A prior recon pass found exactly 8 genuinely pure calc-core functions (no VCL
// widget access, no global-mutation side effects); this wave extracts 6 of those 8 as FREE
// FUNCTIONS, following the SAME extract-calc-core / explicit-parameter convention already
// established by cContact.h/.cpp (see that file's own banner -- the direct precedent for
// this one): every external read (TfMain member, global, or another TfMain method's return
// value) becomes an explicit parameter; zero globals/VCL are touched by the bodies below.
// TfMain itself, fMain, and main.cpp/.h/.dfm do NOT exist in this translated tree; this file
// does not create them.
//
// AI(ht9045-v899) 20260723: batch 2 -- extends the same file with 5 more pure/partial calc-core
// extracts (ComputeATCAmbientTemperCheck / ComputeCheckAllMOTHome / ComputeCheckSiteMapState /
// ComputeCheckOLPErrorHasErr / ComputeCheckARTSetupFile). The two functions previously listed
// "OUT OF SCOPE" below (ATCAmbientTemperCheck, CheckARTSetupFile) are RESOLVED this batch: their
// blocking constants (ATC_TYPE_33/35/61) are #define ints in ATC/ATC_Handler_Side.h:24,25,30 --
// inlined as LOCAL named constants inside the function body (with a citation comment) rather
// than #including that whole header, same treatment as this batch gives cmydef.h's
// TOTAL_MOTOR/MInArmY/MOutArmY/MTrayX (macro/extern-const-int globals that would otherwise pull
// in the ht9045_globals link surface this file deliberately avoids -- see CMakeLists.txt's own
// comment on this TU). BinSelect (cprod.h) is NOT pulled in either -- only its two array MEMBERS
// the golden body actually reads (bAutoRetest[eTrayCount], iCatDataT3Pos[]) are taken as
// explicit array parameters, per this file's usual "resolve at the call boundary" convention.
//
// OUT OF SCOPE this wave (deferred, NOT attempted -- explicitly excluded by the task):
//   * TfMain::CheckAuto1OnlyBin1()      -- RESOLVED in batch 5, see below
//   * TfMain::CheckAutoOnlySetOneBin()  -- RESOLVED in batch 5, see below
//   * TfMain::CheckSLKSensor()          -- still out of scope, but NOT for the reason
//                                          originally given here; see batch 5 note below
// AI(W906-calccore) 20260804: the original wording of the 3 bullets above claimed the blocker
// was "a new 'violation-code' return convention not yet designed". That claim was STALE/WRONG
// even when written: the needed convention is plain `bool` + OUT-param AnsiString, and it was
// already established IN THIS FILE by ComputeCheckSiteMapState (declared below -- `bool` return
// plus `AnsiString &ErrPart`, added in batch 2, i.e. the SAME wave that wrote this banner).
// Both Check* functions are extracted in batch 5 below using exactly that convention. The real
// blocker for CheckSLKSensor is different and unrelated to any return convention -- see batch 5.
// Also explicitly OUT OF SCOPE: TfMain::CheckSiteMapState(bool bDelete) -- the file-I/O overload
// at main.cpp:31176-31183 (reads AuToSitMap.txt via ReadIniData) -- only the NO-ARG overload
// (main.cpp:31148-31172) is translated below as ComputeCheckSiteMapState.
//
// AI(W906-maincalccore) 20260727: batch 3 appended below (3 more extracts) --
// ComputeSMCDLLVersionMismatchCode / ComputeATPDLLVersionMismatch (the pure decision tails of
// the plain FREE functions DoCheckSMCDLLVersion()/DoCheckATPDLLVersion() -- NOT TfMain members;
// declared `extern int DoCheckSMCDLLVersion();` / `extern int DoCheckATPDLLVersion();` inline in
// main.cpp:236,238) and ComputeJamRateRecordStrings (a PARTIAL extract of the pure
// MTBF/JamRate-string-building sub-computation inside TfMain::RecordJamRateByTime()). All three
// keep this file's established idioms: every external read becomes an explicit parameter, and
// header-only constants already visible via this file's own MachineType.h include
// (CSMCDLLRevision, ATPDLLVersion -- both #define string literals, MachineType.h:111,113) are
// read directly in the .cpp body rather than threaded through as parameters, same treatment as
// ComputeCheckARTSetupFile's eAuto3 already got in batch 2.
//   OUT OF SCOPE this batch (left for a future wrapper wave):
//     - CheckSMCDLLVersion()'s own MOTION_CARD_TYPE guard + MessageDlg (main.cpp:254-287) and
//       DoCheckSMCDLLVersion()'s own VerInfo().GetAppVersion() file reads (main.cpp:291-301).
//     - CheckATPDLLVersion()'s own CosFunction.bDLLCommands guard + MessageDlg (main.cpp:321-338)
//       and DoCheckATPDLLVersion()'s own VerInfo().GetAppVersion() file read (main.cpp:342-347).
//     - RecordJamRateByTime()'s own static clock_t ctStart interval-gate (main.cpp:32030,32039,
//       32043-32046), its two global-counter resets (main.cpp:32069-32070), and its
//       RecordProcess(sJamRateRecord) call (main.cpp:32068) -- all side effects.
//
// AI(W906-maincalccore) 20260728: batch 4 -- TriTemp_Ch/ESD_Temperature header archaeology
// (see docs/DEVLOG.md 2026-07-27 recon). TriTemp_Ch is `extern int TriTemp_Ch[ATC_MAX_SITE]`
// (ATC/ATC_Handler_Side.h:497, defined ATC_Handler_Side.cpp:192-198); ESD_Temperature is NOT a
// variable -- it is 4 members (ESD_TemperatureAmbient=82/Hot=83/Cold=84/SuperHot=88) of the
// ESD_COMMAND enum (Interface/InterfaceSYS.h:60-...). Neither exists in this ported tree yet.
// Of the 3 main.cpp functions that read them, only ONE is a clean pure extraction:
//   * TfMain::SET_ESD_Tri_Temp(int) (main.cpp:34528-34550) -> ComputeSetESDTriTempCommand
//     below -- reads 4 plain scalars, decides which ESD_COMMAND code (if any) to send; the
//     actual SendCommand_ESD() dispatch (Interface/InterfaceSYS.cpp:459, IPC to the external
//     ESD program) is OUT OF SCOPE, same "decision tail" treatment already given
//     DoCheckSMCDLLVersion()/DoCheckATPDLLVersion() above.
// The other 2 candidates considered and REJECTED (NOT pure -- writes to shared extern globals
// and/or VCL, matching the task's own "side-effecting" exclusion, not this file's "OUT param"
// convention, which so far has only ever applied to plain LOCAL AnsiStrings the golden body
// itself declares -- see ComputeCheckSiteMapState's ErrPart / ComputeJamRateRecordStrings'
// sMTBFRecord+sJamRateRecord, both fresh locals in golden, never extern):
//   * TfMain::Tri_Temp_Set_Site() (main.cpp:34453-34510) -- its ENTIRE body writes directly
//     into bUT150Install[tcTotalCount] and bUT150HasUse[tcTotalCount] (cmydef.h:3292,5529;
//     `extern bool ...[tcTotalCount]`), global heater-channel-enable flags read continuously
//     elsewhere (ATC_Handler_Side.cpp, uTemp_Set.cpp, TempCtrl/TriTemp.cpp) -- the same
//     "writing back to a shared global array" pattern the task explicitly calls out as NOT
//     extractable (MOT[]/Sen[]-style side effect), not a pure by-product local.
//   * TfMain::Tri_Temp_ChangeATCSiteUse() (main.cpp:34512-34526) -- writes global
//     bATC_EnablesChannel[] AND calls ATC_InterfaceForm->EnablesChannel(...), a VCL form
//     method -- doubly out of scope (global write + VCL call).
// Also NOT extractable: the 5 other TriTemp_Ch reads at main.cpp:19165/19781/19833/19928/20223
// all live INSIDE TfMain::Index16Heater(bool) (main.cpp:18613-20226+, ~1600 lines), a giant
// heater-configuration state function that reads ATC_InterfaceForm (VCL), CUSTOMER_CODE,
// TestIF_File, DeviceForm_File, LastSet, etc. and writes bUT150Install/HasUse throughout --
// far outside "pure calc-core" by any reading.
//
// AI(W906-calccore) 20260804: batch 5 -- the two remaining cheap pure-calc Check* functions from
// golden main.cpp, clearing the stale OUT-OF-SCOPE bullets corrected at the top of this banner:
//   * TfMain::CheckAutoOnlySetOneBin() (main.cpp:32523-32553) -> ComputeCheckAutoOnlySetOneBin
//   * TfMain::CheckAuto1OnlyBin1()     (main.cpp:32502-32521) -> ComputeCheckAuto1OnlyBin1
// Both are `bool`-returning TfMain members whose ONLY side effect is a ShowMyMessage() dialog
// (mymessbox.h:58, `void ShowMyMessage(AnsiString S1, AnsiString S2="", AnsiString S3=NULL,
// bool Ok=false, bool bServoOff=false)`) fired immediately before the decision is returned. So
// each is split the same way ComputeCheckSiteMapState already splits CheckSiteMapState: the pure
// decision stays here as the `bool` return, the dialog TEXT becomes an OUT-param AnsiString
// (golden builds it into its own fresh locals, no widget/file/global writes), and the actual
// ShowMyMessage() call is left to a future ht9045_sm-layer wrapper. Unlike
// ComputeCheckSiteMapState, the return POLARITY here needs no inversion -- both of these return
// their raw decision directly in golden too (true == violation found).
//   STILL OUT OF SCOPE, and the earlier "return convention" excuse for it was wrong:
//     * TfMain::CheckSLKSensor() (main.cpp:32424-32500) -- it reads Cylinder[C_SLK1_Clamp] /
//       [C_SLK1_Unclamp] / [C_SLK2_Clamp] / [C_SLK2_Unclamp] .OnSensor() (main.cpp:32434,32442,
//       32451,32456,32469,32477,32486,32491), i.e. the global Cylinder[] hardware array, which is
//       ht9045_sm substrate -- it must NOT be homed in this UI-free/globals-free TU at all. Its
//       return shape (bool + 2 message strings) is in fact identical to the two extracted below;
//       substrate ownership, not the return convention, is what keeps it out.
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.

#ifndef MAINCALCCORE_H
#define MAINCALCCORE_H

#include "MachineType.h"   // MAX_SOCKET_ROW / MAX_SOCKET_COL / TEST_MAX_BIN / eATCType /
                           // eAutoRetest / e6TrayName enums (already translated, header-only)

// AnsiString needed by ComputeCheckSiteMapState (ErrPart out-param) and
// ComputeCheckARTSetupFile (aFileName_ART in-param). Narrow include (NOT the full
// vclcompat/vcl_compat.h umbrella, which also drags in windows.h/ServerSocket/IniFiles) --
// keeps this file's minimal-dependency character (see banner above). vclcompat is already
// PUBLIC-linked by ht9045_core (CMakeLists.txt), so this adds no new link requirement.
#include "vclcompat/AnsiString.h"
using vclcompat::AnsiString;

// ---------------------------------------------------------------------------
// ComputeShtModeFlag
//   Portable replacement for TfMain::GetShtModeFlag().
//   BCB6 source: main.cpp:34423-34442 (Jimmychiu 20241203 : add get site on off for
//   SIGURD_PeiXing)
//
//   Parameters (every external read the golden body performs, as an explicit param):
//     iShuttleMode  -- global TestIF.iShuttleMode  (cprod.h SYSTEM_TEST_IF, int)
//     iShuttle_Sel  -- global TestIF.iShuttle_Sel  (cprod.h SYSTEM_TEST_IF, int)
//
//   Confirmed zero type dependencies -- cleanest of the six.  Faithfully preserves the
//   iShuttleMode==1 branch's nested iShuttle_Sel==1 check (-> 1, else -> 0) and the
//   iShuttleMode!=1 else branch (-> 2).
// ---------------------------------------------------------------------------
int ComputeShtModeFlag(int iShuttleMode, int iShuttle_Sel);

// ---------------------------------------------------------------------------
// ComputeCanChangeRealDummy
//   Portable replacement for TfMain::CheckCanChangeRealDummy().
//   BCB6 source: main.cpp:12374-12380
//
//   Parameters (every external HasIC() read the golden body performs, already resolved to
//   a plain bool at the call boundary):
//     bPlate1HasIC     -- MOT[MMPlate1].HasIC()   (Motor/mymotor.h TTrayMotor, bool)
//     bPlate2HasIC     -- MOT[MMPlate2].HasIC()   (Motor/mymotor.h TTrayMotor, bool)
//     bShuttleHasIC    -- global ShuttleHasIC()    (csystem.h, bool; free function, not a
//                         TfMain member)
//     bIndexHasIC      -- global IndexHasIC()      (csystem.h, bool; free function)
//     bInArmSuckHasIC  -- InArmSuck.HasIC()        (aHotPlateSubstrate.h, bool)
//     bOutArmSuckHasIC -- OutArmSuck.HasIC()       (aHotPlateSubstrate.h, bool)
//
//   Faithfully preserves the 3 short-circuit OR-pair early-returns (Plate1||Plate2,
//   Shuttle||Index, InArmSuck||OutArmSuck) in golden's exact order, falling through to
//   true only when ALL SIX are false (no IC anywhere).
// ---------------------------------------------------------------------------
bool ComputeCanChangeRealDummy(bool bPlate1HasIC,
                                bool bPlate2HasIC,
                                bool bShuttleHasIC,
                                bool bIndexHasIC,
                                bool bInArmSuckHasIC,
                                bool bOutArmSuckHasIC);

// ---------------------------------------------------------------------------
// ComputeSiteMapIsStander
//   Portable replacement for TfMain::CheckSiteMapIsStander().
//   BCB6 source: main.cpp:23526-23543 (Steven 20230914 : 統一走for迴圈判斷)
//   NOTE: golden's own method name misspells "Standard" as "Stander" -- preserved
//   verbatim in this free-function name for grep-back-to-golden fidelity.
//
//   Parameters:
//     iShtRow  -- global TestSocket.iShtRow (aHotPlateSubstrate.h, int)
//     iShtCol  -- global TestSocket.iShtCol (aHotPlateSubstrate.h, int)
//     iSiteMap -- global TestIF_File.iSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL] (cprod.h,
//                 int; first dim decayed per this tree's existing 2D-array-param
//                 convention, see cContact.h's ComputeAutoHeightMaxKGTorque)
//
//   Faithfully preserves the running `count` idiom: count starts at iR+1 each outer-loop
//   iteration (column-major numbering: [0][0]=1 [1][0]=2 [2][0]=3 [3][0]=4 per golden's own
//   inline comment) and increments by iRowCount (==iShtRow) each inner-loop step; a site is
//   "not standard" if its map value is neither 0 (unmapped, Ifor 20251002 : avoid false
//   mismatch) nor the expected running count.
// ---------------------------------------------------------------------------
bool ComputeSiteMapIsStander(int iShtRow,
                              int iShtCol,
                              const int iSiteMap[][MAX_SOCKET_COL]);

// ---------------------------------------------------------------------------
// ComputeSiteMapPriority
//   Portable replacement for TfMain::CheckSiteMapPriority().
//   BCB6 source: main.cpp:31187-31208 (kevin 20160125 : AutoSitMap site 順序判斷)
//
//   Parameters:
//     iAutoSiteMap -- global int iAutoSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL] (cmydef.h,
//                     int; first dim decayed, same convention as iSiteMap above)
//     iSiteTotal   -- global iSiteTotal (cmydef.h, int; "目前此機台所使用SITE數量")
//
//   GOLDEN QUIRK preserved verbatim: golden also tracks a running max `int iMax` across
//   iAutoSiteMap while building the iDouble[] duplicate-count histogram, but iMax is never
//   READ again after the loop -- it has zero effect on the return value (the histogram
//   loop below reads iSiteTotal, an independent parameter, not iMax).  This is dead
//   tracking in golden itself; kept verbatim, not removed as "unused" clean-up, per this
//   wave's faithful-translation mandate.  iDouble[33] (32->33 per kevin 20180302 comment)
//   is sized to hold site-index counts for the full MAX_SOCKET_ROW*MAX_SOCKET_COL==32 grid
//   plus one (index 0 == "unmapped").  Returns false the first time any site index in
//   [1..iSiteTotal] appears 2+ times in iAutoSiteMap (a duplicate auto-site-map priority).
// ---------------------------------------------------------------------------
bool ComputeSiteMapPriority(const int iAutoSiteMap[][MAX_SOCKET_COL],
                             int iSiteTotal);

// ---------------------------------------------------------------------------
// ComputeCanChangeSite
//   Portable replacement for TfMain::CanChangeSite(bool bNoIncludeHotplate=false).
//   BCB6 source: main.cpp:14350-14393
//
//   Parameters:
//     bNoIncludeHotplate     -- unchanged golden param (bool, default false per golden
//                               main.h:1325 `CanChangeSite(bool bNoIncludeHotplate=false)`;
//                               ChungHung 20120713 : add OneCycle Can Enable Site)
//     bCanAutoCloseSite      -- global bCanAutoCloseSite (cmydef.h, bool; Steven 20200420 :
//                               獨立Auto Site Off)
//     bI28_OnOffSiteOnTheFly -- global IniConfig.bI28_OnOffSiteOnTheFly (Config.h, bool;
//                               kevin 20211103 add I28)
//     bInArmSuckHasIC        -- InArmSuck.HasIC()      (aHotPlateSubstrate.h, bool)
//     bInputShuttleHasIC     -- global InputShuttleHasIC() (csystem.h, bool; free function)
//     bIndexHasIC            -- global IndexHasIC()        (csystem.h, bool; free function)
//     bPlate1HasIC           -- MOT[MMPlate1].HasIC()  (Motor/mymotor.h TTrayMotor, bool)
//     bPlate2HasIC           -- MOT[MMPlate2].HasIC()  (Motor/mymotor.h TTrayMotor, bool)
//     bPickFromLoader        -- global bPickFromLoader (ainarm2.h, bool; Steven 20171226
//                               (Wei) : 確認Loader子任務中)
//     iPickFromLoadStageTask -- global iPickFromLoadStageTask (atester.cpp/ainarm2.h, int)
//
//   Faithfully preserves the full branch tree: bCanAutoCloseSite selects between the
//   "3-way HasIC guard" (InArmSuck||InputShuttle||Index) and the OTHER branch's own
//   bI28_OnOffSiteOnTheFly gate, which further splits on bNoIncludeHotplate into either
//   the SAME 3-way guard or a 5-way guard that ALSO includes Plate1/Plate2 HasIC; the
//   bI28_OnOffSiteOnTheFly==true sub-branch (I28 fly-mode) instead gates on
//   bPickFromLoader/iPickFromLoadStageTask.  Biggest of the six leaves (43 golden lines).
// ---------------------------------------------------------------------------
bool ComputeCanChangeSite(bool bNoIncludeHotplate,
                           bool bCanAutoCloseSite,
                           bool bI28_OnOffSiteOnTheFly,
                           bool bInArmSuckHasIC,
                           bool bInputShuttleHasIC,
                           bool bIndexHasIC,
                           bool bPlate1HasIC,
                           bool bPlate2HasIC,
                           bool bPickFromLoader,
                           int  iPickFromLoadStageTask);

// ---------------------------------------------------------------------------
// ComputeCanChangeToSocket
//   Portable replacement for TfMain::CanChangeToSocket().
//   BCB6 source: main.cpp:30156-30163 (kevin 20130425)
//
//   Thin wrapper: `!SystemStart && CanChangeSite()==true` (golden calls CanChangeSite()
//   with its default bNoIncludeHotplate==false).  Composes ComputeCanChangeSite() above --
//   translate ComputeCanChangeSite FIRST, then this one calls it with bNoIncludeHotplate
//   fixed to false (golden's default), passing every other ComputeCanChangeSite parameter
//   straight through plus the one extra read golden itself performs:
//     bSystemStart -- global SystemStart (cmydef.h, bool)
// ---------------------------------------------------------------------------
bool ComputeCanChangeToSocket(bool bSystemStart,
                               bool bCanAutoCloseSite,
                               bool bI28_OnOffSiteOnTheFly,
                               bool bInArmSuckHasIC,
                               bool bInputShuttleHasIC,
                               bool bIndexHasIC,
                               bool bPlate1HasIC,
                               bool bPlate2HasIC,
                               bool bPickFromLoader,
                               int  iPickFromLoadStageTask);

// ---------------------------------------------------------------------------
// ComputeATCAmbientTemperCheck
//   Portable replacement for TfMain::ATCAmbientTemperCheck().
//   BCB6 source: main.cpp:31942-31962 (wei 20151014)
//
//   Parameters:
//     dATCAmbientTemperature -- global IniConfig.dATCAmbientTemperature (Config.h, double)
//     iATC_SYSTEM            -- global ATC_SYSTEM (cmydef.h:3345, extern int); compared
//                               verbatim against eNewATCSystem (MachineType.h eATCType
//                               enum, ==6) inside the body, same as golden -- kept as a raw
//                               int param (not pre-resolved to bool) since golden compares
//                               it directly, matching this file's "plain global int" idiom
//                               already used for iShuttleMode/iSiteTotal etc, not its
//                               "HasIC()-call resolved to bool" idiom.
//     iATC_MODE_TYPE          -- ATC_InterfaceForm->iATC_MODE_TYPE (ATC/ATC_Handler_Side.h
//                               :224, int; VCL form member deref resolved at call site)
//
//   ATC_TYPE_33/35/61 (main.cpp:31949-31951) are #define ints in
//   ATC/ATC_Handler_Side.h:24,25,30 (33/35/61 respectively) -- inlined as local named
//   constants inside the .cpp body (citation comment there), NOT pulled in via that header,
//   per this file's "avoid the ht9045_globals link surface" convention.
//
//   Faithfully preserves: the outer guard is a tri-state OR (==0 sentinel OR <25 OR >30);
//   golden's own `dATCAmbientTemperature==0` exact-double-equality check is a deliberate
//   "unconfigured sentinel" test (IniConfig defaults to 0), kept bit-for-bit, not "fixed" to
//   an epsilon compare. Inside that guard, a NEW-ATC-system-with-matching-mode-type branch
//   only fails (-> false) when the ambient temp additionally drops below -5; every OTHER
//   path through the outer guard (old ATC system, or new system with a non-33/35/61 mode)
//   falls straight to false with no further check.
// ---------------------------------------------------------------------------
bool ComputeATCAmbientTemperCheck(double dATCAmbientTemperature,
                                   int iATC_SYSTEM,
                                   int iATC_MODE_TYPE);

// ---------------------------------------------------------------------------
// ComputeCheckAllMOTHome
//   Portable replacement for TfMain::Check_AllMOT_Home().
//   BCB6 source: main.cpp:15112-15125
//
//   Parameters:
//     bAtHomeLed -- caller-resolved array, one bool per motor index, ==
//                   MOT[i].Led[iHomeLed] AFTER golden's own MOT[i].ScanMotorStatus() call
//                   (Motor/mymotor.h TTrayMotor + Motor/HTMotor.h iHomeLed==1) has already
//                   run for that i. Golden interleaves the ScanMotorStatus() hardware poll
//                   INSIDE the loop, immediately before reading Led[iHomeLed]; per this
//                   wave's explicit instruction, that hardware read is resolved by the
//                   CALLER into this plain bool array (same "HasIC()-call -> explicit bool"
//                   convention already used for MOT[].HasIC() in the existing 6 functions).
//                   Caller must size/populate this array for TOTAL_MOTOR (cmydef.h:33,
//                   ==164) entries; the implementation inlines that same 164 (and the
//                   MTrayX==30/MInArmY==1/MOutArmY==20 skip-list, cmydef.cpp:2331,2351,2361)
//                   as local named constants -- see the .cpp body citation -- rather than
//                   #including cmydef.h (extern-const-int globals would need ht9045_globals
//                   linkage, which this TU deliberately avoids).
//
//   Faithfully preserves the 3-index skip-list (MTrayX/MInArmY/MOutArmY): a motor NOT in
//   that list that is off-home fails the whole check immediately (return false); a listed
//   motor that is off-home is simply skipped (continue) and does not affect the result.
// ---------------------------------------------------------------------------
bool ComputeCheckAllMOTHome(const bool bAtHomeLed[]);

// ---------------------------------------------------------------------------
// ComputeCheckSiteMapState
//   PARTIAL portable replacement for TfMain::CheckSiteMapState() -- the NO-ARG overload
//   ONLY (main.cpp:31148-31172). The bool-param overload (main.cpp:31176-31183, file I/O via
//   ReadIniData) is OUT OF SCOPE and NOT translated here (see file banner).
//
//   Parameters:
//     iSiteMap       -- global TestIF_File.iSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL]
//                        (cprod.h, int; first dim decayed, same convention as
//                        ComputeSiteMapIsStander above)
//     iDutOnOff0     -- global TestMode.iDutOnOff[0][MAX_SOCKET_ROW][MAX_SOCKET_COL]
//                        (cprod.h: `int iDutOnOff[2][MAX_SOCKET_ROW][MAX_SOCKET_COL]`,
//                        golden always indexes the OUTER dim with the literal 0 here --
//                        caller passes TestMode.iDutOnOff[0], this param is that already-
//                        sliced 2D slab, first dim decayed same as iSiteMap above)
//     IndexSuckName  -- global IndexSuckName[MAX_SOCKET_ROW][MAX_SOCKET_COL] (cmydef.h:3226,
//                        `extern AnsiString IndexSuckName[4][8]`; first dim decayed)
//     ErrPart        -- OUT param. Golden's local `AnsiString ErrPart=""` built up across the
//                        same loop as bHasErr -- a pure by-product with no side effects of
//                        its own (no widget/file writes), so it is extracted alongside the
//                        decision rather than dropped. Caller must pass an already-empty
//                        AnsiString (matching golden's own `ErrPart=""` initializer); this
//                        function only APPENDS to it via operator+=, never clears it.
//
//   RETURN VALUE -- IMPORTANT POLARITY NOTE: this function returns the raw `bHasErr` decision
//   (true == at least one AutoSiteMap-mapped, Index-closed site found), which is the OPPOSITE
//   sense of golden's own CheckSiteMapState() return (golden returns false when bHasErr, true
//   otherwise, AFTER also firing MyDBIProcess("System", "AutoSiteMap Do not close Site") and
//   ShowErrorMessage("WAR07400", ...) -- both of those side-effecting calls are OUT OF SCOPE
//   here and belong in a not-yet-translated wrapper that inverts this bool and fires them).
//
//   Faithfully preserves the row-major (i then j) scan and the `iSiteMap[i][j]>=1 &&
//   iDutOnOff0[i][j]==0` per-site test (kevin 20150115 comment: a site the AutoSiteMap has
//   mapped in, but Index itself still shows closed/off).
// ---------------------------------------------------------------------------
bool ComputeCheckSiteMapState(const int iSiteMap[][MAX_SOCKET_COL],
                               const int iDutOnOff0[][MAX_SOCKET_COL],
                               const AnsiString IndexSuckName[][MAX_SOCKET_COL],
                               AnsiString &ErrPart);

// ---------------------------------------------------------------------------
// ComputeCheckOLPErrorHasErr
//   PARTIAL portable replacement for TfMain::CheckOLPError() (main.cpp:33986-34012, Sam
//   20230921). Extracts ONLY the pure reduction "does any LastSet.OLPSetBinErr[0..9] reach
//   >=3"; the 3 distinct ShowMyMessagePWD() dialogs golden fires per-index (i==0/i==1/i==2)
//   are OUT OF SCOPE and belong in a not-yet-translated wrapper around this bool.
//
//   Parameters:
//     OLPSetBinErr -- global LastSet.OLPSetBinErr[10] (LastSet.h:472, int array)
//
//   Faithfully preserves the >=3 threshold and the full 0..9 scan range (no early-out: golden
//   itself does not break/return early on the first hit either, since it still needs to fire
//   every index's own dialog -- this extract does not need the full scan for correctness, but
//   keeps it anyway for one-to-one fidelity with golden's loop shape).
// ---------------------------------------------------------------------------
bool ComputeCheckOLPErrorHasErr(const int OLPSetBinErr[10]);

// ---------------------------------------------------------------------------
// ComputeCheckARTSetupFile
//   Portable replacement for TfMain::CheckARTSetupFile().
//   BCB6 source: main.cpp:32211-32238 (wei 20160115)
//
//   Parameters:
//     iUSE_AUTO_RETEST -- global USE_AUTO_RETEST (cmydef.h:2972, extern int); compared
//                         verbatim against eartInstall (MachineType.h eAutoRetest enum,
//                         ==1) inside the body, same raw-int idiom as iATC_SYSTEM above
//     bA10_AutoReTest  -- global IniConfig.bA10_AutoReTest (Config.h, bool)
//     bAutoRetest      -- BinSelect[FT_ART].bAutoRetest[] (cprod.h SYSTEM_BIN_SELECT,
//                         `bool bAutoRetest[eTrayCount]`; FT_ART==eBinFT_ART==4 already
//                         applied by the caller -- this param is that already-indexed
//                         33-element row, NOT the whole BinSelect struct/array, per this
//                         wave's explicit instruction). Only indices [0..eAuto3] (0..2) are
//                         ever read by this function's loop bound below.
//     iCatDataT3Pos    -- BinSelect[FT_ART].iCatDataT3Pos[] (cprod.h SYSTEM_BIN_SELECT,
//                         `int iCatDataT3Pos[TEST_MAX_BIN]`; same FT_ART pre-indexing as
//                         bAutoRetest above). Only indices [0..iTestBinCount) are read.
//     iTestBinCount    -- global iTestBinCount (cmydef.h:3383, extern int) -- the loop-j
//                         upper bound (a runtime lot-dependent count, independent of the
//                         static TEST_MAX_BIN==256 array capacity)
//     aFileName_ART    -- fMain->cbSetupFileName->Text (VCL form field, AnsiString;
//                         resolved at the call site, same convention as other VCL reads in
//                         this file's other functions)
//
//   eAuto3 (MachineType.h e6TrayName enum, ==2) and TEST_MAX_BIN (MachineType.h, `const int
//   ==256`) are both header-only constants already available via the MachineType.h include
//   above (no separate inlining needed, unlike ATC_TYPE_33/35/61 / TOTAL_MOTOR above, which
//   live in headers this file does NOT include).
//
//   Faithfully reproduces all 7 AnsiString::Pos() substring checks in golden's exact order
//   (Ifor 20170424/20190711/20200914 KYEC customer-code checks): "_ART" tested with `!=0`
//   (anywhere in the string), all 6 others ("9203"/"5611"/"9287"/"3971"/"9606"/"KL"/"9378" --
//   note that is 6 literals plus "_ART" == 7 total) tested with `==1` (string must START with
//   that literal). Outer gate (iUSE_AUTO_RETEST==eartInstall && bA10_AutoReTest) and the
//   nested BinSelect match test (bAutoRetest[i]==true && iCatDataT3Pos[j]==i) are both
//   preserved verbatim; any qualifying (i,j) cell whose ART-keyword test passes returns true
//   immediately (short-circuit on first match, same as golden).
// ---------------------------------------------------------------------------
bool ComputeCheckARTSetupFile(int iUSE_AUTO_RETEST,
                               bool bA10_AutoReTest,
                               const bool bAutoRetest[],
                               const int iCatDataT3Pos[],
                               int iTestBinCount,
                               const AnsiString &aFileName_ART);

// ---------------------------------------------------------------------------
// ComputeSMCDLLVersionMismatchCode
//   Portable replacement for the decision tail of free function
//   DoCheckSMCDLLVersion() (main.cpp:289-319; a plain extern free function, NOT a TfMain
//   member -- declared `extern int DoCheckSMCDLLVersion();` inline at main.cpp:236).
//   BCB6 source: main.cpp:303-318 (the 3-way AnsiString compare only).
//
//   Parameters:
//     strSMCVersion0 -- global strSMCVersion[0] (main.cpp:220, `AnsiString
//                       strSMCVersion[2]={"",""}`), already populated by the caller's own
//                       VerInfo().GetAppVersion(...) + .sprintf(...) call (main.cpp:299-300)
//                       -- that file-read + formatting is OUT OF SCOPE (side-effecting I/O),
//                       per the task.
//     strSMCVersion1 -- global strSMCVersion[1], same provenance as above.
//
//   CSMCDLLRevision (MachineType.h:111, `#define CSMCDLLRevision "3.15.0.0"`) is compared
//   directly -- already visible via this file's own MachineType.h include, same treatment as
//   ComputeCheckARTSetupFile's eAuto3 (header-only, no separate inlining needed, unlike
//   ATC_TYPE_33/35/61 / TOTAL_MOTOR above, which live in headers this file does NOT include).
//
//   OUT OF SCOPE (left for a future wrapper, per the task):
//     - main.cpp:254-287 CheckSMCDLLVersion()'s own MOTION_CARD_TYPE guard, the
//       str/strtemp.sprintf(...) message construction, MessageDlg(...), and its own final
//       bool return (== ret==0).
//     - main.cpp:291-301 DoCheckSMCDLLVersion()'s own strFilePath[]/VerInfo().GetAppVersion()
//       file reads that populate strSMCVersion[0]/[1] in the first place.
//
//   Faithfully preserves the exact if/else-if/else chain and its (deliberately asymmetric,
//   NOT a bitmask) return codes: 3 (BOTH strSMCVersion[0] and [1] mismatch CSMCDLLRevision),
//   1 (ONLY strSMCVersion[0] mismatches), 2 (ONLY strSMCVersion[1] mismatches), 0 (neither
//   mismatches -- OK).
// ---------------------------------------------------------------------------
int ComputeSMCDLLVersionMismatchCode(const AnsiString &strSMCVersion0,
                                      const AnsiString &strSMCVersion1);

// ---------------------------------------------------------------------------
// ComputeATPDLLVersionMismatch
//   Portable replacement for the decision tail of free function
//   DoCheckATPDLLVersion() (main.cpp:340-356; a plain extern free function, NOT a TfMain
//   member -- declared `extern int DoCheckATPDLLVersion();` inline at main.cpp:238).
//   BCB6 source: main.cpp:348-355 (the single AnsiString compare only).
//
//   Parameters:
//     sATPDLLVersion -- global sATPDLLVersion (main.cpp:221, `AnsiString sATPDLLVersion=""`),
//                        already populated by the caller's own VerInfo().GetAppVersion(...) +
//                        .sprintf(...) call (main.cpp:346-347) -- OUT OF SCOPE (side-effecting
//                        I/O), same treatment as ComputeSMCDLLVersionMismatchCode above.
//
//   ATPDLLVersion (MachineType.h:113, `#define ATPDLLVersion "1.0.0.1"`) is compared directly
//   -- already visible via this file's own MachineType.h include.
//
//   OUT OF SCOPE (per the task):
//     - main.cpp:321-338 CheckATPDLLVersion()'s own CosFunction.bDLLCommands guard, message
//       construction, MessageDlg(...), and its own final bool return.
//     - main.cpp:342-347 DoCheckATPDLLVersion()'s own strFilePath/VerInfo().GetAppVersion()
//       file read that populates sATPDLLVersion in the first place.
//
//   Faithfully preserves the single compare's two return codes: 1 (mismatch), 0 (OK, per
//   golden's own "//OK" comment at main.cpp:354).
// ---------------------------------------------------------------------------
int ComputeATPDLLVersionMismatch(const AnsiString &sATPDLLVersion);

// ---------------------------------------------------------------------------
// ComputeJamRateRecordStrings
//   PARTIAL portable replacement for TfMain::RecordJamRateByTime() (main.cpp:32028-32072,
//   2015.11.11 Joye "Add Jam Rate Record"). Extracts ONLY the pure MTBF/JamRate
//   string-building sub-computation at main.cpp:32047-32067; see header banner above for the
//   OUT-OF-SCOPE list (the static clock_t interval-gate, the counter resets, and the
//   RecordProcess() call -- all side effects, deferred to a future wrapper wave).
//
//   Parameters:
//     iRecordJamRateByTime_JamCount    -- global iRecordJamRateByTime_JamCount (cmydef.h:3878,
//                                          extern int)
//     iRecordJamRateByTime_LoaderCount -- global iRecordJamRateByTime_LoaderCount
//                                          (cmydef.h:3877, extern int)
//     iRecordJamRateIntervalTime       -- global IniConfig.iRecordJamRateIntervalTime
//                                          (Config.h:237, int)
//     sMTBFRecord    -- OUT param. Unconditionally OVERWRITTEN (assigned via operator=, not
//                       appended) -- unlike ComputeCheckSiteMapState's append-only ErrPart
//                       above, golden's own sMTBFRecord is a fresh local reassigned by
//                       whichever if/else branch runs, so the caller does not need to
//                       pre-clear it.
//     sJamRateRecord -- OUT param, same "unconditionally overwritten" contract as
//                       sMTBFRecord.
//
//   PRECONDITION carried over from the OUT-OF-SCOPE outer guard (main.cpp:32031-32032, `if
//   (IniConfig.bRecordJamRateByTime==false || IniConfig.iRecordJamRateIntervalTime<=0) return;`):
//   golden only ever reaches this code with iRecordJamRateIntervalTime>0. This extract does
//   NOT re-check that guard (it is out of scope), so calling it directly with
//   iRecordJamRateIntervalTime<=0 is undefined by the caller's contract, not by this function
//   (matches golden, which likewise never runs this tail under that condition).
//
//   GOLDEN QUIRKS preserved verbatim (found while reading the full body -- neither is
//   "fixed" here):
//     1) The iRecordJamRateByTime_JamCount!=0 branch's sMTBFRecord literal is "  MTBF 1/..."
//        -- a fixed digit '1', NOT iRecordJamRateByTime_JamCount itself (main.cpp:32063).
//        The displayed MTBF numerator is therefore always "0" (JamCount==0) or "1"
//        (JamCount!=0, no matter how large the real count is) -- never the true jam count.
//     2) Despite the "MTBF" (Mean Time Between Failures, i.e. hours/jam) label, the non-zero
//        branch's fH formula (main.cpp:32060) computes
//        `iRecordJamRateByTime_JamCount / (iRecordJamRateIntervalTime/60.)`, i.e.
//        jams-per-hour (a RATE), the reciprocal of an actual mean-time-between-failures
//        (hours-per-jam). Kept as-is, not "corrected" to invert the ratio.
//   Also faithfully preserves the truncating iH=(int)(fH*1000); fH=iH/1000.0 round-trip
//   (BCB6's implicit double->int assignment truncates toward zero, same as a C-style cast).
// ---------------------------------------------------------------------------
void ComputeJamRateRecordStrings(int iRecordJamRateByTime_JamCount,
                                  int iRecordJamRateByTime_LoaderCount,
                                  int iRecordJamRateIntervalTime,
                                  AnsiString &sMTBFRecord,
                                  AnsiString &sJamRateRecord);

// ---------------------------------------------------------------------------
// ComputeSetESDTriTempCommand
//   Portable replacement for the decision logic of TfMain::SET_ESD_Tri_Temp(int iTemperature).
//   BCB6 source: main.cpp:34528-34550 (Ztex 2023.04.19 Add HT-1032 TriTemp Function)
//
//   Parameters:
//     iUSE_NOVX3360   -- global USE_NOVX3360 (cmydef.h:2955, extern int; golden compares it
//                        as `USE_NOVX3360==true`, i.e. ==1 -- raw-int idiom, same treatment
//                        already used for iATC_SYSTEM/iUSE_AUTO_RETEST above)
//     iTriTempMachine -- global Tri_Temp_Machine (cmydef.h:5523, extern int)
//     fWorkTemperBase -- global Temperature.fWorkTemperBase (cprod.h:1391, double)
//     iTemperature    -- unchanged golden param (int); both call sites (main.cpp:6257,27303)
//                        pass LastSet.iTemperature
//
//   Tempture_Hot(==1)/Tempture_AmbientHot(==3) are `const int` globals defined in
//   cmydef.cpp:2972-2973 (NOT header-only -- would need ht9045_globals linkage) -- inlined as
//   local named constants in the .cpp body, same treatment this file already gives
//   ATC_TYPE_33/35/61 / TOTAL_MOTOR (see banner above and ComputeATCAmbientTemperCheck).
//
//   ESD_TemperatureHot(83)/ESD_TemperatureSuperHot(88)/ESD_TemperatureCold(84)/
//   ESD_TemperatureAmbient(82) are members of Interface/InterfaceSYS.h's ESD_COMMAND enum (a
//   plain compile-time enum, no extern storage) -- inlined the same way rather than
//   #including that header, keeping this file's minimal-dependency character.
//
//   RETURN VALUE: the ESD_COMMAND code that golden's SendCommand_ESD() would be called with,
//   or the sentinel kNoESDTriTempCommand (-1, distinct from every non-negative ESD_COMMAND
//   value, see ESD_NORMAL==0) when golden would not call SendCommand_ESD() at all for this
//   invocation. SendCommand_ESD() itself (Interface/InterfaceSYS.cpp:459, IPC dispatch to the
//   external ESD program) is OUT OF SCOPE -- belongs in a not-yet-translated wrapper around
//   this decision, same "decision tail" split as ComputeSMCDLLVersionMismatchCode above.
//
//   GOLDEN QUIRK preserved verbatim: when iTemperature==Tempture_Hot and fWorkTemperBase
//   falls in the gap [10,40) -- neither >=40 (Hot/SuperHot) nor <10 (Cold) -- NONE of golden's
//   three inner if/else-if branches fire (there is no final catch-all else), so
//   SendCommand_ESD() is not called at all for that sample; this extract returns
//   kNoESDTriTempCommand for that gap rather than guessing a nearest command.
//   Also preserved: the iTemperature!=Tempture_Hot && iTemperature!=Tempture_AmbientHot
//   "else" branch (main.cpp:34545-34548) sends the SAME ESD_TemperatureAmbient command as the
//   AmbientHot branch just above it -- both collapse to the identical return value here.
// ---------------------------------------------------------------------------
const int kNoESDTriTempCommand = -1;

int ComputeSetESDTriTempCommand(int iUSE_NOVX3360,
                                 int iTriTempMachine,
                                 double fWorkTemperBase,
                                 int iTemperature);

// ---------------------------------------------------------------------------
// ComputeCheckAutoOnlySetOneBin
//   Portable replacement for TfMain::CheckAutoOnlySetOneBin() (golden main.h:1510,
//   `bool CheckAutoOnlySetOneBin();`).
//   BCB6 source: main.cpp:32523-32553 (V3.27R.560 Ifor 20171213 (Steven) : Auto Tray
//   設定Pass時僅可設定一個Bin)
//
//   Parameters:
//     bUsePassBinOnlyCanSetOneBin -- global CosFunction.bUsePassBinOnlyCanSetOneBin
//                                     (CosFunction.h:144, bool)
//     iIsPassT6      -- global Prod.iIsPassT6[eTrayCount] (cprod.h:1068, int; the golden
//                        body's own comment records the Steven 20240105 rename
//                        Prod.bIsPass --> Prod.iIsFailT6). Indices [0,eTrayCount) are read.
//     iT6CatData     -- global Prod.iT6CatData[TEST_MAX_BIN] (cprod.h:512, int, "Auto1 = 0");
//                        only [0,iTestBinCount) is read, so this param is left UNSIZED, same
//                        runtime-bounded-array convention as ComputeCheckARTSetupFile's
//                        iCatDataT3Pos above (contrast the compile-time-sized
//                        ComputeCheckOLPErrorHasErr(const int OLPSetBinErr[10])).
//     iTestBinCount  -- global iTestBinCount (cmydef.h:3396, extern int) -- the inner loop-j
//                        bound (runtime lot-dependent, independent of TEST_MAX_BIN capacity)
//     s6TrayName     -- global s6TrayName[eTrayCount] (cmydef.h:138, `extern AnsiString
//                        s6TrayName[eTrayCount]`) -- an extern-linkage array, so it is taken as
//                        an explicit param rather than referenced directly, exactly like
//                        ComputeCheckSiteMapState's IndexSuckName above.
//     Msg1, Msg2     -- OUT params, == golden's own two fresh locals `AnsiString str1, str2`
//                        (main.cpp:32527) as passed to ShowMyMessage(str1, str2) at
//                        main.cpp:32544, i.e. Msg1 is the English S1 argument and Msg2 the
//                        Chinese S2 argument of mymessbox.h:58's ShowMyMessage. Building them
//                        is a pure by-product (two AnsiString::sprintf calls into locals, no
//                        widget/file/global writes), so they are extracted alongside the
//                        decision; the ShowMyMessage() call itself is OUT OF SCOPE (belongs in
//                        the future ht9045_sm-layer wrapper).
//                        CONTRACT: both are assigned ONLY on the `return true` path. On either
//                        `return false` path golden fires no dialog at all and its str1/str2 stay
//                        at their "" initialisers, so this function leaves the caller's objects
//                        UNTOUCHED there -- read Msg1/Msg2 only when the return is true. (This is
//                        neither ComputeCheckSiteMapState's append-only nor
//                        ComputeJamRateRecordStrings' unconditional-overwrite contract; it is
//                        assign-on-true, which is what golden's control flow actually does.)
//
//   eTrayCount (MachineType.h:1104, last enumerator of e6TrayName, ==33 -- eMag14==32 at
//   MachineType.h:1103) is the outer loop bound and is read directly: header-only, already
//   visible via this file's own MachineType.h include, same treatment as eAuto3 in
//   ComputeCheckARTSetupFile above (no local inlining needed, unlike TOTAL_MOTOR/ATC_TYPE_*).
//
//   RETURN VALUE: golden's raw decision, NO polarity inversion (contrast
//   ComputeCheckSiteMapState): true == a violation was found (some Pass-marked tray has 2+ bins
//   mapped to it) == golden's own `return true` at main.cpp:32545; false == OK.
//
//   Faithfully preserves: the `iBinCount=0` re-initialisation at the TOP of each iIsPassT6[i]==1
//   tray (main.cpp:32534) rather than per-bin; the dead `int iBinCount=0` initialiser at
//   main.cpp:32525 (always overwritten by :32534 before any read -- kept, not cleaned up); the
//   `iBinCount>1` threshold, i.e. the violation fires on the SECOND matching bin, not the first;
//   and the immediate return on that second match, so ONLY the first offending tray in
//   ascending-i order ever produces a message.
// ---------------------------------------------------------------------------
bool ComputeCheckAutoOnlySetOneBin(bool bUsePassBinOnlyCanSetOneBin,
                                    const int iIsPassT6[eTrayCount],
                                    const int iT6CatData[],
                                    int iTestBinCount,
                                    const AnsiString s6TrayName[eTrayCount],
                                    AnsiString &Msg1,
                                    AnsiString &Msg2);

// ---------------------------------------------------------------------------
// ComputeCheckAuto1OnlyBin1
//   Portable replacement for TfMain::CheckAuto1OnlyBin1() (golden main.h:1195,
//   `bool CheckAuto1OnlyBin1();`).
//   BCB6 source: main.cpp:32502-32521 (Alick 20160802 add for SCC Auto1 Only Bin1 Function)
//
//   Parameters:
//     iCUSTOMER_CODE -- global CUSTOMER_CODE (cmydef.h:3181, `extern int CUSTOMER_CODE`);
//                        extern-linkage int, so an explicit param, compared inside the body
//                        against CC_UNISEM_M -- same raw-int idiom as iATC_SYSTEM /
//                        iUSE_AUTO_RETEST / iUSE_NOVX3360 above
//     iTester        -- global LastSet.iTester (golden LastSet.h:52, int), compared against
//                        OFF_LINE. NOTE: LastSet.h does NOT exist in this ported tree yet
//                        (verified: `find . -iname LastSet.h` returns nothing outside build
//                        dirs), which is a further reason this read is a plain int param
//                        rather than a struct access -- same treatment ComputeCheckOLPErrorHasErr
//                        above already gives LastSet.OLPSetBinErr[].
//     iT6PosCate     -- global Prod.iT6PosCate[TEST_MAX_BIN] (cprod.h:513, int, "Auto1 = 1" --
//                        note this array is 1-BASED on the position enum, unlike iT6CatData's
//                        0-based "Auto1 = 0" at cprod.h:512, which is why it is ePosAuto1 and
//                        not eAuto1 that is compared here). Only [0,iTestBinCount) is read, so
//                        UNSIZED, same convention as iT6CatData above.
//     iTestBinCount  -- global iTestBinCount (cmydef.h:3396, extern int) -- the loop bound
//     Msg            -- OUT param == the single S1 argument golden passes to ShowMyMessage
//                        (main.cpp:32511 / 32516); mymessbox.h:58 defaults S2 to "", and golden
//                        uses the ONE-argument form here (both literals embed their own "\n"
//                        between the Chinese and English halves), which is why this function has
//                        ONE message OUT param where ComputeCheckAutoOnlySetOneBin above has two.
//                        Only one of the two literals can ever be produced per call (each branch
//                        returns immediately), so one param is sufficient AND faithful.
//                        CONTRACT: assign-on-true only, exactly as for Msg1/Msg2 above -- both
//                        `return false` paths fire no dialog and leave the caller's object
//                        untouched.
//
//   CC_UNISEM_M (MachineType.h:358, `#define CC_UNISEM_M 981 // Unisem Malaysia`) and ePosAuto1
//   (MachineType.h:1146, eBinPositionName enum, ==1) are both read directly -- header-only,
//   already visible via this file's own MachineType.h include. OFF_LINE, by contrast, is
//   `#define OFF_LINE 0` in cmydef.h:85, a header this file deliberately does NOT include, so it
//   is inlined as a local named constant in the .cpp body with a citation comment -- same
//   treatment as ATC_TYPE_33/35/61 and TOTAL_MOTOR above.
//
//   RETURN VALUE: golden's raw decision, no inversion: true == a Bin1/Auto1 violation was found
//   (main.cpp:32512, 32517); false == OK, OR the UNISEM_M offline bypass.
//
//   ORDERING IS LOAD-BEARING -- preserved bit-for-bit, do NOT "tidy" any of this:
//     1) The CUSTOMER_CODE==CC_UNISEM_M && iTester==OFF_LINE early return (main.cpp:32504-32505,
//        Ifor 20171213 (Steven): UNISEM_M asked that Offline not be blocked by Auto1-Only-Bin1)
//        returns FALSE, i.e. "no violation" -- it is a BYPASS, not a violation report, and it
//        runs before the loop is ever entered.
//     2) Branch 1 (main.cpp:32509) is `iT6PosCate[i]!=ePosAuto1 && i==1` -- the `i==1` conjunct
//        means this branch can fire on ONLY ONE iteration (i==1, the second bin), whereas
//        branch 2 (main.cpp:32514, `iT6PosCate[i]==ePosAuto1 && i!=1`) can fire on ANY other
//        iteration. Because both branches return immediately, i==0 is tested against branch 2
//        BEFORE i==1 is ever tested against branch 1: a bin-0 mapped to Auto1 therefore reports
//        the "only Bin 1 may be Auto1" message and the `i==1` check never runs at all. Kept
//        verbatim (the asymmetric `i==1` / `i!=1` conjuncts are golden's, quirk and all).
// ---------------------------------------------------------------------------
bool ComputeCheckAuto1OnlyBin1(int iCUSTOMER_CODE,
                                int iTester,
                                const int iT6PosCate[],
                                int iTestBinCount,
                                AnsiString &Msg);

#endif // MAINCALCCORE_H
