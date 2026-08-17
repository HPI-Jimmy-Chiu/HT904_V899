// =============================================================================
//  Command.cpp  --  FW-3 Wave A translation: golden Command.cpp PURE-method group
//
//  Translation wave: FW-3 Wave A
//  Translator: AI(W906-FW3-WA) 20260817
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Command.cpp (15,273 lines, cp950)
//
//  ROLE
//  ----
//  This is a NEW file (Command.cpp did not exist in the port before this wave). It
//  carries the out-of-line bodies for 63 golden TfMain:: GPIB/status-string methods
//  drawn from four golden byte ranges (see WAVE SCOPE below). Every method is
//  declared `virtual` in forms/fMain.h's "FW3-WA ADD" block (append-only, no
//  existing line touched) except the 3 that already had a declaration + offline
//  stub body from an earlier wave -- see STUB COLLISIONS.
//
//  TASK-COUNT DISCREPANCY (stated, not silently absorbed): the task brief said
//  "~82 個方法" across the four ranges; grepping the cp950-decoded golden for
//  every top-level `TfMain::` definition inside the four exact byte ranges (A1
//  86-2242, A2 3825-4005, A3 5170-5638 minus SetSiteMapData/SetAlarmSetup, A4
//  12063-12508) this pass finds 63, not 82 (A1=16, A2=4, A3=9, A4=34 -- A4 in
//  particular is 34, not the "47" the brief named). The GOLDEN LINE COUNT matches
//  closely (3,265 raw extracted lines vs. the brief's "~3,250"), so the byte
//  ranges themselves are right; the per-range method tally in the brief appears to
//  be stale/inflated. No methods were invented to reach either headline number.
//
//  WAVE SCOPE  (every golden method, golden line span, ACTIVE or GATED-partial)
//  ------------------------------------------------------------------------------
//  GROUP A1 (golden :86-2242, 16 methods):
//    RefreshTempData                :86-933      ACTIVE
//    WritePERSITETemperature        :935-943     ACTIVE   [STUB COLLISION]
//    PERSITETemperatureStrings      :945-1482    GATED-partial (fContact)  [STUB COLLISION]
//    WriteHandlerID                 :1484-1487   ACTIVE
//    WriteArmStatus                 :1489-1495   ACTIVE
//    ArmStatusStrings                :1497-1508   ACTIVE   [STUB COLLISION]
//    WriteArmForce                   :1510-1541   ACTIVE
//    WriteTempData                   :1543-1549   ACTIVE
//    TempDataStrings                 :1551-1586   ACTIVE
//    WriteSetTempStatus              :1588-1649   GATED-partial (ChangeTempMode x2)
//    WriteSetTestTempStatus          :1651-1764   GATED-partial (fContact, ATC_InterfaceForm, fLotInfo)
//    WriteSoakTimeData                :1766-1775   ACTIVE
//    WriteSetSoakTimeStatus           :1777-1794   ACTIVE
//    WriteSiteMapData                 :1797-2028   ACTIVE
//    WriteStartMode_NS                :2030-2060   ACTIVE
//    WriteAssign_NS                   :2062-2242   ACTIVE
//  GROUP A2 (golden :3825-4005, 4 methods):
//    WriteForce_NS                    :3825-3855   ACTIVE
//    WriteBinMap                      :3858-3912   ACTIVE
//    WriteSetBinMap                   :3914-3987   GATED-partial (fBinSel)
//    WriteTestMode                    :3989-4005   ACTIVE
//  GROUP A3 (golden :5170-5638 minus SetSiteMapData/:5301 and SetAlarmSetup/:5341, 9 methods):
//    WriteChkSetup                    :5170-5275   ACTIVE
//    WriteHandlerTestArmEncoder       :5277-5291   ACTIVE
//    WriteHandlerTestArmEP            :5293-5299   GATED-partial (ADAM_ReadPA)
//    GetCZtesterBin                   :5390-5417   ACTIVE
//    GetCZSoakTime                    :5419-5427   ACTIVE
//    GetCZDoubleContactCount          :5429-5438   ACTIVE
//    GetCDHandlerID                   :5440-5445   ACTIVE
//    GetCZJamCode                     :5447-5461   ACTIVE  (fNote IS present -- real facade, not gated)
//    GetCZSiteMap                     :5463-5638   ACTIVE
//  GROUP A4 (golden :12063-12508, 34 methods -- see TASK-COUNT DISCREPANCY above):
//    SetTesterID                      :12063-12102 GATED-partial (fConfiguration)
//    GetTesterID                      :12104-12109 ACTIVE
//    GetAutoClean                     :12111-12116 ACTIVE
//    AutoCleanStrings                 :12118-12128 ACTIVE
//    GetForcePerPinN                  :12130-12135 ACTIVE
//    ForcePerPinNStrings              :12137-12142 ACTIVE
//    GetContactHeight                 :12144-12149 ACTIVE
//    ContactHeightStrings             :12151-12156 ACTIVE
//    GetYieldContinusFail             :12158-12163 ACTIVE
//    YieldContinusFailStrings         :12165-12173 ACTIVE
//    GetYieldSiteCompare              :12175-12180 ACTIVE
//    YieldSiteCompareStrings          :12182-12190 ACTIVE
//    GetDUTStaus                      :12192-12197 ACTIVE
//    DUTStausStrings                  :12199-12264 ACTIVE
//    GetUPH                           :12266-12271 ACTIVE
//    UPHStrings                       :12273-12285 GATED-partial (fShowBinSelect, whole body)
//    GetIndexCycleTime                :12287-12292 ACTIVE
//    IndexCycleTimeStrings            :12294-12306 GATED-partial (fObserver, whole body)
//    GetTempOfs                       :12308-12313 ACTIVE
//    TempOfsStrings                   :12315-12332 ACTIVE  (UserOffSet resolved as a real literal, not gated)
//    GetTempRange                     :12334-12339 ACTIVE
//    TempRangeStrings                 :12341-12347 ACTIVE
//    GetVacuumAir                     :12349-12354 ACTIVE
//    VacuumAirStrings                 :12356-12363 ACTIVE
//    GetAll                           :12365-12385 ACTIVE
//    GetHandlerVersion                :12387-12392 ACTIVE
//    HandlerVersionStrings            :12394-12399 ACTIVE
//    UploadProdLog                    :12401-12447 ACTIVE
//    GetShuttleMode                   :12449-12464 ACTIVE
//    SetMaxTest                       :12466-12478 GATED-partial (FTestIF)
//    GetMaxTest                       :12480-12485 ACTIVE
//    SetMaxInitialTest                :12487-12499 GATED-partial (FTestIF)
//    GetMaxInitialTest                :12501-12506 ACTIVE
//    GetSiteState                     :12508-12536 ACTIVE
//  TOTALS: 63 methods, 3,265 raw golden lines extracted; 53 ACTIVE, 10 GATED-partial.
//
//  GATE REGISTER  (13 #if 0 sites; each states WHY the gate is correct, not just
//  "not found" -- per this wave's own anti-absence-claim-rot instruction)
//  ------------------------------------------------------------------------------
//   1. PERSITETemperatureStrings, golden :956 `if(fContact->fShow)` -- TfContact/
//      fContact (golden cContact.h, the 22761-line out-of-scope VCL Contact-mode
//      form) has NO translated home anywhere in the port; fShow can only be
//      raised by that form's own live UI thread, which does not exist offline, so
//      "always false" is the SAME "no such subsystem is live offline" posture
//      used tree-wide (e.g. TfMainInplace::InArmPlacementEnable()), not a
//      behavior guess. Gated to the ELSE arm, which is fully ACTIVE (IndexStatus/
//      iContactMode/iIndexArm/CONTACT_NORMAL are all real).
//   2. WriteSetTempStatus, golden :1596 and :1607, both
//      `ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);` --
//      ChangeTempMode is golden main.h:1323, a TfMain MEMBER (bare call = implicit
//      this->), not in forms/fMain.h. Its real body is a ShowMyMessageBox_YES_NO +
//      temp-mode UI cascade with no facade path here. `ret` is left at whatever
//      SetTemp() (a real, already-GATED-leaf fMain member, offline success code 0)
//      returned, which already drives the SETTINGOK/SETTINGNG reply exactly as if
//      ChangeTempMode had itself succeeded -- same "offline never fails a
//      hardware/UI op" posture as W906_SetTemp_Sim.
//   3. WriteSetTestTempStatus, golden :1656 `if(fContact->fShow)` -- identical
//      absence/reasoning as gate #1; gated to the ELSE arm (`iNowDownArm=IndexStatus;`).
//   4. WriteSetTestTempStatus, golden :1721-1774, the whole
//      `ATC_InterfaceForm->SetSingleTemp(...)`/`->SetAllTemp(...)` cluster --
//      ATC_InterfaceForm DOES exist (acarry_shims.h TATC_InterfaceFormShim), but
//      that shim carries ONLY `int iATC_MODE_TYPE`, no SetSingleTemp/SetAllTemp
//      (golden's real TATC_InterfaceForm is ATC_Handler_Side.h, a whole
//      untranslated hardware-channel VCL class -- same class-of-thing MainCalcCore.h
//      / acarry_shims.h / SCK_ART_Remainder.h already gate). Every gated line only
//      ever WRITES to that nonexistent hardware channel; the externally-visible
//      `t.sprintf(...)` reply and the real bChangeTest_TempOffset/
//      bChangeTest_TempAlarm globals are all preserved ACTIVE and un-gated.
//   5. WriteSetTestTempStatus, golden :1737 `fLotInfo->SetATCOffset(true);` --
//      ESTABLISHED precedent, not a fresh finding: aTester_Front.cpp:3743-3744 /
//      aTester_Rear.cpp:3612-3613 already gate this EXACT golden call because
//      TfLotInfo (forms/fLotInfo.h) has no SetATCOffset member at all (the method
//      is missing, not the object).
//   6. WriteSetBinMap, golden :3921/:3924/:3958/:3971/:3975/:3984-3985 (all
//      `fBinSel->sBinTraySetT3Pos[eBinFT]->...` and `->Save(...)`) -- fBinSel
//      (golden TfBinSel, the whole Bin-Select mapping VCL form) has NO translated
//      home; forms/fMain.h's OWN SetMainRunStartMode comment already documents
//      this exact form as out of scope. Every gated line only mutates that form's
//      OWN internal grid; the loop's one real port-global side effect,
//      `BinSelect[eBinFT].IfErrorT3=iTray;` (SYSTEM_BIN_SELECT, cprod.h:2622,
//      confirmed real), and the final `Data="SETTINGOK";` reply stay ACTIVE.
//   7. WriteHandlerTestArmEP, golden :5297 `ADAM_ReadPA(&dValue)` -- the ADAM6024
//      DAQ EP-voltage readback has no translated home at all (only dfm2rc layout
//      artifacts for the adam6024 FORM exist under tools/dfm2rc/, no logic header).
//      No ADAM DLL binding exists offline to read a value from; `t` stays empty
//      (SendMSG_CMD is itself an offline no-op sink either way).
//   8. SetTesterID, golden :2887 `fConfiguration->edN27_6->Text=asCmd;` --
//      fConfiguration (golden TfConfiguration, cConfiguration.h, a whole
//      untranslated VCL form) has no forms/ facade; the only `fConfiguration` in
//      the port at all is Automation/SCK_ART_Remainder.h's TU-local
//      W5SckArtRem_ConfigStub, which carries only ->mmoN04_IP, not edN27_6. Pure
//      UI-mirror write: the persisted WriteIniData() call and the live
//      IniConfig.sN27_TesterID global are both real and un-gated either side of it.
//   9. UPHStrings, golden :12277 `fShowBinSelect->UPH_StringGrid->Cells[3][1]` --
//      fShowBinSelect (golden TfShowBinSelect, cShowBinSelect.h) has NO stand-in
//      anywhere in the port at all (zero hits, any header or source). No offline
//      UI thread ever populates that grid cell, so it is faithfully in the SAME
//      state golden's own empty-cell arm already handles (`Cells[3][1]==""` ->
//      return "0"); whole body collapses to that arm rather than gating an
//      unreachable branch in isolation.
//  10. IndexCycleTimeStrings, golden :12298 `fObserver->TimeInfoGrid->Cells[4][11]`
//      -- fObserver (golden TfObserver, cObserver.h) has no forms/ facade; the
//      only existing `fObserver` stand-in (atester_shims.h TfObserverShim) carries
//      labFactory/memoLotSummary only, not TimeInfoGrid. Same "collapses to
//      golden's own empty-cell fallback" reasoning as gate #9.
//  11. SetMaxTest, golden :12471 `FTestIF->edMaxTestTime->Text=...` -- FTestIF
//      (golden TFTestIF, cTesterIF.h, a whole untranslated VCL form) has no
//      forms/ facade; the only `FTestIF` in the port (SCK_ART_Remainder.h's gate
//      #2 note) carries only ->ReadTestIFFile(), not edMaxTestTime. Pure
//      UI-mirror write: TestIF_File.iMaxTime and the WriteIniData() persistence
//      either side of it are both real and un-gated.
//  12. SetMaxInitialTest, golden :12494 `FTestIF->edInitialMaxTest->Text=...` --
//      same FTestIF absence as gate #11.
//
//  Two symbols LOOKED like gates at first grep but resolved to real,
//  already-verified literals instead (documented at their file-scope declaration
//  site above the GROUP A1 marker, NOT counted in the 13 above): CONTACT_TEST
//  (golden cContact.cpp:77, `const int CONTACT_TEST=3;`, used by ArmStatusStrings
//  and WriteArmForce) and UserOffSet (golden uTemp_Set.cpp:51, `const int
//  UserOffSet=3;`, used by TempOfsStrings). Both are compile-time integer
//  constants compared against REAL live globals (iContactMode /
//  Temperature.fTempOffSet); supplying the missing named literal is not a
//  behavior change, so these two methods are counted ACTIVE above, not
//  GATED-partial. fNote (GetCZJamCode) similarly turned out to have a real,
//  already-translated forms/fNote.h facade (fShow/edErrorCode both present) --
//  simply needed its #include added, no gate at all.
//
//  STUB COLLISIONS  (per this wave's task brief -- retire in the NEXT integration
//  pass, NOT this one; this wave is barred from touching forms/fMain.cpp)
//  ------------------------------------------------------------------------------
//    ArmStatusStrings            -- offline stub at forms/fMain.cpp:284 (`return AnsiString("");`)
//    WritePERSITETemperature     -- offline body at forms/fMain.cpp:303-316 (already a faithful
//                                    wrapper identical in shape to this file's copy)
//    PERSITETemperatureStrings   -- offline body at forms/fMain.cpp:320-323 (`return
//                                    W906_PERSITETemperatureStrings_Sim;`)
//  Re-verified by `grep -rn "TfMain::<name>\b" --include=*.cpp .` for all 63 names,
//  20260817 22:39 -- these 3 are the ONLY existing bodies anywhere in the port
//  tree; no other collision exists. Once Command.cpp is added to CMakeLists.txt,
//  these 3 forms/fMain.cpp bodies must be deleted (linker will otherwise report
//  "multiple definition") -- the W906_PERSITETemperatureStrings_Sim seam member
//  in forms/fMain.h can stay (it is still referenced nowhere in Command.cpp's own
//  GATED-partial PERSITETemperatureStrings body, which computes its own iArm/Str
//  from real globals rather than returning a canned Sim string).
//
//  INTEGRATION STATUS (read before running build.bat gate)
//  ------------------------------------------------------------------------------
//  Command.cpp is NOT YET added to any CMakeLists.txt target (out of this wave's
//  write scope by the task brief -- "本檔尚未進 CMakeLists，主迴圈做"). Because
//  forms/fMain.h now declares 60 new `virtual` TfMain members with NO body
//  anywhere in the CURRENT build graph (their only bodies live in this
//  not-yet-registered file), `build.bat gate` will presently FAIL AT LINK TIME
//  (not compile time) for every target that links ht9045_forms, with ~28 targets
//  each reporting ~60 `undefined reference to TfMain::<name>` against
//  `libht9045_forms.a(fMain.cpp.obj)`'s vtable -- reproduced this pass, full log
//  at build_last.log. This is an EXPECTED, INHERENT consequence of the two-file
//  write mandate (fMain.h needs the declarations for Command.cpp to compile as
//  member-function definitions; CMakeLists.txt integration is explicitly a later
//  pass's job) -- NOT a sign this wave's translation is wrong. Every .cpp in the
//  tree still COMPILES cleanly against the updated fMain.h (verified: the gate
//  run's only errors are `ld returned 1 exit status` / `undefined reference`,
//  zero `error:` compile diagnostics). The fix for the next pass: add Command.cpp
//  next to forms/fMain.cpp in whichever CMakeLists.txt library target lists it
//  (ht9045_forms), then delete the 3 STUB COLLISION bodies from forms/fMain.cpp.
// =============================================================================
#include "MachineDefine.h"
#include "forms/fMain.h"
#include "cprod.h"
#include "cmydef.h"
#include "MachineType.h"
#include "aHotPlateSubstrate.h"    // TMyKitSuck/TestSocket (NOT mykitsuck.h -- see KNOWLEDGE.md ODR gotcha)
#include "CosFunction.h"
#include "MessageDef.h"
#include "cpublic.h"
#include "common.h"                // WriteIniData / GetLastOpenFN
#include "canary_support.h"        // LastSet / ShowMyMessage / ShowErrorMessage / RecordProcess
#include "acarry_shims.h"          // ATC_InterfaceForm (iATC_MODE_TYPE only -- see GATE REGISTER)
#include "csystem.h"                // HasICUnderMachine
#include "cContact.h"               // CONTACT_NORMAL (CONTACT_TEST is NOT here -- see GATE REGISTER)
#include "cinitial.h"                // SetWorkParameter
#include "mysensor.h"                // Sen[] / SnAirIsEnough
#include "forms/fNote.h"             // fNote->fShow / ->edErrorCode (real facade, offline fShow=false)
//---------------------------------------------------------------------------

// AI(W906-FW3-WA) 20260817: golden Command.cpp file-scope global (golden :?, right
// after `#pragma package(smart_init)`, re-read from the cp950-decoded golden file
// head this pass): `AnsiString asTempArmOrder[2][16];` -- the per-arm/per-site GPIB
// temperature-string cache RefreshTempData() fills and every Write*/Get*Strings
// reader below consumes. Not a class member, not in any golden header -- this file
// is its ONE golden definition site, so it is declared here, matching golden exactly.
AnsiString asTempArmOrder[2][16];

// AI(W906-FW3-WA) 20260817: golden main.h declares NewRecordProcess as a free
// function (golden main.h -- see cMyDB.h's own HOMECOMING NOTICE); its ACTUAL
// definition lives in acatchtray_shims.cpp (external linkage, already reachable
// at link time -- confirmed by `grep -rn "^void NewRecordProcess" acatchtray_shims.cpp`,
// 20260817). Declared directly here (rather than #include "cMyDB.h" or
// "acatchtray_shims.h") because cMyDB.h's OWN declaration of RecordProcess collides
// with canary_support.h's (both give parameter 2 a default argument in the same TU
// -- "default argument given for parameter 2 of RecordProcess", g++ -fsyntax-only,
// reproduced 20260817, matches the SAME collision ainarm9045.cpp:7459-7463 already
// documents) -- pulling in the whole of either shim header is unnecessary just for
// this one declaration.
void NewRecordProcess(AnsiString AlarmCode, AnsiString S, AnsiString Debug=" ");

// AI(W906-FW3-WA) 20260817: golden cContact.cpp:77 "const int CONTACT_TEST=3;".
// Port's cContact.h (already included above) carries CONTACT_NORMAL/
// CONTACT_MANUAL_GET_HEIGHT/CONTACT_DEVICE_MAP_CHECK from that SAME golden file
// but NOT this one (grep -rn "CONTACT_TEST" --include=*.h . -- 20260817 22:39 --
// the only hits are BarCode/BarCode_Shuttle2_CCDScan.h's own TU-LOCAL internal-
// linkage copy, a different TU, not included here). Re-declared here as a plain
// file-scope const int (internal linkage, same idiom cContact.h itself uses for
// its three), NOT a GATE -- iContactMode is a real, live global and this is just
// the missing named literal it is compared against; no ODR risk since this TU
// never includes BarCode_Shuttle2_CCDScan.h.
const int CONTACT_TEST = 3;

// AI(W906-FW3-WA) 20260817: golden uTemp_Set.cpp:51 `const int UserOffSet=3;` --
// a fixed column index into Temperature.fTempOffSet[19][tcTotalCount] (cprod.h:1388,
// confirmed real). uTemp_Set.h/.cpp (the temperature-offset-table VCL form) is not
// in the port at all (find . -iname "*Temp_Set*" -- 20260817 22:39 -- only dfm2rc
// layout artifacts for the FORM, no translated logic header). Not a GATE -- this is
// just the missing named literal for a real array index, same idiom as CONTACT_TEST
// above.
const int UserOffSet = 3;

/* ===================== GROUP A1 ===================== */

/* ---- golden Command.cpp:86-933 ---- */
int TfMain::RefreshTempData(bool bTransfer, int iArm, int iSite)     //JerryYang 20190812 新增sitemap to 溫控器轉換, 輸入site,輸出Heater位置
{
    AnsiString asNULL = "NULL";
    int iTempKit0_Arm1=0, iTempKit1_Arm1=0, iTempKit0_Arm2=0, iTempKit1_Arm2=0;
    ZeroMemory(asTempArmOrder, sizeof(asTempArmOrder));
    int j2, iHead=-1;
    bool bSendTmp=(IniConfig.bL17HeadHeaterOnWhenCloseSite &                    //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                    IniConfig.bVTESTFunction);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<16; j++)
        {
            asTempArmOrder[i][j]=asNULL;
        }
    }

    if(Temperature.iIndexHeatMode==ChamberOnly ||
       Temperature.iIndexHeatMode==SocketChamber)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)                                 //wei 20160526 temp for sitemap
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)                                //Steven 20190604 : Fixed for記憶體破壞
                {
                    if(bTestSiteUse[0][i][j]==true)
                    {
                        asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[tcChamber];
                    }
                    else
                    {
                        asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                    }

                    if(bTestSiteUse[1][i][j]==true ||
                       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                        TestIF_File.bArm1PickPlaceArm2Test==true &&
                        bTestSiteUse[0][i][j]==true))                           //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題
                    {
                        asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[tcChamber];
                    }
                    else
                    {
                        asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                    }
                }
            }
        }
        iHead=-1;
    }
    else
    {
        if(TestIF_File.iTestMode==QualSite2X2N)                                 //Steven 20231018 : Fixed for 2x2 NN mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else if(TestIF_File.iTestMode==_6Site2X3N)                              //Steven 20220425 : 2X3NN Mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcAc2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcAc1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else if(TestIF_File.iTestMode==_8Site2X4N)                              //Wei 20231211 : 2X4NN Mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcAc2]:asNULL;
                if(TestIF_File.iSiteMap[0][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][3]-1]=(bTestSiteUse[1][0][3]==true)?asGPIBTempShow[tcAd2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcAc1]:asNULL;
                if(TestIF_File.iSiteMap[1][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][3]-1]=(bTestSiteUse[0][0][3]==true)?asGPIBTempShow[tcAd1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[0][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][3]-1]=(bTestSiteUse[1][0][3]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcHead2]:asNULL;
                if(TestIF_File.iSiteMap[1][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][3]-1]=(bTestSiteUse[0][0][3]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else
        {
            for(int i=0; i<MAX_Index_Row; i++)                                  //wei 20160526 temp for sitemap
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(TestIF_File.iSiteMap[i][j]>0)
                    {
                        iTempKit0_Arm1=tcAa1+(i*4+j);
                        iTempKit0_Arm2=tcAa2+(i*4+j);
                        if(TestIF_File.iTestMode==QualSite1X4)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.b2CableLayoutKit ||
                                   (ATC_SYSTEM==eNewATCSystem &&
                                    Temperature.bATCActiveCooling==true))
                                {
                                }
                                else if(TestIF_File.b1CableLayoutKit)
                                {
                                    if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                    {
                                        iTempKit0_Arm1=tcAa1;
                                    }
                                    else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                    {
                                        iTempKit0_Arm1=tcBa1;
                                    }

                                    if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                    {
                                        iTempKit0_Arm2=tcAa2;
                                    }
                                    else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                    {
                                        iTempKit0_Arm2=tcBa2;
                                    }
                                }
                                else  //三條線的版本
                                {
                                    if(iTempKit0_Arm1==tcAb1)
                                        iTempKit0_Arm1=tcBa1;
                                    else if(iTempKit0_Arm1==tcAc1)
                                        iTempKit0_Arm1=tcAb1;
                                    else if(iTempKit0_Arm1==tcAd1)
                                        iTempKit0_Arm1=tcBb1;

                                    if(iTempKit0_Arm2==tcAb2)
                                        iTempKit0_Arm2=tcBa2;
                                    else if(iTempKit0_Arm2==tcAc2)
                                        iTempKit0_Arm2=tcAb2;
                                    else if(iTempKit0_Arm2==tcAd2)
                                        iTempKit0_Arm2=tcBb2;
                                }
                            }
                            else    //4組溫控器
                            {
                                if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                {
                                    iTempKit0_Arm1=tcHead1;
                                }
                                else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                {
                                    iTempKit0_Arm1=tcHead2;
                                }

                                if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                {
                                    iTempKit0_Arm2=tcHead3;
                                }
                                else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                {
                                    iTempKit0_Arm2=tcHead4;
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true ||
                            (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                            TestIF_File.bArm1PickPlaceArm2Test==true &&
                            bTestSiteUse[0][i][j]==true))                       //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        //QQ 20230214 : 2x1
                        else if(TestIF_File.iTestMode==QualSite2X2)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.bSquare_OctalKit)                //JerryYang 20181003 2x2 用8 site kit要轉換
                                {
                                    if(iTempKit0_Arm1==tcAa1)
                                        iTempKit0_Arm1=tcAb1;
                                    else if(iTempKit0_Arm1==tcAb1)
                                        iTempKit0_Arm1=tcAc1;
                                    else if(iTempKit0_Arm1==tcBa1)
                                        iTempKit0_Arm1=tcBb1;
                                    else if(iTempKit0_Arm1==tcBb1)
                                        iTempKit0_Arm1=tcBc1;

                                    if(iTempKit0_Arm2==tcAa2)
                                        iTempKit0_Arm2=tcAb2;
                                    else if(iTempKit0_Arm2==tcAb2)
                                        iTempKit0_Arm2=tcAc2;
                                    else if(iTempKit0_Arm2==tcBa2)
                                        iTempKit0_Arm2=tcBb2;
                                    else if(iTempKit0_Arm2==tcBb2)
                                        iTempKit0_Arm2=tcBc2;
                                }
                            }
                            else
                            {
                                if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcBa1)
                                {
                                    iTempKit0_Arm1=tcHead1;
                                }
                                else if(iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBb1)
                                {
                                    iTempKit0_Arm1=tcHead2;
                                }

                                if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcBa2)
                                {
                                    iTempKit0_Arm2=tcHead3;
                                }
                                else if(iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBb2)
                                {
                                    iTempKit0_Arm2=tcHead4;
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true ||
                            (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                            TestIF_File.bArm1PickPlaceArm2Test==true &&
                            bTestSiteUse[0][i][j]==true))                       //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        else if(TestIF_File.iTestMode==_8Site2X4 ||
                                TestIF_File.iTestMode==_6Site2X3 ||
                                TestIF_File.iTestMode==TriSite1X3 ||
                                TestIF_File.iTestMode==DualSite ||
                                TestIF.iTestMode==SingleSite)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.iTestMode==_8Site2X4)
                                {
                                    if(TestIF_File.bNS8000CS==true)
                                    {
                                        if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                        {
                                            iTempKit0_Arm1=tcAa1;
                                        }
                                        else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1 || iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                        {
                                            iTempKit0_Arm1=tcAb1;
                                        }

                                        if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                        {
                                            iTempKit0_Arm2=tcAa2;
                                        }
                                        else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2 || iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                        {
                                            iTempKit0_Arm2=tcAb2;
                                        }
                                    }
                                    else if(TestIF_File.dSiteXPitch<=30)
                                    {
                                        //     a  b  c  d     一組加熱棒對2 site
                                        //   **************
                                        //  A* Aa  *  Ab  *
                                        //   **************
                                        //  B* Ba  *  Bb  *
                                        //   **************
                                        if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                        {
                                            iTempKit0_Arm1=tcAa1;
                                        }
                                        else if(iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                        {
                                            iTempKit0_Arm1=tcBa1;
                                        }
                                        else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                        {
                                            iTempKit0_Arm1=tcAb1;
                                        }
                                        else if(iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                        {
                                            iTempKit0_Arm1=tcBb1;
                                        }

                                        if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                        {
                                            iTempKit0_Arm2=tcAa2;
                                        }
                                        else if(iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                        {
                                            iTempKit0_Arm2=tcBa2;
                                        }
                                        else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                        {
                                            iTempKit0_Arm2=tcAb2;
                                        }
                                        else if(iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                        {
                                            iTempKit0_Arm2=tcBb2;
                                        }
                                    }
                                    else if(TestIF_File.bOctal_12Kit &&
                                            (USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08) &&
                                            IniConfig.bSPILFunction==true)
                                    {
                                        if(iTempKit0_Arm1==tcAa1)
                                            iTempKit0_Arm1=tcAb1;
                                        else if(iTempKit0_Arm1==tcAb1)
                                            iTempKit0_Arm1=tcAc1;
                                        else if(iTempKit0_Arm1==tcAc1)
                                            iTempKit0_Arm1=tcAd1;
                                        else if(iTempKit0_Arm1==tcAd1)
                                            iTempKit0_Arm1=tcAe1;
                                        else if(iTempKit0_Arm1==tcBa1)
                                            iTempKit0_Arm1=tcBb1;
                                        else if(iTempKit0_Arm1==tcBb1)
                                            iTempKit0_Arm1=tcBc1;
                                        else if(iTempKit0_Arm1==tcBc1)
                                            iTempKit0_Arm1=tcBd1;
                                        else if(iTempKit0_Arm1==tcBd1)
                                            iTempKit0_Arm1=tcBe1;

                                        if(iTempKit0_Arm2==tcAa2)
                                            iTempKit0_Arm2=tcAb2;
                                        else if(iTempKit0_Arm2==tcAb2)
                                            iTempKit0_Arm2=tcAc2;
                                        else if(iTempKit0_Arm2==tcAc2)
                                            iTempKit0_Arm2=tcAd2;
                                        else if(iTempKit0_Arm2==tcAd2)
                                            iTempKit0_Arm2=tcAe2;
                                        else if(iTempKit0_Arm2==tcBa2)
                                            iTempKit0_Arm2=tcBb2;
                                        else if(iTempKit0_Arm2==tcBb2)
                                            iTempKit0_Arm2=tcBc2;
                                        else if(iTempKit0_Arm2==tcBc2)
                                            iTempKit0_Arm2=tcBd2;
                                        else if(iTempKit0_Arm2==tcBd2)
                                            iTempKit0_Arm2=tcBe2;
                                    }
                                    else if(TestIF_File.bOctal_16Kit &&
                                            (USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08)) //JerryYang 20230814 : fix 2X4 mode使用2X8 SLK, 收到Remote temperature溫度offset補錯位置的問題
                                    {
                                        if(iTempKit0_Arm1==tcAa1)
                                            iTempKit0_Arm1=tcAc1;
                                        else if(iTempKit0_Arm1==tcAb1)
                                            iTempKit0_Arm1=tcAd1;
                                        else if(iTempKit0_Arm1==tcAc1)
                                            iTempKit0_Arm1=tcAe1;
                                        else if(iTempKit0_Arm1==tcAd1)
                                            iTempKit0_Arm1=tcAf1;
                                        else if(iTempKit0_Arm1==tcBa1)
                                            iTempKit0_Arm1=tcBc1;
                                        else if(iTempKit0_Arm1==tcBb1)
                                            iTempKit0_Arm1=tcBd1;
                                        else if(iTempKit0_Arm1==tcBc1)
                                            iTempKit0_Arm1=tcBe1;
                                        else if(iTempKit0_Arm1==tcBd1)
                                            iTempKit0_Arm1=tcBf1;

                                        if(iTempKit0_Arm2==tcAa2)
                                            iTempKit0_Arm2=tcAc2;
                                        else if(iTempKit0_Arm2==tcAb2)
                                            iTempKit0_Arm2=tcAd2;
                                        else if(iTempKit0_Arm2==tcAc2)
                                            iTempKit0_Arm2=tcAe2;
                                        else if(iTempKit0_Arm2==tcAd2)
                                            iTempKit0_Arm2=tcAf2;
                                        else if(iTempKit0_Arm2==tcBa2)
                                            iTempKit0_Arm2=tcBc2;
                                        else if(iTempKit0_Arm2==tcBb2)
                                            iTempKit0_Arm2=tcBd2;
                                        else if(iTempKit0_Arm2==tcBc2)
                                            iTempKit0_Arm2=tcBe2;
                                        else if(iTempKit0_Arm2==tcBd2)
                                            iTempKit0_Arm2=tcBf2;
                                    }
                                }
                            }
                            else
                            {
                                if(TestIF_File.iTestMode==_8Site2X4)
                                {
                                    if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                    {
                                        iTempKit0_Arm1=tcHead1;
                                    }
                                    else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1 || iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                    {
                                        iTempKit0_Arm1=tcHead2;
                                    }

                                    if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                    else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2 || iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                    {
                                        iTempKit0_Arm2=tcHead4;
                                    }
                                }
                                else if(TestIF_File.iTestMode==DualSite || TestIF_File.iTestMode==SingleSite)
                                {
                                    if(iTempKit0_Arm1==tcAa1)
                                    {
                                        iTempKit0_Arm1=tcHead1;
                                    }
                                    else if(iTempKit0_Arm1==tcAb1)
                                    {
                                        iTempKit0_Arm1=tcHead2;
                                    }

                                    if(iTempKit0_Arm2==tcAa2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                    else if(iTempKit0_Arm2==tcAb2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true || bSendTmp)         //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true|| bSendTmp ||
                            (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                            TestIF_File.bArm1PickPlaceArm2Test==true &&
                            bTestSiteUse[0][i][j]==true))                       //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題  //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(TestIF_File.iTestMode==_10Site2X5 ||             //Steven 20220104 : fixed for 2x5mode
                               TestIF_File.iTestMode==_12Site2X6 ||
                               TestIF_File.iTestMode==_16Site2X8)
                            {
                                if(USE_16_HEATER==eht16Heater       ||          //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                                   USE_16_HEATER==eht16HeaterEJ1N   ||
                                   USE_16_HEATER==eht32HeaterEJ1N   ||          //Steven 20140923 : Index使用EJ1N版32組加熱器
                                   USE_16_HEATER==eht32HeaterKT4H   ||          //Steven 20150211 : Index使用KT4H版32組加熱器
                                   USE_16_HEATER==eht16HeaterDTME08 ||          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                                   USE_16_HEATER==eht32HeaterDTME08 )           //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                                {
//                                  tcAa1=11, tcAb1=12, tcAc1=13, tcAd1=14, tcBa1=15, tcBb1=16, tcBc1=17, tcBd1=18,
//                                  tcAe1=33, tcAf1=34, tcAg1=35, tcAh1=36, tcBe1=37, tcBf1=38, tcBg1=39, tcBh1=40,

//                                  tcAa2=19, tcAb2=20, tcAc2=21, tcAd2=22, tcBa2=23, tcBb2=24, tcBc2=25, tcBd2=26,
//                                  tcAe2=41, tcAf2=42, tcAg2=43, tcAh2=44, tcBe2=45, tcBf2=46, tcBg2=47, tcBh2=48,

                                    if(j<4)
                                    {
                                        if((USE_16_HEATER==eht16Heater ||
                                            USE_16_HEATER==eht16HeaterEJ1N ||
                                            USE_16_HEATER==eht16HeaterDTME08) || //JerryYang 20181002 1組加熱棒對應2個site
                                           ((USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08) &&   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                                             TestIF_File.bUse32Heater==false) ||
                                           (ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true && TestIF_File.bUse32Heater==false))
                                        {
                                            if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                            {
                                                iTempKit0_Arm1=tcAa1;
                                            }
                                            else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                            {
                                                iTempKit0_Arm1=tcAb1;
                                            }
                                            else if(iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                            {
                                                iTempKit0_Arm1=tcBa1;
                                            }
                                            else if(iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                            {
                                                iTempKit0_Arm1=tcBb1;
                                            }

                                            if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                            {
                                                iTempKit0_Arm2=tcAa2;
                                            }
                                            else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                            {
                                                iTempKit0_Arm2=tcAb2;
                                            }
                                            else if(iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                            {
                                                iTempKit0_Arm2=tcBa2;
                                            }
                                            else if(iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                            {
                                                iTempKit0_Arm2=tcBb2;
                                            }
                                        }

                                        if(bTestSiteUse[0][i][j]==true)
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                                        }
                                        else
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTestSiteUse[1][i][j]==true ||
                                        (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                                        TestIF_File.bArm1PickPlaceArm2Test==true &&
                                        bTestSiteUse[0][i][j]==true))           //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                                        }
                                        else
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTransfer==true)
                                        {
                                            if(TestIF_File.iSiteMap[i][j]==iSite)
                                            {
                                                if(iArm==1)
                                                {
                                                    iHead=iTempKit0_Arm1;
                                                }
                                                else if(iArm==2)
                                                {
                                                    iHead=iTempKit0_Arm2;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        j2=j-4;
                                        iTempKit1_Arm1=tcAe1+(i*4+j2);
                                        iTempKit1_Arm2=tcAe2+(i*4+j2);

                                        if((USE_16_HEATER==eht16Heater ||
                                            USE_16_HEATER==eht16HeaterEJ1N ||
                                            USE_16_HEATER==eht16HeaterDTME08) ||       //JerryYang 20181002 1組加熱棒對應2個site
                                          ((USE_16_HEATER==eht32HeaterEJ1N ||
                                            USE_16_HEATER==eht32HeaterKT4H ||
                                            USE_16_HEATER==eht32HeaterDTME08) &&
                                            TestIF_File.bUse32Heater==false) ||
                                          (ATC_SYSTEM==eNewATCSystem &&
                                           Temperature.bATCActiveCooling==true &&
                                           TestIF_File.bUse32Heater==false))
                                        {
                                            if(iTempKit1_Arm1==tcAe1 || iTempKit1_Arm1==tcAf1)
                                            {
                                                iTempKit1_Arm1=tcAc1;
                                            }
                                            else if(iTempKit1_Arm1==tcAg1 || iTempKit1_Arm1==tcAh1)
                                            {
                                                iTempKit1_Arm1=tcAd1;
                                            }
                                            else if(iTempKit1_Arm1==tcBe1 || iTempKit1_Arm1==tcBf1)
                                            {
                                                iTempKit1_Arm1=tcBc1;
                                            }
                                            else if(iTempKit1_Arm1==tcBg1 || iTempKit1_Arm1==tcBh1)
                                            {
                                                iTempKit1_Arm1=tcBd1;
                                            }

                                            if(iTempKit1_Arm2==tcAe2 || iTempKit1_Arm2==tcAf2)
                                            {
                                                iTempKit1_Arm2=tcAc2;
                                            }
                                            else if(iTempKit1_Arm2==tcAg2 || iTempKit1_Arm2==tcAh2)
                                            {
                                                iTempKit1_Arm2=tcAd2;
                                            }
                                            else if(iTempKit1_Arm2==tcBe2 || iTempKit1_Arm2==tcBf2)
                                            {
                                                iTempKit1_Arm2=tcBc2;
                                            }
                                            else if(iTempKit1_Arm2==tcBg2 || iTempKit1_Arm2==tcBh2)
                                            {
                                                iTempKit1_Arm2=tcBd2;
                                            }
                                        }

                                        if(bTestSiteUse[0][i][j]==true)
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit1_Arm1];
                                        }
                                        else
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTestSiteUse[1][i][j]==true ||
                                        (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                                        TestIF_File.bArm1PickPlaceArm2Test==true &&
                                        bTestSiteUse[0][i][j]==true))           //KevinCheng 20260330 : 修改 GPIB Arm2 回傳NULL的問題
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit1_Arm2];
                                        }
                                        else
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTransfer==true)
                                        {
                                            if(TestIF_File.iSiteMap[i][j]==iSite)
                                            {
                                                if(iArm==1)
                                                {
                                                    iHead=iTempKit1_Arm1;
                                                }
                                                else if(iArm==2)
                                                {
                                                    iHead=iTempKit1_Arm2;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //QQ 20230214 : 4x4
                        //QQ 20230214 : 4x8
                    }
                }
            }
        }
    }
    return iHead;
}

/* ---- golden Command.cpp:935-943 ---- */
void TfMain::WritePERSITETemperature()                               //JerryYang 20181003  (Steven) : 回傳各Site溫度改為依照Site Map順序回覆
{
    AnsiString sRet="";
    sRet=PERSITETemperatureStrings();
    if(TestIF.iTestType==TCP_IP_MODE)                                           //wei 20211027 open short TCP/IP
        asTCPIPTemperature.sprintf("%s\r", sRet);                               //Sam 20231205 : 修正 NOVATEK 指令
    else
        SendMSG_CMD(MSG_CMD_TempArm, sRet+"\r");                                //Sam 20231205 : 修正 NOVATEK 指令
}

/* ---- golden Command.cpp:945-1482 ---- */
AnsiString TfMain::PERSITETemperatureStrings()                       //Sam 20220408 : Novatek 新增 SET_ALL?
{
    //Head1 //Head2 //Head5 //Head6
    AnsiString t="";
    AnsiString asSite = "NULL";                                                 //Ifor 20160130 Add 回覆字串改變數處理
    int iArm=-1;
    AnsiString Str="";

    RefreshTempData();

    //2013.01.24 Q_Q TSMC GPIB COMMAND Part 2. {
    // GATE(FW3-WA) golden :956 `if(fContact->fShow)` -- TfContact/fContact has NO
    // translated home anywhere in the port (grep -rn "TfContact \*fContact"
    // --include=*.h --include=*.cpp . returns zero hits outside cContact.h's own
    // file-scope comments describing the hazard; confirmed 20260817 22:39).
    // cContact.cpp is the 22761-line out-of-scope VCL form per atester.cpp's own
    // established gate precedent (atester.cpp banner: "the contact-mode form
    // fContact ... -> atester_shims"). WHY GATING TO THE ELSE ARM IS FAITHFUL:
    // fShow can only be raised by that form's own UI thread running a live
    // Contact-mode dialog; offline there is no such thread, so "fShow is never
    // true" is the same "no such subsystem is live offline" posture used
    // throughout this tree (TfMainInplace::InArmPlacementEnable()==false, etc.),
    // not a behavior guess. The ELSE branch below (IndexStatus/iContactMode/
    // iIndexArm/CONTACT_NORMAL -- all real, confirmed available) is translated
    // ACTIVE and unconditional.
#if 0
    if(fContact->fShow)                                                         //Ifor 20240612 add:避免Contact Mode 資料回覆錯誤
    {
        if(iWhichArmDown==1)
        {
            iArm=0;
            Str="iWhichArmDown==1";
        }
        else if(iWhichArmDown==2)
        {
            iArm=1;
            Str="iWhichArmDown==2";
        }
        else
        {
            iArm=-1;
            Str="iArm=-1(fContact)";
        }
    }
    else
#endif
    {
        if(IndexStatus==Z1Down_Z2Up || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            iArm=0;
            Str="Z1Down_Z2Up";
        }
        else if(IndexStatus==Z1Up_Z2Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            iArm=1;
            Str="Z1Up_Z2Down";
        }
        else if(IndexStatus==Z1_Z2_Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))  //JerryYang 20201101 : 修正GPIB傳輸溫度命令異常
        {
            iArm=0;
            Str="Z1_Z2_Down";
        }
        else
        {
            iArm=-1;
            Str="iArm=-1";
        }
    }

    if(TestIF_File.iTestMode==SingleSite)                                       //Ifor 20160130 add 海思 SingleSite GPIB溫度上報格式
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="SINGLESITE";                                                //Ifor 20160310 後面無site需加上"_"+溫度
        }
        else
        {
            asSite="SINGLESITE_";
        }

        if(iArm==-1)
        {
            t.sprintf("%sNULL_", asSite);                                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s%s_", asSite, asTempArmOrder[iArm][0]);                //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
    {                                                                           //Ifor 20160130 add 海思GPIB Dual Site 1x2 資料格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="DUAL1X2";
        }
        else
        {
            asSite="DUAL_1X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_", asSite);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_", asSite,
                                   asTempArmOrder[iArm][0],
                                   asTempArmOrder[iArm][1]);                    //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {                                                                           //wei 20160512 add TSMC 1X3 TempArm
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="TRI1X3";
        }
        else
        {
            asSite="TRI_1X3";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_", asSite);                            //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_", asSite,
                                        asTempArmOrder[iArm][0],
                                        asTempArmOrder[iArm][1],
                                        asTempArmOrder[iArm][2]);               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4)                                 // Qual Site 1x4
    {                                                                           //Ifor 20160130 add Qual Site 1x4 海思GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="QUAD1X4";
        }
        else
        {
            asSite="QUAD_1X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_", asSite,
                                           asTempArmOrder[iArm][0],
                                           asTempArmOrder[iArm][1],
                                           asTempArmOrder[iArm][2],
                                           asTempArmOrder[iArm][3]);            //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 //wei 20170417 (Steven) : Fixed for Dual Site 2x1
    {                                                                           //Ifor 20160130 add 海思GPIB Dual Site 1x2 資料格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="DUAL2X1";
        }
        else
        {
            asSite="DUAL_2X1";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_", asSite);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_", asSite,
                                   asTempArmOrder[iArm][0],
                                   asTempArmOrder[iArm][1]);                    //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite2X2)                                 // Qual Site 2x2
    {                                                                           //Ifor 20160130 add 海思 Qual Site 2x2 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="QUAD2X2";
        }
        else
        {
            asSite="QUAD_2X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_", asSite,
                                           asTempArmOrder[iArm][0],
                                           asTempArmOrder[iArm][1],
                                           asTempArmOrder[iArm][2],
                                           asTempArmOrder[iArm][3]);            //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)
    {                                                                           //Ifor 20160130 add 海思 Qual Site 2x2 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="QUAD2X2";
            else
                asSite="QUAD2X2N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="QUAD_2X2";
            else
                asSite="QUAD_2X2N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_\r", asSite,
                                           asTempArmOrder[0][0],
                                           asTempArmOrder[0][1],
                                           asTempArmOrder[0][2],
                                           asTempArmOrder[0][3]);               //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_6Site2X3)
    {                                                                           //Ifor 20160130 add 海思 6 Site 2X3 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="6SITE2X3";
        }
        else
        {
            asSite="6SITE_2X3";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);             //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                 asTempArmOrder[iArm][0],
                                                 asTempArmOrder[iArm][1],
                                                 asTempArmOrder[iArm][2],
                                                 asTempArmOrder[iArm][3],
                                                 asTempArmOrder[iArm][4],
                                                 asTempArmOrder[iArm][5]);      //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="6SITE2X3";
            else
                asSite="6SITE2X3N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="6SITE_2X3";
            else
                asSite="6SITE_2X3N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);             //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                 asTempArmOrder[0][0],
                                                 asTempArmOrder[0][1],
                                                 asTempArmOrder[0][2],
                                                 asTempArmOrder[0][3],
                                                 asTempArmOrder[0][4],
                                                 asTempArmOrder[0][5]);         //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="8SITE2X4";
            else
                asSite="8SITE2X4N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="8SITE_2X4";
            else
                asSite="8SITE_2X4N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                     asTempArmOrder[0][0],
                                                     asTempArmOrder[0][1],
                                                     asTempArmOrder[0][2],
                                                     asTempArmOrder[0][3],
                                                     asTempArmOrder[0][4],
                                                     asTempArmOrder[0][5],
                                                     asTempArmOrder[0][6],
                                                     asTempArmOrder[0][7]);     //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_8Site2X4)                                   // 8 Site 2x4
    {                                                                           //Ifor 20160130 add 海思 8 Site 2X4 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="8SITE2X4";
        }
        else
        {
            asSite="8SITE_2X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
            NewRecordProcess("", "GetNULLTemp!", Str);                          //RogerYang 20250924 : 紀錄Log
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7]);                               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {                                                                           //Ifor 20160130 add 海思 12 Site 2X6 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="10SITE5X2";
        }
        else
        {
            asSite="10SITE_5X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite); //Sam 20231205 : 修正 NOVATEK 指令    //Steven 20220104 : fixed for 2x5mode
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite ,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9]);                               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //12 Site //wei 20150702
    {                                                                           //Ifor 20160130 add 海思 12 Site 2X6 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="12SITE6X2";
        }
        else
        {
            asSite="12SITE_6X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite ,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9],
                        asTempArmOrder[iArm][10],
                        asTempArmOrder[iArm][11]);                              //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16 Site
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="16SITE8X2";
        }
        else
        {
            asSite="16SITE_8X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9],
                        asTempArmOrder[iArm][10],
                        asTempArmOrder[iArm][11],
                        asTempArmOrder[iArm][12],
                        asTempArmOrder[iArm][13],
                        asTempArmOrder[iArm][14],
                        asTempArmOrder[iArm][15]);                              //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_16Site4X4)                                  //16 Site
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="16SITE4X4";
        }
        else
        {
            asSite="16SITE_4X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[1][0],
                        asTempArmOrder[1][1],
                        asTempArmOrder[0][0],
                        asTempArmOrder[0][1],
                        asTempArmOrder[1][2],
                        asTempArmOrder[1][3],
                        asTempArmOrder[0][2],
                        asTempArmOrder[0][3],
                        asTempArmOrder[1][4],
                        asTempArmOrder[1][5],
                        asTempArmOrder[0][4],
                        asTempArmOrder[0][5],
                        asTempArmOrder[1][6],
                        asTempArmOrder[1][7],
                        asTempArmOrder[0][6],
                        asTempArmOrder[0][7]);                                  //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="32SITE4X8";
        }
        else
        {
            asSite="32SITE_4X8";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[1][0],
                        asTempArmOrder[1][1],
                        asTempArmOrder[0][0],
                        asTempArmOrder[0][1],
                        asTempArmOrder[1][2],
                        asTempArmOrder[1][3],
                        asTempArmOrder[0][2],
                        asTempArmOrder[0][3],
                        asTempArmOrder[1][4],
                        asTempArmOrder[1][5],
                        asTempArmOrder[0][4],
                        asTempArmOrder[0][5],
                        asTempArmOrder[1][6],
                        asTempArmOrder[1][7],
                        asTempArmOrder[0][6],
                        asTempArmOrder[0][7],
                        asTempArmOrder[1][8],
                        asTempArmOrder[1][9],
                        asTempArmOrder[0][8],
                        asTempArmOrder[0][9],
                        asTempArmOrder[1][10],
                        asTempArmOrder[1][11],
                        asTempArmOrder[0][10],
                        asTempArmOrder[0][11],
                        asTempArmOrder[1][12],
                        asTempArmOrder[1][13],
                        asTempArmOrder[0][12],
                        asTempArmOrder[0][13],
                        asTempArmOrder[1][14],
                        asTempArmOrder[1][15],
                        asTempArmOrder[0][14],
                        asTempArmOrder[0][15]);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else
    {
        t.sprintf("UNKNOWN\r");
    }

    return t;
}

/* ---- golden Command.cpp:1484-1487 ---- */
void TfMain::WriteHandlerID()                                        //kevin 20130425
{
    SendMSG_CMD(MSG_CMD_HandlerID, IniConfig.sGPIBMachineID);
}

/* ---- golden Command.cpp:1489-1495 ---- */
void TfMain::WriteArmStatus()
{
    AnsiString sRet="";
    sRet=ArmStatusStrings();
    sRet+="\r";                                                                 //Sam 20231205 : 修正 NOVATEK 指令
    SendMSG_CMD(MSG_CMD_TestArm, sRet);
}

/* ---- golden Command.cpp:1497-1508 ---- */
AnsiString TfMain::ArmStatusStrings()
{
    AnsiString t;
    if(IndexStatus==Z1_Z2_Down ||                                               //Steven 20230214 : Add GPIB for NN mode
       IndexStatus==Z1Down_Z2Up || iContactMode==CONTACT_TEST && iIndexArm==0)  //kevin 20310605 add contract mode
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"F":"1";                  //Sam 20231205 : 修正 NOVATEK 指令
    else if(IndexStatus==Z1Up_Z2Down || iContactMode==CONTACT_TEST && iIndexArm==1) //kevin 20310605 add contract mode
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"B":"2";                  //Sam 20231205 : 修正 NOVATEK 指令
    else
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"F":"0";                  //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20250414 : HANA ART Function
    return t;
}

/* ---- golden Command.cpp:1510-1541 ---- */
void TfMain::WriteArmForce()
{
    AnsiString t;

    if(IniConfig.bKoreaFunction==true)
    {
        t.sprintf("%s\r", asArmForce1.c_str());       //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN)             //wei 20151117 傳送ContactForce設定值
    {
        t.sprintf("%sT\r", asArmForce1.c_str());                                //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else
    {
        if(IndexStatus==Z1_Z2_Down ||                                               //Steven 20230214 : Add GPIB for NN mode
           IndexStatus==Z1Down_Z2Up || iContactMode==CONTACT_TEST && iIndexArm==0)  //kevin 20310605 add contract mode
        {
            t.sprintf("%s\r", asArmForce1.c_str());                             //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else if(IndexStatus==Z1Up_Z2Down || iContactMode==CONTACT_TEST && iIndexArm==1)    //kevin 20310605 add contract mode
        {
            t.sprintf("%s\r", asArmForce2.c_str());                             //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else
        {
            //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
            t.sprintf("%d\r", 0);
        }
    }

    SendMSG_CMD(MSG_CMD_Force, t);
}

/* ---- golden Command.cpp:1543-1549 ---- */
void TfMain::WriteTempData()                                         //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    t=TempDataStrings();
    t+=" \r\n";                                                                 //Sam 20231205 : 修正 NOVATEK 指令
    SendMSG_CMD(MSG_CMD_HandlerTemperature, t);
}

/* ---- golden Command.cpp:1551-1586 ---- */
AnsiString TfMain::TempDataStrings()                                 //Sam 20220408 : Novatek 新增 SET_ALL?
{
    AnsiString t;
    if(IniConfig.iI38SETTEMPRespondSetTemp==1)                                  //kevin 20180308 Settemp? 回傳需要 Settemp +25.0.
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
            t.sprintf("Settemp +%0.1f", Temperature.fAbitTemp);                 //Sam 20231205 : 修正 NOVATEK 指令
        else
            t.sprintf("Settemp +%s.0", edWorkTemperBase->Text.c_str());         //Sam 20231205 : 修正 NOVATEK 指令
    }
    else if(IniConfig.iI38SETTEMPRespondSetTemp==2)                             //Steven 20250701 : for DOOSAN TESNA
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
            t.sprintf("%d", int(Temperature.fAbitTemp));                        //Sam 20231205 : 修正 NOVATEK 指令
        else
            t.sprintf("%d", atoi(edWorkTemperBase->Text.c_str()));              //Sam 20231205 : 修正 NOVATEK 指令
    }
    else
    {
        if(TestIF.iGpibMode==InterfaceType_Delta_Castle)
        {
            if(LastSet.iTemperature==Tempture_Ambient)
                t.sprintf("25.0\n");                                            //kevin 20180305 strncpy(t, "+25.0", sizeof(t));
            else
                t.sprintf("%s.0\n", edWorkTemperBase->Text.c_str());            //kevin 20180305 sprintf(t, "+%s.0", edWorkTemperBase->Text.c_str(), t);
        }
        else
        {
            if(LastSet.iTemperature==Tempture_Ambient)
                t.sprintf("+25.0");                                                 //Sam 20231205 : 修正 NOVATEK 指令  //kevin 20180305 strncpy(t, "+25.0", sizeof(t));
            else
                t.sprintf("+%s.0", edWorkTemperBase->Text.c_str());                 //Sam 20231205 : 修正 NOVATEK 指令  //kevin 20180305 sprintf(t, "+%s.0", edWorkTemperBase->Text.c_str(), t);
        }
    }
    return t;
}

/* ---- golden Command.cpp:1588-1649 ---- */
void TfMain::WriteSetTempStatus()                                    //Steven 20110613 : Corn GPIB add from 7040
{
    int ret=-1, i, TempMode;
    AnsiString t;

    try                                                                         //kevin 20180320 add 避免資料轉換異常
    {
        i=atoi(HGpib2Handler->cReturn);
    }
    catch(...)
    {
        i=25;
        MyDBIProcess("Exception", "TfMain::WriteSetTempStatus");
    }

    if(IniConfig.iI38SETTEMPRespondSetTemp==1)                                  //kevin 20180308 Settemp? 回傳需要 Settemp +25.0.
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
        {
            if(i>50)
            {
                TempMode=1;
                Temperature.fAbitTemp=30;
            }
            else
            {
                TempMode=0;
            }
            ret=SetTemp(false, Temperature.fAbitTemp, atof(edSoakTime->Text.c_str()));
        }
        else
        {
            ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
        }

        // GATE(FW3-WA) golden :1596 `ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);`
        // -- ChangeTempMode is golden main.h:1323, a TfMain MEMBER function (bare-call
        // = implicit this->ChangeTempMode), NOT present in forms/fMain.h (grep -rn
        // "ChangeTempMode" forms/*.h -- 20260817 22:39 -- zero declarations, only a
        // prose mention inside SetTemp's own comment). Its real body (main.cpp, not
        // Command.cpp) walks a ShowMyMessageBox_YES_NO confirm + temp-mode UI cascade
        // with no facade path from this TU -- same "GATED LEAF, no hardware/UI to
        // fail" posture as the already-established W906_SetTemp_Sim (default 0 =
        // golden success code) this function's own SetTemp() call above resolves to.
        // Faithful offline default: ret stays whatever SetTemp() returned (0 on the
        // offline-success path), which already drives the SETTINGOK/SETTINGNG reply
        // below exactly as if ChangeTempMode had itself succeeded.
#if 0
        if(ret==0)
            ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);
#endif
    }
    else
    {
        if(i>25)
        {
            TempMode=1;
            edWorkTemperBase->Text=i;
        }
        else
        {
            TempMode=0;
        }
        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
    }

    // GATE(FW3-WA) golden :1607 -- same ChangeTempMode absence as above (see that
    // GATE's full citation); ret is left as SetTemp()'s own return value.
#if 0
    if(ret==0)
        ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);
#endif

    if(ret==0)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetTemp, t);
}

/* ---- golden Command.cpp:1651-1764 ---- */
void TfMain::WriteSetTestTempStatus()                                //Ifor 20210623 : //Ifor 20210623 add: Test Temp Change
{
    int iSetTemp;
    AnsiString t;

    try                                                                         //kevin 20180320 add 避免資料轉換異常
    {
        iSetTemp=atoi(HGpib2Handler->cReturn);
    }
    catch(...)
    {
        iSetTemp=0;
    }

    double dbSetATCTemp=0;
    int iNowDownArm=0;

    if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
        dbSetATCTemp=Temperature.fWorkTemperBase;
    else
        dbSetATCTemp=IniConfig.dATCAmbientTemperature;

    // GATE(FW3-WA) golden :1656 `if(fContact->fShow)` -- same fContact absence as
    // PERSITETemperatureStrings' identical construct above (see that function's
    // GATE for the full citation); faithful offline default is the ELSE arm.
#if 0
    if(fContact->fShow)                                                         //Ifor 20240612 add:避免Contact Mode 資料回覆錯誤
    {
        if(iWhichArmDown==1)
        {
            iNowDownArm=Z1Down_Z2Up;
        }
        else if(iWhichArmDown==2)
        {
            iNowDownArm=Z1Up_Z2Down;
        }
    }
    else
#endif
    {
        iNowDownArm=IndexStatus;
    }

    if(iSetTemp<25)
    {
        t.sprintf("SETTINGNG");
    }
    else
    {
        bChangeTest_TempOffset=iSetTemp-dbSetATCTemp;

        if(bChangeTest_TempOffset!=0)
        {
            bChangeTest_TempAlarm=true;
            // GATE(FW3-WA) golden :1721-1774 `ATC_InterfaceForm->SetSingleTemp(...)` /
            // `->SetAllTemp(...)` -- ATC_InterfaceForm DOES exist in the port
            // (acarry_shims.h TATC_InterfaceFormShim, extern TATC_InterfaceFormShim
            // *ATC_InterfaceForm;), but that shim carries ONLY `int iATC_MODE_TYPE;`
            // (grep -n "SetSingleTemp\|SetAllTemp" acarry_shims.h -- 20260817 22:39 --
            // zero hits); golden's real TATC_InterfaceForm (ATC_Handler_Side.h, a whole
            // untranslated VCL form/hardware-channel class) is out of scope here per
            // the SAME "gate #14/cContact-class-of-thing" precedent MainCalcCore.h /
            // acarry_shims.h / SCK_ART_Remainder.h already document for this exact
            // object. Every #if 0 branch below only ever WRITES to the (nonexistent)
            // ATC channel hardware -- it reads/mutates no port global -- so the sole
            // externally-visible effect of each branch, the `t.sprintf(...)` reply
            // string and the bChangeTest_TempOffset/bChangeTest_TempAlarm globals
            // (both real, both already assigned just above/below), is preserved
            // ACTIVE and un-gated.
            if(iNowDownArm==Z1Down_Z2Up)    //ARM1 在下
            {
#if 0
                if(iATC_Use_Heat_Count<=4)
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);
                }
                else
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);

                    ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp);
                }
#endif
                t.sprintf("SETTINGOK");
            }
            else if(iNowDownArm==Z1Up_Z2Down)
            {
#if 0
                if(iATC_Use_Heat_Count<=4)
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);
                }
                else
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);

                    ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp+bChangeTest_TempOffset);
                }
#endif
                t.sprintf("SETTINGOK");
            }
            else
            {
#if 0
                ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
#endif
                bChangeTest_TempOffset=0;                                       //Ifor 20230505 add:清除資料
                bChangeTest_TempAlarm=false;                                    //Ifor 20230505 add:清除資料
                t.sprintf("SETTINGNG");
            }
        }
        else
        {
#if 0
            ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
#endif
            bChangeTest_TempOffset=0;                                           //Ifor 20230505 add:清除資料
            bChangeTest_TempAlarm=false;                                        //Ifor 20230505 add:清除資料
            t.sprintf("SETTINGOK");
        }
    }
    // GATE(FW3-WA) golden :1737 `fLotInfo->SetATCOffset(true);` -- established
    // ABSENCE precedent (TfLotInfo, forms/fLotInfo.h, has no SetATCOffset member at
    // all): aTester_Front.cpp:3743-3744 / aTester_Rear.cpp:3612-3613 already gate
    // this EXACT golden call under the same reasoning ("the METHOD is missing, not
    // the object"). Re-verified 20260817 22:39: grep -rn "SetATCOffset" forms/*.h
    // returns zero hits.
#if 0
    fLotInfo->SetATCOffset(true);                                               //WriteSetTestTempStatus
#endif
    SendMSG_CMD(MSG_CMD_SetTestTemp, t);
}

/* ---- golden Command.cpp:1766-1775 ---- */
void TfMain::WriteSoakTimeData()                                     //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient)
        t.sprintf("NONE\r\n");
    else
        t.sprintf("%s\r\n", edSoakTime->Text.c_str());

    SendMSG_CMD(MSG_CMD_HandlerSoakTime, t);
}

/* ---- golden Command.cpp:1777-1794 ---- */
void TfMain::WriteSetSoakTimeStatus()    //Steven 20110613 : Corn GPIB add from 7040
{
    int i, ret;
    AnsiString t;
    i=atoi(HGpib2Handler->cReturn);
    if(i>=0)
    {
        edSoakTime->Text=i;
        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));     //Steven 20120730
    }

    if(ret==0)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetSoakTime, t);
}

/* ---- golden Command.cpp:1797-2028 ---- */
AnsiString TfMain::WriteSiteMapData(bool bGPIB)    //Sam 20170801 (Steven) 移植超豐 OEE 功能 form HT-7045       //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    AnsiString tt;

    if(TestIF_File.iTestMode==SingleSite)       // Single Site 1x1
    {
        t.sprintf("SINGLE1X1-%d_\r\n",
                     TestIF_File.iSiteMap[0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)       // Dual Site 1x2
    {
        t.sprintf("DUAL1X2-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)      //Frank 20160329 add for 1x3_4
    {
        t.sprintf("TRI1X3-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)     // Qual Site 1x4
    {
        t.sprintf("QUAD1X4-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)  // Dual Site 2x1
    {
        t.sprintf("DUAL2X1-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)  // Qual Site 2x2
    {
        t.sprintf("QUAD2X2-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)  // Qual Site 2x2 OK
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="QUAD2X2";
        else
            tt="QUAD2X2N";

        t.sprintf("%s-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)                                   //ChungHung 20140115 add for 2x3_6
    {
        t.sprintf("6SITE2X3-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="6SITE2X3";
        else
            tt="6SITE2X3N";

        t.sprintf("%s-%d-%d-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="8SITE2X4";
        else
            tt="8SITE2X4N";

        t.sprintf("%s-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
        t.sprintf("8SITE2X4-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {
        t.sprintf("10SITE2X5-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",               //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        t.sprintf("12SITE2X6-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",         //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
    {
        t.sprintf("16SITE2X8-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        t.sprintf("16SITE4X4-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        t.sprintf("32SITE4X8-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[2][4],
                     TestIF_File.iSiteMap[3][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[2][5],
                     TestIF_File.iSiteMap[3][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[2][6],
                     TestIF_File.iSiteMap[3][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7],
                     TestIF_File.iSiteMap[2][7],
                     TestIF_File.iSiteMap[3][7]);
    }
    else
    {
        t.sprintf("UNKNOWN\r\n", sizeof(t));
    }

    if(bGPIB)
        SendMSG_CMD(MSG_CMD_HandlerSiteMap, t);

    t=StringReplace(t, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
    return t;                                                                   //Sam 20170801 (Steven) 移植超豐 OEE 功能 form HT-7045
}

/* ---- golden Command.cpp:2030-2060 ---- */
void TfMain::WriteStartMode_NS()
{
    // 01: Initital Start
    // 02: Continuous start
    // 03: Restest Initial start
    // 04: Retest Continuous Restest

    AnsiString t;
    int mode=0;

    if(LastSet.iRunStartMode==rsmNull)
        mode=0;
    else if(LastSet.iRunStartMode==rsmInitialStart)
        mode=1;
    else if(LastSet.iRunStartMode==rsmContinuStart)
        mode=2;
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
        mode=3;
    else if(LastSet.iRunStartMode==rsmContinuRetest)
        mode=4;
    else if(LastSet.iRunStartMode==rsmAutoSiteMap)
        mode=5;
    else if(LastSet.iRunStartMode==rsmQAMode)
        mode=6;
    else
        mode=-1;

    t.sprintf("%02d\r", mode);

    SendMSG_CMD(MSG_CMD_StartMode, t);
}

/* ---- golden Command.cpp:2062-2242 ---- */
void TfMain::WriteAssign_NS()
{
    AnsiString t;

    if(TestIF.iTestMode==SingleSite)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000000%d\r\n",
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==DualSite)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,000000%d%d\r\n",
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,00000%d%d%d\r\n",
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==QualSite1X4)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000%d%d%d%d\r\n",
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==DualSite2x1)                                      // Dual Site 2x1
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,000000%d%d_\r\n",
                     TestIF.iSiteMap[1][0],
                     TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==QualSite2X2N)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_6Site2X3 ||                                      //ChungHung 20140115 add for 2x3_6
            TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,00%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_8Site2X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_10Site2X5)    //wei 20190614 10 site
    {
        t.sprintf("ASSIGN:00000000,00000000,000000%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_12Site2X6)
    {
        t.sprintf("ASSIGN:00000000,00000000,0000%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_16Site2X8)
    {
        t.sprintf("ASSIGN:00000000,00000000,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][7],
                    TestIF.iSiteMap[0][7],
                    TestIF.iSiteMap[1][6],
                    TestIF.iSiteMap[0][6],
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        t.sprintf("ASSIGN:00000000,00000000,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[3][3],
                    TestIF.iSiteMap[2][3],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[3][2],
                    TestIF.iSiteMap[2][2],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[3][1],
                    TestIF.iSiteMap[2][1],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[3][0],
                    TestIF.iSiteMap[2][0],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        t.sprintf("ASSIGN:%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[3][7],
                    TestIF.iSiteMap[2][7],
                    TestIF.iSiteMap[1][7],
                    TestIF.iSiteMap[0][7],
                    TestIF.iSiteMap[3][6],
                    TestIF.iSiteMap[2][6],
                    TestIF.iSiteMap[1][6],
                    TestIF.iSiteMap[0][6],
                    TestIF.iSiteMap[3][5],
                    TestIF.iSiteMap[2][5],
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[3][4],
                    TestIF.iSiteMap[2][4],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[3][3],
                    TestIF.iSiteMap[2][3],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[3][2],
                    TestIF.iSiteMap[2][2],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[3][1],
                    TestIF.iSiteMap[2][1],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[3][0],
                    TestIF.iSiteMap[2][0],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else
    {
        t.sprintf("UNKNOWN\r\n");
    }
    SendMSG_CMD(MSG_CMD_Assign, t);
}

/* ===================== GROUP A2 ===================== */

/* ---- golden Command.cpp:3825-3855 ---- */
void TfMain::WriteForce_NS()
{
    AnsiString t;

    if(IniConfig.bKoreaFunction==true)
    {
        t.sprintf("%s\r", asArmForce1);                                         //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN || IniConfig.bSPILFunction==true || CUSTOMER_CODE==CC_ASE_KaohSiung)              //wei 20151117 傳送ContactForce設定值  //JerryYang 20230204 : Add SPIL support回傳GPIB force指令
    {
        t.sprintf("%sT\r", asArmForce1);                                        //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else
    {
        if(IndexStatus==Z1Down_Z2Up ||
           IndexStatus==Z1_Z2_Down)                                             //Steven 20230214 : Add GPIB for NN mode
        {
            t.sprintf("%s\r\n", asArmForce1);                                   //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            t.sprintf("%s\r\n", asArmForce2);                                   //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else
        {
            //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
            t.sprintf("%d\r", 0);
        }
    }
    SendMSG_CMD(MSG_CMD_ContactForce, t);
}

/* ---- golden Command.cpp:3858-3912 ---- */
AnsiString TfMain::WriteBinMap(bool bGPIB)                           //ChungHung 20150217 add for SCK request
{
    int iBinSelCT;
    AnsiString Data="";
    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[]={"Auto1-", "Auto2-", "Auto3-", "Fix1-", "Fix2-", "Fix3-", "Fix4-", "Fix5-", "Fix6-"};
    if(TrayForm.iFixTrayMode)
    {
        iBinSelCT=9;    //use up down
    }
    else
    {
        iBinSelCT=6;
    }

    for(int i=0; i<iBinSelCT; i++)
    {
        //temp="";
        bHaveBinData=false;
        for(int j=0; j<iTestBinCount; j++)
        {
            temp=BinSelect[iTestRunMode].iCatDataT3Pos[j];
            if(temp<=0)
                continue;

            if(i==temp-1)
            {
                //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                bHaveBinData=true;
                aUnloader[i]+=AnsiString(j)+",";
            }
        }

        if(BinSelect[iTestRunMode].IfErrorT3==i)
        {
            aUnloader[i]+="error";
            bHaveBinData=true;
        }
        else
        {
            if(bHaveBinData)
                aUnloader[i].Delete(aUnloader[i].Length(), 1);
            else
                aUnloader[i]+="NA";
        }
        Data+=aUnloader[i]+"_";
    }
    Data.Delete(Data.Length(), 1);
    Data+=".";

    if(bGPIB)
        SendMSG_CMD(MSG_CMD_BinMap, Data);
    return Data;
}

/* ---- golden Command.cpp:3914-3987 ---- */
void TfMain::WriteSetBinMap(AnsiString BinData)                      //Steven 20230210 : Set Bin Map.
{
    int iMaxBin=16, iPos1, iPos2, iBin, iTray;
    AnsiString Data, Data1=BinData;
    AnsiString Data2;
    AnsiString aUnloader[10]={"BINMAP_", "Auto1-", "Auto2-", "Auto3-", "Fix1-", "Fix2-", "Fix3-", "Fix4-", "Fix5-", "Fix6-"};

    if(HasICUnderMachine()==false)
    {
        TStringList *SL2=new TStringList();
        // GATE(FW3-WA) golden :3921/:3924 `fBinSel->sBinTraySetT3Pos[eBinFT]->Clear()/Add("0")`
        // -- fBinSel (golden TfBinSel*, the Bin-Select mapping VCL form) has NO
        // translated home anywhere in the port; forms/fMain.h's OWN
        // SetMainRunStartMode comment already documents this exact form as "a new
        // fBinSel VCL form ... out of scope" (re-confirmed by grep -rn "fBinSel"
        // --include=*.h . -- 20260817 22:39 -- the only hit is that same comment).
        // Every fBinSel-> line in this function only ever mutates that form's OWN
        // internal grid state (sBinTraySetT3Pos) and has no other port-visible
        // effect; the loop's ONE real side effect on a port global,
        // `BinSelect[eBinFT].IfErrorT3=iTray;` (SYSTEM_BIN_SELECT, cprod.h:2622,
        // confirmed real), and the final `Data="SETTINGOK";` reply string stay
        // ACTIVE and un-gated below.
#if 0
        fBinSel->sBinTraySetT3Pos[eBinFT]->Clear();
        for(int i=0; i<16; i++)
        {
            fBinSel->sBinTraySetT3Pos[eBinFT]->Add("0");
        }
#endif

        for(int i=0; i<10; i++)
        {
            iTray=i;
            if(i==0)
                continue;
            iPos1=Data1.AnsiPos(aUnloader[i]);
            if(iPos1!=0)
            {
                SL2->Clear();
                Data2=Data1.SubString(iPos1, Data1.Length());
                iPos1=Data2.AnsiPos("-")+1;
                iPos2=Data2.AnsiPos("_");
                if(iPos2!=0)
                    Data2=Data2.SubString(iPos1, iPos2-iPos1);
                else
                    Data2=Data2.SubString(iPos1, Data2.Length());
                SL2->CommaText=Data2;

                for(int j=0; j<SL2->Count; j++)
                {
                    // AI(W906-FW3-WA) 20260817: `SL2->Strings[j]` is a
                    // vclcompat::StringsProxy (index-proxy), not an AnsiString --
                    // it does not itself forward AnsiPos()/c_str(). Materializing it
                    // into a plain AnsiString first (golden itself would implicitly
                    // convert the same way through TStrings::operator[]) is a
                    // vclcompat-surface accommodation, not a behavior change.
                    AnsiString sSL2j = SL2->Strings[j];
                    if(sSL2j.AnsiPos("error")!=0)
                    {
                        BinSelect[eBinFT].IfErrorT3=iTray;
                    }
                    else
                    {
                        iBin=atoi(sSL2j.c_str());
                        if(iBin>=iMaxBin)
                        {
                            iMaxBin=iBin+1;
#if 0
                            do
                            {
                                fBinSel->sBinTraySetT3Pos[eBinFT]->Add("0");
                            }while(fBinSel->sBinTraySetT3Pos[eBinFT]->Count<iMaxBin);
#endif
                        }

                        if(iBin!=0 || sSL2j=="0")
                        {
#if 0
                            fBinSel->sBinTraySetT3Pos[eBinFT]->Strings[iBin]=iTray;
#endif
                        }
                    }
                }
            }
        }

#if 0
        fBinSel->Save(3617, eBinFT);
        fBinSel->Save(3616, eBinFT);
#endif
        SL2->Clear();
        delete SL2;
        Data="SETTINGOK";
    }
    else
    {
        Data="SETTINGNG";
    }

    SendMSG_CMD(MSG_CMD_SetBinMap, Data);
}

/* ---- golden Command.cpp:3989-4005 ---- */
void TfMain::WriteTestMode()                                         //ChungHung 20150217 add for SCK request
{
    AnsiString t;

    if(iTestRunMode==FT)
        t.sprintf("Normal\r\n");
    else if(iTestRunMode==RT)
        t.sprintf("Retest\r\n");
    else if(iTestRunMode==FT_ART)
        t.sprintf("Normal_ART\r\n");
    else if(iTestRunMode==RT_ART)
        t.sprintf("Retest_ART\r\n");
    else
        t.sprintf("Off-line\r\n");                                              //ChungHung 20150408 add for SCK

    SendMSG_CMD(MSG_CMD_TestMode, t);
}

/* ===================== GROUP A3 ===================== */

/* ---- golden Command.cpp:5170-5275 ---- */
void TfMain::WriteChkSetup()                                                    // wei 20151125 Add CHKSETUP? Command
{
//Handler Respond
//Setup_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4
//上行代表好品bin1卡關90%(小於90%需alarm)，不良品bin2~6分別卡關0.05%與1~4% (大於等於需alarm)
//Setup_1|P|97_2|P|97_6|F|1
//上行代表有複數個好品bin1和bin2，當有複數個好品bin，他們的卡關yield會是一樣的，以此例子請卡bin1+bin2需大於等於97%(若小於需alarm)
//原先是：Setup_bin|status|yield(%)_bin|status|yield(%)_......
//變更為：Setup_SiteDiff_bin|status|yield(%)_bin|status|yield(%)_......
//SiteDiff：site different的yield，單位是百分比，若不需要設定會代NULL
//e.g.1 Setup_5_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4
//e.g.2 Setup_NULL_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4

    AnsiString sTemp="", sTemp1;
    AnsiString sAlarmData = "";
    int i=0;
    SetWorkParameter();                                                         //Steven 20120130 : 存檔後要重新load參數
    double dVal =0.0;

    if(IniConfig.bSIGURDFunction)
    {
        if(Prod.bFailAlarmSiteYieldCmp)
        {
            sTemp1.sprintf("%f", Prod.dFailAlarmSiteYieldCmp);
            dVal = atof(sTemp1.c_str());
            sTemp1=FloatToStr(dVal);
            sTemp="SETUP_"+sTemp1;                                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            sTemp="SETUP_NULL";
        }
    }
    else
    {
        if(Prod.bFailAlarmSiteYieldDifferent)
        {
            sTemp1.sprintf("%f", Prod.dFailAlarmSiteYield);                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            dVal = atof(sTemp1.c_str());
            sTemp1=FloatToStr(dVal);
            sTemp="SETUP_"+sTemp1;                                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            sTemp="SETUP_NULL";
        }
    }

    sAlarmData = sAlarmData + sTemp;

    //Bin Alarm Yield
    for(i=0; i<16; i++)                                                         //JerryYang 20151226
    {
        AnsiString sBinName = "";
        AnsiString sPassOrFail = "";
        AnsiString sYield = "";

        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]>0)                          //JerryYang 20151210    //Steven 20230929 : !=0 --> >0
        {
            sBinName = IntToStr(i);
            if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                  //KaiChen 20200108 ：矽格-湖口，修改 CHKSETUP?，設定 SiteYieldCmp 和 LowYieldByTotal
            {
                if(Prod.bIsPassBin[i] && Prod.bFailAlarmLowYieldByTotal)
                {
                    sPassOrFail = "P";
                    sYield.sprintf("%f", Prod.dLowYieldLimitByTotal);
                }
                else
                {
                    if(BinSelect[iTestRunMode].bFailure[i]==true)
                    {
                        sPassOrFail = "F";
                        sYield.sprintf("%f",Prod.dFailureLimit[i]);
                    }
                }
            }
            else
            {
                if(Prod.bIsPassBin[i] && TestIF_File.bFailAlarmLowYield)        //JerryYang 20160202 判斷是否開啟Low yield enable
                {
                    sPassOrFail = "P";
                    sYield.sprintf("%f", Prod.dLowYieldLimit);                  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                }
                else
                {
                    if(BinSelect[iTestRunMode].bFailure[i]==true)
                    {
                        sPassOrFail = "F";
                        sYield.sprintf("%f",Prod.dFailureLimit[i]);
                    }
                }
            }
            //sYield.sprintf("%1.0f",Prod.fBinAlarmLimit[i]);
            dVal = atof(sYield.c_str());
            sYield = FloatToStr(dVal);

            if(sYield!=0) //JerryYang 20151210 濾掉Yield=0
            {
                sTemp.sprintf("_%s|%s|%s",sBinName,sPassOrFail,sYield);
                sAlarmData = sAlarmData + sTemp;
            }
        }
    }

    SendMSG_CMD(MSG_CMD_ChkSetup, sAlarmData);
}

/* ---- golden Command.cpp:5277-5291 ---- */
void TfMain::WriteHandlerTestArmEncoder()                                       //wei 20160122
{
    AnsiString t="";
    long iArm1_Y=0, iArm1_Z=0, iArm2_Y=0, iArm2_Z=0;

    iArm1_Y=MOT[MTestY1].Gali_ReadPos();
    iArm1_Z=MOT[MTestZ1].Gali_ReadPos();
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
        iArm2_Y=MOT[MTestY2].Gali_ReadPos();
    iArm2_Z=MOT[MTestZ2].Gali_ReadPos();

    t.sprintf("ARM1_Y_%d_Z_%d_ARM2_Y_%d_Z_%d\r",iArm1_Y,iArm1_Z,iArm2_Y,iArm2_Z );

    SendMSG_CMD(MSG_CMD_GetTestArmPos, t);
}

/* ---- golden Command.cpp:5293-5299 ---- */
void TfMain::WriteHandlerTestArmEP()                                            //wei 20160122
{
    AnsiString t;
    double dValue=0.0;
    // GATE(FW3-WA) golden :5297 `ADAM_ReadPA(&dValue)` -- the ADAM6024 DAQ
    // EP-return-voltage readback has NO translated home anywhere in the port
    // (grep -rln "ADAM_ReadPA\|adam6024" --include=*.h --include=*.cpp . --
    // 20260817 22:39 -- only dfm2rc layout artifacts for the adam6024 FORM exist
    // under tools/dfm2rc/, no translated logic header/source at all). No ADAM DLL
    // binding exists offline to read a value from, so `t` stays empty (SendMSG_CMD
    // below is itself an offline no-op sink either way).
#if 0
    t.sprintf("%d PA\r", ADAM_ReadPA(&dValue));                                 //wei 20220309 Add EP Return Voltage
#endif
    SendMSG_CMD(MSG_CMD_GetTestArmEP, t);
}

/* ---- golden Command.cpp:5390-5417 ---- */
void TfMain::GetCZtesterBin()                                        //JerryYang 20160308 回傳各Bin數量
{
    int iBinCount[1024];
    AnsiString sTestBinCount="";

    ZeroMemory(iBinCount, sizeof(iBinCount));
    for(int i=0; i<iTestBinCount; i++)
    {
        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]!=0 || LastSet.iBinData32[0][i]!=0)    //回傳有設定Tray盤及數量不為0的bin數量
        {
            iBinCount[i]=LastSet.iBinData32[0][i];
        }
        else
        {
            iBinCount[i]=-1;
        }
    }
    for(int i=0; i<iTestBinCount; i++)
    {
        if(iBinCount[i]>=0)
        {
            sTestBinCount=(sTestBinCount+IntToStr(i)+"-"+IntToStr(iBinCount[i])+",");
        }
    }
    sTestBinCount=sTestBinCount+"U-0";

    SendMSG_CMD(MSG_CMD_TesterBin, sTestBinCount);
}

/* ---- golden Command.cpp:5419-5427 ---- */
void TfMain::GetCZSoakTime()                                         //JerryYang 20160315 回傳soak time
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient)
        t="NONE";
    else
        t=edSoakTime->Text;
    SendMSG_CMD(MSG_CMD_SoakTime, t);
}

/* ---- golden Command.cpp:5429-5438 ---- */
void TfMain::GetCZDoubleContactCount()                               //Isaac 20210706 : add MSG_CMD_DoubleContactCount指令，詢問handler doublecontact次數
{
    AnsiString t;
    if(Prod.bD22SupportMultiDoubleContact==false)
       //IniConfig.bD22VerifyMode==false)                                       //Sam 20231117 : 整合到 QA 模式//Sam 20221012 : 新增 VerifyMode 功能
        t="0";
    else
        t=IntToStr(Prod.iD22DoubleContactCount+1);                              //Sam 20231117 : 整合到 QA 模式
    SendMSG_CMD(MSG_CMD_DoubleContactCount, t);
}

/* ---- golden Command.cpp:5440-5445 ---- */
void TfMain::GetCDHandlerID()                                        //JerryYang 20190627 回傳handler ID
{
    AnsiString t;
    t=IniConfig.SocketHandlerID;
    SendMSG_CMD(MSG_CMD_HanderIDRS232, t);
}

/* ---- golden Command.cpp:5447-5461 ---- */
void TfMain::GetCZJamCode()                                          //JerryYang 20160315 回傳Jam Code
{
    AnsiString t, sTemp;
    if(fNote->fShow)                                                            //Isaac 20170825(jou) 回傳JAM增加支援WAR,MES
    {
        sTemp=fNote->edErrorCode->Text;
        sTemp.Delete(1, 3);                                                     //刪除JAM字元 只回傳Jam code的數字部分
        t=sTemp;
    }
    else
    {
        t="0";                                                                  //沒有發生Jam的時候回傳Jam code為"0"
    }
    SendMSG_CMD(MSG_CMD_JamCode, t);                                            //JerryYang 20200324 fix回傳jam code異常
}

/* ---- golden Command.cpp:5463-5638 ---- */
void TfMain::GetCZSiteMap(bool bSendGPIB)                            //JerryYang 20160324 回傳SiteMap
{
    AnsiString sTemp, t="";

    if(TestIF_File.iTestMode==SingleSite)                                       // Single Site 1x1
    {
        t.sprintf("%d", TestIF_File.iSiteMap[0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
    {
        t.sprintf("%d,%d", TestIF_File.iSiteMap[0][0],
                           TestIF_File.iSiteMap[0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        t.sprintf("%d,%d,%d", TestIF_File.iSiteMap[0][0],
                              TestIF_File.iSiteMap[0][1],
                              TestIF_File.iSiteMap[0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)
    {
        t.sprintf("%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                 TestIF_File.iSiteMap[0][1],
                                 TestIF_File.iSiteMap[0][2],
                                 TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 // Dual Site 2x1
    {
        t.sprintf("%d,%d", TestIF_File.iSiteMap[0][0],
                           TestIF_File.iSiteMap[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2 ||                               // Qual Site 2x2
            TestIF_File.iTestMode==QualSite2X2N)
    {
        t.sprintf("%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                 TestIF_File.iSiteMap[1][0],
                                 TestIF_File.iSiteMap[0][1],
                                 TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||                                 //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        t.sprintf("%d,%d,%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                       TestIF_File.iSiteMap[1][0],
                                       TestIF_File.iSiteMap[0][1],
                                       TestIF_File.iSiteMap[1][1],
                                       TestIF_File.iSiteMap[0][2],
                                       TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||                                 // 8 Site 2x4
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",
                    TestIF_File.iSiteMap[0][0],
                    TestIF_File.iSiteMap[1][0],
                    TestIF_File.iSiteMap[0][1],
                    TestIF_File.iSiteMap[1][1],
                    TestIF_File.iSiteMap[0][2],
                    TestIF_File.iSiteMap[1][2],
                    TestIF_File.iSiteMap[0][3],
                    TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",                        //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[2][4],
                     TestIF_File.iSiteMap[3][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[2][5],
                     TestIF_File.iSiteMap[3][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[2][6],
                     TestIF_File.iSiteMap[3][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7],
                     TestIF_File.iSiteMap[2][7],
                     TestIF_File.iSiteMap[3][7]);
    }
    else
    {
        t.sprintf("UNKNOWN", sizeof(t));
    }
    aSendSiteMapping=t;
    if(bSendGPIB==true)
        SendMSG_CMD(MSG_CMD_SiteMap, t);
}

/* ===================== GROUP A4 ===================== */

/* ---- golden Command.cpp:12063-12102 ---- */
void TfMain::SetTesterID()                                                      //Sam 20210617 : Add GPIB SETTESTERID
{
    AnsiString asRet,asCmd;
    char str[256];
    AnsiString sPath;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SETTESTERID, asRet);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        sPath=AuthPath+"config.ini";

        if(asCmd!="")
        {
            WriteIniData(sPath, "bUseAlarmLogXml", "sN27_TesterID", asCmd);
            // GATE(FW3-WA) golden :2887 `fConfiguration->edN27_6->Text=asCmd;` --
            // fConfiguration (golden TfConfiguration, cConfiguration.h -- a whole
            // untranslated VCL form) has no forms/ facade (grep -rln "fConfiguration"
            // forms/*.h -- 20260817 22:39 -- zero hits; the only port-tree
            // `fConfiguration` at all is Automation/SCK_ART_Remainder.h's TU-local
            // `W5SckArtRem_ConfigStub`, which carries only ->mmoN04_IP, NOT edN27_6).
            // Pure UI-mirror write: the persisted value (WriteIniData just above) and
            // the live IniConfig.sN27_TesterID global (just below) are both already
            // real and un-gated, so the ONLY thing elided here is a text box that
            // renders nowhere offline.
#if 0
            fConfiguration->edN27_6->Text=asCmd;
#endif
            IniConfig.sN27_TesterID=asCmd;
            asRet.sprintf("OK");
        }
        else
        {
            asRet.sprintf("NG");
        }
        SendMSG_CMD(MSG_CMD_SETTESTERID, asRet);
    }
}

/* ---- golden Command.cpp:12104-12109 ---- */
void TfMain::GetTesterID()                    //Sam 20210617 : Add GPIB SETTESTERID
{
    AnsiString asRet;
    asRet=IniConfig.sN27_TesterID+"\r\n";//Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    SendMSG_CMD(MSG_CMD_GETTESTERID, asRet);
}

/* ---- golden Command.cpp:12111-12116 ---- */
void TfMain::GetAutoClean()                                                     //Sam 20220408 : Novatek 新增 AUTOCLEAN?
{
    AnsiString asRet="";
    asRet=AutoCleanStrings();
    SendMSG_CMD(MSG_CMD_GetAutClean , asRet);
}

/* ---- golden Command.cpp:12118-12128 ---- */
AnsiString TfMain::AutoCleanStrings()                                           //Sam 20220408 : Novatek 新增 AUTOCLEAN?
{
    AnsiString asRet="";
    asRet.sprintf("%d,%d,%d,%2.1f,%d",
                    TestIF.iAutoClean_Function,
                    TestIF.iAutoClean_IntervalContact,
                    TestIF_File.iAutoClean_ContactMode,
                    (double)TestIF.iAutoClean_ContactTime/10.0,
                    TestIF.iAutoClean_ContactCount);
    return asRet;
}

/* ---- golden Command.cpp:12130-12135 ---- */
void TfMain::GetForcePerPinN()                                                  //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
{
    AnsiString asRet="";
    asRet=ForcePerPinNStrings();
    SendMSG_CMD(MSG_CMD_ForcePerPinN , asRet);
}

/* ---- golden Command.cpp:12137-12142 ---- */
AnsiString TfMain::ForcePerPinNStrings()                                        //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
{
    AnsiString asRet="";
    asRet.sprintf("%0.4f", DeviceForm.ForcePerPinN);                            //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}

/* ---- golden Command.cpp:12144-12149 ---- */
void TfMain::GetContactHeight()                                                 //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
{
    AnsiString asRet="";
    asRet=ContactHeightStrings();
    SendMSG_CMD(MSG_CMD_ContactHeight , asRet);
}

/* ---- golden Command.cpp:12151-12156 ---- */
AnsiString TfMain::ContactHeightStrings()                                       //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f,%0.2f", DeviceForm.IndexContact[0]/100.0, DeviceForm.IndexContact[1]/100.0);
    return asRet;
}

/* ---- golden Command.cpp:12158-12163 ---- */
void TfMain::GetYieldContinusFail()                                             //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
{
    AnsiString asRet="";
    asRet=YieldContinusFailStrings();
    SendMSG_CMD(MSG_CMD_YieldContinusFail , asRet);
}

/* ---- golden Command.cpp:12165-12173 ---- */
AnsiString TfMain::YieldContinusFailStrings()                                   //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
{
    AnsiString asRet="";
    //int iOnOff=0;
    //iOnOff=(Prod.bContsFailBySocket)?1:0;
    //asRet.sprintf("%d,%d",iOnOff, Prod.iContsFailSocketAlarmCT);
    asRet.sprintf("%d", Prod.iContsFailSocketAlarmCT);                          //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}

/* ---- golden Command.cpp:12175-12180 ---- */
void TfMain::GetYieldSiteCompare()                                              //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
{
    AnsiString asRet="";
    asRet=YieldSiteCompareStrings();
    SendMSG_CMD(MSG_CMD_YieldSiteCompare , asRet);
}

/* ---- golden Command.cpp:12182-12190 ---- */
AnsiString TfMain::YieldSiteCompareStrings()                                    //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
{
    AnsiString asRet="";
    //int iOnOff=0;
    //iOnOff=(Prod.bFailAlarmSiteYieldCmp)?1:0;
    //asRet.sprintf("%d,%2.1f,%d",iOnOff, Prod.dFailAlarmSiteYieldCmp, Prod.iFailAlarmSiteYieldCmpCount );
    asRet.sprintf("%2.1f", Prod.dFailAlarmSiteYieldCmp );                       //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}

/* ---- golden Command.cpp:12192-12197 ---- */
void TfMain::GetDUTStaus()                                                      //Sam 20220408 : Novatek 新增 DUTSTATUS?
{
    AnsiString asRet="";
    asRet=DUTStausStrings();
    SendMSG_CMD(MSG_CMD_DUTStatus, asRet);
}

/* ---- golden Command.cpp:12199-12264 ---- */
AnsiString TfMain::DUTStausStrings()                                            //Sam 20220408 : Novatek 新增 DUTSTATUS?
{
    AnsiString asRet="";
    int iDut[32];

    for(int i=0; i<32; i++)
        iDut[i]=0;

    //Sam 20231205 : 修正 NOVATEK 指令
    //==>
    if(TestIF_File.iTestMode==SingleSite)                                       // Single Site 1x1
        asRet="SINGLE1X1";
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
        asRet="DUAL1X2";
    else if(TestIF_File.iTestMode==TriSite1X3)                                  //Frank 20160329 add for 1x3_4
        asRet="TRI1X3";
    else if(TestIF_File.iTestMode==QualSite1X4)                                 // Qual Site 1x4
        asRet="QUAD1X4";
    else if(TestIF_File.iTestMode==DualSite2x1)                                 // Dual Site 2x1
        asRet="DUAL2X1";
    else if(TestIF_File.iTestMode==QualSite2X2)                                 // Qual Site 2x2
        asRet="QUAD2X2";
    else if(TestIF_File.iTestMode==QualSite2X2N)                                // Qual Site 2x2 OK
        asRet=(CUSTOMER_CODE==CC_SCK)?"QUAD2X2":"QUAD2X2N";
    else if(TestIF_File.iTestMode==_6Site2X3)                                   //ChungHung 20140115 add for 2x3_6
        asRet="6SITE2X3";
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
        asRet=(CUSTOMER_CODE==CC_SCK)?"6SITE2X3":"6SITE2X3N";
    else if(TestIF_File.iTestMode==_8Site2X4)                                   // 8 Site 2x4
        asRet="8SITE2X4";
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
        asRet=(CUSTOMER_CODE==CC_SCK)?"8SITE2X4":"8SITE2X4N";
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
        asRet="10SITE2X5";
    else if(TestIF_File.iTestMode==_12Site2X6)
        asRet="12SITE2X6";
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
        asRet="16SITE2X8";
    else if(TestIF_File.iTestMode==_16Site4X4)
        asRet="16SITE4X4";
    else if(TestIF_File.iTestMode==_32Site4X8N)
        asRet="32SITE4X8";
    else
        asRet="UNKNOWN";
    asRet+="_";
    //<==
    //Sam 20231205 : 修正 NOVATEK 指令
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=32)            //SiteMap：Use=1, not use=0。
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
                    iDut[TestIF.iSiteMap[i][j]-1]=(bTestSiteUse[1][i][j])?1:0;
                else
                    iDut[TestIF.iSiteMap[i][j]-1]=(bTestSiteUse[0][i][j])?1:0;
            }
        }
    }

    for(int i=0; i<TestSocket.iShtCnt; i++)
        asRet+=IntToStr(iDut[i])+",";

    return asRet;
}

/* ---- golden Command.cpp:12266-12271 ---- */
void TfMain::GetUPH()                                                           //Sam 20220408 : Novatek 新增 UPH?
{
    AnsiString asRet="";
    asRet=UPHStrings();
    SendMSG_CMD(MSG_CMD_UPH , asRet);
}

/* ---- golden Command.cpp:12273-12285 ---- */
AnsiString TfMain::UPHStrings()                                                 //Sam 20220408 : Novatek 新增 UPH?
{
    AnsiString asRet="";
    // GATE(FW3-WA) golden :12277 `fShowBinSelect->UPH_StringGrid->Cells[3][1]` --
    // fShowBinSelect (golden TfShowBinSelect, cShowBinSelect.h -- a whole
    // untranslated VCL form) has no forms/ facade and no other stand-in anywhere in
    // the port (grep -rln "fShowBinSelect" --include=*.h --include=*.cpp . --
    // 20260817 22:39 -- zero hits at all). No offline UI thread ever populates that
    // grid cell, so it is faithfully in the SAME state golden's own empty-cell arm
    // already handles: `Cells[3][1]==""` -> return "0". Whole body collapses to
    // that arm rather than gating a single unreachable branch.
#if 0
    if(fShowBinSelect->UPH_StringGrid->Cells[3][1]=="")                         //Sam 20231205 : 修正 NOVATEK 指令
    {
        return "0";
    }
    else
    {
        asRet.sprintf("%s",fShowBinSelect->UPH_StringGrid->Cells[3][1]);
        return asRet;
    }
#else
    return "0";
#endif
}

/* ---- golden Command.cpp:12287-12292 ---- */
void TfMain::GetIndexCycleTime()                                                //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
{
    AnsiString asRet="";
    asRet=IndexCycleTimeStrings();
    SendMSG_CMD(MSG_CMD_IndexCycleTime , asRet);
}

/* ---- golden Command.cpp:12294-12306 ---- */
AnsiString TfMain::IndexCycleTimeStrings()                                      //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
{
    AnsiString asRet="";
    // GATE(FW3-WA) golden :12298 `fObserver->TimeInfoGrid->Cells[4][11]` --
    // fObserver (golden TfObserver, cObserver.h -- a whole untranslated VCL form)
    // has no forms/ facade; the ONLY existing `fObserver` stand-in in the port is
    // atester_shims.h's TfObserverShim, which carries labFactory/memoLotSummary
    // only (grep -n "TimeInfoGrid" atester_shims.h forms/*.h -- 20260817 22:39 --
    // zero hits anywhere). No offline UI thread ever populates that grid cell, so
    // it is faithfully in the SAME state golden's own empty-cell arm already
    // handles: `Cells[4][11]==""` -> return "0". Whole body collapses to that arm
    // (same reasoning as the sibling UPHStrings() just above).
#if 0
    if(fObserver->TimeInfoGrid->Cells[4][11]=="")                               //Sam 20231205 : 修正 NOVATEK 指令
    {
        return "0";
    }
    else
    {
        asRet.sprintf("%s",fObserver->TimeInfoGrid->Cells[4][11]);
        return asRet;
    }
#else
    return "0";
#endif
}

/* ---- golden Command.cpp:12308-12313 ---- */
void TfMain::GetTempOfs()                                                       //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
{
    AnsiString asRet="";
    asRet=TempOfsStrings();
    SendMSG_CMD(MSG_CMD_TempOfs , asRet);
}

/* ---- golden Command.cpp:12315-12332 ---- */
AnsiString TfMain::TempOfsStrings()                                             //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
{
    AnsiString asRet="", asTempOfs="";

    for(int i=0; i<tcTotalCount; i++)
    {
        if((i>=tcAa1 && i<=tcBd2) ||
           (i>=tcAe1 && i<=tcBh2))
        {
            if(bUT150Install[i])
            {
                asTempOfs.sprintf("%2.1f", Temperature.fTempOffSet[UserOffSet][i]);
                asRet+=asTempOfs+",";
            }
        }
    }
    return asRet;
}

/* ---- golden Command.cpp:12334-12339 ---- */
void TfMain::GetTempRange()                                                     //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
{
    AnsiString asRet="";
    asRet=TempRangeStrings();
    SendMSG_CMD(MSG_CMD_TempRange , asRet);
}

/* ---- golden Command.cpp:12341-12347 ---- */
AnsiString TfMain::TempRangeStrings()                                           //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
{
    AnsiString asRet="";
    //asRet.sprintf("%d,%d",IniConfig.iL04TemptureRange, IniConfig.iL05ChamberTemptureRange);
    asRet.sprintf("%d",IniConfig.iL04TemptureRange);                            //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}

/* ---- golden Command.cpp:12349-12354 ---- */
void TfMain::GetVacuumAir()                                                     //Sam 20220408 : Novatek 新增 VACUUMAIR?
{
    AnsiString asRet="";
    asRet=VacuumAirStrings();
    SendMSG_CMD(MSG_CMD_VACUUMAIR , asRet);
}

/* ---- golden Command.cpp:12356-12363 ---- */
AnsiString TfMain::VacuumAirStrings()                                           //Sam 20220408 : Novatek 新增 VACUUMAIR?
{
    AnsiString asRet="";
    int iVacuumAir=0;
    iVacuumAir=(Sen[SnAirIsEnough].IsOn())?1:0;
    asRet.sprintf("%d",iVacuumAir);
    return asRet;
}

/* ---- golden Command.cpp:12365-12385 ---- */
void TfMain::GetAll()                                                           //Sam 20220408 : Novatek 新增 SET_ALL?
{
    AnsiString asRet="";
    asRet.sprintf("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s",
    AutoCleanStrings(),
    ForcePerPinNStrings(),
    ContactHeightStrings(),
    YieldContinusFailStrings(),
    YieldSiteCompareStrings(),
    DUTStausStrings(),
    UPHStrings(),
    IndexCycleTimeStrings(),
    TempDataStrings(),
    TempOfsStrings(),
    TempRangeStrings(),
    ArmStatusStrings(),
    PERSITETemperatureStrings(),
    VacuumAirStrings(),
    HandlerVersionStrings());
    SendMSG_CMD(MSG_CMD_Get_All , asRet);
}

/* ---- golden Command.cpp:12387-12392 ---- */
void TfMain::GetHandlerVersion()                                                //Sam 20220408 : Novatek 新增 HANDLERVERSION?
{
    AnsiString asRet="";
    asRet=HandlerVersionStrings();
    SendMSG_CMD(MSG_CMD_HandlerVersion , asRet);
}

/* ---- golden Command.cpp:12394-12399 ---- */
AnsiString TfMain::HandlerVersionStrings()                                      //Sam 20220408 : Novatek 新增 HANDLERVERSION?
{
    AnsiString asRet="";
    asRet.sprintf("%s",asHandlerVersion);
    return asRet;
}

/* ---- golden Command.cpp:12401-12447 ---- */
void TfMain::UploadProdLog()                                                    //JerryYang 20220923 : [N17] upload lot summary
{
    GetYesterdayInfo();
    AnsiString  asPath, strPath, asFileName;
    bool bResult=false;

    asPath.sprintf("%s\\%04d%02d\\", asTravelingLogPath, SystemYearYesterday, SystemMonthYesterday);
    if(IniConfig.bSPILFunction==true)                                           //Steven 20250812 : Change file name for QLE
    {
        if(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU && SPIL_FOR_QLE==1)
        {
            asFileName.sprintf("%s_%s_%04d%02d%02d_ProductionLog.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
        }
        else
        {
            asFileName.sprintf("%s_%04d%02d%02d.csv", PC_NAME, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
        }
    }
    else
    {
        asFileName.sprintf("%s_%s_%04d%02d%02d_ProductionLog.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
    }
    asPath+=asFileName;
    if(IniConfig.bN17UploadProdLog)
    {
        if(DirectoryExists(IniConfig.asN17ProductionLogPath))
        {
            if(FileExists(asPath))
            {
                strPath.sprintf("%s\\%s", IniConfig.asN17ProductionLogPath, asFileName);
                bResult=CopyFile(asPath.c_str(), strPath.c_str(), true);
                if(bResult==false)
                {
                    ShowMyMessage("Uploaded Production log error\r\nPlease check the path of N-17.","上傳Production log失敗\r\n請檢查N-17路徑是否存在");
                }
                else
                {
                    RecordProcess("Uploaded Production log successfully.");   //Steven 20190722 : add TSV log
                }
            }
        }
        else
        {
            ShowMyMessage("Uploaded Production log error\r\nPlease check the path of N-17.","上傳Production log失敗\r\n請檢查N-17路徑是否存在");
        }
    }
}

/* ---- golden Command.cpp:12449-12464 ---- */
void TfMain::GetShuttleMode()                                                   //Sam 20230130 : Add GPIB GETSHUTTLEMODE?
{
    AnsiString asRet="";
    if(TestIF_File.iShuttleMode==0)
    {
        asRet="0";                                                              //Normal
    }
    else
    {
        if(TestIF_File.iShuttle_Sel==0)
            asRet="1";                                                          //UseShuttle1
        else
            asRet="2";                                                          //UseShuttle2
    }
    SendMSG_CMD(MSG_CMD_GETSHUTTLEMODE, asRet);
}

/* ---- golden Command.cpp:12466-12478 ---- */
void TfMain::SetMaxTest()                                                       //Sam 20230201 : Add GPIB SETMAXTEST_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    TestIF_File.iMaxTime=atof(asCmd.c_str());
    // GATE(FW3-WA) golden :12471 `FTestIF->edMaxTestTime->Text=...` -- FTestIF
    // (golden TFTestIF, cTesterIF.h -- a whole untranslated VCL form) has no
    // forms/ facade; the only port-tree `FTestIF` at all is
    // Automation/SCK_ART_Remainder.h's gate #2 note (`FTestIF->ReadTestIFFile()`
    // only, no edMaxTestTime -- grep -n "edMaxTestTime" --include=*.h . -- 20260817
    // 22:39 -- zero hits). Pure UI-mirror write: TestIF_File.iMaxTime (just above)
    // and the WriteIniData persistence (just below) are both real and un-gated.
#if 0
    FTestIF->edMaxTestTime->Text=TestIF_File.iMaxTime;
#endif
    WriteIniData(szDir, "Time", "MAX Time", TestIF_File.iMaxTime);
    asRet="OK";
    SendMSG_CMD(MSG_CMD_SETMAXTEST, asRet);
}

/* ---- golden Command.cpp:12480-12485 ---- */
void TfMain::GetMaxTest()                                                       //Sam 20230201 : Add GPIB GETMAXTEST
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f", TestIF_File.iMaxTime);
    SendMSG_CMD(MSG_CMD_GETMAXTEST, asRet);
}

/* ---- golden Command.cpp:12487-12499 ---- */
void TfMain::SetMaxInitialTest()                                                //Sam 20230201 : Add GPIB SETINITIALMAXTEST_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    TestIF_File.iInitialMaxTime=atof(asCmd.c_str());
    // GATE(FW3-WA) golden :12494 `FTestIF->edInitialMaxTest->Text=...` -- same
    // FTestIF absence as SetMaxTest() above (see that GATE for the full citation).
#if 0
    FTestIF->edInitialMaxTest->Text=TestIF_File.iInitialMaxTime;
#endif
    WriteIniData(szDir, "Time", "Initial MAX Time", TestIF_File.iInitialMaxTime);
    asRet="OK";
    SendMSG_CMD(MSG_CMD_SETINITIALMAXTEST, asRet);
}

/* ---- golden Command.cpp:12501-12506 ---- */
void TfMain::GetMaxInitialTest()                                                //Sam 20230201 : Add GPIB GETINITIALMAXTEST
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f", TestIF_File.iInitialMaxTime);
    SendMSG_CMD(MSG_CMD_GETINITIALMAXTEST, asRet);
}

/* ---- golden Command.cpp:12508-12536 ---- */
AnsiString TfMain::GetSiteState()                                               //Sam 20230511 : 機台資料變更後須上傳 FTP
{
    AnsiString sMessage="";

    int iSiteState[32];
    for(int i=0; i<32; i++)
        iSiteState[i]=-1;

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF_File.iSiteMap[i][j]>0 && TestIF_File.iSiteMap[i][j]<=32)
            {
                if(bTestSiteUse[0][i][j] || bTestSiteUse[1][i][j])              //Sam 20231227 : 修正關 Arm 的時候資料錯誤
                    iSiteState[TestIF_File.iSiteMap[i][j]-1]=1;
                else
                    iSiteState[TestIF_File.iSiteMap[i][j]-1]=0;
            }
        }
    }

    for(int i=0; i<TestSocket.iShtCnt; i++)
    {
        sMessage+=IntToStr(iSiteState[i])+",";
    }

    return sMessage;
}

// =============================================================================
//  FW-3 Wave B translation: golden Command.cpp giant-triplet PURE-method group
//
//  Translation wave: FW-3 Wave B
//  Translator: AI(W906-FW3-WB) 20260817
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Command.cpp (15,273 lines, cp950)
//
//  WAVE SCOPE (3 methods, 4,402 raw golden lines; ALL PURE -- zero widget/form
//  references, single tail SendMSG_CMD each; verified this pass by
//  `grep -nE '\b(f[A-Z][A-Za-z]+|F[A-Z][A-Za-z]+)->' ` over all three golden
//  spans -- 0 hits, 20260817)
//  ------------------------------------------------------------------------------
//    WriteTemp_NS         golden :2244-3823 (1,580 lines)  ACTIVE
//    WriteNowAllTempData  golden :4007-5168 (1,162 lines)  ACTIVE
//    GetCZAllMassTemp     golden :5640-7299 (1,660 lines)  ACTIVE
//  All three are "機械性分支海": nested if/else trees over TestIF.iTestMode /
//  IndexStatus / USE_16_HEATER / TestIF_File.* customer-layout flags, each leaf
//  a single AnsiString::sprintf(...) call formatting 1-32 site slots out of the
//  asGPIBTempShow[] temperature-string table (the SAME file-scope array/global
//  RefreshTempData (Wave A, this file) already fills; no interaction with it is
//  made here beyond reading it), followed by one SendMSG_CMD(...) at the very
//  end. No method touches any VCL form, calls any other TfMain member, or
//  mutates any global -- confirmed by the dependency audit below.
//
//  DEPENDENCY AUDIT (every non-keyword identifier used across all three golden
//  spans, extracted by regex and checked individually against the port tree,
//  20260817 22:5x -- commands reproduced in this wave's report)
//  ------------------------------------------------------------------------------
//   Function calls used (besides AnsiString methods sprintf/c_str and the libc
//   atof): bGetHeaterUsed(int) and bGet16HeaterUsedTo4Heater(int) -- both real,
//   declared bthermo.h:46-47, defined bthermo.cpp:944/:4795 (already-landed
//   W906-GA-BTHERMO wave, archived in ht9045_sm). #include "bthermo.h" added
//   just above GROUP B's first function (this file's include block at the top
//   is Wave A's and is append-only per this wave's write contract).
//   Data members read: IniConfig.bL30Use1CableLayoutKit /
//   .bL30Use1CableLayoutKitByConfig / .bSIGURDFunction (Config.h:899-900/:135);
//   LastSet.bUseTestSocket[2][4][8] (LastSet.h:431 -- NOT the sibling
//   bUseTestSocketEE[2][4][8], a different member used only by
//   Automation/SCK_ART*.h); Temperature.iIndexHeatMode (cprod.h:1393);
//   TestIF.iGpibMode/.iShuttleMode/.iShuttle_Sel/.iTestMode (cprod.h:1673/
//   :1654/:1655/ SYSTEM_TEST_IF); TestIF_File.b1CableLayoutKit/.bNS8000CS/
//   .bUse32Heater (cprod.h:1878/:1875/:2128). All already included via Wave
//   A's #include block (cprod.h, Config.h via cpublic.h/MachineType.h chain,
//   LastSet.h transitively) -- confirmed by the successful -fsyntax-only run
//   below, no new #include needed for any of these five roots.
//   Enum/const literals used (tc* temperature-channel indices, eht*Heater
//   USE_16_HEATER values, *Site test-mode values, Z1Down_Z2Up/Z1Up_Z2Down
//   IndexStatus values, ChamberOnly/SocketChamber iIndexHeatMode values,
//   CONTACT_NORMAL, InterfaceType_Delta_Castle): all pre-existing in
//   MachineType.h / cmydef.h / cprod.h, same headers Wave A's RefreshTempData
//   (this file, GROUP A1) already draws the identical tc*/eht* family from.
//   Two identifiers (ATC_SYSTEM, eNonChamber) appear ONLY inside a golden `//`
//   line comment (golden WriteNowAllTempData :5109, a commented-out dead
//   `if(bGetHeaterUsed(i) && i==tcChamber && ATC_SYSTEM==eNonChamber)`) --
//   copied verbatim as a comment, never compiled, so no dependency check
//   needed for those two.
//
//  GATE REGISTER: NONE. Zero #if 0 sites in this wave -- every identifier used
//  by all three methods resolves to a real port symbol (see audit above); no
//  VCL form, no missing member, no untranslated subsystem is touched anywhere
//  in the 4,402 golden lines.
//
//  STUB COLLISIONS: NONE. `grep -rn "TfMain::WriteTemp_NS\|TfMain::WriteNowAllTempData\|TfMain::GetCZAllMassTemp" --include=*.cpp .`
//  (20260817 22:5x) finds only this file's own three new definitions -- no
//  pre-existing stub body anywhere else in the port to retire.
//
//  GOLDEN ODDITY (noted, NOT fixed per contract): GetCZAllMassTemp golden
//  :7295 `t.sprintf("UNKNOWN", sizeof(t));` -- passes an unused `sizeof(t)`
//  vararg to a format string with zero `%` specifiers (harmless: the extra
//  arg is simply never consumed), almost certainly a copy-paste leftover from
//  a sibling call earlier in the same golden function. Copied verbatim.
//
//  INTEGRATION STATUS: same posture as Wave A -- Command.cpp is still NOT
//  added to any CMakeLists.txt target (out of this wave's write scope), so
//  `build.bat gate` will link-fail on undefined references to these 3 new
//  TfMain:: members for the same INHERENT reason Wave A already documented at
//  the top of this file. This wave's own compile evidence is the standalone
//  `g++ -fsyntax-only` run specified in the task brief (see this wave's
//  report), not a full gate run.
// =============================================================================

#include "bthermo.h"    // AI(W906-FW3-WB) 20260817: bGetHeaterUsed/bGet16HeaterUsedTo4Heater
                          // (golden bthermo.h:46-47) -- Wave A's include block above is
                          // append-only, so this wave's one new header goes here instead.

/* ===================== FW3-WB GROUP B ===================== */

/* ---- golden Command.cpp:2244-3823 ---- */
void TfMain::WriteTemp_NS()
{
    AnsiString t, asNULL = "NULL";

    if(Temperature.iIndexHeatMode==ChamberOnly)
    {
        for(int i=tcHead1; i<=tcBd2; i++)
        {
            if(i!=tcChamber && i!=tcSocket && i!=tcCCD)
                asGPIBTempShow[i]=asGPIBTempShow[tcChamber];
        }
    }

    //2013.01.24 Q_Q TSMC GPIB COMMAND Part 2. {
    if(TestIF.iTestMode==SingleSite)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0);
            }
        }
        else
        {
            t.sprintf("1,%+06.1f\r\n", 0);
        }
    }
    else if(TestIF.iTestMode==DualSite)                                         // Dual Site 1x2
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n",
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite1X4)      // Qual Site 1x4
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2)                                      // Qual Site 2x2
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            0,
                            0,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     // Qual Site 2x2
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            0,
                            0,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3)                                        //ChungHung 20140115 add for 2x3_6
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                             0,
                             0,
                             0,

                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4)                                        // 8 Site 2x4
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_10Site2X5)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_12Site2X6)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //16 Site
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcAg1]!="ERR" && asGPIBTempShow[tcAg1]!="NULL")?atof(asGPIBTempShow[tcAg1].c_str()):0,
                            (asGPIBTempShow[tcAh1]!="ERR" && asGPIBTempShow[tcAh1]!="NULL")?atof(asGPIBTempShow[tcAh1].c_str()):0,

                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0,
                            (asGPIBTempShow[tcBg1]!="ERR" && asGPIBTempShow[tcBg1]!="NULL")?atof(asGPIBTempShow[tcBg1].c_str()):0,
                            (asGPIBTempShow[tcBh1]!="ERR" && asGPIBTempShow[tcBh1]!="NULL")?atof(asGPIBTempShow[tcBh1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcAg2]!="ERR" && asGPIBTempShow[tcAg2]!="NULL")?atof(asGPIBTempShow[tcAg2].c_str()):0,
                            (asGPIBTempShow[tcAh2]!="ERR" && asGPIBTempShow[tcAh2]!="NULL")?atof(asGPIBTempShow[tcAh2].c_str()):0,

                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0,
                            (asGPIBTempShow[tcBg2]!="ERR" && asGPIBTempShow[tcBg2]!="NULL")?atof(asGPIBTempShow[tcBg2].c_str()):0,
                            (asGPIBTempShow[tcBh2]!="ERR" && asGPIBTempShow[tcBh2]!="NULL")?atof(asGPIBTempShow[tcBh2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcAg2]!="ERR" && asGPIBTempShow[tcAg2]!="NULL")?atof(asGPIBTempShow[tcAg2].c_str()):0,
                            (asGPIBTempShow[tcAh2]!="ERR" && asGPIBTempShow[tcAh2]!="NULL")?atof(asGPIBTempShow[tcAh2].c_str()):0,

                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0,
                            (asGPIBTempShow[tcBg2]!="ERR" && asGPIBTempShow[tcBg2]!="NULL")?atof(asGPIBTempShow[tcBg2].c_str()):0,
                            (asGPIBTempShow[tcBh2]!="ERR" && asGPIBTempShow[tcBh2]!="NULL")?atof(asGPIBTempShow[tcBh2].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcAg1]!="ERR" && asGPIBTempShow[tcAg1]!="NULL")?atof(asGPIBTempShow[tcAg1].c_str()):0,
                            (asGPIBTempShow[tcAh1]!="ERR" && asGPIBTempShow[tcAh1]!="NULL")?atof(asGPIBTempShow[tcAh1].c_str()):0,

                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0,
                            (asGPIBTempShow[tcBg1]!="ERR" && asGPIBTempShow[tcBg1]!="NULL")?atof(asGPIBTempShow[tcBg1].c_str()):0,
                            (asGPIBTempShow[tcBh1]!="ERR" && asGPIBTempShow[tcBh1]!="NULL")?atof(asGPIBTempShow[tcBh1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    else
    {
        t.sprintf("UNKNOWN\r", sizeof(t));
    }

    if(TestIF.iGpibMode==InterfaceType_Delta_Castle)
    {
        if(Temperature.iIndexHeatMode==ChamberOnly)
        {
            t=asGPIBTempShow[tcChamber]+"\n";
        }
        else
        {
            if(IndexStatus==Z1Down_Z2Up)
            {
                t=asGPIBTempShow[tcShuttle1]+"\n";
            }
            else
            {
                t=asGPIBTempShow[tcShuttle2]+"\n";
            }
        }
    }

    SendMSG_CMD(MSG_CMD_ActualTemp, t);
}

/* ---- golden Command.cpp:4007-5168 ---- */
void TfMain::WriteNowAllTempData()                                              //Frank 20150729 add Send Now All Temp Value To Test
{
    AnsiString sTemp="";
    double fTemp = 999.9;
    AnsiString t="";
    AnsiString asNULL="NULL";
    bool bReturnNull=false;

//    Handler Respond
//    Format : TEMP_Plate1_Plate2_Shuttle1_Shuttle2_Head1_Head2_Head5_Head6_Socket_Chamber
//    Ex1 , TEMP_+90.4_+90.2_+89.7_+90.3_+90.1_+90.2_+89.9_+90.0
//    Ex2, TEMP_NULL_+90.2_+89.7_+90.3_+90.1_+90.2_+89.9_+90.0
//    NULL代表沒有使用, 以Ex2為例, 代表Plate1未使用.

    AnsiString sCMDStr="TEMP_";
    AnsiString asStr;

    if(USE_16_HEATER==eht4Heater)
    {
        for(int i=tcHead1; i<=tcChamber; i++)                                   //Ifor 20151006 : Add GPIB Temp Report
        {
            fTemp=999.9;
            bReturnNull=false;                                                  //jou 2015-10-30 修正GPIB 溫度常溫Chamber only,chamber 溫度異常
            if(bUT150Install[i] && bGetHeaterUsed(i))
            {
                if(Temperature.iIndexHeatMode==ChamberOnly ||
                   Temperature.iIndexHeatMode==SocketChamber)
                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                else
                    fTemp=atof(asGPIBTempShow[i].c_str());
            }
            else
            {                                                                   //JerryYang 20160126 回傳溫度要考慮是否關ARM或關SITE
                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    if(((i>=tcHead1 && i<=tcHead2) || i==tcChamber) && bGetHeaterUsed(i))   //JerryYang 20160126 關ARM或關SITE就不回傳溫度
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    if(((i>=tcHead3 && i<=tcHead4) || i==tcChamber) && bGetHeaterUsed(i))   //JerryYang 20160126 關ARM或關SITE就不回傳溫度
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull = true;
                    }
                }
                else
                {
                    if(bGetHeaterUsed(i) && ((i>=tcHead1 && i<=tcHead4) || i==tcChamber)) //JerryYang 20160126 要考慮關SITE
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                }
            }

            if(bReturnNull==true)
            {
                sTemp="NULL";
            }
            else
            {
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f", fTemp);
                else
                    sTemp.sprintf("-%1.1f", fTemp);
            }

            t+=sTemp;
            if(i<tcChamber)
            {
                t+="_";
            }
        }
    }
    //JerryYang 20160518 矽格湖口 柏均 要求16組加熱器的機型也要能支援4組加熱器的指令
    else if(USE_16_HEATER==eht16Heater       ||                                 //Steven 20120606 : 16溫控器 8Site使用Hontech頭
            USE_16_HEATER==eht16HeaterEJ1N   ||
            USE_16_HEATER==eht32HeaterEJ1N   ||                                 //Steven 20140923 : Index使用EJ1N版32組加熱器
            USE_16_HEATER==eht32HeaterKT4H   ||                                 //Steven 20150211 : Index使用KT4H版32組加熱器
            USE_16_HEATER==eht16HeaterDTME08 ||                                 //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            USE_16_HEATER==eht32HeaterDTME08 )                                  //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
    {
        bool bUsed[2][2]={{false, false}, {false, false}};                      //JerryYang 20160518 對應4組加熱器 Head1、Head2、Head3、Head4
        if(TestIF.iTestMode==SingleSite)
        {
            for(int i=0; i<4; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int iSinglesiteHead[4]={tcAa1, tcBa1, tcAa2, tcBa2};
                if(bUT150Install[iSinglesiteHead[i]] && bGet16HeaterUsedTo4Heater(iSinglesiteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[iSinglesiteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }

                if(bReturnNull==true)
                {
                    sTemp="NULL";
                }
                else
                {
                    if(fTemp>=0)
                        sTemp.sprintf("+%1.1f", fTemp);
                    else
                        sTemp.sprintf("-%1.1f", fTemp);
                }
                t+=sTemp;
                t+="_";
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        else if(TestIF.iTestMode==DualSite)
        {
            for(int i=0; i<4; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int iDualsiteHead[4]={tcAa1, tcAb1, tcAa2, tcAb2};
                if((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                   (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit))   //Sam 20220429 : 修正 1Cabcle layout 溫度回傳問題
                {
                    iDualsiteHead[0]=tcAa1;
                    iDualsiteHead[1]=tcBa1;
                    iDualsiteHead[2]=tcAa2;
                    iDualsiteHead[3]=tcBa2;
                }

                if(bUT150Install[iDualsiteHead[i]] &&
                   bGet16HeaterUsedTo4Heater(iDualsiteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[iDualsiteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }

                if(bReturnNull==true)
                {
                    sTemp="NULL";
                }
                else
                {
                    if(fTemp>=0)
                        sTemp.sprintf("+%1.1f",fTemp);
                    else
                        sTemp.sprintf("-%1.1f",fTemp);
                }
                t+=sTemp;
                t+="_";
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false)                                //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][1]==false)
                {
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][0]==false)
                {
                    sTemp3.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][1]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 1x3
        else if(TestIF.iTestMode==QualSite1X4)
        {
            //Richard 20220809 SG湖口
            //==>
            int iOneCable[8]={tcAa1, tcBa1, tcAb1, tcBb1, tcAa2, tcBa2, tcAb2, tcBb2};
            int iOtherCable[8]={tcAa1, tcAb1, tcAc1, tcAd1, tcAa2, tcAb2, tcAc2, tcAd2};
            int i1X4siteHead[8];
            for(int j=0; j<8; j++)
            {
                if(USE_16_HEATER==eht4Heater ||
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))
                {
                    i1X4siteHead[j]=iOneCable[j];
                }
                else
                {
                    i1X4siteHead[j]=iOtherCable[j];
                }
            }

            for(int i=0; i<8; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;

                if(bUT150Install[i1X4siteHead[i]] &&
                   bGet16HeaterUsedTo4Heater(i1X4siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i1X4siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(USE_16_HEATER==eht4Heater ||
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))   //jou 2015-10-15  : 16溫控器 1條線版本)
                {
                    if(i1X4siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1]))
                    {
//                        bUsed[0][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcBa1 && bUsed[0][0]==false && bReturnNull==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2]||bTestSiteUse[0][0][3]))
                    {
//                        bUsed[0][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcBb1 && bUsed[0][1]==false && bReturnNull==false )
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0]||bTestSiteUse[1][0][1]))
                    {
//                        bUsed[1][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcBa2 && bUsed[1][0]==false && bReturnNull==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2]||bTestSiteUse[1][0][3]))
                    {
//                        bUsed[1][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcBb2 && bUsed[1][1]==false && bReturnNull==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }
            //<==
                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false)    //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][2]==false && bTestSiteUse[0][0][3]==false)
                {
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false)
                {
                    sTemp3.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][2]==false && bTestSiteUse[1][0][3]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x1
        else if(TestIF.iTestMode==QualSite2X2)
        {
            for(int i=0; i<8; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i2x2siteHead[8]={tcAa1, tcBa1, tcAb1, tcBb1, tcAa2, tcBa2, tcAb2, tcBb2};
                if(bUT150Install[i2x2siteHead[i]] && bGet16HeaterUsedTo4Heater(i2x2siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i2x2siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(i2x2siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                {
                    bUsed[0][0]=true;
                }
                else if(i2x2siteHead[i]==tcBa1 && bUsed[0][0]==false)
                {
                    bUsed[0][0]=true;
                }
                else if(i2x2siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][1])
                {
                    bUsed[0][1]=true;
                }
                else if(i2x2siteHead[i]==tcBb1 && bUsed[0][1]==false)
                {
                    bUsed[0][1]=true;
                }
                else if(i2x2siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                {
                    bUsed[1][0]=true;
                }
                else if(i2x2siteHead[i]==tcBa2 && bUsed[1][0]==false)
                {
                    bUsed[1][0]=true;
                }
                else if(i2x2siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][1])
                {
                    bUsed[1][1]=true;
                }
                else if(i2x2siteHead[i]==tcBb2 && bUsed[1][1]==false)
                {
                    bUsed[1][1]=true;
                }
                else
                {
                    bwrite=false;
                }

                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][1][0]==false)    //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][1]==false && bTestSiteUse[0][1][1]==false)
                {
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][1][0]==false)
                {
                    sTemp3.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][1]==false && bTestSiteUse[1][1][1]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x2N
        else if(TestIF.iTestMode==_6Site2X3)
        {
            for(int i=0; i<12; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i6siteHead[12]={tcAa1, tcAb1, tcAc1, tcBa1, tcBb1, tcBc1, tcAa2, tcAb2, tcAc2, tcBa2, tcBb2, tcBc2};
                if(bUT150Install[i6siteHead[i]] && bGet16HeaterUsedTo4Heater(i6siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i6siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(i6siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][1])
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcAc1 && bUsed[0][0]==false)
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][0])
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcBb1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][1])
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcBc1 && bUsed[0][1]==false)
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][1])
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcAc2 && bUsed[1][0]==false)
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][0])
                {
                    bUsed[1][1]=true;
                }
                else if(i6siteHead[i]==tcBb2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][1])
                {
                    bUsed[1][1]=true;
                }
                else if(i6siteHead[i]==tcBc2 && bUsed[1][1]==false)
                {
                    bUsed[1][1]=true;
                }
                else
                {
                    bwrite=false;
                }

                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1, sTemp2, sTemp3, sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false && bTestSiteUse[0][0][2]==false)
                {
                    sTemp1.sprintf("NULL");
                }

                if(bTestSiteUse[0][1][0]==false && bTestSiteUse[0][1][1]==false && bTestSiteUse[0][1][2]==false)
                {
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false && bTestSiteUse[1][0][2]==false)
                {
                    sTemp3.sprintf("NULL");
                }

                if(bTestSiteUse[1][1][0]==false && bTestSiteUse[1][1][1]==false && bTestSiteUse[1][1][2]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x3N
        else if(TestIF.iTestMode==_8Site2X4)
        {
            int iHeadNum;                                                       //JerryYang 20160613 使用NS8000也要支援GPIB溫度指令
            if((IniConfig.bSIGURDFunction &&
               ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit))) ||      //Sam 20210524 2x4 新增一條線版本
                TestIF_File.bNS8000CS)
                iHeadNum=4;
            else
                iHeadNum=16;

            for(int i=0; i<iHeadNum; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i8siteHead[16]={tcAa1, tcAb1, tcBa1, tcBb1, tcAc1, tcAd1, tcBc1, tcBd1, tcAa2, tcAb2, tcBa2, tcBb2, tcAc2, tcAd2, tcBc2, tcBd2};
                int iNS8siteHead[4]={tcAa1, tcAb1, tcAa2, tcAb2};
                int ib1CablesiteHead[4]={tcAa1, tcBa1, tcAa2, tcBa2};           //Sam 20210524 2x4 新增一條線版本
                bool bwrite=true;
                if(IniConfig.bSIGURDFunction &&
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))  //Sam 20210524 2x4 新增一條線版本
                {
                    if(bUT150Install[ib1CablesiteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(ib1CablesiteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[ib1CablesiteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }

                    if(ib1CablesiteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1] || bTestSiteUse[0][1][0] || bTestSiteUse[0][1][1]))
                    {
                        bUsed[0][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcAa1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2] || bTestSiteUse[0][0][3] || bTestSiteUse[0][1][2] || bTestSiteUse[0][1][3]))
                    {
                        bUsed[0][1]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(ib1CablesiteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0] || bTestSiteUse[1][0][1] || bTestSiteUse[1][1][0] || bTestSiteUse[1][1][1]))
                    {
                        bUsed[1][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcAa2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2] || bTestSiteUse[1][0][3] || bTestSiteUse[1][1][2] || bTestSiteUse[1][1][3]))
                    {
                        bUsed[1][1]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }
                else if(TestIF_File.bNS8000CS==true)
                {
                    if(bUT150Install[iNS8siteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(iNS8siteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[iNS8siteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }

                    if(iNS8siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1] || bTestSiteUse[0][1][0] || bTestSiteUse[0][1][1]))
                    {
                        bUsed[0][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAa1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2] || bTestSiteUse[0][0][3] || bTestSiteUse[0][1][2] || bTestSiteUse[0][1][3]))
                    {
                        bUsed[0][1]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(iNS8siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0] || bTestSiteUse[1][0][1] || bTestSiteUse[1][1][0] || bTestSiteUse[1][1][1]))
                    {
                        bUsed[1][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAa2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2] || bTestSiteUse[1][0][3] || bTestSiteUse[1][1][2] || bTestSiteUse[1][1][3]))
                    {
                        bUsed[1][1]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }  //
                    else
                    {
                        bwrite=false;
                    }
                }
                else
                {
                    if(bUT150Install[i8siteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(i8siteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[i8siteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }

                    if(i8siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][1])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][1][0])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcBb1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcAc1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][2])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcAd1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][3])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcBc1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][2])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcBd1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(i8siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][1])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][1][0])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcBb2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcAc2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][2])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcAd2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][3])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcBc2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][2])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcBd2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }

                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false &&      //左邊全關
                   bTestSiteUse[0][1][0]==false && bTestSiteUse[0][1][1]==false)
                {
                    sTemp1.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][2]==false && bTestSiteUse[0][0][3]==false &&
                   bTestSiteUse[0][1][2]==false && bTestSiteUse[0][1][3]==false)
                {
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false &&
                   bTestSiteUse[1][1][0]==false && bTestSiteUse[1][1][1]==false)
                {
                    sTemp3.sprintf("NULL");
                }

                if(bTestSiteUse[1][0][2]==false && bTestSiteUse[1][0][3]==false &&
                   bTestSiteUse[1][1][2]==false && bTestSiteUse[1][1][3]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
       else if(TestIF.iTestMode==_16Site2X8)                                   //Sam 20241128 : 新增 2X8 溫度回傳
        {
            int i2X8_HeaterOneByOne[]={tcAa1, tcBa1, tcAb1, tcBb1, tcAc1, tcBc1, tcAd1, tcBd1,
                                       tcAe1, tcBe1, tcAf1, tcBf1, tcAg1, tcBg1, tcAh1, tcBh1,
                                       tcAa2, tcBa2, tcAb2, tcBb2, tcAc2, tcBc2, tcAd2, tcBd2,
                                       tcAe2, tcBe2, tcAf2, tcBf2, tcAg2, tcBg2, tcAh2, tcBh2};

            int i2X8_HeaterOneByTwo[]={tcAa1, tcBa1, tcAb1, tcBb1, tcAc1, tcBc1, tcAd1, tcBd1,
                                       tcAa2, tcBa2, tcAb2, tcBb2, tcAc2, tcBc2, tcAd2, tcBd2};
            int iHeater=0;
            int iAdrr=0;

            if(Temperature.iIndexHeatMode==ChamberOnly || Temperature.iIndexHeatMode==SocketChamber)
            {
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f",fTemp);
                else
                    sTemp.sprintf("-%1.1f",fTemp);

                for(int iCol=0; iCol<8; iCol++)
                {
                    for(int iRow=0; iRow<2; iRow++)
                    {
                        if(bTestSiteUse[0][iRow][iCol])
                            t+=sTemp+"_";
                        else
                            t+="NULL_";
                    }
                }
                for(int iCol=0; iCol<8; iCol++)
                {
                    for(int iRow=0; iRow<2; iRow++)
                    {
                        if(bTestSiteUse[1][iRow][iCol])
                            t+=sTemp+"_";
                        else
                            t+="NULL_";
                    }
                }
            }
            else
            {
                if(TestIF_File.bUse32Heater)
                    iHeater=sizeof(i2X8_HeaterOneByOne)/sizeof(int);
                else
                    iHeater=sizeof(i2X8_HeaterOneByTwo)/sizeof(int);

                for(int i=0; i<iHeater; i++)
                {
                    if(TestIF_File.bUse32Heater)
                        iAdrr=i2X8_HeaterOneByOne[i];
                    else
                        iAdrr=i2X8_HeaterOneByTwo[i];
                    if(bUT150Install[iAdrr] &&bGetHeaterUsed(iAdrr))
                    {
                        fTemp=atof(asGPIBTempShow[iAdrr].c_str());
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                        t+=sTemp+"_";
                    }
                    else
                    {
                         t+="NULL_";
                    }
                }
            }
        }

        for(int i=tcSocket; i<=tcChamber; i++)
        {
            fTemp=999.9;
            bReturnNull=false;
            if(bUT150Install[i] && bGetHeaterUsed(i))
            {
                if(Temperature.iIndexHeatMode==ChamberOnly ||
                   Temperature.iIndexHeatMode==SocketChamber)
                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                else
                    fTemp=atof(asGPIBTempShow[i].c_str());
            }
            else
            {
//                if(bGetHeaterUsed(i) && i==tcChamber && ATC_SYSTEM==eNonChamber)
//                {
//                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
//                }
//                else
                {
                    bReturnNull=true;
                }
            }

            if(bReturnNull==true)
            {
                sTemp="NULL";
            }
            else
            {
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f",fTemp);
                else
                    sTemp.sprintf("-%1.1f",fTemp);
            }

            t+=sTemp;
            if(i<tcChamber)
            {
                t+="_";
            }
        }
    }

    for(int i=tcHotPlate1; i<=tcShuttle2; i++)
    {
        fTemp = 999.9;
        if(bUT150Install[i] && bGetHeaterUsed(i))
        {
            fTemp=atof(asGPIBTempShow[i].c_str());
            if(fTemp >= 0)
            {
                sTemp.sprintf("+%1.1f",fTemp);
            }
            else
            {
                sTemp.sprintf("-%1.1f",fTemp);
            }
        }
        else
        {
            sTemp="NULL";
        }

        sCMDStr+=sTemp;
        if(i<tcShuttle2)
        {
            sCMDStr+="_";
        }
    }

    asStr=sCMDStr+"_"+t+"\r\n";                                                 //Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    SendMSG_CMD(MSG_CMD_GetNowAllTemp, asStr);
}

/* ---- golden Command.cpp:5640-7299 ---- */
void TfMain::GetCZAllMassTemp()                                      //JerryYang 20160324 回傳all mass temp
{
    AnsiString asNULL="NULL", t="";

    if(TestIF.iTestMode==SingleSite)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcHead1]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s", asGPIBTempShow[tcAa1]);
            }
            else
            {
                t.sprintf("%s", asGPIBTempShow[tcHead1]);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcHead3]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s", asGPIBTempShow[tcAa2]);
            }
            else
            {
                t.sprintf("%s", asGPIBTempShow[tcHead3]);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcChamber]);
            }
            else
            {
                t.sprintf("NULL");
            }
        }
    }
    else if(TestIF.iTestMode==DualSite)                                         // Dual Site 1x2
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber], asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL ,
                                   LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                   LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber], asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                   LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                   LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                   (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s", asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s", asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                      LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                      LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                      LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                      LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                      (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcHead3]:asNULL,
                                      (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite1X4)                                      // Qual Site 1x4
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                         LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead4]:asNULL,
                                         LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcHead3]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcHead4]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber],
                                   asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                   LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                   LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber],
                                   asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                   LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                   LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                   (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2)                                      // Qual Site 2x2
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                                         LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                         LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                                         LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                                         LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     // Qual Site 2x2
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s" ,
                            LastSet.bUseTestSocket[0][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[1][0][1] ? asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                                               LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                                               LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAd2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4)                                        // 8 Site 2x4
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4] ? asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_12Site2X6)                                       //12 Site   //wei 20150702
    {
        if(IndexStatus==Z1Down_Z2Up ||
          (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf2]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][5] || LastSet.bUseTestSocket[1][0][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][5] || LastSet.bUseTestSocket[1][1][5])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //16 Site
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAg1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAh1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf1]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBg1]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBh1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcAg2]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcAh2]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcBf2]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcBg2]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcBh2]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcBd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][5] || LastSet.bUseTestSocket[1][0][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][6] || LastSet.bUseTestSocket[1][0][6])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][7] || LastSet.bUseTestSocket[1][0][7])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][5] || LastSet.bUseTestSocket[1][1][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][6] || LastSet.bUseTestSocket[1][1][6])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][7] || LastSet.bUseTestSocket[1][1][7])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAb1]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAc1]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAd1]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBb1]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBc1]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead4]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcChamber]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAg2]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAh2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBg2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBh2]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcAg1]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcAh1]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcBf1]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcBg1]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcBh1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else
    {
        t.sprintf("UNKNOWN", sizeof(t));
    }

    SendMSG_CMD(MSG_CMD_AllMassTemp, t);
}

// -- FW3-WB APPEND -- end (WriteTemp_NS / WriteNowAllTempData / GetCZAllMassTemp) --
