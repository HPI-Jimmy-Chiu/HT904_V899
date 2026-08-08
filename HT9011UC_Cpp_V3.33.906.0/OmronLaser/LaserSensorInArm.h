// =============================================================================
//  OmronLaser/LaserSensorInArm.h  --  InArm hot-plate laser floating-check
//                                     state machine (declarations).
//
//  Faithful translation of golden OmronLaser/LaserSensorInArm.h (32 lines,
//  BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W3-laser) 20260807
//  Translation wave: W906-PT-W3 ("laser" group), file 2 of 3.
//
//  ROLE: declares the two InArm hot-plate laser state machines
//  (InArmLaserInit / CheckInArmFloating) plus the per-cell "needs checking"
//  worklist (TInLaserCheck / LaserCheckPos) that CheckInArmFloating drains.
//  Trivial header -- no VCL type anywhere in golden's own version either.
//
//  WAVE SCOPE: ACTIVE, whole file, verbatim (nothing gated -- `vector<>` is
//  already `using namespace std;`-visible via MachineDefine.h's umbrella,
//  matching every other ported unit that declares a bare `vector<T*>` global).
//
//  VCL/Borland conversions: none needed -- golden itself has none in this
//  header (no __fastcall/__property/VCL include here).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#ifndef LaserSensorInArmH
#define LaserSensorInArmH
//---------------------------------------------------------------------------
#include "MachineType.h"

extern int iInArmLaserInitTask;
extern int iInArmLaserCheckTask;
extern double dInArmCurrentValue[2][50][50];

class TInLaserCheck
{
    public:
        TInLaserCheck(int ip, int x, int y)
        {
            iPlate=ip;
            iX=x;
            iY=y;
        };

        int iPlate;
        int iX;
        int iY;
};

extern vector< TInLaserCheck* > LaserCheckPos;

bool InArmLaserInit(bool bReset=false);         //將讀頭歸零並取得每個位置的數值
bool CheckInArmFloating(bool bReset=false);     //確認每個位置的數值
#endif
