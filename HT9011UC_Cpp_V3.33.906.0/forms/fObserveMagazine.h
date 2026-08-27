// =============================================================================
//  forms/fObserveMagazine.h  --  non-VCL facade for golden's TfObserveMagazine
//                                 / fObserveMagazine (magazine-tray monitor
//                                 form)
//
//  AI(W906-FW3-OWM1) 20260828: new file. Wave FW3-OWM1 (golden
//  ObserveMagazine.cpp, HT9011UC_Code_V3.33.906.0_20260618, cp950-decoded
//  with `python -c "open(p,encoding='cp950').read()"`, 0 U+FFFD).
//  Denominator (tools/census/wave_preflight.py, 20260828): 3 bodies = 3
//  TfObserveMagazine:: members + 0 file-scope, 14 span lines. The whole
//  golden unit is a ctor + FormShow + FormClose that only ever touch the
//  single field `fShow` -- nothing else in golden's own .cpp is thick.
//
//  WHY THIS FILE MATTERS BEYOND ITS OWN 14 LINES: TfObserveMagazine/
//  fObserveMagazine were BOTH completely absent from the port before this
//  wave (class name + global name both "clear" per wave_preflight's
//  shim-occupation scan, 20260828). 15 `fObserveMagazine->` call sites
//  tree-wide sit inside `#if 0`: csystem.cpp:625 (labSimMagazineTrayNo) and
//  cinitial.cpp:14347-14360 (mtMagazineTray1..14) -- ALL 15 are FIELDS, not
//  method calls, and every one is declared below with a matching name+type
//  even though this file's own 3 translated bodies never touch any of them.
//  Landing this facade resolves those 15 gates' STATED PREMISE (the object
//  now exists with the right shape); UN-GATING those 15 call sites is a
//  separate decision left to whichever wave owns csystem.cpp/cinitial.cpp.
//
//  WAVE SCOPE -- all 3 golden bodies, ACTIVE, verbatim (nothing to gate):
//    TfObserveMagazine ctor   golden :13-17  ACTIVE
//    FormShow                  golden :19-22  ACTIVE
//    FormClose                 golden :24-28  ACTIVE
//
//  DEVIATIONS (port-only adaptations, zero intended behaviour change):
//    (D-1) `__fastcall` + `TComponent* Owner` ctor glue dropped; class is
//        not TForm-derived (established convention, every translated form
//        facade in this tree).
//    (D-2) FormShow/FormClose: golden `TObject *Sender` (and FormClose's
//        `TCloseAction &Action`) dropped -- neither parameter is read in
//        either body (verified this wave), and TCloseAction has zero port
//        anywhere in this tree (established precedent: forms/fGroundMan.cpp
//        :257, forms/fHandlerSys.h:814, forms/fCounterClear.h:199).
//    (D-3) golden __published TTMyTray*/TLabel* members -> vclcompat::TrayCore*
//        / TLabel* (Part 1/Part-2-deferred split, forms/FormWidgets.h /
//        forms/fBinSel.h:675-676 precedent: golden TTMyTray -> TrayCore, NOT
//        Tray256Core -- see vclcompat/TrayCore.h's own "MAX_ITEM=100 vs 300"
//        banner for why the two are not interchangeable). `new`'d as default
//        member initializers (own-widget-only, static-init-ctor-safe, same
//        idiom as forms/fQwertyKey.h / forms/fBinSel.h).
//    (D-4) golden's 14 decorative `TLabel *Label1..Label14` (the numeric
//        captions "1".."14" next to each tray) are NOT declared -- no method
//        anywhere (golden's own 3 bodies, or any #if 0 call site tree-wide)
//        dereferences them by name (forms/fDynamicTemp.h "only what's
//        dereferenced" rule). `labSimMagazineTrayNo` IS declared: it is one
//        of the 15 predetermined GATE names (csystem.cpp:625).
//
//  DEPENDENCY AUDIT (this wave, 20260828) -- all grepped before translating:
//    vclcompat::TrayCore   REAL  vclcompat/TrayCore.h, compiled into the
//                          `vclcompat` CMake target itself (CMakeLists.txt
//                          :256, "TrayCore.cpp is zero-dependency by design")
//                          -- ht9045_forms already links vclcompat
//                          (CMakeLists.txt:859), so this is NOT a new link
//                          edge, unlike the ATC_WinWay/MonitorTCPIP cases
//                          this same wave's sibling files hit.
//    TLabel                REAL  vclcompat/Controls.h (global via
//                          `using vclcompat::TLabel;`)
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt is outside the
//  write boundary). Suggested landing: forms/fObserveMagazine.cpp -> the
//  ht9045_forms source list (CMakeLists.txt:654 block) -- diet-clean as
//  translated: zero ht9045_sm/io/motor/db/comms symbol is touched by any
//  line in this file (nothing here is even gated -- there is nothing to
//  gate).
// =============================================================================
#ifndef FORMS_FOBSERVEMAGAZINE_H
#define FORMS_FOBSERVEMAGAZINE_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject at global scope
#include "vclcompat/Controls.h"     // TLabel (global via `using vclcompat::TLabel;`)
#include "vclcompat/TrayCore.h"     // vclcompat::TrayCore -- golden TTMyTray stand-in

using vclcompat::TrayCore;

// ===========================================================================
//  TfObserveMagazine -- non-VCL facade (golden ObserveMagazine.h:14-51,
//  TfObserveMagazine : public TForm)
// ===========================================================================
class TfObserveMagazine
{
public:
    TfObserveMagazine();   // golden ctor :13-17, ACTIVE verbatim (D-1)

    // -- golden __published (h:19-32): the 14 magazine-tray slots ----------
    // GATE-REGISTER predetermined names (cinitial.cpp:14347-14360). Declared
    // in golden's own reverse (14..1) __published order; harmless, matches
    // golden's own .dfm-stream field order exactly (D-3).
    TrayCore *mtMagazineTray14 = new TrayCore();
    TrayCore *mtMagazineTray13 = new TrayCore();
    TrayCore *mtMagazineTray12 = new TrayCore();
    TrayCore *mtMagazineTray11 = new TrayCore();
    TrayCore *mtMagazineTray10 = new TrayCore();
    TrayCore *mtMagazineTray9  = new TrayCore();
    TrayCore *mtMagazineTray8  = new TrayCore();
    TrayCore *mtMagazineTray7  = new TrayCore();
    TrayCore *mtMagazineTray6  = new TrayCore();
    TrayCore *mtMagazineTray5  = new TrayCore();
    TrayCore *mtMagazineTray4  = new TrayCore();
    TrayCore *mtMagazineTray3  = new TrayCore();
    TrayCore *mtMagazineTray2  = new TrayCore();
    TrayCore *mtMagazineTray1  = new TrayCore();

    // GATE-REGISTER predetermined name (csystem.cpp:625).
    TLabel *labSimMagazineTrayNo = new TLabel();

    // -- golden public: User declarations (h:35-36) -------------------------
    bool fShow = false;   // ZeroInitVclFields-equivalent default; ctor also sets it

    void FormShow();    // golden :19-22 (D-2)
    void FormClose();   // golden :24-28 (D-2)
};

// AI(W906-FW3-OWM1) 20260828: golden ObserveMagazine.h:53 `extern PACKAGE
// TfObserveMagazine *fObserveMagazine;` -- PACKAGE glue dropped. Class name
// and global name were BOTH free tree-wide (wave_preflight, 20260828), so
// this facade declares the global -- same idiom as forms/fCleaning.cpp:56 /
// forms/fMesSystem.cpp:597 / forms/fSmartDiagnostic.cpp:37.
extern TfObserveMagazine *fObserveMagazine;

#endif // FORMS_FOBSERVEMAGAZINE_H
