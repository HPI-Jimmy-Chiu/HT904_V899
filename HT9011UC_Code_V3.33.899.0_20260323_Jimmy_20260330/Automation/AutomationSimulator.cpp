//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AutomationSimulator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain *Main;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TMain::ShowOLPState(int iState)
{
    AnsiString Name="";
    Name="Automation ";

    switch(iState)
    {
        case 0:
            lbSystemStatus->Caption=Name+"Off-Line";
            lbSystemStatus->Font->Color=clTeal;
            break;
        case 1:
            lbSystemStatus->Caption=Name+"On-Line";
            lbSystemStatus->Font->Color=clBlue;
            break;
        case 2:
            lbSystemStatus->Caption=Name+"Client Error";
            lbSystemStatus->Font->Color=clMaroon;
            break;
    }
    lbSystemStatus->Alignment=2;
}
