// ===========================================================================
//  LastSet.cpp
//  Standard C++ translation of BCB6 LastSet.cpp (V3.33.906.0_20260618).
//
//  BCB6 source: HT9011UC_Code_V3.33.906.0_20260618/LastSet.cpp (18 lines,
//  cp950) -- decoded via Python open(p,'rb').read().decode('cp950','replace');
//  0 U+FFFD replacement characters on decode.
//  Translation wave: W906-GA1-B1 (GA-1-B1: LastSet full-volume translation)
//  Translator: AI(W906-GA1-B1) 20260804
//
//  TRANSLATED: the four externed global OBJECT DEFINITIONS golden declares
//  for the four persisted-state struct types in LastSet.h -- LastSet, Tech,
//  CmdData, AlignTeach. Nothing else is in the golden file; it has no
//  functions.
//
//  DROPPED (BCB6-project-only plumbing, no standard-C++ equivalent, matches
//  this tree's established convention -- see cprod.cpp's own note doing the
//  same for the same reason):
//    #include "MachineDefine.h"  -- pulls the (mostly #if 0-gated) VCL/driver
//                                   umbrella header; this TU only needs the
//                                   LastSet.h it already includes directly.
//    #pragma hdrstop             -- BCB6 precompiled-header boundary marker.
//    #pragma package(smart_init) -- BCB6 package static-init ordering hint.
//
//  CmdData stays an uninitialised pointer (`INFO *CmdData;`), exactly as
//  golden declares it -- golden never allocates it in this file either;
//  static storage duration zero-initialises it to nullptr in both BCB6 and
//  standard C++, so the observable state is unchanged.
//
//  Toolchain: MinGW g++ 6.3+, C++17 (matches this tree's other W906 units).
// ===========================================================================
#include "LastSet.h"

//---------------------------------------------------------------------------
//這檔案裡的結構千萬不要任意加東西了
//要加也是加在最下面
//有加了東西請跟大家講一下~~
//---------------------------------------------------------------------------
LAST_GENERAL_SET LastSet;
TECH Tech;
INFO *CmdData;                                                                  //JerryYang 20181126 (Steven) : support Epson DLL function
ALIGNTEACH AlignTeach;                                                          //Sam 20181201 : Auto Alignment
