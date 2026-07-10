//---------------------------------------------------------------------------
//  atester_32Site.h  --  32-Site dual-arm test-head SM interface (W5 translation)
//
//  Translation wave: W5-atester32site (biggest single unit in this batch)
//  Translator: AI(W5-atester32site-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/atester_32Site.h (17 lines, cp950)
//
//  Mirrors the golden header exactly (same declarations/order) plus one entry
//  golden itself never put in this header (Do32RTCAutoModelVerify is defined
//  earlier in the same golden .cpp and used later in the same TU with no
//  separate declaration needed there; declared here for clarity/reuse, same
//  as atester.h already does for several such "not in golden .h" entries).
//---------------------------------------------------------------------------
#ifndef atester_32SiteH
#define atester_32SiteH
//---------------------------------------------------------------------------

void InitTestSuckTestIC_TwoArm32Site_Task();
void InitTestYTwoArm32SiteTask();
bool DoInterFaceErrorStep_TwoArm32Site();                                       //Steven 20140815 : Add For 32 Site
bool DoTestSuckTestIC_TwoArm32Site();
bool DoTestY_TwoArm32Site();
bool Do32SiteTestDestroyIC();                                                   //Steven 20150910 : 32Site dual-arm suck/destroy together
bool Do32SiteTestSuckIC();                                                      //Steven 20150910 : 32Site dual-arm suck/destroy together
extern int i32RTCAutoModelVerifyTask;
extern int iTestSuckTestIC_TwoArm32Site_Task;
extern int iTestTwoArm32SiteTask;

// AI(W5-atester32site-Translate) 20260710: not in golden atester_32Site.h (golden
// calls it from later in the same .cpp with no forward decl needed there) --
// declared here so other TUs / the integrate step can reference it if needed.
bool Do32RTCAutoModelVerify(bool bInitial=false);                               //jou 2014-06-24 RTC auto model verify (dual-arm 32-site variant)

#endif
