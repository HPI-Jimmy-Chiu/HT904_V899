// ContactForce.cpp
// Standard C++ translation of BCB6 ContactForce.cpp (V3.33.906.0_20260618).
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/ContactForce.cpp
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.
//
// Translation scope: see ContactForce.h for full rationale.
// Only the pure math extracted from the four class constructors is translated.
// All VCL widget construction, global-config reads, ini I/O, event handlers,
// and cross-form calls are excluded.
//
// 1-based -> 0-based string index corrections (the #1 silent-bug risk):
//
//   BCB6 L183: Dia.Pos("Arm2_") == 0
//     AnsiString.Pos() returns 0 when NOT found (1-based; first char = 1).
//     std::string.find() returns string::npos when not found, and 0 when found
//     at position 0.  A mechanical map "== 0" would mean "found at start",
//     which is the OPPOSITE of the BCB6 intent.
//     Correct port: dia.find("Arm2_") == std::string::npos
//
//   BCB6 L185: Dia.SubString(6, Dia.Length())
//     1-based start = 6, i.e. skip chars 1-5 (5 chars = "Arm2_" or "Arm1_").
//     std::string::substr is 0-based: skip 5 chars -> substr(5).
//
//   BCB6 L441: edtCurrentType->Text.Pos("30") == 1 / Pos("60") == 1
//     1-based: == 1 means "starts with".
//     std port: text.find("30") == 0  (not translated here; UI-only).

#include "ContactForce.h"
#include <cstdlib>   // atof
#include <cstring>   // (not used; included for completeness)

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

// ParseDiameter_Standard
//   Diameter parsing for THTSLKClass (SLKV_STANDARD).
//   BCB6 source: ContactForce.cpp L171-188
//
//   Special tokens:
//     "80_Hi"  -> 80.0  (BCB6 L172-174)
//     "40x2"   -> 402.0 (BCB6 L175-178; comment: "40 dual socket")
//   Normal path:
//     When epInstall == 5 AND the string does NOT contain "Arm2_"
//     (BCB6 L181-187: Dia.Pos("Arm2_")==0 means NOT found in 1-based),
//     strip the first 5 characters (e.g. "Arm1_" prefix).
//     Then parse with atof.
//
//   BCB6 index conversion:
//     L183: Dia.Pos("Arm2_")==0  -> dia.find("Arm2_")==npos  (NOT found)
//     L185: Dia.SubString(6, Dia.Length()) -> dia.substr(5)   (skip first 5)
static double ParseDiameter_Standard(const std::string& dia, int epInstall)
{
    // BCB6 L171-174: "80_Hi" special case
    if (dia == "80_Hi")
        return 80.0;

    // BCB6 L175-178: "40x2" -> 402 (40mm dual socket sentinel)
    if (dia == "40x2")
        return 402.0;

    // BCB6 L180-188: EP_Install==5 "Arm2_" strip logic
    std::string work = dia;
    if (epInstall == 5)
    {
        // BCB6 L183: Dia.Pos("Arm2_")==0  -> AnsiString: returns 0 when NOT found
        // std port : dia.find("Arm2_")==npos means NOT found
        if (work.find("Arm2_") == std::string::npos)
        {
            // BCB6 L185: Dia=Dia.SubString(6, Dia.Length())
            // 1-based start=6 skips chars 1-5 (the "Arm1_" prefix, 5 chars).
            // std port: substr(5) -- 0-based offset 5.
            if (work.size() > 5)
                work = work.substr(5);
        }
    }

    // BCB6 L188: dDiameter=atof(Dia.c_str())
    return atof(work.c_str());
}

// ParseDiameter_Simple
//   Plain atof parse used by THTSLKIndClass, THTDieForceSLKClass,
//   and THTDieForceOneByOneSLKClass.
//   BCB6 source: L313, L404, L1545
static double ParseDiameter_Simple(const std::string& dia)
{
    return atof(dia.c_str());
}

// ComputeMinForce_Standard
//   dMinForce ladder for THTSLKClass -- includes the 402 special case.
//   BCB6 source: ContactForce.cpp L191-202
static double ComputeMinForce_Standard(double d)
{
    if (d <= 30.0) return 0.5;      // BCB6 L191-192
    if (d <  40.0) return 1.0;      // BCB6 L193-194
    if (d <  50.0) return 2.0;      // BCB6 L195-196
    if (d <  60.0) return 4.0;      // BCB6 L197-198
    if (d == 402.0) return 4.0;     // BCB6 L199-200: 40x2 sentinel (40mm dual socket)
    return 8.0;                     // BCB6 L201-202
}

// ComputeMinForce_NoSpecial
//   dMinForce ladder for THTSLKIndClass / THTDieForceSLKClass /
//   THTDieForceOneByOneSLKClass -- NO 402 special case.
//   BCB6 source: L315-324 (THTDieForceOneByOneSLKClass)
//                L406-415 (THTDieForceSLKClass)
//                L1547-1556 (THTSLKIndClass)
static double ComputeMinForce_NoSpecial(double d)
{
    if (d <= 30.0) return 0.5;      // BCB6 L315/406/1547
    if (d <  40.0) return 1.0;      // BCB6 L316/407/1548 -- note: <40 (strictly less)
    if (d <  50.0) return 2.0;      // BCB6 L317/408/1549
    if (d <  60.0) return 4.0;      // BCB6 L318/409/1550
    return 8.0;                     // BCB6 L319/410/1551
}

// ComputeMaxForce
//   Shared dMaxForce formula for all four classes.
//   BCB6 source: L204, L326, L417, L1558
//   Formula: 3.14 * (dDiameter / 100.0) * (dDiameter / 100.0) * 500
//   (all double arithmetic; the constant 3.14 is used explicitly -- NOT M_PI)
static double ComputeMaxForce(double d)
{
    double r = d / 100.0;
    return 3.14 * r * r * 500.0;
}

// ---------------------------------------------------------------------------
// ComputeSlkForce
//   Public API -- see ContactForce.h for parameter docs.
// ---------------------------------------------------------------------------
SlkForceData ComputeSlkForce(const std::string& dia,
                              int tag,
                              bool bDefault,
                              int epInstall,
                              SlkVariant variant)
{
    SlkForceData result;
    result.sDiameter = dia;
    result.iTag      = tag;
    result.bShow     = bDefault;

    switch (variant)
    {
        // ------------------------------------------------------------------
        // SLKV_STANDARD: THTSLKClass constructor calc section
        // BCB6 source: ContactForce.cpp L171-204
        // ------------------------------------------------------------------
        case SLKV_STANDARD:
        {
            result.dDiameter = ParseDiameter_Standard(dia, epInstall);
            result.dMinForce = ComputeMinForce_Standard(result.dDiameter);
            result.dMaxForce = ComputeMaxForce(result.dDiameter);
            break;
        }

        // ------------------------------------------------------------------
        // SLKV_IND: THTSLKIndClass constructor calc section
        // BCB6 source: ContactForce.cpp L1545-1558
        // ------------------------------------------------------------------
        case SLKV_IND:
        {
            result.dDiameter = ParseDiameter_Simple(dia);
            result.dMinForce = ComputeMinForce_NoSpecial(result.dDiameter);
            result.dMaxForce = ComputeMaxForce(result.dDiameter);
            break;
        }

        // ------------------------------------------------------------------
        // SLKV_DIEFORCE: THTDieForceSLKClass constructor calc section
        // BCB6 source: ContactForce.cpp L404-418
        // Ifor 20191003: add Die Force custom Kit diameter
        // Derivation identical to SLKV_IND (no 402 case).
        // ------------------------------------------------------------------
        case SLKV_DIEFORCE:
        {
            result.dDiameter = ParseDiameter_Simple(dia);
            result.dMinForce = ComputeMinForce_NoSpecial(result.dDiameter);
            result.dMaxForce = ComputeMaxForce(result.dDiameter);
            break;
        }

        // ------------------------------------------------------------------
        // SLKV_DIEFORCE_OBO: THTDieForceOneByOneSLKClass constructor calc
        // BCB6 source: ContactForce.cpp L313-327
        // Eastsun 20260525: INSTALL_DOUBLE_EP_3 addition
        // int iCount = (Tag%8)+1 -- only affects caption strings (not calc);
        // the calc section is identical to SLKV_DIEFORCE.
        // ------------------------------------------------------------------
        case SLKV_DIEFORCE_OBO:
        {
            // BCB6 L215: int iCount=(Tag%8)+1
            // iCount affects group-box captions only; no effect on dDiameter,
            // dMinForce, or dMaxForce.  Preserved here as a comment for
            // reverse-lookup clarity.
            // (int iCount = (tag % 8) + 1;  -- caption only, not used in calc)
            result.dDiameter = ParseDiameter_Simple(dia);
            result.dMinForce = ComputeMinForce_NoSpecial(result.dDiameter);
            result.dMaxForce = ComputeMaxForce(result.dDiameter);
            break;
        }

        default:
            // Unknown variant; leave fields at default (0.0).
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
// ComputeEpMaxVoltage
//   Self-contained EP voltage interpolation.
//   BCB6 source: ContactForce.cpp L1588-1598 (TfContactForce::Button1Click)
//
//   Formula (exact):
//     dMaxVol = (((dMidVol - dMinVol) / 5) * 9) + dMinVol
//
//   All arithmetic is double.  /5 is double division (no integer truncation).
//   The BCB6 operands are all double (dMinVol, dMidVol are double locals,
//   initialized from .ToDouble() at L1592-1594).
// ---------------------------------------------------------------------------
double ComputeEpMaxVoltage(double dMinVol, double dMidVol)
{
    // BCB6 L1596: dMaxVol=(((dMidVol - dMinVol)/5)*9)+dMinVol;
    return (((dMidVol - dMinVol) / 5.0) * 9.0) + dMinVol;
}
