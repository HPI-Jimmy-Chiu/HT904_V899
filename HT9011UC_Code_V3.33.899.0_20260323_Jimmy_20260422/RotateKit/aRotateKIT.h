//---------------------------------------------------------------------------
#ifndef aRotateKITH
#define aRotateKITH
//---------------------------------------------------------------------------

extern void InitialInArmRotateKIT();
extern bool DoInArmRotateKIT();

extern void InitialOutArmRotateKIT();
extern bool DoOutArmRotateKIT();

extern int iInArmRotateKit;
extern int iOutArmRotateKit;

extern int i2x2Suck;
extern int i2x2Suck_Out;

extern int iInRotateFinish;   //kevin 20130524 確保IC有放到旋轉馬達轉向  1:初始化  2:旋轉完成  3:放置SHUTTLE 0:放完SHUTTLE
extern int iOutRotateFinish;  //kevin 20130524 確保IC有放到旋轉馬達轉向  1:初始化  2:旋轉完成  3:放置SHUTTLE 0:放完SHUTTLE
extern bool CheckNeedRotateWithoutHotIC();  //Steven 20160809 : 改成Function

//void RotateKIT_FormSet( int iStartX , int iStartY , int iPitchX , int iPitchY , int iWidth , int iHeight );
void RotateKIT_FormSet(bool bInput, int iStartX, int iStartY, int iPitchX, int iPitchY);
bool CheckRotateSensor(bool bRotateIn);//kevin 20130524  bRotateIn=true : in rotate sensor   bRotateIn=false : out rotate sensor

//ChungHung 20130708 add extern 解決重複定義
extern int iInRotateWhichKit;
extern int iOutRotateWhichKit;//kevin 20130629 使用4個rotate 0:表示放第一次 Aa Ab Ae Ae  1:表示放第二次 Ac Ad Ag Ah
extern int SetMotorResolution(int &Ang45, int &Ang90, bool bInRotate);
extern void InitSuckState();

bool MoveRotateToDegree(bool bInRotate, int iDegree, bool bInitial=false);   //Steven 20170329 : Add individual rotate motor

extern int iRotateKIT_Start_X_H;
extern int iRotateKIT_Start_Y_H;
extern int iRotateKIT_Pitch_X_H;
extern int iRotateKIT_Pitch_Y_H;
extern int iRotateKIT_Start_X_A;
extern int iRotateKIT_Start_Y_A;
extern int iRotateKIT_Pitch_X_A;
extern int iRotateKIT_Pitch_Y_A;
extern bool bSingleDut;
extern bool CheckRotateOutNotFinish();
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
