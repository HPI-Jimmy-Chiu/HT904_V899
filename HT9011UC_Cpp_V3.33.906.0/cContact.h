// cContact.h
// Standard C++ translation of BCB6 cContact.h (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/cContact.h (declarations, lines 607-609,
//                   612/dMinForce is actually at line 619; see note below), and
//                   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp (bodies, lines 18893-19022).
//
// AI(W64b-ForceCalc-Translate) 20260710: new file. Extract-calc-core translation of the two
// pure-math TfContact member functions used by CalculateTotalAirForce (golden :18675-18891,
// itself NOT translated here -- see scope note below).
//
// Translation scope: cContact.cpp is a 22761-line VCL form (TfContact) mixing dense switch-case
// state machines with VCL widgets (TRadioGroup, TEdit, TLabel, ...) and global config reads.  Per
// the project's established extract-calc-core convention (see ContactForce.h/.cpp -- the DIRECT
// PRECEDENT for this file, same SLK/contact-force domain), this wave translates ONLY:
//
//   1. TfContact::GetMaxIndexForceLimit()                    (golden cContact.cpp:18893-18955)
//   2. TfContact::GetMinForce(double dKitDiameter, int iTag) (golden cContact.cpp:18957-19022)
//
// as FREE FUNCTIONS that take every external read (class members, global config, and the one VCL
// widget selection GetMaxIndexForceLimit reads) as explicit parameters, and RETURN the computed
// value instead of mutating class state.  TfContact itself, the global fContactForce instance, and
// the rgKitDiameter VCL radio group do NOT exist in the translated tree; this file does not create
// them.  CalculateTotalAirForce (golden :18675-18891, VCL-widget-heavy caller) and everything else
// in cContact.cpp/.h/.dfm is OUT OF SCOPE for this wave -- see MIGRATION_ROADMAP.md's cContact
// DEFERRED entry (TfContact::DoTestContactFunction's CarlibrationTask + 6 sub-SMs, ~11347 lines).
//
// ---------------------------------------------------------------------------------------------
// DEFERRED note on dMinForce member-persistence semantics (read before wiring up a future
// full-TfContact translation):
//
//   Golden TfContact::GetMinForce mutates AND returns the class member `double dMinForce;`
//   (golden cContact.h:619) in every branch.  Grepping the whole golden 906 tree for
//   `GetMinForce\|GetMaxIndexForceLimit\|dMinForce` shows:
//     * The ONLY call site of GetMinForce anywhere in the golden tree is
//       cContact.cpp:18763 (inside CalculateTotalAirForce): `dMinForce=GetMinForce(dKitDiameter,
//       iTag);` -- i.e. it just re-assigns the SAME member from the function's return value.
//       Treating GetMinForce as a pure function (local variable in, local variable out, no member
//       side-effect) is therefore 100% behaviorally faithful for THAT call site.
//     * The member `dMinForce` is READ again later, independent of any GetMinForce call, at
//       cContact.cpp:5650 and :8414 (`ADAM_WriteVoltage(dMinForce/2.0);` inside the Z1/Z2
//       auto-get-height routines) -- both untranslated/out of scope this wave.
//     * This prompt's scoping guidance also cited a AutoClean/uCleaning.cpp:1741 dMinForce read;
//       that line/reference does NOT exist in this golden 906 snapshot (uCleaning.cpp has zero
//       occurrences of `dMinForce`) -- GOLDEN WINS, so that citation is dropped as a discrepancy
//       (documented in the wave's final report, not fixed here).
//   CONCLUSION: a future wave that translates the REAL TfContact class + CalculateTotalAirForce +
//   the Z1/Z2 auto-get-height routines will still need to wire up `dMinForce` as a persistent
//   member (assigned once per CalculateTotalAirForce call, read later by the unrelated Z1/Z2
//   routines) -- that persistence is NOT modeled by the free function below and is explicitly
//   OUT OF SCOPE / DEFERRED, not "solved" by returning a value instead of a member.
// ---------------------------------------------------------------------------------------------
//
// BCB6 source lines cited inline in cContact.cpp (this translated file).

#ifndef CCONTACT_H
#define CCONTACT_H

#include "MachineType.h"   // eIndexPressType, eTestMode constants (already translated, header-only)

// ---------------------------------------------------------------------------
// ComputeMaxIndexForceLimit
//   Portable replacement for TfContact::GetMaxIndexForceLimit().
//   BCB6 source: cContact.cpp:18893-18955
//
//   Parameters (every external read the golden body performs, as an explicit param):
//     indexPressType         -- global INDEX_PRESS_TYPE            (cmydef.h/.cpp, eIndexPressType)
//     dDutCount               -- TfContact member double dDutCount  (cContact.h:555)
//     iTestMode               -- global TestIF.iTestMode            (int; compared against eTestMode
//                                 constants SingleSite/DualSite/QualSite2X2N, MachineType.h)
//     bD27UseSingleSite85kg   -- global IniConfig.bD27UseSingleSite85kg (Config.h, bool)
//     iKitDiameterItemIndex   -- VCL rgKitDiameter->ItemIndex (int; TRadioGroup selection index)
//
//   Faithfully preserves the fixed-kg if/else-if ladder over indexPressType, and the else-branch's
//   nested condition + its own inner if/else-if (ItemIndex==1 -> 55, ItemIndex==2 -> 85) with
//   NO trailing else -- this is a REAL golden gap: any other ItemIndex value inside that inner
//   branch leaves dMaxLimit at its prior value, which is 0.0 (the local's initializer), NOT a
//   sensible default.  Do not "fix" this gap; it is preserved on purpose (see inline comment in
//   the .cpp body).
// ---------------------------------------------------------------------------
double ComputeMaxIndexForceLimit(eIndexPressType indexPressType,
                                  double dDutCount,
                                  int iTestMode,
                                  bool bD27UseSingleSite85kg,
                                  int iKitDiameterItemIndex);

// ---------------------------------------------------------------------------
// ComputeMinForce
//   Portable replacement for TfContact::GetMinForce(double dKitDiameter, int iTag).
//   BCB6 source: cContact.cpp:18957-19022
//
//   Parameters:
//     dKitDiameter        -- unchanged golden param (double)
//     dTagContactOffset    -- fContactForce->SLKClass[iTag]->dContactOffset    (already-resolved
//                             scalar; see ContactForce.h's SlkForceData::dContactOffset)
//     dTagContactOffsetNS  -- fContactForce->SLKClass[iTag]->dContactOffset_NS (see
//                             SlkForceData::dContactOffset_NS)
//     bNSKitPress          -- global TestIF_File.bNSKitPress (cprod.h, bool)
//     bD04MinForceByFile   -- global IniConfig.bD04MinForceByFile (Config.h, bool)
//     dD04MinForceByFile{,_20mm,_30mm,_40mm,_60mm,_80mm}
//                          -- global IniConfig.dD04MinForceByFile* family (Config.h, double)
//
//   Faithfully preserves every diameter bucket (20 / 30-or-28 / 40 / 60-or-58 / 80 / else) and the
//   "only override if the file-configured min is LARGER" pattern (`if(dMinForce<X) dMinForce=X`)
//   in every bucket, and the else-branch's bNSKitPress selection (dContactOffset_NS vs
//   dContactOffset) before its own bD04MinForceByFile override.
//
//   NOTE: the golden function mutates+returns the TfContact member dMinForce (golden cContact.h:619,
//   NOT line ~612 as a rough estimate might suggest).  This free function returns a LOCAL value
//   instead of mutating a member -- see the DEFERRED note above this header's include guard for why
//   that is faithful for the one real call site, and what remains deferred for other call sites.
// ---------------------------------------------------------------------------
double ComputeMinForce(double dKitDiameter,
                        double dTagContactOffset,
                        double dTagContactOffsetNS,
                        bool bNSKitPress,
                        bool bD04MinForceByFile,
                        double dD04MinForceByFile,
                        double dD04MinForceByFile_20mm,
                        double dD04MinForceByFile_30mm,
                        double dD04MinForceByFile_40mm,
                        double dD04MinForceByFile_60mm,
                        double dD04MinForceByFile_80mm);

#endif // CCONTACT_H
