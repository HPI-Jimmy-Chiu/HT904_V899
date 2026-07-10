// =============================================================================
//  Automation/AGV_predicates.h  --  3 pure AMR-mode predicates extracted from
//                                    the golden TfAGV VCL form  (W5 translation)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Automation/AGV.cpp
//                    lines 1315-1348 (verified exact by grep):
//                      TfAGV::IsSPIL_AMR()  :1315-1323
//                      TfAGV::IsATK_AMR()   :1327-1335
//                      TfAGV::Use_AMR()     :1338-1346
//  Golden decl: Automation/AGV.h:204-206 (public methods of TfAGV : TForm).
//  Translation wave: W5 (Automation network unit)
//  Translator: AI(W5-AMR-Translate) 20260710
//
//  IMPORTANT: this is NOT a translation of AGV.cpp/.h as a whole. The rest of
//  AGV.cpp (~1860 lines) is the golden TfAGV VCL FORM implementing the full
//  E84 handshake state machine (bE84Loaderflag/bE84Unloaderflag timers, TTimer
//  event handlers, TEdit/TComboBox widget reads) and remains DEFERRED (VCL-
//  heavy, needs LastSet.h fields not yet translated -- see MIGRATION_ROADMAP
//  DEFERRED table). These 3 methods are the ONLY zero-VCL-coupling pure reads
//  in the whole TfAGV surface: each is a single boolean expression over
//  already-translated globals (USE_COVER_TRAYID / IniConfig.bA65_BundleIDList /
//  IniConfig.bSPILFunction / CUSTOMER_CODE), no `this->` widget access, no
//  side effects. Named distinctly (AGV_predicates, not AGV) so nobody later
//  mistakes this for the deferred full VCL form -- mirrors the established
//  CCLink/MyCCLinkSensor_predicates.h precedent (same wave, same convention).
//
//  WHY THIS MATTERS (recon finding): FormsFacade.h currently stubs
//  `fAGV->IsATK_AMR()` / `fAGV->Use_AMR()` as always-false no-ops (there is no
//  `fAGV->IsSPIL_AMR()` stub at all yet). 14 call sites in the ALREADY-
//  TRANSLATED acatchtray.cpp (7x IsATK_AMR, 7x Use_AMR) currently take the
//  false/stub branch purely because FormsFacade has no real implementation to
//  delegate to -- NOT because the golden behaviour is false in a real AMR
//  deployment. Wiring FormsFacade's TfAGV methods to call
//  AGV_IsATK_AMR()/AGV_Use_AMR() (see this unit's translate report for the
//  exact integrate-agent snippet) makes those 14 sites -- and any future
//  IsSPIL_AMR() call site (golden: csystem.cpp, uLotInfo.cpp, asendic_Loader.cpp,
//  asendic_Auto.cpp, cTrayMapping.cpp, cprod.cpp -- none translated yet) --
//  evaluate the REAL condition instead of a hardcoded false.
//
//  Free-function naming: golden names are TfAGV member methods (IsSPIL_AMR/
//  IsATK_AMR/Use_AMR); prefixed AGV_ here to stay distinct from FormsFacade's
//  TfAGV:: methods of the (almost) same name -- no ODR collision (different
//  symbols/signatures: free function vs. class member), but the AGV_ prefix
//  keeps call sites and grep results unambiguous about which is being called.
// =============================================================================
#ifndef AGV_predicatesH
#define AGV_predicatesH

// golden Automation/AGV.cpp:1315 (TfAGV::IsSPIL_AMR) -- true when the cover-
// tray-ID reader is Keyence AND the A65 bundle-ID list is enabled AND the
// SPIL customer function flag is on.
bool AGV_IsSPIL_AMR();

// golden Automation/AGV.cpp:1327 (TfAGV::IsATK_AMR) -- true when the cover-
// tray-ID reader is NFC AND the A65 bundle-ID list is enabled AND the
// customer is CC_AMKOR_Korea.
bool AGV_IsATK_AMR();

// golden Automation/AGV.cpp:1338 (TfAGV::Use_AMR) -- true whenever ANY
// cover-tray-ID reader is configured (!= tCIDNotUse) AND the A65 bundle-ID
// list is enabled -- the general "an AMR cover-tray link is active" gate
// (IsSPIL_AMR/IsATK_AMR are narrower, customer-specific subsets of this).
bool AGV_Use_AMR();

#endif // AGV_predicatesH
