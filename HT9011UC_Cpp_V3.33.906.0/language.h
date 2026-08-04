//---------------------------------------------------------------------------
//  language.h -- TfLan headless translation (V906 port)
//
//  Translation wave: W906-GA1-B5 (GA-1-B5: language.cpp -- 語系載入 / language
//  pack loading)
//  Translator: AI(W906-GA1-B5) 20260804
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/language.h (47 lines) +
//                 language.cpp (746 lines), both decoded via Python
//                 open(path,'rb').read().decode('cp950','replace') per this
//                 task's reading rule -- 0 U+FFFD on either file.
//
//  WHAT THIS FILE IS
//  ------------------
//  Golden `class TfLan : public TForm` (language.h:13-41) is the language-pack
//  form. It owns the CSV-backed Name->Caption/Hint translation table
//  (LanList/tLanList/mapLanList) and both directions of the language feature:
//    SAVE   -- walk the live UI, write default English/Chinese rows to
//              Language.csv (UpdateLanguageToStrList/SearchCompomentForSave).
//    APPLY  -- walk the live UI, overwrite each control's Caption/Hint from
//              the table (ChangeLanguage/LoadCompomentForChangeLan).
//    QUERY  -- the CSV load + Name->row lookup engine used by both directions
//              (LoadLanguageFromFile/GotLanString/UpdateCompomentToStrList).
//
//  Per this wave's brief, translated HEADLESS: TfLan is a plain class with NO
//  TForm base (matches forms/fMain.h's TfMain / SECSGEM/uHGemEquipment.h's
//  THGem precedent -- see those files' own headers: no VCL window is modeled
//  in this port, so a facade/binder base buys nothing here). `__fastcall` is
//  dropped tree-wide (see SECSGEM/uHGemEquipment.h's own "__fastcall dropped,
//  project convention" citations) -- not spelled out per-method below.
//
//  CORE vs GATED (see language.cpp's file-head note for the full function-by-
//  function tally and golden-line citations):
//    CORE, translated FULLY FAITHFUL (byte-for-byte control flow) --
//      ctor, ~TfLan/FormDestroy, LoadLanguageFromFile, UpdateCompomentToStrList,
//      IsMainFormNotChange, UpdateLanguageToStrList, GotLanString,
//      ChangeLanguage. This is the CSV-load + key-lookup engine
//      (bUseLanguageCSV / mapLanList) the task brief calls out as the value
//      that MUST be faithful.
//    GATED (documented no-op stubs; pure UI glue / display handlers per the
//    task brief's explicit carve-out) --
//      SearchCompomentForSave, LoadCompomentForChangeLan (both walk a live
//      TWinControl tree via ->ControlCount/->Controls[i], which this headless
//      port does not model -- same boundary forms/fMain.h draws for every
//      other form), plus the 4 free Auto-Form-Size helpers (FormShowPos/
//      FormChangeSize/FormReSize/FormChangeAction) declared/defined for
//      linkage-compatibility with golden's extern surface only.
//
//  MINIMAL TU-LOCAL STAND-INS (NOT added to vclcompat/Controls.h -- disclosed
//  per this wave's brief: "types missing from Controls.h get a TU-visible
//  stand-in in this file; Controls.h itself is not touched"):
//    TWinControl / TForm -- golden TfLan derives from TForm, and 4 of its
//    methods take TWinControl*/TForm* parameters. vclcompat/Controls.h
//    deliberately has no TWinControl/TForm (see that file's SCOPE BOUNDARY
//    notes: no live widget HIERARCHY/window model exists anywhere in this
//    port yet). Because every method that takes a TWinControl*/TForm* here is
//    EITHER gated (SearchCompomentForSave/LoadCompomentForChangeLan, whose
//    bodies never dereference the pointer) OR only ever reads ->Name
//    (UpdateLanguageToStrList/ChangeLanguage), these stand-ins carry ONLY
//    ->Name (AnsiString) on top of the real vclcompat::TControl base
//    (Visible/Enabled/hCtl) for shape-parity with the rest of the tree --
//    no Controls[]/ControlCount is declared because nothing here would ever
//    dereference it. A future wave that needs a real widget-tree walk should
//    grow vclcompat/Controls.h (or a dedicated forms/ facade) instead of this
//    file's private stand-in.
// ---------------------------------------------------------------------------
#ifndef languageH
#define languageH

#include "vclcompat/vcl_compat.h"     // AnsiString, TObject, FileExists
#include "vclcompat/Controls.h"       // TControl (base for the TWinControl/TForm stand-ins below)
#include <map>

// ---------------------------------------------------------------------------
//  TWinControl / TForm -- TU-local minimal stand-ins, see file-head note.
//  NOT full VCL: no Controls[]/ControlCount (nothing translated here walks a
//  widget tree; the two methods that golden gives these parameter types are
//  both GATED, see language.cpp).
// ---------------------------------------------------------------------------
class TWinControl : public TControl
{
public:
    AnsiString Name;                  // golden TComponent->Name (the one member UpdateLanguageToStrList/ChangeLanguage read)
    virtual ~TWinControl() {}
};

class TForm : public TWinControl
{
public:
    virtual ~TForm() {}
};

//---------------------------------------------------------------------------
//  TfLan -- golden language.h:13-41 (class TfLan : public TForm). Headless
//  here: no TForm base, no TComponent* Owner ctor parameter (never referenced
//  in golden's own ctor body either -- see language.cpp), plain (non-virtual)
//  methods matching SECSGEM/uHGemEquipment.h's THGem convention (a logic
//  engine, not a forms/ facade binder -- no MFC subclass is planned for this
//  wave's scope).
//---------------------------------------------------------------------------
class TfLan
{
private:                                                                        // User declarations
    TStringList *LanList;
    TStringList *tLanList;
    std::map<AnsiString, AnsiString> mapLanList;
    std::map<AnsiString, AnsiString>::iterator mapLanIter;

public:                                                                         // User declarations
    TfLan();
    // AI(W906-GA1-B5) 20260804: golden has no explicit TForm-level destructor
    // -- VCL's TComponent destroy chain fires the OnDestroy event (FormDestroy,
    // below) before the object's memory is actually freed. There is no such
    // framework here, so ~TfLan() reproduces that ordering directly by calling
    // FormDestroy(0) itself. FormDestroy stays a directly-callable public
    // method (matches golden's __published shape exactly), but callers must
    // NOT also call it manually before letting the object destruct -- it frees
    // LanList/tLanList and is not idempotent, exactly as in golden.
    ~TfLan();
    TfLan(const TfLan&) = delete;
    TfLan& operator=(const TfLan&) = delete;

    void FormDestroy(TObject *Sender);                                          // golden __published void __fastcall FormDestroy(TObject *Sender);

    void LoadLanguageFromFile();

    void UpdateLanguageToStrList(TForm *P);                                     //決定要被更新的對象
    void SearchCompomentForSave(TWinControl *PCtrl);                            //搜尋畫面中的元件 -- GATED, see language.cpp
    bool IsMainFormNotChange(AnsiString S);                                     //主畫面的例外清單
    bool UpdateCompomentToStrList(AnsiString Name, AnsiString Cap);             //將元件資料存入陣列

    void ChangeLanguage(TForm *P);                                              //決定要被更新的對象
    void LoadCompomentForChangeLan(TWinControl *PCtrl);                         //載入要被更新的對象 -- GATED, see language.cpp
    bool GotLanString(AnsiString Name, AnsiString *CapStr, AnsiString *HintStr);//回傳對應的字串

    AnsiString LanguageFormName;
    bool ShowHint;
    AnsiString TableName;
    AnsiString FullFileName;
    bool bUseLanguageCSV;
};
//---------------------------------------------------------------------------
extern TfLan *fLan;                                                             // golden: extern PACKAGE TfLan *fLan;  (PACKAGE dropped -- BCB6 DLL-export macro, no meaning in this port)

extern void FormChangeAction();                                                 // 20230601 Joseph , Auto Form Size // -- GATED, see language.cpp
//---------------------------------------------------------------------------
#endif
