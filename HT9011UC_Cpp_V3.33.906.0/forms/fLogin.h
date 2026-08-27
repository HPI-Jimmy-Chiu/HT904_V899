// =============================================================================
//  forms/fLogin.h  --  non-VCL facade for golden's TfLogin (login.h/login.cpp)
//  -- the username/password entry dialog cSecurity.cpp drives for account
//  add/delete/modify.
//
//  AI(W906-FW3-LGM1) 20260828: new file, wave FW3-LGM1 (landed together with
//  forms/fGroundMan.h -- unrelated forms, paired only by wave scheduling).
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/login.h (33 lines) +
//  login.cpp (88 lines) + login.dfm (cp950, decoded with
//  `open(p, encoding='cp950', errors='replace')`, 0 U+FFFD, this wave
//  20260828).
//
//  ROLE
//  ----
//  TfLogin is the small modal collecting a user name + old/new password pair
//  for cSecurity.cpp's account maintenance flow (rgLoginOption selects
//  add(0)/delete(1)/modify(2)). It carries no file I/O, no comm, no machine
//  action of its own -- cSecurity.cpp (NOT this file, and NOT touched by this
//  wave) owns the actual account-file read/write. Every one of cSecurity.cpp's
//  `fLogin->...` derefs sits inside an `#if 0` block today (verified this
//  wave, see SHIM OCCUPATION below) -- this facade exists so a future
//  un-gate of that block binds to real members/types instead of failing to
//  compile or (worse) binding to a same-named member on a different class.
//
//  THIS IS A NEW CLASS -- NO EXISTING STAND-IN, NO ODR CONFLICT
//  --------------------------------------------------------------------------
//  Verified this wave (20260828, `python tools/census/wave_preflight.py
//  login.cpp`):
//    * class name `TfLogin`         -- 0 hits tree-wide (clear)
//    * global name `fLogin`         -- 0 hits tree-wide (clear)
//    * member names (FormShow/sbOkClick/rgLoginOptionClick) exist on SIX
//      OTHER classes tree-wide (TFormBarcodeReader/TfContactCT/
//      TfCounterClear/TfSecurity/TfLotInfo/TfPassword, all own `::FormShow`)
//      -- those are each class's OWN method, not an occupation of TfLogin's
//      name; re-checked by path, none is this file.
//  Re-run before wiring -- sibling waves can land files that falsify this.
//
//  FORM CLASS SHAPE: plain non-VCL class, NOT TForm-derived (established
//  convention, forms/fPassword.h precedent). golden's `__fastcall
//  TfLogin(TComponent* Owner) : TForm(Owner) {}` ctor body is EMPTY (no
//  golden logic beyond the base-class call) -- no ctor/Init() split needed,
//  no PORT-SPLIT risk: the real ctor here touches nothing but its own
//  `new`'d widget stand-ins (ctor-safety rule; this is one of the two waves
//  this project run that must get it right FIRST TIME, see GroundMan's
//  header for the ctor that actually needed splitting).
//
//  GLOBAL POINTER: `extern TfLogin *fLogin;` constructed via `new` at static
//  init (forms/fCleaning.cpp:56 / forms/fMesSystem.cpp:597 /
//  forms/fSmartDiagnostic.cpp:37 convention -- supersedes the earlier
//  "leave NULL" fPassword-wave posture). Safe here because the ctor body
//  below derefs nothing but its own fields.
//
//  WAVE SCOPE -- ALL 4 golden TfLogin:: methods, every one read line-by-line
//  in full. Denominator: 4 distinct TfLogin:: method definitions in golden
//  login.cpp (the whole file; confirmed via `python
//  tools/census/wave_preflight.py login.cpp`: "census sees 4 bodies /
//  stripped sees 4 / swallowed 0", 88 span lines).
//    TfLogin (ctor)        golden :12-15  ACTIVE (body is empty -- see above)
//    FormShow               golden :17-67  MIXED -- GATE (LG-1)
//    sbOkClick               golden :69-75  ACTIVE
//    rgLoginOptionClick      golden :77-102 ACTIVE
//  Totals: 3 fully ACTIVE + 1 MIXED (active spine, one gated branch) =
//  4/4 declared, 0 omitted.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (LG-1) FormShow golden :56-62 -- the CUSTOMER_CODE==CC_Greatek /
//      IniConfig.bN15UseESDControlMachine branch calls
//      `fProductionInfo->ScreenkeyboardShow();`. Checked this wave (20260828):
//      forms/fProductionInfo.h declares NO `ScreenkeyboardShow` member (grep
//      "ScreenkeyboardShow" tree-wide -- 0 hits anywhere in this port, golden
//      only). CUSTOMER_CODE/CC_Greatek/IniConfig.bN15UseESDControlMachine are
//      all REAL (see DEPENDENCY AUDIT) -- only the callee is missing. Gated
//      in place; everything else in FormShow (including the rest of THIS
//      SAME if-block's sibling branches) stays ACTIVE. BEHAVIOUR DELTA while
//      gated: CC_Greatek machines with bN15UseESDControlMachine=true do not
//      get the on-screen keyboard auto-shown for this dialog -- cosmetic,
//      not a login-security change (login still requires the physical/
//      web keyboard).
//
//  NULL-GLOBAL NOTE
//  --------------------------------------------------------------------------
//  None -- unlike fPassword/fQwertyKey, fLogin itself IS constructed (see
//  GLOBAL POINTER above). No other global this file touches is NULL: pwPath/
//  CUSTOMER_CODE/AccessLevel/iDefEngineerLevel/IniConfig are plain data
//  globals (extern, always live), and the one object-pointer global this
//  file WOULD touch (fProductionInfo, for GATE LG-1) is not dereferenced --
//  it stays inside the closed gate.
//
//  GOLDEN NOTES (faithful, recorded for the ledger)
//  --------------------------------------------------------------------------
//  (G-L1) FormShow golden :29: `if(AccessLevel<=iDefEngineerLevel)` -- reads
//      as "at or below engineer level", i.e. the LOWER-privileged branch
//      forces ItemIndex=2 (modify) and hides the option group; the
//      higher-privileged branch (else) leaves ItemIndex=0 (add) and shows
//      it. Kept exactly as written; not restructured.
//  (G-L2) sbOkClick golden :71-72: `if(cbLoginUserName->Visible)
//      edUserName->Text=cbLoginUserName->Text;` -- only overwrites the typed
//      user name from the dropdown when that dropdown is showing (delete/
//      modify flows); the add flow (dropdown hidden) keeps whatever was
//      typed into edUserName directly. Verbatim.
//
//  DEVIATIONS (port-only adaptations, zero intended behaviour change)
//  --------------------------------------------------------------------------
//  (D-1) `__fastcall`/`TComponent* Owner`/PACKAGE glue dropped; plain class,
//      default (no-arg) ctor.
//  (D-2) Event handler `TObject *Sender` parameters dropped everywhere --
//      verified unread in all 4 golden bodies including the gated branch.
//  (D-3) Form-level Left/Top/Width/Height are plain int fields (fPassword.h
//      D-5 precedent). login.dfm has no top-level `Width=`/`Height=` line
//      (only `ClientWidth=405`/`ClientHeight=337`, BorderStyle=bsSingle) --
//      seeded Width/Height from those ClientWidth/ClientHeight values
//      (real VCL's actual Width/Height would run a few px larger for the
//      title bar/border chrome; FormShow's use is screen-centering math
//      only, not safety-relevant, so the approximation is accepted here and
//      not chased further). Left/Top seeded from the .dfm's own
//      Left=355/Top=356 (immediately overwritten by FormShow regardless).
//  (D-4) No wrapper widget types needed: vclcompat::TLabel/TEdit/
//      TSpeedButton/TRadioGroup/TComboBox already carry every property this
//      form's 4 methods touch (Text/Visible/ItemIndex/Down/Caption,
//      SetFocus() from the TControl base) -- first facade this campaign
//      that needed ZERO forms/FormWidgets.h-style composition.
//  (D-5) golden TForm::ShowModal()/Close() -- offline no-ops
//      (fPassword.h/fTemp_Set.h precedent). `Close()` sets `Visible=false`;
//      `ShowModal()` is a pure no-op (golden's modal loop does not exist
//      headless -- the web layer owns show/dismiss). `Visible` added for
//      the same reason fPassword's was (SECSGEM/uHGemHT9045.cpp G23
//      precedent: cheap to carry, and `Close()` needs something to write).
//
//  DEPENDENCY AUDIT (this wave, 20260828) -- all grepped before translating:
//    pwPath                   REAL  cmydef.h:3314 (extern AnsiString)
//    FileExists                REAL  vclcompat/SysUtils.h:90
//    CUSTOMER_CODE             REAL  cmydef.h:3181 (extern int)
//    CC_AMKOR_China/CC_QUALCOMM/CC_Greatek
//                               REAL  MachineType.h:348/377/330
//    AccessLevel                REAL  cmydef.h:3503 (extern int)
//    iDefEngineerLevel          REAL  cmydef.h:3590 (extern int)
//    IniConfig.bN15UseESDControlMachine
//                               REAL  Config.h:1125 (field) / :1499 (extern
//                                     HT9045_CONFIG IniConfig)
//    fProductionInfo->ScreenkeyboardShow()
//                               NO PORT -- GATED (LG-1)
//    TLabel/TEdit/TSpeedButton/TRadioGroup/TComboBox
//                               REAL  vclcompat/Controls.h
//
//  CMAKE STATUS -- NOT WIRED by this wave. Suggested landing: ht9045_forms
//  source list (diet-clean; no ht9045_sm/ht9045_io/ht9045_motor/ht9045_db
//  symbol is touched by any ACTIVE line in forms/fLogin.cpp).
// =============================================================================
#ifndef FORMS_FLOGIN_H
#define FORMS_FLOGIN_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject at global scope
#include "vclcompat/Controls.h"     // TLabel/TEdit/TSpeedButton/TRadioGroup/TComboBox

// ===========================================================================
//  TfLogin -- non-VCL facade (golden login.h:11-30, TfLogin : public TForm)
// ===========================================================================
class TfLogin
{
public:
    TfLogin();   // golden ctor :12-15 -- body is empty, nothing to split out

    // -- __published widgets (golden h:14-22) --------------------------------
    TLabel       *labUserName         = new TLabel();
    TEdit        *edUserName          = new TEdit();
    TLabel       *labOldPassword      = new TLabel();
    TEdit        *edLoginOldPassword  = new TEdit();
    TSpeedButton *sbOk                = new TSpeedButton();
    TRadioGroup  *rgLoginOption       = new TRadioGroup();
    TLabel       *labNewPassword      = new TLabel();
    TEdit        *edLoginNewPassword  = new TEdit();
    TComboBox    *cbLoginUserName     = new TComboBox();   //0:new , 1:delete , 2:modify //ChungHung 20110920 add

    // -- form-level fields (see DEVIATION (D-3)) -----------------------------
    int Left   = 355;   // .dfm Left
    int Top    = 356;   // .dfm Top
    int Width  = 405;   // .dfm ClientWidth (see D-3) -- READ by FormShow's centering math
    int Height = 337;   // .dfm ClientHeight (see D-3) -- READ by FormShow's centering math

    // -- golden TForm members (see DEVIATION (D-5)) --------------------------
    void ShowModal() {}                  // golden TForm::ShowModal -- offline no-op
    void Close()     { Visible = false; }// golden TForm::Close -- offline no-op + Visible drop
    bool Visible = false;

    // -- event handlers (golden h:23-25) -------------------------------------
    void FormShow();             // golden :17-67 -- GATE (LG-1) inside
    void sbOkClick();            // golden :69-75
    void rgLoginOptionClick();   // golden :77-102
};

// ---------------------------------------------------------------------------
// AI(W906-FW3-LGM1) 20260828: golden login.h:32 `extern PACKAGE TfLogin
// *fLogin;` -- PACKAGE glue dropped. Constructed via `new` (GLOBAL POINTER
// note above) in forms/fLogin.cpp -- ctor-safe (see banner).
extern TfLogin *fLogin;

#endif // FORMS_FLOGIN_H
