#include "MachineDefine.h"
#pragma hdrstop

#include "cDataHandling.h"
#include "common.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfDataHandling *fDataHandling;
//---------------------------------------------------------------------------
__fastcall TfDataHandling::TfDataHandling(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfDataHandling::InitCompPara()
{
    TSearchRec SearchRec;
    AnsiString SDataPath=DataPath;
    int iAttr=faAnyFile;//檔案
    ComboBox1->Clear();
    CheckListBox1->Clear();
    if(!DirectoryExists(SDataPath))
    {
        ShowMyMessage("File Path Lost");
        return;
    }
    else if(DirectoryExists(SDataPath))
    {
        if(SDataPath.SubString(SDataPath.Length(), 1)!="\\")
        {
            SDataPath=SDataPath+"\\*.*";
        }

        if(FindFirst(SDataPath, iAttr, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttr) == SearchRec.Attr)
                {
                     if(SearchRec.Name!="." && SearchRec.Name!="..")
                     {
                        ComboBox1->Items->Add(SearchRec.Name);
                        CheckListBox1->Items->Add(SearchRec.Name);
                     }
                }
            }
            while (FindNext(SearchRec) == 0);
                FindClose(SearchRec);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDataHandling::FormShow(TObject *Sender)
{
    InitCompPara();
}
//---------------------------------------------------------------------------
void __fastcall TfDataHandling::ComboBox1Change(TObject *Sender)
{
    TSearchRec SearchRec;
    AnsiString SDataPath=DataPath;
    int iAttr=faAnyFile;//檔案
    CheckListBox1->Clear();
    if(!DirectoryExists(SDataPath))
    {
        ShowMyMessage("File Path Lost");
        return;
    }
    else if(DirectoryExists(SDataPath))
    {
        if(SDataPath.SubString(SDataPath.Length(), 1)!="\\")
        {
            SDataPath=SDataPath+"\\*.*";
        }

        if(FindFirst(SDataPath, iAttr, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttr) == SearchRec.Attr)
                {
                     if(SearchRec.Name!="." && SearchRec.Name!="..")
                     {
                        if(ComboBox1->Text!=SearchRec.Name)
                            CheckListBox1->Items->Add(SearchRec.Name);
                     }
                }
            }
            while (FindNext(SearchRec) == 0);
                FindClose(SearchRec);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDataHandling::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfDataHandling::spbExeClick(TObject *Sender)
{
    TCheckBox *ChkPtr[]={CheckBox1,CheckBox2,CheckBox3};
    AnsiString FileName[3]={"Tray.Data","HotPlate.Data","Temperature.Data"};
    AnsiString SourPath,TargetPath,sCheck;
    char str[128]="";
    int i, j,ret;
    for(i=0; i<CheckListBox1->Items->Count; i++)
    {
        if(CheckListBox1->Checked[i])
        {
            for(j=0; j<3; j++)
            {
                if(ChkPtr[j]->Checked)
                {
                    sprintf(str,"\"%s%s\\%s\"",DataPath,ComboBox1->Text,FileName[j]);
                    SourPath=str;
                    str[0]=-1;
                    sprintf(str,"\"%s%s\\%s\"",DataPath,CheckListBox1->Items->Strings[i],FileName[j]);
                    TargetPath=str;
                    //jou 980331 add MessageDlg
                    sprintf(str, "Sure to cover Old %s File? (確定要覆蓋舊檔案？)", FileName[j]);
                    sCheck=str;
                    ret=MessageDlg(sCheck, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
                    if(ret==mrNo)
                        continue;
                    CopyFile(SourPath.c_str(), TargetPath.c_str(),false);
                }
            }
        }
    }
    spbExe->Down=false;
}
//---------------------------------------------------------------------------
