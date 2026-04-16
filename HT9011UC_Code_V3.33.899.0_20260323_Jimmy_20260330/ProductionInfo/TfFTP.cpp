#include "MachineDefine.h"
#pragma hdrstop

#include "TfFTP.h"
#include "common.h"
#include "FileInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

extern bool MNetLog(AnsiString Message);                                        //Steven 20110406
//---------------------------------------------------------------------------
TfFTP::TfFTP(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost)
{
//    #ifdef SOFT_SIMULTE
//    asUserID    ="HONPREC";
//    asPassword  ="27025312";
//    asHost      ="127.0.0.1";
//    #else
    asUserID    =_asUserID;
    asPassword  =_asPassword;
    asHost      =_asHost;
//    #endif
    TimeOut     =5000;
    Vendor      =NMOS_AUTO;
    Passive     =true;
    Port        =21;

    pFTP        =new TNMFTP(NULL);
    lFolder     =new TStringList;
    lTempFolder =new TStringList;
    bError      =false;
    bChangeDirOk=false;
    bNListOk    =false;    

    pFTP->OnListItem              =NMFTP2ListItem;
    pFTP->OnSuccess               =NMFTP2Success;
    pFTP->OnFailure               =NMFTP2Failure;
    pFTP->OnError                 =NMFTP2Error;
    pFTP->OnStatus                =NMFTP2Status;
    pFTP->OnConnectionFailed      =NMFTP2ConnectionFailed;
    pFTP->OnTransactionStart      =NMFTP2TransactionStart;
    pFTP->OnTransactionStop       =NMFTP2TransactionStop;
    pFTP->OnAuthenticationFailed  =NMFTP2AuthenticationFailed;
    pFTP->OnUnSupportedFunction   =NMFTP2UnSupportedFunction;
    pFTP->OnDisconnect            =NMFTP2Disconnect;
    pFTP->OnConnect               =NMFTP2Connect;
}
//---------------------------------------------------------------------------
TfFTP::~TfFTP()                                                                 //Jimmychiu 20220901 新增Close FTP
{
    Close();
    lFolder->Clear();
    delete lFolder;

    lTempFolder->Clear();
    delete lTempFolder;
}
//---------------------------------------------------------------------------
bool TfFTP::Connect()
{
    pFTP->Vendor    =Vendor;
    pFTP->TimeOut   =TimeOut;                                                   //Jimmychiu 20220901 調整FTP連線間隔3sec->0.5sec
    pFTP->Passive   =Passive;
    pFTP->Port      =Port;
    pFTP->UserID    =asUserID;
    pFTP->Password  =asPassword;
    pFTP->Host      =asHost;
    try
    {
        if(pFTP->Connected)
            return true;

        pFTP->Connect();
        for(int i=0; i<100; i++)                                                // 100 x 5 = 500 ( 0.5 Sec )
        {
            MySleep(5);
            if(pFTP->Connected)
            {
                return true;
            }
        }
    }
    catch(...)
    {
        Close();                                                                //Jimmychiu 20220901 新增Close FTP
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfFTP::Connect(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost, int _TimeOut, int _Vendor, bool _Passive, int _Port)
{
    Port        =_Port;
    Vendor      =_Vendor;
    TimeOut     =_TimeOut;
    Passive     =_Passive;
    asUserID    =_asUserID;
    asPassword  =_asPassword;
    asHost      =_asHost;

    return Connect();
}
//---------------------------------------------------------------------------
// 輔助函數：將路徑分割為資料夾列表
TStringList* TfFTP::SplitPath(AnsiString path)
{
    TStringList *folders = new TStringList();

    if (path.IsEmpty())
        return folders;

    int start = 1;
    int len = path.Length();
    while (start <= len)
    {
        // 從 start 開始找下一個 '/'
        AnsiString sub = path.SubString(start, len - start + 1);
        int posSlash = sub.Pos("/");

        AnsiString part;

        if (posSlash > 0)
        {
            // 擷取 / 之前的部分
            if (posSlash > 1)
            {
                part = path.SubString(start, posSlash - 1);
                part = part.Trim();
                if (!part.IsEmpty())
                    folders->Add(part);
            }
            // 將 start 移動到下一個位置（略過這個 '/')
            start = start + posSlash;
        }
        else
        {
            // 最後一段
            part = path.SubString(start, len - start + 1);
            part = part.Trim();
            if (!part.IsEmpty())
                folders->Add(part);
            break;
        }
    }

    return folders;
}
//---------------------------------------------------------------------------
// 輔助函數：回退到根目錄
bool TfFTP::NavigateToCommonPrefix()
{
    AnsiString currentPath=pFTP->CurrentDir;
    TStringList *folders=SplitPath(currentPath);
    bool bret=false;
    try
    {
        if(currentPath=="/")
        {
        }
        else
        {
            for (int i=folders->Count-1; i>=0; i--)
            {
                ChangeDir("..");
            }
        }
        bret=true;
    }
    catch (Exception &e)
    {
        ShowMessage("無法回退到共同前綴或根目錄: " + e.Message);
        bret=false;
    }
    delete folders;
    return bret;
}
//---------------------------------------------------------------------------
// 輔助函數：逐層切換或創建資料夾
bool TfFTP::NavigateOrCreateFolders(TStringList *targetFolders, AnsiString originalDir)
{
    try
    {
        // 逐層檢查，從根目錄開始
        AnsiString currentPath=pFTP->CurrentDir;
        AnsiString sPath="";
        for (int i = 0; i < targetFolders->Count; i++)
        {
            if(i==0)
            {
                if(currentPath!="/")
                {
                    ChangeDir("/");
                }
            }
            sPath=targetFolders->Strings[i];
            if (sPath.IsEmpty())
                continue; 
            if(ChangeDir(sPath)==false)
            {
                currentPath=pFTP->CurrentDir;
                ShowMessage("無法創建或切換到資料夾: " + sPath+"   AbsPath"+currentPath);
                return false;
            }
        }
        return true;
    }
    catch (Exception &e)
    {
        ShowMessage("錯誤: " + e.Message);
        return false;
    }
}
//---------------------------------------------------------------------------
// 輔助函數：清理路徑（移除首尾斜線）
AnsiString TfFTP::CleanPath(AnsiString path)
{
    path = path.Trim();
    if (path == "/")
        return path;
    while (path.Length() > 1 && path[1] == '/')
        path = path.SubString(2, path.Length() - 1);
    while (path.Length() > 1 && path[path.Length()] == '/')
        path = path.SubString(1, path.Length() - 1);
    // 移除中間多餘斜線
    AnsiString cleanPath = "";
    bool lastWasSlash = false;
    for (int i = 1; i <= path.Length(); i++)
    {
        if (path[i] == '/')
        {
            if (!lastWasSlash)
            {
                cleanPath += '/';
                lastWasSlash = true;
            }
        }
        else
        {
            cleanPath += path[i];
            lastWasSlash = false;
        }
    }
    return cleanPath;
}
//---------------------------------------------------------------------------
bool TfFTP::ChangeDirectories(AnsiString asFilePath)                            //Steven 20250716 : 多層次FTP資料夾切換
{
    AnsiString CurrentDir=pFTP->CurrentDir;
    CheckFTPFilePath(asFilePath);
    // 清理目標路徑和當前路徑
    asFilePath=CleanPath(asFilePath);
    // 分割路徑
    TStringList *targetFolders = SplitPath(asFilePath);
    // 保存原始工作目錄
    AnsiString originalDir = CurrentDir;
    // 逐層切換或創建剩餘資料夾
    NavigateToCommonPrefix();
    bool success = NavigateOrCreateFolders(targetFolders, originalDir);
    // 清理
    delete targetFolders;
    return success;
}
//---------------------------------------------------------------------------
bool TfFTP::ChangeDir(AnsiString asFilePath)
{
    bflag=false;                                                                //check Directory
    AnsiString sFilePath=RemoveAllTrailingSlashes(asFilePath);
    try                                                                         //檢查目標路徑，用 ChangeDir 的方式來檢查
    {
        pFTP->ChangeDir(sFilePath);
    }
    catch(...)
    {
        bflag=true;
    }

    try                                                                         //檢查目標路徑，用 ChangeDir 的方式來檢查
    {
        if(bflag)
        {
            pFTP->MakeDirectory(sFilePath);
            pFTP->ChangeDir(sFilePath);
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TfFTP::CheckFTPFilePath(AnsiString &aspath)
{
    if(aspath=="NULL")
        return;

    aspath=StringReplace(aspath, "\\", "//", TReplaceFlags()<<rfReplaceAll);
    aspath=StringReplace(aspath, "////", "//", TReplaceFlags()<<rfReplaceAll);
    if(aspath.SubString(aspath.Length()-1, 2)!="//")
        aspath=aspath+AnsiString("//");
}
//---------------------------------------------------------------------------
void TfFTP::CheckLocalFilePath(AnsiString &aspath)
{
    if(aspath=="NULL")
        return;

    if(aspath.SubString(aspath.Length(), 1)!="\\")
    {
        aspath=aspath+AnsiString("\\");
    }
}
//---------------------------------------------------------------------------
bool TfFTP::Rename(const AnsiString &sourcePath, const AnsiString &targetPath, const AnsiString &sSourFileName, const AnsiString &sTarFileName)
{
    try
    {
        AnsiString asError="", asFileName="", asSource="", asTarget="";
        ChangeDir(sourcePath);
        ChangeDir(targetPath);
        asSource=sourcePath;
        GetAllFolder(asSource, asError);
        if(asError!="")
            return false;

        if(mapFileList.size()!=0)
        {
            for(unsigned int i=0; i<mapFileList.size(); i++)
            {
                asFileName=mapFileList[i];
                if(asFileName.Pos(sSourFileName)>0)
                {
                    asSource=FileInfo().PathCombin(sourcePath, asFileName);
                    asTarget=FileInfo().PathCombin(targetPath, sTarFileName);
                    pFTP->Rename(asSource, asTarget);
                    return true;
                }
            }
        }
        return false;
    }
    catch(...)
    {
        return false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFTP::GetFileList(AnsiString Listing)
{
    mapFileList[mapFileList.size()]=Listing;
}
//------------------------------------------------------------------------------
bool TfFTP::DownloadFilterFile(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError)
{
    bool bReturn=false;
    AnsiString asFileName;
    try
    {
        asFilter=asFilter.UpperCase();                                          //Jimmychiu 20230503 : FTP Download不分檔案大小寫
        GetAllFolder(sSourcesFilePath, asError);
        if(asError!="")
            return bReturn;
        //check target file path
        CheckLocalFilePath(sTargetFilePath);
        if(DirectoryExists(sTargetFilePath)==false)
        {
            MyForceDirectories(sTargetFilePath);
            if(DirectoryExists(sTargetFilePath)==false)
            {
                asError=AnsiString("Error Target File Path : "+sTargetFilePath);
                return bReturn;
            }
        }

        if(mapFileList.size()!=0)
        {
            for(unsigned int i=0; i<mapFileList.size(); i++)
            {
                if(mapFileList[i].UpperCase().Pos(asFilter)>0)                  //Jimmychiu 20230503 : FTP Download不分檔案大小寫
                {
                    asFileName=mapFileList[i];
                    pFTP->Download(sSourcesFilePath+asFileName, sTargetFilePath+asFileName);
                    bReturn=true;
                }
            }
        }
    }
    catch(...)
    {
        asError=AnsiString("FTP Connect Error");
        return bReturn;
    }

    if(bReturn==false)
        asError=AnsiString("No File is Downloaded");
    return bReturn;
}
//------------------------------------------------------------------------------
AnsiString TfFTP::DownloadFilterFile_Get1stFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError)
{
    AnsiString asReturn="";
    AnsiString asFileName;
    try
    {
        asFilter=asFilter.UpperCase();                                          //Jimmychiu 20230503 : FTP Download不分檔案大小寫
        GetAllFolder(sSourcesFilePath, asError);
        if(asError!="")
            return asReturn;

        CheckLocalFilePath(sTargetFilePath);                                    //check target file path
        if(DirectoryExists(sTargetFilePath)==false)
        {
            MyForceDirectories(sTargetFilePath);
            if(DirectoryExists(sTargetFilePath)==false)
            {
                asError=AnsiString("Error Target File Path : "+sTargetFilePath);
                return asReturn;
            }
        }

        if(mapFileList.size()!=0)
        {
            for(unsigned int i=0; i<mapFileList.size(); i++)
            {
                if(mapFileList[i].UpperCase().Pos(asFilter)>0)                  //Jimmychiu 20230503 : FTP Download不分檔案大小寫
                {
                    asFileName=mapFileList[i];
                    pFTP->Download(sSourcesFilePath+asFileName, sTargetFilePath+asFileName);
                    return asFileName;
                }
            }
        }
    }
    catch(...)
    {
        asError=AnsiString("FTP Connect Error");
        return asReturn;
    }
    if(asReturn=="")
        asError=AnsiString("No File is Downloaded");
    return asReturn;
}
//------------------------------------------------------------------------------
AnsiString TfFTP::DownloadFilterFile_GetLastFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError)
{
    AnsiString asReturn="";
    AnsiString asFileName="";
    try
    {
        asFilter=asFilter.UpperCase();                                          //Jimmychiu 20230503 : FTP Download不分檔案大小寫
        GetAllFolder(sSourcesFilePath, asError);
        if(asError!="")
            return asReturn;

        CheckLocalFilePath(sTargetFilePath);                                    //check target file path
        if(DirectoryExists(sTargetFilePath)==false)
        {
            MyForceDirectories(sTargetFilePath);
            if(DirectoryExists(sTargetFilePath)==false)
            {
                asError=AnsiString("Error Target File Path : "+sTargetFilePath);
                return asReturn;
            }
        }

        if(mapFileList.size()!=0)
        {
            for(unsigned int i=0; i<mapFileList.size(); i++)
            {
                if(mapFileList[i].UpperCase().Pos(asFilter)>0)                  //Jimmychiu 20230503 : FTP Download不分檔案大小寫
                {
                    asFileName=mapFileList[i];
                    pFTP->Download(sSourcesFilePath+asFileName, sTargetFilePath+asFileName);
                }
            }
        }
    }
    catch(...)
    {
        asError=AnsiString("FTP Connect Error");
        return asReturn;
    }

    if(asFileName!="")
        return asFileName;

    if(asReturn=="")
        asError=AnsiString("No File is Downloaded");

    return asReturn;
}
//------------------------------------------------------------------------------
bool TfFTP::Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFileName, AnsiString asError)
{
    return Upload(sSourcesFilePath, sTargetFilePath, asFileName, asFileName, asError);
}
//------------------------------------------------------------------------------
bool TfFTP::Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSourceFileName, AnsiString sTargetFileName, AnsiString asError)
{
    bool bReturn=false;
    if(sSourceFileName=="NULL" && sSourceFileName=="")
    {
        asError=AnsiString("Can not find : "+sSourceFileName);
        return bReturn;
    }

    if(ChangeDirectories(sTargetFilePath)==false)                               //FTP
    {
        asError=AnsiString("Error Sources File Path : "+sSourcesFilePath);
        return bReturn;
    }

    CheckLocalFilePath(sSourcesFilePath);                                       //Local
    MyForceDirectories(sSourcesFilePath, __FUNC__);
    MNetLog("FTP, Upload!, "+FileInfo().PathCombin(sTargetFilePath, sTargetFileName));

    try
    {
        pFTP->Upload(FileInfo().PathCombin(sSourcesFilePath, sSourceFileName), sTargetFileName);
        return true;
    }
    catch(...)
    {
        MNetLog("FTP, Upload Fail!, "+FileInfo().PathCombin(sTargetFilePath, sTargetFileName));
        return bReturn;
    }
}
//------------------------------------------------------------------------------
bool TfFTP::Delete(AnsiString sSourcesFilePath, AnsiString asFilter, AnsiString asError)
{
    try
    {
        AnsiString asFileName;
        GetAllFolder(sSourcesFilePath, asError);
        if(asError!="")
            return false;
        if(mapFileList.size()!=0)
        {
            for(unsigned int i=0; i<mapFileList.size(); i++)
            {
                asFileName=mapFileList[i];
                if(asFileName.Pos(asFilter)>0)
                {
                    asFileName=mapFileList[i];
                    pFTP->Delete(asFileName);
                    return true;
                }
            }
        }
        asError=AnsiString("No File Filter");
        return false;
    }
    catch(...)
    {
        asError=AnsiString("FTP Delete Error");
        return false;
    }
}
//------------------------------------------------------------------------------
std::map<int, AnsiString> TfFTP::GetAllFolder(AnsiString &sSourcesFilePath, AnsiString asError)
{
    asError="";
    if(pFTP==NULL)
    {
        asError=AnsiString("FTP UnConnected");
        return mapFileList;
    }
    try
    {
        CheckFTPFilePath(sSourcesFilePath);
        if(ChangeDir(sSourcesFilePath)==false)
        {
            asError=AnsiString("Error Sources File Path : "+sSourcesFilePath);
            return mapFileList;
        }
        pFTP->OnListItem=GetFileList;
        if(mapFileList.size()!=0)
            mapFileList.clear();
        pFTP->Nlist();
        MySleep(1);
    }
    catch(...)
    {
        asError=AnsiString("FTP GetAllFolder Error");
        if(mapFileList.size()!=0)
            mapFileList.clear();
    }
    return mapFileList;
}
//------------------------------------------------------------------------------
void TfFTP::Close()                                                             //Jimmychiu 20220901 新增Close FTP
{
    MySleep(1);
    mapFileList.clear();
    try
    {
        if(pFTP!=NULL)
        {
            if(pFTP->Connected)
            {
                pFTP->RequestCloseSocket();
                pFTP->Disconnect();
            }
            pFTP->Abort();
            delete pFTP;
            pFTP=NULL;
        }
    }
    catch(...)
    {
    }
}
//------------------------------------------------------------------------------
AnsiString TfFTP::RemoveAllTrailingSlashes(AnsiString sInput)
{
    int ilen=sInput.Length();
    while(ilen>1 && sInput[ilen]=='/')
    {
        ilen--;
    }
    return sInput.SubString(1, ilen);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2ListItem(AnsiString Listing)
{
    lFolder->Add(Listing);                                                      //列出目錄內的檔案-----
}
//------------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Success(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MNetLog("FTP, Success -- ChangeDir successful"   );bChangeDirOk=true;break;
        case cmdMakeDir     : MNetLog("FTP, Success -- MakeDir successful"     );bChangeDirOk=true;break;
        case cmdRemoveDir   : MNetLog("FTP, Success -- RemoveDir successful"   );break;
        case cmdDelete      : MNetLog("FTP, Success -- Delete successful"      );break;
        case cmdRename      : MNetLog("FTP, Success -- Rename successful"      );break;
        case cmdReInit      : MNetLog("FTP, Success -- Reinitialize successful");break;
        case cmdCurrentDir  : MNetLog("FTP, Success -- CurrentDir successful"  );break;
        case cmdList        : MNetLog("FTP, Success -- List successful"        );bNListOk=true; break;
        case cmdNList       : MNetLog("FTP, Success -- NList successful"       );bNListOk=true; break;
        case cmdUpRestore   : MNetLog("FTP, Success -- UpRestore successful"   );break;
        case cmdDownRestore : MNetLog("FTP, Success -- DownRestore successful" );break;
        case cmdDownload    : MNetLog("FTP, Success -- Download successful"    );break;
        case cmdUpload      : MNetLog("FTP, Success -- Upload successful"      );break;
        case cmdAppend      : MNetLog("FTP, Success -- Append successful"      );break;
        case cmdAllocate    : MNetLog("FTP, Success -- Allocate successful"    );break;
        case cmdDoCommand   : MNetLog("FTP, Success -- DoCommand successful"   );break;
    }
    bError=false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2AuthenticationFailed(bool &Handled)
{
    MNetLog("FTP, Authentication Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2TransactionStop(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, Data transfer end, "+Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2TransactionStart(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, Data transfer start, "+Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2ConnectionFailed(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, Failure -- Connection Failed, "+Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Failure(bool &Handled, TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MNetLog("FTP, Failure -- ChangeDir failed"      ); bChangeDirOk=true;break;
        case cmdMakeDir     : MNetLog("FTP, Failure -- MakeDir failed"        ); bChangeDirOk=true;break;
        case cmdDelete      : MNetLog("FTP, Failure -- Delete failed"         ); break;
        case cmdRemoveDir   : MNetLog("FTP, Failure -- RemoveDir failed"      ); break;
        case cmdList        : MNetLog("FTP, Failure -- List failed"           ); bNListOk=true; break;
        case cmdRename      : MNetLog("FTP, Failure -- Rename failed"         ); break;
        case cmdUpRestore   : MNetLog("FTP, Failure -- UploadRestore failed"  ); break;
        case cmdDownRestore : MNetLog("FTP, Failure -- DownloadRestore failed"); break;
        case cmdDownload    : MNetLog("FTP, Failure -- Download failed"       ); break;
        case cmdUpload      : MNetLog("FTP, Failure -- Upload failed"         ); break;
        case cmdAppend      : MNetLog("FTP, Failure -- UploadAppend failed"   ); break;
        case cmdReInit      : MNetLog("FTP, Failure -- Reinitialize failed"   ); break;
        case cmdAllocate    : MNetLog("FTP, Failure -- Allocate failed"       ); break;
        case cmdNList       : MNetLog("FTP, Failure -- NList failed"          ); bNListOk=true; break;
        case cmdDoCommand   : MNetLog("FTP, Failure -- DoCommand failed"      ); break;
        case cmdCurrentDir  : MNetLog("FTP, Failure -- CurrentDir failed"     ); break;
    }
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2UnSupportedFunction(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MNetLog("FTP, UnSupported -- ChangeDir not supported by this server"       ); bChangeDirOk=true;break;
        case cmdMakeDir     : MNetLog("FTP, UnSupported -- MakeDir not supported by this server"         ); bChangeDirOk=true;break;
        case cmdDelete      : MNetLog("FTP, UnSupported -- Delete not supported by this server"          ); break;
        case cmdRemoveDir   : MNetLog("FTP, UnSupported -- RemoveDir not supported by this server"       ); break;
        case cmdList        : MNetLog("FTP, UnSupported -- List not supported by this server"            ); bNListOk=true; break;
        case cmdRename      : MNetLog("FTP, UnSupported -- Rename not supported by this server"          ); break;
        case cmdUpRestore   : MNetLog("FTP, UnSupported -- UploadRestore not supported by this server"   ); break;
        case cmdDownRestore : MNetLog("FTP, UnSupported -- DownloadRestore not supported by this server" ); break;
        case cmdDownload    : MNetLog("FTP, UnSupported -- Download not supported by this server"        ); break;
        case cmdUpload      : MNetLog("FTP, UnSupported -- Upload not supported by this server"          ); break;
        case cmdAppend      : MNetLog("FTP, UnSupported -- UploadAppend not supported by this server"    ); break;
        case cmdReInit      : MNetLog("FTP, UnSupported -- Reinitialize not supported by this server"    ); break;
        case cmdAllocate    : MNetLog("FTP, UnSupported -- Allocate not supported by this server"        ); break;
        case cmdNList       : MNetLog("FTP, UnSupported -- NList not supported by this server"           ); bNListOk=true; break;
        case cmdDoCommand   : MNetLog("FTP, UnSupported -- DoCommand not supported by this server"       ); break;
        case cmdCurrentDir  : MNetLog("FTP, UnSupported -- CurrentDir not supported by this server"      ); break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, "+Errmsg+", "+Ptr->Name);
    bNListOk=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Status(TComponent *Sender,
      AnsiString Status)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, "+Status+", "+Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Connect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, FTP Connect, "+Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTP::NMFTP2Disconnect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MNetLog("FTP, FTP Disconnect,"+Ptr->Name);
}
//------------------------------------------------------------------------------
