// =============================================================================
//  handlerlog.h  --  public interface of TMyLog (the generic form-change
//                    logger) plus the minimal TWinControl/TForm/TScrollBar/
//                    TTrackBar stand-ins it needs.
//
//  Faithful translation of golden handlerlog.h (40 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3, group "misc_a".
//
//  ROLE: TMyLog walks a VCL control tree (a form, or any control on one),
//  snapshotting every leaf control's displayed value (Init_Log), then later
//  re-walking the same tree and diffing each leaf against its snapshot
//  (Comp_Log/Do_Log), writing one line per changed control to a dated
//  ChangeLog ini file (Compare_Diff/Save_Log) and, when IniConfig.
//  bSPILFunction is off, also into the machine's own EventLog
//  (slEventLog/SaveEventLogInfo).  Save_SiteStatusLog is a SEPARATE,
//  special-purpose sibling that snapshots the Site On/Off + Site-Mapping +
//  Shuttle-Select + Use-Suck-Mode state directly (no control-tree walk) and
//  is the one ACTIVE call site already wired elsewhere in this tree's golden
//  ancestor (golden cSetUp.cpp:4096 `myLog.Save_SiteStatusLog();`, not in
//  this wave's scope to re-wire).  Bodies: handlerlog.cpp -- read its file
//  head for the full gate register.
//
//  KNOWN, DELIBERATE, PRE-EXISTING DEFERRAL THIS WAVE RESOLVES PARTIALLY
//  -----------------------------------------------------------------------
//  cmydef.h:12 already documents this exact dependency as deferred to a
//  future "W7" UI wave: "forward-decls (defs deferred): TMyLog->W7 (VCL
//  TWinControl/TObject)", and cmydef.cpp:3479-3481 keeps the golden global
//  instance (`TMyLog myLog;`) inside `#if 0` for the SAME reason. This wave's
//  brief assigns handlerlog.cpp itself (not the cmydef.cpp global, which is a
//  pre-existing file this wave may not touch) to group "misc_a", so this file
//  supplies a REAL TMyLog class definition -- but the situation cmydef.h's
//  note warned about is real: golden's TWinControl-tree walk needs a
//  ->Name property on EVERY leaf control type, and vclcompat/Controls.h (the
//  tree's established, pre-existing, out-of-this-wave's-write-scope stock-
//  widget file) deliberately carries a "minimal surface" that does NOT
//  include ->Name on ANY of its stock types (TRadioGroup/TCheckBox/
//  TRadioButton/TComboBox/TCustomEdit/...) -- see that file's own SCOPE
//  BOUNDARY / MEASURED PROPERTY COVERAGE notes. See handlerlog.cpp's own
//  banner for exactly which statements this forces to be gated (NOT whole
//  functions -- only the ->Name-keyed map-write/Compare_Diff call sites).
//
//  FOUR NEW STAND-IN TYPES (NOT in vclcompat/Controls.h -- see above)
//  -----------------------------------------------------------------------
//  golden TWinControl/TForm/TScrollBar/TTrackBar have NO port anywhere in
//  this tree (grepped tree-wide 20260807: zero real `class TWinControl`/
//  `class TForm`/`class TScrollBar`/`class TTrackBar` definitions; the only
//  hits are comments and the cmydef.h forward-decl note quoted above). This
//  file is the FIRST ported consumer that needs generic control-tree
//  reflection (->Parent/->ControlCount/->Controls[i]) rather than a single
//  stock leaf widget, so -- per the same "add it where the first real
//  consumer needs it" precedent vclcompat/Controls.h's own banner describes
//  for ITS OWN creation (originally 6 types for one SECSGEM consumer, later
//  expanded) -- four minimal, real, NEW types are defined below, local to
//  this header, deriving from the EXISTING `vclcompat::TControl`/`TObject`
//  (vclcompat/Controls.h / vclcompat/TStringList.h) so `dynamic_cast`
//  between them and every existing stock widget type stays well-formed
//  (single inheritance chain, real vtable via TObject's virtual dtor).
//  A future wave may promote these into vclcompat/Controls.h proper if a
//  second consumer appears; not done here (rule 7: no editing pre-existing
//  files).
//    * TWinControl : TControl  -- adds Name / Parent / ControlCount /
//      Controls (a real std::vector<TControl*>, so `Controls[i]` keeps
//      golden's own `[]` syntax with no proxy). ControlCount defaults 0 and
//      Controls stays empty: with ZERO real TWinControl instances anywhere
//      in the object graph this wave (no form is wired up), every walk this
//      type could drive is offline-correct dead code today -- exactly the
//      same "unreachable dead code" posture vclcompat/Controls.h's own
//      TRadioGroup::Items(NULL) note already establishes for the identical
//      situation.
//    * TForm : TWinControl -- golden dynamic_casts to this to find the
//      enclosing form's ->Name (Find_Parent). No extra members.
//    * TScrollBar / TTrackBar : TControl -- golden TScrollBar/TTrackBar
//      (Control_Log_Value only ever reads ->Position off either).
//
//  VCL/Borland conversions: golden's #include block (Classes.hpp/
//  Controls.hpp/StdCtrls.hpp/Forms.hpp/Buttons.hpp/Dialogs.hpp/ExtCtrls.hpp/
//  ComCtrls.hpp/vcl.h/Filectrl.hpp) collapses to the vclcompat umbrella +
//  vclcompat/Controls.h (stock widgets) + this header's own 4 new types.
//  golden :11 `#include <stdio.h>` kept (FILE*/fopen/fputs/fclose used by
//  Save_Log/Save_SiteStatusLog/Save_AnyName in the .cpp).  No __fastcall/
//  __property/__published in the translated declarations (golden :30
//  `_fastcall TMyLog::TMyLog();` -- note golden's OWN typo, missing the
//  leading `__`; BCB6 tolerates `_fastcall` as a no-op identifier here since
//  it is inside a class body with no matching out-of-line qualifier grammar
//  rule triggered -- dropped the same as every other __fastcall in this
//  tree).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#ifndef handlerlogH
#define handlerlogH

#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W3) 20260807: umbrella (AnsiString/TStringList/...); was <Classes.hpp>+<vcl.h>+...
#include "vclcompat/Controls.h"     //AI(W906-PT-W3) 20260807: stock widget stand-ins (TControl/TCheckBox/TRadioButton/TComboBox/TCustomEdit/TEdit/TLabeledEdit/TRadioGroup) -- was <Controls.hpp>/<StdCtrls.hpp>/<ComCtrls.hpp>
#include <stdio.h>
#include <map>
#include <vector>

//AI(W906-PT-W3) 20260807: FOUR NEW stand-in types -- see file-head banner for
//  the full rationale (no port anywhere else in the tree; local to this
//  header, not vclcompat/Controls.h, per rule 7).
class TWinControl : public TControl
{
public:
    AnsiString Name;
    TWinControl *Parent;
    int ControlCount;
    std::vector<TControl*> Controls;
    TWinControl() : Parent(NULL), ControlCount(0) {}
    virtual ~TWinControl() {}
};

class TForm : public TWinControl
{
public:
    virtual ~TForm() {}
};

// golden TScrollBar -- Control_Log_Value only ever reads ->Position off it.
class TScrollBar : public TControl
{
public:
    int Position;
    TScrollBar() : Position(0) {}
    virtual ~TScrollBar() {}
};

// golden TTrackBar -- Control_Log_Value only ever reads ->Position off it.
class TTrackBar : public TControl
{
public:
    int Position;
    TTrackBar() : Position(0) {}
    virtual ~TTrackBar() {}
};

class TMyLog
{
    private:
        std::vector<AnsiString> loglist;                                        //vector to save all changes
        std::map <AnsiString, AnsiString>   mapLogValue;                        //Map structure for saving all values
        AnsiString fName;
        AnsiString sUser;
        AnsiString sFolder;
        void Control_Log_Value(TWinControl *PCtrl,bool bInit);
        void Compare_Diff(AnsiString sFind, AnsiString sValue, AnsiString sCaption="");
        void Save_Log();
        void Find_Parent(TObject *PCtrl);

    protected:
    public:

        TMyLog();
        AnsiString Delimiter;
        bool Do_Log(TObject *PCtrl, AnsiString  tmpUser, AnsiString tmpFolder="");
        void Init_Log(TObject *PCtrl, AnsiString  tmpUser, AnsiString tmpFolder="");
        void Comp_Log(TObject *PCtrl);
        void Save_SiteStatusLog();                                              //JerryYang 20160617 記錄開關Site的狀態
        void Save_AnyName(AnsiString ComponentName,AnsiString AData);           //kevin 20160728 記錄狀態
};
#endif
