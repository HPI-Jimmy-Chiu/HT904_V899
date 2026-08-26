//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "FixAICCD.h"

#include "cpublic.h"
#include "cmydef.h"
#include "common.h"
#include "main.h"
#include "database.h"
#include "MyKitSuck.h"
#include "aoutarm.h"
#include "aoutarm9045.h"
#include "csystem.h"
#include "cprod.h"
#include "MyMotor.h"
#include "cOffSet.h"
#include "mysensor.h"
#include "note.h"
#include "rs232.h"
#include "myQwertyKeyBoard.h"
#include "mymessbox.h"
#include "cShowBinSelect.h"
#include "uTrayEditForm.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfFixAICCD *fFixAICCD;

int iFix2AICCDTask=1;
int iAIBGAViewTask=1;
int iBGAViewInspTask=1;
int iLearnTask=1;
int iFix2AIBGACCDConntectionOkTask;
const int eAOI_BGA=3;
bool bAIBGAViewUse[MAX_ARM_Row][MAX_ARM_Col];
HTimer hAIStartDelay;
HTimer hAITimeOut;
bool bAIAOI_Fail_Unit=false;
//------------------------------------------------------------------------------
__fastcall TfFixAICCD::TfFixAICCD(TComponent* Owner)
    : TForm(Owner)
{
    for(int i=0; i<eTotalFixAOI; i++)
    {
        bEnableFix2AICCDChannelConnect[i]=true;
        SocketFixAICCDReceiveList[i]=new TStringList;
        SocketFixAICCDReceiveList[i]->Clear();
    }

    for(int i=0; i<6; i++)
        UnloadAICntNG[i]=0;

    //Sam 20210609 : Fix AOI 軟體升級
    //==>
    clntsckt_FixAOI[eFix2AIBGACCD1]= ClientSocket_FixAICCD;
    clntsckt_FixAOI[eFix2AIBGACCD2]= ClientSocket_FixAICCD2;
    edAddress[eFix2AIBGACCD1]=edFix2AICCD_Address;
    edAddress[eFix2AIBGACCD2]=edFix2AICCD_Address2;
    edPort[eFix2AIBGACCD1]=edtFix2AICCD_Port;
    edPort[eFix2AIBGACCD2]=edtFix2AICCD_Port2;
    edCMD[eFix2AIBGACCD1]=edtFix2AICCD_CMD;
    edCMD[eFix2AIBGACCD2]=edtFix2AICCD_CMD2;
    //<==
    //Sam 20210609 : Fix AOI 軟體升級

    fShow=false;
    bWait=false;
    tmrProcessFixAICCDData->Enabled=true;
    iOutArmCycleCounter=0;
    bBGAViewInspResult=false;
    bBGAViewInspRunning=false;
    iRetryConnectTimer=15;                                                      //Sam 20231116 : 新增自定義參數
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::DoIniDataToForm()
{
    edFix2AICCD_Address->Text               =HSys.asFix2BGAAICCDIP[0];
    edtFix2AICCD_Port->Text                 =HSys.asFix2BGAAICCDPort[0];
    edFix2AICCD_Address2->Text              =HSys.asFix2BGAAICCDIP[1];
    edtFix2AICCD_Port2->Text                =HSys.asFix2BGAAICCDPort[1];
    chkEnableFix2AICCD->Checked             =TestIF_File.bEnableFix2BGAAICCD;
    chkEnableLearningMode->Checked          =TestIF_File.bEnableLearningMode;
    edtlFix2AICCDStartDelay->Text           =TestIF_File.iFix2BGAAICCDStartDelay;
    edtlFix2AICCDExposureTimeOut->Text      =TestIF_File.iFix2BGAAICCDExposureTimeOut;
    edtlFix2AICCDGetResultTimeOut->Text     =TestIF_File.iFix2BGAAICCDGetResultTimeOut;
    edtlFix2AICCDAutoRetry->Text            =TestIF_File.iFix2BGAAICCDAutoRetry;
    edtOutArmCycleInsp->Text                =TestIF_File.iFix2BGAAICCDOutArmCycleInsp;
    edtInspectResultThres->Text             =TestIF_File.dInspectResultThres;
    rgResultShowType->ItemIndex             =TestIF_File.iResultShowType;       //Sam 20240325 : 新增 DamageTrayMapping 功能
    rgResultShowType->Enabled=(CheckAndReadIniDataGeneral("AICCD", "LockNoWaitAOIResult",  1)==1)?false:true;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

    if(USE_Fix_AI_CCD==0)
    {
        TestIF_File.bEnableFix2BGAAICCD      =false;
        TestIF_File.iBGALightScrPos          =0;
    }
    else
    {
        TestIF_File.bEnableFix2BGAAICCD         =ReadIniData(szDir, "Configuration", "Fix2 AI CCD Enable",      false);
        TestIF_File.iBGALightScrPos             =ReadIniData(szDir, "Configuration", "Fix2 AI CCD BGA Light Value",      50);
    }
    TestIF_File.bEnableLearningMode             =ReadIniData(szDir, "Configuration", "Fix2 AI Learning Mode",   false);
    TestIF_File.iFix2BGAAICCDStartDelay         =ReadIniData(szDir, "Configuration", "Fix2 AI CCD Start Delay",            500);
    TestIF_File.iFix2BGAAICCDExposureTimeOut    =ReadIniData(szDir, "Configuration", "Fix2 AI CCD Exposure Time Out",      1500);
    TestIF_File.iFix2BGAAICCDGetResultTimeOut   =ReadIniData(szDir, "Configuration", "Fix2 AI CCD Get Result Time Out",    1500);
    TestIF_File.iFix2BGAAICCDAutoRetry          =ReadIniData(szDir, "Configuration", "Fix2 AI CCD Auto Retry",             1);
    TestIF_File.iFix2BGAAICCDOutArmCycleInsp    =ReadIniData(szDir, "Configuration", "Fix2 AI OutArmCycleInsp",            1);
    TestIF_File.dInspectResultThres             =ReadIniData(szDir, "Configuration", "Fix2 AI InspectResultThres",         0.5);
    TestIF_File.iResultShowType                 =ReadIniData(szDir, "Configuration", "Fix2 AI iResultShowType",            2);      //Sam 20240826 : 預設值 1>2
    iRetryConnectTimer=CheckAndReadIniDataGeneral("AICCD", "RetryConnectTimer",  15);   //Sam 20231116 : 新增自定義參數
    ChangeFix2AICCDSetupFile();
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::FormShow(TObject *Sender)
{
    fShow=true;
    DoIniDataToForm();
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::FormDestroy(TObject *Sender)
{
    tmrProcessFixAICCDData->Enabled  =false;
    TimerDownFixAICCDConnect->Enabled=false;
    ClientSocket_FixAICCD->Active    =false;
    ClientSocket_FixAICCD->Close();

    for(int i=0; i<2; i++)
    {
        SocketFixAICCDReceiveList[i]->Clear();                                  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete SocketFixAICCDReceiveList[i];
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::tmrProcessFixAICCDDataTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)
    {
        return;
    }
    bTimerRun=true;

    AnsiString sPrefix, sReply="";
    AnsiString Str2, Str;
    AnsiString Data, msg="";

    int iAiSite=0, iAiStructLen=4, iAiDataLen=0;
    int iSht=0, iBin=0, iAuto=0, iSiteNo=0;
    double dAiScore=0.0;
    int iCounter;

    for(int Tag=0; Tag<eTotalFixAOI; Tag++)
    {
        if(SocketFixAICCDReceiveList[Tag]->Count>0)
        {
            sProessFixAICCDRecevieData[Tag]=SocketFixAICCDReceiveList[Tag]->Strings[0];
            Str2=StringReplace(sProessFixAICCDRecevieData[Tag], ",", "\n", TReplaceFlags()<<rfReplaceAll);     //Steven 20160401 : 解決Code有空格的問題
            SocketFixAICCDReceiveList[Tag]->Delete(0);
            if(SocketFixAICCDReceiveList[Tag]->Count==0)
                SocketFixAICCDReceiveList[Tag]->Clear();

            TStringList *sList = new TStringList();
            sList->SetText(Str2.c_str());                                       //Steven 20160401 : 解決Code有空格的問題
            sPrefix=sList->Strings[0];

            if(sPrefix=="VR")                                                   //軟體版本
            {
            }
            else if(sPrefix=="CA")                                              //關閉Vision軟體
            {
            }
            else if(sPrefix=="AP")                                              //取得路徑
            {
            }
            else if(sPrefix=="ST")                                              //設定時間
            {
            }
            else if(sPrefix=="GV")
            {
                if(atoi(sList->Strings[2].c_str())==1)                          //InspMode Mode
                {
                    if(bAICCDExposureOK==false)
                    {
                        bAICCDExposureOK=true;
                    }
                }
            }
            else if(sPrefix=="GR")                                              //GR,1,d1,d2,d3,.......
            {
                if(TestIF_File.iResultShowType==1 || TestIF_File.iResultShowType==2)  //Sam 20240325 : 新增 DamageTrayMapping 功能
                {
                    //GR,<shuttle num> <site num> <HW bin> <辨識結果(0/1)>,<shuttle num> <site num> <HW bin> <辨識結果(0/1)>,<shuttle num> <site num> <HW bin> <辨識結果(0/1)>…
                    //UnloadAINGCnt[]

                    iAiDataLen=sList->Count-2;
                    iAiSite=ChangeToFloatNonPcnt((double)(iAiDataLen), (double)(iAiStructLen));
                    if(atoi(sList->Strings[1].c_str())==1 && iAiDataLen%iAiStructLen==0 && iAiSite>0) //檢查資料需要對齊
                    {
                        iAICCDResultCnt++;
                        if(bAICCDGetResultOK==false)                            //Sam 20240325 : 新增 DamageTrayMapping 功能
                        {
                            if(iAICCDResultCnt>=iAICCDGrabCnt)
                            {
                                iAICCDGrabCnt=0;
                                iAICCDResultCnt=0;
                                bAICCDGetResultOK=true;
                            }
                        }

                        for(int i=0; i<iAiSite; i++)
                        {
                            iSht     =atoi(sList->Strings[2+i*iAiStructLen].c_str());
                            iSiteNo  =atoi(sList->Strings[3+i*iAiStructLen].c_str());
                            iBin     =atoi(sList->Strings[4+i*iAiStructLen].c_str());
                            dAiScore =atof(sList->Strings[5+i*iAiStructLen].c_str());

                            if(dAiScore<TestIF_File.dInspectResultThres)
                            {
                                if(TestIF_File.iResultShowType==2)              //Sam 20240325 : 新增 DamageTrayMapping 功能
                                {
                                    bool bFlag=false;
                                    for(int iRow=0; iRow<MAX_ARM_Row; iRow++)
                                    {
                                        for(int iCol=0; iCol<MAX_ARM_Col; iCol++)
                                        {
                                            if(OutArmSuck.iWhichSite[iRow][iCol]+1==iSiteNo &&
                                               OutArmSuck.iBinData[iRow][iCol]==iBin)
                                            {
                                                OutArmSuck.iAOIResult[iRow][iCol]=4;
                                                bFlag=true;
                                            }
                                        }
                                    }

                                    if(bFlag==false)
                                    {
                                        msg.sprintf("The AOI result cannot find the match out arm nozzle position. SiteNo:%d ; Bin:%d", iSiteNo, iBin);
                                        ShowMyMessage(msg,"");
                                    }
                                }
                                else if(TestIF_File.iResultShowType==1)
                                {
                                    iAuto=CheckBin(iBin,iSht);
                                    if(iAuto>=0 && iAuto<=5)
                                    {
                                         UnloadAICntNG[iAuto]++;
                                    }
                                }
                            }
                        }
                        fShowBinSelect->RefreshAiCnt();
                    }
                    else
                    {
                         ShowMyMessage("AI Inspect Result Error!!");
                    }
                }
                else
                {
                    if(atoi(sList->Strings[1].c_str())==1)
                    {
                        int iTotalData=sList->Count-2;
                        iCounter=0;
                        for(int i=0; i<iTotalData; i++)                         //d1,d2,d3,.......
                        {
                            if(atof(sList->Strings[i+2].c_str())<TestIF_File.dInspectResultThres)  //資料NG
                            {
                                iCounter++;
                            }
                        }

                        if(iCounter>0)  //只要有一筆資料NG就會>0
                            bBGAViewInspResult=false;
                        if(bAICCDGetResultOK==false)
                            bAICCDGetResultOK=true;
                    }
                }
            }
//            else if(dVisionVer>=2)          //新版Command
//            {
//                if(sList->Count>=2)
//                    iFunction=atoi(sList->Strings[1].c_str());
//
//                if(sList->Count>=3)
//                    iResult=atoi(sList->Strings[2].c_str());
//
//
//                //----------------------------
//                // TRAY MAPPING
//                //----------------------------
//                else if(iFunction==4)
//                {
//                    if(sPrefix=="GV")
//                    {
//                        if(sList->Count>=4)
//                        {
//                            if(bTrayMapGetResultOK[Tag]==false)
//                            {
//                                bResult=CCDTrayMap_Decoding(Tag);
//                                if(bResult)
//                                {
//                                    bTrayMapGetResultOK[Tag]=true;
//                                    sReply.sprintf("Tray Map GVOK : %s", sProessFixAICCDRecevieData[Tag]);
//                                }
//                                else
//                                {
//                                    sReply.sprintf("Tray Map GVNG : %s", sProessFixAICCDRecevieData[Tag]);
//                                }
//                            }
//                            else
//                            {
//                                sReply.sprintf("Tray Map GVTO : %s", sProessFixAICCDRecevieData[Tag]);
//                            }
//                        }
//                        else
//                        {
//                            sReply.sprintf("Tray Map GVNG : %s", sProessFixAICCDRecevieData[Tag]);
//                        }
//                    }
//                    else
//                    {
//
//                    }
//                }
//            }
            SendFix2AICCDCommand(Tag, sReply);
            sList->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sList;
        }
    }

    if(TestIF_File.iResultShowType==0)                                          //Sam 20240325 : 新增 DamageTrayMapping 功能
    {
        if(bBGAViewInspRunning==true)                                           //等待回傳結果
        {
            if(bAICCDGetResultOK==true)                                         //已完成檢測
            {
                if(bBGAViewInspResult==false)
                {
                    ShowMyMessage("AI Inspect Device=>damage,\n Please check Device!!");
                }
                bBGAViewInspRunning=false;
            }
            else if(hAITimeOut.Off())
            {
                ShowMyMessage(AnsiString().sprintf("AI Inspect Timeout Error!! Func:%s",__FUNC__));
                bBGAViewInspRunning=false;
            }
        }
    }
    bTimerRun=false;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::TimerDownFixAICCDConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    static int iCount[eTotalFixAOI]={0, 0};
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    for(int i=0;i<eTotalFixAOI;i++)
    {
        if(i==eFix2AIBGACCD1 && TestIF_File.bEnableFix2BGAAICCD==false)
            continue;
        else if(i==eFix2AIBGACCD2 && (TestIF_File.bEnableFix2BGAAICCD==false || TestIF_File.bEnableLearningMode==true))
            continue;

        if(clntsckt_FixAOI[i]->Active                      &&                   //相同的IP跟Port就不要重新連線
           clntsckt_FixAOI[i]->Address==edAddress[i]->Text &&
           clntsckt_FixAOI[i]->Port==edPort[i]->Text)
        {
        }
        else
        {
            if(clntsckt_FixAOI[i]->Active)
            {
                clntsckt_FixAOI[i]->Close();
                SendFix2AICCDCommand(i, "ClientSocket Close", "");
                iCount[i]=0;
            }
        }

        if(clntsckt_FixAOI[i]->Active==false && bEnableFix2AICCDChannelConnect[i]==true)
        {
            iCount[i]++;
            if(iCount[i]>iRetryConnectTimer)                                    //Sam 20231116 : 新增自定義參數
            {
                clntsckt_FixAOI[i]->Address   =edAddress[i]->Text;
                clntsckt_FixAOI[i]->Port      =atoi(edPort[i]->Text.c_str());
                SendFix2AICCDCommand(i, "ClientSocket connecting", "");
                try
                {
                    clntsckt_FixAOI[i]->Open();
                }
                catch(...)
                {
                    LogClientSocketExceptionError(clntsckt_FixAOI[i], "AICCDConnect");             //Steven 20231113 : 紀錄網路連線例外
                }
                SendFix2AICCDCommand(i, "ClientSocket Open", "");
                iCount[i]=0;
            }
        }
    }
    /*
    static int Count1=90;
    static int Count2=90;

    if(TestIF_File.bEnableFix2BGAAICCD)
    {
        if(ClientSocket_FixAICCD->Active                           &&  //相同的IP跟Port就不要重新連線
           ClientSocket_FixAICCD->Address==edFix2AICCD_Address->Text &&
           ClientSocket_FixAICCD->Port==edtFix2AICCD_Port->Text)
        {
        }
        else
        {
            if(ClientSocket_FixAICCD->Active)
            {
                ClientSocket_FixAICCD->Close();
                SendFix2AICCDCommand(iFix2AIBGACCD, "ClientSocket Close", "");
                Count1=0;
            }
        }

        if(ClientSocket_FixAICCD->Active==false && bEnableFix2AICCDChannelConnect[iFix2AIBGACCD]==true)
        {
            Count1++;
            if(Count1>15)
            {
                ClientSocket_FixAICCD->Address   =edFix2AICCD_Address->Text;
                ClientSocket_FixAICCD->Port      =atoi(edtFix2AICCD_Port->Text.c_str());
                SendFix2AICCDCommand(iFix2AIBGACCD, "ClientSocket connecting", "");
                try
                {
                    ClientSocket_FixAICCD->Open();
                }
                catch(...)
                {
                }
                SendFix2AICCDCommand(iFix2AIBGACCD, "ClientSocket Open", "");
                Count1=0;
            }
        }

        if(TestIF_File.bEnableLearningMode==false) //Learning Mode 不需要開啟port2
        {
            if(ClientSocket_FixAICCD2->Active                           &&  //相同的IP跟Port就不要重新連線
               ClientSocket_FixAICCD2->Address==edFix2AICCD_Address2->Text &&
               ClientSocket_FixAICCD2->Port==edtFix2AICCD_Port2->Text)
            {
            }
            else
            {
                if(ClientSocket_FixAICCD2->Active)
                {
                    ClientSocket_FixAICCD2->Close();
                    SendFix2AICCDCommand(iFix2AIBGACCD2, "ClientSocket Close", "");
                    Count2=0;
                }
            }

            if(ClientSocket_FixAICCD2->Active==false && bEnableFix2AICCDChannelConnect[iFix2AIBGACCD2]==true)
            {
                Count2++;
                if(Count2>15)
                {
                    ClientSocket_FixAICCD2->Address   =edFix2AICCD_Address2->Text;
                    ClientSocket_FixAICCD2->Port      =atoi(edtFix2AICCD_Port2->Text.c_str());
                    SendFix2AICCDCommand(iFix2AIBGACCD2, "ClientSocket connecting", "");
                    try
                    {
                        ClientSocket_FixAICCD2->Open();
                    }
                    catch(...)
                    {
                    }
                    SendFix2AICCDCommand(iFix2AIBGACCD2, "ClientSocket Open", "");
                    Count2=0;
                }
            }
        }
    }
    */
    bTimerRun=false;
}
//------------------------------------------------------------------------------
void TfFixAICCD::ChangeFix2AICCDSetupFile()
{
    iFix2AIBGACCDConntectionOkTask=1;
    scrBGALightValue->Position=TestIF_File.iBGALightScrPos;                     //打開原設定的光源
}
//------------------------------------------------------------------------------
void TfFixAICCD::AddFix2AICCDCommunicationLog(int index, AnsiString Str)
{
    AnsiString Path, Name, Log;

    if(index==eFix2AIBGACCD1)
        Name="Fix2 AI CCD Port1";
    else if (index==eFix2AIBGACCD2)
        Name="Fix2 AI CCD Port2";

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Name, Str);
    if(memoFix2AICCDCommLog->Lines->Count>1000)
        memoFix2AICCDCommLog->Clear();
    memoFix2AICCDCommLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d_%02d", asTrayMapCommLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTrayMapCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//------------------------------------------------------------------------------
void TfFixAICCD::SendFix2AICCDCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if(memoFix2AICCDCommLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    GetTimeInfo();

    if(Msg2=="")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s, %s", Msg, Msg2);

    AddFix2AICCDCommunicationLog(index, Log);
    if(index==eFix2AIBGACCD1)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_FixAICCD->Active)
            {
                //ClientSocket_FixAICCD->Socket->SendText(Msg2+"\r\n");
                ClientSocket_FixAICCD->Socket->SendText(Msg2);
            }
        }
    }
    else if(index==eFix2AIBGACCD2)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_FixAICCD2->Active)
            {
                ClientSocket_FixAICCD2->Socket->SendText(Msg2);
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::ClientSocket_FixAICCDConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    AnsiString Str;
    Str.sprintf("IP: %s, Port:%d [Connect OK]", Ptr->Address, Ptr->Port);
    SendFix2AICCDCommand(Ptr->Tag, Str, "");

    if(Ptr->Tag==eFix2AIBGACCD1 || Ptr->Tag==eFix2AIBGACCD2)
    {
        ChangeFix2AICCDSetupFile();                                             //Steven 20160425 : 重置2DSys時間
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::ClientSocket_FixAICCDDisconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    SendFix2AICCDCommand(Ptr->Tag, "DisConnect", "");
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::ClientSocket_FixAICCDError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    if(InitialOK==false)
    {
        return;
    }

    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;
    SendFix2AICCDCommand(Tag, GetErrorMsg(Sender, ErrorCode), "");
    ErrorEvent=TErrorEvent(NULL);
    ErrorCode =0;

    try
    {
        Ptr->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ClientSocket_FixAICCD Error");
    }

    Abort();
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::ClientSocket_FixAICCDRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;

    AnsiString EthernetBuffer;
    int iBufferLenght=0;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0) //13.09.30.01   klutter
    {
        EthernetBuffer=Socket->ReceiveText();
        EthernetBuffer=StringReplace(EthernetBuffer, "\r", ",", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
        EthernetBuffer=StringReplace(EthernetBuffer, "\n", "", TReplaceFlags()<<rfReplaceAll);
        SocketFixAICCDReceiveList[Tag]->Add(EthernetBuffer);

        AnsiString Log;
        Log.sprintf("Recv, , %s", EthernetBuffer);
        AddFix2AICCDCommunicationLog(Tag, Log);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::spbSaveClick(TObject *Sender)
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

    WriteIniData(szDir, "Configuration", "Fix2 AI CCD Enable",                  (chkEnableFix2AICCD->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Fix2 AI Learning Mode",               (chkEnableLearningMode->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Fix2 AI CCD Start Delay",             edtlFix2AICCDStartDelay->Text);
    WriteIniData(szDir, "Configuration", "Fix2 AI CCD Exposure Time Out",       edtlFix2AICCDExposureTimeOut->Text);
    WriteIniData(szDir, "Configuration", "Fix2 AI CCD Get Result Time Out",     edtlFix2AICCDGetResultTimeOut->Text);
    WriteIniData(szDir, "Configuration", "Fix2 AI CCD Auto Retry",              edtlFix2AICCDAutoRetry->Text);
    WriteIniData(szDir, "Configuration", "Fix2 AI OutArmCycleInsp",             edtOutArmCycleInsp->Text);
    WriteIniData(szDir, "Configuration", "Fix2 AI InspectResultThres",          edtInspectResultThres->Text);

    //光源
    WriteIniData(szDir, "Configuration", "Fix2 AI CCD BGA Light Value",        lblBGALightValue->Caption);

    WriteIniData(szDir, "Configuration", "Fix2 AI iResultShowType", rgResultShowType->ItemIndex);   //Sam 20240325 : 新增 DamageTrayMapping 功能
    ReadFile();
    spbSave->Down=false;
    fMain->BackupSetupFile();    //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::btnFix2AICCD_ConnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    TimerDownFixAICCDConnect->Enabled=true;
    bEnableFix2AICCDChannelConnect[tempBtn->Tag]=true;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::btnFix2AICCD_DisconnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    bEnableFix2AICCDChannelConnect[tempBtn->Tag]=false;
    clntsckt_FixAOI[tempBtn->Tag]->Close();
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::btFix2AICCD_TriggerClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    AnsiString strSendCMD=edCMD[tempBtn->Tag]->Text;
    SendFix2AICCDCommand(tempBtn->Tag, " ", strSendCMD);
}
//------------------------------------------------------------------------------
void TfFixAICCD::Fix2AICCDFunction()
{
    iFix2AICCDTask=1;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            OutArmSuck.iAOIStation[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
void TfFixAICCD::InitBGAViewFunction()
{
    iAIBGAViewTask=1;
}
//------------------------------------------------------------------------------
bool TfFixAICCD::DoFix2AICCDFunction()
{
        int &Task=iFix2AICCDTask;
        bool bResult=false;

        switch(Task)
        {
            case 1:
                if(TestIF_File.bEnableFix2BGAAICCD)
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            bAOIPassFail[0][i][j]=false;
                        }
                    }
                    Task=1000;
                }
//                else if(tAOISetup.tPADView.bEnabled)
//                {
//                    Task=2000;
//                }
//                else
//                {
//                    Task=4000;
//                }
                break;
            case 1000:
                InitBGAViewFunction();
                Task=1100;
                break;
            case 1100:
                if(DoBGAViewFunction())
                {
//                    if(tAOISetup.tPADView.bEnabled)
//                    {
//                        Task=2000;
//                    }
//                    else
//                    {
                        Task=4000;
//                    }
                }
                break;
//            case 2000:
//                InitDoPADViewFunction();
//                Task=2100;
//                break;
//            case 2100:
//                if(DoPADViewFunction())
//                {
//                    Task=4000;
//                }
//                break;
//            case 3000:
//                InitDoTopViewFunction();
//                Task=3100;
//                break;
//            case 3100:
//                if(DoTopViewFunction())
//                {
//                    Task=4000;
//                }
//                break;
            case 4000:
                bResult=true;
                break;
            default:
                break;
        }
        return bResult;
//    #else
//        return true;
//    #endif
}
//------------------------------------------------------------------------------
void TfFixAICCD::InitBGAViewInsp()
{
    iBGAViewInspTask=1;
}
//------------------------------------------------------------------------------
bool TfFixAICCD::DoBGAViewFunction_Inspection()
{
    int &Task=iBGAViewInspTask;
    AnsiString sCCDCommand;
    switch(Task)
    {
        case 1:        //initial
            #ifdef SOFT_SIMULTE
            return true;
            #else
            if(bBGAViewInspRunning==true)                                       //如果還在等AOI回傳結果就PASS
            {
                return true;
            }
            else
            {
                bAICCDGetResultOK=false;
                bBGAViewInspResult=true;
                Task=100;
            }
            break;
            #endif
        case 100:
            //--------------------------------------
            //GR  , 0
            //請AOI檢測並等待檢測結果
            //--------------------------------------
            bBGAViewInspRunning=true;
            sCCDCommand.sprintf("GR,1");
            SendFix2AICCDCommand(eFix2AIBGACCD2, "Getting Result... 100: ", sCCDCommand);
            hAITimeOut.SetMSAndOn(TestIF_File.iFix2BGAAICCDGetResultTimeOut);
            Task=1;
            return true;
//            break;
//        case 200:
//            if(bAICCDGetResultOK==true)    //取得結果
//            {
//                Task=400;
//            }
//            else if(hAITimeOut.Off())
//            {
//                Task=300;
//            }
//            #ifdef SOFT_SIMULTE
//            Task=400;
//            #endif
//            break
//        case 300:
//            ret=ShowErrorMessage("WAR16302", K_RETRY|K_SKIP, MMSystem);
//            if(ret==K_RETRY)
//            {
//                Task=100;     //再丟一次
//            }
//            else              //放棄
//            {
//                Task=1;
//                return true;
//            }
//            break;
//        case 400:  //判斷結果
//            if(bBGAViewInspResult==true)
//            {
//                Task=1;
//                return true;
//            }
//            else //結果有IC fail
//            {
//
//            }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TfFixAICCD::NeedToGrabImage()
{
    if(TestIF_File.bEnableLearningMode==true)
    {
        return true;
    }
    else
    {
        if(iOutArmCycleCounter>TestIF_File.iFix2BGAAICCDOutArmCycleInsp)
        {
            iOutArmCycleCounter=0;
            return true;
        }
        else
        {
            return false;
        }
    }
}
//------------------------------------------------------------------------------
bool TfFixAICCD::DoBGAViewFunction()
{
    int &Task=iAIBGAViewTask;
    bool bResult=false;
    bool bflag=false;
    AnsiString ErrPart="";
    static int iRow=0, iCol=0;

    switch(Task)
    {
        case 1:
            bAICCDGetResultOK=false;                                            //Sam 20240325 : 新增 DamageTrayMapping 功能
            iAICCDGrabCnt=0;
            iAICCDResultCnt=0;
            bAICCDExposureOK=true;
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    bAIBGAViewUse[i][j]=true;                                   //RogerYang 第一階段沒有要檢測
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_BGA &&
                       bRunAOI==false &&
                       bAIBGAViewUse[i][j])
                    {
                        iRow=i;
                        iCol=j;
                        bRunAOI=true;
                        Task=1000;
                        break;
                    }
                }
            }
            break;
        case 1000:
            if(DoMoveXY_BGAView(iRow, iCol))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    (Sen[SnRKManualStep].IsOn() ||
                     fOffSet->UseAutoOffsetFunction("BGA View")))               //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoBGAViewFunction");
                    Task=1100;
                }
                else
                {
                    InitBGAViewProcess();
                    Task=2000;
                }
            }
            break;
        case 1100:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=false;
                Task=1000;
            }
            break;
        case 2000:
//            if(TestIF_File.bEnableLearningMode)
//            {
              if(DoBGAViewFunction_LearningMode(iRow, iCol))
              {
                  Task=3000;
              }
//            }
//            else  //檢測模式未開放
//            {
//                if(DoBGAViewFunction_Inspection(iRow, iCol))
//                {
//                    Task=3000;
//                }
//            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    bAIBGAViewUse[i][j]=true;                                   //RogerYang 第一階段沒有要檢測
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_BGA &&
                       bAIBGAViewUse[i][j])
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                Task=4500;
            }
            break;
        case 4500:                                                              //檢查是否需要丟GR通知AOI檢測(目前模式:Outarm上全部IC全部都取完像再一次檢)
            if(TestIF_File.bEnableLearningMode==false)
            {
                InitBGAViewInsp();
                if(TestIF_File.iResultShowType==2)                              //Sam 20240325 : 新增 DamageTrayMapping 功能
                    hAITimeOut.SetMSAndOn(TestIF_File.iFix2BGAAICCDGetResultTimeOut);
                Task=4600;
            }
            else
            {
                Task=5000;
            }
            break;
        case 4600:
            if(TestIF_File.iResultShowType==2)                                  //Sam 20240325 : 新增 DamageTrayMapping 功能
            {
                if(bAICCDGetResultOK)                                           //第三個版本。Fix AOI 檢查後，紀錄損壞 IC 在 OutArm 哪支吸嘴上，最後將資料帶到 Unloader Tray，當 Tray 退出時顯示損壞 IC Tray Mapping。
                {
                    Task=5000;
                }
                else if(hAITimeOut.Off())
                {
                    ShowMyMessage(AnsiString().sprintf("AI Inspect Timeout Error!! Func:%s",__FUNC__));
                    Task=5000;
                }
            }
            else if(TestIF_File.iResultShowType==1)                             //Sam 20210609 : Fix AOI 軟體升級
            {
                Task=5000;                                                      //第二個版本。Fix AOI 檢查後，紀錄損壞 IC 數量在 Unloader 位置，當 Tray 退出來時才顯示損壞數量並提示人員檢查
            }
            else if(TestIF_File.iResultShowType==0)
            {
                if(DoBGAViewFunction_Inspection()==true)                        //第一個版本。Fix AOI 檢查後，有損壞 IC 直接報警提示人員檢查。
                {
                    Task=5000;
                }
            }
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TfFixAICCD::DoMoveXY_BGAView(int &iRow, int &iCol)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=dOutArmXPitch_MovePitch;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iZPos[iRow][iCol]=Prod.OutArm_BGA_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch/2*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iBGAView_X;
    iYPos=Prod.iBGAView_Y;

    iXPos=(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;                               //移到第一顆
    iYPos=iYPos+TestIF.iARM_Y_PITCH*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)                   //2013-08-02    Dell     add Pick 前先確認CheckInArmDestroyActive 已完成
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void TfFixAICCD::InitBGAViewProcess()
{
    iLearnTask=1;
}
//------------------------------------------------------------------------------
bool TfFixAICCD::DoBGAViewFunction_LearningMode(int &iRow, int &iCol)
{
    bool bResult=false;
    int &Task=iLearnTask;
    int ret;
    AnsiString sCCDCommand, S="";

    switch(Task)
    {
        case 1:
            hAIStartDelay.SetMSAndOn(TestIF_File.iFix2BGAAICCDStartDelay);
            //bAIAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hAIStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            bAICCDExposureOK=false;
            //--------------------------------------
            //GV,<setup file name>,<shuttle num> <site num>,<HW bin>            //Sam 20210609 : Fix AOI 軟體升級
            //--------------------------------------
            S=GetLastOpenFN();
            sCCDCommand.sprintf("GV,%s,%d,%d,%d,%s", S,                         //Sam 20240328 : 修正 FIXAOI 通訊錯誤
                                                     0,                         //Sam 20231108 : FixAOI 新增功能。
                                                     OutArmSuck.iWhichSite[iRow][iCol]+1,
                                                     OutArmSuck.iBinData[iRow][iCol],
                                                     IniConfig.asA32_1_HandlerID);
            iAICCDGrabCnt++;                                                    //Sam 20240325 : 新增 DamageTrayMapping 功能
            SendFix2AICCDCommand(eFix2AIBGACCD1, "Learning Exposure 2000: ", sCCDCommand);
            hAITimeOut.SetMSAndOn(TestIF_File.iFix2BGAAICCDExposureTimeOut);
            Task=2500;
        case 2500:
            if(bAICCDExposureOK==true)                                          //拍照完成
            {
                Task=4000;
            }
            else if(hAITimeOut.Off())
            {
                Task=3500;
            }
            #ifdef SOFT_SIMULTE
            Task=4000;
            #endif
            break;
        case 3500:
            ret=ShowErrorMessage("WAR16302", K_RETRY|K_SKIP, MMSystem);
            if(ret==K_RETRY)
            {
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3600:
            if(tAOISetup.tAutoSkipBGA.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16312", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_BGAView(iRow, iCol))
            {
                Task=1;                                                         //Sam 20231108 : FixAOI 拍照前要 Start Delay
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_BGA)
            {
//                if(tAOISetup.tAOINoSort.bEnabled==false)                        //wei 20161207
//                {
//                    if(bAIAOI_Fail_Unit)
//                    {
//                        if(tAOISetup.tTesterFailBin.bEnabled)
//                        {
//                            iGetBin=OutArmSuck.Item[iRow][iCol]-TEST_PASS;
//                            if(Prod.bConsFail[iGetBin]==true)
//                            {
//                                iBin=tAOISetup.tBGAPADView.iTestFailFailBin;
//                            }
//                            else
//                            {
//                                iBin=tAOISetup.tBGAPADView.iTestPassFailBin;
//                            }
//                        }
//                        else
//                        {
//                            iBin=tAOISetup.tBGAView.iFailBin;
//
//                        }
//                        CheckBin(iBin, 1);
//                        OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
//                        bAOIPassFail[0][iRow][iCol]=false;            //wei 20160616
//                    }
//                    else
//                    {
//                        bAOIPassFail[0][iRow][iCol]=true;             //wei 20160616
//                    }
//                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_BGA;
            }
            Task=5000;
            break;
        case 5000:
//            if(tAOISetup.bEnabledContinueAlarm==true)                                                    //Alick 20160715
//            {
//                if(tAOISetup.bEnabledBySiteBGAView==true)                 // bAIAOI_Fail_Unit==true &&
//                {
//                    DoContinuousFailBySiteBGAView(bAOI_Fail_Unit, iRow, iCol);
//                }
//
//                if(tAOISetup.bEnabledByArmBGAView==true)
//                {
//                    if(bPickSH1Flag==true && bPickSH2Flag==false && bBGAViewByArmDouChk[0]==false)
//                        DoContinuousFailByArmBGAView(bAIAOI_Fail_Unit, 0);
//                    else if (bPickSH1Flag==false && bPickSH2Flag==true && bBGAViewByArmDouChk[1]==false)
//                        DoContinuousFailByArmBGAView(bAIAOI_Fail_Unit, 1);
//                }
//            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::scrBGALightValueChange(TObject *Sender)
{
    if(InitialOK==false)
    {
        return;
    }
    TestIF_File.iBGALightScrPos=scrBGALightValue->Position;
    LightDataReflesh();
}
//------------------------------------------------------------------------------
void TfFixAICCD::OutArmCycleCounterUpdate()
{
    if(TestIF_File.bEnableLearningMode==false)
    {
        iOutArmCycleCounter++;
    }
}
//------------------------------------------------------------------------------
void TfFixAICCD::LightDataReflesh()
{
    lblBGALightValue->Caption=TestIF_File.iBGALightScrPos;
    bWait=false;
    LightControlUpdate();
}
//------------------------------------------------------------------------------
void TfFixAICCD::LightControlUpdate()
{
    if(bRS232Delay==false)
    {
        COM2->WriteVisionLight(TestIF_File.iBGALightScrPos, 0);
        bWait=false;
    }
    else
    {
        bWait=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::tmrLightControlTimer(TObject *Sender)
{
    if(InitialOK==false || USE_Fix_AI_CCD==0)                                   //Steven 20260423 : add protection
    {
        return;
    }

    asOutArmCycleCounter.printf("%d", iOutArmCycleCounter);
    lablFix2AICCDInspectCycleCounter->Caption=asOutArmCycleCounter;

    if(bWait==true && COM2!=NULL && COM2->bVisionLightCommOK)                   //AI(appexception-av-log-flood) 20260617 (RogerYang) : COM8 未開啟成功不送燈控，避免持續觸發 AV
    {
        LightControlUpdate();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::pnlBGALightValueDblClick(TObject *Sender)
{
    bRS232Delay=false;
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::edtInspectResultThresClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 1.0, 0.0);
}
//------------------------------------------------------------------------------
void TfFixAICCD::bCheckUnloaderHasAiNG(int iMot)
{
    AnsiString msg="";
    if(USE_Fix_AI_CCD && TestIF_File.bEnableFix2BGAAICCD )                      //Sam 20240325 : 新增 DamageTrayMapping 功能
    {
        int iTrayNum1[9]={MMAuto1, MMAuto2, MMAuto3, MMAuto1_Car, MMAuto2_Car, MMAuto3_Car, MManualTray1, MManualTray2, MManualTray3};
        AnsiString sTrayName[9]={"Auto1","Auto2","Auto3","AutoCar1","AutoCar2","AutoCar3","Fix1","Fix2","Fix3"};
        int iPos=-1;
        for(int i=0; i<9; i++)
        {
            if(iTrayNum1[i]==iMot)
                iPos=i;
        }

        if(iPos>=0)
        {
            if(TestIF_File.iResultShowType==2)
            {
                int iDamageCnt=0;
                for(int iRow=0; iRow<MOT[iMot].Tray.XItem; iRow++)
                {
                     for(int iCol=0; iCol<MOT[iMot].Tray.YItem; iCol++)
                     {
                        if(MOT[iMot].Tray.iAOIResult[iRow][iCol]==4)
                        {
                            iDamageCnt++;
                        }
                     }
                }

                if(iDamageCnt>0)
                {
                    msg.sprintf("%s AI Inspect Device=> %d damage,\n Please check Device!!", sTrayName[iPos], iDamageCnt);
                    ShowMyMessage(msg);
                    EditTray(iMot,4);
                }
            }
            else if(TestIF_File.iResultShowType==1)
            {
                int iTray=0;
                if(iPos>=3)
                    iTray=iPos-3;
                else
                    iTray=iPos;
                if(UnloadAICntNG[iTray]>0)
                {
                    msg.sprintf("%s AI Inspect Device=> %d damage,\n Please check Device!!", sTrayName[iPos], UnloadAICntNG[iTray]);
                    ShowMyMessage(msg);
                    UnloadAICntNG[iTray]=0;
                    fShowBinSelect->RefreshAiCnt();
                }
            }
        }
        else
        {
            ShowMyMessage("Unloader type not support Fix AI !!");
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::edtFix2AICCD_PortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::edtlFix2AICCDStartDelayClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10000, 0);
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::edtlFix2AICCDAutoRetryClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 0);
}
//------------------------------------------------------------------------------
void __fastcall TfFixAICCD::edtOutArmCycleInspClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 1);
}
//------------------------------------------------------------------------------
void TfFixAICCD::InitialCycleCount()                                            //Sam 20231116 : 新增自定義參數
{
    iOutArmCycleCounter=0;
    asOutArmCycleCounter.printf("%d", iOutArmCycleCounter);
    lablFix2AICCDInspectCycleCounter->Caption=asOutArmCycleCounter;
}
//------------------------------------------------------------------------------
