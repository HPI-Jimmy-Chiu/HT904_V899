// =============================================================================
//  aRotateKIT.h  --  shared RotateKit contract (In + Out rotate stations)
//
//  Faithful translation of golden RotateKit/aRotateKIT.h (47 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-ROTKIT) 20260807
//  Translation wave: W906-ROTKIT (pure-translation completion campaign; this is
//  the SHARED header the two big siblings aRotateKIT_In.cpp / aRotateKIT_Out.cpp
//  both include, so it lands with aRotateKIT.cpp ahead of them)
//
//  ROLE: declares the four RotateKit entry points owned by the In/Out units
//  (Initial*/Do*ArmRotateKIT), the cross-unit rotate handshake state
//  (iInRotateFinish / iOutRotateFinish / iIn|iOutRotateWhichKit /
//  iIn|iOutArmRotateKit / i2x2Suck / i2x2Suck_Out), the rotate-kit teach
//  geometry (iRotateKIT_Start|Pitch_X|Y_H|A) and the five free functions this
//  unit's .cpp defines (RotateKIT_FormSet / SetMotorResolution / InitSuckState /
//  CheckRotateSensor / CheckNeedRotateWithoutHotIC).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim): every golden declaration, unchanged.
//   DEFINED BY aRotateKIT.cpp (this unit): iRotateKIT_Start_X_H .. _Pitch_Y_A,
//     i2x2Suck, i2x2Suck_Out, RotateKIT_FormSet, SetMotorResolution,
//     InitSuckState, CheckRotateSensor, CheckNeedRotateWithoutHotIC.
//   DEFINED ELSEWHERE (declaration-only here, exactly as in golden):
//     InitialInArmRotateKIT / DoInArmRotateKIT / iInArmRotateKit /
//     iInRotateWhichKit         -> golden RotateKit/aRotateKIT_In.cpp  (NOT yet
//                                  translated in this tree)
//     InitialOutArmRotateKIT / DoOutArmRotateKIT / iOutArmRotateKit /
//     iOutRotateWhichKit / CheckRotateOutNotFinish
//                               -> golden RotateKit/aRotateKIT_Out.cpp (NOT yet
//                                  translated in this tree)
//     iInRotateFinish  -> golden aRotateKIT_In.cpp:30; in THIS tree it is already
//                         defined at ported acatchtray.cpp:138 (and re-declared
//                         at ported aHotPlateSubstrate.h:1099 -- identical
//                         `extern int`, so the two declarations coexist).
//     iOutRotateFinish -> golden aRotateKIT_Out.cpp:29; in THIS tree already
//                         defined at ported aoutarm9045.cpp:185.
//   GATE (#if 0): NONE in this header.
//
//  GOLDEN QUIRKS CARRIED OVER VERBATIM (see the .cpp banner for the register):
//   * `extern bool bSingleDut;` (golden :42) is declared and NEVER DEFINED
//     anywhere in the golden tree -- its only four appearances are inside
//     commented-out code in golden aRotateKIT_Out.cpp (:822, :1329, :1833,
//     :2927).  Kept as-is; a declaration with no odr-use costs nothing at link.
//   * `bool MoveRotateToDegree(bool,int,bool=false);` (golden :32) is likewise
//     declared here and defined NOWHERE in the golden tree, and called nowhere.
//     Kept verbatim rather than deleted.
//
//  VCL/Borland conversions: none needed -- the golden header has no #include, no
//  __fastcall, no __property, no __published, no VCL type.  It is copied through
//  unchanged below the guard.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
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
