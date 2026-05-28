#include "MachineDefine.h"
#pragma hdrstop

#include "MyMotor.h"
#include "myGALILmotor.h"

#include "MachineType.h"
#include "cpublic.h"
#include "myswitch.h"
#include "MyLaneIo.h"
#include "note.h"
#include "mymessbox.h"
#include "uMotorTest.h"
#include "uhome.h"
#include "cOffSet.h"
#include "common.h"
#include "main.h"
#include "cinitial.h"
#include "cMyDB.h"
#include "cmydef.h"
#include "mysensor.h"
#include "csystem.h"
#include "mycylin.h"
#include "OCR.h"
#include "iosetview.h"
#include "myMN200motor.h"
#include "asendic.h"
#include "AdvMotDrv.h"
#include "AdvMotApi.h"
#include "MyCCLinkSensor.h"
#include "rs232.h"

extern bool bGali_CardInstall;
extern bool GaliAxisAlarm[4];
extern long rc;
extern char response[64];
extern char szBuffer[128], szBuffer1[128];
extern HANDLEDMC hDmc;
extern HWND hWnd;
extern class TTrayMotor MOT[300];
extern int TestYBuffer, TestY2Buffer;
extern int GailAcSpeed;
extern int GailDcSpeed;
extern int GailAcSpeed2;
extern int GailDcSpeed2;
extern int iEncoderTorence;
extern int iCheckZ;
extern CRITICAL_SECTION g_cs;
extern char bMask[8];
extern HAlarm *Alarm;
extern bool IndexZCanMove[2];
extern int DelayCount;

bool CheckTestZ(AnsiString sFunc);
bool CheckTestZ1(AnsiString sFunc);
bool CheckTestZ2(AnsiString sFunc);

const int iPreMovePos=100;                                                      //unit:0.001cm

#pragma package(smart_init)

static char GetGalilAxisChar(unsigned int iPort)
{
    switch(iPort)
    {
        case 0: return 'x';
        case 1: return 'y';
        case 2: return 'z';
        case 3: return 'w';
        default:return 'x';
    }
}
//==============================================================================
__fastcall TMyGALILMotor::TMyGALILMotor(int Addr)
{
    Address=0;
    iBoardID=0;
    iPortID=0;

    if(Addr>=0)
    {
        if(Addr<10)
        {
            iPortID=Addr;
        }
        else
        {
            iBoardID=Addr/10;
            iPortID=Addr%10;
        }
    }

    MotorType=Servo_Motor;
    EncoderType=0;
}
//==============================================================================
TMyGALILMotor::~TMyGALILMotor()
{

}
//==============================================================================
int TMyGALILMotor::InitMotor(int IoAddress)
{
    return true;
}
//==============================================================================
void TMyGALILMotor::SetSpeed(unsigned int x)
{
    iSpeed=x;
}
//==============================================================================
void TMyGALILMotor::SetInitSpeed(unsigned int x)
{
    InitSpeed=x;
}
//==============================================================================
void TMyGALILMotor::SetServoAlarmOn(bool Value)
{
    PServoAlarmOn=Value;
}
//==============================================================================
int TMyGALILMotor::ReadPos()
{
    return ReadRealPos()*GearRatio;
}
//==============================================================================
void TMyGALILMotor::ScanMotorStatus(bool *Led)
{
    if(Led==NULL)
        return;

    Led[iCwLed]=false;
    Led[iHomeLed]=false;
    Led[iCcwLed]=false;
    Led[iEmgLed]=false;
    Led[iSoftcwLed]=false;
    Led[iSoftccwLed]=false;
    Led[iServoalarmLed]=false;
    Led[iServoOn]=Enable;
    Led[iInposLed]=MotionDone();
    Led[iAlarmLed]=GetAlarm();
}
//==============================================================================
bool TMyGALILMotor::MoveToPos(int Tar)
{
    if(!Enable)
        return true;

    if(!bGali_CardInstall)
        return false;

    int Pos=Tar;
    if(Direction==false)
        Pos=-Pos;

    unsigned int Spd=(iSpeed>0)?iSpeed:1;
    int Acc=(dAcc>0)?(int)dAcc:GailAcSpeed2;
    int Dec=(dDec>0)?(int)dDec:GailDcSpeed2;
    char Axis=GetGalilAxisChar(iPortID);

    AnsiString Cmd;
    Cmd.sprintf("SP%c=%u;AC%c=%d;DC%c=%d;PA%c=%d;BG%c;", Axis, Spd, Axis, Acc, Axis, Dec, Axis, Pos, Axis);
    return (DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))==0);
}
//==============================================================================
void TMyGALILMotor::Stop()
{
    if(!Enable || !bGali_CardInstall)
        return;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("ST%c;", Axis);
    DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1));
}
//==============================================================================
void TMyGALILMotor::DecStop()
{
    Stop();
}
//==============================================================================
bool TMyGALILMotor::JogP()
{
    if(!Enable)
        return true;

    if(!bGali_CardInstall)
        return false;

    int Spd=(iSpeed>0)?(int)iSpeed:(int)PJogHighSpeed;
    if(Spd<=0)
        Spd=1;
    if(Direction==false)
        Spd=-Spd;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("SP%c=%d;AC%c=%d;DC%c=%d;JG%c=%d;BG%c;", Axis, abs(Spd), Axis, GailAcSpeed, Axis, GailDcSpeed, Axis, Spd, Axis);
    return (DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))==0);
}
//==============================================================================
bool TMyGALILMotor::JogN()
{
    if(!Enable)
        return true;

    if(!bGali_CardInstall)
        return false;

    int Spd=(iSpeed>0)?(int)iSpeed:(int)PJogHighSpeed;
    if(Spd<=0)
        Spd=1;
    if(Direction)
        Spd=-Spd;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("SP%c=%d;AC%c=%d;DC%c=%d;JG%c=%d;BG%c;", Axis, abs(Spd), Axis, GailAcSpeed, Axis, GailDcSpeed, Axis, Spd, Axis);
    return (DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))==0);
}
//==============================================================================
bool TMyGALILMotor::HomeObject()
{
    if(!Enable)
        return true;

    if(!bGali_CardInstall)
        return false;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("HM%c;BG%c;", Axis, Axis);
    return (DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))==0);
}
//==============================================================================
void TMyGALILMotor::SetRange(unsigned int a)
{
    Range=a;
}
//==============================================================================
void TMyGALILMotor::SetRate(unsigned int a)
{
    Rate=a;
}
//==============================================================================
bool TMyGALILMotor::GetAlarm(void)
{
    if(!Enable || !bGali_CardInstall)
        return false;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("MG_SC%c", Axis);
    if(DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))!=0)
        return false;

    return (atoi(szBuffer1)!=0);
}
//==============================================================================
bool TMyGALILMotor::HomeFlag(void)
{
    return MotionDone();
}
//==============================================================================
bool TMyGALILMotor::ResetPos(int Pulse)
{
    SetCommand(Pulse);
    SetPosition(Pulse);
    return true;
}
//==============================================================================
bool TMyGALILMotor::MotionDone()
{
    if(!Enable || !bGali_CardInstall)
        return true;

    AnsiString Cmd;
    char Axis=GetGalilAxisChar(iPortID);
    Cmd.sprintf("MG_BG%c", Axis);

    if(DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))!=0)
        return true;

    return (atoi(szBuffer1)==0);
}
//==============================================================================
int TMyGALILMotor::ReadRealPos()
{
    if(!Enable || !bGali_CardInstall)
        return 0;

    AnsiString Cmd;
    char Axis=GetGalilAxisChar(iPortID);
    Cmd.sprintf("MG _TP%c", Axis);

    if(DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))!=0)
        return 0;

    return atol(szBuffer1);
}
//==============================================================================
int TMyGALILMotor::ReadEnCoderRealPos()
{
    if(!Enable || !bGali_CardInstall)
        return 0;

    AnsiString Cmd;
    char Axis=GetGalilAxisChar(iPortID);
    Cmd.sprintf("MG _TE%c", Axis);

    if(DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1))!=0)
        return 0;

    return atol(szBuffer1);
}
//==============================================================================
int TMyGALILMotor::SetCommand(int p)
{
    if(!Enable || !bGali_CardInstall)
        return 0;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("DP%c=%d;", Axis, p);
    return DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1));
}
//==============================================================================
int TMyGALILMotor::SetPosition(int p)
{
    if(!Enable || !bGali_CardInstall)
        return 0;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    Cmd.sprintf("DE%c=%d;", Axis, p);
    return DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1));
}
//==============================================================================
void TMyGALILMotor::SetServoOn(bool IsOn)
{
    if(!bGali_CardInstall)
        return;

    char Axis=GetGalilAxisChar(iPortID);
    AnsiString Cmd;
    if(IsOn)
        Cmd.sprintf("SH%c;", Axis);
    else
        Cmd.sprintf("MO%c;", Axis);

    DMCCommand(hDmc, Cmd.c_str(), szBuffer1, sizeof(szBuffer1));
}
//==============================================================================
void TMyGALILMotor::SetSoftLimit(int iPLimit, int iNLimit)
{
    PSoftLimitP=iPLimit;
    PSoftLimitN=iNLimit;
}
//==============================================================================
AnsiString GetGalilErrString(long RC, AnsiString FunctionName)                  //Steven 20151117 : Galil Error Message
{
    AnsiString Str, ErrorString;

    if(RC==DMCERROR_TIMEOUT                             ) ErrorString="DMCERROR_TIMEOUT";
    if(RC==DMCERROR_COMMAND                             ) ErrorString="DMCERROR_COMMAND";
    if(RC==DMCERROR_CONTROLLER                          ) ErrorString="DMCERROR_CONTROLLER";
    if(RC==DMCERROR_FILE                                ) ErrorString="DMCERROR_FILE";
    if(RC==DMCERROR_DRIVER                              ) ErrorString="DMCERROR_DRIVER";
    if(RC==DMCERROR_HANDLE                              ) ErrorString="DMCERROR_HANDLE";
    if(RC==DMCERROR_HMODULE                             ) ErrorString="DMCERROR_HMODULE";
    if(RC==DMCERROR_MEMORY                              ) ErrorString="DMCERROR_MEMORY";
    if(RC==DMCERROR_BUFFERFULL                          ) ErrorString="DMCERROR_BUFFERFULL";
    if(RC==DMCERROR_RESPONSEDATA                        ) ErrorString="DMCERROR_RESPONSEDATA";
    if(RC==DMCERROR_DMA                                 ) ErrorString="DMCERROR_DMA";
    if(RC==DMCERROR_ARGUMENT                            ) ErrorString="DMCERROR_ARGUMENT";
    if(RC==DMCERROR_DATARECORD                          ) ErrorString="DMCERROR_DATARECORD";
    if(RC==DMCERROR_DOWNLOAD                            ) ErrorString="DMCERROR_DOWNLOAD";
    if(RC==DMCERROR_FIRMWARE                            ) ErrorString="DMCERROR_FIRMWARE";
    if(RC==DMCERROR_CONVERSION                          ) ErrorString="DMCERROR_CONVERSION";
    if(RC==DMCERROR_RESOURCE                            ) ErrorString="DMCERROR_RESOURCE";
    if(RC==DMCERROR_REGISTRY                            ) ErrorString="DMCERROR_REGISTRY";
    if(RC==DMCERROR_BUSY                                ) ErrorString="DMCERROR_BUSY";
    if(RC==DMCERROR_DEVICE_DISCONNECTED                 ) ErrorString="DMCERROR_DEVICE_DISCONNECTED";
    if(RC==DMCERROR_TIMEING_ERROR                       ) ErrorString="DMCERROR_TIMEING_ERROR";
    if(RC==DMCERROR_WRITEBUFFER_TOO_LARGE               ) ErrorString="DMCERROR_WRITEBUFFER_TOO_LARGE";
    if(RC==DMCERROR_NO_MODIFY_PNP_CONTROLLER            ) ErrorString="DMCERROR_NO_MODIFY_PNP_CONTROLLER";
    if(RC==DMCERROR_FUNCTION_OBSOLETE                   ) ErrorString="DMCERROR_FUNCTION_OBSOLETE";
    if(RC==DMCERROR_STREAMING_COMMAND_IN_PROGRESS       ) ErrorString="DMCERROR_STREAMING_COMMAND_IN_PROGRESS";
    if(RC==DMCERROR_DEVICEDRIVER_VERSION_TOO_OLD        ) ErrorString="DMCERROR_DEVICEDRIVER_VERSION_TOO_OLD";
    if(RC==DMCERROR_STREAMING_COMMAND_MUST_BE_SOLITARY  ) ErrorString="DMCERROR_STREAMING_COMMAND_MUST_BE_SOLITARY";
    if(RC==DMCERROR_FIRMWARE_VERSION_TOO_OLD            ) ErrorString="DMCERROR_FIRMWARE_VERSION_TOO_OLD";
    if(RC==DMCERROR_ETHERNET_NO_MORE_HANDLES            ) ErrorString="DMCERROR_ETHERNET_NO_MORE_HANDLES";
    if(RC==DMCERROR_NETWORK_UNREACHABLE                 ) ErrorString="DMCERROR_NETWORK_UNREACHABLE";

    Str.sprintf("%s Err RC= %d (%s)", FunctionName, RC, ErrorString);
    return Str;
}
//------------------------------------------------------------------------------
TQPF_Timer tGalilCmdTimer;
long TMyMotor::Gali_Command(AnsiString Data, AnsiString sFunc)
{
    static AnsiString OldData="";
    AnsiString Data1=Data;
    AnsiString Str;
    int iTryCount=0;
    int iErrCnt=0;
    AnsiString sFileName;                                                       //kevin 20150908
    AnsiString sBuffer;
    int iOldrc=0;                                                               //JerryYang 20180411 (jou) : 修改Galil command error log記錄方式, 不要一直重複記錄相同的alarm
    static int iGalilErrCnt=0;

    if(OldData!=Data)                                                           //Steven 20200320 : 紀錄Galil命令
    {
        if(Data=="TSW" || Data=="TSX" || Data=="TSY" || Data=="TSZ" ||          //JerryYang 20241129 : fix Galil log
           Data=="MG_MOW" || Data=="MG_MOX" || Data=="MG_MOY" || Data=="MG_MOZ" ||
           Data=="TI" ||
           Data=="TPW" || Data=="TPX" || Data=="TPY" || Data=="TPZ" ||
           Data=="TDW" || Data=="TDX" || Data=="TDY" || Data=="TDZ" ||
           Data=="SC" ||
           Data=="MG_BGw" || Data=="MG_BGx" || Data=="MG_BGy" || Data=="MG_BGz" ||      //JerryYang 20250826 : fix galil log
           Data=="MG_BGW" || Data=="MG_BGX" || Data=="MG_BGY" || Data=="MG_BGZ" ||
           Data=="MG_SCy" || Data=="MG_SCz")
        {

        }
        else
        {
            QueueGalilCmd.Add(Data, sFunc);                                     //JerryYang 20250826 : fix galil log
        }

        OldData=Data;
    }

    EnterCriticalSection(&g_cs);                                                //ChungHung 20141015 add fix Gail command error
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable && bGali_CardInstall==true)
    {
        while(1)
        {
            #ifndef SOFT_SIMULTE
                rc=DMCCommand(hDmc, Data1.c_str(), szBuffer, sizeof(szBuffer));
            #endif
            if(rc!=0)
            {
                iTryCount++;
                MySleepEx(1, true);
                if(rc==iOldrc)                                                  //JerryYang 20180411 (jou) : 修改Galil command error log記錄方式, 不要一直重複記錄相同的alarm
                {
                }
                else
                {
                    if(iOldrc!=0)
                    {
                        Str=GetGalilErrString(iOldrc, "Gail Command");
                        MyDBIProcessNew("Motion", "WAR2201", "Gail Command", Str+","+AnsiString(Data1)+",ErrCnt:"+IntToStr(iErrCnt)+",Func:"+sFunc);
                        DMCCommand(hDmc, "TC1", szBuffer, sizeof(szBuffer));    /// TC :   要查表對GAIL ALARM    TC1:有詳細說明會被客戶追著打
                        MyDBIProcessNew("Motion", "WAR2202", "Gail TC1", AnsiString(szBuffer)+",ErrCnt:"+IntToStr(iErrCnt));
                        DMCCommand(hDmc, "VS0;SP0,0,0,0;", szBuffer1, sizeof(szBuffer1));   //Steven 20220309 : 避免Galil Command Error時, 不能Alarm
                        SystemStart=false;
                        fAllMotorHome=false;
                        ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
                        iErrCnt=0;
                        return 0;                                               //JerryYang 20240111 : 異常了要return掉
                    }
                    iOldrc=rc;
                }

                if(rc==-1)                                                      //JerryYang 20181101 10分鐘內發生3次error要跳alarm
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis &&
                       Data1.Pos("LMXYZW")==1)                                  //JerryYang 20240111 : 新增Galil四軸保護，四軸同動指令回傳timeout，不要重複送一次指令
                    {
                        MyDBIProcessNew("Motion", "WAR2203", "Gail Command", AnsiString(Data1)+",Func:"+sFunc);
                        DMCCommand(hDmc, "VS0;SP0,0,0,0;", szBuffer1, sizeof(szBuffer1));   //Steven 20220309 : 避免Galil Command Error時, 不能Alarm
                        SystemStart=false;
                        fAllMotorHome=false;
                        ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
                        return 0;
                    }

                    iGalilErrCnt++;
                    if(iGalilErrCnt==1)
                    {
                        tGalilCmdTimer.LatchCycleTimeSec(true);
                    }
                    else if(iGalilErrCnt==3)
                    {
                        iGalilErrCnt=0;
                        if(tGalilCmdTimer.LatchCycleTimeSec()<600)
                        {
                            ShowErrorMessage("WAR0348", 0, MMSystem, 0, "");
                        }
                    }
                }

                iErrCnt++;
                QueueGalilCmd.SafeData();                                       //JerryYang 20250826 : fix galil log  //Steven 20200320 : 紀錄Galil命令
                if(iTryCount<150)                                               //JerryYang 20180508 (jou) : 因index撞機所以修改300 -> 150
                    continue;
            }
            else
            {
                if(iOldrc!=0)
                {
                    Str=GetGalilErrString(iOldrc, "Gail Command");
                    MyDBIProcessNew("Motion", "WAR2201", "Gail Command", Str+","+AnsiString(Data1)+",ErrCnt:"+IntToStr(iErrCnt)+",Func:"+sFunc);
                    DMCCommand(hDmc, "TC1", szBuffer, sizeof(szBuffer));        /// TC :   要查表對GAIL ALARM    TC1:有詳細說明會被客戶追著打
                    MyDBIProcessNew("Motion", "WAR2202", "Gail TC1", AnsiString(szBuffer)+",ErrCnt:"+IntToStr(iErrCnt));
                }
            }

            if(rc!=0)
            {
                SoftStop=true;
                SystemStart=false;
                Str=GetGalilErrString(rc, "Gail Command");
                MyDBIProcessNew("Motion", "WAR2203", "Gail Command Err", AnsiString(Data1)+",ErrCnt:"+IntToStr(iTryCount)+",Func:"+sFunc);
                ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
                fAllMotorHome=false;
                DMCClear(hDmc);                                                 //kevin 20150909
                LeaveCriticalSection(&g_cs);                                    //ChungHung 20141015 add fix Gail command error
                return 0;
            }
            else
            {
                DMCDiagnosticsOff(hDmc);                                        //kevin 20150909 記錄 GAIL通訊
                if(Data=="ST")
                {
                    MOT[MTestY1].MovFlag=false;
                    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                    {
                        MOT[MTestY2].MovFlag=false;
                    }
                    MOT[MTestZ1].MovFlag=false;
                    MOT[MTestZ2].MovFlag=false;

                    MOT[MTestY1].bZ1Z2Exute=false;                              //kevin 20110630
                    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                    {
                        MOT[MTestY2].bZ1Z2Exute=false;                              //kevin 20110630
                    }
                }
                LeaveCriticalSection(&g_cs);                                    //ChungHung 20141015 add fix Gail command error
                return atol(szBuffer);
            }
        }
    }
    else
    {
        if(Data=="ST")
        {
            MOT[MTestY1].MovFlag=false;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                MOT[MTestY2].MovFlag=false;
            }
            MOT[MTestZ1].MovFlag=false;
            MOT[MTestZ2].MovFlag=false;

            MOT[MTestY1].bZ1Z2Exute=false;                                      //kevin 20110630
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                MOT[MTestY2].bZ1Z2Exute=false;                              //kevin 20110630
            }
        }
    }
    LeaveCriticalSection(&g_cs);                                                //ChungHung 20141015 add fix Gail command error
    return 0;
}
//------------------------------------------------------------------------------
void TMyMotor::Gali_ScanMotStatusTIMO()
{
    BYTE TIData;
    char Gali_TiData[]={1, 3, 5, 7};
    int Sel=0;
    AnsiString GetMot;
    GetMot=Gali_GetMOT(Mot_Name);

    if(Mot_Name==MTestY1)
    {
        Sel=0;
    }
    else if(Mot_Name==MTestZ1)
    {
        Sel=1;
    }
    else if(Mot_Name==MTestZ2)
    {
        Sel=2;
    }
    else
    {
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
        {
            return;
        }
        Sel=3;
    }

    TIData=Gali_Command("TI", __FUNC__);                                        //回應電腦input 狀態
    Led[iAlarmLed]=TIData&MyLongMask[Gali_TiData[Sel]];                         //Alarm
    if(Led[iAlarmLed] && MovFlag==true)
    {
        Gali_MotorAlarm=true;
        Alarm->Set(ALM_MOTOR_MOVE);
    }
    else
    {
        Gali_MotorAlarm=false;
    }
    Led[iServoalarmLed]=TIData&MyLongMask[Gali_TiData[Sel]];                    //SAlarm
    if(GaliScanServo)
        Led[iServoOn]=!Gali_Command("MG_MO"+GetMot, __FUNC__);
}
//------------------------------------------------------------------------------
void TMyMotor::Gali_ScanMotStatus()
{
    BYTE Data;
    AnsiString GetMot;

    if(Led[iAlarmLed] && MovFlag==true)
    {
        Gali_MotorAlarm=true;
        Alarm->Set(ALM_MOTOR_MOVE);
    }
    else
    {
        Gali_MotorAlarm=false;
    }

    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable && bGali_CardInstall==true)
    {
        GetMot=Gali_GetMOT(Mot_Name);
        Data=Gali_Command("TS"+GetMot, __FUNC__);

        //---------------------------------------
        Led[iCwLed]         =!(Data&bMask[3]);      //CW
        Led[iHomeLed]       =!(Data&bMask[1]);      //HOME
        Led[iCcwLed]        =!(Data&bMask[2]);      //CCW
        Led[iEmgLed]        =false;                 //EMG.S
        Led[iSoftcwLed]     =false;                 //Soft Cw
        Led[iSoftccwLed]    =false;                 //SoftCCW
        Led[iInposLed]      =Data&bMask[7];
        //---------------------------------------

        iCheckStatusCT++;
        if(iCheckStatusCT<Mot_Name)
          return;
        iCheckStatusCT=0;

        Gali_ScanMotStatusTIMO();
        //---------------------------------------
    }
}
//------------------------------------------------------------------------------
void TMyMotor::Gali_ScanAlarmStatus()
{
    BYTE TIData;
    int Sel=0;
    char Gali_TiData[]={1, 3, 5, 7};
    AnsiString GetMot;

    if(Mot_Name==MTestY1)
        Sel=0;
    else if(Mot_Name==MTestZ1)
        Sel=1;
    else if(Mot_Name==MTestZ2)
        Sel=2;
    else
        Sel=3;

    if(Led[iAlarmLed] && MovFlag==true)
    {
        Gali_MotorAlarm=true;
        Alarm->Set(ALM_MOTOR_MOVE);
    }
    else
    {
        Gali_MotorAlarm=false;
    }

    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable && bGali_CardInstall==true)
    {
        GetMot=Gali_GetMOT(Mot_Name);

        TIData=Gali_Command("TI", __FUNC__);

        Led[iAlarmLed]=TIData&MyLongMask[Gali_TiData[Sel]];                     //Alarm
        if(Led[iAlarmLed] && MovFlag==true)
        {
            Gali_MotorAlarm=true;
            Alarm->Set(ALM_MOTOR_MOVE);
        }
        else
        {
            Gali_MotorAlarm=false;
        }
        Led[iServoalarmLed]=TIData&MyLongMask[Gali_TiData[Sel]];                //SAlarm
        //---------------------------------------
    }
}
//------------------------------------------------------------------------------
AnsiString TMyMotor::Gali_GetMOT(int MOT)
{
    if(MOT==MTestY1)
        return "X";
    else if(MOT==MTestZ1)
        return "Y";
    else if(MOT==MTestZ2)
        return "Z";
    else
        return "W";
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_MotMove2(int Pos, int Speed, int SpeedDec)                  //Steven 20170818 : 測試UPH用
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(Pos==0)
    {
        Pos=0;
    }

    bool flag, checkflag=true;
    char str[256]="";
    AnsiString SelMot;

    SelMot=Gali_GetMOT(Mot_Name);
    int iOrgPos=Pos;
    if(Mot_Name!=MTestY1)
        Pos=-Pos;
    int iReatePos=Pos;
    GetRealPos(&iReatePos);
    double sp=Speed;
    Speed=(int)sp;
    if(Mot_Name==MTestZ1)
    {
        if(IndexZCanMove[0]==false)
            return false;
        IndexZCanMove[1]=false;
    }
    else if(Mot_Name==MTestZ2)
    {
        if(IndexZCanMove[1]==false)
            return false;
        IndexZCanMove[0]=false;
    }

    if(CosFunction.bIndexProtect==true)                                         //Steven 20180319 (jou) : 加入Index Y軸移動前確認Z軸位置保護
    {
        if(CheckYPosWhenZDown(Pos, iOrgPos, AnsiString(__FUNC__))==false)       //jimmychiu 20220815 for index arm checking y pos before z down
            return false;
    }

    if(Motor->Enable && bGali_CardInstall==true)
    {
        Position=Gali_ReadPos();
        if(MovFlag==false)
        {
            #ifdef DEBUG_GALIL_CHECK_Y
            if(Mot_Name==MTestY1 || Mot_Name==MTestY2)                          //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
            {
                if(CheckY1Y2TargetPos(iOrgPos, AnsiString(__FUNC__))==false)
                    return false;
            }
            #endif

            if(Mot_Name==MTestZ1)
                checkflag=CheckTestZ1("Gali_MotMove2");
            else if(Mot_Name==MTestZ2)
                checkflag=CheckTestZ2("Gali_MotMove2");
            else
                checkflag=CheckTestZ("Gali_MotMove2");

            if(checkflag)
            {
                iGali_SpSpeed=Speed;
                sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;PA%s=%d;BG%s;",
                             SelMot, Speed,
                             SelMot, SpeedDec,
                             SelMot, SpeedDec,
                             SelMot, iReatePos,
                             SelMot);

                if(ScanIndexMotorCanMove())
                    Gali_Command(str, __FUNC__);
                else
                    return false;
                MovFlag=true;
                bScanFlag=false;
                Led[iInposLed]=true;
                GaliSofDelayCount=0;
            }
            else
            {
                return false;                                                   //kevin 20141028 add 2014.10.08 , Joye , KYEC Z1 Position
            }
        }
        else
        {
            if(Gali_Command("MG_BG"+SelMot, __FUNC__)==1)                       //馬達目前是否移動 0:移動完成  //ChungHung 20141016 add 確保每次皆到位
                return false;

            if(bScanFlag==false)
                Gali_ScanMotStatus();
            if(Led[iInposLed]==false)
            {
                if(TargetPosition!=Pos)                                         //kevin 20141028 2014.10.08 , Joye , KYEC Z1 Position
                {
                    MovFlag = false;
                    GaliSofDelayCount = 0;
                    return false;
                }
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(Mot_Name==MTestY1 || Mot_Name==MTestY2 || Pos==0)
                        flag=Gali_ReadEncoderInRandge(Pos);
                    else
                        flag=Gali_ReadEncoderMaxRandge(Pos);

                    if(flag==false)
                    {
                        iHome=1;
                        if(GaliAxisAlarm[0])
                            JamCode = MotorIndexToJamCode(13);
                        else if(GaliAxisAlarm[1])
                            JamCode = MotorIndexToJamCode(14);
                        else if(GaliAxisAlarm[2])
                            JamCode = MotorIndexToJamCode(15);
                        else  //if(GaliAxisAlarm[3])
                            JamCode = MotorIndexToJamCode(16);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1);            //Sam 20200903 : 補 Alarm
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    bScanFlag=false;
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
        }
    }
    else
    {
        if(Mot_Name==MTestY2)
            Pos=-Pos;
        if(Position==Pos)
        {
            if(Mot_Name==MTestZ1)
                IndexZCanMove[1]=true;
            else if(Mot_Name==MTestZ2)
                IndexZCanMove[0]=true;

            return true;
        }
        else
        {
            if(Position>Pos)
            {
                Position-=speed;
                if(Position<=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
            else
            {
                Position+=speed;
                if(Position>=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
        }
    }
    TargetPosition=Pos;
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_MotMove(int Pos, int Speed, AnsiString _Func)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis && Mot_Name==MTestY2)                //JimmyChiu 20220708 : add Index Arm Axis
    {
        return true;
    }

    if(Pos==0)
    {
        Pos=0;
    }

    bool flag=false,checkflag=true;
    char str[256]="";
    AnsiString SelMot=AnsiString("");
    AnsiString sFunc=AnsiString(__FUNC__)+AnsiString("_")+_Func;

    SelMot=Gali_GetMOT(Mot_Name);
    int iOrgPos=Pos;
    if(Mot_Name!=MTestY1)
        Pos=-Pos;
    int iReatePos=Pos;
    GetRealPos(&iReatePos);
    double sp=Speed;
    Speed=(int)sp;
    if(Mot_Name==MTestZ1)
    {
        if(IndexZCanMove[0]==false)
            return false;
        IndexZCanMove[1]=false;
    }
    else if(Mot_Name==MTestZ2)
    {
        if(IndexZCanMove[1]==false)
            return false;
        IndexZCanMove[0]=false;
    }

    if(CosFunction.bIndexProtect==true)                                         //Steven 20180319 (jou) : 加入Index Y軸移動前確認Z軸位置保護
    {
        if(CheckYPosWhenZDown(Pos, iOrgPos, sFunc)==false)                      //jimmychiu 20220815 for index arm checking y pos before z down
            return false;
    }

    if(Motor->Enable && bGali_CardInstall==true)
    {
        Position=Gali_ReadPos();
        if(MovFlag==false)
        {
            #ifdef DEBUG_GALIL_CHECK_Y
            if(Mot_Name==MTestY1 || Mot_Name==MTestY2)                          //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
            {
                if(CheckY1Y2TargetPos(iOrgPos, sFunc)==false)
                    return false;
            }
            #endif

            if(Mot_Name==MTestZ1)
                checkflag=CheckTestZ1(sFunc);
            else if(Mot_Name==MTestZ2)
                checkflag=CheckTestZ2(sFunc);
            else
                checkflag=CheckTestZ(sFunc);

            if(checkflag)
            {
                iGali_SpSpeed=Speed;
                sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;PA%s=%d;BG%s;",
                              SelMot, Speed,
                              SelMot, GailAcSpeed2,
                              SelMot, GailDcSpeed2,
                              SelMot, iReatePos,
                              SelMot);

                if(ScanIndexMotorCanMove())
                    Gali_Command(str, sFunc);
                else
                    return false;
                MovFlag=true;
                bScanFlag=false;
                Led[iInposLed]=true;
                GaliSofDelayCount=0;
            }
            else
            {
                return false;                                                   //kevin 20141028 add 2014.10.08 , Joye , KYEC Z1 Position
            }
        }
        else
        {
            if(Gali_Command("MG_BG"+SelMot, sFunc)==1)                          //馬達目前是否移動 0:移動完成  //ChungHung 20141016 add 確保每次皆到位
                return false;

            if(bScanFlag==false)
                Gali_ScanMotStatus();
            if(Led[iInposLed]==false)
            {
                if(TargetPosition!=Pos)                                         //kevin 20141028 2014.10.08 , Joye , KYEC Z1 Position
                {
                    MovFlag = false;
                    GaliSofDelayCount = 0;
                    return false;
                }
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(Mot_Name==MTestY1 || Mot_Name==MTestY2 || Pos==0)
                        flag=Gali_ReadEncoderInRandge(Pos);
                    else
                        flag=Gali_ReadEncoderMaxRandge(Pos);

                    if(flag==false)
                    {
                        iHome=1;
                        if(GaliAxisAlarm[0])
                            JamCode = MotorIndexToJamCode(13);
                        else if(GaliAxisAlarm[1])
                            JamCode = MotorIndexToJamCode(14);
                        else if(GaliAxisAlarm[2])
                            JamCode = MotorIndexToJamCode(15);
                        else  //if(GaliAxisAlarm[3])
                            JamCode = MotorIndexToJamCode(16);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1, sFunc);     //Sam 20200903 : 補 Alarm
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    bScanFlag=false;
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
        }
    }
    else
    {
        if(Mot_Name==MTestY2)
            Pos=-Pos;
        if(Position==Pos)
        {
            if(Mot_Name==MTestZ1)
                IndexZCanMove[1]=true;
            else if(Mot_Name==MTestZ2)
                IndexZCanMove[0]=true;

            return true;
        }
        else
        {
            if(Position>Pos)
            {
                Position-=speed;
                if(Position<=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
            else
            {
                Position+=speed;
                if(Position>=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    return true;
                }
            }
        }
    }
    TargetPosition=Pos;
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_MotMoveNoWait(int Pos, int Speed, int iNeedDelayTime, bool bCheckZ)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis && Mot_Name==MTestY2)                //JimmyChiu 20220708 : add Index Arm Axis
    {
        return true;
    }

    if(Pos==0)
    {
        Pos=0;
    }
    bool checkflag=true;
    char str[256]="";
    AnsiString SelMot=AnsiString("");

    SelMot=Gali_GetMOT(Mot_Name);
    int iOrgPos=Pos;
    if(Mot_Name!=MTestY1)
        Pos=-Pos;
    double sp=Speed;
    int iReatePos=Pos;
    GetRealPos(&iReatePos);
    Speed=(int)sp;
    if(Mot_Name==MTestZ1)
    {
        if(IndexZCanMove[0]==false)
            return false;
        IndexZCanMove[1]=false;
    }
    else if(Mot_Name==MTestZ2)
    {
        if(IndexZCanMove[1]==false)
            return false;
        IndexZCanMove[0]=false;
    }

    if(CheckYPosWhenZDown(Pos, iOrgPos, AnsiString(__FUNC__))==false)           //jimmychiu 20220815 for index arm checking y pos before z down
        return false;

    if(Motor->Enable && bGali_CardInstall==true)
    {
        Position=Gali_ReadPos();
        if(MovFlag==false)
        {
            #ifdef DEBUG_GALIL_CHECK_Y
            if(Mot_Name==MTestY1 || Mot_Name==MTestY2)                          //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
            {
                if(CheckY1Y2TargetPos(iOrgPos, AnsiString(__FUNC__))==false)
                    return false;
            }
            #endif

            if(Mot_Name==MTestZ1)
                checkflag=CheckTestZ1("Gali_MotMoveNoWait");
            else if(Mot_Name==MTestZ2)
                checkflag=CheckTestZ2("Gali_MotMoveNoWait");
            else
                checkflag=CheckTestZ("Gali_MotMoveNoWait");

            if(checkflag)
            {
                iGali_SpSpeed=Speed;
               if(ScanIndexMotorCanMove())
                {
                    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;PA%s=%d;BG%s;",
                                  SelMot, Speed,
                                  SelMot, GailAcSpeed2,
                                  SelMot, GailDcSpeed2,
                                  SelMot, iReatePos,
                                  SelMot);
                    Gali_Command(str, __FUNC__);
                }
                else
                {
                    return false;
                }

                bScanFlag=false;
                MovFlag=true;
                Led[iInposLed]=true;
                GaliSofDelayCount=0;
            }
        }
        else
        {
            if(Gali_Command("MG_BG"+SelMot, __FUNC__)==1)                       //馬達目前是否移動 0:移動完成  //ChungHung 20141016 add 確保每次皆到位
                return false;

            if(bScanFlag==false)
                Gali_ScanMotStatus();
            if(Led[iInposLed]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=iNeedDelayTime)
                {
                    bScanFlag=false;
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;
                    MySleep(100);
                    ScanIndexOtherMotorCanMove(Mot_Name);
                    return true;
                }
            }
        }
    }
    else
    {
        if(Mot_Name==MTestY2)
            Pos=-Pos;
        if(Position==Pos)
        {
            if(Mot_Name==MTestZ1)
                IndexZCanMove[1]=true;
            else if(Mot_Name==MTestZ2)
                IndexZCanMove[0]=true;

            return true;
        }
        else
        {
            if(Position>Pos)
            {
                Position-=speed;
                if(Position<=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;

                    return true;
                }
            }
            else
            {
                Position+=speed;
                if(Position>=Pos)
                {
                    Position=Pos;
                    if(Mot_Name==MTestZ1)
                        IndexZCanMove[1]=true;
                    else if(Mot_Name==MTestZ2)
                        IndexZCanMove[0]=true;

                    return true;
                }
            }
        }
    }
    TargetPosition=Pos;
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_MotMoveSkipEncoder(int Pos, int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    char str[256]="";
    AnsiString SelMot=AnsiString("");
    SelMot=Gali_GetMOT(Mot_Name);
    if(Mot_Name!=MTestY1)
        Pos=-Pos;
    double sp=Speed;
    int iReatePos=Pos;
    GetRealPos(&iReatePos);
    sp*=0.6;
    Speed=(int)sp;
    if(Motor->Enable && bGali_CardInstall==true)
    {
        if(MovFlag==false)
        {
            iGali_SpSpeed=Speed;
            sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;PA%s=%d;BG%s;",
                          SelMot, Speed,
                          SelMot, GailAcSpeed2,
                          SelMot, GailDcSpeed2,
                          SelMot, iReatePos,
                          SelMot);
            if(ScanIndexMotorCanMove())
            {
                Gali_Command(str, __FUNC__);
            }
            else
            {
                return false;
            }

            MovFlag=true;
        }
        else
        {
            if(Gali_Command("MG_BG"+SelMot, __FUNC__)==1)                       //馬達目前是否移動 0:移動完成  //ChungHung 20141016 add 確保每次皆到位
                return false;

            ScanIndexOtherMotorCanMove(Mot_Name);
            if(bScanFlag==false)
                Gali_ScanMotStatus();
            if(Led[iInposLed]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    bScanFlag=false;
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    return true;
                }
            }
        }
    }
    else
    {
        if(Position==Pos)
        {
            return true;
        }
        else
        {
            if(Position>Pos)
            {
                Position-=speed;
                if(Position<=Pos)
                {
                    Position=Pos;
                    return true;
                }
            }
            else
            {
                Position+=speed;
                if(Position>=Pos)
                {
                    Position=Pos;
                    return true;
                }
            }
        }
    }
    TargetPosition=Pos;
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1UpZ2Down1(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    char str[512]="";
    char tmpstr[512]="";
    AnsiString str2="";
    static int iCount=0;                                                        //kevin 20220221 add Gali  Command state  Mylin 20211228 Add Gali Command Log
    static bool MOTMovflag[]={true,true,true,true};
    int iEncoderZ1=0, iEncoderY1=0;
    int iEncoderZ2=0, iEncoderY2=0;

    GetIndexParm();
    if(Motor->Enable && bGali_CardInstall==true)
    {
        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
            }
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ1("Z1UpZ2Down1"))
            {
                iGali_VsSpeed=Speed;
                TestYBuffer=XShiftF;
                TestY2Buffer=-XShiftR;
                MOT[MTestZ1].GetRealPos(&Z1Safe);

                #ifdef DEBUG_GALIL_CHECK_Y
                if(Check_Y1Y2_TargetPosWillCrash(TestYBuffer, -TestY2Buffer))   //JerryYang 20250822 : fix index y保護  //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
                {
                    str2.sprintf("Index Y Position Calculate error, %s",__FUNC__);
                    ShowMyMessage(str2);
                    fAllMotorHome=false;
                    return false;
                }
                #endif

                MOT[MTestY1].GetRealPos(&TestYBuffer);
                MOT[MTestZ1].GetRealPos(&Z1Up);
                MOT[MTestZ2].GetRealPos(&Z2DownSafe);

                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                         //JimmyChiu 20220708 : add Index Arm Axis
                {
                    MOT[MTestY2].GetRealPos(&TestY2Buffer);
                    MOT[MTestY2].GetRealPos(&AxisY_Pre_MovePos);                //2013-01-15    Dell   提高Index 運動速度(目前by9046LS專用)
                }
                else//IndexArm_3_Axis
                {
                    //
                }

                MOT[MTestZ2].GetRealPos(&Z2Down);
                #ifdef DEBUG_GALIL
                MOT[MTestZ2].GetRealPos(&Z2Down2Speed);
                #endif

                if(IniConfig.bD38IndexPutICToShtNoWaitMotion &&                 //Steven 20181228 : Add Index Action
                   bZ1UpAndPlaceZ2Down==true                 &&
                   IniConfig.bIndexArm2SupplyLight==false    &&
                   TestIF_File.bForEgisTecTest==false        &&
                   TestIF_File.bArm1PickPlaceArm2Test==false)
                {
                    sprintf(str,"LMXYZW;"
                                "LI0,%d,0,0;"
                                "LI%d,%d,%d,%d;"
                                "LI0,%d,%d,0;"
                                "LE;"
                                "VS%d;VA%d;VD%d;BGS;",
                                -Z1Safe,
                                TestYBuffer, -Z1Up, -Z2DownSafe, TestY2Buffer,
                                -Z1DownToShuttle, -Z2Down,
                                Speed, GailAcSpeed, GailDcSpeed);
                }
                else                                                            ////20111114  Dell for Disable Index Arm    Start
                {
                    #ifdef DEBUG_GALIL
                    sprintf(str, "LMXYZW;"
                                 "LI0,%d,0,0;"
                                 "LI%d,%d,%d,%d;"
                                 "LI0,0,%d,0 >5000;"
                                 "LI0,0,%d,0 <5000;"
                                 "LE;"
                                 "VS%d;VA%d;VD%d;BGS;",
                                 -Z1Safe,
                                 TestYBuffer, -Z1Up, -Z2DownSafe, TestY2Buffer,
                                 -Z2Down,
                                 Z2Down2Speed,
                                 Speed, GailAcSpeed, GailDcSpeed);
                    #else
                    if(TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false &&
                       TestIF_File.iShuttle_Sel==1 && Z1Safe==0)                //jou 2014-10-23 增加index arm轉換保護
                    {
                        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
                        {
                            sprintf(tmpstr, "LMXYZ;"
                                            "LI%d,%d,%d;",
                                            TestYBuffer, -Z1Up, -Z2DownSafe);
                        }
                        else
                        {
                            sprintf(tmpstr, "LMXYZW;"
                                            "LI%d,%d,%d,%d;",
                                            TestYBuffer, -Z1Up, -Z2DownSafe, TestY2Buffer);
                        }
                    }
                    else
                    {
                        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
                        {
                            sprintf(tmpstr, "LMXYZ;"
                                            "LI0,%d,0;"
                                            "LI%d,%d,%d;",
                                            -Z1Safe,
                                            TestYBuffer, -Z1Up, -Z2DownSafe);
                        }
                        else if(MachineTypeChoice==Type_HT9046_LS &&            //KenHsieh 20230508 : 修正關Arm之Arm偷跑問題
                                TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false)
                        {
                            sprintf(tmpstr, "LMXYZW;"
                                            "LI0,%d,0,0;"
                                            "LI%d,%d,%d,%d;",
                                            -Z1Safe,
                                            TestYBuffer, -Z1Up, -Z2DownSafe, TestY2Buffer);
                        }
                        else if(REAL_TIME_CCD && COM2->bCCDDummyRum==false && TestIF_File.bUseRTCStepAsideMode) //Ifor 20230908 add:RTC 讓位功能 By Setup File
                        {
                            sprintf(tmpstr, "LMXYZW;"
                                            "LI0,%d,0,0;"
                                            "LI%d,%d,%d,%d;"
                                            "LI%d,%d,%d,%d;",
                                            -Z1Safe,
                                            TestYBuffer, -Z1Up, 0, 0,
                                            0, 0, -Z2DownSafe, TestY2Buffer);
                        }
                        else if(MachineTypeChoice==Type_HT9046_LS &&            //jou 2014-10-23 增加index arm轉換保護
                                bGail_HighSpeedMode==true)
                        {

                            sprintf(tmpstr, "LMXYZW;"
                                            "LI0,%d,0,%d;"
                                            "LI%d,%d,%d,%d<%d>%d;",
                                            -Z1Safe, AxisY_Pre_MovePos,
                                            TestYBuffer, -Z1Up, -Z2DownSafe, (TestY2Buffer-AxisY_Pre_MovePos), MOT[MTestY1].GailSpeed, Speed);
                        }
                        else
                        {
                            sprintf(tmpstr, "LMXYZW;"
                                            "LI0,%d,0,0;"
                                            "LI%d,%d,%d,%d;",
                                            -Z1Safe,
                                            TestYBuffer, -Z1Up, -Z2DownSafe, TestY2Buffer);
                        }
                    }

                    if(TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false &&
                       Z2Down==0)                                               //jou 2014-10-23 增加index arm轉換保護
                    {
                         sprintf(str, "%sLE;VS%d;VA%d;VD%d;BGS;", tmpstr, Speed, GailAcSpeed, GailDcSpeed);
                    }
                    else
                    {
                         sprintf(str, "%sLI0,0,%d,0;LE;VS%d;VA%d;VD%d;BGS;", tmpstr, -Z2Down, Speed, GailAcSpeed, GailDcSpeed);
                    }
                    #endif
                }

                bScanFlag=false;
                if(ScanIndexVectorCanMove(str))
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[iInposLed]=true;
                }
                MovFlag=true;
            }
        }
        else
        {

            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                iEncoderZ2=MOT[MTestZ2].Gali_ReadEncoderPos();                  //JerryYang 20251124 : add portect, 避免index arm撞機
                iEncoderY2=MOT[MTestY2].Gali_ReadEncoderPos();
                iEncoderZ1=MOT[MTestZ1].Gali_ReadEncoderPos();
                iEncoderY1=MOT[MTestY1].Gali_ReadEncoderPos();

                if(iEncoderZ2<Prod.All_TestZ_Test_Safe-iCheckZ)                 //JerryYang 20260114 : 輕壓的速度比較快, 放寬range   //JerryYang 20251224 : 3000->4000
                {
                    if(CheckArmPosArrival(iEncoderY2, Prod.TestY2_Middle, IniConfig.GaliPosRange)==false)
                    {
                        ShowIndexMotorError(AnsiString("Z1UpZ2Down1"));
                    }
                }

                if(iEncoderZ2<-4000 && iEncoderY2>-9000)                        //JerryYang 20260114 : 檢查下降過程是否會尻到SHUTTLE
                {
                    ShowIndexMotorError(AnsiString("Z1UpZ2Down1_TEST"));
                }

                if(iEncoderZ1<-4000 && iEncoderY1<9000)                             //JerryYang 20260114 : 檢查下降過程是否會尻到SHUTTLE
                {
                    ShowIndexMotorError(AnsiString("Z1DownZ2Up1_TEST2"));
                }
            }


            if(Gali_Command("SC")==100)                                         //ChungHung 20150831 add 確保每次皆到位
            {
                iCount++;
                if(iCount>5000)
                {
                    fMain->sb_Gali_LogClick();                                  //Mylin 20211228 Add Gali Command Log
                    iCount=0;
                    iHome =1;
                    ShowMyMessage("Gali Command Error When Z1UpZ2Down1, Please Home and Start", "請更換Galil 轉板或100P線材");
                }
                return false;
            }
            iCount=0;                                                           //kevin 20220221 add Gali  Command state Mylin 20211228 Add Gali Command Log

            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                   //JimmyChiu 20220708 : add Index Arm Axis
                       (MTestY1+i)==MTestY2)
                    {
                        MOTMovflag[i]=false;
                    }
                    else
                    {
                        if(MOTMovflag[i])
                        {
                            MOT[MTestY1+i].Gali_ScanMotStatus();
                            MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                        }
                    }
                }
            }

            if(MOTMovflag[0]==false && MOTMovflag[1]==false && MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos(true)==false)                                   //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode=MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Up_Z2Down())
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1Up_Z2Down;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode=MotorIndexToJamCode(14);
                        else
                            JamCode=MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode, MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ2].Position+=Speed;
        MOT[MTestZ1].Position-=Speed;
        if(MOT[MTestZ2].Position<=0)
            MOT[MTestZ2].Position=0;
        MOT[MTestY1].Position-=Speed;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
        {
            MOT[MTestY2].Position-=Speed;
        }

        MOT[MTestY1].TargetPosition=Prod.TestY1_Front;                          //Steven 20210827 : 修改顯示Galil的Target / Current
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
        {
            MOT[MTestY2].TargetPosition=Prod.TestY2_Middle;
        }
        MOT[MTestZ1].TargetPosition=0;
        MOT[MTestZ2].TargetPosition=-Prod.TestZ2_Test;

        if(MOT[MTestZ1].Position<=0 || MOT[MTestY1].Position<=Prod.TestY1_Front)
        {
            IndexStatus=Z1Up_Z2Down;
            MOT[MTestY1].Position=Prod.TestY1_Front;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                MOT[MTestY2].Position=Prod.TestY2_Middle;
            }
            MOT[MTestZ1].Position=0;
            MOT[MTestZ2].Position=-Prod.TestZ2_Test;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1DownZ2Up1(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    char str[512]="";
    char tmpstr[512]="";
    AnsiString str2="";

    static int iCount=0;                                                        //kevin 20220221 add Gali  Command state  Mylin 20211228 Add Gali Command Log
    static bool MOTMovflag[]={true, true, true, true};
    int iEncoderZ1=0, iEncoderY1=0;
    int iEncoderZ2=0, iEncoderY2=0;

    GetIndexParm();                                                             //取得移動距離

    if(Motor->Enable && bGali_CardInstall==true)
    {
        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
            {
                MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
            }
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ2("Z1DownZ2Up1"))                                      //確認四軸command & encoder位置是否不同
            {
                iGali_VsSpeed=Speed;                                            //?與 Z1UpZ2Down 不同?
                MOT[MTestZ2].GetRealPos(&Z2Safe);

                #ifdef DEBUG_GALIL_CHECK_Y
                if(Check_Y1Y2_TargetPosWillCrash(-XShiftF, -XShiftR))       //JerryYang 20250822 : fix index y保護  //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
                {
                    str2.sprintf("Index Y Position Calculate error, %s",__FUNC__);
                    ShowMyMessage(str2);
                    fAllMotorHome=false;
                    return false;
                }
                #endif

                MOT[MTestY1].GetRealPos(&XShiftF);
                MOT[MTestZ1].GetRealPos(&Z1DownSafe);
                MOT[MTestZ2].GetRealPos(&Z2Up);
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                         //JimmyChiu 20220708 : add Index Arm Axis
                {
                    MOT[MTestY2].GetRealPos(&XShiftR);
                }
                MOT[MTestZ1].GetRealPos(&Z1Down);
                #ifdef DEBUG_GALIL
                MOT[MTestZ1].GetRealPos(&Z1Down2Speed);
                #endif
                MOT[MTestY1].GetRealPos(&AxisY_Pre_MovePos);                    //2013-01-15    Dell   提高Index 運動速度(目前by9046LS專用)

                if(IniConfig.bD38IndexPutICToShtNoWaitMotion &&
                   bZ1DownZ2UpAndPlace==true                 &&
                   IniConfig.bIndexArm2SupplyLight==false    &&
                   TestIF_File.bForEgisTecTest==false        &&
                   TestIF_File.bArm1PickPlaceArm2Test==false)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                     //JimmyChiu 20220708 : add Index Arm Axis
                    {
                        sprintf(str,"LMXYZ;"
                                "LI0,0,%d;"
                                "LI%d,%d,%d;"
                                "LI0,%d,%d;"
                                "LE;VS%d;VA%d;VD%d;BGS;",
                                -Z2Safe,
                                -XShiftF, -Z1DownSafe, -Z2Up,
                                -Z1Down, -Z2DownToShuttle,
                                Speed, GailAcSpeed, GailDcSpeed);
                    }
                    else
                    {
                        sprintf(str,"LMXYZW;"
                                "LI0,0,%d,0;"
                                "LI%d,%d,%d,%d;"
                                "LI0,%d,%d,0;"
                                "LE;VS%d;VA%d;VD%d;BGS;",
                                -Z2Safe,
                                -XShiftF, -Z1DownSafe, -Z2Up, XShiftR,
                                -Z1Down, -Z2DownToShuttle,
                                Speed, GailAcSpeed, GailDcSpeed);
                    }
                }
                else                                                            ////20111114  Dell for Disable Index Arm    Start
                {
                    #ifdef DEBUG_GALIL
                    sprintf(str, "LMXYZW;"
                                 "LI0,0,%d,0;"
                                 "LI%d,%d,%d,%d;"
                                 "LI0,%d,0,0 >5000;"
                                 "LI0,%d,0,0 <5000;"
                                 "LE;"
                                 "VS%d;VA%d;VD%d;BGS;",
                                 -Z2Safe,
                                 -XShiftF, -Z1DownSafe, -Z2Up, XShiftR,
                                 -Z1Down,
                                 Z1Down2Speed,
                                 Speed, GailAcSpeed, GailDcSpeed);
                    #else
                    if(TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false &&
                       TestIF_File.iShuttle_Sel==0 && Z2Safe==0)                //jou 2014-10-23 增加index arm轉換保護
                    {
                        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
                            sprintf(tmpstr, "LMXYZ;LI%d,%d,%d;",
                                            -XShiftF, -Z1DownSafe, -Z2Up);
                        else
                            sprintf(tmpstr, "LMXYZW;LI%d,%d,%d,%d;",
                                            -XShiftF, -Z1DownSafe, -Z2Up, XShiftR);
                    }
                    else
                    {
                        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
                        {
                            sprintf(tmpstr, "LMXYZ;"
                                            "LI0,0,%d;"
                                            "LI%d,%d,%d;",
                                            -Z2Safe,
                                            (-XShiftF), -Z1DownSafe, -Z2Up);
                        }
                        else if(MachineTypeChoice==Type_HT9046_LS &&            //KenHsieh 20230508 : 修正關Arm之Arm偷跑問題
                                TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false)
                        {
                            sprintf(tmpstr, "LMXYZW;LI0,0,%d,0;LI%d,%d,%d,%d;",
                                            -Z2Safe, -XShiftF, -Z1DownSafe, -Z2Up, XShiftR);
                        }
                        else if(REAL_TIME_CCD && COM2->bCCDDummyRum==false && TestIF_File.bUseRTCStepAsideMode) //Ifor 20230908 add:RTC 讓位功能 By Setup File
                        {
                            sprintf(tmpstr, "LMXYZW;"
                                            "LI0,0,%d,0;"
                                            "LI%d,%d,%d,%d;"
                                            "LI%d,%d,%d,%d;",
                                            -Z2Safe,
                                            0, 0, -Z2Up, XShiftR,
                                            -XShiftF, -Z1DownSafe, 0, 0);
                        }
                        else if(MachineTypeChoice==Type_HT9046_LS &&
                                bGail_HighSpeedMode==true)                      //2013-01-15    Dell   提高Index 運動速度(目前by9046LS專用)    //jou 2014-10-23 增加index arm轉換保護
                        {
                            sprintf(tmpstr, "LMXYZW;LI%d,0,%d,0;LI%d,%d,%d,%d<%d>%d;",
                                            AxisY_Pre_MovePos, -Z2Safe, (-XShiftF-AxisY_Pre_MovePos), -Z1DownSafe, -Z2Up, XShiftR, MOT[MTestY1].GailSpeed, Speed);
                        }
                        else
                        {
                            sprintf(tmpstr, "LMXYZW;LI0,0,%d,0;LI%d,%d,%d,%d;",
                                            -Z2Safe, -XShiftF, -Z1DownSafe, -Z2Up, XShiftR);
                        }
                    }

                    if(TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false &&
                       Z1Down==0)                                               //jou 2014-10-23 增加index arm轉換保護
                    {
                        sprintf(str, "%sLE;VS%d;VA%d;VD%d;BGS;",
                                     tmpstr, Speed, GailAcSpeed, GailDcSpeed);
                    }
                    else
                    {
                        sprintf(str, "%sLI0,%d,0,0;LE;VS%d;VA%d;VD%d;BGS;",
                                     tmpstr, -Z1Down, Speed, GailAcSpeed, GailDcSpeed);
                    }
                    #endif
                }

                bScanFlag=false;
                if(ScanIndexVectorCanMove(str))
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                MovFlag=true;
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[iInposLed]=true;
                }
            }
        }
        else
        {

            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            {
                iEncoderZ1=MOT[MTestZ1].Gali_ReadEncoderPos();                  //JerryYang 20251124 : add portect, 避免index arm撞機
                iEncoderY1=MOT[MTestY1].Gali_ReadEncoderPos();
                iEncoderZ2=MOT[MTestZ2].Gali_ReadEncoderPos();
                iEncoderY2=MOT[MTestY2].Gali_ReadEncoderPos();

                if(iEncoderZ1<Prod.All_TestZ_Test_Safe-iCheckZ)                 //JerryYang 20260114 : 輕壓的速度比較快, 放寬range   //JerryYang 20251224 : 3000->4000
                {
                    if(CheckArmPosArrival(iEncoderY1, Prod.TestY1_Middle, IniConfig.GaliPosRange)==false)
                    {
                        ShowIndexMotorError(AnsiString("Z1DownZ2Up1"));
                    }
                }

                if(iEncoderZ1<-4000 && iEncoderY1<9000)                         //JerryYang 20260114 : 檢查下降過程是否會尻到SHUTTLE
                {
                    ShowIndexMotorError(AnsiString("Z1DownZ2Up1_TEST"));
                }

                if(iEncoderZ2<-4000 && iEncoderY2>-9000)                            //JerryYang 20260114 : 檢查下降過程是否會尻到SHUTTLE
                {
                    ShowIndexMotorError(AnsiString("Z1UpZ2Down1_TEST2"));
                }
            }

            if(Gali_Command("SC")==100)                                         //ChungHung 20150831 add 確保每次皆到位
            {
                iCount++;
                if(iCount>5000)
                {
                    fMain->sb_Gali_LogClick();                                  //Mylin 20211228 Add Gali Command Log
                    iCount=0;
                    iHome =1;
                    ShowMyMessage("Gali Command Error When Z1UpZ2Down1, Please Home and Start", "請更換Galil 轉板或100P線材");
                }
                return false;
            }
            iCount=0;                                                           //kevin 20220221 add Gali  Command state Mylin 20211228 Add Gali Command Log

            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                   //JimmyChiu 20220708 : add Index Arm Axis
                       (MTestY1+i)==MTestY2)
                    {
                        MOTMovflag[i]=false;
                    }
                    else
                    {
                        if(MOTMovflag[i])
                        {
                            MOT[MTestY1+i].Gali_ScanMotStatus();
                            MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                        }
                    }
                }
            }
            if(MOTMovflag[0]==false && MOTMovflag[1]==false && MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos(false)==false)                                  //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode = MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode,MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Down_Z2Up())                                         //check z1 & z2 encoder position
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1Down_Z2Up;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode=MotorIndexToJamCode(14);
                        else
                            JamCode=MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode,MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ2].Position-=Speed;
        MOT[MTestZ1].Position+=Speed;
        if(MOT[MTestZ2].Position<=0)
            MOT[MTestZ2].Position=0;
        MOT[MTestY1].Position+=Speed;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
        {
            MOT[MTestY2].Position+=Speed;
        }

        MOT[MTestY1].TargetPosition=Prod.TestY1_Middle;                         //Steven 20210827 : 修改顯示Galil的Target / Current
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            MOT[MTestY2].TargetPosition=Prod.TestY2_Rear;
        MOT[MTestZ1].TargetPosition=-Prod.TestZ1_Test;
        MOT[MTestZ2].TargetPosition=0;

        bool bY2PosSafe=(USE_INDEX_ARM_AXES==IndexArm_4_Axis)?(MOT[MTestY2].Position>=Prod.TestY2_Rear):true;  //JimmyChiu 20220708 : add Index Arm Axis
        if(MOT[MTestZ2].Position<=0 || bY2PosSafe)
        {
            IndexStatus=Z1Down_Z2Up;
            MOT[MTestY1].Position=Prod.TestY1_Middle;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
                MOT[MTestY2].Position=Prod.TestY2_Rear;
            MOT[MTestZ1].Position=-Prod.TestZ1_Test;
            MOT[MTestZ2].Position=0;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1UpZ2Down2(int Speed, bool bPickErr)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    static bool MOTMovflag[]={true, true, true, true};
    static bool bMOTMoveFinish[]={false, false, false, false};

    GetIndexParm();
    if(Motor->Enable && bGali_CardInstall==true)
    {
        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
                MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ1("Z1UpZ2Down2"))
            {
                bScanFlag=false;
                if(ScanIndexMotorCanMove())
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[iInposLed]=true;
                    bMOTMoveFinish[i]=false;
                }
            }
        }
        else if(!bMOTMoveFinish[0] || !bMOTMoveFinish[1] ||
                !bMOTMoveFinish[2] || !bMOTMoveFinish[3])                       //Sam 202400510 : T Movw 補 RTC 讓位功能
        {
            if(REAL_TIME_CCD && COM2->bCCDDummyRum==false &&
               TestIF_File.bUseRTCStepAsideMode)                                //Sam 202400510 : T Move 補 RTC 讓位功能
            {
                if(!bMOTMoveFinish[0])
                {
                    bMOTMoveFinish[0]=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, Speed*T_MODE_SPEED, "Z1UpZ2Down2_1");
                }
                else if(!bMOTMoveFinish[1])
                {
                    bMOTMoveFinish[1]=MOT[MTestY1].Gali_MotMove(Prod.TestY1_Front, Speed, "Z1UpZ2Down2_2");
                }
                else if(!bMOTMoveFinish[2])
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                     //JimmyChiu 20220708 : add Index Arm Axis
                    {
                        bMOTMoveFinish[2]=MOT[MTestY1].Gali_MotMove(Prod.TestY1_Front, Speed, "Z1UpZ2Down2_3");
                    }
                    else
                    {
                        bMOTMoveFinish[2]=MOT[MTestY2].Gali_MotMove(Prod.TestY2_Middle, Speed, "Z1UpZ2Down2_4");
                    }
                }
                else if(!bMOTMoveFinish[3])
                {
                    bMOTMoveFinish[3]=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, Speed*T_MODE_SPEED, "Z1UpZ2Down2_5");
                }
            }
            else
            {
                bMOTMoveFinish[3]=true;                                         //Sam 202400510 : T Move 補 RTC 讓位功能
                if(!bMOTMoveFinish[0])
                {
                    bMOTMoveFinish[0]=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, Speed*T_MODE_SPEED, "Z1UpZ2Down2_6");   //jou 2012-05-21 T mode index會跳電
                }
                else if(!bMOTMoveFinish[1])
                {
                    bMOTMoveFinish[1]=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, Speed, "Z1UpZ2Down2_7");
                    if(bMOTMoveFinish[1]==true && bPickErr==true)               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                    {
                        ShowMyMessage("Test Arm 2 Pick up error after Skip, need take away Arm 2 device", "請檢查index arm上是否有殘留Device", "Z1UpZ2Down2_8");
                        SoftStop=true;
                        StopAllMotor();
                    }
                }
                else if(!bMOTMoveFinish[2])
                {
                    bMOTMoveFinish[2]=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, Speed*T_MODE_SPEED, "Z1UpZ2Down2_9");   //jou 2012-05-21 T mode index會跳電
                }
            }
        }
        else
        {
            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                   //JimmyChiu 20220708 : add Index Arm Axis
                       (MTestY1+i)==MTestY2)
                    {
                        MOTMovflag[i]=false;
                    }
                    else
                    {
                        if(MOTMovflag[i])
                        {
                            MOT[MTestY1+i].Gali_ScanMotStatus();
                            MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                        }
                    }
                }
            }

            if(MOTMovflag[0]==false && MOTMovflag[1]==false && MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos(true)==false)                                   //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode = MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1, "Z1UpZ2Down2_10");
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Up_Z2Down())
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1Up_Z2Down;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode=MotorIndexToJamCode(14);
                        else
                            JamCode=MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode, MotErrPos+1, "Z1UpZ2Down2_11");
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ2].Position+=Speed;
        MOT[MTestZ1].Position-=Speed;
        if(MOT[MTestZ2].Position<=0)
            MOT[MTestZ2].Position=0;
        MOT[MTestY1].Position-=Speed;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].Position-=Speed;

        MOT[MTestY1].TargetPosition=Prod.TestY1_Front;                          //Steven 20210827 : 修改顯示Galil的Target / Current
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].TargetPosition=Prod.TestY2_Middle;
        MOT[MTestZ1].TargetPosition=0;
        MOT[MTestZ2].TargetPosition=-Prod.TestZ2_Test;

        if(MOT[MTestZ1].Position<=0 || MOT[MTestY1].Position<=Prod.TestY1_Front)
        {
            IndexStatus=Z1Up_Z2Down;
            MOT[MTestY1].Position=Prod.TestY1_Front;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                MOT[MTestY2].Position=Prod.TestY2_Middle;
            MOT[MTestZ1].Position=0;
            MOT[MTestZ2].Position=-Prod.TestZ2_Test;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1DownZ2Up2(int Speed, bool bPickErr)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    static bool MOTMovflag[]={true, true, true, true};
    static bool bMOTMoveFinish[]={false, false, false, false};
    GetIndexParm();                                                             //取得移動距離
    if(Motor->Enable && bGali_CardInstall==true)
    {
        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
            {
                MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
            }
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ2("Z1DownZ2Up2"))                                      //確認四軸command & encoder位置是否不同
            {
                bScanFlag=false;
                if(ScanIndexMotorCanMove())                                     //確認四軸是否alarm
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[iInposLed]=true;
                    bMOTMoveFinish[i]=false;
                }
            }
        }
        else if(!bMOTMoveFinish[0] || !bMOTMoveFinish[1] ||
                !bMOTMoveFinish[2] || !bMOTMoveFinish[3])                       //Sam 202400510 : T Movw 補 RTC 讓位功能
        {
            if(REAL_TIME_CCD && COM2->bCCDDummyRum==false &&
               TestIF_File.bUseRTCStepAsideMode)                                //Sam 202400510 : T Move 補 RTC 讓位功能
            {
                if(!bMOTMoveFinish[0])
                {
                    bMOTMoveFinish[0]=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, Speed*T_MODE_SPEED, "Z1DownZ2Up2_1");
                }
                else if(!bMOTMoveFinish[1])
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                     //JimmyChiu 20220708 : add Index Arm Axis
                    {
                        bMOTMoveFinish[1]=MOT[MTestY1].Gali_MotMove(Prod.TestY1_Middle, Speed, "Z1DownZ2Up2_2");
                    }
                    else
                    {
                        bMOTMoveFinish[1]=MOT[MTestY2].Gali_MotMove(Prod.TestY2_Rear, Speed, "Z1DownZ2Up2_3");
                    }
                }
                else if(!bMOTMoveFinish[2])
                {
                    bMOTMoveFinish[2]=MOT[MTestY1].Gali_MotMove(Prod.TestY1_Middle, Speed, "Z1DownZ2Up2_4");
                }
                else if(!bMOTMoveFinish[3])
                {
                    bMOTMoveFinish[3]=MOT[MTestZ1].Gali_MotMove( Prod.TestZ1_Test, Speed*T_MODE_SPEED, "Z1DownZ2Up2_5");
                }
            }
            else
            {
                bMOTMoveFinish[3]=true;                                         //Sam 202400510 : T Move 補 RTC 讓位功能
                if(!bMOTMoveFinish[0])
                {
                    bMOTMoveFinish[0]=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, Speed*T_MODE_SPEED, "Z1DownZ2Up2_6");   //jou 2012-05-21 T mode index會跳電
                }
                else if(!bMOTMoveFinish[1])
                {
                    bMOTMoveFinish[1]=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, Speed, "Z1DownZ2Up2_7");//JimmyChiu 20220708 : add Index Arm Axis
                    if(bMOTMoveFinish[1]==true && bPickErr==true)
                    {
                        ShowMyMessage("Test Arm 1 Pick up error after Skip, need take away Arm 1 device", "請檢查index arm上是否有殘留Device", "Z1DownZ2Up2_8");
                        SoftStop=true;
                        StopAllMotor();
                    }
                }
                else if(!bMOTMoveFinish[2])
                {
                    bMOTMoveFinish[2]=MOT[MTestZ1].Gali_MotMove( Prod.TestZ1_Test, Speed*T_MODE_SPEED, "Z1DownZ2Up2_9");   //jou 2012-05-21 T mode index會跳電
                }
            }
        }
        else
        {
            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                   //JimmyChiu 20220708 : add Index Arm Axis
                       (MTestY1+i)==MTestY2)
                    {
                        MOTMovflag[i]=false;
                    }
                    else
                    {
                        if(MOTMovflag[i])
                        {
                            MOT[MTestY1+i].Gali_ScanMotStatus();
                            MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                        }
                    }
                }
            }

            if(MOTMovflag[0]==false && MOTMovflag[1]==false && MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;

                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos(false)==false)                                  //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode = MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1, "Z1DownZ2Up2_10");
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Down_Z2Up())                                         //check z1 & z2 encoder position
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1Down_Z2Up;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode = MotorIndexToJamCode(14);
                        else
                            JamCode = MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode,MotErrPos+1, "Z1DownZ2Up2_11");
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ2].Position-=Speed;
        MOT[MTestZ1].Position+=Speed;
        if(MOT[MTestZ2].Position<=0)
            MOT[MTestZ2].Position=0;
        MOT[MTestY1].Position+=Speed;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].Position+=Speed;

        MOT[MTestY1].TargetPosition=Prod.TestY1_Middle;                         //Steven 20210827 : 修改顯示Galil的Target / Current
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].TargetPosition=Prod.TestY2_Rear;
        MOT[MTestZ1].TargetPosition=-Prod.TestZ1_Test;
        MOT[MTestZ2].TargetPosition=0;

        bool bY2PosSafe=(USE_INDEX_ARM_AXES==IndexArm_4_Axis)?(MOT[MTestY2].Position>=Prod.TestY2_Rear):true;  //JimmyChiu 20220708 : add Index Arm Axis
        if(MOT[MTestZ2].Position<=0 || bY2PosSafe)
        {
            IndexStatus=Z1Down_Z2Up;
            MOT[MTestY1].Position=Prod.TestY1_Middle;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                MOT[MTestY2].Position=Prod.TestY2_Rear;
            MOT[MTestZ1].Position=-Prod.TestZ1_Test;
            MOT[MTestZ2].Position=0;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1UpZ2Down(int Speed, bool TMode, bool bPickErr)
{
    if(CosFunction.bSortingBy2DList==true &&                                    //JerryYang 20230322 : 增加保護, 2D SORT模式index arm不用下壓到socket
       TestIF_File.bSortingBy2DIDList==true &&
       LastSet.iTester==_2D_SORT)
    {
        ShowMyMessage("Z1UpZ2Down Error");
        return false;
    }
    bool ret=false;

    #ifdef INDEX_PROTECT_TMOVE
    if(bOverRangeDoTMode==true)                                                 //Isaac 20201012 : index Y超過範圍，做一次Tmode
    {
        if(bOverRange4Indexhome==true)
        {
            return false;
        }
        else
        {
            ret=Z1UpZ2Down2(Speed, bPickErr);
            if(ret==true)
            {
                bOverRangeDoTMode=false;
                bTriger4Indexhome=false;
                GetTimeInfo();
                InitialMaxMinValue("Z1UpZ2Down");                               //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，歸零
                fMain->AddIndexPosLog("Save Log");
                fMain->AddIndexPosLog("Auto-calibration End.", true);
                RecordProcess("Auto-calibration End.");
            }
        }
    }
    else
    #endif
    {
        if(CosFunction.bSortingBy2DList==true &&                                //Frank 20221122 : 2DID sorting for ATK
           TestIF_File.bSortingBy2DIDList==true &&
           LastSet.iTester==_2D_SORT)
        {
            ShowMyMessage("Z1UpZ2Down Error");
            return false;
        }

        if(!TMode)
        {
            ret=Z1UpZ2Down1(Speed);
        }
        else
        {
            ret=Z1UpZ2Down2(Speed, bPickErr);
        }
    }
    return ret;
}
//------------------------------------------------------------------------------
bool TMyMotor::Z1DownZ2Up(int Speed, bool TMode, bool bPickErr)
{
    if(CosFunction.bSortingBy2DList==true &&                                    //JerryYang 20230322 : 增加保護, 2D SORT模式index arm不用下壓到socket
       TestIF_File.bSortingBy2DIDList==true &&
       LastSet.iTester==_2D_SORT)
    {
        ShowMyMessage("Z1UpZ2Down Error");
        return false;
    }
    bool ret=false;

    #ifdef INDEX_PROTECT_TMOVE
    if(bOverRangeDoTMode==true)                                                 //Isaac 20201012 : index Y超過範圍，做一次Tmode
    {
        if(bOverRange4Indexhome==true)
        {
            return false;
        }
        else
        {
            ret=Z1DownZ2Up2(Speed, bPickErr);
            if(ret==true)
            {
                bOverRangeDoTMode=false;
                bTriger4Indexhome=false;
                GetTimeInfo();
                InitialMaxMinValue("Z1DownZ2Up");                               //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，歸零
                fMain->AddIndexPosLog("Save Log");
                fMain->AddIndexPosLog("Auto-calibration End.", true);
                RecordProcess("Auto-calibration End.");
            }
        }
    }
    else
    #endif
    {
        if(CosFunction.bSortingBy2DList==true &&                                //Frank 20221122 : 2DID sorting for ATK
           TestIF_File.bSortingBy2DIDList==true &&
           LastSet.iTester==_2D_SORT)
        {
            ShowMyMessage("Z1UpZ2Down Error");
            return false;
        }

        if(!TMode)
        {
            ret=Z1DownZ2Up1(Speed);
        }
        else
        {
            ret=Z1DownZ2Up2(Speed, bPickErr);
        }
    }
    return ret;
}
//------------------------------------------------------------------------------
long TMyMotor::Gali_ReadPos()
{
    char str[256]="";
    long L1=0;
    AnsiString SelMot=AnsiString("");
    int Pos=0;
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable && bGali_CardInstall==true)
    {
        SelMot=Gali_GetMOT(Mot_Name);
        sprintf(str, "TD%s", SelMot);
        L1=Gali_Command(str, __FUNC__);
        if(Mot_Name==MTestY2)
            L1=-L1;

        if(bShowMotorMove==true)
            ScreenPos=(int)(Scale*(L1-FactStart))+RefStart;

        Pos=Gali_Command(str, __FUNC__);
        if(Mot_Name==MTestY1)
            Position=Pos*Motor->GearRatio;
        else
            Position=-Pos*Motor->GearRatio;

        return Position;
    }
    else
    {
        if(bShowMotorMove==true)
        {
            ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
            if(bPanelUse)
            {
                if(bUpDownMove)
                    PWinCtrl->Top=ScreenPos;
                else
                    PWinCtrl->Left=ScreenPos;
            }
        }
        if(Mot_Name==MTestY2)
            return Position;
        if(Mot_Name==MTestY1)
            return Position;
        else
            return -Position;
    }
}
//------------------------------------------------------------------------------
void TMyMotor::Gali_MotHome(AnsiString HomeAxis)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    AnsiString aTransferData="", Data="", Str="";
    if(HomeAxis=="X" && MovFlag)
        return;
    else if(HomeAxis=="Y" && MovFlag)
        return;
    else if(HomeAxis=="Z" && MovFlag)
        return;
    else if(HomeAxis=="W" && MovFlag)
        return;
    else if(HomeAxis=="YZ" && MovFlag)
        return;

    if(Motor->Enable && bGali_CardInstall==true)
    {
        aTransferData="";
        aTransferData="SP"+HomeAxis+"=2000;"+
                      "JG"+HomeAxis+"=-5000;"+
                      "AC"+HomeAxis+"=20000;"+
                      "DC"+HomeAxis+"=20000;"+
                      "HM"+HomeAxis+";"+
                      "BG"+HomeAxis+";";

        #ifndef SOFT_SIMULTE
            rc = DMCCommand(hDmc, aTransferData.c_str(), szBuffer1, sizeof(szBuffer1));
        #endif
        if(rc!=0)
        {
            SoftStop=true;
            SystemStart=false;
            Str=GetGalilErrString(rc, "Gali_MotHome");
            MyDBIProcessNew("Motion", "WAR2203", "Gail Command Err", AnsiString(aTransferData));
            fAllMotorHome=false;
            ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
            return;
        }
        else
        {
            MovFlag=true;
        }
    }
}
//---------------------------------------------------------------------------
void TMyMotor::Gali_MotHomeFindZ(AnsiString HomeAxis)                           //ChungHung 20141016 add find Z相
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    AnsiString aTransferData=AnsiString(""), Data=AnsiString(""), Str=AnsiString("");
    if(HomeAxis=="X" && MovFlag)
        return;
    else if(HomeAxis=="Y" && MovFlag)
        return;
    else if(HomeAxis=="Z" && MovFlag)
        return;
    else if(HomeAxis=="W" && MovFlag)
        return;
    else if(HomeAxis=="YZ" && MovFlag)
        return;

    if(Motor->Enable && bGali_CardInstall==true)
    {
        aTransferData="";
        //SP  Speed
        //JG  Jog
        //AC  Acceleration 加速
        //DC  Deceleration 減速
        //FI  Find Index
        //BG  Begin
        aTransferData="SP"+HomeAxis+"=300;"+                                    //Ifor 20170817 (Steven) "JG"+HomeAxis+"=-300;" ==> "JG"+HomeAxis+"=300;"
                          "JG"+HomeAxis+"=300;"+
                          "AC"+HomeAxis+"=500;"+
                          "DC"+HomeAxis+"=500;"+
                          "FI"+HomeAxis+";"+
                          "BG"+HomeAxis+";";
        #ifndef SOFT_SIMULTE
            rc=DMCCommand(hDmc, aTransferData.c_str(), szBuffer1, sizeof(szBuffer1));
        #endif
        if(rc!=0)
        {
            SoftStop=true;
            SystemStart=false;
            MyDBIProcessNew("Motion", "WAR2203", "Gail Command Err", AnsiString(aTransferData));
            Str=GetGalilErrString(rc, "Gali_MotHome");
            ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
            fAllMotorHome=false;
            return;
        }
        else
        {
            MovFlag=true;
        }
    }
}
//---------------------------------------------------------------------------
TQPF_Timer tGalilTwoZMoveDelay;
bool bGalilTwoZMoveFlag=false;
bool TMyMotor::Gali_Two_ZAxis_Move(int Pos, int Speed, AnsiString sFunc, bool bTwoPos, int Pos2)  //Steven 20150911 : modify for 2 Index Z move
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    char str[256]="";
    AnsiString SelMot=AnsiString(""), sErrFunc=__FUNC__+AnsiString(",Func2,")+sFunc;
    int Z1Pos=0, Z2Pos=0;
    bool flag[2]={false,false};

    if(Pos<0)                                                                   //JerryYang 20180411 (jou) : Z軸移動前確認Y軸位置保護
    {
        if(CosFunction.bIndexProtect==true)
        {
            if(CheckYPos()==false)
            {
                return false;
            }
        }
    }

    Z1Pos=0-Pos;
    if(bTwoPos)                                                                 //Steven 20150911 : modify for 2 Index Z move
    {
        Z2Pos=0-Pos2;

        if(Z2Pos==0)
        {
            Z2Pos=0;
        }
    }
    else
    {
        Z2Pos=0-Pos;
    }

    MOT[MTestZ1].GetRealPos(&Z1Pos);
    MOT[MTestZ2].GetRealPos(&Z2Pos);

    if(Pos==0)
    {
        Pos=0;
    }

    if(Motor->Enable && bGali_CardInstall==true)
    {
        MOT[MTestZ1].Position=MOT[MTestZ1].Gali_ReadPos();
        MOT[MTestZ2].Position=MOT[MTestZ2].Gali_ReadPos();
        if(MovFlag==false)
        {
            if(CheckTestZ(sErrFunc))
            {
                iGali_SpSpeed=Speed;
                sprintf(str, "SP,%d,%d;AC,%d,%d;DC,%d,%d;PA,%d,%d;BGYZ;",
                              Speed, Speed, GailAcSpeed, GailAcSpeed, GailDcSpeed, GailDcSpeed, Z1Pos, Z2Pos);

                if(ScanIndexMotorCanMove())
                {
                    Gali_Command(str, sErrFunc);
                }
                else
                {
                    return false;
                }

                MovFlag=true;
            }
        }
        else
        {
            if(Gali_Command("MG_BGy", sErrFunc)==1 ||                           //馬達目前是否移動 0:移動完成
               Gali_Command("MG_BGz", sErrFunc)==1)                             //ChungHung 20141016 add 確保每次皆到位
                return false;

            MOT[MTestZ1].Gali_ScanMotStatus();
            if(MOT[MTestZ1].Led[iInposLed]==false)
                flag[0]=true;
            MOT[MTestZ2].Gali_ScanMotStatus();
            if(MOT[MTestZ2].Led[iInposLed]==false)
                flag[1]=true;
            if(flag[0] && flag[1])
            {
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    IndexZCanMove[0]=true;                                      //jou 20200526 : 解除index hang up
                    IndexZCanMove[1]=true;
                    return true;
                }
            }
        }
    }
    else
    {
        MOT[MTestZ1].Position=-Pos;
        MOT[MTestZ2].Position=-Pos;
        MOT[MTestZ1].TargetPosition=Pos;
        MOT[MTestZ2].TargetPosition=Pos;
        IndexZCanMove[0]=true;                                                  //jou 20200526 : 解除index hang up
        IndexZCanMove[1]=true;
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogP(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    int JogData=Speed;
    AnsiString SelMot=AnsiString("");
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                  SelMot, Speed, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogPSetup(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }
    int JogData=1000;
    AnsiString SelMot=AnsiString("");
    JogData=abs(Speed);
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                  SelMot, 100, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogPAndCount(int Speed, int Count)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    int JogData=9990;
    JogData=Count;
    AnsiString SelMot=AnsiString("");
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                 SelMot, Speed, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogN(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    int JogData=-Speed;
    AnsiString SelMot=AnsiString("");
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                  SelMot, Speed, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogNSetup(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    int JogData=-1000;
    JogData=abs(Speed);
    JogData=0-JogData;
    AnsiString SelMot=AnsiString("");
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                  SelMot, 100, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//-----------------------------------------------------------------------------
void TMyMotor::Gali_JogNAndCount(int Speed,int Count)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    int JogData=-19990;
    JogData=-Count;
    AnsiString SelMot=AnsiString("");
    if(Mot_Name!=MTestY1)
        JogData=-JogData;
    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);
    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;JG%s=%d;BG%s;",
                  SelMot, Speed, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, JogData, SelMot);

    Gali_Command(str, __FUNC__);
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_MovePR(int Pos, int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    AnsiString SelMot=AnsiString(""), PRData=AnsiString("");
    GetRealPos(&Pos);
    if(Mot_Name!=MTestY1)
        PRData=-Pos;
    else
        PRData=Pos;

    char str[256]="";
    SelMot=Gali_GetMOT(Mot_Name);

    sprintf(str, "SP%s=%d;AC%s=%d;DC%s=%d;PR%s=%s;BG%s;",
                  SelMot, Speed, SelMot, GailAcSpeed, SelMot, GailDcSpeed, SelMot, PRData, SelMot);

    Gali_Command(str, __FUNC__);
    return true;
}
//------------------------------------------------------------------------------
bool Open_GaliCard()
{
    #ifdef SOFT_SIMULTE
    bGali_CardInstall=true;
    return true;
    #else
    char ControllerDesc[256]="";
    AnsiString  ControllerInfo=AnsiString(""), Str=AnsiString("");
    #ifndef SOFT_SIMULTE
        rc=DMCGetControllerDesc(1, ControllerDesc, sizeof(ControllerDesc));
    #endif
    ControllerInfo=ControllerDesc;
    if(rc!=0)
    {
        ShowErrorMessage("WAR1636", 0, MMSystem);
        return false;
    }
    #ifndef SOFT_SIMULTE
        rc=DMCOpen(1, hWnd, &hDmc);                                             //開卡
    #endif
    if(rc!=0)
    {
        SoftStop=true;
        SystemStart=false;
        Str=GetGalilErrString(rc, "Open_GaliCard");
        ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
        fAllMotorHome=false;
        return false;
    }
    #ifndef SOFT_SIMULTE
    rc=DMCReset(hDmc);                                                          //kevin 20150909 add  RESET
    #endif
    if(rc!=0)
    {
        SoftStop=true;
        SystemStart=false;
        Str=GetGalilErrString(rc, "Open_GaliCard Reset");
        ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
        fAllMotorHome=false;
        return false;
    }

    #ifndef SOFT_SIMULTE
        rc=DMCCommand(hDmc, "SH", szBuffer, sizeof(szBuffer));
    #endif
    if(rc!=0)
    {
        SoftStop=true;
        SystemStart=false;
        MyDBIProcessNew("Motion", "WAR2203", "Gail Command Err", AnsiString("SH"));
        Str=GetGalilErrString(rc, "Open_GaliCard ServoON");
        ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
        fAllMotorHome=false;
    }
    bGali_CardInstall=true;
    return true;
    #endif
}
//------------------------------------------------------------------------------
// kevin 20150909 GAIL CLOSE CARD
//------------------------------------------------------------------------------
bool Close_GaliCard()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        AnsiString Str=AnsiString("");
        DMCDiagnosticsOff(hDmc);                                                //kevin 20150909 關閉記錄 GAIL通訊
        rc=DMCClose(hDmc);
        if(rc!=0)
        {
            SoftStop=true;
            SystemStart=false;
            Str=GetGalilErrString(rc, "Close_GaliCard");
            ShowErrorMessage("WAR1635", 0, MMSystem, 0, Str);
            fAllMotorHome=false;
            return false;
        }
        return true;
    #endif
}
//------------------------------------------------------------------------------
long TMyMotor::Gali_ReadEncoderPos()
{
    AnsiString SelMot=AnsiString("");
    int iPos=0;
    if(Motor->Enable==false)
    {
       return Gali_ReadPos();
    }
    else
    {
        SelMot=Gali_GetMOT(Mot_Name);
        iPos=Gali_Command("TP"+SelMot, __FUNC__);
        if(Mot_Name==MTestY1 && (USE_INDEX_ARM_AXES==IndexArm_4_Axis))          //JimmyChiu 20220708 : add Index Arm Axis
        {
            EncoderPosition=-iPos*Motor->GearRatio;
        }
        else
        {
            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis && Mot_Name==MTestY2)        //JimmyChiu 20220708 : add Index Arm Axis
            {
                return 0;
            }
            EncoderPosition=iPos*Motor->GearRatio;
        }
        return EncoderPosition;
    }
}
//------------------------------------------------------------------------------
bool TMyMotor::CheckPos(bool Front)
{
    int iEncoderPos1=MOT[MTestY1].Gali_ReadEncoderPos();
    int iEncoderPos2=0;
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
    {
        iEncoderPos2=MOT[MTestY2].Gali_ReadEncoderPos();
    }
    bool b1=false, b2=false, b3=false;
    if(Front)
    {
        b1=CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Front, GaliPosOffSet);
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            b2=true;
        else
            b2=CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Middle, GaliPosOffSet);
    }
    else
    {
        b1=CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Middle, GaliPosOffSet);
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            b2=true;
        else
            b2=CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Rear, GaliPosOffSet);
    }
    b3=(b1 && b2);
    return b3;
}
//------------------------------------------------------------------------------
bool TMyMotor::CheckYPos()                                                      //JerryYang 20180411 (jou) : Z軸移動前確認Y軸位置保護
{
    static int iRetryCnt=0;
    int iEncoderPos1=MOT[MTestY1].Gali_ReadEncoderPos();
    int iEncoderPos2=0;
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
    {
        iEncoderPos2=MOT[MTestY2].Gali_ReadEncoderPos();
    }
    bool b1=(CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Front,  IniConfig.GaliPosRange) ||
             CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Middle, IniConfig.GaliPosRange));
    bool b2=false;

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
        b2=true;
    else
        b2=(CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Rear,   IniConfig.GaliPosRange) ||
            CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Middle, IniConfig.GaliPosRange));

    if(b1 && b2)
    {
        iRetryCnt=0;
        bShowIndexMotorError=false;                                             //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
        return true;
    }
    else
    {
        #ifdef INDEX_PROTECT_TMOVE
        if(iRetryCnt>50)
        {
            if(bOverRangeDoTMode==false)
                RecordIndexPosition(3, 3);                                      //Gali_Two_ZAxis_Move
            bOverRangeDoTMode=true;                                             //觸發做Tmode
        }
        #else
        if(iRetryCnt<100)
        {
            iRetryCnt++;
            return false;
        }
        #endif
        iRetryCnt=0;
        bShowIndexMotorError=true;                                              //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
        ShowIndexMotorError(AnsiString("Gali_Two_ZAxis_Move"), true);
        return false;
    }
}
//-------------------------------------------------------------------------------
bool TMyMotor::CheckPos_nnMode(bool bOpen)
{
    int iEncoderPos1=MOT[MTestY1].Gali_ReadEncoderPos();
    int iEncoderPos2=0;
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
    {
        iEncoderPos2=MOT[MTestY2].Gali_ReadEncoderPos();
    }
    bool b1=false, b2=false, b3=false;
    if(bOpen)
    {
        b1=CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Front,  IniConfig.GaliPosRange);
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            b2=false;
        else
            b2=CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Rear,   IniConfig.GaliPosRange);
    }
    else
    {
        b1=CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Middle,  IniConfig.GaliPosRange);
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            b2=false;
        else
            b2=CheckArmPosArrival(iEncoderPos2, Prod.TestY2_Middle,  IniConfig.GaliPosRange);
    }
    b3=(b1 && b2);
    return b3;
}
//-------------------------------------------------------------------------------
// 用於 TestY 及 TestZ1 TestZ2 之 safe point so ,randge is small
bool TMyMotor::Gali_ReadEncoderInRandge(long checkpos)
{
    if(Motor->Enable==false)
        return true;

    long pos=0, check=0, pos1=0;
    pos=Gali_ReadEncoderPos();
    if(pos<0L)
        pos=0L-pos;
    pos1=checkpos;
    if(pos1<0L)
        pos1=0-pos1;
    check=pos-pos1;
    if(check<0L)
        check=0L-check;
    GaliAxisAlarm[0]=false;
    GaliAxisAlarm[1]=false;
    GaliAxisAlarm[2]=false;
    GaliAxisAlarm[3]=false;
    if(check>=GaliPosOffSet)
    {
        if(Mot_Name==MTestY1)
            GaliAxisAlarm[0]=true;
        else if(Mot_Name==MTestZ1)
            GaliAxisAlarm[1]=true;
        else if(Mot_Name==MTestZ2)
            GaliAxisAlarm[2]=true;
        else
            GaliAxisAlarm[3]=true;

        return false;
    }
    else
    {
        return true;
    }
}
//-------------------------------------------------------------------------------
bool TMyMotor::Gali_ReadEncoderBelowCheckHeight(long checkpos)
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        long pos, pos1;
        if(Motor->Enable==false)
            return false;

        pos=checkpos-GaliPosOffSet;
        pos1=Gali_ReadEncoderPos();
        if(pos1<pos)
            return true;
        else
            return false;
    #endif
}
//-------------------------------------------------------------------------------
bool TMyMotor::Gali_ReadEncoderInRandgeMinLimit(long checkpos)                  //檢查Y軸command position和Encoder值
{
    if(Motor->Enable==false)
        return true;

    long pos=0, check=0, pos1=0;
    pos=Gali_ReadEncoderPos();
    pos=abs(pos);
    pos1=checkpos;
    pos1=abs(pos1);
    check=pos-pos1;
    check=abs(check);

    //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值
    //=>
    if(Mot_Name==MTestY1)//Arm1
    {
        if(check>iMaxCommandY1)         //找出最大值
        {
            iMaxCommandY1=check;
        }
        else if(check<iMinCommandY1)    //找出最小值
        {
            iMinCommandY1=check;
        }
    }
    else if(Mot_Name==MTestY2)//Arm2
    {
        if(check>iMaxCommandY2)      // iMaxCommand=0,iMinCommand=0,iMaxTeach=0,iMinTeach=0;
        {
            iMaxCommandY2=check;        //找出最大值
        }
        else if(check<iMinCommandY2)    //找出最小值
        {
            iMinCommandY2=check;
        }
    }
    //<=
    //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值

    if(check>=IniConfig.GaliPosRange)
    {
        bShowIndexMotorError=true;      //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
        return false;

    }
    else
    {
        bShowIndexMotorError=false;     //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
        return true;
    }
}
//-------------------------------------------------------------------------------
bool TMyMotor::Gali_ReadEncoderInRandgeNoWait(long checkpos)
{
    if(Motor->Enable==false)
        return true;

    long pos=0, check=0, pos1=0;
    pos=Gali_ReadEncoderPos();
    if(pos<0L)
        pos=0L-pos;
    pos1=checkpos;
    if(pos1<0L)
        pos1=0-pos1;
    check=pos-pos1;
    if(check<0L)
        check=0L-check;
    GaliAxisAlarm[0]=false;
    GaliAxisAlarm[1]=false;
    GaliAxisAlarm[2]=false;
    GaliAxisAlarm[3]=false;
    if(check>=(GaliPosOffSet*2))
    {
        if(Mot_Name==MTestY1)
            GaliAxisAlarm[0]=true;
        else if(Mot_Name==MTestZ1)
            GaliAxisAlarm[1]=true;
        else if(Mot_Name==MTestZ2)
            GaliAxisAlarm[2]=true;
        else
            GaliAxisAlarm[3]=true;
        return false;
    }
    else
    {
        return true;
    }
}
//-------------------------------------------------------------------------------
// 用於 TestZ1 TestZ2 之 test point ,so randge is big
bool TMyMotor::Gali_ReadEncoderMaxRandge(long checkpos)
{
    if(Motor->Enable==false)
        return true;

    long pos=0, check=0, pos1=0;
    pos=Gali_ReadEncoderPos();
    if(pos<0L)
        pos=0L-pos;
    pos1=checkpos;
    if(pos1<0L)
        pos1=0-pos1;
    check=pos-pos1;
    GaliAxisAlarm[0]=false;
    GaliAxisAlarm[1]=false;
    GaliAxisAlarm[2]=false;
    GaliAxisAlarm[3]=false;
    if(check<0L)
        check=0L-check;
    if(check>=(GaliPosOffSet*2))
    {
        if(Mot_Name==MTestY1)
            GaliAxisAlarm[0]=true;
        else if(Mot_Name==MTestZ1)
            GaliAxisAlarm[1]=true;
        else if(Mot_Name==MTestZ2)
            GaliAxisAlarm[2]=true;
        else
            GaliAxisAlarm[3]=true;
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_ReadEncoderOver(long checkpos)
{
    long pos=Gali_ReadEncoderPos();
    if(pos>checkpos)
        return false;
    else
        return true;
}
//------------------------------------------------------------------------------
// 1112 Start
bool TMyMotor::ISZ1Up_Z2Down()
{
    //*Index更換Offset會hang up?   (atester.cpp #3917的判斷會異常) jou 2010-04-13
    if(MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
       MOT[MTestZ2].Gali_ReadEncoderMaxRandge(iBackUpZ2DownPosition))
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::ISZ1Up_Z2DownNoWait()
{
    if(Prod.TestZ2_Drop_Offset!=0)  //DropContact or DirectContactModeDiffentSpeed
    {
        //Ifor 20191022 : 避免Arm2不動作時更新到TestZ2_Test位置導致發出四軸異常
        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
           TestIF_File.bArm1PickPlaceArm2Test==true     &&
           bCheckGiveWay==true                          )
        {
            if(MOT[MTestZ1].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ2_Safe))
                return true;
            else
                return false;
        }
        else
        {
            if(MOT[MTestZ1].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset))
                return true;
            else
                return false;
        }
    }
    else
    {
        //Ifor 20191022 : 避免Arm2不動作時更新到TestZ2_Test位置導致發出四軸異常
        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
           TestIF_File.bArm1PickPlaceArm2Test==true     &&
           bCheckGiveWay==true                          )
        {
            if(MOT[MTestZ1].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ2_Safe))
                return true;
            else
                return false;
        }
        else
        {
            if(MOT[MTestZ1].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ2_Test))
                return true;
            else
                return false;
        }
    }
}
//------------------------------------------------------------------------------
bool TMyMotor::ISZ1Down_Z2Up()
{
    //*Index更換Offset會hang up?   (atester.cpp #3917的判斷會異常) jou 2010-04-13
    if(MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe) &&
       MOT[MTestZ1].Gali_ReadEncoderMaxRandge(iBackUpZ1DownPosition))
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::ISZ1Down_Z2UpNoWait()
{
    if(Prod.TestZ1_Drop_Offset!=0) //DropContact or DirectContactModeDiffentSpeed
    {
        if(MOT[MTestZ2].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ2_Safe) &&
            MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset))
            return true;
        else
            return false;
    }
    else
    {
        if(MOT[MTestZ2].Gali_ReadEncoderInRandgeNoWait(Prod.TestZ2_Safe) &&
            MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Test ))
            return true;
        else
            return false;
    }
}
//------------------------------------------------------------------------------
bool TMyMotor::ISNormal()
{
    if(MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
        MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe))
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
// 1112 end
//------------------------------------------------------------------------------
bool TMyMotor::Gali_SingalHome(bool IndexZFirstHome)
{
    long lnumber=0;
    bool checkflag=false;
    int  TestZSafePos=Prod.TestZ1_Safe;                                         //kevin 20160905
    int iPos=0;

    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(Motor->Enable==false)                                                    //Jimmychiu 20230201 add
        return true;

    AnsiString tempS="",tempS1="";
    int tempMOT=0;                                                              //kevin 20170607 motor number
    AnsiString sTime=Now().FormatString("hh:nn:ss"), sData="";                  //kevin 20170515 add

    if(Alias=="MTestY1")
    {
        tempS="X";
        tempS1="x";
        tempMOT=MTestY1;                                                        //kevin 20170515 add motor number
    }
    else if(Alias=="MTestZ1")
    {
        tempS="Y";
        tempS1="y";
        tempMOT=MTestZ1;                                                        //kevin 20170515 add motor number
    }
    else if(Alias=="MTestZ2")
    {
        tempS="Z";
        tempS1="z";
        tempMOT=MTestZ2;                                                        //kevin 20170515 add motor number
    }
    else
    {
        tempS="W";
        tempS1="w";
        tempMOT=MTestY2;                                                        //kevin 20170515 add motor number
    }
    if(Motor->Enable==false)
    {
        bScanFlag=false;
        MovFlag=false;
        GaliSofDelayCount=0;
        HomeFlag=1;
        return true;
    }
    Gali_ScanMotStatus();
    int &Task=iGali_SingalHomeTask;
    switch(Task)
    {
        case 1:
            if(Gali_Command("MG_BG"+tempS1, "Gali_SingalHome_1")==0)            //馬達目前是否移動? 0:移動完成
            {
                Gali_Command("DP"+tempS+"=0;DE"+tempS+"=0;", "Gali_SingalHome_1");         //改變軸卡 command pos and Encoder pos set to 0
                Task=5;
            }
            break;
        case 5:
            if(Gali_Command("MG_BG"+tempS1, "Gali_SingalHome_5")==0)            //馬達目前是否移動 0:移動完成
            {
                Task=8;
            }
            break;
        case 8:
            Gali_ScanMotStatusTIMO();                                           //取得目前I/O狀態
            if(Led[iServoOn]==true)                                             //Servo On
            {
                Task=10;
            }
            else                                                                //Servo Off
            {
                Task=1;
                return false;
            }
            break;
        case 10:
            if(Led[iHomeLed] && (Alias=="MTestY1" || Alias=="MTestY2"))         //20140923 wei  Y軸極限回Home
            {
                Task=20;
            }
            else
            {
                Task=30;
            }
            break;
        case 20:
            if(Motor->HomeDirection)
                Gali_JogN(Motor->PHomeHighSpeed);
            else
                Gali_JogP(Motor->PHomeHighSpeed);
            if(Led[iHomeLed]==false)
            {
                Gali_Command("ST"+tempS, "Gali_SingalHome_20");                 //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                Task=30;
            }
            break;
        case 30:
            if(Led[iHomeLed])
            {
                Gali_Command("ST"+tempS, "Gali_SingalHome_30");                 //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                DelayCT.Set0_1SecAndOn(5);
                Task=100;
            }
            else
            {
                Task=50;
            }
            break;
        case 50:
            if(Motor->HomeDirection)
                Gali_JogP(Motor->PHomeHighSpeed);
            else
                Gali_JogN(Motor->PHomeHighSpeed);
            Task=30;                                                            //20140923 wei  Z軸極限回Home
            break;
        case 100:
            if(DelayCT.Off())
            {
                if(Motor->HomeDirection)
                    Gali_JogN(Motor->PHomeLowSpeed);
                else
                    Gali_JogP(Motor->PHomeLowSpeed);
                Task=150;
            }
            break;
        case 150:
            if(Led[iHomeLed])
            {
                Task=100;
                iPos=MOT[tempMOT].Gali_ReadEncoderPos();
                if((tempMOT==MTestZ1 || tempMOT==MTestZ2) &&
                   iPos<-1000)                                                  //Steven 20191213 : Index Z sensor故障保護
                {
                    Gali_Command("ST"+tempS, "Gali_SingalHome_150");            //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                    DelayCT.Set0_1SecAndOn(5);
                    Task=160;
                }
            }
            else
            {
                Gali_Command("ST"+tempS, "Gali_SingalHome_150");                //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                DelayCT.Set0_1SecAndOn(5);
                Task=200;
            }
            break;
        //Steven 20191213 : Index Z sensor故障保護
        //==>
        case 160:
            if(DelayCT.Off())
            {
                if(Gali_MotMove(0, Motor->PHomeLowSpeed, "Gali_SingalHome_160"))
                {
                    Task=170;
                }
            }
            break;
        case 170:
            if(tempMOT==MTestZ1)
                ShowErrorMessage("WAR03502", 0, MTestZ1, 0, "Gali_SingalHome_170");
            else
                ShowErrorMessage("WAR03503", 0, MTestZ2, 0, "Gali_SingalHome_170");
            Task=1;
            break;
        //<==
        //Steven 20191213 : Index Z sensor故障保護
        case 200:
            if(DelayCT.Off())
            {
                Gali_MotHome(tempS);
                MovFlag=true;
                Task=250;                                                       //Ifor 20170817 (wei) Mark Z PHASE 移置別處處理
            }
            break;
        case 250:                                                               //jimmychiu 20220815 avoid repeat sent home command
            if(Gali_Command("MG_BG"+tempS1, "Gali_SingalHome_250")==1)
            {
                Task=300;
            }
            break;
        case 300:
            lnumber=Gali_Command("MG_SC"+tempS1, "Gali_SingalHome_300");
            if(lnumber==10)                                                     //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
            {
                Motor->LastHomePos=Gali_ReadEncoderPos();                       //讀取目前Encoder
                Gali_MotHomeFindZ(tempS);                                       //ChungHung 20141016 add find Z相
                DelayCT.SetMSAndOn(100);                                        //ChungHung 20141016 10--->100
                Task=350;
            }
            break;
        case 350:                                                               //ChungHung 20141016 add find Z相
            if(DelayCT.Off())
            {
                if(Gali_Command("MG_SC"+tempS1, "Gali_SingalHome_350")==10)     //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                {
                    DelayCT.SetMSAndOn(100);                                    //ChungHung 20141016 10--->100
                    Task=400;
                }
            }
            break;
        case 400:
            if(DelayCT.Off())
            {
                Motor->LastHomePos=MOT[tempMOT].Gali_ReadEncoderPos();          //讀取目前Encoder
                Gali_Command("DP"+tempS+"=0;DE"+tempS+"=0;", "Gali_SingalHome_400");
                Motor->LastHomePos=Gali_ReadEncoderPos();                       //讀取目前Encoder
                bScanFlag=false;
                MovFlag=false;
                GaliSofDelayCount=0;
                HomeFlag=1;
                if(Alias=="MTestZ1" || Alias=="MTestZ2")                        //Richard 20221206 : 開會討論後決定先回復為原先模式
                {
                    if(Alias=="MTestZ1")
                        checkflag=CheckTestZ1("Gali_SingalHome400");
                    if(Alias=="MTestZ2")
                        checkflag=CheckTestZ2("Gali_SingalHome400");
                    if(checkflag)
                        Task=450;
                    else
                        Task=500;
                }
                else
                {
                    Task=500;
                }
            }
            break;
        case 450:
            TestZSafePos=10;
            if(Alias=="MTestZ1" || Alias=="MTestZ2")
            {
                TestZSafePos=Prod.TestZ1_Safe;                                  //kevin 20160905
            }

            if(Gali_MotMove(TestZSafePos, Motor->PHomeHighSpeed, "Gali_SingalHome_450"))
            {
                Task=500;
            }
            break;
        case 500:
            Task=900;
            if((CUSTOMER_CODE==CC_ASE_KaohSiung ||
                CosFunction.bHiSiliconFunction==true) &&
               IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true &&
               (Alias=="MTestZ1" || Alias=="MTestZ2"))                          //kevin 20170605 (Steven) add
            {
                MovFlag=false;
                GaliSofDelayCount=0;
                HomeFlag=1;
            }
            return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool TMyMotor::Gali_FindZPhase()
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    bool checkflag=false;
    int  TestZSafePos=Prod.TestZ1_Safe;

    static bool bCheckOverRangeZ1=false;                                        //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    static bool bCheckOverRangeZ2=false;                                        //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    static int iZ1DistanceRef=0;                                                //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    static int iZ1DistanceReal=0;                                               //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    static int iZ2DistanceRef=0;                                                //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    static int iZ2DistanceReal=0;                                               //kevin 20170515 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
    int iZData=0,ret=0;                                                         //kevin 20170511 add Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range

    //Isaac 20201110 : Index Y find motor phase
    //=>
    int iYData=0;
    static bool bCheckOverRangeY1=false;
    static bool bCheckOverRangeY2=false;
    static int iY1DistanceRef=0;
    static int iY1DistanceReal=0;
    static int iY2DistanceRef=0;
    static int iY2DistanceReal=0;
    //<=
    //Isaac 20201110 : Index Y find motor phase

    AnsiString tempS="", tempS1="";
    int tempMOT=0;                                                              //kevin 20170607 motor number
    AnsiString sTime=Now().FormatString("hh:nn:ss"), sData="";                  //kevin 20170515 add
    int iwhichIndexAxis=-1;                                                     //Isaac 20201110 : Index Y find motor phase，x,y,z,w == Y1,Z1,Z2,Y2 ==0,1,2,3

    if(Alias=="MTestY1")                                                        //Isaac 20201110 : Index Y find motor phase
    {
        tempS="X";
        tempS1="x";
        tempMOT=MTestY1;
        iwhichIndexAxis=0;
    }
    else if(Alias=="MTestZ1")
    {
        tempS="Y";
        tempS1="y";
        tempMOT=MTestZ1;                                                        //kevin 20170515 add motor number
        iwhichIndexAxis=1;
    }
    else if(Alias=="MTestZ2")
    {
        tempS="Z";
        tempS1="z";
        tempMOT=MTestZ2;                                                        //kevin 20170515 add motor number
        iwhichIndexAxis=2;
    }
    else// if(Alias=="MTestY2")                                                 //Isaac 20201110 : Index Y find motor phase
    {
        tempS="W";
        tempS1="w";
        tempMOT=MTestY2;
        iwhichIndexAxis=3;
    }

    if(Motor->Enable==false)
    {
        bScanFlag=false;
        MovFlag=false;
        GaliSofDelayCount=0;
        HomeFlag=1;
        return true;
    }
    Gali_ScanMotStatus();
    //int &Task=iGali_FindZPhaseTask;
    int &Task=iGali_FindZPhaseTask[iwhichIndexAxis];                            //Isaac 20201110 : Index Y find motor phase
    switch(Task)
    {
        case 1:
            if(Gali_Command("MG_BG"+tempS1, __FUNC__)==0)                       //馬達目前是否移動? 0:移動完成
            {
                iZ1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", 120);
                iZ2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", 120);
                iY1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", 20);
                iY2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", 20);

                Task=300;
            }
            break;
        case 300:
            Motor->LastHomePos=Gali_ReadEncoderPos();                           //讀取目前Encoder
            if(Alias=="MTestZ1" || Alias=="MTestZ2")                            //kevin 20170605
            {
                MovFlag=false;                                                  //Ifor 20170818 尋找Z相需設定false
                Gali_MotHomeFindZ(tempS);                                       //kevin 20170606 馬達移到離開HOME 找Z相 ChungHung 20141016 add find Z相
            }
            else if(Alias=="MTestY1" || Alias=="MTestY2")                       //Isaac 20201110 : Index Y find motor phase
            {
                MovFlag=false;
                Gali_MotHomeFindZ(tempS);
            }
            DelayCT.SetMSAndOn(100);                                            //ChungHung 20141016 10--->100
            Task=350;
            break;
        case 350:                                                               //ChungHung 20141016 add find Z相
            if(DelayCT.Off())
            {
                if(Gali_Command("MG_SC"+tempS1, "Gali_FindZPhase_1")==10)                  //ChungHung 20141016 fix 每次回Home後 位置可能不在Home點
                {
                    DelayCT.SetMSAndOn(100);                                    //ChungHung 20141016 10--->100
                    Task=400;
                }
            }
            break;
        case 400:
            if(DelayCT.Off())
            {
                Motor->LastHomePos=MOT[tempMOT].Gali_ReadEncoderPos();          //讀取目前Encoder
                bScanFlag=false;
                MovFlag=false;
                Task=410;
            }
        //==========================================kevin 20170515 ZPhase home==============>
        case 410:
            if(tempMOT==MTestZ1)
            {
                iIndexZ1HomeToZPhaseDistanceFirst=Gali_ReadEncoderPos();
                iIndexZ1HomeToZPosFirst=MOT[MTestZ1].Gali_ReadPos();            //kevin 20170605 read pos
            }
            else if(tempMOT==MTestZ2)
            {
                iIndexZ2HomeToZPhaseDistanceFirst=Gali_ReadEncoderPos();
                iIndexZ2HomeToZPosFirst=MOT[MTestZ2].Gali_ReadPos();            //kevin 20170605 read pos
            }
            else if(tempMOT==MTestY1)                                           //Isaac 20201110 : Index Y find motor phase
            {
                iIndexY1HomeToYPhaseDistanceFirst=Gali_ReadEncoderPos();
                iIndexY1HomeToYPosFirst=MOT[MTestY1].Gali_ReadPos();            //kevin 20170605 read pos
            }
            else if(tempMOT==MTestY2)                                           //Isaac 20201110 : Index Y find motor phase
            {
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                 //JimmyChiu 20220708 : add Index Arm Axis
                {
                    iIndexY2HomeToYPhaseDistanceFirst=Gali_ReadEncoderPos();
                    iIndexY2HomeToYPosFirst=MOT[MTestY2].Gali_ReadPos();            //kevin 20170605 read pos
                }
            }
            MovFlag=false;
            Task=420;
           break;
        case 420:                                                               //Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
            Gali_MotHome(tempS);                                                //kevin 20170605 add 找 HOME 點
            if(Gali_Command("MG_BG"+tempS1, __FUNC__)==1)                       //是否移動?
            {
                Task=421;
            }
            break;
       case 421:
            if(Gali_Command("MG_SC"+tempS1, "Gali_FindZPhase_421")==10)         //ChungHung 20150831 add for home process SC==9 : Stopped after homing(HM)
            {
                DelayCT.Set0_1SecAndOn(10);
                Task=430;
            }
            break;
        case 430:
            if(DelayCT.Off()==true)
            {
                if(Gali_Command("MG_BG"+tempS1, "Gali_FindZPhase_430")==0)      //移動完成
                {
                    MovFlag=true;
                    Task=436;
                }
            }
            break;
        case 436:                                                               //Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
            if(tempMOT==MTestZ1)
            {
                iIndexZ1HomeToZPhaseDistanceSecond=MOT[MTestZ1].Gali_ReadEncoderPos();
                iIndexZ1HomeToZPosSecond=MOT[MTestZ1].Gali_ReadPos();           //kevin 20170605 read pos
            }
            else if(tempMOT==MTestZ2)
            {
                iIndexZ2HomeToZPhaseDistanceSecond=MOT[MTestZ2].Gali_ReadEncoderPos();
                iIndexZ2HomeToZPosSecond=MOT[MTestZ2].Gali_ReadPos();           //kevin 20170605 read pos
            }
            else if(tempMOT==MTestY1)                                           //Isaac 20201110 : Index Y find motor phase
            {
                iIndexY1HomeToYPhaseDistanceSecond=MOT[MTestY1].Gali_ReadEncoderPos();
                iIndexY1HomeToYPosSecond=MOT[MTestY1].Gali_ReadPos();           //kevin 20170605 read pos
            }
            else if(tempMOT==MTestY2)                                           //Isaac 20201110 : Index Y find motor phase
            {
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                         //JimmyChiu 20220708 : add Index Arm Axis
                {
                    iIndexY2HomeToYPhaseDistanceSecond=MOT[MTestY2].Gali_ReadEncoderPos();
                    iIndexY2HomeToYPosSecond=MOT[MTestY2].Gali_ReadPos();       //kevin 20170605 read pos
                }
            }
            MovFlag=false;                                                      //Z motor move
            GaliSofDelayCount=0;
            Task=446;
            break;
        case 446:                                                               //Jeffrey 20170414 Check IndexZ Home to Z Phase Distance Range
            if(IniConfig.iD63IndexZHomeToZPhaseRange<0)
            {
                IniConfig.iD63IndexZHomeToZPhaseRange=-IniConfig.iD63IndexZHomeToZPhaseRange;
            }

            if(tempMOT==MTestZ1)
            {
                if(bCheckOverRangeZ1==false)
                {
                    iZ1DistanceRef=abs(iIndexZ1HomeToZPhaseDistanceFirst-iIndexZ1HomeToZPhaseDistanceSecond);
                    if(CheckIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position")==false)
                    {
                        WriteIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", iZ1DistanceRef);//  iZ1DistanceRef );
                        sData.sprintf("%s Finding phase no parameter, add Index_Z1_Home_Position=%d", sTime, iZ1DistanceRef);
                        SaveFile(asIndexZphasePath, sData);
                    }
                    else
                    {
                        iZ1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", 120);
                    }
                    bCheckOverRangeZ1=true;
                }
                iZ1DistanceReal=abs(iIndexZ1HomeToZPhaseDistanceFirst-iIndexZ1HomeToZPhaseDistanceSecond);
                iZData=abs(iZ1DistanceReal-iZ1DistanceRef);
                sData.sprintf("%s Arm 1 Zphase:First: %d ,Second: %d ,Real %d,ref %d,Zhase:%d", sTime, iIndexZ1HomeToZPhaseDistanceFirst, iIndexZ1HomeToZPhaseDistanceSecond, iZ1DistanceReal, iZ1DistanceRef, iZData); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511
                RecordProcess(sData);
                sData.sprintf("%s Arm 1 ZRead Pos First: %d ,Second: %d", sTime, iIndexZ1HomeToZPosFirst, iIndexZ1HomeToZPosSecond); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511
                if(iZData>(IniConfig.iD63IndexZHomeToZPhaseRange))              //50->0
                {
                    ret=ShowErrorMessage("WAR0307", K_RETRY|K_SKIP, MMSystem);
                    if(ret==K_SKIP)
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                        {
                            iZ1DistanceRef=iZ1DistanceReal;
                            WriteIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", iZ1DistanceRef);
                            sData.sprintf("%s Skip press,and save Index_Z1_Home_Position as %d", sTime, iZ1DistanceRef);
                            SaveFile(asIndexZphasePath, sData);

                            iZ1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", 120);
                        }
                        else
                        {
                            bZ1ModifyDistanceRef=true;                          //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                        }
                        Task=447;
                    }
                    else if(ret==K_RETRY)
                    {
                        iZ1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", 120);
                        Task=1;
                        break;
                    }
                }
                else
                {
                    Task=447;
                    bZ1ModifyDistanceRef=false;
                }

                if(bZ1ModifyDistanceRef==true)                                  //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                {
                    iZ1ModifyDistanceRef=iZ1DistanceReal;
                }
                else
                {
                    iZ1ModifyDistanceRef=iZ1DistanceRef;
                }

                iIndexZ1HomeToZPhaseDistanceFirst=0;
                iIndexZ1HomeToZPhaseDistanceSecond=0;
                iZ1DistanceReal=0;                                              //kevin 20170511 add log
            }
            else if(tempMOT==MTestZ2)
            {
                if(bCheckOverRangeZ2==false)
                {
                    iZ2DistanceRef=abs(iIndexZ2HomeToZPhaseDistanceFirst-iIndexZ2HomeToZPhaseDistanceSecond);
                    if(CheckIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position")==false)
                    {
                        WriteIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", iZ2DistanceRef);
                        sData.sprintf("%s Finding phase no parameter, add Index_Z2_Home_Position=%d", sTime, iZ2DistanceRef);
                        SaveFile(asIndexZphasePath, sData);
                    }
                    else
                    {
                        iZ2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", 120);
                    }
                    bCheckOverRangeZ2=true;
                }
                iZ2DistanceReal=abs(iIndexZ2HomeToZPhaseDistanceFirst-iIndexZ2HomeToZPhaseDistanceSecond);
                iZData=abs(iZ2DistanceReal-iZ2DistanceRef);
                sData.sprintf("%s Arm 2 Zphase:First: %d ,Second: %d ,Real %d,ref %d,Zhase:%d", sTime, iIndexZ2HomeToZPhaseDistanceFirst, iIndexZ2HomeToZPhaseDistanceSecond, iZ2DistanceReal, iZ2DistanceRef, iZData); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511
                RecordProcess(sData);
                sData.sprintf("%s Arm 2 ZRead Pos First: %d ,Second: %d", sTime, iIndexZ2HomeToZPosFirst, iIndexZ2HomeToZPosSecond); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511
                if(iZData>(IniConfig.iD63IndexZHomeToZPhaseRange))              //50->0
                {
                    ret=ShowErrorMessage("WAR0308", K_RETRY|K_SKIP, MMSystem, "Gali_FindZPhase_446");
                    if(ret==K_SKIP)
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                        {
                            iZ2DistanceRef=iZ2DistanceReal;
                            WriteIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", iZ2DistanceRef);
                            sData.sprintf("%s Skip press,and save Index_Z2_Home_Position as %d", sTime, iZ2DistanceRef);
                            SaveFile(asIndexZphasePath, sData);

                            iZ2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", 120);
                        }
                        else
                        {
                            bZ2ModifyDistanceRef=true;                          //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                        }
                        Task=447;
                    }
                    else if(ret==K_RETRY)
                    {
                        iZ2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", 120);
                        Task=1;
                    }
                }
                else
                {
                    Task=447;
                    bZ2ModifyDistanceRef=false;
                }

                if(bZ2ModifyDistanceRef==true)                                  //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                {
                    iZ2ModifyDistanceRef=iZ2DistanceReal;
                }
                else
                {
                    iZ2ModifyDistanceRef=iZ2DistanceRef;
                }

                iIndexZ2HomeToZPhaseDistanceFirst=0;
                iIndexZ2HomeToZPhaseDistanceSecond=0;
                iZ2DistanceReal=0;                                              //kevin 20170511 add log
            }
            else if(tempMOT==MTestY1)                                           //Isaac 20201110 : Index Y find motor phase
            {
                if(bCheckOverRangeY1==false)
                {
                    iY1DistanceRef=abs(iIndexY1HomeToYPhaseDistanceFirst-iIndexY1HomeToYPhaseDistanceSecond);
                    if(CheckIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position")==false)
                    {
                        WriteIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", iY1DistanceRef);
                        sData.sprintf("%s Finding phase no parameter, add Index_Y1_Home_Position=%d", sTime, iY1DistanceRef);
                        SaveFile(asIndexZphasePath, sData);
                    }
                    else
                    {
                        iY1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", 20);
                    }
                    bCheckOverRangeY1=true;
                }
                iY1DistanceReal=abs(iIndexY1HomeToYPhaseDistanceFirst-iIndexY1HomeToYPhaseDistanceSecond);
                iYData=abs(iY1DistanceReal-iY1DistanceRef);
                sData.sprintf("%s Arm 1 Yphase:First: %d ,Second: %d ,Real %d,ref %d,Yhase:%d", sTime, iIndexY1HomeToYPhaseDistanceFirst, iIndexY1HomeToYPhaseDistanceSecond, iY1DistanceReal, iY1DistanceRef, iYData); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511
                RecordProcess(sData);
                sData.sprintf("%s Arm 1 YRead Pos First: %d ,Second: %d", sTime, iIndexY1HomeToYPosFirst, iIndexY1HomeToYPosSecond); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511

                if(iYData>(IniConfig.iD63IndexZHomeToZPhaseRange))
                {
                    ret=ShowErrorMessage("WAR03314", K_RETRY|K_SKIP, MMSystem);
                    if(ret==K_SKIP)
                    {
                        bY1ModifyDistanceRef=true;                              //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                        iIndexY1FindPhaseFrontPos=Tech.iHT9040TestY1_Front;
                        iIndexY1FindPhaseMiddlePos=Tech.iHT9040TestY1_Middle;
                        sData.sprintf("%s Y1 Error Now Teaching Position Y1:Front:%d,Middle:%d", sTime, Tech.iHT9040TestY1_Front, Tech.iHT9040TestY1_Middle);
                        SaveFile(asIndexZphasePath,sData);                      //kevin 20170511

                        Task=447;
                    }
                    else if(ret==K_RETRY)
                    {
                        iY1DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", 20);
                        Task=1;
                        break;
                    }
                }
                else
                {
                    Task=447;
                    bY1ModifyDistanceRef=false;
                }

                if(bY1ModifyDistanceRef==true)                                  //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                {
                    iY1ModifyDistanceRef=iY1DistanceReal;
                }
                else
                {
                    if(iY1DistanceRef<0)
                    {
                        iY1ModifyDistanceRef=-iY1DistanceRef;
                    }
                    else
                    {
                        iY1ModifyDistanceRef=iY1DistanceRef;
                    }
                }

                iIndexY1HomeToYPhaseDistanceFirst=0;
                iIndexY1HomeToYPhaseDistanceSecond=0;
                iY1DistanceReal=0;                                              //kevin 20170511 add log
            }
            else if(tempMOT==MTestY2)
            {
                if(bCheckOverRangeY2==false)
                {
                    iY2DistanceRef = abs(iIndexY2HomeToYPhaseDistanceFirst-iIndexY2HomeToYPhaseDistanceSecond);
                    if(CheckIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position")==false)
                    {
                        WriteIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", iY2DistanceRef);
                        sData.sprintf("%s Finding phase no parameter, add Index_Y2_Home_Position=%d", sTime, iY2DistanceRef);
                        SaveFile(asIndexZphasePath, sData);
                    }
                    else
                    {
                        iY2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", 20);
                    }
                    bCheckOverRangeY2=true;
                }
                iY2DistanceReal=abs(iIndexY2HomeToYPhaseDistanceFirst-iIndexY2HomeToYPhaseDistanceSecond);
                iY2DistanceRef=abs(iY2DistanceRef);
                iYData=abs(iY2DistanceReal-iY2DistanceRef);
                sData.sprintf("%s Arm 2 Yphase:First: %d ,Second: %d ,Real %d,ref %d,Zhase:%d", sTime, iIndexY2HomeToYPhaseDistanceFirst, iIndexY2HomeToYPhaseDistanceSecond, iY2DistanceReal, iY2DistanceRef, iYData); //kevin 20170511 add log
                SaveFile(asIndexZphasePath,sData);                              //kevin 20170511
                RecordProcess(sData);
                sData.sprintf("%s Arm 2 YRead Pos First: %d ,Second: %d", sTime, iIndexY2HomeToYPosFirst, iIndexY2HomeToYPosSecond); //kevin 20170511 add log
                SaveFile(asIndexZphasePath, sData);                             //kevin 20170511

                if(iYData>(IniConfig.iD63IndexZHomeToZPhaseRange))
                {
                    ret=ShowErrorMessage("WAR03314", K_RETRY|K_SKIP, MMSystem);
                    if(ret==K_SKIP)
                    {
                        bY2ModifyDistanceRef=true;                              //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                        iIndexY2FindPhaseMiddlePos=Tech.iHT9040TestY2_Middle;
                        iIndexY2FindPhaseRearPos=Tech.iHT9040TestY2_Rear;
                        sData.sprintf("%s Y2 Error Now Teaching Position Y2:Middle:%d,Rear:%d",sTime, Tech.iHT9040TestY2_Middle, Tech.iHT9040TestY2_Rear);
                        SaveFile(asIndexZphasePath, sData);                     //kevin 20170511
                        Task=447;
                    }
                    else if(ret==K_RETRY)
                    {
                        iY2DistanceRef=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", 20);
                        Task=1;
                    }
                }
                else
                {
                    Task=447;
                    bY2ModifyDistanceRef=false;
                }

                if(bY2ModifyDistanceRef==true)                                  //Ifor 20190530 : add Z Phase 可SKIP 但需重新contact height
                {
                    iY2ModifyDistanceRef=iY2DistanceReal;
                }
                else
                {
                    if(iY2DistanceRef<0)
                    {
                        iY2ModifyDistanceRef=-iY2DistanceRef;
                    }
                    else
                    {
                        iY2ModifyDistanceRef=iY2DistanceRef;
                    }
                }

                iIndexY2HomeToYPhaseDistanceFirst=0;
                iIndexY2HomeToYPhaseDistanceSecond=0;
                iY2DistanceReal=0;                                              //kevin 20170511 add log
            }
            break;
        case 447:
            bScanFlag=false;
            if(Alias=="MTestZ1" || Alias=="MTestZ2")
            {
                if(Alias=="MTestZ1")
                    checkflag=CheckTestZ1("Gali_FindZPhase447");
                if(Alias=="MTestZ2")
                    checkflag=CheckTestZ2("Gali_FindZPhase447");

                if(checkflag)
                    Task=450;
                else
                    Task=500;
            }
            else
            {
                Task=500;
            }
            break;
        case 450:                                                               //kevin 20170515
            TestZSafePos=10;
            if(Alias=="MTestZ1" || Alias=="MTestZ2")
            {
                TestZSafePos=Prod.TestZ1_Safe;                                  //kevin 20160905
            }

            if(Gali_MotMove(TestZSafePos, Motor->PHomeHighSpeed, "Gali_FindZPhase_450"))
            {
                Task=500;
            }
            break;
        case 500:
            Task=900;
            if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)        //kevin 20170605 add
            {
                MovFlag=false;
                GaliSofDelayCount=0;
                HomeFlag=1;
            }
            return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
TQPF_Timer tGalilTwoYMoveDelay;
bool bGalilTwoYMoveFlag=false;
bool TMyMotor::GalilTwoY_Move(int iYPos, int Y2Pos, int Speed, AnsiString sFunc)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        bGalilTwoYMoveFlag=false;
        return false;
    }

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        return Gali_MotMove(iYPos, Speed, "GalilTwoY_Move"+sFunc);
    }

    Check_Y1_TargetPosInTeachPos(iYPos, sFunc);
    Check_Y2_TargetPosInTeachPos(Y2Pos, sFunc);

    bool flag[2]={false, false};
    char str[256]="";
    AnsiString SelMot=AnsiString(""), sErrFunc=__FUNC__+AnsiString(",Func2,")+sFunc, str1="";
    double sp=Speed;
    Speed=(int)sp;
    static int iOldY1=0, iOldY2=0;                                              //Sam 20220611 : 避免上一動作  bGalilTwoYMoveFlag 沒有被清除乾淨，導致偵測誤報警

    #ifndef SOFT_SIMULTE
    static int iRetryCnt=0;
    int iIndexZ1Pos=0, iIndexZ2Pos=0;
    if(CosFunction.bIndexProtect==true)                                         //Steven 20180319 (jou) : 加入Index Y軸移動前確認Z軸位置保護
    {
        iIndexZ1Pos=MOT[MTestZ1].Gali_ReadEncoderPos();
        iIndexZ2Pos=MOT[MTestZ2].Gali_ReadEncoderPos();

        if(iIndexZ1Pos<-200 || iIndexZ2Pos<-200)
        {
            if(iRetryCnt<100)
            {
                iRetryCnt++;
                return false;
            }
            bGalilTwoYMoveFlag=false;
            ShowIndexMotorError(AnsiString("GalilTwoY_Move"));
        }
        iRetryCnt=0;
        #ifdef DEBUG_GALIL_CHECK_Y
        if((iYPos>(Tech.iHT9040TestY1_Middle-7000)) &&      //JerryYang 20250822 : fix index y保護
           (Y2Pos<(Tech.iHT9040TestY2_Middle+7000)))
        {
            str1.sprintf("Index Y Position Calculate error. Y1 Target Pos: %d, Y2 Target Pos:%d, %s", iYPos, Y2Pos, __FUNC__);
            ShowMyMessage(str1);
            fAllMotorHome=false;
            return false;
        }
        #endif
    }
    #endif

    Y2Pos=-Y2Pos;
    if(Motor->Enable && bGali_CardInstall==true)
    {
        MOT[MTestY1].GetRealPos(&iYPos);
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
        {
            MOT[MTestY2].GetRealPos(&Y2Pos);
        }
        MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
        {
            MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
        }
        if(bGalilTwoYMoveFlag==false || bResetGalilTwoYMove==true)              //Steven 20210819 : Galil移動異常偵測 //Ifor 20220729 add Reset Galil Two Y Move時間計時
        {
            tGalilTwoYMoveDelay.SetSecAndOn(300);
            bGalilTwoYMoveFlag=true;
            bResetGalilTwoYMove=false;
        }
        if(MovFlag==false)
        {
            iGali_SpSpeed=Speed;
            sprintf(str,"SP%d,,,%d;AC%d,,,%d;DC%d,,,%d;PA%d,,,%d;BGXW;", Speed, Speed, GailAcSpeed2, GailAcSpeed2, GailDcSpeed2, GailDcSpeed2, iYPos, Y2Pos);

            if(ScanIndexMotorCanMove())
            {
                Gali_Command(str, sErrFunc);
            }
            else
            {
                if(tGalilTwoYMoveDelay.Off())                                   //Steven 20210819 : Galil移動異常偵測
                {
                    SelMot.sprintf("GalilTwoY_Move - ScanIndexMotorCanMove fail, iYPos=%d, Y2Pos=%d, Speed=%d, sFunc=%s", iYPos, Y2Pos, Speed, sErrFunc);
                    ShowMyMessage("GalilTwoY_Move fail", SelMot, SelMot);
                    tGalilTwoYMoveDelay.SetSecAndOn(300);
                    bGalilTwoYMoveFlag=true;
                }
                return false;
            }
            GaliSofDelayCount=0;
            bScanFlag=false;
            MovFlag=true;
        }
        else
        {
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            {
                if(iOldY1!=MOT[MTestY1].Position ||
                   iOldY2!=MOT[MTestY2].Position ||
                   bGalilTwoYMoveFlag==false)                                   //Sam 20220720 : 修正 GalilTwoY_Move 誤報警問題。
                {
                    iOldY1=MOT[MTestY1].Position;
                    iOldY2=MOT[MTestY2].Position;
                    bGalilTwoYMoveFlag=true;                                    //Sam 20220720 : 修正 GalilTwoY_Move 誤報警問題。
                    tGalilTwoYMoveDelay.SetSecAndOn(300);
                }
            }
            if(tGalilTwoYMoveDelay.Off())                                       //Steven 20210819 : Galil移動異常偵測
            {
                SelMot.sprintf("GalilTwoY_Move fail, iYPos=%d, Y2Pos=%d, Speed=%d, sFunc=%s", iYPos, Y2Pos, Speed, sErrFunc);
                ShowMyMessage("GalilTwoY_Move fail", SelMot, SelMot);
                tGalilTwoYMoveDelay.SetSecAndOn(300);
                bGalilTwoYMoveFlag=true;
            }

            if(Gali_Command("MG_BGx", sErrFunc)==1 ||                           //ChungHung 20141016 Try fix 點位飄移
               Gali_Command("MG_BGw", sErrFunc)==1)                             //馬達目前是否移動? 0:移動完成
                return false;

            if(bScanFlag==false)
            {
                MOT[MTestY1].Gali_ScanMotStatus();
                flag[0]=MOT[MTestY1].Led[iInposLed];
                if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)
                {
                    flag[1]=true;
                }
                else
                {
                    MOT[MTestY2].Gali_ScanMotStatus();
                    flag[1]=MOT[MTestY2].Led[iInposLed];
                }
            }
            if(flag[0]==false && flag[1]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    bScanFlag=false;
                    GaliSofDelayCount=0;
                    MovFlag=false;
                    bGalilTwoYMoveFlag=false;                                   //Steven 20210819 : Galil移動異常偵測
                    return true;
                }
            }
        }
    }
    else //if(Motor->Enable==false)
    {
        Speed=ArmSpeed[IndexArm].iBodySP;
        if(Speed<=0)
            Speed=10;
        MOT[MTestY1].Position+=Speed;
        MOT[MTestY2].Position-=Speed;
        if(MOT[MTestY1].Position>=iYPos || MOT[MTestY2].Position<=-Y2Pos)
        {
            MOT[MTestY1].Position=iYPos;
            MOT[MTestY2].Position=-Y2Pos;
            MOT[MTestY1].TargetPosition=iYPos;                                  //Steven 20210827 : 修改顯示Galil的Target / Current
            MOT[MTestY2].TargetPosition=-Y2Pos;
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool ScanIndexMotorCanMove()
{
    int MotorIndex[4]={MTestY1, MTestZ1, MTestZ2, MTestY2};
    for(int j=0; j<4; j++)
    {
        int i=MotorIndex[j];
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis && i==MTestY2)                   //JimmyChiu 20220708 : add Index Arm Axis
            continue;
        MOT[i].Gali_ScanAlarmStatus();
        if(MOT[i].Led[iAlarmLed] && MOT[i].Motor->Enable && MOT[i].Motor->PServoAlarmOn)
        {
            MOT[i].HomeFlag=0;
            fAllMotorHome=false;
            SystemStart=false;
            JamCode=MotorIndexToJamCode(i);
            ShowMotorErrorMessage(JamCode,1);
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool ScanIndexVectorCanMove(const char* vecCmd)                                 //AI(Copilot) 20260316 (RogerYang) : 分離前置放行與向量啟動確認
{
    if(ScanIndexMotorCanMove()==false)
        return false;

    if(vecCmd==NULL)
        return true;

    //AI(Copilot) 20260316 (RogerYang) : 改為 BG/SC + 位置變化雙條件確認，避免BG延遲回報誤判
    long lStartX=MOT[MTestY1].Gali_ReadPos();
    long lStartY=MOT[MTestZ1].Gali_ReadPos();
    long lStartZ=MOT[MTestZ2].Gali_ReadPos();
    long lStartW=(USE_INDEX_ARM_AXES==IndexArm_4_Axis) ? MOT[MTestY2].Gali_ReadPos() : 0;

    const int iStartRetry=50;                                                   //約100ms，足以覆蓋大多數PLC更新周期
    const int iPosDeadband=2;                                                   //避免雜訊導致誤判

    MOT[MTestY1].Gali_Command(vecCmd, __FUNC__);

    for(int iTry=0; iTry<iStartRetry; iTry++)
    {
//        long lSC  = MOT[MTestY1].Gali_Command("SC",     __FUNC__);             //全軸停止碼，100=異常
        long lBGx = MOT[MTestY1].Gali_Command("MG_BGx", __FUNC__);             //Y1軸運動中=1
        long lBGy = MOT[MTestY1].Gali_Command("MG_BGy", __FUNC__);             //Z1軸運動中=1
        long lBGz = MOT[MTestY1].Gali_Command("MG_BGz", __FUNC__);             //Z2軸運動中=1
        long lBGw = (USE_INDEX_ARM_AXES==IndexArm_4_Axis) ?
                    MOT[MTestY1].Gali_Command("MG_BGw", __FUNC__) : 0;         //4軸模式才讀Y2

        long lNowX=MOT[MTestY1].Gali_ReadPos();
        long lNowY=MOT[MTestZ1].Gali_ReadPos();
        long lNowZ=MOT[MTestZ2].Gali_ReadPos();
        long lNowW=(USE_INDEX_ARM_AXES==IndexArm_4_Axis) ? MOT[MTestY2].Gali_ReadPos() : 0;

        long lDiffX=lNowX-lStartX; if(lDiffX<0) lDiffX=-lDiffX;
        long lDiffY=lNowY-lStartY; if(lDiffY<0) lDiffY=-lDiffY;
        long lDiffZ=lNowZ-lStartZ; if(lDiffZ<0) lDiffZ=-lDiffZ;
        long lDiffW=lNowW-lStartW; if(lDiffW<0) lDiffW=-lDiffW;
        bool bPosMoved=(lDiffX>=iPosDeadband || lDiffY>=iPosDeadband || lDiffZ>=iPosDeadband ||
                       (USE_INDEX_ARM_AXES==IndexArm_4_Axis && lDiffW>=iPosDeadband));

        bool bBGAllStop = (lBGx==0 && lBGy==0 && lBGz==0 &&
                          ((USE_INDEX_ARM_AXES==IndexArm_4_Axis) ? (lBGw==0) : true));
        if(!bBGAllStop || bPosMoved)
            return true;

        MySleepEx(2, true);
    }

    iHome=1;
    ShowMyMessage("Vector command start check fail",
                  "向量命令送出後啟動確認失敗(BG/SC/Pos, retry 50次)，請先Home再啟動");
    return false;
}
//------------------------------------------------------------------------------
bool ScanIndexOtherMotorCanMove(int Index)
{
    return ScanIndexMotorCanMove();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void StopAllMotor(bool bIndexCanStop)
{
    if(MOT[MTestY1].Motor!=NULL &&
       bIndexCanStop==true)                                                     //Steven 20220309 : 避免Galil Command Error時, 不能Alarm
    {
        MOT[MTestY1].Gali_Command("VS0;SP0,0,0,0;", __FUNC__);
        bGalilTwoYMoveFlag=false;                                               //Sam 20220720 : 修正 GalilTwoY_Move 誤報警問題。
    }

    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(MOT[i].Motor==NULL)                                                  //2013-08-30    Dell    沒開24V造成記憶體溢位
            continue;

        if(MOT[i].Motor->Enable==true)
        {
            if(MOT[i].Motor->PServoAlarmOn==1)                                  //jou 2014-07-02 retrun to V2.12R 修正立即停止造成步進馬達失步
            {
                MOT[i].PCIL132_StopMotor();
            }
            else                                                                //Steven 20170504 (wei) : 修正在馬達工具裡面按STOP,步進馬達不會停的問題
            {
                if(SystemStart==false)
                    MOT[i].PCIL132_StopMotor();
            }
        }
    }

    if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)                       //wei 20150925 待機位置改道 Color
    {
        MOT[MLoaderY].PCIL132_StopMotor();
    }
    else
    {
        TrayMoveOut(false, 0);                                                  //Sam 20210325 : 修正整合 tray move
    }

    for(int i=eAuto1; i<=iAutoRight; i++)
    {
        int iAuto=iAutoIndex[i];
        SW[SwAutoCCW[iAuto]].Off();
        SW[SwAutoCW[iAuto]].Off();
    }

    for(int i=0; i<MAX_TRACK; i++)
       TrayMoveOut(false, i);

    //******************************************************************************
    //  注意!! 有在本Function使用到氣缸的話, 必須將Cylinder[i].bCheckSafeDoor==false, 否則進入CheckIsSafeDoorOpen會當掉
    //******************************************************************************
    Cylinder[C_HotplateVibration].Off();
    Cylinder[C_TrayVibration].Off();

    SW[SwShuttleVibration1].Off();
    SW[SwShuttleVibration2].Off();
    SW[SwLoaderVibration].Off();                                                //JerryYang 20191001 loader 震動馬達
}
//------------------------------------------------------------------------------
bool TMyMotor::ISZ1Up_Z2Up()
{
    if(MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
       MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe))
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::ISZ1Down_Z2Down()
{
    if(MOT[MTestZ1].Gali_ReadEncoderMaxRandge(iBackUpZ1DownPosition) &&
       MOT[MTestZ2].Gali_ReadEncoderMaxRandge(iBackUpZ2DownPosition))
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_nnMode_Z1Z2_Down(int Speed, bool TMode)                     //2012-10-22  Dell for HT9046LS
{
    char str[512]="";
    char tmpstr[512]="";
    static bool MOTMovflag[]={true, true, true, true};
    static int iCount=0;                                                        //kevin 20220221 add Gali  Command state  Mylin 20211228 Add Gali Command Log

    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(Motor->Enable && bGali_CardInstall==true)
    {
        GetIndexParm();                                                         //取得移動距離

        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
            {
                MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();
            }
            #ifdef DEBUG_32SITE
                fMain->StringGrid14->Cells[1][1]=MOT[MTestY1].Position;
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                {
                    fMain->StringGrid14->Cells[1][2]=MOT[MTestY2].Position;
                }
            #endif
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ("Gali_nnMode_Z1Z2_Down"))                             //確認四軸command & encoder位置是否不同
            {
                #ifdef DEBUG_32SITE
                    fMain->StringGrid14->Cells[2][1]=MOT[MTestY1].Position;
                    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                    {
                        fMain->StringGrid14->Cells[2][2]=MOT[MTestY2].Position;
                    }

                    fMain->StringGrid14->Cells[3][1]=MOT[MTestY1].Gali_ReadEncoderPos();
                    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                    {
                        fMain->StringGrid14->Cells[3][2]=MOT[MTestY2].Gali_ReadEncoderPos();
                    }
                #endif

                iGali_VsSpeed=Speed;                                            //?與 Z1UpZ2Down 不同?
                MOT[MTestZ2].GetRealPos(&Z2Safe);

                MOT[MTestY1].GetRealPos(&XShiftF);
                MOT[MTestZ1].GetRealPos(&Z1DownSafe);
                MOT[MTestZ1].GetRealPos(&Z2DownSafe);
                MOT[MTestZ2].GetRealPos(&Z2Up);
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                {
                    MOT[MTestY2].GetRealPos(&XShiftR);
                }

                MOT[MTestZ1].GetRealPos(&Z1Down);
                MOT[MTestZ1].GetRealPos(&Z2Down);

                sprintf(tmpstr, "LMXYZW;LI%d,%d,%d,%d;",
                                -XShiftF, -Z1DownSafe, -Z2DownSafe, -XShiftR);
                sprintf(str, "%s;LI0,%d,%d,0;LE;VS%d;VA%d;VD%d;BGS;",
                                tmpstr, -Z1Down, -Z2Down, Speed, GailAcSpeed, GailDcSpeed);

                bScanFlag=false;
                if(ScanIndexVectorCanMove(str))
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                MovFlag=true;
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[iInposLed]=true;
                }
            }
        }
        else
        {
            #ifdef DEBUG_32SITE
                fMain->StringGrid14->Cells[4][1]= MOT[MTestY1].Gali_ReadEncoderPos();
                if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                {
                    fMain->StringGrid14->Cells[4][2]= MOT[MTestY2].Gali_ReadEncoderPos();
                }
            #endif

            if(Gali_Command("SC")==100)                                         //ChungHung 20150831 add 確保每次皆到位
            {
                iCount++;                                                       //Mylin 20211228 Add Gali Command Log ==>
                if(iCount>5000)
                {
                    fMain->sb_Gali_LogClick();
                    iCount=0;
                    iHome =1;
                    ShowMyMessage("Gali Command Error When Z1UpZ2Down1, Please Home and Start", "請更換Galil 轉板或100P線材");
                }
                return false;
            }
            iCount=0;                                                           //kevin 20220221 add Gali  Command state Mylin 20211228 Add Gali Command Log

            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&
                       (MTestY1+i)==MTestY2)                                    //JimmyChiu 20220708 : add Index Arm Axis
                    {
                        MOTMovflag[i]=false;
                    }
                    else
                    {
                        if(MOTMovflag[i])
                        {
                            MOT[MTestY1+i].Gali_ScanMotStatus();
                            MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                        }
                    }
                }
            }

            if(MOTMovflag[0]==false && MOTMovflag[1]==false &&
               MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;

                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos_nnMode(false)==false)                           //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode = MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode,MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Down_Z2Down())                                       //check z1 & z2 encoder position
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1_Z2_Down;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode=MotorIndexToJamCode(14);
                        else
                            JamCode=MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode, MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ1].Position-=Speed;
        MOT[MTestZ2].Position-=Speed;
        MOT[MTestY1].Position+=Speed;
        MOT[MTestY2].Position-=Speed;

        MOT[MTestY1].TargetPosition=Prod.TestY1_Middle;                         //Steven 20210827 : 修改顯示Galil的Target / Current
        MOT[MTestY2].TargetPosition=Prod.TestY2_Middle;
        MOT[MTestZ1].TargetPosition=-Prod.TestZ1_Test;
        MOT[MTestZ2].TargetPosition=Prod.TestZ2_Test;

        if(MOT[MTestZ1].Position<=Prod.TestZ1_Test   || MOT[MTestZ2].Position <=Prod.TestZ2_Test ||
           MOT[MTestY1].Position<=Prod.TestY1_Middle || MOT[MTestY2].Position>=Prod.TestY2_Middle)
        {
            IndexStatus=Z1_Z2_Down;
            MOT[MTestY1].Position=Prod.TestY1_Middle;
            MOT[MTestY2].Position=Prod.TestY2_Middle;
            MOT[MTestZ1].Position=-Prod.TestZ1_Test;
            MOT[MTestZ2].Position=Prod.TestZ2_Test;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Gali_nnMode_Z1Z2_Up(int Speed, bool TMode)                       //2012-10-22  Dell for HT9046LS
{
    char str[512]="";
    char tmpstr[512]="";
    static bool MOTMovflag[]={true, true, true, true};
    static int iCount=0;                                                        //kevin 20220221 add Gali  Command state  Mylin 20211228 Add Gali Command Log

    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(Motor->Enable && bGali_CardInstall==true)
    {
        GetIndexParm();                                                         //取得移動距離

        if(bShowMotorMove==true)
        {
            MOT[MTestY1].Position=MOT[MTestY1].Gali_ReadPos();
            MOT[MTestY2].Position=MOT[MTestY2].Gali_ReadPos();

            #ifdef DEBUG_32SITE
                fMain->StringGrid14->Cells[1][3]=MOT[MTestY1].Position;
                fMain->StringGrid14->Cells[1][4]=MOT[MTestY2].Position;
            #endif
        }

        if(MovFlag==false && bZ1Z2Exute==false)                                 //kevin 20110630
        {
            if(CheckTestZ("Gali_nnMode_Z1Z2_Up"))                               //確認四軸command & encoder位置是否不同
            {
                #ifdef DEBUG_32SITE
                    fMain->StringGrid14->Cells[2][3]=MOT[MTestY1].Gali_ReadPos();
                    fMain->StringGrid14->Cells[2][4]=MOT[MTestY2].Gali_ReadPos();

                    fMain->StringGrid14->Cells[3][3]=MOT[MTestY1].Gali_ReadEncoderPos();
                    fMain->StringGrid14->Cells[3][4]=MOT[MTestY2].Gali_ReadEncoderPos();
                #endif

                iGali_VsSpeed=Speed;                                            //?與 Z1UpZ2Down 不同?
                MOT[MTestZ2].GetRealPos(&Z1Safe);
                MOT[MTestZ2].GetRealPos(&Z2Safe);

                MOT[MTestY1].GetRealPos(&XShiftF);
                MOT[MTestZ1].GetRealPos(&Z1DownSafe);
                MOT[MTestZ1].GetRealPos(&Z2DownSafe);
                MOT[MTestZ2].GetRealPos(&Z2Up);
                MOT[MTestY2].GetRealPos(&XShiftR);

                MOT[MTestZ1].GetRealPos(&Z1Down);
                MOT[MTestZ1].GetRealPos(&Z2Down);

                sprintf(tmpstr, "LMXYZW;LI0,%d,%d,0;LI%d,%d,%d,%d;",
                                -Z1Safe, -Z2Safe,                               //Steven 20160518 : 修正32site drop mode
                                XShiftF, Z1DownSafe, Z2DownSafe, XShiftR);
                sprintf(str, "%s;LE;VS%d;VA%d;VD%d;BGS;",
                             tmpstr, Speed, GailAcSpeed, GailDcSpeed);

                bScanFlag=false;
                if(ScanIndexVectorCanMove(str))
                {
                    bZ1Z2Exute=true;                                            //kevin 20110630
                }
                else
                {
                    return false;
                }

                MovFlag=true;
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    MOTMovflag[i]=true;
                    MOT[MTestY1+i].Led[8]=true;
                }
            }
        }
        else
        {
            #ifdef DEBUG_32SITE
                fMain->StringGrid14->Cells[4][3]=MOT[MTestY1].Gali_ReadEncoderPos();
                fMain->StringGrid14->Cells[4][4]=MOT[MTestY2].Gali_ReadEncoderPos();
            #endif

            if(Gali_Command("SC")==100)                                         //ChungHung 20150831 add 確保每次皆到位
            {
                iCount++;                                                       //Mylin 20211228 Add Gali Command Log ==>
                if(iCount>5000)
                {
                    fMain->sb_Gali_LogClick();
                    iCount=0;
                    iHome =1;
                    ShowMyMessage("Gali Command Error When Z1UpZ2Down1, Please Home and Start", "請更換Galil 轉板或100P線材");
                }
                return false;
            }
            iCount=0;                                                           //kevin 20220221 add Gali  Command state Mylin 20211228 Add Gali Command Log

            if(bScanFlag==false)
            {
                for(int i=0; i<Gali_MaxAxis; i++)
                {
                    if(MOTMovflag[i])
                    {
                        MOT[MTestY1+i].Gali_ScanMotStatus();
                        MOTMovflag[i]=MOT[MTestY1+i].Led[iInposLed];
                    }
                }
            }
            if(MOTMovflag[0]==false && MOTMovflag[1]==false &&
               MOTMovflag[2]==false && MOTMovflag[3]==false)
            {
                bScanFlag=true;
                GaliSofDelayCount++;
                if(GaliSofDelayCount>=DelayCount)
                {
                    if(CheckPos_nnMode(true)==false)                            //now only check TestY
                    {
                        if(GaliSofDelayCount<=(DelayCount*5))
                            return false;
                        iHome=1;
                        JamCode = MotorIndexToJamCode(13);
                        ShowMotorErrorMessage(JamCode, MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }

                    if(ISZ1Up_Z2Up())                                           //check z1 & z2 encoder position
                    {
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        bZ1Z2Exute=false;                                       //kevin 20110630

                        IndexStatus=Z1_Z2_Normal;
                        fOffSet->bEnterSpecialOffset=false;
                        return true;
                    }
                    else
                    {
                        iHome=1;
                        if(GaliAxisAlarm[1])
                            JamCode=MotorIndexToJamCode(14);
                        else
                            JamCode=MotorIndexToJamCode(15);

                        ShowMotorErrorMessage(JamCode, MotErrPos+1);
                        bScanFlag=false;
                        GaliSofDelayCount=0;
                        MovFlag=false;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        Speed=ArmSpeed[IndexArm].iBodySP*10;
        if(Speed<=0)
            Speed=10;
        MOT[MTestZ1].Position+=Speed;
        MOT[MTestZ2].Position+=Speed;
        MOT[MTestY1].Position-=Speed;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].Position+=Speed;

        MOT[MTestY1].TargetPosition=Prod.TestY1_Front;                          //Steven 20210827 : 修改顯示Galil的Target / Current
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            MOT[MTestY2].TargetPosition=Prod.TestY2_Rear;
        MOT[MTestZ1].TargetPosition=0;
        MOT[MTestZ2].TargetPosition=0;

        bool bY2PosSafe=(USE_INDEX_ARM_AXES==IndexArm_4_Axis)?(MOT[MTestY2].Position>=Prod.TestY2_Rear):true;  //JimmyChiu 20220708 : add Index Arm Axis
        if(MOT[MTestZ1].Position>=0 || MOT[MTestZ2].Position >=0 ||
           MOT[MTestY1].Position<=Prod.TestY1_Front ||
           bY2PosSafe)
        {
            IndexStatus=Z1_Z2_Normal;
            MOT[MTestY1].Position=Prod.TestY1_Front;
            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
                MOT[MTestY2].Position=Prod.TestY2_Rear;
            MOT[MTestZ1].Position=0;
            MOT[MTestZ2].Position=0;
            fOffSet->bEnterSpecialOffset=false;
            return true;
        }
        return false;
    }
    return false;
}
