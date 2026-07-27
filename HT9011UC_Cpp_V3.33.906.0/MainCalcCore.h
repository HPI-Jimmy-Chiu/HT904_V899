// MainCalcCore.h
// Standard C++ translation of 11 pure calc-core functions from BCB6 main.cpp
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
// OUT OF SCOPE this wave (deferred, NOT attempted -- explicitly excluded by the task, they need
// a new "violation-code" return convention not yet designed):
//   * TfMain::CheckAuto1OnlyBin1()
//   * TfMain::CheckAutoOnlySetOneBin()
//   * TfMain::CheckSLKSensor()
// Also explicitly OUT OF SCOPE: TfMain::CheckSiteMapState(bool bDelete) -- the file-I/O overload
// at main.cpp:31176-31183 (reads AuToSitMap.txt via ReadIniData) -- only the NO-ARG overload
// (main.cpp:31148-31172) is translated below as ComputeCheckSiteMapState.
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

#endif // MAINCALCCORE_H
