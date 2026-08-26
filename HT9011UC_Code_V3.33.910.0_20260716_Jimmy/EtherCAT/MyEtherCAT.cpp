//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MyEtherCAT.h"
#include "MyNUEC1.h"
#include "common.h"
#include "cmydef.h"
#include "AdvMotDrv.h"
#include "AdvMotApi.h"
#include "MyMotor.h"
#include "note.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TMyEtherCAT *MyEtherCAT;
#pragma package(smart_init)
__fastcall TMyEtherCAT::TMyEtherCAT(unsigned short _iTotalDevices,unsigned char *_iDesMACIDList,unsigned char *_iAmplifierList): TComponent(Owner)
{
    iTotalDevices=_iTotalDevices;                                               //使用幾顆 NU-EC1
    for(int i=0; i<MaxNUEC1; i++)
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
    iRing=iEtherCatRing;

    for(int i=0; i<iTotalDevices; i++)
    {
        if(NUEC1Item[i]!=NULL)
            delete NUEC1Item[i];

        NUEC1Item[i]=new TMyNUEC1(iRing,
                                  iDesMACIDList[i],
                                  iAmplifierList[i]);
    }
    bReOpenPCI1203=false;

    tmrReadInputData=new TTimer(this);
    tmrReadInputData->OnTimer=tmrReadInputDataTimer;
    tmrReadInputData->Interval=30;
    tmrReadInputData->Enabled=false;
    iEtherCATTask=1;
};
//---------------------------------------------------------------------------
__fastcall TMyEtherCAT::~TMyEtherCAT()
{
    tmrReadInputData->Enabled=false;
};
//---------------------------------------------------------------------------
void TMyEtherCAT::Start()
{
     tmrReadInputData->Enabled=true;
}
//---------------------------------------------------------------------------
void TMyEtherCAT::Pause()
{
     tmrReadInputData->Enabled=false;
}
//---------------------------------------------------------------------------
void TMyEtherCAT::TaskClear()
{
    bHasErr=false;
    iEtherCATTask=1;
    bReOpenPCI1203=true;
}
//---------------------------------------------------------------------------
void __fastcall TMyEtherCAT::tmrReadInputDataTimer(TObject *Sender)
{
    static bool bIn=false;
    static int iNUEC1No=0;
    static int iErrCount=0;

    if(bInitialOK==false ||
       bHasErr==true ||
       bIn==true)                                                               //Steven 20250915 : EtherCAT Timer加上保護
    {
        return;
    }

    bool bRet=false;
    int ret=0;
    int &Task=iEtherCATTask;
    AnsiString sErr="";
    AnsiString StrEng="";
    bIn=true;

    switch (Task)
    {
        case 1:
            if(bReOpenPCI1203)                                                  //PCI1203 重新開卡
            {
                Task=500;
                break;
            }
            iNUEC1No=0;
            Task=1000;
            break;
        case 500:                                                               //PCI1203 重新開卡
            ret=OpenEtherCatMastCard();
            if(ret!=SUCCESS)
            {
//                ShowErrorMessage("WAR16150", 0, MMSystem, 0, IntToStr(ret));    //Eastsun 20260710整合
                AnsiString sCode = "0x" + IntToHex(ret, 8) + " : " + GetEtherCATErrDesc(ret);  //Eastsun 20260710整合
                ShowErrorMessage("WAR16150", 0, MMSystem, 0, sCode);          //Eastsun 20260710整合
            }
            bReOpenPCI1203=false;
            Task=1;
            break;
        case 1000:                                                              //讀取 FS-N12 Sensor 數值
            if(NUEC1Item[iNUEC1No]->iGetAmplifier==0)                           //沒有接 FS-N12 就跳下醫科
            {
                iNUEC1No++;
                if(iNUEC1No>=iTotalDevices)                                     //讀寫完全部 NU-EC1
                {
                    Task=1;
                }
                break;
            }

            SysMess="Read Currect Value";
            bRet=NUEC1Item[iNUEC1No]->ReadCurrentValue();
            if(bRet)
            {
                iErrCount=0;
                Task=2000;
            }
            else
            {
                iErrCount++;
                if(iErrCount>10)
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUEC1=%d, EtherCAT Read CurrentValue error", NUEC1Item[iNUEC1No]->iGetDesMACID);
                    NewRecordProcess("", "EtherCAT Read CurrentValue error", ErrMess);
                }
                else
                {
                    Task=1000;
                }
            }
            break;
        case 2000:                                                              //讀取 FS-N12 Sensor IO
            SysMess="Read I/O Value";
            bRet=NUEC1Item[iNUEC1No]->ReadCurrentIO();
            if(bRet)
            {
                iErrCount=0;
                Task=3000;
            }
            else
            {
                iErrCount++;
                if(iErrCount>10)
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUEC1=%d, EtherCAT Read I/O error", NUEC1Item[iNUEC1No]->iGetDesMACID);
                    NewRecordProcess("", "EtherCAT Read I/O error", ErrMess);
                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 3000:                                                              //設定 FS-N12 Sensor IO Value 閥值
            bRet=NUEC1Item[iNUEC1No]->WriteSettingValue();
            if(bRet)
            {
                iErrCount=0;
                Task=4000;
            }
            else
            {
                iErrCount++;
                if(iErrCount>10)
                {
                    Task=1;
                    bHasErr=true;
                    for(int i=0;i<MaxFSN12;i++)
                       sErr+=AnsiString(NUEC1Item[iNUEC1No]->RetWriteSettingValue[i]);

                    ErrMess.printf("NUEC1=%d, EtherCAT Setting I/O Value error %s", NUEC1Item[iNUEC1No]->iGetDesMACID,sErr);
                    NewRecordProcess("", "EtherCAT Setting I/O Value error", ErrMess);
                    NUEC1Item[iNUEC1No]->ClearError();
                }
                else
                {
                    Task=3000;
                }
            }
            break;
        case 4000:                                                              //讀取 FS-N12 IO Value 閥值
            bRet=NUEC1Item[iNUEC1No]->ReadSettingValue();
            if(bRet)
            {
                iErrCount=0;
                Task=1000;
                iNUEC1No++;
                if(iNUEC1No>=iTotalDevices)                                     //讀寫完全部 NU-EC1
                {
                    Task=1;
                    break;
                }
            }
            else
            {
                iErrCount++;
                if(iErrCount>10)
                {
                    Task=1;
                    bHasErr=true;
                    ErrMess.printf("NUEC1=%d, EtherCAT Read I/O Value error", NUEC1Item[iNUEC1No]->iGetDesMACID);
                    NewRecordProcess("", "EtherCAT Read I/O Value error", ErrMess);
                }
                else
                {
                    Task=4000;
                }
            }
            break;
    }

    bIn=false;
};
extern bool MNetLog(AnsiString Message);                                        //Steven 20110406
//------------------------------------------------------------------------------
bool bCheckPCI_EtherCatStateRun=false;
int CheckPCI_EtherCatState()                                                    //Sam 20230811 : 新增 EtherCAT 檢查功能
{
    if(MyEtherCAT->InitialOK==false)                                            //RogerYang 20250731 fix flag
    {
        return 0;
    }

    if(bCheckPCI_EtherCatStateRun==true)
    {
        return 0;
    }
    bCheckPCI_EtherCatStateRun=true;
    #ifdef SOFT_SIMULTE
    {
        bCheckPCI_EtherCatStateRun=false;
        return 1;
    }
    #else
    U32 Result, MasCheckEvt=0, iCheckAxisCnt=0, iCheckIoCnt=0;
    ULONG  AxesPerDev, buffLen=64;
    AnsiString S="", Str1="", sRet="";
    int iIORing=1;
    unsigned int iTotalDeviceRing0=0, iTotalDeviceRing1=0;
    U16 SlaveState=0;
    bool bResetEtherCAT=false, bCheckIOError=false;
    static int iFailCount=0;

    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT  ||
       SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
    {
        iTotalDeviceRing1+=fCCLink->iTotalDevices;
    }

    if(VCCU_UNIT_TYPE==1)
    {
        if(MachineTypeChoice==Type_HT9046      ||
           MachineTypeChoice==Type_HT9046_LS   ||
           MachineTypeChoice==Type_HT1032      ||
           USE_46_SUCKER_DB==1)                                                 //HT9046LS sucker_9046.db
        {
            iTotalDeviceRing1+=1;                                               //目前只裝1個在模擬測試
        }
        else
        {
            iTotalDeviceRing1+=1;                                               //目前只裝1個在模擬測試
        }
    }

    Result=Acm_DevCheckEvent(uiDevhand, &MasCheckEvt, 10);                      //檢查 Ring
    if(Result!=SUCCESS ||
       (MasCheckEvt&EVT_DEV_DISCONNET) ||
       (MasCheckEvt&EVT_DEV_IO_DISCONNET))
    {
        if(MasCheckEvt&EVT_DEV_DISCONNET &&
           iTotalDeviceRing0>0)                                                 //雖然事件已經抓到是 Ring0 斷線，可能是誤判，再抓 Device 數量來雙重判斷
        {
            Result=Acm_GetU32Property(uiDevhand, FT_MasCyclicCnt_R0, &iCheckAxisCnt);
            if(Result==SUCCESS)
            {
                if(iTotalDeviceRing0!=iCheckAxisCnt)
                {
                    S.sprintf("Ring0 Error Device %[d] [%d]", iTotalDeviceRing0 ,iCheckAxisCnt);
                    bResetEtherCAT=true;
                }
            }
            else
            {
                S="Ring0 Error";
                bResetEtherCAT=true;
            }
        }
        else if(MasCheckEvt & EVT_DEV_IO_DISCONNET && iTotalDeviceRing1>0)
        {
            Result=Acm_GetU32Property(uiDevhand, FT_MasCyclicCnt_R1, &iCheckIoCnt);
            if(Result==SUCCESS)
            {
                if(iTotalDeviceRing1!=iCheckIoCnt)
                {
                    bResetEtherCAT=true;
                    S="Ring1 Error";                                            //RgoerYang 20250411 add
                }
            }
            else
            {
                S="Ring1 Error";
                bResetEtherCAT=true;
            }
        }
        else
        {
            S="Ring Error";
            bResetEtherCAT=true;
        }
    }

    if(bResetEtherCAT)
    {
        ShowErrorMessage("WAR16152", 0, MMSystem, 0, S);
    }

    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT  ||
       SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
    {                                                                           //檢查 NU-EC1 站別
        Result=Acm_GetProperty(uiDevhand, FT_DevAxesCount, &AxesPerDev, &buffLen);
        if(Result==SUCCESS)
        {
            Str1="";
            for(int i=0; i<fCCLink->iTotalDevices; i++)
            {
                SlaveState=0;
                Result=Acm_DevGetSlaveStates(uiDevhand, iIORing, fCCLink->iDesMACIDList[i], &SlaveState);
                if(Result==SUCCESS)
                {
                    if(SlaveState!=EC_SLAVE_STATE_OP)
                    {
                        bCheckIOError=true;
                        Str1+=S.sprintf("[%d]=X ",fCCLink->iDesMACIDList[i]);
                    }
                    else
                    {
                        Str1+=S.sprintf("[%d]=O ",fCCLink->iDesMACIDList[i]);
                    }
                }
                else
                {
                    Str1+=S.sprintf("[%d]=X ",fCCLink->iDesMACIDList[i]);
                    bCheckIOError=true;
                }
            }

            if(bCheckIOError)
            {
                ShowErrorMessage("WAR16151", 0, MMSystem, 0, Str1);
                bResetEtherCAT=true;
            }
        }

        if(SystemHour==0 && SystemMin==0 &&
           (0<=SystemSec && SystemSec<20))                                      //JerryYang 20250723 : 抓數量錯誤先不alarm
        {
            iFailCount=0;
        }

        for(int i=0; i<3; i++)                                                  //檢查各 NU-EC1 Amplifier 數量是否正確  //JerryYang 20250723 : Mark掉
        {
            sRet=MyEtherCAT->NUEC1Item[i]->CheckAmplifierExist();
            if(sRet!="OK")
            {
                iFailCount++;
                if(iFailCount<10)
                {
                    Str1.sprintf("EtherCAT AMP count error, %s", sRet);
                    MNetLog(Str1);
//                    NewRecordProcess("", Str1);
                }
//                ShowErrorMessage("WAR16153", 0, MMSystem, 0, sRet);
            }
        }
    }

    if(bResetEtherCAT)
    {
        int ret=OpenEtherCatMastCard();
        if(ret!=SUCCESS)
        {
//            ShowErrorMessage("WAR16150", 0, MMSystem, 0, IntToStr(ret));    //Eastsun 20260710整合
            AnsiString sCode = "0x" + IntToHex(ret, 8) + " : " + MyEtherCAT->GetEtherCATErrDesc(ret);  //Eastsun 20260710整合
            ShowErrorMessage("WAR16150", 0, MMSystem, 0, sCode);              //Eastsun 20260710整合
        }
    }
    bCheckPCI_EtherCatStateRun=false;
    return 0;
    #endif
}
//------------------------------------------------------------------------------
bool INSTALL_ETHETCAT()                                                         //Sam 20230210 : 新增 VacuumUnit 通訊模組
{
    bool bRet=false;
    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT  ||
       SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3 ||
       VCCU_UNIT_TYPE==1)
    {
        bRet=true;
    }
    return bRet;
}
//------------------------------------------------------------------------------
int OpenEtherCatMastCard()                                                      //Sam 20230707 : EtherCAT Shuttle sensor
{
    DEVLIST m_avaDevs[MAX_DEVICES];
    U32 Result, deviceCounter=0;
    AnsiString sErrorCode=0;
    Acm_DevClose(&uiDevhand);
    Result=Acm_GetAvailableDevs(m_avaDevs, MAX_DEVICES, &deviceCounter);
    if(Result!=SUCCESS)
    {
//        sErrorCode=IntToHex((int)Result, 8);    //Eastsun 20260710整合
//        return atoi(sErrorCode.c_str());    //Eastsun 20260710整合
        //==>
        return (int)Result;                                                 //Eastsun 20260710整合
    }

    m_dwDevNum=m_avaDevs[0].dwDeviceNum;
    int iOpenCardFailTimes=0;

    while(true)
    {
        if(iOpenCardFailTimes>2)                                                //開卡失敗
        {
//            sErrorCode=IntToHex((int)Result, 8);    //Eastsun 20260710整合
//            return atoi(sErrorCode.c_str());    //Eastsun 20260710整合
            //==>
            return (int)Result;                                                 //Eastsun 20260710整合
        }
        else
        {
            Result=Acm_DevOpen(m_dwDevNum, &uiDevhand);
            if(Result==SUCCESS)
            {
                break;
            }
            else
            {
                ReOpen:
                Result=Acm_DevReOpen(uiDevhand);
                if(Result!=EC_OpenMasterDevFailed)
                {
                    goto ReOpen;
                }
                Acm_DevClose(&uiDevhand);
                Sleep(2000);
                iOpenCardFailTimes++;
            }
        }
    }
    MySleep(500);
    U32 MasEnableEvt=0;
    MasEnableEvt|=EVT_DEV_DISCONNET;
    MasEnableEvt|=EVT_DEV_IO_DISCONNET;
    Result=Acm_DevEnableEvent(uiDevhand, MasEnableEvt);                         //啟用檢查事件
    if(Result!=SUCCESS)
    {
//        sErrorCode=IntToHex((int)Result, 8);    //Eastsun 20260710整合
//        return atoi(sErrorCode.c_str());    //Eastsun 20260710整合
        //==>
        return (int)Result;                                                     //Eastsun 20260710整合
    }

    for(int i=0; i<TOTAL_MOTOR; i++)                                            //Rogeryang 20250411 EtherCAT Motor Reinitiate here
    {
        if(MOT[i].Motor!=NULL         &&
         MOT[i].CardType=="PCI1203" &&
         MOT[i].Motor->Enable)
        {
          MOT[i].Motor->ResetAxisOpen();
          MOT[i].Motor->InitMotor(0); //No Need Addr here
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
AnsiString TMyEtherCAT::GetEtherCATErrDesc(int ret)                             //Eastsun 20260710整合
{
    switch((unsigned int)ret)
    {
        case 0x83000000: return "Failed to get NIC count";
        case 0x83000001: return "Failed to get NIC information";
        case 0x83000002: return "Failed to open EtherCAT master device";
        case 0x83000003: return "Failed to get slave device";
        case 0x83000004: return "Failed to start OP mode";
        case 0x83000005: return "Failed to close device";
        case 0x83000006: return "Memory allocation failed";
        case 0x83000007: return "Invalid NIC index";
        case 0x83000008: return "Failed to open device";
        case 0x83000009: return "Failed to read configuration file";
        case 0x8300000A: return "Failed to get NIC info (2)";
        case 0x8300000B: return "Failed to read SDO object";
        case 0x8300000C: return "Invalid parameter";
        case 0x8300000D: return "Failed to get PDO offset";
        case 0x8300000E: return "Failed to initialize mapping info";
        case 0x8300000F: return "Failed to initialize resource";
        case 0x83000010: return "Failed to write SDO object";
        case 0x83000011: return "Invalid port type";
        case 0x83000012: return "Failed to set cycle time";
        case 0x83000013: return "Invalid AO range";
        case 0x83000014: return "Invalid AI range";
        case 0x83000015: return "Failed to get slave info";
        case 0x83000016: return "Failed to get network state";
        case 0x83000017: return "Failed to register event";
        case 0x83000018: return "Invalid integration time";
        case 0x83000019: return "Invalid AI enable setting";
        case 0x8300001A: return "Invalid DI filter setting";
        case 0x8300001B: return "Failed to set slave state";
        case 0x8300001C: return "Zero calibration failed";
        case 0x8300001D: return "Invalid master handle";
        case 0x8300001E: return "Invalid ENI configuration file";
        case 0x8300001F: return "Invalid calibration type";
        case 0x83000020: return "Failed to set calibration value";
        case 0x83000021: return "AO calibration failed";
        case 0x83000022: return "Invalid IO mapping";
        case 0x83000023: return "Port index exceeds port count";
        case 0x83000024: return "Channel ID exceeds channel count";
        case 0x83000025: return "Input index exceeds input count";
        case 0x83000026: return "Output index exceeds output count";
        case 0x83000027: return "Failed to set enable";
        case 0x83000028: return "Failed to set AI range";
        case 0x83000029: return "Failed to set integration time";
        case 0x8300002A: return "Property not supported";
        case 0x8300002B: return "Slave ID conflicted - duplicate slave ID on network";
        case 0x8300002C: return "Span calibration failed";
        case 0x8300002D: return "Invalid AI value";
        case 0x8300002E: return "Invalid AO value";
        case 0x8300002F: return "Failed to get module";
        case 0x83000030: return "Invalid counter enable setting";
        case 0x83000031: return "Invalid counter pulse-in mode";
        case 0x83000032: return "Invalid counter initial value";
        case 0x83000033: return "Invalid counter max value";
        case 0x83000034: return "Invalid counter overflow mode";
        case 0x83000035: return "Invalid counter latch enable";
        case 0x83000036: return "Invalid counter latch edge";
        case 0x83000037: return "Invalid counter compare enable";
        case 0x83000038: return "Invalid counter compare method";
        case 0x83000039: return "Invalid compare DO enable";
        case 0x8300003A: return "Invalid compare DO output mode";
        case 0x8300003B: return "Invalid compare DO logic";
        case 0x8300003C: return "Invalid compare DO pulse width";
        case 0x8300003D: return "Axis counter exceeded limit";
        case 0x8300003E: return "Firmware upgraded - please restart";
        case 0x8300003F: return "Invalid counter pulse-in logic";
        case 0x83000040: return "Invalid counter pulse-in max frequency";
        case 0x83000041: return "Invalid counter multi-compare deviation";
        case 0x83000042: return "Invalid AO enable setting";
        case 0x83000043: return "Invalid DO enable setting";
        case 0x83000044: return "Invalid counter latch logic";
        case 0x83000045: return "Invalid compare DO link pulse-in";
        case 0x83000046: return "Invalid DO output mode";
        case 0x83000047: return "Invalid DO logic";
        case 0x83000048: return "Invalid DO pulse width";
        case 0x83000049: return "Invalid DO delay time";
        case 0x8300004A: return "Invalid DO link compare DO";
        case 0x8300004B: return "Unequal compare data numbers";

        // ECTDevError group (0x83010000)
        case 0x83010001: return "EtherCAT master internal error";
        case 0x83010002: return "Ring0 slave disconnected";
        case 0x83010003: return "Ring1 slave disconnected";
        case 0x83010004: return "Ring0 check OP state timeout";
        case 0x83010005: return "Ring0 initialization timeout";
        case 0x83010006: return "Ring0 start timeout";
        case 0x83010007: return "Ring0 get slave ID timeout";
        case 0x83010008: return "Ring0 slave lost PDO threshold warning";
        case 0x83010009: return "Ring1 slave lost PDO threshold warning";
        case 0x8301000A: return "Ring0 slave mode changed unexpectedly";
        case 0x8301000B: return "Ring1 slave mode changed unexpectedly";
        case 0x8301000C: return "Ring0 slave lost frame";
        case 0x8301000D: return "Ring1 slave lost frame";
        case 0x8301000E: return "Ring0 reconnect failed";
        case 0x8301000F: return "Ring1 reconnect failed";
        case 0x83010010: return "Ring1 check OP state timeout";
        case 0x83010011: return "Ring1 initialization timeout";
        case 0x83010012: return "Ring1 start timeout";
        case 0x83010013: return "Ring1 get slave ID timeout";
        case 0x83010014: return "Ring0 get device type failed";
        case 0x83010015: return "Ring1 get device type failed";

        // ECTAxError group (0x83050000)
        case 0x83050001: return "Axis retry error";
        case 0x83050002: return "Axis reset counter error";
        case 0x83050003: return "Axis command error protection triggered";
        case 0x83050004: return "Slave servo alarm active";
        case 0x83050005: return "Following error exceeded limit";
        case 0x83050006: return "Home operation failed";

        case 0x83050011: return "Operation mode set: protocol timeout";
        case 0x83050012: return "Operation mode set: object not exist";
        case 0x83050013: return "Operation mode set: sub-index not exist";
        case 0x83050014: return "Operation mode set: value range exceeded";
        case 0x83050015: return "Operation mode set: written value too high";
        case 0x83050016: return "Operation mode set: written value too low";
        case 0x83050017: return "Operation mode set: max less than min";
        case 0x83050018: return "Operation mode set: other error";

        case 0x83050021: return "Home offset set: protocol timeout";
        case 0x83050022: return "Home offset set: object not exist";
        case 0x83050023: return "Home offset set: sub-index not exist";
        case 0x83050024: return "Home offset set: value range exceeded";
        case 0x83050025: return "Home offset set: written value too high";
        case 0x83050026: return "Home offset set: written value too low";
        case 0x83050027: return "Home offset set: max less than min";
        case 0x83050028: return "Home offset set: other error";

        case 0x83050031: return "Home mode set: protocol timeout";
        case 0x83050032: return "Home mode set: object not exist";
        case 0x83050033: return "Home mode set: sub-index not exist";
        case 0x83050034: return "Home mode set: value range exceeded";
        case 0x83050035: return "Home mode set: written value too high";
        case 0x83050036: return "Home mode set: written value too low";
        case 0x83050037: return "Home mode set: max less than min";
        case 0x83050038: return "Home mode set: other error";

        // ECTMotionError group (0x83030000)
        case 0x83030001: return "Motion master handle is NULL";
        case 0x83030002: return "Motion scan is busy";
        case 0x83030003: return "Motion domain setup failed";
        case 0x83030004: return "Motion slave not supported";
        case 0x83030005: return "Motion slave configuration failed";
        case 0x83030006: return "Motion activation failed";
        case 0x83030007: return "Motion domain data error";
        case 0x83030008: return "Too many motion axes";

        // ECTIOError group (0x83040000)
        case 0x83040001: return "IO master handle is NULL";
        case 0x83040002: return "IO scan is busy";
        case 0x83040003: return "IO domain setup failed";
        case 0x83040004: return "IO slave not supported";
        case 0x83040005: return "IO slave configuration failed";
        case 0x83040006: return "IO activation failed";
        case 0x83040007: return "IO domain data error";

        // ECTDevSDOError group (0x83020000)
        case 0x83025300: return "SDO: toggle bit not changed";
        case 0x83025400: return "SDO: protocol timeout";
        case 0x83025401: return "SDO: client/server command not valid or unknown";
        case 0x83025405: return "SDO: out of memory";
        case 0x83026100: return "SDO: unsupported access to object";
        case 0x83026101: return "SDO: attempt to read a write-only object";
        case 0x83026102: return "SDO: attempt to write a read-only object";
        case 0x83026200: return "SDO: object does not exist in dictionary";
        case 0x83026441: return "SDO: object cannot be mapped into PDO";
        case 0x83026442: return "SDO: mapped objects exceed PDO length";
        case 0x83026443: return "SDO: general parameter incompatibility";
        case 0x83026447: return "SDO: general internal incompatibility";
        case 0x83026600: return "SDO: hardware error";
        case 0x83026710: return "SDO: data type mismatch - length does not match";
        case 0x83026712: return "SDO: data type mismatch - length too high";
        case 0x83026713: return "SDO: data type mismatch - length too low";
        case 0x83026911: return "SDO: sub-index does not exist";
        case 0x83026930: return "SDO: value range of parameter exceeded";
        case 0x83026931: return "SDO: written value too high";
        case 0x83026932: return "SDO: written value too low";
        case 0x83026936: return "SDO: maximum value less than minimum value";
        case 0x83028000: return "SDO: general error";
        case 0x83028020: return "SDO: data cannot be stored to application";
        case 0x83028021: return "SDO: data cannot be stored due to local control";
        case 0x83028022: return "SDO: data cannot be stored due to device state";
        case 0x83028023: return "SDO: object dictionary dynamic generation fails";

        default:
            return "Unknown error code";
    }
}

