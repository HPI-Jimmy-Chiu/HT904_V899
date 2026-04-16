#include "MachineDefine.h"
#pragma hdrstop

#include "InterfaceSYS.h"

#include "cpublic.h"
#include "cprod.h"
#include "cmydef.h"

#include "BarCode.h"
#include "Main.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//編寫紀錄
//bGPIBError , bEcho , bExist 錯誤程序  支援
//bDoubleContact  支援
//

using namespace std;
#define iGPIBMAXItem 32

GPIBInterface_Data *GPIBSend=new GPIBInterface_Data("TSerialPoll", "Interface", WM_Interface_GPIB);
M_V *MV_Event=new M_V;
M_V *MV_ESD=new M_V;
M_V *MV_AutoUpdate=new M_V;                                                     //Steven 20150728 : Auto Update

extern bool bGPIBError;                                                         //OLD : 負責處理 Tester 因為資料面錯誤時的回報。
extern bool bEcho, bUnderTest;                                                  //OLD : 負責處理 Tester 回傳資料  Echo = true  代表  Handler 收到 Tester 的資料了
extern bool bExist;                                                             //OLD : 負責處理 Tester Time Out 時的處理
extern bool bDoubleContact;
extern unsigned int iBin[4][8];
//==============================================================================
GPIBInterface_Data::GPIBInterface_Data(AnsiString aFormName, AnsiString aCaptionName, WPARAM WPARAM_TO)
{
    bModeType     = 0x00 ;
    bCommandType  = 0x00 ;
    bCommand      = 0x00 ;
    FormName      = aFormName;
    CaptionName   = aCaptionName;
    _WPARAM_TO    = WPARAM_TO;

    vData.clear();
    vMessage.clear();
};
//==============================================================================
GPIBInterface_Data::~GPIBInterface_Data()
{
    try
    {
        for(vector<Byte>::iterator iter=vData.begin(); iter!=vData.end(); ++iter)
        {
            delete iter;
        }
        vData.clear();

        for(vector<Byte>::iterator iter=vMessage.begin(); iter!=vMessage.end(); ++iter)
        {
            delete iter;
        }
        vMessage.clear();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~GPIBInterface_Data");
    }
};
//==============================================================================
bool GPIBInterface_Data::bFindProgram()
{
    _HVisionWnd=FindWindow(FormName.c_str(), CaptionName.c_str());
    if(_HVisionWnd)
        return true;
    else
        return false;
};
//==============================================================================
void GPIBInterface_Data::PushData(int iDatain[], int iSize)
{
    vData.clear();
    for(int i=0; i<iSize; i++)
        vData.push_back((Byte)*(iDatain+i));
};
//==============================================================================
void GPIBInterface_Data::PushData(bool bDatain[], int iSize)
{
    vData.clear();
    for(int i=0; i<iSize; i++)
    {
        if(*(bDatain+i))
            vData.push_back('1');
        else
            vData.push_back('0');
    }
};
//==============================================================================
void GPIBInterface_Data::PushMessage(int iMessagein[], int iSize)
{
    vMessage.clear();
    for(int i=0; i<iSize; i++)
        vMessage.push_back((Byte)*(iMessagein+i));
};
//==============================================================================
void GPIBInterface_Data::PushMessage(bool bMessagein[], int iSize)
{
    vMessage.clear();
    for(int i=0; i<iSize; i++)
    {
        if(*(bMessagein+i))
            vMessage.push_back('1');
        else
            vMessage.push_back('0');
    }
};
//==============================================================================
void GPIBInterface_Data::PushMessage(Byte ByteMessagein[], int iSize)
{
    vMessage.clear();
    for(int i=0; i<iSize; i++)
        vMessage.push_back(*(ByteMessagein+i));
};
//==============================================================================
void GPIBInterface_Data::PushMessage(vector <Byte> vMessagein, int iSize)
{
    vMessage.clear();
    for(int i=0; i<iSize; i++)
        vMessage.push_back(vMessagein[i]);
}
//==============================================================================
void GPIBInterface_Data::SendStructMessage_SendMessage()
{
    if(bFindProgram()==false)
        return ;

    pMV                 =new M_V;
    pcp                 =new COPYDATASTRUCT;
    pMV->bModeType      =bModeType;
    pMV->bCommandType   =bCommandType;
    pMV->bCommand       =bCommand;
    pMV->iDataSize      =vData.size();
    pMV->iMessageSize   =vMessage.size();

    for(unsigned int i=0; i<vData.size(); i++)
    {
        pMV->bData[i]=vData[i];
    }
    for(unsigned int i=0; i<vMessage.size(); i++)
    {
        pMV->bMessage[i]=vMessage[i];
    }
    pcp->dwData=_WPARAM_TO;
    pcp->cbData=sizeof(M_V);
    pcp->lpData=(M_V*)pMV;

    SendMessage(_HVisionWnd, WM_COPYDATA, _WPARAM_TO, (LPARAM)pcp);
    delete pMV;
    delete pcp;
};
//==============================================================================
void GPIBInterface_Data::Clear()
{
    bModeType     = 0x00;
    bCommandType  = 0x00;
    bCommand      = 0x00;
    vData.clear();
    vMessage.clear();
};
//==============================================================================
//1.送出Close GPIB Program
void _CloseGpibProgram()
{
    //結構
    GPIBSend->Clear();
    GPIBSend->bModeType    = TYPE_HANDLER_GPIB;
    GPIBSend->bCommandType = CommandType_CLOSE;
    GPIBSend->SendStructMessage_SendMessage();
}
//==============================================================================
//2.Send START RUN GPIB InterFace Program
bool _RunTestProgram_BarMess(int iDataSize, bool *bSiteOpen, int iMessageSize, Byte *bMessage)
{
    //全域變數
    bEcho  = false;
    bExist = false;
    bUnderTest=true;
    //結構
    GPIBSend->Clear();
    GPIBSend->bModeType    = TYPE_HANDLER_GPIB;
    GPIBSend->bCommandType = CommandType_BARCODE;

    if(LastSet.iTester==ON_LINE)
        GPIBSend->bCommand |= CommandType_TEST_REAL;                            //Steven 20150713 : 整理LastSet.iTester

    if(TestISTimeOut)
        GPIBSend->bCommand |= CommandType_TEST_TIMEOUT;

    GPIBSend->PushData(bSiteOpen, iDataSize);
    GPIBSend->PushMessage(bMessage, iMessageSize);
    GPIBSend->SendStructMessage_SendMessage();

    return true;
}
//==============================================================================
bool _RunTestProgram(int iDataSize, bool *bSiteOpen)
{
    //全域變數
    bEcho  = false;
    bExist = false;
    bUnderTest=true;
    //iSendGpibTestHome=true;                                                   //kevin 20150626 送出測試訊號 等收到資料才能歸home
    //結構
    GPIBSend->Clear();
    GPIBSend->bModeType    = TYPE_HANDLER_GPIB;
    GPIBSend->bCommandType = CommandType_TEST;

    if(LastSet.iTester==ON_LINE)
        GPIBSend->bCommand |= CommandType_TEST_REAL;                            //Steven 20150713 : 整理LastSet.iTester
//    if(bDoubleContact&LastSet.bGPIBSupportDoubleContact0x42Command)
//        GPIBSend->bCommand |= CommandType_TEST_DOUBLE;
    if(TestISTimeOut)
        GPIBSend->bCommand |= CommandType_TEST_TIMEOUT;

    GPIBSend->PushData(bSiteOpen, iDataSize);

    GPIBSend->SendStructMessage_SendMessage();

    return true;
}
//==============================================================================
//3.Send GPIB ADDR To InterFace Program
void _SendADDRToInterfaceProgram(int iADDR)
{
    GPIBSend->Clear();
    GPIBSend->bModeType    =TYPE_HANDLER_GPIB;
    GPIBSend->bCommandType =CommandType_INFSEND;
    GPIBSend->bCommand     =CommandType_INFSEND_SENDGPIBADDR;
    GPIBSend->PushData(&iADDR, 1);
    GPIBSend->SendStructMessage_SendMessage();
}
//==============================================================================
//4.Send Information To InterFace Program
void _SendInformationToInterfaceProgram(Byte _BModeType, Byte _bCommandType, Byte _bCommand, int _iData[], int _iDataSize, int _iMessage[], int _iMessageSize)
{
    GPIBSend->bModeType    = _BModeType;
    GPIBSend->bCommandType = _bCommandType;
    GPIBSend->bCommand     = _bCommand ;
    GPIBSend->PushData(_iData,_iDataSize);
    GPIBSend->SendStructMessage_SendMessage();
}
//==============================================================================
//1.Command_TEST 相關//---------------------------------------------------------
//2.Command_ECHOBACK 相關//-----------------------------------------------------
bool _FeedBackHandlerVariable(M_V &pVM)
{
    if(pVM.bCommand==CommandType_ECHOBACK_BACKGPIBADDR)
    {
        fMain->oldGpibAddress=pVM.bData[0];
    }

    return false;
}
//==============================================================================
bool _GPIBGetCommand_TEST(M_V &pVM)
{
    char cBuffer[1024];
    AnsiString asBuffer ="";
    AnsiString asReMess[iGPIBMAXItem] ={""};
    int k=0;

    if(pVM.bCommand & CommandType_TESTECHOOK)
    {
        int iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF.iSiteMap[i][j]<=0)                                    //Hung 20111108 add
                {
                    iBin[i][j]=0;
                }
                else
                {
                    if((TestIF.iSiteMap[i][j]-1)<int(pVM.iDataSize))
                    {
                        iBin[i][j]=pVM.bData[TestIF.iSiteMap[i][j]-1];

                    }
                    else
                    {
                        iBin[i][j]=0;
                    }
                    iCount++;
                }
            }
        }

        memset(cBuffer,'\0', sizeof(cBuffer));
        for(int i=0; i<int(pVM.iMessageSize); i++)
            *(cBuffer+i)=(char) *(pVM.bMessage+i);
        asBuffer=cBuffer;

        if(asBuffer.Pos("BARCODEERR:")==1)
        {
            asBuffer.Delete(1, 11);

            if(asBuffer.Pos(";") !=0) asBuffer=asBuffer.SubString(1, asBuffer.Pos(";")-1);
            if(asBuffer.Pos("\r")!=0) asBuffer=asBuffer.SubString(1, asBuffer.Pos("\r")-1);

            for(int j=0; j<asBuffer.Length(); j++)
            {
                if(asBuffer[j+1]!=',')
                {
                    asReMess[k]=asReMess[k]+asBuffer[j+1];
                }
                else
                {
                    k++;
                }
            }

            if(k==iGPIBMAXItem-1)
            {
                for(int i=0; i<iGPIBMAXItem; i++)
                {
                    fBarCode->iBinReturnMess[iGPIBMAXItem-i-1]=asReMess[i];
                }
                fBarCode->bGPIBTestBarCodeError=true;
            }
            else
            {
                fBarCode->bGPIBTestBarCodeFormatError=true;
            }
        }
        else
        {
            fBarCode->bGPIBTestBarCodeError      =false;
            fBarCode->bGPIBTestBarCodeFormatError=false;
        }
        bGPIBError=false;
        bTimeOutForNoFullSite=false;
        bEcho     =true;
        bUnderTest=false;
        bExist    =true;
        return true;
    }
    else
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF.iSiteMap[i][j]<=0)                                    //Hung 20111108 add
                {
                    iBin[i][j]=0;
                }
                else
                {
                    if((TestIF.iSiteMap[i][j]-1)<int(pVM.iDataSize))
                        iBin[i][j]=pVM.bData[TestIF.iSiteMap[i][j]-1];
                    else
                        iBin[i][j]=0;
                }
            }
        }

        bTimeOutForNoFullSite=false;
        bGPIBError=true;
        bEcho     =true;
        bUnderTest=false;
        bExist    =true;
        return true;
    }
}
//==============================================================================
//0.Command 接收端
M_V *pVM;
void _OnMyCopyMsg_Interface(TMessage &msg)
{
    PCOPYDATASTRUCT P   =(PCOPYDATASTRUCT) msg.LParam;
    &pVM->bModeType     =(Byte *)P->lpData;

    if(pVM->bModeType==TYPE_HANDLER_GPIB)
    {
        if(pVM->bCommandType      == CommandType_TEST)
        {

        }
        else if(pVM->bCommandType == CommandType_TESTECHO)
        {
            _GPIBGetCommand_TEST(*pVM);
        }
        else if(pVM->bCommandType == CommandType_CLOSE)
        {
            _CloseGpibProgram();
        }
        else if(pVM->bCommandType == CommandType_ECHOBACK)
        {
            _FeedBackHandlerVariable(*pVM);
        }
    }
    else
    {

    }
//    delete pVM;
}
//---------------------------------------------------------------------------
void _SendStructMessage_Send(Byte bType, Byte bSendCommandType, Byte bSendCommand, vector <Byte> vData, vector <Byte> vMessage)
{
    if(fMain->HESDWnd==NULL)
    {
        return;
    }

    M_V            *pMV =new M_V;
    COPYDATASTRUCT *pcp =new COPYDATASTRUCT;
    pMV->bModeType      =bType;
    pMV->bCommandType   =bSendCommandType;
    pMV->bCommand       =bSendCommand;
    pMV->iDataSize      =vData.size();

    for(unsigned int i=0; i<vData.size(); i++)
    {
        pMV->bData[i]=vData[i];
    }
    pMV->iMessageSize=vMessage.size();

    for(unsigned int i=0; i<vMessage.size(); i++)
    {
        pMV->bMessage[i]=vMessage[i];
    }

    pcp->dwData=WM_ESD_Program;
    pcp->cbData=sizeof(M_V);
    pcp->lpData=(M_V*)pMV;

    int Lengh=0;
    if(bEPHandleToESD || bTempHandleToESD)                                      //kevin 20160106
    {
        if(bEPHandleToESD)
        {
           Lengh=bEPMessage.Length();
           bEPHandleToESD=false;
           memcpy(pMV->cSendData, bEPMessage.c_str(), Lengh);
        }

        if(bTempHandleToESD)
        {
          Lengh= bTempMessage.Length();
          bTempHandleToESD=false;
          memcpy(pMV->cSendData, bTempMessage.c_str(), Lengh);
        }
    }
    SendMessage(fMain->HESDWnd, WM_COPYDATA, WM_ESD_Program, (LPARAM)pcp);
    delete pcp;
    delete pMV;
}
//******************************************************************************
//
//  注意!! SendCommand_ESD為Handler與ESD通訊相關, 修改時要小心!!
//
//******************************************************************************
void SendCommand_ESD(ESD_COMMAND CMD)
{
    if(USE_NOVX3360 || USE_KASUGA || USE_KASUGA_Fan ||                          //KaiChen 20191225 ：KASUGA Fan 通訊  //kevin 20180821
       iUseHTIonBarFunction!=0)                                                 //RogerYang 20250825 : Unloader新增3支IonBar，取代4 5 8 ion fan
    {
        Byte  bModeType;
        Byte  bCommandType;
        Byte  bCommand;
        vector <Byte>  vSendData;
        vSendData.clear();
        vector <Byte>  vSendMessage;
        vSendMessage.clear();
        //============================
        bModeType    = TYPE_HANDLER_ESD;
        bCommandType = (Byte)CommandType_ESD;
        bCommand     = (Byte)CMD;
        _SendStructMessage_Send(bModeType, bCommandType, bCommand, vSendData, vSendMessage);
    }
}
//---------------------------------------------------------------------------
void SendCommand_EventLog(EventLog_COMMAND CMD, AnsiString Data)                //Steven 20231127 : 改用分析器
{
    if(CosFunction.bUseMDB==false)
    {
        if(fMain->HEventLogWnd==NULL)
            fMain->HEventLogWnd=FindWindow("TfrmELA", "Event Log Analyzer");

        if(fMain->HEventLogWnd==NULL)
            return;

        M_V            *pMV =new M_V;
        COPYDATASTRUCT *pcp =new COPYDATASTRUCT;

        pMV->bModeType      =TYPE_HANDLER_EventLog;
        pMV->bCommandType   =(Byte)CommandType_EventLog;
        pMV->bCommand       =(Byte)CMD;
        pMV->iDataSize      =Data.Length();
        pMV->bData[0]       =atoi(Data.c_str());
        pMV->iMessageSize   =0;
        pcp->dwData=WM_EventAnalysis;
        pcp->cbData=sizeof(M_V);
        pcp->lpData=(M_V*)pMV;
        SendMessage(fMain->HEventLogWnd, WM_COPYDATA, WM_EventAnalysis, (LPARAM)pcp);
        delete pcp;
        delete pMV;
    }
}
//---------------------------------------------------------------------------
void SendCommand_AutoUpdate(AutoUpdate_COMMAND CMD, AnsiString Data)            //Steven 20150728 : Auto Update
{
    fMain->HAutoUpdateWnd=FindWindow("TfAutoUpdate", "fAutoUpdate");
    if(fMain->HAutoUpdateWnd==NULL)
        return;

    M_V            *pMV =new M_V;
    COPYDATASTRUCT *pcp =new COPYDATASTRUCT ;

    pMV->bModeType      =TYPE_HANDLER_AutoUpdate;
    pMV->bCommandType   =(Byte)CommandType_AutoUpdate;
    pMV->bCommand       =(Byte)CMD;
    pMV->iDataSize      =Data.Length();
    pMV->bData[0]       =atoi(Data.c_str());
    pMV->iMessageSize   =0;

    pcp->dwData=WM_AutoUpdate;
    pcp->cbData=sizeof(M_V);
    pcp->lpData=(M_V*)pMV;
    SendMessage(fMain->HAutoUpdateWnd, WM_COPYDATA, WM_AutoUpdate, (LPARAM)pcp);
    delete pcp;
    delete pMV;
}
//---------------------------------------------------------------------------

