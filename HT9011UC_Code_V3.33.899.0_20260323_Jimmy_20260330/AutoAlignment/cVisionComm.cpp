#include "MachineDefine.h"
//---------------------------------------------------------------------------

#pragma hdrstop

#include "cVisionComm.h"

#include "cmydef.h"
#include "common.h"
#include "main.h"
#include "cAutoAlignment.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
__fastcall TVisionComm::TVisionComm(AnsiString asInputCameraID)
{
    asCameraID=asInputCameraID;
    SocketReceiveList=new TStringList;
    SocketReceiveList->Clear();
    clntsckt=new TClientSocket(NULL);
    memoCommLog=new TMemo(this);
    bEnableVision=true;
    iCount=0;
}
//---------------------------------------------------------------------------
__fastcall TVisionComm::~TVisionComm()
{
    SocketReceiveList->Clear();
    delete SocketReceiveList;
    delete clntsckt;
    delete memoCommLog;
}
//---------------------------------------------------------------------------
void TVisionComm::Connect()
{
    if(clntsckt->Active ==true  &&  //相同的IP跟Port就不要重新連線
       clntsckt->Address==asIP  &&
       clntsckt->Port   ==iPort &&
       bEnableVision    ==true)
    {

    }
    else
    {
        if(clntsckt->Active)
        {
            clntsckt->Close();
            SendCommand("ClientSocket Close", "");
            iCount=0;
        }
    }

    if(clntsckt->Active==false && bEnableVision==true)
    {
        iCount++;
        if(iCount>15)
        {
            clntsckt->Address   =asIP;
            clntsckt->Port      =iPort;
            SendCommand("ClientSocket connecting..", "");
            try
            {
                clntsckt->Open();
            }
            catch(...)
            {

            }
            SendCommand("ClientSocket Open", "");
            iCount=0;
        }
    }
}
//---------------------------------------------------------------------------
void TVisionComm::ChangeSetupFile()
{
    iConnectionOkTask=1;
}
//---------------------------------------------------------------------------
void TVisionComm::ConnectOkInitial()
{
    AnsiString Str;

    int &Task=iConnectionOkTask;

    if(clntsckt->Active ==true  &&  //相同的IP跟Port就不要重新連線
       clntsckt->Address==asIP  &&
       clntsckt->Port   ==iPort &&
       bEnableVision    ==true)
    {
        switch(Task)
        {
            case 1:
                dVisionVer=1;
                Str.sprintf("VR");
                SendCommand("Get Version", Str);     //Reset CCD Shuttle暫存器
                bGetVR=false;
                tCCDChangeSetupFile.SetSecAndOn(30);
                Task=100;
            case 100:
                if(bGetVR==true)
                {
                    Task=1000;
                }
                else if(tCCDChangeSetupFile.Off())
                {
                        Task=1;
                }
                else
                {
                    break;
                }
            case 1000:  //Reset CCD Shuttle暫存器
                bGetSE9=false;
                Str.sprintf("E9,4");
                SendCommand("Clear buffer", Str);
                tCCDChangeSetupFile.SetSecAndOn(10);
                Task=1050;
            case 1050:
                if(bGetSE9)
                {
                    Task=1100;
                }
                else if(tCCDChangeSetupFile.Off())
                {
                    Task=1000;
                }
                else
                {
                    break;
                }
            case 1100:
                bGetGF=false;
                SendCommand("Get File Name", "GF,7");
                tCCDChangeSetupFile.SetSecAndOn(10);
                Task=1150;
            case 1150:
                if(bGetGF)
                {
                    if(fMain->cbSetupFileName->Text.UpperCase().AnsiCompare(sCurrentGF)==0)
                    {
                        Task=3000;
                        bGetLF=true;
                    }
                    else
                        Task=1200;
                }
                else if(tCCDChangeSetupFile.Off())
                {
                    Task=1100;
                }
                else
                {
                    break;
                }
            case 1200:
                bGetLF=false;
                SendCommand("Change File", "LF,7,"+fMain->cbSetupFileName->Text);
                tCCDChangeSetupFile.SetSecAndOn(10);
                Task=1250;
            case 1250:
                if(bGetLF)
                {
                    Task=3000;
                }
                else if(tCCDChangeSetupFile.Off())
                {
                    Task=1200;
                }
                break;
            case 3000:
                break;
        }
    }
}
//---------------------------------------------------------------------------
void TVisionComm::ProcessGetData()
{
    int iMatchType;
    int iFunction;
    AnsiString asRecevieData,asFixRecevieData,asLog,sPrefix,sReply="";
    int iStatus;

    if(SocketReceiveList->Count>0)
    {
        asRecevieData=SocketReceiveList->Strings[0];
        SocketReceiveList->Delete(0);
        if(SocketReceiveList->Count==0)
        {
            SocketReceiveList->Clear();
        }
        asFixRecevieData=StringReplace(asRecevieData, ",", "\n", TReplaceFlags()<<rfReplaceAll);     //Steven 20160401 : 解決Code有空格的問題

        TStringList *sList = new TStringList();
        sList->SetText(asFixRecevieData.c_str());                                       //Steven 20160401 : 解決Code有空格的問題
        sPrefix=sList->Strings[0];

        if(sPrefix=="VR")               //軟體版本  Sample : VR,2.01         [Version Number]
        {
            bGetVR=true;
            if(sList->Count>=2)
            {
                dVisionVer=atof(sList->Strings[1].c_str());
            }
            else
            {
                dVisionVer=1;
            }
            sReply.sprintf("VROK : %s",  asRecevieData);
        }
        else if(sPrefix=="CA")          //關閉Vision軟體     Sample : CA,1   [0:Err, 1:Complete]
        {

        }
        else if(sPrefix=="AP")          //取得路徑
        {

        }
        else if(sPrefix=="ST")          //設定時間         Sample : ST,1    [0:Err, 1:Complete]
        {
            if(sList->Count>=2)
                iStatus=atoi(sList->Strings[1].c_str());
            bGetST=true;
            sReply.sprintf("%s %s, %s",   sPrefix, (iStatus==1)?"OK":"NG",  asRecevieData);
        }
        else if(dVisionVer>=1)          //新版 Command
        {
            //E1,7,iResult,iCCD,iMatchType,iPixelx,iPixely 。Sample : E1,7,1,1,0,10,-10
            if(sList->Count>=2)
                iFunction=atoi(sList->Strings[1].c_str());  //取得 Function [0:2D-Code, 1:IC Check, 2:OCR, 3:TrayID, 4:TrayMap, 5:IC Remain, 6:IC Count, 7:Alignment]

            if(sList->Count>=3)
                iStatus=atoi(sList->Strings[2].c_str());    //取得狀態  [0:Err, 1:Pass, 2:取像未完成, 3:殘料檢出]

            //----------------------------
            // Auto Alignment
            //----------------------------
            if(iFunction==7)//Alignment
            {
                sReply.sprintf("%s Alignment %s %s, %s",asCameraID, sPrefix, (iStatus==1)?"OK":"NG", asRecevieData);
                if(sPrefix=="E1")   //取得拍照結果 0:CCD Error 1:Complete 2:Can not find 3:Type Different
                {
                    if(iStatus!=1)
                    {
                        asLog.sprintf("%s Aignment Alarm, %s result error!",asCameraID, sPrefix);
                        AddCommandLog(asLog);
                        iResult=iStatus;
                    }
                    else
                    {
                        if(iResult==0)
                        {
                            if(sList->Count>=7)
                            {
                                iMatchType=atoi(sList->Strings[4].c_str());
                                iResult=1;
                                if(iMatchType==1)
                                {
                                    dCenterOffsetPixelX=-atof(sList->Strings[5].c_str());
                                    dCenterOffsetPixelY=-atof(sList->Strings[6].c_str());
                                }
                                else
                                {
                                    dCenterOffsetPixelX=atoi(sList->Strings[5].c_str());
                                    dCenterOffsetPixelY=atoi(sList->Strings[6].c_str());
                                }
                            }
                            else
                            {
                                iResult=4;
                                ShowMyMessage("Receive Vision Error");
                            }
                        }
                    }
                }

                else if(sPrefix=="LI")          //live
                {
                    if(iStatus==1)
                        asStatus="Live";
                }
                else if(sPrefix=="E9")          //Reset全部資料
                    bGetSE9=true;
                else if(sPrefix=="GS")          //取得狀態
                    ;
                else if(sPrefix=="CF")          //產生工作檔資料
                    bGetCF=true;
                else if(sPrefix=="LF")          //設定工作檔
                    bGetLF=true;
                else if(sPrefix=="FL")          //取得檔案列表
                            ;
                else if(sPrefix=="GF")          //取得工作檔名
                {
                    if(sList->Count>=4)
                        sCurrentGF=sList->Strings[3];
                    else
                        sCurrentGF="";
                    bGetGF=true;
                }
                else if(sPrefix=="SF")          //存檔
                    ;
            }
        }
        SendCommand(sReply);
        sList->Clear();
        delete sList;
    }
}
//---------------------------------------------------------------------------
void TVisionComm::SendCommand(AnsiString Msg, AnsiString Msg2)
{
    AnsiString asLog,asAddLog,asFolderPath,asFilePath;
    if(InitialOK==false)
        return;

    if(Msg2=="")
    {
        asLog.sprintf("Action, %s", Msg);
    }
    else
    {
        asLog.sprintf("Send, %s, %s", Msg, Msg2);
    }

    AddCommandLog(asLog);

    if(Msg2=="")
    {

    }
    else
    {
        if(clntsckt->Active)
        {
            clntsckt->Socket->SendText(Msg2+"\r\n");
        }
    }
}
//---------------------------------------------------------------------------
void TVisionComm::AddCommandLog(AnsiString Message)
{
    AnsiString asCommLogPath="D:\\HT9045_Log\\AutoAlignComm";
    AnsiString asLog,asFolderPath,asFilePath;

    asLog.sprintf("%02d:%02d:%02d:%03d %s",SystemHour,SystemMin,SystemSec,SystemMSec,Message);
    if(fAutoAlignment->mmoCommLog!=NULL)
    {
        if(fAutoAlignment->mmoCommLog->Lines->Count>1000)
            fAutoAlignment->mmoCommLog->Clear();
        fAutoAlignment-> mmoCommLog->Lines->Add(asLog);
    }

    asFolderPath.sprintf("%s\\%04d_%02d_%02d", asCommLogPath, SystemYear, SystemMonth, SystemDate);
    if(!DirectoryExists(asFolderPath))
        ForceDirectories(asFolderPath);

    asFilePath.sprintf("%s\\%04d_%02d_%02d_%02d.txt", asFolderPath, SystemYear, SystemMonth, SystemDate, SystemHour);
    WriteDataToFile(asFilePath.c_str() , asLog.c_str());
}
//---------------------------------------------------------------------------



