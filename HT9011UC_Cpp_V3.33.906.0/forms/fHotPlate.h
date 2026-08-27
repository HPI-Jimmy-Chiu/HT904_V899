// =============================================================================
//  forms/fHotPlate.h  --  non-VCL facade for golden's TfHotPlate (cHotPlate.h)
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1 (facade wave 1 of
//  cHotPlate -- read-only direction, no write path). Golden source of truth:
//  HT9011UC_Code_V3.33.906.0_20260618/cHotPlate.h (75 lines) + cHotPlate.cpp
//  (770 lines). Decoded this wave with `io.open(p, encoding='cp950')`,
//  written out UTF-8, 0 U+FFFD (self-checked, see banner tail).
//
//  ROLE
//  ----
//  TfHotPlate is golden's Hot-Plate setup dialog: X/Y start/pitch/division per
//  hot plate, which of HotPlate1/HotPlate2 is enabled, and (on Save) writes
//  those values into HotPlate.Data and re-derives the arm's Y-pitch geometry.
//
//  `python tools/census/wave_preflight.py cHotPlate.cpp` (20260828, re-run at
//  closeout, see GATE REGISTER note below): 19 bodies = 19 `TfHotPlate::`
//  members + 0 file-scope, 718 span lines, 0 brace-swallow short-fall, 0
//  overload collapse. THIS WAVE: 7 of 19 ACTIVE (36.8%) + 11 GATED (declared,
//  not defined) + 1 OMITTED ENTIRELY (FormShortCut, missing type, see below).
//  BY LINES: the 7 ACTIVE bodies' golden spans total 149 of 718 lines (20.7%)
//  -- CheckHotPlate8PickMode (42L) is the largest ACTIVE delivery; ReadFile
//  (155L, GATE) and SaveSetupFile (147L, GATE) together are 2x this whole
//  wave's ACTIVE total. Quote both numbers or neither.
//
//  ============================================================================
//  *** THE GLOBALS `TfHotPlate` / `fHotPlate` ARE FREE -- THIS WAVE CLAIMS
//  THEM *** (verified 20260828, comment-and-string-stripped scan, all
//  .h/.hpp/.cpp under this tree excluding build*/.git/.svn/.pti_frames/web
//  via `python tools/census/wave_candidates.py`): `class TfHotPlate` and
//  `TfHotPlate *fHotPlate` -- 0 hits outside this file/forms/fHotPlate.cpp.
//  `.h` declares `extern TfHotPlate *fHotPlate;`, `.cpp` defines
//  `TfHotPlate *fHotPlate = new TfHotPlate();` -- same idiom as
//  forms/fCleaning.cpp:45 / forms/fMesSystem.cpp:597 / forms/fLotInfo.cpp:23.
//
//  ============================================================================
//  NINE `#if 0`-GATED CALL SITES ELSEWHERE IN THE TREE -- NAMES/TYPES ARE
//  PRE-COMMITTED, NOT THIS WAVE'S CHOICE (measured by wave_preflight.py
//  20260828, re-checked by hand against each site)
//  ============================================================================
//    DoIniDataToForm()   csystem.cpp:29307      -- matches this wave's ACTIVE
//                                                   `void DoIniDataToForm();`
//    SaveSetupFile(...)  csystem.cpp:29308      -- matches this wave's GATED
//                                                   `void SaveSetupFile(AnsiString,AnsiString);`
//    fShow               Command.cpp:10244,15032 -- matches `bool fShow;`
//    XST1/YST1/XPitch1/YPitch1/XCT1/YCT1
//                         ProductionInfo/uPAT_Function.cpp:1769-1774
//                                                -- matches this wave's own
//                                                   `TEdit *XST1;` etc (see
//                                                   FIELD LIST below)
//  These 9 sites are STILL `#if 0` after this wave -- declaring the matching
//  members does NOT un-gate them (separate, deliberate edits to those OTHER
//  files, out of this wave's write boundary). See DISCLOSURE at the bottom.
//
//  ============================================================================
//  GATE REGISTER (12 -- 11 declared-not-defined + 1 omitted entirely)
//  ============================================================================
//  (G-1) ReadFile              golden :154-308 (155L) -- HIDDEN WRITE. Line
//        :203 `HotPlateForm_File.bTrayHotplateCheck=CheckAndReadIniData(szDir,
//        "System","bTrayHotplateCheck",true);` (guarded by
//        `IniConfig.bVTESTFunction==true`). `CheckAndReadIniData`
//        (common.cpp:603) WRITES BACK the default value when the key is
//        missing (common.cpp:1567-1569) -- this is NOT the same function as
//        the pure-read `ReadIniData` (common.cpp:692) that every OTHER line
//        in this function uses. Per task rule ("隱性寫檔陷阱...逐段確認用哪一
//        支"), one disqualifying line gates the WHOLE function -- NOT split,
//        because silently dropping that one assignment would be an invented
//        behaviour change (task rule: 忠實優先, 不要寫退化版本), and both
//        ReadFile's own trailing calls (`i8PickerHPMode=
//        CheckHotPlate8PickMode(); SetArmHotPlateYPitch();`, golden :306-307)
//        would otherwise have been reachable from a caller that never
//        exists in THIS wave's ACTIVE set anyway (FormShow, G-3, is itself
//        gated on other grounds). Same "one disqualifying line gates the
//        whole body" idiom as forms/fMesSystem.h GATE (W-09) RunModeRW.
//  (G-2) LoadImage (private)   golden :147-152 -- MISSING TYPE. `Image1/
//        Image2/Image3` are golden `TImage*` (`->Picture->LoadFromFile(...)`);
//        vclcompat has no `TImage`/`TPicture` anywhere (`grep -rn "class
//        TImage\|class TPicture" vclcompat/` -- 0 hits, 20260828, matches the
//        task brief's own stated gap list). Declared (private, matching
//        golden visibility), not defined.
//  (G-3) FormShow               golden :35-145 (111L) -- TRANSITIVE. Calls
//        `LoadImage()` (G-2, missing type) at :38 and `ReadFile()` (G-1,
//        hidden write) at :39 unconditionally as its first two statements --
//        gated on BOTH grounds independently before any of its other ~15
//        global reads (LastSet.iLanguageCountry / USE_ROTATE_KIT /
//        CUSTOMER_CODE / fSecurity->Insufficient / IniConfig.bVTESTFunction
//        / bHasPlateCSV / fConfiguration->... -- all individually verified
//        reachable or gate-worthy this wave, see DISCLOSURE) even get
//        evaluated. Same "a method that calls a gated method is itself
//        gated" transitive rule as every prior facade wave.
//  (G-4) FormClose               golden :401-410 -- TRANSITIVE via ReadFile
//        (G-1) at :404 (`ReadFile(); DoIniDataToForm();`). DoIniDataToForm
//        itself is this wave's OWN ACTIVE delivery, but the transitive rule
//        still applies because ReadFile precedes it in the same body.
//  (G-5) cbSelectHPFromDBChange   golden :412-438 (27L) -- TWO INDEPENDENT
//        GATES. (a) LINK BOUNDARY: `Barcode_Reader(bcPlateForm)` (:417) is
//        REAL (BarcodeReader.h/.cpp) but its `add_library` block is
//        `ht9045_sm` (CMakeLists.txt:2129, confirmed this wave by locating
//        the enclosing `add_library` line with `awk`), and this facade's
//        expected landing is `ht9045_forms` (same layer as forms/fCleaning.cpp
//        / forms/fMesSystem.cpp), which does NOT link `ht9045_sm`
//        (CMakeLists.txt:769-776). (b) MISSING SYMBOL: `fConfiguration->
//        sbtReloadHP->Click()` / `->strngrdHP->Cells[...]` (:429-436) --
//        `fConfiguration` IS a real, live global but is ALREADY BOUND to
//        `W5SckArtRem_ConfigStub*` (Automation/SCK_ART_Remainder.h:594-599),
//        which carries exactly one member (`mmoN04_IP`); `sbtReloadHP`/
//        `strngrdHP` do not exist on that type (same X-1 gap forms/
//        fMesSystem.h GATE (X-1) already documented for the SAME
//        `fConfiguration` stub, re-verified independently this wave).
//  (G-6) spbSaveClick             golden :440-474 (35L) -- TRANSITIVE via
//        `SaveSetupFile(szDir,S)` (:460, G-7, write) unconditionally.
//  (G-7) SaveSetupFile            golden :479-625 (147L) -- WRITES DISK. 8x
//        `WriteIniData(szDir,"Hotplate Form",...)` (:597-624) unconditionally
//        (name itself says "save" -- task brief's own flagged-suspicious
//        name, read in full before deciding, per task rule).
//  (G-8) XST1MouseDown            golden :379-387 -- LINK BOUNDARY, same
//        `Barcode_Reader` reason as (G-5)(a) -- called at :382 before the
//        (otherwise-reachable) `fQwertyKey->ShowQwertyKey(...)` at :386.
//  (G-9) XCT1MouseDown            golden :389-399 -- same as (G-8), Barcode_
//        Reader at :392.
//  (G-10) ShowTypePage(int)       golden :744-759 (16L) -- MISSING SYMBOL,
//        same `fConfiguration->sbtReloadHP->Click()` / `->strngrdHP->
//        Cells[...]` reason as (G-5)(b).
//  (G-11) SetArmHotPlateYPitch    golden :682-742 (61L) -- LINK BOUNDARY.
//        Every line but the LAST is pure global arithmetic on TestIF_File /
//        HotPlateForm_File (all `ht9045_globals`, reachable) -- would have
//        been this wave's SECOND-LARGEST ACTIVE delivery had the final,
//        unconditional line not existed: `GetHotPlateYHalfPos();` (:741).
//        That function's ONLY definition is `ainarm_SearchPlacePlate.cpp:132`,
//        inside the `ht9045_sm` `add_library` block (CMakeLists.txt:1619,
//        confirmed via `awk` this wave) -- unreachable from `ht9045_forms`,
//        same boundary shape as forms/fMesSystem.h GATE (L-1)/(L-2)'s
//        `TestSocket`. NOT split (dropping the unconditional trailing call
//        would silently change behaviour). Re-visit target for a future
//        `ht9045_sm`-reaching wave.
//  (G-12) FormShortCut            golden :761-768 -- OMITTED ENTIRELY, not
//        even declared. `TWMKey &Msg` has ZERO port anywhere in this tree
//        (`grep -rn "TWMKey" .` -- 0 hits outside comments, 20260828) --
//        same established precedent as forms/fContact.h GATE (X-27) /
//        forms/fSetup.h / forms/fTemp_Set.h's own FormShortCut notes (a
//        declaration would have to invent the type itself, out of this
//        wave's write boundary).
//
//  ============================================================================
//  ACTIVE (7) -- reachability evidence
//  ============================================================================
//   TfHotPlate()             golden :28-31 -- empty body (`: TForm(Owner) {}`
//        in golden), fields-only, no global deref. SIOF-safe (docs/KNOWLEDGE.md
//        "static-init ctor 不可碰 NULL 全域" -- this ctor touches nothing).
//   DoIniDataToForm()        golden :310-365 (56L) -- pure widget reflection
//        off `HotPlateForm_File` (cprod.cpp:472, `ht9045_globals`),
//        `TestIF_File.iAutoClean_Function` (cprod.h:1696, same struct/target),
//        `IniConfig.b*` (Config.cpp:496, `ht9045_globals`),
//        `CosFunction.bAutoCleanUseHPSetByRecipe` (CosFunction.cpp:508, same),
//        `USE_PRECISER`/`iPreciserInstallArea` (cmydef.cpp, same),
//        `FormatFloat` (vclcompat/SysUtils.cpp, `vclcompat`). No write, no
//        transmit, no machine action. Pre-committed name (see 9-SITE list).
//   XST1KeyPress(char&)      golden :367-371 -- `OnlyNumberAndDotInPut`
//        (common.cpp:1436, `ht9045_core`).
//   YCT1KeyPress(char&)      golden :373-377 -- `OnlyNumberInPut`
//        (common.cpp:1435, `ht9045_core`).
//   HotPlateNameMouseDown(...) golden :633-637 -- `fQwertyKey->ShowQwertyKey`
//        (forms/fQwertyKey.cpp, `ht9045_forms` -- confirmed via `awk` this
//        wave, same target this file expects to land in).
//   CheckHotPlate8PickMode() golden :639-680 (42L) -- pure read+return over
//        `HotPlateForm_File`/`HotPlateForm`/`IniConfig.bHotPlateMove1CM`/
//        `TestIF.iTestMode`/`TestIF.iUseSuckMode` (cprod.cpp, `ht9045_globals`)
//        and `HOT_PLATE_POSITION`/`iHPWideHP`/`iHPNoUse8suck` (cmydef.cpp,
//        same). No write, no widget touch at all.
//   sbtExitClick(TObject*)   golden :627-631 -- `sbtExit->Down=false;` (own
//        widget) + `Close();`. Golden's `Close()` is inherited `TForm::Close`;
//        this facade has no TForm base (DEVIATION D-2 below), so a port-only
//        no-op `Close(){}` is declared (same idiom as forms/fQwertyKey.h:366 /
//        BarcodeReader.h:108), making this body a full, faithful translation
//        rather than a partial one.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `TfHotPlate` has NO base class (matches forms/fCleaning.h /
//      forms/fMesSystem.h D-4 -- not `: public TForm`; no delivered method
//      needs a TForm base beyond the two no-ops below).
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- golden's own `TForm::Close`,
//      not translated (D-2). Needed by the ACTIVE `sbtExitClick`.
//  (D-4) `FormShortCut` OMITTED ENTIRELY -- see GATE (G-12).
//  (D-5) `LoadImage` kept `private` (matches golden visibility) even though
//      GATED -- declared-not-defined either way, visibility is free.
//
//  ============================================================================
//  FIELD LIST (12 widgets + 1 bool; only what an ACTIVE body OR a
//  pre-committed #if 0 site touches -- minimal-declaration idiom, matches
//  forms/fMesSystem.h's own "misc golden members NOT declared" practice)
//  ============================================================================
//    fShow (bool)                                          -- pre-committed
//    HotPlateName/XST1/YST1/XPitch1/YPitch1/XCT1/YCT1 (TEdit*) -- DoIniDataToForm
//      (XST1/YST1/XPitch1/YPitch1/XCT1/YCT1 ALSO pre-committed, see 9-SITE list)
//    cbEnableHP1/cbEnableHP2/chkUseWideHotplate/chkTrayHotplateCheck (TCheckBox*)
//      -- DoIniDataToForm
//    sbtExit (TSpeedButton*)                                -- sbtExitClick
//  NOT declared (touched only by a GATED body, whose prototype carries no
//  field dependency of its own): Panel1/Panel2/Label1-8/GroupBox1/GroupBox2/
//  Image1/Image2/Image3/cbSelectHPFromDB/spbSave.
//
//  ============================================================================
//  ZERO-WRITER FIELDS -- does not apply to this wave's delivered set
//  ============================================================================
//  None of the 7 ACTIVE bodies write a field that only a GATED body reads (or
//  vice versa) in a way that would silently misbehave -- DoIniDataToForm
//  READS `HotPlateForm_File`, whose only WRITER in this file is ReadFile
//  (G-1, gated), so `HotPlateForm_File` stays at its process-wide default
//  (zero-initialised extern) for as long as G-1 stays gated. This is an
//  EXTERNAL global (cprod.cpp), not a field this wave owns -- flagged for
//  disclosure (task rule 7) but not a "zero-writer field" in the sense of a
//  member this wave itself introduces with no writer.
//
//  ============================================================================
//  DISCLOSURE (task rule 6 -- not read to completion / not verified further)
//  ============================================================================
//  - FormShow's OWN remaining ~14 statements past the 2 that gate it (G-3)
//    were read for gate-classification purposes only, NOT individually
//    verified reachable (e.g. `fSecurity->Insufficient(15,false)`,
//    `LastSet.iLanguageCountry`, `bHasPlateCSV`) -- moot while LoadImage/
//    ReadFile gate the whole function, but a future wave lifting THOSE two
//    gates must re-verify the rest from scratch.
//  - SaveSetupFile (G-7) and ReadFile (G-1) were read in FULL (both are
//    task-brief-flagged "name says write/read" cases) but their internal
//    global surface beyond the disqualifying lines was not individually
//    traced for reachability (moot, both are gated regardless).
//  - `#ifdef ASE_KaohSiung` branch inside spbSaveClick (golden :461-463,
//    `fBuilder->bSaveAsJobFile(...)`) not investigated -- moot, spbSaveClick
//    is gated on SaveSetupFile alone (G-6).
// =============================================================================
#ifndef FORMS_FHOTPLATE_H
#define FORMS_FHOTPLATE_H

#include "forms/FormWidgets.h"   // AnsiString / TEdit / TCheckBox / TSpeedButton
#include "vclcompat/ShiftState.h" // TMouseButton / TShiftState (HotPlateNameMouseDown/XST1MouseDown/XCT1MouseDown)

// ===========================================================================
//  TfHotPlate -- non-VCL facade (golden cHotPlate.h:12-70, `class TfHotPlate :
//  public TForm`; NO base class here, see DEVIATION D-2).
// ===========================================================================
class TfHotPlate
{
public:
    TfHotPlate();

    // -- golden public: User declarations -----------------------------------
    bool fShow = false;                                    // golden :68, pre-committed (Command.cpp:10244,15032)

    // -- golden __published widgets touched by ACTIVE bodies or pre-committed
    //    #if 0 sites (see FIELD LIST above) -----------------------------------
    TEdit *HotPlateName = new TEdit();                      // golden :17
    TEdit *XST1          = new TEdit();                     // golden :31, pre-committed (uPAT_Function.cpp:1769)
    TEdit *XPitch1        = new TEdit();                    // golden :32, pre-committed (uPAT_Function.cpp:1771)
    TEdit *XCT1           = new TEdit();                    // golden :33, pre-committed (uPAT_Function.cpp:1773)
    TEdit *YST1           = new TEdit();                    // golden :34, pre-committed (uPAT_Function.cpp:1770)
    TEdit *YPitch1        = new TEdit();                    // golden :35, pre-committed (uPAT_Function.cpp:1772)
    TEdit *YCT1           = new TEdit();                    // golden :36, pre-committed (uPAT_Function.cpp:1774)
    TCheckBox *cbEnableHP1 = new TCheckBox();                // golden :22
    TCheckBox *cbEnableHP2 = new TCheckBox();                // golden :20
    TCheckBox *chkUseWideHotplate  = new TCheckBox();        // golden :39
    TCheckBox *chkTrayHotplateCheck = new TCheckBox();       // golden :43
    TSpeedButton *sbtExit = new TSpeedButton();               // golden :42

    // -- ACTIVE this wave (defined in forms/fHotPlate.cpp) -------------------
    void DoIniDataToForm();                                  // golden :310-365   ACTIVE, pre-committed (csystem.cpp:29307)
    void XST1KeyPress(char &Key);                            // golden :367-371   ACTIVE
    void YCT1KeyPress(char &Key);                            // golden :373-377   ACTIVE
    void HotPlateNameMouseDown(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y);  // golden :633-637 ACTIVE
    int  CheckHotPlate8PickMode();                            // golden :639-680   ACTIVE
    void sbtExitClick(TObject *Sender);                       // golden :627-631   ACTIVE, needs Close() (D-3)
    void Close() {}                                           // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared, NOT defined. Calling one of these is a
    //    LINK ERROR by design (see GATE REGISTER above). --------------------
    void FormShow(TObject *Sender);                           // golden :35-145    GATE (G-3)
    void ReadFile();                                          // golden :154-308   GATE (G-1)
    void FormClose();                                         // golden :401-410   GATE (G-4), D-5-style no-param (TCloseAction has no port)
    void cbSelectHPFromDBChange(TObject *Sender);              // golden :412-438   GATE (G-5)
    void spbSaveClick(TObject *Sender);                        // golden :440-474   GATE (G-6)
    void SaveSetupFile(AnsiString szDir, AnsiString S);        // golden :479-625   GATE (G-7), pre-committed (csystem.cpp:29308)
    void XST1MouseDown(TObject *Sender, TMouseButton Button,
                        TShiftState Shift, int X, int Y);      // golden :379-387   GATE (G-8)
    void XCT1MouseDown(TObject *Sender, TMouseButton Button,
                        TShiftState Shift, int X, int Y);      // golden :389-399   GATE (G-9)
    void ShowTypePage(int index);                              // golden :744-759   GATE (G-10)
    void SetArmHotPlateYPitch();                                // golden :682-742   GATE (G-11)

private:
    void LoadImage();                                          // golden :147-152   GATE (G-2), private (matches golden)

    // FormShortCut(TWMKey&, bool&) -- OMITTED ENTIRELY, see GATE (G-12) / DEVIATION D-4.
};

// AI(W906-FW3-HSP1) 20260828: TfHotPlate/fHotPlate globals were FREE tree-wide
// (see banner) -- declared here per this wave's task brief so the 9 #if 0
// call sites listed above have something real to bind to once each site's
// OWN file is separately un-gated (not done by this wave).
extern TfHotPlate *fHotPlate;

#endif // FORMS_FHOTPLATE_H
