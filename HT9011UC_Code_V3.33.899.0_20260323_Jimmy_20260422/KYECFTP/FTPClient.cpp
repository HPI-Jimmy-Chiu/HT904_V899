#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "FTPClient.h"

#include "cObserver.h"
#include "main.h"
#include "common.h"
#include "cprod.h"
#include "uLotInfo.h"
#include "cMyDB.h"
#include "Note.h"
#include "mymessbox.h"
#include "BarcodeReader.h"                                                      // 2013.11.29 , Joye , KYEC Barcode Reader  20140103 wei
#include "cmydef.h"
#include "cConfiguration.h"                                                     //20140310 WEI    在FTP頁面顯示 Download 和 Uplaod路徑    KYEC
#include "cMyDB.h"                                                              //ChungHung 20140716 add
#include "cpublic.h"                                                            //kevin 20150415 add
#include "uHGemHT9045.h"
#include "uCleaning.h"
#include "csystem.h"
#include "cBuilder.h"
#include "uESDControl.h"
#include "cBinSel.h"
#include "cShowBinSelect.h"
#include "uLotInfo.h"
#include "ProductionInfo.h"
#include "atester.h"
#include "MyMotor.h"
#include "INPUT.h"
#include "Password.h"
#include "TfFTP.h"
#include "TesterTCP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TfFTPClient *fFTPClient;
TStringList *tmpList;                                                           //Landam
void delay(int ms);
bool bTempList=false;
//bool bError=false;                                                            //KaiChen 20190530 ：改為public  //Landam
bool FTP_DownloadFail=false;                                                    // 2013.12.03 , Joye , KYEC FTP

bool bCanExit=true;                                                             //ChungHung 20150413 add for TSMC
//---------------------------------------------------------------------------
__fastcall TfFTPClient::TfFTPClient(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    bListOk=false;
    bIsFtpRunning=false;
    slDailyTempLog=new TMyStringList("D:\\HT9045_Log\\ChipMosFTP",
                                 IniConfig.SocketHandlerID,
                                 "Time,File Name,Temperature Mode,Temperature,Soak Time,Machine Model,Machine ID,HOTPLATE 1,HOTPLATE 2,SHUTTLE 1,SHUTTLE 2,A1,B1,C1,D1,E1,F1,G1,H1,A2,B2,C2,D2,E2,F2,G2,H2,CHAMBER");     //Steven 20220513 : For ChipMos FTP
    slDailyTempLog->SaveType=TByDay;

    sl10MinTempLog=new TMyStringList("D:\\HT9045_Log\\ChipMosFTP",
                                 IniConfig.SocketHandlerID+"_10Min",
                                 "Time,File Name,Temperature Mode,Temperature,Soak Time,Machine Model,Machine ID,HOTPLATE 1,HOTPLATE 2,SHUTTLE 1,SHUTTLE 2,A1,B1,C1,D1,E1,F1,G1,H1,A2,B2,C2,D2,E2,F2,G2,H2,CHAMBER");     //Steven 20220513 : For ChipMos FTP
}
//---------------------------------------------------------------------------
void delay(int ms)
{
    int tick1=MyTickCount();
    int tick2;

    while(1)
    {
        tick2=MyTickCount();
        Application->ProcessMessages();
        if(tick2<tick1)
            tick1=tick2;
        if(tick2-tick1>=ms)
            break;
    }
}
//---------------------------------------------------------------------------
double dblFileSize(const char* fname)                                           //Ifor 20160907 FTP Download File Size
{
    if(!fname && !*fname)
        return 0.0;
    HANDLE h;
    WIN32_FIND_DATA info;
    if((h=FindFirstFile(fname, &info))!=INVALID_HANDLE_VALUE)
    {
        FindClose(h);
        if((info.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)                 // Is it a file?
        {
            union
            {
                struct {DWORD low, high;} lh;
                __int64 size;                                                   // MS large int extension
            } file;
            file.lh.low = info.nFileSizeLow;
            file.lh.high= info.nFileSizeHigh;
            return file.size;                                                   // will be casted to double
        }
                                                                                // It's a directory, not a file
    }
    return 0.0;                                                                 // No such name.
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::LoadFileFormServer2(AnsiString FtpPath, AnsiString Source)
{
    TNMFTP *NMFTP2;                                                             //Landam 用動態元件 才穩定
    bool bRunOffset=true;
    double dFileSize[3]={0,0,0};                                                //Ifor 20160907 FTP Download File Size
    try
    {
        NMFTP2=new TNMFTP(this);

        //設定參數--------
        NMFTP2->Vendor                  =NMOS_AUTO;
        NMFTP2->TimeOut                 =20000;                                 //Landam
        NMFTP2->Passive                 =true;                                  //Steven 20121020 : 實驗看看
        if(bSigurdDownload_Recipe==true)
        {
            if(IniConfig.FtpHost!="")
                NMFTP2->Host                =FTPAutomation_Down_ServerIP;
            NMFTP2->UserID                  =FTPAutomation_Down_UserID;
            NMFTP2->Password                =FTPAutomation_Down_Password;
        }
        else
        {
            if(IniConfig.FtpHost!="")
                NMFTP2->Host                =IniConfig.FtpHost;
            NMFTP2->UserID                  =IniConfig.FtpUserName;             //edtUserName->Text;
            NMFTP2->Password                =IniConfig.FtpPassword;             //edtPassword->Text;
            NMFTP2->Port                    =StrToInt(IniConfig.N06_FtpPort);   //Ifor 20201015 add:使用者自定義 FTP Port
        }

        //設定Event--------
        NMFTP2->OnListItem              =NMFTP1ListItem;
        NMFTP2->OnSuccess               =NMFTP1Success;
        NMFTP2->OnFailure               =NMFTP1Failure;
        NMFTP2->OnError                 =NMFTP1Error;
        NMFTP2->OnStatus                =NMFTP1Status;
        NMFTP2->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;
        //連線--------
        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            delay(50);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            ShowMyMessage("FTP Server is not connected","");
            delete NMFTP2;
            bError=true;

            FTP_DownloadFail = true;                                            // 2013.12.03 , Joye , KYEC FTP    20140103 wei

            return;
        }

        //下載下來的工作檔要加上_NET結尾------
        AnsiString str01, str02, str03, str04, str05, str06;                    //JerryYang 20190906 ATC工作檔經由handler上傳/下載
        AnsiString sDLFileName=Source;

        if(CUSTOMER_CODE==CC_KYEC_LEE && Source.Pos(asSetupFileCheckList)!=0)
        {
            str01="D:\\HT9045\\system\\"+asSetupFileCheckList;
            str02=Source;
            bRunOffset=false;
        }
        else
        {
            sDLFileName=sDLFileName+"_NET";
            str01=DataPath+sDLFileName+".zip";
            str02=Source+".zip";
            str03=OffsetPath+sDLFileName+".Offset";
            str04=Source+".Offset";                                             //JerryYang 20170727 (Steven) Offset改為offset,檔名要完全一樣否則download會失敗
            str05=DataPath+sDLFileName+"ATC_Recipe"+".zip";                     //JerryYang 20190906 ATC工作檔經由handler上傳/下載
            str06=Source+"ATC_Recipe"+".zip";
            if(CUSTOMER_CODE==CC_ASE_N ||                                       //JerryYang 20170727 (Steven) 日月新不download offset
               IniConfig.bSIGURDFunction ||                                     //KaiChen 20200611 ：矽格 不 download offset
               IniConfig.bVTESTFunction==true ||
               CUSTOMER_CODE==CC_JSCC_OS)                                       //RogerYang 20260127 : JSCC_OS Add
            {
                bRunOffset=false;
            }
        }

        if(FtpPath.SubString(FtpPath.Length(), 1)!="/")
        {
            NMFTP2->ChangeDir(FtpPath + "/");
        }
        else
        {
            NMFTP2->ChangeDir(FtpPath);
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20231221 : 力成反映有跳出 10035 錯誤。
        {

        }
        else
        {
            if(IniConfig.FtpTransMode==0)                                       //Steven 20230719 : 加入FTP傳輸模式
                NMFTP2->Mode(MODE_ASCII);
            else if(IniConfig.FtpTransMode==1)
                NMFTP2->Mode(MODE_IMAGE);
            else
                NMFTP2->Mode(MODE_BYTE);
        }

        for(int i=0; i<3; i++)                                                  //Ifor 20160907 add ftp setup file check size
        {
            delay(200);
            NMFTP2->Download(str02, str01);                                     //下載檔案
            for(int j=0; j<10; j++)
            {
                delay(100);
            }
            dFileSize[i]=dblFileSize(str01.c_str());
            RecordProcess("FTP DownLoad File Size"+IntToStr(i+1)+"_" +FloatToStr(dFileSize[i])+"KB");   //yunghsin 20160906
        }
        bHasFTPDownload=true;                                                   //Ifor 20240422 add:FTP 工作檔下載
        if(CUSTOMER_CODE==CC_AMD_M && iAMD_Function==1)                         //Ifor 20240419 add:上傳密碼相關資料
        {
            try
            {
                delay(200);
                str01="D:\\HT9045\\Error\\English\\JAM0000.dat";
                str02="/JAM0000.dat";
                NMFTP2->Download(str02, str01);
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2 JAM0000.dat");
            }

            try
            {
                delay(200);
                str01="C:\\winnt\\system32\\tech.com";
                str02="/tech.com";
                NMFTP2->Download(str02, str01);
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2 tech.com");
            }

            try
            {
                delay(200);
                str01="d:\\HT9045\\system\\levelset.dat";
                str02="/levelset.dat";
                NMFTP2->Download(str02, str01);
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2 levelset.dat");
            }

            try
            {
                delay(200);
                str01="D:\\HT9045\\system\\unknown.com";
                str02="/unknown.com";
                NMFTP2->Download(str02, str01);
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2 unknown.com");
            }
        }

        if(bRunOffset==true)
        {
            if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                   //ChungHung 20150413 add for TSMC
            {
                fLotInfo->DownloadFromServer_TSMC(Source, sDLFileName);
            }
            else if(CUSTOMER_CODE==CC_Greatek)                                  //Sam 20171019 (wei) : Setup File Download 完成後刪除原本的 Setup File，本機只留一個
            {
                AnsiString strSetupfile=GetLastOpenFN();
                fLotInfo->DownloadFromServer_TSMC(strSetupfile, sDLFileName);
            }
            else
            {
                try
                {
                    delay(200);
                    NMFTP2->Download(str04, str03);                             //jou 2012-12-22 下載 Offset 檔案 start
                    delay(100);
                }
                catch(Exception& e)
                {
                    MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2", e.Message);
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2");
                }

                if(CosFunction.bUseFTPDownLoadATCRecipe==true &&                //Ifor 20191115 : add FTP DownLoad ATC Recipe
                   ATC_SYSTEM==eNewATCSystem &&                                 //JerryYang 20190906 ATC工作檔經由handler上傳/下載
                   Temperature.bATCActiveCooling==true)
                {
                    try
                    {
                        delay(200);
                        NMFTP2->Download(str06, str05);
                        delay(100);
                    }
                    catch(Exception& e)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2", e.Message);
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2");
                    }
                }
                fLotInfo->DownloadFromServer(sDLFileName);
            }
        }
        else if(CUSTOMER_CODE==CC_ASE_N ||                                      //JerryYang 20170727 (Steven) 日月新不download offset
                IniConfig.bSIGURDFunction ||                                    //KaiChen 20200611 ：矽格 不 download offset
                IniConfig.bVTESTFunction==true ||
                CUSTOMER_CODE==CC_JSCC_OS)                                      //RogerYang 20260127 : JSCC_OS Add
        {
            fLotInfo->DownloadFromServer(sDLFileName);
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20231221 : 力成反映有跳出 10035 錯誤。
        {

        }
        else
        {
            NMFTP2->Mode(MODE_ASCII);                                           //Steven 20230719 : 加入FTP傳輸模式
        }
        NMFTP2->Abort();
        delay(100);
        NMFTP2->RequestCloseSocket();
        delay(500);

        FTP_DownloadFail=false;                                                 // 2013.12.03 , Joye , KYEC FTP  20140103 wei
        delete NMFTP2;

        if(dFileSize[0]!=dFileSize[1] ||                                        //Ifor 20160907 add ftp setup file check size
           dFileSize[1]!=dFileSize[2] ||
           dFileSize[2]!=dFileSize[0])
        {
            FTP_DownloadFail=true;
            ShowMyMessage("FTP DownLoad File Size Error");
        }
        else
        {
            RecordProcess("FTP DownLoad File Size Verify Ok!");
        }
    }
    catch(Exception& e)
    {
        MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2", e.Message);
        FTP_DownloadFail=true;                                                  // 2013.12.03 , Joye , KYEC FTP       20140103 wei
        bError=true;
        delete NMFTP2;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::LoadFileFormServer2");
        FTP_DownloadFail=true;                                                  // 2013.12.03 , Joye , KYEC FTP       20140103 wei
        bError=true;
        delete NMFTP2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::UploadFileToServer2(AnsiString FtpPath, AnsiString Source, bool bZip) //ChungHung 20140108 add FTP unload jam code
{
    SetCurrentDirectory(_T("D://"));
    TNMFTP *NMFTP2;                                                             //Landam 用動態元件 才穩定
//    HINSTANCE hInstance;
    AnsiString str, str1, str2, str3;
    AnsiString sULFileName=Source;
    AnsiString ServerDir;
    AnsiString str01, str02;
    AnsiString ActivePath=FileListBox1->Directory+"\\";                         //ChungHung 20150413 add for TSMC

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)
        bZip=true;

    try
    {
        NMFTP2=new  TNMFTP(this);

        //設定參數----
        NMFTP2->Vendor                  =NMOS_AUTO;
        NMFTP2->TimeOut                 =20000;                                 //Landam
        NMFTP2->Passive                 =true;                                  //Steven 20121020 : 實驗看看

        if(bSigurdUpload_Jamcode==true || bSigurdUpload_Recipe==true)           //KaiChen 20190530 ：Sigurd FTP Automation
        {
            if(IniConfig.FtpHost!="")
                NMFTP2->Host            =FTPAutomation_Up_ServerIP;
            NMFTP2->UserID              =FTPAutomation_Up_UserID;
            NMFTP2->Password            =FTPAutomation_Up_Password;
        }
        else
        {
            if(IniConfig.FtpHost!="")
                NMFTP2->Host                =IniConfig.FtpHost;
            NMFTP2->UserID                  =IniConfig.FtpUserName;
            NMFTP2->Password                =IniConfig.FtpPassword;
            NMFTP2->Port                    =StrToInt(IniConfig.N06_FtpPort);   //Ifor 20201015 add:使用者自定義 FTP Port
        }
        //設定Event----
        NMFTP2->OnListItem              =NMFTP1ListItem;
        NMFTP2->OnSuccess               =NMFTP1Success;
        NMFTP2->OnFailure               =NMFTP1Failure;
        NMFTP2->OnError                 =NMFTP1Error;
        NMFTP2->OnStatus                =NMFTP1Status;
        NMFTP2->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        //連線-----
        NMFTP2->Connect();
        if(bZip)                                                                //ChungHung 20140108 add FTP unload jam code
        {
            for(int i=0; i<10; ++i)
            {
                delay(50);
                if(NMFTP2->Connected)
                    break;
            }

            if(!NMFTP2->Connected)
            {
                 ShowMyMessage("FTP Server is not connected","");
                 delete NMFTP2;
                 bError=true;
                 return;
            }
        }
        else                                                                    //ChungHung 20140714 delete 不管有沒有壓縮 都要檢查是否連線
        {
            for(int k=0; k<5; k++)
            {
                for(int i=0; i<10; ++i)
                {
                    delay(50);
                    if(NMFTP2->Connected)
                        break;
                }
                if(NMFTP2->Connected)
                {
                    break;
                }
                else
                {
                    NMFTP2->Abort();
                    MySleep(10);
                    NMFTP2->RequestCloseSocket();
                    MySleep(10);
                    NMFTP2->Connect();
                }
            }

            if(!NMFTP2->Connected)
            {
                 ShowMyMessage("FTP Server is not connected","");
                 NMFTP2->Abort();
                 NMFTP2->RequestCloseSocket();
                 delete NMFTP2;

                 bError=true;
                 return;
            }
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20231221 : 力成反映有跳出 10035 錯誤。
        {

        }
        else
        {
            if(IniConfig.FtpTransMode==0)                                       //Steven 20230719 : 加入FTP傳輸模式
                NMFTP2->Mode(MODE_ASCII);
            else if(IniConfig.FtpTransMode==1)
                NMFTP2->Mode(MODE_IMAGE);
            else
                NMFTP2->Mode(MODE_BYTE);
        }

        if(bZip)                                                                //ChungHung 20140108 add FTP unload jam code
        {
            if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                   //ChungHung 20150413 add for TSMC
            {
                //先刪除舊的壓縮檔
                str.sprintf("del %s%s.zip", ActivePath, sULFileName);           //ChungHung 20150413 add for TSMC
                system(str.c_str());

                //把7Zip準備好
                if(FileExists("d:\\HT9045\\7z.exe")==false)
                {
                    CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , FALSE);
                }

                //把工作檔壓縮
                str.sprintf("d:\\HT9045\\7z.exe a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", ActivePath, sULFileName, ActivePath, sULFileName);   //ChungHung 20150413 add for TSMC
                system(str.c_str());

                //上傳工作檔
                str01=ActivePath+sULFileName+".zip";                            //ChungHung 20150413 add for TSMC
                str02=FtpPath+sULFileName+".zip";
                NMFTP2->Upload(str01, str02);

                str.sprintf("del \"%s%s.zip\"", ActivePath, sULFileName);       //最後要把壓縮檔給刪除。 //ChungHung 20150413 add for TSMC
                system(str.c_str());
            }
            else
            {
                //先刪除舊的壓縮檔
                str.sprintf("del %s%s.zip", DefaultPath, sULFileName);
                system(str.c_str());

                //把7Zip準備好
                if(FileExists("d:\\HT9045\\7z.exe")==false)
                {
                    CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , FALSE);
                }

                fTesterTCP->CopyRecipeFromTester(sULFileName);                  //Steven 20250327 : OS測試機的工作檔也要上傳

                //把工作檔壓縮
                str.sprintf("d:\\HT9045\\7z.exe a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", DataPath, sULFileName, DataPath, sULFileName);
                system(str.c_str());

                //把Offset壓縮
                str.sprintf("d:\\HT9045\\7z.exe a -tzip \"%s%s.Offset\" \"%s%s\\*.*\"", OffsetPath, sULFileName, OffsetPath, sULFileName);
                system(str.c_str());

                //把ATC工作檔壓縮
                if(CosFunction.bUseFTPDownLoadATCRecipe==true &&
                   ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true)                         //JerryYang 20190906 ATC工作檔經由handler上傳/下載  //Ifor 20191115 : add FTP DownLoad ATC Recipe
                {
                    if(DirectoryExists(sATCPath))
                    {
                        str.sprintf("d:\\HT9045\\7z.exe a -tzip \"%s%s.zip\" \"%s\\*%s*\"", DataPath, sULFileName+"ATC_Recipe", sATCPath, sULFileName+"_");
                        system(str.c_str());
                    }
                    else
                    {
                        ShowMyMessage("Upload ATC recipe fail, check below path", sATCPath);
                    }
                }

                //上傳工作檔
                str01=DataPath+sULFileName+".zip";
                str02=FtpPath+sULFileName+".zip";
                NMFTP2->Upload(str01, str02);

                //上傳Offset檔
                str01=OffsetPath+sULFileName+".Offset";
                str02=FtpPath+sULFileName+".Offset";
                NMFTP2->Upload(str01, str02);

                //上傳ATC工作檔
                if(CosFunction.bUseFTPDownLoadATCRecipe==true &&                //JerryYang 20190906 ATC工作檔經由handler上傳/下載
                   ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true)                         //Ifor 20191115 : add FTP DownLoad ATC Recipe
                {
                    str01=DataPath+sULFileName+"ATC_Recipe"+".zip";
                    str02=FtpPath+sULFileName+"ATC_Recipe"+".zip";
                    NMFTP2->Upload(str01, str02);
                }

                if(CUSTOMER_CODE==CC_AMD_M && iAMD_Function==1)
                {
                    try
                    {
                        str01="D:\\HT9045\\Error\\English\\JAM0000.dat";
                        str02="/JAM0000.dat";
                        if(FileExists(str01))
                        {
                            NMFTP2->Upload(str01, str02);
                            RecordProcess("File 1 Upload done");
                        }
                        else
                        {
                            RecordProcess("File 1 Upload Fail");
                        }
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::UploadFileToServer2 JAM0000.dat");
                    }

                    try
                    {
                        str01="C:\\winnt\\system32\\tech.com";
                        str02="/tech.com";
                        if(FileExists(str01))
                        {
                            NMFTP2->Upload(str01, str02);
                            RecordProcess("File 2 Upload done");
                        }
                        else
                        {
                            RecordProcess("File 2 Upload Fail");
                        }
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::UploadFileToServer2 tech.com");
                    }

                    try
                    {
                        str01="d:\\HT9045\\system\\levelset.dat";
                        str02="/levelset.dat";
                        if(FileExists(str01))
                        {
                            NMFTP2->Upload(str01, str02);
                            RecordProcess("File 3 Upload done");
                        }
                        else
                        {
                            RecordProcess("File 3 Upload fail");
                        }
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::UploadFileToServer2 levelset.dat");
                    }

                    try
                    {
                        str01="D:\\HT9045\\system\\unknown.com";
                        str02="/unknown.com";
                        if(FileExists(str01))
                        {
                            NMFTP2->Upload(str01, str02);
                            RecordProcess("File 4 Upload done");
                        }
                        else
                        {
                            RecordProcess("File 4 Upload Fail");
                        }

                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "TfFTPClient::UploadFileToServer2 unknown.com");
                    }
                }

                str.sprintf("del \"%s%s.zip\"", DataPath, sULFileName);         //最後要把壓縮檔給刪除。
                system(str.c_str());

                str.sprintf("del \"%s%s.Offset\"", OffsetPath, sULFileName);    //最後要把壓縮檔給刪除。
                system(str.c_str());

                if(CosFunction.bUseFTPDownLoadATCRecipe==true &&
                   ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true)                         //Ifor 20191115 : add FTP DownLoad ATC Recipe
                {
                    str.sprintf("del \"%s%s.zip\"", DataPath, sULFileName+"ATC_Recipe"); //最後要把壓縮檔給刪除。
                    system(str.c_str());
                }
            }
        }
        else
        {
            if(CUSTOMER_CODE==CC_PTI && Source.Pos("JamAlarmLogTxt")>0)         //Sam 20170518 (wei) 增加路徑檢查 JamAlarmLogTxt //Sam 20170502 (wei) JamAlarm Data 格式修改 //RogerYang 20170417 for 力成 JamAlarm Data 格式
            {
                str01=Source;                                                   //Sam 20170518 (wei) 改為完整路徑
                Source=Source.SubString(30 ,Source.Length());                   //Sam 20170518 (wei) 從完整路徑中取得檔名先刪掉路徑部分 "D:\\HT9045_Log\\JamAlarmLogTxt\\"
                int n =Source.Pos("\\");
                Source=Source.SubString(n+1 ,Source.Length());                  //刪除 Lot ID 路徑 "//LotId"     //RogerYang 20170508 (wei) 修改 n->n+1
            }
            else if(CUSTOMER_CODE==CC_JCET && Source.Pos("_FT")>0)              //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
            {
                int iPos=Source.AnsiPos("_FT");
                AnsiString sLotID=Source.SubString(1, iPos-1);
                AnsiString str03, sCust;

                //檢查目標路徑最後必需為 "/"                                    //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
                if(FtpPath.SubString(FtpPath.Length(),1)!="/")
                {
                    FtpPath=FtpPath+AnsiString("/");
                }
                sCust=sLotID.SubString(0,3);                                    //取前3碼作為客戶代碼資料夾
                str03.sprintf("%s%s/", FtpPath, sCust);

                //檢查目標路徑，第一層 客戶代碼(批次前3碼)
                try
                {
                    NMFTP2->ChangeDir(str03);
                }
                catch(Exception& e)
                {
                    //建立目標路徑
                    NMFTP2->MakeDirectory(str03);
                }
                catch(...)
                {
                    //建立目標路徑
                    NMFTP2->MakeDirectory(str03);
                }

                str03=str03+sLotID+AnsiString("/");                             //最後一個資料夾為批號
                //檢查目標路徑，第二層 sLotID
                try
                {
                    NMFTP2->ChangeDir(str03);
                }
                catch(Exception& e)
                {
                    NMFTP2->MakeDirectory(str03);
                }
                catch(...)
                {
                    NMFTP2->MakeDirectory(str03);
                }
                FtpPath=str03;
                str01.sprintf("%s%s\\%s\\",sWhite2DIDListLoc, sCust, sLotID);
                str01+=Source;
            }
            else if(bSigurdUpload_Jamcode)                                      //KaiChen 20190530 ：Sigurd FTP Automation
            {
                str01="D:\\HT9045_Log\\ErrorBackup\\"+Source;
            }
            else if(bSigurdUpload_Recipe)                                       //KaiChen 20190530 ：Sigurd FTP Automation
            {
                str01="D:\\HT9045_Log\\CheckingList\\"+Source;
            }
            else
            {
                str01="d:\\HandlerTemp\\"+sULFileName;
            }
            str02=FtpPath+Source;
            if(FileExists(str01))                                               //ChungHung 20140704 加入保護
                NMFTP2->Upload(str01, str02);
            else
                ShowMyMessage("Can not find : "+str01);                         //RogerYang 20170502 (Wei) str02->str01
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20231221 : 力成反映有跳出 10035 錯誤。
        {

        }
        else
        {
            NMFTP2->Mode(MODE_ASCII);                                           //Steven 20230719 : 加入FTP傳輸模式
        }

        NMFTP2->Abort();

        NMFTP2->RequestCloseSocket();
        delete NMFTP2;
        if(bZip)                                                                //ChungHung 20140108 add FTP unload jam code
        {
            if(CUSTOMER_CODE==CC_GIGAS)                                         //Isaac 20200723 : 全智要求不要顯示，有log就好
            {
                str01="Upload done: "+sULFileName;
                RecordProcess(str01);                                           //log紀錄 Upload done: JobfileName
            }
            else
            {
                ShowMyMessage("Upload done", "");
            }
        }
        else
        {
            RecordProcess("JAM Code Upload done");
            fNote->bSendJamCodeToFTP=false;                                     //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
        }
    }
    catch(Exception& e)                                                         //Steven 20140505 : 試著抓出連線異常的訊息
    {
        if(bSigurdUpload_Jamcode)                                               //KaiChen 20190530 ：Sigurd FTP Automation
        {
            str="FTP Server is not connected Error No : " + AnsiString(NMFTP2->LastErrorNo);
            NewRecordProcess("Message", str, e.Message);
        }
        else
        {
            ShowMyMessage(e.Message);
            if(!bZip)                                                           //ChungHung 20140108 add FTP unload jam code
            {
                str="FTP Server is not connected Error No : " + AnsiString(NMFTP2->LastErrorNo);
                ShowMyMessage(str, "", e.Message);
            }
        }

        bError=true;
        fNote->bSendJamCodeToFTP=false;                                         //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
        delete NMFTP2;
    }
    catch(...)
    {
        str="FTP Server is not connected Error No : " + AnsiString(NMFTP2->LastErrorNo);
        MyDBIProcess("Exception", str);
        bError=true;
        fNote->bSendJamCodeToFTP=false;                                         //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
        delete NMFTP2;
    }
}
//---------------------------------------------------------------------------
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");//Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::plSLoadClick(TObject *Sender)
{
    if(bControlBySECSGEM==false         &&
       Barcode_Reader(bcSetupFile)==0   &&                                      //ChungHung 20150515 add Control by SECSGEM // 20140103 wei KYEC Barcode Reader
       bControlByGPIB==false)                                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    {
        return;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       HasICUnderMachine() &&
       edtServerWaferName->Text!=fMain->cbSetupFileName->Text)
    {
        ShowMyMessage("Different Setup File Name,Need Finish Clean Out!","檔案名稱不相同，需完成Clean Out!");
        return;
    }

    int iCount=0;
    if(bDownloadFTP==true)
    {
        iErrorBySECSGEM=1;                                                      //ChungHung 20150515 add Control by SECSGEM
        return;
    }
    bDownloadFTP=true;

    AnsiString str=edtServerWaferName->Text+".zip";

    if(bControlBySECSGEM==false && bControlByGPIB==false)                       //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    {
        if(lstServerFile->Items->Count==0)                                      // Landam  選對一定會剩一條record
        {
            if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                    //ChungHung 20150413 add for TSMC
                ShowMyMessage("未選取產品名稱");
            else
                ShowMyMessage("未選擇路徑");

            bDownloadFTP=false;
            return;
        }

        if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                       //wei 20151208 FTP比對lstServerFile全部檔案
        {
            for(int i=0; i<lstServerFile->Items->Count; i++)
            {
                if(edtServerWaferName->Text!=lstServerFile->Items->Strings[i])  // Landam  選對名稱會一樣
                {
                    iCount++;
                    if(iCount>=lstServerFile->Items->Count)
                    {
                        ShowMyMessage("未選取產品名稱");
                        bDownloadFTP=false;
                        return;
                    }
                }
            }

            AnsiString ActiveDataPath="D:\\HT9045\\IniData\\DataFTP";
            if(DirectoryExists(ActiveDataPath)==false)
            {
                SetCurrentDirectory(_T("D://"));
                MyForceDirectories(ActiveDataPath);
                MySleep(50);
            }
        }
        else
        {
            if(edtServerWaferName->Text!=lstServerFile->Items->Strings[0])      // Landam  選對名稱會一樣
            {
                ShowMyMessage("未選擇這路徑");
                bDownloadFTP=false;
                return;
            }
        }
    }
    else
    {
        if(lstServerFile->Items->Count==0)                                      //jou 2015-10-02 Auto Retest GPIB mode
        {
            iErrorBySECSGEM=3;                                                  //JerryYang 20181108 (Steven) ftp download error
            iErrorByGPIB=3;                                                     //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
            bDownloadFTP=false;
            edtServerWaferName->Text="";
            Close();
            return;
        }

        if(edtServerWaferName->Text!=lstServerFile->Items->Strings[0])          // Landam  選對名稱會一樣
        {
            iErrorBySECSGEM=3;                                                  //ChungHung 20150515 add Control by SECSGEM

            bDownloadFTP=false;
            edtServerWaferName->Text="";
            bCanExit==true;                                                     //ChungHung 20150413 add for TSMC
            Close();
            return;
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20160511 Send Site On Off
    {
        bNoSendSiteOnOff=true;
    }

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20200710 : 全智科技，從FTP下載，如果是continue start而且檔案名稱相同，不進行下載
    {
        if((LastSet.iRunStartMode==rsmContinuStart ||
            LastSet.iRunStartMode==rsmContinuRetest) &&
            (edtServerWaferName->Text==fMain->cbSetupFileName->Text))
        {
            ShowMyMessage("Start mode is 'ContinueStart' and the selected filename is the same.\r\nNot download from Server!!");
            bDownloadFTP=false;
            return;
        }
        tmpList->Clear();
        for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
        {
            lstHDFile->Items->Add(fMain->cbSetupFileName->Items->Strings[i]);
            tmpList->Add(fMain->cbSetupFileName->Items->Strings[i]);            //Landam
        }
        edtHDWaferName->Text=fMain->cbSetupFileName->Text;
        MySleep(1000);

        plUnloadClick(this);                                                    //下載前先上傳目前的工作檔
        edtHDWaferName->Text="";
        MySleep(1000);
    }
    iOldRunStartMode=LastSet.iRunStartMode;

    if(bSigurdDownload_Recipe==true)                                            //KaiChen 20190530 ：Sigurd FTP Automation
        LoadFileFormServer2(sRootPath+FTPAutomation_Down_ServerPath, edtServerWaferName->Text);
    else
        LoadFileFormServer2(sRootPath+IniConfig.FtpDownloadPath, edtServerWaferName->Text);

    DownloadPasswordFormServer();                                               //Sam 20210526 : 從 N06 DownloadPath 下載密碼本
    bCanExit==false;                                                            //ChungHung 20150413 add for TSMC
    SYS_SetupFile=edtServerWaferName->Text;                                     //20140103 wei

    if(FTP_DownloadFail==true)                                                  //jou 2014-04-16 ftp download fail fix
    {
        iErrorBySECSGEM       = 3;                                              //ChungHung 20150515 add Control by SECSGEM
        iErrorByGPIB=3;                                                         //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        bFTPDownloadSetupFile = false;
    }
    else
    {
        bFTPDownloadSetupFile = true;
        bInitNeedDownloadFTP=false;                                             //JerryYang 20200416 艾科要求切initial start按start要強制download recipe
        if(bEnablePEModel==false)                                               //Ifor 20160824 add 非PE工程模式底下才可清除旗標
        {
            bHasEnteredPEModel=false;                                           //Ifor 20160823 FTP檔案下載後關閉PE模式判斷旗標
            WriteIniDataGeneral("System", "bHasEnteredPEModel", bHasEnteredPEModel); //Ifor 20160824 避免 PE模式修改後程式被關閉上傳
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       CosFunction.bHiSiliconFunction==true)
    {
        LoadFileFormServer2("/cim_ftp_lft_handler/", asSetupFileCheckList);
    }
    AnsiString sRecord = "Download file : " + DownloadWorkFile_NET;             // 2013.12.10 , Joye , KYEC FTP
    RecordProcess(sRecord.c_str());                                             // 2013.12.10 , Joye , KYEC FTP
    fMain->cbSetupFileName->Clear();
    fMain->LookForFile();

    edtServerWaferName->Enabled=false;                                          //Ifor 20160511 避免下載途中被修改到名稱
    fMain->cbSetupFileName->Text=edtServerWaferName->Text+"_NET";

    AnsiString sDirPath ="";
    sDirPath.sprintf("D:\\HT9045\\IniData\\Data\\%s\\TestMode.Data", fMain->cbSetupFileName->Text);
    if(FileExists(sDirPath))                                                    //Ifor 20161221 (Steven) 判斷FTP檔案是否存在
        bFTPDownLoadHasTestMode=true;
    else
        bFTPDownLoadHasTestMode=false;

    fMain->cbSetupFileNameChange(fMain);
    fLotInfo->btSaveSetupFile->Visible=true;

    fLotInfo->btSaveSetupFile->Click();
    bDownloadFTP=false;
    edtServerWaferName->Text="";

    bCanExit==true;                                                             //ChungHung 20150413 add for TSMC

    if(IniConfig.bEnable_SECS_GEM==true)                                        //wei 20150525 : Secs Gem
    {
        if(bControlBySECSGEM==true)                                             //wei 20150525 SECS GEM FTP 下載
        {
            if(FTP_DownloadFail==true)
                EventReport(SECS_EVENT.DownLoadRecipeByFTPNG);
            else
                EventReport(SECS_EVENT.DownLoadRecipeByFTPOK);
        }
    }

    if(CosFunction.bUseFTPDownloadDataCheck==true &&                            //Ifor 20170727 (wei) add 比對下載資料 by 矽格中興廠
       bFTPDownloadSetupFile==true)                                             //Ifor 20180125 : Use FTP Download Data Check
    {                                                                           //jou 20180130 (Steven) : FTP download check 要code保持在最下面
        // record data
        DownloadWorkFile_NET    = SYS_SetupFile;
        LastSetTemperature_NET  = LastSet.iTemperature;
        TrayForm_NET            = TrayForm;
        HotPlateForm_NET        = HotPlateForm_File;
        Temperature_NET         = Temperature;
        TestIF_NET              = TestIF_File;

//        if(CUSTOMER_CODE==CC_SCC)                                             //Ifor 20181023 add SCC要求FTP資料卡控加入Contact相關資料
        {
            DeviceForm_NET      = DeviceForm_File;
        }

        for(int i=0; i<8; i++)                                                  //ChungHung 20141002 add for KYEC AutoRetest 3->5 //Ifor 20170418 add MRT Mode 5 -> 8
        {
            BinSelect_NET[i]    = BinSelect[i];
        }
    }

    bSetTempChange=true;                                                        //Ifor 20160425 工作檔切換需重新設定ATC參數
    edtServerWaferName->Enabled=true;                                           //Ifor 20160511 避免下載途中被修改到名稱

    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20191110 : 下載後自動切換 Online & Nornal
    {
        LastSet.iTester=ON_LINE;
        fMain->ChangeTesterConnect(LastSet.iTester, false);
        LastSet.iRealDummy=REALLY;
        fMain->UpdateMainOperateMode();
        fMain->LoadRunModePicture();
        fMain->SetStartModeData();
        fCleaning->ResetSmartAutoClean();                                       //Sam 20230111 : Smart Auto Clean
    }

    if(CosFunction.bUseMRTMode==true)                                           //Ifor 20170414 add 下載setup File後，因要顯示MRT模式需新整理Start Mode
        fMain->SetStartModeData();

    if(bSigurdDownload_Recipe==true)                                            //KaiChen 20190530 ：Sigurd FTP Automation
    {
        fLotInfo->GenerateCheckList(false, fMain->cbSetupFileName->Text, false);
        AnsiString sCheckListName;
        sCheckListName.sprintf("%s-down-%04d%02d%02d%02d%02d", fMain->cbSetupFileName->Text, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
        fLotInfo->GenerateCheckList(false, sCheckListName, true);
    }

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20200710 : 全智科技，從FTP下載後要切換到initial start
    {
        SetRunStartMode(rsmInitialStart);
    }

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FormCreate(TObject *Sender)
{
    ProgressBar1->Parent = sbFTPStatus;
    ProgressBar1->Top = 2;
    ProgressBar1->Left = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::plUnloadClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_AMD_M && iAMD_Function==1)                             //Ifor 20240109 add:FTP 上傳需要帳號密碼
    {
        if(DoCheckPassword()==true)
        {
            ShowMyMessage("密碼錯誤不可上傳檔案");
            return;
        }
    }

    if(Barcode_Reader(bcSetupFile)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(bHasEnteredPEModel==true)                                                //Ifor 20160823 進入PE工程模式或者PE工程模式開啟 不可上傳檔案
    {
        if(bEnablePEModel)
            ShowMyMessage("PE engineering model can not upload files\r\nPE工程模式不可上傳檔案");
        else
            ShowMyMessage("Leave PE engineering model must download Setup File first\r\n離開PE工程模式必須先下載Setup File");
        return;
    }

    if(lstHDFile->Items->Count==0)                                              // Landam  選對一定會剩一條record
    {
        if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                       //ChungHung 20150413 add for TSMC
            ShowMyMessage("未選取產品名稱");
        else
            ShowMyMessage("未選擇這路徑");
        return;
    }

    if(edtHDWaferName->Text!=lstHDFile->Items->Strings[0])                      // Landam  選對名稱會一樣
    {
        if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                       //ChungHung 20150413 add for TSMC
            ShowMyMessage("未選取產品名稱");
        else
            ShowMyMessage("未選擇這路徑");
        return;
    }

    plUnload->Enabled=false;

    SetMD5ByFolder(DataPath+edtHDWaferName->Text);                              //Steven 20170927 (wei) : 將工作檔加入檢查碼

    if(bSigurdUpload_Recipe==true)                                              //KaiChen 20190530 ：Sigurd FTP Automation
    {
        AnsiString sName;
        if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                  //Sam 20210428 : 中興杜鴻其要求先切到要上傳的工作檔在產生 Check List
        {
            if(edtHDWaferName->Text!=GetLastOpenFN())                           //修正中興廠 A32 上傳工作檔造成的 Loader 夾 Tray 異常
            {
                RecordProcess("plUnloadClick");
                int imode=fMain->ChangeSetUpFile(edtHDWaferName->Text);
                if(imode==1)
                {
                    fMain->cbSetupFileName->Text=GetLastOpenFN();
                }
                fMain->cbSetupFileName->Text=edtHDWaferName->Text;
            }
        }
        sName.sprintf("%s%04d%02d%02d%02d%02d", edtHDWaferName->Text, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);

        fLotInfo->GenerateCheckList(false, sName, true);
        fLotInfo->GenerateCheckList(false, edtHDWaferName->Text, false);
        AnsiString sCheckListName;
        sCheckListName.sprintf("%s.txt", edtHDWaferName->Text);
        UploadFileToServer2(sRootPath+FTPAutomation_Up_ServerPath, sCheckListName, false);
        UploadFileToServer2(sRootPath+FTPAutomation_Up_ServerPath, edtHDWaferName->Text, true);
    }
    else
    {
        UploadFileToServer2(sRootPath+IniConfig.FtpUplaodPath, edtHDWaferName->Text, true);    //jou 2015-01-16 修正FTP upload error
    }
    plUnload->Enabled=true;
    edtHDWaferName->Text="";
}
//---------------------------------------------------------------------------
TNMFTP *NMFTP3;
void __fastcall TfFTPClient::ShowFTPModal(int HD)
{
    int i=0;

    if(bShow)
    {
        ShowMyMessage("FTP Form already Opened!!");
        iErrorBySECSGEM=1;                                                      //ChungHung 20150515 add Control by SECSGEM
        bControlBySECSGEM=false;                                                //ChungHung 20150515 add Control by SECSGEM
        iErrorByGPIB=1;                                                         //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        bControlByGPIB=false;                                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        return;
    }

    bShow=true;
    bError=false;
    tmpList = new TStringList;
    AnsiString cDir="", Cur="", DirName="";
    iHD=HD;
    switch(HD)
    {
        case 0:                                                                 //Server 要列表,有問題不能用return, 要用break,這樣才能delete NMFTP3
            try
            {
                NMFTP3=new TNMFTP(this);
            }
            catch(Exception& e)
            {
                MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal", e.Message);
                if(NMFTP3!=NULL)
                    delete NMFTP3;
                NMFTP3=NULL;                                                    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                bError=true;
                return;
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal");
                if(NMFTP3!=NULL)
                    delete NMFTP3;
                NMFTP3=NULL;                                                    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                bError=true;
                return;
            }

            PageControl1->ActivePage=TabSheet2;
            TabSheet1->TabVisible=false;
            TabSheet2->TabVisible=true;
            TabSheet3->TabVisible=false;
            TabSheet4->TabVisible=false;
            Panel15->Visible=!bSigurdDownload_Recipe;                           //KaiChen 20190530 ：Sigurd FTP Automation
            lstServerFile->Clear();

            //設定並連線FTP--------------------
            try
            {
                NMFTP3->Vendor                  =NMOS_AUTO;
                NMFTP3->TimeOut                 =20000;                         //Landam
                NMFTP3->Passive                 =true;                          //Steven 20121020 : 實驗看看
                if(bSigurdDownload_Recipe==true)                                //KaiChen 20190530 ：Sigurd FTP Automation
                {
                    if(IniConfig.FtpHost!="")
                        NMFTP3->Host                =FTPAutomation_Down_ServerIP;
                    NMFTP3->UserID                  =FTPAutomation_Down_UserID;
                    NMFTP3->Password                =FTPAutomation_Down_Password;
                }
                else
                {
                    if(IniConfig.FtpHost!="")
                        NMFTP3->Host                =IniConfig.FtpHost;
                    NMFTP3->UserID                  =IniConfig.FtpUserName;
                    NMFTP3->Password                =IniConfig.FtpPassword;
                }
                NMFTP3->ParseList               =true;
                NMFTP3->OnListItem              =NMFTP1ListItem;
                NMFTP3->OnSuccess               =NMFTP1Success;
                NMFTP3->OnFailure               =NMFTP1Failure;
                NMFTP3->OnError                 =NMFTP1Error;
                NMFTP3->OnStatus                =NMFTP1Status;
                NMFTP3->OnConnectionFailed      =NMFTP1ConnectionFailed;
                NMFTP3->OnTransactionStart      =NMFTP1TransactionStart;
                NMFTP3->OnTransactionStop       =NMFTP1TransactionStop;
                NMFTP3->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;
                NMFTP3->Port                    =StrToInt(IniConfig.N06_FtpPort); //Ifor 20201015 add:使用者自定義 FTP Port

                memoFTP->Lines->Add("Process -- Connecting to server-----------");

                try
                {
                    NMFTP3->Connect();
                }
                catch(Exception& e)                                             //Steven 20140505 : 試著抓出連線異常的訊息
                {
                    if(bControlByGPIB==false)                                   //KaiChen 20190530 ：Sigurd FTP Automation
                        ShowMyMessage(e.Message);
                    NMFTP3->Disconnect();
                    NMFTP3->Abort();
                }

                delay(500);

                if(!NMFTP3->Connected)
                {
                    if(bControlByGPIB==false)                                   //KaiChen 20190530 ：Sigurd FTP Automation
                        ShowMyMessage("FTP Server is not connected","");
                    if(NMFTP3!=NULL)
                        delete NMFTP3;
                    NMFTP3=NULL;                                                //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                    bError=true;
                    break;
                }
                memoFTP->Lines->Add("Process -- Connecting to server, OK--------");
                plUnload->Enabled=true;

                //切換路徑-------------------------
                if(sRootPath=="/")
                    sRootPath="";

                // Landam 改變Path路徑 須加 "/"
                if(bSigurdDownload_Recipe==true)                                //KaiChen 20190530 ：Sigurd FTP Automation
                    DirName=sRootPath+FTPAutomation_Down_ServerPath;
                else
                    DirName=sRootPath+IniConfig.FtpDownloadPath;
                cDir.sprintf("%s%s", DirName, "/");
                Cur=NMFTP3->CurrentDir;
                if(Cur!=DirName)
                {
                    try
                    {
                        if(DirName.SubString(DirName.Length(), 1)!="/")
                        {
                            NMFTP3->ChangeDir(cDir);
                        }
                        else
                        {
                            NMFTP3->ChangeDir(DirName);
                        }
                    }
                    catch(Exception& e)                                         //Steven 20140505 : 試著抓出連線異常的訊息
                    {
                        if(bControlByGPIB==false)                               //KaiChen 20190530 ：Sigurd FTP Automation
                            ShowMyMessage(e.Message);
                    }
                }

                delay(300);
                //列表---------------------------
                if(!NMFTP3->Connected)
                {
                    if(bControlByGPIB==false)                                   //KaiChen 20190530 ：Sigurd FTP Automation
                        ShowMyMessage("FTP Server is disconnect","");
                    if(NMFTP3!=NULL)
                        delete NMFTP3;
                    NMFTP3=NULL;                                                //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                    bError=true;
                    break;
                }
                ListBox1->Items->Clear();

                bTempList=false;
                bListOk=false;

                memoFTP->Lines->Add("Process -- List directory and files--------");

                try
                {
                    NMFTP3->Nlist();
                }
                catch(Exception& e)
                {
                    MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal", e.Message);
                    memoFTP->Lines->Add("Error -- List directory and files, Fail--------");
                    bError=true;
                    if(NMFTP3!=NULL)
                        delete NMFTP3;
                    NMFTP3=NULL;                                                //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                    break;
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal");
                    memoFTP->Lines->Add("Error -- List directory and files, Fail--------");
                    bError=true;
                    if(NMFTP3!=NULL)
                        delete NMFTP3;
                    NMFTP3=NULL;                                                //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
                    break;
                }

                i=0;
                do
                {
                    i++;
                    delay(300);
                }
                while(bListOk==false && i<20);

                bTempList=true;
                delay(300);
                if(tmpList->Count==0)                                           //jou 2013-01-22 FTP error show alarm message
                {
                    ShowMyMessage("FTP Error -- List fail! Empty directory!","FTP錯誤，資料夾找不到檔案");  //RogerYang 202601278 : add chinnese
                    memoFTP->Lines->Add("Error -- List fail!");
                    bError=true;
                }
            }
            catch(Exception& e)
            {
                MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal", e.Message);
                if(bControlByGPIB==false)                                       //KaiChen 20190530 ：Sigurd FTP Automation
                    ShowMyMessage("FTP Server is disconnect", "");
                bError=true;
                if(NMFTP3!=NULL)
                    delete NMFTP3;
                NMFTP3=NULL;                                                    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal");
                if(bControlByGPIB==false)                                       //KaiChen 20190530 ：Sigurd FTP Automation
                    ShowMyMessage("FTP Server is disconnect", "");
                bError=true;
                if(NMFTP3!=NULL)
                    delete NMFTP3;
                NMFTP3=NULL;                                                    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            }
            break;
        case 1: //HD
            PageControl1->ActivePage=TabSheet3;
            TabSheet1->TabVisible=false;
            TabSheet2->TabVisible=false;
            TabSheet3->TabVisible=true;
            TabSheet4->TabVisible=false;
            plLoad->Visible=!bSigurdUpload_Recipe;                              //KaiChen 20190530 ：Sigurd FTP Automation
            lstHDFile->Clear();

            if(CUSTOMER_CODE==CC_TSMC_TAINAN ||                                 //ChungHung 20150413 add for TSMC
               CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171019 (wei) : Setup File Download 完成後刪除原本的 Setup File，本機只留一個
            {
                plUnload->Visible=(AccessLevel>=IniConfig.iServerEnable? true : false);

                FileListBox1->Directory="D:\\HT9045\\IniData\\Data\\";
                FileListBox1->FileType>>ftNormal;
                FileListBox1->FileType<<ftDirectory;
                FileListBox1->Update();
                for(int i=0; i<FileListBox1->Items->Count; i++)
                {
                    DirName=FileListBox1->Items->Strings[i];
                    DirName.Delete(1, 1);
                    DirName.Delete(DirName.Length(),1);
                    if(DirName=="." || DirName=="..")
                        continue;
                    lstHDFile->Items->Add(DirName);
                    tmpList->Add(DirName);
                }
            }
            else
            {
                for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
                {
                    lstHDFile->Items->Add(fMain->cbSetupFileName->Items->Strings[i]);
                    tmpList->Add(fMain->cbSetupFileName->Items->Strings[i]);    //Landam
                }
            }
            break;
        case 2: //Taster
            PageControl1->ActivePage=TabSheet4;
            TabSheet1->TabVisible=false;
            TabSheet2->TabVisible=false;
            TabSheet3->TabVisible=false;
            TabSheet4->TabVisible=true;
            if(IniConfig.N06_TasterListFile!="")
            {
                if(FileExists(IniConfig.N06_TasterListFile.c_str()))
                {
                    cbTesterType->Clear();
                    GetTesterType();

                    cbTesterType->Text=IniConfig.TasterType;
                    cbTesterType->ItemIndex=cbTesterType->Items->IndexOf(IniConfig.TasterType);
                    cbTesterTypeChange(this);
                    rgInputMethod->ItemIndex=IniConfig.TasterInputMethod;
                    if(rgInputMethod->ItemIndex==0)
                    {
                        grpTesterMap->Visible=true;
                        grpTesterName->Enabled=false;
                        cbTesterID->Text=IniConfig.TasterNo;
                        cbTesterID->ItemIndex=cbTesterID->Items->IndexOf(IniConfig.TasterNo);
                        cbTesterIDChange(this);
                    }
                    else
                    {
                        grpTesterMap->Visible=false;
                        grpTesterName->Enabled=true;
                        edTesterName->Text=IniConfig.TasterName;
                    }
                    edHandlerType->Text=IniConfig.sMachineType;
                    edHandlerID->Text  =IniConfig.SocketHandlerID;
                }
            }
            break;
    }
    plSLoad->Enabled=true;
    plUnload->Enabled=true;

    if(bError==false)
    {
        iErrorBySECSGEM=0;                                                      //ChungHung 20150515 add Control by SECSGEM
        iErrorByGPIB=0;                                                         //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        if(bControlBySECSGEM==true)                                             //ChungHung 20150515 add Control by SECSGEM
        {
            edtServerWaferName->Text = aSetUpNameBySECSGEM;                     //ChungHung 20150515 add Control by SECSGEM
            plSLoadClick(this);
            bShow=false;
        }
        else if(bControlByGPIB==true)                                           //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        {
            edtServerWaferName->Text=asSetUpNameByGPIB;
            plSLoadClick(this);
            bShow=false;
        }
        else if(fProductionInfo->bAutoDownloadSetupFile==true &&                //Sam 20190801 : AutoDown SetupFile
                CUSTOMER_CODE==CC_Greatek)
        {
            bool bFind=false;
            for(int i=0; i<lstServerFile->Items->Count; i++)
            {
                if(fProductionInfo->sLoadMO_SetupFileName==lstServerFile->Items->Strings[i])
                {
                    bFind=true;
                    break;
                }
            }

            bShow=false;
            if(bFind==true)
            {
                edtServerWaferName->Text=fProductionInfo->sLoadMO_SetupFileName;
                plSLoadClick(this);
                fProductionInfo->bAutoDownloadSetupFile=false;
            }
            else
            {
                Close();
            }
        }
        else
        {
            ShowModal();
        }
    }
    else
    {
        bShow=false;
        Close();
    }
    bControlBySECSGEM=false;                                                    //ChungHung 20150515 add Control by SECSGEM
    bControlByGPIB=false;                                                       //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::plLoadClick(TObject *Sender)
{
    if(Barcode_Reader(bcSetupFile)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(lstHDFile->Items->Count==0)                                              // Landam  選對一定會剩一條record
    {
        if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                       //ChungHung 20150413 add for TSMC
            ShowMyMessage("未選取產品名稱");
        else
            ShowMyMessage("未選擇這路徑");
        return;
    }

    if(edtHDWaferName->Text!=lstHDFile->Items->Strings[0])                      // Landam  選對名稱會一樣
    {
        if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                       //ChungHung 20150413 add for TSMC
            ShowMyMessage("未選取產品名稱");
        else
            ShowMyMessage("未選擇這路徑");
        return;
    }

    bCanExit==false;                                                            //ChungHung 20150413 add for TSMC
    AnsiString Backup,str;
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //ChungHung 20150413 add for TSMC
    {
        if(IniConfig.bEnableFTP)
        {
            Backup="D:\\HT9045\\IniData\\Data\\";
            DataPath="D:\\HT9045\\IniData\\DataFTP\\";
        }
        else
        {
            Backup="D:\\HT9045\\IniData\\DataFTP\\";
            DataPath="D:\\HT9045\\IniData\\Data\\";
        }

        if(DirectoryExists(DataPath)==false)
        {
            SetCurrentDirectory(_T("D://"));
            MyForceDirectories(DataPath);
            MySleep(50);
        }
        if(DirectoryExists(DataPath+edtHDWaferName->Text)==false)
        {
            str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", Backup+edtHDWaferName->Text, DataPath+edtHDWaferName->Text);
            system(str.c_str());
            MySleep(50);
        }
    }

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20200710 : 全智科技，從FTP下載，如果是continue start而且檔案名稱相同，不進行下載
    {
        if(LastSet.iRunStartMode==rsmContinuStart &&
           edtHDWaferName->Text==fMain->cbSetupFileName->Text)
        {
            ShowMyMessage("Test mode is ContinuStart and the selected filename is the same!\r\nNot download from HD");
            return;
        }
    }

    int mode=0;

    RecordProcess("plLoadClick");
    mode=fMain->ChangeSetUpFile(edtHDWaferName->Text);
    if(mode==1)
    {
        fMain->cbSetupFileName->Text=GetLastOpenFN();
    }
    fMain->cbSetupFileName->Text=edtHDWaferName->Text;

    edtHDWaferName->Text="" ;
    bCanExit==true;                                                             //ChungHung 20150413 add for TSMC
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    memoFTP->Lines->Add("Process -- Close-----------");
    memoFTP->Lines->Add("");
    if(memoFTP->Lines->Count>1024)
        memoFTP->Clear();
    bShow=false;                                                                //JerryYang 20200416 : add
    tmpList->Clear();
    delete tmpList;
    //關閉連線-----------------------
    if(iHD==0 && NMFTP3!=NULL)                                                  //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
    {
        if(NMFTP3->Connected)
        {
            try
            {
                NMFTP3->Abort();                                                // Landam 直接 Abort
                if(fFTPClient->Visible==true)
                    MySleep(200);                                                   //Sam 20220722 : 修正 Win10 FTP 死機
                if(NMFTP3->Connected)                                           //是否有連線
                {
                    NMFTP3->RequestCloseSocket();
                    if(fFTPClient->Visible==true)
                        MySleep(300);                                               //Sam 20220722 : 修正 Win10 FTP 死機
                    if(NMFTP3!=NULL)
                        delete NMFTP3;
                }
            }
            catch(Exception& e)
            {
                MyDBIProcess("Exception", "TfFTPClient::FormClose", e.Message);
                if(NMFTP3!=NULL)
                    delete NMFTP3;
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::FormClose");
                if(NMFTP3!=NULL)
                    delete NMFTP3;
            }
        }
    }
    bSigurdUpload_Recipe=false;                                                 //KaiChen 20190530 ：Sigurd FTP Automation
    bSigurdDownload_Recipe=false;                                               //KaiChen 20190530 ：Sigurd FTP Automation
    bHasFTPDownload=false;                                                      //Ifor 20240422 add:FTP 工作檔下載
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::Button3Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FormShow(TObject *Sender)
{
    enter=false;
    fFTPClient->Left=10;
    fFTPClient->Top=100;

    iErrorBySECSGEM=0;          //ChungHung 20150515 add Control by SECSGEM
    bControlBySECSGEM=false;    //ChungHung 20150515 add Control by SECSGEM
    iErrorByGPIB=0;             //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    bControlByGPIB=false;       //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    bDownloadFTP=false;         //wei 20160512 避免Download失敗，下次就無法Download
    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)       //20140310 WEI    在FTP頁面顯示 Download 和 Uplaod路徑     KYEC
    {
        labN06_DownloadPath->Visible    =true;
        labN06_UploadPath->Visible      =true;
        FTP_DownPath->Visible           =true;
        FTP_DownPath->Text              =IniConfig.FtpDownloadPath;
        FTP_UpLdPath->Visible           =true;
        FTP_UpLdPath->Text              =IniConfig.FtpUplaodPath ;

        labN06_DownloadPath1->Visible   =true;                      //wei 20150325  在FTP頁面顯示 Download 和 Uplaod路徑     KYEC
        labN06_UploadPath1->Visible     =true;
        FTP_DownPath1->Visible          =true;
        FTP_DownPath1->Text             =IniConfig.FtpDownloadPath;
        FTP_UpLdPath1->Visible          =true;
        FTP_UpLdPath1->Text             =IniConfig.FtpUplaodPath ;
    }
    else
    {
        labN06_DownloadPath->Visible    =false;
        labN06_UploadPath->Visible      =false;
        FTP_DownPath->Visible           =false;
        FTP_UpLdPath->Visible           =false;

        labN06_DownloadPath1->Visible   =false;                     //wei 20150325  在FTP頁面顯示 Download 和 Uplaod路徑     KYEC
        labN06_UploadPath1->Visible     =false;
        FTP_DownPath1->Visible          =false;
        FTP_UpLdPath1->Visible          =false;
    }

    if(CosFunction.bHiSiliconFunction==true && (CUSTOMER_CODE==CC_JCET || CUSTOMER_CODE==CC_SCC))    //jou 20170921 (Steven) : center要求海思版hontech權限也不能選擇工作檔
    {
        plLoad->Visible=false;
    }

    if(CUSTOMER_CODE==CC_ASE_N) //Steven 20201231 : FTP HD選項 setup file是要保留的，loadfrom HD功能不需要了
    {
        plLoad->Visible=false;
    }

    Panel16->Visible=(CUSTOMER_CODE==CC_TSMC_TAINAN);               //wei 20151208 清除FTP檔案名稱

    if(CUSTOMER_CODE==CC_Greatek)                                   //Sam 20171006 (wei) : 超豐要求不顯示。
    {
        Panel15->Visible =false;
    }
    else if(CUSTOMER_CODE==CC_GIGAS)    //Isaac 20200710 : 全智科技FTP
    {
        if(PageControl1->ActivePageIndex==1)
            edtServerWaferName->SetFocus();
        else
            edtHDWaferName->SetFocus();

        plUnloadALL->Visible=true;  //一鍵上傳的按鈕
    }

    if(CosFunction.bFTPUseBarcodeReader==true)
    {
        if(IniConfig.bN06_UseBarcode==true)
        {
            if(CUSTOMER_CODE==CC_CYUEAN)                                        //Sam 20230706 : 确安科技上傳不需要用 Barcode reader
            {
                lstServerFile->Enabled=false;
                lstHDFile->Enabled=true;
            }
            else
            {
                lstServerFile->Enabled=false;
                lstHDFile->Enabled=false;
            }
        }
        else
        {
            lstServerFile->Enabled=true;
            lstHDFile->Enabled=true;
        }
    }
    edtServerWaferName->Text="";
    edtHDWaferName->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1ListItem(AnsiString Listing)
{
    bool bAdd=true;
    //列出目錄內的檔案-----
    if(bTempList==false)
    {
        if(tmpList==NULL)
            tmpList=new TStringList;

        if(Listing.AnsiPos(".zip")>0 &&
           (Listing.AnsiPos(".Offset")==0 ||
            Listing.AnsiPos("ATC_Recipe.zip")==0))                              //JerryYang 20190906 ATC工作檔經由handler上傳/下載
        {
            if(CUSTOMER_CODE==CC_JSCC_OS &&                                     //RogerYang 20260127 : JSCC_OS 只顯示device對應的工作檔
                Listing.AnsiPos(fLotInfo->sJSCCOSFileName)==0)
            {
                bAdd=false;
            }
            if(bAdd==true)
            {
                lstServerFile->Items->Add(Listing.SubString(1, Listing.Length()-4));
                tmpList->Add(Listing.SubString(1, Listing.Length()-4));             //Landam
            }

        }
    }
    else
    {
        ListBox1->Items->Add(Listing);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::cbTesterTypeChange(TObject *Sender)
{
    bool bAlreadyIn;
    TStrings *sListY=new TStringList();
    TStrings *sListX=new TStringList();

    sListY->LoadFromFile(IniConfig.N06_TasterListFile);

    cbTesterID->Items->Clear();
    cbTasterIp->Items->Clear();
    cbTesterID->Text="";
    cbTasterIp->Text="";

    for(int i=1; i<sListY->Count; i++)
    {
        sListX->CommaText=sListY->Strings[i]; //取得ROW
        bAlreadyIn=false;

        for(int j=0; j<cbTesterID->Items->Count; j++)
        {
            if(sListX->Strings[0]!=cbTesterType->Text || sListX->Strings[1]==cbTesterID->Items->Strings[j])
                bAlreadyIn=true;
        }

        if(bAlreadyIn==false && sListX->Strings[0]==cbTesterType->Text)
        {
            cbTesterID->Items->Add(sListX->Strings[1]);
            cbTasterIp->Items->Add(sListX->Strings[2]);
        }
    }
    sListY->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    sListX->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sListY;
    delete sListX;
    edTesterName->Text=cbTesterType->Text+"-"+cbTesterID->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::btSafeTasterNameClick(TObject *Sender)
{
//    AnsiString sPath=AuthPath+"config.ini";
    IniConfig.TasterInputMethod=rgInputMethod->ItemIndex;
    IniConfig.TasterType=cbTesterType->Text;
    IniConfig.TasterNo=cbTesterID->Text;
    IniConfig.TasterName=edTesterName->Text;

    //Landam 檢查Tester 不存在   IniConfig.TasterName=""
    int ipos=edTesterName->Text.Pos("-");
    if(ipos!=0)                                                                 //無-
    {
        // Tester 為 Type-ID
        AnsiString strType=edTesterName->Text.SubString(1, ipos-1);
        AnsiString strID=edTesterName->Text.SubString(ipos+1, edTesterName->Text.Length());

        if(cbTesterType->Items->Count==0)
        {
            GetTesterType();
            if(cbTesterType->Items->Count==0)                                   //檔案不存在
            {
                edTesterName->Text="";
            }
        }

        if(cbTesterType->Items->Count!=0)                                       //因為上面會重新Load資料,所以這邊不能用else
        {
            int iType=cbTesterType->Items->IndexOf(strType);

            cbTesterType->ItemIndex=iType;

            int iID=cbTesterID->Items->IndexOf(strID);

            if(iType<0 || iID<0)     // Type 存在
            {
                edTesterName->Text="";
            }
        }
    }
    else
    {
        edTesterName->Text="";
    }

    if(edTesterName->Text=="")
        ShowMyMessage("Tester Name 錯誤", "");

    AnsiString str="";
    TStringList *sList=new TStringList();
    sList->Add("Handler Name, Handler Address, Tester Name, Tester Address");

    WSAData   wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);                                       //初始化WINSOCK?用
    char   HostName[80];                                                        //存放本主機名
    LPHOSTENT lpHostEnt;
    gethostname(HostName, sizeof(HostName));                                    //利用得到的主機名去取得主機結構
    lpHostEnt=gethostbyname(HostName);                                          //利用主機名去取主機結構

//    for(int i=0; lpHostEnt->h_addr_list[i]!=0; i++)                           //迴圈取得全部的IP位置
    {
        struct in_addr *p=(struct in_addr *)(lpHostEnt->h_addr_list[0]);
        String IP=inet_ntoa(*p);

        str.sprintf("%s,%s,%s,%s", edHandlerID->Text, IP, edTesterName->Text, cbTasterIp->Text);
        sList->Add(str);
    }
    WSACleanup();

    str=ExtractFileDir(IniConfig.N06_TasterListMap);                            //取出資料夾
    MyForceDirectories(str);                                                    //不存在就建立資料夾
    sList->SaveToFile(IniConfig.N06_TasterListMap);
    sList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sList;
    //--------------------------
    IniConfig.TasterInputMethod =rgInputMethod->ItemIndex;
    IniConfig.TasterType        =cbTesterType->Text;
    IniConfig.TasterNo          =cbTesterID->Text;
    IniConfig.TasterName        =edTesterName->Text;
    SaveTasterInfo();

    str.sprintf("%s is connect with %s", edHandlerID->Text, edTesterName->Text);
    RecordProcess(str);                                                         //Steven 20091004
    if(edTesterName->Text!="")
        Close();                                                                //Landam
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::cbTesterIDChange(TObject *Sender)
{
    cbTasterIp->ItemIndex=cbTesterID->ItemIndex;
    edTesterName->Text=cbTesterType->Text+"-"+cbTesterID->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Success(TCmdType Trans_Type)
{
    AnsiString Str="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : Str="Success -- ChangeDir successful"     ;break;
        case cmdMakeDir     : Str="Success -- MakeDir successful"       ;break;
        case cmdRemoveDir   : Str="Success -- RemoveDir successful"     ;break;
        case cmdDelete      : Str="Success -- Delete successful"        ;break;
        case cmdRename      : Str="Success -- Rename successful"        ;break;
        case cmdReInit      : Str="Success -- Reinitialize successful"  ;break;
        case cmdCurrentDir  : Str="Success -- CurrentDir successful"    ;break;
        case cmdList        : Str="Success -- List successful"          ; bListOk=true; break;
        case cmdNList       : Str="Success -- NList successful"         ; bListOk=true; break;
        case cmdUpRestore   : Str="Success -- UpRestore successful"     ;break;
        case cmdDownRestore : Str="Success -- DownRestore successful"   ;break;
        case cmdDownload    : Str="Success -- Download successful"      ;break;
        case cmdUpload      : Str="Success -- Upload successful"        ;break;
        case cmdAppend      : Str="Success -- Append successful"        ;break;
        case cmdAllocate    : Str="Success -- Allocate successful"      ;break;
        case cmdDoCommand   : Str="Success -- DoCommand successful"     ;break;
    }
    memoFTP->Lines->Add(Str);
    MyDBIProcess("Exception", Str);
    bError=false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1AuthenticationFailed(bool &Handled)
{
    memoFTP->Lines->Add("Authentication Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1TransactionStop(TObject *Sender)
{
    memoFTP->Lines->Add("Data transfer end");
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1TransactionStart(TObject *Sender)
{
    memoFTP->Lines->Add("Data transfer start");
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1ConnectionFailed(TObject *Sender)
{
    memoFTP->Lines->Add("Failure -- Connection Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Failure(bool &Handled, TCmdType Trans_Type)
{
    AnsiString Str="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : Str="Failure -- ChangeDir failed"      ; break;
        case cmdMakeDir     : Str="Failure -- MakeDir failed"        ; break;
        case cmdDelete      : Str="Failure -- Delete failed"         ; break;
        case cmdRemoveDir   : Str="Failure -- RemoveDir failed"      ; break;
        case cmdList        : Str="Failure -- List failed"           ; break;
        case cmdRename      : Str="Failure -- Rename failed"         ; break;
        case cmdUpRestore   : Str="Failure -- UploadRestore failed"  ; break;
        case cmdDownRestore : Str="Failure -- DownloadRestore failed"; break;
        case cmdDownload    : Str="Failure -- Download failed"       ; break;
        case cmdUpload      : Str="Failure -- Upload failed"         ; break;
        case cmdAppend      : Str="Failure -- UploadAppend failed"   ; break;
        case cmdReInit      : Str="Failure -- Reinitialize failed"   ; break;
        case cmdAllocate    : Str="Failure -- Allocate failed"       ; break;
        case cmdNList       : Str="Failure -- NList failed"          ; break;
        case cmdDoCommand   : Str="Failure -- DoCommand failed"      ; break;
        case cmdCurrentDir  : Str="Failure -- CurrentDir failed"     ; break;
    }
    memoFTP->Lines->Add(Str);
    MyDBIProcess("Exception", Str);
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1UnSupportedFunction(TCmdType Trans_Type)
{
    AnsiString Str="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : Str="UnSupported -- ChangeDir not supported by this server"       ; break;
        case cmdMakeDir     : Str="UnSupported -- MakeDir not supported by this server"         ; break;
        case cmdDelete      : Str="UnSupported -- Delete not supported by this server"          ; break;
        case cmdRemoveDir   : Str="UnSupported -- RemoveDir not supported by this server"       ; break;
        case cmdList        : Str="UnSupported -- List not supported by this server"            ; break;
        case cmdRename      : Str="UnSupported -- Rename not supported by this server"          ; break;
        case cmdUpRestore   : Str="UnSupported -- UploadRestore not supported by this server"   ; break;
        case cmdDownRestore : Str="UnSupported -- DownloadRestore not supported by this server" ; break;
        case cmdDownload    : Str="UnSupported -- Download not supported by this server"        ; break;
        case cmdUpload      : Str="UnSupported -- Upload not supported by this server"          ; break;
        case cmdAppend      : Str="UnSupported -- UploadAppend not supported by this server"    ; break;
        case cmdReInit      : Str="UnSupported -- Reinitialize not supported by this server"    ; break;
        case cmdAllocate    : Str="UnSupported -- Allocate not supported by this server"        ; break;
        case cmdNList       : Str="UnSupported -- NList not supported by this server"           ; break;
        case cmdDoCommand   : Str="UnSupported -- DoCommand not supported by this server"       ; break;
        case cmdCurrentDir  : Str="UnSupported -- CurrentDir not supported by this server"      ; break;
    }
    memoFTP->Lines->Add(Str);
    MyDBIProcess("Exception", Str);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::memoFTPDblClick(TObject *Sender)
{
    memoFTP->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::rgInputMethodClick(TObject *Sender)
{
    if(rgInputMethod->ItemIndex==0)
    {
        grpTesterMap->Visible=true;
        grpTesterName->Enabled=false;
        cbTesterID->Text=IniConfig.TasterNo;
        cbTesterID->ItemIndex=cbTesterID->Items->IndexOf(IniConfig.TasterNo);
        cbTesterIDChange(this);
    }
    else
    {
        AnsiString sPath=AuthPath+"config.ini";
        grpTesterMap->Visible=false;
        grpTesterName->Enabled=true;
        edTesterName->Text=CheckAndReadIniData(sPath, "Taster", "Taster Name",  AnsiString(""));
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    ShowMyMessage(Errmsg, "");
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Status(TComponent *Sender,
      AnsiString Status)
{
    ShowMyMessage(Status, "");
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FilterList(TObject *Sender)
{
    TEdit *edit=dynamic_cast <TEdit *>   (Sender);
    AnsiString str1, str2;
    lstHDFile->Clear();
    lstServerFile->Clear();
    str2= edit->Text.UpperCase();
    for(int i=0; i<tmpList->Count; ++i)
    {
        str1=tmpList->Strings[i].UpperCase();
        if(str1==str2 || str2=="")                                  //Sam 20190925 : 修正 FTP 相似檔名無法下載問題
        {
            str1=tmpList->Strings[i];
            lstHDFile->Items->Add(str1);
            lstServerFile->Items->Add(str1);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtHDWaferNameChange(TObject *Sender)
{
    FilterList(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtServerWaferNameChange(TObject *Sender)
{
    FilterList(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::lstServerFileDblClick(TObject *Sender)
{
    int index=lstServerFile->ItemIndex;
    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20200803 : 全智要求不能從選單連點選取工作檔
        return;
    if(index<0)
        return;

    AnsiString str=lstServerFile->Items->Strings[index];
    edtServerWaferName->Text=str;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::lstHDFileDblClick(TObject *Sender)
{
    int index=lstHDFile->ItemIndex;
    if(index<0)
        return;
    edtHDWaferName->Text=lstHDFile->Items->Strings[index];
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::GetTesterType()                                    //Steven 20121018 : 取消不用MDB, 改用直接讀取文字檔
{
    int i, j;
    bool bAlreadyIn;
    TStrings *sListY=new TStringList();
    TStrings *sListX=new TStringList();

    if(FileExists(IniConfig.N06_TasterListFile)==false)
        return

    cbTesterType->Clear();
    cbTesterID  ->Clear();
    cbTasterIp  ->Clear();

    cbTesterType->Text="";
    cbTesterID  ->Text="";
    cbTasterIp  ->Text="";
    edTesterName->Text="";

    sListY->LoadFromFile(IniConfig.N06_TasterListFile); //讀檔

    for(i=1; i<sListY->Count; i++)
    {
        sListX->CommaText=sListY->Strings[i]; //取得ROW
        bAlreadyIn=false;

        for(j=0; j<cbTesterType->Items->Count; j++)
        {
            if(sListX->Strings[0]==cbTesterType->Items->Strings[j])
                bAlreadyIn=true;
        }

        if(bAlreadyIn==false)
        {
            cbTesterType->Items->Add(sListX->Strings[0]);
        }
    }
    sListY->Clear();    //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    sListX->Clear();    //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sListY;
    delete sListX;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtServerWaferNameKeyPress(TObject *Sender,
      char &Key)
{
    if(CosFunction.bFTPUseBarcodeReader==true && IniConfig.bN06_UseBarcode==true)
    {
        Key=NULL;
    }
    else
    {
        if(Key==VK_RETURN && edtServerWaferName->Text.Length()!=0)
        {
            plSLoadClick(this);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtHDWaferNameKeyPress(TObject *Sender,
      char &Key)
{
    if(CosFunction.bFTPUseBarcodeReader==true && IniConfig.bN06_UseBarcode==true)
    {
        Key=NULL;
    }
    else
    {
        if(Key==VK_RETURN && edtHDWaferName->Text.Length()!=0)
        {
            plLoadClick(this);
        }
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfFTPClient::SaveJamCodeFile(AnsiString aHandlerID, TDateTime  tDate, AnsiString aJamCode, AnsiString aMessage)  //ChungHung 20140108 add FTP unload jam code
{
    aJamCode=aJamCode.SubString(4, aJamCode.Length());  //Steven 20140123 : 存JAM CODE不需要前三碼

    AnsiString aDate=FormatDateTime("yyyymmddhhmmss", tDate);
    AnsiString aFileName =aHandlerID + "_" + aDate + "_" + aJamCode + ".txt";
    AnsiString aFilePath = "d:\\HandlerTemp\\" + aFileName;
    int iFileHandle;
    if(DirectoryExists("d:\\HandlerTemp\\"))
    {
        TFileListBox *filelist = new TFileListBox(this);
        filelist->Parent = this;
        filelist->Mask="*.*";
        filelist->Directory="d:\\HandlerTemp\\";
        for(int i=0; i<filelist->Items->Count; i++)
            DeleteFile(filelist->Items->Strings[i]);
        delete filelist;
    }

    MyForceDirectories("d:\\HandlerTemp\\");
    iFileHandle = -1;
    iFileHandle = FileCreate(aFilePath);
    if(iFileHandle!=-1)                                                         //JerryYang 20200325 File Create error的話就不要做
    {                                                                           //2013/12/18,00:32:59,  7,329,Arm2: consecutive failure
        aMessage=FormatDateTime("yyyy/mm/dd,hh:mm:ss", tDate)+"," + aJamCode +"," + aMessage;
        FileWrite(iFileHandle, aMessage.c_str(), aMessage.Length());

        FileClose(iFileHandle);
    }
    else
    {
        RecordProcess("FileCreate error, SaveJamCodeFile");
    }
    return aFileName;
}
//-----------------------------------------------------------------
void __fastcall TfFTPClient::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
     if(bCanExit==false)  //ChungHung 20150413 add for TSMC
        CanClose=false;
}
//---------------------------------------------------------------------------

void __fastcall TfFTPClient::Panel15Click(TObject *Sender)
{
    edtServerWaferName->Text=fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfFTPClient::Panel16Click(TObject *Sender)
{
    edtServerWaferName->Text="";
}
//---------------------------------------------------------------------------
//Sam 20170525 (wei) 力成 add Socket ID Product Data Upload To FTP
//==>
void __fastcall TfFTPClient::UpSocketIdPoductDataToServerByFTP(AnsiString asDirPath, AnsiString sFileName)
{
    //Landam 用動態元件 才穩定
    AnsiString asCSV;
    AnsiString asFileName=asDirPath + "\\" + sFileName;
    TNMFTP *NMFTP2;
    try
    {
        NMFTP2=new TNMFTP(this);
        NMFTP2->Vendor = NMOS_AUTO;

        NMFTP2->TimeOut=20000; //Landam
        if(IniConfig.asN12_FtpHost!="")
            NMFTP2->Host = IniConfig.asN12_FtpHost;
        NMFTP2->UserID = IniConfig.asN12_FtpUserName;
        NMFTP2->Password = IniConfig.asN12_FtpPassword;
        NMFTP2->Port=21;    //Ifor 20200820 add:FTP Port = 70
        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            MySleep(5);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            ShowMyMessage("FTP Server is not connected");
            delete NMFTP2;
            return;
        }

        //上傳檔案
        asCSV.sprintf("%s%s",IniConfig.asN12_FtpUplaodPath, sFileName.c_str());
        NMFTP2->Upload(asFileName, asCSV);
        NMFTP2->Abort();
        NMFTP2->RequestCloseSocket();
        delete NMFTP2;
        //Sam 20170602 (wei) FTP 上傳成功後才清除 SocketID Count
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
                LastSet.iSocketContactCount[i][j]=0;
            }
        }

    }
    catch(Exception& e)
    {
        MyDBIProcess("Exception", "TfFTPClient::UpSocketIdPoductDataToServerByFTP", e.Message);
        DeleteFile(asFileName);                                                 //Sam 20170602 (wei) 將上傳 FTP 失敗的檔案刪除
        ShowMyMessage("Socket ID Product Data Upload To FTP Upload Fail");      //Sam 20170602 (wei) 修改提示
        delete NMFTP2;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::UpSocketIdPoductDataToServerByFTP");
        DeleteFile(asFileName);                                                 //Sam 20170602 (wei) 將上傳 FTP 失敗的檔案刪除
        ShowMyMessage("Socket ID Product Data Upload To FTP Upload Fail");      //Sam 20170602 (wei) 修改提示
        delete NMFTP2;
    }
}
//<==
//---------------------------------------------------------------------------
//Steven 20181224 : For ASE-CL
//==>
//pig 20180928 AddASECLFTP ==>
void TfFTPClient::DownloadWorkFileSettingFormServer()
{
    if(IniConfig.bN22Enable_ASE_CL_FTP==false)
        return;

    if(IniConfig.sN22ASE_CL_FTPHost=="")
    {
        ShowMyMessage("FTP Server Host is Empty");
        return;
    }
    if(IniConfig.sN22ASE_CL_FTPUserName=="")
    {
        ShowMyMessage("FTP Server User Name is Empty");
        return;
    }
    if(IniConfig.sN22ASE_CL_FTPPassword=="")
    {
        ShowMyMessage("FTP Server Download Path is Empty");
        return;
    }
    if(fMain->CheckCanChangeRealDummy()==false)
    {
        ShowMyMessage("Handler Must Clean Out");
        return;
    }
    if(SystemStart || fMain->cbRunStartMode->Enabled==false ||  //JerryYang 20220215 : 機台內有IC不能download
      (LastSet.iRealDummy==REALLY && ((MOT[MMTrayY].fHasTray && MOT[MMTrayY].Tray.HasIC()) || MOT[MMTrayY_Car].fHasTray)))
    {
        ShowMyMessage("Handler Must Clean Out");
        return;
    }

    AnsiString sSetupFile, aLotID, S;
    AnsiString sFTPPath=IniConfig.sN22ASE_CL_FTPDownlaodPath;

    TNMFTP *pFTP;
    pFTP=new TNMFTP(this);

    MyForceDirectories(asASECLFTPPath);

    S=fLotInfo->edtASECL_LotID->Text;
    S=S.Trim();
    aLotID=S;
    sSetupFile.sprintf("%s_%s.txt", aLotID, fMain->cbSetupFileName->Text);

    try
    {
        pFTP->Vendor    = NMOS_AUTO;
        pFTP->TimeOut   = 30000;

        pFTP->Host      = IniConfig.sN22ASE_CL_FTPHost;
        pFTP->UserID    = IniConfig.sN22ASE_CL_FTPUserName;
        pFTP->Password  = IniConfig.sN22ASE_CL_FTPPassword;

        pFTP->Connect();

        for(int i=0; i<300; i++)     // 300 x 10 = 1000 ( 3 Sec )
        {
            MySleep(10);
            if(pFTP->Connected==true)
                break;
        }

        if(pFTP->Connected==false)
        {
            ShowMyMessage("FTP Server is not connected");
            delete pFTP;
            return;
        }

        //pFTP->Download(sFTPPath+aLotID+"\\"+sSetupFile, asASECLFTPPath+sSetupFile);
        pFTP->Download(sFTPPath+aLotID+"\\"+sSetupFile, asASECLFTPPath+"\\"+sSetupFile); //KaiChen 20200316 ：修正
        pFTP->Abort();
        pFTP->RequestCloseSocket();

        //DoUpdateSetupFileData(asASECLFTPPath+sSetupFile);
        DoUpdateSetupFileData(asASECLFTPPath+"\\"+sSetupFile); //KaiChen 20200316 ：修正
    }
    catch(Exception& e)
    {
        MyDBIProcess("Exception", "TfFTPClient::DownloadWorkFileSettingFormServer", e.Message);
        ESDForm->aDebugLog.printf("%s %s --> %s [ %6d ]", __DATE__, __TIME__, __FUNC__, __LINE__);
        ESDForm->WriteLogs(ESDForm->aDebugLog);
        delete pFTP;
        ShowMyMessage("FTP Download Fail");
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::DownloadWorkFileSettingFormServer");
        ESDForm->aDebugLog.printf("%s %s --> %s [ %6d ]", __DATE__, __TIME__, __FUNC__, __LINE__);
        ESDForm->WriteLogs(ESDForm->aDebugLog);
        delete pFTP;
        ShowMyMessage("FTP Download Fail");
    }

    delete pFTP;
}
//------------------------------------------------------------------------------
void TfFTPClient::DoUpdateSetupFileData(AnsiString FileName)
{
    AnsiString sMap[4][8];
    int iBin, ret, iMaxErrTray=0;
    int tag=FT;
    double dSitePitchX, dSitePitchY;

    AnsiString S="", aBin="", aPF="", sSiteLayout="",sSitePitch, sSitePitchX="", sSitePitchY="", sSiteMap="", sConFail="", sbConFail="", siConFail="", sTemp="", sbACC="", siACC="";
    AnsiString szDir="", szDirTarget="", TempFile, BinFile, szSetup, szYieldFile;
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;
    bool bHasData[7]={false, false, false, false, false, false, false};
    AnsiString sCheckData[7]={"[SITE LAYOUT]", "[SITE PITCH]", "[SITE MAPPING CODE]", "I/F Error Tray", "Default", "Soak Time", "Bin"};

    char *cSiteMap[32];

    if(SystemStart || fMain->cbRunStartMode->Enabled==false ||  //JerryYang 20220215 : 機台內有IC不能download
      (LastSet.iRealDummy==REALLY && ((MOT[MMTrayY].fHasTray && MOT[MMTrayY].Tray.HasIC()) || MOT[MMTrayY_Car].fHasTray)))
        return;

    bFTPDownlodFinish=false;

    S=GetLastOpenFN();
    szDir.sprintf("%s%s", DataPath, S);

    MyForceDirectories(szDir, "TfFTPClient::DoUpdateSetupFileData");
    TempFile=szDir+"\\Temperature.Data";
    BinFile=szDir+"\\Binasgn.Data";
    szSetup=szDir+"\\HandlerCondition.Data";
    szYieldFile=szDir+"\\Tester.Data";

    TStringList *MyList=new TStringList();
    TStringList *sBinTraySetT3Pos=new TStringList();
    MyList->LoadFromFile(FileName.c_str());

    fMain->edWorkTemperBase->Text  =ReadIniData(FileName, "Temperature", "Default", AnsiString("25"));
    fMain->edSoakTime->Text        =ReadIniData(FileName, "Temperature", "Soak Time", AnsiString("0"));
    if(atof(fMain->edWorkTemperBase->Text.c_str())>25)
    {
        fMain->ChangeTempMode(1, false, true);
    }
    else
    {
        fMain->ChangeTempMode(0, false, true);
    }
    fMain->SetTemp(false, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));

    WriteIniData(TempFile, "Mode",       "Temperature", FormatFloat("0.0", fMain->edWorkTemperBase->Text.ToDouble()));
    WriteIniData(TempFile, "Time",       "Soak",        FormatFloat("0.0", fMain->edSoakTime->Text.ToDouble()));

    BinSelect[tag].IfErrorT3=ReadIniData(FileName, "Bin (FT)", "I/F Error Tray", eFix2);

    if(CosFunction.bUseTrayUpDownSet && TrayForm.iFixTrayMode==1)
    {
        iMaxErrTray=iFixRightHalf;
    }
    else
    {
        iMaxErrTray=iFixRight;
    }

    if(BinSelect[tag].IfErrorT3<0 || BinSelect[tag].IfErrorT3>iMaxErrTray)    //Auto1=0, Auto2=1,Auto3=2,Fix1=3,Fix2=4,Fix3=5
    {
        ShowMyMessage("Auto download fail: Error tray setting over limit.","");
        return;
    }

    WriteIniData(BinFile, "I/F Error", "Bin", BinSelect[tag].IfErrorT3);

    for(int i=0; i<iTestBinCount; i++)
    {
        BinSelect[tag].iCatDataT3Pos[i]=ePosNoUse;
    }

    for(int i=0; i<MyList->Count; i++)
    {
        S=MyList->Strings[i];
        if(S=="[SITE LAYOUT]")
        {
            sSiteLayout=MyList->Strings[i+1];
            sSiteLayout=sSiteLayout.UpperCase();
            bHasData[0]=true;
        }
        else if(S=="[SITE PITCH]")
        {
            sSitePitch=MyList->Strings[i+1];
            bHasData[1]=true;
        }
        else if(S=="[SITE MAPPING CODE]")
        {
            sSiteMap=MyList->Strings[i+1];
            bHasData[2]=true;
        }
        else if(S=="[AUTO CLEAN]")
        {
            sTemp=MyList->Strings[i+1];
            if(sTemp.Pos(";")!=0)
            {
                sbACC=sTemp.SubString(1, 1);
                siACC=sTemp.SubString(sTemp.Pos(";")+1, sTemp.Length()-sTemp.Pos(";"));
            }
        }
        else if(S=="[CONTINUOUS FAIL]")
        {
            sConFail=MyList->Strings[i+1];
            if(sConFail.Pos(";")!=0)
            {
                sbConFail=sConFail.SubString(1, 1);
                siConFail=sConFail.SubString(sConFail.Pos(";")+1, sConFail.Length()-sConFail.Pos(";"));
            }
        }
        else if(S.Pos("I/F Error Tray")!=0)
        {
            bHasData[3]=true;
        }
        else if(S.Pos("Default")!=0)
        {
            bHasData[4]=true;
        }
        else if(S.Pos("Soak Time")!=0)
        {
            bHasData[5]=true;
        }
        else
        {
            for(int j=0; j<eTrayCount; j++)
            {
                if(S.Pos(s6TrayName[j])!=0)
                {
                    bHasData[6]=true;
                    aBin=S.SubString(1, S.Pos("="));
                    iBin=atoi(aBin.c_str());
                    if(iBin>=iTestBinCount || iBin<0)
                    {
                        ShowMyMessage("Bin setting over max bin","");
                        return;
                    }
                    aPF=S.SubString(S.Length(), 1);
                    if(aPF!="0" && aPF!="1")
                    {
                        ShowMyMessage("Bin setting pass/fail error","");
                        return;
                    }
                    BinSelect[tag].iCatDataT3Pos[iBin]=j+1;
                    BinSelect[tag].iStackDefFailCate[j]=atoi(aPF.c_str());
                    WriteIniData(BinFile, s6TrayName[j], "Pass/Fail", atoi(aPF.c_str()));
                    break;
                }
            }
        }
    }

    for(int i=0; i<7; i++)
    {
        if(bHasData[i]==false)
        {
            AnsiString strData;
            strData.sprintf("Setup file缺少%s資料", sCheckData[i]);
            ShowMyMessage(strData);
            return;
        }
    }

    //Site Pitch
    //<==
    if(sSitePitch.Pos("X")>0)
    {
        sSitePitchX=sSitePitch.SubString(1, sSitePitch.Pos("X")-1);
        sSitePitchY=sSitePitch.SubString(sSitePitch.Pos("X")+1, (sSitePitch.Length()-sSitePitch.Pos("X")+1));

        dSitePitchX=StrToFloatDef(sSitePitchX, -1);
        dSitePitchY=StrToFloatDef(sSitePitchY, -1);
        if(dSitePitchX<0 || dSitePitchY<0)
        {
            ShowMyMessage("Auto download fail: Site pitch Error","");
            return;
        }
    }
    else if(sSitePitch.Pos("x")>0)
    {
        sSitePitchX=sSitePitch.SubString(1, sSitePitch.Pos("x")-1);
        sSitePitchY=sSitePitch.SubString(sSitePitch.Pos("x")+1, (sSitePitch.Length()-sSitePitch.Pos("x")+1));

        dSitePitchX=StrToFloatDef(sSitePitchX, -1);
        dSitePitchY=StrToFloatDef(sSitePitchY, -1);
        if(dSitePitchX<0 || dSitePitchY<0)
        {
            ShowMyMessage("Auto download fail: Site pitch Error","");
            return;
        }
    }
    else if(sSitePitch.Pos(";")>0)
    {
        sSitePitchX=sSitePitch.SubString(1, sSitePitch.Pos(";")-1);
        sSitePitchY=sSitePitch.SubString(sSitePitch.Pos(";")+1, (sSitePitch.Length()-sSitePitch.Pos(";")+1));

        dSitePitchX=StrToFloatDef(sSitePitchX, -1);
        dSitePitchY=StrToFloatDef(sSitePitchY, -1);
        if(dSitePitchX<0 || dSitePitchY<0)
        {
            ShowMyMessage("Auto download fail: Site pitch Error","");
            return;
        }
    }
    else
    {
        sSitePitchX=sSitePitch;
        dSitePitchX=StrToFloatDef(sSitePitchX, -1);
        dSitePitchY=0;
        if(dSitePitchX<0)
        {
            ShowMyMessage("Auto download fail: Site pitch Error","");
            return;
        }
    }
    //<==
    //Site Pitch
    AnsiString str;

    if(sSiteLayout=="1" || sSiteLayout=="1X1")
    {
        iSiteCnt=1;
    }
    else if(sSiteLayout=="2" || sSiteLayout=="2X1")
    {
        iSiteCnt=2;
    }
    else if(sSiteLayout=="3X1")
    {
        iSiteCnt=3;
    }
    else if(sSiteLayout=="2X2")
    {
        iSiteCnt=4;
    }
    else if(sSiteLayout=="4X1")
    {
        iSiteCnt=4;
    }
    else if(sSiteLayout=="4X2")
    {
        iSiteCnt=8;
    }
    else if(sSiteLayout=="6X2")
    {
        iSiteCnt=12;
    }
    else if(sSiteLayout=="8X2")
    {
        iSiteCnt=16;
    }
    else
    {
        ShowMyMessage("Auto download fail: SiteLayout error","");
        return; //error
    }

    //Site Map
    //<==
    int iCount=0;
    if(sSiteMap.Pos("-")!=0)
    {
        char str[200];
        memset(str, '\0',200);
        strncpy(str, sSiteMap.c_str(), sizeof(str));
        char *temp=strtok(str, "-");

        while (temp!=NULL)
        {
            cSiteMap[iCount]=temp;

            temp = strtok(NULL, "-");
            if(temp!=NULL)
            {
                iCount++;
                if(iCount>=iSiteCnt)
                {
                    //資料異常
                    ShowMyMessage("Auto download fail: Site count over limit","");
                    return ;   //Parameter Error
                }
            }
        }
        if(iCount<iSiteCnt-1)
        {
            //資料異常
            ShowMyMessage("Auto download fail: Site count error","");
            return ;   //Parameter Error
        }

    }
    int iSiteMap[32];

    for(int i=0; i<iSiteCnt; i++)   //JerryYang 20160113 Sitemap防護
    {
        iSiteMap[i]=atoi(cSiteMap[i]);
    }

    for(int i=0; i<iSiteCnt; i++)   //JerryYang 20160113 Sitemap防護
    {
        if(iSiteMap[i]>iSiteCnt || iSiteMap[i]<0)
        {
            ShowMyMessage("Auto download fail: Site Number over limit","");
            return;  //Parameter Error
        }
        for(int j=i+1;j<iSiteCnt;j++)
        {
            if(iSiteMap[i]==iSiteMap[j] && iSiteMap[i]!=0)
            {
                ShowMyMessage("Auto download fail: Site number same error","");
                return;  //Parameter Error
            }
        }
    }
    if(sSiteLayout=="1" || sSiteLayout=="1X1")
    {
        iSiteCnt=1;

        if(TestIF_File.iTestMode!=SingleSite || TestIF_File.iSiteMap[0][0]!=iSiteMap[0])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=SingleSite;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=SingleSite;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        }
    }
    else if(sSiteLayout=="2" || sSiteLayout=="2X1")
    {
        iSiteCnt=2;
        if(TestIF_File.iTestMode!=DualSite || TestIF_File.iSiteMap[0][0]!=iSiteMap[0] || TestIF_File.iSiteMap[0][1]!=iSiteMap[1])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=DualSite;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=DualSite;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        }
    }
    else if(sSiteLayout=="3X1")
    {
        iSiteCnt=3;
        if(TestIF_File.iTestMode!=TriSite1X3 || TestIF_File.iSiteMap[0][0]!=iSiteMap[0] ||
           TestIF_File.iSiteMap[0][1]!=iSiteMap[1] || TestIF_File.iSiteMap[0][2]!=iSiteMap[2])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=TriSite1X3;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[0][2]=iSiteMap[2];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=TriSite1X3;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[0][2]=iSiteMap[2];
        }
    }
    else if(sSiteLayout=="2X2")
    {
        iSiteCnt=4;
        if(TestIF_File.iTestMode!=QualSite2X2 || TestIF_File.iSiteMap[0][0]!=iSiteMap[0] || TestIF_File.iSiteMap[0][1]!=iSiteMap[1] ||
           TestIF_File.iSiteMap[1][0]!=iSiteMap[2] || TestIF_File.iSiteMap[1][1]!=iSiteMap[3])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=QualSite2X2;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[1][0]=iSiteMap[2];
                    TestIF_File.iSiteMap[1][1]=iSiteMap[3];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=QualSite2X2;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[1][0]=iSiteMap[2];
            TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        }
    }
    else if(sSiteLayout=="4X1")
    {
        iSiteCnt=4;
        if(TestIF_File.iTestMode!=QualSite1X4 || TestIF_File.iSiteMap[0][0]!=iSiteMap[0] || TestIF_File.iSiteMap[0][1]!=iSiteMap[1] ||
           TestIF_File.iSiteMap[0][2]!=iSiteMap[2] || TestIF_File.iSiteMap[0][3]!=iSiteMap[3])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=QualSite1X4;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[0][2]=iSiteMap[2];
                    TestIF_File.iSiteMap[0][3]=iSiteMap[3];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=QualSite1X4;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[0][2]=iSiteMap[2];
            TestIF_File.iSiteMap[0][3]=iSiteMap[3];
        }
    }
    else if(sSiteLayout=="4X2")
    {
        iSiteCnt=8;
        if(TestIF_File.iTestMode!=_8Site2X4 ||
           TestIF_File.iSiteMap[0][0]!=iSiteMap[0] || TestIF_File.iSiteMap[0][1]!=iSiteMap[1] || TestIF_File.iSiteMap[0][2]!=iSiteMap[2] || TestIF_File.iSiteMap[0][3]!=iSiteMap[3] ||
           TestIF_File.iSiteMap[1][0]!=iSiteMap[4] || TestIF_File.iSiteMap[1][1]!=iSiteMap[5] || TestIF_File.iSiteMap[1][2]!=iSiteMap[6] || TestIF_File.iSiteMap[1][3]!=iSiteMap[7])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=_8Site2X4;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[0][2]=iSiteMap[2];
                    TestIF_File.iSiteMap[0][3]=iSiteMap[3];
                    TestIF_File.iSiteMap[1][0]=iSiteMap[4];
                    TestIF_File.iSiteMap[1][1]=iSiteMap[5];
                    TestIF_File.iSiteMap[1][2]=iSiteMap[6];
                    TestIF_File.iSiteMap[1][3]=iSiteMap[7];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=_8Site2X4;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[0][2]=iSiteMap[2];
            TestIF_File.iSiteMap[0][3]=iSiteMap[3];
            TestIF_File.iSiteMap[1][0]=iSiteMap[4];
            TestIF_File.iSiteMap[1][1]=iSiteMap[5];
            TestIF_File.iSiteMap[1][2]=iSiteMap[6];
            TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        }
    }
    else if(sSiteLayout=="6X2")
    {
        iSiteCnt=12;
        if(TestIF_File.iTestMode!=_12Site2X6 ||
            TestIF_File.iSiteMap[0][0]!=iSiteMap[0] ||
            TestIF_File.iSiteMap[0][1]!=iSiteMap[1] ||
            TestIF_File.iSiteMap[0][2]!=iSiteMap[2] ||
            TestIF_File.iSiteMap[0][3]!=iSiteMap[3] ||
            TestIF_File.iSiteMap[0][4]!=iSiteMap[4] ||
            TestIF_File.iSiteMap[0][5]!=iSiteMap[5] ||
            TestIF_File.iSiteMap[1][0]!=iSiteMap[6] ||
            TestIF_File.iSiteMap[1][1]!=iSiteMap[7] ||
            TestIF_File.iSiteMap[1][2]!=iSiteMap[8] ||
            TestIF_File.iSiteMap[1][3]!=iSiteMap[9] ||
            TestIF_File.iSiteMap[1][4]!=iSiteMap[10] ||
            TestIF_File.iSiteMap[1][5]!=iSiteMap[11])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=_12Site2X6;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[0][2]=iSiteMap[2];
                    TestIF_File.iSiteMap[0][3]=iSiteMap[3];
                    TestIF_File.iSiteMap[0][4]=iSiteMap[4];
                    TestIF_File.iSiteMap[0][5]=iSiteMap[5];
                    TestIF_File.iSiteMap[1][0]=iSiteMap[6];
                    TestIF_File.iSiteMap[1][1]=iSiteMap[7];
                    TestIF_File.iSiteMap[1][2]=iSiteMap[8];
                    TestIF_File.iSiteMap[1][3]=iSiteMap[9];
                    TestIF_File.iSiteMap[1][4]=iSiteMap[10];
                    TestIF_File.iSiteMap[1][5]=iSiteMap[11];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=_12Site2X6;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[0][2]=iSiteMap[2];
            TestIF_File.iSiteMap[0][3]=iSiteMap[3];
            TestIF_File.iSiteMap[0][4]=iSiteMap[4];
            TestIF_File.iSiteMap[0][5]=iSiteMap[5];
            TestIF_File.iSiteMap[1][0]=iSiteMap[6];
            TestIF_File.iSiteMap[1][1]=iSiteMap[7];
            TestIF_File.iSiteMap[1][2]=iSiteMap[8];
            TestIF_File.iSiteMap[1][3]=iSiteMap[9];
            TestIF_File.iSiteMap[1][4]=iSiteMap[10];
            TestIF_File.iSiteMap[1][5]=iSiteMap[11];
        }
    }
    else if(sSiteLayout=="8X2")
    {
        iSiteCnt=16;
        if(TestIF_File.iTestMode!=_16Site2X8 ||
            TestIF_File.iSiteMap[0][0]!=iSiteMap[0] ||
            TestIF_File.iSiteMap[0][1]!=iSiteMap[1] ||
            TestIF_File.iSiteMap[0][2]!=iSiteMap[2] ||
            TestIF_File.iSiteMap[0][3]!=iSiteMap[3] ||
            TestIF_File.iSiteMap[0][4]!=iSiteMap[4] ||
            TestIF_File.iSiteMap[0][5]!=iSiteMap[5] ||
            TestIF_File.iSiteMap[0][6]!=iSiteMap[6] ||
            TestIF_File.iSiteMap[0][7]!=iSiteMap[7] ||
            TestIF_File.iSiteMap[1][0]!=iSiteMap[8] ||
            TestIF_File.iSiteMap[1][1]!=iSiteMap[9] ||
            TestIF_File.iSiteMap[1][2]!=iSiteMap[10] ||
            TestIF_File.iSiteMap[1][3]!=iSiteMap[11] ||
            TestIF_File.iSiteMap[1][4]!=iSiteMap[12] ||
            TestIF_File.iSiteMap[1][5]!=iSiteMap[13] ||
            TestIF_File.iSiteMap[1][6]!=iSiteMap[14] ||
            TestIF_File.iSiteMap[1][7]!=iSiteMap[15])
        {
            ret=ShowMyMessageBox_YES_NO("Are you sure to download different site mapping?", "要下載的Site map與目前不同,確定要下載?");
            if(ret==2)
            {
                return;
            }
            else
            {
                if(DoPassword()==false)
                {
                    ShowMyMessage("password error or login level reject", "");
                    return;
                }
                else
                {
                    TestIF_File.iTestMode=_16Site2X8;
                    TestIF_File.iSiteMap[0][0]=iSiteMap[0];
                    TestIF_File.iSiteMap[0][1]=iSiteMap[1];
                    TestIF_File.iSiteMap[0][2]=iSiteMap[2];
                    TestIF_File.iSiteMap[0][3]=iSiteMap[3];
                    TestIF_File.iSiteMap[0][4]=iSiteMap[4];
                    TestIF_File.iSiteMap[0][5]=iSiteMap[5];
                    TestIF_File.iSiteMap[0][6]=iSiteMap[6];
                    TestIF_File.iSiteMap[0][7]=iSiteMap[7];
                    TestIF_File.iSiteMap[1][0]=iSiteMap[8];
                    TestIF_File.iSiteMap[1][1]=iSiteMap[9];
                    TestIF_File.iSiteMap[1][2]=iSiteMap[10];
                    TestIF_File.iSiteMap[1][3]=iSiteMap[11];
                    TestIF_File.iSiteMap[1][4]=iSiteMap[12];
                    TestIF_File.iSiteMap[1][5]=iSiteMap[13];
                    TestIF_File.iSiteMap[1][6]=iSiteMap[14];
                    TestIF_File.iSiteMap[1][7]=iSiteMap[15];
                }
            }
        }
        else
        {
            TestIF_File.iTestMode=_16Site2X8;
            TestIF_File.iSiteMap[0][0]=iSiteMap[0];
            TestIF_File.iSiteMap[0][1]=iSiteMap[1];
            TestIF_File.iSiteMap[0][2]=iSiteMap[2];
            TestIF_File.iSiteMap[0][3]=iSiteMap[3];
            TestIF_File.iSiteMap[0][4]=iSiteMap[4];
            TestIF_File.iSiteMap[0][5]=iSiteMap[5];
            TestIF_File.iSiteMap[0][6]=iSiteMap[6];
            TestIF_File.iSiteMap[0][7]=iSiteMap[7];
            TestIF_File.iSiteMap[1][0]=iSiteMap[8];
            TestIF_File.iSiteMap[1][1]=iSiteMap[9];
            TestIF_File.iSiteMap[1][2]=iSiteMap[10];
            TestIF_File.iSiteMap[1][3]=iSiteMap[11];
            TestIF_File.iSiteMap[1][4]=iSiteMap[12];
            TestIF_File.iSiteMap[1][5]=iSiteMap[13];
            TestIF_File.iSiteMap[1][6]=iSiteMap[14];
            TestIF_File.iSiteMap[1][7]=iSiteMap[15];
        }
    }
    else
    {
        ShowMyMessage("Auto download fail: Site Layout error","");
        return; //error
    }
    //<==
    //Site Map

    if(dSitePitchX>0)
    {
        TestIF_File.dSiteXPitch=dSitePitchX;
    }
    if(dSitePitchY>0)
    {
        TestIF_File.dSiteYPitch=dSitePitchY;
    }
    bGPIBChangeSiteMap=true;
    fSetup->ScrollBar1Change(this);
    fSetup->DoIniDataToForm();
    fSetup->sbUpdateClick(this);

    //Bin
    //==<
    for(int i=0; i<iTestBinCount; i++)
    {
        sBinTraySetT3Pos->Add(BinSelect[tag].iCatDataT3Pos[i]);
    }
    WriteIniData(BinFile, "Bin Func FT", "3617 BinTraySetting", sBinTraySetT3Pos->CommaText);
    //<==
    //Bin

    //Continous fail
    //<==
    if(sbConFail=="Y")
    {
        WriteIniData(szYieldFile, "Alarm",     "SocketEnable",    int(1));
        WriteIniData(szYieldFile, "Alarm",     "HeadEnable",      int(1));
    }
    else if(sbConFail=="N")
    {
        WriteIniData(szYieldFile, "Alarm",     "SocketEnable",    int(0));
        WriteIniData(szYieldFile, "Alarm",     "HeadEnable",      int(0));
    }

    int iTemp;
    iTemp=StrToIntDef(siConFail, -1);
    if(iTemp>0)
    {
        WriteIniData(szYieldFile, "Alarm",     "SocketCT",        iTemp);
        WriteIniData(szYieldFile, "Alarm",     "HeadCT",          iTemp);
    }
    //<==
    //Continous fail

    //Auto clean
    //==>
    fCleaning->LoadAutoCleanData();
    if(sbACC=="Y")
    {
        fCleaning->chkAutoCleanMode6->Checked=true;
    }
    else if(sbACC=="N")
    {
        fCleaning->chkAutoCleanMode6->Checked=false;
    }

    iTemp=StrToIntDef(siACC, -1);
    if(iTemp>0)
    {
        fCleaning->edIntervalContact->Text=iTemp;
    }
    fCleaning->SaveAutoCleanData();
    fCleaning->LoadAutoCleanData();
    //<==
    //Auto clean

    fBinSel->ReadFile(false, false, "");
    fShowBinSelect->ShowBinSel();
    fMain->UpdateMainOperateMode();
    fSetup->ReadFile();

    fMain->ShowTestHeadComp(true);
    fYieldMonitoring->ReadFile();

    MyList->Clear();
    sBinTraySetT3Pos->Clear();
    delete MyList;
    delete sBinTraySetT3Pos;
    bFTPDownlodFinish=true;
    ShowMyMessage("Auto download sucessfully");
}
//pig 20180928 AddASECLFTP <==
//<==
//Steven 20181224 : For ASE-CL
//---------------------------------------------------------------------------
bool __fastcall TfFTPClient::DoPassword()                                       //JerryYang 20151028 add for Amkor
{
    bool bFlag=true;
    bool bTechComExist=FileExists(pwPath);                                      //2012-01-03    Dell modify

    if(fInput->fShow==false)
    {
        if(bTechComExist)
            fMain->cbUserSelectChange(NULL);
        else
            fMain->stOperatorClick(fMain);

        if(AccessLevel<3)                                                       //Supervise才有權限
        {
            bFlag=false;
            if(bTechComExist)
            {
                fPassword->Label3->Visible=true;
                fPassword->Label4->Visible=true;
            }
        }
        else
        {
            if(bTechComExist)
            {
                fPassword->Label3->Visible=false;
                fPassword->Label4->Visible=false;
            }
        }
    }
    return bFlag;
}
//---------------------------------------------------------------------------
int TfFTPClient::N25_ReadAutoStartFileFromFTP()                                 //Steven 20210413 : 南茂的自動Start功能, -1:Alarm, 0:不能跑, 1:可以跑
{
    int iResult=0;
    AnsiString sDownloadFileName, sMsg;
    TStringList *sList;
    int iStep=0;

    if(IniConfig.bN25_1_EnableStartControl==false)
        return 1;

    if(IniConfig.sN25_1_FTPHost=="")
    {
        ShowMyMessage("FTP Server Host is Empty");
        return -1;
    }
    if(IniConfig.sN25_1_FTPUserName=="")
    {
        ShowMyMessage("FTP Server User Name is Empty");
        return -1;
    }
    if(IniConfig.sN25_1_FTPPassword=="")
    {
        ShowMyMessage("FTP Server Password is Empty");
        return -1;
    }
    if(IniConfig.sN25_1_FTPPath=="")
    {
        ShowMyMessage("FTP Server File Path is Empty");
        return -1;
    }

    AnsiString sFTPPath=IniConfig.sN25_1_FTPPath;
    sDownloadFileName.sprintf("%s\\Start.txt", asChipMosFTPPath);

    TNMFTP *pFTP;
    pFTP=new TNMFTP(this);

    MyForceDirectories(asChipMosFTPPath);

    if(FileExists(sDownloadFileName))
    {
        DeleteFile(sDownloadFileName);
    }

    sFTPPath.sprintf("%s", IniConfig.SocketHandlerID);

    try
    {
        pFTP->Vendor    = NMOS_AUTO;
        pFTP->TimeOut   = 30000;
        pFTP->Host      = IniConfig.sN25_1_FTPHost;
        pFTP->UserID    = IniConfig.sN25_1_FTPUserName;
        pFTP->Password  = IniConfig.sN25_1_FTPPassword;
        pFTP->OnListItem= NMFTP1ListItem;
        pFTP->Name      = "FTP_N25";
        pFTP->Connect();

        for(int i=0; i<300; i++)                                                // 300 x 10 = 1000 ( 3 Sec )
        {
            MySleep(10);
            if(pFTP->Connected==true)
                break;
        }

        if(pFTP->Connected==false)
        {
            ShowMyMessage("FTP Server is not connected");
            delete pFTP;
            return -1;
        }

        if(IniConfig.sN25_1_FTPPath.SubString(IniConfig.sN25_1_FTPPath.Length(), 1)!="/")
        {
            pFTP->ChangeDir(IniConfig.sN25_1_FTPPath + "/");
        }
        else
        {
            pFTP->ChangeDir(IniConfig.sN25_1_FTPPath);
        }

        bTempList=true;
        try
        {
            pFTP->Nlist();
        }
        catch(Exception& e)
        {
            MyDBIProcess("Exception", "TfFTPClient::N25_ReadAutoStartFileFromFTP", e.Message);
            memoFTP->Lines->Add("Error -- List directory and files, Fail--------");
            bError=true;
            delete pFTP;
            pFTP=NULL;                                                          //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return -1;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfFTPClient::N25_ReadAutoStartFileFromFTP");
            memoFTP->Lines->Add("Error -- List directory and files, Fail--------");
            bError=true;
            delete pFTP;
            pFTP=NULL;                                                          //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return -1;
        }

        if(ListBox1->Items->Text.AnsiPos(IniConfig.SocketHandlerID)!=0)
        {
            pFTP->Download(sFTPPath, asChipMosFTPPath+"\\Start.txt");           //KaiChen 20200316 ：修正
            sList=new TStringList();
            sList->LoadFromFile(sDownloadFileName);
            if(sList->Count>0)
            {
                if(atoi(sList->Strings[0].c_str())==1)
                {
                    iStep=1;
                    iResult=1;
                }
                else
                {
                    iStep=2;
                }
            }
            else
            {
                iStep=3;
            }
        }
        else
        {
            iStep=4;
            iResult=1;                                                          //沒檔案可以跑
        }
        pFTP->Abort();
        pFTP->RequestCloseSocket();
    }
    catch(Exception& e)
    {
        MyDBIProcess("Exception", "TfFTPClient::ReadAutoStartFileFromFTP", e.Message);
        delete pFTP;
        ShowMyMessage("FTP Download Fail");
        return -1;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::ReadAutoStartFileFromFTP");
        delete pFTP;
        ShowMyMessage("FTP Download Fail");
        return -1;
    }

    if(iStep==1)
        sMsg="N25_1 Start.txt file content is 1! (iStep==1)";
    else if(iStep==2)
        sMsg="N25_1 Start.txt file content is not 1! (iStep==2)";
    else if(iStep==3)
        sMsg="N25_1 Start.txt file content is empty! (iStep==3)";
    else if(iStep==4)
        sMsg="N25_1 Start.txt file not found! (iStep==4)";
    else
        sMsg="N25_1 FTP exception Error! (iStep==0)";

    RecordProcess(sMsg);

    delete pFTP;
    return iResult;
}
//---------------------------------------------------------------------------
void TfFTPClient::N25_UploadTempDataToFTP(int iType)                            //Steven 20210413 : 南茂的自動Start功能, 0:上傳十筆, 1:只記錄不上傳, 2:整日
{
    if(IniConfig.bN25_2_EnableUploadLog==false)
        return ;

    if(IniConfig.sN25_2_FTPHost=="")
    {
        ShowMyMessage("FTP Server Host is Empty");
        return ;
    }
    if(IniConfig.sN25_2_FTPUserName=="")
    {
        ShowMyMessage("FTP Server User Name is Empty");
        return ;
    }
    if(IniConfig.sN25_2_FTPPassword=="")
    {
        ShowMyMessage("FTP Server Password is Empty");
        return ;
    }
    if(IniConfig.sN25_2_FTPPath=="")
    {
        ShowMyMessage("FTP Server File Path is Empty");
        return ;
    }

    AnsiString sFTPPath, sFTPFile, sUploadFile, sContent, sContent1, str;
    AnsiString sFTPPathHour, sFTPFileHour, sUploadFileHour;
    if(IniConfig.sN25_2_FTPPath.SubString(IniConfig.sN25_2_FTPPath.Length(), 1)!="/")
    {
        sFTPPath.sprintf("%s/%s/", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID);
        sFTPFile.sprintf("%s/%s/HandlerTemperature.csv", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID, IniConfig.SocketHandlerID);
        sFTPPathHour.sprintf("%s/%s/", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID);
        sFTPFileHour.sprintf("%s/%s/BackUp/TemperatureLog_%04d%02d%02d.csv", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    }
    else
    {
        sFTPPath.sprintf("%s%s/", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID);
        sFTPFile.sprintf("%s%s/HandlerTemperature.csv", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID, IniConfig.SocketHandlerID);
        sFTPPathHour.sprintf("%s%s/", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID);
        sFTPFileHour.sprintf("%s%s/BackUp/TemperatureLog_%04d%02d%02d.csv", IniConfig.sN25_2_FTPPath, IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    }

    if(LastSet.iTemperature==Tempture_Ambient)                                  //Steven 20220513 : For ChipMos FTP
        sContent1.sprintf("%s,Ambient,25,00,%s,%s,", fMain->cbSetupFileName->Text, IniConfig.sMachineType, IniConfig.SocketHandlerID);
    else if(LastSet.iTemperature==Tempture_AmbientHot)
        sContent1.sprintf("%s,Ambient,%s,00,%s,%s,", fMain->cbSetupFileName->Text, fMain->edWorkTemperBase->Text, IniConfig.sMachineType, IniConfig.SocketHandlerID);
    else
        sContent1.sprintf("%s,Hot,%s,%s,%s,%s,", fMain->cbSetupFileName->Text, fMain->edWorkTemperBase->Text, fMain->edSoakTime->Text, IniConfig.sMachineType, IniConfig.SocketHandlerID);

    //AI(ht9045-v899) 20260521: limit ChipMos Chamber upload value by selected heat mode
    bool bN25UploadChamberTemp=true;
    if(CUSTOMER_CODE==CC_ChipMos_TAINAN || CUSTOMER_CODE==CC_ChipMos_ZHUBEI)
    {
        bN25UploadChamberTemp=(Temperature.iIndexHeatMode==ChamberOnly ||
                                Temperature.iIndexHeatMode==HeadChamber ||
                                Temperature.iIndexHeatMode==SocketChamber ||
                                Temperature.iIndexHeatMode==HeadChamberSocket);
    }

    sContent1+=(bUT150Install[tcHotPlate1])?(str.sprintf("%0.2f,", UN150Read[tcHotPlate1])):AnsiString("-,");   //Steven 20230410 : 南茂鍾永生要求修改
    sContent1+=(bUT150Install[tcHotPlate2])?(str.sprintf("%0.2f,", UN150Read[tcHotPlate2])):AnsiString("-,");
    sContent1+=(bUT150Install[tcShuttle1])?(str.sprintf("%0.2f,", UN150Read[tcShuttle1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcShuttle2])?(str.sprintf("%0.2f,", UN150Read[tcShuttle2])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAa1])?(str.sprintf("%0.2f,", UN150Read[tcAa1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBa1])?(str.sprintf("%0.2f,", UN150Read[tcBa1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAb1])?(str.sprintf("%0.2f,", UN150Read[tcAb1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBb1])?(str.sprintf("%0.2f,", UN150Read[tcBb1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAc1])?(str.sprintf("%0.2f,", UN150Read[tcAc1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBc1])?(str.sprintf("%0.2f,", UN150Read[tcBc1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAd1])?(str.sprintf("%0.2f,", UN150Read[tcAd1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBd1])?(str.sprintf("%0.2f,", UN150Read[tcBd1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAa2])?(str.sprintf("%0.2f,", UN150Read[tcAa1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBa2])?(str.sprintf("%0.2f,", UN150Read[tcBa1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAb2])?(str.sprintf("%0.2f,", UN150Read[tcAb1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBb2])?(str.sprintf("%0.2f,", UN150Read[tcBb1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAc2])?(str.sprintf("%0.2f,", UN150Read[tcAc1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBc2])?(str.sprintf("%0.2f,", UN150Read[tcBc1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcAd2])?(str.sprintf("%0.2f,", UN150Read[tcAd1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcBd2])?(str.sprintf("%0.2f,", UN150Read[tcBd1])):AnsiString("-,");
    sContent1+=(bUT150Install[tcChamber] && bN25UploadChamberTemp)?(str.sprintf("%0.2f", UN150Read[tcChamber])):AnsiString("-");

    slDailyTempLog->AddTextWithDateTime2(sContent1+sContent);
    slDailyTempLog->MySaveToFile();
    sUploadFileHour=slDailyTempLog->GetFileName();

    sl10MinTempLog->AddTextWithDateTime2(sContent1+sContent);
    sl10MinTempLog->MySaveToFile();
    sUploadFile=sl10MinTempLog->GetFileName();

    TNMFTP *pFTP;
    pFTP=new TNMFTP(this);

    if(iType!=1)
    {
        try
        {
            pFTP->Vendor    = NMOS_AUTO;
            pFTP->TimeOut   = 30000;
            pFTP->Host      = IniConfig.sN25_2_FTPHost;
            pFTP->UserID    = IniConfig.sN25_2_FTPUserName;
            pFTP->Password  = IniConfig.sN25_2_FTPPassword;
            pFTP->OnListItem= NMFTP1ListItem;
//            pFTP->OnFailure = NMFTP1Failure;
            //Steven 20220510 : 針對FTP動作做紀錄
            //==>
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
            //<==
            //Steven 20220510 : 針對FTP動作做紀錄
            pFTP->Connect();

            for(int i=0; i<300; i++)                                            // 300 x 10 = 1000 ( 3 Sec )
            {
                MySleep(10);
                if(pFTP->Connected==true)
                    break;
            }

            if(pFTP->Connected==false)
            {
                ShowMyMessage("FTP Server is not connected");
                delete pFTP;
                return ;
            }

            try
            {
                pFTP->Upload(sUploadFile, sFTPFile);
            }
            catch(Exception &e)                                                 //Steven 20220510 : 針對FTP動作做紀錄
            {
                str.sprintf("TfFTPClient::N25_UploadTempDataToFTP Upload file fail : %s to %s ", sUploadFile, sFTPFile);
                MyDBIProcess("Exception", str, e.Message);
            }

            if(SystemMin==59 || iType==2)
            {
                try
                {
                    pFTP->Upload(sUploadFileHour, sFTPFileHour);
                }
                catch(Exception &e)                                             //Steven 20220510 : 針對FTP動作做紀錄
                {
                    str.sprintf("TfFTPClient::N25_UploadTempDataToFTP Upload file fail : %s to %s", sUploadFileHour, sFTPFileHour);
                    MyDBIProcess("Exception", str, e.Message);
                }
            }
            pFTP->Abort();
            pFTP->RequestCloseSocket();
        }
        catch(Exception &e)                                                     //Steven 20220510 : 針對FTP動作做紀錄
        {
            MyDBIProcess("Exception", "TfFTPClient::N25_UploadTempDataToFTP", e.Message);
            delete pFTP;
            ShowMyMessage("FTP upload Fail");
            return ;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfFTPClient::N25_UploadTempDataToFTP");
            delete pFTP;
            ShowMyMessage("FTP upload Fail");
            return ;
        }

        if(SystemMin==59 && FileExists(sUploadFile))
        {
            DeleteFile(sUploadFile);
        }
    }

    delete pFTP;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtHDWaferNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    AnsiString Str;

    if(CosFunction.bFTPUseBarcodeReader==true &&
       IniConfig.bN06_UseBarcode==true)
    {
        if(CUSTOMER_CODE==CC_CYUEAN)
        {
                                                                                //Sam 20230706 : 确安科技上傳不需要用 Barcode reader
        }
        else
        {
            AnsiString sBarcodeID=InputBarcodeNumber("Input SetupFile Name:");
            edtHDWaferName->Text=sBarcodeID;
            if(lstHDFile->Items->Count==0)
            {
                edtHDWaferName->Text="";
                Str.sprintf("%s|", sBarcodeID);
                ShowMyMessage("Setup file name does not exists in HDD.", Str);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtHDWaferNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString Str;

    if(CosFunction.bFTPUseBarcodeReader==true &&
       IniConfig.bN06_UseBarcode==true)
    {
        if(CUSTOMER_CODE==CC_CYUEAN)
        {
                                                                                //Sam 20230706 : 确安科技上傳不需要用 Barcode reader
        }
        else
        {
            AnsiString sBarcodeID=InputBarcodeNumber("Input SetupFile Name:");
            edtHDWaferName->Text=sBarcodeID;
            if(lstHDFile->Items->Count==0)
            {
                edtHDWaferName->Text="";
                Str.sprintf("%s|", sBarcodeID);
                ShowMyMessage("Setup file name does not exists in HDD.", Str);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtServerWaferNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    AnsiString Str;
    if(CosFunction.bFTPUseBarcodeReader==true &&
       IniConfig.bN06_UseBarcode==true)
    {
        AnsiString sBarcodeID=InputBarcodeNumber("Input SetupFile Name:");
        edtServerWaferName->Text=sBarcodeID;
        if(lstServerFile->Items->Count==0)
        {
            edtServerWaferName->Text="";
            Str.sprintf("%s|", sBarcodeID);
            ShowMyMessage("Setup file name does not exists at FTP.", Str);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtServerWaferNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString Str;
    if(CosFunction.bFTPUseBarcodeReader==true &&
       IniConfig.bN06_UseBarcode==true)
    {
        AnsiString sBarcodeID=InputBarcodeNumber("Input SetupFile Name:");
        edtServerWaferName->Text=sBarcodeID;
        if(lstServerFile->Items->Count==0)
        {
            edtServerWaferName->Text="";
            Str.sprintf("%s|", sBarcodeID);
            ShowMyMessage("Setup file name does not exists at FTP.", Str);
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfFTPClient::CheckSetupFileNameFromServer(AnsiString asCheckSetupFileName)  //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
{
    int i=0;
    //if(tmpList==NULL)
        tmpList=new TStringList;
    AnsiString cDir="", Cur="", DirName="";
    try
    {
        NMFTP3=new TNMFTP(this);
    }
    catch(Exception& e)
    {
        delete NMFTP3;
        NMFTP3=NULL;                                                            //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
        MyDBIProcess("FTP", "CheckSetupFileNameFromServer - new TNMFTP Fail", e.Message);
        return false;
    }
    lstServerFile->Clear();

    //設定並連線FTP--------------------
    try
    {
        NMFTP3->Vendor                  =NMOS_AUTO;
        NMFTP3->TimeOut                 =20000; //Landam
        NMFTP3->Passive                 =true;  //Steven 20121020 : 實驗看看

        if(IniConfig.bA32EnableFTPAutomation==true)
        {
            if(IniConfig.FtpHost!="")
                NMFTP3->Host                =FTPAutomation_Down_ServerIP;
            NMFTP3->UserID                  =FTPAutomation_Down_UserID;
            NMFTP3->Password                =FTPAutomation_Down_Password;
        }
        else
        {
            if(IniConfig.FtpHost!="")
                NMFTP3->Host                =IniConfig.FtpHost;
            NMFTP3->UserID                  =IniConfig.FtpUserName;//edtUserName->Text;
            NMFTP3->Password                =IniConfig.FtpPassword;//edtPassword->Text;
        }

        NMFTP3->ParseList               =true;
        NMFTP3->OnListItem              =NMFTP1ListItem;
        NMFTP3->OnSuccess               =NMFTP1Success;
        NMFTP3->OnFailure               =NMFTP1Failure;
        NMFTP3->OnError                 =NMFTP1Error;
        NMFTP3->OnStatus                =NMFTP1Status;
        NMFTP3->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP3->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP3->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP3->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        try
        {
            NMFTP3->Connect();
        }
        catch(Exception& e)                                                     //Steven 20140505 : 試著抓出連線異常的訊息
        {
            NMFTP3->Disconnect();
            NMFTP3->Abort();
            MyDBIProcess("FTP", "CheckSetupFileNameFromServer - Connect Fail", e.Message);
            return false;
        }

        delay(500);

        if(!NMFTP3->Connected)
        {
            delete NMFTP3;
            NMFTP3=NULL;                                                        //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return false;
        }

        //切換路徑-------------------------
        if(sRootPath=="/")
            sRootPath="";

        if(IniConfig.bA32EnableFTPAutomation==true)                             //KaiChen 20190530 ：Sigurd FTP Automation
            DirName=sRootPath+FTPAutomation_Down_ServerPath;
        else
            DirName=sRootPath+IniConfig.FtpDownloadPath;

        cDir.sprintf("%s%s", DirName, "/");
        Cur=NMFTP3->CurrentDir;
        if(Cur!=DirName)
        {
            try
            {
                if(DirName.SubString(DirName.Length(),1)!="/")
                {
                    NMFTP3->ChangeDir(cDir);
                }
                else
                {
                    NMFTP3->ChangeDir(DirName);
                }
            }
            catch(Exception& e)                                                 //Steven 20140505 : 試著抓出連線異常的訊息
            {
                MyDBIProcess("FTP", "CheckSetupFileNameFromServer - ChangeDir Fail", e.Message);
                return false;
            }
        }

        delay(300);
        //列表---------------------------
        if(!NMFTP3->Connected)
        {
            delete NMFTP3;
            NMFTP3=NULL;                                                        //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return false;
        }
        ListBox1->Items->Clear();

        bTempList=false;
        bListOk=false;

        try
        {
            NMFTP3->Nlist();
        }
        catch(Exception& e)
        {
            delete NMFTP3;
            NMFTP3=NULL;    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            MyDBIProcess("FTP", "CheckSetupFileNameFromServer - Nlist Fail", e.Message);
            return false;
        }
        catch(...)
        {
            delete NMFTP3;
            NMFTP3=NULL;    //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return false;
        }

        i=0;
        do
        {
            i++;
            delay(300);
        }
        while(bListOk==false && i<20);

        bTempList=true;
        delay(300);

        if(tmpList->Count==0)
        {
            return false;
        }
        for(int i=0; i<tmpList->Count; i++)
        {
            if(tmpList->Strings[i]==asCheckSetupFileName)
            {
                return true;
            }
        }
    }
    catch(Exception& e)
    {
        delete NMFTP3;
        NMFTP3=NULL;                                                            //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
        return false;
    }
    catch(...)
    {
        delete NMFTP3;
        NMFTP3=NULL;                                                            //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
        return false;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfFTPClient::UploadFileFTP(AnsiString asSourcePath, AnsiString asSourceFileName, AnsiString asTargetPath, AnsiString asTargetFileName, AnsiString asUserID, AnsiString asPassword, AnsiString asHost, AnsiString asFun) //Sam 20210120 : Auto Upload FTP Temperature Info 功能
{
    AnsiString asSourceFilePath="";
    AnsiString asTargetFilePath="";
    AnsiString asErr="";
    if(asTargetPath.Trim()=="")
        return;

    //檢查來源路徑最後必需為 "\\"
    if(asSourcePath.SubString(asSourcePath.Length(),1)!="\\")
    {
        asSourcePath=asSourcePath+AnsiString("\\");
    }
    //檢查目標路徑最後必需為 "/"
    if(asTargetPath.SubString(asTargetPath.Length(),1)!="/")
    {
        asTargetPath=asTargetPath+AnsiString("/");
    }

    if(asTargetFileName!="")
        asTargetFilePath.sprintf("%s%s",asTargetPath, asTargetFileName);
    else
        asTargetFilePath.sprintf("%s%s",asTargetPath, asSourceFileName);

    asSourceFilePath.sprintf("%s%s", asSourcePath, asSourceFileName);
    if(FileExists(asSourceFilePath)==false)
        return;

    bIsFtpRunning=true;
    TNMFTP *NMFTP2;
    try
    {
        NMFTP2=new  TNMFTP(NULL);
        NMFTP2->Vendor = NMOS_AUTO;

        NMFTP2->TimeOut=20000;

        if(asHost!= "" )
            NMFTP2->Host=asHost;
        NMFTP2->UserID  =asUserID;
        NMFTP2->Password=asPassword;
        //Steven 20220510 : 針對FTP動作做紀錄
        //==>
        NMFTP2->OnSuccess               =NMFTP2Success;
        NMFTP2->OnFailure               =NMFTP2Failure;
        NMFTP2->OnError                 =NMFTP2Error;
        NMFTP2->OnStatus                =NMFTP2Status;
        NMFTP2->OnConnectionFailed      =NMFTP2ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP2TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP2TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP2AuthenticationFailed;
        NMFTP2->OnUnSupportedFunction   =NMFTP2UnSupportedFunction;
        NMFTP2->Name                    ="UploadFileFTP";
        //<==
        //Steven 20220510 : 針對FTP動作做紀錄

        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            MySleep(5);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            asErr.sprintf("FTP Server is not connected, %s", asFun);
            ShowMyMessage(asErr);
            bIsFtpRunning=false;
            delete NMFTP2;
            return;
        }

        //檢查目標路徑，用 ChangeDir 的方式來檢查
        try
        {
            NMFTP2->ChangeDir(asTargetPath);
        }
        catch(Exception& e)
        {
            //建立目標路徑
            NMFTP2->MakeDirectory(asTargetPath);
        }
        catch(...)
        {
            //建立目標路徑
            NMFTP2->MakeDirectory(asTargetPath);
        }

        //Up load File
        NMFTP2->Upload(asSourceFilePath, asTargetFilePath);

        NMFTP2->Abort();
        NMFTP2->RequestCloseSocket();
        bIsFtpRunning=false;
        delete NMFTP2;

    }
    catch(Exception& e)
    {
        asErr.sprintf("FTP Upload Fail, %s", asFun);
        ShowMyMessage(asErr, e.Message);
        bIsFtpRunning=false;
        delete NMFTP2;
    }
    catch(...)
    {
        asErr.sprintf("FTP Upload Fail, %s", asFun);
        ShowMyMessage(asErr);
        bIsFtpRunning=false;
        delete NMFTP2;
    }
}
//---------------------------------------------------------------------------
//Steven 20220510 : 針對FTP動作做紀錄
//==>
void __fastcall TfFTPClient::NMFTP2Success(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MyDBIProcess("FTP", "Success -- ChangeDir successful"   );break;
        case cmdMakeDir     : MyDBIProcess("FTP", "Success -- MakeDir successful"     );break;
        case cmdRemoveDir   : MyDBIProcess("FTP", "Success -- RemoveDir successful"   );break;
        case cmdDelete      : MyDBIProcess("FTP", "Success -- Delete successful"      );break;
        case cmdRename      : MyDBIProcess("FTP", "Success -- Rename successful"      );break;
        case cmdReInit      : MyDBIProcess("FTP", "Success -- Reinitialize successful");break;
        case cmdCurrentDir  : MyDBIProcess("FTP", "Success -- CurrentDir successful"  );break;
        case cmdList        : MyDBIProcess("FTP", "Success -- List successful"        );break;
        case cmdNList       : MyDBIProcess("FTP", "Success -- NList successful"       );break;
        case cmdUpRestore   : MyDBIProcess("FTP", "Success -- UpRestore successful"   );break;
        case cmdDownRestore : MyDBIProcess("FTP", "Success -- DownRestore successful" );break;
        case cmdDownload    : MyDBIProcess("FTP", "Success -- Download successful"    );break;
        case cmdUpload      : MyDBIProcess("FTP", "Success -- Upload successful"      );break;
        case cmdAppend      : MyDBIProcess("FTP", "Success -- Append successful"      );break;
        case cmdAllocate    : MyDBIProcess("FTP", "Success -- Allocate successful"    );break;
        case cmdDoCommand   : MyDBIProcess("FTP", "Success -- DoCommand successful"   );break;
    }
    bError=false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2AuthenticationFailed(bool &Handled)
{
    MyDBIProcess("FTP", "Authentication Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2TransactionStop(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Data transfer end", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2TransactionStart(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Data transfer start", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2ConnectionFailed(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Failure -- Connection Failed", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Failure(bool &Handled, TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MyDBIProcess("FTP", "Failure -- ChangeDir failed"      ); break;
        case cmdMakeDir     : MyDBIProcess("FTP", "Failure -- MakeDir failed"        ); break;
        case cmdDelete      : MyDBIProcess("FTP", "Failure -- Delete failed"         ); break;
        case cmdRemoveDir   : MyDBIProcess("FTP", "Failure -- RemoveDir failed"      ); break;
        case cmdList        : MyDBIProcess("FTP", "Failure -- List failed"           ); break;
        case cmdRename      : MyDBIProcess("FTP", "Failure -- Rename failed"         ); break;
        case cmdUpRestore   : MyDBIProcess("FTP", "Failure -- UploadRestore failed"  ); break;
        case cmdDownRestore : MyDBIProcess("FTP", "Failure -- DownloadRestore failed"); break;
        case cmdDownload    : MyDBIProcess("FTP", "Failure -- Download failed"       ); break;
        case cmdUpload      : MyDBIProcess("FTP", "Failure -- Upload failed"         ); break;
        case cmdAppend      : MyDBIProcess("FTP", "Failure -- UploadAppend failed"   ); break;
        case cmdReInit      : MyDBIProcess("FTP", "Failure -- Reinitialize failed"   ); break;
        case cmdAllocate    : MyDBIProcess("FTP", "Failure -- Allocate failed"       ); break;
        case cmdNList       : MyDBIProcess("FTP", "Failure -- NList failed"          ); break;
        case cmdDoCommand   : MyDBIProcess("FTP", "Failure -- DoCommand failed"      ); break;
        case cmdCurrentDir  : MyDBIProcess("FTP", "Failure -- CurrentDir failed"     ); break;
    }
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2UnSupportedFunction(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MyDBIProcess("FTP", "UnSupported -- ChangeDir not supported by this server"       ); break;
        case cmdMakeDir     : MyDBIProcess("FTP", "UnSupported -- MakeDir not supported by this server"         ); break;
        case cmdDelete      : MyDBIProcess("FTP", "UnSupported -- Delete not supported by this server"          ); break;
        case cmdRemoveDir   : MyDBIProcess("FTP", "UnSupported -- RemoveDir not supported by this server"       ); break;
        case cmdList        : MyDBIProcess("FTP", "UnSupported -- List not supported by this server"            ); break;
        case cmdRename      : MyDBIProcess("FTP", "UnSupported -- Rename not supported by this server"          ); break;
        case cmdUpRestore   : MyDBIProcess("FTP", "UnSupported -- UploadRestore not supported by this server"   ); break;
        case cmdDownRestore : MyDBIProcess("FTP", "UnSupported -- DownloadRestore not supported by this server" ); break;
        case cmdDownload    : MyDBIProcess("FTP", "UnSupported -- Download not supported by this server"        ); break;
        case cmdUpload      : MyDBIProcess("FTP", "UnSupported -- Upload not supported by this server"          ); break;
        case cmdAppend      : MyDBIProcess("FTP", "UnSupported -- UploadAppend not supported by this server"    ); break;
        case cmdReInit      : MyDBIProcess("FTP", "UnSupported -- Reinitialize not supported by this server"    ); break;
        case cmdAllocate    : MyDBIProcess("FTP", "UnSupported -- Allocate not supported by this server"        ); break;
        case cmdNList       : MyDBIProcess("FTP", "UnSupported -- NList not supported by this server"           ); break;
        case cmdDoCommand   : MyDBIProcess("FTP", "UnSupported -- DoCommand not supported by this server"       ); break;
        case cmdCurrentDir  : MyDBIProcess("FTP", "UnSupported -- CurrentDir not supported by this server"      ); break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", Errmsg, Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Status(TComponent *Sender,
      AnsiString Status)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", Status, Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Connect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "FTP Connect", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Disconnect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "FTP Disconnect", Ptr->Name);
}
//---------------------------------------------------------------------------
//<==
//Steven 20220510 : 針對FTP動作做紀錄
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N31_AtuoTempureOfsByFTP()                               //Sam 20220406 : 溫度自動補償功能 By FTP
{
    int iProcess=0;
    AnsiString sResult="NULL", sServerFileName="", sTargetFileFullPath="", sListText="", sDateTime="", sBackupFileFullPath="";
    int iPos=0, iLen=0;
    double dDateTime=0.0,dNewDateTime=0.0;
    AnsiString str="";
    if(IniConfig.sN31_FTPHost=="")
    {
        ShowMyMessage("[N31] FTP Server Host is Empty");
        return "NULL";
    }
    if(IniConfig.sN31_FTPUserName=="")
    {
        ShowMyMessage("[N31] FTP Server User Name is Empty");
        return "NULL";
    }
    if(IniConfig.sN31_FTPPassword=="")
    {
        ShowMyMessage("[N31] FTP Server Password is Empty");
        return "NULL";
    }
    if(IniConfig.sN31_FTPDownloadPath=="")
    {
        ShowMyMessage("[N31] FTP Server File Path is Empty");
        return "NULL";
    }

    TNMFTP *pFTP;
    pFTP=new TNMFTP(this);
    MyForceDirectories(aAutoTempureOffsetFTP);

    try
    {
        pFTP->Vendor    = NMOS_AUTO;
        pFTP->TimeOut   = 30000;
        pFTP->Host      = IniConfig.sN31_FTPHost;
        pFTP->UserID    = IniConfig.sN31_FTPUserName;
        pFTP->Password  = IniConfig.sN31_FTPPassword;

        iProcess=1;
        pFTP->OnListItem              =NMFTP1ListItem;
        pFTP->OnSuccess               =NMFTP1Success;
        pFTP->OnFailure               =NMFTP1Failure;
        pFTP->OnError                 =NMFTP1Error;
        pFTP->OnStatus                =NMFTP1Status;
        pFTP->OnConnectionFailed      =NMFTP1ConnectionFailed;
        pFTP->OnTransactionStart      =NMFTP1TransactionStart;
        pFTP->OnTransactionStop       =NMFTP1TransactionStop;
        pFTP->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        iProcess=2;
        pFTP->Connect();
        iProcess=3;

        for(int i=0; i<300; i++)                                                // 300 x 10 = 1000 ( 3 Sec )
        {
            MySleep(10);
            if(pFTP->Connected==true)
                break;
        }

        if(pFTP->Connected==false)
        {
            ShowMyMessage("[N31] FTP Server is not connected");
            delete pFTP;
            return "NULL";
        }

        iProcess=4;
        if(IniConfig.sN31_FTPDownloadPath.SubString(IniConfig.sN31_FTPDownloadPath.Length(), 1)!="/")
        {
            pFTP->ChangeDir(IniConfig.sN31_FTPDownloadPath + "/");
        }
        else
        {
            pFTP->ChangeDir(IniConfig.sN31_FTPDownloadPath);
        }
        iProcess=5;

        bTempList=true;
        ListBox1->Items->Clear();
        try
        {
            pFTP->Nlist();
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfFTPClient::N31_AtuoTempureOfsByFTP1");
            memoFTP->Lines->Add("Error -- List directory and files, Fail--------");
            bError=true;
            delete pFTP;
            pFTP=NULL;
            return "NULL";
        }

        sListText=ListBox1->Items->Text.UpperCase();
        if(sListText.AnsiPos("SETTEMPOFS")!=0)
        {
            sBackupFileFullPath.sprintf("%s\\%d\\%d", aAutoTempureOffsetFTP, SystemYear, SystemMonth);
            MyForceDirectories(sBackupFileFullPath);                            //Steven 20241220 : TJ Offset的備份資料夾裡面加上年月資料夾

            dNewDateTime=0.0;
            for(int i=0; i<ListBox1->Items->Count; i++)
            {
                sListText=ListBox1->Items->Strings[i].UpperCase();              //SetTempOfs_20220309150000.ini
                iPos=sListText.Pos("SETTEMPOFS");
                if(iPos!=0)
                {
                    sServerFileName=ListBox1->Items->Strings[i].SubString(iPos, ListBox1->Items->Strings[i].Length());
                    MyDBIProcess("FTP", sServerFileName);
                    iPos=sServerFileName.Pos("_")+1;
                    iLen=sServerFileName.Length();
                    if(iLen>14)
                        iLen=14;
                    sDateTime=sServerFileName.SubString(iPos, iLen);
                    dDateTime=atof(sDateTime.c_str());
                    sBackupFileFullPath.sprintf("%s\\%d\\%d\\%s", aAutoTempureOffsetFTP, SystemYear, SystemMonth, sServerFileName);//D:\HT9045_Log\AutoTempureOffsetFTP\\SetTempOfs_20220309150000.ini
                    sResult.sprintf("Download file %s to %s", sServerFileName, sBackupFileFullPath);
                    MyDBIProcess("FTP", sResult);

                    iProcess=6;
                    pFTP->Download(sServerFileName, sBackupFileFullPath);
                    iProcess=7;
                    pFTP->Delete(sServerFileName);
                    iProcess=8;
                    if(dDateTime>dNewDateTime)                                  //只回傳最後伺服器上最新的檔名
                    {
                        dDateTime=dNewDateTime;
                        sResult=sBackupFileFullPath;
                    }
                }
            }
            ListBox1->Clear();
        }
        iProcess=9;
        pFTP->Abort();
        pFTP->RequestCloseSocket();
        iProcess=10;
    }
    catch(...)
    {
        sResult.sprintf("TfFTPClient::N31_AtuoTempureOfsByFTP2 - %d", iProcess);
        MyDBIProcess("Exception", sResult);
        delete pFTP;
        ShowMyMessage("FTP Download Fail");
        return sResult;
    }
    delete pFTP;

    str.sprintf("Using file %s to change temp offset", sResult);
    MyDBIProcess("FTP", str);
    return sResult;
}
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N31_AtuoTempureOfsByLocal()                             //Sam 20220427 : 溫度自動補償功能 By Loacl
{
    AnsiString sResult="NULL", sServerFileName="", sTargetFileFullPath="", sBackupFileFullPath, sListText="", sDateTime="";
    int iPos=0,iLen=0;
    double dDateTime=0.0,dNewDateTime=0.0;

    MyForceDirectories(aAutoTempureOffsetFTP);
    sBackupFileFullPath.sprintf("%s\\%d\\%02d", aAutoTempureOffsetFTPBackup, SystemYear, SystemMonth);
    MyForceDirectories(sBackupFileFullPath);                                    //Steven 20241220 : TJ Offset的備份資料夾裡面加上年月資料夾

    try
    {
        AnsiString sSetupFile=aAutoTempureOffsetFTP+"\\*.*";
        TSearchRec srFile;
        if(FindFirst(sSetupFile, 0, srFile)==0)
        {
            dNewDateTime=0.0;
            do
            {
                sServerFileName=srFile.Name.UpperCase();
                iPos=sServerFileName.Pos("SETTEMPOFS");
                if(sServerFileName.Pos("SETTEMPOFS")!=0)
                {
                    iPos=sServerFileName.Pos("_")+1;
                    iLen=sServerFileName.Length();
                    if(iLen>14)
                        iLen=14;
                    sDateTime=sServerFileName.SubString(iPos, iLen);
                    dDateTime=atof(sDateTime.c_str());
                    sTargetFileFullPath.sprintf("%s\\%s", aAutoTempureOffsetFTP, sServerFileName);  //D:\HT9045_Log\AutoTempureOffsetFTP\\SetTempOfs_20220309150000.ini
                    sBackupFileFullPath.sprintf("%s\\%d\\%02d\\%s", aAutoTempureOffsetFTPBackup, SystemYear, SystemMonth, sServerFileName);     //D:\HT9045_Log\AutoTempureOffsetFTPBackup\\SetTempOfs_20220309150000.ini
                    CopyFile(sTargetFileFullPath.c_str(), sBackupFileFullPath.c_str(), false);
                    DeleteFile(sTargetFileFullPath);
                    if(dDateTime>dNewDateTime)
                    {
                        dDateTime=dNewDateTime;
                        sResult=sBackupFileFullPath;
                    }
                }
            }
            while(FindNext(srFile)==0);
            FindClose(srFile);
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::N31_AtuoTempureOfsByLocal");
        ShowMyMessage("Search file fail");
        return sResult;
    }
    return sResult;
}
//---------------------------------------------------------------------------
bool TfFTPClient::DownloadPasswordFormServer()                                  //Sam 20210526 : 從 N06 DownloadPath 下載密碼本
{
    if(CosFunction.PassworDownloadByFTP==false ||
       IniConfig.bFtpPasswordDownload==false)
        return false;

    TNMFTP *NMFTP2;
    try
    {
        NMFTP2=new TNMFTP(this);

        //設定參數--------
        NMFTP2->Vendor                  =NMOS_AUTO;
        NMFTP2->TimeOut                 =20000;                                 //Landam
        NMFTP2->Passive                 =true;                                  //Steven 20121020 : 實驗看看
        if(IniConfig.FtpHost!="")
            NMFTP2->Host                =IniConfig.FtpHost;
        NMFTP2->UserID                  =IniConfig.FtpUserName;
        NMFTP2->Password                =IniConfig.FtpPassword;

        //設定Event--------
        NMFTP2->OnListItem              =NMFTP1ListItem;
        NMFTP2->OnSuccess               =NMFTP1Success;
        NMFTP2->OnFailure               =NMFTP1Failure;
        NMFTP2->OnError                 =NMFTP1Error;
        NMFTP2->OnStatus                =NMFTP1Status;
        NMFTP2->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        //連線--------
        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            delay(50);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            ShowMyMessage("FTP Server is not connected","");
            delete NMFTP2;
            bError=true;
            FTP_DownloadFail = true;
            return false;
        }

        if(IniConfig.FtpPasswordDownloadPath.SubString(IniConfig.FtpPasswordDownloadPath.Length(), 1)!="/")
            NMFTP2->ChangeDir(IniConfig.FtpPasswordDownloadPath + "/");
        else
            NMFTP2->ChangeDir(IniConfig.FtpPasswordDownloadPath);

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20231221 : 力成反映有跳出 10035 錯誤。
        {

        }
        else
        {
            if(IniConfig.FtpTransMode==0)                                       //Steven 20230719 : 加入FTP傳輸模式
                NMFTP2->Mode(MODE_ASCII);
            else if(IniConfig.FtpTransMode==1)
                NMFTP2->Mode(MODE_IMAGE);
            else
                NMFTP2->Mode(MODE_BYTE);
        }

        delay(200);
        NMFTP2->Download(pwName, pwPath);                                       //下載檔案   //Steven 20221216 : 使用網路硬碟下載安裝包

        NMFTP2->Abort();
        delay(100);
        NMFTP2->RequestCloseSocket();
        delay(500);
        RecordProcess("Password file download by FTP");
        FTP_DownloadFail=false;                                                 // 2013.12.03 , Joye , KYEC FTP  20140103 wei
        delete NMFTP2;
        return true;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::DownloadPasswordFormServer");
        FTP_DownloadFail=true;                                                  // 2013.12.03 , Joye , KYEC FTP       20140103 wei
        bError=true;
        delete NMFTP2;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::CheckFTPConnection()                               //Isaac 20200710 : 全智科技，從FTP下載後要切換到initial start
{
    try
    {
        NMFTP3=new TNMFTP(this);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal");
        delete NMFTP3;
        NMFTP3=NULL;                                                            //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
        return;
    }

    try                                                                         //設定並連線FTP--------------------
    {
        NMFTP3->Vendor                  =NMOS_AUTO;
        NMFTP3->TimeOut                 =20000;                                 //Landam
        NMFTP3->Passive                 =true;                                  //Steven 20121020 : 實驗看看
        if(IniConfig.FtpHost!="")
            NMFTP3->Host                =IniConfig.FtpHost;
        NMFTP3->UserID                  =IniConfig.FtpUserName;//edtUserName->Text;
        NMFTP3->Password                =IniConfig.FtpPassword;//edtPassword->Text;
        NMFTP3->ParseList               =true;
        NMFTP3->OnListItem              =NMFTP1ListItem;
        NMFTP3->OnSuccess               =NMFTP1Success;
        NMFTP3->OnFailure               =NMFTP1Failure;
        NMFTP3->OnError                 =NMFTP1Error;
        NMFTP3->OnStatus                =NMFTP1Status;
        NMFTP3->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP3->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP3->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP3->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        memoFTP->Lines->Add("Process -- Connecting to server-----------");

        try
        {
            NMFTP3->Connect();
        }
        catch(Exception& e)                                                     //Steven 20140505 : 試著抓出連線異常的訊息
        {
            NMFTP3->Disconnect();
            NMFTP3->Abort();
        }

        delay(500);

        if(!NMFTP3->Connected)
        {
            ShowMyMessage("FTP Server is not connected","");
            fLotInfo->lbFTPStatus->Caption="Connect Fail";
            delete NMFTP3;
            NMFTP3=NULL;                                                        //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return;
        }
        fLotInfo->lbFTPStatus->Caption="Connect OK";
        memoFTP->Lines->Add("Process -- Connecting to server, OK--------");
        plUnload->Enabled=true;

        delay(300);
        //列表---------------------------
        if(!NMFTP3->Connected)
        {
            ShowMyMessage("FTP Server is disconnect","");
            delete NMFTP3;
            NMFTP3=NULL;                                                        //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
            return;
        }
        ListBox1->Items->Clear();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::ShowFTPModal");
        ShowMyMessage("FTP Server is disconnect","");
        delete NMFTP3;
        NMFTP3=NULL;                                                            //jou 2013-01-23 NMFTP3 已經被 delete 了，所以增加NULL判斷
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::plUnloadALLClick(TObject *Sender)                  //Isaac 20200806 : FTP一鍵上傳全部工作檔
{
    AnsiString strUploadName="",strUploadPath="";
    int i,ret;

    strUploadPath="Sure to upload all setup files? Target Folder: "+IniConfig.FtpHost+IniConfig.FtpUplaodPath;

    ret=ShowMyMessageBox_YES_NO(strUploadPath, "確定要上傳全部工作檔?");

    if(ret==1)
    {
        plUnload->Enabled=false;
        plUnloadALL->Enabled=false;

        for(i=0 ; i<tmpList->Count ; i++)
        {
            strUploadName=tmpList->Strings[i];
            SetMD5ByFolder(DataPath+tmpList->Strings[i]);                       //Steven 20170927 (wei) : 將工作檔加入檢查碼

            UploadFileToServer2(sRootPath+IniConfig.FtpUplaodPath, strUploadName, true);    //jou 2015-01-16 修正FTP upload error

            edtHDWaferName->Text="Uploading all setupfile from HD to server: "+strUploadName;
            MySleep(100);

            if(bError==true)
            {
                ShowMyMessage("Upload all, upload "+strUploadName+" failed.");  //Upload 工作檔名 Failed.
                break;
            }
        }

        plUnload->Enabled=true;
        plUnloadALL->Enabled=true;
        edtHDWaferName->Text="";
    }
}
//---------------------------------------------------------------------------
void TfFTPClient::DownloadUpdateAutomatically(bool bhint)                       //Sam 20220824 : FTP 自動下載安裝更新包
{
    if(CosFunction.bDownloadUpdateAutomatically==false ||
       IniConfig.bN32_DownloadUpdatesAutomatically==false)
        return;

    if(IniConfig.bN32_CheckForUpdatesOnceDay && bUpdateAutomaticallyChecked && bhint==false)
        return;

    if(IniConfig.bN32_CheckForUpdatesOnceDay)                                   //一天只要檢查過就不要再檢查了
        bUpdateAutomaticallyChecked=true;
    AnsiString sInstallerFile="",sDownsloadFile1="",sDownsloadFile2="";
    AnsiString sParameters="-IT=silent -CP=HT9045.exe -WT=5 -WC=3";             //每『5』秒鐘檢查一次 『HT9045.exe』是否存在，共檢查『3』次，若都存在就強制關閉。
    int iRet=0;
    bool bUpdata=false;

    if(bhint)                                                                   //手動更新使用
        iRet=ShowMyMessageBox_YES_NO("Sure update HT9045 program??", "確定要手動檢查更新 HT9045 程式??");

    if(iRet==1 || bhint==false)
    {
        if(IniConfig.iN32_DownloadMode==0)
        {
            sDownsloadFile1=fFTPClient->N32_DownloadUpdateAutomatically(IniConfig.sN32_FTPDownloadPath,true);               //下載後刪除伺服器上的安裝包，Beta版少量機台更新用
            sDownsloadFile2=fFTPClient->N32_DownloadUpdateAutomatically(IniConfig.sN32_FTPDownloadPath2,false);             //下載後不刪除伺服器上的安裝包，穩定版可以放這裡資料夾

            if(FileExists(sDownsloadFile2))
                sInstallerFile=sDownsloadFile2;
            else
                sInstallerFile=sDownsloadFile1;
        }
        else
        {
            sInstallerFile=fFTPClient->N32_DownloadUpdateAutomatically();
        }

        if(FileExists(sInstallerFile))
        {
            bUpdata=false;
            if(IniConfig.iN32_DownloadMode==0)
            {
                if(GetSoftwareFileVersion(sInstallerFile)!=asHandlerVersion)    //Sam 20230328 : 改使用更新包的產品版本來判別是否更新。
                    bUpdata=true;
            }
            else
            {
                if(sInstallerFile.AnsiPos("INSTALLER")!=0)
                    bUpdata=true;
            }

            if(bUpdata)
            {
                if(bhint==false)
                    ShowMyMessage("N32 Server has a new HT9045 installation! HT9045 is about to be closed and updated", "N32 伺服器有新 HT9045 安裝包! HT9045 即將關閉程式並且更新");
                NewRecordProcess("", "Update HT9045 Automatically", sInstallerFile);
                fProductionInfo->DoRunExecutFile(sInstallerFile,sParameters);
                bUpdateAutomatically=true;
                fMain->Close();
            }
        }
        else
        {
            if(bhint)                                                           //手動更新使用
            {
                if(sInstallerFile.AnsiPos("NULL")!=0)
                    ShowMyMessage("N32 Server hasn't HT9045 installer File!.", "N32 伺服器上沒有 HT9045 安裝包!");
                else
                    ShowMyMessage("N32 Server Download fail."+sInstallerFile, "N32 伺服器下載發生錯誤!"+sInstallerFile);
            }
        }
    }
}
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N32_DownloadUpdateAutomatically(AnsiString sPatch,bool bDeleteFile)    //Sam 20220824 : FTP 自動下載安裝更新包
{
    AnsiString sResult="NULL", sServerFileName="", sTargetFileFullPath="", sListText="", sDateTime="";
    AnsiString sStep="";                                                        //Sam 20221011 : N32 修改

    if(IniConfig.iN32_DownloadMode==0)                                          //Steven 20221216 : 使用網路硬碟下載安裝包
    {
        if(IniConfig.sN32_FTPHost=="")
        {
            ShowMyMessage("N32 FTP Server Host is Empty");
            return "Error1";                                                    //Sam 20221011 : N32 修改
        }
        if(IniConfig.sN32_FTPUserName=="")
        {
            ShowMyMessage("N32 FTP Server User Name is Empty");
            return "Error2";
        }
        if(IniConfig.sN32_FTPPassword=="")
        {
            ShowMyMessage("N32 FTP Server Password is Empty");
            return "Error3";
        }
        if(sPatch=="")
        {
            ShowMyMessage("N32 FTP Server File Path is Empty");
            return "Error4";
        }

        TNMFTP *pFTP;
        pFTP=new TNMFTP(this);

        MyForceDirectories(aDownloadUpdateAutomaticallyPath);

        try
        {
            sStep="Step1";
            pFTP->Vendor    = NMOS_AUTO;
            pFTP->TimeOut   = 30000;
            pFTP->Host      = IniConfig.sN32_FTPHost;
            pFTP->UserID    = IniConfig.sN32_FTPUserName;
            pFTP->Password  = IniConfig.sN32_FTPPassword;
            pFTP->OnListItem= NMFTP1ListItem;
            pFTP->Connect();
            sStep="Step2";
            for(int i=0; i<300; i++)                                            // 300 x 10 = 1000 ( 3 Sec )
            {
                MySleep(10);
                if(pFTP->Connected==true)
                    break;
            }
            sStep="Step3";
            if(pFTP->Connected==false)
            {
                ShowMyMessage("N32 FTP Server is not connected");
                delete pFTP;
                return "Error5";
            }
            sStep="Step4";

            if(sPatch.SubString(sPatch.Length(), 1)!="/")
            {
                pFTP->ChangeDir(sPatch+"/");
                sStep="Step5";
            }
            else
            {
                pFTP->ChangeDir(sPatch);
                sStep="Step6";
            }
            bTempList=true;
            try
            {
                pFTP->Nlist();
                sStep="Step7";
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfFTPClient::N32_DownloadUpdateAutomatically1");
                ShowMyMessage("FTP Download Fail1 "+sStep);
                delete pFTP;
                pFTP=NULL;
                return sStep;
            }

            sListText=ListBox1->Items->Text.UpperCase();
            sStep="Step8";
            if(sListText.AnsiPos("INSTALLER")!=0)
            {
                for(int i=0; i<ListBox1->Items->Count; i++)
                {
                    sServerFileName=ListBox1->Items->Strings[i].UpperCase();                    //HT9045_V3.31A001.Beta.733_20220822_Installer.exe
                    sTargetFileFullPath= aDownloadUpdateAutomaticallyPath+"\\"+sServerFileName; //D:\\HT9045_Log\\DownloadUpdateAutomatically\\HT9045_V3.31A001.Beta.733_20220822_Installer.exe
                    pFTP->Download(sServerFileName, sTargetFileFullPath);
                    sStep="Step9";
                    if(bDeleteFile)
                    pFTP->Delete(sServerFileName);
                    sStep="Step10";
                    if(sResult=="NULL")                                         //將伺服器上所有的安裝包都下載下來並且刪除伺服器上的檔案，但只安裝伺服器上抓到的第一個安裝包。
                        sResult=sTargetFileFullPath;
                }
                ListBox1->Clear();
                sStep="Step11";
            }

            pFTP->Abort();
            sStep="Step12";
            pFTP->RequestCloseSocket();
            sStep="Step13";
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfFTPClient::N32_DownloadUpdateAutomatically2");
            ShowMyMessage("FTP Download Fail2 "+sStep);
            delete pFTP;
            return sStep;
        }
        delete pFTP;
    }
    else                                                                        //Steven 20221216 : 使用網路硬碟下載安裝包
    {
        FileListBox1->Directory=IniConfig.sN32_NetDownloadPath;
        FileListBox1->FileType<<ftNormal;
        FileListBox1->Update();
        for(int i=0; i<FileListBox1->Items->Count; i++)
        {
            sListText=FileListBox1->Items->Strings[i].UpperCase();
            if(sListText.AnsiPos("INSTALLER")!=0)
            {
                sServerFileName=ListBox1->Items->Strings[i].UpperCase();
                sListText=IniConfig.sN32_NetDownloadPath+"\\"+sServerFileName;
                sResult=aAutoTempureOffsetFTP+"\\"+sServerFileName;
                CopyFile(sResult.c_str(), sListText.c_str(), false);
                MySleep(100);
                DeleteFile(sListText);
                return sResult;
            }
        }
    }
    return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::Download_2DSortingList(AnsiString FtpPath, AnsiString Source, int iTimeOut)  //RogerYang 20251217 Add for timeout    //Frank 20221122 : 2DID sorting for ATK
{
    TNMFTP *NMFTP2;                                                             //Landam 用動態元件 才穩定
    NMFTP2=new TNMFTP(this);
    try
    {
        //設定參數--------
        NMFTP2->Vendor                  =NMOS_AUTO;
        NMFTP2->TimeOut                 =iTimeOut;                              //RogerYang 20251217 Add for timeout
        NMFTP2->Passive                 =true;                                  //Steven 20121020 : 實驗看看
        if(IniConfig.cN23FtpHost!="")
            NMFTP2->Host                =IniConfig.cN23FtpHost;
        NMFTP2->UserID                  =IniConfig.cN23FtpUserName;//edtUserName->Text;
        NMFTP2->Password                =IniConfig.cN23FtpPassword;//edtPassword->Text;

        //設定Event--------
        NMFTP2->OnListItem              =NMFTP1ListItem;
        NMFTP2->OnSuccess               =NMFTP1Success;
        NMFTP2->OnFailure               =NMFTP1Failure;
        NMFTP2->OnError                 =NMFTP1Error;
        NMFTP2->OnStatus                =NMFTP1Status;
        NMFTP2->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        //連線--------
        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            delay(50);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            ShowMyMessage("FTP Server is not connected","");
            delete NMFTP2;
            bError=true;
            return;
        }

        AnsiString str01, str02, str03, str04;
        AnsiString sDLFileName=Source;
        DataPath="D:\\RMS\\";
        str01=DataPath+sDLFileName;

        if(FtpPath.SubString(FtpPath.Length(), 1)!="/")
        {
            NMFTP2->ChangeDir(FtpPath + "/");
        }
        else
        {
            NMFTP2->ChangeDir(FtpPath);
        }

        NMFTP2->Download(Source, str01);

        NMFTP2->Abort();
        delay(100);
        NMFTP2->RequestCloseSocket();
        delay(500);

        FTP_DownloadFail=false;                                                 // 2013.12.03 , Joye , KYEC FTP  20140103 wei
        if(NMFTP3!=NULL)
            delete NMFTP2;
    }
    catch(...)
    {
        bError=true;
        if(NMFTP3!=NULL)
            delete NMFTP2;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfFTPClient::DoCheckPassword()                                  //Ifor 20240109 add:FTP 上傳需要帳號密碼
{
    bool bError=false;
    TStringList *PasswordList[3];
    char dest[20];
    char str[256];
    AnsiString asPwPatch="D:\\HT9045\\system\\unknown.com";
    AnsiString S="";

    if(FileExists(asPwPatch))
    {
        TfPassword *PassworfForm;
        PassworfForm=fPassword;

        for(int i=0; i<3; i++)    //Steven 20101109
            PasswordList[i]= new TStringList;

        PasswordList[0]->Clear();
        PasswordList[1]->Clear();
        PasswordList[2]->Clear();

        PasswordList[2]->LoadFromFile(asPwPatch);              //2012-01-03    Dell modify
        PassworfForm->cbUserName->Clear();              //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
        for(int i=0; i<PasswordList[2]->Count; i++)
        {
            strncpy(str, PasswordList[2]->Strings[i].c_str(), sizeof(str));

            SplitStrByDotSpaceOnly(str, dest, 20);
            PasswordList[0]->Add(dest);
            PassworfForm->cbUserName->Items->Add(dest);    //Steven 20140530 : 使用密碼本時,用下拉選單選使用者

            SplitStrByDotSpaceOnly(str, dest, 20);
            PasswordList[1]->Add(dest);
        }

        if(PasswordList[0]->Count!=0)
        {
            PassworfForm->edUserName->Text="";        // name
            if(USE_BARCODE_AS_KEYBOARD!=0)
            {
                PassworfForm->edUserName->Text=fLotInfo->edtSysOperatorID->Text;
            }
            PassworfForm->edPassword->Text="";        // password
            PassworfForm->ShowModal();

            for(int i=0; i<PasswordList[0]->Count; i++)
            {
                if(PassworfForm->edUserName->Text.UpperCase()==PasswordList[0]->Strings[i].UpperCase() &&
                   PassworfForm->edPassword->Text.UpperCase()==PasswordList[1]->Strings[i].UpperCase())
                {
                    S.sprintf("======== %s Upload %s to FTP ========", PasswordList[0]->Strings[i], edtHDWaferName->Text);
                    RecordProcess(S.c_str());
                    for(int j=0; j<3; j++)
                    {
                        PasswordList[j]->Clear();
                        delete PasswordList[j];
                    }
                    return bError;
                }
            }
            bError=true;
        }
        for(int j=0; j<3; j++)
        {
            PasswordList[j]->Clear();
            delete PasswordList[j];
        }
    }

    return bError;
}
//---------------------------------------------------------------------------
