#include "MachineDefine.h"
#pragma hdrstop

#include "aoutarm9045.h"

#include "aArmHeader.h"
#include "main.h"

#include "cSocket.h"
#include "MyKitSuck.h"
#include "SCK_ART.h"
#include "BarCode.h"
#include "OCR.h"
#include "cprod.h"
#include "mymotor.h"
#include "cinitial.h"
#include "note.h"
#include "csystem.h"
#include "mymessbox.h"
#include "cMyDB.h"
#include "mysensor.h"
#include "common.h"
#include "cmydef.h"
#include "mycylin.h"
#include "aRotateKIT.h"
#include "fRotate.h"
#include "fAOI.h"
#include "FixAICCD.h"                                                           //KaiChen 20190508 ：矽格湖口 AI CCD Function
#include "acatchtray.h"                                                         //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
#include "ATC_Handler_Side.h"
#include "main.h"
#include "cSortCT.h"
#include "cmydef.h"
#include "asendic.h"
#include "cTrayMapping.h"                                                       //wei 20161219 (Steven) Tray Mapping
#include "Magazine.h"
#include "ProductionInfo.h"
#include "TesterTCP.h"
#include "aRotateKIT_Out.h"
#include "AGV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool bOutArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
//---------------------------------------------------------------------------
int iBinBoxShiftY=0;                                                            //jou 2012-12-11 support Bin Box
void BinBoxShiftY()                                                             //jou 2012-12-11 support Bin Box
{
    iBinBoxShiftY=0;
}
//==============================================================================
//  Ver : 2003_07
// 輸出臂移到自動 TRAY安全高度
//==============================================================================
bool MoveOutArmToAutoSafe_9045()
{
    bool OK=true;
    int iAlarmSuck=0, iAlarmSuck1=0;
    static int iRetryCount=0;
    static int iRetryFail=0;
    int iMot=0;
    #ifndef SOFT_SIMULTE
    int iZPos=0;
    bool bHomeFlag=false;
    AnsiString sData;
    #endif

    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            iMot=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMot].MotorMove(Prod.ZOutArmSafe[i][j])==false)
            {
                OK=false;
                #ifndef SOFT_SIMULTE
                MOT[iMot].ScanMotorStatus();
                bHomeFlag=MOT[iMot].Led[iHomeLed];
                if(bHomeFlag)                                                   //JerryYang 20200924 : Z軸失步偵測
                {
                   iZPos=MOT[iMot].ReadPos();
                   if(iZPos<-200)
                   {
                        sData.sprintf("%s pos %d home sensor on", MOT[iMot].Alias, iZPos);
                        if(CUSTOMER_CODE==CC_ATEC)                              //Steven 20221224 : 艾科暫時不檢查Z軸Home Sensor
                            MyDBIProcess("Motion", sData, "MoveOutArmToAutoSafe_9045");
                        else
                            ShowMyMessage(sData, "", "MoveOutArmToAutoSafe_9045");
                        SetOutArmHome();
                   }
                }
                #endif
            }
        }
    }

    if(OK)
    {
        iAlarmSuck=OutArmZSafe(DETECT_ALL_FLAG);
        if(iAlarmSuck!=-1)
        {
            if(iRetryCount>50)
            {
                iRetryCount=0;
                if(iRetryFail<3)
                {
                    iRetryFail++;
                    SetOutArmHome();
                }
                else
                {
                    ShowErrorMessage("WAR0257", 0, MOutArmX, false, MOT[iAlarmSuck].NumberAlias);       //出料手臂的吸嘴 %s 沒有在安全位置
                    iRetryFail=0;
                }
                return false;
            }
            else
            {
                iRetryCount++;
                return false;
            }
        }
        iRetryFail=0;
        iRetryCount=0;
    }

    if(OK)
    {
        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                iMot=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
                MOT[iMot].fCMD=false;
            }
        }
    }

    if(OK==true && iAlarmSuck==-1)                                              //Jou 2011-02-09
    {
        iAlarmSuck1=OutArmZSafe(DETECT_SENSOR_FLAG);
        if(iAlarmSuck1==-1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
int GetOutArmPitch_9045(int w)
{
    double m, r;
    m=(double)ChangeToFloatNonPcnt((double)((Prod.iOutArmX120Pitch-Prod.iOutArmX40Pitch)), (double)(iPitch_Max_minus_Min));//Isaac 20171204 (Steven) : Xpitch40->50mm, 新增150mm的Teaching點位，內差法公式計算
    r=(double)Prod.iOutArmX40Pitch+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//------------------------------------------------------------------------------
int GetOutArmPitch2_9045(int w)                                                 //Steven 20131002 : XY變距
{
    double m, r;                                                                //Jerryyang 20180205 float -> double
    m=(double)ChangeToFloatNonPcnt((double)((Prod.iOutArmX120Pitch2-Prod.iOutArmX40Pitch2)), (double)(iPitch_Max_minus_Min));      //Isaac 20171204 (Steven) : Xpitch40->50mm     //Isaac 20171204 (Steven) : Xpitch40->50mm, 新增150mm的Teaching點位，內差法公式計算   //Steven 20200327 : 修正out arm x-pitch的點位錯誤
    r=(double)Prod.iOutArmX40Pitch2+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//------------------------------------------------------------------------------
int GetOutArmPitchY_9045(int w, int iOffsetPos)                                 //Steven 20131002 : XY變距
{
    if(iOffsetPos>=0)                                                           //Jimmychiu 20240731 : for out arm offset value
        w+=OutArmOffSet[iOffsetPos]->GetVariableY();
    double m=0.0, r=0.0;
    m=(double)(Prod.iOutArmY60Pitch-Prod.iOutArmY15Pitch)/(6000-IN_OUT_ARM_Y_PITCH_MIN);
    r=(double)Prod.iOutArmY15Pitch+m*((double)w-(double)IN_OUT_ARM_Y_PITCH_MIN);
    return r;
}
//------------------------------------------------------------------------------
int GetOutArmPitchX_9045(int w, int iX, int iOffsetPos)                         //Steven for HT1032
{
    if(iOffsetPos>=0)                                                           //Jimmychiu 20240731 : for out arm offset value
        w+=OutArmOffSet[iOffsetPos]->GetXPitch(iX);
    double m=0.0, r=0.0;
    double dX120[4]={Prod.iOutArmX120Pitch, Prod.iOutArmX120Pitch2, Prod.iOutArmX120Pitch3, Prod.iOutArmX120Pitch4};
    double dX_40[4]={Prod.iOutArmX40Pitch,  Prod.iOutArmX40Pitch2,  Prod.iOutArmX40Pitch3,  Prod.iOutArmX40Pitch4};
    m=ChangeToFloatNonPcnt((double)(((double)(dX120[iX]-dX_40[iX]))), (double)(iPitch_Max_minus_Min));
    if(USE_PICKER_COUNT==ep16Picker)
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX7));
    else
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX3));
    return r;
}
//-----------------------------------------------------------------------------
void GetOutArmZDownPos_9045(int iWhichAuto, bool bNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col])  //Steven 20230323 : For HT1032
{
    int iR, iC;
    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            iR=OutArmSuck.Suck[i][j].iMyRow;                                    //實體Row   //Steven 20250826 : fixed for out arm offset
            iC=OutArmSuck.Suck[i][j].iMyCol;                                    //實體Col

            if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)
            {
                if(bNeedDown==true)
                {
                    ZDownSel[i][j]=bOutArmSuckActive[i][j];
                }
                else
                {
                    iOutArmXPosition[i][j]=j;
                    bOutArmSuckActive[i][j]=true;
                    ZDownSel[i][j]=false;
                }

                if(ZDownSel[i][j])
                {
                    iZPos[i][j]=Prod.ZPlace[iWhichAuto][iR][iC];
                }
                else
                {
                    iZPos[i][j]=ZSafePos;
                }
            }
            else
            {
                ZDownSel[i][j]=false;
                iZPos[i][j]=ZSafePos;
            }
        }
    }

    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        if(bNeedDown==true)
        {
            iZPos[0][0]=Prod.ZPlace[iWhichAuto][0][0];
        }
        else
        {
            iZPos[0][0]=ZSafePos;
        }
    }
}
//-----------------------------------------------------------------------------
bCheckOutArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col]={{true, true, true, true}, {true, true, true, true}};
bool IsCheckOutArmDestroyActiveFinish(int iXPos, int iYPos)                     //2013-08-02    Dell add Pick 前先確認CheckInArmDestroyActive 已完成
{
    #ifdef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_AMKOR_Japan)                                           //RogerYang 20250813 add
        return true;
    #endif
    int iPosX=0, iPosY=0;
    int iPreRange=100;
    bool bRet=true;
    iPosX=MOT[MOutArmX].ReadPos();
    iPosY=MOT[MOutArmY].ReadPos();

    if(iPosX>=iXPos-iPreRange && iPosX<=iXPos+iPreRange &&
       iPosY>=iYPos-iPreRange && iPosY<=iYPos+iPreRange)
    {
        for(int i=0; i<OutArmSuck.iPickRow; i++)
        {
            for(int j=0; j<OutArmSuck.iPickCol; j++)
            {
                if(bCheckOutArmDestroyActiveFinish[i][j]==false)
                    bRet=false;
            }
        }
    }

    if(bRet==false)                                                             //Steven 20241023 : 為了避免卡死, 先把吸嘴抬起來
    {
        MoveOutArmToAutoSafe();
    }

    return bRet;
}
//------------------------------------------------------------------------------
bool bCheckOutArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col]={{false,false,false,false},{false,false,false,false}};
TQPF_Timer CheckOutArmDestroyActiveDelay[MAX_ARM_Row][MAX_ARM_Col];
void CheckOutArmDestroyActive()                                                 //jou 981130 start : 確認device確實destroy完成
{
    int iPos[MAX_ARM_Row][MAX_ARM_Col];
    static int iTask[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0}, {0, 0, 0, 0}};
    static int iXPos=0, iYPos=0;                                                //kevin 20120514 開吹氣時xy 馬達移動就關吹氣
    int iXpos;
    int iYpos;
    int iMotZ;
    int iWhichAuto, iCol, iRow, iMotor;
    AnsiString Msg;

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            if(bOutArmCheckDestroyACT[i][j]==false)
            {
                iTask[i][j]=1;
                continue;
            }

            switch(iTask[i][j])
            {
                case 1:
                    bCheckOutArmDestroyActiveFinish[i][j]=false;                //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    iMotZ=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
                    iPos[i][j]=MOT[iMotZ].ReadPos();
                    if(iXPos!=MOT[MOutArmX].ReadPos() &&
                       iYPos!=MOT[MOutArmY].ReadPos())                          //kevin 20120514 Xy 移動關吹氣
                    {
                        if(OutArmSuck.Item[i][j]==NULL_IC)                      //Steven 20120507 : 要持續吹氣
                            OutArmSuck.Suck[i][j].OffDestroy();
                    }

                    if(iPos[i][j]>-500)
                    {
                        iTask[i][j]=200;
                        OutArmSuck.Suck[i][j].OffDestroy();
                    }
                    else
                    {
                        if(OutArmSuck.Item[i][j]==NULL_IC)                      //Steven 20120507 : 要持續吹氣
                        {
                            if(ArmSpeed[InArm].bSuckOnDown)                     //jou 2012-10-29 大IC才持續吹氣
                                OutArmSuck.Suck[i][j].OnDestroy();
                            iXPos=MOT[MOutArmX].ReadPos();
                            iYPos=MOT[MOutArmY].ReadPos();
                        }
                    }
                    break;
                case 200:
                    OutArmSuck.Suck[i][j].OnSuck();
                    CheckOutArmDestroyActiveDelay[i][j].SetSecAndOn(ArmSpeed[OutArm].dDestroyCheckTime); //2013-08-01    Dell    modify  公司統一回吸功能
                    if(ArmSpeed[OutArm].bSuckOnDown==false ||
                       ArmSpeed[OutArm].bDestroyPauseCheck)                     //2013-08-01    Dell    modify  公司統一回吸功能
                    {
                        bCheckOutArmDestroyActive[i][j]=true;                   //Steven 20120507 : 要偵測完才可以動，會影響UPH
                    }

                    iTask[i][j]=300;
                    break;
                case 300:
                    if(CheckOutArmDestroyActiveDelay[i][j].Off())
                    {
                        bResetOutArm=false;
                        if(OutArmSuck.Suck[i][j].GetStatus())
                        {
                            iXpos=MOT[MOutArmX].ReadPos();
                            iYpos=MOT[MOutArmY].ReadPos();
                            ShowErrorMessage("WAR0258", 0, MOutArmX, false, OutArmSuck.Suck[i][j].sName);       //破壞錯誤, 請將IC取下

                            iWhichAuto=OutArmPordRec[i][j].GetWhcihAuto();
                            iCol=OutArmPordRec[i][j].GetAutoX();
                            iRow=OutArmPordRec[i][j].GetAutoY();
                            if(iWhichAuto>=0)                                   //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
                            {
                                iMotor=iMMAuto[iWhichAuto];

                                if(IniConfig.bE65_ClearTrayDataWhenOutArmDestoryErr)
                                {
                                    MOT[iMotor].SetTrayBinData(iCol, iRow, NULL_IC, AnsiString(""));

                                    if(iWhichAuto<0)
                                        Msg.sprintf("Clear unloader data error");
                                    else if(iWhichAuto<iAutoCnt)
                                        Msg.sprintf("Clear data of Auto%d, X:%d, Y%d", iWhichAuto+1, iCol, iRow);
                                    else
                                        Msg.sprintf("Clear data of Fix%d, X:%d, Y%d", iWhichAuto+1, iCol, iRow);

                                    MyDBIProcess("Process", Msg);
                                }
                                OutArmPordRec[i][j].bUse=true;
                                OutArmPordRec[i][j].AddErrorRecord("WAR0258", true, iXpos, iYpos);
                            }
                            bCheckOutArmDestroyActive[i][j]=true;
                            iTask[i][j]=200;                                    //Steven 20120507 : 有出錯就繼續吸吸看，看IC有沒有被拿掉。
                        }
                        else
                        {
                            bCheckOutArmDestroyActiveFinish[i][j]=true;         //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                            OutArmSuck.Suck[i][j].Normal();
                            OutArmPordRec[i][j].InitialRecord();                //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
                            bOutArmCheckDestroyACT[i][j]=false;
                            bCheckOutArmDestroyActive[i][j]=false;
                            iTask[i][j]=1;                                      //ChungHung 20111229 add 防止跳出後bInArmCheckDestroyACT馬上被設為true 會照成有一次無效
                        }
                    }
                    break;
                default :
                    break;
            }
        }
    }
}
//------------------------------------------------------------------------------
void DoOutArm_9045()
{
    if(IniConfig.bEnableTestingNeedStopAllMotor==true &&                        //jou 2013-09-25 Testing Need Stop All Motor
       IniConfig.bI24TestingNeedStopAllMotor==true &&
       bTestingStopAllMotor==true)
    {
        return;
    }

    if(IniConfig.bAlarmNeedServoOff==true)                                      //JerryYang 20161227 (Steven) 修正發生JAM,servo off後推動out arm, 再重新start會發生點位錯誤
    {
        if(fNote->bMyServoOffOutArm==true)
        {
            return;
        }
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(ArmSpeed_File[OutArm].bDevicConfirm)                                 //kevin 20170510 (wei) 回吸功能開關
            CheckOutArmDestroyActive();                                         //jou 981130 start : 確認device確實destroy完成
    }
    else
    {
        CheckOutArmDestroyActive();                                             //jou 981130 start : 確認device確實destroy完成
    }

    for(int i=0; i<OutArmSuck.iPickRow; i++)
        for(int j=0; j<OutArmSuck.iPickCol; j++)
            if(bCheckOutArmDestroyActive[i][j]==true)
                return;

    if(IniConfig.bQAMode==true &&
       LastSet.iRunStartMode==rsmQAMode &&
       CosFunction.bQAModeUseUnloadCnt)                                         //JerryYang 20221004 : Maxim版本QA mode
    {
        Check_QA_ModeUnloadCount();
    }

    if(iPauseBackUp!=-1 &&
       OutArmSuck.IsPickSuckFinish()==true &&
       OutArmSuck.IsPickDestroyFinish()==true)
    {
        return;
    }

    if(bResetOutArmTask==true)
    {
        bResetOutArmTask=false;
        InitOutArmTask();
        return;
    }

    bool bDoOutArmAutoAlignment=false;
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
       TestIF.bEnableAutoAlignment==true &&
       (LastSet.iRealDummy==HAS_TRAY || LastSet.iRealDummy==REALLY))            //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    {
        if(bAutoNeedTrayMustFinish)
        {
            if(iCleanOut==1 && HasICUnderMachine()==false)                      //process clean out ,don't supply new tray
            {
                bAutoNeedTrayMustFinish=false;
                return;
            }
            else
            {
                if(MOT[MManualTray1].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix1]]==true)
                {
                    ShowMyMessage("Alignment Must Put Fix1 Tray!");
                    return;
                }
                if(MOT[MManualTray2].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix2]]==true)
                {
                    ShowMyMessage("Alignment Must Put Fix2 Tray!");
                    return;
                }
                if(MOT[MManualTray3].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix3]]==true)
                {
                    ShowMyMessage("Alignment Must Put Fix3 Tray!");
                    return;
                }

                if(AUTO_EMPTY_COLOR>=3)                                         //Steven 20240428 : Add for HT9011 AOA
                {
                    if(MOT[MManualTray4].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]]==true)
                    {
                        ShowMyMessage("Alignment Must Put Fix4 Tray!");
                        return;
                    }
                    if(MOT[MManualTray5].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]]==true)
                    {
                        ShowMyMessage("Alignment Must Put Fix5 Tray!");
                        return;
                    }
                    if(MOT[MManualTray6].fHasTray==false && TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]]==true)
                    {
                        ShowMyMessage("Alignment Must Put Fix6 Tray!");
                        return;
                    }
                }

                if(WhichAutoNeedTray()==0)
                {
                    bDoOutArmAutoAlignment =CheckOutArmAutoAlignmentTrayModeBeUse(lOutArmAutoAlignmentTrayTimeingFlag, true);
                    bDoOutArmAutoAlignment|=CheckOutArmAutoAlignmentCKModeBeUse(lOutArmAutoAlignmentCKTimeingFlag, true);
                    bRunOutArmAutoAlignment=bDoOutArmAutoAlignment;
                    bAutoNeedTrayMustFinish=false;
                    return;
                }
                else
                {
                    IsCatchTrayReadySupplyNewTray();
                    return;
                }
            }
        }
        else if(bRunOutArmAutoAlignment)
        {
            return;
        }
    }
    else
    {
        bAutoNeedTrayMustFinish=false;
        bRunOutArmAutoAlignment=false;
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        DoOutArm_9045_All_1Picker();
    }
    else if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        DoOutArm_9045_1x1_1();
    }
    else if(iInArmType==e9045_1x2_2_14 ||
            iInArmType==e9045_1x2_2_13)
    {
        DoOutArm_9045_1x2_2();
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        DoOutArm_9045_1x2_4();
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        DoOutArm_9045_1x3_2_14();
    }
    else if(iInArmType==e9045_1x3_4)                                            //Steven 20220425 : 1x3_14
    {
        DoOutArm_9045_1x3_4();
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        DoOutArm_9045_1x4_2();
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        DoOutArm_9045_1x4_4S();
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
//
    }
    else if(iInArmType==e9045_1x4_4)
    {
        DoOutArm_9045_1x4_4();
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        DoOutArm_9045_1x4_8();
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        DoOutArm_9045_2x1_2();
    }
    else if(iInArmType==e9045_2x2_4_12 ||
            iInArmType==e9045_2x2_4_13 ||
            iInArmType==e9045_2x2_4_14)
    {
        DoOutArm_9045_2x2_4();
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        DoOutArm_9045_2x2_8();
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        DoOutArm_9045_2x3_6_14();
    }
    else if(iInArmType==e9045_2x3_6)
    {
        DoOutArm_9045_2x3_6();
    }
    else if(iInArmType==e9045_2x4_4_13 ||
            iInArmType==e9045_2x4_4_14)
    {
        DoOutArm_9045_2x4_4();
    }
    else if(iInArmType==e9045_2x4_8)
    {
        DoOutArm_9045_2x4_8();
    }
    else if(iInArmType==e9045_2x5_8)                                            //wei 20190614 10 site
    {
        DoOutArm_9045_2x5_8();
    }
    else if(iInArmType==e9045_2x6_8)
    {
        DoOutArm_9045_2x6_8();
    }
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        DoOutArm_9045_2x8_8();
    }
}
extern int CheckSuckInitialStatus(TMyKitSuck &Ptr, int iR, int iC);
//-----------------------------------------------------------------------------
bool CheckOutArmInitState_9045()                                                //Steven 20100104 : 將所有錯誤整合只顯示一次
{
    bool bHasFail=false;
    int ret;
    int flag[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0}, {0, 0, 0, 0}};
    AnsiString errSuck="at Out Arm Suck";
    int iXpos=MOT[MOutArmX].ReadPos();
    int iYpos=MOT[MOutArmY].ReadPos();
    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            flag[i][j]=CheckSuckInitialStatus(OutArmSuck, i, j);
            if(flag[i][j]!=0)
            {
                bHasFail=true;
                errSuck+=OutArmSuck.Suck[i][j].sName;
            }
        }
    }

    if(bHasFail)
        ret=ShowErrorMessage("WAR0226", K_RETRY|K_SKIP, MOutArmX, false, errSuck);

    if(ret==K_SKIP)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(flag[i][j]>=Vaccum_Initial_Off)
                {
                    OutArmSuck.PordRec[i][j].AddErrorRecord("WAR0226", true, iXpos, iYpos);         //Steven 20160114 (jou) : Add Production Error Log
                    OutArmSuck.SetItemData(i, j, HAS_NULL_IC);
                }
            }
        }
    }
    return !bHasFail;
}
//------------------------------------------------------------------------------
void DoOutArmSuckPreOn(int iSht, int iKit, int iXPos, int iYPos, bool bZDown)
{
    if(bZDown==false)                                                           //Steven 20230601 : 加入iKit跟bZDown判斷
        return;

    int iPosX=0, iPosY=0;
    int iPreRange=50;
    iKit=(iKit==0)?0:OutArmSuck.iPickKitStep;

    if(iSht==0)
        ptrOutSHT=&FRCarryKit;
    else
        ptrOutSHT=&BRCarryKit;

    if(LastSet.iRealDummy==REALLY &&
       ArmSpeed[OutArm].bSuckOnDown &&
       Sen[SnRKManualStep].IsOn()==false)
    {
        iPosX=MOT[MOutArmX].ReadPos();
        iPosY=MOT[MOutArmY].ReadPos();                                          //2013-08-02    Dell
        if(iPosX>=iXPos-iPreRange && iPosX<=iXPos+iPreRange &&
           iPosY>=iYPos-iPreRange && iPosY<=iYPos+iPreRange)
        {
            for(int i=0; i<OutArmSuck.iPickRow; i++)
            {
                for(int j=0; j<OutArmSuck.iPickCol; j++)                        //少了kit
                {
                    if(ptrOutSHT->Item[i][j+iKit] &&                            //有問題!!
                       OutArmSuck.Suck[i][j].GetNeedSuckStatus())
                        OutArmSuck.Suck[i][j].On();
                }
            }
        }
    }
}
//==============================================================================
int iOutShtRetryCount=0;
bool bOutSuckShtDupErr[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
void GetOutShuttleStatus_9045(int iSht, int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bZDown, bool bZFlag[MAX_ARM_Row][MAX_ARM_Col])
{
    int iR, iC;
    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            iR=OutArmSuck.Suck[i][j].iMyRow;                                    //實體Row   //Steven 20250826 : fixed for out arm offset
            iC=OutArmSuck.Suck[i][j].iMyCol;                                    //實體Col

            if(bZDown && OutArmSuck.Suck[i][j].GetNeedSuckStatus())
                bZFlag[i][j]=true;
            else
                bZFlag[i][j]=false;

            if(bZDown)
            {
                if(iSht==0)
                    iZPos[i][j]=Prod.ZOutArm_Shuttle1_Pick[iR][iC]+iOutShtRetryCount*ArmSpeed[OutArm].dRetryDown;
                else
                    iZPos[i][j]=Prod.ZOutArm_Shuttle2_Pick[iR][iC]+iOutShtRetryCount*ArmSpeed[OutArm].dRetryDown;
            }
            else
            {
                iZPos[i][j]=ZSafePos;
            }
        }
    }
}
//------------------------------------------------------------------------------
void InitialFix3CanFullTask()                                                   //ChungHung 20140313 add Fix3 can Full Tray
{
    iFix3CanFullTask=1;
}
//------------------------------------------------------------------------------
bool Fix3MoveToLeft(bool bLeft)                                                 //JimmyChiu 20220927 : Stepper Motor Control in Fix3
{
    int iXPos=0;
    bool bBack=false;
    if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor)
    {
        iXPos=(bLeft)?Tech.iFix3PosL:Tech.iFix3PosR;
        if(MOT[MFix3Full].CompareCommandPos(iXPos, 10)!=1)
            bBack=MOT[MFix3Full].MotorMove(iXPos);
        else
            bBack=true;

        if(bBack)
        {
            if(bLeft)
            {
                if(Sen[SnFix3FullPlace].Enable &&                               //Steven 20250814 : Fix3滿盤功能-馬達版+Sensor
                   Sen[SnFix3FullPlace].IsOn()==false)
                {
                    bBack=false;
                    ShowMyMessage("The fix3 motor does not move to left!!", "Please check the motor or the sensor (SnFix3FullPlace) should be on");
                }
            }
            else
            {
                MOT[MFix3Full].ScanMotorStatus();
                int iPos=MOT[MFix3Full].ReadPos();
                if(iPos>=0 &&
                   MOT[MFix3Full].Led[iHomeLed]==false)
                {
                    bBack=false;
                    ShowMyMessage("The fix3 motor does not move to right!!", "Please check the motor or the motor home sensor should be on");
                }
            }
        }
    }
    return bBack;
}
//------------------------------------------------------------------------------
TQPF_Timer Fix3CylinderDelay;
bool UseFix3Cylinder(int iWhichFix)                                             //ChungHung 20140313 add Fix3 can Full Tray
{
    bool bResult=false;
    static int iCount=0;
    static bool bDuplicate=false;
    bool bByPass=false;
    int &Task=iFix3CanFullTask;                                                 //ChungHung 20140313 add Fix3 can Full Tray
    #ifdef SOFT_SIMULTE
        bByPass=true;                                                           //Steven 20230516 : 為了讓軟體模擬看起來Fix3會動
    #endif

    if(AUTO_EMPTY_COLOR>=3)
        return true;

    if(FIX3_FULL_PLACE==Fix3K_UseCylinder)
    {
        switch(Task)                                                            //ChungHung 20140313 add Fix3 can Full Tray
        {
            case 1:
                if(MoveOutArmToAutoSafe_9045()==false)                          //ChungHung 20140313 add Fix3 can Full Tray
                    return bResult;

                iCount=0;
                bDuplicate=false;
                if(iMMAuto[iWhichFix]==MManualTray3)
                {
                    bShuttleMoveToLeftforFix3=true;                             //Shuttle1 and Shuttle2 go to left side
                }
                else
                {
                    bShuttleMoveToLeftforFix3=false;
                }
                bUseFix3CylinderActive=true;                                    //ChungHung 20150520 add fix Fix3 full 和 ShuttleShanke 衝突
                Cylinder[C_FixTray_FullPlace].Reset();
                Task=20;
            case 20:
                if(bShuttleMoveToLeftforFix3)                                   //ChungHung 20140313 add Fix3 can Full Tray
                {
                    if(InSHT1InLF()!=true || InSHT2InLF()!=true)                //wait shuttle1 and shuttle2 already in left side
                    {
                        if(bSuckingFlagZ1==false &&                             //Steven 20240916 : index下降到shuttle吸放料
                           InSHT1InLF()==false)                                 //Steven 20210802 : 修正index吸取異常跑[D43]流程, 同時out arm又要放Fix3
                        {
                            MOT[MInShuttle1].fCanMoveR=true;
                            if(InArmZSafe(DETECT_ALL_FLAG)==-1)                 //Steven 20211122 : 修正Fix3使用氣缸滿盤, 如果Z軸在上才可以推
                            {
                                MOT[MInShuttle1].fCanMoveM=true;                //Steven 20230118 : 修正index吸取異常跑[D43]流程, 同時out arm又要放Fix3
                                MOT[MInShuttle1].fCanMoveL=true;
                            }
                        }

                        if(bSuckingFlagZ2==false &&                             //Steven 20240916 : index下降到shuttle吸放料
                           InSHT2InLF()==false)
                        {
                            MOT[MInShuttle2].fCanMoveR=true;
                            if(InArmZSafe(DETECT_ALL_FLAG)==-1)                 //Steven 20211122 : 修正Fix3使用氣缸滿盤, 如果Z軸在上才可以推
                            {
                                MOT[MInShuttle2].fCanMoveM=true;                //Steven 20230118 : 修正index吸取異常跑[D43]流程, 同時out arm又要放Fix3
                                MOT[MInShuttle2].fCanMoveL=true;
                            }
                        }
                        return bResult;
                    }
                    MOT[MInShuttle1].fCanMoveR=false;
                    MOT[MInShuttle2].fCanMoveR=false;
                    if(Cylinder[C_FixTray_FullPlace].OnStatus()==false)         //ChungHung 20140609 add Use Fix3 Cylinder時 不要每次等Delay time
                    {
                        Cylinder[C_FixTray_FullPlace].On();
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Task=30;
                    }
                    else
                    {
                        Task=40;
                    }
                }
                else
                {
                    Cylinder[C_FixTray_FullPlace].Reset();
                    if(Cylinder[C_FixTray_FullPlace].OffStatus()==false)        //ChungHung 20140609 add Use Fix3 Cylinder時 不要每次等Delay time
                    {
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Cylinder[C_FixTray_FullPlace].Off();
                        Task=50;
                    }
                    else
                    {
                        Task=60;
                    }
                }
                if(Task!=30)
                {
                    break;
                }
            case 30:
                if(Cylinder[C_FixTray_FullPlace].Push() || bByPass)
                {
                    Fix3CylinderDelay.SetMSAndOn((bByPass)?1:300);              //wei 20150901 減少Delay時間 1000--->300
                    Task=31;
                }
                else if(Fix3CylinderDelay.Off())                                //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                {
                    if(bHangTimePause==true)                                    //Rogeryang 20251231 : Add HangTimePause need to reset
                        Fix3CylinderDelay.SetSecAndOn(10);
                    else
                        Task=45;
                    break;
                }
                else
                {
                    break;
                }
            case 31:
                if(Fix3CylinderDelay.Off())
                {
                    if(Cylinder[C_FixTray_FullPlace].OnSensor())                //wei 20150424 到位檢查
                    {
                        Task=40;
                    }
                    else
                    {
                        if(bHangTimePause==false)                               //Rogeryang 20251231 : Add HangTimePause need to reset
                            iCount++;
                        Task=30;
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Cylinder[C_FixTray_FullPlace].Reset();
                        if(iCount>=3)                                           //Steven 20190619 : Fix3氣缸動作增加異常的例外處理
                        {
                            Task=45;
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            case 40:
                bResult=true;
                break;
            case 45:
                ShowErrorMessage("JAM1940", K_RETRY, MManualTray3, bDuplicate);
                Fix3CylinderDelay.SetSecAndOn(10);                              //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                iCount=0;
                bDuplicate=true;
                Task=30;
                break;
            case 50:
                if(Cylinder[C_FixTray_FullPlace].Pop() || bByPass)
                {
                    Fix3CylinderDelay.SetMSAndOn((bByPass)?1:300);              //wei 20150901 減少Delay時間 1000--->300
                    Task=51;
                }
                else if(Fix3CylinderDelay.Off())                                //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                {
                    if(bHangTimePause==true)                                    //Rogeryang 20251231 : Add HangTimePause need to reset
                        Fix3CylinderDelay.SetSecAndOn(10);
                    else
                        Task=65;
                    //break;                                                    //RogerYang 20251230 : remove break
                }
                else
                {
                    Fix3CylinderDelay.SetSecAndOn(10);
                    break;
                }
            case 51:
                if(Fix3CylinderDelay.Off())
                {
                    if(Cylinder[C_FixTray_FullPlace].OffSensor())               //wei 20150424 到位檢查
                    {
                        Task=60;
                    }
                    else
                    {
                        Task=50;
                        if(bHangTimePause==false)                               //Rogeryang 20251231 : Add HangTimePause need to reset
                            iCount++;
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Cylinder[C_FixTray_FullPlace].Reset();
                        if(iCount>=3)                                           //Steven 20190619 : Fix3氣缸動作增加異常的例外處理
                        {
                            Task=65;
                        }
                    }
                }
                if(Task!=60)
                {
                    break;
                }
            case 60:
                bUseFix3CylinderActive=false;
                MOT[MInShuttle1].fCanMoveR=true;
                MOT[MInShuttle2].fCanMoveR=true;
                bResult=true;
                break;
            case 65:
                ShowErrorMessage("JAM1941", K_RETRY, MManualTray3, bDuplicate);
                Fix3CylinderDelay.SetSecAndOn(10);                              //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                iCount=0;
                bDuplicate=true;
                Task=50;
                break;
        }
    }
    else if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA)                             //ChungHung 20140722 add for HT9046LA //蓋二樓的版本
    {
        if(Sen[SnFix3Lock].IsOn()==true)
        {
            ShowMyMessage("Please UnLock Fix3","");
            Task=1;
            return bResult;
        }

        switch(Task)
        {
            case 1:
                if(MoveOutArmToAutoSafe_9045()==false)                          //ChungHung 20140313 add Fix3 can Full Tray
                    return bResult;

                iCount=0;
                bDuplicate=false;
                if(iMMAuto[iWhichFix]==MManualTray3)
                {
                    if(Cylinder[C_FixTray_FullPlace].OnStatus()==false)         //ChungHung 20140609 add Use Fix3 Cylinder時 不要每次等Delay time
                    {
                        Cylinder[C_FixTray_FullPlace].On();
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Task=30;
                    }
                    else
                    {
                        Task=50;
                        bResult=true;
                    }
                }
                else
                {
                    if(Cylinder[C_FixTray_FullPlace].OnStatus()==true)          //ChungHung 20140609 add Use Fix3 Cylinder時 不要每次等Delay time
                    {
                        Cylinder[C_FixTray_FullPlace].Off();
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        Task=40;
                    }
                    else
                    {
                        Task=50;
                        bResult=true;
                    }
                }
                break;
            case 30:
                if(Cylinder[C_FixTray_FullPlace].Push() || bByPass)
                {
                    if(Cylinder[C_FixTray_FullPlace].OnSensor())                //wei 20150424 到位檢查
                    {
                        Task=50;
                        bResult=true;
                    }
                    else
                    {
                        Cylinder[C_FixTray_FullPlace].Reset();
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        if(bHangTimePause==false)                               //Rogeryang 20251231 : Add HangTimePause need to reset
                            iCount++;
                        if(iCount>=3)                                           //Steven 20190619 : Fix3氣缸動作增加異常的例外處理
                            Task=35;
                        else
                            Task=30;
                    }
                }
                else if(Fix3CylinderDelay.Off())
                {
                    if(bHangTimePause==true)                                    //Rogeryang 20251231 : Add HangTimePause need to reset
                        Fix3CylinderDelay.SetSecAndOn(10);
                    else
                        Task=35;
                }
                break;
            case 35:
                ShowErrorMessage("JAM1940", K_RETRY, MManualTray3, bDuplicate);
                Fix3CylinderDelay.SetSecAndOn(10);                              //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                iCount=0;
                bDuplicate=true;
                Task=30;
                break;
            case 40:
                if(Cylinder[C_FixTray_FullPlace].Pop() || bByPass)
                {
                    if(Cylinder[C_FixTray_FullPlace].OffSensor())               //wei 20150424 到位檢查
                    {
                        Task=50;
                        bResult=true;
                    }
                    else
                    {
                        Cylinder[C_FixTray_FullPlace].Reset();
                        Fix3CylinderDelay.SetSecAndOn(10);                      //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                        if(bHangTimePause==false)                               //Rogeryang 20251231 : Add HangTimePause need to reset
                            iCount++;
                        if(iCount>=3)                                           //Steven 20190619 : Fix3氣缸動作增加異常的例外處理
                            Task=45;
                        else
                            Task=40;
                    }
                }
                else if(Fix3CylinderDelay.Off())
                {
                    if(bHangTimePause==true)                                    //Rogeryang 20251231 : Add HangTimePause need to reset
                        Fix3CylinderDelay.SetSecAndOn(10);
                    else
                        Task=45;
                }
                break;
            case 45:
                ShowErrorMessage("JAM1941", K_RETRY, MManualTray3, bDuplicate);
                Fix3CylinderDelay.SetSecAndOn(10);                              //Steven 20230517 : 增加Fix 3氣缸10秒內沒到位的Alarm
                iCount=0;
                bDuplicate=true;
                Task=40;
                break;
            case 50:
                bResult=true;
                break;
        }
    }
    else if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor)                             //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    {
        switch(Task)
        {
            case 1:
                if(iMMAuto[iWhichFix]==MManualTray3 ||
                   iMMAuto[iWhichFix]==MManualTray2)
                {
                    if(MoveOutArmToAutoSafe_9045()==false)
                        return false;
                    if(iMMAuto[iWhichFix]==MManualTray3)
                        Task=30;
                    else
                        Task=40;
                }
                else
                {
                    return true;
                }
                break;
            case 25:                                                            //Initial home
                InitProcessSingleMotorTask(MFix3Full);
                Task=27;
            case 27:
                if(ProcessSingleMotorHome(MFix3Full)==true)
                {
                    Task=30;
                }
                break;
            case 30:
                if(bHandlerPause==true)                                         //JimmyChiu 20230808 : Fixed for Pause to lost position when step motor move
                {
                    Task=25;
                }
                else if(Fix3MoveToLeft(true))
                {
                    Task=50;
                }
                break;
            case 35:                                                            //Initial home
                InitProcessSingleMotorTask(MFix3Full);
                Task=37;
            case 37:
                if(ProcessSingleMotorHome(MFix3Full)==true)
                {
                    Task=40;
                }
                break;
            case 40:
                if(bHandlerPause==true)                                         //JimmyChiu 20230808 : Fixed for Pause to lost position when step motor move
                {
                    Task=35;
                }
                else if(Fix3MoveToLeft(false))
                {
                    Task=50;
                }
                break;
            case 50:
                bResult=true;
                break;
        }
    }
    return bResult;
}
//==============================================================================
int GetVariableYOutShuttleData()                                                //ChungHung 20131231 alter AutoYPitch
{
    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                           //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(OutArmSuck.iPickRow==1 ||
           iInArmType==e9045_1x4_8_Hot ||
           iInArmType==e9045_1x2_4_Hot ||
           iInArmType==e9045_2x2_8_Hot)     //JerryYang 20231003
            return 6000;

        if(TestIF.dSiteYPitch>=IN_OUT_ARM_Y_PITCH_MAX)                          //kevin 20190901 : 修正Y-Pitch
             return IN_OUT_ARM_Y_PITCH_MAX;
        else if(TestIF.dSiteYPitch<=IN_OUT_ARM_Y_PITCH_MIN)                     //kevin 20190901
            return IN_OUT_ARM_Y_PITCH_MIN;

        return TestIF.dSiteYPitch;
    }
    else
    {
        return TestIF_File.iARM_Y_PITCH;
    }
}
//------------------------------------------------------------------------------
int GetOutArmToShuttleOffset_9045(int iSht, int iModeRow, int iModeCol, bool bPitch)
{
    int iOffsetPos=-1;
    if(TestIF.bEnableAutoAlignment==false)
    {
        if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true && bPitch)
        {
            iOffsetPos=OutOfsOutSh1;
        }
        else
        {
            if(IniConfig.bE47_ShuttleUse4Offset)
            {
                if(iModeRow==1)
                {
                    if(iModeCol%100<10)
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1LB;
                        else
                            iOffsetPos=OutOfsOutSh2LB;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1RB;
                        else
                            iOffsetPos=OutOfsOutSh2RB;
                    }
                }
                else if(iModeRow==0)
                {
                    if(iModeCol%100<10)
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1;
                        else
                            iOffsetPos=OutOfsOutSh2;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1RA;
                        else
                            iOffsetPos=OutOfsOutSh2RA;
                    }
                }
                else
                {
                    if(iModeCol%100<10)                                         //Steven 20240826 : 補上Offset
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1;
                        else
                            iOffsetPos=OutOfsOutSh2;
                    }
                    else
                    {
                        if(iSht==0)
                            iOffsetPos=OutOfsOutSh1RA;
                        else
                            iOffsetPos=OutOfsOutSh2RA;
                    }
                }
            }
            else
            {
                if(iSht==0)
                    iOffsetPos=OutOfsOutSh1;
                else
                    iOffsetPos=OutOfsOutSh2;
            }
        }
    }
    return iOffsetPos;
}
//==============================================================================
int GetOutArmXToShuttleOffset_9045(int iSht, int iOffsetPos)                    //Steven 20240309 : fixed for out arm to sht offset
{
    int iPos=0;
    if(iOffsetPos>=0)
    {
        if(iSht==0)
            iPos=OutArmOffSet[iOffsetPos]->GetX();
        else
            iPos=OutArmOffSet[iOffsetPos]->GetX();
    }
    return iPos;
}
//==============================================================================
int GetOutArmYToShuttleOffset_9045(int iSht, int iOffsetPos)
{
    int iPos=0;
    if(iOffsetPos>=0)
    {
        if(iSht==0)
            iPos=OutArmOffSet[iOffsetPos]->GetY();
        else
            iPos=OutArmOffSet[iOffsetPos]->GetY();
    }
    return iPos;
}
//------------------------------------------------------------------------------
int SearchTrayToPlace_9045()                                                    //Steven 20160524 : 簡化Out Arm程式碼
{
//    int iSuckX=0, iSuckY=3;
//    if(Prod.bSingleUseOtherSuck)                                                //wei 20220823 Single Site使用C吸嘴判斷
//    {
//        iSuckY=2;
//    }

    if(IniConfig.bI37_EnableFIFOMode &&
       LastSet.iRunStartMode==rsmFIFOMode)                                      //Ifor 20181127 : Find_OutArm_Single 搬至 SearchTrayToPlace_9045()判斷避免進入DoMoveOutArmXYToPlace_9045
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(OutArmSuck.Item[i][j]!=NULL_IC)
                {
                    if(iLoaderNum==0)                                           //Frank 20181121 add iLoaderNum
                        iLoaderNum=OutArmSuck.PordRec[i][j].GetLoaderNum();
                    if(iLoaderNum!=OutArmSuck.PordRec[i][j].GetLoaderNum())
                    {
                        if(MOT[MMAuto2].HasIC())                                //Ifor 20181127 : add 避免滿Tray後換Tray會再次換Tray
                        {
                            MOT[MMAuto2].InitNewTray(HAS_IC, false, __FUNC__);
                        }
                        iLoaderNum=OutArmSuck.PordRec[i][j].GetLoaderNum();
                    }
                }
            }
        }

        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20181107 : add KYEC FIFO 強制放至Auto 2
            return 1;
    }

    if(fAGV->IsATK_AMR() &&
       LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                                 //Steven 20260202 : for ATK AMR
    {
        if(OutArmSuck.ArmRow0HaveRealIC())
            iOutArmPlaceOrder=0;
        else
            iOutArmPlaceOrder=1;
        return iFixToWhichAuto-eFix1;
    }

    if(AUTO3_IS_MAGAZINE==1)                                                    //JerryYang 20221215 : add Magazine
    {
        return SearchTrayToPlace_Magazine();
    }

    iOutArmPlaceOrder=0;
//    if(TestIF.iTestMode==SingleSite)
//    {
//        for(int k=0; k<eTrayCount; k++)
//        {
////            if(Prod.iTrayType[k]==tNotUse)
////                continue;
//
//            if(OutArmSuck.Item[iSuckX][iSuckY]!=NULL_IC &&
//               OutArmSuck.iWhichAuto[iSuckX][iSuckY]==iSortTrayIndex[k])
//            {
//                iOutArmPlaceOrder=0;
//                return iSortTrayIndex[k];
//            }
//        }
//    }
//    else
//    {
        for(int k=0; k<eTrayCount; k++)                                         //JerryYang 20230820
        {
            for(int i=0; i<OutArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<OutArmSuck.iMaxCol; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iWhichAuto[i][j]==iSortTrayIndex[k])
                    {
                        if(i==0)
                            iOutArmPlaceOrder=0;
                        else
                            iOutArmPlaceOrder=1;
                        return iSortTrayIndex[k];
                    }
                }
            }
        }
//    }

    return 4;
}
//------------------------------------------------------------------------------
int SearchTrayToPlace_Magazine()                                                //JerryYang 20221215 : add Magazine
{
    int iTempWhichMag=-1;
    iOutArmPlaceOrder=0;
    bool bHasChang=false;

    for(int i=0; i<OutArmSuck.iMaxRow; i++)                                     //Sam 20221121 : 先檢查吸嘴上有沒有可以直接放 Auto3 Magazine 的 Tray
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            for(int k=0; k<MAX_MGZ_TRAY; k++)
            {
                if(OutArmSuck.Item[i][j]!=NULL_IC &&
                    OutArmSuck.iWhichAuto[i][j]==iSortTrayIndex[k])
                {
                    if(i==0)
                        iOutArmPlaceOrder=0;
                    else
                        iOutArmPlaceOrder=1;

                    iTempWhichMag=OutArmSuck.iWhichAuto[i][j]-iMagMin;          //JerryYang 20221121 : 避免iWhichMag一直被改變, 有找到當下要放auto3的再改
                    if(MOT[MMMagazineTary1+iTempWhichMag].Tray.FullIC()==true)  //JerryYang 20250920 : fixed for Multi bin
                    {
                        for(int l=1; l<14; l++)
                        {
                            if(iTempWhichMag+l<14)
                            {
                                if(BinSelect[iTestRunMode].bMagazineLink[iTempWhichMag+l]==true)
                                {
                                    if(MOT[MMMagazineTary1+iTempWhichMag+l].Tray.FullIC()==false && iTempWhichMag+l==iAuto3MagazineIndex)
                                    {
                                        iWhichMag=iTempWhichMag+l;
                                        iOutArmWhichAuto=iMagAtAuto;
                                        return 2;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(iTempWhichMag==iAuto3MagazineIndex)
                        {
                            iWhichMag=iTempWhichMag;
                            iOutArmWhichAuto=iMagAtAuto;
                            return 2;
                        }
                    }
                }
            }
        }
    }

    for(int k=0; k<eTrayCount; k++)
    {
        if(Prod.iTrayType[iSortTrayIndex[k]]==tNotUse)
            continue;

        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(OutArmSuck.Item[i][j]!=NULL_IC &&
                   OutArmSuck.iWhichAuto[i][j]==iSortTrayIndex[k])
                {
                    if(i==0)
                        iOutArmPlaceOrder=0;
                    else
                        iOutArmPlaceOrder=1;
                    if(OutArmSuck.iWhichAuto[i][j]>=iMagMin)
                    {
                        iWhichBuff=(OutArmSuck.iWhichAuto[i][j]-iMagMin)%iFixCnt+iFixCnt;   //QQQ
                        iTempWhichMag=OutArmSuck.iWhichAuto[i][j]-iMagMin;
                        bHasChang=false;
                        if(MOT[MMMagazineTary1+iTempWhichMag].Tray.FullIC()==true)  //JerryYang 20250920 : fixed for Multi bin
                        {
                            for(int l=1; l<14; l++)
                            {
                                if(iTempWhichMag+l<14)
                                {
                                    if(BinSelect[iTestRunMode].bMagazineLink[iTempWhichMag+l]==true)
                                    {
                                        if(MOT[MMMagazineTary1+iTempWhichMag+l].Tray.FullIC()==false)
                                        {
                                            iWhichMag=iTempWhichMag+l;
                                            bHasChang=true;
                                             break;
                                        }
                                    }
                                    else
                                    {
                                        iWhichMag=iTempWhichMag;
                                        break;
                                    }
                                }
                            }
                        }

                        if(bHasChang==false)
                            iWhichMag=iTempWhichMag;

                        if(TestIF_File.iMagFixTrayType==1)                      //buffer tray
                        {
                            if(iAuto3MagazineIndex==iWhichMag)
                            {
                                iOutArmWhichAuto=iMagAtAuto;
                                return 2;
                            }
                            else if(CheckPlaceToBufferTray(OutArmSuck.iWhichAuto[i][j]))          //buffer區有位置, 先放buffer區
                            {
                                iOutArmWhichAuto=OutArmSuck.iWhichAuto[i][j];
                                return OutArmSuck.iWhichAuto[i][j];
                            }
                            else                                                //buffer區不夠放, 放AUTO3
                            {
                                iOutArmWhichAuto=iMagAtAuto;
                                return 2;
                            }
                        }
                        else
                        {
                            iOutArmWhichAuto=iMagAtAuto;
                            return 2;
                        }
                    }
                    else
                    {
                        iOutArmWhichAuto=iSortTrayIndex[k];
                        return iSortTrayIndex[k];
                    }
                }
            }
        }
    }
    return Prod.iIfErrorT6;
}
//------------------------------------------------------------------------------
int SearchTrayToPick_Buffer()                                                   //JerryYang 20221215 : add Magazine
{
    iOutArmPickOrder=0;
    int iMag=iPickWhichMag-eMag1;
    int iRx=iMag%iFixCnt+eMag1;
    bool bCheck=false;

    if(fAGV->IsATK_AMR() &&
       LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                                 //Steven 20260202 : for ATK AMR
    {
        for(int i=iFixMin; i<=iFixMax; i++)                                     //Fix Tray
        {
            if(bCheck==false &&
               MOT[iMMAuto[i]].HasRealIC())
            {
                iRx=i;
                iWhichAuto=i-iFixMin;
                bCheck=true;
            }
        }
    }

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            if(OutArmSuck.Item[i][j]==NULL_IC)
            {
                if(i==0)
                    iOutArmPickOrder=0;
                else
                    iOutArmPickOrder=1;
                return iRx;
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
int VerifyFixTrayLink()
{
    if(AUTO3_IS_MAGAZINE==1 &&                                                  //JerryYang 20221215 : add Magazine
       iWhichAuto==iMagAtAuto)
    {
        return 10000;
    }
    else if(AUTO3_IS_MAGAZINE==1             &&                                 //JerryYang 20221215 : Magazine把fix區當buffer區功能
            TestIF_File.iMagFixTrayType==1   &&
            iWhichAuto>=iMagMin)
    {
        return 0;
    }
    else
    {
        SetFixTrayFullIC();
        if(MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
           (MOT[iMMAuto[iWhichAuto]].FullIC() &&
            CheckUseFixBinBoxFunction()==false))                                //kevin 20160822
        {
            if(iWhichAuto>=iFixMin)
            {
                if(TrayForm.iFixTrayMode==0)                                    // full tray
                {
                    if(iWhichAuto==iFixMin && Prod.bLinkTo6Tray[eFix2])
                    {
                        for(int j=0; j<OutArmSuck.iPickCol; j++)
                        {
                            if(OutArmSuck.Item[iOutArmPlaceOrder][j]!=NULL_IC &&
                               OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]==iWhichAuto)
                            {
                                OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]=iWhichAuto+1;
                                return 1;
                            }
                        }
                    }

                    if(iWhichAuto==iFixMin+1 && TrayForm.bFixTrayLink[eFix3])    // fix 2 full link fix3
                    {
                        for(int j=0; j<OutArmSuck.iPickCol; j++)
                        {
                            if(OutArmSuck.Item[iOutArmPlaceOrder][j]!=NULL_IC &&
                               OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]==iWhichAuto)
                            {
                                OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]=iWhichAuto+1;
                                return 1;
                            }
                        }
                    }
                }
                return 4000;
            }
            else if(IniConfig.bAutoTrayLink==true)                              //jou 2012-06-14 Auto Tray Link
            {
                if((iWhichAuto==eAuto2 && Prod.bLinkTo6Tray[eAuto3]==true) ||
                   (iWhichAuto==eAuto1 && Prod.bLinkTo6Tray[eAuto2]==true))
                {
                    for(int j=0; j<OutArmSuck.iPickCol; j++)
                    {
                        if(OutArmSuck.Item[iOutArmPlaceOrder][j]!=NULL_IC &&
                           OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]==iWhichAuto)
                        {
                            OutArmSuck.iWhichAuto[iOutArmPlaceOrder][j]=iWhichAuto+1;
                        }
                    }
                    return 1;
                }
            }
            MoveOutArmXY_ToFix_Tray_Full();
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool DoFixTrayFullAlarm()                                                       //Steven 20181113 : 修正Fix Link Alarm的問題
{
    bool bResult=true;
    bool bScan1By1=false;
    int iAuto, iFix=iAutoIndex[iWhichAuto];
    AnsiString str, s;

    if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor &&
       iWhichAuto==eFix3 &&                                                     //Jimmychiu 20250306 : iFix --> iWhichAuto
       Fix3MoveToLeft(false)==false)                                            //Jimmychiu 20230628 : FIX3到達左側不檢查盤是否存在
    {
        bResult=false;
        return bResult;
    }

    if(AUTO3_IS_MAGAZINE==1             &&                                      //JerryYang 20221215 : Magazine把fix區當buffer區功能
       TestIF_File.iMagFixTrayType==1   &&
       iWhichAuto>=iMagMin)
    {
    }
    else
    {
        if(iWhichAuto==eFix1 &&                                                 //Steven 20181113 : 修正Fix Link顯示問題
           Prod.bLinkTo6Tray[eFix2]==true &&
           Prod.bLinkTo6Tray[eFix3]==true)
        {
            if(MOT[MManualTray1].FullIC() &&
               MOT[MManualTray2].FullIC() &&
               MOT[MManualTray3].FullIC())
            {
                #ifdef SOFT_SIMULTE
                    if(CosFunction.bReadClipCodeFromUnloader==true &&
                       IniConfig.bP60ReadClipCodeFromUnloader==true &&
                       LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix1);
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                        InitNewFixTrayForUnloaderClipRead(eFix3);
                    }
                    else
                    {
                        InitNewFixTray(eFix1, "AOutArm_FullIC");                //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix2, "AOutArm_FullIC");
                        InitNewFixTray(eFix3, "AOutArm_FullIC");
                    }
                #else
                    if(LastSet.iRealDummy!=DUMMY)
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)                    //Steven 20140528 : Secs Gem
                        {
                            EventReport(SECS_EVENT.Fix1Full);                   //38     Fix 1 Full
                            EventReport(SECS_EVENT.Fix2Full);
                            EventReport(SECS_EVENT.Fix3Full);
                        }

                        ShowErrorMessage("MES1710", K_RETRY, MManualTray1);     //Fix Tray 1, 2 and 3 is filled with devices.
                    }
                    else if(CosFunction.bReadClipCodeFromUnloader==true &&
                            IniConfig.bP60ReadClipCodeFromUnloader==true &&
                            LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix1);
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                        InitNewFixTrayForUnloaderClipRead(eFix3);
                    }
                    else
                    {
                        InitNewFixTray(eFix1, "AOutArm_FullIC");                //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix2, "AOutArm_FullIC");
                        InitNewFixTray(eFix3, "AOutArm_FullIC");
                    }
                #endif
                bResult=false;
            }
            else
            {
                bScan1By1=true;
            }
        }
        else if(iWhichAuto==eFix2 &&
                Prod.bLinkTo6Tray[eFix2]==true &&
                Prod.bLinkTo6Tray[eFix3]==false)
        {
            if(MOT[MManualTray1].FullIC() &&
               MOT[MManualTray2].FullIC())
            {
                #ifdef SOFT_SIMULTE
                    if(CosFunction.bReadClipCodeFromUnloader==true &&
                       IniConfig.bP60ReadClipCodeFromUnloader==true &&
                       LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix1);
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                    }
                    else
                    {
                        InitNewFixTray(eFix1, "AOutArm_FullIC");                    //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix2, "AOutArm_FullIC");
                    }
                #else
                    if(LastSet.iRealDummy!=DUMMY)
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)                    //Steven 20140528 : Secs Gem
                        {
                            EventReport(SECS_EVENT.Fix1Full);                   //38     Fix 1 Full
                            EventReport(SECS_EVENT.Fix2Full);
                        }

                        ShowErrorMessage("MES1711", K_RETRY, MManualTray1);     //Fix Tray 1 and 2 is filled with devices.
                    }
                    else if(CosFunction.bReadClipCodeFromUnloader==true &&
                            IniConfig.bP60ReadClipCodeFromUnloader==true &&
                            LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix1);
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                    }
                    else
                    {
                        InitNewFixTray(eFix1, "AOutArm_FullIC");                //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix2, "AOutArm_FullIC");
                    }
                #endif
                bResult=false;
            }
            else
            {
                bScan1By1=true;
            }
        }
        else if(iWhichAuto==eFix3 &&
                Prod.bLinkTo6Tray[eAuto2]==false &&
                Prod.bLinkTo6Tray[eAuto3]==true)
        {
            if(MOT[MManualTray2].FullIC() &&
               MOT[MManualTray3].FullIC())
            {
                #ifdef SOFT_SIMULTE
                    if(CosFunction.bReadClipCodeFromUnloader==true &&
                       IniConfig.bP60ReadClipCodeFromUnloader==true &&
                       LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                        InitNewFixTrayForUnloaderClipRead(eFix3);
                    }
                    else
                    {
                        InitNewFixTray(eFix2, "AOutArm_FullIC");                    //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix3, "AOutArm_FullIC");
                    }
                #else
                    if(LastSet.iRealDummy!=DUMMY)
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)                    //Steven 20140528 : Secs Gem
                        {
                            EventReport(SECS_EVENT.Fix2Full);                   //38     Fix 1 Full
                            EventReport(SECS_EVENT.Fix3Full);
                        }
                        ShowErrorMessage("MES1811", K_RETRY, MManualTray1);     //Fix Tray 2 and 3 is filled with devices.
                    }
                    else if(CosFunction.bReadClipCodeFromUnloader==true &&
                            IniConfig.bP60ReadClipCodeFromUnloader==true &&
                            LastSet.iRealDummy!=DUMMY)
                    {
                        InitNewFixTrayForUnloaderClipRead(eFix2);
                        InitNewFixTrayForUnloaderClipRead(eFix3);
                    }
                    else
                    {
                        InitNewFixTray(eFix2, "AOutArm_FullIC");                //Steven 20160414 : 整合Fix盤設定
                        InitNewFixTray(eFix3, "AOutArm_FullIC");
                    }
                #endif
                bResult=false;
            }
            else
            {
                bScan1By1=true;
            }
        }
        //<==
        //Steven 20181113 : 修正Fix Link顯示問題
        else
        {
            bScan1By1=true;
        }

        if(bScan1By1)
        {
            for(int i=iFixMin; i<=iFixMax; i++)
            {
                iFix=iAutoIndex[i];
                if(MOT[iMMAuto[i]].FullIC())
                {
                    #ifdef SOFT_SIMULTE
                        if(CosFunction.bReadClipCodeFromUnloader==true &&
                           IniConfig.bP60ReadClipCodeFromUnloader==true &&
                           LastSet.iRealDummy!=DUMMY)
                        {
                            InitNewFixTrayForUnloaderClipRead(i);
                        }
                        else
                        {
                            if(MOT[iMMAuto[i]].Tray.HasRealIC())
                            {
                                SaveProductionRecord(&MOT[iMMAuto[i]].Tray, s6TrayName[i]);   //Steven 20200330 : production log by unloader tray存檔
                            }
                            InitNewFixTray(i, "AOutArm_FullIC");                    //Steven 20160414 : 整合Fix盤設定
                        }
                    #else
                        if(LastSet.iRealDummy!=DUMMY)
                        {
                            if(IniConfig.bEnable_SECS_GEM==true)                //Steven 20140528 : Secs Gem
                            {
                                if(i==eFix1)
                                    EventReport(SECS_EVENT.Fix1Full);           //38     Fix 1 Full
                                else if(i==eFix2)
                                    EventReport(SECS_EVENT.Fix2Full);
                                else if(i==eFix3)
                                    EventReport(SECS_EVENT.Fix3Full);
                                else if(i==eFix4)
                                    EventReport(SECS_EVENT.Fix4Full);           //Steven 20230907 : For HT-9011UC
                                else if(i==eFix5)
                                    EventReport(SECS_EVENT.Fix5Full);
                                else if(i==eFix6)
                                    EventReport(SECS_EVENT.Fix6Full);
                            }

                            if(IniConfig.bA68_AutoLoadUnload)                   //JerryYang 20250521 : For AMR
                            {
                                iPortStatus[ePortFix1+i-iFixMin]=eFixFullTray;
                                iLastPortStatus[ePortFix1+i-iFixMin]=eFixFullTray;
                                EventReport(SECS_EVENT.Fix1PortStatusChanged+i-iFixMin);
                            }

                            ShowErrorMessage(sMES1720[iFix], K_RETRY, iMMAuto[i]);
                        }
                        else if(CosFunction.bReadClipCodeFromUnloader==true &&
                                IniConfig.bP60ReadClipCodeFromUnloader==true &&
                                LastSet.iRealDummy!=DUMMY)
                        {
                            InitNewFixTrayForUnloaderClipRead(i);
                        }
                        else
                        {
                            InitNewFixTray(i, "AOutArm_FullIC");                //Steven 20160414 : 整合Fix盤設定
                        }
                    #endif
                    bResult=false;
                }
            }
        }
    }

    if(AUTO3_IS_MAGAZINE==1             &&                                      //JerryYang 20221215 : Magazine把fix區當buffer區功能
       TestIF_File.iMagFixTrayType==1   &&
       iWhichAuto>=iMagMin &&
       iWhichBuff>=iFixMin)
    {
        iAuto=iWhichBuff;
        iFix=iAutoIndex[iWhichBuff];
    }
    else
    {
        iAuto=iWhichAuto;
        iFix=iAutoIndex[iWhichAuto];
    }

    if(Sen[SnFixedTrayDetect[iFix]].IsOff() && LastSet.iRealDummy!=DUMMY)
    {
        if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA &&                            //ChungHung 20140730 fix LA fix3 function 中途按Stop 會秀Alarm
           iWhichAuto==eFix2 &&                                                 //Jimmychiu 20250306 : iFix --> iWhichAuto
           Cylinder[C_FixTray_FullPlace].OffSensor()==false)
        {
            ;
        }
        else
        {
            ShowErrorMessage(sMES1721[iFix], K_RETRY, iMMAuto[iAuto]);
            bResult=false;
        }
    }

    for(int i=iFixMin; i<=iFixMax; i++)
    {
        iFix=iAutoIndex[i];
        if(LastSet.iRealDummy!=DUMMY)
        {
            if(MOT[iMMAuto[i]].fHasTray==false &&
               Sen[SnFixedTrayDetect[iFix]].IsOff()==false)
            {
                if(CosFunction.bReadClipCodeFromUnloader==true &&
                   IniConfig.bP60ReadClipCodeFromUnloader==true &&
                   LastSet.iRealDummy!=DUMMY)
                {
                    if(InitNewFixTrayForUnloaderClipRead(i)==true)
                    {
                        if(IniConfig.bA68_AutoLoadUnload)                               //JerryYang 20250521 : For AMR
                        {
                            iPortStatus[ePortFix1+i-iFixMin]=eFixFTrayArrived;
                            iLastPortStatus[ePortFix1+i-iFixMin]=eFixFTrayArrived;
                            EventReport(SECS_EVENT.Fix1PortStatusChanged+i-iFixMin);
                        }
                    }
                    else
                    {
                        bResult=false;
                    }
                }
                else
                {
                    InitNewFixTray(i, "AOutArm");                                   //Steven 20160414 : 整合Fix盤設定
                    if(IniConfig.bA68_AutoLoadUnload)                               //JerryYang 20250521 : For AMR
                    {
                        iPortStatus[ePortFix1+i-iFixMin]=eFixFTrayArrived;
                        iLastPortStatus[ePortFix1+i-iFixMin]=eFixFTrayArrived;
                        EventReport(SECS_EVENT.Fix1PortStatusChanged+i-iFixMin);
                    }
                }
            }
        }
        else
        {
            if(MOT[iMMAuto[i]].fHasTray==false)
            {
                if(CosFunction.bReadClipCodeFromUnloader==true &&
                   IniConfig.bP60ReadClipCodeFromUnloader==true &&
                   LastSet.iRealDummy!=DUMMY)
                {
                    if(InitNewFixTrayForUnloaderClipRead(i)==false)
                    {
                        bResult=false;
                    }
                }
                else
                {
                    InitNewFixTray(i, "AOutArm");                                   //Steven 20160414 : 整合Fix盤設定
                }
            }
        }
    }
    return bResult;
}
//------------------------------------------------------------------------------
extern bool DoPickFromShuttle_9045_1x1_1(int iSht);
extern bool DoPickFromShuttle_9045_1x2_2(int iSht);
extern bool DoPickFromShuttle_9045_1x2_4(int iSht);
extern bool DoPickFromShuttle_9045_1x3_2_14(int iSht);
extern bool DoPickFromShuttle_9045_1x3_4(int iSht);
extern bool DoPickFromShuttle_9045_1x4_2(int iSht);
extern bool DoPickFromShuttle_9045_1x4_4(int iSht);
extern bool DoPickFromShuttle_9045_1x4_4S(int iSht);
extern bool DoPickFromShuttle_9045_1x4_8(int iSht);
extern bool DoPickFromShuttle_9045_2x1_2(int iSht);
extern bool DoPickFromShuttle_9045_2x2_4(int iSht);
extern bool DoPickFromShuttle_9045_2x2_8(int iSht);
extern bool DoPickFromShuttle_9045_2x3_6_14(int iSht);
extern bool DoPickFromShuttle_9045_2x3_6(int iSht);
extern bool DoPickFromShuttle_9045_2x4_4(int iSht);
extern bool DoPickFromShuttle_9045_2x4_8(int iSht);
extern bool DoPickFromShuttle_9045_2x5_8(int iSht);
extern bool DoPickFromShuttle_9045_2x6_8(int iSht);
extern bool DoPickFromShuttle_9045_2x8_8(int iSht);
//------------------------------------------------------------------------------
bool DoPickFromShuttle_9045(int iSht)
{
    AnsiString Str;
    bool bRet=false;
    if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        bRet=DoPickFromShuttle_9045_1x1_1(iSht);
    }
    else if(iInArmType==e9045_1x2_2_13 ||
            iInArmType==e9045_1x2_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x2_2(iSht);
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        bRet=DoPickFromShuttle_9045_1x2_4(iSht);
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x3_2_14(iSht);
    }
    else if(iInArmType==e9045_1x3_4)
    {
        bRet=DoPickFromShuttle_9045_1x3_4(iSht);
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        bRet=DoPickFromShuttle_9045_1x4_4S(iSht);
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x4_2(iSht);
    }
    else if(iInArmType==e9045_1x4_4_Back ||
            iInArmType==e9045_1x4_4)
    {
        bRet=DoPickFromShuttle_9045_1x4_4(iSht);
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        bRet=DoPickFromShuttle_9045_1x4_8(iSht);
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        bRet=DoPickFromShuttle_9045_2x1_2(iSht);
    }
    else if(iInArmType==e9045_2x2_4_12 ||
            iInArmType==e9045_2x2_4_13 ||
            iInArmType==e9045_2x2_4_14)
    {
        bRet=DoPickFromShuttle_9045_2x2_4(iSht);
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        bRet=DoPickFromShuttle_9045_2x2_8(iSht);
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        bRet=DoPickFromShuttle_9045_2x3_6_14(iSht);
    }
    else if(iInArmType==e9045_2x3_6)
    {
        bRet=DoPickFromShuttle_9045_2x3_6(iSht);
    }
    else if(iInArmType==e9045_2x4_4_13 ||
            iInArmType==e9045_2x4_4_14)
    {
        bRet=DoPickFromShuttle_9045_2x4_4(iSht);
    }
    else if(iInArmType==e9045_2x4_8)
    {
        bRet=DoPickFromShuttle_9045_2x4_8(iSht);
    }
    else if(iInArmType==e9045_2x5_8)
    {
        bRet=DoPickFromShuttle_9045_2x5_8(iSht);
    }
    else if(iInArmType==e9045_2x6_8)
    {
        bRet=DoPickFromShuttle_9045_2x6_8(iSht);
    }
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        bRet=DoPickFromShuttle_9045_2x8_8(iSht);
    }
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoPickFromShuttle_9045()", Str);
    }
    return bRet;
}
//------------------------------------------------------------------------------
//==============================================================================
//AI(ht9045-v899) 20260810: OutArm 逐輪診斷 Log 實作。宣告與用途見 aoutarm9045.h
static AnsiString sOutArmRoundLog[OUTARM_ROUND_LOG_MAX];
static int iOutArmRoundHead   =-1;                                              //目前輪在 ring 的位置
static int iOutArmRoundSeq    =0;                                               //累計輪次序號, 不隨 ring 繞回
static int iOutArmRoundEnable =-1;                                              //-1=尚未讀 0=關 1=開
//------------------------------------------------------------------------------
bool OutArmRoundLog_Enabled()
{
    if(iOutArmRoundEnable<0)
    {
        try
        {
            AnsiString sPath=AuthPath+"config.ini";
            iOutArmRoundEnable=CheckAndReadIniData(sPath, "Debug", "bOutArmRoundLog", 1);
        }
        catch(...)
        {
            iOutArmRoundEnable=0;
        }
    }
    return (iOutArmRoundEnable==1);
}
//------------------------------------------------------------------------------
int OutArmRoundLog_Seq()
{
    return iOutArmRoundSeq;
}
//------------------------------------------------------------------------------
void OutArmRoundLog_Line(AnsiString s)
{
    if(OutArmRoundLog_Enabled()==false)
        return;
    try
    {
        if(iOutArmRoundHead<0)
            return;
        SYSTEMTIME st;
        GetLocalTime(&st);
        AnsiString sT;
        sT.sprintf("  %02d:%02d:%02d.%03d ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        sOutArmRoundLog[iOutArmRoundHead]+=sT+s+"\r\n";
    }
    catch(...)
    {
    }
}
//------------------------------------------------------------------------------
void OutArmRoundLog_Begin(int iSht, int iKit)
{
    if(OutArmRoundLog_Enabled()==false)
        return;
    try
    {
        iOutArmRoundSeq++;
        iOutArmRoundHead=(iOutArmRoundHead+1)%OUTARM_ROUND_LOG_MAX;
        SYSTEMTIME st;
        GetLocalTime(&st);
        AnsiString s;
        s.sprintf("==== Round #%d  Sht=%d  Kit=%d  %04d-%02d-%02d %02d:%02d:%02d.%03d ====\r\n",
                  iOutArmRoundSeq, iSht, iKit,
                  st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        sOutArmRoundLog[iOutArmRoundHead]=s;
        s.sprintf("CFG iInArmType=%d iPickRow=%d iPickCol=%d iMaxRow=%d iMaxCol=%d iShtCol=%d "
                  "iXStep=%d iYStep=%d iModeX=%d iPickKitStep=%d iShtKitStep=%d",
                  iInArmType, OutArmSuck.iPickRow, OutArmSuck.iPickCol,
                  OutArmSuck.iMaxRow, OutArmSuck.iMaxCol, OutArmSuck.iShtCol,
                  OutArmSuck.iXStep, OutArmSuck.iYStep, OutArmSuck.iModeX,
                  OutArmSuck.iPickKitStep, OutArmSuck.iShtKitStep);
        OutArmRoundLog_Line(s);
        s.sprintf("CFG i2x2Suck=%d i2x2Suck_Out=%d USE_ROTATE_KIT=%d iRotate_Type=%d "
                  "ActiveRotate=%d RotKitIn=%d RotKitOut=%d bE47_4Offset=%d iE50Opt=%d",
                  i2x2Suck, i2x2Suck_Out, USE_ROTATE_KIT, iRotate_Type,
                  (int)tRotate.ActiveRotate,
                  TrayForm.iRotateKIT_InputType, TrayForm.iRotateKIT_OutputType,
                  (int)IniConfig.bE47_ShuttleUse4Offset,
                  IniConfig.iE50_OutArmPickUpErrorOption);
        OutArmRoundLog_Line(s);
    }
    catch(...)
    {
    }
}
//------------------------------------------------------------------------------
static void OutArmRoundLog_WriteTo(AnsiString sFile, AnsiString sReason)
{
    try
    {
        FILE *fp=fopen(sFile.c_str(), "w");
        if(fp==NULL)
            return;
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(fp, "==== HT9045 OutArm Round Log ====\n");
        fprintf(fp, "Dump: %04d-%02d-%02d %02d:%02d:%02d  Reason: %s\n",
                st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
                sReason.c_str());
        fprintf(fp, "Keep: last %d rounds, newest last. Seq=%d\n\n",
                OUTARM_ROUND_LOG_MAX, iOutArmRoundSeq);
        for(int k=1; k<=OUTARM_ROUND_LOG_MAX; k++)                              //由最舊排到最新
        {
            int idx=(iOutArmRoundHead+k)%OUTARM_ROUND_LOG_MAX;
            if(sOutArmRoundLog[idx].Length()>0)
                fprintf(fp, "%s\n", sOutArmRoundLog[idx].c_str());
        }
        fflush(fp);
        fclose(fp);
    }
    catch(...)
    {
    }
}
//------------------------------------------------------------------------------
void OutArmRoundLog_Flush(AnsiString sReason)
{
    if(OutArmRoundLog_Enabled()==false)
        return;
    try
    {
        if(DirectoryExists("D:\\HT9045\\Error")==false)
            ForceDirectories("D:\\HT9045\\Error");
        OutArmRoundLog_WriteTo("D:\\HT9045\\Error\\OutArmRoundLog.txt", sReason);
    }
    catch(...)
    {
    }
}
//------------------------------------------------------------------------------
void OutArmRoundLog_Dump(AnsiString sBasePath)
{
    if(OutArmRoundLog_Enabled()==false)
        return;
    OutArmRoundLog_WriteTo(sBasePath+"\\OutArmRoundLog.txt", "State Record");
}
//==============================================================================
bool bOutRotator;
bool bDoAOI;
bool bDoFixAI;                                                                  //Sam 20211220 : 整合 Fix AI AOI
int iOutArmAdditionalFunctionTask;
//------------------------------------------------------------------------------
void InitDoOutArmAdditionalFunction()
{
    iOutArmAdditionalFunctionTask=1;
}
//------------------------------------------------------------------------------
bool CheekNeedToDoOutArmAdditionalFunction()                                    //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    bool bResult=false;
    bool bSupportFixAI=false;
    if(OutArmSuck.bAlreadyRotate==false)
    {
        if(CheckRotateOutNotFinish())
        {
            bOutRotator=true;
            bResult=true;
        }
        else if(USE_ROTATE_KIT==1 &&
                tRotate.ActiveRotate &&
                TrayForm.iRotateKIT_InputType!=0 &&
                (iOutRotateFinish==0 || iOutRotateFinish==1))
        {
            bOutRotator=true;
            bResult=true;
        }
    }

    if(CosFunction.bART_RT_NoRotate && tRotate.bART_RT_NoRotate && bCanRunSCKART)                //Sam 20240809 : ART RT No Rotate
        bOutRotator=false;
    if(OutArmSuck.bAlreadyAOI==false)
    {
        if(tAOISetup.bEnabledAOI ||                                             //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
           (USE_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableScannerMode!=0) ||
           (USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0))
        {
            bDoAOI=true;
            bResult=true;
        }
    }

    if(OutArmSuck.bAlreadyFixAI==false)                                         //Sam 20211220 : 整合 Fix AI AOI
    {
        if(TestIF.iTestMode==SingleSite                         ||
           TestIF.iTestMode==QualSite2X2N                       ||
           TestIF.iTestMode==DualSite)
        {
            bSupportFixAI=true;
        }
        else if(TestIF.iTestMode==QualSite2X2)
        {
            if(USE_PICKER_COUNT==0)
            {
                bSupportFixAI=true;
            }
            else if(TestIF.iUseSuckMode==4 && TestIF.dSiteXPitch>iXpitchMinX2)
            {
                bSupportFixAI=true;
            }
            else
            {
                if(TestIF.iUseSuckMode==8 &&
                   TestIF.dSiteXPitch>iXpitchMinX2)                             //Sam 20231107 : 補 Fix AOI 模式
                {
                    bSupportFixAI=true;
                }
            }
        }
       else if(TestIF.iTestMode==QualSite1X4 ||                                 //Sam 20220613: 修正 FixAOI 未動作問題
               TestIF.iTestMode==_12Site2X6  ||                                 //Sam 20240329 : 新增 FixAOI 模式
               TestIF.iTestMode==_16Site2X8)
        {
            //if(TestIF.iUseSuckMode==4)                                        //Sam 20231107 : 補 Fix AOI 模式 Mark
            {
                bSupportFixAI=true;
            }
        }
        else if(TestIF.iTestMode==_8Site2X4 ||
                TestIF.iTestMode==_16Site4X4)
        {
            if(ArmCanSuck4IC(0, true)==false || USE_PICKER_COUNT==0)
            {
            }
            else
            {
               bSupportFixAI=true;
            }
        }

        if(bSupportFixAI &&
           USE_Fix_AI_CCD &&
           TestIF_File.bEnableFix2BGAAICCD &&
           fFixAICCD->NeedToGrabImage())                                        //KaiChen 20190508 ：矽格湖口 AI CCD Function
        {
            bDoFixAI=true;
            bResult=true;
        }
    }

    if(bResult)
    {
        InitDoOutArmAdditionalFunction();
    }

    //AI(ht9045-v899) 20260810: 記錄旋轉/AOI 閘門的判定結果, 讓「為什麼沒去旋轉」有跡可循
    {
        AnsiString sLog;
        sLog.sprintf("ROT-GATE bAlreadyRotate=%d bOutRotator=%d iOutRotateFinish=%d "
                     "bDoAOI=%d bDoFixAI=%d NeedAdditional=%d",
                     (int)OutArmSuck.bAlreadyRotate, (int)bOutRotator, iOutRotateFinish,
                     (int)bDoAOI, (int)bDoFixAI, (int)bResult);
        OutArmRoundLog_Line(sLog);
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoOutArmAdditionalFunction()                                               //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    bool bResult=false;
    int &Task=iOutArmAdditionalFunctionTask;

    switch(iOutArmAdditionalFunctionTask)
    {
        case 1:
            CheekNeedToDoOutArmAdditionalFunction();
            Task=100;
        case 100:
            if(MoveOutArmToAutoSafe())
            {
                CheckOutArmSuckICFallDown();
                MOT[MInShuttle1].fCanMoveR=true;
                MOT[MInShuttle2].fCanMoveR=true;
                if(OutArmSuck.bAlreadyRotate==false && bOutRotator)
                {
                    if(iOutRotateFinish==0)
                    {
                        InitialOutArmRotateKIT();
                        iOutRotateFinish=1;
                    }
                    Task=10000;
                }
                else if(OutArmSuck.bAlreadyAOI==false && bDoAOI)
                {
                    InitAOIFunction();
                    Task=20000;
                }
                else if(OutArmSuck.bAlreadyFixAI==false && bDoFixAI)
                {
                    fFixAICCD->Fix2AICCDFunction();
                    Task=30000;
                }
                else
                {
                    bResult=true;
                }
            }
            break;
        case 10000:
            if(DoOutArmRotateKIT()==true)
            {
                OutArmSuck.bAlreadyRotate==true;
                bOutRotator=false;
                iOutRotateFinish=2;                                             //kevin 20130524 目前放到 auto tray
                Task=100;
            }
            break;
       case 20000:
            if(DoAOIFunction()==true)
            {
                OutArmSuck.bAlreadyAOI=true;
                bDoAOI=false;
                Task=100;
            }
            break;
       case  30000:
            if(fFixAICCD->DoFix2AICCDFunction()==true)
            {
                OutArmSuck.bAlreadyFixAI=true;
                bDoFixAI=false;
                Task=100;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void PreSetOutAdditionalFlag()                                                  //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    if(USE_ROTATE_KIT==1 && tRotate.ActiveRotate==true && MOT[MOutRotateKit].HasRealIC())
    {
        OutArmSuck.bAlreadyRotate=false;
    }
    else
    {
        OutArmSuck.bAlreadyRotate=(USE_ROTATE_KIT==1 && tRotate.ActiveRotate==true)?false:true;          //JerryYang 20230503 : 修正out rotate不作動的問題
    }
    OutArmSuck.bAlreadyAOI  =(tAOISetup.bEnabledAOI || (USE_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableScannerMode!=0))?false:true;
    OutArmSuck.bAlreadyFixAI=(USE_Fix_AI_CCD && TestIF_File.bEnableFix2BGAAICCD && fFixAICCD->NeedToGrabImage())?false:true;   //Sam 20211220 : 整合 Fix AI AOI
}
//==============================================================================
bool DoMoveOutArmXYToPlace_9045(bool &bCanPick2ICAtOnceTime, int iRow, int iCol, bool RealMove)
{
    int iOutPutTray=0;
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=AutoCalculateOutArmXClosePitch(iWhichAuto+bOutArmXOverLimit);                 //Steven 20251101 : 修正超出極限後, 要重新算一次X-Pitch
    int iMovePitchY=AutoCalculateOutArmYClosePitch(iWhichAuto);
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    bool bNeedDown=true;
    bool bFlag=false;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(AUTO3_IS_MAGAZINE==1             &&                                      //JerryYang 20221215 : Magazine把fix區當buffer區功能
       TestIF_File.iMagFixTrayType==1   &&
       iWhichAuto>=iMagMin)
    {
        iOutPutTray=iWhichBuff;
    }
    else
    {
        iOutPutTray=iWhichAuto;
    }

    if(IsTrayArmMoveAvoidOutArmCrash())
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    bFlag=GetOutArmToUnLoaderPosition(bCanPick2ICAtOnceTime, iOutPutTray, iXPos, iYPos, iRow, iCol, RealMove);

    if(bCanPick2ICAtOnceTime==false)
        iMovePitchX=AutoCalculateOutArmXClosePitch(iWhichAuto, bOutArmXOverLimit);  //Steven 20240501 : 修正超出極限後, 要重新算一次X-Pitch

    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                           //Steven 20190520 : Fixed Y-Pitch position error  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iOutArmPlaceOrder==0)
            iYPos=iYPos-iMovePitchY;
    }
    else
    {
        if(iOutArmPlaceOrder!=0)
            iYPos+=Prod.iOutARM_Y_PITCH;
    }

    if(iYPos<(MOT[MOutArmY].Motor->PSoftLimitN+10))                             //JerryYang 20180221 (Steven) : fix y-pitch 最小25mm會有行程的問題, y-pitch要縮到最小
    {
        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(iOutArmPlaceOrder==0)
            {
                iYPos=iYPos+iMovePitchY;
                iMovePitchY=IN_OUT_ARM_Y_PITCH_MIN;
                iYPos=iYPos-iMovePitchY;
                iYVariable=GetOutArmPitchY_9045(iMovePitchY);
            }
        }
    }

    if(RealMove==false)
        return bFlag;

    if(iXPos<(MOT[MOutArmX].Motor->PSoftLimitN+10) ||                           //jou 2010-02-01 fix 3 吸嘴超過 軟體極限時 , 設成 has null ic
       iXPos>(MOT[MOutArmX].Motor->PSoftLimitP-10))
    {
        if(MOT[iMMAuto[iOutPutTray]].Tray.Data[iCol][iRow]==NULL_IC)            //ChungHung 20111215 嘗試將Fix3放滿
        {
            MOT[iMMAuto[iOutPutTray]].SetTraySingleData(iCol, iRow, HAS_NULL_IC);
            ReserveEmptyPoint(iMMAuto[iOutPutTray], iCol, iRow);
        }
        if(iHWFix_BinBox==1 && iWhichAuto==eBulkBox)
        {
            ShowErrorMessage("WAR16318", 0, MMSystem, 0, "623939");             //kevin 20210401 超出極限秀ALARM 避免HANG UP
        }
        return false;
    }

    if(IniConfig.bFix3PutAllFullIC)                                             //ChungHung 20111215 嘗試將Fix3放滿
    {
        if(iWhichAuto==iFixRight)
            ReversionEmptyPoint();
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
    {
        if(iWhichAuto>=0 && iWhichAuto<=iAutoRight)
            iOffsetPos=OutOfsAuto1;
        else
            iOffsetPos=OutOfsFix1;
    }
    else
    {
        iOffsetPos=OutOfsAuto1+iOutPutTray;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    if(MOT[iMMAuto[iOutPutTray]].fHasTray==false ||
       MOT[iMMAuto[iOutPutTray]].FullIC())
        bNeedDown=false;
    GetOutArmZDownPos_9045(iWhichAuto, bNeedDown, bZFlag, iZPos);

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//==============================================================================
bool SearchUnLoadTrayUpDown_9045(bool &bCanPick2ICAtOnceTime, int &iRow, int &iCol)
{
    if(AUTO3_IS_MAGAZINE==1             &&                                      //JerryYang 20221215 : Magazine把fix區當buffer區功能
       TestIF_File.iMagFixTrayType==1   &&
       iWhichAuto>=iMagMin)
    {
        AdjustOutArmClosePitchCondition(iWhichBuff, bCanPick2ICAtOnceTime);
    }
    else
    {
        AdjustOutArmClosePitchCondition(iWhichAuto, bCanPick2ICAtOnceTime);
    }
    if(bOutArmXOverLimit)                                                       //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
        bCanPick2ICAtOnceTime=false;

    if(bCanPick2ICAtOnceTime==true)
    {
        int ct=Find_OutArm_PickerMaxUseCountOnTime(OutArmSuck.iPickCol, iRow, iCol);
        if(ct<=1)
        {
            bCanPick2ICAtOnceTime=false;
        }
        else if(iWhichAuto==iAutoRight)
        {
            if(DoMoveOutArmXYToPlace_9045(bCanPick2ICAtOnceTime, iRow, iCol, false)==true)
                return true;
            bCanPick2ICAtOnceTime=false;
        }
        else
        {
            SearchBinBoxUpDown(bCanPick2ICAtOnceTime, iRow, iCol);              //Steven 20230601 : 重新整理Bin Box
            return true;
        }
    }
//    else                                                                      //JerryYang 20250917 : Mark掉
//    {
        Find_OutArm_Single(OutArmSuck.iPickCol, iRow, iCol);
//    }
    return true;
}
//==============================================================================
bool SetOutArm_9045()
{
    int iCol=0, iRow=0;
    bool bCanPick2ICAtOnceTime;
    if(IsTrayArmMoveAvoidOutArmCrash())
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }
    if(SearchUnLoadTrayUpDown_9045(bCanPick2ICAtOnceTime, iRow, iCol)==false)
        return false;

    bOverTray=false;
    if(iCol<0 || iRow<0)                                                            //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
    {
        bOverTray=true;
        return false;
    }

    if(TRAY_ARM_MODE==eAboveCoveyor &&
       MOT[MTrayX].ReadPos()>Prod.iXTrayColor)
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    return DoMoveOutArmXYToPlace_9045(bCanPick2ICAtOnceTime, iRow, iCol, true);
}
//------------------------------------------------------------------------------
bool DoOutArmPlaceToAuto(int iToWhichAuto)                                      //Steven 20110216
{
    static bool bSuckDuplicateErr[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};  //Steven 20091218 : Avoid duplicate message
    int Motor=iMMAuto[iToWhichAuto];
    int iXPos=0, iYPos=0;
    int iXPitchPos=0, iXPitch2Pos=0, iYPitchPos=0;                              //JerryYang 20210119 : add x,y pitch位置
    int iTrayRow=0, iTrayCol=0;
    bool flag=true;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    AnsiString ErrPart=" ", str="";
    AnsiString asCheck="";                                                      //2012.06.01 Q_Q
    AnsiString asOCRCode="";                                                    //wei 20161005 紀錄OCR Code
    AnsiString asInfo="";

    iOutArmYStep=ChangeToFloatNonPcnt((double)(AutoCalculateOutArmYClosePitch(iToWhichAuto)), (double)(AutoForm[iWhichAuto]->YPitch));     //ChungHung 20131231 alter AutoYPitch
    if((USE_OUT_Y_IS_AUTO_PITCH==false && TestIF.bSearchLastMode==false) ||     //JerryYang 20251218 : IN/OUT ARM支援不同模組
       UnloaderTrayYDirForArmYPitch(iToWhichAuto)==false ||                     //ChungHung 20131231 alter AutoYPitch
       iOutArmPlaceOrder==1)
    {
        iOutArmYStep=0;                                                         //kevin 20171120 (wei) 判斷LOAD TRAY Y PITCH是否是 ARM Y PITCH可以一次8吸嘴吸IC
    }

    if(AUTO3_IS_MAGAZINE==1             &&                                      //JerryYang 20221215 : Magazine把fix區當buffer區功能
       TestIF_File.iMagFixTrayType==1   &&
       iToWhichAuto>=iMagAtAuto)
    {
        Motor=iMMAuto[iWhichBuff];
    }
    else
    {
        Motor=iMMAuto[iToWhichAuto];
    }

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            if(bOutArmSuckActive[i][j])
            {
                if(OutArmSuck.Item[i][j]==HAS_NULL_IC)                          //Steven 20170113 : 沒IC的地方不檢查吹氣
                {
                    OutArmSuck.SetItemData(i, j, NULL_IC);
                }
                else
                {
                    flag=OutArmSuck.Suck[i][j].Destroy();
                    if(flag)
                    {
                        iTrayCol=iOutArmXPosition[i][j];
                        iTrayRow=iOutArmYPosition+iOutArmYStep*i;
                        MOT[Motor].Tray.iWhichSite [iTrayCol][iTrayRow]=OutArmSuck.iWhichSite[i][j];
                        MOT[Motor].Tray.iWhichIndex[iTrayCol][iTrayRow]=OutArmSuck.iWhichIndex[i][j];
                        MOT[Motor].Tray.iBinCode   [iTrayCol][iTrayRow]=OutArmSuck.Item[i][j];
                        MOT[Motor].Tray.iWhichAuto [iTrayCol][iTrayRow]=OutArmSuck.iWhichAuto[i][j];      //JerryYang 20221215 : Magazine把fix區當buffer區功能
                        MOT[Motor].Tray.iBinData   [iTrayCol][iTrayRow]=OutArmSuck.iBinData[i][j];
                        MOT[Motor].Tray.iAOIResult [iTrayCol][iTrayRow]=OutArmSuck.iAOIResult[i][j];      //Sam 20240325 : 新增 DamageTrayMapping 功能

                        fTesterTCP->PlaceOSTestResultToTray(i, j, iTrayRow, iTrayCol, OutArmSuck.iWhichAuto[i][j]);     //Steven 20250515 : 整合Open Short測試報表
                        InspectOutArmPosition(iToWhichAuto, OutArmSuck.Suck[i][j].iMyRow, OutArmSuck.Suck[i][j].iMyCol, iTrayRow, iTrayCol, iOutPlaceToAuto);

                        if(OutArmSuck.Item[i][j]>=START_TEST)
                        {
                            if(OutArmSuck.iWhichSite[i][j]>=0 &&
                               OutArmSuck.iWhichSite[i][j]<32)                  //Steven 20240320 : 避免溢位
                                LotSummary.AddCount(OutArmSuck.iWhichSite[i][j], OutArmSuck.Item[i][j]-TEST_PASS);

                            if(fSCKART->iInfo_MultiLotCnt>1)
                            {
                                LotSummary.AddByLotCount(OutArmSuck.iWhichSite[i][j], OutArmSuck.Item[i][j]-TEST_PASS, OutArmSuck.cDeviceInf[i][j]);
                            }
                        }

                        if(LastSet.iTester==_2D_SORT &&
                           CosFunction.bSortingBy2DList &&
                           TestIF_File.bSortingBy2DIDList)                      //JerryYang 20230814 : 有開2D SORT才存
                        {
                            if(CUSTOMER_CODE==CC_QUALCOMM)                      //RogerYang 20260121 : Add for RF360 2DID sorting 塞bin結果
                            {
                                sl2DMappingLog->MyInsert2DMappingToFile(OutArmSuck.cDeviceInf[i][j], OutArmSuck.PordRec[i][j].asBuffer->Strings[eTestCate].ToIntDef(-1));
                            }
                            else
                            {
                                sl2DMappingLog->MyInsert2DMappingToFile(OutArmSuck.cDeviceInf[i][j], 1); //JerryYang 20230322 : add 2D mapping result
                            }
                        }

                        if(OutArmSuck.Item[i][j]>=START_TEST)                   //Steven 20171031 (wei) : Add TTMyTray滑鼠滑過去顯示Hint
                        {
                            if(TestIF_File.bEnableBarCode)
                            {
                                asCheck.sprintf("%s", OutArmSuck.cReDeviceInf[i][j]);
                                if(asCheck==0 || asCheck=="")
                                    asInfo.sprintf("BIN: %d, 2DID: ERROR %s", OutArmSuck.Item[i][j]-TEST_PASS, OutArmSuck.cDeviceInf[i][j]);
                                else
                                    asInfo.sprintf("BIN: %d, 2DID: %s", OutArmSuck.Item[i][j]-TEST_PASS, OutArmSuck.cDeviceInf[i][j]);
                            }
                            else
                            {
                                asInfo.sprintf("BIN: %d", OutArmSuck.Item[i][j]-TEST_PASS);
                            }
                        }
                        else
                        {
                            if(TestIF_File.bEnableBarCode)
                            {
                                asCheck.sprintf("%s", OutArmSuck.cReDeviceInf[i][j]);
                                if(asCheck==0 || asCheck=="")
                                    asInfo.sprintf("BIN: %d, 2DID: ERROR %s", OutArmSuck.Item[i][j]-TEST_PASS, OutArmSuck.cDeviceInf[i][j]);
                                else
                                    asInfo.sprintf("BIN: %d, 2DID: %s", OutArmSuck.Item[i][j], OutArmSuck.cDeviceInf[i][j]);
                            }
                            else
                            {
                                asInfo.sprintf("BIN: %d", OutArmSuck.Item[i][j]);
                            }
                        }

                        if((OutArmSuck.Item[i][j]-TEST_PASS)>iTestBinCount)     //error bin
                            iByBinTotal[iTestBinCount]++;                       //kevin 20180705 (wei) bin 數量 Bin total[0]
                        else
                            iByBinTotal[OutArmSuck.Item[i][j]-TEST_PASS]++;     //kevin 20180705 (wei) bin 數量 Bin total[0]

                        if(CosFunction.bTrayOCR && TestIF.bOcrFunction)         //wei 20161005 紀錄OCR Code
                            asOCRCode=OutArmSuck.cDeviceInf[i][j];
                        else
                            asOCRCode="na";

//                        if(IniConfig.bN14_9_ULQtyReport==true)
//                        {
//                            if(iToWhichAuto>=iFixRight)
//                                LastSet.iN14_9_TrayICCnt[iToWhichAuto-3]++;     //Sam 20190801 : Bug Fix
//                            else
//                                LastSet.iN14_9_TrayICCnt[iToWhichAuto]++;       //Sam 20190801 : Bug Fix
//                        }

                        iXPos=MOT[MOutArmX].ReadPos();                          //Frank 20160727 add iXPos iYPos
                        iYPos=MOT[MOutArmY].ReadPos();                          //Frank 20160727 add iXPos iYPos
                        iXPitchPos=MOT[MOutArmPitch].ReadPos();                 //JerryYang 20210119 : add x,y pitch位置
                        if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable ||             //JerryYang 20251218 : IN/OUT ARM支援不同模組
                           USE_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)        //Ztex 2024.02.24 Add HT-1132
                        {
                            iXPitch2Pos=MOT[MOutArmPitchX2].ReadPos();
                            iYPitchPos=MOT[MOutArmPitchY].ReadPos();
                        }
                        else if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||     //QQ  //JerryYang 20251218 : IN/OUT ARM支援不同模組
                                USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)        //Ztex 2023.12.06 Add HT-1032
                        {
                            iXPitch2Pos=MOT[MOutArmPitchX2].ReadPos();
                            iYPitchPos =MOT[MOutArmPitchY].ReadPos();
                        }

                        bOutArmCheckDestroyACT[i][j]=true;                      //kevin 20140220 回吸檢測 jou 981130 確認device確實destroy完成

                        if(TestIF_File.bEnableBarCode)
                        {
                            asCheck.sprintf("%s", OutArmSuck.cReDeviceInf[i][j]);
                            if(asCheck==0 || asCheck=="")
                                MOT[Motor].SetTrayBinData(iTrayCol, iTrayRow, HAS_IC, asInfo);              //ChungHung 20131231 alter AutoYPitch
                            else
                                MOT[Motor].SetTrayBinData(iTrayCol, iTrayRow, HAS_BARCODEERROR_IC, asInfo); //ChungHung 20131231 alter AutoYPitch

                            if(LastSet.iRealDummy==REALLY  &&                   //jou 20190930 : Barcode Tray record file
                               TestIF_File.bEnableBarCode==true)                //jou 20191008 : Barcode duplicate file by out arm
                            {
                                if(CosFunction.bBarcodeTrayRecFile==true)
                                    MOT[Motor].Tray.cDeviceInf[iTrayCol][iTrayRow]=OutArmSuck.cDeviceInf[i][j];

                                if(CosFunction.bBarcodeDuplicateFileByOutArm==true && TestIF_File.bCheckCodeByLot==true &&
                                   OutArmSuck.Item[i][j]!=HAS_NULL_IC && OutArmSuck.Item[i][j]!=NULL_IC &&
                                   OutArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend && OutArmSuck.cDeviceInf[i][j]!="")
                                {
                                    fBarCode->list2DByLot->Clear();
                                    fBarCode->map2DList[OutArmSuck.cDeviceInf[i][j]]=i;
                                    fBarCode->list2DByLot->Add(OutArmSuck.cDeviceInf[i][j]);
                                    WriteDataToFile(asBarCodeLot, fBarCode->list2DByLot->Text);   //Steven 20160429 : 開程式要把2D List讀回來
                                }
                            }
                        }
                        else
                        {
                            if(IniConfig.bBinBox==true &&                       //jou 2012-12-11 support Bin Box
                               iToWhichAuto==iBinBoxAtFix &&
                               AutoForm[iBinBoxAtFix]->iTrayType==iBinBoxType)
                            {
                                LastSet.iBinBoxCount++;
                            }
                            else
                            {
                                MOT[Motor].SetTrayBinData(iTrayCol, iTrayRow, HAS_IC, asInfo);  //ChungHung 20131231 alter AutoYPitch
                            }
                        }

                        bOutArmSuckActive[i][j]=false;

                        if(AUTO3_IS_MAGAZINE==1             &&                  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                           TestIF_File.iMagFixTrayType==1   &&
                           iToWhichAuto>=iMagAtAuto)
                        {

                        }
                        else
                        {
                            for(int k=0; k<4; k++)
                            {
                                LastSet.BinCT[k][iTo3Unload[OutArmSuck.iWhichAuto[i][j]]]++;

                                if(IniConfig.bVTESTFunction==true &&            //marvin 20200424 (Kirin) Added always record report by time.
                                   IniConfig.bCheckFile==true)
                                {
                                    #ifdef SOFT_SIMULTE
                                    if(OutArmSuck.iWhichAuto[i][j]<=iFixRightHalf)
                                        LastSet.iBinCTForAlways[k][OutArmSuck.iWhichAuto[i][j]]++;
                                    #else
                                    if(LastSet.iTester==ON_LINE &&
                                       OutArmSuck.iWhichAuto[i][j]<=iFixRightHalf)
                                        LastSet.iBinCTForAlways[k][OutArmSuck.iWhichAuto[i][j]]++;
                                    #endif
                                }

                                if(CosFunction.bSpecailLowYeild &&
                                   Prod.bFailAlarmLowYieldSpecial)              //Sam 20210505 : PTI 要求的兩段 Low Yeild
                                    LastSet.BinCT_PTI[k][OutArmSuck.iWhichAuto[i][j]]++;

                                if(CUSTOMER_CODE==CC_TERAPOWER &&
                                   Prod.bD22SupportMultiDoubleContact)          //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
                                {
                                    if(OutArmSuck.iBinDataBackUp[i][j]<0 ||
                                       OutArmSuck.iBinDataBackUp[i][j]>iTestBinCount)  //Steven 20260316 : 修正 bin code 超出範圍問題
                                        LastSet.iBinData32[k][iTestBinCount]++;
                                    else
                                        LastSet.iBinData32[k][OutArmSuck.iBinDataBackUp[i][j]]++;
                                }
                                else
                                {
                                    if(OutArmSuck.iBinData[i][j]<0)             //jou 20170606 (wei) : 修正iBinData陣列被填入-1
                                        LastSet.iBinData32[k][iTestBinCount]++;
                                    else
                                        LastSet.iBinData32[k][OutArmSuck.iBinData[i][j]]++;
                                }
                                if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount) //kevin 20150615   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                                {
                                    if(CUSTOMER_CODE==CC_TERAPOWER &&
                                       Prod.bD22SupportMultiDoubleContact)      //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
                                    {
                                        if(OutArmSuck.iBinDataBackUp[i][j]<0)
                                            LastSet.iBinData32_ART[k][iTestBinCount]++;
                                        else
                                            LastSet.iBinData32_ART[k][OutArmSuck.iBinDataBackUp[i][j]]++;
                                    }
                                    else
                                    {
                                        LastSet.BinCT_ART[k][iTo3Unload[OutArmSuck.iWhichAuto[i][j]]]++;
                                        if(OutArmSuck.iBinData[i][j]<0)         //jou 20170606 (wei) : 修正iBinData陣列被填入-1
                                            LastSet.iBinData32_ART[k][iTestBinCount]++;
                                        else
                                            LastSet.iBinData32_ART[k][OutArmSuck.iBinData[i][j]]++;
                                    }
                                }
                            }

                            if(bCanRunSCKART==true)                             //Steven 20161201 (wei) : For SCK 93K ART
                            {
                                LastSet.lSCKARTBinCT[OutArmSuck.iWhichAuto[i][j]]++;  //Steven 20170113 (wei) : For count unload count
                                if(TestIF_File.iSCKART_SortMode==1)
                                {
                                    if(Prod.bCateRTo6Tray[OutArmSuck.iWhichAuto[i][j]])
                                        LastSet.iSCKART_RTUnitCount++;
                                    else if(Prod.bART6Tray[OutArmSuck.iWhichAuto[i][j]])   //Steven 20190605 : 可能沒選CateR
                                        LastSet.iSCKART_RTUnitCount++;
                                }
                                else
                                {
                                    if(Prod.bART6Tray[OutArmSuck.iWhichAuto[i][j]])
                                        LastSet.iSCKART_RTUnitCount++;
                                }
                            }
                        }

                        fSortCT->ShowSortIC();

                        if(AUTO3_IS_MAGAZINE==1             &&                  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                           TestIF_File.iMagFixTrayType==1   &&
                           iToWhichAuto>=iMagAtAuto)
                        {
                            MOT[Motor].Tray.PordRec[iTrayCol][iTrayRow]->asBuffer->CommaText=OutArmSuck.PordRec[i][j].asBuffer->CommaText;  //把production log資料過到fix buffer區
                        }
                        else
                        {
                            if(AUTO3_IS_MAGAZINE==1 &&
                               iToWhichAuto==iMagAtAuto)                        //JerryYang 20230509 : Magazine production log
                            {
                                OutArmSuck.PordRec[i][j].AddUnloadRecord(OutArmSuck.iWhichAuto[i][j],
                                                                         LastSet.iUnloaderTrayCount_ART[OutArmSuck.iWhichAuto[i][j]],
                                                                         iTrayRow, iTrayCol,                                             //Frank 20160505 add
                                                                         iXPos, iYPos,                                                   //Frank 20160727 add iXPos iYPos
                                                                         asOCRCode,                                                      //wei 20161005 紀錄OCR Code
                                                                         iXPitchPos, iXPitch2Pos, iYPitchPos,                            //JerryYang 20210119 : add x,y pitch位置
                                                                         "");                                                            //Steven 20220120 : Amlogic需要收SBIN
                            }
                            else
                            {
                                OutArmSuck.PordRec[i][j].AddUnloadRecord(iToWhichAuto,
                                                                         LastSet.iUnloaderTrayCount_ART[iToWhichAuto],                   //Frank 20160505 add
                                                                         iTrayRow, iTrayCol,                                             //Frank 20160505 add
                                                                         iXPos, iYPos,                                                   //Frank 20160727 add iXPos iYPos
                                                                         asOCRCode,                                                      //wei 20161005 紀錄OCR Code
                                                                         iXPitchPos, iXPitch2Pos, iYPitchPos,                            //JerryYang 20210119 : add x,y pitch位置
                                                                         "");                                                            //Steven 20220120 : Amlogic需要收SBIN
                            }
                            iWhichMag=-1;                                       //JerryYang 20250920 : fixed for Multi bin

                            if(fMain->hanaART->IsHanaArtAvailable())            //Steven 20250414 : HANA ART Function
                            {
                                str.sprintf("%s,%s,%s - (%d,%d)", OutArmSuck.PordRec[i][j].asBuffer->Strings[eOrderTest],
                                                                  OutArmSuck.PordRec[i][j].asBuffer->Strings[eSiteNO],
                                                                  OutArmSuck.PordRec[i][j].asBuffer->Strings[eTestCate],
                                                                  iTrayRow+1, iTrayCol+1);

                                slHanaTrayMap[iWhichAuto]->AddText(str);
                            }

                            MOT[Motor].Tray.PordRec[iTrayCol][iTrayRow]->asBuffer->CommaText=OutArmSuck.PordRec[i][j].asBuffer->CommaText;      //Steven 20221005 : Production Log減少記憶體使用量

                            if(OutArmSuck.PordRec[i][j].asBuffer->Strings[eTestCate]=="E")  //JerryYang 20230322 : Lot summary要計算各類型ERR的數量
                            {
                                if(OutArmSuck.PordRec[i][j].asBuffer->Strings[eErrorCode].Pos("Duplicate 2DID error")>0)
                                {
                                    iE2Count++;
                                }
                                else if(OutArmSuck.PordRec[i][j].asBuffer->Strings[eErrorCode].Pos("NonTestToRBin")>0 ||
                                        OutArmSuck.PordRec[i][j].asBuffer->Strings[eErrorCode].Pos("No2DCodeDevice")>0)
                                {
                                    iE1Count++;
                                }
                                else if(OutArmSuck.PordRec[i][j].asBuffer->Strings[eErrorCode].Pos("2DID is not in sorting list")>0)
                                {
                                    iE3Count++;
                                }
                                else
                                {
                                    iENotDefinedCount++;
                                }
                            }

                            if(IniConfig.bOCRAndBinLog &&
                               TestIF.bOcrFunction &&
                               CosFunction.bTrayOCR)                            //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
                            {
                                fOCR->SaveOCRAndBinData(asOCRCode, OutArmSuck.PordRec[i][j].asBuffer->Strings[eTestCate], Prod.iIsPassT6[iToWhichAuto]);   //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                            }
                            OutArmSuck.PordRec[i][j].SaveRecord();              //Frank 20160505 add
                            if(IniConfig.bQAMode==true &&
                               LastSet.iRunStartMode==rsmQAMode &&
                               CosFunction.bQAModeUseUnloadCnt)                 //JerryYang 20221004 : Maxim版本QA mode
                            {
                                if((OutArmSuck.Item[i][j]-TEST_PASS)==1)
                                {
                                    iQAModePassCT++;
                                    if(iQAModePassCT==Prod.iQAModeCount)
                                    {
                                        MOT[Motor].SetTray(HAS_IC, __FUNC__);
                                        str.sprintf("Bin 1 count met QA Sample count:%d ea", Prod.iQAModeCount);
                                        ShowMyMessage(str);
                                    }
                                }
                            }
                        }
                        OutArmSuck.SetItemData(i, j, NULL_IC);

                        if(CosFunction.bTraySortCntFunc &&                      //Steven 20210608 : Tray指定數量退盤功能
                           iToWhichAuto<iAutoCnt &&
                           (LastSet.iRunStartMode==rsmContinuStart ||
                            LastSet.iRunStartMode==rsmContinuStart_ART))        //Steven 20251023 : fixed for sort mode
                        {
                            if(TrayForm.bTraySortCntFunc[iToWhichAuto]==true &&
                               LastSet.iTrayCntForSortFunc[iToWhichAuto]<6)
                            {
                                LastSet.iTrayICCntForSortFunc[iToWhichAuto]++;
                            }
                        }
                    }
                    else if(OutArmSuck.Suck[i][j].Error)
                    {
                        ErrPart+=OutArmSuck.Suck[i][j].sName;
                        bHasErr=true;
                    }
                    else
                    {
                        bSuckDuplicateErr[i][j]=false;
                    }
                }
            }
            if(bOutArmSuckActive[i][j])
                flag=false;
        }
    }

    if(bHasErr)
    {
        ShowErrorMessage("JAM0217", K_RETRY, MOutArmX, bHasDuplicateErr, ErrPart);  //jou 2015-03-27 JAM0203->JAM0217 Device drop error -> Vacuum sensor OFF error
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(OutArmSuck.Suck[i][j].Error)
                {
                    bSuckDuplicateErr[i][j]=true;
                    iXPos=MOT[MOutArmX].ReadPos();
                    iYPos=MOT[MOutArmY].ReadPos();
                    OutArmSuck.PordRec[i][j].AddErrorRecordNoSave("JAM0217", true, iXPos, iYPos);   //Steven 20160114 (jou) : Add Production Error Log
                }
                else
                {
                    bSuckDuplicateErr[i][j]=false;
                }
            }
        }
    }

    for(int i=0; i<OutArmSuck.iMaxRow; i++)
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
            if(bOutArmSuckActive[i][j])
                return false;

    if(IniConfig.bBinBox==true &&                                               //jou 2012-12-11 support Bin Box
       iToWhichAuto==iBinBoxAtFix &&
       AutoForm[iBinBoxAtFix]->iTrayType==iBinBoxType)
    {
        if(LastSet.iBinBoxCount>=AutoForm[iBinBoxAtFix]->iBinBoxAlarm)
        {
            MOT[Motor].SetTray(HAS_IC, __FUNC__);
        }
    }

    if(CosFunction.bTraySortCntFunc && iToWhichAuto<iAutoCnt)                   //Steven 20210608 : Tray指定數量退盤功能
    {
        if(TrayForm.bTraySortCntFunc[iToWhichAuto]==true &&
           LastSet.iTrayCntForSortFunc[iToWhichAuto]<6 &&
           (LastSet.iRunStartMode==rsmContinuStart ||
            LastSet.iRunStartMode==rsmContinuStart_ART))                        //Steven 20251023 : fixed for sort mode
        {
            int iTrayCnt=LastSet.iTrayCntForSortFunc[iToWhichAuto];
            if(LastSet.iTrayICCntForSortFunc[iToWhichAuto]>=TrayForm.iTraySortCntFunc[iToWhichAuto][iTrayCnt])  //當目前IC大於設定值,就要退盤
            {
                MOT[Motor].SetNullIcToHasNullIc();                              //退盤
                LastSet.iTrayICCntForSortFunc[iToWhichAuto]=0;
                do
                {
                    LastSet.iTrayCntForSortFunc[iToWhichAuto]++;                //下一盤
                }
                while(LastSet.iTrayCntForSortFunc[iToWhichAuto]<6 &&            //先判斷陣列內的值,要小於6
                      TrayForm.iTraySortCntFunc[iToWhichAuto][LastSet.iTrayCntForSortFunc[iToWhichAuto]]==0);
            }
        }
    }

    fSortCT->CheckTheYieldAfterPlaceAuto();                                     //Sam 20230306 : 修正 Initial Start 誤報警問題

    return true;
}
//------------------------------------------------------------------------------
bool DoOutArmPlaceToAuto_9045()
{
    int ct;
    bool flag;
    static int iRetry=0;
    int &Task=iPlaceToAutoTask;
    AnsiString asAutoOffset;

    switch(Task)
    {
        case 1:
            bOutArmXOverLimit=false;                                            //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
            InitialFix3CanFullTask();                                           //ChungHung 20140313 add Fix3 can Full Tray
            Task=10;
        case 10:
            if(CheckOutSuckICFallDown(false)==true)                             //Steven 20210916 : 全時檢查out arm掉料
            {
                Task=220;
                return false;
            }

            iWhichAuto=SearchTrayToPlace_9045();                                //Steven 20160524 : 簡化Out Arm程式碼
            IfUseOnebyOne(iWhichAuto);                                          //Steven 20111101 : 整合成function     //Steven 20210608 : 加入iWhichAuto判斷

            if(AUTO3_IS_MAGAZINE==1 &&                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
               iWhichAuto>=iMagMin  &&
               TestIF_File.iMagFixTrayType==1)
            {
                ct=iWhichBuff;
            }
            else
            {
                ct=iWhichAuto;
            }

            if(AUTO3_IS_MAGAZINE==1 && iMagazineStatus!=0)                      //JerryYang 20250920 : fixed for Multi bin
            {
                return false;
            }

            if(TRAY_ARM_MODE==eAboveCoveyor &&
               (MOT[iMMAuto[ct]].fHasTray==false ||
                (CheckUseFixBinBoxFunction()==false &&                          //kevin 20160912
                 MOT[iMMAuto[ct]].FullIC())))                                   //Steven 20091002 : 當要放IC時，編輯成滿盤會Hang Up，修正
            {
                InitOutArmTask();
                break;
            }

            if(iWhichAuto!=eBulkBox && (EnableFix3UseCylinder()))
            {
                if(UseFix3Cylinder(ct)==false)
                    break;
            }

            flag=SetOutArm_9045();
            if(flag)
            {
                if(MOT[iMMAuto[ct]].fHasTray==false ||
                   MOT[iMMAuto[ct]].FullIC())
                    Task=30;
                else
                    Task=50;
            }
            else                                                                //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
            {
                if(bOverTray==true)
                {
                    MOT[iMMAuto[ct]].SetTray(HAS_IC, __FUNC__);
                    return true;
                }
            }
            break;
        case 30:
            IsCatchTrayReadySupplyNewTray();
            if(AUTO3_IS_MAGAZINE==1 &&                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
               iWhichAuto>=iMagMin  &&
               TestIF_File.iMagFixTrayType==1)
            {
                ct=iWhichBuff;
            }
            else
            {
                ct=iWhichAuto;
            }

            if(MOT[iMMAuto[ct]].fHasTray &&
               MOT[iMMAuto[ct]].FullIC()==false)
                Task=10;
            break;
        case 50:
            if(CheckOutSuckICFallDown(false)==false)                            //Steven 20110216 Start : 整合Alarm Code
            {
                iRetry=0;
                Task=100;
            }
            else
            {
                Task=220;
                return false;
            }
        case 100:
            if(OutArmNeedCheckOffset(true, iWhichAuto))                         //Steven 20230531 : 簡化判斷式
            {
                Task=200;
                break;
            }

            if(ArmSpeed[OutArm].bSuckOnDown==false &&                           //jou 2010-12-01 start : 小IC時,會偶發性置偏
               ArmSpeed[OutArm].dDestroyPauseTime!=0)
            {
                DoPlaceToAutoDelay.SetSecAndOn(ArmSpeed[OutArm].dDestroyPauseTime);
                Task=110;
            }
            else
            {
                Task=300;
            }
            break;
        case 110:
            if(DoPlaceToAutoDelay.Off()==true)
            {
                Task=300;
            }
            break;
        case 200:
            if(bEnterOffset==false)
            {
                Task=300;
            }
            else
            {
                bEnterOffset=false;
                Task=220;
            }
            break;
        case 220:
            if(MoveOutArmToAutoSafe())
            {
                Task=1;
                if(OutArmSuck.HasIC()==false)                                   //Steven 20220812 : 避免IC掉光後卡死
                    return true;
            }
            break;
        case 300:
            if(CosFunction.bManualSteplAutoTeach &&
               IniConfig.bA56EnableAutoTeachFunciton &&
               fAutoTeach->IsRun())                                             //Jimmychiu 20211020 : Auto alignment mode
            {
                iWhichAuto=iFixRight;
            }

            if(DoOutArmPlaceToAuto(iWhichAuto)==true)                           //Steven 20110216 : 整合AlarmCode
            {
                Task=400;
            }
            break;
        case 400:
            if(USE_ROTATE_KIT==1 &&
               iRotate_Type==eInOutArm1Motor &&
               USE_PICKER_COUNT==ep1Picker)
            {
                if(MoveOutArmToAutoSafe()==false)
                    break;

//                if(INOUT_ARM_SUCK_TYPE==1)                                      //KenHsieh 20250423 : In Out Arm add 自保持型吸嘴
//                    OutArmSuck.Suck[0][0].OffDestroy();
            }

            if(CosFunction.bNewAutoTrayDetect==true &&                          //JerryYang 20170822 (Steven) 更改Auto區偵測tray盤時機 避免out arm吸嘴下降時造成誤判
               iWhichAuto<iAutoRight)
            {
                if(MOT[iMMAuto[iWhichAuto]].Tray.FullIC())                      //JerryYang 20170919 (Steven) Auto擺滿的時候會自動退tray,不要再偵測auto tray
                {
                    ReversionEmptyPoint();
                    Task=1;
                    return true;
                }
                else
                {
                    if(MoveOutArmToAutoSafe())
                    {
                        Task=500;
                    }
                }
            }
            else
            {
                ReversionEmptyPoint();
                Task=1;
                return true;
            }
            break;
        case 500:
            if(DetectAutoTray(iWhichAuto, &iRetry)==true)                       //JerryYang 20170822 (Steven) 更改Auto區偵測tray盤時機 避免out arm吸嘴下降時造成誤判
            {
                ReversionEmptyPoint();
                Task=1;
                return true;
            }
    }
    return false;
}
//------------------------------------------------------------------------------
int DoOutArmAfterPlaceToAuto(bool &bCheckSpeed, bool bInitial)                  //Steven 20220526 : 針對放下IC到Unloader後的動作做整合
{
    int iResult=0;
    int &Task=iDoOutArmAfterPlaceToAutoTask;
    if(bInitial)
    {
        Task=1;
        InitialFix3CanFullTask();
        return iResult;
    }

    switch(Task)
    {
        case 1:
            if(OutArmSuck.HasRealIC()==false)
            {
                if(IniConfig.bASE_Report)                                       //kevin 20150121 (jou) 高雄日月光IC履歷記錄
                    SendDataToASE(AseIcRecord);                                 //kevin 20161228

                BinBoxShiftY();                                                 //jou 2012-12-11 support Bin Box

                if(fAGV->IsATK_AMR() &&
                        LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                //Steven 20260202 : for ATK AMR
                {
                    for(int i=iFixMin; i<=iFixMax; i++)                         //Fix Tray
                    {
                        if(MOT[iMMAuto[i]].HasRealIC())
                        {
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }
                    }
                    LastSet.iUnloadFixTray=eAtkTfFeedFix;
                    Task=1;
                    iResult=1;
                }
                else
                {
                    if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1)      //JerryYang 20221215 : Magazine把fix區當buffer區功能
                    {
                        if((MagazineBufferIsEmpty(iPickWhichMag)==false &&
                            iWhichMag==iAuto3MagazineIndex && iAuto3MagazineIndex!=-1) ||
                           WhichMagazineBufferIsFull()!=-1)                         //要把buffer區清完
                        {
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }

                        if(iAuto3MagazineIndex!=-1 &&
                           MagazineBufferIsEmpty(iAuto3MagazineIndex)==false)   //如果buffer區還有AUTO3的Magazine bin別, 要放到AUTO3去
                        {
                            iPickWhichMag=iAuto3MagazineIndex;
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }
                    }

                    if(CosFunction.bUseFix3FullTray &&                          //Ifor 20161122 add Fix3 Full Tray 流程
                       FIX3_FULL_PLACE==Fix3K_Uninstall &&                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
                       IniConfig.bE55UseFix3FullTray==true &&                   //Ifor 20161122 add 判斷是否有開啟 Fix3 Full Tray 功能
                       MOT[iMMAuto[iSortingTrayFix3]].Tray.HasRealIC()==true)
                    {
                        SortingBinTray_Data.iMAXSUCK_OutArmSuckX=4;
                        SortingBinTray_SetMotorPosData();                           //設定馬達、及點位資料
                        SortingBinTray_SetTrayData(iSortingTrayFix3);               //設定Tray盤資料
                        bFix3HasNullICSet=true;
                        Task=1000;
                    }
                    else
                    {
                        InitialFix3CanFullTask();
                        Task=2000;
                    }
                }
            }
            else
            {
                if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1)      //JerryYang 20221215 : Magazine把fix區當buffer區功能
                {
                    Task=1;
                    iResult=3010;
                }
                else
                {
                    if(EnableFix3UseCylinder())                                 //JimmyChiu 20220927 : Stepper Motor Control in Fix3
                    {
                        iWhichAuto=SearchTrayToPlace_9045();                    //Steven 20160524 : 簡化Out Arm程式碼
                        if(iMMAuto[iWhichAuto]!=MManualTray3)                   //吸嘴還有料要繼續做, 而且不是去Fix3的, 先把氣缸縮回來
                        {
                            InitialFix3CanFullTask();                           //ChungHung 20140313 add Fix3 can Full Tray
                            Task=500;
                        }
                        else
                        {
                            Task=1;
                            iResult=3010;
                        }
                    }
                    else
                    {
                        Task=1;
                        iResult=3010;
                    }
                }
            }
            break;
        case 500:
            if(UseFix3Cylinder(0)==true)                                        //吸嘴還有料要繼續做
            {
                Task=1;
                iResult=3010;
            }
            break;
        case 1000:
            if(DoFix3FullTray())                                                //Ifor 20161122 執行Fix3 Full Tray 整盤功能
            {
                InitialFix3CanFullTask();
                Task=2000;
            }
            else
            {
                break;
            }
        case 2000:
            if(EnableFix3UseCylinder())                                         //JimmyChiu 20220927 : Stepper Motor Control in Fix3
            {
                if(UseFix3Cylinder(0)==true)                                    //吸嘴已經放完料
                {
                    Task=3000;
                }
                else
                {
                    break;
                }
            }
            else
            {
                Task=3000;
            }
//            break;
        case 3000:
            Task=CheckOutArmCleanOut(3100);
            if(ArmSpeed[InArm].bAutoSpeed)                                      //Steven 20110525 : Auto Speed
                bCheckSpeed=true;
            else
                bCheckSpeed=false;
            break;
        case 3100:
            Task=1;
            iResult=100;
            if(tRotate.ActiveRotate==1 &&
               iRotate_Type==eInOutArm1Motor)                               //add One sucker with rotate
            {
                iResult+=10;
            }
            break;
        case 5000:
            if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut==true &&    //JerryYang 20170911 (Steven) Clean Out時整盤
               CosFunction.bSortingBinTraywhenCleanOut==true &&                 //是否使用Auto Sorting AutoTray Auto Sorting AutoTray的功能
               bSortingAllBinTrayFinish==false)                                 //是否所有的Bin Tray IC都整盤完成
            {
                DoSortingBinTray(0);                                            //Bin IC排序主流程
                Task=5100;
                break;
            }

            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                if(iCleanOut==1)
                {
                    if(InArmSuck.HasIC()    ||
                       OutArmSuck.HasIC()   ||
                       ShuttleHasIC()       ||
                       IndexHasIC())
                    {
                        Task=1;
                        iResult=1;
                    }
                }
            }
            break;
        case 5100:                                                              //JerryYang 20150910 Auto Sorting BinTray by Out Arm when Clean Out
            if(DoSortingBinTray()==true)                                        //Bin IC排序主流程
            {
                if(bSortingAllBinTrayFinish==true)                              //是否所有的Bin Tray IC都整盤完成
                {
                    Task=5000;
                }
            }
            break;
    }

    return iResult;
}
//------------------------------------------------------------------------------
bool InitialOutArmNeedSuck(int iShuttle)                                        //Steven 20220927 : 重新整理In / Out Arm Need Suck
{
    int iPos1=0, iPos2=0;
    bool bLedFlag1=false, bLedFlag2=false;

    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            OutArmSuck.Suck[i][j].SetNeedSuck(false);                           //inital
        }
    }

    if(iShuttle==0)                                                             //jou 980916 start : offset shuttle right hang up
    {
        iPos1=MOT[MInShuttle1].ReadPos();
        MOT[MInShuttle1].ScanMotorStatus();
        bLedFlag1=MOT[MInShuttle1].Led[iInposLed];
        if(bLedFlag1 || (iPos1<(Prod.InSHT[0].iRight+InputLimit.iOffsetXYLow*100)))
        {
            return false;                                                       //kevin 20120612 ourarm double pick
        }
    }
    else if(iShuttle==1)
    {
        iPos2=MOT[MInShuttle2].ReadPos();
        MOT[MInShuttle2].ScanMotorStatus();
        bLedFlag2=MOT[MInShuttle2].Led[iInposLed];
        if(bLedFlag2 || (iPos2<(Prod.InSHT[1].iRight+InputLimit.iOffsetXYLow*100)))
        {
            return false;                                                       //kevin 20120612 ourarm double pick
        }
    }

    if(iShuttle==0)
        ptrOutSHT=&FRCarryKit;
    else
        ptrOutSHT=&BRCarryKit;

    return true;
}
//------------------------------------------------------------------------------
bool OutArmNeedCheckOffset(bool bPlace, int iPos)                               //Steven 20230531 : 簡化判斷式
{
    bool bresult=false;
    eOutOffset eOutOffsetPos;
    AnsiString sOutOffsetPos;
    if(bPlace)
    {
        int ct=GetOutOffsetFromWhichAuto(iPos);                                 //Jimmychiu 20240731 : for out arm offset value
        sOutOffsetPos=CapStrOutput[ct];                                         //Steven 20230907 : For HT-9011UC
        eOutOffsetPos=eOutOffset(ct);

        if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&          //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
           (Sen[SnRKManualStep].IsOn() ||
            fOffSet->UseAutoOffsetFunction(sOutOffsetPos) ||                    //jou 2013-08-29 Use Auto Offset Funtion
            fOffSet->UseOutArmSetupTeach(eOutOffsetPos)))                       //JerryYang 20180921 Setup Teach功能 //jou 2013-08-29 Use Auto Offset Funtion
            bresult=true;
    }
    else
    {
        eOutOffsetPos=(iPos==0)?OutOfsOutSh1:OutOfsOutSh2;
        sOutOffsetPos=(iPos==0)?"Output Shuttle1":"Output Shuttle2";

        if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&          //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
           (Sen[SnRKManualStep].IsOn() ||
            fOffSet->UseAutoOffsetFunction(sOutOffsetPos) ||                    //jou 2013-08-29 Use Auto Offset Funtion
            fOffSet->UseOutArmSetupTeach(eOutOffsetPos)))                       //JerryYang 20180921 Setup Teach功能
            bresult=true;
    }

    if(bresult)
    {
        bEnterOffset=false;
        fMain->Pause("OutArmNeedCheckOffset");
    }
    return bresult;
}
//==============================================================================
bool CheckOutArmToTask50(int iSht)                                              //Steven 20241019 : 整合out arm判斷式
{
    if(iSht==0)
    {
        if((iOneCycle==1 || iCleanOut==1) &&
            FTestSuck.UseSiteNoIC() &&
            FRCarryKit.UseSiteNoIC() &&
            (BRCarryKit.UseSiteHasIC() ||
             BTestSuck.UseSiteHasIC()))
        {
            return true;
        }

        if(FRCarryKit.UseSiteNoIC() &&
           OutSHT1InRT() &&
           BRCarryKit.UseSiteHasIC() &&
           OutSHT2InRT())
        {
            return true;
        }
    }
    else
    {
        if((iOneCycle==1 || iCleanOut==1) &&
            BTestSuck.UseSiteNoIC() &&
            BRCarryKit.UseSiteNoIC() &&
            (FRCarryKit.UseSiteHasIC() ||
             FTestSuck.UseSiteHasIC()))
        {
            return true;
        }

        if(BRCarryKit.UseSiteNoIC() &&
           OutSHT2InRT())
        {
            if(FRCarryKit.UseSiteHasIC() &&
               OutSHT1InRT())
            {
                return true;
            }
        }
    }

    return false;
}
//==============================================================================
int VerifyTrayStatus()
{
    static int iRetry=0;
    bool flag1=false, flag2=false, flag3=false, flag4=false;
    AnsiString str, Str2;
    int Task=3300, ret;
    if(TRAY_ARM_MODE==eAboveCoveyor &&
       MOT[iMMAuto[iWhichAuto]].fHasTray==true)
    {
        flag1=Sen[SnAutoTrayDetect[iWhichAuto]].IsOff();
        #ifndef SOFT_SIMULTE
        flag2=Sen[SnAutoFixCyPush[iWhichAuto]].IsOn();
        flag3=Cylinder[C_AutoSide_Fixer[iWhichAuto]].OffSensor();               //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
        #else
        flag2=false;
        flag3=false;
        #endif
        flag4=Sen[SnAutoEdgePush[iWhichAuto]].IsOn();

        if((flag1 || flag2 || flag3 || flag4) &&
           LastSet.iRealDummy!=DUMMY)
        {
            if(iPlaceTrayToAutoTask!=1)                                         //Steven 20241125 : fixed for place tray to auto alarm.
            {
                Task=3100;
                return Task;
            }

            Cylinder[C_AutoSide_Fixer[iWhichAuto]].Off();
            Cylinder[C_AutoEdgePush  [iWhichAuto]].Off();
            Cylinder[C_AutoUpPress   [iWhichAuto]].Off();                       //JerryYang 20190423 新增unloader壓tray

            if(flag2)
            {
                Str2.sprintf("%s must off.", Sen[SnAutoFixCyPush[iWhichAuto]].Name);
                str=sJAM1103[iWhichAuto];
            }
            else if(flag3)                                                      //Steven 20241108 : 增加顯示sensor狀態
            {
                Str2.sprintf("%s must off.", Cylinder[C_AutoSide_Fixer[iWhichAuto]].OffSensorName);
                str=sJAM1103[iWhichAuto];
            }
            else if(flag4)
            {
                Str2.sprintf("%s must off.", Sen[SnAutoEdgePush[iWhichAuto]].Name);
                str=sJAM1102[iWhichAuto];
            }
            else
            {
                Str2.sprintf("%s must on.", Sen[SnAutoTrayDetect[iWhichAuto]].Name);
                str=sWAR1130[iWhichAuto];
            }

            if(iRetry<=5)                                                       //Steven 20241122 : retry 5次
            {
                iRetry++;
//                RecordProcess(Str2, "Auto Retry");
                ret=K_RETRY;
            }
            else
            {
                ret=ShowErrorMessage(str, K_RETRY|K_SKIP, iMMAuto[iWhichAuto], bOutArmTrayDuplicateErr[iWhichAuto], Str2);
                iRetry=0;
            }
            bOutArmTrayDuplicateErr[iWhichAuto]=true;

            if(ret==K_SKIP)
            {
                fProductionInfo->CalTrayICCount(iWhichAuto);
                MOT[iMMAuto[iWhichAuto]].ClearTray(__FUNC__);
                bOutArmTrayDuplicateErr[iWhichAuto]=false;
                Task=3020;
            }
            else
            {
                Task=3050;                                                      //Ifor 20171031 add Auto Tray ReCheck
            }
            return Task;
        }
        else
        {
            bOutArmTrayDuplicateErr[iWhichAuto]=false;
        }
    }
    return Task;
}
//==============================================================================
bool Check_QA_ModeUnloadCount()                                                 //JerryYang 20221004 : Maxim版本QA mode
{
    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //Steven 20111005 : QA Mode
    {                                                                           //jou 2012-05-03 增加QA mode使用獨立的Loader Count
        if(iQAModePassCT>=Prod.iQAModeCount)                                    //數量到的話要先快速CleanOut
        {
            if(bQAModeQuickCleanOut==false && bQAModeFinishCleanOut==false)
            {
                TrayForm.bAutoFeed=false;
                bQAModeQuickCleanOut=true;
                if(fMain->BtnOneCycle->Down==false)
                {
                    InitOneCycle("QA Mode");
                    fMain->BtnOneCycle->Down=true;
                }
                return true;
            }

            if(bQAModeQuickCleanOut==true && bQAModeFinishCleanOut==false)
            {
                ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;
                bQAModeFinishCleanOut=true;
                bQAModeQuickCleanOut=false;
            }
        }
        else if(iQAModePassCT>Prod.iQAModeCount-20 &&
                iQAModePassCT<Prod.iQAModeCount)                                //數量接近時，要改用Fix方式生產
        {
            ArmSpeed[OutArm].bVariModeFIX=true;
        }
    }
    return false;
}
//==============================================================================
int GetOutArmPitchXMM_9045(int iMotPulse)
{
    double m=0.0, r=0.0;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iOutArmX120Pitch-Prod.iOutArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iOutArmX40Pitch)), (double)(m));
    r+=double(iXpitchMinX3);
    return (int)r;
}
//==============================================================================
int GetOutArmPitchYMM_9045(int iMotPulse)
{
    double m=0.0,r=0.0;
    if(USE_OUT_Y_IS_AUTO_PITCH==true)               //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        m=((double)(Prod.iOutArmY60Pitch-Prod.iOutArmY15Pitch))/(6000-IN_OUT_ARM_Y_PITCH_MIN);
        r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iOutArmY15Pitch)), (double)(m));
        r+=double(IN_OUT_ARM_Y_PITCH_MIN);
    }
    else
    {
        r=TestIF.iARM_Y_PITCH;
    }

    return (int)r;
}
//==============================================================================
void GetOutArmCellPos(int iRow, int iCol, int &Ypos, int &Xpos)
{
    int iMovePitchX=GetOutArmPitchXMM_9045(MOT[MOutArmPitch].ReadPos());
    int iMovePitchY=GetOutArmPitchYMM_9045(MOT[MOutArmPitchY].ReadPos());

    if(USE_PICKER_COUNT==ep1Picker)
        iMovePitchX=0;
    else
        iMovePitchX/=3;

    Xpos=MOT[MOutArmX].ReadPos()+(iMovePitchX*(iCol-iOutArmXBase));
    Ypos=MOT[MOutArmY].ReadPos()-(iMovePitchY*(iRow-iOutArmYBase));
}
//==============================================================================
void GetOutArmToShtCellPos(int iRow, int iCol, int &Ypos, int &Xpos)
{
    int iMovePitchX=GetOutArmPitchXMM_9045(MOT[MOutArmPitch].ReadPos());
    int iMovePitchY=GetOutArmPitchYMM_9045(MOT[MOutArmPitchY].ReadPos());

    if(USE_PICKER_COUNT==ep1Picker)
        iMovePitchX=0;
    else
        iMovePitchX/=3;

    Xpos=MOT[MOutArmX].ReadPos()+(iMovePitchX*(iCol*OutArmSuck.iPickStep-iOutArmXBase));
    Ypos=MOT[MOutArmY].ReadPos()-(iMovePitchY*(iRow-iOutArmYBase));
}

int iOutPickFromSht=0;
int iOutPlaceToAuto=1;
//==============================================================================
void InspectOutArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, int iAction)
{
    if(IniConfig.bE74_InspectArmPosition==false)                                //Jimmychiu 20240408 : debug for inarm position
        return;

    bool bX, bY;
    int iOutPutTray;
    AnsiString sError, sPos="";
    AnsiString sPlace;

    if(iAction==iOutPlaceToAuto)
    {
        sPlace="Place to";
        sPos=s6TrayName[iTarget];

        if(AUTO3_IS_MAGAZINE==1             &&                                  //JerryYang 20221215 : Magazine把fix區當buffer區功能
           TestIF_File.iMagFixTrayType==1   &&
           iTarget>=iMagMin)
        {
            iOutPutTray=iWhichBuff;
        }
        else
        {
            iOutPutTray=iTarget;
        }
    }
    else
    {
        return;                                                                 //Steven 20241220 : 暫時跳過
        sPlace="Pick from";
        if(iTarget==MOutShuttle1)
        {
            sPos="Shuttle 1";
        }
        else if(iTarget==MOutShuttle2)
        {
            sPos="Shuttle 2";
        }
        else if(iTarget==MOutRotateKit)
        {
            sPos="Out Rotator";
        }
    }

    int XEncoder=0, YEncoder=0, HardwarePosX=0, HardwarePosY=0;

    if(OutArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_IC &&
       OutArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_CLEAN_IC)
    {
        if(iTarget==MOutShuttle1 ||
           iTarget==MOutShuttle2)
            GetOutArmToShtCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);
        else
            GetOutArmCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);

        if(iAction==iOutPlaceToAuto)
        {
            if(iTarget==eBulkBox)
            {
                HardwarePosX=Prod.iOutArmBinBoxX+dOutArmXPitch_1Step*iTargetCol;
                HardwarePosY=Prod.iOutArmBinBoxY-Prod.iOutArmBinBoxY*200;       //待確認
            }
            else                                                                //Jimmychiu 20240731 : remove offset value
            {
                HardwarePosX=Prod.XStart[iOutPutTray][iOutArmYBase][iOutArmXBase]+AutoForm[iOutPutTray]->XPitch*iTargetCol; //Jimmy 20240826 : fixed for E74
                HardwarePosY=Prod.YStart[iOutPutTray][iOutArmYBase][iOutArmXBase]-AutoForm[iOutPutTray]->YPitch*iTargetRow;

                if(USE_PICKER_COUNT==ep1Picker)
                    HardwarePosX=HardwarePosX;
                else
                    HardwarePosX+=((OutArmOffSet[GetOutOffsetFromWhichAuto(iOutPutTray)]->GetVariable())/3)*(iSuckCol-iOutArmXBase);

                HardwarePosY-=OutArmOffSet[GetOutOffsetFromWhichAuto(iOutPutTray)]->GetVariableY()*(iSuckRow-iOutArmYBase);
            }
        }
        else
        {
            if(iTarget==MOutShuttle1 ||
               iTarget==MOutShuttle2)
            {
                GetShtRowColStartPos(iTarget,HardwarePosX,HardwarePosY);
                HardwarePosX+=TestIF.dSiteXPitch*iTargetCol;
                HardwarePosY-=TestIF.dSiteYPitch*iTargetRow;
                int iXoffset=0, iYoffset=0;
                if(iTarget==MOutShuttle1)
                {
                    iXoffset=OutArmOffSet[OutOfsOutSh1]->GetVariable();
                    iYoffset=OutArmOffSet[OutOfsOutSh1]->GetVariableY();
                }
                else //MOutShuttle2
                {
                    iXoffset=OutArmOffSet[OutOfsOutSh2]->GetVariable();
                    iYoffset=OutArmOffSet[OutOfsOutSh2]->GetVariableY();
                }

                if(USE_PICKER_COUNT==ep1Picker)
                    HardwarePosX=HardwarePosX;
                else
                    HardwarePosX+=(iXoffset/3)*(iSuckCol-iOutArmXBase);

                HardwarePosY-=iYoffset*(iSuckRow-iOutArmYBase);
                if(TestIF.bNS7000kit)                                               //jou 981208 start : NS7000 bias kit
                {
                    int iShiftY=(TestIF.dSiteYPitch==0)?(6000/2):(TestIF.dSiteYPitch/2);
                    if(iTarget==MOutShuttle1)
                        HardwarePosY+=iShiftY;
                    else
                        HardwarePosY-=iShiftY;
                }
            }
            else if(iTarget==MOutRotateKit)                                     //待確認
            {
                HardwarePosX=Prod.iOutArmRotateToUnloaderX;
                HardwarePosY=Prod.iOutArmRotateToUnloaderY;
            }
        }

        int iLimit=100*InputLimit.iOffsetXYHigh;                                //Jimmychiu 20240731 : remove offset value
        bX=MOT[MOutArmX].CheckArmPosArrival(HardwarePosX, XEncoder, iLimit);    //Steven 20240719 : 放寬檢查的範圍
        bY=MOT[MOutArmY].CheckArmPosArrival(HardwarePosY, YEncoder, iLimit);
        if(bX==false || bY==false)
        {

            sError.sprintf("OutArm Suck[%d, %d] Pos(Y=%d, X=%d), \r%s %s [%d, %d] Pos(Y=%d, X=%d)",
                           iSuckRow, iSuckCol, YEncoder, XEncoder, sPlace, sPos, iTargetRow, iTargetCol, HardwarePosY, HardwarePosX);

            ShowMyMessage(sError);
        }
    }
}
//------------------------------------------------------------------------------
int GetOutOffsetFromWhichAuto(int iWhichAuto)                                   //Jimmychiu 20240731 : for out arm offset value
{
    if(iWhichAuto>=eTrayCount)
    {
        ShowMyMessage(AnsiString().sprintf("Error Func:%s iWhichAuto:%s",__FUNC__,iWhichAuto));
        return 0;
    }
    if(AUTO_EMPTY_COLOR>=3)
    {
        int iWhichAutoToOutOffset[eTrayCount]={OutOfsAuto1,                 //eAuto1
                                               OutOfsAuto2,                 //eAuto2
                                               OutOfsAuto3,                 //eAuto3
                                               OutOfsAuto4,                 //eAuto4
                                               OutOfsAuto5,                 //eAuto5
                                               OutOfsAuto6,                 //eAuto6
                                               OutOfsFix1,                  //eFix1
                                               OutOfsFix2,                  //eFix2
                                               OutOfsFix3,                  //eFix3
                                               OutOfsFix4,                  //eFix4
                                               OutOfsFix5,                  //eFix5
                                               OutOfsFix6,                  //eFix6
                                               OutOfsFix1,                  //eFix7
                                               OutOfsFix2,                  //eFix8
                                               OutOfsFix3,                  //eFix9
                                               OutOfsFix4,                  //eFix10
                                               OutOfsFix5,                  //eFix11
                                               OutOfsFix6,                  //eFix12
                                               OutOfsFix6,                  //eBulkBox
                                               OutOfsAuto6,                 //eMag1
                                               OutOfsAuto6,                 //eMag2
                                               OutOfsAuto6,                 //eMag3
                                               OutOfsAuto6,                 //eMag4
                                               OutOfsAuto6,                 //eMag5
                                               OutOfsAuto6,                 //eMag6
                                               OutOfsAuto6,                 //eMag7
                                               OutOfsAuto6,                 //eMag8
                                               OutOfsAuto6,                 //eMag9
                                               OutOfsAuto6,                 //eMag10
                                               OutOfsAuto6,                 //eMag11
                                               OutOfsAuto6,                 //eMag12
                                               OutOfsAuto6,                 //eMag13
                                               OutOfsAuto6,                 //eMag14
                                              };
        return iWhichAutoToOutOffset[iWhichAuto];
    }
    else
    {
        int iWhichAutoToOutOffset[eTrayCount]={OutOfsAuto1,                 //eAuto1
                                               OutOfsAuto2,                 //eAuto2
                                               OutOfsAuto3,                 //eAuto3
                                               OutOfsAuto4,                 //eAuto4
                                               OutOfsAuto5,                 //eAuto5
                                               OutOfsAuto6,                 //eAuto6
                                               OutOfsFix1,                  //eFix1
                                               OutOfsFix2,                  //eFix2
                                               OutOfsFix3,                  //eFix3
                                               OutOfsFix1,                  //eFix4
                                               OutOfsFix2,                  //eFix5
                                               OutOfsFix3,                  //eFix6
                                               OutOfsFix1,                  //eFix7
                                               OutOfsFix2,                  //eFix8
                                               OutOfsFix3,                  //eFix9
                                               OutOfsFix4,                  //eFix10
                                               OutOfsFix5,                  //eFix11
                                               OutOfsFix6,                  //eFix12
                                               OutOfsFix3,                  //eBulkBox
                                               OutOfsAuto3,                 //eMag1
                                               OutOfsAuto3,                 //eMag2
                                               OutOfsAuto3,                 //eMag3
                                               OutOfsAuto3,                 //eMag4
                                               OutOfsAuto3,                 //eMag5
                                               OutOfsAuto3,                 //eMag6
                                               OutOfsAuto3,                 //eMag7
                                               OutOfsAuto3,                 //eMag8
                                               OutOfsAuto3,                 //eMag9
                                               OutOfsAuto3,                 //eMag10
                                               OutOfsAuto3,                 //eMag11
                                               OutOfsAuto3,                 //eMag12
                                               OutOfsAuto3,                 //eMag13
                                               OutOfsAuto3,                 //eMag14
                                              };
        return iWhichAutoToOutOffset[iWhichAuto];
    }
}
//==============================================================================
MyFillTheTrayAfterOutArmPlace mFillTray;
bool IsEnableFillTheTrayAfterOutArmPlace()                                      //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(CosFunction.bFillTheTrayAfterOutArmPlace==true &&
       IniConfig.bE85_FillTray_Enable==true)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool IsRunWhichAtuoFillTray(int iWhichAuto)                                     //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(IsEnableFillTheTrayAfterOutArmPlace())
    {
        if(iWhichAuto==iSortingTrayAuto3 &&
           IniConfig.bE85_FillTray_Auto3==true)
        {
            return true;
        }
        else if(iWhichAuto==iSortingTrayAuto1 &&
                IniConfig.bE85_FillTray_Auto1==true)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool RecTrayICStatus(TTrayMotor *myMotor, int irow, int icol, bool &bHasMidEmpty, uPoint2D &pTargetPos, uPoint2D &pSourcePos) //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(bHasMidEmpty==false && myMotor->Tray.Data[icol][irow]==HAS_NULL_IC)
    {
        pTargetPos.X=icol;
        pTargetPos.Y=irow;
        bHasMidEmpty=true;
    }
    else if(bHasMidEmpty==true && myMotor->Tray.Data[icol][irow]==HAS_IC)
    {
        pSourcePos.X=icol;
        pSourcePos.Y=irow;
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool HasGapsInTheTray(int iWhichAuto, uPoint2D &pTargetPos, uPoint2D &pSourcePos)   //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bHasMidEmpty=false;
    TTrayMotor *myMotor=&MOT[iMMAuto[iWhichAuto]];
    if(myMotor->fHasTray==false || myMotor->HasIC()==false)
        return false;
    int Direction=0, ixStart=0, ixEnd=0, ixStep=0, iyStart=0, iyEnd=0, iyStep=0;
    GetTrayDirection(iWhichAuto, Direction, ixStart, ixEnd, ixStep, iyStart, iyEnd, iyStep);
    Direction=AutoForm[iWhichAuto]->Direction;
    if(Direction<4)
    {
        for(int irow=iyStart; irow!=iyEnd; irow+=iyStep)
        {
            for(int icol=ixStart; icol!=ixEnd; icol+=ixStep)
            {
                if(RecTrayICStatus(myMotor, irow, icol, bHasMidEmpty, pTargetPos, pSourcePos))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        for(int icol=ixStart; icol!=ixEnd; icol+=ixStep)
        {
            for(int irow=iyStart; irow!=iyEnd; irow+=iyStep)
            {
                if(RecTrayICStatus(myMotor, irow, icol, bHasMidEmpty, pTargetPos, pSourcePos))
                {
                    return true;
                }
            }
        }
    }
    pTargetPos.Clear();
    pSourcePos.Clear();
    return false;
}
//------------------------------------------------------------------------------
bool DetermineFeasibilityOfSuck(SingleSuckPosOnTray &ssp, uPoint2D pArmSuckActive)  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    ssp.iArmXV1=iXpitchMaxX3;
    ssp.iArmXV2=iXpitchMaxX3;
    ssp.iArmYV1=TestIF.iARM_Y_PITCH;
    uPoint2D pStartPos;
    GetWhichAutoStart(ssp.iWhichAuto,pStartPos);
    //to tray cell(0, 0)
    ssp.pArmXYPos.X=pStartPos.X;
    ssp.pArmXYPos.Y=pStartPos.Y;
    //to tray cell(iy, ix)
    ssp.pArmXYPos.X+=(ssp.pTrayRowCol.X*AutoForm[ssp.iWhichAuto]->XPitch);
    ssp.pArmXYPos.Y-=(ssp.pTrayRowCol.Y*AutoForm[ssp.iWhichAuto]->YPitch);
    //suck xy to cell(iy, ix)
    ssp.pArmXYPos.X-=(pArmSuckActive.X-iOutArmXBase)*iXpitchMax;
    ssp.pArmXYPos.Y+=(pArmSuckActive.Y-iOutArmYBase)*ssp.iArmYV1;
    if(IsOutArmPosOutLimit(ssp.pArmXYPos)==true)
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
void GetWhichAutoStart(int iWhichAuto, uPoint2D &pStartPos)                     //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(iWhichAuto<3)
    {
        pStartPos.X=Prod.XOutArm_Auto_Place[iWhichAuto][iOutArmYBase][iOutArmXBase];
        pStartPos.Y=Prod.YOutArm_Auto_Place[iWhichAuto][iOutArmYBase][iOutArmXBase];
    }
    else
    {
        iWhichAuto%=3;
        pStartPos.X=Prod.XOutArm_Fix_Place[iWhichAuto][iOutArmYBase][iOutArmXBase];
        pStartPos.Y=Prod.YOutArm_Fix_Place[iWhichAuto][iOutArmYBase][iOutArmXBase];
    }
}
//------------------------------------------------------------------------------
int GetWhichAutoPickZ(int iWhichAuto, int irow, int icol)                       //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(iWhichAuto<3)
    {
        return Prod.ZOutArm_Auto_Place[iWhichAuto][irow][icol]-OutArmOffSet[OutOfsAuto1+iWhichAuto]->GetPlace()+OutArmOffSet[OutOfsAuto1+iWhichAuto]->GetPickUp();
    }
    else
    {
        iWhichAuto%=3;
        return Prod.ZOutArm_Fix_Place[iWhichAuto][irow][icol]-OutArmOffSet[OutOfsAuto1+iWhichAuto]->GetPlace()+OutArmOffSet[OutOfsFix1+iWhichAuto]->GetPickUp();
    }
}
//------------------------------------------------------------------------------
int GetWhichAutoPlaceZ(int iWhichAuto, int irow, int icol)                      //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(iWhichAuto<3)
    {
        return Prod.ZOutArm_Auto_Place[iWhichAuto][irow][icol];
    }
    else
    {
        iWhichAuto%=3;
        return Prod.ZOutArm_Fix_Place[iWhichAuto][irow][icol];
    }
}
//------------------------------------------------------------------------------
bool IsOutArmPosOutLimit(uPoint2D pArmPos)                                      //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(pArmPos.X<(MOT[MOutArmX].Motor->PSoftLimitN+10) || pArmPos.X>(MOT[MOutArmX].Motor->PSoftLimitP-100))
    {
        return true;
    }
    if(pArmPos.Y<(MOT[MOutArmY].Motor->PSoftLimitN+10) || pArmPos.Y>(MOT[MOutArmY].Motor->PSoftLimitP-100))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool FindArmSuck(int iWhichAuto, MyFillTheTrayAfterOutArmPlace &mfillT)         //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    mfillT.ssTarget.iWhichAuto=iWhichAuto;
    mfillT.ssTarget.bPlace=true;
    mfillT.ssSource.iWhichAuto=iWhichAuto;
    mfillT.ssSource.bPlace=false;
    for(int irow=0; irow<MAX_ARM_Row; irow++)
    {
        for(int icol=0; icol<MAX_ARM_Col; icol++)
        {
            mfillT.pArmSuckActive.X=icol;
            mfillT.pArmSuckActive.Y=irow;
            if(DetermineFeasibilityOfSuck(mfillT.ssTarget, mfillT.pArmSuckActive)==true &&
               DetermineFeasibilityOfSuck(mfillT.ssSource, mfillT.pArmSuckActive)==true)
            {
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool MoveOutArmXYAndSuck(SingleSuckPosOnTray ssTarget, uPoint2D pActiveSuck)    //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bDoSuck=IsOutArmArrival(ssTarget.pArmXYPos);
    if(bDoSuck==false && MoveOutArmToAutoSafe()==false)
    {
        return false;
    }
    bool ZDownFlag[MAX_ARM_Row][MAX_ARM_Col];
    int ZDownPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    ZeroMemory(iXVariable,  sizeof(iXVariable));
    ZeroMemory(ZDownFlag,   sizeof(ZDownFlag));
    ZeroMemory(ZDownPos,    sizeof(ZDownPos));
    iXVariable[0]=GetOutArmPitch_9045(ssTarget.iArmXV1);
    iXVariable[1]=GetOutArmPitch2_9045(ssTarget.iArmXV2);
    iYVariable   =GetOutArmPitchY_9045(ssTarget.iArmYV1);
    int irow     =pActiveSuck.Y, icol=pActiveSuck.X;
    ZDownFlag[irow][icol]=true;
    if(ssTarget.bPlace)
    {
        ZDownPos[irow][icol]=GetWhichAutoPlaceZ(ssTarget.iWhichAuto, irow, icol);
    }
    else
    {
        ZDownPos[irow][icol]=GetWhichAutoPickZ(ssTarget.iWhichAuto, irow, icol);
    }
    bool bZDown=true;
    if(OutArmContinuousMove_9045(ssTarget.pArmXYPos.X, ssTarget.pArmXYPos.Y, iXVariable, iYVariable, ZDownFlag, ZDownPos, bZDown))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MoveOutArmXYAndSuck(ArmAndSuckInfo asInfo)                                 //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(IsOutArmArrival(asInfo.pArmXYPos)==false && MoveOutArmToAutoSafe()==false)
    {
        return false;
    }
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    ZeroMemory(iXVariable,  sizeof(iXVariable));
    iXVariable[0]=GetOutArmPitch_9045(asInfo.iXVariable[0]);
    iXVariable[1]=GetOutArmPitch2_9045(asInfo.iXVariable[1]);
    iYVariable   =GetOutArmPitchY_9045(asInfo.iYVariable);
    bool bZDown=true;
    if(OutArmContinuousMove_9045(asInfo.pArmXYPos.X, asInfo.pArmXYPos.Y, iXVariable, iYVariable, asInfo.bZActive, asInfo.iZPosition, bZDown))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool IsOutArmArrival(uPoint2D pTargetPos)                                       //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bX=TMyMotor().CheckArmPosArrival(pTargetPos.X, MOT[MOutArmX].ReadPos(), 10);
    bool bY=TMyMotor().CheckArmPosArrival(pTargetPos.Y, MOT[MOutArmY].ReadPos(), 10);
    return (bX && bY);
}
//------------------------------------------------------------------------------
int iOutArmFillTheTrayAfterPlaceAllICTask;
bool DoOutArmFillTheTrayAfterPlaceAllIC(bool bIsFirst, int iWhichAuto)          //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bResult=false;
    int &Task=iOutArmFillTheTrayAfterPlaceAllICTask;
    if(bIsFirst)
    {
        Task=1;
        mFillTray.Clear();
        return true;
    }
    int ret=0;
    switch(Task)
    {
        case 1:
            if(HasGapsInTheTray(iWhichAuto, mFillTray.ssTarget.pTrayRowCol, mFillTray.ssSource.pTrayRowCol)==true &&
               FindArmSuck(iWhichAuto, mFillTray)==true)
            {
                Task=100;
            }
            else
            {
                return true;
            }
            break;
        case 100:                                                               //move to source tray
            if(MoveOutArmXYAndSuck(mFillTray.ssSource, mFillTray.pArmSuckActive))
            {
                if(OutArmNeedCheckOffset(false, 0))                             //Steven 20230531 : 簡化判斷式
                {
                    bEnterOffset=false;
                    fMain->Pause("DoOutArmFillTheTrayAfterPlaceAllIC");
                    Task=100;
                    break;
                }
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                Task=200;
            }
            break;
        case 200:
            if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Suck())
            {
                #ifdef SOFT_SIMULTE                                             //模擬ic不見了
                if(fMain->chkInPickLoadError->Checked==true)
                {
                    OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error=true;
                }
                #endif                                                          //模擬ic不見了 end
                MOT[iMMAuto[iWhichAuto]].SetTrayBinData(mFillTray.ssSource.pTrayRowCol.X, mFillTray.ssSource.pTrayRowCol.Y, NULL_IC, "");
                OutArmSuck.SetItemData(mFillTray.pArmSuckActive.Y, mFillTray.pArmSuckActive.X, HAS_IC);
                InspectOutArmPosition(iWhichAuto, mFillTray.pArmSuckActive.Y, mFillTray.pArmSuckActive.X, mFillTray.ssSource.pTrayRowCol.Y, mFillTray.ssSource.pTrayRowCol.X, iOutPlaceToAuto);
                Task=300;
            }
            else
            {
                if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error==false)
                    break;
            }

            if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error)
            {
                AnsiString ErrPart=OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].sName;
                ret=OutArmPickTrayAlarm(iWhichAuto, ErrPart);
                if(ret==K_RETRY)
                {
                    Task=100;
                }
                else                                                            //K_SKIP
                {
                    MOT[iMMAuto[iWhichAuto]].SetTrayBinData(mFillTray.ssSource.pTrayRowCol.X, mFillTray.ssSource.pTrayRowCol.Y, NULL_IC, "");
                    Task=500;
                }
            }
            break;
        case 300:                                                               //move to target tray
            if(MoveOutArmXYAndSuck(mFillTray.ssTarget,mFillTray.pArmSuckActive))
            {
                if(OutArmNeedCheckOffset(false, 0))                             //Steven 20230531 : 簡化判斷式
                {
                    bEnterOffset=false;
                    fMain->Pause("DoOutArmFillTheTrayAfterPlaceAllIC");
                    Task=300;
                    break;
                }
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空
                Task=400;
            }
            break;
        case 400:
            if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Destroy())
            {
                #ifdef SOFT_SIMULTE                                             //模擬ic不見了
                if(fMain->chkInPickLoadError->Checked==true)
                {
                    OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error=true;
                }
                #endif                                                          //模擬ic不見了 end
                MOT[iMMAuto[iWhichAuto]].SetTrayBinData(mFillTray.ssTarget.pTrayRowCol.X, mFillTray.ssTarget.pTrayRowCol.Y, HAS_IC , "");
                OutArmSuck.SetItemData(mFillTray.pArmSuckActive.Y, mFillTray.pArmSuckActive.X, NULL_IC);
                Task=500;
            }
            else
            {
                if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error==false)
                    break;
            }

            if(OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].Error)
            {
                AnsiString ErrPart=OutArmSuck.Suck[mFillTray.pArmSuckActive.Y][mFillTray.pArmSuckActive.X].sName;
                ret=ShowErrorMessage("JAM0217", K_RETRY|K_SKIP, MOutArmX, false, ErrPart);
                if(ret==K_RETRY)
                {
                    Task=300;
                }
                else                                                            //K_SKIP
                {
                    OutArmSuck.SetItemData(mFillTray.pArmSuckActive.Y, mFillTray.pArmSuckActive.X, NULL_IC);
                    Task=500;
                }
            }
            break;
        case 500:
            if(MoveOutArmToAutoSafe()==true)
            {
                Task=1;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool InitNewFixTrayForUnloaderClipRead(int iwhichauto,bool bOutputReport)                          //Jimmychiu 20250818 : 海康智能讀碼
{
    bool bret=false;
    AnsiString sErr="";
    AnsiString sGet="";
    AnsiString sAreaName="";
    //
    int iiccdUnloaderID=0;
    if(iwhichauto==(int)eAuto1)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipAuto1);
        sAreaName="Auto1";
    }
    else if(iwhichauto==(int)eAuto2)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipAuto2);
        sAreaName="Auto2";
    }
    else if(iwhichauto==(int)eAuto3)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipAuto3);
        sAreaName="Auto3";
    }
    else if(iwhichauto==(int)eFix1)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipFix1);
        sAreaName="Fix1";
    }
    else if(iwhichauto==(int)eFix2)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipFix2);
        sAreaName="Fix2";
    }
    else if(iwhichauto==(int)eFix3)
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipFix3);
        sAreaName="Fix3";
    }
    else
    {
        iiccdUnloaderID=(int)(fBarCode->eUnloaderClipAuto1);
        sAreaName="Unknown Area";
    }
    //
    if(fBarCode->ccdUnloader[iiccdUnloaderID].bGetResult==false)
    {
        sErr="";
        sGet=fBarCode->ccdUnloader[iiccdUnloaderID].DoGetPhotoCmd(false,sErr);
        if(sGet.IsEmpty()==false)
        {
            if(bOutputReport==true)
            {
                //out put report
            }
            InitNewFixTray(iwhichauto, "AOutArm_FullIC");        //Steven 20160414 : 整合Fix盤設定
            MOT[iMMAuto[iwhichauto]].Tray.cCassetteID=sGet;
            bret=true;
        }
        else if(sErr.IsEmpty()==false)
        {
            ShowMyMessage(AnsiString().sprintf("%s CCD Clip ID:%s",sAreaName,sGet));
            fBarCode->ccdUnloader[iiccdUnloaderID].DoGetPhotoCmd(true,sErr);
        }
    }
    else
    {
        bret=true;
    }
    return bret;
}
//------------------------------------------------------------------------------