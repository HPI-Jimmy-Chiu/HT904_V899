#include "MachineDefine.h"
#pragma hdrstop

#include "aRotateKIT_Out.h"

#include "aRotateKIT.h"

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
//---------------------------------------------------------------------------

#pragma package(smart_init)

int iOutArmRotateKit        =1;
int iOutRotateFinish        =0;                                                 //kevin 20130524 確保IC有放到旋轉馬達轉向  1:初始化  2:旋轉完成  3:放置SHUTTLE 0:放完SHUTTLE    //Steven 20160809 : 改成全域變數
int iShakeOutArmRotateKit   =1;                                                 //JerryYang 20160803
int iOutArmShakeCnt         =0;                                                 //JerryYang 20160803 旋轉次數計數
int iOutRotateWhichKit      =0;                                                 //kevin 20130629 使用4個rotate 0:表示放第一次 Aa Ab Ae Ae  1:表示放第二次 Ac Ad Ag Ah
int OutAngle90              =0;                                                 //90度等於多少pulse
int OutAngle45              =0;                                                 //45度等於多少pulse   //kevin 20130710 旋轉45 度 add
//---------------------------------------------------------------------------
void InitialOutArmRotateKIT()
{
    iOutArmRotateKit=1;
}
//---------------------------------------------------------------------------
void InitialShakeOutArmRotateKIT()                                              //JerryYang 20160803 初始化Rotate旋轉功能
{
    iShakeOutArmRotateKit=1;
    iOutArmShakeCnt=0;
}
//---------------------------------------------------------------------------
bool MoveOutArmXY_ToRotateKIT(int iKit)
{
    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffOutArm)
        {
            return false;
        }
    }

    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =iRotateKIT_Pitch_X_A*3/2;;
    int iMovePitchY =6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(iKit==1)                                                                 // Z2 & Z4
    {
        iXPos=Prod.iOutArmRotateToUnloaderX+iRotateKIT_Start_X_A+iRotateKIT_Pitch_X_A/2;
        if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)        //ChungHung 20150528 add for 海思 _8Site1x4
            iYPos=Prod.iOutArmRotateToUnloaderY-iRotateKIT_Start_Y_A-iRotateKIT_Pitch_Y_A;
        else
            iYPos=Prod.iOutArmRotateToUnloaderY-iRotateKIT_Start_Y_A;
    }
    else // iKit = 0                                                            // Z1 & Z3(F)
    {
        iXPos   =Prod.iOutArmRotateToUnloaderX+iRotateKIT_Start_X_A+iRotateKIT_Pitch_X_A;
        iYPos   =Prod.iOutArmRotateToUnloaderY-iRotateKIT_Start_Y_A;
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
    {
        if(iWhichAuto>=0 && iWhichAuto<iAutoCnt)
        {
            iOffsetPos=OutOfsAuto1;
        }
    }
    else
    {
        iOffsetPos=OutOfsAuto1+iRotate_Out_Tray6;
    }

    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool MoveOutArmZ_ToRotateKIT_Place(int iKit)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=iKit; j<MAX_ARM_Col; j+=2)                                    //(0, 2) & (1, 3)
        {
            if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
            {
                OutArmSuck.Suck[i][j].SetNeedDestroy(true);
                flag[i][j]=MOT[OutArmZIndex[i][j]].MotorMove(Prod.iOutArmRotateToUnloader_PlaceZ[i][j]);
            }
            else
            {
                flag[i][j]=true;
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(flag[i][j]==false)
                return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool MoveOutArmZ_ToRotateKIT_Pick(int iKit)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};
    int  j2;

    if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)            //ChungHung 20150528 add for 海思 _8Site1x4
    {
        int i=iKit;
        for(int j=0; j<MAX_ARM_Col/2; j++)                                      //(0, 2) & (1, 3)
        {
            j2=j*2+iKit;                                                        //Kit(0, 1) -> Suck(0, 2) or Suck(1, 3)
            if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC)
            {
                OutArmSuck.Suck[0][j2].SetNeedSuck(true);
                flag[0][j2]=MOT[OutArmZIndex[0][j2]].MotorMove(Prod.iOutArmRotateToUnloader_PickZ[0][j2]);
            }
            else
            {
                flag[0][j2]=true;
            }
        }
    }
    else
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col/2; j++)                                  //(0, 2) & (1, 3)
            {                                                                   //Kit(0, 1) -> Suck(0, 2) or Suck(1, 3)
                if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC)
                {
                    OutArmSuck.Suck[i][j].SetNeedSuck(true);
                    flag[i][j]=MOT[OutArmZIndex[i][j]].MotorMove(Prod.iOutArmRotateToUnloader_PlaceZ[i][j]);
                }
                else
                {
                    flag[i][j]=true;
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(flag[i][j]==false)
                return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool DoOutArmRotateKIT_Cylinder()
{
    int &Task=iOutArmRotateKit;
    int j2;

    static int iRetryCT=0;
    static int iKit=0;

    int ret;
    bool bHasError=false, flag=true;
    bool bHasDuplicateErr=false;
    static bool bSuckDuplicateErr[2][4]={{false, false, false, false}, {false, false, false, false}};  //Steven 20091218 : Avoid duplicate message
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            //RotateKIT_FormSet(bool bInput,int iStartX, int iStartY, int iPitchX, int iPitchY)
            RotateKIT_FormSet(        false,       2800,        7000,        8000,        6000);
            iKit=0;
            Task=100;
        case 100:
            if(TrayForm.iRotateKIT_OutputType==1)            // +90
            {
                Cylinder[C_OutputRotateKIT].Off();
            }
            else if(TrayForm.iRotateKIT_OutputType==2)       // -90
            {
                Cylinder[C_OutputRotateKIT].On();
            }
            else // if(TrayForm.iRotateKIT_OutputType==0)
            {
                 return true;
            }
            MOT[MOutRotateKit].SetTray(NULL_IC, __FUNC__);
            ZeroMemory(bSuckDuplicateErr, sizeof(bSuckDuplicateErr));
            Task=1000;
        case 1000:
            if((OutArmSuck.Item[0][0+iKit]!=NULL_IC && OutArmSuck.Item[0][0+iKit]!=HAS_NULL_IC) ||
               (OutArmSuck.Item[0][2+iKit]!=NULL_IC && OutArmSuck.Item[0][2+iKit]!=HAS_NULL_IC) ||
               (OutArmSuck.Item[1][0+iKit]!=NULL_IC && OutArmSuck.Item[1][0+iKit]!=HAS_NULL_IC) ||
               (OutArmSuck.Item[1][2+iKit]!=NULL_IC && OutArmSuck.Item[1][2+iKit]!=HAS_NULL_IC))
            {
                Task=1100;
            }
            else
            {
                Task=2000;
                break;
            }
        case 1100:
            if(MoveOutArmXY_ToRotateKIT(iKit)==true)
            {
                CheckOutArmSuckICFallDown();
                if((TrayForm.iRotateKIT_OutputType==1 && Cylinder[C_OutputRotateKIT].Pop() ==true) ||  // +90
                   (TrayForm.iRotateKIT_OutputType==2 && Cylinder[C_OutputRotateKIT].Push()==true))    // -90
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            OutArmSuck.Suck[i][j].SetNeedDestroy(false);
                        }
                    }
                    bOut_XYMoveFinishOnRotationKit=true;                        //Sam 20240726 : 修正移動完 XY 準備放 IC 到 RotationKit 時 Home
                    Task=1200;
                }
                else
                {
                    break;
                }
            }
            else
            {
                CheckOutArmSuckICFallDown();
                break;
            }
        case 1200:
            if(bOut_XYMoveFinishOnRotationKit==false)                           //Sam 20240726 : 修正移動完 XY 準備放 IC 到 RotationKit 時 Home
                Task=1200;

            if(MoveOutArmZ_ToRotateKIT_Place(iKit)==true)
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoOutArmRotateKIT_Cylinder 1200");
                    Task=1500;
                    break;
                }
                Task=1300;
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
            }
            else
            {
                break;
            }
        case 1300:
            flag=true;
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    j2=j*2+iKit;
                    if(OutArmSuck.Item[i][j2] && OutArmSuck.Suck[i][j2].GetNeedDestroyStatus() || OutArmSuck.Item[i][j2]==HAS_NULL_IC)
                    {
                        if(OutArmSuck.Item[i][j2]==HAS_NULL_IC || OutArmSuck.Suck[i][j2].Destroy())
                        {
                            bSuckDuplicateErr[i][j2]=false;
                            bInArmCheckDestroyACT[i][j2]=true;                  //jou 981130 確認device確實destroy完成
                            if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
                            {
                                MOT[MOutRotateKit].SetTraySingleData(j, i+iKit, OutArmSuck.Item[i][j2]);
                            }
                            else
                            {
                                MOT[MOutRotateKit].SetTraySingleData(j, i, OutArmSuck.Item[i][j2]);
                            }
                            OutArmSuck.SetItemData(i, j2, NULL_IC);
                        }
                        else if(OutArmSuck.Suck[i][j2].Error==false)
                        {
                            flag=false;
                        }

                        if(OutArmSuck.Suck[i][j2].Error)
                        {
                            bHasError=true;
                        }
                    }
                }
            }

            if(flag==false)
                break;

            for(int i=0; i<MAX_ARM_Row; i++)
                for(int j=0; j<MAX_ARM_Col; j++)
                    if(OutArmSuck.Item[i][j] && OutArmSuck.Suck[i][j].GetNeedDestroyStatus())
                        return false;

            if(bHasError)
            {
                Task=1600;
                break;
            }

            ZeroMemory(bSuckDuplicateErr, sizeof(bSuckDuplicateErr));
            Task=2000;
            break;
        case 2000:
            if(MoveOutArmToAutoSafe())
            {
                if(iKit==0 && MOT[MOutRotateKit].FullIC()==false)
                {
                    iKit++;
                    Task=1000;
                    break;
                }
                else
                {
                    if(OutArmSuck.Item[0][0]==NULL_IC && OutArmSuck.Item[0][2]==NULL_IC &&
                       OutArmSuck.Item[1][0]==NULL_IC && OutArmSuck.Item[1][2]==NULL_IC)
                        iKit=0;
                    Task=3000;
                }
            }
            else
            {
                break;
            }
        case 3000:
            if(TrayForm.iRotateKIT_OutputType==1)            // +90
            {
                Cylinder[C_OutputRotateKIT].On();
                Task=3100;
            }
            else if(TrayForm.iRotateKIT_OutputType==2)       // -90
            {
                Cylinder[C_OutputRotateKIT].Off();
            }

            Task=3100;
            break;
        case 3100:  //吸取IC
            if((MOT[MOutRotateKit].Tray.Data[0][0]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[0][0]!=HAS_NULL_IC) ||
               (MOT[MOutRotateKit].Tray.Data[1][0]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[1][0]!=HAS_NULL_IC) ||
               (MOT[MOutRotateKit].Tray.Data[0][1]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[0][1]!=HAS_NULL_IC) ||
               (MOT[MOutRotateKit].Tray.Data[1][1]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[1][1]!=HAS_NULL_IC))
            {
                Task=4000;
            }
            else
            {
                Task=6000;
                break;
            }
        case 4000:
            if(MoveOutArmXY_ToRotateKIT(iKit)==true)
            {
                CheckOutArmSuckICFallDown();                                    //Steven 20110516 : 修改成整合式Alarm
                if((TrayForm.iRotateKIT_OutputType==1 && Cylinder[C_OutputRotateKIT].Push()==true) ||         // +90
                   (TrayForm.iRotateKIT_OutputType==2 && Cylinder[C_OutputRotateKIT].Pop() ==true))           // -90
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            OutArmSuck.Suck[i][j].SetNeedSuck(false);
                        }
                    }
                    Task=4100;
                }
                else
                {
                    break;
                }
            }
            else
            {
                CheckOutArmSuckICFallDown();                                    //Steven 20110516 : 修改成整合式Alarm
                break;
            }
        case 4100:
            if(MoveOutArmZ_ToRotateKIT_Pick(iKit)==true)
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoOutArmRotateKIT_Cylinder 4100");
                    Task=4500;
                    break;
                }

                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                iRetryCT=0;
                Task=4200;
            }
            else
            {
                break;
            }
        case 4200:
            flag=true;
            if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)    //ChungHung 20150528 add for 海思 _8Site1x4
            {
                for(int j=0; j<MAX_ARM_Col/2; j++)
                {
                    if(MOT[MOutRotateKit].Tray.Data[j][iKit]!=NULL_IC)
                    {
                        j2=j*2+iKit;
                        if(MOT[MOutRotateKit].Tray.Data[j][iKit]==HAS_NULL_IC || OutArmSuck.Suck[0][j2].Suck())
                        {
                            bSuckDuplicateErr[0][j2]=false;
                            OutArmSuck.SetItemData(0, j2, MOT[MOutRotateKit].Tray.Data[j][iKit]);
                            MOT[MOutRotateKit].SetTraySingleData(j, iKit,NULL_IC);
                        }
                        else if(OutArmSuck.Suck[0][j2].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col/2; j++)
                    {
                        if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC)
                        {
                            j2=j*2+iKit;
                            if(MOT[MOutRotateKit].Tray.Data[j][i]==HAS_NULL_IC || OutArmSuck.Suck[i][j2].Suck())
                            {
                                bSuckDuplicateErr[i][j2]=false;
                                OutArmSuck.SetItemData(i, j2, MOT[MOutRotateKit].Tray.Data[j][i]);
                                MOT[MOutRotateKit].SetTraySingleData(j, i,NULL_IC);
                            }
                            else if(OutArmSuck.Suck[i][j2].Error==false)
                            {
                                flag=false;
                            }
                        }
                    }
                }
            }

            if(flag==false)
                break;

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        iRetryCT++;
                        Task=4600;
                        return false;
                    }
                }
            }

            iRetryCT=0;
            Task=4300;
            break;
        case 4300:
            if(MoveOutArmToAutoSafe())
            {
                if(iKit==0)
                {
                    iKit++;
                    if(TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site2X8 || TestIF.iTestMode==_16Site4X4) //Sam 20190226 : 16Site4X4
                        Task=100;
                    else
                        Task=3100;
                    break;
                }
                else
                {
                    iKit=0;
                    Task=6000;
                }
            }
            else
            {
                break;
            }
        case 6000:
            if(TrayForm.iRotateKIT_OutputType==1)            // +90
            {
                Cylinder[C_OutputRotateKIT].Off();
            }
            else if(TrayForm.iRotateKIT_OutputType==2)       // -90
            {
                Cylinder[C_OutputRotateKIT].On();
            }

            iKit=0;
            return true;
        // 例外處理 -----------------------------
        case 1500:                                                              //(0, 2)調整Offset
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1510;
            }
            else
            {
                Task=1200;
            }
            break;
        case 1510:
            if(MoveOutArmToAutoSafe())
            {
                Task=1100;
            }
            break;
        case 1600:                                                              //(0, 2)吹氣異常
            ErrPart=" ";
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bSuckDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        ErrPart+=OutArmSuck.Suck[i][j].sName;
                        bSuckDuplicateErr[i][j]=true;
                        OutArmSuck.Suck[i][j].Error=false;
                    }
                    else
                    {
                        bSuckDuplicateErr[i][j]=false;
                    }
                }
            }
            ShowErrorMessage("JAM0217", K_RETRY, MOutArmX, bHasDuplicateErr, ErrPart);
            Task=1300;
            break;
        case 4500:                                                              //(1, 3)調整Offset
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=4510;
            }
            else
            {
                Task=4100;
            }
            break;
        case 4510:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4600:
            if(MoveOutArmToAutoSafe())
            {
                if(iRetryCT>ArmSpeed[OutArm].iRetryCT)                          //Sam 20250206 : Rotate pick error retry for setting
                {
                    ErrPart=" ";
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(bSuckDuplicateErr[i][j])
                                bHasDuplicateErr=true;
                            if(OutArmSuck.Suck[i][j].Error)
                            {
                                ErrPart+=OutArmSuck.Suck[i][j].sName;
                                bSuckDuplicateErr[i][j]=true;
                                OutArmSuck.Suck[i][j].Error=false;
                            }
                            else
                            {
                                bSuckDuplicateErr[i][j]=false;
                            }
                        }
                    }
                    ret=ShowErrorMessage("JAM0210", K_RETRY|K_SKIP, MOutArmX, bHasDuplicateErr, ErrPart);
                }
                else
                    ret=K_RETRY;

                if(ret==K_SKIP)
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col/2; j++)
                        {
                            if(tRotate.DutNum == tDutType_8)                    //kevin 20130517 add 8 site
                                j2=j;
                            else
                                j2=j*2+iKit;

                            if(bSuckDuplicateErr[i][j])                         //kevin 20130524
                            {
                                MOT[MOutRotateKit].Tray.PordRec[i][j]->AddErrorRecord("JAM0210");   //Steven 20161215 (jou) : Add Production Error Log  //Steven 20221005 : Production Log減少記憶體使用量
                                MOT[MOutRotateKit].SetTraySingleData(j, i, NULL_IC);                //kevin 20130524
                                bSuckDuplicateErr[i][j]=false;                                      //kevin 20130524
                                OutArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                                if(CosFunction.bUseSCKART)                      //Steven 20161214 (wei) : For SCK ART
                                    fSCKART->AddOutputJamCnt(i, j, ret);        //RogerYang 20250923 : 整合ART OutArm JamCount
                            }
                        }
                    }

                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            OutArmSuck.Suck[i][j].Error=false;
                            bSuckDuplicateErr[i][j]=false;
                        }
                    }
                }
                Task=4000;
            }
            break;
        // --------------------------------------
    }
    return false;
}
//---------------------------------------------------------------------------
//2013-04-12    Dell :旋轉站;馬達版  底下都是馬達版的Code
//---------------------------------------------------------------------------
bool DoOutArmRotateKIT()
{
    bool bRet=false;

    if(iRotate_Type==eCynRotate)                                                //氣缸版
    {
        bRet=DoOutArmRotateKIT_Cylinder();
    }
    else
    {
        if(tRotate.ActiveRotate)
        {
            if(iRotate_Type==eInOutArm1Motor)                                   //add One sucker with rotate
                bRet=M_DoOutArmRotate_InOutArm();
            else
                bRet=M_DoOutArmRotateKIT_Motor();
        }
        else
        {
            bRet=true;                                                          //不使用旋轉機構
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
void CalcPosition_OutArm(int &iXpos, int &iYpos, int &iXPitch, int iYPitch, int iKit)
{
    int OneSuckPitch;
    //AI(ht9045-v899) 20260811: 2x4 交錯 4 站(e9045_2x4_4_13/_14, 非 16 picker) 的 OutArm 資料已被
    //  SetInOutArmParameter_2x4_4() 搬到邏輯欄 0 與 1, 對應實體吸嘴 1 與 3(相隔 2 段 pitch);
    //  20260810 已把 i2x2Suck_Out 改成 1 讓列舉對得上, 但幾何若仍走下面 Sam 20211224
    //  的「1 4 吸嘴」分支, 兩支吸嘴實距只有 2*(Pitch_X_H/3), 到不了相隔 Pitch_X_H
    //  的旋轉站第二格, 故改走「吸嘴1中心到吸嘴3中心」(Pitch_X_H*3/2) 分支;
    //  因 iOutArmXBase=2, 兩式對實體欄 0 那支吸嘴算出來的位置完全相同,
    //  只有遠端吸嘴由欄 3 換成欄 2, 與 2x2 既有作法同源
    bool bOutRotateUse13Picker=(USE_PICKER_COUNT!=ep16Picker &&
                                (iInArmType==e9045_2x4_4_13 || iInArmType==e9045_2x4_4_14));
    iYpos=Prod.iOutArm_RotateY+(iYPitch*iRotato_Out_Row);

    if(USE_OUT_Y_IS_AUTO_PITCH==true && iRotato_Out_Row==0)                     //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        iYpos=iYpos-iYPitch;
    }

    if(iRotate_Type==e1MotRotate1Dut)                                           //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    {
        if(OutArmSuck.iPickCol==1)                                              //Steven 20241012 : 1x1 mode直接使用最小X-Pitch
        {
            if(USE_PICKER_COUNT==ep16Picker)
            {
                iXPitch=iXpitchMinX7;
            }
            else
            {
                iXPitch=iXpitchMinX3;
            }
            OneSuckPitch=iXpitchMin;
        }
        else
        {
            if(USE_PICKER_COUNT==ep16Picker)
            {
                iXPitch=iXpitchMaxX7;
            }
            else
            {
                iXPitch=iXpitchMaxX3;
            }
            OneSuckPitch=iXpitchMax;
        }

        iXpos=Prod.iOutArm_RotateX-(OneSuckPitch*(OutArmSuck.Suck[iRotato_Out_Row][iKit].iMyCol-iOutArmXBase));
    }
    else if(iRotate_Type==e2MotRotate2Dut)                                      //wei 20190515 modify 2 Dut 2 Motor
    {
        if(OutArmSuck.iPickCol==1)                                              //Steven 20241012 : 1x1 mode直接使用最小X-Pitch
        {
            if(USE_PICKER_COUNT==ep16Picker)
            {
                iXPitch=iXpitchMinX7;
            }
            else
            {
                iXPitch=iXpitchMinX3;
            }
            OneSuckPitch=iXpitchMin;
            iXpos=Prod.iOutArm_RotateX-(OneSuckPitch*(OutArmSuck.Suck[iRotato_Out_Row][iKit].iMyCol-iOutArmXBase));
        }
        else if(bUseAxxGPicker() ||
                iCloseSiteModeFor1x4==e1x4CloseAbAc)                            //Steven 20241111 : for 1x4 close 2 site
        {
            if(USE_PICKER_COUNT==ep16Picker)
            {
                iXPitch=iRotateKIT_Pitch_X_H;
            }
            else
            {
                iXPitch=iRotateKIT_Pitch_X_H;
            }
            OneSuckPitch=iRotateKIT_Pitch_X_H/3;
            iXpos=Prod.iOutArm_RotateX-(OneSuckPitch*(OutArmSuck.Suck[iRotato_Out_Row][iKit].iMyCol-iOutArmXBase))-iRotateKIT_Pitch_X_H;
        }
        else
        {
            if(USE_PICKER_COUNT==ep16Picker)
            {
                iXPitch=iRotateKIT_Pitch_X_H/2*3;
            }
            else
            {
                iXPitch=iRotateKIT_Pitch_X_H/2*3;
            }
            OneSuckPitch=iXPitch/3;
            iXpos=Prod.iOutArm_RotateX-(OneSuckPitch*(OutArmSuck.Suck[iRotato_Out_Row][iKit].iMyCol-iOutArmXBase))-iRotateKIT_Pitch_X_H;
        }

    }
    else if(tRotate.DutNum==tDutType_8)
    {
        if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
        {
            iXPitch=iXpitchMaxX3;
            if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)    //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
            {
                iXpos   = Prod.iOutArm_RotateX + (iRotateKIT_Pitch_X_H) - ((iRotateKIT_Pitch_X_H*2)*iKit);
                iYpos   = Prod.iOutArm_RotateY + (iRotateKIT_Pitch_Y_H*iRotato_Out_Row) ;
            }
            else
            {
                iXpos   = Prod.iOutArm_RotateX + (iRotateKIT_Pitch_X_H) - ((iRotateKIT_Pitch_X_H)*iKit);
                iYpos   = Prod.iOutArm_RotateY - (iRotateKIT_Pitch_Y_H*iKit) + (iRotateKIT_Pitch_Y_H*iRotato_Out_Row) ;
            }
        }
        else
        {
            iXPitch =iXpitchMaxX3;                                              //Steven 20140424 : 避免Rotate Pitch錯誤     //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            iXpos   = Prod.iOutArm_RotateX;
            iYpos   = Prod.iOutArm_RotateY ;
        }
    }
    else// if(tRotate.DutNum == tDutType_4)
    {
        if(iRotate_Type==e2MotRotate2Dut)                                       //wei 20190515 modify 2 Dut 2 Motor
        {
            iXPitch =iXpitchMaxX3;
            iXpos   =Prod.iOutArm_RotateX - (iXpitchMax*iKit);
            iYpos   =Prod.iOutArm_RotateY + (6000*iRotato_Out_Row);
        }
        else if((TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site2X8 || TestIF.iTestMode==_16Site4X4) &&  //Sam 20190226 : 16Site4X4
                ArmCanSuck4IC(0)==false &&
                bOutRotateUse13Picker==false)                                    //AI(ht9045-v899) 20260811: 2x4_4_13/_14 改走下方 1,3 吸嘴分支
        {
            if(tRotate.RotateKit_PitchX==40)                                    //20140307 wei Pitch_X 40   需*3 ，  Pitch_X 80 需要  /2*3
            {
                iXPitch = iRotateKIT_Pitch_X_H *2;                              //Sam 20211224 : 修正為使用 1 4 吸嘴
            }
            else
            {
                iXPitch = iRotateKIT_Pitch_X_H;                                 //Sam 20211224 : 修正為使用 1 4 吸嘴
            }
            OneSuckPitch = iXPitch / 3;                                         //Sam 20211224 : 修正為使用 1 4 吸嘴
            iXpos   = Prod.iOutArm_RotateX + iRotateKIT_Start_X_H - OneSuckPitch;   //Sam 20211224 : 修正為使用 1 4 吸嘴
            iYpos   = Prod.iOutArm_RotateY - iRotateKIT_Start_Y_H;
        }
        else
        {
            /*if(bSingleDut)                                                      //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
            {
                iXPitch = iXpitchMaxX3;
                iXpos   = (Prod.iOutArm_RotateX)-4000;
                iYpos   = Prod.iOutArm_RotateY;
            }
            else   */
            {
                if(ArmCanSuck4IC(0)==true)                                      //Steven 20140207 : 修正Rotate吸八顆放四顆
                {                                                               //kevin 20130629    //吸8顆 使用4個rotate
                    iXPitch = iRotateKIT_Pitch_X_H *3/2;                        //吸嘴1中心到吸嘴3中心   //20140318 wei Rotate PitchX=80    |40|40|40|
                    iXpos   = Prod.iOutArm_RotateX-(iRotateKIT_Pitch_X_H/2)*iKit;     //20140318 wei  PitchX=80 移動40
                    iYpos   = Prod.iOutArm_RotateY ;
                }
                else
                {
                    iXPitch = iRotateKIT_Pitch_X_H * 3/2;                       //吸嘴1中心到吸嘴3中心
                    iXpos   = Prod.iOutArm_RotateX + iRotateKIT_Start_X_H;
                    iYpos   = Prod.iOutArm_RotateY - iRotateKIT_Start_Y_H;
                }
            }
        }
    }
}
int OutArmRotate_PitchY;
//==============================================================================
int AutoCalculateOutArmRotatePitch_Y()
{
    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                           //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(OutArmSuck.iPickRow==1)
        {
            OutArmRotate_PitchY=TestIF.iARM_Y_PITCH;
        }
        else
        {
            OutArmRotate_PitchY=iRotateKIT_Pitch_Y_A;                           //RogerYang 20250916 : Fix 單位錯誤，需透過RotateKIT_FormSet轉換
        }
    }
    else
    {
        OutArmRotate_PitchY=TestIF.iARM_Y_PITCH;
    }
    return OutArmRotate_PitchY;
}
//---------------------------------------------------------------------------
bool M_MoveOutArmXY_ToRotateKIT(int iKit)
{
    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffOutArm)
        {
            return false;
        }
    }

    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =iXpitchMaxX3;
    int iMovePitchY =AutoCalculateOutArmRotatePitch_Y();
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)                         //jou 2010-12-20 Pitch & Z 縮減為一個
    {
        iOffsetPos=OutOfsAuto1;
    }
    else
    {
        iOffsetPos=OutOfsRotate_Out;
    }

    CalcPosition_OutArm(iXPos, iYPos, iMovePitchX, iMovePitchY, iKit);

    //AI(ht9045-v899) 20260811: 把旋轉站的 XY 目標與 X-pitch 計算結果記進診斷 Log,
    //  下次 State Record 可直接對「實際開的 pitch vs 旋轉站 KitPitchX」, 不用再反推
    try
    {
        AnsiString sLog;
        sLog.sprintf("ROT-XY iKit=%d Use13=%d iXPitch=%d OneSuck=%d TgtX=%d TgtY=%d "
                     "KitPitchX=%d RotateX=%d StartXH=%d PitchXH=%d ActX=%d",
                     iKit,
                     (int)(USE_PICKER_COUNT!=ep16Picker &&
                           (iInArmType==e9045_2x4_4_13 || iInArmType==e9045_2x4_4_14)),
                     iMovePitchX, iMovePitchX/3, iXPos, iYPos,
                     (int)tRotate.RotateKit_PitchX, Prod.iOutArm_RotateX,
                     iRotateKIT_Start_X_H, iRotateKIT_Pitch_X_H,
                     MOT[MOutArmX].ReadPos());
        OutArmRoundLog_Line(sLog);
    }
    catch(...)
    {
    }

    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool M_MoveOutArmZ_ToRotateKIT_Place(int iKit)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};
    bool bPass[2][4]={{false, false, false, false}, {false, false, false, false}};
    int  iSuckR, iSuckC;

    if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true)    //jou 20231020 : Pass bin no rotate
    {
        for(int i=0; i<MAX_ARM_Row; i++)
            for(int j=0; j<MAX_ARM_Col; j++)
                bPass[i][j]=OutArmSuck.bPass[i][j];
    }

    if(iRotate_Type==e1MotRotate1Dut)                                           //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    {
        for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
        {
            iSuckR=iRotato_Out_Row;
            iSuckC=j;

            if(OutArmSuck.Item[iSuckR][iSuckC]!=NULL_IC &&
               OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC &&
               bPass[iSuckR][iSuckC]==false)                                    //jou 20231020 : Pass bin no rotate
            {
                OutArmSuck.Suck[iSuckR][iSuckC].SetNeedDestroy(true);
                flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Sam 20250207 : Rotate function 吸放也要二段速
            }
            else
            {
                flag[iSuckR][iSuckC]=true;
            }
        }
    }
    else if(iRotate_Type==e2MotRotate2Dut)                                      //wei 20190515 modify 2 Dut 2 Motor
    {
        if(OutArmSuck.iPickCol==1)
        {
            for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
            {
                iSuckR=iRotato_Out_Row;
                iSuckC=j;

                if(OutArmSuck.Item[iSuckR][iSuckC]!=NULL_IC &&
                   OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC &&
                   bPass[iSuckR][iSuckC]==false)                                //jou 20231020 : Pass bin no rotate
                {
                    OutArmSuck.Suck[iSuckR][iSuckC].SetNeedDestroy(true);
                    flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Sam 20250207 : Rotate function 吸放也要二段速
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
        else if(OutArmSuck.iPickCol==2)                                         //Steven 20241111 : for 1x4 close 2 site
        {
            for(int j=0; j<2; j++)
            {
                iSuckR=iRotato_Out_Row;
                iSuckC=j;

                if(OutArmSuck.Item[iSuckR][iSuckC]!=NULL_IC &&
                   OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC &&
                   bPass[iSuckR][iSuckC]==false)                                //jou 20231020 : Pass bin no rotate
                {
                    OutArmSuck.Suck[iSuckR][iSuckC].SetNeedDestroy(true);
                    flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Sam 20250207 : Rotate function 吸放也要二段速
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
        else
        {
            for(int j=0; j<2; j++)
            {
                iSuckR=iRotato_Out_Row;
                iSuckC=j*2+iKit;

                if(OutArmSuck.Item[iSuckR][iSuckC]!=NULL_IC &&
                   OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC &&
                   bPass[iSuckR][iSuckC]==false)                                //jou 20231020 : Pass bin no rotate
                {
                    OutArmSuck.Suck[iSuckR][iSuckC].SetNeedDestroy(true);
                    flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Sam 20250207 : Rotate function 吸放也要二段速
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
    }
    else if(tRotate.DutNum==tDutType_8)                                         //kevin 20130517 add 8 Site
    {
        if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
        {
            if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)    //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
            {
                for(int j=iKit; j<MAX_ARM_Col; j+=4)
                {
                    int i=iRotato_Out_Row;
                    int j2=j*2;
                    if(OutArmSuck.Item[i][j2]!=NULL_IC && OutArmSuck.Item[i][j2]!=HAS_NULL_IC)
                    {
                        OutArmSuck.Suck[i][j2].SetNeedDestroy(true);
                        flag[i][j2]=MOT[OutArmZIndex[i][j2]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[i][j2], &ArmSpeed[OutArm]);
                    }
                    else
                    {
                        flag[i][j2]=true;
                    }
                }
            }
            else
            {
                for(int j=iKit; j<MAX_ARM_Col; j+=4)
                {
                    int i=iRotato_Out_Row;
                    int j2=j*2;
                    if(OutArmSuck.Item[i][j2]!=NULL_IC && OutArmSuck.Item[i][j2]!=HAS_NULL_IC)
                    {
                        OutArmSuck.Suck[i][j2].SetNeedDestroy(true);
                        flag[i][j2]=MOT[OutArmZIndex[i][j2]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[i][j2], &ArmSpeed[OutArm]);
                    }
                    else
                    {
                        flag[i][j2]=true;
                    }
                }
            }
        }
        else
        {
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                    {
                        OutArmSuck.Suck[i][j].SetNeedDestroy(true);
                        flag[i][j]=MOT[OutArmSuck.Suck[i][j].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[i][j], &ArmSpeed[OutArm]);
                    }
                    else
                    {
                        flag[i][j]=true;
                    }
                }
            }
        }
    }
    else
    {                                                                           //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=iKit; j<MAX_ARM_Col; j+=i2x2Suck_Out)                 //(0, 2) & (1, 3)
            {
                if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                {
                    OutArmSuck.Suck[i][j].SetNeedDestroy(true);
                    flag[i][j]=MOT[OutArmSuck.Suck[i][j].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePlace[i][j], &ArmSpeed[OutArm]);   //Sam 20250428 : 修正 1X2 OutRotate 旋轉異常
                }
                else
                {
                    flag[i][j]=true;
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(flag[i][j]==false)
                return false;
        }
    }

    //AI(ht9045-v899) 20260810: 記錄這一趟到底把哪幾支吸嘴放到旋轉站。列舉用的 stride 是 i2x2Suck_Out, 只要它跟吸嘴實際欄位對不上, 就會有 IC 沒旋轉就進 Auto tray (力成PTI DUT1/DUT3 即此)
    try
    {
        AnsiString sLog, sOne;
        sLog.sprintf("ROT-PLACE iKit=%d i2x2Suck_Out=%d iRotato_Out_Row=%d DutNum=%d Item:",
                     iKit, i2x2Suck_Out, iRotato_Out_Row, tRotate.DutNum);
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                sOne.sprintf(" N%d%d=%d/D%d", i, j, OutArmSuck.Item[i][j],
                             (int)OutArmSuck.Suck[i][j].GetNeedDestroyStatus());
                sLog+=sOne;
            }
        }
        OutArmRoundLog_Line(sLog);
    }
    catch(...)
    {
    }
    return true;
}
//---------------------------------------------------------------------------
bool M_MoveOutArmZ_ToRotateKIT_Pick(int iKit, int PickAngle)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};
    int  iSuckR, iSuckC, iTrayY, iTrayX, iRealR, iRealC;
    int iMAX_ARM_Col=0;
    if(tRotate.DutNum == tDutType_8)                                            //kevin 20130517 add 8 Site
        iMAX_ARM_Col=MAX_ARM_Col;
    else
        iMAX_ARM_Col=MAX_ARM_Col/2;

    if(iRotate_Type==e1MotRotate1Dut)                                           //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    {
        for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
        {
            iTrayY=0;
            iTrayX=0;
            iSuckR=iRotato_Out_Row;
            iSuckC=j;
            iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
            iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
            if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
               MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
            {
                if(CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                {
                    OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                    flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Ifor 20250701 fix Rotate Suck Error
                }
            }
            else
            {
                flag[iSuckR][iSuckC]=true;
            }
        }
    }
    else if(iRotate_Type==e2MotRotate2Dut)                                      //wei 20190515 modify 2 Dut 2 Motor
    {
        if(OutArmSuck.iPickCol==1)
        {
            for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
            {
                iTrayY=0;
                iTrayX=0;
                iSuckR=iRotato_Out_Row;
                iSuckC=j;
                iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                {
                    if(CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                    {
                        OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                        flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Ifor 20250701 fix Rotate Suck Error
                    }
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
        else if(OutArmSuck.iPickCol==2)
        {
            for(int j=0; j<2; j++)
            {
                iTrayY=0;
                iTrayX=j;
                iSuckR=iRotato_Out_Row;
                iSuckC=j;
                iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                {
                    if(CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                    {
                        OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                        flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Ifor 20250701 fix Rotate Suck Error
                    }
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
        else
        {
            for(int j=0; j<2; j++)
            {
                iTrayY=0;
                iTrayX=j;
                iSuckR=iRotato_Out_Row;
                iSuckC=j*2+iKit;
                iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                {
                    if(CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                    {
                        OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                        flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Ifor 20250701 fix Rotate Suck Error
                    }
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
    }
    else if((TestIF.iTestMode==QualSite1X4 && TestIF.iUseSuckMode==4) || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
    {
        for(int j=0; j<iMAX_ARM_Col; j++)                                       //(0, 2) & (1, 3)
        {                                                                       //Kit(0, 1) -> Suck(0, 2) or Suck(1, 3)
            iSuckR=0;
            if(tRotate.DutNum==tDutType_8)                                      //jou 2015-10-14 fix 1x4 rotate dut8 hang up
                iSuckC=j;
            else
                iSuckC=j*i2x2Suck_Out+iKit;

            iTrayX=j;
            iTrayY=0;

            if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
            {
                if(CheckRotateOutAnglePostion(iTrayY, iTrayX, PickAngle, 1)==true)    //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                {
                    OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                    flag[iSuckR][iSuckC]=MOT[OutArmZIndex[iSuckR][iSuckC]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);
                }
            }
            else
            {
                flag[iSuckR][iSuckC]=true;
            }
        }
    }
    else if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14) //|| iInArmType==e9045_2x4_4_14)   //Sam 20211224 : 修正為使用 1 4 吸嘴  //Steven 20201028 : for rotator
    {
        if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)        //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
        {
            for(int j=iKit; j<iMAX_ARM_Col; j+=4)                               //(0, 2) & (1, 3)
            {
                iSuckR=iRotato_Out_Row;
                iSuckC=j*2;
                iTrayX=1;
                iTrayY=0;
                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                {
                    if(CheckRotateOutAnglePostion(iTrayY, iTrayX, PickAngle, 1)==true)    //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                    {
                        OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                        flag[iSuckR][iSuckC]=MOT[OutArmZIndex[iSuckR][iSuckC]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);
                    }
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
        else
        {
            if(iRotate_Type==e2MotRotate2Dut && TestIF.iTestMode==DualSite)
            {
                for(int j=iKit; j<iMAX_ARM_Col; j++)                            //(0, 2) & (1, 3)
                {
                    iSuckR=iRotato_Out_Row;
                    iSuckC=j*2;
                    iTrayX=j;
                    iTrayY=0;
                    if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                    {
                        if(CheckRotateOutAnglePostion(iTrayY, iTrayX, PickAngle, 1)==true)   //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                            flag[iSuckR][iSuckC]=MOT[OutArmZIndex[iSuckR][iSuckC]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);
                        }
                    }
                    else
                    {
                        flag[iSuckR][iSuckC]=true;
                    }
                }
            }
            else
            {
                for(int j=iKit; j<iMAX_ARM_Col; j+=4)                           //(0, 2) & (1, 3)
                {
                    iSuckR=iRotato_Out_Row;
                    iSuckC=j*2;
                    iTrayX=j+1;
                    iTrayY=iKit;
                    if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC && MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                    {
                        if(CheckRotateOutAnglePostion(iTrayY, iTrayX, PickAngle, 1)==true)   //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                            flag[iSuckR][iSuckC]=MOT[OutArmZIndex[iSuckR][iSuckC]].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);
                        }
                    }
                    else
                    {
                        flag[iSuckR][iSuckC]=true;
                    }
                }
            }
        }
    }
    else
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<iMAX_ARM_Col; j++)                                   //(0, 2) & (1, 3)
            {
                if(iRotate_Type==e2MotRotate2Dut && TestIF.iTestMode==SingleSite)
                {
                    iSuckR=0;
                    //iSuckC=3;
                    iSuckC=0;                                                   //Sam 20250618 : 修正 Single Rotate 錯誤
                    iTrayX=1;
                    iTrayY=0;
                }
               /* else if(bSingleDut&& TestIF.iTestMode==SingleSite)              //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
                {
                    iSuckR=0;
                    //iSuckC=3;
                    iSuckC=0;                                                   //Sam 20250618 : 修正 Single Rotate 錯誤
                    iTrayX=0;
                    iTrayY=0;
                    if(i!=0 || j!=0)                                            //Steven 20190809 : Fixed for hang up
                        continue;
                }    */
                else
                {                                                               //Kit(0, 1) -> Suck(0, 2) or Suck(1, 3)
                    iSuckR=i;
                    if(tRotate.DutNum==tDutType_8)                              //kevin 20130517 add 8 Site
                        iSuckC=j;
                    else
                        iSuckC=j*i2x2Suck_Out+iKit;

                    iTrayX=j;
                    iTrayY=i;
                }

                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC)
                {
                    if(CheckRotateOutAnglePostion(iTrayY, iTrayX, PickAngle, 1)==true)    //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                    {
                        OutArmSuck.Suck[iSuckR][iSuckC].SetNeedSuck(true);
                        flag[iSuckR][iSuckC]=MOT[OutArmSuck.Suck[iSuckR][iSuckC].iMotNo].MotorMove2SpeedForPicker(Prod.iOutArm_RotatePick[iSuckR][iSuckC], &ArmSpeed[OutArm]);  //Sam 20250428 : 修正 1X2 OutRotate 旋轉異常
                    }
                }
                else
                {
                    flag[iSuckR][iSuckC]=true;
                }
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(flag[i][j]==false)
                return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool M_DoOutArmRotate_InOutArm()                                                //add One sucker with rotate
{
    int &Task=iOutArmRotateKit;
    int ret;
    static int rCount;

    switch(Task)
    {
        case 1:
            rCount=1;
//            DoRotate_Out(rCount, true);
            ret=SetMotorResolution(OutAngle45, OutAngle90, false);
            Task=100;
            break;
        case 100:
            ret=DoRotate_Out(rCount);

            if(ret==1)
            {
                return true;
            }
            else if(ret==-999)                                                  //Error
            {
                Task=2000;
            }
            break;
        case 2000:
            ret=ShowErrorMessage("JAM0553", K_RETRY, MInRotateKit);             //kevin 20130524

            if(ret==K_RETRY)
            {
                Task=2100;
            }
            break;
        case 2100:
            FrmRotate->bRotateOutHome=true;                                     //kevin 20130706
            FrmRotate->InitialOutRotateHome();                                  //Steven 20170329 (wei) : Add individual rotate motor
            Task=2200;
            break;
        case 2200:
            if(FrmRotate->bRotateOutHome==false)                                //kevin 20130706
            {
                FrmRotate->SetOutRotateSpeed(ArmSpeed[OutRotate].iACDCBodySP, ArmSpeed[OutRotate].iBodySP);    //Steven 20170329 (wei) : Add individual rotate motor
                Task=100;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int GotOutRotateKitForSingle(int &iKit, int &iCount, bool bRelease)
{
    int Task=1000;
    if(bRelease)
    {
        Task=1000;
        if(OutArmSuck.Item[iRotato_Out_Row][0]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][0]!=HAS_NULL_IC && iCount==0)
        {
            iKit=0;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][0]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][0]==HAS_NULL_IC) && iCount==0)
        {
            iCount=1;
            Task=1000;
        }
        else if(OutArmSuck.Item[iRotato_Out_Row][1]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][1]!=HAS_NULL_IC && iCount==1)
        {
            iKit=1;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][1]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][1]==HAS_NULL_IC) && iCount==1)
        {
            iCount=2;
            Task=1000;
        }
        else if(OutArmSuck.Item[iRotato_Out_Row][2]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][2]!=HAS_NULL_IC && iCount==2)
        {
            iKit=2;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][2]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][2]==HAS_NULL_IC) && iCount==2)
        {
            iCount=3;
            Task=1000;
        }
        else if(OutArmSuck.Item[iRotato_Out_Row][3]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][3]!=HAS_NULL_IC && iCount==3)
        {
            iKit=3;
            Task=1100;
        }
        else if(iRotato_Out_Row==1)
        {
            Task=6000;
        }
        else
        {
            iRotato_Out_Row=1;
            Task=1000;
        }
    }
    else
    {
        Task=1;
        if(iRotato_Out_Row==0)
        {
            if(iCount==0)
            {
                iKit=1;
                iCount=1;
            }
            else if(iCount==1)
            {
                iKit=2;
                iCount=2;
            }
            else if(iCount==2)
            {
                iKit=3;
                iCount=3;
            }
            else
            {
                iCount=0;
                iRotato_Out_Row=1;
                iKit=0;
            }
            Task=10;
        }
        else
        {
            if(iCount==0)
            {
                iKit=1;
                iCount=1;
                Task=10;
            }
            else if(iCount==1)
            {
                iKit=2;
                iCount=2;
                Task=10;
            }
            else if(iCount==2)
            {
                iKit=3;
                iCount=3;
                Task=10;
            }
            else
            {
                iCount=0;
                iRotato_Out_Row=0;
                iKit=0;
                MOT[MOutRotateKit].SetHasNullIcToNullIc();                      //Frank 20170822 (wei): 清空Rotate資料
                Task=1;
            }
        }
    }
    return Task;
}
//------------------------------------------------------------------------------
int GotOutRotateKitForAxxG(int &iKit, int &iCount, bool bRelease)
{
    int Task=1000;
    if(bRelease)
    {
        if(((OutArmSuck.Item[iRotato_Out_Row][0]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][0]!=HAS_NULL_IC) ||
            (OutArmSuck.Item[iRotato_Out_Row][1]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][1]!=HAS_NULL_IC)) &&
           iCount==0)
        {
            iKit=0;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][0]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][0]==HAS_NULL_IC ||
                 OutArmSuck.Item[iRotato_Out_Row][1]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][1]==HAS_NULL_IC) &&
                iCount==0)
        {
            iCount=1;
            Task=1000;
        }
        else if(iRotato_Out_Row==1)
        {
            Task=6000;
        }
        else
        {
            iRotato_Out_Row=1;
            Task=1000;
        }
    }
    else
    {
        Task=10;
        if(iRotato_Out_Row==0)
        {
            iCount=0;
            iRotato_Out_Row=1;
            iKit=0;
            Task=10;
        }
        else
        {
            iCount=0;
            iRotato_Out_Row=0;
            iKit=0;
            MOT[MOutRotateKit].SetHasNullIcToNullIc();                          //Frank 20170822 (wei): 清空Rotate資料
            Task=1;
        }
    }
    return Task;
}
//------------------------------------------------------------------------------
int GotOutRotateKitForAxEx_2RotMot(int &iKit, int &iCount, bool bRelease)
{
    int Task=1000;
    if(bRelease)
    {
        Task=1000;
        if(((OutArmSuck.Item[iRotato_Out_Row][0]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][0]!=HAS_NULL_IC) ||
            (OutArmSuck.Item[iRotato_Out_Row][1]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][1]!=HAS_NULL_IC)) &&
           iCount==0)
        {
            iKit=0;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][0]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][0]==HAS_NULL_IC ||
                 OutArmSuck.Item[iRotato_Out_Row][1]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][1]==HAS_NULL_IC) &&
                iCount==0)
        {
            iCount=1;
            Task=1000;
        }
        else if(((OutArmSuck.Item[iRotato_Out_Row][2]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][2]!=HAS_NULL_IC) ||
                 (OutArmSuck.Item[iRotato_Out_Row][3]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][3]!=HAS_NULL_IC)) &&
                iCount==1)
        {
            iKit=1;
            Task=1100;
        }
        else if((OutArmSuck.Item[iRotato_Out_Row][2]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][2]==HAS_NULL_IC ||
                 OutArmSuck.Item[iRotato_Out_Row][3]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][3]==HAS_NULL_IC) &&
                iCount==0)
        {
            iCount=2;
            Task=1000;
        }
        else if(iRotato_Out_Row==1)
        {
            Task=6000;
        }
        else
        {
            iRotato_Out_Row=1;
            Task=1000;
        }
    }
    else
    {
        Task=1;
        if(iRotato_Out_Row==0)
        {
            if(iCount==0)
            {
                iKit=1;
                iCount=1;
            }
            else
            {
                iCount=0;
                iRotato_Out_Row=1;
                iKit=0;
            }
            Task=10;
        }
        else
        {
            if(iCount==0)
            {
                iKit=1;
                iCount=1;
                Task=10;
            }
            else
            {
                iCount=0;
                iRotato_Out_Row=0;
                iKit=0;
                MOT[MOutRotateKit].SetHasNullIcToNullIc();                       //Frank 20170822 (wei): 清空Rotate資料
                Task=1;
            }
        }
    }
    return Task;
}
//---------------------------------------------------------------------------
bool M_DoOutArmRotateKIT_Motor()
{
    int &Task=iOutArmRotateKit;
    int j2;
    int iSuckR, iSuckC, iTrayY, iTrayX, iRealR, iRealC;

    static int iRetryCT=0;
    static int iKit=0;
    static int rCount=0;
    bool flag=true;
    int iRotateCount=0;                                                         //wei 20141230 Rotate旋轉問題 for 大IC單獨吸
    int ret;
    static int iOutCount=0;
    bool bHasError=false;
    bool bHasDuplicateErr=false;
    static bool bSuckDuplicateErr[MAX_ARM_Row][MAX_ARM_Col]={false};            //Steven 20091218 : Avoid duplicate message
    static bool bRotateMoveOk=false, bOutXYMoveOk=false;
    AnsiString ErrPart="";

    int iMAX_ARM_Row, iMAX_ARM_Col;
    if(tRotate.DutNum==tDutType_8)                                              //kevin 20130517 add 8 site
    {
        iMAX_ARM_Row=MAX_ARM_Row;
        iMAX_ARM_Col=MAX_ARM_Col;
    }
    else
    {
        iMAX_ARM_Row=MAX_ARM_Row;
        iMAX_ARM_Col=MAX_ARM_Col/2;
    }

    switch(Task)
    {
        case 1:
            iKit=0;
            Task=10;
            iOutCount=0;
            MoveOutRotateToDegreeAtSameTime(0, true);                           //Steven 20170504 (wei) : 針對獨立馬達旋轉站, 一起旋轉到特定角度
        case 10:                                                                //jou 2013-05-08 不等旋轉完成
            ret=SetMotorResolution(OutAngle45, OutAngle90, false);              //kevin 20131003
            Task=100;
        case 100:
            MOT[MOutRotateKit].SetTray(NULL_IC, __FUNC__);
            ZeroMemory(bSuckDuplicateErr, sizeof(bSuckDuplicateErr));
            Task=1000;
        case 1000:
            if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                Task=GotOutRotateKitForSingle(iKit, iOutCount, true);
            }
            else if(iRotate_Type==e2MotRotate2Dut)
            {
                if(OutArmSuck.iPickCol==1)
                {
                    Task=GotOutRotateKitForSingle(iKit, iOutCount, true);
                }
                else if(bUseAxxGPicker() ||
                        iCloseSiteModeFor1x4==e1x4CloseAbAc)                    //Steven 20241111 : for 1x4 close 2 site
                {
                    Task=GotOutRotateKitForAxxG(iKit, iOutCount, true);
                }
                else
                {
                    Task=GotOutRotateKitForAxEx_2RotMot(iKit, iOutCount, true);
                }
            }
            else if(tRotate.DutNum==tDutType_8)                                 //kevin 20130517 add 8 site
            {
                if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                {
                    if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)    //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                    {
                        if(OutArmSuck.Item[iRotato_Out_Row][0]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][0]!=HAS_NULL_IC && iOutCount==0)
                        {
                            iKit=0;
                            Task=1100;
                        }
                        else if((OutArmSuck.Item[iRotato_Out_Row][0]==NULL_IC || OutArmSuck.Item[iRotato_Out_Row][0]==HAS_NULL_IC) && iOutCount==0)
                        {
                            iOutCount=1;
                            Task=1000;
                        }
                        else if(OutArmSuck.Item[iRotato_Out_Row][2]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][2]!=HAS_NULL_IC && iOutCount==1)
                        {
                            iKit=1;
                            Task=1100;
                        }
                        else if(iRotato_Out_Row==1)
                        {
                            Task=6000;
                        }
                        else
                        {
                            iRotato_Out_Row=1;
                            Task=1000;
                        }
                    }
                    else
                    {
                        if(OutArmSuck.Item[iRotato_Out_Row][0]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][0]!=HAS_NULL_IC)
                        {
                            iKit=0;
                            Task=1100;
                        }
                        else if(OutArmSuck.Item[iRotato_Out_Row][2]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][2]!=HAS_NULL_IC)
                        {
                            iKit=1;
                            Task=1100;
                        }
                        else if(iRotato_Out_Row==1)
                        {
                            Task=6000;
                        }
                        else
                        {
                            iRotato_Out_Row=1;
                            Task=1000;
                        }
                    }
                }
                else
                {
                    Task=1100;
                }
            }
            else
            {
                if(iRotate_Type==e2MotRotate2Dut)                               //wei 20190515 modify 2 Dut 2 Motor
                {
                    if(TestIF.iTestMode==SingleSite)
                    {
                        if(OutArmSuck.Item[0][0]!=NULL_IC && OutArmSuck.Item[0][0]!=HAS_NULL_IC)
                        {
                            iKit=1;
                            Task=1100;
                        }
                        else
                        {
                            Task=2000;
                        }
                    }
                    else if(TestIF.iTestMode==DualSite)
                    {
                        if((OutArmSuck.Item[0][0]!=NULL_IC && OutArmSuck.Item[0][0]!=HAS_NULL_IC) ||
                           (OutArmSuck.Item[0][2]!=NULL_IC && OutArmSuck.Item[0][2]!=HAS_NULL_IC) )
                        {
                            Task=1100;
                        }
                        else
                        {
                            Task=2000;
                        }
                    }
                    else
                    {
                        Task=2000;
                    }
                }
                /*else if(bSingleDut)                                             //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
                {
                     if(OutArmSuck.Item[0][3]!=NULL_IC && OutArmSuck.Item[0][3]!=HAS_NULL_IC)
                     {
                        Task=1100;
                     }
                     else
                     {
                        Task=2000;
                     }
                }  */
                else
                {
                    if((OutArmSuck.Item[0][0+iKit]!=NULL_IC             && OutArmSuck.Item[0][0+iKit]!=HAS_NULL_IC) ||
                       (OutArmSuck.Item[0][i2x2Suck_Out+iKit]!=NULL_IC  && OutArmSuck.Item[0][i2x2Suck_Out+iKit]!=HAS_NULL_IC) ||
                       (OutArmSuck.Item[1][0+iKit]!=NULL_IC             && OutArmSuck.Item[1][0+iKit]!=HAS_NULL_IC) ||
                       (OutArmSuck.Item[1][i2x2Suck_Out+iKit]!=NULL_IC  && OutArmSuck.Item[1][i2x2Suck_Out+iKit]!=HAS_NULL_IC))
                    {
                        Task=1100;
                    }
                    else
                    {
                        Task=2000;
                    }
                }
            }
            break;
        case 1100:                                                              //wei 20160106 增加放Rotate前偵測
            if(CheckRotateSensor(false))
            {
                Task=1150;
            }
            else
            {
                Task=1120;
            }
            break;
        case 1120:
            if(MoveOutArmXY_ToShuttleAlarmArea()==false)                        //如果要移開,而且還沒移到定位
            {
                return false;                                                   //先離開等In Arm
            }
            Task=1130;
            break;
        case 1130:                                                              //out Rotate 疊料
            ShowErrorMessage("JAM0552", K_RETRY, MOutRotateKit);
            Task=1100;
            break;
        case 1150:
        //Steven 20170505 (wei) : for 提升Rotate的UPH
        //==>
            MoveOutRotateToDegreeAtSameTime(0, true);
            bRotateMoveOk=false;
            bOutXYMoveOk=false;
            Task=1160;
        case 1160:
            if(bRotateMoveOk==false)                                            //Steven 20170504 (wei) : 針對獨立馬達旋轉站, 一起旋轉到特定角度
                bRotateMoveOk=MoveOutRotateToDegreeAtSameTime(0);

            if(bOutXYMoveOk==false)
                bOutXYMoveOk=M_MoveOutArmXY_ToRotateKIT(iKit);
        //<==
        //Steven 20170505 (wei) : for 提升Rotate的UPH

            if(bRotateMoveOk && bOutXYMoveOk)
            {
                CheckOutArmSuckICFallDown();

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        OutArmSuck.Suck[i][j].SetNeedDestroy(false);            //kevin 20130524   //jou 20231012 : 修正rotate預設值true->false
                    }
                }
                Task=1200;
            }
            else
            {
                CheckOutArmSuckICFallDown();
                break;
            }
        case 1200:
            if(M_MoveOutArmZ_ToRotateKIT_Place(iKit)==true)
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("M_DoOutArmRotateKIT_Motor 1200");
                    Task=1500;
                    break;
                }

                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                Task=1300;
            }
            else
            {
                break;
            }
        case 1300:
            flag=true;
            if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                {
                    iTrayY=0;
                    iTrayX=0;
                    iSuckR=iRotato_Out_Row;
                    iSuckC=j;
                    if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                    {
                    }
                    else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC)
                    {
                        if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                            if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC)    //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                bOutArmCheckDestroyACT[iSuckR][iSuckC]=true;    //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                        }
                        else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                        {
                            flag=false;
                        }
                        else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                        {
                            bHasError=true;
                        }
                    }
                }
            }
            else if(iRotate_Type==e2MotRotate2Dut)
            {
                if(OutArmSuck.iPickCol==1)
                {
                    for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                    {
                        iTrayY=0;
                        iTrayX=0;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=0;
                        if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                        {
                        }
                        else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC)
                        {
                            if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC)    //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                    bOutArmCheckDestroyACT[iSuckR][iSuckC]=true;    //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                            {
                                bHasError=true;
                            }
                        }
                    }
                }
                else if(OutArmSuck.iPickCol==2)
                {
                    for(int j=0; j<2; j++)
                    {
                        iTrayY=0;
                        iTrayX=j;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j;
                        if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                        {
                        }
                        else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC)
                        {
                            if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC)    //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                    bOutArmCheckDestroyACT[iSuckR][iSuckC]=true;    //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                            {
                                bHasError=true;
                            }
                        }
                    }
                }
                else
                {
                    for(int j=0; j<2; j++)
                    {
                        iTrayY=0;
                        iTrayX=j;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j*2+iKit;

                        if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                        {
                        }
                        else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC)
                        {
                            if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC)    //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                    bOutArmCheckDestroyACT[iSuckR][iSuckC]=true;    //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                            {
                                bHasError=true;
                            }
                        }
                    }
                }
            }
            else if(tRotate.DutNum==tDutType_8)                                 //Steven 20160615 : fixed for 2x4_4 wirh rotate
            {
                if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                {
                    for(int j=iKit; j<iMAX_ARM_Col; j+=4)
                    {
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        //==>
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j*2;
                        if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)
                        {                                                       //wei 20151229  Xilinx 52.5x52.5 放一個Rotate     //wei 20170822 (Steven): ==5000 -> >=5000
                            iTrayX=1;
                            iTrayY=0;
                        }
                        else
                        {
                            iTrayX=1+iKit;
                            iTrayY=iKit;
                        }

                        if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                        {
                        }
                        else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC)
                        {
                            if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC) //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                    bOutArmCheckDestroyACT[iSuckR][iSuckC]=true; //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                            {
                                bHasError=true;
                            }
                        }
                        //<==
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                    }
                }
                else
                {
                    for(int i=0; i<iMAX_ARM_Row; i++)
                    {
                        for(int j=0; j<iMAX_ARM_Col; j++)
                        {
                            //Steven 20170504 (wei) : 修改Rotate資料交換方式
                            //==>
                            iSuckR=i;
                            iSuckC=j;

                            if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
                            {
                                iTrayX=j;
                                iTrayY=i+iKit;
                            }
                            else
                            {
                                iTrayX=j;
                                iTrayY=i;
                            }

                            if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                            {
                            }
                            else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC] == HAS_NULL_IC)
                            {
                                if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                                {
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                    if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC) //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                        bOutArmCheckDestroyACT[iSuckR][iSuckC]=true; //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                                }
                                else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                                {
                                    flag=false;
                                }
                                else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                                {
                                    bHasError=true;
                                }
                            }
                            //<==
                            //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<iMAX_ARM_Row; i++)
                {
                    for(int j=0; j<iMAX_ARM_Col; j++)
                    {
                        if(iRotate_Type==e2MotRotate2Dut && TestIF.iTestMode==SingleSite)   //wei 20190515 modify 2 Dut 2 Motor
                        {
                            iSuckR=0;
                            //iSuckC=3;
                            iSuckC=0;   //Sam 20250618 : 修正 Single Rotate 錯誤
                            iTrayX=1;
                            iTrayY=0;
                        }
                        else
                        {
                            //Steven 20170504 (wei) : 修改Rotate資料交換方式
                            //==>
                            iSuckR=i;
                            iSuckC=j*i2x2Suck_Out+iKit;

                            if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
                            {
                                iTrayX=j;
                                iTrayY=i+iKit;
                            }
                            else
                            {
                                iTrayX=j;
                                iTrayY=i;
                            }
                        }

                        if(CosFunction.bPassBinNoRotate==true && tRotate.bPassBinNoRotate==true && OutArmSuck.bPass[iSuckR][iSuckC]==true) //jou 20231020 : Pass bin no rotate
                        {
                        }
                        else if(OutArmSuck.Item[iSuckR][iSuckC] && OutArmSuck.Suck[iSuckR][iSuckC].GetNeedDestroyStatus() || OutArmSuck.Item[iSuckR][iSuckC] == HAS_NULL_IC)
                        {
                            if(OutArmSuck.Item[iSuckR][iSuckC]==HAS_NULL_IC || OutArmSuck.Suck[iSuckR][iSuckC].Destroy())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyToTray(iSuckR, iSuckC, NULL_IC, MOT[MOutRotateKit], iTrayY, iTrayX, OutArmSuck.Item[iSuckR][iSuckC]);
                                //AI(ht9045-v899) 20260811: 模擬用——沿用 IC 資料轉移時才檢查的慣例
                                //  (同 SwapShuttleDataToOutArm / DoPlaceToHPSwapData)。iSuckR/iSuckC 是吸嘴，
                                //  iTrayY/iTrayX 是旋轉站孔位，四個索引這裡本來就算好了
                                //  必須傳 iMyRow/iMyCol（實體）不能傳邏輯 i/j：GetOutArmCellPos() 用的是實體欄位，
                                //  而本機 CopyInitSuck 把邏輯欄1映射到實體欄2，傳錯會造出一段 pitch 的假誤差
                                InspectOutArmPosition(MOutRotateKit, OutArmSuck.Suck[iSuckR][iSuckC].iMyRow, OutArmSuck.Suck[iSuckR][iSuckC].iMyCol,
                                                      iTrayY, iTrayX, iOutPlaceToRotate);
                                if(OutArmSuck.Item[iSuckR][iSuckC]!=HAS_NULL_IC) //Steven 20170109 (Jou) : 沒IC的地方不檢查吹氣
                                    bOutArmCheckDestroyACT[iSuckR][iSuckC]=true; //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error)
                            {
                                bHasError=true;
                            }
                        }
                        //<==
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                    }
                }
            }

            if(flag==false)                                                     //kevin 20130517
                break;

            if(bHasError)
            {
                Task=1600;
                break;
            }

            ZeroMemory(bSuckDuplicateErr, sizeof(bSuckDuplicateErr));
            Task=2000;
            break;
         // 例外處理 -----------------------------
        case 1500:      //(0, 2)調整Offset
            if(MoveOutArmToAutoSafe())
            {
                bEnterOffset=false;
                Task=1100;
            }
            break;
        case 1600:      //(0, 2)吹氣異常
            if(MoveOutArmToAutoSafe())
            {
                Task=1601;
            }
            break;
        case 1601:
            ErrPart=" ";
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bSuckDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        ErrPart+=OutArmSuck.Suck[i][j].sName;
                        bSuckDuplicateErr[i][j]=true;
                        OutArmSuck.Suck[i][j].Error=false;
                    }
                    else
                    {
                        bSuckDuplicateErr[i][j]=false;
                    }
                }
            }
            ShowErrorMessage("JAM0217", K_RETRY, MOutArmX, bHasDuplicateErr, ErrPart);
            Task=1200;
            break;
        case 2000:
            if(MoveOutArmToAutoSafe())
            {
                if(iRotate_Type==e1MotRotate1Dut ||                             //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
                   iRotate_Type==e2MotRotate2Dut)
                {
                    Task=2500;
                }
                else if(tRotate.DutNum==tDutType_8)                             //kevin 20130517 add 8 site
                {
                    if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                    {
                        if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)       //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                        {
                            Task=2500;
                        }
                        else
                        {
                            if(iKit==0)
                            {
                                if(OutArmSuck.Item[iRotato_Out_Row][2]!=NULL_IC && OutArmSuck.Item[iRotato_Out_Row][2]!=HAS_NULL_IC)
                                {
                                    iKit=1;
                                    Task=1100;
                                }
                                else
                                {
                                    iKit=0;
                                    Task=2500;
                                }
                                break;
                            }
                            else
                            {
                                iKit=0;
                                Task=2500;
                            }
                        }
                        break;
                    }
                    else if(OutArmSuck.FindNoIC())                              //kevkin 20130519 IC已放下
                    {
                        iKit=0;
                    }
                }
                else
                {
                }
                Task=2500;
                break;
            }
            break;
        case 2500:                                                              //偵測IC 是否疊料
            if(CheckRotateSensor(false))
            {
                Task=2510;
            }
            else
            {
                if(IniConfig.bA21RotateDetectErrNeedShake)
                {
                    InitialShakeOutArmRotateKIT();
                    Task=2501;
                }
                else
                {
                    Task=2502;
                }
            }
            break;
        case 2501:
            if(M_DoShakeOutArmRotateKIT()==true)                                //JerryYang 20160804 偵測到異常要轉三下
            {
                if(CheckRotateSensor(false))                                    //JerryYang 20160804 轉完再偵測一次是否疊料
                {
                    Task=2510;
                }
                else
                {
                    Task=2502;
                }
            }
            break;
        case 2502:
            if(MoveOutArmXY_ToShuttleAlarmArea()==false)                        //如果要移開,而且還沒移到定位
            {
                return false;                                                   //先離開等In Arm
            }
            Task=2503;
            break;
        case 2503:                                                              //out Rotate 疊料
            ShowErrorMessage("JAM0552", K_RETRY, MOutRotateKit);
            Task=2500;
            break;
        case 2510:
            DoRotate_Out(rCount, true);
            if(FrmRotate->bShowRotateBySite==true)                              //Steven 20170329 (wei) : Add individual rotate motor
            {
                Task=2600;
            }
            else if(tRotate.DutNum==tDutType_8)
            {
                if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                {
                    Task=2550;                                                  //wei 20141230 Rotate旋轉問題 for 大IC單獨吸
                }
                else
                {
                    Task=2600;
                }
            }
            else
            {
                Task=2600;
            }
            break;
        case 2550:                                                              //wei 20141230 Rotate旋轉問題 for 大IC單獨吸
            DoRotate_Out(rCount, true);
            if(iInArmType==e9045_1x2_2_14 ||                                    //Steven 20201028 : for rotator
               iInArmType==e9045_2x2_4_14 ||
               iInArmType==e9045_2x4_4_14  ||
               iRotate_Type==eInOutArm1Motor)
            {
                for(iRotateCount=4; iRotateCount>=0; iRotateCount--)
                {
                    if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)       //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                    {
                        if(CheckRotateOutAnglePostion(0, 1, iRotateCount-1, 0)==true)       //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            rCount=iRotateCount;
                            Task=2600;
                            break;
                        }
                    }
                    else
                    {
                        if(CheckRotateOutAnglePostion(iKit, iKit+1, iRotateCount-1, 0)==true)   //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            rCount=iRotateCount;
                            Task=2600;
                            break;
                        }
                    }
                }
            }
            break;
        case 2600:
            if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {

            }
            else if(iRotate_Type==e2MotRotate2Dut)
            {
                for(int j=0; j<2; j++)
                {
                    if(MOT[MOutRotateKit].Tray.Data[j][0]!=NULL_IC &&
                       MOT[MOutRotateKit].Tray.Data[j][0]!=HAS_NULL_IC &&
                       MOT[MOutRotateKit].Tray.iNeedRotAng[j][0]!=MOT[MOutRotateKit].Tray.iCurrRotAng[j][0])
                    {
                        DoRotate_Out(rCount, true);
                    }
                }
            }
            else if(FrmRotate->bShowRotateBySite==true)                         //Sam 20240110 : 檢查這次的選轉位置若都為 Has_Null_IC 都不要轉了
            {
                if(rCount-1>0)
                {
                    bool bNeedR=false;
                    for(int i=0; i<iMAX_ARM_Row; i++)
                    {
                        for(int j=0; j<iMAX_ARM_Col; j++)
                        {
                            iSuckR=i;
                            iSuckC=j;
                            iTrayX=j;
                            iTrayY=i;
                            if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                               MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=HAS_NULL_IC &&
                               CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)
                            {
                                bNeedR=true;
                            }
                        }
                    }

                    if(bNeedR==false)
                    {
                        rCount--;
                        break;
                    }
                }
            }

            ret=DoRotate_Out(rCount);

            if(ret==1)
            {
                bOut_ICRotationCompleteOnKit=true;                              //Sam 20210623 : 修正 IC 轉完後被 Home，導致 IC 轉向異常。
                Task=3000;
            }
            else if(ret==-999 || ret==-100)                                     //Error
            {
                Task=4800;
            }
            break;
        case 3000:
            Task=3100;
        case 3100:                                                              //吸取IC
            if(iRotate_Type==e1MotRotate1Dut ||                                 //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
               iRotate_Type==e2MotRotate2Dut)
            {
                if(MOT[MOutRotateKit].Tray.HasIC())
                {
                    iRetryCT=0;
                    Task=4000;
                }
                else
                {
                    Task=4400;
                }
            }
            else if(tRotate.DutNum==tDutType_8)                                      //kevin 20130517 add 8 site
            {
                if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                {
                    if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)            //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                    {
                        if(MOT[MOutRotateKit].Tray.Data[1][0]!=NULL_IC)
                        {
                            Task=4000;
                        }
                    }
                    else
                    {
                        if(MOT[MOutRotateKit].Tray.Data[1][0]!=NULL_IC)
                        {
                            iKit=0;
                            Task=4000;
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[2][1]!=NULL_IC)
                        {
                            iKit=1;
                            Task=4000;
                        }
                        else
                        {
                            Task=6000;
                        }
                    }
                }
                else
                {
                    if(MOT[MOutRotateKit].Tray.HasRealIC())
                    {
                        iRetryCT=0;
                        Task=4000;
                    }
                    else
                    {
                        Task=5000;
                    }
                }
            }
            else
            {
                if(MOT[MOutRotateKit].Tray.Data[0][0]!=NULL_IC ||
                   MOT[MOutRotateKit].Tray.Data[1][0]!=NULL_IC ||
                   MOT[MOutRotateKit].Tray.Data[0][1]!=NULL_IC ||
                   MOT[MOutRotateKit].Tray.Data[1][1]!=NULL_IC)
                {
                    iRetryCT=0;
                    Task=4000;
                }
                else
                {
                    // Task=5000; //jou 2014-03-18 Task=5000->4400; kit=0 關site不需要旋轉,kit=1沒有檢查就return true
                    Task=4400;
                    break;
                }
            }
            break;
        case 4000:
            if(M_MoveOutArmXY_ToRotateKIT(iKit)==true)
            {
                CheckOutArmSuckICFallDown();                                    //Steven 20110516 : 修改成整合式Alarm

                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        OutArmSuck.Suck[i][j].SetNeedSuck(false);
                    }
                }
                Task=4100;
            }
            else
            {
                CheckOutArmSuckICFallDown();                                    //Steven 20110516 : 修改成整合式Alarm
                break;
            }
        case 4100:
            if(bOut_ICRotationCompleteOnKit==false)                             //Sam 20210623 : 修正 IC 轉完後被 Home，導致 IC 轉向異常。
            {
                DoRotate_Out(rCount, true);
                Task=2600;
                break;
            }

            if(M_MoveOutArmZ_ToRotateKIT_Pick(iKit, Prod.OutRotationCount[rCount-1])==true)  //kevin 20131003  更改OUTARM相對位置
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualStep].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("M_DoOutArmRotateKIT_Motor 4100");
                    Task=4500;
                    break;
                }

                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                Task=4200;
            }
            else
            {
                break;
            }
        case 4200:
            flag=true;

            if(iRotate_Type==e1MotRotate1Dut)                               //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                {
                    iTrayY=0;
                    iTrayX=0;
                    iSuckR=iRotato_Out_Row;
                    iSuckC=j;
                    iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                    iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                    if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                    {
                        bSuckDuplicateErr[iSuckR][iSuckC]=false;
                        OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                    }
                    else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                            CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                    {
                        if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                        {
                            flag=false;
                        }
                    }
                }
            }
            else if(iRotate_Type==e2MotRotate2Dut)                              //wei 20190515 modify 2 Dut 2 Motor
            {
                if(InArmSuck.iPickCol==1)
                {
                    for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                    {
                        iTrayY=0;
                        iTrayX=0;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j;
                        iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                        iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                    }
                }
                else if(InArmSuck.iPickCol==2)
                {
                    for(int j=0; j<2; j++)
                    {
                        iTrayY=0;
                        iTrayX=j;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j;
                        iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                        iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                    }
                }
                else
                {
                    for(int j=0; j<2; j++)
                    {
                        iTrayY=0;
                        iTrayX=j;
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j*2+iKit;
                        iRealR=OutArmSuck.Suck[iSuckR][iSuckC].iMyRow;
                        iRealC=OutArmSuck.Suck[iSuckR][iSuckC].iMyCol;
                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                CheckOutRotateAnglePostion(iRealR, iRealC, iTrayX, iTrayY)==true)
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                    }
                }
            }
            else if(tRotate.DutNum==tDutType_8)                                      //Steven 20160615 : fixed for 2x4_4 wirh rotate
            {
                if((TestIF.iTestMode==QualSite1X4 && TestIF.iUseSuckMode==4) || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
                {                                                               //jou 2015-10-28 新增 1x4 8吸嘴 Rotate 模式
                    for(int j=0; j<iMAX_ARM_Col; j++)
                    {
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        //==>
                        iSuckR=0;
                        if(tRotate.DutNum==tDutType_8)                          //jou 2015-10-14 fix 1x4 rotate dut8 hang up
                            iSuckC=j;
                        else
                            iSuckC=j*i2x2Suck_Out+iKit;

                        iTrayX=j;
                        iTrayY=iKit;

                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC)
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                        //<==
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                    }
                }
                else if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
                {
                    for(int j=iKit; j<MAX_ARM_Col; j+=4)
                    {
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        //==>
                        iSuckR=iRotato_Out_Row;
                        iSuckC=j*2;

                        if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)
                        {   //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                            iTrayX=1;
                            iTrayY=0;
                        }
                        else
                        {
                            iTrayX=j+1;
                            iTrayY=iKit;
                        }

                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)        //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                        //<==
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                    }
                }
                else
                {
                    for(int i=0; i<iMAX_ARM_Row; i++)
                    {
                        for(int j=0; j<iMAX_ARM_Col; j++)
                        {
                            //Steven 20170504 (wei) : 修改Rotate資料交換方式
                            //==>
                            iSuckR=i;
                            iSuckC=j;

                            iTrayX=j;
                            iTrayY=i;

                            if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                    CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)        //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                            {
                                if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                                {
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                                }
                                else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                                {
                                    flag=false;
                                }
                            }
                            //<==
                            //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        }
                    }
                }
            }
            else
            {
                //jou 2015-10-28 新增 1x4 8吸嘴 Rotate 模式
                if((TestIF.iTestMode == QualSite1X4 && TestIF.iUseSuckMode==4) || TestIF.iTestMode==_8Site1X4)  //ChungHung 20150528 add for 海思 _8Site1x4
                {
                    for(int j=0; j<iMAX_ARM_Col; j++)
                    {
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                        //==>
                        iSuckR=0;
                        iSuckC=j*i2x2Suck_Out+iKit;

                        iTrayX=j;
                        iTrayY=iKit;
                        if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                        {
                            bSuckDuplicateErr[iSuckR][iSuckC]=false;
                            OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                        }
                        else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)        //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                        {
                            if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                            {
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                            }
                            else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                            {
                                flag=false;
                            }
                        }
                        //<==
                        //Steven 20170504 (wei) : 修改Rotate資料交換方式
                    }
                }
                else
                {
                    //Steven 20170329 (wei) : Add individual rotate motor
                    //==>
                    if(USE_ROTATE_KIT==1 && (iRotate_Type==e4MotRotate || iRotate_Type==e8MotRotate || iRotate_Type==e2MotRotate2Dut))  //wei 20190515 modify 2 Dut 2 Motor
                    {
                        for(int i=0; i<2; i++)
                        {
                            for(int j=0; j<2; j++)
                            {
                                iSuckR=i;
                                iSuckC=j*i2x2Suck_Out+iKit;

                                iTrayX=j;
                                iTrayY=i;
                                if(iRotate_Type==e2MotRotate2Dut && TestIF.iTestMode==SingleSite)   //wei 20190515 modify 2 Dut 2 Motor
                                {
                                    iSuckR=0;
                                    //iSuckC=3;
                                    iSuckC=0;   //Sam 20250618 : 修正 Single Rotate 錯誤
                                    iTrayX=1;
                                    iTrayY=0;
                                }

                                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                                {
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                                }
                                else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                        CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)        //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                                {
                                    if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                                    {
                                        bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                        OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                                    }
                                    else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                                    {
                                        flag=false;
                                    }
                                }
                            }
                        }
                    }
                    //<==
                    //Steven 20170329 (wei) : Add individual rotate motor
                    else
                    {
                        for(int i=0; i<iMAX_ARM_Row; i++)
                        {
                            for(int j=0; j<iMAX_ARM_Col; j++)
                            {
                                //Ifor 20180110 (Steven) : add 僅有一個Rotate sensor
                              /*  if(bSingleDut || TestIF.iTestMode==SingleSite)  //Ifor 20201028 add: 避免Single Site 吸Rotate位置異常
                                {
                                    iSuckR=0;
                                    //iSuckC=3;
                                    iSuckC=0;   //Sam 20250618 : 修正 Single Rotate 錯誤
                                    iTrayX=0;
                                    iTrayY=0;
                                    if(i!=0 || j!=0)                            //Steven 20190809 : Fixed for hang up
                                        continue;
                                }
                                else     */
                                {
                                    //Steven 20170504 : 修改Rotate資料交換方式
                                    //<==
                                    iSuckR=i;
                                    iSuckC=j*i2x2Suck_Out+iKit;

                                    iTrayX=j;
                                    iTrayY=i;
                                }
                                if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]==HAS_NULL_IC)
                                {
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                                }
                                else if(MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY]!=NULL_IC &&
                                        CheckRotateOutAnglePostion(iTrayY, iTrayX, rCount-1, 0)==true)        //Ifor 20170412 (Steven) Modify 角度已算過不需在減掉原本角度
                                {
                                    if(OutArmSuck.Suck[iSuckR][iSuckC].Suck())
                                    {
                                        bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                        OutArmSuck.CopyFromTray(iSuckR, iSuckC, MOT[MOutRotateKit].Tray.Data[iTrayX][iTrayY], MOT[MOutRotateKit], iTrayY, iTrayX, NULL_IC);
                                    }
                                    else if(OutArmSuck.Suck[iSuckR][iSuckC].Error==false)
                                    {
                                        flag=false;
                                    }
                                }
                                //<==
                                //Steven 20170504 : 修改Rotate資料交換方式
                            }
                        }
                    }
                }
            }

            if(flag==false)                                                     //kevin 20130517
                break;

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        iRetryCT++;
                        Task=4600;
                        return false;
                    }
                }
            }

            rCount--;
            iRetryCT=0;
            Task=4300;
            break;
        case 4300:
            if(MoveOutArmToAutoSafe())
            {
                if(FrmRotate->bShowRotateBySite==true)                          //wei 20190515 modify 2 Dut 2 Motor
                {
                    Task=4400;
                }
                else
                {
                    if(rCount>0)
                        Task=2600;
                    else
                        Task=4400;
                }
            }
            break;
        case 4400:
            if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                if(MOT[MOutRotateKit].Tray.Data[0][0]==NULL_IC)
                    Task=5000;
                else
                    Task=2600;
            }
            else if(iRotate_Type==e2MotRotate2Dut)                              //wei 20190515 modify 2 Dut 2 Motor
            {
                if(MOT[MOutRotateKit].Tray.Data[0][0]==NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[1][0]==NULL_IC)
                    Task=5000;
                else
                    Task=2600;
            }
            else if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
            {
                if(tRotate.DutNum==tDutType_8)
                {
                    if(MOT[MOutRotateKit].Tray.Data[1][0]==NULL_IC && MOT[MOutRotateKit].Tray.Data[2][1]==NULL_IC)
                    {
                        iKit=0;
                        Task=5000;
                    }
                    else
                    {
                        Task=2550;
                    }
                }
                else
                {
                    if(MOT[MOutRotateKit].Tray.Data[1][0]==NULL_IC && MOT[MOutRotateKit].Tray.Data[1][1]==NULL_IC)
                    {
                        iKit=0;
                        Task=5000;
                    }
                    else
                    {
                        Task=2600;
                    }
                }
            }
            else if(iKit==0)
            {
                iKit++;
                if((TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4 || TestIF.iTestMode==_16Site2X8 || (TestIF.iTestMode==QualSite2X2 && TestIF.iUseSuckMode==8)) && ArmCanSuck4IC(0, true)==true)  //Sam 20190226 : 16Site4X4 //JerryYang 20170606 (Steven) 獨立rotate 支援2x2 8吸嘴模式
                {
                    if(tRotate.DutNum==tDutType_4)                              //Steven 20140207 : 修正Rotate吸八顆放四顆
                        Task=10;                                                //kevin 20130629
                    else                                                        //己做完所有旋轉
                        Task=5000;
                }
                else
                {
                    Task=3100;
                }
            }
            else
            {
                iKit=0;
                Task=5000;
            }
            break;
        case 5000:
            Task=6000;
        case 6000:
            if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                Task=GotOutRotateKitForSingle(iKit, iOutCount, false);
                if(Task==1)
                    return true;
            }
            else if(iRotate_Type==e2MotRotate2Dut)                              //wei 20190515 modify 2 Dut 2 Motor
            {
                if(InArmSuck.iPickCol==1)
                {
                    Task=GotOutRotateKitForSingle(iKit, iOutCount, false);
                }
                else if(bUseAxxGPicker() ||
                        iCloseSiteModeFor1x4==e1x4CloseAbAc)                    //Steven 20241111 : for 1x4 close 2 site
                {
                    Task=GotOutRotateKitForAxxG(iKit, iOutCount, false);
                }
                else
                {
                    Task=GotOutRotateKitForAxEx_2RotMot(iKit, iOutCount, false);
                }

                if(Task==1)
                    return true;
            }
            else if(tRotate.DutNum==tDutType_4)
            {
                iKit=0;
                MOT[MOutRotateKit].SetHasNullIcToNullIc();                      //Frank 20170822 (wei): 清空Rotate資料
                return true;
            }
            else if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)    //Steven 20201028 : for rotator
            {
                if(TestIF.iTestMode==QualSite2X2 && DeviceForm.XDimension>=5000)     //wei 20151229  Xilinx 52.5x52.5 放一個Rotate  //Alick 20160105 (wei) modify IC>=50x50 放一個Rotate
                {
                    if(iRotato_Out_Row==0)
                    {
                        if(iOutCount==0)
                        {
                            iKit=1;
                            iOutCount=1;
                        }
                        else
                        {
                            iOutCount=0;
                            iRotato_Out_Row=1;
                            iKit=0;
                        }
                        Task=10;
                    }
                    else
                    {
                        if(iOutCount==0)
                        {
                            iKit=1;
                            iOutCount=1;
                            Task=10;
                        }
                        else
                        {
                            iOutCount=0;
                            iRotato_Out_Row=0;
                            iKit=0;
                            MOT[MOutRotateKit].SetHasNullIcToNullIc();          //Frank 20170822 (wei): 清空Rotate資料
                            return true;
                        }
                    }
                }
                else
                {
                    if(iRotato_Out_Row==0)
                    {
                        iRotato_Out_Row=1;
                        iKit=0;
                        Task=10;
                    }
                    else
                    {
                        iRotato_Out_Row=0;
                        iKit=0;
                        MOT[MOutRotateKit].SetHasNullIcToNullIc();              //Frank 20170822 (wei): 清空Rotate資料
                        return true;
                    }
                }
            }
            else
            {
                MOT[MOutRotateKit].SetHasNullIcToNullIc();                      //Frank 20170822 (wei): 清空Rotate資料
                return true;
            }
            iKit=0;
            break;
        case 4500:                                                              //(1, 3)調整Offset
            if(MoveOutArmToAutoSafe())
            {
                bEnterOffset=false;
                Task=4000;
            }
            break;
        case 4600:
            if(MoveOutArmToAutoSafe())
            {
                if(iRetryCT>ArmSpeed[OutArm].iRetryCT)                    //Sam 20250206 : Rotate pick error retry for setting
                {
                    ErrPart=" ";
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(bSuckDuplicateErr[i][j])
                                bHasDuplicateErr=true;
                            if(OutArmSuck.Suck[i][j].Error)
                            {
                                ErrPart+=OutArmSuck.Suck[i][j].sName;
                                bSuckDuplicateErr[i][j]=true;
                                OutArmSuck.Suck[i][j].Error=false;
                            }
                            else
                            {
                                bSuckDuplicateErr[i][j]=false;
                            }
                        }
                    }
                    ret=ShowErrorMessage("JAM0210", K_RETRY|K_SKIP, MOutArmX, bHasDuplicateErr, ErrPart);     //Sam 20250204 : Add Skip
                }
                else
                {
                    ret=K_RETRY;
                }

                if(ret==K_SKIP)
                {
                    if(iRotate_Type==e1MotRotate1Dut)                                   //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
                    {
                        for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                        {
                            iTrayY=0;
                            iTrayX=0;
                            iSuckR=iRotato_Out_Row;
                            iSuckC=j;
                            if(bSuckDuplicateErr[iSuckR][iSuckC])
                            {
                                OutArmSuck.PordRec[iSuckR][iSuckC].AddErrorRecord("JAM0210");
                                MOT[MOutRotateKit].SetTraySingleData(iTrayX, iTrayY, NULL_IC);
                                bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                OutArmSuck.SetItemData(iSuckR, iSuckC, HAS_NULL_IC);
                            }
                        }
                    }
                    else if(iRotate_Type==e2MotRotate2Dut)                              //wei 20190515 modify 2 Dut 2 Motor
                    {
                        if(OutArmSuck.iPickCol==1)
                        {
                            for(int j=iKit; j<OutArmSuck.iMaxCol; j+=4)
                            {
                                iTrayY=0;
                                iTrayX=0;
                                iSuckR=iRotato_Out_Row;
                                iSuckC=j;
                                if(bSuckDuplicateErr[iSuckR][iSuckC])
                                {
                                    OutArmSuck.PordRec[iSuckR][iSuckC].AddErrorRecord("JAM0210");
                                    MOT[MOutRotateKit].SetTraySingleData(iTrayX, iTrayY, NULL_IC);
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.SetItemData(iSuckR, iSuckC, HAS_NULL_IC);
                                }
                            }
                        }
                        else if(OutArmSuck.iPickCol==2)
                        {
                            for(int j=0; j<2; j++)
                            {
                                iTrayY=0;
                                iTrayX=0;
                                iSuckR=iRotato_Out_Row;
                                iSuckC=j;
                                if(bSuckDuplicateErr[iSuckR][iSuckC])
                                {
                                    OutArmSuck.PordRec[iSuckR][iSuckC].AddErrorRecord("JAM0210");
                                    MOT[MOutRotateKit].SetTraySingleData(iTrayX, iTrayY, NULL_IC);
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.SetItemData(iSuckR, iSuckC, HAS_NULL_IC);
                                }
                            }
                        }
                        else
                        {
                            for(int j=0; j<2; j++)
                            {
                                iTrayY=0;
                                iTrayX=0;
                                iSuckR=iRotato_Out_Row;
                                iSuckC=j*2+iKit;
                                if(bSuckDuplicateErr[iSuckR][iSuckC])
                                {
                                    OutArmSuck.PordRec[iSuckR][iSuckC].AddErrorRecord("JAM0210");
                                    MOT[MOutRotateKit].SetTraySingleData(iTrayX, iTrayY, NULL_IC);
                                    bSuckDuplicateErr[iSuckR][iSuckC]=false;
                                    OutArmSuck.SetItemData(iSuckR, iSuckC, HAS_NULL_IC);
                                }
                            }
                        }
                    }
                    else if(iInArmType==e9045_1x2_2_14 || iInArmType==e9045_2x2_4_14 || iInArmType==e9045_2x4_4_14)     //Steven 20201028 : for rotator
                    {
                        for(int j=iKit; j<MAX_ARM_Col; j+=4)                    //jou 2015-07-03 修正 Rotate 2x2 Pitch>3750 Pick up error SKIP NG
                        {
                            int i=iRotato_Out_Row;
                            j2=j*2;
                            if(bSuckDuplicateErr[i][j2])
                            {
                                OutArmSuck.PordRec[i][j2].AddErrorRecord("JAM0210");     //Steven 20161214 : Add Jam Record
                                MOT[MOutRotateKit].SetTraySingleData(j+1, iKit, NULL_IC);    //kevin 20130524
                                bSuckDuplicateErr[i][j2]=false;                 //kevin 20130524
                                OutArmSuck.SetItemData(i, j2, HAS_NULL_IC);     //Steven 20161116 : NULL_IC --> HAS_NULL_IC
                            }
                        }
                    }
                    else
                    {
                        for(int i=0; i<iMAX_ARM_Row; i++)                       //kevin 20130517
                        {
                            for(int j=0; j<iMAX_ARM_Col; j++)                   //kevin 20130517
                            {
                                if(tRotate.DutNum == tDutType_8)                //kevin 20130517 add 8 site
                                    j2=j;
                                else
                                    j2=j*i2x2Suck+iKit;

                                if(bSuckDuplicateErr[i][j2])                    //kevin 20140220
                                {
                                    OutArmSuck.PordRec[i][j2].AddErrorRecord("JAM0210");     //Steven 20161214 : Add Jam Record
                                    MOT[MOutRotateKit].SetTraySingleData(j, i, NULL_IC);    //kevin 20130524
                                    bSuckDuplicateErr[i][j2]=false;             //kevin 20130524
                                    OutArmSuck.SetItemData(i, j2, HAS_NULL_IC); //Steven 20161116 : NULL_IC --> HAS_NULL_IC
                                }
                            }
                        }
                    }

                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            OutArmSuck.Suck[i][j].Error=false;
                            bSuckDuplicateErr[i][j]=false;
                        }
                    }
                    Task=4300;
                    break;
                }
                Task=4000;
                if(ret==K_RETRY && iRetryCT>ArmSpeed[OutArm].iRetryCT)   //Sam 20250206 : Rotate pick error retry for setting
                {
                    Task=4700;
                }
            }
            break;
        case 4700:
            SetOutArmHome();
            iRetryCT=0;
            Task =4000;
            break;
        // --------------------------------------
        case 4800:
            if(MoveOutArmXY_ToShuttleAlarmArea()==false)                        //如果要移開,而且還沒移到定位
            {
                return false;                                                   //先離開等In Arm
            }
            Task=4801;
            break;
        case 4801:                                                              //旋轉失敗
            ret=ShowErrorMessage("JAM0553", K_RETRY, MOutRotateKit);            //kevin 20130524
            if(ret==K_RETRY)
            {
                Task=4810;
            }
            break;
        case 4810:
            FrmRotate->bRotateOutHome=true;                                     //kevin 20130706
            FrmRotate->InitialOutRotateHome();                                  //Steven 20170329 (wei) : Add individual rotate motor
            Task=4820;
            break;
        case 4820:
            if(FrmRotate->bRotateOutHome==false)                                //kevin 20130706
            {
                FrmRotate->SetOutRotateSpeed(ArmSpeed[OutRotate].iACDCBodySP, ArmSpeed[OutRotate].iBodySP);    //Steven 20170329 (wei) : Add individual rotate motor
                Task=2600;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool M_DoShakeOutArmRotateKIT()                                                 //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm
{
    int &iTask=iShakeOutArmRotateKit;
    int ret, iShakeLimitNum=3;

    if(USE_ROTATE_KIT==1 && iRotate_Type==e1MotRotate)
    {
        switch(iTask)
        {
            case 1:
                ret=SetMotorResolution(OutAngle45, OutAngle90, false);          //Rotate旋轉功能,反轉4.5度   //Ifor 20170629 (Steven) fix Out Rotate Err
                ret=ret-100;
                if(MOT[MOutRotateKit].MotorMove(ret))
                    iTask=10;
                break;
            case 10:
                ret=SetMotorResolution(OutAngle45, OutAngle90, false);          //Rotate旋轉功能,正轉4.5度    //Ifor 20170629 (Steven) fix Out Rotate Err
                ret=ret+100;
                if(MOT[MOutRotateKit].MotorMove(ret))
                {
                    iOutArmShakeCnt++;
                    if(iOutArmShakeCnt>=iShakeLimitNum)                         //完成旋轉三次
                    {
                        iOutArmShakeCnt=0;
                        iTask=20;
                    }
                    else
                    {
                        iTask=1;                                                //還沒轉完要繼續轉
                    }
                }
                break;
            case 20:
                ret=SetMotorResolution(OutAngle45, OutAngle90, false);          //回到原本的角度     //Ifor 20170629 (Steven) fix Out Rotate Err
                if(MOT[MOutRotateKit].MotorMove(ret))
                    return true;
        }
    }
    else
    {
        //Steven 20170329 (wei) : Add individual rotate motor
        //==>
        switch(iTask)
        {
            case 1:
                MoveOutRotateToDegreeAtSameTime(0, true);
                iTask=10;
                break;
            case 10:
                ret=MoveOutRotateToDegreeAtSameTime(0);
                if(ret)
                {
                    iOutArmShakeCnt++;
                    if(iOutArmShakeCnt>=iShakeLimitNum)                         //完成旋轉三次
                    {
                        iOutArmShakeCnt=0;
                        return true;
                    }
                    else
                    {
                        iTask=20;                                               //還沒轉完要繼續轉
                    }
                }
                break;
            case 20:
                MoveOutRotateToDegreeAtSameTime(0, true);
                iTask=30;
            case 30:
                ret=MoveOutRotateToDegreeAtSameTime(90);
                if(ret)
                    iTask=1;
                break;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
int DoRotate_Out(int &rCount, bool bReset)
{
    int iRet=0;

    if(USE_ROTATE_KIT==1)
    {
        if(iRotate_Type==e1MotRotate ||
           iRotate_Type==eInOutArm1Motor)
        {
            iRet=M1_DoOutRotateMove(rCount, bReset);
        }
        else if(iRotate_Type==e1MotRotate1Dut)                                  //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
        {
            iRet=M1_DoOutRotateMove1Mot(rCount, bReset);
        }
        else if(iRotate_Type==e2MotRotate2Dut)
        {
            iRet=M2_DoOutRotateMove(rCount, bReset);
        }
        else if(iRotate_Type==e4MotRotate)
        {
            iRet=M4_DoOutRotateMove(rCount, bReset);
        }
        else if(iRotate_Type==e8MotRotate)
        {
            iRet=M8_DoOutRotateMove(rCount, bReset);
        }
    }
    else
    {
        iRet=1;
    }

    return iRet;
}
//---------------------------------------------------------------------------
int M1_DoOutRotateMove(int &rCount, bool bReset)
{
    int iResult         =0;
    int iAngle          =0;
    int iPos            =0;
    int iBacklash       =0;
    int iNowPos         =0;
    static bool bDoHomeFlag=false;
//    int iCount          =0;
//    static bool QQ      =false;

    if(bReset)
    {
        rCount=Prod.RotationTimeOut;                                            //Ifor 20170412 (Steven) RotationTime ==> RotationTimeOut   //jou 20231122 : Rotate Use RT mode
        CheckOutRotateSensor(true);
//        QQ=false;
        return iResult;
    }

//    iCount=CheckOutRotateSensor();
//    if(iCount==-100)
//    {
//        QQ=true;
//    }

    //jou 20231122 : Rotate Use RT mode
    if(Prod.OutRotationCount[rCount-1]!=0)                                      //20140318    如果設為大於0才進入的話-90度會不會旋轉
    {
        iAngle=(int)Prod.OutRotationCount[rCount-1]/90;                         //Ifor 20170407 已換算好角度直接/90即可
    }
    //Ifor 20211220 add:旋轉角度270度時改反轉減少旋轉時間
    if(iAngle==3)
        iAngle=-1;
    else if(iAngle==-3)
        iAngle=1;

    iPos=(OutAngle90*iAngle)+Prod.iOut_iRotateA;  //kevin 20130415
    iBacklash=MOT[MOutRotateKit].GetRotatorBacklash(iPos, false);               //RogerYang 20260113 : Rotator新增背隙補償
    iPos+=iBacklash;

    if(iRotate_Type==eInOutArm1Motor && iAngle!=0)                                           //Ifor 20251204 add:rotate Offset
    {
        iPos=iPos+tRotate.iRotateOffset[1];
    }

    if(MOT[MOutRotateKit].MotorMove(iPos))
    {
        int btest=MOT[MInRotateKit].iLastRotatorDirP;                           //RogerYang 20260113 : Rotator新增背隙補償
        if(iRotate_Type==eInOutArm1Motor)
            iResult=1;
        else
            iResult=CheckOutRotateSensor();
    }

//    if(iResult==1 && QQ==true)
//    {
//        iResult=-100;
//    }
    return iResult;
}
//---------------------------------------------------------------------------
int M1_DoOutRotateMove1Mot(int &rCount, bool bReset)
{
    int iResult =0;
    int iAngle  =0;
    int iPos    =0;

    if(bReset)
    {
        return iResult;
    }

    if(MOT[MOutRotateKit].Tray.Data[0][0]!=NULL_IC &&
       MOT[MOutRotateKit].Tray.Data[0][0]!=HAS_NULL_IC &&
       MOT[MOutRotateKit].Tray.iNeedRotAng[0][0]!=MOT[MOutRotateKit].Tray.iCurrRotAng[0][0])
        iAngle=MOT[MOutRotateKit].Tray.iNeedRotAng[0][0]/90;
    else
        iAngle=MOT[MOutRotateKit].Tray.iCurrRotAng[0][0]/90;

    iPos=(OutAngle90*iAngle)+Prod.iOut_iRotateA;
    rCount=iAngle;
    iResult=MOT[MOutRotate[0][0]].MotorMove(iPos);
    if(iResult!=0)
    {
        MOT[MOutRotateKit].Tray.iCurrRotAng[0][0]=MOT[MOutRotateKit].Tray.iNeedRotAng[0][0];
        iResult=CheckOutRotateSensor();                                         //Steven 20170504 (wei) : 針對入料旋轉站的Sensor檢查
    }
    return iResult;
}
//---------------------------------------------------------------------------
//V3.27C Steven 20170504 (wei) : For 2顆獨立旋轉馬達同時轉動
//---------------------------------------------------------------------------
int M2_DoOutRotateMove(int &rCount, bool bReset)
{
    static bool bM8Result[2][4]={false};
    int j2;
    int iResult =1;
    int iAngle  =0;
    int iPos    =0;
    int iCol    =2;

    if(bReset)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bM8Result[i][j]=false;
            }
        }
        return iResult;
    }

    if(InArmSuck.iPickCol==1)
    {
        int i=0;
        int j=0;
        j2=2;

        if(bM8Result[i][j2]==false)
        {
            if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC &&
               MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC &&
               MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]!=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i])
                iAngle=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]/90;
            else
                iAngle=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]/90;

            iPos=(OutAngle90*iAngle)+Tech.iOutRotate[i][j2+1];
            bM8Result[i][j2]=MOT[MOutRotate[i][j2]].MotorMove(iPos);
            if(bM8Result[i][j2]==false)
            {
                iResult=0;
            }
            else
            {
                MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i];
            }
        }
    }
    else
    {
        for(int i=0; i<1; i++)
        {
            for(int j=0; j<iCol; j++)
            {
                j2=j*2;

                if(bM8Result[i][j2]==false)
                {
                    if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC &&
                       MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC &&
                       MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]!=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i])
                        iAngle=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]/90;
                    else
                        iAngle=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]/90;

                    iPos=(OutAngle90*iAngle)+Tech.iOutRotate[i][j2+1];
                    bM8Result[i][j2]=MOT[MOutRotate[i][j2]].MotorMove(iPos);
                    if(bM8Result[i][j2]==false)
                    {
                        iResult=0;
                    }
                    else
                    {
                        MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i];
                    }
                }
            }
        }
    }

    return iResult;
}
//---------------------------------------------------------------------------
//V3.27C Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
//---------------------------------------------------------------------------
int M4_DoOutRotateMove(int &rCount, bool bReset)
{
    static bool bM8Result[2][4]={false};
    int j2;
    int iResult =1;
    int iAngle  =0;
    int iPos    =0;
//    static bool QQ      =false;
//    int iCount          =0;

    if(bReset)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bM8Result[i][j]=false;
            }
        }
        CheckOutRotateSensor(true);
//        QQ=false;
        return iResult;
    }

//    iCount=CheckOutRotateSensor();
//    if(iCount==-100)
//    {
//        QQ=true;
//    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            j2=j*2;
            if(bM8Result[i][j2]==false)
            {
                if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC &&
                   MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]!=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i])
                    iAngle=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]/90;
                else
                    iAngle=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]/90;

                iPos=(OutAngle90*iAngle)+Tech.iOutRotate[i][j2];

                bM8Result[i][j2]=MOT[MOutRotate[i][j2]].MotorMove(iPos);
                if(bM8Result[i][j2]==false)
                {
                    iResult=0;
                }
                else
                {
                    MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i];
                }
            }
        }
    }

    if(iResult!=0)
    {
        iResult=CheckOutRotateSensor();                                         //Steven 20170504 (wei) : 針對入料旋轉站的Sensor檢查
    }

//    if(iResult==1 && QQ==true)
//    {
//        iResult=-100;
//    }

    return iResult;
}
//---------------------------------------------------------------------------
//V3.27C Steven 20170504 (wei) : For 8顆獨立旋轉馬達同時轉動
//---------------------------------------------------------------------------
int M8_DoOutRotateMove(int &rCount, bool bReset)
{
    static bool bM8Result[2][4]={false};
    int j2;
    int iResult =1;
    int iAngle  =0;
    int iPos    =0;
//    static bool QQ      =false;
//    int iCount          =0;

    if(bReset)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bM8Result[i][j]=false;
            }
        }
        CheckOutRotateSensor(true);
//        QQ=false;
        return iResult;
    }

//    iCount=CheckOutRotateSensor();
//    if(iCount==-100)
//    {
//        QQ=true;
//    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            j2=j;
            if(bM8Result[i][j2]==false)
            {
                if(MOT[MOutRotateKit].Tray.Data[j][i]!=NULL_IC &&
                   MOT[MOutRotateKit].Tray.Data[j][i]!=HAS_NULL_IC &&
                   MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]!=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i])
                    iAngle=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i]/90;
                else
                    iAngle=MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]/90;

                iPos=(OutAngle90*iAngle)+Tech.iOutRotate[i][j2];

                bM8Result[i][j2]=MOT[MOutRotate[i][j2]].MotorMove(iPos);
                if(bM8Result[i][j2]==false)
                {
                    iResult=0;
                }
                else
                {
                    MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=MOT[MOutRotateKit].Tray.iNeedRotAng[j][i];
                }
            }
        }
    }

    if(iResult!=0)
    {
        iResult=CheckOutRotateSensor();  //Steven 20170504 (wei) : 針對入料旋轉站的Sensor檢查
    }

//    if(iResult==1 && QQ==true)
//    {
//        iResult=-100;
//    }

    return iResult;
}
//---------------------------------------------------------------------------
//V3.27C.523 Steven 20170504 (wei) : 針對獨立馬達旋轉站, 一起旋轉到特定角度
//---------------------------------------------------------------------------
bool MoveOutRotateToDegreeAtSameTime(int iDegree, bool bInitial)
{
    static bool bMoveFinish[2][4];
    int buffer, j2;
    bool bResult=true;

    if(bInitial)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bMoveFinish[i][j]=false;
            }
        }
        return bResult;
    }

    if(USE_ROTATE_KIT==1 &&
       (iRotate_Type==e1MotRotate ||
        iRotate_Type==e1MotRotate1Dut ||
        iRotate_Type==eInOutArm1Motor))                                         //add One sucker with rotate
    {
//        buffer=SetMotorResolution(OutAngle90, OutAngle90, true);
        buffer=SetMotorResolution(OutAngle90, OutAngle90, false);               //Ifor 20170629 (Steven) fix Out Rotate Err
        if(MOT[MOutRotateKit].MotorMove(buffer))                                //旋轉成和shuttle一樣方向
        {
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=iDegree;
                }
            }
        }
        else
        {
            bResult=false;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(USE_ROTATE_KIT==1 && iRotate_Type==e4MotRotate)
                {
                    j2=j*2;
                    if(j>=2)
                    {
                        continue;
                    }
                }
                else if(USE_ROTATE_KIT==1 && iRotate_Type==e2MotRotate2Dut)     //wei 20190515 modify 2 Dut 2 Motor
                {
                    j2=j*2;
                    if(j>=2)
                    {
                        continue;
                    }
                    if(i>=1)
                    {
                        continue;
                    }
                }
                else
                {
                    j2=j;
                }

                if(bMoveFinish[i][j]==false)
                {
                    if(USE_ROTATE_KIT==1 && iRotate_Type==e1MotRotate)
                        buffer=Tech.iOutRotate[i][j2]+2000*iDegree;
                    else if(USE_ROTATE_KIT==1 && iRotate_Type==e2MotRotate2Dut)
                        buffer=Tech.iOutRotate[i][j2+1]+800*iDegree;
                    else
                        buffer=Tech.iOutRotate[i][j2]+800*iDegree;
                    if(MOT[MOutRotate[i][j2]].ReadPos()!=buffer)
                    {
                        bResult=false;
                        bMoveFinish[i][j]=MOT[MOutRotate[i][j2]].MotorMove(buffer);
                    }
                    else
                    {
                        bMoveFinish[i][j]=true;
                    }
                }

                if(bMoveFinish[i][j]==true)
                {
                    MOT[MOutRotateKit].Tray.iCurrRotAng[j][i]=iDegree;
                }
            }
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool CheckOutRotateAnglePostion(int iSuckR, int iSuckC, int iTrayX, int iTrayY)
{
    bool bResult=false;
    if(Prod.RotateDutDate[1][iSuckR][iSuckC]==MOT[MOutRotateKit].Tray.iCurrRotAng[iTrayX][iTrayY])
        bResult=true;

    return bResult;
}
//---------------------------------------------------------------------------
//Ifor 20170412 (Steven) add Check Angle 與各Site位置是否相同
//==>
bool CheckRotateOutAnglePostion(int iTrayY, int iTrayX, int iTemp, int iMode)
{
    bool bResult=false;
    int dbTempAngle=0;
    int iRealR, iRealC;

    if(FrmRotate->bShowRotateBySite==true)                                      //Steven 20170329 (wei) : Add individual rotate motor  //wei 20190515 modify 2 Dut 2 Motor
    {
        if(iRotate_Type==e1MotRotate1Dut)
        {
            iRealR=iTrayY;
            iRealC=OutArmSuck.Suck[iTrayY][iTrayX].iMyCol;
            if(Prod.RotateDutDate[1][iRealR][iRealC]==MOT[MOutRotateKit].Tray.iCurrRotAng[0][0])
                bResult=true;
        }
        else if(iRotate_Type==e2MotRotate2Dut && TestIF.iTestMode==SingleSite)
        {
            if(Prod.RotateDutDate[1][0][0]==MOT[MOutRotateKit].Tray.iCurrRotAng[1][0])
                bResult=true;
        }
        else
        {
            if(Prod.RotateDutDate[1][iTrayY][iTrayX]==MOT[MOutRotateKit].Tray.iCurrRotAng[iTrayX][iTrayY])
                bResult=true;
        }
    }
    else
    {
        if(tRotate.bUseDifferentAngle==true)
        {
            dbTempAngle=360-Prod.RotateDutDate[0][iTrayY][iTrayX]+Prod.RotateDutDate[1][iTrayY][iTrayX]-360;
        }
        else
        {
            dbTempAngle=0-Prod.RotateDutDate[0][iTrayY][iTrayX];
        }

        if(iMode==0)
        {
            if(dbTempAngle==Prod.OutRotationCount[iTemp])
                bResult=true;
        }
        else
        {
            if(dbTempAngle==iTemp)
                bResult=true;
        }
    }

    return bResult;
}
//<==
//Ifor 20170412 (Steven) add Check Angle 與各Site位置是否相同
//---------------------------------------------------------------------------
int CheckOutRotateSensor(bool bReset)
{
    static int iCount=0;
    int iResult=0;

    if(bReset)
    {
        iCount=0;
        return iResult;
    }

    iCount++;
    if(Sen[SnRotateRowOut1].Enable==true &&
           Sen[SnRotateRowOut2].Enable==true &&
           Sen[SnRotateRowOut1].IsOn()       &&
           Sen[SnRotateRowOut2].IsOn()       )
    {
        iResult=1;
        iCount=0;
    }
    else if(Sen[SnRotateRowOut1].Enable==true    &&
            Sen[SnRotateRowOut2].Enable==false   &&
            Sen[SnRotateRowOut1].IsOn()          )
    {
        iResult=1;
        iCount=0;
    }
    else if(Sen[SnRotateRowOut1].Enable==false)
    {
        iResult=1;
        iCount=0;
    }
    else
    {
        iCount++;
        if(iCount>50)
        {
            iCount=0;
            iResult=-999;     //Alarm
        }

//        if(iCount>30)
//        {
//            iResult=-100;     //Alarm
//        }
    }
    return iResult;
}
//---------------------------------------------------------------------------
bool CheckRotateOutNotFinish()
{
    if(USE_ROTATE_KIT==1 && tRotate.ActiveRotate && TrayForm.iRotateKIT_InputType!=0 && (iOutRotateFinish==1)) //kevin 20130524 發生alarm inarm到等待點
    {
        return true;
    }
    else
    {
        return false;
    }
}
