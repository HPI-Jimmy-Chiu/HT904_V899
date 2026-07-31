// =============================================================================
//  tests/test_w7_f1_wall2_probe.cpp
//
//  AI(W906-W7-F1) 20260729: the W7-F1 GATE for
//  docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F1 ("Wall 2" unlock). The wave's own
//  gate is: "the 22 overrides must go from 'cannot reference fMain at all' to
//  'can be translated one by one' -- demonstrate that concretely". This file
//  is that demonstration.
//
//  WHAT WALL 2 WAS (plan SS4-V1, already fixed by W7-F0 -- re-verified here,
//  not re-fixed): FormsFacade.cpp used to compile into ht9045_sm, which
//  ht9045_secsgem did NOT link, while ht9045_sm DID link ht9045_secsgem -- a
//  genuine CMake target cycle that made it IMPOSSIBLE for
//  SECSGEM/uHGemHT9045.cpp to reach `fMain`, no matter how many facade members
//  existed. W7-F0 moved the facade into the new bottom-layer `ht9045_forms`
//  library and had `ht9045_secsgem` link it (CMakeLists.txt's
//  `target_link_libraries(ht9045_secsgem PUBLIC ... ht9045_forms)` line) --
//  acyclic, because `ht9045_forms` depends on nothing above itself.
//
//  WHAT THIS PROBE ACTUALLY CHECKS, IN LAYERS:
//    1. LINK LAYER: this section has now been corrected TWICE; the version
//       below is the one backed by an experiment. History, so nobody
//       re-litigates it: the wave originally claimed the executable "names
//       only ht9045_secsgem in its CODE" and resolves `fMain` "purely through
//       ht9045_secsgem's PUBLIC dependency on it" (FALSE -- the CMake call
//       names seven libraries explicitly); AI(W906-W7-F1fix) 20260729 then
//       replaced that with "the explicit list is REQUIRED ... a bare
//       target_link_libraries(... PRIVATE ht9045_secsgem) (relying purely on
//       ht9045_secsgem's PUBLIC transitive edge to ht9045_forms) hit the
//       IDENTICAL undefined-reference failure", i.e. it treated the transitive
//       edge as the thing that does not work. AI(W906-W7-F1fix2) 20260729
//       MEASURED it instead of arguing about it, and the true split is:
//         (a) The transitive PUBLIC edge DOES put ht9045_forms on the link
//             line by itself. Evidence, from the generated build system rather
//             than from reasoning: test_SecsWireCodec's CMake call is a bare
//             `PRIVATE ht9045_secsgem`, and the linkLibs response file CMake
//             generates for it is
//               libht9045_secsgem.a libht9045_core.a libht9045_public.a
//               libht9045_db.a libht9045_forms.a libht9045_globals.a
//               libvclcompat.a
//             -- ht9045_forms is there, unnamed by anyone, purely via the
//             PUBLIC closure. So a consumer naming only ht9045_secsgem CAN
//             see fMain.
//         (b) What the bare form lacks is the RESCAN (--start-group), not
//             ht9045_forms. Relinking THIS probe's own objects against exactly
//             that bare-form archive list fails with 5 errors, and every one
//             of them is `libht9045_globals.a(cpublic.cpp.obj): undefined
//             reference to SearchFile(...)/md5_Folder(...)` -- symbols defined
//             in libht9045_public.a (verified with nm --defined-only), which
//             single-pass GNU ld already scanned and passed. ZERO of the
//             errors involve fMain or anything in ht9045_forms.
//         (c) Re-linking that SAME bare archive list wrapped in
//             -Wl,--start-group/--end-group succeeds (exit 0). So the group,
//             not the explicit library list, is the load-bearing part.
//       WHAT THIS PROBE THEREFORE PROVES: Wall 2 is a CONFIGURE-time target-
//       cycle question and it is answered (ht9045_secsgem and ht9045_forms
//       coexist on one link line without CMake diagnosing a cycle), AND fMain
//       genuinely resolves for a consumer of ht9045_secsgem -- (b) shows the
//       facade symbols resolve even in the un-grouped link. WHAT IT DOES NOT
//       prove: that a future consumer can drop the RESCAN group. It cannot --
//       (b) is a cpublic/ht9045_public archive-ordering problem that has
//       nothing to do with Wall 2, and the explicit list exists only because
//       CMake's $<LINK_GROUP:RESCAN,...> wraps only the libraries named
//       INSIDE it. Same reason test_config_loaders needs the same set.
//    2. INCLUDE LAYER: this ONE translation unit #includes BOTH
//       "SECSGEM/uHGemHT9045.h" (the header the real 22 overrides will live
//       in) AND "FormsFacade.h" (the facade umbrella, at repo root -- mirrors
//       golden's own main.h location) -- exactly the situation
//       golden's uHGemHT9045.cpp is in (it #includes "main.h" directly,
//       uHGemHT9045.cpp:8). If these two headers could not coexist, no
//       override could ever be translated.
//    3. EXPRESSION LAYER (the part that actually matters for "can be
//       translated one by one"): PART A below reproduces, EXPRESSION FOR
//       EXPRESSION, the exact `fMain->member` shapes golden's
//       SECSGEM/uHGemHT9045.cpp dereferences (golden file:line cited at each
//       site), inside free functions shaped like a future override body. If
//       every one of golden's 26 LIVE (non-commented-out) `fMain->` spellings
//       compiles here, a translator has nothing left blocking a first
//       override. ONE EXCEPTION, flagged where it occurs below
//       (mimic_site_on_off_decode): the tSiteOnOff site needs an explicit
//       `AnsiString(...)` wrapper around `->Strings[z]` that golden's own
//       `.Strings[z].c_str()` does not need, because vclcompat's
//       StringsProxy return type has no `c_str()` of its own. That one site
//       is therefore "compiles against the real facade" but not literally
//       "expression for expression" with golden.
//
//  INVENTORY NOTE (re-derived this wave by grepping golden directly, not
//  trusting the plan's own count): golden SECSGEM/uHGemHT9045.cpp touches 28
//  DISTINCT `fMain->NAME` spellings by a naive grep. 2 of those --
//  `fMain->PPID` and `fMain->bNeedClearFile` (golden :5311-5312) -- are
//  inside `//`-commented-out code with NO live call site, so they are
//  deliberately NOT reproduced below and NOT added to the facade (adding a
//  member nothing dereferences would be inventing surface, which this
//  project's conventions forbid). The 26 live members split as:
//    * 10 already existed before this wave: Start, ShowTestHeadComp,
//      CleanOut, LoadTestModePicture, SetStartModeData, BtnOneCycleClick,
//      Reset, cbSetupFileName, edWorkTemperBase, Home. (The plan's own "do
//      not duplicate" list named only the first 9 of these -- Home was ALSO
//      already present and reusable but not mentioned there.)
//    * 16 added by this wave: cbSetupFileNameChange, Clarn_Data,
//      BtnPauseClick, LoadRunModePicture, CanChangeSite, BtnTrayEndClick,
//      UpdateMainOperateMode, LoadStartModePicture, LookForFile,
//      ChangeTesterConnect, SetTemp, tSiteOnOff, edSoakTime (all 13 named by
//      the plan) PLUS 3 the plan's own inventory missed: ChangePassword,
//      FTClick, RTClick (golden uHGemHT9045.cpp:2513/2241/2266 -- all live,
//      non-commented calls).
//
//  AI(W906-W7-F1fix) 20260729 -- HOW FAR THIS ACTUALLY UNLOCKS TRANSLATION
//  (corrects an over-broad "the 22 overrides can now be translated one by
//  one" reading of the gate above): of golden's 22 HT9045Gem overrides, 9
//  touch fMain at all (grepped directly against each override's own body,
//  not estimated): ReloadParameter, LookForFile,
//  S2F15_CheckNewEquipmentConstant, S2F15_UpdateNewEquipmentConstant,
//  S2F42_Host_Command_Acknowledge, ProcessS7F23FromatReceipe,
//  ProcessS7F25FromatReceipe (all in uHGemHT9045.cpp), plus AddSV
//  (uHGemHT9045_SV.cpp) and AddEC (uHGemHT9045_EC.cpp). Only 5 of those 9
//  touch ONLY fMain and are therefore FULLY unblocked by this file's facade
//  additions: ReloadParameter, LookForFile, S2F15_CheckNewEquipmentConstant,
//  ProcessS7F23FromatReceipe, ProcessS7F25FromatReceipe. The remaining 4 ALSO
//  dereference fLotInfo/fSCKART/fNote/fSetup -- none of which gained a single
//  member this wave -- so they remain blocked exactly as before:
//  S2F15_UpdateNewEquipmentConstant (+fLotInfo, +fSetup),
//  S2F42_Host_Command_Acknowledge (+fLotInfo, +fNote, +fSCKART -- one of the
//  two largest override bodies, golden uHGemHT9045.cpp:1146-4189 -- corrected
//  from :1146-4191 by AI(W906-W7-F1fix2) after brace-matching the body: :4189
//  is its closing brace, :4190 a separator comment, :4191 already
//  S5F6_ListAlarmData's own signature line), AddSV
//  (+fLotInfo), and AddEC (+fLotInfo, +fSCKART, +fSetup -- the other largest,
//  effectively all ~1912 lines of uHGemHT9045_EC.cpp). Do not plan a later
//  bucket assuming all 22 (or even all 9 fMain-touching) overrides are ready;
//  see docs/W7_UI_ARCHITECTURE_PLAN.md SS10 for the corresponding
//  comment-filtered fLotInfo/fSCKART/fNote/fSetup member inventory.
//
//  AI(W906-W7-F1fix2) 20260729 -- ONE MORE GAP BOTH BLOCKS ABOVE MISS, and it
//  changes the AddSV entry: the INVENTORY NOTE is scoped to uHGemHT9045.cpp
//  alone, but AddSV lives in uHGemHT9045_SV.cpp, and that file dereferences SIX
//  fMain members of its own (grepped and comment-filtered the same way):
//  SVID1190_OSSetup (:231), palMainStatus (:73) and tTestResult (:337) -- all
//  three already in the facade from earlier waves -- PLUS edTorue0 (:74),
//  edTorue1 (:75) and lbEPenconder (:100), which do NOT exist in forms/fMain.h
//  and were NOT added by this wave (verified by grepping the whole ported tree,
//  not just fMain.h). So AddSV is blocked on the fMain side TOO, not only by
//  fLotInfo; reading its entry above as "+fLotInfo" only would send the next
//  agent hunting 18 fLotInfo members and let it hit the 3 missing fMain widgets
//  the hard way. uHGemHT9045_EC.cpp's fMain surface, by contrast, IS complete
//  (cbSetupFileName :64 + tSiteOnOff :72-73, both present). Counting golden's
//  three SECSGEM TUs together: 34 distinct fMain spellings, 32 live, 29 of
//  which now exist in the facade.
// =============================================================================
#include "SECSGEM/uHGemHT9045.h"   // the header the 22 overrides will live in
#include "FormsFacade.h"           // fMain -- exactly what those overrides will dereference
#include "MachineType.h"           // MAX_SOCKET_ROW / MAX_SOCKET_COL (tSiteOnOff prefill check)

#include <cstdio>
#include <cstdlib>

static int g_pass = 0;
static int g_fail = 0;

static void check(const char *name, bool ok)
{
    if (ok) { printf("PASS  %s\n", name); ++g_pass; }
    else    { printf("FAIL  %s\n", name); ++g_fail; }
}

// =============================================================================
//  PART A -- expression-shape mirror of every LIVE fMain-> site golden's
//  uHGemHT9045.cpp dereferences. Each function below is shaped like a small
//  slice of a future override body: it is never called from a real SM, it
//  exists purely so the compiler proves the expression is well-formed against
//  the REAL fMain type (not a mock) -- the same role test_w7_f0_controls_guard
//  plays for the widget-overload hazard, one layer up.
// =============================================================================
namespace w7f1_wall2_probe {

// golden uHGemHT9045.cpp:1135 `fMain->cbSetupFileNameChange(fMain);`
void mimic_S2F49_or_similar_recipe_dl(int HCACK)
{
    if (HCACK != 0)
        fMain->cbSetupFileNameChange(fMain);
}

// golden uHGemHT9045.cpp:1771 `fMain->Clarn_Data(8, "Clean by TRAY CHECK");`
// golden uHGemHT9045.cpp:3473 `fMain->Clarn_Data(1, "ART_LOTCLEARED");`
void mimic_S2F42_clear_count()
{
    fMain->Clarn_Data(8, "Clean by TRAY CHECK");
    fMain->Clarn_Data(1, "ART_LOTCLEARED");
}

// golden uHGemHT9045.cpp:4116/:4121 `fMain->BtnPauseClick(fMain);`
void mimic_S2F41_pause_rcmd()
{
    fMain->BtnPauseClick(fMain);
}

// golden uHGemHT9045.cpp:378 `fMain->LoadRunModePicture();`
// golden uHGemHT9045.cpp:377 `fMain->UpdateMainOperateMode();`
// golden uHGemHT9045.cpp:379 `fMain->LoadStartModePicture();`
void mimic_ctor_or_reload_picture_refresh()
{
    fMain->UpdateMainOperateMode();
    fMain->LoadRunModePicture();
    fMain->LoadStartModePicture();
}

// golden uHGemHT9045.cpp:543-544
//   `if((LastSet.iTemperature==Tempture_Hot && fMain->CanChangeSite(true)==false) ||
//       (LastSet.iTemperature!=Tempture_Hot && fMain->CanChangeSite(false)==false))`
bool mimic_temperature_site_gate(bool bIsHot)
{
    if ((bIsHot  && fMain->CanChangeSite(true) == false) ||
        (!bIsHot && fMain->CanChangeSite(false) == false))
        return false;
    return true;
}

// golden uHGemHT9045.cpp:2092/:2108 `fMain->BtnTrayEndClick(fMain);`
void mimic_tray_end_rcmd()
{
    fMain->BtnTrayEndClick(fMain);
}

// golden uHGemHT9045.cpp:859 `fMain->ChangeTesterConnect(LastSet.iTester, false, true);`
void mimic_online_mode_change(int iTester)
{
    fMain->ChangeTesterConnect(iTester, false, true);
}

// golden uHGemHT9045.cpp:1077
//   `fMain->SetTemp(true, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));`
void mimic_ec_temperature_apply()
{
    fMain->SetTemp(true,
                   atof(fMain->edWorkTemperBase->Text.c_str()),
                   atof(fMain->edSoakTime->Text.c_str()));
}

// golden uHGemHT9045.cpp:2513 `fMain->ChangePassword();`
void mimic_s2f49_password_change()
{
    fMain->ChangePassword();
}

// golden uHGemHT9045.cpp:2241 `HCACK=fMain->FTClick();`
// golden uHGemHT9045.cpp:2266 `HCACK=fMain->RTClick();`
int mimic_switch_to_ft_rt(bool bToFt)
{
    return bToFt ? fMain->FTClick() : fMain->RTClick();
}

// golden uHGemHT9045.cpp:893-914 (site-on-off decode loop, shape preserved)
void mimic_site_on_off_decode(int x, int y, int z, bool bUseTestSocket[2][4][8])
{
    if (z < fMain->tSiteOnOff[0]->Count)   // StringsProxy has no c_str() -- explicit AnsiString cast (established idiom,
                                            // e.g. database.cpp's TIODATA::TIODATA constructor). NOT expression-for-
                                            // expression with golden here (golden writes `.Strings[z].c_str()`
                                            // directly, uHGemHT9045.cpp:894/914) -- see the header's EXPRESSION LAYER
                                            // note for this one documented exception. AI(W906-W7-F1fix2) 20260729:
                                            // the same site has a SECOND, smaller deviation the header did not
                                            // mention -- golden assigns the atoi() int straight into the bool array
                                            // element (`bUseTestSocket[0][x][y]=atoi(...)`), this probe writes
                                            // `(atoi(...) != 0)` to stay clean under -Wall. Neither deviation
                                            // changes what is being proved (that the fMain-> subexpressions are
                                            // well-formed against the real facade), but "expression for expression"
                                            // is not literally true for this line.
        bUseTestSocket[0][x][y] = (atoi(AnsiString(fMain->tSiteOnOff[0]->Strings[z]).c_str()) != 0);
    if (z < fMain->tSiteOnOff[1]->Count)
        bUseTestSocket[1][x][y] = (atoi(AnsiString(fMain->tSiteOnOff[1]->Strings[z]).c_str()) != 0);
}

// -- the 10 members that ALREADY existed before this wave (plan's "do not
//    duplicate" list + Home, which the plan's list omitted but which was also
//    already present) -- reproduced here too so PART A's claim ("all 26 live
//    members compile against the real facade in this one TU") is total, not
//    partial.
void mimic_preexisting_members()
{
    fMain->Start("probe");
    fMain->ShowTestHeadComp(true);
    fMain->CleanOut("probe");
    fMain->LoadTestModePicture();
    fMain->SetStartModeData();
    fMain->BtnOneCycleClick(fMain);
    fMain->Reset("probe");
    AnsiString s = fMain->cbSetupFileName->Text;
    (void)s;
    fMain->edWorkTemperBase->Text = AnsiString("25.0");
    (void)fMain->Home("probe");
}

} // namespace w7f1_wall2_probe

int main()
{
    printf("=== W7-F1 Wall-2 unlock probe ===\n");
    // AI(W906-W7-F1fix2) 20260729: this banner used to end with "fMain ...
    // resolves purely via ht9045_secsgem's own PUBLIC dependency on it" while
    // the target's own link line names ht9045_forms explicitly -- the header's
    // LINK LAYER section (corrected twice, now measured) is the authority.
    printf("NOTE  PART A (namespace w7f1_wall2_probe) is a COMPILE-TIME gate:\n");
    printf("NOTE  this executable existing and linking at all means every\n");
    printf("NOTE  fMain-> expression in it -- copied shape-for-shape from\n");
    printf("NOTE  golden SECSGEM/uHGemHT9045.cpp -- compiled clean against the\n");
    printf("NOTE  REAL forms/fMain.h facade, in the SAME translation unit as\n");
    printf("NOTE  SECSGEM/uHGemHT9045.h. This target's link line names\n");
    printf("NOTE  ht9045_forms explicitly (inside a LINK_GROUP:RESCAN), so the\n");
    printf("NOTE  bare-transitive-edge property is NOT what this run shows; see\n");
    printf("NOTE  the LINK LAYER section in this file's header for the measured\n");
    printf("NOTE  split between 'transitive edge delivers ht9045_forms' (true)\n");
    printf("NOTE  and 'the RESCAN group is droppable' (false).\n");

    // -------------------------------------------------------------------
    //  RUNTIME LAYER -- exercise every new member's documented default /
    //  observable behaviour (the "not a bare no-op" gate).
    // -------------------------------------------------------------------
    // AI(W906-W7-F1fix2) 20260729: label corrected -- this target does NOT link
    // "via ht9045_secsgem only" (see the LINK LAYER header section).
    check("fMain resolved (non-NULL global defined in ht9045_forms, reached from a ht9045_secsgem consumer TU)",
          fMain != 0);

    check("W906_cbSetupFileNameChangeCallCount starts at 0", fMain->W906_cbSetupFileNameChangeCallCount == 0);
    fMain->cbSetupFileNameChange(fMain);
    fMain->cbSetupFileNameChange(fMain);
    check("cbSetupFileNameChange() increments its call-count seam (gap is test-observable)",
          fMain->W906_cbSetupFileNameChangeCallCount == 2);

    check("W906_Clarn_DataCallCount starts at 0", fMain->W906_Clarn_DataCallCount == 0);
    fMain->Clarn_Data(8, "test");
    check("Clarn_Data() increments its call-count seam", fMain->W906_Clarn_DataCallCount == 1);

    check("W906_LoadRunModePictureCallCount starts at 0", fMain->W906_LoadRunModePictureCallCount == 0);
    fMain->LoadRunModePicture();
    check("LoadRunModePicture() increments its call-count seam", fMain->W906_LoadRunModePictureCallCount == 1);

    check("W906_CanChangeSite_Sim defaults true (golden's own 'no IC anywhere' fall-through)",
          fMain->W906_CanChangeSite_Sim == true);
    check("CanChangeSite(true) returns the Sim seam's value (true)", fMain->CanChangeSite(true) == true);
    fMain->W906_CanChangeSite_Sim = false;
    check("CanChangeSite is test-DRIVABLE: setting the Sim seam false flips the return value",
          fMain->CanChangeSite(false) == false);
    fMain->W906_CanChangeSite_Sim = true;   // restore

    check("W906_BtnTrayEndClickCallCount starts at 0", fMain->W906_BtnTrayEndClickCallCount == 0);
    fMain->BtnTrayEndClick(fMain);
    check("BtnTrayEndClick() increments its call-count seam", fMain->W906_BtnTrayEndClickCallCount == 1);

    check("W906_UpdateMainOperateModeCallCount starts at 0", fMain->W906_UpdateMainOperateModeCallCount == 0);
    fMain->UpdateMainOperateMode();
    check("UpdateMainOperateMode() increments its call-count seam", fMain->W906_UpdateMainOperateModeCallCount == 1);

    check("W906_LoadStartModePictureCallCount starts at 0", fMain->W906_LoadStartModePictureCallCount == 0);
    fMain->LoadStartModePicture();
    check("LoadStartModePicture() increments its call-count seam", fMain->W906_LoadStartModePictureCallCount == 1);

    check("W906_LookForFileCallCount starts at 0", fMain->W906_LookForFileCallCount == 0);
    fMain->LookForFile();
    check("LookForFile() increments its call-count seam", fMain->W906_LookForFileCallCount == 1);

    check("W906_ChangeTesterConnect_Sim defaults 0 (golden success code)", fMain->W906_ChangeTesterConnect_Sim == 0);
    check("ChangeTesterConnect(...) returns the Sim seam's value", fMain->ChangeTesterConnect(1, false, true) == 0);
    fMain->W906_ChangeTesterConnect_Sim = 1;
    check("ChangeTesterConnect is test-DRIVABLE: Sim seam=1 flips the return value",
          fMain->ChangeTesterConnect(1, false, true) == 1);
    fMain->W906_ChangeTesterConnect_Sim = 0;   // restore

    check("W906_SetTemp_Sim defaults 0", fMain->W906_SetTemp_Sim == 0);
    check("SetTemp(...) returns the Sim seam's value", fMain->SetTemp(true, 25.0, 0.0) == 0);
    // AI(W906-W7-F1fix2) 20260729: SetTemp/FTClick/RTClick previously only ever
    // asserted "== 0" against a seam that was never moved off its 0 default --
    // which a hard-coded `return 0;` would satisfy just as well. Drive each seam
    // to a DISTINCT golden return code (golden main.cpp:23897 SetTemp=1;
    // :29700 FTClick=8; :29797 RTClick=7 -- the SPIL gate FTClick has no
    // counterpart for) so the assertion pins "returns the seam", not "returns 0".
    fMain->W906_SetTemp_Sim = 1;
    check("SetTemp is test-DRIVABLE: Sim seam=1 (golden SystemStart block) flips the return value",
          fMain->SetTemp(true, 25.0, 0.0) == 1);
    fMain->W906_SetTemp_Sim = 0;   // restore

    check("W906_ChangePasswordCallCount starts at 0", fMain->W906_ChangePasswordCallCount == 0);
    fMain->ChangePassword();
    check("ChangePassword() increments its call-count seam", fMain->W906_ChangePasswordCallCount == 1);

    check("W906_FTClick_Sim defaults 0 (golden success code)", fMain->W906_FTClick_Sim == 0);
    check("FTClick() returns the Sim seam's value", fMain->FTClick() == 0);
    fMain->W906_FTClick_Sim = 8;
    check("FTClick is test-DRIVABLE: Sim seam=8 (a real golden FTClick code) flips the return value",
          fMain->FTClick() == 8);
    fMain->W906_FTClick_Sim = 0;   // restore
    check("W906_RTClick_Sim defaults 0", fMain->W906_RTClick_Sim == 0);
    check("RTClick() returns the Sim seam's value", fMain->RTClick() == 0);
    fMain->W906_RTClick_Sim = 7;
    check("RTClick is test-DRIVABLE: Sim seam=7 (RTClick's SPIL-gate code, which FTClick never returns) flips it",
          fMain->RTClick() == 7);
    fMain->W906_RTClick_Sim = 0;   // restore

    // tSiteOnOff: REAL concrete storage, not a Sim seam -- pins the golden
    // prefill count (main.cpp:2242-2248) that uHGemHT9045.cpp's
    // `if(z<fMain->tSiteOnOff[0]->Count)` guard depends on.
    check("tSiteOnOff[0] prefilled to MAX_SOCKET_ROW*MAX_SOCKET_COL entries",
          fMain->tSiteOnOff[0]->Count == MAX_SOCKET_ROW * MAX_SOCKET_COL);
    check("tSiteOnOff[1] prefilled to MAX_SOCKET_ROW*MAX_SOCKET_COL entries",
          fMain->tSiteOnOff[1]->Count == MAX_SOCKET_ROW * MAX_SOCKET_COL);
    check("tSiteOnOff[0] entries default \"0\" (golden main.cpp:2246)",
          fMain->tSiteOnOff[0]->Strings[0] == AnsiString("0"));

    // edSoakTime: reused TfLotInfoEdit widget, ->Text read/written exactly
    // like the sibling edWorkTemperBase already in the facade.
    fMain->edSoakTime->Text = AnsiString("5.0");
    check("edSoakTime->Text round-trips (TfLotInfoEdit stand-in)",
          fMain->edSoakTime->Text == AnsiString("5.0"));

    // BtnPauseClick: TRANSLATED (forwards to the pre-existing Pause() virtual),
    // but Pause() is a bare `return false;` -- so the forward is invisible
    // unless Pause() itself is instrumented. Three generations of this block:
    //   1. the wave asserted the literal constant `true` (could never fail);
    //   2. AI(W906-W7-F1fix) 20260729 replaced it with the
    //      W906_BtnPauseClickCallCount checks below -- failable, but only for
    //      "BtnPauseClick ran", NOT for the forward;
    //   3. AI(W906-W7-F1fix2) 20260729 added W906_PauseCallCount /
    //      W906_PauseLastFunc on Pause() itself and the assertions below,
    //      which DO see the forward (and pin the "BtnPauseClick" argument
    //      golden passes, main.cpp:6967).
    // MEASURED, by relinking this probe against a fMain.cpp with exactly one
    // line changed (ar-replacing fMain.cpp.obj inside libht9045_forms.a, so the
    // shared tree was never left broken):
    //   * increment removed  -> 41 passed, 1 failed: "increments its own
    //     call-count seam" FAILS, both forward checks still PASS (correct -- the
    //     forward really did still happen).
    //   * forward removed    -> 40 passed, 2 failed: the two forward checks
    //     FAIL, and "increments its own call-count seam" still PASSES. That
    //     pass is the whole point: it is direct proof that the fix1 seam alone
    //     could not see the forward, and that the gap MEDIUM-1 was raised about
    //     survived the first fix.
    check("W906_BtnPauseClickCallCount starts at 0", fMain->W906_BtnPauseClickCallCount == 0);
    check("W906_PauseCallCount starts at 0 (Pause not yet called by anything in this TU)",
          fMain->W906_PauseCallCount == 0);
    check("W906_PauseLastFunc starts empty", fMain->W906_PauseLastFunc == AnsiString(""));
    fMain->BtnPauseClick(fMain);
    check("BtnPauseClick() increments its own call-count seam (it ran)",
          fMain->W906_BtnPauseClickCallCount == 1);
    check("BtnPauseClick() FORWARDED into Pause() exactly once (the forward itself, not just the call)",
          fMain->W906_PauseCallCount == 1);
    check("BtnPauseClick()'s forward carried golden's own \"BtnPauseClick\" Func argument (golden main.cpp:6967)",
          fMain->W906_PauseLastFunc == AnsiString("BtnPauseClick"));

    printf("\n=== Summary: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
