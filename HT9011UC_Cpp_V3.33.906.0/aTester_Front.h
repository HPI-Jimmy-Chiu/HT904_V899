//---------------------------------------------------------------------------
//  aTester_Front.h  --  TESTER/INDEX front (Z1) down-press test-head helpers
//
//  Translation wave: W64b-Translate (front test-head destroy/down-press SM)
//  Translator: AI(W64b-Translate) 20260706
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Front.h
//
//  SCOPE: narrowly the 6 golden externs aTester_Front.cpp defines this wave
//  (iFrontTestDestroyICTask / InitFrontTestDestroyICTask / DoFrontTestDestroyIC /
//  CheckZ1IsDown / TestZ1OutRandge / TestZ1SetPos).  Every OTHER golden
//  aTester_Front.cpp export (DoFrontTestSuckIC / DoTestYFront / TestZ1OutRandge2 /
//  DoArm1Suck / DoArm1D44VacCheck / FTestSeparateSLK / ... ) is OUT OF SCOPE this
//  wave and stays declared in the existing atester_shims.h (offline shim) exactly
//  as today -- intentionally NOT re-declared here.
//---------------------------------------------------------------------------
#ifndef aTester_FrontH
#define aTester_FrontH
//---------------------------------------------------------------------------

extern int iFrontTestDestroyICTask;

extern void InitFrontTestDestroyICTask();
extern void TestZ1SetPos();

extern bool DoFrontTestDestroyIC(bool bCheckZ2);
extern bool CheckZ1IsDown();
extern bool TestZ1OutRandge();

#endif
