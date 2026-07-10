//---------------------------------------------------------------------------
//  atester_ProcessCount.h  --  TESTER/INDEX count/yield-alarm bookkeeping (leaf subset)
//
//  Translation wave: W6.4b-Translate (atester_ProcessCount partial leaf extraction)
//  Translator: AI(W6.4b-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/atester_ProcessCount.h/.cpp (2161 lines, cp950)
//
//  SCOPE (narrow -- do NOT expand): exactly the 14 golden exports this file
//  defines this wave (see atester_ProcessCount.cpp file header for the full
//  per-function golden line-range table + gated-dependency rationale):
//    RecordPiggyBackStartEnd / ProcessPiggyBackFunction / InitialPiggyBackFunction /
//    ProcessSocketPurgeCount / DoLowYieldAlarm / DoLowYieldAlarmFirst /
//    CheckContinuoussFail / ProcessContinuoussFailForATC / GetMultiplierNum /
//    CheckContinuoussPassBySocket / CheckContinuoussPass / ProcessHeadContactCount /
//    ProcessSocketCount / ProcessAutoloadcellMeasureCount.
//
//  Every OTHER golden atester_ProcessCount.cpp export (ProcessShowTestStatus /
//  ProcessCount / ProcessStartTestData / RecordHistroy / ProcessQASampling /
//  ProcessAutoCleanCount / ...) is OUT OF SCOPE this wave -- ProcessShowTestStatus
//  additionally stays declared in the existing atester_shims.h (offline shim,
//  routes to fTestCategory which is untranslated) exactly as today; do NOT
//  re-declare it here.
//---------------------------------------------------------------------------
#ifndef atester_ProcessCountH
#define atester_ProcessCountH

#include "vclcompat/vcl_compat.h"   // AnsiString

void RecordPiggyBackStartEnd(bool bStart);
void ProcessPiggyBackFunction();
void InitialPiggyBackFunction();
void ProcessSocketPurgeCount();

int DoLowYieldAlarm(AnsiString AlarmCode, AnsiString ErrPart);                  //Steven 20180627 (wei) : Low Yield Alarm整合
int DoLowYieldAlarmFirst(AnsiString AlarmCode, AnsiString ErrPart);

void CheckContinuoussFail(int Index);                                           //Steven 20110603 : 整合只叫一次
void ProcessContinuoussFailForATC(int Index);                                   //Steven 20151123 : Continue Fail Temp Offset for ATC

int  GetMultiplierNum();                                                        //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
void CheckContinuoussPassBySocket(int Index);                                   //Steven 20110919
void CheckContinuoussPass(int Index);

void ProcessHeadContactCount(int Index);                                        //Ifor 20160516 京元要求銦片 Life Time 功能
void ProcessSocketCount(int Index);                                             //JerryYang 20170208 (wei) socket contact計數

void ProcessAutoloadcellMeasureCount();                                         //kevin 20190907 Arm 測區次數道量測 功能

#endif
