// =============================================================================
//  Automation/uRENESAS_Other.cpp  --  RENESAS constant-table global instances
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Automation/uRENESAS_Other.cpp
//                    (12 lines)
//  Translation wave: W5 Automation (whole-file, trivial POD data-record unit)
//  Translator: AI(W5-RENESAS_Other-Translate) 20260710
//
//  Key changes vs. BCB6 original:
//    - `#pragma hdrstop` / `#pragma package(smart_init)` dropped (Borland-
//      specific, no standard-C++ equivalent needed -- same convention as
//      CanBus/cMyNUDN1.cpp / EtherCAT/MyNUEC1.cpp).
//    - Otherwise verbatim: just the 3 global constant-table instance
//      definitions declared `extern` in uRENESAS_Other.h.
// =============================================================================
#include "uRENESAS_Other.h"

//---------------------------------------------------------------------------
TRENESAS_CMD         RENESAS_CMD;
TRENESAS_Status_Code RENESAS_Status;
TRENESAS_Err_Index   RENESAS_Err_Index; //Kirin 20220127 TRENESAS_Err_Index
