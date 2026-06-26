// =============================================================================
//  ainarm9045.h  --  In-Arm ENGINE public interface  (W6.2b translation)
//
//  Faithful translation of golden  ainarm9045.h  (BCB6, Big5/cp950).
//  Translator: AI(W6.2b-INARM) 20260626
//
//  Names / signatures / default args preserved VERBATIM so the team can
//  reverse-look-up the BCB6 version.  vclcompat AnsiString (1-based) replaces
//  the VCL AnsiString.  No __fastcall / __property / PACKAGE in this header.
//
//  SCOPE (W6.2b): this header declares the WHOLE golden surface (so callers and
//  the per-variant modules can link against the same names in later waves), but
//  the .cpp this wave keeps ACTIVE only:
//    * pure geometry / picker helpers (GetInArmPitch*, GetJStep, GetShuttleCol
//      ep1 path, FindCentorPointIndex, GetShtStartPos, ...),
//    * the two dispatch ladders DoInArm_9045 / DoInArm_9045_SuckerMap,
//    * the 3 small task SMs whose cursor flow is substrate-pumpable
//      (DoInArmAdditionalFunction / DoInArmIonFanGiveWay /
//       ProcessSCKARTLoadingCount),
//    * the 11 functions aHotPlateSubstrate.cpp previously stubbed.
//  Everything else is an ACTIVE offline stub (return false / no-op / sane
//  default) gated `#if 0 // TODO(W6.2b variants)` / `// TODO(W7)` so the engine
//  LINKS into lib ht9045_sm and the kept SMs pump over the Sim HAL.  See the
//  .cpp leaf-style gate documentation.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045H
#define ainarm9045H

#include "cmydef.h"
//---------------------------------------------------------------------------
extern int  iZPosToCleanKit[MAX_ARM_Row][MAX_ARM_Col];
extern bool bZFlgToCleanKit[MAX_ARM_Row][MAX_ARM_Col];

extern int iMoveToShuttle;
extern int iTempSuckX;
extern int iTempSuckY;

extern bool bUseAxExPicker();
extern bool bUseAxxGPicker();
extern bool bUseACEGPicker();

extern int  GetJStep();
extern int  GetShuttleCol(int iSuckRow, int iSuckCol, bool bOutArm=false);
extern int  GetVariableYInShuttleData();                                        //ChungHung 20131231 alter AutoYPitch
extern int  AutoCalculateInArmYClosePitch(bool bStart=false, bool bCheckIsZero=false);                                  //ChungHung 20131231 alter AutoYPitch
extern int  GetInArmPitch_9045(int w);
extern int  GetInArmPitchX_9045(int w, int iX, int iOffsetPos=-1);              //Steven 20230323 : For HT1032
extern int  GetInArmPitch2_9045(int w);                                         //Steven 20131002 : XY變距
extern int  GetInArmPitchY_9045(int w, int iOffsetPos=-1);                      //ChungHung 20131231 alter AutoYPitch
extern int  GetInArmToLoaderOffset_9045(int iSelRow);

extern int  GetInArmToShuttleOffset_9045(int iSht, int iModeRow, int iModeCol, bool bPitch=true);
extern int  GetInArmXToShuttleOffset_9045(int iSht, int iOffsetPos);
extern int  GetInArmYToShuttleOffset_9045(int iSht, int iOffsetPos);
extern void GetInArmZShtDownPos_9045(int iSht, bool bPlace, bool IncludeZ);     //Steven 20230323 : For HT1032

extern bool SetClosedShtKitToHasNullIC_9045(int iSht, int iKit, AnsiString Func, AnsiString Task);
extern void SetShuttleStatus_9045(int SHT, int iShtRow, int iShtCol, int iSuckRow, int iSuckCol);
extern void SetShuttleToHasNullIC_9045(int iSht, int iKit=-1);
extern void SetInOutArmParameter();
extern void SetArmRowCount();

extern void DoInArm_9045();
extern void DoInArm_9045_Type();                                                //Steven 20201014 : 將DoInArm_9045_Type獨立,避免DoInArm_9045還沒執行就被使用
extern void DoInArm_9045_SuckerMap();
extern void DoInArmSuckPreOn(int iXPos, int iYPos);
extern bool DoInArmPickFromLoadStage_9045();
extern void DoInArmLoadPickUP_9045(AnsiString ErrPart, AnsiString ErrTray);     //kevin 20220823 add log 9046
extern bool DoPickLoaderOK(int iSht, int iKit);
extern bool DoInArmPlaceToShuttle_9045();
extern void AddInArmPickerCount(int iSuckRow, int iSuckCol);                    //JerryYang 20220331 : add 吸真空次數計數
extern int  CheckOneCycleAction(int iTask);                                     //Steven 20240326 : 判斷one cycle的時候要不要繼續放料

extern bool Find_InArm_Single(int iUseSuck, int &iRow, int &iCol);
extern bool SearchAndMoveInArmXYToLoad_9045();
extern bool MoveInArmZToLoaderPick(int Step);                                   //Steven 20160414 : 簡化In Arm程式碼
extern bool MoveArmXYToLoader_9045(bool &bCanPick2ICAtOnceTime, int iSelRow, int &iRow, int &iCol, bool RealMove, bool ZNeedDown=false);
extern bool MoveInArmXYToShuttle_9045(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern int  MoveInArmXYToWaitTrayArm(int iSht, int iKit, bool IncludeZ, bool bPlace);
extern bool MoveInArmZToShuttlePlace_9045(int iSht);

extern void sLoadPickupClean();                                                 //kevin 20220723 initial data
extern void SetAutoSkipCount(int Add);                                          //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數
extern void RecordUPH(int UPH);

extern int iProcessSCKARTLoadingCountTask;

int CheckShuttleSensor_9045_1x1(int Index, bool alarmflag, bool bAutoclean=false);                                      //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
int CheckShuttleSensor_9045_1x4(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x1(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x3(int Index, bool alarmflag, bool bAutoclean=false);                                      //ChungHung 20150119 add for 2x3 mode autoclean
int CheckShuttleSensor_9045_2x4(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_1x2(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x2(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x5(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x6(int Index, bool alarmflag, bool bAutoclean=false);
int CheckShuttleSensor_9045_2x8(int Index, bool alarmflag, bool bAutoclean=false);

bool CheckShuttleSensor_9045(int CheckItem=0);                                  //jou 2010-01-23 for contech do auto-high use

bool IsCheckInArmDestroyActiveFinish();                                         //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
void CheckInArmDestroyActive();                                                 //kevin 20121023
extern bool bCheckInArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col];                 //kevin 20121023

void InArmSuckReset();
bool CheckPlaceToShuttle();                                                     //Steven 20170511 (wei) : 使用initial delay當 Soak time
void ShowAutoSkipError();                                                       //kevin 20170904 (wei) 記錄每一盤 SKIP 沒吸到IC位置
void PickErrorData(int iX, int iY);                                             //kevin 20170904 (wei) 記錄每一盤 SKIP 沒吸到IC位置
bool AutoTrayEndMusic(int iSkeyTime,int iHandMode);                             //kevin 20170904 (wei) add AutoTray 吸到空 發出聲音及警告燈光
void AutoSkipHasIClog(AnsiString ZName,int iX, int iY);                         //kevin 20170904 (wei) 記錄每一盤 SKIP 沒吸到IC位置
void CheckInArmDestroyActiveFinishInitial();                                    //kevin 20170915
void TrayPickupErrorData();                                                     //kevin 20171213 (Steven) 整盤沒有吸到IC位置
bool bCheckYPitchHome(int iXYHome);                                             //kevin 20180822 (Steven) : y pitch 歸home 動作
bool bCheckYPitchRunHomeSen(int iXYHome);                                       //kevin 20180822 (Steven) : y pitch 遮住home
bool SearchLoadTrayUpDown_9045_FIFO_2Step(bool &bCanPick2ICAtOnceTime, int &iStep, int &ix, int &iy, int &NowRow);
bool DoTrayEndProcess_9045();                                                   //Steven 20190703 : 整合Tray End動作
bool DoTraySkipProcess_9045();                                                  //Steven 20190703 : 整合Load Tray Skip動作
bool DoRecordSkipPosition_9045(bool bClearLoaderTray);                          //Steven 20190703 : 整合Load Tray Skip動作

extern bool DoInArmAdditionalFunction();                                        //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
extern bool CheekNeedToDoInArmAdditionalFunction();
extern void InitDoInArmAdditionalFunction();
extern int  iInArmAdditionalFunctionTask;
extern void PreSetInArmAdditionalFlag();                                        //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean

extern bool DoInArmCheckShuttleFloating(int iSht, bool bPlaceOtherShuttl=false, bool bNeedCheck=false);
extern void InitDoInArmCheckShtFloatTask();
extern int iInArmInArmCheckShtFloatTask;
extern int iAutoChkInSHLatchTask;                                               //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
bool bCheckInShuttlePosAndNOIC();                                               //KenHsieh 20230614 : In/Outarm 取放料需待Shuttle到位且為可取放料狀態
bool InitialInArmNeedSuck(int iShuttle, bool bPlace);                           //Steven 20220927 : 重新整理In / Out Arm Need Suck
bool InArmLeftSideNoIC(int iRow=2);
bool InArmLeftSideHasIC(int iRow=2);
bool InArmSideAllClose(int iSht);
bool InArmSideAllCloseWithKit(int iSht, int iKit);
void SetShuttleToHasNullICWhenCleanOut();
void SetShuttleToNullICWhenCleanOut();                                          //Steven 20240326 : 修正Clean時, 都沒IC造成的Hang Up
void InitAutoChkInSHLatchTask();                                                //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
int CheckInShuttleSensor_Latch(int iShuttle, int iMoveAct=0, bool bAlarm=true); //iMoveAct 0=SH 不移動 1=SH 沒異常移動到左   //KenHsieh 20251104 : add 檢測後動作 & alarm 變數
extern void InArmZNeedDown_9045(int iSht, int iKit, bool bPlace);
extern bool InArmNeedCheckOffset(bool bPlace, int iPos);                        //Steven 20230531 : 簡化判斷式
extern void InspectInArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, bool bPlace);
int GetInOffsetFromWhichTarget(int iWhichTarget);                               //Jimmychiu 20240731 : for out arm offset value
//action
bool DoInArmAutoCalSuckZ(bool bIsFirst);
bool MoveInArmXYAndSuckUp(int iPosX,int iPosY,int iType);                       //iType 0:A row 1:B row
void GetShtRowColStartPos(int iTarget,int &iColStart,int &iRowStart);
void GetShtRowColPos(int iTarget, int iColIndex, int iRowindex, int &HardwarePosX, int &HardwarePosY);
extern void RecordLotUPH_For_FOREHOPE_NINGBO(AnsiString UPH_StartTime, AnsiString UPH_EndTime, AnsiString UPH_PauseTime, int iUPH,int iTrayCount,int iOpenSiteNum);
extern bool DoInArmIonFanGiveWay();                                             //Ifor 20241126 add: InArm Ion Fan Give Way
extern void InitDoInArmIonFanGiveWayTask();                                     //Ifor 20241126 add: InArm Ion Fan Give Way
extern int iInArmIonFanGiveWayTask;                                             //Ifor 20241126 add: InArm Ion Fan Give Way  //Eastsun 20260522 add: missing extern
#endif
