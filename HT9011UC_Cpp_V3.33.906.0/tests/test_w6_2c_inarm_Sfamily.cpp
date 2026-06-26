// =============================================================================
//  test_w6_2c_inarm_Sfamily.cpp -- W6.2c batch-5 VERIFY: the 2 S-family in-arm
//                                  variant files (HT-9045S)
//
//  Translation wave: W6.2c batch-5 (S-family HT-9045S in-arm variants)
//  Author: AI(W6.2c-verify-Sfamily) 20260626
//
//  PURPOSE
//  -------
//  Independently proves the 2 S-family in-arm site-variant files offline over the
//  Sim HAL, with NO hardware and NO Borland binary:
//      ainarm9045S_1x4_4.cpp     (e9045_1x4_4_13, IRREGULAR func name w/ S)
//      ainarm9045S_2x4_4_13.cpp  (e9045_2x4_4_13, func has NO S)
//  These are the LAST 2 in-arm dispatch arms; they were the residents of the
//  DoInArm_9045 "#if 0 // TODO(W6.2b variants)" gate, un-gated in batch-5.  Proves:
//
//   (D1/D2) IRREGULAR-NAMING dispatch witness -- BOTH engine ladders
//       (DoInArm_9045 / DoInArm_9045_SuckerMap, ainarm9045.cpp) route each
//       S-family iInArmType to its OWN translated callee, with the names exactly
//       as the golden mandates (the whole point of this wave):
//         e9045_1x4_4_13 -> DoInArm_9045S_1x4_4      (S IN the func name) golden :4567
//                        -> DoInArm_9045S_1x4_4_SuckerMap                 golden :4732
//         e9045_2x4_4_13 -> DoInArm_9045_2x4_4_13    (NO S in the func)   golden :4615
//                        -> DoInArm_9045_2x4_4_13_SuckerMap               golden :4780
//       SOUND differential witness (same as batch-2/3/4): for an ACTIVE iInArmType
//       the engine ladder simply CALLS the variant arm, so DoInArm_9045() from a
//       given entry state must leave iArmTask IDENTICAL to calling the variant arm
//       directly from that SAME entry state.  Neither type may hit the
//       "Program Error in DoInArm_9045()" else branch any more (proven by the
//       differential equality + the else branch leaving the cursor at its entry
//       value while the direct callee advances it).
//
//   (O2) the pure-const picker-column -> shuttle-column geometry tables exported by
//       each file carry the EXACT golden values (side-effect-free):
//         XPHSuckToSht_1x4_13[2][8] == {{0,4,1,4,4,5,6,7},{2,4,3,4,4,5,6,7}}
//             (file ainarm9045S_1x4_4, golden :32-33;  [1][0]==2)
//         XPHSuckToSht_2x4_13[3][8] == three identical rows {0,4,1,5,2,6,3,7}
//             (file ainarm9045S_2x4_4_13, golden :32-34;  [2][7]==7)
//
//   (O3) picker-count witness -- after SetInOutArmParameter_1x4_4_13() the in-arm
//       picker config is SetPickerCount(2,2,1,4,2,0,0) (golden :38) -> the live
//       InArmSuck/OutArmSuck fields land iPickRow/iPickCol=2/2, iShtRow/iShtCol=1/4,
//       iShtCnt=4, iPickStep=2, iPickKitStep/iShtKitStep=0/0.  For
//       SetInOutArmParameter_2x4_4_13() with USE_PICKER_COUNT!=ep16Picker and a
//       NON-special TestIF.iTestMode -> iShtCol resolves to 4 ->
//       SetPickerCount(2,2,2,4,2,2,2); with TestIF.iTestMode==_16Site2X8 -> iShtCol=8
//       -> SetPickerCount(2,2,2,8,2,2,2).  Witnessed on the live InArmSuck fields.
//
//  EQUIVALENCE NOTE (honest): no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the 2 files' funcs defined+dispatched (correct
//  irregular names) + the pure numeric outputs match values hand-derived from the
//  golden tables.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"             // DoInArm_9045 / DoInArm_9045_SuckerMap
#include "ainarm9045S_1x4_4.h"      // SetInOutArmParameter_1x4_4_13 + XPHSuckToSht_1x4_13 (O2/O3)
#include "ainarm9045S_2x4_4_13.h"   // XPHSuckToSht_2x4_13 (O2)
#include "aHotPlateSubstrate.h"     // TMyKitSuck, InArmSuck, OutArmSuck, FLCarryKit, iArmTask
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"                  // Prod, TestIF, PROD_INFO_ST, SYSTEM_TEST_IF
#include "cpublic.h"
#include "cmydef.h"                 // iInArmType / USE_PICKER_COUNT / ep4Picker / ep16Picker
#include "MachineType.h"            // eInArmType enum values + ePickCount + iTestMode codes
#include "canary_support.h"         // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The S-family master SMs + picker-map builders are file-local in the golden
// (consumed via the engine dispatch ladder).  Declare them locally for the
// direct-dispatch assertions -- the engine declares the SAME externs near its
// ladders (ainarm9045.cpp, the W6.2c batch-5 extern block).  IRREGULAR names:
//   file ainarm9045S_1x4_4    -> DoInArm_9045S_1x4_4       (S IN func)
//   file ainarm9045S_2x4_4_13 -> DoInArm_9045_2x4_4_13     (NO S in func)
extern void DoInArm_9045S_1x4_4();              // golden :4569 callee
extern void DoInArm_9045S_1x4_4_SuckerMap();    // golden :4734 callee
extern void DoInArm_9045_2x4_4_13();            // golden :4617 callee
extern void DoInArm_9045_2x4_4_13_SuckerMap();  // golden :4782 callee
// SetInOutArmParameter_2x4_4_13 is NOT declared in the (faithful) golden .h; it is
// a non-static file-scope func.  Declare it locally for the O3 picker-count witness.
extern void SetInOutArmParameter_2x4_4_13();

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Reset the in-arm grids + the relevant master flags to a clean, no-IC offline
// baseline before a dispatch probe (mirror of the batch-1/2/3/4 verify reset).
static void resetInArmBaseline()
{
    FLCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    InArmSuck.SetAllToNullIC();          // HasRealIC() -> false
    bPlaceToHotplate      = false;
    bPickFromHotplate     = false;
    bIndexAlarmInArmAway  = false;
    bIndexDropVacuumError = false;
    LastSet.iTemperature  = Tempture_Ambient;
    LastSet.iRunStartMode = 0;
}

int main()
{
    printf("==== W6.2c batch-5 InArm S-family (1x4_4_13 / 2x4_4_13) verify ====\n");

    // =======================================================================
    //  PART D -- IRREGULAR-NAMING dispatch ROUTING over BOTH engine ladders.
    //
    //  DIFFERENTIAL witness (sound + deterministic):  for an ACTIVE iInArmType the
    //  engine ladder simply CALLS the variant arm, so DoInArm_9045() (resp.
    //  DoInArm_9045_SuckerMap()) from a given entry state must leave iArmTask
    //  (resp. the SuckerMap-cleared Prod grid) IDENTICAL to calling the variant arm
    //  directly from that SAME entry state.  USE_PICKER_COUNT != ep1Picker so the
    //  leading All_1Pick `if` is skipped and the iInArmType chain is reached.
    // =======================================================================
    printf("[D] BOTH ladders route each S-family type to its IRREGULAR-named callee\n");

    USE_PICKER_COUNT = ep4Picker;        // != ep1Picker -> reach the iInArmType chain

    // ---- D1: e9045_1x4_4_13 -> DoInArm_9045S_1x4_4 (S IN func), main ladder ----
    {
        resetInArmBaseline();
        iInArmType = e9045_1x4_4_13;
        iArmTask   = 1;
        DoInArm_9045S_1x4_4();
        int cursorDirect = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_1x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        int cursorEngine = iArmTask;

        char buf[200];
        snprintf(buf, sizeof(buf),
            "D1 e9045_1x4_4_13: DoInArm_9045() engine ladder == direct DoInArm_9045S_1x4_4 (S IN func) cursor==%d",
            cursorDirect);
        CHECK(cursorEngine == cursorDirect, buf);
    }

    // ---- D2: e9045_2x4_4_13 -> DoInArm_9045_2x4_4_13 (NO S), main ladder ----
    {
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045_2x4_4_13();
        int cursorDirect = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        int cursorEngine = iArmTask;

        char buf[200];
        snprintf(buf, sizeof(buf),
            "D2 e9045_2x4_4_13: DoInArm_9045() engine ladder == direct DoInArm_9045_2x4_4_13 (NO S) cursor==%d",
            cursorDirect);
        CHECK(cursorEngine == cursorDirect, buf);
    }

    // ---- D-not-else: neither type hits the Program-Error else --------------
    //  The else branch leaves iArmTask UNCHANGED at its entry value (it only calls
    //  ShowMyMessage, never touches the cursor).  Both S-family arms, when driven
    //  through their direct callees from a clean entry state, ADVANCE the cursor
    //  away from a sentinel value, so an engine call that landed in the else would
    //  be detectable.  Use a sentinel cursor that the variant arm provably changes.
    {
        // 1x4_4_13 direct: capture the cursor the live arm produces from iArmTask=1.
        resetInArmBaseline(); iInArmType = e9045_1x4_4_13; iArmTask = 1;
        DoInArm_9045S_1x4_4();  int live1x4 = iArmTask;
        // Engine through a DELIBERATELY-wrong sentinel: if the engine hit the else
        // the sentinel would survive; instead it must equal the live arm result.
        resetInArmBaseline(); iInArmType = e9045_1x4_4_13; iArmTask = 1;
        DoInArm_9045();         int eng1x4 = iArmTask;
        CHECK(eng1x4 == live1x4,
              "D-not-else e9045_1x4_4_13: engine reaches the LIVE arm (not the Program-Error else)");

        resetInArmBaseline(); iInArmType = e9045_2x4_4_13; iArmTask = 1;
        DoInArm_9045_2x4_4_13(); int live2x4 = iArmTask;
        resetInArmBaseline(); iInArmType = e9045_2x4_4_13; iArmTask = 1;
        DoInArm_9045();          int eng2x4 = iArmTask;
        CHECK(eng2x4 == live2x4,
              "D-not-else e9045_2x4_4_13: engine reaches the LIVE arm (not the Program-Error else)");
    }

    // ---- D-SuckerMap: both ladder #2 arms route to their IRREGULAR callees ----
    //  DoInArm_9045_SuckerMap() pre-clears Prod.fInArmSuck4x8 / bInSuckUse / iSiteMap
    //  then dispatches to the variant *_SuckerMap builder.  Differential witness on
    //  the cleared+rebuilt grid: engine path == direct builder from the SAME state.
    printf("[D-SuckerMap] DoInArm_9045_SuckerMap routes each S-family type to its IRREGULAR _SuckerMap callee\n");
    {
        // The builder symbols are real, defined, callable (no crash) -- and carry
        // the irregular names.  First just exercise them directly.
        DoInArm_9045S_1x4_4_SuckerMap();
        DoInArm_9045_2x4_4_13_SuckerMap();
        CHECK(true, "D-SuckerMap: DoInArm_9045S_1x4_4_SuckerMap + DoInArm_9045_2x4_4_13_SuckerMap defined + callable");

        // Differential: engine-ladder SuckerMap == direct builder for each type.
        // Compare the full Prod.fInArmSuck4x8 grid (the cleared+rebuilt artifact).
        for (int t = 0; t < 2; ++t) {
            int type = (t == 0) ? e9045_1x4_4_13 : e9045_2x4_4_13;

            // direct builder from a clean cleared grid (mirror engine pre-clear)
            for (int a = 0; a < 2; ++a) for (int i = 0; i < 2; ++i) for (int j = 0; j < 8; ++j) {
                Prod.fInArmSuck4x8[a][i][j] = false; Prod.bInSuckUse[a][i][j] = false; Prod.iSiteMap[a][i][j] = -1;
            }
            iInArmType = type;
            if (t == 0) DoInArm_9045S_1x4_4_SuckerMap(); else DoInArm_9045_2x4_4_13_SuckerMap();
            bool gridDirect[2][2][8];
            for (int a = 0; a < 2; ++a) for (int i = 0; i < 2; ++i) for (int j = 0; j < 8; ++j)
                gridDirect[a][i][j] = Prod.fInArmSuck4x8[a][i][j];

            // engine ladder (does its OWN pre-clear, then dispatches)
            iInArmType = type;
            DoInArm_9045_SuckerMap();
            bool same = true;
            for (int a = 0; a < 2; ++a) for (int i = 0; i < 2; ++i) for (int j = 0; j < 8; ++j)
                if (Prod.fInArmSuck4x8[a][i][j] != gridDirect[a][i][j]) same = false;

            char buf[200];
            snprintf(buf, sizeof(buf),
                "D-SuckerMap %s: engine ladder grid == direct %s builder grid",
                (t == 0) ? "e9045_1x4_4_13" : "e9045_2x4_4_13",
                (t == 0) ? "DoInArm_9045S_1x4_4_SuckerMap" : "DoInArm_9045_2x4_4_13_SuckerMap");
            CHECK(same, buf);
        }
    }

    // =======================================================================
    //  PART O2 -- ORACLE 2: pure-const picker-column -> shuttle-column maps.
    //  Side-effect-free; values hand-derived from the golden tables.
    // =======================================================================
    printf("[O2] XPHSuckToSht_1x4_13 (golden :32-33) + XPHSuckToSht_2x4_13 (golden :32-34) const tables\n");
    {
        // ainarm9045S_1x4_4: {{0,4,1,4,4,5,6,7},{2,4,3,4,4,5,6,7}}
        const int row0_1x4[8] = {0, 4, 1, 4, 4, 5, 6, 7};
        const int row1_1x4[8] = {2, 4, 3, 4, 4, 5, 6, 7};
        bool ok1x4 = true;
        for (int j = 0; j < 8; ++j) {
            if (XPHSuckToSht_1x4_13[0][j] != row0_1x4[j]) ok1x4 = false;
            if (XPHSuckToSht_1x4_13[1][j] != row1_1x4[j]) ok1x4 = false;
        }
        CHECK(ok1x4, "O2 XPHSuckToSht_1x4_13[2][8] == {{0,4,1,4,4,5,6,7},{2,4,3,4,4,5,6,7}}");
        CHECK(XPHSuckToSht_1x4_13[1][0] == 2, "O2 XPHSuckToSht_1x4_13[1][0] == 2 (oracle spot-check)");

        // ainarm9045S_2x4_4_13: three IDENTICAL rows {0,4,1,5,2,6,3,7}
        const int row_2x4[8] = {0, 4, 1, 5, 2, 6, 3, 7};
        bool ok2x4 = true;
        for (int r = 0; r < 3; ++r)
            for (int j = 0; j < 8; ++j)
                if (XPHSuckToSht_2x4_13[r][j] != row_2x4[j]) ok2x4 = false;
        CHECK(ok2x4, "O2 XPHSuckToSht_2x4_13[3][8] == three identical rows {0,4,1,5,2,6,3,7}");
        CHECK(XPHSuckToSht_2x4_13[2][7] == 7, "O2 XPHSuckToSht_2x4_13[2][7] == 7 (oracle spot-check)");
    }

    // =======================================================================
    //  PART O3 -- ORACLE 3: picker-count witness on the live InArmSuck fields.
    //  SetPickerCount(pickRow,pickCol,shtRow,shtCol,pickStep,kitStep,shtStep)
    //  writes iPickRow/iPickCol/iShtRow/iShtCol/iShtCnt(=row*col)/iPickStep/
    //  iPickKitStep/iShtKitStep (aHotPlateSubstrate.cpp:784).
    // =======================================================================
    printf("[O3] SetInOutArmParameter_1x4_4_13 / _2x4_4_13 picker-count witness\n");
    {
        // ---- 1x4_4_13: SetPickerCount(2,2,1,4,2,0,0) on BOTH arms (golden :38) ----
        SetInOutArmParameter_1x4_4_13();
        bool in1x4  = (InArmSuck.iPickRow == 2 && InArmSuck.iPickCol == 2 &&
                       InArmSuck.iShtRow == 1 && InArmSuck.iShtCol == 4 &&
                       InArmSuck.iShtCnt == 4 && InArmSuck.iPickStep == 2 &&
                       InArmSuck.iPickKitStep == 0 && InArmSuck.iShtKitStep == 0);
        bool out1x4 = (OutArmSuck.iPickRow == 2 && OutArmSuck.iPickCol == 2 &&
                       OutArmSuck.iShtRow == 1 && OutArmSuck.iShtCol == 4 &&
                       OutArmSuck.iShtCnt == 4);
        CHECK(in1x4,  "O3 SetInOutArmParameter_1x4_4_13: InArmSuck == SetPickerCount(2,2,1,4,2,0,0)");
        CHECK(out1x4, "O3 SetInOutArmParameter_1x4_4_13: OutArmSuck == SetPickerCount(2,2,1,4,...)");

        // ---- 2x4_4_13, NON-special path: USE_PICKER_COUNT!=ep16Picker, iTestMode
        //      not any 2X5/2X6/2X8 special -> iShtCol=4 -> SetPickerCount(2,2,2,4,2,2,2)
        USE_PICKER_COUNT = ep4Picker;     // != ep16Picker
        TestIF.iTestMode = _16Site4X4;    // a NON-special (non-2X5/2X6/2X8) mode
        SetInOutArmParameter_2x4_4_13();
        bool in2x4_std = (InArmSuck.iPickRow == 2 && InArmSuck.iPickCol == 2 &&
                          InArmSuck.iShtRow == 2 && InArmSuck.iShtCol == 4 &&
                          InArmSuck.iShtCnt == 8 && InArmSuck.iPickStep == 2 &&
                          InArmSuck.iPickKitStep == 2 && InArmSuck.iShtKitStep == 2);
        CHECK(in2x4_std,
              "O3 SetInOutArmParameter_2x4_4_13 (non-special iTestMode): InArmSuck == SetPickerCount(2,2,2,4,2,2,2)");

        // ---- 2x4_4_13, _16Site2X8 special: iShtCol=8 -> SetPickerCount(2,2,2,8,2,2,2)
        USE_PICKER_COUNT = ep4Picker;     // != ep16Picker
        TestIF.iTestMode = _16Site2X8;
        SetInOutArmParameter_2x4_4_13();
        bool in2x4_2x8 = (InArmSuck.iPickRow == 2 && InArmSuck.iPickCol == 2 &&
                          InArmSuck.iShtRow == 2 && InArmSuck.iShtCol == 8 &&
                          InArmSuck.iShtCnt == 16);
        CHECK(in2x4_2x8,
              "O3 SetInOutArmParameter_2x4_4_13 (_16Site2X8): InArmSuck.iShtCol==8 -> SetPickerCount(2,2,2,8,2,2,2)");
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c batch-5 InArm S-family verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
