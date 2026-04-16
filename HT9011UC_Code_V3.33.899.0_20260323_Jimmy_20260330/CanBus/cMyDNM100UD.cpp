#include "MachineDefine.h"
#pragma hdrstop

#include "cMyDNM100UD.h"
#include "cMyNUDN1.h"

//#include "MyCCLinkSensor.h"
//#include "MachineDefine.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMyDNM100UD_Card::TMyDNM100UD_Card(unsigned short _iTotalDevices,unsigned char *_iDesMACIDList,unsigned char *_iAmplifierList): TComponent(Owner)
{
    int i, iRet;
    DNM100IsShutDownNeedPowerOffPC=false;
    DNM100InitailCount=0;
    iTotalDevices=_iTotalDevices;
    for(i=0; i<16; i++)
    {
        if(i<iTotalDevices)
        {
            iDesMACIDList[i]=_iDesMACIDList[i];
            iAmplifierList[i]=_iAmplifierList[i];
        }
        else
        {
            iDesMACIDList[i]=0;
        }
    }
    //找尋 BoardCard No
    iRet=DNM100_TotalDNM100Board(&TotalBoards,iBoardIDList);
    if(iRet!=0 || TotalBoards==0)
    {
         iActivedBoardNo=0;
        //return -2001;
    }
    else
    {
        iActivedBoardNo=iBoardIDList[0];        //目前寫法僅支援一張 泓格 DNM100UD
    }

    iBaudRateType=0;         //0:500k ; 1:250k ; 2:125k  (bit/s)
    iType=ConType_Poll;

    for(i=0; i<iTotalDevices; i++)
    {
        if(NUDN1Item[i]!=NULL)
            delete NUDN1Item[i];

        NUDN1Item[i]=new TMyNUDN1(iActivedBoardNo,
                                  iDesMACIDList[i],
                                  iType,
                                  (iAmplifierList[i]*2)+8,
                                  0,
                                  200,
                                  iAmplifierList[i]);
    }

    //tmrReadInputData=new TTimer(fCCLink);
    tmrReadInputData=new TTimer(this);
    tmrReadInputData->OnTimer=tmrReadInputDataTimer;
    tmrReadInputData->Interval=30;
    tmrReadInputData->Enabled=true;
    iDNM100UDTask=1;
    bUseRespValueDelay=false;//V270 不用延遲
    bCanBusSeach=false;
};
//---------------------------------------------------------------------------
__fastcall TMyDNM100UD_Card::~TMyDNM100UD_Card()
{
    tmrReadInputData->Enabled=false;
    //delete tmrReadInputData;
};
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::Initail()
{
    int iRet,i;

    bInitialCardOK=false;

    //啟動 BoardCard
    iRet=DNM100_ActiveBoard(iActivedBoardNo);
    if(iRet!=0) return -iRet;
    //取得 BoardCard DLL Version
    asDLLVer.printf("%X",DNM100_GetDLLVersion());

    //取得 BoardCard Firmware Version
    asFirmwareVer.printf("%X",DNM100_GetFirmwareVersion(iActivedBoardNo));

    //設定 BoardCard BaudRate
    iRet=DNM100_SetBaudRate(iActivedBoardNo,iBaudRateType);
    if(iRet!=0)
    {
        DNM100_CloseBoard(iActivedBoardNo);
        bActivedBoard=false;
        return -iRet;
    }

    //取得 BoardCard 目前 BaudRate
    iCurrectBaudRateType=DNM100_GetBaudRate(iActivedBoardNo);
    if(iCurrectBaudRateType!=iBaudRateType)
    {
        DNM100_CloseBoard(iActivedBoardNo);
        bActivedBoard=false;
        return -2003;   //BaudRate 錯誤
    }

    //清除 BoardCard 裡面所有 Devices
    DNM100_ClearAllConfig(iActivedBoardNo);
//    for(i=0; i<iTotalDevices; i++)
//    {
//        if(NUDN1Item[i]!=NULL)
//            delete NUDN1Item[i];
//
//        NUDN1Item[i]=new TMyNUDN1(iActivedBoardNo,
//                                  iDesMACIDList[i],
//                                  iType,
//                                  (iAmplifierList[i]*2)+8,
//                                  0,
//                                  200,
//                                  iAmplifierList[i]);
//    }
    for(i=0; i<iTotalDevices; i++)
    {
        NUDN1Item[i]->Initail();
    }

    DNM100_StartAllDevice(iActivedBoardNo);
    bInitialCardOK=true;
    return 0;
};
//---------------------------------------------------------------------------
void TMyDNM100UD_Card::TaskClear()
{
    bHasErr=false;
    iDNM100UDTask=1;
}
//---------------------------------------------------------------------------
AnsiString TMyDNM100UD_Card::asGetBaudRate()
{
    if      (iCurrectBaudRateType==0)   {return "500k(bit/s)";}
    else if (iCurrectBaudRateType==1)   {return "250k(bit/s)";}
    else if (iCurrectBaudRateType==2)   {return "125k(bit/s)";}
    else                                {return "ERROR";}
}
//---------------------------------------------------------------------------
AnsiString  TMyDNM100UD_Card::asSearchErrCodeMsg(int i)
{
    //Board Error Code
    if      (i==-10001)     return "DNM100_DriverError";
    else if (i==-10002)     return "DNM100_ActiveBoardError";
    else if (i==-10003)     return "DNM100_BoardNumberError";
    else if (i==-10004)     return "DNM100_PortNumberError";
    else if (i==-10007)     return "DNM100_InitError";
    else if (i==-10021)     return "DNM100_SoftBufferIsEmpty";
    else if (i==-10022)     return "DNM100_SoftBufferIsFull";
    else if (i==-10023)     return "DNM100_TimeOut";
    else if (i==-10024)     return "DNM100_SetCyclicMsgFailure";
    else if (i==-10025)     return "DNM100_DpramOverRange";
    else if (i==-10026)     return "DNM100_NoDpramCmd";
    else if (i==-10027)     return "DNM100_ModeError";
    else if (i==-10030)     return "DNM100_NoFileInside";
    else if (i==-10031)     return "DNM100_DownloadFailure";
    else if (i==-10032)     return "DNM100_EEPROMDamage";
    else if (i==-10033)     return "DNM100_NotEnoughSpace";
    else if (i==-10034)     return "DNM100_StillDownloading";
    else if (i==-10035)     return "DNM100_BoardModeError";
    else if (i==-10036)     return "DNM100_CardTypeError";

    //Firmware Error Code
    else if (i==-5000)      return "DNMXS_UnKnowError";

    //Master Status Error Code
    else if (i==-1000)      return "DNMXS_BoardNotActive";
    else if (i==-1001)      return "DNMXS_OnlineError";
    else if (i==-1002)      return "DNMXS_CANBusError";
    else if (i==-1003)      return "DNMXS_Booting";

    //General Error Code
    else if (i==-1050)      return "DNMXS_MACIDError";
    else if (i==-1051)      return "DNMXS_BaudRateError";
    else if (i==-1052)      return "DNMXS_ConnectionTypeError";
    else if (i==-1053)      return "DNMXS_DuplicMasterMACID";           //DesMACID與Master MACID相同
    else if (i==-1054)      return "DNMXS_EEPROMError";
    else if (i==-1055)      return "DNMXS_NowScanning";
    else if (i==-1056)      return "DNMXS_ScanListError";
    else if (i==-1057)      return "DNMXS_DeviceExist";
    else if (i==-1058)      return "DNMXS_DeviceNotExist";
    else if (i==-1059)      return "DNMXS_MapTableError";

    //IOConnection Error
    else if (i==-1100)      return "DNMXS_ExplicitNotAllocate";
    else if (i==-1101)      return "DNMXS_PollNotAllocate";
    else if (i==-1102)      return "DNMXS_BitStrobeNotAllocate";
    else if (i==-1103)      return "DNMXS_COSNotAllocate";
    else if (i==-1104)      return "DNMXS_CyclicNotAllocate";
    else if (i==-1105)      return "DNMXS_PollAlreadyExist";
    else if (i==-1106)      return "DNMXS_BitStrobeAlreadyExist";
    else if (i==-1107)      return "DNMXS_COSAlreadyExist";
    else if (i==-1108)      return "DNMXS_CyclicAlreadyExist";
    else if (i==-1109)      return "DNMXS_CommunicationPause";

    //Slave Error Code
    else if (i==-1150)      return "DNMXS_SlaveNoResp";
    else if (i==-1151)      return "DNMXS_WaitForSlaveResp";
    else if (i==-1152)      return "DNMXS_SlaveRespError";
    else if (i==-1153)      return "DNMXS_OutputDataLenError";
    else if (i==-1154)      return "DNMXS_InputDataLenError";

    //Input / Output Area
    else if (i==-1200)      return "DNMXS_OutofRange";

    //My Define
    else if (i==-2001)      return "My_BoardNoExist";                   //找不到 泓格 DNM100UD 卡片
    else if (i==-2003)      return "My_SetBandRateNotMatch";            //設定完 BandRate 後，讀回 BandRate 不相同
    else if (i==-2004)      return "My_DevicesNoExist";                 //找不到 KYEC NUDN1
    else if (i==-2006)      return "My_StartAllDeviceTimeOut";          //找尋所有 Device 逾時
    else if (i==-2007)      return "My_ExplicitMSGRespOKTimeOut";       //Explicit Message 回應 OK 逾時。
    else                    return "UnKnowError_Message";
}
HTimer DoDNM100ResetFirmwareDelay;
HTimer GetExplicitMSGRespValueDelay;
//---------------------------------------------------------------------------
void __fastcall TMyDNM100UD_Card::tmrReadInputDataTimer(TObject *Sender)
{
    if(bInitialOK==false)
        return;
    if(bHasErr==true)
        return;
    if(bCanBusSeach==true)
        return;

    int iRet;
    int iSetValue;
    unsigned char   ServiceID;
    unsigned short  ClassID;
    unsigned short  InstanceID;
    unsigned short  AttributeIDLen;
    unsigned char   AttributeID[1];
    unsigned short  GetDataLen ;
    unsigned short  iGetValue;             //Use Explicit Messaging 讀到 I/O 閥值暫存器。

    static int iDevicesCount;
    static bool bfirstReadSettig=true;      //Sam 20191003 : 只有第一次才全部讀取閥值，以後有變更才讀取，加快速度
    static int iAmplifierCount;
    static int iReqCount;
    static int iErrCount;

    int &Task=iDNM100UDTask;
    switch (Task)
    {
        case 1:
            SysMess="Active DNM100U-D Borad Card";
            iRet=Initail();
            if(iRet==0)
            {
                if(DNM100InitailCount>20)
                {
                    DNM100IsShutDownNeedPowerOffPC=true;
                }
                DNM100InitailCount=0;
                bHasErr=false;
                ErrMess="";
                iDevicesCount=0;
                iErrCount=0;
                Task=1000;
            }
            else
            {
                DNM100InitailCount++;
                if(DNM100InitailCount>5)
                {
                    tmrReadInputData->Enabled=false;
                    ErrMess="DNM100U-D Borad Card Time Out Over Count, ResetFirmware";
                    NewRecordProcess("", "DNM100U-D Borad Card is error, over reset", ErrMess);
                    DNM100_ResetFirmware(iActivedBoardNo);
                    DoDNM100ResetFirmwareDelay.SetSecAndOn(3);
                    Task=100;
                }
                else
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess=asSearchErrCodeMsg(iRet);
                }
            }
            break;
       case 100:
            if(DoDNM100ResetFirmwareDelay.Off())
            {
                Task=1;
            }
            break;
       case 1000:
            SysMess="Read I/O And Currect Value";
            iRet=NUDN1Item[iDevicesCount]->iReadInputData(2);
            if(iRet==0)
            {
                bHasErr=false;
                ErrMess="";
                iErrCount=0;
                if(NUDN1Item[iDevicesCount]->iGetAmplifier==0)
                {
                    iDevicesCount++;
                    if(iDevicesCount>=iTotalDevices)
                    {
                        bfirstReadSettig=false;//Sam 20191003 : 只有第一次才全部讀取閥值，以後有變更才讀取，加快速度
                        iDevicesCount=0;
                        iErrCount=0;
                        Task=1000;          //重新刷新資料
                    }
                }
                else
                {
                    iAmplifierCount=0;
                    Task=1500;
                }
            }
            else
            {
                iErrCount++;
                if(iErrCount>500)
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUDN1=%d, %s", NUDN1Item[iDevicesCount]->iGetDesMACID ,asSearchErrCodeMsg(iRet));
                    NewRecordProcess("", "DNM100U-D ReadInputData is timeout", ErrMess);
                }
                else
                {
                    Task=1000;
                }
            }
            break;
        case 1500:                                                              //設定 Sensor 閥值
            iSetValue=NUDN1Item[iDevicesCount]->iSetIO_Value[iAmplifierCount];
            if(iSetValue>=0 || bfirstReadSettig)                                //Sam 20191003 : 只有第一次才全部讀取閥值，以後有變更才讀取，加快速度
            {
                iRet=NUDN1Item[iDevicesCount]->iSetIO_Value_Send(iAmplifierCount, iSetValue);
                if(iRet==0)
                {
                    NUDN1Item[iDevicesCount]->iSetIO_Value[iAmplifierCount]=-1;
                }
                Task=2000;
            }
            else
            {
                iAmplifierCount++;
                if(iAmplifierCount>=NUDN1Item[iDevicesCount]->iGetAmplifier)
                {
                    iDevicesCount++;
                    if(iDevicesCount>=iTotalDevices)
                    {
                        bfirstReadSettig=false;                                 //Sam 20191003 : 只有第一次才全部讀取閥值，以後有變更才讀取，加快速度
                        iDevicesCount=0;
                        iErrCount=0;
                        Task=1000;                                              //重新刷新資料
                    }
                    else
                    {
                        Task=1000;
                    }
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 2000:
            SysMess="Read IO Value Setting Req.";
            //Input
            ServiceID      =SETR;       //讀取 IO 閥值請求
            ClassID        =NUDN1ID;
            InstanceID     =(iAmplifierCount+1)*10+2;
            AttributeIDLen =1;
            AttributeID[0] =SETVALUE;
            iRet=DNM100_SendExplicitMSG_W(NUDN1Item[iDevicesCount]->iGetActivedBoardNo,
                                          NUDN1Item[iDevicesCount]->iGetDesMACID,
                                          ServiceID,
                                          ClassID,
                                          InstanceID,
                                          AttributeIDLen,
                                          AttributeID);
            if(iRet==0)
            {
                bHasErr=false;
                ErrMess="";
                iErrCount=0;
                iReqCount=0;
                Task=2100;
            }
            else
            {
                iErrCount++;
                if(iErrCount>2000)
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUDN1=%d, AmplifierNo=%d, %s",NUDN1Item[iDevicesCount]->iGetDesMACID,(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
                    NewRecordProcess("", "DNM100U-D Read IO Value Setting Req. is timeout", ErrMess);
                }
                else
                {
                    Task=2000;
                }
            }
            break;
       case 2100:
            SysMess="Read Value Of Output IsReqOK?.";
            iRet=DNM100_IsExplicitMSGRespOK(NUDN1Item[iDevicesCount]->iGetActivedBoardNo,
                                            NUDN1Item[iDevicesCount]->iGetDesMACID);
            iReqCount++;
            if(iRet==0)
            {
                if(bUseRespValueDelay)
                {
                    GetExplicitMSGRespValueDelay.SetMSAndOn(50); //V260 需要延遲在取 Value
                }
                Task=2200;
            }
            else if(iReqCount>2000)
            {
                Task=1;
                bHasErr=true;
                ErrMess.printf("NUDN1=%d, AmplifierNo=%d, IsReqOK? TimeOut!, %s ",NUDN1Item[iDevicesCount]->iGetDesMACID,(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
                NewRecordProcess("", "DNM100U-D Read Value Of Output IsReqOK? is error", ErrMess);
                break;
            }
            else
            {
                break;
            }
        case 2200:
            if(GetExplicitMSGRespValueDelay.Off() || bUseRespValueDelay==false)
            {
                GetDataLen=4;
                iRet=DNM100_GetExplicitMSGRespValue(NUDN1Item[iDevicesCount]->iGetActivedBoardNo,
                                                    NUDN1Item[iDevicesCount]->iGetDesMACID,
                                                    &GetDataLen,
                                                    &iGetValue);
                if(iRet==0)
                {
                    bHasErr=false;
                    ErrMess="";
                    NUDN1Item[iDevicesCount]->iGetIO_Value[iAmplifierCount]=iGetValue;
                    iAmplifierCount++;
                    if(iAmplifierCount>=NUDN1Item[iDevicesCount]->iGetAmplifier)
                    {
                        iDevicesCount++;
                        if(iDevicesCount>=iTotalDevices)
                        {
                            bfirstReadSettig=false;//Sam 20191003 : 只有第一次才全部讀取閥值，以後有變更才讀取，加快速度
                            iDevicesCount=0;
                            iErrCount=0;
                            Task=1000;          //重新刷新資料
                        }
                        else
                        {
                            Task=1000;
                        }
                    }
                    else
                    {
                        Task=1500;
                    }
                }
                else
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUDN1=%d, AmplifierNo=%d, %s",NUDN1Item[iDevicesCount]->iGetDesMACID,(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
                    NewRecordProcess("", "DNM100U-D Read Value Of Output IsReqValue? is error", ErrMess);
                }
            }
            break;
    }
};
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::MyDNM100_SendExplicitMSG_W(BYTE DesMACID, BYTE ServiceID, WORD ClassID, WORD InstanceID, WORD DataLen, BYTE *DATA, WORD GetDataLen)
{
    int iRet,iCount;
    WORD iGetValue;
    iRet=DNM100_SendExplicitMSG_W(iActivedBoardNo,DesMACID,ServiceID,ClassID,InstanceID,DataLen,DATA);
    if(iRet==0)
    {
        iCount=0;
        while(1)
        {
            MySleep(5);      //要等一下才讀才不會有問題
            iCount++;
            iRet=DNM100_IsExplicitMSGRespOK(iActivedBoardNo,DesMACID);
            if(iRet==0)
            {
                iRet=DNM100_GetExplicitMSGRespValue(iActivedBoardNo,DesMACID,&GetDataLen,&iGetValue);
                if(iRet==0)
                {
                    return iGetValue;
                }
                else
                {
                    return -iRet;
                }
            }
            else if(iCount>20)
            {
                return -2007;
            }
        }
    }
    else
    {
        return -iRet;
    }
}
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::iTestSetIO_Value(BYTE DesMACID,byte iAmplifierNo,int iSetValue)
{
    int iRet;
    UNIONWORD iSetValue_HL;
    byte AttributeID[3];
    //Input
    byte ServiceID          =SETW;
    WORD ClassID            =NUDN1ID;
    WORD InstanceID         =(iAmplifierNo+1)*10+2;
    WORD AttributeIDLen     =3;
    iSetValue_HL.WHOLE_WORD =(unsigned short)iSetValue;
    AttributeID[0]          =SETVALUE;
    AttributeID[1]          =iSetValue_HL.BYTES.Lo_BYTE;   //低位元
    AttributeID[2]          =iSetValue_HL.BYTES.Hi_BYTE;   //高位元

    iRet=DNM100_SendExplicitMSG_W(iActivedBoardNo, DesMACID, ServiceID, ClassID, InstanceID, AttributeIDLen, AttributeID);
    return -iRet;
};
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::iTestGetIO_Value(BYTE DesMACID,byte iAmplifierNo)
{
    int iRet;
    byte AttributeID[1];
    //Input
    byte ServiceID      =SETR;
    WORD ClassID        =NUDN1ID;
    WORD InstanceID     =(iAmplifierNo+1)*10+2;
    WORD AttributeIDLen =1;
    AttributeID[0]      =SETVALUE;
    WORD DataLen        =4;

    iRet=MyDNM100_SendExplicitMSG_W(DesMACID, ServiceID, ClassID, InstanceID, AttributeIDLen, AttributeID, DataLen);
    return iRet;
};
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::iTestGetValue(BYTE DesMACID,byte iAmplifierNo)
{
    int iRet;
    byte AttributeID[1];
    //Input
    byte ServiceID      =SETR;
    WORD ClassID        =NUDN1ID;
    WORD InstanceID     =(iAmplifierNo+1)*10+1;
    WORD AttributeIDLen =1;
    AttributeID[0]      =GETCURRECT;
    WORD DataLen        =4;

    iRet=MyDNM100_SendExplicitMSG_W(DesMACID, ServiceID, ClassID, InstanceID, AttributeIDLen, AttributeID, DataLen);
    return iRet;
};
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::iTestGetIO(BYTE DesMACID,byte iAmplifierNo)
{
    int iRet;
    byte AttributeID[1];
    //Input
    byte ServiceID      =SETR;
    WORD ClassID        =NUDN1ID;
    WORD InstanceID     =(iAmplifierNo+1)*10+1;
    WORD AttributeIDLen =1;
    AttributeID[0]      =GETOUTPUT;
    WORD DataLen        =3;

    iRet=MyDNM100_SendExplicitMSG_W(DesMACID, ServiceID, ClassID, InstanceID, AttributeIDLen, AttributeID, DataLen);
    return iRet;
}
//---------------------------------------------------------------------------
