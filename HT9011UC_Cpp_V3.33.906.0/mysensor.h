// =============================================================================
//  mysensor.h  --  TMySensor per-point input sensor reader
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/mysensor.h
//  Translation wave: W4-IO (IO HAL layer)
//  Translator: AI(W4-IO) 20260626
//
//  Key changes vs. BCB6 original:
//    - `_fastcall` removed (vclcompat macro neutralizes it).
//    - `AnsiString` from vclcompat/vcl_compat.h.
//    - BCB6 qualified ctor/dtor names (`TMySensor::TMySensor`) -> unqualified.
//    - `#include "MachineType.h"` kept (provides MAX_SENSOR_ITEM, MAX_TTL_BIT
//      and the ISABase enum constants).
//    - GetTrayBuildState / SetTrayBuildState / ClrTrayBuildState free-function
//      declarations retained (defined in mysensor.cpp or gated; not touched here).
//    - All fields, methods, externs, and free-function decls preserved verbatim.
// =============================================================================
#ifndef mysensorH
#define mysensorH

#include "vclcompat/vcl_compat.h"   // AnsiString, __fastcall no-op
#include "MachineType.h"            // Steven 20130809: MAX_SENSOR_ITEM, MAX_TTL_BIT

bool GetTrayBuildState(int pos);
void SetTrayBuildState(int pos);
void ClrTrayBuildState(int pos);

class TMySensor {
public:
    TMySensor();   // BCB6: _fastcall TMySensor::TMySensor()
    ~TMySensor();  // BCB6: _fastcall TMySensor::~TMySensor()

    AnsiString Name;
    AnsiString Using;
    int  Ring;
    int  IP;
    int  Port;
    int  Bit;
    int  Type;
    int  State;
    bool Status();
    bool IsOn();
    bool IsOff();
    bool Enable;
    int  ISABase;
};

extern class TMySensor Sen[MAX_SENSOR_ITEM];
extern class TMySensor SenBackUp[MAX_TTL_BIT];  // Alick 20161011 (Steven): TTL 8-site

void CopySensor(TMySensor *Source, TMySensor *Target);

#endif // mysensorH
