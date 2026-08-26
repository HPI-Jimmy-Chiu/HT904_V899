#include "MachineDefine.h"
#pragma hdrstop

#include "CCINPUT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCCInput *CCInput;

//---------------------------------------------------------------------------
__fastcall TCCInput::TCCInput(TComponent* Owner)
     : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::Button15Click(TObject *Sender)
{
    Result=atoi(Label1->Caption.c_str());
    fResult=atof(Label1->Caption.c_str());
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::NumberClick(TObject *Sender)
{
    //if(LastSet.bPasswordCanInputByMouse==false &&bPassword)
      //  return;

    if(bFirstEnter)
    {
        bFirstEnter=false;
        Label1->Caption="";
    }
    char str[2]={"0"};
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    str[0]='0'+Ptr->Tag;
    Label1->Caption=Label1->Caption+AnsiString(str);
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::FormShow(TObject *Sender)
{
    this->Height=387;
    this->Width=581;

    Back=Label1->Caption;
    Button1->SetFocus();
    //Label1->SetFocus();
    bFirstEnter=true;
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::Button13Click(TObject *Sender)
{
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Label1->Caption="";
    }

    if(Label1->Caption.Length()<=0)
        return;
    Label1->Caption=Label1->Caption.SubString(0, Label1->Caption.Length()-1);
}
//---------------------------------------------------------------------------
void MyCCInputBox(TEdit *Ptr)
{
    CCInput->SpeedButton1->Caption="+100";
    CCInput->SpeedButton2->Caption="+10";
    CCInput->SpeedButton3->Caption="+1";

    CCInput->SpeedButton5->Caption="-100";
    CCInput->SpeedButton6->Caption="-10";
    CCInput->SpeedButton7->Caption="-1";

    CCInput->Result=atoi( Ptr->Text.c_str());
    CCInput->fResult=atof( Ptr->Text.c_str());
    CCInput->Label1->Caption=Ptr->Text;
    CCInput->Button17->Visible=false;
    CCInput->ShowModal();
    Ptr->Text=CCInput->Result;
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::Button4Click(TObject *Sender)
{
    AnsiString S;
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Label1->Caption="-";
    }
    else
    {
        S=Label1->Caption;
        if(S.Pos("-")==1)
            S=S.SubString(2,S.Length());
        else
            S="-"+S;
        Label1->Caption=S;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::Button17Click(TObject *Sender)
{
    if(bFirstEnter)
    {
        bFirstEnter=false;
        Label1->Caption="";
    }
    char str[2]={"0"};
    str[0]='.';
    Label1->Caption=Label1->Caption+AnsiString(str);
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::SpeedButton1Click(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    int   iT, iS;
    iT=atoi(Ptr->Caption.c_str());
    iS=atof(Label1->Caption.c_str());
    iS+=iT;
    Label1->Caption=iS;
}
//---------------------------------------------------------------------------
void __fastcall TCCInput::SpeedButton4Click(TObject *Sender)
{
    Label1->Caption=Back;
    Button15Click(this);
}
//---------------------------------------------------------------------------
