//---------------------------------------------------------------------------

#ifndef atester_32SiteH
#define atester_32SiteH
//---------------------------------------------------------------------------

void InitTestSuckTestIC_TwoArm32Site_Task();
void InitTestYTwoArm32SiteTask();
bool DoInterFaceErrorStep_TwoArm32Site();    //Steven 20140815 : Add For 32 Site
bool DoTestSuckTestIC_TwoArm32Site();
bool DoTestY_TwoArm32Site();
bool Do32SiteTestDestroyIC();               //Steven 20150910 : 32Site 雙Arm一起吸放
bool Do32SiteTestSuckIC();                  //Steven 20150910 : 32Site 雙Arm一起吸放
extern int i32RTCAutoModelVerifyTask;
extern int iTestSuckTestIC_TwoArm32Site_Task;
extern int iTestTwoArm32SiteTask;
#endif
