// =============================================================================
//  DynamicTemp.cpp  --  temperature-IC calibration/diagnostic dialog: channel
//                        chart plotting, draggable channel-box layout, RS232
//                        Temp-IC handshake, load/save-to-file record/playback.
//
//  Faithful translation of golden DynamicTemp.cpp (346 lines, BCB6, cp950).
//  Translator: AI(W906-FW3-DynTemp-WA) 20260820.
//  Translation wave: FW-3 batch-3 DynamicTemp Wave A (form-logic campaign).
//  Facade: forms/fDynamicTemp.h (read its banner FIRST -- widget shape, gate
//  register and design notes are documented there, not repeated in full here).
//
//  WAVE SCOPE -- see forms/fDynamicTemp.h's own "WAVE SCOPE" banner for the
//  full golden-line-span table (all 25 event handlers + ctor, this wave).
//
//  GATE REGISTER -- 4 narrow gates, full rationale in forms/fDynamicTemp.h:
//   (Q1) fQwertyKey->ShowQwertyKey -- 4 sites (edMin/edMax/edLower/
//        edUpperMouseDown). Missing-dependency: fQwertyKey has no port
//        anywhere in this tree (established tree-wide gate).
//   (C1) COM2->TempComm6->{WriteCommData,StopComm,StartComm} -- 5 sites
//        (lblRealTime6Click, lblRealTime2Click, gbUpperDblClick,
//        gbLowerDblClick, Button1Click). Missing-dependency: TCOM2Shim
//        (atester_shims.h) carries no TempComm6 member.
//   (D1) btTempICLoadClick's `ListBox1->Items->LoadFromFile(...)` -- SAFETY,
//        real disk read of an arbitrary dialog-selected path.
//   (D2) btTempICSaveClick's `ListBox1->Items->SaveToFile(...)` -- SAFETY,
//        real disk write of an arbitrary dialog-selected path.
//   sbTempICClearClick is NOT gated (no file/hardware call point on
//   inspection) despite being named in the task's safety cluster -- see
//   forms/fDynamicTemp.h's own note.
//
//  DEPENDENCY AUDIT (this wave, 20260820) -- every non-widget global
//  DynamicTemp.cpp's translated methods touch was grepped against this port
//  tree before translating: `iTempICTask` (atester.cpp:149, real),
//  `GetTempICResult()` (atester.cpp:8901, real -- a deliberate TODO(W7) stub,
//  not missing), `bTempComm6ReceiveOK` (cmydef.h:3187/cmydef.cpp:3418, real),
//  `InitialOK` (cmydef.h:220, real). `COM2`/`TCOM2Shim` exists
//  (atester_shims.h:364-405) but lacks `TempComm6` -- GATE (C1). `fQwertyKey`
//  has no port at all -- GATE (Q1). Full citations in forms/fDynamicTemp.h.
// =============================================================================
#include "MachineDefine.h"     // de-VCL'd include hub: vclcompat umbrella + portable STL
#pragma hdrstop

#include "forms/fDynamicTemp.h"

#include "cmydef.h"            // InitialOK / bTempComm6ReceiveOK / N_INTEGER (cited, unused once GATE (Q1) fires)

// AI(W906-FW3-DynTemp-WA) 20260820: golden DynamicTemp.cpp:15-18, file scope,
// verbatim shape and position. `fDynamicTemp` is zero-initialized (NOT
// `new`'d) -- matches golden's own PACKAGE global and this tree's
// static-init-order safety rule for cross-TU singletons (see
// docs/KNOWLEDGE.md's "V906 homecoming SIOF lottery" entry). `iTempICTask`/
// `GetTempICResult` are REAL externs (atester.cpp:149/:8901, see file
// banner) -- translated verbatim, not LOCALLY PROVIDED and not a gate.
// `btestReadTemp` has 0 other references anywhere in this port tree (grepped
// this wave, 20260820) -- a fresh file-scope global, used only within this
// TU (sbTempICClearClick/btShowClick/Timer1Timer), same as golden.
TfDynamicTemp *fDynamicTemp;
bool btestReadTemp=false;       //kevin 20130808 溫度ic
extern int iTempICTask;         //kevin 20130808 IC量測溫度
extern bool GetTempICResult();  //kevin 20130808 IC量測溫度

// AI(W906-FW3-DynTemp-WA) 20260820: LOCALLY PROVIDED -- golden Graphics.hpp
// VCL system colours (clRed/clBlue/clGreen/clBlack/clWhite/clAqua/clLime),
// needed by the ctor's TC[] population. SAME established pattern as
// cTemperFrom.cpp:130-133 / acatchtray_shims.h:126-128 / SECSGEM/
// uHGemEquipment.h:393-396 / vclcompat/{TrayCore,BtnPanelCore,LedCore}.h --
// a FILE-SCOPE (not vclcompat-namespaced) mirror, matching cTemperFrom.cpp's
// own choice, since this file (like that one) is a plain TU, not a header
// reused inside `namespace vclcompat`. Only the 7 named constants this
// file's ctor actually uses; the 3 remaining TC[] entries are raw hex
// `(TColor)0x...` literal casts needing no name (golden :23-24, itself).
// `TColor` comes from cmydef.h:16 (already included above) -- not
// re-typedef'd here (an identical repeat would be legal but is unnecessary).
const TColor clRed   = 0x000000FF;
const TColor clBlue  = 0x00FF0000;
const TColor clGreen = 0x00008000;
const TColor clBlack = 0x00000000;
const TColor clWhite = 0x00FFFFFF;
const TColor clAqua  = 0x00FFFF00;
const TColor clLime  = 0x0000FF00;

//---------------------------------------------------------------------------
// golden :20-40 -- ctor. Populates TC[]/SetEdit[]/myCB[] from the widgets
// NSDMI-allocated in forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
TfDynamicTemp::TfDynamicTemp()
{
    TC[ 0]=clRed;               TC[ 1]=clBlue;              TC[ 2]=clGreen;             TC[ 3]=clBlack;
    TC[ 4]=clWhite;             TC[ 5]=(TColor)0x00C08080;  TC[ 6]=(TColor)0x0047F8F3;  TC[ 7]=(TColor)0x00400080;
    TC[ 8]=clRed;               TC[ 9]=clBlue;              TC[10]=clGreen;             TC[11]=clBlack;
    TC[12]=clWhite;             TC[13]=(TColor)0x00C08080;  TC[14]=(TColor)0x0047F8F3;  TC[15]=clAqua;
    TC[16]=clLime;              TC[17]=clLime;

    SetEdit[ 0]=edtRealTime1;   SetEdit[ 1]=edtRealTime2;   SetEdit[ 2]=edtRealTime3;   SetEdit[ 3]=edtRealTime4;
    SetEdit[ 4]=edtRealTime5;   SetEdit[ 5]=edtRealTime6;   SetEdit[ 6]=edtRealTime7;   SetEdit[ 7]=edtRealTime8;
    SetEdit[ 8]=edtRealTime9;   SetEdit[ 9]=edtRealTime10;  SetEdit[10]=edtRealTime11;  SetEdit[11]=edtRealTime12;
    SetEdit[12]=edtRealTime13;  SetEdit[13]=edtRealTime14;  SetEdit[14]=edtRealTime15;  SetEdit[15]=edtRealTime16;
    SetEdit[16]=edUpper;        SetEdit[17]=edLower;

    myCB[ 0]=CheckBox1;         myCB[ 1]=CheckBox2;         myCB[ 2]=CheckBox3;         myCB[ 3]=CheckBox4;
    myCB[ 4]=CheckBox5;         myCB[ 5]=CheckBox6;         myCB[ 6]=CheckBox7;         myCB[ 7]=CheckBox8;
    myCB[ 8]=CheckBox9;         myCB[ 9]=CheckBox10;        myCB[10]=CheckBox11;        myCB[11]=CheckBox12;
    myCB[12]=CheckBox13;        myCB[13]=CheckBox14;        myCB[14]=CheckBox15;        myCB[15]=CheckBox16;
    myCB[16]=CheckBox17;        myCB[17]=CheckBox18;
}
//---------------------------------------------------------------------------
// golden :42-45.
//---------------------------------------------------------------------------
void TfDynamicTemp::btExitClick()
{
    // golden :44 `Close();` -- a TForm method (closes/hides the dialog); no
    // TForm base here (same posture as every other translated form facade).
    // DEVIATION: translated as a no-op body -- matches this tree's "GDI/
    // window method -> no-op" convention (forms/fObserver.h's own
    // Timer1Timer Close() note).
}
//---------------------------------------------------------------------------
// golden :47-53. GATE (C1) narrow (golden :50) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::lblRealTime6Click()
{
    AnsiString asWriteComm=":StartTest+";
#if 0 // GATE (C1) -- golden :50, COM2->TempComm6 has no port (see forms/fDynamicTemp.h)
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(),asWriteComm.Length());
#endif // GATE (C1)
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0,asWriteComm);
}
//---------------------------------------------------------------------------
// golden :55-58.
//---------------------------------------------------------------------------
void TfDynamicTemp::lblRealTime1DblClick()
{
    ListBox1->Clear();
}
//---------------------------------------------------------------------------
// golden :60-99. GATE (D1) narrow (golden :73) -- see file banner. The
// `sLine` materialization (DEVIATION, see forms/fDynamicTemp.h) replaces
// every repeated `ListBox1->Items->Strings[ct]` proxy re-read with one real
// AnsiString per outer-loop iteration.
//---------------------------------------------------------------------------
void TfDynamicTemp::btTempICLoadClick()
{
    int i, iPos;
    double fTemp[20];
    char cData[20], cTime[20];

    AnsiString asFind,asCh;

    if(OpenDialog2->Execute())
    {
        ListBox1->Clear();
        sbTempICClearClick();

#if 0 // GATE (D1) -- golden :73, real disk read of an arbitrary
      // dialog-selected path; SAFETY-classified, see forms/fDynamicTemp.h.
        ListBox1->Items->LoadFromFile(OpenDialog2->FileName);
#endif // GATE (D1)

        for(int ct=ListBox1->Items->Count-2;ct>0;ct--)
        {
            AnsiString sLine = ListBox1->Items->Strings[ct];   // DEVIATION: materialize the StringsProxy once (see file/header banner)

            if(sLine.Pos("T1:")<=0)
                continue;

            sprintf(cData, "%s", sLine.SubString(1,  10).c_str());   // DEVIATION: .c_str() -- see header banner
            sprintf(cTime, "%s", sLine.SubString(12, 8).c_str());    // DEVIATION: .c_str() -- see header banner

            for(i=0; i<16; i++)
            {
                asCh.printf("T%d:", (i+1));
                iPos=sLine.Pos(asCh);
                asFind=sLine.SubString(iPos+asCh.Length(), 6);
                fTemp[i]=atof(asFind.c_str());
                SetEdit[i]->Text=FormatFloat("0.00", fTemp[i]);

//                if(fTemp[i]!=0)
                    Chart1->Series[i]->AddY(fTemp[i], cTime, TC[i]);
            }

            Chart1->Series[16]->AddY(atof(edUpper->Text.c_str()), cTime, TC[16]);
            Chart1->Series[17]->AddY(atof(edLower->Text.c_str()), cTime, TC[17]);
        }
    }
}
//---------------------------------------------------------------------------
// golden :101-109. GATE (D2) narrow (golden :106) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::btTempICSaveClick()
{
    SaveDialog1->FileName="D:\\Dynamic.txt";    //Steven 20120921 : 存檔保護
    if(SaveDialog1->Execute())
    {
#if 0 // GATE (D2) -- golden :106, real disk write of an arbitrary
      // dialog-selected path; SAFETY-classified, see forms/fDynamicTemp.h.
        ListBox1->Items->SaveToFile(SaveDialog1->FileName);
#endif // GATE (D2)
        ListBox1->Clear();
    }
}
//---------------------------------------------------------------------------
// golden :111-121. NOT GATED -- see file/header banner (no file/hardware
// call point on inspection, despite being named in the task's safety
// cluster).
//---------------------------------------------------------------------------
void TfDynamicTemp::sbTempICClearClick()
{
    for(int j=0; j<16; j++)
        SetEdit[j]->Text="";

    for(int j=0; j<18; j++)
        Chart1->Series[j]->Clear();

    ListBox1->Clear();
    btestReadTemp=false;    //kevin 20130812 溫度IC使用
}
//---------------------------------------------------------------------------
// golden :123-128. GATE (C1) narrow (golden :126-127) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::lblRealTime2Click()
{
    ListBox1->Items->Insert(0,"ReStart Temp IC Comm Port");
#if 0 // GATE (C1) -- golden :126-127, COM2->TempComm6 has no port (see forms/fDynamicTemp.h)
    COM2->TempComm6->StopComm();
    COM2->TempComm6->StartComm();  //僅能啟動一次
#endif // GATE (C1)
}
//---------------------------------------------------------------------------
// golden :130-142.
//---------------------------------------------------------------------------
void TfDynamicTemp::btShowClick()
{
    int i;
    for(i=0; i<18; i++)
    {
        if(myCB[i]->Checked==true)
            Chart1->Series[i]->Active=true;
        else
            Chart1->Series[i]->Active=false;
    }
    btestReadTemp=true;  //kevin 20130812
    iTempICTask=1;    //kevin 20130812
}
//---------------------------------------------------------------------------
// golden :144-150. GATE (Q1) narrow (golden :147) -- see file banner.
// DEVIATION: zero-parameter signature -- Sender's only read was inside the
// gated call; Button/Shift/X/Y never read (see forms/fDynamicTemp.h).
//---------------------------------------------------------------------------
void TfDynamicTemp::edMinMouseDown()
{
#if 0 // GATE (Q1) -- golden :147, fQwertyKey has no port anywhere in this tree (see forms/fDynamicTemp.h)
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 20, atoi(edMin->Text.c_str()));
#endif // GATE (Q1)
    int min=atoi(edMin->Text.c_str());
    Chart1->LeftAxis->Minimum=min;
}
//---------------------------------------------------------------------------
// golden :152-158. GATE (Q1) narrow (golden :155) -- see file banner.
// GOLDEN ODDITY, not a translation artifact: the gated ShowQwertyKey call
// reads `edMin->Text` (not `edMax`) as its "current value" argument -- a
// golden copy-paste artifact from edMinMouseDown, preserved verbatim inside
// the gate text (inert either way while GATE (Q1) stays closed).
//---------------------------------------------------------------------------
void TfDynamicTemp::edMaxMouseDown()
{
#if 0 // GATE (Q1) -- golden :155, fQwertyKey has no port anywhere in this tree (see forms/fDynamicTemp.h)
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 200, atoi(edMin->Text.c_str()));
#endif // GATE (Q1)
    int max=atoi(edMax->Text.c_str());
    Chart1->LeftAxis->Maximum=max;
}
//---------------------------------------------------------------------------
// golden :160-173.
//---------------------------------------------------------------------------
void TfDynamicTemp::FormResize()
{
    edMin           ->Top    =Chart1->Height-30;
    btShow          ->Height =Panel1->Height/2;
    btSelectAll     ->Height =Panel1->Height/2;
    sbTempICClear   ->Height =Panel1->Height/2;
    btDefaultPos    ->Height =Panel1->Height/2;
    btShow          ->Left   =Panel1->Width-60;
    btSelectAll     ->Left   =Panel1->Width-120;
    sbTempICClear   ->Left   =Panel1->Width-60;
    btDefaultPos    ->Left   =Panel1->Width-120;
    sbTempICClear   ->Top    =Panel1->Height/2;
    btDefaultPos    ->Top    =Panel1->Height/2;
}
//---------------------------------------------------------------------------
// golden :175-187. GATE (Q1) narrow (golden :178) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::edLowerMouseDown()
{
#if 0 // GATE (Q1) -- golden :178, fQwertyKey has no port anywhere in this tree (see forms/fDynamicTemp.h)
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 20, atoi(edLower->Text.c_str()));
#endif // GATE (Q1)

    int count=Chart1->Series[16]->Count();
    Chart1->Series[16]->Clear();

    for(int i=0; i<count; i++)
    {
        Chart1->Series[16]->AddY(atof(edLower->Text.c_str()), "", TC[16]);
    }
}
//---------------------------------------------------------------------------
// golden :189-201. GATE (Q1) narrow (golden :192) -- see file banner.
// GOLDEN ODDITY, not a translation artifact: golden :199 (`TC[16]`) reuses
// Series[16]'s colour for Series[17] instead of `TC[17]` -- a copy-paste
// artifact from edLowerMouseDown (which correctly uses TC[16] for
// Series[16]); btTempICLoadClick's own AddY calls (golden :95-96) DO use
// TC[16]/TC[17] correctly for Series[16]/Series[17] respectively, so this is
// an inconsistency within golden itself, not a systematic redefinition of
// Series[17]'s colour. Translated literally per this project's "照翻，並在
// //AI 註解寫下它為什麼看起來錯" rule -- not changed to TC[17].
//---------------------------------------------------------------------------
void TfDynamicTemp::edUpperMouseDown()
{
#if 0 // GATE (Q1) -- golden :192, fQwertyKey has no port anywhere in this tree (see forms/fDynamicTemp.h)
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, atoi(edUpper->Text.c_str()), atoi(edLower->Text.c_str()));
#endif // GATE (Q1)

    int count=Chart1->Series[17]->Count();
    Chart1->Series[17]->Clear();

    for(int i=0; i<count; i++)
    {
        Chart1->Series[17]->AddY(atof(edUpper->Text.c_str()), "", TC[16]);   // GOLDEN ODDITY golden :199 -- see comment above
    }
}
//---------------------------------------------------------------------------
// golden :203-209.
//---------------------------------------------------------------------------
void TfDynamicTemp::btSelectAllClick()
{
    for(int i=0; i<18; i++)
    {
        myCB[i]->Checked=true;
    }
}
//---------------------------------------------------------------------------
// golden :211-220. DEVIATION: Sender cast target TfDynamicTempGroupBox*, not
// TGroupBox* -- see forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
void TfDynamicTemp::gbA1MouseUp(void *Sender)
{
    TfDynamicTempGroupBox *Ptr;
    Ptr=(TfDynamicTempGroupBox *)Sender;
    if(Ptr->Tag!=0)
    {
        Ptr->Tag=0;
    }
}
//---------------------------------------------------------------------------
// golden :222-231. DEVIATION: Sender cast target TfDynamicTempGroupBox*, not
// TGroupBox* -- see forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
void TfDynamicTemp::gbA1MouseDown(void *Sender, int X, int Y)
{
    TfDynamicTempGroupBox *Ptr;
    Ptr=(TfDynamicTempGroupBox *)Sender;
    if(Ptr->Tag!=1)
        Ptr->Tag=1;
    iStartX=X;
    iStartY=Y;
}
//---------------------------------------------------------------------------
// golden :233-243. DEVIATION: Sender cast target TfDynamicTempGroupBox*, not
// TGroupBox* -- see forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
void TfDynamicTemp::gbA1MouseMove(void *Sender, int X, int Y)
{
    TfDynamicTempGroupBox *Ptr;
    Ptr=(TfDynamicTempGroupBox *)Sender;
    if(Ptr->Tag==1)
    {
        Ptr->Left=Ptr->Left+(X-iStartX);
        Ptr->Top =Ptr->Top+(Y-iStartY);
    }
}
//---------------------------------------------------------------------------
// golden :245-251. DEVIATION: Sender cast target TfDynamicTempPanel*, not
// TPanel* -- see forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
void TfDynamicTemp::btSelectAllMouseDown(void *Sender)
{
    TfDynamicTempPanel *Ptr;
    Ptr=(TfDynamicTempPanel *)Sender;
    Ptr->BevelOuter=bvLowered;
}
//---------------------------------------------------------------------------
// golden :253-259. DEVIATION: Sender cast target TfDynamicTempPanel*, not
// TPanel* -- see forms/fDynamicTemp.h.
//---------------------------------------------------------------------------
void TfDynamicTemp::btSelectAllMouseUp(void *Sender)
{
    TfDynamicTempPanel *Ptr;
    Ptr=(TfDynamicTempPanel *)Sender;
    Ptr->BevelOuter=bvRaised;
}
//---------------------------------------------------------------------------
// golden :261-273. GOLDEN ODDITY, not a translation artifact: the branch
// condition compares `atoi(edMax->Text)` against `Chart1->LeftAxis->Minimum`
// (not `->Maximum`) even though the branch is about to reassign Minimum --
// an asymmetric compare golden itself carries. Translated literally.
//---------------------------------------------------------------------------
void TfDynamicTemp::FormShow()
{
    if(atoi(edMax->Text.c_str())<Chart1->LeftAxis->Minimum)
    {
        Chart1->LeftAxis->Minimum=atoi(edMin->Text.c_str());
        Chart1->LeftAxis->Maximum=atoi(edMax->Text.c_str());
    }
    else
    {
        Chart1->LeftAxis->Maximum=atoi(edMax->Text.c_str());
        Chart1->LeftAxis->Minimum=atoi(edMin->Text.c_str());
    }
}
//---------------------------------------------------------------------------
// golden :275-313.
//---------------------------------------------------------------------------
void TfDynamicTemp::btDefaultPosClick()
{
    gbA1->Top       =0;
    gbA1->Left      =4;
    gbB2->Top       =50;
    gbB2->Left      =4;
    gbA3->Top       =0;
    gbA3->Left      =84;
    gbB4->Top       =50;
    gbB4->Left      =84;
    gbC5->Top       =0;
    gbC5->Left      =164;
    gbD6->Top       =50;
    gbD6->Left      =164;
    gbC7->Top       =0;
    gbC7->Left      =244;
    gbD8->Top       =50;
    gbD8->Left      =244;
    gbE9->Top       =0;
    gbE9->Left      =324;
    gbF10->Top      =50;
    gbF10->Left     =324;
    gbE11->Top      =0;
    gbE11->Left     =404;
    gbF12->Top      =50;
    gbF12->Left     =404;
    gbG13->Top      =0;
    gbG13->Left     =484;
    gbH14->Top      =50;
    gbH14->Left     =484;
    gbG15->Top      =0;
    gbG15->Left     =564;
    gbH16->Top      =50;
    gbH16->Left     =564;
    gbUpper->Top    =0;
    gbUpper->Left   =644;
    gbLower->Top    =50;
    gbLower->Left   =644;
}
//---------------------------------------------------------------------------
// golden :315-321. GATE (C1) narrow (golden :318) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::gbUpperDblClick()
{
    AnsiString asWriteComm=":StartTest+";
#if 0 // GATE (C1) -- golden :318, COM2->TempComm6 has no port (see forms/fDynamicTemp.h)
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(), asWriteComm.Length());
#endif // GATE (C1)
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0, asWriteComm);
}
//---------------------------------------------------------------------------
// golden :323-328. GATE (C1) narrow (golden :326-327) -- see file banner.
//---------------------------------------------------------------------------
void TfDynamicTemp::gbLowerDblClick()
{
    ListBox1->Items->Insert(0,"ReStart Temp IC Comm Port");
#if 0 // GATE (C1) -- golden :326-327, COM2->TempComm6 has no port (see forms/fDynamicTemp.h)
    COM2->TempComm6->StopComm();
    COM2->TempComm6->StartComm();  //僅能啟動一次
#endif // GATE (C1)
}
//---------------------------------------------------------------------------
// golden :330-337.
//---------------------------------------------------------------------------
void TfDynamicTemp::Timer1Timer()
{
    if(InitialOK==false)
        return;

    if(btestReadTemp)   //kevin 20130808
        GetTempICResult();
}
//---------------------------------------------------------------------------
// golden :339-345. GATE (C1) narrow (golden :342) -- see file banner.
// Identical body shape to lblRealTime6Click/gbUpperDblClick -- see file
// banner's GATE (C1) note on why all such sites are treated the same.
//---------------------------------------------------------------------------
void TfDynamicTemp::Button1Click()
{
    AnsiString asWriteComm=":StartTest+";
#if 0 // GATE (C1) -- golden :342, COM2->TempComm6 has no port (see forms/fDynamicTemp.h)
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(), asWriteComm.Length());
#endif // GATE (C1)
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0, asWriteComm);
}
//---------------------------------------------------------------------------
