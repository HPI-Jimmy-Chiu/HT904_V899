#include "MachineDefine.h"
#pragma hdrstop

#include "aRotateKIT.h"

#include "aRotateKIT_In.h"
#include "aRotateKIT_Out.h"
#include "aArmHeader.h"
#include "cprod.h"
#include "csystem.h"
#include "mymotor.h"
#include "MyKitSuck.h"
#include "mycylin.h"
#include "mysensor.h"
#include "main.h"
#include "note.h"
#include "fRotate.h"
#include "uhome.h"
#include "cinitial.h"
#include "cMyDB.h"
#include "cmydef.h"
#include "SCK_ART.h"
#include "fAOI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int iRotateKIT_Start_X_H = 7800;
int iRotateKIT_Start_Y_H = 7975;
int iRotateKIT_Pitch_X_H = 8000;
int iRotateKIT_Pitch_Y_H = 6000;
int iRotateKIT_Start_X_A = 2800;
int iRotateKIT_Start_Y_A = 7000;
int iRotateKIT_Pitch_X_A = 8000;
int iRotateKIT_Pitch_Y_A = 6000;

int i2x2Suck                =0;     //判斷2x2kit時用第幾支吸嘴IC <2500 =2 (Suck[0][2]) ;IC >2500 =3  (suck[0][3]);
int i2x2Suck_Out            =0;
//---------------------------------------------------------------------------
void RotateKIT_FormSet(bool bInput, int iStartX, int iStartY, int iPitchX, int iPitchY)
{
    if(bInput==true)
    {
        iRotateKIT_Start_X_H = iStartX;
        iRotateKIT_Start_Y_H = iStartY;
        iRotateKIT_Pitch_X_H = iPitchX;
        iRotateKIT_Pitch_Y_H = iPitchY;
    }
    else
    {
        iRotateKIT_Start_X_A = iStartX;
        iRotateKIT_Start_Y_A = iStartY;
        iRotateKIT_Pitch_X_A = iPitchX;
        iRotateKIT_Pitch_Y_A = iPitchY;
    }
}
//---------------------------------------------------------------------------
//kevin 20131003 傳送馬達角度pluse
//const int MotorResolution=8000;       //日本Tamagawa 步進馬達
//---------------------------------------------------------------------------
int SetMotorResolution(int &Ang45, int &Ang90, bool bInRotate)
{
    int buffer=0;
    int iBacklash=0;
//    int iAOIInAngle=0;
//    int iAOIOutAngle=0;

    if(iRotate_Type==e4MotRotate ||
       iRotate_Type==e8MotRotate ||
       iRotate_Type==e2MotRotate2Dut)
    {                                                                           //Steven 20170427 (wei) : 六軸步進版就是3200轉一圈
        Ang45=400;
        Ang90=800;
    }
    else if(iRotate_Type==eInOutArm1Motor)                                      //Frank 20250812 : modify InOutArm1Motor
    {
        Ang45=625;                                                              // 5000轉一圈
        Ang90=1250;
    }
    else
    {
        Ang45=1000;                                                             //MotorResolution/8; //kevin 20130710 旋轉45 度
        Ang90=2000;                                                             //MotorResolution/4;
    }

//    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)
//    {
//        iAOIInAngle=FrmAOI->ttbInsp->iRotateKitAngOffset_In;
//        iAOIOutAngle=FrmAOI->ttbInsp->iRotateKitAngOffset_Out;
//    }

    if(bInRotate)
    {   //inrotate 0 度 水平位置
        buffer=Prod.iIn_iRotateA;
        iBacklash=MOT[MInRotateKit].GetRotatorBacklash(buffer, true);           //RogerYang 20260113 : Rotator新增背隙補償
        buffer+=iBacklash;
    }
    else
    {   //outrotate shuttle 一樣方向位置 0 度 水平位置
//        buffer=Ang90*tRotate.RotationTime+Prod.iOut_iRotateA ;
        //jou 2014-03-06 make code 看不懂為什麼要先轉
//        if(DeviceForm_File.XDimension != DeviceForm_File.YDimension)            //20140307 wei 如果長方形的要先轉  //JerryYang 20160603 MARK掉,修正旋轉角度錯誤
//        {
//            buffer=Ang90*tRotate.RotationTime+Prod.iOut_iRotateA ;
//        }
//        else
//        {
        buffer=Prod.iOut_iRotateA;
        iBacklash=MOT[MOutRotateKit].GetRotatorBacklash(buffer, false);         //RogerYang 20260113 : Rotator新增背隙補償
        buffer+=iBacklash;
//        }
    }

    return buffer;
}
//---------------------------------------------------------------------------
void InitSuckState()
{
    if((TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4) && ArmCanSuck4IC(0)==false || TestIF.iTestMode==_16Site2X8 && ArmCanSuck4IC(0)==false) //Sam 20190226 : 16Site4X4 // 2x4 && IC 大於2500mm
    {
        i2x2Suck=3;
        //i2x2Suck_Out=2;
        i2x2Suck_Out=3;                                                         //Sam 20211224 : 修正為使用 1 4 吸嘴
        //AI(ht9045-v899) 20260810: 2x4 交錯 4 站(e9045_2x4_4_13/_14)時 SetInOutArmParameter_2x4_4() 已用 CopyInitSuck 把 OutArm 資料搬到邏輯欄位 0 與 1, 但這裡仍給 3(欄位 0 與 3), 使 M_MoveOutArmZ_ToRotateKIT_Place()/_Pick() 的列舉永遠碰不到吸嘴 (1,1), B 排那顆(DUT1/DUT3)不放旋轉站就直接進 Auto tray; 比照下方 2x2 的既有先例改為 1。i2x2Suck(InArm)維持 3, 因為 2x4_4 只搬 OutArm 的資料, InArm 未搬
        if(USE_PICKER_COUNT!=ep16Picker &&
           (iInArmType==e9045_2x4_4_13 || iInArmType==e9045_2x4_4_14))
        {
            i2x2Suck_Out=1;
        }
    }
    else if(TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==DualSite)                                         //Sam 20250428 : 修正 1X2 OutRotate 旋轉異常
    {
        i2x2Suck=2;
        i2x2Suck_Out=1;                                                         //SetInOutArmParameter_2X2_4 已經將 OutArm 的資料搬到 1 2 位置上  //Sam 20250217 : 修正 2X2 OutRotate 旋轉異常
    }
    else// if(tRotate.DutNum==tDutType_4)
    {
        i2x2Suck=2;
        i2x2Suck_Out=2;
    }
}
//---------------------------------------------------------------------------
//Rotate 偵測是否疊料或放不好   kevin 20130524
//---------------------------------------------------------------------------
bool CheckRotateSensor(bool bRotateIn)
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
    if(bRotateIn)// In rotate
    {
        //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
        if(Sen[SnRotateRowIn1].Enable==true && Sen[SnRotateRowIn2].Enable==false)
        {
            if(Sen[SnRotateRowIn1].IsOn())
                return true;
        }
        else
        {
            if(Sen[SnRotateRowIn1].Enable==false || Sen[SnRotateRowIn2].Enable==false)
            {
                return true;
            }
            else if(Sen[SnRotateRowIn1].IsOn() && Sen[SnRotateRowIn2].IsOn())
            {
                return true;
            }
        }
    }
    else
    {   //out rotate
        //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
        if(Sen[SnRotateRowOut1].Enable==true && Sen[SnRotateRowOut2].Enable==false)
        {
            if(Sen[SnRotateRowOut1].IsOn())
                return true;
        }
        else
        {
            if(Sen[SnRotateRowOut1].Enable==false || Sen[SnRotateRowOut2].Enable==false)
            {
                return true;
            }
            else if(Sen[SnRotateRowOut1].IsOn() && Sen[SnRotateRowOut2].IsOn())
            {
                return true;
            }
        }
    }
    return false;
    #endif
}
//---------------------------------------------------------------------------
bool CheckNeedRotateWithoutHotIC()  //Steven 20160809 : 改成Function
{
    bool bHasHotICNeedRotate=false;
    if(TestIF.iTestMode==DualSite && LastSet.iTemperature==Tempture_Hot && TestIF_File.UseRotateForHT7000HPKit)   //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
        bHasHotICNeedRotate=true; //加熱完的要放 Rotate

    bool Result=false;
    if(InArmSuck.HasRealIC()==true &&           //Steven 20161102 : 修正從加熱盤吸取全部都是HAS_NULL_IC時會Hang up
       InArmSuck.HasType(HAS_HOT_IC)==bHasHotICNeedRotate &&  //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit //ChungHung 20131219 HAS_HOT_IC 不需跑ROTATE
       USE_ROTATE_KIT==1 && tRotate.ActiveRotate && TrayForm.iRotateKIT_InputType!=0 && (iInRotateFinish==0 || iInRotateFinish==1)) //kevin 20130524 發生alarm inarm到等待點
    {
        if(iInRotateFinish==0) //尚未到旋轉站。
        {
            InitialInArmRotateKIT();
//            iInRotateFinish=1;
        }
        Result=true;
    }
    return Result;
}
//---------------------------------------------------------------------------
