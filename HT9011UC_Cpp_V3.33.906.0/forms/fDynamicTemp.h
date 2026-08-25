// =============================================================================
//  forms/fDynamicTemp.h  --  non-VCL facade for golden's TfDynamicTemp
//  (DynamicTemp.h/.cpp)
//
//  AI(W906-FW3-DynTemp-WA) 20260820: new file, FW-3 batch-3 DynamicTemp Wave A.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/DynamicTemp.h (162 lines) +
//  DynamicTemp.cpp (346 lines, cp950 -- decoded with `python3 -c
//  "open(...,'rb').read().decode('cp950')"`, 0 U+FFFD, this wave).
//
//  ROLE
//  ----
//  TfDynamicTemp is golden's ad-hoc temperature-IC calibration/diagnostic
//  dialog: a TeeChart (Chart1, 18 TLineSeries) plotting up to 16 realtime
//  channel readings plus an Upper/Lower guard band, a small draggable-panel
//  layout ("gbA1".."gbH16"/"gbUpper"/"gbLower", one per channel readout box),
//  an RS232 handshake to a "Temp IC" calibration board (COM2->TempComm6), and
//  a load/save-to-file record/playback pair for that handshake's captured
//  log. This wave translates ALL 25 event handlers + the ctor (every golden
//  method in the file) -- there is no excluded-method list this wave, unlike
//  most other FW-3 form waves, because the whole golden file is small enough
//  to fit in one pass. Widget stand-ins store data (Caption/Color/Text/
//  Checked/geometry), they do not render -- same "logic faithful, rendering
//  = web" split as every other translated form facade in this tree (forms/
//  fObserver.h, forms/fTemperFrom.h, forms/fYieldMonitoring.h).
//
//  THIS IS A NEW CLASS -- NO EXISTING SHIM, NO LIVE GLOBAL DECLARED ELSEWHERE
//  --------------------------------------------------------------------------
//  Grepped `TfDynamicTemp|fDynamicTemp` tree-wide this wave (20260820): 0
//  compiled hits anywhere (the only matches are tools/dfm2rc's own DFM->JSON/
//  layout-generator ARTIFACTS for this form -- data files from the DFM2WEB
//  campaign's read-only recon, not C++ declarations -- and docs/
//  RECON_GateA_FormRegistry.md). No ODR conflict, no prior stand-in to reuse.
//
//  HARD DEPENDENCY FOR A SIBLING WAVE (per task brief)
//  --------------------------------------------------------------------------
//  uTemp_Set.cpp (not yet landed as of this wave -- grepped, 0 hits for the
//  file existing in this tree yet) is expected to `#include
//  "forms/fDynamicTemp.h"` and write `edUpper`/`edLower`/`edMax`/`edMin`
//  through `extern TfDynamicTemp *fDynamicTemp;`. All four are declared below
//  as plain `TEdit*`/`TfDynamicTempEdit*` members (see widget table) so that
//  wave's `->Text` writes bind without any further header change here.
//
//  WAVE SCOPE (every golden method this header declares, golden line span)
//  --------------------------------------------------------------------------
//    TfDynamicTemp()          golden :20-40   (ctor -- TC[]/SetEdit[]/myCB[] maps)
//    btExitClick              golden :42-45
//    lblRealTime6Click        golden :47-53   GATE (C1) narrow
//    lblRealTime1DblClick     golden :55-58
//    btTempICLoadClick        golden :60-99   GATE (D1) narrow
//    btTempICSaveClick        golden :101-109 GATE (D2) narrow
//    sbTempICClearClick       golden :111-121 (no gate -- see GATE REGISTER)
//    lblRealTime2Click        golden :123-128 GATE (C1) narrow
//    btShowClick              golden :130-142
//    edMinMouseDown           golden :144-150 GATE (Q1) narrow
//    edMaxMouseDown           golden :152-158 GATE (Q1) narrow
//    FormResize               golden :160-173
//    edLowerMouseDown         golden :175-187 GATE (Q1) narrow
//    edUpperMouseDown         golden :189-201 GATE (Q1) narrow
//    btSelectAllClick         golden :203-209
//    gbA1MouseUp              golden :211-220
//    gbA1MouseDown            golden :222-231
//    gbA1MouseMove            golden :233-243
//    btSelectAllMouseDown     golden :245-251
//    btSelectAllMouseUp       golden :253-259
//    FormShow                 golden :261-273
//    btDefaultPosClick        golden :275-313
//    gbUpperDblClick          golden :315-321 GATE (C1) narrow
//    gbLowerDblClick          golden :323-328 GATE (C1) narrow
//    Timer1Timer              golden :330-337
//    Button1Click             golden :339-345 GATE (C1) narrow
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  All four gates below are NARROW (T1-style): only the single unavailable/
//  policy-blocked call is `#if 0`-gated; every other statement in the same
//  function (state flags, ListBox echo, chart/edit updates) stays ACTIVE and
//  is translated faithfully, because none of it depends on the gated call's
//  result (verified per-function, this wave).
//
//  (Q1) fQwertyKey->ShowQwertyKey(...) -- edMinMouseDown golden :147,
//      edMaxMouseDown golden :155, edLowerMouseDown golden :178,
//      edUpperMouseDown golden :192. MISSING-DEPENDENCY gate, not safety:
//      `fQwertyKey` (the on-screen numeric-entry keyboard form) has NO
//      compiled declaration or definition anywhere in this tree -- this is
//      the SAME established tree-wide gate cited by ATC/ATCInterface.h:96-103,
//      forms/fLotInfo.h:172-178/338-357, forms/fConfiguration.h:78-112,
//      forms/fSetup.h:96-115, EJ1N/MyOmronPanel.h:77-124, forms/fSpeed.h:73,
//      OmronLaser/LaserSensor.h:108-113, MyTempPanel.h:54, Automation/
//      automation.h:70, Automation/AGV_PortScan.h:126, Automation/
//      SCK_ART_Remainder.h:69 (re-verified this wave, 20260820:
//      `grep -rn "class TfQwertyKey\|TfQwertyKey *\*\|fQwertyKey *="` over the
//      whole tree -- 0 compiled hits). Per forms/fSetup.h's own instruction
//      ("跨檔缺口 GATE 不自建 shim") no local stand-in is built here either.
//      `N_INTEGER` itself IS real (cmydef.h:289, `extern const int N_INTEGER;`)
//      but becomes unreferenced once gated -- cited for completeness only.
//      OPENED 20260824 (FW-QWKEY2): FW-QWKEY1 (fc08e09) landed forms/
//      fQwertyKey.{h,cpp}; all 4 Q1 sites in DynamicTemp.cpp are un-gated
//      and live (latent: runtime instance NULL until HTEdit GATE (6)
//      wiring). N_INTEGER is referenced again.
//      Since the ONLY use of each function's `Sender` parameter was inside
//      the gated call, all four MouseDown handlers reduce to a
//      zero-parameter signature (nothing surviving reads Button/Shift/X/Y
//      either) -- see DEVIATIONS below.
//
//  (C1) COM2->TempComm6->{WriteCommData,StopComm,StartComm}(...) --
//      lblRealTime6Click golden :50, lblRealTime2Click golden :126-127,
//      gbUpperDblClick golden :318, gbLowerDblClick golden :326-327,
//      Button1Click golden :342. MISSING-DEPENDENCY gate: `COM2` itself DOES
//      have a port (`TCOM2Shim`, atester_shims.h:364-404, `extern TCOM2Shim
//      *COM2;` at :405) but that shim's member list is scoped to exactly what
//      the W5 tester-comm wave needed (`bCCDDummyRum`, `ATCAlarmSenCheck()`) --
//      it carries NO `TempComm6` member (grepped this session, 20260820:
//      `grep -n TempComm6 atester_shims.h` -- 0 hits across the whole file,
//      including inside the class body lines 364-404). This exact call shape
//      is ALREADY a known gate elsewhere in this tree: atester.cpp's own
//      GATE G-PTk3-GetTempICResult (golden-verbatim pair, atester.cpp
//      :8816-8900) carries the identical `COM2->TempComm6->WriteCommData(...)`
//      line (golden atester.cpp :8857) as inert `#if 0` reference text for
//      the same reason -- this file's gate is the same shape, reused for a
//      different golden call site. `Spcomm::TComm::WriteCommData`/
//      `StopComm`/`StartComm` themselves (vclcompat/Comm.h) are real,
//      SIM-safe, and used ungated at 116/63/37 other call sites tree-wide
//      (vclcompat/Comm.h banner) -- the gap here is specifically `TCOM2Shim`
//      not yet exposing `TempComm6`, not the underlying `TComm` API. Fixing
//      the gap (adding a `TempComm6` member to `TCOM2Shim`) is out of this
//      wave's 2-file write boundary (atester_shims.h is not one of the two
//      files this wave may touch) -- left as a gate, not "solved" by editing
//      a shared header outside scope.
//      NOTE: the task brief classified `lblRealTime*Click` as "pure display,
//      no gate" without accounting for this member gap; `gbUpperDblClick`/
//      `gbLowerDblClick`/`Button1Click` are structurally IDENTICAL bodies
//      (same COM2 calls, verified by direct comparison, this wave) and were
//      not named in either the brief's "pure display" or "safety" list at
//      all. All five are treated identically here (translate everything
//      that compiles, gate only the literal `COM2->TempComm6` chain) because
//      treating structurally-identical bodies differently based on which
//      name happened to be enumerated would be an arbitrary inconsistency,
//      not a translation decision -- see the .cpp's own per-function comments.
//
//  (D1) `ListBox1->Items->LoadFromFile(OpenDialog2->FileName);` --
//      btTempICLoadClick golden :73. SAFETY-classified per task brief, NOT a
//      missing-dependency gate: `TStringList::LoadFromFile` (vclcompat/
//      TStringList.h:245) already exists and would compile and run today --
//      it is a real, unrestricted disk READ of an arbitrary
//      dialog-selected path. Reserved for explicit user sign-off before this
//      wave lets a facade build actually touch the filesystem here, same
//      posture as cTemperFrom.cpp's GATE (T1). BEHAVIOUR DELTA while gated:
//      `OpenDialog2->Execute()` (translated faithfully, always returns
//      `false` per the facade's own file-dialog stand-in -- see DESIGN NOTE
//      below) already makes the enclosing `if` body unreachable at runtime
//      in THIS wave's build regardless of the gate, so this is belt-and-
//      suspenders, matching the task's explicit ask for a real `#if 0`
//      irrespective of that natural inertness. Everything else inside the
//      `if` -- the `Count`-driven parse loop, the per-channel `SetEdit[]`/
//      `Chart1->Series[]` population -- touches only in-process facade
//      objects (TStringList/TEdit/TfDynamicTempChart), has no missing
//      dependency, and stays ACTIVE.
//
//  (D2) `ListBox1->Items->SaveToFile(SaveDialog1->FileName);` --
//      btTempICSaveClick golden :106. Same SAFETY classification as (D1): a
//      real, unrestricted disk WRITE of an arbitrary dialog-selected path.
//      `ListBox1->Clear();` right after (golden :107) is a pure in-process
//      state reset with no disk dependency and stays ACTIVE.
//
//  NOT GATED, THOUGH NAMED IN THE TASK'S SAFETY CLUSTER --
//  sbTempICClearClick (golden :111-121)
//  --------------------------------------------------------------------------
//  The task brief groups `btTempICLoad`/`btTempICSave`/`sbTempICClear` as one
//  "temperature IC measurement chain" needing a safety gate at any real
//  hardware/file call point. Read in full this wave: sbTempICClearClick's
//  body is `for(j<16) SetEdit[j]->Text=""; for(j<18)
//  Chart1->Series[j]->Clear(); ListBox1->Clear(); btestReadTemp=false;` --
//  four in-process state resets, zero file I/O, zero COM2/hardware calls.
//  Per the brief's own stated criterion ("真正發出硬體動作或寫檔的呼叫點"),
//  there is no call point here that qualifies, so nothing is gated; the
//  function is translated fully ACTIVE. (It is also called internally, as a
//  helper, from btTempICLoadClick's ACTIVE prologue -- golden :71 -- which is
//  itself unaffected, since that call happens before the gated LoadFromFile.)
//
//  DEPENDENCY AUDIT (this wave, 20260820)
//  --------------------------------------------------------------------------
//  Grepped/read before translating:
//   * `iTempICTask` -- REAL, defined `atester.cpp:149` (`int
//     iTempICTask=1;`), also written `atester.cpp:1434`. This file's
//     `extern int iTempICTask;` (golden DynamicTemp.cpp:17) links against
//     that definition -- not a gap, translated verbatim.
//   * `GetTempICResult()` -- REAL, defined `atester.cpp:8901`
//     (`bool GetTempICResult(){ ... return false; }` -- a deliberate TODO(W7)
//     stub per that file's own banner, NOT missing). This file's `extern bool
//     GetTempICResult();` (golden DynamicTemp.cpp:18) links against that
//     definition -- translated verbatim, calling into an already-inert stub
//     is not this wave's gap to gate.
//   * `bTempComm6ReceiveOK` -- REAL, `extern bool bTempComm6ReceiveOK;`
//     (cmydef.h:3187), defined `cmydef.cpp:3418`.
//   * `InitialOK` -- REAL, `extern bool InitialOK;` (cmydef.h:220).
//   * `N_INTEGER` -- REAL, `extern const int N_INTEGER;` (cmydef.h:289); see
//     GATE (Q1) (unreferenced once gated).
//   * `TListBox`/`TPanel`/`TGroupBox`/`TCheckBox`/`TEdit`/`TCustomEdit` --
//     REAL, vclcompat/Controls.h, brought to GLOBAL scope by that header's
//     own trailing `using` list (:470-488) -- no local `using` needed here.
//   * `AnsiString`/`TObject`/`TStringList` -- REAL, brought to GLOBAL scope by
//     vclcompat/vcl_compat.h's own trailing `using` list (:244-279).
//   * `TStringList::Insert/LoadFromFile/SaveToFile/Count/Strings[]/Clear` --
//     all REAL (vclcompat/TStringList.h). `Strings[i]` returns a
//     `StringsProxy` (read-only `operator AnsiString()`, no forwarded
//     `.Pos()`/`.SubString()`) -- see DEVIATION below on materializing it.
//   * `AnsiString::Pos/SubString/Length/sprintf/printf/c_str` -- all REAL
//     (vclcompat/AnsiString.h).
//   * `FormatFloat` -- REAL, `vclcompat/SysUtils.h:41`, brought to GLOBAL
//     scope by vcl_compat.h:279.
//   * `COM2`/`TCOM2Shim` -- REAL but missing `TempComm6` -- see GATE (C1).
//   * `fQwertyKey` -- real since FW-QWKEY1 (fc08e09) -- GATE (Q1) OPENED
//     20260824.
//   * `rs232.h`/`myQwertyKeyBoard.h` (golden's own includes) -- NEITHER is
//     ported UNDER THOSE NAMES; since 20260824 the keyboard lives at
//     forms/fQwertyKey.h (FW-QWKEY1) and IS included; rs232.h remains
//     dropped per the "don't shim a portless header" convention above.
//   * `cprod.h` (golden's own include) -- grepped this wave: DynamicTemp.cpp
//     touches nothing from it (no `Prod.`/`TestIF.`/`Temperature.` reference
//     anywhere in the file) -- DROPPED as unused, not a gap.
//   * vclcompat/Controls.h carries NO geometry (`Top`/`Left`/`Width`/
//     `Height`) or `Tag`/`BevelOuter` on `TControl`/`TPanel`/`TGroupBox`
//     anywhere (grepped this wave, 20260820: `grep -n "int Top|int Left|
//     int Width|int Height|bvLowered|bvRaised" vclcompat/Controls.h` -- 0
//     hits) -- see DESIGN NOTES below for the facade-only wrapper types this
//     wave adds instead of forking that shared header.
//
//  DEVIATION -- materializing `Strings[ct]` before chaining a method call
//  --------------------------------------------------------------------------
//  Golden: `ListBox1->Items->Strings[ct].Pos("T1:")`,
//  `...Strings[ct].SubString(1,10)`, etc. (btTempICLoadClick, golden :77/
//  :80-81/:86-87). `TStringList::Strings[i]` (vclcompat/TStringList.h:253-256)
//  returns a `StringsProxy` value whose ONLY members are `operator
//  AnsiString() const` and assignment operators -- it does not forward
//  `.Pos()`/`.SubString()`/`.Length()`, so chaining those directly off the
//  subscript expression does not compile (a proxy's own method set has to
//  contain the name; an implicit conversion operator is not consulted for
//  member-call lookup). This file materializes the proxy into a real
//  `AnsiString sLine = ListBox1->Items->Strings[ct];` once per loop iteration
//  and chains every `.Pos()`/`.SubString()` off `sLine` instead -- a
//  PORT-ONLY adaptation (zero behaviour change: `operator AnsiString()`
//  performs the identical read golden's implicit BCB6 property-to-value
//  conversion would have done at each use site; this file just does it once
//  and reuses the result instead of re-reading the same index repeatedly).
//
//  DEVIATION -- raw C-runtime `sprintf(buf,"%s", AnsiStringExpr)` needs
//  `.c_str()`
//  --------------------------------------------------------------------------
//  Golden: `sprintf(cData, "%s", ...SubString(1,10));` (btTempICLoadClick,
//  golden :80-81) passes an `AnsiString` rvalue straight into the C runtime's
//  variadic `::sprintf`. Real BCB6 `AnsiString` has an implicit `operator
//  const char*()` that made this legal; THIS PORT'S `AnsiString`
//  (vclcompat/AnsiString.h) deliberately has no such operator (see that
//  header's own :33-36 note on why its member `sprintf`/`printf` are
//  templated instead) -- passing a class object through `...` here would not
//  yield a valid `const char*` for glibc's `%s`. This file appends `.c_str()`
//  at both call sites -- zero behaviour change (the templated member
//  `sprintf`/`printf` family this same file also calls, e.g. `asCh.printf(
//  "T%d:", i+1)`, is unaffected -- it already handles `AnsiString` args via
//  its own `conv()` helper, only the RAW C-runtime `::sprintf` calls needed
//  the explicit adaptation).
//
//  DEVIATION -- dropped/simplified parameters (Sender/Button/Shift/X/Y)
//  --------------------------------------------------------------------------
//  Golden gives every event handler a `TObject *Sender` (MouseDown/Up/Move
//  handlers add `TMouseButton Button, TShiftState Shift, int X, int Y`).
//  Matching the tree-wide "keep only what's read" convention (forms/
//  fLotInfo.h's own WB-9/WB-11 notes; forms/fTemperFrom.h's FormClose
//  DEVIATION), each handler below keeps ONLY the parameters its OWN body
//  (after gating, where applicable) actually reads:
//   * `btExitClick/lblRealTime6Click/lblRealTime1DblClick/
//     btTempICLoadClick/btTempICSaveClick/sbTempICClearClick/
//     lblRealTime2Click/btShowClick/FormResize/btSelectAllClick/FormShow/
//     btDefaultPosClick/gbUpperDblClick/gbLowerDblClick/Timer1Timer/
//     Button1Click` -- `Sender` never read anywhere in the body -> dropped,
//     zero-parameter signature.
//   * `edMinMouseDown/edMaxMouseDown/edLowerMouseDown/edUpperMouseDown` --
//     `Sender`'s ONLY read was inside the now-gated GATE (Q1) call;
//     `Button`/`Shift`/`X`/`Y` are never read at all -> all five dropped,
//     zero-parameter signatures.
//   * `gbA1MouseUp/gbA1MouseDown/gbA1MouseMove` -- `Sender` IS read (cast to
//     a GroupBox pointer); `gbA1MouseDown`/`gbA1MouseMove` also read `X`/`Y`.
//     `Button`/`Shift` never read -> dropped. Kept signature: `Sender` typed
//     `void*` (matches forms/fObserver.h's `SpeedButton1Click(void *Sender)`
//     convention for a Sender that is immediately cast to a concrete
//     pointer type, rather than re-declaring golden's `TObject*` spelling).
//   * `btSelectAllMouseDown/btSelectAllMouseUp` -- `Sender` IS read (cast to
//     a Panel pointer); `Button`/`Shift`/`X`/`Y` never read -> dropped except
//     `Sender` (`void*`, same convention).
//   sbTempICClearClick's OWN internal call site (golden :71, `
//   sbTempICClearClick(this);` inside btTempICLoadClick) becomes
//   `sbTempICClearClick();` (no argument) to match its own dropped-Sender
//   signature -- the callee never read the argument either way.
//
//  DEVIATION -- Sender cast TARGET TYPE changed (TGroupBox*/TPanel* ->
//  facade wrapper types)
//  --------------------------------------------------------------------------
//  Golden: `Ptr=(TGroupBox *)Sender;` (gbA1MouseUp/Down/Move) and
//  `Ptr=(TPanel *)Sender;` (btSelectAllMouseDown/Up). Both then read/write
//  `Ptr->Tag`/`Ptr->Left`/`Ptr->Top` (GroupBox) or `Ptr->BevelOuter`
//  (Panel) -- NONE of which `vclcompat::TGroupBox`/`vclcompat::TPanel` carry
//  (see DESIGN NOTES below). This file casts to `TfDynamicTempGroupBox*`/
//  `TfDynamicTempPanel*` instead -- the facade-only wrapper types below that
//  add exactly those fields -- so the cast target is the type that actually
//  has the member being accessed. Every widget these handlers can plausibly
//  receive as `Sender` at runtime (the 18 `gbA1`..`gbLower` GroupBoxes;
//  `btSelectAll`, the one Panel this handler pair is named after) is already
//  declared as that same wrapper type below, so no other cast target is ever
//  needed.
//
//  DESIGN NOTE -- geometry/Tag/BevelOuter wrapper types (compose, don't fork
//  vclcompat/Controls.h)
//  --------------------------------------------------------------------------
//  `FormResize` (golden :160-173) and `btDefaultPosClick` (golden :275-313)
//  read/write `->Top`/`->Left`/`->Width`/`->Height` on `Panel1`/`btShow`/
//  `btSelectAll`/`sbTempICClear`/`btDefaultPos`/the 18 GroupBoxes/`edMin`;
//  `btSelectAllMouseDown/Up` read/write `->BevelOuter`; `gbA1MouseUp/Down/
//  Move` read/write `->Tag`/`->Left`/`->Top`. `vclcompat::TControl` (the
//  common base of `TPanel`/`TGroupBox`/`TEdit`) carries only `Visible`/
//  `Enabled`/`hCtl` (vclcompat/Controls.h:213-223) -- no geometry, no `Tag`,
//  and `TPanel` itself carries no `BevelOuter` (:240-246). Rather than fork
//  or edit that shared, ~110-TU-reaching header (out of this wave's write
//  boundary), three small facade-only wrapper types below PUBLICLY INHERIT
//  the relevant vclcompat base and add exactly the fields this file's
//  methods touch -- same "compose, don't fork" pattern as forms/
//  fTemperFrom.h's `TfTemperFromPanel : public vclcompat::TPanel` and forms/
//  fObserver.h's `TfObserverChart`/`TfObserverGrid`:
//    TfDynamicTempPanel    : public vclcompat::TPanel     (+Top/Left/Width/
//                            Height/BevelOuter) -- Panel1, btShow,
//                            btSelectAll, sbTempICClear, btDefaultPos.
//    TfDynamicTempGroupBox : public vclcompat::TGroupBox  (+Tag/Top/Left) --
//                            all 18 gbA1..gbLower.
//    TfDynamicTempEdit     : public vclcompat::TEdit      (+Top) -- edMin
//                            only (the only Edit widget FormResize's
//                            geometry write touches; edMax/edUpper/edLower/
//                            edtRealTime1..16 need no geometry and stay
//                            plain `TEdit*`).
//  `TPanelBevel` (bvNone/bvLowered/bvRaised/bvSpace, golden Graphics.hpp,
//  real VCL declared order) is declared at GLOBAL scope (not nested) so
//  `Ptr->BevelOuter=bvLowered;` reads exactly like golden's own unqualified
//  VCL spelling from a `TfDynamicTemp` member function. Grepped this wave
//  (20260820) before choosing global scope: the only existing precedent is
//  `vclcompat::TBevelShim` (vclcompat/BtnPanelCore.h:61), which is INSIDE
//  `namespace vclcompat` -- a different scope, no collision; 0 hits for any
//  GLOBAL-scope `TPanelBevel`/`bvLowered`/`bvRaised`/`bvNone`/`bvSpace`
//  declaration anywhere in the tree.
//
//  DESIGN NOTE -- TfDynamicTempChart / TfDynamicTempChartSeries (Chart1)
//  --------------------------------------------------------------------------
//  Golden `Chart1` is a `TChart*` (TeeChart, golden DynamicTemp.h:25) wired
//  in the `.dfm` to 18 PRE-EXISTING named `TLineSeries` (Series1..Series18,
//  golden :26-41/:46-47) -- golden's OWN ctor code (:20-40) never
//  constructs or touches them; they simply exist once the form streams in.
//  This is the SAME shape as forms/fObserver.h's `ChartYield` (32
//  PRE-EXISTING series, "PORT-ONLY bootstrapping" in that facade's own ctor,
//  see that header's DESIGN NOTES) -- `TfDynamicTempChart`'s own ctor
//  pre-populates 18 `TfDynamicTempChartSeries` the same way, matching that
//  precedent exactly. `TfObserverChart`/`TfObserverChartSeries` themselves
//  are NOT reused here (different translation wave, different file --
//  reusing across form facades would create the same kind of cross-file
//  coupling the "two TMyKitSuck headers" gotcha warns against if either
//  facade's shape ever needs to diverge) -- `TfDynamicTempChart`/
//  `TfDynamicTempChartSeries` are this wave's OWN, differently-named,
//  minimal stand-ins with the identical shape (`Series[i]->AddY(value,text,
//  color)`/`->Clear()`/`->Active`/`->Count()`) plus the two members THIS
//  file's methods additionally need that fObserver's chart never touched:
//  `LeftAxis` (a pointer, matching golden's `Chart1->LeftAxis->Minimum=...`
//  `->` syntax -- real VCL `TChart::LeftAxis` is itself a `TChartAxis*`
//  property) with `Minimum`/`Maximum` (`double`, matching real VCL
//  `TChartAxis`'s property type -- golden always feeds these via `atoi()`
//  int locals, an int->double widening, zero behaviour change), and `Height`
//  (`int`, read-only this wave -- `FormResize`'s `edMin->Top=Chart1->Height
//  -30;`).
//
//  DESIGN NOTE -- TfDynamicTempFileDialog (OpenDialog2, SaveDialog1)
//  --------------------------------------------------------------------------
//  Golden `OpenDialog2`/`SaveDialog1` are `TOpenDialog*`/`TSaveDialog*`
//  (golden DynamicTemp.h:44-45); only `->FileName` and `->Execute()` are
//  ever touched (no `->Title`/`->Filter`/etc.). Same shape as forms/
//  fLotInfo.h's own `TfLotInfoOpenDialog` (that header's own note: "Offline
//  Execute() always returns false ('user cancelled') -- there is no real
//  modal file picker in a headless build, matching this tree's established
//  'no UI -> the interactive path never completes' convention"). One shared
//  type, `TfDynamicTempFileDialog`, covers both golden dialog classes here
//  (neither golden call site needs anything beyond `FileName`/`Execute()`,
//  and golden's own `TOpenDialog`/`TSaveDialog` are both `TOpenDialog`
//  subclasses in real VCL with an identical touched-surface here) --
//  independently named (not reusing `TfLotInfoOpenDialog`) to avoid the same
//  cross-file coupling the ChartSeries DESIGN NOTE above explains.
//
//  EXCLUDED WIDGETS (golden __published members with NO Wave A member
//  dereferencing them by name -- matches forms/fTemperFrom.h's own "only
//  what's dereferenced" convention; adding unused surface would be
//  scope-creep this class does not need yet)
//  --------------------------------------------------------------------------
//    btExit (TPanel*, golden h:23) -- btExitClick's body is only `Close();`,
//      never touches the widget itself.
//    PanelMain7 (TPanel*, h:24), Label4 (TLabel*, h:43), Panel2 (TPanel*,
//      h:56) -- never referenced anywhere in DynamicTemp.cpp.
//    Series1..Series16 (TLineSeries*, h:26-41), Series17/Series18
//      (TLineSeries*, h:46-47) -- never referenced BARE; every touch is via
//      `Chart1->Series[i]` (see TfDynamicTempChart DESIGN NOTE -- the 18
//      series are bootstrapped directly into `Chart1`, not held as
//      individual `TfDynamicTemp` members, matching golden's own
//      `.dfm`-streamed-not-ctor-constructed shape).
//    btTempICSave (TPanel*, h:58), btTempICLoad (TPanel*, h:59) -- their own
//      `OnClick` bodies (btTempICSaveClick/btTempICLoadClick) never
//      dereference the widget itself, only OpenDialog2/SaveDialog1/
//      ListBox1/Chart1/SetEdit[].
//    gbUpperDbl (TLabel*, h:112), gbLowerDbl (TLabel*, h:113) -- their own
//      `OnDblClick` bodies (gbUpperDblClick/gbLowerDblClick) never
//      dereference the widget itself, only ListBox1/COM2.
//    Timer1 (TTimer*, h:114) -- Timer1Timer's body never dereferences
//      `Timer1->` (only reads `InitialOK`/`btestReadTemp`, calls
//      `GetTempICResult()`).
//    Button1 (TButton*, h:115) -- Button1Click's body never dereferences
//      `Button1->`, only ListBox1/COM2 (identical body shape to
//      lblRealTime6Click/gbUpperDblClick -- see GATE (C1)).
//
//  SINGLE-INSTANCE ASSUMPTION (inherited from golden, not new)
//  --------------------------------------------------------------------------
//  Golden's own one-`fDynamicTemp`-per-process assumption (a real BCB6 run
//  only ever streams one instance of this form). `fDynamicTemp` itself is
//  declared, NOT `new`'d, at the bottom of cDynamicTemp... i.e. this wave's
//  DynamicTemp.cpp (zero-initialized, matching golden's own
//  `TfDynamicTemp *fDynamicTemp;` PACKAGE global and this tree's
//  static-init-order safety rule for cross-TU singletons -- see
//  docs/KNOWLEDGE.md's "V906 homecoming SIOF lottery" entry).
// =============================================================================
#ifndef FORMS_FDYNAMICTEMP_H
#define FORMS_FDYNAMICTEMP_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject/TStringList at global scope
#include "vclcompat/Controls.h"    // TPanel/TGroupBox/TEdit/TCheckBox/TListBox at global scope

#include <vector>

// ---------------------------------------------------------------------------
//  LOCALLY PROVIDED: golden Graphics.hpp `TColor` -- vclcompat/Controls.h
//  deliberately keeps its own Color members plain `int` rather than pull in
//  a TColor definition (see that header's own note); this facade's `TC[]`
//  array and Chart series DO need the golden property name, so it defines
//  the one-line alias here -- SAME idiom as forms/fObserver.h's own
//  HT9045_W906_FOBSERVER_TCOLOR_SHIM block (identical `typedef int TColor;`,
//  differently-named guard to avoid a duplicate-macro warning if both
//  headers are ever included in one TU; the typedef itself is harmless to
//  repeat, this tree already carries N independent copies of it -- see
//  cmydef.h:16, the "canonical" one).
// ---------------------------------------------------------------------------
#ifndef HT9045_W906_FDYNAMICTEMP_TCOLOR_SHIM
#define HT9045_W906_FDYNAMICTEMP_TCOLOR_SHIM
typedef int TColor;
#endif

// ---------------------------------------------------------------------------
//  LOCALLY PROVIDED: golden Graphics.hpp `TPanelBevel` -- needed by
//  TfDynamicTempPanel::BevelOuter (btSelectAllMouseDown/Up). See this file's
//  own DESIGN NOTE above for why this is GLOBAL scope, not nested.
// ---------------------------------------------------------------------------
#ifndef HT9045_W906_FDYNAMICTEMP_BEVEL_SHIM
#define HT9045_W906_FDYNAMICTEMP_BEVEL_SHIM
enum TPanelBevel { bvNone, bvLowered, bvRaised, bvSpace };
#endif

// ---------------------------------------------------------------------------
//  TfDynamicTempPanel -- see DESIGN NOTE above. Panel1, btShow, btSelectAll,
//  sbTempICClear, btDefaultPos.
// ---------------------------------------------------------------------------
class TfDynamicTempPanel : public vclcompat::TPanel
{
public:
    int         Top;
    int         Left;
    int         Width;
    int         Height;
    TPanelBevel BevelOuter;

    TfDynamicTempPanel()
        : Top(0), Left(0), Width(0), Height(0), BevelOuter(bvRaised) {}   // bvRaised: real VCL TPanel default
};

// ---------------------------------------------------------------------------
//  TfDynamicTempGroupBox -- see DESIGN NOTE above. All 18 gbA1..gbLower.
// ---------------------------------------------------------------------------
class TfDynamicTempGroupBox : public vclcompat::TGroupBox
{
public:
    // AI(W906-FW-TAG1) 20260825: `int Tag;` removed -- it now lives on
    // vclcompat::TControl for every widget (see the provenance note there);
    // keeping a copy here would shadow it.  Top/Left stay: vclcompat models
    // no geometry, so those two are still this wrapper's reason to exist.
    int Top;
    int Left;

    TfDynamicTempGroupBox() : Top(0), Left(0) {}
};

// ---------------------------------------------------------------------------
//  TfDynamicTempEdit -- see DESIGN NOTE above. edMin only.
// ---------------------------------------------------------------------------
class TfDynamicTempEdit : public vclcompat::TEdit
{
public:
    int Top;

    TfDynamicTempEdit() : Top(0) {}
};

// ---------------------------------------------------------------------------
//  TfDynamicTempFileDialog -- see DESIGN NOTE above. OpenDialog2, SaveDialog1.
// ---------------------------------------------------------------------------
class TfDynamicTempFileDialog : public vclcompat::TObject
{
public:
    AnsiString FileName;

    TfDynamicTempFileDialog() {}
    virtual ~TfDynamicTempFileDialog() {}
    bool Execute() { return false; }   // no real modal file picker headless (see DESIGN NOTE)
};

// ---------------------------------------------------------------------------
//  TfDynamicTempChartSeries / TfDynamicTempChart -- see DESIGN NOTE above.
// ---------------------------------------------------------------------------
class TfDynamicTempChartSeries : public vclcompat::TObject
{
public:
    struct Point { double Value; AnsiString Text; TColor Color; };

    bool               Active;
    std::vector<Point> Points;

    TfDynamicTempChartSeries() : Active(false) {}
    virtual ~TfDynamicTempChartSeries() {}

    void Clear() { Points.clear(); }
    void AddY(double value, const AnsiString &text, TColor color)
    {
        Point p; p.Value = value; p.Text = text; p.Color = color;
        Points.push_back(p);
    }
    int Count() const { return static_cast<int>(Points.size()); }
};

class TfDynamicTempChart : public vclcompat::TObject
{
public:
    // golden `Chart1->LeftAxis->Minimum/Maximum` -- `->` syntax means real
    // VCL's `LeftAxis` is itself a pointer property (`TChartAxis*`); modeled
    // the same way here, see DESIGN NOTE above.
    struct AxisType { double Minimum; double Maximum; AxisType() : Minimum(0), Maximum(0) {} };

    AxisType *LeftAxis;
    int       Height;   // golden `Chart1->Height` (FormResize, read-only this wave)

    TfDynamicTempChart() : Height(0)
    {
        LeftAxis = new AxisType();
        // golden .dfm wires 18 named TLineSeries (Series1..Series18) onto
        // Chart1 at design time -- PORT-ONLY bootstrapping, matching forms/
        // fObserver.h's own ChartYield precedent (see DESIGN NOTE above).
        for (int i = 0; i < 18; ++i)
            seriesList_.push_back(new TfDynamicTempChartSeries());
    }
    virtual ~TfDynamicTempChart()
    {
        delete LeftAxis;
        for (size_t i = 0; i < seriesList_.size(); ++i)
            delete seriesList_[i];
    }

    struct SeriesAccessor
    {
        TfDynamicTempChart *owner;
        TfDynamicTempChartSeries *operator[](int idx) const { return owner->seriesList_.at(static_cast<size_t>(idx)); }
    };
    SeriesAccessor Series{this};

private:
    std::vector<TfDynamicTempChartSeries *> seriesList_;
};

// ===========================================================================
//  TfDynamicTemp -- non-VCL facade (golden DynamicTemp.h, TfDynamicTemp:public TForm)
// ===========================================================================
class TfDynamicTemp
{
public:
    TfDynamicTemp();   // golden `__fastcall TfDynamicTemp(TComponent* Owner)` --
                        // Owner param dropped, matches every other translated
                        // form facade's ctor (e.g. forms/fTemperFrom.h's own
                        // `TfTemperFrom();`).

    // -- Chart + its 18 series (golden DynamicTemp.h:25/26-41/46-47) --------
    TfDynamicTempChart *Chart1 = new TfDynamicTempChart();

    // -- TeeChart-adjacent guard-band edits (golden h:48-49/107/110) --------
    // HARD DEPENDENCY: uTemp_Set.cpp (sibling wave, not yet landed) writes
    // these four through `extern TfDynamicTemp *fDynamicTemp;` -- see banner.
    TEdit             *edMax   = new TEdit();
    TfDynamicTempEdit *edMin   = new TfDynamicTempEdit();   // ->Top written by FormResize
    TEdit             *edUpper = new TEdit();
    TEdit             *edLower = new TEdit();

    // -- the 16 realtime-channel edits (golden h:52/54/75-81/88-94) ----------
    // Golden order preserved (ctor references every one of these bare to
    // populate SetEdit[], so all 16 must exist as real members -- same "ctor
    // in scope needs the real names" rule as forms/fTemperFrom.h's 142+10
    // panel note).
    TEdit *edtRealTime1  = new TEdit(),  *edtRealTime2  = new TEdit();
    TEdit *edtRealTime3  = new TEdit(),  *edtRealTime4  = new TEdit();
    TEdit *edtRealTime5  = new TEdit(),  *edtRealTime6  = new TEdit();
    TEdit *edtRealTime7  = new TEdit(),  *edtRealTime8  = new TEdit();
    TEdit *edtRealTime9  = new TEdit(),  *edtRealTime10 = new TEdit();
    TEdit *edtRealTime11 = new TEdit(),  *edtRealTime12 = new TEdit();
    TEdit *edtRealTime13 = new TEdit(),  *edtRealTime14 = new TEdit();
    TEdit *edtRealTime15 = new TEdit(),  *edtRealTime16 = new TEdit();

    // -- the 18 series-select checkboxes (golden h:51/55/74/82-87/95-101/
    // 108/111) -- ctor populates myCB[] from these bare names. --------------
    TCheckBox *CheckBox1  = new TCheckBox(),  *CheckBox2  = new TCheckBox();
    TCheckBox *CheckBox3  = new TCheckBox(),  *CheckBox4  = new TCheckBox();
    TCheckBox *CheckBox5  = new TCheckBox(),  *CheckBox6  = new TCheckBox();
    TCheckBox *CheckBox7  = new TCheckBox(),  *CheckBox8  = new TCheckBox();
    TCheckBox *CheckBox9  = new TCheckBox(),  *CheckBox10 = new TCheckBox();
    TCheckBox *CheckBox11 = new TCheckBox(),  *CheckBox12 = new TCheckBox();
    TCheckBox *CheckBox13 = new TCheckBox(),  *CheckBox14 = new TCheckBox();
    TCheckBox *CheckBox15 = new TCheckBox(),  *CheckBox16 = new TCheckBox();
    TCheckBox *CheckBox17 = new TCheckBox(),  *CheckBox18 = new TCheckBox();

    // -- the 18 draggable channel-box GroupBoxes (golden h:50/53/60-73/106/
    // 109) -- btDefaultPosClick + gbA1MouseUp/Down/Move's generic Sender
    // cast target. ------------------------------------------------------
    TfDynamicTempGroupBox *gbA1  = new TfDynamicTempGroupBox(), *gbB2  = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbA3  = new TfDynamicTempGroupBox(), *gbB4  = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbC5  = new TfDynamicTempGroupBox(), *gbD6  = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbC7  = new TfDynamicTempGroupBox(), *gbD8  = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbE9  = new TfDynamicTempGroupBox(), *gbF10 = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbE11 = new TfDynamicTempGroupBox(), *gbF12 = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbG13 = new TfDynamicTempGroupBox(), *gbH14 = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbG15 = new TfDynamicTempGroupBox(), *gbH16 = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbUpper = new TfDynamicTempGroupBox();
    TfDynamicTempGroupBox *gbLower = new TfDynamicTempGroupBox();

    // -- Panels FormResize/btSelectAllMouseDown/Up touch (golden h:42/58-59/
    // 102-105) -- btTempICSave/btTempICLoad EXCLUDED, see banner. -----------
    TfDynamicTempPanel *Panel1        = new TfDynamicTempPanel();
    TfDynamicTempPanel *btShow        = new TfDynamicTempPanel();
    TfDynamicTempPanel *btSelectAll   = new TfDynamicTempPanel();   // ->BevelOuter (MouseDown/Up)
    TfDynamicTempPanel *sbTempICClear = new TfDynamicTempPanel();
    TfDynamicTempPanel *btDefaultPos  = new TfDynamicTempPanel();

    // -- Temp-IC load/save chain (golden h:44-45/56-57) ---------------------
    TfDynamicTempFileDialog *OpenDialog2  = new TfDynamicTempFileDialog();
    TfDynamicTempFileDialog *SaveDialog1  = new TfDynamicTempFileDialog();
    TListBox                *ListBox1     = new TListBox();

    // -- public arrays golden declares OUTSIDE __published (golden h:155-157)
    TColor     TC[18];
    TEdit     *SetEdit[18];
    TCheckBox *myCB[18];

private:
    // golden h:151-152 (private: User declarations) -- gbA1MouseDown/Move drag state.
    int iStartX = 0;
    int iStartY = 0;

public:
    // -- Wave A translated methods (bodies: DynamicTemp.cpp, this wave) -----
    // Parameter lists already reflect the DEVIATIONS documented above.
    void btExitClick();                                       // golden :42-45
    void lblRealTime6Click();                                 // golden :47-53   GATE (C1)
    void lblRealTime1DblClick();                              // golden :55-58
    void btTempICLoadClick();                                 // golden :60-99   GATE (D1)
    void btTempICSaveClick();                                 // golden :101-109 GATE (D2)
    void sbTempICClearClick();                                // golden :111-121 (no gate)
    void lblRealTime2Click();                                 // golden :123-128 GATE (C1)
    void btShowClick();                                       // golden :130-142
    void edMinMouseDown(TObject *Sender);        // +Sender 20260824 (FW-QWKEY2): read by the live Q1 call                                    // golden :144-150 GATE (Q1)
    void edMaxMouseDown(TObject *Sender);        // +Sender 20260824 (FW-QWKEY2): read by the live Q1 call                                    // golden :152-158 GATE (Q1)
    void FormResize();                                        // golden :160-173
    void edLowerMouseDown(TObject *Sender);        // +Sender 20260824 (FW-QWKEY2): read by the live Q1 call                                  // golden :175-187 GATE (Q1)
    void edUpperMouseDown(TObject *Sender);        // +Sender 20260824 (FW-QWKEY2): read by the live Q1 call                                  // golden :189-201 GATE (Q1)
    void btSelectAllClick();                                  // golden :203-209
    void gbA1MouseUp(void *Sender);                           // golden :211-220
    void gbA1MouseDown(void *Sender, int X, int Y);           // golden :222-231
    void gbA1MouseMove(void *Sender, int X, int Y);           // golden :233-243
    void btSelectAllMouseDown(void *Sender);                  // golden :245-251
    void btSelectAllMouseUp(void *Sender);                    // golden :253-259
    void FormShow();                                          // golden :261-273
    void btDefaultPosClick();                                 // golden :275-313
    void gbUpperDblClick();                                   // golden :315-321 GATE (C1)
    void gbLowerDblClick();                                   // golden :323-328 GATE (C1)
    void Timer1Timer();                                       // golden :330-337
    void Button1Click();                                      // golden :339-345 GATE (C1)
};

// AI(W906-FW3-DynTemp-WA) 20260820: `extern TfDynamicTemp *fDynamicTemp;` and
// the golden file-scope `bool btestReadTemp=false;`/`extern int
// iTempICTask;`/`extern bool GetTempICResult();` trio are declared/defined
// in DynamicTemp.cpp (this wave), matching golden's own file-scope placement
// verbatim -- NOT declared here (see forms/fTemperFrom.h precedent: widgets
// go in the header, file-scope globals stay in the .cpp that owns them).

#endif // FORMS_FDYNAMICTEMP_H
