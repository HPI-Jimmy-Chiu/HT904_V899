// =============================================================================
//  forms/fHome.h  --  non-VCL stand-in for golden's fHome form pointer
//
//  AI(W906-W7-L2) 20260803: FIRST home for TfHome anywhere in this migrated
//  tree (golden uhome.h:32 `class TfHome : public TForm`).  Landed by the
//  W7-L2 substrate pass that runs ahead of the translation of golden
//  ckernel.cpp.  Follows the forms/fNote.{h,cpp} convention exactly: plain
//  class, extern global pointer with the golden declaration cited, offline
//  defaults justified individually, virtual destructor.  The facade-wide
//  contract written out in forms/fMain.h binds this file too.
//
//  THAT THIS IS THE FIRST HOME IS NOT AN ASSUMPTION -- AutoClean/AutoClean.cpp
//  :253-258 already recorded it in prose ("TfHome (uhome.h) is not yet ported
//  anywhere in this tree"), and this pass re-verified it by grep before
//  writing a line.
//
//  ---------------------------------------------------------------------------
//  NO MACRO COLLISION -- CHECKED, BECAUSE THE BRIEF ASKED FOR IT SPECIFICALLY
//  ---------------------------------------------------------------------------
//  Two places in this tree already stand in for a TfHome method.  NEITHER
//  defines a macro named `fHome`, so introducing the real global below cannot
//  collide with either, and neither file is touched by this wave:
//    * csystem.cpp:2659  `#define W7C2_FHOME_SERVOOFF(f) do{(void)(f);}while(0)`
//      -- stands in for golden fHome->GaliMotorServoOff(AnsiString) (uhome.h:77).
//    * AutoClean/AutoClean.cpp:264-265  `static void W906DIAC_InitDoTestZHome(){}`
//      + `#define W906DIAC_FHOME_INITDOTESTZHOME() W906DIAC_InitDoTestZHome()`
//      -- stands in for golden fHome->InitDoTestZHome() (uhome.h:74).
//  Both are used at their call sites AS THE MACRO, never as `fHome->...`, so
//  they keep working unchanged and keep their gates.  Deliberately, this facade
//  does NOT declare GaliMotorServoOff / InitDoTestZHome / TestZTask: adding them
//  would invite someone to "retire" those two seams without translating the
//  golden bodies (uhome.cpp:4891 and the ServoOff path), which would swap two
//  honest no-ops for two silent ones.  Retiring them belongs to the wave that
//  translates uhome.cpp.  Reported to the integrator, not done here.
//
//  ---------------------------------------------------------------------------
//  GOLDEN ckernel.cpp CENSUS FOR fHome -- MEASURED OVER ALL 2589 LINES
//  ---------------------------------------------------------------------------
//    :372  fHome->fAbort=false;      (write) -- ScanSystemSensor, THIS FRONT
//    :373  fHome->iHomeStep=1;       (write) -- ScanSystemSensor, THIS FRONT
//    :744  else if(fHome->fShow)     (read)  -- ShowRunLed,   W7-L2 DEFERRED
//    :1073 else if(fHome->fShow)     (read)  -- ShowRunLabel, W7-L2 DEFERRED
//  Exactly 4 sites, 3 members.  All three are landed; see the per-member note
//  for why fShow is landed ahead of its consumer.
// =============================================================================
#ifndef FORMS_FHOME_H
#define FORMS_FHOME_H

// No include is needed: every member below is a plain int/bool.  (fNote.h
// includes forms/FormWidgets.h only because it owns a TEdit*.)  Keeping this
// header dependency-free lets ckernel_shims.cpp -- and the ckernel.cpp that
// follows -- include it without dragging widget types into their TU.

class TfHome
{
public:
    // -----------------------------------------------------------------------
    //  [DATA] golden uhome.h:69 `int iHomeStep;` -- the Home form's own home
    //  sequence cursor, switched on by golden ProcessMotorHome (the free
    //  function opening at uhome.cpp:1180 `bool ProcessMotorHome(bool Flag2)`;
    //  e.g. :2164 =2, :2281 =3, :2335 =4, :4819 =1300).  Counted this pass
    //  rather than estimated: uhome.cpp contains 135 `iHomeStep=` assignments.
    //  OFFLINE VALUE 1, which is GOLDEN'S OWN CTOR VALUE (uhome.cpp:109
    //  `iHomeStep =1;` inside `__fastcall TfHome::TfHome(TComponent* Owner)`
    //  at uhome.cpp:106), not an invented default.
    //  WHICH BRANCH DOES IT SELECT?  Stated honestly: NONE, inside ckernel.cpp.
    //  Golden ckernel.cpp only ever WRITES this member (:373) and never reads
    //  it, so no arm of ScanSystemSensor turns on its value.  The value still
    //  is not arbitrary: 1 is simultaneously golden's ctor value AND the value
    //  golden writes at every "restart the home sequence" site (uhome.cpp:400,
    //  :711, :1222, :1696, :2530, :3189, :4151 among others), i.e. "sequence
    //  parked at step 1, nothing in flight".  Choosing 1 makes the pre-SoftStart
    //  state identical to the post-:373 state, which is what golden gets on a
    //  freshly constructed form.
    // -----------------------------------------------------------------------
    int  iHomeStep;

    // -----------------------------------------------------------------------
    //  [DATA] golden uhome.h:70 `bool fShow;` -- "the Home dialog is displayed",
    //  i.e. a homing sequence is on screen and running.
    //  OFFLINE VALUE false, GOLDEN'S OWN CTOR VALUE (uhome.cpp:110
    //  `fShow =false;`).
    //  BRANCHES THAT false SELECTS -- both verified by reading the golden lines:
    //    * ckernel.cpp:744 `else if(fHome->fShow) { RunState=LED_Homeing; }`
    //      false SKIPS the LED_Homeing tower-light state and lets ShowRunLed
    //      fall through to the FT-CT reply arm (:748), the Auto-Retest arm
    //      (:753) and the OFF_LINE alarm arm (:758).  true would have pinned
    //      the tower light to "Homing" and masked all three.
    //    * ckernel.cpp:1073 `else if(fHome->fShow) { fMain->ShowNowStatus(...,
    //      "Homing"); CoolTime=0; }` false SKIPS the "Homing" status caption
    //      and the CoolTime reset, falling through to the ATC-self-test arm at
    //      :1078.  true would have re-zeroed CoolTime on every tick.
    //  LANDED AHEAD OF ITS CONSUMER, ON PURPOSE, AND SAID SO PLAINLY: both
    //  readers live in ShowRunLed / ShowRunLabel, which the W7-L2 recon
    //  DEFERRED, so nothing in this wave reads it.  It is landed anyway for the
    //  same reason forms/fNote.h gives for its own AlarmType: it is the only
    //  other fHome member golden ckernel.cpp touches, it is a plain bool with a
    //  real golden ctor value, and forms/ files are edited one-owner-at-a-time
    //  -- omitting it guarantees a second serialised edit to this same file.
    // -----------------------------------------------------------------------
    bool fShow;

    // -----------------------------------------------------------------------
    //  [DATA] golden uhome.h:71 `bool fAbort;` -- "the operator pressed Abort
    //  Home"; golden sets it true in sbAbortHomeClick (uhome.cpp:4983
    //  `fAbort=true;`) and clears it at every home (re)start.
    //  OFFLINE VALUE false, GOLDEN'S OWN CTOR VALUE (uhome.cpp:111
    //  `fAbort =false;`), and also the value golden's own reset path writes
    //  (uhome.cpp:4848).
    //  WHICH BRANCH DOES IT SELECT?  Again honestly: none in ckernel.cpp --
    //  golden ckernel.cpp:372 only WRITES it.  Its readers are all inside
    //  uhome.cpp's home loop, which has no ported home.  false is still the
    //  only defensible value: it means "no abort pending", which is exactly the
    //  state ScanSystemSensor itself establishes at :372 before starting.
    // -----------------------------------------------------------------------
    bool fAbort;

    TfHome();
    virtual ~TfHome() {}
};

// golden: extern PACKAGE TfHome *fHome;   (uhome.h:85)
extern TfHome *fHome;

#endif // FORMS_FHOME_H
