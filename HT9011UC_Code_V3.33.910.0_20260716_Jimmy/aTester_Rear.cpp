#include "MachineDefine.h"
#pragma hdrstop

#include "aTester_Rear.h"

#include "atester.h"
#include "atester_32Site.h"
#include "atester_ProcessCount.h"
#include "aTester_Front.h"
#include "mycylin.h"
//---------------------------------------------------------------------------

#include "MachineType.h"
#include "MyMotor.h"
#include "mysensor.h"
#include "myswitch.h"
#include "csetup.h"
#include "rs232.h"
#include "cTestCategory.h"
#include "cObserver.h"
#include "cSocket.h"
#include "cContactCT.h"
#include "cinitial.h"
#include "csystem.h"
#include "main.h"
#include "note.h"
#include "mymessbox.h"
#include "AutoClean.h"
#include "cContact.h"
#include "cAuthority.h"
#include "cprod.h"
#include "adam6024.h"
#include "cMyDB.h"
#include "iosetview.h"
#include "automation.h"
#include "uShowMessage.h"
#include "common.h"
#include "CCDInterface.h"
#include "uLotInfo.h"
#include "ATCInterface.h"
#include "cpublic.h"
//#include "SECSGEM.h"
#include "Socket_ASE_KR.h"
//#include "ScanBtnThread.h"
#include "InterfaceSYS.h"
#include "BarCode.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "ckernel.h"
#include "MessageDef.h"
#include "SCK_ART.h"
#include "ainarm2.h"
#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "ATC_Handler_Side.h"                                                   //Ifor 20160509 :add New ATC Interface
#include "uHeaterThread.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20160509 :add New ATC Interface
#include "uHeaterThread.h"
#include "AMR.h"
#pragma package(smart_init)

extern int iTesterBIN[MAX_SOCKET_ROW][MAX_SOCKET_COL];
extern bool bEcho, bExist, bUnderTest;
extern bool bEchoStop;                                                          //ChungHung 20130326 add
extern bool bGPIBError;
static bool bIndexZ2NeedUp=false;                                               //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
extern bool bPlaceToShuttle2Step;
extern bool CheckCFixTrayFullPlace();
extern void CheckShuttle2EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log
extern unsigned int iBin[4][8];
extern ScanBtn *ScanBtnThd;

int iBTestSocketClampCloseTask;
int iBTestSocketClampOpenTask;
int iHangupCTArm2=0;

bool bReadTorqueOK=false;                                                       //kevin 20211129 read torque ok
bool bArm2SuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},         //Steven 20110301 : 確認吸取完成
                                                      {false, false, false, false, false, false, false, false},
                                                      {false, false, false, false, false, false, false, false},
                                                      {false, false, false, false, false, false, false, false}};

//bool bArm2DuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
//                                                        {false, false, false, false, false, false, false, false},
//                                                        {false, false, false, false, false, false, false, false},
//                                                        {false, false, false, false, false, false, false, false}};

bool bArm2PutDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},    //Steven 20100105 Start: 排除重複的Alarm Code
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false}};

TQPF_Timer hBTestTimeOutDelay;                                                  //Jou 20101018
TQPF_Timer BTestSocketClampCloseDelay;                                          //JerryYang 20160524
TQPF_Timer BTestSocketClampOpenDelay;                                           //JerryYang 20160524
TQPF_Timer BTestSocketClampTimeOutDelay;                                        //JerryYang 20160524
TQPF_Timer BTorqueTimeOutDelay;                                                 //kevin 2021018 扭力
//------------------------------------------------------------------------------
void ResetInxedArm2Flag()                                                       //Steven 20241017 : 打包成function
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bArm2SuckFinish[i][j]=false;                                        //Steven 20110301 : 初始化，都當作還沒做完
            bArm2DuplicateErr[i][j]=false;
            BTestSuck.Suck[i][j].Reset();                                       //Steven 20140213 : Jordan說Index下去不吸直接Alarm
        }
    }

    BLCarryKit.SetUnuseToNullIC();                                              //Steven 20241017 : 清除沒用到的資料
}
//------------------------------------------------------------------------------
void ResetArm2VacFlag()                                                         //Steven 20241017 : 打包成function
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bSkipNeedCheckVac[1][i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
void ResetArm2PutFlag()                                                         //Steven 20241017 : 打包成function
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bArm2PutDuplicateErr[i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
void CheckBTFullSite()                                                          //wei 20151228 No FullSite delay
{
    int iCount=0, iUseSiteCount=0;

    iArmsitecount[1]=0;
    for(int i=0; i<BTestSuck.iShtRow; i++)
    {
        for(int j=0; j<BTestSuck.iShtCol; j++)
        {
            if(BTestSuck.Item[i][j]==HAS_IC ||
               BTestSuck.Item[i][j]==HAS_HOT_IC)
            {
                iCount++;
            }
            iUseSiteCount++;
        }
    }

    if(iCount!=0)
    {
        iArmsitecount[1]=iUseSiteCount-iCount;
        if(iCount!=iUseSiteCount)
        {
            iInitContactCount=0;                                                //Steven 20160519 : 起測時溫度要補Offset
            bDoWhenNoFullSiteUseInitialDelay=true;
        }
        else
        {
            bDoWhenNoFullSiteUseInitialDelay=false;
        }
    }
}
//------------------------------------------------------------------------------
bool CheckZ2IsDown()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        long lPos[4]={0, 0, 0, 0};                                              //kevin 20150915

        if(IniConfig.bD52InterFaceErrHeadNeedUp && bNowDoInterFaceErrorStep)
            return false;

        if(bContactTimeOverStep)
            return false;

        if(CosFunction.bSortingBy2DList==true &&
           LastSet.iTester==_2D_SORT &&
           TestIF_File.bSortingBy2DIDList==true)                                //Frank 20221122 : 2DID sorting for ATK
        {
            return true;
        }

        if(MOT[MTestZ2].Motor->Enable==false)
            return true;

        if(TestIF_File.iShuttleMode==1 &&                                       //20111114  Dell for Disable Index Arm    Start
           TestIF_File.iShuttle_Sel==0)                                         //jou 2014-11-13 remake code , close arm index postion error
        {
            return true;
        }

        if(bUseTwoArm32Site==true)
        {                                                                       //nn Mode 是同時有吸取IC動作
            return true;
        }
        else
        {
            //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
            if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
            {
                return true;
            }
            else if(MOT[MTestZ2].Gali_ReadEncoderPos()>Prod.iTestDown_Z2)
            //<== Eastsun 20260511 F007 整合
            {
                lPos[2]=MOT[MTestZ2].Gali_ReadEncoderPos();                     //kevin 20150915
                lPos[3]=Prod.iTestDown_Z2;                                      //kevin 20150915
                RecordIndexPositionError("CheckZ2IsDown()", false, false, true, true, &lPos[0]);                        //kevin 20150915 record
                ShowIndexMotorError(AnsiString("CheckZ2IsDown"));
                return false;
            }
        }
        return true;
    #endif
}
//==============================================================================
bool TestZ2OutRandge()
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        int Pos1=MOT[MTestZ2].Gali_ReadPos();
        int Pos2=MOT[MTestZ2].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        Pos1=MOT[MTestY1].Gali_ReadPos();
        Pos2=MOT[MTestY1].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        Pos1=MOT[MTestY2].Gali_ReadPos();
        Pos2=MOT[MTestY2].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        return false;
    #endif
}
//==============================================================================
bool TestZ2OutRandge2()
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        int Pos1=MOT[MTestZ2].Gali_ReadPos();
        int Pos2=MOT[MTestZ2].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;

        Pos1=MOT[MTestY1].Gali_ReadPos();
        Pos2=MOT[MTestY1].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        Pos1=MOT[MTestY2].Gali_ReadPos();
        Pos2=MOT[MTestY2].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        return false;
    #endif
}
//==============================================================================
void TestZ2SetPos()
{
    int Pos3=ChangeToFloatNonPcnt((double)(MOT[MTestZ2].Gali_ReadEncoderPos()), (double)(MOT[MTestZ2].Motor->GearRatio));                                       //Steven 20100728 Start: 要加上齒輪比
    int Pos4=ChangeToFloatNonPcnt((double)(MOT[MTestY2].Gali_ReadEncoderPos()), (double)(MOT[MTestY2].Motor->GearRatio));
    char str[256];
    sprintf(str, "DP,,%d,%d", 0-Pos3, 0-Pos4);
    MOT[MTestY2].Gali_Command(str, __FUNC__);
}
//---------------------------------------------------------------------------
int iRearTestDestroyICTask=1;
void InitRearTestDestroyICTask()
{
    iRearTestDestroyICTask=1;
    bZ2Isdownflag=false;
}
//------------------------------------------------------------------------------
TQPF_Timer CheckSocketHasICDelay2;                                              //kevin 20190819 add
TQPF_Timer BTempRiseHasICDelay;
bool DoRearTestDestroyIC(bool bCheckZ1)
{
    static int iSiteCount=0;
    static int iRetry=0;                                                        //kevin 20190709  index check one by one

    int &Task=iRearTestDestroyICTask;
    int sp=0, iPos=0;
    bool flag=false, flag2=false;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    AnsiString sTempMessge="";
    AnsiString ErrPart="";
    static bool bHasDropErr=false;

    switch(Task)
    {
        case 1:
            iSiteCount=0;                                                       //kevin 20190819  index check one by one
            bHasDropErr=false;
            iIndexTakeDeviceChk2=0;                                             //Steven 20250224 回吸檢測狀態
            if(BRCarryKit.UseSiteHasIC())
            {
                return false;
            }

            if(OutSHT2InLF()==false)
            {
                Task=100;                                                       //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
                return false;
            }

            MOT[MInShuttle2].fCanMoveM=false;
            Task=200;
            if(CheckIndexAllSuckICFallDown(false, true))                        //jou 20210713 : 增加index destroy ic時掉料偵測
            {
                ShowMyMessage("Index Arm2 掉料，請檢查IC是否有在Out Shuttle中!!");
            }

            if(IsNNMode()==NN_2Row)
                break;
            break;                                                              //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
        case 100:
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc);                                    //Steven 20170818 : 測試UPH用
            #else
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed))
            #endif
            {
                if(bHasDropErr)                                                                                         //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
                {
                    bHasDropErr=false;
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Enable       &&
                               BTestSuck.Suck[i][j].SenUsing!="" &&
                               BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                            {
                                if(BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    ErrPart+=IndexSuckName[i][j];
                                    bHasErr=true;
                                }
                            }
                        }
                    }

                    if(LastSet.iRealDummy==REALLY && bHasErr)
                    {
                        if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                     //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                            bIsTestSitICFallDown=true;
                        ShowErrorMessage("JAM0304", K_SKIP, MTestZ2, false, ErrPart);                                   //Steven 20100129 : Device Drop Error          //Jimmychiu 20260223 : 修正Index Arm 異常顯示位置錯誤
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(BTestSuck.Suck[i][j].Error ||
                                   (BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                    BTestSuck.Item[i][j]!=NULL_IC &&
                                    BTestSuck.Suck[i][j].GetStatus()==false))                                           //有用到且有吸到IC的卻掉了
                                {
                                    BTestSuck.SetItemData(i, j, HAS_NULL_IC);                                           //Steven 20110829 : 把有IC掉料的位置改成Has Null IC
                                    BTestSuck.Suck[i][j].Normal();                                                      //Steven 20110829 : 把真空關掉
                                }
                            }
                        }
                    }
                }

                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                    //JerryYang 20250723 : avoid shuttle hang up
                {
                    if(BLCarryKit.HasRealIC()==false &&                                                                 //Arm 2 no料
                       BRCarryKit.HasRealIC()==false)
                    {
                        BLCarryKit.SetHasNullIcToNullIc();
                        BRCarryKit.SetHasNullIcToNullIc();
                        MOT[MInShuttle2].fCanMoveM=true;
                    }
                }
                bSuckingFlagZ2=false;                                                                                   //Steven 20240916 : index下降到shuttle吸放料
                Task=1;
            }
            break;
        case 200:
            bHasDropErr|=CheckIndexAllSuckICFallDown(false, true);              //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
            if(bHasDropErr)
            {
                Task=100;
                break;
            }

            if(InShtInLF(1)==false)                                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
            {
                bZ2Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                ShowMyMessage("Shuttle 2 position error", "Shuttle 2 位置錯誤");
                MOT[MInShuttle2].fCanMoveM=true;
                Task=1;
            }
            else if(bCheckZ1==false || CheckZ1IsDown())                         //ChungHung 20111115
            {
                bSuckingFlagZ2=true;                                            //Steven 20240916 : index下降到shuttle吸放料
                flag2=false;                                                    //ChungHung 20110816 add Index Place slow down
                bZ2Isdownflag=true;                                             //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                if(BTestSuck.HasRealIC()==false)
                {
                    flag2=true;
                }
                else
                {
                    if(IniConfig.bD54SlowDown)
                    {
                        sp=GetIndexZSpeed(1);                                   //Steven 20160524 : Index Z軸速度整合為Function
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Place, sp))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                    else
                    {
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Place, MOT[MTestZ2].GailSpeed, IniConfig.iD46WaitIndexDestroyTime))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                }

                if(flag2)
                {
                    BTestSuck.ResetAll();
                    if(ATC_SYSTEM==eNewATCSystem && Temperature.bEnableTempRise==true &&
                       ATC_InterfaceForm->IS_ATC33())
                    {
                        DoTemperatureRise(1, true);
                        BTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
                        Task=309;
                    }
                    else
                    {
                        Task=310;
                    }
                }
            }
            break;
        case 309:
            if(BTempRiseHasICDelay.Off())
            {
                DoTemperatureRise(1, false);
                Task=310;
            }
            break;
        case 310:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j])
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Suck[i][j].Destroy())
                        {
                            if(IniConfig.bD44CheckIndexICDestroy)
                            {
                                if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    BTestSuck.Suck[i][j].Off();                                                         //Suck Off and Destroy On
                                    BTestSuck.bNeedCheck[i][j]=true;                                                    //Destroy finish
                                    iIndexTakeDeviceChk2=2;                                                             //kevin 20190103 回吸檢測狀態
                                }
                            }

                            if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                LastSet.iIndexInputOutPut[1]++;                                                         //kevin 20130125
                                BTestSuck.PordRec[i][j].AddIndexCycleTimeRecord(fRecindexCycleTim);                     //Frank 20160505 add
                                if(CosFunction.RecordIndexAirOnTime)                                                    //Sam 20220329 : Record Index Air On Time
                                    QueueAirOnTime2.Add(BTestSuck.Suck[i][j].sRecordStartOffDelayTime, BTestSuck.Suck[i][j].sRecordEndOffDelayTime, BTestSuck.Suck[i][j].iRecordOffDelayTime);
                            }
                            BRCarryKit.MoveSuckData(BTestSuck, i, j);
                            BRCarryKit.PordRec[i][j].AddIndexPlaceShuttleRecord(TestIF.iSiteMap[i][j], i, j);           //Sam 20201216 : Add record //Sam 20200716 : Add record
                            bArm2PutDuplicateErr[i][j]=false;
                        }
                    }
                }
            }
            flag=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j] &&
                       BTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            Task=320;
                            return false;
                        }
                    }
                }
            }

            if(BTestSuck.UseSiteHasIC())
            {
                iRetry=0;
                break;
            }

            if(TestZ2OutRandge())
            {
                iRetry++;
                if(iRetry>20)
                {
                    iRetry=0;
                    TestZ2SetPos();
                }
                else
                {
                    break;
                }
            }
            RecordIndexAirOnTime2();                                                                                    //Sam 20220329 : Record Index Air On Time
            iRetry=0;
            ResetArm2PutFlag();                                                                                         //Steven 20241017 : 打包成function
            if(IniConfig.bF25VibrateForOutShuttle)
            {
                DoVibrateOutShuttle(1, true);
            }

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                         //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                TestIF_File.bArm1PickPlaceArm2Test==true) &&
                TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                        //Steven 20150129 : 需要確認Arm2有沒有粘料
            {
                BLCarryKit.SetNullIcToHasNullIc();
                BRCarryKit.SetNullIcToHasNullIc();
            }
            Task=500;
            break;
        case 320:                                                                                                       //破壞異常處理
            RecordIndexAirOnTime2();                                                                                    //Sam 20220329 : Record Index Air On Time
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2PutDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        BTestSuck.Suck[i][j].Error=false;                                                               //Steven 20101229 : 換位置
                        bArm2PutDuplicateErr[i][j]=true;
                    }
                    else
                    {
                        bArm2PutDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                               //Vacuum sensor OFF error
            bHasErr=false;
            if(ATC_SYSTEM==eNewATCSystem &&
               Temperature.bEnableTempRise==true &&
               ATC_InterfaceForm->IS_ATC33())
            {
                DoTemperatureRise(1, true);
                BTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
                Task=309;
            }
            else
            {
                Task=310;
            }
            BTestSuck.ResetAll();                                                                                       //Sam 20220330 : 補 ResetAll
            break;
        case 500:
            if(IniConfig.bL42_UseOutShuttleDesoakTime==true)                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            {
                BTestSuck.TSoakTime.SetSecAndOn(IniConfig.iL42_UseOutShuttleDesoakTime);
            }
            iPos=Prod.TestZ1_Safe;
            sp=GetIndexZSpeed(1);                                               //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(iPos, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ2].Gali_MotMoveNoWait(iPos, sp, 0))
            #endif
            {
                bSuckingFlagZ2=true;                                            //Steven 20240916 : index下降到shuttle吸放料
                bZ2Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖 //Steven 20180522 : 換位置, 解除Hang up
                flag=false;
                if(IniConfig.bF25VibrateForOutShuttle)
                {
                    if(DoVibrateOutShuttle(1)==false)
                    {
                        break;
                    }
                }
                MOT[MInShuttle2].fCanMoveM=true;                                //JerryYang 20181219 fix D44異常時 shuttle剛好要搖搖會發生hang up
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190819 add index confim 一次4個SITE
                {
                    TotalErrPart="";
                    Task=650;
                    BRCarryKit.SetHasNullIcToNullIc();
                    return false;
                }

                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.bNeedCheck[i][j])
                            {
                                bIndexPlaceIcCheck=true;                        //kevin 20110505
                                flag=true;
                                if(INDEX_SUCKER_TYPE==1)                        //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[1][i][j]=true;
                                }
                                else
                                {
                                    BTestSuck.Suck[i][j].On();
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    bD44Arm2CheckVacOn=true;                                    //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                    fiosetview->ProcessIndexSuckDestroy2(1);                    //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 //kevin 20181102 add 提前動作
                    DoRearTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                    Task=600;
                    break;
                }
                else
                {
                    bD44Arm2CheckVacOn=false;                                   //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                }
                BRCarryKit.SetHasNullIcToNullIc();                              //jou 2011-06-29 out shuttle has_null_ic hang up fix    //Steven 20160524 : ADD SetHasNullIcToNullIc
                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            bArm2PutDuplicateErr[i][j]=false;
                            if(BTestSuck.bNeedCheck[i][j])
                            {
                                BTestSuck.Suck[i][j].OffDestroy();              //kevin 20130719 關吹氣
                            }
                        }
                    }
                }
                iIndexTakeDeviceChk2=0;                                         //kevin 20190103 回吸檢測狀態
                if(((iOneCycle && IsInArmOneCycleFinish()) ||
                    (iCleanOut && IsInArmCleanOutFinish())) &&
                    BLCarryKit.UseSiteNoIC())                                   //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                    Task=550;
                else
                    return true;
            }
            break;
        case 550:
            if(BRCarryKit.UseSiteNoIC() ||
               InSHT2InRT())
            {
                return true;
            }
            break;
        case 600:
            if(INDEX_SUCKER_TYPE==1)                                                                                    //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bArm2D44SuckCheck=false;
                bArm2D44SuckCheck=fiosetview->ProcessIndexSuckDestroy2(1);                                              //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
            }
            else
            {
                bArm2D44SuckCheck=true;
            }

            if(DoRearTestDestroyICDelay.Off() && bArm2D44SuckCheck==true)
            {
                flag=false;
                ErrPart=" ";
                bHasErr=false;
                bHasDuplicateErr=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(bArm2PutDuplicateErr[i][j])
                            bHasDuplicateErr=true;
                    }
                }
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.bNeedCheck[i][j])
                        {
                            if(BTestSuck.Suck[i][j].GetStatus())
                            {
                                bHasErr=true;
                                ErrPart+=IndexSuckName[i][j];
                                flag=true;
                                bArm2PutDuplicateErr[i][j]=true;
                                iIndexTakeDeviceChk2=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                BTestSuck.bNeedCheck[i][j]=false;
                                BTestSuck.Suck[i][j].Normal();
                                bArm2PutDuplicateErr[i][j]=false;
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                bD44Arm2CheckVacOn=false;                                                                               //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                if(bHasErr)
                {
                    if(MOT[MInArmY].ReadPos()<Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase])                     //kevin 20181107 shuttle 1
                    {
                       ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                        //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else if(IndexAlarmInArmAway()==true)                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    Task=500;                                                                                           //kevin 20181102
                    return false;
                }
                else
                {
                    iIndexTakeDeviceChk2=0;                                                                             //Steven 20250224 回吸檢測狀態
                }
                bIndexAlarmInArmAway=false;                                                                             //kevin 20181102 (Steven) :  index掉料 inarm 讓位 inarm先不要動
                bHasErr=false;
                if(flag==false)
                {
                    Task=500;
                }
                else
                {
                    if(IniConfig.bD44CheckIndexICDestroy)                                                               //jou 2011-11-01負壓不能一直開著真空，必須關掉
                    {
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(BTestSuck.bNeedCheck[i][j])
                                {
                                    bIndexPlaceIcCheck=true;                                                            //kevin 20110505
                                    if(INDEX_SUCKER_TYPE==1)                                                            //Steven 20111202
                                    {
                                        fiosetview->bIndexSuck[1][i][j]=true;
                                    }
                                    else
                                    {
                                        BTestSuck.Suck[i][j].On();
                                    }
                                }
                            }
                        }
                    }
                    DoRearTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                }
            }
            break;
        case 650:                                                               //kevin 2019819 add onecycle index check 4 Site
            if((ArmSpeed_File[IndexArm].bDevicConfirm ||
               (IniConfig.bD44CheckIndexICDestroy &&
                (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&               //JerryYang 20250120 : modify
                INDEX_SUCKER_TYPE==1)                                           //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE   //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 1, iSiteCount);
                Task=700;
            }
            break;
        case 700:                                                               //kevin 20190709 20190531 index check 4 Site
            if(IndexCheck4Site(false, 1, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<TestSocket.iShtCol/2)                             //JerryYang 20250120 : modify
                    Task=650;
                else
                {
                    Task=750;
                    iSiteCount=0;
//                    CheckSocketHasICDelay2.SetSecAndOn(0.5);     //Steven 20110908 : 上來後也要Delay一下
                }
            }
            break;
        case 750:
//            if(CheckSocketHasICDelay2.Off())
            {
                flag=false;
                if((ArmSpeed_File[IndexArm].bDevicConfirm ||
                   (IniConfig.bD44CheckIndexICDestroy &&
                    (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&           //JerryYang 20250120 : modify
                    TotalErrPart!="")                                           //kevin 20190629 add error show
                {
                   flag=true;
                   ShowErrorMessage("JAM0327", K_RETRY, MTestY2, false, TotalErrPart);
                }
                TotalErrPart="";
                if(flag)                                                        //Steven 20120726 : 有跑IC才檢查Socket
                {
                    Task=650;                                                   //kevin 20181102
                    return false;
                }
                else
                {
                    Task=800;                                                   //pass
                }
            }
            break;
        case 800:
            iIndexTakeDeviceChk2=0;                                             //kevin 20190103 回吸檢測狀態
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer hDoRearTestSuckIC;
TQPF_Timer hDoRearTestSuckICdelay;                                              //JerryYang 20220923 : add
int iRearTestSuckICTask=1;
void InitRearTestSuckICTask()
{
    iRearTestSuckICTask=1;
}
//------------------------------------------------------------------------------
extern int CheckOneCycleAction(int iTask);                                      //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
TQPF_Timer dwStartInitialCount2;
bool DoRearTestSuckIC()
{
    static int iHomeAlarm=0;
    static bool bOverHappen=false, bHasDropAtShuttle=false, bHasICErr=false;
    static AnsiString ErrPart2="";
    int ZPos=0;
    int sp=0;
    int &Task=iRearTestSuckICTask, ret=0;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    bool flag1=false, flag2=false;
    AnsiString ErrPart="", str="";
    switch(Task)
    {
        case 1:                                                                 //判斷Index與Shuttle上IC的狀態
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                return false;                                                   //kevin 20161102 break;
            }

            if(iOneCycle &&
               bUseTwoArm32Site &&                                              //針對NN mode的one cycle動作做額外卡控
               BTestSuck.UseSiteNoIC() &&                                       //Arm 2沒料
               BLCarryKit.UseSiteNoIC())
            {
                if(FTestSuck.HasRealIC()==false &&                              //Arm 1有料
                   FLCarryKit.HasRealIC()==false)
                {
                    FTestSuck.SetAllToNullIC();
                    FLCarryKit.SetAllToNullIC();
                    MOT[MInShuttle2].fCanMoveM=true;
                    return true;
                }
                else
                {
                    if(iOneCycle && IsInArmOneCycleFinish())                    //Sam 20250417 : 修正 AutoSiteOff 後 OneCycle Hangup 問題
                    {
                        return true;
                    }
                    else if(InSHT2InLF())                                       //等待
                    {
                        MOT[MInShuttle2].fCanMoveM=false;
                        return false;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            if((iOneCycle && IsInArmOneCycleFinish()) ||
               (iCleanOut && IsInArmCleanOutFinish()))
            {
                if(BTestSuck.UseSiteNoIC() &&
                   BLCarryKit.UseSiteNoIC() &&
                   InArmSuck.HasRealIC()==false)                                //JerryYang 20170801 (wei) 修正按one cycle hang up問題
                {
                    MOT[MInShuttle2].fCanMoveM=true;                            //RogerYang 20250820 : 1 --> 2
                    return true;
                }
            }

            if(iCleanOut          &&
               BLCarryKit.UseSiteNoIC()  &&
               InArmSuck.UseSiteNoIC()   &&
               MOT[MMPlate1].HasIC()==false &&
               MOT[MMPlate2].HasIC()==false)
            {
                return true;
            }

            if(CheckCFixTrayFullPlace() &&
               MOT[MTestZ2].Led[iHomeLed]==true)                                //Steven 20220524 : fixed for fix 3 氣缸 Hang up
            {
                MOT[MInShuttle2].fCanMoveM=true;
                return false;
            }

            if(BLCarryKit.UseSiteHasIC())
            {
                if(CanYieldAlarmRemainInSHT()==true)                            //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                    return true;
                }

                if(InSHT2InRT()==false)
                {
                    if((bUseTwoArm32Site==false && IndexStatus==Z1Down_Z2Up) ||
                       (bUseTwoArm32Site==true  && IndexStatus==Z1_Z2_Normal))
                        MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(bReadAndCheckCPUName==true &&
                   CosFunction.bInShuttleDetectByLatch &&                       //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                   Prod.bF18InshuttleDetect &&
                   MOTION_CARD_TYPE==MotionCard_Contec &&
                   LastSet.iRealDummy==REALLY)                                  //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                {
                    if(bInSht2LtcDetectTesterCanMove==false)
                    {
                        return false;
                    }
                }

                if((Prod.bF18InshuttleDetect ||                                 //Sam 20250925 : 修正關 Arm 後檢查 F18 功能被卡死 hangeup
                    IniConfig.bF20InShuttleProminentDetect) &&
                    CosFunction.bInShuttleDetectByLatch==false)
                {
                    if(bF18CheckShuttle2MustHasIC)
                        break;
                }

                if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
                   bInSh2DoLtc==true)                                           //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
                    return false;
                MOT[MInShuttle2].fCanMoveM=false;
                bSuckingFlagZ2=true;
                Task=300;
                bIndexZ2NeedUp=false;                                           //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            }
            break;
        case 300:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle2MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT2InRT()!=true)
                {
                    return false;
                }
            }
            bIndexPickUpErrMoveSht2=false;                                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
            MOT[MInShuttle2].fCanMoveM=false;
            bShuttle2MoveToRight=false;
            bShuttle2HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            if(CosFunction.bPurgeBeforePickShuttle)                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                InitRearTestPurgBeforePickShuttle();                            //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue Start
            flag2=false;
            Task=301;
            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&
               Temperature.iInitialStart2Time!=0)                               //kevin 20180905 (Steven) : add  iInitialStart2Time
            {
                bInitialStart2Time=true;                                        //wei 20171020 (jou) InitialStart1 秒數倒數
            }

            if(IsNNMode()==NN_2Row)
                break;
        case 301:                                                                                                                                               //確認Z軸位置 位置在下->吸取IC->Z軸到位後->Task=310
            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                                                                                                           //jou 2012-06-29 Index Pick up need wait Soak Time
                LastSet.iTemperature==Tempture_AmbientHot) &&                                                                                                   //kevin 20180903 (Steven) : add 恆溫控制
               Temperature.iInitialStart2Time!=0)
            {
                dwEndShuttle2Soak=MyTickCount();                                                                                                                //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常  //kevin 20180905 add  iInitialStart2Time
                if(iInitialStart2Count>0)
                {
                    iInitialStart2Count=(Temperature.iInitialStart2Time)-((dwEndShuttle2Soak-dwStartShuttle2Soak)/1000);                                        //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)                                                                                            //Steven 20161213 : Fix iInitialStart2 關ARM Hangup
                {
                }
                else
                {
                    if(BLCarryKit.TSoakTime.Off()==false)
                        break;
                }
                bInitialStart2Time=false;                                                                                                                       //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
            }

            if(bNeedTesterDuckingFinishLightYellowAndAlarmOn)                                                                                                   //ChungHung 20141015 add for SCK When the handler start running (finished Start count function),Yellow light blinking & alarm on (press "alarm reset" then clear)
            {
                bNeedTesterDuckingFinishLightYellowAndAlarmOn=false;
                bTesterDuckingFinishLightYellowAndAlarmOn=true;
            }

            if(TestIF_File.bIndexPickICWhenOutShtNoIC)                                                                                                          //Steven 20221207 : Index必須在out shuttle沒料才可以吸
            {
                if(BRCarryKit.HasRealIC())
                {
                    return false;
                }
            }

            if(TestIF.iShuttleMode==1 &&                                                                                                                        //jou 980317 SOFT_SIMULTE hang
               TestIF.iShuttle_Sel!=0 &&
               bCheckShuttle2Flag)
                return false;
            MOT[MInShuttle2].ScanMotorStatus();
            if(BLCarryKit.UseSiteHasIC() && bCheckShuttle2Flag==false)
            {
                flag2=false;
                if(CheckZ1IsDown())
                {
                    if(BLCarryKit.HasRealIC()==false)
                    {
                        flag2=true;
                    }
                    else
                    {
                        ZPos=MOT[MTestZ1].Gali_ReadPos();
                        //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
                        if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
                        {
                        }
                        else if(CosFunction.bSortingBy2DList==true &&           //jou Index更換Offset會hang up? 2010-04-13
                           LastSet.iTester==_2D_SORT &&
                           TestIF_File.bSortingBy2DIDList==true)                                                                                                //Frank 20221122 : 2DID sorting for ATK
                        //<== Eastsun 20260511 F007 整合
                        {
                        }
                        else
                        {
                            if(bUseTwoArm32Site==false)
                            {
                                if(ZPos>=(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+500))                                                                        //2008/06/24
                                {
                                    if(ZPos!=iBackUpZ1DownPosition)
                                        return false;
                                }
                            }
                        }

                        if(MOT[MTestZ2].Gali_ReadPos()<(Prod.TestZ2_Pick+1000))                                                                                 //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                        {
                            bZ2PickShuttle=true;
                        }

                        if(flag2==false)
                        {
                            bSuckingFlagZ2=true;                                                                                                                //Steven 20240916 : index下降到shuttle吸放料
                            sp=GetIndexZSpeed(1);                                                                                                               //Steven 20160524 : Index Z軸速度整合為Function
                            if(CosFunction.bPurgeBeforePickShuttle)                                                                                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                            {
                                if(DeviceForm_File.bPurgeBeforePickShuttle)                                                                                     //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue
                                {
                                    if(DoRearTestPurgBeforePickShuttle(sp)==false)
                                        break;
                                }
                            }
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick, sp);
                            #endif
                        }
                    }
                }
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac &&                                                                                                //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm2PressSkipNeedDownCheckVac==true &&
                    BLCarryKit.UseSiteNoIC())
            {
                flag2=false;
                if(CheckZ1IsDown())
                {
                    ZPos=MOT[MTestZ1].Gali_ReadPos();
                    //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
                    if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
                    {
                    }
                    else if(IsNNMode()!=NN_2Row)                                                                                                                     //jou Index更換Offset會hang up? 2010-04-13
                    //<== Eastsun 20260511 F007 整合
                    {
                        if(ZPos>=(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+500))                                                                                //2008/06/24
                        {
                            if(ZPos!=iBackUpZ1DownPosition)
                                return false;
                        }
                    }

                    if(MOT[MTestZ2].Gali_ReadPos()<(Prod.TestZ2_Pick+1000))                                                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                    {
                        bZ2PickShuttle=true;
                    }

                    if(flag2==false)
                    {
                        bSuckingFlagZ2=true;                                                                                                                    //Steven 20240916 : index下降到shuttle吸放料
                        if(CUSTOMER_CODE==CC_AMKOR_Philippines)                                                                                                 //Frank QQ
                        {
                            flag2=true;
                        }
                        else
                        {
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick, iSpeedSlow);                                                                      //JerryYang 20170610 這裡改慢速
                            #endif
                        }
                    }
                }
            }
            else
            {
                Task=1;
                break;
            }

            if(LastSet.iRealDummy==REALLY && ArmSpeed[IndexArm].bSuckOnDown && INDEX_SUCKER_TYPE==0)
            {
                for(int i=0; i<BLCarryKit.iShtRow; i++)
                {
                    for(int j=0; j<BLCarryKit.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j]!=NULL_IC &&
                           BLCarryKit.Item[i][j]!=HAS_NULL_IC)
                        {
                            BTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            if(flag2)
            {
                RecordIndexPosition(2, 0);                                                                                                                      //Isaac 20200922 : 紀錄indexArmY encoder值和command值，Arm2/Shuttle
                EncoderTeachingMaxMinCount(2);                                                                                                                  //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
                flag2=false;                                                                                                                                    //JerryYang 20170610 (wei) 移到上面
                bZ2PickShuttle=true;                                                                                                                            //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                if(CosFunction.bIndexPickErrSkipNeedCheckVac &&
                   IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                   bArm2PressSkipNeedDownCheckVac==true &&
                   BLCarryKit.UseSiteNoIC())                                                                                                                    //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[1][i][j]==true)
                            {
                                bArm2SuckFinish[i][j]=false;                                                                                                    //Steven 20110301 : 初始化，都當作還沒做完
                                bArm2DuplicateErr[i][j]=false;
                                BTestSuck.Suck[i][j].Reset();                                                                                                   //Steven 20140213 : Jordan說Index下去不吸直接Alarm
                            }
                        }
                    }
                    Task=307;                                                                                                                                   //JerryYang 20170610 (wei) 這裡跳去檢查按skip的真空
                    break;
                }
                ResetInxedArm2Flag();                                                                                                                           //Steven 20241017 : 打包成function
                if(CosFunction.bUseShuttlePickShiftDetect==true &&
                   DeviceForm_File.dDropByPassDetect!=0)                                                                                                        //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    if(TestIF_File.iShuttleMode==0 ||                                                                                                           //Ifor 20221013 add:關Arm不處理避免資料轉移異常
                       (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                    {
                        bIndexZ2NeedUp=true;
                    }
                    else
                    {
                        bIndexZ2NeedUp=false;
                    }
                }
                else
                {
                    bIndexZ2NeedUp=false;
                }

                if(USE_IO_CHANGE_TOQUE==true &&                                                                                                                 //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
                   (bNeedCheckIndexToque==true ||
                    bNeedCheckIndexToque2==true))                                                                                                               //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque2=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                    fMain->edTorue1->Text="";
                    bOverHappen=false;
                    hDoRearTestSuckIC.SetSecAndOn(1);
                }
                bHasDropAtShuttle=false;
                if(IniConfig.bD81IndexCheckVacuumOnShuttle)
                {
                    for(int i=0; i<MAX_Index_Row; i++)                                                                                                          //JerryYang 20241002 : add
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BLCarryKit.Item[i][j]==NULL_IC || BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                if(INDEX_SUCKER_TYPE==1 && bDropAtSht2NeedCheckVac[i][j]==true)                                                                 //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[1][i][j]=true;
                                    bHasDropAtShuttle=true;
                                }
                            }
                        }
                    }
                }
                Task=310;
            }
            break;
        case 307:                                                               //JerryYang 20170610 (wei) JSCC要求index arm pick up error按SKIP要慢速下降再吸一次
            bArm2SuckComplete=true;
            ErrPart="";
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bSkipNeedCheckVac[1][i][j] &&
                       BLCarryKit.Item[i][j]==NULL_IC)
                    {
                        if(BLCarryKit.Item[i][j]==NULL_IC &&
                           bArm2SuckFinish[i][j]==true)                         //Steven 20110301
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                               BTestSuck.Item[i][j]==NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                            }
                        }
                        else
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_IC &&
                               BTestSuck.Suck[i][j].Suck())
                            {
                                bHasErr=true;                                   //按skip後卻吸到IC，要跳alarm
                                bArm2DuplicateErr[i][j]=true;
                                bArm2SuckFinish[i][j]=true;
                            }
                            else if(BTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bArm2SuckFinish[i][j]=true;
                            }
                            else if(BTestSuck.Item[i][j]==HAS_IC &&
                                    BTestSuck.Suck[i][j].Suck())                //Ifor 20171122 (Steven) :add 修正bArmSuckFinish 資料異常造成Hangup
                            {
                                bArm2SuckFinish[i][j]=true;
                            }
                            else
                            {
                                bArm2SuckComplete=false;                        //jou 2011-08-16 只要有未完成的就繼續等
                            }
//                            else
//                                flag1=false;                                  //jou 2011-08-16 只要有未完成的就繼續等
                        }
                    }
                    else
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Item[i][j]==NULL_IC)
                            BTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        bArm2SuckFinish[i][j]=true;                             //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2SuckFinish[i][j]==false)                            //只要有未完成的就繼續等
                        bArm2SuckComplete=false;
                }
            }

            if(bArm2SuckComplete==true)                                         //Steven 20110301 : 所有吸嘴都做完
            {
                bArm2SuckComplete=false;
                if(bHasErr)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[1][i][j]==true &&
                               bArm2DuplicateErr[i][j]==true)
                                BTestSuck.Suck[i][j].Normal();
                        }
                    }
                    Task=308;
                    return false;
                }

                if(BLCarryKit.HasRealIC())
                    break;
                ZeroMemory(bArm2DuplicateErr, sizeof(bArm2DuplicateErr));
                if(BLCarryKit.UseSiteHasIC())
                    break;
                iD43AutoRetryWhenIndexPickErrCnt[0]=0;
                bResetIndexArm1Pick=false;
                bArm2PressSkipNeedDownCheckVac=false;
                ResetArm2VacFlag();                                             //Steven 20241017 : 打包成function
                Task=500;
            }
            break;
        case 308:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeedSlow))         //JerryYang 20180628 (wei) 只有pick up error的arm要上升
            {
                bSuckingFlagZ2=false;                                           //Steven 20240916 : index下降到shuttle吸放料
                Task=309;
            }
            break;
        case 309:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bArm2PressSkipNeedDownCheckVac=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(bSkipNeedCheckVac[1][i][j]==true)
                            ErrPart+=IndexSuckName[i][j];
                        if(CUSTOMER_CODE!=CC_AMKOR_Philippines)                 //Frank QQ
                            bSkipNeedCheckVac[1][i][j]=false;
                    }
                }

                if(CUSTOMER_CODE==CC_AMKOR_Philippines)                         //Frank QQ
                {
                    bArm2PressSkipNeedDownCheckVac=true;
                    str.sprintf("Arm2 detect %s has device",ErrPart);
                    ShowMyMessage(str,"請檢查Arm2上是否有IC");
                    ErrPart="";
                    Task=3091;
                }
                else
                {
                    str.sprintf("Arm2 detect Shuttle %s has device",ErrPart);
                    ShowMyMessage(str,"請檢查shutle2上是否有IC");
                    ErrPart="";
                    Task=500;
                }
            }
            break;
        case 3091:
            ResetInxedArm2Flag();                                               //Steven 20241017 : 打包成function
            Task=307;
            break;
        case 3099:                                                              //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            bSuckingFlagZ2=true;                                                //Steven 20240916 : index下降到shuttle吸放料
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick+DeviceForm_File.dDropByPassDetect*100, iSpeedSlow))
            {
                ResetInxedArm2Flag();                                           //Steven 20241017 : 打包成function
                Task=310;
            }
            break;
        case 310:                                                                                                       //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
            if(USE_IO_CHANGE_TOQUE==true && bOverHappen==false)                                                         //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
            {
                if(fMain->edTorue1->Text!="")
                {
                    if(atoi(fMain->edTorue1->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z2 Torque Over,Z2 Pick from Shuttle need up","Index Z2 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoRearTestSuckIC.Off()==false)
                {
                    break;                                                                                              //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                                                                   //Over Wait Time
                }
            }
            bArm2NeedSuck=true;                                                                                         //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
            bArm2SuckComplete=true;
            DoArm2Suck();                                                                                               //JerryYang 20190123 把index arm吸真空&交換狀態包成函式
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2SuckFinish[i][j]==false)                                                                    //只要有未完成的就繼續等
                        bArm2SuckComplete=false;
                }
            }

            if(bArm2SuckComplete==true)                                                                                 //Steven 20110301 : 所有吸嘴都做完
            {
                if(bIndexZ2NeedUp)                                                                                      //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    bIndexZ2NeedUp=false;
                    Task=3099;
                    break;
                }
                BTestSuck.HotCount=BLCarryKit.HotCount;                                                                 //JerryYang 20230204 : add hot plate放置順序資料
                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasDropAtShuttle==true)                                  //JerryYang 20241002 : add
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(fiosetview->ProcessIndexSuckDestroy2()==true)
                        {
                            bHasICErr=false;
                            ErrPart2="";
                            for(int i=0; i<MAX_Index_Row; i++)
                            {
                                for(int j=0; j<NEW_MAX_Index_Col; j++)
                                {
                                    #ifdef SOFT_SIMULTE
                                    if((BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC) && i==0 && j==3)
                                    #else
                                    if((BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC) && BTestSuck.Suck[i][j].GetStatus()==true)
                                    #endif
                                    {
                                        bHasICErr=true;
                                        bSHT2_DropPosHasIC[i][j]=true;
                                        if(TestIF_File.iTestMode==_32Site4X8M ||
                                           TestIF_File.iTestMode==_32Site4X8N ||
                                           TestIF_File.iTestMode==_16Site4X4)                                           //Sam 20190226 : 16Site4X4 ///kevin 20180504 add  error pos
                                            ErrPart2+=IndexSuckName[i+2][j];
                                        else if(TestIF_File.iTestMode==QualSite2X2N ||
                                                TestIF_File.iTestMode==_6Site2X3N)
                                            ErrPart2+=IndexSuckName[i+1][j];                                            //Steven 20230712 : 修正NN mode alarm顯示
                                        else
                                            ErrPart2+=IndexSuckName[i][j];
                                        BTestSuck.Suck[i][j].Normal();
                                    }
                                }
                            }

                            if(bHasICErr==false)
                            {
                                for(int i=0; i<MAX_Index_Row; i++)
                                {
                                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                                    {
                                        bDropAtSht2NeedCheckVac[i][j]=false;
                                        bSHT2_DropPosHasIC[i][j]=false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                bHasErr=false;
                //──── DEBUG: 強制觸發 JAM0302 Rear pick-up error ────
//                static bool bDebugForceJAM0302Rear = true;   // ← 設 true 重現，false 關閉
//                if(bDebugForceJAM0302Rear)
//                {
//                    bHasErr = true;
//                    BTestSuck.Suck[0][0].Error = true;
//                    bDebugForceJAM0302Rear = false;           // 只觸發一次
//                }
                //──── DEBUG END ────
                bArm2NeedSuck=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            if(USE_IO_CHANGE_TOQUE==true)                                                               //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
                            {                                                                                           //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
                                fMain->chkReadTorque1->Checked=false;
                                fMain->chkReadTorque2->Checked=true;
                                fMain->edTorue1->Text="";
                                bOverHappen=false;
                                hDoRearTestSuckIC.SetSecAndOn(1);
                            }

                            if(IniConfig.bD62PickUpErrorNeedPurge)                                                      //Steveb 20161024 : 吸取異常需要吹氣一次
                            {
                                BTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bHasErr || (IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr))
                {
                    bShuttle2HasPickErr=true;                                                                           //Steven 20230118 : 避免In arm 偷放料, 往上移動
                    bIndexPickUpErrMoveSht2=true;                                                                       //Steven 20221107 : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    if(CosFunction.bIndexPickErrSkipBlowAirTime)                                                        //Frank QQ
                    {
                        Task=3051;
                        return false;
                    }
                    else
                    {
                        Task=305;
                        return false;
                    }
                }

                if(BLCarryKit.HasRealIC())
                    break;
                if(TestIF_File.iShuttleMode==0 ||
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==1))                                                                       //Jou 20230224 : 修正Auto Site Map關arm異常
                {
                    if(bAutoSiteMapWaitTestResult==true &&
                       BTestSuck.HasRealIC()==false &&
                       FTestSuck.HasRealIC()==false &&                                                                  //Steven 20200326 : 修正JCET Auto site map發生inarm掉料會hang up
                       TestSocket.HasRealIC()==false &&
                       FLCarryKit.HasRealIC()==false &&
                       BLCarryKit.HasRealIC()==false)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                        }
                        else
                        {
                            bAutoSiteMapWaitTestResult=false;
                        }
                    }
                }
                ZeroMemory(bArm2DuplicateErr, sizeof(bArm2DuplicateErr));
                if(BLCarryKit.UseSiteHasIC())
                    break;
                bResetIndexArm2Pick=false;
                if(CUSTOMER_CODE==CC_TSMC_TAINAN &&
                   (Prod.bWhenNoFullSiteUseInitialDelay &&                                                              //wei 20161102 No FullSite delay修改||->&&
                    IniConfig.bL18NofullsiteaddTemperatureoffset))                                                      //wei 20151228 No FullSite delay
                {
                    CheckBTFullSite();
                }
                Task=500;
                if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
                   LastSet.iTemperature==Tempture_Hot &&                                                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                   FTestSuck.HasRealIC()==false)
                {
                    hDoRearTestSuckICdelay.SetSecAndOn(Temperature.fJamSoakTime);
                }
                iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                                  //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
            }
            break;
        case 3051:                                                              //Frank QQ
            bSuckingFlagZ2=true;                                                //Steven 20240916 : index下降到shuttle吸放料
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Place, MOT[MTestZ2].GailSpeed, 0))
            {
                hDoRearTestSuckIC.SetSecAndOn(IniConfig.iD62IndexBlowAirTime);
                Task=3052;
            }
            break;
        case 3052:                                                              //Frank QQ
            if(hDoRearTestSuckIC.Off())
            {
                Task=305;
            }
            break;
        case 305:
            if(USE_IO_CHANGE_TOQUE==true &&                                     //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
               bOverHappen==false)                                              //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
            {
                if(fMain->edTorue1->Text!="")
                {
                    if(atoi(fMain->edTorue1->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z2 Torque Over,Z2 Pick from Shuttle need up","Index Z2 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoRearTestSuckIC.Off()==false)
                {
                    break;                                                      //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                           //Over Wait Time
                }
            }
            CheckIndexAllSuckICFallDown(false, true);                           //Steven 20110725 : 修改負壓檢查方式
            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed, 0))
            #endif
            {
                bSuckingFlagZ2=false;                                           //Steven 20240916 : index下降到shuttle吸放料
                Task=306;                                                       //Steven 20160718 : Index pick up error with [D43]
            }
            break;
        case 306:
            bZ2PickShuttle=false;                                               //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
            {
                if(BRCarryKit.UseSiteNoIC())                                    //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle2].fCanMoveM=true;
                    bShuttle2MoveToLeft=true;
                    bShuttle2HasPickErr=true;                                   //Steven 20220712 : 避免In arm 偷放料
                    bIndexPickErrShtStayRight2=false;                           //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    bCheckNullIC2=false;                                        //JerryYang 20170623 (wei) 修正有裝out shuttle 前後對照的機台發生index arm吸取異常無法跳出alram造成hang up
                    Task=320;
                }
            }
            else
            {
                if(bPlaceToShuttle2Step)                                        //Steven 20160718 : 避免放蝦頭放到一半讓位會死雞
                    return false;
                Task=320;
            }
            break;
        case 320:                                                               //等Shuttle移出來
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle2MoveToLeft)                                             //ChungHung 20131015 fix hangup
            {
                if(InSHT2InLF()!=true)
                {
                    return false;
                }
            }
            bShuttle2MoveToLeft=false;
            MOT[MInShuttle2].fCanMoveM=false;
            Task=321;
            if(IsNNMode()==NN_2Row)
                break;
        case 321:                                                                                                       //ChungHung 20130924 add
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            bSht1OnlyHasICErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2DuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        if(IniConfig.bD62PickUpErrorNeedPurge)                                                          //Steveb 20161024 : 吸取異常需要吹氣一次
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                        BLCarryKit.PordRec[i][j].AddErrorRecordNoSave("JAM0302");
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)
            {
                if(IndexAlarmInArmAway()==false)                                                                        //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                bHasErr=false;
                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr==true)
                {
                    bHasICErr=false;
                    str.sprintf("Need to remove device of Shuttle2 %s", ErrPart2);
                    ShowMyMessage(str);
                }

                if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                           //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                   iD43AutoRetryWhenIndexPickErrCnt[1]==0)
                {
                    ret=K_RETRY;
                }
                else if(IniConfig.bNewResetFunction==true &&
                        bResetIndexArm2Pick==true)
                {
                    ret=K_SKIP;
                }
                else
                {
                    if(CosFunction.bJAM0301NeedOpenChamberDoor)                                                         //wei : JAM0301 & JAM0302需要開啟Chamber門10秒
                    {
                        bIsTestSitICFallDown=true;
                    }

                    if(IniConfig.bIndexPickErrOnlySKIP==true ||                                                         //jou 2012-02-13 index pick-up error only skip
                       IniConfig.bD64IndexPickErrOnlySKIP)                                                              //kevin 20171103 (wei) add retry function
                        ret=ShowErrorMessage("JAM0302", K_SKIP, MTestZ2, bHasDuplicateErr, ErrPart);                    //Devicr Pick-Up Error
                    else
                        ret=ShowErrorMessage("JAM0302", K_SKIP|K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);            //Devicr Pick-Up Error
                }

                if(ret==K_SKIP)
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap)
                        iAutoSiteMapCount++;                                                                            //Steven 20220811 : 紀錄目前是哪個Shuttle
                    bAutoSiteMapWaitTestResult=false;                                                                   //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                    bIndexPickUpErrMoveSht2=false;                                                                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                              //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]==NULL_IC)                                                           //jou 2011-12-27 有發生過Skip又重吸一次,改成下面的方式
                            {
                                if(BTestSuck.Suck[i][j].Error)                                                          //JerryYang 20200303 fix 沒IC的地方jam count被++
                                {
                                    if(CosFunction.bUseSCKART)                                                          //Steven 20161214 (wei) : For SCK ART
                                    {
                                        if(TestIF_File.bRENESAS_EnableFTCT==true)                                       //RogeryYang 20251014 : FTCT add to HdRejectBT
                                            fSCKART->iInputJamCnt++;
                                        else
                                            fSCKART->AddOutputJamCnt(i, j, ret);                                        //RogerYang 20250923 : 整合ART OutArm JamCount
                                    }
                                }
                                BLCarryKit.PordRec[i][j].AddErrorRecord("JAM0302");                                     //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                BTestSuck.Suck[i][j].Normal();
                                if(CosFunction.bIndexPickErrSkipNeedCheckVac==true &&
                                   IniConfig.bD50IndexPickErrSkipNeedCheckVac)                                          //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                                {
                                    bSkipNeedCheckVac[1][i][j]=true;
                                    bArm2PressSkipNeedDownCheckVac=true;
                                }
                            }
                            BLCarryKit.SetItemData(i, j, NULL_IC);
                            bArm2DuplicateErr[i][j]=false;
                        }
                    }

                    if(IniConfig.bNewResetFunction==true &&
                       bResetIndexArm2Pick==true)
                    {
                    }
                    else
                    {
                        if(IniConfig.bD42IndexPickICShuttlePause)
                        {
                            bInArmNeedToSafePos=true;
                            bShuttle2Pause=true;
                            bIndexArm2PickupErrStop=true;                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                            bShowShuttle2Device=true;                                                                   //kevin 20180504 index pick up error
                        }
                    }
                    bIndexPickUpErrorWaitRetry=false;                                                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight2=false;
                    }

                    if(IniConfig.bD43IndexPickErrCheckSocket==true)                                                     //Steven 20190115 : SCC要求吸取異常要檢查Socket
                    {
                        bIndexArm2PickUpErrNeedPiggyback=true;
                        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                            //Sam 20230108 : 矽格俊堯要求不要 Reset IC
                        {
                        }
                        else
                        {
                            fMain->ResetRecordforPiggyBack("RESET_ForIndexPickUpErr");
                        }
                        iWhoTriggerPiggyBack=pbtIndexArmPickUpErr;
                        ProcessPiggyBackFunction();
                    }
                }
                else
                {
                    if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                       //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                       iD43AutoRetryWhenIndexPickErrCnt[1]==0)
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[1]++;
                    }
                    else
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                          //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                                if(BTestSuck.Suck[i][j].Error)
                                    bArm2DuplicateErr[i][j]=true;
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight2=true;
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }
                BTestSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空
            }
            else if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr==true)
            {
                if(IndexAlarmInArmAway()==false)                                                                        //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                bHasICErr=false;
                bSht2OnlyHasICErr=true;
                str.sprintf("Need to remove device of Shuttle2 %s", ErrPart2);
                ShowMyMessage(str);
                if(IniConfig.bD42IndexPickICShuttlePause)
                {
                    bInArmNeedToSafePos=true;
                    bShuttle2Pause=true;
                    bIndexArm2PickupErrStop=true;                                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                    bShowShuttle2Device=true;                                                                           //kevin 20180504 index pick up error
                }
            }

            if(bShuttle2Pause)
                MOT[MInShuttle2].SetSpeed(10);                                                                          //kevin 20180226 (Steven) add pick up error  shuttle down speed
            if(IniConfig.bNewResetFunction==true && bResetIndexArm2Pick==true)
            {
                bResetIndexArm2Pick=false;
            }
            else
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                         //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle2].fCanMoveM=true;                                                                    //等待Shuttle 移至右邊
                    bShuttle2MoveToRight=true;
                    bShuttle2HasPickErr=true;                                                                           //Steven 20220712 : 避免In arm 偷放料
                }
            }

            if(IniConfig.bD42IndexPickICShuttlePause &&                                                                 //AI(staterecord-analysis) 20260417 (RogerYang) : D42 SKIP後強制走500, 避免300讓Z2再次下降造成四方死鎖
               bShuttle2Pause)
            {
                Task=500;
            }
            else if(BLCarryKit.HasRealIC())
            {
                Task=300;
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac==true &&
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm2PressSkipNeedDownCheckVac==true)                                                               //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
            {
                Task=300;
            }
            else
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j])
                        {
                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                            {
                                BLCarryKit.SetItemData(i, j, NULL_IC);
                            }

                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                                bArm2DuplicateErr[i][j]=false;
                            }
                        }
                    }
                }

                if(BLCarryKit.UseSiteHasIC())
                {
                    Task=300;
                    break;
                }
                Task=500;
            }
            break;
        case 500:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle2MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT2InRT()!=true)
                {
                    return false;
                }
            }
            CheckShuttle2EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log
            MOT[MInShuttle2].fCanMoveM=false;
            bShuttle2MoveToRight=false;
            bShuttle2HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            bIndexPickUpErrorWaitRetry=false;                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
            if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
               LastSet.iTemperature==Tempture_Hot)                              //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
            {
                if(fFrontNeedTest==true && bD52IndexArmUp==false)               //JerryYang 20200812 : fix RTC error hang up
                    break;
                if(FTestSuck.HasRealIC()==false)                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                {
                    if(hDoRearTestSuckICdelay.Off()==false)
                        break;
                }
            }
            Task=501;
            iHomeAlarm=0;
            if(IsNNMode()==NN_2Row)
                break;
        case 501:                                                                                                       //ChungHung 20130924 add
            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }
            sp=GetIndexZSpeed(1);                                                                                       //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            flag1=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc);
            #else
            flag1=MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Safe, sp, 0);
            #endif
            if(flag1)
            {
                bSuckingFlagZ2=true;                                                                                    //Steven 20240916 : index下降到shuttle吸放料
                #ifndef SOFT_SIMULTE
                if(TestIF_File.iShuttleMode==0 ||                                                                       //Ifor 20160303 新增馬達回Home保護機制
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==1))
                {
                    MOT[MTestZ2].ScanMotorStatus();
                    if(MOT[MTestZ2].Led[iHomeLed]==false)
                    {
                        if(iHomeAlarm>10)
                        {
                            ShowMyMessage("Motor Z2 not at the origin!!", "馬達Z2不在原點上!");
                            fAllMotorHome=false;
                            iHomeAlarm=0;
                        }
                        else
                        {
                            iHomeAlarm++;
                        }
                        return false;
                    }
                }
                #endif
                iHomeAlarm=0;
                bZ2PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                         //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                {
                    bIndexPickErrShtStayRight2=false;
                }
                #ifndef SOFT_SIMULTE
                {
                    if(LastSet.iRealDummy==REALLY)                                                                      //Steven 20110131 Start
                    {
                        ErrPart=" ";
                        bHasErr=false;
                        bHasDuplicateErr=false;
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(bArm2DuplicateErr[i][j])
                                    bHasDuplicateErr=true;
                                if(BTestSuck.Item[i][j]!=NULL_IC &&
                                   BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    bHasErr=true;
                                    BTestSuck.Suck[i][j].Normal();                                                      //Steven 20110707 : 預防負壓掉料
                                    BTestSuck.Suck[i][j].Error=true;
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    BTestSuck.Suck[i][j].Error=false;
                                }
                            }
                        }

                        if(bHasErr)
                        {
                            if(IndexAlarmInArmAway()==false)                                                            //Steven 20130613 : Index異常時, In Arm要先讓位功能
                            {
                                return false;
                            }
                            bHasErr=false;
                            if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                 //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                                bIsTestSitICFallDown=true;                                                              //kevin 20130706
                            if(IniConfig.bIndexDropOnlySKIP==true ||
                               IniConfig.bKoreaFunction)                                                                //jou 2012-02-13 index drop error only skip
                            {
                                ret=ShowErrorMessage("JAM0304", K_SKIP, MTestZ2, bHasDuplicateErr, ErrPart);            //Device Drop Error
                            }
                            else
                            {
                                ret=ShowErrorMessage("JAM0304", K_SKIP|K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);    //Device Drop Error
                            }

                            if(ret==K_SKIP)
                            {
                                bAutoSiteMapWaitTestResult=false;                                                       //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                                for(int i=0; i<BTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<BTestSuck.iShtCol; j++)
                                    {
                                        if(BTestSuck.Suck[i][j].Error)
                                        {
                                            BTestSuck.PordRec[i][j].AddErrorRecord("JAM0304");                          //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                            if(CosFunction.bUseSCKART)                                                  //Steven 20161214 (wei) : For SCK ART
                                            {
                                                fSCKART->AddOutputJamCnt(i, j, ret,
                                                    Prod.bIsPassBin[BTestSuck.iBinData[i][j]]);                         //RogerYang 20250923 : 整合ART OutArm JamCount
                                            }
                                            BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                            BTestSuck.Suck[i][j].Error=false;
                                        }
                                        bArm2DuplicateErr[i][j]=false;
                                    }
                                }

                                if(IniConfig.bD42IndexPickICShuttlePause)                                               //ChungHung 20110302 start
                                {
                                    bInArmNeedToSafePos=true;
                                    bShuttle2Pause=true;
                                    bIndexArm2PickupErrStop=true;                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                                    bShowShuttle2Device=true;                                                           //kevin 20180504 index pick up error
                                }

                                if(IniConfig.bD43IndexPickErrCheckSocket==true)                                         //Steven 20190115 : SCC要求吸取異常要檢查Socket
                                {
                                    bIndexArm2PickUpErrNeedPiggyback=true;
                                    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                //Sam 20230108 : 矽格俊堯要求不要 Reset IC
                                    {
                                    }
                                    else
                                    {
                                        fMain->ResetRecordforPiggyBack("RESET_ForIndexDropUpErr");
                                    }
                                    iWhoTriggerPiggyBack=pbtIndexArmPickUpErr;
                                    ProcessPiggyBackFunction();
                                }
                            }
                            else
                            {
                                for(int i=0; i<BTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<BTestSuck.iShtCol; j++)
                                    {
                                        if(BTestSuck.Suck[i][j].Error)
                                        {
                                            bArm2DuplicateErr[i][j]=true;
                                            BLCarryKit.MoveSuckData(BTestSuck, i, j);
                                            BTestSuck.Suck[i][j].Error=false;
                                        }
                                    }
                                }
                                Task=300;
                                return false;
                            }
                        }
                    }
                }
                #endif
                if(bShuttle2Pause)
                    MOT[MInShuttle2].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed
                MOT[MInShuttle2].fCanMoveM=true;
                BTestSuck.SetUnuseToNullIC();                                                                           //Steven 20241017 : 清除沒用到的資料
                bSuckingFlagZ2=false;
                ResetInxedArm2Flag();                                                                                   //Steven 20241017 : 打包成function
                if(fSCKART->iInfo_MultiLotCnt>1)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]!=NULL_IC &&
                               BTestSuck.cDeviceInf[i][j]!="")
                            {
                                LotSummary.AddByLotLoadCount(BTestSuck.cDeviceInf[i][j]);
                            }
                        }
                    }
                }

                if(DeviceForm_File.ContactMode==DirectContactSoftEP ||
                   DeviceForm_File.ContactMode==DropContactSoftEP)                                                      //kevin 20130608 Soft Contact mode
                {
                     APAX_WriteData(true, 0, 2);                                                                        //ARM2 shuttle 吸完ic 浮動頭不充氣
                }
                fAutoTeach->SetIndexBinError(1);                                                                        //JimmyChiu 20211020 : Auto alignment mode
                if(bHPCleanout)                                                                                         //wei 20160624 Hotplate clean out
                {
                    return true;
                }
                else if(LastSet.iTemperature==Tempture_Hot && bInitialSackTime &&                                       //2013-11-27   Dell    需要做Index soak time
                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false &&                                       //JerryYang 20220805 : 有開下壓shuttle預熱功能就不要做懸空預熱
                        (Temperature.iInitialStart1Time>0 || Temperature.iIndexSoakTime>0))                             //kevin 20131112 第一次吸取ic等待時間)
                {
                    int iSetTmr=Temperature.iInitialStart1Time;
                    if(Temperature.iIndexSoakTime>Temperature.iInitialStart1Time)
                        iSetTmr=Temperature.iIndexSoakTime;

                    DoTestHeadMotorDelay2.SetSecAndOn(iSetTmr);
                    Task=502;
                    bInitialSackTime=false;
                    bInitialStart1Time=true;                                                                            //wei 20171020 (jou) InitialStart1 秒數倒數
                    dwStartInitialCount2.LatchCycleTime(true);
                    iInitialStart1Count=Temperature.iInitialStart1Time;
                }
                else
                {
                    return true;
                }
            }
            break;
        case 502:                                                               //kevin 20131112  等待initial sock time
            if(bInitialStart1Time)                                              //wei 20171020 (jou) InitialStart1 秒數倒數
            {
                if(iInitialStart1Count>0)
                {
                    iInitialStart1Count=(Temperature.iInitialStart1Time)-(dwStartInitialCount2.LatchCycleTime()/1000);
                    break;
                }
            }

            if(DoTestHeadMotorDelay2.Off())
            {
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iBTestSuckTestICTask=1;
void InitBTestSuckTestICTask()
{
    iBTestSuckTestICTask=1;
    bZ2Isdownflag=false;
}
//------------------------------------------------------------------------------
TQPF_Timer DoBTestSuckTestICDelay;
extern int SendTestResultToHttp();
//==> Eastsun 20260511 F007 整合: T8 case 8011/8012 用 SuckOff delay timer
TQPF_Timer DoBTestSuckOffDelay;                                                     //Ifor 20200622 add:Rear 版 Index Pick Shuttle Err Need Purge
//<== Eastsun 20260511 F007 整合
bool DoBTestSuckTestIC()
{
    static int iTestCount=0, iReadTIntervel=0, iReadCount=0, iXMLRetryCnt=0;
    static int iDoubleCount=0;                                                  //ChungHung 20140709 add for SPIL
    static int iArm2ReadtorquCount=0;                                           //kevin 20210902 add 扭力讀取
    static bool ShowTestStatus=false;                                           //ChungHung 20150526 add for QualComm US
    static bool bFirstIn=true;                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
    static DWORD dwNowTickCount, dwOldTickCount=0;
    static DWORD dwStartAfterTestCount=0;                                       //ChungHung 20140730 add for ATK function after tested delay time

    int &Task=iBTestSuckTestICTask, ret, iMaxDoubleContact, ret2;
    int iMaxPreasure=0, iErrCnt=0, iTestCh=0;
    bool bHasFailIC=false;
    bool bManualStep=false, bManualTStart=false;                                //ChungHung 20150526 add for QualComm US
    bool bATCSiteTest[32];
    bool bAlreadyTested;
    DWORD dwEndAfterTestCount=0;                                                //ChungHung 20140730 add for ATK function after tested delay time
    AnsiString ErrPart="", ErrCnt="";                                           //kevin 20130418
    AnsiString SData="@e02019Arm2,sideA";                                       //kevin 20191029 add loadcell reaad
    int iDbContactHigh=1000;                                                    //RogerYang 20260126 : JSCC_OS 第二次contact要拉高慢放
    int iDbContactHighSlowSpd;
    //==> Eastsun 20260511 F007 整合: T8 case 8000~9101 所需區域變數
    bool bSuck_OK=false;                                                            //Ifor 20200622 add:T8 case 8001 真空判定結果
    bool flag2=false;                                                               //T8 case 8003 內 vac initial 旗標
    int iIndexUpPos=0;                                                              //T8 case 8002 Above Socket Z 高度
    static AnsiString AllErrPart="";                                                //T8 case 8003/8010 累計錯位字串
    //<== Eastsun 20260511 F007 整合

    switch(Task)
    {
        case 1:
            iXMLRetryCnt=0;                                                     //Steven 20201102 : For Murata資料上拋
            iArm2ReadtorquCount=0;                                              //kevin 20210902 add 扭力讀取

            if(CUSTOMER_CODE==CC_KYEC_XILINX &&
               IniConfig.bD01EnableReadTorque)                                  //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
            {
                bReadRearTestArmTorque=true;
            }
            else
            {
                if(IniConfig.bD01EnableReadTorque &&
                   IniConfig.iD01ReadTorqueTimeCount>0)                         //Steven 20100617 Start: Add form 9080A for 即時更新扭力值
                {
                    iReadTIntervel++;
                    if(iReadTIntervel>=IniConfig.iD01ReadTorqueTimeCount)       //kevin 20130611   if(iReadTIntervel>LastSet.iReadTorqueTimeCount)
                    {
                        iReadTIntervel=0;
                        bReadRearTestArmTorque=true;
                    }
                }
            }

            if(CosFunction.bSocketSensorCheckICAtArmDown==true)
            {
                CheckICExistInSocket(__FUNC__);
            }
            #ifdef SOFT_SIMULTE
            bReadRearTestArmTorque=false;
            #endif
            if(IniConfig.bD02OffReadTorqueDuringTest==true)
                bReadRearTestArmTorque=false;

            if(CosFunction.bHiSiliconFunction==true ||                          //Ifor 20190912 :add 海思 V02.30 版 Record Torque
               CUSTOMER_CODE==CC_KYEC_LEE)
            {
                bRearTestArmTorqueFinish=false;
                bFrontTestArmTorqueFinish=true;
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=true;
                fMain->edTorue1->Text="";
            }
            else
            {
                if(bReadRearTestArmTorque)
                {
                    bReadRearTestArmTorque=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                    Task=5000;
                    break;
                }
                else
                {
                    Task=100;
                }
            }
        case 100:                                                                                                       //kevin 20130611 fix
            bAlreadyTested=false;                                                                                       //Steven 20170517 (jou) : 新增保護,如果IC已經測過就離開
            ErrPart="";
            iTestCh=0;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]!=NULL_IC     &&
                       BTestSuck.Item[i][j]!=HAS_NULL_IC)
                    {
                        if(BTestSuck.Item[i][j]>=TEST_PASS)
                            bAlreadyTested=true;

                        if((CosFunction.bBarcodeErrNoTestAndShowH==true ||                                              //jou 20191007 : Barcode Error No Test & Show "H"
                            TestIF_File.iNoCodeDeviceToErr==2) &&                                                       //Steven 20200909 : 將2DID all site fail變成選項
                           TestIF_File.bEnableBarCode==true &&
                           (BTestSuck.cDeviceInf[i][j]==asBarCodeErrorSend ||
                            BTestSuck.cDeviceInf[i][j]==""))
                        {
                            ;
                        }
                        else
                        {
                            iTestCh++;                                                                                  //Steven 20250318 : 先統計有幾個site要測試
                        }
                    }
                }
            }

            if(bResetModeAndCleanOut && IniConfig.bI49_TesterTimeOutResetAllIC)                                         //Sam 20250916 : [I49] Restet 已測 IC 不需要報警
                bAlreadyTested=false;                                           //Steven 20260612 : Fix == to = (was comparison, not assignment)

            if(bAlreadyTested==true)
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        ErrPart+=BTestSuck.Item[i][j]+", ";
                        if(BTestSuck.Item[i][j]!=NULL_IC     &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                           BTestSuck.Item[i][j]<TEST_PASS)
                        {
                            BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                        }
                    }
                }
                ShowMyMessage("IC in index already tested, abort process.", ErrPart);
                return true;
            }

            if(iTestCh==0)                                                                                              //Steven 20250318 : 如果都沒要測試就return true;
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]!=NULL_IC &&
                           BTestSuck.Item[i][j]<TEST_PASS &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            if((CosFunction.bBarcodeErrNoTestAndShowH==true ||                                          //jou 20191007 : Barcode Error No Test & Show "H"
                                TestIF_File.iNoCodeDeviceToErr==2) &&                                                   //Steven 20200909 : 將2DID all site fail變成選項
                               TestIF_File.bEnableBarCode==true &&
                               (BTestSuck.cDeviceInf[i][j]==asBarCodeErrorSend ||
                                BTestSuck.cDeviceInf[i][j]==""))
                            {
                                BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                BTestSuck.iBinData[i][j]=iTestBinCount;
                                BTestSuck.PordRec[i][j].AddTestResultRecord(iTestBinCount, BTestSuck.cSBin[i][j], "NonTestToRBin");
                            }
                        }
                    }
                }
                return true;
            }

            Task=200;
        case 200:
            TestSocket.CopyFrom(BTestSuck);
            bDoubleContact=false;
            InitTestTask();
            ShowTestStatus=false;                                               //ChungHung 20150526 add for QualComm US
            iTestCount=0;
            ZeroMemory(iReContactCnt, sizeof(iReContactCnt));                   //Steven 20231205 : 計算某site contact 次數

            if(CUSTOMER_CODE==CC_MTI ||
               CUSTOMER_CODE==CC_PTI)                                           //Sam 20190429 : Add CC_PTI_NEWWORK
            {
                InitDoStartMode();                                              //ChungHung 聚成 Init Test Start
            }

            Task=2200;
//            break;                                                            //Steven 20150724 : Add index speed
        case 2200:
            if(CUSTOMER_CODE==CC_MTI ||                                         //ChungHung 聚成 Test Start
               CUSTOMER_CODE==CC_PTI)                                           //Sam 20190429 : Add CC_PTI_NEWWORK
            {
                if(TestIF_File.iShuttleMode==0 ||                               //Sam 20200214 : 修正關 Arm 不需要送通訊
                   (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                {
                    if(DoStartMode(fAutomation->TestMode)==false)
                        return false;
                }
            }

            if(IniConfig.bD37EnableManualProcess)                               //ChungHung 20150526 add for QualComm US
            {
                bHangTimePause=true;
                if(CosFunction.bEnableSoftWareControlButton)                    //ChungHung 20150609 add only for TSMC
                {
                    fMain->BtnT_Start->Enabled=true;
                }

                if(WaitManualStartKey()==false)
                    return false;
                bHangTimePause=false;
                if(CosFunction.bEnableSoftWareControlButton)                    //ChungHung 20150609 add only for TSMC
                {
                    fMain->BtnT_Start->Enabled=false;
                    fMain->BtnT_Start->Color=(TColor)0x00804000;                //ChungHung 20150609 add only for TSMC
                }
            }

            ProcessStartTestData(1);                                            //畫面顯示黃色，測試中
            SetTestTimeOutTimer(1);                                             //Steven 20200407 : 整合Time Out時間設定
            HangTime.SetSecAndOn(Prod.iHangupMaxTime);

            if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)                  //Sam 20190429 : Add CC_PTI_NEWWORK
            {
                InitDoEndMode();                                                //ChungHung 聚成 only
            }

//            ShowIndexTime(1);                                                 //Steven 20140619 : 測試 //到這裡大概0.00~0.02Sec
            if(IniConfig.bL10IndexTestlogTemp &&
               (TestIF_File.iShuttleMode==0 ||
                (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)))  //kevin 20190323 : index 測試時才記錄溫度
            {
                TemperatureStorageLog(1);                                       //kevin 20190323 add Steven 20140617 : for 海思
            }

            if(IniConfig.bD67LoadCellMeasure)
            {
                SData="@e02019Arm2,sideA";
                RespondASECom(SData);                                           //kevin 20191029 add
                bloadcellRece=false;
                sLoadCellReceData="";
                iloadcellRece=0;                                                //kevin 20190906
            }
            Task=2400;
        case 2400:                                                                                                                                              // 編號不得改變 *******************
            if(dwOldTickCount==0 || bNeedInitialTestDelay)                                                                                                      //ChungHung 20140619 add TSMC Function 不算在TimeOut time 內
            {
                dwOldTickCount=MyTickCount();
                dwNowTickCount=dwOldTickCount;
                SetTestTimeOutTimer(1);                                                                                                                         //Steven 20200407 : 整合Time Out時間設定
                HangTime.SetSecAndOn(Prod.iHangupMaxTime);
            }
            else
            {
                dwNowTickCount=MyTickCount();
                if(dwNowTickCount<dwOldTickCount)
                {
                    dwOldTickCount=dwNowTickCount;
                    SetTestTimeOutTimer(1);                                                                                                                     //Steven 20200407 : 整合Time Out時間設定
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);                                                                                                  //Jou 20101018 Start : 重新設定Time Out時間時,Hang Up時間也要重設
                }
                else
                {
                    if(bHandlerPause ||                                                                                                                         //jou 20170526 (wei) 修正長時間alarm未處理,測試當下會發生tester time out
                       bTesterSendPause)                                                                                                                        //kevin 20161101 check problem time out reset
                    {
                        if(CosFunction.bStopMustTestTimeOut==false ||
                           bEnterTestIF==true ||
                           bTesterSendPause)                                                                                                                    //ChungHung 20121221 add
                        {
                            bEnterTestIF=false;
                            SetTestTimeOutTimer(1);                                                                                                             //Steven 20200407 : 整合Time Out時間設定
                        }

                        HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    }
                    dwOldTickCount=dwNowTickCount;
                }
            }

            if(bDoOverDrive || bDoReContact)                                                                                                                    //Steven 20151207 : OverDrive for TSMC  //Steven 20151207 : Recontact for TSMC
            {
                iDoubleCount=0;                                                                                                                                 //jou 20230823 : 修正GPIB ReContact 命令hang up異常
                Task=6000;
                return false;
            }

            if(IniConfig.bD23EveryDeviceDoubleContactFirstNoTesting)                                                                                            //ChungHung 20140709 add for SPIL  //ChungHung 20140709 add for Spil 第一次Contact 不測試 第二次才測
            {
                iDoubleCount++;
                if(iDoubleCount<IniConfig.iD23_MultiContactCount)                                                                                               //Steven 20151001 : Add for TSMC
                {
                    Task=3000;
                    return false;
                }
            }

            if((TestIF_File.bOutShtLoseICSetErrUntilOneCycle==true ||
                TestIF_File.bIndexDropICSetErrUntilOneCycle) &&                                                                                                 //JerryYang 20220923 : add index arm drop error設ERROR BIN
               bFirstIn==true)                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
            {
                bFirstIn=false;
                if(bOutShtLoseICNeedSetErrBin==true ||
                   bIndexDropICNeedSetErrBin==true)                                                                                                             //JerryYang 20220923 : add index arm drop error設ERROR BIN
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(bTestSiteNeedSetErrBin[i][j]==true &&
                               TestSocket.Item[i][j]!=NULL_IC     &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC &&
                               TestSocket.Item[i][j]<TEST_PASS      )
                            {
                                TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                iErrCnt++;
                            }
                        }
                    }

                    if(iErrCnt>0)
                    {
                        if(bOutShtLoseICNeedSetErrBin)
                        {
                            ErrCnt.sprintf("Out Shuttle lose IC, Arm 2 set to error Bin (before test) : %d pcs",iErrCnt);
                            MyDBIProcess("Message", ErrCnt);
                        }
                        else
                        {
                            ErrCnt.sprintf("Index Arm Drop error, Arm 2 set to error Bin (before test) : %d pcs",iErrCnt);
                            MyDBIProcess("Message", ErrCnt);
                        }
                    }
                }
            }
            bFinshTest=false;

            if(IniConfig.bI27_ManualSortMode &&                                                                                                                 //Steven 20150915 : For TSMC 手動整盤功能
               LastSet.iTester==OFF_LINE &&
               bRunManualSortMode==true)                                                                                                                        //ChungHung 20140730 add ContinuousFailHaveOneCycle add bIndexArmNoTestting
            {
                ret=1;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC     &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC)
                        {
                            iTesterBIN[i][j]=TestSocket.iWhichAuto[i][j];
                            TestSocket.iBinData[i][j]=iTesterBIN[i][j];
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j]);                                             //Frank 20160505 add
                        }
                    }
                }
            }
            else if(bIndexArmNoTestting ||
                    bResetMode ||                                                                                                                               //jou 981219 start : reset mode
                    bResetModeAndCleanOut ||                                                                                                                    //JerryYang 20151007 : ResetMode可選OneCycle或CleanOut
                    //bResetModeAndCleanOutAndNoTest ||                         //Jimmychiu 20210927 Reset clean out with no tester
                    (bOneTimeHotPlateCheckAll &&
                     IniConfig.bE39_1PutTheDevicesToErrorBin &&
                     bHotPlateCheckNeedTrayFeed))                                                                                                               //Steven 20120315 : CleanOut後的HotPlate Check, 要放到Error Bin
            {
                ret=1;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC     &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC &&
                           TestSocket.Item[i][j]<TEST_PASS)
                        {
                            if(bDoEmptySocketOneCycle)                                                                                                          //Steven 20220817 : Bin of ESC function iTestBinCount --> IniConfig.iI41_BinOfESC
                            {
                                iTesterBIN[i][j]=IniConfig.iI41_BinOfESC;
                            }
                            else
                            {
                                iTesterBIN[i][j]=iTestBinCount;                                                                                                 //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                            }
                            TestSocket.iBinData[i][j]=iTesterBIN[i][j];                                                                                         //Steven 20220830 : Add for ESC function
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "RESET_Mode");                               //Frank 20160505 add
                        }
                    }
                }
            }
            else
            {
                if(DeviceForm.DummyMode)                                                                                                                        //contect form
                {
                    if(iTestCount<1)
                    {
                        ret=1;
                    }
                    else
                    {
                        ret=ProcessTestResult(1);
                    }
                }
                else
                {
                    ret=ProcessTestResult(1);
                }
            }

            if(TestSocket.HasRealIC()==false)                                                                                                                   //Steven 20151016 : Fixed for Hang up while no devices in socket.
            {
                ret=1;
            }

            iWhichIndexArm=2;                                                                                                                                   //Sam 20231214 : Temp offset use ready temp range
            if(ret==1)
            {
                if(IniConfig.bD23EveryDeviceDoubleContactFirstNoTesting)
                    iDoubleCount=0;                                                                                                                             //ChungHung 20140709 add for SPIL
                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin

                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)                                                                                  //Ifor 20220317 add:關Arm不累積Contact次數)
                {
                }
                else
                {
                    if(IniConfig.bL28TempOfsUseReadyTempRange &&
                       bEnable_KLT_Function==false)                                                                                                             //Sam 20231214 : Temp offset use ready temp range
                    {
                        if(TestSocket.HasRealIC())
                        {
                            iInitContactCount++;
                            if(iInitContactCount<(Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset))
                            {
                                fHeaterOK=false;
                                bNeedReCheckHeat=true;
                            }
                        }
                    }
                    else
                    {
                        iInitContactCount++;                                                                                                                    //Steven 20141117 : 起測時溫度要補Offset
                    }
                }

                bTestingStopAllMotor=false;                                                                                                                     //jou 2013-09-25 Testing Need Stop All Motor
                bTestDuplicateErr=false;                                                                                                                        //Steven 20120102 : 測試ok要重置Flag
                bInitialMaxTime=false;                                                                                                                          //jou 2011-11-09 增加initial max time set
                SetHangupMaxTime();                                                                                                                             //Wei 20230224 : 重置死機時間
                if(iInitContactCount>Temperature.iCintactCntForTempOffsetAtInitial)                                                                             //kevin 20160312
                {
                    bTestOverTimeTempOffsetF=false;
                }

                if(iInitContactCount==Temperature.iCintactCntForTempOffsetAtInitial)                                                                            //Ifor 20190605 :add ATC Init Offset 設定
                {
                    fLotInfo->SetATCOffset(true);                                                                                                               //Ifor 20190605 :add ATC Init Offset 設定
                    iInitContactCount++;
                }

                if(bloadcellRece)                                                                                                                               //kevin 20191029 add load cell read
                {
                    if(iloadcellRece==2)                                                                                                                        //kevin 20190906 add load cell command
                    {
                                                                                //kevin 20190906 add load cell NG
                        ShowErrorMessage("WAR0309", K_SKIP, MMSystem, false, sLoadCellReceData);
                        iloadcellRece=0;
                    }
                }

                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }

                if(CUSTOMER_CODE==CC_MTI ||
                   CUSTOMER_CODE==CC_PTI)                                                                                                                       //Sam 20190429 : Add CC_PTI_NEWWORK
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==1))                                                                                                           //Sam 20200214 : 修正關 Arm 不需要送通訊
                    {
                        if(DoEndMode(fAutomation->TestMode)==false)                                                                                             //ChungHung 聚成 Test End
                            return false;
                    }
                }

                if(IniConfig.bEnableSocketCommunication)                                                                                                        //ChungHung 20130112 add for ASE_KR Socket Tester
                {
                    fSocketCommunication->bSendBinMapReport=true;
                }

                SW[SwTesterPower].Off();
                bHasFailIC=false;

                if(IniConfig.bD37EnableManualProcess)                                                                                                           //ChungHung 20150526 add for QualComm US
                {
                    bHangTimePause=true;
                    if(ShowTestStatus==false)
                    {
                        ShowTestStatus=true;
                        ProcessShowTestStatus(1);
                    }

                    if(CosFunction.bEnableSoftWareControlButton)                                                                                                //ChungHung 20150609 add only for TSMC
                    {
                        fMain->BtnT_Start->Enabled=true;
                        fMain->BtnSTEP->Enabled=true;
                        fMain->BtnZUpDown->Enabled=true;
                    }
                    bManualStep=WaitManualStepKey();
                    if(bManualStep==false)
                        bManualTStart=WaitManualStartKey();
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    if(bManualTStart)
                    {
                        SetTestTimeOutTimer(1);                                                                                                                 //Steven 20200407 : 整合Time Out時間設定

                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
                                    TestSocket.SetItemData(i, j, HAS_IC);
                            }
                        }

                        if(CosFunction.bEnableSoftWareControlButton)                                                                                            //ChungHung 20150609 add only for TSMC
                        {
                            if(fMain->BtnZUpDown->Down)
                            {
                                ShowTestStatus=false;                                                                                                           //ChungHung 20150526 add for QualComm US
                                Task=3000;
                            }
                            else
                            {
                                InitTestTask();
                                ProcessStartTestData(0);                                                                                                        //畫面顯示黃色，測試中
                                ShowTestStatus=false;                                                                                                           //ChungHung 20150526 add for QualComm US
                                Task=2400;
                            }
                            fMain->BtnT_Start->Enabled=false;
                            fMain->BtnSTEP->Enabled=false;
                            fMain->BtnZUpDown->Enabled=false;
                            fMain->BtnSTEP->Color=(TColor)0x00804000;
                            fMain->BtnT_Start->Color=(TColor)0x00804000;
                        }
                        else
                        {
                            InitTestTask();
                            ShowTestStatus=false;                                                                                                               //ChungHung 20150526 add for QualComm US
                            Task=2400;
                        }
                        bHangTimePause=false;
                        return false;
                    }

                    if(bManualStep==false && bManualTStart==false)
                        return false;
                    bHangTimePause=false;
                    if(CosFunction.bEnableSoftWareControlButton)                                                                                                //ChungHung 20150609 add only for TSMC
                    {
                        fMain->BtnT_Start->Enabled=false;
                        fMain->BtnSTEP->Enabled=false;
                        fMain->BtnZUpDown->Enabled=false;
                        fMain->BtnSTEP->Color=(TColor)0x00804000;
                        fMain->BtnT_Start->Color=(TColor)0x00804000;
                        fMain->BtnZUpDown->Down=false;
                    }
                }

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(IniConfig.bD22_4_PassBinCanDoubleContact)                                                                                            //JerryYang 20230909 : pass bin也可以設定Double contact
                        {
                            if(TestSocket.Item[i][j]!=NULL_IC &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC &&
                               TestSocket.bNeedReTest[i][j])
                            {
                                bHasFailIC=true;
                            }
                        }
                        else
                        {
                            if(TestSocket.Item[i][j]!=NULL_IC &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC &&
                               TestSocket.bPass[i][j]==false  &&
                               TestSocket.bNeedReTest[i][j])
                            {
                                bHasFailIC=true;
                            }
                        }
                    }
                }

                if(bHasFailIC || DeviceForm.DummyMode ||
                   bNeedReplunge_RFMD)                                                                                                                          //Steven 20201022 : For RFMD
                {
                    iTestCount++;
                    iMaxDoubleContact=2;
                    if(Prod.bD22SupportMultiDoubleContact)
                      // IniConfig.bD22VerifyMode)                              //Sam 20231117 : 整合到 QA 模式 //Sam 20221012 : 新增 VerifyMode 功能
                    {
                        if(CUSTOMER_CODE==CC_TERAPOWER && iTestCount==1)                                                                                        //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    TestSocket.iBinDataBackUp[i][j]=TestSocket.iBinData[i][j];
                                }
                            }
                            ProcessCount(1);
                        }

                        iMaxDoubleContact=Prod.iD22DoubleContactCount+2;                                                                                        //Sam 20231117 : 整合到 QA 模式
                        if(iMaxDoubleContact<2)
                            iMaxDoubleContact=2;
                        if(iMaxDoubleContact>10)
                            iMaxDoubleContact=10;
                    }

                    if(bNeedReplunge_RFMD)                                                                                                                      //Steven 20201022 : For RFMD
                    {
                        bNeedReplunge_RFMD=false;
                        ProcessShowTestStatus(1);
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(TestSocket.Item[i][j]!=NULL_IC     &&
                                   TestSocket.Item[i][j]!=HAS_NULL_IC)
                                    TestSocket.SetItemData(i, j, HAS_IC);
                            }
                        }
                        bDoubleContact=true;
                        Task=3000;
                        break;
                    }
                    else if(iTestCount<iMaxDoubleContact)
                    {
                        ProcessShowTestStatus(1);
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(IniConfig.bD22_4_PassBinCanDoubleContact)                                                                                    //JerryYang 20230909 : pass bin也可以設定Double contact
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       TestSocket.bNeedReTest[i][j])
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);
                                    }
                                }
                                else
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       TestSocket.bPass[i][j]==false  &&
                                       TestSocket.bNeedReTest[i][j])
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);
                                    }
                                }
                            }
                        }
                        bDoubleContact=true;
                        Task=3000;
                        break;
                    }
                }

                if(TestSocket.HasRealIC())                                                                                                                      //Steven 20210218 : 修正測試時間的紀錄
                    ret2=RecordEndTestTime(1);                                                                                                                  //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常。0:arm1 1:arm2 2:雙Arm
                else
                    ret2=1;

                if(DeviceForm.ContactMode==DropContact)
                    DropContactTimer1.LatchCycleTime(true);                                                                                                     //JerryYang 20170425 (wei) 第一段時間, 測試完成到另一支arm下降到drop高度

                bATC_SITE_2ND_CHECK[1]=false;
                if(ATC_SYSTEM==eNewATCSystem)                                                                                                                   //Ifor 20160509 add ATC 測試時開啟第二點溫度監控
                {
                    for(int i=0; i<iATC_Use_Heat_Count; i++)                                                                                                    //Ifor 20160516 修改ATC Heat 設定數
                        bATCSiteTest[i]=false;
                    ATC_InterfaceForm->SiteTesting(iATC_Use_Heat_Count, bATCSiteTest);
                }
                else if(ATC_SYSTEM==eATCHonPrecType)
                {
                    ATCInterfaceForm->SendTestEnd(1);
                }

                SetNoiseDelay=false;
                TestISTimeOut=false;

                if(CUSTOMER_CODE==CC_TERAPOWER &&
                   Prod.bD22SupportMultiDoubleContact)                                                                                                          //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
                {
                    if(iTestCount==0)
                    {
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                TestSocket.iBinDataBackUp[i][j]=TestSocket.iBinData[i][j];
                            }
                        }
                        ProcessCount(1);
                    }
                }
                else
                {
                    ProcessCount(1);
                }

                RecordPiggyBackStartEnd(true);                                                                                                                  //jou 2011-12-26 此funtion要在ProcessCount之後

                #ifdef TEST_BIN_MISS_SIMULATE
                if(MakeNoise==true)
                {
                    if(TestSocket.Item[0][0]==(TEST_PASS+1))
                        TestSocket.SetItemData(0, 0, (TEST_PASS+2));
                    else
                        TestSocket.SetItemData(0, 0, (TEST_PASS+1));
                    MakeNoise=false;
                }
                #endif

                if(TestSocket.HasRealIC())                                                                                                                      //Steven 20210218 : 修正測試時間的紀錄
                {
                    RecordHistroy(1);
                    BTestSuck.MoveAllItem(TestSocket);
                }
                bP65QAReTest=false;                                                                                                                             //Ifor 20260407 add: [P65] QA ReTest done, clear flag
                bFinshTest=true;
                bInitStartDelayNotFinish=true;                                                                                                                  //Ifor 20181220 : add Init Start Delay Time Not Finish
                bTJControlMode=false;                                                                                                                           //Ifor 20190328 : add TJ Temp Over Range
                ATC_InterfaceForm->SendHandler2DID(0, false);
                if(IniConfig.bTesterTimeUpErrorNeedPassword==true &&
                   CUSTOMER_CODE==CC_LINGSEN &&                                                                                                                 //jou 2012-08-28 菱生要求 Test Time Up Error 之後的兩次測試, 不管結果如何都排到R
                   iTestTimeUpErrContinueR>0)
                {
                    iTestTimeUpErrContinueR--;
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]!=NULL_IC &&
                               BTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            }
                        }
                    }

                    MyDBIProcess("Message", "Arm 2 Tester Time Up error place to R Bin : "+AnsiString(BTestSuck.CountRealIC())+" pcs" );
                }
                else if(TestIF_File.bOutShtLoseICSetErrUntilOneCycle==true ||                                                                                   //JerryYang 20170610 (wei) JSCC要求Out shuttle lose IC需自動one cycle,並將對應的site設為Error bin
                        TestIF_File.bIndexDropICSetErrUntilOneCycle)                                                                                            //JerryYang 20220923 : add index arm drop error設ERROR BIN
                {
                    if(bOutShtLoseICNeedSetErrBin==true ||
                       bIndexDropICNeedSetErrBin==true)                                                                                                         //JerryYang 20220923 : add index arm drop error設ERROR BIN
                    {
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(bTestSiteNeedSetErrBin[i][j]==true &&
                                   BTestSuck.Item[i][j]!=NULL_IC     &&
                                   BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   BTestSuck.Item[i][j]!=TEST_PASS+iTestBinCount)
                                {
                                    BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                    iErrCnt++;
                                }
                            }
                        }

                        if(iErrCnt>0)
                        {
                            if(bOutShtLoseICNeedSetErrBin)
                            {
                                ErrCnt.sprintf("Out Shuttle lose IC, Arm 2 set to error Bin(After test) : %d pcs",iErrCnt);
                                MyDBIProcess("Message", ErrCnt);
                            }
                            else
                            {
                                ErrCnt.sprintf("Index Arm Drop error, Arm 2 set to error Bin(After test) : %d pcs",iErrCnt);
                                MyDBIProcess("Message", ErrCnt);
                            }
                        }
                    }
                }

                if(ret2!=1)                                                                                                                                     //Steven 20201102 : For Murata資料上拋
                {
                    iXMLRetryCnt++;
                    DoBTestSuckTestICDelay.SetSecAndOn(60);
                    Task=7000;
                }
                else
                {
                    iXMLRetryCnt=0;                                                                                                                             //Steven 20201102 : For Murata資料上拋
                    if(CheckContactOver())
                    {
                        iDoInterFaceErrorStepTask=1;
                        bContactCTOverCHK=true;
                        Task=2500;
                    }
                    else
                    {
                        if(bEchoStop==true)                                                                                                                     //ChungHung 20130326 add
                        {
                            Task=2500;
                            iDoInterFaceErrorStepTask=1;
                            return false;
                        }

                        iDoubleCount=0;                                                                                                                         //ChungHung 20140709 add for SPIL
                        //Ifor 20260407 add: [P65] ARM QA Mode ReTest - Arm2
                        //==>
                        if(IniConfig.bP65EnableArmQAMode && IniConfig.iP65ArmQAModeValue > 0 && iP65QAReTestCount < IniConfig.iP65ArmQAModeValue)
                        {
                            bool bAllBin1_P65=true;
                            for(int qi=0; qi<BTestSuck.iShtRow; qi++)
                            {
                                for(int qj=0; qj<BTestSuck.iShtCol; qj++)
                                {
                                    if(BTestSuck.Item[qi][qj]!=NULL_IC && BTestSuck.Item[qi][qj]!=HAS_NULL_IC)
                                    {
                                        if(BTestSuck.bPass[qi][qj]==false)
                                            bAllBin1_P65=false;
                                    }
                                }
                            }

                            if(bAllBin1_P65 && BTestSuck.HasRealIC())
                            {
                                RecordProcess("P65 QA ReTest Arm2: count="+AnsiString(iP65QAReTestCount+1)+"/"+AnsiString(IniConfig.iP65ArmQAModeValue));
                                for(int qi=0; qi<BTestSuck.iShtRow; qi++)
                                {
                                    for(int qj=0; qj<BTestSuck.iShtCol; qj++)
                                    {
                                        if(BTestSuck.Item[qi][qj]!=NULL_IC && BTestSuck.Item[qi][qj]!=HAS_NULL_IC && BTestSuck.bPass[qi][qj])
                                            TestSocket.SetItemData(qi, qj, HAS_IC);
                                    }
                                }
                                iP65QAReTestCount++;
                                bP65QAReTest=true;                              //Ifor 20260407 add: [P65] QA ReTest, notify RunTestProgram to send iLotStatus=2
                                Task=3200;
                                break;
                            }
                        }
                        //<==
                        //Ifor 20260407 add: [P65] ARM QA Mode ReTest - Arm2
                        if(CosFunction.bEnableAfterTestedDelay)                                                                                                 //ChungHung 20140730 add for ATK function after tested delay time
                        {
                            iAfterTestedCount=Prod.dAfterTestedDelay;
                            dwStartAfterTestCount=MyTickCount();
                            Task=2410;
                        }
                        //==> Eastsun 20260511 F007 整合: Ifor 20200622+20240430 KYEC 包覆 D79 Purge + SECS Yiel + ATC cooling 三段
                        else if(CUSTOMER_CODE==CC_KYEC_LEE)
                        {
                            if(CosFunction.bUseIndexPickShuttleErrNeedPurge==true     &&
                               IniConfig.bD79EnableIndexPickShuttleErrNeedPurge==true &&
                               bBTestSuckHasError==true                              ) //Ifor 20200622 add
                            {
                                Task=8000;
                                bBTestSuckHasError=false;
                            }
                            else if(iSECSGEM_ConsecutiveFailureAlarm==1)             //Ifor 20240430 add:secs gem cmd Index Yiel Fail
                            {
                                Task=9000;
                            }
                            else if(Temperature.bATCActiveCooling==true && Temperature.bTestCompleteWaitTemp==true)
                            {
                                Task=9100;
                            }
                            else
                            {
                                return true;
                            }
                        }
                        //<== Eastsun 20260511 F007 整合
                        else
                        {
                            return true;
                        }
                    }
                }
            }
            else if(ret==2 && NowNoteIsShow()==false)                                                                                                           //interface error  //ChungHung 20130110 add 防止訊息重複進入
            {
                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }

                if(IniConfig.bEnableSocketCommunication)                                                                                                        //ChungHung 20130112 add for ASE_KR Socket Tester
                {
                    fSocketCommunication->bSendBinMapReport=true;
                }
                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將Out shuttle lose IC對應未測的site設為error bin
                RecordPiggyBackStartEnd(true);
                bInitialMaxTime=false;                                                                                                                          //jou 2011-11-09 增加initial max time set
                SetHangupMaxTime();                                                                                                                             //Wei 20230224 : 重置死機時間
                SetNoiseDelay=false;
                Task=2200;
            }
            else if((LastSet.iTester==ON_LINE &&                                                                                                                //Steven 20150713 : 整理LastSet.iTester
                     hBTestTimeOutDelay.Off() &&                                                                                                                //Jou 20101018  //Time Out
                     NowNoteIsShow()==false) ||                                                                                                                 //ChungHung 20130110 add 防止訊息重複進入
                    (bEcho && bTimeOutForNoFullSite==true))                                                                                                     //Steven 20141016 : FullSite的Test Time Out
            {
                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }

                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將Out shuttle lose IC對應未測的site設為error bin
                RecordPiggyBackStartEnd(true);
                if(IniConfig.bD52InterFaceErrHeadNeedUp)
                {
                    if(CosFunction.bStopMustTestTimeOut &&                                                                                                      //Steven 20200330 : 暫停也要Time out跟 [D52]功能衝突
                       (SoftStop || SystemStart==false))
                    {
                    }
                    else
                    {
                        iDoInterFaceErrorStepTask=1;
                        Task=2500;                                                                                                                              //不跑下面流程
                        bD52IndexArmUp=true;                                                                                                                    //JerryYang 20200812 : fix RTC error hang up
                        break;
                    }
                }

                if((LastSet.iTester==ON_LINE &&                                                                                                                 //Steven 20150713 : 整理LastSet.iTester
                    hBTestTimeOutDelay.Off()) ||                                                                                                                //Jou 20101018
                   (bEcho && bTimeOutForNoFullSite==true))                                                                                                      //Steven 20141016 : FullSite的Test Time Out
                {
                    if(fNote->fShow)                                                                                                                            //JerryYang 20200408 : 修正Alarm畫面佔住時,不會發出Test time out的問題
                        break;
                    bEcho=false;                                                                                                                                //Steven 20150306 : Fixed for FullSite的Test Time Out
                    bTimeOutForNoFullSite=false;

                    ret=ProcessTesterTimeOut(1);

                    if(ret==2)
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
                           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                                                                                 //Steven 20131101 : Add ASE-K12
                        {
                            InitTestTask();
                        }

                        SetNoiseDelay=false;
                        TestISTimeOut=true;
                        if(TestIF.iTestType==GPIB_MODE)
                        {
                            if(IniConfig.bRetryNoNeedRestartGpib)                                                                                               //Steven 20111220 : 測試TimeOut Retry時,不需要重開GPIB
                            {                                                                                                                                   //連接OT BOX不需要重開GPIB
                                if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)                                                                            //Steven 20181121 : fixed time out後會發生重測
                                    InitTestTask();
                                Task=2200;
                            }
                            else
                            {
                                Task=2600;
                            }
                        }
                        else if(TestIF_File.iTestType==TTL_MODE &&
                                (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                                                                                         //Isaac 20210309 :TTL RS232兩塊板子
                        {
                            if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)                                                                                //resend SOT
                            {
                                InitTestTask();
                                Task=2200;
                            }
                            else
                            {
                                Task=2600;
                            }
                        }
                        else
                        {
                            Task=2200;
                        }

                        bTimeOutForNoFullSite=false;                                                                                                            //ChungHung 20141017 fix Full Site Test Time Out problem
                    }
                    else
                    {
                        if(ret==1)
                            ProcessTestResult(1);

                        SetNoiseDelay=false;
                        TestISTimeOut=false;
                        ProcessCount(1);

                        if(TestSocket.HasRealIC())                                                                                                              //Steven 20210218 : 修正測試時間的紀錄
                            RecordHistroy(1);

                        if(TestSocket.UseSiteHasIC())                                                                                                           //KenHsieh 20231208 : 修改為有資料才傳，避免重複移資料導致被覆蓋為NULL
                            BTestSuck.MoveAllItem(TestSocket);

                        bFinshTest=true;
                        bInitStartDelayNotFinish=true;                                                                                                          //Ifor 20181220 : add Init Start Delay Time Not Finish
                        bTJControlMode=false;                                                                                                                   //Ifor 20190328 : add TJ Temp Over Range
                        bTimeOutForNoFullSite=false;                                                                                                            //ChungHung 20141017 fix Full Site Test Time Out problem
                        ATC_InterfaceForm->SendHandler2DID(0, false);
                        SW[SwTesterPower].Off();
                        if(CheckContactOver())
                        {
                            iDoInterFaceErrorStepTask=1;
                            bContactCTOverCHK=true;
                            Task=2500;
                        }
                        else
                        {
                            if(CosFunction.bEnableAfterTestedDelay)                                                                                             //ChungHung 20140730 add for ATK function after tested delay time
                            {
                                iAfterTestedCount=Prod.dAfterTestedDelay;
                                dwStartAfterTestCount=MyTickCount();
                                Task=2410;
                            }
                            //==> Eastsun 20260511 F007 整合: Ifor 20200622+20240430 KYEC 包覆 D79 Purge + SECS Yiel + ATC cooling 三段
                            else if(CUSTOMER_CODE==CC_KYEC_LEE)
                            {
                                if(CosFunction.bUseIndexPickShuttleErrNeedPurge==true     &&
                                   IniConfig.bD79EnableIndexPickShuttleErrNeedPurge==true &&
                                   bBTestSuckHasError==true                              ) //Ifor 20200622 add
                                {
                                    Task=8000;
                                    bBTestSuckHasError=false;
                                }
                                else if(iSECSGEM_ConsecutiveFailureAlarm==1)             //Ifor 20240430 add:secs gem cmd Index Yiel Fail
                                {
                                    Task=9000;
                                }
                                else if(Temperature.bATCActiveCooling==true && Temperature.bTestCompleteWaitTemp==true)
                                {
                                    Task=9100;
                                }
                                else
                                {
                                    return true;
                                }
                            }
                            //<== Eastsun 20260511 F007 整合
                            else
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            break;
        case 2410:                                                              //ChungHung 20140730 add for ATK function after tested delay time
            if(iAfterTestedCount>0)
            {
                dwEndAfterTestCount=MyTickCount();
                iAfterTestedCount= Prod.dAfterTestedDelay-((dwEndAfterTestCount-dwStartAfterTestCount)/1000);
            }
            else
            {
                //==> Eastsun 20260511 F007 整合: Ifor 20200622+20240430 KYEC 包覆 D79 Purge + SECS Yiel + ATC cooling 三段 (case 2410 body)
                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    if(CosFunction.bUseIndexPickShuttleErrNeedPurge==true     &&
                       IniConfig.bD79EnableIndexPickShuttleErrNeedPurge==true &&
                       bBTestSuckHasError==true                              )  //Ifor 20200622 add
                    {
                        Task=8000;
                        bBTestSuckHasError=false;
                    }
                    else if(iSECSGEM_ConsecutiveFailureAlarm==1)             //Ifor 20240430 add:secs gem cmd Index Yiel Fail
                    {
                        Task=9000;
                    }
                    else if(Temperature.bATCActiveCooling==true && Temperature.bTestCompleteWaitTemp==true)
                    {
                        Task=9100;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
                //<== Eastsun 20260511 F007 整合
                }
            break;
        case 2500:
            if(SoftStop || SystemStart==false)                                                                          //Steven 20111004
                break;

            if(DoInterFaceErrorStep(TESTZ2UP))
            {
                HangTime.SetSecAndOn(Prod.iHangupMaxTime);                                                              //ChungHung 20140516 add  使用 LastSet.bInterFaceErrHeadNeedUp 時 會發生Hangup 訊息
                bHangTimePause=true;                                                                                    //Steven 20090827 Start: Hang Up dectector
                RecordProcess("TESTZ2UP");
                if(BTestSuck.Tested())                                                                                  //是否有測試完的IC
                {
                    return true;
                }
                else
                {
                    TestSocket.CopyFrom(BTestSuck);
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
                       CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                                             //Steven 20131101 : Add ASE-K12
                    {
                        InitTestTask();
                    }

                    SetNoiseDelay=false;
                    TestISTimeOut=true;
                    if(TestIF.iTestType==GPIB_MODE)
                    {
                        if(IniConfig.bRetryNoNeedRestartGpib)                                                           //Steven 20111220 : 測試TimeOut Retry時,不需要重開GPIB
                        {                                                                                               //連接OT BOX不需要重開GPIB
                            if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)                                        //Steven 20181121 : fixed time out後會發生重測
                                InitTestTask();
                            Task=2200;
                        }
                        else
                        {
                            Task=2600;
                        }
                    }
                    else
                    {
                        Task=2200;
                    }
                    return false;
                }
            }
            break;
        case 2600:
            if(SoftStop || SystemStart==false)
                break;

            DoBTestSuckTestICDelay.SetSecAndOn(0.3);
            Task=2700;
            break;
        case 2700:
            if(DoBTestSuckTestICDelay.Off())
            {
                Task=2200;
            }
            break;
        case 3000:
            if(CosFunction.bEnableSoftWareControlButton &&
               fMain->BtnZUpDown->Down)                                         //ChungHung 20150609 add only for TSMC
            {
                #ifdef SOFT_SIMULTE
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+10000, 1))
                {
                    DoBTestSuckTestICDelay.SetSecAndOn(0.5);
                    Task=3050;
                }
                break;
                #else
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+500, MOT[MTestZ2].GailSpeed))
                    Task=3100;
                break;
                #endif
            }
            else if(IniConfig.bD22DoubleContactNoNeedReContact)                 //Steven 20131202 : Double Contact不需要Index Arm上下動
            {
                InitTestTask();
                Task=2200;
            }
            else
            {
            #ifdef SOFT_SIMULTE
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+10000, 1))
                {
                    DoBTestSuckTestICDelay.SetSecAndOn(0.5);
                    Task=3050;
                }
                break;
            #else
                if(CUSTOMER_CODE==CC_JSCC_OS)                                                                           //RogerYang 20260126 : JSCC_OS 第二次contact要拉高慢放
                {
                    iDbContactHigh=5000;
                    if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+iDbContactHigh, MOT[MTestZ2].GailSpeed))
                        Task=3100;
                }
                else
                {
                    Task=3100;
                }
                break;
            #endif
            }
            break;
        case 3050:
            if(DoBTestSuckTestICDelay.Off())
            {
                Task=3100;
            }
            break;
        case 3100:
            if(CUSTOMER_CODE==CC_JSCC_OS)                                                                               //RogerYang 20260126 : JSCC_OS 第二次contact要拉高慢放
            {
                iDbContactHighSlowSpd=MOT[MTestZ2].GailSpeed/2;
            }
            else
            {
                iDbContactHighSlowSpd=MOT[MTestZ2].GailSpeed;
            }

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iDbContactHighSlowSpd))              //Alick 20161122 修正-Prod.TestZ2_Drop_Offset，開D23功能下壓會不到位造成O/S
            {
                if(CosFunction.bEnableSoftWareControlButton &&
                   fMain->BtnZUpDown->Down)                                                                             //ChungHung 20150609 add only for TSMC
                {
                    fMain->BtnZUpDown->Down=false;
                    ProcessStartTestData(0);                                                                            //畫面顯示黃色，測試中
                    InitTestTask();
                    Task=2400;
                }
                else
                {
                    if(bDoubleContact)                                                                                  //JerryYang 20220923 : After double contact use initial delay
                    {
                        RecordProcess("After double contact use initial delay");
                        bNeedInitialTestDelay=true;                                                                     //ChungHung 20140425 add for TSMC Device
                    }
                    InitTestTask();
                    iRTCErrorCount=0;                        //wei 20221222 RTC ARM Error
                    Task=2200;
                }
            }
            break;
        case 5000:
            if(CUSTOMER_CODE==CC_KYEC_XILINX && IniConfig.bD01EnableReadTorque &&
               IniConfig.bChangeKitNoHardStop==true &&                          //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
               IniConfig.bRemeberAutoHeight==true)
            {
                DoBTestSuckTestICDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                Task=5050;
            }
            else                                                                //Steven 20100617 Start: Add form 9080A for 即時更新扭力值
            {
                DoBTestSuckTestICDelay.SetSecAndOn(0.01);                       // for switch read torque relay
                Task=5100;
            }
            break;
        case 5050:
            if(DoBTestSuckTestICDelay.Off())
            {
                DoBTestSuckTestICDelay.SetSecAndOn(0.01);                       // for switch read torque relay
                Task=5100;
            }
            break;
        case 5100:
            if(DoBTestSuckTestICDelay.Off())
            {
                fMain->chkReadTorque1->Checked=false;                           //2008/06/24 lee
                fMain->chkReadTorque2->Checked=true;                            //2008/06/24 lee

                fMain->edTorue1->Text="";
                fMain->lbArm1Torque->Caption="2:Reading";
                iReadCount=0;
                DoBTestSuckTestICDelay.SetSecAndOn(5);                          //kevin 20210902 2->5   //2008/07/15 lee
                Task=5300;
            }
            break;
        case 5300:
            if(fMain->edTorue1->Text!="")
            {
                iReadCount++;

                if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                   IniConfig.bD01EnableReadTorque)                              //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
                {
                    ShowMainScreenPresure(1);
                    if(atoi(fMain->edTorue1->Text.c_str())>=(IniConfig.dD01ReadTorque+DeviceForm_File.dZ2Torue))
                        Task=5400;
                    else
                        Task=100;                                               //kevin 20130611
                }
                else
                {
                    if(iReadCount>1)                                            //2008/07/15 lee
                    {
                        ShowMainScreenPresure(1);

                        if(IniConfig.bControlTorque)                            //jou 2013-11-05 Index Control Torque
                            iMaxPreasure=DeviceForm.iIndexTorqueMax;
                        else
                            iMaxPreasure=Prod.iMaxPreasure;

                        if(atoi(fMain->edTorue1->Text.c_str())>=iMaxPreasure)
                            Task=5400;
                        else
                            Task=100;                                           //kevin 20130611
                    }
                    else
                    {
                        fMain->edTorue1->Text="";
                        COM2->InitReadTorueTask();
                        bReadMCU2=true;                                         //kevin 20220225 read MCU DATA
                    }
                }
            }
            else if(DoBTestSuckTestICDelay.Off())
            {
                fMain->chkReadTorque1->Checked=true;                            //2008/07/15 lee
                fMain->chkReadTorque2->Checked=false;                           //2008/07/15 lee
                Task=5350;
                iArm2ReadtorquCount++;                                          //kevin 20210902 add 扭力讀取
                DoBTestSuckTestICDelay.SetSecAndOn(5);                          //kevin 20210902 2->5
            }
            break;
        case 5350:
            if(DoBTestSuckTestICDelay.Off())
            {
                if(iArm2ReadtorquCount>3)
                {
                    ShowMainScreenPresure(1);
                    iArm2ReadtorquCount=0;
                    Task=100;                                                   //kevin 20130611
                    break;
                }
                Task=5000;
            }
            break;
        case 5400:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, 30000, "DoBTestSuckTestIC 5400"))
            {
                Task=5500;
                if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                   IniConfig.bD01EnableReadTorque)                              //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
                {
                    Task=5450;
                }
            }
            break;
        case 5450:                                                              //Frank 20170626 add Xilinx 浮動Shuttle Kit 強制開啟[D01]
            if(IndexAlarmInArmAway()==true)
            {
                ErrPart="The test head 2, contact force over error";            //kevin 20130418
                bIsContactforce=true;                                           //kevin 20130418 contact force over 需開們確認
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ2,false, ErrPart);
                fAllMotorHome=false;
                iHome=1;
                Task=1;
            }
            break;
        case 5500:
            ErrPart="The test head 2, contact force over error";                //kevin 20130418
            bIsContactforce=true;                                               //kevin 20130418 contact force over 需開們確認

            if(CosFunction.bIndexAreaOnlyCanUseSkip)                            //Steven 20141105 : Index內的所有異常都只能用Skip
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ2, false, ErrPart);
            else
                ShowErrorMessage("WAR0321", K_RETRY, MTestZ2, false, ErrPart);

            fAllMotorHome=false;
            iHome=1;
            Task=1;
            break;
        case 6000:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+1000, MOT[MTestZ2].GailSpeed))
            {
                DoBTestSuckTestICDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);
                Task=6100;
            }
            break;
        case 6100:
            if(DoBTestSuckTestICDelay.Off())
            {
                if(bDoOverDrive)                                                //Steven 20151207 : OverDrive for TSMC
                {
                    Task=6200;
                }
                else                                                            //Steven 20151207 : Recontact for TSMC
                {
                    Task=6500;
                }
            }
            break;
        case 6200:                                                              //Steven 20151207 : OverDrive for TSMC
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iOverDriveDistance, MOT[MTestZ2].GailSpeed*Prod.TestZ_Drop_Speed/100))
            {
                DoBTestSuckTestICDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);
                Task=6300;
            }
            break;
        case 6300:
            if(DoBTestSuckTestICDelay.Off())
            {
                fMain->SendMSG_CMD(MSG_CMD_OverDrive);
                bDoOverDrive=false;
                Task=2400;
            }
            break;
        case 6500:                                                              //Steven 20151207 : Recontact for TSMC
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed/100))
            {
                iDoubleCount++;
                DoBTestSuckTestICDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);
                Task=6600;
            }
            break;
        case 6600:
            if(DoBTestSuckTestICDelay.Off())
            {
                if(iDoubleCount>iReContactCount)
                {
                    fMain->SendMSG_CMD(MSG_CMD_ReContact);
                    bDoOverDrive=false;
                    bDoReContact=false;                                         //jou 20230823 : 修正GPIB ReContact 命令hang up異常
                    Task=2400;
                }
                else
                {
                    Task=6000;
                }
            }
            break;
        case 7000:                                                                                                      //Steven 20201102 : For Murata資料上拋
            if(iXMLRetryCnt>=3)
            {
                ret=ShowErrorMessage("WAR16321", K_RETRY|K_RESET, MTestZ2, false);                                      //Steven 20201113 : 上傳失敗3次要有Alarm
                if(ret==K_RESET)
                {
                    fMain->Reset("DoBTestSuckTestIC");
                    Task=7100;
                }
                else
                {
                    iXMLRetryCnt=0;
                    DoBTestSuckTestICDelay.SetSecAndOn(30);
                    Task=7000;
                }
            }
            else if(DoBTestSuckTestICDelay.Off())
            {
                ret2=SendTestResultToHttp();

                if(ret2!=1)
                {
                    iXMLRetryCnt++;
                    DoBTestSuckTestICDelay.SetSecAndOn(30);
                    Task=7000;
                }
                else
                {
                    Task=7100;
                }
            }
            break;
        case 7100:
            iXMLRetryCnt=0;
            if(CheckContactOver())
            {
                iDoInterFaceErrorStepTask=1;
                bContactCTOverCHK=true;
                Task=2500;
            }
            else
            {
                if(bEchoStop==true)                                             //ChungHung 20130326 add
                {
                    Task=2500;
                    iDoInterFaceErrorStepTask=1;
                    return false;
                }

                iDoubleCount=0;                                                 //ChungHung 20140709 add for SPIL
                if(CosFunction.bEnableAfterTestedDelay)                         //ChungHung 20140730 add for ATK function after tested delay time
                {
                    iAfterTestedCount=Prod.dAfterTestedDelay;
                    dwStartAfterTestCount=MyTickCount();
                    Task=2410;
                }
                else
                {
                    return true;
                }
            }
            break;                                                              //Steven 20201102 : For Murata資料上拋
        //==> Eastsun 20260511 F007 整合: Ifor 20200622+20240430 IndexPickShuttleErrPurge case 8000~8012 + KYEC SECS case 9000/9001 + ATC cooling case 9100/9101 (T8 Rear)
        //Steven 20201102 : For Murata資料上拋
        case 8000:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bBTestSuckError[i][j]==true)
                    {
                        fiosetview->bIndexSuck[1][i][j]=true;
                    }
                }
            }
            Task=8001;
            break;
        case 8001:
            bSuck_OK=fiosetview->ProcessIndexSuckDestroy2();
            if(bSuck_OK==true)
            {
                if((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
                   (LastSet.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false)) //Above Socket
                {
                    Task=8002;
                }
                else
                {
                    Task=8003;
                }
            }
            break;
        case 8002:  //Above Socket
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);  //Steven 20140620 : 整合為Function
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos, iSpeedFast))
            {
                Task=8003;
            }
            break;
        case 8003:  //Check IO Status
            AllErrPart="";
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bBTestSuckError[i][j]==true)
                    {
                        BTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        if(flag2==true)
                        {
                            ErrPart+=IndexSuckName[i][j];
                            AllErrPart+= ErrPart;
                        }
                    }
                }
            }
            Task=8010;
            break;
        case 8010:  //Alarm
            if(AllErrPart!="")
            {
                Task=8011;
            }
            else
            {
                for(int i=0; i<MAX_Index_Row; i++)
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                        bBTestSuckError[i][j]=false;

                if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==1)   //Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
                {
                    Task=9000;
                }
                else if(Temperature.bATCActiveCooling==true && Temperature.bTestCompleteWaitTemp==true)
                {
                    Task=9100;
                }
                else
                {
                    return true;
                }
            }
            break;
        case 8011:  //上升吹氣
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+300, MOT[MTestZ2].GailSpeed, 0))
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bBTestSuckError[i][j]==true)
                        {
                            BTestSuck.Suck[i][j].Off();
                        }
                    }
                }
                DoBTestSuckOffDelay.SetMSAndOn(ArmSpeed[IndexArm].dCTAirOn*1000);
                Task=8012;
            }
            break;
        case 8012:
            if(DoBTestSuckOffDelay.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bBTestSuckError[i][j]==true)
                        {
                            BTestSuck.Suck[i][j].Normal();
                            bBTestSuckError[i][j]=false;
                        }
                    }
                }
                ShowErrorMessage("WAR0310", K_HOME, MTestY2, false, AllErrPart);
                fAllMotorHome=false;
                iHome=1;
            }
            break;
        case 9000:
            if((TestIF_File.iShuttleMode==1) && (TestIF_File.iShuttle_Sel==0))
            {
                return true;
            }
            else
            {
                if(iSECSGEM_ConsecutiveFailureAlarm==1)                         //Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
                {
                    iSECSGEM_ConsecutiveFailureAlarm=2;
                }

                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed))
                {
                    bInArmNeedToSafePos=true;
                    Task=9001;
                }
            }
            break;
        case 9001:
            if(bInArmNeedToSafePos==false)
            {
                EventReport(SECS_EVENT.DoSecsGemIndexFail);   //Eastsun 20260515 F018 KYEC Index Fail
                ShowErrorMessage("WAR07362", K_RETRY, MTestZ2, false, "");  //Vacuum Sensor Off Error
                #ifdef  SOFT_SIMULTE
                    iSECSGEM_ConsecutiveFailureAlarm=0;
                #endif
                return true;
            }
            break;
        case 9100:
            fHeaterOK=false;
            bNeedReCheckHeat=true;
            Task=9101;
            break;
        case 9101:
            if(fHeaterOK==true)
            {
                return true;
            }
            break;
        //<== Eastsun 20260511 F007 整合 (T8)
        }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer DoUseSocketTestYRearDelay;
int iBRTCUseSocketFloatTask=1;
bool DoBRTCUseSocketFloat(bool bInitial)
{
    static bool bVerifyNG=false;

    if(bInitial==true)
    {
        iBRTCUseSocketFloatTask=1;
        return false;
    }

    int ret;
    int &Task=iBRTCUseSocketFloatTask;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, MOT[MTestZ1].GailSpeed, "DoBRTCUseSocketFloat 1"))
            {
                bVerifyNG=false;
                Task=100;
            }
            break;
        case 100:
            if(REAL_TIME_CCD==true && MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false)
            {
                ShowIndexMotorError(AnsiString("DoBRTCUseSocketFloat"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear,MOT[MTestY1].GailSpeed, "DoBRTCUseSocketFloat 100"))
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                COM2->SendCommToVision(COM2->rtCHECKNULL, true);
                COM2->SendCommToVision(COM2->rtFullTOK, true);
                DoUseSocketTestYRearDelay.SetSecAndOn(10);
                Task=150;
            }
            break;
        case 150:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
//                RecordProcess("REALTIME CCD Full OK");
//                COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
                COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                COM2->DoReleaseAndInspEnd();
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                Task=200;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                Task=170;
                break;
            }
            else if(DoUseSocketTestYRearDelay.Off())
            {
                ShowErrorMessage("WAR0337", 0, MMIndex, 0, __FUNC__);           //RTC FullT Time Out Error.
                bSendRealCCDSendStart=true;                                     //Steven 20110824 : Real time CCD - 不可以關閉CCD
                COM2->DoReleaseAndInspEnd();
                Task=100;
            }
            break;
        case 170:
            if(IndexAlarmInArmAway()==true)                                                                             //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                    //Steven 20151022 : add for MAXIM
                    bIsTestSitICFallDown=true;
                COM2->DoReleaseAndInspEnd();
                if(CosFunction.bRTCFullViewErrorOnlyRetry)                                                              //Steven 20140529 : RTCFullViewErrorOnlyRetry
                    ret=ShowErrorMessage("WAR0346", K_RETRY, MMCCD, bRTCFullViewError, ErrPart);                        //RTC FullView Socket Has Device Error!
                else
                    ret=ShowErrorMessage("WAR0346", K_RETRY|K_SKIP, MMCCD, bRTCFullViewError, ErrPart);                 //RTC FullView Socket Has Device Error!

                bRTCFullViewError=true;                                                                                 //Steven 20120206 : RTC重複錯誤

                if(ret==K_SKIP)                                                                                         //Steven 20120823 : Run Time出錯也要RTC
                {
                    fContact->InitROILearningTask();
                    bRTCFullViewError=false;
                    Task=200;
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 200:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle,MOT[MTestY1].GailSpeed, "DoBRTCUseSocketFloat 200"))
            {
                Task=300;
            }
            break;
        case 300:
            if(Prod.TestZ2_Drop_Offset!=0)
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, MOT[MTestZ1].GailSpeed))
                {
                    Task=400;
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, MOT[MTestZ1].GailSpeed))
                {
                    Task=400;                                                   //Steven 20110511
                }
            }
            break;
        case 400:
            COM2->InitRealTimeCCDPara();
            bBTestSuckDrop=false;
            IndexStatus=Z1Up_Z2Down;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iBRTCGiveWayCheckTask=1;
TQPF_Timer DoGiveWayFrontDelay;
bool DoBRTCGiveWayCheck(bool bInitial,bool bCheckHasIC,int iIndex)
{
    static int iTestY1_Middle=0;                                                //Ifor 20190719 : add 加快讓位移動速度
    static int iTestZ1_Test=0;
    static int iTestZ1_Drop=0;
    static int iTestY2_Middle=0;
    static int iTestZ2_Test=0;
    static int iTestZ2_Drop=0;
    static bool bHasSend=false;
    static AnsiString strSiteMap="";

    if(bInitial==true)
    {
        iBRTCGiveWayCheckTask=1;
        return false;
    }

    int iY2Pos=0;
    int &Task=iBRTCGiveWayCheckTask;
    long lZ2pos=0;
    long lY2pos=0;
    long lZ1pos=0;
    long lY1pos=0;
    AnsiString str="";

    switch(Task)
    {
        case 1:
            bHasSend=false;
            if(iIndex==2 && bCheckHasIC==true)
            {
                strSiteMap="";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_IC ||
                           FTestSuck.Item[i][j]==HAS_HOT_IC)
                        {
                            strSiteMap=strSiteMap+"1";
                        }
                        else
                        {
                            strSiteMap=strSiteMap+"0";
                        }
                    }

                    if(i<FTestSuck.iShtRow-1)
                        strSiteMap=strSiteMap+",";
                }
            }
            COM2->sRealTimeCom_Send[COM2->rtTESTMAP] = str.sprintf("@TESTSITE:%s+" , strSiteMap);                       //Ifor 20200522 fix:RTC2.0 命令變更
            Task=2;
        case 2:
            lZ1pos=MOT[MTestZ1].Gali_ReadPos();
            lY1pos=MOT[MTestY1].Gali_ReadPos();
            lZ2pos=MOT[MTestZ2].Gali_ReadPos();
            lY2pos=MOT[MTestY2].Gali_ReadPos();
            if(lZ2pos>=0 && lY2pos==Prod.TestY2_Rear &&
               lZ1pos>=0 && lY1pos==Prod.TestY1_Front)
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK]=false;
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                if(bCheckHasIC==1)
                {
                    COM2->SendCommToVision(COM2->rtTESTMAP, true);
                    COM2->SendCommToVision(COM2->rtCHECKIC, true);
                    COM2->SendCommToVision(COM2->rtFullTOK, true);
                }
                else
                {
                    COM2->SendCommToVision(COM2->rtCHECKNULL, true);
                    COM2->SendCommToVision(COM2->rtFullTOK, true);
                }
                DoGiveWayDelay.SetSecAndOn(10);
                Task=20;
            }
            else
            {
                if(iIndex==2)
                {
                    iTestY2_Middle            =Prod.TestY2_Middle;
                    iTestZ2_Test              =Prod.TestZ2_Test;
                    iTestZ2_Drop              =Prod.TestZ2_Drop_Offset;
                    Prod.TestY2_Middle        =Prod.TestY2_Rear;
                    Prod.TestZ2_Test          =Prod.TestZ2_Safe;
                    Prod.TestZ2_Drop_Offset   =0;
                    iBackUpZ2DownPosition     =Prod.TestZ2_Test;
                }
                else
                {
                    iTestY1_Middle            =Prod.TestY1_Middle;
                    iTestZ1_Test              =Prod.TestZ1_Test;
                    iTestZ1_Drop              =Prod.TestZ1_Drop_Offset;
                    Prod.TestY1_Middle        =Prod.TestY1_Front;
                    Prod.TestZ1_Test          =Prod.TestZ1_Safe;
                    Prod.TestZ1_Drop_Offset   =0;
                    iBackUpZ1DownPosition     =Prod.TestZ1_Test;
                }

                DoGiveWayFrontDelay.SetMSAndOn(0);                              //50 ms 5.9~6.0 Decode
                Task=10;
                bDoBRTCGiveWayCheck=true;
            }

            if(Task!=10)
            {
                break;
            }
        case 10:
            bDoBRTCGiveWayCheck=true;
            if(TestIF_File.bArm1PickPlaceArm2Test &&
               DeviceForm.ContactMode==TMoveDrop)                               //二段速上升
            {
                if(iIndex==2)
                {
                    Task=15;
                }
                else
                {
                    iY2Pos=MOT[MTestY2].Gali_ReadPos();
                    if(abs(iY2Pos-Prod.TestY2_Middle)<30)
                    {
                        if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+Prod.TestZ2_Up_Offset, MOT[MTestZ2].GailSpeed*Prod.TestZ_Up_Speed/100))
                        {
                            DoGiveWayFrontDelay.SetSecAndOn(Prod.TestZ_Up_Wait);
                            Task=12;
                        }
                    }
                }
            }
            else
            {
                if(iIndex==2)
                {
                    if(MOT[MTestY1].Z1UpZ2Down(MOT[MTestZ2].GailSpeed, true, false))
                    {
                        Prod.TestY2_Middle      =iTestY2_Middle;
                        Prod.TestZ2_Test        =iTestZ2_Test;
                        Prod.TestZ2_Drop_Offset =iTestZ2_Drop;
                        iBackUpZ2DownPosition   =Prod.TestZ2_Test;
                        IndexStatus=Z1Down_Z2Up;
                        Task=18;
                    }
                }
                else
                {
                    if(MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, true, false))
                    {
                        Prod.TestY1_Middle      =iTestY1_Middle;
                        Prod.TestZ1_Test        =iTestZ1_Test;
                        Prod.TestZ1_Drop_Offset =iTestZ1_Drop;
                        iBackUpZ1DownPosition   =Prod.TestZ1_Test;
                        IndexStatus=Z1Up_Z2Down;
                        Task=18;
                    }
                }
            }

            if(Task!=12)
            {
                break;
            }
        case 12:
            if(DoGiveWayFrontDelay.Off())
            {
                Task=15;
            }

            if(Task!=15)
            {
                break;
            }
        case 15:
            if(iIndex==2)
            {
                if(MOT[MTestY1].Z1UpZ2Down(MOT[MTestZ2].GailSpeed, true, false))
                {
                    Prod.TestY2_Middle      =iTestY2_Middle;
                    Prod.TestZ2_Test        =iTestZ2_Test;
                    Prod.TestZ2_Drop_Offset =iTestZ2_Drop;
                    iBackUpZ2DownPosition   =Prod.TestZ2_Test;
                    IndexStatus=Z1Down_Z2Up;
                    Task=18;
                }
            }
            else
            {
                if(MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, true, false))
                {
                    Prod.TestY1_Middle      =iTestY1_Middle;
                    Prod.TestZ1_Test        =iTestZ1_Test;
                    Prod.TestZ1_Drop_Offset =iTestZ1_Drop;
                    iBackUpZ1DownPosition   =Prod.TestZ1_Test;
                    IndexStatus=Z1Up_Z2Down;
                    Task=18;
                }
            }
            break;
        case 18:
            if(bHasSend==false)
            {
                if(DoGiveWayFrontDelay.Off())
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK]=false;
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                    if(bCheckHasIC==1)
                    {
                        COM2->SendCommToVision(COM2->rtTESTMAP, true);
                        COM2->SendCommToVision(COM2->rtCHECKIC, true);
                        COM2->SendCommToVision(COM2->rtFullTOK, true);
                    }
                    else
                    {
                        COM2->SendCommToVision(COM2->rtCHECKNULL, true);
                        COM2->SendCommToVision(COM2->rtFullTOK, true);
                    }
                    DoGiveWayDelay.SetSecAndOn(10);
                    bHasSend=true;
                }
            }
            Task=20;
            break;
        case 20:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->DoReleaseAndInspEnd();                                    //JerryYang 20260506 : Add
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                iBRTCGiveWayCheckTask=1;
                bDoBRTCGiveWayCheck=false;
                return true;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                ShowMyMessage("RTC Full View NG");
                COM2->DoReleaseAndInspEnd();
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                Task=1;
            }
            else if(DoGiveWayDelay.Off())
            {
                COM2->DoReleaseAndInspEnd();
                ShowMyMessage("RTC Release Time out of Give Way Check");
                Task=1;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
HTimer DoTestYRearDelay, DoTestYRearDelay2;
int iBRTCAutoModelVerifyTask=1;
bool DoBRTCAutoModelVerify(bool bInitial)
{
    static int iReleaseCT=0;
    static bool bVerifyNG=false;
    static bool bHasErr=false, bIndexSuckCheck=false;
    static bool bBTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bBTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};
    static bool bRTCRetry=false;                                                //Ifor 20251023 add:RTC無回應Retry 一次
    if(bInitial==true)
    {
        iBRTCAutoModelVerifyTask=1;
        for(int i=0; i<BTestSuck.iMaxRow; i++)
        {
            for(int j=0; j<BTestSuck.iMaxCol; j++)
            {
                bBTestSuckUse[i][j]=false;
                bBTSuckFinish[i][j]=false;
            }
        }
        return false;
    }

    int ret=0;
    int &Task=iBRTCAutoModelVerifyTask;
    bool bCheckAllSuck=false, flag=false;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoBRTCAutoModelVerify 1"))
            {
                bVerifyNG=false;
                bPickUpErrReAutoVerify=false;                                   //JerryYang 20220215 : RTC Auto Verify half view check
                Task=100;
            }
            break;
        case 100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear, iSpeedY, "DoBRTCAutoModelVerify 100"))
            {
                Task=150;
            }
            break;
        case 150:
            if(SendSiteMapToRTC(true, 2)!=-1)
            {
                DoTestYRearDelay.SetSecAndOn(10);
                Task=160;
            }
            break;
        case 160:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtSiteMap])
            {
                DoAllPassVerifyRTC(true);
                Task=200;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Site Map Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 200:
            if(DoAllPassVerifyRTC())
            {
                if(CosFunction.bRTCHalfViewAutoVerify )                         //JerryYang 20220215 : RTC Auto Verify half view check
                {
                    DoHalfViewAllPassVerifyRTC(true);
                    Task=12005;
                }
                else
                {
                    Task=12010;
                }
            }
            break;
        case 12005:
            if(DoHalfViewAllPassVerifyRTC())
            {
                Task=12010;
            }
            break;
        case 12010:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoBRTCAutoModelVerify 12010"))
            {
                iReleaseCT=0;
                Task=12020;
            }
            break;
        case 12020:                                                                                                                                             //Z2 Down

            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+750))                                                                      //kevin 20140612 add start
            {
                flag=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            //Steven 20110725 : 不再使用IsSuckICFallDown
                            if(BTestSuck.Suck[i][j].Enable       &&
                               BTestSuck.Suck[i][j].SenUsing!="" &&
                               BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                            {
                                if(BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    BTestSuck.Suck[i][j].Normal();                                                                                              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag=true;
                                    bRecIndexDropAlarm2=true;                                                                                                   //jou 2012-01-17 紀錄index Drop alarm
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    MOT[MTestZ2].Gali_Command("ST", __FUNC__);
                    Task=12050;
                    return false;
                }
            }

            if(Prod.TestZ2_Drop_Offset!=0)
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+100+IniConfig.iD36_RTCAutoVerifyReleaseHeight+iReleaseCT, iSpeedSlow))    //jou 20180226 : 200 -> 150 -> 130
                {
                    Task=12100;
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+100+IniConfig.iD36_RTCAutoVerifyReleaseHeight+iReleaseCT, iSpeedSlow))                            //jou 20180226 : 200 -> 150 -> 130
                {
                    Task=12100;                                                                                                                                 //Steven 20110511
                }
            }
            break;
        case 12050:                                                             //kevin 20140612  上升方便取料
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, 50000))              //JimmyChiu 20211028 : All speed can set by speed setting.
            {
                Task=12055;
            }
            break;
        case 12055:
            bHasErr|=CheckIndexAllSuckICFallDown(false, true);
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BTestSuck.Suck[i][j].Enable       &&
                       BTestSuck.Suck[i][j].SenUsing!="" &&
                       BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                       BTestSuck.Item[i][j]!=NULL_IC)
                    {
                        if(BTestSuck.Suck[i][j].GetStatus()==false)
                        {
                            ErrPart+=IndexSuckName[i][j];
                            bHasErr=true;
                        }
                    }
                }
            }

            if(LastSet.iRealDummy==REALLY && bHasErr)
            {
                if(IndexAlarmInArmAway()==true)
                {
                    if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                        bIsTestSitICFallDown=true;
                    ShowErrorMessage("JAM0304", K_SKIP, MTestZ2, false, ErrPart);                                       //Steven 20100129 : Device Drop Error

                    bPickUpErrReAutoVerify=true;                                                                        //JerryYang 20241220 : auto verify發生index drop error要重做

                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Error ||
                               (BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                BTestSuck.Item[i][j]!=NULL_IC &&
                                BTestSuck.Suck[i][j].GetStatus()==false))                                               //有用到且有吸到IC的卻掉了
                            {
                                BTestSuck.SetItemData(i, j, HAS_NULL_IC);                                               //Steven 20110829 : 把有IC掉料的位置改成Has Null IC
                                BTestSuck.Suck[i][j].Normal();                                                          //Steven 20110829 : 把真空關掉
                            }
                        }
                    }
                    break;
                }
            }

            if(bIsTestSitICFallDown==false)
            {
                Task=12020;
            }
            break;
        case 12100:                                                             //Place IC to socket
            bBTestSuckDrop=true;

            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_HOT_IC || BTestSuck.Item[i][j]==HAS_IC)
                    {
                        BTestSuck.Suck[i][j].Off();
                    }

                    DoTestYRearDelay.SetSecAndOn(0.3);
                }
            }
            Task=12200;
            break;
        case 12200:
            if(DoTestYRearDelay.Off())
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_HOT_IC ||
                           BTestSuck.Item[i][j]==HAS_IC)
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }

                bIndexCheckNoStopVaccum=false;
                DoTestYRearDelay.SetSecAndOn(0.3);
                Task=12210;
            }
            break;
        case 12210:
            if(DoTestYRearDelay.Off())
            {
                iReleaseCT+=10;
                if(iReleaseCT<100)
                {
                    Task=12020;
                }
                else
                {
                    DoTestYRearDelay.SetSecAndOn(3.0);
                    Task=12220;
                }
            }
            break;
        case 12220:
            if(DoTestYRearDelay.Off())
            {
                Task=12300;
            }
            break;
        case 12300:                                                             //Z1 Up //Steven 20180417 : RTC Auto Verify新增回吸偵測
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+1000, iSpeedSlow))
            {
                Task=12310;
            }
            break;
        case 12310:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_HOT_IC ||
                       BTestSuck.Item[i][j]==HAS_IC)
                    {
                        if(INDEX_SUCKER_TYPE==1)                                //Steven 20111202
                        {
                            fiosetview->bIndexSuck[1][i][j]=true;
                        }
                        else
                        {
                            BTestSuck.Suck[i][j].On();                          //kevin 20110504 check 掉料
                        }
                    }
                }
            }
            DoTestYRearDelay.SetSecAndOn(1);
            bIndexSuckCheck=false;
            Task=12320;
        case 12320:
            if(INDEX_SUCKER_TYPE==1)
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=fiosetview->ProcessIndexSuckDestroy2();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeedSlow))
            {
                Task=12330;
            }
            break;
        case 12330:
            if(INDEX_SUCKER_TYPE==1)
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=fiosetview->ProcessIndexSuckDestroy2();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(DoTestYRearDelay.Off() && bIndexSuckCheck==true)
            {
                bHasErr=false;

                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].GetStatus())
                        {
                            bHasErr=true;
                        }
                        else
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }

                        if(INDEX_SUCKER_TYPE==1)                                //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }

                if(bHasErr)
                {
                    Task=12340;
                }
                else
                {
                    Task=12390;
                }
            }
            break;
        case 12340:
            ErrPart=" ";
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Suck[i][j].GetStatus())
                    {
                        ErrPart+=IndexSuckName[i][j];
                    }
                }
            }

            if(bHasErr)
            {                                                                                                           //jou 2012-03-05 開啟D44偵測到卡料時，In arm也要讓位。
                if(IndexAlarmInArmAway()==true)                                                                         //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, false, ErrPart);                                      //Vacuum Sensor Off Error
                    Task=12310;
                }
                else
                {
                    return false;
                }
            }
            break;
        case 12390:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeedSlow))
            {
                Task=12400;
            }
            break;                                                              //Steven 20180417 : RTC Auto Verify新增回吸偵測
        case 12400:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoBRTCAutoModelVerify 12400"))
            {
                bRTCAutoVerifyControlEP=true;
                ADAM_WriteVoltage(4.0);

                if(IniConfig.bD36_1EnableRTCAutoModelVerifyLive==true)
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtOpenLive]=false;
                    COM2->SendCommToVision(COM2->rtOpenLive, true);
                    DoTestYRearDelay.SetSecAndOn(10);
                    Task=12410;
                }
                else
                {
                    Task=12500;
                }
            }
            break;
        case 12410:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtOpenLive])
            {
                Task=12415;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Open Live Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12400;
            }
            break;
        case 12415:
            if(IndexAlarmInArmAway())
            {
                ShowMyMessage("RTC Auto Verity 產品可能因為沾黏導致放置歪斜,請確認!","device may be skewed due to sticking. Please confirm!");
                Task=12420;
            }
            break;
        case 12420:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtCloseLive]=false;
            COM2->SendCommToVision(COM2->rtCloseLive, true);
            DoTestYRearDelay.SetSecAndOn(10);
            Task=12430;
            break;
        case 12430:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtCloseLive])
            {
                Task=12500;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Close Live Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12420;
            }
            break;
        case 12500:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease]=false;
            COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd]=false;
            COM2->DoReleaseAndInspEnd();                                        //JerryYang 20220215 : Release跟InspEnd一起送
            DoTestYRearDelay.SetSecAndOn(10);
            Task=12600;
            break;
        case 12600:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease] &&
               COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd] &&
               COM2->bRealTimeCom_ReceiveOK[COM2->rtInspStart])                 //JerryYang 20220215 : Release跟InspEnd一起送
            {
                Task=12900;
                bRTCRetry=false;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(bRTCRetry==true)
                {
                    bRTCRetry=false;
                    if(COM2->OpenRTCComPortAgain())
                        ShowMyMessage("RTC Release Time out");                  //JerryYang 20220215 : Release跟InspEnd一起送
                    COM2->DoReleaseAndInspEnd();
                }
                else
                {
                    bRTCRetry=true;
                }
                Task=12500;
            }
            break;
        case 12900:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYOK]=false;
            COM2->SendCommToVision(COM2->rtOPENVERIFYNG, true);
            DoTestYRearDelay.SetSecAndOn(10);
            Task=13000;
            break;
        case 13000:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYOK])
            {
                Task=13100;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYNG])
            {
                ShowMyMessage("RTC Open Verify NG");
                Task=12500;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Open Verify Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12500;
            }
            break;
        case 13100:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG]=false;
            COM2->SendCommToVision(COM2->rtALLFAILOK, true);
            DoTestYRearDelay.SetSecAndOn(10);
            Task=13200;
            break;
        case 13200:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK])
            {
                bVerifyNG=false;
                if(CosFunction.bRTCHalfViewAutoVerify)                          //JerryYang 20220215 : RTC Auto Verify half view check
                {
                    DoHalfViewAllFailVerifyRTC(true);
                    Task=13210;
                }
                else
                {
                    Task=13400;
                }
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG])
            {
                bVerifyNG=true;
                Task=13300;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Verify All Fail Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12500;
            }
            break;
        case 13210:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease]=false;                //JerryYang 20220215 : Release跟InspEnd一起送
            COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd]=false;
            COM2->DoReleaseAndInspEnd();
            DoTestYRearDelay.SetSecAndOn(10);
            Task=13220;
            break;
        case 13220:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease] &&
               COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd] &&
               COM2->bRealTimeCom_ReceiveOK[COM2->rtInspStart])
            {
                Task=13350;
                bRTCRetry=false;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(bRTCRetry==true)
                {
                    bRTCRetry=false;
                    if(COM2->OpenRTCComPortAgain())
                        ShowMyMessage("RTC Release Time out");
                }
                else
                {
                    bRTCRetry=true;
                }
                Task=13210;
            }
            break;
        case 13300:
            if(IndexAlarmInArmAway()==true)
            {
                Task=13400;
            }
            break;
        case 13350:
            if(DoHalfViewAllFailVerifyRTC())                                    //JerryYang 20220215 : RTC Auto Verify half view check
            {
                Task=13400;
            }
            break;
        case 13400:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoBRTCAutoModelVerify 13400"))
            {
                Task=13500;
            }
            break;
        case 13500:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+10+IniConfig.iD36_RTCAutoVerifyPickHeight, iSpeedSlow/3))                     //jou 20180226 : 0 -> +110 -> +60 -> 0 -> +10
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]!=NULL_IC &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            bBTestSuckUse[i][j]=true;
                        }
                        else
                        {
                            bBTestSuckUse[i][j]=false;
                        }

                        BTestSuck.Suck[i][j].Error=false;
                        bBTSuckFinish[i][j]=false;
                    }
                }
                iHangupCTArm2=0;
                Task=13600;
            }
            break;
        case 13600:
            COM2->SendCommToVision(COM2->rtCLOSEVERIFY, true);
            DoTestYRearDelay.SetSecAndOn(10);
            Task=13700;
            break;
        case 13700:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtCLOSEVERIFY])
            {
                Task=13800;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Close Verify Time out");
                COM2->DoReleaseAndInspEnd();
                Task=13600;
            }
            break;
        case 13800:
            bCheckAllSuck=true;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bBTSuckFinish[i][j]==false)
                    {
                        if(bBTestSuckUse[i][j])
                        {
                            if(BTestSuck.Suck[i][j].Suck())
                            {
                                bBTSuckFinish[i][j]=true;
                                bBTestSuckUse[i][j]=false;
                            }
                            else if(BTestSuck.Suck[i][j].Error)
                            {
                                bBTSuckFinish[i][j]=true;
                            }
                            else
                            {
                                bCheckAllSuck=false;
                            }
                        }
                        else
                        {
                            bBTSuckFinish[i][j]=true;
                        }
                    }
                }
            }

            flag=true;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bBTSuckFinish[i][j]==false)
                        flag=false;
                }
            }

            if(flag)
            {
                bHasErr=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            if(IniConfig.bD62PickUpErrorNeedPurge)              //Steven 20161024 : 吸取異常需要吹氣一次
                            {
                                BTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bCheckAllSuck)
                {
                    Task=13900;
                }
            }
            break;
        case 13900:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoBRTCAutoModelVerify 13900"))
            {
                bRTCAutoVerifyControlEP=false;
                ADAM_WriteVoltage(DeviceForm.dPress);                           //JerryYang 20220215 : 修正EP氣量錯誤
                DoTestYRearDelay.SetSecAndOn(3);
                Task=13950;
            }
            break;
        case 13950:
            if(DoTestYRearDelay.Off())
            {
                if(bVerifyNG==true || bHasErr==true)
                    Task=14000;
                else
                    Task=14400;
            }
            break;
        case 14000:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoBRTCAutoModelVerify 14000"))
            {
                if(bVerifyNG==true && bHasErr==false)
                {
                    ShowMyMessage("RTC Auto Verify fail!!");
                    Task=1;
                }
                else
                {
                    if(bVerifyNG==true && bHasErr==true)
                        bRTCAutoModelVerifyFirstTime=true;

                    Task=14100;
                }
            }
            break;
        case 14100:
            ErrPart=" ";
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        ErrPart+=IndexSuckName[i][j];
                        if(IniConfig.bD62PickUpErrorNeedPurge)                  //Steveb 20161024 : 吸取異常需要吹氣一次
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].Error=false;
                    }
                }
            }
            bIsTestSitICFallDown=true;
            bPickUpErrReAutoVerify=true;                                        //JerryYang 20220215 : RTC Auto Verify half view check
            ret=ShowErrorMessage("JAM0302", K_SKIP, MTestZ2, false, ErrPart);   //Devicr Pick-Up Error

            if(ret==K_SKIP)
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
            }

            DoAllPassVerifyRTC(true);
            Task=14200;
            break;
        case 14200:
            if(DoAllPassVerifyRTC())
            {
                Task=14300;
            }
            break;
        case 14300:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoBRTCAutoModelVerify 14300"))
            {
                Task=14400;
            }
            break;
        case 14400:
            if(fContact->fShow && iContactMode!=CONTACT_NORMAL)                 //JerryYang 20220215 : RTC Auto Verify half view check
            {
                Task=15000;
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
                {
                    Task=15000;
                }
            }
            break;
        case 15000:
            if(SendSiteMapToRTC(true,0)!=-1)
            {
                DoTestYRearDelay.SetSecAndOn(10);
                Task=15100;
            }
            break;
        case 15100:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtSiteMap])
            {
                DoAllPassVerifyRTC(true);
                Task=15200;
            }
            else if(DoTestYRearDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Site Map Time out");
                COM2->DoReleaseAndInspEnd();
                Task=15000;
            }
            break;
        case 15200:
            if(CheckTwoArmSiteMap()==true)
            {
                if(bDoFRTCAutoModelVerify==false)
                    bRTCAutoModelVerifyFirstTime=true;
            }

            RecordProcess("RTC auto verify end");
            bDoFRTCAutoModelVerify=true;
            COM2->InitRealTimeCCDPara();
            bBTestSuckDrop=false;
            if(fContact->fShow && iContactMode!=CONTACT_NORMAL)                 //JerryYang 20220215 : RTC Auto Verify half view check
            {
            }
            else
            {
                IndexStatus=Z1Up_Z2Down;
            }
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iIndexArm1PickUpErrNeedPiggybackTask=1;
TQPF_Timer hCheckSockerDelay1;
bool DoIndexArm1PickUpErrNeedPiggyback(bool bInitial)                           //Steven 20190115 : SCC要求吸取異常要檢查Socket
{
    static AnsiString ErrPart="";

    if(bInitial==true)
    {
        iIndexArm1PickUpErrNeedPiggybackTask=1;
        return false;
    }

    int iIndexUpPos=0;
    int iNN=IsNNMode();
    int iIndexCheckOffSet=IniConfig.fIndexCheckOffset*100;                      //ChungHung 20140807 add for ATK TestZ_Test + fIndexCheckOffset
    int &Task=iIndexArm1PickUpErrNeedPiggybackTask;
    bool flag=false, flag2=false;

    switch(Task)
    {
        case 1:
            Task=1030;                                                          //對齊DoCheckSocketHasIC()的Task
        case 1030:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==NULL_IC ||
                       FTestSuck.Item[i][j]==HAS_NULL_IC)
                    {
                        if(INDEX_SUCKER_TYPE==1)
                        {
                            fiosetview->bIndexSuck[0][i][j]=true;
                            bIndexCheckNoStopVaccum=true;
                        }
                        else
                        {
                            FTestSuck.Suck[i][j].On();                          //Sam 20230110 : 修正連續 Start<>Stop Drop Contact 掉料問題
                        }
                    }
                }
            }

            hCheckSockerDelay1.SetSecAndOn(0.5);
            Task=1040;
//            break;
        case 1040:
            if(INDEX_SUCKER_TYPE==1)
            {
                fiosetview->ProcessIndexSuckDestroy1();
            }

            if(hCheckSockerDelay1.Off())
            {
                Task=1050;
            }
            break;
        case 1050:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                                                                                                    //Steven 20140620 : 整合為Function

            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos+iIndexCheckOffSet, iSpeedFast))                                   //ChungHung 20140807 add for ATK TestZ_Test
            {
                Task=1060;
                if(CUSTOMER_CODE==CC_Greatek)                                                                                                                   //Wei 20160413
                    hCheckSockerDelay1.SetSecAndOn(5);                                                                                                          //Steven 20110908 : 上來後也要Delay一下
                else
                    hCheckSockerDelay1.SetSecAndOn(0.5);                                                                                                        //Steven 20110908 : 上來後也要Delay一下
            }
            break;
        case 1060:
            if(hCheckSockerDelay1.Off())
            {
                flag=false;
                ErrPart=" ";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==NULL_IC ||
                           FTestSuck.Item[i][j]==HAS_NULL_IC)
                        {
                            flag2=false;
                            FTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                            if(flag2==true)
                            {
                                flag=true;
                                ErrPart+=IndexSuckName[i+iNN][j];

                                if(CUSTOMER_CODE==CC_SCS)                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                    FTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(flag && LastSet.iRealDummy==REALLY)                          //Steven 20120726 : 有跑IC才檢查Socket
                {
                    IndexStatus=Z1_Z2_Normal;
                    Task=1070;
                }
                else
                {
                    return true;
                }
            }
            break;
        case 1070:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoIndexArm1PickUpErrNeedPiggyback 1070"))
            {
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]==NULL_IC ||
                               FTestSuck.Item[i][j]==HAS_NULL_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }
                }
                Task=1080;
            }
            break;
        case 1080:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear, iSpeedY, "DoIndexArm1PickUpErrNeedPiggyback 1080"))
            {
                Task=1090;
            }
            break;
        case 1090:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)            //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm1 detect Test Socket has IC error", "Arm 1偵測到Socket有IC殘留!!", "DoIndexArm1PickUpErrNeedPiggyback 1090");
                }
                ErrPart="";
                Task=1100;
            }
            break;
        case 1100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoIndexArm1PickUpErrNeedPiggyback 1100"))
            {
                IndexStatus=Z1Down_Z2Up;
                Task=2200;
            }
            break;
        case 2200:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, iSpeedFast))         //ChungHung 20140807 add for ATK TestZ_Test
            {
                return true;
            }
    }

    return false;
}
//------------------------------------------------------------------------------
int CheckAnyCaseNeedToDoArm2()
{
    int Task=0;
    if(IniConfig.bD43IndexPickErrCheckSocket &&
       bIndexArm1PickUpErrNeedPiggyback==true)
    {
        bIndexArm1PickUpErrNeedPiggyback=false;
        return 16000;
    }

    if(TestIF_File.bUseSLKClamp && TestIF_File.iSeparabilityTest==1)            //JerryYang 20160429 分離模式
    {
        BTestCombineSLK(true);                                                  //初始化流程
        return 14000;                                                           //JerryYang 20160429 分離模式, 測試完成 Z1 SLK要與Clamp結合,再將IC吸起流程
    }

    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum && LastSet.iRealDummy==REALLY &&
       CosFunction.bRTCAutoModelVerify==true && IniConfig.bD36EnableRTCAutoModelVerify==true &&
       bRTCAutoModelVerifyFirstTime==true)                                      //jou 2014-06-24 RTC 自動進行Model驗證
    {
        if(BTestSuck.UseSiteHasIC() &&
           BTestSuck.AlreadyTest()==false)
        {
            if(SendSiteMapToRTC(false, 2)==BTestSuck.CountRealIC())
            {
                bNeedWaitRTCAutoVerify=true;
                bRTCAutoModelVerifyFirstTime=false;
                RecordProcess("RTC auto verify start");
                return 12000;
            }
        }
    }
    else
    {
        bRTCAutoVerifyControlEP=false;
    }

    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum && TestIF_File.bUseSocketFloat==true)
    {
        return 15000;
    }

    if(CosFunction.bTesterSidePushFunction==true &&
              DeviceForm.bTesterSidePush==true &&
              DeviceForm_File.iSidePushMode==1)                                 //Richard 20230301 : 測試完成, 側推縮回
    {
        return 18500;
    }

    if(DeviceForm.VacuumMode==VacuumONMode)
    {
        iHangupCTArm2=0;
        if(DeviceForm.ContactMode==DropContact &&                               //JerryYang 20170522 drop contact改為邊吸邊測
           CosFunction.bSuckDevicesDuringTest==true &&                          //JerryYang 20170804 (Steven) 移除邊吸邊測的選項
           INDEX_SUCKER_TYPE==1)
        {
            CheckIndexAllSuckICFallDown(true, false);
        }

        if(IniConfig.bIndexArm2SupplyLight==true ||                             //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
           TestIF_File.bForEgisTecTest==true     ||                             //Steven 20140922 : Arm2當作指紋測試
           (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                     //kevin 20150127 Arm1 下壓 arm2 測試
            TestIF_File.bArm1PickPlaceArm2Test==true))                          //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
        {
            Task=11040;
        }
        else
        {
//            ShowIndexTime(1);                                                 //Steven 20140619 : 測試      //到這裡大概0.015~0.031Sec
            Task=1;
        }
    }
    else
    {
        if(DeviceForm.ContactMode==DropPlaceShiftContact)                       //ChungHung 20150528 add for 海思 _8Site1x4
        {
            Task=211;
        }
        else
        {
            Task=215;
        }

        iHangupCTArm2=0;

        if(IniConfig.bIndexArm2SupplyLight==true ||                             //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
           TestIF_File.bForEgisTecTest==true     ||                             //Steven 20140922 : Arm2當作指紋測試
           (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                     //kevin 20150127 Arm1 下壓 arm2 測試
            TestIF_File.bArm1PickPlaceArm2Test==true))                          //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
        {
            Task=11040;
        }
    }
    return Task;
}
//------------------------------------------------------------------------------
int iTestYRearTask=1;
void InitTestYRearTask()
{
    iTestYRearTask=1;
}
//------------------------------------------------------------------------------
TQPF_Timer iWaitIndexArm2;
TQPF_Timer hTestZ1Delay;
TQPF_Timer hFRTCTimeOutDelay;                                                   //JerryYang 20220923 : add
extern bool CheckTestSuckICOn(class TMyKitSuck &Ptr, int iR, int iC);
bool DoTestYRear()
{
    static int iRetry=0;
    static bool bOneTimeFlag=true, flag1=false, bSocketCheckSkip=false, bSucketHasICError=false;
    static bool bGetTime1=true, bGetTime2=true, bOnce=false;                    //JerryYang 20220923 : add
    static bool bintered2=true;                                                 //Isaac 20200922 : 紀錄indexArmY encoder值和command值
    static bool bIndexFinish=false;
    static bool bPlaceTestIC=false;                                             //Ifor 20190713 : add Arm1 丟下IC
    static bool bCheckShuttle=false;
    static bool bEPfirst=true, bFirst=true;                                     //Ifor 20150803 : 新增EP量測旗標，第一次才執行Log紀錄  //JerryYang 20180817 (Steven) : fix EOT delay
    static bool bSLKClampProcessFinish=false;                                   //JerryYang 20160526
    static bool bFTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false} };

    static bool bFTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},                                        //Steven 20110301 : 確認吸取完成
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bArmDuplicateErr[4][8]={{false, false, false, false, false, false, false, false},
                                        {false, false, false, false, false, false, false, false},
                                        {false, false, false, false, false, false, false, false},
                                        {false, false, false, false, false, false, false, false}};

    static AnsiString ErrPart="";
    static AnsiString str="";
    //==> Eastsun 20260511 F007 整合: T11/T12 Index 掉料當下馬達位置
    AnsiString StrPos="";                                                          //Ifor 20220125 add:Index 掉料當下 馬達位置
    int iArm1_Y=0, iArm1_Z=0, iArm2_Y=0, iArm2_Z=0;                                //Ifor 20220125 add:Index 掉料當下 馬達位置
    //<== Eastsun 20260511 F007 整合
    DoTestYRear_ReStart:                                                        //2008/10/20 lee

    QueueTaskList[23].CheckTaskChange();                                        //Steven 20200821 : 使用Goto也要記錄Task變化

    int &Task=iTestYRearTask;
    int iIndexZPos=0;                                                           //Ifor 20220906 add:讀取Index Z 目前位置
    int iIndexUpPos=0, iCT=0;
    int ret=0, iFlag=0;
    int iNN=IsNNMode();
    int iFinishTestUpWaitHeight=0;                                              //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
    int iAZ1=0, iAZ2=0, iAZ3=0;                                                 //kevin 20150506
    bool TMode=false;
    bool bCheckAllSuck=false, flag=false, bCheckSuck=false, bCheckDestroy=false, bHasErr=false, bHasDuplicateErr=false;
    long lPos[4]={0, 0, 0, 0};                                                  //kevin 20150915
    AnsiString str1;
    AnsiString StrDate="";
    AnsiString sBufferT="";                                                     //kevin 20150506
    AnsiString sTesterSidePush="Cylinder Side Push Status Error";               //Richard 20220321 : 渠梁Side Push
    static bRetryRTC=false;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
    {
        iIndexArmCheck_SG_Arm1=Offset.iIndexArmContact[0];
        iIndexArmCheck_SG_Arm2=Offset.iIndexArmContact[1];
    }
    else
    {
        iIndexArmCheck_SG_Arm1=0;
        iIndexArmCheck_SG_Arm2=0;
    }

    switch(Task)
    {
        case 1:
            bIndex1Suck=false;                                                  //kevin 20220105 Index 在下真空建立 pause 不能關閉
            fRearNeedSuck=false;
            fRearNeedDestroy=false;
            InitRearTestSuckICTask();
            InitRearTestDestroyICTask();
            fFrontNeedSuckIC=false;

            if(BTestSuck.UseSiteNoIC())
            {
                InitRearTestSuckICTask();
                if(CanYieldAlarmRemainInSHT())                                  //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)       //AI(ht9045-index-flow) 20260415 (RogerYang) : Shuttle2取消時不需要後臂吸取, 避免fCanMoveM鎖住造成F18偵測死鎖
                {
                }
                else
                {
                    fRearNeedSuck=true;
                }
            }
            else
            {
                if((BTestSuck.UseSiteHasIC() &&
                    BTestSuck.AlreadyTest()) ||
                   BTestNeedDestroy())
                {
                    fRearNeedDestroy=true;                                      // 先放掉已測IC,再進行新IC吸取
                    if(CanYieldAlarmRemainInSHT())                              //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                    {
                    }
                    else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)   //AI(ht9045-index-flow) 20260415 (RogerYang) : Shuttle2取消時不需要後臂吸取, 避免fCanMoveM鎖住造成F18偵測死鎖
                    {
                    }
                    else
                    {
                        fRearNeedSuck=true;
                    }
                    InitRearTestSuckICTask();
                    InitRearTestDestroyICTask();
                }
            }

            fFrontNeedTest=false;
            if(FTestSuck.UseSiteHasIC() &&
               FTestSuck.AlreadyTest()==false)
            {
                fFrontNeedTest=true;
                InitFTestSuckTestICTask();
                bArm1IsTest=false;                                              //JerryYang 20180629 (wei) : 用來判斷是否在測試中
            }

            Task=100;
            if(bNeedCheckRTCReport==true)
            {
                Task=2;
                DoGiveWayDelay.SetSecAndOn(10);
            }
            goto DoTestYRear_ReStart;                                           //2008/10/20 lee
        case 2:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->DoReleaseAndInspEnd();                                    //JerryYang 20260506 : Add
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                Task=100;
                bNeedCheckRTCReport=false;
                goto DoTestYRear_ReStart;                                       //2008/10/20 lee
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                COM2->DoReleaseAndInspEnd();
                ShowMyMessage("RTC Full View NG");
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                Task=3;
            }
            else if(DoGiveWayDelay.Off())
            {
                COM2->DoReleaseAndInspEnd();
                ShowMyMessage("RTC Release Time out Test Y Rear");
                Task=3;
            }
            break;
        case 3:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK]=false;
            COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
            COM2->SendCommToVision(COM2->rtCHECKNULL, true);
            COM2->SendCommToVision(COM2->rtFullTOK, true);
            DoGiveWayDelay.SetSecAndOn(10);
            Task=2;
            break;
        //-------------------------
        //掉料時的處置方式
        //-------------------------
        case 50:
            MOT[MTestZ1].MovFlag=false;
            MOT[MTestZ1].bScanFlag=false;
            ShowIndexTime(-2);                                                  //Steven 20200715 : 重新計算Cycle Time
            DoTestYRearDelay.SetMSAndOn(300);
            Task=52;
            break;
        case 52:
            if(DoTestYRearDelay.Off())
                Task=55;
            break;
        case 55:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 50000, "DoTestYRear 55"))
            {
//                if(REAL_TIME_CCD==true && COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
//                if(REAL_TIME_CCD==true && COM2->RTC_AlarmType())    //wei 20221222 RTC ARM Error
//                    Task=61;
//                else
//                    Task=60;

                if(IniConfig.bI26TestCloseSiteHaveBin && bTestBinDataError!=0)  //kevin 20150202 需強致將arm上 ic取出
                {
                    Task=56;
                    return false;
                }
                else
                {
                    if(REAL_TIME_CCD==true && COM2->RTC_AlarmType())    //wei 20221222 RTC ARM Error
                    {
                        bRetryRTC=true;
                        Task=61;
                    }
                    else
                    {
                        bRetryRTC=false;
                        Task=60;
                    }
//                    Task=60;
                }
            }
            break;
        case 56:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                Task=57;
                return false;
            }
            break;
        case  57:                                                               //kevin 20150202 需將arm上 ic取出
            if(IniConfig.bI26TestCloseSiteHaveBin &&bTestBinDataError==0)       //kevin 20150202 需強致將arm上 ic取出
            {
                Task=55;
                return false;
            }
            else
            {
                ShowMyMessageUp("Test Bin error, must take out all IC for test.", "測試bin 設定有問題,將ARM 2 上產品取出", false);
            }
            break;
        case 60:
            if(MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false)
            {
                lPos[0]=Prod.TestZ1_Safe;                                                                               //kevin 20150915
                RecordIndexPositionError("DoTestYRear()Task=60", true, false, false, false, &lPos[0]);                  //kevin 20150915 record

                ShowIndexMotorError(AnsiString("DoTestYRear60"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestYRear 60"))
            {
                Task=6000;
            }
            break;
        case 6000:
            if(IndexAlarmInArmAway()==true)                                                                             //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                str1.sprintf("DoTestYRear 6000: iShowSocketSensor=%d, bSucketHasICError=%d", iShowSocketSensor, (bSucketHasICError)?1:0);
                RecordProcess(str1);

                if(TestIF_File.bEnSocketSensor &&                                                                       //Steven 20200709 : 換位置
                   iShowSocketSensor)                                                                                   //kevin 20130504 socket sensor
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                //Steven 20151022 : add for MAXIM
                        bIsTestSitICFallDown=true;
                    bIsSocketSensor=true;

                    if(iShowSocketSensor==2)                                                                            //Steven 20200604 : Arm1丟 Arm2測的時候, Arm2只檢查置偏
                    {
                        ShowErrorMessage("WAR0323", K_RETRY, MTestZ1, false, sSocketSensorErr);                         //Socket detect device floting error
                        Task=78;                                                                                        //Steven 20200615 : 置偏不用做RTC的檢查
                        iShowSocketSensor=0;                                                                            //Jimmychiu 20230817 : Socket sensor異常顯示後參數歸零
                        CheckSocketSensor(1, "DoTestYRear_6000", true);
                        break;
                    }
                    else if(iShowSocketSensor==1)
                    {
                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, sSocketSensorErr);                         //kevin 20130504 socket sensor
                    }
                    iShowSocketSensor=0;
                    CheckSocketSensor(1, "DoTestYRear_6000", true);
                }

                if(bSucketHasICError)
                {
                    bIsTestSitICFallDown=true;
                    bSucketHasICError=false;
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                //Steven 20130604 : Socket殘料要按Z1
                    {
                        ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, ErrPart);                                  //Steven 20101122
                    }
                    else
                    {
                        ShowMyMessage("Socket has IC error", "Socket有IC殘留!!", "DoTestYRear 60");
                    }
                }

                if(IniConfig.bVTESTFunction==true &&                                                                    //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
                   IniConfig.bGetRcsCheckingResult==true)
                {
                    if(fMesSystem->asGetRcsCheckingResult!="PASS")
                    {
                        if(fMesSystem->GetRcsCheckingResult(true)==false)
                            break;
                    }
                }

                Task=61;                                                                                                //Steven 20201014 : 修正掉料異常時, index arm要分開
            }
            break;
        case 61:
            if(REAL_TIME_CCD==true &&                                           //----- by dell ccd realtime-------------
               (MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
                MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false))
            {
                ShowIndexMotorError(AnsiString("DoTestYRear61"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestYRear 61"))
            {
                //----- by dell ccd realtime-------------
//                if(REAL_TIME_CCD==true && COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
                if(REAL_TIME_CCD==true && COM2->RTC_AlarmType())    //wei 20221222 RTC ARM Error
                {
                    bRetryRTC=true;
                    Task=64;                                                    //Only RealTime
                }
                else
                {
                    bRetryRTC=false;
                    Task=62;
                }
            }
            break;
        case 62:
            if(bRecIndexDropAlarm2==true)
            {
                if(CheckIndexSuckICFallDownSetToHasNullIC(1))
                {
                    if(REAL_TIME_CCD==true)                                     //----- by dell ccd realtime-------------
                    {
                        COM2->DoReleaseAndInspEnd();
                    }
                    Task=2000;
                }
            }
            else
            {
                if(CheckIndexSuckICFallDownSetToHasNullIC(0))
                {
                    if(REAL_TIME_CCD==true)                                     //----- by dell ccd realtime-------------
                    {
                        COM2->DoReleaseAndInspEnd();
                    }
                    Task=2000;
                }
            }
            break;
        case 64:
            if(IndexAlarmInArmAway()==true)                                                                             //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
//                if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
                if(COM2->RTC_AlarmType())       //wei 20221222 RTC ARM Error
                {
                    bRetryRTC=true;
                    if(CUSTOMER_CODE==CC_KYEC_LEE)                              //Ifor 20210505 add:
                    {
                        AnsiString Str="";
                        ErrPart="";
                        for(int i=0; i<4; i++)
                        {
                            if(iRTC_CCD_NG[i]!=0)
                            {
                                Str.sprintf("CCD %d, ", i+1);
                                ErrPart=ErrPart+Str;
                                iRTC_CCD_NG[i]=0;
                            }
                        }
                    }

                    if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm1NG])           //wei 20221222 RTC ARM Error
                    {
                        iRTCErrorCount++;
                        if(iRTCErrorCount>1)
                        {
                            if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)     //Steven 20151022 : add for MAXIM
                                bIsTestSitICFallDown=true;

                            if(CUSTOMER_CODE==CC_KYEC_LEE)
                                ShowErrorMessage("WAR0354", 0, MMIndex, bRTCArm2HalfViewError, ErrPart);
                            else
                                ShowErrorMessage("WAR0354", 0, MMIndex, bRTCArm2HalfViewError, __FUNC__);

                            iRTCErrorCount=0;
                        }
                        else
                        {
                            sBufferT.printf("RTC ALARM Arm1 NG Auto Retry:%d", iRTCErrorCount);
                            MyDBIProcess("Message", sBufferT);
                        }
                    }
                    else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm2NG])      //wei 20221222 RTC ARM Error
                    {
                        iRTCErrorCount++;
                        if(iRTCErrorCount>1)
                        {
                            if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)     //Steven 20151022 : add for MAXIM
                                bIsTestSitICFallDown=true;

                            if(CUSTOMER_CODE==CC_KYEC_LEE)
                                ShowErrorMessage("WAR0355", 0, MMIndex, bRTCArm2HalfViewError, ErrPart);
                            else
                                ShowErrorMessage("WAR0355", 0, MMIndex, bRTCArm2HalfViewError, __FUNC__);

                            iRTCErrorCount=0;
                        }
                        else
                        {
                            sBufferT.printf("RTC ALARM Arm2 NG Auto Retry:%d", iRTCErrorCount);
                            MyDBIProcess("Message", sBufferT);
                        }
                    }
                    else
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                //Steven 20151022 : add for MAXIM
                        bIsTestSitICFallDown=true;

                        if(CUSTOMER_CODE==CC_KYEC_LEE)
                            ShowErrorMessage("WAR0342", 0, MMIndex, bRTCArm2HalfViewError, ErrPart);   //RTC Socket Has Device Error
                        else
                    ShowErrorMessage("WAR0342", 0, MMIndex, bRTCArm2HalfViewError, __FUNC__);                           //RTC Socket Has Device Error
                    }
//                    COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                    COM2->DoReleaseAndInspEnd();                                                                        //Steven 20120522 : 換位置
                    bRTCArm2HalfViewError=true;                                                                         //Steven 20120206 : RTC重複錯誤
                    if(IniConfig.bD49RTCAlarmSetIndexToErrBin)                                                          //JerryYang 20160712 for 力成,發生RTC Alarm時把Index上所有IC設為Errorbin
                    {
                        iCT=FTestSuck.CountRealIC();
                        AnsiString sBuffer;
                        sBuffer.printf("RTC Alarm set Arm1 Place to Error bin : Device=%d;", iCT);
                        MyDBIProcess("Message", sBuffer);
                        FTestSuck.SetAllRealIC2InterfaceBin();

                        iCT=BTestSuck.CountRealIC();
                        sBuffer.printf("RTC Alarm set Arm2 Place to Error bin : Device=%d;", iCT);
                        MyDBIProcess("Message", sBuffer);
                        BTestSuck.SetAllRealIC2InterfaceBin();
                    }
                    Task=65;                                                                                            //if RealTime NG ---->Next Start FullView
                }
            }
            break;
        case 65:
            if(REAL_TIME_CCD==true &&                                           //----- by dell ccd realtime-------------
               (MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
                MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false))
            {
                ShowIndexMotorError(AnsiString("DoTestYRear65"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestYRear 65"))
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //Ifor 20190712 need modify
                   TestIF_File.bArm1PickPlaceArm2Test==true     &&
                   TestIF_File.bRTC20CheckFunction==true        &&
                   bPlaceTestIC==true                           )               //Ifor 20190713 : add Arm1 丟下IC
                {
//                    COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
//                    COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                    COM2->DoReleaseAndInspEnd();
                    bRTCFullViewError=false;                                    //Steven 20120206 : RTC重複錯誤
                    Task=70;
                }
                else
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;        //----- by dell ccd realtime-------------
                    COM2->SendCommToVision(COM2->rtFullTOK, true);
                    iWaitIndexArm2.SetSecAndOn(10);
                    Task=66;
                }
            }
            break;
        case 66:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
//                RecordProcess("REALTIME CCD Full OK");
//                COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
//                COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                COM2->DoReleaseAndInspEnd();
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                if(IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden ||        //Sam 20240117 : OneCycle 完成做完 Full view check 後都需要做 RTC Learning golden
                   bTriggerRTC_AutoSTD)                                         //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
                {
                    fContact->InitROILearningTask();
                    Task=68;
                }
                else
                {
                    Task=70;
                }
                COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                bRetryRTC=false;
                break;
            }

            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                Task=67;
                break;
            }

            if(iWaitIndexArm2.Off())                                            //Steven 20110824 : Real time CCD - 不可以關閉CCD
            {                                                                   //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
                if(COM2->OpenRTCComPortAgain())                                 //ChungHung 20121005 add
                    ShowErrorMessage("WAR0335", 0, MTestY1, 0, __FUNC__);       //RTC Arm1 Error!

                bSendRealCCDSendStart=true;
                bSendRealCCDSendVerify=true;
                COM2->DoReleaseAndInspEnd();                                    //JerryYang 20220923 : add
                Task=65;
            }
            break;
        case 67:
            if(IndexAlarmInArmAway()==true)                                                                             //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                    //Steven 20151022 : add for MAXIM
                    bIsTestSitICFallDown=true;

                COM2->DoReleaseAndInspEnd();
                if(CosFunction.bRTCFullViewErrorOnlyRetry)                                                              //Steven 20140529 : RTCFullViewErrorOnlyRetry
                    ret=ShowErrorMessage("WAR0346", K_RETRY, MMCCD, bRTCFullViewError, ErrPart);                        //RTC FullView Socket Has Device Error!
                else
                    ret=ShowErrorMessage("WAR0346", K_RETRY|K_SKIP, MMCCD, bRTCFullViewError, ErrPart);                 //RTC FullView Socket Has Device Error!

                bRTCFullViewError=true;                                                                                 //Steven 20120206 : RTC重複錯誤

                if(ret==K_SKIP)                                                                                         //Steven 20120823 : Run Time出錯也要RTC
                {
                    fContact->InitROILearningTask();
                    bRTCFullViewError=false;
                    Task=68;
                }
                else
                {
                    Task=65;
                }
            }
            break;
        case 68:                                                                //Steven 20120823 : Run Time出錯也要RTC
            if(fContact->Do_ROILearning(true))
            {
                Task=70;
            }
            break;
        case 70:
            if(IniConfig.bEnableCCDUSETCPIP)
            {
                CCDInterfaceForm->CCDTimerOnOff(IniConfig.bC02InstallCCD);      //Steven 20110809
                if(IniConfig.bC02InstallCCD==true)
                    Task=72;
                else
                    Task=78;
                break;
            }
            else
            {
                Task=78;
                break;
            }
        case 72:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestYRear 72"))
            {
                fMain->lbCCDStatus->Visible=true;
                fMain->LightOn();
                DoTestYRearDelay.SetSecAndOn(3);
                iCCDTimeOutCount=0;
                if(CCDInterfaceForm->bAtestScanCCDProgram)
                {
                    CCDInterfaceForm->CloseCCDForm();
                    RecordProcess("Stop CCD check.");
                }
                Task=73;
            }
            break;
        case 73:
            if(DoTestYRearDelay.Off())
            {
                CCDInterfaceForm->CCDIdentificationOpen();
                CCDInterfaceForm->CCDIdentification();                          //kevin 20110811
                RecordProcess("Start CCD check.");
                DoTestYRearDelay.SetSecAndOn(20);                               //CCDTimeOutSec);  20110810 設定5秒太短會一直取像
                DoTestYRearDelay2.SetMSAndOn(200);
                Task=75;
            }
            break;
        case 75:
            if(DoTestYRearDelay2.Off()==false ||
               CCDInterfaceForm->bCCDProgramExistence==false)                   //ChungHung 20121127 add 等待程式開啟
            {
                if(DoTestYRearDelay2.Off()==true)
                {
                    DoTestYRearDelay2.SetMSAndOn(200);
                    CCDInterfaceForm->CCDIdentificationOpen();
                    DoTestYRearDelay.SetSecAndOn(20);                           //CCDTimeOutSec);  20110810 設定5秒太短會一直取像
                }
                return false;
            }

            if(CCDInterfaceForm->bIdentificationFinish==true)
            {
                if(CCDInterfaceForm->iIdentificationStatus==1)                  //0:未測試 1:Pass 2:Fail
                {
                    SW[SwCCDLight].Off();
                    Task=76;
                }
                else if(CCDInterfaceForm->iIdentificationStatus==2)
                {
                    Task=7500;
                }
                else
                {
                    Task=72;                                                    //jou 2012-12-06 修正 CCD 測試結果為0的時候會直接by pass
                }
                fShowMessage->FormClick(fShowMessage);
                fMain->lbCCDStatus->Visible=false;
                break;                                                          //kevin 20110811
            }

            if(DoTestYRearDelay.Off())
            {
                iCCDTimeOutCount++;
                CCDInterfaceForm->RetrunUpStep();
                if(iCCDTimeOutCount>(ChangeToFloatNonPcnt((double)(100), (double)(CCDTimeOutSec))))
                {
                    CCDInterfaceForm->CloseCCDForm();
                    RecordProcess("Stop CCD check.");
                    ShowMyMessage("CCD Time out", "CCD判斷時間過長!!");
                    iCCDTimeOutCount=0;
                }
                Task=73;
                break;
            }
            break;
        case 7500:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, 50000, "DoTestYRear 7500"))
            {
                RecordProcess("CCD check socket FAIL !!!!!!!!");
                fMain->Pause("CCD check socket fail");
                Task=70;
            }
            break;
        case 76:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, 50000, "DoTestYRear 76"))
            {
                iCCDTimeOutCount=0;
                Task=78;
            }
            break;
        case 78:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 50000, "DoTestYRear 78"))
            {
                CCDInterfaceForm->CCDTimerOnOff(false);                         //Steven 20110809
                Task=79;
            }
            break;
        case 79:
            if(CheckSocketSensor(1, "DoTestYRear_79_"))                         //Steven 20200617 : Socket sensor Alarm後,要再檢查一次
            {
                Task=50;                                                        //Steven 20200821 : 61 --> 50
            }
            else
            {
                Task=80;
            }
            break;
        case 80:
            if(bRecIndexDropAlarm1==true)                                       //JerryYang 20180122 (Steven) index arm掉料要用掉料的arm做index check
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, 50000, "DoTestYRear 80"))
                {
                    Task=10081;
                }
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, 50000, "DoTestYRear 80"))
                {
                    Task=81;
                }
            }
            break;
        case 10081:                                                                                                                                             //JerryYang 20180122 (Steven) index arm掉料要用掉料的arm做index check
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset-iIndexArmCheck_SG_Arm1, 50000, 0))                                      //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                IndexStatus=Z1Down_Z2Up;
                if(LastSet.bD41TestSocketICCheckSkip)                                                                                                           //打勾的話就跳過檢查
                {
                    bRecIndexDropAlarm1=false;
                    Task=78;
                    break;
                }

                fiosetview->ResetIndexSuck();

                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(INDEX_SUCKER_TYPE==0)
                        {
                            FTestSuck.Suck[i][j].On();
                        }
                        else                                                                                                                                    //負壓檢查
                        {                                                                                                                                       //有個盲點就是如果掉落IC剛好掉到關Site的地方，就會無法知道。
                            if(bTestSiteUse[0][i][j]==true)                                                                                                     //jou 20110504 //增加關Site時就不開真空偵測，
                            {
                                flag1=false;                                                                                                                    //jou 20110503 start
                                fiosetview->bIndexSuck[0][i][j]=true;
                                do
                                {
                                    if(flag1==false)
                                        flag1=fiosetview->ProcessIndexSuckDestroy1();

                                    MySleepEx(1, true);
                                }
                                while(flag1==false);                                                                                                            //jou 20110503 end
                            }
                        }
                    }
                }

                DoTestYRearDelay.SetSecAndOn(0.5);
                Task=10082;
                flag1=false;
            }
            break;
        case 10082:
            if(DoTestYRearDelay.Off() || INDEX_SUCKER_TYPE==1)                  //jou 20110503
            {
                Task=10084;
            }
            break;
        case 10084:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                                                                                                    //Steven 20140620 : 整合為Function

            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedFast))                              //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(CUSTOMER_CODE==CC_Greatek)                                                                                                                   //Wei 20160413
                    DoTestYRearDelay.SetSecAndOn(5);                                                                                                            //Steven 20110908 : 上來後也要Delay一下
                else
                    DoTestYRearDelay.SetSecAndOn(0.5);                                                                                                          //Steven 20110908 : 上來後也要Delay一下
                Task=10090;
            }
            break;
        case 10090:
            if(DoTestYRearDelay.Off())
            {
                bSucketHasICError=false;                                        //Steven 20101214 : 換位置
                if(LastSet.iRealDummy==REALLY)
                {
                    ErrPart=" ";
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(CheckTestSuckICOn(FTestSuck, i, j))
                            {
                                bSucketHasICError=true;
                                ErrPart+=IndexSuckName[i+iNN][j];
                            }
                        }
                    }
                }
                Task=10095;
            }
            break;
        case 10095:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==NULL_IC ||
                       FTestSuck.Item[i][j]==HAS_NULL_IC)
                    {
                        FTestSuck.Suck[i][j].Off();
                    }
                }
            }
            DoTestYRearDelay.SetSecAndOn(0.05);
            Task=10097;
            break;
        case 10097:
            if(DoTestYRearDelay.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==NULL_IC ||
                           FTestSuck.Item[i][j]==HAS_NULL_IC)
                        {
                            FTestSuck.Suck[i][j].Normal();                      //jou 2011-11-01 開破壞不可能同時開真空，所以OffDestroy -> Normal
                        }
                    }
                }

                if(bSucketHasICError)                                           // sucket has ic error
                {                                                               //Steven 20101214 : 換位置，換到Delay.Off()裡面
                    Task=50;
                    if(bSocketCheckSkip)
                    {
                        bRecIndexDropAlarm1=false;
                        Task=78;
                    }
                }
                else
                {
                    bRecIndexDropAlarm1=false;
                    Task=78;
                }
            }
            break;
        case 81:
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset-iIndexArmCheck_SG_Arm2, 50000, 0))                                      //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                IndexStatus=Z1Up_Z2Down;
                if(LastSet.bD41TestSocketICCheckSkip)                                                                                                           //打勾的話就跳過檢查
                {
                    Task=100;
                    break;
                }

                fiosetview->ResetIndexSuck();

                if(TestIF_File.bArm1PickPlaceArm2Test==false ||
                   ((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                     TestIF_File.bArm1PickPlaceArm2Test==true) &&                                                                                               //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bCheckArm2Vacuum==true))                                                                                                        //Steven 20150129 : 需要確認Arm2有沒有粘料
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(INDEX_SUCKER_TYPE==0)
                            {
                                BTestSuck.Suck[i][j].On();
                            }
                            else                                                                                                                                //負壓檢查
                            {                                                                                                                                   //jou 20170516 (Steven) : fix index drop error ,index check arm2失效.
                                if((BTestSuck.Item[i][j]==NULL_IC ||                                                                                            //有個盲點就是如果掉落IC剛好掉到關Site的地方，就會無法知道。
                                    BTestSuck.Item[i][j]==HAS_NULL_IC) &&                                                                                       //Steven 20210105 : 修正有IC的地方不檢查
                                   bTestSiteUse[1][i][j]==true)                                                                                                 //jou 20110504  //增加關Site時就不開真空偵測，
                                {
                                    flag1=false;                                                                                                                //jou 20110503 start
                                    fiosetview->bIndexSuck[1][i][j]=true;
                                    do
                                    {
                                        if(flag1==false)
                                           flag1=fiosetview->ProcessIndexSuckDestroy2();

                                        MySleepEx(1, true);
                                    }
                                    while(flag1==false);                                                                                                        //jou 20110503 end
                                }
                            }
                        }
                    }
                }

                DoTestYRearDelay.SetSecAndOn(0.5);
                Task=82;
                flag1=false;
            }
            break;
        case 82:
            if(DoTestYRearDelay.Off() || INDEX_SUCKER_TYPE==1)                  //jou 20110503
            {
                Task=84;
            }
            break;
        case 84:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                                                                                                    //Steven 20140620 : 整合為Function

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedFast))                              //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(CUSTOMER_CODE==CC_Greatek)                                                                                                                   //Wei 20160413
                    DoTestYRearDelay.SetSecAndOn(5);                                                                                                            //Steven 20110908 : 上來後也要Delay一下
                else
                    DoTestYRearDelay.SetSecAndOn(0.5);                                                                                                          //Steven 20110908 : 上來後也要Delay一下
                Task=90;
            }
            break;
        case 90:
            if(DoTestYRearDelay.Off())
            {
                bSucketHasICError=false;                                        //Steven 20101214
                if(LastSet.iRealDummy==REALLY)
                {
                    ErrPart=" ";
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(CheckTestSuckICOn(BTestSuck, i, j))
                            {
                                bSucketHasICError=true;
                                ErrPart+=IndexSuckName[i][j];
                            }
                        }
                    }
                }
                Task=95;
            }
            break;
        case 95:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==NULL_IC ||
                       BTestSuck.Item[i][j]==HAS_NULL_IC)
                    {
                        BTestSuck.Suck[i][j].Off();
                    }
                }
            }
            DoTestYRearDelay.SetMSAndOn(500);
            Task=97;
            break;
        case 97:
            if(DoTestYRearDelay.Off())
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]==NULL_IC ||
                           BTestSuck.Item[i][j]==HAS_NULL_IC)
                        {
                            BTestSuck.Suck[i][j].Normal();                      //jou 2011-11-01 開破壞不可能同時開真空，所以OffDestroy -> Normal
                        }
                    }
                }

                if(bSucketHasICError)                                           // sucket has ic error
                {                                                               //Steven 20101214 : 換位置，換到Delay.Off()裡面
                    Task=50;
                    if(CUSTOMER_CODE==CC_AMKOR_China ||                         //Steven 20101112
                       IniConfig.bKoreaFunction ||
                       CUSTOMER_CODE==CC_QUALCOMM)                              //JerryYang 20170412 (Steven) add QUALCOMM
                    {
                        if(bSocketCheckSkip)
                        {
                            bSocketCheckSkip=false;
                            Task=100;
                        }
                    }
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 100:
            if(CheckIndexStatus("DoTestYRear100"))
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true    &&
                    bCheckGiveWay==true                         )
                {
                    Prod.TestY2_Middle      =iBackupTestY2_Middle;
                    Prod.TestZ2_Test        =iBackupTestZ2_Test;
                    Prod.TestZ2_Drop_Offset =iBackupTestZ2_Drop;
                    iBackUpZ2DownPosition   =Prod.TestZ2_Test;                  //JerryYang 20220923 : add
                    bCheckGiveWay=false;
                }

                if(ATC_SYSTEM==eATCSiliconType &&
                   Temperature.bATCActiveCooling==true &&                       //jou 2012-03-14 增加ATC控制方式
                   Temperature.bATCTestStrat==true &&
                   (TestIF.iTestMode==DualSite ||
                    TestIF.iTestMode==SingleSite))
                {
                    if(bATCInitialFinish==false ||
                       COM2->ATCAlarmSenCheck()==false)
                    {
                        ShowMyMessage("ATC Alarm Sensor Off, please check ATC system is OK!", "ATC警報偵測, 請確認ATC系統是否正常!");
                        return false;
                    }
                }

                iRetry=0;
                //==> Eastsun 20260511 F007 整合: Ifor 20220308+20230524 KLT 兩段式上升 (Rear 版 TestZ2_Up_Offset)
                if(CUSTOMER_CODE == CC_KYEC_LEE                                    &&
                   bEnable_KLT_Function==true                                      &&
                   DeviceForm.ContactMode==DropContactModeDiffentSpeed             &&
                   Prod.TestZ2_Up_Offset!=0                                        ) //Ifor 20220308 add + 20230524 add 設定不為0才走
                {
                    Task=20000;
                }
                //<== Eastsun 20260511 F007 整合
                else if(IniConfig.bD21EnableFinishTestUpWait &&                 //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
                   IndexStatus==Z1Up_Z2Down)                                    //Steven 20180613 (Jou) : 增加保護機制, 避免撞機
                {
                    Task=105;
                }
                else
                {
                    Task=108;
                }
                goto DoTestYRear_ReStart;                                       //2008/10/20 lee
            }
            break;
        case 105:
            if(DeviceForm.ContactMode==DropContact ||                           //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveDrop   ||
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                iFinishTestUpWaitHeight=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+IniConfig.iD21FinishTestUpWaitHeight;
            }
            else
            {
                iFinishTestUpWaitHeight=Prod.TestZ2_Test+IniConfig.iD21FinishTestUpWaitHeight;
            }

            if(MOT[MTestZ2].Gali_MotMove(iFinishTestUpWaitHeight, 10000))
            {
                Task=106;
                DoTestYRearDelay.SetMSAndOn(IniConfig.iD21FinishTestUpWaitTime);
            }

            if(Task!=106)                                                       //Steven 20180813 : add index arm speed
                break;
        case 106:
            if(DoTestYRearDelay.Off())
            {
                Task=108;
            }

            if(Task!=108)                                                       //Steven 20180813 : add index arm speed
                break;
        case 108:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung_K12 &&
               LastSet.iTemperature==Tempture_Hot &&
               bFirstZ1UPZ2Down &&
               TestIF_File.bUseSocketFloat==false)                              //kevin 20131112 加熱模式預先動作避免第一顆溫度過高
            {
                bCheckShuttle=false;
                Task=110;
            }
            else
            {
                if(MOT[MTestZ1].Gali_ReadPos()!=Prod.TestZ1_Safe)
                {
                    if(IndexStatus==Z1Down_Z2Up)
                        Task=110;
                    else
                        Task=109;
                }
                else
                {
                    if(IndexStatus==Z1Up_Z2Down &&
                       REAL_TIME_CCD==true &&                                   //----- by dell ccd realtime-------------
                       !COM2->bCCDDummyRum &&
                       TestIF_File.bUseSocketFloat==false)                      //Frank QQ
                    {
                        if(bRealCCDSendArm)
                        {
                            COM2->SendCommToVision(COM2->rtArmIndex1, true);
                            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                               TestIF_File.bArm1PickPlaceArm2Test==true     &&
                               TestIF_File.bRTC20CheckFunction==true        &&
                               TestIF_File.bRTC20GiveWayCheck==false        )
                            {
                                COM2->SendCommToVision(COM2->rtCHECKNULL, true);
                            }
                            iWaitIndexArm2.SetMSAndOn(200);
                            bRealCCDSendArm=false;
                        }
                    }
                    Task=110;
                }
            }
            goto DoTestYRear_ReStart;                                           //2008/10/20 lee
        case 109:
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Safe, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, 0))
            #endif
            {
                if(IndexStatus==Z1Up_Z2Down &&
                   REAL_TIME_CCD==true &&                                       //----- by dell ccd realtime-------------
                   !COM2->bCCDDummyRum &&
                   TestIF_File.bUseSocketFloat==false)                          //Frank QQ
                {
                    if(bRealCCDSendArm)
                    {
                        COM2->SendCommToVision(COM2->rtArmIndex1, true);
                        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                           TestIF_File.bArm1PickPlaceArm2Test==true     &&
                           TestIF_File.bRTC20CheckFunction==true        &&
                           TestIF_File.bRTC20GiveWayCheck==false        )
                        {
                            COM2->SendCommToVision(COM2->rtCHECKIC, true);
                        }
                        iWaitIndexArm2.SetMSAndOn(200);
                        bRealCCDSendArm=false;
                    }
                }

                if(Prod.bIndexUpSpeed)
                    DoTestYRearDelay.SetSec(20);                                //kevin 20190913 add
                Task=110;
            }

            if(Task!=110)                                                       //Steven 20180813 : add index arm speed
                break;
        case 110:                                                               //kevin 20170524 (wei) two speed up
            QueueTaskList[23].CheckTaskChange();                                //Steven 20200821 : 使用Goto也要記錄Task變化    //Steven 20201201 : 19 --> 23
            CheckSocketSensor(1, "DoTestYRear_110", true);                      //Z2上升時, 檢查socket sensor
            iAZ1=MOT[MTestY2].Gali_ReadPos();
            iAZ2=abs(Prod.TestY2_Middle-iAZ1);                                  //kevin 2019404 add 測區位置 > 5有問題
            iAZ3=abs(Prod.TestY2_Rear-iAZ1);

            if(Prod.bIndexUpSpeed)
            {
                if(iAZ2<=5)                                                     //kevin 20190404
                {
                    if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+300, MOT[MTestZ2].GailSpeed*Prod.TestZ_Drop_Speed/100))
                    {
                        if(InputShuttleRearHasIC() && InShtInLF(1))             //KaiChen 20171225 (Steven)：Add Speed Display
                            IndexAddSpeedDisplay();
                        else
                            IndexSubSpeedDisplay();
                        Task=115;
                    }
                }
                else if(iAZ3<=5)                                                //kevin 20190404
                {
                    if(InputShuttleRearHasIC() && InShtInLF(1))                 //KaiChen 20171225 (Steven)：Add Speed Display
                        IndexAddSpeedDisplay();
                    else
                        IndexSubSpeedDisplay();
                    Task=115;
                }
            }
            else
            {
                if(InputShuttleRearHasIC() && InShtInLF(1))                     //KaiChen 20171225 (Steven)：Add Speed Display
                    IndexAddSpeedDisplay();
                else
                    IndexSubSpeedDisplay();
                Task=115;
            }
            bIndexFinish=false;

            if(IniConfig.bD38IndexPutICToShtNoWaitMotion &&                     //Steven 20181228 : Add Index Action
               IniConfig.bIndexArm2SupplyLight==false    &&
               TestIF_File.bForEgisTecTest==false        &&
               TestIF_File.bArm1PickPlaceArm2Test==false)
            {
                bZ1DownZ2UpAndPlace=CheckPlaceOutShuttle(1);                    //ChungHung 20171116 modify for Index Action
            }

            if(Task!=115)                                                       //Steven 20180813 : add index arm speed
            {
                break;
            }
            else
            {
                if(CosFunction.bSortingBy2DList==true &&
                   LastSet.iTester==_2D_SORT &&
                   TestIF_File.bSortingBy2DIDList==true)                        //Frank 20221122 : 2DID sorting for ATK
                {
                }
                else
                {
                    if((iAZ2<=5 || iAZ2>5) && iAZ1!=Prod.TestY2_Middle)
                    {
                        sBufferT.sprintf("Arm 2 Prod.TestY2_Middle:%d    ReadPos():%d ", Prod.TestY2_Middle ,iAZ1);
                        SaveFile(asHandlePath, sBufferT);                       //kevin 20190411 add log
                    }
                    else if((iAZ3<=5 || iAZ3>5) && (iAZ1!=Prod.TestY2_Rear))
                    {
                        sBufferT.sprintf("Arm 2 Prod.TestY2_Rear:%d    ReadPos():%d ", Prod.TestY2_Rear ,iAZ1);
                        SaveFile(asHandlePath, sBufferT);                       //kevin 20190411 add log
                    }
                }
            }
        case 115:
            QueueTaskList[23].CheckTaskChange();                                                                                                                //Steven 20200821 : 使用Goto也要記錄Task變化
            if(bGetTime1)
            {
                bGetTime1=false;
                bGetTime2=true;
//                ShowIndexTime(3);                                             //Steven 20140619 : 測試
            }

            str="check socket sensor : ";                                                                                                                       //kevin 20130504 socket sensor
            if(USE_IO_CHANGE_TOQUE==true)                                                                                                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            if(bCheckShuttle==false)
            {
                if(CheckShuttlePos())
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(CUSTOMER_CODE==CC_ASE_KaohSiung_K12 &&
                   (LastSet.iTemperature==Tempture_Hot) && bFirstZ1UPZ2Down)                                                                                    //kevin 20131112 加熱模式預先動作避免第一顆溫度過高
                {
                    bFirstZ1UPZ2Down=false;                                                                                                                     //kevin 20131112 加熱時z1在shuttle 1上面 z2在下
                }
                else if(MOT[MTestZ1].Gali_ReadPos()!=Prod.TestZ1_Safe)
                {
                    if(IndexStatus!=Z1Down_Z2Up)
                    {
                        ShowIndexTime(-2);                                                                                                                      //Steven 20200715 : 重新計算Cycle Time
                        Task=108;
                        break;
                    }
                }
                bCheckShuttle=true;
            }

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                                                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(IndexStatus==Z1_Z2_Normal)
                {
                    HHandler2Gpib.iLotStatus=TestIF_File.iTestMode;                                                                                             //Ifor 20190321 : add Test Mode & ATC Enables Site Send to GPIB
                    #ifdef AMD_Version
                    sprintf(HHandler2Gpib.UseSiteMapData, aSendSiteMapping.c_str());                                                                            //Ifor 20201030 add:送Site Mapping 資料給GPIB
                    #else
                    if(TestIF.iGpibMode==InterfaceType_Delta_Castle)                                                                                            //Steven 20260428 : Delta Castle 強制送SiteMap (取代AMD_Version compile flag)
                    {
                        memset(HHandler2Gpib.UseSiteMapData, '\0', sizeof(HHandler2Gpib.UseSiteMapData));
                        sprintf(HHandler2Gpib.UseSiteMapData, aSendSiteMapping.c_str());
                    }
                    #endif
                    if(DeviceForm.ContactMode==DropContact)
                    {
                        fObserver->AddTimeData(18, DropContactTimer1.LatchCycleTime()/1000.0);                                                                  //JerryYang 20170425 (wei) 第一段時間, 測試完成到另一支arm下降到drop高度
                        DropContactTimer2.LatchCycleTime(true);
                    }

                    if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                                                         //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                        TestIF_File.bArm1PickPlaceArm2Test==true) &&
                        TestIF_File.bCheckArm2Vacuum==true)                                                                                                     //Steven 20150129 : 需要確認Arm2有沒有粘料
                    {
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                BTestSuck.Suck[i][j].Normal();                                                                                                  //關掉吹氣
                            }
                        }
                    }

                    bGetTime1=true;
                    bGetTime2=true;

                    bIndexArm1PickupErrStop=false;
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    bCheckShuttle=false;
                    bOneTimeFlag=true;
                    ShowIndexTime(-2);
                    ShowMainScreenPresure(0);                                                                                                                   //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                    bRearHeadICFallDown=false;
                    Task=200;
                }
            }
            else
            {
                if(IndexStatus==Z1Down_Z2Up)
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    bCheckShuttle=false;
                    bOneTimeFlag=true;
                    if(USE_ReadIndex_TOQUE==false &&
                       TestIF_File.bEnableReadAndCheckTorque==false)                                                                                            //kevin 20210804 change
                        ShowMainScreenPresure(0);                                                                                                               //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
    //                ShowIndexTime(-2);
                    bRearHeadICFallDown=false;
                    ShowIndexTime(-2);                                                                                                                          //Steven 20200715 : 重新計算Cycle Time
                    Task=200;
                    goto DoTestYRear_ReStart;                                                                                                                   //2008/10/20 lee
                }
                else if(IndexStatus==Z1_Z2_Normal)
                {
                    if(TestIF_File.bUseSocketFloat)
                    {
                        ShowIndexTime(-2);                                                                                                                      //Steven 20200715 : 重新計算Cycle Time
                        Task=116;
                        break;
                    }
                    else
                    {
                        lPos[0]=IndexStatus;                                                                                                                    //kevin 20150915
                        RecordIndexPositionError("DoTestYRear()Z1_Z2_Normal", true, false, false, false, &lPos[0]);                                             //kevin 20150915 record
                        ShowIndexMotorError(AnsiString("DoTestYRear115"));
                        return false;
                    }
                }
                else
                {
                    if(IniConfig.bVTESTFunction==true &&                                                                                                        //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
                       IniConfig.bGetRcsCheckingResult==true)
                    {
                        if(LastSet.iRunStartMode!=rsmAutoSiteMap &&
                           fMesSystem->asGetRcsCheckingResult!="PASS")
                        {
                            Task=50;
                            return false;
                        }
                    }

//                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&    //JerryYang 20241122 : 一丟一測也要偵測drop error
//                       TestIF_File.bArm1PickPlaceArm2Test==true)
//                    {
//                    }
//                    else
                    {                                                                                                                                           //jou 2011-08-16 +100 -> +500 drop mode容易造成誤判
                        iIndexZPos=MOT[MTestZ2].Gali_ReadPos();                                                                                                 //jou 2012-01-17 +500 -> +750 疑似有誤判的情況發生，一直來來回回Hang up (備注 : 750條 = 7.5mm )
                        if((iIndexZPos>(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+750)) ||
                           (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0))                                                                                  //2008/10/20 lee //Ifor 20210716 add:避免關ARM不偵測掉料
                        {
                            flag=false;
                            if(LastSet.iRealDummy==REALLY)
                            {
                                if(DeviceForm_File.dDropByPassDetect!=0 &&
                                   (iIndexZPos>(Prod.TestZ2_Test+DeviceForm_File.dDropByPassDetect*100)))
                                {
                                }
                                else
                                {
                                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                                       TestIF_File.bArm1PickPlaceArm2Test==true &&
                                       bPlaceTestIC==true)                                                                                                      //JerryYang 20241122 : 一丟一測也要偵測drop error
                                    {
                                    }
                                    else
                                    {
                                        for(int i=0; i<BTestSuck.iShtRow; i++)
                                        {
                                            for(int j=0; j<BTestSuck.iShtCol; j++)
                                            {
                                                if(BTestSuck.Suck[i][j].Enable       &&
                                                    BTestSuck.Suck[i][j].SenUsing!="" &&
                                                    BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                                    BTestSuck.Item[i][j]!=NULL_IC)                                                                              //Steven 20110725 : 不再使用IsSuckICFallDown
                                                {
                                                    if(BTestSuck.Suck[i][j].GetStatus()==false)
                                                    {
                                                        BTestSuck.Suck[i][j].Normal();                                                                          //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                                        flag=true;
                                                        bRecIndexDropAlarm2=true;                                                                               //jou 2012-01-17 紀錄index Drop alarm
                                                        //==> Eastsun 20260511 F007 整合: Ifor 20220125 add: Index 掉料當下 馬達位置設定 (Index2 / Rear)
                                                        if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==true)
                                                        {
                                                            MOT[MTestZ2].Gali_Command("ST");
                                                            StopAllMotor();
                                                            iArm1_Y=MOT[MTestY1].Gali_ReadPos();
                                                            iArm1_Z=MOT[MTestZ1].Gali_ReadPos();
                                                            iArm2_Y=MOT[MTestY2].Gali_ReadPos();
                                                            iArm2_Z=MOT[MTestZ2].Gali_ReadPos();

                                                            StrPos.sprintf("Index2 %d %d Drop :ARM1_Y_%d_Z_%d_ARM2_Y_%d_Z_%d", i, j, iArm1_Y, iArm1_Z, iArm2_Y, iArm2_Z);
                                                            RecordProcess(StrPos);
                                                        }
                                                        //<== Eastsun 20260511 F007 整合
                                                        }
                                                }

                                                if(FTestSuck.Suck[i][j].Enable       &&                                                                         //jou 2012-04-24 增加IC掉落檢查
                                                    FTestSuck.Suck[i][j].SenUsing!="" &&
                                                    FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                                    FTestSuck.Item[i][j]!=NULL_IC)
                                                    {
                                                        if(FTestSuck.Suck[i][j].GetStatus()==false)
                                                        {
                                                            FTestSuck.Suck[i][j].Normal();                                                                      //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                                            flag=true;
                                                            bRecIndexDropAlarm1=true;                                                                           //jou 2012-01-17 紀錄index Drop alarm
                                                            //==> Eastsun 20260511 F007 整合: Ifor 20220125 add: Index 掉料當下 馬達位置設定 (Index1 / Rear 對稱記錄)
                                                            if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==true)
                                                            {
                                                                MOT[MTestZ1].Gali_Command("ST");
                                                                StopAllMotor();
                                                                iArm1_Y=MOT[MTestY1].Gali_ReadPos();
                                                                iArm1_Z=MOT[MTestZ1].Gali_ReadPos();
                                                                iArm2_Y=MOT[MTestY2].Gali_ReadPos();
                                                                iArm2_Z=MOT[MTestZ2].Gali_ReadPos();

                                                                StrPos.sprintf("Index1 %d %d Drop :ARM1_Y_%d_Z_%d_ARM2_Y_%d_Z_%d", i, j, iArm1_Y, iArm1_Z, iArm2_Y, iArm2_Z);
                                                                RecordProcess(StrPos);
                                                            }
                                                            //<== Eastsun 20260511 F007 整合
                                                            }
                                                    }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        for(int i=0; i<Gali_MaxAxis; i++)
                        {
                            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                                                                                           //JimmyChiu 20220708 : add Index Arm Axis
                               (MTestY1+i)==MTestY2)
                            {
                                continue;
                            }

                            MOT[MTestY1+i].Gali_ScanMotStatusTIMO();
                            if(MOT[MTestY1+i].Led[iAlarmLed] ||
                               MOT[MTestY1+i].Led[iServoalarmLed] ||
                               MOT[MTestY1+i].Led[iServoOn]==false)
                            {
                                iHome=1;
                                IndexMotorBreakerOFF();
                                MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:115"));
                                if(REAL_TIME_CCD==true)
                                {
                                    ScanBtnThd->Stop();
                                }
                                Task=50;
                                str.sprintf("Motor:%s, Alarm:%d, ServoAlarm:%d, Servo On:%d", MOT[MTestY1+i].Alias, MOT[MTestY1+i].Led[iAlarmLed], MOT[MTestY1+i].Led[iServoalarmLed], MOT[MTestY1+i].Led[iServoOn]);
                                ShowErrorMessage("WAR0349", 0, MMSystem, 0, str);                                                                               //JerryYang 20240111 : 改成跳alarm code
                                fHome->GaliMotorServoOff("DoTestYRear");                                                                                        //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
                                return false;
                            }
                        }

                        if(flag)
                        {
                            bRearHeadICFallDown=true;
                            MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:115"));
                            if(REAL_TIME_CCD==true)
                            {
                                ScanBtnThd->Stop();
                            }
                            str1.sprintf("DoTestYRear 115 to 50: bRecIndexDropAlarm1=%d, bRecIndexDropAlarm2=%d", bRecIndexDropAlarm1?1:0, bRecIndexDropAlarm2?1:0);
                            RecordProcess(str1);
                            Task=50;
                            return false;
                        }
                        else
                        {
                            bRecIndexDropAlarm1=false;                                                                                                          //jou 2012-01-17 紀錄index Drop alarm
                            bRecIndexDropAlarm2=false;                                                                                                          //jou 2012-01-17 紀錄index Drop alarm
                        }
                    }

                    if(CheckSocketSensor(1, "DoTestYRear_115_"))                                                                                                //Z1DownZ2Up //Steven 20200615 : Socket Sensor整合成Function
                    {
                        Task=50;
                        return false;
                    }

                    if(IniConfig.bI26TestCloseSiteHaveBin &&
                       bTestBinDataError!=0)                                                                                                                    //kevin 20150202 需強致將arm上 ic取出
                    {
                        Task=50;
                        RecordProcess("[I26] Test Close Site Have Bin Error!", __FUNC__);                                                                       //Steven 20201201 : add event log for debug
                        return false;
                    }

                    if(bOneTimeFlag)
                    {
                        bOneTimeFlag=false;
                        if(TestIF_File.bUseSLKClamp &&
                           TestIF_File.iSeparabilityTest==1 &&
                           TestIF_File.bArm1PickPlaceArm2Test==false)                                                                                           //JerryYang 20180518 (wei) : add 分離式SLK ARM1丟IC ARM2分離CLAMP流程
                        {
                            iBackUpZ1DownPosition=Prod.TestZ1_Test+1500;                                                                                        //JerryYang 20160523 Z1分離高度要上升1500
                        }
                        else
                        {
                            iBackUpZ1DownPosition=Prod.TestZ1_Test;
                        }

                        FTestSeparateSLK(true);                                                                                                                 //JerryYang 20180518 (wei) : 分離流程初始化
                        BTestSeparateSLK(true);
                        iRealCCDSendArmCT=0;                                                                                                                    //jou 2013-08-23 修正RTC 斷訊不會Alarm的錯誤.
                    }

                    if(REAL_TIME_CCD==true &&                                                                                                                   //----- by dell ccd realtime-------------
                       !COM2->bCCDDummyRum &&
                       TestIF_File.bUseSocketFloat==false)
                    {
                        if(bRealCCDSendArm)
                        {
                            COM2->SendCommToVision(COM2->rtArmIndex1, true);
                            iWaitIndexArm2.SetMSAndOn(200);
                            bRealCCDSendArm=false;
                        }

                        if(Sen[SnRealTimeCCDIndexArm].IsOn())
                        {
                            if(iWaitIndexArm2.Off())
                            {
                                bRealCCDSendArm=true;
                                iRealCCDSendArmCT++;                                                                                                            //jou 2013-08-23 修正RTC 斷訊不會Alarm的錯誤.
                                if(iRealCCDSendArmCT>15)
                                {
                                    iRealCCDSendArmCT=0;                                                                                                        //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
                                    if(COM2->OpenRTCComPortAgain())                                                                                             //ChungHung 20121005 add
                                        ShowErrorMessage("WAR0336", 0, MMIndex, 0, __FUNC__);
                                    else
                                        RecordProcess("WAR0336 auto retry com port.", __FUNC__);                                                                //Steven 20201201 : add event log for debug
                                    bSendRealCCDSendStart=true;                                                                                                 //Sam 20250220 :　修正一直報警 RTC WAR0335 RTC Arm1 Error! 問題
                                    COM2->DoReleaseAndInspEnd();                                                                                                //JerryYang 20220923 : add
                                    bRealCCDSendArm=true;
                                }
                                return false;
                            }
                            return false;
                        }
                        iRealCCDSendArmCT=0;                                                                                                                    //jou 2013-08-23 修正RTC 斷訊不會Alarm的錯誤.
                        fLotInfo->ALed2->Value=true;                                                                                                            //Steven 20110916
                        ScanBtnThd->Start();
                    }
                    //---------------------------------------

                    if(bGetTime2)
                    {
                        bGetTime2=false;
    //                    ShowIndexTime(4);                                     //Steven 20140619 : 測試
                    }

                    if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)                                                                                              //----- by dell ccd realtime-------------
                    {
                        if(ScanBtnThd->GetArmState())
                        {
                            #ifdef RTCErrorType
                            if(bCheckThirdPos)
                            {
                                if(iRTCErrorSend==false)        //wei 20221222 RTC ARM Error
                                {
                                    COM2->SendCommToVision(COM2->rtErrorType, false);
                                    iRTCErrorSend=true;
                                    tRTCErrorTimeOut.SetSecAndOn(20);
                                }
                            }
                            #endif

                            if(bOnce==false)                                                                                                                    //JerryYang 20220923 : add RTC timeout
                            {
                                bOnce=true;
                                hFRTCTimeOutDelay.SetSecAndOn(15.0);
                            }

                            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
                            {
                                MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(", case:115"));
                                bRearHeadICFallDown=true;
                                ScanBtnThd->SetArmState(false);
                                iRTCErrorSend=false;                            //JerryYang 20260306 : fix沒有清除flag
                                RecordProcess("RTC alarm has IC", __FUNC__);                                                                                    //Steven 20201201 : add event log for debug
                                Task=50;
                            }
                            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm1NG])      //wei 20221222 RTC ARM Error
                            {
                                MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(", case:115"));
                                ScanBtnThd->SetArmState(false);
                                iRTCErrorSend=false;
                                RecordProcess("RTC alarm Arm 1 NG", __FUNC__);    //Steven 20201201 : add event log for debug
                                Task=50;
                            }
                            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm2NG])      //wei 20221222 RTC ARM Error
                            {
                                MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(", case:115"));
                                ScanBtnThd->SetArmState(false);
                                iRTCErrorSend=false;
                                RecordProcess("RTC alarm Arm 2 NG", __FUNC__);    //Steven 20201201 : add event log for debug
                                Task=50;
                            }
                            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut])
                            {
                                COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut]=false;
                                ShowErrorMessage("WAR0341", 0, MMIndex, 0, __FUNC__);                                                                           //RTC Grab TimeOut Error!
                                COM2->SendCommToVision(COM2->rtRelease, false);
                                MySleep(100);
                                ScanBtnThd->SetArmState(false);
                            }
                            else
                            {
                                if(hFRTCTimeOutDelay.Off())                                                                                                     //JerryYang 20220923 : add RTC timeout
                                {
                                    ShowMyMessage("RTC Arm1 half view check timeout error");
                                    bOnce=false;
                                }
                            }
                            #ifdef RTCErrorType
                            if(bCheckThirdPos)
                            {
                                if(tRTCErrorTimeOut.Off())      //wei 20221222 RTC ARM Error
                                {
                                    iRTCErrorSend=false;
                                    ShowErrorMessage("WAR0356", 0, MMIndex, (bRTCArm1HalfViewError || bRTCArm2HalfViewError), __FUNC__);
                                }
                            }
                            #endif
                            return false;
                        }
                    }
                    bOnce=false;                                                                                                                                //JerryYang 20220923 : add RTC timeout

                    if(IniConfig.bIndexPickupErrStop==false)
                    {
                        bIndexArm1PickupErrStop=false;
                        bIndexArm2PickupErrStop=false;
                    }

                    if(DeviceForm.ContactMode==TMove ||
                       DeviceForm.ContactMode==TMoveDrop ||                                                                                                     //jou 2012-02-03 新增T Move Drop
                       DeviceForm.ContactMode==TMoveDropSlowContact ||
                       DeviceForm.ContactMode==TMoveSlowContact)                                                                                                //Steven 20160130 : TMove Soft contact
                    {
                        TMode=true;
                    }
                    else
                    {
                        if(IniConfig.bIndexPickupErrStop==true &&
                           bIndexArm1PickupErrStop==true)                                                                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                        {
                            TMode=true;
                        }
                        else
                        {
                            TMode=false;
                        }
                    }

                    if(DeviceForm_File.ContactMode==DirectContactSoftEP ||                                                                                      //kevin 20130608 soft contact 到測區上面2mm 開通ep 充氣
                       DeviceForm_File.ContactMode==DropContactSoftEP)                                                                                          //JerryYang 20151202 add DropContactSoftEP
                    {
                        if(MOT[MTestZ1].Gali_ReadPos()<(Prod.TestZ1_Test+1500))                                                                                 //JerryYang 20231205 : SoftEP高度由+1000改為+1500避免撞到guide pin
                        {
//                            bContSoftEpSwitch(0, false);                        //ARM2 先浮動頭充氣    //JerryYang 20151202 false->true
                            EPSwitchOnOff(eEPSwArm2);
                            if(DeviceForm.fAireForce==0)
                                DeviceForm.fAireForce=DeviceForm.dPress;                                                                                        //kevin 20220215 EP 會被設為 0 ，恢復生產重量
                            ADAM_WriteVoltage(DeviceForm.fAireForce);                                                                                           //JerryYang 20210119 修正誤用die force造成錯誤
                        }
                    }

                    if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                                                         //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                        TestIF_File.bArm1PickPlaceArm2Test==true) &&
                        TestIF_File.bCheckArm2Vacuum==true)                                                                                                     //Steven 20150129 : 需要確認Arm2有沒有粘料
                    {
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                BTestSuck.Suck[i][j].Off();                                                                                                     //要把IC吹掉
                            }
                        }
                    }

                    if(bintered2==true)                                                                                                                         //Isaac 20200922 : 紀錄indexArmY encoder值和command值
                    {
                        bintered2=false;
                        RecordIndexPosition(2, 1);                                                                                                              //Isaac 20200922 : 紀錄indexArmY encoder值和command值，Arm2/Socket
                        EncoderTeachingMaxMinCount(2);                                                                                                          //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
                    }

                    #ifdef INDEX_PROTECT_TMOVE
                    if(bOverRangeDoTMode==true && bTriger4Indexhome==false)                                                                                     //Isaac 20201012 : index Y超過範圍，做一次Tmode
                    {
                        bTriger4Indexhome=true;
                        bOverRange4Indexhome=true;
                        TrigerIndexAxisHome();                                                                                                                  //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                        return false;
                    }
                    #endif

                    ShowIndexTime(-1);                                                                                                                          //Steven 20200715 : 重新計算Cycle Time

                    #ifndef SOFT_SIMULTE
                    if(TorqueUseHPComCard &&
                       TestIF_File.bEnableReadAndCheckTorque)                                                                                                   //KenHsieh 20220317 : 確認有Get value後Test Arm才能上升
                    {
                        if(COM2->bGetValue)
                            bIndexFinish=MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, TMode, bIndexArm1PickupErrStop);                                       //JimmyChiu 20220708 : add Index Arm Axis MTestY2 --> MTestY1
                    }
                    else
                    #endif
                    {
                        bIndexFinish=MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, TMode, bIndexArm1PickupErrStop);                                           //JimmyChiu 20220708 : add Index Arm Axis
                    }

                    //==> Eastsun 20260511 F007 整合: Ifor 20220317 add: KYEC 要求 溫度還沒到不執行
                    if(CUSTOMER_CODE==CC_KYEC_LEE                                  &&
                       LastSet.iTemperature==Tempture_Hot                          &&
                       fHeaterOK==false                                            )
                    {
                        bIndexFinish=false;
                    }
                    //<== Eastsun 20260511 F007 整合
                    if(bIndexFinish)
                    {
                        if(CUSTOMER_CODE==CC_KYEC_LEE && USE_BU5_Function==true && asCheckContactOver[1]!="") //Frank 20260710 ADD //Eastsun 20260710整合
                        {
                            ShowMyMessage(asCheckContactOver[1]);
                            asCheckContactOver[1]="";
                        }
                        bIndex1Suck=false;                                                                                                                      //kevin 20220105 Index 在下真空建立 pause 不能關閉
                        bintered2=true;
                        if(DeviceForm_File.ContactMode==DirectContactSoftEP ||
                           DeviceForm_File.ContactMode==DropContactSoftEP)                                                                                      //kevin 20220616 add EP //JerryYang 20151202 add DropContactSoftEP
                        {
//                            bContSoftEpSwitch(0, true);                         //ARM2 先浮動頭充氣     //JerryYang 20151202 false->true
                            EPSwitchOnOff(eEPSwBoth);
                            if(DeviceForm.fAireForce==0)
                               DeviceForm.fAireForce=DeviceForm.dPress;                                                                                         //kevin 20220215 EP 會被設為 0 ，恢復生產重量
                            ADAM_WriteVoltage(DeviceForm.fAireForce);                                                                                           //JerryYang 20210119 修正誤用die force造成錯誤
                        }

                        if(ATC_SYSTEM==eNewATCSystem &&                                                                                                         //Ifor 20190321 : add Test Mode & ATC Enables Site Send to GPIB
                           Temperature.bATCActiveCooling==true &&
                           TestIF_File.i2DIDFormat==eAMD)                                                                                                       //JerryYang 20200422 2DID format選項改用下拉選單
                        {
                            for(int i=0; i<MAX_SOCKET_TOTAL; i++)
                            {
                                HHandler2Gpib.Site[i]=bATC_EnablesChannel[i];
                            }
                        }
                        HHandler2Gpib.iLotStatus=TestIF_File.iTestMode;

                        if(ATC_SYSTEM==eNewATCSystem &&
                           Temperature.bATCActiveCooling==true)                                                                                                 //JerryYang 20220815 : send ATC which ARM
                        {
                            ATC_InterfaceForm->HandlerArm(0);
                        }
                        iRTCErrorCount=0;                                       //wei 20221222 RTC ARM Error
                        iWhichArmDown=1;                                                                                                                        //JerryYang 20200316 add SVID 哪支arm下壓在測區
                        fContact->ATC_SwitchTjSignal(1, false);                                                                                                 //Ifor 20210622 add: ATC Switch TJ

                        if(DeviceForm.ContactMode==DropContact)
                        {
                            fObserver->AddTimeData(18, DropContactTimer1.LatchCycleTime()/1000.0);                                                              //JerryYang 20170425 (wei) 第一段時間, 測試完成到另一支arm下降到drop高度
                            DropContactTimer2.LatchCycleTime(true);
                        }

                        if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                                                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                            TestIF_File.bArm1PickPlaceArm2Test==true) &&
                            TestIF_File.bCheckArm2Vacuum==true)                                                                                                 //Steven 20150129 : 需要確認Arm2有沒有粘料
                        {
                            for(int i=0; i<BTestSuck.iShtRow; i++)
                            {
                                for(int j=0; j<BTestSuck.iShtCol; j++)
                                {
                                    BTestSuck.Suck[i][j].Normal();                                                                                              //關掉吹氣
                                }
                            }
                        }

                        bGetTime1=true;
                        bGetTime2=true;

                        bIndexArm1PickupErrStop=false;
                        MOT[MInShuttle1].fCanMoveM=true;
                        MOT[MInShuttle2].fCanMoveM=true;
                        bCheckShuttle=false;
                        bOneTimeFlag=true;
                        ShowIndexTime();
                        ShowMainScreenPresure(0);                                                                                                               //jou 2010-06-23 畫面Z1,Z2 encoder 顯示

                        bRearHeadICFallDown=false;
                        Task=200;
                        if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
                        {
                            bRTCArm2HalfViewError=false;                                                                                                        //Steven 20120206 : RTC重複錯誤
                            COM2->SendCommToVision(COM2->rtArmFinish, false);
                            ScanBtnThd->Stop();
                        }

                        if(IniConfig.bTModeMotorFree==true)
                        {
                            if(TMode==true)                                                                                                                     //20110923 Tmode 煞車
                            {
                                hTestZ1Delay.SetSecAndOn(0.1);
                                Task=130;
                            }
                        }

                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                                                     //JerryYang 20170523 (wei) ReadEPData會造成index cycle time增加
                        {
                            bReadEpTime=true;                                                                                                                   //kevin 20170524 add ep read change time
                        }

                        if(ATC_SYSTEM==eATCHonPrecType &&
                           ATC_SYSTEM==eATCHonPrecType)                                                                                                         //Dell 20140509
                        {
                            ATCInterfaceForm->ATC_SYS.SetNowArm(0);                                                                                             //ATC
                        }

                        if(ATC_SYSTEM==eATC60 || ATC_SYSTEM==eATC30)                                                                                            //20141204 ChungHung add for ATC3.0 //2014-05-30    Dell    for ATC6.0
                        {
                            ATCInterfaceForm->ATC_60_SYS.SetHandlerNowArm(0);
                        }

                        #ifdef ASE_KaohSiung
                            if(TestIF_File.bEnableReadAndCheckTorque==true &&                                                                                   //KaiHuang 20201222 : Index Check 後,用第一次 Contact 的扭力值當標準
                               FTestSuck.UseSiteHasIC()==true)                                                                                                  //kevin 20210804 change
                            {
                                iReadTorqueError=0;
                                fMain->edTorue0->Text="";
                                bReadArm1_Torque=true;
                                hTorqueDelay.SetSecAndOn(TestIF_File.dReadTorqueDelayTime);                                                                     //kevin 20210804 等待時間去讀取扭力 change by setup
                            }
                        #endif
                        goto DoTestYRear_ReStart;                                                                                                               //2008/10/20 lee
                    }
                }
            }
            break;
        case 130:
            if(hTestZ1Delay.Off())
            {
                SW[SwFMotorBreaker].Off();
                Task=200;
                goto DoTestYRear_ReStart;
            }
            break;
        case 140:
            if(hTestZ1Delay.Off())
            {
                MOT[MTestZ1].ServoOnOff(false);
                Task=200;
                goto DoTestYRear_ReStart;
            }
            break;
        case 200:
            if(DeviceForm_File.ContactMode==DirectContactSoftEP ||
               DeviceForm_File.ContactMode==DropContactSoftEP)                  //kevin 20130608 Soft Contact mode
            {
                SoftContactTim.SetMSAndOn(500);
            }

            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].On();
            }

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //JerryYang 20230322 : 2D SORT模式index arm不用下壓到socket
            {
                Task=2091;
                break;
            }

            if(Prod.TestZ1_Drop_Offset!=0)                                      //DropContact or DirectContactModeDiffentSpeed
            {
                Task=300;
                break;
            }

            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Task=300;
            }
            else
            {
                if(USE_ReadIndex_TOQUE &&
                   TestIF_File.bEnableReadAndCheckTorque)                       //kevin 20210804 change
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque1=false;
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                    fMain->edTorue0->Text="";
                }
                else if(TestIF_File.bEnableReadAndCheckTorque)                  //kevin 20210804 change
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque1=false;
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                    fMain->edTorue0->Text="";
                    bRetryReadToqu=false;                                       //kevin 20210419 重讀扭力
                }

                bReadTorqueOK=false;                                            //kevin 20211129 read torque ok
                Task=2091;
                fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                           //Steven 20150304 : Add GPIB LOG
            }
            goto DoTestYRear_ReStart;                                           //Steven 20180813 : add index arm speed
//            break;
        case 209:
            QueueTaskList[23].CheckTaskChange();                                //Steven 20200821 : 使用Goto也要記錄Task變化
            if(DeviceForm_File.ContactMode==DropContactSoftEP)
            {
//                bContSoftEpSwitch(0, true);                                     //ARM1 先浮動頭充氣  //JerryYang 20151202 flase->true
                EPSwitchOnOff(eEPSwBoth);
                SoftContactTim.SetMSAndOn(500);
            }
            Task=2091;
//            break;                                                            //Steven 20180813 : add index arm speed
        case 2091:
            QueueTaskList[23].CheckTaskChange();                                //Steven 20200821 : 使用Goto也要記錄Task變化

            if(CUSTOMER_CODE==CC_KYEC_CHEN &&
               DeviceForm.VacuumMode==VacuumOFFMode)                            //jou 2015-07-22 新增Index Vacuum Off mode
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        FTestSuck.Suck[i][j].Normal();
                    }
                }
            }

            if(DeviceForm_File.ContactMode==DirectContactSoftEP ||              //kevin 20130608 Soft Contact mode
               DeviceForm_File.ContactMode==DropContactSoftEP)
            {
                if(SoftContactTim.Off())
                    Task=2092;
                else
                    break;                                                      //Steven 20141022 : Add Index Speed
            }
            else
            {
                Task=2092;
            }
        case 2092:
            QueueTaskList[23].CheckTaskChange();                                //Steven 20200821 : 使用Goto也要記錄Task變化
            if(bNeedUpDonwOneTome==true)
            {
                bNeedUpDonwOneTome=false;
                Task=1000;
                break;
            }

            HangTime.SetSecAndOn(Prod.iHangupMaxTime);                          //Steven 20090827 Start: Hang Up dectector
            bHangTimePause=false;

            if(CosFunction.bAfterInitialDelayUseOtherArm &&                     //JerryYang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
               Prod.bUseOtherArmToTestAfterInitialDelay &&
               bArm1IsTest==false && bFirst==true)                              //JerryYang 20180817 (Steven) : fix EOT delay
            {
                if(fFrontNeedTest==true && FTestSuck.HasRealIC())               //JerryYang 20220923 : 修正換arm預熱問題
                {
                    bFirst=false;                                               //JerryYang 20180817 (Steven) : fix EOT delay
                    if(bArm1Delay==false && bArm2Delay==false)                  //JerryYang 20200909 修正預熱功能,Arm1下壓->Arm2預熱-> Arm 1測試
                    {
                        CheckInitialStartDelayInSocket();
                        if(bNeedInitialTestDelay==true)
                        {
                            if(InArmSuck.HasIC()            ||
                               BLCarryKit.UseSiteHasIC()   ||
                               BTestSuck.HasNotTestYet()    ||
                               bInArmWaitOneCycle==false    ||                  //JerryYang 20220923 : 修正換arm預熱問題
                              (iOneCycle==0 &&
                               (MOT[MMPlate1].HasIC() ||
                                MOT[MMPlate2].HasIC())))                        //Sam 20240911 : 避免 HotPlate 還在預熱時就直接測試
                            {
                                bArm2Delay=true;
                            }
                        }
                    }
                    else
                    {
                        if(bNeedInitialTestDelay==true)
                        {
                            if(bArm1Delay==true)
                            {
                                dwStartInitialCount.LatchCycleTime(true);
                            }
                        }
                    }
                }
            }

            BTorqueTimeOutDelay.Set0_1SecAndOn(50);                             //kevin 20210823 扭力 time
            Task=210;
        case 210:                                                                                                       // 編號不得改變 *******************
            QueueTaskList[23].CheckTaskChange();                                                                        //Steven 20200821 : 使用Goto也要記錄Task變化
            if(bHangTimePause==true || bRunAutoClean)                                                                   //Steven 20090827 Start: Hang Up dectector DoTestYRear
            {
                Task=209;
                break;
            }

            if(DeviceForm.ContactMode==DropContact &&
               CosFunction.bSuckDevicesDuringTest==true &&                                                              //JerryYang 20170804 (Steven) 移除邊吸邊測的選項
               INDEX_SUCKER_TYPE==1)                                                                                    //JerryYang 20170522 (wei) drop contact改為邊吸邊測
            {
                bCheckSuck=fiosetview->ProcessIndexSuckDestroy1();
            }
            else
            {
                bCheckSuck=true;
            }

            if(fFrontNeedTest)
            {
                if(TestIF_File.bForEgisTecTest==true && bSwitchArm2)                                                    //Steven 20140922 : Arm2當作指紋測試
                {
                    Task=11000;
                    bSwitchArm2=false;
                    break;
                }

                if(CosFunction.bAfterInitialDelayUseOtherArm &&                                                         //JerryYang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
                   Prod.bUseOtherArmToTestAfterInitialDelay &&
                   bArm1IsTest==false)
                {
                    if(bNeedInitialTestDelay==true)
                    {
                        if(bArm1Delay==true)                                                                            //JerryYang 20200909 修正預熱功能,Arm1下壓->Arm2預熱-> Arm 1測試
                        {
                            if(fRearNeedDestroy)                                                                        //JerryYang 20190626 預熱時另一支arm要能吸放IC
                            {
                                bZ1PickShuttle=false;                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                                bZ2PickShuttle=false;                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                                fRearNeedDestroy=!DoRearTestDestroyIC(true);                                            //ChungHung 20111115
                            }
                            else if(fRearNeedSuck)
                            {
                                if(DeviceForm.bSuckShuttleDeviceAfterTested &&
                                   (LastSet.iTemperature==Tempture_Hot ||
                                    LastSet.iTemperature==Tempture_AmbientHot) &&                                       //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
                                   DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)                              //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                                {
                                    if(fFrontNeedTest==false)
                                        fRearNeedSuck=!DoRearTestSuckIC();
                                }
                                else if(DeviceForm.bSuckShuttleDeviceAfterTested &&                                     //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                                        (LastSet.iTemperature==Tempture_Ambient ||
                                         LastSet.iTemperature==Tempture_AmbientHot) &&                                  //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
                                        CUSTOMER_CODE==CC_KYEC_LEE &&
                                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)
                                {
                                    if(fFrontNeedTest==false)
                                        fRearNeedSuck=!DoRearTestSuckIC();
                                }
                                else
                                {
                                    fRearNeedSuck=!DoRearTestSuckIC();
                                }
                            }

                            if(iInitialCount>0)
                            {
                                iInitialCount=(Prod.iInitialDelay)-(dwStartInitialCount.LatchCycleTime()/1000);
                                break;
                            }

                            str.sprintf("Arm 1 intial delay finish, device: %d ea", FTestSuck.CountRealIC());
                            RecordProcess(str);
                            bArm1Delay=false;
                            fFrontNeedTest=false;
                            bNeedInitialTestDelay=false;
                            bFirstTest=true;                                                                            //JerryYang 20180817 (Steven) : fix EOT delay
                            break;
                        }
                        else if(bArm2Delay==true)                                                                       //換arm 2再做預熱
                        {
                            fFrontNeedTest=false;
                            break;
                        }
                    }
                }

                if(TestIF.bUseSLKClamp==true &&
                   TestIF_File.iSeparabilityTest==1 &&
                   bSLKClampProcessFinish==false)                                                                       //JerryYang 20160524 分離模式
                {
                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                  //JerryYang 20180518 (wei) : 分離式SLK ARM1丟IC ARM2分離CLAMP流程
                       TestIF_File.bArm1PickPlaceArm2Test==true)                                                        //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    {
                        if(FTestSeparateSLK()==true)                                                                    //JerryYang 20160429 (wei) 分離模式, Z2 SLK與Clamp分離流程
                        {
                            bSLKClampProcessFinish=true;
                            bArm2NeedCombine=true;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if(BTestSeparateSLK()==true)                                                                    //JerryYang 20160429 (wei) 分離模式, Z1 SLK與Clamp分離流程
                        {
                            bSLKClampProcessFinish=true;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                bArm1IsTest=true;                                                                                       //JerryYang 20180629 (wei) : 用來判斷是否在測試中
                fFrontNeedTest=!DoFTestSuckTestIC();                                                                    //做測試頭下降至測試點測試動作
                if(bCheckSuck==true)                                                                                    //JerryYang 20170522 drop contact改為邊吸邊測)  //JerryYang 20170522 drop contact改為邊吸邊測
                {
                    CheckIndexAllSuckICFallDown(true, false);                                                           //Steven 20110725 : 修改負壓檢查方式
                }
            }
            else                                                                                                        //JerryYang 20200909 修正預熱功能,Arm1下壓->Arm2預熱-> Arm 1測試
            {
                bArm1Delay=false;
            }

            if(fRearNeedDestroy)
            {
                bZ1PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                bZ2PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                fRearNeedDestroy=!DoRearTestDestroyIC(true);                                                            //ChungHung 20111115
            }
            else if(fRearNeedSuck)
            {
                if(DeviceForm.bSuckShuttleDeviceAfterTested &&
                   (LastSet.iTemperature==Tempture_Hot ||
                    LastSet.iTemperature==Tempture_AmbientHot) &&                                                       //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
                   DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)                                              //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                {
                    if(fFrontNeedTest==false)
                        fRearNeedSuck=!DoRearTestSuckIC();
                }
                else if(DeviceForm.bSuckShuttleDeviceAfterTested &&
                        (LastSet.iTemperature==Tempture_Ambient ||
                         LastSet.iTemperature==Tempture_AmbientHot) &&                                                  //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
                        CUSTOMER_CODE==CC_KYEC_LEE &&                                                                   //KaiChen 20180125 (Steven) ：京元-竹南 Suck Shuttle Device After Tested 功能，開啟常溫使用
                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)                                         //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                {
                    if(fFrontNeedTest==false)
                        fRearNeedSuck=!DoRearTestSuckIC();
                }
                else
                {
                    fRearNeedSuck=!DoRearTestSuckIC();
                }
            }

            if(DeviceForm.ContactMode==DropContact &&
               CosFunction.bSuckDevicesDuringTest==true &&                                                              //JerryYang 20170522 drop contact改為邊吸邊測
               bCheckSuck==true)                                                                                        //JerryYang 20170804 (Steven) 移除邊吸邊測的選項
            {
                bFTestSuckDrop=false;
            }

            if(fRearNeedDestroy==false &&
               fRearNeedSuck==false &&
               fFrontNeedTest==false &&
               fFrontNeedSuckIC==false &&
               bCheckSuck==true)                                                                                        //JerryYang 20170522 (wei) drop contact改為邊吸邊測
            {
                bEPfirst=true;                                                                                          //Ifor 20150803 : 新增EP量測旗標，第一次才執行Log紀錄
                bFirst=true;                                                                                            //JerryYang 20180817 (Steven) : fix EOT delay
                bSLKClampProcessFinish=false;
                bArm1IsTest=false;                                                                                      //JerryYang 20180629 (wei) : 用來判斷是否在測試中
                iWhichArmDown=0;                                                                                        //JerryYang 20200316 add SVID 哪支arm下壓在測區
                if(USE_IO_CHANGE_TOQUE==true)                                                                           //jou 2012-06-21 Enable index I/O Change Toque
                {
                    SW[SwIndexChangeToque1].Off();
                    SW[SwIndexChangeToque2].Off();
                }

                if(IniConfig.bTModeMotorFree==true)
                {
                    SW[SwFMotorBreaker].On();                                                                           //20110923 Tmode 煞車
                    MySleep(200);
                }

                iFlag=CheckAnyCaseNeedToDoArm2();                                                                       //Steven 20190115 : SCC要求吸取異常要檢查Socket
                if(iFlag>1)
                {
                    Task=iFlag;
                    if(iFlag==211 || iFlag==215 || iFlag==11040)                                                        //Ifor 20191119 :Fix Arm1 Pick/place Arm2 測試後Arm1不吸料問題
                    {
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(FTestSuck.Item[i][j]!=NULL_IC &&
                                   FTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    FTestSuck.Suck[i][j].Error=false;
                                    bFTestSuckUse[i][j]=true;
                                }
                                bFTSuckFinish[i][j]=false;                                                              //Steven 20110301
                            }
                        }
                    }
                }
                else if(iFlag==1)
                {
                    if(USE_ReadIndex_TOQUE &&
                       TestIF_File.bEnableReadAndCheckTorque)                                                           //kevin 20210804 change
                    {
                        ShowMainScreenPresure(0);                                                                       //kevin 20201027  //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                        CheckAndRecodrTorque(0);
                    }

                    #ifdef ASE_KaohSiung
                        if( TestIF_File.bEnableReadAndCheckTorque)                                                      //kevin 20210804 change
                        {
                            if(TestIF_File.iShuttleMode==1 &&
                               TestIF_File.iShuttle_Sel==1)                                                             //kevin 20210304
                            {
                                return true;
                            }
                            else
                            {
                                if(NewCheckAndRecodeTorque(0)==false)
                                {
                                    if(bRetryReadToqu==false)                                                           //kevin 20210419 重讀扭力
                                    {
                                        bRetryReadToqu=true;
                                        bNeedCheckIndexToque=false;                                                     //KEVIN 20210419 ADD TORQUE
                                        bNeedCheckIndexToque1=false;
                                        fMain->chkReadTorque1->Checked=true;
                                        fMain->chkReadTorque2->Checked=false;
                                        COM2->ReadIndexTorqueSetting(0);
                                    }

                                    if(BTorqueTimeOutDelay.Off()==false)                                                //kevin 2021018 扭力
                                       return false;
                                }
                            }
                        }
                        ShowMainScreenPresure(0);
                    #endif
                    bIndex1Suck =false;                                                                                 //kevin 20220105 Index 在下真空建立 pause 不能關閉
                    return true;
                }
            }

            if(IniConfig.bRecordSkipPosition)                                                                           //jou 20150320   auto skip 次數時間導致Hang up
            {
                if(bAutoSkipFlag==true && ArmSpeed[InArm].bAutoSKIP==true)
                {
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    bAutoSkipFlag=false;
                }
            }

            if(EnableTraymapCheckFunction(0) &&
               EnableTraymapCheckFunction(1))                                                                           //JerryYang 20250120 : fix誤發hang up
            {
                if(bAutoSkipFlag==true)
                {
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    bAutoSkipFlag=false;
                }
            }

            if(bAskStopPort[ePortLoader] ||                                                                             //JerryYang 20250529 : avoid hang up
               bAskStopPort[ePortAuto1] ||
               bAskStopPort[ePortAuto2] ||
               bAskStopPort[ePortAuto3] ||
               bWaitingAMR==true)                                                                                       //RogerYang 20250617 Load已清空但還沒滿bundle，等料車來
            {
                HangTime.SetSecAndOn(Prod.iHangupMaxTime);
            }

            if(HangTime.Off() &&
               TestISTimeOut==false &&
               bHangTimePause==false)
            {
                if(AMR.NeedAMRTransport())                                                                              //Sam 20250423 : AMR 搬運花費時間太久可能會報警需要 By Pass
                {
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    break;
                }

                if(IniConfig.bTModeMotorFree==true)
                {
                    SW[SwFMotorBreaker].On();                                                                           //20110923 Tmode 煞車
                    MySleep(200);
                }

                if(CheckHeaterOK()==false)                                                                              //Steven 20250116 : 確認HeaterOK
                {
                    bHangTimePause=true;
                    return false;                                                                                       //kevin 20161102 break;
                }

                bNoUseAutoRecord=true;                                                                                  //wei 20160311

                #ifdef SOFT_SIMULTE
                Task=209;
                break;
                #else
                RecordProcess("Auto State Record by DoTestYRear");
                fMain->DoStateRecord(0, false);                                                                         //Steven 20120705 : Hang Up時，會自動存畫面     //KenHsieh 20230116 : 區分手動或自動(sbclick -> Function)

                #ifdef DEBUG_HANGUP_NO_HOME
                    ShowErrorMessage("JAM0317", K_SKIP, MTestZ2);
                    Task=209;
                #else
                    iHangupCTArm2++;
                    if(iHangupCTArm2>1)
                    {
                        iHangupCTArm2=0;
                        ShowErrorMessage("JAM0317", K_SKIP, MTestZ2);
                        Task=209;
                    }
                    else
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                             //kevin 20160722 ASE 高雄取消
                        {
                            ShowErrorMessage("JAM0317", K_SKIP, MTestZ1);
                            Task=209;
                        }
                        else
                        {
                            MyDBIProcessNew("Motion", "WAR2206", "Auto homing", "0317");                                //Steven 20120705 : Hang Up時，會自動存畫面
                            bHomeByStart=true;
                            fAllMotorHome=false;
                        }
                    }
                #endif
                break;
                #endif
            }
            break;
        case 211:                                                                                                       //ChungHung 20150528 add for 海思 _8Site1x4
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick, MOT[MTestZ1].GailSpeed))                                     //Z1 上升至安全位置
            {
                Task=212;
            }
            else
            {
                break;
            }
        case 212:                                                               //ChungHung 20150528 add for 海思 _8Site1x4
            if(MOT[MTestY1].Gali_MotMove(Prod.TestY1_Middle, MOT[MTestY1].GailSpeed))
            {
                Task=213;
            }
            else
            {
                break;
            }
        case 213:                                                               //ChungHung 20150528 add for 海思 _8Site1x4
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed))
            {
                Task=215;
            }
            else
            {
                break;
            }
        case 215:
            bCheckAllSuck=true;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTSuckFinish[i][j]==false)                              //Steven 20110301
                    {
                        if(bFTestSuckUse[i][j])
                        {
                            if(FTestSuck.Suck[i][j].Suck())
                            {
                                bFTSuckFinish[i][j]=true;                       //Steven 20110301
                                bFTestSuckUse[i][j]=false;
                            }
                            else if(FTestSuck.Suck[i][j].Error)                 //jou 2011-08-16
                            {
                                bFTSuckFinish[i][j]=true;                       //Steven 20110301
                            }
                            else
                            {
                                bCheckAllSuck=false;
                            }
                        }
                        else
                        {
                            bFTSuckFinish[i][j]=true;                           //Steven 20110301
                        }
                    }
                }
            }

            flag=true;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTSuckFinish[i][j]==false)
                        flag=false;
                }
            }

            if(flag)                                                            //Steven 20110301
            {
                if(bCheckAllSuck)
                {
                    bHasICinSocket=false;
                    return true;
                }
            }
            break;
        case 220:
            DoTestYRearDelay.SetMSAndOn(OverEncoderDelay);
            Task=230;
//             break;
        case 230:
            if(DoTestYRearDelay.Off())
            {
                if(TestZ1OutRandge())
                {
                    DoTestYRearDelay.SetMSAndOn(1);
                    Task=240;
                    iRetry=0;
                }
                else
                {
                    return true;
                }
            }
            else                                                                //Steven 20180813 : add index arm speed
            {
                break;
            }
        case 240:
            if(DoTestYRearDelay.Off())
            {
                if(TestZ1OutRandge2())
                {
                    DoTestYRearDelay.SetSecAndOn(1);
                    Task=240;
                    iRetry++;
                    if(iRetry<=10)
                        break;
                    else
                        ShowMyMessage("Test Head Ecncoder error, maybe press too low!", "Index錯誤，可能是壓力太低!!", "DoTestYRear 240");
                }
                TestZ1SetPos();
                Task=1;
                return true;
            }
            break;
        case 300:
            if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||         //Eliot 2011_0318 Start
               DeviceForm.ContactMode==DirectContactSoftEP ||                   //Steven 20160530
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
            }
            else
            {
                bFTestSuckDrop=true;
                fFrontNeedSuckIC=false;                                         //jou 2011-12-13 丟測才需要設成true

                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                           FTestSuck.Item[i][j]==HAS_IC)
                        {
                            FTestSuck.Suck[i][j].Off();

                            if(IniConfig.bD44CheckIndexICDestroy)               //JerryYang 20241122 : 啟用D44功能時，INDEX ARM丟料後會持續吹氣，一直到INDEX ARM上抬到丟料高度
                            {
                                FTestSuck.bNeedCheck[i][j]=true;                //Destroy finish
                                iIndexTakeDeviceChk1=2;                         //kevin 20190103 回吸檢測狀態
                            }
                        }
                    }
                }
            }

            if(CosFunction.bTesterSidePushFunction==true &&                     //Richard 20220321 : 渠梁Side Push
               DeviceForm.bTesterSidePush==true)
            {
                if(Cylinder[C_TesterSidePush].Enable==true)
                {
                    if(Cylinder[C_TesterSidePush].OffStatus()==false)
                    {
                        #ifndef SOFT_SIMULTE
                        ShowErrorMessage("JAM16130", K_RETRY, MMSystem, false, sTesterSidePush);
                        break;
                        #endif
                    }
                }
            }

            if(TestIF_File.iShuttleMode==0 ||
               (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) ||
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true && TestIF_File.bArm1PickPlaceArm2Test==true))  //JerryYang 20260508 : fix 一丟一測沒有delay
            {
                DoTestYRearDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);             // delay 0.3 sec for ic down        //Steven 20140909 : 換到迴圈外面
            }
            else
            {
                DoTestYRearDelay.SetSecAndOn(0);
            }

            Task=310;
            break;
        case 310:                                                                                                       //丟測放下IC
            bCheckDestroy=true;
            if(bCheckDestroy==true && DoTestYRearDelay.Off())
            {                                                                                                           //Steven 20210617 : Add for 修正Soft Contact的Delay Time
                if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||                                             //Eliot 2011_0318 Start //ChungHung 20150528 Mark
                   DeviceForm.ContactMode==DirectContactSoftEP ||                                                       //Steven 20160530
                   DeviceForm.ContactMode==TMoveSlowContact)                                                            //Steven 20160130 : TMove Soft contact
                {
                    Task=320;
                }
                else
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                               FTestSuck.Item[i][j]==HAS_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }

                    if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                 //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                        TestIF_File.bArm1PickPlaceArm2Test==true) &&
                        TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                //Steven 20150129 : 需要確認Arm2有沒有粘料
                    {
                        if(TestSocket.HasRealIC()==false)
                        {
                            TestSocket.CopyFrom(FTestSuck);                                                             //JerryYang 20250723 : add
                            FTestSuck.SetAllToNullIC();                                                                 //JerryYang 20250722 : add
                        }
                    }

                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                       TestIF_File.bArm1PickPlaceArm2Test==true)                                                        //JerryYang 20241122 : 啟用D44功能時，INDEX ARM丟料後會持續吹氣，一直到INDEX ARM上抬到丟料高度
                    {
                        if(IniConfig.bD44CheckIndexICDestroy)
                        {
                            for(int i=0; i<MAX_Index_Row; i++)
                            {
                                for(int j=0; j<NEW_MAX_Index_Col; j++)
                                {
                                    if(FTestSuck.Item[i][j]==HAS_HOT_IC || FTestSuck.Item[i][j]==HAS_IC)
                                    {
                                        FTestSuck.Suck[i][j].Destroy();
                                    }
                                }
                            }
                        }
                    }
                    bIndexCheckNoStopVaccum=false;
                    bHasICinSocket=true;

                    if(IniConfig.bIndexArm2SupplyLight==true ||                                                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                 //kevin 20150127 Arm1 下壓 arm2 測試
                        TestIF_File.bArm1PickPlaceArm2Test==true))                                                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    {
                        Task=11000;
                        bPlaceTestIC=true;                                                                              //Ifor 20190713 : add Arm1 丟下IC
                    }
                    else
                    {
                        if(IniConfig.bC08_SocketSensor &&
                           TestIF_File.bEnSocketSensor &&
                           TestIF_File.bCheckSocketFloating)                                                            //Steven 20180313 : 使用Socket Sensor驗證置偏
                        {
                            Task=311;
                            break;
                        }
                        else
                        {
                            Task=320;
                        }
                    }
                }

                if(CosFunction.bTesterSidePushFunction==true &&                                                         //Richard 20220321 : 渠梁Side Push
                   DeviceForm.bTesterSidePush==true)
                {
                    DoTestYRearDelay.SetSecAndOn(DeviceForm_File.dSitePushWaitTime);                                    //JerryYang 20240111 : add
                    #ifdef SOFT_SIMULTE
                        Task=17900;
                        break;
                    #else
                    if(Cylinder[C_TesterSidePush].Enable==true)
                    {
                        Task=17900;
                        break;
                    }
                    #endif
                }
                //<==
            }
            break;
        case 311:                                                               //Steven 20180313 (Jou) : 使用Socket Sensor驗證置偏
            flag=false;
            str="";
            for(int i=0; i<TestIF_File.iSocketCount; i++)
            {
                if(TestIF_File.iSensorCheckType[i]==2 &&                        //Steven 20200420 : Socket Sensor功能可以選
                   Sen[SThreadPara.iSocketSensor[i]].IsOn())
                {
                    MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:311"));
                    flag=true;
                    str+=IntToStr(i+1)+",";
                }
            }

            if(flag)
            {
                sBufferT="Z1DownZ2Up: Z1Pos "+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000)+"Z2Pos "+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000);
                RecordProcess(sBufferT);                                        //kevin 20150506
                str1.sprintf("%s, case 311", __FUNC__);
                RecordProcess(str1);
                Task=312;
                return false;
            }
            else
            {
                Task=320;
            }
            break;
        case 312:                                                               //丟測放下IC後,偵測到置偏,Index往上
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 50000, "DoTestYRear 312"))
            {
                Task=313;
            }
            break;
        case 313:                                                               //Y軸分開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestYRear 313"))
            {
                Task=314;
            }
            break;
        case 314:                                                               //丟測放下IC後,偵測到置偏,Alarm
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)            //Steven 20151022 : add for MAXIM
                    bIsTestSitICFallDown=true;
                bIsSocketSensor=true;

                ShowErrorMessage("WAR0323", K_RETRY, MTestZ1, false, str);
                Task=315;
            }
            break;
        case 315:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, 50000, "DoTestYRear 315"))
            {
                Task=316;
            }
            break;
        case 316:                                                               //丟測放下IC後,偵測到置偏排除後, Index往下移動
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, ((MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed)/100)))
            {
                Task=311;
            }
            break;                                                              //Steven 20180313 (Jou) : 使用Socket Sensor驗證置偏
        case 320:
            if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||                                                 //Eliot 2011_0318 Start
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveSlowContact ||                                                              //Steven 20160130 : TMove Soft contact
               DeviceForm.ContactMode==TMoveDropSlowContact)
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, ((MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed)/100)))
                {
                    if(TestIF_File.bForEgisTecTest==true)                                                               //Steven 20140922 : Arm2當作指紋測試
                    {
                        Task=10500;
                    }
                    else
                    {
                        if(DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
                           DeviceForm.ContactMode==TMoveDropSlowContact)
                        {
                            Task=330;                                                                                   //Steven 20160518 : Fixed for DropContactModeDiffentSpeed
                        }
                        else
                        {
                            Task=209;
                            fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                                                       //Steven 20150304 : Add GPIB LOG
                        }
                    }
                }
            }
            else if(DeviceForm.ContactMode==DropPlaceShiftContact)                                                      //ChungHung 20150528 add for 海思 _8Site1x4
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick, MOT[MTestZ1].GailSpeed))                                 //Z2 上升至安全位置
                {
                    Task=325;
                }
            }
            else
            {
                if(CosFunction.bTesterSidePushFunction==true &&
                   DeviceForm.bTesterSidePush==true &&
                   DoTestYRearDelay.Off()==false)                                                                       //JerryYang 20240111 : add
                {
                    break;
                }

                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed/100))
                {
                    if(DeviceForm.ContactMode==DropContact)
                    {
                        fObserver->AddTimeData(19, DropContactTimer2.LatchCycleTime()/1000.0);                          //JerryYang 20170503 (wei) 第二段drop contact計時,吹氣drop wait time+移動約2mm到contact高度
                        DropContactTimer3.LatchCycleTime(true);

                        if(CosFunction.bTesterSidePushFunction==true &&                                                 //Richard 20230301 : 渠梁Side Push 新模式
                           DeviceForm.bTesterSidePush==true &&
                           DeviceForm_File.iSidePushMode==0)
                        {
                            DoTestYRearDelay.SetSecAndOn(DeviceForm_File.dSitePushWaitTime);
                            #ifdef SOFT_SIMULTE
                                Task=18100;
                                break;
                            #else
                            if(Cylinder[C_TesterSidePush].Enable==true)
                            {
                                Task=18100;
                                break;
                            }
                            #endif
                        }
                    }

                    if(TestIF_File.bForEgisTecTest==true)                                                               //Steven 20140922 : Arm2當作指紋測試
                    {
                        Task=10500;
                    }
                    else
                    {
                        Task=330;                                                                                       //Steven 20141218 : 209 --> 330 修正丟測模式異常
                    }
                }
            }
            break;
        case 325:                                                               //ChungHung 20150528 add for 海思 _8Site1x4
            if(MOT[MTestY1].Gali_MotMove(Prod.TestY1_Middle-TestIF.dSiteYPitch, MOT[MTestY1].GailSpeed))
            {
                Task=326;
            }
            break;
        case 326:                                                                                                       //ChungHung 20150528 add for 海思 _8Site1x4
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed))             //ChungHung 20150528 Mark Index2 下壓至測試高度
                Task=330;
            break;
        case 330:
            if(CosFunction.bTesterSidePushFunction==true && DeviceForm.bTesterSidePush==true && DoTestYRearDelay.Off()==false)                                  //JerryYang 20240111 : add
            {
                break;
            }

            if(USE_ReadIndex_TOQUE && TestIF_File.bEnableReadAndCheckTorque)                                                                                    //kevin 20210804 change
            {
                bNeedCheckIndexToque=false;
                bNeedCheckIndexToque1=false;
                fMain->chkReadTorque1->Checked=true;
                fMain->chkReadTorque2->Checked=false;
                fMain->edTorue0->Text="";
                //bOverHappen=false;
            }

            bIndex1Suck=true;                                                                                                                                   //kevin 20220105 Index 在下真空建立 pause 不能關閉
            if(DeviceForm.VacuumMode==VacuumONMode)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                           FTestSuck.Item[i][j]==HAS_IC)
                        {
                            if(INDEX_SUCKER_TYPE==1)                                                                                                            //Steven 20111202
                            {
                                fiosetview->bIndexSuck[0][i][j]=true;
                                bIndexCheckNoStopVaccum=true;                                                                                                   //Sam 20220816 : 增加保護避免暫停 AllSite 掉料
                            }
                            else
                            {
                                FTestSuck.Suck[i][j].On();                                                                                                      //Sam 20230110 : 修正連續 Start<>Stop Drop Contact 掉料問題
                            }
                        }
                    }
                }
            }

            if(DeviceForm.ContactMode==DropContact &&                                                                                                           //JerryYang 20170804 (Steven) 移除邊吸邊測的選項
               CosFunction.bSuckDevicesDuringTest==true)                                                                                                        //JerryYang 20170522 (wei) drop contact改為邊吸邊測
            {
                fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                                                                                                           //Steven 20150304 : Add GPIB LOG
                Task=209;
                break;
            }

            DoTestYRearDelay.SetMSAndOn(100);                                                                                                                   // delay 0.3 sec for ic down
            Task=340;
            break;
        case 340:
            if(INDEX_SUCKER_TYPE==1)
            {
                bCheckSuck=fiosetview->ProcessIndexSuckDestroy1();
            }
            else
            {
                bCheckSuck=true;
            }

            if(bCheckSuck==true && DoTestYRearDelay.Off())
            {
                bIndexCheckNoStopVaccum=false;                                  //Sam 20220902 : 修正 All Site 掉料
                bFTestSuckDrop=false;
                fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                           //Steven 20150304 : Add GPIB LOG
                Task=209;
            }
            break;
        case 350:
            bFTestSuckDrop=false;
            Task=209;
            break;
        case 1000:
            #ifdef  SOFT_SIMULTE
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+10000, MOT[MTestZ1].GailSpeed))
                    Task=1100;
            #else
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000, MOT[MTestZ1].GailSpeed))
                    Task=1100;
            #endif
            break;
        case 1100:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed))
                Task=209;
            break;
        case 1200:
            DoTestYRearDelay.SetMSAndOn(300);
            Task=1300;
            break;
        case 1300:
            if(DoTestYRearDelay.Off())
                Task=1000;
            break;
        case 2000:
            if(REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)
            {
                Task=65;
            }
            else
            {
                Task=70;
            }
            break;
        case 10500:                                                             //Steven 20140922 : Arm2當作指紋測試
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            if(bCheckShuttle==false)
            {
                if(CheckShuttlePos())
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(MOT[MTestZ2].Gali_ReadPos()!=Prod.TestZ2_Safe)
                {
                    Task=11000;
                    break;
                }
                bCheckShuttle=true;
            }

            if(IndexStatus==Z1Down_Z2Up)
            {
                MOT[MInShuttle1].fCanMoveM=true;                                //jou 2011-04-19 start : Task直接跳到200，一些參數沒有初始化
                MOT[MInShuttle2].fCanMoveM=true;
                bCheckShuttle=false;
                bOneTimeFlag=true;
                tIndexTimer.LatchCycleTime(true);
                ShowMainScreenPresure(0);                                       //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                bSwitchArm2=false;
                fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                           //Steven 20150304 : Add GPIB LOG
                Task=209;
                break;
            }
            else if(IndexStatus==Z1_Z2_Normal)
            {
                ShowIndexMotorError(AnsiString("DoTestYRear10500"));
                return false;
            }
            break;
        case 11000:                                                             //jou 2012-10-19 Index Arm 2 供應光源 for CMOS start
            if(MOT[MTestZ2].Gali_ReadPos()!=Prod.TestZ2_Safe)
            {
                Task=11010;
            }
            else
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //JerryYang 20241122 : 啟用D44功能時，INDEX ARM丟料後會持續吹氣，一直到INDEX ARM上抬到丟料高度
                   TestIF_File.bArm1PickPlaceArm2Test==true     &&
                   IndexStatus==Z1Down_Z2Up)
                {
                    Task=11010;
                }
                else
                {
                    Task=11020;
                }
            }

            if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true     &&
                   TestIF_File.bRTC20CheckFunction==true        )
                {
                    if(TestIF_File.bRTC20GiveWayCheck==true)
                    {
                        if(LastSet.iRealDummy==REALLY || LastSet.iRealDummy==DUMMY)
                        {
                            DoBRTCGiveWayCheck(true, true, 2);
                        }
                        else
                        {
                            DoBRTCGiveWayCheck(true, false, 2);
                        }
                        Task=11002;
                        goto DoTestYRear_ReStart;
                    }
                    else
                    {
                        COM2->SendCommToVision(COM2->rtArmIndex2, true);
                        COM2->SendCommToVision(COM2->rtCHECKIC, true);
                        bRealCCDSendArm=false;
                        ScanBtnThd->Start();
                    }
                }
            }
            break;
        case 11002:
            if(LastSet.iRealDummy==REALLY || LastSet.iRealDummy==DUMMY)
            {
                if(DoBRTCGiveWayCheck(false, true, 2)==true)
                {
                    Task=11020;
                    bCheckShuttle=true;
                    goto DoTestYRear_ReStart;
                }
            }
            else
            {
                if(DoBRTCGiveWayCheck(false, false, 2)==true)
                {
                    Task=11020;
                    bCheckShuttle=true;
                    goto DoTestYRear_ReStart;
                }
            }
            break;
        case 11010:
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed, 0))
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //JerryYang 20241122 : 啟用D44功能時，INDEX ARM丟料後會持續吹氣，一直到INDEX ARM上抬到丟料高度
                   TestIF_File.bArm1PickPlaceArm2Test==true     &&
                   IndexStatus==Z1Down_Z2Up)
                {
                    Task=11015;
                }
                else
                {
                    Task=11020;
                }
            }
            break;
        case 11015:
            if(DeviceForm.ContactMode==DropContact ||
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||           //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
               DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveDrop   ||
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //kevin 20150127 Arm1 下壓 arm2 測試
                   TestIF_File.bArm1PickPlaceArm2Test==true)
                {
                    iFinishTestUpWaitHeight=Prod.TestZ1_Test+Prod.TestZ1_Up_Offset;
                }
                else
                {
                    iFinishTestUpWaitHeight=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+Prod.TestZ1_Up_Offset;
                }
            }
            else
            {
                iFinishTestUpWaitHeight=Prod.TestZ1_Test+Prod.TestZ1_Up_Offset;
            }

            if(MOT[MTestZ1].Gali_MotMove(iFinishTestUpWaitHeight, ((MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed)/100)))
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true)
                {
                    if(IniConfig.bD44CheckIndexICDestroy)
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                                   FTestSuck.Item[i][j]==HAS_IC)
                                {
                                    FTestSuck.Suck[i][j].Normal();
                                }
                            }
                        }
                    }
                }

                Task=11016;
                DoTestYRearDelay.SetSecAndOn(Prod.TestZ_Up_Wait);
            }
            break;
        case 11016:
            if(DoTestYRearDelay.Off())
            {
                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)            //Steven 20150129 : 需要確認Arm2有沒有粘料
                {
                    InitRearTestDestroyICTask();                                //JerryYang 20250722 : add
                }

                Task=11020;
                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.bNeedCheck[i][j])                      //Suck On Destry Off
                            {
                                bIndexPlaceIcCheck=true;                        //kevin 20110505
                                flag=true;

                                if(INDEX_SUCKER_TYPE==1)                        //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[0][i][j]=true;
                                }
                                else
                                {
                                    FTestSuck.Suck[i][j].On();                  //kevin 20110504 check 掉料
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    bD44Arm1CheckVacOn=true;                                    //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                    fiosetview->ProcessIndexSuckDestroy1(1);                    //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 //kevin 20181108 add 提前動作
                    DoFrontTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                    Task=11017;
                    break;
                }
                else
                {
                    bD44Arm1CheckVacOn=false;                                   //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                }
            }
            break;
        case 11017:                                                                                                     //jou 2012-01-04 需確認Index suck已經完整做完
            if(INDEX_SUCKER_TYPE==1)
            {
                bArm1D44SuckCheck=false;                                                                                //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                bArm1D44SuckCheck=fiosetview->ProcessIndexSuckDestroy1(1);                                              //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
            }
            else
            {
                bArm1D44SuckCheck=true;
            }

            if(DoFrontTestDestroyICDelay.Off() && bArm1D44SuckCheck==true)
            {
                flag=false;
                ErrPart=" ";
                bHasErr=false;
                bHasDuplicateErr=false;
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                        if(bArmDuplicateErr[i][j])
                            bHasDuplicateErr=true;
                }

                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.bNeedCheck[i][j])                                                                  //確認真空狀態
                        {
                            if(FTestSuck.Suck[i][j].GetStatus())
                            {
                                bHasErr=true;
                                ErrPart+=IndexSuckName[i+iNN][j];                                                       //kevin 20190418 add 32 site show IC name
                                flag=true;
                                bArmDuplicateErr[i][j]=true;                                                            //Steven 20110127
                                iIndexTakeDeviceChk1=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                FTestSuck.bNeedCheck[i][j]=false;
                                FTestSuck.Suck[i][j].Normal();
                                bArmDuplicateErr[i][j]=false;                                                           //Steven 20110127
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                bD44Arm1CheckVacOn=false;                                                                               //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                if(bHasErr)
                {
                    if(MOT[MInArmY].ReadPos()<Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase])                     //kevin 20181107 shuttle 1 在load 不需移走
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else if(IndexAlarmInArmAway()==true)                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    Task=11016;                                                                                         //kevin 20181102
                    return false;
                }
                bIndexAlarmInArmAway=false;                                                                             //kevin 20181102 (Steven) :  index掉料 inarm 讓位 inarm先不要動
                bHasErr=false;
                //Steven 20100105 End

                if(flag==false)
                {
                    Task=11016;
                }
                else
                {
                    if(IniConfig.bD44CheckIndexICDestroy)                                                               //jou 2011-11-01負壓不能一直開著真空，必須關掉
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(FTestSuck.bNeedCheck[i][j])
                                {
                                    bIndexPlaceIcCheck=true;                                                            //kevin 20110505
                                    if(INDEX_SUCKER_TYPE==1)                                                            //Steven 20111202
                                    {
                                        fiosetview->bIndexSuck[0][i][j]=true;
                                    }
                                    else
                                    {
                                        FTestSuck.Suck[i][j].On();                                                      //kevin 20110504 check 掉料
                                    }
                                }
                            }
                        }
                    }
                    DoFrontTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                }
            }
            break;
        case 11020:
            if(USE_IO_CHANGE_TOQUE==true)                                                                               //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            if(bCheckShuttle==false)
            {
                if(CheckShuttlePos())
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(MOT[MTestZ2].Gali_ReadPos()!=Prod.TestZ2_Safe)
                {
                    Task=11000;
                    break;
                }
                bCheckShuttle=true;
            }

            if(IndexStatus==Z1Up_Z2Down)
            {
                if(TestIF_File.bForEgisTecTest==true)                                                                   //Steven 20140922 : Arm2當作指紋測試
                {
                    fMain->SendMSG_CMD(MSG_CMD_SwitchArmOK);
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveM=true;                                                                    //jou 2011-04-19 start : Task直接跳到200，一些參數沒有初始化
                    MOT[MInShuttle2].fCanMoveM=true;
                    bCheckShuttle=false;
                    bOneTimeFlag=true;
                    ShowMainScreenPresure(1);                                                                           //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                }

                fMain->SendMSG_CMD(MSG_CMD_Arm2Down);                                                                   //Steven 20150304 : Add GPIB LOG
                Task=209;
                break;
            }
            else if(IndexStatus==Z1_Z2_Normal)
            {
                ShowIndexMotorError(AnsiString("DoTestYRear11020"));
                return false;
            }
            else
            {
                if(bOneTimeFlag)
                {
                    bOneTimeFlag=false;
                    if(TestIF_File.bUseSLKClamp &&
                       TestIF_File.iSeparabilityTest==1)                                                                //JerryYang 20160523 Z1分離高度要上升1500
                    {
                        iBackUpZ2DownPosition=Prod.TestZ2_Test+1500;
                    }
                    else
                    {
                        iBackUpZ2DownPosition=Prod.TestZ2_Test;
                    }
                    BTestSeparateSLK(true);                                                                             //JerryYang 20180518 (wei) : 分離流程初始化
                    FTestSeparateSLK(true);
                }

                if(CheckSocketSensor(1, "DoTestYRear_11020_"))                                                          //Z1UpZ2Down //Steven 20200615 : Socket Sensor整合成Function
                {
                    Task=50;
                    return false;
                }

                if(IniConfig.bIndexPickupErrStop==false)
                {
                    bIndexArm1PickupErrStop=false;
                    bIndexArm2PickupErrStop=false;
                }

                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                    //Steven 20150129 : 需要確認Arm2有沒有粘料
                {
                    if(fRearNeedDestroy)                                                                                //JerryYang 20250723 : arm 2 destroy
                    {
                        bZ1PickShuttle=false;                                                                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                        bZ2PickShuttle=false;                                                                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                        fRearNeedDestroy=!DoRearTestDestroyIC(true);                                                    //ChungHung 20111115
                        return false;
                    }
                }

                if(DeviceForm.ContactMode==TMove ||
                   DeviceForm.ContactMode==TMoveDrop ||                                                                 //jou 2012-02-03 新增T Move Drop
                   DeviceForm.ContactMode==TMoveDropSlowContact)
                {
                    TMode=true;
                }
                else
                {
                    if(IniConfig.bIndexPickupErrStop==true &&                                                           //jou 2012-02-29 index pick up error,index arm move to center & alarm
                        bIndexArm2PickupErrStop==true)
                    {
                        TMode=true;
                    }
                    else
                    {
                        TMode=false;
                    }
                }

                if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)                                                          //----- by dell ccd realtime-------------
                {
                    if(ScanBtnThd->GetArmState())
                    {
                        if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
                        {
                            MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:11020"));
                            ScanBtnThd->SetArmState(false);
                            Task=50;
                        }
                        else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut])
                        {
                            COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut]=false;
                            ShowErrorMessage("WAR0341", 0, MMIndex, 0, __FUNC__+AnsiString(",case:11020"));             //RTC Grab TimeOut Error!
                            COM2->SendCommToVision(COM2->rtRelease, false);
                            MySleep(100);

                            ScanBtnThd->SetArmState(false);
                        }
                        return false;
                    }
                }

                #ifdef INDEX_PROTECT_TMOVE
                if(bOverRangeDoTMode==true && bTriger4Indexhome==false)                                                 //Isaac 20201012 : index Y超過範圍，做一次Tmode
                {
                    bTriger4Indexhome=true;
                    bOverRange4Indexhome=true;
                    TrigerIndexAxisHome();                                                                              //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                    return false;
                }
                #endif

                if(MOT[MTestY1].Z1UpZ2Down(MOT[MTestZ2].GailSpeed, TMode, bIndexArm2PickupErrStop))
                {
                    if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
                    {
                        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                           TestIF_File.bArm1PickPlaceArm2Test==true     &&
                           TestIF_File.bRTC20CheckFunction==true        )
                        {
                            ScanBtnThd->Stop();
                        }
                    }

                    fMain->SendMSG_CMD(MSG_CMD_Arm2Down);                                                               //Steven 20150304 : Add GPIB LOG
                    if(ATC_SYSTEM==eNewATCSystem &&
                       Temperature.bATCActiveCooling==true)                                                             //JerryYang 20220815 : send ATC which ARM
                    {
                        ATC_InterfaceForm->HandlerArm(1);
                    }
                    iRTCErrorCount=0;                                           //wei 20221222 RTC ARM Error
                    iWhichArmDown=2;                                                                                    //JerryYang 20200316 add SVID 哪支arm下壓在測區
                    fContact->ATC_SwitchTjSignal(2);                                                                    //Ifor 20210622 add: ATC Switch TJ
                    bIndexArm2PickupErrStop=false;
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    bCheckShuttle=false;
                    bOneTimeFlag=true;
                    ShowMainScreenPresure(1);                                                                           //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                    Task=11030;
                }
            }
            break;
        case 11030:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, MOT[MTestZ2].GailSpeed*Prod.TestZ_Drop_Speed/100))
            {
                if(TestIF_File.bForEgisTecTest==true)                           //Steven 20140922 : Arm2當作指紋測試
                {
                    fMain->SendMSG_CMD(MSG_CMD_SwitchArmOK);
                }

                fRearNeedDestroy=false;
                fRearNeedSuck=false;
                fMain->SendMSG_CMD(MSG_CMD_Arm2Down);                           //Steven 20150304 : Add GPIB LOG
                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)            //Steven 20150129 : 需要確認Arm2有沒有粘料
                {
                    BTestSuck.ResetAll();
                    BTestSuck.CopyFrom(TestSocket);
                    for(int i=0; i<BTestSuck.iShtRow; i++)                      //JerryYang 20160727 修正沒使用的真空產生器也吸真空
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]==HAS_IC)
                            {
                                fiosetview->bIndexSuck[1][i][j]=true;
                            }
                        }
                    }
                    Task=11035;
                }
                else
                {
                    Task=209;
                }
            }
            break;
        case 11035:                                                             //JerryYang 20250723 : SUCK 把socket資料過到arm上面
            bCheckSuck=fiosetview->ProcessIndexSuckDestroy2();
            if(bCheckSuck==true)
            {
                Task=209;
            }
            break;
        case 11040:
            if(CheckIndexStatus("DoTestYRear11040"))
            {
                iRetry=0;
                Task=11050;                                                     //Steven 20200708 : 改成強制跑Drop Height
                if(REAL_TIME_CCD &&                                             //Ifor 20191120 : add 測試完畢需檢查IC有無置偏
                   COM2->bCCDDummyRum==false &&
                   fMain->palMainStatus->Caption!="Index Check")
                {
                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                       TestIF_File.bArm1PickPlaceArm2Test==true     &&
                       TestIF_File.bRTC20CheckFunction==true        &&
                       TestIF_File.bRTC20GiveWayCheck==true         )
                    {
                        if(LastSet.iRealDummy==REALLY || LastSet.iRealDummy==DUMMY)
                        {
                            DoBRTCGiveWayCheck(true, true, 1);
                        }
                        else
                        {
                            DoBRTCGiveWayCheck(true, false, 1);
                        }
                        Task=11041;
                        goto DoTestYRear_ReStart;
                    }
                }
            }
            break;
        case 11041:
            if(LastSet.iRealDummy==REALLY || LastSet.iRealDummy==DUMMY)
            {
                if(DoBRTCGiveWayCheck(false, true, 1)==true)
                {
                    Task=11080;
                    bCheckShuttle=true;
                    goto DoTestYRear_ReStart;
                }
            }
            else
            {
                if(DoBRTCGiveWayCheck(false, false, 1)==true)
                {
                    Task=11080;
                    bCheckShuttle=true;
                    goto DoTestYRear_ReStart;
                }
            }
            break;
        case 11050:
            //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
            if(DeviceForm.ContactMode==DropContact ||
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveDrop   ||
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                if(IniConfig.bD21EnableFinishTestUpWait)                        //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
                    iFinishTestUpWaitHeight=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+IniConfig.iD21FinishTestUpWaitHeight;
                else
                    iFinishTestUpWaitHeight=Prod.TestZ2_Test;
            }
            else
            {
                if(IniConfig.bD21EnableFinishTestUpWait)
                    iFinishTestUpWaitHeight=Prod.TestZ2_Test+IniConfig.iD21FinishTestUpWaitHeight;
                else
                    iFinishTestUpWaitHeight=Prod.TestZ2_Test;
            }
            //jou 2011-10-31 end

            //Steven 20200630 : 測試完後要吹一下, 避免殘料
            //==>
            if(TestIF_File.bCheckArm2Vacuum)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]!=NULL_IC && FTestSuck.Item[i][j]!=HAS_NULL_IC)
                            BTestSuck.Suck[i][j].Off();
                    }
                }
            }
            //<==
            //Steven 20200630 : 測試完後要吹一下, 避免殘料

            if(MOT[MTestZ2].Gali_MotMove(iFinishTestUpWaitHeight, ((MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed)/100)))
            {
                Task=11060;
                if(IniConfig.bD21EnableFinishTestUpWait)                        //Steven 20200708
                    DoTestYRearDelay.SetMSAndOn(IniConfig.iD21FinishTestUpWaitTime);
                else
                    DoTestYRearDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);
            }
            break;
        case 11060:
            if(DoTestYRearDelay.Off())
            {
                //Steven 20200630 : 測試完後要吹一下, 避免殘料
                //==>
                if(TestIF_File.bCheckArm2Vacuum)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Item[i][j]!=NULL_IC && FTestSuck.Item[i][j]!=HAS_NULL_IC)
                                BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                //<==
                //Steven 20200630 : 測試完後要吹一下, 避免殘料
                Task=11080;
            }
            break;
        case 11080:
            if(MOT[MTestZ1].Gali_ReadPos()!=Prod.TestZ1_Safe)
            {
                Task=11090;
            }
            else
            {
                Task=11100;
                if(Prod.bIndexUpSpeed &&
                   (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //kevin 20190219 add Arm pick arm2 place
                    TestIF_File.bArm1PickPlaceArm2Test==true))                  //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    Task=11095;
            }

            if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true     &&
                   TestIF_File.bRTC20CheckFunction==true        )
                {
                    if(TestIF_File.bRTC20GiveWayCheck==true)
                    {
                    }
                    else
                    {
                        COM2->SendCommToVision(COM2->rtArmIndex1, true);
                        COM2->SendCommToVision(COM2->rtCHECKIC, true);
                        bRealCCDSendArm=false;
                        ScanBtnThd->Start();
                    }
                }
            }
            break;
        case 11090:
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, 0))
            {
                Task=11100;
                if(Prod.bIndexUpSpeed &&
                   (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //kevin 20190219 add Arm pick arm2 place
                    TestIF_File.bArm1PickPlaceArm2Test==true))                  //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    Task=11095;
            }
            break;
        case 11095:
            if(MOT[MTestY2].Gali_ReadPos()==Prod.TestY2_Middle)                 //kevin 20190215 add ARM 2 在測區
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+500, MOT[MTestZ2].GailSpeed*Prod.TestZ_Drop_Speed/100))
                {
                    DoBTestSuckTestICDelay.SetSecAndOn(Prod.TestZ_Drop_Wait);
                    Task=11096;
                }
            }
            break;
        case 11096:
            if(DoBTestSuckTestICDelay.Off())
            {
                Task=11100;
            }
            break;
        case 11100:
            if(USE_IO_CHANGE_TOQUE==true)                                                                               //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            if(bCheckShuttle==false)
            {
                if(CheckShuttlePos())
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(MOT[MTestZ1].Gali_ReadPos()!=Prod.TestZ1_Safe)
                {
                    Task=11080;
                    break;
                }
                bCheckShuttle=true;
            }

            if(IndexStatus==Z1Down_Z2Up)
            {
                MOT[MInShuttle1].fCanMoveM=true;                                                                        //jou 2011-04-19 start : Task直接跳到200，一些參數沒有初始化
                MOT[MInShuttle2].fCanMoveM=true;
                bCheckShuttle=false;
                bOneTimeFlag=true;
                ShowMainScreenPresure(0);                                                                               //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                bRearHeadICFallDown=false;
                Task=215;
            }
            else if(IndexStatus==Z1_Z2_Normal)
            {
                ShowIndexMotorError(AnsiString("DoTestYRear11100"));
                return false;
            }
            else
            {
                if(bOneTimeFlag)
                {
                    bOneTimeFlag=false;
                    iBackUpZ1DownPosition=Prod.TestZ1_Test;
                }

                if(CheckSocketSensor(1, "DoTestYRear_11100_"))                                                          //Z1DownZ2Up //Steven 20200615 : Socket Sensor整合成Function
                {
                    Task=50;
                    return false;
                }

                if(IniConfig.bIndexPickupErrStop==false)
                {
                    bIndexArm1PickupErrStop=false;
                    bIndexArm2PickupErrStop=false;
                }

                if(DeviceForm.ContactMode==TMove ||
                   DeviceForm.ContactMode==TMoveDrop ||                                                                 //jou 2012-02-03 新增T Move Drop
                   DeviceForm.ContactMode==TMoveDropSlowContact ||
                   DeviceForm.ContactMode==TMoveSlowContact)                                                            //Steven 20160130 : TMove Soft contact
                {
                    TMode=true;
                }
                else
                {
                    if(IniConfig.bIndexPickupErrStop==true &&                                                           //jou 2012-02-29 index pick up error,index arm move to center & alarm
                       bIndexArm1PickupErrStop==true)
                    {
                        TMode=true;
                    }
                    else
                    {
                        TMode=false;
                    }
                }

                if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)                                                          //----- by dell ccd realtime-------------
                {
                    if(ScanBtnThd->GetArmState())
                    {
                        if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC])
                        {
                            MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:11100"));
                            ScanBtnThd->SetArmState(false);
                            Task=50;
                        }
                        else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut])
                        {
                            COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarGrabTimeOut]=false;
                            ShowErrorMessage("WAR0341", 0, MMIndex, 0, __FUNC__);                                       //RTC Grab TimeOut Error!
                            COM2->SendCommToVision(COM2->rtRelease, false);
                            MySleep(100);

                            ScanBtnThd->SetArmState(false);
                        }
                        return false;
                    }
                }

                #ifdef INDEX_PROTECT_TMOVE
                if(bOverRangeDoTMode==true && bTriger4Indexhome==false)                                                 //Isaac 20201012 : index Y超過範圍，做一次Tmode
                {
                    bTriger4Indexhome=true;
                    bOverRange4Indexhome=true;
                    TrigerIndexAxisHome();                                                                              //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                    return false;
                }
                #endif

                if(MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, TMode, bIndexArm1PickupErrStop))
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]!=NULL_IC &&
                               FTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }

                    if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
                    {
                        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                           TestIF_File.bArm1PickPlaceArm2Test==true     &&
                           TestIF_File.bRTC20CheckFunction==true        )
                        {
                            ScanBtnThd->Stop();
                        }
                    }
                    fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                                                               //Steven 20150304 : Add GPIB LOG

                    if(ATC_SYSTEM==eNewATCSystem &&
                       Temperature.bATCActiveCooling==true)                                                             //JerryYang 20220815 : send ATC which ARM
                    {
                        ATC_InterfaceForm->HandlerArm(0);
                    }
                    iRTCErrorCount=0;                                           //wei 20221222 RTC ARM Error
                    iWhichArmDown=1;                                                                                    //JerryYang 20200316 add SVID 哪支arm下壓在測區
                    fContact->ATC_SwitchTjSignal(1);                                                                    //Ifor 20210622 add: ATC Switch TJ
                    bIndexArm1PickupErrStop=false;
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    bCheckShuttle=false;
                    bOneTimeFlag=true;
                    ShowMainScreenPresure(0);                                                                           //jou 2010-06-23 畫面Z1,Z2 encoder 顯示
                    bRearHeadICFallDown=false;
                    Task=11110;
                }
            }
            break;
        case 11110:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed))
            {
                fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                           //Steven 20150304 : Add GPIB LOG
                FTestSuck.ResetAll();
                Task=215;
                bPlaceTestIC=false;                                             //Ifor 20190713 : add Arm1 丟下IC
                bCheckGiveWay=true;                                             //Ifor 20190723 : add 測試完畢 Arm2 不下去測區
            }
            break;                                                              //jou 2012-10-19 Index Arm 2 供應光源 for CMOS end
        case 12000:                                                             //jou 2014-06-24 RTC 自動進行Model驗證 start
            DoBRTCAutoModelVerify(true);
            Task=12010;
            break;
        case 12010:
            if(DoBRTCAutoModelVerify(false))
            {
                bNeedWaitRTCAutoVerify=false;
                return true;
            }
            break;
        case 14000:
            if(BTestCombineSLK()==true)                                         //JerryYang 20160429 分離模式, 測試完成 Z1 SLK要與Clamp結合,再將IC吸起流程
            {
                return true;
            }
            break;
        case 16000:                                                             //Steven 20190115 : SCC要求吸取異常要檢查Socket
            DoIndexArm1PickUpErrNeedPiggyback(true);
            Task=16100;
//            break;
        case 16100:
            if(DoIndexArm1PickUpErrNeedPiggyback(false))
            {
                Task=1;
                return true;
            }
            break;                                                              //Steven 20190115 : SCC要求吸取異常要檢查Socket
        case 17900:                                                             //Richard 20220321 : 渠梁Side Push
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)      //Richard 20221110 : 渠梁Side Push 新增關SITE不動作
            {
                RecordProcess("DoTestYRear_Case17900");
                Task=320;
                break;
            }

            if(DoTestYRearDelay.Off()==false)
            {
                break;
            }

            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-100, MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed/100))
            {
                DoTestYRearDelay.SetSecAndOn(DeviceForm_File.dSitePushWaitTime);
                Task=18000;
            }
            break;
        case 18000:
            if(DoTestYRearDelay.Off()==false)
            {
                break;
            }

            if(DoTesterSidePush(true)==1)
            {
                DoTestYRearDelay.SetSecAndOn(DeviceForm_File.dSitePushWaitTime);

                if(IniConfig.bIndexArm2SupplyLight==true ||                     //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                   (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //kevin 20150127 Arm1 下壓 arm2 測試
                    TestIF_File.bArm1PickPlaceArm2Test==true))                  //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                {
                    Task=11000;
                    bPlaceTestIC=true;                                          //Ifor 20190713 : add Arm1 丟下IC
                }
                else
                {
                    Task=320;                                                   //Richard 20230301 : 渠梁Side Push 新模式   18100->320
                }
            }
            break;
        case 18100:
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)      //Richard 20221110 : 渠梁Side Push 新增關SITE不動作
            {
                RecordProcess("DoTestYRear_Case18100");
                Task=330;
                break;
            }

            if(DoTestYRearDelay.Off()==false)
            {
                break;
            }

            if(DoTesterSidePush(false)==10)
            {
                DoTestYRearDelay.SetSecAndOn(DeviceForm_File.dSitePushWaitTime);
                Task=330;                                                       //Richard 20230301 : 渠梁Side Push 新模式   320->330
            }
            break;
        case 18500:
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)      //Richard 20221110 : 渠梁Side Push 新增關SITE不動作
            {
                RecordProcess("DoTestYFront_Case18500");
                return true;
            }

            if(DoTesterSidePush(false)==10)
            {
                return true;
            }
            break;
        //==> Eastsun 20260511 F007 整合: Ifor 20220308 add: Index 兩段式上升 case 20000/20001 (從 a-side L8648-8660 verbatim)
        case 20000:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+Prod.TestZ2_Up_Offset, MOT[MTestZ2].GailSpeed*Prod.TestZ_Up_Speed/100, __FUNC__))
            {
                DoTestYRearDelay.SetSecAndOn(Prod.TestZ_Up_Wait);
                Task=20001;
            }
            break;
        case 20001:
            if(DoTestYRearDelay.Off())
            {
                Task=108;
            }
            break;
        //<== Eastsun 20260511 F007 整合
        }
    return false;
}
//------------------------------------------------------------------------------
int iRearTestPurgBeforePickShuttle=1;
TQPF_Timer hDoRearTestPurgBeforePickShuttle;
//------------------------------------------------------------------------------
void InitRearTestPurgBeforePickShuttle()                                        //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue Start
{
    iRearTestPurgBeforePickShuttle=1;
}
//------------------------------------------------------------------------------
bool DoRearTestPurgBeforePickShuttle(int isp)
{
    int &Task=iRearTestPurgBeforePickShuttle;
    static int iCT=0;

    switch(Task)
    {
        case 1:                                                                 //移至預吹氣位置
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick+DeviceForm_File.iPurgeBdforePickShuttleOffSet*100, isp))
            {
                iCT=0;
                Task=100;
            }
            break;
        case 100:
            if(iCT<DeviceForm_File.iPurgeBeforePickShuttleInterval)
            {
                iCT++;
                hDoRearTestPurgBeforePickShuttle.SetSecAndOn(DeviceForm_File.iPurgeBeforePickShuttleTime);
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_IC ||
                           BTestSuck.Item[i][j]==HAS_HOT_IC)                    //Steven 20151016 : Fixed for shuttle pruge function
                            ;
                        else
                            BTestSuck.Suck[i][j].Off();
                    }
                }
                Task=200;
            }
            else
            {
                Task=400;
                break;
            }
        case 200:
            if(hDoRearTestPurgBeforePickShuttle.Off())
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_IC ||
                           BTestSuck.Item[i][j]==HAS_HOT_IC)                    //Steven 20151016 : Fixed for shuttle pruge function
                            ;
                        else
                            BTestSuck.Suck[i][j].Normal();
                    }
                }
                hDoRearTestPurgBeforePickShuttle.SetSecAndOn(1);
                Task=300;
            }
            break;
        case 300:
            if(hDoRearTestPurgBeforePickShuttle.Off())
            {
                Task=100;
            }
            break;
        case 400:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_IC ||
                       BTestSuck.Item[i][j]==HAS_HOT_IC)                        //Steven 20151016 : Fixed for shuttle pruge function
                        ;
                    else
                        BTestSuck.Suck[i][j].Normal();
                }
            }
            Task=500;
            break;
        case 500:
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool BTestNeedDestroy()
{
    if(BTestSuck.All_HAS_NULL_IC())
        return true;
    else if(BTestSuck.All_HAS_NULL_IC())
        return true;
    else if(BTestSuck.UseSiteNoIC())
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool BTestSeparateSLK(bool bReset)                                              //JerryYang 20160429 Socket clamp模式, Z1將IC放到Socket流程
{
    bool bResult=false;
    int &Task=iBTestSocketClampCloseTask;
    #ifndef SOFT_SIMULTE
    int ret;
    #endif

    if(bReset ||
       TestIF_File.bUseSLKClamp==false ||                                       //JerryYang 20180518 (wei) : 新增保護
       TestIF_File.iSeparabilityTest==0 ||
       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
        TestIF_File.bArm1PickPlaceArm2Test==true))                              //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            {
                Cylinder[C_Socket_Unclamp].Off();
                Cylinder[C_Socket_Clamp].On();                                  //Z1已經下降到測試高度,Socket clamp夾持
                Task=10;
                BTestSocketClampCloseDelay.SetMSAndOn(300);
                BTestSocketClampTimeOutDelay.SetSecAndOn(2.5);
            }
            break;
        case 10:
            if(BTestSocketClampCloseDelay.Off())
            {
                #ifdef SOFT_SIMULTE
                    Task=20;
                #else
                if(Sen[SnSocketClampPush1].IsOn()==true && Sen[SnSocketClampPush2].IsOn()==true &&
                   Sen[SnSocketHasClamp1].IsOn()==true && Sen[SnSocketHasClamp2].IsOn()==true)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Suck[i][j].Normal();                      //破真空將IC放到SOCKET上
                        }
                    }
                    Task=20;
                }
                else
                {
                    if(BTestSocketClampTimeOutDelay.Off())
                    {
                        ret=ShowErrorMessage("JAM0375", K_RETRY, MMIndex);      //socket clamp異常
                        if(ret==K_RETRY)
                        {
                            Task=1;
                        }
                    }
                }
                #endif
            }
            break;
        case 20:                                                                // Socket clamp夾持後,SLK1 放開clamp
            Cylinder[C_SLK1_Clamp].Off();
            Cylinder[C_SLK1_Unclamp].On();
            BTestSocketClampCloseDelay.SetMSAndOn(500);
            BTestSocketClampTimeOutDelay.SetSecAndOn(2.5);
            Task=30;
            break;
        case 30:
            if(BTestSocketClampCloseDelay.Off())
            {
                #ifdef SOFT_SIMULTE
                    Task=40;
                #else
                if(Cylinder[C_SLK1_Unclamp].OnStatus())
                    Task=40;
                if(BTestSocketClampTimeOutDelay.Off())
                {
                    ret=ShowErrorMessage("JAM0373", K_RETRY, MTestY1);          //ARM1 SLK放開clamp異常
                    if(ret==K_RETRY)
                    {
                        Task=20;
                    }
                }
                #endif
            }
            break;
        case 40:                                                                //JerryYang 2016042 Arm 1上升5mm再測試
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test+1500, iSpeedSlow))
                bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool BTestCombineSLK(bool bReset)                                               //JerryYang 20160429 Socket clamp模式, 測試完成 Z1要將IC從Socket吸起流程
{
    static bool bSLK1NeedClamp=false;
    static bool bFTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bFTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false,false,false,false,false,false,false,false},
                                                               {false,false,false,false,false,false,false,false},
                                                               {false,false,false,false,false,false,false,false},
                                                               {false,false,false,false,false,false,false,false}};

    int &Task=iBTestSocketClampOpenTask;
    int ret=0;
    bool bCheckAllSuck=false, flag=false;
    bool bResult=false;

    if(bReset ||
       TestIF_File.bUseSLKClamp==false ||                                       //JerryYang 20180518 (wei) : 新增保護
       TestIF_File.iSeparabilityTest==0 ||
       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
        TestIF_File.bArm1PickPlaceArm2Test==true))                              //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
    {
        Task=1;
        for(int i=0; i<FTestSuck.iMaxRow; i++)
        {
            for(int j=0; j<FTestSuck.iMaxCol; j++)
            {
                bFTestSuckUse[i][j]=false;
                bFTSuckFinish[i][j]=false;
            }
        }
        return bResult;
    }

    switch(Task)
    {
        case 1:
            {
                bSLK1NeedClamp=false;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]!=NULL_IC &&
                           FTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            FTestSuck.Suck[i][j].Error=false;
                            bFTestSuckUse[i][j]=true;
                            bSLK1NeedClamp=true;
                        }
                        bFTSuckFinish[i][j]=false;                              //Steven 20110301
                    }
                }
                Task=10;
            }
            break;
        case 10:
            if(bSLK1NeedClamp==true)
            {
                Task=15;                                                        //JerryYang 20160714 修正index上有已測IC, 回home後hang up問題
            }
            else
            {
                return true;                                                    //arm1上無IC 無須進行流程
            }
            break;
        case 15:
            if(MOT[MTestZ1].Gali_ReadPos()==Prod.TestZ1_Test)                   //JerryYang 20160714 修正index上有已測IC, 回home後hang up問題
            {
                Task=30;
            }
            else
            {
                Cylinder[C_SLK1_Unclamp].On();
                Cylinder[C_SLK1_Clamp].Off();
                BTestSocketClampTimeOutDelay.SetSecAndOn(2.5);
                Task=20;
            }
            break;
        case 20:                                                                //確認SLK Clamp放開
            if(Cylinder[C_SLK1_Unclamp].OnSensor())
            {
                Task=30;
            }
            else if(BTestSocketClampTimeOutDelay.Off())
            {
                ret=ShowErrorMessage("JAM0373", K_RETRY, MTestY1);              //JerryYang 20160714 ARM1 SLK放開clamp異常
                if(ret==K_RETRY)
                {
                    Task=15;
                }
            }
            break;
        case 30:                                                                //測試完,Arm1要下降
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, iSpeedSlow))
            {
                Task=40;
            }
            break;
        case 40:                                                                //SLK Clamp 夾持
            Cylinder[C_SLK1_Unclamp].Off();
            Cylinder[C_SLK1_Clamp].On();
            BTestSocketClampOpenDelay.SetMSAndOn(500);
            Task=50;
            BTestSocketClampTimeOutDelay.SetSecAndOn(2.5);
            break;
        case 50:
            if(BTestSocketClampOpenDelay.Off())
            {
                if(Cylinder[C_SLK1_Clamp].OnSensor())
                {
                    Task=60;
                }
                else
                {
                    if(BTestSocketClampTimeOutDelay.Off())
                    {
                        ret=ShowErrorMessage("JAM0371", K_RETRY, MTestY1);      //ARM2 SLK clamp未到位
                        if(ret==K_RETRY)
                        {
                            Task=40;
                        }
                    }
                }
            }
            break;
        case 60:
            Cylinder[C_Socket_Clamp].Off();                                     //Socket Clamp 打開
            Cylinder[C_Socket_Unclamp].On();
            BTestSocketClampOpenDelay.SetMSAndOn(500);
            Task=70;
            BTestSocketClampTimeOutDelay.SetSecAndOn(2.5);
            break;
        case 70:
            if(BTestSocketClampOpenDelay.Off())
            {
                #ifdef SOFT_SIMULTE
                    Task=80;
                #else
                if(Sen[SnSocketClampPull1].IsOn()==true && Sen[SnSocketClampPull2].IsOn()==true &&
                   Sen[SnSocketClampPush1].IsOff()==true && Sen[SnSocketClampPush2].IsOff()==true)
                {
                    Task=80;
                }
                else
                {
                    if(BTestSocketClampTimeOutDelay.Off())
                    {
                        ret=ShowErrorMessage("JAM0376", K_RETRY, MMIndex);      //放開clamp異常
                        if(ret==K_RETRY)
                        {
                            Task=60;
                        }
                    }
                }
                #endif
            }
            break;
        case 80:                                                                //吸取IC
            bCheckAllSuck=true;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTSuckFinish[i][j]==false)
                    {
                        if(bFTestSuckUse[i][j])
                        {
                            if(FTestSuck.Suck[i][j].Suck())
                            {
                                bFTSuckFinish[i][j]=true;
                                bFTestSuckUse[i][j]=false;
                            }
                            else if(FTestSuck.Suck[i][j].Error)
                            {
                                bFTSuckFinish[i][j]=true;
                            }
                            else
                            {
                                bCheckAllSuck=false;
                            }
                        }
                        else
                        {
                            bFTSuckFinish[i][j]=true;
                        }
                    }
                }
            }

            flag=true;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTSuckFinish[i][j]==false)
                        flag=false;
                }
            }

            if(flag)
            {
                if(bCheckAllSuck)
                {
                    Task=90;
                }
            }
            break;
        case 90:
            bResult=true;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void DoArm2Suck()                                                               //JerryYang 20190123 把index arm吸真空&交換狀態包成函式
{
    bool bNeedWaitUp=false;

    if(CosFunction.bUseShuttlePickShiftDetect==true &&
       DeviceForm_File.dDropByPassDetect!=0 &&
       bIndexZ2NeedUp==true)                                                    //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
    {
        bNeedWaitUp=true;
    }
    else
    {
        bNeedWaitUp=false;
    }

    for(int i=0; i<BTestSuck.iShtRow; i++)
    {
        for(int j=0; j<BTestSuck.iShtCol; j++)
        {
            #ifdef SOFT_SIMULTE
            if(fMain->cbIndexDrop->Checked==true)
            {
                bArm2SuckFinish[0][1]=true;
                BTestSuck.Suck[0][1].Error=true;
            }
            #endif

            if(BLCarryKit.Item[i][j])                                           // && bArm2SuckFinish[i][j]==false)
            {
                if(BLCarryKit.Item[i][j]==HAS_NULL_IC)
                {
                    if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                       BTestSuck.Item[i][j]==NULL_IC)                           //Steven 20111202 : Retry會掉料
                    {
                        BTestSuck.Suck[i][j].Normal();                          //Steven 20111201 : 預防負壓壓降
                    }

                    if(BLCarryKit.Item[i][j]==HAS_NULL_IC &&
                       BTestSuck.Item[i][j]==NULL_IC)                           //Steven 20160817 : 移到上面
                    {
                        BTestSuck.Suck[i][j].Normal();
                        if(bNeedWaitUp==false)
                        {
                            BTestSuck.MoveSuckData(BLCarryKit, i, j);
                        }
                    }
                    else if(BLCarryKit.Item[i][j]==HAS_NULL_IC)                 //Index有料, 蝦頭沒料, 這個怪怪的
                    {
                        if(bNeedWaitUp==false)
                        {
                            BLCarryKit.SetItemData(i, j, NULL_IC);
                        }
                    }
                    else                                                        //蝦頭有料, index也有料, 怪怪的
                    {
                    }

                    bArm2SuckFinish[i][j]=true;                                 //Steven 20110301
                }
                else if(BLCarryKit.Item[i][j]!=NULL_IC)
                {
                    if(BTestSuck.Suck[i][j].Error)                              //Steven 20110301 : 有錯誤的不做
                    {
                        bArm2SuckFinish[i][j]=true;
                    }
                    else if(BTestSuck.Item[i][j]==NULL_IC &&                    //Steven 20160817 (Jou) : 不能吸了又吸
                            BTestSuck.Suck[i][j].Suck())
                    {
                        if(bNeedWaitUp==false)
                        {
                            BTestSuck.MoveSuckData(BLCarryKit, i, j);
                            BTestSuck.PordRec[i][j].AddIndexPickShuttleRecord(TestIF.iSiteMap[i][j], i, j);             //Sam 20201216 : Add record  //Sam 20200716 : Add record
                            bArm2DuplicateErr[i][j]=false;                      //Steven 20100105
                            bArm2SuckFinish[i][j]=true;                         //Steven 20110301 : 吸取完成的不做
                            if(bRunAutoClean==false)                            //kevin 20120531
                                LastSet.iIndexInputOutPut[0]++;                 //kevin 20120511 Index 吸取IC計數
                            TestIF_File.Arm2PickerLifeCnt[i][j]++;              //JerryYang 20220923 : add
                            CheckPickerLifeNeedOneCycle();                      //AI(ht9045-config) 20260521 (RogerYang) : SCC吸嘴壽命報警OneCycle優化
                            BTestSuck.PordRec[i][j].AddIndexPickVacuum(BTestSuck.Suck[i][j].GetIOValue());              //Sam 20230210 : 新增 VacuumUnit 通訊模組
                        }
                        bArm2DuplicateErr[i][j]=false;                          //Steven 20100105
                        bArm2SuckFinish[i][j]=true;                             //Steven 20110301 : 吸取完成的不做
                    }
                    else
                    {
                        bArm2SuckComplete=false;                                //jou 2011-08-16 只要有未完成的就繼續等
                    }
                }
            }
            else
            {
                if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                   BTestSuck.Item[i][j]==NULL_IC)                               //Steven 20111202 : Retry會掉料
                {
                    BTestSuck.Suck[i][j].Normal();                              //Steven 20111201 : 預防負壓壓降
                }
                bArm2SuckFinish[i][j]=true;                                     //Steven 20110301 : 沒有東西的地方要跳過
            }
        }
    }
}
//------------------------------------------------------------------------------
void DoArm2D44VacCheck()                                                        //JerryYang 20190123 把index arm回吸檢查包成函式
{
    if(INDEX_SUCKER_TYPE==1)
    {
        bArm2D44SuckCheck=false;
        bArm2D44SuckCheck=fiosetview->ProcessIndexSuckDestroy2(1);              //Sam 20241225 : 修正回吸偵測
    }
    else
    {
        bArm2D44SuckCheck=true;
    }

    if(DoRearTestDestroyICDelay.Off() &&
       bArm2D44SuckCheck==true)
    {
        for(int i=0; i<BTestSuck.iShtRow; i++)
        {
            for(int j=0; j<BTestSuck.iShtCol; j++)
            {
                if(BTestSuck.bNeedCheck[i][j])
                {
                    if(BTestSuck.Suck[i][j].GetStatus())
                    {
                    }
                    else
                    {
                        BTestSuck.bNeedCheck[i][j]=false;
                        BTestSuck.Suck[i][j].Normal();
                    }
                }

                if(INDEX_SUCKER_TYPE==1)                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                {
                    BTestSuck.Suck[i][j].Normal();
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
