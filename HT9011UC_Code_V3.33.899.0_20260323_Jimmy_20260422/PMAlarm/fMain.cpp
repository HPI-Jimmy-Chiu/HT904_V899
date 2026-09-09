//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fMain.h"

#include "PMAlarmInterFace.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    fPMAlarmInterFace->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
    fPMAlarmInterFace->PMAlarm_SYS.LoadPMAlarm();
    fPMAlarmInterFace->PMAlarm_SYS.SetMachineNo("M001");
    fPMAlarmInterFace->PMAlarm_SYS.SavePMAlarm();
    fPMAlarmInterFace->Show();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    fPMAlarmInterFace->PMAlarm_SYS.LoadPMAlarm();
    fPMAlarmInterFace->PMAlarm_SYS.SetMachineNo("M005");
    fPMAlarmInterFace->PMAlarm_SYS.SavePMAlarm();
}
//---------------------------------------------------------------------------

