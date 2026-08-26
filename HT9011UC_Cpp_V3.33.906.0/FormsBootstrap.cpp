//---------------------------------------------------------------------------
//  FormsBootstrap.cpp -- explicit construction of the translated form globals
//  golden builds inside HT9045.cpp's WinMain.
//
//  AI(W906-FW-BOOTSTRAP-W35) 20260827: NEW FILE.  BEHAVIOUR-CHANGE WAVE --
//  the four constructors reached from here have never executed in this port
//  tree before.
//
//  GOLDEN SOURCE
//    D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\HT9045.cpp -- 118
//    `Application->CreateForm(__classid(C), &g);` calls over 115 distinct
//    globals.  This file translates the SEMANTICS of that segment for the
//    four globals that (a) have a translated class, (b) have a global
//    definition, and (c) are constructed by nobody.  Measured with
//    tools/wavescan/bootstrap_survey.py, re-run 20260827 01:15 (2381 port
//    files, 121 s): 41 already built / 9 buildable candidates / 5 class-but-
//    no-global / 60 class-not-translated.
//
//    Golden's line for each row, in golden's own order:
//      golden HT9045.cpp:210  CreateForm(__classid(TFormBarcodeReader), &FormBarcodeReader);
//      golden HT9045.cpp:221  CreateForm(__classid(TfDynamicTemp),      &fDynamicTemp);
//      golden HT9045.cpp:223  CreateForm(__classid(TfPassword),         &fPassword);
//      golden HT9045.cpp:274  CreateForm(__classid(TfVacuumUnit),       &fVacuumUnit);
//
//  WHAT IS NOT TRANSLATED HERE, AND WHY
//   (N-1) WinMain itself.  golden HT9045.cpp:153 is a real
//         `MessageBox(0, "Please copy HT9045.EXE to D:\\HT9045\\EXE",
//         "EXE Path Error", MB_OK);` -- a MODAL dialog raised whenever the
//         exe is not at that path.  A modal dialog inside a batch/ctest run
//         stalls the run and logs nothing (docs/KNOWLEDGE.md, "windows-batch-
//         runs-must-not-popup-modal-dialogs": an MSVC Debug CRT assert box
//         once froze ctest at 94/95 with an empty log).  Only the CreateForm
//         segment's semantics are ported -- WinMain's path check, the
//         Application object and its lifetime, Initialize()/Run() and the
//         surrounding try/catch are all deliberately absent.
//   (N-2) fLan / TfLan (golden :171).  EXCLUDED THIS WAVE -- language.cpp is
//         not a member of ANY CMake target: CMakeLists.txt has zero
//         case-insensitive "language" source rows, there is no file(GLOB)
//         anywhere in it, and no .cpp #includes language.cpp.  Confirmed by
//         nm on 20260827: 0 defined `TfLan` symbols across all 18
//         build/*.a archives, and no language.cpp.obj under build/CMakeFiles.
//         Constructing it here would be an undefined reference at link time
//         for every executable that pulls this TU in.  Compiling language.cpp
//         is its own wave (967 lines carrying three large #if 0 regions and
//         an unexamined link surface), not a side effect of this one.
//   (N-3) HGem / THGem.  EXCLUDED BY INSTRUCTION -- the SECS/GEM host comms
//         object.  Moving it from NULL to non-NULL inverts every `if(HGem)`
//         guard tree-wide and it owns a dedicated test
//         (tests/test_uHGemEquipment.cpp).  Safety-critical; needs the user.
//   (N-4) The 60 globals whose class is not translated yet, and the 5 whose
//         class exists but whose global has no definition.  No stubs are
//         created for any of them.  A stub would satisfy the linker while the
//         real class stayed missing -- form #2 of the archive-extraction trap
//         in docs/KNOWLEDGE.md ("V906 build 綠 證明不了接上了"), which
//         `nm --undefined-only` cannot see.
//   (N-5) Event handlers.  Nothing here wires an OnClick/OnShow/OnTimer.  The
//         browser-HMI write path is the safety-critical work item; a
//         construction-only wave does not touch it.
//   (N-6) fPassword2 (forms/fPassword.cpp:30).  Not a golden CreateForm
//         target at all (checked golden HT9045.cpp for `&fPassword2)`,
//         20260827: absent), so it is correctly out of this table's scope and
//         stays NULL.
//
//  WHY AN EXPLICIT FUNCTION AND NOT A STATIC INITIALIZER
//    The order of dynamic initialization across translation units is
//    unspecified, so a file-scope constructor here could run before the
//    globals it depends on exist.  This tree has already paid that bill:
//    fLaserSensor's ctor called InitLaserEdtList() while elLaser was still
//    NULL and 88 of 134 ctest binaries SEGFAULTed before main()
//    (docs/KNOWLEDGE.md, "V906 homecoming SIOF lottery" -- and note that one
//    green executable proves nothing there, because the draw is per-binary).
//    InitForms() therefore runs only when a caller calls it, after static
//    initialization is complete, in an order the caller controls.
//
//  DOUBLE CONSTRUCTION: A DELIBERATE DEVIATION FROM GOLDEN
//    golden constructs three globals TWICE -- fBinAOISel (:278 and :279),
//    fObserveMagazine (:280 and :282), frmFileTransfer (:281 and :283).  In
//    golden that is benign: `Application` OWNS both instances, so the first
//    one is merely an orphaned form and is still released at shutdown.  This
//    port has no Application and no owner chain, so translating that shape
//    literally would be a real, permanent leak.  None of those three globals
//    is in this wave's set, but this file is a GENERAL bootstrap table, so the
//    rule is recorded once here and binds every future row:
//    BUILD EACH GLOBAL EXACTLY ONCE.  The `== 0` guards below also make
//    InitForms() idempotent, so a second call constructs nothing rather than
//    leaking the first set.
//
//  OWNERSHIP AND TEARDOWN
//    No teardown counterpart is provided, matching golden: golden never
//    destroys these either -- Application frees them at process exit.  Here
//    they live until the process ends and the OS reclaims the memory.  (A
//    ShutdownForms() would in particular have to run ~TfLan() ->
//    FormDestroy(0) -> LogSoftwareOffTime(), i.e. shutdown-time I/O; out of
//    scope, and unreachable anyway while (N-2) holds.)
//
//  MEASURED BEHAVIOUR DELTA (20260827, #if 0 honoured throughout)
//    Consumers of all four globals were enumerated across every .cpp/.h in
//    the tree (the generated tools/dfm2rc layout tables excluded) and each hit
//    was classified live or dead against its own file's preprocessor nesting.
//    A textual grep hit is NOT evidence that the compiler sees the line.
//
//    * fVacuumUnit -- 24 dereference sites, ALL of them inside #if 0
//      (VacuumUnit.cpp regions 632..683 and 859..903).  Zero live consumers,
//      which matches VacuumUnit.h:14's own "ZERO CONSUMERS" note.
//      Constructing it is unobservable.
//
//    * FormBarcodeReader and fPassword -- every live consumer dereferences the
//      pointer UNCONDITIONALLY; there is no NULL guard anywhere.  Live sites:
//      BarcodeReader.cpp:156-165 and :224; cObserver.cpp:6262-6264;
//      cSetUp.cpp:1172-1174; forms/fLotInfo.cpp:4902-4910 and :5086-5091;
//      forms/fPassword.cpp:55; SECSGEM/uHGemHT9045.cpp:6427 and :6429.
//      Each of those is a NULL dereference today if it is ever reached, so
//      there is no defined behaviour to preserve -- constructing strictly
//      removes latent crashes rather than changing any working path.
//      (csystem.cpp:2728 and :2953 also read fPassword->bShow, but both sit
//      inside the 2390-line #if 0 that gates the golden-verbatim MainProc, so
//      the live slim MainProc never touches fPassword.  Likewise
//      forms/fLotInfo.cpp:3176-3177 reads FormBarcodeReader->pmBarcode inside
//      #if 0 3175..3178 -- consistent with BarcodeReader.h's (D-3), which
//      omits the menu trio.)
//
//    * fDynamicTemp -- THE ONE TRUE FLIP.  uTemp_Set.cpp:2270 is
//      `if(fDynamicTemp!=NULL)`, a guard that is false today and becomes true
//      once InitForms() has run; :2272-2275 then write
//      edUpper/edLower/edMax/edMin ->Text.  All four are NSDMI-allocated
//      TEdits (forms/fDynamicTemp.h:600-603), so the writes are memory-safe;
//      they change in-memory widget text that no live reader consumes.
//
//    NOTHING CALLS InitForms() YET (grepped 20260827).  Until a call site
//    lands, this wave changes no runtime behaviour whatsoever -- the delta
//    above materializes only when a caller is added, which is the moment to
//    re-read this block.
//
//  PER-CTOR AUDIT (every body re-read in full this wave, none taken on trust)
//    TFormBarcodeReader -- BarcodeReader.cpp:50-55.  Three own-field stores
//      (iKeyType/sBarcodeInfo/bShow); no base class; no member dereference.
//      Its by-value TQPF_Timer member's ctor (myTimer.cpp:14-17) runs 1001
//      QueryPerformanceCounter calls to calibrate overhead -- syscall only, no
//      globals, no I/O.
//    TfDynamicTemp -- DynamicTemp.cpp:87-106.  18 lines filling THREE arrays:
//      TC[18] (colours), SetEdit[18] and myCB[18].  Every widget it names is
//      NSDMI `new` in forms/fDynamicTemp.h (edtRealTime1..16 at :610-617,
//      edUpper/edLower at :602-603, CheckBox1..18 at :621-629), so all of them
//      exist before the ctor body runs.  The NSDMI set also builds a
//      TfDynamicTempChart (h:556-564: one AxisType plus 18 series, heap only).
//      No global is touched.
//    TfPassword -- forms/fPassword.cpp:36-40.  Two lines; dereferences
//      PageControl1 (h:311) and cbUserName (h:319) and reads tsPassword
//      (h:312) -- all three NSDMI `new`, constructed before the ctor body.
//      golden's own ctor BODY was split out into TfPassword::Init() (h:308),
//      which calls ReadPasswordFile().  Init() is NOT called from here: this
//      wave constructs, it does not initialize, so no file I/O is introduced.
//      Whoever later wants golden's full CreateForm equivalence for this form
//      must call Init() explicitly and account for that read.
//    TfVacuumUnit -- VacuumUnit/VacuumUnit.cpp:141-148.  `iCount=5;` only.
//      The base vclcompat::TComponent ctor is `: Owner_(AOwner) {}`
//      (vclcompat/Comm.h:86) -- a plain store, NULL-safe.  Its sibling
//      Initial(), which is what actually builds the TMyVacuumPanels, is NOT
//      called.  CAUTION for whoever opens this next: this class's widget
//      members (VacuumUnit.h:130-187) and its myPal*/d* arrays (h:208-217)
//      have NO initializers, so after construction they hold indeterminate
//      values.  That is harmless only because every site that would read them
//      is inside the two #if 0 regions named above; un-gating any of those
//      sites REQUIRES calling Initial() first.
//
//  OWNER ARGUMENT
//    golden passes `Application` as the VCL Owner.  There is no Application
//    here, so NULL is passed.  vclcompat::TComponent merely stores it and
//    exposes it through Owner(), and no translated code reads
//    fVacuumUnit->Owner() (grepped 20260827).  VacuumUnit.h:145's "Owner is
//    load-bearing" note refers to `this` being handed to each TMyVacuumPanel
//    inside Initial(), not to the stored Owner_.
//
//  CMAKE HOME
//    This TU belongs in ht9045_sm.  Three of the four classes already live
//    there (BarcodeReader.cpp, DynamicTemp.cpp, VacuumUnit/VacuumUnit.cpp)
//    and the fourth, forms/fPassword.cpp, is in ht9045_forms -- which
//    ht9045_sm already links (CMakeLists.txt:2284-2295).  The reverse edge is
//    not available: ht9045_forms is a bottom layer and CMakeLists.txt:589-591
//    records that `target_link_libraries(ht9045_forms PUBLIC ht9045_sm)`
//    fails configure with a target cycle.  Registering this file anywhere
//    other than ht9045_sm therefore either breaks the configure or leaves
//    TfPassword unresolvable.
//---------------------------------------------------------------------------
#include "FormsBootstrap.h"

#include "BarcodeReader.h"            // TFormBarcodeReader + extern FormBarcodeReader (h:113)
#include "forms/fDynamicTemp.h"       // TfDynamicTemp
#include "forms/fPassword.h"          // TfPassword + extern fPassword (h:411)
#include "VacuumUnit/VacuumUnit.h"    // TfVacuumUnit + extern fVacuumUnit (h:233)

// forms/fDynamicTemp.h:698-703 deliberately does NOT declare this extern --
// that header's rule is "widgets go in the header, file-scope globals stay in
// the .cpp that owns them", and the definition is DynamicTemp.cpp:58.  So it
// is redeclared here exactly as its other consumer does (uTemp_Set.cpp:212).
extern TfDynamicTemp *fDynamicTemp;

//---------------------------------------------------------------------------
void InitForms()
{
    // golden's own order (HT9045.cpp :210 / :221 / :223 / :274) preserved.
    // Each guard keeps the call idempotent -- see DOUBLE CONSTRUCTION above.
    if (FormBarcodeReader == 0) FormBarcodeReader = new TFormBarcodeReader();  // golden :210
    if (fDynamicTemp      == 0) fDynamicTemp      = new TfDynamicTemp();       // golden :221
    if (fPassword         == 0) fPassword         = new TfPassword();          // golden :223
    if (fVacuumUnit       == 0) fVacuumUnit       = new TfVacuumUnit(0);       // golden :274 -- Owner: Application -> NULL, see OWNER ARGUMENT
}
//---------------------------------------------------------------------------
