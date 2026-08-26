#include "MachineDefine.h"
#pragma hdrstop

#include "atester.h"
#include "atester_32Site.h"
#include "atester_ProcessCount.h"
#include "aTester_Front.h"
#include "aTester_Rear.h"

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
//#include "SECSGEM.h"
#include "Socket_ASE_KR.h"
//#include "ScanBtnThread.h"
#include "InterfaceSYS.h"
#include "BarCode.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "ainarm2.h"
#include "MessageDef.h"
#include "uHeaterThread.h"
#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "cpublic.h"
#include "SCK_ART.h"
#include "ATC_Handler_Side.h"
#include "AMR.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

extern bool bEcho, bExist, bUnderTest;
extern bool bEchoStop;                                                          //ChungHung 20130326 add
extern bool CheckTestSuckICOn(class TMyKitSuck &Ptr, int iR, int iC);
extern void InitInArmTask();
extern bool MoveInArm2XYToWait();
extern bool bHangTimePause;                                                     //Steven 20090827 : Hang Up dectector
extern bool bDoubleContact;
extern int  iTesterBIN[MAX_SOCKET_ROW][MAX_SOCKET_COL];

int iCCDTimeOutCount_32=0;
int iHangupCTArm1_32=0;
//==============================================================================
bool DoInterFaceErrorStep_TwoArm32Site()                                        //Steven 20140815 : Add For 32 Site
{
    static bool bArm1OK=false, bArm2OK=false;

    int &Task=iDoInterFaceErrorStepTask;
    AnsiString asChinese=AnsiString("下壓次數已經超過設定值，請打開Chamber側門並清潔Socket");       //Steven 20230104 : 直接寫死
    AnsiString asEnglish=AnsiString("Contact over setting # Please Open Chamber Side Door and clean socket");           //ChungHung 20121029 alter 客戶會誤解。

    switch(Task)
    {
        case 1:
            bArm1OK=false;
            bArm2OK=false;
            if(bContactCTOverCHK)
            {
                bContactTimeOverStep=true;
            }
            else
            {
                bNowDoInterFaceErrorStep=true;
            }
            Task=2;
        case 2:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, "DoInterFaceErrorStep_TwoArm32Site 2"))
                Task=100;
            break;
        case 100:
            if(bContactCTOverCHK)
            {
                MyMessageBox->lblChineseMsg->Font->Size=10;
                ShowMyMessage(asEnglish, asChinese);                            //jou 2012-06-05
                MyMessageBox->lblChineseMsg->Font->Size=12;
                Task=150;
            }
            else
            {
                if(bEchoStop==true)                                             //ChungHung 20130326 add
                {
                    bEchoStop=false;
                    ShowMyMessage("Receive ECHOSTOP Form TESTER");
                    Task=200;
                }
                else
                {
                    ProcessTesterTimeOut(0);
                    SetNoiseDelay=false;
                    TestISTimeOut=false;
                    ProcessCount(0);
                    if(TestSocket.UseSiteHasIC())                               //jou 20230828 : 修正index all drop error
                    {
                        if(IsNNMode()==NN_1Row)
                            TestSocket.SplitSuck1Row(BTestSuck, FTestSuck);
                        else
                            TestSocket.SplitSuck(BTestSuck, FTestSuck, false);  //Steven 20140815 : Add For 32 Site
                    }
                    RecordHistroy(0);
                    bFinshTest=true;
                    bInitStartDelayNotFinish=true;                              //Ifor 20181220 : add Init Start Delay Time Not Finish
                    bTJControlMode=false;                                       //Ifor 20190328 : add TJ Temp Over Range
                    ATC_InterfaceForm->SendHandler2DID(0, false);
                    SW[SwTesterPower].Off();
                    Task=200;
                }
                break;
            }
            break;
        case 150:
            if(bContactCTOverCHK==false)                                        //已經開門了
            {
                Task=170;
            }
            else
            {
                MyMessageBox->lblChineseMsg->Font->Size=10;
                ShowMyMessage(asEnglish, asChinese);                            //jou 2012-06-05
                MyMessageBox->lblChineseMsg->Font->Size=12;
            }

            bArm1OK=false;
            bArm2OK=false;
            break;
        case 170:
            if(bArm1OK==false)
                bArm1OK=MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, MOT[MTestZ1].GailSpeed);

            if(bArm2OK==false)
                bArm2OK=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, MOT[MTestZ2].GailSpeed);

            if(bArm1OK && bArm2OK)
            {
                bContactTimeOverStep=false;
                return true;
            }
            break;
        case 200:
            if(bArm1OK==false)
                bArm1OK=MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, MOT[MTestZ1].GailSpeed);

            if(bArm2OK==false)
                bArm2OK=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, MOT[MTestZ2].GailSpeed);

            if(bArm1OK && bArm2OK)
            {
                bNowDoInterFaceErrorStep=false;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer DoTestYFrontDelay_32, DoTestYFrontDelay2_32;

int i32RTCAutoModelVerifyTask=1;
bool Do32RTCAutoModelVerify(bool bInitial)
{
    if(bInitial==true)
    {
        i32RTCAutoModelVerifyTask=1;
        return false;
    }

    static bool bVerifyNG=false;
    static bool bFTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false} };

    static bool bFTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bBTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false} };

    static bool bBTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    int &Task=i32RTCAutoModelVerifyTask;
    bool bCheckAllSuck=false, flag=false;

    switch(Task)
    {
        case 1:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "Do32RTCAutoModelVerify 1"))
            {
                bVerifyNG=false;
                Task=100;
            }
            break;
        case 100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "Do32RTCAutoModelVerify 100"))
            {
                DoAllPassVerifyRTC(true);
                Task=200;
            }
            break;
        case 200:
            if(DoAllPassVerifyRTC())
            {
                Task=12010;
            }
            break;
        case 12010:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Middle, iSpeedY, "Do32RTCAutoModelVerify 12010"))
            {
                Task=12020;
            }
            break;
        case 12020:
            if(Prod.TestZ1_Drop_Offset!=0)
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+200, iSpeedSlow))
                {
                    Task=12100;
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test+200, iSpeedSlow))
                {
                    Task=12100;                                                 //Steven 20110511
                }
            }
            break;
        case 12100:
            bFTestSuckDrop=true;

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                       FTestSuck.Item[i][j]==HAS_IC)
                    {
                        FTestSuck.Suck[i][j].Off();
                    }

                    DoTestYFrontDelay_32.SetSecAndOn(3);
                }
            }
            Task=12200;
            break;
        case 12200:
            if(DoTestYFrontDelay_32.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_HOT_IC ||
                           FTestSuck.Item[i][j]==HAS_IC)
                            FTestSuck.Suck[i][j].Normal();
                    }
                }

                bIndexCheckNoStopVaccum=false;
                Task=12300;
            }
            break;
        case 12300:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed))
            {
                Task=12310;
            }
            break;
        case 12310:
            if(Prod.TestZ2_Drop_Offset!=0)
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+200, iSpeedSlow))
                {
                    Task=12320;
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test+200, iSpeedSlow))
                {
                    Task=12320;                                                 //Steven 20110511
                }
            }
            break;
        case 12320:
            bBTestSuckDrop=true;

            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_HOT_IC ||
                       BTestSuck.Item[i][j]==HAS_IC)
                    {
                        BTestSuck.Suck[i][j].Off();
                    }

                    DoTestYFrontDelay_32.SetSecAndOn(3);
                }
            }
            Task=12330;
            break;
        case 12330:
            if(DoTestYFrontDelay_32.Off())
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_HOT_IC ||
                           BTestSuck.Item[i][j]==HAS_IC)
                            BTestSuck.Suck[i][j].Normal();
                    }
                }

                bIndexCheckNoStopVaccum=false;
                Task=12340;
            }
            break;
        case 12340:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed, "Do32RTCAutoModelVerify 12340"))
            {
                Task=12400;
            }
            break;
        case 12400:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "Do32RTCAutoModelVerify 12400"))
            {
                Task=12500;
            }
            break;
        case 12500:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease]=false;
            COM2->SendCommToVision(COM2->rtRelease, true);
            DoTestYFrontDelay_32.SetSecAndOn(10);
            Task=12600;
            break;
        case 12600:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease])
            {
                Task=12700;
            }
            else if(DoTestYFrontDelay_32.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Release Time out of Auto Model Verify");
                COM2->DoReleaseAndInspEnd();
                Task=12500;
            }
            break;
        case 12700:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd]=false;
            COM2->SendCommToVision(COM2->rtInspEnd, true);
            DoTestYFrontDelay_32.SetSecAndOn(10);
            Task=12800;
            break;
        case 12800:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd])
            {
                Task=12900;
            }
            else if(DoTestYFrontDelay_32.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC End Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12500;
            }
            break;
        case 12900:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYOK]=false;
            COM2->SendCommToVision(COM2->rtOPENVERIFYNG, true);
            DoTestYFrontDelay_32.SetSecAndOn(10);
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
            else if(DoTestYFrontDelay_32.Off())
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
            DoTestYFrontDelay_32.SetSecAndOn(10);
            Task=13200;
            break;
        case 13200:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK])
            {
                bVerifyNG=false;
                Task=13400;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG])
            {
                bVerifyNG=true;
                Task=13300;
            }
            else if(DoTestYFrontDelay_32.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Verify All Fail Time out");
                COM2->DoReleaseAndInspEnd();
                Task=12500;
            }
            break;
        case 13300:
            if(IndexAlarmInArmAway()==true)
            {
                ShowMyMessage("RTC Verify All Fail NG");
                Task=13400;
            }
            break;
        case 13400:
            COM2->SendCommToVision(COM2->rtCLOSEVERIFY, true);
            DoTestYFrontDelay_32.SetSecAndOn(10);
            Task=13500;
            break;
        case 13500:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtCLOSEVERIFY])
            {
                Task=13600;
            }
            else if(DoTestYFrontDelay_32.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Close Verify Time out");
                COM2->DoReleaseAndInspEnd();
                Task=13400;
            }
            break;
        case 13600:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Middle, iSpeedY, "Do32RTCAutoModelVerify 13600"))
            {
                Task=13700;
            }
            break;
        case 13700:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))
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
                        bFTSuckFinish[i][j]=false;
                    }
                }

                Task=13800;
            }
            break;
        case 13800:
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
                for(int i=0; i<FTestSuck.iShtRow; i++)                          //若有吸取錯誤
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            if(IniConfig.bD62PickUpErrorNeedPurge)              //Steven 20161024 : 吸取異常需要吹氣一次
                            {
                                FTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bCheckAllSuck)
                {
                    Task=13810;
                }
            }
            break;
        case 13810:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))
            {
                Task=13820;
            }
            break;
        case 13820:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Item[i][j]!=NULL_IC &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            BTestSuck.Suck[i][j].Error=false;
                            bBTestSuckUse[i][j]=true;
                        }
                        bBTSuckFinish[i][j]=false;
                    }
                }

                Task=13830;
            }
            break;
        case 13830:
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
                for(int i=0; i<BTestSuck.iShtRow; i++)                          //若有吸取錯誤
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            if(IniConfig.bD62PickUpErrorNeedPurge)              //Steveb 20161024 : 吸取異常需要吹氣一次
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
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "Do32RTCAutoModelVerify 13900"))
            {
                Task=14000;
            }
            break;
        case 14000:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear, iSpeedY, "Do32RTCAutoModelVerify 14000"))
            {
                if(bVerifyNG==true)
                {
                    ShowMyMessage("RTC Auto Verify fail!!");
                    Task=1;
                }
                else
                {
                    COM2->InitRealTimeCCDPara();
                    bFTestSuckDrop=false;
                    IndexStatus=Z1Down_Z2Up;
                    return true;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iTestTwoArm32SiteTask=1;

void InitTestYTwoArm32SiteTask()                                                //2013-01-15    Dell    Add nn Mode
{
    iTestTwoArm32SiteTask=1;
}
//---------------------------------------------------------------------------
TQPF_Timer iWaitIndexArm1_32;
TQPF_Timer hTestZ2Delay_32;
TQPF_Timer TorqueTimeOutDelay;                                                  //KenHsieh 20220409 : nn Mode扭力紀錄
bool DoTestY_TwoArm32Site()                                                     //2013-01-15    Dell    Add nn Mode
{
    static bool bSucketHasICError=false;
    static bool bOneTimeFlag=true, bSocketCheckSkip=false, flag1=false;
    static bool bCheckShuttle=false;
    static bool bFTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static bool bBTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static bool bFTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static bool bBTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static bool bMovOK_1=false, bMovOK_2=false, bIndexSuckCheck1=false, bIndexSuckCheck2=false;
    static bool bintered1=true;                                                 //Isaac 20200922 : 紀錄indexArmY encoder值和command值
    static AnsiString ErrPart="";                                               //Steven 20101122
    static AnsiString str="";                                                   //kevin 20180508 socket sensor for 32 site

    DoTestYTwoArm_ReStart:
    QueueTaskList[51].CheckTaskChange();                                        //Steven 20200821 : 使用Goto也要記錄Task變化

    int &Task=iTestTwoArm32SiteTask;
    int iIndexUpPos=0;
    int iAZ1=0, iAZ2=0;
    int iNN=IsNNMode();
    int iFinishTestUpWaitHeight_Z1=0;                                           //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
    int iFinishTestUpWaitHeight_Z2=0;
    int ret;
    bool bCheckAllSuck=false, flagSuckErr=false;
    bool flag=false;
    bool TMode=false;
    bool PickShuttleIC_Together=IniConfig.bD59_32SitePnpTogether;               //JerryYang 20220830 : 統一用D59功能
    AnsiString sBufferT="", str1;

    switch(Task)
    {
        case 1:
            fRearNeedSuck=false;
            fRearNeedDestroy=false;
            InitRearTestSuckICTask();
            InitRearTestDestroyICTask();
            fFrontNeedSuckIC=false;

            fFrontNeedSuck=false;
            fFrontNeedDestroy=false;
            InitFrontTestSuckICTask();
            InitFrontTestDestroyICTask();
            InitBTestSuckTestICTask();
            fRearNeedSuckIC=false;

            if(FTestSuck.UseSiteNoIC())
            {
                InitFrontTestSuckICTask();
                fFrontNeedSuck=true;
            }
            else
            {
                if((FTestSuck.UseSiteHasIC() && FTestSuck.AlreadyTest()) || FTestNeedDestroy())
                {
                    fFrontNeedDestroy=true;                                                                             // 先放掉已測IC,再進行新IC吸取
                    fFrontNeedSuck=true;
                    InitFrontTestSuckICTask();
                    InitFrontTestDestroyICTask();
                }
            }

            if(BTestSuck.UseSiteNoIC())
            {
                InitRearTestSuckICTask();
                fRearNeedSuck=true;
            }
            else
            {
                if((BTestSuck.UseSiteHasIC() && BTestSuck.AlreadyTest()) || BTestNeedDestroy())
                {
                    fRearNeedDestroy=true;                                                                              // 先放掉已測IC,再進行新IC吸取
                    fRearNeedSuck=true;
                    InitRearTestSuckICTask();
                    InitRearTestDestroyICTask();
                }
            }

            fFrontNeedTest=false;
            fRearNeedTest =false;

            if(iOneCycle &&
               IsInArmOneCycleFinish()==true &&
               bCanNotDisableOneCycle==false &&
               FLCarryKit.UseSiteNoIC() &&
               InArmSuck.HasRealIC()==false &&
               ((LastSet.iTemperature==Tempture_Hot && bPickFromHotplate==false) ||                                     //Steven 20220121 : 避免In arm吸到一半還沒吸起來
                (LastSet.iTemperature!=Tempture_Hot && bPickFromLoader==false)))
            {
                fFrontNeedSuck=false;
            }

            Task=100;
            goto DoTestYTwoArm_ReStart;                                                                                 //2008/10/20 lee

    //-------------------------
    //掉料時的處置方式
    //-------------------------
        case 50:
            MOT[MTestZ2].MovFlag=false;
            MOT[MTestZ2].bScanFlag=false;
            DoTestYFrontDelay_32.SetMSAndOn(300);
            Task=52;
            break;
        case 52:
            if(DoTestYFrontDelay_32.Off())
                Task=55;
            break;
        case 55:                                                                                                        //----- by dell ccd realtime-------------
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 50000, "DoTestY_TwoArm32Site 55"))                    //Steven 20140815 : Add For 32 Site
            {
                if(REAL_TIME_CCD==true && COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
                    Task=61;
                else
                    Task=60;
            }
            break;
        case 60:
            if(MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false)
            {
                ShowIndexMotorError(AnsiString("DoTestY_TwoArm32Site60"));
                break;
            }
            bSocketCheckSkip=false;

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestY_TwoArm32Site 60"))
            {
                if(bSucketHasICError)
                {
                    bIsTestSitICFallDown=true;
                    bSucketHasICError=false;
                    if(CUSTOMER_CODE==CC_AMKOR_China ||                         //Steven 20101112
                       IniConfig.bKoreaFunction ||
                       IniConfig.bMaximFunction ||                              //JerryYang 20190522 Maxim統一軟體功能
                       CUSTOMER_CODE==CC_QUALCOMM ||                            //JerryYang 20170412 (Steven) add QUALCOMM
                       CUSTOMER_CODE==CC_Renesas)
                    {
                        ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, ErrPart);
                    }
                    else
                    {
                        ShowMyMessage("Socket has IC error", "Socket有IC殘留!!", "DoTestY_TwoArm32Site 60");
                    }
                }
                Task=61;
            }
            break;
        case 61:
            if(TestIF_File.bEnSocketSensor && iShowSocketSensor)                                                        //kevin 20180508 socket sensor
            {
                if(IndexAlarmInArmAway()==true)                                                                         //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                //Steven 20151022 : add for MAXIM
                        bIsTestSitICFallDown=true;
                    bIsSocketSensor=true;
                    iShowSocketSensor=0;
                    ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, sSocketSensorErr);                             //kevin 20130504 socket sensor
                }
                break;
            }

            if(REAL_TIME_CCD==true &&                                                                                   //----- by dell ccd realtime-------------
               MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false)
            {
                ShowIndexMotorError(AnsiString("DoTestY_TwoArm32Site61"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestY_TwoArm32Site 61"))
            {
                if(REAL_TIME_CCD==true &&                                                                               //----- by dell ccd realtime-------------
                   COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
                {
                    Task=64;                                                                                            //Only RealTime
                }
                else
                {
                    Task=62;
                }
            }
            break;
        case 62:
            flag=false;
            if(bRecIndexDropAlarm1==true && bRecIndexDropAlarm2==true)          //Steven 20190904 : 兩隻Arm同時Alarm
            {
                flag=CheckIndexSuckICFallDownSetToHasNullIC(3);
            }
            else if(bRecIndexDropAlarm1==true)
            {
                flag=CheckIndexSuckICFallDownSetToHasNullIC(0);
            }
            else
            {
                flag=CheckIndexSuckICFallDownSetToHasNullIC(1);
            }

            if(flag)
            {
                if(REAL_TIME_CCD==true)                                         //----- by dell ccd realtime-------------
                {
                    COM2->DoReleaseAndInspEnd();
                }
                Task=2000;
            }
            break;
        case 64:
            if(CheckInArmFinishAllPickerAction()==false)                                                                                                        //Steven 20171226 (Wei) : 修改in arm讓開的flag
                return false;
            InitInArmTask();
            if(MoveInArm2XYToWait()==true)                                                                                                                      //如果要移開,而且還沒移到定位
            {
                if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                    ShowErrorMessage("WAR0346", 0, MMIndex, (bRTCArm1HalfViewError || bRTCArm2HalfViewError), __FUNC__);                                        //RTC FullView Socket Have Devices Error!
                    COM2->DoReleaseAndInspEnd();                                                                                                                //Steven 20120522 : 換位置
                    bRTCArm1HalfViewError=true;                                                                                                                 //Steven 20120206 : RTC重複錯誤
                    if(IniConfig.bD49RTCAlarmSetIndexToErrBin)                                                                                                  //JerryYang 20160712 for 力成,發生RTC Alarm時把Index上所有IC設為Errorbin
                    {
                        int iCT=FTestSuck.CountRealIC();
                        AnsiString sBuffer;
                        sBuffer.printf("RTC Alarm set Arm1 Place to Error bin : Device=%d;",iCT);
                        MyDBIProcess("Message", sBuffer);
                        FTestSuck.SetAllRealIC2InterfaceBin();
                        iCT=BTestSuck.CountRealIC();
                        sBuffer.printf("RTC Alarm set Arm2 Place to Error bin : Device=%d;",iCT);
                        MyDBIProcess("Message", sBuffer);
                        BTestSuck.SetAllRealIC2InterfaceBin();
                    }
                    Task=65;                                                                                                                                    //if RealTime NG ---->Next Start FullView
                }
            }
            break;
        case 65:
            if(REAL_TIME_CCD==true &&                                           //----- by dell ccd realtime-------------
               MOT[MTestZ1].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false ||
               MOT[MTestZ2].Gali_ReadEncoderMaxRandge(Prod.TestZ1_Safe)==false)
            {
                ShowIndexMotorError(AnsiString("DoTestY_TwoArm32Site65"));
                break;
            }

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestY_TwoArm32Site 65"))
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;            //----- by dell ccd realtime-------------
                COM2->SendCommToVision(COM2->rtFullTOK, true);
                iWaitIndexArm1_32.SetSecAndOn(10);                              //Steven 20250626 32 sites RTC time out alarm 2 -> 10
                Task=66;
            }
            break;
        case 66:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
                COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
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
                    Task=81;
                }
                break;
            }

            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                Task=67;
                break;
            }

            if(iWaitIndexArm1_32.Off())                                         //Steven 20110824 : Real time CCD - 不可以關閉CCD
            {
                ShowErrorMessage("WAR0337", 0, MMIndex, 0, __FUNC__);           //RTC FullT Time Out Error.!
                COM2->DoReleaseAndInspEnd();
                bSendRealCCDSendStart=true;
                Task=65;
            }
            break;
        case 67:
            if(CheckInArmFinishAllPickerAction()==false)                                                                //Steven 20171226 (Wei) : 修改in arm讓開的flag
                return false;
            InitInArmTask();

            if(MoveInArm2XYToWait()==true)                                                                              //如果要移開,而且還沒移到定位
            {
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
                Task=81;
            }
            break;
        case 81:
            if(DeviceForm.ContactMode==TMove ||
               DeviceForm.ContactMode==TMoveDrop ||                             //jou 2012-02-03 新增T Move Drop
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                TMode=true;
            }
            else
            {
                if(IniConfig.bIndexPickupErrStop==true &&                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                   bIndexArm2PickupErrStop==true)
                {
                    TMode=true;
                }
                else
                {
                    TMode=false;
                }
            }

            if(bintered1==true)                                                 //Isaac 20200922 : 紀錄indexArmY encoder值和command值
            {
                bintered1=false;
                RecordIndexPosition(1, 1);                                      //Isaac 20200922 : 紀錄indexArmY encoder值和command值Arm1/Socket
                EncoderTeachingMaxMinCount(1);                                  //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
            }

            #ifdef INDEX_PROTECT_TMOVE
            if(bOverRangeDoTMode==true && bTriger4Indexhome==false)             //Isaac 20201012 : index Y超過範圍，做一次Tmode
            {
                bTriger4Indexhome=true;
                bOverRange4Indexhome=true;
                TrigerIndexAxisHome();                                          //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                return false;
            }
            #endif

            if(MOT[MTestY1].Gali_nnMode_Z1Z2_Down(MOT[MTestZ1].GailSpeed, TMode))
            {
                IndexStatus=Z1_Z2_Down;
                if(LastSet.bD41TestSocketICCheckSkip)                           //打勾的話就跳過檢查
                {
                    Task=100;
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
                            BTestSuck.Suck[i][j].On();
                        }
                        else                                                    //負壓檢查
                        {                                                       //增加關Site時就不開真空偵測，
                            if((FTestSuck.Item[i][j]==NULL_IC ||                //有個盲點就是如果掉落IC剛好掉到關Site的地方，就會無法知道。
                                FTestSuck.Item[i][j]==HAS_NULL_IC) &&           //Steven 20210309 : 有IC的地方不檢查
                                bTestSiteUse[0][i+iNN][j]==true)                //jou 20110504
                            {
                                flag1=false;                                    //jou 20110503 start
                                fiosetview->bIndexSuck[0][i][j]=true;
                                do
                                {
                                    if(flag1==false)
                                        flag1=fiosetview->ProcessIndexSuckDestroy1();

                                    MySleepEx(1, true);
                                }
                                while(flag1==false);                            //jou 20110503 end
                            }

                            //增加關Site時就不開真空偵測，
                            //有個盲點就是如果掉落IC剛好掉到關Site的地方，就會無法知道。
                            if((BTestSuck.Item[i][j]==NULL_IC ||
                                BTestSuck.Item[i][j]==HAS_NULL_IC) &&           //Steven 20210309 : 有IC的地方不檢查
                               bTestSiteUse[1][i][j]==true)                     //jou 20110504
                            {
                                flag1=false;                                    //jou 20110503 start
                                fiosetview->bIndexSuck[1][i][j]=true;
                                do
                                {
                                    if(flag1==false)
                                       flag1=fiosetview->ProcessIndexSuckDestroy2();

                                    MySleepEx(1, true);
                                }
                                while(flag1==false);                            //jou 20110503 end
                            }
                        }
                    }
                }

                DoTestYFrontDelay_32.SetSecAndOn(0.5);
                Task=82;
                flag1=false;
            }
            break;
        case 82:
            if(DoTestYFrontDelay_32.Off() || INDEX_SUCKER_TYPE==1)              //jou 20110503
            {
                bMovOK_1=false;
                Task=84;
            }
            break;
        case 84:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                    //Steven 20140620 : 整合為Function
            bMovOK_1=MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos, iSpeedFast);

            if(bMovOK_1)
            {
                if(CUSTOMER_CODE==CC_Greatek)                                   //Wei 20160413
                    DoTestYFrontDelay_32.SetSecAndOn(5);                        //Steven 20110908 : 上來後也要Delay一下
                else
                    DoTestYFrontDelay_32.SetSecAndOn(0.5);                      //Steven 20110908 : 上來後也要Delay一下
                bMovOK_2=false;
                Task=85;
            }
            break;
        case 85:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                    //Steven 20140620 : 整合為Function
            if(bMovOK_2==false)
                bMovOK_2=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos, iSpeedFast);
            if(bMovOK_2)
            {
                if(CUSTOMER_CODE==CC_Greatek)                                   //Wei 20160413
                    DoTestYFrontDelay_32.SetSecAndOn(5);                        //Steven 20110908 : 上來後也要Delay一下
                else
                    DoTestYFrontDelay_32.SetSecAndOn(0.5);                      //Steven 20110908 : 上來後也要Delay一下
                Task=90;
            }
            break;
        case 90:
            if(DoTestYFrontDelay_32.Off())
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
                                ErrPart+=IndexSuckName[i+iNN][j];               //kevin 20180514 arm 1 +2 取到32 site alarm 位置才會對
                            }

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
            for(int i=0; i<FTestSuck.iShtRow; i++)                              //Steven 20110719 : 不需要全部吹 for 9046 1x4 bug
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC)
                        FTestSuck.Suck[i][j].Off();
                    if(BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC)
                        BTestSuck.Suck[i][j].Off();
                }
            }
            DoTestYFrontDelay_32.SetSecAndOn(5);
            Task=97;
            break;
        case 97:
            if(DoTestYFrontDelay_32.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)                          //Steven 20110719 不需要全部吹 for 9046 1x4 bug
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC)
                            FTestSuck.Suck[i][j].Normal();                      //jou 2011-11-01 開破壞不可能同時開真空，所以OffDestroy -> Normal
                        if(BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC)
                            BTestSuck.Suck[i][j].Normal();                      //jou 2011-11-01 開破壞不可能同時開真空，所以OffDestroy -> Normal
                    }
                }

                if(bSucketHasICError)                                           // sucket has ic error  //Steven 20101214 : 換位置，換到Delay.Off()裡面
                {
                    Task=50;
                    if(bSocketCheckSkip)
                    {
                        bSocketCheckSkip=false;
                        Task=100;
                    }
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 100:
            if(CheckIndexStatus("DoTestY_TwoArm32Site"))
            {
                if(IniConfig.bD21EnableFinishTestUpWait &&                      //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
                   IndexStatus==Z1_Z2_Down)                                     //Steven 20180613 (Jou) : 增加保護機制, 避免撞機
                    Task=105;
                else
                    Task=108;

                bMovOK_1=false;
                goto DoTestYTwoArm_ReStart;                                     //2008/10/20 lee
            }
            break;
        case 105:                                                                                                                                               //jou 2011-10-31 start : 丟測模式與不同速度模式也要支援兩段式上升
            if(DeviceForm.ContactMode==DropContact ||
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveSlowContact)                                                                                                        //Steven 20160130 : TMove Soft contact
            {
                iFinishTestUpWaitHeight_Z1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+IniConfig.iD21FinishTestUpWaitHeight;
                iFinishTestUpWaitHeight_Z2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+IniConfig.iD21FinishTestUpWaitHeight;
            }
            else
            {
                iFinishTestUpWaitHeight_Z1=Prod.TestZ1_Test+IniConfig.iD21FinishTestUpWaitHeight;
                iFinishTestUpWaitHeight_Z2=Prod.TestZ2_Test+IniConfig.iD21FinishTestUpWaitHeight;
            }

            bMovOK_1=MOT[MTestZ1].Gali_Two_ZAxis_Move(iFinishTestUpWaitHeight_Z1, 10000, "DoTestY_TwoArm32Site 105", true, iFinishTestUpWaitHeight_Z2);         //Steven 20150930 : Fixed for 32Site

            if(bMovOK_1)
            {
                Task=106;
                DoTestYFrontDelay_32.SetMSAndOn(IniConfig.iD21FinishTestUpWaitTime);
            }
            break;
        case 106:
            if(DoTestYFrontDelay_32.Off())
                Task=108;
            break;
        case 108:
            bCheckShuttle=false;
            Task=110;

            goto DoTestYTwoArm_ReStart;                                         //2008/10/20 lee
        case 110:
            if(USE_IO_CHANGE_TOQUE==true)  //jou 2012-06-21 Enable index I/O Change Toque
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

                bCheckShuttle=true;
            }

            if(IndexStatus==Z1Up_Z2Down || IndexStatus==Z1Down_Z2Up)
            {
                ShowIndexMotorError(AnsiString("DoTestY_TwoArm32Site110"));
                return false;
            }
            else
            {
//                iCheckFailDown++; //Ifor 20220304 Mark 避免來不及偵測掉料
//                if(iCheckFailDown>CheckFailDownCT)
//                {
//                    iCheckFailDown=0;
                    //jou 2011-08-16 +100 -> +500 drop mode容易造成誤判
                    //jou 2012-01-17 +500 -> +750 疑似有誤判的情況發生，一直來來回回Hang up (備注 : 750條 = 7.5mm )
                    if((MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+750)) ||  //2008/10/20 lee
                       (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)                            )  //Ifor 20210716 add:避免關ARM不偵測掉料
                    {
                        flagSuckErr=false;
                        if(LastSet.iRealDummy==REALLY)
                        {
                            for(int i=0; i<FTestSuck.iShtRow; i++)
                            {
                                for(int j=0; j<FTestSuck.iShtCol; j++)
                                {
                                    if(FTestSuck.Suck[i][j].Enable       &&
                                       FTestSuck.Suck[i][j].SenUsing!="" &&
                                       FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                       FTestSuck.Item[i][j]!=NULL_IC)
                                    {
                                        if(FTestSuck.Suck[i][j].GetStatus()==false)
                                        {
                                            FTestSuck.Suck[i][j].Normal();  //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                            flagSuckErr=true;
                                            bRecIndexDropAlarm1=true;  //jou 2012-01-17 紀錄index Drop alarm
                                            ErrPart+=IndexSuckName[i+iNN][j];  //kevin 20220924  add  真空沒訊號log
                                        }
                                    }

                                    if(BTestSuck.Suck[i][j].Enable       &&
                                       BTestSuck.Suck[i][j].SenUsing!="" &&
                                       BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                       BTestSuck.Item[i][j]!=NULL_IC)
                                    {
                                        if(BTestSuck.Suck[i][j].GetStatus()==false)
                                        {
                                            BTestSuck.Suck[i][j].Normal();  //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                            flagSuckErr=true;
                                            bRecIndexDropAlarm2=true;  //jou 2012-01-17 紀錄index Drop alarm
                                            ErrPart+=IndexSuckName[i][j];  //kevin 20220924  add  真空沒訊號log
                                        }
                                    }
                                }
                            }
                        }

                        if(flagSuckErr)
                        {
                            ShowIndexTime(-2);
                            MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:110"));
                            Task=50;
                            str1.sprintf("DoTestY_TwoArm32Site 110 to 50: bRecIndexDropAlarm1=%d, bRecIndexDropAlarm2=%d :%s", bRecIndexDropAlarm1?1:0, bRecIndexDropAlarm2?1:0 ,ErrPart);  //kevin 20220924  add  真空沒訊號log   //Steven 20201201 : add event log for debug
                            RecordProcess(str1);
                            return false;
                        }
                        else
                        {
                            bRecIndexDropAlarm1=false;  //jou 2012-01-17 紀錄index Drop alarm
                            bRecIndexDropAlarm2=false;  //jou 2012-01-17 紀錄index Drop alarm
                        }
                    }
//                }

                if(IniConfig.bC08_SocketSensor && TestIF_File.bEnSocketSensor)  //kevin 20130504 socket sensor detect error
                {
                    iAZ1=MOT[MTestZ1].Gali_ReadEncoderPos();  //JerryYang 20190327 fix socket sensor誤判問題, 要讀Encoder位置
                    iAZ2=MOT[MTestZ2].Gali_ReadEncoderPos();
                    if(Prod.TestZ1_Test==0)  //jimmychiu 20230830 : add
                        iSocketSenSosPos1=-1000;
                    else
                        iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;
                    if(Prod.TestZ2_Test==0)
                        iSocketSenSosPos2=-1000;
                    else
                        iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;
                    if((iAZ1>iSocketSenSosPos1) && (iAZ2>iSocketSenSosPos2))  //kevin 20140508 socket sensor
                    {
                        for(int i=0; i<TestIF_File.iSocketCount; i++)
                        {
                            if(Sen[SThreadPara.iSocketSensor[i]].Enable && Sen[SThreadPara.iSocketSensor[i]].IsOn())  //On的有殘料
                            {
                                MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:110"));
                                flag=true;
                                iShowSocketSensor=1;
                                str+=IntToStr(i+1)+",";
                            }
                        }

                        if(flag)
                        {
                            ShowIndexTime(-2);
                            sBufferT="Z1Z2_Up: Z1Pos "+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000)+"Z2Pos "+IntToStr(iAZ2)+">"+IntToStr(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000);
                            RecordProcess(sBufferT);  //kevin 20150506
                            Task=50;
                            return false;
                        }
                    }
                }

                if(bOneTimeFlag)
                {
                    bOneTimeFlag=false;
                    iBackUpZ2DownPosition=Prod.TestZ2_Test;
                }

                ShowIndexTime(-1);

                if(IniConfig.bIndexPickupErrStop==false)
                {
                    bIndexArm1PickupErrStop=false;
                    bIndexArm2PickupErrStop=false;
                }

                if(DeviceForm.ContactMode==TMove ||
                   DeviceForm.ContactMode==TMoveDrop ||  //jou 2012-02-03 新增T Move Drop
                   DeviceForm.ContactMode==TMoveDropSlowContact ||
                   DeviceForm.ContactMode==TMoveSlowContact)  //Steven 20160130 : TMove Soft contact
                {
                    TMode=true;
                }
                else
                {
                    if(IniConfig.bIndexPickupErrStop==true &&  //jou 2012-02-29 index pick up error,index arm move to center & alarm
                       bIndexArm2PickupErrStop==true)
                    {
                        TMode=true;
                    }
                    else
                    {
                        TMode=false;
                    }
                }

                if(MOT[MTestY1].Gali_nnMode_Z1Z2_Up(MOT[MTestZ1].GailSpeed, TMode))
                {
                    Task=120;
                }
            }
            break;
        case 120:                                                               //Steven 20140725 : 避免Index Y軸位置不準
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, MOT[MTestY1].GailSpeed, "DoTestY_TwoArm32Site 120"))
            {
                bIndexArm2PickupErrStop=false;
                MOT[MInShuttle1].fCanMoveM=true;
                MOT[MInShuttle2].fCanMoveM=true;
                bCheckShuttle=false;
                bOneTimeFlag=true;
                ShowIndexTime();

                ShowMainScreenPresure(1);                                       //jou 2010-06-23 畫面Z1,Z2 encoder 顯示

                Task=200;
                if(REAL_TIME_CCD && COM2->bCCDDummyRum==false)
                {
                    bRTCArm1HalfViewError=false;                                //Steven 20120206 : RTC重複錯誤
                    COM2->SendCommToVision(COM2->rtArmFinish, false);

                    COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;        //20220815 wei 往上移;兩支arm起來時就可以做RTC 減少index cycle time //Steven 20240919 : unmark for RTC
                    COM2->SendCommToVision(COM2->rtFullTOK, true);
                }

                if(IniConfig.bTModeMotorFree==true)
                {
                    if(TMode==true)                                             //20110923 Tmode 煞車
                    {
                        hTestZ2Delay_32.SetSecAndOn(0.1);
                        Task=130;
                    }
                }
                goto DoTestYTwoArm_ReStart;                                     //2008/10/20 lee
            }
            break;
        case 130:
            if(hTestZ2Delay_32.Off())
            {
                SW[SwBMotorBreaker].Off();
                Task=200;
            }
            break;
        case 200:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].On();
                SW[SwIndexChangeToque2].Off();
            }

            Task=209;
        case 209:
            HangTime.SetSecAndOn(Prod.iHangupMaxTime);                          //Steven 20090827 Start: Hang Up dectector
            bHangTimePause=false;
            f32SiteNeedDestroy=false;
            if(FRCarryKit.UseSiteNoIC() &&
               OutSHT1InLF() &&
               BRCarryKit.UseSiteNoIC() &&
               OutSHT2InLF())
            {
                if(iOneCycle==0 ||                                              //Steven 20151006 : For 32Site One Cycle問題
                   IsInArmOneCycleFinish()==false)
                    f32SiteNeedDestroy=true;
            }
            Task=210;
        case 210:                                                               // 編號不得改變 *******************
            if(IniConfig.bD59_32SitePnpTogether && f32SiteNeedDestroy)          //Steven 20150910 : 32Site 雙Arm一起吸放
            {
                fRearNeedDestroy=!Do32SiteTestDestroyIC();
                fFrontNeedDestroy=fRearNeedDestroy;
            }
            else
            {
                if(fRearNeedDestroy)
                {
                    fRearNeedDestroy=!DoRearTestDestroyIC(true);                //ChungHung 20111115
                }

                if(fFrontNeedDestroy)                                           //Index1 上有IC是否要拋下IC (Index1 已在Shuttle1 上)
                {
                    fFrontNeedDestroy=!DoFrontTestDestroyIC(true);
                }
            }

            if(fFrontNeedDestroy==false && fRearNeedDestroy==false)
            {
                Task=220;
            }
            else
            {
                break;
            }
        case 220:
            f32SiteNeedSuck=false;
            if(InSHT1InRT() && InSHT2InRT())
            {
                if(iCleanOut &&
                   FLCarryKit.UseSiteHasIC() &&                                 //SHUTTLE 1 HAS IC
                   BLCarryKit.UseSiteNoIC() &&                                  //SHUTTLE 2 NO IC
                   InArmSuck.NoIC() &&
                   MOT[MMPlate1].HasIC()==false &&
                   MOT[MMPlate2].HasIC()==false)                                //JerryYang 20230717 : fix 32 site clean out hang up
                {
                    f32SiteNeedSuck=false;
                }
                else
                {
                    if(iOneCycle==0 ||                                          //Steven 20151006 : For 32Site One Cycle問題
                       IsInArmOneCycleFinish()==false)
                        f32SiteNeedSuck=true;
                }
                Task=221;
            }
            else if(PickShuttleIC_Together)
            {
                if(InSHT1InRT() && InSHT2InRT())
                {
                    if(iCleanOut &&
                       FLCarryKit.UseSiteHasIC() &&                             //SHUTTLE 1 HAS IC
                       BLCarryKit.UseSiteNoIC() &&                              //SHUTTLE 2 NO IC
                       InArmSuck.NoIC() &&
                       MOT[MMPlate1].HasIC()==false &&
                       MOT[MMPlate2].HasIC()==false)                            //JerryYang 20230717 : fix 32 site clean out hang up
                    {
                        f32SiteNeedSuck=false;
                    }
                    else
                    {
                        if(iOneCycle==0 ||                                      //Steven 20151006 : For 32Site One Cycle問題
                           IsInArmOneCycleFinish()==false)
                            f32SiteNeedSuck=true;
                    }

                    Task=221;
                }
                else if((fFrontNeedTest || fRearNeedTest) &&                    //Richard 20230417 : fix RTC hang up error.
                        (FTestSuck.HasRealIC() || BTestSuck.HasRealIC()))
                {
                    Task=221;
                }
                else
                {
                    break;
                }
            }
            else
            {
                Task=221;
            }
        case 221:
            if(IniConfig.bD59_32SitePnpTogether &&                                                                      //Steven 20150910 : 32Site 雙Arm一起吸放
               f32SiteNeedSuck)
            {
                fRearNeedSuck=!Do32SiteTestSuckIC();
                fFrontNeedSuck=fRearNeedSuck;
            }
            else
            {
                if(fRearNeedSuck)
                {
                    if(DeviceForm.bSuckShuttleDeviceAfterTested &&
                      (LastSet.iTemperature==Tempture_Hot ||
                       LastSet.iTemperature==Tempture_AmbientHot) &&
                       DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)                                          //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能  //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                    {
                        if(fFrontNeedTest==false)
                            fRearNeedSuck=!DoRearTestSuckIC();
                    }
                    else
                    {
                        fRearNeedSuck=!DoRearTestSuckIC();
                    }
                }

                if(fFrontNeedSuck)                                                                                      //Index1 是否需要吸取IC
                {
                    if(DeviceForm.bSuckShuttleDeviceAfterTested &&
                       (LastSet.iTemperature==Tempture_Hot ||
                        LastSet.iTemperature==Tempture_AmbientHot) &&
                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false)                                         //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能  //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
                    {
                        if(fRearNeedTest==false)
                            fFrontNeedSuck=!DoFrontTestSuckIC();
                    }
                    else
                    {
                        fFrontNeedSuck=!DoFrontTestSuckIC();
                    }
                }
            }

            if(fFrontNeedSuck==false &&
               fRearNeedSuck==false)
            {
                bZ1PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                bZ2PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing

                if(FTestSuck.UseSiteHasIC() && FTestSuck.AlreadyTest()==false)
                {
                    fFrontNeedTest=true;
                    InitFTestSuckTestICTask();
                }

                if(BTestSuck.UseSiteHasIC() && BTestSuck.AlreadyTest()==false)
                {
                    fRearNeedTest=true;
                    InitBTestSuckTestICTask();
                }

                Task=225;
            }
            else
            {
                break;
            }
        case 225:                                                               //----- by dell ccd realtime-------------
            if(IndexStatus==Z1_Z2_Normal && REAL_TIME_CCD==true && !COM2->bCCDDummyRum)
            {
                //----- by dell ccd realtime-------------
//                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;          //20220815 wei 往上移;兩支arm起來時就可以做RTC 減少index cycle time //Steven 20240919 : mark for RTC
//                COM2->SendCommToVision(COM2->rtFullTOK, true);
                iWaitIndexArm1_32.SetSecAndOn(10);                              //Steven 20250626 32 sites RTC time out alarm 4 -> 10
                //---------------------------------------
                Task=226;
            }
            else
            {
                Task=230;
            }
            //---------------------------------------

            goto DoTestYTwoArm_ReStart;                                         //2008/10/20 lee
        case 226:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
                COM2->RTC_ResetAlarm();     //wei 20221222 RTC ARM Error
                COM2->DoReleaseAndInspEnd();
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                Task=230;
                goto DoTestYTwoArm_ReStart;                                     //2008/10/20 lee
            }

            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                Task=50;
            }

            if(iWaitIndexArm1_32.Off())
            {                                                                   //Steven 20110824 : Real time CCD - 不可以關閉CCD
                ShowErrorMessage("WAR0337", 0, MMIndex, 0, __FUNC__);           //RTC FullT Time Out Error.!
                COM2->DoReleaseAndInspEnd();
                bSendRealCCDSendStart=true;
                Task=225;
            }
            break;
        case 230:                                                                                                       //Steven 20190903 : 補上index下壓時檢查掉料
//            iCheckFailDown++;
//            if(iCheckFailDown>CheckFailDownCT)//Ifor 20220304 Mark 避免來不及偵測掉料
//            {
//                iCheckFailDown=0;
                //jou 2011-08-16 +100 -> +500 drop mode容易造成誤判
                //jou 2012-01-17 +500 -> +750 疑似有誤判的情況發生，一直來來回回Hang up (備注 : 750條 = 7.5mm )
                if((MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+750)) ||                      //2008/10/20 lee
                   (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1))                                                  //Ifor 20210716 add:避免關ARM不偵測掉料
                {
                    //Steven 20101214 Start
                    flagSuckErr=false;
                    if(LastSet.iRealDummy==REALLY)
                    {
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(FTestSuck.Suck[i][j].Enable       &&
                                   FTestSuck.Suck[i][j].SenUsing!="" &&
                                   FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   FTestSuck.Item[i][j]!=NULL_IC)
                                {
                                    if(FTestSuck.Suck[i][j].GetStatus()==false)
                                    {
                                        FTestSuck.Suck[i][j].Normal();                                                  //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                        flagSuckErr=true;
                                        bRecIndexDropAlarm1=true;                                                       //jou 2012-01-17 紀錄index Drop alarm
                                    }
                                }

                                if(BTestSuck.Suck[i][j].Enable       &&
                                   BTestSuck.Suck[i][j].SenUsing!="" &&
                                   BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   BTestSuck.Item[i][j]!=NULL_IC)
                                {
                                    if(BTestSuck.Suck[i][j].GetStatus()==false)
                                    {
                                        BTestSuck.Suck[i][j].Normal();                                                  //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                        flagSuckErr=true;
                                        bRecIndexDropAlarm2=true;                                                       //jou 2012-01-17 紀錄index Drop alarm
                                    }
                                }
                            }
                        }
                    }

                    if(flagSuckErr)
                    {
                        MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:230"));
                        Task=50;

                        return false;
                    }
                    else
                    {
                        bRecIndexDropAlarm1=false;                                                                      //jou 2012-01-17 紀錄index Drop alarm
                        bRecIndexDropAlarm2=false;                                                                      //jou 2012-01-17 紀錄index Drop alarm
                    }
                    //----- by dell ccd realtime-------------
                    //Steven 20101214 End
                }
//            }

            if(DeviceForm.ContactMode==TMove ||
               DeviceForm.ContactMode==TMoveDrop ||                                                                     //jou 2012-02-03 新增T Move Drop
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                                                                //Steven 20160130 : TMove Soft contact
            {
                TMode=true;
            }
            else
            {
                if(IniConfig.bIndexPickupErrStop==true &&                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                   bIndexArm2PickupErrStop==true)
                {
                    TMode=true;
                }
                else
                {
                    TMode=false;
                }
            }

            if(bintered1==true)                                                                                         //Isaac 20200922 : 紀錄indexArmY encoder值和command值
            {
                bintered1=false;
                RecordIndexPosition(1, 1);                                                                              //Isaac 20200922 : 紀錄indexArmY encoder值和command值Arm1/Socket
                EncoderTeachingMaxMinCount(1);                                                                          //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
            }

            #ifdef INDEX_PROTECT_TMOVE
            if(bOverRangeDoTMode==true && bTriger4Indexhome==false)                                                     //Isaac 20201012 : index Y超過範圍，做一次Tmode
            {
                bTriger4Indexhome=true;
                bOverRange4Indexhome=true;
                TrigerIndexAxisHome();                                                                                  //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                return false;
            }
            #endif

            if(MOT[MTestY1].Gali_nnMode_Z1Z2_Down(MOT[MTestZ1].GailSpeed, TMode))
            {
//                TempIndexTime[6]=MyTickCount();                               //Steven 20150917 : 修正時間顯示
                if(fFrontNeedTest || fRearNeedTest)
                {
                    fTwoArmNeedTest=true;
                    InitTestSuckTestIC_TwoArm32Site_Task();
                }
                Task=240;

                if(Prod.TestZ2_Drop_Offset!=0)                                                                          //DropContact or DirectContactModeDiffentSpeed  //jou 2015-04-29 修正32 sites drop mode index 撞機
                {
                    Task=300;
                    break;
                }
            }
            break;
        case 240:                                                               //開始測試
            fMain->SendMSG_CMD(MSG_CMD_Arm1Down);                               //Steven 20210225 : Add GPIB LOG
            Task=241;
        case 241:
            if(fTwoArmNeedTest)
            {
                fTwoArmNeedTest=!DoTestSuckTestIC_TwoArm32Site();
                CheckIndexAllSuckICFallDown(false, true);                                                               //Steven 20110725 : 修改負壓檢查方式
            }

            if(fTwoArmNeedTest==false)
            {
                #ifdef ASE_KaohSiung
                if(TestIF_File.bEnableReadAndCheckTorque &&                                                             //KenHsieh 20220409 : nn Mode扭力紀錄
                   TorqueUseHPComCard &&
                   COM2->bGetValue &&
                   COM2->bGetValue1)                                                                                    //kevin 20210804 change
                {
                    if(NewCheckAndRecodeTorque(0)==false)
                    {
                        if(bRetryReadToqu==false)                                                                       //kevin 20210419 重讀扭力
                        {
                            bRetryReadToqu=true;
                            bNeedCheckIndexToque=false;                                                                 //KEVIN 20210419 ADD TORQUE
                            bNeedCheckIndexToque1=false;
                            fMain->chkReadTorque1->Checked=true;
                            COM2->ReadIndexTorqueSetting(0);
                        }

                        if(TorqueTimeOutDelay.Off()==false)                                                             //kevin 2021018 扭力
                           return false;
                    }

                    if(NewCheckAndRecodeTorque(1)==false)
                    {
                        if(bRetryReadToqu==false)                                                                       //kevin 20210419 重讀扭力
                        {
                            bRetryReadToqu=true;
                            bNeedCheckIndexToque=false;                                                                 //KEVIN 20210419 ADD TORQUE
                            bNeedCheckIndexToque1=false;
                            fMain->chkReadTorque2->Checked=true;
                            COM2->ReadIndexTorqueSetting(1);
                        }

                        if(TorqueTimeOutDelay.Off()==false)                                                             //kevin 2021018 扭力
                           return false;
                    }
                }
                #endif

                if(USE_IO_CHANGE_TOQUE==true)                                                                           //jou 2012-06-21 Enable index I/O Change Toque
                {
                    SW[SwIndexChangeToque1].Off();
                    SW[SwIndexChangeToque2].Off();
                }

                if(IniConfig.bTModeMotorFree==true)
                {
                    SW[SwBMotorBreaker].On();                                                                           //20110923 Tmode 煞車
                    MySleep(200);
                }

                #ifdef ASE_KaohSiung
                if(TestIF_File.bEnableReadAndCheckTorque &&
                   TorqueUseHPComCard)                                                                                  //KenHsieh 20220409 : nn Mode扭力紀錄
                {
                    if(COM2->bGetValue==false || COM2->bGetValue1==false)
                        return false;
                }
                #endif
                if(DeviceForm.VacuumMode==VacuumONMode)
                {
                    iHangupCTArm1_32=0;
                    return true;
                }
                else
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)                                                              //ARM->Index
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]!=NULL_IC &&
                               FTestSuck.Item[i][j]!=HAS_NULL_IC)                                                       //Steven 20180402 (Jou) : 補上
                            {
                                FTestSuck.Suck[i][j].Error=false;
                                bFTestSuckUse[i][j]=true;
                            }
                            bFTSuckFinish[i][j]=false;

                            if(BTestSuck.Item[i][j]!=NULL_IC &&
                               BTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Error=false;
                                bBTestSuckUse[i][j]=true;
                            }
                            bBTSuckFinish[i][j]=false;                                                                  //Steven 20110301
                        }
                    }
                    Task=250;
                    iHangupCTArm1_32=0;
                }
            }

            if(HangTime.Off() && TestISTimeOut==false && bHangTimePause==false)
            {
                if(AMR.NeedAMRTransport())                                                                              //Sam 20250423 : AMR 搬運花費時間太久可能會報警需要 By Pass
                {
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    break;
                }

                if(IniConfig.bTModeMotorFree==true)
                {
                    SW[SwBMotorBreaker].On();                                                                           //20110923 Tmode 煞車
                    MySleep(200);
                }

                if(CheckHeaterOK()==false)                                                                              //Steven 20250116 : 確認HeaterOK
                {
                    bHangTimePause=true;
                    break;                                                                                              //kevin 20161102 break;
                }

                bNoUseAutoRecord=true;                                                                                  //wei 20160311

                RecordProcess("Auto State Record by DoTestY_TwoArm32Site");
                fMain->DoStateRecord(0, false);                                                                         //Steven 20120705 : Hang Up時，會自動存畫面     //KenHsieh 20230116 : 區分手動或自動(sbclick -> Function)

                #ifdef DEBUG_HANGUP_NO_HOME
                    ShowErrorMessage("JAM0316", K_SKIP, MTestZ1);
                    Task=209;
                #else
                    iHangupCTArm1_32++;
                    if(iHangupCTArm1_32>1)
                    {
                        iHangupCTArm1_32=0;
                        ShowErrorMessage("JAM0316", K_SKIP, MTestZ1);
                        Task=209;
                    }
                    else
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                             //kevin 20160722 ASE 高雄取消
                        {
                            ShowErrorMessage("JAM0316", K_SKIP, MTestZ1);
                            Task=209;
                        }
                        else
                        {
                            MyDBIProcessNew("Motion", "WAR2206", "Auto homing", "0316");                                //Steven 20120705 : Hang Up時，會自動存畫面
                            bHomeByStart=true;
                            fAllMotorHome=false;
                        }
                    }
                #endif
                break;
            }
            break;
        case 250:                                                               //丟測關真空模式, 要先把IC吸回來
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

                    if(bBTSuckFinish[i][j]==false)                              //Steven 20110301
                    {
                        if(bBTestSuckUse[i][j])
                        {
                            if(BTestSuck.Suck[i][j].Suck())
                            {
                                bBTSuckFinish[i][j]=true;                       //Steven 20110301
                                bBTestSuckUse[i][j]=false;
                            }
                            else if(BTestSuck.Suck[i][j].Error)                 //jou 2011-08-16
                            {
                                bBTSuckFinish[i][j]=true;                       //Steven 20110301
                            }
                            else
                            {
                                bCheckAllSuck=false;
                            }
                        }
                        else
                        {
                            bBTSuckFinish[i][j]=true;                           //Steven 20110301
                        }
                    }
                }
            }

            flagSuckErr=true;                                                   //Steven 20110301 : Start
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if((bBTSuckFinish[i][j]==false) ||
                       (bFTSuckFinish[i][j]==false))
                        flagSuckErr=false;
                }
            }

            if(flagSuckErr)                                                     //Steven 20110301
            {
                if(bCheckAllSuck)
                {
                    return true;
                }
            }
            break;
        case 300:                                                               //丟測模式, 要在往下Offset, 這裡先關閉真空, 放下IC
            if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||         //Eliot 2011_0318 Start
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                Task=320;
                break;
            }                                                                   //Eliot 2011_0318 end

            bFTestSuckDrop=true;                                                //jou 2011-12-13 丟測才需要設成true
            fFrontNeedSuckIC=false;
            bBTestSuckDrop=true;
            fRearNeedSuckIC=false;

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==HAS_HOT_IC || FTestSuck.Item[i][j]==HAS_IC)
                        FTestSuck.Suck[i][j].Off();
                    if(BTestSuck.Item[i][j]==HAS_HOT_IC || BTestSuck.Item[i][j]==HAS_IC)
                        BTestSuck.Suck[i][j].Off();
                }
            }
            DoTestYFrontDelay_32.SetSecAndOn(Prod.TestZ_Drop_Wait);             // delay 0.3 sec for ic down        //Steven 20140909 : 換到迴圈外面
            Task=310;
            break;
        case 310:                                                               //關閉吹氣
            if(DoTestYFrontDelay_32.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_HOT_IC || FTestSuck.Item[i][j]==HAS_IC)
                            FTestSuck.Suck[i][j].Normal();
                        if(BTestSuck.Item[i][j]==HAS_HOT_IC || BTestSuck.Item[i][j]==HAS_IC)
                            BTestSuck.Suck[i][j].Normal();
                    }
                }

                bMovOK_1=false;
                if(IniConfig.bC08_SocketSensor &&                               //Steven 20180313 (Jou) : 使用Socket Sensor驗證置偏
                   TestIF_File.bEnSocketSensor &&
                   TestIF_File.bCheckSocketFloating)
                {
                    Task=311;
                    break;
                }
                Task=320;
            }
            break;
        case 311:                                                               //Steven 20180313 (Jou) : 使用Socket Sensor驗證置偏
            flag=false;
            for(int i=0; i<TestIF_File.iSocketCount; i++)
            {
                if(TestIF_File.iSensorCheckType[i]==2 &&                        //Steven 20200420 : Socket Sensor功能可以選
                   Sen[SThreadPara.iSocketSensor[i]].IsOn())                    //On的有置偏
                {
                    MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case:311"));
                    flag=true;
                    iShowSocketSensor=2;
                    str+=IntToStr(i+1)+",";
                }
            }

            if(flag)
            {
                Task=312;
                return false;
            }
            else
            {
                Task=320;
            }
            break;
        case 312:                                                               //丟測放下IC後,偵測到置偏,Index往上
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestY_TwoArm32Site 312"))
            {
                Task=313;
            }
            break;
        case 313:                                                               //Y軸分開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 50000, "DoTestY_TwoArm32Site 313"))
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

                ShowErrorMessage("WAR0323", K_RETRY, MTestZ2, false, str);      //Socket detect device floting error
                Task=315;
            }
            break;
        case 315:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Middle, 50000, "DoTestY_TwoArm32Site 315"))
            {
                Task=316;
            }
            break;
        case 316:                                                               //丟測放下IC後,偵測到置偏排除後, Index往下移動
            iFinishTestUpWaitHeight_Z1=Prod.TestZ1_Test;
            iFinishTestUpWaitHeight_Z2=Prod.TestZ2_Test;

            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(iFinishTestUpWaitHeight_Z1, 10000, "DoTestY_TwoArm32Site 316", true, iFinishTestUpWaitHeight_Z2))
            {
                Task=311;
            }
            break;
        case 320:  //往下移動Contact
            if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||  //Eliot 2011_0318 Start
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveSlowContact ||  //Steven 20160130 : TMove Soft contact
               DeviceForm.ContactMode==TMoveDropSlowContact)
            {
                bMovOK_1=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, ((MOT[MTestZ1].GailSpeed*Prod.TestZ_Drop_Speed)/100), "DoTestY_TwoArm32Site 320", true, Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset);  //Steven 20150930 : Fixed for 32Site
                if(bMovOK_1)
                {
                    if(DeviceForm.ContactMode==DropContactModeDiffentSpeed ||  //JerryYang 20201022 修正32 site丟測模式沒有吸真空的問題
                       DeviceForm.ContactMode==TMoveDropSlowContact)
                    {
                        Task=330;  //kevin 20191210
                    }
                    else
                    {
                        //Task=240;                                             //Steven 20110511 //jou 2015-04-29 修正32 sites drop mode index 撞機
                        Task=325;  //kevin 20191210
                    }
                }
            }
            else
            {
                bMovOK_1=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, MOT[MTestZ1].GailSpeed, "DoTestY_TwoArm32Site 320", true, Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset);  //Steven 20150930 : Fixed for 32Site
                if(bMovOK_1)
                    Task=325;
            }
            break;
        case 325:                                                               //kevin 20191210 add SOFT EP  充氣
            if(DeviceForm_File.ContactMode==DirectContactSoftEP ||
               DeviceForm_File.ContactMode==DropContactSoftEP)
            {                                                                   //kevin 20191210 Soft Contact mode
//                bContSoftEpSwitch(0, true);                                     //ARM1 在 測區  浮動頭充氣
//                bContSoftEpSwitch(1, true);                                     //ARM2 在 測區  浮動頭充氣
                EPSwitchOnOff(eEPSwBoth);                                       //Steven 20250417 : fixed for [D58]

                if(DeviceForm.fAireForce==0)
                    DeviceForm.fAireForce=DeviceForm.dPress;                    //kevin 20220505 add 32 site soft EP Pick up error

                ADAM_WriteVoltage(DeviceForm.fAireForce);                       //kevin 20220505 add  EP 充氣
            }

            if(DeviceForm.ContactMode==DirectContactModeDiffentSpeed ||         //Eliot 2011_0318 Start
               DeviceForm.ContactMode==DropContactModeDiffentSpeed ||
               DeviceForm.ContactMode==TMoveSlowContact ||                      //Steven 20160130 : TMove Soft contact
               DeviceForm.ContactMode==TMoveDropSlowContact)
            {
                Task=240;                                                       //Steven 20110511 //jou 2015-04-29 修正32 sites drop mode index 撞機
                break;
            }
            Task=330;
            break;
        case 330:                                                               //真空On模式, 要把IC先吸住
            if(DeviceForm.VacuumMode==VacuumONMode)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_HOT_IC || FTestSuck.Item[i][j]==HAS_IC)
                        {
                            if(INDEX_SUCKER_TYPE==1)                            //Steven 20111202
                            {
                                fiosetview->bIndexSuck[0][i][j]=true;
                                bIndexCheckNoStopVaccum=true;                   //Sam 20220816 : 增加保護避免暫停 AllSite 掉料
                            }
                            else
                            {
                                FTestSuck.Suck[i][j].On();
                            }
                        }

                        if(BTestSuck.Item[i][j]==HAS_HOT_IC || BTestSuck.Item[i][j]==HAS_IC)
                        {
                            if(INDEX_SUCKER_TYPE==1)                            //Steven 20111202
                            {
                                fiosetview->bIndexSuck[1][i][j]=true;
                                bIndexCheckNoStopVaccum=true;                   //Sam 20220816 : 增加保護避免暫停 AllSite 掉料
                            }
                            else
                            {
                                BTestSuck.Suck[i][j].On();
                            }
                        }
                    }
                }
            }

            bIndexSuckCheck1=false;
            bIndexSuckCheck2=false;
            if(Prod.TestZ_Drop_Wait<0.1)                                        //Steven 20220622 : 改成可以設定
                DoTestYFrontDelay_32.SetSecAndOn(0.1);                          // delay 0.1 sec for ic down
            else
                DoTestYFrontDelay_32.SetSecAndOn(Prod.TestZ_Drop_Wait);
            Task=340;
//            break;
        case 340:
            if(INDEX_SUCKER_TYPE==1)
            {
                if(bIndexSuckCheck1==false)
                    bIndexSuckCheck1=fiosetview->ProcessIndexSuckDestroy1(1);   //JerryYang 20250120 : modify
                if(bIndexSuckCheck2==false)
                    bIndexSuckCheck2=fiosetview->ProcessIndexSuckDestroy2(1);   //JerryYang 20250120 : modify
            }

            if(DoTestYFrontDelay_32.Off() &&
               (bIndexSuckCheck1 &&
                bIndexSuckCheck2))                                              //Steven 20220622 : || --> && 修正丟測時暫停會掉料
            {
                bIndexCheckNoStopVaccum=false;                                  //Sam 20220902 : 修正 All Site 掉料
                bBTestSuckDrop=false;
                Task=240;                                                       //jou 2015-04-29 修正32 sites drop mode index 撞機
            }
            break;
        case 2000:
            if(REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)
            {
                Task=65;
            }
            else
            {
                Task=81;
            }
            break;
        case 12000:                                                             //jou 2014-06-24 RTC 自動進行Model驗證 start
            Do32RTCAutoModelVerify(true);
            Task=12010;
            break;
        case 12010:
            if(Do32RTCAutoModelVerify(false))
            {
                Task=225;
            }
            break;
        default:
            break;
    }

    return false;
}
//------------------------------------------------------------------------------
int iTestSuckTestIC_TwoArm32Site_Task=1;
void InitTestSuckTestIC_TwoArm32Site_Task()                                     //2013-01-15    Dell    Add nn Mode
{
    iTestSuckTestIC_TwoArm32Site_Task =1;
}
//==============================================================================
TQPF_Timer DoFTestSuckTestICDelay_32;
extern int SendTestResultToHttp();
bool DoTestSuckTestIC_TwoArm32Site()                                            //2013-01-15    Dell    Add nn Mode
{
    static int iTestCount=0, iReadTIntervel=0, iReadCount=0, iXMLRetryCnt=0;
    static bool bFirstIn=true;                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
    static DWORD dwNowTickCount, dwOldTickCount=0;

    int &Task=iTestSuckTestIC_TwoArm32Site_Task;
    int iMaxDoubleContact=0, ret=0, ret2=0, iTestCh=0;
    int iErrCnt=0;
    int iNN=IsNNMode();
    bool bHasFailIC=false;
    bool bAlreadyTested;
    AnsiString ErrPart1, ErrPart2, ErrCnt;

    switch(Task)
    {
        case 1:
            iXMLRetryCnt=0;                                                     //Steven 20201102 : For Murata資料上拋
            bAlreadyTested=false;                                               //Steven 20170517 (jou) : 新增保護,如果IC已經測過就離開
            ErrPart1="";
            ErrPart2="";
            iTestCh=0;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]!=NULL_IC     &&
                       FTestSuck.Item[i][j]!=HAS_NULL_IC)
                    {
                        if(FTestSuck.Item[i][j]>=TEST_PASS)
                            bAlreadyTested=true;

                        if((CosFunction.bBarcodeErrNoTestAndShowH==true ||      //jou 20191007 : Barcode Error No Test & Show "H"
                            TestIF_File.iNoCodeDeviceToErr==2) &&               //Steven 20200909 : 將2DID all site fail變成選項
                           TestIF_File.bEnableBarCode==true &&
                           (FTestSuck.cDeviceInf[i][j]==asBarCodeErrorSend ||
                            FTestSuck.cDeviceInf[i][j]==""))
                        {
                            ;
                        }
                        else
                        {
                            iTestCh++;                                          //Steven 20250318 : 先統計有幾個site要測試
                        }
                    }

                    if(BTestSuck.Item[i][j]!=NULL_IC     &&
                       BTestSuck.Item[i][j]!=HAS_NULL_IC)
                    {
                        if(BTestSuck.Item[i][j]>=TEST_PASS)
                            bAlreadyTested=true;

                        if((CosFunction.bBarcodeErrNoTestAndShowH==true ||      //jou 20191007 : Barcode Error No Test & Show "H"
                            TestIF_File.iNoCodeDeviceToErr==2) &&               //Steven 20200909 : 將2DID all site fail變成選項
                           TestIF_File.bEnableBarCode==true &&
                           (BTestSuck.cDeviceInf[i][j]==asBarCodeErrorSend ||
                            BTestSuck.cDeviceInf[i][j]==""))
                        {
                            ;
                        }
                        else
                        {
                            iTestCh++;                                          //Steven 20250318 : 先統計有幾個site要測試
                        }
                    }
                }
            }

            if(bAlreadyTested==true)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        ErrPart1+=IndexSuckName[i+iNN][j]+", ";
                        ErrPart2+=IndexSuckName[i][j]+", ";
                        if(FTestSuck.Item[i][j]!=NULL_IC     &&
                           FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                           FTestSuck.Item[i][j]<TEST_PASS)
                        {
                            FTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                        }

                        if(BTestSuck.Item[i][j]!=NULL_IC     &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                           BTestSuck.Item[i][j]<TEST_PASS)
                        {
                            BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                        }
                    }
                }
                ShowMyMessage("IC in index already tested, abort process.", ErrPart1+"; "+ErrPart2);
                return true;
            }

            if(iTestCh==0)                                                      //Steven 20250318 : 如果都沒要測試就return true;
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]!=NULL_IC &&
                           FTestSuck.Item[i][j]<TEST_PASS &&
                           FTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            if((CosFunction.bBarcodeErrNoTestAndShowH==true ||  //jou 20191007 : Barcode Error No Test & Show "H"
                                TestIF_File.iNoCodeDeviceToErr==2) &&           //Steven 20200909 : 將2DID all site fail變成選項
                               TestIF_File.bEnableBarCode==true &&
                               (FTestSuck.cDeviceInf[i][j]==asBarCodeErrorSend ||
                                FTestSuck.cDeviceInf[i][j]==""))
                            {
                                FTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                FTestSuck.iBinData[i][j]=iTestBinCount;
                                FTestSuck.PordRec[i][j].AddTestResultRecord(iTestBinCount, FTestSuck.cSBin[i][j], "NonTestToRBin");
                            }
                        }

                        if(BTestSuck.Item[i][j]!=NULL_IC &&
                           BTestSuck.Item[i][j]<TEST_PASS &&
                           BTestSuck.Item[i][j]!=HAS_NULL_IC)
                        {
                            if((CosFunction.bBarcodeErrNoTestAndShowH==true ||  //jou 20191007 : Barcode Error No Test & Show "H"
                                TestIF_File.iNoCodeDeviceToErr==2) &&           //Steven 20200909 : 將2DID all site fail變成選項
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

            if(IniConfig.bD01EnableReadTorque &&                                //Steven 20100617 Start: Add form 9080A for 即時更新扭力值
               IniConfig.iD01ReadTorqueTimeCount>0)
            {
                iReadTIntervel++;
                if(iReadTIntervel>IniConfig.iD01ReadTorqueTimeCount)
                {
                    iReadTIntervel=0;
                    bReadRearTestArmTorque=true;
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

            if(bReadRearTestArmTorque)
            {
                bReadRearTestArmTorque=false;
                fMain->chkReadTorque1->Checked=true;
                fMain->chkReadTorque2->Checked=true;
                Task=5000;
                break;
            }
            else
            {
                if(IsNNMode()==NN_1Row)
                    TestSocket.MergeSuck1Row(BTestSuck, FTestSuck);
                else
                    TestSocket.MergeSuck(BTestSuck, FTestSuck ,false);
                bDoubleContact=false;
                InitTestTask();
                iTestCount=0;
                ZeroMemory(iReContactCnt, sizeof(iReContactCnt));               //Steven 20231205 : 計算某site contact 次數

                if(CUSTOMER_CODE==CC_MTI ||
                   CUSTOMER_CODE==CC_PTI)                                       //Sam 20190429 : Add CC_PTI_NEWWORK
                {
                    InitDoStartMode();                                          //ChungHung 聚成 Init Test Start
                }

                Task=2200;
            }
        case 2200:
            if(CUSTOMER_CODE==CC_MTI ||                                         //ChungHung 聚成 Test Start
               CUSTOMER_CODE==CC_PTI)                                           //Sam 20190429 : Add CC_PTI_NEWWORK
            {
                if(DoStartMode(fAutomation->TestMode)==false)
                    return false;
            }

            ProcessStartTestData(0);
            SetTestTimeOutTimer(0);                                             //Steven 20200407 : 整合Time Out時間設定

            HangTime.SetSecAndOn(Prod.iHangupMaxTime);

            if(CUSTOMER_CODE==CC_MTI ||
               CUSTOMER_CODE==CC_PTI)                                           //Sam 20190429 : Add CC_PTI_NEWWORK
            {
                InitDoEndMode();                                                //ChungHung 聚成 only
            }

            if(IniConfig.bL10IndexTestlogTemp)                                  //kevin 20190323 : index 測試時才記錄溫度
                TemperatureStorageLog(1);                                       //kevin 20190323 add Steven 20140617 : for 海思
            Task=2400;
        case 2400:                                                                                                                                              // 編號不得改變 *******************
            if(dwOldTickCount==0)
            {
                dwOldTickCount=MyTickCount();
                dwNowTickCount=dwOldTickCount;
                SetTestTimeOutTimer(0);                                                                                                                         //Steven 20200407 : 整合Time Out時間設定
                HangTime.SetSecAndOn(Prod.iHangupMaxTime);
            }
            else
            {
                dwNowTickCount=MyTickCount();
                if(dwNowTickCount<dwOldTickCount)
                {
                    dwOldTickCount=dwNowTickCount;                                                                                                              //Jou 20101018 Start : 重新設定Time Out時間時,Hang Up時間也要重設
                    SetTestTimeOutTimer(0);                                                                                                                     //Steven 20200407 : 整合Time Out時間設定
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                }
                else
                {
                    if(bHandlerPause || bTesterSendPause)
                    {
                        if(CosFunction.bStopMustTestTimeOut==false ||
                           bEnterTestIF==true ||
                           bTesterSendPause)
                        {
                            bEnterTestIF=false;
                            SetTestTimeOutTimer(0);                                                                                                             //Steven 20200407 : 整合Time Out時間設定
                        }

                        HangTime.SetSecAndOn(Prod.iHangupMaxTime);
                    }
                    dwOldTickCount=dwNowTickCount;
                }
            }

            if(CosFunction.bOutShtLoseICSetErrUntilOneCycle==true &&                                                                                            //Steven 20210904 : 補上32site
               bFirstIn==true)                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
            {
                bFirstIn=false;
                if(bOutShtLoseICNeedSetErrBin==true)
                {
                    if(IsNNMode()==NN_1Row)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(bTestSiteNeedSetErrBin[2][j]==true &&                                                                                            //Shuttle 1
                               TestSocket.Item[1][j]!=NULL_IC     &&
                               TestSocket.Item[1][j]!=HAS_NULL_IC &&
                               TestSocket.Item[1][j]<TEST_PASS    )
                            {
                                TestSocket.SetItemData(1, j, TEST_PASS+iTestBinCount);
                                iErrCnt++;
                            }

                            if(bTestSiteNeedSetErrBin[0][j]==true &&                                                                                            //Shuttle 2
                               TestSocket.Item[0][j]!=NULL_IC     &&
                               TestSocket.Item[0][j]!=HAS_NULL_IC &&
                               TestSocket.Item[0][j]<TEST_PASS    )
                            {
                                TestSocket.SetItemData(0, j, TEST_PASS+iTestBinCount);
                                iErrCnt++;
                            }
                        }
                    }
                    else
                    {
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(bTestSiteNeedSetErrBin[i+2][j]==true &&
                                   TestSocket.Item[i+2][j]!=NULL_IC     &&
                                   TestSocket.Item[i+2][j]!=HAS_NULL_IC &&
                                   TestSocket.Item[i+2][j]<TEST_PASS)
                                {
                                    TestSocket.SetItemData(i+2, j, TEST_PASS+iTestBinCount);
                                    iErrCnt++;
                                }

                                if(bTestSiteNeedSetErrBin[i][j]==true &&
                                   TestSocket.Item[i][j]!=NULL_IC     &&
                                   TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                   TestSocket.Item[i][j]<TEST_PASS)
                                {
                                    TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                    iErrCnt++;
                                }
                            }
                        }
                    }

                    if(iErrCnt>0)
                    {
                        ErrCnt.sprintf("Shuttle lose IC, Arm 1/2 set to error Bin (before test) : %d pcs", iErrCnt);
                        MyDBIProcess("Message", ErrCnt);
                    }
                }
            }

            bFinshTest=false;

            if(bResetMode ||                                                                                                                                    //jou 981219 start : reset mode
               bResetModeAndCleanOut ||                                                                                                                         //JerryYang 20151007 : ResetMode可選OneCycle或CleanOut
               (bOneTimeHotPlateCheckAll &&                                                                                                                     //Steven 20120315 : CleanOut後的HotPlate Check, 要放到Error Bin
                IniConfig.bE39_1PutTheDevicesToErrorBin &&
                bHotPlateCheckNeedTrayFeed))
            {
                ret=1;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC &&
                           TestSocket.Item[i][j]<TEST_PASS)
                        {
                            if(bDoEmptySocketOneCycle)                                                                                                          //Steven 20220817 : Bin of ESC function iTestBinCount --> IniConfig.iI41_BinOfESC
                            {
                                iTesterBIN[i][j]=IniConfig.iI41_BinOfESC;
                            }
                            else
                            {
                                iTesterBIN[i][j]=iTestBinCount;
                            }
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
                        ret=ProcessTestResult(0);
                    }
                }
                else
                {
                    ret=ProcessTestResult(0);
                }
            }

            if(ret==1)
            {
                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }

                bInitialMaxTime=false;                                                                                                                          //jou 2011-11-09 增加initial max time set
                SetHangupMaxTime();                                                                                                                             //Wei 20230224 : 重置死機時間
                if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)                                                                                              //Sam 20190429 : Add CC_PTI_NEWWORK
                {
                    if(DoEndMode(fAutomation->TestMode)==false)                                                                                                 //ChungHung 聚成 Test End
                        return false;
                }

                SW[SwTesterPower].Off();
                bHasFailIC=false;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(IniConfig.bD22_4_PassBinCanDoubleContact)                                                                                            //JerryYang 20230909 : pass bin也可以設定Double contact
                        {
                            if(TestSocket.Item[i][j]!=NULL_IC  &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC &&
                               TestSocket.bNeedReTest[i][j])
                            {
                                bHasFailIC=true;
                            }
                        }
                        else
                        {
                            if(TestSocket.Item[i][j]!=NULL_IC  &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC &&
                               TestSocket.bPass[i][j]==false   &&
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
                            ProcessCount(0);
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
                        ProcessShowTestStatus(0);
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(TestSocket.Item[i][j]!=NULL_IC     &&
                                   TestSocket.Item[i][j]!=HAS_NULL_IC)
                                {
                                    TestSocket.SetItemData(i, j, HAS_IC);
                                }
                            }
                        }
                        bDoubleContact=true;
                        Task=3000;
                        break;
                    }
                    else if(iTestCount<iMaxDoubleContact)
                    {
                        ProcessShowTestStatus(0);
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(IniConfig.bD22_4_PassBinCanDoubleContact)                                                                                    //JerryYang 20230909 : pass bin也可以設定Double contact
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC     &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       TestSocket.bNeedReTest[i][j])
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);
                                    }
                                }
                                else
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC     &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       TestSocket.bPass[i][j]==false      &&
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
                    ret2=RecordEndTestTime(2);                                                                                                                  //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常。0:arm1 1:arm2 2:雙Arm
                else
                    ret2=1;

                SetNoiseDelay=false;
                TestISTimeOut=false;

                if(CUSTOMER_CODE==CC_TERAPOWER &&                                                                                                               //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
                   Prod.bD22SupportMultiDoubleContact)
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
                        ProcessCount(0);
                    }
                }
                else
                {
                    ProcessCount(0);
                }

                RecordPiggyBackStartEnd(true);                                                                                                                  //jou 2011-12-26 此funtion要在ProcessCount之後

                #ifdef TEST_BIN_MISS_SIMULATE
                if(MakeNoise==true)
                {
                    if(TestSocket.Item[0][0]==(TEST_PASS+1))
                        TestSocket.Item[0][0]=(TEST_PASS+2);
                    else
                        TestSocket.Item[0][0]=(TEST_PASS+1);
                    MakeNoise=false;
                }
                #endif

                if(TestSocket.UseSiteHasIC())                                                                                                                   //KenHsieh 20231208 : 修改為有資料才傳，避免重複移資料導致被覆蓋為NULL
                {
                    if(IsNNMode()==NN_1Row)
                        TestSocket.SplitSuck1Row(BTestSuck, FTestSuck);
                    else
                        TestSocket.SplitSuck(BTestSuck, FTestSuck ,false);                                                                                      //2013-01-15    Dell    Add nn Mode
                }
                RecordHistroy(0);
                bFinshTest=true;
                bInitStartDelayNotFinish=true;                                                                                                                  //Ifor 20181220 : add Init Start Delay Time Not Finish
                bTJControlMode=false;                                                                                                                           //Ifor 20190328 : add TJ Temp Over Range
                ATC_InterfaceForm->SendHandler2DID(0, false);
                if(IniConfig.bTesterTimeUpErrorNeedPassword==true &&
                   CUSTOMER_CODE==CC_LINGSEN &&                                                                                                                 //jou 2012-08-28 菱生要求 Test Time Up Error 之後的兩次測試, 不管結果如何都排到R
                   iTestTimeUpErrContinueR>0)
                {
                    iTestTimeUpErrContinueR--;
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]!=NULL_IC && FTestSuck.Item[i][j]!=HAS_NULL_IC)
                                FTestSuck.SetItemData(i, j, TEST_PASS+15);
                            if(BTestSuck.Item[i][j]!=NULL_IC && BTestSuck.Item[i][j]!=HAS_NULL_IC)
                                BTestSuck.SetItemData(i, j, TEST_PASS+15);
                        }
                    }
                    MyDBIProcess("Message", "Arm 1/Arm 2 Tester Time Up error place to R Bin : "+AnsiString(FTestSuck.CountRealIC())+" pcs" );
                }

                if(ret2!=1)                                                                                                                                     //Steven 20201102 : For Murata資料上拋
                {
                    iXMLRetryCnt++;
                    DoFTestSuckTestICDelay_32.SetSecAndOn(60);
                    Task=6000;
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
                        return true;
                    }
                }
            }
            else if(ret==2)                                                                                                                                     //interface error
            {
                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }
                RecordPiggyBackStartEnd(true);
                bInitialMaxTime=false;                                                                                                                          //jou 2011-11-09 增加initial max time set
                SetHangupMaxTime();                                                                                                                             //Wei 20230224 : 重置死機時間
                SetNoiseDelay=false;
                Task=2200;
            }
            else if((LastSet.iTester==ON_LINE &&                                                                                                                //Steven 20150713 : 整理LastSet.iTester
                     hFTestTimeOutDelay.Off()) ||                                                                                                               //Jou 20101018  //Time Out
                    (bEcho && bTimeOutForNoFullSite==true))                                                                                                     //Steven 20141016 : FullSite的Test Time Out
            {
                bFirstIn=true;                                                                                                                                  //JerryYang 20170610 (wei) 只需進來一次,測試前需將out shuttle lose IC對應的site設成error bin
                if(bATCHasAlarmBinNeedToError)                                                                                                                  //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
                {
                    iDoInterFaceErrorStepTask=1;
                    bContactCTOverCHK=false;                                                                                                                    //Ifor 20160726 避免ATC異常IC放至Error Bin 流程被影響到
                    Task=2500;                                                                                                                                  //不跑下面流程
                    break;
                }

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
                        bD52IndexArmUp=true;                                                                                                                    //JerryYang 20200804 : fix D52 & Index arm在shuttle高度預熱功能同時啟用時，發生tester timeout時會誤發handler hang up
                        break;
                    }
                }

                if((LastSet.iTester==ON_LINE && hFTestTimeOutDelay.Off()) ||                                                                                    //Jou 20101018   //Steven 20150713 : 整理LastSet.iTester
                   (bEcho && bTimeOutForNoFullSite==true))                                                                                                      //Steven 20141016 : FullSite的Test Time Out
                {
                    if(fNote->fShow)                                                                                                                            //JerryYang 20200408 : 修正Alarm畫面佔住時,不會發出Test time out的問題
                        break;
                    bEcho=false;                                                                                                                                //Steven 20150306 : Fixed for FullSite的Test Time Out
                    bTimeOutForNoFullSite=false;
                    ret=ProcessTesterTimeOut(0);

                    if(ret==2)
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
                           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                                                                                 //Steven 20131101 : Add ASE-K12
                            InitTestTask();
                        SetNoiseDelay=false;
                        TestISTimeOut=true;

                        if(TestIF.iTestType==GPIB_MODE ||
                           TestIF.iTestType==TCP_IP_MODE)                                                                                                       //wei 20211027 open short TCP/IP
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
                        else
                        {
                            Task=2200;
                        }

                        bTimeOutForNoFullSite=false;                                                                                                            //ChungHung 20141017 fix Full Site Test Time Out problem
                    }
                    else
                    {
                        if(ret==1)
                            ProcessTestResult(0);
                        SetNoiseDelay=false;
                        TestISTimeOut=false;
                        ProcessCount(0);
                        if(TestSocket.UseSiteHasIC())                                                                                                           //KenHsieh 20231208 : 修改為有資料才傳，避免重複移資料導致被覆蓋為NULL
                        {
                            if(IsNNMode()==NN_1Row)
                                TestSocket.SplitSuck1Row(BTestSuck, FTestSuck);
                            else
                                TestSocket.SplitSuck(BTestSuck, FTestSuck ,false);                                                                              //2013-01-15    Dell    Add nn Mode  //Steven 20140815 : Add For 32 Site
                        }
                        RecordHistroy(0);
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
                            return true;
                        }
                    }
                }
            }
            break;
        case 2500:
            if(SoftStop || SystemStart==false)                                  //Steven 20111004
                break;

            if(DoInterFaceErrorStep_TwoArm32Site())                             //ChungHung 20140516 32Site待補上 兩支Arm要同時起來  //Steven 20140815 : Add For 32 Site
            {
                HangTime.SetSecAndOn(Prod.iHangupMaxTime);                      //Steven 20090827 Start: Hang Up dectector
                bHangTimePause=true;
                return true;
            }
            break;
        case 2600:
            if(SoftStop || SystemStart==false)
                break;

            if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)                //Steven 20111214 : 不需要重測的話,不能關閉GPIB  //steven 2011-12-16 true -> false bTesterTimerOutNotNeedReTest邏輯相反
            {
                fMain->CloseGpibProgram(__FUNC__);
            }

            DoFTestSuckTestICDelay_32.SetSecAndOn(0.3);
            Task=2700;
            break;
        case 2700:
            if(DoFTestSuckTestICDelay_32.Off())
                Task=2200;
            break;
        case 3000:
            #ifdef SOFT_SIMULTE
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test+20000, 30*300))
                    Task=3100;
                break;
            #else
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test+5000, 30*3000))
                    Task=3100;
                break;
            #endif
        case 3100:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test,30*3000))
            {
                InitTestTask();
                Task=2200;
            }
            break;
        case 5000:                                                              //Steven 20100617 Start: Add form 9080A for 即時更新扭力值
            DoFTestSuckTestICDelay_32.SetSecAndOn(0.01);                        // for switch read torque relay
            Task=5100;
            break;
        case 5100:
            if(DoFTestSuckTestICDelay_32.Off())
                Task=5200;
            break;
        case 5200:
            fMain->chkReadTorque1->Checked=true;                                //2008/06/24 lee
            fMain->chkReadTorque2->Checked=false;                               //2008/06/24 lee

            fMain->edTorue0->Text="";
            fMain->lbArm0Torque->Caption="1:Reading";
            iReadCount=0;
            DoFTestSuckTestICDelay_32.SetSecAndOn(0.2);                         //2008/07/15 lee
            Task=5300;
            break;
        case 5300:
            fMain->chkReadTorque1->Checked=true;                                //2008/07/15 lee
            fMain->chkReadTorque2->Checked=false;                               //2008/07/15 lee

            if(fMain->edTorue0->Text!="")
            {
                iReadCount++;
                if(iReadCount>1)                                                //2008/07/15 lee
                {
                    ShowMainScreenPresure(0);
                    if(atoi(fMain->edTorue0->Text.c_str())>=Prod.iMaxPreasure)
                        Task=5400;
                    else
                        Task=1;
                }
                else
                {
                    fMain->edTorue0->Text="";
                    COM2->InitReadTorueTask();
                }
            }
            else if(DoFTestSuckTestICDelay_32.Off())
            {
                ShowMainScreenPresure(0);
                Task=1;
            }
            break;
        case 5400:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "DoTestSuckTestIC_TwoArm32Site 5400"))
                Task=5500;
            break;
        case 5500:
            ShowMyMessage("Index 1 contact force over error", "Index 1 扭力過大錯誤!!", "DoFTestSuckTestIC");
            fAllMotorHome=false;
            iHome=1;
            Task=1;
            break;
        case 6000:                                                              //Steven 20201102 : For Murata資料上拋
            if(iXMLRetryCnt>=3)                                                 //Steven 20201113 : 上傳失敗3次要有Alarm
            {
                ret=ShowErrorMessage("WAR16321", K_RETRY|K_RESET, MTestZ1, false);
                if(ret==K_RESET)
                {
                    fMain->Reset("DoTestSuckTestIC_TwoArm32Site");
                    Task=6100;
                }
                else
                {
                    iXMLRetryCnt=0;
                    DoFTestSuckTestICDelay_32.SetSecAndOn(30);
                    Task=6000;
                }
            }
            else if(DoFTestSuckTestICDelay_32.Off())
            {
                ret2=SendTestResultToHttp();

                if(ret2!=1)
                {
                    iXMLRetryCnt++;
                    DoFTestSuckTestICDelay_32.SetSecAndOn(30);
                    Task=6000;
                }
                else
                {
                    Task=6100;
                }
            }
            break;
        case 6100:
            iXMLRetryCnt=0;
            if(CheckContactOver())
            {
                iDoInterFaceErrorStepTask=1;
                bContactCTOverCHK=true;
                Task=2500;
            }
            else
            {
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer Do32TestDestroyICDelay;
bool Do32SiteTestDestroyIC()                                                    //Steven 20150910 : 32Site 雙Arm一起吸放
{
    static int iRetry=0;
    static bool bFTDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    static bool bBTDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    int &Task=iFrontTestDestroyICTask;
    int iPos=0;
    int sp;
    int iNN=IsNNMode();

    bool flag, flag1, flag2, bIndexSuckCheck1, bIndexSuckCheck2;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;

    AnsiString sTempMessge;
    AnsiString sBuffer="";
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            MOT[MInShuttle1].fCanMoveM=false;
            MOT[MInShuttle2].fCanMoveM=false;
            if(InShtInLF(0)==false)
            {
                ShowMyMessage("Shuttle 1 position error ", "Shuttle 1 位置錯誤");
                MOT[MInShuttle1].fCanMoveM=true;
                Task=1;
            }
            else if(InShtInLF(1)==false)
            {
                ShowMyMessage("Shuttle 2 position error ", "Shuttle 2 位置錯誤");
                MOT[MInShuttle2].fCanMoveM=true;
                Task=1;
            }
            else
            {
                flag2=false;
                flag1=false;
                if(FTestSuck.HasRealIC()==false)
                    flag1=true;
                if(BTestSuck.HasRealIC()==false)
                    flag2=true;

                if(IniConfig.bD54SlowDown &&
                  (flag1==false || flag2==false))                               //Sam 20240904 : 沒有 IC 動作也要做完，避免動作沒做完後面流程的 IndexArmPosition error
                {
                    sp=GetIndexZSpeed(0);                                       //Steven 20160524 : Index Z軸速度整合為Function
                    if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Place, sp, "Do32SiteTestDestroyIC 1", true, Prod.TestZ2_Place))
                    {
                        FTestSuck.ResetAll();
                        BTestSuck.ResetAll();
                        Task=310;
                    }
                }
                else
                {
                    if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Place, MOT[MTestZ1].GailSpeed, "Do32SiteTestDestroyIC 1", true, Prod.TestZ2_Place))
                    {
                        FTestSuck.ResetAll();
                        BTestSuck.ResetAll();
                        Task=310;
                    }
                }
            }
            break;
        case 310:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j])
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                           FTestSuck.Suck[i][j].Destroy())                                                              //放下IC
                        {
                            if(IniConfig.bD44CheckIndexICDestroy)
                            {
                                if(FTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    FTestSuck.Suck[i][j].Off();                                                         //Suck Off and Destroy On
                                    FTestSuck.bNeedCheck[i][j]=true;                                                    //Destroy finish
                                    iIndexTakeDeviceChk1=2;                                                             //kevin 20190103 回吸檢測狀態
                                }
                            }

                            if(FTestSuck.Item[i][j]!=HAS_NULL_IC)                                                       //kevin 20130125
                            {
                                LastSet.iIndexInputOutPut[1]++;                                                         //kevin 20130125
                            }
                            FRCarryKit.MoveSuckData(FTestSuck, i, j);
                            FRCarryKit.PordRec[i][j].AddIndexPlaceShuttleRecord(TestIF.iSiteMap[i+iNN][j], i+iNN, j);   //Sam 20201216 : Add record  //Sam 20200716 : Add record
                            bFTDuplicateErr[i][j]=false;
                        }
                    }

                    if(BTestSuck.Item[i][j])
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Suck[i][j].Destroy())
                        {
                            if(IniConfig.bD44CheckIndexICDestroy)
                            {
                                if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    BTestSuck.Suck[i][j].Off();
                                    BTestSuck.bNeedCheck[i][j]=true;
                                    iIndexTakeDeviceChk2=2;                                                             //kevin 20190103 回吸檢測狀態
                                }
                            }

                            if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                LastSet.iIndexInputOutPut[1]++;                                                         //kevin 20130125
                            }
                            BRCarryKit.MoveSuckData(BTestSuck, i, j);
                            BRCarryKit.PordRec[i][j].AddIndexPlaceShuttleRecord(TestIF.iSiteMap[i][j], i, j);           //Sam 20201216 : Add record //Sam 20200716 : Add record
                            bBTDuplicateErr[i][j]=false;
                        }
                    }
                }
            }

            flag=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j] && FTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }

                    if(BTestSuck.Item[i][j] && BTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=320;
                            return false;
                        }

                        if(BTestSuck.Suck[i][j].Error)
                        {
                            Task=320;
                            return false;
                        }
                    }
                }
            }

            if(FTestSuck.UseSiteHasIC() ||
               BTestSuck.UseSiteHasIC())
            {
                iRetry=0;
                break;
            }

            iRetry=0;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    bFTDuplicateErr[i][j]=false;
                }
            }
            Task=500;
            break;
        case 320:                                                                                                       //破壞異常處理
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iNN][j];                                                               //kevin 20180514 arm 1 +2 取到32 site alarm 位置才會對
                        FTestSuck.Suck[i][j].Error=false;
                        bFTDuplicateErr[i][j]=true;
                    }
                    else
                    {
                        bFTDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                               //Vacuum sensor OFF error

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bBTDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        BTestSuck.Suck[i][j].Error=false;                                                               //Steven 20101229 : 換位置
                        bBTDuplicateErr[i][j]=true;                                                                     //Steven 20101229 : 換位置
                    }
                    else
                    {
                        bBTDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                               //Vacuum sensor OFF error

            bHasErr=false;
            Task=310;
            break;
        case 500:
            if(IniConfig.bD44CheckIndexICDestroy)
            {
                if(MOT[MTestZ1].Gali_ReadPos()<(Prod.TestZ1_Place+10))          //kevin 20130719  吹太高會將ic吹出
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.bNeedCheck[i][j])                      //Suck Off Destroy On
                            {
                                FTestSuck.Suck[i][j].Off();                     //kevin 20130615 放完shuttle 關真空 開破壞
                            }
                        }
                    }
                }
                else                                                            //kevin 20130615 放完shuttle 上升關破壞
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.bNeedCheck[i][j])                      // Destroy Off
                            {
                                FTestSuck.Suck[i][j].OffDestroy();              //kevin 20130719 關吹氣
                            }
                        }
                    }
                }

                if(MOT[MTestZ2].Gali_ReadPos()<(Prod.TestZ2_Place+10))          //kevin 20130719 關吹氣
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.bNeedCheck[i][j])                      //  kevin 20130615 放完shuttle 關真空 開破壞
                            {
                                BTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }
                else                                                            //  kevin 20130615 放完shuttle 上升關破壞
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.bNeedCheck[i][j])
                            {
                                BTestSuck.Suck[i][j].OffDestroy();              //kevin 20130719 關吹氣
                            }
                        }
                    }
                }
            }

            iPos=Prod.TestZ1_Safe;
            sp=GetIndexZSpeed(0);                                               //Steven 20160524 : Index Z軸速度整合為Function
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(iPos, sp, "Do32SiteTestDestroyIC 500"))
            {
                flag=false;
                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
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
                    Do32TestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                    Task=600;
                    break;
                }

                FRCarryKit.SetHasNullIcToNullIc();                              //jou 2011-06-29 out shuttle has_null_ic hang up fix    //Steven 20160524 : ADD SetHasNullIcToNullIc
                BRCarryKit.SetHasNullIcToNullIc();                              //jou 2011-06-29 out shuttle has_null_ic hang up fix    //Steven 20160524 : ADD SetHasNullIcToNullIc
                MOT[MInShuttle1].fCanMoveM=true;
                MOT[MInShuttle2].fCanMoveM=true;
                iIndexTakeDeviceChk1=0;                                         //kevin 20190103 回吸檢測狀態
                iIndexTakeDeviceChk2=0;                                         //kevin 20190103 回吸檢測狀態
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        bFTDuplicateErr[i][j]=false;
                        bBTDuplicateErr[i][j]=false;
                    }
                }
                return true;
            }
            break;
        case 600:
            if(INDEX_SUCKER_TYPE==1)                                                                                    //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bIndexSuckCheck1=fiosetview->ProcessIndexSuckDestroy1(1);                                               //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
                bIndexSuckCheck2=fiosetview->ProcessIndexSuckDestroy2(1);                                               //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
            }
            else
            {
                bIndexSuckCheck1=true;
                bIndexSuckCheck2=true;
            }

            if(Do32TestDestroyICDelay.Off() &&
               bIndexSuckCheck1==true &&
               bIndexSuckCheck2==true)
            {
                flag=false;
                ErrPart=" ";
                bHasErr=false;
                bHasDuplicateErr=false;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(bFTDuplicateErr[i][j])
                            bHasDuplicateErr=true;
                    }
                }

                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.bNeedCheck[i][j])                                                                  //確認真空狀態
                        {
                            if(FTestSuck.Suck[i][j].GetStatus())
                            {
                                bHasErr=true;
                                ErrPart+=IndexSuckName[i+iNN][j];                                                       //kevin 20180514 arm 1 +2 取到32 site alarm 位置才會對
                                flag=true;
                                bFTDuplicateErr[i][j]=true;                                                             //Steven 20110127
                                iIndexTakeDeviceChk1=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                FTestSuck.bNeedCheck[i][j]=false;                                                       //確認完成
                                FTestSuck.Suck[i][j].Normal();
                                bFTDuplicateErr[i][j]=false;                                                            //Steven 20110127
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                if(bHasErr)
                {
                    if(IndexAlarmInArmAway()==true)                                                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else
                    {
                        return false;
                    }
                }
                bHasErr=false;

                flag=false;
                ErrPart=" ";
                bHasErr=false;

                bHasDuplicateErr=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(bBTDuplicateErr[i][j])
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
                                bBTDuplicateErr[i][j]=true;                                                             //Steven 20110127
                                iIndexTakeDeviceChk2=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                BTestSuck.bNeedCheck[i][j]=false;
                                BTestSuck.Suck[i][j].Normal();
                                bBTDuplicateErr[i][j]=false;                                                            //Steven 20110127
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                if(bHasErr)
                {
                    if(IndexAlarmInArmAway()==true)                                                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else
                    {
                        return false;
                    }
                }

                if(flag==false)
                {
                    Task=500;
                }
                else
                {
                    if(IniConfig.bD44CheckIndexICDestroy)                                                               //jou 2011-11-01負壓不能一直開著真空，必須關掉
                    {
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
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
                    Do32TestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
extern int CheckOneCycleAction(int iTask);                                      //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
TQPF_Timer DoTestHeadMotorDelay2_32site;
TQPF_Timer hDo32TestSuckIC;
bool Do32SiteTestSuckIC()                                                       //Steven 20150910 : 32Site 雙Arm一起吸放
{
    static bool bFTDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    static bool bBTDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    static bool bFTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]  ={{false, false, false, false, false, false, false, false},                                      //Steven 20110301 : 確認吸取完成
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    static bool bBTSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]  ={{false, false, false, false, false, false, false, false},                                      //Steven 20110301 : 確認吸取完成
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false},
                                                                 {false, false, false, false, false, false, false, false}};

    int &Task=iFrontTestSuckICTask, ret;
    int sp;
    int iNN=IsNNMode();
    int ZPos1, ZPos2;
    bool flag1=false, flag2=false;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    bool bResult;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:                                                                 //判斷Index與Shuttle上IC的狀態
            if(iOneCycle)
            {
                bResult=false;
                ret=CheckOneCycleAction(0);                                     //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
                if(ret==0)
                {
                    if(FTestSuck.UseSiteNoIC()  &&
                       FRCarryKit.UseSiteNoIC() &&
                       FLCarryKit.UseSiteNoIC() &&
                       !InArmSuck.HasRealIC())
                    {
                        MOT[MInShuttle1].fCanMoveM=true;
                        bResult=true;
                    }

                    if(BTestSuck.UseSiteNoIC()  &&
                       BRCarryKit.UseSiteNoIC() &&
                       BLCarryKit.UseSiteNoIC() &&
                       !InArmSuck.HasRealIC())
                    {
                        MOT[MInShuttle2].fCanMoveM=true;
                        bResult=true;
                    }
                }

                if(bResult)
                {
                    return bResult;
                }
            }

            if(FLCarryKit.UseSiteNoIC() ||
               BLCarryKit.UseSiteNoIC())
            {
                if(iCleanOut &&
                   InArmSuck.NoIC() &&
                   MOT[MMPlate1].HasIC()==false &&
                   MOT[MMPlate2].HasIC()==false)
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    MOT[MInShuttle2].fCanMoveM=true;
                    return true;
                }
            }

            if(iCleanOut &&
               FLCarryKit.UseSiteNoIC() &&
               InArmSuck.NoIC()  &&
               MOT[MMPlate1].HasIC()==false &&
               MOT[MMPlate2].HasIC()==false)
            {
                return true;
            }

            if(iCleanOut &&
               BLCarryKit.UseSiteNoIC() &&
               InArmSuck.NoIC()  &&
               MOT[MMPlate1].HasIC()==false &&
               MOT[MMPlate2].HasIC()==false)
            {
                return true;
            }

            if(FLCarryKit.UseSiteHasIC() &&
               BLCarryKit.UseSiteHasIC())
            {
                if(InSHT1InRT()==false || InSHT2InRT()==false)
                {
                    return false;
                }

                if(bReadAndCheckCPUName==true &&
                   CosFunction.bInShuttleDetectByLatch &&                       //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                   Prod.bF18InshuttleDetect &&                                  //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                   MOTION_CARD_TYPE==MotionCard_Contec &&
                   LastSet.iRealDummy==REALLY)
                {
                    if((bInSht1LtcDetectTesterCanMove==false) || (bInSht2LtcDetectTesterCanMove==false))
                    {
                        return false;
                    }
                }

                if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
                   bInSh1DoLtc==true && bInSh2DoLtc==true)                      //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
                    return false;

                MOT[MInShuttle1].fCanMoveM=false;
                MOT[MInShuttle2].fCanMoveM=false;
                Task=300;
            }
            break;
        case 299:                                                               //Steven 20140827 : 第一次吸取ic等待時間
            if(DoTestHeadMotorDelay2_32site.Off())
            {
                Task=301;
            }
            break;
        case 300:
            MOT[MInShuttle1].fCanMoveM=false;
            MOT[MInShuttle2].fCanMoveM=false;

            if(DeviceForm.bSuckShuttleDeviceAfterTested &&                      //Steven 20140827 : 第一次吸取ic等待時間
               (LastSet.iTemperature==Tempture_Hot ||                           //2013-11-27   Dell    需要做Index soak time
                LastSet.iTemperature==Tempture_AmbientHot) &&
               bInitialSackTime &&                                              //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
               Temperature.iInitialStart1Time>0)                                //kevin 20131112 第一次吸取ic等待時間)
            {
                DoTestHeadMotorDelay2_32site.SetSecAndOn(Temperature.iInitialStart1Time);
                iInitialSoakTimer = Temperature.iInitialStart1Time;
                fMain->tInitSoakTimer.LatchCycleTimeSec(true);
                bInitialSackTime=false;
                Task=299;
                break;
            }

            flag2=false;
            Task=301;
        case 301:                                                               //確認Z軸位置 位置在下->吸取IC->Z軸到位後->Task=310
            if(IniConfig.bIndexPickupWait==true &&                              //jou 2012-06-29 Index Pick up need wait Soak Time
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&                   //kevin 20180903 (Steven) : add 恆溫控制
               Temperature.iInitialStart2Time!=0)
            {
                if(FLCarryKit.TSoakTime.Off()==false)
                    break;
            }

            if(IniConfig.bA05UseAutoDocking==true &&                            //jou 2014-04-25 Tester Docking Wait delay time
               Temperature.bUseTesterDocking==true &&                           //ChungHung 20141111 modify for SCK request
               LastSet.iTemperature==Tempture_Hot &&
               iTesterDucking>0)
            {
                bHangTimePause=true;                                            //ChungHung 20140714 add fix Tester Ducking function if wait long time will Auto Home then Show Hangup Message
                break;
            }

            if(bNeedTesterDuckingFinishLightYellowAndAlarmOn)                   //ChungHung 20141015 add for SCK When the handler start running (finished Start count function),Yellow light blinking & alarm on (press "alarm reset" then clear)
            {
                bNeedTesterDuckingFinishLightYellowAndAlarmOn=false;
                bTesterDuckingFinishLightYellowAndAlarmOn=true;
            }

            MOT[MInShuttle1].ScanMotorStatus();
            MOT[MInShuttle2].ScanMotorStatus();

            if(BLCarryKit.UseSiteHasIC() &&
               FLCarryKit.UseSiteHasIC())
            {
                flag2=false;
                if(BLCarryKit.HasRealIC()==false &&
                   FLCarryKit.HasRealIC()==false)
                {
                    flag2=true;
                }
                else
                {
                    ZPos1=MOT[MTestZ1].Gali_ReadPos();
                    ZPos2=MOT[MTestZ2].Gali_ReadPos();

                    if(ZPos1<(Prod.TestZ1_Pick+1000))                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                    {
                        bZ1PickShuttle=true;
                    }

                    if(ZPos2<(Prod.TestZ2_Pick+1000))                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                    {
                        bZ2PickShuttle=true;
                    }

                    if(flag2==false)
                    {
                        sp=GetIndexZSpeed(0);                                   //Steven 20160524 : Index Z軸速度整合為Function
                        flag2=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Pick, sp, "Do32SiteTestDestroyIC 301", true, Prod.TestZ2_Pick);
                    }
                }
            }
            else if((FLCarryKit.HasIC()==false && FTestSuck.HasIC()) && (BLCarryKit.HasRealIC()==true))
            {
                Task=3010;
                break;
            }
            else if((BLCarryKit.HasIC()==false && BTestSuck.HasIC()) && (FLCarryKit.HasRealIC()==true))
            {
                Task=3020;
                break;
            }
            else
            {
                Task=1;
                break;
            }

            if(LastSet.iRealDummy==REALLY &&
               ArmSpeed[IndexArm].bSuckOnDown &&
               INDEX_SUCKER_TYPE==0)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j]!=NULL_IC &&
                           FLCarryKit.Item[i][j]!=HAS_NULL_IC)
                            FTestSuck.Suck[i][j].On();

                        if(BLCarryKit.Item[i][j]!=NULL_IC &&
                           BLCarryKit.Item[i][j]!=HAS_NULL_IC)
                            BTestSuck.Suck[i][j].On();
                    }
                }
            }

            if(flag2)
            {
                bZ1PickShuttle=true;                                            //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        bFTSuckFinish[i][j]=false;                              //Steven 20110301 : 初始化，都當作還沒做完
                        bBTSuckFinish[i][j]=false;                              //Steven 20110301 : 初始化，都當作還沒做完
                        FTestSuck.Suck[i][j].Reset();                           //Steven 20140213 : Jordan說Index下去不吸直接Alarm
                        BTestSuck.Suck[i][j].Reset();                           //Steven 20140213 : Jordan說Index下去不吸直接Alarm
                    }
                }
                flag2=false;
                Task=310;
            }
            break;
        case 310:
            flag1=true;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FLCarryKit.Item[i][j])
                    {
                        if(FLCarryKit.Item[i][j]==HAS_NULL_IC ||
                           bFTSuckFinish[i][j]==true)                                                                   //Steven 20110301
                        {
                            if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                               FTestSuck.Item[i][j]==NULL_IC)                                                           //Steven 20111202 : Retry會掉料
                                FTestSuck.Suck[i][j].Normal();                                                          //Steven 20111201 : 預防負壓壓降
                            bFTSuckFinish[i][j]=true;                                                                   //Steven 20110301
                            bFTDuplicateErr[i][j]=false;                                                                //Steven 20100105
                        }
                        else
                        {
                            if(FTestSuck.Suck[i][j].Suck())
                            {
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                                FTestSuck.PordRec[i][j].AddIndexPickVacuum(FTestSuck.Suck[i][j].GetIOValue());          //Sam 20230210 : 新增 VacuumUnit 通訊模組
                                bFTDuplicateErr[i][j]=false;                                                            //Steven 20100105
                                bFTSuckFinish[i][j]=true;                                                               //Steven 20110301 : 吸取完成的不做
                                if(bRunAutoClean==false)                                                                //kevin 20130125
                                    LastSet.iIndexInputOutPut[0]++;                                                     //kevin 20130125 Index 吸取IC計數
                            }
                            else if(FTestSuck.Suck[i][j].Error)                                                         //Steven 20110301 : 有錯誤的不做
                            {
                                bFTSuckFinish[i][j]=true;
                            }
                            else
                            {
                                flag1=false;                                                                            //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                           FTestSuck.Item[i][j]==NULL_IC)                                                               //Steven 20111202 : Retry會掉料
                            FTestSuck.Suck[i][j].Normal();                                                              //Steven 20111201 : 預防負壓壓降
                        bFTSuckFinish[i][j]=true;                                                                       //Steven 20110301 : 沒有東西的地方要跳過
                    }

                    if(BLCarryKit.Item[i][j])
                    {
                        #ifdef SOFT_SIMULTE
                        if(fMain->cbIndexDrop->Checked==true)
                        {
                            bBTSuckFinish[0][1]=true;
                            BTestSuck.Suck[0][1].Error=true;
                        }
                        #endif

                        if(BLCarryKit.Item[i][j]==HAS_NULL_IC ||
                           bBTSuckFinish[i][j]==true)                                                                   //Steven 20110301
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                               BTestSuck.Item[i][j]==NULL_IC)                                                           //Steven 20111202 : Retry會掉料
                                BTestSuck.Suck[i][j].Normal();                                                          //Steven 20111201 : 預防負壓壓降
                            bBTSuckFinish[i][j]=true;                                                                   //Steven 20110301
                        }
                        else
                        {
                            if(BTestSuck.Suck[i][j].Error)                                                              //Steven 20110301 : 有錯誤的不做
                            {
                                bBTSuckFinish[i][j]=true;
                            }
                            else if(BTestSuck.Suck[i][j].Suck())
                            {
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                                BTestSuck.PordRec[i][j].AddIndexPickVacuum(BTestSuck.Suck[i][j].GetIOValue());          //Sam 20230210 : 新增 VacuumUnit 通訊模組
                                bBTDuplicateErr[i][j]=false;                                                            //Steven 20100105
                                bBTSuckFinish[i][j]=true;                                                               //Steven 20110301 : 吸取完成的不做
                                if(bRunAutoClean==false)                                                                //kevin 20120531
                                    LastSet.iIndexInputOutPut[0]++;                                                     //kevin 20120511 Index 吸取IC計數
                            }
                            else
                            {
                                flag1=false;                                                                            //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Item[i][j]==NULL_IC)                                                               //Steven 20111202 : Retry會掉料
                            BTestSuck.Suck[i][j].Normal();                                                              //Steven 20111201 : 預防負壓壓降
                        bBTSuckFinish[i][j]=true;                                                                       //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTSuckFinish[i][j]==false)                                                                      //只要有未完成的就繼續等
                        flag1=false;

                    if(bBTSuckFinish[i][j]==false)                                                                      //只要有未完成的就繼續等
                        flag1=false;
                }
            }

            if(flag1==true)                                                                                             //Steven 20110301 : 所有吸嘴都做完
            {
                bHasErr=false;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            bShuttle1HasPickErr=true;                                                                   //Steven 20230118 : 避免In arm 偷放料, 往上移動
                            if(IniConfig.bD62PickUpErrorNeedPurge)                                                      //Steven 20161024 : 吸取異常需要吹氣一次
                            {
                                FTestSuck.Suck[i][j].Off();
                            }
                        }

                        if(BTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            bShuttle2HasPickErr=true;                                                                   //Steven 20230118 : 避免In arm 偷放料, 往上移動
                            if(IniConfig.bD62PickUpErrorNeedPurge)                                                      //Steven 20161024 : 吸取異常需要吹氣一次
                            {
                                BTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bHasErr)
                {
                    Task=305;
                    return false;
                }

                if(FLCarryKit.HasRealIC() || BLCarryKit.HasRealIC())
                    break;

                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j])
                        {
                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC && FTestSuck.Item[i][j]!=NULL_IC)
                                FLCarryKit.SetItemData(i, j, NULL_IC);
                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                            }
                        }
                        bFTDuplicateErr[i][j]=false;                                                                    //Steven 20100105

                        if(BLCarryKit.Item[i][j])
                        {
                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC && BTestSuck.Item[i][j]!=NULL_IC)
                                BLCarryKit.SetItemData(i, j, NULL_IC);
                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                            }
                        }
                        bBTDuplicateErr[i][j]=false;                                                                    //Steven 20100105
                    }
                }

                if(FLCarryKit.UseSiteHasIC() ||
                   BLCarryKit.UseSiteHasIC())
                    break;

                bShuttle1HasPickErr=false;                                                                              //Steven 20220712 : 避免In arm 偷放料
                bShuttle2HasPickErr=false;                                                                              //Steven 20220712 : 避免In arm 偷放料
                bResetIndexArm1Pick=false;
                Task=500;
            }
            break;
        case 305:
            CheckIndexAllSuckICFallDown(true, true);                            //Steven 20110725 : 修改負壓檢查方式

            if(MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Pick+1000))             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ1PickShuttle=false;
            }

            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }

            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, "Do32SiteTestDestroyIC 305"))
            {
                bZ1PickShuttle=false;                                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                bZ2PickShuttle=false;                                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                Task=320;
            }
            break;
        case 3010:
            return DoRearTestSuckIC();
        case 3020:
            return DoFrontTestSuckIC();
        case 320:
            bShuttle1MoveToLeft=false;
            bShuttle2MoveToLeft=false;
            MOT[MInShuttle1].fCanMoveM=false;
            MOT[MInShuttle2].fCanMoveM=false;
            Task=321;
        case 321:                                                                                                       //ChungHung 20130924 add
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            flag1=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bFTDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iNN][j];                                                               //kevin 20180514 arm 1 +2 取到32 site alarm 位置才會對
                        if(IniConfig.bD62PickUpErrorNeedPurge)                                                          //Steveb 20161024 : 吸取異常需要吹氣一次
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
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

                if(IniConfig.bNewResetFunction==true && bResetIndexArm1Pick==true)
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
                        ret=ShowErrorMessage("JAM0301", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart);                    //Devicr Pick-Up Error
                    else
                        ret=ShowErrorMessage("JAM0301", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);            //Devicr Pick-Up Error
                }

                if(ret==K_SKIP)
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap)
                        iAutoSiteMapCount++;

                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]==NULL_IC)
                            {
                                FLCarryKit.PordRec[i][j].AddErrorRecord("JAM0301");                                     //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                if(CosFunction.bUseSCKART)                                                              //Steven 20161214 (wei) : For SCK ART
                                {
                                    if(TestIF_File.bRENESAS_EnableFTCT==true)                                           //RogeryYang 20251014 : FTCT add to HdRejectBT
                                        fSCKART->iInputJamCnt++;
                                    else
                                        fSCKART->AddOutputJamCnt(i, j, ret);                                            //RogerYang 20250923 : 整合ART OutArm JamCount
                                }
                                FTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                FTestSuck.Suck[i][j].Normal();
                            }
                            FLCarryKit.SetItemData(i, j, NULL_IC);
                            bFTDuplicateErr[i][j]=false;
                        }
                    }

                    if(IniConfig.bNewResetFunction==true && bResetIndexArm1Pick==true)
                    {
                    }
                    else
                    {
                        if(IniConfig.bD42IndexPickICShuttlePause)
                        {
                            bInArmNeedToSafePos=true;
                            bShuttle1Pause=true;
                            bIndexArm1PickupErrStop=true;                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                            bShowShuttle1Device=true;                                                                   //kevin 20180504 index pick up error
                        }
                    }
                    bIndexPickUpErrorWaitRetry=false;                                                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }
                else
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Suck[i][j].Error)
                                bFTDuplicateErr[i][j]=true;
                        }
                    }
                    flag1=true;                                                                                         //Steven 20220712 : 避免Arm 1 Jam, Arm 2 沒Jam, flag被清空
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }

                FTestSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空

                if(IniConfig.bNewResetFunction==true && bResetIndexArm1Pick==true)
                {
                    bResetIndexArm1Pick=false;
                }
                else
                {
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //ChungHung 20120717 add Index Drop Error Can Retry and Start
                    {
                        MOT[MInShuttle1].fCanMoveM=true;                                                                //等待Shuttle 移至右邊
                        bShuttle1MoveToRight=true;
                        bShuttle1HasPickErr=true;                                                                       //Steven 20220712 : 避免In arm 偷放料
                    }
                }
            }

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bBTDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];

                        if(IniConfig.bD62PickUpErrorNeedPurge)                                                          //Steveb 20161024 : 吸取異常需要吹氣一次
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

            if(bHasErr)
            {
                if(IndexAlarmInArmAway()==false)                                                                        //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }

                bHasErr=false;

                if(IniConfig.bNewResetFunction==true &&
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

                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]==NULL_IC)
                            {
                                BLCarryKit.PordRec[i][j].AddErrorRecord("JAM0301");                                     //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                if(CosFunction.bUseSCKART)                                                              //Steven 20161214 (wei) : For SCK ART
                                {
                                    if(TestIF_File.bRENESAS_EnableFTCT==true)                                           //RogeryYang 20251014 : FTCT add to HdRejectBT
                                        fSCKART->iInputJamCnt++;
                                    else
                                        fSCKART->AddOutputJamCnt(i, j, ret);                                            //RogerYang 20250923 : 整合ART OutArm JamCount
                                }
                                BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                BTestSuck.Suck[i][j].Normal();
                            }
                            BLCarryKit.SetItemData(i, j, NULL_IC);
                            bBTDuplicateErr[i][j]=false;
                        }
                    }

                    if(IniConfig.bNewResetFunction==true && bResetIndexArm2Pick==true)
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

                    if(flag1==false)                                                                                    //Steven 20220712 : 避免Arm 1 Jam, Arm 2 沒Jam, flag被清空
                        bIndexPickUpErrorWaitRetry=false;                                                               //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }
                else
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                            if(BTestSuck.Suck[i][j].Error)
                                bBTDuplicateErr[i][j]=true;
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }

                BTestSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空

                if(IniConfig.bNewResetFunction==true &&
                   bResetIndexArm2Pick==true)
                {
                    bResetIndexArm2Pick=false;
                }
                else
                {
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //ChungHung 20120717 add Index Drop Error Can Retry and Start
                    {
                        MOT[MInShuttle2].fCanMoveM=true;                                                                //等待Shuttle 移至右邊
                        bShuttle2MoveToRight=true;
                        bShuttle2HasPickErr=true;                                                                       //Steven 20220712 : 避免In arm 偷放料
                    }
                }
            }

            if(FLCarryKit.HasRealIC() || BLCarryKit.HasRealIC())
            {
                Task=300;
            }
            else
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j])
                        {
                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC &&
                               FTestSuck.Item[i][j]!=NULL_IC)
                                FLCarryKit.SetItemData(i, j, NULL_IC);

                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                                bFTDuplicateErr[i][j]=false;
                            }
                        }

                        if(BLCarryKit.Item[i][j])
                        {
                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                                BLCarryKit.SetItemData(i, j, NULL_IC);

                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                                bBTDuplicateErr[i][j]=false;
                            }
                        }
                    }
                }

                if(bShuttle1Pause)
                    MOT[MInShuttle1].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed

                if(bShuttle2Pause)
                    MOT[MInShuttle2].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed

                if(FLCarryKit.UseSiteHasIC() ||
                   BLCarryKit.UseSiteHasIC())
                {
                    Task=300;
                    break;
                }
                Task=500;
            }
            break;
        case 500:
            MOT[MInShuttle2].fCanMoveM=false;
            MOT[MInShuttle1].fCanMoveM=false;
            bShuttle1MoveToRight=false;
            bShuttle1HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            bShuttle2MoveToRight=false;
            bShuttle2HasPickErr=false;                                          //Steven 20230314 : 避免In arm 偷放料
            bIndexPickUpErrorWaitRetry=false;                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
            Task=501;
        case 501:                                                                                                       //ChungHung 20130924 add
            if(MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Pick+1000))                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ1PickShuttle=false;
            }

            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }

            sp=GetIndexZSpeed(0);                                                                                       //Steven 20160524 : Index Z軸速度整合為Function
            flag1=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, sp, "Do32SiteTestDestroyIC 501");
            if(flag1)
            {
                bZ1PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                bZ2PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing

                #ifndef SOFT_SIMULTE
                {
                    if(LastSet.iRealDummy==REALLY)
                    {
                        ErrPart=" ";
                        bHasErr=false;
                        bHasDuplicateErr=false;
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(bFTDuplicateErr[i][j])
                                    bHasDuplicateErr=true;

                                if(FTestSuck.Item[i][j] &&
                                   FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    bHasErr=true;
                                    FTestSuck.Suck[i][j].Normal();                                                      //Steven 20110707 : 預防負壓掉料
                                    FTestSuck.Suck[i][j].Error=true;
                                    ErrPart+=IndexSuckName[i+iNN][j];                                                   //kevin 20180514 arm 1 +2 取到32 site alarm 位置才會對
                                }
                                else
                                {
                                    FTestSuck.Suck[i][j].Error=false;
                                }
                            }
                        }

                        if(bHasErr)
                        {
                            if(IndexAlarmInArmAway()==false)                                                            //Steven 20130613 : Index異常時, In Arm要先讓位功能
                            {
                                return false;
                            }

                            if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                 //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                                bIsTestSitICFallDown=true;                                                              //kevin 20130706

                            bHasErr=false;
                            if(IniConfig.bIndexDropOnlySKIP==true ||
                               IniConfig.bKoreaFunction)                                                                //jou 2012-02-13 index drop error only skip
                            {
                                ret=ShowErrorMessage("JAM0303", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart);            //Device Drop Error
                            }
                            else
                            {
                                ret=ShowErrorMessage("JAM0303", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);    //Device Drop Error
                            }

                            if(ret==K_SKIP)
                            {
                                for(int i=0; i<FTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<FTestSuck.iShtCol; j++)
                                    {
                                        if(FTestSuck.Suck[i][j].Error)
                                        {
                                            FTestSuck.PordRec[i][j].AddErrorRecord("JAM0303");                          //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                            if(CosFunction.bUseSCKART)                                                  //Steven 20161214 (wei) : For SCK ART
                                            {
                                                fSCKART->AddOutputJamCnt(i, j, ret,
                                                    Prod.bIsPassBin[FTestSuck.iBinData[i][j]]);                         //RogerYang 20250923 : 整合ART OutArm JamCount
                                            }
                                            FTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                            FTestSuck.Suck[i][j].Error=false;
                                        }
                                        bFTDuplicateErr[i][j]=false;
                                    }
                                }

                                if(IniConfig.bD42IndexPickICShuttlePause)
                                {
                                    bInArmNeedToSafePos=true;
                                    bShuttle1Pause=true;
                                    bIndexArm1PickupErrStop=true;                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                                    bShowShuttle1Device=true;                                                           //kevin 20180504 index pick up error
                                }
                            }
                            else
                            {
                                for(int i=0; i<FTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<FTestSuck.iShtCol; j++)
                                    {
                                        if(FTestSuck.Suck[i][j].Error)
                                        {
                                            bFTDuplicateErr[i][j]=true;
                                            FLCarryKit.MoveSuckData(FTestSuck, i, j);
                                            FTestSuck.Suck[i][j].Error=false;
                                        }
                                    }
                                }
                                Task=300;
                                return false;
                            }
                        }

                        ErrPart=" ";
                        bHasErr=false;
                        bHasDuplicateErr=false;
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(bBTDuplicateErr[i][j])
                                    bHasDuplicateErr=true;

                                if(BTestSuck.Item[i][j]&&
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
                                        bBTDuplicateErr[i][j]=false;
                                    }
                                }

                                if(IniConfig.bD42IndexPickICShuttlePause)
                                {
                                    bInArmNeedToSafePos=true;
                                    bShuttle2Pause=true;
                                    bIndexArm2PickupErrStop=true;                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                                    bShowShuttle2Device=true;                                                           //kevin 20180504 index pick up error
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
                                            bBTDuplicateErr[i][j]=true;
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

                if(bShuttle1Pause)
                    MOT[MInShuttle1].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed

                if(bShuttle2Pause)
                    MOT[MInShuttle2].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed

                MOT[MInShuttle1].fCanMoveM=true;
                MOT[MInShuttle2].fCanMoveM=true;

                for(int i=0; i<FTestSuck.iMaxRow; i++)
                {
                    for(int j=0; j<FTestSuck.iMaxCol; j++)
                    {
                        if(i>=FTestSuck.iShtRow || j>=FTestSuck.iShtCol)
                        {
                            FTestSuck.SetItemData(i, j, NULL_IC);
                            BTestSuck.SetItemData(i, j, NULL_IC);
                        }
                    }
                }

                fAutoTeach->SetIndexBinError(2);                                                                        //JimmyChiu 20211020 : Auto alignment mode
                if(DeviceForm.bSuckShuttleDeviceAfterTested==false &&                                                   //Steven 20140827 : 第一次吸取ic等待時間
                   (LastSet.iTemperature==Tempture_Hot ||
                    LastSet.iTemperature==Tempture_AmbientHot) &&                                                       //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
                   bInitialSackTime &&                                                                                  //2013-11-27   Dell    需要做Index soak time
                   (Temperature.iInitialStart1Time>0 ||
                    Temperature.iIndexSoakTime>0))                                                                      //kevin 20131112 第一次吸取ic等待時間)
                {
                    int iSetTmr=Temperature.iInitialStart1Time;
                    if(Temperature.iIndexSoakTime>Temperature.iInitialStart1Time)
                        iSetTmr=Temperature.iIndexSoakTime;

                    DoTestHeadMotorDelay2_32site.SetSecAndOn(iSetTmr);
                    Task=502;
                    bInitialSackTime=false;
                }
                else                                                                                                    //Steven 20140827 : 第一次吸取ic等待時間
                {
                    return true;
                }
            }
            break;
        case 502:                                                               //kevin 20131112  等待initial sock time
            if(DoTestHeadMotorDelay2_32site.Off())
            {
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------