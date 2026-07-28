// =============================================================================
//  forms/fAGV.cpp  --  definitions for the fAGV facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM.
//
//  AI(W906-W7-F0-fix) 20260728: NO CROSS-LIBRARY BACK-EDGE ANY MORE.  The 3
//  predicates below delegate to the free functions in
//  Automation/AGV_predicates.cpp.  As W7-F0 first landed, that file was still
//  compiled into ht9045_sm, so libht9045_forms.a carried 3 undefined references
//  (AGV_IsSPIL_AMR / AGV_IsATK_AMR / AGV_Use_AMR -- `nm --undefined-only`
//  attributed all 3 to this file's object) that ht9045_forms declared no
//  dependency for: this "bottom layer" really depended on ht9045_sm.
//  AGV_predicates.cpp is now compiled into ht9045_forms itself, which is also
//  golden's own grouping -- golden defines all 3 as TfAGV MEMBER functions in
//  the fAGV form's own unit (Automation/AGV.cpp:1315 / 1327 / 1339, declared
//  Automation/AGV.h:204-206).  Nothing about the code below changed.  See the
//  "NO UNDECLARED BACK-EDGE" block in CMakeLists.txt for the full reasoning.
// =============================================================================
#include "forms/fAGV.h"
#include "Automation/AGV_predicates.h"      // AI(W5-Automation-Integrate) 20260710: real AMR-mode predicates

// --- W6.1 ------------------------------------------------------------------
// -- W5-Final-AGV_E84 INTEGRATE ADD: explicit ctor (was implicit) so mmE84Log
//    is constructed -- see forms/FormWidgets.h TfMainMemo.
TfAGV::TfAGV() { mmE84Log = new TfMainMemo(); }
TfAGV *fAGV = new TfAGV();

// AI(W5-Automation-Integrate) 20260710: wired to the real, faithfully-translated
// predicates (Automation/AGV_predicates.cpp) instead of the previous hardcoded
// false stand-ins -- see that unit's translate report.  Behaviourally identical
// offline TODAY (USE_COVER_TRAYID defaults tCIDNotUse / IniConfig.bA65_BundleIDList
// defaults false, so every predicate still evaluates false on the untouched
// baseline), but now evaluates the REAL condition once those globals are set.
bool TfAGV::IsSPIL_AMR() { return AGV_IsSPIL_AMR(); }
bool TfAGV::IsATK_AMR()  { return AGV_IsATK_AMR();  }
// --- W6.3 ADD --------------------------------------------------------------
bool TfAGV::Use_AMR()    { return AGV_Use_AMR();    }
