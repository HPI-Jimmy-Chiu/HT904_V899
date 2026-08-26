#include "MachineDefine.h"                                                      //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "uESDControl.h"
#include "cprod.h"
#include "mymessbox.h"
#include "login.h"
#include "note.h"
#include "common.h"                                                             //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "cMyDB.h"                                                              //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "login.h"                                                              //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "LastSet.h"                                                            //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "cpublic.h"                                                            //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "cmydef.h"                                                             //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "ProductionInfo.h"                                                     //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "FTPClient.h"                                                          //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "cConfiguration.h"                                                     //Sam 20170822 移植超豐 ESD Control 功能 form HT-7045
#include "Password.h"                                                           //Sam 20170912 移植超豐 ESD Control 功能 form HT-7045
#include "TfFTP.h"                                                              //Jimmychiu 20230223 帳密權限加密，提升資訊安全
#include "FileInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TESDForm *ESDForm;
ESD_CONFIG ESDConfig;
//---------------------------------------------------------------------------
__fastcall TESDForm::TESDForm(TComponent* Owner)
    : TForm(Owner)
{
     Width                      = 805;
     Height                     = 605;
     iRow                       = 0;
     iCol                       = 0;
     fDatabaseTime              = 0.0;
     bCheck                     = false;
     iWhichTimeRange            = 0;
     iOldWhichTimeRange         = 0;
     ESDList                    = new TStringList;
     sgESDSetTime->Cells[0][0]  = "Start Time";
     sgESDSetTime->Cells[1][0]  = "End Time";
     LoadESDData();

     sSaveESDControlMsgToFileFilePath = "";                                     //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
     sSaveESDControlMsgToFileFileName = "";
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;
    DateTimePickerESD->DateTime= Now();
}
//==============================================================================
//                                  Save ESD Data
//==============================================================================
void __fastcall TESDForm::SaveESDData()
{
    AnsiString sPath="D:\\HT9045\\config\\ESDconfig.ini", StartTimeItem="", EndTimeItem="";
    TIniFile *INIFile=new TIniFile(sPath);

    MyForceDirectories("D:\\HT9045\\config", "TESDForm::SaveESDData");
    INIFile->WriteString ("ESD", "Forward Time", edESDCheckForwardTime->Text);
    INIFile->WriteInteger("ESD", "Time Count"  , sgESDSetTime->RowCount);

    for(int i=1; i<sgESDSetTime->RowCount; i++)
    {
        StartTimeItem.sprintf("Start Time Item %d", i);
        INIFile->WriteString("ESD", StartTimeItem, sgESDSetTime->Cells[0][i]);
        ESDConfig.aStartTime[i-1]=sgESDSetTime->Cells[0][i];
        EndTimeItem.sprintf("End Time Item %d", i);
        INIFile->WriteString("ESD", EndTimeItem, sgESDSetTime->Cells[1][i]);
        ESDConfig.aEndTime[i-1]=sgESDSetTime->Cells[1][i];
    }
    ESDConfig.iForwardTime  = atoi(edESDCheckForwardTime->Text.c_str());
    ESDConfig.iSetTimeCount = sgESDSetTime->RowCount;
    delete INIFile;
}
//==============================================================================
//                                  Load ESD Data
//==============================================================================
void __fastcall TESDForm::LoadESDData()
{
    AnsiString sPath="D:\\HT9045\\config\\ESDconfig.ini", EndTimeItem="";
    AnsiString StartTimeItem="";
    TIniFile *INIFile = new TIniFile(sPath);

    MyForceDirectories("D:\\HT9045\\config", "TESDForm::LoadESDData()");
    ESDConfig.iForwardTime  = CheckAndReadIniData(sPath,"ESD", "Forward Time"  , 1);
    ESDConfig.iSetTimeCount = CheckAndReadIniData(sPath,"ESD", "Time Count"  , 2);

    edESDCheckForwardTime->Text = ESDConfig.iForwardTime;
    sgESDSetTime->RowCount = ESDConfig.iSetTimeCount;

    for(int i=1; i<ESDConfig.iSetTimeCount; i++)
    {
        StartTimeItem.sprintf("Start Time Item %d", i);
        ESDConfig.aStartTime[i-1] = CheckAndReadIniData(sPath, "ESD", StartTimeItem  ,  AnsiString(""));
        sgESDSetTime->Cells[0][i] = ESDConfig.aStartTime[i-1];

        EndTimeItem.sprintf("End Time Item %d",i);
        ESDConfig.aEndTime[i-1]   = CheckAndReadIniData(sPath, "ESD", EndTimeItem  ,  AnsiString(""));
        sgESDSetTime->Cells[1][i] = ESDConfig.aEndTime[i-1];
    }
    delete INIFile;
}
//==============================================================================
//                         Check Time Range
//==============================================================================
void __fastcall TESDForm::CheckWhichTimeRange()
{
    AnsiString NowTime="";                                                      //Jimmychiu 20220901 修改時間運算方式
    TDateTime DTEnd, DTNow, DTStartNow,DTNowDate;
    static TDateTime DTStartOld;

    WORD y, m, d;
    DecodeDate(Now(),y,m,d);
    DTNowDate=TDateTime(y,m,d);
    NowTime  =Now().FormatString("hh:mm:ss");
    DTNow    =DTNowDate+StrToDateTime(NowTime);
    bCheck   =false;

    for(int i=0; i<ESDConfig.iSetTimeCount-1; i++)
    {
        if(ESDConfig.aStartTime[i]=="" || ESDConfig.aEndTime[i]=="")            //20160615 Mylin Modify ESD Control StartTime Empty Cause Hang Up
        {
            continue;
        }

        DTStartNow = DTNowDate+StrToDateTime(ESDConfig.aStartTime[i]);
        DTEnd      = DTNowDate+StrToDateTime(ESDConfig.aEndTime[i]);
        if(DTNow>=DTStartNow && DTNow<=DTEnd)
        {
            iWhichTimeRange=i+1;
            bCheck = true;
        }
    }

    if(DTStartOld!=DTNowDate)
    {
        DTStartOld = DTNowDate;
        iOldWhichTimeRange = 0;
    }
}
//==============================================================================
//                         Check Time Range
//==============================================================================
bool __fastcall TESDForm::CheckTimeRange(TDateTime DT)
{
    AnsiString NowTime="";                                                      //Jimmychiu 20220901 修改時間運算方式
    double fForward=0.0;
    TDateTime DTStart, DTEnd, DTNow, DTStartNow, DTNowDate;

    WORD y, m, d;
    DecodeDate(Now(), y, m, d);
    fForward =ESDConfig.iForwardTime/24.0;
    DTNowDate=TDateTime(y, m, d);
    NowTime  =Now().FormatString("hh:mm:ss");
    DTNow    =DTNowDate+StrToDateTime(NowTime);
    bCheck   =false;

    for(int i=0; i<ESDConfig.iSetTimeCount-1; i++)
    {
        if(ESDConfig.aStartTime[i]=="" || ESDConfig.aEndTime[i]=="")            //20160615 Mylin Modify ESD Control StartTime Empty Cause Hang Up
        {
            continue;
        }

        DTStartNow  = DTNowDate+StrToDateTime(ESDConfig.aStartTime[i]);
        DTStart     = DTNowDate+StrToDateTime(ESDConfig.aStartTime[i])-fForward;
        DTEnd       = DTNowDate+StrToDateTime(ESDConfig.aEndTime[i]);
        if(DTNow>=DTStartNow && DTNow<=DTEnd)
        {
            bCheck=true;
            if(DT>=DTStart && DT<=DTEnd)
            {
                return true;
            }
        }
    }

    if(bCheck)
        return false;
    else
        return true;
}
//==============================================================================
//                         download File path and Update
//==============================================================================
bool __fastcall TESDForm::UpdateESDDate()
{
    TStringList *slESDDate;
    slESDDate=new(TStringList);
    AnsiString aFilePath;
    char dest[20];
    char str[256];
    ESDList->Clear();
    if(IniConfig.bN15UseESDControlMachine==false)
    {
        delete slESDDate;
        return true;
    }

    if(IniConfig.asN15UseESDControlMachineReadFilePath=="")
    {
        ShowMyMessage("Please Keyin ESD File Path!");
        delete slESDDate;
        return false;
    }

    AnsiString sSetupFilePath="d:\\HT9045\\system\\";                           //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine {
    sSetupFilePath=sSetupFilePath+IniConfig.asN15UseESDControlMachineReadFilePath.SubString(2, IniConfig.asN15UseESDControlMachineReadFilePath.Length()-1);
    FTP_ESDFile_Download(IniConfig.asN15UseESDControlMachineReadFilePath,true); //Sam 20170822 (Steven) 改為 HT9045 方式

    try                                                                         //20160425 Mylin Modify Load File On Net Add Try Catch
    {                                                                           //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine }
        MyForceDirectories(sSetupFilePath);
        DirectoryListBox1->Directory=sSetupFilePath;                            //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine
        FileListBox1->Update();                                                 //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine

        for(int i=0; i<FileListBox1->Items->Count; i++)
        {
            slESDDate->Clear();
            aFilePath=sSetupFilePath+"\\"+FileListBox1->Items->Strings[i];      //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine
            if(FileExists(aFilePath.c_str())==true)                             //Sam 20170822 改為 HT9045 方式
                slESDDate->LoadFromFile( aFilePath );

            for(int ct=0; ct<slESDDate->Count; ct++)
            {
                strcpy(str, slESDDate->Strings[ct].c_str());
                ESDList->Clear();
                for(int j=0; j<8; j++)                                          //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine
                {
                    SplitStrByDotSpaceOnly_ESD(str, dest, 20);
                    ESDList->Add(dest);
                }
                CreateESDInformation(ESDList);
            }
        }
    }
    catch(...)                                                                  //20160425 Mylin Modify Load File On Net Add Try Catch }
    {
        MyDBIProcess("Exception", "TESDForm::UpdateESDDate");
        AnsiString sMsg="";
        sMsg="Load File from Link Error. Please Retry, or Keyin ByPass Pass Word.";
        if(LastSet.iLanguageCountry==1)
        {
            sMsg="載入檔案連線錯誤. 請重試，或輸入共用密碼.";
        }
        ShowMyMessage(sMsg);
    }
    delete slESDDate;
    return true;
}
//==============================================================================
//                              create ESD Database
//==============================================================================
void __fastcall TESDForm::CreateESDTable()
{
    TableESD->Active = false;
    TTable *tb=new TTable(NULL);                                                //Mylin 20160729 Debug Program Memory Increase
    try
    {
        tb->DatabaseName="D:\\HT9045\\system";
        tb->TableType=ttParadox;
        DeleteFile("D:\\HT9045\\system\\ESDData.db");                           //Mylin 20160729 Debug Program Memory Increase TTable *tb=new TTable(NULL);
        tb->TableName="D:\\HT9045\\system\\ESDData";
        tb->FieldDefs->Clear();
        TFieldDef* pNewDef;
        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "ID";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "Date";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "Time";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "W-State";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "L-State";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        pNewDef = tb->FieldDefs->AddFieldDef();
        pNewDef->Name = "R-State";
        pNewDef->DataType = ftString;
        pNewDef->Size=10;

        tb->CreateTable();
        tb->Close();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TESDForm::CreateESDTable");
    }
    delete tb;                                                                  //Mylin 20160729 Debug Program Memory Increase
}
//==============================================================================
//                              Add ESD Data
//==============================================================================
void __fastcall TESDForm::CreateESDInformation(TStringList *slESD)
{
    TableESD->Active = false;
    TableESD->TableName = "D:\\HT9045\\system\\ESDData.db";
    TableESD->Open();
    TableESD->Edit();

    TableESD->Append();
    TableESD->FieldValues["ID"]         = slESD->Strings[0];
    TableESD->FieldValues["Date"]       = slESD->Strings[3];
    TableESD->FieldValues["Time"]       = slESD->Strings[4];
    TableESD->FieldValues["W-State"]    = slESD->Strings[5];
    TableESD->FieldValues["L-State"]    = slESD->Strings[6];
    TableESD->FieldValues["R-State"]    = slESD->Strings[7];
    TableESD->Post();
    TableESD->Close();
    TableESD->Active = true;
}
//==============================================================================
//                                For ESD
//==============================================================================
bool SplitStrByDotSpaceOnly_ESD(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1=0, ct2=0;

    while(1)                                                                    // find first character
    {
        if(str[ct1]=='\x0')
            return false;
        if((str[ct1] !=' ' && str[ct1] !=',' && str[ct1]!='\t' && str[ct1]!='\r'))
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if((ct2+1)>=Max)
            break;

        if(str[ct1]!=' ' && str[ct1]!=',' && str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r')
        {
        }
        else
        {
            break;
        }
    }
    ct2=0;
    while(1)
    {
        Buffer[ct2]=str[ct1];
        if(str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;
        Buffer[ct2]='\x0';
        if(ct2>=10240)
            break;
    }
    strcpy(str,Buffer);
    return true;
}
//==============================================================================
//                                Save data
//==============================================================================
void __fastcall TESDForm::sbUpdateClick(TObject *Sender)
{
    SaveESDData();
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sgESDSetTimeSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    iRow=ARow;
    iCol=ACol;
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sbAddESDTimeSetClick(TObject *Sender)
{
    sgESDSetTime->RowCount=sgESDSetTime->RowCount+1;
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sbDeleteESDTimeClick(TObject *Sender)
{
    for(int i=iRow+1; i<sgESDSetTime->RowCount; ++i)
    {
        sgESDSetTime->Rows[i]->Assign(sgESDSetTime->Rows[i+1]);                 // 以下一個 row 的資料指到目前這 row
    }

    if(sgESDSetTime->RowCount>2)
        sgESDSetTime->RowCount--;
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sbSetTimeClick(TObject *Sender)
{
    AnsiString aTime=TimeToStr(DateTimePickerESD->Time);
    if(iRow==0)
        return;
    sgESDSetTime->Cells[iCol][iRow]=aTime;
}
//---------------------------------------------------------------------------
TDateTime TESDForm::ConversionDateFormat(AnsiString aDate, AnsiString aTime)
{
    AnsiString aY, aM, aD, aS;
    TDateTime TCheckTime;
    char DateSeparatorOld=DateSeparator;                                        //Sam 20201221 : 增加保護
    aY=aDate.SubString(1, 4);
    aM=aDate.SubString(6, 2);
    aD=aDate.SubString(9, 2);
    DateSeparator='/';                                                          //Sam 20201221 : 增加保護
    TCheckTime=TDateTime(atoi(aY.c_str()), atoi(aM.c_str()), atoi(aD.c_str())); //Jimmychiu 20220901 修改時間運算方式
    TCheckTime+=StrToDateTime(aTime);
    DateSeparator=DateSeparatorOld;                                             //Sam 20201221 : 增加保護
    return TCheckTime;
}
//===========================================================================
bool TESDForm::SearchAndCheckESD()
{
    bool bfine=false;
    int iDebug=0;                                                               //Sam 20201229 : Add Debug Mark
    AnsiString aDate, aTime, aID, aWESD, aLESD, aRESD, aMess="";
    AnsiString sFilePath="D:\\HT9045\\system\\";
    if(IniConfig.bN15UseESDControlMachine==false)
        return true;
    try
    {
        CheckWhichTimeRange();
        iDebug=1;
        if(iOldWhichTimeRange==iWhichTimeRange && iWhichTimeRange!=0)
            return true;
        if(bCheck==false)
            return true;

        CreateESDTable();
        iDebug=2;
        if(UpdateESDDate()==false)
            return false;

        sFilePath=sFilePath+IniConfig.asN15UseESDControlMachineReadFilePath.SubString(2, IniConfig.asN15UseESDControlMachineReadFilePath.Length()-1);
        IncludeTrailingPathDelimiter(sFilePath);
        AnsiString sFindFile=sFilePath+"*.*";
        TSearchRec srFile;
        iDebug=3;
        if(FindFirst(sFindFile, 0, srFile)==0)
        {
            do
            {
                AnsiString sSearchFile=srFile.Name;
                AnsiString sDeleteFile=sFilePath+sSearchFile;
                DeleteFile(sDeleteFile);
            }
            while(FindNext(srFile)==0);
        }
        iDebug=4;
        fPassword->edPassword->Visible=false;
        fPassword->Label2->Visible=false;                                       //Sam 20190814 : 顯示修正
        fPassword->edUserName->Text="";                                         //Sam 20171018 : OEE Bug 修正
        fPassword->ShowModal();
        fPassword->edPassword->Visible=true;
        fPassword->Label2->Visible=true;                                        //Sam 20190814 : 顯示修正
        iDebug=5;

        if(fProductionInfo->sLoadMO_AUTOMOTIVE=="Y")                            //KaiChen 20171116 (Steven) ：超豐 人員車規認證 (AUTOMOTIVE)
        {
            AnsiString path="D:\\HT9045\\system\\AUOTMOTIVE.CSV";
            if(CheckAUTOMOTIVEByCSV(fPassword->edUserName->Text, path)!=true)
            {
                aMess.sprintf("%s Not have this person ID, Please Check AUTOMOTIVE !", fPassword->edUserName->Text.UpperCase());
                ShowMyMessage(aMess);
                return false;
            }
            iDebug=6;
        }

        TableESD->Active=false;
        TableESD->TableName="D:\\HT9045\\system\\ESDData.db";
        TableESD->Open();
        TableESD->First();
        iDebug=7;
        if(fPassword->edUserName->Text==fNote->sESDControlMachineGeneralUserID) //Mylin 20170328 (Joye,Hungyeh) Add ESD Control Machine GeneralUserID //Sam 20170822 改為 HT9045 方式
        {
            aMess.sprintf("%s ESD Pass!", fPassword->edUserName->Text.c_str()); //Sam 20170822 改為 HT9045 方式
            SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
            RecordProcess(aMess);
            iOldWhichTimeRange=iWhichTimeRange;
            return true;
        }
        iDebug=8;
        while(!TableESD->Eof)
        {
            aID=TableESD->FieldByName("ID")->AsString;                          //pig 2016.09.19 ModifyDBSearch
            if(aID.UpperCase()==fPassword->edUserName->Text.UpperCase())        //Sam 20170822 改為 HT9045 方式
            {
                aDate = TableESD->FieldByName("Date")->AsString;                //pig 2016.09.19 ModifyDBSearch
                aTime = TableESD->FieldByName("Time")->AsString;                //pig 2016.09.19 ModifyDBSearch
                aWESD = TableESD->FieldByName("W-State")->AsString;             //pig 2016.09.19 ModifyDBSearch
                aLESD = TableESD->FieldByName("L-State")->AsString;             //pig 2016.09.19 ModifyDBSearch
                aRESD = TableESD->FieldByName("R-State")->AsString;             //pig 2016.09.19 ModifyDBSearch
                bfine = true;
            }
            TableESD->Next();
        }
        TableESD->Close();
        iDebug=9;
        edtWESD->Text = aWESD;
        edtLESD->Text = aLESD;
        edtRESD->Text = aRESD;

        aID=fPassword->edUserName->Text.c_str();                                //Sam 20170822 改為 HT9045 方式
        if(bfine==false)
        {
            aMess.sprintf("%s Not have this person ID", aID);
            SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
            ShowMyMessage(aMess);
            return false;
        }
        iDebug=10;
        if(CheckTimeRange(ConversionDateFormat(aDate, aTime)))
        {
            iDebug=11;
            if(bCheck==true && aWESD=="Wok" && aLESD=="Lok" && aRESD=="Rok")
            {
                iDebug=12;
                iOldWhichTimeRange = iWhichTimeRange;
                aMess.sprintf("%s ESD Pass!", aID);
                SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
                RecordProcess(aMess);
                return true;
            }
            else if(bCheck==false)
            {
                iDebug=13;
                aMess.sprintf("%s ESD not need check!", aID);
                SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
                RecordProcess(aMess);
                return true;
            }
            else
            {
                iDebug=14;
                aMess.sprintf("%s Please check personnel ESD unusual!", aID);
                SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
                ShowMyMessage(aMess);
                return false;
            }
        }
        else
        {
            iDebug=15;
            aMess.sprintf("%s This person ID ESD over time!", aID);
            SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
            ShowMyMessage(aMess);
            return false;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TESDForm::SearchAndCheckESD");
        AnsiString sMsg="ESDForm->SearchAndCheckESD() try catch Error!!, Debug:" + IntToStr(iDebug);
        ShowMyMessage(sMsg);
        return false;
    }
}
//===========================================================================
void __fastcall TESDForm::SpeedButton1Click(TObject *Sender)
{
    bool bfine=false;
    AnsiString aDate, aTime, aID, aWESD, aLESD, aRESD, aMess;
    TableESD->Active = false;
    TableESD->TableName = "D:\\HT9045\\system\\ESDData.db";
    TableESD->Open();
    TableESD->First();
    while(!TableESD->Eof)
    {
        aID=TableESD->FieldByName("ID")->AsString;                              //pig 2016.09.19 ModifyDBSearch
        if(aID==fLogin->cbLoginUserName->Text.c_str())                          //Sam 20170822 改為 HT9045 方式
        {
            aDate = TableESD->FieldByName("Date")->AsString;                    //pig 2016.09.19 ModifyDBSearch
            aTime = TableESD->FieldByName("Time")->AsString;                    //pig 2016.09.19 ModifyDBSearch
            aWESD = TableESD->FieldByName("W-State")->AsString;                 //pig 2016.09.19 ModifyDBSearch
            aLESD = TableESD->FieldByName("L-State")->AsString;                 //pig 2016.09.19 ModifyDBSearch
            aRESD = TableESD->FieldByName("R-State")->AsString;                 //pig 2016.09.19 ModifyDBSearch
            bfine = true;
        }
        TableESD->Next();
    }
    TableESD->Close();
    edtWESD->Text = aWESD;
    edtLESD->Text = aLESD;
    edtRESD->Text = aRESD;
    if(bfine==false)
    {
        aMess.sprintf("%s Not have this person ID", aID);
        SaveESDControlMsgToFile(sSaveESDControlMsgToFileFilePath, sSaveESDControlMsgToFileFileName, aMess); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
        ShowMyMessage(aMess);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::sbESDDataImportClick(TObject *Sender)
{
    CreateESDTable();
    UpdateESDDate();
    LoadESDData();
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::FormDestroy(TObject *Sender)
{
    try
    {
        if(ESDList!=NULL)
        {
            delete ESDList;
            ESDList=NULL;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TESDForm::FormDestroy");
    }
    LogSoftwareOffTime("TESDForm, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TESDForm::SetSaveESDControlMsgToFile(AnsiString sFilePath, AnsiString sFileName)
{                                                                               //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
    sSaveESDControlMsgToFileFilePath=sFilePath;
    sSaveESDControlMsgToFileFileName=sFileName;
}
//---------------------------------------------------------------------------
bool TESDForm::SaveESDControlMsgToFile(AnsiString sFilePath, AnsiString sFileName, AnsiString sMsg)
{
    if(IniConfig.bN15UseESDControlMachine==false)
    {
        return false;
    }
    else if(IniConfig.asN15UseESDControlMachineReadFilePath=="")
    {
        ShowMyMessage("Please Keyin ESD File Path!");
        return false;
    }

    if(sFilePath=="" || sFileName=="" || sMsg=="")
    {
        ShowMyMessage("Please Check File Path and Name");
        return false;
    }
    TStringList *slSaveESDMsg;
    slSaveESDMsg=new(TStringList);
    AnsiString sFilePathName="";
    AnsiString sMsgString="";
    TDateTime dtNowDateTime=Now();
    try                                                                         //JimmyChiu 20220120 避免網路不穩定連線失敗造成異常
    {
        slSaveESDMsg->Clear();
        sFilePathName=sFilePath+sFileName;
        MyForceDirectories(sFilePath);
        if(FileExists(sFilePathName.c_str())==true)                             //Sam 20170822 改為 HT9045 方式
        {
            slSaveESDMsg->LoadFromFile(sFilePathName);
        }
        else
        {
            sMsgString="Date Time,ESD Control Message";
            slSaveESDMsg->Add(sMsgString);
        }
        DateSeparator='/';                                                      // Sam 20210225 : 增加保護
        sMsgString=dtNowDateTime.FormatString("YYYY/mm/dd hh:nn:ss");
        sMsgString=sMsgString+","+sMsg;
        slSaveESDMsg->Add(sMsgString);
        slSaveESDMsg->SaveToFile(sFilePathName);

        if(IniConfig.asN15ESDControlMachineSaveRecordFilePath!="" &&            //Mylin 20170412 (Joye,Hungyeh) Add Save ESD Control Machine to Path
           IniConfig.asN15ESDControlMachineSaveRecordFilePath.Trim()!="")
        {
            AnsiString sFullFileName=IniConfig.asN15ESDControlMachineSaveRecordFilePath;
            #ifdef SOFT_SIMULTE
            sFullFileName.sprintf("//handler_data//SETUP_FILE//%s//", AnsiString(IniConfig.SocketHandlerID));
            #endif
            IncludeTrailingPathDelimiter(sFullFileName);
            MyForceDirectories(sFullFileName);
            sFullFileName=sFullFileName+sFileName;
            if(FileExists(sFullFileName.c_str())==true)                         //Sam 20170822 改為 HT9045 方式
            {
                slSaveESDMsg->LoadFromFile(sFullFileName);
            }
            slSaveESDMsg->Add(sMsgString);
            slSaveESDMsg->SaveToFile(sFullFileName);
        }
        delete slSaveESDMsg;
    }
    catch(...)
    {
        delete slSaveESDMsg;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TESDForm::WriteLogs(AnsiString S)                                          //pig 20170105 RecordTryCatchLog end
{
    FILE *pFile;
    AnsiString S1;
    AnsiString sDirPath="D:\\HT9045_log\\DeBugLog";
    char filename[256];

    MyForceDirectories(sDirPath);
    GetTimeInfo();
    sprintf(filename, "%s\\%04d%02d%02d.txt", sDirPath, SystemYear, SystemMonth, SystemDate);

    pFile=fopen(filename, "a+");

    if(pFile!=NULL)
    {
        fputs(S.c_str(), pFile);
        fclose(pFile);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TESDForm::FTP_ESDFile_Download(AnsiString sFTPPath, bool IsAUTOMOTIVE)
{
    bool bReturn=false;
    if(SystemStart==true || bESDFTPAction==true)
        return bReturn;
    bESDFTPAction=true;
    RecordProcess("Enter ESDFile Download");
    AnsiString sSetupFilePath=sSetupFilePath+IniConfig.asN15UseESDControlMachineReadFilePath.SubString(2, IniConfig.asN15UseESDControlMachineReadFilePath.Length()-1);

    AnsiString asUserID="", asPassword="", asHost="";
    fProductionInfo->GetFTP_Setting(asUserID, asPassword, asHost);              //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID, asPassword, asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fProductionInfo->CheckFTPFilePath(sFTPPath);                            //JimmyChiu 20220121 check FTP file path
        sSetupFilePath="D:\\HT9045\\system\\"+sSetupFilePath;
        if(fFTP.DownloadFilterFile(sFTPPath, sSetupFilePath, AnsiString(".txt"), asError))
            bReturn=true;
        else
            ShowMyMessage("ESD Control FTP Server is not connected Path:"+sFTPPath);

        if(IsAUTOMOTIVE)                                                        //KaiChen 20171116 (Steven) ：超豐 人員車規認證 (AUTOMOTIVE)
        {
            sFTPPath=IniConfig.asN15HandlerAUTOMOTIVEDownloadPath;
            fProductionInfo->CheckFTPFilePath(sFTPPath);                        //Jimmychiu 20220901 檢查檔案是否存在
            sSetupFilePath="D:\\HT9045\\system\\";
            if(fFTP.DownloadFilterFile(sFTPPath,sSetupFilePath,AnsiString(".csv"),asError))
                bReturn=true;
        }
    }
    else
    {
        MyDBIProcess("Exception", "ESD Control FTP Server is not connected Path:"+sFTPPath);
        ShowMyMessage("ESD Control FTP Server is not connected Path:"+sFTPPath);
    }
    bESDFTPAction=false;
    return bReturn;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TESDForm::FTP_PasswordFile_Download(AnsiString sFTPPath)
{
    AnsiString sReturnFileName="";
    if(SystemStart==true || bESDFTPAction==true)
        return sReturnFileName;
    AnsiString sFileName="LevelLogin.txt";
    if(IniConfig.bN14_23_ReadTextFileforPassword==false)                        //JimmyChiu 20240115 : Read Text File for Password
    {
        sFileName="LevelLogin.bin";
    }
    bESDFTPAction=true;
    RecordProcess("Enter Password Download");
    AnsiString sSetupFilePath="";

    AnsiString asUserID="",asPassword="",asHost="";
    fProductionInfo->GetFTP_Setting(asUserID, asPassword, asHost);              //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID, asPassword, asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fProductionInfo->CheckFTPFilePath(sFTPPath);                            //JimmyChiu 20220121 check FTP file path
        sSetupFilePath=sFTPPath;
        sSetupFilePath=StringReplace(sSetupFilePath, "\\", "", TReplaceFlags()<<rfReplaceAll);
        sSetupFilePath=StringReplace(sSetupFilePath, "//", "", TReplaceFlags()<<rfReplaceAll);
        sSetupFilePath="D:\\HT9045\\system\\"+sSetupFilePath+"\\";
        sReturnFileName=fFTP.DownloadFilterFile_Get1stFileName(sFTPPath,sSetupFilePath,sFileName,asError);    //Jimmychiu 20230223 帳密權限加密，提升資訊安全
        if(sReturnFileName!="")
        {
            sSetupFilePath+=sReturnFileName;
            if(IniConfig.bN14_23_ReadTextFileforPassword==false)                //JimmyChiu 20240115 : Read Text File for Password
            {
                sSetupFilePath=ExangeBin2Txt(sSetupFilePath);                   //Jimmychiu 20230223 帳密權限加密，提升資訊安全
            }
            sReturnFileName=sSetupFilePath;
        }
    }
    else
    {
        MyDBIProcess("Exception", "Password Control FTP Server is not connected Path:"+sFTPPath);
        ShowMyMessage("Password Control FTP Server is not connected Path:"+sFTPPath);
    }
    bESDFTPAction=false;
    return sReturnFileName;
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::FTPList(AnsiString Listing)                           //20160505 Mylin Use FTP To do User Level By txt And ESD Control Machine
{
    fConfiguration->cbESDFTPList->Items->Add(Listing);
}
//---------------------------------------------------------------------------
bool __fastcall TESDForm::CheckAUTOMOTIVEByCSV(AnsiString asCheckUserName, AnsiString asCsvFilePath)
{
    int index=-1;
    bool bflag=false;
    TStringList *slList=new TStringList();
    AnsiString ff="";
    fProductionInfo->LoadCSVFileToStringGrid(asCsvFilePath, sg_List_AUTOMOTIVE, 8);
    slList->Clear();
    for(int j=0; j<=(sg_List_AUTOMOTIVE->RowCount-1); j++)
    {
        slList->Add(sg_List_AUTOMOTIVE->Cells[1][j]);
        ff=sg_List_AUTOMOTIVE->Cells[1][j];
    }
    index=slList->IndexOf(asCheckUserName);
    if(index>=0)
    {
        if(sg_List_AUTOMOTIVE->Cells[6][index]=="Y")
        {
            bflag=true;
        }
    }
    slList->Clear();
    delete slList;
    return bflag;
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::AUOTMOTIVEFTPList(AnsiString Listing)                 //KaiChen 20171116 (Steven) ：超豐 人員車規認證 (AUTOMOTIVE)
{
    fConfiguration->cbOEEAUTOMOACTIVEDownloadList->Items->Add(Listing);
}
//---------------------------------------------------------------------------
void __fastcall TESDForm::edESDCheckForwardTimeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10000, 1);
}
//---------------------------------------------------------------------------
AnsiString TESDForm::ExangeBin2Txt(AnsiString asSource)                         //Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    int idotPos=asSource.LastDelimiter('.');
    AnsiString asReturn="";
    if(idotPos>0)
    {
        AnsiString asFile="";
        AnsiString asExtension="";
        FileInfo moFileInfo(asSource);
        moFileInfo.GetNameAndExtension(asSource, asFile, asExtension);
        AnsiString asContent=moFileInfo.DecodeASCII(asSource,-100);
        asReturn=asFile+".com";
        moFileInfo.SaveAsTxtFile(asContent, asReturn);
    }
    return asReturn;
}
//---------------------------------------------------------------------------
AnsiString TESDForm::ExangeTxt2Bin(AnsiString asSource)                         //Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    int idotPos=asSource.LastDelimiter('.');
    AnsiString asReturn="";
    if(idotPos>0)
    {
        AnsiString asFile="";
        AnsiString asExtension="";
        FileInfo moFileInfo(asSource);
        moFileInfo.GetNameAndExtension(asSource, asFile, asExtension);
        AnsiString asContent=moFileInfo.DecodeASCII(asSource,100);
        asReturn=asFile+".bin";
        moFileInfo.SaveAsTxtFile(asContent, asReturn);
    }
    return asReturn;
}
//---------------------------------------------------------------------------

