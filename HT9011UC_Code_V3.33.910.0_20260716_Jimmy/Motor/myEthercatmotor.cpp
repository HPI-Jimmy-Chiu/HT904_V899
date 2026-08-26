//---------------------------------------------------------------------------
#include "MachineDefine.h"
//#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
//#include <assert.h>
//#include "IncludeAllHeader.h"
#include "myEthercatmotor.h"
#include "note.h"
//#include "uMotorTest.h"
#include "mymessbox.h"
#include "mysensor.h"
#include "common.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//-------------
//MotorType
//-------------
//const int Step_Motor  =0;
//const int Servo_Motor =1;
//const int Rotate_Motor=2;
//==============================================================================

//Acm_AxGetMotionStatus
const int Stop         =0;                //停止
const int Res1         =1;                //保留1
const int WaitERC      =2;                //等待 ERC 完成
const int Res2         =3;                //保留2
const int CorrectBksh  =4;                //補償
const int Res3         =5;                //保留3
const int InFA         =6;                //處於特定速度
const int InFL         =7;                //處於低速
const int InACC        =8;                //加速中
const int InFH         =9;                //處於最大速度中
const int InDEC        =10;               //減速中
const int WaitINP      =11;               //等待到位
//==============================================================================

unsigned int TMyEtherCatMotor::gDevhand    = NULL;
#define MAX_EtherCat_MOTOR 64
bool bAlarm=false;                                                              //RogerYang 20250421 get motor alarm state
bool bFirstClickJog=true;
//---------------------------------------------------------------------------
//HTAdvantechMotor::HTAdvantechMotor(int AxisHandle)
//---------------------------------------------------------------------------
__fastcall TMyEtherCatMotor::TMyEtherCatMotor(int addr)
{
    if(addr==-1)
    {
    }
    else
    {
        //PCordModel = "Advantech";
        //MotorID = 0; //RogerYang

        iBoardID    = addr/100;
        iPortID     = addr%100;

        MotorID=(iBoardID*10)+iPortID;

        //sprintf(cDeviceName,"%s,Board_Port:%02d_%02d","PCI1203", iBoardID, iPortID);
        cDeviceName.sprintf("%s,Board_Port:%02d_%02d","PCI1203", iBoardID, iPortID);
    }
    bAxisOpen=false; //RogerYang 20250403
}
// ------------------------------------------------------------------------

TMyEtherCatMotor::~TMyEtherCatMotor()
{
    Close_Card();
}
//-----------------------------------------------------------------
//initialize motor
//-----------------------------------------------------------------
void Open_Card()
{
/*    if (gDevhand != NULL)
        return;

    DEVLIST m_avaDevs[MAX_DEVICES];
    U32 deviceCounter = 0;
    U32 Result;

    Result = Acm_GetAvailableDevs(m_avaDevs,MAX_DEVICES,&deviceCounter);
    if(Result != SUCCESS)
    {
        ShowMessage("No Available Device In System");
        return;
    }

    ULONG  buffLen=64;
    ULONG  AxesPerDev;
    m_dwDevNum = m_avaDevs[0].dwDeviceNum;

    Result = Acm_DevOpen(m_dwDevNum,&gDevhand);
    for (int i = 0; i < 10; ++i)
    {
        if (Result != SUCCESS)
        {
            MySleep(5000);
            Result = Acm_DevOpen(m_dwDevNum,&gDevhand);
        }
        else
        {
            break;
        }
    }

    if(Result != SUCCESS)
    {
      ShowMessage("Open Device Failed With Error Code: " + IntToHex((int)Result,8));
      return;
    }
    buffLen = 4;
    Result = Acm_GetProperty(gDevhand,FT_DevAxesCount,&AxesPerDev,&buffLen);
    if(Result != SUCCESS)
    {
      ShowMessage("Can Not Get Device Axes Number With Error Code: " + IntToHex((int)Result,8));
      return;
    }

    if(AxesPerDev<=0)
        ShowMessage("There is no Motor !!");

    MotorCount=AxesPerDev;
    */
//    Result = Acm_SetU32Property(gDevhand, CFG_DevLogMsg, 0);     //for debug use

//    for(int i=0; i<AxesPerDev; i++)
//    {
//        Result = Acm_AxOpen(gDevhand,(USHORT)i, &m_Axishand[i]);
//        if(Result != SUCCESS)
//        {
//            ShowMessage("Can Cot Open Motor : "+IntToStr(i+1));
//            return;
//        }
//    }
}
//------------------------------------------------------------------
void TMyEtherCatMotor::Open_Axis()
{
    U32 Result;
    AnsiString strError;
    bool AxisCheck = true;

    if(uiDevhand==0)
        return;

    if(!bAxisOpen)
    {
        Result=Acm_AxOpenbyID(uiDevhand, iBoardID, iPortID, &m_Axishand[MotorID]);
        if(Result!=SUCCESS)
        {
            AxisCheck=false;
            strError="MotorID:" + IntToStr(MotorID) + ","+ IntToHex((int)Result,8);
            ShowErrorMessage("WAR16120", 0, MMSystem, 0, strError);
        }

        bAxisOpen=AxisCheck;
    }
}
//------------------------------------------------------------------
int TMyEtherCatMotor::InitMotor(int axIndex)
{
    U32 Result;
    AnsiString strError;
    unsigned short state=0;

    if(!Enable)
        return true;

    Open_Axis();

    if(bAxisOpen==false)
        return false;

    if(Sen[SnFrontLeftEMG].IsOff() || Sen[SnFrontRightEMG].IsOff() ||
        Sen[SnRearLeftEMG].IsOff() || Sen[SnRearRightEMG].IsOff())
    {
        ShowMyMessage("Please Unlock EMG And Restart The Software!!", "請解開EMG並重新啟動軟體!!");
        return false;
    }

    ResetMotorError:    //KaiHuang 20200928 Add
    //Reset Error
    Result=Acm_AxResetError(m_Axishand[MotorID]);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " ResetError:" + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    //確認馬達在 Ready 狀態才可寫入參數
    Result=Acm_AxGetState(m_Axishand[MotorID], &state);
    if(Result==SUCCESS)
    {
        if(state!=STA_AX_READY)
        {
            //Reset Error
            Result=Acm_AxResetError(m_Axishand[MotorID]);
            if(Result!=SUCCESS)
            {
                strError="Mot MotorID-" + IntToStr(MotorID) + " State:" + IntToStr(state);
                ShowErrorMessage("WAR16123", 0, MMSystem, 0, strError);
                goto ResetMotorError;   //KaiHuang 20200928 Add
            }
        }
    }
    else
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + "," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
        goto ResetMotorError;   //KaiHuang 20200928 Add
    }

    unsigned int PPU=1;
                                     //Setting the value of PPU
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPPU,PPU);
    if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " PPU," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    unsigned int uElReact=0;                                     //設定遇到極限點時所反應的動作  0:立即停止 1:減速停止
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxElReact, uElReact);
    if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " ElReact," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    unsigned int uAlmEnable=1;                                   //設定啟用Alarm訊號觸發 0:Disable 1:Enable
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxAlmEnable, uAlmEnable);
    if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " AlmEnable," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    unsigned int AlmReact=0;                                     //設定Alarm訊號觸發所反應的動作 0:立即停止 1:減速停止
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxAlmReact, AlmReact);
    if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " AlmReact," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    unsigned int uOrgLogic=bSensorType ? 0 : 1;                //設定原點的觸發邏輯
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxOrgLogic, uOrgLogic);
    if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " OrgLogic," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    double iMode=0;                                                             //RogerYang 20250729 move from SetSpeed to here
    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxJerk, iMode);          //RogerYang 20250729 move from SetSpeed to here
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " Jerk," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    /*板卡不支援
    unsigned int uPulseOutMode = MotorType ? 8 : 32;            //Setting of command pulse output mode. 16:CW/CCW   1:OUT/DIR
    Result=Acm_SetProperty(m_Axishand[Address],CFG_AxPulseOutMode,&uPulseOutMode,sizeof(uPulseOutMode));
    if (Result != SUCCESS)
    {
        strError="Mot Address:" + IntToStr(Address) + " PulseOutMode," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
    */

//    板卡不支援
//    unsigned int uLnpLogic = 0;
//    Result=Acm_SetProperty(m_Axishand[Address],CFG_AxInpLogic,&uLnpLogic,sizeof(uLnpLogic));                                //Setting of active logic for In-Position signal.  Low:0  High:1
//    if (Result != SUCCESS)
//    {
//        ShowMessage("Axis-" + IntToStr(Address) + " Set CFG_AxInpLogic False ! Error Code : " + IntToHex((int)Result,8));
//        return false;
//    }

//    unsigned int uPulseInMode = AB_4X;                               //Setting of encoder feedback pulse input mode.   0: 1X A/B 2: 4X
//    Result=Acm_SetProperty(m_Axishand[Address],CFG_AxPulseInMode,&uPulseInMode,sizeof(uPulseInMode));
//    if (Result != SUCCESS)
//    {
//        ShowMessage("Axis-" + IntToStr(Address) + " Set CFG_AxPulseInMode False ! Error Code : " + IntToHex((int)Result,8));
//        return false;
//    }

//    unsigned int uPulseInLogic;                               //Setting of encoder feedback pulse input logic.   0: 1X A/B 2: 4X
//    if(Direction == 1)
//        uPulseInLogic = INV_DIR;
//    else
//        uPulseInLogic = NO_INV_DIR;
//    Result=Acm_SetProperty(m_Axishand[Address],CFG_AxPulseInLogic,&uPulseInLogic,sizeof(uPulseInLogic));
//    if (Result != SUCCESS)
//    {
//        ShowMessage("Axis-" + IntToStr(Address) + " Set CFG_AxPulseInLogic False ! Error Code : " + IntToHex((int)Result,8));
//        return false;
//    }
    SetEtherCatInType();

    if(MotorType==Servo_Motor)
    {
        unsigned int uPulseInMode=AB_4X;                         //設定脈衝輸入模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseInMode, uPulseInMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseInMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uPulseOutMode=O_CW_CCW;                     //設定脈衝輸出模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseOutMode, uPulseOutMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseOutMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
    else if(MotorType==Rotate_Motor)
    {
        unsigned int uPulseInMode=I_CW_CCW;                      //設定脈衝輸入模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseInMode, uPulseInMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseInMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uPulseOutMode=OUT_DIR_ALL_NEG;              //設定脈衝輸出模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseOutMode, uPulseOutMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseOutMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
    else
    {
        unsigned int uPulseInMode=I_CW_CCW;                      //設定脈衝輸入模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseInMode, uPulseInMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseInMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uPulseOutMode=OUT_DIR_DIR_NEG;              //設定脈衝輸出模式
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxPulseOutMode, uPulseOutMode);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " PulseOutMode," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], CFG_AxMaxVel, PJogHighSpeed);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MaxVel," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], CFG_AxMaxAcc, dAcc);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MaxAcc," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], CFG_AxMaxDec, dDec);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MaxDec," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    //Reset Error
    Result=Acm_AxResetError(m_Axishand[MotorID]);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " ResetError:" + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    SetServoOn(true);
    SetCommand(0);
    SetPosition(0);
    return true;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
void TMyEtherCatMotor::Stop(void)
{
    U32 Result;
    AnsiString strError;

    Result=Acm_AxStopEmg(m_Axishand[MotorID]);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " StopEmg," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
}
//-----------------------------------------------------------------
//Deceleration
//-----------------------------------------------------------------
void TMyEtherCatMotor::DecStop()
{
    U32 Result;
    AnsiString strError;

    if(!bAxisOpen)
        return;

    Result=Acm_AxStopDec(m_Axishand[MotorID]);
    if(Result != SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " StopDec," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }

    Result=Acm_AxSetExtDrive(m_Axishand[MotorID], 0);
    if (Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " SetJogModeOff," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
        return;
    }

    bFirstClickJog=true;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Jog(Position direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::JogP()
{
    U32 Result;
    AnsiString strError;
//  if (Error())                    //if the axis's state is error
//      return false;
//  if (Busy())                     //if the axis's state is busy
//      return false;

//    if(Direction)
//        Result = Acm_AxMoveVel(m_Axishand[Address],0); //To command axis to make a never ending movement with a specified velocity.
//    else
//        Result = Acm_AxMoveVel(m_Axishand[Address],1);
//    if (Result != 0)
//        return false;

    if(bFirstClickJog==true)
    {
        //Enable or disable external drive mode.Setting 1 is jog mode.
        Result=Acm_AxSetExtDrive(m_Axishand[MotorID], 1); //只有 Jog 時要切換成 Jog Mode
        if (Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " SetJogModeOn," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
            return false;
        }
        else
        {
            bFirstClickJog=false;
        }
    }

    if(Direction)
        Result=Acm_AxJog(m_Axishand[MotorID],1);
    else
        Result=Acm_AxJog(m_Axishand[MotorID],0);

    if (Result!=SUCCESS)
    {
        return false;
    }

    return true;
}
//-----------------------------------------------------------------
//Jog(Negative direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::JogN(void)
{
    U32 Result;
    AnsiString strError;
//  if (Error())                    //if the axis's state is error
//      return false;
//  if (Busy())                     //if the axis's state is busy
//      return false;
//    if(Direction)
//        Result = Acm_AxMoveVel(m_Axishand[Address],1); //To command axis to make a never ending movement with a specified velocity.
//    else
//        Result = Acm_AxMoveVel(m_Axishand[Address],0);
//    if (Result != 0)
//        return false;

    //Enable or disable external drive mode.Setting 1 is jog mode.

    if(bFirstClickJog==true)
    {
        Result=Acm_AxSetExtDrive(m_Axishand[MotorID], 1);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " SetJogModeOn," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
            return false;
        }
        else
        {
            bFirstClickJog=false;
        }
    }

    if(Direction)
        Result=Acm_AxJog(m_Axishand[MotorID],0);
    else
        Result=Acm_AxJog(m_Axishand[MotorID],1);
    if (Result!=SUCCESS)
    {
        return false;
    }

    return true;
}
//-----------------------------------------------------------------
//PTP motion(unit: 1/100mm)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::G00(int iPos)
{
    int p;
    double r;
    p=iPos;
    r=GearRatio;
    if(r==0)
        return false;
    int p1,p2;
    p1=p/r;        // unit: 1/100mm -> pulse
    p2=p1*r;
    if(p2<p){      //Compensate
        while(1){
            p1++;
            p2=p1*r;
            if(p2>=p) break;
        }
    }
    else if(p2>p){
        while(1){
            p1--;
            p2=p1*r;
            if(p2<=p) break;
        }
    }
    return RealG00(p1);
}
//-----------------------------------------------------------------
//Setting value of range
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetRange(unsigned int a)
{
    if(a>1000)
        a=1000;
    Range=a;
}
//-----------------------------------------------------------------
//Setting value of AxAcc and AxDec
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetRate(unsigned int a)
{
    double Rate;
    U32 Result;
    AnsiString strError;

    Rate=a;
    dAcc=(PJogHighSpeed-InitSpeed)*Rate/8000000.;  // 8 x 10^6      // 2011.11.28 , Joye
    if(dAcc==0)
        return;

    Rate=(iSpeed*Range-InitSpeed*Range)/dAcc;

    unsigned int iAccPersent,iAccMax,iAccMin;

    iAccPersent=(iSpeed*Range)/65535;
    if(iAccPersent==0)
        iAccPersent = 1;

    iAccMin=iAccPersent*2001;
    iAccMax=iAccPersent*8192000;

    if(Rate<iAccMin)
        Rate=iAccMin;
    if(Rate>iAccMax)
        Rate=iAccMax;

//    double Rate = (double)(4000000 / a * PRange);               //unit:pps/sec    Rate = (4℅□10■^6)/a℅PRange
    Result=Acm_SetF64Property(m_Axishand[MotorID],PAR_AxAcc,Rate);   //Set acceleration of this axis
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " Acc," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
    Result=Acm_SetF64Property(m_Axishand[MotorID],PAR_AxDec,Rate);   //Set deceleration of this axis
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " Dec," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
}
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetSpeed(unsigned int x, bool bSetJog)  //Input 進來是 %數   ==> //RogerYang 20250402 not percent, it's joghighspeed
{
    double iSpeed1,iSpeed2,persent;
    U32 Result;
    AnsiString strError;

    iSpeed=x;
    if(PJogHighSpeed==0)
        return;
    persent=(double)x/PJogHighSpeed*1.0;      //RogerYang 20250402 add
    //persent=(double)x/100.0;
    if(persent>1.0)
        persent=1.0;
    else if(persent<0.01)
        persent=0.01;

    if(!bAxisOpen)
        return;

    if(MotorType==Step_Motor)  //步進用太小顆沒辦法跑太快,從上層改太麻煩(單位不同)
    {
        iSpeed1=InitSpeed*persent;
        iSpeed2=PJogHighSpeed*persent;

        if(iSpeed2<iSpeed1)
            iSpeed2=iSpeed1;

    //        iSpeed1=InitSpeed/100.0;
//        iSpeed2=PJogHighSpeed/10.0*persent;
//        if(iSpeed2>0.5)
//            iSpeed2=0.5;  //最快只能0.5
    }
    else
    {
        iSpeed1=InitSpeed*persent;
        iSpeed2=PJogHighSpeed*persent;
        if(iSpeed2<iSpeed1)
            iSpeed2=iSpeed1;
    }

//    if(x>6553500/Range)                                                      //DL單軸最大可達9.8Mpps//
//        x=6553500/Range;                                                     //DF單軸最大可達6.5Mpps//
//    else if(x==0)
//        x=1;
//    iSpeed1=InitSpeed*Range;
//    iSpeed2=iSpeed*Range;

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxVelLow, iSpeed1);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " VelLow," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxVelHigh, iSpeed2);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " VelHigh," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxAcc, dAcc);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " Acc," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxDec, dDec);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " Dec," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

//    iMode=0;
//    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxJerk, iMode);
//    if(Result!=SUCCESS)
//    {
//        strError="Mot MotorID-" + IntToStr(MotorID) + " Jerk," + IntToHex((int)Result,8);
//        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
//    }

    if(bSetJog==true)
    {
        Result = Acm_SetF64Property(m_Axishand[MotorID], CFG_AxJogVelLow, iSpeed1);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " JogVelLow," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
        Result = Acm_SetF64Property(m_Axishand[MotorID], CFG_AxJogVelHigh, iSpeed2);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " JogVelHigh," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
        Result = Acm_SetF64Property(m_Axishand[MotorID], CFG_AxJogAcc,dAcc);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " JogAcc," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
        Result = Acm_SetF64Property(m_Axishand[MotorID],CFG_AxJogDec,dDec);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " JogDec," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
        Result =Acm_SetI32Property(m_Axishand[MotorID],CFG_AxJogVLTime,0);
        if (Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " JogVLTime," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
}
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetInitSpeed(unsigned int x)
{
    InitSpeed=x;
}
//-----------------------------------------------------------------
int TMyEtherCatMotor::SetPosition(int p)
{
    U32 Result;
    AnsiString strError;

    Result=Acm_AxSetActualPosition(m_Axishand[MotorID], (double)p);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " ActualPosition," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
    return true;
}
//-----------------------------------------------------------------
int TMyEtherCatMotor::SetCommand(int p)
{
    U32 Result;
    AnsiString strError;
    if(Enable)
    {
        Result=Acm_AxSetCmdPosition(m_Axishand[MotorID],(double)p);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " CmdPosition," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
    return Result;
}
//-----------------------------------------------------------------
//Setting the value of software limit.(unit: pulse)
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetSoftLimit(int iPLimit,int iNLimit)
{
    int LP,LN;
    U32 Result;
    AnsiString strError;

    if(Direction)
    {
        LP= -iNLimit;
        LN= -iPLimit;
    }
    else
    {
        LP= iPLimit;
        LN= iNLimit;
    }
    //Result=Acm_SetProperty(m_Axishand[MotorID], CFG_AxSwPelValue, &LP, sizeof(LP));
    Result=Acm_SetF64Property(m_Axishand[MotorID], CFG_AxSwPelValue, LP);//Sam 20211019 : 新版韌體版本需要使用此函式不然會出錯，研華孟學說可以相容舊版韌體
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " SwPelValue," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    //Result=Acm_SetProperty(m_Axishand[MotorID], CFG_AxSwMelValue, &LN,sizeof(LN));
    Result=Acm_SetF64Property(m_Axishand[MotorID], CFG_AxSwMelValue, LN);  //Sam 20211019 : 新版韌體版本需要使用此函式不然會出錯，研華孟學說可以相容舊版韌體
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " SwMelValue," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
}
//-----------------------------------------------------------------
//Enable/disable Alarm function.
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetServoAlarmOn(bool Value)
{
    PServoAlarmOn=Value;
}
//-----------------------------------------------------------------
//Getting the value of command position(unit: 1/100mm)
//-----------------------------------------------------------------
int TMyEtherCatMotor::ReadPos()
{
    return ReadRealPos() * GearRatio;           //pulse -> 1/100mm
}
//-----------------------------------------------------------------
//Getting the value of actual position(unit: 1/100mm)
//-----------------------------------------------------------------
//Getting signal of alarm
//-----------------------------------------------------------------
bool TMyEtherCatMotor::GetAlarm(void)
{
    U32 Result;
    AnsiString strError;
    unsigned long Status = 0;

    Result=Acm_AxGetMotionIO(m_Axishand[MotorID],&Status);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MotionIO," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
        return false;
    }

    if((Status&0x3004e)==0)    //110000000001001110  SLMT_P﹜SLMT_N﹜EMG﹜LMT+﹜LMT-﹜ALM
        return false;
    else
        return true;
}
//-----------------------------------------------------------------
//Getting the status of motor
//-----------------------------------------------------------------
void TMyEtherCatMotor::ScanMotorStatus( bool *Led )
{
    U32 Result;
    AnsiString strError;
    unsigned long Status=0;
    unsigned short Status_1;

    if(Enable)
    {
        Result=Acm_AxGetMotionIO(m_Axishand[MotorID],&Status);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " MotionIO," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
            return;
        }

        bool Flag   =(((Status>>4) & 0x1) > 0) ? true : false;     //ORG
        Led[iCcwLed]=(((Status>>2) & 0x1) > 0) ? true : false;     //LMT+
        Led[iCwLed] =(((Status>>3) & 0x1) > 0) ? true : false;     //LMT-

//        if(bSensorType)
//            Led[iHomeLed]=Flag;        // Home
//        else
//            Led[iHomeLed]=!Flag;    // Home
        Led[iHomeLed]=Flag;                                                     //RogerYang 20250425 because set CFG_AxOrgLogic by SensorType when initial

        //Led[iAlarmLed] = GetAlarm();
        Led[iSoftcwLed] =(((Status>>16) & 0x1) > 0) ? true : false;    //SLMT_P
        Led[iSoftccwLed]=(((Status>>17) & 0x1) > 0) ? true : false;    //SLMT_N
        Led[iAlarmLed]  =(((Status>>1)  & 0x1) > 0) ? true : false;    //ALM
        //Led[iInposLed]  =(((Status>>13) & 0x1) > 0) ? false : true;    //INP  //RogerYang 20250421 not work

        Led[iServoOn]   =(((Status>>14) & 0x1) > 0) ? true : false;
        Led[iEmgLed]    =(((Status>>6)  & 0x1) > 0) ? true : false;    //EMG

        Result=Acm_AxGetState(m_Axishand[MotorID],&Status_1);
        if(Result==SUCCESS)
        {
            if(Status_1==STA_AX_ERROR_STOP)
                Led[iAlarmLed]=true;    //iServoOn 改 iAlarmLed
        }
    }
    else
    {
        Led[iHomeLed] =true;
        Led[iInposLed]=false;
    }
    bAlarm=Led[iAlarmLed];                                                      //RogerYang 20250421 get motor alarm state
}
//-----------------------------------------------------------------
//Getting the flag of home
//-----------------------------------------------------------------
bool TMyEtherCatMotor::HomeFlag(void)
{
//    if(bSensorType)
//    {
//        if(GetHomeIO())
//            return(true);
//        else
//            return(false);
//    }
//    else
//    {
//        if(GetHomeIO())
//            return(false);
//        else
//            return(true);
//    }

    if(GetHomeIO())                                                             //RogerYang 20250425 because set CFG_AxOrgLogic by SensorType when initial
        return(true);
    else
        return(false);
}
//-----------------------------------------------------------------
//----------------------HTAdvantechMotor Only-------------------------
//-----------------------------------------------------------------
//PTP motion(unit: pulse)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::RealG00(int p)
{
    int a;
    U32 Result;
    AnsiString strError;

    if(Error())
        return(false);         //if the axis's state is error
    if(Busy())
        return(false);         //if the axis's state is busy
    /*
    if(bAxisID == 0 || bAxisID == 1 || bAxisID == 8 || bAxisID == 9 || bAxisID == 16 || bAxisID == 17)
    {
        a=p-ReadeEnCoderRealPos();                //Get relative displacement amount
        if(a < 50 && a > -50)
            return(true);
    }
    else
    {
        a=p-ReadRealPos();                //Get relative displacement amount
        if(a==0)
            return(true);
    }
    */
    a=p-ReadRealPos();
    if(a==0)
        return(true);
    if(Direction)
    {
        Result=Acm_AxMoveAbs(m_Axishand[MotorID], p);  //Start single axis' relative motion
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " MoveAbs," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
            return false;
        }
    }
    else
    {
        Result=Acm_AxMoveAbs(m_Axishand[MotorID],-p);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " MoveAbs," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
            return false;
        }
    }

    /*if (GetAlarm())
        Alarm->Set(ALM_MOTOR_MOVE);*/

    return(false);
}
//-----------------------------------------------------------------
//Getting the value of command position(unit: pulse)
//-----------------------------------------------------------------
int TMyEtherCatMotor::ReadRealPos(void)
{
    double Pos = 0;
    U32 Result;
    AnsiString strError;

    if(Enable==false) //RogerYang 2050410 add
    return 0;

    Result=Acm_AxGetCmdPosition(m_Axishand[MotorID],&Pos);       //Get current command position of the specified axis
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " CmdPos," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
    }

    if(Direction)
        Pos=-Pos;

    return Pos;
}
//-----------------------------------------------------------------
//Getting the value of actual position(unit: pulse)
//-----------------------------------------------------------------

//Getting signal of axis busy
//-----------------------------------------------------------------
bool TMyEtherCatMotor::Busy(void)
{
    unsigned short state = 0;
    U32 Result;
    AnsiString strError;

    Result=Acm_AxGetState(m_Axishand[MotorID],&state);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " GetState," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
    }

    if(state==STA_AX_READY)
        return false;
    else
        return true;
}
//-----------------------------------------------------------------
//Getting signal of axis error
//-----------------------------------------------------------------
bool TMyEtherCatMotor::Error(void)
{
    unsigned short state = 0;
    U32 Result;
    AnsiString strError;

    Result=Acm_AxGetState(m_Axishand[MotorID],&state);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " GetState," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0,strError);
    }

    if(state==STA_AX_ERROR_STOP)
    {
//        Result=Acm_AxResetError(m_Axishand[Address]);
//        if (Result != 0)
//            return true;
        return true;
    }
    return false;
}
//-----------------------------------------------------------------
//Getting signal of org/home
//-----------------------------------------------------------------
bool TMyEtherCatMotor::GetHomeIO(void)
{
    unsigned long Status = 0;
    U32 Result;
    AnsiString strError;

    Result=Acm_AxGetMotionIO(m_Axishand[MotorID],&Status);   //Get current command position of the specified axis
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MotionIO," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
        return false;
    }

    return (((Status>>4) & 0x1) > 0) ? true : false;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::DoHome(unsigned int mode, unsigned int dir)
{
//    unsigned int result = 0;
    if(GearRatio==0)
        return false;
    double crossDistance = 100/GearRatio; // 1cm
    switch (iHomeObjectTask)
    {
        case 1:
            if (Error())                            //if the axis's state is error
                return false;
            if (Busy())                             //if the axis's state is busy
                return false;

            Acm_SetProperty(m_Axishand[MotorID], PAR_AxHomeCrossDistance, &crossDistance, sizeof(crossDistance));
            SetSpeed(PHomeHighSpeed);               //Set velHigh of home motion
            SetInitSpeed(PHomeLowSpeed);            //Set velLow of home motion
            SetSoftLimit(999999,-999999);           //like disable soft limit
            Acm_AxHome(m_Axishand[MotorID],mode,dir);  //MODE12_AbsSearchReFind  1:Negative direction
            iHomeObjectTask=2;
            return false;
        case 2:
            if (Busy())
                return false;
            MySleep(300);
            //SetPos(0);                              //The command position set to 0
//            SetEnCoderPos(0);                       //The actual position set to 0
            SetSpeed(ReadSpeed());                  //Set last velHigh
            SetInitSpeed(ReadInitSpeed());          //Set last velLow
            SetSoftLimit(PSoftLimitP/GearRatio,PSoftLimitN/GearRatio);//Set last soft limit
            return true;
    }
    return false;
}

//-----------------------------------------------------------------
//Home motion(Position direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::PosDirectHome()
{
    return DoHome(11, 0);  //MODE12_AbsSearchReFind  1:Negative direction
}
//-----------------------------------------------------------------
//Home motion(Negative direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::NegDirectHome()
{
    return DoHome(11, 1);  //MODE12_AbsSearchReFind  1:Negative direction
}

//-----------------------------------------------------------------
//Home motion(Position direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::PosDirectHomeOneTouch()
{
    return DoHome(6, 0);   //MODE7_AbsSearch  0:Positive direction
}
//-----------------------------------------------------------------
//Home motion(Negative direction)
//-----------------------------------------------------------------
bool TMyEtherCatMotor::NegDirectHomeOneTouch()
{
    return DoHome(6, 1);   //MODE7_AbsSearch  0:Positive direction
}
//-----------------------------------------------------------------
//bool HT8040Motor::HomeType90()
//{
//}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::AddAxis(int iAxis)
{
    //unsigned int result;
    //result = Acm_GpAddAxis(&m_Gphand,m_Axishand[iAxis]);
    return 0;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::AddPath(int iAxis)
{
    return true;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::RunPath(void)
{
    return true;
}
//-----------------------------------------------------------------
int  TMyEtherCatMotor::ethercat_set_output_bit(int Ring, int IP, int Port, int Bit, int Data)
{
    U32 Result;
    U16 ChannelID=Port * 8 + Bit;

    Result=Acm_DaqDoSetBit(gDevhand, ChannelID, Data);    // 0: Off, 1: On
//    Result=Acm_DaqDoSetByteEx(m_Devhand,Ring,IP,ChannelID,Data);

    if (Result!=SUCCESS)
        return -1;
    return 1;
}
//-----------------------------------------------------------------
int TMyEtherCatMotor::ethercat_set_output_byte(int Ring, int IP, int Port, int Byte)
{
    U32 Result;
//    U16 ChannelID = Port * 8 + Byte;

    Result=Acm_DaqDoSetByte(gDevhand, Port, Byte);
    if(Result!=SUCCESS)
        return -1;
    return 1;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::IOBitOff(int port, int bit)
{
    U32 Result;
    U16 ChannelID=port * 8 + bit;

    Result=Acm_DaqDoSetBit(gDevhand, ChannelID, 0);    // 0: Off, 1: On
    if (Result!=0)
        return false;
    return true;
}

//-----------------------------------------------------------------
void TMyEtherCatMotor::SetAcc(double a)
{
    dAcc=a;
}
//-----------------------------------------------------------------
void TMyEtherCatMotor::SetDec(double a)
{
    dDec=a;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::MotionDone()                                                  // motion done //
{
    U16 state=0;
    U32 Result;
    AnsiString strError;

    if(Enable)
    {
        Result=Acm_AxGetState(m_Axishand[MotorID], &state);
        if(Result!=SUCCESS)
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " GetState," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
        }

        if(state==STA_AX_READY)
            return true;
        else
            return false;
    }
    else
        return false;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::MoveTo(int Tar)                                            // Move to //
{
    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    int iPos,iP1,iP2;
    iPos=ReadPos();
    if(GearRatio==0)
        return false;
    iP1=Tar/GearRatio;        // unit: 1/100mm -> pulse
    iP2=iP1*GearRatio;

    if(iP2<Tar)
    {
        while(1)
        {
            iP1++;
            iP2=iP1*GearRatio;
            if(iP2>=Tar)
                break;
        }
    }
    else if(iP2>Tar)
    {
        while(1)
        {
            iP1--;
            iP2=iP1*GearRatio;
            if(iP2<=Tar)
                break;
        }
    }

    if(Direction)
        iP1=-iP1;
    else
        iP1=iP1;

    SetSpeed(iSpeed);

    U32 Result;
    AnsiString strError;
//    Result=Acm_AxMoveAbs(m_Axishand[Address],iP1);
    Result=Acm_AxMoveRel(m_Axishand[MotorID], iP1);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MoveAbs," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::HomeObject()
{
    return EtherCatMotHome();
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::EtherCatMotHome()
{
    int &Task=iHomeObjectTask;
    unsigned short state=0;
    U32 Result;
    AnsiString strError;

    switch(Task)
    {
        case 1:
        {
            if(!MotionDone())
            {
                DecStop();
                break;
            }
//            SetCommand(0);    //KaiHuang 202010121 Mark : Servo Off 時會造成 Error
//            SetPosition(0);   //KaiHuang 202010121 Mark : Servo Off 時會造成 Error
            DecStop();          //KaiHuang 202010121 Add
            InitMotor(MotorID);
            SetHomeSpeed();
            OldSpeed=iSpeed;
//            DecStop();        //KaiHuang 202010121 Mark

            if(GearRatio==0)
                break;
            double crossDistance = 100/GearRatio; // 撞到 Home 點滑行距離
            Acm_SetProperty(m_Axishand[MotorID], PAR_AxHomeCrossDistance, &crossDistance, sizeof(crossDistance));
            SetSoftLimit(999999,-999999);           //like disable soft limit

            if(HomeDirection)
            {
                Result=Acm_AxMoveHome(m_Axishand[MotorID], MODE12_AbsSearchReFind, 0);                //MODE12_AbsSearchReFind : Home Mode,EtherCat 控制兩段回Home
                if(Result!=SUCCESS)
                {
                    strError="Mot Address-" + IntToStr(MotorID) + " Home," + IntToHex((int)Result,8);
                    ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
                }
            }
            else
            {
                Result=Acm_AxMoveHome(m_Axishand[MotorID],MODE12_AbsSearchReFind,1);
                if(Result!=SUCCESS)
                {
                    strError="Mot Address-" + IntToStr(MotorID) + " Home," + IntToHex((int)Result,8);
                    ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
                }
            }

            Task=10;
            break;
        }
        case 10:
        {
            Result=Acm_AxGetState(m_Axishand[MotorID],&state);
            if(Result!=SUCCESS)
            {
                strError="Mot MotorID-" + IntToStr(MotorID) + " GetState," + IntToHex((int)Result,8);
                ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
            }

            if(state==STA_AX_READY)
            {
                HomeDelay.SetSecAndOn(0.3);
                Task=20;
            }
            break;
        }
        case 20:
        {
            if(HomeDelay.Off())
            {
                LastHomePos=-ReadPos();
                SetCommand(0);
                MySleep(10);     //RogerYang 20250528 100->10
                SetPosition(0);
                SetSpeed(OldSpeed);
                SetSoftLimit(PSoftLimitP, PSoftLimitN);

                Task=1;
                return true;
            }
            break;
        }
        default:
            Task=1;
            return false;
    }
    return false;
}
//-----------------------------------------------------------------
bool TMyEtherCatMotor::ResetPos(int Pulse=0)
{
    long iR1 ,iR2;

    iR1=SetCommand(Pulse);
    iR2=SetPosition(Pulse);

    if ((iR1==0) && (iR2==0))
        return true;
    return false;
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::MotOutputOn(int iOutport)
{
    U32 Result;
    U32 Result2;
    AnsiString strError;

    if(bAlarm==true)                                                            //RogerYang 20250421 reset alarm before servo on
    {
        Result2=Acm_AxResetError(m_Axishand[MotorID]);
        MySleep(100);
        if(Result2!=SUCCESS)
        {
        }
    }
    Result=Acm_AxSetSvOn(m_Axishand[MotorID], 1);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " SvOn," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::MotOutputOff(int iOutport)
{
    U32 Result;
    AnsiString strError;

    Result=Acm_AxSetSvOn(m_Axishand[MotorID], 0);
    if(Result != SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " SvOn," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::SetServoOn(bool IsOn)
{
    if(Enable==false)
        return;

    if(IsOn)
    {
        MotOutputOn(1);
        //Acm_AxSetSvOn(axisHand, 1);
    }
    else
    {
        MotOutputOff(1);
        //Acm_AxSetSvOn(axisHand, 0);
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::MotInputStatus(bool *bInputPort)
{
    ;
}
//--------------------------------------------------------------------------
bool TMyEtherCatMotor::LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag)
{
    /*
    if(bFlag==false)
    {
        short InterAxis;
        short InterType=1; // line interpolation
        short BankNum=1;

        short AxisNo=1;
        short MotionType=4;
        InterAxis = 0x03;

        SmcWSetBankNumber       (Id, AxisNo,    BankNum);
        SmcWSetBankInterpolation(Id, AxisNo,    BankNum, InterType, InterAxis, Range);
        SmcWSetBankDistance     (Id, AxisNo,    1,       lPos[0]);
        SmcWSetBankDistance     (Id, AxisNo+1,  1,       lPos[1]);
        SmcWSetBankStartSpeed   (Id, AxisNo,    1,       iSpeed*Range);
        SmcWSetBankTargetSpeed  (Id, AxisNo,    1,       iSpeed*Range);
        SmcWSetBankAccelTime    (Id, AxisNo,    1,       dAcc);
        SmcWSetBankDecelTime    (Id, AxisNo,    1,       dDec);
        SmcWSetBankSSpeed       (Id, AxisNo,    1,       200);
        SmcWSetBankReady        (Id, AxisNo,    MotionType);
        SmcWMotionStart         (Id, AxisNo);
    }
    else
    {
        if(MotionDone())
            return true;
    } */
    return false;
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::EnableTrigger(bool bFlag,int iMode,long lValue)
{
    /*HWND hWnd=FindWindow(NULL, "MotorTest");
    if(bFlag)
    {
        long lPitch=lValue/GearRatio;
        if(Direction)
            lPitch=-lPitch;
        else
            lPitch=lPitch;
        if(iMode)
        {
            SmcWSetCtrlTypeOut  (Id, iPortID, 0x03, 0x00, 0x00);
            SmcWCountEvent      (Id, iPortID, hWnd, 0x01, 0x00, lPitch);
        }
        else
        {
            SmcWSetCtrlTypeOut  (Id, iPortID, 0x04, 0x00, 0x00);
            SmcWCountEvent      (Id, iPortID, hWnd, 0x01, 0x01, lPitch);
        }
        SmcWSetInitParam        (Id, iPortID);
    }
    else
    {
        SmcWSetCtrlTypeOut  (Id, iPortID, 0x03, 0x00, 0x00);
        SmcWStopEvent       (Id, iPortID, hWnd, 1);
        SmcWSetInitParam    (Id, iPortID);
    }*/
    ;
}
//--------------------------------------------------------------------------
bool TMyEtherCatMotor::MoveToPos(int Tar)
{
//    unsigned short Status=0;
    U32 Result;
    AnsiString strError;

    if(!bAxisOpen)
        return false;

    if(!MotionDone())
        return false;

    int iPos;
    iPos=ReadPos();

    //SetSpeed(iSpeed);   //RogerYang 20250715 TestSpeed

    Result=Acm_AxMoveAbs(m_Axishand[MotorID], Tar);
//    Result=Acm_AxMoveRel(m_Axishand[MotorID],Tar);

    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " MoveAbs," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::ResetLatch()
{
}
//--------------------------------------------------------------------------
int TMyEtherCatMotor::GetLatchTotalLen()
{
    return 0;
}
//--------------------------------------------------------------------------
int TMyEtherCatMotor::GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable)
{
    return 0;
}
//--------------------------------------------------------------------------
bool TMyEtherCatMotor::GetLatchIOStatus(unsigned short LatchInputNo)
{
    return false;
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable)
{
}
//--------------------------------------------------------------------------
int TMyEtherCatMotor::ReadEnCoderRealPos()
{
    double Pos = 0;
    U32 Result;
    AnsiString strError;

    if(Enable==false) //RogerYang 2050410 add
        return 0;

    Result=Acm_AxGetActualPosition(m_Axishand[MotorID],&Pos);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " CmdPos," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16121", 0, MMSystem, 0, strError);
    }

    if(Direction)
        Pos=-Pos;

    return Pos;
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::SetEtherCatInType()
{
    U32 Result;
    AnsiString strError;

    if(MotorType==Servo_Motor)
    {
        unsigned int uLnpEnable=1;                  //設定啟用INP訊號觸發 0:Disable 1:Enable
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxInpEnable, uLnpEnable);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " InpEnable," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uInpLogic=0;                   //設定INP訊號觸發 0:低準為 1:高準為
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxInpLogic, uInpLogic);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " InpLogic," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uAlmLogic=bIn1Logic? 0:1;      //設定Alarm訊號觸發 0:低準為 1:高準為
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxAlmLogic, uAlmLogic);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " AlmLogic," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
    else if(MotorType==Rotate_Motor)
    {
        unsigned int uLnpEnable=1;                  //設定啟用INP訊號觸發 0:Disable 1:Enable
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxInpEnable, uLnpEnable);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " InpEnable," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uInpLogic=0;                   //設定INP訊號觸發 0:低準為 1:高準為
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxInpLogic, uInpLogic);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " InpLogic," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }

        unsigned int uAlmLogic=0;                  //設定Alarm訊號觸發 0:低準為 1:高準為
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxAlmLogic, uAlmLogic);
        if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " AlmLogic," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }
    else
    {
        //鳴治馬達不能用以下兩個會像賽
//        unsigned int uLnpEnable=1;                  //設定啟用INP訊號觸發 0:Disable 1:Enable
//        Result=Acm_SetU32Property(m_Axishand[Address],CFG_AxInpEnable,uLnpEnable);
//        if ((Result != SUCCESS) && (Result != Dsp_PropertyIDNotSupport))
//        {
//            strError="Mot Address-" + IntToStr(Address) + " InpEnable," + IntToHex((int)Result,8);
//            ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
//        }
//
//        unsigned int uInpLogic=0;                   //設定INP訊號觸發 0:低準為 1:高準為
//        Result=Acm_SetU32Property(m_Axishand[Address],CFG_AxInpLogic,uInpLogic);
//        if ((Result != SUCCESS) && (Result != Dsp_PropertyIDNotSupport))
//        {
//            strError="Mot Address-" + IntToStr(Address) + " InpLogic," + IntToHex((int)Result,8);
//            ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
//        }

        unsigned int uAlmLogic=bIn1Logic? 0:1;      //設定Alarm訊號觸發 0:低準為 1:高準為
        Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxAlmLogic, uAlmLogic);
        if ((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
        {
            strError="Mot MotorID-" + IntToStr(MotorID) + " AlmLogic," + IntToHex((int)Result,8);
            ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
        }
    }

    unsigned int uEzLogic=1;                        //設定EZ訊號觸發 0:低準為 1:高準為
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxEzLogic, uEzLogic);
    if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " EzLogic," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }

    unsigned int uErcLogic=1;                       //設定ERC訊號觸發 0:低準為 1:高準為
    Result=Acm_SetU32Property(m_Axishand[MotorID], CFG_AxErcLogic, uErcLogic);
    if((Result!=SUCCESS) && (Result!=Dsp_PropertyIDNotSupport))
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " ErcLogic," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
}
//--------------------------------------------------------------------------
void EtherCatWriteAO(int ID, int Channel, F32 Data)
{
    U32 Result;

    Result=Acm_DaqAoSetCurrDataEx(uiDevhand, 1, ID, Channel, Data);             //IO_Ring=1
    if(Result!=SUCCESS)
    {
        ;
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::SetHomeSpeed()
{
    U32 Result;
    AnsiString strError;

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxHomeVelLow, PHomeLowSpeed);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " HomeVelLow," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxHomeVelHigh, PHomeHighSpeed);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " HomeVelHigh," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxHomeAcc, dAcc);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " HomeAcc," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxHomeDec, dDec);
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " HomeDec," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }

    Result=Acm_SetF64Property(m_Axishand[MotorID], PAR_AxHomeJerk, 0);        //T型曲線
    if(Result!=SUCCESS)
    {
        strError="Mot MotorID-" + IntToStr(MotorID) + " HomeJerk," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::Close_Card()
{
    U32 Result;
    AnsiString strError;

    if(uiDevhand==0) //避免重負關
        return;

    Result=Acm_DevClose(&uiDevhand);
    if(Result!=SUCCESS)
    {
        strError="Close Card," + IntToHex((int)Result,8);
        ShowErrorMessage("WAR16122", 0, MMSystem, 0, strError);
    }
    else
    {
        uiDevhand=0;
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::ResetState()
{
    U32 Result;
    Result=Acm_AxResetError(m_Axishand[MotorID]);
    if(Result!=SUCCESS)
    {
//        strError="Mot MotorID-" + IntToStr(MotorID) + " ResetError:" + IntToHex((int)Result,8);
//        ShowErrorMessage("WAR16122", 0, MMSystem, 0,strError);
    }
}
//--------------------------------------------------------------------------
void TMyEtherCatMotor::ResetAxisOpen()
{
//    for(int i=0; i<999; i++)    //重開卡須初始化, 不然不會開馬達    //RogerYang 20250402 9046AU
//        bAxisOpen[i]=false;
    bAxisOpen=false;
}
//--------------------------------------------------------------------------
