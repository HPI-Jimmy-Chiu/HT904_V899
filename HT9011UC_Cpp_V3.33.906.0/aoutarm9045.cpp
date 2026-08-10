// =============================================================================
//  aoutarm9045.cpp  --  Out-Arm ENGINE  (W6.2c translation)
//
//  Faithful translation of golden  aoutarm9045.cpp (4753 lines, BCB6, Big5/cp950).
//  Translator: AI(W6.2c-OUTARM) 20260626
//
//  ROLE: this is the out-arm ENGINE -- the DIRECT mirror of the in-arm engine
//  (ainarm9045.cpp).  It owns the central place-IC-to-Unloader/Auto cycle SM, the
//  iInArmType dispatch ladders into 26 site-variant modules, the pure pitch/offset
//  geometry, and several auxiliary task SMs.
//
//  WAVE SCOPE (W6.2c) -- what is ACTIVE vs GATED (see each gate's golden cite):
//
//   ACTIVE (faithful, verbatim formulas / branches / cursor flow / fall-throughs):
//     * pure geometry/pitch helpers: GetOutArmPitch_9045, GetOutArmPitch2_9045,
//       GetOutArmPitchY_9045, GetOutArmPitchX_9045, GetOutArmPitchXMM_9045,
//       GetOutArmPitchYMM_9045, BinBoxShiftY.  All use ChangeToFloatNonPcnt
//       (MachineType.h, zero-guarded) -> EXACT numeric semantics incl final
//       (int)r truncation.
//     * the 2 iInArmType dispatch ladders DoOutArm_9045 / DoPickFromShuttle_9045
//       (pre-dispatch guards + if-ladder STRUCTURE; the DoPickFromShuttle final
//       else Program-Error path stays the live offline behavior; DoOutArm_9045
//       has NO final else in golden -> reproduced verbatim).
//     * the 4 pumpable task SMs (switch skeleton + cursor flow + fall-throughs
//       verbatim): DoOutArmAdditionalFunction, DoOutArmPlaceToAuto_9045,
//       DoOutArmAfterPlaceToAuto, DoOutArmIonFanGiveWay.  Inner hardware /
//       cross-module / Magazine / AOI / ATK bodies are gated behind active stubs.
//
//   GATED (compiling ACTIVE stub right after; documented at each site):
//     * the ~26 per-variant DoOutArm_9045_<layout>() / DoPickFromShuttle_9045_
//       <layout>(iSht) / _All_1Picker call-arms (`#if 0 // TODO(W6.2b variants)`)
//       -- variant bodies live in aoutarm9045_<layout>.cpp, out of scope; active
//       stubs (void no-op / return false) declared right after so the ladders link.
//     * the hardware/cross-module/UI surface (MOT[]/sensor/Magazine/AOI/Rotate/
//       SortingBinTray/SetTrayBinData/...) the SMs call -- `// TODO(W7)` ACTIVE
//       stubs (return false/0/no-op/sane default) so the engine LINKS and the SMs
//       pump.  Each stub cites golden file:line.
//
//  CORRECTION to the original task brief: the engine dispatches on the SHARED
//  global `int iInArmType` (golden cmydef.h), NOT an `iOutArmType` (no such
//  symbol exists in this file) -- exactly like the in-arm engine.
//
//  VCL/Borland conversions: #pragma hdrstop / #pragma package(smart_init) dropped
//  (MinGW); AnsiString (1-based) + AnsiString::sprintf via vclcompat (kept
//  verbatim); __FUNC__ via aArmHeader.h W6.0 shim; TQPF_Timer via myTimer.h.
//  SOFT_SIMULTE is NOT defined -> #ifndef SOFT_SIMULTE paths stay active;
//  #ifdef SOFT_SIMULTE paths are kept VERBATIM but inert.  The TMyQASample type
//  (golden aoutarm.h:155, __fastcall ctor) is out of scope -> NOT reproduced.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#include "MachineDefine.h"

#include "aoutarm9045.h"
#include "aArmHeader.h"             // __FUNC__ shim, RecordProcess, ShowErrorMessage, etc.

#include "cprod.h"                 // Prod / TestIF / TestIF_File / OutArmOffSet / ArmSpeed
#include "Motor/mymotor.h"         // MOT[], TMyMotor, TTrayMotor
#include "csystem.h"               // OutSHT1InRT / OutSHT2InRT / ShuttleHasIC / IndexHasIC
#include "cmydef.h"                // global scalar universe + IC consts + iInArmType
#include "cpublic.h"               // CosFunction
#include "common.h"
#include "aHotPlateSubstrate.h"    // OutArmSuck/FRCarryKit/BRCarryKit/OutArm2Suck, cursors, tRotate
#include "FormsFacade.h"           // fMain/fAGV offline stand-ins
#include "canary_support.h"        // LastSet / ShowMyMessage / ShowErrorMessage / RecordProcess / WhichAutoNeedTray
// AI(W906-PT-W4-integrate) 20260809: aoutarm.h is now included DIRECTLY, because the
//   eight file-scope stand-in DEFINITIONS this TU used to carry (iWhichAuto /
//   bOutArmXOverLimit / bOverTray / DoPlaceToAutoDelay / CheckOutSuckICFallDown /
//   SendDataToASE / ErrorBinBoxDetect / PorcessJAM0201OutArmPickUpErrorSkip) were also
//   serving as this TU's DECLARATIONS -- retiring them (aoutarm.cpp landed in PT-W4 with
//   golden's real bodies) left the call sites with no declaration in scope.
//   aoutarm.h declares every one of them (:81 / :87 / :95 / :91 / :153 / :166 / :187 /
//   :170) and that header shipped with aoutarm.cpp in the same wave.
//   WHY DIRECTLY, and not by ungating: golden reaches aoutarm.h through aArmHeader.h,
//   but this tree's aArmHeader.h:61 sits inside the `#if 0` block opened at :23 that
//   also pulls ~40 per-site arm headers -- ungating it is a W7 decision, not this
//   integrate step's.  Same include, narrowest possible route.
#include "aoutarm.h"               // golden reaches this via aArmHeader.h:61 (still gated here)

//==============================================================================
//  golden file globals (verbatim)  -- golden aoutarm9045.cpp:44/46/251/285/2133-2136/3862/4607
//==============================================================================
bool bOutArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};   // golden :44

int iBinBoxShiftY=0;                                                            //jou 2012-12-11 support Bin Box  -- golden :46

bool bCheckOutArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col]={{true, true, true, true}, {true, true, true, true}};  // golden :251
bool bCheckOutArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col]={{false,false,false,false},{false,false,false,false}};       // golden :285

bool bOutRotator;                                                               // golden :2133
bool bDoAOI;                                                                    // golden :2134
bool bDoFixAI;                                                                  //Sam 20211220 : 整合 Fix AI AOI  -- golden :2135
int  iOutArmAdditionalFunctionTask;                                             // golden :2136

int  iOutPickFromSht=0;                                                         // golden :3862
int  iOutPlaceToAuto=1;                                                         // golden :3863

int  iOutArmIonFanGiveWayTask;                                                  // golden :4607

//==============================================================================
//==  GATED externs: the ~26 per-layout dispatch callees + the hardware /
//==  cross-module surface the ladders + SMs call.  Golden DECLARES these via the
//==  per-layout headers (aoutarm9045_<layout>.h) / app headers (main.h / acatchtray.h
//==  / Magazine.h / fAOI.h / aRotateKIT_Out.h / cSortCT.h / ...) -- none in scope
//==  this wave.  Below: the real golden prototypes behind `#if 0`, then an ACTIVE
//==  compiling stub block (mirror ainarm9045.cpp:887).
//==============================================================================
// --- W6.2c-OUT batch: 6 site variants made ACTIVE -- their real per-layout
//     externs un-gated here (translated bodies live in aoutarm9045_<layout>.cpp,
//     registered in CMake lib ht9045_sm); their static no-op stubs removed below.
void DoOutArm_9045_1x1_1();      void DoOutArm_9045_1x2_2();     void DoOutArm_9045_1x2_4();
void DoOutArm_9045_1x3_2_14();   void DoOutArm_9045_1x3_4();     void DoOutArm_9045_1x4_2();
bool DoPickFromShuttle_9045_1x1_1(int);     bool DoPickFromShuttle_9045_1x2_2(int);
bool DoPickFromShuttle_9045_1x2_4(int);     bool DoPickFromShuttle_9045_1x3_2_14(int);
bool DoPickFromShuttle_9045_1x3_4(int);     bool DoPickFromShuttle_9045_1x4_2(int);
// --- W6.2c-OUT batch-2: 7 more variants made ACTIVE (LIVE) -- externs un-gated here.
void DoOutArm_9045_1x4_4S();  void DoOutArm_9045_1x4_4();  void DoOutArm_9045_1x4_8();
void DoOutArm_9045_2x1_2();   void DoOutArm_9045_2x2_4();  void DoOutArm_9045_2x2_8();
void DoOutArm_9045_2x3_6_14();
bool DoPickFromShuttle_9045_1x4_4S(int); bool DoPickFromShuttle_9045_1x4_4(int);
bool DoPickFromShuttle_9045_1x4_8(int);  bool DoPickFromShuttle_9045_2x1_2(int);
bool DoPickFromShuttle_9045_2x2_4(int);  bool DoPickFromShuttle_9045_2x2_8(int);
bool DoPickFromShuttle_9045_2x3_6_14(int);

// --- W6.2c-OUT batch-3 (FINAL): 7 last variants made ACTIVE (LIVE) -- externs
//     un-gated here (translated bodies in aoutarm9045_<layout>.cpp). This empties
//     both the gated-extern and static-stub blocks; all out-arm variants now real.
//     NOTE: All_1Picker is DoOutArm-ONLY (picker-gated dispatch); it has NO
//     DoPickFromShuttle_9045_All_1Picker extern -- matches the dispatch ladder.
void DoOutArm_9045_All_1Picker();
void DoOutArm_9045_2x3_6();      void DoOutArm_9045_2x4_4();
void DoOutArm_9045_2x4_8();      void DoOutArm_9045_2x5_8();     void DoOutArm_9045_2x6_8();
void DoOutArm_9045_2x8_8();
bool DoPickFromShuttle_9045_2x3_6(int);
bool DoPickFromShuttle_9045_2x4_4(int);     bool DoPickFromShuttle_9045_2x4_8(int);
bool DoPickFromShuttle_9045_2x5_8(int);     bool DoPickFromShuttle_9045_2x6_8(int);
bool DoPickFromShuttle_9045_2x8_8(int);

//==============================================================================
//==  GATED hardware / cross-module / UI surface (// TODO(W7)).  ACTIVE stubs so
//==  the kept SMs LINK and pump.  Each cites golden file:line + the offline
//==  rationale.  These have no translated home this wave (MOT[]/sensor/Magazine/
//==  AOI/Rotate/SortingBinTray/SetTrayBinData/cinitial/fProductionInfo/...).
//==============================================================================
//  (i) functions ALREADY declared in shared headers (csystem.h / cprod.h) but
//      with NO translated body anywhere -> DEFINE them here (non-static, matching
//      the header prototype) as offline-safe stubs.
//      CORRECTION (W6.2c verify): ReversionEmptyPoint / EnableFix3UseCylinder are
//      DECLARED in cprod.h but their cprod.cpp bodies (golden cprod.cpp:1452/1484)
//      sit inside cprod.cpp's `#if 0 // TODO(W6)` block (opened cprod.cpp:184) --
//      so they are NOT in ht9045_globals.  DEFINE offline-safe stubs here matching
//      the cprod.h prototype, faithful to the golden offline end-state.  When
//      cprod.cpp later ungates them, remove these two (will become duplicates).
// AI(W906-GA1-B2-integrate) 20260804: the two offline stand-ins that lived here
// (EnableFix3UseCylinder / ReversionEmptyPoint) are RETIRED -- cprod.cpp's real
// bodies were ungated by GA-1-B2, exactly the removal this file's own
// CORRECTION note anticipated. Declarations still come from cprod.h.
#if 0   // PT-W5c RETIRED (CheckOutArmSuckICFallDown)
//AI(ht9045-v906) 20260809: PT-W5c -- RETIRED. The real faithful body now lives in csystem.cpp (its golden home); keeping this stand-in is a multiple-definition error, measured in build_0809_w5c. Same convention as csystem_shims.cpp:165.
bool CheckOutArmSuckICFallDown()                   { return false; }  // golden csystem.h:84 -- offline: no fall-down
#endif
// AI(W906-PT-W4-integrate) 20260809: 8 STAND-IN DEFINITION(S) RETIRED FROM HERE.
//   aoutarm.cpp (golden's own home for all of them) landed in wave PT-W4 and is
//   registered in ht9045_sm, so both definitions were in libht9045_sm.a and every
//   executable linking it failed with `multiple definition of ...`. The linker named
//   each one, which is also the proof the signatures match exactly -- a decorated-name
//   collision cannot happen otherwise.
//   Retired here: CheckOutSuckICFallDown, SendDataToASE, ErrorBinBoxDetect, PorcessJAM0201OutArmPickUpErrorSkip, iWhichAuto, bOutArmXOverLimit, bOverTray, DoPlaceToAutoDelay
//   BEHAVIOUR: these were offline defaults (return true/false/0/no-op); the real bodies
//   run golden's actual logic, so out-arm paths that used to short-circuit now execute.
//   That is the point of the wave, and it is why this wave was measured on its own.
#if 0   // PT-W5c RETIRED (DetectAutoTray)
//AI(ht9045-v906) 20260809: PT-W5c -- RETIRED. The real faithful body now lives in csystem.cpp (its golden home); keeping this stand-in is a multiple-definition error, measured in build_0809_w5c. Same convention as csystem_shims.cpp:165.
bool DetectAutoTray(int,int*)                      { return true;  }  // golden csystem.h:232 -- offline: tray detected
#endif
#if 0   // PT-W5f RETIRED (MagazineBufferIsEmpty)
//AI(ht9045-v906) 20260810: PT-W5f -- RETIRED. csystem.cpp wave 2 landed the real faithful body in its golden home; keeping this stand-in is a multiple-definition error. Same convention as csystem_shims.cpp:165. *** BEHAVIOUR DELTA, NOT NEUTRAL: the stub answered "buffer empty" unconditionally, so retiring it lets the real check actually run and it can now REFUSE. ***
bool MagazineBufferIsEmpty(int)                    { return true;  }  // golden csystem.h:290 -- no magazine present
#endif
#if 0   // PT-W5f RETIRED (WhichMagazineBufferIsFull)
//AI(ht9045-v906) 20260810: PT-W5f -- RETIRED. csystem.cpp wave 2 landed the real faithful body in its golden home; keeping this stand-in is a multiple-definition error. Same convention as csystem_shims.cpp:165.
int  WhichMagazineBufferIsFull()                   { return -1;    }  // golden csystem.h:289 -- no magazine full
#endif
// -- W6.2c-OUT ADD: out-arm engine cross-module surface the 6 site-variant
//    DoOutArm/DoPickFromShuttle SMs call (golden aoutarm.h:124 / aoutarm9045.cpp
//    Steven 20161214).  No translated home yet -> offline-safe non-static defs so
//    the variant TUs link.  TODO(W7): wire to the real bin-box / JAM0201 skip SMs.
//  (ii) functions with NO target home at all -> file-local static stubs.
//
//  AI(W906-PT-W4-integrate) 20260809: NINE OF THESE STATICS ARE RETIRED, and the
//  compiler is what forced the issue rather than a judgement call. `static` gave them
//  internal linkage, so while aoutarm.cpp's real bodies were absent they linked
//  cleanly AND SILENTLY SHADOWED anything with the same name -- the same invisible
//  defect class as a test defining its own empty copy. The moment this TU started
//  including aoutarm.h (see the include block above, needed because eight retired
//  stand-in definitions were also serving as declarations), every one of them became
//  `declared 'extern' and later 'static'`, a hard error, plus one ambiguous overload.
//  Retired, with the aoutarm.h declaration that now governs each:
//      MoveOutArmToAutoSafe          aoutarm.h:115
//      MoveOutArm2XYToDecayTeach     aoutarm.h:106
//      MoveOutArmXY_ToFix_Tray_Full  aoutarm.h:104  (no-arg static made the calls at
//                                    :1253/:1289 ambiguous against the real
//                                    `bool bMoveY=false` form -- they now reach it)
//      IfUseOnebyOne                 aoutarm.h:164
//      InitOutArmTask                aoutarm.h:97
//      IsCatchTrayReadySupplyNewTray aoutarm.h:127
//      CheckOutArmCleanOut           aoutarm.h:121
//      SetOutArmHome                 aoutarm.h:160
//      CheckUseFixBinBoxFunction     aoutarm.h:183
//  BEHAVIOUR: all nine were offline defaults (true / false / no-op / echo the Task
//  argument back). This TU's out-arm SMs now call golden's real bodies in aoutarm.cpp,
//  so paths that used to short-circuit now execute. That is the wave's whole point.
//  KEPT (aoutarm.h does not declare them, so no clash and no real body to reach):
//  MoveOutArmZToPlateSafe / DoOutArmPlaceToAuto / DoFix3FullTray /
//  InitialDoPickFromMagazineBuffer -- checked name by name, not assumed.
static bool MoveOutArmZToPlateSafe()               { return true;  }  // golden -- offline: Z reaches plate-safe immediately
//  OutArmContinuousMove_9045 is REAL-declared in Motor/mymotor.h (8 params,
//  bLoader=false default) + sim-defined in mymotor.cpp (returns false) -> use it.
static bool DoOutArmPlaceToAuto(int)               { return true;  }  // golden aoutarm9045.cpp dispatch -> offline: placed
static bool DoFix3FullTray()                       { return true;  }  // golden -- offline: full-tray done
static void InitialDoPickFromMagazineBuffer()      {}                 // golden Magazine.h -- no magazine present
//AI(W906-PT-W1-integrate) 20260807: TODO(W7) -- the REAL engine behind the next
//  five stubs now exists: SortingBinTray/SortingBinTray.cpp landed in wave PT-W1
//  and is compiled into ht9045_sm.  These file-local statics still shadow it, so
//  that unit currently has zero callers.  Retiring them means deleting these five
//  lines AND ungating aArmHeader.h:23-98's `#include "SortingBinTray.h"`, which
//  pulls the whole arm god-header -- a W7 decision, deliberately not taken in the
//  wave's integrate step.  The same note is in SortingBinTray.cpp's banner.
static void DoSortingBinTray(int)                  {}                 // golden cSortCT.h -- nothing to sort
static bool DoSortingBinTray()                     { return true;  }  // golden cSortCT.h -- sort done
static void SortingBinTray_SetMotorPosData()       {}                 // golden cSortCT.h
static void SortingBinTray_SetTrayData(int)        {}                 // golden cSortCT.h

//  (iii) golden file globals with NO target home -> offline definitions here.
int        iOutRotateFinish=0;          //kevin 20130524        -- golden (out-rotate handshake)

//==============================================================================
//==  ACTIVE: BinBoxShiftY  -- golden aoutarm9045.cpp:47 (trivial)
//==============================================================================
void BinBoxShiftY()                                                             //jou 2012-12-11 support Bin Box
{
    iBinBoxShiftY=0;
}

//==============================================================================
//==  ACTIVE: pure geometry / pitch helpers (faithful, verbatim formulas)
//==============================================================================
//------------------------------------------------------------------------------
int GetOutArmPitch_9045(int w)                                                  // golden :157
{
    double m, r;
    m=(double)ChangeToFloatNonPcnt((double)((Prod.iOutArmX120Pitch-Prod.iOutArmX40Pitch)), (double)(iPitch_Max_minus_Min));                                     //Isaac 20171204 (Steven) : Xpitch40->50mm, 新增150mm的Teaching點位，內差法公式計算
    r=(double)Prod.iOutArmX40Pitch+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//------------------------------------------------------------------------------
int GetOutArmPitch2_9045(int w)                                                 //Steven 20131002 : XY變距  -- golden :165
{
    double m, r;                                                                //Jerryyang 20180205 float -> double
    m=(double)ChangeToFloatNonPcnt((double)((Prod.iOutArmX120Pitch2-Prod.iOutArmX40Pitch2)), (double)(iPitch_Max_minus_Min));                                   //Isaac 20171204 (Steven) : Xpitch40->50mm     //Steven 20200327 : 修正out arm x-pitch的點位錯誤
    r=(double)Prod.iOutArmX40Pitch2+m*((double)w-double(iXpitchMinX3));
    return (int)r;
}
//------------------------------------------------------------------------------
int GetOutArmPitchY_9045(int w, int iOffsetPos)                                 //Steven 20131002 : XY變距  -- golden :173
{
    if(iOffsetPos>=0)                                                           //Jimmychiu 20240731 : for out arm offset value
        w+=OutArmOffSet[iOffsetPos]->GetVariableY();
    double m=0.0, r=0.0;
    m=ChangeToFloatNonPcnt((double)(Prod.iOutArmY60Pitch-Prod.iOutArmY15Pitch), (double)(6000-IN_OUT_ARM_Y_PITCH_MIN)); //Steven 20260505 : add zero-guard
    r=(double)Prod.iOutArmY15Pitch+m*((double)w-(double)IN_OUT_ARM_Y_PITCH_MIN);
    return r;
}
//------------------------------------------------------------------------------
int GetOutArmPitchX_9045(int w, int iX, int iOffsetPos)                         //Steven for HT1032  -- golden :183
{
    if(iOffsetPos>=0)                                                           //Jimmychiu 20240731 : for out arm offset value
        w+=OutArmOffSet[iOffsetPos]->GetXPitch(iX);
    double m=0.0, r=0.0;
    double dX120[4]={Prod.iOutArmX120Pitch, Prod.iOutArmX120Pitch2, Prod.iOutArmX120Pitch3, Prod.iOutArmX120Pitch4};
    double dX_40[4]={Prod.iOutArmX40Pitch,  Prod.iOutArmX40Pitch2,  Prod.iOutArmX40Pitch3,  Prod.iOutArmX40Pitch4};
    m=ChangeToFloatNonPcnt((double)(((double)(dX120[iX]-dX_40[iX]))), (double)(iPitch_Max_minus_Min));
    if(USE_PICKER_COUNT==ep16Picker)
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX7));
    else
        r=(double)dX_40[iX]+m*((double)w-double(iXpitchMinX3));
    return r;
}
//==============================================================================
int GetOutArmPitchXMM_9045(int iMotPulse)                                       // golden :3808
{
    double m=0.0, r=0.0;
    m=ChangeToFloatNonPcnt((double)(((double)(Prod.iOutArmX120Pitch-Prod.iOutArmX40Pitch))), (double)(iPitch_Max_minus_Min));
    r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iOutArmX40Pitch)), (double)(m));
    r+=double(iXpitchMinX3);
    return (int)r;
}
//==============================================================================
int GetOutArmPitchYMM_9045(int iMotPulse)                                       // golden :3817
{
    double m=0.0,r=0.0;
    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                           //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        m=ChangeToFloatNonPcnt((double)(Prod.iOutArmY60Pitch-Prod.iOutArmY15Pitch), (double)(6000-IN_OUT_ARM_Y_PITCH_MIN)); //Steven 20260505 : add zero-guard
        r=(double)ChangeToFloatNonPcnt((double)((iMotPulse-Prod.iOutArmY15Pitch)), (double)(m));
        r+=double(IN_OUT_ARM_Y_PITCH_MIN);
    }
    else
    {
        r=TestIF.iARM_Y_PITCH;
    }

    return (int)r;
}
//==============================================================================
//==  GetOutArmCellPos / GetOutArmToShtCellPos -- golden :3834 / :3848.  Keep the
//==  pitch-math STRUCTURE active; gate the MOT[].ReadPos() encoder reads (mirror
//==  in-arm GetInArmCellPos -- those reads have no offline home here).  Offline
//==  the encoders read 0 so the cell positions are pure pitch deltas.
//==============================================================================
void GetOutArmCellPos(int iRow, int iCol, int &Ypos, int &Xpos)                 // golden :3834
{
    int iPitchPulse=0, iPitchPulseY=0, iBaseX=0, iBaseY=0;
#if 0 // TODO(W7) -- golden :3836-3845 MOT[MOutArmPitch/PitchY/X/Y].ReadPos() encoders
    iPitchPulse =MOT[MOutArmPitch ].ReadPos();
    iPitchPulseY=MOT[MOutArmPitchY].ReadPos();
    iBaseX      =MOT[MOutArmX].ReadPos();
    iBaseY      =MOT[MOutArmY].ReadPos();
#endif
    int iMovePitchX=GetOutArmPitchXMM_9045(iPitchPulse);
    int iMovePitchY=GetOutArmPitchYMM_9045(iPitchPulseY);

    if(USE_PICKER_COUNT==ep1Picker)
        iMovePitchX=0;
    else
        iMovePitchX/=3;

    Xpos=iBaseX+(iMovePitchX*(iCol-iOutArmXBase));
    Ypos=iBaseY-(iMovePitchY*(iRow-iOutArmYBase));
}
//==============================================================================
void GetOutArmToShtCellPos(int iRow, int iCol, int &Ypos, int &Xpos)            // golden :3848
{
    int iPitchPulse=0, iPitchPulseY=0, iBaseX=0, iBaseY=0;
#if 0 // TODO(W7) -- golden :3850-3859 MOT[MOutArmPitch/PitchY/X/Y].ReadPos() encoders
    iPitchPulse =MOT[MOutArmPitch ].ReadPos();
    iPitchPulseY=MOT[MOutArmPitchY].ReadPos();
    iBaseX      =MOT[MOutArmX].ReadPos();
    iBaseY      =MOT[MOutArmY].ReadPos();
#endif
    int iMovePitchX=GetOutArmPitchXMM_9045(iPitchPulse);
    int iMovePitchY=GetOutArmPitchYMM_9045(iPitchPulseY);

    if(USE_PICKER_COUNT==ep1Picker)
        iMovePitchX=0;
    else
        iMovePitchX/=3;

    Xpos=iBaseX+(iMovePitchX*(iCol*OutArmSuck.iPickStep-iOutArmXBase));
    Ypos=iBaseY-(iMovePitchY*(iRow-iOutArmYBase));
}

//==============================================================================
//==  GATED hardware helpers kept as ACTIVE compiling stubs (// TODO(W7)).
//==  Golden bodies are MOT[]/OutArmZSafe/ScanMotorStatus/sensor-bound.
//==============================================================================
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- MoveOutArmToAutoSafe_9045()
//  GATED : golden aoutarm9045.cpp:55-155 (101 lines), inert reference text.
//  LIVE  : the MoveOutArmToAutoSafe_9045() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 101 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body is MOT[]/OutArmZSafe/SetOutArmHome/WAR0257 bound.
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:55-155 (101 lines).  GATE G-PTW6c-MoveOutArmToAutoSafe_9045.  NOT COMPILED: the ACTIVE MoveOutArmToAutoSafe_9045() is the body immediately after this #endif.
bool MoveOutArmToAutoSafe_9045()
{
    bool OK=true;
    int iAlarmSuck=0, iAlarmSuck1=0;
    static int iRetryCount=0;
    static int iRetryFail=0;
    int iMot=0;
    #ifndef SOFT_SIMULTE
    int iZPos=0;
    bool bHomeFlag=false;
    AnsiString sData;
    #endif

    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            iMot=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMot].MotorMove(Prod.ZOutArmSafe[i][j])==false)
            {
                OK=false;
                #ifndef SOFT_SIMULTE
                MOT[iMot].ScanMotorStatus();
                bHomeFlag=MOT[iMot].Led[iHomeLed];
                if(bHomeFlag)                                                   //JerryYang 20200924 : Z軸失步偵測
                {
                   iZPos=MOT[iMot].ReadPos();
                   if(iZPos<-200)
                   {
                        sData.sprintf("%s pos %d home sensor on", MOT[iMot].Alias, iZPos);
                        if(CUSTOMER_CODE==CC_ATEC)                              //Steven 20221224 : 艾科暫時不檢查Z軸Home Sensor
                            MyDBIProcess("Motion", sData, "MoveOutArmToAutoSafe_9045");
                        else
                            ShowMyMessage(sData, "", "MoveOutArmToAutoSafe_9045");
                        SetOutArmHome();
                   }
                }
                #endif
            }
        }
    }

    if(OK)
    {
        iAlarmSuck=OutArmZSafe(DETECT_ALL_FLAG);
        if(iAlarmSuck!=-1)
        {
            if(iRetryCount>50)
            {
                iRetryCount=0;
                if(iRetryFail<3)
                {
                    iRetryFail++;
                    SetOutArmHome();
                }
                else
                {
                    ShowErrorMessage("WAR0257", 0, MOutArmX, false, MOT[iAlarmSuck].NumberAlias);                       //出料手臂的吸嘴 %s 沒有在安全位置
                    iRetryFail=0;
                }
                return false;
            }
            else
            {
                iRetryCount++;
                return false;
            }
        }
        iRetryFail=0;
        iRetryCount=0;
    }

    if(OK)
    {
        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                iMot=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
                MOT[iMot].fCMD=false;
            }
        }
    }

    if(OK==true && iAlarmSuck==-1)                                              //Jou 2011-02-09
    {
        iAlarmSuck1=OutArmZSafe(DETECT_SENSOR_FLAG);
        if(iAlarmSuck1==-1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:55-155  (GATE G-PTW6c-MoveOutArmToAutoSafe_9045, end)
bool MoveOutArmToAutoSafe_9045()                                                // golden :55
{
    // golden :57-154 walks OutArmSuck.iMotRow/iMotCol driving MOT[].MotorMove /
    // OutArmZSafe / SetOutArmHome / ShowErrorMessage("WAR0257").  All MOT[]/
    // alarm-bound with no offline home -> gated; offline the arm is "at safe".
    return true;                                                                // TODO(W7) -- golden aoutarm9045.cpp:55 (MOT[]/OutArmZSafe motion)
}
//-----------------------------------------------------------------------------
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- GetOutArmZDownPos_9045()
//  GATED : golden aoutarm9045.cpp:198-249 (52 lines), inert reference text.
//  LIVE  : the GetOutArmZDownPos_9045() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 52 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body reads OutArmSuck.Suck[][].iMyRow/iMyCol + Prod.ZPlace[][].
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:198-249 (52 lines).  GATE G-PTW6c-GetOutArmZDownPos_9045.  NOT COMPILED: the ACTIVE GetOutArmZDownPos_9045() is the body immediately after this #endif.
void GetOutArmZDownPos_9045(int iWhichAuto, bool bNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col])                       //Steven 20230323 : For HT1032
{
    int iR, iC;
    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            iR=OutArmSuck.Suck[i][j].iMyRow;                                    //實體Row   //Steven 20250826 : fixed for out arm offset
            iC=OutArmSuck.Suck[i][j].iMyCol;                                    //實體Col

            if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)
            {
                if(bNeedDown==true)
                {
                    ZDownSel[i][j]=bOutArmSuckActive[i][j];
                }
                else
                {
                    iOutArmXPosition[i][j]=j;
                    bOutArmSuckActive[i][j]=true;
                    ZDownSel[i][j]=false;
                }

                if(ZDownSel[i][j])
                {
                    iZPos[i][j]=Prod.ZPlace[iWhichAuto][iR][iC];
                }
                else
                {
                    iZPos[i][j]=ZSafePos;
                }
            }
            else
            {
                ZDownSel[i][j]=false;
                iZPos[i][j]=ZSafePos;
            }
        }
    }

    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        if(bNeedDown==true)
        {
            iZPos[0][0]=Prod.ZPlace[iWhichAuto][0][0];
        }
        else
        {
            iZPos[0][0]=ZSafePos;
        }
    }
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:198-249  (GATE G-PTW6c-GetOutArmZDownPos_9045, end)
void GetOutArmZDownPos_9045(int iWhichAuto, bool bNeedDown, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col])                       //Steven 20230323 : For HT1032  -- golden :198
{
    // golden :200-248 fills ZDownSel/iZPos from OutArmSuck.Suck[][].iMyRow/iMyCol
    // + Prod.ZPlace[][]/bOutArmSuckActive[][]/ZSafePos.  bOutArmSuckActive[][]
    // has no offline home (per-layout module) -> gate; offline mark every cell
    // "no down" at ZSafePos so the place SM does not drive a phantom Z move.
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:200-248 (OutArmSuck.Suck iMyRow/iMyCol + bOutArmSuckActive[][] + Prod.ZPlace[][])
#endif
    (void)iWhichAuto; (void)bNeedDown;
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            ZDownSel[i][j]=false;
            iZPos[i][j]=ZSafePos;
        }
}
//-----------------------------------------------------------------------------
bool IsCheckOutArmDestroyActiveFinish(int iXPos, int iYPos)                     //2013-08-02 Dell  -- golden :252
{
    #ifdef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_AMKOR_Japan)                                           //RogerYang 20250813 add  (flag undefined -> inert)
        return true;
    #endif
    // golden :258-282 reads MOT[MOutArmX/Y].ReadPos() within iPreRange + walks
    // bCheckOutArmDestroyActiveFinish[][], then MoveOutArmToAutoSafe() if not
    // finished.  MOT[] encoder reads gated; offline destroy is finished.
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:258-282 (MOT[MOutArmX/Y].ReadPos() proximity check)
#endif
    (void)iXPos; (void)iYPos;
    return true;                                                                // offline: destroy-active finished
}
//------------------------------------------------------------------------------
//  CheckOutArmDestroyActive -- golden :287.  Per-nozzle destroy-confirm SM
//  (static iTask[Row][Col]; cases 1/200/300).  The whole body derefs MOT[]/
//  OutArmSuck.Suck[].OnDestroy/OffDestroy/OnSuck/GetStatus + OutArmPordRec[][] +
//  iMMAuto[]/SetTrayBinData -- hardware-bound, no offline home.  GATED whole-body;
//  ACTIVE no-op so DoOutArm_9045's bCheckOutArmDestroyActive[][] guard reads false
//  and dispatch proceeds (mirror in-arm CheckInArmDestroyActive gate).
//------------------------------------------------------------------------------
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- CheckOutArmDestroyActive()
//  GATED : golden aoutarm9045.cpp:287-400 (114 lines), inert reference text.
//  LIVE  : the CheckOutArmDestroyActive() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 114 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body is the per-nozzle destroy-confirm SM (iTask[Row][Col], cases 1/200/300).
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:287-400 (114 lines).  GATE G-PTW6c-CheckOutArmDestroyActive.  NOT COMPILED: the ACTIVE CheckOutArmDestroyActive() is the body immediately after this #endif.
void CheckOutArmDestroyActive()                                                 //jou 981130 start : 確認device確實destroy完成
{
    int iPos[MAX_ARM_Row][MAX_ARM_Col];
    static int iTask[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0}, {0, 0, 0, 0}};
    static int iXPos=0, iYPos=0;                                                //kevin 20120514 開吹氣時xy 馬達移動就關吹氣
    int iXpos;
    int iYpos;
    int iMotZ;
    int iWhichAuto, iCol, iRow, iMotor;
    AnsiString Msg;

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            if(bOutArmCheckDestroyACT[i][j]==false)
            {
                iTask[i][j]=1;
                continue;
            }

            switch(iTask[i][j])
            {
                case 1:
                    bCheckOutArmDestroyActiveFinish[i][j]=false;                //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                    iMotZ=(InOutArmPickerUseMotor==eptUseMotCyn)?MOutArmZA:OutArmSuck.Suck[i][j].iMotNo;
                    iPos[i][j]=MOT[iMotZ].ReadPos();
                    if(iXPos!=MOT[MOutArmX].ReadPos() &&
                       iYPos!=MOT[MOutArmY].ReadPos())                          //kevin 20120514 Xy 移動關吹氣
                    {
                        if(OutArmSuck.Item[i][j]==NULL_IC)                      //Steven 20120507 : 要持續吹氣
                            OutArmSuck.Suck[i][j].OffDestroy();
                    }

                    if(iPos[i][j]>-500)
                    {
                        iTask[i][j]=200;
                        OutArmSuck.Suck[i][j].OffDestroy();
                    }
                    else
                    {
                        if(OutArmSuck.Item[i][j]==NULL_IC)                      //Steven 20120507 : 要持續吹氣
                        {
                            if(ArmSpeed[InArm].bSuckOnDown)                     //jou 2012-10-29 大IC才持續吹氣
                                OutArmSuck.Suck[i][j].OnDestroy();
                            iXPos=MOT[MOutArmX].ReadPos();
                            iYPos=MOT[MOutArmY].ReadPos();
                        }
                    }
                    break;
                case 200:
                    OutArmSuck.Suck[i][j].OnSuck();
                    CheckOutArmDestroyActiveDelay[i][j].SetSecAndOn(ArmSpeed[OutArm].dDestroyCheckTime);                //2013-08-01    Dell    modify  公司統一回吸功能
                    if(ArmSpeed[OutArm].bSuckOnDown==false ||
                       ArmSpeed[OutArm].bDestroyPauseCheck)                                                             //2013-08-01    Dell    modify  公司統一回吸功能
                    {
                        bCheckOutArmDestroyActive[i][j]=true;                                                           //Steven 20120507 : 要偵測完才可以動，會影響UPH
                    }

                    iTask[i][j]=300;
                    break;
                case 300:
                    if(CheckOutArmDestroyActiveDelay[i][j].Off())
                    {
                        bResetOutArm=false;
                        if(OutArmSuck.Suck[i][j].GetStatus())
                        {
                            iXpos=MOT[MOutArmX].ReadPos();
                            iYpos=MOT[MOutArmY].ReadPos();
                            ShowErrorMessage("WAR0258", 0, MOutArmX, false, OutArmSuck.Suck[i][j].sName);               //破壞錯誤, 請將IC取下

                            iWhichAuto=OutArmPordRec[i][j].GetWhcihAuto();
                            iCol=OutArmPordRec[i][j].GetAutoX();
                            iRow=OutArmPordRec[i][j].GetAutoY();
                            if(iWhichAuto>=0)                                                                           //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
                            {
                                iMotor=iMMAuto[iWhichAuto];

                                if(IniConfig.bE65_ClearTrayDataWhenOutArmDestoryErr)
                                {
                                    MOT[iMotor].SetTrayBinData(iCol, iRow, NULL_IC, AnsiString(""));

                                    if(iWhichAuto<0)
                                        Msg.sprintf("Clear unloader data error");
                                    else if(iWhichAuto<iAutoCnt)
                                        Msg.sprintf("Clear data of Auto%d, X:%d, Y%d", iWhichAuto+1, iCol, iRow);
                                    else
                                        Msg.sprintf("Clear data of Fix%d, X:%d, Y%d", iWhichAuto+1, iCol, iRow);

                                    MyDBIProcess("Process", Msg);
                                }
                                OutArmPordRec[i][j].bUse=true;
                                OutArmPordRec[i][j].AddErrorRecord("WAR0258", true, iXpos, iYpos);
                            }
                            bCheckOutArmDestroyActive[i][j]=true;
                            iTask[i][j]=200;                                                                            //Steven 20120507 : 有出錯就繼續吸吸看，看IC有沒有被拿掉。
                        }
                        else
                        {
                            bCheckOutArmDestroyActiveFinish[i][j]=true;                                                 //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                            OutArmSuck.Suck[i][j].Normal();
                            OutArmPordRec[i][j].InitialRecord();                                                        //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
                            bOutArmCheckDestroyACT[i][j]=false;
                            bCheckOutArmDestroyActive[i][j]=false;
                            iTask[i][j]=1;                                                                              //ChungHung 20111229 add 防止跳出後bInArmCheckDestroyACT馬上被設為true 會照成有一次無效
                        }
                    }
                    break;
                default :
                    break;
            }
        }
    }
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:287-400  (GATE G-PTW6c-CheckOutArmDestroyActive, end)
void CheckOutArmDestroyActive()                                                 //jou 981130 : 確認device確實destroy完成
{
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:287-400 (MOT[]/OutArmSuck destroy SM + OutArmPordRec[][] + SetTrayBinData)
#endif
}

//==============================================================================
//==  ACTIVE: iInArmType dispatch ladder #1  DoOutArm_9045   -- golden :402
//==  Pre-dispatch guards + if-ladder STRUCTURE kept ACTIVE & verbatim; the
//==  cross-module guard bodies (fNote / AOA CCD block) are gated with active
//==  fallbacks; the ~21 per-variant call-arms dispatch to the ACTIVE stubs above.
//==  golden DoOutArm_9045 has NO final else -> unmatched iInArmType is a silent
//==  no-op.  Reproduced verbatim.
//==============================================================================
void DoOutArm_9045()                                                            // golden :402
{
    if(IniConfig.bEnableTestingNeedStopAllMotor==true &&                        //jou 2013-09-25 Testing Need Stop All Motor
       IniConfig.bI24TestingNeedStopAllMotor==true &&
       bTestingStopAllMotor==true)
    {
        return;
    }

    if(IniConfig.bAlarmNeedServoOff==true)                                      //JerryYang 20161227 (Steven) 修正發生JAM,servo off後推動out arm, 再重新start會發生點位錯誤
    {
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:413 fNote->bMyServoOffOutArm (TfNote VCL form, no home)
        if(fNote->bMyServoOffOutArm==true)
        {
            return;
        }
#endif
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(ArmSpeed_File[OutArm].bDevicConfirm)                                 //kevin 20170510 (wei) 回吸功能開關
            CheckOutArmDestroyActive();                                         //jou 981130 start : 確認device確實destroy完成
    }
    else
    {
        CheckOutArmDestroyActive();                                             //jou 981130 start : 確認device確實destroy完成
    }

    for(int i=0; i<OutArmSuck.iPickRow; i++)
        for(int j=0; j<OutArmSuck.iPickCol; j++)
            if(bCheckOutArmDestroyActive[i][j]==true)
                return;

    if(IniConfig.bQAMode==true &&
       LastSet.iRunStartMode==rsmQAMode &&
       CosFunction.bQAModeUseUnloadCnt)                                         //JerryYang 20221004 : Maxim版本QA mode
    {
        Check_QA_ModeUnloadCount();
    }

    if(iPauseBackUp!=-1 &&
       OutArmSuck.IsPickSuckFinish()==true &&
       OutArmSuck.IsPickDestroyFinish()==true)
    {
        return;
    }

    if(bResetOutArmTask==true)
    {
        bResetOutArmTask=false;
        InitOutArmTask();
        return;
    }

    // golden :455-532 MACHINE_HAS_AUTO_ALIGNMENT_CCD AOA block.  Its body derefs
    // MOT[MManualTray*].fHasTray / TestIF_File.iAutoAlignment_UseFix[] /
    // CheckOutArmAutoAlignmentTrayModeBeUse / IsCatchTrayReadySupplyNewTray -- all
    // cross-module with no offline home.  Keep the OUTER guard ACTIVE (faithful:
    // offline bEnableAutoAlignment is false -> takes the else, which sets the two
    // flags false); GATE the inner true-branch body.
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
       TestIF.bEnableAutoAlignment==true &&
       (LastSet.iRealDummy==HAS_TRAY || LastSet.iRealDummy==REALLY))            //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    {
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:460-526 (MOT[MManualTray*]/AOA tray-mode block)
        bool bDoOutArmAutoAlignment=false;
        if(bAutoNeedTrayMustFinish) { /* ... golden :462-522 verbatim ... */ }
        else if(bRunOutArmAutoAlignment) { return; }
#endif
    }
    else
    {
        bAutoNeedTrayMustFinish=false;
        bRunOutArmAutoAlignment=false;
    }

    if(bOutArmLaserActionflag[0]==true || bOutArmLaserActionflag[1]==true)
    {
        return;
    }

    // --- iInArmType dispatch ladder (golden :539-628) ----------------------
    // STRUCTURE kept ACTIVE & verbatim.  The ~21 per-variant callees dispatch to
    // the ACTIVE stubs declared above (variant bodies live in aoutarm9045_<layout>.cpp,
    // W6.2b variants).  The e9045_1x4_4_Back arm is an EMPTY `//` body in golden --
    // kept empty.  golden has NO final else -> silent no-op on unmatched type.
    if(USE_PICKER_COUNT==ep1Picker)
    {
        DoOutArm_9045_All_1Picker();
    }
    else if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        DoOutArm_9045_1x1_1();
    }
    else if(iInArmType==e9045_1x2_2_14 ||
            iInArmType==e9045_1x2_2_13)
    {
        DoOutArm_9045_1x2_2();
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        DoOutArm_9045_1x2_4();
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        DoOutArm_9045_1x3_2_14();
    }
    else if(iInArmType==e9045_1x3_4)                                            //Steven 20220425 : 1x3_14
    {
        DoOutArm_9045_1x3_4();
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        DoOutArm_9045_1x4_2();
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        DoOutArm_9045_1x4_4S();
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
//
    }
    else if(iInArmType==e9045_1x4_4)
    {
        DoOutArm_9045_1x4_4();
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        DoOutArm_9045_1x4_8();
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        DoOutArm_9045_2x1_2();
    }
    else if(iInArmType==e9045_2x2_4_12 ||
            iInArmType==e9045_2x2_4_13 ||
            iInArmType==e9045_2x2_4_14)
    {
        DoOutArm_9045_2x2_4();
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        DoOutArm_9045_2x2_8();
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        DoOutArm_9045_2x3_6_14();
    }
    else if(iInArmType==e9045_2x3_6)
    {
        DoOutArm_9045_2x3_6();
    }
    else if(iInArmType==e9045_2x4_4_13 ||
            iInArmType==e9045_2x4_4_14)
    {
        DoOutArm_9045_2x4_4();
    }
    else if(iInArmType==e9045_2x4_8)
    {
        DoOutArm_9045_2x4_8();
    }
    else if(iInArmType==e9045_2x5_8)                                            //wei 20190614 10 site
    {
        DoOutArm_9045_2x5_8();
    }
    else if(iInArmType==e9045_2x6_8)
    {
        DoOutArm_9045_2x6_8();
    }
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        DoOutArm_9045_2x8_8();
    }
}
//-----------------------------------------------------------------------------
//  CheckOutArmInitState_9045 -- golden :632.  Walks OutArmSuck + CheckSuckInitialStatus
//  + ShowErrorMessage("WAR0226"); MOT[]/CheckSuckInitialStatus hardware-bound.
//  GATED whole-body; offline init-state is OK (no fail) -> true.
//-----------------------------------------------------------------------------
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- CheckOutArmInitState_9045()
//  GATED : golden aoutarm9045.cpp:632-671 (40 lines), inert reference text.
//  LIVE  : the CheckOutArmInitState_9045() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 40 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body walks OutArmSuck + CheckSuckInitialStatus + WAR0226.
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:632-671 (40 lines).  GATE G-PTW6c-CheckOutArmInitState_9045.  NOT COMPILED: the ACTIVE CheckOutArmInitState_9045() is the body immediately after this #endif.
bool CheckOutArmInitState_9045()                                                //Steven 20100104 : 將所有錯誤整合只顯示一次
{
    bool bHasFail=false;
    int ret;
    int flag[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0}, {0, 0, 0, 0}};
    AnsiString errSuck="at Out Arm Suck";
    int iXpos=MOT[MOutArmX].ReadPos();
    int iYpos=MOT[MOutArmY].ReadPos();
    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            flag[i][j]=CheckSuckInitialStatus(OutArmSuck, i, j);
            if(flag[i][j]!=0)
            {
                bHasFail=true;
                errSuck+=OutArmSuck.Suck[i][j].sName;
            }
        }
    }

    if(bHasFail)
        ret=ShowErrorMessage("WAR0226", K_RETRY|K_SKIP, MOutArmX, false, errSuck);

    if(ret==K_SKIP)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(flag[i][j]>=Vaccum_Initial_Off)
                {
                    OutArmSuck.PordRec[i][j].AddErrorRecord("WAR0226", true, iXpos, iYpos);                             //Steven 20160114 (jou) : Add Production Error Log
                    OutArmSuck.SetItemData(i, j, HAS_NULL_IC);
                }
            }
        }
    }
    return !bHasFail;
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:632-671  (GATE G-PTW6c-CheckOutArmInitState_9045, end)
bool CheckOutArmInitState_9045()                                                //Steven 20100104 : 將所有錯誤整合只顯示一次
{
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:632-671 (CheckSuckInitialStatus/MOT[]/ShowErrorMessage WAR0226)
#endif
    return true;                                                                // offline: no suck-init failure
}
//------------------------------------------------------------------------------
void DoOutArmSuckPreOn(int iSht, int iKit, int iXPos, int iYPos, bool bZDown)   // golden :673
{
    // golden :675-709 reads MOT[MOutArmX/Y].ReadPos() proximity + OutArmSuck.Suck
    // pre-on solenoid drive.  Keep the early-return guard ACTIVE & faithful; gate
    // the MOT[]/solenoid body.
    if(bZDown==false || iSht==2)                                                //RogerYang 20250513 Add for 9046AU  //Steven 20230601 : 加入iKit跟bZDown判斷
        return;
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:678-709 (MOT[MOutArmX/Y].ReadPos() proximity + OutArmSuck.Suck pre-on)
#endif
    (void)iKit; (void)iXPos; (void)iYPos;
}
//------------------------------------------------------------------------------
void GetOutShuttleStatus_9045(int iSht, int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bZDown, bool bZFlag[MAX_ARM_Row][MAX_ARM_Col])   // golden :711
{
    // golden :713+ builds the out-shuttle Z status grid from OutArmSuck/Prod/MOT[].
    // GATED whole-body; offline default the grid to "no down" at ZSafePos.
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:711+ (OutArmSuck/Prod/MOT[] out-shuttle Z status)
#endif
    (void)iSht; (void)bZDown;
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bZFlag[i][j]=false;
            iZPos[i][j]=ZSafePos;
        }
}

//==============================================================================
//==  ACTIVE: iInArmType dispatch ladder #2  DoPickFromShuttle_9045  -- golden :2038
//==  Pre-dispatch is none; the if-ladder STRUCTURE + the final else Program-Error
//==  are kept ACTIVE & verbatim (the final else is the live offline behavior).
//==  The ~19 per-layout callees dispatch to the ACTIVE stubs above.
//==============================================================================
bool DoPickFromShuttle_9045(int iSht)                                           // golden :2038
{
    AnsiString Str;
    bool bRet=false;
    if(iInArmType==e9045_1x1_1 ||
       iInArmType==e9045_1x4_1_Ac)
    {
        bRet=DoPickFromShuttle_9045_1x1_1(iSht);
    }
    else if(iInArmType==e9045_1x2_2_13 ||
            iInArmType==e9045_1x2_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x2_2(iSht);
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        bRet=DoPickFromShuttle_9045_1x2_4(iSht);
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x3_2_14(iSht);
    }
    else if(iInArmType==e9045_1x3_4)
    {
        bRet=DoPickFromShuttle_9045_1x3_4(iSht);
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        bRet=DoPickFromShuttle_9045_1x4_4S(iSht);
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        bRet=DoPickFromShuttle_9045_1x4_2(iSht);
    }
    else if(iInArmType==e9045_1x4_4_Back ||
            iInArmType==e9045_1x4_4)
    {
        bRet=DoPickFromShuttle_9045_1x4_4(iSht);
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        bRet=DoPickFromShuttle_9045_1x4_8(iSht);
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        bRet=DoPickFromShuttle_9045_2x1_2(iSht);
    }
    else if(iInArmType==e9045_2x2_4_12 ||
            iInArmType==e9045_2x2_4_13 ||
            iInArmType==e9045_2x2_4_14)
    {
        bRet=DoPickFromShuttle_9045_2x2_4(iSht);
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        bRet=DoPickFromShuttle_9045_2x2_8(iSht);
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        bRet=DoPickFromShuttle_9045_2x3_6_14(iSht);
    }
    else if(iInArmType==e9045_2x3_6)
    {
        bRet=DoPickFromShuttle_9045_2x3_6(iSht);
    }
    else if(iInArmType==e9045_2x4_4_13 ||
            iInArmType==e9045_2x4_4_14)
    {
        bRet=DoPickFromShuttle_9045_2x4_4(iSht);
    }
    else if(iInArmType==e9045_2x4_8)
    {
        bRet=DoPickFromShuttle_9045_2x4_8(iSht);
    }
    else if(iInArmType==e9045_2x5_8)
    {
        bRet=DoPickFromShuttle_9045_2x5_8(iSht);
    }
    else if(iInArmType==e9045_2x6_8)
    {
        bRet=DoPickFromShuttle_9045_2x6_8(iSht);
    }
    else if(iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        bRet=DoPickFromShuttle_9045_2x8_8(iSht);
    }
    else
    {
        Str.sprintf("iInArmType=%d", iInArmType);                               //Steven 20220620 : add log message
        ShowMyMessage("Program Error in DoPickFromShuttle_9045()", Str);
    }
    return bRet;
}

//==============================================================================
//==  ACTIVE SM #1: DoOutArmAdditionalFunction  -- golden :2246
//==  DIRECT mirror of DoInArmAdditionalFunction.  Cursor flow (Task transitions)
//==  + the case 1->100 fall-through kept ACTIVE & verbatim.  The cross-module
//==  action bodies (MoveOutArmToAutoSafe/InitialOutArmRotateKIT/InitAOIFunction/
//==  fFixAICCD-> + predicate CheekNeedToDoOutArmAdditionalFunction) are gated; the
//==  offline "nothing to do" path drives Task 1->100->bResult=true (SM converges).
//==============================================================================
void InitDoOutArmAdditionalFunction()                                           // golden :2138
{
    iOutArmAdditionalFunctionTask=1;
}
//------------------------------------------------------------------------------
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- CheekNeedToDoOutArmAdditionalFunction()
//  GATED : golden aoutarm9045.cpp:2143-2244 (102 lines), inert reference text.
//  LIVE  : the CheekNeedToDoOutArmAdditionalFunction() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 102 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body is the Rotate/AOI/FixAI predicate (tRotate/tAOISetup/FrmAOI/fFixAICCD).
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:2143-2244 (102 lines).  GATE G-PTW6c-CheekNeedToDoOutArmAdditionalFunction.  NOT COMPILED: the ACTIVE CheekNeedToDoOutArmAdditionalFunction() is the body immediately after this #endif.
bool CheekNeedToDoOutArmAdditionalFunction()                                    //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
{
    bool bResult=false;
    bool bSupportFixAI=false;
    if(OutArmSuck.bAlreadyRotate==false)
    {
        if(CheckRotateOutNotFinish())
        {
            bOutRotator=true;
            bResult=true;
        }
        else if(USE_ROTATE_KIT==1 &&
                tRotate.ActiveRotate &&
                TrayForm.iRotateKIT_InputType!=0 &&
                (iOutRotateFinish==0 || iOutRotateFinish==1))
        {
            bOutRotator=true;
            bResult=true;
        }
    }

    if(CosFunction.bART_RT_NoRotate && tRotate.bART_RT_NoRotate && bCanRunSCKART)                                       //Sam 20240809 : ART RT No Rotate
        bOutRotator=false;
    if(OutArmSuck.bAlreadyAOI==false)
    {
//        if(tAOISetup.bEnabledAOI ||                                             //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
//           (USE_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableScannerMode!=0) ||
//           (USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0))
        if(tAOISetup.bEnabledAOI ||  //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
           (USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall && ScannerAOIIF.iEnableScannerMode!=0) || //AI(general) 20260411 (RogerYang) : fix eBtnAOI_Uninstall typo, was always false
           (USE_Top_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall && ScannerAOIIF.iEnableTopScannerMode!=0) ||
           FrmAOI->RunTopBottomInspect()==true)  //Jimmychiu 20240322 : Top & Bottom Inspect
        {
            bDoAOI=true;
            bResult=true;
        }
    }

    if(OutArmSuck.bAlreadyFixAI==false)                                         //Sam 20211220 : 整合 Fix AI AOI
    {
        if(TestIF.iTestMode==SingleSite                         ||
           TestIF.iTestMode==QualSite2X2N                       ||
           TestIF.iTestMode==DualSite)
        {
            bSupportFixAI=true;
        }
        else if(TestIF.iTestMode==QualSite2X2)
        {
            if(USE_PICKER_COUNT==0)
            {
                bSupportFixAI=true;
            }
            else if(TestIF.iUseSuckMode==4 && TestIF.dSiteXPitch>iXpitchMinX2)
            {
                bSupportFixAI=true;
            }
            else
            {
                if(TestIF.iUseSuckMode==8 &&
                   TestIF.dSiteXPitch>iXpitchMinX2)                             //Sam 20231107 : 補 Fix AOI 模式
                {
                    bSupportFixAI=true;
                }
            }
        }
       else if(TestIF.iTestMode==QualSite1X4 ||                                 //Sam 20220613: 修正 FixAOI 未動作問題
               TestIF.iTestMode==_12Site2X6  ||                                 //Sam 20240329 : 新增 FixAOI 模式
               TestIF.iTestMode==_16Site2X8)
        {
            //if(TestIF.iUseSuckMode==4)                                        //Sam 20231107 : 補 Fix AOI 模式 Mark
            {
                bSupportFixAI=true;
            }
        }
        else if(TestIF.iTestMode==_8Site2X4 ||
                TestIF.iTestMode==_16Site4X4)
        {
            if(ArmCanSuck4IC(0, true)==false || USE_PICKER_COUNT==0)
            {
            }
            else
            {
               bSupportFixAI=true;
            }
        }

        if(bSupportFixAI &&
           USE_Fix_AI_CCD &&
           TestIF_File.bEnableFix2BGAAICCD &&
           fFixAICCD->NeedToGrabImage())                                        //KaiChen 20190508 ：矽格湖口 AI CCD Function
        {
            bDoFixAI=true;
            bResult=true;
        }
    }

    if(bResult)
    {
        InitDoOutArmAdditionalFunction();
    }
    return bResult;
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:2143-2244  (GATE G-PTW6c-CheekNeedToDoOutArmAdditionalFunction, end)
bool CheekNeedToDoOutArmAdditionalFunction()                                    //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean  -- golden :2143
{
    bool bResult=false;
    // golden :2145-2243 reads OutArmSuck.bAlready{Rotate,AOI,FixAI} + CheckRotateOutNotFinish
    // + USE_ROTATE_KIT/tRotate + tAOISetup/ScannerAOIIF/FrmAOI-> + fFixAICCD-> and
    // sets bOutRotator/bDoAOI/bDoFixAI.  All cross-module (Rotate/AOI/FixAI), no
    // offline home.  Offline: nothing to do -> bResult stays false.
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:2145-2238 (Rotate/AOI/FixAI predicate: tRotate/tAOISetup/ScannerAOIIF/FrmAOI/fFixAICCD)
#endif
    if(bResult)
    {
        InitDoOutArmAdditionalFunction();
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoOutArmAdditionalFunction()                                               //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean  -- golden :2246
{
    bool bResult=false;
    int &Task=iOutArmAdditionalFunctionTask;

    switch(iOutArmAdditionalFunctionTask)
    {
        case 1:
            CheekNeedToDoOutArmAdditionalFunction();
            Task=100;
            // fall-through to case 100 (golden :2255 -- intentional, no break)
        case 100:
            if(MoveOutArmToAutoSafe())
            {
                CheckOutArmSuckICFallDown();
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:2260-2280 (MOT[MInShuttle1/2].fCanMoveR + Rotate/AOI/FixAI Init* dispatch)
                MOT[MInShuttle1].fCanMoveR=true;
                MOT[MInShuttle2].fCanMoveR=true;
                if(OutArmSuck.bAlreadyRotate==false && bOutRotator)
                {
                    if(iOutRotateFinish==0)
                    {
                        InitialOutArmRotateKIT();
                        iOutRotateFinish=1;
                    }
                    Task=10000;
                }
                else if(OutArmSuck.bAlreadyAOI==false && bDoAOI)
                {
                    InitAOIFunction();
                    Task=20000;
                }
                else if(OutArmSuck.bAlreadyFixAI==false && bDoFixAI)
                {
                    fFixAICCD->Fix2AICCDFunction();
                    Task=30000;
                }
                else
#endif
                {
                    // offline: Rotate/AOI/FixAI all "already done" -> nothing to do.
                    bResult=true;
                }
            }
            break;
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:2287-2311 (Rotate/AOI/FixAI action SMs: DoOutArmRotateKIT/DoAOIFunction/fFixAICCD->DoFix2AICCDFunction)
        case 10000:
            if(DoOutArmRotateKIT()==true)
            {
                OutArmSuck.bAlreadyRotate=true;                                 //Steven 20260612 : Fix == to = (was comparison, not assignment)
                bOutRotator=false;
                iOutRotateFinish=2;                                             //kevin 20130524 目前放到 auto tray
                Task=100;
            }
            break;
       case 20000:
            if(DoAOIFunction()==true)
            {
                OutArmSuck.bAlreadyAOI=true;
                bDoAOI=false;
                Task=100;
            }
            break;
       case  30000:
            if(fFixAICCD->DoFix2AICCDFunction()==true)
            {
                OutArmSuck.bAlreadyFixAI=true;
                bDoFixAI=false;
                Task=100;
            }
            break;
#endif
    }
    return bResult;
}
//------------------------------------------------------------------------------
void PreSetOutAdditionalFlag()                                                  //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean  -- golden :2316
{
    // golden :2318-2330 presets OutArmSuck.bAlready{Rotate,AOI,FixAI} from
    // USE_ROTATE_KIT/tRotate/MOT[MOutRotateKit]/tAOISetup/ScannerAOIIF/FrmAOI->/
    // fFixAICCD->.  Offline the additional functions are all "already done" so the
    // additional-fn SM converges immediately.  Keep that faithful end-state.
    OutArmSuck.bAlreadyRotate=true;
    OutArmSuck.bAlreadyAOI=true;
    OutArmSuck.bAlreadyFixAI=true;
}

//==============================================================================
//==  ACTIVE SM #2: DoOutArmPlaceToAuto_9045  -- golden :3077
//==  CENTRAL pumpable out-cycle SM (mirror of in-arm central pick SM).  switch
//==  skeleton + cursor flow + the case 1->10 and case 50->100 FALL-THROUGHS kept
//==  ACTIVE & verbatim.  Inner hardware / cross-module call bodies (CheckOutSuckICFallDown
//==  / SearchTrayToPlace_9045 / SetOutArm_9045 / MoveOutArmToAutoSafe / DoOutArmPlaceToAuto
//==  / DetectAutoTray / MOT[]) dispatch to the ACTIVE stubs above (// TODO(W7)).
//==============================================================================
bool DoOutArmPlaceToAuto_9045()                                                 // golden :3077
{
    int ct;
    bool flag;
    static int iRetry=0;
    int &Task=iPlaceToAutoTask;

    switch(Task)
    {
        case 1:
            bOutArmXOverLimit=false;                                            //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
            InitialFix3CanFullTask();                                           //ChungHung 20140313 add Fix3 can Full Tray
            Task=10;
            // fall-through to case 10 (golden :3090 -- intentional, no break)
        case 10:
            if(CheckOutSuckICFallDown(false)==true)                             //Steven 20210916 : 全時檢查out arm掉料
            {
                Task=220;
                return false;
            }

            iWhichAuto=SearchTrayToPlace_9045();                                //Steven 20160524 : 簡化Out Arm程式碼
            IfUseOnebyOne(iWhichAuto);                                          //Steven 20111101 : 整合成function  //Steven 20210608 : 加入iWhichAuto判斷

            if(AUTO3_IS_MAGAZINE==1 &&                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
               iWhichAuto>=iMagMin  &&
               TestIF_File.iMagFixTrayType==1)
            {
                ct=iWhichBuff;
            }
            else
            {
                ct=iWhichAuto;
            }

            if(AUTO3_IS_MAGAZINE==1 && iMagazineStatus!=0)                      //JerryYang 20250920 : fixed for Multi bin
            {
                return false;
            }

            if(TRAY_ARM_MODE==eAboveCoveyor &&
               (MOT[iMMAuto[ct]].fHasTray==false ||
                (CheckUseFixBinBoxFunction()==false &&                          //kevin 20160912
                 MOT[iMMAuto[ct]].FullIC())))                                   //Steven 20091002 : 當要放IC時，編輯成滿盤會Hang Up，修正
            {
                InitOutArmTask();
                break;
            }

            if(iWhichAuto!=eBulkBox && (EnableFix3UseCylinder()))
            {
                if(UseFix3Cylinder(ct)==false)
                    break;
            }

            flag=SetOutArm_9045();
            if(flag)
            {
                if(MOT[iMMAuto[ct]].fHasTray==false ||
                   MOT[iMMAuto[ct]].FullIC())
                    Task=30;
                else
                    Task=50;
            }
            else                                                                //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
            {
                if(bOverTray==true)
                {
                    MOT[iMMAuto[ct]].SetTray(HAS_IC, __FUNC__);
                    return true;
                }
            }
            break;
        case 30:
            IsCatchTrayReadySupplyNewTray();
            if(AUTO3_IS_MAGAZINE==1 &&                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
               iWhichAuto>=iMagMin  &&
               TestIF_File.iMagFixTrayType==1)
            {
                ct=iWhichBuff;
            }
            else
            {
                ct=iWhichAuto;
            }

            if(MOT[iMMAuto[ct]].fHasTray &&
               MOT[iMMAuto[ct]].FullIC()==false)
                Task=10;
            break;
        case 50:
            if(CheckOutSuckICFallDown(false)==false)                            //Steven 20110216 Start : 整合Alarm Code
            {
                iRetry=0;
                Task=100;
            }
            else
            {
                Task=220;
                return false;
            }
            // fall-through to case 100 (golden :3178 -- intentional, no break)
        case 100:
            if(OutArmNeedCheckOffset(true, iWhichAuto))                         //Steven 20230531 : 簡化判斷式
            {
                Task=200;
                break;
            }

            if(ArmSpeed[OutArm].bSuckOnDown==false &&                           //jou 2010-12-01 start : 小IC時,會偶發性置偏
               ArmSpeed[OutArm].dDestroyPauseTime!=0)
            {
                DoPlaceToAutoDelay.SetSecAndOn(ArmSpeed[OutArm].dDestroyPauseTime);
                Task=110;
            }
            else
            {
                Task=300;
            }
            break;
        case 110:
            if(DoPlaceToAutoDelay.Off()==true)
            {
                Task=300;
            }
            break;
        case 200:
            if(bEnterOffset==false)
            {
                Task=300;
            }
            else
            {
                bEnterOffset=false;
                Task=220;
            }
            break;
        case 220:
            if(MoveOutArmToAutoSafe())
            {
                Task=1;
                if(OutArmSuck.HasIC()==false)                                   //Steven 20220812 : 避免IC掉光後卡死
                    return true;
            }
            break;
        case 300:
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:3222-3227 fAutoTeach->IsRun() (TfAutoTeach VCL form, no home)
            if(CosFunction.bManualSteplAutoTeach &&
               IniConfig.bA56EnableAutoTeachFunciton &&
               fAutoTeach->IsRun())                                             //Jimmychiu 20211020 : Auto alignment mode
            {
                iWhichAuto=iFixRight;
            }
#endif

            if(DoOutArmPlaceToAuto(iWhichAuto)==true)                           //Steven 20110216 : 整合AlarmCode
            {
                Task=400;
            }
            break;
        case 400:
            if(USE_ROTATE_KIT==1 &&
               iRotate_Type==eInOutArm1Motor &&
               USE_PICKER_COUNT==ep1Picker)
            {
                if(MoveOutArmToAutoSafe()==false)
                    break;
            }

            if(CosFunction.bNewAutoTrayDetect==true &&                          //JerryYang 20170822 (Steven) 更改Auto區偵測tray盤時機 避免out arm吸嘴下降時造成誤判
               iWhichAuto<iAutoRight)
            {
                if(MOT[iMMAuto[iWhichAuto]].Tray.FullIC())                      //JerryYang 20170919 (Steven) Auto擺滿的時候會自動退tray,不要再偵測auto tray
                {
                    ReversionEmptyPoint();
                    Task=1;
                    return true;
                }
                else
                {
                    if(MoveOutArmToAutoSafe())
                    {
                        Task=500;
                    }
                }
            }
            else
            {
                ReversionEmptyPoint();
                Task=1;
                return true;
            }
            break;
        case 500:
            if(DetectAutoTray(iWhichAuto, &iRetry)==true)                       //JerryYang 20170822 (Steven) 更改Auto區偵測tray盤時機 避免out arm吸嘴下降時造成誤判
            {
                ReversionEmptyPoint();
                Task=1;
                return true;
            }
    }
    return false;
}

//==============================================================================
//==  ACTIVE SM #3: DoOutArmAfterPlaceToAuto  -- golden :3281
//==  post-place after-action SM (Fix3 full-tray, magazine buffer, ATK-AMR,
//==  clean-out sort, ion-fan give-way handoff, AOI consecutive-fail).  Returns int
//==  iResult.  switch skeleton + cursor flow + the case 1000->2000 (DoFix3FullTray
//==  true) + case 2000->3000 (commented //break) FALL-THROUGHS kept VERBATIM.
//==  Hardware/Magazine/AOI/ATK bodies dispatch to the ACTIVE stubs above.
//==============================================================================
int DoOutArmAfterPlaceToAuto(bool &bCheckSpeed, bool bInitial)                  //Steven 20220526 : 針對放下IC到Unloader後的動作做整合  -- golden :3281
{
    int iResult=0;
    int &Task=iDoOutArmAfterPlaceToAutoTask;
    if(bInitial)
    {
        Task=1;
        InitialFix3CanFullTask();
        return iResult;
    }

    switch(Task)
    {
        case 1:
            if(bAOIConsecutiveFailFlag || bAOIConsecutiveFailPictureFlag || bAOIFailCountFlag)
            {
                Task=6000;
            }
            else if(OutArmSuck.HasRealIC()==false)
            {
                if(IniConfig.bASE_Report)                                       //kevin 20150121 (jou) 高雄日月光IC履歷記錄
                    SendDataToASE(AseIcRecord);                                 //kevin 20161228

                BinBoxShiftY();                                                 //jou 2012-12-11 support Bin Box

                // golden :3306-3323 ATK-AMR branch.  The condition derefs
                // LastSet.iUnloadFixTray (==eAtkTfMoveFixIC) and the body the
                // eAtkTf* enum + MOT[iMMAuto[]].Tray -- none translated this wave.
                // Offline fAGV->IsATK_AMR() is false, so the live path is the
                // else below; GATE the whole ATK-AMR condition+body.
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:3306-3323 (fAGV ATK-AMR: LastSet.iUnloadFixTray/eAtkTf*/MOT[iMMAuto[]].Tray)
                if(fAGV->IsATK_AMR() &&
                        LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                //Steven 20260202 : for ATK AMR
                {
                    for(int i=iFixMin; i<=iFixMax; i++)                         //Fix Tray
                    {
                        if(MOT[iMMAuto[i]].HasRealIC())
                        {
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }
                    }
                    for(int i=iFixMin; i<=iFixMax; i++)                         //AI(ht9045-atk-amr-flow) 20260427 (RogerYang) : clear null IC after sorting
                        MOT[iMMAuto[i]].Tray.ClearNullIC();
                    LastSet.iUnloadFixTray=eAtkTfFixToAMR;                      //AI(ht9045-atk-amr-flow) 20260421 (RogerYang) : activate FixToAMR
                    Task=1;                                                     //EventReport moved to main loop FixToAMR branch
                    iResult=1;
                }
                else
#endif
                {
                    if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1)  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                    {
                        if((MagazineBufferIsEmpty(iPickWhichMag)==false &&
                            iWhichMag==iAuto3MagazineIndex && iAuto3MagazineIndex!=-1) ||
                           WhichMagazineBufferIsFull()!=-1)                     //要把buffer區清完
                        {
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }

                        if(iAuto3MagazineIndex!=-1 &&
                           MagazineBufferIsEmpty(iAuto3MagazineIndex)==false)   //如果buffer區還有AUTO3的Magazine bin別, 要放到AUTO3去
                        {
                            iPickWhichMag=iAuto3MagazineIndex;
                            InitialDoPickFromMagazineBuffer();
                            iResult=11100;
                            return iResult;
                        }
                    }

                    if(CosFunction.bUseFix3FullTray &&                          //Ifor 20161122 add Fix3 Full Tray 流程
                       FIX3_FULL_PLACE==Fix3K_Uninstall &&                      //Steven 20250911 : for 硬體 fix3 full 可以關閉
                       IniConfig.bE55UseFix3FullTray==true &&                   //Ifor 20161122 add 判斷是否有開啟 Fix3 Full Tray 功能
                       MOT[iMMAuto[iSortingTrayFix3]].Tray.HasRealIC()==true)
                    {
                        SortingBinTray_Data.iMAXSUCK_OutArmSuckX=4;
                        SortingBinTray_SetMotorPosData();                       //設定馬達、及點位資料
                        SortingBinTray_SetTrayData(iSortingTrayFix3);           //設定Tray盤資料
                        bFix3HasNullICSet=true;
                        Task=1000;
                    }
                    else
                    {
                        InitialFix3CanFullTask();
                        Task=2000;
                    }
                }
            }
            else
            {
                if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1)      //JerryYang 20221215 : Magazine把fix區當buffer區功能
                {
                    Task=1;
                    iResult=3010;
                }
                else
                {
                    if(EnableFix3UseCylinder())                                 //JimmyChiu 20220927 : Stepper Motor Control in Fix3
                    {
                        iWhichAuto=SearchTrayToPlace_9045();                    //Steven 20160524 : 簡化Out Arm程式碼
                        if(iMMAuto[iWhichAuto]!=MManualTray3)                   //吸嘴還有料要繼續做, 而且不是去Fix3的, 先把氣缸縮回來
                        {
                            InitialFix3CanFullTask();                           //ChungHung 20140313 add Fix3 can Full Tray
                            Task=500;
                        }
                        else
                        {
                            Task=1;
                            iResult=3010;
                        }
                    }
                    else
                    {
                        Task=1;
                        iResult=3010;
                    }
                }
            }
            break;
        case 500:
            if(UseFix3Cylinder(0)==true)                                        //吸嘴還有料要繼續做
            {
                Task=1;
                iResult=3010;
            }
            break;
        case 1000:
            if(DoFix3FullTray())                                                //Ifor 20161122 執行Fix3 Full Tray 整盤功能
            {
                InitialFix3CanFullTask();
                Task=2000;
            }
            else
            {
                break;
            }
            // fall-through to case 2000 (golden :3413 -- only when DoFix3FullTray() true)
        case 2000:
            if(EnableFix3UseCylinder())                                         //JimmyChiu 20220927 : Stepper Motor Control in Fix3
            {
                if(UseFix3Cylinder(0)==true)                                    //吸嘴已經放完料
                {
                    Task=3000;
                }
                else
                {
                    break;
                }
            }
            else
            {
                Task=3000;
            }
//            break;
        case 3000:
            Task=CheckOutArmCleanOut(3100);
            if(ArmSpeed[InArm].bAutoSpeed)                                      //Steven 20110525 : Auto Speed
                bCheckSpeed=true;
            else
                bCheckSpeed=false;
            break;
        case 3099:                                                              //==> Eastsun 20260521 整合
            if(DoOutArmIonFanGiveWay())                                         //Ifor 20241127 add:
            {
                Task=1;
                iResult=100;
            }
            break;
        case 3100:
            if(IniConfig.bA15_1ESDGiveWayFunction==true)
            {
                InitDoOutArmIonFanGiveWayTask();
                Task=3099;
            }
            else
            {
            Task=1;
            iResult=100;
            }

            if(tRotate.ActiveRotate==1 &&
               iRotate_Type==eInOutArm1Motor)                                   //add One sucker with rotate
            {
                iResult+=10;
            }
            break;
        case 5000:
            if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut==true &&    //JerryYang 20170911 (Steven) Clean Out時整盤
               CosFunction.bSortingBinTraywhenCleanOut==true &&                 //是否使用Auto Sorting AutoTray Auto Sorting AutoTray的功能
               bSortingAllBinTrayFinish==false)                                 //是否所有的Bin Tray IC都整盤完成
            {
                DoSortingBinTray(0);                                            //Bin IC排序主流程
                Task=5100;
                break;
            }

            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                if(iCleanOut==1)
                {
                    if(InArmSuck.HasIC()    ||
                       OutArmSuck.HasIC()   ||
                       ShuttleHasIC()       ||
                       IndexHasIC())
                    {
                        if(USE_OUT_SORT_ARM!=eartUninstall)                     //RogerYang 20250516 add for 9046AU
                        {
                            if(OutArm2Suck.HasIC())
                            {
                                Task=1;
                                iResult=1;
                            }
                        }
                        else
                        {
                            Task=1;
                            iResult=1;
                        }
                    }
                }
            }
            break;
        case 5100:                                                              //JerryYang 20150910 Auto Sorting BinTray by Out Arm when Clean Out
            if(DoSortingBinTray()==true)                                        //Bin IC排序主流程
            {
                if(bSortingAllBinTrayFinish==true)                              //是否所有的Bin Tray IC都整盤完成
                {
                    Task=5000;
                }
            }
            break;
        case 6000:
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                if(bAOIConsecutiveFailFlag)
                {
                    ShowMyMessage("AOI Consecutive Fail");
                    bAOIConsecutiveFailFlag=false;
                }

                if(bAOIConsecutiveFailPictureFlag)
                {
                    ShowMyMessage("AOI Consecutive Picture Fail");
                    bAOIConsecutiveFailPictureFlag=false;
                }

                if(bAOIFailCountFlag)
                {
                    ShowMyMessage("AOI Accumulated Count Fail");
                    bAOIFailCountFlag=false;
                }
                Task=1;
            }
            break;
    }

    return iResult;
}

//==============================================================================
//==  ACTIVE SM #4: DoOutArmIonFanGiveWay  -- golden :4613 (Eastsun 20260521)
//==  ESD ion-fan give-way SM.  Substrate-pumpable: gating predicates are
//==  OutArmSuck.HasRealIC()/HasIC + FRCarryKit/BRCarryKit.HasIC + OutSHT1InRT/
//==  OutSHT2InRT (csystem predicates) + InArmSuck/ShuttleHasIC/IndexHasIC +
//==  WhichAutoNeedTray + MOT[MMPlate1/2].HasIC.  Cursor flow + early-out kept
//==  ACTIVE & verbatim (no fall-throughs); only the 2 motion bodies
//==  (MoveOutArmZToPlateSafe / MoveOutArm2XYToDecayTeach) dispatch to the ACTIVE
//==  stubs above, and the MOT[MMPlate1/2].HasIC reads in case 5 are gated.
//==============================================================================
void InitDoOutArmIonFanGiveWayTask()                                            //Eastsun 20260521 整合  -- golden :4608
{
    iOutArmIonFanGiveWayTask=1;
}
//------------------------------------------------------------------------------
bool DoOutArmIonFanGiveWay()                                                    //Eastsun 20260521 整合  -- golden :4613
{
    bool bResult=false;
    int &Task=iOutArmIonFanGiveWayTask;

    if(IniConfig.bA15_1ESDGiveWayFunction==false)
    {
        iOutArmIonFanGiveWayTask=1;
        return true;
    }

    switch(Task)
    {
        case 1:
            if(IniConfig.bA15_1ESDGiveWayFunction==true && OutArmSuck.HasRealIC()==false)
            {
                if(OutArmSuck.HasRealIC()==false)
                {
                    if((FRCarryKit.HasIC() && OutSHT1InRT()) ||
                       (BRCarryKit.HasIC() && OutSHT2InRT()))
                    {
                        Task=5;
                    }
                    else
                    {
                        Task=2;
                    }
                }
                else
                {
                    Task=5;
                }
            }
            else
            {
                Task=5;
            }
            break;
        case 2:
            if(MoveOutArmZToPlateSafe()==true)
            {
                Task=3;
            }
            break;
        case 3:
            if(MoveOutArm2XYToDecayTeach())
            {
                RecordProcess("Out Arm Ion Fan Give Way");
                Task=5;
            }
            break;
        case 5:
            if(IniConfig.bA15_1ESDGiveWayFunction==true)
            {
                if((FRCarryKit.HasIC() && OutSHT1InRT()) ||
                   (BRCarryKit.HasIC() && OutSHT2InRT()))
                {
                    bResult=true;
                    bOutArmIonFanGiveWay=false;
                }
                else if(WhichAutoNeedTray()!=0)
                {
                     bResult=true;
                     bOutArmIonFanGiveWay=false;
                }
                else  if(InArmSuck.HasIC()==false   &&
                         OutArmSuck.HasIC()==false  &&
                         ShuttleHasIC()==false      &&
                         IndexHasIC()==false        )
//                         bOutArmIonFanGiveWay==false )
                {
                    if(bOutArmIonFanGiveWay==false)
                    {
                        bOutArmIonFanGiveWay=false;
                        bResult=true;
                    }
                    else
                    {
                        bool bPlate1HasIC=false, bPlate2HasIC=false;
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:4692-4693 MOT[MMPlate1/2].HasIC() (HP-occupancy encoder read)
                        bPlate1HasIC=MOT[MMPlate1].HasIC();
                        bPlate2HasIC=MOT[MMPlate2].HasIC();
#endif
                        if(iCleanOut==1                 &&
                           bPlate1HasIC==false          &&
                           bPlate2HasIC==false          )
                        {
                            bOutArmIonFanGiveWay=true;
                            bResult=true;
                        }
                    }
                }
            }
            else
            {
                bResult=true;
            }
            break;
    }

    return bResult;
}

//==============================================================================
//==  ACTIVE: InspectOutArmPosition  -- golden :3865.  IniConfig.bE74 early-return
//==  is the live offline behavior (debug flag off by default); the bX/bY compare
//==  body (MOT[] encoders + ShowErrorMessage alarm sink) is gated.
//==============================================================================
// ===========================================================================
//  GOLDEN VERBATIM PAIR -- InspectOutArmPosition()
//  GATED : golden aoutarm9045.cpp:3865-3993 (129 lines), inert reference text.
//  LIVE  : the InspectOutArmPosition() body immediately AFTER the #endif below.  It is UNCHANGED by
//          this wave -- net behaviour change is ZERO.
//  WHY   : the census scored this "translated" because a same-named LIVE body
//          existed, without comparing SIZE.  Golden's 129 lines were NOWHERE in
//          the tree -- lost text, not deferred behaviour.  Now the text EXISTS
//          and is auditable, so a later un-gate is mechanical.
//  NOTE  : golden's body is the bX/bY encoder compare + WAR0263/WAR0264 alarm sink.
//  RULES : nothing inside the gate is fixed, renamed, reflowed or reindented;
//          it is golden's bytes.  Nothing it references had to be made to
//          exist -- no stub, declaration or header was added for it.
//  SHAPE : same pair shape as csystem.cpp MainProc / atester.cpp (PT-W6a/W6b)
//          golden-verbatim gates.
// ===========================================================================
#if 0 // GOLDEN VERBATIM -- golden aoutarm9045.cpp:3865-3993 (129 lines).  GATE G-PTW6c-InspectOutArmPosition.  NOT COMPILED: the ACTIVE InspectOutArmPosition() is the body immediately after this #endif.
void InspectOutArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, int iAction)
{
    if(IniConfig.bE74_InspectArmPosition==false)                                //Jimmychiu 20240408 : debug for inarm position
        return;

    bool bX, bY;
    int iOutPutTray;
    AnsiString sError, sPos="";
    AnsiString sPlace;

    if(iAction==iOutPlaceToAuto)
    {
        sPlace="Place to";
        sPos=s6TrayName[iTarget];

        if(AUTO3_IS_MAGAZINE==1             &&                                  //JerryYang 20221215 : Magazine把fix區當buffer區功能
           TestIF_File.iMagFixTrayType==1   &&
           iTarget>=iMagMin)
        {
            iOutPutTray=iWhichBuff;
        }
        else
        {
            iOutPutTray=iTarget;
        }
    }
    else
    {
        return;                                                                 //Steven 20241220 : 暫時跳過
        sPlace="Pick from";
        if(iTarget==MOutShuttle1)
        {
            sPos="Shuttle 1";
        }
        else if(iTarget==MOutShuttle2)
        {
            sPos="Shuttle 2";
        }
        else if(iTarget==MOutRotateKit)
        {
            sPos="Out Rotator";
        }
    }

    int XEncoder=0, YEncoder=0, HardwarePosX=0, HardwarePosY=0;

    if(OutArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_IC &&
       OutArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_CLEAN_IC)
    {
        if(iTarget==MOutShuttle1 ||
           iTarget==MOutShuttle2)
            GetOutArmToShtCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);
        else
            GetOutArmCellPos(iSuckRow, iSuckCol, YEncoder, XEncoder);

        if(iAction==iOutPlaceToAuto)
        {
            if(iTarget==eBulkBox)
            {
                HardwarePosX=Prod.iOutArmBinBoxX+dOutArmXPitch_1Step*iTargetCol;
                HardwarePosY=Prod.iOutArmBinBoxY-Prod.iOutArmBinBoxY*200;       //待確認
            }
            else                                                                //Jimmychiu 20240731 : remove offset value
            {
                HardwarePosX=Prod.XStart[iOutPutTray][iOutArmYBase][iOutArmXBase]+AutoForm[iOutPutTray]->XPitch*iTargetCol;                                     //Jimmy 20240826 : fixed for E74
                HardwarePosY=Prod.YStart[iOutPutTray][iOutArmYBase][iOutArmXBase]-AutoForm[iOutPutTray]->YPitch*iTargetRow;

                if(USE_PICKER_COUNT==ep1Picker)
                    HardwarePosX=HardwarePosX;
                else
                    HardwarePosX+=((OutArmOffSet[GetOutOffsetFromWhichAuto(iOutPutTray)]->GetVariable())/3)*(iSuckCol-iOutArmXBase);

                HardwarePosY-=OutArmOffSet[GetOutOffsetFromWhichAuto(iOutPutTray)]->GetVariableY()*(iSuckRow-iOutArmYBase);
            }
        }
        else
        {
            if(iTarget==MOutShuttle1 ||
               iTarget==MOutShuttle2)
            {
                GetShtRowColStartPos(iTarget,HardwarePosX,HardwarePosY);
                HardwarePosX+=TestIF.dSiteXPitch*iTargetCol;
                HardwarePosY-=TestIF.dSiteYPitch*iTargetRow;
                int iXoffset=0, iYoffset=0;
                if(iTarget==MOutShuttle1)
                {
                    iXoffset=OutArmOffSet[OutOfsOutSh1]->GetVariable();
                    iYoffset=OutArmOffSet[OutOfsOutSh1]->GetVariableY();
                }
                else                                                            //MOutShuttle2
                {
                    iXoffset=OutArmOffSet[OutOfsOutSh2]->GetVariable();
                    iYoffset=OutArmOffSet[OutOfsOutSh2]->GetVariableY();
                }

                if(USE_PICKER_COUNT==ep1Picker)
                    HardwarePosX=HardwarePosX;
                else
                    HardwarePosX+=(iXoffset/3)*(iSuckCol-iOutArmXBase);

                HardwarePosY-=iYoffset*(iSuckRow-iOutArmYBase);
                if(TestIF.bNS7000kit)                                           //jou 981208 start : NS7000 bias kit
                {
                    int iShiftY=(TestIF.dSiteYPitch==0)?(6000/2):(TestIF.dSiteYPitch/2);
                    if(iTarget==MOutShuttle1)
                        HardwarePosY+=iShiftY;
                    else
                        HardwarePosY-=iShiftY;
                }
            }
            else if(iTarget==MOutRotateKit)                                     //待確認
            {
                HardwarePosX=Prod.iOutArmRotateToUnloaderX;
                HardwarePosY=Prod.iOutArmRotateToUnloaderY;
            }
        }

        int iLimit=100*InputLimit.iOffsetXYHigh;                                //Jimmychiu 20240731 : remove offset value
        bX=MOT[MOutArmX].CheckArmPosArrival(HardwarePosX, XEncoder, iLimit);    //Steven 20240719 : 放寬檢查的範圍
        bY=MOT[MOutArmY].CheckArmPosArrival(HardwarePosY, YEncoder, iLimit);
        if(bX==false || bY==false)
        {
            sError.sprintf("OutArm Suck[%d, %d] Pos(Y=%d, X=%d), \r%s %s [%d, %d] Pos(Y=%d, X=%d)",
                           iSuckRow, iSuckCol, YEncoder, XEncoder, sPlace, sPos, iTargetRow, iTargetCol, HardwarePosY, HardwarePosX);

            ShowMyMessage(sError);
        }
    }
}
#endif // GOLDEN VERBATIM -- golden aoutarm9045.cpp:3865-3993  (GATE G-PTW6c-InspectOutArmPosition, end)
void InspectOutArmPosition(int iTarget, int iSuckRow, int iSuckCol, int iTargetRow, int iTargetCol, int iAction)   // golden :3865
{
    if(IniConfig.bE74_InspectArmPosition==false)                                //Jimmychiu 20240408 : debug for inarm position
        return;

#if 0 // TODO(W7) -- golden aoutarm9045.cpp:3870+ (bX/bY encoder compare + ShowErrorMessage alarm sink)
#endif
    (void)iTarget; (void)iSuckRow; (void)iSuckCol; (void)iTargetRow; (void)iTargetCol; (void)iAction;
}

//==============================================================================
//==  ACTIVE: IsMotorArrival -- golden :3802.  Pure arithmetic via TMyMotor
//==  CheckArmPosArrival; gate the MOT[].ReadPos() encoder read (offline 0).
//==============================================================================
bool IsMotorArrival(int iMot,int iTargetPos)                                    //Jimmychiu 20240726 : Fill The Tray After Out Arm Place  -- golden :3802
{
    int iReadPos=0;
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:3804 MOT[iMot].ReadPos() encoder read
    iReadPos=MOT[iMot].ReadPos();
#endif
    (void)iMot;
    const int iArrivalGap=10;                                                   // golden :3800
    bool b=TMyMotor().CheckArmPosArrival(iTargetPos, iReadPos, iArrivalGap);
    return b;
}

//==============================================================================
//==  ACTIVE: MoveOutArmXY_To_ESDSafePos -- golden :4711.  Keep the bAlarmNeedServoOff
//==  guard + the position math STRUCTURE; gate the fNote / MOT[].ReadPos() reads
//==  and route the final move through the OutArmContinuousMove_9045 stub.
//==============================================================================
bool MoveOutArmXY_To_ESDSafePos()                                               //Eastsun 20260521 整合  -- golden :4711
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : Out Arm咖啡杯解除
    {
#if 0 // TODO(W7) -- golden aoutarm9045.cpp:4724 fNote->bMyServoOffOutArm (TfNote VCL form, no home)
        if(fNote->bMyServoOffOutArm)
        {
            return false;
        }
#endif
    }

    iXPos=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
    iYPos=((Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase])/2);

#if 0 // TODO(W7) -- golden aoutarm9045.cpp:4733-4747 MOT[MOutArmPitch/X2/X3/X4/Y].ReadPos() encoder reads
    iXVariable[0]=MOT[MOutArmPitch].ReadPos();
    if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable ||                                 //JerryYang 20251218 : IN/OUT ARM支援不同模組
       USE_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                               //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MOutArmPitchX2].ReadPos();
        iYVariable   =MOT[MOutArmPitchY].ReadPos();
    }
    else if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                            //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                               //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MOutArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MOutArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MOutArmPitchX4].ReadPos();
        iYVariable   =MOT[MOutArmPitchY].ReadPos();
    }
#endif

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
        return true;
    return false;
}

//==============================================================================
//==  ACTIVE offline stubs for the remaining golden out-arm engine surface.
//==  Each is declared in aoutarm9045.h; the golden body is MOT[]/cross-module/
//==  UI-bound with no translated home this wave.  Offline-safe defaults keep the
//==  engine linkable.  Golden file:line cited per symbol.  TODO(W7).
//==============================================================================
bool DoMoveOutArmXYToPlace_9045(bool&, int, int, bool)   { return false; }      // golden :2333
bool SetOutArm_9045()                                    { return false; }      // golden :2493 (offline: cannot place -> false)
int  SearchTrayToPlace_9045(bool)                        { return 0;     }      // golden :2451 -> iWhichAuto=0 (Auto1)
bool SearchUnLoadTrayUpDown_9045(bool&, int&, int&)      { return false; }      // golden (decl aoutarm9045.h:62)
int  VerifyFixTrayLink()                                 { return 0;     }      // golden :1600
bool InitialOutArmNeedSuck(int)                          { return false; }      // golden :3535
bool UseFix3Cylinder(int)                                { return true;  }      // golden -- offline: Fix3 cylinder reports "in place"
bool OutArmNeedCheckOffset(bool, int)                    { return false; }      // golden -- offline: no offset re-check
bool Check_QA_ModeUnloadCount()                          { return false; }      // golden :70(decl) (offline: QA unload count not reached)
void InitialFix3CanFullTask()                            {}                     // golden :749 (Fix3 cursor owned by aoutarm2.cpp; offline no-op)
