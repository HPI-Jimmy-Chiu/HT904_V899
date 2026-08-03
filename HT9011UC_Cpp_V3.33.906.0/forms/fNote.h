// =============================================================================
//  forms/fNote.h  --  non-VCL stand-in for golden's fNote form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtual destructor.  See forms/fMain.h for the facade-wide
//  contract.
// =============================================================================
#ifndef FORMS_FNOTE_H
#define FORMS_FNOTE_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfNote -- FIRST home for fNote
//  (golden note.h `class TfNote : public TForm`) anywhere in this migrated
//  tree. Multiple earlier waves (Automation/AGV_PortScan.h's TfAGV::Timer2Timer
//  note, BarCode/BarCode_Bottom2DID.h's fNote->t2DCode/bMyServoOffInArm note)
//  found fNote had no home and deliberately left their own fNote-dependent
//  code gated/untranslated rather than stand up "a whole new TfNote facade
//  CLASS" for what was, in each of those cases, a single narrow call with no
//  other payoff. This wave's need is different in kind, not just degree: TWO
//  genuinely in-scope Part A functions (DoInArmPineRelease's servo-off-during-
//  alarm recovery cycle; InitialAutoCleanTask's SPIL FTP jam-code upload trace)
//  read/write exactly 4 plain-data fields -- no VCL lifecycle, no widgets, no
//  settings-editor surface like the deferred cases above. Standing up THIS
//  minimal a home costs nothing extra and unblocks a faithful (not gated)
//  translation of both functions; it does not retroactively un-gate any of
//  the earlier waves' own deferred fNote call sites (still all self-gated,
//  unaffected by this addition).
// ===========================================================================
//
// ===========================================================================
//  AI(W906-W7-L2) 20260803: SECOND growth of TfNote -- the golden ckernel.cpp
//  surface.  This wave is the substrate pass in front of the W7-L2 translation
//  of golden ckernel.cpp (2589 lines, 17 functions, 0% ported).  Everything
//  below is backed by a golden line I decoded from cp950 and read.
//
//  (1) EXHAUSTIVE fNote CENSUS OF GOLDEN ckernel.cpp -- MEASURED, NOT QUOTED.
//      A scan of all 2589 lines finds `fNote` on exactly 18 lines carrying 30
//      `fNote->` dereferences, in 4 of the file's 17 functions.  Broken down by
//      function and by member:
//        ShowRunLed()   (golden :704-932, DEFERRED by the W7-L2 recon):
//            :708  fNote->fShow                      (1 deref)
//            :711-:716 fNote->edErrorCode->Text      (18 derefs, one per
//                      MESxxxx literal in the bTowerLightUseLD LD/ULD chain)
//            :720  fNote->AlarmType==3               (1 deref)
//            ( = 20 for the function, which reproduces the recon's "fNote 20,
//              edErrorCode 18" for ShowRunLed exactly)
//        ShowRunLabel() (golden :935-1726, DEFERRED):
//            :971  fNote->fShow==true                (1 deref)
//        DoPanelLamp()  (golden :1728-1897, THIS FRONT):
//            :1749 fNote->fShow==false               (1 deref, the only one)
//        ScanPannelKey() (golden :1919-2406, THIS FRONT):
//            :2007 :2038 :2184 :2215  fNote->fShow               (4 derefs)
//            :2009 :2040 :2186 :2217  fNote->IsTestSitICFallDown() (4 calls)
//      1 + 8 = 9 live sites inside the two ranges this front owns -- the recon
//      count is CONFIRMED exactly, and so is its warning that the two `fMain->`
//      tokens at golden :1981 and :2158 are NOT live.  Re-read verbatim, both
//      lines are identical and read
//        `if(Sen[SnFKStart].IsOn() || bAseStart)` / `...SnRKStart...`
//      followed by the trailing comment
//        `//Ifor 20160114 Auto Decay 搬移 至 fMain->ScanKey 判斷`.
//      So the CODE on those lines is live but contains no form dereference at
//      all; `fMain->ScanKey` occurs only inside the `//` comment text.  They
//      cost this facade nothing.
//
//  (2) DISCREPANCY, REPORTED LOUDLY.  The recon's dependency item C.7 says
//      fNote needs "`bool fShow;` and `bool IsTestSitICFallDown();`".  That is
//      right about the CALL SITES and wrong about the SUBSTRATE, because it
//      never opened IsTestSitICFallDown's BODY.  Golden's body (note.cpp:5462-
//      5492) reads TWO more things that had no home here: the form's own
//      `TEdit *edErrorCode` (golden note.h:48) and the note.cpp file-scope
//      global `int iPosition` (golden note.cpp:83).  Both are landed below;
//      without them the method could only have been a fake-success stub.
//
//  (3) `int AlarmType` (golden note.h:432) is landed too even though NEITHER
//      of this front's two functions touches it.  Stated plainly so it is not
//      mistaken for a measured need: it is the one and only remaining fNote
//      member dereferenced anywhere in golden ckernel.cpp (census (1) above,
//      ShowRunLed :720), it is a plain int with a golden constructor value,
//      and this file is under a one-owner-at-a-time serialised editing regime
//      -- leaving it out guarantees a second serialised edit to this same file
//      when ShowRunLed/ShowRunLabel come off the DEFERRED list.  Its offline
//      value is still justified as a branch selection below, like every other.
//
//  (4) NOT LANDED, ON PURPOSE.  `bMyServoOffOutArm` (golden note.h:417) and
//      `t2DCode` are dereferenced by ported aoutarm9045.cpp:388/:1460 and
//      BarCode/BarCode_Bottom2DID.cpp:1180 -- but every one of those sites is
//      inside a `#if 0 // TODO(W7)` block owned by ANOTHER wave, none of them
//      is in ckernel.cpp, and un-gating them needs edits to files this wave
//      does not own.  Adding the members without un-gating the call sites
//      would grow the facade for zero reachable behaviour.  Reported to the
//      integrator instead.
// ===========================================================================
class TfNote
{
public:
    bool       bMyServoOffInArm;     // [DATA] golden note.h:417ish (bool) -- "an alarm-triggered ServoOff on the in-arm is pending recovery"
    int        iMyServoOffInArmPosX; // [DATA] golden note.h:418 (int) -- encoder pos to jog back to (X)
    int        iMyServoOffInArmPosY; // [DATA] golden note.h:419 (int) -- encoder pos to jog back to (Y)
    AnsiString aJamCodeFilePath;     // [DATA] golden note.h:434 (AnsiString) -- SPIL FTP jam-code trace file path

    // -----------------------------------------------------------------------
    //  AI(W906-W7-L2) 20260803 ADD -- the golden ckernel.cpp surface.
    //  Every offline value below is GOLDEN'S OWN CONSTRUCTOR VALUE (read out
    //  of `__fastcall TfNote::TfNote(TComponent* Owner)`, golden note.cpp:161-
    //  onwards), not an invented "safe" default -- and each is then justified
    //  as the branch it selects, naming the golden line.
    // -----------------------------------------------------------------------

    //  [DATA] golden note.h:393 `bool fShow;`  -- "the alarm/note dialog is up".
    //  OFFLINE VALUE false, from golden note.cpp:164 `fShow=false;` in the ctor.
    //  The only two golden writers are note.cpp:2167 (`fShow=true;` in the form
    //  Show path) and note.cpp:2519 (`fShow=false;` in Reset) -- both are VCL
    //  form lifecycle with no ported home, so offline it is a permanent false.
    //  BRANCHES THAT false SELECTS (this is the point of the field, not a
    //  formality):
    //    * golden ckernel.cpp:1749 `if(fNote->fShow==false)` -- false OPENS the
    //      alarm-reset-lamp-extinguish arm at :1749-1756, i.e. DoPanelLamp DOES
    //      run `SW[SwFKAlarmReset].Off(); SW[SwRKAlarmReset].Off();` whenever
    //      bNeedMusicAndAlarmOn==false.  true would have left both lamps latched.
    //    * golden ckernel.cpp:2007/:2038/:2184/:2215 `if(fNote->fShow)` -- false
    //      takes the ELSE arms (:2029-2036, :2060-2067, :2206-2213, :2237-2244),
    //      which accept front/rear Retry and Skip UNCONDITIONALLY and never call
    //      IsTestSitICFallDown() at all.  That is golden's normal running state
    //      (no alarm dialog displayed), and it is exactly the state the W7-L2
    //      recon's DoPanelLamp test presumes (`fNote->fShow=false`).
    //  Consistent with every other offline fNote->fShow decision already taken
    //  in this tree: atester_32Site.cpp:298-302 `#define W5_32S_FNOTE_FSHOW
    //  (false)` and Automation/AGV_PortScan.h:61-70.  Those two seams are now
    //  RETIREABLE against this member -- see forms/fNote.cpp.
    bool fShow;

    //  [DATA] golden note.h:432 `int AlarmType;` -- 1=Jam / 3=MES-message class
    //  (golden note.cpp:846 has MyDBIEvent write it out through &fNote->AlarmType).
    //  OFFLINE VALUE 0, from golden note.cpp:218 `AlarmType =0;` in the ctor.
    //  BRANCH IT SELECTS: golden ckernel.cpp:720 `else if(fNote->AlarmType==3)`
    //  is FALSE at 0, so ShowRunLed -- a function this front DEFERS and does NOT
    //  translate -- falls to the else at :724-726 and sets
    //  RunState=LED_ErrJam (the jam-siren tower-light state) rather than
    //  LED_Message at :722.  0 is also not 1, which closes golden note.cpp:2528
    //  and :6765.  See banner note (3): landed ahead of its consumer on purpose.
    int AlarmType;

    //  [DATA] golden note.h:48 `TEdit *edErrorCode;` -- the note dialog's error-
    //  code box; the dfm2rc IR agrees it is a TEdit
    //  (build/.../note_layout.gen.cpp:148, "fNote.pnlNote.pnlButton.edErrorCode",
    //  class "TEdit").  Stock control, so per the facade contract rule 3 this is
    //  the unified vclcompat/Controls.h type, NOT a new bespoke struct.
    //  OFFLINE VALUE: allocated in the ctor with Text=="" (vclcompat::TCustomEdit
    //  default).  BRANCH IT SELECTS: golden note.cpp:5472's
    //  `edErrorCode->Text=="MES1640" || =="MES1641"` is FALSE on an empty Text,
    //  which CLOSES the "one-cycle-finish after a drop still needs the door open
    //  and Z1 pressed" arm of IsTestSitICFallDown (JerryYang 20220923).  It is a
    //  real settable field, not a constant, so a test can open that arm.
    vclcompat::TEdit *edErrorCode;

    //  [METHOD] golden note.h:430 `bool __fastcall IsTestSitICFallDown();`,
    //  body golden note.cpp:5462-5492.  FAITHFULLY TRANSLATED (not a stub) --
    //  the body is VCL-free once edErrorCode is a stand-in; see forms/fNote.cpp
    //  for the line-by-line correspondence and for the branch analysis of what
    //  its offline return value selects in ScanPannelKey.
    virtual bool IsTestSitICFallDown();

    TfNote();
    virtual ~TfNote() {}
};
extern TfNote *fNote;   // golden: extern PACKAGE TfNote *fNote; (note.h:462)

// AI(W906-W7-L2) 20260803: `iPosition` is golden's note.cpp:83 FILE-SCOPE
// GLOBAL (`int iPosition;`, external linkage, zero-initialised), not a member
// of TfNote -- kept a global here so the eventual real note.cpp port drops in
// without a signature change.  forms/fNote.cpp is this tree's stand-in for
// golden note.cpp, so note.cpp's file-scope data belongs here; when a real
// note.cpp lands, ownership of this definition moves with it.
// It is DECLARED (not just defined in the .cpp) because it is the sole
// discriminator IsTestSitICFallDown tests, so any test that wants to open that
// method's true-arms must be able to drive it.
// GOLDEN'S ONLY WRITER is `void ShowErrorUnit(int Pos)` at golden note.cpp:4511
// (`iPosition=Pos;` at :4514, declared golden note.h:469).  ShowErrorUnit has
// NO ported home anywhere in this tree: a tree-wide grep for `ShowErrorUnit`
// returns no definition and no call site -- the only textual hits are comment
// mentions (this file, and canary_support.cpp:375's "NOT reproduced" list).
// So offline iPosition never leaves its zero-init value.
// WHAT 0 SELECTS: 0 == MInArmX -- `const int MInArmX       =0;` at PORTED
// cmydef.cpp:2334 (golden cmydef.cpp:2330).  This file cites the PORTED tree for
// cmydef, consistently with forms/fNote.cpp's cmydef.cpp:2347-2350 citations.
// 0 is NOT any of MTestY1(13)/MTestZ1(14)/MTestZ2(15)/MTestY2(16)/MMIndex(503)
// nor MOutShuttle1(17)/MOutShuttle2(18) -- so golden note.cpp:5465-5468 and
// :5484-5485 both evaluate FALSE and IsTestSitICFallDown reports "no test-site
// IC fall-down", which is the correct state for a machine that has never had
// an error unit flushed onto the note panel.
extern int iPosition;

#endif // FORMS_FNOTE_H
