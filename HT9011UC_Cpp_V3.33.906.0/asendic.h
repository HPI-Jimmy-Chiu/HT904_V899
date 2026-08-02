// =============================================================================
//  asendic.h  --  tray-move / cylinder-lifter free-function interface
//
//  Mirrored (SUBSET) from: HT9011UC_Code_V3.33.906.0_20260618/asendic.h
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  The golden asendic.h is a GOD-HEADER: it #includes the 7 sibling SM headers
//  (asendic_Loader/_Loader_RT/_Auto/_Auto_RT/_Auto2/_Empty/_Color) and declares
//  the whole tray-handling free-function surface.  Pulling those siblings would
//  drag the entire Loader/Auto/Color SM god-stack into the W6.1 canary build.
//
//  This SUBSET declares the whole golden FREE-FUNCTION surface but NOT the
//  sibling-SM god-includes (those are kept in aArmHeader.h, gated #if 0).
//  Signatures here are VERBATIM from the golden (asendic.h:14-38).
//
//  AI(W906-W7-L1-Wave3) 20260802: EXTENDED to golden's full free-function
//  surface, because asendic.cpp no longer ships Sim bodies -- the whole golden
//  file (1581 lines) is now translated there, so every declaration golden's
//  asendic.h carries has a real definition behind it in this tree:
//      AutoCylinderUp / AutoCylinderMiddle / AutoCylinderLower  (golden :19-21)
//      PushLoaderTrayInAverageTime / RecordAutoCleanOutStartEnd (golden :22-23)
//      NewDoAutoTrayEdgeCylinderLoop / DoAutoTrayEdgeCylinderLoop (golden :24-25)
//      PushUnLoaderTrayInAverageTime                            (golden :26)
//      InitDoLoaderTrayFeedTask / DoLoaderTrayFeed              (golden :36-37)
//      bARTUnloaderUseOneCylin / bARTUnloaderUseTwoCylin        (golden :38-39)
//  Two golden asendic.cpp symbols are deliberately NOT declared here, because
//  golden's asendic.h does not declare them either and adding them would be an
//  invention rather than a translation:
//      * initLifterTask() / initAutoTask() -- golden main.cpp:9138-9139
//        extern-declares them LOCALLY at its own file scope.
//      * CheckCylinderCanMove(int) -- golden Magazine.cpp:33 extern-declares it
//        locally ("a-side asendic.cpp L1447").
//      * AutoCylinderMidIsOn(int,int) -- golden declares it nowhere; it is
//        reached only from inside asendic.cpp.
//  Likewise NOT declared here: iLifterTask[3][10] / iAutoTask[3][MAX_UNLOAD_TRAY]
//  (golden asendic.cpp:28/:35).  Golden main.cpp:9136-9137 extern-declares BOTH
//  as [3][7], i.e. with a DIFFERENT ROW STRIDE from the definition, which makes
//  its 42 QueueTaskList registrations address the wrong slots for rows 1 and 2.
//  Publishing either stride here would silently pick a winner and change
//  behaviour under the guise of a header cleanup.  Left alone for the W7-U
//  main.cpp wave; anything that must reach these arrays declares them locally
//  against asendic.cpp's DEFINITION strides and says so.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef asendicH
#define asendicH
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "asendic_Empty.h"          // the canary's own SM prototypes + task ints

// AI(W906-W7-L1-Wave0) 20260801: golden asendic.h:14 `extern int
// iTrayZLoadTrayToWaitTask;` -- DECLARATION ONLY, landed here once in the
// serialized pass so the W7-L1 agents that only READ it do not each add their own.
// The DEFINITION belongs to the Loader bundle: golden asendic_Loader.cpp:51
// `int iTrayZLoadTrayToWaitTask=1;` (note the non-zero initial value -- it is the
// SM's idle cursor, and golden :1471 resets it to 1, :1478 binds `int &Task=` to
// it).  Verified absent tree-wide before adding.  Every other file (golden
// asendic_Auto_RT.cpp writes it at :951) externs only.
extern int iTrayZLoadTrayToWaitTask;                                            // golden asendic.h:14

// --- lifter cylinder GoUp / GoMiddle / GoDown (golden asendic.h:16-18) -------
extern bool CylinderUp(int CylinderName);
extern bool CylinderMiddle(int CylinderName);
extern bool CylinderLower(int CylinderName);

// --- per-Auto stack cylinder GoUp / GoMiddle / GoDown (golden asendic.h:19-21)
// AI(W906-W7-L1-Wave3) 20260802: PARAMETERS 2 AND 3 ARE NOT SYMMETRIC AND GOLDEN
// CALLERS DISAGREE ABOUT THEIR ORDER -- do NOT "normalise" any call site.  In the
// real bodies (asendic.cpp) CylinderName is driven UNCONDITIONALLY while
// CylinderNameMid is Enable-guarded and is the sole input to AutoCylinderMidIsOn,
// so swapping them drives the opposite physical cylinder.  Every site below was
// re-derived by grepping the cp950-decoded golden IN THIS PASS:
//   * the COMMON order is (Up, Selector) -- golden asendic_Auto.cpp:282-284,
//     :310, :322; asendic_Auto_RT.cpp:75, :109, :121; csystem.cpp:7175, :7196;
//     acatchtray.cpp:8957-9003.
//   * asendic_Auto2.cpp passes the REVERSE, (C_Auto2_Selector, C_Auto2_Up), at
//     ALL 12 of its call sites: :65, :85, :101, :106, :132, :137, :617, :620,
//     :638, :643, :674, :679.
//   * ELEVEN further golden sites take the reversed order as the ELSE arm of a
//     runtime bARTUnloaderUseTwoCylin test -- the SAME call site swaps at run
//     time: asendic_Auto.cpp:788 (vs :784), :819 (vs :811), :1363 (vs :1350),
//     :1547 (vs :1539), :1631 (vs :1623); asendic_Auto_RT.cpp:783 (vs :791),
//     :816 (vs :814/:818), :856 (vs :854/:858), :872 (vs :870/:874);
//     csystem.cpp:6940 (vs :6936), :6956 (vs :6948).
// AI(W906-W7-L1-W3fixB) 20260802: CORRECTED CITATION -- the four lines that used
// to sit here named `tests/test_w7_l1_wave3_argorder.cpp`, a file that has never
// existed in this tree.  The coverage itself is real; it just lives elsewhere, and
// pointing at a non-existent file made a true claim unverifiable.  THE PIN IS
// tests/test_w7_l1_auto2.cpp SUB-TEST [12] (`test_argument_order_pin`, called from
// that file's main(), shipped as the ctest target W7_L1_Auto2).  [12b] shows that
// once parameters 2 and 3 trade places AutoCylinderUp NEVER returns true and
// raises golden's "Lifter Up error" dialog that the correct order never raises;
// [12c] shows AutoCylinderMiddle then extends the OPPOSITE physical cylinder while
// still returning true.  MUTATION-MEASURED IN THIS PASS, not asserted: swapping
// the two arguments at the single call site golden asendic_Auto2.cpp:85
// (DoLoadNewAuto2TrayToCar case 50) takes that suite from 69 passed / 0 failed to
// 62 passed / 7 failed.  While the old acatchtray_shims stubs returned true
// unconditionally the difference was invisible and no test could see it; that
// window is now closed.
extern bool AutoCylinderUp    (int Part, int CylinderName, int CylinderNameMid, bool bReset=false);     //kevin 20120718 修改輸入氣缸因代號位置不同共用程式)//Part 0=Auto1 1=Auto2 2=Auto3.....
extern bool AutoCylinderMiddle(int Part, int CylinderName, int CylinderNameMid, bool bReset=false);
extern bool AutoCylinderLower (int Part, int CylinderName, int CylinderNameMid, bool bReset=false);     //kevin 20120718 修改輸入氣缸因代號位置不同共用程式

// --- tray-in average-time trackers + AutoClean-out record (golden :22-23, :26)-
extern void PushLoaderTrayInAverageTime(DWORD st);
extern void RecordAutoCleanOutStartEnd(bool bStart);                            //ChungHung 20130910 add for SCK
extern void PushUnLoaderTrayInAverageTime(DWORD st, int pos);

// --- Auto-tray edge-push / vibration loops (golden asendic.h:24-25) ----------
extern void NewDoAutoTrayEdgeCylinderLoop();
extern bool DoAutoTrayEdgeCylinderLoop(int iAuto);

// --- belt feed in/out + under-conveyor cylinder move (golden asendic.h:28-33) -
extern void TrayMoveIn(bool bMove, int iAxis, int iPos=0);
extern void TrayMoveOut(bool bMove, int iAxis, int iPos=0);
extern void TrayCylinMoveIn(int iAxis);
extern void TrayCylinMoveOut(int iAxis);
// AI(W906-W7-L1-Wave0) 20260801: NO Sim SEAM IS BEING ADDED TO TrayMoveStatus,
// and that is a decision, not an oversight -- recorded here because the gap is
// invisible otherwise.  asendic.cpp's current body is a hardwired `return 0`, so
// every caller sees "idle" forever.  Two W7-L1 recons asked for a settable
// g_SimTrayMoveStatus / TrayMoveIn-Out call-count seam; both were declined
// because the Wave-3 (asendic.cpp L1b) landing replaces this whole family with
// REAL translated bodies (golden ~62 lines for TrayMoveStatus, ~177 for the four
// TrayMove*), so a seam added now would be written twice and could leave a dead
// override behind.  CONSEQUENCE the Wave-1/2 agents must record rather than
// paper over: while `return 0` stands, DoLoaderTrayToFront re-arms both of its
// timers on every case-100 pass, which makes the JAM1012 arm and the DUMMY
// early-out structurally unreachable.  Those arms are explicitly NOT COVERED
// until Wave 3 -- do not claim coverage for them, and do not add a seam here.
//
// AI(W906-W7-L1-Wave3) 20260802: THE NOTE ABOVE IS NOW HISTORY -- the prediction
// it makes came true and the gap it describes is CLOSED.  TrayMoveStatus is the
// real golden body (golden asendic.cpp:1449-1510): it reads SW[iInSwitch[iAxis]]
// / SW[iOutSwitch[iAxis]] (or, when LOAD_Y_USE_MOTOR[iAxis], the stepper's
// iInposLed + sign of ReadPos()) and returns 1 / 2 / 0, with the
// "Tray move status error" ShowMyMessage + forced-stop arm for the impossible
// combinations.  It can no longer return 0 unconditionally, so
// DoLoaderTrackDetectICFloating's JAM1012 arm and CheckLoaderICFloating's
// JAM09102 block are structurally REACHABLE.
// AI(W906-W7-L1-W3fixB) 20260802: CORRECTED CITATION -- this sentence used to end
// "...and are exercised by tests/test_w7_l1_wave3_traymove.cpp", a file that has
// never existed in this tree.  The coverage is real and lives in
// tests/test_w7_l1_loader.cpp SUB-TEST [19]
// (`test_wave3_unblocked_jam1012_and_jam09102`, ctest target W7_L1_Loader): it
// enables SW[SwACTrayY]/SW[SwACLoaderCCW] (iInSwitch[0]/iOutSwitch[0]), lets the SM
// drive them itself at case 300's TrayMoveOut, asserts the real TrayMoveStatus(0)
// then answers 2 -- a value the old hardwired `return 0` could not produce -- and
// raises JAM1012 with K_SKIP|K_RETRY once DetectLoaderTime expires.
// Still no Sim override seam, and still none needed: SW[] is drivable directly
// from a fixture.
extern int  TrayMoveStatus(int iAxis, AnsiString sFun);                         //Sam 20210325 : unified tray-move status

// --- Loader clean-out tray feed (golden asendic.h:36-37) ---------------------
// Golden declares these two WITHOUT `extern` -- kept verbatim.
void InitDoLoaderTrayFeedTask();                                                //kevin 20211110
bool DoLoaderTrayFeed();                                                        //kevin 20211110

// --- ART unloader one-/two-cylinder selector (golden asendic.h:38-39) --------
extern bool bARTUnloaderUseOneCylin(int iPart);                                 //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
extern bool bARTUnloaderUseTwoCylin(int iPart);                                 //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式

#endif
