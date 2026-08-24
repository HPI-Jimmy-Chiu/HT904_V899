// =============================================================================
//  forms/fPassword.h  --  non-VCL facade for golden's TfPassword
//  (Password.h/.cpp) -- the operator login / EventLog-login password dialog
//
//  AI(W906-FW-QWKEY1) 20260824: new file, FW-3 wave FW-QWKEY1 (landed
//  together with forms/fQwertyKey.h -- fPassword's own FormShow/MouseDown
//  handlers dereference fQwertyKey, so the two forms ship as one wave).
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/Password.h (116 lines) +
//  Password.cpp (398 lines) + Password.dfm (1,545 lines) -- all cp950,
//  decoded with `python -c "open(p,'rb').read().decode('cp950')"`,
//  0 U+FFFD, this wave (20260824).
//
//  ROLE
//  ----
//  TfPassword is golden's login dialog: the classic user-name/password page
//  (tsPassword) plus the ASE-CL EventLog-login pair (tsEventlogLogin /
//  tsEventLogLoginModify, Steven 20181224) with its own 6-account password
//  book (EvenLogUSER <- D:\HT9045\System\EvenLogLevel.ini).  NOTE the
//  20260819 write-path ruling ("登入與權限比照 BCB6 既有 fSecurity 體系") and
//  WebAuth.h: the WEB login's verification core is WebAuth.h, NOT this form
//  -- this facade exists for translation completeness (fSetup's queued
//  fPassword-blocked methods, uHGemHT9045's G23 AUTHORITY_CHECK site,
//  csystem's two gated fPassword->bShow reads) and carries the EventLog
//  password-book logic those sites need, with every password-file
//  read/write GATED (see GATE REGISTER -- task rule: 密碼相關安全語意一律
//  gate 不解).
//
//  THIS IS A NEW CLASS -- NO EXISTING STAND-IN, NO ODR CONFLICT
//  --------------------------------------------------------------------------
//  Re-verified this wave (20260824 11:25-12:00):
//    * `Grep "TfPassword|fPassword\s*;|fPassword\s*=|extern.*fPassword"
//      --glob *.h` tree-wide -- 0 hits (only comments/artifacts elsewhere).
//    * csystem.cpp:2728/:2953 DO spell `fPassword->bShow` -- traced this
//      wave: both sit inside the `#if 0 // GOLDEN VERBATIM` block that opens
//      at csystem.cpp:598 (preprocessor-stack walk, 20260824) -- NOT
//      compiled, no conflict; they become un-gate candidates now.
//    * `Grep "EvenLog_PASS_WORD|EvenLogUSER" --glob *.{h,cpp}` -- 0 hits.
//  Re-run all three at wave close (absence claims go stale).
//
//  FORM CLASS SHAPE: plain non-VCL class, NOT TForm-derived (established).
//
//  CTOR / Init() SPLIT (task-mandated; fTemp_Set.h:483 / cSetUp.cpp Init()
//  convention)
//  --------------------------------------------------------------------------
//  Golden's ctor (Password.cpp:20-26) sets bShow/bShowTab AND calls
//  ReadPasswordFile() -- file I/O with missing-key WRITE-BACK (see GATE
//  (P-R1)) -- exactly the class of work the "ctor 只塞欄位" rule exists to
//  keep out of any static-init path.  Here: ctor = field defaults (in-class
//  initializers, ZeroInitVclFields semantics -- golden never initializes
//  iNowEvenLogUserLevel/bLoginSuccess anywhere and RELIES on BCB6 TObject's
//  zero-fill; reproduced as `=0/false` below) + .dfm-sourced values; the
//  golden ctor BODY (bShow/bShowTab sets + the ReadPasswordFile() call)
//  moves verbatim into explicit Init().  Neither global is constructed at
//  static init (both stay NULL, golden's own pre-CreateForm state).
//
//  WAVE SCOPE -- ALL 24 methods (23 golden TfPassword:: + ctor-as-Init),
//  every one read line-by-line in full.  Denominator: 23 distinct
//  TfPassword:: method definitions in golden Password.cpp (counted this
//  wave -- the whole file).
//    TfPassword ctor           (PORT split)     fields only, see above
//    Init()                    golden :20-26    ACTIVE (ctor body; the
//                                               ReadPasswordFile() call is
//                                               ACTIVE -- the gate sits
//                                               INSIDE ReadPasswordFile)
//    ShowEventLogLogin         golden :28-32    ACTIVE (see NULL-GLOBAL note)
//    FormShow                  golden :34-116   ACTIVE (GOLDEN (G-P1) inside)
//    SpeedButton1Click         golden :118-121  ACTIVE -- DFM-ORPHANED, see
//                                               GOLDEN (G-P3)
//    spbCancelClick            golden :123-128  ACTIVE
//    edUserNameKeyDown         golden :130-163  MIXED -- GATE (P-B1)
//    edPasswordKeyDown         golden :165-184  MIXED -- GATE (P-B2)
//    FormClose                 golden :186-207  ACTIVE (SystemStart=false is
//                                               the SAFE direction -- golden
//                                               Ifor 20180928's own purpose:
//                                               避免密碼輸入完後機台直接動作)
//    edPasswordMouseDown       golden :209-232  MIXED -- GATE (P-B3); the two
//                                               fQwertyKey->ShowQwertyKey
//                                               branches are ACTIVE (first
//                                               compiled consumer of this
//                                               wave's TfQwertyKey)
//    edUserNameMouseDown       golden :234-263  MIXED -- GATE (P-B4)
//    cbUserNameChange          golden :265-269  ACTIVE
//    sbPasswordCancelClick     golden :273-276  ACTIVE
//    sbPasswordOKClick         golden :278-282  ACTIVE
//    sbPasswordModifyOKClick   golden :284-297  ACTIVE -- lives in ROOT
//        Password.cpp (ht9045_sm suggestion), NOT forms/fPassword.cpp:
//        ShowMyMessage (canary_support.h:80, body canary_support.cpp --
//        ht9045_sm) is outside the ht9045_forms diet.  Same root split as
//        cSetUp.cpp::cbI21Click (that file's precedent for this exact
//        symbol) and forms/fQwertyKey.h's FormDestroy entry.
//    sbPasswordModifyClick     golden :299-312  ACTIVE
//    SetUserLevel              golden :314-317  ACTIVE
//    CheckPassword             golden :319-336  ACTIVE (pure in-process)
//    CheckLoginSuccess         golden :338-341  ACTIVE
//    GetLoginLevel             golden :343-346  ACTIVE
//    ReadPasswordFile          golden :348-369  MIXED -- GATE (P-R1)
//    SavePasswordFile          golden :371-381  GATED  -- GATE (P-S1)
//    btnOKClick                golden :385-388  ACTIVE
//    btnDownloadClick          golden :390-397  MIXED -- GATE (P-D1)
//  Totals: 18 fully ACTIVE + 5 MIXED (active spine, gated branch) + 1
//  fully-gated body (SavePasswordFile) = 24/24 declared, 0 omitted.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (P-B1) edUserNameKeyDown golden :134-153 -- the two barcode branches
//      (IniConfig.bVTESTFunction; CC_SCC||CosFunction.bUseBarCoderAutoLogin)
//      call Clipboard()->Clear() and InputBarcodeNumber(...), NEITHER of
//      which has a port anywhere in this tree (forms/fLotInfo.cpp GATE
//      WB-2/WB-4/WB-5/WB-7/WB-9 -- the established gate for these exact two
//      names; re-verified this wave: `Grep "InputBarcodeNumber" --glob
//      *.{h,cpp}` hits only fLotInfo.cpp's own gated blocks).  The golden
//      `#ifndef SOFT_SIMULTE` wrapper is kept VERBATIM AND ACTIVE (acarry.cpp
//      posture -- SOFT_SIMULTE is NOT defined in this tree); each barcode
//      branch BODY is `#if 0`-gated IN PLACE inside its own braces, so the
//      if/else-if/else chain keeps golden's exact branch structure and a
//      barcode-configured machine falls into an empty branch (NOT into the
//      keyboard else-branch).  BEHAVIOUR DELTA while gated: those customers
//      get a no-op instead of a barcode prompt -- joins the BarcodeReader
//      wave.
//  (P-B2) edPasswordKeyDown golden :169-177 -- same shape, one branch
//      (CC_SCC || IniConfig.bVTESTFunction).
//  (P-B3) edPasswordMouseDown golden :217-226 -- same shape, middle branch
//      (CC_SCC || USE_BARCODE_AS_KEYBOARD!=0 || bVTESTFunction).  The
//      CC_KYEC_LEE branch (:213-216) and the trailing else (:229-231) --
//      both `fQwertyKey->ShowQwertyKey(edPassword,
//      N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD)` -- are ACTIVE: TfQwertyKey lands
//      this same wave (forms/fQwertyKey.h), retiring the tree-wide WA-1
//      "no compiled TfQwertyKey" gate FOR THIS FILE ONLY (other files'
//      WA-1 gates are outside this wave's write boundary and stay closed).
//  (P-B4) edUserNameMouseDown golden :238-257 -- same shape, two gated
//      branches; trailing else `fQwertyKey->ShowQwertyKey(edUserName,
//      N_NO_SYMBOL|N_NO_SPACE)` ACTIVE.
//  (P-R1) ReadPasswordFile golden :353/:356/:359/:362/:365/:368 -- the six
//      `EvenLogUSER.PassWord[..]=CheckAndReadIniData(sFileName,"LEVEL",...,
//      AnsiString("1"))` reads.  CheckAndReadIniData (common.h:220-225) is
//      the WRITE-BACK family: a missing key is CREATED in the ini with the
//      caller's default -- here that would write password "1" for all six
//      accounts into D:\HT9045\System\EvenLogLevel.ini, a shared-machine
//      config write AND password semantics -- double-gated by task rule
//      (寫共用 config 照慣例 gate；密碼安全語意一律 gate 不解).  The six
//      `EvenLogUSER.ID[..]="..."` assignments are pure in-process and stay
//      ACTIVE.  ⚠ SECURITY DELTA WHILE GATED: PassWord[] stays "" (AnsiString
//      default), so CheckPassword() would accept an EMPTY password for any
//      account -- unreachable today (nothing constructs fPassword and no
//      handler is wired), but the wiring wave MUST resolve this gate BEFORE
//      wiring any EventLog-login path.  Recorded here so it cannot be
//      absorbed silently.
//  (P-S1) SavePasswordFile golden :375-380 -- six WriteIniData calls that
//      REWRITE the password book on disk.  Entire write block gated
//      (password-file write; task rule).  sbPasswordModifyOKClick's call to
//      SavePasswordFile() stays ACTIVE (the callee is the no-op).
//  (P-D1) btnDownloadClick golden :393-395 --
//      `fFTPClient->DownloadPasswordFormServer()`: the fFTPClient singleton
//      was deliberately DEMOTED by the KYECFTP wave (no `fFTPClient->`
//      surface exists anywhere -- AutoRetest.cpp:319-332's seam note, the
//      established citation).  Gated with its dependent
//      `if(bFlag) lblPWDownload->Visible=true;`.  `btnDownload->Down=false;`
//      (:396) is ACTIVE.
//
//  NULL-GLOBAL NOTE (inherited from golden, not new)
//  --------------------------------------------------------------------------
//  FormShow (:51-54) dereferences the GLOBAL fQwertyKey (`->Showing` /
//  `->Close()`), ShowEventLogLogin (:31) the GLOBAL fPassword
//  (`->ShowModal()`), and CheckPassword/sbPasswordModify* read the GLOBAL
//  EvenLogUSER.  fQwertyKey/fPassword stay NULL until a wiring wave
//  constructs them (golden's own pre-CreateForm exposure); no handler is
//  wired today, so no runtime path reaches the derefs.  ShowModal()/Close()
//  are offline no-op members (fTemp_Set.h:575-584 precedent) -- golden's
//  modal loop does not exist headless, so ShowEventLogLogin sets bShowTab
//  and returns; the web layer owns show/dismiss.
//
//  GOLDEN NOTES (faithful, recorded for the ledger)
//  --------------------------------------------------------------------------
//  (G-P1) FormShow golden :89-102: `if(bShowTab==0){...tsPassword...}
//      else if(bShowTab==0){...tsEventlogLogin...}` -- the SECOND condition
//      repeats ==0, so the EventLog branch is UNREACHABLE dead code (an
//      else-if can never re-test the same condition true).  With bShowTab==1
//      (ShowEventLogLogin's path) NEITHER branch runs -- the EventLog tab is
//      never activated by FormShow; almost certainly intended
//      `else if(bShowTab==1)`.  TRANSLATED VERBATIM (defect and all).
//  (G-P2) Password.h:92 declares `bool bShowTab` but every golden write is
//      integer (0/1) and ShowEventLogLogin's `bShowTab=1` reads back as
//      `bShowTab==0` false -- works via bool conversion; cosmetic.  Kept
//      bool, kept 0/1 spellings.
//  (G-P3) SpeedButton1Click (:118-121) is wired to NOTHING in Password.dfm
//      (event census this wave: no `= SpeedButton1Click` anywhere in the
//      .dfm) and no SpeedButton1 member exists -- a dfm-orphaned leftover
//      handler.  Translated verbatim (forms/fSetup.cpp RadioButton1KeyDown
//      precedent for exactly this situation).
//  (G-P4) CheckPassword golden :325-326: with JCET_FOR_EVAN==1 the ID match
//      is bypassed INSIDE the loop, so the login level granted is the LOWEST
//      index whose PassWord matches -- order-dependent by design.  Verbatim.
//
//  DEVIATIONS (port-only adaptations, zero intended behaviour change)
//  --------------------------------------------------------------------------
//  (D-1) `__fastcall`/`TComponent* Owner`/PACKAGE glue dropped; plain class.
//  (D-2) golden ctor body -> explicit Init() (see CTOR/Init SPLIT).
//  (D-3) Unread VCL glue parameters dropped per-handler (verified
//      per-function): every handler below is zero-parameter EXCEPT
//      edUserNameKeyDown/edPasswordKeyDown (Key IS read; golden `WORD &Key`
//      -> `unsigned short &Key`, same D-4 as forms/fQwertyKey.h) and
//      SetUserLevel(int).  edPassword/edUserName MouseDown lose all five
//      params (Sender/Button/Shift/X/Y -- none read anywhere in either
//      golden body, gated branches included).
//  (D-4) golden private: members (iNowEvenLogUserLevel/bLoginSuccess/
//      CheckPassword/ReadPasswordFile/SavePasswordFile) collapsed to public
//      (established convention, forms/fTemp_Set.h:566-569).
//  (D-5) Form-level Left/Top/Width/Height are plain int fields seeded from
//      the .dfm (508/379/617/336) -- FormShow READS Width/Height for its
//      centering math before writing Left/Top.
//  (D-6) Wrapper widget types (compose, don't fork vclcompat --
//      forms/fDynamicTemp.h precedent): TfPasswordPageControl adds
//      `TTabSheet *ActivePage` (vclcompat::TPageControl carries only
//      ActivePageIndex); TfPasswordEdit adds SetFocus() (offline no-op);
//      TfPasswordComboBox adds `int Left` (FormShow writes cbUserName->
//      Left=216).  edPassword's .dfm PasswordChar='*' is NOT carried --
//      no translated statement reads or writes it on THIS form (the
//      keyboard's own edQwertyContent handles masking), and unused surface
//      is how facades rot.
//
//  EXCLUDED WIDGETS (no method dereferences them by name): Label2..Label9,
//  lbPasswordUserName, lbPasswordPassWord, sbPasswordModify,
//  sbPasswordModifyOK, sbPasswordModifyCancel, sbPasswordCancel,
//  sbPasswordOK, btnOK, spbCancel.  `Visible` on the FORM itself was
//  deliberately absent at translation time; ADDED 20260824 (FW-QWKEY4) for
//  its single measured consumer, SECSGEM/uHGemHT9045.cpp G23 -- see the
//  field's own note near ShowModal()/Close().
//
//  DEPENDENCY AUDIT (this wave, 20260824) -- all grepped before translating:
//    CUSTOMER_CODE            REAL  cmydef.h:3181; CC_Greatek/CC_SCC/
//                                   CC_KYEC_LEE/CC_JCET MachineType.h:330/316/292/333
//    JCET_FOR_EVAN            REAL  cmydef.h:4989 (extern int)
//    USE_BARCODE_AS_KEYBOARD  REAL  cmydef.h:2974 (extern int)
//    bEnableEmployeeIDCheck   REAL  cmydef.h:3889
//    SystemStart              REAL  cmydef.h:221
//    AccessLevel              REAL  cmydef.h:3503 (root file only)
//    IniConfig                REAL  Config.h:1499 (.bVTESTFunction :252,
//                                   .bFtpPasswordDownload :990,
//                                   .bA02DisableSaveParsWhenSwitchToOp :274)
//    CosFunction              REAL  CosFunction.h:488 (.bTechComUseComboBox
//                                   :63, .bUseBarCoderAutoLogin :392,
//                                   .bLoginASECL :476)
//    CheckAndReadIniData      REAL  common.h:220-225 (GATED here, P-R1)
//    WriteIniData             REAL  common.h (GATED here, P-S1)
//    ShowMyMessage            REAL  canary_support.h:80 (root file only)
//    fQwertyKey/TfQwertyKey   REAL  forms/fQwertyKey.h (THIS wave)
//    InputBarcodeNumber/Clipboard()/fFTPClient  NO PORT -- GATED (above)
//    TEdit/TLabel/TComboBox/TSpeedButton/TPageControl/TTabSheet  REAL
//                                   vclcompat/Controls.h
//
//  CMAKE STATUS -- NOT WIRED by this wave.  SUGGESTED landing (mirrors
//  forms/fQwertyKey.h's):
//    forms/fPassword.cpp -> ht9045_forms source list (diet-clean; the
//        fPassword/fPassword2/EvenLogUSER globals live here so ht9045_forms
//        consumers can reach them without a forms->sm edge).
//    Password.cpp (root) -> ht9045_sm source list (sbPasswordModifyOKClick
//        only, next to cSetUp.cpp).
// =============================================================================
#ifndef FORMS_FPASSWORD_H
#define FORMS_FPASSWORD_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject at global scope
#include "vclcompat/Controls.h"     // TEdit/TLabel/TComboBox/TSpeedButton/TPageControl/TTabSheet

// ---------------------------------------------------------------------------
//  TfPasswordPageControl -- facade-only wrapper (see DEVIATION (D-6)):
//  vclcompat::TPageControl carries only ActivePageIndex; golden writes
//  `PageControl1->ActivePage=tsPassword;` (a TTabSheet* property in real VCL).
// ---------------------------------------------------------------------------
class TfPasswordPageControl : public vclcompat::TPageControl
{
public:
    TTabSheet *ActivePage;

    TfPasswordPageControl() : ActivePage(0) {}
};

// ---------------------------------------------------------------------------
//  TfPasswordEdit -- facade-only wrapper: adds SetFocus() (offline no-op,
//  same as forms/fQwertyKey.h's TfQwertyKeyEdit::SetFocus).
// ---------------------------------------------------------------------------
class TfPasswordEdit : public vclcompat::TEdit
{
public:
    void SetFocus() {}
};

// ---------------------------------------------------------------------------
//  TfPasswordComboBox -- facade-only wrapper: adds Left (FormShow golden :78
//  `cbUserName->Left=216;`; .dfm initial 238).
// ---------------------------------------------------------------------------
class TfPasswordComboBox : public vclcompat::TComboBox
{
public:
    int Left;

    TfPasswordComboBox() : Left(0) {}
};

// ===========================================================================
//  TfPassword -- non-VCL facade (golden Password.h:11-99,
//  TfPassword : public TForm)
// ===========================================================================
class TfPassword
{
public:
    TfPassword();    // PORT: fields only -- golden ctor body lives in Init()
    void Init();     // golden ctor body :20-26

    // -- pages (golden h:14-17) ----------------------------------------------
    TfPasswordPageControl *PageControl1          = new TfPasswordPageControl();
    TTabSheet             *tsPassword            = new TTabSheet();
    TTabSheet             *tsEventlogLogin       = new TTabSheet();
    TTabSheet             *tsEventLogLoginModify = new TTabSheet();

    // -- classic login page (golden h:22-24, 42, 44-45) ----------------------
    TfPasswordEdit     *edPassword    = new TfPasswordEdit();
    TfPasswordEdit     *edUserName    = new TfPasswordEdit();
    TfPasswordComboBox *cbUserName    = new TfPasswordComboBox();
    TLabel             *lblUserName   = new TLabel();
    TSpeedButton       *btnDownload   = new TSpeedButton();
    TLabel             *lblPWDownload = new TLabel();

    // -- EventLog login / modify pages (golden h:27-34) ----------------------
    TComboBox *cob_UseIDList       = new TComboBox();
    TEdit     *edPasswordPassWord  = new TEdit();
    TComboBox *cob_ModifyUseIDList = new TComboBox();
    TEdit     *edModifyPassword    = new TEdit();
    TEdit     *edReModifyPassword  = new TEdit();

    // -- form-level fields (see DEVIATION (D-5)) -----------------------------
    int Left   = 508;   // .dfm Left
    int Top    = 379;   // .dfm Top
    int Width  = 617;   // .dfm Width  (READ by FormShow's centering math)
    int Height = 336;   // .dfm Height (READ by FormShow's centering math)

    void ShowModal() {}  // golden TForm::ShowModal -- offline no-op (see NULL-GLOBAL note)
    void Close()     { Visible=false; }  // golden TForm::Close -- offline no-op + Visible drop (fTemp_Set.h precedent)
    // AI(W906-FW-QWKEY4) 20260824: golden TForm::Visible, added for its single
    //   measured consumer (SECSGEM/uHGemHT9045.cpp G23 `fPassword->Visible`).
    //   Headless it stays false (ShowModal is an instant-submit no-op that
    //   never raises it), so G23's auto-close branch is faithfully unreached.
    bool Visible = false;

    // -- golden private: User declarations (h:65-73), public collapse (D-4) --
    int  iNowEvenLogUserLevel = 0;      //帳號帶入,下拉是選單顯示用 -- ZeroInitVclFields: golden never initializes it
    bool bLoginSuccess        = false;  // ZeroInitVclFields: golden never initializes it
    void CheckPassword();               // golden :319-336
    void ReadPasswordFile();            // golden :348-369 -- GATE (P-R1) inside
    void SavePasswordFile();            // golden :371-381 -- GATE (P-S1)

    // -- golden public: User declarations (h:75-98) ---------------------------
    bool bShow = false;

    //Steven 20181224 : For ASE-CL
    //==>
    enum EvenLog_Level                        //enum 型別
    {
        OPERATOR            = 0,
        ENG_VENDOR          = 1,
        ENG_MI              = 2,
        PM                  = 3,
        AUDIT               = 4,
        ADMIN               = 5
    };

    bool bShowTab = false;              // golden bool written 0/1 -- GOLDEN (G-P2)
    void SetUserLevel(int iLevel);      // golden :314-317   帳號帶入,下拉是選單顯示用
    bool CheckLoginSuccess();           // golden :338-341   檢查登入是否成功
    int  GetLoginLevel();               // golden :343-346   目前登入權限
    void ShowEventLogLogin();           // golden :28-32
    //<==
    //Steven 20181224 : For ASE-CL

    // -- event handlers (parameter lists per DEVIATION (D-3)) ----------------
    void FormShow();                                 // golden :34-116
    void SpeedButton1Click();                        // golden :118-121 -- DFM-ORPHANED (G-P3)
    void spbCancelClick();                           // golden :123-128
    void edUserNameKeyDown(unsigned short &Key);     // golden :130-163 -- GATE (P-B1)
    void edPasswordKeyDown(unsigned short &Key);     // golden :165-184 -- GATE (P-B2)
    void FormClose();                                // golden :186-207
    void edPasswordMouseDown();                      // golden :209-232 -- GATE (P-B3)
    void edUserNameMouseDown();                      // golden :234-263 -- GATE (P-B4)
    void cbUserNameChange();                         // golden :265-269
    void sbPasswordCancelClick();                    // golden :273-276
    void sbPasswordOKClick();                        // golden :278-282
    void sbPasswordModifyOKClick();                  // golden :284-297 -- ROOT Password.cpp
    void sbPasswordModifyClick();                    // golden :299-312
    void btnOKClick();                               // golden :385-388
    void btnDownloadClick();                         // golden :390-397 -- GATE (P-D1)
};

// ---------------------------------------------------------------------------
//Steven 20181224 : For ASE-CL
//==>
// golden Password.h:103-109 verbatim (name unclaimed tree-wide, verified
// this wave -- see banner).  Defined in forms/fPassword.cpp.
typedef struct
{
    AnsiString ID[30];
    AnsiString PassWord[30];
    //int  Level[30];
}EvenLog_PASS_WORD;
extern EvenLog_PASS_WORD EvenLogUSER;
//<==
//Steven 20181224 : For ASE-CL
// ---------------------------------------------------------------------------
// AI(W906-FW-QWKEY1) 20260824: golden Password.h:113-114 `extern PACKAGE
// TfPassword *fPassword; / *fPassword2;` -- PACKAGE glue dropped.  Defined
// (zero-initialized, NEVER constructed at static init) in forms/fPassword.cpp.
extern TfPassword *fPassword;
extern TfPassword *fPassword2;       //Steven 20150505 : 剛好在輸入時,Alarm會卡死

#endif // FORMS_FPASSWORD_H
