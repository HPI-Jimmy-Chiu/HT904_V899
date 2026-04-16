#include "MachineDefine.h"
#pragma hdrstop

#include "MemoryAlarm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMemoryAlarmForm *MemoryAlarmForm;
//---------------------------------------------------------------------------
__fastcall TMemoryAlarmForm::TMemoryAlarmForm(TComponent* Owner)
    : TForm(Owner)
{
    bShow = false;
    ansiFile=new TStringList;
    ansiFile->Clear();
    listPtr=new TList;
    listPtr->Clear();
}
//---------------------------------------------------------------------------
void ShowMemoryAlarm(AnsiString sAlarm)
{
    if(MemoryAlarmForm->bShow==true)
        return;

    MemoryAlarmForm->lbMemoryAlarm->Caption=sAlarm;
    MemoryAlarmForm->ShowModal();
    WriteAlarmLogs(sAlarm);
}
//---------------------------------------------------------------------------
void __fastcall TMemoryAlarmForm::FormShow(TObject *Sender)
{
    bShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TMemoryAlarmForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void WriteAlarmLogs(AnsiString sAlarm)
{
    AnsiString Filename;

    if(DirectoryExists("D:\\HonNewLog")==false)
        ForceDirectories("D:\\HonNewLog");

    Filename.sprintf("D:\\HonNewLog\\%s.csv",Now().FormatString("yyyymmdd"));
    FILE *Fp=fopen(Filename.c_str(), "a+");
    if(Fp!=NULL)
    {
        strcat(sAlarm.c_str(), "\n");
        fputs(sAlarm.c_str(), Fp);
        fclose(Fp);
    }
}
