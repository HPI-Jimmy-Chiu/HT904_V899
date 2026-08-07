// =============================================================================
//  Public/HTEdit.h  --  THTEdit : the single "edit-list item" binding one VCL
//  widget (TEdit/TComboBox/TRadioGroup/TCheckBox/TDateTimePicker/...) to one
//  machine parameter (int*/double*/AnsiString*/bool*) plus its ini group/key
//  and min/max limits.
//
//  Faithful translation of golden Public/HTEdit.h (167 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("hteditlist" group).  NOT in the originally
//  assigned file list (only Public/HTEditList.cpp was named) -- ported here
//  anyway because golden Public/HTEditList.h #includes "HTEdit.h" and uses
//  THTEdit/TEditContent/TEditUnit as hard, load-bearing types (TList<THTEdit*>,
//  SetItemLimit(THTEdit*,...)).  HTEditList.h/.cpp cannot be made to typecheck,
//  let alone behave faithfully, without a real THTEdit.  See
//  Public/HTEditList.cpp's own banner for the full accounting of this decision
//  and its integration consequences.
//
//  ROLE: golden's `__property` wrapper around one widget + one parameter
//  pointer, with format-aware min/max clamping (positive/negative/percent/port/
//  bool) and (in real VCL) small-keypad popup + on-screen-keyboard wiring.
//
//  WAVE SCOPE -- ACTIVE vs GATED (see the .cpp for the full function list with
//  golden line numbers):
//    ACTIVE, real bodies: ctor/dtor, SetFileInformation, SetContentType,
//      SetDefaultValue, SetCheckRange, SetIniGroupName, SetIniKeyName,
//      ChangeProperty(x2), SetMax/SetMin/SetCustomMax/SetCustomMin/
//      SetDefaultLimitation/SetCustomLimitation (the real clamp-math engine),
//      ReadParamter/SetParamter, EditIntKeyPress/EditFloatKeyPress/
//      EditPathKeyPress (golden bodies are themselves empty -- verbatim).
//    GATED (documented no-op / safe-default stubs; live-widget-window glue
//      with nothing in this offline substrate to bind to): OnWriteEdit,
//      DisableEventOverlap, EditClick, EditChange, GetDefaultPosition,
//      SetToDefaultPosition. See the .cpp GATE REGISTER for each, with golden
//      line citations and the behaviour delta.
//
//  __property -> PLAIN PUBLIC FIELD, hand-translated per vclcompat/vcl_compat.h's
//  own instruction ("any surviving __property ...; line should be hand-
//  translated to a getter/setter"). Concretely:
//    * IniGroupName/IniKeyName/Content/DefaultValue/CheckRange/CustomMaxValue/
//      CustomMinValue -- golden's property WRITE side (SetIniGroupName/
//      SetIniKeyName/SetContentType/SetDefaultValue/SetCheckRange) is a bare
//      one-line assignment with NO side effect beyond storing the value, so
//      these are exposed as plain public fields: `Item->Content=ECText;`
//      compiles and behaves identically to golden's
//      `__property Content={read=HTContentType,write=SetContentType}`.
//    * MaxValue/MinValue -- golden's SetMax/SetMin DO have real per-content-type
//      clamp logic (ECPosInt floors at 0, ECPort ceils at 65535, ECBool forces
//      0/1, ...), so a bare field would silently drop that behaviour. These stay
//      plain public fields for the READ side (`atof(Temp->MaxValue.c_str())`
//      compiles unchanged), but every WRITE golden expressed as
//      `Item->MaxValue=X;` is translated to the explicit call `Item->SetMax(X);`
//      at the call site (Public/HTEditList.cpp -- SetItemLimit and Add()) --
//      SetMax/SetMin are elevated from golden's private property-setter to a
//      public method for exactly this purpose. This is NOT a behaviour change:
//      golden's `__property ... write=SetMax` means every property write
//      already called SetMax(); this just makes that call spelled out instead
//      of implicit.
//  WHY NOT a proxy/property-object type (the alternative "keep `Item->MaxValue=
//  X` as literal assignment syntax" route): vclcompat::AnsiString::sprintf's
//  variadic `conv(Args...)` step has BOTH a non-template `conv(const
//  AnsiString&)` overload AND a `template<T> conv(T)` catch-all. For a plain
//  AnsiString argument both are exact matches and the non-template wins (the
//  standard tie-break), which is why raw AnsiString fields pass through
//  `.sprintf("%s",...)` correctly tree-wide. A WRAPPER/proxy type (even one
//  publicly deriving from AnsiString) is a *different* type: binding it to
//  `conv(const AnsiString&)` needs a derived-to-base reference conversion,
//  which ranks WORSE than the template's exact-match deduction on the wrapper
//  type itself -- so the template would win, and `formatString`'s C `...`
//  would receive a raw non-POD class object instead of a `const char*`, which
//  is at best a redundant-currently-broken build and at worst UB. Plain fields
//  sidestep this trap entirely; verified by grep that Temp->MaxValue/MinValue/
//  CustomMaxValue/CustomMinValue are never passed directly into a
//  sprintf/printf/cat_printf call anywhere in golden HTEditList.cpp (always via
//  atoi()/atof() first).
//
//  MINIMAL TU-LOCAL STAND-INS (NOT added to vclcompat/Controls.h -- same
//  documented pattern as language.h's TWinControl/TForm and
//  aHotPlateSubstrate.h's TList: "types missing from Controls.h get a
//  TU-visible stand-in in the file that first needs them; Controls.h itself is
//  not touched"):
//    * TWinControl -- golden THTEdit::Par is `TWinControl *Par;` (GetDefaultPosition/
//      SetToDefaultPosition, both GATED below -- see GATE REGISTER). Carries
//      only ->Name (AnsiString), matching language.h's own TWinControl shape
//      byte-for-byte. KNOWN DUPLICATE-DEFINITION HAZARD: language.h ALREADY
//      defines a global `class TWinControl : public TControl {AnsiString Name;
//      ...}` (verified identical shape). Nothing currently #includes both
//      language.h and this header in the same TU, so there is no ACTIVE
//      collision today: if a future TU ever does, it is a hard redefinition
//      compile error that integration must resolve by promoting ONE shared
//      TWinControl (most naturally into vclcompat/Controls.h) -- not something
//      this file can fix without editing language.h, which is out of scope.
//    * TNotifyEvent / TKeyPressEvent -- real VCL's `__closure` bound-method
//      pointer types. vclcompat/vcl_compat.h's own comment says such needs
//      should become "a std::function-style typedef"; these are simplified to
//      plain free-function pointers because the ONLY use here is storing/
//      restoring a PREVIOUS handler (FOldOnClickEvent/FOldKeyPressEvent/
//      FOldOnChange) that this offline substrate can never observe being set in
//      the first place (see the OnWriteEdit gate) -- a full closure type would
//      be strictly more machinery for zero additional behaviour.
//  Verified absent from the target tree before adding (grepped
//  TDateTimePicker/TPopupMenu/TWinControl/TKeyPressEvent/TNotifyEvent/Variant
//  over every ported .h outside build*/ -- only TWinControl pre-exists, in
//  language.h, as noted above).
// =============================================================================
#ifndef HTEditH
#define HTEditH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"
#include "vclcompat/Controls.h"

//---------------------------------------------------------------------------
enum TEditContent                                                               // golden HTEdit.h:13-27, verbatim
{
    ECText=0,           //純文字
    ECInteger=1,        //數字
    ECDouble=2,         //浮點數
    ECPosInt=3,         //正整數
    ECPosDouble=4,      //正浮點數
    ECNegInt=5,         //負數
    ECNegDouble=6,      //負浮點數
    ECFileName=7,       //檔案名稱或路徑
    ECPassword=8,       //密碼
    ECPercent=9,        //百分比
    ECPort=10,          //通訊埠
    ECIPAddr=11,        //IP
    ECBool=12           //對還錯       //Steven 20230224 : 加入支援對還錯
};
enum TEditUnit                                                                  //Steven 20230905 : HTEditList加入型態轉換的Flag
{
    EUNone      =0,     //不做任何處理
    EUuMToMM    =1,     //顯示為MM, 存檔為uM
    EUMSToSec   =2      //顯示為Sec, 存檔為MS
};

//---------------------------------------------------------------------------
//  TWinControl -- TU-local minimal stand-in.  See file-head note: KNOWN
//  duplicate-definition hazard vs language.h's own TWinControl (identical
//  shape, not currently co-included anywhere).
//---------------------------------------------------------------------------
class TWinControl : public TControl
{
public:
    AnsiString Name;                  // golden TComponent->Name (the one member GetDefaultPosition/SetToDefaultPosition would read/write if live)
    virtual ~TWinControl() {}
};

// golden `__closure`-based event types -- see file-head note.  Never bound to
// a live widget field anywhere in this substrate (vclcompat's TCustomEdit/
// TEdit/TComboBox/TRadioGroup/TCheckBox carry no OnClick/OnChange/OnKeyPress
// member at all) -- these exist purely so THTEdit's FOldOnClickEvent/
// FOldKeyPressEvent/FOldOnChange fields (golden HTEdit.h private section)
// declare and NULL-compare correctly.
typedef void (*TNotifyEvent)(TObject *Sender);
typedef void (*TKeyPressEvent)(TObject *Sender, char &Key);

// golden VCL `Variant` -- no vclcompat equivalent (no consumer needs one
// tree-wide today). FParemeter/vParameter are write-only scratch storage: grep
// confirms nothing anywhere reads THTEdit::vParameter, and FParemeter is only
// ever round-tripped by ReadParamter/SetParamter (themselves uncalled outside
// this class). AnsiString is a safe, minimal stand-in that preserves the
// storage-only shape without inventing a Variant type for zero behavioural
// gain.
typedef AnsiString Variant;

//---------------------------------------------------------------------------
class THTEdit : public TObject                                                  // golden: "THTEdit:TObject //: public TEdit" -- the commented-out TEdit base was never live in golden either
{
private:
    Variant         FParemeter;
    char           *DisablePopupMenu;   // GATE: golden `TPopupMenu *DisablePopupMenu;` -- see .cpp GATE REGISTER. `char*` (not `void*`) so the dtor's NULL-guarded `delete` stays well-formed standard C++; always NULL in this substrate (see ctor).
public:
    // ---- former __property backing fields, now the properties themselves ---
    // (plain public fields; see file-head note for which ones also get a
    //  same-named public SETTER METHOD that callers must invoke explicitly)
    AnsiString      IniGroupName;
    AnsiString      IniKeyName;
    TEditContent    Content;
    AnsiString      MaxValue;            // WRITE via SetMax(AnsiString) -- see file-head note
    AnsiString      MinValue;            // WRITE via SetMin(AnsiString) -- see file-head note
    AnsiString      CustomMaxValue;      // WRITE via SetCustomMax(AnsiString) or the Set*Limitation methods
    AnsiString      CustomMinValue;      // WRITE via SetCustomMin(AnsiString) or the Set*Limitation methods
    AnsiString      DefaultValue;
    bool            CheckRange;

private:
    //事件處理-----------
    TNotifyEvent    FOldOnClickEvent;
    TKeyPressEvent  FOldKeyPressEvent;
    TNotifyEvent    FOldOnChange;

public:
    THTEdit();
    ~THTEdit();
    void SetDefaultLimitation(AnsiString Max, AnsiString Min);
    void SetCustomLimitation(AnsiString Max, AnsiString Min);
    void SetFileInformation(AnsiString GroupName, AnsiString KeyName);
    void ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, AnsiString Default);
    void ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool Default);

    // former private property setters -- elevated to public methods so
    // HTEditList.cpp can call them explicitly at every former property-write
    // site (see file-head note). Names kept verbatim from golden.
    void SetContentType(TEditContent P);
    void SetDefaultValue(AnsiString P);
    void SetCheckRange(bool P);
    void SetIniGroupName(AnsiString P);
    void SetIniKeyName(AnsiString P);
    void SetMax(AnsiString Max);
    void SetMin(AnsiString Min);
    void SetCustomMax(AnsiString Max);
    void SetCustomMin(AnsiString Min);
    Variant ReadParamter();
    void SetParamter(Variant VData);

    void EditIntKeyPress(TObject *Sender, char &Key);
    void EditFloatKeyPress(TObject *Sender, char &Key);
    void EditPathKeyPress(TObject *Sender, char &Key);
    void EditClick(TObject *Sender);
    void EditChange(TObject *Sender);
    void OnWriteEdit(TCustomEdit *SourceEdit);      //Steven 20230224 : Edit List改支援多型態元件 TEdit --> TCustomEdit
    void DisableEventOverlap();  //取消覆蓋原本的Event,也就是使用Edit本身的OnClick與OnChange Event

    TControl    *SourceControl;         //Steven 20230224 : Edit List改支援多型態元件 TEdit --> TControl
    bool        *bParameter;            //Steven 20230502 : 修正bool沒有跟著存讀檔
    int         *iParameter;
    double      *dParameter;
    AnsiString  *sParameter;
    Variant     vParameter;
    TWinControl *Par;
    int  iTag;
    bool bEnable;
    bool bVisible;
    bool bReadFromFile;         //是否從檔案讀取
    int  iBarcodeReadType;      //是否要做Barcode檢查判斷
    int  iTransformType;        //是否使用指定轉換 TEditUnit
    int iTop;
    int iLeft;
    int iHeight;
    int iWidth;
    void GetDefaultPosition();
    void SetToDefaultPosition();
};
//---------------------------------------------------------------------------
#endif
