#include "MachineDefine.h"
#pragma hdrstop

#include "PMAlarmShowData.h"
#include "PMAlarmInterFace.h"
#include "ExternFunction.h"
#include "FileInfo.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfPMAlarm_ShowData *fPMAlarm_ShowData;
//---------------------------------------------------------------------------
__fastcall TfPMAlarm_ShowData::TfPMAlarm_ShowData(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfPMAlarm_ShowData::LoadFileFromCSV(AnsiString sFullFileName)
{
    for(int ix=0;ix<sg_PMAlarmShowData->ColCount;ix++)                          //Jimmychiu 20240226 : add initial stinggrid
    {
        for(int iy=0;iy<sg_PMAlarmShowData->RowCount;iy++)
        {
            sg_PMAlarmShowData->Cells[ix][iy] = "";
        }
    }
    TStringList *slPMAlarm_CSVData;
//    bool bFileExists = false;
    AnsiString sTemp = "";

    TDateTime dtNowDate = Now();
    Word wNowYear,wNowMonth,wNowDate;
    DecodeDate( dtNowDate , wNowYear , wNowMonth , wNowDate );

    slPMAlarm_CSVData = new TStringList;
    slPMAlarm_CSVData->Clear();

    if(FileExists(sFullFileName) == true)
    {
        slPMAlarm_CSVData->LoadFromFile(sFullFileName);
//        bFileExists = true;
    }
    else
    {
        slPMAlarm_CSVData->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete slPMAlarm_CSVData;
        ShowMyMessage("File is not Exist!!");
        return;
    }

    for(int iy=0;iy<sg_PMAlarmShowData->RowCount;iy++)
    {
        if(iy >= slPMAlarm_CSVData->Count)
        {
            break;
        }
        sTemp = slPMAlarm_CSVData->Strings[iy];
        for(int ix=0;ix<sg_PMAlarmShowData->ColCount;ix++)
        {
            sg_PMAlarmShowData->Cells[ix][iy] = fPMAlarmInterFace->GetCsvData(ix + 1,sTemp); //取得CSV格式中的第幾筆資料;
        }
    }
    slPMAlarm_CSVData->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slPMAlarm_CSVData;
}
//---------------------------------------------------------------------------
void TfPMAlarm_ShowData::RefreshFileList()
{
    cb_PMAlarmChoiseFile->Items->Clear();
    int iErr=0;
    std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(fPMAlarmInterFace->PMAlarm_SYS.GetSaveFilePath(),iErr);   //"D:\\PM RECORD\\";
    if(iErr==1)
    {
        ShowMyMessage("Folder path error \n\r"+fPMAlarmInterFace->PMAlarm_SYS.GetSaveFilePath());
    }
    else
    {
        AnsiString sFileName="";
        for(unsigned int i=0;i<fileNames.size();i++)
        {
            sFileName=fileNames[i];
            sFileName = sFileName.SubString(1,sFileName.Length() - 4);
            cb_PMAlarmChoiseFile->Items->Add(sFileName);
        }
        cb_PMAlarmChoiseFile->ItemIndex = 0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarm_ShowData::cb_PMAlarmChoiseFileChange(
      TObject *Sender)
{
    AnsiString sFullFileName = "";
    sFullFileName = cb_PMAlarmChoiseFile->Text + ".csv";
    //20150707 sFullFileName = "D:\\PM RECORD\\" + sFullFileName;
    sFullFileName = sDataFilePath + sFullFileName; //20150707
    LoadFileFromCSV(sFullFileName);
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarm_ShowData::FormShow(TObject *Sender)
{
    RefreshFileList();
}
//---------------------------------------------------------------------------
