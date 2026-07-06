//---------------------------------------------------------------------------
//  atester.h  --  TESTER / INDEX engine interface (W6.4 translation)
//
//  Translation wave: W6.4 (TESTER/INDEX ENGINE -- atester.cpp)
//  Translator: AI(W6.4-TESTER) 20260626
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/atester.h (cp950)
//
//  Faithful port off the Borland dialect:
//    * __fastcall removed (bContSoftEpSwitch).
//    * TQPF_Timer / DWORD / FILE / AnsiString come via the standard headers the
//      .cpp pulls (vclcompat + cmydef/MachineType).  Names + decl order verbatim
//      so the team can reverse-look-up the BCB6 header.
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8.  ZERO U+FFFD.
//---------------------------------------------------------------------------
#ifndef atesterH
#define atesterH
//---------------------------------------------------------------------------
//  W6.4: the golden atester.h relies on the .cpp pulling its type providers first
//  (TQPF_Timer via myTimer.h; MAX_SOCKET_ROW/COL + IC consts via MachineType.h;
//  AnsiString via vclcompat).  Mirror aoutarm9045.h: include cmydef.h, which pulls
//  MachineType.h + myTimer.h + cprod.h + cpublic.h, so this header is self-sufficient
//  whichever TU includes it (the canary includes atester.h before MachineType.h).
//---------------------------------------------------------------------------
#include "cmydef.h"
//---------------------------------------------------------------------------
extern int iTestYTask;
extern int iSetupTask;
extern int iDoInterFaceErrorStepTask;
extern int iTempICTask;                                                         //kevin 20130808 IC量測溫度
extern int iCCDTimeOutCount;
extern int iSocketSenSosPos1;
extern int iSocketSenSosPos2;                                                   //kevin 20150613 關arm 設定可判斷位置
extern int iDoStartMode;
extern int iDoEndMode;
extern int iIndexYAxisServoOnStateTas;
extern int iIndexYAxisServoOnStateTask;
extern bool bFirstZ1UPZ2Down;
extern bool fRearCheckSuckICPass;
extern bool bTestDuplicateErr;
extern bool bInitialSackTime;                                                   //kevin 20131112 第一次吸取ic等待時間
//extern bool bOneTimeWait;
extern bool bDoubleContact;
extern bool bHangTimePause;                                                     //Steven 20090827 : Hang Up dectector
extern bool GetIndexTime_flag;
extern bool bNeedUpDonwOneTome;
extern bool bFTestSuckDrop;
extern bool bBTestSuckDrop;
extern FILE *EPOut;                                                             //Ifor 20150707
extern bool bIndexPickUpErrMoveSht1;                                            //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
extern bool bIndexPickUpErrMoveSht2;                                            //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
extern bool bIndexWaitingInArmAway;                                             //Steven 20171228 (Wei) : Index在等In Arm讓開
extern bool bNeedCheckRTCReport;                                                //Ifor 20190723 : add
extern TQPF_Timer DoGiveWayDelay;                                               //Ifor 20190723 : add
extern const int TESTZ1UP;
extern const int TESTZ2UP;
extern const int CCDTimeOutSec;
extern const int CheckFailDownCT;                                               //kevin 20131121   10 來不及偵測掉料
extern const int OverEncoderDelay;
extern TQPF_Timer HangTime;                                                     //Steven 20090827 : Hang Up dectector
extern TQPF_Timer SoftContactTim;                                              //kevin 20130608 soft 模式 需等ep 穩定
extern TQPF_Timer DoTestHeadMotorDelay;
extern TQPF_Timer DoTestHeadMotorDelay2;
extern int iReContactCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL];                       //Steven 20231205 : 計算某site contact 次數
extern bool IsIndexRunCycle();
extern bool ProcessPauseTester();
extern bool DoSetupTest(int iContactArm=0);
extern bool IndexAlarmInArmAway();                                              //Steven 20130613 : Index異常時, In Arm要先讓位功能
extern bool SetNoiseDelay;
extern bool CheckIndexStatus(AnsiString str);
extern bool CheckShuttlePos();
extern bool CheckPlaceOutShuttle(int iShuttle);                                 //Steven 20181228 : Add Index Action  //ChungHung 20171116 modify for Index Action
extern bool DoStartMode(int mode);
extern bool DoEndMode(int mode);
extern bool DoInterFaceErrorStep(int ZAxisSelect);

extern void ProcessCount(int Index, bool bHasIC);                              //Eastsun 20260515 F022: D7
extern void InitFTestSuckTestICTask();
extern void InitTestTask();
extern void InitDoStartMode();
extern void InitDoEndMode();
extern void ProcessShowTestStatus(int Index);
extern void RecordPiggyBackStartEnd(bool bStart);
extern void InitTestYTask();
extern void InitialTestHeadMotorTask();

extern int  ProcessTestResult(int Index);
extern int  Test_GetTestResul(int X,int Y);
extern int  ProcessTesterTimeOut(int Index);
extern void SetTestTimeOutTimer(int Index);                                     //Steven 20200407 : 整合Time Out時間設定
extern void TestProcessSetToErr(AnsiString sErrorLog);
extern int  GetSocketCheckPos(int IndexPos);
extern int  GetIndexZSpeed(int index);                                          //Steven 20160524 : Index Z軸速度整合為Function
extern TQPF_Timer DropContactTimer1;                                            //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
extern TQPF_Timer DropContactTimer2;
extern TQPF_Timer DropContactTimer3;
//extern DWORD dwEndInitialCount;                                              //ChungHung 20140801 add Korea Want to count down in main status  //JerryYang 20180629 (wei) : 預熱後改用另一支arm做測試
extern TQPF_Timer dwStartInitialCount;                                          //ChungHung 20140801 add Korea Want to count down in main status

extern TQPF_Timer HTestDeley;                                                   //kevin 20190826 add
extern TQPF_Timer dwEndInitialCount;                                            //kevin 20180926 add
extern DWORD dwEndShuttle1Soak;                                                 //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
extern DWORD dwStartShuttle1Soak;
extern DWORD dwEndShuttle2Soak;
extern DWORD dwStartShuttle2Soak;
//jou 2010-01-05 start : 節省空氣流量
//void InitiationSaveAirLoseTask();
//void SaveAirLose(int iSelFrontRear);
//jou 2010-01-05 end

//------------------------------------------------------------------------------
// jou 981214
// Contect & OneCycle & CleanOut & Home 完成時，index Y axis servo off
// contech & tech & AutoRun 時，檢察index是否有被動到
//------------------------------------------------------------------------------
//void IndexYAxisServoOff();
//bool CheckIndexYAxisServoOnState();
bool GetTempICResult();                                                         //kevin 20130808 IC量測溫度
void bContSoftEpSwitch(int Index, bool Arm);                                    //kevin 20130608 soft contactg使用  (W6.4: __fastcall removed)
void IndexAddSpeedDisplay();                                                    //KaiChen 20171225 (Steven)：Add Speed Display
void IndexSubSpeedDisplay();                                                    //KaiChen 20171225 (Steven)：Add Speed Display
bool IndexCheckOneByOne(bool bReset, int iWhichArm, int iSiteCount, bool bNeedUpCheck=false, bool bIsAboveCalibrate=false);                                     //Ifor 20200617 : add Use One By One Index Check Function 整合
void CheckInitialStartDelayInSocket();                                          //JerryYang 20180629 (wei) : Initial delay判斷包成函式
bool NeedResetInitialDelay();                                                   //JerryYang 20180828 (Steven) : 預熱過程中又觸發預熱的話, 就重新執行預熱
int CheckToBoostIndexTemp();
int DoTesterSidePush(bool bInitial);                                            //Richard 20220321 : 渠梁Side Push
bool DoAllPassVerifyRTC(bool bInitial=false);                                   //jou 2014-06-24 RTC 自動進行Model驗證
int SendSiteMapToRTC(bool bSendToRTC,int iSelArm);                              //jou 2014-06-24 RTC 自動進行Model驗證
bool CheckTwoArmSiteMap();                                                      //jou 2014-06-24 RTC 自動進行Model驗證

void InitCheckSocketHasIC();                                                    //jou 20180802 : index pick error need index vaccum check
bool DoCheckSocketHasIC(int iSelArm=3);                                         //jou 20180802 : index pick error need index vaccum check
bool CheckAndRecodrEP(int iArm);
bool IndexCheck4Site(bool bReset,int iWhichArm, int iSiteCount);                //kevin 20190530 index check
extern TQPF_Timer TestTimeOut;
bool CheckSocketSensor(int iArm, AnsiString Func, bool bInit=false, bool bCheckArmHieght=true);                         //jimmychiu 20230830 : add switch check arm height in check socket sensor function
void CheckICExistInSocket(AnsiString Func);                                     //Jimmychiu 20250827 : Gigas 要求加入Index下壓時確認IC存在
void InitialIndexSocketCheckTask();
void DoIndexSocketCheck();
bool CheckAndRecodrTorque(int iArm);                                            //kevin 20201027 Arm  扭力log
extern int iDoIndexSocketCheckTask;

bool NewCheckAndRecodeTorque(int iArm);                                         //KaiHuang 20201222
void SaveTorque(int iPosition, AnsiString asArm, AnsiString asValue, AnsiString asAlarmCount,AnsiString asSetValue);    //KaiHuang 20201222
bool DoHalfViewAllPassVerifyRTC(bool bInitial=false);                           //JerryYang 20220215 : RTC Auto Verify half view check
bool DoHalfViewAllFailVerifyRTC(bool bInitial=false);
void DoTemperatureRise(int iArm, bool bTemperatureRise);
void DoCheckHasTestTempChange();                                                //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
void TriggerATC_FFC_Function(bool bEnabled=false);                              //Ifor 20240507 add :FFC Trigger Even

//------------------------------------------------------------------------------
// W6.4 ADD: tester-engine entry points NOT in the golden atester.h (the golden
// declares them inline in the .cpp / via cprod.h), exported here so the canary +
// the central pump can call the result-decode anchor + SM skeletons.
//------------------------------------------------------------------------------
int  GetRowCol(int &iRow, int &iCol);                                           //golden atester.cpp:144
int  GetRTCSiteMap();                                                           //golden atester.cpp:423
int  ArrayConvertSite(int X, int Y);                                            //golden atester.cpp:504  (pure site geometry)
int  GetOneByOneData(int X, int Y);                                             //golden atester.cpp:621  (decode anchor)
int  GetBinaryData(int X, int Y);                                               //golden atester.cpp:670  (decode anchor)
int  Test_GetTestResulSub(int X, int Y);                                        //golden atester.cpp:759  (decode anchor, HAL-light)
bool Check_TTL_Status(int Type);                                                //golden atester.cpp:2564 (decode consumer)
bool CheckTestSocketIsError(int i, int j);                                      //golden atester.cpp:2584 (decode consumer)
bool GetTesterResult(int Type);                                                 //golden atester.cpp:849  (result-acquisition SM)
void DoTestY();                                                                 //golden atester.cpp:4789 (test-cycle dispatcher SM)
void DoTestHeadMotor();                                                         //golden atester.cpp:5562 (central index/test SM)
bool IndexEveryTimeCheckEP();                                                   // AI(W7T1-Integrate) 20260701: golden bool sig (atester.cpp:9158); un-gated case 30000 calls if(IndexEveryTimeCheckEP())
void InitIndexEveryTimeCheckEP();                                               //golden atester.cpp:9151
extern int iTestTask;                                                           //golden atester.cpp:592
extern int iTestHeadMotorTask;                                                  //golden atester.cpp:5346
extern int iCheckSocketHasIC;                                                   //golden cursor (init via InitCheckSocketHasIC)
extern int iIndexEveryTimeCheckEPTask;                                          //golden cursor
extern int iGetTestDataDelayTask;                                              //golden atester.cpp:416
extern int iTesterBIN[MAX_SOCKET_ROW][MAX_SOCKET_COL];                          //golden atester.cpp:117 (file-local in golden; exported here for the canary oracle)
extern bool ScanPort[];                                                         //golden atester.cpp:124 (file-local in golden; exported for the canary oracle)
#endif
