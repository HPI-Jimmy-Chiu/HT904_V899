//------------------------------------------------------------------------------
//  language.cpp -- TfLan headless translation (V906 port)
//
//  Translation wave: W906-GA1-B5 (GA-1-B5: language.cpp -- 語系載入 / language
//  pack loading)
//  Translator: AI(W906-GA1-B5) 20260804
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/language.cpp (746 lines),
//                 decoded via Python open(path,'rb').read().decode('cp950',
//                 'replace') per this task's reading rule -- 0 U+FFFD.
//
//  See language.h's file-head note for the full design rationale (headless
//  class, TWinControl/TForm TU-local stand-ins, core-vs-gated split).
//
//  FUNCTION TALLY (golden has 14 functions in this TU: 1 ctor + 9 TfLan member
//  methods + 4 free functions):
//
//    CORE, translated FULLY FAITHFUL (8):
//      TfLan::TfLan (ctor)                    golden :15-28
//      TfLan::FormDestroy                     golden :30-45
//      TfLan::LoadLanguageFromFile            golden :47-86
//      TfLan::UpdateCompomentToStrList         golden :90-117
//      TfLan::IsMainFormNotChange              golden :190-261
//      TfLan::UpdateLanguageToStrList           golden :263-270
//      TfLan::GotLanString                     golden :272-326
//      TfLan::ChangeLanguage                   golden :459-464
//
//    GATED (documented no-op stubs; pure UI glue / display handlers, per this
//    wave's brief -- 6):
//      TfLan::SearchCompomentForSave           golden :119-188
//      TfLan::LoadCompomentForChangeLan         golden :328-457
//      FormShowPos                             golden :466-470
//      FormChangeSize                          golden :472-476
//      FormReSize                              golden :478-733
//      FormChangeAction                        golden :735-745
//
//  WHY THOSE 6 ARE GATED (one reason, stated once): each either walks a live
//  TWinControl tree via ->ControlCount/->Controls[i] (SearchCompomentForSave,
//  LoadCompomentForChangeLan, FormReSize), or exists only to be called by one
//  of those / to move real widget geometry (FormShowPos/FormChangeSize/
//  FormChangeAction). This headless port models no widget hierarchy anywhere
//  (see language.h's TWinControl/TForm note and vclcompat/Controls.h's own
//  SCOPE BOUNDARY) -- translating these "for real" would mean inventing a
//  widget tree that does not exist yet in this tree, which is out of this
//  wave's scope. FormReSize alone dynamic_casts across ~23 distinct golden
//  widget types (TPageControl/TEdit/TLabeledEdit/TCheckBox/TRadioButton/
//  TRadioGroup/TComboBox/TScrollBar/TImage/TALed/TMaskEdit/TPanel/TButton/
//  TSpeedButton/TLabel/TGroupBox/TScrollBox/TListBox/TCheckListBox/TBevel/
//  TDriveComboBox/TDirectoryListBox/TStringGrid/TBitBtn) purely to scale each
//  one's Top/Left/Height/Width/Font->Size -- confirming it is pure display
//  glue with zero CSV/lookup content.
//
//  TRANSLATION-INFRASTRUCTURE GOTCHA FOUND THIS WAVE (verified empirically,
//  not just reasoned about -- see this wave's report for the compile+run
//  probe): vclcompat::TStringList's Strings[i] accessor returns a StringsProxy
//  (vclcompat/TStringList.h), which converts to AnsiString via a user-defined
//  conversion operator. AnsiString::sprintf/printf/cat_printf are VARIADIC
//  TEMPLATES whose per-argument `conv()` helper is itself overloaded: a
//  non-template `conv(const AnsiString&)` plus a generic passthrough template
//  `template<typename T> T conv(T v)`. Passing a StringsProxy DIRECTLY as a
//  sprintf argument makes the compiler deduce T=StringsProxy for the generic
//  template (an EXACT match, which strictly outranks the AnsiString overload's
//  user-defined conversion) -- so the proxy's raw bytes (a pointer + an int)
//  are written into the `...` slot instead of a `const char*`, and vsnprintf's
//  "%s" then reads them as if they were a string pointer. This COMPILES
//  CLEANLY with no warning even under -Wall -Wextra -Wpedantic and silently
//  prints garbage at runtime. Golden's own idiom (`str.sprintf("%s_%s",
//  tLanList->Strings[0], tLanList->Strings[1])`, language.cpp:58, and 4 more
//  sites in GotLanString, :300-306) hits this exactly. FIX applied at every
//  site below: wrap the `->Strings[i]` argument in an explicit `(AnsiString)`
//  cast, which forces the argument's static type to be AnsiString BEFORE
//  template argument deduction runs, so `conv(const AnsiString&)` is picked
//  (now an equally-ranked exact match, and the non-template overload wins the
//  tie per [over.match.best]). No golden LOGIC changed -- sprintf("%s", X) and
//  a cast-then-sprintf both copy X's text into the destination; only the
//  argument's compile-time type is different.
//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "language.h"
#include "cprod.h"
#include "cmydef.h"
// AI(W906-GA1-B5) 20260804: golden declares MyDBIProcess in cMyDB.h (3-param,
// S2 defaulted: `MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString
// S2="")`, verified against golden cMyDB.h:20) and reaches LastSet.h via
// MachineDefine.h. Neither is true in THIS ported tree: MachineDefine.h's own
// `#include "LastSet.h"` sits inside its dead `#if 0` block (verified: lines
// 34-146), and MyDBIProcess's already-ported home is aHotPlateSubstrate.h's
// 2-required-arg reduction (`void MyDBIProcess(AnsiString S1, AnsiString
// S2);`, no default) -- already used tree-wide by every other translated call
// site with exactly 2 positional args, exactly like this file's own
// `MyDBIProcess("Exception", "TfLan::FormDestroy")` (golden :42). Reusing it
// (not re-declaring a second, possibly-drifting extern) needs this explicit
// include; cprod.h/cmydef.h/MachineType.h do not carry either symbol.
#include "LastSet.h"                  // LastSet.iLanguageCountry (GotLanString)
#include "aHotPlateSubstrate.h"       // MyDBIProcess (FormDestroy's catch handler)
#include <cstdlib>                    // atoi (GotLanString)
//------------------------------------------------------------------------------
//Steven 20210626 : 語言包改用CSV, 不再使用Database方式
//------------------------------------------------------------------------------

TfLan *fLan = 0;                                                                // golden: TfLan *fLan;  (explicit =0: no VCL framework auto-constructs this, contract is nullptr until whoever wires up TfLan actually assigns one)
//------------------------------------------------------------------------------
TfLan::TfLan()                                                                  // golden: __fastcall TfLan::TfLan(TComponent* Owner) : TForm(Owner) -- Owner dropped, never referenced in golden's own body, headless has no TForm/TComponent tree to attach to (project convention, see language.h)
{
    TableName="Language.DB";
    FullFileName ="D:\\HT9045\\system\\Language.csv";

//    LanguageType=0;
    ShowHint=false;
    bUseLanguageCSV=FileExists(FullFileName);

    LanList=new TStringList;
    tLanList=new TStringList;
    mapLanList.clear();
}
//------------------------------------------------------------------------------
TfLan::~TfLan()                                                                 // AI(W906-GA1-B5) 20260804: NEW, no golden counterpart -- see language.h's ctor-note banner for why this calls FormDestroy(0) itself
{
    FormDestroy(0);
}
//------------------------------------------------------------------------------
void TfLan::FormDestroy(TObject * /*Sender*/)                                   // Sender genuinely unused in golden's own body too (verified); comment-out matches this tree's established unused-parameter convention (e.g. acarry_shims.cpp's LogSoftwareOffTime)
{
    try
    {
        LanList->Clear();
        delete LanList;
        tLanList->Clear();
        delete tLanList;
        mapLanList.clear();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfLan::FormDestroy");
    }
    LogSoftwareOffTime("TfLan, FormDestroy");                                   //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void TfLan::LoadLanguageFromFile()
{
    AnsiString str;
    mapLanList.clear();
    if(bUseLanguageCSV)
    {
        LanList->Clear();
        LanList->LoadFromFile(FullFileName);
        for(int i=0; i<LanList->Count; i++)
        {
            tLanList->CommaText=LanList->Strings[i];
            // AI(W906-GA1-B5) 20260804: (AnsiString) casts on both Strings[]
            // args -- see file-head GOTCHA note; without them this silently
            // formats garbage instead of "<From>_<Name>".
            str.sprintf("%s_%s", (AnsiString)tLanList->Strings[0], (AnsiString)tLanList->Strings[1]);
            mapLanIter=mapLanList.find(str);
            if(mapLanIter==mapLanList.end())
            {
                mapLanList[str]=i;
            }
//            else    //有重複的就不用加進去了
//            {
//            }
        }
    }
    else
    {
        str.sprintf("From_Name");
        mapLanIter=mapLanList.find(str);
        if(mapLanIter==mapLanList.end())
        {
            tLanList->Clear();
            tLanList->Add("From");
            tLanList->Add("Name");
            tLanList->Add("EString");
            tLanList->Add("CString");
            tLanList->Add("EHint");
            tLanList->Add("CHint");
            LanList->Add(tLanList->CommaText);
            mapLanList[str]=0;
        }
    }
}
//------------------------------------------------------------------------------
//找出元件, 存到檔案裡
//------------------------------------------------------------------------------
bool TfLan::UpdateCompomentToStrList(AnsiString Name, AnsiString Caption)        // golden header names this param `Cap`; golden .cpp definition names it `Caption` -- quirk kept verbatim (definition's parameter name is the one that matters for the body below)
{
    AnsiString str, S2;
    int iIndex;
    if(Name=="" || Caption=="")
        return false;

    if(IsMainFormNotChange(Name))
        return false;

    str.sprintf("%s_%s", LanguageFormName, Name);
    mapLanIter=mapLanList.find(str);
    if(mapLanIter==mapLanList.end())
    {
        tLanList->Clear();
        tLanList->Add(LanguageFormName);
        tLanList->Add(Name);
        S2.sprintf("%s", Caption);
        tLanList->Add(S2);
        tLanList->Add(S2);                                                      //預設中英文相同
        tLanList->Add("");
        tLanList->Add("");
        LanList->Add(tLanList->CommaText);
        iIndex=LanList->Count-1;
        mapLanList[str]=iIndex;
    }
    return true;
}
//------------------------------------------------------------------------------
// GATED -- golden :119-188. Recurses over a live TWinControl tree
// (PCtrl->ControlCount/PCtrl->Controls[iP]) dynamic_cast-dispatching across
// TPanel/TTabSheet/TGroupBox/TRadioGroup/TLabel/TSpeedButton/TButton/TCheckBox
// (plus TPageControl/TTabControl/THeader/TPage/TScrollBox as pure recursion
// gates) to harvest ->Name/->Caption pairs into UpdateCompomentToStrList. No
// widget hierarchy is modeled in this headless port (see language.h's
// TWinControl/TForm note) -- offline, there is nothing to walk. No-op stub;
// UpdateLanguageToStrList (translated faithfully, below) still calls this in
// the same sequence golden does, so the CSV load/save half of that caller
// stays exercisable/testable even though this one leaf is inert.
//------------------------------------------------------------------------------
void TfLan::SearchCompomentForSave(TWinControl * /*PCtrl*/)
{
    // GATED: see banner comment above. Offline: no live widget tree to walk.
}
//------------------------------------------------------------------------------
bool TfLan::IsMainFormNotChange(AnsiString S)
{
    tLanList->Clear();
    tLanList->Add("Panel17");
    tLanList->Add("lbEnCoder0");
    tLanList->Add("lbEnCoder1");
    tLanList->Add("AutoCleanContactCountLabel");
    tLanList->Add("lblShuttle1");
    tLanList->Add("lblShuttle2");
    tLanList->Add("Label12");
    tLanList->Add("Label6");
    tLanList->Add("Label7");
    tLanList->Add("Label8");
    tLanList->Add("Label9");
    tLanList->Add("Label3");
    tLanList->Add("pnlSpeed1");
    tLanList->Add("pnlSpeed2");
    tLanList->Add("pnlSpeed3");
    tLanList->Add("pnlSpeed4");
    tLanList->Add("iWhichKitLabel");
    tLanList->Add("Label1");
    tLanList->Add("Label2");
    tLanList->Add("LabZ1_Test");
    tLanList->Add("LabZ2_Test");
    tLanList->Add("spbFan");
    tLanList->Add("spbLight");
    tLanList->Add("spbChamberFan");
    tLanList->Add("labStopTime");
    tLanList->Add("sbPEModel");
    tLanList->Add("labGuardBand");
    tLanList->Add("palTesterMode");
    tLanList->Add("palRunMode_1");
    tLanList->Add("palPrime");
    tLanList->Add("palNormal");
    tLanList->Add("AutoCleanPad");
    tLanList->Add("palMainStatus");
    tLanList->Add("labTrayMap");
    tLanList->Add("labOCR");
    tLanList->Add("labHonQC");
    tLanList->Add("PiggyBackLab");
    tLanList->Add("labFailAlarmCnt");
    tLanList->Add("labTesterMode");
    tLanList->Add("labAutomation");
    tLanList->Add("lblInOutAlarm");
    tLanList->Add("labRTCCD");
    tLanList->Add("lbArm1Torque");
    tLanList->Add("lbArm0Torque");
    tLanList->Add("labQAMode");
    tLanList->Add("labATC");
    tLanList->Add("labAutoSkip");
    tLanList->Add("lb_PMAlarmDate");
    tLanList->Add("lbEPenconder");
    tLanList->Add("labARTmode");
    tLanList->Add("lbMonitorConnectState");
    tLanList->Add("labAutoClean");
    tLanList->Add("ARTCombine");
    tLanList->Add("labDelayStatus");
    tLanList->Add("labScanAOI");
    tLanList->Add("lblTemperatureMode");

    for(int i=0; i<tLanList->Count; i++)
    {
        if(S==tLanList->Strings[i])
        {
            tLanList->Clear();
            return true;
        }
    }

    tLanList->Clear();
    return false;
}
//------------------------------------------------------------------------------
void TfLan::UpdateLanguageToStrList(TForm *P)
{
    LoadLanguageFromFile();
    LanguageFormName=P->Name;
    SearchCompomentForSave(P);
    LanList->SaveToFile(FullFileName);
    bUseLanguageCSV=true;
}
//------------------------------------------------------------------------------
bool TfLan::GotLanString(AnsiString Name, AnsiString *CapStr, AnsiString *HintStr)  //回傳對應的字串
{
    AnsiString str, sIndex;
    bool bResult=true;
    int iIndex;
    if(bUseLanguageCSV==false || LanguageFormName=="" || Name=="")
    {
        return false;
    }

    str.sprintf("%s_%s", LanguageFormName, Name);
    mapLanIter=mapLanList.find(str);
    if(mapLanIter!=mapLanList.end())
    {
        sIndex=mapLanList[str];
        iIndex=atoi(sIndex.c_str());
        if(iIndex<0 || iIndex>=LanList->Count)
        {
            bResult=false;
        }
        else
        {
            tLanList->Clear();
            tLanList->CommaText=LanList->Strings[iIndex];
            if(tLanList->Count>=6)
            {
                if(LastSet.iLanguageCountry==0)
                {
                    // AI(W906-GA1-B5) 20260804: (AnsiString) casts -- see file-head GOTCHA note.
                    CapStr->sprintf("%s", (AnsiString)tLanList->Strings[2]);
                    HintStr->sprintf("%s", (AnsiString)tLanList->Strings[4]);
                }
                else
                {
                    CapStr->sprintf("%s", (AnsiString)tLanList->Strings[3]);
                    HintStr->sprintf("%s", (AnsiString)tLanList->Strings[5]);
                }

                if(CapStr->Length()==0)
                    bResult=false;
                else
                    bResult=true;
            }
            else
            {
                bResult=false;
            }
        }
    }
    else
    {
        bResult=false;
    }

    return bResult;
}
//------------------------------------------------------------------------------
// GATED -- golden :328-457. Same shape as SearchCompomentForSave above (live
// TWinControl recursion), plus per-leaf SetCompomentLanguage(...,
// eUserLanguage(IniConfig.iUserLanguage)) charset switching and
// Caption/Hint/ShowHint writes. No widget hierarchy is modeled headless (see
// language.h's TWinControl/TForm note); no-op stub. ChangeLanguage (translated
// faithfully, below) still calls this in the same sequence golden does.
//------------------------------------------------------------------------------
void TfLan::LoadCompomentForChangeLan(TWinControl * /*PCtrl*/)
{
    // GATED: see banner comment above. Offline: no live widget tree to walk.
}
//------------------------------------------------------------------------------
void TfLan::ChangeLanguage(TForm *P)
{
    LanguageFormName=P->Name;
    LoadLanguageFromFile();
    LoadCompomentForChangeLan(P);
}
//==============================================================================
// GATED -- golden :466-470 ( 20230601 Joseph , Auto Form Size ). Centers a
// control using Screen->Width/Height, a live-window concept this headless
// port does not model. No-op stub, called by nothing else in this file
// (golden itself never calls FormShowPos from within language.cpp either --
// grepped the whole golden tree: FormShowPos has zero callers anywhere,
// dead-on-arrival even in golden).
//==============================================================================
void FormShowPos( TWinControl * /*PCtrl*/ )
{
    // GATED: see banner comment above.
}
//==============================================================================
// GATED -- golden :472-476. Scales a control's Height/Width by
// fNowSystemHeightScale/fNowSystemWidthScale (both real globals, cmydef.h) --
// pure display glue, only ever invoked by FormChangeAction below.
//==============================================================================
void FormChangeSize( TWinControl * /*PCtrl*/ )
{
    // GATED: see banner comment above.
}
//==============================================================================
// GATED -- golden :478-733. The Auto-Form-Size recursive resizer: walks a
// live TWinControl tree, dynamic_cast-dispatching across ~23 distinct golden
// widget types purely to scale each one's Top/Left/Height/Width/Font->Size by
// fNowSystemHeightScale/fNowSystemWidthScale. Zero CSV/lookup content --
// textbook pure UI glue. No widget hierarchy is modeled headless.
//==============================================================================
void FormReSize( TWinControl * /*PCtrl*/ )
{
    // GATED: see banner comment above.
}
//==============================================================================
// GATED -- golden :735-745 ( 20230601 Joseph , Auto Form Size ). Golden walks
// Screen->Forms[] looking for the live "fMain" window and, if found, calls
// FormChangeSize+FormReSize on it. Screen->Forms[]/FormCount is a live-VCL-
// application concept this headless port does not model. Declared in
// language.h (`extern void FormChangeAction();`) and called from golden
// main.cpp (untranslated) -- kept as a no-op stub purely for linkage
// compatibility with that extern surface.
//==============================================================================
void FormChangeAction()
{
    // GATED: see banner comment above.
}
//==============================================================================
