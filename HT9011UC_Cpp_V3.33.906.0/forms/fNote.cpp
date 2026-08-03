// =============================================================================
//  forms/fNote.cpp  --  definitions for the fNote facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Body moved VERBATIM.
// =============================================================================
#include "forms/fNote.h"

// AI(W906-W7-L2) 20260803: cmydef.h is the ONLY new include, and it covers all
// three external things golden's IsTestSitICFallDown body reads:
//   * `bool bIsTestSitICFallDown`      -- cmydef.h:2723 (defined cmydef.cpp:2915)
//   * MTestY1/MTestZ1/MTestZ2/MTestY2  -- cmydef.h:2114-2117 (cmydef.cpp:2347-2350)
//     MOutShuttle1/MOutShuttle2        -- cmydef.h:2118/:2120 (cmydef.cpp:2351/2353)
//     MMIndex                          -- cmydef.h:2401 (cmydef.cpp:2695)
//   * `HT9045_CONFIG IniConfig`        -- Config.h:1499, reached transitively
//     (cmydef.h:10 -> cprod.h:6 -> Config.h); defined cprod.cpp:50.
// NO CMakeLists CHANGE IS NEEDED FOR THIS: every one of those symbols lives in
// cmydef.cpp / cprod.cpp, i.e. in ht9045_globals, which ht9045_forms already
// declares as a link dependency -- the statement is `target_link_libraries(
// ht9045_forms PUBLIC vclcompat ht9045_globals)` at CMakeLists.txt:617, and this
// file is already in that target's source list (`forms/fNote.cpp`, :574), both as
// of this pass.  The "DEPENDENCIES -- and why they are acyclic" block that opens
// at :509 records why that edge is safe.  (Line numbers there will shift when the
// integrator edits CMakeLists.txt; the quoted statement text is the stable key.)
// forms/fMain.cpp already sets the precedent of including
// cprod.h/cmydef.h from this library.  No new translation unit is created.
#include "cmydef.h"

// --- W906-AutoCleanFoundation ADD: TfNote (first home, see forms/fNote.h) --
// AI(W906-W7-L2) 20260803: init list extended with the three new members, in
// DECLARATION ORDER (fShow, AlarmType, edErrorCode) so no -Wreorder.  The four
// pre-existing initialisers are unchanged.  fShow(false) is golden note.cpp:164;
// AlarmType(0) is golden note.cpp:218; edErrorCode is allocated because golden's
// is a real TEdit created by the .dfm loader and IsTestSitICFallDown
// dereferences it unconditionally (golden note.cpp:5472) -- a null here would
// be a crash, not a default.  vclcompat::TEdit's own Text defaults to "".
TfNote::TfNote()
    : bMyServoOffInArm(false), iMyServoOffInArmPosX(0), iMyServoOffInArmPosY(0), aJamCodeFilePath(""),
      fShow(false), AlarmType(0), edErrorCode(new vclcompat::TEdit())
{
}
TfNote *fNote = new TfNote();

// AI(W906-W7-L2) 20260803: golden note.cpp:83 `int iPosition;` -- see the
// declaration comment in forms/fNote.h for who writes it in golden
// (ShowErrorUnit, golden note.cpp:4511/:4514, no ported home) and for what the
// zero value selects.  Written `=0` explicitly rather than relying on static
// zero-init, to make the value reviewable at the point of definition; golden's
// bare `int iPosition;` has the identical initial value.
int iPosition = 0;

// ---------------------------------------------------------------------------
//  AI(W906-W7-L2) 20260803: FAITHFUL translation of golden note.cpp:5462-5492
//  `bool __fastcall TfNote::IsTestSitICFallDown()`.  Structure, operand order,
//  the redundant `else { bReturn=false; }` and the separate second `if` block
//  are all preserved verbatim; only `__fastcall` is dropped (tree-wide idiom)
//  and `edErrorCode` now resolves to the vclcompat TEdit stand-in.  Golden's
//  own comments are carried over on the lines they annotate.
//
//  WHAT ITS RETURN VALUE SELECTS IN ScanPannelKey -- the branch justification
//  this facade owes.  The method is called at golden ckernel.cpp:2009, :2040,
//  :2186 and :2217, always as `if(fNote->IsTestSitICFallDown()==false)`:
//    * false OPENS the plain arms (:2011-2015 FKRetry, :2042-2046 FKSkip,
//      :2188-2192 RKRetry, :2219-2223 RKSkip) -- Retry/Skip are accepted with
//      no extra condition.
//    * true selects the else arms (:2017-2027, :2048-2058, :2194-2204,
//      :2225-2235), where the SAME key is accepted only when
//      IniConfig.bD40IndexICFallDownMustPressFMotorDown==false, i.e. the
//      "after an index drop you must press the motor-down key first" lockout.
//  OFFLINE, this body returns false unless a test drives it, and it does so
//  through a REAL evaluation, not a hard-coded return:
//    - bIsTestSitICFallDown is genuinely live in this tree: a tree-wide grep
//      outside build*/ counts 22 `bIsTestSitICFallDown=true;` writers, incl.
//      ported acarry.cpp:2399, atester.cpp:2753 and AutoClean/AutoClean.cpp:
//      5862, so half of every conjunct really does move;
//    - iPosition is the discriminator and offline stays 0 == MInArmX
//      (ported cmydef.cpp:2334 `const int MInArmX       =0;`; golden
//      cmydef.cpp:2330), which matches none of the seven motor ids tested, so
//      arms 1 and 3 stay closed;
//    - edErrorCode->Text stays "" so arm 2 stays closed.
//  Note the consequence honestly: with iPosition's only golden writer
//  (ShowErrorUnit) untranslated, NOTHING in the ported tree can currently make
//  this return true on its own -- a test must set iPosition (or the Text) to
//  walk the lockout arms.  Both are public/extern precisely so it can.
//  Being false by default is also the behaviour-preserving choice for the
//  common case: when bD40IndexICFallDownMustPressFMotorDown is false the two
//  arms are observationally identical anyway.
// ---------------------------------------------------------------------------
bool TfNote::IsTestSitICFallDown()                                              //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
{
    bool bReturn=false;
    if(bIsTestSitICFallDown==true &&
       (iPosition==MTestY1 || iPosition==MTestZ1 ||
        iPosition==MTestY2 || iPosition==MTestZ2 ||
        iPosition==MMIndex))
    {
        bReturn=true;
    }
    else if(bIsTestSitICFallDown==true && (edErrorCode->Text=="MES1640" || edErrorCode->Text=="MES1641"))               //JerryYang 20220923 : 掉料後one cycle finish要開門按z1
    {
        bReturn=true;
    }
    else
    {
        bReturn=false;
    }

    if(IniConfig.bOutShLoseNeedOpenChamber==true ||                             //jou 2013-12-12 Out Shuttle Lose Device Need Open Chamber Door and press Z1
       IniConfig.bF24OutShuttleLoseIcOpenIndexDoor)                             //kevin 20180725 add
    {
        if(bIsTestSitICFallDown==true &&
           (iPosition==MOutShuttle1 || iPosition==MOutShuttle2))
        {
            bReturn=true;
        }
    }

    return bReturn;
}

// AI(W906-W7-L2) 20260803: SHIM RETIREMENT DEBT, REPORTED NOT DISCHARGED.
// Two offline stand-ins for fNote->fShow now have a real member to point at,
// but both live in files this wave does not own, so neither is touched here:
//   * atester_32Site.cpp:302  `#define W5_32S_FNOTE_FSHOW (false)`, consumed at
//     atester_32Site.cpp:2835.  Retiring it means `if(fNote->fShow) break;`.
//   * Automation/AGV_PortScan.h:61-70 records TfAGV::Timer2Timer as
//     deliberately untranslated *because* fNote had no home; per that same
//     comment its guard is `if(!fNote->fShow || InitialOK==false) return;` and
//     its golden span is Automation/AGV.cpp:1290-1307.  That span was AGV_
//     PortScan.h's own claim and is now VERIFIED against golden: :1290 is
//     `void __fastcall TfAGV::Timer2Timer(TObject *Sender)`, the guard is split
//     over :1294 `if(!fNote->fShow || InitialOK==false)` + :1295 `return;`, and
//     :1307 is the closing brace.  The stated reason for deferring it has now
//     expired.
// Both are behaviour-identical to the new member's offline value (false), so
// retiring them is a pure cleanup with no behaviour change -- which is exactly
// why it is safe to defer to the owning wave rather than reach across files.
