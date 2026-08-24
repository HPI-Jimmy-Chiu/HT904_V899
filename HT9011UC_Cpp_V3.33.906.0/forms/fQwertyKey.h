// =============================================================================
//  forms/fQwertyKey.h  --  non-VCL facade for golden's TfQwertyKey
//  (myQwertyKeyBoard.h/.cpp) -- the on-screen QWERTY / numeric keyboard form
//
//  AI(W906-FW-QWKEY1) 20260824: new file, FW-3 wave FW-QWKEY1.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/myQwertyKeyBoard.h (164
//  lines) + myQwertyKeyBoard.cpp (539 lines) + myQwertyKeyBoard.dfm (2,527
//  lines) -- all cp950, decoded with `python -c
//  "open(p,'rb').read().decode('cp950')"`, 0 U+FFFD, this wave (20260824).
//
//  ROLE
//  ----
//  TfQwertyKey is golden's modal on-screen keyboard: a QWERTY panel
//  (palQwertyKey, 48 TQwertyKeyClass-mapped TSpeedButtons), a numeric pad
//  (palNumKey, 10 digit keys + +/-N step keys + DP/percent/minus), and a
//  current/max/min value strip (palValueLimit).  Every other form's
//  `ed*MouseDown -> fQwertyKey->ShowQwertyKey(...)` call funnels here.  THIS
//  WAVE LANDS THE FIRST COMPILED TfQwertyKey ANYWHERE IN THE PORT TREE -- the
//  class every WA-1-family GATE across the tree has been waiting on
//  (forms/fSetup.h:96-115, forms/fConfiguration.h:78-112, forms/
//  fLotInfo.h:172-178, forms/fDynamicTemp.h GATE (Q1), ATC/ATCInterface.h:
//  96-103, EJ1N/MyOmronPanel.h:77-124, Public/HTEdit.cpp GATE (6), ...).
//  Those gates are in files outside this wave's write boundary and stay
//  CLOSED; un-gating them is the next wave's wiring decision, now unblocked.
//
//  THIS IS A NEW CLASS -- NO EXISTING STAND-IN, NO ODR CONFLICT
//  --------------------------------------------------------------------------
//  Re-verified this wave (20260824 11:25-12:00):
//    * `Grep "TfQwertyKey|fQwertyKey" --glob *.{h,cpp}` tree-wide -- every hit
//      is either a GATE-REGISTER comment, an `#if 0` block (Public/
//      HTEdit.cpp:298-353 GATE (6) holds the only textual
//      `Application->CreateForm(__classid(TfQwertyKey),...)` sites), or a
//      tools/dfm2rc data artifact.  0 compiled declarations.
//    * `Grep "eKeyType|TQwertyKeyClass" --glob *.{h,cpp}` -- 0 hits anywhere
//      (this header's enum/helper-class names are unclaimed).
//  Both greps must be re-run at wave close (absence claims go stale under
//  parallel waves -- docs/KNOWLEDGE.md).
//
//  FORM CLASS SHAPE: plain non-VCL class, NOT TForm-derived -- same posture
//  as every translated form facade in this tree (forms/fSecurity.h's own
//  survey: none re-derive TForm).  Widget stand-ins store data, they do not
//  render -- "logic faithful, rendering = web".
//
//  CTOR / Init() SPLIT (task-mandated, fTemp_Set.h:483 / cSetUp.cpp Init()
//  convention)
//  --------------------------------------------------------------------------
//  The ctor stuffs FIELDS ONLY: BCB6-TObject-zero-fill equivalents
//  (ZeroInitVclFields semantics -- golden never sets bIntegerOnly/KeyCode in
//  its ctor and relies on TObject's zero-fill; plain C++ has none, so the
//  defaults below reproduce it) plus the .dfm-sourced geometry/text initial
//  values (fBinSel.h "dims from dfm into ctor" precedent).  Golden's ENTIRE
//  ctor body (myQwertyKeyBoard.cpp:64-135 -- the 58 TQwertyKeyClass
//  push_backs + the OnClick wiring loop + the scalar re-sets) moves VERBATIM
//  into explicit Init(); nothing constructs either global at static-init
//  time (both stay NULL pointers, golden's own zero state), so there is no
//  SIOF exposure at all.
//
//  WAVE SCOPE -- ALL 20 golden methods (19 TfQwertyKey:: + 1
//  TQwertyKeyClass::), every one read line-by-line in full.  Denominator: 20
//  of 20 distinct methods in golden myQwertyKeyBoard.cpp (verified this
//  wave: 19 `TfQwertyKey::` definitions + 1 `TQwertyKeyClass::` -- the whole
//  file).  Golden ctor counts as Init() below.
//    TQwertyKeyClass ctor       golden :56-62   ACTIVE
//    TfQwertyKey ctor           (PORT split)    fields only, see above
//    Init()                     golden :64-135  ACTIVE except GATE (Q-D1)
//    FormShow                   golden :137-143 ACTIVE
//    FormClose                  golden :145-150 ACTIVE
//    FormDestroy                golden :152-167 ACTIVE -- lives in ROOT
//        myQwertyKeyBoard.cpp (ht9045_sm suggestion), NOT forms/
//        fQwertyKey.cpp: it calls MyDBIProcess (real decl
//        aHotPlateSubstrate.h:933, body aHotPlateSubstrate.cpp -- ht9045_sm)
//        and LogSoftwareOffTime (decl cmydef.h:5032, offline no-op body
//        acarry_shims.cpp:255 -- ht9045_sm), both OUTSIDE the ht9045_forms
//        link diet (vclcompat+ht9045_globals+ht9045_core, CMakeLists.txt:
//        706-708).  Same root-file split cSetUp.cpp/cTemperFrom.cpp
//        established, and the same two symbols TfSetup::FormDestroy
//        (cSetUp.cpp:538) already resolves this way.
//    ShowQwertyKey              golden :169-302 ACTIVE (see BEHAVIOUR NOTE)
//    spbChangeCaseClick         golden :304-318 ACTIVE
//    spbKeyClick                golden :320-339 ACTIVE
//    spbBackSpaceClick          golden :341-345 ACTIVE
//    spbSummitClick             golden :347-350 ACTIVE
//    spbClearClick              golden :352-355 ACTIVE
//    FormResize                 golden :357-360 ACTIVE
//    spbCancelClick             golden :362-366 ACTIVE
//    spbPercentClick            golden :368-377 ACTIVE
//    ChangeDecimalPoint         golden :379-419 ACTIVE
//    spbMinusClick              golden :421-431 ACTIVE
//    spbAdd1Click               golden :433-449 ACTIVE
//    spbDPClick                 golden :451-458 ACTIVE
//    edQwertyContentKeyDown     golden :460-467 ACTIVE
//    edQwertyContentKeyPress    golden :469-538 ACTIVE
//  Total: 19 ACTIVE + 1 ACTIVE-in-root = 20/20 methods, 1 narrow gate.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (Q-D1) `QwertyKey[i]->spbKey->OnClick=spbKeyClick;` -- Init(), golden
//      :131-134 (the whole 4-line wiring loop).  Stock vclcompat widgets
//      carry no OnClick delegate slot (cSetUp.cpp GATE(G-Delegate), the
//      established tree-wide idiom, verbatim shape reused here).  Handler
//      BODIES are translated in full; wiring is the web write-path layer's
//      job ("event handler 本體翻譯但不接線", this wave's task instruction) --
//      the future web layer calls fQwertyKey->spbKeyClick(btn) directly.
//
//  BEHAVIOUR NOTE -- ShowModal()/Close() are offline no-ops
//  --------------------------------------------------------------------------
//  Golden's `this->ShowModal();` (ShowQwertyKey, :283) blocks in a modal
//  message loop while the operator types; `Close()` (spbSummitClick /
//  spbCancelClick / edQwertyContentKeyDown) ends it.  Headless there is no
//  window and no loop: both are permanent no-op members (forms/fTemp_Set.h:
//  575-584 Close() precedent, extended to ShowModal()).  Net offline
//  semantics of a ShowQwertyKey call = "operator opened the keyboard and
//  immediately submitted the unchanged text": the post-modal tail runs at
//  once, so with bCheckRange the atof->CheckRange->AnsiString(double)
//  round-trip MAY REWRITE the target control's text (e.g. Caption "abc" +
//  N_INTEGER -> "0") -- that is golden's own submit path applied to the
//  unedited value, not an invention of this port.  Similarly
//  `spbChangeCase->Click();` (:223) dispatches to the wired handler in
//  golden; vclcompat::TControl::Click() is the documented offline no-op
//  (Controls.h:222, acatchtray.cpp's ->Click() sites are the compiled
//  precedent), so offline the QWERTY captions are not refreshed until the
//  web layer calls spbChangeCaseClick() itself.  Kept verbatim.
//
//  GOLDEN NOTES (faithful, recorded for the ledger)
//  --------------------------------------------------------------------------
//  (G-a) ShowQwertyKey golden :290: `CheckRange(d, min, max)` -- CheckRange's
//      declared parameter order is (Value, Maximum, Minimum)
//      (MachineType.h:1525), so golden passes min INTO Maximum and max INTO
//      Minimum.  NOT a behaviour bug: CheckRange's own Maximum<Minimum
//      branch handles swapped bounds symmetrically -- but any future reader
//      diffing parameter names will trip on it.  Translated verbatim.
//  (G-b) spbChangeCaseClick golden :307 loops `i<47` over a 58-entry
//      QwertyKey vector: entries 47 (space) and 48-57 (numeric pad) are
//      deliberately excluded from case/symbol toggling (their captions are
//      case-invariant).  Verbatim.
//  (G-c) Init() golden :79-82/:93-94 (Steven 20210805): digits 9/0 and
//      -/=/[/] were reclassified eKeyNumAndSymbol -> eKeyalphabet, so
//      N_NO_SYMBOL no longer disables or uppercase-forces them.  Verbatim.
//  (G-d) ShowQwertyKey golden :171-178: the fQwertyKey/fQwertyKey2
//      double-keyboard delegation (Steven 20150505 "剛好在輸入時,Alarm會卡死")
//      dereferences BOTH globals from inside a member function; golden
//      guarantees both were CreateForm'd first (HTEdit.cpp:302-305).  In
//      this tree both globals stay NULL until a wiring wave constructs them
//      -- any call before that is a NULL deref, same exposure golden itself
//      has pre-CreateForm.  Nothing calls ShowQwertyKey today (all call
//      sites still gated).
//
//  DEVIATIONS (port-only adaptations, zero intended behaviour change)
//  --------------------------------------------------------------------------
//  (D-1) `__fastcall` + `TComponent* Owner` ctor glue dropped; class is not
//      TForm-derived (established, see FORM CLASS SHAPE).
//  (D-2) golden ctor body -> explicit Init() (task-mandated; fTemp_Set.h:483
//      / cSetUp.cpp precedent).
//  (D-3) Unread VCL glue parameters dropped per-handler (forms/
//      fConfiguration.h WA-8 / forms/fDynamicTemp.h convention; verified
//      per-function this wave that each dropped parameter is never read):
//      every handler below except spbKeyClick/spbAdd1Click (Sender IS read,
//      cast to TSpeedButton*) and the two edQwertyContent Key handlers (Key
//      IS read/written).  Sender cast target typed directly in the
//      signature, collapsing golden's C-style cast (forms/fSetup.cpp
//      RadioButton1KeyDown precedent); the local `Ptr` is kept line-for-line.
//  (D-4) edQwertyContentKeyDown: golden `WORD &Key` -> `unsigned short &Key`
//      (WORD is exactly unsigned short in <windows.h>; spelled out so this
//      header stays windows.h-free).  KeyPress keeps golden's `char &Key`
//      (forms/fSetup.cpp XPitchKeyPress precedent, including `Key=NULL`).
//  (D-5) ShowQwertyKey parameter: golden `TWinControl *Ptr` -> `TControl
//      *Ptr`.  No TWinControl port exists (grepped Controls.h this wave);
//      vclcompat::TControl is the polymorphic common base of TPanel/TEdit/
//      TLabeledEdit, so the three golden dynamic_casts inside compile and
//      dispatch identically.
//  (D-6) golden private: members (spbKeyClick/ChangeDecimalPoint/
//      bIntegerOnly/KeyCode/sBackup) collapsed to public -- established
//      convention (forms/fTemp_Set.h:566-569 citation chain).
//  (D-7) Form-level Top/Left/Width/Height are plain int fields on the facade
//      (golden TForm properties).  dfm gives ClientWidth=1005/
//      ClientHeight=469 (no Width/Height keys); those client values seed
//      Width/Height here -- ShowQwertyKey overwrites both on every call
//      before any read, so the border-vs-client delta is unreachable.
//  (D-8) `Showing` (bool field): golden TForm::Showing, READ by golden
//      Password.cpp:51 (`if(fQwertyKey->Showing)`).  PORT-ONLY data field --
//      nothing sets it offline (real VCL sets it during Show/Close; the web
//      layer will own it).  Declared because the sibling fPassword facade's
//      FormShow dereferences it by name this same wave.
//
//  EXCLUDED WIDGETS (golden __published members with NO method dereferencing
//  them by name -- forms/fDynamicTemp.h "only what's dereferenced" rule):
//    SpeedButton63/64/65/66 (decorative corner keys), spbBackSpace,
//    spbSummit, spbClear, spbCancel (their OnClick bodies never touch the
//    widget itself), Label1.
//
//  DEPENDENCY AUDIT (this wave, 20260824) -- all grepped before translating:
//    N_INTEGER..N_IP_ADDR       REAL  cmydef.h:289-297 (extern const int)
//    CheckRange<T>              REAL  MachineType.h:1525 (inline template)
//    vec_clr                    REAL  MachineType.h:1610 (inline template)
//    OnlyNumberInPut            REAL  common.h:403 / common.cpp:1435
//    OnlyNumberAndDotInPut      REAL  common.h:404 / common.cpp:1436
//    CUSTOMER_CODE              REAL  cmydef.h:3181;  CC_JCET MachineType.h:333
//    MyDBIProcess(S1,S2)        REAL  aHotPlateSubstrate.h:933 (root file only)
//    LogSoftwareOffTime         REAL  cmydef.h:5032 (root file only)
//    TEdit/TPanel/TLabel/TSpeedButton/TControl  REAL  vclcompat/Controls.h
//    AnsiString(+int/double ctors, Pos/SubString/Length/sprintf/c_str)
//                               REAL  vclcompat/AnsiString.h
//    golden includes MachineDefine.h/MachineType.h/cmydef.h/common.h --
//    carried by the .cpp; golden's <vector> + `using namespace std;` --
//    <vector> kept, the blanket using-directive NARROWED to std::vector
//    (established: forms/fDynamicTemp.h also holds a bare #include <vector>
//    and no using-directive).
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt is outside the
//  write boundary).  SUGGESTED landing, mirroring the fSetup split:
//    forms/fQwertyKey.cpp   -> ht9045_forms source list (CMakeLists.txt:654
//        block, next to forms/fSetup.cpp) -- everything here fits the
//        vclcompat+ht9045_globals+ht9045_core diet, and the fQwertyKey/
//        fQwertyKey2 GLOBALS must live at this layer so the queued
//        ht9045_forms consumers (fSetup/fConfiguration/fLotInfo ed*Click)
//        can un-gate without creating the undeclared forms->sm archive edge
//        CMakeLists.txt:618-626 documents as a past defect.
//    myQwertyKeyBoard.cpp   -> ht9045_sm source list (next to cSetUp.cpp,
//        CMakeLists.txt:1922 area) -- FormDestroy only.
// =============================================================================
#ifndef FORMS_FQWERTYKEY_H
#define FORMS_FQWERTYKEY_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject at global scope
#include "vclcompat/Controls.h"     // TPanel/TEdit/TLabel/TSpeedButton/TControl

#include <vector>

// ---------------------------------------------------------------------------
//  eKeyType -- golden myQwertyKeyBoard.h:16-22, verbatim (global scope in
//  golden; grepped this wave, name unclaimed tree-wide).
// ---------------------------------------------------------------------------
enum eKeyType
{
    eKeyNumAndSymbol=0,
    eKeyNumOnly     =1,
    eKeySymbol      =2,
    eKeyalphabet    =3
};

// ---------------------------------------------------------------------------
//  TQwertyKeyClass -- golden myQwertyKeyBoard.h:24-32, verbatim.  One entry
//  per key: upper/lower caption pair + the TSpeedButton it drives + its
//  filter class.
// ---------------------------------------------------------------------------
class TQwertyKeyClass
{
    public :
        TQwertyKeyClass(AnsiString sUpper, AnsiString sLower, TSpeedButton *SPB, eKeyType Type);
        AnsiString UpperCase;
        AnsiString LowerCase;
        TSpeedButton *spbKey;
        eKeyType     KeyType;
};

// ---------------------------------------------------------------------------
//  TfQwertyKeyPanel -- facade-only wrapper (compose, don't fork vclcompat --
//  forms/fDynamicTemp.h precedent): vclcompat::TPanel carries no geometry
//  and no SetFocus.  palContent (Width READ by FormResize, SetFocus by
//  FormShow) and palValueLimit (Height WRITTEN by ShowQwertyKey).
// ---------------------------------------------------------------------------
class TfQwertyKeyPanel : public vclcompat::TPanel
{
public:
    int Width;
    int Height;

    TfQwertyKeyPanel() : Width(0), Height(0) {}
    void SetFocus() {}   // golden TWinControl::SetFocus -- offline no-op (no HWND)
};

// ---------------------------------------------------------------------------
//  TfQwertyKeyEdit -- facade-only wrapper: vclcompat::TEdit carries only
//  Text/Visible/Enabled.  edQwertyContent needs PasswordChar (read+written,
//  ShowQwertyKey :206/:219/:294), SelStart/SelLength (spbKeyClick/
//  spb*Click cursor moves), Width (FormResize) and SetFocus (FormShow).
// ---------------------------------------------------------------------------
class TfQwertyKeyEdit : public vclcompat::TEdit
{
public:
    char PasswordChar;   // real VCL default #0 = "not a password box"; .dfm sets none
    int  SelStart;
    int  SelLength;
    int  Width;

    TfQwertyKeyEdit() : PasswordChar(0), SelStart(0), SelLength(0), Width(0) {}
    void SetFocus() {}   // offline no-op, same as TfQwertyKeyPanel::SetFocus
};

// ===========================================================================
//  TfQwertyKey -- non-VCL facade (golden myQwertyKeyBoard.h:34-159,
//  TfQwertyKey : public TForm)
// ===========================================================================
class TfQwertyKey
{
public:
    TfQwertyKey();   // PORT: fields only -- golden ctor body lives in Init()
    void Init();     // golden ctor body :64-135 (GATE (Q-D1) inside)

    // -- content strip (golden h:37-38) --------------------------------------
    TfQwertyKeyPanel *palContent      = new TfQwertyKeyPanel();
    TfQwertyKeyEdit  *edQwertyContent = new TfQwertyKeyEdit();

    // -- numeric pad (golden h:43-66) ----------------------------------------
    TPanel       *palNumKey    = new TPanel();
    TSpeedButton *speedbNo17   = new TSpeedButton(), *speedbNo18 = new TSpeedButton();
    TSpeedButton *speedbNo19   = new TSpeedButton(), *speedbNo14 = new TSpeedButton();
    TSpeedButton *speedbNo15   = new TSpeedButton(), *speedbNo16 = new TSpeedButton();
    TSpeedButton *speedbNo11   = new TSpeedButton(), *speedbNo12 = new TSpeedButton();
    TSpeedButton *speedbNo13   = new TSpeedButton(), *speedbNo10 = new TSpeedButton();
    TSpeedButton *spbDP        = new TSpeedButton(), *spbMinus   = new TSpeedButton();
    TSpeedButton *spbCancel2   = new TSpeedButton(), *spbSummit2 = new TSpeedButton();
    TSpeedButton *spbClear2    = new TSpeedButton(), *spbBackSpace2 = new TSpeedButton();
    TSpeedButton *spbPercent   = new TSpeedButton();
    TSpeedButton *spbAdd1      = new TSpeedButton(), *spbMinus1   = new TSpeedButton();
    TSpeedButton *spbAdd10     = new TSpeedButton(), *spbMinus10  = new TSpeedButton();
    TSpeedButton *spbAdd100    = new TSpeedButton(), *spbMinus100 = new TSpeedButton();

    // -- QWERTY panel (golden h:67-120) --------------------------------------
    TPanel       *palQwertyKey = new TPanel();
    TSpeedButton *speedbSyb3   = new TSpeedButton();
    TSpeedButton *speedbNo01   = new TSpeedButton(), *speedbNo02 = new TSpeedButton();
    TSpeedButton *speedbNo03   = new TSpeedButton(), *speedbNo04 = new TSpeedButton();
    TSpeedButton *speedbNo05   = new TSpeedButton(), *speedbNo06 = new TSpeedButton();
    TSpeedButton *speedbNo07   = new TSpeedButton(), *speedbNo08 = new TSpeedButton();
    TSpeedButton *speedbNo09   = new TSpeedButton(), *speedbNo00 = new TSpeedButton();
    TSpeedButton *speedbSyb_   = new TSpeedButton(), *speedbSyPl = new TSpeedButton();
    TSpeedButton *speedbChrQ   = new TSpeedButton(), *speedbChrW = new TSpeedButton();
    TSpeedButton *speedbChrE   = new TSpeedButton(), *speedbChrR = new TSpeedButton();
    TSpeedButton *speedbChrT   = new TSpeedButton(), *speedbChrY = new TSpeedButton();
    TSpeedButton *speedbChrU   = new TSpeedButton(), *speedbChrI = new TSpeedButton();
    TSpeedButton *speedbChrO   = new TSpeedButton(), *speedbChrP = new TSpeedButton();
    TSpeedButton *speedbSybL   = new TSpeedButton(), *speedbSybR = new TSpeedButton();
    TSpeedButton *speedbSyOr   = new TSpeedButton();
    TSpeedButton *speedbChrA   = new TSpeedButton(), *speedbChrS = new TSpeedButton();
    TSpeedButton *speedbChrD   = new TSpeedButton(), *speedbChrF = new TSpeedButton();
    TSpeedButton *speedbChrG   = new TSpeedButton(), *speedbChrH = new TSpeedButton();
    TSpeedButton *speedbChrJ   = new TSpeedButton(), *speedbChrK = new TSpeedButton();
    TSpeedButton *speedbChrL   = new TSpeedButton();
    TSpeedButton *speedbSyb1   = new TSpeedButton(), *speedbSyb2 = new TSpeedButton();
    TSpeedButton *speedbChrZ   = new TSpeedButton(), *speedbChrX = new TSpeedButton();
    TSpeedButton *speedbChrC   = new TSpeedButton(), *speedbChrV = new TSpeedButton();
    TSpeedButton *speedbChrB   = new TSpeedButton(), *speedbChrN = new TSpeedButton();
    TSpeedButton *speedbChrM   = new TSpeedButton();
    TSpeedButton *speedbSyb4   = new TSpeedButton(), *speedbSyb5 = new TSpeedButton();
    TSpeedButton *speedbSyb6   = new TSpeedButton();
    TSpeedButton *speedbSpac   = new TSpeedButton();
    TSpeedButton *spbChangeCase= new TSpeedButton();

    // -- value-limit strip (golden h:121-127) --------------------------------
    TfQwertyKeyPanel *palValueLimit = new TfQwertyKeyPanel();
    TEdit  *edCurrValue = new TEdit();
    TLabel *labMaxValue = new TLabel();
    TEdit  *edMaxValue  = new TEdit();
    TLabel *labMinValue = new TLabel();
    TEdit  *edMinValue  = new TEdit();

    // -- form-level fields (see DEVIATION (D-7)/(D-8)) -----------------------
    int  Top    = 239;    // .dfm Top
    int  Left   = 654;    // .dfm Left
    int  Width  = 1005;   // .dfm ClientWidth (see D-7)
    int  Height = 469;    // .dfm ClientHeight (see D-7)
    bool Showing = false; // golden TForm::Showing (see D-8)

    void ShowModal() {}   // golden TForm::ShowModal -- offline no-op (see BEHAVIOUR NOTE)
    void Close()     {}   // golden TForm::Close    -- offline no-op (fTemp_Set.h precedent)

    // -- golden public: User declarations (h:150-158) ------------------------
    std::vector<TQwertyKeyClass *> QwertyKey;
    void ShowQwertyKey(TControl *Ptr, int iFunction, int iDP=0, bool bCheckRange=false, double min=0, double max=0);  // golden :169-302; D-5
    bool bUpperCase    = false;
    bool bNoSymbol     = false;
    int  iDecimalPoint = 0;
    bool bShow         = false;
    bool bKey2         = false;                                                 //Steven 20150505 : 剛好在輸入時,Alarm會卡死

    // -- golden private: User declarations (h:144-149), public collapse (D-6)
    bool bIntegerOnly = false;   // ZeroInitVclFields: golden ctor never sets it
    int  KeyCode      = 0;       // ZeroInitVclFields: golden ctor never sets it
    AnsiString sBackup;

    // -- event handlers / helpers (parameter lists per DEVIATIONS) -----------
    void spbChangeCaseClick();                     // golden :304-318
    void spbBackSpaceClick();                      // golden :341-345
    void FormDestroy();                            // golden :152-167 -- ROOT myQwertyKeyBoard.cpp
    void FormShow();                               // golden :137-143
    void spbSummitClick();                         // golden :347-350
    void spbClearClick();                          // golden :352-355
    void FormResize();                             // golden :357-360
    void spbCancelClick();                         // golden :362-366
    void spbPercentClick();                        // golden :368-377
    void spbMinusClick();                          // golden :421-431
    void spbAdd1Click(TSpeedButton *Sender);       // golden :433-449 (D-3)
    void spbDPClick();                             // golden :451-458
    void edQwertyContentKeyDown(unsigned short &Key);  // golden :460-467 (D-4)
    void edQwertyContentKeyPress(char &Key);       // golden :469-538
    void FormClose();                              // golden :145-150
    void spbKeyClick(TSpeedButton *Sender);        // golden :320-339 (D-3)
    void ChangeDecimalPoint();                     // golden :379-419
};

// AI(W906-FW-QWKEY1) 20260824: golden myQwertyKeyBoard.h:161-162
// `extern PACKAGE TfQwertyKey *fQwertyKey; / *fQwertyKey2;` -- PACKAGE glue
// dropped.  Defined (zero-initialized, NEVER constructed at static init --
// golden's own pre-CreateForm state) in forms/fQwertyKey.cpp.
extern TfQwertyKey *fQwertyKey;
extern TfQwertyKey *fQwertyKey2;                                                //Steven 20150505 : 剛好在輸入時,Alarm會卡死

#endif // FORMS_FQWERTYKEY_H
