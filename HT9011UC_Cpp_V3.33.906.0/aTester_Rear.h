//---------------------------------------------------------------------------
//  aTester_Rear.h  --  TESTER/INDEX ENGINE, Rear (Z2/Arm2) narrow-scope slice
//
//  Translation wave: W64b-Translate (paired with a sibling aTester_Front.h)
//  Translator: AI(W64b-Translate) 20260706
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Rear.h
//
//  NARROW SCOPE: this mirrors only the subset of golden aTester_Rear.h's own
//  extern list that the translated DoRearTestDestroyIC/CheckZ2IsDown/
//  TestZ2OutRandge/TestZ2SetPos/InitRearTestDestroyICTask slice needs.  All
//  other golden aTester_Rear.h externs (DoRearTestSuckIC, DoBTestSuckTestIC,
//  DoRearTestPurgBeforePickShuttle, DoTestYRear, TestZ2OutRandge2, BTestCombineSLK,
//  BTestSeparateSLK, DoBRTCAutoModelVerify, bArm2SuckFinish, hBTestTimeOutDelay,
//  CheckAnyCaseNeedToDoArm2, DoArm2Suck, DoArm2D44VacCheck, BTestNeedDestroy,
//  iBTestSuckTestICTask/iRearTestSuckICTask/iTestYRearTask/...) are OUT OF SCOPE
//  and remain declared in atester_shims.h as today -- do NOT add them here.
//
//  ResetArm2PutFlag() and bArm2PutDuplicateErr[][] are golden aTester_Rear.cpp-
//  local (golden aTester_Rear.h does not declare them either) -- correctly
//  absent from this header.
//---------------------------------------------------------------------------
#ifndef aTester_RearH
#define aTester_RearH

extern int iRearTestDestroyICTask;

void InitRearTestDestroyICTask();

bool DoRearTestDestroyIC(bool bCheckZ1);
bool CheckZ2IsDown();
bool TestZ2OutRandge();
void TestZ2SetPos();

#endif
