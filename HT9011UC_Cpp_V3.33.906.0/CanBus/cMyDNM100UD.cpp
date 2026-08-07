// =============================================================================
//  CanBus/cMyDNM100UD.cpp  --  TMyDNM100UD_Card implementation
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CanBus/cMyDNM100UD.cpp (585 lines)
//  Translation wave: W906-PT-W2 fieldbus (CanBus + EtherCAT group)
//  Translator: AI(W906-PT-W2) 20260807
//
//  WAVE SCOPE -- ACTIVE (faithful, whole file, all 10 golden functions):
//    TMyDNM100UD_Card::TMyDNM100UD_Card      golden :14-69
//    TMyDNM100UD_Card::~TMyDNM100UD_Card     golden :71-75
//    TMyDNM100UD_Card::Initail                golden :77-133
//    TMyDNM100UD_Card::TaskClear              golden :135-139
//    TMyDNM100UD_Card::asGetBaudRate          golden :141-147
//    TMyDNM100UD_Card::asSearchErrCodeMsg     golden :149-222
//    TMyDNM100UD_Card::tmrReadInputDataTimer  golden :226-480
//    TMyDNM100UD_Card::MyDNM100_SendExplicitMSG_W golden :482-517
//    TMyDNM100UD_Card::iTestSetIO_Value       golden :519-536
//    TMyDNM100UD_Card::iTestGetIO_Value       golden :538-552
//    TMyDNM100UD_Card::iTestGetValue          golden :554-568
//    TMyDNM100UD_Card::iTestGetIO             golden :570-584
//  SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//    TMyNUDN1 (CanBus/cMyNUDN1.h/.cpp, whole class incl. Initail()/
//    iReadInputData()/iSetIO_Value_Send()/iGetIO_Value[]/iSetIO_Value[]),
//    TQPF_Timer (myTimer.h, aliased to golden's `HTimer` -- see local typedef
//    below, same established idiom as acatchtray.cpp:114), MySleep
//    (declared locally, real body elsewhere per this tree's convention),
//    AnsiString::printf/sprintf (vclcompat).
//
//  GATE REGISTER -- 1 entry (many call sites, all the SAME reused flag):
//   (1) The DNM100_* vendor family (DNM100_TotalDNM100Board/ActiveBoard/
//       GetDLLVersion/GetFirmwareVersion/SetBaudRate/GetBaudRate/
//       ClearAllConfig/StartAllDevice/ResetFirmware/SendExplicitMSG_W/
//       IsExplicitMSGRespOK/GetExplicitMSGRespValue/CloseBoard, from
//       CanBus/DNM100API.h + CanBus/DNM100_BCB.lib, the SAME 泓格/ICP DAS
//       DeviceNet-master vendor SDK) is gated behind `#if HAVE_DNM100`,
//       REUSING the EXACT flag CanBus/cMyNUDN1.cpp already established for
//       this SDK (see that file's banner: "this machine's active hardware
//       inventory per KNOWLEDGE.md does not list a DNM100UD board; CanBus is
//       W5/未啟用"). Not a new gate invented by this unit; flagged here
//       because Hard Rule 6 technically applies (real vendor-DLL-exported
//       declarations, zero compiled bodies anywhere in this tree by design).
//       Call sites (golden line numbers): ctor :33, Initail() :84/:87/:90/
//       :93/:102/:111, tmrReadInputDataTimer case 1 :278 / case 2000 :380 /
//       case 2100 :413 / case 2200 :440, MyDNM100_SendExplicitMSG_W :486/
//       :494/:497, iTestSetIO_Value :534.
//       DEFAULT (HAVE_DNM100 undefined): every site takes the SAME
//       "simulate success" default CanBus/cMyNUDN1.cpp already established
//       (mirrors the TSimIOBackend "benign success" convention used
//       elsewhere in the HAL, IOBackend.cpp) -- board activation/baud-rate/
//       explicit-message calls all report success with harmless placeholder
//       values (DLL/Firmware version "0", explicit-message reads return 0),
//       so the state machine's SHAPE and cursor advancement are preserved
//       exactly; only the physical DeviceNet traffic is skipped.
//       BEHAVIOUR DELTA: on a real machine with a DNM100UD board this build
//       never actually opens/polls it -- identical in spirit to
//       cMyNUDN1.cpp's own HAVE_DNM100 default.
// =============================================================================
#include "CanBus/cMyDNM100UD.h"

#include "myTimer.h"        // TQPF_Timer
#include "acatchtray_shims.h" // NewRecordProcess

#include <cstdlib>          // atoi

#if HAVE_DNM100
#include "CanBus/DNM100API.h"   // READ-ONLY vendor header (泓格 DNM100UD SDK)
#endif

// AI(W906-PT-W2) 20260807: `ConType_Poll` (vendor DNM100API.h, ConnectionType
// enum, ==1) is assigned to the plain `iType` mode field in the ctor (golden
// :45) -- a call site that runs regardless of HAVE_DNM100 (it just records
// the intended I/O-communication mode, same as CanBus/cMyNUDN1.cpp's own
// local NUDN1ID/SETW/SETVALUE constant duplication for the identical
// "avoid depending on the vendor header outside the gate" reason). Provide
// the SAME numeric fallback when the vendor header was not included above.
#ifndef ConType_Poll
#define ConType_Poll 1
#endif

// AI(W906-PT-W2) 20260807: golden HTimer.h (a high-precision timer) is not
// translated in this tree; per the established tree-wide convention
// (acatchtray.cpp:114, aHotPlateSubstrate.h:1044, atester_shims.h) HTimer is
// aliased to TQPF_Timer offline -- same Off()/SetSecAndOn()/SetMSAndOn() API
// shape golden's HTimer exposes.
typedef TQPF_Timer HTimer;

// golden common.h declares MySleep but that declaration is gated
// `#if 0 // TODO(wave-timing)` in the translated common.h (real body lives
// in acarry_shims.cpp) -- declared locally instead, same pattern
// CCLink/MyCCLink.cpp already uses for the SAME reason.
extern void MySleep(DWORD dwMilliseconds);

//---------------------------------------------------------------------------
//  ctor / dtor
//  golden CanBus/cMyDNM100UD.cpp:14-75
//---------------------------------------------------------------------------
// AI(W906-PT-W2) 20260807: golden's `: TComponent(Owner)` base
// mem-initializer is dropped along with the TComponent base itself (see
// cMyDNM100UD.h header comment for the full "Owner" ctor-bug diagnosis and
// the in-wave precedent this follows, CanBus/cMyNUDN1.h). No base ctor call
// remains here because there is no base left to initialize.
TMyDNM100UD_Card::TMyDNM100UD_Card(unsigned short _iTotalDevices, unsigned char *_iDesMACIDList, unsigned char *_iAmplifierList)
{
    int i, iRet;

    // AI(W906-PT-W2) 20260807: golden's `if(NUDN1Item[i]!=NULL) delete
    // NUDN1Item[i];` guard below (golden :49-50) relies on VCL's
    // NewInstance zero-filling the whole instance before the ctor body runs
    // (same reasoning CanBus/cMyNUDN1.cpp's sibling EtherCAT/MyNUEC1.cpp
    // ctor banner documents for its own array members). Plain C++ `new
    // TMyDNM100UD_Card(...)` gives no such guarantee, so NUDN1Item[] is
    // explicitly zeroed here first -- ADDED (not present in golden's text)
    // to reproduce golden's ACTUAL runtime behavior.
    for(i=0; i<16; i++)
        NUDN1Item[i]=NULL;

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
#if HAVE_DNM100
    iRet=DNM100_TotalDNM100Board(&TotalBoards, iBoardIDList);
#else
    // GATE (1): no vendor SDK -- simulate "no DNM100UD board found".
    iRet=-1;
    TotalBoards=0;
#endif
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
    // AI(W906-PT-W2) 20260807: substitutes for golden's `tmrReadInputData=new
    // TTimer(this); ...->OnTimer=tmrReadInputDataTimer; ...->Interval=30;
    // ...->Enabled=true;` (see .h banner note -- bTimerEnabled stands in for
    // tmrReadInputData->Enabled).
    bTimerEnabled=true;
    iDNM100UDTask=1;
    bUseRespValueDelay=false;//V270 不用延遲
    bCanBusSeach=false;
}
//---------------------------------------------------------------------------
TMyDNM100UD_Card::~TMyDNM100UD_Card()
{
    bTimerEnabled=false;   // golden: tmrReadInputData->Enabled=false;
    //delete tmrReadInputData;
}
//---------------------------------------------------------------------------
//  Initail -- second-phase board init. golden cMyDNM100UD.cpp:77-133.
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::Initail()
{
    int iRet, i;

    bInitialCardOK=false;

    //啟動 BoardCard
#if HAVE_DNM100
    iRet=DNM100_ActiveBoard(iActivedBoardNo);
#else
    iRet=0;   // GATE (1): no vendor SDK -- simulate a successful board activation.
#endif
    if(iRet!=0) return -iRet;

    //取得 BoardCard DLL Version
#if HAVE_DNM100
    asDLLVer.printf("%X", DNM100_GetDLLVersion());
#else
    asDLLVer.printf("%X", 0);   // GATE (1): no vendor SDK.
#endif

    //取得 BoardCard Firmware Version
#if HAVE_DNM100
    asFirmwareVer.printf("%X", DNM100_GetFirmwareVersion(iActivedBoardNo));
#else
    asFirmwareVer.printf("%X", 0);   // GATE (1): no vendor SDK.
#endif

    //設定 BoardCard BaudRate
#if HAVE_DNM100
    iRet=DNM100_SetBaudRate(iActivedBoardNo, iBaudRateType);
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
#else
    // GATE (1): no vendor SDK -- nothing to set/clear offline; simulate the
    // BaudRate readback matching so the (golden-dead-anyway) mismatch branch
    // above is not spuriously taken.
    iCurrectBaudRateType=iBaudRateType;
#endif
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

#if HAVE_DNM100
    DNM100_StartAllDevice(iActivedBoardNo);
#endif
    // #else: GATE (1), no vendor SDK -- nothing to start offline (matches
    // golden's own lack of return-code checking on this fire-and-forget call).
    bInitialCardOK=true;
    return 0;
}
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
//  tmrReadInputDataTimer -- switch(Task)-stepped DNM100UD fleet poll.
//  golden CanBus/cMyDNM100UD.cpp:226-480.  __fastcall dropped (see .h note).
//  golden's bare property reads (`->iGetAmplifier`/`->iGetDesMACID`/
//  `->iGetActivedBoardNo`, no parens) become method CALLS here per
//  CanBus/cMyNUDN1.h's own __property->getter translation.
//---------------------------------------------------------------------------
void TMyDNM100UD_Card::tmrReadInputDataTimer(TObject * /*Sender*/)
{
    // AI(W906-PT-W2) 20260807: substitutes for the VCL TTimer's own Enabled
    // gate (a real TTimer with Enabled==false simply never fires OnTimer) --
    // see .h note. Golden itself only checks bInitialOK/bHasErr/bCanBusSeach
    // here (the TTimer's Enabled gate was implicit); bTimerEnabled is ADDED
    // to reproduce that implicit gate now that there is no real VCL TTimer.
    if(bTimerEnabled==false)
        return;
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
                    bTimerEnabled=false;   // golden: tmrReadInputData->Enabled=false;
                    ErrMess="DNM100U-D Borad Card Time Out Over Count, ResetFirmware";
                    NewRecordProcess("", "DNM100U-D Borad Card is error, over reset", ErrMess);
#if HAVE_DNM100
                    DNM100_ResetFirmware(iActivedBoardNo);
#endif
                    // #else: GATE (1), no vendor SDK -- nothing to reset
                    // offline (fire-and-forget call in golden too).
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
                if(NUDN1Item[iDevicesCount]->iGetAmplifier()==0)
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
                    ErrMess.printf("NUDN1=%d, %s", NUDN1Item[iDevicesCount]->iGetDesMACID() ,asSearchErrCodeMsg(iRet));
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
                if(iAmplifierCount>=NUDN1Item[iDevicesCount]->iGetAmplifier())
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
#if HAVE_DNM100
            iRet=DNM100_SendExplicitMSG_W(NUDN1Item[iDevicesCount]->iGetActivedBoardNo(),
                                          NUDN1Item[iDevicesCount]->iGetDesMACID(),
                                          ServiceID,
                                          ClassID,
                                          InstanceID,
                                          AttributeIDLen,
                                          AttributeID);
#else
            // GATE (1): no vendor SDK -- simulate a successful send.
            iRet=0;
#endif
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
                    ErrMess.printf("NUDN1=%d, AmplifierNo=%d, %s",NUDN1Item[iDevicesCount]->iGetDesMACID(),(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
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
#if HAVE_DNM100
            iRet=DNM100_IsExplicitMSGRespOK(NUDN1Item[iDevicesCount]->iGetActivedBoardNo(),
                                            NUDN1Item[iDevicesCount]->iGetDesMACID());
#else
            // GATE (1): no vendor SDK -- simulate the request completing OK.
            iRet=0;
#endif
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
                ErrMess.printf("NUDN1=%d, AmplifierNo=%d, IsReqOK? TimeOut!, %s ",NUDN1Item[iDevicesCount]->iGetDesMACID(),(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
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
#if HAVE_DNM100
                iRet=DNM100_GetExplicitMSGRespValue(NUDN1Item[iDevicesCount]->iGetActivedBoardNo(),
                                                    NUDN1Item[iDevicesCount]->iGetDesMACID(),
                                                    &GetDataLen,
                                                    &iGetValue);
#else
                // GATE (1): no vendor SDK -- simulate a successful read of
                // value 0.
                iRet=0;
                iGetValue=0;
#endif
                if(iRet==0)
                {
                    bHasErr=false;
                    ErrMess="";
                    NUDN1Item[iDevicesCount]->iGetIO_Value[iAmplifierCount]=iGetValue;
                    iAmplifierCount++;
                    if(iAmplifierCount>=NUDN1Item[iDevicesCount]->iGetAmplifier())
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
                    ErrMess.printf("NUDN1=%d, AmplifierNo=%d, %s",NUDN1Item[iDevicesCount]->iGetDesMACID(),(iAmplifierCount+1),asSearchErrCodeMsg(iRet));
                    NewRecordProcess("", "DNM100U-D Read Value Of Output IsReqValue? is error", ErrMess);
                }
            }
            break;
    }
}
//---------------------------------------------------------------------------
//  MyDNM100_SendExplicitMSG_W -- send+poll+read one Explicit Message.
//  golden CanBus/cMyDNM100UD.cpp:482-517.
//---------------------------------------------------------------------------
int TMyDNM100UD_Card::MyDNM100_SendExplicitMSG_W(BYTE DesMACID, BYTE ServiceID, WORD ClassID, WORD InstanceID, WORD DataLen, BYTE *DATA, WORD GetDataLen)
{
#if HAVE_DNM100
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
#else
    // GATE (1): no vendor SDK -- nothing to send/poll. Faithful default:
    // report the SAME timeout golden's own retry-exhausted path reports
    // (-2007, My_ExplicitMSGRespOKTimeOut -- see asSearchErrCodeMsg).
    (void)DesMACID; (void)ServiceID; (void)ClassID; (void)InstanceID;
    (void)DataLen; (void)DATA; (void)GetDataLen;
    return -2007;
#endif
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

#if HAVE_DNM100
    iRet=DNM100_SendExplicitMSG_W(iActivedBoardNo, DesMACID, ServiceID, ClassID, InstanceID, AttributeIDLen, AttributeID);
#else
    // GATE (1): no vendor SDK -- simulate a successful send.
    (void)DesMACID; (void)ClassID; (void)InstanceID; (void)AttributeIDLen; (void)AttributeID[0];
    iRet=0;
#endif
    return -iRet;
}
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
}
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
}
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
