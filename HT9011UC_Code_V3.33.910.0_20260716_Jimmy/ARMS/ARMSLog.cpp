//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "ARMSLog.h"
#include "ARMS.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfARMSLog *fARMSLog;
//---------------------------------------------------------------------------
__fastcall TfARMSLog::TfARMSLog(TComponent* Owner)
    : TForm(Owner)
{
    Width  = 515 ;
    Height = 435 ;
    bShow  = false;
}
//---------------------------------------------------------------------------
void __fastcall TfARMSLog::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;
    bSaveReady = false;
    bShow = true;
}
//---------------------------------------------------------------------------
void __fastcall TfARMSLog::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfARMSLog::FormDestroy");
    }
    LogSoftwareOffTime("TfARMSLog, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfARMSLog::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfARMSLog::sbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void TfARMSLog::SaveMemoLog()
{
    AnsiString filename;

    filename.sprintf("%s\\%s_%04d%02d%02d_%02d%02d%02d.txt", SavePath, SaveName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    MemoARMSLog->Lines->SaveToFile(filename);
}
//---------------------------------------------------------------------------
void __fastcall TfARMSLog::sbSaveClick(TObject *Sender)
{
    if(fARMS->CheckDirExit(3)==false)
    {
        ShowMessage("Please Check Coonect");
        return;
    }

    if(MemoARMSLog->GetTextLen()<=2)
    {
        ShowMessage("Please Keyin Data");
        return;
    }
    SaveMemoLog();
    bSaveReady=true;
    ShowMessage("Save Successful");
}
//---------------------------------------------------------------------------

