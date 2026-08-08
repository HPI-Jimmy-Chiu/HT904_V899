// =============================================================================
//  MyTempPanel.cpp  --  TMyTempPanel definitions (see MyTempPanel.h for the
//                        full ROLE / ZERO CONSUMERS / GATE REGISTER writeup --
//                        re-cited briefly at each call site here).
//
//  Faithful translation of golden MyTempPanel.cpp (615 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3 group "panels".  Third file to land.
//
//  golden_fns = 11: ctor(1) dtor(2) SetParent(3) SetEnable(4)
//  edBaseMouseDown(5) edLimitMouseDown(6) edIndiviMouseDown(7)
//  edinitialMouseDown(8) SetCaption(9) SetIndexTag(10) GetCaption(11).
//  ported_fns = 7: ctor, dtor, SetParent, SetEnable, SetCaption, SetIndexTag,
//  GetCaption all get real bodies below. The 4 mouse-down handlers (5-8) are
//  OMITTED per MyTempPanel.h's GATE (1) -- their
//  `TMouseButton`/`TShiftState` parameter types cannot be spelled anywhere in
//  this port (the identical class of omission EJ1N/MyOmronPanel.cpp and
//  MyVacuumPanel.cpp already apply to their own mouse/click handlers of the
//  same golden shape).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "MyTempPanel.h"
//   #include "uTemp_Set.h" / "BarcodeReader.h" / "myQwertyKeyBoard.h" /
//     "ATC_Handler_Side.h" / "cmydef.h"
//   #pragma package(smart_init)
//
//   NOT re-included here, and why: "uTemp_Set.h" -- golden's `fTemp_Set` form;
//   its ONLY use in the ACTIVE bodies below was the ctor's
//   `new TPanel(fTemp_Set->tsUnUse)` parent argument, itself gated (see
//   ctor's own note -- vclcompat::TPanel takes no ctor argument at all).
//   Every OTHER `fTemp_Set` reference in golden (MaxTempSetting()/
//   MinTempSetting()) lives exclusively inside the 4 omitted mouse-down
//   handlers (GATE 1). "BarcodeReader.h" (Barcode_Reader) / "myQwertyKeyBoard.h"
//   (fQwertyKey) / "ATC_Handler_Side.h" -- referenced ONLY inside the same 4
//   omitted handlers. "cmydef.h" -- nothing in the 7 ACTIVE bodies below
//   reaches a cmydef.h-only symbol (MyDBIProcess is forward-declared locally,
//   matching this tree's established idiom -- see the ctor/dtor note below).
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella)
#include "MyTempPanel.h"            // this unit's own contract

// MyDBIProcess -- golden home aHotPlateSubstrate.h:924. Forward-declared
// locally rather than pulling in that whole header, matching this tree's
// widely-used established idiom for this EXACT function (automation.cpp:72,
// CCLink/MyCCLink.cpp:36, OmronLaser/LaserSensor.cpp, SECSGEM/
// uHGemEquipment.cpp:93 all do the identical one-line forward declaration
// rather than #include-ing aHotPlateSubstrate.h whole).
extern void MyDBIProcess(AnsiString S1, AnsiString S2);
//---------------------------------------------------------------------------
TList *listNormal;
TList *listArm1;
TList *listArm2;
//TList *listDut;
//TList *listNoUse;
//TList *listHeater;
//---------------------------------------------------------------------------
TMyTempPanel::TMyTempPanel(AnsiString Alias, int iTag)
{
    AnsiString Str;
    //AI(W906-PT-W3) 20260807: golden `new TPanel(fTemp_Set->tsUnUse)` (golden
    //  :23) -- vclcompat::TPanel (and every other vclcompat control type used
    //  below) takes NO constructor argument at all (see vclcompat/
    //  Controls.h); golden's Owner/initial-Parent argument is dropped, same
    //  idiom EJ1N/MyOmronPanel.cpp and MyVacuumPanel.cpp already use for
    //  every one of their own `new TXxx(this)`/`new TXxx(palTemp)` calls.
    palTemp         =new TPanel();

    labName         =new TLabel();                                       //name

    edLow           =new TEdit();
    edMid           =new TEdit();
    edLowbase       =new TEdit();
    edBase          =new TEdit();
    edHighBase      =new TEdit();
    edSHighBase     =new TEdit();                                        //Ztex 2024.07.27 Add 6 Point Temperature Offset
    //==> Eastsun 20260526 #026-4.A7 Ifor 20241101 add :KYEC
    edPreOffset     =new TEdit();       //預先補償
    edPreOfsTime    =new TEdit();       //補償秒數
    edAfterOfs      =new TEdit();       //補償秒數結束後的補償值
    //<== Eastsun 20260526 #026-4.A7

    edKit_Low       =new TEdit();                                        //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid       =new TEdit();
    edKit_Lowbase   =new TEdit();
    edKit_Base      =new TEdit();
    edKit_HighBase  =new TEdit();

    edOffset        =new TEdit();

    edSingleLimit   =new TEdit();
    edIndiTemp      =new TEdit();
    edInitTempOffset=new TEdit();
    edEOTTempOffset =new TEdit();
    palLine         =new TPanel();

    iIndexTag=iTag;

    //AI(W906-PT-W3) 20260807: golden's ->Left/->Top/->Width/->Height/
    //  ->DragKind/->BevelInner/->BevelOuter writes (golden :55-59,61-62) have
    //  no substrate (vclcompat::TPanel carries only Caption+Color, plus
    //  TControl's Visible/Enabled -- see Controls.h). ->Color and ->Visible
    //  ARE real and stay ACTIVE below.
#if 0
    palTemp->Left      =0;
    palTemp->Top       =0;
    palTemp->Width     =793;
    palTemp->Height    =27;
    palTemp->DragKind  =dkDock;
#endif
    palTemp->Color     =0x00C2B8A6;
#if 0
    palTemp->BevelInner=bvNone;
    palTemp->BevelOuter=bvNone;
#endif
    palTemp->Visible   =true;

    //AI(W906-PT-W3) 20260807: golden's ->Parent/->AutoSize/->Left/->Top/
    //  ->Width/->Height/->Font->.../->ParentFont writes on labName (golden
    //  :65,66-70,72-76) have no substrate (vclcompat::TLabel carries only
    //  Caption+Color, plus TControl's Visible/Enabled). ->Caption IS real and
    //  stays ACTIVE.
#if 0
    labName->Parent=palTemp;
    labName->AutoSize=false;
    labName->Left = 5;
    labName->Top = 2;
    labName->Width = 80;
    labName->Height = 16;
#endif
    labName->Caption=Alias;
#if 0
    labName->Font->Charset = DEFAULT_CHARSET;
    labName->Font->Color = clNavy;
    labName->Font->Size = 10;
    labName->Font->Name = "MS Sans Serif";
    labName->ParentFont = false;
#endif

    //AI(W906-PT-W3) 20260807: golden's ->Parent/Str-based ->Name/->AutoSize/
    //  ->Left/->Top/->Width/->Height/->ImeMode/->ParentFont writes (golden
    //  :78-87) have no substrate (vclcompat::TCustomEdit/TEdit carry only
    //  Text, plus TControl's Visible/Enabled -- no ->Name member anywhere in
    //  this port's TEdit chain either, unlike golden's TWinControl-derived
    //  original). ->Enabled IS real and stays ACTIVE. ->OnMouseDown has no
    //  substrate AND its target (edBaseMouseDown) is omitted -- GATE (1).
#if 0
    edLow->Parent=palTemp;
    Str.sprintf("edLow%03d", iTag);
    edLow->Name=Str;
    edLow->AutoSize=false;
    edLow->Left = 90;
    edLow->Top = 2;
    edLow->Width = 50;                                                          //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLow->Height = 21;
    edLow->ImeMode= imClose;
    edLow->ParentFont = false;
#endif
    edLow->Enabled =true;
#if 0
    edLow->OnMouseDown=edBaseMouseDown;
#endif

    //AI(W906-PT-W3) 20260807: every edKit_Xxx block below (golden :91-101,
    //  116-126,141-151,166-176,191-201) repeats the SAME gated shape as
    //  edLow above, PLUS a `->Color=clMedGray;` golden write that also has no
    //  substrate (TCustomEdit/TEdit carry no Color member -- only TPanel/
    //  TLabel do). Only ->Enabled stays ACTIVE for each. Not re-annotated
    //  per-block below to keep this file proportional to golden's own
    //  density; this one note covers all 5 Kit_ blocks.
#if 0
    edKit_Low->Parent=palTemp;                                                  //JerryYang 20221003 Analog要求新增Kit三點offset
#endif
#if 0
    edKit_Low->AutoSize=false;
    edKit_Low->Left = 90;
    edKit_Low->Top = 2;
    edKit_Low->Width = 60;
    edKit_Low->Height = 21;
    edKit_Low->ImeMode= imClose;
    edKit_Low->ParentFont = false;
#endif
    edKit_Low->Enabled =true;
#if 0
    edKit_Low->OnMouseDown=edBaseMouseDown;
    edKit_Low->Color=clMedGray;
#endif

#if 0
    edMid->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edMid%03d", iTag);
    edMid->Name=Str;
    edMid->AutoSize=false;
    edMid->Left = 141;                                                          //151->141 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edMid->Top = 2;
    edMid->Width = 50;                                                          //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edMid->Height = 21;
    edMid->ImeMode= imClose;
    edMid->ParentFont = false;
#endif
    edMid->Enabled =true;
#if 0
    edMid->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edKit_Mid->Parent=palTemp;                                                  //JerryYang 20221003 Analog要求新增Kit三點offset
#endif
#if 0
    edKit_Mid->AutoSize=false;
    edKit_Mid->Left = 151;
    edKit_Mid->Top = 2;
    edKit_Mid->Width = 60;
    edKit_Mid->Height = 21;
    edKit_Mid->ImeMode= imClose;
    edKit_Mid->ParentFont = false;
#endif
    edKit_Mid->Enabled =true;
#if 0
    edKit_Mid->OnMouseDown=edBaseMouseDown;
    edKit_Mid->Color=clMedGray;
#endif

#if 0
    edLowbase->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edLowbase%03d", iTag);
    edLowbase->Name=Str;
    edLowbase->AutoSize=false;
    edLowbase->Left = 192;                                                      //192->212 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLowbase->Top = 2;
    edLowbase->Width = 50;                                                      //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLowbase->Height = 21;
    edLowbase->ImeMode= imClose;
    edLowbase->ParentFont = false;
#endif
    edLowbase->Enabled =true;
#if 0
    edLowbase->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edKit_Lowbase->Parent=palTemp;                                              //JerryYang 20221003 Analog要求新增Kit三點offset
#endif
#if 0
    edKit_Lowbase->AutoSize=false;
    edKit_Lowbase->Left = 212;
    edKit_Lowbase->Top = 2;
    edKit_Lowbase->Width = 60;
    edKit_Lowbase->Height = 21;
    edKit_Lowbase->ImeMode= imClose;
    edKit_Lowbase->ParentFont = false;
#endif
    edKit_Lowbase->Enabled =true;
#if 0
    edKit_Lowbase->OnMouseDown=edBaseMouseDown;
    edKit_Lowbase->Color=clMedGray;
#endif

#if 0
    edBase->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edBase%03d", iTag);
    edBase->Name=Str;
    edBase->AutoSize=false;
    edBase->Left = 243;                                                         //192->273 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edBase->Top = 2;
    edBase->Width = 50;                                                         //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edBase->Height = 21;
    edBase->ImeMode= imClose;
    edBase->ParentFont = false;
#endif
    edBase->Enabled =true;
#if 0
    edBase->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edKit_Base->Parent=palTemp;                                                 //JerryYang 20221003 Analog要求新增Kit三點offset
#endif
#if 0
    edKit_Base->AutoSize=false;
    edKit_Base->Left = 273;
    edKit_Base->Top = 2;
    edKit_Base->Width = 60;
    edKit_Base->Height = 21;
    edKit_Base->ImeMode= imClose;
    edKit_Base->ParentFont = false;
#endif
    edKit_Base->Enabled =true;
#if 0
    edKit_Base->OnMouseDown=edBaseMouseDown;
    edKit_Base->Color=clMedGray;
#endif

#if 0
    edHighBase->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edHighBase%03d", iTag);
    edHighBase->Name=Str;
    edHighBase->AutoSize=false;
    edHighBase->Left = 294;                                                     //294->334 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edHighBase->Top = 2;
    edHighBase->Width = 50;                                                     //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edHighBase->Height = 21;
    edHighBase->ImeMode= imClose;
    edHighBase->ParentFont = false;
#endif
    edHighBase->Enabled =true;
#if 0
    edHighBase->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edKit_HighBase->Parent=palTemp;                                             //JerryYang 20221003 Analog要求新增Kit三點offset
#endif
#if 0
    edKit_HighBase->AutoSize=false;
    edKit_HighBase->Left = 334;
    edKit_HighBase->Top = 2;
    edKit_HighBase->Width = 60;
    edKit_HighBase->Height = 21;
    edKit_HighBase->ImeMode= imClose;
    edKit_HighBase->ParentFont = false;
#endif
    edKit_HighBase->Enabled =true;
#if 0
    edKit_HighBase->OnMouseDown=edBaseMouseDown;
    edKit_HighBase->Color=clMedGray;
#endif

#if 0
    edSHighBase->Parent=palTemp;                                                //Ztex 2024.07.27 Add 6 Point Temperature Offset ==>
#endif
#if 0
    Str.sprintf("edSHighBase%03d", iTag);
    edSHighBase->Name=Str;
    edSHighBase->AutoSize=false;
    edSHighBase->Left = 345;
    edSHighBase->Top = 2;
    edSHighBase->Width = 50;
    edSHighBase->Height = 21;
    edSHighBase->ImeMode= imClose;
    edSHighBase->ParentFont = false;
#endif
    edSHighBase->Enabled =true;
#if 0
    edSHighBase->OnMouseDown=edBaseMouseDown;                                   //Ztex 2024.07.27 Add 6 Point Temperature Offset <==
#endif
    //==> Eastsun 20260526 #026-4.A7 Ifor 20241101 add :KYEC
#if 0
    edPreOffset->Parent=palTemp;
#endif
#if 0
    edPreOffset->AutoSize=false;
    edPreOffset->Left = 212;
    edPreOffset->Top = 2;
    edPreOffset->Width = 60;
    edPreOffset->Height = 21;
    edPreOffset->ImeMode= imClose;
    edPreOffset->ParentFont = false;
#endif
    edPreOffset->Enabled =true;
    edPreOffset->Visible =false;
#if 0
    edPreOffset->OnMouseDown=edBaseMouseDown;
    edPreOffset->Color=clMedGray;
#endif
#if 0
    edPreOfsTime->Parent=palTemp;
#endif
#if 0
    edPreOfsTime->AutoSize=false;
    edPreOfsTime->Left = 273;
    edPreOfsTime->Top = 2;
    edPreOfsTime->Width = 60;
    edPreOfsTime->Height = 21;
    edPreOfsTime->ImeMode= imClose;
    edPreOfsTime->ParentFont = false;
#endif
    edPreOfsTime->Enabled =true;
    edPreOfsTime->Visible =false;
#if 0
    edPreOfsTime->OnMouseDown=edBaseMouseDown;
    edPreOfsTime->Color=clMedGray;
#endif
#if 0
    edAfterOfs->Parent=palTemp;
#endif
#if 0
    edAfterOfs->AutoSize=false;
    edAfterOfs->Left = 334;
    edAfterOfs->Top = 2;
    edAfterOfs->Width = 60;
    edAfterOfs->Height = 21;
    edAfterOfs->ImeMode= imClose;
    edAfterOfs->ParentFont = false;
#endif
    edAfterOfs->Enabled =true;
    edAfterOfs->Visible =false;
#if 0
    edAfterOfs->OnMouseDown=edBaseMouseDown;
    edAfterOfs->Color=clMedGray;
#endif
    //<== Eastsun 20260526 #026-4.A7

#if 0
    edOffset->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edOffset%03d", iTag);
    edOffset->Name=Str;
    edOffset->AutoSize=false;
    edOffset->Left = 415;
    edOffset->Top = 2;
    edOffset->Width = 60;
    edOffset->Height = 21;
    edOffset->BorderStyle=bsNone;
#endif
    edOffset->Text = "0";
#if 0
    edOffset->Color = clBtnFace;
    edOffset->ParentFont = false;
#endif
    edOffset->Enabled =true;
#if 0
    edOffset->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edSingleLimit->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edSingleLimit%03d", iTag);
    edSingleLimit->Name=Str;
    edSingleLimit->AutoSize=false;
    edSingleLimit->Left = 494;
    edSingleLimit->Top = 2;
    edSingleLimit->Width = 60;
    edSingleLimit->Height = 21;
    edSingleLimit->ParentFont = false;
#endif
    edSingleLimit->Enabled =true;
#if 0
    edSingleLimit->OnMouseDown=edLimitMouseDown;
#endif

#if 0
    edIndiTemp->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edIndiTemp%03d", iTag);
    edIndiTemp->Name=Str;
    edIndiTemp->AutoSize=false;
    edIndiTemp->Left = 573;
    edIndiTemp->Top = 2;
    edIndiTemp->Width = 60;
    edIndiTemp->Height = 21;
    edIndiTemp->ParentFont = false;
#endif
    edIndiTemp->Enabled =true;
#if 0
    edIndiTemp->OnMouseDown=edIndiviMouseDown;
#endif

#if 0
    edInitTempOffset->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edInitTempOffset%03d", iTag);
    edInitTempOffset->Name=Str;
    edInitTempOffset->AutoSize=false;
    edInitTempOffset->Left = 652;
    edInitTempOffset->Top = 2;
    edInitTempOffset->Width = 60;
    edInitTempOffset->Height = 21;
    edInitTempOffset->ParentFont = false;
#endif
    edInitTempOffset->Enabled =true;
#if 0
    edInitTempOffset->OnMouseDown=edinitialMouseDown;                           //kevin 20210421 edBaseMouseDown; ->edinitialMouseDown 獨立 範圍
    //edInitTempOffset->OnMouseDown=edBaseMouseDown;
#endif

#if 0
    edEOTTempOffset->Parent=palTemp;
#endif
#if 0
    Str.sprintf("edEOTTempOffset%03d", iTag);
    edEOTTempOffset->Name=Str;
    edEOTTempOffset->AutoSize=false;
    edEOTTempOffset->Left = 731;
    edEOTTempOffset->Top = 2;
    edEOTTempOffset->Width = 60;
    edEOTTempOffset->Height = 21;
    edEOTTempOffset->ParentFont = false;
#endif
    edEOTTempOffset->Enabled =true;
#if 0
    edEOTTempOffset->OnMouseDown=edBaseMouseDown;
#endif

    palLine->Caption="";
#if 0
    palLine->Parent=palTemp;
    palLine->Height=1;
    palLine->BevelInner=bvNone;
    palLine->BevelOuter=bvNone;
    palLine->Align=alBottom;
#endif

    //AI(W906-PT-W3) 20260807: golden's 19 `->Tag=iTag;` writes (golden
    //  :325-346) have no substrate (no Tag member anywhere in this port's
    //  TControl chain) AND their sole consumer (the 4 omitted mouse-down
    //  handlers, GATE 1) does not exist either -- kept verbatim, fully gated:
#if 0
    edLow->Tag           =iTag;
    edMid->Tag           =iTag;
    edLowbase->Tag       =iTag;
    edBase->Tag          =iTag;
    edHighBase->Tag      =iTag;
    //==> Eastsun 20260526 #026-4.A7 Ifor 20241101 add :KYEC
    edPreOffset->Tag     =iTag;
    edPreOfsTime->Tag    =iTag;
    edAfterOfs->Tag      =iTag;
    //<== Eastsun 20260526 #026-4.A7

    edKit_Low->Tag       =iTag;                                                 //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid->Tag       =iTag;
    edKit_Lowbase->Tag   =iTag;
    edKit_Base->Tag      =iTag;
    edKit_HighBase->Tag  =iTag;

    edOffset->Tag        =iTag;
    edSingleLimit->Tag   =iTag;
    edIndiTemp->Tag      =iTag;
    edInitTempOffset->Tag=iTag;
    edEOTTempOffset->Tag =iTag;
#endif
    (void)Str;
    iOffsetByRecipeMaxLimit = 60;
    iOffsetByRecipeMinLimit = -60;
}
//---------------------------------------------------------------------------
TMyTempPanel::~TMyTempPanel()
{
    try
    {
        delete labName;
        delete edLow;
        delete edMid;
        delete edLowbase;
        delete edBase;
        delete edHighBase;
        //==> Eastsun 20260526 #026-4.A7 Ifor 20241101 add :KYEC
        delete edPreOffset;
        delete edPreOfsTime;
        delete edAfterOfs;
        //<== Eastsun 20260526 #026-4.A7

        delete edKit_Low;                                                       //JerryYang 20221003 Analog要求新增Kit三點offset
        delete edKit_Mid;
        delete edKit_Lowbase;
        delete edKit_Base;
        delete edKit_HighBase;

        delete edOffset;
        delete edSingleLimit;
        delete edIndiTemp;
        delete edInitTempOffset;
        delete edEOTTempOffset;
        delete palLine;
        //AI(W906-PT-W3) 20260807: golden `palTemp->Parent=NULL;` (golden
        //  :379) has no substrate (no ->Parent member, see ctor's note).
#if 0
        palTemp->Parent=NULL;
#endif
        delete palTemp;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyTempPanel");
    }
}
//---------------------------------------------------------------------------
void TMyTempPanel::SetParent(TTabSheet *tsTemp)                      //目前設定值
{
    //AI(W906-PT-W3) 20260807: golden `palTemp->Parent=tsTemp;
    //  palTemp->Align=alTop;` (golden :390-391) has no substrate (no
    //  ->Parent, no ->Align member on vclcompat::TPanel). ACTIVE arm: no-op.
#if 0
    palTemp->Parent=tsTemp;
    palTemp->Align=alTop;
#endif
    (void)tsTemp;
}
//---------------------------------------------------------------------------
void TMyTempPanel::SetEnable(bool bflag)
{
    edLow->Enabled=bflag;
    edMid->Enabled=bflag;
    edLowbase->Enabled=bflag;
    edBase->Enabled=bflag;
    edHighBase->Enabled=bflag;
    edSHighBase->Enabled=bflag;                                                 //Ztex 2024.07.27 Add 6 Point Temperature Offset

    edKit_Low->Enabled=bflag;                                                   //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid->Enabled=bflag;
    edKit_Lowbase->Enabled=bflag;
    edKit_Base->Enabled=bflag;
    edKit_HighBase->Enabled=bflag;

    edOffset->Enabled=bflag;
    edSingleLimit->Enabled=bflag;
    edIndiTemp->Enabled=bflag;
    edInitTempOffset->Enabled=bflag;
    edEOTTempOffset->Enabled=bflag;
    labName->Enabled=bflag;
}
//---------------------------------------------------------------------------
//AI(W906-PT-W3) 20260807: GATE (1) -- golden `edBaseMouseDown`/
//  `edLimitMouseDown`/`edIndiviMouseDown`/`edinitialMouseDown` (golden
//  :417-599) OMITTED entirely: their `TMouseButton`/`TShiftState` parameter
//  types cannot be spelled anywhere in this port (see MyTempPanel.h's own
//  banner GATE (1)). Golden bodies kept in comment form, for provenance, in
//  MyTempPanel.h's own GATE (1) citation -- not repeated verbatim here a
//  second time to keep this already-large file proportional; see golden
//  MyTempPanel.cpp:417-599 directly for the full text if ever needed.
//---------------------------------------------------------------------------
void TMyTempPanel::SetCaption(AnsiString Name)
{
    labName->Caption=Name;
}
//---------------------------------------------------------------------------
void TMyTempPanel::SetIndexTag(int iTag)
{
    iIndexTag=iTag;
}
//---------------------------------------------------------------------------
AnsiString TMyTempPanel::GetCaption()
{
    return labName->Caption;
}
//---------------------------------------------------------------------------
