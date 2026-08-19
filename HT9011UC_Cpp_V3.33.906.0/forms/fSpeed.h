// =============================================================================
//  forms/fSpeed.h  --  non-VCL facade for golden's TfSpeed (cSpeed.h/.cpp)
//
//  AI(W906-FW3-Speed-WA) 20260819: new file, batch-5 cSpeed display-side wave.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cSpeed.h (556 lines) +
//  cSpeed.cpp (2,517 lines, 57 methods; cp950 -- decoded with `python3 -c
//  "open(...,'rb').read().decode('cp950')"`, 0 U+FFFD, this wave) + cSpeed.dfm.
//  cSpeed was 0% translated before this wave: cprod.cpp:2656-2662 carries a
//  `#if 0 // TODO(GA1-B2): blocked by fSpeed@not declared anywhere in ported
//  tree (golden form, no forms/fSpeed.h port exists)` note guarding its own
//  `fSpeed->ReadFile();` call site inside `ProcessLastSetIni_Tray` -- STILL
//  TRUE after this wave (see INTEGRATION-PENDING below) and left untouched
//  (cprod.cpp is an existing file, out of this wave's write boundary; the
//  stale-looking comment is reported to the user, not edited here).
//
//  ROLE
//  ----
//  TfSpeed is golden's "Arm/Shuttle/Index/Magazine speed & timing recipe"
//  dialog -- one PageControl of tab sheets (All/InArm/Shuttle/Index/OutArm/
//  TrayArm/Loader/Magazine), each holding dozens of TEdit/TUpDown/TTrackBar/
//  TCheckBox/TRadioGroup controls bound 1:1 to a numeric field of
//  ArmSpeed_File[]/SHSpeed_File/MGSpeed_File/TestIF_File (all ALREADY ported,
//  cprod.h -- see DEPENDENCY AUDIT in cSpeed.cpp). This wave's mandate is
//  BATCH 5, scope = read-only current-value DISPLAY side only; every
//  editing/persist path is queued (see WAVE SCOPE below and the project's
//  standing write-path-is-safety-critical rule -- WebBridge write path is a
//  separate, not-yet-designed, user-gated effort).
//
//  METHOD CLASSIFICATION (all 57 golden methods; the mandate's 4-bucket taxonomy)
//  --------------------------------------------------------------------------
//  (a) DISPLAY/READ-ONLY -- TRANSLATED this wave (6 methods, bodies in
//      cSpeed.cpp):
//        TfSpeed()          golden :31-39   (ctor -- 3 widget defaults + 1
//                                             GATE, see GATE (S1))
//        FormShow           golden :41-338  (298 lines -- Visible/Enabled/Max
//                                             policy from CosFunction/
//                                             IniConfig/CUSTOMER_CODE/
//                                             fSecurity->Insufficient; calls
//                                             ReadFile()+DoIniDataToForm())
//        ReadFile           golden :340-783 (444 lines -- recipe-scoped
//                                             ArmCondition.Data -> globals via
//                                             ReadIniData, pure read; ends by
//                                             calling ReadWriteFile(true,...))
//        ReadWriteFile      golden :784-1006 (bRead-parameterized; this wave
//                                             keeps ONLY the read arm live --
//                                             see GATE (S5)/(S6))
//        DoIniDataToForm    golden :1008-1215 (207 lines -- pure struct-field
//                                             -> widget Text/Position/
//                                             ItemIndex/Checked fill, zero I/O)
//        FormClose          golden :1272-1280 (GOLDEN ODDITY -- see below:
//                                             this is a DISCARD-EDITS refresh,
//                                             not a save)
//
//  (b) WRITE PATH -- QUEUED, NOT TRANSLATED, NOT EVEN DECLARED this wave (per
//      forms/fTemperFrom.h's own "not even a stub declared" precedent) -- 47
//      methods. All either persist to disk directly (WriteIniData/
//      ReadWriteIni's write arm) or exist ONLY to stage a value that the Save
//      button (spbSaveClick) later persists -- the campaign mandate buckets
//      the latter into "write path" too ("一切 MouseDown-編輯/KeyPress
//      驗證後回寫"), since translating an edit gesture in isolation from the
//      persistence it feeds is incomplete/misleading:
//        spbSaveClick (:1433-1787, 355 lines -- the Save button; ~90
//          WriteIniData calls plus calls DoSetRPDefault/SearchRecipeParameter,
//          see (d) below), SaveSetupFile (:2250-2447, 198 lines -- more
//          WriteIniData, "kevin 20180824 儲存檔案")
//        3x KeyPress numeric-validation (edInArmRetryCountKeyPress,
//          edInVacumCheckTimeKeyPress, edIndexArmRetryMMKeyPress) + 5 more
//          later (Edt_CheckTime_InKeyPress, edRelaseDelayKeyPress,
//          edtMonitoringIndexCycletimeKeyPress, edtMonitoringOutlierKeyPress,
//          edtMonitoringWindowKeyPress, edtIndexCycleTimetoleranceKeyPress,
//          edShakeDelayKeyPress) = 9 total KeyPress handlers
//        ~24 MouseDown handlers that pop the on-screen numeric keypad
//          (`fQwertyKey->ShowQwertyKey(...)`) -- an EDIT gesture, always
//          write-path-adjacent (edInArmRetryCountMouseDown,
//          edInVacumCheckTimeMouseDown, edIndexArmRetryMMMouseDown,
//          edIndexSpeedMouseDown, edIndexAccDecMouseDown,
//          Edt_CheckTime_InMouseDown, edtAutoSkipCTMouseDown,
//          edSecondSpeedInMouseDown, edSecondADCInMouseDown,
//          edSecondSpeedOutMouseDown, edSecondADCOutMouseDown,
//          edRelaseDelayMouseDown, Edt_HeightCheck_InMouseDown,
//          edtMonitoringIndexCycletimeMouseDown, edtMonitoringOutlierMouseDown,
//          edtMonitoringWindowMouseDown, edtIndexCycleTimetoleranceMouseDown,
//          edtLoaderSpeed1MouseDown, edInArmDieCleanDelayMouseDown,
//          edInArmDieCleanHeightMouseDown, edOutArmShtWaitTimeMouseDown,
//          edAutoSpeedLowMouseDown, edTwoSpeedDistanceCatchYMouseDown,
//          edShakeCyclesMouseDown, edShakeDistanceMouseDown,
//          edShakeDelayMouseDown, edShakeAccDecMouseDown)
//        6 batch-edit helpers that mutate widget state ahead of Save
//          (tbAllSpeedChange, tbAccSpeedChange, tbEPControlChange,
//          spbSpeedAddClick, spbSpeedDecClick, cbIndexArmClick,
//          spbSelectAllClick, spbSetToDefClick -- 8, not 6; see full count
//          note below)
//        sbtExitClick (:1788-1793 -- `Close(); fShowMessage->sgdSpeedView->
//          Repaint(); sbtExit->Down=false;` -- pure MFC/.dfm chrome, no
//          logic; bucketed here rather than invented as a 5th category
//          because it is not display and not a GATE-worthy dependency gap,
//          it is simply nothing to translate)
//      Running total: 2 (save) + 9 (KeyPress) + 26 (MouseDown, see count
//      above) + 8 (batch-edit) + 1 (exit) = 46. The 47th is DoSetRPDefault --
//      SEE (d), NOT (b): it is a missing-dependency GATE, not a queued write,
//      even though its body is 100% WriteIniData calls (kept in the (d) list
//      below because the blocking reason is structural, not policy).
//
//  (c) SAFETY-CRITICAL -- NONE found distinct from (b) in this file. Every
//      write-path method here edits a PERCENTAGE/TIME numeric recipe field;
//      the physical consequence (an actual motor speed change) is realized
//      elsewhere, when the saved ArmCondition.Data is loaded by the motion
//      HAL at run start -- not inside cSpeed.cpp itself. No direct
//      interlock/motor-command call was found in any of the 57 methods.
//
//  (d) GOLDEN-DEAD or STRUCTURALLY GATED (missing dependency; NOT declared
//      this wave, matching (b)'s "not even a stub" convention) -- 4 methods:
//        DoSetRPDefault, DoReplyDefaultToForm, SearchRecipeParameter -- see
//          GATE (S4) below (TWinControl-shaped recursive control-tree walk;
//          the type golden needs does not exist in a compatible shape).
//        (no golden-dead METHOD found in cSpeed -- the two golden-commented-
//          out FRAGMENTS below are file-internal, not whole methods)
//      Two golden-commented-out (in golden's own source, `//`-prefixed)
//      fragments, both already dead in golden itself, not translated (same
//      "already dead, matching golden's own state" convention as forms/
//      fTemperFrom.h's TempOverCannotRunShowAlarm precedent):
//        golden :752-758 (ReadFile) -- `bOutPickErrAct`/`iOutPickErrAuto` per-
//          customer branch, superseded by the always-false-branch that
//          follows it (which itself already exists as an ACTIVE ArmSpeed
//          default in golden and is faithfully kept in this wave's ReadFile).
//        golden :1211-1212 (DoIniDataToForm) -- `cbOutPickErrAct->Checked`/
//          `cobOutPickErrAct->ItemIndex` fill, superseded by the same mark.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (S1) ctor, golden :38 `edtHPVacuumDelay->Hint="edtHPVacuumDelay";` --
//      `Hint` has NO port anywhere in vclcompat/Controls.h (grepped this
//      wave: 0 hits for a `Hint` member on any stand-in type). Its ONLY
//      consumer tree-wide is the `->Hint`-keyed Recipe-Parameter-Default scan
//      in DoSetRPDefault/DoReplyDefaultToForm/SearchRecipeParameter (golden
//      :2022-2186), all three of which are themselves GATE (S4) below -- so
//      this line is doubly inert. `#if 0`-gated in the .cpp.
//  (S2) FormShow, golden :49 `PageControl1->ActivePage=tsAllSpeed;` --
//      vclcompat::TPageControl (Controls.h:434) carries only `int
//      ActivePageIndex`, no pointer-valued `ActivePage`. Pure "which tab
//      shows first" UI nicety, zero logic effect. `#if 0`-gated.
//  (S3) FormShow, golden :208 `rbTemp->SetFocus();` -- vclcompat::TControl
//      (Controls.h:213) carries no `SetFocus()`. Pure UI-focus nicety, zero
//      logic effect. `#if 0`-gated.
//  (S4) DoSetRPDefault (golden :1995-2062, RECURSIVE, ~67 lines) /
//      DoReplyDefaultToForm (golden :2064-2115) / SearchRecipeParameter
//      (golden :2117-2187) -- ALL THREE take `TWinControl *PCtrl` and
//      recurse via `PCtrl->Controls[iP]` / `PCtrl->ControlCount` (a real VCL
//      parent-child component-tree walk). Golden's actual call-site arguments
//      are `fSpeed->tsAllSpeed`/`tsIndex`/`tsInArm`/... (TTabSheet*) and,
//      recursively, whatever TGroupBox/TPanel/etc. children the tree finds.
//      vclcompat::TTabSheet/TGroupBox/TPanel (Controls.h:443/251/240) ALL
//      derive directly from `vclcompat::TControl` -- NONE of them derive from
//      any TWinControl-shaped ancestor carrying ControlCount/Controls[]. A
//      DIFFERENT, unrelated `TWinControl`/`TTrackBar` pair DOES already exist
//      (handlerlog.h:104/131, built for TMyLog's OWN, unrelated control-tree
//      walk) -- but that TWinControl is a SIBLING of TTabSheet/TGroupBox/
//      TPanel (all independently derive from TControl), not their ancestor,
//      so `fSpeed->tsAllSpeed` (a TTabSheet*) is NOT convertible to
//      handlerlog.h's `TWinControl*` and cannot be passed to it -- this is
//      the identical shape-mismatch class of trap as this tree's own
//      "two TMyKitSuck headers" gotcha, just with TWinControl in the role of
//      the ambiguous type. Retrofitting TTabSheet/TGroupBox/TPanel to derive
//      from TWinControl would require editing vclcompat/Controls.h (shared,
//      ~110+ TU blast radius, out of this wave's write boundary) or
//      handlerlog.h (same). Per campaign mandate ("跨檔缺口 GATE 不自建
//      shim") no new shim is built here; all three methods are simply NOT
//      DECLARED in this header (matching forms/fTemperFrom.h's "EXPLICITLY
//      EXCLUDED... not even a stub declared here" convention for
//      structurally-unreachable methods). SECONDARY DISCLOSURE (not the
//      blocking reason, but real): DoReplyDefaultToForm/SearchRecipeParameter
//      also depend on `fRPDefault` (golden RPDefault.h/.cpp) -- grepped this
//      wave, NOT ported anywhere in this tree (only tools/dfm2rc/* generator
//      artifacts exist for it, no C++ facade) -- a SECOND, independent reason
//      either would need to be gated even if the TWinControl problem were
//      solved.
//  (S5) ReadWriteFile, golden :797-879 (the `if(LoaderUnload_StepMotor){...}`
//      Tray-Y-axis step-motor block) -- reads/writes `dmTrayMotor->
//      iStepMotorSpeed[0..9]` / `dmTrayMotor->bNeedSetStepMotorSpeed`.
//      `dmTrayMotor` (golden Motor/TrayStepMotor.h/.cpp, a TDataModule) has
//      NO port anywhere in this tree (grepped 20260819: `grep -rn
//      dmTrayMotor` over `HT9011UC_Cpp_V3.33.906.0/**/*.h` -- 0 hits; the
//      port tree carries no TrayStepMotor.h/.cpp at all, confirmed by `find .
//      -iname "TrayStepMotor*"` returning nothing outside golden). `#if 0`-
//      gated verbatim (kept for when/if a future wave ports TrayStepMotor).
//      NOTE this is DIFFERENT from `TestIF_File.iTrayZStepSpeed[12]` (the
//      Tray-Z-axis speeds, golden :881-1005) which IS already ported
//      (cprod.h) and IS translated live this wave.
//  (S6) ReadWriteFile's bRead==false arm -- golden's `ReadWriteIni(...,
//      bRead, true, ...)` calls branch internally (common.cpp:1662-1681) on
//      the SAME `bRead` this function receives: when false, ReadWriteIni
//      calls WriteIniData for real. The ONLY golden call site that ever
//      passes bRead=false is SaveSetupFile (golden :1715, itself bucket (b),
//      queued/not translated this wave), so today NO in-scope caller can
//      reach the write arm -- but as belt-and-suspenders (the mandate's
//      explicit "寫臂 #if 0 GATE" instruction for this exact function,
//      applied to a RUNTIME bool rather than a compile-time branch) this
//      wave's ReadWriteFile forces `bRead=true;` at entry regardless of the
//      caller's argument, so a FUTURE wave cannot silently reactivate the
//      write arm merely by wiring a new caller without revisiting this gate.
//
//  DISCLOSED, NOT GATED -- CheckAndReadIniData / ReadWriteIni(bRead=true)
//  lazy-seed-on-missing-key behaviour
//  --------------------------------------------------------------------------
//  This tree's own documented trap ("Show/Read 前綴會騙人") is a `ReadXxx`-
//  named function that hides a real disk write. ReadFile/ReadWriteFile's read
//  arm calls `CheckAndReadIniData`/`ReadWriteIni(...,bIsRead=true,...)`
//  (common.cpp:592-628/1662-1681): when a key is ABSENT from the ini file,
//  these write the DEFAULT value back (`INIFile->WriteInteger/WriteBool(...)`
//  -- a real, if narrow, write-on-read). Checked whether this is a NEW risk
//  cSpeed introduces: it is not -- `CheckAndReadIniData` is an existing,
//  already-merged, ALREADY WIDELY RELIED-ON common.cpp utility, used the
//  identical way by 18 OTHER already-translated files this wave found via
//  `grep -rl "CheckAndReadIniData(" --include=*.cpp` (cBinSel.cpp, csystem.cpp,
//  cSecurity.cpp, cObserver.cpp, cSocket.cpp, cAuthority.cpp,
//  SECSGEM/uHGemHT9045.cpp, ATC/ATCInterface.cpp, cprod.cpp,
//  OmronLaser/LaserSensor.cpp, CosFunction.cpp, database.cpp,
//  Automation/auto9045.cpp, forms/fLotInfo.cpp, + 4 test files), none of
//  which gate it. UNLIKE `common.cpp:89`'s `asGeneralPath` (the shared
//  production `system\Gerneral.ini`), the path this write lands on here is
//  the RECIPE-scoped `<DataPath><recipe>\ArmCondition.Data` (or
//  `sSaveByMachine`'s per-machine variant) -- not the shared machine-identity
//  file the `--dry` gotcha is about. Not gated; disclosed per the campaign's
//  "遇到就登記" instruction so this is not silently relied upon without a
//  paper trail.
//
//  GOLDEN ODDITY -- FormClose is a DISCARD, not a save (golden :1272-1280)
//  --------------------------------------------------------------------------
//  A method NAMED "Close" that calls `ReadFile(); DoIniDataToForm();` reads
//  backwards from disk and re-fills the form -- i.e. it THROWS AWAY any
//  unsaved on-screen edits, it does not persist them. Golden's own inline
//  comment confirms this is deliberate: `//JerryYang 20250411 : 離開頁面要
//  刷新一次, 避免誤存檔` ("refresh once on leaving the page, to avoid an
//  accidental save"). Translated faithfully (read-only, matches this wave's
//  bucket (a)) -- flagged here so a future reader does not assume "Close"
//  implies "persist".
//
//  DEVIATION -- ad-hoc TForm-property members on TfSpeed (Caption/Left/Top)
//  --------------------------------------------------------------------------
//  Golden `TfSpeed : public TForm` inherits `Caption`/`Left`/`Top` from VCL's
//  TForm; this facade has no TForm base (same posture as every other
//  translated form facade in this tree, e.g. forms/fTemperFrom.h's bare
//  `class TfTemperFrom`). FormShow assigns all three (golden :48/:51-52)
//  purely as one-way writes with no other in-scope method reading them back
//  -- same "NSDMI-default plain member, write-only, no behaviour lost"
//  precedent as forms/fTemperFrom.h's own `AutoSize` member. Added here
//  rather than gated because, unlike (S1)-(S3), a compatible member is
//  trivial to add on THIS wave's own new class (not a shared file).
//
//  DESIGN NOTE -- TfSpeedTrackBar / TfSpeedUpDown (2 new local widget types)
//  --------------------------------------------------------------------------
//  Neither golden `TTrackBar` nor `TUpDown` has a port in vclcompat/
//  Controls.h (grepped this wave: 0 hits for either inside `namespace
//  vclcompat`). A DIFFERENT, unrelated bare/global `class TTrackBar : public
//  TControl` already exists (handlerlog.h:131, `Position` only, built for
//  TMyLog's OWN unrelated Control_Log_Value consumer) -- reusing that exact
//  name here risks the same "two headers, same bare name, different shape"
//  collision this tree's two-TMyKitSuck gotcha warns about (this one is also
//  missing `->Max`, which FormShow's `CosFunction.bLimitMaxSpeed` block
//  needs). Distinctly-named, minimal, local stand-ins instead -- same
//  "compose, don't collide" convention as forms/fTemperFrom.h's
//  TfTemperFromPanel/TfTemperFromTimer. Both derive from `vclcompat::
//  TControl` (picking up Visible/Enabled for free, matching golden TTrackBar/
//  TUpDown both being real TWinControl-descended VCL controls with those
//  properties) and add `int Position` / `int Max` (the only two members any
//  in-scope method touches: `->Position` in ReadFile/ReadWriteFile/
//  DoIniDataToForm/ctor-adjacent FormShow reads, `->Max` in FormShow's speed-
//  limit block). `->SelEnd` (TTrackBar) is NOT carried -- its only golden use
//  is inside tbAllSpeedChange/tbAccSpeedChange/tbEPControlChange, all bucket
//  (b), not declared this wave.
//
//  NOTE ON THE FULL WIDGET MEMBER LIST BELOW
//  --------------------------------------------------------------------------
//  FormShow alone touches the large majority of golden's __published widget
//  set (Visible/Enabled/Max policy across nearly every tab), and
//  DoIniDataToForm touches most of the rest -- so, per forms/fTemperFrom.h's
//  own precedent ("all 152 must exist... so this header declares the full
//  golden-order set up front"), this header declares golden's COMPLETE
//  __published widget list (330 pointers, golden order preserved exactly),
//  each heap-allocated via NSDMI, rather than a hand-picked subset that a
//  future wave would have to keep re-extending. `LastFileName`/`OrgStr`
//  (golden :540-541, private AnsiString fields) are also kept for structural
//  fidelity; `OrgStr` is a GOLDEN ODDITY -- grepped this wave, it is used
//  NOWHERE in golden cSpeed.cpp (0 hits), i.e. already dead in golden itself,
//  kept only so this header's private section matches golden's shape 1:1.
//
//  INTEGRATION-PENDING -- NO `extern TfSpeed *fSpeed;` here
//  --------------------------------------------------------------------------
//  Same posture as forms/fTemperFrom.h: the one known consumer
//  (cprod.cpp:2656-2662's `fSpeed->ReadFile();`) is already `#if 0`-gated and
//  sits outside this wave's write boundary, so declaring the global here
//  would not itself unblock it -- picking the live singleton wiring (and
//  flipping cprod.cpp's own gate) is a later integration wave's call, not
//  this one's. Reported to the user as-is; cprod.cpp not edited.
// =============================================================================
#ifndef FORMS_FSPEED_H
#define FORMS_FSPEED_H

#include "vclcompat/Controls.h"   // TEdit/TPanel/TGroupBox/TCheckBox/TRadioGroup/
                                  //   TRadioButton/TComboBox/TSpeedButton/
                                  //   TPageControl/TTabSheet/TLabel/TLabeledEdit
                                  //   (all brought to global scope by that
                                  //   header's own footer -- see its "Brought
                                  //   into the global namespace" block)

// ---------------------------------------------------------------------------
//  TfSpeedTrackBar / TfSpeedUpDown -- see DESIGN NOTE above.
// ---------------------------------------------------------------------------
class TfSpeedTrackBar : public vclcompat::TControl
{
public:
    int Position = 0;
    int Max      = 100;
};

class TfSpeedUpDown : public vclcompat::TControl
{
public:
    int Position = 0;
    int Max      = 100;
};

// ===========================================================================
//  TfSpeed -- non-VCL facade (golden cSpeed.h, TfSpeed : public TForm)
// ===========================================================================
class TfSpeed
{
private:
    // golden :540-541 (private User declarations)
    AnsiString LastFileName;
    AnsiString OrgStr;   // GOLDEN ODDITY: unused anywhere in golden cSpeed.cpp -- see banner

public:
    TfSpeed();

    // -- ad-hoc TForm-property members -- see DEVIATION above ---------------
    AnsiString Caption;
    int Left = 0;
    int Top  = 0;

    // -- golden :547 (public bool fShow) -------------------------------------
    bool fShow = false;

    // -----------------------------------------------------------------------
    //  Widget members -- golden __published order preserved exactly
    //  (golden cSpeed.h :17-458). See "NOTE ON THE FULL WIDGET MEMBER LIST"
    //  above for why the complete set is declared rather than a subset.
    // -----------------------------------------------------------------------
    TPageControl *PageControl1 = new TPageControl();
    TTabSheet    *tsAllSpeed   = new TTabSheet();
    TTabSheet    *tsInArm      = new TTabSheet();
    TTabSheet    *tsShuttle    = new TTabSheet();
    TTabSheet    *tsIndex      = new TTabSheet();
    TTabSheet    *tsOutArm     = new TTabSheet();
    TTabSheet    *tsTrayArm    = new TTabSheet();
    TGroupBox    *gpSpeed      = new TGroupBox();
    TfSpeedTrackBar *tbAllSpeed = new TfSpeedTrackBar();
    TEdit        *edAllSpeed   = new TEdit();
    TGroupBox    *gbAcc        = new TGroupBox();
    TEdit        *edAllAccSpeed = new TEdit();
    TfSpeedTrackBar *tbAccSpeed = new TfSpeedTrackBar();
    TGroupBox    *gbUnitForChange = new TGroupBox();
    TCheckBox    *cbIndexArm   = new TCheckBox();
    TCheckBox    *cbInArm      = new TCheckBox();
    TCheckBox    *cbOutArm     = new TCheckBox();
    TCheckBox    *cbShuttle    = new TCheckBox();
    TCheckBox    *cbTrayArm    = new TCheckBox();
    TCheckBox    *cbInArmZ     = new TCheckBox();
    TCheckBox    *cbOutArmZ    = new TCheckBox();
    TRadioGroup  *rgAutoSpeed  = new TRadioGroup();
    TSpeedButton *spbSpeedAdd  = new TSpeedButton();
    TSpeedButton *spbSpeedDec  = new TSpeedButton();
    TSpeedButton *spbSelectAll = new TSpeedButton();
    TPanel       *Panel1       = new TPanel();
    TSpeedButton *spbSave      = new TSpeedButton();
    TSpeedButton *sbtExit      = new TSpeedButton();
    TCheckBox    *cbInRotate   = new TCheckBox();
    TCheckBox    *cbOutRotate  = new TCheckBox();
    TGroupBox    *gbEPControl  = new TGroupBox();
    TfSpeedTrackBar *tbEPControl = new TfSpeedTrackBar();
    TEdit        *edEPControl  = new TEdit();
    TSpeedButton *spbSetToDef  = new TSpeedButton();
    TPanel       *palInArmLeft = new TPanel();
    TGroupBox    *gbInArmSpeed = new TGroupBox();
    TLabel *Label1 = new TLabel(), *Label4 = new TLabel(), *Label5 = new TLabel();
    TLabel *Label2 = new TLabel(), *Label3 = new TLabel();
    TEdit        *edInXYSpd    = new TEdit();
    TEdit        *edInXYAcc    = new TEdit();
    TfSpeedUpDown *udInXSpd    = new TfSpeedUpDown();
    TfSpeedUpDown *udInXAcc    = new TfSpeedUpDown();
    TEdit        *edInZSpd     = new TEdit();
    TEdit        *edInZAcc     = new TEdit();
    TfSpeedUpDown *udInZSpd    = new TfSpeedUpDown();
    TfSpeedUpDown *udInZAcc    = new TfSpeedUpDown();
    TEdit        *edInPitchSpd = new TEdit();
    TEdit        *edInPitchAcc = new TEdit();
    TfSpeedUpDown *udInPitchSpd = new TfSpeedUpDown();
    TfSpeedUpDown *udInPitchAcc = new TfSpeedUpDown();
    TPanel       *palInRotate  = new TPanel();
    TLabel       *Label59      = new TLabel();
    TEdit        *edInRotSpd   = new TEdit();
    TfSpeedUpDown *udInRotSpd  = new TfSpeedUpDown();
    TEdit        *edInRotAcc   = new TEdit();
    TfSpeedUpDown *udInRotAcc  = new TfSpeedUpDown();
    TGroupBox    *gbInVacuumOnFunc = new TGroupBox();
    TCheckBox    *chkInArmPreSuck = new TCheckBox();
    TRadioGroup  *rgInArmPitch = new TRadioGroup();
    TGroupBox    *gbAutoSkip   = new TGroupBox();
    TLabel       *labAutoSkip  = new TLabel();
    TRadioGroup  *rgInArmAutoSpeed = new TRadioGroup();
    TEdit        *edtAutoSkipCT = new TEdit();
    TGroupBox    *gbInArmReleaseDelay = new TGroupBox();
    TLabel *Label57 = new TLabel(), *Label58 = new TLabel();
    TRadioGroup  *rgReleaseDelayTime = new TRadioGroup();
    TEdit        *edRelaseDelay = new TEdit();
    TPanel       *palInArmRight = new TPanel();
    TGroupBox    *gbInArmRetry = new TGroupBox();
    TLabel *Label6 = new TLabel(), *Label7 = new TLabel(), *Label8 = new TLabel();
    TEdit        *edInArmRetryCount = new TEdit();
    TEdit        *edInArmRetryMM = new TEdit();
    TGroupBox    *gbInArmWaitTime = new TGroupBox();
    TGroupBox    *gbInArmVacuumCheck = new TGroupBox();
    TLabel       *Label9       = new TLabel();
    TCheckBox    *cbInVacuumCheckOptimize = new TCheckBox();
    TEdit        *edInVacumCheckTime = new TEdit();
    TGroupBox    *gbInArmDestoryTime = new TGroupBox();
    TLabel *Label49 = new TLabel(), *Label50 = new TLabel(), *Label51 = new TLabel(), *Label52 = new TLabel();
    TEdit        *edInArmDestroyAgainTime = new TEdit();
    TEdit        *edInArmDestroyAgainCount = new TEdit();
    TGroupBox    *gbInDestoryCheck = new TGroupBox();
    TLabel *Label65 = new TLabel(), *Label66 = new TLabel();
    TCheckBox    *ckb_PauseCheck_In = new TCheckBox();
    TEdit        *Edt_CheckTime_In = new TEdit();
    TGroupBox    *gbTwoSpeed_In = new TGroupBox();
    TLabel       *labTwoSpeedDistance_In = new TLabel();
    TLabel       *labSecondSpeedIn = new TLabel();
    TLabel       *labSecondADCIn = new TLabel();
    TRadioGroup  *rgTwoSpeedOnOff_In = new TRadioGroup();
    TEdit        *edTwoSpeedDistance_In = new TEdit();
    TEdit        *edSecondSpeedIn = new TEdit();
    TEdit        *edSecondADCIn = new TEdit();
    TPanel       *palOutLeft   = new TPanel();
    TGroupBox    *gbOutArmSpeed = new TGroupBox();
    TLabel *Label24 = new TLabel(), *Label25 = new TLabel(), *Label26 = new TLabel();
    TLabel *Label27 = new TLabel(), *Label28 = new TLabel();
    TEdit        *edOutXSpd    = new TEdit();
    TEdit        *edOutXAcc    = new TEdit();
    TfSpeedUpDown *udOutXSpd   = new TfSpeedUpDown();
    TfSpeedUpDown *udOutXAcc   = new TfSpeedUpDown();
    TEdit        *edOutZSpd    = new TEdit();
    TEdit        *edOutZAcc    = new TEdit();
    TfSpeedUpDown *udOutZSpd   = new TfSpeedUpDown();
    TfSpeedUpDown *udOutZAcc   = new TfSpeedUpDown();
    TEdit        *edOutPitchSpd = new TEdit();
    TEdit        *edOutPitchAcc = new TEdit();
    TfSpeedUpDown *udOutPitchSpd = new TfSpeedUpDown();
    TfSpeedUpDown *udOutPitchAcc = new TfSpeedUpDown();
    TPanel       *palOutRotate = new TPanel();
    TLabel       *Label63      = new TLabel();
    TEdit        *edOutRotSpd  = new TEdit();
    TfSpeedUpDown *udOutRotSpd = new TfSpeedUpDown();
    TEdit        *edOutRotAcc  = new TEdit();
    TfSpeedUpDown *udOutRotAcc = new TfSpeedUpDown();
    TGroupBox    *gbOutVacuumOnFunc = new TGroupBox();
    TLabel       *Label44      = new TLabel();
    TCheckBox    *chkOutArmPreSuck = new TCheckBox();
    TEdit        *edtOutArmPreSuck = new TEdit();
    TRadioGroup  *rgOutArmPitch = new TRadioGroup();
    TGroupBox    *gbOutPickErrAct = new TGroupBox();
    TLabel       *Label60      = new TLabel();
    TCheckBox    *cbOutPickErrAct = new TCheckBox();
    TComboBox    *cobOutPickErrAct = new TComboBox();
    TPanel       *palOutRight  = new TPanel();
    TGroupBox    *gbOutArmRetry = new TGroupBox();
    TLabel *Label29 = new TLabel(), *Label30 = new TLabel(), *Label31 = new TLabel();
    TEdit        *edOutArmRetryCount = new TEdit();
    TEdit        *edOutArmRetryMM = new TEdit();
    TGroupBox    *gbOutArmWaitTime = new TGroupBox();
    TGroupBox    *gbOutArmVacuumCheck = new TGroupBox();
    TLabel       *Label32      = new TLabel();
    TCheckBox    *cbOutVacuumCheckOptimize = new TCheckBox();
    TEdit        *edOutVacumCheckTime = new TEdit();
    TGroupBox    *gbOutArmAirOnTime = new TGroupBox();
    TLabel       *Label33      = new TLabel();
    TEdit        *edOutArmAirOnTime = new TEdit();
    TGroupBox    *gbOutArmDestoryTime = new TGroupBox();
    TLabel *Label53 = new TLabel(), *Label54 = new TLabel(), *Label55 = new TLabel(), *Label56 = new TLabel();
    TEdit        *edOutArmDestroyAgainTime = new TEdit();
    TEdit        *edOutArmDestroyAgainCount = new TEdit();
    TGroupBox    *gbOutDestoryCheck = new TGroupBox();
    TLabel *Label67 = new TLabel(), *Label68 = new TLabel();
    TCheckBox    *ckb_PauseCheck_Out = new TCheckBox();
    TEdit        *Edt_CheckTime_Out = new TEdit();
    TGroupBox    *gbTwoSpeed_Out = new TGroupBox();
    TLabel       *labTwoSpeedDistance_Out = new TLabel();
    TLabel       *labSecondSpeedOut = new TLabel();
    TLabel       *labSecondADCOut = new TLabel();
    TRadioGroup  *rgTwoSpeedOnOff_Out = new TRadioGroup();
    TEdit        *edTwoSpeedDistance_Out = new TEdit();
    TEdit        *edSecondSpeedOut = new TEdit();
    TEdit        *edSecondADCOut = new TEdit();
    TPanel       *palIndexLeft = new TPanel();
    TGroupBox    *gbIndexArmSpeed = new TGroupBox();
    TLabel *Label17 = new TLabel(), *Label18 = new TLabel(), *Label20 = new TLabel(), *Label19 = new TLabel();
    TEdit        *edIndexSpeed = new TEdit();
    TfSpeedUpDown *udIndexSpd  = new TfSpeedUpDown();
    TEdit        *edIndexAccDec = new TEdit();
    TfSpeedUpDown *udIndexAcc  = new TfSpeedUpDown();
    TGroupBox    *grpIndexPreSuck = new TGroupBox();
    TCheckBox    *chkIndexPreSuck = new TCheckBox();
    TPanel       *palIndexRight = new TPanel();
    TGroupBox    *gbIndexWaitTime = new TGroupBox();
    TGroupBox    *gbIndexVacuumCheck = new TGroupBox();
    TLabel       *Label21      = new TLabel();
    TCheckBox    *cbIndexVacuumCheckOptimize = new TCheckBox();
    TEdit        *edIndexVacumCheckTime = new TEdit();
    TGroupBox    *gbIndexAirOnTime = new TGroupBox();
    TLabel       *Label22      = new TLabel();
    TEdit        *edIndexAirOnTime = new TEdit();
    TGroupBox    *gbIndexRetry = new TGroupBox();
    TLabel *Label23 = new TLabel(), *Label42 = new TLabel(), *Label43 = new TLabel();
    TEdit        *edIndexRetryCount = new TEdit();
    TEdit        *edIndexArmRetryMM = new TEdit();
    TRadioGroup  *rgSocketCheck = new TRadioGroup();
    TGroupBox    *gbIndexDestoryTime = new TGroupBox();
    TLabel *Label45 = new TLabel(), *Label47 = new TLabel(), *Label46 = new TLabel(), *Label48 = new TLabel();
    TEdit        *edIndexDestroyAgainTime = new TEdit();
    TEdit        *edIndexDestroyAgainCount = new TEdit();
    TPanel       *palTrayArmLeft = new TPanel();
    TGroupBox    *gbTrayArmSpeed = new TGroupBox();
    TLabel *Label34 = new TLabel(), *Label35 = new TLabel(), *Label36 = new TLabel(), *Label37 = new TLabel();
    TEdit        *edTrayXSpd   = new TEdit();
    TfSpeedUpDown *udTrayXSpd  = new TfSpeedUpDown();
    TEdit        *edTrayXAcc   = new TEdit();
    TfSpeedUpDown *udTrayXAcc  = new TfSpeedUpDown();
    TPanel       *palTrayArmRight = new TPanel();
    TGroupBox    *gbTrayArmWaitTime = new TGroupBox();
    TGroupBox    *gbTrayArmVacuumCheck = new TGroupBox();
    TLabel       *Label39      = new TLabel();
    TEdit        *edTrayVacumCheckTime = new TEdit();
    TGroupBox    *gbTrayArmAirOnTime = new TGroupBox();
    TLabel       *Label40      = new TLabel();
    TEdit        *edTrayArmAirOnTime = new TEdit();
    TGroupBox    *gbTrayArmHandDown = new TGroupBox();
    TLabel       *Label41      = new TLabel();
    TEdit        *edTrayArmHandDown = new TEdit();
    TGroupBox    *gbTrayArmRetryCount = new TGroupBox();
    TLabel       *Label38      = new TLabel();
    TEdit        *edTrayArmRetryCount = new TEdit();
    TPanel       *palShuttleLeft = new TPanel();
    TGroupBox    *gbShuttleSpeed = new TGroupBox();
    TLabel *Label12 = new TLabel(), *Label13 = new TLabel(), *Label14 = new TLabel(), *Label15 = new TLabel();
    TEdit        *edSht1Spd    = new TEdit();
    TEdit        *edSht1Acc    = new TEdit();
    TfSpeedUpDown *udSht1Spd   = new TfSpeedUpDown();
    TfSpeedUpDown *udSht1Acc   = new TfSpeedUpDown();
    TEdit        *edSht2Spd    = new TEdit();
    TEdit        *edSht2Acc    = new TEdit();
    TfSpeedUpDown *udSht2Spd   = new TfSpeedUpDown();
    TfSpeedUpDown *udSht2Acc   = new TfSpeedUpDown();
    TRadioGroup  *rgStepShuttle = new TRadioGroup();
    TPanel       *palShuttleRight = new TPanel();
    TGroupBox    *gbShtDeviceCheckTime = new TGroupBox();
    TLabel       *Label16      = new TLabel();
    TEdit        *edShtDeviceCheckTime = new TEdit();
    TCheckBox    *ckb_HeightCheck_In = new TCheckBox();
    TEdit        *Edt_HeightCheck_In = new TEdit();
    TRadioGroup  *gbOutarmDeviveConfirm = new TRadioGroup();
    TRadioGroup  *gbInarmDeviveConfirm = new TRadioGroup();
    TGroupBox    *grpIndexUPHTryRun = new TGroupBox();
    TLabel *Label61 = new TLabel(), *Label62 = new TLabel();
    TEdit        *edtTryAcc    = new TEdit();
    TEdit        *edtTrySpeed  = new TEdit();
    TGroupBox    *grpIndexCycleTimeMonitoring = new TGroupBox();
    TLabel *Label69 = new TLabel(), *Label70 = new TLabel(), *Label71 = new TLabel();
    TLabel *Label72 = new TLabel(), *Label73 = new TLabel(), *Label74 = new TLabel();
    TCheckBox    *chkEnableIndexCycleTimeMonitoring = new TCheckBox();
    TEdit        *edtMonitoringIndexCycletime = new TEdit();
    TEdit        *edtMonitoringOutlier = new TEdit();
    TEdit        *edtMonitoringWindow = new TEdit();
    TPanel       *Panel2       = new TPanel();
    TGroupBox    *gbInArmAirOnTime = new TGroupBox();
    TLabel       *Label10      = new TLabel();
    TEdit        *edInArmAirOnTime = new TEdit();
    TGroupBox    *gbInArmWaitOnSht = new TGroupBox();
    TLabel       *Label11      = new TLabel();
    TEdit        *edInArmShtWaitTime = new TEdit();
    TGroupBox    *grpHPVacuumDelay = new TGroupBox();
    TLabel       *Label64      = new TLabel();
    TEdit        *edtHPVacuumDelay = new TEdit();
    TCheckBox    *chkUseHPVacuumDelay = new TCheckBox();
    TRadioGroup  *rgSearchLastModeForYPitch = new TRadioGroup();
    TCheckBox    *chkTwoSpeedOnlyLoader = new TCheckBox();
    TRadioGroup  *gbIndexeviveConfirm = new TRadioGroup();
    TTabSheet    *tsLoader     = new TTabSheet();
    TPanel       *Panel3       = new TPanel();
    TGroupBox    *grpLoader    = new TGroupBox();
    TGroupBox    *grpAuto2     = new TGroupBox();
    TGroupBox    *grpAuto3     = new TGroupBox();
    TGroupBox    *grpAuto1     = new TGroupBox();
    TGroupBox    *grpColor     = new TGroupBox();
    TGroupBox    *grpEmpty     = new TGroupBox();
    TRadioGroup  *rgShakeShuttle = new TRadioGroup();
    TPanel       *pnlpnlLoaderZ = new TPanel();
    TPanel       *pnlLoaderY   = new TPanel();
    TLabeledEdit *edtLoaderSpeed1 = new TLabeledEdit();
    TLabel       *Lab_LoaderVer = new TLabel();
    TfSpeedUpDown *udLoaderSpeed1 = new TfSpeedUpDown();
    TLabeledEdit *edtLoaderSpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udLoaderSpeedZ = new TfSpeedUpDown();
    TPanel       *pnEmptyY     = new TPanel();
    TPanel       *pnlColorY    = new TPanel();
    TPanel       *pnlAuto1Y    = new TPanel();
    TPanel       *pnlAuto2Y    = new TPanel();
    TPanel       *pnlAuto3Y    = new TPanel();
    TLabeledEdit *edtEmptySpeed1 = new TLabeledEdit();
    TfSpeedUpDown *udEmptySpeed1 = new TfSpeedUpDown();
    TLabel       *Lab_EmptyVer = new TLabel();
    TLabeledEdit *edtColorSpeed1 = new TLabeledEdit();
    TfSpeedUpDown *udColorSpeed1 = new TfSpeedUpDown();
    TLabel       *Lab_ColorVer = new TLabel();
    TLabeledEdit *edtAuto1Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto1Speed1 = new TfSpeedUpDown();
    TLabel       *Lab_Auto1Ver = new TLabel();
    TLabeledEdit *edtAuto2Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto2Speed1 = new TfSpeedUpDown();
    TLabel       *Lab_Auto2Ver = new TLabel();
    TPanel       *pnEmptyZ     = new TPanel();
    TLabeledEdit *edtAuto3Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto3Speed1 = new TfSpeedUpDown();
    TLabel       *Lab_Auto3Ver = new TLabel();
    TLabeledEdit *edtEmptySpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udEmptySpeedZ = new TfSpeedUpDown();
    TPanel       *pnlColorZ    = new TPanel();
    TLabeledEdit *edtColorSpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udColorSpeedZ = new TfSpeedUpDown();
    TPanel       *pnlAuto1Z    = new TPanel();
    TLabeledEdit *edtAuto1SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto1SpeedZ = new TfSpeedUpDown();
    TPanel       *pnlAuto2Z    = new TPanel();
    TLabeledEdit *edtAuto2SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto2SpeedZ = new TfSpeedUpDown();
    TPanel       *pnlAuto3Z    = new TPanel();
    TLabeledEdit *edtAuto3SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto3SpeedZ = new TfSpeedUpDown();
    TGroupBox    *gbInArmDieClean = new TGroupBox();
    TLabel *Label75 = new TLabel(), *Label76 = new TLabel(), *Label77 = new TLabel(), *Label78 = new TLabel();
    TRadioGroup  *rgInArmDieClean = new TRadioGroup();
    TEdit        *edInArmDieCleanDelay = new TEdit();
    TEdit        *edInArmDieCleanHeight = new TEdit();
    TGroupBox    *gbOutArmDieClean = new TGroupBox();
    TLabel *Label79 = new TLabel(), *Label80 = new TLabel();
    TRadioGroup  *rgOutArmDieClean = new TRadioGroup();
    TEdit        *edOutArmDieCleanDelay = new TEdit();
    TGroupBox    *GroupBox1    = new TGroupBox();
    TLabel       *Label81      = new TLabel();
    TEdit        *edOutArmShtWaitTime = new TEdit();
    TRadioGroup  *rgSearchInarmLastModeForYPitch = new TRadioGroup();
    TRadioGroup  *rgOutArmPickupErrMode = new TRadioGroup();
    TLabel       *labIndexCycleTimeTolerance = new TLabel();
    TEdit        *edtIndexCycleTimetolerance = new TEdit();
    TLabel       *Label84      = new TLabel();
    TCheckBox    *chkPickAfterOutArm = new TCheckBox();
    TTabSheet    *tsMagazine   = new TTabSheet();
    TGroupBox    *GroupBox2    = new TGroupBox();
    TLabel *Label85 = new TLabel(), *Label86 = new TLabel(), *Label87 = new TLabel(), *Label88 = new TLabel();
    TEdit        *edMagCatchYSpd = new TEdit();
    TEdit        *edMagCatchYAcc = new TEdit();
    TfSpeedUpDown *udMagCatchYSpd = new TfSpeedUpDown();
    TfSpeedUpDown *udMagCatchYAcc = new TfSpeedUpDown();
    TEdit        *edMagZSpd    = new TEdit();
    TEdit        *edMagZAcc    = new TEdit();
    TfSpeedUpDown *udMagZSpd   = new TfSpeedUpDown();
    TfSpeedUpDown *udMagZAcc   = new TfSpeedUpDown();
    TGroupBox    *GroupBox3    = new TGroupBox();
    TLabel *Label89 = new TLabel(), *Label90 = new TLabel(), *Label91 = new TLabel();
    TEdit        *edTwoSpeedDistanceCatchY = new TEdit();
    TEdit        *edSecondSpeedCatchY = new TEdit();
    TEdit        *edSecondADCCatchY = new TEdit();
    TRadioButton *rbTemp       = new TRadioButton();
    TGroupBox    *gbInArmCylinderDelay = new TGroupBox();
    TLabel       *Label92      = new TLabel();
    TEdit        *edInArmCylinderDelay = new TEdit();
    TLabel       *Label93      = new TLabel();
    TGroupBox    *gbOutArmCylinderDelay = new TGroupBox();
    TLabel *Label94 = new TLabel(), *Label95 = new TLabel();
    TEdit        *edOutArmCylinderDelay = new TEdit();
    TPanel       *Panel4       = new TPanel();
    TLabel *Label82 = new TLabel(), *Label83 = new TLabel();
    TEdit        *edAutoSpeedLow = new TEdit();
    TPanel       *Panel5       = new TPanel();
    TLabel       *lblTrayStepSpeedByMachine = new TLabel();
    TLabeledEdit *edtLoaderSpeed2 = new TLabeledEdit();
    TGroupBox    *grpAuto6     = new TGroupBox();
    TPanel       *pnlAuto6Y    = new TPanel();
    TLabel       *Lab_Auto6Ver = new TLabel();
    TLabeledEdit *edtAuto6Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto6Speed1 = new TfSpeedUpDown();
    TPanel       *pnlAuto6Z    = new TPanel();
    TLabeledEdit *edtAuto6SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto6SpeedZ = new TfSpeedUpDown();
    TGroupBox    *grpAuto5     = new TGroupBox();
    TPanel       *pnlAuto5Y    = new TPanel();
    TLabel       *Lab_Auto5Ver = new TLabel();
    TLabeledEdit *edtAuto5Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto5Speed1 = new TfSpeedUpDown();
    TPanel       *pnlAuto5Z    = new TPanel();
    TLabeledEdit *edtAuto5SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto5SpeedZ = new TfSpeedUpDown();
    TGroupBox    *grpAuto4     = new TGroupBox();
    TPanel       *pnlAuto4Y    = new TPanel();
    TLabel       *Lab_Auto4Ver = new TLabel();
    TLabeledEdit *edtAuto4Speed1 = new TLabeledEdit();
    TfSpeedUpDown *udAuto4Speed1 = new TfSpeedUpDown();
    TPanel       *pnlAuto4Z    = new TPanel();
    TLabeledEdit *edtAuto4SpeedZ = new TLabeledEdit();
    TfSpeedUpDown *udAuto4SpeedZ = new TfSpeedUpDown();
    TPanel       *Panel12      = new TPanel();
    TRadioGroup  *rgIndexCycleTime = new TRadioGroup();
    TRadioGroup  *rgTT_SetSpeed = new TRadioGroup();
    TRadioGroup  *rgInShtOrder = new TRadioGroup();
    TGroupBox    *gbEnableDelayTimeZero = new TGroupBox();
    TCheckBox    *cbEnableDelayTimeZero = new TCheckBox();
    TGroupBox    *grpShake     = new TGroupBox();
    TLabel       *lblShakeAccDec = new TLabel();
    TLabel       *lblShakeDistance = new TLabel();
    TLabel       *lblShakeDelay = new TLabel();
    TLabel       *lblShakeCycles = new TLabel();
    TEdit        *edShakeAccDec = new TEdit();
    TEdit        *edShakeDistance = new TEdit();
    TEdit        *edShakeDelay = new TEdit();
    TEdit        *edShakeCycles = new TEdit();
    TGroupBox    *GroupBox4    = new TGroupBox();
    TLabel *Label96 = new TLabel(), *Label97 = new TLabel();
    TPanel       *Panel6       = new TPanel();
    TLabel       *Label98      = new TLabel();
    TEdit        *edPrecisorOpenSp = new TEdit();
    TfSpeedUpDown *udPrecisorOpenSp = new TfSpeedUpDown();
    TEdit        *edPrecisorCloseSp = new TEdit();
    TfSpeedUpDown *udPrecisorCloseSp = new TfSpeedUpDown();

    // -- Wave A translated methods (bodies: cSpeed.cpp, this wave) ----------
    void ReadFile();                                    // golden :340-783
    void ReadWriteFile(bool bRead, AnsiString szDir);   // golden :784-1006 (read arm only, see GATE (S5)/(S6))
    void DoIniDataToForm();                              // golden :1008-1215
    void FormShow();                                     // golden :41-338 (Sender dropped, see below)
    void FormClose();                                    // golden :1272-1280 (Sender/TCloseAction& dropped, see below)
};

// DEVIATION: FormShow/FormClose drop their golden TObject*/TCloseAction&
// parameters -- neither body reads them, matching every other translated
// form facade's identical FormShow/FormClose simplification in this tree
// (forms/fTemperFrom.h's own FormClose DEVIATION note cites the same
// rationale and four sibling precedents).

// AI(W906-FW3-Speed-WA) 20260819: integration-pending -- NO `extern TfSpeed
// *fSpeed;` here. See banner above: cprod.cpp:2656-2662's own `#if 0` call
// site sits outside this wave's write boundary, so declaring the global here
// would not itself unblock it -- picking the live singleton wiring is a later
// integration wave's call.

#endif // FORMS_FSPEED_H
