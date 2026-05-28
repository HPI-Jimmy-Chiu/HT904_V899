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
                ShowErrorMessage("WAR16150", 0, MMSystem, 0, IntToStr(ret));
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
    if(InitialOK==false)
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
            ShowErrorMessage("WAR16150", 0, MMSystem, 0, IntToStr(ret));
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
        sErrorCode=IntToHex((int)Result, 8);
        return atoi(sErrorCode.c_str());
    }

    m_dwDevNum=m_avaDevs[0].dwDeviceNum;
    int iOpenCardFailTimes=0;

    while(true)
    {
        if(iOpenCardFailTimes>2)                                                //開卡失敗
        {
            sErrorCode=IntToHex((int)Result, 8);
            return atoi(sErrorCode.c_str());
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
        sErrorCode=IntToHex((int)Result, 8);
        return atoi(sErrorCode.c_str());
    }

    return 0;
}
//------------------------------------------------------------------------------
