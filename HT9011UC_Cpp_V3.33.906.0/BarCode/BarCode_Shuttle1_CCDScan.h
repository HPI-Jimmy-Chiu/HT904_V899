// =============================================================================
//  BarCode/BarCode_Shuttle1_CCDScan.h  --  In-Shuttle-1 2D/CCD barcode scan SM
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh1.cpp
//                    lines 82-2453:  TfBarCode::DoBarcodeCCDInShuttle_1(bool bVerify=false)
//  Translation wave: W5-BarCode (2026-07-11), unit "DoBarcodeCCDInShuttle_1"
//  Translator: AI(W5-BarCode-Shuttle1CCDScan-Translate) 20260711
//
//  This is the REAL BODY for `TfBarCode_Shim::DoBarcodeCCDInShuttle_1` (golden
//  BarCode.h:807; shim declared aHotPlateSubstrate.h:644).  It is ONE of 17
//  TfBarCode_Shim methods being given real bodies this wave by parallel
//  translate units; this unit owns ONLY this one function (the largest single
//  unit in the batch, ~2372 golden lines).
//
//  See the .cpp file banner for the full scope / gating map (what is faithful
//  vs. conservatively gated) and the golden line citations for every gate.
// =============================================================================
#ifndef BarCode_Shuttle1_CCDScanH
#define BarCode_Shuttle1_CCDScanH

#include "vclcompat/vcl_compat.h"   // AnsiString

// golden BarCode_Sh1.cpp:83 -- TfBarCode::DoBarcodeCCDInShuttle_1(bool bVerify=false)
// Free-function stand-in; TfBarCode_Shim::DoBarcodeCCDInShuttle_1 should
// delegate here (see this unit's translate report for the exact integrate
// snippet).  Default arg matches golden verbatim.
bool BarCode_DoBarcodeCCDInShuttle_1(bool bVerify=false);

#endif // BarCode_Shuttle1_CCDScanH
