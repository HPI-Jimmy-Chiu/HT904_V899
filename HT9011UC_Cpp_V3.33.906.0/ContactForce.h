// ContactForce.h
// Standard C++ translation of BCB6 ContactForce.h (V3.33.906.0_20260618).
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/ContactForce.h
//
// Translation scope: CALC CORE ONLY.
//   The original BCB6 file contains four VCL component classes whose constructors
//   both build UI widgets AND derive force values from the kit diameter.  This
//   translation extracts only the portable math (diameter parse + dMinForce ladder
//   + dMaxForce formula) and exposes it through two free functions and four plain
//   data structs that mirror the original names.
//
//   VCL form class TfContactForce, all widget pointers (TGroupBox, TTrackBar,
//   TEdit, TLabel, TPageControl, TScrollBox, TStringList), __fastcall event
//   handlers, ReadFile/WriteFile/ShowValue, and every global-config reference
//   are NOT translated here; they belong to a UI layer that depends on VCL.
//
// Key conversion notes (1-based AnsiString -> 0-based std::string):
//   BCB6 L183: Dia.Pos("Arm2_")==0  means NOT found (AnsiString.Pos is 1-based,
//              returns 0 when absent).  Correct std port: find()==npos.
//   BCB6 L185: Dia.SubString(6, Dia.Length())  starts at 1-based char 6, i.e.
//              skips the first 5 chars ("Arm2_" or similar prefix of len 5).
//              Correct std port: substr(5).
//
// BCB6 source lines cited inline in ContactForce.cpp.

#ifndef CONTACTFORCE_H
#define CONTACTFORCE_H

#include <string>

// ---------------------------------------------------------------------------
// SlkForceData
//   Plain data struct mirroring the calc-relevant fields of THTSLKClass,
//   THTSLKIndClass, THTDieForceSLKClass, and THTDieForceOneByOneSLKClass.
//   Non-calc fields (dLoadRate, dHotOffset, dContactOffset, bShow, iTag,
//   sDiameter) are preserved so callers that need them can populate them
//   separately via ReadFile logic ported later.
// ---------------------------------------------------------------------------
struct SlkForceData
{
    std::string sDiameter;  // original diameter token string (BCB6: AnsiString sDiameter)
    int         iTag;       // slot index                     (BCB6: int iTag)
    bool        bShow;      // visible flag                   (BCB6: bool bShow)
    double      dDiameter;  // parsed mm value                (BCB6: double dDiameter)
    double      dMinForce;  // kg  -- derived in ctor         (BCB6: double dMinForce)
    double      dMaxForce;  // kg  -- derived in ctor         (BCB6: double dMaxForce)
    double      dLoadRate;        // ini-loaded at runtime  (BCB6: double dLoadRate)
    double      dLoadRate_NS;     // NS variant              (BCB6: double dLoadRate_NS, THTSLKClass only)
    double      dHotOffset;       // ini-loaded             (BCB6: double dHotOffset)
    double      dContactOffset;   // ini-loaded             (BCB6: double dContactOffset)
    double      dContactOffset_NS;// NS variant             (BCB6: double dContactOffset_NS, THTSLKClass only)

    SlkForceData()
        : iTag(0), bShow(false), dDiameter(0.0), dMinForce(0.0), dMaxForce(0.0),
          dLoadRate(1.0), dLoadRate_NS(1.0), dHotOffset(0.0),
          dContactOffset(0.0), dContactOffset_NS(0.0) {}
};

// ---------------------------------------------------------------------------
// Variant selector passed to ComputeSlkForce.
//
//   SLKV_STANDARD     -- THTSLKClass:             "80_Hi", "40x2" specials;
//                        EP_Install==5 "Arm2_" strip; 402 in dMinForce ladder.
//   SLKV_IND          -- THTSLKIndClass:           plain atof; no 402 case.
//   SLKV_DIEFORCE     -- THTDieForceSLKClass:      identical to IND (no 402).
//   SLKV_DIEFORCE_OBO -- THTDieForceOneByOneSLKClass: identical to IND (no 402).
// ---------------------------------------------------------------------------
enum SlkVariant
{
    SLKV_STANDARD     = 0,
    SLKV_IND          = 1,
    SLKV_DIEFORCE     = 2,
    SLKV_DIEFORCE_OBO = 3
};

// ---------------------------------------------------------------------------
// ComputeSlkForce
//   Portable, self-contained replacement for the diameter + force derivation
//   that lives inside each BCB6 class constructor.
//
//   Parameters
//     dia        -- diameter token string (BCB6: AnsiString Dia ctor param)
//     tag        -- slot index            (BCB6: int Tag ctor param)
//     bDefault   -- visibility default    (BCB6: bool bDefault ctor param)
//     epInstall  -- EP_Install global     (BCB6: EP_Install macro/global, int)
//     variant    -- selects which class's rules to apply (see SlkVariant)
//
//   Returns a fully populated SlkForceData with dDiameter/dMinForce/dMaxForce
//   computed.  dLoadRate and offset fields are left at their default (0.0 / 1.0);
//   the caller populates them from ini data (ReadFile equivalent).
//
//   BCB6 source: ContactForce.cpp L171-205  (THTSLKClass)
//                ContactForce.cpp L313-327  (THTDieForceOneByOneSLKClass)
//                ContactForce.cpp L404-418  (THTDieForceSLKClass)
//                ContactForce.cpp L1545-1558 (THTSLKIndClass)
// ---------------------------------------------------------------------------
SlkForceData ComputeSlkForce(const std::string& dia,
                              int tag,
                              bool bDefault,
                              int epInstall,
                              SlkVariant variant);

// ---------------------------------------------------------------------------
// Button1 voltage interpolation helper
//   Self-contained numeric op from TfContactForce::Button1Click.
//   BCB6 source: ContactForce.cpp L1588-1598
//
//   Formula: dMaxVol = (((dMidVol - dMinVol) / 5) * 9) + dMinVol
//   All arithmetic is double; /5 is double division (no truncation).
// ---------------------------------------------------------------------------
double ComputeEpMaxVoltage(double dMinVol, double dMidVol);

#endif // CONTACTFORCE_H
