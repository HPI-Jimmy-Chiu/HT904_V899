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
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- TfLan::SearchCompomentForSave
//  golden language.cpp:119-188 (70 golden lines), transcribed CHARACTER-FOR-CHARACTER
//  (Big5/cp950 -> UTF-8 only) and GATED OFF.  The block is INERT: the ACTIVE
//  TfLan::SearchCompomentForSave is the 4-line body immediately after the #endif, and that body is
//  UNCHANGED by this pair being here.  Net behaviour change: ZERO.
//
//  WHY: the census scored this function "translated" because a same-named
//  LIVE body exists -- but it never compares SIZE.  That body is 4 lines
//  standing in for 70 golden lines whose text was NOWHERE in this tree.
//  That is LOST TEXT, not deferred behaviour.  Now the text EXISTS and is
//  auditable line-by-line, and a later un-gate is mechanical rather than a
//  re-translation.
//
//  NOTHING inside the gate is fixed, renamed, reflowed or reindented --
//  golden's own defects are preserved ON PURPOSE so a diff against golden
//  stays EMPTY (this wave's report lists them with their golden line
//  numbers).  Same shape as this tree's existing pairs: atester.cpp
//  GetTesterResult (golden 1,705 gated / 6-line live) and csystem.cpp
//  MainProc (2,390 / 26), DoTrayFeedProcess (1,235 / 6),
//  CheckContinusStartIsReady (700 / 9), DoAllProcess.  Being gated it needs
//  NO callee to exist -- only lexical validity; no stub, declaration or
//  header edit was added anywhere for its symbols.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden language.cpp:119-188.  INERT reference text; the live TfLan::SearchCompomentForSave follows the #endif.
void __fastcall TfLan::SearchCompomentForSave(TWinControl *PCtrl)
{
    AnsiString S="";

    TPanel    *PanelPtr  = dynamic_cast <TPanel *>   (PCtrl);                   // 先將主物件的資料寫入
    TTabSheet *ShtPtr    = dynamic_cast <TTabSheet *>(PCtrl);
//    TForm     *FrmPtr    = dynamic_cast <TForm *>    (PCtrl);
    TGroupBox *PGroupBox = dynamic_cast <TGroupBox *>(PCtrl);
    TRadioGroup *RadGup  = dynamic_cast <TRadioGroup *>(PCtrl);
    if(PanelPtr!=NULL)
        UpdateCompomentToStrList(PanelPtr->Name, PanelPtr->Caption);
    else if(ShtPtr!=NULL)
        UpdateCompomentToStrList(ShtPtr->Name, ShtPtr->Caption);
//    else if(FrmPtr!=NULL)                                                     //fMain不能改Caption
//        UpdateCompomentToStrList(FrmPtr->Name);
    else if(PGroupBox!=NULL)
        UpdateCompomentToStrList(PGroupBox->Name, PGroupBox->Caption);
    else if(RadGup!=NULL)
        UpdateCompomentToStrList(RadGup->Name, RadGup->Caption);

    for(int iP=0; iP<PCtrl->ControlCount; iP++)                                 // 再將附屬物件的資料寫入
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast<TPanel *>(P)        !=NULL ||                           // 有子節點
           dynamic_cast<TPageControl *>(P)  !=NULL ||
           dynamic_cast<TTabSheet *>(P)     !=NULL ||
           dynamic_cast<TTabControl *>(P)   !=NULL ||
           dynamic_cast<TForm *>(P)         !=NULL ||
           dynamic_cast<THeader *>(P)       !=NULL ||
           dynamic_cast<TPage *>(P)         !=NULL ||
           dynamic_cast<TGroupBox*>(P)      !=NULL ||
           dynamic_cast<TScrollBox*>(P)     !=NULL ||
           dynamic_cast<TRadioGroup*>(P)    !=NULL)
        {
            SearchCompomentForSave((TWinControl *) P);                          // 找該物件附屬的物件(遞迴)
        }

        TLabel *PLabel                =dynamic_cast <TLabel *>(P);
        TSpeedButton    *PSpeedButton =dynamic_cast <TSpeedButton *>(P);
        TButton         *PButton      =dynamic_cast <TButton *>(P);
        TCheckBox       *PCheckBox    =dynamic_cast <TCheckBox *>(P);
//        TRadioButton    *PRadioButton =dynamic_cast <TRadioButton *>(P);
        if(PLabel!=NULL)                                                        // 如果是TLabel型態
        {
            UpdateCompomentToStrList(PLabel->Name, PLabel->Caption);
        }
        else if(PSpeedButton!=NULL)                                             // 如果是TSpeedButton型態
        {
            UpdateCompomentToStrList(PSpeedButton->Name, PSpeedButton->Caption);
        }
        else if(PButton!=NULL)                                                  // 如果是TButton型態
        {
            UpdateCompomentToStrList(PButton->Name, PButton->Caption);
        }
        else if(PCheckBox!=NULL)                                                // 如果是TCheckBox型態
        {
            UpdateCompomentToStrList(PCheckBox->Name, PCheckBox->Caption);
        }
//        else if(PRadioButton!=NULL)                                           // 如果是TRadioButton型態
//        {
//            S=PRadioButton->Name;
//            if(RadGup!=NULL)                                                  // 其父屬為 TRadioButton
//            {
//                S=RadGup->Name;
//                S=S+AnsiString("_SUB_")+AnsiString(iP);
//            }
//            UpdateCompomentToStrList(S, PRadioButton->Caption);
//        }
    }
}
#endif // GOLDEN VERBATIM -- golden language.cpp:119-188 (end)
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
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- TfLan::LoadCompomentForChangeLan
//  golden language.cpp:328-457 (130 golden lines), transcribed CHARACTER-FOR-CHARACTER
//  (Big5/cp950 -> UTF-8 only) and GATED OFF.  The block is INERT: the ACTIVE
//  TfLan::LoadCompomentForChangeLan is the 4-line body immediately after the #endif, and that body is
//  UNCHANGED by this pair being here.  Net behaviour change: ZERO.
//
//  WHY: the census scored this function "translated" because a same-named
//  LIVE body exists -- but it never compares SIZE.  That body is 4 lines
//  standing in for 130 golden lines whose text was NOWHERE in this tree.
//  That is LOST TEXT, not deferred behaviour.  Now the text EXISTS and is
//  auditable line-by-line, and a later un-gate is mechanical rather than a
//  re-translation.
//
//  NOTHING inside the gate is fixed, renamed, reflowed or reindented --
//  golden's own defects are preserved ON PURPOSE so a diff against golden
//  stays EMPTY (this wave's report lists them with their golden line
//  numbers).  Same shape as this tree's existing pairs: atester.cpp
//  GetTesterResult (golden 1,705 gated / 6-line live) and csystem.cpp
//  MainProc (2,390 / 26), DoTrayFeedProcess (1,235 / 6),
//  CheckContinusStartIsReady (700 / 9), DoAllProcess.  Being gated it needs
//  NO callee to exist -- only lexical validity; no stub, declaration or
//  header edit was added anywhere for its symbols.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden language.cpp:328-457.  INERT reference text; the live TfLan::LoadCompomentForChangeLan follows the #endif.
void __fastcall TfLan::LoadCompomentForChangeLan(TWinControl *PCtrl)
{
    AnsiString CapStr="", HintStr="";

    TPanel    *PanelPtr  = dynamic_cast <TPanel *>   (PCtrl);                   // 先將主物件的資料寫入
    TTabSheet *ShtPtr    = dynamic_cast <TTabSheet *>(PCtrl);
//    TForm     *FrmPtr    = dynamic_cast <TForm *>    (PCtrl);
    TGroupBox *PGroupBox = dynamic_cast <TGroupBox *>(PCtrl);
//    TScrollBox *ScrBox   = dynamic_cast <TScrollBox *>(PCtrl);
    TRadioGroup *RadGup  = dynamic_cast <TRadioGroup *>(PCtrl);
    if(PanelPtr!=NULL)
    {
        if(GotLanString(PanelPtr->Name, &CapStr, &HintStr))
        {
            SetCompomentLanguage(PanelPtr, eUserLanguage(IniConfig.iUserLanguage));
            PanelPtr->Caption=CapStr;
        }
    }
    else if(ShtPtr!=NULL)
    {
        if(GotLanString(ShtPtr->Name, &CapStr, &HintStr))
        {
            SetCompomentLanguage(ShtPtr, eUserLanguage(IniConfig.iUserLanguage));
            ShtPtr->Caption=CapStr;
        }
    }
//    else if(FrmPtr!=NULL)
//    {
//        if(GotLanString(FrmPtr->Name, &CapStr, &HintStr))
//        {
//            SetCompomentLanguage(FrmPtr, eUserLanguage(IniConfig.iUserLanguage));
//            FrmPtr->Caption=CapStr;
//        }
//    }
    else if(PGroupBox!=NULL)
    {
        if(GotLanString(PGroupBox->Name, &CapStr, &HintStr))
        {
            SetCompomentLanguage(PGroupBox, eUserLanguage(IniConfig.iUserLanguage));
            PGroupBox->Caption=CapStr;
        }
    }
    else if(RadGup!=NULL)
    {
        if(GotLanString(RadGup->Name, &CapStr, &HintStr))
        {
            SetCompomentLanguage(RadGup, eUserLanguage(IniConfig.iUserLanguage));
            RadGup->Caption=CapStr;
        }
    }

    for(int iP=0; iP<PCtrl->ControlCount; iP++)                                 // 再將附屬物件的資料寫入 .....................
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TPanel *>(P)       !=NULL ||
           dynamic_cast <TPageControl *>(P) !=NULL ||
           dynamic_cast <TTabSheet *>(P)    !=NULL ||
           dynamic_cast <TTabControl *>(P)  !=NULL ||
           dynamic_cast <TForm *>(P)        !=NULL ||
           dynamic_cast <THeader *>(P)      !=NULL ||
           dynamic_cast <TPage *>(P)        !=NULL ||
           dynamic_cast <TGroupBox*>(P)     !=NULL ||
           dynamic_cast <TScrollBox*>(P)    !=NULL ||
           dynamic_cast <TRadioGroup*>(P)   !=NULL)
        {
            LoadCompomentForChangeLan((TWinControl *)P);                        // 找該物件附屬的物件(遞迴)
        }
        TLabel          *PLabel       =dynamic_cast <TLabel *>(P);
        TSpeedButton    *PSpeedButton =dynamic_cast <TSpeedButton *>(P);
        TButton         *PButton      =dynamic_cast <TButton *>(P);
        TCheckBox       *PCheckBox    =dynamic_cast <TCheckBox *>(P);
//        TRadioButton    *PRadioButton =dynamic_cast <TRadioButton *>(P);
        if(PLabel!=NULL)                                                        // 如果是TLabel型態
        {
            if(GotLanString(PLabel->Name, &CapStr, &HintStr))
            {
                SetCompomentLanguage(PLabel, eUserLanguage(IniConfig.iUserLanguage));
                PLabel->Caption=CapStr;
            }

            if(ShowHint)
            {
                PLabel->Hint=HintStr;
                if(HintStr!="")                                                 //has Hint string
                    PLabel->ShowHint=true;
                else
                    PLabel->ShowHint=false;
            }
        }
        else if(PSpeedButton!=NULL)                                             // 如果是TSpeedButton型態
        {
            if(GotLanString(PSpeedButton->Name, &CapStr, &HintStr))
            {
                SetCompomentLanguage(PSpeedButton, eUserLanguage(IniConfig.iUserLanguage));
                PSpeedButton->Caption=CapStr;
            }
        }
        else if(PButton!=NULL)                                                  // 如果是TButton型態
        {
            if(GotLanString(PButton->Name, &CapStr, &HintStr))
            {
                SetCompomentLanguage(PButton, eUserLanguage(IniConfig.iUserLanguage));
                PButton->Caption=CapStr;
            }
        }
        else if(PCheckBox!=NULL)                                                // 如果是TCheckBox型態
        {
            if(GotLanString(PCheckBox->Name, &CapStr, &HintStr))
            {
                SetCompomentLanguage(PCheckBox, eUserLanguage(IniConfig.iUserLanguage));
                PCheckBox->Caption=CapStr;
            }
        }
//        else if(PRadioButton!=NULL)                                           // 如果是TRadioButton型態
//        {
//            S=PRadioButton->Name;
//            if(RadGup!=NULL)                                                  // 其父屬為 TRadioButton
//            {
//                SetCompomentLanguage(RadGup, eUserLanguage(IniConfig.iUserLanguage));
//                S=RadGup->Name;
//                S=S+AnsiString("_SUB_")+AnsiString(iP);
//            }
//            if(GotLanString(S, &CapStr, &HintStr))
//            {
//                SetCompomentLanguage(PRadioButton, eUserLanguage(IniConfig.iUserLanguage));
//                PRadioButton->Caption=CapStr;
//            }
//        }
    }
}
#endif // GOLDEN VERBATIM -- golden language.cpp:328-457 (end)
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
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- FormReSize
//  golden language.cpp:478-733 (256 golden lines), transcribed CHARACTER-FOR-CHARACTER
//  (Big5/cp950 -> UTF-8 only) and GATED OFF.  The block is INERT: the ACTIVE
//  FormReSize is the 4-line body immediately after the #endif, and that body is
//  UNCHANGED by this pair being here.  Net behaviour change: ZERO.
//
//  WHY: the census scored this function "translated" because a same-named
//  LIVE body exists -- but it never compares SIZE.  That body is 4 lines
//  standing in for 256 golden lines whose text was NOWHERE in this tree.
//  That is LOST TEXT, not deferred behaviour.  Now the text EXISTS and is
//  auditable line-by-line, and a later un-gate is mechanical rather than a
//  re-translation.
//
//  NOTHING inside the gate is fixed, renamed, reflowed or reindented --
//  golden's own defects are preserved ON PURPOSE so a diff against golden
//  stays EMPTY (this wave's report lists them with their golden line
//  numbers).  Same shape as this tree's existing pairs: atester.cpp
//  GetTesterResult (golden 1,705 gated / 6-line live) and csystem.cpp
//  MainProc (2,390 / 26), DoTrayFeedProcess (1,235 / 6),
//  CheckContinusStartIsReady (700 / 9), DoAllProcess.  Being gated it needs
//  NO callee to exist -- only lexical validity; no stub, declaration or
//  header edit was added anywhere for its symbols.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden language.cpp:478-733.  INERT reference text; the live FormReSize follows the #endif.
void FormReSize( TWinControl *PCtrl )                                           // 20230601 Joseph , Auto Form Size //
{
    for (int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            FormReSize((TWinControl *) P);
        }

        TPageControl *PPageControl          = dynamic_cast <TPageControl        *>(P);
        TEdit *PEdit                        = dynamic_cast <TEdit               *>(P);
        TLabeledEdit *PTLEdit               = dynamic_cast <TLabeledEdit        *>(P);  //Steven 20200715 : 補上TLabeledEdit
        TCheckBox *PCheckBox                = dynamic_cast <TCheckBox           *>(P);
        TRadioButton *PRadioButton          = dynamic_cast <TRadioButton        *>(P);
        TRadioGroup *PRadioGroup            = dynamic_cast <TRadioGroup         *>(P);
        TComboBox *PComboBox                = dynamic_cast <TComboBox           *>(P);
        TScrollBar *PScrollBar              = dynamic_cast <TScrollBar          *>(P);
        TImage *PImage                      = dynamic_cast <TImage              *>(P);
        TALed *PALed                        = dynamic_cast <TALed               *>(P);
        TMaskEdit *PMaskEdit                = dynamic_cast <TMaskEdit           *>(P);
        TPanel *PPanel                      = dynamic_cast <TPanel              *>(P);
        TButton *PButton                    = dynamic_cast <TButton             *>(P);
        TSpeedButton *PSpeedButton          = dynamic_cast <TSpeedButton        *>(P);
        TLabel *PLabel                      = dynamic_cast <TLabel              *>(P);
        TGroupBox *PGroupBox                = dynamic_cast <TGroupBox           *>(P);
        TScrollBox *PScrollBox              = dynamic_cast <TScrollBox          *>(P);
        TListBox *PListBox                  = dynamic_cast <TListBox            *>(P);
        TCheckListBox *PCheckListBox        = dynamic_cast <TCheckListBox       *>(P);
        TBevel *PBevel                      = dynamic_cast <TBevel              *>(P);
        TDriveComboBox *PDriveComboBox      = dynamic_cast <TDriveComboBox      *>(P);
        TDirectoryListBox *PDirectoryListBox= dynamic_cast <TDirectoryListBox   *>(P);
        TStringGrid *PStringGrid            = dynamic_cast <TStringGrid         *>(P);
//        TDBGrid *PDBGrid                    = dynamic_cast <TDBGrid             *>(P);
        TBitBtn *PBitBtn                    = dynamic_cast <TBitBtn             *>(P);

        if(PPageControl!=NULL && PPageControl->Name!="")
        {
            PPageControl->Top               = PPageControl->Top             * fNowSystemHeightScale ;
            PPageControl->Left              = PPageControl->Left            * fNowSystemWidthScale  ;
            PPageControl->Height            = PPageControl->Height          * fNowSystemHeightScale ;
            PPageControl->Width             = PPageControl->Width           * fNowSystemWidthScale  ;
        }

        if(PEdit!=NULL && PEdit->Name!="")
        {
            PEdit->Top                      = PEdit->Top                    * fNowSystemHeightScale ;
            PEdit->Left                     = PEdit->Left                   * fNowSystemWidthScale  ;
            PEdit->Height                   = PEdit->Height                 * fNowSystemHeightScale ;
            PEdit->Width                    = PEdit->Width                  * fNowSystemWidthScale  ;
            PEdit->Font->Size               = PEdit->Font->Size             * fNowSystemHeightScale ;
        }

        if(PTLEdit!=NULL && PTLEdit->Name!="")
        {
            PTLEdit->Top                    = PTLEdit->Top                  * fNowSystemHeightScale ;
            PTLEdit->Left                   = PTLEdit->Left                 * fNowSystemWidthScale  ;
            PTLEdit->Height                 = PTLEdit->Height               * fNowSystemHeightScale ;
            PTLEdit->Width                  = PTLEdit->Width                * fNowSystemWidthScale  ;
            PTLEdit->Font->Size             = PTLEdit->Font->Size           * fNowSystemHeightScale ;
        }

        if(PCheckBox!=NULL && PCheckBox->Name!="")
        {
            PCheckBox->Top                  = PCheckBox->Top                * fNowSystemHeightScale ;
            PCheckBox->Left                 = PCheckBox->Left               * fNowSystemWidthScale  ;
            PCheckBox->Height               = PCheckBox->Height             * fNowSystemHeightScale ;
            PCheckBox->Width                = PCheckBox->Width              * fNowSystemWidthScale  ;
            PCheckBox->Font->Size           = PCheckBox->Font->Size         * fNowSystemHeightScale ;
        }

        if(PRadioButton!=NULL  && PRadioButton->Name!="")
        {
            PRadioButton->Top               = PRadioButton->Top             * fNowSystemHeightScale ;
            PRadioButton->Left              = PRadioButton->Left            * fNowSystemWidthScale  ;
            PRadioButton->Height            = PRadioButton->Height          * fNowSystemHeightScale ;
            PRadioButton->Width             = PRadioButton->Width           * fNowSystemWidthScale  ;
            PRadioButton->Font->Size        = PRadioButton->Font->Size      * fNowSystemHeightScale ;
        }

        if(PRadioGroup!=NULL  && PRadioGroup->Name!="")
        {
            PRadioGroup->Top                = PRadioGroup->Top              * fNowSystemHeightScale ;
            PRadioGroup->Left               = PRadioGroup->Left             * fNowSystemWidthScale  ;
            PRadioGroup->Height             = PRadioGroup->Height           * fNowSystemHeightScale ;
            PRadioGroup->Width              = PRadioGroup->Width            * fNowSystemWidthScale  ;
            PRadioGroup->Font->Size         = PRadioGroup->Font->Size       * fNowSystemHeightScale ;
        }

        if(PComboBox!=NULL && PComboBox->Name!="")
        {
            PComboBox->Top                  = PComboBox->Top                * fNowSystemHeightScale ;
            PComboBox->Left                 = PComboBox->Left               * fNowSystemWidthScale  ;
            PComboBox->Height               = PComboBox->Height             * fNowSystemHeightScale ;
            PComboBox->Width                = PComboBox->Width              * fNowSystemWidthScale  ;
            PComboBox->Font->Size           = PComboBox->Font->Size         * fNowSystemHeightScale ;
        }

        if(PScrollBar!=NULL && PScrollBar->Name!="")
        {
            PScrollBar->Top                 = PScrollBar->Top               * fNowSystemHeightScale ;
            PScrollBar->Left                = PScrollBar->Left              * fNowSystemWidthScale  ;
            PScrollBar->Height              = PScrollBar->Height            * fNowSystemHeightScale ;
            PScrollBar->Width               = PScrollBar->Width             * fNowSystemWidthScale  ;
        }

        if(PImage!=NULL && PImage->Name!="")
        {
            PImage->Top                     = PImage->Top                   * fNowSystemHeightScale ;
            PImage->Left                    = PImage->Left                  * fNowSystemWidthScale  ;
            PImage->Height                  = PImage->Height                * fNowSystemHeightScale ;
            PImage->Width                   = PImage->Width                 * fNowSystemWidthScale  ;
        }

        if(PALed!=NULL && PALed->Name!="")
        {
            PALed->Top                      = PALed->Top                    * fNowSystemHeightScale ;
            PALed->Left                     = PALed->Left                   * fNowSystemWidthScale  ;
            PALed->Height                   = PALed->Height                 * fNowSystemHeightScale ;
            PALed->Width                    = PALed->Width                  * fNowSystemWidthScale  ;
        }

        if(PMaskEdit!=NULL && PMaskEdit->Name!="")
        {
            PMaskEdit->Top                  = PMaskEdit->Top                * fNowSystemHeightScale ;
            PMaskEdit->Left                 = PMaskEdit->Left               * fNowSystemWidthScale  ;
            PMaskEdit->Height               = PMaskEdit->Height             * fNowSystemHeightScale ;
            PMaskEdit->Width                = PMaskEdit->Width              * fNowSystemWidthScale  ;
            PMaskEdit->Font->Size           = PMaskEdit->Font->Size         * fNowSystemHeightScale ;
        }

        if(PPanel!=NULL && PPanel->Name!="")
        {
            PPanel->Top                     = PPanel->Top                   * fNowSystemHeightScale ;
            PPanel->Left                    = PPanel->Left                  * fNowSystemWidthScale  ;
            PPanel->Height                  = PPanel->Height                * fNowSystemHeightScale ;
            PPanel->Width                   = PPanel->Width                 * fNowSystemWidthScale  ;
            PPanel->Font->Size              = PPanel->Font->Size            * fNowSystemHeightScale ;
        }

        if(PButton!=NULL && PButton->Name!="")
        {
            PButton->Top                    = PButton->Top                  * fNowSystemHeightScale ;
            PButton->Left                   = PButton->Left                 * fNowSystemWidthScale  ;
            PButton->Height                 = PButton->Height               * fNowSystemHeightScale ;
            PButton->Width                  = PButton->Width                * fNowSystemWidthScale  ;
            PButton->Font->Size             = PButton->Font->Size           * fNowSystemHeightScale ;
        }

        if(PSpeedButton!=NULL && PSpeedButton->Name!="")
        {
            PSpeedButton->Top               = PSpeedButton->Top             * fNowSystemHeightScale ;
            PSpeedButton->Left              = PSpeedButton->Left            * fNowSystemWidthScale  ;
            PSpeedButton->Height            = PSpeedButton->Height          * fNowSystemHeightScale ;
            PSpeedButton->Width             = PSpeedButton->Width           * fNowSystemWidthScale  ;
            PSpeedButton->Font->Size        = PSpeedButton->Font->Size      * fNowSystemHeightScale ;
        }

        if(PLabel!=NULL && PLabel->Name!="")
        {
            PLabel->Top                     = PLabel->Top                   * fNowSystemHeightScale ;
            PLabel->Left                    = PLabel->Left                  * fNowSystemWidthScale  ;
            PLabel->Height                  = PLabel->Height                * fNowSystemHeightScale ;
            PLabel->Width                   = PLabel->Width                 * fNowSystemWidthScale  ;
            PLabel->Font->Size              = PLabel->Font->Size            * fNowSystemHeightScale ;
        }

        if(PGroupBox!=NULL && PGroupBox->Name!="")
        {
            PGroupBox->Top                  = PGroupBox->Top                * fNowSystemHeightScale ;
            PGroupBox->Left                 = PGroupBox->Left               * fNowSystemWidthScale  ;
            PGroupBox->Height               = PGroupBox->Height             * fNowSystemHeightScale ;
            PGroupBox->Width                = PGroupBox->Width              * fNowSystemWidthScale  ;
            PGroupBox->Font->Size           = PGroupBox->Font->Size         * fNowSystemHeightScale ;
        }

        if(PScrollBox!=NULL && PScrollBox->Name!="")
        {
            PScrollBox->Top                 = PScrollBox->Top               * fNowSystemHeightScale ;
            PScrollBox->Left                = PScrollBox->Left              * fNowSystemWidthScale  ;
            PScrollBox->Height              = PScrollBox->Height            * fNowSystemHeightScale ;
            PScrollBox->Width               = PScrollBox->Width             * fNowSystemWidthScale  ;
        }

        if(PListBox!=NULL && PListBox->Name!="")
        {
            PListBox->Top                   = PListBox->Top                 * fNowSystemHeightScale ;
            PListBox->Left                  = PListBox->Left                * fNowSystemWidthScale  ;
            PListBox->Height                = PListBox->Height              * fNowSystemHeightScale ;
            PListBox->Width                 = PListBox->Width               * fNowSystemWidthScale  ;
            PListBox->Font->Size            = PListBox->Font->Size          * fNowSystemHeightScale ;
        }

        if(PCheckListBox!=NULL && PCheckListBox->Name!="")
        {
            PCheckListBox->Top              = PCheckListBox->Top            * fNowSystemHeightScale ;
            PCheckListBox->Left             = PCheckListBox->Left           * fNowSystemWidthScale  ;
            PCheckListBox->Height           = PCheckListBox->Height         * fNowSystemHeightScale ;
            PCheckListBox->Width            = PCheckListBox->Width          * fNowSystemWidthScale  ;
            PCheckListBox->Font->Size       = PCheckListBox->Font->Size     * fNowSystemHeightScale ;
        }

        if(PBevel!=NULL && PBevel->Name!="")
        {
            PBevel->Top                     = PBevel->Top                   * fNowSystemHeightScale ;
            PBevel->Left                    = PBevel->Left                  * fNowSystemWidthScale  ;
            PBevel->Height                  = PBevel->Height                * fNowSystemHeightScale ;
            PBevel->Width                   = PBevel->Width                 * fNowSystemWidthScale  ;
        }

        if(PDriveComboBox!=NULL && PDriveComboBox->Name!="")
        {
            PDriveComboBox->Top             = PDriveComboBox->Top           * fNowSystemHeightScale ;
            PDriveComboBox->Left            = PDriveComboBox->Left          * fNowSystemWidthScale  ;
            PDriveComboBox->Height          = PDriveComboBox->Height        * fNowSystemHeightScale ;
            PDriveComboBox->Width           = PDriveComboBox->Width         * fNowSystemWidthScale  ;
            PDriveComboBox->Font->Size      = PDriveComboBox->Font->Size    * fNowSystemHeightScale ;
        }

        if(PDirectoryListBox!=NULL && PDirectoryListBox->Name!="")
        {
            PDirectoryListBox->Top          = PDirectoryListBox->Top        * fNowSystemHeightScale ;
            PDirectoryListBox->Left         = PDirectoryListBox->Left       * fNowSystemWidthScale  ;
            PDirectoryListBox->Height       = PDirectoryListBox->Height     * fNowSystemHeightScale ;
            PDirectoryListBox->Width        = PDirectoryListBox->Width      * fNowSystemWidthScale  ;
            PDirectoryListBox->Font->Size   = PDirectoryListBox->Font->Size * fNowSystemHeightScale ;
        }

        if(PStringGrid!=NULL && PStringGrid->Name!="")
        {
            PStringGrid->Top                = PStringGrid->Top              * fNowSystemHeightScale ;
            PStringGrid->Left               = PStringGrid->Left             * fNowSystemWidthScale  ;
            PStringGrid->Height             = PStringGrid->Height           * fNowSystemHeightScale ;
            PStringGrid->Width              = PStringGrid->Width            * fNowSystemWidthScale  ;
            PStringGrid->Font->Size         = PStringGrid->Font->Size       * fNowSystemHeightScale ;
        }

//        if(PDBGrid!=NULL && PDBGrid->Name!="")
//        {
//            PDBGrid->Top                    = PDBGrid->Top                  * fNowSystemHeightScale ;
//            PDBGrid->Left                   = PDBGrid->Left                 * fNowSystemWidthScale  ;
//            PDBGrid->Height                 = PDBGrid->Height               * fNowSystemHeightScale ;
//            PDBGrid->Width                  = PDBGrid->Width                * fNowSystemWidthScale  ;
//            PDBGrid->Font->Size             = PDBGrid->Font->Size           * fNowSystemHeightScale ;
//        }

        if(PBitBtn!=NULL && PBitBtn->Name!="")
        {
            PBitBtn->Top                    = PBitBtn->Top                  * fNowSystemHeightScale ;
            PBitBtn->Left                   = PBitBtn->Left                 * fNowSystemWidthScale  ;
            PBitBtn->Height                 = PBitBtn->Height               * fNowSystemHeightScale ;
            PBitBtn->Width                  = PBitBtn->Width                * fNowSystemWidthScale  ;
            PBitBtn->Font->Size             = PBitBtn->Font->Size           * fNowSystemHeightScale ;
        }
    }
}
#endif // GOLDEN VERBATIM -- golden language.cpp:478-733 (end)
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
