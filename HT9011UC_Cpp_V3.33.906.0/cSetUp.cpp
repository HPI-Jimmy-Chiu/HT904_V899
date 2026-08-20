// =============================================================================
//  cSetUp.cpp  --  Setup-dialog logic that needs ht9045_sm-layer symbols
//                  (site-map suck-mode reload, STM close-site dispatch, the
//                  Auto-Site-Mapping checkbox guard).
//
//  AI(W906-FW-SETUP-B) 20260821: new file. Translation wave: FW-SETUP-B
//  (cSetUp Wave B, form-logic campaign). Golden source of truth:
//  HT9011UC_Code_V3.33.906.0_20260618/cSetUp.cpp (4,858 lines, cp950 --
//  decoded with `python3 -c "open(p,'rb').read().decode('cp950')"`, 0
//  U+FFFD, 20260821) + cSetUp.h (295 lines) + cSetUp.dfm (cp950, 0 U+FFFD).
//
//  WHY THIS FILE EXISTS SEPARATELY FROM forms/fSetup.cpp -- read forms/
//  fSetup.h's Wave B banner FIRST (full citation there, not repeated in
//  full here): forms/fSetup.cpp is compiled into the `ht9045_forms` CMake
//  target, which CMakeLists.txt:706 restricts to `vclcompat ht9045_globals`
//  only -- a deliberate bottom-layer diet, and CMakeLists.txt:589-593
//  documents that `ht9045_forms PUBLIC ht9045_sm` does not even CONFIGURE
//  (a real cycle: ht9045_sm already PUBLIC-links ht9045_forms,
//  CMakeLists.txt:2216). The 3 methods below all need real, non-stub bodies
//  that live in `ht9045_sm` (aHotPlateSubstrate.cpp/csystem.cpp/ainarm2.cpp/
//  ainarm9045_2x6_8.cpp/ainarm9045_2x8_8.cpp/ainarm9045_1x4_4.cpp/
//  ainarm9045_1x3_2_14.cpp/canary_support.cpp), so this file is written to
//  belong to `ht9045_sm` -- the SAME split cTemperFrom.cpp/MainTempMode.cpp
//  already established for TfTemperFrom::/TfMain:: methods with the same
//  shape of dependency (root .cpp for the ht9045_sm-heavy methods; the
//  forms/fXxx.cpp sibling stays on the ht9045_globals-only diet).
//
//  CMAKE STATUS -- NOT YET WIRED. This wave's edit boundary is `cSetUp.cpp`
//  (this file, append-only) + forms/fSetup.h only; CMakeLists.txt is out of
//  scope. This file compiles (verified with `g++ -fsyntax-only` against the
//  ht9045_sm target's own includes_CXX.rsp, see hand-off report for the
//  exact command) but is UNREACHABLE from any build until a future pass adds
//  it to `add_library(ht9045_sm STATIC ...)`'s source list, immediately
//  after `cTemperFrom.cpp` (CMakeLists.txt:1895) and before `MainTempMode.cpp`
//  (:1916) -- both siblings in the exact same split, so the insertion point
//  is contiguous with its own kind.
//
//  WAVE SCOPE -- 3 of 47 golden TfSetup:: methods (all (a) 顯示/讀取側,
//  read line-by-line in full):
//    ReadUseSuckModeFile   golden :2133-2146 (14L) -- reloads TestIF_File.
//      iUseSuckMode from the recipe file, then either lets
//      AdjustShuttlePlaceOrder run (via ResetShuttleWhichKit) or defers to
//      ChangeHotPlateData if IC is present. THE method MainTempMode.cpp's
//      two SAFETY GATEs (dep-fSetup-ReadUseSuckModeFile, golden :21886/
//      :21893) are waiting on -- see the READ/WRITE AUDIT section below for
//      why it is safe to land un-gated.
//    CheckSTMMode          golden :2148-2177 (30L) -- pure dispatch: resets
//      3 close-site-mode globals to their Standard values, then calls AT
//      MOST one of 5 already-real per-arm-variant functions based on
//      TestIF_File.iTestMode/iInArmType. Zero widget dependency.
//    cbI21Click            golden :4804-4828 (25L) -- the "Enable auto site
//      mapping function" checkbox's guard: refuses to let the operator
//      manually close Auto-Site-Mapping while there is still IC in the
//      machine (re-checks it, re-sets the checkbox back on, and warns).
//  Total: 3 methods / 69 golden lines this file. Denominator: TfSetup:: has
//  47 distinct methods tree-wide (same count Wave A verified,
//  forms/fSetup.h's own banner); 18/47 now have real bodies (15 Wave A + 3
//  here); the other 29 are queued -- see this wave's hand-off report for the
//  per-method reason (write-path / ctor-array-blocked / TScrollBar-zero-port
//  / TImage-zero-port / fQwertyKey-gated / fPassword-unported / other
//  established GATEs, each cited against real golden line ranges read this
//  wave, not inferred from name/size alone).
//
//  READ/WRITE AUDIT -- ReadUseSuckModeFile (task-mandated, 20260821):
//    * `GetRecipeFileName("HandlerCondition.Data")` resolves to
//      `DataPath + GetLastOpenFN() + "HandlerCondition.Data"` (common.cpp:
//      2410-2414) -- the per-recipe RUNTIME DATA directory family, NOT
//      system\Gerneral.ini and NOT ProductionInfo's *.bin family. Distinct,
//      already-real, already-used-tree-wide path.
//    * `ReadIniData(FileName, Group, Name, int Default)` (common.cpp:722-732)
//      is a PURE READ: `INIFile->ReadInteger(Group, Name, Value)` with the
//      caller's Value as fallback on open failure. NO CheckAndReadIniData-
//      style missing-key write-back, no MyForceDirectories, no bare
//      WriteIniData anywhere in ReadUseSuckModeFile's own golden body
//      (grepped this wave: `ReadUseSuckModeFile`'s 14 golden lines contain
//      exactly one I/O call, the ReadIniData above).
//    * The ONE systemic risk that DOES apply here is not unique to this
//      function: `OpenIniFile` (common.cpp:432-443) keeps a single global
//      `TIniFile *INIFile`, and opening a DIFFERENT filename than the one
//      currently held calls `CloseIniFile()` first, which calls
//      `INIFile->UpdateFile()` (common.cpp:457) -- a FLUSH that reformats
//      whatever file was PREVIOUSLY open (strips comments/blank lines,
//      values unchanged; the same "TIniFile flush毀排版" shape as the
//      Gerneral.ini 20260817 incident memory). This is TRUE of every single
//      ReadIniData call site already active tree-wide (csystem.cpp alone has
//      59), not a NEW vector this function introduces -- ReadUseSuckModeFile
//      adds no new risk beyond what every other already-shipped ReadIniData
//      caller already carries.
//    * `ResetShuttleWhichKit()` (ainarm2.cpp:3971-3989, ACTIVE) is PURE
//      in-memory logic (calls AdjustShuttlePlaceOrder() or
//      ChangeHotPlateData() depending on LastSet.iTemperature/MOT[].HasIC())
//      -- no file I/O of its own.
//    CONCLUSION: ReadUseSuckModeFile has NO counter.clear-shape write risk,
//    no MyForceDirectories, no bare WriteIniData. It is safe to land
//    un-gated as a genuine (a) 顯示/讀取側 method. MainTempMode.cpp's two
//    gates are UNCHANGED this wave (MainTempMode.cpp is out of this wave's
//    edit boundary) -- whether/when the main loop un-gates them is now
//    blocked ONLY on (1) wiring this file into CMakeLists' ht9045_sm source
//    list (see CMAKE STATUS above) and (2) the scratch-redirect window the
//    task brief flagged for `LoadMachineConfig()`-adjacent tools -- NOT on
//    any further code gap in ReadUseSuckModeFile itself.
//
//  DEPENDENCY AUDIT (this wave, 20260821) -- every non-widget global/
//  function these 3 methods touch was grepped against this port tree before
//  translating; all resolve inside `ht9045_sm` or its own declared PUBLIC
//  dependencies (ht9045_globals directly; ht9045_core transitively via
//  ht9045_sm -> ht9045_secsgem -> ht9045_core, CMakeLists.txt:2209/:1165 --
//  the same path csystem.cpp's own 59 ReadIniData/GetRecipeFileName call
//  sites already rely on):
//    aHotPlateSubstrate.h  -- InArmSuck / OutArmSuck (TMyKitSuck) / MOT[] /
//                             ResetShuttleWhichKit / iCloseSiteModeFor2x6 /
//                             iCloseSiteModeFor2x8 / iCloseSiteModeFor1x4
//    csystem.h             -- IndexHasIC() / ShuttleHasIC()
//    canary_support.h      -- LastSet (-> LastSet.h, ht9045_globals) /
//                             ShowMyMessage
//  CheckSTMMode's 5 per-arm-variant dispatch targets + the e2x6Standard/
//  e1x4Standard enum values are declared LOCALLY below instead of by
//  including ainarm9045_2x6_8.h/ainarm9045_2x8_8.h/ainarm9045_1x4_4.h/
//  ainarm9045_1x3_2_14.h -- discovered this wave (g++ -fsyntax-only,
//  20260821): ainarm9045_2x6_8.h's `enum e2x6Mode` and ainarm9045_1x4_4.h's
//  `enum e1x4Mode` each redeclare an enumerator name
//  (e2x6OneByOne / e1x4CloseAbAc) that aHotPlateSubstrate.h ALREADY shims as
//  its own anonymous enum for unrelated callers (aHotPlateSubstrate.h:878/
//  :975) -- `enum { e2x6OneByOne = 2 };` vs `enum e2x6Mode { ...,
//  e2x6OneByOne=2, ... };` in the same TU is a hard redeclaration error, not
//  a mere warning. (ainarm9045_2x8_8.h/ainarm9045_1x3_2_14.h carry no such
//  clash and could be included safely today, but are left out too, for
//  consistency and so this file does not implicitly depend on
//  aHotPlateSubstrate.h never adding a similar shim for them later.) Same
//  "declare just the symbol needed" posture as NewRecordProcess below; all
//  5 functions are void() with no default arguments, so there is no
//  default-argument collision risk the way NewRecordProcess has.
//
//  NewRecordProcess -- declared locally with the SAME signature Command.cpp
//  (:282) and MainTempMode.cpp (:57) already declare it with, for the SAME
//  reason documented at both sites: including cMyDB.h or acatchtray_shims.h
//  here would reintroduce THEIR OWN NewRecordProcess declaration, which
//  collides on default arguments in the same TU. Real body today is the
//  logging-sink stub at acatchtray_shims.cpp:145 (MainTempMode.cpp's own
//  banner already documents this as the accepted current behaviour, not a
//  gap introduced here).
// =============================================================================
#include "forms/fSetup.h"

#include "aHotPlateSubstrate.h"      // InArmSuck / OutArmSuck / MOT[] / ResetShuttleWhichKit /
                                     // iCloseSiteModeFor2x6 / iCloseSiteModeFor2x8 / iCloseSiteModeFor1x4
#include "csystem.h"                 // IndexHasIC() / ShuttleHasIC()
#include "canary_support.h"          // LastSet (-> LastSet.h) / ShowMyMessage

// AI(W906-FW-SETUP-B) 20260821: same "declare just the one symbol needed"
// posture Command.cpp (:282) and MainTempMode.cpp (:57) already established
// for this exact function -- see this file's header-comment for why.
void NewRecordProcess(AnsiString AlarmCode, AnsiString S, AnsiString Debug=" ");

// AI(W906-FW-SETUP-B) 20260821: CheckSTMMode's 5 per-arm-variant dispatch
// targets -- declared locally, not via #include, to sidestep the
// enum-redeclaration clash with aHotPlateSubstrate.h documented above.
void CheckSTMMode_2x6_8();                                       // ainarm9045_2x6_8.h:38
void CheckSTMMode_2x8_8();                                       // ainarm9045_2x8_8.h:46
void CheckCloseSiteMode_1x3_14();                                 // ainarm9045_1x3_2_14.h:20
void CheckCloseSiteMode_2x3N_14();                                // ainarm9045_1x3_2_14.h:21
void CheckCloseSiteMode_1x4_4();                                  // ainarm9045_1x4_4.h:31

// e2x6Standard/e1x4Standard (=0 each) -- the two enum values CheckSTMMode
// needs from ainarm9045_2x6_8.h's `enum e2x6Mode`/ainarm9045_1x4_4.h's
// `enum e1x4Mode`, without pulling in either header (see redeclaration note
// above). e2x8Standard needs no such shim -- it is MachineType.h's own
// `enum e2x8Mode` (already visible via forms/fSetup.h -> cprod.h ->
// MachineType.h), a distinct header with no clash.
static const int e2x6Standard = 0;                                // ainarm9045_2x6_8.h:25
static const int e1x4Standard = 0;                                // ainarm9045_1x4_4.h:21

// ---------------------------------------------------------------------------
// TfSetup::ReadUseSuckModeFile
//   BCB6 source: cSetUp.cpp:2133-2146 (14 lines)
// ---------------------------------------------------------------------------
void TfSetup::ReadUseSuckModeFile()
{
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
    TestIF_File.iUseSuckMode=ReadIniData(szDir, "Configuration", "Use Suck Mode", 4);

    // golden :2138-2145 -- if any live IC is under the machine, leave the
    // shuttle-place order alone (empty golden if-body, translated verbatim);
    // otherwise re-run AdjustShuttlePlaceOrder via ResetShuttleWhichKit.
    if(IndexHasIC() || ShuttleHasIC() || InArmSuck.HasIC() ||
       MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())                          //Steven 20160213 : 解決Input功能順序問題
    {
    }
    else
    {
        ResetShuttleWhichKit();
    }
}

// ---------------------------------------------------------------------------
// TfSetup::CheckSTMMode
//   BCB6 source: cSetUp.cpp:2148-2177 (30 lines)                              //Steven 20221006 : 確認STM模式
// ---------------------------------------------------------------------------
void TfSetup::CheckSTMMode()
{
    iCloseSiteModeFor2x8=e2x8Standard;                                         //JerryYang 20191122 STM 8 site Auto clean支援前後排一起收
    iCloseSiteModeFor2x6=e2x6Standard;                                         //Steven 20240417 : 12site分成前後4site
    iCloseSiteModeFor1x4=e1x4Standard;

    if(TestIF_File.iTestMode==_12Site2X6)
    {
        CheckSTMMode_2x6_8();
    }
    else if(TestIF_File.iTestMode==_16Site2X8 ||
            TestIF_File.iTestMode==_32Site4X8N)
    {
        CheckSTMMode_2x8_8();
    }
    else if(TestIF_File.iTestMode==TriSite1X3 &&
            iInArmType==e9045_1x3_2_14)
    {
        CheckCloseSiteMode_1x3_14();                                           //Steven 20251113 : for 1x3_14 run 3x7 HP
    }
    else if(TestIF_File.iTestMode==_6Site2X3N &&
            iInArmType==e9045_1x3_2_14)
    {
        CheckCloseSiteMode_2x3N_14();                                           //Steven 20251113 : for 1x3_14 run 3x7 HP
    }
    else if(TestIF_File.iTestMode==QualSite1X4)
    {
        CheckCloseSiteMode_1x4_4();                                            //Steven 20241111 : for 1x4 close site
    }
}

// ---------------------------------------------------------------------------
// TfSetup::cbI21Click
//   BCB6 source: cSetUp.cpp:4804-4828 (25 lines)                              //Ifor 20210913 add:偵測Site Mapping
//   golden `TObject *Sender` dropped, never read.
// ---------------------------------------------------------------------------
void TfSetup::cbI21Click()
{
    if(CosFunction.bI21EnableASMByRecipe)                                      //Steven 20210518 : 針對客戶要求Auto site map開工作時開啟
    {
        if(cbI21->Checked==false    &&
           CosFunction.bAutoSiteMappingUseHotPlate==true    &&
           IniConfig.bI21AutoSiteMappingUseHotplate==true   &&
           LastSet.iRunStartMode==rsmAutoSiteMap            )
        {
            if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false &&
               ShuttleHasIC()==false    && IndexHasIC()==false)
            {
                NewRecordProcess("", "Has no device, manually close AutoSiteMapping OK!", "沒device，手動關閉AutoSiteMapping OK!");
            }
            else
            {
                if(cbI21->Checked==false)
                {
                    cbI21->Checked=true;
                    ShowMyMessage("Device remain in machine, do onecycle first, and can close AutoSiteMapping!","裝置remain device，請先執行onecycle才能夠關閉AutoSiteMapping");
                }
            }
        }
    }
}
