#include "MachineDefine.h"
#pragma hdrstop

#include "INPUT.h"

#include "common.h"
#include "MachineType.h"
#include "cmydef.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfInput *fInput;

bool bFloatFormat=false;
bool bDoubleFormat=false;
bool bPassword=false;
//---------------------------------------------------------------------------
__fastcall TfInput::TfInput(TComponent* Owner)
     : TForm(Owner)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Button15Click(TObject *Sender)
{
    Result=atoi(Edit1->Text.c_str());
    fResult=atof(Edit1->Text.c_str());
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfInput::NumberClick(TObject *Sender)
{
    //if(LastSet.bPasswordCanInputByMouse==false &&bPassword)
      //  return;

    if(bFirstEnter)
    {
        bFirstEnter=false;
        Edit1->Text="";
    }
    char str[2]={"0"};
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    str[0]='0'+Ptr->Tag;
    Edit1->Text=Edit1->Text+AnsiString(str);
}
//---------------------------------------------------------------------------
void __fastcall TfInput::FormShow(TObject *Sender)
{
    Left=(XResolution-Width)/2;                                                 //Steven 20230912 : 調整畫面為FULL HD
    Top =(XResolution-Height)/2;
    Back=Edit1->Text;
    Button1->SetFocus();
    Edit1->SetFocus();
    bFirstEnter=true;
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Button13Click(TObject *Sender)
{
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Edit1->Text="";
    }

    if(Edit1->Text.Length()<=0)
        return;
    Edit1->Text=Edit1->Text.SubString(0, Edit1->Text.Length()-1);
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Edit1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key==0x0d)
    {
        Result=atoi(Edit1->Text.c_str());
        fResult=atof(Edit1->Text.c_str());
        Close();
    }
}
//---------------------------------------------------------------------------
void MyInputBox(TEdit *Ptr)
{
    bFloatFormat=false;
    bDoubleFormat=false;
    fInput->SpeedButton1->Caption="+100";
    fInput->SpeedButton2->Caption="+10";
    fInput->SpeedButton3->Caption="+1";

    fInput->SpeedButton5->Caption="-100";
    fInput->SpeedButton6->Caption="-10";
    fInput->SpeedButton7->Caption="-1";

    fInput->Result=atoi( Ptr->Text.c_str());
    fInput->fResult=atof( Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=false;
    fInput->ShowModal();
    Ptr->Text=fInput->Result;
}
//---------------------------------------------------------------------------
void MyPassWordInputBox(TEdit *Ptr)
{
    char c;
    bPassword=true;
    fInput->Button4->Visible=false;
    fInput->Button13->Visible=true;
    fInput->SpeedButton4->Visible=false;
    fInput->Button17->Visible=false;
    fInput->SpeedButton1->Visible=false;
    fInput->SpeedButton2->Visible=false;
    fInput->SpeedButton3->Visible=false;
    fInput->SpeedButton5->Visible=false;
    fInput->SpeedButton6->Visible=false;
    fInput->SpeedButton7->Visible=false;

    fInput->Result=atoi( Ptr->Text.c_str());
    fInput->fResult=atof( Ptr->Text.c_str());
    fInput->Edit1->Text="";
    c=fInput->Edit1->PasswordChar;
    fInput->Edit1->PasswordChar='*';
    fInput->ShowModal();
    fInput->Edit1->PasswordChar=c;
    fInput->Button4->Visible=true;
    fInput->Button13->Visible=true;
    fInput->SpeedButton4->Visible=true;
    fInput->Button17->Visible=true;
    fInput->SpeedButton1->Visible=true;
    fInput->SpeedButton2->Visible=true;
    fInput->SpeedButton3->Visible=true;
    fInput->SpeedButton5->Visible=true;
    fInput->SpeedButton6->Visible=true;
    fInput->SpeedButton7->Visible=true;
    bPassword=false;
    Ptr->Text=fInput->Edit1->Text;
}
//---------------------------------------------------------------------------
void MyFloatInputBox(TEdit *Ptr)
{
    bFloatFormat=true;
    bDoubleFormat=false;
    fInput->SpeedButton1->Caption="+1.00";
    fInput->SpeedButton2->Caption="+0.10";
    fInput->SpeedButton3->Caption="+0.01";

    fInput->SpeedButton5->Caption="-1.00";
    fInput->SpeedButton6->Caption="-0.10";
    fInput->SpeedButton7->Caption="-0.01";

    fInput->Result=atoi(Ptr->Text.c_str());
    fInput->fResult=atof(Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=true;
    fInput->ShowModal();
    Ptr->Text=fInput->Edit1->Text;
}
//---------------------------------------------------------------------------
void MyFloatInputBox2(TEdit *Ptr)                                               //wei 20150708 直接顯示小數點後兩位
{
    bFloatFormat=true;
    bDoubleFormat=false;
    fInput->SpeedButton1->Caption="+1.00";
    fInput->SpeedButton2->Caption="+0.10";
    fInput->SpeedButton3->Caption="+0.01";

    fInput->SpeedButton5->Caption="-1.00";
    fInput->SpeedButton6->Caption="-0.10";
    fInput->SpeedButton7->Caption="-0.01";

    fInput->Result=atoi(Ptr->Text.c_str());
    fInput->fResult=atof(Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=true;
    fInput->ShowModal();
    Ptr->Text=FormatFloat("0.000", fInput->Edit1->Text.ToDouble());             //jou 2016-04-22 直接顯示小數點後三位
}
//---------------------------------------------------------------------------
void MyTempFloatInputBox(TEdit *Ptr)
{
    bFloatFormat=true;
    bDoubleFormat=false;
    fInput->SpeedButton1->Caption="+10.0";
    fInput->SpeedButton2->Caption="+1.00";
    fInput->SpeedButton3->Caption="+0.10";

    fInput->SpeedButton5->Caption="-10.0";
    fInput->SpeedButton6->Caption="-1.00";
    fInput->SpeedButton7->Caption="-0.10";

    fInput->Result=atoi(Ptr->Text.c_str());
    fInput->fResult=atof(Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=true;
    fInput->ShowModal();
    Ptr->Text=fInput->Edit1->Text;
}
//---------------------------------------------------------------------------
void MyDoubleInputBox(TEdit *Ptr)                                               //Steven 20090716
{
    bFloatFormat=false;
    bDoubleFormat=true;
    fInput->SpeedButton1->Caption="+0.0100";
    fInput->SpeedButton2->Caption="+0.0010";
    fInput->SpeedButton3->Caption="+0.0001";

    fInput->SpeedButton5->Caption="-0.0100";
    fInput->SpeedButton6->Caption="-0.0010";
    fInput->SpeedButton7->Caption="-0.0001";

    fInput->Result=atof(Ptr->Text.c_str());
    fInput->fResult=atof(Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=true;
    fInput->ShowModal();
    Ptr->Text=fInput->Edit1->Text;
}
//---------------------------------------------------------------------------
void MySuperInputBox(TEdit *Ptr, double base, double Max, double Min)           //Steven 20120209
{
    if(base>=1.0)
    {
        bFloatFormat=false;
        bDoubleFormat=false;
    }
    else if(base>=0.001)
    {
        bFloatFormat=true;
        bDoubleFormat=false;
    }
    else
    {
        bFloatFormat=false;
        bDoubleFormat=true;
    }

    fInput->SpeedButton1->Caption="+"+AnsiString(base*100.0);
    fInput->SpeedButton2->Caption="+"+AnsiString(base*10.0);
    fInput->SpeedButton3->Caption="+"+AnsiString(base);

    fInput->SpeedButton5->Caption="-"+AnsiString(base*100.0);
    fInput->SpeedButton6->Caption="-"+AnsiString(base*10.0);
    fInput->SpeedButton7->Caption="-"+AnsiString(base);

    fInput->Result=atof(Ptr->Text.c_str());
    fInput->fResult=atof(Ptr->Text.c_str());
    fInput->Edit1->Text=Ptr->Text;
    fInput->Button17->Visible=true;
    fInput->Label1->Caption="The limitation is between "+AnsiString(Max)+" ~ "+AnsiString(Min);
    fInput->Label1->Visible=true;
    fInput->ShowModal();
    Ptr->Text=AnsiString(CheckRange(atof(fInput->Edit1->Text.c_str()), Max, Min));
    fInput->Label1->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Button4Click(TObject *Sender)
{
    AnsiString S;
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Edit1->Text="-";
    }
    else
    {
        S=Edit1->Text;
        if(S.Pos("-")==1)
            S=S.SubString(2,S.Length());
        else
            S="-"+S;
        Edit1->Text=S;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Button17Click(TObject *Sender)
{
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Edit1->Text="";
    }
    char str[2]={"0"};
    //TButton *Ptr;
    //Ptr=(TButton *)Sender;
    str[0]='.';
    Edit1->Text=Edit1->Text+AnsiString(str);
}
//---------------------------------------------------------------------------
void __fastcall TfInput::SpeedButton1Click(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    AnsiString str;
    double fT=0.0, fS=0.0;
    int   iT, iS;

    if(bFloatFormat)
    {
        fT=atof(Ptr->Caption.c_str());
        fS=atof(Edit1->Text.c_str());
        fS+=fT;
        Edit1->Text=str.sprintf("%1.3f", fS);
//        Edit1->Text=GetFloatFormatString(fS, 6, 3);
    }
    else if(bDoubleFormat)
    {
        fT=atof(Ptr->Caption.c_str());
        fS=atof(Edit1->Text.c_str());
        fS+=fT;
        Edit1->Text=str.sprintf("%1.6f", fS);
        //Edit1->Text=GetFloatFormatString(fS, 6, 6);
    }
    else
    {
        iT=atoi(Ptr->Caption.c_str());
        iS=atof(Edit1->Text.c_str());
        iS+=iT;
        Edit1->Text=iS;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfInput::SpeedButton4Click(TObject *Sender)
{
    Edit1->Text=Back;
    Button15Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Button17KeyPress(TObject *Sender, char &Key)
{
    if(Button17->Visible)
    {
        if(OnlyNumberAndDotInPut(Key)==false)
            Key=NULL;
    }
    else
    {
        if(OnlyNumberInPut(Key)==false)
            Key=NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfInput::Edit1KeyPress(TObject *Sender, char &Key)
{
    if(Button17->Visible)//can dot input
    {
        if(OnlyNumberAndDotInPut(Key)==false)
            Key=NULL;
    }
    else
    {
        if(OnlyNumberInPut(Key)==false)
            Key=NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfInput::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
