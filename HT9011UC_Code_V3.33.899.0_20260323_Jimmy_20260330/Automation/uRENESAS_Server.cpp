//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "uRENESAS_Server.h"

//#include "LastSet.h"
#include "csystem.h"
//#include "cpublic.h"
#include "cmydef.h"
#include "common.h"
//#include "DateUtils.hpp"
#include "SCK_ART.h"
#include "Main.h"
#include "uLotInfo.h"
#include "cSortCT.h"
#include "mymessbox.h"
#include "cSocket.h"
#include "ckernel.h"
//---------------------------------------------------------------------------
//#include <dir.h>                                                                //Kirin 20201023 (Marvin) 使用fnsplit
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TRENESAS_Server::TRENESAS_Server()
{
    ServerSocket=new TServerSocket(NULL);
    ServerSocket->OnListen=OnProcSocketListen;
    ServerSocket->OnClientRead=OnProcSocketRead;
    ServerSocket->OnClientWrite=OnProcSocketWrite;
    ServerSocket->OnClientError=OnProcSocketError;
    ServerSocket->OnClientConnect=OnProcSocketConnect;
    ServerSocket->OnClientDisconnect=OnProcSocketDisconnect;
    iListenPort=50282;
    iProcTask=1;
    cLineEndData[0]=0xD;
    cLineEndData[1]=0xA;
    FLogMemo=NULL;
    ServerFrm=new TfServerFrm(NULL);
//    ServerFrm->frameProdInfo1->lbledtLotNo->Text        =LastSet->asRENESASLotNo;
//    ServerFrm->frameProdInfo1->lbledtLotID->Text        =LastSet->asRENESASLotID;
//    ServerFrm->frameProdInfo1->lbledtHdParameter->Text  =LastSet->asRENESASHdParameter;
//    ServerFrm->frameProdInfo1->lbledtProduct->Text      =LastSet->asRENESASProduct;
//    ServerFrm->frameProdInfo1->lbledtStepCode->Text     =LastSet->asRENESASStepCode;
//    ServerFrm->frameProdInfo1->lbledtQty->Text          =LastSet->asRENESASQty;
//    ServerFrm->frameProdInfo1->lbledtDate->Text         =LastSet->asRENESASLotStartTime;
    //LogMemo=ServerFrm->mmoHistory;

    InitParamter();
    //<==
    //Kirin 20220119 變數初始化整理到InitParamter
    ProcTimer=new TTimer(NULL);
    ProcTimer->Interval=10;
    ProcTimer->Enabled=true;
    ProcTimer->OnTimer=OnProcTimer;
    InitErrData();                                                              //Kirin 20191218 InitErrData
    FLogSavePath="D:\\TempLog\\SocketData\\ServerData\\ServerSocket.TXT";
    iFTCTMagazineAlarmShow=0;                                                   //Kirin 20200220 Show Magazine Alarm

}
//---------------------------------------------------------------------------
TRENESAS_Server::~TRENESAS_Server()
{
    delete ProcTimer;
    ProcTimer=NULL;
    delete ServerSocket;
    ServerSocket=NULL;
    delete ServerFrm;
    ServerFrm=NULL;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::Listen()
{
    ServerSocket->Active=false;
    ServerSocket->Port=iListenPort;
    ServerSocket->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketListen(System::TObject* Sender, TCustomWinSocket* Socket)
{

}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketRead(System::TObject* Sender, TCustomWinSocket* Socket)
{
    AnsiString asMsg;
    AnsiString asHexData="";
    char cbuf[1024];
    int iLen;
    int iRealLen;
    int iIndex;
    memset(&cbuf[0],0,sizeof(cbuf));
    iLen=Socket->ReceiveLength();
    iRealLen=Socket->ReceiveBuf(&cbuf[0],iLen);
    DoProcRecvData(iRealLen,&cbuf[0]);
    for(iIndex=0;iIndex<iRealLen;iIndex++)
    {
        asHexData=asHexData+" "+AnsiString(IntToHex(cbuf[iIndex],2));
    }
    asMsg.sprintf   ("Recv iLen=%d\n"     ,iLen);
    asMsg.cat_printf("Recv iRealLen=%d\n" ,iRealLen);
    asMsg.cat_printf("Recv data=%s\n"     ,cbuf);
    asMsg.cat_printf("Recv Hexdata=%s\n"      ,asHexData.c_str());
    asMsg.cat_printf("=======================================================\n");
    AppendToMemo(asMsg);
}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketWrite(System::TObject* Sender, TCustomWinSocket* Socket)
{

}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketError(System::TObject* Sender, TCustomWinSocket* Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorCode=0;                                                                //Kirin 20320120
    SaveTaskLog("OnProcSocketError", -1);                                       //Kirin 20220127 Add Log
}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketConnect(System::TObject* Sender, TCustomWinSocket* Socket)
{
    SaveTaskLog("OnProcSocketConnect", -1);                                     //Kirin 20220127 Add Log
}
//---------------------------------------------------------------------------
//void __fastcall TRENESAS_Server::OnProcSocketConnecting(System::TObject* Sender, TCustomWinSocket* Socket)
//{
//
//}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcSocketDisconnect(System::TObject* Sender, TCustomWinSocket* Socket)
{
    if(iHaveRecvTestEnd==1)
    {
//        iHaveRecvTestEnd=0;                                                   //RogerYang 20251020 : mark, avoid 70 hangup when return FTCT error
        b70_Disconnect=true;                                                    //RogerYang 20251106 : 啟用
    }
    SaveTaskLog("OnProcSocketDisconnect", -1);                                  //Kirin 20220127 Add Log
}
//---------------------------------------------------------------------------
bool TRENESAS_Server::GetClientConnect()
{
    bool result=false;
    if(ServerSocket!=NULL)
    {
        if(ServerSocket->Socket->ActiveConnections>0)
            result=true;
    }
    return result;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::AppendToMemo(AnsiString asMsg)
{
    //if(FLogMemo!=NULL)
    {
        AnsiString Path, s="";
        GetTimeInfo();
        s.sprintf("%s %02d:%02d:%02d.%03d %s",Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec,asMsg);   //Sam 20230420 : 時間紀錄到小數點
        //FLogMemo->Lines->Add(s);

        Path.sprintf("%s\\%04d_%02d_%02d", asRenesasCommPath, SystemYear, SystemMonth, SystemDate);
        if(!DirectoryExists(Path))
            ForceDirectories(Path);
        Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asRenesasCommPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
        WriteDataToFile(Path.c_str(), s);
    }
}
//---------------------------------------------------------------------------
void TRENESAS_Server::DoProcRecvData(int iLen,char* cbuf)
{
    char TempBuf[1024];
    AnsiString asTempSourceData;
    AnsiString asTempCMD;
    AnsiString asLogMsg;
    memset(&TempBuf[0],0,1024);
    memcpy(&TempBuf[0],&cbuf[0],iLen);
    asTempSourceData=TempBuf;
    for(int i=0;i<2;i++)
        asTempCMD=asTempCMD+AnsiString(cbuf[i]);
    if(asTempCMD==RENESAS_CMD.OnLineCMD)                                        //00
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        RecvData.OnLine.CMD           =asTempCMD;
        RecvData.OnLine.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"        , RecvData.OnLine.CMD           );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.ProductInfoCMD)                              //10 //INPUTQTY //HTSET,702,LotCnt,LotID
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.ProdInfo.CMD           =asTempCMD;
        RecvData.ProdInfo.LotNo         =GetFieldData(asTempSourceData,"LotNo"      );
        RecvData.ProdInfo.LotID         =GetFieldData(asTempSourceData,"LotID"      );
        RecvData.ProdInfo.HdParameter   =GetFieldData(asTempSourceData,"HdParameter");
        RecvData.ProdInfo.Product       =GetFieldData(asTempSourceData,"Product"    );
        RecvData.ProdInfo.StepCode      =GetFieldData(asTempSourceData,"StepCode"   );
        RecvData.ProdInfo.Qty           =GetFieldData(asTempSourceData,"Qty"        );
        RecvData.ProdInfo.Date          =GetFieldData(asTempSourceData,"Date"       );
        RecvData.ProdInfo.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"        ,RecvData.ProdInfo.CMD           );
        asLogMsg.cat_printf("LotNo:%s\n"      ,RecvData.ProdInfo.LotNo         );
        asLogMsg.cat_printf("LotID:%s\n"      ,RecvData.ProdInfo.LotID         );
        asLogMsg.cat_printf("HdParameter:%s\n",RecvData.ProdInfo.HdParameter   );
        asLogMsg.cat_printf("Product:%s\n"    ,RecvData.ProdInfo.Product       );
        asLogMsg.cat_printf("StepCode:%s\n"   ,RecvData.ProdInfo.StepCode      );
        asLogMsg.cat_printf("Qty:%s\n"        ,RecvData.ProdInfo.Qty           );
        asLogMsg.cat_printf("Date:%s\n"       ,RecvData.ProdInfo.Date          );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.StartCMD)                                    //20((00|10|20))  //LOTCLEAR?+LOTORDER0/LOTORDER2/LOTORDER2  //HTSET,700+HTSET,703/HTSET,704/HTSET,704
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.ModeStart.CMD          =asTempCMD;
        RecvData.ModeStart.Status       =GetFieldData(asTempSourceData,"Status"     );
        RecvData.ModeStart.Date         =GetFieldData(asTempSourceData,"Date"       );
        RecvData.ModeStart.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"    ,RecvData.ModeStart.CMD          );
        asLogMsg.cat_printf("Status:%s\n" ,RecvData.ModeStart.Status       );
        asLogMsg.cat_printf("Date:%s\n"   ,RecvData.ModeStart.Date         );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.HandlerStartCMD)                             //50
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.HandlerStart.CMD   =asTempCMD;
        RecvData.HandlerStart.Date  =GetFieldData(asTempSourceData,"Date");
        RecvData.HandlerStart.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"    ,RecvData.HandlerStart.CMD   );
        asLogMsg.cat_printf("Date:%s\n"   ,RecvData.HandlerStart.Date  );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.EndCMD)                                      //30
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.ModeEnd.CMD    =asTempCMD;
        RecvData.ModeEnd.Status =GetFieldData(asTempSourceData,"Status"     );
        RecvData.ModeEnd.Date   =GetFieldData(asTempSourceData,"Date"       );
        RecvData.ModeEnd.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"    ,RecvData.ModeEnd.CMD    );
        asLogMsg.cat_printf("Status:%s\n" ,RecvData.ModeEnd.Status );
        asLogMsg.cat_printf("Date:%s\n"   ,RecvData.ModeEnd.Date   );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.LotEndCMD)                                   //40
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.LotEnd.CMD     =asTempCMD;
        RecvData.LotEnd.LECD    =GetFieldData(asTempSourceData,"LECD"       );
        RecvData.LotEnd.Date    =GetFieldData(asTempSourceData,"Date"       );
        RecvData.LotEnd.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"    ,RecvData.LotEnd.CMD     );
        asLogMsg.cat_printf("LECD:%s\n"   ,RecvData.LotEnd.LECD    );
        asLogMsg.cat_printf("Date:%s\n"   ,RecvData.LotEnd.Date    );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.TestEndCMD)                                  //70
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.TestEnd.CMD     =asTempCMD;
        RecvData.TestEnd.Date    =GetFieldData(asTempSourceData,"Date"       );
        RecvData.TestEnd.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"    ,RecvData.TestEnd.CMD     );
        asLogMsg.cat_printf("Date:%s\n"   ,RecvData.TestEnd.Date    );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.SendAlarmCMD)                                //90
    {
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.SendAlarm.CMD=asTempCMD;
        RecvData.SendAlarm.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"        ,RecvData.SendAlarm.CMD           );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
    else if(asTempCMD==RENESAS_CMD.SendCleanOutCMD)
    {
        bRenesasFTCTAlarm=false;    //解90的alarm
        CloseContinueMessageForm();                                             //RogerYang 20251107 : Add Continue Form
        RecvData.SendCleanOut.CMD=asTempCMD;
        RecvData.SendCleanOut.iRecv=1;

        asLogMsg.sprintf(   "CMD:%s\n"        ,RecvData.SendCleanOut.CMD           );
        asLogMsg.cat_printf("=======================================================\n");
        AppendToMemo(asLogMsg);
    }
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetFieldData(AnsiString asSource,AnsiString asFieldName)
{
    int iFind1=-1;
    int iFind2=-1;
    AnsiString asResultData="NoFind";
    AnsiString asFindData="NoFind";
    AnsiString asEndData;
    iFind1=asSource.Pos(asFieldName);
    if(iFind1>0)
    {
        asFindData=asSource.SubString(iFind1+asFieldName.Length()+1,asSource.Length());
        iFind2=asFindData.Pos(char(0xD)); //Kirin 20191218 0xA=>0XD
        if(iFind2>0)
        {
            asResultData=asFindData.SubString(1,iFind2-1);
        }
    }
    return asResultData;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::DoProcess()
{
    static bool bProc=false;
    static int iManualStartTmp;
    if(bProc==true)
        return;
    bProc=true;
    AnsiString sData[4];
    AnsiString sData1="", sData2="", sData3="", sData4="", szDir="";
    //AnsiString sLastFTCTComm="", sLastFTCTCommSuccess="";
    //int iRet=0;
    int &Task=iProcTask;
    AnsiString firstStr1="", firstStr2="";
    bool flag=false;

    switch(Task)
    {
        case 1:
        {
            if(RecvData.OnLine.iRecv==1)
            {
                Task=10000;
                SaveTaskLog("DoProcess_OnLineiRecv",Task); //Kirin 20220127 Add Log
            }
            else if(RecvData.ProdInfo.iRecv==1)
            {
                Task=20000;
                SaveTaskLog("DoProcess_ProdInfoiRecv",Task); //Kirin 20220127 Add Log
            }
            else if(RecvData.ModeStart.iRecv==1)
            {
                Task=30000;
                SaveTaskLog("DoProcess_ModeStartiRecv",Task); //Kirin 20220127 Add Log
            }
            else if(RecvData.HandlerStart.iRecv==1)
            {
                Task=40000;
                SaveTaskLog("DoProcess_HandlerStartiRecv",Task); //Kirin 20220127 Add Log
            }
            else if(RecvData.ModeEnd.iRecv==1)
            {
                Task=50000;
                SaveTaskLog("DoProcess_ModeEndiRecv",Task); //Kirin 20220127 Add Log
            }
            else if(RecvData.LotEnd.iRecv==1)
            {
                Task=60000;
                SaveTaskLog("DoProcess_LotEndiRecv",Task); //Kirin 20220127 Add Log
            }
            //Kirin 20220118 TestEnd
            //==>
            else if(RecvData.TestEnd.iRecv==1)
            {
                Task=70000;
                SaveTaskLog("DoProcess_TestEndiRecv",Task); //Kirin 20220127 Add Log
            }
            //<==
            //Kirin 20220118 TestEnd
            //Kirin 20211020 SendAlarm
            //==>
            else if(RecvData.SendAlarm.iRecv==1)
            {
                Task=80000; //Kirin 20220118 70000=>80000
                SaveTaskLog("DoProcess_SendAlarmiRecv",Task); //Kirin 20220127 Add Log
            }
            //<==
            //Kirin 20211020 SendAlarm
            //Kirin 20111121 SendCleanOut
            //==>
            else if(RecvData.SendCleanOut.iRecv==1)
            {
                Task=90000;
                SaveTaskLog("DoProcess_SendCleanOutiRecv",Task);
            }
            //<==
            //Kirin 20211020 SendCleanOut
        }
        break;
        //Kirin 20191213 Proc OnLine
        //==>
        //============================================================================
        // FTCT Send to Hander 00
        //============================================================================
        case 10000:
        {
            ServerFrm->asFTCTLastSeqStatus="00";                                //Kirin 20200207 增加最後送出狀態來看
            RecvData.OnLine.iRecv=0;
            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
               if(ServerFrm->asHandlerLastSeqStatus!=RENESAS_CMD.StartReplyCMD          && //當前狀態不為 21 51 71 時，鎖定start
                  ServerFrm->asHandlerLastSeqStatus!=RENESAS_CMD.HandlerStartReplyCMD   &&
                  ServerFrm->asHandlerLastSeqStatus!=RENESAS_CMD.TestEndReplyCMD)
                {
                    bRenFTCTManualStartTmp=false;
                    fSCKART->iManualStart=false;
                    fSCKART->AccessFile(false, 1);
                }
                iErrInt=RENESAS_Err_Index.OK;                                   //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK //Kirin 20191219 must be accapt
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;                         //Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT //Kirin 20191219 must be accapt
                //SW[SwMusic2].On();
            }
            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);

            if(iErrInt==0)
                iOnLineStatus=1;
            else
                iOnLineStatus=0;

            SendOnLine(iErrInt);
            Task=1;
            SaveTaskLog("DoProcess_00_SendOnLine", Task);                                 //Kirin 20220127 Add Log
        }
        break;

        //<==
        //Kirin 20191213 Proc OnLine
        //Kirin 20191218 FTCT
        //==>
        //============================================================================
        // FTCT Send to Hander 20
        //============================================================================
         case 20000:
        {
            ServerFrm->asFTCTLastSeqStatus="10";                                //Kirin 20200207 增加最後送出狀態來看
            RecvData.ProdInfo.iRecv=0;
            bReturn41Flag=false;

            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
                if(bCanNotUse_Cmd_With_TestEnd(ServerFrm->asFTCTLastSeqStatus))
                {
                    Task=1;
                    SaveTaskLog("DoProcess_10_CanNotUse_Cmd_With_TestEnd",Task); //Kirin 20220127 Add Log
                    break;
                }
                if(CheckInputCommand(ServerFrm->asFTCTLastSeqStatus)==false)
                {
                    iErrInt=6;                                                  //"Out of order"
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendProductInfo(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_10_Out of order",Task);              //Kirin 20220127 Add Log
                    break;
                }

                //========sData[0]=="HTSET" && sData[1]=="702" (INPUTQTY)========
                if(HasICUnderMachine()==false && HasAnyICInMachine()==false)    //也要檢查 Auto tray
                {
                    //切換工作檔
                    fMain->EnabledSetupFile(false);
                    if(fMain->cbSetupFileName->Text!=RecvData.ProdInfo.HdParameter) //工作檔不一樣才切換
                    {
                        for (int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
                        {
                            if (fMain->cbSetupFileName->Items->Strings[i].AnsiCompareIC(RecvData.ProdInfo.HdParameter)==0)  // 不分大小寫比較
                            {
                                flag=true;
                                break;
                            }
                        }
                        if(flag==false)                                         //"The Work file not found."
                        {
                            iErrInt=15;
                            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                            SendProductInfo(iErrInt);
                            Task=1;
                            SaveTaskLog("DoProcess_10_The Work file not found",Task);
                            break;
                        }
                        fMain->iHasChangeFile=9;
                        fMain->cbSetupFileName->Text=RecvData.ProdInfo.HdParameter;
                        fMain->cbSetupFileNameChange(fMain);
                        if(fMain->iHasChangeFile==1)
                        {
                            Task=23000;
                            SaveTaskLog("DoProcess_10_ChangeFileSuccess",Task);
                        }
                        else
                        {
                            iErrInt=RENESAS_Err_Index.CanNotLoadWorkFile;       //"Can not load work file"
                            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                            SendProductInfo(iErrInt);
                            Task=1;
                            SaveTaskLog("DoProcess_10_Can not load work file",Task);
                        }
                    }
                    else
                    {
                        Task=23000;
                    }
                }
                else
                {                                                               //有IC的情況下
                    iErrInt=RENESAS_Err_Index.HandlerHaveDevice;                //"Handler have device"
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendProductInfo(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_10_HandlerHaveDevice",Task);
                }
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;                         //Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
                iHaveProdInfoStart=false;                                       //Kirin 20191220 FTCT
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);                //Kirin 20220127 整理程式
                SendProductInfo(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_10_DisiableFTCT",Task);
            }
            break;
        }
        case 23000:
        {
            ServerFrm->frameProdInfo1->lbledtLotNo ->Text=RecvData.ProdInfo.LotNo;
            ServerFrm->frameProdInfo1->lbledtLotID->Text=RecvData.ProdInfo.LotID;
            ServerFrm->frameProdInfo1->lbledtHdParameter->Text=RecvData.ProdInfo.HdParameter;
            ServerFrm->frameProdInfo1->lbledtProduct->Text=RecvData.ProdInfo.Product;
            ServerFrm->frameProdInfo1->lbledtStepCode->Text=RecvData.ProdInfo.StepCode;
            ServerFrm->frameProdInfo1->lbledtQty->Text=RecvData.ProdInfo.Qty;
            ServerFrm->frameProdInfo1->lbledtDate->Text=RecvData.ProdInfo.Date;

            if(RecvData.ProdInfo.StepCode[1]=='F')
            {
                //Handler在從FT-CT接收到的工序代碼的開頭為F的情況下
                //-將事先輸入的數值原封不動地設為有效
                //-如果事先沒有輸入，則對條件設定請求應答錯誤(NoInputQty, errcodr=9)
                //-通過錯誤的註釋，發送輸入投入數的內容（顯示在FT-CT畫面上）
                if(fSCKART->iLotCount==0)
                {
                    iErrInt=RENESAS_Err_Index.NoInputQty;
                    //iHaveProdInfoStart=true;
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendProductInfo(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_20_NoInputQty",Task);
                    break;
                }
                else
                {


                }

            }
            else
            {
                //F以外的情況下，輸入了投入數時
                //-將事先輸入的數值原封不動地設為有效
                //-在事先沒有輸入的情況下，在條件設定請求中覆寫從 FT-CT送來的數值
                if(fSCKART->iLotCount==0)
                {
                    fSCKART->iLotCount=StrToInt(RecvData.ProdInfo.Qty);
                }
                else
                {
                    fSCKART->iLotCount=StrToInt(RecvData.ProdInfo.Qty); //不管怎樣都先蓋過去
                }
            }
            if(fSCKART->iInputCount==0)
                fSCKART->iInputCount=fSCKART->iLotCount;
            fSCKART->iCurrent93KARTStep=1;
            fSCKART->iCurrentFlexARTStep=4;
            fMain->Clarn_Data(1, "ART_INPUTQTY");
            RecordProcess("ART INPUTQTY.");

            EndFileData.InitData();

//            if(bQAModeFlag==true)
//            {
//                fSCKART->iLotCount  =TestIF_File.iQAModeCount;
//                fSCKART->iInputCount=TestIF_File.iQAModeCount;
//                fSCKART->AccessFile(false, 1);                                  //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料
//                SetRunStartMode(rsmInitial_ART);
//                bQAModeFlag=false;
//            }
//            else
//            {


            fSCKART->sLotID.sprintf("%s", RecvData.ProdInfo.LotID);
            fSCKART->sProcessCode.sprintf("%s", RecvData.ProdInfo.StepCode);
            bRenFTCTManualStartTmp=false;
            fSCKART->iManualStart=false;
            fSCKART->AccessFile(false, 1);
//            }
//            LastSet.REN_iUseControlLoaderInputCount=fSCKART->iLotCount;

            sData3.sprintf("Lot start: %s, %d", fSCKART->sLotID, fSCKART->iLotCount);
            RecordProcess(sData3);

            iFTRTCntTmpFor70=1;  //initial to FT stat
            iErrInt=RENESAS_Err_Index.OK;
            GetErrCode(iErrInt, asSendErrCode, asSendErrComm);
            SendProductInfo(iErrInt);
            Task=1;
//            if(RecvData.ProdInfo.StepCode[1]=='F')
//            {
//                //if(atoi(fMain->lbledtInputQty->Text.c_str())==0) //Kirin 20220223
//                {
//                    iErrInt=RENESAS_Err_Index.NoInputQty;
//                    //fMain->spb_Setup->Enabled=false;
//                    iHaveProdInfoStart=true;
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendProductInfo();
//                    Task=1;
//                    SaveTaskLog("DoProcess_1800",Task);
//                }
//                //else
//                {
//                    //LastSet.REN_iUseControlLoaderInputCount=atoi(fMain->lbledtInputQty->Text.c_str());
////                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
////                    //==>
////                    NewLastSet->iUseControlLoaderInputCount=NewLastSet->iUseControlLoaderInputCount+NewLastSet->iUseSampleLoaderInputCount;//Kirin 20220216
////                    fMain->lbledtInputQty->Text=NewLastSet->iUseControlLoaderInputCount;//Kirin 20220216
////                    //<==
////                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
//                    EndFileData.InitData(); //Kirin 20220218 InitData
//                    iErrInt=RENESAS_Err_Index.OK;
//                    //fMain->spb_Setup->Enabled=false;
//                    iHaveProdInfoStart=true;
//                    //bLotStartState=false;//Kirin 20220615 清除LOTSTART狀態
//                    //Kirin 20220615 add log
//                    //==>
//                    {
//                        AnsiString asMsg;
//                        asMsg.sprintf("System_1300 ");
//                        //asMsg.cat_printf("bLotStartState=%s ",BoolToStr(bLotStartState,true));
//                        //RenesasAction.DoLogForSystem(asMsg.c_str(),0);
//                    }
//                    //<==
//                    //Kirin 20220615 add log
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendProductInfo();
//                    Task=1;
//                    SaveTaskLog("DoProcess_1900",Task);
//                }
//
//            }
//            else
//            {
//                //if(atoi(fMain->lbledtInputQty->Text.c_str())==0)
//                {
//                    //fMain->lbledtInputQty->Text=NewLastSet->asRENESASQty;
//                    //LastSet.REN_iUseControlLoaderInputCount=atoi(NewLastSet->asRENESASQty.c_str());
//                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
//                    //==>
////                    NewLastSet->iUseControlLoaderInputCount=NewLastSet->iUseControlLoaderInputCount+NewLastSet->iUseSampleLoaderInputCount;//Kirin 20220216
////                    fMain->lbledtInputQty->Text=NewLastSet->iUseControlLoaderInputCount;//Kirin 20220216
//                    //<==
//                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
//                }
//                //else
//                {
//                    //NewLastSet->iUseControlLoaderInputCount=atoi(fMain->lbledtInputQty->Text.c_str());
//                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
//                    //==>
////                    NewLastSet->iUseControlLoaderInputCount=NewLastSet->iUseControlLoaderInputCount+NewLastSet->iUseSampleLoaderInputCount;//Kirin 20220216
////                    fMain->lbledtInputQty->Text=NewLastSet->iUseControlLoaderInputCount;//Kirin 20220216
//                    //<==
//                    //Kirin 20220216 FTCT Input Qty+Sample Count 先不開
//                }
//                EndFileData.InitData(); //Kirin 20220218 InitData
//                iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
//                //fMain->spb_Setup->Enabled=false;    //Kirin 20191220 FTCT
//                iHaveProdInfoStart=true;            //Kirin 20191220 FTCT
//                //bLotStartState=false;//Kirin 20220615 清除LOTSTART狀態
//                //Kirin 20220615 add log
//                //==>
//                {
//                    AnsiString asMsg;
//                    asMsg.sprintf("System_1400 ");
//                    //asMsg.cat_printf("bLotStartState=%s ",BoolToStr(bLotStartState,true));
//                    //RenesasAction.DoLogForSystem(asMsg.c_str(),0);
//                }
//                //<==
//                //Kirin 20220615 add log
//                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                SendProductInfo();
//                Task=1;
//                SaveTaskLog("DoProcess_2000",Task); //Kirin 20220127
//            }
        }
        break;
        //<==
        //Kirin 20220127
        //<==
        //Kirin 20191218 FTCT
        //<==
        //Kirin 20191218 FTCT
        //Kirin 20191218 FTCT
        //==>
        //======================================================================
        // FTCT Send to Hander 20
        //======================================================================
        case 30000:
        {
            ServerFrm->asFTCTLastSeqStatus="20"; //Kirin 20200207 增加最後送出狀態來看
            RecvData.ModeStart.iRecv=0;

            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                if(bCanNotUse_Cmd_With_TestEnd(ServerFrm->asFTCTLastSeqStatus))
                {
                    Task=1;
                    SaveTaskLog("DoProcess_20_CanNotUse_Cmd_With_TestEnd",Task); //Kirin 20220127 Add Log
                    break;
                }

                if(CheckInputCommand(ServerFrm->asFTCTLastSeqStatus)==false)
                {
                    iErrInt=6;                                                      //"Out of order"
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendStart(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_20_Out of order",Task);
                    break;
                }

                bRenFTCTManualStartTmp=false;
                fSCKART->iManualStart=false;
                fMain->Pause("FTCT_20");

                fSCKART->sLotStartTime=RecvData.ModeStart.Date;
                asFTCTModeEndFilePath="D:\\FT-CT\\EndSaveFile\\";               //清空所有報表資料
                if(DirectoryExists(asFTCTModeEndFilePath)==true)
                {
                    TSearchRec sr;
                    if (FindFirst(asFTCTModeEndFilePath + "*.*", faAnyFile, sr)==0)
                    {
                        do
                        {
                            // 排除子目錄 "." 和 ".."
                            if ((sr.Attr & faDirectory)==0)
                            {
                                AnsiString fileToDelete=asFTCTModeEndFilePath + sr.Name;
                                remove(fileToDelete.c_str());  // 刪檔
                            }
                        } while (FindNext(sr) == 0);
                        FindClose(sr);
                    }
                }

                if(RecvData.ModeStart.Status=="00")
                {
                    TestIF_File.iSCKART_TryCnt=1;                               //拿來做FTRT次數記錄
                    //========sData[0]=="HTSET" && sData[1]=="700" (LOTCLEARED)========
                    fSCKART->iWaitGPIBLotR=0;
                    ZeroMemory(iAutoTrayCount, sizeof(iAutoTrayCount));
                    if(HasICUnderMachine()==false)
                    {
                        SetRunStartMode(rsmInitial_ART);                        //RogerYang 20250913 Add
                        //LastSet.iTCPModeLotState=0;
                        LastSet.bBreakSCKART=false;
                        fMain->Clarn_Data(1, "ART_LOTCLEARED");
                        fSCKART->ClearLotInfo();                                           //LOTCLEARED
                        RecordProcess("ART LOTCLEARED.");
                        fLotInfo->btClearBarcodeList->Click();
                        iModeEndCounterClear=1;

                        if(fSCKART->iTesterType==1)// && CosFunction.bAutoRetestGPIBmode==true)
                        {
                            fSCKART->iFTRTCount=0;
                            fSCKART->iNeedRT=0;
                            fMain->SetLotState(2);                                              //ART Lot Start
                            LastSet.bEndLotAutoRetestGPIB=false;
                            LastSet.bWaitStartLotAutoRetestGPIB=false;
                            LastSet.bFirstTestAutoRetestGPIB=true;
                        }

//                        fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W); //1
                        //fSCKART->sLotStartTime=Now().FormatString("yyyymmddhhnnss"); //RogerYang 20250930 : RogerYang 瑞薩FT-CT 指令"20"已更新
//                        fSCKART->iInputCount=fSCKART->iLotCount;    //Need OP to edit
                        bRenFTCTManualStartTmp=true;
                        fSCKART->iManualStart=true;
                        fSCKART->AccessFile(false, -1);

                        iErrInt=RENESAS_Err_Index.OK;
                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                        SendStart(iErrInt);
                        bWait20or40=false;
                        bWait20or40Alarm=false;
                        sModeStartTmp=RecvData.ModeStart.Status;
                        Task=1;
                    }
                    else
                    {
                        iErrInt=RENESAS_Err_Index.HandlerHaveDevice;                //"Handler have device"
                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                        SendStart(iErrInt);
                        Task=1;
                        SaveTaskLog("DoProcess_20_HandlerHaveDevice1",Task);
                        break;
                    }
                }
                //========sData[0]=="HTSET" && sData[1]=="701" (LOTRETESTCLEAR?)========
                else // RecvData.ModeStart.Status=="10/20"  do RT
                {
                    //if(fSCKART->iTesterType==1 || fSCKART->iCurrentStatus!=fSCKART->iLOTSTATUS_A)
                    if(fSCKART->iTesterType==1)
                    {
                        fSCKART->iNeedRT=1;

                        if(RecvData.ModeStart.Status=="10")
                        {
                            TestIF_File.iSCKART_TryCnt=2;
                            fSCKART->iFTRTCount=1;
                        }
                        else
                        {
                            TestIF_File.iSCKART_TryCnt=3;
                            fSCKART->iFTRTCount=2;
                        }
                        fMain->Clarn_Data(2, "ART_LOTRETESTCLEARED");
                        if(LastSet.iTester==OFF_LINE)
                        {
                            if(BinSelect[OffT].bAutoRetest[0])
                                LastSet.BinCT[0][0]=0;
                            if(BinSelect[OffT].bAutoRetest[1])
                                LastSet.BinCT[0][1]=0;
                            if(BinSelect[OffT].bAutoRetest[2])
                                LastSet.BinCT[0][2]=0;
                        }
                        else
                        {
                            if(BinSelect[FT].bAutoRetest[0])
                                LastSet.BinCT[0][0]=0;
                            if(BinSelect[FT].bAutoRetest[1])
                                LastSet.BinCT[0][1]=0;
                            if(BinSelect[FT].bAutoRetest[2])
                                LastSet.BinCT[0][2]=0;
                        }

                        for(int i=0; i<10; i++)
                        {
                            LastSet.lSCKARTBinCT[i]=0;
                        }
                        LastSet.iSCKARTInputCT=0;
                        LastSet.lShuttleCount=0;
                        LastSet.iHdPass=0;
                        LastSet.iHdFail=0;
                        LastSet.iHdRejectATPass=0;
                        LastSet.iHdRejectATFail=0;
                        LastSet.iHdRejectBT=0;
                        LastSet.iTesterMatch=0;
                        LastSet.iTesterUnmatch=0;
                        LastSet.iTestIgnore=0;
                        LastSet.iHdPickUp=0;
                        LotSummary.ClearRTData();

//                        WriteLastDataFile(false);//kevin 20141030
                        fSortCT->ShowLoadingIC();
                        fSortCT->ShowSortIC();

//                        if(fSCKART->iTesterType==0)
//                        {
//                            fSCKART->iInputCount=LastSet.iSCKART_RTUnitCount;
//                            LastSet.iSCKART_RTUnitCount=0;
//                        }
                        fSCKART->iInputJamCnt    =0;
                        fSCKART->iOutputJamCnt   =0;

//                        if(fSCKART->iCurrentStatus==fSCKART->iLOTSTATUS_R)
//                        {
//                            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
//                            fSCKART->iWaitGPIBLotR=3;
//                        }
                        RecordProcess("ART LOTRETESTCLEARED.");
                        fLotInfo->btClearBarcodeList->Click();
                        sData1="OK";
                                                                                //LOTRETESTCLEAR
                        iErrInt=RENESAS_Err_Index.OK;
                        bRenFTCTManualStartTmp=true;
                        fSCKART->iManualStart=true;
                        fSCKART->CheckNeedRT(); //移到這裡
                        //fSCKART->UpdateCount(); //更新畫面上的數量
                        //fSCKART->AccessFile(false, -1);    //CheckNeedRT裡面已經存了
                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                        SendStart(iErrInt);
                        bWait20or40=false;
                        bWait20or40Alarm=false;
                        sModeStartTmp=RecvData.ModeStart.Status;
                        Task=1;
                    }
                    else
                    {
//                        fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
                        fSCKART->iCurrentFlexARTStep=10;
                        sData1="NG";                                                        //SETTINGNG
                        iErrInt=RENESAS_Err_Index.HandlerHaveDevice;                //"Handler have device"
                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                        SendStart(iErrInt);
                        Task=1;
                        SaveTaskLog("DoProcess_20_HandlerHaveDevice2",Task);
                        break;
                    }
                }
                ServerFrm->TestMode=RecvData.ModeStart.Status;
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
                //iHaveRecvModeStart=false;
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendStart(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_20_DisiableFTCT",Task); //Kirin 20220127 Add Log
                break;
            }
//            SaveTaskLog("DoProcess_20",Task);

            //if(NewLastSet->CloseFTCT==0)      //Kirin 20191220 FTCT
//            {
//                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
//                //==>
//                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
//                //Kirin 20221123 收到20後不可以接受B0
//                //==>
//                bCanRecvCleanOut_B0=false;
//                //<==
//                //Kirin 20221123 收到20後不可以接受B0
//                if(iHaveRecvModeEnd==true)
//                {
//                    iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
//                    ServerFrm->TestMode=RecvData.ModeStart.Status;
//                    //RenesasAction.iCleanOutFinishLockSTART=0; //Kirin 20200115 CleanOut Finish Lock start
//                    iHaveRecvModeStart=true; //Kirin 20200207 Add RecvModeStart
//                    iHaveRecvModeEnd=false; //Kirin 20200207 Add RecvModeStart
//                }
//                else if(iHaveProdInfoStart==true)
//                {
//                    if(iProdStartProcess==false)
//                    {
//                        iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
//                        ServerFrm->TestMode=RecvData.ModeStart.Status;
//                        iHaveRecvModeStart=true;
//                        //RenesasAction.iCleanOutFinishLockSTART=0; //Kirin 20200115 CleanOut Finish Lock start
//                    }
//                    //Kirin 20200113 已經收到MODESTART要回NG
//                    //==>
//                    else
//                    {
//                        iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder   //Krin 20200114 3=>6 6順序異常
//                        Task=35000;
//                        SaveTaskLog("DoProcess_2200",Task); //Kirin 20220127 Add Log
//                        break;
//                    }
//                    //<==
//                    //Kirin 20200113 已經收到MODESTART要回NG
//                }
//                else
//                {
//                    iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder
//                    iHaveRecvModeStart=false;
//                    Task=35000;
//                    SaveTaskLog("DoProcess_2300",Task); //Kirin 20220127 Add Log
//                    break;
//                }
//            }
            //else if(NewLastSet->CloseFTCT==1)
//            {
//                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
//                iHaveRecvModeStart=false;
//                Task=35000;
//                SaveTaskLog("DoProcess_2400",Task); //Kirin 20220127 Add Log
//                break;
//            }
//            if(RecvData.ModeStart.Status=="00")
//            {
//                //RenesasAction.iFTCTModeChangeBefore=LastSet.iTestMode; //Kirin 20220616 bFTCTModeChangeBefore
//                //Kirin 20230426
//                //==>
//                asMsg.sprintf("FTCT Set TestMode");
//                //fData->i_Record_AlarmData(asMsg,5,true,false,"System");
//                //<==
//                //Kirin 20230426
//                //Set_iTestMode(0);//Kirin 20230426 增加iTestMode變數變化紀錄 LastSet.iTestMode=0;
//                #if defined HT3016
//                RenesasAction.iARTInitStatus[0]=0;
//                RenesasAction.iARTInitStatus[1]=0;
//                RenesasAction.iARTInitStatus[2]=0;
//                #endif
//            }
//            else if(RecvData.ModeStart.Status=="10")
//            {
//                //Kirin 20200109 FTCT
//                //==>
//                //if(frmFTCTProcess->Showing)
//                {
//                    //frmFTCTProcess->iReturnCode=12;  //Kirin 20200110 FTCT RT=12
//                    //frmFTCTProcess->Close();
//                }
//                //<==
//                //Kirin 20200109 FTCT
//                //Kirin 20220215 強制中斷
//                //==>
//                //if(fNote->Showing)
//                //{
//                //    fNote->Close();
//                //}
//                //<==
//                //Kirin 20220215 強制中斷
//                #if defined HT3016
//              //Kirin 20200108 增加ART開關
//              //==>
//                if(NewLastSet->bARTSW)
//                {
//                    RenesasAction.iFTCTModeChangeBefore=LastSet.iTestMode; //Kirin 20220616 bFTCTModeChangeBefore
//                    //Kirin 20230426
//                    //==>
//                    asMsg.sprintf("FTCT Set TestMode");
//                    fData->i_Record_AlarmData(asMsg,5,true,false,"System");
//                    //<==
//                    //Kirin 20230426
//                    Set_iTestMode(1);//Kirin 20230426 增加iTestMode變數變化紀錄 LastSet.iTestMode=1;   //Kirin 20200109 FTCT
//                    RenesasAction.iARTInitStatus[0]=0;
//                    RenesasAction.iARTInitStatus[1]=1;
//                    //Kirin 20230111 add two passbin
//                    //==>
//                    if(TrayForm.iPassBinMode==0)
//                        RenesasAction.iARTInitStatus[2]=1;
//                    else
//                        RenesasAction.iARTInitStatus[2]=0;
//                    //<==
//                    //Kirin 20230111 add two passbin
//                }
//                else
//                {
//                    RenesasAction.iARTInitStatus[0]=0;
//                    RenesasAction.iARTInitStatus[1]=0;
//                    RenesasAction.iARTInitStatus[2]=0;
//                }
//              //<==
//              //Kirin 20200108 增加ART開關
//                #endif
//
//            }
//            else if(RecvData.ModeStart.Status=="20")
//            {
//                //Kirin 20200109 FTCT
//                //==>
//                //if(frmFTCTProcess->Showing)
//                //{
//                   // frmFTCTProcess->iReturnCode=14; //Kirin 20200110 FTCT RRT=4  //Kirin 20200109 RT2所以不能設定3
//                    //frmFTCTProcess->Close();
//                //}
//                //<==
//                //Kirin 20200109 FTCT
//                //Kirin 20220215 強制中斷
//                //==>
//                //if(fNote->Showing)
//                //{
//                //    fNote->Close();
//                //}
//                //<==
//                //Kirin 20220215 強制中斷
//                #if defined HT3016
//              //Kirin 20200108 增加ART開關
//              //==>
//                if(NewLastSet->bARTSW)
//                {
//                    RenesasAction.iFTCTModeChangeBefore=LastSet.iTestMode; //Kirin 20220616 bFTCTModeChangeBefore
//                    //Kirin 20230426
//                    //==>
//                    asMsg.sprintf("FTCT Set TestMode");
//                    fData->i_Record_AlarmData(asMsg,5,true,false,"System");
//                    //<==
//                    //Kirin 20230426
//
//                    Set_iTestMode(2);//Kirin 20230426 增加iTestMode變數變化紀錄 LastSet.iTestMode=2; //Kirin 20200109 FTCT
//                    RenesasAction.iARTInitStatus[0]=0;
//                    RenesasAction.iARTInitStatus[1]=1;
//                    //Kirin 20230111 add two passbin
//                    //==>
//                    if(TrayForm.iPassBinMode==0)
//                        RenesasAction.iARTInitStatus[2]=1;
//                    else
//                        RenesasAction.iARTInitStatus[2]=0;
//                    //<==
//                    //Kirin 20230111 add two passbin
//                }
//                else
//                {
//                    RenesasAction.iARTInitStatus[0]=0;
//                    RenesasAction.iARTInitStatus[1]=0;
//                    RenesasAction.iARTInitStatus[2]=0;
//                }
//              //<==
//              //Kirin 20200108 增加ART開關
//                #endif
//
//            }
//            //fMain->ShowTestMode(); //Kirin 20230426 這裡移除掉 Set_iTestMode裡面有了
//            Task=34000;
//            SaveTaskLog("DoProcess_2500",Task); //Kirin 20220127 Add Log
//        }
        }
        break;
        case 34000:
        {
//            iModeEndCounterClear=1; //Kirin 20191219 FTCT
            //Kirin 20220208 把TotalFAIL數量放到投入量
            //==>
            //if(LastSet.iTestMode>0) //Kirin 20220217 大於FT
            {
                //Kirin 20220216
                //==>
//                NewLastSet->iUseControlLoaderInputCount=EndFileData.HdFail;
//                fMain->lbledtInputQty->Text=EndFileData.HdFail;
                //if(NewLastSet->iFTCTFailInputCount!=0)
                {
                    //NewLastSet->iUseControlLoaderInputCount=NewLastSet->iFTCTFailInputCount;
                    //if(LastSet.iTestMode==1) //Kirin 20220217 只有RT
                    //    NewLastSet->iUseControlLoaderInputCount=NewLastSet->iUseControlLoaderInputCount-NewLastSet->iUseSampleLoaderInputCount;//Kirin 20220216
                    //fMain->lbledtInputQty->Text=NewLastSet->iUseControlLoaderInputCount;
                    //Kirin 20220216 FTCT
                    //==>
                    //NewLastSet->iUseSampleLoaderInputCount=0;
                    //fMain->lbledtSimLoaderCount->Text=NewLastSet->iUseSampleLoaderInputCount;
                    //<==
                    //Kirin 20220216 FTCT
                }
                //else
                {
                //<==
                //Kirin 20220216
                    //Kirin 20220218 當人員沒有輸入時 RT中斷重啟時要帶入 FTCT Count
                    //==>
                    //if(EndFileData.HdFail>0)
                        //NewLastSet->iUseControlLoaderInputCount=EndFileData.HdFail;
                    //else
                        //NewLastSet->iUseControlLoaderInputCount=atoi(NewLastSet->asRENESASQty.c_str());
                    //<==
                    //Kirin 20220218 當人員沒有輸入時 RT中斷重啟時要帶入 FTCT Count
                   // if(LastSet.iTestMode==1) //Kirin 20220217 只有RT
                    //    NewLastSet->iUseControlLoaderInputCount=NewLastSet->iUseControlLoaderInputCount-NewLastSet->iUseSampleLoaderInputCount;//Kirin 20220216
                  //  fMain->lbledtInputQty->Text=NewLastSet->iUseControlLoaderInputCount;//Kirin 20220216 iUseControlLoaderInputCount=>EndFileData.HdFail;
                    //Kirin 20220216 FTCT
                    //==>
                  //  NewLastSet->iUseSampleLoaderInputCount=0;
                   // fMain->lbledtSimLoaderCount->Text=NewLastSet->iUseSampleLoaderInputCount;
                    //<==
                    //Kirin 20220216 FTCT
                }//Kirin 20220216
            }
            //Kirin 20220217 這裡不要了
            //==>
//            //Kirin 20220216
//            //==>
//            if(LastSet.iTestMode==2)
//            {
//                NewLastSet->iUseControlLoaderInputCount=EndFileData.HdFail;
//                fMain->lbledtInputQty->Text=EndFileData.HdFail;
//            }
//            //Kirin 20220216
            //<==
            //Kirin 20220217 這裡不要了
            //<==
            //Kirin 20220208 把TotalFAIL數量放到投入量
            //fMain->Pause();//Kirin 20230814 強制停止 //Kirin 20220215 強制停止
            bCleanCountFail=true;//Kirin 20230829 //Kirin 20230814 add 清量失敗旗標
            //OnClearSummary_Click(NULL);
            //bLotStartState=false;//Kirin 20230202
            //Kirin 20230113 下一批要開始前change to auto
            //==>
            //RunMode=AutoMode;
            //fMain->ProcessModeUpdate();
            //asMsg.sprintf("%s Process AUTO Mode",AnsiNowUserID);
            //fData->i_Record_AlarmData(asMsg,5,true,false,"System");
            //<==
            //Kirin 20230113 下一批要開始前change to auto
            //RenesasAction.bFTCTCleanOutFinish=false; //Kirin 20230115 add two pass clear Keep bin FTCT CleanOut Finish
//            Task=35000;
//            SaveTaskLog("DoProcess_34000_1",Task); //Kirin 20220127 Add Log
        }
        break;
        case 35000:
        {
            //if(NewLastSet->CloseFTCT==0)      //Kirin 20191220 FTCT
            {
                //if(NewLastSet->bARTSW==false)  //Kirin 20200109 FTCT
//                    iErrInt=7;
                //else if(bCleanCountFail==false)//Kirin 20230829 //Kirin 20230814
//                    iErrInt=RENESAS_Err_Index.CleanCountFail; //Kirin 20230814
                //Kirin 20200113 這裡拿掉避免影響
                //==>
//                else
//                    iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
                //<==
                //Kirin 20200113 這裡拿掉避免影響
                //Kirin 20220215 註解掉FTCT會送START
                //==>
//                fMain->Start(); //Kirin 20220215 start
                //<==
                //Kirin 20220215 註解掉FTCT會送START
            }
            //else
//            {
//                iErrInt=RENESAS_Err_Index.DisiableFTCT;
//            }
//            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//            SendStart();
//            Task=1;
//            SaveTaskLog("DoProcess_35000_1",Task);
        }
        break;
        //<==
        //Kirin 20191218 FTCT
        //Kirin 20191213 Proc HandlerStart
        //==>
        case 40000:
        {
            ServerFrm->asFTCTLastSeqStatus="50"; //Kirin 20200207 增加最後送出狀態來看
            RecvData.HandlerStart.iRecv=0;

            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                if(bCanNotUse_Cmd_With_TestEnd(ServerFrm->asFTCTLastSeqStatus))
                {
                    Task=1;
                    SaveTaskLog("DoProcess_50_CanNotUse_Cmd_With_TestEnd",Task); //Kirin 20220127 Add Log
                    break;
                }
                if(CheckInputCommand(ServerFrm->asFTCTLastSeqStatus)==false)
                {
                    iErrInt=6;                                                      //"Out of order"
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendHandlerStart(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_50_Out of order",Task);
                    break;
                }
                //Kirin 20240312
                //==>
                if(bSend71WiteReply50_40_90)
                    bSend71WiteReply50_40_90=false;
                //<==
                //Kirin 20240312


                //iRet=fMain->RemoteControl(2);

//                else
//                    iErrInt=12;

                Task=41000;
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendHandlerStart(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_50_DisiableFTCT",Task); //Kirin 20220127 Add Log
            }

            //OnCallHandlerStartEvent();
            //if(NewLastSet->CloseFTCT==0)      //Kirin 20191220 FTCT
//            {
//                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
//                //==>

//                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
//                if(iHaveProdInfoStart==false) //Kirin 20200113 未收到要回NG
//                {
//                    iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendHandlerStart();
//                    Task=1;
//                    SaveTaskLog("DoProcess_2900",Task); //Kirin 20220127 Add Log
//                    //Kirin 20200217 Add Log
//                    //==>
//                    asMsg.sprintf("iErrInt=%d ",iErrInt);
//                    asMsg.cat_printf("50_100 ");
//                    asMsg.cat_printf("iHaveProdInfoStart=false ");
//                    //PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);
//                    //<==
//                    //Kirin 20200217 Add Log
//                    break; //Kirin 20200210 不要開始
//                }
//                else if(iHaveRecvModeStart==false) //Kirin 20200210 沒有收到ModeStart要回NG
//                {
//                    iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendHandlerStart();
//                    Task=1;
//                    SaveTaskLog("DoProcess_3000",Task); //Kirin 20220127 Add Log
//                    //Kirin 20200217 Add Log
//                    //==>
//                    asMsg.sprintf("iErrInt=%d ",iErrInt);
//                    asMsg.cat_printf("50_200 ");
//                    asMsg.cat_printf("iHaveRecvModeStart=false ");
//                    //PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);
//                    //<==
//                    //Kirin 20200217 Add Log
//                    break; //Kirin 20200210 不要開始
//                }
//                else if(iHaveRecvModeEnd==true)  //Kirin 20200113 已經ModeEnd 要回傳NG
//                {
//                    iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendHandlerStart();
//                    Task=1;
//                    SaveTaskLog("DoProcess_3100",Task); //Kirin 20220127 Add Log
//                    //Kirin 20200217 Add Log
//                    //==>
//                    asMsg.sprintf("iErrInt=%d ",iErrInt);
//                    asMsg.cat_printf("50_300 ");
//                    asMsg.cat_printf("iHaveRecvModeEnd=false ");
//                    //PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);
//                    //<==
//                    //Kirin 20200217 Add Log
//                    break; //Kirin 20200210 不要開始
//                }
                //Kirin 20200210 Already StartLot
                //==>
//                else if(iProdStartProcess==true)
//                {
                    //Kirin 20200217 這裡不需要，因為第二次開始會因為狀態沒有變更被卡住
                    //==>
//                    if(ServerFrm->asHandlerLastSeqStatus!="51") //Kirin 20200210 recv not 51
//                    {
//                        iErrInt=RENESAS_Err_Index.OutOfOrder; //Kirin 20220127 Modify ErrIndex 6=>RENESAS_Err_Index.OutOfOrder
//                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                        SendHandlerStart();
//                        Task=1;
//                        //Kirin 20200217 Add Log
//                        //==>
//                        asMsg.sprintf("iErrInt=%d ",iErrInt);
//                        asMsg.cat_printf("50_400 ");
//                        asMsg.cat_printf("asHandlerLastSeqStatus!=51 ");
//                        PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);
//                        //<==
//                        //Kirin 20200217 Add Log
//                        break; //Kirin 20200210 不要開始
//                        break; //Kirin 20200210 不要開始
//                    }
//                    else
                    //<==
                    //Kirin 20200217 這裡不需要，因為第二次開始會因為狀態沒有變更被卡住
//                    {
//                        iProdStartProcess=true; //Kirin 20200210 iProdStartProcess
//                        iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
//                        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                        SendHandlerStart();
//                        //if(fAllMotorHome==false)
//                        {
//                            //fMain->HomeKey();
//                            //fMain->Start(); //Kirin 20191219 start
//                        }
//                        Task=41000;
//                        SaveTaskLog("DoProcess_3200",Task); //Kirin 20220127 Add Log
//                    }
//                }
                //<==
                //Kirin 20200210 Already StartLot
                //else if(RenesasAction.bCleanOutMagazineDoorOpen[1] &&
                //        fMain->RENESAS_Server->bAfterSend41CanCheckAuto2Remove) //Kirin 20230119
//                {
//                    iErrInt=RENESAS_Err_Index.NoRemoveTray;
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendHandlerStart();
//                    Task=1;
//                    SaveTaskLog("DoProcess_4600",Task);
//                    asMsg.sprintf("iErrInt=%d ",iErrInt);
//                    asMsg.cat_printf("50_400 ");
//                    asMsg.cat_printf("iHaveRecvModeEnd=false ");
//                    //PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);
//                    asMsg.sprintf("Please check the Auto%d already remove fail tray",2);
//                    //ShowMyMessage(asMsg.c_str());
//                    break;
//                }
                //else
//                {
//                    iProdStartProcess=true; //Kirin 20200210 iProdStartProcess
//                    iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
//                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                    SendHandlerStart();
//                    //if(fAllMotorHome==false)
//                    {
//                        //fMain->HomeKey();
//                        //fMain->Start(); //Kirin 20191219 start
//                    }
//                    Task=41000;
//                    SaveTaskLog("DoProcess_3300",Task); //Kirin 20220127 Add Log
//                }
//            }
            //else
//            {
//                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
//                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
//                SendHandlerStart();
//                Task=1;
//                SaveTaskLog("DoProcess_3400",Task); //Kirin 20220127 Add Log
//            }
        }
        break;
        //Kirin 20191213 Proc HandlerStart
        //==>
        case 41000:
        {
            //等待狀態都完成，才回復51，特別是RT，提早切到setp9會亂掉
//            if((TestIF_File.iSCKART_TryCnt=1 && fSCKART->iCurrent93KARTStep!=2)||    //FT
//                TestIF_File.iSCKART_TryCnt>1 && fSCKART->iCurrent93KARTStep!=8)      //RT
//            {
//                iWaitForSendHandlerStart=1;
//                break;
//            }
            //========sData[0]=="HTSET" && sData[1]=="703" (LOTORDER0)========
            if(fSCKART->iFTRTCount==0)               //不要先回復51，並且改成確認狀態是在wait狀態才變更
                fSCKART->iCurrent93KARTStep=3;
            else
            {

                //fSCKART->iCurrent93KARTStep=9;   RT 多了搬tray的動作，立即改iCurrent93KARTStep會變成步序異常
            }

            bLoadingCountFullFlag=false;                                        //釋放掉供給/不供給旗標
            fSCKART->bFirstFullSkip=false;                                      //RogerYang 20251112 : ART數量到，如果inarm上有IC直接丟到shuttle
            bRenFTCTManualStartTmp=true;
            fSCKART->iManualStart=true;
            fSCKART->AccessFile(false, 1);
            LastSet.bWaitStartLotAutoRetestGPIB=true;
            iProdStartProcess=true;
            iErrInt=RENESAS_Err_Index.OK;
            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
            SendHandlerStart(iErrInt);

            //if(fMain->palMainStatus->Caption=="HALT" && SystemStart==false)     //先送51再啟動
            if(SystemStart==false)
            {                                                                   //只能在閒置時啟動
                fMain->Start("FTCT_50");
            }

            SaveTaskLog("DoProcess_50_Start()",Task);
            Task=1;
//            if(fHome->Showing==false)
//            {
//                //if(fAllMotorHome==true)
//                {
//                //    fMain->Start();
//                    Task=1;
////                    fMain->RENESAS_Server->bAfterSend41CanCheckAuto2Remove=false;//Kirin 20230119
//                    SaveTaskLog("DoProcess_3500",Task); //Kirin 20220127 Add Log
//                }
                //Kirin 20200114 wait home finish
                //==>
//                else //Kirin 20200109 FTCT
//                {
//                    Task=1;
//                }
                //<==
                //Kirin 20200114 wait home finish
//            }
        }
        break;
        //<==
        //Kirin 20191213 Proc HandlerStart
        //Kirin 20191218 FTCT
        //==>
        case 50000:
        {
            ServerFrm->asFTCTLastSeqStatus="30"; //Kirin 20200207 增加最後送出狀態來看
            //Kirin 20220224
            //==>
            if(bSend71WaitReply30)
                bSend71WaitReply30=false;
            //<==
            //Kirin 20220224
            RecvData.ModeEnd.iRecv=0;
            bRenFTCTManualStartTmp=false;
            iManualStartTmp=fSCKART->iManualStart;
            fSCKART->iManualStart=false;

            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
                //==>
                if(bCanNotUse_Cmd_With_TestEnd(ServerFrm->asFTCTLastSeqStatus))
                {
                    bRenFTCTManualStartTmp=iManualStartTmp;
                    fSCKART->iManualStart=iManualStartTmp;
                    Task=1;
                    SaveTaskLog("DoProcess_30_CanNotUse_Cmd_With_TestEnd",Task); //Kirin 20220127 Add Log
                    break;
                }
                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
                if(CheckInputCommand(ServerFrm->asFTCTLastSeqStatus)==false)
                {
                    iErrInt=6;                                                      //"Out of order"
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendEnd(iErrInt);
                    bRenFTCTManualStartTmp=iManualStartTmp;
                    fSCKART->iManualStart=iManualStartTmp;
                    Task=1;
                    SaveTaskLog("DoProcess_30_Out of order",Task); //Kirin 20220127 Add Log
                    break;
                }

//                if(iHaveRecvTestEnd==1)
//                {
//                    iHaveRecvTestEnd=0;
//                }

                //firstStr1=RecvData.ModeStart.Status.SubString(1, 1);
                firstStr1=sModeStartTmp.SubString(1, 1);
                firstStr2=RecvData.ModeEnd.Status.SubString(1, 1);
                if(firstStr1!=firstStr2)    //RogerYang 20251003 : 如果收到30，要對應FT(00-01)/RT(10-11)/RRT(20-21)
                {
                    iErrInt=13;
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                    SendEnd(iErrInt);
                    bRenFTCTManualStartTmp=iManualStartTmp;
                    fSCKART->iManualStart=iManualStartTmp;
                    Task=1;
                    SaveTaskLog("DoProcess_30_Status code does not match",Task);
                    break;
                }
                SaveModeEndFile();
                Task=51000; //Kirin 20191219 FTCT
                SaveTaskLog("DoProcess_30_SaveModeEndFile",Task); //Kirin 20220127 Add Log
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendEnd(iErrInt);
                bRenFTCTManualStartTmp=iManualStartTmp;
                fSCKART->iManualStart=iManualStartTmp;
                Task=1;
                SaveTaskLog("DoProcess_30_DisiableFTCT",Task);
                break;
            }
        }
        break;
        case 51000:
        {
            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                fSCKART->iInputCount=0;                                         //RogerYang 202510115 : 清空InputQty，方便這時候人員去修改
                iErrInt=RENESAS_Err_Index.OK;

                bCanRecvCleanOut_B0=true;

                //if(HasICUnderMachine()==true || HasAnyICInMachine()==true)
                if(HasICUnderMachine()==true)
                {
                    iErrInt=RENESAS_Err_Index.HandlerHaveDevice; //Kirin 20220127 Modify ErrIndex 3=>RENESAS_Err_Index.HandlerHaveDevice
                    iHaveRecvModeEnd=false; //Kirin 20200113 紀錄收到RecvModeEnd
                }
                else
                {
                    if(TestIF_File.iSCKART_TryCnt==1)
                        fSCKART->iCurrent93KARTStep=4;
                    else
                        fSCKART->iCurrent93KARTStep=10;

                    iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
                    iProdStartProcess=false;//Kirin 20200210 iProdStartProcess
                    iHaveRecvModeEnd=true; //Kirin 20200113 紀錄收到RecvModeEnd
                }
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;
                iHaveRecvModeEnd=false; //Kirin 20200113 紀錄收到RecvModeEnd
            }
            fSCKART->AccessFile(false, 1);
            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
            SendEnd(iErrInt);
            bRenFTCTManualStartTmp=iManualStartTmp;
            fSCKART->iManualStart=iManualStartTmp;
            Task=1;
            SaveTaskLog("DoProcess_30_SendEnd",Task); //Kirin 20220127 Add Log
        }
        break;
        case 60000:
        {
            ServerFrm->asFTCTLastSeqStatus="40"; //Kirin 20200207 增加最後送出狀態來看
            RecvData.LotEnd.iRecv=0;
            fSCKART->iNeedRT=0; //不確定要不要這樣做
            bRenFTCTManualStartTmp=false;
            fSCKART->iManualStart=false;
            fSCKART->AccessFile(false, 1);
            bWait20or40=false;
            bWait20or40Alarm=false;
//            bReturn41Flag=true;
            //Kirin 20240312
            //==>
            if(bSend71WiteReply50_40_90)
                bSend71WiteReply50_40_90=false;
            //<==
            //Kirin 20240312

            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                //Kirin 20220119 收到70要拒絕 除00,40,90之外的指令
                if(bCanNotUse_Cmd_With_TestEnd(ServerFrm->asFTCTLastSeqStatus))
                {
                    Task=1;
                    SaveTaskLog("DoProcess_40_CanNotUse_Cmd_With_TestEnd",Task); //Kirin 20220127 Add Log
                    break;
                }

                fSCKART->iCurrent93KARTStep=10;  //暫時先這樣寫，先求流程能夠中斷

                iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);//Kirin 20191220 FTCT

                bCanRecvCleanOut_B0=false;

                iHaveProdInfoStart=false; //Kirin 20200109 FTCT



                if(ServerFrm->asHandlerLastSeqStatus==RENESAS_CMD.HandlerStartReplyCMD)
                {
                    //FTCT 在這個狀態下，需要先執行cleanout
                    iCleanOut=1;
                }

                if(HasICUnderMachine()==false &&                                //沒有IC就先送41
                    HasAnyICInMachine()==false)
                {
                    //SendLotEnd();
                }
                SendLotEnd(iErrInt);                                            //RogerYang 20251014 : 改成收到40無論如何都先回復41 (因為FTCT端有自己的timer)
                fMain->EnabledSetupFile(true);

                iHaveRecvModeEnd=false; //Kirin 20200113 紀錄收到RecvModeEnd
                iProdStartProcess=false; //Kirin 20200210 iProdStartProcess
                iHaveRecvTestEnd=0;  //Kirin 20220224

            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
                SendLotEnd(iErrInt);
            }
            bAfterSend41CanCheckAuto2Remove=true;//Kirin 20230113 bAfterSend41CanCheckAuto2Remove
            Task=1;
            //SaveTaskLog("DoProcess_40_Done,Task); //Kirin 20220127 Add Log
        }
        break;
        case 70000:
        {
            ServerFrm->asFTCTLastSeqStatus="70";
            RecvData.TestEnd.iRecv=0;
            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                if(CheckInputCommand(ServerFrm->asFTCTLastSeqStatus)==false)
                {
//                    sLastLegalState=ServerFrm->asFTCTLastSeqStatus;
                    iErrInt=6;                                                      //"Out of order"
                    GetErrCode(iErrInt, asSendErrCode, asSendErrComm);
                    SendTestEnd(iErrInt);
                    Task=1;
                    SaveTaskLog("DoProcess_70_Out of order",Task);
                    break;
                }
                else
                {
                    sLastLegalState=ServerFrm->asFTCTLastSeqStatus;
//                    if(iFTRTCntTmpFor70==TestIF_File.iSCKART_TryCnt)    //same FT/RT Cycle
//                        bCanReturn71=true;
//                    else
//                        bCanReturn71=false;
                    iHaveRecvTestEnd=1;
                    bRenFTCTManualStartTmp=true;
                    fSCKART->iManualStart=true;
                    fSCKART->AccessFile(false, 1);
                    iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK

                    if(fMain->palMainStatus->Caption=="HALT" && SystemStart==false)     //收到70也嘗試啟動
                    {                                                                   //只能在閒置時啟動
                        fMain->Start("FTCT_50");
                    }

                    //Kirin 20220225
                    //==>
                    if(b70_Disconnect)
                    {
                        b70_Disconnect=false;
                    }
                    //<==
                    //Kirin 20220225
                    b70_DisconnectAlarm=false;
                    b70_Do70MsgAlarm=false;
                    Task=1;
                    SaveTaskLog("DoProcess_70_Start()",Task); //Kirin 20220127 Add Log
                }
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;//Kirin 20220127 Modify ErrIndex 5=>RENESAS_Err_Index.DisiableFTCT
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendTestEnd(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_70_DisiableFTCT",Task); //Kirin 20220127 Add Log

            }
        }
        break;
        case 80000:
        {
            ServerFrm->asFTCTLastSeqStatus="90";
            RecvData.SendAlarm.iRecv=0;
            //Kirin 20240312
            //==>
            if(bSend71WiteReply50_40_90)
                bSend71WiteReply50_40_90=false;
            //<==
            //Kirin 20240312
            if(TestIF_File.bRENESAS_EnableFTCT==true)
            {
                bRenesasFTCTAlarm=true;
                iErrInt=RENESAS_Err_Index.OK; //Kirin 20220127 Modify ErrIndex 0=>RENESAS_Err_Index.OK
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendAlarm(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_90_SendAlarm",Task); //Kirin 20220127 Add Log
            }
            else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
                SendAlarm(iErrInt);
                Task=1;
                SaveTaskLog("DoProcess_90_DisiableFTCT",Task);
            }
        }
        break;
        case 90000:
        {
            ServerFrm->asFTCTLastSeqStatus="B0";
            RecvData.SendCleanOut.iRecv=0;
            //if(NewLastSet->CloseFTCT==0)
            {
                //Kirin 20221123 要卡收到30之後與20或40之前才可以接收
                //==>
                if(bCanRecvCleanOut_B0)
                {
                    //if(RenesasAction.bFTCTCleanOutFinish==false) //Kirin 20230115 add two pass Keep bin FTCT CleanOut Finish
                    {
                        //Kirin 20230103 增加檢查 FTCT決策視窗開啟時 就關閉避免決策視窗卡住動作
                        //==>
                        //if(NewLastSet->bRecvB0CloseFTCTMsg)
                        {
                            //if(frmFTCTProcess->Showing)
                            {
                                asMsg.sprintf("Recv B0 Close FTCT Window");
                                //fData->i_Record_AlarmData(asMsg,5,true,false,"System");
                                //frmFTCTProcess->iReturnCode=13;
                                //frmFTCTProcess->Close();
                            }
                            //Kirin 20230113 強制中斷 要正常回應然後畫面還有mymessage關掉
                            //==>
                            //if(MyMessageBox->Showing)
                            //{
                            //    MyMessageBox->iReturnCode=1;
                            //    MyMessageBox->Close();
                            //}
                            //<==
                            //Kirin 20230113 強制中斷 要正常回應然後畫面還有mymessage關掉
                            //Kirin 20230115 add two passbin 強制中斷要正常回應然後NOTE畫面關掉
                            //==>
                            //if(fNote->Showing)
                            //{
                           //     fNote->Close();
                           // }
                            //<==
                            //Kirin 20230115 add two passbin 強制中斷要正常回應然後NOTE畫面關掉
                        }
                        //<==
                        //Kirin 20230103 增加檢查 FTCT決策視窗開啟時 就關閉避免決策視窗卡住動作
//                        //Kirin 20230119 檢查Magazine都在下面就只要秀訊息用
//                        //==>
//                        bool bCleanOutAllMagazineIsZero=false;
//                        bCleanOutAllMagazineIsZero= CheckHasAnyICUnderMachine()==false &&
//                                                    MOT[Renesas_Magazine.JamEmpty].HomeLed ==true   &&
//                                                    MOT[Renesas_Magazine.Loader].HomeLed   ==true   &&
//                                                    MOT[Renesas_Magazine.PassEmpty].HomeLed==true   &&
//                                                    MOT[Renesas_Magazine.FailEmpty].HomeLed==true   &&
//                                                    MOT[Renesas_Magazine.PassAuto].HomeLed ==true   &&
//                                                    MOT[Renesas_Magazine.FailAuto1].HomeLed==true   &&
//                                                    MOT[Renesas_Magazine.FailAuto2].HomeLed==true    ;
//                        //<==
//                        //Kirin 20230119 檢查Magazine都在下面就只要秀訊息用
                        {
                            //RunMode=CleanOutMode;
                            //fMain->ProcessModeUpdate();
                            //asMsg.sprintf("%s Process CLEAN OUT Mode",AnsiNowUserID);
                            //fData->i_Record_AlarmData(asMsg,5,true,false,"System");
                            //iFTCTSendCleanOut=true;
                           // fMain->Start();
                           /// Task=1;
                           // SaveTaskLog("DoProcess_51000",Task);
                        }
                    }
                    //Kirin 20230115 add two passbin 強制中斷要正常回應然後提示畫面關掉
                    //==>
                    //else
                    {
                        //if(frmFTCTProcess->Showing)
                        {
                            asMsg.sprintf("Recv B0 Close FTCT Window");
                            //fData->i_Record_AlarmData(asMsg,5,true,false,"System");
                            //frmFTCTProcess->iReturnCode=13;
                            //frmFTCTProcess->Close();
                        }
                        //Kirin 20230113 強制中斷 要正常回應然後畫面還有mymessage關掉
                        //==>
                        //if(MyMessageBox->Showing)
                        //{
                       //     MyMessageBox->iReturnCode=1;
                       //     MyMessageBox->Close();
                       // }
                        //<==
                        //Kirin 20230113 強制中斷 要正常回應然後畫面還有mymessage關掉
                        //Kirin 20230115 add two passbin 強制中斷要正常回應然後NOTE畫面關掉
                        //==>
                        //if(fNote->Showing)
                       // {
                       //     fNote->Close();
                       // }
                        //<==
                        //Kirin 20230115 add two passbin 強制中斷要正常回應然後提示畫面關掉
                        //fMain->RENESAS_Server->SendCleanOut(RENESAS_Err_Index.OK);
                        Task=1;
                        SaveTaskLog("DoProcess_51100",Task);
                    }
                    //<==
                    //Kirin 20230115 add two passbin 強制中斷要正常回應然後提示畫面關掉
                }
                else
                {
                    iErrInt=RENESAS_Err_Index.OutOfOrder;
                    GetErrCode(iErrInt,asSendErrCode,asSendErrComm); //Kirin 20221124
                    SendCleanOut(RENESAS_Err_Index.OutOfOrder); //Kirin 20221124
                    Task=1;
                    SaveTaskLog("DoProcess_53000",Task);
                }
                //<==
                //Kirin 20230115 add two passbin 強制中斷要正常回應然後提示畫面關掉
            }
            //else
            {
                iErrInt=RENESAS_Err_Index.DisiableFTCT;
                GetErrCode(iErrInt,asSendErrCode,asSendErrComm);  //Kirin 20221124
                SendCleanOut(RENESAS_Err_Index.DisiableFTCT);     //Kirin 20221124
                Task=1;
                SaveTaskLog("DoProcess_52000",Task);
            }
        }
        break;
        //<==
        //Kirin 20221121 SendCleanOut

    }
    bProc=false; //Kirin 20220127 卡Timer重入保護
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetOnLineMessge()
{
    AnsiString asMsg;
    asMsg.sprintf("%s%c%c",RENESAS_CMD.OnLineReplyCMD,cLineEndData[0],cLineEndData[1]);
//    if(asOnLineErr==Err_Code.OK)
    {
        asMsg.cat_printf("Result:%s%c%c",asSendErrCode,cLineEndData[0],cLineEndData[1]);
        asMsg.cat_printf("Comment:%s%c%c",asSendErrComm,cLineEndData[0],cLineEndData[1]);
    }
//    else
//    {
//        asMsg.cat_printf("Result:%s%c%c",asErrCode,cLineEndData[0],cLineEndData[1]);
//        asMsg.cat_printf("Comment:%s%c%c",asErrCmd,cLineEndData[0],cLineEndData[1]);
//    }
//    if(asSendErrCode=="N0001")
//        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.OnLineReplyCMD;         //回復00不可以更改當前狀態

    asMsg.cat_printf("%c",0x0);
    return asMsg;
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetProductInfoMessge()
{
    AnsiString asMsg;

    asMsg.sprintf("%s%c%c",RENESAS_CMD.ProductInfoReplyCMD,cLineEndData[0],cLineEndData[1]);
//    if(iLoadWorkFileStatus==0)
    {
        GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
        asMsg.cat_printf("Result:%s%c%c"        ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
        asMsg.cat_printf("Comment:%s%c%c"       ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
    }
//    else
//    {
//        GetErrCode(2,asSendErrCode,asSendErrComm);
//        asMsg.cat_printf("Result:%s%c%c"        ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
//        asMsg.cat_printf("Comment:%s%c%c"       ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
//    }
    asMsg.cat_printf("HdParameter:%s%c%c"   ,RecvData.ProdInfo.HdParameter,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Product:%s%c%c"       ,RecvData.ProdInfo.Product,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.ProductInfoReplyCMD; //Kirin 20200207 增加最後送出狀態來看
    return asMsg;
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetStartMessge()
{
    AnsiString asMsg;
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    asMsg.sprintf("%s%c%c",RENESAS_CMD.StartReplyCMD,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Status:%s%c%c",RecvData.ModeStart.Status,cLineEndData[0],cLineEndData[1]); //Kirin 20191218 FTCT
    asMsg.cat_printf("Result:%s%c%c",asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c",asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.StartReplyCMD; //Kirin 20200207 增加最後送出狀態來看
    return asMsg;
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetHandlerStartMessge()
{
    AnsiString asMsg;
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    asMsg.sprintf("%s%c%c",RENESAS_CMD.HandlerStartReplyCMD,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Result:%s%c%c",asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c",asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.HandlerStartReplyCMD; //Kirin 20200207 增加最後送出狀態來看
    return asMsg;
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetEndMessge()
{
    AnsiString asMsg;
    AnsiString asHdParameter;
    //Kirin 20201023 (Marvin) fnsplit分離檔名串資料用
    //==>
    char cDrive[255];
    char cDir[255];
    char cName[255];
    char cExt[255];
    memset(cDrive,0,255);
    memset(cDir,0,255);
    memset(cName,0,255);
    memset(cExt,0,255);
    //<==
    //Kirin 20201023 (Marvin) fnsplit分離檔名串資料用

    //Kirin 20201023 (Marvin) 改用fnsplit分離檔名串資料，舊的註解掉
    //==>
    //asHdParameter=ExtractFileName(LastSet.LastOpenFilename); //Kirin 20191219 FTCT
    //asHdParameter=asHdParameter.SubString(1,asHdParameter.Pos(".")-1);
    //<==
    //Kirin 20201023 (Marvin) 改用fnsplit分離檔名串資料，舊的註解掉
    //Kirin 20201023 (Marvin) 改用fnsplit分離檔名串資料
    //==>
    fnsplit(LastSet.LastOpenFilename,cDrive,cDir,cName,cExt);
    asHdParameter=cName;
    //<==
    //Kirin 20201023 (Marvin) 改用fnsplit分離檔名串資料
    asMsg.sprintf("%s%c%c",RENESAS_CMD.EndReplyCMD,cLineEndData[0],cLineEndData[1]);
    //asMsg.cat_printf("Status:%s%c%c",RecvData.ModeEnd.Status,cLineEndData[0],cLineEndData[1]); //Kirin 20200109  (Marvin) 修正 Status 回傳
    if(RecvData.ModeStart.Status=="00")
        asMsg.cat_printf("Status:%s%c%c","01",cLineEndData[0],cLineEndData[1]); //Kirin 20200109 (Marvin) 修正 Status 回傳
    else if(RecvData.ModeStart.Status=="10")
        asMsg.cat_printf("Status:%s%c%c","11",cLineEndData[0],cLineEndData[1]); //Kirin 20200109 (Marvin) 修正 Status 回傳
    else if(RecvData.ModeStart.Status=="20")
        asMsg.cat_printf("Status:%s%c%c","21",cLineEndData[0],cLineEndData[1]); //Kirin 20200109 (Marvin) 修正 Status 回傳
    asMsg.cat_printf("Result:%s%c%c",asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c",asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("TotalQty:%s%c%c",RecvData.ProdInfo.Qty,cLineEndData[0],cLineEndData[1]);      //Kirin 20191218 (Marvin) 數量上報修改
//    asMsg.cat_printf("TotalQty:%d%c%c",Production_Info->iTotal,cLineEndData[0],cLineEndData[1]);  //Kirin 20191218 (Marvin) 數量上報修改
//    asMsg.cat_printf("HdParameter:%s%c%c",asHdParameter.c_str(),cLineEndData[0],cLineEndData[1]);   //Kirin 20191218 (Marvin) FTCT上報修改
    asMsg.cat_printf("HdParameter:%s%c%c",fMain->cbSetupFileName->Text,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.EndReplyCMD; //Kirin 20200207 (Marvin) 增加最後送出狀態來看
    return asMsg;
}
//---------------------------------------------------------------------------
AnsiString TRENESAS_Server::GetLotEndMessge()
{
    AnsiString asMsg;
    asMsg.sprintf("%s%c%c",RENESAS_CMD.LotEndReplyCMD,cLineEndData[0],cLineEndData[1]);
        //SetErrData(3    ,"E0003","Handler have device");
    asMsg.cat_printf("Result:%s%c%c"    ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c"   ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.LotEndReplyCMD; //Kirin 20200207 增加最後送出狀態來看
    return asMsg;
}
//---------------------------------------------------------------------------
//Kirin 20220119 GetTestEnd
//==>
AnsiString TRENESAS_Server::GetTestEnd()
{
    AnsiString asMsg;
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    asMsg.sprintf("%s%c%c",RENESAS_CMD.TestEndReplyCMD,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Result:%s%c%c"    ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c"   ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.TestEndReplyCMD;
    return asMsg;
}
//<==
//Kirin 20220119 GetTestEnd
//Kirin 20211020 GetSendAlarm
//==>
AnsiString TRENESAS_Server::GetSendAlarm()
{
    AnsiString asMsg;
//    iErrInt=0; //Kirin 20220225
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    asMsg.sprintf("%s%c%c",RENESAS_CMD.SendAlarmReplyCMD,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Result:%s%c%c"    ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c"   ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
//    if(asSendErrCode=="N0001")                                                //00 90 不可更改當前狀態
//        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.SendAlarmReplyCMD;
    return asMsg;
}
//<==
//Kirin 20211020 GetSendAlarm
//---------------------------------------------------------------------------
//Kirin 20221121 GetSendCleanOut
//==>
AnsiString TRENESAS_Server::GetSendCleanOut()
{
    AnsiString asMsg;
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    asMsg.sprintf("%s%c%c",RENESAS_CMD.SendCleanOutReplyCMD,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Result:%s%c%c"    ,asSendErrCode,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("Comment:%s%c%c"   ,asSendErrComm,cLineEndData[0],cLineEndData[1]);
    asMsg.cat_printf("%c",0x0);
    if(asSendErrCode=="N0001")
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.SendCleanOutReplyCMD;
    return asMsg;
}
//<==
//Kirin 20221121 GetSendCleanOut
//---------------------------------------------------------------------------
void TRENESAS_Server::SendOnLine(int iErrorCode)  //01
{
    if(SendSocketData(GetOnLineMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.OnLineReplyCMD;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SendProductInfo(int iErrorCode)  //11
{
    if(SendSocketData(GetProductInfoMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
    {
        sLastLegalState=RENESAS_CMD.ProductInfoReplyCMD; //RogerYang 20251018 : Add
        bTrayFeedDone=false;
    }
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SendStart(int iErrorCode)   //21
{
    if(SendSocketData(GetStartMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.StartReplyCMD;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SendHandlerStart(int iErrorCode)  //51
{
    if(SendSocketData(GetHandlerStartMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.HandlerStartReplyCMD;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SendEnd(int iErrorCode)  //31
{
    if(SendSocketData(GetEndMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.EndReplyCMD;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SendLotEnd(int iErrorCode) //41
{
    if(SendSocketData(GetLotEndMessge())==true && iErrorCode==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.LotEndReplyCMD;

    bSend41WaitTimeout=true;
    TimeOut_Send41WaitTimeout=GetDelayStart();
//    bReturn41Flag=false;

    //清除入料數資訊，如果沒有清掉，下一次"10"進來會判斷有值誤認為人員輸入(優先)，而不考慮FTCT給的數量
    fSCKART->iInputCount=0;
    fSCKART->iLotCount=0;
    bReturn41Flag=true;
    fSCKART->iManualStart=false; //41 Lock Start
}
//---------------------------------------------------------------------------
//Kirin 20220119 SendTestEnd
//==>
void TRENESAS_Server::SendTestEnd(int iErrorCode)   //71
{
    if(b70_DisconnectAlarm==true)                                               //RogerYang 20251111 : 發生過斷線，FTCT收不到，假裝已回復
    {
        iErrInt=RENESAS_Err_Index.OK;
        sLastLegalState=RENESAS_CMD.TestEndReplyCMD;
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.TestEndReplyCMD;
    }
    else if(SendSocketData(GetTestEnd())==true && iErrorCode==RENESAS_Err_Index.OK)
    {
        sLastLegalState=RENESAS_CMD.TestEndReplyCMD;
    }
    //Kirin 20220301 客戶說如果設定為0就使之失效
    //==>
    //if(NewLastSet->i30RecvTimeOut==0)
//    {
//        bSend71WaitReply30=false;
//        bSend71WaitReply30Alarm=false;
//    }
    //else
//    {
        bSend71WaitReply30=true;         //Kirin 20220224
        TimeOut_Send71WaitReply30=GetDelayStart(); //Kirin 20220224
//    }
    //<==
    //Kirin 20220301 客戶說如果設定為0就使之失效
    //Kirin 20240312
    //==>
    //if(NewLastSet->i50_40_90RecvTimeOut==0)
    {
//        bSend71WiteReply50_40_90=false;
//        bSend71WaitReply50_40_90Alarm=false;
    }
    //else
    {
        bSend71WiteReply50_40_90=true;         //Kirin 20220224
        TimeOut_Send71WaitReply50_40_90=GetDelayStart(); //Kirin 20220224
    }
    //<==
    //Kirin 20240312
    bWait20or40=true;
    bWait20or40Alarm=false;
    TimeOut_Wait20or40=GetDelayStart();
}
//<==
//Kirin 20220119 SendTestEnd
//---------------------------------------------------------------------------
void TRENESAS_Server::DoSendTestEnd()                                           //RogerYang 20251106 : FT-CT在70與71之間有斷線的狀況時，再回復71的同時跳警報
{
    iErrInt=RENESAS_Err_Index.OK;
    GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
    SendTestEnd(iErrInt);
}
//---------------------------------------------------------------------------
//Kirin 20211020
//==>
void TRENESAS_Server::SendAlarm(int iErrorCode)   //91
{
    iErrInt=iErrorCode;//Kirin 20220225
    if(SendSocketData(GetSendAlarm())==true && iErrInt==RENESAS_Err_Index.OK)
        sLastLegalState=RENESAS_CMD.SendAlarmReplyCMD;
}
//<==
//Kirin 20211020
//---------------------------------------------------------------------------
//Kirin 20221121 SendCleanOut
//==>
void TRENESAS_Server::SendCleanOut(int iErrorCode)
{
    iErrInt=iErrorCode;//Kirin 20221124
    SendSocketData(GetSendCleanOut());

}
//<==
//Kirin 20221121 SendCleanOut
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TRENESAS_Server::SendSocketData(AnsiString asMsg)
{
    int iRealLen;
    int iIndex;
    char SendBuff[255]={0};
    AnsiString asHexData="";
    AnsiString asMemoMsg="";
    bool rlt=false;
    memcpy(&SendBuff[0],asMsg.c_str(),asMsg.Length());
    try
    {
        if(ServerSocket->Socket->ActiveConnections>0)
        {
            for(int i=0;i<ServerSocket->Socket->ActiveConnections;i++) //Kirin 20230120 當FTCT關掉重啟時會變成1個連結之上
            {
                ServerSocket->Socket->Connections[i]->SendBuf(SendBuff,asMsg.Length());
            }
            rlt=true;
//            PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMsg);  //Kirin 20230118 註解掉 這裡存到的內容不好分析
        }
    }
    catch(...)
    {

    }
    iRealLen=asMsg.Length();
    for(iIndex=0;iIndex<iRealLen;iIndex++)
    {
        asHexData=asHexData+" "+AnsiString(IntToHex(SendBuff[iIndex],2));
    }
    asMemoMsg.cat_printf("Send iRealLen=%d\r\n" ,iRealLen);
    asMemoMsg.cat_printf("Send data=%s\r\n"     ,asMsg);
    asMemoMsg.cat_printf("Send Hexdata=%s\r\n"      ,asHexData.c_str());
    asMemoMsg.cat_printf("=======================================================\r\n");
    AppendToMemo(asMemoMsg);

    //PreSaveLog->Save("D:\\TempLog\\SocketData\\ServerData\\ServerSocket.txt",asMemoMsg); //Kirin 20230118 更改為存這裡處理過的LOG資料

    //回復FTCT狀態為Error時，Handler發警報音但不停機，繼續作業
    if(asMemoMsg.Pos("N0001")==0)
    {
        ShowErrorMessageByReplyError(asSendErrCode, asSendErrComm);
    }

    return rlt;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::ShowServerForm()
{
    ServerFrm->BringToFront();
    ServerFrm->Show();
}
//---------------------------------------------------------------------------
void TRENESAS_Server::DoProcOnShow()
{
    if(ServerFrm!=NULL)
    {
        ServerFrm->iServerListenStatus  =bHaveClientConnect;
        ServerFrm->iOnLineStatus        =iOnLineStatus;
        ServerFrm->frameProdInfo1->ShowData=RecvData;
        //Kirin 20200112 hdParamter Show
        //==>
        //if(Production_Info!=NULL) //Kirin 20200114 增加保護 避免關掉時例外
        {
            //Kirin 20230111 add two passbin
            //==>
            //if(iGetTwoPassBinMode()==0)
                //ServerFrm->iHdPass=Production_Info->iAuto[0];
            //else
            {
                //ServerFrm->iHdPass=Production_Info->iAuto[0]+Production_Info->iAuto[iPassBin2Auto];
            }
            //<==
            //Kirin 20230111 add two passbin
            //Kirin 20221128 add two passbin
            //==>
            //if(iGetTwoPassBinMode()==0) //Kirin 20221221 iSwapAuto23=>iGetTwoPassBinMode
            //    ServerFrm->iHdFail=Production_Info->iAuto[1]+Production_Info->iAuto[2];
            //else
            //    ServerFrm->iHdFail=Production_Info->iAuto[1]; //Kirin 20230111 2=>1
            //<==
            //Kirin 20221128 add two passbin
            //ServerFrm->iHdRejectATPass=LastSet.iHdRejectATPass;
            //ServerFrm->iHdRejectATFail=LastSet.iHdRejectATFail;
            //ServerFrm->iHdRejectBT    =LastSet.iHdRejectBT;
            //ServerFrm->iTesterMatch   =LastSet.iTesterMatch;
            //ServerFrm->iTesterUnmatch =LastSet.iTesterUnmatch;
            //ServerFrm->iTestIgnore    =LastSet.iTestIgnore;
            //ServerFrm->iHdPickUp=Production_Info->iLoad;
            //Kirin 20221115 add two passbin
            //==>
            //if(iGetTwoPassBinMode()==0) //Kirin 20221221 iSwapAuto23=>iGetTwoPassBinMode
            {

            }
            //else
            {
               // ServerFrm->iHdPass1=Production_Info->iAuto[0];
               // ServerFrm->iHdPass2=Production_Info->iAuto[iPassBin2Auto]; //Kirin 20230111 1=>iPassBin2Auto
               // ServerFrm->iHdRejectATPass1=LastSet.iHdRejectATPass1;
               // ServerFrm->iHdRejectATPass2=LastSet.iHdRejectATPass2;
            }
            //<==
            //Kirin 20221115 add two passbin
        }
        //<==
        //Kirin 20200112 hdParamter Show
        ServerFrm->iNowCounter=iGetAfter71CmdTimeOut();   //Kirin 20220224
        //ServerFrm->iSetCounter=NewLastSet->i30RecvTimeOut;//Kirin 20220224
    }
}
//---------------------------------------------------------------------------
void __fastcall TRENESAS_Server::OnProcTimer(TObject *Sender)
{
    DoProcOnShow();
    DoProcess();
    DoCheckAfter71CmdTimeOut(); //Kirin 20220224 DoCheckAfter71CmdTimeOut
    DoCheckAfter71_50_40_90CmdTimeOut();//Kirin 20240312
    DoCheckAfter41CmdTimeOut();
    DoCheckWait20or40TimeOut();
    if(TestIF_File.bRENESAS_EnableFTCT==true &&
        ServerFrm->asFTCTLastSeqStatus=="")
    {
        fSCKART->iManualStart=false;
    }
    DoSystemMessage();
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SaveModeEndFile()
{
    AnsiString asData;
    AnsiString asFileName;
    FILE *SaveFilePtr;
    AnsiString asTempSendModeStart; //Kirin 20200110 送出ModeStart用
    AnsiString asFTCTDesc1; //Kirin 20231002 FTCT Code 自定義
    AnsiString asFTCTDesc2; //Kirin 20231002 FTCT Code 自定義

//    int iUnloadCnt=0;
//    int iTotalUnloadCount=0;

//    for(int i=0; i<10; i++)
//    {
//        iTotalUnloadCount+=LastSet.lSCKARTBinCT[i];
//    }
//
//    if(LastSet.iTester==OFF_LINE)
//    {
//        if(BinSelect[OffT].bAutoRetest[0])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[0];                                //Steven 20170113 : For count unload count
//        if(BinSelect[OffT].bAutoRetest[1])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[1];
//        if(BinSelect[OffT].bAutoRetest[2])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[2];
//    }
//    else
//    {
//        if(BinSelect[FT].bAutoRetest[0])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[0];                                //Steven 20170113 : For count unload count
//        if(BinSelect[FT].bAutoRetest[1])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[1];
//        if(BinSelect[FT].bAutoRetest[2])
//            iUnloadCnt+=LastSet.lSCKARTBinCT[2];
//    }

    asData.sprintf(     "HdPass,%d%c%c"         , LastSet.iHdPass, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "HdFail,%d%c%c"         , LastSet.iHdFail, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "HdRejectATPass,%d%c%c" , LastSet.iHdRejectATPass, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "HdRejectATFail,%d%c%c" , LastSet.iHdRejectATFail, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "HdRejectBT,%d%c%c"     , LastSet.iHdRejectBT, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "TesterMatch,%d%c%c"    , LastSet.iTesterMatch, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "TesterUnmatch,%d%c%c"  , LastSet.iTesterUnmatch, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "TesterIgnore,%d%c%c"   , LastSet.iTestIgnore, cLineEndData[0], cLineEndData[1]);
    asData.cat_printf(  "HdPickUp,%d%c%c"       , LastSet.iHdPickUp, cLineEndData[0], cLineEndData[1]);

    asFTCTModeEndFilePath="D:\\FT-CT\\EndSaveFile\\";
    if(asFTCTModeEndFilePath.Length()>0 &&
       DirectoryExists(asFTCTModeEndFilePath)==false)
    {
        ForceDirectories(asFTCTModeEndFilePath);
    }

    if(TestIF_File.iSCKART_TryCnt==1)
    {
        asTempSendModeStart="00";
        RecvData.ModeStart.Status="00";
    }
    else if(TestIF_File.iSCKART_TryCnt==2)
    {
        asTempSendModeStart="10";
        RecvData.ModeStart.Status="10";
    }
    else if(TestIF_File.iSCKART_TryCnt==3)
    {
        asTempSendModeStart="20";
        RecvData.ModeStart.Status="20";
    }

    asFileName.sprintf("%s_%s_", fSCKART->sLotID, asTempSendModeStart); //Kirin 20200110 改成用asTempSendModeStart來儲存
    asFileName.cat_printf("%s.csv", fSCKART->sLotStartTime);
    asFileName=asFTCTModeEndFilePath+asFileName;
    SaveFilePtr=fopen(asFileName.c_str(), "wb+");
    fwrite(asData.c_str(),asData.Length(),1,SaveFilePtr);
    fclose(SaveFilePtr);

    asFTCTModeEndFilePath="D:\\FT-CT\\EndSaveFileBackup\\";   //備份檔
    if(asFTCTModeEndFilePath.Length()>0 &&
       DirectoryExists(asFTCTModeEndFilePath)==false)
    {
        ForceDirectories(asFTCTModeEndFilePath);
    }
    asFileName.sprintf("%s_%s_", fSCKART->sLotID, asTempSendModeStart); //Kirin 20191219 FTCT
    asFileName.cat_printf("%s.csv", fSCKART->sLotStartTime);
    asFileName=asFTCTModeEndFilePath+asFileName;
    SaveFilePtr=fopen(asFileName.c_str(), "wb+");
    fwrite(asData.c_str(),asData.Length(),1,SaveFilePtr);
    fclose(SaveFilePtr);
}
//---------------------------------------------------------------------------
void TRENESAS_Server::SetErrData(int iAlarmIndex,AnsiString asErrCode,AnsiString asErrComm)
{
    asErrGroup[iAlarmIndex]     =asErrCode;
    asErrCommGroup[iAlarmIndex] =asErrComm;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::GetErrCode(int iAlarmIndex,AnsiString &asErrCode,AnsiString &asErrComm)
{
    if(iAlarmIndex<=0 && iAlarmIndex>=40)
    {
        iAlarmIndex=40;
    }
    asErrCode=asErrGroup[iAlarmIndex];
    asErrComm=asErrCommGroup[iAlarmIndex];
}
//---------------------------------------------------------------------------
void TRENESAS_Server::InitErrData()
{
    SetErrData(0    ,"N0001","OK");
    SetErrData(1    ,"E0001","Handler not home");
    SetErrData(2    ,"E0002","Handler have device data");
    SetErrData(3    ,"E0003","Handler have device");
    SetErrData(4    ,"E0004","Can not load work file.");
    SetErrData(5    ,"E0005","Disiable FT-CT");
    SetErrData(6    ,"E0006","Out of order");
    SetErrData(7    ,"E0007","ART is Disable");
    SetErrData(8    ,"E0008","Recv TestEnd");
    SetErrData(9    ,"E0009","Input Qty is not entered. Enter the numbetr of input qty on the handler screen."); //Kirin 20220128
    SetErrData(10   ,"E0010","No Remove Tray");                                     //Kirin 20230130 Modify Err Msg
    SetErrData(11   ,"E0011","CleanOutFail");                                       //Kirin 20230130 Modify Err Msg
    SetErrData(12   ,"E0012","Can not do Start");                                   //RogerYang 20250911 : 針對啟動失敗時新增
    SetErrData(13   ,"E0013","Status code does not match");
    SetErrData(14   ,"E0014","Can not receive command before 41 process finish");
    SetErrData(15   ,"E0015","The Work file not found.");
    SetErrData(16   ,"E0016","Other err");
    SetErrData(17   ,"E0017","Other err");
    SetErrData(18   ,"E0018","Other err");
    SetErrData(19   ,"E0019","Other err");
    SetErrData(20   ,"E0020","Other err");
    SetErrData(21   ,"E0021","Other err");
    SetErrData(22   ,"E0022","Other err");
    SetErrData(23   ,"E0023","Other err");
    SetErrData(24   ,"E0024","Other err");
    SetErrData(25   ,"E0025","Other err");
    SetErrData(26   ,"E0026","Other err");
    SetErrData(27   ,"E0027","Other err");
    SetErrData(28   ,"E0028","Other err");
    SetErrData(29   ,"E0029","Other err");
    SetErrData(30   ,"E0030","Other err");
    SetErrData(31   ,"E0031","Other err");
    SetErrData(32   ,"E0032","Other err");
    SetErrData(33   ,"E0033","Other err");
    SetErrData(34   ,"E0034","Other err");
    SetErrData(35   ,"E0035","Other err");
    SetErrData(36   ,"E0036","Other err");
    SetErrData(37   ,"E0037","Other err");
    SetErrData(38   ,"E0038","Other err");
    SetErrData(39   ,"E0039","Other err");
    SetErrData(40   ,"E0040","Other err");
    SetErrData(41   ,"E0041","Other err");
    SetErrData(42   ,"E0042","Other err");
    SetErrData(43   ,"E0043","Other err");

}
//---------------------------------------------------------------------------
int TRENESAS_Server::GetNowErrInt()
{
    int iResult=0;
    //Kirin 20191219 FTCT
    //==>
//    if(fAllMotorHome==false)
//    {
//        iResult=1;
//    }
    //<==
    //Kirin 20191219 FTCT
    //if(Production_Info->iLoad>0)
    {
        iResult=2;
    }
    //else if(CheckHasAnyICUnderMachineOneCycleCondition())
    {
        iResult=3;
    }
    //else if(fMain->RENESAS_Server->iLoadWorkFileStatus==1)
    {
        iResult=4;
    }
    //else
    {
        iResult=0;
    }
    return iResult;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::OnCallHandlerOnLineEvent()
{
    iErrInt=GetNowErrInt(); //Kirin 20191218 FTCT
}
//---------------------------------------------------------------------------
void TRENESAS_Server::OnCallHandlerStartEvent()
{
   // fMain->Start(); //Kirin 20191218 FTCT
}
//---------------------------------------------------------------------------
void TRENESAS_Server::OnCallHandlerProdInfoEvent()
{
    //Kirin 20191218 FTCT
   // NewLastSet->asRENESASLotNo          =RecvData.ProdInfo.LotNo        ;
   // NewLastSet->asRENESASLotID          =RecvData.ProdInfo.LotID        ;
   // NewLastSet->asRENESASHdParameter    =RecvData.ProdInfo.HdParameter  ;
   // NewLastSet->asRENESASProduct        =RecvData.ProdInfo.Product      ;
   // NewLastSet->asRENESASStepCode       =RecvData.ProdInfo.StepCode     ;
   // NewLastSet->asRENESASQty            =RecvData.ProdInfo.Qty          ;
   // NewLastSet->asRENESASLotStartTime   =RecvData.ProdInfo.Date         ;
   // NewLastSet->SaveToINI();
    iLoadWorkFileStatus=0;
    //iErrInt=GetNowErrInt();  //Kirin 20191219 這裡不用了
    iErrInt=0;
    if(iErrInt==0)
    {
        //fMain->Pause(); //Kirin 20191219 FTCT
        //RENESAS_LoadWorkSetup(NewLastSet->asRENESASHdParameter);      //Kirin 20191218 FTCT
        //iErrInt=GetNowErrInt();  //Kirin 20191219 這裡不用了
    }
}
//===========================================================================
// 函式說明: RENESAS FTCT載入工作檔
// v1.0 : //Kirin 20191218 RENESAS FTCT載入工作檔
//===========================================================================
//bool TRENESAS_Server::RENESAS_LoadWorkSetup(AnsiString asWorkFileName)
//{
//    AnsiString asWorkFileFullPath;
//    bool bTempLoadStatus=false;
//    asWorkFileFullPath="D:\\HT2000\\Data\\"+asWorkFileName+"\\";
//        asWorkFileFullPath=asWorkFileFullPath+asWorkFileName+".BLD";
//        //if(AnsiString(fMain->edMainWorkFile->Caption.UpperCase())!=AnsiString(asWorkFileFullPath.UpperCase())) //Kirin 20200716 轉成大寫比對  //Kirin 20191220 與原本不同才需要載入
//        {
//            //bTempLoadStatus=fMain->LoadWorkSetup(asWorkFileFullPath.c_str());
//            if(bTempLoadStatus)
//            {
//                //fMain->edMainWorkFile->Caption=asWorkFileFullPath;
//                //strcpy(LastSet.LastOpenFilename,fMain->edMainWorkFile->Caption.c_str());
//                //Kirin 20200107 AddLog
//                //==>
//                asMsg.sprintf("FTCT Load WorkFile %d ",asWorkFileFullPath);
//                //PreSaveLog->Save("D:\\TempLog\\FTCTLog\\FTCTLog.txt",asMsg,true);
//                //<==
//                //Kirin 20200107 AddLog
//            }
//        }
//        //else
//        {
//            bTempLoadStatus=true;
//            //Kirin 20200107 AddLog
//            //==>
//            asMsg.sprintf("FTCT Load WorkFile , alreay Load %d no load workfile",asWorkFileFullPath);
//            //PreSaveLog->Save("D:\\TempLog\\FTCTLog\\FTCTLog.txt",asMsg,true);
//            //<==
//            //Kirin 20200107 AddLog
//        }
//    return bTempLoadStatus;
//}
//===========================================================================
// 函式說明: InitParamter
// v1.0 : //Kirin 20220118 RENESAS InitParamter
//===========================================================================
void TRENESAS_Server::InitParamter()
{
    RecvData.OnLine.iRecv       =0;
    RecvData.ProdInfo.iRecv     =0;
    RecvData.ModeStart.iRecv    =0;
    RecvData.HandlerStart.iRecv =0;
    RecvData.ModeEnd.iRecv      =0;
    RecvData.LotEnd.iRecv       =0;
    RecvData.TestEnd.iRecv      =0;
    RecvData.SendAlarm.iRecv    =0;
    iHaveRecvTestEnd            =0;
    iLoadWorkFileStatus         =0;//Kirin 20220127
    iModeEndCounterClear=false;
    iHaveRecvModeStart=false;
    iHaveProdInfoStart=false;
    iProdStartProcess=false; //Kirin 20200210 iProdStartProcess
    bSend71WaitReply30=false;//Kirin 20220224 bSend71WaitReply30
    bSend71WaitReply30Alarm=false;//Kirin 20220224 bSend71WaitReply30Alarm
    b70_Disconnect=false;//Kirin 20220225 b70_Disconnect
    b70_DisconnectAlarm=false;//Kirin 20220225
    b70_Do70MsgAlarm=false;
    iProcTask=1;
    iFTCTSendCleanOut =false;//Kirin 20221121 iFTCTSendCleanOut
    bCanRecvCleanOut_B0=false;//Kirin 20221123 bCanRecvB0
    bCleanCountFail=false;//Kirin 20230814 add 清量失敗旗標
    bSend71WiteReply50_40_90=false;//Kirin 20240312
    bSend71WaitReply50_40_90Alarm=false;//Kirin 20240312
    bWait20or40=false;
    bWait20or40Alarm=false;
    bRenesasFTCTAlarm=false;
    bLoadingCountFullFlag=false;
    bNeedSupply=false;
    bSend41WaitTimeout=false;
    bSend41WaitTimeoutAlarm=false;
//    bReturn41Flag=false;
    sModeStartTmp="";
    bTrayFeedDone=false;
    bContFrmShow=false;                                                         //RogerYang 20251107 : Add Continue Form
    ResetFTCTSeq();

}
//===========================================================================
// 函式說明: 收到70要拒絕 除00,40,90之外的指令
// v1.0 : //Kirin 20220118 RENESAS bCanNotUse_Cmd_With_TestEnd
//===========================================================================
bool TRENESAS_Server::bCanNotUse_Cmd_With_TestEnd(AnsiString asCmd)
{
    //Kirin 20220118 收到70要拒絕 除00,40,90之外的指令
    bool bResult=false;
    int iFindIndex=-1;
    TStringList *strPassCmd=new TStringList;
    strPassCmd->Add("00");
    strPassCmd->Add("40");
    strPassCmd->Add("70");
    strPassCmd->Add("90");
    iFindIndex=strPassCmd->IndexOf(asCmd);

    if(iFindIndex==-1)
    {
        if(iHaveRecvTestEnd==1)
        {
            bResult=true;
            iErrInt=8;
            GetErrCode(iErrInt,asSendErrCode,asSendErrComm);
            SendReplyToFTCT(ServerFrm->asFTCTLastSeqStatus);
        }
    }
    strPassCmd->Clear();
    delete strPassCmd;
    strPassCmd=NULL;
    return bResult;
}
//===========================================================================
// 函式說明: SendReplyToFTCT
// v1.0 : //Kirin 20220118 RENESAS SendReplyToFTCT
//===========================================================================
void TRENESAS_Server::SendReplyToFTCT(AnsiString asCmd, int iErrorCode)
{
    if(asCmd==RENESAS_CMD.OnLineCMD)
    {
        SendOnLine(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.ProductInfoCMD)
    {
        SendProductInfo(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.StartCMD)
    {
        SendStart(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.HandlerStartCMD)
    {
        SendHandlerStart(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.EndCMD)
    {
        SendEnd(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.LotEndCMD)
    {
        SendLotEnd(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.TestEndCMD)
    {
        SendTestEnd(iErrorCode);
    }
    else if(asCmd==RENESAS_CMD.SendAlarmCMD)
    {
        SendAlarm(iErrorCode);
    }
}
//===========================================================================
// 函式說明: SaveTaskLog
// v1.0 : //Kirin 20220127 SaveTaskLog
//===========================================================================
void TRENESAS_Server::SaveTaskLog(char *Task_Name,int iTask)
{
    #ifdef NOT_RECORD_LOG
        return;
    #endif

    TDateTime tTime=Now();
    AnsiString asFileName,asDirectory,asStr,asNewFileName;

    static AnsiString BackupStr;

    if(BackupStr==AnsiString(Task_Name))
        return;

    BackupStr=Task_Name;

    asStr.printf("%s: %s , Task:%d",tTime.FormatString("yyyy/mm/dd hh:nn:ss").c_str(),Task_Name,iTask);

    if (!DirectoryExists(asRenesasTaskPath.c_str()))
    {
        CreateDirectory(asRenesasTaskPath.c_str(),NULL);
    }

    asDirectory.sprintf("%s\\Task",asRenesasTaskPath);

    if (!DirectoryExists(asDirectory.c_str()))
    {
        CreateDirectory(asDirectory.c_str(),NULL);
    }

    asFileName = asDirectory + "\\Renesas_Server_Task.txt";
    FILE *Fp = fopen(asFileName.c_str(),"ab");

    if (Fp != NULL)
    {
        asStr +="\r\n";
        fwrite(asStr.c_str(),1,asStr.Length(),Fp);
        fclose(Fp);
        //CheckTaskFileToBackup(asFileName);
        return;
    }
    else
    {

    }

    return;
}
//===========================================================================
// 函式說明: DoCheckAfter71CmdTimeOut
// v1.0 : //Kirin 20220224 DoCheckAfter71CmdTimeOut
//===========================================================================
void TRENESAS_Server::DoCheckAfter71CmdTimeOut()
{
    if(bSend71WaitReply30)
    {
        //if(CheckTimeOut(TimeOut_Send71WaitReply30,NewLastSet->i30RecvTimeOut))
        if(CheckTimeOut(TimeOut_Send71WaitReply30, 600000))  //待新增變數
        {
            bSend71WaitReply30=false;
            bSend71WaitReply30Alarm=true;
        }
    }
}
//===========================================================================
// 函式說明: DoCheckAfter41CmdTimeOut
//===========================================================================
void TRENESAS_Server::DoCheckAfter41CmdTimeOut()
{
    if(bSend41WaitTimeout)
    {
        //if(CheckTimeOut(TimeOut_Send71WaitReply30,NewLastSet->i30RecvTimeOut))
        if(CheckTimeOut(TimeOut_Send41WaitTimeout, 60000))    //待新增變數
        {
            bSend41WaitTimeout=false;
            bSend41WaitTimeoutAlarm=true;
        }
    }
}
//===========================================================================
// 函式說明: DoCheckAfter71_50_40_90CmdTimeOut
// v1.0 : //Kirin 20240312 DoCheckAfter71_50_40_90CmdTimeOut
//===========================================================================
void TRENESAS_Server::DoCheckAfter71_50_40_90CmdTimeOut()
{
    if(bSend71WiteReply50_40_90)
    {
        //if(CheckTimeOut(TimeOut_Send71WaitReply50_40_90,NewLastSet->i50_40_90RecvTimeOut))
        if(CheckTimeOut(TimeOut_Send71WaitReply50_40_90, 600000))   //待新增變數
        {
            bSend71WiteReply50_40_90=false;
            bSend71WaitReply50_40_90Alarm=true;
        }
    }
}
//===========================================================================
// 函式說明: iGetAfter71CmdTimeOut
// v1.0 : //Kirin 20220224 iGetAfter71CmdTimeOut
//===========================================================================
int TRENESAS_Server::iGetAfter71CmdTimeOut()
{
    TDateTime NowTimeStamp=Now();
    int Diff=0;
    if(bSend71WaitReply30)
        Diff=MySecondsBetween(TimeOut_Send71WaitReply30, NowTimeStamp);           //Kirin 20190919
    else
        Diff=0;
    return Diff;
}
//===========================================================================
// 函式說明: iGetAfter71_50_40_90CmdTimeOut
// v1.0 : //Kirin 20240312 iGetAfter71_50_40_90CmdTimeOut
//===========================================================================
int TRENESAS_Server::iGetAfter71_50_40_90CmdTimeOut()
{
    TDateTime NowTimeStamp=Now();
    int Diff=0;
    if(bSend71WiteReply50_40_90)
        Diff=MySecondsBetween(TimeOut_Send71WaitReply50_40_90, NowTimeStamp);
    else
        Diff=0;
    return Diff;
}
//===========================================================================
// 函式說明: GetDelayStart
// v1.0 : //Kirin 20220224 GetDelayStart
//===========================================================================
TDateTime GetDelayStart()
{
    return Now();
}
//===========================================================================
// 函式說明: TimeOut
// v1.0 : //Kirin 20220224 TimeOut
//===========================================================================
bool CheckTimeOut(TDateTime DelayStamp,DWORD OutTime)
{
    TDateTime NowTimeStamp=Now();
    unsigned int Diff=MySecondsBetween(DelayStamp, NowTimeStamp);                 //Kirin 20190919
    if(Diff>OutTime)
        return true;
    else
        return false;
}
//<==
//Kirin 20170320
//===========================================================================
void TRENESAS_Server::DoCheckWait20or40TimeOut()
{
    if(bWait20or40==true)
    {
        //if(CheckTimeOut(TimeOut_Send71WaitReply50_40_90,NewLastSet->i50_40_90RecvTimeOut))
        if(CheckTimeOut(TimeOut_Wait20or40, 600000))
        {
            bWait20or40=false;
            bWait20or40Alarm=true;
        }
    }
}
//---------------------------------------------------------------------------
bool TRENESAS_Server::DoGet20or40After71()
{
    if(bWait20or40==true || bWait20or40Alarm==true)
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
bool TRENESAS_Server::DoNeedSupplyOrNot(bool bNotEnough)
{
    int ret=-1;
    AnsiString str, str2;
    bool bOverPick=fSCKART->bLdCntExdInputCnt; 
    if(bOverPick==false && bNotEnough==false) //tray盤無IC && 已滿足QTY數量
    {
        bNeedSupply=false;
    }
    else if(bLoadingCountFullFlag==false &&          //只會問一次
            (bOverPick==true || bNotEnough==true))  //還有IC，或是不足
    {
        bLoadingCountFullFlag=true;
        if(bOverPick==true)
        {
            str.sprintf("數量多%d 枚", LastSet.iSCKARTInputCT-fSCKART->iInputCount);
            str2.sprintf("Current %d / QTY %d", LastSet.iSCKARTInputCT, fSCKART->iInputCount);
            ret=ShowLotEndMessage(str, str2, false);
        }
        if(bNotEnough==true)
            ret=ShowLotEndMessage("The number of receipts and the number of Lots(QTY) do not match.\r\n Want to supply untested products?", "", false);
        if(ret==1)  //供給
        {
            bNeedSupply=true;
        }
        else        //不供給
        {
            bNeedSupply=false;
        }
    }
    return bNeedSupply;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::FTCTManStartUnlock(AnsiString str)                        //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
{
    if(str.Pos("Start blocked by FTCT command")!=0)                             //不允許啟動的FTCT message，不能解綁
        return;

    if(fSCKART->iCurrent93KARTStep<1)                                           //避免還沒開始作業，隨便一個alarm就讓handler可以啟動
        return;

    bRenFTCTManualStartTmp=fSCKART->iManualStart;
    fSCKART->iManualStart=true;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::FTCTManStartLock()                                        //RogerYang 20251002 : 瑞薩FT-CT 復原manualstart
{
    if(bTrayFeedDone==true)
        fSCKART->iManualStart=false;  //need lock when trayfeed done
    else
        fSCKART->iManualStart=bRenFTCTManualStartTmp;
}
//---------------------------------------------------------------------------
void TRENESAS_Server::ResetFTCTSeq()                                            //RogerYang 20251017 : 瑞薩FT-CT
{
//    if(HasICUnderMachine()==true)
//    {
//        ShowMyMessage("Has IC Under Machine!");
//    }
//    else
//    {
        ServerFrm->asFTCTLastSeqStatus="";
        ServerFrm->asHandlerLastSeqStatus=RENESAS_CMD.LotEndReplyCMD;
        sLastLegalState=RENESAS_CMD.LotEndReplyCMD;
        iHaveRecvTestEnd=0;
        fSCKART->iInputCount=0;
        fSCKART->iLotCount=0;
        fSCKART->AccessFile(false, 1);
//    }

    //fSCKART->iManualStart=false;    //alreay lock when asFTCTLastSeqStatus=""
}
//---------------------------------------------------------------------------
bool TRENESAS_Server::CheckInputCommand(AnsiString str)                         //RogerYang 20251017 : 瑞薩FT-CT 檢查命令
{
    bool rlt=true;

    if(str==RENESAS_CMD.OnLineCMD)              //00
    {

    }
    else if(str==RENESAS_CMD.ProductInfoCMD)    //10
    {
        if(sLastLegalState==RENESAS_CMD.StartReplyCMD           || //當前狀態為 21 51 70 71 31 時，不接受10
            sLastLegalState==RENESAS_CMD.HandlerStartReplyCMD   ||
            sLastLegalState==RENESAS_CMD.TestEndCMD             ||
            sLastLegalState==RENESAS_CMD.TestEndReplyCMD        ||
            sLastLegalState==RENESAS_CMD.EndReplyCMD)
        {
            rlt=false;
        }
    }
    else if(str==RENESAS_CMD.StartCMD)          //20
    {
        if(sLastLegalState==RENESAS_CMD.HandlerStartReplyCMD    || //當前狀態為 51 70 71 41 時，不接受20
            sLastLegalState==RENESAS_CMD.TestEndCMD             ||
            sLastLegalState==RENESAS_CMD.TestEndReplyCMD        ||
            sLastLegalState==RENESAS_CMD.LotEndReplyCMD)
        {
            rlt=false;
        }
    }
    else if(str==RENESAS_CMD.HandlerStartCMD)   //50
    {
        if(sLastLegalState==RENESAS_CMD.ProductInfoReplyCMD     || //當前狀態為 11 70 31 41 時，不接受50
            sLastLegalState==RENESAS_CMD.TestEndCMD             ||
            sLastLegalState==RENESAS_CMD.EndReplyCMD            ||
            sLastLegalState==RENESAS_CMD.LotEndReplyCMD)
        {
            rlt=false;
        }
    }
    else if(str==RENESAS_CMD.EndCMD)            //30
    {
        if(sLastLegalState==RENESAS_CMD.ProductInfoReplyCMD     || //當前狀態為 11 70 41時，不接受30
            sLastLegalState==RENESAS_CMD.TestEndCMD             ||
            sLastLegalState==RENESAS_CMD.LotEndReplyCMD)
        {
            rlt=false;
        }
    }
    else if(str==RENESAS_CMD.LotEndCMD)         //40
    {

    }
    else if(str==RENESAS_CMD.TestEndCMD)        //70
    {
        if(sLastLegalState==RENESAS_CMD.ProductInfoReplyCMD     || //當前狀態為 11 31 41 時，不接受70
            sLastLegalState==RENESAS_CMD.EndReplyCMD            ||
            sLastLegalState==RENESAS_CMD.LotEndReplyCMD)
        {
            rlt=false;
        }
    }
    else if(str==RENESAS_CMD.SendAlarmCMD)      //90
    {

    }
    else
    {
        rlt=false;
    }
    return rlt;
}
//---------------------------------------------------------------------------
bool TRENESAS_Server::Check71CommandDuring50()                                  //RogerYang 20251019 : 瑞薩FT-CT
{
    bool rlt=true;
    static int icnt=0;

    if(iFTRTCntTmpFor70==TestIF_File.iSCKART_TryCnt)    //same FT/RT Cycle
        bCanReturn71=true;
    else
        bCanReturn71=false;

    if(iHaveRecvTestEnd==1 &&                                                   //RogerYang 20250913 : FT-CT "send 71"
        bCanReturn71==true)    //need to be the smae FT/RT cycle
    {
        if(icnt<10)
        {                                                                       //RogerYang 20251106 : 先等超過2秒，以防止突然收到30之後的FTCT斷線來不及知道
            MySleep(200);
            icnt++;
        }
        else
        {
            icnt=0;
            if(b70_Disconnect==true)                                            //RogerYang 20251106 : FT-CT在70與71之間有斷線的狀況時，再回復71的同時跳警報
            {
                b70_DisconnectAlarm=true; //fMain timer3 showmessage
                b70_Disconnect=false;
            }
            else                                                                //RogerYang 20251111 新增70期間收到違法(30)命令
            {
                DoSendTestEnd();
                iHaveRecvTestEnd=0;  //這裡清0
                ShowContinueMessageByReply71(b70_DisconnectAlarm);              //RogerYang 20251107 : Add Continue Form
                b70_DisconnectAlarm=false;
            }
        }
    }

    if(iFTRTCntTmpFor70!=TestIF_File.iSCKART_TryCnt)
    {
        rlt=true;   //Led FT cleanout finish to RT process
    }
    else if(sLastLegalState==RENESAS_CMD.TestEndCMD)   //wait between 70 and71      avoid other command to release cleanout
    {
        rlt=false;
    }
    else if(DoGet20or40After71()==false) //wait after 71 等命令或timeout，判斷是否RT，或LotEnd
    {
        rlt=false;
    }
    else    //這裡已經知道
    {

    }
    return rlt;
}
//---------------------------------------------------------------------------
__fastcall TErrorForm::TErrorForm(TComponent* Owner):TForm(Owner, 0)
{
    this->Caption="Reply FTCT Error Command";
    this->BorderStyle=bsDialog;
    this->Position=poScreenCenter;
    this->Width=1200;
    this->Height=250;
    this->Color=(TColor)0x00CCFFFF;
    this->FormStyle=fsStayOnTop;
    this->BorderIcons=TBorderIcons()<<biSystemMenu;

    lbl=new TLabel(this);
    lbl->Parent=this;
    lbl->AutoSize=false;
    lbl->Alignment=taCenter;
    lbl->Layout=tlCenter;
    lbl->SetBounds(20, 20, this->Width-40, 150);
    lbl->WordWrap=true;
    lbl->Font->Size=28;
    lbl->Font->Color=clRed;
    lbl->Transparent=true;

    btnClose=new TButton(this);
    btnClose->Parent=this;
    btnClose->Caption="Close";
    btnClose->SetBounds((this->Width-100)/2, this->Height-70, 100, 30);
    btnClose->OnClick=BtnCloseClick;
}
//---------------------------------------------------------------------------
void __fastcall TErrorForm::BtnCloseClick(TObject* Sender)
{
    this->Hide(); // 隱藏而非釋放
    bReplyFTCTAlarm=false;
    bAlarmBuzzer=false;
}
//---------------------------------------------------------------------------
void TErrorForm::SetMessage(AnsiString str1,AnsiString str2)
{
    lbl->Caption="Return Error TO FTCT : "+str1+"\r\nCode : "+str2;
}
//---------------------------------------------------------------------------
TErrorForm* TRENESAS_Server::frm = NULL;
void __fastcall TRENESAS_Server::ShowErrorMessageByReplyError(AnsiString sSendErrCode, AnsiString sSendErrComm)
{
    if(!frm)
        frm=new TErrorForm(Application); // 第一次呼叫才建立

    frm->SetMessage(sSendErrCode, sSendErrComm);

    if(!frm->Visible)
        frm->Show();
    else
        frm->BringToFront();

    bReplyFTCTAlarm=true;
    bAlarmBuzzer=true;
}
//---------------------------------------------------------------------------
__fastcall TContinueForm::TContinueForm(TComponent* Owner):TForm(Owner, 0)      //RogerYang 20251107 : Add Continue Form
{
    this->Caption="(FTCT) Waiting After Reply 71 Command";
    this->BorderStyle=bsDialog;
    this->Position=poScreenCenter;
    this->Width=800;
    this->Height=300;
    this->Color=(TColor)0x00CCFFCC;
    this->FormStyle=fsStayOnTop;
    this->BorderIcons=TBorderIcons()<<biSystemMenu;

    lbl=new TLabel(this);
    lbl->Parent=this;
    lbl->AutoSize=false;
    lbl->Alignment=taCenter;
    lbl->Layout=tlCenter;
    lbl->SetBounds(20, 20, this->Width-40, 150);
    lbl->WordWrap=true;
    lbl->Font->Size=28;
    lbl->Font->Color=clBlue;
    lbl->Transparent=true;
    //lbl->Caption="Waiting After Reply 71 Command";

    pnl=new TPanel(this);
    pnl->Parent=this;
    pnl->Caption="CONTINUE";
    pnl->Font->Name="微軟正黑體";
    pnl->Font->Style=TFontStyles()<<fsBold;
    pnl->Font->Size=36;
    pnl->SetBounds((this->Width-300)/2, this->Height-150, 300, 100);
    pnl->OnClick=BtnCloseClick;
}
//---------------------------------------------------------------------------
void __fastcall TContinueForm::BtnCloseClick(TObject* Sender)                   //RogerYang 20251107 : Add Continue Form
{
    this->Hide(); // 隱藏而非釋放
    bContinueMessageByReply71=false;
    bAlarmBuzzer=false;
}
//---------------------------------------------------------------------------
TContinueForm* TRENESAS_Server::contfrm=NULL;                                   //RogerYang 20251107 : Add Continue Form
void __fastcall TRENESAS_Server::ShowContinueMessageByReply71(bool bAlarm)
{
    if(!contfrm)
        contfrm=new TContinueForm(Application); // 第一次呼叫才建立

    AnsiString Str="";
    if(bAlarm==true)
    {
        contfrm->lbl->Font->Color=clRed;
        Str="FTCT DisConnect during 70!";
        bContinueMessageByReply71=true;
        bAlarmBuzzer=true;
    }
    else
    {
        contfrm->lbl->Font->Color=clBlue;
        Str="Waiting After Reply 71 Command";
    }
    contfrm->lbl->Caption=Str;

    if(!contfrm->Visible)
        contfrm->Show();
    else
        contfrm->BringToFront();
}
//---------------------------------------------------------------------------
void TRENESAS_Server::CloseContinueMessageForm()                                //RogerYang 20251107 : Add Continue Form
{
    if(contfrm && contfrm->Visible==true)
        contfrm->Hide(); // 隱藏而非釋放
    bContinueMessageByReply71=false;
    bAlarmBuzzer=false;
}
//===========================================================================