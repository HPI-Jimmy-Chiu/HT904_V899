//---------------------------------------------------------------------------

#ifndef aTester_FrontH
#define aTester_FrontH
//---------------------------------------------------------------------------

extern int iFTestSuckTestICTask;
extern int iFrontTestDestroyICTask;
extern int iFrontTestSuckICTask;
extern int iTestYFrontTask;
extern int iFRTCUseSocketFloatTask;
extern int iFRTCAutoModelVerifyTask;
extern int iFrontTestPurgBeforePickShuttle;
extern int iIndexArm2PickUpErrNeedPiggybackTask;
extern int iFTestSocketClampCloseTask;
extern int iFTestSocketClampOpenTask;
extern int CheckAnyCaseNeedToDoArm1();                                          //Steven 20190115 : SCC要求吸取異常要檢查Socket

extern TQPF_Timer hFTestTimeOutDelay;                                           //Jou 20101018

extern void InitTestYFrontTask();
extern void InitFTestSuckTestICTask();
extern void InitFrontTestSuckICTask();
extern void InitFrontTestDestroyICTask();
extern void InitFrontTestPurgBeforePickShuttle();
extern void TestZ1SetPos();
extern void DoArm1Suck();                                                       //JerryYang 20190123 把index arm吸真空&交換狀態包成函式
extern void DoArm1D44VacCheck();                                                //JerryYang 20190123 把index arm回吸檢查包成函式

extern bool FTestNeedDestroy();
extern bool DoFrontTestDestroyIC(bool bCheckZ2);
extern bool DoFrontTestSuckIC();
extern bool DoFTestSuckTestIC();
extern bool CheckZ1IsDown();
extern bool DoFrontTestPurgBeforePickShuttle(int isp);
extern bool DoTestYFront();
extern bool TestZ1OutRandge();
extern bool TestZ1OutRandge2();
extern bool bArm2NeedCombine;                                                   //JerryYang 20180520 (wei) : 判斷Arm2分離SLK是否需結合
extern bool FTestSeparateSLK(bool bReset=false);                                //JerryYang 20160429
extern bool FTestCombineSLK(bool bReset=false);                                 //JerryYang 20160429
extern bool DoFRTCAutoModelVerify(bool bInitial);                               //JerryYang 20220215 : RTC Auto Verify half view check
#endif
