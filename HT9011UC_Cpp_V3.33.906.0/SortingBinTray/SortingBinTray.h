// =============================================================================
//  SortingBinTray.h  --  Unloader Bin-Tray SORTING unit (public surface)
//
//  Faithful translation of golden SortingBinTray/SortingBinTray.h (53 lines,
//  BCB6, Big5/cp950).
//  Translator: AI(W906-SORTBINTRAY) 20260807
//  Translation wave: W906-SORTBINTRAY (pure-translation completion campaign).
//  Sibling of the already-landed out-arm engine (aoutarm9045.cpp, which owns the
//  Clean-Out dispatch that drives DoSortingBinTray / DoFix3FullTray) and of the
//  tray-arm engine (acatchtray.cpp, whose MTrayXCanSafeMove /
//  IsTrayArmMoveAvoidOutArmCrash interlocks this unit consults).
//
//  ROLE: declares the 29 free functions and the 6 state-machine cursors of the
//  unloader bin-tray "整盤" (whole-tray re-sort) unit -- the Out-Arm pass that
//  compacts scattered ICs in an Auto/Fix bin tray at Clean Out, plus the Fix3
//  Full-Tray variant of the same walk.  Three SM cursor families:
//    iTask_DoSortingBinTray / iTask_ToTrayPickIC / iTask_ToTrayPlaceIC
//      -- the generic Auto/Fix bin-tray sort (golden 2014.01.14 Mylin unit).
//    iTask_DoFix3FullTray / iTask_DoPickFix3IC / iTask_DoPlaceFix3IC
//      -- the Fix3 Full-Tray sort (Ifor 20161122).
//  Plus the pure geometry/data helpers (SortingBinTray_CalPos,
//  SortingBinTray_HAS_NULL_IC_Pos, SortingBinTray_DataChange, the FindThe*Line
//  family, OppositeDirection) and the two production-log sinks
//  (SaveProductionRecord / SaveTrayRecord) golden parks in this same unit.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim):  every declaration below -- all 29 golden
//     functions and all 6 golden `extern int` cursors are declared here and
//     DEFINED in SortingBinTray.cpp this wave.  Nothing in this header is a
//     stand-in.
//   SATISFIED-BY-SHIM (so it LINKS): none introduced by this header.
//   GATE (#if 0): NONE in this header.  (The .cpp carries a 5-entry gate
//     register -- see its banner.)
//
//  VCL/Borland conversions: no BCB6 pragmas in the golden header.  No
//  __fastcall / __property / __published anywhere in this unit.  AnsiString
//  (1-based) comes from vclcompat via cmydef.h -> cprod.h -> ... umbrella.
//  Numeric semantics kept EXACT (no int/float rewrites); default arguments
//  (iFlag = 1) reproduced verbatim.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "mytray.h"
//   #include "cmydef.h"
// =============================================================================

#ifndef SortingBinTrayH
#define SortingBinTrayH

#include "mytray.h"                 // TMyTray (Data[][] / HasRealIC / FullIC / PordRec / XItem / YItem)
#include "cmydef.h"                 // TSortingBinTray_PAR / _SUCKDATA / _TRAYDATA / _CALDATA,
                                    //   SortingBinTray_Data, IC consts, K_RETRY/K_SKIP/K_HOME;
                                    //   pulls cprod.h -> TRAY_TYPE_PARA / Prod / AutoForm / Config.h
//---------------------------------------------------------------------------

bool DoSortingBinTray(int iFlag = 1); //Bin IC排序主流程
bool SortingBinTray_ToTrayPickIC(int iFlag = 1); //到Tray取IC流程
bool SortingBinTray_ToTrayPlaceIC(int iFlag = 1); //到Tray放IC流程

void SortingBinTray_TrayDataToSortingBin(int iNowActTray); //將Tray盤的資料轉成SortingBinTray的格式
void SortingBinTray_SortingBinToTrayData(int iNowActTray); //將SOrtingBinTray的資料轉成Tray盤的格式
bool SortingBinTray_IsBinTrayNeedToSorting(TRAY_TYPE_PARA *TraySpec,TMyTray *TrayData); //檢查Bin Tray盤的IC是否需要整盤

void SortingBinTray_SetMotorPosData(); //設定馬達、及點位資料
void SortingBinTray_SetTraySpecData(); //設定Tray盤資料
int OppositeDirection(int iDir); //找出對角的方向編號

bool bOutArmAxisXMove(int iPos); //Out Arm X軸到iPos點位
bool bOutArmAxisYMove(int iPos); //Out Arm Y軸到iPos點位

bool SortingBinTray_CalPos(TSortingBinTray_PAR Parameter,TSortingBinTray_SUCKDATA SuckData,TSortingBinTray_TRAYDATA TrayData,TSortingBinTray_CALDATA *CalData);
void SortingBinTray_DataChange(TSortingBinTray_PAR Parameter,TSortingBinTray_SUCKDATA *SuckData,TSortingBinTray_TRAYDATA *TrayData,TSortingBinTray_CALDATA CalData);
int SortingBinTray_FindTheICLine(TSortingBinTray_PAR Par,TSortingBinTray_TRAYDATA TD);
int SortingBinTray_FindTheSuckLine(TSortingBinTray_PAR Par,TSortingBinTray_TRAYDATA TD,TSortingBinTray_SUCKDATA SD);
int OutArmPickTrayAlarm(int iwhichTray, AnsiString ErrPart);     //JerryYang 20151013 整盤OutArm吸取異常

bool DoFix3FullTray();                                                                                          //Ifor 20161122 Fix3 Full Tray 主流程
bool DoPickTrayIC(int iTray);                                                                                   //Ifor 20161122 add OutArm To Fix3 取IC流程
bool DoPlaceTrayIC(int iTray);                                                                                  //Ifor 20161122 add OutArm To Fix3 放IC流程
void SortingBinTray_SetTrayData(int iTray);                                                                     //Ifor 20161123 add Set Fix3 Tray Data
void SortingBinTray_SetArmZPosData();                                                                           //Ifor 20161123 add 設定馬達及點位吸嘴 Function
void SortingBinTray_TrayDataToSortingBin_SelectSuck(int iNowActTray,int iICType);                               //Ifor 20161123 add 將Tray盤的資料轉成SortingBinTray的格式
void SortingBinTray_SortingBinToTrayData_SelectSuck(int iNowActTray);                                           //Ifor 20161123 add 將SOrtingBinTray的資料轉成Tray盤的格式
bool SortingBinTray_FindICType(TRAY_TYPE_PARA *TraySpec,TMyTray *TrayData,int iICType,int *iXPos,int *iYPos);   //Ifor 20161125 add Find Tray IC Type Position
bool SortingBinTray_MoveToPickPlace(bool bPick,bool ZNeedDown, int iTray);                                      //Ifor 20161222 add 整合 Fix3 Full Tray Move Function

bool SortingBinTray_HAS_NULL_IC_Pos(TSortingBinTray_PAR Parameter, TSortingBinTray_SUCKDATA SuckData, TSortingBinTray_TRAYDATA TrayData, TSortingBinTray_CALDATA *CalData);
int SortingBinTray_FindSuckICStatusLine(TSortingBinTray_PAR Par,TSortingBinTray_TRAYDATA TD,TSortingBinTray_SUCKDATA SD,int iStatus);    //Ifor 20161123 add Find Suck IC Status Line
extern int iTask_DoFix3FullTray;    //Ifor 20161122 Fix3 Full Tray 主流程
extern int iTask_DoPickFix3IC  ;    //Ifor 20161122 add OutArm To Fix3 放IC流程
extern int iTask_DoPlaceFix3IC ;    //Ifor 20161122 add OutArm To Fix3 取IC流程
void SaveProductionRecord(TMyTray *TrayData, AnsiString Name);  //Steven 20200330 : production log by unloader tray存檔
void SaveTrayRecord(int iAuto);

extern int iTask_DoSortingBinTray;
extern int iTask_ToTrayPickIC;
extern int iTask_ToTrayPlaceIC;
//---------------------------------------------------------------------------
#endif
