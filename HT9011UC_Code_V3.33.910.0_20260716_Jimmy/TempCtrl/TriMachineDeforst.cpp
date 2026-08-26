//---------------------------------------------------------------------------
//#include "IncludeAllHeader.H"
#include <vcl.h>
#pragma hdrstop

#include "TriMachineDeforst.h"
#include "TriTemp.h"
#include "cinitial.h"
#include "main.h"
#include "ATC_Bridge.h"
#include "ATC_Handler_Side.h"
#include "cprod.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "butPa1"
#pragma resource "*.dfm"
TfrmDefrost *frmDefrost;
//---------------------------------------------------------------------------
__fastcall TfrmDefrost::TfrmDefrost(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::tmrGetTemperatureStatusTimer(TObject *Sender)
{
    /*
    tmrGetTemperatureStatus->Enabled = false;
    const int iChannelSize = 20;
    int iChannelMax;
    double dAtcNowTemp[iChannelSize]={0.0};
    double dAtcRefTemp[iChannelSize]={0.0};
    double dAtcSetTemp[iChannelSize]={0.0};
    for( int i = 0 ; i < iChannelMax ; i++ )
    {
        dAtcNowTemp[i] = ATCBridge_GetATCSiteNowTemperature(i);
        dAtcRefTemp[i] = ATCBridge_GetATCSiteNowTemperature_Ref(i);
    }
    tmrGetTemperatureStatus->Enabled = true;
    */
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::FormShow(TObject *Sender)
{
    m_dtTime = Now();
    tmrClose->Enabled = true;
    tmrMessageFlash->Enabled = true;
    sgTriMachineAutoDefrost->Cells[0][0]="Device";
    sgTriMachineAutoDefrost->Cells[1][0]="Status";
    lblSetTemp->Caption = IntToStr(IniConfig.iLA14_5_SetDefrostTempature);   //fMain->labDefrostSetTemp->Caption;
    lblSetTime->Caption = IntToStr(IniConfig.iLA14_6_SetDefrostTime);        //fMain->labDefrostSetTime->Caption;
    btnForceDefrost->Enabled = true;
    btnCloseFormWaitDefrost->Enabled = true;
//    if( ATC_CHANNEL_MODULE == 1 )                //Isaac 20181119 : Mark
//    {
//        tsTriTemp_20ChShow->TabVisible =false;
//        tsTriTemp_12ChShow->TabVisible =true;
//    }
//    else if( ATC_CHANNEL_MODULE == 2 )
//    {
//        tsTriTemp_12ChShow->TabVisible =false;
//        tsTriTemp_20ChShow->TabVisible =true;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmDefrost::tmrCloseTimer(TObject *Sender)
{
    tmrClose->Enabled = false;
    AnsiString sTime;

    unsigned short usHour, usMin, usSec, usMsec;
    Now().DecodeTime(&usHour, &usMin, &usSec, &usMsec);

    Word Year1, Month1, Day1, Hour1, Min1, Sec1, MSec1;
    Word Year, Month, Day, Hour, Min, Sec, MSec;
    TDateTime dtPresent = Now();
    DecodeTime(m_dtTime, Hour, Min, Sec, MSec);
    DecodeTime(dtPresent, Hour1, Min1, Sec1, MSec1);
    DecodeTime((m_dtTime-dtPresent), Hour1, Min1, Sec1, MSec1);
    tmrClose->Tag = 300 - (Sec1+Min1*60+Hour1*60*60+1);
    int isec =  (tmrClose->Tag)%60;
    int imin =  (tmrClose->Tag/60)%60;   //¤À
    int ihour = (tmrClose->Tag/3600)%24;   //®É
    //sTime.printf("%02d:%02d:%02d",(ihour),(imin),(isec));
    sTime.printf("%02d:%02d",(imin),(isec));
    m_labTmr->Caption = sTime;
    if (tmrClose->Tag <= 0 )
    {
         tmrClose->Enabled = false;
         gbAutoDefrostProduction->Enabled = false;
         bAutoDefrost_ByMachineProduction_CountDownEnd = true;
         return;
    }
    tmrClose->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDefrost::tmrMessageFlashTimer(TObject *Sender)
{
    tmrMessageFlash->Enabled = false;
    if (pnlChangeMsg_Eng->Color == 0x00BFFFFF)
    {
        pnlChangeMsg_Eng->Color =  0x00400000;
        pnlChangeMsg_Eng->Font->Color = 0x007FFF00;
    }
    else
    {
        pnlChangeMsg_Eng->Color =  0x00BFFFFF;
        pnlChangeMsg_Eng->Font->Color = 0x008000FF;
    }

    if (pnlChangeMsg_Chi->Color == 0x007FFF00)
    {
        pnlChangeMsg_Chi->Color =  0x00400000;
        pnlChangeMsg_Chi->Font->Color = 0x007FFF00;
    }
    else
    {
        pnlChangeMsg_Chi->Color =  0x007FFF00;
        pnlChangeMsg_Chi->Font->Color = 0x008000FF;
    }
    /*
    if(fMain->AccessLevel>2)
        btnClose->Visible = true;
    else
        btnClose->Visible = false;
     */
    tmrMessageFlash->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::btnCloseClick(TObject *Sender)
{
    //fAllDevicePosDefrostON_OFF("Off");
    tmrClose->Enabled = false;
    tmrMessageFlash->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::btnCloseFormWaitDefrostClick(TObject *Sender)
{
    bAutoDefrost_ByMachineProduction_Force = false;
    bAutoDefrost_ByMachineProduction_Wait = true;
    tmrClose->Enabled = false;
    tmrMessageFlash->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::btnForceDefrostClick(TObject *Sender)
{
    tmrClose->Enabled = false;
    m_labTmr->Caption = "00:00";
    bAutoDefrost_ByMachineProduction_Force = true;
    bAutoDefrost_ByMachineProduction_Wait = false;
    btnForceDefrost->Enabled = false;
    btnCloseFormWaitDefrost->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDefrost::btnDefrostTerminationClick(TObject *Sender)
{
    fAllDevicePosDefrostON_OFF("Off");
    bAutoDefrost_ByMachineHalf_Termination = true;
    bAutoDefrost_ByMachineProduction_Termination = true;
    tmrClose->Enabled = false;
    tmrMessageFlash->Enabled = false;
    Close();
}
//---------------------------------------------------------------------------

