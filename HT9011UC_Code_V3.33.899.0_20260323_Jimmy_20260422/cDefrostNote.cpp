#include "IncludeAllHeader.H"
#pragma hdrstop

#include "cDefrostNote.h"
#include "TriTemp.h"
//#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfDefrostNote *fDefrostNote;
bool bBlink = false;
//---------------------------------------------------------------------------
__fastcall TfDefrostNote::TfDefrostNote(TComponent* Owner)
    : TForm(Owner)
{
    btn_Interrupt->Enabled = false;
//    fDefrostNote->SendToBack();
}
//---------------------------------------------------------------------------
void __fastcall TfDefrostNote::btn_StartClick(TObject *Sender)
{
    bStartDefrost = true;
    btn_Interrupt->Enabled = true;
    btn_Start    ->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfDefrostNote::Timer1Timer(TObject *Sender)
{
    if(Retrrn_InitialOK()==false)
        return;

    if(Retrrn_AccessLevel()==0)
        btn_Interrupt->Enabled = false;

    pnl_ShowMessage->Caption = sShowMessage;
    if(bBlink==false)
    {
        bBlink = true;
        pnl_ShowMessage->Font->Color = clRed;
    }
    else
    {
        bBlink = false;
        pnl_ShowMessage->Font->Color = clWindowText;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDefrostNote::btn_InterruptClick(TObject *Sender)
{
    int ret;
    ret=MessageDlg("Are you sure interrupt defrost? (你確定中斷除霜嗎?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
    if(ret==mrYes)
    {
        bInterruptDefrost = true;
        bStartDefrost = false;
        btn_Interrupt->Enabled = false;
        btn_Start    ->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDefrostNote::FormShow(TObject *Sender)
{
    bShow = true;
}
//---------------------------------------------------------------------------
void __fastcall TfDefrostNote::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow = false;
}
//---------------------------------------------------------------------------
