// tests/test_SCK_ART.cpp
// Verification harness for the Automation/SCK_ART.h/.cpp 8-function extract (W5-SckArt-Translate).
//
// Exercises the translated public API against input->expected-output values hand-derived from the
// BCB6 formula in the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp:639-4391 (see SCK_ART.h for the exact
//   per-function line ranges).
//
// LIMITATION (same as test_ContactForce.cpp/test_cContact.cpp): we CANNOT run the original BCB6
// binary (no Borland compiler in this environment). Verification here is therefore:
//   (1) the translation compiles+links clean against the real shared globals (cprod.h/Config.h/
//       CosFunction.h/cmydef.h/canary_support.h/aHotPlateSubstrate.h/FormsFacade.h), AND
//   (2) its outputs equal values hand-computed / transcribed from the BCB6 source (golden file:line
//       cited per case), driving those SAME real shared globals to the documented golden values.
//
// Each PART sets every global field the function-under-test reads to an EXPLICIT value (no reliance
// on ambient/leftover state from an earlier PART), matching this project's test_w6_4b_front_rear_
// destroy.cpp convention of driving shared globals directly with save/restore where a later PART
// would otherwise leak state into an earlier assertion.
//
// No external test framework: a tiny check harness prints PASS/FAIL per case and a final summary,
// and returns non-zero on ANY failure.

#include "Automation/SCK_ART.h"

#include "MachineDefine.h"
#include "MachineType.h"
#include "cprod.h"
#include "cmydef.h"
#include "canary_support.h"
#include "aHotPlateSubstrate.h"
#include "FormsFacade.h"

#include <cmath>
#include <cstdio>

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { std::printf("PASS  %s\n", msg); ++g_pass; }                 \
        else      { std::printf("FAIL  %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

static bool feq(double a, double b, double eps = 1e-9)
{
    return std::fabs(a - b) <= eps * (1.0 + std::fabs(b));
}

int main()
{
    printf("=== Automation/SCK_ART.cpp (8-function extract) translation verification ===\n");
    printf("(values hand-derived/transcribed from golden Automation/SCK_ART.cpp; original binary NOT run)\n\n");

    // =========================================================================================
    // PART 1 -- SetLotStatus / GetLotStatus -- golden :639-672
    // =========================================================================================
    printf("-- SetLotStatus / GetLotStatus --\n");
    {
        SckArtState st;
        int comboIdx = -999;

        SckArt_SetLotStatus(st, 1, &comboIdx);
        CHECK(st.sLOTSTATUS == "LOTSTATUS_W", "iStatus=1 -> sLOTSTATUS=LOTSTATUS_W (golden :644)");
        CHECK(st.iCurrentStatus == 1,         "iStatus=1 -> iCurrentStatus=1 (golden :665)");
        CHECK(comboIdx == 1,                  "iStatus=1 -> outComboItemIndex=1 (extract-calc-core of golden :664 cbLotStatus->ItemIndex)");
        CHECK(SckArt_GetLotStatus(st) == "LOTSTATUS_W", "GetLotStatus returns sLOTSTATUS (golden :671)");

        SckArt_SetLotStatus(st, 4, &comboIdx);
        CHECK(st.sLOTSTATUS == "LOTSTATUS_R", "iStatus=4 -> sLOTSTATUS=LOTSTATUS_R (golden :653)");
        CHECK(st.iCurrentStatus == 4,         "iStatus=4 -> iCurrentStatus=4");

        SckArt_SetLotStatus(st, 6, &comboIdx);
        CHECK(st.sLOTSTATUS == "LOTSTATUS_A", "iStatus=6 -> sLOTSTATUS=LOTSTATUS_A (golden :659)");

        SckArt_SetLotStatus(st, 99, &comboIdx);
        CHECK(st.sLOTSTATUS == "NONE",        "iStatus=99 (unmapped) -> sLOTSTATUS=NONE (golden default: :662)");
        CHECK(st.iCurrentStatus == 99,        "iCurrentStatus still tracks the raw iStatus even for the default label (golden :665, unconditional)");

        // outComboItemIndex is optional (default 0 / nullptr) -- must not crash when omitted.
        SckArt_SetLotStatus(st, 2);
        CHECK(st.sLOTSTATUS == "LOTSTATUS_T", "iStatus=2, no out-param -> still updates sLOTSTATUS (golden :647)");
    }

    // =========================================================================================
    // PART 2 -- CheckLoadingCount -- golden :930-993 (0:not enough, 1:same, 2:over count, -1:neither mode)
    // =========================================================================================
    printf("\n-- CheckLoadingCount --\n");
    {
        // Reset every global field this function reads (explicit, not relying on link-time zero-init).
        CosFunction.bUseSCKART = true;
        USE_AUTO_RETEST = eartInstall;
        IniConfig.bA10_AutoReTest = true;
        TestIF_File.bSCKART_EnableART = true;
        IniConfig.bA65_BundleIDList = false;
        CUSTOMER_CODE = 0;                       // not CC_PTI -- skip the PTI early-return branch
        IniConfig.bB03_TesterReport = false;
        TestIF_File.iSCKART_TryCnt = 5;
        TestIF_File.bRENESAS_EnableFTCT = false;
        IniConfig.bP57LoaderAutoCleanOutByInputCT = false;
        LastSet.iP57_InputCT = 0;

        SckArtState st;
        st.iFTRTCount = 1;
        st.iInputCount = 10;

        LastSet.iSCKARTInputCT = 5;
        CHECK(SckArt_CheckLoadingCount(st) == 0, "loadingCount(5) < inputCount(10) -> 0:not enough (golden :951-954)");
        CHECK(st.bLdCntExdInputCnt == false,     "bLdCntExdInputCnt cleared on the not-enough branch (golden :952)");

        LastSet.iSCKARTInputCT = 10;
        CHECK(SckArt_CheckLoadingCount(st) == 1, "loadingCount(10) == inputCount(10) -> 1:same (golden :955-959)");

        LastSet.iSCKARTInputCT = 15;
        TestIF_File.bRENESAS_EnableFTCT = false;
        CHECK(SckArt_CheckLoadingCount(st) == 2, "loadingCount(15) > inputCount(10), FTCT off -> 2:over count (golden :969-972)");

        TestIF_File.bRENESAS_EnableFTCT = true;
        CHECK(SckArt_CheckLoadingCount(st) == 1, "loadingCount(15) > inputCount(10), FTCT on -> 1 + bLdCntExdInputCnt=true (golden :962-968)");
        CHECK(st.bLdCntExdInputCnt == true,      "bLdCntExdInputCnt set when RENESAS FTCT excess path taken (golden :966)");
        TestIF_File.bRENESAS_EnableFTCT = false;

        // PTI early-return branch (golden :938-942).
        CUSTOMER_CODE = CC_PTI;
        IniConfig.bB03_TesterReport = true;
        st.iFTRTCount = 0;
        CHECK(SckArt_CheckLoadingCount(st) == 0, "CC_PTI + bB03_TesterReport + iFTRTCount==0 -> early return 0 (golden :940-941)");
        st.iFTRTCount = 10;
        TestIF_File.iSCKART_TryCnt = 3;
        CHECK(SckArt_CheckLoadingCount(st) == 0, "CC_PTI + TryCnt(3) < iFTRTCount(10) -> early return 0 (golden :940-941)");
        CUSTOMER_CODE = 0;
        IniConfig.bB03_TesterReport = false;
        TestIF_File.iSCKART_TryCnt = 5;

        // P57 Loader-count AutoCleanOut branch (golden :975-991) -- only reached when the whole
        // outer SCKART condition is false.
        CosFunction.bUseSCKART = false;
        IniConfig.bA65_BundleIDList = false;
        IniConfig.bP57LoaderAutoCleanOutByInputCT = true;
        LastSet.iP57_InputCT = 20;
        LastSet.SendCT[0] = 15;
        CHECK(SckArt_CheckLoadingCount(st) == 0, "P57 mode: SendCT[0](15) < iP57_InputCT(20) -> 0 (golden :979-981)");
        LastSet.SendCT[0] = 20;
        CHECK(SckArt_CheckLoadingCount(st) == 1, "P57 mode: SendCT[0](20) == iP57_InputCT(20) -> 1 (golden :983-986)");
        LastSet.SendCT[0] = 25;
        CHECK(SckArt_CheckLoadingCount(st) == 2, "P57 mode: SendCT[0](25) > iP57_InputCT(20) -> 2 (golden :988-990)");

        // Neither mode applies -> golden's own explicit trailing `return -1;` (golden :992).
        IniConfig.bP57LoaderAutoCleanOutByInputCT = false;
        CHECK(SckArt_CheckLoadingCount(st) == -1, "neither SCKART nor P57 mode active -> -1 (golden :992, verbatim)");
    }

    // =========================================================================================
    // PART 3 -- CheckInArmNeedVariModeFIX -- golden :995-1029
    // =========================================================================================
    printf("\n-- CheckInArmNeedVariModeFIX --\n");
    {
        CosFunction.bUseSCKART = true;
        USE_AUTO_RETEST = eartInstall;
        IniConfig.bA10_AutoReTest = true;
        TestIF_File.bSCKART_EnableART = true;
        TestIF_File.bRENESAS_EnableFTCT = false;
        TestIF_File.bSCKART_LotDeviceCheck = true;
        IniConfig.bP57LoaderAutoCleanOutByInputCT = false;

        SckArtState st;
        st.iInputCount = 100;
        st.bBackUpInArmMode = false;
        ArmSpeed[InArm].bVariModeFIX = false;

        // iDevice==20 (golden :997). loadingCount(85) <= inputCount(100) and
        // loadingCount(85) > inputCount(100)-iDevice(20)=80 -> Fix mode ON.
        LastSet.iSCKARTInputCT = 85;
        SckArt_CheckInArmNeedVariModeFIX(st);
        CHECK(ArmSpeed[InArm].bVariModeFIX == true, "loadingCount(85) close to inputCount(100)-20=80 -> InArm Fix mode ON (golden :1010-1011)");

        // loadingCount(50) <= inputCount(100)-20=80 far away -> NOT fix, bBackUpInArmMode=false.
        ArmSpeed[InArm].bVariModeFIX = true;   // pre-dirty to prove the else path actually runs
        st.bBackUpInArmMode = true;            // pre-dirty sentinel
        LastSet.iSCKARTInputCT = 50;
        SckArt_CheckInArmNeedVariModeFIX(st);
        CHECK(st.bBackUpInArmMode == false, "loadingCount(50) far from the 80 threshold -> bBackUpInArmMode cleared (golden :1012-1013)");

        // P57 branch (golden :1017-1028), only reached when the outer SCKART condition is false.
        // AI(integrate) 20260710: fixed a test bug -- the P57 branch's iLoadingCount comes from
        // LastSet.SendCT[0] (golden :1020), NOT LastSet.iSCKARTInputCT (that field only feeds the
        // OTHER (SCKART) branch, golden :1005) -- the original assertion here left SendCT[0] at its
        // stale value (25, from the earlier CheckLoadingCount PART) and so failed; SckArt_
        // CheckInArmNeedVariModeFIX itself was already a byte-exact match of golden.
        CosFunction.bUseSCKART = false;
        IniConfig.bP57LoaderAutoCleanOutByInputCT = true;
        LastSet.iP57_InputCT = 100;
        ArmSpeed[InArm].bVariModeFIX = false;
        LastSet.SendCT[0] = 85;   // iDevice==20 here too (golden :1017); this IS the field golden :1020 reads
        SckArt_CheckInArmNeedVariModeFIX(st);
        CHECK(ArmSpeed[InArm].bVariModeFIX == true, "P57 mode: loadingCount(85) close to iP57_InputCT(100)-20=80 -> Fix ON (golden :1023-1024)");
    }

    // =========================================================================================
    // PART 4 -- CheckOutArmNeedVariModeFIX -- golden :1031-1070
    // =========================================================================================
    printf("\n-- CheckOutArmNeedVariModeFIX --\n");
    {
        CosFunction.bUseSCKART = true;
        USE_AUTO_RETEST = eartInstall;
        IniConfig.bA10_AutoReTest = true;
        TestIF_File.bSCKART_EnableART = true;
        TestIF_File.bSCKART_LotDeviceCheck = true;
        CUSTOMER_CODE = 0;   // non-TERAPOWER -> iDevice==30 (golden :1040)

        SckArtState st;
        st.iInputCount = 100;
        Prod.iIsFailT6[0] = 1;
        LastSet.iSCKARTInputCT = 50;

        SckArt_CheckOutArmNeedVariModeFIX(st, 0);
        // NOTE: LastSet.lSCKARTBinCT[] is a TU-local zero-initialized stand-in this wave (gate #3,
        // golden LastSet.h:507 not yet in the canary_support.h shim) -- iTotalUnloadCount is always 0
        // offline, documented in SCK_ART.cpp's gate comments.
        CHECK(ArmSpeed[OutArm].bVariModeFIX == true, "Prod.iIsFailT6[0]==1 -> OutArm Fix mode forced ON (golden :1053-1054)");

        Prod.iIsFailT6[0] = 0;
        ArmSpeed[OutArm].bVariModeFIX = true;   // pre-dirty
        // inputCount(100) - iTotalUnloadCount(0, gated) = 100 > iDevice(30) -> NOT fix -> bBackUpOutArmMode cleared.
        st.bBackUpOutArmMode = true;            // pre-dirty sentinel
        SckArt_CheckOutArmNeedVariModeFIX(st, 0);
        CHECK(ArmSpeed[OutArm].bVariModeFIX == false, "iIsFailT6==0 path re-evaluates bVariModeFIX=false first (golden :1055-1056)");
        CHECK(st.bBackUpOutArmMode == false, "inputCount(100)-0 > iDevice(30) -> bBackUpOutArmMode cleared (golden :1065-1066)");

        // TERAPOWER special-case (golden :1035-1037): iDevice stays its initial 20 -- NOT
        // reassigned to 30 -- a faithful golden quirk: TERAPOWER's `if` body is EMPTY.
        // AI(integrate) 20260710: fixed a test bug -- the outer Fix-decision block is only entered
        // when `st.iInputCount>=iLoadingCount` (golden :1059, iLoadingCount==LastSet.iSCKARTInputCT);
        // the original assertion left LastSet.iSCKARTInputCT at its stale value (50, from line ~210
        // above) so 15>=50 was false and the whole branch (hence bVariModeFIX) never ran -- the
        // production function was already a byte-exact match of golden.
        CUSTOMER_CODE = CC_TERAPOWER;
        st.iInputCount = 15;   // inputCount(15)-0=15 <= iDevice(20, TERAPOWER's untouched initial value) -> Fix ON
        LastSet.iSCKARTInputCT = 10;   // <= st.iInputCount(15) so the Fix-decision block is entered (golden :1059)
        SckArt_CheckOutArmNeedVariModeFIX(st, 0);
        CHECK(ArmSpeed[OutArm].bVariModeFIX == true, "CC_TERAPOWER: iDevice stays the un-reassigned initial 20 (golden :1035-1037 empty if-body, faithful quirk) -> 15<=20 -> Fix ON");
        CUSTOMER_CODE = 0;
    }

    // =========================================================================================
    // PART 5 -- CheckNeedRT -- golden :1072-1230
    //   fMain->hanaART->IsHanaArtAvailable() is hardwired false offline (FormsFacade.cpp), and
    //   CosFunction.iAutoRetestTCPmode!=2 by default, so the reachable branch offline is the golden
    //   `else` ladder (golden :1122-1162). ShowErrorMessage's Sim body always returns K_RETRY
    //   (canary_support.cpp), so the `iRet==K_TRAY_FEED` branch never fires offline -- documented,
    //   not a gap in this translation.
    //
    //   The golden function has TWO independent tail blocks gated on the SAME flag with OPPOSITE
    //   polarity: `if(bRENESAS_EnableFTCT==false)` sets iCurrent93KARTStep (golden :1164-1170), while
    //   the LATER `if(iNeedRT!=0){ if(bRENESAS_EnableFTCT==true){...} else{ if(RTUnitCount==0)
    //   iNeedRT=0; ...} }` (golden :1172-1223) only PRESERVES the ladder's iNeedRT decision when
    //   bRENESAS_EnableFTCT==TRUE (this wave's gate #3 stand-in for LastSet.iSCKART_RTUnitCount is
    //   always 0, so the ==false sub-branch always force-resets iNeedRT to 0). So: 5A below uses
    //   bRENESAS_EnableFTCT==false to exercise iCurrent93KARTStep (and documents the resulting
    //   iNeedRT==0 collapse); 5B/5C use bRENESAS_EnableFTCT==true to observe the ladder's iNeedRT
    //   decision itself (iCurrent93KARTStep is untouched in that mode, by golden design).
    // =========================================================================================
    printf("\n-- CheckNeedRT --\n");
    CUSTOMER_CODE = 0;                     // non-TERAPOWER -> iDevice==20 (golden :1078-1079)
    CosFunction.iAutoRetestTCPmode = 0;     // != 2 -> skip the RENESAS-TCP branch (golden :1112)
    TestIF_File.bLowYieldForArtFT = false;  // skip ShowErrorMessage call entirely
    TestIF_File.iSCKART_TryCnt = 3;
    TestIF_File.dSCKART_Yield = 90.0;

    // ---- 5A: bRENESAS_EnableFTCT==false -- iCurrent93KARTStep + the documented iNeedRT collapse --
    {
        TestIF_File.bRENESAS_EnableFTCT = false;
        SckArtState st;
        st.iFTRTCount = 0; st.dCurrYield = 50.0; st.iNeedRT = 0;

        // call 1: iFTRTCount 0->1; 1<TryCnt(3) && yield(50)<target(90) -> ladder sets iNeedRT=1
        // (golden :1148-1152); iFTRTCount==1 && iNeedRT==1 -> iCurrent93KARTStep=5 (golden :1166-
        // 1167) BEFORE the tail; the tail then force-resets iNeedRT to 0 (gate #3 stand-in).
        SckArt_CheckNeedRT(st);
        CHECK(st.iFTRTCount == 1,        "5A call1: iFTRTCount 0->1 (golden :1124)");
        CHECK(st.iCurrent93KARTStep == 5, "5A call1: iFTRTCount==1 && ladder-iNeedRT==1 -> iCurrent93KARTStep=5 (golden :1166-1167)");
        CHECK(st.iNeedRT == 0,           "5A call1: tail force-resets iNeedRT to 0 (gate #3: LastSet.iSCKART_RTUnitCount stand-in always 0, golden :1172-1186 else-branch)");

        // call 2: iFTRTCount 1->2; still <TryCnt(3), yield(50)<target -> ladder sets iNeedRT=1 again;
        // iFTRTCount!=1 this time -> iCurrent93KARTStep=10 (golden :1168-1169); tail resets again.
        SckArt_CheckNeedRT(st);
        CHECK(st.iFTRTCount == 2,         "5A call2: iFTRTCount 1->2");
        CHECK(st.iCurrent93KARTStep == 10, "5A call2: iFTRTCount!=1 && ladder-iNeedRT==1 -> iCurrent93KARTStep=10 (golden :1168-1169)");
        CHECK(st.iNeedRT == 0,            "5A call2: tail force-resets iNeedRT to 0 again (same gate #3 reasoning)");
    }

    // ---- 5B: bRENESAS_EnableFTCT==true -- the ladder's iNeedRT decision is PRESERVED through the
    //          tail (golden :1172-1173 bRENESAS_EnableFTCT==true sub-branch does not touch iNeedRT).
    {
        TestIF_File.bRENESAS_EnableFTCT = true;

        // b1: iFTRTCount reaches TryCnt exactly -> Final ART (golden :1144-1147), takes priority
        // over the yield-based branches in the else-if chain.
        SckArtState st;
        st.iFTRTCount = 2; st.dCurrYield = 50.0; st.iNeedRT = 0;   // ++ -> 3 == TryCnt(3)
        SckArt_CheckNeedRT(st);
        CHECK(st.iFTRTCount == 3, "5B-b1: iFTRTCount 2->3 == TryCnt(3)");
        CHECK(st.iNeedRT == 2,    "5B-b1: iFTRTCount==TryCnt -> Final ART, iNeedRT=2, preserved through the tail (golden :1144-1147)");

        // b2: iFTRTCount exceeds TryCnt, non-TERAPOWER -> iNeedRT=0 (golden :1137-1142).
        SckArtState st2;
        st2.iFTRTCount = 3; st2.dCurrYield = 50.0; st2.iNeedRT = 0;   // ++ -> 4 > TryCnt(3)
        SckArt_CheckNeedRT(st2);
        CHECK(st2.iFTRTCount == 4, "5B-b2: iFTRTCount 3->4 > TryCnt(3)");
        CHECK(st2.iNeedRT == 0,    "5B-b2: non-TERAPOWER, count>TryCnt -> iNeedRT=0 (golden :1137-1142)");

        // b3: same as b2 but CC_TERAPOWER -> iNeedRT stays 2 instead of resetting to 0 (golden
        // :1139-1140, "Final RT 後還會++ iNeedRT 狀態還需要維持2").
        CUSTOMER_CODE = CC_TERAPOWER;
        SckArtState st3;
        st3.iFTRTCount = 3; st3.dCurrYield = 50.0; st3.iNeedRT = 2;   // ++ -> 4 > TryCnt(3)
        SckArt_CheckNeedRT(st3);
        CHECK(st3.iNeedRT == 2, "5B-b3: CC_TERAPOWER, count(4)>TryCnt(3) -> iNeedRT stays 2, not reset to 0 (golden :1139-1140)");
        CUSTOMER_CODE = 0;

        // b4: yield>=target && iNeedRT!=2 -> Final ART (golden :1153-1157), reached only when the
        // >-TryCnt / ==-TryCnt / <-TryCnt-and-below-yield branches all fail to fire first.
        SckArtState st4;
        st4.iFTRTCount = 0; st4.dCurrYield = 95.0; st4.iNeedRT = 0;   // ++ -> 1 < TryCnt(3), yield>=90
        SckArt_CheckNeedRT(st4);
        CHECK(st4.iNeedRT == 2, "5B-b4: yield(95)>=target(90) && iNeedRT(0)!=2 -> Final ART, iNeedRT=2 (golden :1153-1157)");
    }

    // ---- 5C: ArmSpeed fix-mode selection in the iNeedRT!=0 tail (golden :1210-1222). With
    //          bRENESAS_EnableFTCT==true this wave's gate #3 stand-in for LastSet.iSCKART_RTUnitCount
    //          is always 0, so the common tail's `RTUnitCount<iDevice && RTUnitCount!=0` guard is
    //          always FALSE (0!=0) -> always takes the bBackUpInArmMode/bBackUpOutArmMode restore
    //          branch (golden :1219-1221), never the force-Fix branch -- documented gate-shaped
    //          behaviour, not a bug (a real LastSet.iSCKART_RTUnitCount!=0 would flip this).
    {
        TestIF_File.bRENESAS_EnableFTCT = true;
        SckArtState st;
        st.iFTRTCount = 2;   // ++ -> 3 == TryCnt(3) -> iNeedRT=2 (independent of yield, golden :1144-1147)
        st.bBackUpInArmMode = true;
        st.bBackUpOutArmMode = true;
        ArmSpeed[InArm].bVariModeFIX  = false;   // pre-dirty (opposite of expected) to prove the write happens
        ArmSpeed[OutArm].bVariModeFIX = false;
        CUSTOMER_CODE = CC_TERAPOWER;            // needed for the OutArm restore line to also run (golden :1220-1221)
        SckArt_CheckNeedRT(st);
        CHECK(ArmSpeed[InArm].bVariModeFIX == true,  "gate #3 stand-in (RTUnitCount==0) -> ArmSpeed[InArm] takes bBackUpInArmMode (golden :1219-1220)");
        CHECK(ArmSpeed[OutArm].bVariModeFIX == true, "CC_TERAPOWER -> ArmSpeed[OutArm] ALSO takes bBackUpOutArmMode (golden :1220-1221)");
        CUSTOMER_CODE = 0;
    }

    // =========================================================================================
    // PART 6 -- DoAutoSocketOff -- golden :1232-1359
    // =========================================================================================
    printf("\n-- DoAutoSocketOff --\n");
    {
        // TestSocket.iShtRow==2 / iShtCol==1 by ctor default (aHotPlateSubstrate.cpp:76-77) -- kept
        // as-is (small, real default; not overridden) so the loop bounds below are (i<2, j<1).
        SckArtState st;

        // Outer gate off -> whole function a no-op regardless of any other field.
        TestIF_File.bSCKART_AutoSocketOff = false;
        LastSet.bUseTestSocket[0][0][0] = true;
        SckArt_DoAutoSocketOff(st, false);
        CHECK(LastSet.bUseTestSocket[0][0][0] == true, "bSCKART_AutoSocketOff==false -> function is a no-op (golden :1238 outer gate)");

        // "Final RT after, re-open all" branch (golden :1330-1357): bAllSiteOn==true takes the else
        // branch regardless of iFTRTCount. bI35UseThirdSiteControlByEngineer==false -> direct enable
        // wherever TestIF_File.iSiteMap[i][j]!=0.
        TestIF_File.bSCKART_AutoSocketOff = true;
        IniConfig.bI35UseThirdSiteControlByEngineer = false;
        TestIF_File.iSiteMap[0][0] = 1;   // used site
        TestIF_File.iSiteMap[1][0] = 0;   // unused site
        LastSet.bUseTestSocket[0][0][0] = false;
        LastSet.bUseTestSocket[1][0][0] = false;
        LastSet.bUseTestSocket[0][1][0] = false;
        LastSet.bUseTestSocket[1][1][0] = false;
        st.iFTRTCount = 0;   // irrelevant to this branch since bAllSiteOn==true
        SckArt_DoAutoSocketOff(st, true);
        CHECK(LastSet.bUseTestSocket[0][0][0] == true,  "re-open: iSiteMap[0][0]!=0 -> bUseTestSocket[0][0][0]=true (golden :1349-1350)");
        CHECK(LastSet.bUseTestSocket[1][0][0] == true,  "re-open: iSiteMap[0][0]!=0 -> bUseTestSocket[1][0][0]=true (golden :1350-1351)");
        CHECK(LastSet.bUseTestSocket[0][1][0] == false, "re-open: iSiteMap[1][0]==0 -> row 1 untouched, stays false (golden :1348 guard)");

        // Same re-open branch but bI35UseThirdSiteControlByEngineer==true -- the EE-gated field
        // (gate #3, golden LastSet.h:392, TU-local stand-in always false this wave) means the site is
        // NOT re-enabled via this sub-path -- documented gate-shaped behaviour.
        IniConfig.bI35UseThirdSiteControlByEngineer = true;
        LastSet.bUseTestSocket[0][0][0] = false;
        LastSet.bUseTestSocket[1][0][0] = false;
        SckArt_DoAutoSocketOff(st, true);
        CHECK(LastSet.bUseTestSocket[0][0][0] == false, "bI35=true + gated bUseTestSocketEE stand-in (always false) -> site NOT re-enabled via this sub-path (golden :1340-1343, gate #3)");
        IniConfig.bI35UseThirdSiteControlByEngineer = false;

        // bAllSiteOn==false && iFTRTCount!=0 branch (golden :1242-1329): ArmData/GetPCA is a TU-local
        // stand-in returning 0.0 (gate #4) -> dMaxYield stays 0 -> dTargetYield<=0 for any non-
        // negative dSCKART_AutoSocketOffYield -> the disable-loop body never runs -> bUseTestSocket
        // is left UNCHANGED by this branch, regardless of mode. Documented gate-shaped inert branch.
        TestIF_File.iSCKART_AutoSocketOffMode = 0;   // Auto Head
        TestIF_File.dSCKART_AutoSocketOffYield = 5.0;
        LastSet.bUseTestSocket[0][0][0] = true;
        LastSet.bUseTestSocket[1][0][0] = true;
        st.iFTRTCount = 1;
        SckArt_DoAutoSocketOff(st, false);
        CHECK(LastSet.bUseTestSocket[0][0][0] == true, "mode 0 (Auto Head), gated GetPCA==0.0 -> dTargetYield<=0 -> no site disabled (golden :1242-1279, gate #4)");

        TestIF_File.iSCKART_AutoSocketOffMode = 1;   // Auto Socket
        TestIF_File.iSCKART_MinAutoCloseSite = 0;    // avoid the WAR0724 alarm path noise
        SckArt_DoAutoSocketOff(st, false);
        CHECK(LastSet.bUseTestSocket[0][0][0] == true, "mode 1 (Auto Socket), gated GetPCA==0.0 -> dTargetYield<=0 -> no site disabled (golden :1281-1324, gate #4)");

        TestIF_File.iSCKART_AutoSocketOffMode = 2;   // Manual -- golden empty branch (:1325-1327)
        SckArt_DoAutoSocketOff(st, false);
        CHECK(LastSet.bUseTestSocket[0][0][0] == true, "mode 2 (Manual) -> golden empty branch, no-op (golden :1325-1327)");
    }

    // =========================================================================================
    // PART 7 -- DoChkInputCntAlarm -- golden :4359-4391
    // =========================================================================================
    printf("\n-- DoChkInputCntAlarm --\n");
    {
        SckArtState st;
        st.iInputCount = 100;
        TestIF_File.iSCKLdCntLimP = 10;
        TestIF_File.iSCKLdCntLimN = 5;

        CUSTOMER_CODE = 0;   // non-QUALCOMM -> always true (golden :4387-4390)
        CHECK(SckArt_DoChkInputCntAlarm(st, true)  == true, "non-CC_QUALCOMM -> always true regardless of bExcess (golden :4387-4390)");
        CHECK(SckArt_DoChkInputCntAlarm(st, false) == true, "non-CC_QUALCOMM -> always true regardless of bExcess (golden :4387-4390)");

        CUSTOMER_CODE = CC_QUALCOMM;
        LastSet.iSCKARTInputCT = 105;   // <= 100+10 -> within the allowed-excess band
        CHECK(SckArt_DoChkInputCntAlarm(st, true) == false, "CC_QUALCOMM, excess check: loadingCount(105)<=inputCount+LimP(110) -> no alarm (golden :4366-4369)");
        LastSet.iSCKARTInputCT = 115;   // > 100+10 -> alarm
        CHECK(SckArt_DoChkInputCntAlarm(st, true) == true, "CC_QUALCOMM, excess check: loadingCount(115)>inputCount+LimP(110) -> alarm (golden :4370-4373)");

        LastSet.iSCKARTInputCT = 96;    // >= 100-5 -> within the allowed-shortfall band
        CHECK(SckArt_DoChkInputCntAlarm(st, false) == false, "CC_QUALCOMM, shortfall check: loadingCount(96)>=inputCount-LimN(95) -> no alarm (golden :4377-4380)");
        LastSet.iSCKARTInputCT = 90;    // < 100-5 -> alarm
        CHECK(SckArt_DoChkInputCntAlarm(st, false) == true, "CC_QUALCOMM, shortfall check: loadingCount(90)<inputCount-LimN(95) -> alarm (golden :4381-4384)");
        CUSTOMER_CODE = 0;
    }

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
