//---------------------------------------------------------------------------
//  aTester_Front.h  --  TESTER/INDEX front (Z1) down-press test-head helpers
//
//  Translation wave: W64b-Translate (front test-head destroy/down-press SM)
//  Translator: AI(W64b-Translate) 20260706
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Front.h
//
//  SCOPE (Wave 1): narrowly the 6 golden externs aTester_Front.cpp defined that
//  wave (iFrontTestDestroyICTask / InitFrontTestDestroyICTask /
//  DoFrontTestDestroyIC / CheckZ1IsDown / TestZ1OutRandge / TestZ1SetPos).
//
//  SCOPE (Wave 2, AI(W64b-Translate) 20260710): adds the 3 golden externs
//  DoFrontTestSuckIC needs (iFrontTestSuckICTask / InitFrontTestSuckICTask /
//  DoFrontTestSuckIC).  These were previously declared+offline-stubbed in
//  atester_shims.h/.cpp (see aTester_Front.cpp's Wave-2 "INTEGRATE-STEP FIX
//  NEEDED" note) -- the integrate agent must remove those 3 duplicate
//  declarations/definitions once this file lands, same as Wave 1's 6-symbol
//  precedent.
//
//  Every OTHER golden aTester_Front.cpp export (DoTestYFront / TestZ1OutRandge2 /
//  DoArm1Suck / DoArm1D44VacCheck / FTestSeparateSLK / ... ) remains OUT OF
//  SCOPE and stays declared in the existing atester_shims.h (offline shim)
//  exactly as today -- intentionally NOT re-declared here.
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

// -- Wave 2 additions --
extern int iFrontTestSuckICTask;

extern void InitFrontTestSuckICTask();
extern bool DoFrontTestSuckIC();

#endif
