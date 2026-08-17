// =============================================================================
//  ckernel_shims.cpp  --  bodies for the two ckernel.cpp free functions that
//                         have no ported translation unit (W7-L2)
//
//  AI(W906-W7-L2) 20260803.  See ckernel_shims.h for the role of this file, the
//  exhaustive-scan evidence that both golden definitions exist in cinitial.cpp,
//  and the bool-vs-void return-type correction.
//
//  Big5: every Chinese comment below was decoded from golden with cp950 and is
//  stored here as real UTF-8.  ZERO U+FFFD.
// =============================================================================
#include "ckernel_shims.h"

#include "cmydef.h"                 // IN_SHT_LAST_SENSOR, SnInPutSHT*, MachineTypeChoice,
                                    // bDoBRTCGiveWayCheck, the MM*/MManualTray* motor ids;
                                    // pulls cprod.h (Prod / TestIF_File / SThreadPara /
                                    // ArmSpeed) and Config.h (IniConfig) transitively.
#include "MachineType.h"            // MAX_Index_Row / MAX_Index_Col, Type_HT9045*,
                                    // enum eSpeedPart { InArm, IndexArm, OutArm, TrayArm, ... }
#include "aHotPlateSubstrate.h"     // TMySucker / TMyKitSuck + InArmSuck / OutArmSuck /
                                    // FTestSuck / BTestSuck / CatchTraySuck
#include "acarry_shims.h"           // TECH_AcarryShim Tech (OutSH1/2ZOneRowDetectPos)
#include "Motor/mymotor.h"          // TTrayMotor MOT[] and its TMyTray Tray member
#include "canary_support.h"         // ShowMyMessage

// =============================================================================
//  SECTION 1 -- CROSS-MODULE SEAMS
// =============================================================================
//  Golden SetWorkParameter opens by delegating to six cinitial/cUnitConvert
//  leaf functions and later touches fMain.  NONE of them has a ported home, and
//  none is in this wave's scope.  They are stood up here with the exact idiom
//  this tree already uses for that situation -- a TU-local `static` stand-in
//  plus a `#define` of the golden name onto it, so the call sites below stay
//  character-for-character golden.  The precedent is Automation/auto9045.cpp
//  :132-140 (`static void W5FA_ChangeSite() {}` + `#define ChangeSite
//  W5FA_ChangeSite`), repeated because a `static` in another TU has no linkage
//  here; AutoClean/AutoClean.cpp:264-265 and atester.cpp's W7T1_* cluster are
//  the same pattern.
//
//  These are SEAMS, NOT TRANSLATIONS, and this file does not pretend otherwise.
//  Every golden definition site below was located by an exhaustive cp950 scan of
//  the golden tree and is cited so the wave that lands cinitial.cpp can find and
//  retire each one:
//
//    ChangeSite()          golden cinitial.cpp:12074  (decl cinitial.h:40)
//    ReadTechData()        golden cinitial.cpp:13584  (decl cinitial.h:18) -> bool
//    DoStructUnitConvert() golden cUnitConvert.cpp:243 (decl cUnitConvert.h:5)
//    SetTechDataToProd()   golden cinitial.cpp:11636  (decl cinitial.h:32)
//    DoSetupSystemToProd() golden cinitial.cpp:6650   (decl cinitial.h:36)
//    SetSimuScreenPara()   golden cinitial.cpp:5855
//    GetSHCHKPos(int,int)  golden cinitial.cpp:77     (decl cinitial.h:54)
//
//  WHY NOT TRANSLATE THEM HERE?  Because they are cinitial.cpp's body, not
//  ckernel.cpp's: between them they rewrite Prod/Tech/TestIF from the recipe
//  files, and GetSHCHKPos alone is a 179-line per-test-mode pitch table
//  (cinitial.cpp:77-255, brace-matched this pass) that further needs
//  iUnitMultiply100, CUSTOMER_CODE
//  gating and ~12 TestIF_File mode enums.  Dragging that in behind ckernel's
//  back would duplicate work the cinitial wave must do properly and would
//  create a second definition to reconcile later.  Reported to the integrator.
// -----------------------------------------------------------------------------
static void W7L2_ChangeSite()            {}   // golden cinitial.cpp:12074 -- offline no-op
static bool W7L2_ReadTechData()          { return true; }  // golden cinitial.cpp:13584
static void W7L2_DoStructUnitConvert()   {}   // golden cUnitConvert.cpp:243 -- offline no-op
static void W7L2_SetTechDataToProd()     {}   // golden cinitial.cpp:11636 -- offline no-op
static void W7L2_DoSetupSystemToProd()   {}   // golden cinitial.cpp:6650  -- offline no-op
static void W7L2_SetSimuScreenPara()     {}   // golden cinitial.cpp:5855  -- offline no-op

// W7L2_ReadTechData returns TRUE, and that is a deliberate branch selection, not
// padding: golden's ReadTechData returns `bool ret=true;` unconditionally
// (cinitial.cpp:13586, returned at :13612 -- it never assigns ret again), so
// true is golden's ONLY possible answer, seam or no seam.  SetWorkParameter
// discards it anyway (golden cinitial.cpp:13496 is a bare statement).

// GetSHCHKPos: the shuttle-site check position for site `iSite` around a centre
// base.  Offline it returns iCenterBase unchanged -- which is not an invented
// neutral value but golden's OWN answer for the single-site modes
// (cinitial.cpp:83-87: `if(TestIF_File.iTestMode==SingleSite ||
// TestIF_File.iTestMode==DualSite2x1) { Pos=iCenterBase; }`), i.e. the
// "no per-site X pitch offset" case.  Every
// other golden arm adds a multiple of the site X pitch, which offline is
// unknown because TestIF_File.dSiteXPitch is loaded by the very recipe reader
// (ReadTechData) that is itself a seam above.
// REACHABILITY, MEASURED: this seam is UNREACHABLE in the offline tree.  Its
// only caller is the rotate-shuttle block at golden cinitial.cpp:13549-13553,
// which sits behind `if(IniConfig.bHaveRotateShuttle==true && TestIF_File
// .bRotateShuttle)`.  IniConfig is a zero-initialised global (cprod.cpp:50
// `HT9045_CONFIG IniConfig;`, no ctor) so bHaveRotateShuttle is false offline
// and the whole block is closed.  The seam therefore cannot currently corrupt
// SThreadPara; it exists so the block compiles and so the block becomes live
// the moment a test or a recipe load sets that flag.
static int W7L2_GetSHCHKPos(int /*iSite*/, int iCenterBase) { return iCenterBase; }

// fMain->ShowFunctions() (golden main.cpp:8696 `int __fastcall
// TfMain::ShowFunctions()`, decl main.h:1687) repaints the main form's feature
// list.  The ported TfMain facade (forms/fMain.h) does NOT declare
// ShowFunctions -- checked; forms/fMain.h declares MainFormChange (:655) and
// ChangeLevelAttr (:216) but no ShowFunctions.  forms/fMain.* belongs to another
// agent this wave, so this file neither edits it nor includes it (including it
// would also pull the whole widget facade into this TU for one call).  Gated
// with the same macro idiom csystem.cpp:2641-2646 uses for its own six absent
// fMain members.  REPORTED AS A GAP: if the fMain owner adds `virtual void
// ShowFunctions();`, this macro becomes `do { if(fMain!=NULL) fMain->
// ShowFunctions(); } while(0)` and the seam retires.
// Offline behaviour lost: a UI repaint only -- golden's body returns an int that
// SetWorkParameter ignores (cinitial.cpp:13572 is a bare statement).
#define W7L2_FMAIN_SHOWFUNCTIONS()   do { } while(0)   // golden: if(fMain!=NULL) fMain->ShowFunctions();

// Now map the golden names onto the stand-ins.  Done AFTER every #include so no
// header declaration is affected by these macros.
#define ChangeSite            W7L2_ChangeSite
#define ReadTechData          W7L2_ReadTechData
#define DoStructUnitConvert   W7L2_DoStructUnitConvert
#define SetTechDataToProd     W7L2_SetTechDataToProd
#define DoSetupSystemToProd   W7L2_DoSetupSystemToProd
#define SetSimuScreenPara     W7L2_SetSimuScreenPara
#define GetSHCHKPos           W7L2_GetSHCHKPos

// =============================================================================
//  SECTION 2 -- TMySucker::SetRetryCount OBSERVABILITY SEAM
// =============================================================================
//  THIS IS THE MOST IMPORTANT LIMITATION IN THIS FILE.  READ IT BEFORE TRUSTING
//  SetSuckRetryCount() BELOW.
//
//  Golden's SetSuckRetryCount does exactly one kind of thing: it calls
//  `TMySucker::SetRetryCount(int)` on ~97 nozzles.  Golden's method is a
//  one-liner -- `void TMySucker::SetRetryCount(int ct)` opens at
//  MyKitSuck.cpp:2673 and its whole body is `RetryCT=ct;` at :2675 (closing
//  brace :2676) -- writing the member `int RetryCT;` (MyKitSuck.h:20).
//
//  THE PORTED TMySucker (aHotPlateSubstrate.h) HAS NEITHER.  There is no
//  RetryCT field and no SetRetryCount method: a word-boundary grep for both
//  across every .h and .cpp in this tree returns nothing -- note the boundary
//  matters, because a naive substring grep DOES hit the unrelated local
//  `iRetryCT` counters in acarry.cpp.  aHotPlateSubstrate.h is owned by
//  another agent this wave and is explicitly out of bounds for me, so I cannot
//  add them, and I will not silently drop the calls either.
//
//  WHAT THIS SEAM DOES INSTEAD.  It records, per suck grid, the retry count that
//  golden would have written and how many nozzles golden would have written it
//  to, and exposes that through W7L2_GetRetryCountSeam() so the value is
//  VERIFIABLE rather than merely lost.  This is the same "observability seam"
//  device canary_support.h:270-311 already introduces for the operator dialogs
//  (section-4 banner :270, extern declarations :303-311).
//
//  BE CLEAR ABOUT WHAT IS AND IS NOT TRUE:
//    * The loop bounds, the operand expressions (ArmSpeed[InArm].iRetryCT etc.)
//      and the traversal order below are golden's, and they really execute.
//    * The retry counts are NOT stored on the nozzles.  Anything downstream
//      that reads a nozzle's retry count would see the wrong thing -- except
//      that nothing does.  FULL RetryCT CENSUS, re-measured this pass by a
//      word-boundary cp950 scan of all 708 golden source files with comment text
//      masked and `iRetryCT` excluded: SEVEN sites, all in MyKitSuck, and that
//      is the whole of it --
//        MyKitSuck.h:20     `int RetryCT;`   the member declaration
//        MyKitSuck.cpp:1775 `RetryCT=1;`     ctor init, in _fastcall
//                                            TMySucker::TMySucker() (:1736)
//        MyKitSuck.cpp:2675 `RetryCT=ct;`    the ONLY writer, in
//                                            TMySucker::SetRetryCount (:2673)
//        MyKitSuck.cpp:2320, :2336           readers `rct=RetryCT;` in
//                                            TMySucker::Suck()    (:2161)
//        MyKitSuck.cpp:2533, :2578           readers `rct=RetryCT;` in
//                                            TMySucker::Destroy() (:2362)
//      (An earlier draft of this comment put the writer at :2673 -- that is the
//      signature line, not the assignment.  Corrected in place.)  The ported
//      TMySucker::Suck/Destroy are different, retry-free offline bodies, so
//      today the difference is unobservable through the nozzles --
//      which is why this is a seam and not a defect, and why it must be retired
//      before any real retry logic lands.
//    * RETIREMENT IS A THREE-LINE CHANGE the integrator can make the moment
//      aHotPlateSubstrate.{h,cpp} are free.  An earlier draft of this comment
//      called it two lines and listed only the field and the setter; that recipe
//      is WRONG and is corrected here in place, because followed literally it
//      installs the opposite default:
//        1. `int RetryCT;`                                 -- golden MyKitSuck.h:20
//        2. `void SetRetryCount(int ct) { RetryCT = ct; }` -- golden declares it
//           `void SetRetryCount(int);` at MyKitSuck.h:113; the body is
//           MyKitSuck.cpp:2673-2676 and its one statement `RetryCT=ct;` is :2675
//        3. `RetryCT=1;` IN THE CONSTRUCTOR -- golden MyKitSuck.cpp:1775, inside
//           `_fastcall TMySucker::TMySucker()` (golden really does spell it with
//           ONE underscore, MyKitSuck.cpp:1736).  MISS THIS AND RetryCT IS 0:
//           the ported suck grids are namespace-scope objects, so an unassigned
//           int is zero-initialised, and the tree would silently ship "no
//           retries" where golden ships "one retry" -- at exactly the moment
//           real retry logic lands and starts reading it.
//      Then replace the W7L2_SetRetryCount() calls below with golden's own
//      `.SetRetryCount(...)` syntax and delete this section.
// -----------------------------------------------------------------------------
W7L2_RetryCountSeam::W7L2_RetryCountSeam()
    : iInArmRetryCT(0),     iInArmNozzles(0),
      iOutArmRetryCT(0),    iOutArmNozzles(0),
      iFTestRetryCT(0),     iFTestNozzles(0),
      iBTestRetryCT(0),     iBTestNozzles(0),
      iCatchTrayRetryCT(0), iCatchTrayNozzles(0)
{
}

static W7L2_RetryCountSeam W7L2_Seam;

static void W7L2_SetRetryCount(TMySucker & /*Nozzle*/, int ct, int &iSeamValue, int &iSeamNozzles)
{
    // Golden's body is `RetryCT=ct;` on the nozzle.  The nozzle has no such
    // field here (see the section banner), so the value and the visit are
    // recorded against the grid instead.  `Nozzle` is still taken by reference
    // so the call site keeps naming the exact nozzle golden names, and so the
    // signature does not change when the member lands.
    iSeamValue = ct;
    iSeamNozzles++;
}

const W7L2_RetryCountSeam &W7L2_GetRetryCountSeam()
{
    return W7L2_Seam;
}

// =============================================================================
//  SECTION 3 -- SetWorkParameter
// =============================================================================
//  FAITHFUL translation of golden cinitial.cpp:13493-13582.  Statement order,
//  the if/else-if ladder, operand order and golden's own comments are preserved
//  verbatim; the Chinese comments are golden's, decoded from cp950.  The only
//  departures are the seven seams named in SECTION 1, each of which keeps
//  golden's call SYNTAX through a #define, and the fMain call which is gated by
//  a macro because the member does not exist on the ported facade.
//
//  OFFLINE BRANCH SELECTION -- every selector, with the golden line it opens.
//  These are not "safe defaults"; each one picks an arm.  Every file:line below
//  was re-opened and read this pass; the three ported cmydef.cpp citations in
//  selectors 1, 4 and 5 had been rotated among each other by an earlier draft
//  (4975/3191/3510 attached to the wrong globals) and are corrected in place:
//
//   1. bDoBRTCGiveWayCheck == false  (cmydef.cpp:3510 `bool bDoBRTCGiveWayCheck
//      =false;`)  -> OPENS golden :13499-13502, so SetTechDataToProd() and
//      DoSetupSystemToProd() both run.  Both are SECTION 1 seams, so offline the
//      arm runs and does nothing; when they land the arm is already open, which
//      is the correct default -- true is the RTC-2.0 give-way state that
//      deliberately SKIPS reloading positions mid-swap (golden's comment on
//      :13498).
//
//   2. IniConfig.bHaveRotateShuttle == false  (IniConfig is the zero-initialised
//      global at cprod.cpp:50; Config.h:109 declares the field, nothing assigns
//      it offline)  -> CLOSES the whole rotate-shuttle block, golden
//      :13505-13557.  Consequence, stated rather than hidden: offline this
//      function does NOT populate SThreadPara.iInShRotateCheck /
//      iOShRotateToLeftCheck / iOShRotateToRightCheck / bChechInShFirst at all,
//      exactly as golden does not on a machine without a rotate shuttle head.
//      It is also what makes the GetSHCHKPos seam unreachable (SECTION 1).
//
//   3. InArmSuck.iShtCol == 1  (aHotPlateSubstrate.cpp TMyKitSuck::TMyKitSuck
//      sets `iShtCol = 1;`)  -> inside the block above, would select the FIRST
//      rung, golden :13508-13512 (the 1x1 iInSHSen7DetectPos1x1 pair), and would
//      NOT reach the `ShowMyMessage("The mode is not support!!")` else at
//      :13543-13546.  Currently moot because selector 2 closes the block.
//
//   4. IN_SHT_LAST_SENSOR == 0  (cmydef.cpp:4975 `int IN_SHT_LAST_SENSOR =0;`)
//      -> != 1, so OPENS THE ELSE arm at golden :13564-13569, i.e. the
//      MachineTypeChoice ternaries, not the plain S9 assignment at :13561-13562.
//
//   5. MachineTypeChoice == Type_HT9045  (cmydef.cpp:3191 `int MachineTypeChoice
//      =Type_HT9045;`)  -> inside that else, the ternary at :13567-13568 picks
//      SnInPutSHT1S7 / SnInPutSHT2S7 (the 7-sensor in-shuttle layout), not the
//      S9 pair.  So SThreadPara.iInShuttleSen7[] IS genuinely populated offline.
//
//   6. MOT[..].Tray.XItem == 1 and YItem == 1  (mytray.cpp:19-20, TMyTray's
//      ctor)  -> the 15-motor guard at golden :13577-13579 finds no motor with
//      XItem<=0 or YItem<=0, so the early `return false` is NOT taken and this
//      function RETURNS TRUE offline.  Worth naming explicitly: golden returns
//      false there when tray geometry has never been loaded, and this tree's
//      TMyTray ctor happens to seed 1x1 rather than 0x0, so the ported default
//      is "geometry present" rather than "not initialised yet".  No CALLER is
//      affected either way: all 30 golden call sites, ckernel.cpp:371 included,
//      invoke this as a bare statement and discard the result (census in
//      ckernel_shims.h).  It is recorded here because a direct-call test IS
//      affected -- that test is the only thing that can see the return -- and
//      because the cinitial wave will have to decide whether TMyTray's ported
//      1x1 ctor seed is itself faithful.
// -----------------------------------------------------------------------------
// AI(W906-FW3-WA) 20260817: SetWorkParameter RETIRED (homecoming) -- the newer,
// gate-registered translation at cinitial.cpp:6972 is the golden home
// (golden cinitial.cpp:13494). This older stand-in body coexisted only
// because no single link ever extracted both objects; FW-3 Wave A's
// Command.cpp calls changed the extraction shape and the linker
// reported the collision (trap #1). Offline behaviour equivalent:
// this body called TU-local no-op stand-ins where cinitial's gates
// the same calls out.

// =============================================================================
//  SECTION 4 -- SetSuckRetryCount
// =============================================================================
//  FAITHFUL translation of golden cinitial.cpp:13676-13700 in every respect
//  EXCEPT the nozzle write itself, which goes through the SECTION 2 seam because
//  the ported TMySucker has no RetryCount member.  Do not read this function as
//  complete until that seam is retired.
//
//  Golden's two commented-out lines at :13694-13695 are carried over verbatim --
//  they record that the index arms deliberately do NOT take ArmSpeed[IndexArm]
//  .iRetryCT, which is exactly the kind of intent that gets lost when dead code
//  is "cleaned up" during a migration.
//
//  OFFLINE BRANCH / BOUND SELECTION:
//    * InArmSuck.iMaxRow and .iMaxCol are BOTH 0 offline.  Measured, not assumed:
//      TMyKitSuck::TMyKitSuck (aHotPlateSubstrate.cpp) assigns iShtRow, iShtCol,
//      iXStep, iYStep and iPickKitStep but never iMaxRow/iMaxCol, and these grids
//      are namespace-scope objects, so those two ints are zero-initialised.  The
//      FIRST loop therefore performs ZERO iterations offline and neither
//      InArmSuck nor OutArmSuck is visited.  That is golden's own behaviour for
//      an unloaded machine (cinitial's loader is what sets iMaxRow/iMaxCol), and
//      a test can make the loop real simply by setting them.
//    * The SECOND loop is bounded by the compile-time MAX_Index_Row (2) and
//      MAX_Index_Col (8) from MachineType.h:389-390, so it always runs 16
//      iterations and always visits FTestSuck/BTestSuck.  Note golden uses
//      MAX_Index_Col here while ScanSystemSensor's own index loops use the
//      runtime NEW_MAX_Index_Col -- that asymmetry is golden's and is preserved.
//    * CatchTraySuck.Suck[0][0] is always visited.
// -----------------------------------------------------------------------------
// AI(W906-FW3-WA) 20260817: SetSuckRetryCount RETIRED (homecoming) -- the newer,
// gate-registered translation at cinitial.cpp:(sibling, same wave) is the golden home
// (golden cinitial.cpp:see cinitial). This older stand-in body coexisted only
// because no single link ever extracted both objects; FW-3 Wave A's
// Command.cpp calls changed the extraction shape and the linker
// reported the collision (trap #1). Offline behaviour equivalent:
// this body called TU-local no-op stand-ins where cinitial's gates
// the same calls out.
