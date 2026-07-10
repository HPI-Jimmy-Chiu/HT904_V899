//---------------------------------------------------------------------------
//  SECSGEM/SecsEventType.cpp  --  SECS_EVENT global instance definition
//
//  Translation wave: W5 (SECSGEM base layer)
//  Translator: AI(W5-SECSGEM-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemHT9045.cpp
//  (the `struct ETypeStruct SECS_EVENT;` definition -- golden defines it once
//  at file scope; the exact defining line was not hunted down since
//  uHGemHT9045.cpp is out of scope for this wave, but the shape is trivial:
//  a zero-initialized struct instance whose only purpose is to let call sites
//  read named enum constants via `SECS_EVENT.XXX` member-access syntax).
//
//  Single translation unit definition matching the `extern` declaration in
//  SecsEventType.h (mirrors this project's established extern-in-.h /
//  define-in-.cpp convention -- see cprod.h/cprod.cpp, cpublic.h/cpublic.cpp).
//---------------------------------------------------------------------------

#include "SecsEventType.h"

struct ETypeStruct SECS_EVENT;
