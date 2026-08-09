// =============================================================================
//  csystem_shims.cpp  --  HT9045 system-HUB offline link shims (W6.6)
//
//  Translation wave: W6.6 (csystem orchestration SPINE)
//  Author: AI(W6.6-HUB) 20260626
//
//  See csystem_shims.h for the role description.  ACTIVE offline stand-ins for
//  the cross-module free functions the HUB main loop (csystem.cpp DoAllProcess)
//  calls but whose BCB6 source units are NOT translated this wave.  The DoInArm /
//  DoOutArm shims faithfully reproduce the golden pre-guards / kit setup and then
//  call the REAL translated engine entry (DoInArm_9045 / DoOutArm_9045), so the
//  in/out-arm engines are genuinely pumped by the loop.  All other shims are
//  faithful-but-offline cursor resets / no-ops.  NOTHING here is #if 0.
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "csystem_shims.h"

#include "cmydef.h"                 // bInitialStartIndexCheckDone / iTestBinCount / HAS_IC / TEST_PASS / IniConfig / MInShuttle1.. / bDoingF16
#include "cprod.h"                  // Prod (not strictly needed; pulled for parity)
#include "cpublic.h"
#include "MachineType.h"
#include "Motor/mymotor.h"          // MOT[] (fCanMoveL/M used by the F16 guard)
// ainarm9045.h / aoutarm9045.h MUST precede aHotPlateSubstrate.h: all three
// declare InArmLeftSideNoIC/HasIC(int=2) with a default arg, and g++ rejects the
// repeated default unless the engine header is seen first (same order the engine
// TUs use).
#include "ainarm9045.h"             // DoInArm_9045()
#include "aoutarm9045.h"            // DoOutArm_9045()
#include "aHotPlateSubstrate.h"     // OutArmSuck / FRCarryKit / BRCarryKit / InArmSuck
#include "ainarm_SearchPlacePlate.h"// iHPHangUpCount / sHPHangUpFunc / ClearHotPlateHangUp()
#include "canary_support.h"         // RecordProcess / ShowErrorMessage
#include "FormsFacade.h"            // fMain->DoStateRecord
#include "csystem.h"                // XPitchIsStand / DoSiteMappingCHK / AutoSiteMappingCheckShuttle decls

// ---------------------------------------------------------------------------
//  bShuttleShake -- golden ainarm2.cpp:78.  Offline default false (no shake).
// ---------------------------------------------------------------------------
bool bShuttleShake = false;

// ===========================================================================
//  DoInArm  -- golden ainarm2.cpp:1620 (fat in-arm dispatcher).
//  Faithful head guards (Index-check-done / HP hang-up / F16 sensor-broken),
//  then dispatch to the translated in-arm engine entry.  The QA-mode +
//  variant-dispatch bulk between the guards and DoInArm_9045() lives in
//  ainarm2.cpp (NOT translated); offline it is skipped -- DoInArm_9045() owns
//  the real per-tick in-arm SM (translated in ainarm9045.cpp).
// ===========================================================================
void DoInArm()
{
    if(bInitialStartIndexCheckDone==false)                                      // golden ainarm2.cpp:1622
    {
        return;
    }

    if(iHPHangUpCount!=0)                                                       // golden :1627 -- HotPlate hang-up
    {
        RecordProcess("Auto State Record by in arm");
        fMain->DoStateRecord(0, false);
        ShowErrorMessage("WAR0150", 0, MInArmX, false, sHPHangUpFunc);
        ClearHotPlateHangUp();
        fAllMotorHome=false;
        return;
    }

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     // golden :1637 -- shuttle sensor-broken
    {
        MOT[MInShuttle1].fCanMoveL=true;                                        // avoid follow-on hang up
        MOT[MInShuttle2].fCanMoveL=true;
        MOT[MInShuttle1].fCanMoveM=true;
        MOT[MInShuttle2].fCanMoveM=true;
        return;
    }

    // golden :1646-end : QA-mode + the variant-dispatch ladder live in the
    // untranslated ainarm2.cpp; offline we proceed straight to the engine.
    DoInArm_9045();                                                            // translated engine entry (ainarm9045.cpp)
}

// ===========================================================================
//  DoOutArm -- AI(W906-PT-W4-integrate) 20260809: THIS STAND-IN IS RETIRED.
//  The wrapper that used to live here was a partial rendering of golden
//  aoutarm.cpp:1195 -- it skipped golden's kit-prep (FRCarryKit/BRCarryKit
//  SetHasNullIcToNullIc + the FRCarryKit.Item -> TEST_PASS+bin result-tag loop,
//  golden :1197-1209) because the substrate's TMyKitSuck did not expose
//  SetHasNullIcToNullIc, and dispatched straight to DoOutArm_9045().  Its own
//  banner carried a TODO(W7) to restore the verbatim prep loop.
//  aoutarm.cpp landed in wave PT-W4 with golden's REAL body (port :1749), so the
//  two collided in libht9045_sm.a and this partial one goes.  The behaviour delta
//  is therefore not "no-op becomes real" but "partial becomes whole": whatever
//  golden does in :1197-1209 now actually happens.
// ===========================================================================

// ===========================================================================
//  DoLoad   -- golden asendic_Loader.cpp:2448 (loader feed SM).  Loader feed is
//  NOT in W6.6 scope; offline no-op.
//  DoSortArm-- golden asortarm.cpp:349 (9046AU sort-arm).  USE_OUT_SORT_ARM
//  defaults eartUninstall so the HUB never calls it; offline no-op.
// ===========================================================================
// DoLoad: RETIRED BODY (W7-L1 Wave 1 "Loader") -- real body now in
// asendic_Loader.cpp.  Declaration kept in the header: csystem.cpp calls it
// through csystem_shims.h and the two signatures are identical.
#if 0   // PT-W5c RETIRED (DoSortArm)
//AI(ht9045-v906) 20260809: PT-W5c phase 2 -- RETIRED. PRE-EXISTING latent ODR violation, not this wave: the real body always existed, but its archive member was never extracted until csystem.cpp entered the link. Real body wins.
void DoSortArm() {}
#endif

// ===========================================================================
//  SetFixTrayMiddleDtata -- golden aoutarm.cpp (Fix-tray middle-data shuffle).
//  Offline no-op (no Fix tray in the sim feed).
// ===========================================================================
// AI(W906-PT-W4-integrate) 20260809: 2 STAND-IN DEFINITION(S) RETIRED FROM HERE.
//   aoutarm.cpp (golden's own home for all of them) landed in wave PT-W4 and is
//   registered in ht9045_sm, so both definitions were in libht9045_sm.a and every
//   executable linking it failed with `multiple definition of ...`. The linker named
//   each one, which is also the proof the signatures match exactly -- a decorated-name
//   collision cannot happen otherwise.
//   Retired here: SetFixTrayMiddleDtata, InitOutArmTask
//   BEHAVIOUR: these were offline defaults (return true/false/0/no-op); the real bodies
//   run golden's actual logic, so out-arm paths that used to short-circuit now execute.
//   That is the point of the wave, and it is why this wave was measured on its own.

// ===========================================================================
//  InitAllProcessTask() leaves with no translated home -- offline cursor/flag
//  resets (the engines own the rest).
// ===========================================================================
// InitLoadTask: RETIRED BODY (W7-L1 Wave 1 "Loader") -- real body now in
// asendic_Loader.cpp (golden).  Behaviourally inert: the only ported caller is
// asendic_Loader.cpp's own case 1400.
void InitialDoLockUnloader(int /*iAuto*/) {}  // golden csystem.h:258 -- unloader lock cursor reset (real body W7)
// Initial_Auto_BinTray_Task: RETIRED BODY (W7-L1 Wave 2 "Auto") -- real body now
// in asendic_Auto.cpp (golden asendic_Auto.h:6).  Declaration kept in the header.
// InitAutoColorTask: RETIRED BODY (W7-L1 Wave 1 "Color") -- real body now in
// asendic_Color.cpp (golden asendic_Color.h:15).
void InitialAuto3MagazineTask()      {}   // golden Magazine SM
void InitialCatchTrayChangeTray()    {}   // golden Magazine SM
void InitialCatchTrayGetNewTray()    {}   // golden Magazine SM
void InitialDoMagazineTrayFeedTask() {}   // golden Magazine SM

// ===========================================================================
//  Tail Auto-tray / alignment / vibration loops the HUB calls every tick.
// ===========================================================================
void DoAuto3Magazine()               {}   // golden Magazine SM (only when AUTO3_IS_MAGAZINE==1)
// NewDoAutoTrayEdgeCylinderLoop: RETIRED BODY (W7-L1 Wave 3 / L1b) -- the real
// golden body now lives in asendic.cpp (golden asendic.cpp:1206-1233, the
// "2011.10.19 Q_Q V207C new Tray Vibration" loop with its NewLoopEdgeDelay /
// NewLoopEdgeOnDelay pair and the IniConfig.iVibratorUnloader accumulator), so
// keeping this no-op would be a duplicate symbol at link.  The DECLARATION in
// csystem_shims.h is KEPT, exactly like DoLoad / InitLoadTask: csystem.cpp binds
// to the symbol through that header and does not include asendic.h.
// AI(W906-W7-L1-Wave3) 20260802.
// DoAutoEmpty1() is owned by asendic_Empty.cpp (ODR) -- NOT defined here.
// DoAuto2() : REAL faithful body now lives in asendic_Auto2.cpp (golden
// asendic_Auto2.cpp:414-592) as of W906-W7-L1.  The no-op stub here was REMOVED
// to keep a single ODR definition; the prototype now lives in asendic_Auto2.h.
// AI(W906-W7-L1) 20260729.

//  Alignment processes: offline they report "finished" so the HUB does NOT
//  early-return on them (golden returns true when the alignment step completes;
//  DoAllProcess does bRunXXXAutoAlignment = !DoXXXTeachAlignmentProcess(...)).
bool DoInArmTeachAlignmentProcess(unsigned long & /*lAction*/)  { return true; }   // golden AutoAlignment.cpp:4473 -- offline: complete in one step
bool DoOutArmTeachAlignmentProcess(unsigned long & /*lAction*/) { return true; }   // golden AutoAlignment.cpp:8627 -- offline: complete in one step

// ===========================================================================
//  End-of-lot bookkeeping FSMs (golden :12813 / :14713, ~2900 lines, GATED in
//  csystem.cpp).  ACTIVE no-op stubs so MainProc's tail calls link + pump.
// ===========================================================================
// DoOneCycleFinishCheck() : REAL faithful body now lives in csystem.cpp (golden
// csystem.cpp:12813-14047) as of W7-C2.  The no-op stub here was REMOVED to keep
// a single ODR definition; the prototype now lives in csystem.h.  AI(W7C2-Integrate) 20260701.
// DoCleanOutFinishCheck() : REAL faithful body now lives in csystem.cpp (golden
// csystem.cpp:14713-15748) as of W7-C1.  The no-op stub here was REMOVED to keep
// a single ODR definition.  The prototype now lives in csystem.h.  AI(W7C1-Integrate) 20260629.

// ===========================================================================
//  W6.2c batch-1: csystem predicates/actions the 1x2_*/1x3_* in-arm variants
//  call in ACTIVE code (the dead 2x4_16/2x8_32 variants only call them in gated
//  blocks, so they were never needed before).  AI(W6.2c-INARM) 20260626.
// ===========================================================================
// XPitchIsStand (golden csystem.cpp:264): pure config predicate over TestIF /
// CosFunction / iXpitch* -- transcribed VERBATIM (all inputs are config globals,
// no HW).  Offline-faithful: returns the exact golden value for the live config.
#if 0   // PT-W5c RETIRED (XPitchIsStand)
//AI(ht9045-v906) 20260809: PT-W5c -- RETIRED. The real faithful body now lives in csystem.cpp (its golden home); keeping this stand-in is a multiple-definition error, measured in build_0809_w5c. Same convention as csystem_shims.cpp:165.
bool XPitchIsStand()
{
    if(CosFunction.b2x4SupportCenterPitch==true &&                              //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
      (TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4) &&          //Sam 20190226 : 16Site4X4
       TestIF_File.bEnableUseXCenterPitch==true)
    {
        return false;
    }
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
    {
        return false;
    }
    else if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site2X4 ||
            TestIF.iTestMode==_16Site2X8  || TestIF.iTestMode==_12Site2X6 ||
            TestIF.iTestMode==_6Site2X3   ||                                    //ChungHung 20140115 add for 2x3
            TestIF.iTestMode==_8Site1X4   ||                                    //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF.iTestMode==_16Site4X4  ||                                    //Sam 20190226 : 16Site4X4
            TestIF.iTestMode==_32Site4X8N ||                                    //2013-01-15    Dell    Add nn Mode
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        if(TestIF.dSiteXPitch>iXpitchMax)                                       //Isaac 20171204 (Steven) : Xpitch40->50mm
            return false;
    }
    else if(TestIF.iTestMode==TriSite1X3 ||                                     //Frank 20160329 add for 1x3_4
            TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        if(iInArmType==e9045_1x3_2_14)
        {
            if(TestIF.dSiteXPitch>iXpitchMaxX3)
                return false;
        }
        else
        {
            if(TestIF.dSiteXPitch>iXpitchMax)
                return false;
        }
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
        return false;
    }
    else if(TestIF.iTestMode==QualSite2X2)
    {
        if(iInArmType==e9045_2x2_4_14)                                          //Steven 20191023 : fixed for 2x2_14 with Y-Pitch
        {
            if(TestIF.dSiteXPitch>iXpitchMaxX3)
                return false;
        }
        else if(TestIF.dSiteXPitch>iXpitchMaxX2)                                //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            return false;
        }
    }
    else if(iInArmType==e9045_1x2_2_13 ||
            iInArmType==e9045_1x2_4_Hot)                                        //Steven 20150505 : 1x2加大支援X-Pitch 120mm
    {
        if(TestIF.dSiteXPitch>iXpitchMaxX2)                                     //Isaac 20171204 (Steven) : Xpitch40->50mm, 8000->iXpitchMaxX2
            return false;
    }
    else if(iInArmType==e9045_1x2_2_14)                                         //Steven 20220926 : for 1x2_14
    {
        if(TestIF.dSiteXPitch>iXpitchMaxX3)
            return false;
    }
    return true;
}
#endif
// DoSiteMappingCHK (golden csystem.cpp:20183) / AutoSiteMappingCheckShuttle
// (golden csystem.cpp:22202): JCET/ASE Auto-Site-Mapping HW side-effect routines
// (site-map check + which-shuttle select).  Offline-safe no-ops -- there is no
// site-map HW offline and the bRunAutoSiteMapping path is inert (consistent with
// DoCheckAutoSiteMappingPosition's offline no-op in aHotPlateSubstrate.cpp).
void DoSiteMappingCHK(bool /*bAdd*/) {}
void AutoSiteMappingCheckShuttle(bool /*bWhich32*/) {}
