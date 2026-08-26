#include "MachineDefine.h"
#pragma hdrstop

#include "atester.h"
#include "atester_32Site.h"
#include "atester_ProcessCount.h"

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
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
//#include "SECSGEM.h"
#include "Socket_ASE_KR.h"
//#include "ScanBtnThread.h"
#include "InterfaceSYS.h"
#include "BarCode.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "cBinSel.h"
#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "cpublic.h"
#include "uYieldMonitoring.h"
#include "SCK_ART.h"
#include "cShowBinSelect.h"
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "uCleaning.h"
#include "TesterTCP.h"
#include "ProductionInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TQPF_Timer PiggyBackTimer;
void RecordPiggyBackStartEnd(bool bStart)                                       //jou 2011-11-14 start : 紀錄piggyback時間
{
    bPiggyBackIndexCheck=false;

    if(IniConfig.bRecordPiggyBackStartEnd==false)
        return;

    AnsiString AStr;

    if(bStart==true)
    {
        if(bRecordPiggyBackStart==true)
        {
            PiggyBackTimer.LatchCycleTimeSec(true);
        }
    }
    else
    {
        if(bRecordPiggyBackEnd==true && iOneCycle==false)
        {
            bRecordPiggyBackEnd=false;
            bRecordPiggyBackStart=false;
            iPiggyBackTime[1]=MyTickCount();                                    //jou test delay time

            AStr.sprintf("PiggyBack Funtion record time : %d Sec", PiggyBackTimer.LatchCycleTimeSec());
            MyDBIProcess("Message", AStr);

            AStr.sprintf("Piggy Back Pass Finish");                             //JerryYang 20220614 松諭要求新增
            MyDBIProcess("Message", AStr);
        }
    }
}
//---------------------------------------------------------------------------
void ProcessPiggyBackFunction()                                                 //Steven 20110725 : 將所有的Piggy Back整合到這裡
{
    if(CosFunction.bIndexCheckCanTurnOff &&                                     //Isaac 20211019 : 可選擇做index check的時機，符合條件則不檢查
       ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
        (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
         IniConfig.iD71IndexCheckOnOffMode==2))
    {                                                                           //不檢查
    }
    else
    {
        if(bCheckIndex==false)
        {
            bRecordPiggyBackStart=true;
        }

        bCheckIndex=true;

        if(fMain->BtnOneCycle->Down==false)
        {
            InitOneCycle("Process Piggy Back Function");
            fMain->BtnOneCycle->Down=true;
            LastSet.SendCT[3]=0;
            LastSet.SendCT[2]=0;
            LastSet.iIndexCount=0;                                              //wei 20141201 Low Yield Auto Clean
            if((USE_AUTO_RETEST==eartInstall &&
                (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||              //kevin 20150615
                CosFunction.bUseARTSortCount)                                   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
            {
               LastSet.SendCT_ART[3]=0;
               LastSet.SendCT_ART[2]=0;
            }

            for(int i=0; i<30; i++)
            {
                ContinuousPassBinCount[i]=0;
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    ContinuousPassBinBySocket[i][j]=0;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void InitialPiggyBackFunction()                                                 //Steven 20110725 : 重置Piggy Back的狀態
{
    AnsiString str;
    if(bCheckIndex==true)
    {
        switch(iWhoTriggerPiggyBack)                                            //Steven 20111207 : 誰觸發了Piggy Back
        {
            case pbtNone:
                str.sprintf("PiggyBack Function Start");
                break;
            case pbtContinualPassBin:
                str.sprintf("PiggyBack Function Start - Continual Pass Bin");
                break;
            case pbtContinualPassBinBySocket:
                str.sprintf("PiggyBack Function Start - Continual Pass Bin By Socket");
                break;
            case pbtContinualLoader:
                str.sprintf("PiggyBack Function Start - Continual Loader");
                break;
            case pbtContinualContactCount:
                str.sprintf("PiggyBack Function Start - Continual Contact Count");
                break;
            case pbtHome:
                str.sprintf("PiggyBack Function Start - Home");
                break;
            case pbtOneCycle:
                str.sprintf("PiggyBack Function Start - One Cycle");
                break;
            case pbtCleanout:
                str.sprintf("PiggyBack Function Start - Clean Out");
                break;
            case pbtReset:
                str.sprintf("PiggyBack Function Start - Reset");
                break;
            case pbtOutShuttleLose:
                str.sprintf("PiggyBack Function Start - Out Shuttle Lose IC");
                break;
            case pbtIndexArmVacOffErr:                                          //JerryYang 20160815 Index arm 發生Vaccum off error要做piggy back
                str.sprintf("PiggyBack Function Start - Index Arm Vacuum Off Error");
                break;
            case pbtRTCAlarm:
                str.sprintf("PiggyBack Function Start - RTC Alarm");
                break;
            case pbtOneCycleRemainSht:                                          //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                str.sprintf("PiggyBack Function Start - [I09] one cycle remain shuttle device");
                break;
        }
        iWhoTriggerPiggyBack=pbtNone;
        bRecordPiggyBackEnd=true;
        MyDBIProcess("Message", str);
    }

    bCheckIndex=false;
    LastSet.SendCT[3]=0;
    LastSet.SendCT[2]=0;
    LastSet.iIndexCount=0;                                                      //wei 20141201 Low Yield Auto Clean
    if(USE_AUTO_RETEST==eartInstall &&                                          //kevin 20150615
       (bAutoReTest_ART || IniConfig.bA10_AutoReTest))                          //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        LastSet.SendCT_ART[3]=0;
        LastSet.SendCT_ART[2]=0;
    }

    for(int i=0; i<30; i++)
    {
        ContinuousPassBinCount[i]=0;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            ContinuousPassBinBySocket[i][j]=0;
        }
    }
}
//---------------------------------------------------------------------------
void ProcessSocketPurgeCount()
{
    if(IniConfig.bD47EnableSocketPurgeFunction)
        LastSet.iD47SocketTestedCount++;
}
//------------------------------------------------------------------------------
void ProcessShowTestStatus(int Index)
{
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC &&
               TestSocket.Item[i][j]!=HAS_NULL_IC)
            {
                if(TestIF_File.iCloseSiteOnHPDontTest==2 &&                     //Steven 20251022 : 關site的位置有IC不測試送指定 bin
//                   TestSocket.iBinData[i][j]==iTestBinCount &&
                   TestSocket.PordRec[i][j].asBuffer->Strings[eErrorCode].AnsiPos("NonTestToSettedBin")!=0)
                {
                    fTestCategory->SetTestCateCellINT(Index, i, j, TestIF_File.iCloseSiteBin, clSilver);
                }
                else if(TestSocket.bPass[i][j])                                 //kevin 20140322
                {
                    fTestCategory->SetTestCateCellINT(Index, i, j, TestSocket.Item[i][j]-TEST_PASS, clGreen);
                }
                else
                {
                    fTestCategory->SetTestCateCellINT(Index, i, j, TestSocket.Item[i][j]-TEST_PASS, clRed);
                }
            }
            else                                                                //kevin 20150119 沒有 ic sit不秀
            {
                fTestCategory->SetTestCateCellINT(Index, i, j, -1, clWhite);
            }
        }
    }
    fTestCategory->ShowTestCategory(Index);                                     //jou 981017 level down display loading
}
//------------------------------------------------------------------------------
int DoLowYieldAlarm(AnsiString AlarmCode, AnsiString ErrPart)                   //Steven 20180627 (wei) : 整合Low Yield Alarm
{
    AnsiString Str;
    bool bOneCycle=false;
    int ret=K_ONECYCLE;

    if(CosFunction.bSmartAutoClean &&
       fCleaning->CheckSmartAutoClean(AlarmCode, ErrPart))                      //Sam 20250916 : Alarm後需要清除資料才能Start //Sam 20230111 : Smart Auto Clean
    {
        //DoSmartAutoClean
    }
    else if(CosFunction.bYieldAlmNeedOneCycle)
    {
        bOneCycle=true;
        if(CosFunction.bContinueFailNeedAlarmDirectly)                          //Steven 20230831 : 當使用bYieldAlmNeedOneCycle, 連續Fail要直接Alarm
        {
            if(AlarmCode=="WAR07301" ||                                         //Socket consecutive failureAlarmCode
               AlarmCode=="WAR07321" ||                                         //Arm1: consecutive failure
               AlarmCode=="WAR07322" ||                                         //Consecutive Pass Error!
               AlarmCode=="WAR07329" ||                                         //Arm2: consecutive failure
               AlarmCode=="WAR07331" ||                                         //Special Bin Socket Consecutive Failure
               AlarmCode=="WAR07332" ||                                         //Arm1: Special Bin Consecutive Failure
               AlarmCode=="WAR07333")                                           //Arm2: Special Bin Consecutive Failure
            {
                bOneCycle=false;
            }
        }
    }

    if(fCleaning->CheckSmartAutoClean(AlarmCode, ErrPart))                      //Sam 20230111 : Smart Auto Clean
    {
        //DoSmartAutoClean
    }
    else if(bOneCycle)                                                          //JerryYang 20161006 Yeild Alarm 時要做完Onecycle才能跳出Alarm視窗
    {
        bNeedOneCycleByYieldAlm=true;
        Str.sprintf("%s,%s", AlarmCode, ErrPart);
        if(fMain->slLowYieldAlarm->Text.AnsiPos(AlarmCode)==0)
        {
            fMain->slLowYieldAlarm->Add(Str);
        }
        fMain->BtnOneCycleClick(fMain);
    }
    else if(CosFunction.bLowYieldDoOneCycle)                                    //Steven 20170308 (wei) : 良率相關的都做One Cycle
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //JerryYang 20190709 JCET黃剛要求low yield不要強制one cycle
        {
            if(IniConfig.bI05LowYieldForcedOneCycle)
            {
                ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
            }
            else
            {
                ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
            }
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
        }
    }
    else if(CUSTOMER_CODE==CC_Greatek ||
            CUSTOMER_CODE==CC_AMKOR_China ||
            CUSTOMER_CODE==CC_QUALCOMM ||
            IniConfig.bKoreaFunction==true)
    {
        if(CosFunction.bYieldAlmNeedOneCycle)                                   //Steven 20240131 : 修正連續fail做 auto clean 跟 alarm互卡的問題
        {
            if(iOneCycle==0)
                ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
        }
    }
    else if(CosFunction.bSpecailLowYeild && Prod.bFailAlarmLowYieldSpecial &&
            (bLowYeildAlarmSpecial || bLowYeildAlarmSpecial1stPass))            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        ret=ShowErrorMessage(AlarmCode, 0, MMInterface, false, ErrPart);
    }
    else
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
    }

    if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //Sam 20230728 : 發生 LowYield 後重新 Start 清除 Sort Count 和 contact counterKinds
    {
        if(AlarmCode=="WAR0702" || AlarmCode=="WAR0702" || AlarmCode=="WAR0705" || AlarmCode=="WAR07321" || AlarmCode=="WAR07329")
            bInitailQtyByLowYield=true;
    }

    if(ret==K_ONECYCLE)
    {
        fMain->BtnOneCycleClick(fMain);
    }
    return ret;
}
//------------------------------------------------------------------------------
int DoLowYieldAlarmFirst(AnsiString AlarmCode, AnsiString ErrPart)              //Steven 20180627 (wei) : 整合Low Yield Alarm
{
    int ret=K_ONECYCLE;
    if(CosFunction.bLowYieldDoOneCycle)                                         //Steven 20170308 (wei) : 良率相關的都做One Cycle
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //JerryYang 20190709 JCET黃剛要求low yield不要強制one cycle
        {
            if(IniConfig.bI05LowYieldForcedOneCycle)
            {
                ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
            }
            else
            {
                ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
            }
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
        }
    }
    else if(CUSTOMER_CODE==CC_Greatek ||
            CUSTOMER_CODE==CC_AMKOR_China ||
            CUSTOMER_CODE==CC_QUALCOMM)
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
    }
    else
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
    }

    if(ret==K_ONECYCLE)
    {
        fMain->BtnOneCycleClick(fMain);
    }
    return ret;
}
//------------------------------------------------------------------------------
void CheckContinuoussFail(int Index)                                            //Steven 20110603 : 整合只叫一次
{
    bool bConseFailureBySocket_AutoClean;
    unsigned int  iConseFailureCountBySocket_AutoClean;
    bool bConseFailureByHead_AutoClean;
    unsigned int  iConseFailureCountByHead_AutoClean;
    if(iRunStartMode==FT)
    {
        bConseFailureBySocket_AutoClean     =TestIF.bAutoClean_ConseFailureBySocket_Normal;
        iConseFailureCountBySocket_AutoClean=TestIF.iAutoClean_ConseFailureCountBySocket_Normal;
        bConseFailureByHead_AutoClean       =TestIF.bAutoClean_ConseFailureByHead_Normal;
        iConseFailureCountByHead_AutoClean  =TestIF.iAutoClean_ConseFailureCountByHead_Normal;
    }
    else
    {
        bConseFailureBySocket_AutoClean     =TestIF.bAutoClean_ConseFailureBySocket_Retest;
        iConseFailureCountBySocket_AutoClean=TestIF.iAutoClean_ConseFailureCountBySocket_Retest;
        bConseFailureByHead_AutoClean       =TestIF.bAutoClean_ConseFailureByHead_Retest;
        iConseFailureCountByHead_AutoClean  =TestIF.iAutoClean_ConseFailureCountByHead_Retest;
    }
    //ChungHung 20131223 add for SCK request end

    bool bSocketConsecutiveFailure=false, bConsecutiveFailure=false, bSocketAllSiteFail=false;                          //kevin 20170825 (Steven) add all site fail
    bool bSocketConsecutiveFailure_AutoClean=false, bConsecutiveFailure_AutoClean=false;                                //ChungHung 20131223 add for SCK request
    bool bDoAutoCleanByBinCount=false;                                          //Steven 20160308 : By Bin count do auto clean
    bool bDoAutoCleanBySiteCount=false;                                         //Steven 20160308 : By Bin pre site count do auto clean
    bool bSpecailBinContinuousFailByArm=false,bSpecialBinContinuousFailBySocket=false;                                  //ChungHung 20140730 add ContinuousFailHaveOneCycle
    AnsiString SocketErrPart="", ErrPart="", AutoCleanErrPart="";
    int iGetBin=0, iBinCount=0, iSite=0, iTray[TEST_MAX_BIN]={0};               //JerryYang 20221215 : 33->TEST_MAX_BIN
    int iAllSiteFailCount[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iSiteCount=0;

    double dSiteByBinYield=0.0;                                                 //JerryYang 20170712 (Steven) by site by bin compare percent
    double dSiteByBinYield1[32]={0.0};
    double dArm1ByBinYield=0.0;
    double dArm2ByBinYield=0.0;
    double dSiteByBinYieldMax=0.0;
    double dSiteByBinYieldMin=100.0;
    double dArm1ByBinYieldMax=0.0;
    double dArm1ByBinYieldMin=100.0;
    double dArm2ByBinYieldMax=0.0;
    double dArm2ByBinYieldMin=100.0;
    AnsiString sWhichSiteByBinYieldMax="";
    AnsiString sWhichSiteByBinYieldMin="";
    AnsiString sArm1WhichSiteByBinYieldMax="";
    AnsiString sArm1WhichSiteByBinYieldMin="";
    AnsiString sArm2WhichSiteByBinYieldMax="";
    AnsiString sArm2WhichSiteByBinYieldMin="", sSocketAllSiteErrPart="";

    int Z=0, iTotalSite=0, iSocketFailIc=0;

    if(TestIF.iShuttleMode==1)                                                  //kevin 20170825 (Steven) use arm open/close site
    {
        if(TestIF.iShuttle_Sel==1)
        {
            Z=1;
        }
        else
        {
            Z=0;
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bTestSiteUse[Z][i][j])
            {
                iSite++;
                if(IniConfig.bA09_ByArmCloseSite &&                             //Steven 20220818 : By Arm Continue fail, auto site off
                   IsNNMode()==0)                                               //Sam 20250401 : 修正 NN 模式計數錯誤
                {
                    iSiteCount+=2;
                }
                else
                {
                    iSiteCount++;
                }
            }

            iAllSiteFailCount[i][j]=0;                                          //kevin 20170825 (Steven) all site fail
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;

                if(iGetBin>(iTestBinCount-1))                                   //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF.iAutoClean_Function==true &&
                   bIsAutoOneCycle==false)                                      //確保不要重複計算
                {
                    iAutoCleanByBinCount[iGetBin]++;
                    iAutoCleanBySiteCount[i][j][iGetBin]++;
                    if(Prod.iAutoCleanByBin[iGetBin]>0 &&                       //Steven 20160308 : By Bin count do auto clean
                       iAutoCleanByBinCount[iGetBin]>=Prod.iAutoCleanByBin[iGetBin])
                    {
                        bDoAutoCleanByBinCount=true;
                        AutoCleanErrPart+=IndexSuckName[i][j];                  //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    }

                    if(Prod.iAutoCleanBySite[iGetBin]>0 &&                      //Steven 20160308 : By Bin pre site count do auto clean
                       iAutoCleanBySiteCount[i][j][iGetBin]>=Prod.iAutoCleanBySite[iGetBin])
                    {
                        bDoAutoCleanBySiteCount=true;
                        AutoCleanErrPart+=IndexSuckName[i][j];                  //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    }
                }

                if(TestSocket.bPass[i][j])                                      //pass
                {
                    ContinuousFailSKTCount[i][j]=0;
                    ContinuousFailARMCount[Index][i][j]=0;
                    ContinuousFailSKTCount_AutoClean[i][j]=0;                   //ChungHung 20131223 add for SCK request
                    ContinuousFailARMCount_AutoClean[Index][i][j]=0;

                    if(Prod.iContsFailSocketAlarmCT>0)                          //Steven 20230529 : Spc Bin Couont改成連續錯誤
                    {
                        SpecialBinContinuousFailSKTCount[i][j]=0;
                        SpecialBinContinuousFailARMCount[Index][i][j]=0;
                    }
                }
                else if(TestSocket.Item[i][j]!=HAS_IC &&                        //ChungHung 20140730 add ContinuousFailHaveOneCycle start
                        TestSocket.Item[i][j]!=HAS_NULL_IC &&
                        TestSocket.bPass[i][j]==false)                          //Kevin 20161102 : 沒IC不Alarm連續fail
                {
                    if(Prod.bConsFail[iGetBin]==true ||
                       CUSTOMER_CODE==CC_Greatek ||                             //Sam 20200224 : 正賢要求只要是 Fail 就計數不管 Prod.bConsFail 設定
                       CUSTOMER_CODE==CC_SIGURD_HUKOU)                          //Sam 20210312 : 矽格湖口卓邵要求
                    {
                        if(fContact->fShow==false)
                        {
                            if(CUSTOMER_CODE==CC_KYEC_CHEN &&
                               Prod.bContsFailIgnore)                           //wei 20160115 銅鑼前幾顆不計算ContsFail
                            {
                                if(Prod.bContsFailBySocket==false &&
                                   Prod.bContsFailByHead==false)
                                    iContsFailIgnoreCount=0;
                                bContsFailIgnoreStart=true;
                                iContsFailIgnoreCount++;
                                if(iContsFailIgnoreCount>=Prod.iContsFailIgnore)
                                    bContsFailIgnoreStart=false;
                            }
                            else
                            {
                                bContsFailIgnoreStart=false;
                            }

                            if(bContsFailIgnoreStart==false)
                            {
                                ContinuousFailSKTCount[i][j]++;
                                ContinuousFailARMCount[Index][i][j]++;
                                ContinuousFailSKTCount_AutoClean[i][j]++;       //ChungHung 20131223 add for SCK request
                                ContinuousFailARMCount_AutoClean[Index][i][j]++;
                            }
                        }
                    }

                    if(Prod.bSpecialBinBySocket[iGetBin]==1)                    //ChungHung 20140730 add ContinuousFailHaveOneCycle start
                        SpecialBinContinuousFailSKTCount[i][j]++;

                    if(Prod.bSpecialBinByArm[iGetBin]==1)
                        SpecialBinContinuousFailARMCount[Index][i][j]++;

                    if(CosFunction.bAllSiteSameFailBinShowAlarm==true &&        //JerryYang 20170221 (wei) 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm,改成可複選bin
                       IniConfig.bI34AllSiteAreSameFailBinShowAlarm==true)
                    {
                        for(int k=0; k<iTestBinCount; k++)                      //JerryYang 20170221 (wei) 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm,改成可複選bin
                        {
                            iTray[k]=Prod.iT6CatData[k];
                            if(iTray[k]>=0 &&                                   //Steven 20230929 : 加入保護
                               Prod.iIsFailT6[iTray[k]]==1 &&                   //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
                               bSpecificBin[k]==true)
                            {
                                if(iGetBin==k)
                                {
                                    iBinCount++;
                                    if(bTestSiteUse[Z][i][j])                   //kevin 20170831 開關site
                                    {
                                        sSocketAllSiteErrPart+=IndexSuckName[i][j];                                     //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                                    }
                                }
                            }
                        }
                    }
                    iAllSiteFailCount[i][j]=1;                                  //kevin 20170825 (Steven) all site fail
                }
            }
        }
    }

    if(CosFunction.bBySiteByBinPercentCompare==true)                            //JerryYang 20170712 (Steven) by site by bin compare percent
    {
        TastCategory.UpdataCount(true);                                         //Steven 20250514 : 統一計算數量
    }

    if(CosFunction.bAllSiteSameFailBinShowAlarm==true &&                        //kevin 20170825 (Steven) add all site fail
       IniConfig.bI34AllSiteAreSameFailBinShowAlarm==true)                      //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    {
        if(iBinCount>0 && iBinCount>=iSite)
        {
            DoLowYieldAlarmFirst("WAR07334", asErrPart);                        //Steven 20180627 (wei) : 整合Low Yield Alarm
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(Prod.bAllSiteFail && bTestSiteUse[Z][i][j])                      //kevin 20170825 (Steven) all site fail  //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開
            {
                iTotalSite++;
                if(iAllSiteFailCount[i][j]>0)                                   //kevin 20170825 all site fail[i][j]>0)
                {
                    iSocketFailIc++;
                    sSocketAllSiteErrPart+=IndexSuckName[i][j];                 //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }

            if(Prod.bContsFailBySocket && Prod.iContsFailSocketAlarmCT!=0)
            {
                if(ContinuousFailSKTCount[i][j]>=Prod.iContsFailSocketAlarmCT)
                {
                    ContinuousFailSKTCount[i][j]=0;

                    if(bCanAutoCloseSite &&
                       TestIF_File.bLowYieldAutoSiteOffByContiFail)             //Steven 20200420 : Continue fail, auto site off
                    {
                        if(IniConfig.bA09_ByArmCloseSite &&                     //Steven 20220818 : By Arm Continue fail, auto site off
                           IsNNMode()==0)                                       //Sam 20250401 : 修正 NN 模式計數錯誤
                        {
                            iSiteCount-=2;
                        }
                        else
                        {
                            iSiteCount--;
                        }

                        if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                        {
                            bSocketConsecutiveFailure=true;
                        }
                        else
                        {
                            bLowYieldCloseSite[0][i][j]=true;
                            bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                    else
                    {
                        bSocketConsecutiveFailure=true;
                    }
                    SocketErrPart+=IndexSuckName[i][j];                         //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    asErrTemp+=IndexSuckName[i][j];                             //JerryYang 20160407 存error位置
                }
            }
            else
            {
                ContinuousFailSKTCount[i][j]=0;
            }

            if(Prod.bContsFailByHead && Prod.iContsFailHeadAlarmCT!=0)
            {
                if(ContinuousFailARMCount[Index][i][j]>=Prod.iContsFailHeadAlarmCT)
                {
                    ContinuousFailARMCount[Index][i][j]=0;
                    if(bCanAutoCloseSite &&
                       TestIF_File.bLowYieldAutoSiteOffArmContiFail &&
                       (IniConfig.bA09_ByArmCloseSite ||                        //Steven 20220818 : By Arm Continue fail, auto site off
                        IsNNMode()>0))                                          //Sam 20250401 : 修正 NN 模式計數錯誤
                    {
                        if(IsNNMode()>0)                                        //Sam 20250401 : 修正 NN 模式計數錯誤
                        {
                            if(bLowYieldCloseSite[Index][i][j]==false)
                                iSiteCount--;
                        }
                        else
                        {
                            iSiteCount--;
                        }

                        if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                        {
                            bConsecutiveFailure=true;
                        }
                        else
                        {
                            bLowYieldCloseSite[Index][i][j]=true;
                        }
                    }
                    else
                    {
                        bConsecutiveFailure=true;
                    }
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailARMCount[Index][i][j]=0;
            }

            if(bConseFailureBySocket_AutoClean &&                               //ChungHung 20131223 add for SCK request start
               iConseFailureCountBySocket_AutoClean!=0)
            {
                if(ContinuousFailSKTCount_AutoClean[i][j]>=iConseFailureCountBySocket_AutoClean)
                {
                    ContinuousFailSKTCount_AutoClean[i][j]=0;
                    bSocketConsecutiveFailure_AutoClean=true;
                    SocketErrPart+=IndexSuckName[i][j];                         //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailSKTCount_AutoClean[i][j]=0;
            }

            if(bConseFailureByHead_AutoClean && iConseFailureCountByHead_AutoClean!=0)
            {
                if(ContinuousFailARMCount_AutoClean[Index][i][j]>=iConseFailureCountByHead_AutoClean)
                {
                    ContinuousFailARMCount_AutoClean[Index][i][j]=0;
                    bConsecutiveFailure_AutoClean=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailARMCount_AutoClean[Index][i][j]=0;
            }

            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140730 add ContinuousFailHaveOneCycle start
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if(iGetBin>(iTestBinCount-1))
                    continue;

                if(Prod.bSpecialBinBySocket[iGetBin] &&
                   SpecialBinContinuousFailSKTCount[i][j]>=Prod.iSpecialBinCountBySocket[iGetBin])
                {
                    SpecialBinContinuousFailSKTCount[i][j]=0;
                    bSpecialBinContinuousFailBySocket=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }

                if(Prod.bSpecialBinByArm[iGetBin] &&
                   SpecialBinContinuousFailARMCount[Index][i][j]>=Prod.iSpecialBinCountByArm[iGetBin])
                {
                    SpecialBinContinuousFailARMCount[Index][i][j]=0;
                    bSpecailBinContinuousFailByArm=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    if(CUSTOMER_CODE==CC_SJ_Semiconductor)                      //RogerYang 20260123 : add for Alarm Message
                    {
                        asErrPart.sprintf("%s : BIN %d over limit count %d ", ErrPart, iGetBin, Prod.iSpecialBinCountByArm[iGetBin]);
                    }
                }
            }
        }
    }

    if(Prod.bAllSiteFail && (iSocketFailIc>=iTotalSite))                        //Ifor 20190611 all site fail Alarm By Ft/Rt Setting
    {
        iallSitCount++;
        if(iallSitCount>=Prod.iAllSiteFailCount)                                //kevin 20180720 (wei) add all site fail count      //Steven 20230118 : All site fail RT
            bSocketAllSiteFail=true;                                            //kevin 20170825 add
    }

    if(CosFunction.bBySiteByBinPercentCompare==true)                            //JerryYang 20170712 (Steven) by site by bin compare percent
    {
        for(int iCat=0; iCat<iTestBinCount; iCat++)                             //Site by Bin
        {
            dSiteByBinYieldMax=0.0;
            dSiteByBinYieldMin=100.0;
            dArm1ByBinYieldMax=0.0;
            dArm1ByBinYieldMin=100.0;
            dArm2ByBinYieldMax=0.0;
            dArm2ByBinYieldMin=100.0;
            if(Prod.bSpecBinBySiteCompareEnable[iCat]==true &&
               Prod.dSpecBinBySiteComparePercent[iCat]>0 &&
               RunInfo.iUnloadCount-iYeildCT[5]>Prod.iSpecBinBySiteCompareIgnore[iCat])
            {
                if(CUSTOMER_CODE==CC_JCET)
                {
                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((LastSet.bUseTestSocket[0][iRow][iCol] ||        //jou 20171023 (wei) : 修正關site也會by bin site gap alarm
                                LastSet.bUseTestSocket[1][iRow][iCol]) &&
                               TastCategory.iCountSocketTotal[iRow][iCol]>0)
                            {
                                dSiteByBinYield=ChangeToFloat((double)(TastCategory.iCountCategory[0][iRow][iCol][iCat]+TastCategory.iCountCategory[1][iRow][iCol][iCat]), (double)TastCategory.iCountSocketTotal[iRow][iCol]);  //Steven 20250820 : 針對除以0加上保護
                            }
                            else
                            {
                                dSiteByBinYield=-1.0;
                                if(LastSet.bUseTestSocket[0][iRow][iCol]==false)                                        //jou 20171225 (Steven) : 修正被關掉的site再開site也會by bin site gap alarm
                                    TastCategory.iCountCategory[0][iRow][iCol][iCat]=0;
                                if(LastSet.bUseTestSocket[1][iRow][iCol]==false)
                                    TastCategory.iCountCategory[1][iRow][iCol][iCat]=0;
                            }

                            if(dSiteByBinYieldMax<dSiteByBinYield)
                            {
                                dSiteByBinYieldMax=dSiteByBinYield;
                                sWhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }

                            if(dSiteByBinYieldMin>dSiteByBinYield && dSiteByBinYield!=-1.0)
                            {
                                dSiteByBinYieldMin=dSiteByBinYield;
                                sWhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }
                        }
                    }

                    if((dSiteByBinYieldMax-dSiteByBinYieldMin)>Prod.dSpecBinBySiteComparePercent[iCat])
                    {
                        ErrPart=sWhichSiteByBinYieldMax+sWhichSiteByBinYieldMin;
                        DoLowYieldAlarm("WAR07335", ErrPart);                   //Steven 20180627 (wei) : 整合Low Yield Alarm
                        iYeildCT[5]=RunInfo.iUnloadCount;
                        fYieldMonitoring->ClearYieldCount();
                    }
                }
                else
                {
                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((LastSet.bUseTestSocket[0][iRow][iCol] ||        //jou 20171023 (wei) : 修正關site也會by bin site gap alarm
                                LastSet.bUseTestSocket[1][iRow][iCol]) &&
                                TastCategory.iCountSocketTotal[iRow][iCol]>0)
                            {
                                dSiteByBinYield1[iRow*8+iCol]=ChangeToFloat((double)(TastCategory.iCountCategory[0][iRow][iCol][iCat]+TastCategory.iCountCategory[1][iRow][iCol][iCat]), (double)TastCategory.iCountSocketTotal[iRow][iCol]);
                            }
                            else
                            {
                                dSiteByBinYield1[iRow*8+iCol]=0.0;
                            }

                            if(dSiteByBinYieldMin>dSiteByBinYield1[iRow*8+iCol])
                            {
                                dSiteByBinYieldMin=dSiteByBinYield1[iRow*8+iCol];
                            }
                        }
                    }

                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((dSiteByBinYield1[iRow*8+iCol]-dSiteByBinYieldMin)>Prod.dSpecBinBySiteComparePercent[iCat] && bLowYeildAlarm==false)
                            {
                                bByBinPercentCompareAlarm=true;
                                ErrPart+=IndexSuckName[iRow][iCol];             //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }
                        }
                    }

                    if(bByBinPercentCompareAlarm && bLowYeildAlarm==false)
                    {
                        DoLowYieldAlarm("WAR07335", ErrPart);                   //Steven 20180627 (wei) : 整合Low Yield Alarm
                        iYeildCT[5]=RunInfo.iUnloadCount;
                        fYieldMonitoring->ClearYieldCount();
                        bByBinPercentCompareAlarm=false;
                    }
                }
            }
            //Site by Arm
            if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
               Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
               RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat])
            {
                ErrPart="";
                for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                {
                    for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                    {
                        if(TastCategory.iCountCategory[0][iRow][iCol][iCat]>0 && TastCategory.iCountHeadTotal[0][iRow][iCol]>0)
                            dArm1ByBinYield=ChangeToFloat((double)TastCategory.iCountCategory[0][iRow][iCol][iCat], (double)TastCategory.iCountHeadTotal[0][iRow][iCol]);
                        else
                            dArm1ByBinYield=0.0;
                        if(dArm1ByBinYieldMax<dArm1ByBinYield)
                        {
                            dArm1ByBinYieldMax=dArm1ByBinYield;
                            sArm1WhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }

                        if(dArm1ByBinYieldMin>dArm1ByBinYield &&
                          (LastSet.bUseTestSocket[0][iRow][iCol] ||
                           LastSet.bUseTestSocket[1][iRow][iCol]))              //JerryYang 20250120 : modify
                        {
                            dArm1ByBinYieldMin=dArm1ByBinYield;
                            sArm1WhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }

                        if(TastCategory.iCountCategory[0][iRow][iCol][iCat]>0 && TastCategory.iCountHeadTotal[0][iRow][iCol]>0)
                            dArm2ByBinYield=ChangeToFloat((double)TastCategory.iCountCategory[0][iRow][iCol][iCat], (double)TastCategory.iCountHeadTotal[0][iRow][iCol]);
                        else
                            dArm2ByBinYield=0.0;
                        if(dArm2ByBinYieldMax<dArm2ByBinYield)
                        {
                            dArm2ByBinYieldMax=dArm2ByBinYield;
                            sArm2WhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];
                        }

                        if(dArm2ByBinYieldMin>dArm2ByBinYield)
                        {
                            dArm2ByBinYieldMin=dArm2ByBinYield;
                            sArm2WhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];
                        }
                    }
                }

                if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
                   Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
                   RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat] &&
                   (dArm1ByBinYieldMax-dArm1ByBinYieldMin) > Prod.dSpecBinByArmPerSiteComparePercent[iCat])
                {
                    ErrPart=sArm1WhichSiteByBinYieldMax+sArm1WhichSiteByBinYieldMin;
                    DoLowYieldAlarm("WAR07336", ErrPart);                       //Steven 20180627 (wei) : 整合Low Yield Alarm
                    iYeildCT[6]=RunInfo.iUnloadCount;
                    fYieldMonitoring->ClearYieldCount();
                }

                if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
                    Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
                    TastCategory.iTotalSocket>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat] &&
                    RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinBySiteCompareIgnore[iCat] &&
                  (dArm2ByBinYieldMax-dArm2ByBinYieldMin)>Prod.dSpecBinByArmPerSiteComparePercent[iCat])
                {
                    ErrPart=sArm2WhichSiteByBinYieldMax+sArm2WhichSiteByBinYieldMin;
                    DoLowYieldAlarm("WAR07337", ErrPart);                       //Steven 20180627 (wei) : 整合Low Yield Alarm
                    iYeildCT[6]=RunInfo.iUnloadCount;
                    fYieldMonitoring->ClearYieldCount();
                }
            }
        }
    }

    if(bSocketAllSiteFail)                                                      //kevin 20170825 (Steven) all site fail
    {
        bSocketAllSiteFail=false;
        iallSitCount=0;                                                         //kevin 20180720 (wei) all site fail count
        DoLowYieldAlarmFirst("WAR07334", asErrPart);                            //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    if(bSpecialBinContinuousFailBySocket)                                       //ChungHung 20140730 add ContinuousFailHaveOneCycle start
    {
        fYieldMonitoring->ClearYieldCount();                                    //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        bSpecialBinContinuousFailBySocket=false;
        DoLowYieldAlarm("WAR07331", ErrPart);                                   //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    if(bSpecailBinContinuousFailByArm)
    {
        fYieldMonitoring->ClearYieldCount();                                    //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        bSpecailBinContinuousFailByArm=false;
        if(Index==0)
            DoLowYieldAlarm("WAR07332", asErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
        else
            DoLowYieldAlarm("WAR07333", asErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    AnsiString aLowYield;
    if(bSocketConsecutiveFailure_AutoClean)                                     //ChungHung 20131223 add for SCK request start
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true &&
           (TestIF.iAutoClean_Mode & M_SOCKET_ALARM) && bConseFailureBySocket_AutoClean)
        {
            aLowYield = "AutoClean : Consecutive Failure by Socket : " + SocketErrPart + " Contact Count : " + iAutoClean_IndexContactCount;
            RecordProcess(aLowYield);
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
        }
    }

    if(bConsecutiveFailure_AutoClean)
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true &&
           (TestIF.iAutoClean_Mode & M_SOCKET_ALARM) && bConseFailureByHead_AutoClean)
        {
            aLowYield = "AutoClean : Consecutive Failure by Head : " + SocketErrPart + " Contact Count : " + iAutoClean_IndexContactCount;
            RecordProcess(aLowYield);
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
        }
    }

    if(bDoAutoCleanByBinCount || bDoAutoCleanBySiteCount)                       //Steven 20160308 : By Bin count do auto clean
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true)
        {
            if(bDoAutoCleanByBinCount)
                aLowYield = "AutoClean by Bin count : " + AutoCleanErrPart;
            else
                aLowYield = "AutoClean by site count: " + AutoCleanErrPart;
            RecordProcess(aLowYield);
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
        }
    }

    if(bSocketConsecutiveFailure)                                               //ChungHung 20131223 add for SCK request end
    {
        DoLowYieldAlarm("WAR07301", SocketErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
        fYieldMonitoring->ClearYieldCount();                                    //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
    }

    if(bUseTwoArm32Site==true)
    {
        if(bSocketConsecutiveFailure)
            bConsecutiveFailure=false;
    }

    if(bConsecutiveFailure)
    {
        fYieldMonitoring->ClearYieldCount();                                    //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        if(Index==0)
            DoLowYieldAlarm("WAR07321", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
        else
            DoLowYieldAlarm("WAR07329", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
    }
}
//------------------------------------------------------------------------------
void ProcessContinuoussFailForATC(int Index)                                    //Steven 20151123 : Continue Fail Temp Offset for ATC
{
    int iGetBin=0;

    if(ATC_SYSTEM!=eATCHonPrecType || LastSet.iTemperature!=Tempture_Hot)
        return;

    for(int j=0; j<2; j++)                                                      //Steven 20151209 : Modify for ATC 7.0
    {
        if(TestSocket.Item[0][j]>=TEST_PASS)
        {
            iGetBin=TestSocket.Item[0][j]-TEST_PASS;
            if(iGetBin>(iTestBinCount-1))
            {
                Temperature.iATCCurrentFailCount[j]++;
                Temperature.iATCCurrentFailCount[j+2]++;
            }

            if(TestSocket.bPass[0][j])                                          //pass
            {
                Temperature.iATCCurrentFailCount[j]=0;
                Temperature.iATCCurrentFailCount[j+2]=0;
            }
            else
            {
                Temperature.iATCCurrentFailCount[j]++;
                Temperature.iATCCurrentFailCount[j+2]++;
            }
        }
    }
}
//---------------------------------------------------------------------------
int GetMultiplierNum()                                                          //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
{
    return fProductionInfo->cDynaThres->GetMultiplierNum();
}
//---------------------------------------------------------------------------
void CheckContinuoussPassBySocket(int Index)                                    //Steven 20110919
{
    unsigned int iGetBin=0;
    AnsiString ErrPart="";

    if(LastSet.iRunStartMode==rsmAutoSiteMap)                                   //Steven 20110506 : Auto Site Mapping時不啟動
    {
        return;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if(int(iGetBin)>(iTestBinCount-1))                              //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(Prod.bContinuousPassBySocket && Prod.iContinuousPassBinCountBySocket!=0 && TestSocket.bPass[i][j])   //pass
                {
                    ContinuousPassBinBySocket[i][j]++;
                    if(ContinuousPassBinBySocket[i][j]>=(Prod.iContinuousPassBinCountBySocket*GetMultiplierNum()))      //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
                    {
                        if(CosFunction.bPiggyBackShowMainForm && Prod.iCountAlarmAction==0)                             //kevin 20131101 強致 Enable Yield 裡面piggyback 功能    //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                        {
                            bContinuoussPass=true;                              //秀警告不停機
                        }
                        else if(CosFunction.bPiggyBackShowMainForm && Prod.iCountAlarmAction==1)                        //kevin 20131101 強致 Enable Yield 裡面piggyback 功能   //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                        {                                                       //index check
                            iWhoTriggerPiggyBack=pbtContinualPassBinBySocket;   //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                            bContinuoussPass=false;                             //  kevin 20131009改為全域  //Steven 20140923 : 移到上面
                        }
                        else if(Prod.iCountAlarmAction==0)
                        {
                            ContinuousPassBinBySocket[i][j]=0;
                            bContinuoussPass=true;
                            ErrPart+=IndexSuckName[i][j];                       //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }
                        else
                        {
                            bContinuoussPass=false;                             //  kevin 20131009改為全域
                            ContinuousPassBinBySocket[i][j]=0;
                            iWhoTriggerPiggyBack=pbtContinualPassBinBySocket;   //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                        }
                    }
                }
                else
                {
                    ContinuousPassBinBySocket[i][j]=0;
                }
            }
        }
    }

    if(bContinuoussPass)
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
        {
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
        }

        if(CosFunction.bPiggyBackShowMainForm==false)                           //kevin 20131009 高雄ASE不能停機只能畫面秀警告
        {
            DoLowYieldAlarm("WAR07322", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
            bContinuoussPass=false;
            fProductionInfo->cDynaThres->AddThresholdNum();                     //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
        }
    }
}
//---------------------------------------------------------------------------
void CheckContinuoussPass(int Index)
{
    unsigned int iGetBin=0;
    bool bContinuoussPass=false;
    AnsiString ErrPart="";

    if(LastSet.iRunStartMode==rsmAutoSiteMap)                                   //Steven 20110506 : Auto Site Mapping時不啟動
    {
        return;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if((int)iGetBin>(iTestBinCount-1))                              //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(Prod.bContinuousPass && ((Prod.iContinuousPassBin)==iGetBin) &&
                   Prod.iContinuousPassBinCount!=0 && TestSocket.bPass[i][j])   //pass    //Steven 20140922 : 因為沒有Bin 0,所以不用-1
                {
                    ContinuousPassBinCount[iGetBin]++;
                    if(ContinuousPassBinCount[iGetBin]>=Prod.iContinuousPassBinCount)
                    {
                        ContinuousPassBinCount[iGetBin]=0;
                        if(Prod.iCountAlarmAction==0)
                        {
                            bContinuoussPass=true;
                            ErrPart+=IndexSuckName[i][j];                       //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }
                        else
                        {
                            iWhoTriggerPiggyBack=pbtContinualPassBin;           //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                        }
                    }
                }
                else
                {
                    ContinuousPassBinCount[Prod.iContinuousPassBin]=0;          //Steven 20210812 : iGetBin --> Prod.iContinuousPassBin
                }
            }
        }
    }

    if(bContinuoussPass)
    {
        DoLowYieldAlarm("WAR07322", ErrPart);                                   //Steven 20180627 (wei) : 整合Low Yield Alarm
    }
}
//---------------------------------------------------------------------------
void ProcessHeadContactCount(int Index)                                         //Ifor 20160516 京元要求銦片 Life Time 功能
{
    bool bHaveUseArm=true;
    int iPos;

    if(CosFunction.bUseHeadContactCount)
    {
        if(TestIF_File.iShuttleMode==1)
        {
            if(TestIF_File.iShuttle_Sel==0 && Index==1)
            {
                bHaveUseArm=false;
            }
            else if(TestIF_File.iShuttle_Sel==1 && Index==0)
            {
                bHaveUseArm=false;
            }
        }
        else
        {
            bHaveUseArm=true;
        }

        //Ifor 20160513 修改Arm2 Life Time 異常問題
        for(int iA=0; iA<3 ; iA++)                                              //wei 20160509
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[Index][i][j])
                    {
                        iPos=j*2+i;
                        if(bHaveUseArm==true)
                        {
                            IniConfig.HeadContactCount[iA][Index][iPos]++;
                            IniConfig.HeadContactCountHistory[iA][Index][iPos]++;

                            if(IniConfig.HeadContactCountHistory[iA][Index][iPos]>=1000000000)
                                IniConfig.HeadContactCountHistory[iA][Index][iPos]=0;
                        }
                        iSLT_HeadContactCount[Index][iPos]=IniConfig.HeadContactCount[0][Index][iPos];                  //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void ProcessSocketCount(int Index)
{
    //JerryYang 20170208 (wei) socket contact計數
    //==>
    if(CUSTOMER_CODE==CC_ASE_CL ||
       CUSTOMER_CODE==CC_PTI ||                                                 //Sam 20170526 (jou) PTI要求socket contact計數
       CUSTOMER_CODE==CC_TERAPOWER ||                                           //Sam 20190916 : 晶兆成也記錄測頭 Coun
       CUSTOMER_CODE==CC_ASE_JP ||                                              //Sam 20220706 : ASE_JP 也記錄測頭 Count
       CUSTOMER_CODE==CC_AMD_M ||
       CosFunction.bOEEFunction ||
       CosFunction.bUseSocketContactCount)                                      //Sam 20220720 : 新增一組 Socket Count
    {
        for(int i=0; i<FTestSuck.iShtRow; i++)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                if(IsNNMode()==NN_2Row)
                {
                    if(LastSet.bUseTestSocket[0][i][j] &&
                       TestSocket.Item[i][j]!=NULL_IC &&
                       TestSocket.Item[i][j]!=HAS_NULL_IC)                      //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i][j]++;
                        if(LastSet.iSocketContactCount[i][j]>=1000000000 ||
                           LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i][j]=0;

                       //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i][j]++;
                        IniConfig.SocketContactCountHistory[i][j]++;
                        if(IniConfig.SocketContactCountHistory[i][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i][j]=0;
                    }

                    if(LastSet.bUseTestSocket[1][i+2][j] &&                     //Sam 20201021 : Fix 32Site contact count
                       TestSocket.Item[i+2][j]!=NULL_IC &&
                       TestSocket.Item[i+2][j]!=HAS_NULL_IC)                    //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i+2][j]++;
                        if(LastSet.iSocketContactCount[i+2][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i+2][j]=0;

                        //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i+2][j]++;
                        IniConfig.SocketContactCountHistory[i+2][j]++;
                        if(IniConfig.SocketContactCountHistory[i+2][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i+2][j]=0;
                    }

                    if(LastSet.iSocketContactCount[i][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                        LastSet.iSocketContactCount[i][j]=0;
                }
                else
                {
                    if(LastSet.bUseTestSocket[Index][i][j] &&
                       TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)                            //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i][j]++;
                        if(LastSet.iSocketContactCount[i][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i][j]=0;

                        //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i][j]++;
                        IniConfig.SocketContactCountHistory[i][j]++;
                        if(IniConfig.SocketContactCountHistory[i][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i][j]=0;
                    }
                }
            }
        }
    }
    //<==
    //JerryYang 20170208 (wei) socket contact計數
}
//---------------------------------------------------------------------------
void ProcessAutoCleanCount()
{
    AnsiString s="";                                                            //Sam 20230620 : 優化 Smart Auto Clean
    int IntervalCount=0, sum=0, ipass=0;
    double dYield=0.0;

    AnsiString sYield1="", sYield2="";

    if(IniConfig.bEnableAutoCleanFunction)
    {
        if(CosFunction.bAutoCleanOnHotPlate &&
           bACInterval &&
           LastSet.iRunStartMode!=rsmAutoSiteMap)
        {
            if(TestIF.iAutoClean_Function)                                      //Auto Clean
                iAutoClean_IndexContactCount++;
            else
                iAutoClean_IndexContactCount=0;
            fMain->AutoCleanContactCountLabel->Caption=iAutoClean_IndexContactCount;
            EnableAutoclean(false);                                             //kevin 20120501 啟動autoclean
        }
        else
        {
            //Steven 20260427 : ATK P260427-ATK-H9-01 fix operator precedence
            //                  Original: A && B || C => (A && B) || C, lets SOCKET_ALARM bypass AutoSiteMap guard
            if(LastSet.iRunStartMode!=rsmAutoSiteMap &&
               ((TestIF_File.iAutoClean_Mode&M_INTERVAL) ||
                (TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM)))                  //ChungHung 20131225 add
            {
                if(TestIF.iAutoClean_Function)                                  //Auto Clean
                    iAutoClean_IndexContactCount++;
                else
                    iAutoClean_IndexContactCount=0;

                fMain->AutoCleanContactCountLabel->Caption=iAutoClean_IndexContactCount;
                fLotInfo->edtAutoCleanContactCount->Text=iAutoClean_IndexContactCount;                                  //ChungHung 20131225 add

                if(CosFunction.bSmartAutoClean && TestIF.bACSmart)
                {
                    fLotInfo->RefreshYieldMonitor();
                    IntervalCount=iAdaptiveACInterval;
                }
                else
                {
                    IntervalCount=TestIF.iAutoClean_IntervalContact;
                }

                if(CosFunction.bAdaptiveYield && Prod.bAdaptiveLowYield)        //Sam 20230914 : 自適應性良率監控
                {
                    sum=0;
                    ipass=0;
                    for(int i=0; i<6; i++)
                    {
                        sum+=LastSet.BinCT[0][i];
                        if(BinSelect[iTestRunMode].iStackDefFailCate[i]==0)
                            ipass+=LastSet.BinCT[0][i];
                    }

                    if(sum>0)
                    {
                        dYield=(double)ipass*100/sum;
                    }
                    else
                    {
                        dYield=0;
                    }

                    if(sum>Prod.iLowYieldCount &&
                       dAdaptiveStardardYield<0)                                // IC 數量到了計算良率當標準
                    {
                        dAdaptiveStardardYield=dYield;
                        s.sprintf("Adaptive Low Yield : %2.2f%s", dAdaptiveStardardYield, "%");
                        fLotInfo->lblAdaptiveLowYield->Caption=s;
                    }
                }

                //Steven 20260427 : ATK P260427-ATK-H9-01 fix off-by-one
                //                  Original >=(IntervalCount-1) triggers one IC too early
                if(TestIF.iAutoClean_Function &&
                   IntervalCount!=0 &&
                   (TestIF_File.iAutoClean_Mode & M_INTERVAL) &&
                   iCleanOut==0 &&
                   iAutoClean_IndexContactCount>=IntervalCount)
                {
                    if(CosFunction.bSmartAutoClean &&
                       TestIF.bACSmart &&
                       iRunACSmart==0)                                          //Sam 20230620 : 優化 Smart Auto Clean
                    {
                        iRunACSmart=2;
                        if(dAdaptiveStardardYield<0)                            //若 Interval 到了 IC 數量未到就計算良率當標準
                            dAdaptiveStardardYield=dYield;

                        if(CosFunction.bAdaptiveYield &&
                           Prod.bAdaptiveLowYield)                              //Sam 20230914 : 自適應性良率監控
                        {                                                       //良率高於Max設定
                            if(dYield>(Prod.dLowYieldLimit+Prod.iAdaptiveYieldMax))
                            {
                                iAdaptiveLowYieldCntNor=0;
                                iAdaptiveLowYieldCntMin=0;
                                iACUseParam=1;
                                s.sprintf("Start smart auto clean use %d parameter. ↑%2.2f%s > %2.2f%s Max Area. Yield %2.2f%s change to %2.2f%s",
                                            iACUseParam,
                                            dYield,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMax,
                                            "%",
                                            dAdaptiveStardardYield,
                                            "%",
                                            dYield,
                                            "%");
                                fCleaning->ChangeACSmartInterval(99, s);        //良率大於MAX 設定無條件interval 升至最高Count
                            }
                            else
                            {
                                if(dYield<dAdaptiveStardardYield)               //低於自適應性良率
                                {                                               //良率低於Min設定
                                    if(dYield<(Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin))
                                    {
                                        iACUseParam=1;
                                        s.sprintf("Start smart auto clean use %d parameter. ↓%2.2f%s < %2.2f%s Min Area. Yield %2.2f%s change to %2.2f%s",
                                            iACUseParam,
                                            dYield,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin,
                                            "%",
                                            dAdaptiveStardardYield,
                                            "%",
                                            dYield,
                                            "%");
                                        fCleaning->ChangeACSmartInterval(-99, s);
                                        iAdaptiveLowYieldCntNor++;              //Min Area 無條件interval 降至最低Count
                                        iAdaptiveLowYieldCntMin++;
                                    }
                                    else                                        //良率介於Normal間設定
                                    {
                                        iACUseParam=1;
                                        s.sprintf("Start smart auto clean use %d parameter. ↘%2.2f%s = %2.2f%s - %2.2f%s Normal Area. Yield %2.2f%s change to %2.2f%s",
                                            iACUseParam,
                                            dYield,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMax,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin,
                                            "%",
                                            dAdaptiveStardardYield,
                                            "%",
                                            dYield,
                                            "%");
                                        fCleaning->ChangeACSmartInterval(-1, s);                                        //Normal Area比較前次良率較低執行AI clean 2.Adaptive Interval原參數遞減 X Count
                                        iAdaptiveLowYieldCntNor++;
                                    }
                                }
                                else
                                {                                               //良率低於Min設定
                                    if(dYield<(Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin))
                                    {
                                        //iACUseParam=2;
                                        iACUseParam=1;
                                        s.sprintf("Start smart auto clean use %d parameter. ↓%2.2f%s < %2.2f%s Min Area. Yield %2.2f%s change to %2.2f%s",
                                            iACUseParam,
                                            dYield,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin,
                                            "%",
                                            dAdaptiveStardardYield,
                                            "%",
                                            dYield,
                                            "%");
                                         fCleaning->ChangeACSmartInterval(-99, s);
                                    }                                           //Min Area 無條件interval 降至最低Count
                                    else                                        //良率介於Normal間設定
                                    {
                                        iACUseParam=1;
                                        s.sprintf("Start smart auto clean use %d parameter. ↗%2.2f%s = %2.2f%s - %2.2f%s Normal Area. Yield %2.2f%s change to %2.2f%s",
                                            iACUseParam,
                                            dYield,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMax,
                                            "%",
                                            Prod.dLowYieldLimit+Prod.iAdaptiveYieldMin,
                                            "%",
                                            dAdaptiveStardardYield,
                                            "%",
                                            dYield,
                                            "%");
                                        fCleaning->ChangeACSmartInterval(1, s);                                         //Min Area 無條件interval 降至最低Count
                                    }
                                    iAdaptiveLowYieldCntNor=0;
                                    iAdaptiveLowYieldCntMin=0;
                                }
                            }

                            dAdaptiveStardardYield=dYield;                      //以當前的良率作為自適應性良率
                            fLotInfo->RefreshYieldMonitor();
                            s.sprintf("Adaptive Low Yield : %2.2f%s", dAdaptiveStardardYield, "%");
                            fLotInfo->lblAdaptiveLowYield->Caption=s;

                            if(iAdaptiveLowYieldCntNor>=Prod.iAdaptiveContsLowerAlarmNor)
                            {
                                iAdaptiveLowYieldCntNor=0;
                                iAdaptiveLowYieldCntMin=0;
                                ShowErrorMessage("WAR0729", K_RETRY, MMSystem, false);
                                return;
                            }
                            else if(iAdaptiveLowYieldCntMin>=Prod.iAdaptiveContsLowerAlarmMin)
                            {
                                iAdaptiveLowYieldCntNor=0;
                                iAdaptiveLowYieldCntMin=0;
                                 ShowErrorMessage("WAR0730", K_RETRY, MMSystem, false);
                                 return;
                            }
                            else
                            {
                                NewRecordProcess("",s,IntToStr(iAutoClean_IndexContactCount));
                                if(iACUseParam==2)
                                {
                                    Prod.iAutoClean_ContactMode =TestIF_File.iACSmart_ContactMode;
                                    Prod.iAutoCleanDropHigh     =TestIF_File.iACSmart_DropHigh;
                                    Prod.iAutoClean_ContactTime =TestIF_File.iACSmart_ContactTime;
                                    Prod.iAutoClean_ContactCount=TestIF_File.iACSmart_ContactCount;
                                }
                                else
                                {
                                    Prod.iAutoClean_ContactMode =TestIF_File.iAutoClean_ContactMode;
                                    Prod.iAutoCleanDropHigh     =TestIF_File.iAutoCleanDropHigh;
                                    Prod.iAutoClean_ContactTime =TestIF_File.iAutoClean_ContactTime;
                                    Prod.iAutoClean_ContactCount=TestIF_File.iAutoClean_ContactCount;
                                }
                            }
                        }
                    }

                    InitialAutoCleanTask();
                    InitialShuttleAutoCleanTask();
                    InitialIndexAutoCleanTask();
                    bIsAutoOneCycle=true;
                    fMain->BtnOneCycleClick(fMain);
                }
            }

            if(CosFunction.bAutoCleanTimeCT==true &&
               LastSet.iRunStartMode!=rsmAutoSiteMap)                           //jou 20250102 : auto clean triger time count
            {
                if(TestIF_File.iAutoClean_Function==true &&
                   TestIF_File.bEnabledAutoCleanTimeCT==true &&
                   iCleanOut==0 &&
                   (lAutoClean_TimeCount/1000)>=(TestIF_File.iAutoCleanTimeCT*60))
                {
                    RecordProcess("Auto Clean by time count");
                    InitialAutoCleanTask();
                    InitialShuttleAutoCleanTask();
                    InitialIndexAutoCleanTask();
                    bIsAutoOneCycle=true;
                    fMain->BtnOneCycleClick(fMain);
                    lAutoClean_TimeCount=0;
                }
            }
            else
            {
                lAutoClean_TimeCount=0;
            }
        }
    }
}
//---------------------------------------------------------------------------
void ProcessArmCount(int Index)
{
    bool bEnter_1=true;                                                         //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count
    int iBin;
    int iRow32, iCol32, iArm32;

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                if(i==2 || i==3)
                {
                    iArm32=0;
                    iRow32=i-2;
                }
                else
                {
                    iArm32=1;
                    iRow32=i;
                }
                iCol32=j;
            }
            else if(IsNNMode()==NN_1Row)
            {
                if(i==1)
                {
                    iArm32=0;
                    iRow32=0;
                }
                else if(i==0)
                {
                    iArm32=1;
                    iRow32=0;
                }
                else
                {
                    continue;
                }
                iCol32=j;
            }
            else
            {
                iArm32=Index;
                iRow32=i;
                iCol32=j;
            }

            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iBin=TestSocket.Item[i][j]-TEST_PASS;

                TestSocket.PordRec[i][j].AddOrderTestRecord(LastSet.iASEContact);                                       //Frank 20160505 add
                TestSocket.PordRec[i][j].SaveGroundESDData(RunInfo.LotNo);      //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
                fTesterTCP->CopyOSTestResult(i, j);                             //Steven 20250515 : 整合Open Short測試報表

                ArmData[iArm32]->SetArmSKTData(iRow32, iCol32, iBin);
                ArmDataLot[iArm32]->SetArmSKTData(iRow32, iCol32, iBin);        //Steven 20250603 : by lot summary
                ArmHistory[iArm32]->SetArmSKTData(iRow32, iCol32, iBin);
                ArmData[2]->SetArmSKTData(i, j, iBin);
                ArmDataLot[2]->SetArmSKTData(i, j, iBin);                       //Steven 20250603 : by lot summary
                ArmHistory[2]->SetArmSKTData(i, j, iBin);

#ifdef SOFT_SIMULTE
                if(IniConfig.bVTESTFunction==true && IniConfig.bCheckFile==true)
#else
                if(IniConfig.bVTESTFunction==true && IniConfig.bCheckFile==true && LastSet.iTester==ON_LINE)
#endif
                {
                    if(iBin<iTestBinCount && iBin>=0)                           //Steven 20260316 : 修正測試Bin數量超過設定值導致記憶體溢位
                    {
                        LastSet.iSiteBinCTForAlways[i][j][iBin]++;
                        LastSet.iSiteTotalCTForAlways[i][j]++;
                    }
                }

                if(TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM)                //ChungHung 20131223 add for SCK
                    ArmData_AutoClean[iArm32]->SetArmSKTData(iRow32, iCol32, iBin);                                     //Sam 20230104 : 修正 LowYield AutoClean //ChungHung 20131225 add

                if(CosFunction.bYieldControlUseEACount)                         //Steven 20190819 : 修正計算方式
                {
                    if(Prod.bFailAlarmSiteYieldDifferent &&
                       Prod.iFailAlarmSiteYieldDifferentCount!=0)               //Steven 20120423 : 換位置
                        fYieldMonitoring->iFailAlarmSiteYieldIntervalCount++;   //Eliot 2007_0524

                    if(Prod.bLowYieldByPicker &&
                       Prod.iLowYieldCountByPicker!=0)                          //Steven 20230223 : 根據Index吸嘴比較良率
                        fYieldMonitoring->iPickerYieldIntervalCount++;

                    if(Prod.bFailAlarmSiteYieldCmp &&
                       Prod.iFailAlarmSiteYieldCmpCount!=0)                     //jou 2014-08-14 Site Compare Low Yield alarm
                        fYieldMonitoring->iFailAlarmSiteMaxYieldIntervalCount++;

                    if(TestIF.bAutoClean_FailAlarmSiteYieldDifferent &&
                       TestIF.iAutoClean_FailAlarmSiteYieldDifferentCount!=0)
                        fYieldMonitoring->iAutoClean_FailAlarmSiteYieldIntervalCount++;
                }
            }
        }
    }

//    if(bEnter) //ChungHung 20131227 add 防止一次contact 計數多次
//    {
        if(CosFunction.bYieldControlUseEACount==false)                          //Steven 20190819 : 修正計算方式
        {
//            bEnter=false;
            if(TestIF_File.iShuttleMode==0 ||
              (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==Index))                                         //Sam 20230823 : 關Arm時不累加 contact count by Yield
            {
                if(Prod.bFailAlarmSiteYieldDifferent &&
                   Prod.iFailAlarmSiteYieldDifferentCount!=0)                   //Steven 20120423 : 換位置
                {
                    fYieldMonitoring->iFailAlarmSiteYieldIntervalCount++;       //Eliot 2007_0524
                }

                if(Prod.bFailAlarmSiteYieldCmp &&
                   Prod.iFailAlarmSiteYieldCmpCount!=0)                         //jou 2014-08-14 Site Compare Low Yield alarm
                {
                    fYieldMonitoring->iFailAlarmSiteMaxYieldIntervalCount++;
                }

                if(Prod.bLowYieldByPicker && Prod.iLowYieldCountByPicker!=0)    //Steven 20230223 : 根據Index吸嘴比較良率
                {
                    fYieldMonitoring->iPickerYieldIntervalCount++;
                }

                if(TestIF.bAutoClean_FailAlarmSiteYieldDifferent &&
                   TestIF.iAutoClean_FailAlarmSiteYieldDifferentCount!=0)
                {
                    fYieldMonitoring->iAutoClean_FailAlarmSiteYieldIntervalCount++;
                }

                if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)           //JerryYang 20160530 LowYieldLimit要能設定到小數點
                {
                    fYieldMonitoring->iLowYieldContactCount++;                  //Steven 20141212 : Yield控制使用Contact Count
                }

                if(Prod.bFailAlarmLowYieldByTotal &&
                   Prod.dLowYieldLimitByTotal!=0)                               //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count
                {
                    fYieldMonitoring->iLowYieldByTotalContactCount++;
                }
            }
        }
//    }

    if(bEnter_1==true)                                                          //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count
    {
        bEnter_1=false;
        fShowBinSelect->iLowYieldBinSelectContactCount++;                       //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count
    }
}
//---------------------------------------------------------------------------
// kevin 20190907 add load cell Measure                                         //kevin 20190907 Arm 測區次數道量測 功能;
//---------------------------------------------------------------------------
void ProcessAutoloadcellMeasureCount()
{
    if(IniConfig.bD67LoadCellMeasure && iOneCycle==0)
    {
        if(TestIF.bLoadCellMeasure)                                             //kevin 20190907 Arm 測區次數道量測 功能;
            iloadcellCount++;
        else
            iloadcellCount=0;

        if(iloadcellCount>= TestIF.iLoadCellCount)
        {
            bIsAutoOneCycle=true;
            fMain->BtnOneCycleClick(fMain);
            iloadcellCount=0;
            AsebLoadCellTest=true;
        }
    }
}
//---------------------------------------------------------------------------
void ProcessCount(int Index, bool bHasIC)  //Eastsun 20260515 F022: D7
{
    bool bContinuousContactAlarm=false;
    ArmData[Index]->SetContactCT(1);
    ArmHistory[Index]->SetContactCT(1);

    ArmData[2]->SetContactCT(1);
    ArmHistory[2]->SetContactCT(1);

    //wei 20141201 Low Yield Auto Clean
    bool bIsLowYield=false;
    AnsiString SocketErrPart="";
    AnsiString aLowYield;
    AnsiString aStandardYield;
    //wei 20141201 Low Yield Auto Clean

    if(TestIF_File.iAutoClean_Function && (TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM))                               //ChungHung 20131223 add for SCK
    {
        ArmData_AutoClean[Index]->SetContactCT(1);
        ArmData_AutoClean[2]->SetContactCT(1);
    }

    if(TestSocket.HasRealIC()==true)                                            //Steven 20200114 : 修改計算方式
    {
        ProcessHeadContactCount(Index);                                         //Ifor 20160516 京元要求銦片 Life Time 功能
        ProcessSocketCount(Index);                                              //JerryYang 20170208 (wei) socket contact計數
        LastSet.iContactCT[Index]++;                                            //jou 2012-06-05
        LastSet.SendCT[3]++;
        LastSet.iIndexCount++;                                                  //wei 20141201 Low Yield Auto Clean
        LastSet.iAutoTempOfsTriggerCnt++;                                       //Sam 20220406 : 溫度自動補償功能 By FTP
        fLotInfo->AutoTempOfsByFTP();                                           //Sam 20220406 : 溫度自動補償功能 By FTP
        if(CosFunction.IntervalYieldCount && Prod.bFailAlarmIntervalLowYieldBySite)                                     //wei 20180606 Interval Low Yield By Site
        {
            if(iYieldSiteCount[Index]>=Prod.iIntervalLowYieldCountBySite-1)
            {
                iYieldSiteCount[Index]=0;
                bYieldSiteBin[Index]=true;
            }
            else
            {
                iYieldSiteCount[Index]++;                                       //wei 20180606 Interval Low Yield By Site
            }
        }

        if(bRunAutoClean==false)                                                //Steven 20160531 : modify
            LastSet.iASEContact++;                                              //kevin 20141020 ASE 記錄此批一CONTACT 次數

        if(EmptySocketCheckModeBeUse() &&
           IniConfig.bI41_5_RegularExecutionCycle)                              //Steven 20201022 : For RFMD Empty Socket Check Funstion.
        {
            iESC_IndexContactCount++;
            if(IniConfig.iI41_5_RegularExecutionCycleCount!=0 &&
               iESC_IndexContactCount>=IniConfig.iI41_5_RegularExecutionCycleCount)
            {
                fMain->ResetForESC("Start Empty Socket OneCycle by Index Contact Count");
            }
        }
        else
        {
            iESC_IndexContactCount=0;
        }
    }

    //jou 2016-04-28 Socket Air Cooling contact count trun on
    if(IniConfig.bL03SocketAirCoolingCT==true)
    {
        iL03SocketAirCoolingCT++;                                               //jou 2016-04-28 Socket Air Cooling contact count trun on
        if(iL03SocketAirCoolingCT>IniConfig.iL03SocketAirCoolingCT)
        {
            SW[SwTesterAirCooling].On();
        }
    }

    //Steven 20110420
    if(iRunStartMode==FT)
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
            LastSet.SendCT[3]=0;
            if(USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest))                          //kevin 20150615   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                LastSet.SendCT_ART[3]=0;
        }
        else
        {
            if(TestIF.bContinuousContact==true)
            {
                if(LastSet.SendCT[3]>=int(TestIF.iContinuousContactCount))
                {
                    LastSet.SendCT[3]=0;                                        //20141001 wei add
                    if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount)                        //kevin 20150615  //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                        LastSet.SendCT_ART[3]=0;

                    iWhoTriggerPiggyBack=pbtContinualContactCount;              //Steven 20111207 : 誰觸發了Piggy Back
                    if(CosFunction.bPiggyBackShowMainForm && Prod.iCountAlarmAction==0)                                 //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                    {
                        bContactCounOven=true;                                  //kevin 20131009 CONTACT COUNT 超過多設定次數秀警告
                    }
                    else if(Prod.iCountAlarmAction==0)                          //20141001 wei add
                    {
                        bContinuousContactAlarm=true;
                    }
                    else
                    {
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }

        if(IniConfig.bE53LowYieldAutoClean)                                     //kevin 20160802   //wei 20141201 Low Yield Auto Clean(%) start
        {
            //參考值設定後,每次進行Site to Site比較時間點為產生參考值之前n個Cycle數之一半
            //If n = 30 Cycles, 每 15 cycles 比較一次
            //If n = 20 Cycles, 每 10 cycles 比較一次
            //P.S:Arm1 & Arm2 各contact一次 = 2 cycles.
            if(Prod.bFailAlarmLowYield_AutoClean && Prod.iLowYieldLimit_AutoClean!=0 && bLowYieldAutoCleanEnd==false)   //wei 20141216  LowYieldautoclean  避免重複進去
            {
                if(LastSet.iIndexCount>=Prod.iLowYieldCount_AutoClean/2 && bStandardYield==true)
                {
                    LastSet.iIndexCount=0;

                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            iYield[i][j]=fContactCT->ReturnSiteDataArray(false, i, j);
                            iDifferenceYield[i][j]=iStandardYield[i][j]-iYield[i][j];

                            //If 取樣點 Yield < 參考點 Yield 10% , 則啟動Auto Clean Function
                            //If 取樣點 Yield < 參考點 Yield 20% , 則Close Site (Close Site 之前至少要做過一次Auto Clean)
                            //Close Site數不得超過總Site數1/4,否則須Alarm停機
                            if(iDifferenceYield[i][j]>0 && abs(iDifferenceYield[i][j])>Prod.iLowYieldLimit_AutoClean && LastSet.bUseTestSocket[0][i][j]==true)
                            {
                                bIsLowYield=true;
                                iLowYieldSiteCount[i][j]++;

                                if(abs(iDifferenceYield[i][j])>Prod.iLowYieldLimit_AutoClean*2 && iLowYieldSiteCount[i][j]>1)
                                {
                                    bLowYieldCleanOut=true;
                                    bLowYieldCloseSite[0][i][j]=true;
                                    SocketErrPart+=IndexSuckName[i][j]+ " : " + abs(iDifferenceYield[i][j]) + " % Close ,";                                     //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                                }
                                else
                                {
                                    SocketErrPart+=IndexSuckName[i][j]+ " : " + abs(iDifferenceYield[i][j]) + "% ,";    //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                                }
                            }
                        }
                    }
                }
                else if(LastSet.iIndexCount>=Prod.iLowYieldCount_AutoClean && bStandardYield==false)
                {
                    LastSet.iIndexCount=0;
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            iStandardYield[i][j]=fContactCT->ReturnSiteDataArray(false, i, j);
                            SocketErrPart += IndexSuckName[i][j] + " : " + iStandardYield[i][j] + " % ,";               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            bStandardYield=true;
                        }
                        aStandardYield = "Low Yield Start Standard Yield : " + AnsiString(SocketErrPart);
                        RecordProcess(aStandardYield);
                        SocketErrPart="";
                    }
                }
            }

            if(bIsLowYield==true)
            {
                if(bLowYieldCleanOut)
                {
                    aLowYield="Low Yield Close Site : "+AnsiString(SocketErrPart);
                }
                else
                {
                    aLowYield="Low Yield Site : "+AnsiString(SocketErrPart);
                }

                RecordProcess(aLowYield);
                bLowYieldAutoCleanEnd=true;                                     //wei 20141216  LowYieldautoclean  避免重複進去

                if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && Prod.bFailAlarmLowYield_AutoClean)
                {
                    InitialAutoCleanAllTask();                                  //Sam 20230504 : 整理 InitialAutoCleanTask
                }
            }
        }                                                                       //wei 20141201 Low Yield Auto Clean(%) end
    }
    else if(iRunStartMode==RT)
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
            LastSet.SendCT[3]=0;
            if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount)                                //kevin 20150615 //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                LastSet.SendCT_ART[3]=0;
        }
        else
        {
            if(TestIF.bContinuousContact_RT==true)
            {
                if(LastSet.SendCT[3]>=int(TestIF.iContinuousContactCount_RT))
                {
                    LastSet.SendCT[3]=0;
                    if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount)                        //kevin 20150615    //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                       LastSet.SendCT_ART[3]=0;

                    iWhoTriggerPiggyBack=pbtContinualContactCount;              //Steven 20111207 : 誰觸發了Piggy Back
                    if(CosFunction.bPiggyBackShowMainForm &&
                       Prod.iCountAlarmAction==0)                               //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                    {
                        bContactCounOven=true;                                  //kevin 20131009 CONTACT COUNT 超過多設定次數秀警告
                    }
                    else if(Prod.iCountAlarmAction==0)                          //20141001 wei add
                    {
                        bContinuousContactAlarm=true;
                    }
                    else
                    {
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }
    }

    if(bContinuousContactAlarm)                                                 //20141001 wei add
    {
        ShowErrorMessage("WAR07325", 0, MMInterface, false);
    }

    ProcessArmCount(Index);

    if((TestIF_File.iShuttleMode==0 ||
       (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==Index)) &&     //Ifor 20220419 add:Auto Clean
       bHasIC)                                                                   //Eastsun 20260515 F022: D7 fix AutoClean count
    {
        ProcessAutoCleanCount();
    }
    ProcessSocketPurgeCount();
    ProcessShowTestStatus(Index);
    CheckContinuoussFail(Index);
    CheckContinuoussPass(Index);                                                //Eliot 20100709
    CheckContinuoussPassBySocket(Index);                                        //Steven 20110919
    ProcessContinuoussFailForATC(Index);                                        //Steven 20151123 : Continue Fail Temp Offset for ATC
    fYieldMonitoring->CalculateSiteYield();                                     //Steven 20170905 (wei) : Yield Alarm全部移動去fYieldMonitoring
    fContactCT->sgYield->Refresh();
    ProcessAutoloadcellMeasureCount();                                          //kevin 20190907 add load cell Measure
}
//---------------------------------------------------------------------------
void ProcessStartTestData(int iArm)
{
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
            {
                if(TestSocket.Item[i][j]<TEST_PASS && TestSocket.Item[i][j]!=HAS_NULL_IC)
                    fTestCategory->SetTestingCateCell(iArm, i, j, clYellow);
            }
            else
            {
                fTestCategory->SetTestCateCellAS(iArm, i, j, "-1", clWhite);    //kevin 20150119 送給測試機 沒有IC清空
            }
        }
    }

    fTestCategory->ShowTestCategory(iArm);                                      //jou 981017 level down display loading
}
//---------------------------------------------------------------------------
void __fastcall RecordHistroy(int attr)
{
    TColor CellColor;
    int iHistroyKCnt=(CUSTOMER_CODE==CC_KYEC_LEE)?21:9;                         //Eastsun 20260526 #026-1.9 Ifor 20221026 add:KYEC 要求增加比數9 => 21
    if(IsNNMode()==NN_1Row)                                                     //Steven 20231023 : fixed for NN mode
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            for(int k=iHistroyKCnt; k>=1; k--)                                  //Eastsun 20260526 #026-1.9
            {
                HistroyBin[0][j][k]=HistroyBin[0][j][k-1];
                HistroyPassFail[0][j][k]=HistroyPassFail[0][j][k-1];
                HistroyBin[1][j][k]=HistroyBin[1][j][k-1];
                HistroyPassFail[1][j][k]=HistroyPassFail[1][j][k-1];
            }
            HistroyBin[0][j][0]=fTestCategory->GetTestResult(attr, 0, j, &CellColor);
            HistroyPassFail[0][j][0]=(CellColor==clGreen)?true:false;

            HistroyBin[1][j][0]=fTestCategory->GetTestResult(attr, 1, j, &CellColor);
            HistroyPassFail[1][j][0]=(CellColor==clGreen)?true:false;
        }
    }
    else if(IsNNMode()==NN_2Row)
    {
        for(int i=0; i<FTestSuck.iShtRow; i++)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                for(int k=iHistroyKCnt; k>=1; k--)                              //Eastsun 20260526 #026-1.10
                {
                    HistroyBin[i][j][k]=HistroyBin[i][j][k-1];
                    HistroyPassFail[i][j][k]=HistroyPassFail[i][j][k-1];
                    HistroyBin[i+2][j][k]=HistroyBin[i+2][j][k-1];
                    HistroyPassFail[i+2][j][k]=HistroyPassFail[i+2][j][k-1];
                }
                HistroyBin[i][j][0]=fTestCategory->GetTestResult(attr, i, j, &CellColor);
                HistroyPassFail[i][j][0]=(CellColor==clGreen)?true:false;

                HistroyBin[i+2][j][0]=fTestCategory->GetTestResult(attr, i+2, j, &CellColor);
                HistroyPassFail[i+2][j][0]=(CellColor==clGreen)?true:false;
            }
        }
    }
    else
    {
        for(int i=0; i<FTestSuck.iShtRow; i++)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                for(int k=iHistroyKCnt; k>=1; k--)                              //Eastsun 20260526 #026-1.11
                {
                    HistroyBin[i][j][k]=HistroyBin[i][j][k-1];
                    HistroyPassFail[i][j][k]=HistroyPassFail[i][j][k-1];
                }
                HistroyBin[i][j][0]=fTestCategory->GetTestResult(attr, i, j, &CellColor);
                HistroyPassFail[i][j][0]=(CellColor==clGreen)?true:false;
            }
        }
    }
    fObserver->UpdateBin();

    //==>>Isaac 20170809 (wei) TCPIP Reply test result
    if(CosFunction.bEnableHandlerResultServer)
    {
        String sMessage="HTSR,501,";
        sMessage+=fObserver->TimeInfoGrid->Cells[3][11];                        //test time
        sMessage+=",";
        sMessage+=fObserver->TimeInfoGrid->Cells[5][11];                        //index time
        sMessage+=",";
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(HistroyBin[i][j][0]>0)
                {
                    sMessage+=IntToStr(HistroyBin[i][j][0])+",";                //20150130, Nickliu,TCP Reply Result
                }
                else
                {
                    sMessage+="-1," ;
                }
            }
        }

        fMain->HandlerTCPIPResultSendProcess(sMessage);
        fMain->HandlerTeraTResultSendProcess(sMessage);                         //Sam 20230421 : 整理 TCP Log
    }
    //<<==Isaac 20170809 (wei) TCPIP Reply test result
}
//---------------------------------------------------------------------------
void ProcessQASampling(int Index)                                               //Steven 20190326 : QA Sampling
{
    int iPercent;
    int iCount=0;
    int iSiteCount=0;
    if(TestIF_File.bEnableQASampling && TestIF_File.bSCKART_EnableART && fSCKART->iLotCount!=0)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSiteUse[Index][i][j])
                    iSiteCount++;
            }
        }

        if(iSiteCount!=0)
        {
            iPercent=ceil(double(fSCKART->iLotCount)/double(iSiteCount));       //Steven 20250820 : 針對除以0加上保護
        }
        else
        {
            iPercent=0;
        }

        if(iPercent>TestIF_File.iQASamplingCnt ||
           iPercent==0)
        {
            iPercent=1;
        }
        else
        {
            iPercent=ceil(double(TestIF_File.iQASamplingCnt)/double(iPercent));
        }

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(iCount<iPercent &&
                   bQASampleCnt<TestIF_File.iQASamplingCnt &&
                   TestSocket.Item[i][j]-TEST_PASS==TestIF_File.iQASamplingBin)
                {
                    TestSocket.bQATray[i][j]=true;
                    iCount++;
                    bQASampleCnt++;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
