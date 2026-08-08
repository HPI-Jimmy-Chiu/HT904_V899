// =============================================================================
//  OmronLaser/LaserSensorShuttle.h  --  In/Out shuttle laser floating-check
//                                       state machines (declarations).
//
//  Faithful translation of golden OmronLaser/LaserSensorShuttle.h (22 lines,
//  BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W3-laser) 20260807
//  Translation wave: W906-PT-W3 ("laser" group), file 3 of 3.
//
//  ROLE: declares the shuttle-side laser state machines: ShtLaserInit /
//  CheckShtFloating (both shuttle arms' own "measure the golden reference,
//  then keep comparing against it" cycle) and the two In/OutArm-driven
//  golden-capture variants UseInArmCheckShtFloating / UseOutArmCheckShtFloating
//  (called from LaserSensor.cpp's own DoGetLaserValue/DoGetOutLaserValue,
//  golden :1735/:1739/:1762/:1766 and :1817/:1821/:1844/:1848) plus the two
//  motor-position helpers MoveInArmXYToSHLaserPos / MoveOutArmXYToSHLaserPos.
//  Trivial header -- no VCL type anywhere in golden's own version either.
//
//  WAVE SCOPE: ACTIVE, whole file, verbatim.
//
//  VCL/Borland conversions: none needed -- golden itself has none in this
//  header.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#ifndef LaserSensorShuttleH
#define LaserSensorShuttleH
//---------------------------------------------------------------------------
#include "MachineType.h"

extern int iShtLaserInitTask;
extern int iShtLaserCheckTask;
extern int iUseInArmShtLaserCheckTask;
extern int iUseOutArmShtLaserCheckTask;
extern double dCurrentValue[2][MAX_Index_Row][MAX_Index_Col];

bool ShtLaserInit(int iSht, bool bReset=false);         //將讀頭歸零並取得每個位置的數值

bool CheckShtFloating(int iSht, bool bReset=false);     //確認每個位置的數值
bool UseInArmCheckShtFloating(int iSht, bool bReset=false, bool bSetGold=false);     //確認每個位置的數值  3
bool UseOutArmCheckShtFloating(int iSht, bool bReset=false, bool bSetGold=false);
bool MoveInArmXYToSHLaserPos(int iShuttle, int iCol, int iRow);
bool MoveOutArmXYToSHLaserPos(int iShuttle, int iCol, int iRow);
#endif
