#include "MachineDefine.h"
#pragma hdrstop

#include "MyMotor.h"

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
//---------------------------------------------------------------------------

#define PNP_DONE         0                                                      // the order is finished
#define PNP_DOING        1                                                      // the order is doing

#pragma package(smart_init)

//bool bStopGaliErrorMessage=false;                                             //JerryYang 20180411 (jou) : 沒用到,mark掉
int ZSafePos=20;                                                                //Steven 20220207 : 整合In/Out Arm Z軸安全位置
int ZlimitPos=-3200;                                                            //Ifor 20221026 add: In/Out Arm 吸嘴極限位置
MyMNetLine myLine[MAXRing];                                                     //Isaac 20181212 (Steven) : Baud Rate防呆功能
bool bGali_CardInstall=false;
bool GaliAxisAlarm[4]={false,false,false,false};
extern bool CheckOutSuckICFallDown(bool AutoClean);
BYTE cData;
long       rc=0L ;
char       response[64];
char       szBuffer[128], szBuffer1[128];
HANDLEDMC  hDmc;                                                                //declare type long
HWND       hWnd;
class TTrayMotor MOT[300];                                                      //JerryYang 20230515 : 215->300      //Steven 20150910 : Modify from 100 to 200
int TestYBuffer, TestY2Buffer;
//設定馬達實際位置與螢幕位置之比例
//設定類別建構式
bool IndexZCanMove[2]={true, true};

int GailAcSpeed=20000000 ;
int GailDcSpeed=20000000 ;
int GailAcSpeed2=20000000;
int GailDcSpeed2=20000000;

char bMask[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

const int iTorence=10;                                                          //Steven 20110402 : Encoder的容忍值,單位=條
int iEncoderTorence=500;                                                        //JerryYang 20180706 (wei) : 修改Encoder到位容許範圍
int iCheckZ=4000;                                                               //JerryYang 20260114 : 輕壓的速度比較快, 放寬range

bool bPauseInMotor=false, bPauseOutMotor=false;

//jou 2011-04-03 start
int iPCIL112_InArmXYMoveTask=1;
int iPCIL112_OutArmXYMoveTask=1;
//jou 2011-04-03 end
CRITICAL_SECTION g_cs;                                                          //ChungHung 20141015 add fix Gail command error
extern HAlarm *Alarm;
extern void SetInArmHome();
extern void SetOutArmHome();
//==============================================================================
bool CheckTestZ(AnsiString sFunc)                                               //Steven 20141007 : 換位置
{
    AnsiString Str;
    static int iRetryCT=0;
    bool flag1=false, flag2=false, flag3=false, flag4=false;
    long lPos[4]={0, 0, 0, 0};                                                  //kevin 20150915

    lPos[0]=MOT[MTestZ1].Gali_ReadPos();
    if(MOT[MTestZ1].Gali_ReadEncoderInRandgeMinLimit(lPos[0]))
        flag1=true;

    lPos[1]=MOT[MTestZ2].Gali_ReadPos();
    if(MOT[MTestZ2].Gali_ReadEncoderInRandgeMinLimit(lPos[1]))
        flag2=true;

    lPos[2]=MOT[MTestY1].Gali_ReadPos();
    if(MOT[MTestY1].Gali_ReadEncoderInRandgeMinLimit(lPos[2]))
        flag3=true;

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        lPos[3]=0;
        flag4=true;
    }
    else
    {
        lPos[3]=MOT[MTestY2].Gali_ReadPos();
        if(MOT[MTestY2].Gali_ReadEncoderInRandgeMinLimit(lPos[3]))
            flag4=true;
    }

    if(flag1 && flag2 && flag3 && flag4)
    {
        iRetryCT=0;
        return true;
    }
    else
    {
        iRetryCT++;
        if(iRetryCT>150)                                                        //kevin 20130312100
        {
            Str.sprintf("CheckTestZ(%s)", sFunc);
            ShowIndexMotorError(Str);
            iRetryCT=0;                                                         //kevin 20110628 發生alarm 需清為0否則要關程式
            RecordIndexPositionError(Str, flag1, flag2, flag3, flag4, &lPos[0]); //kevin 20150915 record
        }
        return false;
    }
}
//==============================================================================
bool CheckTestZ1(AnsiString sFunc)                                              //Steven 20141007 : 換位置
{
    static int iRetryCT=0;
    bool flag1=false, flag2=false, flag3=false, flag4=false;
    long lPos[4]={0, 0, 0, 0};                                                  //kevin 20150915
    AnsiString S;

    lPos[0]=MOT[MTestZ1].Gali_ReadPos();
    if(MOT[MTestZ1].Gali_ReadEncoderInRandgeMinLimit(lPos[0]))
        flag1=true;

    flag2=true;
    lPos[1]=0;

    lPos[2]=MOT[MTestY1].Gali_ReadPos();
    if(MOT[MTestY1].Gali_ReadEncoderInRandgeMinLimit(lPos[2]))
        flag3=true;

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        lPos[3]=0;
        flag4=true;
    }
    else
    {
        lPos[3]=MOT[MTestY2].Gali_ReadPos();
        if(MOT[MTestY2].Gali_ReadEncoderInRandgeMinLimit(lPos[3]))
            flag4=true;
    }

    if(flag1 && flag2 && flag3 && flag4)
    {
#ifdef DEBUG
        long Pos=MOT[MTestZ1].Gali_ReadPos();
        long Pos1=MOT[MTestZ1].Gali_ReadEncoderPos();
        S.sprintf("%d, %d, %d, %d", Pos, Pos1, Pos-Pos1, iRetryCT);
        fMain->lbEnCoder0->Caption=S;
#endif
        iRetryCT=0;
        return true;
    }
    else
    {
        iRetryCT++;
        MySleepEx(5, true);
        if(iRetryCT>100)
        {
            S.sprintf("CheckTestZ1(%s)", sFunc);
            ShowIndexMotorError(S);
            iRetryCT=0;
            RecordIndexPositionError(S, flag1, flag2, flag3, flag4, &lPos[0]); //kevin 20150915 record
        }
        return false;
    }
}
//==============================================================================
bool CheckTestZ2(AnsiString sFunc)                                              //Steven 20141007 : 換位置
{
    static int iRetryCT=0;
    bool flag1=false, flag2=false, flag3=false, flag4=false;
    long lPos[4]={0, 0, 0, 0};                                             //kevin 20150915
    AnsiString S;

    flag1=true;

    lPos[1]=MOT[MTestZ2].Gali_ReadPos();
    if(MOT[MTestZ2].Gali_ReadEncoderInRandgeMinLimit(lPos[1]))
        flag2=true;

    lPos[2]=MOT[MTestY1].Gali_ReadPos();
    if(MOT[MTestY1].Gali_ReadEncoderInRandgeMinLimit(lPos[2]))
        flag3=true;

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        lPos[3]=0;
        flag4=true;
    }
    else
    {
        lPos[3]=MOT[MTestY2].Gali_ReadPos();
        if(MOT[MTestY2].Gali_ReadEncoderInRandgeMinLimit(lPos[3]))
            flag4=true;
    }

    if(flag1 && flag2 && flag3 && flag4)
    {
#ifdef DEGBU
        long Pos=MOT[MTestZ2].Gali_ReadPos();
        long Pos1=MOT[MTestZ2].Gali_ReadEncoderPos();
        S.sprintf("%d, %d, %d, %d", Pos, Pos1, Pos-Pos1, iRetryCT);
        fMain->lbEnCoder1->Caption=S;
#endif
        iRetryCT=0;
        return true;
    }
    else
    {
        iRetryCT++;
        MySleepEx(5, true);
        if(iRetryCT>100)
        {
            S.sprintf("CheckTestZ2(%s)", sFunc);
            ShowIndexMotorError(S);
            iRetryCT=0;
            RecordIndexPositionError(S, flag1, flag2, flag3, flag4, &lPos[0]);  //kevin 20150915 record
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool IndexPosMonitor(int IsZ1Down)
{
    bool bRet=false;
    bool Z1, Z2, Y1, Y2;
    AnsiString str;

    if(IsZ1Down==1)                                                             //Index Arm 1
    {
        Z1=MOT[MTestZ1].Gali_ReadEncoderPos();
        Y1=MOT[MTestY1].Gali_ReadEncoderPos();

        if(Z1<Prod.All_TestZ_Test_Safe)
        {
            str.sprintf("Z1,%d,Y1,%d", Z1, Y1);
            fMain->tMonitorIndex->Add(str);
        }
    }
    else if(IsZ1Down==2)                                                        //Index Arm 2
    {
        Z2=MOT[MTestZ2].Gali_ReadEncoderPos();
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            Y2=MOT[MTestY2].Gali_ReadEncoderPos();

        if(Z2<Prod.All_TestZ_Test_Safe)
        {
            str.sprintf("Z2,%d,Y2,%d", Z2, Y2);
            fMain->tMonitorIndex->Add(str);
        }
    }

    return bRet;
}
//------------------------------------------------------------------------------
void ShowIndexMotorError(AnsiString Debug, bool bCompareTeachPos)               //Steven 20180319 (Jou) : 增加傳入的Function名稱,方便Debug
{
    AnsiString str,str1;
//    if(CheckOutArmDestory()==0)                                               //kevin 20180119 add     //Steven 20210608 : Mark.
    {
        fAllMotorHome=false;
        iHome=1;
        StopAllMotor();
        int y1CmdPos     = MOT[MTestY1].Gali_ReadPos();
        int y1EncoderPos = MOT[MTestY1].Gali_ReadEncoderPos();
        int y2CmdPos     = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadPos();
        int y2EncoderPos = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadEncoderPos();
        int z1CmdPos     = MOT[MTestZ1].Gali_ReadPos();
        int z1EncoderPos = MOT[MTestZ1].Gali_ReadEncoderPos();
        int z2CmdPos     = MOT[MTestZ2].Gali_ReadPos();
        int z2EncoderPos = MOT[MTestZ2].Gali_ReadEncoderPos();
        if(bCompareTeachPos==false)                                             // 原來，encoder 和 command 比較
        {
            str.sprintf("Y1 CMD:%d, POS:%d; Y2 CMD:%d, POS:%d, \n\rZ1 CMD:%d, POS:%d; Z2 CMD:%d, POS:%d, Function:%s",
                        y1CmdPos, y1EncoderPos,
                        y2CmdPos, y2EncoderPos,
                        z1CmdPos, z1EncoderPos,
                        z2CmdPos, z2EncoderPos,
                        Debug);
        }
        else                                                                    // encoder 和 Teaching 點比較
        {
            str.sprintf("Y1: POS:%d, Teach F:%d, M:%d;\n\rY2: POS:%d, Teach M:%d, R:%d,\n\rZ1 POS:%d; Z2 POS:%d, \n\rFunction:%s",
                        y1EncoderPos, Prod.TestY1_Front, Prod.TestY1_Middle,
                        y2EncoderPos, Prod.TestY2_Middle, Prod.TestY2_Rear,
                        z1EncoderPos,
                        z2EncoderPos,
                        Debug);
        }
        bShowIndexMotorError=true;                                              //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
        ShowMyMessage("Index Position error, Index 4 Axis Need home", str, str);
        SoftStop=true;
    }
}
//---------------------------------------------------------------------------
double GetScale(int s1, int e1, int s2, int e2)
{
    double P, S;
    S=(double)(s2-e2);
    if(S==0)
        return 1.0;
    P=(double)(s1-e1);
    if(S==0)
        ShowMyMessage("GetScale_Fail", "取得比例錯誤");
    return (double) P/S;
}
//------------------------------------------------------------------------------
void SetGaliRate(int Scale)
{
    GailAcSpeed =450000*Scale;                                                  // 加減速 - 慢
    GailDcSpeed =450000*Scale;
    GailAcSpeed2=550000*Scale;
    GailDcSpeed2=550000*Scale;                                                  // 加減速 - 快
}
//------------------------------------------------------------------------------
int DelayCount=2;
void InitialSetUpGaliDelayCount(int iDelayCT)
{
    DelayCount=iDelayCT;
}
//---------------------------------------------------------------------------
void InitialGaliDelayCount()
{
    DelayCount=5;
    int ret;
    AnsiString S="GaliDelayCount";
    ret=CheckAndReadIniDataGeneral("VENDER",S.c_str(),-999999);
    if(ret==-999999)
    {
        WriteIniDataGeneral("VENDER", S.c_str(), DelayCount);
    }
    else
    {
        if(ret<20 && ret>=1)
            DelayCount=ret;
    }
}
//---------------------------------------------------------------------------
__fastcall TMyMotor::TMyMotor()
{
    RefStart=2;
    RefEnd=0;
    FactStart=4;
    FactEnd=0;
    fCanMove=true;
    fCanMoveR=true;
    fCanMoveM=true;
    fCanMoveL=true;
    Scale=GetScale(RefStart,RefEnd,FactStart,FactEnd);
    bIsFullIC=false;
    bIsEmptyIC=false;
    MovFlag=false;
    bPanelUse=false;
    bUpDownMove=false;
    Gali_MotorAlarm=false;
    GaliSofDelayCount=0;
    bScanFlag=false;
    iGali_SingalHomeTask=1;
    mapLockList.clear();                                                        //Steven 20210825 : 吹氣完成才可以歸零

    //Isaac 20201110 : Index Y find motor phase
    //=>
    iGali_FindZPhaseTask[0]=1;
    iGali_FindZPhaseTask[1]=1;
    iGali_FindZPhaseTask[2]=1;
    iGali_FindZPhaseTask[3]=1;
    //<=
    //Isaac 20201110 : Index Y find motor phase

    //new PCI Board
    Mot_Name=0;
    fCMD=false;
    iOldPos=0;
    iCheckStatusCT=0;
    GaliScanServo=true;
    iEncoderCheckCT=100;
    bCheckEncoderEveryTime=false;
    bShowMotorMove=false;
    InitializeCriticalSection(&g_cs);                                           //ChungHung 20141015 add fix Gail command error
    if(MOTION_CARD_TYPE==MotionCard_SYN)                                        //JerryYang 20180706 (wei) : 修改Encoder到位容許範圍
    {
        iEncoderTorence=1000;
    }
    else
    {
        iEncoderTorence=1000;                                                   //kevin 注意 馬達驅動器 PA431  : 100
    }

    if(INDEX_PRESS_TYPE==e160KG && CUSTOMER_CODE==CC_JCET)                      //RogerYang 20260210 : JCET 160公斤容易報錯，先不卡控(10000有的機台會報)
    {
        iCheckZ=11000;
    }
    //AI(ht9045-v899) 20260512: CASE-20260507-001 CC_GIGAS Z2 到位較慢導致 Y2 middle 檢查提早觸發 alarm，放寬 iCheckZ 至 11000
    else if(CUSTOMER_CODE==CC_GIGAS)
    {
        iCheckZ=11000;
    }
    else if(INDEX_PRESS_TYPE==e85KG || INDEX_PRESS_TYPE==e240KG ||                   //JerryYang 20260114 : 輕壓的速度比較快, 放寬range
       INDEX_PRESS_TYPE==e120KG || INDEX_PRESS_TYPE==e160KG)
    {
        iCheckZ=9000;
    }
    else
    {
        iCheckZ=4000;
    }
}
//---------------------------------------------------------------------------
TMyMotor::~TMyMotor()
{
    mapLockList.clear();                                                        //Steven 20210825 : 吹氣完成才可以歸零
}
//---------------------------------------------------------------------------
void TMyMotor::SetPanel(TWinControl *PCtrl, bool b)
{
    bUpDownMove=b;
    bPanelUse=true;
    PWinCtrl=PCtrl;
}
//---------------------------------------------------------------------------
void TMyMotor::SetAlias(int iNo, AnsiString Name)
{
    Alias   =Name;
    Mot_Name=iNo;
    NumberAlias.sprintf("[%02d] %s", iNo, Name);
}
//---------------------------------------------------------------------------
bool TMyMotor::IsCanMove()
{
    if(fCanMove && fCanMoveR && fCanMoveM && fCanMoveL)
        return true;
    return false;
}
//---------------------------------------------------------------------------
void TMyMotor::SetScreenScale(int s1, int e1, int s2, int e2)
{
    RefStart=s1;  RefEnd=e1; FactStart=s2;  FactEnd=e2;
    Scale=GetScale(RefStart, RefEnd, FactStart, FactEnd);
    ScreenPos= (int)(Scale*(Position-FactStart))+RefStart;
}
//------------------------------------------------------------------------------
int TMyMotor::ReadPos()
{
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)
    {
        Position=Motor->ReadPos();
        EncoderPosition=Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition=Position;
    }

    ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
    return Position;
}
//------------------------------------------------------------------------------
int TMyMotor::ReadEncoderPos()
{
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)
    {
        Position=Motor->ReadPos();
        EncoderPosition=Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition=Position;
    }
    return EncoderPosition;
}
//------------------------------------------------------------------------------
void TMyMotor::SetADCRate(int Scale)
{
    double fPersent=0.0;
    double Acc, Dec;

    if(Scale==100.0)
    {
        Acc=Motor->GetAccDataBase();
        Dec=Motor->GetDecDataBase();
    }
    else
    {
        fPersent=double(Scale)/100.0;
        Acc=ChangeToFloatNonPcnt((double)(Motor->GetAccDataBase()), (double)(fPersent));
        Dec=ChangeToFloatNonPcnt((double)(Motor->GetDecDataBase()), (double)(fPersent));
    }

    Motor->SetAcc(double(Acc));
    Motor->SetDec(double(Dec));
}
//------------------------------------------------------------------------------
void TMyMotor::SetSpeed(double p)
{
    int s;

    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)
    {
        if(Mot_Name==MTestY1 || Mot_Name==MTestZ1 || Mot_Name==MTestZ2 || Mot_Name==MTestY2)
        {

        }
        else
        {
            if(p>=100)                                                          //kevin 20180226 (Steven) add 避免改錯速度
                p=100;

            if(Mot_Name==MLoaderZ || Mot_Name==MEmptyZ || Mot_Name==MColorZ ||
               Mot_Name==MAuto1Z  || Mot_Name==MAuto2Z || Mot_Name==MAuto3Z ||  //Steven 20200723 : For軌道Z馬達的速度
               Mot_Name==MAuto4Z  || Mot_Name==MAuto5Z || Mot_Name==MAuto6Z)    //Steven 20230907 : For HT-9011UC
            {
                s=(Motor->PJogHighSpeed-Motor->PJogLowSpeed)*p/100+Motor->PJogLowSpeed;
            }
            else
            {
                s=Motor->PJogHighSpeed*p/100;                                   //ChungHung 20130625 delete
            }

            Motor->SetSpeed(s);
            speed=s;                                                            //kevin 20180201 add  read speed use
        }
    }
    else
    {
        speed=p;                                                                //ChungHung 20130625 delete
    }
}
//------------------------------------------------------------------------------
int TMyMotor::GetSpeed()
{
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)
        return Motor->ReadSpeed();
    else
        return speed;
}
//------------------------------------------------------------------------------
void TMyMotor::EnableMotorMove()
{
    fCanMove=true;
    fCanMoveR=true;
    fCanMoveM=true;
    fCanMoveL=true;
}
//------------------------------------------------------------------------------
void TMyMotor::InitMOTParameter()
{
    fCMD=false;
    GaliSofDelayCount=0;
    //Gali
    bScanFlag=false;
    MovFlag=false;
}
//------------------------------------------------------------------------------
// 1 : Move Success
// 0 : Moving
//-1 : Safe Door Opened
//-2 : Target > Limit P
//-3 : Target < Limit N
//-4 : PServoAlarmOn
//-5 : WAR1639 Motor encoder error, check encoder cable
//-6 : MOT Home sensor error!!
//------------------------------------------------------------------------------
int TMyMotor::MotorMovePosition(int &Position, int speed, int Tar)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return -1;
    }

    int ret=0;
    int Pos=Tar;
    int iEncoder=0;
    AnsiString S1="", S2="", S3="";
    int iGap=2;                                                                 //Sam 20230621 : Gap容許誤差改為1>2 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條

    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)                                                           // has true motor
    {
        if(Tar>=Motor->PSoftLimitP)
        {
            S1=AnsiString("The target position of ")+Alias+AnsiString(" over positive soft limit !");
            S2=Alias+AnsiString("的目標位置超過正向軟體極限!");
            S3.sprintf("%d > %d", Tar, Motor->PSoftLimitP);
            ShowMyMessage(S1, S2, S3);
            return -2;
        }
        if(Tar<=Motor->PSoftLimitN)
        {
            S1=AnsiString("The target position of ")+Alias+AnsiString(" below negative soft limit !");
            S2=Alias+AnsiString("的目標位置低於負向軟體極限!");
            S3.sprintf("%d <= %d", Tar, Motor->PSoftLimitN);
            ShowMyMessage(S1, S2, S3);
            return -3;
        }

        Position=ReadPos();                                                     //calculate GearRate Pos

        if(Position!=Tar)                                                       //RogerYang 20260113 : 用來紀錄Rotator最近一次旋轉方向
        {
            iLastRotatorDirP=(Tar>Position)?true:false;
        }

        if(fCMD==false && Motor->MotionDone()==false)
        {
            return 0;
        }
        else if(fCMD==false)
        {
            GetRealPos(&Pos);                                                   //pos will change to gear ration value
            #ifndef USE_CompareCommandPos
            if(Tar==Position)
            #else
            if((Motor->GearRatio>2 && Motor->GearRatio<=5)  &&
                ((Mot_Name>=MInArmZA && Mot_Name<=MInArmZH) ||
                 (Mot_Name>=MOutArmZA && Mot_Name<=MOutArmZH)))                 //Jimmychiu 20230216 : 吸嘴高度
            {
                iGap=5;
            }

            if(CompareCommandPos(Tar, iGap)==1)                                 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
            #endif
            {
                if(Motor->PServoAlarmOn)
                {
                    ScanMotorStatus();
                    if(Led[iInposLed]==true)
                    {
                        return 0;
                    }
                    else
                    {
                        InitMOTParameter();
                        return 1;                                               // -4 --> 1
                    }
                }
                else
                {
                    InitMOTParameter();
                    return 1;
                }
            }
//            if(Motor->Direction==false)
//                Pos=-Pos;

            if(Mot_Name==MInArmY || Mot_Name==MOutArmY)                         //Steven 20181129 : 短距離的移動把加減速縮短一半
            {
                if(AUTO_EMPTY_COLOR>=3)
                {
                    if(Mot_Name==MOutArmY && (abs(Tar-Position)<6500))
                        ret=Motor->MoveToPosShortDisSlowSP(Pos);
                    else
                        ret=Motor->MoveToPos(Pos);
                }
                else
                {
                    if(abs(Tar-Position)<6500)
                        ret=Motor->MoveToPosShortDistance(Pos);
                    else
                        ret=Motor->MoveToPos(Pos);
                }
            }
            else if(Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2)
            {
                if(abs(Tar-Position)<5000)
                    ret=Motor->MoveToPosShortDistance(Pos);
                else
                    ret=Motor->MoveToPos(Pos);
            }
            else
            {
                ret=Motor->MoveToPos(Pos);
            }

            if(ret==0)
            {

            }

            fCMD=true;
        }
        else
        {
            if(Motor->MotionDone())
            {
                Position=Tar;
                if(Motor->PServoAlarmOn)
                {
                    ScanMotorStatus();
                    if(Led[iInposLed]==true)
                    {
                        return 0;
                    }
                    else
                    {
                        InitMOTParameter();
                        iEncoderCheckCT++;
                        if(bCheckEncoderEveryTime==true || iEncoderCheckCT>100)
                        {
                            iEncoderCheckCT=0;
                            iEncoder=ReadEncoderPos();
                            if((Tar-iEncoderTorence)>iEncoder ||                //JerryYang 20180706 : 修改Encoder到位容許範圍
                               (Tar+iEncoderTorence)<iEncoder)
                            {
                                S1.sprintf("MOT=%s, Tar=%d, Encoder=%d", Alias, Tar, iEncoder);
                                ShowErrorMessage("WAR1639", 0, MMSystem, 0, S1);   //Motor encoder error, check encoder cable
                                return -5;
                            }
                            if(Motor->PServoAlarmOn)
                            {
                                if(Mot_Name==MInRotateKit ||                    //Sam 20190811 : 防止 Rotate 旋轉完後剛好位置剛好落在原點上面導致誤報警。
                                   Mot_Name==MOutRotateKit)
                                {
                                    return 1;
                                }
                                if(iEncoder>2000 || iEncoder<-2000)
                                {
                                    ScanMotorStatus();
                                    if((Mot_Name==MInArmY ||                    //JerryYang 20191210 fix auto clean時後排到shuttle row A跳出home sensor error
                                        Mot_Name==MOutArmY) &&
                                       iEncoder>2000)
                                    {

                                    }
                                    else
                                    {
                                        if(Led[1]==true)
                                        {
                                            S1.sprintf("MOT=%s Home sensor error!!", Alias);
                                            ShowMyMessage(S1, "");
                                            return -6;
                                        }
                                    }
                                }
                            }

                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    InitMOTParameter();

                    //AI(ht9045-v899) 20260505: MLoaderY 工作位置 iMLoaderYCarPos=-505 會誤觸發此防護，僅對 MLoaderY 豁免，其餘馬達維持原有 home sensor 防護
                    if(Position<=-500 && Mot_Name!=MLoaderY)                    //kevin 20140121 Z軸 home sensor 損壞
                    {
                        if(Mot_Name==MInRotateKit ||                            //Sam 20190811 : 防止 Rotate 旋轉完後剛好位置剛好落在原點上面導致誤報警。
                           Mot_Name==MOutRotateKit)
                        {
                            return 1;
                        }
                        ScanMotorStatus();
                        if(Led[1]==true)
                        {
                            S1.sprintf("MOT=%s Home sensor error!!", Alias);
                            ShowMyMessage(S1, "");

                            if(Mot_Name==MInArmPitch   || Mot_Name==MInArmPitchX2 ||
                               Mot_Name==MInArmPitchX3 || Mot_Name==MInArmPitchX4 ||
                               Mot_Name==MInArmPitchY)
                            {
                                SetInArmHome();
                            }
                            if(Mot_Name==MOutArmPitch   || Mot_Name==MOutArmPitchX2 ||
                               Mot_Name==MOutArmPitchX3 || Mot_Name==MOutArmPitchX4 ||
                               Mot_Name==MOutArmPitchY)
                            {
                                SetOutArmHome();
                            }
                            return -6;
                        }
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    #ifndef SOFT_SIMULTE
        Position=Tar;
        return 1;
    #else
        if(Position==Tar)
        {
            fCMD=false;
            return 1;
        }
        else
        {
            if(speed<=0)                                                        //Steven 20210730 : 修正軟體模擬的最小速度
                speed=100;

            if(Position>Tar)
            {
                fCMD=true;
                Position-=speed;
                if(Position<=Tar)
                {
                    fCMD=false;
                    Position=Tar;
                    return 1;
                }
            }
            else
            {
                fCMD=true;
                Position+=speed;
                if(Position>=Tar)
                {
                    fCMD=false;
                    Position=Tar;
                    return 1;
                }
            }
        }
        return 0;
    #endif
}
//------------------------------------------------------------------------------
// 1 : Move Success
// 0 : Moving
//-1 : Safe Door Opened
//-2 : Target > Limit P
//-3 : Target < Limit N
//-4 : PServoAlarmOn
//-5 : WAR1639 Motor encoder error, check encoder cable
//-6 : MOT Home sensor error!!
//------------------------------------------------------------------------------
int TMyMotor::MotorMove(int p)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return -1;
    }

    //jou 2010-12-23 保護兩次命令會造成撞機
    if(p!=iOldPos)
    {
        fCMD=false;
        iOldPos=p;
    }

    int ret=0;
    int iCommandPos;
    int iGap=1;

    if(fCanMove ==false  ||
       fCanMoveR==false  ||
       fCanMoveM==false  ||
       fCanMoveL==false  ||
       mapLockList.size()!=0)  //Klutter 20210817 加入鎖馬達機制                //Steven 20210825 : 吹氣完成才可以歸零
    {
        PCIL132_StopMotor();

        if((Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2) && (Motor->GearRatio>1))
        {
            iCommandPos=ReadPos();
            if(iCommandPos>p+iGap)      //超出+
            {
                return -2;
            }
            else if(iCommandPos<p-iGap)
            {
                return -3;             //低於-
            }
            else
            {
                fCMD=false;
                return 1;              //正常等於
            }
        }
        else
        {
            if(p==ReadPos())
            {
                fCMD=false;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    ret=MotorMovePosition(Position, speed, p);
    if(bShowMotorMove==true)
    {
        ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
        if(bPanelUse)
        {
            if(bUpDownMove)
            {
                if(abs(PWinCtrl->Top-ScreenPos)>2)
                    PWinCtrl->Top=ScreenPos;
            }
            else
            {
                if(abs(PWinCtrl->Left-ScreenPos)>2)
                    PWinCtrl->Left=ScreenPos;
            }
        }
    }

    if(ret==1)
    {
        fCMD=false;
        return 1;
    }
    else
    {
        return ret;
    }
}
//---------------------------------------------------------------------------
bool TMyMotor::MotorMove2SpeedForPicker(int FinalPos, ARM_CONDITION *ARM, bool bIsLoader)     //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader
{
    int iCurrPos;
    int iFlag=0;                                                                //jou 20170811 (Steven) int -> bool int Flag=false;

    if(ARM->iTwoSpeedMove==0 || (ARM->iTwoSpeedMove==1 && ARM->bTwoSpeedOnlyLoader==true && bIsLoader==false) || FinalPos>-100)  //JerryYang 20190729 二段速功能可選擇only at loader
    {
        iFlag=MotorMove(FinalPos);
    }
    else
    {
        iCurrPos=ReadPos();
        if(FinalPos==iCurrPos)                                                  //到位了
        {
            SetADCRate(ARM->iACDCZSP);
            SetSpeed(ARM->iZSP);
            iFlag=1;
        }
        else
        {
            if(iCurrPos>=FinalPos+ARM->dTwoSpeedDistance*100)                   //第一段
            {
                SetADCRate(ARM->iACDCZSP);
                SetSpeed(ARM->iZSP);
                iFlag=MotorMove(FinalPos+ARM->dTwoSpeedDistance*100);
            }

            if(iFlag==1 || iCurrPos<=FinalPos+ARM->dTwoSpeedDistance*100)       //第二段
            {
                SetADCRate(ARM->iTwoADC);
                SetSpeed(ARM->iTwoSpeed);
                iFlag=MotorMove(FinalPos);
            }
        }
    }
    return iFlag;
}
//---------------------------------------------------------------------------
__fastcall TTrayMotor::TTrayMotor()
{
//    IsEmptyTray=true;
    fHasTray=false;
    Tray.SetXYItem(1, 1);                                                       //Steven 20160614 : 使用function加上保護
    ClearTray(__FUNC__);
}
//---------------------------------------------------------------------------
//bool TTrayMotor::CurrectRowNoIC()
//{
//    int i;
//    for(i=0; i<Tray.XItem; i++)
//        if(Tray.Data[i][RowCT]==HAS_IC)
//            return false;
//    return true;
//};
//---------------------------------------------------------------------------
//bool TTrayMotor::CurrectRowFullIC()
//{
//    int i;
//    for(i=0; i<Tray.XItem; i++)
//        if(Tray.Data[i][RowCT]==NULL_IC)
//            return false;
//    return true;
//};
//---------------------------------------------------------------------------
//bool TTrayMotor::BufferNoIC()
//{
//    int i;
//    for(i=0; i<4; i++)
//        if(Buffer[i]==HAS_IC)
//            return false;
//    return true;
//}
//---------------------------------------------------------------------------
//bool TTrayMotor::BufferHasIC()
//{
//    int i;
//    for(i=0; i<4; i++)
//        if(Buffer[i]==HAS_IC)
//            return true;
//    return false;
//}
//---------------------------------------------------------------------------
/*bool TTrayMotor::HasIC()
{
    if(fHasTray==false)
        return false;
    if(Tray.HasIC())
        return true;
    return false;
} */
bool TTrayMotor::HasIC()                                                        //kevin 20130509 Autoclean hotplate1 有clean pad不判為有ic
{
    if(fHasTray==false)
        return false;

    if(pHTray!=NULL)
    {
        if(pHTray->Name=="mtPlate1")                                            //kevin 20120217 AUTO CLEAN
        {
            if(TestIF_File.iAutoClean_Function && TestIF_File.iAutoClean_Tray==eCKPos_HP2) //kevin 20120410
            {
                if(Tray.CleanPlate2HasIC())
                    return true;
            }
            else if(Tray.HasIC())
            {
                return true;
            }
        }
        else if(Tray.HasIC())
        {
            return true;
        }
    }
    else if(Tray.HasIC())
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TTrayMotor::HasCleanPad()                                                  //kevin 20150505 判斷裡面有沒有CLEAN PAD
{
    if(fHasTray==false)
        return false;

    if(pHTray!=NULL)
    {
        if(pHTray->Name=="mtPlate1" || pHTray->Name=="tmyAutoClean")            //kevin 20120217 AUTO CLEAN
            if(Tray.HasCleanPad())                                              //kevin 20150505 autoclean  判斷裡面有沒有CLEAN PAD
                return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TTrayMotor::HasRealIC()
{
    if(fHasTray==false)
        return false;
    if(Tray.HasRealIC())
        return true;
    return false;
}
//---------------------------------------------------------------------------
//V3.27.548 Steven 20171031 (wei) : Add TTMyTray滑鼠滑過去顯示Hint
//---------------------------------------------------------------------------
void TTrayMotor::SetTrayBinData(int x, int y, int data, AnsiString iInfo)
{
    Tray.Data[x][y]=data;
    if(fHTary)
    {
        if(data>=1000)                                                          //wei 20160822 測試後無法顯示IC顏色
            data-=1000;
        pHTray->SetCellColorIndex(x, y, data);
        pHTray->SetCellNumber(x, y, iInfo);
    }
}
//---------------------------------------------------------------------------
void  TTrayMotor::SetTraySingleData(int x, int y, int data, int iTarget)
{
    Tray.Data[x][y]=data;
    Tray.iTarget[x][y]=iTarget;                                                 //Steven 20150915 : For TSMC 手動整盤功能
    if(fHTary)
    {
        if(data>=1000)                                                          //wei 20160822 測試後無法顯示IC顏色
            data-=1000;
        pHTray->SetCellColorIndex(x, y, data);
    }
}
//---------------------------------------------------------------------------
void  TTrayMotor::SetTraySiteMap(int x, int y, int iSiteMap)                    //Steven 20220510 : For QTI SD Device Map Function
{
    SetTraySingleData(x, y, HAS_IC);
    Tray.iWhichSite[x][y]=iSiteMap;
}
//---------------------------------------------------------------------------
void  TTrayMotor::SetNullIcToHasNullIc()
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            if(Tray.Data[i][j]==NULL_IC)
            {
                SetTraySingleData(i, j, HAS_NULL_IC);
            }
        }
    }
}
//---------------------------------------------------------------------------
void  TTrayMotor::SetNullIcToHasIc()                                            //Sam 20240424 : 修正 P54 Fix 分盤功能失效問題。
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            if(Tray.Data[i][j]==NULL_IC)
            {
                SetTraySingleData(i, j, HAS_IC);
            }
        }
    }
}
//Q_Q 2010.09.29 SKIP功能=======================================================
void  TTrayMotor::SetTrayBufferSingleData(int x, int y, int data)
{
    Tray.BufferData[x][y]=data;
}
//==============================================================================
void  TTrayMotor::Refresh()
{
    int x, y;
    if(fHTary)
    {
        pHTray->XBlockItem=Tray.XBItem;                                         //Frank 20160928 add Subtray Function
        pHTray->YBlockItem=Tray.YBItem;

        pHTray->XBlockWidth=Tray.XBWidth;
        pHTray->YBlockWidth=Tray.YBWidth;

        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;
        for(x=0; x<Tray.XItem; x++)
            for(y=0; y<Tray.YItem; y++)
                pHTray->SetCellColorIndex(x, y, Tray.Data[x][y]);
    }
}
//---------------------------------------------------------------------------
void TTrayMotor::InitNewTray(int data, bool bShowSiteMapFlag, AnsiString Func)
{
    int iCount;
    Tray.ClearData();                                                           //Steven 20200619 : 加上保護
    Tray.SetData(data);

    AnsiString Str;
    Str.sprintf("Initial new tray [%s] with IC type %s by function %s ", Alias, sIC_Type[data], Func);
    MNetLog(Str);

    if(fHTary)
    {
        pHTray->XBlockItem=Tray.XBItem;                                         //Frank 20160928 add Subtray Function
        pHTray->YBlockItem=Tray.YBItem;

        pHTray->XBlockWidth=Tray.XBWidth;
        pHTray->YBlockWidth=Tray.YBWidth;

        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;

        bShowSiteMap=bShowSiteMapFlag;
        if(bShowSiteMap)                                                        //Steven 20170302 (wei) : FIFO MODE
        {
            if(IniConfig.bI37_LockLoaderDirection)
            {
                iDirection=IniConfig.iI37_LockLoaderDirection;
            }
            else
            {
                iDirection=TrayForm.Loader.Direction;
            }
            iSiteCount=GetSiteCount();
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    if(TestIF_File.iSiteMap[i][j]>0)                            //Steven 20170302 (wei) : 確認哪個Site有開, 從1開始~32
                    {
                        if(TestIF_File.iShuttleMode==1)                         //單arm       //Isaac 20210821 : FIFO開site修正
                            bSiteHasTurnOn[TestIF_File.iSiteMap[i][j]]=bTestSiteUse[TestIF_File.iShuttle_Sel][i][j];
                        else                                                    //雙arm
                            bSiteHasTurnOn[TestIF_File.iSiteMap[i][j]]=bTestSiteUse[0][i][j];
                    }
                }
            }
        }

        for(int i=0; i<Tray.XItem; i++)
        {
            for(int j=0; j<Tray.YItem; j++)
            {
                pHTray->SetCellColorIndex(i, j, data);
                if(bShowSiteMap==false)                                         //Steven 20170302 (wei) : FIFO MODE
                    pHTray->SetCellNumber(i, j, "");
            }
        }

        if(bShowSiteMap)                                                        //Steven 20170302 (wei) : FIFO MODE
        {
            iCount=1;
            // ----   左至右,上至下
            //  /
            // --->
            if(iDirection==0)
            {
                for(int j=0; j<Tray.YItem; j++)
                {
                    for(int i=0; i<Tray.XItem; i++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // ----   右至左,上至下
            //  \\
            // <---
            else if(iDirection==1)
            {
                for(int j=0; j<Tray.YItem; j++)
                {
                    for(int i=Tray.XItem-1; i>=0; i--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // --->   左至右,下至上
            //  \\
            // ----
            else if(iDirection==2)
            {
                for(int j=Tray.YItem-1; j>=0; j--)
                {
                    for(int i=0; i<Tray.XItem; i++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // <---   右至左,下至上
            //   /
            // ----
            else if(iDirection==3)
            {
                for(int j=Tray.YItem-1; j>=0; j--)
                {
                    for(int i=Tray.XItem-1; i>=0; i--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   | 上至下, 左至右
            // | / |
            // |   V
            else if(iDirection==4)
            {
                for(int i=0; i<Tray.XItem; i++)
                {
                    for(int j=0; j<Tray.YItem; j++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   ^ 下至上, 左至右
            // | \\|
            // |   |
            else if(iDirection==5)
            {
                for(int i=0; i<Tray.XItem; i++)
                {
                    for(int j=Tray.YItem-1; j>=0; j--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   | 上至下, 右至左
            // | \\|
            // V   |
            else if(iDirection==6)
            {
                for(int i=Tray.XItem-1; i>=0; i--)
                {
                    for(int j=0; j<Tray.YItem; j++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // ^   | 下至上, 右至左
            // | / |
            // |   |
            else if(iDirection==7)
            {
                for(int i=Tray.XItem-1; i>=0; i--)
                {
                    for(int j=Tray.YItem-1; j>=0; j--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TTrayMotor::ClearTray(AnsiString Func)
{
    AnsiString Str;
    if(fHasTray)
    {
        Str.sprintf("Clear tray [%s] by function %s ", Alias, Func);
        MNetLog(Str);
    }

    fHasTray=false;
    iIsCoverTray=NULL_IC;                                                       //JerryYang 20240318 : add
    sTrayID="";
    Tray.ClearData();

    if(fHTary)
    {
        pHTray->XBlockItem=1;                                                   //Frank 20160928 add Subtray Function
        pHTray->YBlockItem=1;

        pHTray->XBlockWidth=1;
        pHTray->YBlockWidth=1;

        pHTray->XItem=1;
        pHTray->YItem=1;
        pHTray->SetCellColorIndex(0, 0, NULL_IC);
        pHTray->SetCellNumber(0, 0, "");                                        //Steven 20170302 (wei) : FIFO MODE
    }
    sUnloaderAlarmMsg="";                                                       //Jimmychiu 20240902 : Need Alarm After Unloader Full
}
//---------------------------------------------------------------------------
void TTrayMotor::InitEmptyTray(AnsiString Func)
{
    Tray.ClearData();
    AnsiString Str;
    Str.sprintf("Initial empty tray [%s] by function %s ", Alias, Func);
    MNetLog(Str);

    if(fHTary)
    {
        pHTray->XBlockItem=Tray.XBItem;                                         //Frank 20160928 add Subtray Function
        pHTray->YBlockItem=Tray.YBItem;

        pHTray->XBlockWidth=Tray.XBWidth;
        pHTray->YBlockWidth=Tray.YBWidth;

        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;
        for(int i=0; i<Tray.XItem; i++)
        {
            for(int j=0; j<Tray.YItem; j++)
            {
                pHTray->SetCellColorIndex(i, j, NULL_IC);
                pHTray->SetCellNumber(i, j, "");                                //Steven 20170302 (wei) : FIFO MODE
            }
        }
    }
}
//---------------------------------------------------------------------------
void TTrayMotor::SetHTrayPanel(TTMyTray *ptr)
{
    fHTary=true;
    pHTray=ptr;
}
//---------------------------------------------------------------------------
void TTrayMotor::SetTray(int data, AnsiString Func)
{
    fHasTray=true;
    if(fHTary)
    {
        fHasTray=true;
        InitNewTray(data, false, Func);
    }
}
//---------------------------------------------------------------------------
bool TTrayMotor::UpHalfIsFull()
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<(Tray.YItem/2); j++)
            if(Tray.Data[i][j]==NULL_IC)
                return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TTrayMotor::DownHalfIsFull()
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=(Tray.YItem/2); j<Tray.YItem; j++)
            if(Tray.Data[i][j]==NULL_IC)
                return false;
    }
    return true;
}
//---------------------------------------------------------------------------
int TTrayMotor::WhichBufferIsFull()                                             //JerryYang 20221215 : Magazine把fix區當buffer區功能
{
    int iCnt[5]={0, 0, 0, 0, 0};
    for(int k=0; k<5; k++)
    {
        iCnt[k]=0;
        for(int i=0; i<Tray.XItem; i++)
        {
            for(int j=0; j<Tray.YItem; j++)
            {
                if(j>=k*iYRegNum && j<(k+1)*iYRegNum && Tray.Data[i][j]==NULL_IC)
                {
                    iCnt[k]++;
                }
            }
        }
        if(iCnt[k]<8)
        {
            return k*3;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
void TTrayMotor::MoveTrayAllItem(class TTrayMotor *Source)                      //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin
{
    if(Source->fHasTray==false)
        return;
    fHasTray=true;

    int MinRow, MinCol;
    if(Source->Tray.XItem>=Tray.XItem)
        MinRow=Tray.XItem;
    else
        MinRow=Source->Tray.XItem;

    if(Source->Tray.YItem>=Tray.YItem)
        MinCol=Tray.YItem;
    else
        MinCol=Source->Tray.YItem;

    for(int i=0; i<MinRow; i++)
    {
        for(int j=0; j<MinCol; j++)
        {
            SetTraySingleData(i, j, Source->Tray.Data[i][j]);
            Tray.iWhichSite[i][j]    =Source->Tray.iWhichSite[i][j];
            Tray.iNeedRotAng[i][j]   =Source->Tray.iNeedRotAng[i][j];
            Tray.iCurrRotAng[i][j]   =Source->Tray.iCurrRotAng[i][j];
            Tray.iWhichIndex[i][j]   =Source->Tray.iWhichIndex[i][j];
            Tray.iBinCode[i][j]      =Source->Tray.iBinCode[i][j];
            Tray.BufferData[i][j]    =Source->Tray.BufferData[i][j];
            Tray.iTarget[i][j]       =Source->Tray.iTarget[i][j];
            Tray.iCleanCount[i][j]   =Source->Tray.iCleanCount[i][j];
            Tray.bFliped[i][j]       =Source->Tray.bFliped[i][j];
            Tray.cDeviceInf[i][j]    =Source->Tray.cDeviceInf[i][j];
            Tray.b2DIDNG[i][j]       =Source->Tray.b2DIDNG[i][j];
            Tray.cReDeviceInf[i][j]  =Source->Tray.cReDeviceInf[i][j];
            Tray.iAOIResult[i][j]    =Source->Tray.iAOIResult[i][j];            //Sam 20240325 : 新增 DamageTrayMapping 功能
        }
    }
    sUnloaderAlarmMsg=Source->sUnloaderAlarmMsg;                                //Jimmychiu 20240902 : Need Alarm After Unloader Full
    Source->ClearTray(__FUNC__);
}
//---------------------------------------------------------------------------
int TTrayMotor::HowManyDevice(int iType)                                        //Ifor 20160829 add 確認Tray上Device數量
{
    int iCT=0;
    for(int i=0; i<Tray.XItem; i++)
        for(int j=0; j<Tray.YItem; j++)
            if(Tray.Data[i][j]==iType)
                iCT++;
    return iCT;
}
//---------------------------------------------------------------------------
int TTrayMotor::HowManyDevice()                                                 //Steven 20190627 : 計算Tray上Device數量
{
    int iCT=0;
    for(int i=0; i<Tray.XItem; i++)
        for(int j=0; j<Tray.YItem; j++)
            if(Tray.Data[i][j]!=NULL_IC && Tray.Data[i][j]!=HAS_NULL_IC)
                iCT++;
    return iCT;
}
//---------------------------------------------------------------------------
#define RESET_TIMES 900
void TMyMotor::MotorInitial()
{
    Motor->SetHomeobjectTask(1);
    iMyHomeTask=1;
    ResetTime.Set0_1SecAndOn(RESET_TIMES);
    HomeFlag=0;
}
//---------------------------------------------------------------------------
bool TMyMotor::Home(void)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    Motor->iHomePitch=200;
    GetRealPos(&Motor->iHomePitch);
    fCMD=false;
    SetADCRate(100);                                                            //2013-08-29    Dell    Debug Find Home 因為在Speed的畫面可以設定加減速,造成回home 如果設定太低會造成剎車距離太長而撞機
    return Motor->HomeObject();
}
//---------------------------------------------------------------------------
void TMyMotor::HomeReset()
{
    Motor->SetHomeobjectTask(1);
}
//---------------------------------------------------------------------------
int TMyMotor::MotorHome(bool Flag)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return 0;
    }

    int ret=0;
    int &Task=iMyHomeTask;
    if(Motor->Enable)
    {
        ScanMotorStatus();
        if(GetMotorAlarm())
        {
            ret=GetErrorIndex();
            if(ret<2 || ret>5)
            {
                Task=1;
                return 3;
            }
        }
    }

    switch(Task)
    {
        case 1:
            if(Motor->Enable==false)
            {
                Task=1;
                Position=0;
                HomeFlag=1;
                iLastRotatorDirP=true;                                          //RogerYang 20260113 : 用來紀錄Rotator最近一次旋轉方向
                return 1;
            }
            HomeReset();
            Task=10;
            iHomeRetryCT=3;
            break;
        case 10:
            if(Home())
            {
                Task=20;
                htWaitHomeSensorOnDelay.SetMSAndOn(300);

                if(Led[iServoOn]==false)
                {
                    Task=1;
                    return 4;
                }
            }
            break;
        case 20:
            if(Motor->HomeFlag())
            {
                Task=1;
//                ResetTime.Clear();
                HomeFlag=1;
                fCanMove=true;
                fCanMoveR=true;
                fCanMoveM=true;
                fCanMoveL=true;
                iLastRotatorDirP=true;                                          //RogerYang 20260113 : 用來紀錄Rotator最近一次旋轉方向
                return 1;
            }
            else if(htWaitHomeSensorOnDelay.Off())
            {
                if(iHomeRetryCT)
                {
                    iHomeRetryCT--;
                    Task=30;
                }
                else
                {
                    HomeFlag=2;
                    return 2;
                }
            }
            break;
        case 30:
            if(Motor->Enable==false)
            {
                Task=1;
                Position=0;
//                ResetTime.Clear();
                HomeFlag=1;
                return 1;
            }
            HomeReset();
            Task=10;
            break;
    }

    if(Flag)
    {
        ResetTime.Set0_1SecAndOn(RESET_TIMES);
    }

    if(ResetTime.Off())
    {
        HomeFlag=2;
        return 2;
    }
    return 0;
}
//---------------------------------------------------------------------------
bool TMyMotor::GetMotorAlarm()
{
    if(Motor!=NULL)                                                             //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
    {
        if(Mot_Name==MTestY1 || Mot_Name==MTestZ1 || Mot_Name==MTestZ2 || Mot_Name==MTestY2)
        {
            Gali_ScanAlarmStatus();
            return Gali_MotorAlarm;
        }
        else
        {
            return Motor->GetAlarm();
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void TMyMotor::ScanMotorStatus()
{
    if(Motor!=NULL)                                                             //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
    {
        if(INDEX_MOTION_CARD==0 && (Mot_Name==MTestY1 || Mot_Name==MTestZ1 ||   //Steven 20210623 : Index使用Galil
                                    Mot_Name==MTestZ2 || Mot_Name==MTestY2))
        {
            Gali_ScanMotStatus();
        }
        else
        {
            Motor->ScanMotorStatus(Led);
        }
    }
}
//------------------------------------------------------------------------------
int TMyMotor::GetErrorIndex()
{
    if(Led[iAlarmLed] && Led[iServoalarmLed] && Led[iInposLed])
        return 0;
    else if(Led[iAlarmLed] && Led[iServoalarmLed])
        return 1;
    else if(Led[iAlarmLed] && Led[iCwLed])
        return 2;
    else if(Led[iAlarmLed] && Led[iCcwLed])
        return 3;
    //pig 2016.01.30 MotorAlarm start
    else if(Led[iCwLed] || Led[iCcwLed])                                        //CW    //CCW
        return 6;
    else if(Led[iAlarmLed] && Motor->PServoAlarmOn)                             //Alarm
        return 7;
    else if(Led[iAlarmLed] && Led[iSoftcwLed])
        return 4;
    else if(Led[iAlarmLed] && Led[iSoftccwLed])
        return 5;
    //pig 2016.01.30 MotorAlarm end
    else return 9;
}
//--------------------------------------------------------
void TMyMotor::PCIL132_SetPos(int Pos)
{
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable==false)
    {
        Position=0;
    }
    else
    {
        if(Motor->Enable==false)
            return;

        Motor->SetCommand(Pos);
        Motor->SetPosition(Pos);
    }
}
//--------------------------------------------------------
void TMyMotor::PCIL132_ResetPos()                                               //Steven 20110628 : Servo On後,重設Command Pos
{
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable==false)
    {
        return;
    }

    if(MOTION_CARD_TYPE==MotionCard_SYN)
    {
        Motor->ResetPos();
    }
    else
    {
        int p=Motor->ReadEnCoderRealPos();
        if(Motor->Direction)
            p=-p;
        Motor->ResetPos(p);
    }
}
//--------------------------------------------------------
void TMyMotor::PCIL132_StopMotor()
{
    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)
    {
        if(Mot_Name==MTestY1 || Mot_Name==MTestZ1 || Mot_Name==MTestZ2 || Mot_Name==MTestY2)
        {
            return;
        }
        fCMD=false;
//        if(MOTION_CARD_TYPE==0)                                               //Steven 20140425 : Carter說按下暫停時蝦頭會抖,所以要改成減速停
//            Motor->Stop();
//        else
        Motor->DecStop();
    }
}
//--------------------------------------------------------
void TMyMotor::JogP(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    Motor->JogP();
}
//--------------------------------------------------------
void TMyMotor::JogN(int Speed)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return;
    }

    Motor->JogN();
}
//--------------------------------------------------------
void TMyMotor::ServoOnOff(bool IsOn)
{
    AnsiString S[]={"X", "Y", "Z", "W"};
    if(Motor!=NULL &&
       Motor->Enable)
    {
        if(INDEX_MOTION_CARD==0 &&
           Mot_Name>=MTestY1 && Mot_Name<=MTestY2)
        {
            Gali_ScanMotStatus();
            if(IsOn)
            {
                if(Led[iServoOn]==false)
                    Gali_Command("SH"+S[Mot_Name-MTestY1], __FUNC__);           //sevro on
            }
            else
            {
                if(Led[iServoOn])
                {
                    Gali_Command("AB1", __FUNC__);                              //KEVIN 20110628  需將所有馬達停止 才能 sevro off
                    Gali_Command("MO"+S[Mot_Name-MTestY1], __FUNC__);           //sevro off
                }
            }
        }
        else
        {
            Motor->SetServoOn(IsOn);
            if(Motor->PServoAlarmOn)
            {
                if(IsOn)
                {
                    MySleep(200);                                               //Steven 20110810 : ServonOn後要Delay一下
                    PCIL132_ResetPos();                                         //Steven 20110709 : 伺服馬達Servo On之後,要重置CommandPos
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
//Gali_ Mot
//Note:
//TI:Tell Input Status(TI+"WhichAxis")
//TS:Tell Switch Bite1=Home Bite2=Reverse Limit Switch Bite3=Forward Limit Switch
//----------------------------------------------------------------------------
bool TMyMotor::IsStartGali_Pr()                                                 //Chunghung 20131111 add
{
    if(Gali_Command("MG _XQ", __FUNC__)==-1)
        return false;
    return true;
}
//------------------------------------------------------------------------------
void TMyMotor::StartGali_Pr()                                                   //Chunghung 20131111 add
{
    Gali_Command("XQ", __FUNC__);
}
//------------------------------------------------------------------------------
void TMyMotor::EndGali_Pr()                                                     //Chunghung 20131111 add
{
    Gali_Command("ST", __FUNC__);
}
//------------------------------------------------------------------------------
long TMyMotor::GetGali_Pr_Result()                                              //Chunghung 20131111 add
{
    AnsiString str;
    if(Mot_Name==MTestY1)
        str="E1=";
    else if(Mot_Name==MTestZ1)
        str="E2=";
    else if(Mot_Name==MTestZ2)
        str="E3=";
    else if(Mot_Name==MTestY2)
        str="E4=";
    return  Gali_Command(str, __FUNC__)*Motor->GearRatio;
}
//------------------------------------------------------------------------------
void TMyMotor::GetGali_Pr_ER(long &ERA, long &ERB, long &ERC, long &ERD)
{
    ERA=Gali_Command("ERA=?", __FUNC__)*MOT[MTestY1].Motor->GearRatio;
    ERB=Gali_Command("ERB=?", __FUNC__)*MOT[MTestZ1].Motor->GearRatio;
    ERC=Gali_Command("ERC=?", __FUNC__)*MOT[MTestZ2].Motor->GearRatio;
    if((USE_INDEX_ARM_AXES==IndexArm_3_Axis))
    {
        ERD=0;
    }
    else
    {
        ERD=Gali_Command("ERD=?", __FUNC__)*MOT[MTestY1].Motor->GearRatio;
    }
}
//------------------------------------------------------------------------------
void TMyMotor::SetGali_Pr_ER(long ERA, long ERB, long ERC, long ERD)
{
    AnsiString str;
    ERA=(ERA*100)/(long)(MOT[MTestY1].Motor->GearRatio*100);
    ERB=(ERB*100)/(long)(MOT[MTestZ1].Motor->GearRatio*100);
    ERC=(ERC*100)/(long)(MOT[MTestZ2].Motor->GearRatio*100);
    if((USE_INDEX_ARM_AXES==IndexArm_3_Axis))
    {
        ERD=0;
    }
    else
    {
        ERD=(ERD*100)/(long)(MOT[MTestY1].Motor->GearRatio*100);
    }
    str.printf("ER%d,%d,%d,%d", ERA, ERB, ERC, ERD);
    Gali_Command(str, __FUNC__);
}
//------------------------------------------------------------------------------
int TMyMotor::GetRotatorBacklash(int iGoalPos, bool bInRotator, int iTechData)  //RogerYang 20260113 : Rotator新增背隙補償
{
    int iBacklash=0;
    int iProdData=0;
    if(bInRotator==true)
    {   iProdData=(iTechData==0)?Prod.iIn_iRotateA_Backlash:iTechData;
        if(iGoalPos>MOT[MInRotateKit].ReadPos() &&
            MOT[MInRotateKit].iLastRotatorDirP==false)
        {
            iBacklash=iProdData;
        }
        else if(iGoalPos<MOT[MInRotateKit].ReadPos() &&
            MOT[MInRotateKit].iLastRotatorDirP==true)
        {
            iBacklash=-iProdData;
        }
    }
    else
    {
        iProdData=(iTechData==0)?Prod.iOut_iRotateA_Backlash:iTechData;
        if(iGoalPos>MOT[MOutRotateKit].ReadPos() &&
            MOT[MOutRotateKit].iLastRotatorDirP==false)
        {
            iBacklash=iProdData;
        }
        else if(iGoalPos<MOT[MOutRotateKit].ReadPos() &&
            MOT[MOutRotateKit].iLastRotatorDirP==true)
        {
            iBacklash=-iProdData;
        }
    }
    return iBacklash;
}
//---------------------------------------------------------------------------
void TMyMotor::GetRealPos(int *iPos)
{
    TargetPosition=*iPos;
    int p=0;
    double r=0.0;
    p=*iPos;

    if(Motor->GearRatio==0.0)
        Motor->GearRatio=1.0;

    r=Motor->GearRatio;
    int p1=0, p2=0;
    p1=ChangeToFloatNonPcnt((double)(double(p)), (double)(r));                                //1666
    p2=double(p1)*r;                               //999
    if(p2<p)
    {
        while(1)
        {
            p1++;
            p2=double(p1)*r;
            if(p2>=p)
                break;
        }
    }
    else if(p2>p)
    {
        while(1)
        {
            p1--;
            p2=double(p1)*r;
            if(p2<=p)
                break;
        }
    }
    *iPos=p1;

    if(Mot_Name==MTestY1 || Mot_Name==MTestZ1 || Mot_Name==MTestZ2 || Mot_Name==MTestY2)
    {

    }
    else
    {
        if(MOTION_CARD_TYPE==MotionCard_Contec)
        {
            if(Motor->Direction)
                *iPos=-*iPos;
            else
                *iPos=*iPos;
        }
    }
}
//--------------------------------------------------------------------------
int TMyMotor::CompareEncoderPos(int iPos, int iGap)                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
{
    int iEncoderPos=ReadEncoderPos();

//    if(Motor->GearRatio<=1)                                                     //完全相等
//        iGap=1;

    if(iPos-iGap<=iEncoderPos && iEncoderPos<=iPos+iGap)                        //Sam 20230621 : 改 CompareCommandPos 判斷式
        return 1;
    else
        return -1;
}
//------------------------------------------------------------------------------
int TMyMotor::CompareCommandPos(int iPos, int iGap)                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
{
    int iCommandPos=ReadPos();
//    if(Motor->GearRatio<=1)                                                     //完全相等    //Ifor 20221117 0 => 1
//        iGap=1;

    if(iPos-iGap<=iCommandPos && iCommandPos<=iPos+iGap)                        //Sam 20230621 : 改 CompareCommandPos 判斷式
        return 1;
    else
        return -1;
}
//------------------------------------------------------------------------------
void TMyMotor::SetArmMaxSpeed()
{
    Motor->SetArmMaxSpeed();
}
//------------------------------------------------------------------------------
int CheckInArmZNeedHome()                                                       //ChungHung 20140605 add Fix Shuttle hit In/OutArm
{
    bool bInLedFlag=false;
    int iMotNo;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMotNo=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMotNo].Motor->Enable)
            {
                if(MOT[iMotNo].Position==Prod.ZInArmSafe[i][j])
                {
                    MOT[iMotNo].ScanMotorStatus();
                    bInLedFlag=MOT[iMotNo].Led[iHomeLed];
                    if(bInLedFlag==false)
                    {
                        return iMotNo;
                    }
                }
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
int InArmZSafe(int iFlag)                                                       // -1 = safe
{
    bool bInLedFlag[MAX_ARM_Row][MAX_ARM_Col];
    bool bInPosFlag[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(bInLedFlag, sizeof(bInLedFlag));
    ZeroMemory(bInPosFlag, sizeof(bInPosFlag));
    int iMotNo;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMotNo=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            if(iFlag & 1)
            {
                if(MOT[iMotNo].Motor->Enable)
                {
                    MOT[iMotNo].ScanMotorStatus();
                    bInLedFlag[i][j]=MOT[iMotNo].Led[iHomeLed];
                    if(bInLedFlag[i][j]==false)
                    {
                        return iMotNo;
                    }
                }
            }

            if(iFlag & 2)
            {
                if(MOT[iMotNo].ReadPos()>=0)
                {
                    bInPosFlag[i][j]=true;
                }
                else
                {
                    return iMotNo;
                }
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
AnsiString sInArmZHomeState()                                                   //Sam 20230707 : 新增 InOutArm Z Home前Home sensor 狀態
{
    AnsiString sRet="", s="";
    int iMotNo;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                iMotNo=MInArmZA;
            else
                iMotNo=InArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMotNo].Motor->Enable)
            {
                MOT[iMotNo].ScanMotorStatus();
                if(MOT[iMotNo].Led[iHomeLed]==false)
                {
                    s=InArmSuck.Suck[i][j].sName+"_Off";
                    sRet+=s+" ";
                }
            }
        }
    }
    return sRet;
}
//------------------------------------------------------------------------------
AnsiString sOutArmZHomeState()                                                  //Sam 20230707 : 新增 InOutArm Z Home前Home sensor 狀態
{
    AnsiString sRet="", s="";
    int iMotNo;
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                iMotNo=MOutArmZA;
            else
                iMotNo=OutArmSuck.Suck[i][j].iMotNo;

            if(MOT[iMotNo].Motor->Enable)
            {
                MOT[iMotNo].ScanMotorStatus();
                if(MOT[iMotNo].Led[iHomeLed]==false)
                {
                    s=OutArmSuck.Suck[i][j].sName+"_Off";
                    sRet+=s+" ";
                }
            }
        }
    }
    return sRet;
}
//------------------------------------------------------------------------------
//ChungHung 20140605 add Fix Shuttle hit In/OutArm
int CheckOutArmZNeedHome()
{
    bool bOutLedFlag=false;
    int iMotNo;
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                iMotNo=MOutArmZA;
            else
                iMotNo=OutArmSuck.Suck[i][j].iMotNo;

            if(MOT[iMotNo].Motor->Enable)
            {
                if(MOT[iMotNo].Position==Prod.ZOutArmSafe[i][j])
                {
                    MOT[iMotNo].ScanMotorStatus();
                    bOutLedFlag=MOT[iMotNo].Led[iHomeLed];
                    if(bOutLedFlag==false)
                    {
                        return iMotNo;
                    }
                }
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
int OutArmZSafe(int iFlag)                                                      // -1 = safe
{
    bool bOutLedFlag[MAX_ARM_Row][MAX_ARM_Col];
    bool bOutPosFlag[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(bOutLedFlag, sizeof(bOutLedFlag));
    ZeroMemory(bOutPosFlag, sizeof(bOutPosFlag));
    int iMotNo;
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                iMotNo=MOutArmZA;
            else
                iMotNo=OutArmSuck.Suck[i][j].iMotNo;

            if(iFlag & 1)
            {
                if(MOT[iMotNo].Motor->Enable)
                {
                    MOT[iMotNo].ScanMotorStatus();
                    bOutLedFlag[i][j]=MOT[iMotNo].Led[iHomeLed];
                    if(bOutLedFlag[i][j]==false)
                    {
                        return iMotNo;
                    }
                }
            }

            if(iFlag & 2)
            {
                if(MOT[iMotNo].ReadPos()>=0)
                {
                    bOutPosFlag[i][j]=true;
                }
                else
                {
                    return iMotNo;
                }
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
/*bool CheckInArmZHomeSensor(bool ZNeedDown, bool *ZDownSel)
{
    int iMotNo;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMotNo=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            MOT[iMotNo].fCMD=false;
            if(ZNeedDown && ZDownSel[i+2*j])
            {
                MOT[iMotNo].ScanMotorStatus();
                if(MOT[iMotNo].Motor->Enable && MOT[iMotNo].Led[iHomeLed])
                {
                    MOT[MInArmX].PCIL132_StopMotor();
                    MOT[MInArmY].PCIL132_StopMotor();
                    ShowMyMessage(MOT[iMotNo].NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!",
                                  MOT[iMotNo].NumberAlias+"歸零sensor錯誤；如果吸嘴在下方，可能是sensor壞掉", __FUNC__);
                    return false;
                }
            }
        }
    }
    return true;
}  */
//------------------------------------------------------------------------------
bool CheckInArmZHomeSensor_2x8(bool ZNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col])   //Steven for HT1032
{
    int iMotNo;
    int iPos;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMotNo=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            MOT[iMotNo].fCMD=false;
            if(ZNeedDown && ZDownSel[i][j])
            {
                MOT[iMotNo].ScanMotorStatus();
                iPos=MOT[iMotNo].ReadEncoderPos();
                if(MOT[iMotNo].Motor->Enable && MOT[iMotNo].Led[iHomeLed] && iPos<0)
                {
                    MOT[MInArmX].PCIL132_StopMotor();
                    MOT[MInArmY].PCIL132_StopMotor();
                    ShowMyMessage(MOT[iMotNo].NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!",
                                  MOT[iMotNo].NumberAlias+"歸零sensor錯誤；如果吸嘴在下方，可能是sensor壞掉", __FUNC__);
                    SetInArmHome();
                    return false;
                }
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool CheckOutArmZHomeSensor_2x8(bool ZNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col])  //Steven for HT1032
{
    int iMotNo;
    int iPos;
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                iMotNo=MOutArmZA;
            else
                iMotNo=OutArmSuck.Suck[i][j].iMotNo;
            MOT[iMotNo].fCMD=false;
            if(ZNeedDown && ZDownSel[i][j])
            {
                MOT[iMotNo].ScanMotorStatus();
                iPos=MOT[iMotNo].ReadEncoderPos();
                if(MOT[iMotNo].Motor->Enable && MOT[iMotNo].Led[iHomeLed] && iPos<0)
                {
                    MOT[MOutArmX].PCIL132_StopMotor();
                    MOT[MOutArmY].PCIL132_StopMotor();
                    ShowMyMessage(MOT[iMotNo].NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!",
                                  MOT[iMotNo].NumberAlias+"歸零sensor錯誤；如果吸嘴在下方，可能是sensor壞掉", __FUNC__);
                    SetOutArmHome();
                    return false;
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
int ChangePosition(int Pos, int iMoveTable)                                     //Frank 20180102 add
{
    double dPos1=0.0, dPos2=0.0, dPos3=0.0, dPos4=0.0, dPos5=0.0, dPos6=0.0;
    int j=0;
    int iMove=iMoveTable*2;

    if(iMoveTable%2==0)
    {                  //往負
        for(j=0; j<1000; j++)
        {
            if(Pos>=iMagneticScalePos[iMove][j+1] && Pos<=iMagneticScalePos[iMove][j])
            {
                if(iMagneticScalePos[iMove][j+1]-iMagneticScalePos[iMove][j]!=0)
                {
                    dPos1=iMagneticScalePos[iMove+1][j+1]-iMagneticScalePos[iMove+1][j];
                    dPos2=iMagneticScalePos[iMove][j+1]-iMagneticScalePos[iMove][j];
                    dPos3=(Pos);

                    if(dPos1!=0)
                    {
                        dPos4=(dPos3-(double)iMagneticScalePos[iMove+1][j])/dPos1;
                        dPos5=dPos2*dPos4;
                        dPos6=dPos5+iMagneticScalePos[iMove][j];
                    }
                    else
                    {
                        dPos6=0.0;
                    }
                }
                else
                {
                    dPos6=0.0;
                }
                return dPos6;
            }
        }
    }
    else
    {                 //往正
        for(j=0; j<1000; j++)
        {
            if(iMagneticScalePos[iMove][j+1]>=Pos && Pos>=iMagneticScalePos[iMove][j])
            {
                if(iMagneticScalePos[iMove][j]-iMagneticScalePos[iMove][j+1]!=0)
                {
                    dPos1=iMagneticScalePos[iMove+1][j+1]-iMagneticScalePos[iMove+1][j];
                    dPos2=iMagneticScalePos[iMove][j+1]-iMagneticScalePos[iMove][j];
                    dPos3=(Pos);

                    if(dPos1!=0)
                    {
                        dPos4=(dPos3-iMagneticScalePos[iMove+1][j])/dPos1;
                        dPos5=dPos2*dPos4;
                        dPos6=dPos5+iMagneticScalePos[iMove][j];
                    }
                    else
                    {
                        dPos6=0.0;
                    }
                }
                else
                {
                    dPos6=0.0;
                }
                return dPos6;
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
const int iIDLETime=30;
TQPF_Timer InArmIdle;
extern bool MoveInArmZToPlateSafe(int Task);
//------------------------------------------------------------------------------
void SetInArmPitchSpeed(bool bMoveClose)
{
    if(bMoveClose)                                                              //內縮
    {
        SetMotorAccelSpeed(MInArmPitch, 100);
        SetMotorScaleSpeed(MInArmPitch, ArmSpeed[InArm].iVariSP);
        SetMotorAccelSpeed(MInArmPitchX2, 100);
        SetMotorScaleSpeed(MInArmPitchX2, double(ArmSpeed[InArm].iVariSP)*0.90);
        SetMotorAccelSpeed(MInArmPitchX3, 100);
        SetMotorScaleSpeed(MInArmPitchX3, double(ArmSpeed[InArm].iVariSP)*0.80);
        SetMotorAccelSpeed(MInArmPitchX4, 100);
        SetMotorScaleSpeed(MInArmPitchX4, double(ArmSpeed[InArm].iVariSP)*0.70);
    }
    else
    {

        SetMotorAccelSpeed(MInArmPitchX4, 100);
        SetMotorScaleSpeed(MInArmPitchX4, double(ArmSpeed[InArm].iVariSP));
        SetMotorAccelSpeed(MInArmPitchX3, 100);
        SetMotorScaleSpeed(MInArmPitchX3, double(ArmSpeed[InArm].iVariSP)*0.80);
        SetMotorAccelSpeed(MInArmPitchX2, 100);
        SetMotorScaleSpeed(MInArmPitchX2, double(ArmSpeed[InArm].iVariSP)*0.70);
        SetMotorAccelSpeed(MInArmPitch, 100);
        SetMotorScaleSpeed(MInArmPitch, double(ArmSpeed[InArm].iVariSP)*0.60);
    }
}
//------------------------------------------------------------------------------
int InArmPitchMove(int Vari[X_PITCH_COUNT], int YVari, bool bInit)              //Steven for HT1032
{
    static bool bMoveClose=false;
    static bool bFirstIn=true;
    static int  iFlag[PITCH_COUNT];
    bool bFlag;
    int iResult=0;
    int iPos=0;
    #ifndef SOFT_SIMULTE
    long iCurrPos[X_PITCH_COUNT]={MOT[MInArmPitch  ].Motor->ReadPos(),
                      MOT[MInArmPitchX2].Motor->ReadPos(),
                      MOT[MInArmPitchX3].Motor->ReadPos(),
                      MOT[MInArmPitchX4].Motor->ReadPos()};

    int iDevPos[X_PITCH_COUNT];
    BYTE bDevNo[X_PITCH_COUNT]={MOT[MInArmPitch  ].Motor->iPortID,
                    MOT[MInArmPitchX2].Motor->iPortID,
                    MOT[MInArmPitchX3].Motor->iPortID,
                    MOT[MInArmPitchX4].Motor->iPortID};
    #endif

    if(USE_PICKER_COUNT==ep1Picker)
        return 1;

    if(bInit || bFirstIn)
    {
        ZeroMemory(iFlag, sizeof(iFlag));
        bFlag=MOT[MInArmPitch].Motor->MotionDone();
        if(bFlag==true)
        {
            iPos=MOT[MInArmPitch].ReadPos();
            bMoveClose=(Vari[0]<iPos);
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                #ifdef SOFT_SIMULTE                                             //Ztex Add SOFT_SIMULTE Pass
                    iFlag[1]=MOT[MInArmPitch  ].MotorMove(Vari[0]);
                    iFlag[2]=MOT[MInArmPitchX2].MotorMove(Vari[1]);
                    iFlag[3]=MOT[MInArmPitchX3].MotorMove(Vari[2]);
                    iFlag[4]=MOT[MInArmPitchX4].MotorMove(Vari[3]);
                #else
                    iDevPos[0]=Vari[0]-iCurrPos[0];
                    iDevPos[1]=Vari[1]-iCurrPos[1];
                    iDevPos[2]=Vari[2]-iCurrPos[2];
                    iDevPos[3]=Vari[3]-iCurrPos[3];
                    MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
                    MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
                    MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
                    MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);
                    long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};
                    MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo);
                    MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
                #endif
            }
        }
        bFirstIn=false;
        return 0;
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        if(iFlag[0]==0)
            iFlag[0]=MOT[MInArmPitchY ].MotorMove(YVari);                       //ChungHung 20131231 alter AutoYPitch
        if(iFlag[1]==0)
            iFlag[1]=MOT[MInArmPitch  ].MotorMove(Vari[0]);
        if(iFlag[2]==0)
            iFlag[2]=MOT[MInArmPitchX2].MotorMove(Vari[1]);                     //Steven 20131002 : XY變距
        iFlag[3]=1;
        iFlag[4]=1;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        if(iFlag[4]==0)
            iFlag[4]=MOT[MInArmPitchY].MotorMove(YVari);

        if(iFlag[0]==0)
        {
            #ifndef SOFT_SIMULTE
            bFlag=MOT[MInArmPitch].Motor->MotionDone();
            #else
            bFlag=true;
            #endif
            if(bFlag)
                iFlag[0]=1;
        }
        iFlag[1]=1;
        iFlag[2]=1;
        iFlag[3]=1;
    }
    else
    {
        if(iFlag[0]==0)
            iFlag[0]=MOT[MInArmPitch].MotorMove(Vari[0]);
        iFlag[1]=1;
        iFlag[2]=1;
        iFlag[3]=1;
        iFlag[4]=1;
    }

    for(int i=0; i<PITCH_COUNT; i++)
    {
        if(iFlag[i]<0)  //判斷是不是有Alarm
            iResult=iFlag[i];
    }

    if(iResult==0)      //沒Alarm就確認是不是走完了
    {
        for(int i=0; i<PITCH_COUNT; i++)
        {
            if(iFlag[i]==0)
            {
                return 0;
            }
        }

        #ifndef SOFT_SIMULTE
        if(abs(Vari[0]-iCurrPos[0])>5)
        {
            bFirstIn=true;
            return -1;
        }
        #endif
        bFirstIn=true;
        ZeroMemory(iFlag, sizeof(iFlag));
        iResult=1;
    }
    bFirstIn=true;
    ZeroMemory(iFlag, sizeof(iFlag));
    return iResult;
}
//------------------------------------------------------------------------------
int  iInArmZMoveTask=-1;
bool bInArmZMove[MAX_ARM_Row][MAX_ARM_Col];
bool InArmCynMove(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col])                      //Steven for HT1032
{
    if(InOutArmPickerUseMotor!=eptUseMotCyn)
        return true;

    bool bFlag=true;
    int iCyn;

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            iCyn=InArmSuck.Suck[i][j].iMotNo;
            if(i<InArmSuck.iPickRow && j<InArmSuck.iPickCol)                    //有用到的才做判斷
            {
                if(ZDownSel[i][j])
                    Cylinder[iCyn].On();
                else
                    Cylinder[iCyn].Off();
            }
            else                                                                //沒用到的一律往上打
            {
                Cylinder[iCyn].Off();
            }
        }
    }

    return bFlag;
}
//------------------------------------------------------------------------------
bool InArmZMoveDown(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bLoader, bool bPreOn)//Steven for HT1032
{
    int iMot, iCyn;
    bool bFlag=true;
    if(iInArmZMoveTask==-1 || iInArmZMoveTask==2)                               //JerryYang 20230820
    {
        ZeroMemory(bInArmZMove, sizeof(bInArmZMove));
        iInArmZMoveTask=1;
    }

    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                iCyn=InArmSuck.Suck[i][j].iMotNo;
                if(i<InArmSuck.iPickRow && j<InArmSuck.iPickCol)                //有用到的才做判斷
                {
                    if(ZDownSel[i][j])
                    {
                        if(bPreOn)
                            InArmSuck.Suck[i][j].On();

                        bInArmZMove[i][j]=true;
                        Cylinder[iCyn].On();
                    }
                    else
                    {
                        bInArmZMove[i][j]=true;
                        Cylinder[iCyn].Off();
                    }

                }
                else                                                            //沒用到的一律往上打
                {
                    bInArmZMove[i][j]=true;
                    Cylinder[iCyn].Off();
                }
            }
        }

        bFlag=MOT[MInArmZA].MotorMove2SpeedForPicker(iZPos[0][0], &ArmSpeed[InArm], bLoader);
    }
    else
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
//                if(i<InArmSuck.iPickRow && j<InArmSuck.iPickCol)                //有用到的才做判斷
//                {
                    if(ZDownSel[i][j])
                    {
                        if(bPreOn)
                            InArmSuck.Suck[i][j].On();

                        if(bInArmZMove[i][j]==false)
                            bInArmZMove[i][j]=MOT[iMot].MotorMove2SpeedForPicker(iZPos[i][j], &ArmSpeed[InArm], bLoader);      //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader
                    }
                    else
                    {
                        bInArmZMove[i][j]=true;
                    }
//                }
//                else
//                {
//                    bInArmZMove[i][j]=true;
//                }

                if(bInArmZMove[i][j]==false)
                    bFlag=false;
            }
        }
    }

    if(bFlag)
    {
        iInArmZMoveTask=-1;
    }
    return bFlag;
}
//------------------------------------------------------------------------------
bool InArmZMoveUp(int iZPos, bool bLoader)                                      //Steven for HT1032
{
    int iMot, iCyn;
    bool bFlag=true;
    if(iInArmZMoveTask==-1 || iInArmZMoveTask==1)                               //JerryYang 20230820
    {
        ZeroMemory(bInArmZMove, sizeof(bInArmZMove));
        iInArmZMoveTask=2;
    }

    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                iCyn=InArmSuck.Suck[i][j].iMotNo;
                if(bInArmZMove[i][j]==false)
                {
                    Cylinder[iCyn].Off();
                    bInArmZMove[i][j]=true;
                }
            }
        }

        bFlag=MOT[MInArmZA].MotorMove2SpeedForPicker(iZPos, &ArmSpeed[InArm], bLoader);
    }
    else
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
                if(bInArmZMove[i][j]==false)
                    bInArmZMove[i][j]=MOT[iMot].MotorMove2SpeedForPicker(iZPos, &ArmSpeed[InArm], bLoader);      //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader

                if(bInArmZMove[i][j]==false)
                    bFlag=false;
            }
        }
    }
    if(bFlag)
    {
        iInArmZMoveTask=-1;
    }
    return bFlag;
}
//------------------------------------------------------------------------------
HTimer InArmCylinderDelayTimer;
//------------------------------------------------------------------------------
bool InArmContinuousMove_9045(int X, int Y, int Vari[X_PITCH_COUNT], int YVari, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool ZNeedDown, bool bLoader)     //Steven for HT1032
{
    AnsiString str=AnsiString("");
    AnsiString str1=AnsiString(""), str2=AnsiString("");
    int iCheckX=0, iCheckY=0, iEncodeX=0, iEncodeY=0;
    int iXPosdiffer=0, iYPosdiffer=0, iXpos=0, iXMSPos=0, iYpos=0, iYMSPos=0;
    int ret=PNP_DOING;
    int iX=X, iY=Y;
    bool bRet;
    static bool bFirstIn=true;
    static bool bFlag[8];
    static int  iInArmAllZSafe=1;
    static int  iPitchMoveOk=0;
    static bool bXYMoveFinish=false;
    static bool bXYPosErrorFlag=false;

    static int iLastPosX=-99999;
    static int iLastPosY=-99999;
    if(iLastPosX!=X || iLastPosY!=Y)
    {
        iLastPosX=X;
        iLastPosY=Y;
        iInArmAllZSafe=1;
        ZeroMemory(bFlag, sizeof(bFlag));
        if(MOT[MInArmX].Motor->MotionDone()==false)
            MOT[MInArmX].PCIL132_StopMotor();
        if(MOT[MInArmY].Motor->MotionDone()==false)
            MOT[MInArmY].PCIL132_StopMotor();
    }

    if(bFirstIn)
    {
        bFirstIn=false;
        ZeroMemory(bFlag, sizeof(bFlag));
        InArmPitchMove(Vari, YVari, true);
    }

    //KaiChen 20171228 ：Log Light Scale Data
    //==>
    int iXO=0, iYO=0, iLogCount=500;
    iXO=iX;
    iYO=iY;
    AnsiString S1="";

    if(IniConfig.bA27EnableLightScale==true)
    {
        iCheckX=MOT[MInArmX].ReadPos();
        iCheckY=MOT[MInArmY].ReadPos();

        if(iX>iCheckX)
        {
            iX=ChangePosition(iX, 0);
            if(IniConfig.bA27_1LogEnableLightScaleData==true && iLogLightScaleCount_InArmX1<iLogCount && bLogLightScale_InArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo1->Lines->Add(S1);
                iLogLightScaleCount_InArmX1++;
            }
        }
        else
        {
            iX=ChangePosition(iX, 1);
            if(IniConfig.bA27_1LogEnableLightScaleData==true && iLogLightScaleCount_InArmX2<iLogCount && bLogLightScale_InArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo2->Lines->Add(S1);
                iLogLightScaleCount_InArmX2++;
            }
        }

        if(iY>iCheckY)
        {
            iY=ChangePosition(iY, 2);
            if(IniConfig.bA27_1LogEnableLightScaleData==true && iLogLightScaleCount_InArmY1<iLogCount && bLogLightScale_InArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo3->Lines->Add(S1);
                iLogLightScaleCount_InArmY1++;
            }
        }
        else
        {
            iY=ChangePosition(iY, 3);
            if(IniConfig.bA27_1LogEnableLightScaleData==true && iLogLightScaleCount_InArmY2<iLogCount && bLogLightScale_InArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo4->Lines->Add(S1);
                iLogLightScaleCount_InArmY2++;
            }
        }

        bLogLightScale_InArm=true;
    }

    if(iX>=MOT[MInArmX].Motor->PSoftLimitP || iX<=MOT[MInArmX].Motor->PSoftLimitN)
    {
        str.sprintf("X=%d", iX);
        ShowErrorMessage("WAR0154", 0, MInArmX, 0, str);                        //"In Arm X axis motor will out of limit !"
        return false;
    }
    if(iY>=MOT[MInArmY].Motor->PSoftLimitP || iY<=MOT[MInArmY].Motor->PSoftLimitN)
    {
        str.sprintf("Y=%d", iY);
        ShowErrorMessage("WAR0155", 0, MInArmY, 0, str);                        //"In Arm Y axis motor will out of limit !"
        return false;
    }

    //simulate en
    if(MOT[MInArmX].fCanMove==false || MOT[MInArmY].fCanMove==false)
    {
        MOT[MInArmX].PCIL132_StopMotor();
        MOT[MInArmY].PCIL132_StopMotor();
        iInArmAllZSafe=1;
        return false;
    }

    if(MOT[MInArmX].Motor->Enable && MOT[MInArmY].Motor->Enable)
    {
        if(bPauseInMotor)
        {
            iInArmAllZSafe=1;
            bPauseInMotor=false;
            InArmIdle.SetSecAndOn(iIDLETime);
            if(IniConfig.bA62bUseStopMachineArmHome==true)                      //Ztex 2024.10.30 Add Use Stop Machine In/Out Arm Need To Home
            {
                MOT[MInArmX].PCIL132_StopMotor();
                MOT[MInArmY].PCIL132_StopMotor();
                iInArmAllZSafe=1;
                iPitchMoveOk=0;
                SetInArmHome();
            }
        }

        switch(iInArmAllZSafe)
        {
            case 1:
                if(MoveInArmZToPlateSafe(5555))
                {
                    iPitchMoveOk=0;
                    InArmPitchMove(Vari, YVari, true);
                    InArmIdle.SetSecAndOn(iIDLETime);
                    if(InOutArmPickerUseMotor!=eptUseMotCyn)
                        iInArmAllZSafe=100;
                    else
                        iInArmAllZSafe=50;
                    bXYMoveFinish=false;
                    InArmCynMove(ZDownSel);
                    InArmCylinderDelayTimer.SetMSAndOn(ArmSpeed_File[InArm].dCylinderDelay*1000);
                    InitPCIL112_InArmXYMoveTask();                              //jou 2011-04-03
                    if(bXYPosErrorFlag)
                    {
                        bFlag[0]=false;
                        bFlag[1]=false;
                        iInArmAllZSafe=150;
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            case 50:
                if(InArmCylinderDelayTimer.Off())
                    iInArmAllZSafe=100;
                else
                    break;
            case 100:
                if(iPitchMoveOk==0)
                    iPitchMoveOk=InArmPitchMove(Vari, YVari);

                if(iPitchMoveOk==2)                                             //未變Pitch 則需重新動作
                {
                    iInArmAllZSafe=1;
                }
                else if(iPitchMoveOk>0)                                         //ChungHung 20131231 alter AutoYPitch
                {
                    iInArmAllZSafe=200;
                }
                else if(iPitchMoveOk<0)                                         //Alarm了
                {
                    iInArmAllZSafe=1;
                }
                break;
            case 150:
                if(bFlag[0]==false)
                    bFlag[0]=MOT[MInArmX].MotorMove(X+100);
                if(bFlag[1]==false)
                    bFlag[1]=MOT[MInArmY].MotorMove(Y+100);
                if(bFlag[0] && bFlag[1])
                {
                    bFlag[0]=false;
                    bFlag[1]=false;
                    bXYPosErrorFlag=false;
                    iInArmAllZSafe=100;
                }
                break;
            case 200:
                if(bXYMoveFinish)
                {
                    if(ZNeedDown)
                    {
                        MOT[MInArmX].ScanMotorStatus();
                        MOT[MInArmY].ScanMotorStatus();

                        if(MOT[MInArmX].Led[iInposLed]==true ||
                           MOT[MInArmY].Led[iInposLed]==true)                   //Isaac 20170721 (wei) Z axis protection
                            return false;

                        bRet=InArmZMoveDown(ZDownSel, iZPos, bLoader);
                        InArmCylinderDelayTimer.SetMSAndOn(ArmSpeed_File[InArm].dCylinderDelay*1000);
                        if(bRet==false)
                            return false;
                    }

                    iInArmAllZSafe=1;
                    InitPCIL112_InArmXYMoveTask();                              //jou 2011-04-03

                    if(CheckInArmZHomeSensor_2x8(ZNeedDown, ZDownSel)==false)
                    {
                        return false;
                    }

                    if(IniConfig.bA22MagneticScale==true)                       //Frank 20161109 add 磁性尺
                    {
                        iXpos=MOT[MInArmX].Motor->ReadPos();
                        iXMSPos=MOT[MInArmXScale].Motor->ReadPos();
                        iYpos=MOT[MInArmY].Motor->ReadPos();
                        iYMSPos=MOT[MInArmYScale].Motor->ReadPos();

                        iXPosdiffer=abs(iXpos - iXMSPos);
                        iYPosdiffer=abs(iYpos - iYMSPos);

                        if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100 &&
                           iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();

                            str1.sprintf("In Arm X & Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("In Arm X & Y 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange*100 ||
                               iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();

                            str1.sprintf("In Arm X Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("In Arm X 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();
                            str1.sprintf("In Arm Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("In Arm Y 磁性尺與編碼器差超出設定");
                            if(iYPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                    }

                    if(InArmCylinderDelayTimer.Off())
                    {
                        iPitchMoveOk=0;
                        bLogLightScale_InArm=false;                             //KaiChen 20171228 ：Log Light Scale Data
                        iInArmAllZSafe=1;
                        return true;
                    }
                    else
                    {
                        iInArmAllZSafe=210;
                        break;
                    }
                }

                if(iPitchMoveOk==0)
                {
                    iInArmAllZSafe=1;
                }
                break;
            case 210:
                if(InArmCylinderDelayTimer.Off())
                {
                    iPitchMoveOk=0;
                    iInArmAllZSafe=1;
                    bLogLightScale_InArm=false;                                 //KaiChen 20171228 ：Log Light Scale Data
                    return true;
                }
                break;
            default :
                iInArmAllZSafe=1;
                return false;
        }

        if(bXYPosErrorFlag)
            return false;

        if(bXYMoveFinish==false &&                                              //Steven 20231214 : 避免重複進去
           iPitchMoveOk>=0)                                                     //避免Pitch Alarm時, XY還在動
        {
            ret=PCIL112_InArmXYMove(X, Y);
        }

        if(ret==PNP_DONE)
        {
            if(iPitchMoveOk>0)                                                  //Steven 20150718 : Fixed for Y-Pitch
            {
                if(USE_MAGNETIC_SCALE==false)                                   //Ifor 20180622 (Steven) : 避免位置錯誤
                {
                    iCheckX=MOT[MInArmX].ReadPos();
                    iCheckY=MOT[MInArmY].ReadPos();

                    if((X!=iCheckX) || (Y!=iCheckY))
                    {
                        iEncodeX=MOT[MInArmX].ReadEncoderPos();                 //Steven 20110314 : 比對Encoder位置
                        iEncodeY=MOT[MInArmY].ReadEncoderPos();                 //Steven 20110314 : 比對Encoder位置

                        str1.sprintf(" Move Finish:%d ",bXYMoveFinish);         //Ifor 20181127 : add  bXYMoveFinish Log
                        MNetLog(" iX="+AnsiString(iX)+" iCheckX="+AnsiString(iCheckX)+" iEncodeX="+AnsiString(iEncodeX)+
                                " iY="+AnsiString(iY)+" iCheckY="+AnsiString(iCheckY)+" iEncodeY="+AnsiString(iEncodeY)+ " In Arm Pos Error" + str1);   //Steven 20110402 //Ifor 20181127 : add  bXYMoveFinish Log
                        if(X>(iEncodeX+iTorence) || X<(iEncodeX-iTorence) ||
                           Y>(iEncodeY+iTorence) || Y<(iEncodeY-iTorence))      //Steven 20110314 : 比對Encoder位置
                        {
                            iInArmAllZSafe=1;
                            iPitchMoveOk=0;
                            MOT[MInArmX].PCIL132_StopMotor();
                            MOT[MInArmY].PCIL132_StopMotor();
                            bXYPosErrorFlag=true;
                            bXYMoveFinish=false;                                //Ifor 20181127 : add Encoder位置比對異常時，設定bXYMoveFinish為False避免誤動作
                            return false;
                        }
                    }
                }

                bXYMoveFinish=true;

                if(ZNeedDown)
                {
                    MOT[MInArmX].ScanMotorStatus();
                    MOT[MInArmY].ScanMotorStatus();

                    if(MOT[MInArmX].Led[iInposLed]==true ||
                       MOT[MInArmY].Led[iInposLed]==true)                       //Isaac 20170721 (wei) Z axis protection
                        return false;

                    bRet=InArmZMoveDown(ZDownSel, iZPos, bLoader);
                }
                return false;
            }
        }

        if(InArmIdle.Off())
        {
            MOT[MInArmX].PCIL132_StopMotor();
            MOT[MInArmY].PCIL132_StopMotor();
            iInArmAllZSafe=1;
            iPitchMoveOk=0;
            SetInArmHome();
            InArmIdle.SetSecAndOn(iIDLETime);
        }
    }
    else
    {
        //動作模擬
        if(bFlag[0]==false)
            bFlag[0]=InArmZMoveUp(ZSafePos, bLoader);

        if(bFlag[0])
        {
            if(bFlag[1]==false)
            {
                InArmPitchMove(Vari, YVari, true);
                InitPCIL112_InArmXYMoveTask();
                bFlag[1]=true;
                return false;
            }

            if(bFlag[2]==false)
            {
                ret=PCIL112_InArmXYMove(X, Y);                                  //Steven 20240603 : [E77]改放到PCIL112_OutArmXYMove裡面
                bFlag[2]=(ret==PNP_DONE);
            }

            if(bFlag[3]==false)
                bFlag[3]=InArmPitchMove(Vari, YVari);
        }

        if(bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3])
        {
            if(InOutArmPickerUseMotor==eptUseMot)                               //Steven for HT1032
                bFlag[4]=true;
            else
                bFlag[4]=InArmCynMove(ZDownSel);
        }

        if(bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3] && bFlag[4])
        {
            if(ZNeedDown)
                bRet=InArmZMoveDown(ZDownSel, iZPos, bLoader);
            else
                bRet=InArmZMoveUp(ZSafePos, bLoader);

            if(bRet==false)
                return false;

            ZeroMemory(bFlag, sizeof(bFlag));
            bLogLightScale_InArm=false;                                         //KaiChen 20171228 ：Log Light Scale Data
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool OutArmPitchMove(int Vari[X_PITCH_COUNT], int YVari, bool bInit)            //Steven for HT1032
{
    static bool bMoveClose=false;
    static bool bFirstIn=true;
    static int  iFlag[PITCH_COUNT];
    bool bFlag;
    int iResult=0;
    int iPos=0;
    #ifndef SOFT_SIMULTE
    long iCurrPos[4]={MOT[MOutArmPitch  ].Motor->ReadPos(),
                      MOT[MOutArmPitchX2].Motor->ReadPos(),
                      MOT[MOutArmPitchX3].Motor->ReadPos(),
                      MOT[MOutArmPitchX4].Motor->ReadPos()};

    int iDevPos[4];
    BYTE bDevNo[4]={MOT[MOutArmPitch  ].Motor->iPortID,
                    MOT[MOutArmPitchX2].Motor->iPortID,
                    MOT[MOutArmPitchX3].Motor->iPortID,
                    MOT[MOutArmPitchX4].Motor->iPortID};
    #endif

    if(USE_PICKER_COUNT==ep1Picker)
        return 1;

    if(bInit || bFirstIn)
    {
        ZeroMemory(iFlag, sizeof(iFlag));
        if(bFirstIn)
        {
            iPos=MOT[MOutArmPitch].ReadPos();
            bMoveClose=(Vari[0]<iPos);                                          //目前位置大於目標位置    //內縮
            if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
               USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                #ifdef SOFT_SIMULTE                                             //Ztex Add SOFT_SIMULTE Pass
                    iFlag[1]=MOT[MOutArmPitch  ].MotorMove(Vari[0]);
                    iFlag[2]=MOT[MOutArmPitchX2].MotorMove(Vari[1]);
                    iFlag[3]=MOT[MOutArmPitchX3].MotorMove(Vari[2]);
                    iFlag[4]=MOT[MOutArmPitchX4].MotorMove(Vari[3]);
                #else
                    iDevPos[0]=Vari[0]-iCurrPos[0];
                    iDevPos[1]=Vari[1]-iCurrPos[1];
                    iDevPos[2]=Vari[2]-iCurrPos[2];
                    iDevPos[3]=Vari[3]-iCurrPos[3];
                    MOT[MOutArmPitch  ].GetRealPos(&iDevPos[0]);
                    MOT[MOutArmPitchX2].GetRealPos(&iDevPos[1]);
                    MOT[MOutArmPitchX3].GetRealPos(&iDevPos[2]);
                    MOT[MOutArmPitchX4].GetRealPos(&iDevPos[3]);
                    long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};
                    MOT[MOutArmPitch].Motor->SetGroup(2, 4, bDevNo);
                    MOT[MOutArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
                #endif
            }
        }
        bFirstIn=false;
        return 0;
    }

    if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable ||                              //JerryYang 20251218 : IN/OUT ARM支援不同模組
       USE_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        if(iFlag[0]==0)
            iFlag[0]=MOT[MOutArmPitchY ].MotorMove(YVari);                      //ChungHung 20131231 alter AutoYPitch
        if(iFlag[1]==0)
            iFlag[1]=MOT[MOutArmPitch  ].MotorMove(Vari[0]);
        if(iFlag[2]==0)
            iFlag[2]=MOT[MOutArmPitchX2].MotorMove(Vari[1]);                    //Steven 20131002 : XY變距
        iFlag[3]=1;
        iFlag[4]=1;
    }
    else if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        if(iFlag[4]==0)
            iFlag[4]=MOT[MOutArmPitchY].MotorMove(YVari);

        if(iFlag[0]==0)
        {
            #ifndef SOFT_SIMULTE
            bFlag=MOT[MOutArmPitch].Motor->MotionDone();
            #else
            bFlag=true;
            #endif
            if(bFlag)
                iFlag[0]=1;
        }
        iFlag[1]=1;
        iFlag[2]=1;
        iFlag[3]=1;
    }
    else
    {
        if(iFlag[0]==0)
            iFlag[0]=MOT[MOutArmPitch].MotorMove(Vari[0]);
        iFlag[1]=1;
        iFlag[2]=1;
        iFlag[3]=1;
        iFlag[4]=1;
    }

    for(int i=0; i<PITCH_COUNT; i++)
    {
        if(iFlag[i]<0)                                                          //判斷是不是有Alarm
            iResult=iFlag[i];
    }

    if(iResult==0)                                                              //沒Alarm就確認是不是走完了
    {
        for(int i=0; i<PITCH_COUNT; i++)
        {
            if(iFlag[i]==0)
                return 0;
        }
        #ifndef SOFT_SIMULTE
        if(abs(Vari[0]-iCurrPos[0])>5)                                          //Ztex 20240104 : Fixed for pitch move
        {
            bFirstIn=true;
            return 0;
        }
        #endif
        bFirstIn=true;
        ZeroMemory(iFlag, sizeof(iFlag));
        iResult=1;
    }
    bFirstIn=true;
    ZeroMemory(iFlag, sizeof(iFlag));
    return iResult;
}
//------------------------------------------------------------------------------
TQPF_Timer OutArmIdle;
extern bool MoveOutArmToAutoSafe();                                             //只上升吸嘴到安全高度
/*bool OutArmContinuousMove(int X, int Y, int Vari, bool *ZDownSel, int *ZPos, bool ZNeedDown, int YVari, int Vari2)  //Steven 20131002 : XY變距
{
    AnsiString str=AnsiString("");
    AnsiString str1=AnsiString(""), str2=AnsiString("");
    int i=0, iCheckX=0, iCheckY=0, iEncodeX=0, iEncodeY=0;
    int iXPosdiffer=0, iYPosdiffer=0, iXpos=0, iXMSPos=0, iYpos=0, iYMSPos=0;
    int ret=PNP_DOING;
    int iX=X, iY=Y;
    static bool bFlag[5]={false,false,false,false,false};
    static bool bFlagZ[8]={false,false,false,false,false,false,false,false};
    static bool OutSafeCheck[8]={false,false,false,false,false,false,false,false};
    static int  iOutArmAllZSafe=1;
    static bool bXYMoveFinish=false;
    static bool bXYPosErrorFlag=false;

    static int iLastPosX=-99999;
    static int iLastPosY=-99999;
    if(iLastPosX!=X || iLastPosY!=Y)
    {
        iLastPosX=X;
        iLastPosY=Y;
        iOutArmAllZSafe=1;
        if(MOT[MOutArmX].Motor->MotionDone()==false)
            MOT[MOutArmX].PCIL132_StopMotor();
        if(MOT[MOutArmY].Motor->MotionDone()==false)
            MOT[MOutArmY].PCIL132_StopMotor();
    }

    //KaiChen 20171228 ：Log Light Scale Data
    //==>
    int iXO=0, iYO=0, iLogCount=500;
    iXO=iX;
    iYO=iY;
    AnsiString S1="";

    if(IniConfig.bA27EnableLightScale==true)
    {
        iCheckX=MOT[MOutArmX].ReadPos();
        iCheckY=MOT[MOutArmY].ReadPos();

        if(iX>iCheckX)
        {
            iX=ChangePosition(iX, 4);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmX1<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo5->Lines->Add(S1);
                iLogLightScaleCount_OutArmX1++;
            }
        }
        else
        {
            iX=ChangePosition(iX, 5);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmX2<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo6->Lines->Add(S1);
                iLogLightScaleCount_OutArmX2++;
            }
        }

        if(iY>iCheckY)
        {
            iY=ChangePosition(iY, 6);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmY1<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo7->Lines->Add(S1);
                iLogLightScaleCount_OutArmY1++;
            }
        }
        else
        {
            iY=ChangePosition(iY, 7);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmY2<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo8->Lines->Add(S1);
                iLogLightScaleCount_OutArmY2++;
            }
        }

        bLogLightScale_OutArm=true;
    }
    //<==
    //KaiChen 20171228 ：Log Light Scale Data

    //jou 980313 safe protect
    if(iX>=MOT[MOutArmX].Motor->PSoftLimitP || iX<=MOT[MOutArmX].Motor->PSoftLimitN)
    {
        str.sprintf("X=%d", iX);
        ShowErrorMessage("WAR0254", 0, MOutArmX, 0, str);                       //"Out Arm X axis motor will out of limit !"
        return false;
    }
    if(iY>=MOT[MOutArmY].Motor->PSoftLimitP || iY<=MOT[MOutArmY].Motor->PSoftLimitN)
    {
        str.sprintf("Y=%d", iY);
        ShowErrorMessage("WAR0255", 0, MOutArmY, 0, str);                       //"Out Arm Y axis motor will out of limit !"
        return false;
    }

    if(MOT[MOutArmX].fCanMove==false || MOT[MOutArmY].fCanMove==false)
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        iOutArmAllZSafe=1;
        return false;
    }

    if(MOT[MOutArmX].Motor->Enable && MOT[MOutArmY].Motor->Enable)
    {
        if(MOT[MOutArmX].Motor->Direction==false)   iX=-iX;
        if(MOT[MOutArmY].Motor->Direction==false)   iY=-iY;

        if(bPauseOutMotor)
        {
            iOutArmAllZSafe=1;
            bPauseOutMotor=false;
            OutArmIdle.SetSecAndOn(iIDLETime);
        }

        switch(iOutArmAllZSafe)
        {
            case 1:
                if(MoveOutArmToAutoSafe())
                {
                    OutSafeCheck[0]=false;
                    OutSafeCheck[1]=false;                                      //ChungHung 20131231 alter AutoYPitch
                    OutSafeCheck[2]=false;                                      //Steven 20131002 : XY變距
                    OutArmIdle.SetSecAndOn(iIDLETime);
                    iOutArmAllZSafe=100;
                    bXYMoveFinish=false;
                    for(i=0; i<8; i++)
                        bFlagZ[i]=false;

                    InitPCIL112_OutArmXYMoveTask();                             //jou 2011-04-03
                    if(bXYPosErrorFlag)
                    {
                        bFlag[0]=false;
                        bFlag[1]=false;
                        iOutArmAllZSafe=150;
                        return false;
                    }
                }
                else
                {
                    OutSafeCheck[0]=false;
                    OutSafeCheck[1]=false;                                      //ChungHung 20131231 alter AutoYPitch
                    OutSafeCheck[2]=false;                                      //Steven 20131002 : XY變距
                    return false;
                }
//                break;
            case 100:
                if(OutSafeCheck[0]==false)
                    OutSafeCheck[0]=MOT[MOutArmPitch].MotorMove(Vari);

                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                    //ChungHung 20131231 alter AutoYPitch
                {
                    if(OutSafeCheck[1]==false)
                        OutSafeCheck[1]=MOT[MOutArmPitchY].MotorMove(YVari);
                    if(OutSafeCheck[2]==false)
                        OutSafeCheck[2]=MOT[MOutArmPitchX2].MotorMove(Vari2);   //Steven 20131002 : XY變距
                }
                else
                {
                    OutSafeCheck[1]=true;
                    OutSafeCheck[2]=true;
                }

                if(OutSafeCheck[0] && OutSafeCheck[1] && OutSafeCheck[2])
                {
                    iOutArmAllZSafe=200;
                }
                break;
            case 150:
                if(bFlag[0]==false)
                    bFlag[0]=MOT[MOutArmX].MotorMove(X+100);
                if(bFlag[1]==false)
                    bFlag[1]=MOT[MOutArmY].MotorMove(Y+100);
                if(bFlag[0] && bFlag[1])
                {
                    bFlag[0]=false;
                    bFlag[1]=false;
                    bXYPosErrorFlag=false;
                    iOutArmAllZSafe=100;
                }
                break;
            case 200:
                if(bXYMoveFinish)
                {
                    if(ZNeedDown)                                               //jou 981223 start : //暫時分兩段式動作
                    {
                        MOT[MOutArmX].ScanMotorStatus();
                        MOT[MOutArmY].ScanMotorStatus();

                        if(MOT[MOutArmX].Led[iInposLed]==true ||
                           MOT[MOutArmY].Led[iInposLed]==true)                  //Isaac 20170721 (wei) Z axis protection
                            return false;

                        for(i=0; i<8; i++)
                        {
                            if(ZDownSel[i])
                            {
                                if(bFlagZ[i]==false)
                                    bFlagZ[i]=MOT[MOutArmZA+i].MotorMove2SpeedForPicker(ZPos[i], &ArmSpeed[OutArm]);  //Steven 20140217 : 兩段速移動
                            }
                            else
                            {
                                bFlagZ[i]=true;
                            }
                        }
                        for(i=0; i<8; i++)
                            if(bFlagZ[i]==false)
                                return false;
                    }
                    //jou 981223 end

                    for(i=0; i<8; i++)
                    {
                        MOT[MOutArmZA+i].fCMD=false;

                        if(ZNeedDown && ZDownSel[i])
                        {
                            MOT[MOutArmZA+i].ScanMotorStatus();
                            if(MOT[MOutArmZA+i].Motor->Enable && MOT[MOutArmZA+i].Led[iHomeLed])
                            {
                                  MOT[MOutArmX].PCIL132_StopMotor();
                                  MOT[MOutArmY].PCIL132_StopMotor();
                                  iOutArmAllZSafe=1;
                                  OutSafeCheck[0]=false;
                                  OutSafeCheck[1]=false;
                                  OutSafeCheck[2]=false;
                                  ShowMyMessage(MOT[MOutArmZA+i].NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!",
                                                MOT[MOutArmZA+i].NumberAlias+"歸零感應器錯誤；如果吸嘴在下方，可能是感應失敗", __FUNC__);
                                  return false;
                            }
                        }
                    }

                    if(IniConfig.bA22MagneticScale==true)                       //Frank 20161109 add 磁性尺
                    {
                        iXpos=MOT[MOutArmX].Motor->ReadPos();
                        iXMSPos=MOT[MOutArmXScale].Motor->ReadPos();
                        iYpos=MOT[MOutArmY].Motor->ReadPos();
                        iYMSPos=MOT[MOutArmYScale].Motor->ReadPos();

                        iXPosdiffer=abs(iXpos-iXMSPos);
                        iYPosdiffer=abs(iYpos-iYMSPos);

                        if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100 &&
                           iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();

                            str1.sprintf("Out Arm X & Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm X & Y 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange*100 ||
                               iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();
                            str1.sprintf("Out Arm X Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm X 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();
                            str1.sprintf("Out Arm Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm Y 磁性尺與編碼器差超出設定");
                            if(iYPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                    }

                    iOutArmAllZSafe=1;
                    OutSafeCheck[0]=false;
                    OutSafeCheck[1]=false;
                    OutSafeCheck[2]=false;
                    InitPCIL112_OutArmXYMoveTask();                             //jou 2011-04-03
                    bLogLightScale_OutArm=false;                                //KaiChen 20171228 ：Log Light Scale Data
                    return true;
                }

                if(OutSafeCheck[0]==false ||
                   OutSafeCheck[1]==false ||
                   OutSafeCheck[2]==false)                                      //Steven 20150718 : Fixed for Y-Pitch
                {
                    iOutArmAllZSafe=1;
                }
                break;
            default :
                iOutArmAllZSafe=1;
                return false;
        }

        if(bXYPosErrorFlag)
            return false;

        ret=PCIL112_OutArmXYMove(X, Y);                                         //jou 2011-04-03
        if(ret==PNP_DONE)
        {
            if(OutSafeCheck[0] && OutSafeCheck[1] && OutSafeCheck[2])           //Steven 20150718 : Fixed for Y-Pitch
            {
                if(USE_MAGNETIC_SCALE==false)                                   //Ifor 20180622 (Steven) : 避免位置錯誤
                {
                    iCheckX=MOT[MOutArmX].ReadPos();
                    iCheckY=MOT[MOutArmY].ReadPos();

                    if((X!=iCheckX) || (Y!=iCheckY))
                    {
                        iEncodeX=MOT[MOutArmX].ReadEncoderPos();                //Steven 20110314 : 比對Encoder位置
                        iEncodeY=MOT[MOutArmY].ReadEncoderPos();                //Steven 20110314 : 比對Encoder位置

                        str1.sprintf(" Move Finish:%d ",bXYMoveFinish);         //Ifor 20181127 : add  bXYMoveFinish Log
                        MNetLog(" iX="+AnsiString(iX)+" iCheckX="+AnsiString(iCheckX)+" iEncodeX="+AnsiString(iEncodeX)+
                                " iY="+AnsiString(iY)+" iCheckY="+AnsiString(iCheckY)+" iEncodeY="+AnsiString(iEncodeY)+ " Out Arm Pos Error"+ str1);   //Steven 20110402   //Ifor 20181127 : add  bXYMoveFinish Log
                        if(X>(iEncodeX+iTorence) || X<(iEncodeX-iTorence) ||
                           Y>(iEncodeY+iTorence) || Y<(iEncodeY-iTorence))      //Steven 20110314 : 比對Encoder位置
                        {
                            iOutArmAllZSafe=1;
                            OutSafeCheck[0]=false;
                            OutSafeCheck[1]=false;
                            OutSafeCheck[2]=false;
                            MOT[MOutArmX].PCIL132_StopMotor();
                            MOT[MOutArmY].PCIL132_StopMotor();
                            if(MOTION_CARD_TYPE==MotionCard_SYN)
                                bXYPosErrorFlag=true;
                            bXYMoveFinish=false;                                //Ifor 20181127 : add Encoder位置比對異常時，設定bXYMoveFinish為False避免誤動作
                            return false;
                        }
                    }
                }

                bXYMoveFinish=true;

                if(ZNeedDown)
                {
                    MOT[MOutArmX].ScanMotorStatus();
                    MOT[MOutArmY].ScanMotorStatus();

                    if(MOT[MOutArmX].Led[iInposLed]==true ||
                       MOT[MOutArmY].Led[iInposLed]==true)                      //Isaac 20170721 (wei) Z axis protection
                        return false;

                    for(i=0; i<8; i++)
                    {
                        if(ZDownSel[i])
                        {
                            bFlagZ[i]=MOT[MOutArmZA+i].MotorMove2SpeedForPicker(ZPos[i], &ArmSpeed[OutArm]);  //Steven 20140217 : 兩段速移動
                        }
                    }
                }
                return false;
            }
        }

        if(OutArmIdle.Off())
        {
            MOT[MOutArmX].PCIL132_StopMotor();
            MOT[MOutArmY].PCIL132_StopMotor();
            iOutArmAllZSafe=1;
            OutSafeCheck[0]=false;
            OutSafeCheck[1]=false;
            OutSafeCheck[2]=false;
            SetOutArmHome();
            OutArmIdle.SetSecAndOn(iIDLETime);
        }
    }
    else
    {
        //動作模擬
        for(i=0; i<8; i++)
            if(OutSafeCheck[i]==false)
                OutSafeCheck[i]=MOT[MOutArmZA+i].MotorMove(ZSafePos);

        for(i=0; i<8; i++)
            if(OutSafeCheck[i]==false)
                return false;

        if(bFlag[0]==false)
            bFlag[0]=MOT[MOutArmX].MotorMove(X);
        if(bFlag[1]==false)
            bFlag[1]=MOT[MOutArmY].MotorMove(Y);
        if(bFlag[2]==false)
            bFlag[2]=MOT[MOutArmPitch].MotorMove(Vari);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                            //ChungHung 20131231 alter AutoYPitch
        {
            if(bFlag[3]==false)
                bFlag[3]=MOT[MOutArmPitchY].MotorMove(YVari);
            if(bFlag[4]==false)
                bFlag[4]=MOT[MOutArmPitchX2].MotorMove(Vari2);                  //Steven 20131002 : XY變距
        }
        else
        {
            bFlag[3]=true;
            bFlag[4]=true;
        }

        if(bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3] && bFlag[4])
        {
            for(i=0; i<8; i++)
            {
                if(ZNeedDown)
                {
                    if(ZDownSel[i]==false)
                        ZPos[i]=ZSafePos;
                }
                else
                {
                    ZPos[i]=ZSafePos;
                }
                if(bFlagZ[i]==false)
                    bFlagZ[i]=MOT[MOutArmZA+i].MotorMove(ZPos[i]);
            }

            for(i=0; i<8; i++)
                if(bFlagZ[i]==false)
                    return false;

            bFlag[0]=false;
            bFlag[1]=false;
            bFlag[2]=false;
            bFlag[3]=false;                                                     //ChungHung 20131231 alter AutoYPitch
            bFlag[4]=false;                                                     //ChungHung 20131231 alter AutoYPitch

            for(i=0; i<8; i++)
            {
                bFlagZ[i]=false;
                OutSafeCheck[i]=false;
            }
            bLogLightScale_OutArm=false;                                        //KaiChen 20171228 ：Log Light Scale Data
            return true;
        }
    }
    return false;
}   */
//------------------------------------------------------------------------------
int  iOutArmZMoveTask=-1;
bool bOutArmZMove[MAX_ARM_Row][MAX_ARM_Col];
bool OutArmCynMove(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col])                     //Steven for HT1032
{
    if(InOutArmPickerUseMotor!=eptUseMotCyn)
        return true;

    int iCyn;
    bool bFlag=true;
    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            iCyn=OutArmSuck.Suck[i][j].iMotNo;
            if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)                  //有用到的才做判斷
            {
                if(ZDownSel[i][j])
                    Cylinder[iCyn].On();
                else
                    Cylinder[iCyn].Off();

            }
            else                                                                //沒用到的一律往上打
            {
                Cylinder[iCyn].Off();
            }
        }
    }

    return bFlag;
}
//------------------------------------------------------------------------------
bool OutArmZMoveDown(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bLoader, bool bPreOn)//Steven for HT1032
{
    int iMot, iCyn;
    bool bFlag=true;
    if(iOutArmZMoveTask==-1 || iOutArmZMoveTask==2)
    {
        ZeroMemory(bOutArmZMove, sizeof(bOutArmZMove));
        iOutArmZMoveTask=1;
    }
    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                iCyn=OutArmSuck.Suck[i][j].iMotNo;
                if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)              //有用到的才做判斷
                {
                    if(ZDownSel[i][j])
                    {
                        if(bPreOn)
                            OutArmSuck.Suck[i][j].On();

                        Cylinder[iCyn].On();
                    }
                    else
                    {
                        Cylinder[iCyn].Off();
                    }

                }
                else                                                            //沒用到的一律往上打
                {
                    Cylinder[iCyn].Off();
                }
            }
        }

        bFlag=MOT[MOutArmZA].MotorMove2SpeedForPicker(iZPos[0][0], &ArmSpeed[OutArm], bLoader);
    }
    else
    {
        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                iMot=OutArmSuck.Suck[i][j].iMotNo;
                if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)              //有用到的才做判斷
                {
                    if(ZDownSel[i][j])
                    {
                        if(bPreOn)
                            OutArmSuck.Suck[i][j].On();
                        if(bOutArmZMove[i][j]==false)
                            bOutArmZMove[i][j]=MOT[iMot].MotorMove2SpeedForPicker(iZPos[i][j], &ArmSpeed[OutArm], bLoader);      //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader
                    }
                    else
                    {
                        bOutArmZMove[i][j]=true;
                    }
                }
                else
                {
                    bOutArmZMove[i][j]=true;
                }

                if(bOutArmZMove[i][j]==false)
                    bFlag=false;
            }
        }
    }

    if(bFlag)
    {
        iOutArmZMoveTask=-1;
    }
    return bFlag;
}
//------------------------------------------------------------------------------
bool OutArmZMoveUp(int iZPos, bool bLoader)                                     //Steven for HT1032
{
    int iMot, iCyn;
    bool bFlag=true;
    if(iOutArmZMoveTask==-1 || iOutArmZMoveTask==1)
    {
        ZeroMemory(bOutArmZMove, sizeof(bOutArmZMove));
        iOutArmZMoveTask=2;
    }

    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                iCyn=OutArmSuck.Suck[i][j].iMotNo;
                Cylinder[iCyn].Off();
            }
        }

        bFlag=MOT[MOutArmZA].MotorMove2SpeedForPicker(iZPos, &ArmSpeed[OutArm], bLoader);
    }
    else
    {
        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                iMot=OutArmSuck.Suck[i][j].iMotNo;
                if(bOutArmZMove[i][j]==false)
                    bOutArmZMove[i][j]=MOT[iMot].MotorMove2SpeedForPicker(iZPos, &ArmSpeed[OutArm], bLoader);      //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader

                if(bOutArmZMove[i][j]==false)
                    bFlag=false;
            }
        }
    }

    if(bFlag)
    {
        iOutArmZMoveTask=-1;
    }
    return bFlag;
}
//------------------------------------------------------------------------------
HTimer OutArmCylinderDelayTimer;
//------------------------------------------------------------------------------
bool OutArmContinuousMove_9045(int X, int Y, int Vari[X_PITCH_COUNT], int YVari, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool ZNeedDown, bool bLoader)  //Steven for HT1032
{
    AnsiString str=AnsiString("");
    AnsiString str1=AnsiString(""), str2=AnsiString("");
    int iCheckX=0, iCheckY=0, iEncodeX=0, iEncodeY=0;
    int iXPosdiffer=0, iYPosdiffer=0, iXpos=0, iXMSPos=0, iYpos=0, iYMSPos=0;
    int ret=PNP_DOING;
    int iX=X, iY=Y;
    bool bRet;
    static bool bFirstIn=true;
    static bool bFlag[8];
    static int  iOutArmAllZSafe=1;
    static int  iPitchMoveOk=0;
    static bool bXYMoveFinish=false;
    static bool bXYPosErrorFlag=false;

    static int iLastPosX=-99999;
    static int iLastPosY=-99999;

//    static int iXPosEncoer=0, iYPosEncoder=0;

    if(iLastPosX!=X || iLastPosY!=Y)
    {
        iLastPosX=X;
        iLastPosY=Y;
        iOutArmAllZSafe=1;
        ZeroMemory(bFlag, sizeof(bFlag));
        if(MOT[MOutArmX].Motor->MotionDone()==false)
            MOT[MOutArmX].PCIL132_StopMotor();
        if(MOT[MOutArmY].Motor->MotionDone()==false)
            MOT[MOutArmY].PCIL132_StopMotor();
    }

    if(bFirstIn)
    {
        bFirstIn=false;
        ZeroMemory(bFlag, sizeof(bFlag));
        ZeroMemory(bOutArmZMove, sizeof(bOutArmZMove));
        OutArmPitchMove(Vari, YVari, true);
    }

    //KaiChen 20171228 ：Log Light Scale Data
    //==>
    int iXO=0, iYO=0, iLogCount=500;
    iXO=iX;
    iYO=iY;
    AnsiString S1="";

    if(IniConfig.bA27EnableLightScale==true)
    {
        iCheckX=MOT[MOutArmX].ReadPos();
        iCheckY=MOT[MOutArmY].ReadPos();

        if(iX>iCheckX)
        {
            iX=ChangePosition(iX, 4);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmX1<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo5->Lines->Add(S1);
                iLogLightScaleCount_OutArmX1++;
            }
        }
        else
        {
            iX=ChangePosition(iX, 5);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmX2<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iXO, iX, iXO-iX);
                fMotorTest->mmo6->Lines->Add(S1);
                iLogLightScaleCount_OutArmX2++;
            }
        }

        if(iY>iCheckY)
        {
            iY=ChangePosition(iY, 6);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmY1<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo7->Lines->Add(S1);
                iLogLightScaleCount_OutArmY1++;
            }
        }
        else
        {
            iY=ChangePosition(iY, 7);
            if(IniConfig.bA27_1LogEnableLightScaleData==true &&
               iLogLightScaleCount_OutArmY2<iLogCount &&
               bLogLightScale_OutArm==false)
            {
                S1.sprintf("Old, %d, New, %d, [ %d ]", iYO, iY, iYO-iY);
                fMotorTest->mmo8->Lines->Add(S1);
                iLogLightScaleCount_OutArmY2++;
            }
        }

        bLogLightScale_OutArm=true;
    }
    //<==
    //KaiChen 20171228 ：Log Light Scale Data

    if(iX>=MOT[MOutArmX].Motor->PSoftLimitP ||                                  //jou 980313 safe protect
       iX<=MOT[MOutArmX].Motor->PSoftLimitN)
    {
        str.sprintf("X=%d", iX);
        ShowErrorMessage("WAR0254", 0, MOutArmX, 0, str);                       //"Out Arm X axis motor will out of limit !"
        return false;
    }

    if(iY>=MOT[MOutArmY].Motor->PSoftLimitP || iY<=MOT[MOutArmY].Motor->PSoftLimitN)
    {
        str.sprintf("Y=%d", iY);
        ShowErrorMessage("WAR0255", 0, MOutArmY, 0, str);                       //"Out Arm Y axis motor will out of limit !"
        return false;
    }

    if(MOT[MOutArmX].fCanMove==false || MOT[MOutArmY].fCanMove==false)
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        iOutArmAllZSafe=1;
        return false;
    }

    if(MOT[MOutArmX].Motor->Enable && MOT[MOutArmY].Motor->Enable)
    {
        if(MOT[MOutArmX].Motor->Direction==false)
            iX=-iX;
        if(MOT[MOutArmY].Motor->Direction==false)
            iY=-iY;

        if(bPauseOutMotor)
        {
            iOutArmAllZSafe=1;
            bPauseOutMotor=false;
            OutArmIdle.SetSecAndOn(iIDLETime);
            if(IniConfig.bA62bUseStopMachineArmHome==true)                      //Ztex 2024.10.30 Add Use Stop Machine In/Out Arm Need To Home
            {
                MOT[MOutArmX].PCIL132_StopMotor();
                MOT[MOutArmY].PCIL132_StopMotor();
                iOutArmAllZSafe=1;
                iPitchMoveOk=0;
                SetOutArmHome();
            }
        }

        switch(iOutArmAllZSafe)
        {
            case 1:
                if(MoveOutArmToAutoSafe())
                {
                    iPitchMoveOk=0;
                    OutArmPitchMove(Vari, YVari, true);
                    OutArmCynMove(ZDownSel);
                    OutArmCylinderDelayTimer.SetMSAndOn(ArmSpeed_File[OutArm].dCylinderDelay*1000);
                    OutArmIdle.SetSecAndOn(iIDLETime);
                    if(InOutArmPickerUseMotor!=eptUseMotCyn)
                        iOutArmAllZSafe=100;
                    else
                        iOutArmAllZSafe=50;
                    bXYMoveFinish=false;
                    ZeroMemory(bOutArmZMove, sizeof(bOutArmZMove));

                    InitPCIL112_OutArmXYMoveTask();                             //jou 2011-04-03
                    if(bXYPosErrorFlag)
                    {
                        bFlag[0]=false;
                        bFlag[1]=false;
                        iOutArmAllZSafe=150;
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            case 50:
                if(OutArmCylinderDelayTimer.Off())
                    iOutArmAllZSafe=100;
                else
                    break;
            case 100:
                if(iPitchMoveOk==0)
                    iPitchMoveOk=OutArmPitchMove(Vari, YVari);

                if(iPitchMoveOk>0)
                {
                    iOutArmAllZSafe=200;
                }
                else if(iPitchMoveOk<0)
                {

                }
                break;
            case 150:
                if(bFlag[0]==false)
                    bFlag[0]=MOT[MOutArmX].MotorMove(X+100);
                if(bFlag[1]==false)
                    bFlag[1]=MOT[MOutArmY].MotorMove(Y+100);
                if(bFlag[0] && bFlag[1])
                {
                    bFlag[0]=false;
                    bFlag[1]=false;
                    bXYPosErrorFlag=false;
                    iOutArmAllZSafe=100;
                }
                break;
            case 200:
                if(bXYMoveFinish)
                {
                    if(ZNeedDown)
                    {
                        MOT[MOutArmX].ScanMotorStatus();
                        MOT[MOutArmY].ScanMotorStatus();

                        if(MOT[MOutArmX].Led[iInposLed]==true ||
                           MOT[MOutArmY].Led[iInposLed]==true)                  //Isaac 20170721 (wei) Z axis protection
                            return false;

                        bRet=OutArmZMoveDown(ZDownSel, iZPos, bLoader);
                        OutArmCylinderDelayTimer.SetMSAndOn(ArmSpeed_File[OutArm].dCylinderDelay*1000);
                        if(bRet==false)
                            return false;
                    }

                    iOutArmAllZSafe=1;
                    InitPCIL112_OutArmXYMoveTask();                             //jou 2011-04-03

                    if(CheckOutArmZHomeSensor_2x8(ZNeedDown, ZDownSel)==false)
                    {
                        return false;
                    }

                    if(IniConfig.bA22MagneticScale==true)                       //Frank 20161109 add 磁性尺
                    {
                        iXpos=MOT[MOutArmX].Motor->ReadPos();
                        iXMSPos=MOT[MOutArmXScale].Motor->ReadPos();
                        iYpos=MOT[MOutArmY].Motor->ReadPos();
                        iYMSPos=MOT[MOutArmYScale].Motor->ReadPos();

                        iXPosdiffer=abs(iXpos - iXMSPos);
                        iYPosdiffer=abs(iYpos - iYMSPos);

                        if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100 &&
                           iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();

                            str1.sprintf("Out Arm X & Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm X & Y 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange*100 ||
                               iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iXPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();
                            str1.sprintf("Out Arm X Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm X 磁性尺與編碼器差超出設定");

                            if(iXPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                        else if(iYPosdiffer>IniConfig.dA22MagneticScaleKeepRunRange*100)
                        {
                            if(MyMessageBox->Visible==true)
                                MyMessageBox->Close();
                            str1.sprintf("Out Arm Y Magnetic Scale Discrepancy Encoder Over Setting");
                            str2.sprintf("Out Arm Y 磁性尺與編碼器差超出設定");
                            if(iYPosdiffer>IniConfig.dA22MagneticScaleStopRunRange)
                            {
                                iUnLoaderCount=0;
                            }
                            else
                            {
                                iUnLoaderCount=9;
                            }
                            ShowUnloaderTrayMessage(str1, str2);
                        }
                    }

                    if(OutArmCylinderDelayTimer.Off())
                    {
                        iOutArmAllZSafe=1;
                        iPitchMoveOk=0;
                        bLogLightScale_OutArm=false;                            //KaiChen 20171228 ：Log Light Scale Data
                        return true;
                    }
                    else
                    {
                        iOutArmAllZSafe=210;
                        break;
                    }
                }

                if(iPitchMoveOk==0)                                             //Steven 20150718 : Fixed for Y-Pitch
                {
                    iOutArmAllZSafe=1;
                }
                break;
            case 210:
                if(OutArmCylinderDelayTimer.Off())
                {
                    iOutArmAllZSafe=1;
                    iPitchMoveOk=0;
                    bLogLightScale_InArm=false;                                 //KaiChen 20171228 ：Log Light Scale Data
                    return true;
                }
                break;
            default :
                iOutArmAllZSafe=1;
                return false;
        }

        if(bXYPosErrorFlag)
            return false;

        if(bXYMoveFinish==false &&                                              //Steven 20231214 : 避免重複進去
           iPitchMoveOk>=0)                                                     //避免Pitch Alarm時, XY還在動
            ret=PCIL112_OutArmXYMove(X, Y);                                     //jou 2011-04-03
        if(ret==PNP_DONE)
        {
            if(iPitchMoveOk>0)                                                  //Steven 20150718 : Fixed for Y-Pitch
            {
                if(USE_MAGNETIC_SCALE==false)                                   //Ifor 20180622 (Steven) : 避免位置錯誤
                {
                    iCheckX=MOT[MOutArmX].ReadPos();
                    iCheckY=MOT[MOutArmY].ReadPos();

                    if((X!=iCheckX) || (Y!=iCheckY))
                    {
                        iEncodeX=MOT[MOutArmX].ReadEncoderPos();                //Steven 20110314 : 比對Encoder位置
                        iEncodeY=MOT[MOutArmY].ReadEncoderPos();                //Steven 20110314 : 比對Encoder位置

                        str1.sprintf(" Move Finish:%d ", bXYMoveFinish);        //Ifor 20181127 : add  bXYMoveFinish Log
                        MNetLog(" iX="+AnsiString(iX)+" iCheckX="+AnsiString(iCheckX)+" iEncodeX="+AnsiString(iEncodeX)+
                                " iY="+AnsiString(iY)+" iCheckY="+AnsiString(iCheckY)+" iEncodeY="+AnsiString(iEncodeY)+ " Out Arm Pos Error"+ str1);   //Steven 20110402   //Ifor 20181127 : add  bXYMoveFinish Log
                        if(X>(iEncodeX+iTorence) || X<(iEncodeX-iTorence) ||
                           Y>(iEncodeY+iTorence) || Y<(iEncodeY-iTorence))      //Steven 20110314 : 比對Encoder位置
                        {
                            iOutArmAllZSafe=1;
                            iPitchMoveOk=0;
                            MOT[MOutArmX].PCIL132_StopMotor();
                            MOT[MOutArmY].PCIL132_StopMotor();
                            bXYPosErrorFlag=true;
                            bXYMoveFinish=false;                                //Ifor 20181127 : add Encoder位置比對異常時，設定bXYMoveFinish為False避免誤動作
                            return false;
                        }
                    }
                }

                bXYMoveFinish=true;

                if(ZNeedDown)
                {
                    MOT[MOutArmX].ScanMotorStatus();
                    MOT[MOutArmY].ScanMotorStatus();

                    if(MOT[MOutArmX].Led[iInposLed]==true ||
                       MOT[MOutArmY].Led[iInposLed]==true)                      //Isaac 20170721 (wei) Z axis protection
                        return false;

                    bRet=OutArmZMoveDown(ZDownSel, iZPos, bLoader);
                }
                return false;
            }
        }

        if(OutArmIdle.Off())
        {
            MOT[MOutArmX].PCIL132_StopMotor();
            MOT[MOutArmY].PCIL132_StopMotor();
            iOutArmAllZSafe=1;
            iPitchMoveOk=0;
            SetOutArmHome();
            OutArmIdle.SetSecAndOn(iIDLETime);
        }
    }
    else
    {
        //動作模擬
        if(bFlag[0]==false)
            bFlag[0]=OutArmZMoveUp(ZSafePos, bLoader);

        if(bFlag[0])
        {
            if(bFlag[1]==false)
            {
                OutArmPitchMove(Vari, YVari, true);
                InitPCIL112_OutArmXYMoveTask();
                bFlag[1]=true;
            }

            if(bFlag[2]==false)
            {
                ret=PCIL112_OutArmXYMove(X, Y);                                 //Steven 20240603 : [E77]改放到PCIL112_OutArmXYMove裡面
                bFlag[2]=(ret==PNP_DONE);
            }

            if(bFlag[3]==false)
                bFlag[3]=OutArmPitchMove(Vari, YVari);
        }

        if(bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3])
        {
            if(InOutArmPickerUseMotor==eptUseMot)                               //Steven for HT1032
                bFlag[4]=true;
            else
                bFlag[4]=OutArmCynMove(ZDownSel);
        }

        if(bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3] && bFlag[4])
        {
            if(ZNeedDown)
                bRet=OutArmZMoveDown(ZDownSel, iZPos, bLoader);
            else
                bRet=OutArmZMoveUp(ZSafePos, bLoader);

            if(bRet==false)
                return false;

            ZeroMemory(bFlag, sizeof(bFlag));
            ZeroMemory(bOutArmZMove, sizeof(bOutArmZMove));
            bLogLightScale_OutArm=false;                                        //KaiChen 20171228 ：Log Light Scale Data
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool CatchMgzTrayMove(int FinalPos)                                             //Sam 20221116 : Magazine TrayArm 自動補 Tray
{
    if(Sen[SnMagazineTrackSelectDetect].IsOff()==true)
    {
        ShowMyMessage("Magazine Track Position Error!!");
        return false;
    }

    if(Cylinder[C_Auto3EdgePush].OffSensor()==false)
    {
        ShowMyMessage("C_Auto3EdgePush Cylinder Error!!");
        return false;
    }

    int iCurrPos;
    bool Flag=false;

    iCurrPos=MOT[MCatchMgzTray].ReadPos();
    if(FinalPos==iCurrPos)  //到位了
    {
        SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCYSp);
        SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCYACDCSp);
        Flag=true;
    }
    else
    {
        if(FinalPos>0)
        {
            if(iCurrPos<=FinalPos-MGSpeed.dCYDecRange*100)                      //第一段
            {
                SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCYSp);
                SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCYACDCSp);
                Flag=MOT[MCatchMgzTray].MotorMove(FinalPos-MGSpeed.dCYDecRange*100);
            }

            if(Flag==true || iCurrPos>=FinalPos-MGSpeed.dCYDecRange*100)        //第二段
            {
                SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCY2Sp);
                SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCY2ACDCSp);
                Flag=MOT[MCatchMgzTray].MotorMove(FinalPos);
            }
        }
        else
        {
            if(iCurrPos>=FinalPos+MGSpeed.dCYDecRange*100)                      //第一段
            {
                SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCYSp);
                SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCYACDCSp);
                Flag=MOT[MCatchMgzTray].MotorMove(FinalPos+MGSpeed.dCYDecRange*100);
            }

            if(Flag==true || iCurrPos<=FinalPos+MGSpeed.dCYDecRange*100)        //第二段
            {
                SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCY2Sp);
                SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCY2ACDCSp);
                Flag=MOT[MCatchMgzTray].MotorMove(FinalPos);
            }
        }
    }
    return Flag;
}
//------------------------------------------------------------------------------
void ServoOnAllMOT()
{
    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(i>=MTestY1 && i<=MTestY2)
            continue;
        if(MOT[i].Motor->Enable==false)
            continue;

        MOT[i].Motor->SetServoOn(true);                                         //servo on
        MySleep(2);
    }
}
//------------------------------------------------------------------------------
//void StopOutArmZMotor_9045()
//{
//    for(int i=0; i<8; i++)
//        MOT[MOutArmZA+i].PCIL132_StopMotor();
//}
//------------------------------------------------------------------------------
bool TTrayMotor::SearchHasEmpryToPlace(int iY)                                  //jou 2010-03-05 start : for TestIF.bSearchLastMode use
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=iY; j<Tray.YItem; j++)
            if(Tray.Data[i][j]==NULL_IC)
                return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void  InitPCIL112_InArmXYMoveTask()
{
    iPCIL112_InArmXYMoveTask=1;
    MOT[MInArmX].fCMD=false;
    MOT[MInArmY].fCMD=false;
    MOT[MInArmPitch].fCMD=false;
}
//------------------------------------------------------------------------------
int PCIL112_InArmXYMove(int iXComPos, int iYComPos)                             //jou 2011-04-03
{
    static int iCount=0;                                                        //Ifor 20220715 add:同一次移動中發生三次異常報警
    static int iXPos=0, iYPos=0;
    static int iInArmFlag[2]={0, 0};

    AnsiString StrE, StrC;
    int &iTask=iPCIL112_InArmXYMoveTask;

    if(MOT[MInArmX].CompareEncoderPos(iXComPos, 2)==1 &&                        //Steven 20231214 : CompareCommandPos --> CompareEncoderPos
       MOT[MInArmY].CompareEncoderPos(iYComPos, 2)==1)
    {
        return PNP_DONE;
    }

    int iPos=InArmZSafe(DETECT_SENSOR_FLAG);
    if(iPos!=-1)                                                                //jou 20220113 : 增加in & out arm移動保護
    {
        StrE.sprintf("In arm XY move, %s axis home sensor off alarm", MOT[iPos].NumberAlias);
        StrC.sprintf("In arm XY移動, %s軸home sensor異常警報", MOT[iPos].NumberAlias);
        if(USE_ARM_PROTECTION==true && CUSTOMER_CODE!=CC_ASE_KaohSiung)         //kevin 20220901 ASE_KH close          //Steven 20220314 : In Our Arm Z Sensor保護加上開關
        {
            ShowMyMessage(StrE, StrC);
            return PNP_DOING;
        }
        else
        {
            if(iXPos!=iXComPos || iYPos!=iYComPos)
            {
                iXPos=iXComPos;
                iYPos=iYComPos;

                MyDBIProcess("Motion", StrE, "");
                iCount++;
                if(iCount>=3)
                {
                    iCount=0;
                    ShowMyMessage(StrE, StrC);
                }
                return PNP_DOING;                                               //Steven 20240424 : move down
            }
        }
    }

    switch(iTask)
    {
        case 1:
            iInArmFlag[0]=0;
            iInArmFlag[1]=0;
            iCount=0;
            iTask=100;
        case 100:
            if(iInArmFlag[0]==0)
                iInArmFlag[0]=MOT[MInArmX].MotorMove(iXComPos);
            if(iInArmFlag[1]==0)
                iInArmFlag[1]=MOT[MInArmY].MotorMove(iYComPos);
            if(iInArmFlag[0]>0 && iInArmFlag[1]>0)
            {
                iCount=0;
                return PNP_DONE;
            }
            break;
    }
    return PNP_DOING;
}
//------------------------------------------------------------------------------
void InitPCIL112_OutArmXYMoveTask()
{
    iPCIL112_OutArmXYMoveTask=1;
    MOT[MOutArmX].fCMD=false;
    MOT[MOutArmY].fCMD=false;
    MOT[MOutArmPitch].fCMD=false;
}
//------------------------------------------------------------------------------
int PCIL112_OutArmXYMove(int iXComPos, int iYComPos)
{
    static int iCount=0;                                                        //Ifor 20220715 add:同一次移動中發生三次異常報警
    static int iYEncoder=0;
    static int iXPos=0, iYPos=0;
    static int iOutArmFlag[2]={0, 0};

    AnsiString StrE, StrC;
    int &iTask=iPCIL112_OutArmXYMoveTask;

    if(MOT[MOutArmX].CompareEncoderPos(iXComPos, 2)==1 &&                       //Steven 20231214 : CompareCommandPos --> CompareEncoderPos
       MOT[MOutArmY].CompareEncoderPos(iYComPos, 2)==1)
    {
        return PNP_DONE;
    }

    int iPos=OutArmZSafe(DETECT_SENSOR_FLAG);
    if(iPos!=-1)
    {
        StrE.sprintf("Out arm XY move, %s axis home sensor off alarm", MOT[iPos].NumberAlias);
        StrC.sprintf("Out arm XY移動, %s軸home sensor異常警報", MOT[iPos].NumberAlias);
        if(USE_ARM_PROTECTION==true)                                            //Steven 20220314 : In Our Arm Z Sensor保護加上開關
        {
            ShowMyMessage(StrE, StrC);
            return PNP_DOING;
        }
        else
        {
            if(iXPos!=iXComPos || iYPos!=iYComPos)
            {
                iXPos=iXComPos;
                iYPos=iYComPos;
                MyDBIProcess("Motion", StrE, "");
                iCount++;
                if(iCount>=3)
                {
                    iCount=0;
                    ShowMyMessage(StrE, StrC);
                }
                return PNP_DOING;                                               //Steven 20240424 : move down
            }
        }
    }

    switch(iTask)
    {
        case 1:
            iOutArmFlag[0]=0;
            iOutArmFlag[1]=0;
            iCount=0;
            if(IniConfig.bE77_OutamrCMotion)                                    //Steven 20240603 : [E77]改放到PCIL112_OutArmXYMove裡面
            {
                iYEncoder=MOT[MOutArmY].ReadEncoderPos();
                if((iYEncoder>(Tech.iOutArmShuttle1Y-3000) && iYComPos<(Tech.iOutArmShuttle1Y-3000)) ||
                   (iYEncoder<(Tech.iOutArmShuttle1Y-3000) && iYComPos>(Tech.iOutArmShuttle1Y-3000))) //Y 在FIX區, 要移動到AUTO區
                {
                    iTask=300;
                    return PNP_DOING;
                }
            }
            iTask=100;
        case 100:
            if(iOutArmFlag[0]==0)
                iOutArmFlag[0]=MOT[MOutArmX].MotorMove(iXComPos);
            if(iOutArmFlag[1]==0)
                iOutArmFlag[1]=MOT[MOutArmY].MotorMove(iYComPos);
            if(iOutArmFlag[0]>0 && iOutArmFlag[1]>0)
            {
                iCount=0;
                return PNP_DONE;
            }
            break;
        case 300:
            if(iOutArmFlag[0]==false)
                iOutArmFlag[0]=MOT[MOutArmX].MotorMove(Tech.iOutArmAuto2X);
            if(iOutArmFlag[0])
            {
                iOutArmFlag[0]=false;
                iTask=400;
            }
            else
            {
                return PNP_DOING;
            }
        case 400:
            if(iOutArmFlag[0]==false)
                iOutArmFlag[0]=MOT[MOutArmY].MotorMove(iYComPos);
            if(iOutArmFlag[0])
            {
                iOutArmFlag[0]=false;
                iOutArmFlag[1]=false;
                iTask=100;
            }
            else
            {
                return PNP_DOING;
            }
    }
    return PNP_DOING;
}
//------------------------------------------------------------------------------
bool TTrayMotor::TrayFeedHasIC()                                                //kevin 20130508 Trayfeed 判斷fix tray 有無放ic 有ic發alrm取出
{
    for(int i=0; i<Tray.XItem; i++)
        for(int j=0; j<Tray.YItem; j++)
            if(Tray.Data[i][j]!=NULL && Tray.Data[i][j]!=HAS_NULL_IC)
                return true;                                                    //有ic 放fix 需取出tray才能tray feed

    return false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall SaveLog(AnsiString FileNamePath)
{
    AnsiString sFileName;                                                       //kevin 20150908
    sFileName.sprintf("%s\\%04d%02d\\", FileNamePath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName, "MyMotor_SaveLog");
    String SDate;
    SDate=Now().FormatString("yyyy-mm-dd");
    String SName="";

    SName=sFileName+"\\"+SDate+".ini";                                          //kevin 20150908
    return SName;
}
//------------------------------------------------------------------------------
//     kevin 20150915
//------------------------------------------------------------------------------
void RecordIndexPositionError(AnsiString sFunction, bool A1, bool A2, bool A3, bool A4, long *Pos)
{
    AnsiString str;
    str.sprintf("Function:%s, Z1:%d, PosZ1:%d, Z2:%d, PosZ2:%d, Y1:%d, PosY1:%d, Y2:%d, PosY2:%d",
                 sFunction, A1, Pos[0], A2, Pos[1], A3, Pos[2], A4, Pos[3]);

    MyDBIProcessNew("Motion", "WAR2200", "Index Position Error", str);
}
//------------------------------------------------------------------------------
bool TrayArmContinuousMoveForOCR(int X, int Y)                                  //wei 20150923
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
    static int Task=1;

    if(MOT[MTrayX].Motor->Enable && MOT[MLoaderY].Motor->Enable)
    {
        switch(Task)
        {
            case 1:
                if(TrayArmMotorMove(X, false))                                  //Steven 20250301 : add false for OCR
                {
                    Task=30;
                }
                break;
            case 30:
                if(MOT[MLoaderY].MotorMove(Y))
                {
                    Task=1;
                    return true;
                }
                break;
        }
    }
    return false;
    #endif
}
//------------------------------------------------------------------------------
bool TrayMoveHome()                                                             //wei 20150923
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
    static int Task=1;
    static int iOCRPosition=0;
    switch(Task)
    {
        case 1:
            MOT[MLoaderY].MotorMove(-99999);
            if(Sen[SnOCRPosition].IsOff())
            {
                MOT[MLoaderY].PCIL132_StopMotor();
                iOCRPosition=MOT[MLoaderY].ReadPos();
                Task=2;
            }
            break;
        case 2:
            if(MOT[MLoaderY].MotorMove(iOCRPosition-3000))
            {
                iOCRPosition=0;
                MOT[MLoaderY].PCIL132_StopMotor();
                Task=1;
                return true;
            }
            break;
    }

    return false;
    #endif
}
//--------------------------------------------------------------------------------
bool ShuttleSensorContinuousMove(int SH1, int SH2, bool Result)                 //wei 20160914 Auto Shuttle Sensor
{
    #ifdef SOFT_SIMULTE
        return true;
    #else

    int MotorData[2]={MShuttle1Pitch, MShuttle2Pitch};
    bool bFlag[2]={false,false};
    static bShuttleHome[2]={false};
    static int Task=1;
    if(MOT[MShuttle1Pitch].Motor->Enable && MOT[MShuttle2Pitch].Motor->Enable)
    {
        switch(Task)
        {
            case 1:
                for(int i=0; i<2; i++)
                {
                    InitProcessSingleMotorTask(MotorData[i]);
                    bShuttleHome[i]=false;
                }
                Task=10;
                break;
            case 10:
                for(int i=0; i<2; i++)
                {
                    if(bShuttleHome[i]==false)
                    {
                        bShuttleHome[i]=ProcessSingleMotorHome(MotorData[i]);
                        bFlag[i]=true;
                    }
                }

                if(bFlag[0]==false && bFlag[1]==false)
                {
                    Task=20;
                }
                break;
            case 20:
                if(MOT[MShuttle1Pitch].MotorMove(-SH1))
                {
                    Task=30;
                }
                break;
            case 30:
                if(MOT[MShuttle2Pitch].MotorMove(SH2))
                {
                    Task=1;
                    return true;
                }
                break;
        }
    }
    return false;
    #endif
}
//--------------------------------------------------------------------------------
//V3.27L.540 : Frank 20170822 (wei): 清空Rotate資料
//--------------------------------------------------------------------------------
void TTrayMotor::SetHasNullIcToNullIc()
{
    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            if(Tray.Data[i][j]==HAS_NULL_IC)
            {
                SetTraySingleData(i, j, NULL_IC);
            }
        }
    }
}
//------------------------------------------------------------------------------
//       //kevin 20180119 Add outarm place ic not home
//------------------------------------------------------------------------------
int CheckOutArmDestory()
{
    for(int i=0; i<OutArmSuck.iPickRow; i++)                                    //Steven 20240822 : 修正放料判斷
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            if(bOutArmPlaceDevice[i][j])
                return i*100+j+1;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool TMyMotor::CheckYPosWhenZDown(int Pos, int iOrgPos, AnsiString asErrorFunc)
{
    int iZPos=0;
    static int iRetryCnt[4]={0, 0, 0, 0};

    if(CosFunction.bIndexProtect==true)                                         //Steven 20180319 (jou) : 加入Index Y軸移動前確認Z軸位置保護
    {
        if(Mot_Name==MTestY1)
        {
            iZPos=MOT[MTestZ1].Gali_ReadEncoderPos();
            if(iZPos<-200)
            {
                if(iRetryCnt[0]<100)
                {
                    iRetryCnt[0]++;
                    return false;
                }
                ShowIndexMotorError(asErrorFunc+AnsiString("_Y1"));
                return false;
            }
            iRetryCnt[0]=0;
        }
        else if(Mot_Name==MTestY2)
        {
            iZPos=MOT[MTestZ2].Gali_ReadEncoderPos();
            if(iZPos<-200)
            {
                if(iRetryCnt[1]<100)
                {
                    iRetryCnt[1]++;
                    return false;
                }
                ShowIndexMotorError(asErrorFunc+AnsiString("_Y2"));
                return false;
            }
            iRetryCnt[1]=0;
        }
        else if(Mot_Name==MTestZ1)                                              //JerryYang 20180411 (jou) Z軸移動前確認Y軸位置保護
        {
            if(Pos>0)                                                           //前面Pos有加負號，所以>0是指Z軸往下要檢查Y軸
            {
                int iEncoderPos1=MOT[MTestY1].Gali_ReadEncoderPos();
                if(CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Front, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
                {
                    iRetryCnt[2]=0;
                    if(iContactMode!=CONTACT_AUTO_GET_HEIGHT &&                 //Steven 20231109 : Auto Height的時候, 不檢查高度
                       CheckArmPosInRange(iOrgPos, Prod.TestZ1_Pick-1000, 300)==false)
                    {
                        ShowIndexMotorError(asErrorFunc+AnsiString("_Z1_TooLowOnShuttle"), true);
                        return false;
                    }
                }
                else if(CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Middle, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
                {
                    iRetryCnt[2]=0;
                }
                else
                {
                    iRetryCnt[2]++;
                    #ifdef INDEX_PROTECT_TMOVE
                    if(iRetryCnt[2]<50)                                         //Ifor 20240719 : Add index Y protection
                    {
                        return false;
                    }
                        if(bOverRangeDoTMode==false)
                            RecordIndexPosition(1, 3);
                        bOverRangeDoTMode=true;                                 //觸發做Tmode
                    #else
                    if(iRetryCnt[2]<100)
                    {
                        return false;
                    }
                    ShowIndexMotorError(asErrorFunc+AnsiString("_Z1"), true);
                        #endif
                    iRetryCnt[2]=0;
                    return false;
                }
            }
        }
        else if(Mot_Name==MTestZ2)
        {
            if(Pos>0)                                                           //前面Pos有加負號，所以>0是指Z軸往下要檢查Y軸
            {
                if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                         //JimmyChiu 20220708 : add Index Arm Axis
                {
                    int iEncoderPos1=MOT[MTestY1].Gali_ReadEncoderPos();
                    if(CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Front,  IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護  //Jimmychiu 20230628 : 修正index arm移動保護
                    {
                        iRetryCnt[2]=0;
                    }
                    else if(CheckArmPosArrival(iEncoderPos1, Prod.TestY1_Middle, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
                    {
                        iRetryCnt[2]=0;
                        if(iContactMode!=CONTACT_AUTO_GET_HEIGHT &&             //Steven 20231109 : Auto Height的時候, 不檢查高度
                           CheckArmPosInRange(iOrgPos, Prod.TestZ2_Pick-1000, 300)==false)
                        {
                            ShowIndexMotorError(asErrorFunc+AnsiString().sprintf("_Z2_TooLowOnShuttle_OrgPos:%d",iOrgPos), true);
                            return false;
                        }
                    }
                    else
                    {
                        if(iRetryCnt[2]<100)
                        {
                            iRetryCnt[2]++;
                            return false;
                        }
                        iRetryCnt[2]=0;
                        ShowIndexMotorError(asErrorFunc+AnsiString("_Z2"), true);
                        return false;
                    }
                }
                else
                {
                    int iEncoderPos1=MOT[MTestY2].Gali_ReadEncoderPos();
                    if(CheckArmPosArrival(iEncoderPos1, Prod.TestY2_Rear, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
                    {
                        iRetryCnt[3]=0;
                        if(iContactMode!=CONTACT_AUTO_GET_HEIGHT &&             //Steven 20231109 : Auto Height的時候, 不檢查高度
                           CheckArmPosInRange(iOrgPos, Prod.TestZ2_Pick-1000, 300)==false)
                        {
                            ShowIndexMotorError(asErrorFunc+AnsiString().sprintf("_Z2_TooLowOnShuttle_OrgPos:%d",iOrgPos));
                            return false;
                        }
                    }
                    else if(CheckArmPosArrival(iEncoderPos1, Prod.TestY2_Middle, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
                    {
                        iRetryCnt[3]=0;                                         //Jimmychiu 20230710 : 修正index arm移動保護
                    }
                    else
                    {
                        #ifdef INDEX_PROTECT_TMOVE
                        if(iRetryCnt[3]>50)
                        {
                            if(bOverRangeDoTMode==false)
                                RecordIndexPosition(2, 3);
                            bOverRangeDoTMode=true;                             //觸發做Tmode
                        }
                        #else
                        if(iRetryCnt[3]<100)
                        {
                            iRetryCnt[3]++;
                            return false;
                        }
                        #endif
                        iRetryCnt[3]=0;
                        ShowIndexMotorError(asErrorFunc+AnsiString("_Z2"), true);
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//----------------------------------------------------------------------------
bool TMyMotor::CheckArmPosInRange(int iNowPos, int iMin, int iMax)
{
    int iTemp;
    if(iMin>iMax)
    {
        iTemp=iMax;
        iMax=iMin;
        iMin=iTemp;
    }

    if(iMin<=iNowPos &&                                                         //JerryYang 20230309 : 修正index arm移動保護
       iNowPos<=iMax)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------------------------------
bool TMyMotor::CheckArmPosArrival(int iNowPos, int iDestination, int iTolerance)
{
    if(CheckArmPosInRange(iNowPos, iDestination-iTolerance, iDestination+iTolerance))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
extern bool bShuttleShake;
bool TMyMotor::MotorMoveShuttleShake(int p)                                     //JerryYang 20190628 shuttle shake專用command
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(bShuttleShake==false)
        return false;
    if(Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2)
    {

    }
    else
    {
        return false;
    }

    if(Mot_Name==MInShuttle1)
    {
        if(p==Prod.InSHT[0].iLeft+SHSpeed_File.iShakeDistance*100 || p==Prod.InSHT[0].iLeft)    //Sam 20250326 : 新增 Shake 條件設定
        {

        }
        else
        {
            return false;
        }
    }
    else if(Mot_Name==MInShuttle2)
    {
        if(p==Prod.InSHT[1].iLeft+SHSpeed_File.iShakeDistance*100 || p==Prod.InSHT[1].iLeft)    //Sam 20250326 : 新增 Shake 條件設定
        {

        }
        else
        {
            return false;
        }
    }

    if(p!=iOldPos)                                                              //jou 2010-12-23 保護兩次命令會造成撞機
    {
        fCMD=false;
        iOldPos=p;
    }

    int ret=0;
    int iCommandPos;
    int iGap=1;

    if(fCanMove ==false  ||
//       fCanMoveR==false  ||
       fCanMoveM==false  ||
       fCanMoveL==false  ||                                                     //Klutter 20210817 加入鎖馬達機制
       mapLockList.size()!=0)                                                   //Steven 20210825 : 吹氣完成才可以歸零
    {
        PCIL132_StopMotor();

        if((Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2) && (Motor->GearRatio>1))
        {
            iCommandPos=ReadPos();
            if(iCommandPos>p+iGap)                                              //超出+
            {
                return false;
            }
            else if(iCommandPos<p-iGap)
            {
                return false;                                                   //低於-
            }
            else
            {
                fCMD=false;
                return true;                                                    //正常等於
            }
        }
        else
        {
            if(p==ReadPos())
            {
                fCMD=false;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    ret=MotorMovePosition(Position, speed, p);
    if(bShowMotorMove==true)
    {
        ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
        if(bPanelUse)
        {
            if(bUpDownMove)
            {
                if(abs(PWinCtrl->Top-ScreenPos)>2)
                    PWinCtrl->Top=ScreenPos;
            }
            else
            {
                if(abs(PWinCtrl->Left-ScreenPos)>2)
                    PWinCtrl->Left=ScreenPos;
            }
        }
    }

    if(ret==1)
    {
        fCMD=false;
        return true;
    }
    else
    {
        return false;
    }
}
//--------------------------------------------------------------------------------
void RecordIndexPosition(int iArm,int Part)                                     //Isaac 20200922 : 紀錄indexArmY encoder值和command值
{
    AnsiString Strtemp="",StrType="";
    AnsiString sFileName="",sFileName2="",StrIndexLog="";
    GetTimeInfo();

    int y1CmdPos     = MOT[MTestY1].Gali_ReadPos();
    int y1EncoderPos = MOT[MTestY1].Gali_ReadEncoderPos();
    int y2CmdPos     = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadPos();
    int y2EncoderPos = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadEncoderPos();
    int z1CmdPos     = MOT[MTestZ1].Gali_ReadPos();
    int z1EncoderPos = MOT[MTestZ1].Gali_ReadEncoderPos();
    int z2CmdPos     = MOT[MTestZ2].Gali_ReadPos();
    int z2EncoderPos = MOT[MTestZ2].Gali_ReadEncoderPos();
    if(Part==3)                                                                 //Error
    {
        if(iArm==0)                                                             //Handler status Halt
        {
            StrType="Halt";
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArmHalt_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }
        else if(iArm==3)                                                        //Gali_Two_ZAxis_Move
        {
            StrType="Gali_Two_ZAxis_Move";
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArm12_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }
        else                                                                    //Arm1,Arm2
        {
            StrType.sprintf("IndexArm%d",iArm);
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArm%d_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, iArm, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }

        Strtemp.sprintf("%s :Y1CMD:%d, Y1POS:%d, Teach F:%d, M:%d, Y2CMD:%d, Y2POS:%d, Teach M:%d, R:%d, Z1CMD:%d, Z1POS:%d, Z2CMD:%d, Z2POS:%d",
                StrType,
                y1CmdPos, y1EncoderPos, Prod.TestY1_Front, Prod.TestY1_Middle,
                y2CmdPos, y2EncoderPos, Prod.TestY2_Middle, Prod.TestY2_Rear,
                z1CmdPos, z1EncoderPos,
                z2CmdPos, z2EncoderPos);
        fMain->AddIndexPosLog(Strtemp);
        fMain->AddIndexPosLog("Save Log", true);
    }
    else                                                                        //Part : Shuttle==0 or Socket==1
    {
        if(fMain->cbBoth->Checked)                                              //Record Both Arm1 and Arm2
        {
            if (iArm == 1)
            {
                if (Part == 0)  // Shuttle
                {
                    Strtemp.sprintf("Arm1 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY1_Front, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
                else if (Part == 1)  // Socket
                {
                    Strtemp.sprintf("Arm1 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY1_Middle, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
            }
            else  // iArm == 2
            {
                if (Part == 0)  // Shuttle
                {
                    Strtemp.sprintf("Arm2 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY2_Rear, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
                else if (Part == 1)  // Socket
                {
                    Strtemp.sprintf("Arm2 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY2_Middle, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
            }
        }
        else if (fMain->cbArm1->Checked && iArm == 1)  // Record Arm1 only
        {
            if (Part == 0)  // Shuttle
            {
                Strtemp.sprintf("Arm1 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d",
                                Prod.TestY1_Front, y1CmdPos, y1EncoderPos);
            }
            else if (Part == 1)  // Socket
            {
                Strtemp.sprintf("Arm1 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d",
                                Prod.TestY1_Middle, y1CmdPos, y1EncoderPos);
            }
        }
        else if (fMain->cbArm2->Checked && iArm == 2)  // Record Arm2 only
        {
            if (Part == 0)  // Shuttle
            {
                Strtemp.sprintf("Arm2 on Shuttle : Teach:%d, Y2CMD:%d, Y2POS:%d",
                                Prod.TestY2_Rear, y2CmdPos, y2EncoderPos);
            }
            else if (Part == 1)  // Socket
            {
                Strtemp.sprintf("Arm2 on Socket : Teach:%d, Y2CMD:%d, Y2POS:%d",
                                Prod.TestY2_Middle, y2CmdPos, y2EncoderPos);
            }
        }
        else
        {
            return;
        }

        fMain->AddIndexPosLog(Strtemp);
    }
    return;
}
//--------------------------------------------------------------------------------
void EncoderTeachingMaxMinCount(int iRecordArm)                                 //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值
{
    int check1=0, check2=0;
    int iEncoderPos=0;

    if(iRecordArm==1)                                                           //Arm1
    {
        iEncoderPos=MOT[MTestY1].Gali_ReadEncoderPos();

        check1=abs(iEncoderPos-Prod.TestY1_Front);
        check2=abs(iEncoderPos-Prod.TestY1_Middle);

        if(check1>5000)                                                         //Middle
        {
            if(check2>iMaxTeachY1M)
            {
                iMaxTeachY1M=check2;
            }
            else if(check2<iMinTeachY1M)
            {
                iMinTeachY1M=check2;
            }
        }
        else                                                                    //Front
        {
            if(check1>iMaxTeachY1F)
            {
                iMaxTeachY1F=check1;
            }
            else if(check1<iMinTeachY1F)
            {
                iMinTeachY1F=check1;
            }
        }
    }
    else if(iRecordArm==2)                                                      //Arm2
    {
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)
        {
            iEncoderPos=0;
        }
        else
        {
            iEncoderPos=MOT[MTestY2].Gali_ReadEncoderPos();
        }

        check1=abs(iEncoderPos-Prod.TestY2_Middle);
        check2=abs(iEncoderPos-Prod.TestY2_Rear);

        if(check1>5000)                                                         //Rear
        {
            if(check2>iMaxTeachY2R)
            {
                iMaxTeachY2R=check2;
            }
            else if(check2<iMinTeachY2R)
            {
                iMinTeachY2R=check2;
            }
        }
        else                                                                    //Middle
        {
            if(check1>iMaxTeachY2M)
            {
                iMaxTeachY2M=check1;
            }
            else if(check1<iMinTeachY2M)
            {
                iMinTeachY2M=check1;
            }
        }
    }
    return;
}
//------------------------------------------------------------------------------
void InitialMaxMinValue(AnsiString StrTask)                                     //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，歸零
{
    AnsiString StrIndexLog="";

    iMaxCommandY1=0; iMinCommandY1=0;
    iMaxCommandY2=0; iMinCommandY2=0;
    iMaxTeachY1F=0; iMinTeachY1F=0;
    iMaxTeachY1M=0; iMinTeachY1M=0;
    iMaxTeachY2M=0; iMinTeachY2M=0;
    iMaxTeachY2R=0; iMinTeachY2R=0;

    GetTimeInfo();
    StrIndexLog.sprintf("%s(%s)", "Max/Min Value Initial.", StrTask);
    fMain->AddIndexPosLog(StrIndexLog);
    fMain->slIndexYMaxMinShift->AddTextWithDateTime(StrIndexLog);
}
//------------------------------------------------------------------------------
void TrigerIndexAxisHome()                                                      //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
{
    AnsiString StrIndexLog="";
    RecordProcess("Auto-calibration Start.");

    fMain->AddIndexPosLog("Auto-calibration Start.");
    LogIndexMaxMinPos("TrigerIndexAxisHome()");                                 //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，記錄並存檔

    DoIndex4AxisHome(true);                                                     //Isaac 20201012 : index Y超過範圍，做一次Tmode，indexArm四軸先回home，初始化
}
//==============================================================================
// all function about safe lock by other motor
//==============================================================================//15.09.07.01s  klutter  //new struct
void TMyMotor::Lock(AnsiString MotorAlias, AnsiString FunctionName, int Task)   //Steven 20210825 : 吹氣完成才可以歸零
{
    AnsiString str;
    str.sprintf("%s_%s", MotorAlias, FunctionName);
    mapLockIter=mapLockList.find(str);
    if(mapLockIter==mapLockList.end())
    {
        mapLockList[str]=Task;
    }
}
//==============================================================================
void TMyMotor::UnLock(AnsiString MotorAlias, AnsiString FunctionName)           //Steven 20210825 : 吹氣完成才可以歸零
{
    AnsiString str;
    str.sprintf("%s_%s", MotorAlias, FunctionName);
    mapLockIter=mapLockList.find(str);
    if(mapLockIter!=mapLockList.end())
    {
        mapLockList.erase(mapLockIter);
    }
}
//==============================================================================
void TMyMotor::ClearLock()                                                      //Steven 20210825 : 吹氣完成才可以歸零
{
    mapLockList.clear();
}
//==============================================================================
int TMyMotor::GetLockCount()                                                    //Steven 20210825 : 吹氣完成才可以歸零
{
    return (int)mapLockList.size();
}
//==============================================================================
AnsiString TMyMotor::GetLockString(int Index)                                   //Steven 20210825 : 吹氣完成才可以歸零
{
    AnsiString S="";
    int i=0;

    if(Index<(int)mapLockList.size())
    {
        for(mapLockIter=mapLockList.begin(); mapLockIter!=mapLockList.end(); mapLockIter++)
        {
            if(i==Index)
            {
                S.sprintf("%s_%s", mapLockIter->first, mapLockIter->second);
            }
            else
            {
                i++;
            }
        }
    }
    return S;
}
//==============================================================================
bool TMyMotor::CheckIndexYPos(bool Front)                                       //Ztex 2023.05.11 Add CheckIndexY
{
    bool bResult=false;

    bResult=CheckPos(Front);

    return bResult;
}
//--------------------------------------------------------------------------------
bool TTrayMotor::HasOnlyDataICAndNullIC(int DataType)                           //JimmyChiu 20220908 add Pickup Error Placement
{
    return Tray.HasOnlyDataICAndNullIC(DataType);
}
//------------------------------------------------------------------------------
void TTrayMotor::MoveTrayData(TTrayMotor &TrayMotor)                            //JerryYang 20221215 : add Magazine  // 2011.01.21 , Joye , Auto 3 Magazine
{
    fHasTray    =TrayMotor.fHasTray;
    Tray.XItem  =TrayMotor.Tray.XItem;
    Tray.YItem  =TrayMotor.Tray.YItem;

    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            Tray.Data[i][j]=TrayMotor.Tray.Data[i][j];
        }
    }
    Tray.cCassetteID=TrayMotor.Tray.cCassetteID;
    Refresh();
    TrayMotor.Tray.cCassetteID="";
    TrayMotor.ClearTray(__FUNC__);
    TrayMotor.fHasTray=false;
    TrayMotor.Refresh();
}
//------------------------------------------------------------------------------
bool TMyMotor:: Check_SHUTTLE_FLOODGATE_Staste(int iIndex)                      //Ztex 2023.06.02 Add Check_SHUTTLE_FLOODGATE_Staste
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        bool bResult=false;

        if(SHUTTLE_FLOODGATE==0)
            return false;
        if(iIndex==0)
        {
            if(Cylinder[C_Shuttle1Floodgate].OnSensor()==true &&
               Cylinder[C_Shuttle1Floodgate].OffSensor()==false &&
               Cylinder[C_OutShuttle1Floodgate].OnSensor()==true&&              //Ifor 20240620 add:Out Shuttle Floodgate
               Cylinder[C_OutShuttle1Floodgate].OffSensor()==false)             //Ifor 20240620 add:Out Shuttle Floodgate
                bResult=true;
            else
                bResult=false;
        }
        else
        {
            if(Cylinder[C_Shuttle2Floodgate].OnSensor()==true &&
               Cylinder[C_Shuttle2Floodgate].OffSensor()==false &&
               Cylinder[C_OutShuttle2Floodgate].OnSensor()==true&&              //Ifor 20240620 add:Out Shuttle Floodgate
               Cylinder[C_OutShuttle2Floodgate].OffSensor()==false)             //Ifor 20240620 add:Out Shuttle Floodgate
                bResult=true;
            else
                bResult=false;
        }
        return bResult;
    #endif
}
//------------------------------------------------------------------------------
int iTrayXTask;
int iTrayOldPos;
//------------------------------------------------------------------------------
void TMyMotor::TrayArmInitial()
{
    iTrayXTask=1;
    iTrayOldPos=0;
}
//------------------------------------------------------------------------------
bool TrayArmMotorMove(int p, bool bCheckPos)                                    //bOnOff = true Door Open , bOnOff = false Door Off
{
    bool bResult=false;
    //Sam 20241206 : Tray Arm 新增 Teach 保護
    //==>
    AnsiString sMsg="";
    int iSafePos=0;
    int iTrayArmTarget=-1;
    if(p==Prod.iXTrayLoad ||
       p==Prod.iXTrayLoad_ART)
    {
        iTrayArmTarget=eTrayArmLoader;
    }
    else if(p==Prod.iXTrayEmpty)
    {
        iTrayArmTarget=eTrayArmEmpty;
    }
    else if(p==Prod.iXTrayColor)
    {
        iTrayArmTarget=eTrayArmColor;
    }
    else if(p==Prod.iXTrayAuto[0] ||
            p==Prod.iXTrayAuto[1] ||
            p==Prod.iXTrayAuto[2] ||
            p==Prod.iXTrayAuto[3] ||
            p==Prod.iXTrayAuto[4] ||
            p==Prod.iXTrayAuto[5] ||
            p==Prod.iXTrayAuto_ART[0] ||
            p==Prod.iXTrayAuto_ART[1] ||
            p==Prod.iXTrayAuto_ART[2] ||
            p==Prod.iXTrayAuto_ART[3] ||
            p==Prod.iXTrayAuto_ART[4] ||
            p==Prod.iXTrayAuto_ART[5])
    {
        iTrayArmTarget=eTrayArmAuto;
    }

    if(USE_LdUldCassetteMode==1)                                                //KenHsieh 20251220 : Cassette mode safe pos is Empty real pos +1000
    {
        iSafePos=Tech.iTrayXEmpty+6800+1000;
    }
    else if(MachineTypeChoice==Type_HT9045)
    {
        iSafePos=45742;                                                         //此用 HT9045機台 Staterecord empty & color 的 Teach 點取中心。
    }
    else if(MachineTypeChoice==Type_HT9046)
    {
        iSafePos=49750;                                                         //此用 HT9046機台 Staterecord empty & color 的 Teach 點取中心。
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)
    {

    }
//    else if(MachineTypeChoice==Type_HT1032)
//    {
//
//    }
//    else if(MachineTypeChoice==Type_HT7080)
//    {
//
//    }
    else if(MachineTypeChoice==Type_HT9046_LS)
    {
//        iSafePos=63800;                                                         //此用 HT9046LS機台 Staterecord empty & color 的 Teach 點取中心。
        iSafePos=(Tech.iTrayXEmpty+Tech.iTrayXColor)/2+6500;                    //Steven 20250122 : 改用Teaching位子判斷
        /*
        if(SubMachineType==Type_HT9046LA)

        else if(SubMachineType==Type_HT9016C)

        else if(SubMachineType==Type_HT9046AU ||                                //Steven 20240822 : For HT-9046AU
                USE_OUT_SORT_ARM!=eartUninstall)

        else if(SubMachineType==Type_HT9046CR)                                  //Steven 20241001 : For HT-9046CR

        else
        */
    }

    if(iSafePos>0 && bCheckPos)
    {
        if(USE_LdUldCassetteMode==1 &&
           Tech.iTrayXEmpty!=Tech.iTrayXColor)                                  //KenHsieh 20251220 : Cassette mode empty & color pos need same
        {
            sMsg.sprintf("TrayArm empty & color pos need same.");
            ShowMyMessage(sMsg);
            return bResult;
        }
        else if(iTrayArmTarget==eTrayArmLoader ||
           iTrayArmTarget==eTrayArmEmpty)
        {
            if(p>iSafePos)
            {
                sMsg.sprintf("TrayArm moves %d to the left error. Please check loader and empty teach position.", p);
                ShowMyMessage(sMsg);
                return bResult;
            }
        }
        else if(iTrayArmTarget==eTrayArmColor ||
                iTrayArmTarget==eTrayArmAuto)
        {
            if(p<iSafePos)
            {
                 sMsg.sprintf("TrayArm moves %d to the right error. Please check color and auto teach position.", p);
                 ShowMyMessage(sMsg);
                 return bResult;
            }
        }
        else
        {
            sMsg.sprintf("TrayArm moves %d unknown error. Please check TrayArm teach position.", p);
            ShowMyMessage(sMsg);
            return bResult;
        }
    }
    //<==
    //Sam 20241206 : Tray Arm 新增 Teach 保護

    #ifdef SOFT_SIMULTE
        bResult=MOT[MTrayX].MotorMove(p);
    #else
        static bool bCyflag[4]={false, false, false, false};
        int &Task=iTrayXTask;
        if(TRAY_ARM_MODE==eAboveCoveyor)
        {
            bResult=MOT[MTrayX].MotorMove(p);
        }
        else if(USE_LdUldCassetteMode==1)                                       //RogerYang 20260203 : Add for HT9046CR 移動保護
        {
            if(p==Prod.iXTrayColor ||
                p==Prod.iXTrayEmpty)
            {
                bResult=MOT[MTrayX].MotorMove(p);
            }
            else
            {
                bResult=true;
            }
        }
        else
        {
            switch(Task)
            {
                case 1:
                    for(int i=0; i<4; i++)
                        bCyflag[i]=false;

                    if(iTrayOldPos!=p)
                    {
                        Task=100;
                    }
                    else
                    {
                        bResult=true;
                    }
                    break;
                case 100:
                    if(bCyflag[0]==false)
                        bCyflag[0]=(Cylinder[C_TrayXFloodgate1].Enable==false || Cylinder[C_TrayXFloodgate1].Push());
                    if(bCyflag[1]==false)
                        bCyflag[1]=(Cylinder[C_TrayXFloodgate2].Enable==false || Cylinder[C_TrayXFloodgate2].Push());
                    if(bCyflag[2]==false)
                        bCyflag[2]=(Cylinder[C_TrayXFloodgate3].Enable==false || Cylinder[C_TrayXFloodgate3].Push());
                    if(bCyflag[3]==false)
                        bCyflag[3]=(Cylinder[C_TrayXFloodgate4].Enable==false || Cylinder[C_TrayXFloodgate4].Push());

                    if(bCyflag[0] && bCyflag[1] && bCyflag[2] && bCyflag[3])
                    {
                        Task=200;
                    }
                    break;
                case 200:
                    if((Cylinder[C_TrayXFloodgate1].Enable==true && Cylinder[C_TrayXFloodgate1].OffSensor()==true) ||
                       (Cylinder[C_TrayXFloodgate2].Enable==true && Cylinder[C_TrayXFloodgate2].OffSensor()==true) ||
                       (Cylinder[C_TrayXFloodgate3].Enable==true && Cylinder[C_TrayXFloodgate3].OffSensor()==true) ||
                       (Cylinder[C_TrayXFloodgate4].Enable==true && Cylinder[C_TrayXFloodgate4].OffSensor()==true))
                    {
                        for(int i=0; i<4; i++)
                            bCyflag[i]=false;
                        Task=100;
                        break;
                    }

                    if(MOT[MTrayX].MotorMove(p))
                    {
                        for(int i=0; i<4; i++)
                            bCyflag[i]=false;
                        Task=300;
                    }
                    break;
                case 300:
                    if(bCyflag[0]==false)
                        bCyflag[0]=(Cylinder[C_TrayXFloodgate1].Enable==false || Cylinder[C_TrayXFloodgate1].Pop());
                    if(bCyflag[1]==false)
                        bCyflag[1]=(Cylinder[C_TrayXFloodgate2].Enable==false || Cylinder[C_TrayXFloodgate2].Pop());
                    if(bCyflag[2]==false)
                        bCyflag[2]=(Cylinder[C_TrayXFloodgate3].Enable==false || Cylinder[C_TrayXFloodgate3].Pop());
                    if(bCyflag[3]==false)
                        bCyflag[3]=(Cylinder[C_TrayXFloodgate4].Enable==false || Cylinder[C_TrayXFloodgate4].Pop());

                    if(bCyflag[0] && bCyflag[1] && bCyflag[2] && bCyflag[3])
                    {
                        bResult=true;
                        iTrayOldPos=p;
                        Task=1;
                    }
                    break;
            }
        }
    #endif
    return bResult;
}
//------------------------------------------------------------------------------
bool TMyMotor::Check_Y1Y2_TargetPosWillCrash(int iY1_RelPos, int iY2_RelPos)      //JerryYang 20250822 : fix index y保護  //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
{
    int iY1TarPos=0, iY2TarPos=0;
    AnsiString str="";

    iY1TarPos=MOT[MTestY1].Gali_ReadEncoderPos()+iY1_RelPos;
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
        iY2TarPos=MOT[MTestY2].Gali_ReadEncoderPos()+iY2_RelPos;

    bool bY1UnSafe=iY1TarPos>(Tech.iHT9040TestY1_Middle-7000);
    bool bY2UnSafe=(USE_INDEX_ARM_AXES==IndexArm_4_Axis)?(iY2TarPos<(Tech.iHT9040TestY2_Middle+7000)):false;
    if(bY1UnSafe &&
       bY2UnSafe)
    {
        str.sprintf("Index Y Position Calculate error. Y1 Target Pos: %d, Y2 Target Pos:%d", iY1TarPos, iY2TarPos);
        NewRecordProcess("", str);
        return true;
    }
    else
    {
        return false;
    }

}
//------------------------------------------------------------------------------
bool TMyMotor::CheckY1Y2TargetPos(int Pos, AnsiString asErrorFunc)
{
    int iY1TarPos=0, iY2TarPos=0;
    AnsiString str="";

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)
    {
        return true;
    }

    if(Mot_Name==MTestY1)
    {
        iY1TarPos=Pos;
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)
            iY2TarPos=MOT[MTestY2].Gali_ReadPos();

        bool bY1UnSafe=iY1TarPos>(Tech.iHT9040TestY1_Middle-7000);
        bool bY2UnSafe=(USE_INDEX_ARM_AXES==IndexArm_4_Axis)?(iY2TarPos<(Tech.iHT9040TestY2_Middle+7000)):false;
        if(bY1UnSafe &&      //JerryYang 20250822 : fix index y保護
           bY2UnSafe)
        {
            str.sprintf("Index Y Position Calculate error. Y1 Target Pos: %d, Y2 Target Pos:%d", iY1TarPos, iY2TarPos);
            bShowIndexMotorError=true;
            ShowMyMessage(str);

            fAllMotorHome=false;
            iHome=1;
            return false;
        }
    }
    else if(Mot_Name==MTestY2)
    {
        iY1TarPos=MOT[MTestY1].Gali_ReadPos();
        iY2TarPos=Pos;
        if((iY1TarPos>(Tech.iHT9040TestY1_Middle-7000)) &&
           (iY2TarPos<(Tech.iHT9040TestY2_Middle+7000)))
        {
            str.sprintf("Index Y Position Calculate error. Y1 Target Pos: %d, Y2 Target Pos:%d", iY1TarPos, iY2TarPos);
            bShowIndexMotorError=true;
            ShowMyMessage(str);

            fAllMotorHome=false;
            iHome=1;
            return false;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TMyMotor::Check_Y1_TargetPosWillCrash(int iY1TarPos, AnsiString sFunc)     //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
{
    AnsiString str="";
    if(MOT[MTestY1].Motor->Enable)
    {
        if(iY1TarPos>(Tech.iHT9040TestY1_Middle-7000))
        {
            str.sprintf("Index Y Position Calculate error. Y1 Target Pos: %d Func:%s", iY1TarPos, sFunc);
            NewRecordProcess("Motion", str);
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Check_Y2_TargetPosWillCrash(int iY2TarPos,AnsiString sFunc)      //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
{
    AnsiString str="";
    if(MOT[MTestY2].Motor->Enable)
    {
        if(iY2TarPos<(Tech.iHT9040TestY2_Middle+7000))
        {
            str.sprintf("Index Y Position Calculate error. Y2 Target Pos: %d Func:%s", iY2TarPos, sFunc);
            NewRecordProcess("Motion", str);
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyMotor::Check_Y1_TargetPosInTeachPos(int iY1TarPos, AnsiString sFunc)
{
    bool bret=false;
    if(CheckArmPosArrival(iY1TarPos, Prod.TestY1_Front,  IniConfig.GaliPosRange))
    {
        bret=true;
    }
    else if(CheckArmPosArrival(iY1TarPos, Prod.TestY1_Middle, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
    {
        bret=true;
    }
    if(bret==false)
    {
//        AnsiString str=AnsiString().sprintf("Index Y1 Position Calculate error. Y1 Target Pos: %d, Front Pos: %d, Middle Pos: %d Func:%s", iY1TarPos, Prod.TestY1_Front, Prod.TestY1_Middle, sFunc);
//        NewRecordProcess("Motion", str);
    }
    return bret;
}
//------------------------------------------------------------------------------
bool TMyMotor::Check_Y2_TargetPosInTeachPos(int iY2TarPos, AnsiString sFunc)
{
    bool bret=false;
    if(CheckArmPosArrival(iY2TarPos, Prod.TestY2_Rear, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
    {
        bret=true;
    }
    else if(CheckArmPosArrival(iY2TarPos, Prod.TestY2_Middle, IniConfig.GaliPosRange))  //JerryYang 20230309 : 修正index arm移動保護
    {
        bret=true;
    }
    if(bret==false)
    {
//        AnsiString str=AnsiString().sprintf("Index Y2 Position Calculate error. Y2 Target Pos: %d, Rear Pos: %d, Middle Pos: %d Func:%s", iY2TarPos, Prod.TestY2_Rear, Prod.TestY2_Middle, sFunc);
//        NewRecordProcess("Motion", str);
    }
    return bret;
}
//------------------------------------------------------------------------------
