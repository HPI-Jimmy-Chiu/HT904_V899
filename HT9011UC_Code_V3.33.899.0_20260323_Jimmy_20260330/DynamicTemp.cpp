#include "MachineDefine.h"
#pragma hdrstop

#include "DynamicTemp.h"

#include "rs232.h"
#include "cmydef.h"
#include "cprod.h"
//#include "INPUT.h"
//#include "atester.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfDynamicTemp *fDynamicTemp;
bool btestReadTemp=false;       //kevin 20130808 放ic
extern int iTempICTask;         //kevin 20130808 IC秖代放
extern bool GetTempICResult();  //kevin 20130808 IC秖代放
//---------------------------------------------------------------------------
__fastcall TfDynamicTemp::TfDynamicTemp(TComponent* Owner)
    : TForm(Owner)
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
void __fastcall TfDynamicTemp::btExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::lblRealTime6Click(TObject *Sender)
{
    AnsiString asWriteComm=":StartTest+";
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(),asWriteComm.Length());
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0,asWriteComm);
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::lblRealTime1DblClick(TObject *Sender)
{
    ListBox1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::btTempICLoadClick(TObject *Sender)
{
    int i, iPos;
    double fTemp[20];
    char cData[20], cTime[20];

    AnsiString asFind,asCh;

    if(OpenDialog2->Execute())
    {
        ListBox1->Clear();
        sbTempICClearClick(this);

        ListBox1->Items->LoadFromFile(OpenDialog2->FileName);

        for(int ct=ListBox1->Items->Count-2;ct>0;ct--)
        {
            if(ListBox1->Items->Strings[ct].Pos("T1:")<=0)
                continue;

            sprintf(cData, "%s", ListBox1->Items->Strings[ct].SubString(1,  10));
            sprintf(cTime, "%s", ListBox1->Items->Strings[ct].SubString(12, 8));

            for(i=0; i<16; i++)
            {
                asCh.printf("T%d:", (i+1));
                iPos=ListBox1->Items->Strings[ct].Pos(asCh);
                asFind=ListBox1->Items->Strings[ct].SubString(iPos+asCh.Length(), 6);
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
void __fastcall TfDynamicTemp::btTempICSaveClick(TObject *Sender)
{
    SaveDialog1->FileName="D:\\Dynamic.txt";    //Steven 20120921 : 郎玂臔
    if(SaveDialog1->Execute())
    {
        ListBox1->Items->SaveToFile(SaveDialog1->FileName);
        ListBox1->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::sbTempICClearClick(TObject *Sender)
{
    for(int j=0; j<16; j++)
        SetEdit[j]->Text="";

    for(int j=0; j<18; j++)
        Chart1->Series[j]->Clear();

    ListBox1->Clear();
    btestReadTemp=false;    //kevin 20130812 放ICㄏノ
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::lblRealTime2Click(TObject *Sender)
{
    ListBox1->Items->Insert(0,"ReStart Temp IC Comm Port");
    COM2->TempComm6->StopComm();
    COM2->TempComm6->StartComm();  //度币笆Ω
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::btShowClick(TObject *Sender)
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
void __fastcall TfDynamicTemp::edMinMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 20, atoi(edMin->Text.c_str()));
    int min=atoi(edMin->Text.c_str());
    Chart1->LeftAxis->Minimum=min;
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::edMaxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 200, atoi(edMin->Text.c_str()));
    int max=atoi(edMax->Text.c_str());
    Chart1->LeftAxis->Maximum=max;
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::FormResize(TObject *Sender)
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
void __fastcall TfDynamicTemp::edLowerMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, 20, atoi(edLower->Text.c_str()));

    int count=Chart1->Series[16]->Count();
    Chart1->Series[16]->Clear();

    for(int i=0; i<count; i++)
    {
        Chart1->Series[16]->AddY(atof(edLower->Text.c_str()), "", TC[16]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::edUpperMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, atoi(edUpper->Text.c_str()), atoi(edLower->Text.c_str()));

    int count=Chart1->Series[17]->Count();
    Chart1->Series[17]->Clear();

    for(int i=0; i<count; i++)
    {
        Chart1->Series[17]->AddY(atof(edUpper->Text.c_str()), "", TC[16]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::btSelectAllClick(TObject *Sender)
{
    for(int i=0; i<18; i++)
    {
        myCB[i]->Checked=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::gbA1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=0)
    {
        Ptr->Tag=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::gbA1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=1)
        Ptr->Tag=1;
    iStartX=X;
    iStartY=Y;
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::gbA1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag==1)
    {
        Ptr->Left=Ptr->Left+(X-iStartX);
        Ptr->Top =Ptr->Top+(Y-iStartY);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::btSelectAllMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;
    Ptr->BevelOuter=bvLowered;
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::btSelectAllMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;
    Ptr->BevelOuter=bvRaised;
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::FormShow(TObject *Sender)
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
void __fastcall TfDynamicTemp::btDefaultPosClick(TObject *Sender)
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
void __fastcall TfDynamicTemp::gbUpperDblClick(TObject *Sender)
{
    AnsiString asWriteComm=":StartTest+";
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(), asWriteComm.Length());
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0, asWriteComm);
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::gbLowerDblClick(TObject *Sender)
{
    ListBox1->Items->Insert(0,"ReStart Temp IC Comm Port");
    COM2->TempComm6->StopComm();
    COM2->TempComm6->StartComm();  //度币笆Ω
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::Timer1Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    if(btestReadTemp)   //kevin 20130808
        GetTempICResult();
}
//---------------------------------------------------------------------------
void __fastcall TfDynamicTemp::Button1Click(TObject *Sender)
{
    AnsiString asWriteComm=":StartTest+";
    COM2->TempComm6->WriteCommData(asWriteComm.c_str(), asWriteComm.Length());
    bTempComm6ReceiveOK=false;
    ListBox1->Items->Insert(0, asWriteComm);
}
//---------------------------------------------------------------------------
