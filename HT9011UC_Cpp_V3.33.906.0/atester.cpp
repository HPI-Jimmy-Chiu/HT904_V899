//---------------------------------------------------------------------------
//  atester.cpp  --  TESTER / TEST-HEAD (Index down-press) ENGINE (W6.4 translation)
//
//  Translation wave: W6.4 (TESTER/INDEX ENGINE)
//  Translator: AI(W6.4-TESTER) 20260626
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/atester.cpp (11819 lines, cp950)
//
//  WHAT THIS FILE IS
//  -----------------
//  The tester/test-head engine: it drives the Index down-press test-head Y/Z
//  motors (MTestY1/MTestZ1/MTestY2/MTestZ2), the FTestSuck/BTestSuck nozzles,
//  the Socket test handshake, and owns iTestHeadMotorTask + iTestYTask.
//
//  W6.4 SCOPE (mirrors the W6.2/W6.3 arm + tray engines)
//  -----------------------------------------------------
//  ACTIVE, faithfully transcribed (cursor values + formulas + fall-throughs
//  VERBATIM, pumpable over Sim HAL):
//    * geometry / numeric anchors: GetRowCol (:144), GetRTCSiteMap (:423),
//      ArrayConvertSite (:504), GetOneByOneData (:621), GetBinaryData (:670),
//      Test_GetTestResulSub (:759), Test_GetTestResul (:789),
//      GetSocketCheckPos (:9791), GetIndexZSpeed (:9814).
//    * the test-cycle dispatcher SM DoTestY (:4789, switch(iTestYTask), oracle
//      path 1 -> 50 -> 100 -> 110 fall-through).
//    * the central index/test SM DoTestHeadMotor (:5562, switch(iTestHeadMotor
//      Task), oracle path 4 -> 9 -> 10 -> 15 over Sim HAL; entry preamble made
//      offline-safe; the dense per-case down-press trees (case 30..600000,
//      12000.., 14000.., load-cell) are GATED, the oracle-window cases reproduced
//      verbatim).
//    * the EP-check SM IndexEveryTimeCheckEP (:9158, case 1 early-out).
//    * the small Mode/Init SMs DoStartMode/DoEndMode/InitTest*/Init*Mode and the
//      decode consumers Check_TTL_Status (:2564) / CheckTestSocketIsError (:2584).
//
//  GATED (#if 0 // TODO(...)) with an ACTIVE compiling stub returning the golden
//  default, OR routed to atester_shims.cpp, so the file LINKS + the core SMs run:
//    * GetTesterResult        (:849  -- ~1700-line tester-comm/GPIB/TCP + MOT[]/
//                              Suck/Socket result-acquisition SM).        W5/W7.
//    * ProcessTestResult      (:2621 -- bin-decode + DB + UI).                 W7.
//    * ProcessTesterTimeOut   (:3594).                                         W7.
//    * DoCheckSocketHasIC     (:4305 -- MOT[] vacuum-check SM).                W7.
//    * DoTestHeadMotorLoadCell(:10444).                                        W7.
//    * the per-case heavy bodies of DoTestY / DoTestHeadMotor that deref
//      MOT[]/Suck/Socket/EP/tester-comm beyond the oracle window.             W7.
//    * cContact / iIndexTask (the index-press SM is in cContact.cpp, a VCL FORM,
//      22761 lines -- NOT in scope; never derefed from atester.cpp).          W7.
//    * atester_32Site.cpp / atester_ProcessCount.cpp ->
//      routed to atester_shims (Do* report COMPLETE, Init* no-op).       W6.4b.
//    * aTester_Front.cpp / aTester_Rear.cpp (Front/Rear "Destroy IC" SM pair) --
//      AI(W64b-Integrate) 20260706: now translated for real (no longer an
//      atester_shims stub); every OTHER Front/Rear export (DoFrontTestSuckIC,
//      DoTestYFront, TestZ1OutRandge2, ...) still routes through atester_shims.
//    * the contact-mode form fContact + the ADAM_* EP DAQ API -> atester_shims.
//
//  TRANSLATION RULES
//  -----------------
//    * Off Borland: __fastcall removed; AnsiString/TQPF_Timer/TStringList via
//      vclcompat; numeric semantics EXACT (integer division kept -- esp.
//      GetIndexZSpeed *iScale/100 stays INTEGER division per the divide-safety
//      regression note).
//    * SOFT_SIMULTE NOT defined -> #ifndef/#ifdef SOFT_SIMULTE blocks that fall
//      inside ACTIVE functions are reproduced VERBATIM (the #ifndef bodies are
//      live: Test_GetTestResulSub Sen read; IndexEveryTimeCheckEP #else tree).
//      Blocks inside GATED SM bodies are gated away with those bodies (same as
//      the W6.2 arm engines -- a gated SM does not reproduce its inner directives).
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8.  ZERO U+FFFD.
//---------------------------------------------------------------------------
#include "MachineDefine.h"

#include "atester.h"
#include "atester_shims.h"          // fContact/ADAM_*/CCDInterfaceForm/fAutomation/fObserver/fiosetview/COM2 offline shims (32Site/ProcessCount/Front/Rear now real, see below)
// AI(W64b-Integrate) 20260706: mirrors golden atester.cpp:7-8, which #include
// "aTester_Front.h"/"aTester_Rear.h" directly right after atester.h -- the
// Front/Rear "Destroy IC" state machines are now translated for real (no
// longer routed through atester_shims' offline stubs for these 6 symbols).
#include "aTester_Front.h"
#include "aTester_Rear.h"
// AI(W5-Automation-Integrate) 20260710: atester_32Site.cpp / atester_ProcessCount.cpp
// are now real translations (added to ht9045_sm) -- include their own headers
// directly instead of relying on atester_shims.h's (now-removed) declarations.
#include "atester_32Site.h"
#include "atester_ProcessCount.h"
#include "aArmHeader.h"             // __FUNC__ shim

#include "MachineType.h"
#include "Motor/mymotor.h"          // MOT[], TTrayMotor
#include "mysensor.h"               // Sen[]
#include "myswitch.h"               // AI(W7T1-Integrate) 20260701: SW[] (SwIndexChangeToque1/2 .Off() in case 12000/etc.) -- golden global; un-gated down-press tree needs it
#include "vclcompat/vcl_compat.h"   // AI(W7T1-Integrate) 20260701: AnsiString(int) ctor (labUser Caption from iUseSocketHeating_Time)
#include "csystem.h"                // state predicates (TestHeadHasIC / InputShuttleHasIC / ...)
#include "cprod.h"                  // Prod / TestIF / TestIF_File / CosFunction / DeviceForm / SThreadPara
#include "cmydef.h"                 // global scalar universe + IC consts + enums
#include "cpublic.h"                // COM2
#include "common.h"
#include "aHotPlateSubstrate.h"     // FTestSuck/BTestSuck/FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit/TestSocket/InArmSuck
#include "FormsFacade.h"            // fMain offline stand-in
#include "canary_support.h"         // LastSet / ShowMyMessage / ShowErrorMessage / RecordProcess
//---------------------------------------------------------------------------

const int CCDTimeOutSec=10;                                                     // golden :67
const int TESTZ1UP=0;                                                           // golden :68
const int TESTZ2UP=1;                                                           // golden :69
const int MaxDIO=40;                                                            // golden :70
const int CheckFailDownCT=1;                                                    //kevin 20131121   10 來不及偵測掉料  -- golden :71

FILE *EPOut;                                                                    //Ifor 20150707  -- golden :80

DWORD dwEndShuttle1Soak;                                                        //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
DWORD dwStartShuttle1Soak;
DWORD dwEndShuttle2Soak;
DWORD dwStartShuttle2Soak;

TQPF_Timer NULL_Delay;
TQPF_Timer DutDelay;                                                            //Steven 20180808 (wei) : TTL的時間單位改成microsecond(μs)
TQPF_Timer SoftContactTim;                                                      //kevin 20130608 soft 模式 需等ep 穩定
TQPF_Timer TSDDelay;
TQPF_Timer DoGiveWayDelay;                                                      //Ifor 20190723 : add
TQPF_Timer Noise_Delay;
TQPF_Timer HangTime;                                                            //Steven 20090827 : Hang Up dectector
TQPF_Timer hIndexSoakTime;                                                      //2013-11-27   Dell    需要做Index soak time
TQPF_Timer HAfterTestedDelay;                                                   //ChungHung 20140730 add for ATK function after tested delay time
TQPF_Timer DropContactTimer1;                                                   //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
TQPF_Timer DropContactTimer2;
TQPF_Timer DropContactTimer3;
TQPF_Timer dwStartInitialCount;
TQPF_Timer dwEndInitialCount;                                                   //kevin 20180926 add
TQPF_Timer hStartModeWaitTime;
TQPF_Timer DoTestHeadMotorDelay;
TQPF_Timer DoTestHeadMotorDelay2;
// AI(W7T1-Integrate) 20260701: RTC-CCD index-check entry case ids restored to their golden
// file-scope home (golden atester.cpp:5551-5555).  W6.4 had temporarily relocated iCASE_REAL_CCD2
// into atester_shims.cpp as =9 (valid only WHILE the RTC case tree was gated); now the tree is
// ACTIVE, so `case iCASE_REAL_CCD2:` (=9) would collide with the active `case 9:`.  These `const int`
// have internal linkage (only atester.cpp references them, verified tree-wide), so no ODR export.
const int iCASE_REAL_CCD2=40200;                                                // golden atester.cpp:5551
const int iCASE_REAL_CCD3=40300;                                                // golden atester.cpp:5552
const int iCASE_REAL_CCD4=40400;                                                // golden atester.cpp:5553
const int iCASE_REAL_CCD5=40500;                                                // golden atester.cpp:5554
const int iCASE_REAL_CCD6=40510;                                                // golden atester.cpp:5555
TQPF_Timer TestTimeOut;
TQPF_Timer HTestDeley;

int iTestYTask=1;                                                               // golden cursor (InitTestYTask :4784)
int iSetupTask=1;                                                               // golden cursor (DoSetupTest)
int iDoInterFaceErrorStepTask=1;                                               // golden cursor (DoInterFaceErrorStep)
int iCheckSocketHasIC=1;                                                        // golden cursor (InitCheckSocketHasIC :4298)
int iIndexEveryTimeCheckEPTask=1;                                              // golden cursor (InitIndexEveryTimeCheckEP :9151)
int iIndexYAxisServoOnStateTas=0;
int iIndexYAxisServoOnStateTask=0;
int iDoIndexSocketCheckTask=1;
int iTempICTask=1;
bool bTestDuplicateErr=false;

int iStartModeEvent1=0;
int iStartModeEvent2=1;
int iStartModeEvent3=2;
int iDoEndMode=1;
int iDoStartMode=1;
int iCCDTimeOutCount=0;
int iStartStep=0;
int iWhichAxisTestIC=0;
int iSendSMITimeOutLimit=1;
int iSocketSenSosPos1=0, iSocketSenSosPos2=0;                                   //kevin 20150613 關arm 設定可判斷位置
int iReContactCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL];                              //Steven 20231205 : 計算某site contact 次數
int iTesterBIN[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1}};

bool GetIndexTime_flag=false;
bool ScanPort[MaxDIO];
bool bFTestSuckDrop=false;
bool bBTestSuckDrop=false;
bool bZ1NeedTest=false, bZ2NeedTest=false;
bool bDoubleContact=false;
bool bNeedUpDonwOneTome=false;
bool fRearCheckSuckICPass=false;
bool bIndexPickUpErrMoveSht1=false;                                             //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
bool bIndexPickUpErrMoveSht2=false;                                             //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
bool bIndexWaitingInArmAway=true;                                               //Steven 20171228 (Wei) : Index在等In Arm讓開
bool bNeedCheckRTCReport=false;                                                 //Ifor 20190723 : add
bool SetNoiseDelay=false;
bool bHangTimePause=false;                                                      //Steven 20090827 : Hang Up dectector
bool bInitialSackTime=false;                                                    //kevin 20131112 第一次吸取ic等待時間
bool bFirstZ1UPZ2Down=true;                                                     //kevin 20131112 加熱時z1在shuttle 1上面 z2在下
bool bNeedIndexSoakTime=false;                                                  //2013-11-27   Dell    需要做Index soak time
bool bATCSiteTest[32]={false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false};                                         //Ifor 20160510 add ATC Test Site

//==============================================================================
int GetRowCol(int &iRow, int &iCol)
{
    int iSiteMapRTC=-1;

    if(TestIF_File.iTestMode==DualSite) //1x2
    {
        iRow=1;
        iCol=2;
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==SingleSite) //1x1
    {
        iRow=1;
        iCol=1;
        iSiteMapRTC=0;
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        iRow=1;
        iCol=3;
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==_8Site1X4)
    {
        iRow=1;
        iCol=4;
        iSiteMapRTC=2;
    }
    else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N) //2x2
    {
        iRow=2;
        iCol=2;
        iSiteMapRTC=3;
    }
    else if(TestIF_File.iTestMode==DualSite2x1) //2x1
    {
        iRow=2;
        iCol=1;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||
            TestIF_File.iTestMode==_6Site2X3N)
    {
        iRow=2;
        iCol=3;
        iSiteMapRTC=7;
    }
    else if(TestIF_File.iTestMode==_8Site2X4) //2x4
    {
        iRow=2;
        iCol=4;
        iSiteMapRTC=4;
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        iRow=4;
        iCol=4;
    }
    else if(TestIF_File.iTestMode==_10Site2X5) //2x5
    {
        iRow=2;
        iCol=5;
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_12Site2X6) //2x6
    {
        iRow=2;
        iCol=6;
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //2x8
    {
        iRow=2;
        iCol=8;
        iSiteMapRTC=6;
    }
    else if(TestIF_File.iTestMode==_32Site4X8M || TestIF_File.iTestMode==_32Site4X8N) //4x8
    {
        iRow=4;
        iCol=8;
    }
    return iSiteMapRTC;
}
//==============================================================================
void InitDoStartMode()
{
    iDoStartMode=1;
}
//==============================================================================
void InitDoEndMode()
{
    iDoEndMode=1;
}
//------------------------------------------------------------------------------
bool IndexAlarmInArmAway()                                                      //Steven 20130613 : Index異常時, In Arm要先讓位功能
{
    bool ret=true;
    if(IniConfig.bIndexJamInArmAway==true)                                      //Steven 20110607 : Index Jam, In Arm要移開
    {
        bIndexWaitingInArmAway=true;                                            //Steven 20171228 (Wei) : : Index在等In Arm讓開

        if(CheckInArmFinishAllPickerAction()==false)                            //Steven 20171226 (Wei) : 修改in arm讓開的flag
        {                                                                       //kevin 20131119 Z軸正在吸取不能被中斷
            return false;
        }

        bInArmNeedToSafePos=true;                                               //Steven 20130819
        InitInArmTask();

        if(MoveInArm2XYToWait()==false)                                         //如果要移開,而且還沒移到定位
        {
            ret=false;                                                          //先離開等In Arm
        }
        else
        {
            ret=true;
            bIndexWaitingInArmAway=false;                                       //Steven 20171228 (Wei) : : Index在等In Arm讓開
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
bool DoStartMode(int mode)
{
    int &Task=iDoStartMode;
    if(mode==0)                                                                 //Mode 0 return true
    {
        return true;
    }
    else if(mode==1)                                                            //Mode 1 向host send "PRODUCTION_REQUEST 0003" return true;
    {
        fAutomation->DoCommandBuffer("PRODUCTION_REQUEST", "", "0003", 0, "");
        return true;
    }
    else if(mode==2)                                                            //Mode 2
    {
        switch(Task)
        {
            case 1:                                                                                                     //向host send "PRODUCTION_REQUEST 0003"
                fAutomation->ClearEvent(iStartModeEvent1);                                                              //釋放資源
                fAutomation->GetEventNum(iStartModeEvent1, "PRODUCTION_REPLY",   "0003");                               //取得編號 設定條件字串
                fAutomation->ClearEvent(iStartModeEvent2);                                                              //釋放資源
                fAutomation->GetEventNum(iStartModeEvent2, "PRODUCTION_REQUEST", "0007");                               //取得編號 設定條件字串
                fAutomation->ClearEvent(iStartModeEvent3);                                                              //釋放資源
                fAutomation->GetEventNum(iStartModeEvent3, "PRODUCTION_REQUEST", "0008");                               //取得編號 設定條件字串
                fAutomation->DoCommandBuffer("PRODUCTION_REQUEST", "","0003", 0, "");
                hStartModeWaitTime.SetSecAndOn(10);                                                                     //設定計數
                Task=50;
            case 50:                                                            //等待Host回覆"PRODUCTION_REPLY 0003"
                if(fAutomation->GetEventResult(iStartModeEvent1)==true)         //Host回覆"PRODUCTION_REPLY 0003"
                {
                    fAutomation->ClearEvent(iStartModeEvent1);                  //釋放資源
                    hStartModeWaitTime.SetSecAndOn(10);
                    Task=100;
                }
                else                                                            //Host還未回覆"PRODUCTION_REPLY 0003"
                {
                    if(hStartModeWaitTime.Off())
                    {
                        Task=1;
                        fAutomation->ClearEvent(iStartModeEvent1);              //釋放資源
                        return true;
                    }
                    break;
                }
            case 100:                                                           //Host回應"PRODUCTION_REQUEST 0007" OK or "PRODUCTION_REQUEST 0008" NG
                if(iStartModeEvent1==-1)                                        //Sam 20190429 : Add CC_PTI_NEWWORK
                {
                   iStartModeEvent1=0;
                   Task=1;
                   break;
                }

                if(fAutomation->GetEventResult(iStartModeEvent2)==true)         //Host 有回應
                {
                    fAutomation->ClearEvent(iStartModeEvent2);                  //Sam 20190429 : Add CC_PTI_NEWWORK
                    fAutomation->DoCommandBuffer("PRODUCTION_REPLY", "","0007", 0, "");
                    bStartModeComplete=true;                                    //Sam 20190429 : Add CC_PTI_NEWWORK
                    Task=1;                                                     //Sam 20190429 : Add CC_PTI_NEWWORK
                    return true;
                }
                else if(fAutomation->GetEventResult(iStartModeEvent3)==true)    //NG
                {
                    fAutomation->ClearEvent(iStartModeEvent3);                  //Sam 20190429 : Add CC_PTI_NEWWORK
                    fAutomation->DoCommandBuffer("PRODUCTION_REPLY", "","0008", 0, "");
                    Task=1;                                                     //Sam 20190429 : Add CC_PTI_NEWWORK
                }
                else                                                            //Host 還未回覆
                {
                    if(hStartModeWaitTime.Off())                                //逾時 ShowAlarm Message "Wait PRODUCTION_REPLY 0007 Over Time" Retry or Skip
                    {
                        Task=1;                                                 //Sam 20190429 : Add CC_PTI_NEWWORK
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer hEndModeWaitTime;
int iEndModeEvent1=0;
bool DoEndMode(int mode)
{
    int &Task=iDoEndMode;

    if(bStartModeComplete==false)
    {
        return true;
    }

    if(mode==0)                                                                 //Mode 0 return true
    {
        return true;
    }

    if(mode==1)                                                                 //Mode 1 向host send "TEST_RESULT_REQUEST" return true;
    {
        fAutomation->DoCommandBuffer("TEST_RESULT_REQUEST", "", "", 0, "");
        return true;
    }

    switch(Task)                                                                //Mode 2
    {
        case 1:                                                                 //向host send "TEST_RESULT_REQUEST"
            fAutomation->DoCommandBuffer("TEST_RESULT_REQUEST", "", "", 0, "");
            hEndModeWaitTime.SetSecAndOn(10);                                   //設定計數
            fAutomation->GetEventNum(iEndModeEvent1, "TEST_RESULT_REPLY", "");  //取得編號 設定條件字串
            Task=50;
        case 50:                                                                //等待Host回覆"TEST_RESULT_REPLY"
            if(fAutomation->GetEventStrResult(iEndModeEvent1)==3)               //Host回覆"PRODUCTION_REPLY 0003"
            {
                fAutomation->ClearEvent(iEndModeEvent1);                        //釋放資源
                bStartModeComplete=false;
                Task=1;
                return true;
            }
            else                                                                //Host還未回覆"TEST_RESULT_REPLY"
            {
                if(hEndModeWaitTime.Off())
                {
                    fAutomation->ClearEvent(iEndModeEvent1);
                    Task=1;
                    return true;
                }
                break;
            }
    }
    return false;
}
//==============================================================================
bool ScanCCDProgram()
{
    HWND HCCDWnd=FindWindow(NULL,"Identification");
    if(HCCDWnd==NULL)
    {
        CCDInterfaceForm->bAtestScanCCDProgram=false;                           //kevin 20110811
        return false;
    }
    else
    {
        CCDInterfaceForm->bAtestScanCCDProgram=true;                            //kevin 20110811
        return true;
    }
}
//==============================================================================
const int OverEncoderDelay=50;

int iGetTestDataDelayTask=1;
int iRecordOldBin[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1}};

//==============================================================================
int GetRTCSiteMap()
{
//                    ========================================================
//                    第5碼        Site 分佈樣式
//                    ========================================================
//                    asSiteMap    0         1 x 1
//                                 1         1 x 2
//                                 2         1 x 4
//                                 3         2 x 2
//                                 4         2 x 4
//                                 5         2 x 6
//                                 6         2 x 8
//                    ========================================================
    int iSiteMapRTC=-1;

    if(TestIF_File.iTestMode==SingleSite)                                       //1x1
    {
        iSiteMapRTC=0;
    }
    else if(TestIF_File.iTestMode==DualSite)                                    //1x2
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==TriSite1X3)                                  //Frank 20160329 add for 1x3_4
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||
            TestIF_File.iTestMode==_8Site1X4)                                   //ChungHung 20150528 add for 海思 _8Site1x4 //1x4
    {
        iSiteMapRTC=2;
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 //2x1
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite2X2 ||                               //2x2
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mod
    {
        iSiteMapRTC=3;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||                                 //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        iSiteMapRTC=7;
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||                                 //2x4
            TestIF_File.iTestMode==_8Site2X4N ||                                //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_16Site4X4)                                  //Sam 20190226 : 16Site4X4
    {
        iSiteMapRTC=4;
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //2x5  //wei 20190614 10 site
    {
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //2x6
    {
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //2x8  //Eliot 2009_12_25
    {
        iSiteMapRTC=6;
    }
    else if(TestIF_File.iTestMode==_32Site4X8M ||                               //4x8
            TestIF_File.iTestMode==_32Site4X8N)                                 //ChungHung 20130627 alter TestIF--->TestIF_File 修正無法跑32Site
    {
        iSiteMapRTC=6;
    }
    return iSiteMapRTC;
}
//==============================================================================
//******************************************************************************
//
//  注意!! ArrayConvertSite內容影響Site Mapping與開關Site, 修改時要小心驗證
//
//******************************************************************************
int ArrayConvertSite(int X, int Y)
{
    int Site=-1;

    if(TestIF.iTestMode>=_6Site2X3)                                             //Alick 20161011 (Steven) : TTL支援8Site
    {
        Site=X*4+Y+1;
    }
    else if(TestIF.iTestMode>=DualSite2x1)
    {
        Site=X*2+Y+1;
    }
    else
    {
        Site=Y+1;
    }
    return Site-1;
}
//==============================================================================
//  CheckBackError (golden :523) -- read 4 index axes vs TestZ1_Safe window.
//  ACTIVE: offline Gali_ReadPos preserves Position (deterministic 0 baseline).
//==============================================================================
bool CheckBackError()
{
    int ArmCmdPos[4], i;
    ArmCmdPos[0]=MOT[MTestY1].Gali_ReadPos();
    ArmCmdPos[1]=MOT[MTestZ1].Gali_ReadPos();
    ArmCmdPos[2]=MOT[MTestZ2].Gali_ReadPos();
    ArmCmdPos[3]=MOT[MTestY2].Gali_ReadPos();
    for(i=0; i<4; i++)
    {
        if(ArmCmdPos[i]>=Prod.TestZ1_Safe+30 ||
           ArmCmdPos[i]<=Prod.TestZ1_Safe-30)
            return true;
    }
    return false;
}
//==============================================================================
//  TestArmBackPos (golden :539) -- check Z1/Z2 at home, then GalilTwoY_Move back.
//  The #ifndef SOFT_SIMULTE Z-position block is reproduced VERBATIM (SOFT_SIMULTE
//  not defined -> live).  Offline Gali_ReadPos returns 0; if TestZ*_Safe is ~0 the
//  error branch is skipped and the GalilTwoY_Move reports complete (Motor==NULL).
//==============================================================================
bool TestArmBackPos()
{
    bool bError=false;
    static int Temp[2]={0, 0};
    #ifndef SOFT_SIMULTE
    int Z1CmdPos=MOT[MTestZ1].Gali_ReadPos();
    int Z2CmdPos=MOT[MTestZ2].Gali_ReadPos();

    if(Z1CmdPos>=Prod.TestZ1_Safe+50 ||
       Z1CmdPos<=Prod.TestZ1_Safe-50)
    {
        Temp[0]++;
        if(Temp[0]>1000)                                                        //Steven 20170104 : 加上Delay, 等機構穩定
        {
            ShowMyMessage("Index Motor Z1 is not at home position!! [D51]", "Z1馬達未在上方位置!!");                            //Steven 20091004
            Temp[0]=0;
            return false;
        }
        else
        {
            bError=true;
        }
    }

    if(Z2CmdPos>=Prod.TestZ2_Safe+50 || Z2CmdPos<=Prod.TestZ2_Safe<=-50)
    {
        Temp[1]++;
        if(Temp[1]>1000)
        {
            ShowMyMessage("Index Motor Z2 is not at home position!! [D51]", "Z2馬達未在上方位置!!");                            //Steven 20091004
            Temp[1]=0;
            return false;
        }
        else
        {
            bError=true;
        }
    }
    #endif

    if(bError==false)
    {
        if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, 50000, __FUNC__))
        {
            Temp[0]=0;
            Temp[1]=0;
            IndexStatus=IndexIsBack;
            return true;
        }
    }
    return false;
}
//==============================================================================
int iTestTask=1;
void InitTestTask()
{
    iTestTask=1;
}

//******************************************************************************
//
//  注意!! GetOneByOneData為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
extern bool bEcho, bExist, bUnderTest;
extern bool bEchoStop;                                                          //ChungHung 20130326 add
extern unsigned int iBin[4][8];
extern bool bGPIBError;
int HexData[5]={0x01, 0x02, 0x04, 0x08, 0x10};
//------------------------------------------------------------------------------
int IOCT[]={4,                                                                  //_4Bit=0
            8,                                                                  //_8Bit=1
            4,                                                                  //_5BitPE
            9,                                                                  //_10BitPE
            5,                                                                  //_5Bit
            10,                                                                 //_10Bit
            4,                                                                  //_5BitPO
            9};                                                                 //_10BitPO
                    //_3Bit=8;
//------------------------------------------------------------------------------
int GetOneByOneData(int X, int Y)
{
    int iResult=-1;
    int Data;
    int iSite=ArrayConvertSite(X, Y);
    int sum=0;

    if(Prod.DIOCfg.iCateBitLength==_8Bit || Prod.DIOCfg.iCateBitLength==_10Bit ||
       Prod.DIOCfg.iCateBitLength==_10BitPE || Prod.DIOCfg.iCateBitLength==_10BitPO)
    {
        Data=iSite*10;
        if(CosFunction.bTTLCanUse8Site==true)                                   //Alick 20161011 (Steven) : TTL支援8Site
        {
            if(iSite>=4)
                return -1;
        }
        else
        {
            if(iSite>=2)
                return -1;
        }
    }
    else
    {
        Data=iSite*5;
    }

    for(int i=0; i<IOCT[Prod.DIOCfg.iCateBitLength]; i++)
    {
        if(ScanPort[i+Data])                                                    //jou ??? over array
        {
            sum++;
            iResult=i;
        }
    }

    if(iResult!=-1)
    {
        if(sum!=1)
            return iTestBinCount;                                               //Jou 20210126 : 16 --> iTestBinCount
        return iResult+1;
    }
    return -1;
}
//******************************************************************************
//
//  注意!! GetBinaryData為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int GetBinaryData(int X, int Y)
{
    int Sum=0;
    int Data;
    int iSite=ArrayConvertSite(X, Y);
    if(Prod.DIOCfg.iCateBitLength==_8Bit || Prod.DIOCfg.iCateBitLength==_10Bit ||
       Prod.DIOCfg.iCateBitLength==_10BitPE || Prod.DIOCfg.iCateBitLength==_10BitPO)
    {
        Data=iSite*10;
        if(iSite>=2)
            return -1;
    }
    else
    {
        Data=iSite*5;
    }

    for(int i=0; i<IOCT[Prod.DIOCfg.iCateBitLength]; i++)
        Sum+=ScanPort[i+Data]*HexData[i];
    if(Sum==0)
        return -1;
    else
        return Sum;
}
//******************************************************************************
//
//  注意!! Test_GetTestResulSub為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int Test_GetTestResulSub(int X, int Y)
{
#ifndef SOFT_SIMULTE
    for(int i=0; i<10; i++)
    {
        ScanPort[i]     =Sen[SenBit0+i].Status();
        ScanPort[i+10]  =Sen[SenBit10+i].Status();
        ScanPort[i+20]  =Sen[SenBit20+i].Status();                              //Alick 20161011 (Steven) : TTL支援8Site
        ScanPort[i+30]  =Sen[SenBit30+i].Status();
    }
#endif
    for(int i=0; i<10; i++)
    {
        if(ScanPort[i] && SetNoiseDelay==false)
        {
            Noise_Delay.SetSecAndOn(0.1);
            SetNoiseDelay=true;
        }
    }

    if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        return GetOneByOneData(X, Y);
    else
        return GetBinaryData(X, Y);
}
//******************************************************************************
//
//  注意!! Test_GetTestResul為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int Test_GetTestResul(int X, int Y)                                             //990517 保護機制,重複確認50次
{
    int iOld=-1, iBuffer=-1;

    iOld=Test_GetTestResulSub(X, Y);
    if(iOld==-1)
        return -1;

    for(int i=0; i<50; i++)
    {
        iBuffer=Test_GetTestResulSub(X, Y);
        if(iOld!=iBuffer)
            return -1;
    }

    return iOld;
}
//******************************************************************************
//
//  注意!! GetTesterResult為Handler收發BIN相關, 修改時要小心!!
//
//  GATED whole-body (golden :849-2563, ~1700 lines).  The result-acquisition SM
//  drives the tester comms (GPIB/TCP -- W5), MOT[]/Suck/Socket bin writes, and
//  ~hundreds of cross-module/UI symbols.  ACTIVE stub keeps the cursor var
//  (iTestTask) and returns the golden "result not ready yet" default (false).
//******************************************************************************
TQPF_Timer TestWaitTime;
TQPF_Timer MyTSDTimer;                                                          //Steven 20161208 : Record TSD time data
TQPF_Timer LBBoostTimeOutTimer;
TQPF_Timer TestStopMotorTimer;
HTimer bTestFailNeedWait;                                                       //Eastsun 20260526 #026-1.6 Ifor 20250620 add:測試溫度Fail後延遲20秒
bool bLBBoostTimeOut=false;                                                     //Steven 20181222 : Add LB升溫的Time out
bool GetTesterResult(int Type)
{
#if 0 // TODO(W5 tester-comm / W7) -- golden :849-2563 result-acquisition SM (GPIB/TCP + MOT[]/Suck/Socket bin writes)
#endif
    (void)Type;
    (void)iTestTask;
    return false;                                                              // golden default: tester result not ready
}
//******************************************************************************
bool Check_TTL_Status(int Type)                                                 // golden :2564 (decode consumer)
{
    GetTesterResult(Type);
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC &&
               TestSocket.Item[i][j]!=HAS_NULL_IC &&
               TestSocket.Item[i][j]<TEST_PASS)
            {
                if(iTesterBIN[i][j]<0 ||
                   iTesterBIN[i][j]>iTestBinCount)                              //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckTestSocketIsError(int i, int j)                                       // golden :2584 (decode consumer)
{
    if(iTesterBIN[i][j]>=iTestBinCount ||                                       //jou 980818 add test result no set,result set to Interface Error
       iTesterBIN[i][j]<0 ||                                                    //Steven 20120821 : 記憶體溢位
       Prod.iT6CatData[iTesterBIN[i][j]]<0 ||                                   //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
       Prod.iT6CatData[iTesterBIN[i][j]]>=eTrayCount ||                         //JerryYang 20221215 : Magazine最大到14
       Prod.iTrayType[Prod.iT6CatData[iTesterBIN[i][j]]]==tNotUse)
    {
        if(LastSet.iTester==_2D_SORT &&
           CosFunction.bSortingBy2DList &&
           TestIF_File.bSortingBy2DIDList &&                                    //Frank 20221122 : 2DID sorting for ATK
           TestIF_File.iActionOf2DNotInList==1)                                 //Steven 20250806 : Action Of 2D Not In List
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
//  ProcessTestResult (golden :2621-3593) -- bin-decode + DB write + UI report.
//------------------------------------------------------------------------------
int ProcessTestResult(int Index)
{
#if 0 // TODO(W7) -- golden :2621-3593 (bin-decode + cMyDB + fMain/fContact UI)
#endif
    (void)Index;
    return 0;                                                                  // golden default
}
//------------------------------------------------------------------------------
//  ProcessTesterTimeOut (golden :3594-3855) -- tester timeout handler.
//------------------------------------------------------------------------------
int ProcessTesterTimeOut(int Index)
{
#if 0 // TODO(W5 tester-comm / W7) -- golden :3594-3855
#endif
    (void)Index;
    return 0;                                                                  // golden default: no timeout action
}
//------------------------------------------------------------------------------
//  DoInterFaceErrorStep (golden :3856-4103) -- interface-error step SM.
//------------------------------------------------------------------------------
bool DoInterFaceErrorStep(int ZAxisSelect)
{
#if 0 // TODO(W7) -- golden :3856-4103 (MOT[] error-recovery SM)
#endif
    (void)ZAxisSelect;
    (void)iDoInterFaceErrorStepTask;
    return false;                                                              // golden default: error-step not finished
}
//------------------------------------------------------------------------------
//  CheckIndexStatus (golden :4104-4236).  Under SOFT_SIMULTE the golden returns
//  true immediately; offline (no SOFT_SIMULTE) the body derefs MOT[].ISZ1Up_
//  Z2DownNoWait() etc.  GATED whole-body; ACTIVE stub returns the offline-safe
//  "in position" (true) so the gated test-head SM treats the index as settled.
//------------------------------------------------------------------------------
bool CheckIndexStatus(AnsiString str)
{
#if 0 // TODO(W7) -- golden :4104-4236 (MOT[] ISZ1Up_Z2Down/.. position verify)
#endif
    (void)str;
    return true;                                                               // offline: index treated as in position
}
//------------------------------------------------------------------------------
bool CheckShuttlePos()                                                          // golden :4237
{
    for(int i=0; i<2; i++)
    {
        if(MOT[MInShuttle1+i].CompareCommandPos(Prod.InSHT[i].iLeft,  2)!=1 &&  //Sam 20230621 : Gap容許誤差改為1>2 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條 //Ifor 20221117 add: 簡化流程
           MOT[MInShuttle1+i].CompareCommandPos(Prod.InSHT[i].iRight, 2)!=1)
        {
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
//  CheckPlaceOutShuttle (golden :4250-4297) -- MOT[]/shuttle place verify.
//------------------------------------------------------------------------------
bool CheckPlaceOutShuttle(int iShuttle)                                         //Steven 20181228 : Add Index Action
{
#if 0 // TODO(W7) -- golden :4250-4297 (MOT[]/shuttle place verify)
#endif
    (void)iShuttle;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
void InitCheckSocketHasIC()                                                     // golden :4298
{
    iCheckSocketHasIC=1;
}
//------------------------------------------------------------------------------
//  DoCheckSocketHasIC (golden :4305-4783) -- the dense MOT[]/vacuum socket-IC
//  check SM (Gali_Two_ZAxis_Move trees per index pos).  GATED whole-body;
//  ACTIVE stub keeps cursor + returns golden "check not finished" (false).
//------------------------------------------------------------------------------
bool DoCheckSocketHasIC(int iSelArm)                                            //jou 20180802 : index pick error need index vaccum check
{
#if 0 // TODO(W7) -- golden :4305-4783 (MOT[] vacuum-check SM)
#endif
    (void)iSelArm;
    (void)iCheckSocketHasIC;
    return false;                                                              // golden default: socket check not finished
}
//------------------------------------------------------------------------------
void InitTestYTask()
{
    iTestYTask=1;
}
//------------------------------------------------------------------------------
//  DoTestY (golden :4789-5347) -- the test-cycle dispatcher SM.  ACTIVE: switch
//  + cursor transitions reproduced VERBATIM for the oracle path (case 1 -> 50 ->
//  100 -> 110 fall-through).  The heavy per-case bodies that only touch substrate
//  (FTestSuck/BTestSuck/FLCarryKit/BLCarryKit + MOT[] move calls) are kept
//  faithful; the socket-purge debug-log call (fMain->DebugOneCycleHotPlate) is a
//  FormsFacade no-op; the TSMC/Dell short-test sub-trees off the oracle path are
//  gated; the bUseTwoArm32Site path (case 260/300/310) routes to atester_shims.
//------------------------------------------------------------------------------
void DoTestY()
{
    static bool fFront=false;

    int &Task=iTestYTask;
    int Type1=0, Type2=0;
    int iSpeedZ=0;
    bool flag;

    switch(Task)
    {
        case 1:
            dTestSec=TestIF_File.iInitialMaxTime;                               //2013-11-27    Dell Add Index soak time
            bNeedIndexSoakTime=false;                                           //2013-11-27    Dell Add Index soak time
            if(IsInArmCleanOutFinish() ||
               IsInArmOneCycleFinish() ||                                       //Steven 20131029 : 解決Index Position Error
               bCanNotDisableOneCycle)
            {
                if(CanYieldAlarmRemainInSHT() &&                                //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                   (bCanNotDisableOneCycle ||
                   (TestHeadHasIC()==false)))
                {
                    bCanNotDisableOneCycle=true;

                    if(IndexStatus!=Z1_Z2_Normal)
                    {
                        if(bCheckIndex==false)                                  //jou 2011-12-08 做piggy-back時，不用做兩次socket check
                        {
                            InitCheckSocketHasIC();
                            Task=20;
                        }
                        else
                        {
                            Task=25;
                        }
//                        break;                                                //Steven 20180813 : add index arm speed
                    }
                    else
                    {
                        Task=25;
//                        break;
                    }
                }

                if(bCanNotDisableOneCycle ||
                   (TestHeadHasIC()==false &&
                    InputShuttleHasIC()==false &&
                    InArmSuck.HasRealIC()==false))
                {
                    bCanNotDisableOneCycle=true;

                    if(IndexStatus!=Z1_Z2_Normal &&
                       IndexStatus!=IndexIsBack)                                //Steven 20221027 : Fixed D51 hang up
                    {
                        if(bCheckIndex==false)                                  //jou 2011-12-08 做piggy-back時，不用做兩次socket check
                        {
                            InitCheckSocketHasIC();
                            Task=20;
                        }
                        else
                        {
                            Task=25;
                        }
//                        break;                                                //Steven 20180813 : add index arm speed
                    }
                    else
                    {
                        if(LastSet.iTemperature==Tempture_Hot      &&           //Steven 20220428 : Fixed for one cycle hang up
                           TestHeadHasIC()==false  &&
                           InputShuttleHasIC()==false &&
                           InArmSuck.HasType(HAS_HOT_IC)==false)
                        {
                            if(IndexStatus==Z1_Z2_Normal ||
                               IndexStatus==IndexIsBack)                        //Steven 20221027 : Fixed D51 hang up
                            {
                                if(FRCarryKit.UseSiteHasIC())
                                {
                                    MOT[MInShuttle1].fCanMoveM=true;
                                    iIndexTakeDeviceChk1=0;                     //kevin 20190103 回吸檢測狀態
                                }

                                if(BRCarryKit.UseSiteHasIC())
                                {
                                    MOT[MInShuttle2].fCanMoveM=true;
                                    iIndexTakeDeviceChk2=0;                     //kevin 20190103 回吸檢測狀態
                                }
                            }
                            Task=1;
                            break;
                        }
                        Task=25;
//                        break;
                    }
                }
            }

            if(bCanNotDisableOneCycle==false)
            {
                if(bUseTwoArm32Site==true)
                    Task=260;                                                   //InitTestYTwoArm32Site()
                else
                    Task=50;
            }

//            if(Task!=20)                                                      //Steven 20180813 : add index arm speed
                break;
        case 20:
            if(CosFunction.bIndexCheckCanTurnOff &&                             //Isaac 20211019 : 可選擇做index check的時機
               ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
                (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
                 IniConfig.iD71IndexCheckOnOffMode==2))
            {
                Task=25;                                                        //不檢查
            }
            else if(IniConfig.bD55DisableIndexCheck &&                          //ChungHung 20120606 DisableIndexCheck
                    REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)           //Steven 20150723 : Fixed for SCK
            {
                Task=25;
            }
            else if(CosFunction.bBeforeAutoCleanOnlyUseRTC==true &&             //JerryYang 20161216 (Steven) auto clean的前後只靠RTC來檢查socket,不做index下壓至socket吸真空
                    bIsAutoOneCycle==true &&
                    REAL_TIME_CCD==true &&
                    COM2->bCCDDummyRum==false)
            {
                Task=25;
            }
            else if(CosFunction.bAfterAutoCleanNoIndexCheck &&                  //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
                    IniConfig.iD69IndexCheckModeForAutoClean==2 &&
                    bIsAutoOneCycle==true)
            {
                Task=25;
            }
            else
            {
                if(DoCheckSocketHasIC()==true)
                {
                    Task=25;
                }
            }

//            if(Task!=25)                                                      //Steven 20180813 : add index arm speed
                break;
        case 25:
            iSpeedZ=(INDEX_PRESS_TYPE==e240KG || INDEX_PRESS_TYPE==e400KG || INDEX_PRESS_TYPE==e260KG || INDEX_PRESS_TYPE==e360KG)?200000:50000;                //Steven 20110503 : 240KG加速 //Steven 20131007 : Index 1.5KW, 400KG
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedZ, "DoTestY 25"))
            {
                Task=30;
                if(CosFunction.bIndexCheckCanTurnOff)                                                                                                           //Isaac 20211019 : 可選擇做index check的時機，旗標重置
                {
                    bLotStartEndNeedIndexCheck=false;
                    bIndexJamNeedIndexcheck=false;
                }
            }
//            if(Task!=30)                                                      //Steven 20180813 : add index arm speed
                break;
        case 30:
            if(IniConfig.bD51UseOnecycleCleanOutFinishTestArmAtRear)
            {
                Task=45;
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front_EndWaitPos, Prod.TestY2_Rear, 50000, "DoTestY 30"))    //981118 jou Y1 +2000 easy change kit
                    Task=40;
            }
//            if(Task!=40)                                                      //Steven 20180813 : add index arm speed
                break;
        case 40:
            IndexStatus=Z1_Z2_Normal;
            bCanNotDisableOneCycle=false;
            Task=1;
            break;
        case 45:
            if(TestArmBackPos())
            {
                IndexStatus=IndexIsBack;
                bCanNotDisableOneCycle=false;
                Task=1;
            }
            break;
        case 50:
            flag=false;
            if(FTestSuck.UseSiteHasIC())
            {
                if(FTestSuck.AlreadyTest())
                    flag=true;
            }

            if(FTestSuck.UseSiteHasIC()==false)
                Type1=0;
            else if(flag==true)
                Type1=2;
            else
                Type1=1;

            flag=false;

            if(BTestSuck.UseSiteHasIC())
            {
                if(BTestSuck.AlreadyTest())
                {
                    flag=true;
                }
            }

            if(BTestSuck.UseSiteHasIC()==false)
                Type2=0;
            else if(flag==true)
                Type2=2;
            else
                Type2=1;

            if(FTestSuck.PartAlreadyTest())
            {
                fFront=false;
            }
            else if(BTestSuck.PartAlreadyTest())
            {
                fFront=true;
            }
            else
            {
                fFront=true;
                if(     Type1==2) fFront=true;
                else if(Type2==2) fFront=false;
                else if(Type1==0) fFront=true;
                else if(Type2==0) fFront=false;
                else if(Type1==1) fFront=false;
                else if(Type2==1) fFront=true;
                if(Type1==0 && Type2==0)
                {
                    if(FLCarryKit.UseSiteHasIC())
                    {
                        fFront=true;
                    }
                    else
                    {
                        if(BLCarryKit.UseSiteHasIC())                           //need check chang else
                            fFront=false;
                    }
                }
            }

            if(fFront)
                Task=100;
            else
                Task=200;
            break;
        case 60:
            if(IniConfig.bD47EnableSocketPurgeFunction &&
               IniConfig.iD47SocketPurgeCount!=0 &&
               iOneCycle==0 &&
               iCleanOut==0)
            {
                if(LastSet.iD47SocketTestedCount>=IniConfig.iD47SocketPurgeCount)
                {
                    LastSet.iD47SocketTestedCount=0;
                    iClearSocketFunction=1;
                    iOneCycle=1;
                    fMain->DebugOneCycleHotPlate("DoTestY_60");                                                         //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                }
            }

            if(IsInArmOneCycleFinish() ||
               IsInArmCleanOutFinish())
            {
                if(IniConfig.bIndexArm2SupplyLight==true ||                                                             //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                   TestIF_File.bForEgisTecTest==true     ||                                                             //Steven 20140922 : Arm2當作指紋測試
                   (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                     //kevin 20150127 Arm1 下壓 arm2 測試
                    TestIF_File.bArm1PickPlaceArm2Test==true))                                                          //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                {
                    if(FTestSuck.UseSiteHasIC()==false &&
                       BTestSuck.UseSiteHasIC()==false &&
                       FLCarryKit.UseSiteHasIC()==false &&
                       InArmSuck.HasRealIC()==false)
                    {
                        Task=1;
                        break;
                    }
                }

                if(CanYieldAlarmRemainInSHT()==true)                                                                    //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                    if((FTestSuck.UseSiteHasIC()==false || FTestSuck.HasRealIC()==false) &&
                       (BTestSuck.UseSiteHasIC()==false || BTestSuck.HasRealIC()==false))
                    {
                        FTestSuck.ClearAll();
                        BTestSuck.ClearAll();
                        Task=1;
                        break;
                    }
                }

                if((FTestSuck.UseSiteHasIC()==false  || FTestSuck.HasRealIC()==false) &&
                   (BTestSuck.UseSiteHasIC()==false  || BTestSuck.HasRealIC()==false) &&
                   (FLCarryKit.UseSiteHasIC()==false || FLCarryKit.HasRealIC()==false) &&                               //Steven 20191213 : 修正One Cycle的動作
                   (BLCarryKit.UseSiteHasIC()==false || BLCarryKit.HasRealIC()==false) &&
                   InArmSuck.HasRealIC()==false)
                {
                    FTestSuck.ClearAll();
                    BTestSuck.ClearAll();
                    FLCarryKit.ClearAll();
                    BLCarryKit.ClearAll();

                    Task=1;
                    break;
                }
            }

            if(iCleanOut!=0 && IndexHasIC()==false && InputShuttleHasIC()==false)
                break;

            // golden :5099-5176 -- TSMC short-test/initial-delay + Dell index-soak
            // branch tree (deref Prod/Temperature/CosFunction flags + fMain timers).
            // Off the oracle path; GATED.  Offline takes the plain Task=70.
#if 0 // TODO(W7) -- golden :5099-5176 (TSMC initial-delay + Dell hIndexSoakTime/tSoakTimer tree)
#endif
            Task=70;
            break;
        case 65:                                                                //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            if(hIndexSoakTime.Off())
                Task=70;
            break;
        case 70:
            if(CosFunction.bOEEFunction)                                        //Steven 20180417 (Jou) : OEE功能
            {
                fObserver->bTestIndexZ=true;
            }
            fFront=!fFront;
            if(fFront)
            {
                Task=100;
            }
            else
            {
                Task=200;
            }

            if(Task!=100)                                                       //Steven 20180813 : add index arm speed
                break;
        case 100:
            InitTestYFrontTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=110;
        case 110:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestYFront())
            {
                if(IsInArmOneCycleFinish() ||
                   IsInArmCleanOutFinish())
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       FTestSuck.UseSiteHasIC()==false   &&
                       BTestSuck.UseSiteHasIC()==false   &&
                       FLCarryKit.UseSiteHasIC()==false  &&
                       BLCarryKit.UseSiteHasIC()==false  &&
                       FRCarryKit.UseSiteHasIC()==false  &&                     //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                       InArmSuck.HasType(HAS_HOT_IC)==false)
                    {
                        if(IndexStatus==Z1_Z2_Normal)
                        {
                            if(FRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle1].fCanMoveM=true;
                                iIndexTakeDeviceChk1=0;                         //kevin 20190103 回吸檢測狀態
                            }

                            if(BRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle2].fCanMoveM=true;
                                iIndexTakeDeviceChk2=0;                         //kevin 20190103 回吸檢測狀態
                            }
                            Task=1;
                            break;
                        }
                    }
                }
                Task=60;
            }
            break;
        case 200:
            InitTestYRearTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=210;
        case 210:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestYRear())
            {
                if(IsInArmOneCycleFinish() ||
                   IsInArmCleanOutFinish())                                     //Steven 20220426 : 避免One Cycle發生index position error
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       FTestSuck.UseSiteHasIC()==false   &&
                       BTestSuck.UseSiteHasIC()==false   &&
                       FLCarryKit.UseSiteHasIC()==false  &&
                       BLCarryKit.UseSiteHasIC()==false  &&
                       BRCarryKit.UseSiteHasIC()==false  &&                     //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                       InArmSuck.HasType(HAS_HOT_IC)==false)
                    {
                        if(IndexStatus==Z1_Z2_Normal)
                        {
                            if(FRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle1].fCanMoveM=true;
                                iIndexTakeDeviceChk1=0;                         //kevin 20190103 回吸檢測狀態
                            }

                            if(BRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle2].fCanMoveM=true;
                                iIndexTakeDeviceChk2=0;                         //kevin 20190103 回吸檢測狀態
                            }
                        }
                        Task=1;
                        break;
                    }
                }
                Task=60;
            }
            break;
        case 260:
            if(IniConfig.bD47EnableSocketPurgeFunction &&
               IniConfig.iD47SocketPurgeCount!=0 &&
               iOneCycle==0 &&
               iCleanOut==0)
            {
                if(LastSet.iD47SocketTestedCount>=IniConfig.iD47SocketPurgeCount)
                {
                    LastSet.iD47SocketTestedCount=0;
                    iClearSocketFunction=1;
                    iOneCycle=1;
                    fMain->DebugOneCycleHotPlate("DoTestY_260");                //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                }
            }

            if(IsInArmOneCycleFinish() ||
               IsInArmCleanOutFinish())                                         //Steven 20220426 : 避免One Cycle發生index position error
            {
                if(FTestSuck.UseSiteHasIC()==false  &&
                   BTestSuck.UseSiteHasIC()==false  &&
                   FLCarryKit.UseSiteHasIC()==false &&
                   BLCarryKit.UseSiteHasIC()==false)
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(InArmSuck.HasType(HAS_HOT_IC)==false)
                        {
                            Task=1;
                            break;
                        }
                    }
                    else                                                        //Steven 20220514 : 修正Clean Out死雞
                    {
                        if(IsNNMode()!=NN_2Row)
                        {
                            FTestSuck.ClearAll();
                            BTestSuck.ClearAll();
                        }

                        if(InArmSuck.HasIC()==false)
                        {
                            Task=1;
                            break;
                        }
                    }
                }
            }

            if(iCleanOut!=0 && IndexHasIC()==false && InputShuttleHasIC()==false)
                break;

            Task=300;
            break;
        case 300:
            if(CosFunction.bOEEFunction)                                        //Steven 20180417 (Jou) : OEE功能
            {
                fObserver->bTestIndexZ=true;
            }
            InitTestYTwoArm32SiteTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=310;
//            break;                                                            //Steven 20180813 : add index arm speed
        case 310:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestY_TwoArm32Site())
                Task=260;
            break;
    }
}
//------------------------------------------------------------------------------
int iTestHeadMotorTask=1;
int iProcessIndexSuckDestroyCnt=0;                                              //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
void InitialTestHeadMotorTask()
{
    iTestHeadMotorTask=1;
    if(iProcessIndexSuckDestroyCnt>15 || TestIF_File.bRENESAS_EnableFTCT==false)                                        //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
        iProcessIndexSuckDestroyCnt=0;

    if(MOT[MTestY1].Motor!=NULL)                                                //Sam 20220718 : 防止 OneCycle and CleanOut IndexArm 偷跑
        MOT[MTestY1].Gali_Command("VS0;SP0,0,0,0;", __FUNC__);
}
//------------------------------------------------------------------------------
bool IsIndexRunCycle()
{
    if(iTestHeadMotorTask==600)
    {
        if(bCanNotDisableOneCycle==true)
            return false;
        return true;
    }
    else
    {
        return false;
    }
}
// ===========================================================================
//  W7-T1 SEAM BLOCK  -- TU-local forward declarations / offline stand-ins for
//  the down-press / test / torque / EP / load-cell case tree (golden atester.cpp:
//  6229-8650, un-gated below) references that are NOT yet present in the
//  translated tree.  Mirrors the W7C1_SEAM / W7C2_SEAM pattern in csystem.cpp:
//  each stand-in is offline-inert (no-op / golden-safe default) so the faithful
//  golden body lands + compiles NOW; the serial Integrate must replace every
//  W7T1_* stand-in with the REAL translated symbol / real facade member.
//
//  NO-REGRESSION KEY: SOFT_SIMULTE is OFF in this build, so the #else torque
//  branch (COM2->iWriteAndCheckMotorTorque) compiles -- seamed to return 1 (OK)
//  so the torque SM advances exactly as the SOFT_SIMULTE branch would.  MOT[] Z/Y
//  moves + Suck/Socket bin writes are REAL (S0-converged / grid); NOT seamed.
// ===========================================================================
#ifndef W7T1_SEAM
#define W7T1_SEAM

// --- wave-timing (common.cpp now provides a real MyTickCount, un-gated by
//  AI(W906-CommonCompletion) 20260721 -- this TU-local macro redirect below
//  still intentionally shadows it with the offline stand-in, unchanged by that
//  wave; revisiting whether this seam should switch to the real tick source is
//  a separate translation decision, out of scope here) ------------------------
//  case 20200/20210 idle-drain timer.  Offline: monotonic 0 (no real tick source
//  yet); startTick/endTick/nowTick are function locals below.
static DWORD W7T1_MyTickCount(){ return (DWORD)0; }            // golden common.h:259
#define MyTickCount            W7T1_MyTickCount

// --- RTC-CCD entry case ids: moved to golden file-scope home (atester.cpp:5551-5555,
//  const int iCASE_REAL_CCD2..6 near DoTestHeadMotorDelay above).  The stale
//  atester_shims.cpp iCASE_REAL_CCD2=9 (+ its atester_shims.h extern) is REMOVED
//  by Integrate so the switch labels carry the golden 40200/40300/40400/40500/40510
//  and no longer collide with the active `case 9:`.  Nothing needed here now.

// --- index-check / torque / socket-heating timers (golden atester.cpp file-scope
//  TQPF_Timer objects).  REAL TQPF_Timer instances (NOT stand-ins) -- same class
//  the active DoTestHeadMotorDelay uses -- so their .Off()/.SetSecAndOn()/
//  .LatchCycleTime() behave exactly as golden.  Absent as instances in the tree
//  today; add TU-local.  Integrate: move to the real file-scope home.
static TQPF_Timer W7T1_ReadTorqueDelay;                        // golden atester.cpp (kevin 20210824)
#define ReadTorqueDelay        W7T1_ReadTorqueDelay
static TQPF_Timer W7T1_DoUseSocketHeating;                     // golden atester.cpp (Ztex 2024.09.07)
#define DoUseSocketHeating     W7T1_DoUseSocketHeating
static TQPF_Timer W7T1_CheckSocketHasICDelay;                  // golden atester.cpp (JerryYang 20250807)
#define CheckSocketHasICDelay  W7T1_CheckSocketHasICDelay

// --- absent FREE functions (golden atester.cpp inline / main.h homes) ----------
static void W7T1_ShowMainScreenPresure(int /*iArm*/){}         // golden main.h -- offline: main-screen pressure UI no-op
#define ShowMainScreenPresure  W7T1_ShowMainScreenPresure
static void W7T1_InitWriteAndCheckMotorTorqueTask(){}          // golden atester.cpp -- torque-write cursor reset (offline no-op)
#define InitWriteAndCheckMotorTorqueTask  W7T1_InitWriteAndCheckMotorTorqueTask
static void W7T1_RecordIndexPosition(int /*a*/, int /*b*/){}   // golden atester.cpp (Isaac 20200922) -- encoder-vs-command log (offline no-op)
#define RecordIndexPosition    W7T1_RecordIndexPosition
static void W7T1_EncoderTeachingMaxMinCount(int /*a*/){}       // golden atester.cpp (Isaac 20201012) -- encoder/teach delta log (offline no-op)
#define EncoderTeachingMaxMinCount  W7T1_EncoderTeachingMaxMinCount
static void W7T1_TrigerIndexAxisHome(){}                       // golden atester.cpp (Isaac 20201012) -- index Y over-range auto-home (INDEX_PROTECT_TMOVE gated)
#define TrigerIndexAxisHome    W7T1_TrigerIndexAxisHome
static int  W7T1_ShowMyMessageBox_YES_NO(AnsiString /*s1*/, AnsiString /*s2*/=""){ return 0; } // golden mymessbox.h -- offline: NO(0) (mirrors csystem W7C2)
#define ShowMyMessageBox_YES_NO  W7T1_ShowMyMessageBox_YES_NO

// --- fiosetview index-suck self-check methods (golden iosetview.h) -------------
//  The offline TfiosetviewShim (atester_shims.h) exposes bIndexSuck[][][] but NOT
//  the ProcessIndexSuckDestroy1/2 pump methods.  Offline (no DAQ): report the
//  suck-check "complete" (true) so the index-check SM advances.  Integrate: add
//  the real methods to TfiosetviewShim (or the real iosetview form) + drop the
//  W7T1_FIOSET_PISD* call-site macros.
static bool W7T1_ProcessIndexSuckDestroy1(){ return true; }    // golden iosetview.h -- offline: suck self-check done (true)
static bool W7T1_ProcessIndexSuckDestroy2(){ return true; }    // golden iosetview.h -- offline: suck self-check done (true)
#define W7T1_FIOSET_PISD1()    W7T1_ProcessIndexSuckDestroy1()  // golden fiosetview->ProcessIndexSuckDestroy1()
#define W7T1_FIOSET_PISD2()    W7T1_ProcessIndexSuckDestroy2()  // golden fiosetview->ProcessIndexSuckDestroy2()

// --- TMyKitSuck::CheckVaccumIsIniaialON (golden MyKitSuck.h) -------------------
//  Absent from the W6 TMyKitSuck mirror (aHotPlateSubstrate.h).  Offline (no
//  vacuum DAQ): the "is-initial-vacuum-on" out-param stays false (no residual IC
//  detected) so the socket-residual alarm branch is NOT taken.  Integrate: add
//  the real member + drop the W7T1_CHECKVACINIT call-site macro.  (Called on
//  FTestSuck/BTestSuck -- REAL objects.)
#define W7T1_CHECKVACINIT(kit,i,j,out)  do { (out)=false; } while(0)   // golden (kit).CheckVaccumIsIniaialON(i,j,flag)

// --- fContact contact-mode index-check form (golden cContact.h) ----------------
//  fContact (TfContactShim*, atester_shims.h) exposes Do_ROILearning() [no-arg]
//  but the golden RTC path calls Do_ROILearning(true) + InitROILearningTask().
//  Offline: ROI learning "done" (true); init no-op.  Integrate: add the bool
//  overload + InitROILearningTask to the real fContact / cContact form + drop the
//  W7T1_FCONTACT_* call-site macros.
#define W7T1_FCONTACT_DOROI(b)      (fContact->Do_ROILearning())   // golden fContact->Do_ROILearning(true); offline done(true)
#define W7T1_FCONTACT_INITROI()     do { } while(0)                // golden fContact->InitROILearningTask(); offline no-op

// --- fMain torque / open-bin UI members (golden main.h) ------------------------
//  The FormsFacade TfMain (FormsFacade.h) lacks the torque-read UI widgets
//  (lbArm0Torque / lbArm1Torque / chkReadTorque1 / chkReadTorque2 / edTorue0 /
//  edTorue1 / labUser) + SetOpenBin().  With the COM2 torque seam returning OK,
//  the torque-read path is offline-inert; but the code that reads/writes these
//  widgets must compile.  A TU-local stand-in mirrors the touched surface
//  (Caption / Checked / Text.c_str()).  Integrate: add the real widgets to TfMain
//  (TPanel/TLabel/TCheckBox/TEdit -- see the per-member golden citations below)
//  + SetOpenBin() + drop the W7T1_FMAIN_* call-site macros.
//
//  AI(W906-W7-F2) 20260729: the three TU-local value-holder types this block used to
//  declare -- W7T1_TLabelSeam / W7T1_TCheckSeam / W7T1_TEditSeam -- are RETIRED.
//  vclcompat/Controls.h is now the single home for stock-widget stand-ins (plan D4),
//  so the members below name those unified types directly.  Each member's golden
//  widget class was re-read from golden main.h for this change rather than inherited
//  from the old comment: lbArm0Torque main.h:798 and lbArm1Torque main.h:797 are
//  TPanel (NOT TLabel -- they do not all collapse onto one type), labUser main.h:649
//  is TLabel, chkReadTorque1/2 main.h:464-465 are TCheckBox, edTorue0/1 main.h:466-467
//  are TEdit.  Zero behaviour change: each retired type held exactly the one member
//  used here with the same default ("" / false) and the unified replacements keep
//  those defaults; the ONLY objects of these types are the members of the single
//  file-scope W7T1_fMainTorque below (no by-value copy, no aggregate initialisation,
//  no sizeof/memset), so gaining a vtable disturbs nothing.
struct W7T1_TfMainTorqueSeam {
    TPanel    lbArm0Torque, lbArm1Torque;                      // golden main.h:798 / :797 (TPanel*)
    TLabel    labUser;                                         // golden main.h:649 (TLabel*)
    TCheckBox chkReadTorque1, chkReadTorque2;                  // golden main.h:464-465 (TCheckBox*)
    TEdit     edTorue0, edTorue1;                              // golden main.h:466-467 (TEdit*)
    void SetOpenBin(){}                                        // golden main.h -- offline: open-bin set no-op
};
static W7T1_TfMainTorqueSeam W7T1_fMainTorque;
// AI(W7T1-Integrate) 20260701: golden derefs these as POINTERS (fMain->lbArm0Torque->Caption,
// lbArm0Torque is TPanel*, labUser TLabel*, chkReadTorque* TCheckBox*, edTorue* TEdit* -- golden
// main.h:797/798/649/464-467).  Macros yield &member so the golden `->` deref compiles unchanged.
#define W7T1_FMAIN_LBARM0TORQUE   (&W7T1_fMainTorque.lbArm0Torque)
#define W7T1_FMAIN_LBARM1TORQUE   (&W7T1_fMainTorque.lbArm1Torque)
#define W7T1_FMAIN_LABUSER        (&W7T1_fMainTorque.labUser)
#define W7T1_FMAIN_CHKREADTORQUE1 (&W7T1_fMainTorque.chkReadTorque1)
#define W7T1_FMAIN_CHKREADTORQUE2 (&W7T1_fMainTorque.chkReadTorque2)
#define W7T1_FMAIN_EDTORUE0       (&W7T1_fMainTorque.edTorue0)
#define W7T1_FMAIN_EDTORUE1       (&W7T1_fMainTorque.edTorue1)
#define W7T1_FMAIN_SETOPENBIN()   W7T1_fMainTorque.SetOpenBin()

// --- COM2 torque / RTC-vision members (golden rs232.h) -------------------------
//  TCOM2Shim (atester_shims.h) offline exposes bCCDDummyRum(true) +
//  DoReleaseAndInspEnd(); the down-press tree also derefs the torque-write /
//  RTC-site-map / full-view-vision handshake members.  This TU-local extended
//  shim is a SUPERSET (keeps bCCDDummyRum=true + DoReleaseAndInspEnd no-op so the
//  ACTIVE preamble / case-600000 behaviour is IDENTICAL) and adds the down-press
//  members offline-inert:
//    iWriteAndCheckMotorTorque -> 1 (OK, == SOFT_SIMULTE),
//    GetReadTorueTask          -> 0 (NOT the 999 read-error sentinel),
//    OpenRTCComPortAgain       -> false (no re-open),
//    bRealTimeCom_ReceiveOK[]  -> false (vision never "received" -> RTC branches
//                                 fall to the golden time-out retry; never
//                                 dereference an untranslated RTC path offline),
//    rtSiteMap/rtFullTOK/rtFullTNG index enum ids.
//  #define COM2 redirects EVERY COM2 use in this TU (active + un-gated) to the
//  superset.  Integrate: extend the REAL TCOM2Shim with these members + drop the
//  #define.
enum { W7T1_rtSiteMap=0, W7T1_rtFullTOK=1, W7T1_rtFullTNG=2, W7T1_RT_N=8 };
struct W7T1_TCOM2Ext {
    bool bCCDDummyRum;                                         // golden rs232.h:157 -- offline true (== TCOM2Shim)
    bool bRealTimeCom_ReceiveOK[W7T1_RT_N];                    // golden rs232.h -- offline all false
    int  rtSiteMap, rtFullTOK, rtFullTNG;                      // golden rs232.h -- RT channel index ids
    void DoReleaseAndInspEnd(){}                               // golden rs232.h:160 -- offline no-op (== TCOM2Shim)
    int  iWriteAndCheckMotorTorque(int /*iArm*/, int /*iPre*/){ return 1; } // golden rs232.h -- offline OK(1)
    int  GetReadTorueTask(){ return 0; }                       // golden rs232.h -- offline: not 999 (no read-error)
    void InitReadTorueTask(){}                                 // golden rs232.h -- offline no-op
    void SendCommToVision(int /*ch*/, bool /*b*/){}            // golden rs232.h -- offline no-op
    bool OpenRTCComPortAgain(){ return false; }                // golden rs232.h -- offline: no re-open
    W7T1_TCOM2Ext():bCCDDummyRum(true),rtSiteMap(W7T1_rtSiteMap),
                    rtFullTOK(W7T1_rtFullTOK),rtFullTNG(W7T1_rtFullTNG)
    { for(int i=0;i<W7T1_RT_N;i++) bRealTimeCom_ReceiveOK[i]=false; }
};
static W7T1_TCOM2Ext W7T1_com2_ext;
#ifdef COM2
#undef COM2
#endif
#define COM2 (&W7T1_com2_ext)

#endif // W7T1_SEAM

//------------------------------------------------------------------------------
//  DoTestHeadMotor (golden :5562-8655) -- the central index/test SM, ~3000
//  lines.  ACTIVE: the entry preamble + the oracle-window cases (1, 200000,
//  300000, 400000, 500000, 600000, 2,3,4,5,6,7,9,10,15,20,21) reproduced with
//  cursor transitions VERBATIM so the test-head walks 4 -> 9 -> 10 -> 15 over
//  Sim HAL (offline Motor==NULL -> Gali Z/Y moves report complete immediately,
//  ISNormal()==true).  W7-T1: the dense down-press/test/torque/EP/load-cell
//  per-case tree (golden :6229-8650 -- case 30..600, 12000.., 14000.., 122100..,
//  142100.., the RTC/load-cell branch, the cContact ROI hand-off) is now
//  TRANSLATED + ACTIVE (below), entered from the active window (case 21 ->
//  30/60/100/115; case 9 -> 30000).  The deep externals (torque(COM2)/EP-DAQ/
//  RTC-vision/cContact-ROI/torque-UI) route through the W7T1_SEAM stand-ins
//  above (offline-inert); MOT[] Z/Y + Suck/Socket are REAL.  The CCD-form
//  accesses on the install path stay gated (offline bC02InstallCCD==false).
//------------------------------------------------------------------------------
void DoTestHeadMotor()
{
    static int iRetryCount=0;
    int &Task=iTestHeadMotorTask;
    int ret=0;
    bool flag=false;
    AnsiString str;
    // W7-T1: golden DoTestHeadMotor locals (golden atester.cpp:5564-5578) that the
    // now-un-gated down-press/torque/EP tree consumes.  W6.4 originally elided
    // these while the tree was gated; restored here VERBATIM (minus bFlag/ccRet,
    // which the tree never references).  golden atester.cpp:5564-5578.
    static int iSiteCount=0;
    static int iToqureCount=0;
    static bool bOneTimeFlag=true;
    static bool bFirstTime=true;
    static bool bintered1=true;                                                 //Isaac 20200922 : record indexArmY encoder value vs command value  (golden atester.cpp:5570)
    static DWORD startTick=-1, endTick=0, nowTick=0;
    static AnsiString ErrPart="";
    int TorqueData, iIndexUpPos=0, iContactZ=0;
    int sp, iMaxPreasure=0, iIndexArm[3]={0, 0, 0};
    bool flag2, bIndexSuckCheck;
    bool TMode=false;

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

    if(IniConfig.bEnableCCDUSETCPIP)                                            //kevin 20110811 start
    {
        if(ScanCCDProgram())
        {
            CCDInterfaceForm->CCDRunExec();
        }
    }                                                                           //kevin 20110811 End

    if(iPauseBackUp!=-1 &&
       FTestSuck.IsShtSuckFinish()==true &&                                     //ChungHung 20110901 add
       FTestSuck.IsShtDestroyFinish()==true   &&
       BTestSuck.IsShtSuckFinish()==true &&
       BTestSuck.IsShtDestroyFinish()==true)
    {
        return;
    }

    // golden :5604-5621 -- USE_16_HEATER connect-check (CheckIndexConnect +
    // WAR0360 alarm).  Offline: no heater bus; GATED (the alarm sink + the
    // 16/32-heater bus deref have no translated home).
#if 0 // TODO(W7) -- golden :5604-5621 (USE_16_HEATER CheckIndexConnect + WAR0360)
#endif

    if(bIndexCheckState)                                                        //wei 20150903 如果有Auto clean 就重頭開始
    {
        bIndexCheckState=false;
        Task=1;
    }

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //ChungHung 20210113 add for Alignment CCD start
       TestIF.bEnableAutoAlignment==true &&                                     //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       (LastSet.iRealDummy==HAS_TRAY ||
        LastSet.iRealDummy==REALLY))
    {
        if(lInArmAutoAlignmentCKTimingFlag!=0x00   || bRunInArmAutoAlignment ||
           lOutArmAutoAlignmentCKTimeingFlag!=0x00 || bRunOutArmAutoAlignment)
            return;
    }

    if((bSht1LoseICErr==true ||
        bSht2LoseICErr==true) &&
       LastSet.iRealDummy==REALLY)                                              //Jimmychiu 20230921 : add index arm stop when outshuttle lose ic
    {
        return;
    }

    if(bEject)                                                                  //JerryYang 20251020 : 渠梁半清機功能
    {
        return;
    }

    switch(Task)
    {
        case 1:                                                                 //確認Index Arm 吸嘴狀態
            if(CUSTOMER_CODE==CC_ASE_KaohSiung && IniConfig.bG11ASEReport)
                ReadWriteTrayID(true);                                          // kevin 20220618 read Tray ID

            if(Prod.TestZ1_Test==0)
                iSocketSenSosPos1=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;

            if(Prod.TestZ2_Test==0)
                iSocketSenSosPos2=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;

            if(bUseInitTempOffset)                                              //Steven 20141117 : 起測時溫度要補Offset
            {
                RecordProcess("After index check trigger initial offset function.(Temp)");
                iInitContactCount=0;                                            //Steven 20141117 : 起測時溫度要補Offset
                fHeaterOK=false;
            }

            if(INSTALL_SOCKET_CLAMP)                                            //JerryYang 20160607 機台選用分離機構 需偵測socket sensor
            {
                if(Sen[SnSocketHasClamp1].IsOn() || Sen[SnSocketHasClamp2].IsOn())
                {
                    ShowMyMessage("Please check the socket sensor","socket sensor偵測異常");
                    break;
                }
            }
            Task=200000;
            break;
        case 200000:                                                            //Steven 20160318 : 真空產生器自檢功能
            FTestSuck.ResetAll();
            BTestSuck.ResetAll();
            for(int i=0; i<FTestSuck.iShtRow; i++)                              //JerryYang 20160727 修正沒使用的真空產生器也吸真空
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)                                    //Steven 20111202
                    {
                        fiosetview->bIndexSuck[0][i][j]=true;
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].On();                              //kevin 20110504 check 掉料
                    }
                }
            }

            if(iProcessIndexSuckDestroyCnt==0 ||                                //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
                iProcessIndexSuckDestroyCnt>15)
            {
                iProcessIndexSuckDestroyCnt=0;
            bIndexCheck1=true;                                                  //kevin 20170120
            Task=300000;
            }
            else
            {
                Task=600000;
            }

            if(TestIF_File.bRENESAS_EnableFTCT==true)                           //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
                iProcessIndexSuckDestroyCnt++;
            break;
        case 300000:
            // golden :5719-5760 -- index suck/destroy self-check (fiosetview->
            // ProcessIndexSuckDestroy1 + FTestSuck/BTestSuck destroy).  Offline
            // there is no DAQ; GATED.  Advance straight to the next self-check.
#if 0 // TODO(W7) -- golden :5719-5760 (INDEX_SUCKER_TYPE suck/destroy self-check)
#endif
            Task=400000;
            break;
        case 400000:
#if 0 // TODO(W7) -- golden :5763-5780 (suck/destroy self-check arm2)
#endif
            Task=500000;
            break;
        case 500000:
#if 0 // TODO(W7) -- golden :5783-5800 (suck/destroy settle delay)
#endif
            Task=600000;
            break;
        case 600000:
            // golden :5805-5990 -- the index-check entry selector: AutoClean /
            // AutoSiteMap / CCD-TCPIP / REAL_TIME_CCD branches choose the next
            // Task.  Offline (no CCD, no RTC, no auto-clean) the golden falls to
            // the plain "Task=9" else-branch (oracle).  The auto-clean/auto-site
            // map sub-branches that reach untranslated forms are GATED above the
            // CCD-TCPIP/RTC selector.
#if 0 // TODO(W7) -- golden :5805-5947 (AutoClean / AutoSiteMap index-check sub-branches)
#endif
            if(IniConfig.bEnableCCDUSETCPIP)
            {
                CCDInterfaceForm->CCDTimerOnOff(IniConfig.bC02InstallCCD);      //Steven 20110809
                if(IniConfig.bC02InstallCCD==true)
                {
                    fMain->LightOn();
                    DoTestHeadMotorDelay.SetSecAndOn(3);
                    iCCDTimeOutCount=0;
                    Task=2;
                    break;
                }
                else
                {
                    Task=9;
                    break;
                }
            }
            else
            {
                if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                  //ChungHung 20121011 鉬FullView 謖IndexCheck
                {
                    COM2->DoReleaseAndInspEnd();
                    Task=iCASE_REAL_CCD2;
                }
                else
                {
                    Task=9;
                }
            }
            break;
        case 2:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 2"))
            {
                Task=3;
            }
            break;
        case 3:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 3"))
            {
                Task=4;
            }
            break;
        case 4:
            if(IniConfig.bC02InstallCCD==false)
            {
                Task=9;
                break;
            }
            fMain->lbCCDStatus->Visible=true;
            if(DoTestHeadMotorDelay.Off()==false)
                break;
            // golden :6055-6063 -- CCD identification kick-off (CCDInterfaceForm
            // ->CCDIdentificationOpen/CCDIdentification).  Gated CCD form access;
            // offline this point is unreachable (bC02InstallCCD==false above).
#if 0 // TODO(W7) -- golden :6055-6063 (CCDInterfaceForm identification)
#endif
            Task=5;
            DoTestHeadMotorDelay.SetSecAndOn(20);                               //CCDTimeOutSec);  20110810 設定5秒太短會一直取像
            DoTestHeadMotorDelay2.SetMSAndOn(200);
            break;
        case 5:
            // golden :6067-6128 -- CCD result poll (CCDInterfaceForm program /
            // identification status, fShowMessage form click).  Gated CCD form.
#if 0 // TODO(W7) -- golden :6067-6128 (CCDInterfaceForm result poll)
#endif
            Task=9;
            break;
        case 6:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 6"))
            {
                Task=7;
            }
            break;
        case 7:
            if(IndexAlarmInArmAway()==false)                                    //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                return;
            }
            fMain->Pause("DoTestHeadMotor");
            // golden :6166-6167 -- CCDInterfaceForm->CCDIdentification + program
            // existence flag.  Gated CCD form.
#if 0 // TODO(W7) -- golden :6166-6167 (CCDInterfaceForm CCD identification)
#endif
            Task=1;
            break;
        case 9:                                                                 //IndexZ1 and IndexZ2 皆移至安全位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 9"))
            {
                if(TestIF_File.iTestType==TTL_MODE &&
                   (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                      //Isaac 20210309 :TTL RS232通訊
                    SendTTLRS232CSOTsignal();

                CCDInterfaceForm->CCDTimerOnOff(false);                         //Steven 20110809
                iCCDTimeOutCount=0;
                Task=10;

                if(IniConfig.bD24EnableEPCheckFuntion==true &&
                   IniConfig.bIndexEveryTimeCheckEP==true)
                {
                    if(CUSTOMER_CODE!=CC_VTEST_Shanghai)                        // &&                   //jou 20210911 : 上海無錫 張冬冬 index check 不檢查EP
//                       CUSTOMER_CODE!=CC_SCC)                                 //Steven 20240926 : JSCC 李小川 index check 不檢查EP
                    {
                        InitIndexEveryTimeCheckEP();
                        Task=30000;
                    }
                }
                else
                {
                    bIndexEveryTimeCheckEPing=false;
                }
            }
            break;
        case 10:                                                                //判斷IndexZ1 and IndexZ2 是否已在安全位置
            if(MOT[MTestZ1].ISNormal()==false)
            {
                iRetryCount++;
                if(iRetryCount>3)
                {
                    ShowIndexMotorError(AnsiString("DoTestHeadMotor10"));
                    iRetryCount=0;
                }
                else
                {
                    Task=9;
                }
                return ;
            }
            Task=15;
            break;
        case 15:                                                                //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 15"))
            {
                Task=20;
            }
            break;
        case 20:
            if(IniConfig.bC08_SocketSensor && TestIF_File.bEnSocketSensor &&    //kevin 20160209 //kevin 20130504 socket sensor detect error
               (TestIF_File.bSocketDisibleinitialcheck==false ||                //JerryYang 20170426 (Steven) 修正Disable socket sensor detect
                CUSTOMER_CODE==CC_GIGAS))                                       //Isaac 20220126 : 全智要求Index arm在上要強制偵測sensor(應該為off)
            {
                str="check socket sensor state must off, sensor: ";
                for(int i=0; i<TestIF_File.iSocketCount; i++)
                {
                    if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                       Sen[SThreadPara.iSocketSensor[i]].IsOn())                //kevin 20150429 : Arm在上, 檢查無遮斷, 要Off
                    {
                        flag=true;
                        bIsSocketSensor=true;
                        str+=IntToStr(i+1);
                    }
                }

                if(flag)
                {
                    ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, str);  //kevin 20130504 socket sensor
                    return;                                                     //kevin 20150429確認 socket sensor是否正常
                }
            }
            Task=21;
            break;
        case 21:                                                                //Index1 or Index2 是否需要拋下IC
            EPSwitchOnOff(eEPSwBoth);                                           //Steven 20250417 : fixed for [D58]

            if(bFTestSuckDrop==true)
            {
                Task=30;
            }
            else if(bBTestSuckDrop==true)
            {
                Task=60;
            }
            else
            {
                if(CosFunction.bSortingBy2DList==true &&
                   LastSet.iTester==_2D_SORT &&
                   TestIF_File.bSortingBy2DIDList==true)                        //Frank 20221122 : 2DID sorting for ATK
                    Task=115;
                else
                    Task=100;
            }
            break;
        // golden atester.cpp:6229-8650 -- the dense down-press / test / torque /
        // EP / load-cell per-case tree, TRANSLATED FAITHFULLY (W7-T1).  Every case
        // id, iTestHeadMotorTask transition, goto/fall-through/early-return and
        // #ifdef SOFT_SIMULTE / INDEX_PROTECT_TMOVE region is reproduced VERBATIM.
        // MOT[] Z/Y moves (S0-converged) + Suck/Socket bin writes are REAL; the
        // torque(COM2)/EP-DAQ/RTC-vision/cContact-ROI/torque-UI derefs are routed
        // through the W7T1_SEAM stand-ins above (offline-inert).  Entered from the
        // ACTIVE window: case 21 -> 30/60/100/115 and case 9 -> 30000.
        // AI(W7T1-Translate) 20260701.
        case 30:                                                                //Index1移至中間 Index2移至後面
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 30"))
                Task=40;
            break;
        case 40:                                                                //移至下拋高度
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Down_Z2Up;
                Task=50;
            }
            break;
        case 50:                                                                //拋下IC
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==HAS_IC)
                    {
                        FTestSuck.Suck[i][j].On();
                        if(INDEX_SUCKER_TYPE==1)                                //Steven 20111202
                        {
                            fiosetview->bIndexSuck[0][i][j]=true;
                        }
                    }
                    DoTestHeadMotorDelay.SetSecAndOn(0.3);                      // delay 0.3 sec for ic down
                }
            }
            Task=55;
            break;
        case 55:
            if(DoTestHeadMotorDelay.Off())
            {
                bFTestSuckDrop=false;
                Task=1;
            }
            break;
        case 60:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 60"))
                Task=70;
            break;
        case 70:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=80;
            }
            break;
        case 80:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_IC)
                    {
                        BTestSuck.Suck[i][j].On();
                        if(INDEX_SUCKER_TYPE==1)                                //Steven 20111202
                        {
                            fiosetview->bIndexSuck[1][i][j]=true;
                        }
                    }
                    DoTestHeadMotorDelay.SetMSAndOn(100);                       // delay 0.3 sec for ic down
                }
            }
            Task=90;
            break;
        case 90:
            if(DoTestHeadMotorDelay.Off())
            {
                bBTestSuckDrop=false;
                Task=1;
            }
            break;
        case 100:                                                               //Index2 上IC是否有掉落
            if(bRearHeadICFallDown)
            {
                Task=110;
                break;
            }                                                                   //Index1 移至中間 Index2 移至後面

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 100"))
            {
                Task=120;
            }
            break;
        case 110:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 110"))
                Task=112;
            break;
        case 112:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=141;
            }
            break;
        case 115:                                                               //JerryYang 20230322 : 2D SORT模式index arm不用下壓到socket
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 115"))
                Task=116;
            break;
        case 116:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))
            {
                IndexStatus=Z1_Z2_Normal;
                Task=120;
            }
            break;
        case 120:                                                               //初始化馬達扭力及確認
            InitWriteAndCheckMotorTorqueTask();
            W7T1_FMAIN_LBARM0TORQUE->Caption="1:Writing";
            W7T1_FMAIN_LBARM1TORQUE->Caption="---";
            Task=12000;
        case 12000:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            #ifdef SOFT_SIMULTE
                ret=1;
            #else
                ret=COM2->iWriteAndCheckMotorTorque(0, Prod.iMaxPreasure);
            #endif

            if(ret==1)                                                          //扭力設定OK
            {
                iToqureCount=0;
                Task=12100;
            }
            else if(ret==2)                                                     //扭力設定NG
            {
                ShowMyMessage("Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 12000");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 12100:
            Task=12101;
        case 12101:
            iIndexUpPos=0;

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                                                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(0);
                        Task=12300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=12102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=12110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);                                                                                                //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                            bFirstTime=true;
                        }
                    #endif
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedSlow))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(0);
                        Task=12300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=12102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                            bFirstTime=true;
                        }
                        else
                        {
                            Task=12110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);                                                                                                //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                            bFirstTime=true;
                        }
                    #endif
                    ReadTorqueDelay.SetSecAndOn(10);                                                                                                            //kevin 20210824 read torque wait alarm time
                }
            }
            break;
        case 12102:                                                             //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
            if(DoTestHeadMotorDelay.Off())
            {
                Task=12110;
                DoTestHeadMotorDelay.SetSecAndOn(1);
                bFirstTime=true;
            }
            break;
        case 12110:
            W7T1_FMAIN_LBARM0TORQUE->Caption="1:Reading";

            if(DoTestHeadMotorDelay.Off()==false)
            {
                if(bFirstTime)
                {
                    bFirstTime=false;
                    W7T1_FMAIN_CHKREADTORQUE1->Checked=true;
                    W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                    W7T1_FMAIN_EDTORUE0->Text="";
                    COM2->InitReadTorueTask();
                    return;                                                     //kevin 20210824 clean Tourqe
                }
            }

            if(W7T1_FMAIN_EDTORUE0->Text=="")
            {
                if(ReadTorqueDelay.Off())                                       //kevin 20210824 read torque wait alarm time
                {
                    if(COM2->GetReadTorueTask()==999)                           //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等
                    {
                        ShowMyMessage("The test head 1 Motor torque read error", "馬達扭力讀取錯誤", "DoTestHeadMotor 12110");
                        fAllMotorHome=false;
                        Task=1;
                    }
                }
                return;
            }
            else
            {
                 ShowMainScreenPresure(0);                                      //kevin 20210413
            }

            if(W7T1_FMAIN_LBARM0TORQUE->Caption=="1:Reading")                       //kevin 20210413
            {
                InitWriteAndCheckMotorTorqueTask();
                Task=120;
                RecordProcess("Arm1 Torque Reading.");
                return;
            }

            if(IniConfig.bD30EnableSiteModeSelect &&
               (TestIF.iShuttleMode==0 ||
               (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)))             //KEVIN 20150613
            {
                if(IniConfig.bC08_SocketSensor &&
                   TestIF_File.bEnSocketSensor &&
                   TestIF_File.bSocketDisibleinitialcheck==false)               //JerryYang 20170426 (Stven) 修正Disable socket sensor detect
                {
                    str="index check socket sensor down sensor must on, ";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                            if(i>=2 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOff())       //kevin 20150429 : Arm 1在下, 檢查遮斷, 要On
                            {
                                flag=true;
                                bIsSocketSensor=true;
                                str+=IntToStr(i+1);
                            }
                        }
                        else
                        {
//                            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&     //Jimmychiu 20250809 : Mark for Gigas反應檢測功能失效
//                               TestIF_File.bArm1PickPlaceArm2Test==true)
//                            {
                                if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //kevin 20150429 : Arm 1在下, 檢查遮斷, 要On
                                {
                                    flag=true;
                                    bIsSocketSensor=true;
                                    str+=IntToStr(i+1);
                                }
//                            }
//                            else
//                            {
//                                if(TestIF_File.iSensorCheckType[i]==2 &&
//                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
//                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //Arm 1在下, 手臂上沒IC, 只檢查floating sensor必須遮到
//                                {
//                                    flag=true;
//                                    bIsSocketSensor=true;
//                                    str+=IntToStr(i+1);
//                                }
//                            }
                        }
                    }

                    if(flag)
                    {
                        ErrPart=str;                                            //Steven 20220721 : Alarm之前, Index要先讓開
                        Task=12150;
//                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, str);//kevin 20130504 socket sensor
                        return;                                                 //kevin 20150429確認 socket sensor是否正常
                    }
                }
            }
            TorqueData=atoi(W7T1_FMAIN_EDTORUE0->Text.c_str());
            TorqueData=abs(TorqueData);

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
                iMaxPreasure=DeviceForm.iIndexTorqueMax;
            else
                iMaxPreasure=Prod.iMaxPreasure;

            if(CUSTOMER_CODE==CC_KYEC_XILINX &&
               IniConfig.bD01EnableReadTorque &&                                //Frank 20171030 (Steven) add Floting Shuttle Read Torque Delay
               IniConfig.bChangeKitNoHardStop==true &&
               IniConfig.bRemeberAutoHeight==true)
            {
                if(TorqueData>=(IniConfig.dD01ReadTorque+DeviceForm_File.dZ1Torue))
                {
                    ShowMyMessage("The test head 1, contact force over error", "Index 1 壓力過重錯誤", "DoTestHeadMotor 14110");
                    Task=12111;                                                 //kevin 20130418
                    return;
                }
            }
            else
            {
                if(TorqueData>=iMaxPreasure)
                {
                    if(iToqureCount>10)                                         //jou 981130 double check Torque
                    {
                        iToqureCount=0;
                        ShowMyMessage("The test head 1, contact force over error", "Index 1 壓力過重錯誤", "DoTestHeadMotor 12110");
                        Task=12111;                                             //kevin 20130418
                        return ;
                    }
                    else
                    {
                        iToqureCount++;
                        Task=12100;
                        break;
                    }
                }
            }

            ShowMainScreenPresure(0);
            InitWriteAndCheckMotorTorqueTask();

            if(IniConfig.bVTESTFunction==true)                                  //jou 20231102 : VTEST 增加 Torque log
                CheckAndRecodrTorque(0);

            Task=12200;
            break;
        case 12111:                                                             //kevin 20130418
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12111"))
                Task=12112;
            break;
        case 12112:
            ErrPart="The test head 1, contact force over error";                //kevin 20130418
            bIsContactforce=true;                                               //kevin 20130418 contact force over 需開們確認
            if(CosFunction.bIndexAreaOnlyCanUseSkip)                            //Steven 20141105 : Index內的所有異常都只能用Skip
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ1, false, ErrPart);
            else
                ShowErrorMessage("WAR0321", K_RETRY, MTestZ1, false, ErrPart);
            Task=9;
            break;
        case 12150:                                                             //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12150"))
            {
                Task=12151;
            }
            break;
        case 12151:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY))
            {
                Task=12152;
            }
            break;
        case 12152:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, ErrPart);  //kevin 20130504 socket sensor
                Task=9;
            }
            break;
        case 12200:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            ret=COM2->iWriteAndCheckMotorTorque(0, 300);
            if(ret==1)
            {
                W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                Task=12300;
                iSiteCount=0;                                                   //kevin 20190530
            }
            else if(ret==2)
            {
                ShowMyMessage("The test head 1 Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 12200");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 12300:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE
            {
                 Task=12301;
            }
            else if(LastSet.bD41TestSocketICCheckSkip==false)
            {
                if(IniConfig.bSPILFunction==true)                               //jou 2014-08-18 SPIL 關arm 不檢查真空值 //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=121;
                    else
                        Task=130;
                }
                else
                {
                    if(CosFunction.bUseOneByOneIndexCheck==true &&              //Ifor 20180322 : add Use One By One Index Check
                       INDEX_SUCKER_TYPE==1)
                    {
                        if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==true) ||
                           (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==false))            //Above Socket
                        {
                            FTestSuck.ResetAll();
                            iSiteCount=0;
                            IndexCheckOneByOne(true, 0, iSiteCount);            //Ifor 20200617 : add Use One By One Index Check Function 整合
                            Task=50000;
                        }
                        else
                        {
                            Task=121;
                        }
                    }
                    else
                    {
                        Task=121;
                    }
                }
            }
            else
            {
                Task=130;
            }
            break;
        case 12301:
            if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==true) ||
               (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==false))                                                                                                        //Above Socket
            {
                iIndexUpPos=0;
                if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                    IniConfig.bTestIcCheckInContact==true) ||                                                                                                   //kevin 20190708 add
                    (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                     IniConfig.bTestIcCheckInContact==false))                                                                                                   //Above Socket
                {
                    iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                                                                                            //kevin yang 20201014 : 2 --> 1
                }

                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedSlow))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&                                                                                                 //kevin 20190629 回吸檢測一次 4 個 SITE
                       INDEX_SUCKER_TYPE==1)                                                                                                                    //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true, 0, iSiteCount);
                        Task=12305;
                    }
                    else
                    {
                        Task=121;
                    }
                }
                break;
            }
            else
            {
                if(IniConfig.bSPILFunction==true)                                                                                                               //jou 2014-08-18 SPIL 關arm 不檢查真空值 //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=121;
                    else
                        Task=130;
                }
                else
                {
                    Task=12302;
                }
            }
            break;
        case 12302:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true,0,iSiteCount);
                Task=12305;
            }
            else
            {
                Task=121;
            }
            break;
        case 12305:                                                             //kevin 20190531 index check 4 Site
            if(IndexCheck4Site(false, 0, iSiteCount))
            {
                iSiteCount++;

                if(iSiteCount<TestSocket.iShtCol/2)                             //JerryYang 20250120 : modify
                {
                    Task=12301;
                }
                else
                {
                    Task=122100;
                    iSiteCount=0;
                }
            }
            break;
        case 121:
            if(CosFunction.bUseOneByOneIndexCheck==true &&
               INDEX_SUCKER_TYPE==1)                                            //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                FTestSuck.ResetAll();
                iSiteCount=0;
                IndexCheckOneByOne(true, 0, iSiteCount);                        //Ifor 20200617 : add Use One By One Index Check Function 整合
                Task=80000;
                break;
            }

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[0][i][j]==true)
                            {
                                fiosetview->bIndexSuck[0][i][j]=true;
                                bIndexCheckNoStopVaccum=true;
                            }
                        }
                        else
                        {
                            fiosetview->bIndexSuck[0][i][j]=true;
                            bIndexCheckNoStopVaccum=true;
                        }
                    }
                    else
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[0][i][j]==true)
                            {
                                FTestSuck.Suck[i][j].On();
                            }
                        }
                        else
                        {
                            FTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            Task=122100;
            break;
        case 122100:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                W7T1_FIOSET_PISD1();
            }

            if(DoTestHeadMotorDelay.Off())
            {
                Task=122110;
            }
            break;
        case 122110:
            if(INDEX_SUCKER_TYPE==1)                                                                                                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                W7T1_FIOSET_PISD1();
            }

            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                                                                                                    //Steven 20140620 : 整合為Function
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                                                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    if(CUSTOMER_CODE==CC_Greatek)                                                                                                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                                                                                                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下

                   if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                      TotalErrPart!="")                                                                                                                         //kevin 20190629 add error show
                        Task=123;
                    else
                        Task=122;
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedFast))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    if(CUSTOMER_CODE==CC_Greatek)                                                                                                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                                                                                                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下

                    if(CosFunction.bUseOneByOneIndexCheck==true &&
                       ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                                                                                                        //kevin 20190629 add error show
                        Task=123;
                    else if(CosFunction.bUseOneByOneIndexCheck==true &&
                            TotalErrPart!="")                                                                                                                   //KaiChen 20210104:
                        Task=123;
                    else
                        Task=122;
                }
            }
            break;
        case 122:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=W7T1_FIOSET_PISD1();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(DoTestHeadMotorDelay.Off() && bIndexSuckCheck==true)
            {
                bIndexCheckNoStopVaccum=false;                                  //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
                flag=false;
                ErrPart=" ";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        W7T1_CHECKVACINIT(FTestSuck, i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i+IsNNMode()][j];            //Steven 20230712 : 修正NN mode alarm顯示

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                FTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if(flag && LastSet.iRealDummy==REALLY)                          //Steven 20120726 : 有跑IC才檢查Socket
                    Task=123;
                else
                    Task=130;
            }
            break;
        case 123:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 123"))
            {
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=124;
            }
            break;
        case 124:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 124"))
                Task=125;
            break;
        case 125:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                MyDBIProcess("Message", "Piggy Back Fail");                     //JerryYang 20220614 松諭要求新增
                if(CosFunction.bUseOneByOneIndexCheck==true &&
                   ArmSpeed_File[IndexArm].bDevicConfirm)                       //kevin 20190629 add error show
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(CosFunction.bUseOneByOneIndexCheck==true &&
                        INDEX_SUCKER_TYPE==1 &&
                        TotalErrPart!="")                                       //KaiChen 20210104:
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)       //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm1 detect Test Socket has IC error", "Arm 1偵測到Socket有IC殘留!!", "DoTestHeadMotor 125");
                }
                ErrPart="";
                TotalErrPart="";
                Task=1;
            }
            break;
        case 130:
            Task=15000;
            break;
        case 135:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 135"))
                Task=140;
            break;
        case 140:
            InitWriteAndCheckMotorTorqueTask();
            W7T1_FMAIN_LBARM1TORQUE->Caption="2:Writing";
            Task=14000;
        case 14000:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            #ifdef SOFT_SIMULTE
                ret=1;
            #else
                ret=COM2->iWriteAndCheckMotorTorque(1, Prod.iMaxPreasure);
            #endif

            if(ret==1)
            {
                iToqureCount=0;
                Task=14100;
            }
            else if(ret==2)
            {
                ShowMyMessage("The test head 2 Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 14000");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 14100:
            Task=14101;
        case 14101:
            iIndexUpPos=0;

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                                                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))
                {
                    IndexStatus=Z1_Z2_Normal;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(1);
                        Task=14300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&                                                                                                    //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=14102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=14110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);
                            bFirstTime=true;
                        }
                    #endif
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedSlow))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Up_Z2Down;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(1);
                        Task=14300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&                                                                                                    //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=14102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=14110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);
                            bFirstTime=true;
                        }
                    #endif
                    ReadTorqueDelay.SetSecAndOn(10);                                                                                                            //kevin 20210824 read torque wait alarm time
                }
            }
            break;
        case 14102:                                                             //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
            if(DoTestHeadMotorDelay.Off())
            {
                Task=14110;
                DoTestHeadMotorDelay.SetSecAndOn(1);
                bFirstTime=true;
            }
            break;
        case 14110:
            W7T1_FMAIN_LBARM1TORQUE->Caption="2:Reading";

            if(DoTestHeadMotorDelay.Off()==false)
            {
                if(bFirstTime)
                {
                    bFirstTime=false;
                    W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                    W7T1_FMAIN_CHKREADTORQUE2->Checked=true;
                    W7T1_FMAIN_EDTORUE1->Text="";
                    COM2->InitReadTorueTask();
                    bReadMCU2=true;                                             //kevin 20220225 read MCU DATA
                    return;                                                     //kevin 20210824 clean Tourqe
                }
            }

            if(W7T1_FMAIN_EDTORUE1->Text=="")                                       //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等
            {
                if(ReadTorqueDelay.Off())                                       //kevin 20210824 read torque wait alarm time
                {
                    if(COM2->GetReadTorueTask()==999)
                    {
                        ShowMyMessage("The test head 2 Motor torque read error", "馬達扭力讀取錯誤", "DoTestHeadMotor 14110");
                        fAllMotorHome=false;
                        Task=1;
                    }
                }
                return;
            }
            else
            {
                 ShowMainScreenPresure(1);                                      //kevin 20210413
            }

            if(W7T1_FMAIN_LBARM1TORQUE->Caption=="2:Reading")                       //kevin 20210413
            {
                InitWriteAndCheckMotorTorqueTask();
                Task=140;
                RecordProcess("Arm2 Torque Reading.");
                 return;
            }
            //Rear Arm Only
            if(IniConfig.bD30EnableSiteModeSelect &&
               (TestIF.iShuttleMode==0 ||
               (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)))             //KEVIN 20150613
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true)                    //Steven 20200604 : Arm1丟 Arm2測的時候, 只要檢查Arm1
                {
                }
                else if(IniConfig.bC08_SocketSensor &&
                        TestIF_File.bEnSocketSensor &&
                        TestIF_File.bSocketDisibleinitialcheck==false)          //JerryYang 20170426 (Steven) 修正Disable socket sensor detect
                {
                    str="check socket sensor down off,";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                            if(i<=1 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOff())       //kevin 20150429 : Arm 2在下, 檢查遮斷, 要On
                            {
                                flag=true;
                                bIsSocketSensor=true;
                                str+=IntToStr(i+1);
                            }
                        }
                        else
                        {
//                            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&        //Jimmychiu 20250809 : Mark for Gigas反應檢測功能失效
//                               TestIF_File.bArm1PickPlaceArm2Test==true)
//                            {
                                if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //kevin 20150429 : Arm 2在下, 檢查遮斷, 要On
                                {
                                    flag=true;
                                    bIsSocketSensor=true;
                                    str+=IntToStr(i+1);
                                }
//                            }
//                            else
//                            {
//                                if(TestIF_File.iSensorCheckType[i]==2 &&
//                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
//                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //Arm 2在下, 手臂上沒IC, 只檢查floating sensor必須遮到
//                                {
//                                    flag=true;
//                                    bIsSocketSensor=true;
//                                    str+=IntToStr(i+1);
//                                }
//                            }
                        }
                    }

                    if(flag)
                    {
                        ErrPart=str;                                            //Steven 20220721 : Alarm之前, Index要先讓開
                        Task=14150;
//                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ2, false, str);//kevin 20130504 socket sensor
                        return;                                                 //kevin 20150429確認 socket sensor是否正常
                    }
                }
            }
            TorqueData=atoi(W7T1_FMAIN_EDTORUE1->Text.c_str());
            TorqueData=abs(TorqueData);

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
                iMaxPreasure=DeviceForm.iIndexTorqueMax;
            else
                iMaxPreasure=Prod.iMaxPreasure;

            if(CUSTOMER_CODE==CC_KYEC_XILINX &&
               IniConfig.bD01EnableReadTorque &&                                //Frank 20171030 (Steven) add Floting Shuttle Read Torque Delay
               IniConfig.bChangeKitNoHardStop==true &&
               IniConfig.bRemeberAutoHeight==true)
            {
                if(TorqueData>=(IniConfig.dD01ReadTorque+DeviceForm_File.dZ2Torue))
                {
                    ShowMyMessage("The test head 2, contact force over error", "Index 2 壓力過重錯誤", "DoTestHeadMotor 14110");
                    Task=14111;                                                 //kevin 20130418
                    return;
                }
            }
            else
            {
                if(TorqueData>=iMaxPreasure)
                {
                    if(iToqureCount>10)                                         //jou 981130 double check Torque
                    {
                        iToqureCount=0;
                        ShowMyMessage("The test head 2, contact force over error", "Index 2 壓力過重錯誤", "DoTestHeadMotor 14110");
                        Task=14111;                                             //kevin 20130418
                        return;
                    }
                    else
                    {
                        iToqureCount++;
                        Task=14100;
                        break;
                    }
                }
            }

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
            {
                iIndexArm[0]=atoi(W7T1_FMAIN_EDTORUE0->Text.c_str());
                iIndexArm[1]=atoi(W7T1_FMAIN_EDTORUE1->Text.c_str());
                iIndexArm[2]=abs(iIndexArm[0]-iIndexArm[1]);
                if(TestIF_File.iShuttleMode==0 &&                               //Steven 20240716 : 關arm的時候不檢查兩arm的壓力差
                   iIndexArm[2]>DeviceForm.iIndexTorqueCmp)
                {
                    ShowMyMessage("The test head contact force% difference over error", "Index 壓力%差距過大錯誤", "DoTestHeadMotor 14110");
                }
            }

            ShowMainScreenPresure(1);
            InitWriteAndCheckMotorTorqueTask();

            if(IniConfig.bVTESTFunction==true)                                  //jou 20231102 : VTEST 增加 Torque log
                CheckAndRecodrTorque(1);

            Task=14200;
            break;
        case 14111:                                                             //kevin 20130418
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoTestHeadMotor 14111"))
                Task++;
            break;
        case 14112:
            ErrPart="The test head 2, contact force over error";                //kevin 20130418
            bIsContactforce=true;                                               //kevin 20130418 contact force over 需開們確認

            if(CosFunction.bIndexAreaOnlyCanUseSkip)                            //Steven 20141105 : Index內的所有異常都只能用Skip
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ1, false, ErrPart);
            else
                ShowErrorMessage("WAR0321", K_RETRY, MTestZ2, false, ErrPart);

            Task=135;
            break;
        case 14150:                                                             //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoTestHeadMotor 14150"))
            {
                Task=14151;
            }
            break;
        case 14151:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY))
            {
                Task=14152;
            }
            break;
        case 14152:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ2, false, ErrPart);  //kevin 20130504 socket sensor
                Task=9;
            }
            break;
        case 14200:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            ret=COM2->iWriteAndCheckMotorTorque(1, 300);
            if(ret==1)
            {
                W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                Task=14300;
            }
            else if(ret==2)
            {
                ShowMyMessage("Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 14200");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 14300:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 1, iSiteCount);
                Task=14301;
            }
            else if(LastSet.bD41TestSocketICCheckSkip==false)                   //jou 2014-08-18 SPIL 關arm 不檢查真空值
            {
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==1))
                        Task=141;
                    else
                        Task=150;
                }
                else
                {
                    if(CosFunction.bUseOneByOneIndexCheck==true &&              //Ifor 20180322 : add Use One By One Index Check
                       INDEX_SUCKER_TYPE==1)
                    {
                        if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==true) ||
                           (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==false))            //Above Socket
                        {
                            BTestSuck.ResetAll();
                            iSiteCount=0;
                            IndexCheckOneByOne(true, 1, iSiteCount);            //Ifor 20200617 : add Use One By One Index Check Function 整合
                            Task=60000;
                        }
                        else
                        {
                            Task=141;
                        }
                    }
                    else if(ArmSpeed_File[IndexArm].bDevicConfirm &&            //kevin 20190629 回吸檢測一次 4 個 SITE
                            INDEX_SUCKER_TYPE==1)                               //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true,1,iSiteCount);
                        Task=14305;
                    }
                    else
                    {
                        Task=141;
                    }
                }
            }
            else
            {
                Task=150;
            }

            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Task=150;
            }
            break;
        case 14301:
            if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==true) ||
               (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==false))                                                                                                        //Above Socket
            {
                iIndexUpPos=0;
                if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                    IniConfig.bTestIcCheckInContact==true) ||                                                                                                   //kevin 20190708 add
                    (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                     IniConfig.bTestIcCheckInContact==false))                                                                                                   //Above Socket
                {
                    iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);
                }

                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedSlow))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&                                                                                                 //kevin 20190629 回吸檢測一次 4 個 SITE
                       INDEX_SUCKER_TYPE==1)                                                                                                                    //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true,0,iSiteCount);
                        Task=14305;
                    }
                    else
                    {
                        Task=141;
                    }
                }
                break;
            }
            else
            {
                if(IniConfig.bSPILFunction==true)                                                                                                               //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||                                                                                                           //jou 2014-08-18 SPIL 關arm 不檢查真空值
                       (TestIF_File.iShuttleMode==1  &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=141;
                    else
                        Task=150;
                }
                else
                {
                    Task=14302;
                }
            }
            break;
        case 14302:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 0, iSiteCount);
                Task=14305;
            }
            else
            {
                Task=141;
            }
            break;
        case 14305:                                                             //kevin 20190531 index check 4 Site
            if(IndexCheck4Site(false, 1, iSiteCount))
            {
                iSiteCount++;

                if(iSiteCount<TestSocket.iShtCol/2)                             //JerryYang 20250120 : modify
                    Task=14301;
                else
                    Task=142100;
            }
            break;
        case 141:
            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) &&                    //Steven 20150129 : 需要確認Arm2有沒有粘料
                TestIF_File.bCheckArm2Vacuum==false)                            //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Task=150;
                break;
            }

            if(CosFunction.bUseOneByOneIndexCheck==true &&
               INDEX_SUCKER_TYPE==1)                                            //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                BTestSuck.ResetAll();
                iSiteCount=0;
                IndexCheckOneByOne(true, 1, iSiteCount);                        //Ifor 20200617 : add Use One By One Index Check Function 整合
                Task=80010;
                break;
            }

            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[1][i][j]==true)
                            {
                                fiosetview->bIndexSuck[1][i][j]=true;
                                bIndexCheckNoStopVaccum=true;
                            }
                        }
                        else
                        {
                            fiosetview->bIndexSuck[1][i][j]=true;
                            bIndexCheckNoStopVaccum=true;
                        }
                    }
                    else
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[1][i][j]==true)
                            {
                                BTestSuck.Suck[i][j].On();
                            }
                        }
                        else
                        {
                            BTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            Task=142100;
            break;
        case 142100:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                W7T1_FIOSET_PISD2();
            }

            if(DoTestHeadMotorDelay.Off())
                Task=142110;
            break;
        case 142110:
            if(INDEX_SUCKER_TYPE==1)                                                                                                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                W7T1_FIOSET_PISD2();
            }

            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                                                                                                    //Steven 20140620 : 整合為Function
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                                                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    if(CUSTOMER_CODE==CC_Greatek)                                                                                                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                                                                                                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下

                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                                                                                                        //kevin 20190629 add error show
                        Task=143;
                    else
                        Task=142;
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedFast))                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Up_Z2Down;                                                                                                                    //kevin 20190118 add index z states
                    if(CUSTOMER_CODE==CC_Greatek)                                                                                                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                                                                                                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下

                    if(CosFunction.bUseOneByOneIndexCheck==true &&
                       ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                                                                                                        //kevin 20190629 add error show
                        Task=143;
                    else if(CosFunction.bUseOneByOneIndexCheck==true &&
                            TotalErrPart!="")                                                                                                                   //KaiChen 20210104:
                        Task=143;
                    else
                        Task=142;
                }
            }
            break;
        case 142:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=W7T1_FIOSET_PISD2();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(DoTestHeadMotorDelay.Off() && bIndexSuckCheck==true)
            {
                bIndexCheckNoStopVaccum=false;                                  //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
                flag=false;
                ErrPart=" ";
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        W7T1_CHECKVACINIT(BTestSuck, i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i][j];

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                BTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if(flag && LastSet.iRealDummy==REALLY)                          //Steven 20120726 : 有跑IC才檢查Socket
                    Task=143;
                else
                    Task=150;
            }
            break;
        case 143:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 143"))
            {
                IndexStatus=Z1_Z2_Normal;                                       //kevin 20190118 add index z states
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=144;
            }
            break;
        case 144:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 144"))
                Task=145;
            break;
        case 145:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(CosFunction.bUseOneByOneIndexCheck==true &&
                   ArmSpeed_File[IndexArm].bDevicConfirm)                       //kevin 20190629 add error show
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(CosFunction.bUseOneByOneIndexCheck==true &&
                        INDEX_SUCKER_TYPE==1 &&
                        TotalErrPart!="")                                       //KaiChen 20210104:
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, TotalErrPart);
                }
                else if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)       //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm2 detect Test Socket has IC error", "Arm 2偵測到Socket有IC殘留!!", "DoTestHeadMotor 145");
                }
                ErrPart="";

                Task=1;
            }
            break;
        case 150:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 150"))
            {
                 IndexStatus=Z1_Z2_Normal;                                                                                                                      //kevin 20190118 add index states

                if(IniConfig.bUseAutoSiteMapping &&                                                                                                             //jou 2011-03-24 start : Auto Site Mapping
                   (CosFunction.bUSEJCETSiteMapMode==false ||                                                                                                   //jou 2016-10-28 JCET 要求Site Mapping 必須測試到pass bin才能通過
                    CosFunction.bAutoSiteMappingSetOpenBIN==true))                                                                                              //jou 20230216 : 修正 bAutoSiteMappingSetOpenBIN 失效
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
                    {
                        if(CosFunction.bAutoSiteMappingSetOpenBIN==true)                                                                                        //jou 20200928 : Auto Site Mapping Set Open BIN
                        {
                            bGetOpenBin=false;
                            Prod.iOpenBin=TestIF_File.iOpenBin;
                            W7T1_FMAIN_SETOPENBIN();
                            Task=1500;
                        }
                        else
                        {
                            flag=true;
                            for(int i=0; i<BTestSuck.iShtRow; i++)
                            {
                                for(int j=0; j<BTestSuck.iShtCol; j++)
                                {
                                    if(flag==true ||
                                       IniConfig.bI21ASMNeedCheckEachSiteOpen)                                                                                  //Steven 20120726 : AutoSiteMapping, 當確認Open Bin時,同時也要檢查是不是所有Dut都Open
                                    {
                                        if(IsNNMode()==NN_1Row)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true)
                                            {
                                                if(i<1)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i-1, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                        else if(IsNNMode()==NN_2Row)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true)
                                            {
                                                if(i<2)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i-2, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                        else
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true ||                                                                         //ChungHung 20130910 alter for SCK can close site by Index
                                               LastSet.bUseTestSocket[1][i][j]==true)
                                            {
                                                if(TestIF_File.iShuttleMode==1 &&                                                                               //Ifor 20171005 (Steven) : add 避免Auto Site mapping 關ARM1 導致Hangup
                                                   TestIF_File.iShuttle_Sel==1)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);                                        //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                    }
                                }
                            }

                            if(IsNNMode()==NN_2Row)
                            {
                                InitTestSuckTestIC_TwoArm32Site_Task();
                                Task=170;
                            }
                            else
                            {
                                if(TestIF_File.iShuttleMode==1 &&
                                   TestIF_File.iShuttle_Sel==1)                                                                                                 //jou 20200827 : 修正auto site mapping 只開arm2異常
                                    InitBTestSuckTestICTask();
                                else
                                    InitFTestSuckTestICTask();
                                Task=160;
                            }

                            bGetOpenBin=true;
                            Prod.iOpenBin=0;                                                                                                                    //kevin 20160513
                        }
                        RecordProcess("Auto Site Map Start");                                                                                                   //kevin 20160125
                        break;
                    }
                    else
                    {
                        Task=1500;
                    }
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 160:
            if(TestIF_File.iShuttleMode==1 &&
               TestIF_File.iShuttle_Sel==1)                                     //kevin 20160414 add 只使用ARM2 Auto SiteMap
            {
                 if(DoBTestSuckTestIC())
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            BTestSuck.Item[i][j]=NULL_IC;
                        }
                    }
                    Task=1500;
                    bGetOpenBin=false;
                }
            }
            else
            {
                if(DoFTestSuckTestIC())
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Item[i][j]=NULL_IC;
                        }
                    }
                    Task=1500;
                    bGetOpenBin=false;
                }
            }
            break;
        case 170:                                                               //Steven 20140512: For HT-9047
            if(DoTestSuckTestIC_TwoArm32Site())
            {
                FTestSuck.SetAllToNullIC();
                BTestSuck.SetAllToNullIC();
                Task=1500;
                bGetOpenBin=false;
            }
            break;
        case 1500:
            if(bUseTwoArm32Site==true)
                Task=1650;
            else
                Task=1550;
            break;
        case 1530:                                                              //kevin 20131112
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 1530"))
            {
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }

                    if(CosFunction.bInitialStartDelayCount_Init)                //JerryYang 20250120 : modify
                    {
                    }
                    else
                    {
                        iInitStartDelayTimeCT=0;                                //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                    }
                    iInitStartDelayTimeDetCount=0;                              //kevin 20161214 (Steven) 等待Delay time 加減時間
                    Task=1535;
                }
                else
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }
                    Task=1535;
                }
            }
            break;
        case 1535:                                                              //kevin 20131112 add
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
            }

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                bOneTimeFlag=true;
                IndexStatus=Z1Up_Z2Down;
                bRearHeadICFallDown=false;
                InitTestYTask();
                bCanNotDisableOneCycle=false;
                W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                if(bInitialStartIndexCheckDone==false)                          //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                    bInitialStartIndexCheckDone=true;
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(bOneTimeWait==false || iOneCycle)
                    {
                        if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                            Task=1700;                                          //if tested half do home，need check arm is has device
                        else
                            Task=1720;

                        bInitialSackTime=true;                                  //kevin 20131112 第一次吸取ic等待時間
                        DoTestHeadMotorDelay2.SetSecAndOn(1);                   //kevin 20131112 不等待
                        if(CosFunction.bInitialStartDelayCount_Init)            //JerryYang 20250120 : modify
                        {
                        }
                        else
                        {
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                        }
                        iInitStartDelayTimeDetCount=0;                          //kevin 20161214 (Steven) 等待Delay time 加減時間
                    }
                }
                else
                {
                    Task=600;                                                   // 到此為止 必須 Z1 Down Z2 Up;
                }
            }
            break;
        case 1550:
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1550"))
                {
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(Temperature.bZ2DownSocket)                           //Steven 20140827 : 第一次吸取ic等待時間
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }

                            if(CosFunction.bInitialStartDelayCount_Init)        //JerryYang 20250120 : modify
                            {
                            }
                            else
                            {
                                iInitStartDelayTimeCT=0;                        //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            }
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                        else if(bOneTimeWait==false || iOneCycle)               //kevin 20131112 tem
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                    }
                    else
                    {
                        if(DeviceForm.bShuttleWaitingOutSiteChamber)
                        {
                            MOT[MInShuttle1].fCanMoveM=false;
                            MOT[MInShuttle2].fCanMoveM=false;
                            fFrontNeedTest=true;
                        }
                        Task=1600;
                    }
                }
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1550"))
                {
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;

                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(Temperature.bZ2DownSocket)                           //Steven 20140827 : 第一次吸取ic等待時間
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                        else if(bOneTimeWait==false || iOneCycle)               //kevin 20131112 tem
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                    }
                    else
                    {
                        if(DeviceForm.bShuttleWaitingOutSiteChamber)
                        {
                            MOT[MInShuttle1].fCanMoveM=false;
                            MOT[MInShuttle2].fCanMoveM=false;
                            fFrontNeedTest=true;
                        }
                        Task=1600;
                    }
                }
            }
            break;
        case 1600:
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
            }

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                                                                    //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 1600"))
                {
                    bOneTimeFlag=true;
                    IndexStatus=Z1_Z2_Normal;
                    bRearHeadICFallDown=false;
                    InitTestYTask();
                    bCanNotDisableOneCycle=false;
                    W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                    W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                    if(bInitialStartIndexCheckDone==false)                                                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(bOneTimeWait==false || iOneCycle)
                        {
                            if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                                Task=1700;                                                                              //if tested half do home，need check arm is has device
                            else
                                Task=1720;

                            if(Temperature.iInitialStart1Time!=0)
                                bInitialSackTime=true;                                                                  //kevin 20131112 第一次吸取ic等待時間
                            DoTestHeadMotorDelay2.SetSecAndOn(1);                                                       //kevin 20131112 不等待
                            if(CosFunction.bInitialStartDelayCount_Init)                                                //JerryYang 20250120 : modify
                            {
                            }
                            else
                            {
                                iInitStartDelayTimeCT=0;                                                                //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            }

                            iInitStartDelayTimeDetCount=0;                                                              //kevin 20161214 (Steven) 等待Delay time 加減時間
                        }
                    }
                    else
                    {
                        Task=600;                                                                                       // 到此為止 必須 Z1 Down Z2 Up;
                    }
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))                     //Marc
                {
                    bOneTimeFlag=true;
                    IndexStatus=Z1Down_Z2Up;
                    bRearHeadICFallDown=false;
                    InitTestYTask();
                    bCanNotDisableOneCycle=false;
                    W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
                    W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
                    if(bInitialStartIndexCheckDone==false)                                                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(bOneTimeWait==false || iOneCycle)
                        {
                            if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                                Task=1700;                                                                              //if tested half do home，need check arm is has device
                            else
                                Task=1720;

                            if(Temperature.iInitialStart1Time!=0)
                                bInitialSackTime=true;                                                                  //kevin 20131112 第一次吸取ic等待時間
                            DoTestHeadMotorDelay2.SetSecAndOn(1);                                                       //kevin 20131112 不等待
                            iInitStartDelayTimeCT=0;                                                                    //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                                                              //kevin 20161214 (Steven) 等待Delay time 加減時間
                        }
                    }
                    else
                    {
                        Task=600;                                                                                       // 到此為止 必須 Z1 Down Z2 Up;
                    }
                }
            }
            break;
        case 1650:                                                              //2013-01-15    Dell    Add nn Mode
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1650"))
            {
                Task=1660;
            }
            break;
        case 1660:
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
                iBackUpZ2DownPosition=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset;
            }

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
                return;
            }
            #endif

            if(MOT[MTestY1].Gali_nnMode_Z1Z2_Down(iSpeedY, TMode))
            {
                Task=1670;
            }
            break;
        case 1670:
            if(bInitialStartIndexCheckDone==false)                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                bInitialStartIndexCheckDone=true;
            if(LastSet.iTemperature==Tempture_Hot)
            {
                if(bOneTimeWait==false || iOneCycle)
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }
                    Task=1680;
                }
            }
            else
            {
                if(DeviceForm.bShuttleWaitingOutSiteChamber)
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                    fFrontNeedTest=true;
                }
                Task=1680;
            }

            if(Prod.bUseSocketHeating==true && bUseSocketHeating_Wait==false)   //Ztex 2024.09.07 Add Use Socket Heating
            {
                iInitStartDelayTimeCT=0;                                        //Ztex 2024.09.06 Add 4x4 Mode Clear iInitStartDelayTimeCT
                iInitStartDelayTimeDetCount=0;                                  //Ztex 2024.09.06 Add 4x4 Mode Clear iInitStartDelayTimeCT
                DoUseSocketHeating.SetSecAndOn(Prod.iUseSocketHeating);
                DoUseSocketHeating.LatchCycleTime(true);
                bUseSocketHeating_Wait=true;
                Task=1675;
            }
            break;
        case 1675:                                                              //Ztex 2024.09.07 Add Use Socket Heating
            iUseSocketHeating_Time=DoUseSocketHeating.LatchCycleTime()/1000;
            W7T1_FMAIN_LABUSER->Caption=AnsiString(iUseSocketHeating_Time);
            if(DoUseSocketHeating.Off()==true)
            {
                W7T1_FMAIN_LABUSER->Caption="User";
                bDoWhenPressStopOverUseInitialDelay=false;
                Task=1680;
            }
            break;
        case 1680:
            bOneTimeFlag=true;
            IndexStatus=Z1_Z2_Down;
            bRearHeadICFallDown=false;
            InitTestYTask();
            bCanNotDisableOneCycle=false;
            W7T1_FMAIN_CHKREADTORQUE1->Checked=false;
            W7T1_FMAIN_CHKREADTORQUE2->Checked=false;
            if(bInitialStartIndexCheckDone==false)                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                bInitialStartIndexCheckDone=true;
            if(LastSet.iTemperature==Tempture_Hot)
            {
                if(bOneTimeWait==false || iOneCycle)
                {
                    if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                        Task=1700;                                              //if tested half do home，need check arm is has device
                    else
                        Task=1720;
                    if(Temperature.iInitialStart1Time!=0)
                        bInitialSackTime=true;                                  //kevin 20131112 第一次吸取ic等待時間
                    DoTestHeadMotorDelay2.SetSecAndOn(1);                       //kevin 20131112 不等待
                }
            }
            else
            {
                Task=600;                                                       // 到此為止 必須 Z1 Z2 Up ;
            }
            break;
        case 1700:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    FTestSuck.Suck[i][j].On();
                    if(INDEX_SUCKER_TYPE==1)                                    //Steven 20111202
                    {
                        fiosetview->bIndexSuck[0][i][j]=true;
                    }
                }
            }

            DoTestHeadMotorDelay.SetMSAndOn(150);
            Task=1710;
            break;
        case 1710:
            if(DoTestHeadMotorDelay.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        W7T1_CHECKVACINIT(FTestSuck, i, j, flag);
                    }
                }
                Task=1720;
            }
            break;
        case 1720:
            if(DoTestHeadMotorDelay2.Off())
            {
                if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    MOT[MInShuttle1].fCanMoveM=true;

                Task=600;
            }
            break;
        case 600:
            DoTestY();
            break;
        case 15000:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 15000"))
            {
                IndexStatus=Z1_Z2_Normal;                                       //kevin 20190118 add
                Task=15100;
            }
            break;
        case 15100:
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                Task=140;
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 15100"))
                    Task=140;
            }
            break;
        case 20000:
            Task=20100;
            break;
        case 20100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 20100"))
                Task=20200;
            break;
        case 20200:
            if(FrontTestHeadHasIC())
            {
                Task=20300;
            }
            else if(RearTestHeadHasIC())
            {
                Task=21000;
            }
            else
            {
                bPlaceToShuttleFirst=false;
                Task=20210;
                startTick=MyTickCount();
                endTick=MyTickCount();
            }
            break;
        case 20210:
            nowTick=MyTickCount();
            if((nowTick-endTick)>1000)
            {
                startTick=MyTickCount();
                endTick=MyTickCount();
            }
            else
            {
                endTick=MyTickCount();
                if((endTick-startTick)>2000)                                    //Steven 20190304 : 20000 --> 2000
                {
                    Task=1;
                }
            }

            if(ShuttleHasIC()==false &&
               MOT[MInShuttle1].fCanMoveR==true &&
               MOT[MInShuttle2].fCanMoveR==true)
            {
                SetInitialICCheck();
                Task=1;
            }
            break;
        case 20300:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 20300"))
            {
                Task=20400;
                iFrontTestDestroyICTask=1;
            }
            break;
        case 20400:
            sp=GetIndexZSpeed(1);                                               //Steven 20160524 : Index Z軸速度整合為Function
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, sp))
                Task=20500;
            break;
        case 20500:
            if(DoFrontTestDestroyIC(false))
                Task=20100;
            break;
        case 21000:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 21000"))
            {
                iRearTestDestroyICTask=1;
                Task=21400;
            }
            break;
        case 21400:
            sp=GetIndexZSpeed(0);                                               //Steven 20160524 : Index Z軸速度整合為Function
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe,sp))
                Task=21500;
            break;
       case 21500:
            if(DoRearTestDestroyIC(false))
                Task=20100;
            break;
       case 30000:
            if(IndexEveryTimeCheckEP())                                                                                                                         //jou 2011-04-28 start : Index每一次都確認EP是否有充飽氣。
            {
                Task=10;
            }
            break;
        case iCASE_REAL_CCD2:                                                                                                                                   //Steven 20120222 Start: Index Check時,如果有IC,可以Skip重作ROI或是Retry再檢查一次, 以下整段換位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor iCASE_REAL_CCD2"))
            {
                Task=iCASE_REAL_CCD3;
            }
            break;
        case iCASE_REAL_CCD3:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor iCASE_REAL_CCD3"))
            {
                if((IniConfig.bHaveRTCCheckSiteMap &&
                    IniConfig.bD35RTCCheckSiteMap) ||                                                                                                           //Steven 20140513
                   (CosFunction.bRTCAutoModelVerify==true &&
                    IniConfig.bD36EnableRTCAutoModelVerify==true))                                                                                              //JerryYang 20230324 : 有開Auto verify要傳map給RTC
                {
                    Task=40310;
                }
                else
                {
                    Task=40330;
                }
            }
            break;
        case 40310:
            if(SendSiteMapToRTC(true, 0)!=-1)                                                                                                                   //jou 2014-05-06 add RTC site map
            {
                bDoFRTCAutoModelVerify=false;
                bDoBRTCAutoModelVerify=false;
                DoTestHeadMotorDelay.SetSecAndOn(10);
                Task=40320;
            }
            else
            {
                Task=40330;
            }
            break;
        case 40320:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtSiteMap])
            {
                Task=40330;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())                                                                                                                 //Steven 20161206 (wei) : Add if
                    ShowMyMessage("RTC Site Map Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 40330:
            if(bTriggerRTC_AutoSTD)                                                                                                                             //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD, 不要做Full view check
            {
                ret=ShowMyMessageBox_YES_NO("即將執行RTC Calibration model流程, 請確認Socket中沒有IC或異物", "Ready to do RTC Calibration model process, please check there is no abnormail object in socket!");
                if(ret!=1)
                {
                    break;
                }

                Task=iCASE_REAL_CCD6;
                W7T1_FCONTACT_INITROI();
            }
            else
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                COM2->SendCommToVision(COM2->rtFullTOK, true);
                DoTestHeadMotorDelay.SetSecAndOn(10);
                Task=iCASE_REAL_CCD4;
            }
            break;
        case iCASE_REAL_CCD4:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->DoReleaseAndInspEnd();
                bRTCFullViewError=false;                                                                                                                        //Steven 20120206 : RTC重複錯誤
                if(CosFunction.bIndexCheckCanTurnOff &&                                                                                                         //Isaac 20211019 : 可選擇做index check的時機
                  ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
                   (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
                    IniConfig.iD71IndexCheckOnOffMode==2))
                {
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                                                                                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                                                                                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(IniConfig.bD55DisableIndexCheck &&
                        REAL_TIME_CCD==true &&
                        COM2->bCCDDummyRum==false)                                                                                                              //Steven 20150723 : Fixed for SCK
                {
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                                                                                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                                                                                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(CosFunction.bAfterAutoCleanNoIndexCheck &&                                                                                              //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
                        (IniConfig.iD69IndexCheckModeForAutoClean==1 ||
                         IniConfig.iD69IndexCheckModeForAutoClean==2) &&
                        bAutoCleanFinishOnlyUseRTC==true)
                {
                    bAutoCleanFinishOnlyUseRTC=false;
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                                                                                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                                                                                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(CosFunction.bBeforeAutoCleanOnlyUseRTC==true &&                                                                                         //JerryYang 20161216 (Steven) auto clean的前後只靠RTC來檢查socket,不做index下壓至socket吸真空
                        bAutoCleanFinishOnlyUseRTC==true &&
                        REAL_TIME_CCD==true &&
                        COM2->bCCDDummyRum==false)
                {
                    bAutoCleanFinishOnlyUseRTC=false;
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                                                                                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                                                                                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden ||                                                                                   //Sam 20240117 : OneCycle 完成做完 Full view check 後都需要做 RTC Learning golden
                        bTriggerRTC_AutoSTD)                                                                                                                    //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
                {
                    Task=iCASE_REAL_CCD6;
                    W7T1_FCONTACT_INITROI();
                }
                else
                {
                    Task=9;                                                                                                                                     //2013-01-15    Dell
                }
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                COM2->DoReleaseAndInspEnd();
                Task=iCASE_REAL_CCD5;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())                                                                                                                 //ChungHung 20121005 add
                    ShowErrorMessage("WAR0337", 0, MMIndex, 0, __FUNC__);                                                                                       //RTC FullT Time Out Error!
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case iCASE_REAL_CCD5:
            if(IndexAlarmInArmAway()==true)                                                                                                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)                                                                                            //Steven 20151022 : add for MAXIM
                    bIsTestSitICFallDown=true;

                COM2->DoReleaseAndInspEnd();
                if(CosFunction.bRTCFullViewErrorOnlyRetry)                                                                                                      //Steven 20150304 : bRTCFullViewErrorOnlyRetry預設改為True
                    ret=ShowErrorMessage("WAR0343", K_RETRY, MMCCD, bRTCFullViewError, ErrPart);                                                                //RTC FullView Index Check Error!
                else
                    ret=ShowErrorMessage("WAR0343", K_RETRY|K_SKIP, MMCCD, bRTCFullViewError, ErrPart);                                                         //RTC FullView Index Check Error!
                bRTCFullViewError=true;                                                                                                                         //Steven 20120206 : RTC重複錯誤

                if(ret==K_RETRY)
                {
                    Task=1;
                }
                else
                {
                    Task=iCASE_REAL_CCD6;
                    W7T1_FCONTACT_INITROI();
                    bRTCFullViewError=false;
                }
            }
            break;
        case iCASE_REAL_CCD6:
            if(W7T1_FCONTACT_DOROI(true))                                                                                                                  //ChungHung 20121011 鉬FullView 謖IndexCheck
            {
                Task=9;
            }
            break;
        case 50000:                                                                                                                                             //Index 1 至 Sock 吸料
            if(CUSTOMER_CODE==CC_ASE_CL &&                                                                                                                      //JerryYang 20250120 : modify
              ((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
              (LastSet.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false)))                                                         //Above Socket
            {
                iContactZ=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset-iIndexArmCheck_SG_Arm1-Offset.iIndexArmContact[0];                                           //JerryYang 20250822 : Norman要求不要加offset
            }
            else
            {
                iContactZ=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset-iIndexArmCheck_SG_Arm1;
            }

            if(MOT[MTestZ1].Gali_MotMove(iContactZ, iSpeedSlow))                                                                                                //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(IndexCheckOneByOne(false, 0, iSiteCount, true))                                                                                              //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=50100;
                }
            }
            break;
        case 50100:                                                                                                                                             //Index 1 至 Drop 位置確認真空
            //W7T1_FIOSET_PISD1();

            if(CUSTOMER_CODE==CC_ASE_CL &&                                                                                                                      //JerryYang 20250120 : modify
              ((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
              (LastSet.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false)))                                                         //Above Socket
            {
                iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                                                                                                //Steven 20140620 : 整合為Function
                iIndexUpPos-=Offset.iIndexArmContact[0];
            }
            else
            {
                iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);
            }

             if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedFast))                             //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                Task=50110;
                CheckSocketHasICDelay.SetSecAndOn(1.0);                                                                                                         //JerryYang 20250807 : 0.5-> 1 sec
            }
            break;
        case 50110:
            if(CheckSocketHasICDelay.Off())
            {
                if(IndexCheckOneByOne(false, 0, iSiteCount))                                                                                                    //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=50000;
                    iSiteCount++;
                    if(iSiteCount>=(FTestSuck.iShtCnt))
                    {
                        iSiteCount=0;
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下
                        //Task=122;
                        Task=122100;
                    }
                }
            }
            break;
        case 60000:                                                                                                                                             //Index 2  至 Sock 吸料

            if(CUSTOMER_CODE==CC_ASE_CL &&                                                                                                                      //JerryYang 20250120 : modify
              ((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
              (LastSet.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false)))                                                         //Above Socket
            {
                iContactZ=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset-iIndexArmCheck_SG_Arm2-Offset.iIndexArmContact[1];                                           //JerryYang 20250822 : Norman要求不要加offset
            }
            else
            {
                iContactZ=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset-iIndexArmCheck_SG_Arm2;
            }

            if(MOT[MTestZ2].Gali_MotMove(iContactZ, iSpeedSlow))                                                                                                //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(IndexCheckOneByOne(false, 1, iSiteCount, true))                                                                                              //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=60100;
                }
            }
            break;
        case 60100:                                                                                                                                             //Index 2  至 Sock 吸料
            if(CUSTOMER_CODE==CC_ASE_CL &&                                                                                                                      //JerryYang 20250120 : modify
              ((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
              (LastSet.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false)))                                                         //Above Socket
            {
                iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                                                                                                //Steven 20140620 : 整合為Function
                iIndexUpPos-=Offset.iIndexArmContact[1];
            }
            else
            {
                iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                                                                                                //Steven 20140620 : 整合為Function
            }

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedFast))
            {
                Task=60110;
                CheckSocketHasICDelay.SetSecAndOn(1.0);                                                                                                         //JerryYang 20250807 : 0.5-> 1 sec
            }
            break;
        case 60110:
            if(CheckSocketHasICDelay.Off())
            {
                if(IndexCheckOneByOne(false, 1, iSiteCount))                                                                                                    //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=60000;
                    iSiteCount++;
                    if(iSiteCount>=FTestSuck.iShtCnt)
                    {
                        iSiteCount=0;
                        Task=142100;
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                                                                                                  //Steven 20110908 : 上來後也要Delay一下
                    }
                }
            }
            break;
        case 80000:                                                                                                                                             //Ifor 20200617 : add Use One By One Index Check Function 整合
            if(IndexCheckOneByOne(false, 0, iSiteCount))                                                                                                        //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                iSiteCount++;
                if(iSiteCount>=FTestSuck.iShtCnt)
                {
                    Task=122100;
                }
            }
            break;
        case 80010:                                                                                                                                             //Ifor 20200617 : add Use One By One Index Check Function 整合
            if(IndexCheckOneByOne(false, 1, iSiteCount))                                                                                                        //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                iSiteCount++;
                if(iSiteCount>=FTestSuck.iShtCnt)
                {
                    Task=142100;
                }
            }
            break;

        default:
            // offline-safe terminal: any gated case id parks here (golden would
            // have run the down-press tree).  Keep the cursor; no hardware act.
            (void)ret;
            break;
    }
}
//------------------------------------------------------------------------------
//  DoTesterSidePush (golden :8656-8684) -- Richard 渠梁 side-push.  MOT[]/cylin.
//------------------------------------------------------------------------------
int DoTesterSidePush(bool bInitial)                                             //Richard 20220321 : 渠梁Side Push
{
#if 0 // TODO(W7) -- golden :8656-8684 (MOT[]/cylinder side-push)
#endif
    (void)bInitial;
    return 0;                                                                  // golden default
}
//------------------------------------------------------------------------------
//  DoAllPassVerifyRTC (golden :8685-8817) -- RTC auto model verify SM.
//------------------------------------------------------------------------------
bool DoAllPassVerifyRTC(bool bInitial)                                          //jou 2014-06-24 RTC 自動進行Model驗證
{
#if 0 // TODO(W7) -- golden :8685-8817 (RTC verify SM, COM2/RTC)
#endif
    (void)bInitial;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
//  DoSetupTest (golden :8818-9001) -- setup-test SM.  MOT[]/Suck/Socket.
//------------------------------------------------------------------------------
bool DoSetupTest(int iContactArm)
{
#if 0 // TODO(W7) -- golden :8818-9001 (MOT[]/Suck setup-test SM)
#endif
    (void)iContactArm;
    (void)iSetupTask;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
//  GetTempICResult (golden :9002-9150) -- IC temperature measure SM.
//------------------------------------------------------------------------------
bool GetTempICResult()                                                          //kevin 20130808 IC量測溫度
{
#if 0 // TODO(W7) -- golden :9002-9150 (temperature measure SM)
#endif
    (void)iTempICTask;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
void InitIndexEveryTimeCheckEP()
{
    iIndexEveryTimeCheckEPTask=1;
}
//------------------------------------------------------------------------------
//  IndexEveryTimeCheckEP (golden :9158-9265) -- EP-balloon pressure check SM.
//  ACTIVE: case 1 early-out (when NOT REALLY or EP not installed -> return true)
//  reproduced VERBATIM; the case 100/200/300 EP-pressure body (Gali Z move +
//  ADAM DAQ) reproduced over Sim HAL + ADAM_* shims; the SOFT_SIMULTE-gated
//  alarm tree is reproduced verbatim (SOFT_SIMULTE not defined -> #else live).
//  AI(W7T1-Integrate) 20260701: restored the golden `bool IndexEveryTimeCheckEP()` signature
//  (golden atester.cpp:9158).  The un-gated down-press tree now calls it on a LIVE path as
//  `if(IndexEveryTimeCheckEP())` (case 30000, golden atester.cpp:8331), so the W6.4 void wrapper
//  (added while the only caller was gated) is dropped and the bool body carries the golden name.
//------------------------------------------------------------------------------
TQPF_Timer CheckEPTimer;
TQPF_Timer ReleaseEPTimer;
bool IndexEveryTimeCheckEP()
{
    #ifndef SOFT_SIMULTE
    int ret=0;
    bool bAdamAlarm[2]={false, false};
    #endif
    int &Task=iIndexEveryTimeCheckEPTask;

    switch(Task)
    {
        case 1:
            if(LastSet.iRealDummy==REALLY && (EP_Install==3 || EP_Install==5))  //JerryYang 20171030 (wei) fix D24 EP check功能無效問題
            {
                bIndexEveryTimeCheckEPing=true;
                Task=100;
            }
            else
            {
                return true;
            }
            break;
        case 100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, "IndexEveryTimeCheckEP 100"))
            {
                ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);            //ChungHung 20140508 有些氣球會破
                CheckEPTimer.SetSecAndOn(5);                                    //Steven 20240618 : 3 --> 5
                Task=200;
            }
            break;
        case 200:
            ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);                                                        //ChungHung 20140508 有些氣球會破
            #ifdef SOFT_SIMULTE
                return true;
            #else
                if(CheckEPTimer.Off())
                {
                    if((EP_Install==3 || EP_Install==5))                                                                //20111217 ChungHung
                    {
                        ADAM_Rang(IniConfig.iD26EPEncoderRange);
                        if(EP_Install==5)
                        {
                            bAdamAlarm[0]=ADAM_Alarm(0);
                            bAdamAlarm[1]=ADAM_Alarm(1);
                        }
                        else
                        {
                            bAdamAlarm[0]=ADAM_Alarm();
                            bAdamAlarm[1]=false;
                        }

                        if(bAdamAlarm[0]==false && bAdamAlarm[1]==false)
                        {
                            ADAM_WriteVoltage(DeviceForm.dPress);
                            bIndexEveryTimeCheckEPing=false;
                            ReleaseEPTimer.SetSecAndOn(1);                                                              //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
                            Task=300;
                        }
                        else
                        {
                            ret=ShowErrorMessage("WAR1605", K_RETRY|K_SKIP, 0, MMSystem, "IndexEveryTimeCheckEP_200");  //"請檢查EP是否漏氣!"
                            if(ret==K_RETRY)
                            {
                                Task=1;
                            }
                            else
                            {
                                bIndexEveryTimeCheckEPing=false;
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if(Sen[SnEPAlarm].IsOff()==false)                                                               //9045 lan=0 ip=6 port=3 bit=1
                        {
                            ADAM_WriteVoltage(DeviceForm.dPress);
                            bIndexEveryTimeCheckEPing=false;
                            ReleaseEPTimer.SetSecAndOn(1);                                                              //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
                            Task=300;
                        }
                        else
                        {
                            ret=ShowErrorMessage("WAR1605", K_RETRY|K_SKIP, 0, MMSystem, "IndexEveryTimeCheckEP_200");  //"請檢查EP是否漏氣!"
                            if(ret==K_RETRY)
                            {
                                Task=1;
                            }
                            else
                            {
                                bIndexEveryTimeCheckEPing=false;
                                return true;
                            }
                        }
                    }
                }
            break;
            #endif
        case 300:
            if(ReleaseEPTimer.Off())                                            //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
            {
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
//  CheckAndRecodrEP (golden :9266-9412) -- EP pressure record/alarm.
//------------------------------------------------------------------------------
bool CheckAndRecodrEP(int iArm)                                                 //Steven 20190114 : EP Alarm換位置
{
#if 0 // TODO(W7) -- golden :9266-9412 (EP DAQ record/alarm)
#endif
    (void)iArm;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
//  CheckAndRecodrTorque (golden :9413-9464) / NewCheckAndRecodeTorque (:9465-
//  9684) -- index arm torque record/alarm.
//------------------------------------------------------------------------------
bool CheckAndRecodrTorque(int iArm)                                             //kevin 20201027 Arm  扭力log
{
#if 0 // TODO(W7) -- golden :9413-9464 (torque record/alarm, COM2)
#endif
    (void)iArm;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
bool NewCheckAndRecodeTorque(int iArm)                                          //KaiHuang 20201222
{
#if 0 // TODO(W7) -- golden :9465-9684 (torque record/alarm SM, COM2)
#endif
    (void)iArm;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
//  SaveTorque (golden :9685-9790) -- write torque log file.
//------------------------------------------------------------------------------
void SaveTorque(int iPosition, AnsiString asArm, AnsiString asValue, AnsiString asAlarmCount, AnsiString asSetValue)    //KaiHuang 20201222 Add : For ASE 高雄 扭力值存Log
{
#if 0 // TODO(W7) -- golden :9685-9790 (torque log file write)
#endif
    (void)iPosition; (void)asArm; (void)asValue; (void)asAlarmCount; (void)asSetValue;
}
//------------------------------------------------------------------------------
int GetSocketCheckPos(int IndexPos)                                             //Steven 20140620 : 整合為Function
{
    int iIndexUpPos=0;

    if(IndexPos==0)                                                             //Steven 20160303 : Fixed for close arm offset will crash
    {
        iIndexUpPos=0;
    }
    else if(CosFunction.bIndexZDownToAboveSocket==true)
    {
        iIndexUpPos=IniConfig.dD41SocketInitialCheckOffset*100;
    }
    else if((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
            (IniConfig.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false))                  //Above Socket
    {
        if(IniConfig.bTestIcCheckInContact==true)
            iIndexUpPos=DeviceForm.fSocketInitialICCheckPositionOffset*100;     //Steven 210100818
        else
            iIndexUpPos=IniConfig.fIndexCheckOffset*100;                        //Steven 210100818  //Jimmychiu 20230922 : fixed D41 offset
    }
    return iIndexUpPos;
}
//------------------------------------------------------------------------------
int GetIndexZSpeed(int index)                                                   //Steven 20160524 : Index Z軸速度整合為Function
{
    int iScale, sp=1000;
    if(IniConfig.bD54SlowDown)
    {
        if(IniConfig.iD54SlowDownScale<1)
            iScale=1;
        else
            iScale=IniConfig.iD54SlowDownScale;
        sp=MOT[MTestZ1+index].Motor->PJogHighSpeed*iScale/100;
    }
    else
    {
        sp=MOT[MTestZ1+index].GailSpeed;
    }
    return sp;
}
//------------------------------------------------------------------------------
void IndexAddSpeedDisplay()                                                     //KaiChen 20171225 (Steven)：Add Speed Display
{
#if 0 // TODO(W7) -- golden :9834-9839 (fMain speed-display UI)
#endif
}
//------------------------------------------------------------------------------
void IndexSubSpeedDisplay()                                                     //KaiChen 20171225 (Steven)：Add Speed Display
{
#if 0 // TODO(W7) -- golden :9840-9848 (fMain speed-display UI)
#endif
}
//------------------------------------------------------------------------------
//  IndexCheckOneByOne (golden :9849-9943) / IndexCheck4Site (:9944-10061) --
//  one-by-one / 4-site index-check SMs (MOT[] Z/Suck/Socket).
//------------------------------------------------------------------------------
bool IndexCheckOneByOne(bool bReset, int iWhichArm, int iSiteCount, bool bNeedUpCheck, bool bIsAboveCalibrate)          //Ifor 20200617 : add Use One By One Index Check Function 整合
{
#if 0 // TODO(W7) -- golden :9849-9943 (one-by-one index-check SM)
#endif
    (void)bReset; (void)iWhichArm; (void)iSiteCount; (void)bNeedUpCheck; (void)bIsAboveCalibrate;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
bool IndexCheck4Site(bool bReset,int iWhichArm, int iSiteCount)                 //kevin 20190530 index check
{
#if 0 // TODO(W7) -- golden :9944-10061 (4-site index-check SM)
#endif
    (void)bReset; (void)iWhichArm; (void)iSiteCount;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
//  CheckInitialStartDelayInSocket (golden :10062-10297) / NeedResetInitialDelay
//  (:10298-10376) / CheckToBoostIndexTemp (:10377-10443).
//------------------------------------------------------------------------------
void CheckInitialStartDelayInSocket()                                           //JerryYang 20180629 (wei) Initial delay判斷包成函式
{
#if 0 // TODO(W7) -- golden :10062-10297 (initial-delay-in-socket SM)
#endif
}
//------------------------------------------------------------------------------
bool NeedResetInitialDelay()                                                    //JerryYang 20180828 (Steven) : 預熱過程中又觸發預熱的話, 就重新執行預熱
{
#if 0 // TODO(W7) -- golden :10298-10376
#endif
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
int CheckToBoostIndexTemp()                                                     //Steven 20180817 : Boost Function
{
#if 0 // TODO(W7) -- golden :10377-10443 (boost-temp check)
#endif
    return 0;                                                                  // golden default
}
//------------------------------------------------------------------------------
//  DoTestHeadMotorLoadCell (golden :10444-10746) -- load-cell measure SM.
//------------------------------------------------------------------------------
void DoTestHeadMotorLoadCell()
{
#if 0 // TODO(W7) -- golden :10444-10746 (load-cell measure SM, MOT[]/load-cell)
#endif
}
//------------------------------------------------------------------------------
void SetTestTimeOutTimer(int Index)                                             //Steven 20200407 : 整合Time Out時間設定
{
#if 0 // TODO(W7) -- golden :10747-10783 (TestTimeOut setup, Prod/TestIF)
#endif
    (void)Index;
}
//------------------------------------------------------------------------------
//  CheckSocketSensor (golden :10784-11127) -- socket-sensor verify SM.
//------------------------------------------------------------------------------
bool CheckSocketSensor(int iArm, AnsiString Func, bool bInit, bool bCheckArmHieght)                                     //Steven 20200615 : Socket Sensor整合成Function
{
#if 0 // TODO(W7) -- golden :10784-11127 (socket-sensor verify SM, Sen[]/MOT[])
#endif
    (void)iArm; (void)Func; (void)bInit; (void)bCheckArmHieght;
    return true;                                                               // offline: socket sensor treated OK
}
//------------------------------------------------------------------------------
//  CheckICExistInSocket (golden :11128-11157) -- Gigas IC-exist check.
//------------------------------------------------------------------------------
void CheckICExistInSocket(AnsiString Func)                                      //Jimmychiu 20250827 : Gigas 要求加入Index下壓時確認IC存在
{
#if 0 // TODO(W7) -- golden :11128-11157 (Gigas IC-exist check, Socket)
#endif
    (void)Func;
}
//------------------------------------------------------------------------------
void InitialIndexSocketCheckTask()                                              //Steven 20201022 : For RFMD Empty Socket Check Funstion.
{
    iDoIndexSocketCheckTask=1;
}
//------------------------------------------------------------------------------
//  DoIndexSocketCheck (golden :11164-11296) -- RFMD empty-socket check SM.
//------------------------------------------------------------------------------
void DoIndexSocketCheck()                                                       //Steven 20201022 : For RFMD Empty Socket Check Funstion.
{
#if 0 // TODO(W7) -- golden :11164-11296 (RFMD empty-socket check SM, MOT[]/Socket)
#endif
    (void)iDoIndexSocketCheckTask;
}
//------------------------------------------------------------------------------
//  DoHalfViewAllPassVerifyRTC (golden :11297-11493) / DoHalfViewAllFailVerifyRTC
//  (:11494-11699) -- RTC half-view verify SMs (COM2/RTC).
//------------------------------------------------------------------------------
bool DoHalfViewAllPassVerifyRTC(bool bInitial)                                  //JerryYang 20220215 : RTC Auto Verify half view check
{
#if 0 // TODO(W7) -- golden :11297-11493 (RTC half-view verify SM)
#endif
    (void)bInitial;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
bool DoHalfViewAllFailVerifyRTC(bool bInitial)
{
#if 0 // TODO(W7) -- golden :11494-11699 (RTC half-view verify SM)
#endif
    (void)bInitial;
    return false;                                                              // golden default
}
//------------------------------------------------------------------------------
void DoTemperatureRise(int iArm, bool bTemperatureRise)                         //Ifor 20230418 add
{
#if 0 // TODO(W7) -- golden :11700-11734 (temperature-rise, Temperature/ATC)
#endif
    (void)iArm; (void)bTemperatureRise;
}
//------------------------------------------------------------------------------
void TriggerATC_FFC_Function(bool bEnabled)                                     //Ifor 20240507 add :FFC Trigger Even
{
#if 0 // TODO(W7) -- golden :11735-11749 (ATC FFC trigger)
#endif
    (void)bEnabled;
}
//------------------------------------------------------------------------------
void DoCheckHasTestTempChange()                                                 //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
{
#if 0 // TODO(W7) -- golden :11750-11771 (test-temp-change check, Temperature)
#endif
}
//------------------------------------------------------------------------------
void TestProcessSetToErr(AnsiString sErrorLog)                                  //JerryYang 20231208 : 包起來
{
#if 0 // TODO(W7) -- golden :11772-11819 (set-test-to-error, fMain/Prod)
#endif
    (void)sErrorLog;
}
//------------------------------------------------------------------------------
//==============================================================================
//==  ACTIVE offline stubs for the remaining golden tester-engine surface that
//==  is declared in atester.h but whose golden body is MOT[]/cross-module/UI/
//==  tester-comm-bound with no translated home this wave.  Offline-safe defaults
//==  keep the engine linkable.  Golden file:line cited per symbol.  TODO(W7/W5).
//==============================================================================
bool ProcessPauseTester()                       { return false; }              // golden -- no pause offline (atester.h:57)
void bContSoftEpSwitch(int Index, bool Arm)                                     //kevin 20130608 soft contactg使用 (W6.4: __fastcall removed)
{
#if 0 // TODO(W7) -- golden EPSwitch soft-contact (MOT[]/EP DAQ)
#endif
    (void)Index; (void)Arm;
}
//------------------------------------------------------------------------------
//  SendSiteMapToRTC (golden :5471-5561) / CheckTwoArmSiteMap (:5451) -- RTC
//  sitemap send + two-arm sitemap diff.  CheckTwoArmSiteMap is ACTIVE (substrate
//  only); SendSiteMapToRTC is gated (COM2/RTC).
//------------------------------------------------------------------------------
int SendSiteMapToRTC(bool bSendToRTC, int iSelArm)                              //jou 2014-06-24 RTC 自動進行Model驗證
{
#if 0 // TODO(W7) -- golden :5471-5561 (RTC sitemap send, COM2/RTC)
#endif
    (void)bSendToRTC; (void)iSelArm;
    return 0;                                                                  // golden default
}
bool CheckTwoArmSiteMap()                                                       //jou 2014-06-24
{
    bool bFlag=false;
    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j] ^ LastSet.bUseTestSocket[1][i][j])
            {
                bFlag=true;
            }
        }
    }
    return bFlag;
}
