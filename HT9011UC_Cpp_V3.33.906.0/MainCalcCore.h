// MainCalcCore.h
// Standard C++ translation of 6 pure calc-core functions from BCB6 main.cpp
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
// OUT OF SCOPE this wave (deferred, NOT attempted -- real unresolved type/dependency gaps):
//   * TfMain::ATCAmbientTemperCheck -- needs ATC_TYPE_33/35/61 constants not yet translated.
//   * TfMain::CheckARTSetupFile     -- needs a BinSelect structure not yet translated.
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.

#ifndef MAINCALCCORE_H
#define MAINCALCCORE_H

#include "MachineType.h"   // MAX_SOCKET_ROW / MAX_SOCKET_COL (already translated, header-only)

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

#endif // MAINCALCCORE_H
