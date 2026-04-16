//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MonitorInterface.h"

//#include "IncludeAllHeader.H"
#include "MonitorTCPIP.h"
#include "cprod.h"
#include "cmydef.h"
#include "main.h"
#include "mymessbox.h"
#include "tools.h"
#include "myQwertyKeyBoard.h"
#include "LastSet.h"            //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "MyLed"
#pragma resource "*.dfm"
TfMonitor *fMonitor;
//---------------------------------------------------------------------------
__fastcall TfMonitor::TfMonitor(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    bCommandReady=false;

    MVPageControl->ActivePageIndex=0;
    MVCtrl=new MonitorTCPIP(new TCPClient(MonitorClientSocket));
    MVCtrl->InitialMemo(MonitorMemo);
    LoadTCPIPParament();
    MVCtrl->InitialSocket(sADDRESS, iPORT);

    sComData=new TStringList;                                                   //pig 2015.11.30 ModifyMonitorIssue
    iHDSpace=0;                                                                 //pig 2015.11.30 ModifyMonitorIssue
    sComData->Clear();

    for(int i=0; i<3; i++)
    {
        iStatus[i]=0;
    }
    iDisconnectCount=0; //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
    MonitorTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::FormShow(TObject *Sender)
{
    Width   =610;
    Height  =515;
    Left    =(1024-Width)/2;
    Top     =(768-Height)/2;
    bShow   =true;
    LoadTCPIPParament();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::FormDestroy(TObject *Sender)
{
    try
    {
        MonitorTimer->Enabled=false;
        MVCtrl->Disconnect();
        sComData->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete MVCtrl;
        delete sComData;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfMonitor::FormDestroy");
    }
    LogSoftwareOffTime("TfMonitor, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void TfMonitor::LoadTCPIPParament()
{
    TIniFile *pFile = new TIniFile("D:\\HT9045\\system\\MVData.ini");

    sADDRESS=pFile->ReadString("Setup", "IP", "127.0.0.1");
    iPORT   =pFile->ReadInteger("Setup", "Port", 7000);

    edMVAddress->Text           =sADDRESS;
    edMVPort->Text              =AnsiString(iPORT);
    cbWhenHDFullAlarm->Checked  =pFile->ReadBool( "Specific", "HD_Space", false);
    edLowHDSpace->Text          =AnsiString(pFile->ReadInteger("Specific" , "HD_Space_Low" , 0 ) );
    edWhenHDFullPrompt->Text    =AnsiString(pFile->ReadInteger("Specific" , "HD_Space_Low_Prompt" , 0 ) );
    cbAfterHandlerTrayFeedMonitor1ClosedVideo->Checked=pFile->ReadBool("Specific", "HD_Closed_Wait", false);
    edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTime->Text=AnsiString(pFile->ReadInteger("Specific", "HD_Closed_Wait_Time", 0));

    MVCtrl->InitialSocket(sADDRESS, iPORT);
    delete pFile;
}
//---------------------------------------------------------------------------
void TfMonitor::SaveTCPIPParament()
{
    TIniFile *pFile = new TIniFile("D:\\HT9045\\system\\MVData.ini");
    bool bHDCheck=false;
    int iHDCheckLow=0, iHDCheckLowPrompt=0, iHDClosedWaitTime=0;

    sADDRESS    =edMVAddress->Text;
    iPORT       =atoi(edMVPort->Text.c_str());
    bHDCheck    =cbWhenHDFullAlarm->Checked;
    iHDCheckLow =atoi(edLowHDSpace->Text.c_str());
    iHDCheckLowPrompt=atoi(edWhenHDFullPrompt->Text.c_str());
    iHDClosedWaitTime=atoi(edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTime->Text.c_str());

    pFile->WriteString("Setup", "IP", sADDRESS);
    pFile->WriteInteger("Setup", "Port", iPORT);
    pFile->WriteBool("Specific", "HD_Space", bHDCheck);
    pFile->WriteInteger("Specific", "HD_Space_Low", iHDCheckLow);
    pFile->WriteInteger("Specific", "HD_Space_Low_Prompt", iHDCheckLowPrompt);
    pFile->WriteBool("Specific", "HD_Closed_Wait", cbAfterHandlerTrayFeedMonitor1ClosedVideo->Checked);
    pFile->WriteInteger("Specific", "HD_Closed_Wait_Time", iHDClosedWaitTime);

    MVCtrl->InitialSocket(sADDRESS, iPORT);
    delete pFile;
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMVUpdateClick(TObject *Sender)
{
    SaveTCPIPParament();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMVConnectClick(TObject *Sender)
{
    MVCtrl->Connect();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMVDisconnectClick(TObject *Sender)
{
    MVCtrl->Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMVExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::MonitorTimerTimer(TObject *Sender)   //0.1
{
    int iType,iWaitCT;
    DATA vData;
    static int iConnectCount=0, iCleanOutCT=0;
    AnsiString aData="", str, aTitle, S, aNum;
    TStringList *sNote;
    int iNum;

    if(InitialOK==false || IniConfig.bC11UseMonitorView==false)
        return;

    if(MVCtrl->IsConnect())
    {
        if(MVCtrl->AdapterData(iType, vData)!=0)
        {
            if(iType==TD_READ)
            {
                if(sComData->Count!=0)
                {
                    S=sComData->Strings[0];
                    aNum=S.SubString(4,1);
                    iNum=atoi(aNum.c_str());
                    if(S.SubString(1, 2)=="SR")  //pig 2015.11.30 ModifyMonitorIssue
                    {
                        if(char(vData[0])=='1')
                        {
                            MyLed1->Value=true;
                            iStatus[iNum]=1;
                        }
                        else if(char(vData[0])=='0')
                        {
                            MyLed1->Value=false;
                            iStatus[iNum]=0;
                        }
                        sComData->Delete(0);
                    }
                    else if(S.SubString(1,2)=="SS")
                    {
                        if(char(vData[0])=='1')
                        {
                            MyLed1->Value=true;
                            iStatus[iNum]=0;
                        }
                        else if(char(vData[0])=='0')
                        {
                            MyLed1->Value=false;
                            iStatus[iNum]=1;
                        }
                        sComData->Delete(0);
                    }
                    else if(S.SubString(1, 2)=="GP")                            //pig 2015.11.30 ModifyMonitorIssue
                    {
                        for(unsigned int i=0; i<vData.size()-2; i++)
                        {
                            aData+=char(vData[i]);
                        }
                        sComData->Delete(0);
                        iHDSpace=atoi(aData.c_str());                           //pig 2015.11.30 ModifyMonitorIssue
                        iDisconnectCount=0; //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
                    }
                }
            }
        }
    }
    // Show State
    if(MVCtrl->IsConnect()==true)
    {
        lbMVConnectState->Caption="ON-Line";
        pnMVState->Color=clGreen;
        str.sprintf("Monitor ON-Line   %d %%", iHDSpace);
        fMain->lbMonitorConnectState->Caption=str;                              //pig 2015.11.21 ModifyMonitorFunction
        fMain->lbMonitorConnectState->Font->Color=clGreen;
        //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
        //==>
        if(bCommandReady==false)
        {
            for(int iNum=0;iNum<3;iNum++)
            {
                if(iStatus[iNum]==1)
                {
                    OpenMonitorVedio(iNum);
                }
            }
        }
        //<==
        //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
        bCommandReady=true;                                                     //pig 2015.11.21 ModifyMonitorFunction
    }
    else
    {
        lbMVConnectState->Caption="OFF-Line";
        pnMVState->Color=clRed;
        fMain->lbMonitorConnectState->Caption="Monitor Off-Line";               //pig 2015.11.21 ModifyMonitorFunction
        fMain->lbMonitorConnectState->Font->Color=clRed;
        bCommandReady=false;                                                    //pig 2015.11.21 ModifyMonitorFunction
    }

    if(IniConfig.bC11UseMonitorView==false)
    {
        return;
    }
    else
    {
        sNote = new TStringList;
        iConnectCount++;

        if(iConnectCount>20)//2 S   //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
        {
            iConnectCount=0;
            iDisconnectCount++; //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
            GetMonitorHDSpec();

            if(bShow==false && MVCtrl->IsConnect()==false)
            {
                MVCtrl->Connect();

                //Nickliu 20230314 add connect alway retry connect, over socker error.
//                if(MVCtrl->IsConnect()==false)
//                {
//                    ShowMyMessage("Monitor Hard disk capacity has been below the set value");
//                }
                //Nickliu 20230314 add connect alway retry connect, over socker error.
            }
            else                                                                //pig 2015.11.30 ModifyMonitorIssue
            {
                if(cbWhenHDFullAlarm->Checked && atoi(edLowHDSpace->Text.c_str())>0 && iHDSpace!=0)
                {
                    if(iHDSpace>=atoi(edLowHDSpace->Text.c_str()))
                    {
                        if(SystemStart)
                            ShowMyMessage("Monitor Hard disk capacity has been below the set value");
                    }
                }
                if(cbWhenHDFullAlarm->Checked && atoi(edWhenHDFullPrompt->Text.c_str())>0 && iHDSpace!=0)
                {
                    if(iHDSpace>=atoi(edWhenHDFullPrompt->Text.c_str()))
                    {
                        aTitle.sprintf("Monitor HD capacity");
                        str.sprintf("Monitor Hard disk capacity has been below %s", edWhenHDFullPrompt->Text);
                        sNote->Add(str);
                        ShowMyMessage(aTitle, str);
                    }
                }
            }
            //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
            //==>
            if(LastSet.iRealDummy==REALLY &&
               LastSet.iTester==ON_LINE &&
               MyMessageBox->fShow==false)
            {
                if(iDisconnectCount==5)
                {
                    MVCtrl->Connect();
                    sComData->Clear();
                }
                if(iDisconnectCount>10)
                {
                    iDisconnectCount=0;
                    ShowMyMessage("Monitor Video Disconnect#Please Check CCD IPC");
                }
            }
            //<==
            //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
        }

        if(bCleanOutClosedMonitorVideo)
        {
            iCleanOutCT++;
            iWaitCT=atoi(edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTime->Text.c_str());
            if(iCleanOutCT>=iWaitCT*10)
            {
                iCleanOutCT=0;
                bCleanOutClosedMonitorVideo=false;
                StopMonitorVedio(0);
            }
        }
        sNote->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete sNote;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbSendCommandClick(TObject *Sender)
{
    MVCtrl->SendCommand(edCommandData->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMVLogClearClick(TObject *Sender)
{
    MonitorMemo->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMonitor1OpenClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(Ptr->Tag<=2)
        OpenMonitorVedio(Ptr->Tag);
    else
        StopMonitorVedio(Ptr->Tag-3);
}
//---------------------------------------------------------------------------
void TfMonitor::OpenMonitorVedio(int index)
{
    AnsiString aSendData;

    if(iStatus[index]==1 || bCommandReady==false)
        return;

    if(sComData->Count==0)
    {
        aSendData.sprintf("SR0%d\r\n", index);
        MVCtrl->SendCommand(aSendData);
        sComData->Add(aSendData);
    }                                                       //pig 2015.11.30 ModifyMonitorIssue
}
//---------------------------------------------------------------------------
void TfMonitor::StopMonitorVedio(int index)
{
    AnsiString aSendData;

    if(iStatus[index]==0 || bCommandReady==false)
        return;

    if(sComData->Count==0)
    {
        aSendData.sprintf("SS0%d\r\n", index);
        MVCtrl->SendCommand(aSendData);
        sComData->Add(aSendData);                                               //pig 2015.11.30 ModifyMonitorIssue
    }
}
//---------------------------------------------------------------------------
void TfMonitor::GetMonitorVedioState(int index)
{
    AnsiString aSendData;

    if(bCommandReady==false)
        return;

    aSendData.sprintf("GS0%d\r\n", index);
    MVCtrl->SendCommand(aSendData);
    sComData->Add(aSendData);                                                   //pig 2015.11.30 ModifyMonitorIssue
}
//---------------------------------------------------------------------------
void TfMonitor::GetMonitorHDSpec()                                              //pig 2015.11.21 ModifyMonitorFunction
{
    AnsiString aSendData;

    if(bCommandReady==false)
        return;

    if(sComData->Count==0)
    {
        aSendData.sprintf("GP\r\n");
        MVCtrl->SendCommand(aSendData);
        sComData->Add(aSendData);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::sbMonitorGetHDSpaceClick(TObject *Sender)            //pig 2015.11.30 ModifyMonitorIssue
{
    GetMonitorHDSpec();
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTimeMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 3000, 0);  //JerryYang 20160622
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::edWhenHDFullPromptMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 99, 0);  //JerryYang 20160622
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::edLowHDSpaceMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 99, 0);  //JerryYang 20160622
}
//---------------------------------------------------------------------------
void __fastcall TfMonitor::edMVPortMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_PORT);
}
//---------------------------------------------------------------------------