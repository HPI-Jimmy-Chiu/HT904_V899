// =============================================================================
//  asortarm.h  --  SORT-ARM (HT-9046AU 2nd out-arm) state-machine interface
//
//  Faithful translation of golden asortarm.h (146 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W4-asortarm) 20260808
//  Translation wave: W906-PT-W4 (genuinely-missing non-form units), unit
//  asortarm.cpp/.h.
//
//  ROLE: the public surface of the HT-9046AU "sort arm" -- the SECOND output
//  arm that picks IC out of the SORT shuttle (OutSht3Kit) and places them into
//  the Auto4/5/6 unloader trays.  Everything here is the sort-side mirror of
//  aoutarm.h's out-arm surface (MOutSortX/Y/Sht/PitchX instead of MOutArm*,
//  OutArm2Suck instead of OutArmSuck, OutSht3Kit instead of FR/BRCarryKit).
//
//  TRANSCRIBED VERBATIM from golden, including golden's own quirks -- these are
//  NOT translation slips and must not be "tidied":
//   (Q1) golden re-declares `extern int SortArmTask;` THREE times (:11, :33 and
//        again inside the :33 block) and `extern TMyKitSuck`-free duplicates of
//        iPickFromSortShtTask/iSortPlaceToAutoTask at :33-35 after already
//        declaring SortArmTask at :11.  Kept exactly.
//   (Q2) golden declares `bool DoMoveSortArmXYToPlace(bool&,int,int,int,bool)`
//        -- FIVE parameters (golden :124) -- while asortarm.cpp DEFINES the
//        FOUR-parameter form `(bool&,int,int,bool)` (golden .cpp:3983).  The
//        5-param overload is declared-but-never-defined in golden and every
//        golden call site passes 4 arguments, so it resolves to the 4-param
//        definition.  BOTH are kept exactly as golden wrote them.
//   (Q3) golden DECLARES, but asortarm.cpp never DEFINES: MoveSortArmXY_ToAuto,
//        MoveSortArmXY_ToManual, IsSortArmSuckHasAuto, GetSortArmPlaceToAutoMaxSpin,
//        GetSortArmPlaceToManualMaxSpin, MoveSortArmXY_ToFix_Tray_Full,
//        MoveSortArmXY_ToShuttleAlarmArea, MoveSortArm2XYToDecayTeach,
//        CheckSortShtICPos, Find_SortArm_PickerBufferMaxUseCountOnTime.
//        They have no body in golden's whole tree either (grepped this wave).
//        Declaration-only costs nothing and nothing in this port calls them --
//        kept verbatim rather than deleted, so the file stays a mirror.
//   (Q4) golden's `class TMyQASample` block (:132-145) is COMMENTED OUT here --
//        the live declaration is aoutarm.h:155 (golden) / aoutarm.h:218 (this
//        tree).  Kept commented, exactly as golden has it.
//
//  INCLUDE DELTA vs golden: golden includes only "myTimer.h" and relies on the
//  aArmHeader.h god-header having already pulled MachineType.h (MAX_ARM_Row /
//  MAX_ARM_Col) and the VCL AnsiString.  This tree's aArmHeader.h is a
//  guard-only shim (its whole include list is `#if 0`), so this header pulls
//  MachineType.h + vclcompat/vcl_compat.h itself to stay self-contained --
//  the same fix the sibling aoutarm.h made for the same reason (aoutarm.h:65-66).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef asortarmH
#define asortarmH
//---------------------------------------------------------------------------

#include "MachineType.h"            //AI(W906-PT-W4-asortarm) 20260808: MAX_ARM_Row / MAX_ARM_Col, which golden got transitively via aArmHeader.h
#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W4-asortarm) 20260808: AnsiString, likewise transitive in golden
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

//從Shuttle吸料
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
