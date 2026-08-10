//---------------------------------------------------------------------------
#ifndef aoutarm9045H
#define aoutarm9045H
//---------------------------------------------------------------------------
#include "HTEditList.h"
//---------------------------------------------------------------------------
extern bool bOutArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];                   //jou 981130 確認device確實destroy完成
extern bool bOutSuckShtDupErr[MAX_ARM_Row][MAX_ARM_Col];
extern int  iOutShtRetryCount;
extern int  iBinBoxShiftY;                                                      //jou 2012-12-11 support Bin Box
void BinBoxShiftY();                                                            //jou 2012-12-11 support Bin Box
extern int iOutArmAdditionalFunctionTask;

const int iOUTARM_SUCK=1000;

bool MoveOutArmToAutoSafe_9045();
bool DoMoveOutArmXYToPlace_9045(bool &bCanPick2ICAtOnceTime, int iRow, int iCol, int iStep, bool RealMove);

int  GetOutArmPitch_9045(int w);
int  GetOutArmPitch2_9045(int w);                                               //Steven 20131002 : XY變距
int  GetOutArmPitchY_9045(int w, int iOffset=-1);                               //Steven 20131002 : XY變距
int  GetOutArmPitchX_9045(int w, int iX, int iOffset=-1);                       //Steven for HT1032
int  GetVariableYOutShuttleData();                                              //ChungHung 20131231 alter AutoYPitch

void GetOutArmZDownPos_9045(int iWhichAuto, bool bNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col]);   //Steven 20230323 : For HT1032
int  GetOutArmToShuttleOffset_9045(int iSht, int iModeRow, int iModeCol, bool bPitch=true);
int  GetOutArmXToShuttleOffset_9045(int iSht, int iOffsetPos);                  //Steven 20240309 : fixed for out arm to sht offset
int  GetOutArmYToShuttleOffset_9045(int iSht, int iOffsetPos);
void GetOutShuttleStatus_9045(int iSht, int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bZDown, bool bZFlag[MAX_ARM_Row][MAX_ARM_Col]);

bool Fix3MoveToLeft(bool bLeft);                                                //JimmyChiu 20220927 : Stepper Motor Control in Fix3
int  VerifyTrayStatus();

void DoOutArm_9045();
bool DoPickFromShuttle_9045(int iSht);
bool DoOutArmPlaceToAuto_9045();
void DoOutArmSuckPreOn(int iSht, int iKit, int iXPos, int iYPos, bool bZDown);
bool DoOutArmAdditionalFunction();                                              //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
int  DoOutArmAfterPlaceToAuto(bool &bCheckSpeed, bool bInitial=false);          //Steven 20220526 : 針對放下IC到Unloader後的動作做整合
bool DoFixTrayFullAlarm();
int VerifyFixTrayLink();
bool CheckOutArmToTask50(int iSht);                                             //Steven 20241019 : 整合out arm判斷式

bool CheckOutArmInitState_9045();
bool CheekNeedToDoOutArmAdditionalFunction();

void InitDoOutArmAdditionalFunction();
void InitialFix3CanFullTask();                                                  //ChungHung 20140313 add Fix3 can Full Tray
bool InitialOutArmNeedSuck(int iShuttle);                                       //Steven 20220927 : 重新整理In / Out Arm Need Suck
bool UseFix3Cylinder(int iWhichAuto);                                           //ChungHung 20140313 add Fix3 can Full Tray

bool SetOutArm_9045();

int  SearchTrayToPlace_9045();                                                  //Steven 20160524 : 簡化Out Arm程式碼
bool SearchUnLoadTrayUpDown_9045(bool &bCanPick2ICAtOnceTime, int &iRow, int &iCol);
int  SearchTrayToPlace_Magazine();                                              //JerryYang 20221215 : add Magazine
int  SearchTrayToPick_Buffer();

bool IsCheckOutArmDestroyActiveFinish(int iXPos, int iYPos);                    //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
void PreSetOutAdditionalFlag();                                                 //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
bool OutArmNeedCheckOffset(bool bPlace, int iPos);                              //Steven 20230531 : 簡化判斷式

bool Check_QA_ModeUnloadCount();                                                //JerryYang 20221004 : Maxim版本QA mode

extern int iOutPickFromSht;
extern int iOutPlaceToAuto;
void InspectOutArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, int iAction);

extern MyFillTheTrayAfterOutArmPlace mFillTray;                                 //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
bool IsEnableFillTheTrayAfterOutArmPlace();                                     //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
bool IsRunWhichAtuoFillTray(int iWhichAuto);                                    //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
bool HasGapsInTheTray(int iWhichAuto, uPoint2D &pTargetPos, uPoint2D &pSourcePos);
void GetWhichAutoStart(int iWhichAuto, uPoint2D &pStartPos);
int  GetWhichAutoPickZ(int iWhichAuto, int irow, int icol);
int  GetWhichAutoPlaceZ(int iWhichAuto, int irow, int icol);
bool IsOutArmPosOutLimit(uPoint2D pArmPos);
bool MoveOutArmXYAndSuck(SingleSuckPosOnTray ssTarget, uPoint2D pActiveSuck);
bool MoveOutArmXYAndSuck(ArmAndSuckInfo asInfo);
bool IsOutArmArrival(uPoint2D pTargetPos);
bool DoOutArmFillTheTrayAfterPlaceAllIC(bool bIsFirst, int iWhichAuto);
//<==
//Jimmychiu 20240726 : Fill The Tray After Out Arm Place
//Jimmychiu 20240712 : Auto Calibrate Suck Z height
//<==
//action
void InitialAutoCalSuckZAllTask();
bool DoOutArmAutoCalSuckZ(bool bIsFirst);
bool MoveOutArmXYAndSuckUp(int iPosX, int iPosY, int iType);                    //iType 0:A row 1:B row
//<==
//Jimmychiu 20240712 : Auto Calibrate Suck Z height
int GetOutOffsetFromWhichAuto(int iWhichAuto);                                  //Jimmychiu 20240731 : for out arm offset value
void CaculateOutShtXYPos(int iSht, int &iTarX, int &iTarY);
bool InitNewFixTrayForUnloaderClipRead(int iwhichauto,bool bOutputReport=false);                             //Jimmychiu 20250818 : 海康智能讀碼
//AI(ht9045-v899) 20260810: OutArm 逐輪診斷 Log。目的是客戶回報 OutArm 取放/旋轉異常時, 單靠 Task_ListWithTime.csv 只有 task 編號、沒有當下的 kit/座標/吸嘴結果, 無法判定走了哪個分支。這組函式在 RAM 保留最近 OUTARM_ROUND_LOG_MAX 輪, 回 HOME 不會清掉(只重置 task 變數), 發 alarm 與做 State Record 時各 flush 一次到檔案。純觀察, 不參與任何判斷, 且永不丟出例外
#define OUTARM_ROUND_LOG_MAX 20
bool OutArmRoundLog_Enabled();
int  OutArmRoundLog_Seq();
void OutArmRoundLog_Begin(int iSht, int iKit);
void OutArmRoundLog_Line(AnsiString s);
void OutArmRoundLog_Flush(AnsiString sReason);
void OutArmRoundLog_Dump(AnsiString sBasePath);
#endif
