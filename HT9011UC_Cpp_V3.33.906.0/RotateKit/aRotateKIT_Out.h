// =============================================================================
//  RotateKit/aRotateKIT_Out.h  --  OUT-side rotate-kit (旋轉站) public surface
//
//  Faithful translation of golden RotateKit/aRotateKIT_Out.h (36 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-RKOUT) 20260807
//  Translation wave: W906-RKOUT (pure-translation completion campaign; the
//  out-side mirror of RotateKit/aRotateKIT_In.  Translated from the GOLDEN OUT
//  FILE, never copied from the In file -- the two genuinely differ).
//
//  ROLE: declares the out-arm rotate-kit entry points that the out-arm engine
//  (aoutarm9045.cpp DoOutArmAdditionalFunction case 10000) and the shuttle /
//  in-arm interlocks call: the cylinder-and-motor dispatcher DoOutArmRotateKIT,
//  the XY/Z approach helpers for both the cylinder rig and the motor rig, the
//  five per-topology rotate movers (1/1-per-dut/2/4/8 motors), the
//  same-time-degree mover, the two angle predicates and the CheckRotateOutNotFinish
//  interlock.
//
//  GOLDEN DEFECTS PRESERVED IN THIS HEADER (see the .cpp banner for the full
//  register):
//    * golden :16 declares `bool M_MoveOutArmXY_ToRotateKIT();` with NO argument,
//      while golden .cpp:868 defines `bool M_MoveOutArmXY_ToRotateKIT(int iKit)`.
//      In C++ those are two different overloads: the zero-arg one declared here
//      is NEVER defined and NEVER called.  Kept VERBATIM -- removing it would be
//      a "fix".
//    * golden :32 spells CheckOutRotateAnglePostion's last two parameters
//      (int iTrayY, int iTrayX) while golden .cpp:3914 defines them
//      (int iTrayX, int iTrayY) -- same types, so same function; only the names
//      disagree.  Both spellings kept exactly where golden puts them.
//    * "Postion" (sic) is golden's spelling in both predicate names.
//
//  VCL/Borland conversions: none needed -- this header is free-function
//  declarations only (no VCL type, no __fastcall, no __property).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL header text (mirrored as a comment for provenance):
//   //-------------------------------------------------------------------------
//   #ifndef aRotateKIT_OutH
//   #define aRotateKIT_OutH
//   ... declarations ...
//   #endif
// =============================================================================

//---------------------------------------------------------------------------

#ifndef aRotateKIT_OutH
#define aRotateKIT_OutH
//---------------------------------------------------------------------------

void InitialOutArmRotateKIT();
bool DoOutArmRotateKIT();
bool MoveOutArmXY_ToRotateKIT(int iKit);
bool MoveOutArmZ_ToRotateKIT_Place(int iKit);
bool MoveOutArmZ_ToRotateKIT_Pick(int iKit);

//---------------------------------------------------------------------------
//2013-04-12    Dell :旋轉站;馬達版  底下都是馬達版的Code
//---------------------------------------------------------------------------
//AI(W906-RKOUT) 20260807: golden :16 declares the ZERO-ARG overload while golden
//  .cpp:868 defines the (int iKit) one.  Kept VERBATIM as a golden defect; the
//  (int iKit) declaration below is ADDED so the golden .cpp definition has a
//  matching prototype in this TU (BCB6 tolerated a definition with no prior
//  declaration; -Wall on MinGW/MSVC wants one, and adding a declaration that
//  matches the golden DEFINITION cannot change behaviour).
bool M_MoveOutArmXY_ToRotateKIT();
bool M_MoveOutArmXY_ToRotateKIT(int iKit);
bool M_MoveOutArmZ_ToRotateKIT_Place(int iKit);
bool M_MoveOutArmZ_ToRotateKIT_Pick(int iKit, int PickAngle);
bool M_DoOutArmRotateKIT_Motor();
bool M_DoShakeOutArmRotateKIT();                                                //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm
bool M_DoOutArmRotate_InOutArm();                                               //add One sucker with rotate

int DoRotate_Out(int &rCount, bool bReset=false);
int CheckOutRotateSensor(bool bReset=false);
int M1_DoOutRotateMove(int &rCount, bool bReset=false);
int M1_DoOutRotateMove1Mot(int &rCount, bool bReset=false);
int M2_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M4_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M8_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 8顆獨立旋轉馬達同時轉動
bool MoveOutRotateToDegreeAtSameTime(int iDegree, bool bInitial=false);         //Steven 20170329 (wei) : Add individual rotate motor
bool CheckRotateOutAnglePostion(int iRow, int iCol, int icount, int iMode);     //Ifor 20170412 add Check Angle 與各Site位置是否相同
bool CheckOutRotateAnglePostion(int iSuckR, int iSuckC, int iTrayY, int iTrayX);
bool CheckRotateOutNotFinish();

#endif
