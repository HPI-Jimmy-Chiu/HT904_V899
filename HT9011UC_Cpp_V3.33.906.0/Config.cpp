// =============================================================================
//  Config.cpp  --  translation-unit anchor for the HT9045_CONFIG (IniConfig)
//                  struct declared in Config.h.
//
//  Faithful translation of golden Config.cpp (4 lines, BCB6, Big5/cp950):
//      #include "MachineDefine.h"
//      #pragma hdrstop
//
//      #include "Config.h"
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (rootleaf group).
//
//  ROLE: golden Config.cpp defines ZERO functions and ZERO global variables of
//  its own -- it is purely the compilation-unit home BCB6's IDE generated for
//  Config.h's `typedef struct {...} HT9045_CONFIG;` (1499 golden lines, all
//  data-only field declarations) plus the trailing `extern HT9045_CONFIG
//  IniConfig;`.  The struct's actual instance is NOT defined here in golden
//  either (BCB6 lets a header's `extern` be satisfied from any other unit that
//  happens to link); in THIS port tree that instance is already provided by
//  cprod.cpp:50 (`HT9045_CONFIG IniConfig;`, see that file's own ACTIVE-globals
//  banner list), which this wave leaves untouched per the hard rule against
//  editing another unit's .cpp.
//
//  WAVE SCOPE -- ACTIVE vs SATISFIED-BY-SUBSTRATE:
//   ACTIVE: nothing -- golden_fns = 0, ported_fns = 0 (see banner rationale
//     above; there is no function or global-definition gap to close, only the
//     #include chain to mirror).
//   SATISFIED-BY-SUBSTRATE: struct HT9045_CONFIG + `extern HT9045_CONFIG
//     IniConfig;` -- already ported at Config.h (this wave's group brief says
//     "Config.h already exists in the port tree: read it first and make
//     Config.cpp consistent with it, without editing the header" -- confirmed
//     read in full; the struct/extern pair there already matches golden
//     Config.h field-for-field via an earlier wave's cp950->UTF-8 transcode).
//     Definition of `IniConfig` itself: cprod.cpp:50 (ht9045_globals).
//
//  GATE REGISTER: none. No call in this TU reaches an unbodied symbol -- there
//  is no call at all.
//
//  VCL/Borland conversions: #pragma hdrstop dropped (golden :2); MachineDefine.h
//  is this tree's de-VCL'd stand-in for the BCB6 include hub golden's own
//  #include "MachineDefine.h" pulled in (golden Config.cpp :1). No __fastcall /
//  __property / __published / #pragma package in golden Config.cpp to strip.
//
//  Big5: golden Config.cpp carries no Chinese text at all (verified via the
//  cp950 decode this wave performed) -- 0 characters to preserve, 0 U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored verbatim, golden Config.cpp is 4 lines):
//   #include "MachineDefine.h"
//   #pragma hdrstop
//
//   #include "Config.h"
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std)
#include "Config.h"                 // this unit's own contract: struct HT9045_CONFIG + extern IniConfig (already ported, untouched by this wave)
//---------------------------------------------------------------------------
// (intentionally empty past this point -- see banner: golden defines nothing
//  else in this translation unit either.)
