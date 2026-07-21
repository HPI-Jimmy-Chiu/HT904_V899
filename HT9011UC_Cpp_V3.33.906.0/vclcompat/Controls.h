// ===========================================================================
//  vclcompat/Controls.h
//  BCB6 VCL widget compatibility STAND-INS -- SECSGEM cast-dispatch targets
//  ONLY. This is deliberately NOT a general-purpose VCL Controls.hpp shim.
//
//  AI(W906-VCW1) 20260721: created for SECSGEM's SetECValue/DataItemOutSV/
//  DataItemOutEC/GetECDataValue dynamic_cast dispatch cascade (golden
//  uHGemClass.cpp:2691-2782 [SetECValue], uHGemEquipment.cpp:2513-2585/
//  2882-2957 [DataItemOutSV/DataItemOutEC], SecsSvEcRegistration.cpp:624-701
//  [GetECDataValue -- this same wave also un-gated its IsVCL==1 cast branch,
//  see that file's own note]). Golden
//  dynamic_casts a stored `TObject*` (an EC/SV's registered "VCL pointer",
//  VCL_NAME=="1") against TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/
//  TRadioGroup/TStringList in that fixed order, first match wins.
//  TStringList already exists (vclcompat/TStringList.h, and this wave's own
//  R1 gave it the `: public TObject` base the cast needs -- see that file's
//  own note); these are the other 6.
//
//  SCOPE BOUNDARY -- READ BEFORE ASSUMING ANY REAL EC/SV IS NOW LIVE
//  -------------------------------------------------------------------------
//  Every one of the ~1740 real SetECDataPointer/SetSVDataPointer call sites
//  (uHGemHT9045_EC.cpp/_SV.cpp, still almost entirely untranslated -- only 7
//  of 28 real forms have ANY translated presence today) registers a widget
//  pointer belonging to a REAL VCL form (a real fPanel->Caption, edtXxx,
//  cbxXxx, ...). None of those forms exist in this tree yet, so NOTHING
//  anywhere in the object graph today constructs one of these 6 stand-in
//  types. Every dynamic_cast against them therefore, correctly and safely,
//  returns nullptr for every currently-registered EC/SV -- exactly the same
//  "no real instances yet" situation SecsSvEcRegistration.cpp's own
//  GetECDataValue IsVCL==1 branch (also un-gated this wave) exercises. This
//  header makes the
//  CAST-DISPATCH CODE compile and dispatch correctly; it does NOT wire up any
//  real widget-backed EC/SV, and must not be described as doing so.
//
//  MINIMAL SURFACE
//  -------------------------------------------------------------------------
//  Each class carries ONLY the exact member(s) golden's 3 cast-dispatch
//  bodies above actually touch (verified by direct read of golden
//  uHGemClass.cpp:2682-2882 + uHGemEquipment.cpp:2472-3112, both read in full
//  before writing this file) -- no VCL event/paint/parent-window machinery,
//  no unrelated properties a real TPanel/TComboBox/etc. would also have.
// ===========================================================================
#ifndef VCLCOMPAT_CONTROLS_H
#define VCLCOMPAT_CONTROLS_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"

namespace vclcompat {

// golden TPanel -- only ->Caption is touched: write in SetECValue
// (`PanelPtr->Caption=S;`), read in DataItemOutSV/DataItemOutEC
// (`VCLStr=PanelPtr->Caption;`).
class TPanel : public TObject {
public:
    AnsiString Caption;
};

// golden TCustomEdit -- only ->Text (write/read), identical shape to
// TPanel::Caption above.
class TCustomEdit : public TObject {
public:
    AnsiString Text;
};

// golden TComboBox -- ->Text (ASCII-typed ECs) or ->ItemIndex (every other
// type) on the write side (SetECValue: "Text=S (ASCII) else
// ItemIndex=atoi(S)", golden :2748-2751); the same two members are read back
// symmetrically on the DataItemOutSV/DataItemOutEC side (golden :2547-2550).
// golden's ->Items (a TStrings*) is NEVER touched for TComboBox in any of
// these bodies (only TRadioGroup's ->Items is, below) -- deliberately NOT
// added here per this header's minimal-surface rule.
class TComboBox : public TObject {
public:
    AnsiString Text;
    int ItemIndex;
    TComboBox() : ItemIndex(0) {}
};

// golden TLabel -- only ->Caption (identical shape to TPanel).
class TLabel : public TObject {
public:
    AnsiString Caption;
};

// golden TCheckBox -- only ->Checked (bool): write
// `CheckBoxPtr->Checked=(S!="0");` (SetECValue), read
// `if(CheckBoxPtr->Checked) ...` (DataItemOutSV/DataItemOutEC).
class TCheckBox : public TObject {
public:
    bool Checked;
    TCheckBox() : Checked(false) {}
};

// golden TRadioGroup -- ->ItemIndex (int) AND ->Items (a TStrings* in real
// VCL; golden only ever reads ->Items->Count / ->Items->Strings[x] off it --
// see SetECValue's string-match-to-index loop, golden :2767-2771, and
// DataItemOutSV/DataItemOutEC's symmetric ItemIndex>=0 read branch, golden
// :2554-2560). Modeled here as a `TStringList*` -- satisfies both ->Count
// and ->Strings[i] (see vclcompat/TStringList.h) without inventing a new
// abstract-TStrings-pointer shim just for this one field. Defaults to NULL:
// with zero real TRadioGroup instances anywhere in the object graph (see
// file-head scope note above), golden's ->Items dereference is unreachable
// dead code today -- allocating a real (always-empty) TStringList here that
// no code path can ever read would be pointless heap churn, not extra safety.
class TRadioGroup : public TObject {
public:
    int ItemIndex;
    TStringList *Items;
    TRadioGroup() : ItemIndex(0), Items(0) {}
};

} // namespace vclcompat

// Brought into the global namespace (matching golden's unqualified spelling,
// and vclcompat/vcl_compat.h's own established pattern for AnsiString/
// TStringList/TObject/...) -- explicit one-by-one `using`, not `using
// namespace vclcompat;`, to avoid pulling in every other vclcompat symbol
// into the 2 translation units that include this header directly
// (SECSGEM/uHGemClass.cpp, SECSGEM/uHGemEquipment.cpp already get the full
// umbrella via vclcompat/vcl_compat.h anyway; this header is included
// separately per this wave's own instructions, header-only, zero CMake
// changes needed).
using vclcompat::TPanel;
using vclcompat::TCustomEdit;
using vclcompat::TComboBox;
using vclcompat::TLabel;
using vclcompat::TCheckBox;
using vclcompat::TRadioGroup;

#endif // VCLCOMPAT_CONTROLS_H
