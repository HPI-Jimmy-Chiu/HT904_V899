//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "TesterTCP.h"

#include "cmydef.h"
#include "common.h"
#include "main.h"
#include "Command.h"
#include "uLotInfo.h"
#include "uHGemHT9045.h"
#include "mymessbox.h"
#include "WinSocketErrorCode.h"
#include "cContactCT.h"
#include "cSocket.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTesterTCP *fTesterTCP;
extern unsigned int iBin[4][8];
extern bool bEcho, bExist;
//------------------------------------------------------------------------------
__fastcall TfTesterTCP::TfTesterTCP(TComponent* Owner)
    : TForm(Owner)
{
    OSRecipe="";                                                                //Steven 20230116 : OS測試機傳送工作檔名
    AnsiString Str;
    TComboBox *tempComboBox[32]={cbSite01, cbSite02, cbSite03, cbSite04, cbSite05, cbSite06, cbSite07, cbSite08,
                                 cbSite09, cbSite10, cbSite11, cbSite12, cbSite13, cbSite14, cbSite15, cbSite16,
                                 cbSite17, cbSite18, cbSite19, cbSite20, cbSite21, cbSite22, cbSite23, cbSite24,
                                 cbSite25, cbSite26, cbSite27, cbSite28, cbSite29, cbSite30, cbSite31, cbSite32};

    TPanel *tempPanel[32]={plSite01, plSite02, plSite03, plSite04, plSite05, plSite06, plSite07, plSite08,
                           plSite09, plSite10, plSite11, plSite12, plSite13, plSite14, plSite15, plSite16,
                           plSite17, plSite18, plSite19, plSite20, plSite21, plSite22, plSite23, plSite24,
                           plSite25, plSite26, plSite27, plSite28, plSite29, plSite30, plSite31, plSite32};

    TCheckBox *tempCheckBox[32]={cbSiteOn01, cbSiteOn02, cbSiteOn03, cbSiteOn04, cbSiteOn05, cbSiteOn06, cbSiteOn07, cbSiteOn08,
                                 cbSiteOn09, cbSiteOn10, cbSiteOn11, cbSiteOn12, cbSiteOn13, cbSiteOn14, cbSiteOn15, cbSiteOn16,
                                 cbSiteOn17, cbSiteOn18, cbSiteOn19, cbSiteOn20, cbSiteOn21, cbSiteOn22, cbSiteOn23, cbSiteOn24,
                                 cbSiteOn25, cbSiteOn26, cbSiteOn27, cbSiteOn28, cbSiteOn29, cbSiteOn30, cbSiteOn31, cbSiteOn32};

    TLabel *tempLabel[32]={labOcr01, labOcr02, labOcr03, labOcr04, labOcr05, labOcr06, labOcr07, labOcr08,
                           labOcr09, labOcr10, labOcr11, labOcr12, labOcr13, labOcr14, labOcr15, labOcr16,
                           labOcr17, labOcr18, labOcr19, labOcr20, labOcr21, labOcr22, labOcr23, labOcr24,
                           labOcr25, labOcr26, labOcr27, labOcr28, labOcr29, labOcr30, labOcr31, labOcr32};

    for(int i=0; i<32; i++)
    {
        cbSimulateBin[i]=tempComboBox[i];
        cbSimulateBin[i]->ItemIndex=0;
        plSite[i]=tempPanel[i];
        plSite[i]->Caption="--";
        cbSiteOn[i]=tempCheckBox[i];
        cbSiteOn[i]->Checked=false;
        labOcr[i]=tempLabel[i];
        labOcr[i]->Caption="";
    }

    SocketTCPIPReceiveList          =new TStringList;
    SummaryHead                     =new TStringList;
    bEnableTCPIPChannelConnect      =true;
    SimulateStart                   =false;
    bTCPError                       =false;
    SocketTCPIPReceiveList->Clear();
    SummaryHead->Clear();

    for(int iAuto=0; iAuto<eTrayCount; iAuto++)
    {
        Str.sprintf("GroupBox_%s", s6TrayName[iAuto]);
        bFlag[iAuto]=true;
        if(CUSTOMER_CODE==CC_JSCC_OS &&                                         //RogerYang 20260128 : 田揚志畫面修改需求
            Str.Pos("Fix")!=0)
        {
            bFlag[iAuto]=false;
        }

        GroupBox[iAuto]         =new TGroupBox(scrlbxSummary);
        GroupBox[iAuto]->Name   =Str;
        GroupBox[iAuto]->Caption=s6TrayName[iAuto];
        GroupBox[iAuto]->Parent =scrlbxSummary;
        GroupBox[iAuto]->Height =100;
        GroupBox[iAuto]->Top    =iAuto*100;
        GroupBox[iAuto]->Align  =alTop;
        GroupBox[iAuto]->Font->Name="Courier New";
        GroupBox[iAuto]->Font->Color=clWhite;

        Str.sprintf("RichEdit_%s", s6TrayName[iAuto]);
        RichEdit[iAuto]         =new TMemo(GroupBox[iAuto]);
        RichEdit[iAuto]->Name   =Str;
        RichEdit[iAuto]->Parent =GroupBox[iAuto];
        RichEdit[iAuto]->Align  =alClient;
        RichEdit[iAuto]->Color  =clWhite;                                       //RogerYang 20260128 : 田揚志畫面修改需求
        RichEdit[iAuto]->Font->Name="Courier New";
        RichEdit[iAuto]->Font->Color=clBlack;                                   //RogerYang 20260128 : 田揚志畫面修改需求
        RichEdit[iAuto]->ScrollBars=ssBoth;
        RichEdit[iAuto]->Clear();
    }

    rgUnloader->ItemIndex=1;
    for(int i=0; i<eTrayCount; i++)
    {
        GroupBox[i]->Visible=false;
        GroupBox[i]->Align=alNone;
    }
    GroupBox[rgUnloader->ItemIndex-1]->Visible=true;
    GroupBox[rgUnloader->ItemIndex-1]->Align=alClient;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::FormDestroy(TObject *Sender)
{
    SocketTCPIPReceiveList->Clear();
    delete SocketTCPIPReceiveList;
    SummaryHead->Clear();
    delete SummaryHead;

    for(int iAuto=0; iAuto<eTrayCount; iAuto++)
    {
        delete RichEdit[iAuto];
        delete GroupBox[iAuto];
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::FormShow(TObject *Sender)
{
    edTCPIP_Address->Text=TestIF_File.asTester_Address;
    edTCPIP_Port->Text=TestIF_File.iTester_Port;

    if(CUSTOMER_CODE==CC_JSCC_OS)                                               //RogerYang 20260128 : 田揚志畫面修改需求
    {
        pgcTcpIp->ActivePageIndex=3;

        AnsiString szDir="";                                                    //RogerYang 20260210 : 田揚志需求，有設定的才可以一次放全部，OS報表要照Tray盤放料順序顯示
        LastFileName=GetLastOpenFN();
        if(CosFunction.bSaveArmSpeedByMachine && IniConfig.bA57_1SaveArmSpeedByMachine)
    {
            szDir=sSaveByMachine;
        }
        else
        {
            szDir.sprintf("%s%s", DataPath, LastFileName);
    }
        MyForceDirectories(szDir);
        szDir+="\\ArmCondition.Data";

        bOutArmVariAuto_OS[0]=CheckAndReadIniData(szDir, "Output Arm", "JSCC_OS_Use Auto1", 1);   //default auto 1 enable
        bOutArmVariAuto_OS[1]=CheckAndReadIniData(szDir, "Output Arm", "JSCC_OS_Use Auto2", 0);
        bOutArmVariAuto_OS[2]=CheckAndReadIniData(szDir, "Output Arm", "JSCC_OS_Use Auto3", 0);
        chkAuto1_OS->Checked=bOutArmVariAuto_OS[0];
        chkAuto2_OS->Checked=bOutArmVariAuto_OS[1];
        chkAuto3_OS->Checked=bOutArmVariAuto_OS[2];
    }
    else
    {
        pgcTcpIp->ActivePageIndex=0;
        for(int iAuto=0; iAuto<eTrayCount; iAuto++)
        {
            GroupBox[iAuto]->Visible=
                (Prod.iTrayType[iAuto]!=tNotUse && bFlag[iAuto]==true);             //RogerYang 20260128 : 田揚志畫面修改需求
        }
    }
    fShow=true;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ClientSocket_TCPIPConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddTCPIPCommunicationLog(0, "Connect to tester OK");
    fLotInfo->labTCPIPStatus->Caption="ON-LINE";                                //Connect OK
    fLotInfo->labTCPIPStatus->Color=clGreen;
    bConnectOK=true;
    bConnect=false;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ClientSocket_TCPIPDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AddTCPIPCommunicationLog(0, "Disconnect from tester.");
    fLotInfo->labTCPIPStatus->Caption="OFF-LINE";                               //Connect NG
    fLotInfo->labTCPIPStatus->Color=clRed;
    bConnectOK=false;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::TimerTCPIPConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true || LastSet.iTester==OFF_LINE)        //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    static int Count1=20;

    if(bTCPError)                                                               //Steven 20231113 : TCP/IP Test 連線異常處置
    {
        ShowMyMessage("TCP/IP error for tester!", ErrorMessage);
        bTCPError=false;
    }

    if(ClientSocket_TCPIP->Active                       &&                      //相同的IP跟Port就不要重新連線
       ClientSocket_TCPIP->Address==TestIF_File.asTester_Address &&
       ClientSocket_TCPIP->Port==TestIF_File.iTester_Port)
    {
    }
    else
    {
        if(ClientSocket_TCPIP->Active)
        {
            ClientSocket_TCPIP->Close();
            SendTCPIPCommand(0, "Disconnecting from tester.", "");
            Count1=0;
        }
    }

    if(ClientSocket_TCPIP->Active==false && bEnableTCPIPChannelConnect==true)
    {
        bTCPError=false;
        Count1++;
        if(Count1>30)                                                           //Steven 20231113 : 改成30秒重試一次
        {
            ClientSocket_TCPIP->Address   =TestIF_File.asTester_Address;
            ClientSocket_TCPIP->Port      =TestIF_File.iTester_Port;
            SendTCPIPCommand(0, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_TCPIP->Open();
            }
            catch(...)
            {
                LogClientSocketExceptionError(ClientSocket_TCPIP, "TCP/IP Tester Connect");
            }
            SendTCPIPCommand(0, "ClientSocket Open", "");
            Count1=0;
        }
    }
    bTimerRun=false;
}
//------------------------------------------------------------------------------
void TfTesterTCP::SendTCPIPCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if(mmTCPIPCommLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    GetTimeInfo();

    if(LastSet.iTester==OFF_LINE)
    {
        Msg2+=Msg2+"OffLine";
    }

    if(Msg2=="")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s", Msg2);

    AddTCPIPCommunicationLog(index, Log);

    if(Msg2=="")
    {
    }
    else
    {
        if(LastSet.iTester==OFF_LINE)
            return;

        if(ClientSocket_TCPIP->Active)
        {
            ClientSocket_TCPIP->Socket->SendText(Msg2+"\r\n");
        }
    }
}
//------------------------------------------------------------------------------
void TfTesterTCP::AddTCPIPCommunicationLog(int index, AnsiString Str)
{
    AnsiString Path, Name, Log;

    Name="TCPIP";

    if(Str==" ")
        Log.sprintf(" ");
    else
        Log.sprintf("%04d-%02d-%02d,%02d:%02d:%02d.%03d,%s,%s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Name, Str);

    if(mmTCPIPCommLog->Lines->Count>2000)
    {
        mmTCPIPCommLog->Clear();
        fLotInfo->mmTesterLog->Clear();                                         //Steven 20231113 : Add
    }

    mmTCPIPCommLog->Lines->Add(Log);
    fLotInfo->mmTesterLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d\\%04d_%02d_%02d", asTestTCPIPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);
    Path.sprintf("%s\\%04d_%02d\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTestTCPIPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::btTCPIP_TriggerClick(TObject *Sender)
{
    AnsiString strSendCMD="";
    strSendCMD=edTCPIP_CMD->Text;
    SendTCPIPCommand(0, " ", strSendCMD);
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::btTCPIP_ConnectClick(TObject *Sender)
{
    TimerTCPIPConnect->Enabled=true;
    bEnableTCPIPChannelConnect=true;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::btTCPIP_DisconnectClick(TObject *Sender)
{
    bEnableTCPIPChannelConnect=false;
    TimerTCPIPConnect->Enabled=false;
    ClientSocket_TCPIP->Close();
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ClientSocket_TCPIPRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }

    AnsiString EthernetBuffer;
    int iBufferLenght=0;
    AnsiString Log;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)
    {
        EthernetBuffer=Socket->ReceiveText();
        SocketTCPIPReceiveList->Add(EthernetBuffer);

        EthernetBuffer=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
        sTCPIPRecevieData=SocketTCPIPReceiveList->Strings[0];
        AnsiString Log;
        Log.sprintf("Recv, %s", EthernetBuffer);
        AddTCPIPCommunicationLog(Tag, Log);
    }
}
//------------------------------------------------------------------------------
//SocketTCPIPReceiveList有可能命令會分兩段進來, 需要進一步處理
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::TimerProcessTCPDataTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    AnsiString str="", asECHO="";
    AnsiString StrBarCodeTCPIP;
    bool bBarCodeError=false;
    int iLen=0, iBinCheck[32], iLen1;
    try
    {
        if(InitialOK==false ||
           bTimerRun==true ||
           bTCPError==true)                                                     //Steven 20231113 : TCP/IP Test 連線異常處置
        {
            return;
        }

        AnsiString sPrefix, sReply="", sPrefix2;
        AnsiString Str2, str3="";

        if(SocketTCPIPReceiveList->Count>0)
        {
            sTCPIPRecevieData=SocketTCPIPReceiveList->Strings[0];

            sReply="";
            Str2=StringReplace(sTCPIPRecevieData, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            TStringList *sList = new TStringList();
            TStringList *sBarCodeList = new TStringList();
            TStringList *sBinList = new TStringList();
            sList->SetText(Str2.c_str());
            sPrefix=sList->Strings[0];

            if(sPrefix=="WORKFILE_OK")
            {
            }
            else if(sPrefix=="WORKFILE_FAIL")
            {
                ShowMyMessage("There does not have setup at tester side. Please make a new one", "請在測試機端建立新的工作檔", fMain->cbSetupFileName->Text);
            }
            else if(sPrefix=="BARCODE?")
            {
                StrBarCodeTCPIP="";
                for(int i=0; i<32; i++)
                {
                    StrBarCodeTCPIP+=fMain->tBarCodeList->Strings[i];
                    asTCPIPBarCode[i]=fMain->tBarCodeList->Strings[i];
                    labOcr[i]->Caption=fMain->tBarCodeList->Strings[i];;
                    if(i==31)
                        StrBarCodeTCPIP+=";";
                    else
                        StrBarCodeTCPIP+=",";
                }
                sReply.sprintf("BARCODE:%s", StrBarCodeTCPIP);
            }
            else if(sPrefix.Pos("ECHOCODE:")!=0)
            {
                bBarCodeError=false;
                iLen=Str2.Pos(":");
                Str2=Str2.Delete(1, 9);
                iLen1=Str2.Pos(";");
                Str2=Str2.SubString(1, iLen1-1);
                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
                sBarCodeList->SetText(Str2.c_str());
                if(sBarCodeList->Count!=32)                                     //Jimmychiu 20231002 : check barcode list length is 32
                {
                    bBarCodeError=true;
                    AddTCPIPCommunicationLog(0, AnsiString().sprintf("barcode list count is not 32, real count is %d , command:%s", sBarCodeList->Count, Str2));
                }
                else
                {
                    for(int i=0; i<32; i++)
                    {
                        if(sBarCodeList->Strings[i]!=asTCPIPBarCode[i])
                        {
                            bBarCodeError=true;
                        }
                    }
                }

                if(bBarCodeError==true)
                {
                    sReply.sprintf("ECHOCODENG");
                }
                else
                {
                    sReply.sprintf("ECHOCODEOK");
                }
            }
            else if(sPrefix=="Test Arm?")
            {
                if(iWhichArmDown==1)
                {
                    sReply.sprintf("TestArm:1");
                }
                else if(iWhichArmDown==2)
                {
                    sReply.sprintf("TestArm:2");
                }
                else
                {
                    sReply.sprintf("TestArm:0");
                }
            }
            else if(sPrefix=="TempArm?")
            {
                asTCPIPTemperature="";
                fMain->WritePERSITETemperature();
                if(asTCPIPTemperature!="")
                {
                    asTCPIPTemperature=StringReplace(asTCPIPTemperature, "\r", "", TReplaceFlags()<<rfReplaceAll);
                    sReply.sprintf("TempArm:%s",asTCPIPTemperature.c_str());
                }
            }
            else if(sPrefix.Pos("BINON:")!=0)
            {
                ZeroMemory(iBinCheck, sizeof(iBinCheck));
                for(int i=0; i<32; i++)
                {
                    fMain->tTestResult->Strings[i]=AnsiString("-1");            //wei 20160302 0 ==> -1  因為不測試為-1
                    plSite[i]->Caption="--";
                }
                asECHO=sTCPIPRecevieData;
                iLen=Str2.Pos(":");
                Str2=Str2.SubString(iLen+1,Str2.Length());
                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
                Str2=StringReplace(Str2, ";", "\n", TReplaceFlags()<<rfReplaceAll);
                sBinList->SetText(Str2.c_str());
                for(int i=0; i<MAX_SOCKET_ROW; i++)
                {
                    str3=sBinList->Strings[3-i];
                    for(int j=0; j<MAX_SOCKET_COL; j++)
                    {
                        iBinCheck[i*8+j]=atoi(str3.SubString(MAX_SOCKET_COL-j, 1).c_str());
                    }
                }

                for(int i=0; i<MAX_SOCKET_ROW; i++)
                {
                    for(int j=0; j<MAX_SOCKET_COL; j++)
                    {
                        if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
                        {
                            iBin[i][j]=iBinCheck[TestIF.iSiteMap[i][j]-1];
                            fMain->tTestResult->Strings[TestIF.iSiteMap[i][j]-1]=iBin[i][j];  //Steven 20141230 : 修正SECS GEM參數
                            plSite[TestIF.iSiteMap[i][j]-1]->Caption=iBin[i][j];
                        }
                        else
                        {
                            iBin[i][j]=0;
                        }
                    }
                }

                sReply.sprintf("ECHO:%s", asECHO.c_str());
            }
            else if(sPrefix=="ECHOOK")
            {
                bTimeOutForNoFullSite=false;
                bEcho=true;
                TestIntervalsTime.LatchCycleTime(true);

                AddTCPIPCommunicationLog(0, " ");
            }
            else if(sPrefix=="GETOSSETUP")
            {
                sList->Delete(0);
                fMain->SVID1190_OSSetup=sList->CommaText;
            }
//            else if(sPrefix=="GetTPSVersionOK")
//            {
//                Str2=StringReplace(Str2, ",", "\n", TReplaceFlags()<<rfReplaceAll);
//                sList->SetText(Str2.c_str());
//                sPrefix2=sList->Strings[1];
//
//                edTPSVersion3->Text=sPrefix2;
//                asGetTPSVersion[2]=sPrefix2;
//                EventReport(SECS_EVENT.TesterTCPReady);
//            }

            if(sReply!="")
                SendTCPIPCommand(0, " ", sReply);
            sList->Clear();
            delete sList;
            SocketTCPIPReceiveList->Delete(0);
        }

        if(SocketTCPIPReceiveList->Count==0)
            SocketTCPIPReceiveList->Clear();

        if(LastSet.iTester==OFF_LINE && SimulateStart)
        {
            SimulateBin();
            bTimeOutForNoFullSite=false;
            bEcho=true;
            TestIntervalsTime.LatchCycleTime(true);
            SimulateStart=false;
        }
    }
    catch(...)
    {
        //發生error
        str.sprintf("bTimerRun=%d , %s", bTimerRun, sTCPIPRecevieData);
    }
    bTimerRun=false;
}
//------------------------------------------------------------------------------
void TfTesterTCP::SimulateBin()
{
    int ct=0, iSiteNo;
    AnsiString str="";
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
            {
                iSiteNo=TestIF.iSiteMap[i][j]-1;
                if(     cbSimulateBin[iSiteNo]->ItemIndex==0)                   //2012-10-11    Dell Fix
                    ct=1;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==1)
                    ct=2;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==2)
                    ct=3;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==3)
                    ct=4;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==4)
                    ct=5;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==5)
                    ct=6;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==6)
                    ct=random(5)+1;
                else if(cbSimulateBin[iSiteNo]->ItemIndex==7)
                    ct=random(10)+1;
                else
                    ct=random(15)+1;

                iBin[i][j]=ct;
                fMain->tTestResult->Strings[iSiteNo]=iBin[i][j];                //Steven 20141230 : 修正SECS GEM參數
                plSite[iSiteNo]->Caption=iBin[i][j];
            }
            else
            {
                iBin[i][j]=0;
            }
        }
    }

    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        if(fMain->tTestResult->Strings[31-i]!="-1")
        {
            str+=fMain->tTestResult->Strings[31-i];
        }
        else
        {
            str+="0";
        }

        if(i==31)
            str+=";";
        else
            str+=",";
    }

    AddTCPIPCommunicationLog(0, str);
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ClientSocket_TCPIPError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorMessage=GetErrorMsg(Sender, ErrorCode);                                //Steven 20231113 : TCP/IP Test 連線異常處置
    AddTCPIPCommunicationLog(0, AnsiString("Tester_TCPIP_Error : ")+ErrorMessage);
    bTCPError=true;
    fLotInfo->labTCPIPStatus->Caption="ERROR";
    fLotInfo->labTCPIPStatus->Color=clRed;

    try
    {
        ClientSocket_TCPIP->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "TCP/IP Tester Error");
    }
    ErrorCode=0;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::rgUnloaderClick(TObject *Sender)
{
    if(rgUnloader->ItemIndex==0)
    {
        for(int i=0; i<eTrayCount; i++)
        {
            GroupBox[i]->Visible=(Prod.iTrayType[i]!=tNotUse && bFlag[i]==true);//RogerYang 20260128 : 田揚志畫面修改需求
            GroupBox[i]->Height=100;
            GroupBox[i]->Top=i*100;
            GroupBox[i]->Align=alTop;
        }
    }
    else
    {
        for(int i=0; i<eTrayCount; i++)
        {
            GroupBox[i]->Visible=false;
            GroupBox[i]->Align=alNone;
        }

        GroupBox[rgUnloader->ItemIndex-1]->Visible=true;
        GroupBox[rgUnloader->ItemIndex-1]->Align=alClient;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::CopyOSTestResult(int iSuckRow, int iSuckCol)
{
    if(TestIF_File.iTestType!=TCP_IP_MODE)                                      //Steven 20230116 : OS測試機傳送工作檔名
        return;

    #ifndef SOFT_SIMULTE
    if(LastSet.iTester!=ON_LINE)                                                //Steven 20231113 : 修正TCP/IP Test跑Off Line
        return;
    #endif

    AnsiString sSourceFileName, sTargetFileName, cString;
    int iTesterCh       =TestSocket.PordRec[iSuckRow][iSuckCol].GetSiteNo()-1;
    int iContactIndex   =TestSocket.PordRec[iSuckRow][iSuckCol].GetOrderOfContact();

    MyForceDirectories("D:\\HT9045_Log\\OSTestResult", "CopyOSTestResult");

    #ifdef SOFT_SIMULTE
        sSourceFileName.sprintf("D:\\RMS\\Device%d.TXT", iTesterCh);
    #else
        sSourceFileName.sprintf("Z:\\Device%d.TXT", iTesterCh);
    #endif
    if(CheckFileExist(sSourceFileName)==true)
    {
        sTargetFileName.sprintf("D:\\HT9045_Log\\OSTestResult\\Device%06d_%02d.TXT", iContactIndex, iTesterCh);
        CopyFile(sSourceFileName.c_str(), sTargetFileName.c_str(), false);
    }

    #ifdef SOFT_SIMULTE
        sSourceFileName.sprintf("D:\\RMS\\Device%d.TXT", iTesterCh);
    #else
        sSourceFileName.sprintf("Z:\\FailPin_Device%d.TXT", iTesterCh);
    #endif
    if(CheckFileExist(sSourceFileName)==true)
    {
        sTargetFileName.sprintf("D:\\HT9045_Log\\OSTestResult\\Device%06d_%02d.TXT", iContactIndex, iTesterCh);
        CopyFile(sSourceFileName.c_str(), sTargetFileName.c_str(), false);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::PlaceOSTestResultToTray(int iSuckRow, int iSuckCol, int iTrayRow, int iTrayCol, int iAuto)
{
    if(TestIF_File.iTestType!=TCP_IP_MODE)
        return;

    AnsiString sSourceFileName, sTargetFileName, Str1, Str2;
    int iTotalCh        =TestSocket.iShtRow*TestSocket.iShtCol;
    int iTesterCh       =OutArmSuck.PordRec[iSuckRow][iSuckCol].GetSiteNo();
    int iContactIndex   =OutArmSuck.PordRec[iSuckRow][iSuckCol].GetOrderOfContact();

    SourceMemo->Lines->Clear();
    RichEdit[iAuto]->Lines->Clear();
    sTargetFileName.sprintf("D:\\HT9045_Log\\OS_TestReport\\%s_TestReport.TXT", s6TrayName[iAuto]);
    sSourceFileName.sprintf("D:\\HT9045_Log\\OSTestResult\\Device%06d_%02d.TXT", iContactIndex, iTesterCh-1);
    MyForceDirectories("D:\\HT9045_Log\\OS_TestReport", "ProcessOSPrint");

    if(CheckFileExist(sSourceFileName)==true)
    {
        SourceMemo->Lines->LoadFromFile(sSourceFileName);
        DeleteFile(sSourceFileName);
    }
    else
    {
        Str1.sprintf("can not find file %s", sSourceFileName);
        SourceMemo->Lines->Add(Str1);
    }

    if(CheckFileExist(sTargetFileName)==true)
        RichEdit[iAuto]->Lines->LoadFromFile(sTargetFileName);

    //RogerYang 20260210 : change the test seq value to Tested from Device, and the Device shown the serier number on a tray.
    Str1.sprintf("===========================    Device:%d  X:%d  Y:%d  Tested:%d    ====================================",
        iOneTrayPickCount[1+iAuto], iTrayRow+1, iTrayCol+1, (iTotalCh*iContactIndex+iTesterCh));

    RichEdit[iAuto]->Lines->Add(Str1);
    for(int i=0; i<SourceMemo->Lines->Count; i++)
    {
        RichEdit[iAuto]->Lines->Add(SourceMemo->Lines->Strings[i]);
    }
    RichEdit[iAuto]->Lines->SaveToFile(sTargetFileName);
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ProcessOSPrint(bool bViewOnly)                     //Steven 20250515 : 整合Open Short測試報表
{
    int iSiteCh=0;
    int iTotalCh=TestSocket.iShtRow*TestSocket.iShtCol;
    int iSiteTotalCt=0;
    int iSitePassCt=0;
    int iSiteFailCt=0;
    AnsiString sPassYield;
    AnsiString sFailYield;
    AnsiString asFileName, asFolderName, TargetFile, SourceFile, ServerFile;
    AnsiString sTemp, str, s, sTotal, sTempT, sTempP, sTempF, sP, sF;
    redtSummary->Lines->Clear();
    SummaryHead->Clear();
    TastCategory.UpdataCount(false);                                            //Steven 20250514 : 統一計算數量

    asFolderName.sprintf("D:\\HT9045_Log\\OS_Summary\\%04d%02d", SystemYear, SystemMonth);
    MyForceDirectories(asFolderName, "ProcessOSPrint");

    ServerFile.sprintf("%s\\OS_Summary_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                    IniConfig.sB05_OSReportPath,
                                                    RunInfo.LotNo,
                                                    SystemYear, SystemMonth, SystemDate,
                                                    SystemHour, SystemMin, SystemSec);

    asFileName.sprintf("%s\\OS_Summary_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                    asFolderName,
                                                    RunInfo.LotNo,
                                                    SystemYear, SystemMonth, SystemDate,
                                                    SystemHour, SystemMin, SystemSec);

    if(RunInfo.LotStartTime=="2020-01-01 00:00:00")
        sPassYield.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        sPassYield=RunInfo.LotStartTime;

    if(RunInfo.LotEndTime=="")
        sFailYield.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        sFailYield=RunInfo.LotEndTime;

    if(fLotInfo->lbledtCustomer->Text=="")
        sTemp=" ";
    else
        sTemp=fLotInfo->lbledtCustomer->Text;

    if(fLotInfo->edtSysOperatorID->Text=="")
        sTotal=" ";
    else
        sTotal=fLotInfo->edtSysOperatorID->Text;

    SummaryHead->Add(str.sprintf("============================ SUMMARY REPORT ============================"));
//    SummaryHead->Add(str.sprintf(" "                                     ));                              //RogerYang 20260530 : 田揚志要求格式修改
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Lot#:"),       RunInfo.LotNo));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Start:"),      sPassYield));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("End:"),        sFailYield));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Customer:"),   sTemp));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Program:"),    fMain->cbSetupFileName->Text));
//    SummaryHead->Add(str.sprintf("%-34s ",    AnsiString("LoadBoard ID:")));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Operator ID:"),sTotal));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Machine ID:"), IniConfig.SocketHandlerID));
    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Input:"),      TastCategory.iTotalSocket));
    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Pass:"),       TastCategory.iPassSocket));
    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Fail:"),       TastCategory.iFailSocket));
//    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Reject:"),     TastCategory.iRejectCount));     //RogerYang 20260530 : 田揚志要求格式修改
    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Open:"),       TastCategory.iUnloadCnt[1]));
    SummaryHead->Add(str.sprintf("%-34s %d",  AnsiString("Short:"),      TastCategory.iUnloadCnt[2]));
    SummaryHead->Add(str.sprintf("%-34s %s",  AnsiString("Yield:"),      ChangeToPercentage(TastCategory.iPassSocket, TastCategory.iTotalSocket)));
//    SummaryHead->Add(str.sprintf(" "                                     ));                              //RogerYang 20260530 : 田揚志要求格式修改

    for(int i=0; i<SummaryHead->Count; i++)
        redtSummary->Lines->Add(SummaryHead->Strings[i]);

    redtSummary->Lines->Add(str.sprintf("============================= BY TRAY COUNT ============================"));
    redtSummary->Lines->Add(str.sprintf(" "                                     ));

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            sTemp.sprintf("%-34s %d", s6TrayName[i]+AnsiString(":"), TastCategory.iUnloadCnt[i]);
            redtSummary->Lines->Add(sTemp);
        }
    }

    redtSummary->Lines->Add(str.sprintf(" "                                     ));
    redtSummary->Lines->Add(str.sprintf("============================= BY SITE COUNT ============================"));
    redtSummary->Lines->Add(str.sprintf(" "                                     ));

    str.sprintf("%-34s", "Total Tested DUT Count:");                            //Total Tested DUT Count: DUT1 DUT2 DUT3 DUT4 SUM
    for(int i=0; i<iTotalCh; i++)
    {
        s.sprintf("DUT%d", i+1);
        sTemp.sprintf(" %-20s", s);
        str+=sTemp;
    }
    sTemp.sprintf(" %-20s", "SUM");
    str+=sTemp;
    redtSummary->Lines->Add(str);

    TStringList *sListTotal=new TStringList();
    TStringList *sListPass =new TStringList();
    TStringList *sListFail =new TStringList();
    sListTotal->Clear();
    sListPass->Clear();
    sListFail->Clear();
    for(int i=0; i<TestSocket.iMaxRow; i++)
    {
        for(int j=0; j<TestSocket.iMaxCol; j++)
        {
            sListTotal->Add("0");
            sListPass->Add("0(0.00\%)");
            sListFail->Add("0(0.00\%)");
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF.iSiteMap[i][j]>0)
            {
                iSiteCh=TestIF.iSiteMap[i][j]-1;
                iSiteTotalCt=TastCategory.iBySiteTotal[iSiteCh];
                iSitePassCt =TastCategory.iBySitePass[iSiteCh];
                iSiteFailCt =TastCategory.iBySiteFail[iSiteCh];

                if(iSiteTotalCt>0)
                {
                    sPassYield.sprintf("%d(%s)", iSitePassCt, ChangeToPercentage(iSitePassCt, iSiteTotalCt));
                    sFailYield.sprintf("%d(%s)", iSiteFailCt, ChangeToPercentage(iSiteFailCt, iSiteTotalCt));
                    sListTotal->Strings[iSiteCh]=AnsiString().sprintf("%d", iSiteTotalCt);
                    sListPass->Strings[iSiteCh] =AnsiString().sprintf("%s", sPassYield);
                    sListFail->Strings[iSiteCh] =AnsiString().sprintf("%s", sFailYield);
                }
            }
        }
    }

    sTotal.sprintf("%-34s", "Total");                                           //Total DUT1 DUT2 DUT3 DUT4 SUM
    sPassYield.sprintf("%-34s", "PASS");                                        //PASS DUT1 DUT2 DUT3 DUT4 SUM
    sFailYield.sprintf("%-34s", "FAIL");                                        //FAIL DUT1 DUT2 DUT3 DUT4 SUM

    for(int i=0; i<iTotalCh; i++)
    {
        sTempT.sprintf(" %-20s", sListTotal->Strings[i]);
        sTempP.sprintf(" %-20s", sListPass->Strings[i]);
        sTempF.sprintf(" %-20s", sListFail->Strings[i]);

        sTotal+=sTempT;
        sPassYield+=sTempP;
        sFailYield+=sTempF;
    }

    sP.sprintf("%d(%s)", TastCategory.iPassSocket, ChangeToPercentage(TastCategory.iPassSocket, TastCategory.iTotalSocket));
    sF.sprintf("%d(%s)", TastCategory.iFailSocket, ChangeToPercentage(TastCategory.iFailSocket, TastCategory.iTotalSocket));
    sTempT.sprintf(" %-20d", TastCategory.iTotalSocket);
    sTempP.sprintf(" %-20s", sP);
    sTempF.sprintf(" %-20s", sF);

    sTotal+=sTempT;
    sPassYield+=sTempP;
    sFailYield+=sTempF;

    redtSummary->Lines->Add(sTotal);
    redtSummary->Lines->Add(sPassYield);
    redtSummary->Lines->Add(sFailYield);
    redtSummary->Lines->Add(str.sprintf(" "                                     ));
    redtSummary->Lines->Add(str.sprintf("============================= BY BIN COUNT ============================="));
    redtSummary->Lines->Add(str.sprintf(" "                                     ));

    str.sprintf("%-34s", "HW BIN Count:");                                      //HW BIN Count: DUT1 DUT2 DUT3 DUT4 SUM
    for(int i=0; i<iTotalCh; i++)
    {
        s.sprintf("DUT%d", i+1);
        sTemp.sprintf(" %-20s", s);
        str+=sTemp;
    }
    sTemp.sprintf(" %-20s", "SUM");
    str+=sTemp;
    redtSummary->Lines->Add(str);

    for(int iCat=0; iCat<iTestBinCount; iCat++)
    {
        s.sprintf("BIN %d", iCat);
        sTotal.sprintf("%-34s", s);
        for(int iDut=0; iDut<iTotalCh; iDut++)
        {
            sPassYield.sprintf("%d(%s)", TastCategory.iBySiteCate[iDut][iCat], ChangeToPercentage(TastCategory.iBySiteCate[iDut][iCat], TastCategory.iTotalCategory[iCat]));
            sTempT.sprintf(" %-20s", sPassYield);
            sTotal+=sTempT;
        }

        sPassYield.sprintf("%d(%s)", TastCategory.iTotalCategory[iCat], ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket));
        sTempT.sprintf(" %-20s", sPassYield);
        sTotal+=sTempT;
        redtSummary->Lines->Add(sTotal);
    }

    s.sprintf("REJECT");
    sTotal.sprintf("%-34s", s);
    for(int iDut=0; iDut<iTotalCh; iDut++)
    {
        sPassYield.sprintf("%d(%s)", TastCategory.iBySiteCate[iDut][iTestBinCount], ChangeToPercentage(TastCategory.iBySiteCate[iDut][iTestBinCount], TastCategory.iTotalCategory[iTestBinCount]));
        sTempT.sprintf(" %-20s", sPassYield);
        sTotal+=sTempT;
    }

    sPassYield.sprintf("%d(%s)", TastCategory.iTotalCategory[iTestBinCount], ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount], TastCategory.iTotalSocket));
    sTempT.sprintf(" %-20s", sPassYield);
    sTotal+=sTempT;
    redtSummary->Lines->Add(sTotal);

    if(bViewOnly==false)
    {
//        if(IniConfig.bB05_OSReport)                                             //AI(ht9045-config) 20260520 (RogerYang) : 恢復Summary上傳網盤
//            redtSummary->Lines->SaveToFile(ServerFile);
        redtSummary->Lines->SaveToFile(asFileName);
    }

    ProcessOSTrayData(bViewOnly);
    RunInfo.SaveJamRateByLot();                                                 //Steven 20200415 : SCC要By Lot Jam Rate
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::ProcessOSTrayData(bool bViewOnly)
{
    AnsiString asFolderName, ServerFile, asFileName, SourceFile, str;
    AnsiString asAutoName[eTrayCount], asAutoServer[eTrayCount];
    asFolderName.sprintf("D:\\HT9045_Log\\OS_Summary\\%04d%02d", SystemYear, SystemMonth);
    MyForceDirectories(asFolderName, "ProcessOSPrint");

    ServerFile.sprintf("%s\\OS_Pin_All_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                    IniConfig.sB05_OSReportPath,
                                                    RunInfo.LotNo,
                                                    SystemYear, SystemMonth, SystemDate,
                                                    SystemHour, SystemMin, SystemSec);

    asFileName.sprintf("%s\\OS_Pin_All_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                    asFolderName,
                                                    RunInfo.LotNo,
                                                    SystemYear, SystemMonth, SystemDate,
                                                    SystemHour, SystemMin, SystemSec);

    for(int iAuto=0; iAuto<eTrayCount; iAuto++)
    {
        asAutoName[iAuto].sprintf("%s\\OS_Pin_%s_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                        asFolderName,
                                                        s6TrayName[iAuto],
                                                        RunInfo.LotNo,
                                                        SystemYear, SystemMonth, SystemDate,
                                                        SystemHour, SystemMin, SystemSec);

        asAutoServer[iAuto].sprintf("%s\\OS_Pin_%s_%s_%04d_%02d_%02d_%02d_%02d_%02d.TXT",
                                                        IniConfig.sB05_OSReportPath,
                                                        s6TrayName[iAuto],
                                                        RunInfo.LotNo,
                                                        SystemYear, SystemMonth, SystemDate,
                                                        SystemHour, SystemMin, SystemSec);
    }

    SourceMemo->Clear();
    for(int i=0; i<SummaryHead->Count; i++)                                     //加入summary的標頭
        SourceMemo->Lines->Add(SummaryHead->Strings[i]);

    for(int iAuto=0; iAuto<eTrayCount; iAuto++)
    {
        RichEdit[iAuto]->Clear();

        if(Prod.iTrayType[iAuto]!=tNotUse)
        {
            SourceFile.sprintf("D:\\HT9045_Log\\OS_TestReport\\%s_TestReport.TXT", s6TrayName[iAuto]);
            if(CheckFileExist(SourceFile))
            {
                RichEdit[iAuto]->Lines->LoadFromFile(SourceFile);
                SourceMemo->Lines->Add(str.sprintf("===================================    %s    ==================================================", s6TrayName[iAuto]));
                SourceMemo->Lines->Add(str.sprintf(" "                          ));

                for(int i=0; i<RichEdit[iAuto]->Lines->Count; i++)
                {
                    SourceMemo->Lines->Add(RichEdit[iAuto]->Lines->Strings[i]);
                }
                SourceMemo->Lines->Add(str.sprintf(" "                          ));
            }

            for(int i=0; i<SummaryHead->Count; i++)                             //AI(ht9045-config) 20260520 (RogerYang) : 無料Auto仍產生報表
                RichEdit[iAuto]->Lines->Insert(i, SummaryHead->Strings[i]);
            RichEdit[iAuto]->Lines->Insert(SummaryHead->Count, " ");
            RichEdit[iAuto]->Lines->Insert(SummaryHead->Count+1, "Fail Pin,,,,===============下限,         上限        ,量測值========");

            if(bViewOnly==false)
            {
                if(IniConfig.bB05_OSReport &&                                   //AI(ht9045-config) 20260530 (RogerYang) : 只上傳Auto2+Auto3
                    (s6TrayName[iAuto]=="Auto2" ||
                        s6TrayName[iAuto]=="Auto3"))
                {
                    RichEdit[iAuto]->Lines->SaveToFile(asAutoServer[iAuto]);
                }

                if(s6TrayName[iAuto].Pos("Fix")==0)                             //AI(ht9045-config) 20260530 (RogerYang) : Fix不存本地
                    RichEdit[iAuto]->Lines->SaveToFile(asAutoName[iAuto]);
            }

            if(CheckFileExist(SourceFile))                                      //AI(ht9045-config) 20260520 (RogerYang) : 清除暫存
            {
                RichEdit[iAuto]->Clear();
                RichEdit[iAuto]->Lines->SaveToFile(SourceFile);
            }
        }
    }

    if(bViewOnly==false)
    {
//        if(IniConfig.bB05_OSReport)                                             //AI(ht9045-config) 20260520 (RogerYang) : 恢復All上傳網盤
//            SourceMemo->Lines->SaveToFile(ServerFile);
        SourceMemo->Lines->SaveToFile(asFileName);
    }
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260625: 新增去重式記錄器，N06 recipe 同步成敗寫入既有 EventLog，避免靜默失敗且不膨脹日誌
static void LogN06Result(AnsiString FileName, bool bOk, AnsiString sReason)
{
    static AnsiString sLastKey="";
    AnsiString sKey = FileName + "|" + (bOk ? AnsiString("OK") : sReason);
    if(sKey==sLastKey) return;                                                  // 連續相同結果只記第一次
    sLastKey=sKey;
    if(bOk)
        SaveEventLogInfo("N06000001", AnsiString("Recipe synced to OS Tester: ")+FileName, 20, "OK");
    else
        SaveEventLogInfo("N06000002", AnsiString("Recipe sync to OS Tester FAIL [")+sReason+"]: "+FileName, 0, "NG");
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260625: 取 7z 完整路徑(不硬編碼)，FileExists 為 false 才 fallback
static AnsiString Get7zPath()
{
    AnsiString s7z = ExtractFilePath(Application->ExeName)+"7z.exe";
    if(FileExists(s7z)==false)
        s7z="D:\\HT9045\\7z.exe";
    return s7z;
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260625: 同步執行 7z 並取得 exit code(沿用 main.cpp ShellExecuteEx+WaitForSingleObject 模式)；回傳 false 代表失敗，pdwExit 帶回程序結束碼
static bool RunSevenZipSync(AnsiString s7z, AnsiString sParam, unsigned long *pdwExit)
{
    SHELLEXECUTEINFO execinfo;
    memset(&execinfo, 0, sizeof(execinfo));
    execinfo.cbSize=sizeof(execinfo);
    execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
    execinfo.lpVerb="open";
    execinfo.lpFile=s7z.c_str();
    execinfo.lpParameters=sParam.c_str();
    execinfo.nShow=SW_HIDE;

    if(ShellExecuteEx(&execinfo)==FALSE || execinfo.hProcess==NULL)
        return false;

    unsigned long dwWait = WaitForSingleObject(execinfo.hProcess, 10000);       // timeout 10s
    bool bOk=false;
    unsigned long dwExit=0xFFFFFFFF;
    if(dwWait==WAIT_OBJECT_0)
    {
        if(GetExitCodeProcess(execinfo.hProcess, &dwExit))
            bOk=true;                                                           // 順利取得 exit code，由呼叫端判定 0=OK
    }
    if(pdwExit) *pdwExit=dwExit;
    CloseHandle(execinfo.hProcess);
    return bOk;
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260625: 改回傳 bool + 同步等待 + 取 exit code + 存在性檢查，避免 N06 上傳靜默失敗
bool __fastcall TfTesterTCP::CopyRecipeToTester(AnsiString FileName)            //Steven 20250612 : for OS Tester.
{
    AnsiString str1, str2, str3;
    if(IniConfig.bN06_CopyTesterFile==false)                                    //Steven 20250327 : OS測試機的工作檔也要上傳
        return false;

    str1.sprintf("%s%s\\OS_Setting.zip", DataPath, FileName);                   //Steven 20230710 : OS測試機的工作檔也要上傳
    str2.sprintf("%s", IncludeTrailingPathDelimiter(IniConfig.asN06_TesterPath));

    if(FileExists(str1)==false)                                                 // 來源 zip 不存在 -> 記錄後返回，不可靜默
    {
        LogN06Result(FileName, false, "SRC_ZIP_MISSING");
        return false;
    }
    if(DirectoryExists(IniConfig.asN06_TesterPath)==false)                      // 目的網路磁碟不存在 -> 記錄後返回
    {
        LogN06Result(FileName, false, "DEST_PATH_MISSING");
        return false;
    }

    AnsiString s7z = Get7zPath();
    if(FileExists(s7z)==false)
    {
        LogN06Result(FileName, false, "7Z_MISSING");
        return false;
    }

    str3.sprintf("e \"%s\" -o\"%s\" -y", str1, str2);
    unsigned long dwExit=0xFFFFFFFF;
    if(RunSevenZipSync(s7z, str3, &dwExit)==false)                              // 啟動或等待逾時失敗
    {
        LogN06Result(FileName, false, "EXEC_FAIL_OR_TIMEOUT");
        return false;
    }
    if(dwExit!=0)                                                               // 7z: 0=OK,1=warn,>=2=error
    {
        AnsiString sR; sR.sprintf("7Z_EXIT_%u", dwExit);
        LogN06Result(FileName, false, sR);
        return false;
    }
    LogN06Result(FileName, true, "");
    return true;
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260625: 同 CopyRecipeToTester，改 bool 回傳 + 同步取 exit code + 存在性檢查 + 去重記錄
bool __fastcall TfTesterTCP::CopyRecipeFromTester(AnsiString FileName)          //Steven 20250612 : for OS Tester.
{
    AnsiString str1, str2, str3;
    if(IniConfig.bN06_CopyTesterFile==false)                                    //Steven 20250327 : OS測試機的工作檔也要上傳
        return false;

    str1.sprintf("%s%s\\OS_Setting.zip", DataPath, FileName);                   //Steven 20230710 : OS測試機的工作檔也要上傳
    str2.sprintf("%s%s.ini", IncludeTrailingPathDelimiter(IniConfig.asN06_TesterPath), FileName);

    if(FileExists(str2)==false)                                                 //AI(ht9045-v899) 20260625: 原註解掉的 log 改為呼叫去重記錄器
    {
        LogN06Result(FileName, false, "SRC_INI_MISSING");
        return false;
    }

    AnsiString s7z = Get7zPath();
    if(FileExists(s7z)==false)
    {
        LogN06Result(FileName, false, "7Z_MISSING");
        return false;
    }

    str3.sprintf("a -tzip \"%s\" \"%s\"", str1, str2);
    unsigned long dwExit=0xFFFFFFFF;
    if(RunSevenZipSync(s7z, str3, &dwExit)==false)                              // 啟動或等待逾時失敗
    {
        LogN06Result(FileName, false, "EXEC_FAIL_OR_TIMEOUT");
        return false;
    }
    if(dwExit!=0)                                                               // 7z: 0=OK,1=warn,>=2=error
    {
        AnsiString sR; sR.sprintf("7Z_EXIT_%u", dwExit);
        LogN06Result(FileName, false, sR);
        return false;
    }
    LogN06Result(FileName, true, "");
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TfTesterTCP::btnSaveClick(TObject *Sender)                      //RogerYang 20260210 : 田揚志需求，有設定的才可以一次放全部，OS報表要照Tray盤放料順序顯示
{
    AnsiString szDir="";
    if(CosFunction.bSaveArmSpeedByMachine && IniConfig.bA57_1SaveArmSpeedByMachine)
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir.sprintf("%s%s", DataPath, LastFileName);
    }
    MyForceDirectories(szDir);
    szDir+="\\ArmCondition.Data";
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto1",    (chkAuto1_OS->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto2",    (chkAuto2_OS->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "JSCC_OS_Use Auto3",    (chkAuto3_OS->Checked)?1:0);
}
//---------------------------------------------------------------------------

