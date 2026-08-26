//---------------------------------------------------------------------------
#ifndef asortarmH
#define asortarmH
//---------------------------------------------------------------------------

#include "myTimer.h"

extern bool bSortArmSuckActive[MAX_ARM_Row][MAX_ARM_Col];
extern bool bSortArmSuckActiveBack[MAX_ARM_Row][MAX_ARM_Col];
extern bool bSortArmManualStepPress;
extern int SortArmTask;
extern int iDoSortArmAfterPlaceToAutoTask;
extern int iSortArmPlaceOrder;
extern int iSortArmPickOrder;
extern bool bSortArmXOverLimit;
extern int iSortWhichAuto;

extern TQPF_Timer DoPickFromSortShtDelay;
extern TQPF_Timer DoSortPlaceToAutoDelay;

extern bool bSortPickShuttleError;
extern bool bSortOverTray;

void InitSortArmTask();
void DoSortArm();
bool MoveSortArmXY_ToAuto(int Type);
bool MoveSortArmXY_ToManual(int Type);
bool IsSortArmSuckHasAuto(int type);
int GetSortArmPlaceToAutoMaxSpin();
int GetSortArmPlaceToManualMaxSpin();
bool MoveSortArmXY_ToFix_Tray_Full();
bool MoveSortArmXY_ToShuttleAlarmArea();
bool MoveSortArm2XYToDecayTeach();
extern int SortArmTask;
extern int iPickFromSortShtTask;
extern int iSortPlaceToAutoTask;
bool MoveSortArmToAutoSafe();
bool CheckSortArmZ(bool bMessage);

bool CheckSortShtICPos(int WhichShuttle);
int  CheckSortArmCleanOut(int Task=50);
//bool MoveSortArmZ_SortSht_Place();
bool CheckSortArmInitState();
void InitSortPlaceToAutoTask();
void InitPickFromSortShtTask();
void PickFromSortSht();
void SortArmAddSpeed();
void SortArmSubSpeed();
void SetSortArmNeedDestory(int iRow, int iCol, int iShtRow, int iShtCol);

int Find_SortArm_PickerMaxUseCountOnTime(int iUseSuck, int &iRow, int &iCol);
int Find_SortArm_PickerBufferMaxUseCountOnTime(int iUseSuck, int &iStep, int &iRow, int &iCol, bool bSearchLast);  //JerryYang 20221215 : Magazine把fix區當buffer區功能
void Find_SortArm_Single(int iUseSuck, int &iRow, int &iCol);
extern int iSortArmXPosition[MAX_ARM_Row][MAX_ARM_Col];
extern int iSortArmYPosition;
void AdjustSortArmClosePitchCondition(int iw, bool &bCanPick2ICAtOnceTime);
bool CheckSortSuckICFallDown(bool AutoClean=false);

extern bool bCarryControlSortarm;

void SetSortArmHome();                                                          //RogerYang 20250510 Add for 9046AU

extern bool bSortUseOnebyOne;
void ShowSortShuttleDataMiss();                                                 //Steven 20110527
void SortArmAddBlockPitch(int &nowX, int &nowY, int iRow, int iCol);
void SortTransferAutoRatio(int iWhichAuto, int *iXPos, int *iYPos);                   //Steven 20141029 : Auto的軟體齒輪比
void SortTransferOutShuttleRatio(int iShuttle, int *iXPos, int *iYPos, int Row, int Col);      //Steven 20141029 : Out Shuttle的軟體齒輪比
int SortArmPickShuttleAlarm(int iSht, bool bHasDuplicateErr, AnsiString ErrPart);        //Steven 20141121 : 整合OutArm吸取異常
void PorcessJAM0201SortArmPickUpErrorSkip(int iShtR, int iShtC, int iSuckR, int iSuckC);        //Steven 20161214 : 加上OutArm吸取異常Skip的ErrorLog

//bool UnloaderTrayYDirForArmYPitch(int iW);
extern int AutoCalculateSortArmXClosePitch(int iWhichAuto, bool bUseDeviceDinemsion=false);
extern bool GetSortArmToUnLoaderPosition(int bCanPick2ICAtOnceTime, int iOutPutTray, int &iXPos, int &iYPos, int iR, int iC, bool RealMove);
extern double dSortArmXPitch_1Step;                                              //1吸嘴之間的X-Pitch
extern double dSortArmXPitch_MovePitch;                                          //張到最開的X-Pitch
extern int    iSortArmXStep;                                                     //Sort Arm在Tray X一次要跳幾格
extern int    iSortArmYStep;                                                     //Sort Arm在Tray Y一次要跳幾格

//------------------------------------------------------------------------------
//void SortArmAddSpeedDisplay();
//void SortArmSubSpeedDisplay();

//從Shuttle取料
int  GetSortArmToSortShuttleOffset();
int  GetSortArmPitchX(int w, int iOffset=-1);

bool MoveSortArmToShuttleIncludeZ(bool bZDown=false);
bool DoSortPickFromShuttle();

//2x4_8
int GetNowSortShuttleMode_2x4_8();
bool CheckSortArmXYPitch_2x4_8(int *iX, int *iY, int iMovePitchX);
bool SortArmZNeedDown_2x4_8();
bool DoSortPickFromShuttle_2x4_8();
bool MoveSortArmToShuttleIncludeZ_2x4_8(bool bZDown=false);

//2x8_8
int GetNowSortShuttleMode_2x8_8();
bool CheckSortArmXYPitch_2x8_8(int *iX, int *iY, int iMovePitchX);
bool SortArmZNeedDown_2x8_8();
bool DoSortPickFromShuttle_2x8_8();
bool MoveSortArmToShuttleIncludeZ_2x8_8(bool bZDown=false);

bool bSortShtwaitPick();
bool MoveSortArmXYToSortShtWait();
void SetSortArmNeedSuck(int iRow, int iCol, int iShtRow, int iShtCol, bool bPlace=false);
bool SwapShuttleDataToSortArm(int iShtRow, int iShtCol, int iSuckRow, int iSuckCol, int *iWitchErrBin);

extern bool bSortArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];
extern bool bCheckSortArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col];
extern bool bCheckSortArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col];

extern TQPF_Timer CheckSortArmDestroyActiveDelay[MAX_ARM_Row][MAX_ARM_Col];

void GetSortShuttleStatus_Pick(int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bZDown, bool bZFlag[MAX_ARM_Row][MAX_ARM_Col]);
bool IsCheckSortArmDestroyActiveFinish(int iXPos, int iYPos);
void DoSortArmSuckPreOn(int iKit, int iXPos, int iYPos, bool bZDown);

//放料到Auto
int DoSortArmAfterPlaceToAuto(bool &bCheckSpeed, bool bInitial=false);
bool DoSortArmPlaceToWitchAuto(int iToWhichAuto);
bool DoSortArmPlaceToAuto();

bool DoMoveSortArmXYToPlace(bool &bCanPick2ICAtOnceTime, int iRow, int iCol, int iStep, bool RealMove);
bool SearchUnLoadTrayUpDown_Sort(bool &bCanPick2ICAtOnceTime, int &iRow, int &iCol);
void CheckSortArmDestroyActive();

bool SetSortArm();                                                              //RogerYang 20250515 Add for 9046AU
bool MoveOutArmZToPlateSafe();

//---------------------------------------------------------------------------
//class TMyQASample
//{
//    private:
//    protected:
//    public:
//        __fastcall TMyQASample();
//        int iCount[4][8];
//        int iMinX;
//        int iMinY;
//        int iCurrCount;
//        void ClearCount();
//        void FindDeviceToQA();
//};
//extern TMyQASample QASam;
#endif