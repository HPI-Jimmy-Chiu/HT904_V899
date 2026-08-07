// =============================================================================
//  aRotateKIT_In.h  --  IN-side rotate-station (RotateKit) public contract
//
//  Faithful translation of golden RotateKit/aRotateKIT_In.h (30 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-ROTKIT-IN) 20260807
//  Translation wave: W906-ROTKIT-IN (pure-translation completion campaign).
//  Sibling of RotateKit/aRotateKIT.{h,cpp} (the shared RotateKit contract, landed
//  by AI(W906-ROTKIT) the same day) and of the untranslated RotateKit/
//  aRotateKIT_Out.cpp.
//
//  ROLE: declares the IN rotate-station entry points implemented by
//  aRotateKIT_In.cpp -- the cylinder-version and motor-version place/rotate/pick
//  state machines, the per-motor-count rotate movers (M1/M1_1Mot/M2/M4/M8), the
//  in-rotate sensor / angle predicates and the rotation-angle production record.
//
//  GOLDEN DEFECTS CARRIED OVER VERBATIM (do not "fix" -- see the .cpp banner):
//   (a) `bool M_MoveInArmXY_ToRotateKIT();` below (golden :19) declares a
//       ZERO-ARGUMENT overload.  The .cpp defines `bool M_MoveInArmXY_ToRotateKIT
//       (int iKit)` (golden .cpp:876) and every call site passes iKit, so the
//       nullary overload is DECLARED AND NEVER DEFINED anywhere in the golden
//       tree.  Harmless (no odr-use), kept exactly as golden wrote it.
//   (b) `CheckInRotateAnglePostion(int iSuckR, int iSuckC, int iTrayY, int iTrayX)`
//       below (golden :24) names its last two parameters Y-then-X, while the
//       definition (golden .cpp:3804) names them X-then-Y and every call site
//       passes X-then-Y.  Types are identical so it links and behaves as the
//       definition says; only the declaration's parameter NAMES are wrong.
//       Golden's misspelling "Postion" is also kept.
//
//  VCL/Borland conversions: none needed -- the golden header has no #include, no
//  __fastcall, no __property, no __published and no VCL type.  It is reproduced
//  unchanged below the guard.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef aRotateKIT_InH
#define aRotateKIT_InH
//---------------------------------------------------------------------------

void InitialInArmRotateKIT();
bool DoInArmRotateKIT();
bool MoveInArmXY_ToRotateKIT(int iKit);
bool MoveInArmZ_ToRotateKIT_Place(int iKit);
bool MoveInArmZ_ToRotateKIT_Pick(int iKit);

//---------------------------------------------------------------------------
//2013-04-12    Dell :旋轉站;馬達版  底下都是馬達版的Code
//---------------------------------------------------------------------------
bool M_MoveInArmXY_ToRotateKIT();
bool M_MoveInArmZ_ToRotateKIT_Place(int iKit);
bool M_MoveInArmZ_ToRotateKIT_Pick(int iKit, int PickAngle);
bool M_DoInArmRotateKIT_Motor();
bool M_DoShakeInArmRotateKIT();                                                 //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm
bool M_DoInArmRotate_InOutArm();                                                //add One sucker with rotate

int DoRotate_In(int &rCount, bool bReset=false);
int CheckInRotateSensor(bool bReset=false);                                     //Steven 20170504 (wei) : 針對入料旋轉站的Sensor檢查
bool CheckInRotateAnglePostion(int iSuckR, int iSuckC, int iTrayY, int iTrayX);
int M1_DoInRotateMove(int &rCount, bool bReset=false);
int M1_DoInRotateMove1Mot(int &rCount, bool bReset=false);
int M2_DoInRotateMove(int &rCount, bool bReset=false);
int M4_DoInRotateMove(int &rCount, bool bReset=false);                          //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M8_DoInRotateMove(int &rCount, bool bReset=false);                          //Steven 20170504 (wei) : For 8顆獨立旋轉馬達同時轉動
bool MoveInRotateToDegreeAtSameTime(int iDegree, bool bInitial=false);          //Steven 20170329 (wei) : Add individual rotate motor
void RecordRotateAngle(int iSuckR, int iSuckC, int iTrayR, int iTrayC);         //Sam 20221103 : 新增In旋轉角度
#endif
