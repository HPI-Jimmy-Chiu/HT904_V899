// =============================================================================
//  FormsFacade.h  --  UMBRELLA ONLY.  The facade itself now lives in forms/.
//
//  AI(W906-W7-F0) 20260728: W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  This file used to be a 571-line
//  monolith holding all 9 form stand-ins plus ~18 widget stand-ins.  It is now
//  a pure include list, kept at this exact path and name so that every one of
//  the 113 existing `#include "FormsFacade.h"` lines (83 sources + 30 tests,
//  counted this wave) compiles UNCHANGED --
//  zero call-site edits were needed anywhere in the tree.
//
//  WHY THE SPLIT HAPPENED -- two independent payoffs, both measured:
//
//  (1) IT BREAKS A REAL CMake DEPENDENCY CYCLE ("Wall 2").  FormsFacade.cpp
//      used to be compiled into ht9045_sm.  ht9045_secsgem does NOT link
//      ht9045_sm, while ht9045_sm DOES link ht9045_secsgem -- so
//      SECSGEM/uHGemHT9045.cpp could never reach fMain, no matter how many
//      facade members were added.  MIGRATION_ROADMAP.md:133 blamed "~19 missing
//      FormsFacade stubs"; the member count was roughly right but the stubs were
//      never the blocker (plan SS4-V1 / SS3-C7).  The facade now compiles into a
//      NEW bottom-layer library, ht9045_forms, which depends only on vclcompat
//      (+ht9045_globals), so ht9045_secsgem can link it with no cycle.
//
//  (2) IT DISSOLVES A TREE-WIDE WRITE LOCK.  Because every wave that needed one
//      new member had to edit THIS ONE FILE, parallel waves collided on it --
//      Automation/SCK_ART.h:34-45 records a wave that invented a whole separate
//      `SckArtState` rather than extend TfSCKART for exactly this reason, which
//      is part of why the SckArt state now exists in FIVE places -- AI(W906-W7-F2fix)
//      20260729: was "FOUR places (plan SS3-C4)"; the verified count is five and the
//      enumeration lives in forms/fSCKART.h's RECONCILIATION DEBT banner (plan SS3-C4's
//      four omit forms/fSCKART.h's own TfSCKART; the old fSCKART.h four collapsed
//      csystem.cpp's two independent seam structs into one bullet).
//      After the split a wave edits only its own forms/fXxx.h.
//
//  WHERE THINGS WENT
//  -----------------
//    forms/FormWidgets.h  -- all ~18 widget stand-ins.  The stock ones are now
//                            typedefs onto the single unified set in
//                            vclcompat/Controls.h; the deferred ones (custom
//                            controls + the memo family) are documented there
//                            with the reason each was not aliased.
//    forms/fMain.{h,cpp}       forms/fLotInfo.{h,cpp}   forms/fSCKART.{h,cpp}
//    forms/fSortCT.{h,cpp}     forms/fAGV.{h,cpp}       forms/fNote.{h,cpp}
//    forms/fOffSet.{h,cpp}     forms/fCleaning.{h,cpp}  forms/fShowMessage.{h,cpp}
//
//  READ forms/fMain.h FIRST: it carries the binding facade contract (methods
//  are virtual and their offline no-op bodies are permanent; data members are
//  concrete storage and must never become accessors).
//
//  FormsFacade.cpp is GONE -- its bodies moved into the matching forms/*.cpp.
//  Do not recreate it.
// =============================================================================
#ifndef FormsFacadeH
#define FormsFacadeH

#include "forms/FormWidgets.h"   // widget stand-ins (also re-exports TStringGrid)
#include "forms/fMain.h"
#include "forms/fLotInfo.h"
#include "forms/fSCKART.h"
#include "forms/fSortCT.h"
#include "forms/fAGV.h"
#include "forms/fNote.h"
#include "forms/fOffSet.h"
#include "forms/fCleaning.h"
#include "forms/fShowMessage.h"
// AI(W906-W7-L1-Wave0) 20260801: 4 new form stand-ins landed by the W7-L1 Wave-0
// serialized facade pass, for the asendic_* tray SM family.  See each header's
// own file-head for its golden provenance.  Their prior status differed and is
// stated exactly rather than lumped together as "all new":
//   * fTrayForm, fOCR      -- genuinely FIRST homes; grepped the whole ported tree
//     and found no class and no global for either (fTrayForm's only prior tokens
//     were inside cprod.cpp's `#if 0 // TODO(W6)` block :184-4036 and in generated
//     dfm2rc layout string data; fOCR's only prior token was a csystem.cpp
//     comment).
//   * fProductionInfo      -- first DEFINITION of the global, but not the first
//     mention: two out-arm variants carried their own differently-typed externs
//     for a symbol nothing defined.  Both were retired in this same pass.
//   * fFixAICCD            -- RETIRES the `TfFixAICCD *fFixAICCD = 0;` null global
//     and its TU-local class from aoutarm_shims.cpp, plus three more TU-local
//     copies in the aoutarm9045_* variants.  The global is now a real object.
#include "forms/fTrayForm.h"
#include "forms/fOCR.h"
#include "forms/fProductionInfo.h"
#include "forms/fFixAICCD.h"

#endif // FormsFacadeH
