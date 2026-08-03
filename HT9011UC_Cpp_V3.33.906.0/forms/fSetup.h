// =============================================================================
//  forms/fSetup.h  --  non-VCL stand-in for golden's fSetup form pointer
//
//  AI(W906-W7-L2) 20260803: FIRST home for TfSetup anywhere in this migrated
//  tree (golden csetup.h:14 `class TfSetup : public TForm`).  Landed by the
//  W7-L2 substrate pass ahead of the translation of golden ckernel.cpp.
//  forms/fNote.{h,cpp} convention; the facade contract in forms/fMain.h binds
//  this file too.
//
//  fSetup has been NAMED in this tree before without ever being landed --
//  forms/fMain.h:366 and :372 list it among the forms the SECS/GEM AddEC and
//  S125F4 paths reach, and tests/test_w7_f1_wall2_probe.cpp:120/:154/:157/:165/
//  :170/:191 counts it repeatedly in its "forms with no home" tallies.  This is
//  the first actual class + global.
//
//  ---------------------------------------------------------------------------
//  GOLDEN ckernel.cpp CENSUS FOR fSetup -- MEASURED OVER ALL 2589 LINES
//  ---------------------------------------------------------------------------
//    :411  if(iHome==0 && fSetup->fShow==false)   (read) -- ScanSystemSensor
//  Exactly ONE site, one member.  Nothing else in golden ckernel.cpp mentions
//  fSetup, so this facade is deliberately a single field: golden csetup.h
//  declares ~30 other public members (tSiteMap, shtMode, bSiteMapHasChange,
//  iTestMode, SaveSetupFile(), CheckShuttlePitch(), ...) and none of them is
//  reachable from the front this wave serves.  Growing the facade past its
//  measured need is how facades rot.
// =============================================================================
#ifndef FORMS_FSETUP_H
#define FORMS_FSETUP_H

// Dependency-free on purpose: the single member is a bool.  See forms/fHome.h
// for the same reasoning.

class TfSetup
{
public:
    // -----------------------------------------------------------------------
    //  [DATA] golden csetup.h:261 `bool fShow;` -- "the Setup dialog is open",
    //  i.e. the operator is editing test-mode / site-map / shuttle-mode setup.
    //  Golden's two writers are csetup.cpp:1850 `fShow=true;` (the form Show
    //  path, commented "must be BEFORE ReadFile()") and csetup.cpp:3358
    //  `fShow=false;` -- both pure VCL form lifecycle with no ported home, so
    //  offline this stays at its constructed value permanently.
    //
    //  OFFLINE VALUE false, GOLDEN'S OWN CTOR VALUE (csetup.cpp:163
    //  `fShow=false;` inside `__fastcall TfSetup::TfSetup(TComponent* Owner)`
    //  at csetup.cpp:123).
    //
    //  THE BRANCH false SELECTS -- this is the whole reason the field exists,
    //  and the W7-L2 brief flagged it as recon risk #3:
    //    golden ckernel.cpp:411 `if(iHome==0 && fSetup->fShow==false)`
    //    false OPENS the arm at :413-439 -- the START-time motor-home check.
    //    With the arm open and fAllMotorHome true, ScanSystemSensor runs the
    //    four-way home ladder:
    //        :415 bNeedArmZHome            -> InitDoArmZHome()
    //        :419 FrmRotate->bRotateInHome -> FrmRotate->InitialInRotateHome()
    //        :423 FrmRotate->bRotateOutHome-> FrmRotate->InitialOutRotateHome()
    //        :427 bYpitchNeddHome          -> bYpitchNeddHome=true
    //        :431 else                     -> if(CheckMotorHome()==false)
    //                                           ShowMyMessage("Must home again")
    //                                           return false;   <-- START REFUSED
    //    So false is emphatically NOT the "inert" choice: it is the choice that
    //    can make ScanSystemSensor RETURN FALSE and refuse to start the machine.
    //    true would have skipped the entire ladder and let START through with no
    //    home verification at all -- golden's intent being "while the operator
    //    has Setup open, do not nag about homing".  false is the running-machine
    //    state and the faithful one, and it is also the state that keeps the
    //    :419/:423 FrmRotate calls (this wave's other facade) reachable at all.
    // -----------------------------------------------------------------------
    bool fShow;

    TfSetup();
    virtual ~TfSetup() {}
};

// golden: extern PACKAGE TfSetup *fSetup;   (csetup.h:293)
extern TfSetup *fSetup;

#endif // FORMS_FSETUP_H
