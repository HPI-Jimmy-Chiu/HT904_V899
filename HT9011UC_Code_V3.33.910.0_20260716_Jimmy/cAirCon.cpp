#include "MachineDefine.h"
#pragma hdrstop

#include "cAirCon.h"

#include "MachineType.h"
#include "mymessbox.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
#include "myswitch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TfAirCon *fAirCon;
//---------------------------------------------------------------------------
__fastcall TfAirCon::TfAirCon(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfAirCon::AddMessage(AnsiString Str)
{
    if(Memo1->Lines->Count>1024)
        Memo1->Clear();

    Memo1->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::sbtExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::sbUpdateClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    switch(rgAirConCommand->ItemIndex)
    {
        case eCLS:                                                              //致能輸出電壓
            SetCLS();
            break;
        case ePwrOn:                                                            //開啟輸出電壓
            StartOutput(true);
            break;
        case ePwrOff:                                                           //關閉輸出電壓
            StartOutput(false);
            break;
        case ePwrSts:                                                           //回傳power supply狀態
            GetStatus();
            break;
        case eSetCur:                                                           //設定電壓電流值
            SetCurrent(atof(edOutputCurrent->Text.c_str()));
            break;
        case eGetCur:                                                           //讀取電流
            GetCurrent();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAirCon::SendCommand(AnsiString Str)
{
    AnsiString s;
    char cStr[256];

    s.sprintf("Write : %s", Str);
    sprintf(cStr,"%s%c", Str, char(0x0A));

    AddMessage(s);
    CommAirCon->WriteCommData(cStr, strlen(cStr));
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::CommAirConReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    char *cPtr=(char *)Buffer;
    char cStr[512];

    if(BufferLength>512)                                                        //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        AddMessage(LogMsg);
        return;
    }

    sprintf(cStr, "Read  : %s", cPtr);
    AddMessage(cPtr);
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::InitialAirConCom(AnsiString ComPort, TParity Parity, unsigned Rate, TByteSize ByteSize, bool ParityCheck, TStopBits StopBits)
{
    CommAirCon->CommName       = "\\\\.\\"+ComPort;
    CommAirCon->Parity         = Parity;
    CommAirCon->BaudRate       = Rate;
    CommAirCon->ByteSize       = ByteSize;
    CommAirCon->ParityCheck    = ParityCheck;
    CommAirCon->StopBits       = StopBits;
    if(GetCOMPortStatus(ComPort))
    {
        try
        {
            CommAirCon->StartComm();                                            //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfAirCon::InitialAirConCom");
            #ifdef HT9045
                ShowMyMessage("Air Con : "+ComPort+" port error", "");
            #else
                ShowMyMessage("Air Con : "+ComPort+" port error");
            #endif
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::FormDestroy(TObject *Sender)
{
    try
    {
        if(USE_AIR_CONDITIONER==2)
        {
            PowerDown();
            CommAirCon->StopComm();
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAirCon::FormDestroy");
    }
    LogSoftwareOffTime("TfAirCon, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfAirCon::StartOutput(bool bEnable)                                        //開啟輸出電壓
{
    if(bEnable)
        SendCommand(":OUTPut:STATe 1");
    else
        SendCommand(":OUTPut:STATe 0");
}
//---------------------------------------------------------------------------
void TfAirCon::SetCurrent(double dCurrent)                                      //設定電流
{
    AnsiString Str;
    dCurrent=CheckRange(dCurrent, 5.8, 0.0);                                    //最大輸出為5.8A
    Str.sprintf(":CHAN1:VOLT 00.00;CURR %5.2f", dCurrent);
    SendCommand(Str);
}
//---------------------------------------------------------------------------
void TfAirCon::SetCLS()                                                         //致能輸出電壓
{
    SendCommand("*CLS");
}
//---------------------------------------------------------------------------
void TfAirCon::GetStatus()                                                      //讀取Power狀態
{
    SendCommand(":OUTPut:STATe?");
}
//---------------------------------------------------------------------------
void TfAirCon::GetCurrent()                                                     //讀取電流
{
    SendCommand("CHAN1:CURR ?");
}
//---------------------------------------------------------------------------
void TfAirCon::PowerUp(double dCurrent)
{
    if(USE_AIR_CONDITIONER==2)                                                  //Steven 20131011 : 冷氣機
    {
        SetCurrent(dCurrent);
        StartOutput(true);
        SetCLS();
    }
    SW[SwAirConditioner].On();
}
//---------------------------------------------------------------------------
void TfAirCon::PowerDown()
{
    if(USE_AIR_CONDITIONER==2)                                                  //Steven 20131011 : 冷氣機
    {
        SetCurrent(0.0);
        StartOutput(false);
    }
    SW[SwAirConditioner].Off();
}
//---------------------------------------------------------------------------
void __fastcall TfAirCon::edOutputCurrentMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 1, true, 5.8, 0.0);
}
//---------------------------------------------------------------------------

