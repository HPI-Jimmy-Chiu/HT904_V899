#include "MachineDefine.H"
#pragma hdrstop

#include "ARMS.h"
#include "cprod.h"
#include "mymessbox.h"
#include "ExternFunction.h"
#include "main.h"
//#include "maintenance.h"
#include "csystem.h"
#include "INPUT.h"
#include "ARMSLog.h"
#include "cmydef.h"
#include "common.h"
#include "cpublic.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfARMS *fARMS;
AnsiString aHandlerDataPath="D:\\HT9045\\IniData\\Data\\";
AnsiString aHandlerSystemPath="D:\\HT9045\\system\\";
int iDownloadTag=1;
int iUploadTag=2;
int iBackupTag=3;
//---------------------------------------------------------------------------
__fastcall TfARMS::TfARMS(TComponent* Owner)
    : TForm(Owner)
{
    Height = 600;
    Width  = 800;
    fShow  = false;
    _FILE_PTAH = "D:\\HT9045\\System\\ARMS.ini";
}
//==============================================================================
void __fastcall TfARMS::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfARMS::FormDestroy");
    }
    LogSoftwareOffTime("TfARMS, FormDestroy");                                  //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
bool __fastcall TfARMS::CheckPassword(AnsiString aPath)
{
    bool bFlag=false;
    AnsiString aPwd;
    TIniFile* IniFile=new TIniFile(_FILE_PTAH);
    TEdit *edPtr=new TEdit(this);

    if(fInput->fShow==false)
    {
        aPwd=IniFile->ReadString("Password", "HonPrec", "");

        if(aPwd=="")
        {
            aPwd="16943420";
        }

        MyPassWordInputBox(edPtr);
        if(edPtr->Text!=aPwd)
        {
            ShowMyMessage("ARMS Password Error");
        }
        else
        {
            bFlag=true;
        }
    }
    delete IniFile;
    delete edPtr;                                                               //Steven 20200506 : 用完要delete
    return bFlag;
}
//------------------------------------------------------------------------------
bool __fastcall TfARMS::CheckListBoxIsSelete(TCheckListBox *clb)
{
    int iFind=0;
    for(int i=0; i<clb->Items->Count; i++)
    {
        if(clb->Checked[i])
        {
            iFind++;
        }
    }

    if(iFind==0)
    {
        ShowMyMessage("Please Selet Recipe Name");
        return false;
    }

    if(iFind>1)
    {
        ShowMyMessage("Please Selet one Recipe Name only");
        return false;
    }
    return true;
}
//==============================================================================
//                              設定參數到元件
//==============================================================================
void TfARMS::SetParameterToShow(TObject *PCtrl,int iValue)
{
    TComboBox *PComboBox     = dynamic_cast <TComboBox *>(PCtrl);
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PComboBox!=NULL)
    {
        PComboBox->ItemIndex=iValue;
    }
    else if(PEdit!=NULL)
    {
        PEdit->Text=iValue;
    }
    else if(LabEditPtr!=NULL)
    {
        LabEditPtr->Text=iValue;
    }
}
//------------------------------------------------------------------------------
void TfARMS::SetParameterToShow(TObject *PCtrl,bool bValue)
{
    TCheckBox *PCheckBox     = dynamic_cast <TCheckBox *>(PCtrl);

    if(PCheckBox!=NULL)
    {
        PCheckBox->Checked=bValue;
    }
}
//------------------------------------------------------------------------------
void TfARMS::SetParameterToShow(TObject *PCtrl, char *cValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        PEdit->Text=cValue;
    }
    else if(LabEditPtr!=NULL)
    {
        LabEditPtr->Text=cValue;
    }
}
//------------------------------------------------------------------------------
void TfARMS::SetParameterToShow(TObject *PCtrl, AnsiString aValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        PEdit->Text=aValue;
    }
    else if(LabEditPtr!=NULL)
    {
        LabEditPtr->Text=aValue;
    }
}
//==============================================================================
//                              寫入元件到參數
//==============================================================================
void TfARMS::SetVCLToParameter(TComboBox *PCtrl, int &iValue)
{
    TComboBox *PComboBox     = dynamic_cast <TComboBox *>(PCtrl);

    if(PComboBox!=NULL)
    {
        iValue=PComboBox->ItemIndex;
    }
}
//---------------------------------------------------------------------------
void TfARMS::SetVCLToParameter(TObject *PCtrl, char &aValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        aValue=*PEdit->Text.c_str();
    }
    else if(LabEditPtr!=NULL)
    {
        aValue=*LabEditPtr->Text.c_str();
    }
}
//==============================================================================
void TfARMS::SetVCLToParameter(TObject *PCtrl, AnsiString &aValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        aValue=PEdit->Text;
    }
    else if(LabEditPtr!=NULL)
    {
        aValue=LabEditPtr->Text;
    }
}
//---------------------------------------------------------------------------
void TfARMS::SetVCLToParameter(TObject *PCtrl, int &iValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        iValue=atoi(PEdit->Text.c_str());
    }
    else if(LabEditPtr!=NULL)
    {
        iValue=atoi(LabEditPtr->Text.c_str());
    }
}
//------------------------------------------------------------------------------
void TfARMS::SetVCLToParameter(TObject *PCtrl, bool &bValue)
{
    TCheckBox *PCheckBox     = dynamic_cast <TCheckBox *>(PCtrl);

    if(PCheckBox!=NULL)
    {
        bValue=PCheckBox->Checked;
    }
}
//==============================================================================
void __fastcall TfARMS::sbUploadRecipePathClick(TObject *Sender)
{
    int Tag;
    AnsiString Dir;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    if(SelectDirectory("選擇資料夾名稱","",Dir))
    {
        if(Application->MessageBox(Dir.c_str(), "路徑是否確定?", MB_YESNO)==IDYES);
        {
            if(Tag==0)                                                          //upload
            {
                edUploadRecipePath->Text=Dir;
            }
            else if(Tag==1)                                                     //download
            {
                edDownloadRecipePath->Text=Dir;
            }
            else if(Tag==2)                                                     //download
            {
                edBackupUploadRecipe->Text=Dir;
            }
        }
    }
}
//==============================================================================
bool __fastcall TfARMS::CheckDirExit(int iPath)
{
    AnsiString aPath;

    if(iPath==iDownloadTag)
        aPath = ARMS_PARA.aASEMARMSDownloadPath;
    else if(iPath==iBackupTag)
        aPath=ARMS_PARA.aASEMARMSBackupUploadPath;
    else
        aPath=ARMS_PARA.aASEMARMSUploadPath;

    if(DirectoryExists(aPath)==false)
    {
        return false;
    }
    return true;
}
//==============================================================================
void __fastcall TfARMS::pcARMSFoemChange(TObject *Sender)
{
    pnButtonTitle->Parent=pcARMSFoem->ActivePage;
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbRefreshDownloadRecipeClick(TObject *Sender)
{
    int Tag;
    AnsiString sPath;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    if(CheckDirExit(iDownloadTag)==false)
    {
        ShowMyMessage("Please Check Internet Is Connect!");
        return;
    }

    if(Tag==1)
    {
        sPath = ARMS_PARA.aASEMARMSDownloadPath;
    }
    else if(Tag==3)
    {
        sPath = ARMS_PARA.aASEMARMSBackupUploadPath;
    }
    else if(Tag==2)
    {
        sPath=ARMS_PARA.aASEMARMSUploadPath;
    }
    UpdataServerRecipe(Tag, sPath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UpdataServerRecipe(int iTag, AnsiString sPath)
{
    TSearchRec sr;

    if(iTag==1)
    {
        CheckListBoxDownloadRecipe->Items->Clear();
    }
    else if(iTag==2)
    {
        CheckListBoxUploadRecipe->Items->Clear();
    }
    else if(iTag==3)
    {
        CheckListBoxBackupUploadRecipe->Items->Clear();
    }

    if(!DirectoryExists(sPath))
    {
        ShowMyMessage("File Path Lost!!");
        return;
    }
    else
    {
        WIN32_FIND_DATA filedata;                                               // Structure for file data
        HANDLE filehandle;                                                      // Handle for searching
        AnsiString szFileName;
        filehandle=FindFirstFile((sPath+"\\*").c_str(), &filedata);

        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                /* 不處理隱藏檔及 . 跟 .. */
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata.cFileName, ".")==0 ||
                    strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    cobHandlerRecipeName->Items->Add(ExtractFileName(filedata.cFileName));

                    if(iTag==1)
                    {
                        CheckListBoxDownloadRecipe->Items->Add(ExtractFileName(filedata.cFileName));
                    }
                    else if(iTag==2)
                    {
                        CheckListBoxUploadRecipe->Items->Add(ExtractFileName(filedata.cFileName));
                    }
                    else if(iTag==3)
                    {
                        CheckListBoxBackupUploadRecipe->Items->Add(ExtractFileName(filedata.cFileName));
                    }
                }
            } while(FindNextFile(filehandle, &filedata));
            FindClose(filehandle);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::FormShow(TObject *Sender)
{
    Left  = (1024-Width)/2;
    Top   = (768-Height)/2;

    pcARMSFoem->ActivePageIndex = 0;
    LoadParameter();
    UpdataHandlerRecipe();
    if(AccessLevel<iDefSupervisorLevel)
    {
        sbARMSRecipeUpload->Enabled = false;
        gbARMSConfig->Enabled       = false;
    }
    else
    {
        sbARMSRecipeUpload->Enabled = true;
        gbARMSConfig->Enabled       = true;
    }
    sbARMSRecipeDownload->Enabled   = true;
    cobHandlerRecipeName->Text = fMain->cbSetupFileName->Text;
    FileListBoxARMS->Mask = ARMS_PARA.aASEMARMSBackupUploadPath+"\\*.txt";
    FileListBoxARMS->Update();
    fShow=true;
    UpdataServerRecipe(iDownloadTag,ARMS_PARA.aASEMARMSDownloadPath);
    UpdataServerRecipe(iUploadTag,ARMS_PARA.aASEMARMSUploadPath);
    UpdataServerRecipe(iBackupTag,ARMS_PARA.aASEMARMSBackupUploadPath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::FormClose(TObject *Sender, TCloseAction &Action)
{
//    UpdataParameter();
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UpdataParameter()
{
    TIniFile* IniFile=new TIniFile(_FILE_PTAH);

    SetVCLToParameter(edDownloadRecipePath                 , ARMS_PARA.aASEMARMSDownloadPath);
    SetVCLToParameter(edUploadRecipePath                   , ARMS_PARA.aASEMARMSUploadPath);
    SetVCLToParameter(cbDownloadFormUploadPath             , ARMS_PARA.bDownloadFormUploadPath);
    SetVCLToParameter(cbRecipeDownloadIncludeOffsetFile    , ARMS_PARA.bRecipeDownloadIncludeOffsetFile);
    SetVCLToParameter(edBackupUploadRecipe                 , ARMS_PARA.aASEMARMSBackupUploadPath);

    IniFile->WriteString("ServerPathSetup"  , "DownloadPath"        , ARMS_PARA.aASEMARMSDownloadPath);
    IniFile->WriteString("ServerPathSetup"  , "UploadPath"          , ARMS_PARA.aASEMARMSUploadPath);
    IniFile->WriteBool  ("Config"           , "A01"                 , ARMS_PARA.bDownloadFormUploadPath);
    IniFile->WriteBool  ("Config"           , "A02"                 , ARMS_PARA.bRecipeDownloadIncludeOffsetFile);
    IniFile->WriteString("ServerPathSetup"  , "BackupUploadPath"    , ARMS_PARA.aASEMARMSBackupUploadPath);

    delete IniFile;
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::LoadParameter()
{
    TIniFile* IniFile=new TIniFile(_FILE_PTAH);

    ARMS_PARA.aASEMARMSDownloadPath=IniFile->ReadString("ServerPathSetup" , "DownloadPath", "");
    ARMS_PARA.aASEMARMSUploadPath = IniFile->ReadString("ServerPathSetup" , "UploadPath", "");
    ARMS_PARA.aASEMARMSBackupUploadPath = IniFile->ReadString("ServerPathSetup" , "BackupUploadPath", "");
    ARMS_PARA.bDownloadFormUploadPath = IniFile->ReadBool("Config" , "A01", false);
    ARMS_PARA.bRecipeDownloadIncludeOffsetFile = IniFile->ReadBool("Config" , "A02", false);

    SetParameterToShow(edDownloadRecipePath                 , ARMS_PARA.aASEMARMSDownloadPath);
    SetParameterToShow(edUploadRecipePath                   , ARMS_PARA.aASEMARMSUploadPath);
    SetParameterToShow(cbDownloadFormUploadPath             , ARMS_PARA.bDownloadFormUploadPath);
    SetParameterToShow(cbRecipeDownloadIncludeOffsetFile    , ARMS_PARA.bRecipeDownloadIncludeOffsetFile);
    SetParameterToShow(edBackupUploadRecipe                 , ARMS_PARA.aASEMARMSBackupUploadPath);
    delete IniFile;                                                             //Steven 20200506 : 用完要delete
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbEquipmentUpdateClick(TObject *Sender)
{
    UpdataParameter();
    UpdataHandlerRecipe();
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UpdataHandlerRecipe()
{
    AnsiString sOldDir="", sNewDir="";
    static bool bHasOffsetData=true;
    cobHandlerRecipeName->Items->Clear();
    if(!DirectoryExists(DataPath))
    {
        ShowMyMessage("File Path Lost!!");
        return;
    }
    else
    {
        WIN32_FIND_DATA filedata;                                               // Structure for file data
        HANDLE filehandle;                                                      // Handle for searching
        AnsiString szFileName;
        filehandle=FindFirstFile((DataPath+"*").c_str(), &filedata);

        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||    /* 不處理隱藏檔及 . 跟 .. */
                    strcmp(filedata.cFileName, ".")==0 ||
                    strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   //如果該檔案為資料夾
                {
                    cobHandlerRecipeName->Items->Add(ExtractFileName(filedata.cFileName)); // 將檔名加到 cbSetupFileName

                    if(bHasOffsetData==false)                                   //Offset資料夾不存在，表示要把Offset移動過來
                    {
                        sOldDir=DataPath+filedata.cFileName;
                        sNewDir=OffsetPath+filedata.cFileName;
                        MyForceDirectories(sNewDir);
                        MoveFile(AnsiString(sOldDir+"\\Position Offset.Data").c_str(), AnsiString(sNewDir+"\\Position Offset.Data").c_str());
                        MoveFile(AnsiString(sOldDir+"\\Position Offset Hot.Data").c_str(), AnsiString(sNewDir+"\\Position Offset Hot.Data").c_str());
                    }

                    if(FileExists(DataPath+filedata.cFileName+"\\123aaa.ini"))  //幽靈檔案，砍掉
                    {
                        DeleteFile(AnsiString(DataPath+filedata.cFileName+"\\123aaa.ini").c_str());
                    }
                }
            } while(FindNextFile(filehandle, &filedata));
            FindClose(filehandle);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UnCheckedAllSelet()
{
    int i;
    for(i=0; i<CheckListBoxUploadRecipe->Items->Count; i++)
    {
        if(CheckListBoxUploadRecipe->Checked[i])
        {
            CheckListBoxUploadRecipe->Checked[i]=false;
        }
    }
    for(i=0; i<CheckListBoxDownloadRecipe->Items->Count; i++)
    {
        if(CheckListBoxDownloadRecipe->Checked[i])
        {
            CheckListBoxDownloadRecipe->Checked[i]=false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UploadHandlerRecipe()
{
    AnsiString SNamePath    = aHandlerDataPath+cobHandlerRecipeName->Text;
    AnsiString aServerPath  = ARMS_PARA.aASEMARMSUploadPath+"\\"+cobHandlerRecipeName->Text;

    if(DirectoryExists(aServerPath))                                            //Ifor 20170710 add ARMS Upload 檔案若已存在需備份到另一個資料夾
    {
        BackupHandlerRecipe(cobHandlerRecipeName->Text);
    }
    ARMSDataConversion(SNamePath,aServerPath, false, false);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::BackupHandlerRecipe(AnsiString aName)
{
    AnsiString SBackupPath  = ARMS_PARA.aASEMARMSBackupUploadPath+"\\"+aName;
    AnsiString aServerPath  = ARMS_PARA.aASEMARMSUploadPath+"\\"+aName;

    ARMSDataConversion(aServerPath,SBackupPath, false, true);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbARMSRecipeUploadClick(TObject *Sender)
{
    if(CheckDirExit(iUploadTag)==false)
    {
        ShowMyMessage("Please Check Internet Is Connect!");
        return;
    }
    sbARMSRecipeUpload->Enabled = false;
    if(cobHandlerRecipeName->Text=="")
    {
        sbARMSRecipeUpload->Enabled = true;
        ShowMyMessage("Please Keyin Upload Recipe Name");
        return;
    }

    if(fARMSLog->bShow==false)
    {
        fARMSLog->SavePath=ARMS_PARA.aASEMARMSBackupUploadPath;
        fARMSLog->SaveName=cobHandlerRecipeName->Text;
        fARMSLog->ShowModal();
        if(fARMSLog->bSaveReady==false)
        {
            sbARMSRecipeUpload->Enabled=true;
            return;
        }
    }
    UploadHandlerRecipe();
    sbARMSRecipeUpload->Enabled=true;
    UnCheckedAllSelet();
    ShowMyMessage("Upload successful");
    MySleep(1200);
    UpdataServerRecipe(iUploadTag,ARMS_PARA.aASEMARMSUploadPath);
    UpdataServerRecipe(iBackupTag,ARMS_PARA.aASEMARMSBackupUploadPath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::UploadRecipeMoveToDownloadRecipe()
{
    AnsiString aUploadRecipe, aDownLoadRecipe;
    AnsiString str;

    for(int i=0; i<CheckListBoxUploadRecipe->Items->Count; i++)
    {
        if(CheckListBoxUploadRecipe->Checked[i])
        {
            aUploadRecipe = ARMS_PARA.aASEMARMSUploadPath+"\\"+CheckListBoxUploadRecipe->Items->Strings[i];
            aDownLoadRecipe = ARMS_PARA.aASEMARMSDownloadPath+"\\"+CheckListBoxUploadRecipe->Items->Strings[i];
            break;
        }
    }
    ARMSDataConversion(aUploadRecipe,aDownLoadRecipe, true, false);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbMoveToDownloadClick(TObject *Sender)
{
    if(CheckListBoxIsSelete(CheckListBoxUploadRecipe)==false)
    {
        return;
    }

    if(CheckPassword(_FILE_PTAH)==false)
    {
        ShowMyMessage("Pssword Error");
        return;
    }

    UploadRecipeMoveToDownloadRecipe();
    ShowMyMessage("Move successful");
    MySleep(1200);
    UpdataServerRecipe(iDownloadTag, ARMS_PARA.aASEMARMSDownloadPath);
    UpdataServerRecipe(iUploadTag, ARMS_PARA.aASEMARMSUploadPath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::DownloadRecipeFromServer(bool bflag)                    //bflag=false DownloadPath ; bflag=true UploadPath
{
    int i;
    AnsiString aHandler, asstr2;
    AnsiString asRecipePatch;

    if( bflag )
    {
        for(i=0; i<CheckListBoxUploadRecipe->Items->Count; i++)
        {
            if(CheckListBoxUploadRecipe->Checked[i])
            {
                asRecipePatch=ARMS_PARA.aASEMARMSUploadPath+"\\"+CheckListBoxUploadRecipe->Items->Strings[i];
                asstr2=CheckListBoxUploadRecipe->Items->Strings[i];
                break;
            }
        }
    }
    else
    {
        for(i=0; i<CheckListBoxDownloadRecipe->Items->Count; i++)
        {
            if(CheckListBoxDownloadRecipe->Checked[i])
            {
                asRecipePatch=ARMS_PARA.aASEMARMSDownloadPath+"\\"+CheckListBoxDownloadRecipe->Items->Strings[i];
                asstr2=CheckListBoxDownloadRecipe->Items->Strings[i];
                break;
            }
        }
    }
    aHandler=aHandlerDataPath+asstr2;
    ARMSDataConversion(asRecipePatch, aHandler, false, false);

    fMain->cbSetupFileName->Items->Add(asstr2);
    fMain->cbSetupFileName->Text=asstr2;
    SYS_SetupFile=asstr2;
    fMain->cbSetupFileNameChange(fMain);
    fMain->Button5Click(this);

    DownloadWorkFile_NET    = SYS_SetupFile;
    LastSetTemperature_NET  = LastSet.iTemperature;
    TrayForm_NET            = TrayForm;
    HotPlateForm_NET        = HotPlateForm_File;
    Temperature_NET         = Temperature;
    TestIF_NET              = TestIF_File;
    for(int i=0; i<8; i++)                                                      //ChungHung 20141002 add for KYEC AutoRetest 3->5 //Ifor 20170418 add MRT Mode 5 -> 8
    {
        BinSelect_NET[i]    = BinSelect[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbARMSRecipeDownloadClick(TObject *Sender)
{
    if(CheckDirExit(iDownloadTag)==false)
    {
        ShowMyMessage("Please Check Internet Is Connect!");
        return;
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
    {
        ShowMyMessage("Machine Has IC Can't Download Setup File!");
        return;
    }

    sbARMSRecipeDownload->Enabled=false;

    if(cbDownloadFormUploadPath->Checked)
    {
        if(CheckListBoxIsSelete(CheckListBoxUploadRecipe)==false)
        {
            sbARMSRecipeDownload->Enabled=true;
            return;
        }
    }
    else
    {
        if(CheckListBoxIsSelete(CheckListBoxDownloadRecipe)==false)
        {
            sbARMSRecipeDownload->Enabled=true;
            return;
        }
    }

    if(cbDownloadFormUploadPath->Checked)
        DownloadRecipeFromServer(true);
    else
        DownloadRecipeFromServer(false);

    sbARMSRecipeDownload->Enabled=true;
    UnCheckedAllSelet();
//    cbDownloadFormUploadPath->Checked=false;                                  //Ifor 20170714 Download 檔案改變程式設定欓? 暫時Mark
    ShowMyMessage("Download successful");
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::sbDeletDownloadRecipeClick(TObject *Sender)
{
    AnsiString aUpLoadRecipe="";
    int Tag;
    AnsiString sPath;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    if(CheckPassword(_FILE_PTAH)==false)
    {
        return;
    }

    if(Tag==1)
    {
        if(CheckListBoxIsSelete(CheckListBoxUploadRecipe)==false)
        {
            return;
        }
        for(int i=0; i<CheckListBoxUploadRecipe->Items->Count; i++)
        {
            if(CheckListBoxUploadRecipe->Checked[i])
            {
                aUpLoadRecipe=ARMS_PARA.aASEMARMSUploadPath+"\\"+CheckListBoxUploadRecipe->Items->Strings[i];
                break;
            }
        }
        DeleteDirectory(aUpLoadRecipe);
        UpdataServerRecipe(iUploadTag, ARMS_PARA.aASEMARMSUploadPath);
    }
    else if(Tag==2)
    {
        if(CheckListBoxIsSelete(CheckListBoxBackupUploadRecipe)==false)
        {
            return;
        }
        for(int i=0; i<CheckListBoxBackupUploadRecipe->Items->Count; i++)
        {
            if(CheckListBoxBackupUploadRecipe->Checked[i])
            {
                aUpLoadRecipe=ARMS_PARA.aASEMARMSBackupUploadPath+"\\"+CheckListBoxBackupUploadRecipe->Items->Strings[i];
                break;
            }
        }
        DeleteDirectory(aUpLoadRecipe);
        UpdataServerRecipe(iBackupTag, ARMS_PARA.aASEMARMSBackupUploadPath);
    }
    else
    {
        if(CheckListBoxIsSelete(CheckListBoxDownloadRecipe)==false)
        {
            return;
        }
        for(int i=0; i<CheckListBoxDownloadRecipe->Items->Count; i++)
        {
            if(CheckListBoxDownloadRecipe->Checked[i])
            {
                aUpLoadRecipe=ARMS_PARA.aASEMARMSDownloadPath+"\\"+CheckListBoxDownloadRecipe->Items->Strings[i];
                break;
            }
        }
        DeleteDirectory(aUpLoadRecipe);
        UpdataServerRecipe(iDownloadTag, ARMS_PARA.aASEMARMSDownloadPath);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfARMS::CheckRecipeParameter()
{
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::FileListBoxARMSDblClick(TObject *Sender)
{
    AnsiString S, aFilePath;

    for(int i=0; i<FileListBoxARMS->Items->Count; i++)
    {
        if(FileListBoxARMS->Selected[i]==false)
            continue;
        S=FileListBoxARMS->Items->Strings[i];
    }
    aFilePath=ARMS_PARA.aASEMARMSBackupUploadPath+"\\"+S;
    LoadRecipeLogData(aFilePath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::LoadRecipeLogData(AnsiString aName)
{
    MemoRecipeLog->Lines->LoadFromFile(aName);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::BackupRecipeMoveToUploadRecipe()
{
    AnsiString aUploadRecipe,aBackupRecipe;
    AnsiString str;

    for(int i=0; i<CheckListBoxBackupUploadRecipe->Items->Count; i++)
    {
        if(CheckListBoxBackupUploadRecipe->Checked[i])
        {
            aBackupRecipe=ARMS_PARA.aASEMARMSBackupUploadPath+"\\"+CheckListBoxBackupUploadRecipe->Items->Strings[i];
            aUploadRecipe=ARMS_PARA.aASEMARMSUploadPath+"\\"+CheckListBoxBackupUploadRecipe->Items->Strings[i];
            break;
        }
    }
    ARMSDataConversion(aBackupRecipe,aUploadRecipe,true,false);
}
//-------------------------------------------------------------------------
void __fastcall TfARMS::sbMoveToUploadRecipeFolderClick(TObject *Sender)
{
    if(CheckListBoxIsSelete(CheckListBoxBackupUploadRecipe)==false)
    {
        return;
    }

    if(CheckPassword(_FILE_PTAH)==false)
    {
        ShowMyMessage("Pssword Error");
        return;
    }

    BackupRecipeMoveToUploadRecipe();
    ShowMyMessage("Move successful");
    MySleep(1200);
    UpdataServerRecipe(iUploadTag, ARMS_PARA.aASEMARMSUploadPath);
    UpdataServerRecipe(iBackupTag, ARMS_PARA.aASEMARMSBackupUploadPath);
}
//---------------------------------------------------------------------------
void __fastcall TfARMS::ARMSDataConversion(AnsiString asSource, AnsiString asDirection, bool bDeleteSource, bool bBackup)
{
    AnsiString str;
    TStringList *TestList=new TStringList;

    str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asSource, asDirection); //複製檔案
    TestList->Add(str);

    if(bDeleteSource==true)                                                     //Ifor 20170714 刪除來源檔案
    {
        str.sprintf("rmdir /s/q \"%s\"", asSource);
        TestList->Add(str);
    }

    if(bBackup==true)                                                           //Ifor 20170714 add 備份與複製分開避免發生異常
    {
        TestList->SaveToFile("D:\\HT9045\\system\\ARMSBackup.bat");
        ExecZipCommand      ("D:\\HT9045\\system\\ARMSBackup.bat", " ");
    }
    else
    {
        TestList->SaveToFile("D:\\HT9045\\system\\ARMS.bat");
        ExecZipCommand      ("D:\\HT9045\\system\\ARMS.bat", " ");
    }

    TestList->Clear();
    delete TestList;
}
//---------------------------------------------------------------------------

