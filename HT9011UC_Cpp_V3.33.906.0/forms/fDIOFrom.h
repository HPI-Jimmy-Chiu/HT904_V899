// =============================================================================
//  forms/fDIOFrom.h  --  non-VCL facade for golden's TfDIOFrom
//                        (DIOInterFaceCFG.h)
//
//  AI(W906-FW3-BTQ1) 20260828: new file, FW wave FW3-BTQ1 (4 of 5 facades).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/DIOInterFaceCFG.h
//  (70 lines) + DIOInterFaceCFG.cpp (268 lines), read with
//  `io.open(p, encoding='cp950')` (0 U+FFFD), written out UTF-8 / bare-LF.
//
//  ROLE
//  ----
//  TfDIOFrom is golden's TTL / DIO signal-timing page.  It edits the global
//  `TTLCfg` (a TTL_DATA, cprod.h:18-31): start-signal logic and channel, pulse
//  width, DUT before-on / after-off times, and the bin-out signal's logic, bit
//  length and data type.  It loads and saves those as an .ini under
//  `DIOCFGPath` (or, when `IniConfig.bI16TTLSaveInSetupFile` is set, a copy
//  inside the recipe folder).
//
//  ⚠ THIS IS THE FACADE forms/fTesterIF.cpp WAS WRITTEN AGAINST.  Nine of its
//  members are RESERVED NAMES already spelled at nine `#if 0` sites in
//  forms/fTesterIF.cpp (see PRE-COMMITTED below), which is why the spellings
//  and signatures below are not free choices.
//
//  `python tools/census/wave_preflight.py DIOInterFaceCFG.cpp` (20260828,
//  re-run at closeout): 14 bodies = 14 `TfDIOFrom::` members + 0 file-scope,
//  236 span lines, 0 brace-swallow short-fall, 0 overload collapse.
//
//  THIS WAVE, against the 14-member denominator:
//     8 ACTIVE           66 of 236 golden span lines (28.0%) -- the highest
//                        live fraction of the five facades in this wave
//     6 GATED-WITH-BODY  170 golden span lines carried as `#if 0` transcript
//  Quote both numbers or neither.
//
//  ============================================================================
//  *** THE NAMES `TfDIOFrom` / `fDIOFrom` ARE FREE -- THIS WAVE CLAIMS THEM ***
//  ============================================================================
//  Measured 20260828 with `python tools/census/wave_preflight.py
//  DIOInterFaceCFG.cpp` (section [3], 1203 port files scanned): (a) class name
//  `clear`, (c) global name `clear`, (d) TU-local seam `clear`.  (b) reported 4
//  same-NAME members of ONE other class (TFTestIF::DoIniDataToForm / FormClose
//  / FormShow / spbSaveClick) -- a different class, not an occupation.
//  Cross-checked with `rg -c -g '!build_*' -g '*.h' -g '*.cpp' "\bTfDIOFrom\b"
//  .`: forms/fTesterIF.h:1 + forms/fTesterIF.cpp:1 (both PROSE inside gate
//  banners) and the generated tools/dfm2rc/layout_out/
//  DIOInterFaceCFG_layout.gen.cpp + its _b1d_regen twin, where it is a STRING
//  LITERAL in a .dfm widget table -- and `rg -n "layout_out" CMakeLists.txt`
//  returns 0, i.e. neither generated file is in any add_library.
//
//  ⚠ THIS WAVE FALSIFIES ONE OF THE TREE'S OWN COMMENTS, and it is an
//  unusually explicit one.  forms/fTesterIF.cpp:1466-1469 (GATE F-11) says:
//    "no TfDIOFrom class and no fDIOFrom global exist in this tree, and
//     DIOInterFaceCFG.{h,cpp} do not exist here at all (direct stat, 20260828)"
//  True when written, this morning; false after this file lands.  ⚠ NOTE THAT
//  THE SECOND HALF STAYS TRUE IN LETTER: this wave does NOT add
//  `DIOInterFaceCFG.{h,cpp}` -- it adds `forms/fDIOFrom.{h,cpp}`.  Recorded,
//  NOT edited (this wave adds files only); forms/fTesterIF.h:236 carries the
//  same claim.  Both sit above `#if 0` blocks that stay `#if 0`.
//
//  ============================================================================
//  CONDITION-TWO CHECK (does a port of these bodies ALREADY exist, possibly as
//  a RENAMED FREE FUNCTION outside the class?)
//  ============================================================================
//  Searched 20260828, whole port tree, build_* excluded.
//  ⚠ MEASUREMENT CONVENTION: every count below is AS OF THE PRE-WRITE SCAN and
//  EXCLUDES this wave's own forms/fDIOFrom.{h,cpp}.  Re-running afterwards
//  returns higher numbers purely because these banners mention the names in
//  prose -- a comment is not a port.
//    * `DIOFileName`                 0 files
//    * `GetDIOFileName`              2 files -- forms/fTesterIF.h:236 (prose)
//      and forms/fTesterIF.cpp:1534/:1539 (a gate comment and the `#if 0` call
//      site).  0 definitions.
//    * `LoadData` / `InitData` / `DoIniDataToForm` -- deliberately NOT taken as
//      evidence either way: these three names are generic and belong to many
//      unrelated classes in this tree.  The class-scoped check above (`(b)`)
//      is the one that matters, and it found only TFTestIF's own members.
//    * `TTLCfg`                      6 files -- cinitial.cpp, cprod.{h,cpp},
//      forms/fIoSetView.{h,cpp}, forms/fTesterIF.cpp.  ALL are readers of the
//      shared TTL_DATA global this form edits, not ports of any body here.
//      ⚠ forms/fIoSetView.{h,cpp} is the closest call: it is a REAL facade in
//      the SAME ht9045_forms target that displays TTL state.  Read 20260828 --
//      it is golden's `Tfiosetview` (golden iosetview.cpp), a different class
//      reading the same global.  Not a rename of TfDIOFrom.
//  Renaming shapes probed: `rg "\b(DIO_|DIOFrom_|TTLCfg_)\w+"` -> 0 hits.
//  `rg "DIOInterFace|DIOConfig|LoadDIOData"` -> 0 hits.
//  Conclusion: nothing to re-use, nothing re-translated twice.
//
//  ============================================================================
//  PRE-COMMITTED `#if 0` CALL SITES ELSEWHERE (9 names, 9 sites)
//  ============================================================================
//  All nine are RESERVED NAMES; the spellings and shapes below were read off
//  the call sites, not chosen.  Eight are in forms/fTesterIF.cpp, which was
//  committed one wave before this one and pre-reserved them deliberately:
//    rgStartLogic     forms/fTesterIF.cpp:1477  `->rgStartLogic->Items->Strings[i]`
//    rgStartChannel   forms/fTesterIF.cpp:1478  `->rgStartChannel->Items->Strings[i]`
//    cbSignalType     forms/fTesterIF.cpp:1490  `->cbSignalType->Items->Strings[i]`
//    rgBinLogic       forms/fTesterIF.cpp:1505  `->rgBinLogic->Items->Strings[i]`
//    rgBinBitLength   forms/fTesterIF.cpp:1509  `->rgBinBitLength->Items->Strings[i]`
//    rgBinDataType    forms/fTesterIF.cpp:1510  `->rgBinDataType->Items->Strings[i]`
//        -> the six above are all used as `X->Items->Strings[int]`, i.e. the
//           declared type must expose a TStringList* `Items`.  Golden types
//           them TRadioGroup (x5) and TComboBox (x1) -- vclcompat has both and
//           BOTH carry `TStringList *Items` (vclcompat/Controls.h:386-395 and
//           :450-457), so golden's exact types are used unchanged.
//    GetDIOFileName   forms/fTesterIF.cpp:1539  `S=fDIOFrom->GetDIOFileName();`
//        -> returns something assignable to AnsiString.  Declared
//           `AnsiString GetDIOFileName();` -- matches golden verbatim.
//    LoadData         forms/fTesterIF.cpp:1540  `fDIOFrom->LoadData(S);`
//        -> one AnsiString parameter.  Matches.
//    fShow            Command.cpp:10243, Command.cpp:15031 (2 sites, 1 name)
//        -> `fDIOFrom->fShow` in the "is ANY form open" OR chain.  Matches
//           `bool fShow;`.
//  ⚠ THE WAVE BRIEF'S BULLET LIST FOR THIS FORM NAMES ONLY EIGHT.
//  `rgStartLogic` (forms/fTesterIF.cpp:1477) is the ninth and was omitted from
//  that list; wave_preflight section [4] reports all nine.  It is declared here
//  -- omitting it would have left a pre-committed site unable to bind, which is
//  exactly the failure mode reserved names exist to prevent.
//  All nine sites stay `#if 0` after this wave.
//
//  ============================================================================
//  GATE REGISTER (6)
//  ============================================================================
//  (I-1) FormShow(TObject*)  golden :24-45 (22L) -- NO DIALOG PORT.  golden :26
//        is `OpenDialog1->InitialDir=DIOCFGPath;` and neither TOpenDialog nor
//        TSaveDialog exists in vclcompat (`rg "class TOpenDialog|class
//        TSaveDialog" vclcompat/*.h` -> 0 hits, 20260828).  Its other names
//        (Label55/Label4/Label2, JCET_FOR_EVAN) are not declared here either
//        (D-5).  ⚠ NOT SPLIT even though :27 `InitData()` is ACTIVE and :44
//        `fShow=true;` would work: the whole point of golden's FormShow is to
//        prime the file dialog before the operator browses.
//  (I-2) GetDIOFileName()  golden :47-70 (24L) -- WRITES DISK.  RESERVED NAME
//        (forms/fTesterIF.cpp:1539), so it is DECLARED.
//        golden :61 `CopyFile(szDir.c_str(), S1.c_str(), false)` copies the
//        master DIO .ini from DIOCFGPath INTO the recipe folder whenever the
//        recipe copy is missing.  A file creation under D:\HT9045\, on a tree
//        whose experimental and production data are not yet separated.
//        Everything else it needs IS reachable -- DIOCFGPath (common.h:80,
//        common.cpp, ht9045_core), GetLastOpenFN, FileExists, DataPath,
//        IniConfig.bI16TTLSaveInSetupFile -- so this is a PURE SAFETY GATE,
//        not a link gate.  (`FTestIF->cbDIOType` also resolves: forms/
//        fTesterIF.h declares it, same ht9045_forms target.)
//  (I-3) LoadData(AnsiString)  golden :72-131 (60L) -- MODE SWITCH.  RESERVED
//        NAME (forms/fTesterIF.cpp:1540), so it is DECLARED.
//        golden :81 sets the GLOBAL RUN-STATE FLAG `SystemStart=false;` from a
//        UI path when the DIO file is missing.  That is the "模式切換" class
//        of change the project rules call safety-critical, and this wave's
//        read-only direction does not take it.  ⚠ THIS IS A POLICY GATE, NOT A
//        REACHABILITY GATE, and it is cheap to lift with the user's approval:
//        SystemStart is cmydef.h:221 -> cmydef.cpp (ht9045_globals, reachable),
//        ReadIniData/CheckRange/ShowMyMessage are all reachable, and
//        `DoIniDataToForm()` at :130 is ACTIVE.  Recorded explicitly so a
//        future wave re-asks the question rather than inheriting the answer
//        (docs/KNOWLEDGE.md "前提死掉不代表答案就是退役").
//        Secondary, NOT the deciding fact: its only natural caller,
//        GetDIOFileName, is GATE (I-2), so with (I-2) shut LoadData would be
//        handed a path nothing produced.
//  (I-4) spbSaveClick(TObject*)  golden :191-235 (45L) -- WRITES DISK.  Eleven
//        `WriteIniData(DIOFileName, ...)` calls at :222-232, plus
//        `SaveDialog1->Execute()` (no TSaveDialog port, see I-1).  Both a
//        safety gate and a missing-type gate.
//  (I-5) spbLoadClick(TObject*)  golden :237-246 (10L) -- NO DIALOG PORT
//        (`OpenDialog1->Execute()`); also [T] on GATE (I-3) at :243.
//  (I-6) spbDeleteClick(TObject*)  golden :248-256 (9L) -- DELETES A FILE.
//        `DeleteFile(OpenDialog1->FileName)` at :253 -- and vclcompat DOES
//        provide DeleteFile (vclcompat/SysUtils.h), so an un-gated version
//        would really unlink whatever the dialog returned.  Also no dialog
//        port.  Two independent gates.
//
//  ============================================================================
//  ACTIVE (8) -- reachability evidence
//  ============================================================================
//   TfDIOFrom()               golden :19-22.  Golden's body is EMPTY.  Nothing
//        to gate; SIOF-trivially safe (docs/KNOWLEDGE.md "static-init ctor
//        不可碰 NULL 全域").
//   DoIniDataToForm()         golden :133-146.  Eleven TTLCfg -> widget
//        assignments.  TTLCfg is cprod.h:32 -> cprod.cpp, ht9045_globals.
//        This is the READ direction of the form and touches nothing else.
//   InitData()                golden :148-164.  Blanks the eleven widgets and
//        this facade's own `DIOFileName`.  No external symbol at all.
//   Edit3KeyPress             golden :166-170.  `OnlyNumberAndDotInPut(Key)` --
//        common.h, defined common.cpp, ht9045_core.
//   edSignalBeforeOnKeyPress  golden :172-176.  `OnlyNumberInPut(Key)` -- same
//        file, same target.
//   edPulseWidthMouseDown     golden :178-189.  `CosFunction.bTTLUseUSec`
//        (CosFunction.cpp, ht9045_globals) selecting between two
//        `fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, ...)`
//        calls -- fQwertyKey is forms/fQwertyKey.cpp, SAME ht9045_forms
//        target, 6-argument signature at forms/fQwertyKey.h:370.
//   spbExitClick(TObject*)    golden :258-261.  `Close();` (DEVIATION D-3).
//   FormClose()               golden :263-267.  `fShow=false;` plus
//        `DoIniDataToForm();` -- BOTH live, because unlike fQAMode's FormClose
//        this form's DoIniDataToForm is itself ACTIVE.  A FULL translation.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//      (Golden's GetDIOFileName was already plain, DIOInterFaceCFG.h:63.)
//  (D-2) `TfDIOFrom` has NO base class (golden: `: public TForm`).  Matches
//      forms/fCounterSel.h (D-2) / forms/fBuilder.h (D-2).
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- needed by ACTIVE
//      spbExitClick (and named inside gated spbSaveClick).
//  (D-4) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();`.
//      TCloseAction has no port and golden's body reads neither parameter.
//  (D-5) `edPulseWidthMouseDown(TObject*, TMouseButton, TShiftState, int, int)`
//      -> `(TObject *Sender)`.  TMouseButton/TShiftState have no port; golden
//      reads only `Sender`.  Same reduction as forms/fBuilder.h (D-6).
//  (D-6) golden's other 17 `__published` widgets (GroupBox1..4, Label1..6,
//      Label52, Label55, OpenDialog1, SaveDialog1, spbSave, spbLoad,
//      spbDelete, spbExit, Panel1) are NOT declared: no ACTIVE body and no
//      pre-committed site touches them (minimal-declaration idiom --
//      forms/fSetup.h:20-25).  Their names appear only inside `#if 0`.
//  (D-7) golden's `Key=NULL;` on a `char&` is written `Key=0;` in both
//      KeyPress bodies -- identical value, and `NULL` on a char is a
//      C++11-onward warning magnet.  Same as forms/fLd_ULd.cpp:71.
//
//  ============================================================================
//  FIELD LIST (11 widgets + 1 AnsiString + 1 bool)
//  ============================================================================
//    DIOFileName (AnsiString)          golden DIOInterFaceCFG.h:64
//    fShow       (bool)                golden DIOInterFaceCFG.h:65, PRE-COMMITTED
//    rgStartLogic     (TRadioGroup*)   golden :42   PRE-COMMITTED (fTesterIF.cpp:1477)
//    rgStartChannel   (TRadioGroup*)   golden :43   PRE-COMMITTED (fTesterIF.cpp:1478)
//    edPulseWidth     (TEdit*)         golden :44
//    cbOneSTChannel   (TComboBox*)     golden :45
//    cbSignalType     (TComboBox*)     golden :37   PRE-COMMITTED (fTesterIF.cpp:1490)
//    edSignalBeforeOn (TEdit*)         golden :38
//    edSignalAfterOff (TEdit*)         golden :36
//    rgBinLogic       (TRadioGroup*)   golden :17   PRE-COMMITTED (fTesterIF.cpp:1505)
//    rgBinBitLength   (TRadioGroup*)   golden :18   PRE-COMMITTED (fTesterIF.cpp:1509)
//    rgBinDataType    (TRadioGroup*)   golden :19   PRE-COMMITTED (fTesterIF.cpp:1510)
//    edTTLModeName    (TEdit*)         golden :22
//  NOT declared: see DEVIATION (D-6).
//
//  ============================================================================
//  ZERO-WRITER / EMPTY-ITEMS FIELDS (task rule: state the consequence)
//  ============================================================================
//  * `fShow` is written `false` by ACTIVE FormClose and `true` ONLY by GATED
//    FormShow (:44).  CONSEQUENCE while (I-1) stays gated: permanently false,
//    so Command.cpp:10243 / :15031 would answer "this form is never open".
//  * ⚠ THE SIX PRE-COMMITTED `Items` LISTS ARE EMPTY, and this one matters.
//    forms/fTesterIF.cpp's GATE (F-11) reads them as
//    `rgStartLogic->Items->Strings[TTLCfg.iSTLogicMode]` etc.  In golden those
//    Items come from the .dfm at design time; NOTHING in this port loads a
//    .dfm (dfm2rc keeps the strings in web/forms/*.layout.json, and no C++
//    reads a layout).  So every one of those six lists has Count==0 today.
//    Whoever un-gates (F-11) must supply the item strings FIRST -- this is
//    exactly docs/KNOWLEDGE.md's "解gate前先查「值從哪來」": the type being
//    present is not the same as the value being available, and an
//    out-of-range `Strings[i]` is not a harmless zero.
//  * `DIOFileName` is written "" by ACTIVE InitData and to a real path only by
//    GATED spbSaveClick / spbLoadClick.  CONSEQUENCE: gated spbSaveClick's
//    own `if(DIOFileName=="") return;` guard (golden :200-201) would fire
//    immediately anyway.
//
//  ============================================================================
//  DISCLOSURE (task rule 8 -- what was NOT read to completion / NOT verified)
//  ============================================================================
//  - forms/fTesterIF.cpp was read only around :1460-1620 and :1700-1754 (the
//    nine pre-committed sites and their gate banners).  The rest of that
//    1754-line file was NOT read; in particular no check was made for OTHER
//    `fDIOFrom->` spellings outside that window -- but wave_preflight's
//    section [4] scans the whole tree and reports exactly nine names, which is
//    the number reconciled above.
//  - `JCET_FOR_EVAN` (golden :38, :98) was NOT resolved to a port.  It appears
//    only inside GATE (I-1) and GATE (I-3).
//  - The bit-length enumerators `_5BitPE` / `_5BitPO` / `_10BitPE` /
//    `_10BitPO` / `TTL_MODE` (golden :78, :111-114) were NOT confirmed to
//    exist in this port; all are inside GATE (I-3).
//  - `IniConfig.bI16TTLSaveInSetupFile` (golden :51) was NOT confirmed against
//    Config.h.  Inside GATE (I-2).
//  - golden `DIOInterFaceCFG.dfm` was NOT opened -- which is precisely why the
//    empty-Items consequence above is stated rather than fixed.  Reading it is
//    the natural first step for whoever un-gates forms/fTesterIF.cpp (F-11).
// =============================================================================
#ifndef FORMS_FDIOFROM_H
#define FORMS_FDIOFROM_H

#include "forms/FormWidgets.h"   // AnsiString / TObject / TEdit / TComboBox /
                                 // TRadioGroup (vclcompat)

// ===========================================================================
//  TfDIOFrom -- non-VCL facade (golden DIOInterFaceCFG.h:13-66,
//  `class TfDIOFrom : public TForm`; NO base class here, DEVIATION D-2).
// ===========================================================================
class TfDIOFrom
{
public:
    TfDIOFrom();

    // -- golden public: User declarations ------------------------------------
    AnsiString DIOFileName;                             // golden DIOInterFaceCFG.h:64
    bool       fShow = false;                           // golden DIOInterFaceCFG.h:65, PRE-COMMITTED (Command.cpp:10243, Command.cpp:15031)

    // -- golden __published widgets.  The six marked PRE-COMMITTED are read as
    //    `->Items->Strings[i]` by forms/fTesterIF.cpp's GATE (F-11); golden's
    //    own types (TRadioGroup / TComboBox) both carry `TStringList *Items`,
    //    so no substitution was needed.  ⚠ Those Items lists are EMPTY in this
    //    port -- see the header's ZERO-WRITER section before un-gating (F-11).
    TRadioGroup *rgStartLogic     = new TRadioGroup();  // golden :42  PRE-COMMITTED (forms/fTesterIF.cpp:1477)
    TRadioGroup *rgStartChannel   = new TRadioGroup();  // golden :43  PRE-COMMITTED (forms/fTesterIF.cpp:1478)
    TEdit       *edPulseWidth     = new TEdit();        // golden :44
    TComboBox   *cbOneSTChannel   = new TComboBox();    // golden :45
    TComboBox   *cbSignalType     = new TComboBox();    // golden :37  PRE-COMMITTED (forms/fTesterIF.cpp:1490)
    TEdit       *edSignalBeforeOn = new TEdit();        // golden :38
    TEdit       *edSignalAfterOff = new TEdit();        // golden :36
    TRadioGroup *rgBinLogic       = new TRadioGroup();  // golden :17  PRE-COMMITTED (forms/fTesterIF.cpp:1505)
    TRadioGroup *rgBinBitLength   = new TRadioGroup();  // golden :18  PRE-COMMITTED (forms/fTesterIF.cpp:1509)
    TRadioGroup *rgBinDataType    = new TRadioGroup();  // golden :19  PRE-COMMITTED (forms/fTesterIF.cpp:1510)
    TEdit       *edTTLModeName    = new TEdit();        // golden :22

    // -- ACTIVE this wave (defined in forms/fDIOFrom.cpp) --------------------
    void DoIniDataToForm();                             // golden :133-146  ACTIVE
    void InitData();                                    // golden :148-164  ACTIVE
    void Edit3KeyPress(TObject *Sender, char &Key);     // golden :166-170  ACTIVE, DEVIATION D-7
    void edSignalBeforeOnKeyPress(TObject *Sender, char &Key); // golden :172-176 ACTIVE, DEVIATION D-7
    void edPulseWidthMouseDown(TObject *Sender);        // golden :178-189  ACTIVE, DEVIATION D-5
    void spbExitClick(TObject *Sender);                 // golden :258-261  ACTIVE
    void FormClose();                                   // golden :263-267  ACTIVE (FULL), DEVIATION D-4
    void Close() {}                                     // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fDIOFrom.cpp inside `#if 0`.  Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). -------------------------------------
    void       FormShow(TObject *Sender);               // golden :24-45    GATE (I-1)
    AnsiString GetDIOFileName();                        // golden :47-70    GATE (I-2)  RESERVED NAME  ⚠ WRITES DISK
    void       LoadData(AnsiString S);                  // golden :72-131   GATE (I-3)  RESERVED NAME  ⚠ MODE SWITCH
    void       spbSaveClick(TObject *Sender);           // golden :191-235  GATE (I-4)
    void       spbLoadClick(TObject *Sender);           // golden :237-246  GATE (I-5)
    void       spbDeleteClick(TObject *Sender);         // golden :248-256  GATE (I-6)
};

// AI(W906-FW3-BTQ1) 20260828: TfDIOFrom/fDIOFrom were FREE tree-wide (see the
// banner's measured provenance) -- declared here so the 9 pre-committed
// `#if 0` sites in forms/fTesterIF.cpp (:1477, :1478, :1490, :1505, :1509,
// :1510, :1539, :1540) and Command.cpp (:10243, :15031) have something real to
// bind to once THEIR own gates are separately lifted (NOT done by this wave).
extern TfDIOFrom *fDIOFrom;

#endif // FORMS_FDIOFROM_H
