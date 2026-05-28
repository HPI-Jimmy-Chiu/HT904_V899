#include "MachineDefine.h"
#pragma hdrstop

#include "uTechShow.h"

#include "uteach.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTeachShow *fTeachShow;
//---------------------------------------------------------------------------
__fastcall TfTeachShow::TfTeachShow(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfTeachShow::Button1Click(TObject *Sender)
{
    fTeach->GetTechPos();
//    Teachshow = false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfTeachShow::Button2Click(TObject *Sender)
{
//    Teachshow = false;
    Close();
}
//---------------------------------------------------------------------------
