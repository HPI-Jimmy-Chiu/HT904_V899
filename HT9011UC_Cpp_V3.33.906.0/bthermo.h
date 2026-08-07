// =============================================================================
//  bthermo.h  --  public surface of the handler thermal-control unit.
//
//  Faithful translation of golden bthermo.h (20 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-GA-BTHERMO) 20260807
//  Translation wave: W906-GA-BTHERMO (pure-translation completion campaign).
//
//  ROLE: declares the entry points other units call into the thermal loop --
//  the polling pump (DoThermo / DoThermoReal / DoTemptureControl), the shared
//  Task cursor (iThermoTask), the per-controller SV writers, the heater-enable
//  predicates and the PV-ring reset.  Golden declares nothing else here; the
//  file has no includes and no VCL types, so this translation needs none either.
//
//  WAVE SCOPE -- ACTIVE vs gated:
//   ACTIVE: all 15 golden declarations, verbatim, unchanged names and signatures
//           (default args: none in golden).
//   GATE (#if 0): NONE.  A declaration never fails to link -- only a definition
//           or a call does -- so every one stays declared even where bthermo.cpp
//           had to gate the body's interior.  What IS gated inside the bodies is
//           listed in bthermo.cpp's own GATE REGISTER (29 numbered gates).
//
//  CALLER NOTE (retirement lead, no edit made here): uHeaterThread.cpp:334 has
//  "GATE 2: golden :61 DoThermo() -- bthermo.cpp has NO translated home", and
//  uHeaterThread.cpp:231 carries a TU-local `static bool bGetHeaterUsed(int)`
//  stand-in for the same reason.  Both can be retired against THIS header once
//  bthermo.cpp is in the build; the `static` keeps them collision-free until
//  then.  That file is out of this unit's write scope and was not touched.
//
//  VCL/Borland conversions: none required -- golden bthermo.h has no #pragma, no
//  __fastcall, no __property, no VCL type.  Only the golden include guard name
//  (bthermoH) is kept, so every existing `#include "bthermo.h"` still works.
//
//  Big5: golden's four Chinese trailing comments decoded via cp950 and preserved
//  as UTF-8.  Final gate: ZERO U+FFFD.
// =============================================================================

// BCB6 ORIGINAL header (mirrored for provenance): no includes at all; the file
// is 15 free-function/extern declarations between the guard.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef bthermoH
#define bthermoH
//---------------------------------------------------------------------------
void DoThermo();
void DoSetThermo();
bool bGetHeaterUsed(int Addr);
bool bGet16HeaterUsedTo4Heater(int Addr);   //JerryYang 20160518 矽格湖口 柏均要求16組加熱器要能支援GPIB4組加熱器的指令
extern int iThermoTask;
void ClearAllHotBuffer();
void DoTemptureControl();           //溫控器新架構控制流程
void DoThermoReal();
void DoSetSVOfOmronEJ1N();          //Steven 20120220 : Omron EJ1N溫控器
void DoATC60Temperature();          //ChungHung 21041105 add for ATC6.0 and ATC3.0
double DOUN150ReadTemp(int Addr);   //Ifor 20160507 add ATC 4、8、32 Heat 溫度整合
void DoSetSVOfDTME08();             //JimmyChiu 20210923 : DTME08溫控器
void CheckLBTemp();                 //JerryYang 20220923 : LB吹氣function
double GetFactSetTemp(int Addr, double T);
//AI(W906-GA-BTHERMO) 20260807: golden bthermo.h declares DoSetThermo() (above)
//  but bthermo.cpp does NOT define it -- whole-golden-tree grep finds no
//  definition anywhere.  It is a stale declaration in golden; translated as-is
//  (a declaration alone links fine).  Do NOT invent a body for it.
#endif
