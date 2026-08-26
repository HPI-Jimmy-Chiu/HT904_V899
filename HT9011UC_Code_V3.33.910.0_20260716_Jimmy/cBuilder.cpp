#include "MachineDefine.h"
#pragma hdrstop

#include "cBuilder.h"
#include "common.h"
#include "main.h"
#include "mymessbox.h"
#include "cprod.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
#include "database.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfBuilder *fBuilder;
//------------------------------------------------------------------------------
__fastcall TfBuilder::TfBuilder(TComponent* Owner)
    : TForm(Owner)
{
    asBackupCreate="";
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::FormShow(TObject *Sender)
{
    InitCompData();
    ShowDirBoxPath();
    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103
    asBackupCreate="";

    btCreateSetupFile->Top=88;
    fShow=true;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::cbSourceFileChange(TObject *Sender)
{
    if(cbSourceFile->Text!="")
        edNewFileName->Enabled=true;
    else
        edNewFileName->Enabled=false;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::edNewFileNameChange(TObject *Sender)
{
    if(edNewFileName->Text!="")
        btCreateSetupFile->Enabled=true;
    else
        btCreateSetupFile->Enabled=false;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::Create2DCodeWorkFile()                               //wei 20160802 add 2DCodeCreateWorkFile
{
    AnsiString SPath="D:\\HT9045\\Barcode_File\\";
    AnsiString OrgPath=cbSourceFile->Text+AnsiString(".cfg");
    AnsiString NewPath=edNewFileName->Text+AnsiString(".cfg");
    AnsiString asStr;
    AnsiString str1, str2;

    for(int i=0; i<4; i++)
    {
        asStr=FileInfo().PathCombin(SPath, HSys.asBarCodeComPort[i]);
        str2=FileInfo().PathCombin(asStr, OrgPath);
        if(FileExists(str2))
        {
            str1=FileInfo().PathCombin(asStr, NewPath);
            CopyFile(str2.c_str(), str1.c_str(), false);
            MySleep(50);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::btCreateSetupFileClick(TObject *Sender)
{
    if(cbSourceFile->Text=="")
    {
        ShowMyMessage("Must select Source Setup data","必須選擇來源檔");
        return;
    }

    if(edNewFileName->Text=="")
    {
        ShowMyMessage("Must set setup name","必須設定setup名字");
        return;
    }

    AnsiString str;
    char cStr1[256]="", cStr2[256]="";                                          //20111026 jou
    AnsiString OrgPath="", NewPath="", sString, JOBFILEPath="";
    AnsiString SPath[2]={DataPath, OffsetPath};

    AnsiString SBuffer=edNewFileName->Text;
    SBuffer=SBuffer.UpperCase();

    str.sprintf("Do you create ''%s'' as the name", edNewFileName->Text);
    if(Application->MessageBox(str.c_str(), "Builder", MB_OKCANCEL|MB_TOPMOST)==IDOK)
    {
        if(CUSTOMER_CODE==CC_KYEC_XILINX && BAR_CODE_INSTALL==ebctInShtIntel)   //wei 20160802 add 2DCodeCreateWorkFile
        {
            Create2DCodeWorkFile();
        }

        for(int i=0; i<2; i++)
        {
            OrgPath=FileInfo().PathCombin(SPath[i], cbSourceFile->Text);
            NewPath=FileInfo().PathCombin(SPath[i], edNewFileName->Text);
            MyForceDirectories(NewPath);

            //------------------20111026    jou------------------
            OrgPath+="\\*.*";
            ZeroMemory(&cStr1, sizeof(cStr1));
            ZeroMemory(&cStr2, sizeof(cStr2));
            strncpy(cStr1, OrgPath.c_str(), sizeof(cStr1));
            strncpy(cStr2, NewPath.c_str(), sizeof(cStr2));
            //複製工作檔
            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;
            oFile.wFunc=FO_COPY;
            oFile.pFrom=cStr1;                                                  //The list of names must be double null-terminated.
            oFile.pTo=cStr2;
            oFile.hNameMappings=NULL;
            oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI);
            SHFileOperation(&oFile);
            #ifdef ASE_KaohSiung                                                //kevin 20150105 start
            JOBFILEPath=FileInfo().PathCombin(SPath[i], "JOBFILE");
            if(JOBFILEPath.Pos("JOBFILE"))                                      //kevin 20150120
                continue;

            MyForceDirectories(JOBFILEPath);
            ZeroMemory(&cStr1, sizeof(cStr1));
            ZeroMemory(&cStr2, sizeof(cStr2));
            strncpy(cStr1, OrgPath.c_str(), sizeof(cStr1));
            strncpy(cStr2, JOBFILEPath.c_str(), sizeof(cStr2));

            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;                                                  //複製工作檔
            oFile.wFunc=FO_COPY;
            oFile.pFrom=cStr1;
            oFile.pTo=cStr2;
            oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI);
            SHFileOperation(&oFile);
            #endif                                                              //kevin 20150105 end

            //---------------------------------------------------

            if(i==2)                                                            //Steven 20101118 : 強制要加加
                i++;
        }
        cbSourceFile->Items->Add(edNewFileName->Text);
        cbDeleteFile->Items->Add(edNewFileName->Text);
        CheckListBox1->Items->Add(edNewFileName->Text);
        asBackupCreate=edNewFileName->Text;
        edNewFileName->Text="";

        sString=fMain->cbSetupFileName->Text;
        fMain->cbSetupFileName->Clear();
        fMain->LookForFile();
        fMain->cbSetupFileName->Text=sString;
    }
}
//------------------------------------------------------------------------------
// KEVIN 20150107 Save As JobFile
//------------------------------------------------------------------------------
void __fastcall TfBuilder::bSaveAsJobFile(AnsiString SourceFileFileName, AnsiString FileName)
{
    AnsiString OrgPath="", NewPath="", sString, JOBFILEPath="";
    AnsiString SPath[2]={DataPath, OffsetPath}, sBuffer=SourceFileFileName.UpperCase();

    if(sBuffer.Pos(FileName))                                                   //檔案名稱比較
        return;

    for(int i=0; i<2; i++)
    {
        OrgPath=FileInfo().PathCombin(SPath[i], SourceFileFileName)+"\\*.*";
        #ifdef ASE_KaohSiung                                                    //kevin 20150105 start
        JOBFILEPath=FileInfo().PathCombin(SPath[i], FileName);
        MyForceDirectories(JOBFILEPath);
        ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
        oFile.hwnd=Handle;
        oFile.wFunc=FO_COPY;
        oFile.pFrom=OrgPath.c_str();
        oFile.pTo=JOBFILEPath.c_str();
        oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI);
        SHFileOperation(&oFile);
        #endif                                                                  //kevin 20150105 end

        //---------------------------------------------------
        if(i==2)                                                                //Steven 20101118 : 強制要加加
            i++;
    }
    CheckListBox1->Items->Add(FileName);
    asBackupCreate=FileName;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::cbDeleteFileChange(TObject *Sender)
{
    if(cbDeleteFile->Text!="")
        btDeleteSetupFile->Enabled=true;
    else
        btDeleteSetupFile->Enabled=false;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::btDeleteSetupFileClick(TObject *Sender)
{
    AnsiString str, sString;
    if(cbDeleteFile->Text=="")
    {
        ShowMyMessage("Must select delete Setup data","必須選擇刪除檔");
        return;
    }

    if(cbDeleteFile->Text==fMain->cbSetupFileName->Text)                        //2012-01-13    Dell    工作檔del後,程式無法開啟
    {
        ShowMyMessage("File in use, cannot delete", "工作檔正在使用用中");
        return;
    }

    str.sprintf("Do you delete ''%s'' as the name", cbDeleteFile->Text);
    if(Application->MessageBox(str.c_str(), "Builder", MB_OKCANCEL|MB_TOPMOST)==IDOK)
    {
        DeleteSetupFile(cbDeleteFile->Text);
    }

    sString=fMain->cbSetupFileName->Text;
    fMain->cbSetupFileName->Clear();
    fMain->LookForFile();
    fMain->cbSetupFileName->Text=sString;

    InitCompData();
    cbDeleteFile->Text="";
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::DeleteSetupFile(AnsiString DeleteFileName)           //Steven 20110305
{
    char str1[256]="";
    AnsiString OrgPath="";
    AnsiString SPath[2]={DataPath, OffsetPath};

    for(int i=0; i<2; i++)
    {
        OrgPath=FileInfo().PathCombin(SPath[i], DeleteFileName);
        if(DirectoryExists(OrgPath))
        {
            if(IsFileInUse(OrgPath.c_str())==true)                              //Jimmychiu 20241121 : Verify whether the file is currently in use
            {
                MySleep(5000);
            }
            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;
            oFile.wFunc=FO_DELETE;
            strncpy(str1, OrgPath.c_str(), sizeof(str1));
            oFile.pFrom=str1;
            oFile.fFlags=FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
            bool bDelOK=SHFileOperation(&oFile);
            if(bDelOK==true)
            {
                ShowMyMessage("Delete Setup File ERROR");
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::DirectoryListBox1Change(TObject *Sender)
{
    ShowDirBoxPath();
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::ShowDirBoxPath()
{
    AnsiString str;
    str.sprintf("Path:%s", DirectoryListBox1->Directory);
    labDir->Caption=str;
}
//-----------------------------------------------------------------------------
void __fastcall TfBuilder::CopySourTarget(AnsiString Sour)
{
    char Orgstr[128]="";
    TSearchRec SearchRec;
    AnsiString SDataPath=Sour;
    int iAttr=faDirectory;                                                      //檔案
    if(!DirectoryExists(SDataPath))
    {
        ShowMyMessage("File Path Lost");
        return;
    }
    else if(DirectoryExists(SDataPath))
    {
        if(CosFunction.bBuilderImportSingleFolder)                              //ChungHung 20150413 add for TSMC //ChungHung 20150415 add for TSMC
        {
            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;
            oFile.wFunc=FO_COPY;
            strncpy(Orgstr, SDataPath.c_str(), sizeof(Orgstr));
            oFile.pFrom=Orgstr;
            oFile.pTo=DataPath.c_str();
            oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION);
            SHFileOperation(&oFile);
        }
        else
        {
            if(SDataPath.SubString(SDataPath.Length(), 1)!="\\")
            {
                SDataPath=SDataPath+"\\*.*";
            }
            else
            {
                SDataPath=SDataPath+"*.*";                                      //Steven 20110621
            }

            if(FindFirst(SDataPath, iAttr, SearchRec)==0)
            {
                do
                {
                    if((SearchRec.Attr&iAttr)==faDirectory)
                    {
                        if(SearchRec.Name!="." && SearchRec.Name!="..")
                        {
                            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
                            oFile.hwnd=Handle;
                            oFile.wFunc=FO_COPY;
                            strncpy(Orgstr, SDataPath.c_str(), sizeof(Orgstr));
                            oFile.pFrom=Orgstr;
                            oFile.pTo=DataPath.c_str();
                            oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION);
                            SHFileOperation(&oFile);
                        }
                    }
                }
                while(FindNext(SearchRec)==0);
                FindClose(SearchRec);
            }
        }
    }

    InitCompData();
}
//------------------------------------------------------------------------------
bool __fastcall TfBuilder::NeedExport(AnsiString sName)
{
    for(int i=0; i<CheckListBox1->Items->Count; i++)
    {
        if(CheckListBox1->Checked[i])
        {
            if(CheckListBox1->Items->Strings[i]==sName)
                return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::InitCompData()
{
    WIN32_FIND_DATA filedata;                                                   // Structure for file data
    HANDLE filehandle;                                                          // Handle for searching

    AnsiString szFile="", szDir="";
    AnsiString SDataPath=DataPath;

    cbSourceFile->Clear();
    cbDeleteFile->Clear();
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

        filehandle=FindFirstFile((SDataPath+"*").c_str(), &filedata);
        if(filehandle!=INVALID_HANDLE_VALUE)                                    //Steven 20101118 Start : 不要顯示資料夾以外的檔案
        {
            do
            {
                if((filedata.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata.cFileName, ".")==0 ||                       /* 不處理隱藏檔及 . 跟 .. */
                    strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)     //如果該檔案為資料夾
                {
                    cbSourceFile->Items->Add(filedata.cFileName);
                    cbDeleteFile->Items->Add(filedata.cFileName);
                    CheckListBox1->Items->Add(filedata.cFileName);
                }
            } while(FindNextFile(filehandle, &filedata));
            FindClose(filehandle);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::spbExportClick(TObject *Sender)
{
    spbExport->Down=false;
    AnsiString str="";
    TSearchRec SearchRec;
    AnsiString SDataPath=DataPath;
    AnsiString OrgPath="", NewPath="";
    int iAttr=faDirectory;
    char cStr1[256]="", cStr2[256]="";

    str.sprintf("Make sure export data to ''%s''", DirectoryListBox1->Directory);
    if(Application->MessageBox(str.c_str(), "Export", MB_OKCANCEL|MB_TOPMOST)!=IDOK)
        return;
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
        else
        {
            SDataPath=SDataPath+"*.*";                                          //Steven 20110621
        }

        if(FindFirst(SDataPath, iAttr, SearchRec)==0)
        {
            do
            {
                if((SearchRec.Attr&iAttr)==faDirectory)
                {
                    if(NeedExport(SearchRec.Name))
                    {
                        NewPath=DirectoryListBox1->Directory;
                        OrgPath=DataPath;
                        OrgPath+=SearchRec.Name;
                        NewPath+=AnsiString ("\\");
                        NewPath+=SearchRec.Name;

                        if(DirectoryExists(NewPath))                            //jou 2012-01-13 修正Export不能使用
                        {
                            memset(cStr1, 0, sizeof(cStr1));
                            strncpy(cStr1, NewPath.c_str(), sizeof(cStr1));
                            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
                            oFile.hwnd=Handle;
                            oFile.wFunc=FO_DELETE;
                            oFile.pFrom=cStr1;
                            oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR);
                            SHFileOperation(&oFile);
                        }

                        memset(cStr1, 0, sizeof(cStr1));
                        memset(cStr2, 0, sizeof(cStr2));
                        strncpy(cStr1, OrgPath.c_str(), sizeof(cStr1));
                        strncpy(cStr2, NewPath.c_str(), sizeof(cStr2));

                        ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
                        oFile.hwnd=Handle;                                      //複製工作檔
                        oFile.wFunc=FO_COPY;
                        oFile.pFrom=cStr1;
                        oFile.pTo=cStr2;
                        oFile.fFlags=(FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR);
                        SHFileOperation(&oFile);
                    }
                }
            }
            while(FindNext(SearchRec)==0);
            FindClose(SearchRec);
        }
    }
    DirectoryListBox1->Update();

    Application->MessageBox("Export data finish!!", "Export", MB_OK|MB_TOPMOST);
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::spbImportClick(TObject *Sender)
{
    spbImport->Down=false;
    AnsiString str="";
    str.sprintf("Make sure Import data form ''%s''", DirectoryListBox1->Directory);
    if(Application->MessageBox(str.c_str(), "Import", MB_OKCANCEL)==IDOK)
        CopySourTarget(DirectoryListBox1->Directory);
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::spbExitClick(TObject *Sender)
{
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::edNewFileNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::edNewFileNameKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyMakeFileDataInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfBuilder::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//------------------------------------------------------------------------------
// KEVIN 20180824 add  Save File
//------------------------------------------------------------------------------
void __fastcall TfBuilder::bSaveAllFillOrFile(AnsiString SourceFileFileName, int iFile)
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir=GetRecipePath();

    fHotPlate->DoIniDataToForm();
    fContact->DoIniDataToForm();
    fContact->DutCount();                                                       //jou 2014-09-06 修正開啟程式的時候EP異常
    FTestIF->DoIniDataToForm();
    fYieldMonitoring->DoIniDataToForm();
    fSetup->DoIniDataToForm();
    fTemp_Set->DoIniDataToForm(true);                                           //Steven 20110930 : 得在fSetup後面
    fSpeed->DoIniDataToForm();
    fTrayAssignment->DoIniDataToForm();                                         //wei 20150317 : 得在fSpeed後面

    if(iFile==0)
    {
       if(SourceFileFileName.Pos("ArmCondition"))
          fSpeed->SaveSetupFile(szDir);                                         //USPEED
       else if(SourceFileFileName.Pos("Binasgn"))
          fBinSel->SaveOther(szDir);                                            //cBinSel
       if(SourceFileFileName.Pos("Contact"))
          fContact->SaveSetupFile(szDir, S);                                    //Contract
       if(SourceFileFileName.Pos("HandlerCondition"))                           //SETUP
          fSetup->SaveSetupFile(szDir);
       if(SourceFileFileName.Pos("HotPlate"))                                   //hotplate
          fHotPlate->SaveSetupFile(szDir, S);
       if(SourceFileFileName.Pos("Temperature"))
           fTemp_Set->SaveSetupFile(szDir, S);                                  //TEMP
       if(SourceFileFileName.Pos("Tester"))
       {
          FTestIF->SaveSetupFile(szDir, S);
          fYieldMonitoring->SaveSetupFile(szDir, S);
       }

       if(SourceFileFileName.Pos("TestMode"))
          SaveTestMode();                                                       //USPEED
       if(SourceFileFileName.Pos("Tray"))
       {
          fTrayAssignment->SaveSetupFile(szDir, S);
          fTrayForm->SaveSetupFile(szDir, S);
       }

       if(SourceFileFileName.Pos("UdUld"))
          fLd_ULd->SaveSetupFile(szDir, S);
    }
    else
    {                                                                           //all fil save
         fSpeed->SaveSetupFile(szDir);
         fBinSel->SaveOther(szDir);
         fContact->SaveSetupFile(szDir, S);
         fSetup->SaveSetupFile(szDir);
         fHotPlate->SaveSetupFile(szDir, S);
         fTemp_Set->SaveSetupFile(szDir, S);
         FTestIF->SaveSetupFile(szDir, S);
         fYieldMonitoring->SaveSetupFile(szDir, S);
         fTrayForm->SaveSetupFile(szDir, S);
         fTrayAssignment->SaveSetupFile(szDir, S);
         fLd_ULd->SaveSetupFile(szDir, S);

         if(CosFunction.bCCLinkValueSaveFile)                                   //kevin 20180827 add
            fCCLink->SaveSetupFile();
    }
}
//------------------------------------------------------------------------------
