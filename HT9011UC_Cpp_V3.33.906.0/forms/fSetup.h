// =============================================================================
//  forms/fSetup.h  --  non-VCL stand-in for golden's fSetup form pointer
//
//  AI(W906-W7-L2) 20260803: FIRST home for TfSetup anywhere in this migrated
//  tree (golden csetup.h:14 `class TfSetup : public TForm`).  Landed by the
//  W7-L2 substrate pass ahead of the translation of golden ckernel.cpp.
//  forms/fNote.{h,cpp} convention; the facade contract in forms/fMain.h binds
//  this file too.
//
//  fSetup has been NAMED in this tree before without ever being landed --
//  forms/fMain.h:366 and :372 list it among the forms the SECS/GEM AddEC and
//  S125F4 paths reach, and tests/test_w7_f1_wall2_probe.cpp:120/:154/:157/:165/
//  :170/:191 counts it repeatedly in its "forms with no home" tallies.  This is
//  the first actual class + global.
//
//  ---------------------------------------------------------------------------
//  GOLDEN ckernel.cpp CENSUS FOR fSetup -- MEASURED OVER ALL 2589 LINES
//  ---------------------------------------------------------------------------
//    :411  if(iHome==0 && fSetup->fShow==false)   (read) -- ScanSystemSensor
//  Exactly ONE site, one member.  Nothing else in golden ckernel.cpp mentions
//  fSetup, so this facade is deliberately a single field: golden csetup.h
//  declares ~30 other public members (tSiteMap, shtMode, bSiteMapHasChange,
//  iTestMode, SaveSetupFile(), CheckShuttlePitch(), ...) and none of them is
//  reachable from the front this wave serves.  Growing the facade past its
//  measured need is how facades rot.
// =============================================================================
// =============================================================================
//  AI(W906-FW3-Setup-WA) 20260820: cSetUp Wave A (FW campaign batch 5,
//  wrap-up alongside cConfiguration/cSpeed/cStartCondition -- see forms/
//  fConfiguration.h for the sibling banner and its own GATE REGISTER, which
//  this file's WA-1 entry below cites rather than repeats). Golden ref:
//  HT9011UC_Code_V3.33.906.0_20260618/cSetUp.cpp (4,858 lines, cp950 --
//  decoded with `python3 -c "open(p,'rb').read().decode('cp950')"`, 0
//  U+FFFD, 20260820) + cSetUp.h (295 lines) + cSetUp.dfm (122,156 bytes, 0
//  U+FFFD). The ORIGINAL banner above (W7-L2, 20260803) still describes the
//  single `fShow` field faithfully and is left VERBATIM -- append-only, per
//  the fLotInfo Wave B rule. Everything from here down is new.
//
//  cSetUp IS MOSTLY NOT A DISPLAY FORM.  Unlike cConfiguration/uLotInfo, most
//  of its 44 TfSetup:: methods ARE the test-mode / site-map / shuttle-mode
//  EDITOR itself (e.g. ScrollBar1Change alone is 963 golden lines and writes
//  the global `iSiteTotal`), not a read-only mirror of already-loaded state.
//  Only a small, genuinely zero/near-zero-dependency (a) subset lands this
//  wave; the other 30 methods are (b) write-path / (c) safety-interlock /
//  MIXED and are queued (see the agent's hand-off report for the full
//  44-row classification table -- not reproduced here to avoid a second copy
//  drifting from the first).
//
//  DEPENDENCIES ADDED THIS WAVE (the "Dependency-free on purpose" note below
//  was true and load-bearing for W7-L2's OWN scope, which is unchanged; it
//  no longer describes the file as a whole after this addition):
//    cmydef.h              -- TestSiteFileName[2][TotalTestMode] (GetTestMode)
//    cprod.h               -- TestIF_File.dSiteYOffset (rgYPitchOffsetModeClick)
//    Config.h              -- IniConfig.bD30EnableSiteModeSelect (Arm1PickArm2TestClick)
//    common.h              -- OnlyNumberAndDotInPut (XPitchKeyPress)
//    vclcompat/Controls.h  -- TEdit/TCheckBox/TRadioGroup/TRadioButton/TGroupBox/TLabel
//    vclcompat/SysUtils.h  -- FormatFloat (rgYPitchOffsetModeClick)
//  Grepped this wave: none of cmydef.h/cprod.h/Config.h/common.h/vclcompat/*
//  include any forms/*.h back -- no circular include introduced. The 3
//  existing includers of this header (ckernel.cpp/Command.cpp/
//  handlerlog.cpp) already pull in cmydef.h/cprod.h/Config.h themselves as
//  core machine-logic TUs, so this is not new compile-graph weight for them.
//
//  WAVE SCOPE -- 15 of 47 golden TfSetup:: methods, all (a) 顯示/唯讀, every
//  one read line-by-line in full (zero Tier-2/signal-scan-only shortcuts):
//    GetTestMode                golden :2179-2189 (11L) -- pure lookup (TestSiteFileName), zero widget deps
//    SetShtMode                 golden :4708-4715 (8L)  -- pure member write (shtMode)
//    VertifyShtModeisDiff       golden :4717-4730 (14L) -- pure member predicate (shtMode)
//    XPitchKeyPress             golden :3403-3407 (5L)  -- pure char filter (OnlyNumberAndDotInPut)
//    RadioButton1KeyDown        golden :4443-4449 (7L)  -- DFM-ORPHANED, see GOLDEN ODDITY below
//    rgSensor1Click             golden :4772-4777 (6L)  -- pure Sender-cast widget correction
//    rgShtModeNormalClick       golden :1264-1282 (19L) -- pure widget cascade
//    Arm1PickArm2TestClick      golden :4464-4474 (11L) -- pure widget cascade
//    cbUseSLKClampClick         golden :4637-4643 (7L)  -- pure widget cascade
//    rgYPitchOffsetModeClick    golden :4779-4802 (24L) -- pure widget cascade + TestIF_File read
//    edOcrTextMouseDown         golden :4631-4635 (5L)  -- WA-1 GATE (below), body empty
//    edOverRangeClick           golden :4839-4842 (4L)  -- WA-1 GATE, body empty
//    edDelayTimeClick           golden :4844-4847 (4L)  -- WA-1 GATE, body empty
//    edAuto1CountClick          golden :4849-4852 (4L)  -- WA-1 GATE, body empty
//    edtGetValueDelayTimeClick  golden :4854-4857 (4L)  -- WA-1 GATE, body empty
//  Total: 15 methods / 133 golden lines. Denominator: TfSetup:: has 47
//  DISTINCT methods (verified this wave: `grep -oE "TfSetup::[A-Za-z_]+\("
//  cSetUp.cpp | sort -u | wc -l` = 47 -- an EARLIER pass in this same wave
//  miscounted 44 by missing the 5 non-`__fastcall` free-standing methods'
//  overlap with the `__fastcall` grep, corrected before this file was
//  finalized) over the file's 4,858 golden lines (agent's full census, this
//  wave). Every dropped `TObject *Sender` (and, where present, unread
//  `WORD &Key`/`TShiftState Shift`/`TMouseButton Button`/`int X, int Y`)
//  follows the SAME "drop unused VCL glue parameters" convention forms/
//  fConfiguration.h's own DEVIATION section documents (originally forms/
//  fLotInfo.h's WA-8 precedent) -- verified per-function this wave that the
//  dropped parameter is never read in the golden body.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (WA-1) fQwertyKey->ShowQwertyKey(...) -- the SAME established tree-wide
//      gate forms/fConfiguration.h's WA-1 cites today (see that file for the
//      full cross-tree citation list: forms/fLotInfo.h, ATC/ATCInterface.cpp,
//      VacuumUnit.cpp, VacuumUnit/MyVacuumPanel.cpp, EJ1N/MyOmronPanel.cpp,
//      Public/HTEdit.cpp). Re-verified this wave (20260820): `Grep "class
//      TfQwertyKey|TfQwertyKey\s*\*|fQwertyKey\s*="` tree-wide -- 0 compiled
//      (non-#if 0) hits. Touches the 5 ed*Click methods above; each reduces
//      to an empty body, matching forms/fConfiguration.h's own precedent for
//      the identical situation (13 of its 28 methods do the same). Per task
//      instruction ("跨檔缺口 GATE 不自建 shim"), no local fQwertyKey stand-in
//      is fabricated here -- the call is gated, nothing else.
//
//  DEFERRED THIS WAVE (cross-file gaps -- GATE, not shim, same rule as above):
//    XPitchMouseDown / XShiftPitchMouseDown -- guard on
//      ⚠ AI(W906-FW-SETUP-W19) 20260826: 下面這條 absence claim 已過期——
//      Barcode_Reader 的真本體在 BarcodeReader.cpp:445，XPitchMouseDown/
//      XShiftPitchMouseDown 的 guard 已於 20260825 開閘，rgShtModeNormalMouseDown
//      也已於本波翻進 cSetUp.cpp。原文保留為沿革：
//      `Barcode_Reader(bcSetup)`, a function with ZERO port anywhere in this
//      tree (`Grep "Barcode_Reader" .` tree-wide hits only
//      ProcessLastSetIni_Barcode_Reader, an unrelated name; BarcodeReader.cpp
//      itself is wholly untranslated) -- unlike fQwertyKey, there is not even
//      a class name other files already cite as a known gate. Gating the
//      guard AND the already-gated ShowQwertyKey body would leave a function
//      that provably does nothing; not worth the 2-widget facade footprint
//      (XPitch/XShiftPitch TEdit) this wave. The first Barcode_Reader gate
//      anywhere in this tree deserves its own deliberate wave, not a side
//      effect of this one.
//    FormShortCut -- needs golden `TWMKey &Msg` (Windows-message-specific VCL
//      type); `Grep "TWMKey" .` tree-wide is 0 hits. Genuine vclcompat gap,
//      NOT a dropped-unused-parameter case (`Msg.CharCode` IS read against
//      VK_F5) -- cross-file/infra gap, not this file's to fix.
//    CoSocketComboChange -- only touches `MyTempRGBox[iSnSocketCnt]`, the
//      dynamically-`new`'d TRadioGroup array built in the golden ctor
//      (cSetUp.cpp:201-215). The ctor itself is NOT translated this wave
//      (form-ctor risk, fw-wave-loop skill trap #4: ctors allocate widgets
//      and wire OnClick handlers, and this one also sets `ScrollBar1->Max`
//      per MachineTypeChoice and builds the whole TestSiteCH[][] grid) --
//      with no ctor, there is no array for this method to touch.
//    The remaining 32 methods (ScrollBar1Change/CompChange/CHSetError/
//    FormShow/ReadUseSuckModeFile/CheckSTMMode/ReadFile/DoIniDataToForm/
//    FormClose/XPitchMouseDown/rgUseSuckModeClick/sbtExitClick/sbUpdateClick/
//    SaveSetupFile/btnLUpToRDownNClick/DoPassword/cbEnableRealTimeCCDClick/
//    cbAaChange/cbAaDropDown/rgShtModeNormalMouseDown/cbOctal12SiteClick/
//    chkOffCenterkitClick/cbQualSite2X2ShiftClick/DoPasswordCheck/
//    CheckShuttlePitch/btAutoShuttlePitchClick/XShiftPitchMouseDown/
//    CoSocketComboChange/cbI21Click/FormShortCut/FormDestroy/TfSetup ctor) --
//    47 total - 15 translated = 32 -- (b) write-path / (c) safety-interlock /
//    MIXED / cross-file-gap-blocked, per the classification table in the
//    agent's hand-off report.
//
//  GOLDEN ODDITY -- RadioButton1KeyDown (golden cSetUp.cpp:4443-4449) is
//  declared in cSetUp.h:224-225 and defined in cSetUp.cpp, but
//  `Grep "OnKeyDown = RadioButton1KeyDown" cSetUp.dfm` (cp950-decoded,
//  20260820) is 0 hits -- no widget in the CURRENT .dfm wires this handler to
//  anything (no "RadioButton1" widget even exists in the .dfm; only
//  `RadioButton6` does, per cSetUp.h:18). It is translated anyway, faithfully
//  and unmodified, because the translation discipline is "照翻，並在//AI
//  註解寫下它為什麼看起來錯" -- silently dropping it would be an undocumented
//  behaviour decision, not a neutral cleanup. Retiring it is the user's call,
//  not this pass's.
// =============================================================================
// =============================================================================
//  AI(W906-FW-SETUP-B) 20260821: cSetUp Wave B -- MainTempMode.cpp's two
//  SAFETY GATEs (dep-fSetup-ReadUseSuckModeFile, golden :21886/:21893) named
//  ReadUseSuckModeFile as the missing piece. This wave audits ReadUseSuckModeFile
//  (golden cSetUp.cpp:2133-2146) plus the full 47-method table, then translates
//  3 more (a) 顯示/讀取側 methods on top of Wave A's 15 -- now 18/47. Full
//  47-row classification table is in this wave's hand-off report (not
//  reproduced here -- same "avoid a second copy drifting" rule Wave A's own
//  banner already states); queued-method reasons for the other 29 are also
//  there.
//
//  KEY DISCOVERY THIS WAVE -- A LINK-LAYER WALL, NOT A MISSING-CODE GAP:
//  ReadUseSuckModeFile/CheckSTMMode/cbI21Click all need REAL bodies for
//  InArmSuck/OutArmSuck/MOT[]/ResetShuttleWhichKit (aHotPlateSubstrate.h),
//  IndexHasIC/ShuttleHasIC (csystem.h), CheckSTMMode_2x6_8/CheckSTMMode_2x8_8/
//  CheckCloseSiteMode_1x3_14/_2x3N_14/_1x4_4 (ainarm9045_2x6_8.h/2x8_8.h/
//  1x4_4.h/1x3_2_14.h), and ReadIniData/GetRecipeFileName (common.h) -- ALL of
//  which live in the `ht9045_sm` CMake target. forms/fSetup.cpp (THIS file's
//  sibling .cpp, where Wave A's 15 methods live) is compiled into
//  `ht9045_forms`, which CMakeLists.txt:706 declares links ONLY `vclcompat
//  ht9045_globals` -- and CMakeLists.txt:589-593 documents that adding
//  `ht9045_forms PUBLIC ht9045_sm` was TRIED and FAILS TO CONFIGURE (a real
//  cycle: ht9045_sm already PUBLIC-links ht9045_forms, CMakeLists.txt:2216).
//  So these 3 methods' BODIES do NOT live in forms/fSetup.cpp -- they live in
//  a NEW root file `cSetUp.cpp` (this tree's root, NOT forms/), which this
//  wave adds and which mirrors the ALREADY-ESTABLISHED cTemperFrom.cpp /
//  MainTempMode.cpp split (root .cpp for a form facade's ht9045_sm-dependent
//  methods; forms/fXxx.cpp stays on the ht9045_globals-only diet). `cSetUp.cpp`
//  is written to belong to `ht9045_sm` (same target as cTemperFrom.cpp/
//  MainTempMode.cpp, CMakeLists.txt's `add_library(ht9045_sm ...)` block,
//  ~line 1338) -- confirmed by symbol provenance: aHotPlateSubstrate.cpp/
//  csystem.cpp/ainarm2.cpp/canary_support.cpp (ShowMyMessage) are ALL already
//  IN that target, and ht9045_sm reaches ReadIniData/GetRecipeFileName
//  (common.cpp, `ht9045_core`) transitively via its own declared PUBLIC edge
//  to `ht9045_secsgem` (CMakeLists.txt:2209), which itself PUBLIC-links
//  `ht9045_core` (CMakeLists.txt:1165) -- the exact path csystem.cpp's own 59
//  ReadIniData/GetRecipeFileName call sites already rely on. THIS FILE gains
//  NO new #include for any of the above -- only the 3 method declarations
//  (void, no parameters) and the `cbI21` widget field, none of which need the
//  heavy headers in a class declaration. CMakeLists.txt is OUT OF THIS WAVE'S
//  EDIT BOUNDARY -- ⚠ AI(W906-FW-SETUP-W19) 20260826: 這件交接**已經做掉了**，
//  `cSetUp.cpp` 現在在 CMakeLists.txt:1930 的 `ht9045_sm` 來源清單裡。
//  下面原文保留為沿革：`cSetUp.cpp` is NOT YET added to `ht9045_sm`'s source
//  list; see the hand-off report for the exact one-line insertion point.
//
//  PRE-EXISTING RISK DISCLOSED (NOT introduced or fixed this wave): Wave A's
//  own XPitchKeyPress (forms/fSetup.cpp) calls `OnlyNumberAndDotInPut`, whose
//  real body is common.cpp (`ht9045_core`) -- but `ht9045_forms` (XPitchKeyPress's
//  own target) does not declare `ht9045_core` as a link dependency by any
//  path. This compiles fine (declaration-only from common.h) and evidently
//  links today (Wave A shipped), so `ht9045_core` must already be reaching
//  `ht9045_forms`-linking executables some other way in practice -- but it is
//  an UNDECLARED edge of the exact shape CMakeLists.txt:614-646 already
//  documents fixing once (the AGV_predicates incident). Flagged for the main
//  loop; not this wave's boundary to fix (would require editing CMakeLists.txt).
// =============================================================================
// =============================================================================
//  AI(W906-FW-SETUP-D) 20260824: cSetUp Wave D -- the ctor-array / TScrollBar
//  unlock wave. Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cSetUp.cpp
//  (4,858 lines, cp950, decoded `open(p,'rb').read().decode('cp950')`, 0
//  U+FFFD, 20260824) + cSetUp.h (295 lines). Wave A/B banners above left
//  VERBATIM (append-only).
//
//  WHAT THIS WAVE ADDS (bodies all in root cSetUp.cpp, ht9045_sm -- same
//  link-layer split Wave B's banner documents; forms/fSetup.cpp untouched):
//    Init()                golden ctor :123-216 (94L) -- the dynamic control-
//        array construction (TestSiteCH[][]/TestLabCol[]/TestLabRow[]/
//        iTestSiteCh[][]/MyTempRGBox[], all file-scope globals in cSetUp.cpp
//        :49-57, ZERO users outside cSetUp.cpp in golden -- walked all golden
//        .cpp/.h 20260824), ScrollBar1->Max selection, tSiteMap seeding,
//        rgInOutArmYPitch item swap, MyTempRGBox[] build. Placed in an
//        EXPLICIT Init(), NOT the ctor, per the established fTemp_Set.h:483
//        "ctor only fields, real logic -> explicit Init()" convention (the
//        static-initialization-order trap: forms/fSetup.cpp:21 news the
//        global fSetup at static-init time, and this body reads
//        MachineTypeChoice/USE_IN_OUT_ARM_Y_PITCH -- other TUs' globals).
//        NOT CALLED BY ANYTHING YET (same posture as TfTemp_Set::Init before
//        wb_serve wired it) -- callers of the array-dependent methods below
//        must run Init() first; golden got this ordering for free from the
//        VCL ctor.
//    FormDestroy()         golden :218-230 (13L) -- tSiteMap teardown
//    CompChange(int)       golden :1195-1262 (68L) -- site-CH combo rebuild
//    CHSetError()          golden :1284-1605 (322L) -- site-map/pitch validator
//    rgUseSuckModeClick()  golden :3423-3450 (28L) -- suck-mode widget cascade
//    CheckShuttlePitch()   golden :4645-4706 (62L) -- auto-shuttle-sensor guard
//    CoSocketComboChange() golden :4752-4770 (19L) -- MyTempRGBox[] show/hide
//  Now 25/47 TfSetup:: methods have real bodies (15 Wave A + 3 Wave B + 7
//  here, counting Init() as golden's ctor). The other 22 stay queued -- see
//  cSetUp.cpp's file-head GATE REGISTER / QUEUE LEDGER (single copy, per the
//  no-second-drifting-copy rule).
//
//  NEW FACADE SURFACE THIS WAVE (all verified against golden cSetUp.h line
//  numbers cited inline below): ScrollBar1 (vclcompat::TScrollBar -- NEW
//  vclcompat/ScrollBar.h, headless field bag per user ruling 20260824; NOT
//  global-using'd, handlerlog.h:122 owns global ::TScrollBar and
//  handlerlog.cpp includes this header), the 32 site-CH combos cbAa..cbDh,
//  the 12 site labels labColA-H/labRowA-D, rgInOutArmYPitch,
//  rgSelectSearchLast, cbOctal12Site, XPitch/YPitch, CoSocketCombo,
//  scrlbxSocketSensor, and data members bSiteMapHasChange/iTestMode/
//  iTestModeOcr/tSiteMap.
//
//  DEVIATIONS (each also marked inline at its site):
//    (D-1) vclcompat::TRadioGroup::Items defaults NULL (Controls.h's own
//          documented verbatim-kept default); real VCL allocates Items in
//          the TRadioGroup ctor. Init() therefore allocates
//          rgInOutArmYPitch->Items, and TfSetupSensorRadioGroup's ctor
//          allocates its own Items, before golden's Clear()/Add() lines run
//          -- the rgCustomerList "facade has no dfm/VCL resource stream ->
//          hand-seed, record DEVIATION" precedent.
//    (D-2) golden `new TRadioGroup(this)` -> `new TfSetupSensorRadioGroup()`:
//          Owner arg dropped (VCL glue, established convention), type
//          substituted (S18 "keep the golden expression, substitute the
//          type" idiom, forms/fBinSel.h) so ->Name/->Caption/->Height/
//          ->Columns/->Parent stay spellable.
//    (D-3) golden ctor param `TComponent* Owner` and the `: TForm(Owner)`
//          base call dropped (VCL glue; Init() is not a ctor).
//  GATES inside Init() (cited to forms/fTemp_Set.h's established register):
//    GATE(G-Align)    MyTempRGBox[i]->Align=alTop (no Align port, by design)
//    GATE(G-Delegate) MyTempRGBox[i]->OnClick=rgSensor1Click (stock widgets
//                     carry no OnClick delegate slot; the TScrollBar
//                     OnChange/OnScroll slots are the user-ruled exception
//                     and live on vclcompat::TScrollBar only)
// =============================================================================
#ifndef FORMS_FSETUP_H
#define FORMS_FSETUP_H

// Dependency-free on purpose: the single member is a bool.  See forms/fHome.h
// for the same reasoning.

#include "cmydef.h"              // TestSiteFileName[][]/TotalTestMode (GetTestMode)
#include "cprod.h"               // TestIF_File.dSiteYOffset (rgYPitchOffsetModeClick)
#include "Config.h"              // IniConfig.bD30EnableSiteModeSelect (Arm1PickArm2TestClick)
#include "common.h"              // OnlyNumberAndDotInPut (XPitchKeyPress)
#include "vclcompat/Controls.h"  // TEdit/TCheckBox/TRadioGroup/TRadioButton/TGroupBox/TLabel
#include "vclcompat/SysUtils.h"  // FormatFloat (rgYPitchOffsetModeClick)
#include "vclcompat/ScrollBar.h" // AI(W906-FW-SETUP-D) 20260824: vclcompat::TScrollBar (ScrollBar1) -- NOT global-using'd, see that header's NAME COLLISION note (handlerlog.h:122)
#include "vclcompat/ShiftState.h"   // AI(W906-FW-SETUP-W19) 20260826: rgShtModeNormalMouseDown 的簽章

using vclcompat::TEdit;
using vclcompat::TCheckBox;
using vclcompat::TRadioGroup;
using vclcompat::TRadioButton;
using vclcompat::TGroupBox;
using vclcompat::TLabel;

// ===========================================================================
//  TfSetupLabel -- facade-only subclass, same idiom as forms/fObserver.h's
//  TfObserverGrid / forms/fConfiguration.h's TfConfigurationGrid: publicly
//  inherit the shared vclcompat type (so it still converts for free to any
//  `vclcompat::TLabel*` parameter) and add ONLY the member golden touches
//  that the shared header does not carry. vclcompat::TLabel's ported surface
//  is Caption/Color (+ inherited Visible/Enabled) -- see vclcompat/
//  Controls.h's own file-head note -- but rgYPitchOffsetModeClick (golden
//  :4784/:4791/:4798) writes `labYOffset->Width=`. NOT a GATE: the value
//  compiles and holds exactly what golden assigns; only the RENDERING a real
//  Width would imply (actually resizing an on-screen label) is out of scope,
//  same "no renderer this wave" posture every other headless facade widget
//  in this tree already carries. Shared vclcompat/Controls.h is NOT edited
//  (outside this wave's write boundary; other TUs depend on its exact shape).
// ===========================================================================
class TfSetupLabel : public vclcompat::TLabel
{
public:
    int Width = 0;   // golden TLabel->Width -- vclcompat::TLabel carries no Width
};

// ===========================================================================
//  AI(W906-FW-SETUP-D) 20260824: two facade-local helper types for Init()'s
//  MyTempRGBox[] build (golden cSetUp.cpp:201-215). Same "compose, don't
//  fork the shared vclcompat type" idiom as TfSetupLabel above /
//  forms/fTemp_Set.h's TfTemp_SetTagEdit.
// ===========================================================================

// golden cSetUp.h:84 `TScrollBox *scrlbxSocketSensor;` -- `class TScrollBox`
// has ZERO port anywhere in this tree (grep "class TScrollBox" *.h/*.cpp
// tree-wide, 20260824: only comment/gated hits -- forms/fBinSel.h's is inside
// its gated widget half). Its ONLY ported use is as the MyTempRGBox[i]->Parent
// target (a write; nothing reads it back), so a minimal TControl-derived
// stand-in carries the golden member without inventing scroll surface.
class TfSetupScrollBox : public vclcompat::TControl
{
};

// golden's ctor-built sensor radio groups (`new TRadioGroup(this)`, golden
// cSetUp.cpp:203). vclcompat::TRadioGroup carries only ItemIndex/Items
// (+TControl's Visible/Enabled); golden's build loop also writes Parent/Name/
// Height/Caption/Columns -- plain data here so those lines port verbatim.
// Align=alTop / OnClick=rgSensor1Click are GATED at the call site instead
// (G-Align / G-Delegate, forms/fTemp_Set.h's established register) -- neither
// property belongs on a facade data bag.
class TfSetupSensorRadioGroup : public vclcompat::TRadioGroup
{
public:
    vclcompat::TControl *Parent = nullptr;  // golden TWinControl::Parent -- write-only in golden cSetUp.cpp (only :204 assigns; nothing reads it back)
    AnsiString Name;                        // golden TComponent::Name ("rgSensor<i+1>")
    AnsiString Caption;                     // golden TRadioGroup Caption ("Sensor %d usage")
    int Height = 0;                         // golden ->Height (36 at build time)
    int Columns = 0;                        // golden TRadioGroup->Columns (3 at build time)

    // DEVIATION (D-1, see file-head banner): real VCL TRadioGroup allocates
    // Items in its ctor; the shared vclcompat base deliberately left it NULL,
    // so this facade type allocated and freed its own.
    // AI(W906-FW3-Observer-W2) 20260825: D-1 RETIRED here too. The base
    // (Controls.h:431-437) now allocates and frees Items itself, so the
    // ctor's `Items = new ...` would leak the base's list and the dtor's
    // `delete Items` would run twice over. Both removed; the inherited pair
    // does exactly what this one did.
};

class TfSetup
{
public:
    // -----------------------------------------------------------------------
    //  [DATA] golden csetup.h:261 `bool fShow;` -- "the Setup dialog is open",
    //  i.e. the operator is editing test-mode / site-map / shuttle-mode setup.
    //  Golden's two writers are csetup.cpp:1850 `fShow=true;` (the form Show
    //  path, commented "must be BEFORE ReadFile()") and csetup.cpp:3358
    //  `fShow=false;` -- both pure VCL form lifecycle with no ported home, so
    //  offline this stays at its constructed value permanently.
    //
    //  OFFLINE VALUE false, GOLDEN'S OWN CTOR VALUE (csetup.cpp:163
    //  `fShow=false;` inside `__fastcall TfSetup::TfSetup(TComponent* Owner)`
    //  at csetup.cpp:123).
    //
    //  THE BRANCH false SELECTS -- this is the whole reason the field exists,
    //  and the W7-L2 brief flagged it as recon risk #3:
    //    golden ckernel.cpp:411 `if(iHome==0 && fSetup->fShow==false)`
    //    false OPENS the arm at :413-439 -- the START-time motor-home check.
    //    With the arm open and fAllMotorHome true, ScanSystemSensor runs the
    //    four-way home ladder:
    //        :415 bNeedArmZHome            -> InitDoArmZHome()
    //        :419 FrmRotate->bRotateInHome -> FrmRotate->InitialInRotateHome()
    //        :423 FrmRotate->bRotateOutHome-> FrmRotate->InitialOutRotateHome()
    //        :427 bYpitchNeddHome          -> bYpitchNeddHome=true
    //        :431 else                     -> if(CheckMotorHome()==false)
    //                                           ShowMyMessage("Must home again")
    //                                           return false;   <-- START REFUSED
    //    So false is emphatically NOT the "inert" choice: it is the choice that
    //    can make ScanSystemSensor RETURN FALSE and refuse to start the machine.
    //    true would have skipped the entire ladder and let START through with no
    //    home verification at all -- golden's intent being "while the operator
    //    has Setup open, do not nag about homing".  false is the running-machine
    //    state and the faithful one, and it is also the state that keeps the
    //    :419/:423 FrmRotate calls (this wave's other facade) reachable at all.
    // -----------------------------------------------------------------------
    bool fShow;

    // =========================================================================
    //  AI(W906-FW3-Setup-WA) 20260820: additions below -- see the file-head
    //  banner for the wave scope, GATE REGISTER, and deferred-methods list.
    //  Everything above this line (fShow, the ctor/dtor declarations) is
    //  UNTOUCHED W7-L2 content.
    // =========================================================================

    // -- golden cSetUp.h:271-285 `typedef struct {...} SetUpShuttleMode;` +
    //    `SetUpShuttleMode shtMode;` (SetShtMode/VertifyShtModeisDiff) --------
    typedef struct
    {
        bool bNormal;
        bool bOneSide;
        bool bUsedSht1;
        bool bUsedSht2;
        void Clear()
        {
            bNormal=false;
            bOneSide=false;
            bUsedSht1=false;
            bUsedSht2=false;
        }
    } SetUpShuttleMode;
    SetUpShuttleMode shtMode;

    // -- golden cSetUp.h:254 `int SitCH;` (RadioButton1KeyDown) -- golden
    //    declares this `private`; the facade has no private section (headless
    //    data-only class, matching forms/fConfiguration.h's convention).
    int SitCH = 0;

    // -- widget fields, one per translated (a) method above (dfm leaf names) --
    TCheckBox   *Arm1PickArm2Test     = new TCheckBox();     // golden cSetUp.h:67, dfm Caption="Enable Function"
    TGroupBox   *gbShuttleMode        = new TGroupBox();     // golden cSetUp.h:20
    TCheckBox   *cbUseSLKClamp        = new TCheckBox();     // golden cSetUp.h:73, dfm Caption="Use Clamp SLK"
    TRadioGroup *rgseparabilityTest   = new TRadioGroup();   // golden cSetUp.h:74, dfm Caption="Motion"
    TRadioGroup *rgYPitchOffsetMode   = new TRadioGroup();   // golden cSetUp.h:90, dfm Caption="Y-Offset"
    TfSetupLabel *labYOffset          = new TfSetupLabel();  // golden cSetUp.h:43, dfm Caption="Y Offset (mm)"
    TEdit       *edYOffset            = new TEdit();         // golden cSetUp.h:49
    TEdit       *edOcrText            = new TEdit();         // golden cSetUp.h:71 -- NOTE: OCRInsp.cpp
                                                              // #define's its OWN TU-local `fSetup` to a
                                                              // different seam object (W906OCR_TfSetupSeam);
                                                              // it never includes this header, so this
                                                              // field and that seam's own edOcrText are two
                                                              // independent objects, not aliases. Verified
                                                              // this wave: `Grep "#include.*forms/fSetup"
                                                              // OCRInsp.cpp` is 0 hits.
    TEdit       *edOverRange          = new TEdit();         // golden cSetUp.h:191
    TEdit       *edDelayTime          = new TEdit();         // golden cSetUp.h:190
    TEdit       *edAuto1Count         = new TEdit();         // golden cSetUp.h:195
    TEdit       *edtGetValueDelayTime = new TEdit();         // golden cSetUp.h:201
    TRadioButton *rgShtModeOneSide    = new TRadioButton();  // golden cSetUp.h:22
    TRadioButton *rgUseSht1           = new TRadioButton();  // golden cSetUp.h:24
    TRadioButton *rgUseSht2           = new TRadioButton();  // golden cSetUp.h:25
    TRadioGroup  *rgUseSuckMode       = new TRadioGroup();   // golden cSetUp.h:36

    // -- AI(W906-FW-SETUP-B) 20260821: golden cSetUp.h:93 `TCheckBox *cbI21;`,
    //    dfm Caption='Enable auto site mapping function' (cSetUp.dfm, decoded
    //    cp950, 20260821) -- cbI21Click's only widget dependency.
    TCheckBox   *cbI21                = new TCheckBox();     // golden cSetUp.h:93

    // -- 15 (a) methods, see file-head banner for golden spans/classification --
    int  GetTestMode(AnsiString sTestMode);
    // AI(W906-FW-SETUP-W19) 20260826: golden cSetUp.h:21-22 的兩顆單選鈕，
    // rgShtModeNormalMouseDown 會依 TestIF_File.iShuttleMode 勾選其中一顆。
    // 本樹沒有 .dfm 載入路徑，Checked 的初值由 NSDMI 決定（false）。
    TRadioButton *rgShtModeNormal  = new TRadioButton();   // golden cSetUp.h:21
    // （rgShtModeOneSide 早已存在於本 header，rgShtModeNormalClick 在用它）
    void rgShtModeNormalClick();
    void SetShtMode(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2);
    bool VertifyShtModeisDiff(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2);
    void XPitchKeyPress(char &Key);
    void RadioButton1KeyDown(TRadioButton *Sender);
    void rgSensor1Click(TRadioGroup *Sender);
    void Arm1PickArm2TestClick();
    void cbUseSLKClampClick();
    void rgYPitchOffsetModeClick();
    void edOcrTextMouseDown();
    void edOverRangeClick();
    void edDelayTimeClick();
    void edAuto1CountClick();
    void edtGetValueDelayTimeClick();

    // -- AI(W906-FW-SETUP-B) 20260821: 3 more (a) methods, Wave B. UNLIKE the
    //    15 above, these 3 BODIES live in the root `cSetUp.cpp` (ht9045_sm),
    //    NOT forms/fSetup.cpp (ht9045_forms) -- see this file's Wave B banner
    //    for the link-layer reason. Declared here exactly like any other
    //    member; callers (ckernel.cpp/Command.cpp/handlerlog.cpp/
    //    MainTempMode.cpp) do not need to know or care which .cpp defines them.
    void ReadUseSuckModeFile();   // golden cSetUp.cpp:2133-2146 (14L)
    void CheckSTMMode();          // golden cSetUp.cpp:2148-2177 (30L)
    void cbI21Click();            // golden cSetUp.cpp:4804-4828 (25L)

    // =========================================================================
    //  AI(W906-FW-SETUP-D) 20260824: Wave D additions -- see this file's
    //  Wave D banner for scope/DEVIATIONS/GATES. Bodies in root cSetUp.cpp
    //  (ht9045_sm), same split as Wave B's 3 methods above.
    // =========================================================================

    // -- data members (golden cSetUp.h line cited each; NSDMI value = golden
    //    ctor value, since forms/fSetup.cpp's ctor is out of this wave's edit
    //    boundary and Init() re-asserts each verbatim anyway) ----------------
    bool bSiteMapHasChange = true;          // golden cSetUp.h:289, ctor :126 sets true
    int  iTestMode = TotalTestMode;         // golden cSetUp.h:290, ctor :127 sets TotalTestMode
    int  iTestModeOcr = -1;                 // golden cSetUp.h:252 (private there; facade is flat, same as SitCH above), ctor :183 sets -1
    TStringList *tSiteMap = nullptr;        // golden cSetUp.h:266 -- new'd + "0"-seeded by Init() (golden ctor :185-192); nullptr until Init() runs (golden could not be observed pre-ctor)

    // -- widget fields (dfm leaf names; golden cSetUp.h line cited each) -----
    vclcompat::TScrollBar *ScrollBar1 = new vclcompat::TScrollBar();  // golden cSetUp.h:44 -- fully qualified on purpose (no global using; handlerlog.h:122)
    TRadioGroup *rgInOutArmYPitch = new TRadioGroup();   // golden cSetUp.h:48 -- Items allocated by Init() (DEVIATION D-1)
    TRadioGroup *rgSelectSearchLast = new TRadioGroup(); // golden cSetUp.h:37
    TCheckBox   *cbOctal12Site = new TCheckBox();        // golden cSetUp.h:33
    TEdit       *XPitch = new TEdit();                   // golden cSetUp.h:46
    TEdit       *YPitch = new TEdit();                   // golden cSetUp.h:47
    TComboBox   *CoSocketCombo = new TComboBox();        // golden cSetUp.h:88
    TfSetupScrollBox *scrlbxSocketSensor = new TfSetupScrollBox();  // golden cSetUp.h:84 (TScrollBox -- see helper-class note)

    // the 4x8 site-CH combo grid (golden cSetUp.h:114-145) + its labels
    // (:96-113) -- Init() wires these into the file-scope TestSiteCH[][] /
    // TestLabCol[] / TestLabRow[] arrays in cSetUp.cpp, exactly as golden's
    // ctor does.
    TComboBox *cbAa = new TComboBox(); TComboBox *cbAb = new TComboBox();
    TComboBox *cbAc = new TComboBox(); TComboBox *cbAd = new TComboBox();
    TComboBox *cbAe = new TComboBox(); TComboBox *cbAf = new TComboBox();
    TComboBox *cbAg = new TComboBox(); TComboBox *cbAh = new TComboBox();
    TComboBox *cbBa = new TComboBox(); TComboBox *cbBb = new TComboBox();
    TComboBox *cbBc = new TComboBox(); TComboBox *cbBd = new TComboBox();
    TComboBox *cbBe = new TComboBox(); TComboBox *cbBf = new TComboBox();
    TComboBox *cbBg = new TComboBox(); TComboBox *cbBh = new TComboBox();
    TComboBox *cbCa = new TComboBox(); TComboBox *cbCb = new TComboBox();
    TComboBox *cbCc = new TComboBox(); TComboBox *cbCd = new TComboBox();
    TComboBox *cbCe = new TComboBox(); TComboBox *cbCf = new TComboBox();
    TComboBox *cbCg = new TComboBox(); TComboBox *cbCh = new TComboBox();
    TComboBox *cbDa = new TComboBox(); TComboBox *cbDb = new TComboBox();
    TComboBox *cbDc = new TComboBox(); TComboBox *cbDd = new TComboBox();
    TComboBox *cbDe = new TComboBox(); TComboBox *cbDf = new TComboBox();
    TComboBox *cbDg = new TComboBox(); TComboBox *cbDh = new TComboBox();
    TLabel *labColA = new TLabel(); TLabel *labColB = new TLabel();
    TLabel *labColC = new TLabel(); TLabel *labColD = new TLabel();
    TLabel *labColE = new TLabel(); TLabel *labColF = new TLabel();
    TLabel *labColG = new TLabel(); TLabel *labColH = new TLabel();
    TLabel *labRowA = new TLabel(); TLabel *labRowB = new TLabel();
    TLabel *labRowC = new TLabel(); TLabel *labRowD = new TLabel();

    // -- Wave D methods (golden spans; classification in cSetUp.cpp's ledger).
    //    Init() carries golden's ctor body :123-216 (DEVIATION D-3: not a
    //    ctor here -- fTemp_Set.h:483 convention; MUST run before any of
    //    FormDestroy/CompChange/CHSetError/CoSocketComboChange below touch
    //    the arrays/tSiteMap it builds).
    void Init();                  // golden cSetUp.cpp:123-216 (94L, the ctor)
    void FormDestroy();           // golden cSetUp.cpp:218-230 (13L)
    void CompChange(int iMode);   // golden cSetUp.cpp:1195-1262 (68L)
    bool CHSetError();            // golden cSetUp.cpp:1284-1605 (322L)
    void rgUseSuckModeClick();    // golden cSetUp.cpp:3423-3450 (28L)
    bool CheckShuttlePitch();     // golden cSetUp.cpp:4645-4706 (62L)
    void CoSocketComboChange();   // golden cSetUp.cpp:4752-4770 (19L)
    // AI(W906-FW-SETUP-E) 20260824: the fQwertyKey/fPassword unlock batch
    // (bodies in cSetUp.cpp tail; Sender typed per the D-3 convention).
    void XPitchMouseDown(TEdit *Sender);       // golden cSetUp.cpp:3409-3421 (13L)
    void XShiftPitchMouseDown(TEdit *Sender);
    //AI(W906-FW-SETUP-W19) 20260826: 唯一的 deferral 理由（Barcode_Reader 無 port）已消失。
    void rgShtModeNormalMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);   // golden :4451-4462  // golden cSetUp.cpp:4738-4750 (13L)
    void cbAaDropDown(TComboBox *Sender);      // golden cSetUp.cpp:4408-4441 (34L)

    TfSetup();
    virtual ~TfSetup() {}
};

// golden: extern PACKAGE TfSetup *fSetup;   (csetup.h:293)
extern TfSetup *fSetup;

#endif // FORMS_FSETUP_H
