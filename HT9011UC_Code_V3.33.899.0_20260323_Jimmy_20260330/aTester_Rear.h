//---------------------------------------------------------------------------

#ifndef aTester_RearH
#define aTester_RearH
//---------------------------------------------------------------------------

extern int iBTestSuckTestICTask;
extern int iRearTestDestroyICTask;
extern int iRearTestSuckICTask;
extern int iTestYRearTask;
extern int iBRTCUseSocketFloatTask;
extern int iBRTCGiveWayCheckTask;
extern int iBRTCAutoModelVerifyTask;
extern int iBTestSocketClampCloseTask;
extern int iBTestSocketClampOpenTask;
extern int iRearTestPurgBeforePickShuttle;
extern int iIndexArm1PickUpErrNeedPiggybackTask;
extern int CheckAnyCaseNeedToDoArm2();                                          //Steven 20190115 : SCC要求吸取異常要檢查Socket

extern TQPF_Timer hBTestTimeOutDelay;                                           //Jou 20101018

extern void InitTestYRearTask();
extern void InitBTestSuckTestICTask();
extern void InitRearTestSuckICTask();
extern void InitRearTestDestroyICTask();
extern void InitRearTestPurgBeforePickShuttle();
extern void TestZ2SetPos();
extern void DoArm2Suck();                                                       //JerryYang 20190123 把index arm吸真空&交換狀態包成函式
extern void DoArm2D44VacCheck();                                                //JerryYang 20190123 把index arm回吸檢查包成函式

extern bool BTestNeedDestroy();
extern bool DoRearTestDestroyIC(bool bCheckZ1);
extern bool DoRearTestSuckIC();
extern bool DoBTestSuckTestIC();
extern bool CheckZ2IsDown();
extern bool DoRearTestPurgBeforePickShuttle(int isp);
extern bool DoTestYRear();
extern bool TestZ2OutRandge();
extern bool TestZ2OutRandge2();
extern bool BTestCombineSLK(bool bReset=false);                                 //JerryYang 20160429 分離模式, 測試完成 Z1 SLK要與Clamp結合,再將IC吸起流程
extern bool BTestSeparateSLK(bool bReset=false);                                //JerryYang 20160429 分離模式, Z1 SLK與Clamp分離流程
extern bool DoBRTCAutoModelVerify(bool bInitial);                               //JerryYang 20220215 : RTC Auto Verify half view check
#endif
