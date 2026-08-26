#include "MachineDefine.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#include "SortingBinTray.h"
#include "csystem.h"
#include "MyMotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "note.h"
#include "mymessbox.h"
#include "main.h"
#include "ainarm2.h"
#include "aoutarm.h"
#include "mykitsuck.h"
#include "acatchtray.h"
#include "aoutarm9045.h"
#include "LastSet.h"
#include "cmydef.h"
#include "atester.h"
#include "uLotInfo.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//***************************************************************************
//2014.01.14 , Mylin , Auto Sorting BinTray by Out Arm when Clean Out
//  This is a Unit to Sort Unloader Bin Tray.
//  When Clean Out, IC in Tray may not in order. So use these function in
//  the Unit to sort those scattered IC.
//***************************************************************************

int iTask_DoSortingBinTray      = 0;
int iTask_ToTrayPickIC          = 0;
int iTask_ToTrayPlaceIC         = 0;

int iTask_DoFix3FullTray        = 1;    //Ifor 20161122 Fix3 Full Tray 主流程
int iTask_DoPickFix3IC          = 1;    //Ifor 20161122 add OutArm To Fix3 放IC流程
int iTask_DoPlaceFix3IC         = 1;    //Ifor 20161122 add OutArm To Fix3 取IC流程

const int TRAYSORTING_NOACT     = 0;
const int TRAYSORTING_ACT       = 1;
const int TRAYSORTING_CHECKOK   = 2;
const int TRAYSORTING_ERR       = -1;

int iXPickPos=0,iYPickPos=0;
int iXPlacePos=0,iYPlacePos=0;

const int SLECT_SUCK0 =0;
const int SLECT_SUCK1 =1;
const int SLECT_SUCK2 =2;
const int SLECT_SUCK3 =3;

int iVariablePara[]={2, 1, 0, -1};

int iNowActionTray = 0; //目前整盤的Tray盤
//TRAY_TYPE_PARA *AutoForm[eTrayCount];

//---------------------------------------------------------------------------
//Bin IC排序主流程
bool DoSortingBinTray(int iFlag)                                                //Bin IC排序主流程
{
    bool bRes =false;
    bool bFlag=false;
    bool flag1=false;
    if(iFlag==0)
    {
        bSortingSuckMode=true;                                                  //Sam 20250415 : 修正 P27 整盤功能
        SortingBinTray_Data.iMAXSUCK_OutArmSuckX=4;                             //Sam 20250207 : 移位置
        SortingBinTray_SetMotorPosData();                                       //設定馬達、及點位資料
        SortingBinTray_SetTraySpecData();                                       //設定Tray盤資料
        iTask_DoSortingBinTray=1;
        return true;
    }

    int &Task=iTask_DoSortingBinTray;

    switch(Task)
    {
        case 1:
            if(MoveOutArmToAutoSafe())
            {
                Task=100;
            }
            break;
        case 100:                                                               //設定初值
            Task=200;
            break;
        case 200:                                                               //CatchArm移到安全點位
            if(MTrayXCanSafeMove()==false ||                                    //Sam 20210616 : 修正 P27 & P24 功能同時開啟時，Clean Out 動作 CatchTray 互搶控制導致撞機
               MOT[MTrayX].fHasTray)                                            //kevin 20170612 (wei) tray arm 是否升起
            {
                Task=200;
                break;
            }

            bFlag=(TrayArmMotorMove(Prod.iXTrayEmpty));
            if(bFlag)
            {
                Task=300;
            }
            break;
        case 300:                                                               //找出是否有Tray盤需要整盤
            for(int i=0; i<eTrayCount; i++)                                     //Ifor 20161123 add Fix3 MAX_TRAY_ITEM 5->6
            {
                if(SortingBinTray_Data.bTrayAction[i]==true)                    //此Tray盤是否需要整盤
                {
                    iNowActionTray=i;                                           //目前整盤的Tray盤
                    Task=1000;
                    return false;
                }
            }
            Task=400;
            break;
        case 400:                                                               //所有Tray盤都整盤完成
            Task=10000;
            break;
//******取IC流程**********************************************************
        case 1000:                                                              //取IC流程
            Task = 1100;
            break;
        case 1100:                                                              //檢查吸嘴是否有IC
            if(OutArmSuck.HasIC()==false)                                       //檢查吸嘴是否有IC
            {
                Task=1200;
                break;
            }
            Task=3000;                                                          //到Tray放IC流程
            break;
        case 1200:                                                              //是否還需要整盤
            if(SortingBinTray_IsBinTrayNeedToSorting(AutoForm[iNowActionTray], &MOT[iMMAuto[iNowActionTray]].Tray)==true) //檢查Bin Tray盤的IC是否需要整盤
            {
                SortingBinTray_ToTrayPickIC(0);                                 //初始化到Tray取IC流程
                Task=1400;
                break;
            }
            SortingBinTray_Data.bTrayAction[iNowActionTray]=false;              //此Tray盤是否需要整盤
            Task=300;
            break;
        case 1400:                                                              //到取IC流程
            if(SortingBinTray_ToTrayPickIC()==true)                             //到Tray取IC流程
            {
                Task=3000;                                                      //放IC流程
                break;
            }
            break;
//******放IC流程**********************************************************
        case 3000:
            Task=3100;
            break;
        case 3100:
            if(OutArmSuck.HasIC()==true)                                        //吸嘴是否有IC
            {
                SortingBinTray_ToTrayPlaceIC(0);                                //初始化到Tray放IC流程
                Task=3200;                                                      //到Tray放IC流程
                break;
            }
            Task=1000;                                                          //到Tray取IC流程
            break;
        case 3200:
            if(SortingBinTray_ToTrayPlaceIC()==true)                            //到Tray放IC流程
            {
                Task=3400;
            }
            break;
        case 3400:                                                              //到取IC流程
            if(EnableFix3UseCylinder())                                         //Sam 20250415 : 修正 P27 整盤功能 Fix3汽缸未歸位的 Hange up 問題
                InitialFix3CanFullTask();
            Task=1000;                                                          //取IC流程
            break;
//******結束流程**********************************************************
        case 10000:
            if(EnableFix3UseCylinder())                                         //Sam 20210330 : 整盤完要確認 Fix 汽缸要回恢復位置
                flag1=UseFix3Cylinder(0);
            else
                flag1=true;

            if(flag1)
                Task=10100;
            else
                break;
        case 10100:
            bSortingSuckMode=false;                                             //Sam 20250415 : 修正 P27 整盤功能                                                             //Bin IC排序主流程完成
            bSortingAllBinTrayFinish=true;                                      //是否所有的Bin Tray IC都整盤完成
            bRes=true;
            break;
    }
    return bRes;
}
//---------------------------------------------------------------------------
bool SortingBinTray_ToTrayPickIC(int iFlag)                                     //到Tray取IC流程
{
    bool bRes =false;
    bool bFlag=false;
    int  ret  =0;
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=iXpitchMaxX3;
    int iMovePitchY=AutoCalculateOutArmYClosePitch(iNowActionTray);
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    AnsiString sHappenRegion = "";
    AnsiString ErrPart=" ";
    if(iFlag==0)
    {
        iTask_ToTrayPickIC=1;
        return true;
    }
    int &Task=iTask_ToTrayPickIC;

    switch(Task)
    {
        case 1:
            Task=100;
//            break;
        case 100:                                                               //設定初值
//            Task=200;
//            break;
//        case 200:                                                               //是否可以到Tray盤取IC
//            Task=300;
//            break;
//        case 300:                                                               //吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                InitialFix3CanFullTask();                                       //Sam 20210824 : 修正整盤時，Fix Cylinder
                Task=400;
            }
            break;
        case 400:                                                               //檢查Tray盤是否有IC
            if(MOT[iMMAuto[iNowActionTray]].Tray.HasRealIC()==true)
            {
                if(iNowActionTray!=eBulkBox &&
                   (EnableFix3UseCylinder()))                                   //Sam 20210824 : 修正整盤時，Fix Cylinder
                {
                    if(UseFix3Cylinder(iNowActionTray)==false)
                        break;
                }
                Task=500;
                break;
            }
            Task=9000;
            break;
        case 500:                                                               // 將程式Tray盤的資料轉換到SortingBinTray的結構中
            SortingBinTray_TrayDataToSortingBin(iNowActionTray);                //將Tray盤的資料轉成SortingBinTray的格式
            MyTrayData.iDirection=OppositeDirection(SortingBinTray_Data.TTraySpec[iNowActionTray].Direction); //找出對角的方向編號
            Task=600;
            break;
        case 600:                                                               //計算到Tray盤取IC的相對點位
            if(SortingBinTray_CalPos(MyParameter, MySuckData, MyTrayData, &MyCalData))
            {
                Task=650;
            }
            break;
        case 650:
            SortingBinTray_SetMotorPosData();                                   //設定馬達、及點位資料
            Task=700;
            break;
        case 700:                                                               //X、Y移動到取IC的位置
            iXPos=SortingBinTray_Data.iXBasePos[iNowActionTray]+MyCalData.iCalXPos;
            iYPos=SortingBinTray_Data.iYBasePos[iNowActionTray]+MyCalData.iCalYPos;
            TransferAutoRatio(iNowActionTray, &iXPos, &iYPos);                  //Auto的軟體齒輪比

            for(int i=0; i<MAX_ARM_Row; i++)                                    //基準第三支吸嘴
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if((i==iOutArmYBase) && (j==iOutArmXBase))
                    {
                        bZFlag[i][j]=true;
                    }
                    else
                    {
                        bZFlag[i][j]=false;
                    }
                }
            }

            iZPos[0][2]=SortingBinTray_Data.iSuckerPickZPos[iNowActionTray];    //基準第三支吸嘴
            iOffsetPos=OutOfsAuto1+iNowActionTray;

            for(int i=0; i<X_PITCH_COUNT; i++)
                iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
            iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

            if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
            {
                Task=1100;
            }
            break;
        case 1100:                                                              //依計算結果，吸嘴取IC
            bFlag=(OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Suck() ||        //JerryYang 2015.09.14
                   OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Error);

            if(bFlag==true)
            {
                Task=1200;
            }
            break;
        case 1200:                                                              //等待穩定時間
            Task=1300;
            break;
        case 1300:                                                              //Z軸吸嘴上升
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:                                                              //檢查IC是否有吸起
            if(OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Error==true)
            {
                ErrPart+=OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].sName;
                ret=OutArmPickTrayAlarm(iNowActionTray, ErrPart);
                if(IniConfig.bInOutArmCanPushHome)                              //Input pickup device error時,按"retry"鍵,機台都會自動home
                {
                    if(ret==K_RETRY)
                    {
                        SetOutArmHome();
                        Task=650;
                        break;
                    }
                }
                else
                {
                    if(ret==K_RETRY)
                    {
                        Task=650;
                        break;
                    }
                }

                if(ret==K_HOME)                                                 //回Home後重吸
                {
                    SetOutArmHome();
                    Task=650;
                    break;
                }

                if(ret==K_SKIP)
                {
                    if(MyCalData.iUseSuck[0][0]==TRAYSORTING_ACT)
                    {
                        MyCalData.iUseSuck[0][0]=TRAYSORTING_ERR;
                        Task=1600;
                        break;
                    }
                }
            }
            MyCalData.iUseSuck[0][0]=TRAYSORTING_CHECKOK;
            Task=1600;
            break;
        case 1500:                                                              //Show 錯誤訊息，重吸、跳過
            Task=1600;
            break;
        case 1600:                                                              //依真空更新吸嘴資料
            SortingBinTray_DataChange(MyParameter, &MySuckData, &MyTrayData, MyCalData);
            Task=1700;
            break;
        case 1700:                                                              //將SortingBinTray的結構轉換到程式中Tray盤的資料
            SortingBinTray_SortingBinToTrayData(iNowActionTray);                //將SOrtingBinTray的資料轉成Tray盤的格式
            Task=10000;
            break;
        case 8000:                                                              //取消到Tray取IC
            bRes=true;
            break;
        case 9000:                                                              //到Tray取IC錯誤
            bRes=true;
            break;
        case 10000:                                                             //設定到Tray取IC完成
            bRes=true;
            break;
    }
    return bRes;
}
//---------------------------------------------------------------------------
bool SortingBinTray_ToTrayPlaceIC(int iFlag)                                    //到Tray放IC流程
{
    bool bRes =false;
    bool bFlag=false;
    int  ret=0;

    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=iXpitchMaxX3;
    int iMovePitchY=AutoCalculateOutArmYClosePitch(iNowActionTray);
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));
    AnsiString ErrPart=" ";
    AnsiString sHappenRegion="";
    if(iFlag==0)
    {
        iTask_ToTrayPlaceIC=1;
        return true;
    }

    int &Task=iTask_ToTrayPlaceIC;

    switch(Task)
    {
        case 1:
            Task=200;
            break;
        case 200:                                                               //吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                InitialFix3CanFullTask();                                       //Sam 20210824 : 修正整盤時，Fix Cylinder
                Task=500;
            }
            break;
        case 500:                                                               //檢查Tray盤是否有空位
            if(MOT[iMMAuto[iNowActionTray]].Tray.FullIC()==false)
            {
                if(iNowActionTray!=eBulkBox &&
                   (EnableFix3UseCylinder()))                                   //Sam 20210824 : 修正整盤時，Fix Cylinder
                {
                    if(UseFix3Cylinder(iNowActionTray)==false)
                        break;
                }
                Task=600;
                break;
            }
            Task=9000;
            break;
        case 600:                                                               //將程式Tray盤的資料轉換到SortingBinTray的結構中
            SortingBinTray_TrayDataToSortingBin(iNowActionTray);                //將Tray盤的資料轉成SortingBinTray的格式
            MyParameter.bIsPickIC=false;
            Task=700;
            break;
        case 700:                                                               //計算到Tray盤放IC的相對點位
            if(SortingBinTray_CalPos(MyParameter, MySuckData, MyTrayData, &MyCalData))
            {
                Task=800;
            }
            break;
        case 800:                                                               //X、Y移動到Tray位置
            iXPos=SortingBinTray_Data.iXBasePos[iNowActionTray]+MyCalData.iCalXPos;
            iYPos=SortingBinTray_Data.iYBasePos[iNowActionTray]+MyCalData.iCalYPos;
            TransferAutoRatio(iNowActionTray, &iXPos, &iYPos);                  //Auto的軟體齒輪比

            for(int i=0; i<MAX_ARM_Row; i++)                                        //基準第三支吸嘴
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if ((i==iOutArmYBase) && (j==iOutArmXBase))
                    {
                        bZFlag[i][j]=true;
                    }
                    else
                    {
                        bZFlag[i][j]=false;
                    }
                }
            }

            iZPos[0][2]=SortingBinTray_Data.iSuckerPlaceZPos[iNowActionTray];    //基準第三支吸嘴
            iOffsetPos=OutOfsAuto1+iNowActionTray;

            for(int i=0; i<X_PITCH_COUNT; i++)
                iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
            iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

            if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
            {
                Task=1200;
            }
            break;
        case 1200:                                                              //依計算結果，吸嘴放IC
            bFlag=(OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Destroy() ||
                   OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Error);
            if(bFlag==true)
            {
                Task=1300;
            }
            break;
        case 1300:
                Task=1400;
            break;
        case 1400:                                                              //Z軸吸嘴上升
            if(MoveOutArmToAutoSafe())
            {
                Task=1500;
            }
            break;
        case 1500:                                                              //檢查IC是否有放掉
            if(OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].Error==true)
            {
                ErrPart+=OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].sName;
                ret=ShowErrorMessage("JAM0217", K_RETRY, MOutArmX, 0, ErrPart); //jou 2015-03-27 JAM0203->JAM0217 Device drop error -> Vacuum sensor OFF error
                if(ret==K_RETRY)
                {
                    //重破
                    Task=700;
                    break;
                }
            }
            Task=1700;
            MyCalData.iUseSuck[0][0] = TRAYSORTING_CHECKOK;
            break;
        case 1700:                                                              //依真空更新吸嘴資料
            SortingBinTray_DataChange(MyParameter, &MySuckData, &MyTrayData, MyCalData);
            Task=1800;
            break;
        case 1800:                                                              //將SortingBinTray的結構轉換到程式中Tray的資料
            SortingBinTray_SortingBinToTrayData(iNowActionTray);                //將SOrtingBinTray的資料轉成Tray盤的格式
            Task=10000;
            break;
        case 8000:                                                              //取消到Tray放IC
            bRes=true;
            break;
        case 9000:                                                              //到Tray放IC錯誤
            bRes=true;
            break;
        case 10000:                                                             //設定到Tray放IC完成
            bRes=true;
            break;
    }
    return bRes;
}
//---------------------------------------------------------------------------
bool DoFix3FullTray()                                                           //Ifor 20161122 Fix3 Full Tray 主流程
{
    int &Task=iTask_DoFix3FullTray;
    int pcs=0;

    switch(Task)
    {
        case 1:
            if(MoveOutArmToAutoSafe())
            {
                Task=100;
            }
            break;
        case 100:                                                               //設定初值
            Task=300;                                                           //Ifor 20170623 (wei) Task 200=>300 Fix3 Full Tray 不需要確認Tray Arm 位置
            break;
        case 300:                                                               //判斷是否有需要整盤
            if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_NULL_IC, &iXPlacePos, &iYPlacePos)==true) //Ifor 20161205 尋找Fix3是否有HAS_NULL_I C
            {
                if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_IC, &iXPickPos, &iYPickPos)==true)  //Ifor 20161205 尋找Fix3是否有HAS_IC
                {
                    Task=1000;
                    return false;
                }
            }
            Task=400;
            break;
        case 400:                                                               //所有Tray盤都整盤完成
            Task=10000;
            break;
//******取IC流程**********************************************************
        case 1000:                                                              //取IC流程
            if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_IC, &iXPickPos, &iYPickPos)==true)
                Task=1100;
            else
                Task=10000;
            break;
        case 1100:                                                              //檢查吸嘴是否有IC
            if(OutArmSuck.HasIC()==false)                                       //檢查吸嘴是否有IC
            {
                Task=1400;                                                      //Ifor 20161125 Task 1200->1400
                break;
            }
            Task=3000;                                                          //到Tray放IC流程
            break;
        case 1400:                                                              //到取IC流程
            if(DoPickTrayIC(iSortingTrayFix3)==true)                            //到Fix3 Tray 取IC流程
            {
                Task=3000;                                                      //放IC流程
                break;
            }
            break;
//******放IC流程**********************************************************
        case 3000:
            Task=3100;
            break;
        case 3100:
            if(OutArmSuck.HasIC()==true)                                        //吸嘴是否有IC
            {
                Task=3200;                                                      //到Tray放IC流程
                break;
            }
            Task=1000;                                                          //到Tray取IC流程
            break;
        case 3200:
            if(DoPlaceTrayIC(iSortingTrayFix3)==true)                           //到Tray放IC流程
            {
                Task = 3300;
            }
            break;
        case 3300:
            pcs=(TestSocket.iShtCnt)+1;

            if(MOT[iMMAuto[iSortingTrayFix3]].HowManyDevice(NULL_IC)<=pcs &&
               MOT[iMMAuto[iSortingTrayFix3]].HowManyDevice(HAS_NULL_IC)!=0)
            {
                Task=1000;                                                      //取IC流程
            }
            else
            {
                Task=3400;                                                      //取IC流程
            }
            break;
        case 3400:
            if(OutputShuttleHasIC())                                            //Ifor 20161122 判斷蝦頭上是否有料要取
            {
                Task=10000;
            }
            else
            {
                Task=1000;                                                      //取IC流程
            }
            break;
//******結束流程**********************************************************
        case 10000:                                                             //Bin IC排序主流程完成
            bSortingAllBinTrayFinish=true;                                      //是否所有的Bin Tray IC都整盤完成
            iTask_DoFix3FullTray=1;
            return true;
//            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool DoPickTrayIC(int iTray)                                                    //Ifor 20161122 add OutArm To Fix3 取IC流程
{
    bool bRes =false;
    bool bFlag=false;
    int  ret  =0;
    AnsiString ErrPart=" ";

    int &Task =iTask_DoPickFix3IC;
    int iUseSuckNum=0;                                                          //Ifor 20170310 (wei) add HT9045S Fix3 Full Tray 真空位置
    if(USE_PICKER_COUNT==0)
    {
        iUseSuckNum=SLECT_SUCK2;                                                //HT9045S 使用基準軸
    }
    else
    {
        iUseSuckNum=SLECT_SUCK3;                                                //HT9045 使用第四支吸嘴
    }

    switch(Task)
    {
        case 1:
            Task=100;
//            break;
        case 100:                                                               //設定初值
            Task=200;
//            break;
        case 200:                                                               //是否可以到Tray盤取IC
            Task=300;
//            break;
        case 300:                                                               //吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                Task=400;
            }
            break;
        case 400:                                                               //檢查Tray盤是否有IC
            if(MOT[iMMAuto[iTray]].Tray.HasRealIC()==true)
            {
                Task=500;
                break;
            }
            Task=9000;
            break;
        case 500:                                                               // 將程式Tray盤的資料轉換到SortingBinTray的結構中
            SortingBinTray_TrayDataToSortingBin_SelectSuck(iTray,HAS_IC);       //將Tray盤的資料轉成SortingBinTray的格式    //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
            MyTrayData.iDirection=OppositeDirection(SortingBinTray_Data.TTraySpec[iSortingTrayFix3].Direction); //找出對角的方向編號
            Task=600;
            break;
        case 600:                                                               //計算到Tray盤取IC的相對點位
            if(SortingBinTray_CalPos(MyParameter, MySuckData, MyTrayData, &MyCalData))
            {
                Task=650;
            }
            break;
        case 650:                                                               //設定馬達、及點位資料
            SortingBinTray_SetArmZPosData();                                    //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
            Task=700;
            break;
        case 700:                                                               //X、Y移動到取IC的位置
            if(SortingBinTray_MoveToPickPlace(true, false, iTray))              //Ifor 20161222 (jou) Pick模式 Z不下降
            {
                Task=800;
            }
            break;
        case 800:                                                               //Z下降至Pick位置
            if(SortingBinTray_MoveToPickPlace(true, true, iTray))               //Ifor 20161222 (jou) Pick模式 Z下降
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    fMain->Pause("DoPickTrayIC 800");
                    Task=900;//吸嘴Z到安全位置
                }
                else
                {
                    Task=1100;
                }
            }
            break;
        case 900:                                                               //吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                SortingBinTray_SetArmZPosData();                                //重新取得Offset資料   //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
                Task=700;
            }
            break;
        case 1100:                                                              //依計算結果，吸嘴取IC
            bFlag=(OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Suck() ||
                   OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Error);

            if(bFlag==true)
            {
                Task=1200;
            }
            break;
        case 1200:                                                              //等待穩定時間
            Task=1300;
            break;
        case 1300:                                                              //Z軸吸嘴上升
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:                                                              //檢查IC是否有吸起
            if(OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Error==true)
            {
                ErrPart+=OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].sName;
                ret=OutArmPickTrayAlarm(iTray, ErrPart);
                if(IniConfig.bInOutArmCanPushHome)                              //Input pickup device error時,按"retry"鍵,機台都會自動home
                {
                    if(ret==K_RETRY)
                    {
                        SetOutArmHome();
                        Task =650;
                        break;
                    }
                }
                else
                {
                    if(ret==K_RETRY)
                    {
                        Task =650;
                        break;
                    }
                }

                if(ret==K_HOME)                                                 //回Home後重吸
                {
                    SetOutArmHome();
                    Task=650;
                    break;
                }

                if(ret==K_SKIP)
                {
                    if(MyCalData.iUseSuck[0][0]==TRAYSORTING_ACT)
                    {
                        MyCalData.iUseSuck[0][0]=TRAYSORTING_ERR;
                        Task=1600;
                        break;
                    }
                }
            }
            MyCalData.iUseSuck[0][0]=TRAYSORTING_CHECKOK;
            Task=1600;
            break;
        case 1500:                                                              //Show 錯誤訊息，重吸、跳過
            Task=1600;
            break;
        case 1600:                                                              //依真空更新吸嘴資料
            SortingBinTray_DataChange(MyParameter, &MySuckData, &MyTrayData, MyCalData);
            Task=1700;
            break;
        case 1700:                                                              //將SortingBinTray的結構轉換到程式中Tray盤的資料
            SortingBinTray_SortingBinToTrayData_SelectSuck(iTray);              //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
            Task=10000;
            break;
        case 8000:                                                              //取消到Tray取IC
            bRes=true;
            break;
        case 9000:                                                              //到Tray取IC錯誤
            bRes=true;
            break;
        case 10000:                                                             //設定到Tray取IC完成
            bRes=true;
            break;
    }

    if(bRes==true)
        iTask_DoPickFix3IC=1;

    return bRes;
}
//---------------------------------------------------------------------------
//Ifor 20161122 add OutArm To Tray 放IC流程
//==>
bool DoPlaceTrayIC(int iTray)
{
    bool bRes = false;
    bool bFlag = false;
    int ret = 0;
    AnsiString ErrPart=" ";

    int &Task = iTask_DoPlaceFix3IC;
    //Ifor 20170310 (wei) add HT9045S Fix3 Full Tray 真空位置
    //==>
    int iUseSuckNum=0;
    if(USE_PICKER_COUNT==0)
    {
        iUseSuckNum=SLECT_SUCK2;    //HT9045S 使用基準軸
    }
    else
    {
        iUseSuckNum=SLECT_SUCK3;    //HT9045 使用第四支吸嘴
    }
    //<==
    //Ifor 20170310 (wei) add HT9045S Fix3 Full Tray 真空位置
    switch(Task)
    {
        case 1:
            Task = 200;
            break;
        case 200: //吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                Task = 500;
            }
            break;
        case 500: //檢查Tray盤是否有空位
            if(MOT[iMMAuto[iTray]].Tray.FullIC() == false)
            {
                Task = 600;
                break;
            }
            Task = 9000;
            break;
        case 600: //將程式Tray盤的資料轉換到SortingBinTray的結構中
            SortingBinTray_TrayDataToSortingBin_SelectSuck(iTray,HAS_IC); //將Tray盤的資料轉成SortingBinTray的格式    //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
            MyParameter.bIsPickIC = false;
            Task = 650;
            break;
        case 650: //計算到Tray盤放IC的相對點位
            if(SortingBinTray_HAS_NULL_IC_Pos(MyParameter,MySuckData,MyTrayData,&MyCalData))
            {
                SortingBinTray_SetArmZPosData(); //設定馬達、及點位資料   //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
                Task = 700;
            }
            break;
        case 700: //X、Y移動到取IC的位置
            if(SortingBinTray_MoveToPickPlace(false, false, iTray)) //Ifor 20161222 (jou) Place模式 Z不下降
            {
                Task=800;
            }
            break;
        case 800://Z下降至Pick位置
            if(SortingBinTray_MoveToPickPlace(false, true, iTray)) //Ifor 20161222 (jou) Place模式 Z不下降
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    fMain->Pause("DoPlaceTrayIC 800");
                    Task=900;//吸嘴Z到安全位置
                }
                else
                {
                    Task=1200;
                }
            }
            break;
        case 900://吸嘴Z到安全位置
            if(MoveOutArmToAutoSafe())
            {
                SortingBinTray_SetArmZPosData();   //重新取得Offset資料   //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
                Task = 700;
            }
            break;
        case 1200: //依計算結果，吸嘴放IC
            bFlag = (OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Destroy() ||
                     OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Error);
            if(bFlag == true)
            {
                Task = 1300;
            }
            break;
        case 1300:
            Task = 1400;
            break;
        case 1400: //Z軸吸嘴上升
            if(MoveOutArmToAutoSafe())
            {
                Task = 1500;
            }
            break;
        case 1500: //檢查IC是否有放掉
            if(OutArmSuck.Suck[iOutArmYBase][iUseSuckNum].Error == true)
            {
                ErrPart+=OutArmSuck.Suck[iOutArmYBase][iOutArmXBase].sName;
                ret=ShowErrorMessage("JAM0217", K_RETRY, MOutArmX, 0, ErrPart);  //jou 2015-03-27 JAM0203->JAM0217 Device drop error -> Vacuum sensor OFF error
                if(ret==K_RETRY)
                {
                    //重破
                    Task=700;
                    break;
                }
            }
            Task = 1700;
            MyCalData.iUseSuck[0][0] = TRAYSORTING_CHECKOK;
            break;
        case 1700:                                                              //依真空更新吸嘴資料
            SortingBinTray_DataChange(MyParameter, &MySuckData, &MyTrayData, MyCalData);
            Task = 1800;
            break;
        case 1800:                                                              //將SortingBinTray的結構轉換到程式中Tray的資料
            SortingBinTray_SortingBinToTrayData_SelectSuck(iTray);              //將SOrtingBinTray的資料轉成Tray盤的格式   //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
            Task = 10000;
            break;
        case 8000: //取消到Tray放IC
            bRes = true;
            break;
        case 9000: //到Tray放IC錯誤
            bRes = true;
            break;
        case 10000: //設定到Tray放IC完成
            bRes = true;
            break;
    }

    if(bRes==true)
        iTask_DoPlaceFix3IC=1;
    return bRes;
}
//<==
//Ifor 20161122 add OutArm To Tray 放IC流程
//---------------------------------------------------------------------------
//設定馬達、及點位資料
void SortingBinTray_SetMotorPosData()                                           //設定馬達、及點位資料
{
    //X、Y軸的馬達編號
    SortingBinTray_Data.iMotorX = MOutArmX;
    SortingBinTray_Data.iMotorY = MOutArmY;
    SortingBinTray_Data.iMotorZ = MOutArmZE;//基準第三支

    //Z軸的安全高度
    SortingBinTray_Data.iSuckerSafeZPos = Prod.ZOutArmSafe[iOutArmYBase][iOutArmXBase];

    for(int i=0; i<iFixRightHalf; i++)
    {
        if(i<iFixMin)
        {
            //Z軸取IC高度
            SortingBinTray_Data.iSuckerPickZPos[i] = Tech.iOutArmPlaceZ2+OutArmOffSet[OutOfsAuto1+i]->GetPickUp();
            //Z軸放IC高度
            SortingBinTray_Data.iSuckerPlaceZPos[i]= Prod.ZOutArm_Auto_Place[i][iOutArmYBase][iOutArmXBase];

            //Tray盤左上角基準點位
            SortingBinTray_Data.iXBasePos[i] = Prod.XOutArm_Auto_Place[i][iOutArmYBase][iOutArmXBase];
            SortingBinTray_Data.iYBasePos[i] = Prod.YOutArm_Auto_Place[i][iOutArmYBase][iOutArmXBase];
        }
        else
        {
            //Z軸取IC高度
            SortingBinTray_Data.iSuckerPickZPos[i] = Tech.iOutArmPlaceFixZ1+OutArmOffSet[OutOfsFix1+i%3]->GetPickUp();
            //Z軸放IC高度
            SortingBinTray_Data.iSuckerPlaceZPos[i] = Prod.ZPlace[i][iOutArmYBase][iOutArmXBase];

            //Tray盤左上角基準點位
            SortingBinTray_Data.iXBasePos[i] = Prod.XStart[i][iOutArmYBase][iOutArmXBase];
            SortingBinTray_Data.iYBasePos[i] = Prod.YStart[i][iOutArmYBase][iOutArmXBase];
        }
    }
}
//---------------------------------------------------------------------------
void SortingBinTray_SetArmZPosData()                                            //Ifor 20161123 add 設定馬達及點位吸嘴 Function //Ifor 20170118 (Steven) 整合Fix3 Full tray Function add HT-9045S
{
    int iUseSuckNum=0;
    //Ifor 20170118 add (Steven) Fix3 Full Tray Function 支援 HT-9045S
    //==>
    if(USE_PICKER_COUNT==0)
    {
        iUseSuckNum = SLECT_SUCK2;
    }
    else
    {
        iUseSuckNum = SLECT_SUCK3;
    }
    //<==
    //Ifor 20170118 add Fix3 Full Tray Function 支援 HT-9045S

    //X、Y軸的馬達編號
    SortingBinTray_Data.iMotorX = MOutArmX;
    SortingBinTray_Data.iMotorY = MOutArmY;
    SortingBinTray_Data.iMotorZ = MOutArmZG;//基準第三支

    //Ifor 20170118 (Steven) iArmZ -> iUseSuckNum  && TRAY_FIX3 -> iUseSuckNum
    //==>
    //Z軸的安全高度
    SortingBinTray_Data.iSuckerSafeZPos = Prod.ZOutArmSafe[iOutArmYBase][iUseSuckNum];

    //Z軸取IC高度
    SortingBinTray_Data.iSuckerPickZPos[iUseSuckNum] = Prod.ZPick[iFixRight][iOutArmYBase][iUseSuckNum];

    //Z軸放IC高度
    SortingBinTray_Data.iSuckerPlaceZPos[iUseSuckNum] = Prod.ZPlace[iFixRight][iOutArmYBase][iUseSuckNum];

    //Tray盤左上角基準點位
    SortingBinTray_Data.iXBasePos[iUseSuckNum] = Prod.XStart[iFixRight][iOutArmYBase][iUseSuckNum];
    SortingBinTray_Data.iYBasePos[iUseSuckNum] = Prod.YStart[iFixRight][iOutArmYBase][iUseSuckNum];
    //<==
    //Ifor 20170118 iArmZ -> iUseSuckNum  && TRAY_FIX3 -> iUseSuckNum
}
//---------------------------------------------------------------------------
//設定Tray盤資料
void SortingBinTray_SetTraySpecData()                                           //設定Tray盤資料
{
    for(int i=0; i<eTrayCount; i++)
    {
        SortingBinTray_Data.bTrayAction[i] = false;                             //是否使用此Tray盤
    }

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]==tTrayAuto ||
           Prod.iTrayType[i]==tTrayFix)
        {
            //此Tray盤是否需要整盤
            SortingBinTray_Data.bTrayAction[i]=SortingBinTray_IsBinTrayNeedToSorting(AutoForm[i], &MOT[iMMAuto[i]].Tray); //檢查Bin Tray盤的IC是否需要整盤; //是否使用此Tray盤
            if(TrayForm.iFixTrayMode==1 && i>=iFixMin)                          //JerryYang 20151221 Fix Tray選 Up Down,不整盤
            {
                SortingBinTray_Data.bTrayAction[i]=false;
            }

            if(SortingBinTray_Data.bTrayAction[i]==true)
            {
                //到Auto 1~3的點位、偏移量
                SortingBinTray_Data.TTraySpec[i].XStart    =AutoForm[i]->XStart;
                SortingBinTray_Data.TTraySpec[i].YStart    =AutoForm[i]->YStart;
                SortingBinTray_Data.TTraySpec[i].XPitch    =AutoForm[i]->XPitch;
                SortingBinTray_Data.TTraySpec[i].YPitch    =AutoForm[i]->YPitch;
                SortingBinTray_Data.TTraySpec[i].XDivision =AutoForm[i]->XDivision;
                SortingBinTray_Data.TTraySpec[i].YDivision =AutoForm[i]->YDivision;
                SortingBinTray_Data.TTraySpec[i].Direction =AutoForm[i]->Direction;

                for(int ix=0; ix<SortingBinTray_Data.TTraySpec[i].XDivision; ix++)
                {
                    for(int iy=0; iy<SortingBinTray_Data.TTraySpec[i].YDivision; iy++)
                    {
                        //Auto 1~3、Fix 1~3的Tray盤資料
                        SortingBinTray_Data.iTrayData[i][ix][iy]=MOT[iMMAuto[i]].Tray.Data[ix][iy]; //此Tray盤的IC資料
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
//Ifor 20161123 add Set Tray Data Function
//==>
void SortingBinTray_SetTrayData(int iTray)
{
    //重置
    SortingBinTray_Data.bTrayAction[iTray]=false;                               //是否使用此Tray盤

    //此Tray盤是否需要整盤
    if(Prod.iTrayType[iTray]==tTrayAuto ||
       Prod.iTrayType[iTray]==tTrayFix)
    {
        if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_NULL_IC, &iXPlacePos, &iYPlacePos)==true)
        {
            //到Auto 1~3的點位、偏移量
            SortingBinTray_Data.TTraySpec[iTray].XStart     =AutoForm[iTray]->XStart;
            SortingBinTray_Data.TTraySpec[iTray].YStart     =AutoForm[iTray]->YStart;
            SortingBinTray_Data.TTraySpec[iTray].XPitch     =AutoForm[iTray]->XPitch;
            SortingBinTray_Data.TTraySpec[iTray].YPitch     =AutoForm[iTray]->YPitch;
            SortingBinTray_Data.TTraySpec[iTray].XDivision  =AutoForm[iTray]->XDivision;
            SortingBinTray_Data.TTraySpec[iTray].YDivision  =AutoForm[iTray]->YDivision;
            SortingBinTray_Data.TTraySpec[iTray].Direction  =AutoForm[iTray]->Direction;

            for(int ix=0; ix<SortingBinTray_Data.TTraySpec[iTray].XDivision; ix++)
            {
                for(int iy=0; iy<SortingBinTray_Data.TTraySpec[iTray].YDivision; iy++)
                {
                    //Auto 1~3、Fix 1~3的Tray盤資料
                    SortingBinTray_Data.iTrayData[iTray][ix][iy] = MOT[iMMAuto[iTray]].Tray.Data[ix][iy]; //此Tray盤的IC資料
                }
            }
        }
    }
}
//<==
//Ifor 20161123 add Set Tray Data Function
//---------------------------------------------------------------------------
//Out Arm X軸到iPos點位
bool bOutArmAxisXMove(int iPos)                                                 //Out Arm X軸到iPos點位
{
    bool bRes = false;

    if(CheckOutArmZNeedHome()!=-1)                                              //檢查所有的Z軸是否有遮到Home點
    {
        ShowMyMessage("When Out Arm Axis X Move, Sucker Z Axis not All in Home Position!");
        SoftStop=true;
        StopAllMotor();
        return false;
    }

    if(IsTrayArmMoveAvoidOutArmCrash())                                         //JerryYang 2015.09.14 確認TrayArmX 是否在安全位置
    {
        ShowMyMessage("When Out Arm Axis Y Move, Tray Arm not in Safe Position!");
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    bRes = MOT[SortingBinTray_Data.iMotorX].MotorMove(iPos);
    return bRes;
}
//---------------------------------------------------------------------------
//Out Arm Y軸到iPos點位
bool bOutArmAxisYMove(int iPos)                                                 //Out Arm Y軸到iPos點位
{
    bool bRes=false;
    if(CheckOutArmZNeedHome()!=-1)                                              //檢查所有的Z軸是否有遮到Home點
    {
        ShowMyMessage("When Out Arm Axis Y Move, Sucker Z Axis not All in Home Position!");
        SoftStop=true;
        StopAllMotor();
        return false;
    }

    if(IsTrayArmMoveAvoidOutArmCrash())                                         //JerryYang 2015.09.14 確認TrayArmX 是否在安全位置
    {
        ShowMyMessage("When Out Arm Axis Y Move, Tray Arm not in Safe Position!");
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    bRes = MOT[SortingBinTray_Data.iMotorY].MotorMove(iPos);
    return bRes;
}
//---------------------------------------------------------------------------
//將Tray盤的資料轉成SortingBinTray的格式
void SortingBinTray_TrayDataToSortingBin(int iNowActTray)                       //將Tray盤的資料轉成SortingBinTray的格式
{
    MyParameter.bIsPickIC   = true;
    MyParameter.bIsFindMaxInTrayXLine = true;
    MyParameter.iPickSuckX  = 1;
    MyParameter.iPickSuckY  = 0;
    MyParameter.iPickTrayX  = 0;
    MyParameter.iPickTrayY  = 0;
//MyParameter.iICNum = HAS_HOT_IC;                                              //JerryYang 2015.09.15 MARK
    MyParameter.iICNum      = HAS_IC;                                           //JerryYang 2015.09.15 add
    //Sucker Data
    MySuckData.iXPitch      = 0; //只用固定吸嘴
    MySuckData.iYPitch      = 0; //只用固定吸嘴
    MySuckData.iXItem       = 1; //只用一支吸嘴
    MySuckData.iYItem       = 1; //只用一支吸嘴
    MySuckData.iBaseSuckX   = 1;
    MySuckData.iBaseSuckY   = 1;

    MySuckData.iSuckData[0][0] = OutArmSuck.Item[iOutArmYBase][iOutArmXBase];   //JerryYang 20150918 只用一支吸嘴

    //Tray Data
    MyTrayData.iXPitch = SortingBinTray_Data.TTraySpec[iNowActTray].XPitch;
    MyTrayData.iYPitch = SortingBinTray_Data.TTraySpec[iNowActTray].YPitch;
    MyTrayData.iXDivision = SortingBinTray_Data.TTraySpec[iNowActTray].XDivision;
    MyTrayData.iYDivision = SortingBinTray_Data.TTraySpec[iNowActTray].YDivision;
    MyTrayData.iDirection = SortingBinTray_Data.TTraySpec[iNowActTray].Direction;

    for(int ix=0;ix<MyTrayData.iXDivision;ix++)
    {
        for(int iy=0;iy<MyTrayData.iYDivision;iy++)
        {
            MyTrayData.iTrayData[ix][iy] = MOT[iMMAuto[iNowActTray]].Tray.Data[ix][iy];
        }
    }
}
//---------------------------------------------------------------------------
//Ifor 20161123 add 將Tray盤的資料轉SortingBinTray格式 設定使用哪一支吸嘴 & IC Type
//==>
void SortingBinTray_TrayDataToSortingBin_SelectSuck(int iNowActTray, int iICType)    //Ifor 20170116 (Steven) 取消使用吸嘴變數由Function內判斷
{
    MyParameter.bIsPickIC   = true;
    MyParameter.bIsFindMaxInTrayXLine = true;
    MyParameter.iPickSuckX  = 1;
    MyParameter.iPickSuckY  = 0;
    MyParameter.iPickTrayX  = 0;
    MyParameter.iPickTrayY  = 0;
    MyParameter.iICNum      = iICType;                                          //JerryYang 2015.09.15 add
    MySuckData.iXPitch      = 0; //只用固定吸嘴                                 //Ifor 20161123 0->1
    MySuckData.iYPitch      = 0; //只用固定吸嘴
    MySuckData.iXItem       = 1; //只用一支吸嘴
    MySuckData.iYItem       = 1; //只用一支吸嘴
    MySuckData.iBaseSuckX   = 1;
    MySuckData.iBaseSuckY   = 1;

    if(USE_PICKER_COUNT==0)//Ifor 20170116 (Steven) add HT9045S Fix3 Full Tray 判斷
    {
        MySuckData.iSuckData[0][0] = OutArmSuck.Item[iOutArmYBase][SLECT_SUCK2];   //JerryYang 20150918 只用一支吸嘴
    }
    else
    {
        MySuckData.iSuckData[0][0] = OutArmSuck.Item[iOutArmYBase][SLECT_SUCK3];   //JerryYang 20150918 只用一支吸嘴
    }

    //Tray Data
    MyTrayData.iXPitch = SortingBinTray_Data.TTraySpec[iNowActTray].XPitch;
    MyTrayData.iYPitch = SortingBinTray_Data.TTraySpec[iNowActTray].YPitch;
    MyTrayData.iXDivision = SortingBinTray_Data.TTraySpec[iNowActTray].XDivision;
    MyTrayData.iYDivision = SortingBinTray_Data.TTraySpec[iNowActTray].YDivision;
    MyTrayData.iDirection = SortingBinTray_Data.TTraySpec[iNowActTray].Direction;

    for(int ix=0; ix<MyTrayData.iXDivision; ix++)
    {
        for(int iy=0; iy<MyTrayData.iYDivision; iy++)
        {
            MyTrayData.iTrayData[ix][iy] = MOT[iMMAuto[iNowActTray]].Tray.Data[ix][iy];
        }
    }
}
//<==
//Ifor 20161123 add 將Tray盤的資料轉SortingBinTray格式 設定使用哪一支吸嘴 & IC Type
//---------------------------------------------------------------------------
//將SOrtingBinTray的資料轉成Tray盤的格式
void SortingBinTray_SortingBinToTrayData(int iNowActTray)                       //將SOrtingBinTray的資料轉成Tray盤的格式
{
    //Suck Data
    OutArmSuck.SetItemData(iOutArmYBase,iOutArmXBase,MySuckData.iSuckData[0][0]); //JerryYang 20150910 只使用基準軸

    //Tray Data
    for(int ix=0; ix<MyTrayData.iXDivision; ix++)
    {
        for(int iy=0; iy<MyTrayData.iYDivision; iy++)
        {
            MOT[iMMAuto[iNowActTray]].SetTraySingleData(ix,iy,MyTrayData.iTrayData[ix][iy]);
        }
    }
}
//---------------------------------------------------------------------------
//Ifor 20161123 add  將SOrtingBinTray的資料轉成Tray盤的格式 & 選擇吸嘴
//==>
void SortingBinTray_SortingBinToTrayData_SelectSuck(int iNowActTray)            //Ifor 20170116 (Steven) 取消使用吸嘴變數由Function內判斷
{
    //Suck Data
    if(USE_PICKER_COUNT==0)//Ifor 20170116 add HT9045S Fix3 Full Tray 判斷
    {
        OutArmSuck.SetItemData(iOutArmYBase,SLECT_SUCK2,MySuckData.iSuckData[0][0]); //JerryYang 20150910 只使用基準軸
    }
    else
    {
        OutArmSuck.SetItemData(iOutArmYBase,SLECT_SUCK3,MySuckData.iSuckData[0][0]); //JerryYang 20150910 只使用基準軸
    }

    //Tray Data
    for(int ix=0; ix<MyTrayData.iXDivision; ix++)
    {
        for(int iy=0; iy<MyTrayData.iYDivision; iy++)
        {
            MOT[iMMAuto[iNowActTray]].SetTraySingleData(ix,iy,MyTrayData.iTrayData[ix][iy]);
        }
    }
}
//<==
//Ifor 20161123 add  將SOrtingBinTray的資料轉成Tray盤的格式
//---------------------------------------------------------------------------
//檢查Bin Tray盤的IC是否需要整盤
bool SortingBinTray_IsBinTrayNeedToSorting(TRAY_TYPE_PARA *TraySpec, TMyTray *TrayData) //檢查Bin Tray盤的IC是否需要整盤
{
    bool bFindIC = false;
    int iXStart = 0,iYStart = 0;
    int iXEnd = 0,iYEnd = 0;
    int iXStep = 0,iYStep = 0;

    if(TraySpec->Direction == 0)
    {
        iXStart = TraySpec->XDivision - 1;
        iXEnd = 0;
        iXStep = -1;
        iYStart = TraySpec->YDivision - 1;
        iYEnd = 0;
        iYStep = -1;
    }
    else if(TraySpec->Direction == 1)
    {
        iXStart = 0;
        iXEnd = TraySpec->XDivision - 1;
        iXStep = 1;
        iYStart = TraySpec->YDivision - 1;
        iYEnd = 0;
        iYStep = -1;
    }
    else if(TraySpec->Direction == 2)
    {
        iXStart = TraySpec->XDivision - 1;
        iXEnd = 0;
        iXStep = -1;
        iYStart = 0;
        iYEnd = TraySpec->YDivision - 1;
        iYStep = 1;
    }
    else if(TraySpec->Direction == 3)
    {
        iXStart = 0;
        iXEnd = TraySpec->XDivision - 1;
        iXStep = 1;
        iYStart = 0;
        iYEnd = TraySpec->YDivision - 1;
        iYStep = 1;
    }

    int ix,iy;
    iy = iYStart;
    iy = iy - iYStep;

    do
    {
        ix = iXStart;
        ix = ix - iXStep;
        iy = iy + iYStep;
        do
        {
            ix = ix + iXStep;
            if(TrayData->Data[ix][iy] != NULL_IC && TrayData->Data[ix][iy] != HAS_NULL_IC)
            {
                bFindIC = true;
            }

            if(bFindIC == true)
            {
                if(TrayData->Data[ix][iy] == NULL_IC)
                {
                    return true;
                }
            }
        }while(ix != iXEnd);
    }while(iy != iYEnd);

    return false;
}
//---------------------------------------------------------------------------
bool SortingBinTray_CalPos(TSortingBinTray_PAR Parameter,
                           TSortingBinTray_SUCKDATA SuckData,
                           TSortingBinTray_TRAYDATA TrayData,
                           TSortingBinTray_CALDATA *CalData)
{
    int iStart;
    int iEnd;
    int iStep;
    int iCalCanUseSuck = 0;
    int iMaxCanUseSuck = 0;

    int iLineX = Parameter.iPickTrayX;
    int iLineY = Parameter.iPickTrayY;

    int TRAY_NUM = HAS_IC;
    int SUCK_NUM = NULL_IC;

    int iXItem=TrayData.iXDivision;
    if(iXItem%2==0)
        iXItem/=2;
    else
        iXItem=(iXItem-1)/2;

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161208 add Fix3 Full Tray 放料順序 Direction>=4 有機會超出極限導致未滿盤被退盤
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)
    {
        if(AutoForm[iFixRight]->Direction>=4 || iXItem==2)                      //Ifor 20170112 (Steven) 修正 Fix3 滿盤功能 Aa 吸嘴無法放置X=2 導致退盤問題
            iXItem=iXItem-1;
    }

    if(Parameter.bIsPickIC)
    {
        SUCK_NUM = NULL_IC;
        TRAY_NUM = Parameter.iICNum; //HAS_IC;
    }
    else
    {
        SUCK_NUM = Parameter.iICNum; //HAS_IC;
        TRAY_NUM = NULL_IC;
    }

    //吸嘴上都沒IC時的例外處理
    //Tray盤上都沒IC時的例外處理
    for(int fff=0; fff<2; fff++)
    {
        if(CosFunction.bUseFix3FullTray &&                                      //Ifor 20161205 add 支援Fix3 所有取料模式
           FIX3_FULL_PLACE==Fix3K_Uninstall &&                                  //Steven 20250911 : for 硬體 fix3 full 可以關閉
           IniConfig.bE55UseFix3FullTray==true)
        {
            if(TrayData.iDirection==0 || TrayData.iDirection==2 || TrayData.iDirection==4 || TrayData.iDirection==5)
            {
                if(TrayData.iXPitch!=0)
                {
                    iStart = iXItem -1 + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                    iEnd = -1 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                }
                iStep = -1;
            }
            else if(TrayData.iDirection==1 ||TrayData.iDirection==3 || TrayData.iDirection==6 || TrayData.iDirection==7)
            {
                if(TrayData.iXPitch!=0)
                {
                    iStart = 0 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                    iEnd = iXItem + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                }
                iStep = 1;
            }
        }
        else
        {
            if(TrayData.iDirection==0 || TrayData.iDirection==2)
            {
                if(TrayData.iXPitch!=0)
                {
                    iStart = TrayData.iXDivision -1 + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                    iEnd = -1 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                }
                iStep = -1;
            }
            else if(TrayData.iDirection==1 ||TrayData.iDirection==3)
            {
                if(TrayData.iXPitch!=0)
                {
                    iStart = 0 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                    iEnd = TrayData.iXDivision + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                }
                iStep = 1;
            }
        }

        int iLine = iStart;

        while(iLine != iEnd)
        {
            if(Parameter.bIsFindMaxInTrayXLine)
            {
                iLineX = iLine;
                Parameter.iPickTrayY = SortingBinTray_FindTheICLine(Parameter,TrayData);
                iLineY = Parameter.iPickTrayY;
            }
            else
            {
                iLineY = iLine;
                iLineX = Parameter.iPickTrayX;
            }

            iCalCanUseSuck = 0;

            int iCalUseSuck[10][10] = {NULL_IC};
            int iCalUseTray[100][100] = {NULL_IC};

            for(int iSY=0;iSY<SuckData.iYItem;iSY++)
            {
                for(int iSX=0;iSX<SuckData.iXItem;iSX++)
                {
                    iCalUseSuck[iSX][iSY] = SortingBinTray_NOACT;
                    if((Parameter.bIsPickIC==true  && SuckData.iSuckData[iSX][iSY] == SUCK_NUM) ||                              //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Parameter.bIsPickIC==false && (SuckData.iSuckData[iSX][iSY]!=NULL_IC && SuckData.iSuckData[iSX][iSY]!=HAS_NULL_IC) ))
                    {
                        Parameter.iPickSuckY = SortingBinTray_FindTheSuckLine(Parameter,TrayData,SuckData);
                        int iPosXSuck = SuckData.iXPitch*(iSX-Parameter.iPickSuckX);
                        int iPosYSuck = SuckData.iYPitch*(iSY-Parameter.iPickSuckY);

                        double fRemaindX = iPosXSuck % TrayData.iXPitch;
                        double fRemaindY = iPosYSuck % TrayData.iYPitch;

                        int iTX = -1;
                        int iTY = -1;

                        if((fRemaindX == 0) && (fRemaindY == 0))
                        {
                            if(TrayData.iXPitch != 0)
                            {
                                iTX = iLineX + (iPosXSuck / TrayData.iXPitch);
                            }

                            if(TrayData.iYPitch != 0)
                            {
                                iTY = iLineY + (iPosYSuck / TrayData.iYPitch);
                            }

                            if((iTX < 0) || (iTX >= TrayData.iXDivision) ||
                               (iTY < 0) || (iTY >= TrayData.iYDivision))
                            {
                                AnsiString sTemp;
                                if((iTX < 0) || (iTX > TrayData.iXDivision))
                                {
                                    sTemp = "iTX Out of Range" + IntToStr(iTX);
                                }
                                else
                                {
                                    sTemp = "iTY Out of Range" + IntToStr(iTY);
                                }

                                //ShowMessage("OutError!!  " + sTemp);
                                break;
                            }

                            if((Parameter.bIsPickIC==false  &&  TrayData.iTrayData[iTX][iTY] == TRAY_NUM) ||                                     //Jimmychiu 20241226 : 修正特殊IC吸取異常
                               (Parameter.bIsPickIC==true   && (TrayData.iTrayData[iTX][iTY]!=NULL_IC && TrayData.iTrayData[iTX][iTY]!=HAS_NULL_IC) ))
                            {
                                iCalUseSuck[iSX][iSY] = SortingBinTray_ACT;
                                iCalUseTray[iTX][iTY] = iSY*SuckData.iXItem + iSX + 1;
                                iCalCanUseSuck++;
                            }
                        }
                    }
                }
            }

            if(iMaxCanUseSuck <= iCalCanUseSuck)
            {
                iMaxCanUseSuck = iCalCanUseSuck;
                Parameter.iPickTrayX = iLineX;
                Parameter.iPickTrayY = iLineY;
                for(int y=0; y<SuckData.iYItem; y++)
                {
                    for(int x=0; x<SuckData.iXItem; x++)
                    {
                        CalData->iUseSuck[x][y] = iCalUseSuck[x][y];
                    }
                }

                for(int y=0; y<TrayData.iYDivision; y++)
                {
                    for(int x=0;x<TrayData.iXDivision;x++)
                    {
                        CalData->iUseTray[x][y] = iCalUseTray[x][y];
                    }
                }
            }
            iLine = iLine + iStep;
        }

        if(iMaxCanUseSuck >0)
        {
            CalData->iCalXPos = TrayData.iXPitch*(Parameter.iPickTrayX)+SuckData.iXPitch*(SuckData.iBaseSuckX-Parameter.iPickSuckX);
            CalData->iCalYPos =-TrayData.iYPitch*(Parameter.iPickTrayY)+SuckData.iYPitch*(SuckData.iBaseSuckY-Parameter.iPickSuckY);
            return true;
        }
    }

    CalData->iCalXPos = -1;
    CalData->iCalYPos = -1;
    return false;
}
//---------------------------------------------------------------------------
bool SortingBinTray_HAS_NULL_IC_Pos(TSortingBinTray_PAR Parameter,
                                    TSortingBinTray_SUCKDATA SuckData,
                                    TSortingBinTray_TRAYDATA TrayData,
                                    TSortingBinTray_CALDATA *CalData)
{
    int iStart;
    int iEnd;
    int iStep;
    int iCalCanUseSuck = 0;
    int iMaxCanUseSuck = 0;

    int iLineX = Parameter.iPickTrayX;
    int iLineY = Parameter.iPickTrayY;

    int TRAY_NUM = HAS_IC;
    int SUCK_NUM = HAS_NULL_IC;

    if(Parameter.bIsPickIC)
    {
        SUCK_NUM = HAS_NULL_IC;
        TRAY_NUM = Parameter.iICNum; //HAS_IC;
    }
    else
    {
        SUCK_NUM = Parameter.iICNum; //HAS_IC;
        TRAY_NUM = HAS_NULL_IC;
    }

    //吸嘴上都沒IC時的例外處理
    //Tray盤上都沒IC時的例外處理
    for(int fff=0; fff<2; fff++)
    {
        if(TrayData.iDirection==0 || TrayData.iDirection==2 || TrayData.iDirection==4 || TrayData.iDirection==5)
        {
            if(TrayData.iXPitch!=0)
            {
                iStart = TrayData.iXDivision -1 + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                iEnd = -1 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
            }
            iStep = -1;
        }
        else if(TrayData.iDirection==1 ||TrayData.iDirection==3 || TrayData.iDirection==6 || TrayData.iDirection==7)
        {
            if(TrayData.iXPitch!=0)
            {
                iStart = 0 - (SuckData.iXItem - ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
                iEnd = TrayData.iXDivision + (ChangeToFloatNonPcnt((double)(Parameter.iPickSuckX*SuckData.iXPitch), (double)(TrayData.iXPitch)));
            }
            iStep = 1;
        }

        int iLine = iStart;
        while(iLine != iEnd)
        {
            if(Parameter.bIsFindMaxInTrayXLine)
            {
                iLineX = iLine;
                Parameter.iPickTrayY = SortingBinTray_FindTheICLine(Parameter,TrayData);
                iLineY = Parameter.iPickTrayY;
            }
            else
            {
                iLineY = iLine;
                iLineX = Parameter.iPickTrayX;
            }

            iCalCanUseSuck = 0;

            int iCalUseSuck[10][10] = {HAS_NULL_IC};
            int iCalUseTray[100][100] = {HAS_NULL_IC};

            for(int iSY=0;iSY<SuckData.iYItem;iSY++)
            {
                for(int iSX=0;iSX<SuckData.iXItem;iSX++)
                {
                    iCalUseSuck[iSX][iSY] = SortingBinTray_NOACT;
                    if((Parameter.bIsPickIC==true  && SuckData.iSuckData[iSX][iSY] == SUCK_NUM) ||                                         //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Parameter.bIsPickIC==false && (SuckData.iSuckData[iSX][iSY]!=NULL_IC && SuckData.iSuckData[iSX][iSY]!=HAS_NULL_IC) ))
                    {
                        Parameter.iPickSuckY = SortingBinTray_FindSuckICStatusLine(Parameter,TrayData,SuckData,HAS_IC);

                        int iPosXSuck = SuckData.iXPitch*(iSX-Parameter.iPickSuckX);
                        int iPosYSuck = SuckData.iYPitch*(iSY-Parameter.iPickSuckY);

                        double fRemaindX = iPosXSuck % TrayData.iXPitch;
                        double fRemaindY = iPosYSuck % TrayData.iYPitch;

                        int iTX = -1;
                        int iTY = -1;

                        if((fRemaindX == 0) && (fRemaindY == 0))
                        {
                            if(TrayData.iXPitch != 0)
                            {
                                iTX = iLineX + (iPosXSuck / TrayData.iXPitch);
                            }

                            if(TrayData.iYPitch != 0)
                            {
                                iTY = iLineY + (iPosYSuck / TrayData.iYPitch);
                            }

                            if((iTX < 0) || (iTX >= TrayData.iXDivision) ||
                               (iTY < 0) || (iTY >= TrayData.iYDivision))
                            {
                                AnsiString sTemp;
                                if((iTX < 0) || (iTX > TrayData.iXDivision))
                                {
                                    sTemp = "iTX Out of Range" + IntToStr(iTX);
                                }
                                else
                                {
                                    sTemp = "iTY Out of Range" + IntToStr(iTY);
                                }

                                //ShowMessage("OutError!!  " + sTemp);
                                break;
                            }

                            if((Parameter.bIsPickIC==false  &&  TrayData.iTrayData[iTX][iTY] == TRAY_NUM) ||                                //Jimmychiu 20241226 : 修正特殊IC吸取異常
                               (Parameter.bIsPickIC==true   && (TrayData.iTrayData[iTX][iTY]!=NULL_IC && TrayData.iTrayData[iTX][iTY]!=HAS_NULL_IC) ))
                            {
                                iCalUseSuck[iSX][iSY] = SortingBinTray_ACT;
                                iCalUseTray[iTX][iTY] = iSY*SuckData.iXItem + iSX + 1;
                                iCalCanUseSuck++;
                            }
                        }
                    }
                }
            }

            if(iMaxCanUseSuck <= iCalCanUseSuck)
            {
                iMaxCanUseSuck = iCalCanUseSuck;
                Parameter.iPickTrayX = iLineX;
                Parameter.iPickTrayY = iLineY;
                for(int y=0;y<SuckData.iYItem;y++)
                {
                    for(int x=0;x<SuckData.iXItem;x++)
                    {
                        CalData->iUseSuck[x][y] = iCalUseSuck[x][y];
                    }
                }

                for(int y=0;y<TrayData.iYDivision;y++)
                {
                    for(int x=0;x<TrayData.iXDivision;x++)
                    {
                        CalData->iUseTray[x][y] = iCalUseTray[x][y];
                    }
                }
            }
            iLine = iLine + iStep;
        }

        if(iMaxCanUseSuck >0)
        {
            CalData->iCalXPos = TrayData.iXPitch*(Parameter.iPickTrayX)+SuckData.iXPitch*(SuckData.iBaseSuckX-Parameter.iPickSuckX);
            CalData->iCalYPos =-TrayData.iYPitch*(Parameter.iPickTrayY)+SuckData.iYPitch*(SuckData.iBaseSuckY-Parameter.iPickSuckY);
            return true;
        }
    }
    CalData->iCalXPos = -1;
    CalData->iCalYPos = -1;
    return false;
}
//---------------------------------------------------------------------------
void SortingBinTray_DataChange(TSortingBinTray_PAR Parameter,
                               TSortingBinTray_SUCKDATA *SuckData,
                               TSortingBinTray_TRAYDATA *TrayData,
                               TSortingBinTray_CALDATA CalData)
{
    for(int iTY = 0;iTY < TrayData->iYDivision;iTY++)
    {
        for(int iTX = 0;iTX < TrayData->iXDivision;iTX++)
        {
            if(CalData.iUseTray[iTX][iTY]==0)
            {
                continue;
            }

            int iSX = (CalData.iUseTray[iTX][iTY]-1) % SuckData->iXItem;
            int iSY = ChangeToFloatNonPcnt((double)((CalData.iUseTray[iTX][iTY]-1 - iSX)), (double)(SuckData->iXItem));

            if(CalData.iUseSuck[iSX][iSY] == TRAYSORTING_CHECKOK)
            {
                if(Parameter.bIsPickIC)
                {
                    if(SuckData->iSuckData[iSX][iSY] == NULL_IC)
                    {
                        SuckData->iSuckData[iSX][iSY] = TrayData->iTrayData[iTX][iTY];
                        TrayData->iTrayData[iTX][iTY] = NULL_IC;
                    }
                }
                else
                {
                    if(TrayData->iTrayData[iTX][iTY] == NULL_IC)
                    {
                        TrayData->iTrayData[iTX][iTY] = SuckData->iSuckData[iSX][iSY];
                        SuckData->iSuckData[iSX][iSY] = NULL_IC;
                    }

                    if(CosFunction.bUseFix3FullTray &&                          //Ifor 20161205 add HAS_NULL_IC 資料轉換
                       FIX3_FULL_PLACE==Fix3K_Uninstall &&                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
                       IniConfig.bE55UseFix3FullTray==true)
                    {
                        if(TrayData->iTrayData[iTX][iTY] == HAS_NULL_IC)        //Ifor 20161202 add
                        {
                            TrayData->iTrayData[iTX][iTY] = SuckData->iSuckData[iSX][iSY];
                            SuckData->iSuckData[iSX][iSY] = NULL_IC;
                        }
                    }
                }
            }

            if(CalData.iUseSuck[iSX][iSY] == TRAYSORTING_ERR)
            {
                SuckData->iSuckData[iSX][iSY] = NULL_IC;
                TrayData->iTrayData[iTX][iTY] = NULL_IC;
            }
        }
    }

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161208 (Jou) add Fix3 Full Tray 功能開啟後吸取IC後強制設定Fix3，放至IC後強制清除為-1
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)
    {
        for(int iSuckRow=0 ; iSuckRow<MAX_ARM_Row; iSuckRow++)
        {
            for(int iSuckCol=0 ; iSuckCol<MAX_ARM_Col; iSuckCol++)
                OutArmSuck.iWhichAuto[iSuckRow][iSuckCol]=-1;
        }

        if(Parameter.bIsPickIC)
            OutArmSuck.iWhichAuto[0][3]=5;
        else
            OutArmSuck.iWhichAuto[0][3]=-1;
    }
}
//---------------------------------------------------------------------------
int SortingBinTray_FindTheICLine(TSortingBinTray_PAR Par,TSortingBinTray_TRAYDATA TD)
{
    int iTrayIC = NULL_IC;

    if(Par.bIsPickIC)
    {
        iTrayIC = MyParameter.iICNum;
    }

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161205 add 支援Fix3 Full Tray 模式
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)
    {
        if(Par.bIsPickIC)
        {
            if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_IC, &iXPickPos, &iYPickPos) == true)
                return iYPickPos;
        }
        else
        {
            if(SortingBinTray_FindICType(AutoForm[iSortingTrayFix3], &MOT[iMMAuto[iSortingTrayFix3]].Tray, HAS_NULL_IC, &iXPlacePos, &iYPlacePos) == true)
                return iYPlacePos;
        }
    }
    else
    {
        if((TD.iDirection == 0) || (TD.iDirection == 1))
        {
            for(int iy=0; iy<TD.iYDivision; iy++)
            {
                for(int ix=0; ix<TD.iXDivision; ix++)
                {
                    if((Par.bIsPickIC==false  &&  TD.iTrayData[ix][iy] == iTrayIC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Par.bIsPickIC==true   && (TD.iTrayData[ix][iy]!=NULL_IC && TD.iTrayData[ix][iy]!=HAS_NULL_IC) ))
                    {
                        return iy;
                    }
                }
            }
        }

        if((TD.iDirection==2) || (TD.iDirection==3))
        {
            for(int iy=TD.iYDivision-1; iy>=0; iy--)
            {
                for(int ix=0; ix<TD.iXDivision; ix++)
                {
                    if((Par.bIsPickIC==false  &&  TD.iTrayData[ix][iy] == iTrayIC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Par.bIsPickIC==true   && (TD.iTrayData[ix][iy]!=NULL_IC && TD.iTrayData[ix][iy]!=HAS_NULL_IC) ))
                    {
                        return iy;
                    }
                }
            }
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
//Ifor 20161125 add Find Tray IC Type Position 尋找IC Type 的 X & Y 位置
//==>
bool SortingBinTray_FindICType(TRAY_TYPE_PARA *TraySpec, TMyTray *TrayData, int iICType, int *iXPos, int *iYPos)
{
    int iXStart=0, iYStart=0;
    int iXEnd  =0, iYEnd  =0;
    int iXStep =0, iYStep =0;
    int ix, iy, iXItem;

    iXItem=TraySpec->XDivision;
    if(iXItem%2==0)
        iXItem/=2;
    else
        iXItem=(iXItem-1)/2;

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161208 add Fix3 Full Tray 放料順序 Direction>=4 有機會超出極限導致未滿盤被退盤
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)
    {
        if(AutoForm[iFixRight]->Direction>=4 || iXItem==2)                      //Ifor 20170112 (Steven) 修正 Fix3 滿盤功能 Aa 吸嘴無法放置X=2 導致退盤問題
            iXItem=iXItem-1;
    }

    if(TraySpec->Direction>=0 && TraySpec->Direction<=3)
    {
        if(TraySpec->Direction==0)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = iXItem - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = 0;
                iXEnd = TraySpec->XDivision - 1;
                iXStep = 1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
        }
        else if(TraySpec->Direction == 1)
        {
            if(iICType == HAS_IC)                                               //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = 0;
                iXEnd = iXItem - 1;
                iXStep = 1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = TraySpec->XDivision - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
        }
        else if(TraySpec->Direction == 2)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = iXItem - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = 0;
                iXEnd = TraySpec->XDivision - 1;
                iXStep = 1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
        }
        else if(TraySpec->Direction == 3)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = 0;
                iXEnd = iXItem - 1;
                iXStep = 1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = TraySpec->XDivision - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
        }

        iy = iYStart;
        iy = iy - iYStep;
        do
        {
            ix = iXStart;
            ix = ix - iXStep;
            iy = iy + iYStep;
            do
            {
                ix = ix + iXStep;
                if(TrayData->Data[ix][iy]==iICType)
                {
                    *iXPos = ix;
                    *iYPos = iy;
                    return true;
                }
            }while(ix != iXEnd);
        }while(iy != iYEnd);
    }
    else if(TraySpec->Direction>=4 && TraySpec->Direction<=7)
    {
        if(TraySpec->Direction == 4)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = iXItem - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = 0;
                iXEnd = TraySpec->XDivision - 1;
                iXStep = 1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
        }
        else if(TraySpec->Direction == 5)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = iXItem - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = 0;
                iXEnd = TraySpec->XDivision - 1;
                iXStep = 1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
        }
        else if(TraySpec->Direction == 6)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = 0;
                iXEnd = iXItem - 1;
                iXStep = 1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = TraySpec->XDivision - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
        }
        else if(TraySpec->Direction == 7)
        {
            if(iICType==HAS_IC)                                                 //Ifor 20161125 add 找尋Fix3 上 HAS_IC 位置
            {
                iXStart = 0;
                iXEnd = iXItem - 1;
                iXStep = 1;
                iYStart = 0;
                iYEnd = TraySpec->YDivision - 1;
                iYStep = 1;
            }
            else if(iICType==HAS_NULL_IC)                                       //Ifor 20161125 add 找尋Fix3 上 HAS_NULL_IC 位置
            {
                iXStart = TraySpec->XDivision - 1;
                iXEnd = 0;
                iXStep = -1;
                iYStart = TraySpec->YDivision - 1;
                iYEnd = 0;
                iYStep = -1;
            }
        }

        ix = iXStart;
        ix = ix - iXStep;
        do
        {
            iy = iYStart;
            iy = iy - iYStep;
            ix = ix + iXStep;
            do
            {
                iy = iy + iYStep;
                if(TrayData->Data[ix][iy] == iICType)
                {
                    *iXPos = ix;
                    *iYPos = iy;
                    return true;
                }
            }while(iy != iYEnd);
        }while(ix != iXEnd);
    }

    return false;
}
//<==
//Ifor 20161125 add Find Tray IC Type Position 尋找IC Type 的 X & Y 位置
//---------------------------------------------------------------------------
int SortingBinTray_FindTheSuckLine(TSortingBinTray_PAR Par, TSortingBinTray_TRAYDATA TD, TSortingBinTray_SUCKDATA SD)
{
    int iSuckIC = MyParameter.iICNum;
    if(Par.bIsPickIC)
    {
        iSuckIC = NULL_IC;
    }

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161207 add Find 各模式 Suck Fix3 IC Line
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)
    {
        if((TD.iDirection == 0) || (TD.iDirection == 1) || (TD.iDirection == 4) || (TD.iDirection == 6))
        {
            for(int iy=0; iy<SD.iYItem; iy++)
            {
                for(int ix=0; ix<SD.iXItem; ix++)
                {
                    if((Par.bIsPickIC==true  &&  SD.iSuckData[ix][iy]==NULL_IC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Par.bIsPickIC==false && (SD.iSuckData[ix][iy]!=NULL_IC && SD.iSuckData[ix][iy]!=HAS_NULL_IC)))
                    {
                        return iy;
                    }
                }
            }
        }

        if((TD.iDirection == 2) || (TD.iDirection == 3) || (TD.iDirection == 5) || (TD.iDirection == 7))
        {
            for(int iy=SD.iYItem-1; iy>=0; iy--)
            {
                for(int ix=0; ix<SD.iXItem; ix++)
                {
                    if((Par.bIsPickIC==true  &&  SD.iSuckData[ix][iy]==NULL_IC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                       (Par.bIsPickIC==false && (SD.iSuckData[ix][iy]!=NULL_IC && SD.iSuckData[ix][iy]!=HAS_NULL_IC)))
                    {
                        return iy;
                    }
                }
            }
        }
    }
    else
    {
        if((TD.iDirection==0) || (TD.iDirection==1))
        {
            for(int iy=0; iy<SD.iYItem; iy++)
            {
                for(int ix=0; ix<SD.iXItem; ix++)
                {
                    if(SD.iSuckData[ix][iy]==iSuckIC)
                    {
                        return iy;
                    }
                }
            }
        }

        if((TD.iDirection==2) || (TD.iDirection==3))
        {
            for(int iy=SD.iYItem-1; iy>=0; iy--)
            {
                for(int ix=0; ix<SD.iXItem; ix++)
                {
                    if(SD.iSuckData[ix][iy]==iSuckIC)
                    {
                        return iy;
                    }
                }
            }
        }
    }

    return -1;
}
//---------------------------------------------------------------------------
//Ifor 20161123 add Find Suck IC Status Line
//==>
int SortingBinTray_FindSuckICStatusLine(TSortingBinTray_PAR Par, TSortingBinTray_TRAYDATA TD, TSortingBinTray_SUCKDATA SD, int iStatus)
{
    if(Par.bIsPickIC)
    {
        iStatus = MyParameter.iICNum;
    }

    if((TD.iDirection == 0) || (TD.iDirection == 1))
    {
        for(int iy = 0;iy < SD.iYItem;iy++)
        {
            for(int ix = 0;ix < SD.iXItem;ix++)
            {
                if((Par.bIsPickIC==false &&  SD.iSuckData[ix][iy] == NULL_IC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                   (Par.bIsPickIC==true  && (SD.iSuckData[ix][iy]!=NULL_IC && SD.iSuckData[ix][iy]!=HAS_NULL_IC) ))
                {
                    return iy;
                }
            }
        }
    }

    if((TD.iDirection == 2) || (TD.iDirection == 3))
    {
        for(int iy=SD.iYItem-1; iy>=0; iy--)
        {
            for(int ix=0; ix<SD.iXItem; ix++)
            {
                if((Par.bIsPickIC==false &&  SD.iSuckData[ix][iy] == NULL_IC) ||                               //Jimmychiu 20241226 : 修正特殊IC吸取異常
                   (Par.bIsPickIC==true  && (SD.iSuckData[ix][iy]!=NULL_IC && SD.iSuckData[ix][iy]!=HAS_NULL_IC) ))
                {
                    return iy;
                }
            }
        }
    }
    return -1;
}
//<;==
//Ifor 20161123 add Find Suck IC Status Line
//---------------------------------------------------------------------------
//找出對角的方向編號
int OppositeDirection(int iDir)                                                 //找出對角的方向編號
{
    int iOppositeDir = -1;

    if(CosFunction.bUseFix3FullTray &&                                          //Ifor 20161122 add Fix3 Full Tray 流程
       FIX3_FULL_PLACE==Fix3K_Uninstall &&                                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
       IniConfig.bE55UseFix3FullTray==true)                                     //Ifor 20161122 add 判斷是否有開啟 Fix3 Full Tray 功能
    {
    }
    else
    {
        if(iDir<0 || iDir>3)
        {
            return iOppositeDir;
        }
    }

    if(iDir==0 || iDir==4)                                                      //Ifor 20161206 add Fix3 Tray 取料方向新增
    {
        iOppositeDir = 3;
    }
    else if(iDir == 1 || iDir == 6)
    {
        iOppositeDir = 2;
    }
    else if(iDir == 2 || iDir == 5)
    {
        iOppositeDir = 1;
    }
    else if(iDir == 3 || iDir == 7)
    {
        iOppositeDir = 0;
    }

    return iOppositeDir;
}
//---------------------------------------------------------------------------
int OutArmPickTrayAlarm(int iwhichTray, AnsiString ErrPart)                     //JerryYang 20151013 整盤OutArm吸取異常
{
    int ret;
    if(IniConfig.iE50_OutArmPickUpErrorOption==0)                               //JerryYang 20210813 : RETRY+SKIP
    {
        if(IniConfig.bInOutArmCanPushHome)
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_RETRY|K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
        }
        else
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_RETRY|K_SKIP, MOutArmX, 0, ErrPart);
        }
    }
    else if(IniConfig.iE50_OutArmPickUpErrorOption==1)                          //JerryYang 20210813 : RETRY  //Steven 20141121 : OutArm吸取異常只能Retry
    {
        if(IniConfig.bInOutArmCanPushHome)
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_RETRY|K_HOME, MOutArmX, 0, ErrPart);
        }
        else
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_RETRY, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_RETRY, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_RETRY, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_RETRY, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_RETRY, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_RETRY, MOutArmX, 0, ErrPart);
        }
    }
    else                                                                        //JerryYang 20210813 : SKIP
    {
        if(IniConfig.bInOutArmCanPushHome)
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_SKIP|K_HOME, MOutArmX, 0, ErrPart);
        }
        else
        {
            if(iwhichTray==0)
                ret=ShowErrorMessage("JAM0211", K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==1)
                ret=ShowErrorMessage("JAM0212", K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==2)
                ret=ShowErrorMessage("JAM0213", K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==3)
                ret=ShowErrorMessage("JAM0214", K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==4)
                ret=ShowErrorMessage("JAM0215", K_SKIP, MOutArmX, 0, ErrPart);
            else if(iwhichTray==5)
                ret=ShowErrorMessage("JAM0216", K_SKIP, MOutArmX, 0, ErrPart);
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
bool SortingBinTray_MoveToPickPlace(bool bPick, bool ZNeedDown, int iTray)      //Ifor 20161222 (jou) add 整合 Fix3 Full Tray Move Function
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =iXpitchMaxX3;
    int iMovePitchY =6000;
    int iOffsetPos  =-1;
    int iUseSuckNum =0;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170117 (Steven) Fix3 Full Tray Move Function新增 HT-9045S模式
    {
        if(ZNeedDown==true)
        {
            bZFlag[0][2]=true;
        }
        iUseSuckNum=SLECT_SUCK2;
        if(bPick==true)
            iZPos[0][2]=SortingBinTray_Data.iSuckerPickZPos[iUseSuckNum];       //使用第三隻吸嘴取料
        else
            iZPos[0][2]=SortingBinTray_Data.iSuckerPlaceZPos[iUseSuckNum];      //使用第三隻吸嘴放料
    }
    else
    {
        if(ZNeedDown==true)
        {
            bZFlag[0][3]=true;
        }
        iUseSuckNum=SLECT_SUCK3;
        if(bPick==true)
            iZPos[0][3]=SortingBinTray_Data.iSuckerPickZPos[iUseSuckNum];       //使用第四隻吸嘴取料
        else
            iZPos[0][3]=SortingBinTray_Data.iSuckerPlaceZPos[iUseSuckNum];      //使用第四隻吸嘴放料
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170117 (Steven) Fix3 Full Tray Move Function新增 HT-9045S模式
        iXPos=SortingBinTray_Data.iXBasePos[iUseSuckNum]+MyCalData.iCalXPos;
    else
        iXPos=SortingBinTray_Data.iXBasePos[iUseSuckNum]+MyCalData.iCalXPos-4000;
    iYPos=SortingBinTray_Data.iYBasePos[iUseSuckNum]+MyCalData.iCalYPos;

    TransferAutoRatio(iTray, &iXPos, &iYPos);                                   //Auto的軟體齒輪比

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//<==
//Ifor 20161222 (jou) add 整合 Fix3 Full Tray Move Function
//---------------------------------------------------------------------------
void SaveProductionRecord(TMyTray *TrayData, AnsiString Name)                   //Steven 20200330 : production log by unloader tray存檔
{
    AnsiString asStr, asPath, asFullName, asFileName="", Str;
    AnsiString asLogPath="D:\\HT9045_Log\\UnloadTrayLog";
    TStringList *SList, *tempList;
    int iCount=0;
    bool bHasFile=false;

    if(CosFunction.bSaveProductionLogByUnloaderTray==false)                     //Steven 20200330 : production log by unloader tray存檔
        return;

    if(TrayData->HasRealIC()==false)                                            //Steven 20200528 : 沒IC的Tray Data不用上傳
        return;

    asPath.sprintf("%s\\%04d%02d\\", asLogPath, SystemYear, SystemMonth);
    MyForceDirectories(asPath);

    if(fLotInfo!=NULL)
        asFileName.sprintf("%s_%s_%04d%02d%02d_%02d%02d%02d_%s.csv", PC_NAME, fLotInfo->edtSysLotID->Text, SystemYear, SystemMonth, SystemDate,  SystemHour, SystemMin, SystemSec, Name);     //Steven 20170123 (Jou) : 修改檔案命名格式
    else
        asFileName.sprintf("%s_%04d%02d%02d_%02d%02d%02d_%s.csv", PC_NAME, SystemYear, SystemMonth, SystemDate,  SystemHour, SystemMin, SystemSec, Name);     //Steven 20170123 (Jou) : 修改檔案命名格式
    asFullName=asPath+asFileName;

    SList=new TStringList();                                                    //Steven 20170518 (jou) : 換位置改善UPH
    tempList=new TStringList();
    SList->Clear();
    tempList->Clear();

    bHasFile=FileExists(asFullName);
    if(bHasFile==false)
    {
        Str=StringReplace(TrayData->PordRec[0][0]->asDataTitle, "Schedule name, ", "Schedule name, RETRY, ", TReplaceFlags()<<rfReplaceAll); //Steven 20200715 : Murata要求加上RETRY    //Steven 20221005 : Production Log減少記憶體使用量
        SList->Add(Str);
    }

    for(int j=0; j<TrayData->YItem; j++)
    {
        for(int i=0; i<TrayData->XItem; i++)
        {
            if(TrayData->Data[i][j]!=NULL_IC && TrayData->Data[i][j]!=HAS_NULL_IC)
            {
                tempList->Clear();
                tempList->CommaText=TrayData->PordRec[i][j]->asBuffer->CommaText;   //Steven 20221005 : Production Log減少記憶體使用量
                tempList->Insert(1, fLotInfo->edPage->Text);
                SList->Add(tempList->CommaText);
                iCount++;
            }
        }
    }

    if(iCount>0)                                                                //Steven 20200617 : 確保有IC才存檔與上傳
    {
        if(IniConfig.bN10_9_UploadUnloadTrayToFTP==true &&
           (fLotInfo->iXMLOnLineStatus==0 ||
            fLotInfo->iXMLOnLineStatus==1))                                     //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
        {
            SList->SaveToFile(asFullName);
            if(fMain->UnloadTrayLog->Count==0)                                  //Steven 20200409 : production log by unloader tray存檔並上傳FTP
                fMain->UnloadTrayLog->Clear();
            fMain->UnloadTrayLog->Add(asPath+AnsiString(",")+asFileName);
        }
    }
    SList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    tempList->Clear();
    delete SList;                                                               //Steven 20170518 (jou) : 換位置改善UPH
    delete tempList;
}
//---------------------------------------------------------------------------
void SaveTrayRecord(int iAuto)                                                  //frank 20200814 : 每10盤記錄一次summary log
{
    if(CUSTOMER_CODE!=CC_TSI)
        return;

    FILE * pFile;
    AnsiString asStr, asPath, asFileName="", asLotID, asTesterID;
    AnsiString tmps, str1;
    AnsiString sBinData[10];
    GetTimeInfo();
    asLotID=fLotInfo->edtSysLotID->Text;
    asTesterID=fLotInfo->edtSysOperatorID->Text;
    asPath="D:\\HT9045_Log\\TrayLog\\";

    if(iAuto<10)
    {
        LastSet.TrayCount[iAuto]++;
        str1.sprintf("%s_%s_Auto%d_%02d_%04d%02d%02d%02d%02d%02d.ini", PC_NAME, asLotID, iAuto+1, LastSet.TrayCount[iAuto], SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    else
    {
        LastSet.TrayCount[0]++;
        str1.sprintf("%s_%s_Auto1_%02d_%04d%02d%02d%02d%02d%02d.ini", PC_NAME, asLotID, LastSet.TrayCount[0], SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }

    MyForceDirectories(asPath);
    asFileName=asPath+str1;

    pFile=fopen(asFileName.c_str(), "a+");
    if(pFile!=NULL)
    {
        if(iAuto>=0 && iAuto<=iAutoRight)
        {
            tmps.sprintf("Auto%d 滿Tray紀錄\n", iAuto+1);
            fputs(tmps.c_str(), pFile);
        }
        else if(iAuto==100)
        {
            tmps.sprintf("手動Save Log\n");
            fputs(tmps.c_str(), pFile);
        }

        tmps.sprintf("Auto%d Tray\n", (iAuto+1));
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Tray Count= %d\n", MOT[iMMAuto[iAuto]].Tray.XItem*MOT[iMMAuto[iAuto]].Tray.YItem);
        fputs(tmps.c_str(), pFile);

        tmps.sprintf("PC Name= %s\n", PC_NAME);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Lot ID= %s\n",asLotID);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Tester ID= %s\n",asTesterID);
        fputs(tmps.c_str(), pFile);
        fputs("\n", pFile);
        fputs("當下Bin數量\n", pFile);

        for(int i=0; i<iTestBinCount; i++)
        {
            if(Prod.iT6CatData[i]>=0 &&                                         //Steven 20230929 : 加上保護
               Prod.iT6CatData[i]<eTrayCount)
            {
                if(sBinData[Prod.iT6CatData[i]]=="")
                    sBinData[Prod.iT6CatData[i]]="HW Bin:"+AnsiString(i);
                else
                    sBinData[Prod.iT6CatData[i]]+=","+AnsiString(i);
            }
        }
        sBinData[BinSelect[iTestRunMode].IfErrorT3]+=",E";

        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                tmps.sprintf("%s(%s),Now=%d\n", s3TrayName[iTo3Unload[i]], sBinData[i], LastSet.BinCT[0][iTo3Unload[i]]);
                fputs(tmps.c_str(), pFile);
            }
        }
        fputs("\n", pFile);
        fclose(pFile);
    }
}
//---------------------------------------------------------------------------
