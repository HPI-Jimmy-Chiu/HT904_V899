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

// AI(W906-FW3-WC) 20260818: additional #includes for symbols the FW3-WC GROUP
// below needs that no earlier Command.cpp #include (see top of file) pulls
// in. Placed here rather than sorted into the top block because this file's
// contract is append-only past "-- FW3-WB APPEND -- end" -- existing lines
// (including the existing #include block) are not to be touched. All four
// are header-guarded, so this is safe regardless of what else already
// (transitively) included them.
#include "cUnitConvert.h"    // DoStructUnitConvert (SetTrayBinByDLL / SetSiteMapByDLL)
#include "forms/fContactCT.h" // fContactCT->ShowFormComp (SetSiteMapByDLL)
#include "forms/fHome.h"      // fHome->fShow (SettingsIsWindowOpened)
#include "forms/fSetup.h"     // fSetup->fShow (SettingsIsWindowOpened; also SetSiteMapByDLL's GATE comment)
#include "atester_shims.h"    // fContact->fShow (SettingsIsWindowOpened)
#include "cSocket.h"          // TastCategory (GetBinCountByDLL / GetBinCountPerSiteByDLL)
#include "MainCalcCore.h"     // GetShtModeFlag / ComputeCanChangeRealDummy

// =============================================================================
//  FW3-WC GROUP -- the ByDLL family
//
//  Translation wave: FW-3 Wave C
//  Translator: AI(W906-FW3-WC) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Command.cpp (15,273 lines, cp950)
//
//  ROLE
//  ----
//  26 golden TfMain:: methods, golden Command.cpp :8311-9994, declared in
//  forms/fMain.h's new "FW3-WC ADD" block. These are the external-DLL
//  get/set entry points (Epson-style ATP DLL bridge): tray-bin map, site
//  map, temperature set/get, bin/sort counters, handler/alarm status, and
//  the GPIB site-on-off helpers layered on top of them. Per the task brief:
//  bodies are translated, NO caller (TCP dispatch / DLL export table) is
//  wired up.
//
//  NEVER-WAVE EXCLUSION: RemoteControl (golden :9673-9717) is deliberately
//  skipped -- not declared in fMain.h, not translated here, no stand-in.
//
//  WAVE SCOPE (every golden method, golden line span, ACTIVE or GATED-partial)
//  ------------------------------------------------------------------------------
//    CreateAndOpenMap         :8311-8358  ACTIVE (2 GOLDEN BUGs, see below)
//    SetTrayBinByDLL          :8360-8515  GATED-partial (fBinSel x6, fShowBinSelect x1)
//    GetTrayBinByDLL          :8517-8564  ACTIVE (1 vclcompat-surface substitution)
//    SetSiteMapByDLL          :8566-8998  GATED-partial (fSetup x3, fTestCategory x1)
//    GetSiteMappingByDLL      :9000-9213  ACTIVE (1 vclcompat-surface substitution; 1 GOLDEN BUG)
//    GetSiteMappingForSIGURD  :9215-9236  ACTIVE
//    GetSiteOnOffByChannel    :9237-9257  ACTIVE (1 MainCalcCore substitution)
//    SetSiteOnOffByChannel    :9258-9283  ACTIVE (1 MainCalcCore substitution)
//    SetSiteOnOff             :9284-9303  ACTIVE
//    ParseHexToBoolArray      :9304-9326  ACTIVE (1 vclcompat-surface substitution)
//    HexCharToBits            :9327-9343  ACTIVE
//    SetTempByDLL             :9344-9425  GATED-partial (ChangeTempMode x2, same absence as FW3-WA)
//    GetTempSettingByDLL      :9426-9456  ACTIVE
//    FTPDownloadByDLL         :9457-9494  GATED-partial (fFTPClient, whole body)
//    GetBinCountByDLL         :9495-9525  ACTIVE
//    ClearBinCountByDLL       :9526-9541  ACTIVE
//    GetSortCountByDLL        :9542-9566  ACTIVE
//    ClearSortCountByDLL      :9567-9582  GATED-partial (fCounterClear)
//    GetHandlerStatusByDll    :9583-9636  GATED-partial (MyMessageBox, header-conflict; 1 MainCalcCore substitution)
//    GetAlarmStatusByDll      :9637-9672  ACTIVE
//    [RemoteControl           :9673-9717  EXCLUDED -- never-wave]
//    GetBinCountPerSiteByDLL  :9718-9777  ACTIVE
//    GetTempActualByDLL       :9778-9938  ACTIVE
//    SettingsIsWindowOpened   :9939-9964  GATED-partial (29 of 33 OR-terms absent, see function banner)
//    WriteSiteOnOff           :9965-9978  ACTIVE
//    AutoSiteOnOff            :9979-9985  ACTIVE
//    WriteNumOfSites          :9986-9994  ACTIVE
//
//  VCLCOMPAT-SURFACE SUBSTITUTIONS (behavior-preserving, not gates -- flagged
//  per pt-wave policy so the integration pass can review/veto each)
//  ------------------------------------------------------------------------------
//  (S1) GetTrayBinByDLL golden :8494 `cTemp=asBinSetting.AnsiLastChar();` --
//       vclcompat::AnsiString has no AnsiLastChar() (grep -rn "AnsiLastChar"
//       . -- 0 hits tree-wide, 20260818). Substituted with the equivalent
//       1-based `asBinSetting[asBinSetting.Length()]` (vclcompat/AnsiString.h:100
//       -- "operator[](i): i==1 is the first char"), which reads the exact same
//       last character golden's own AnsiLastChar() pointer would have
//       dereferenced. asBinSetting is never empty at this call site (seeded
//       with `AnsiString(iTrayNum)+":"` before the loop), so the 1-based
//       index is always in range.
//  (S2) GetSiteMappingByDLL's 17 `sprintf(cSiteInfo[i], "...%s...", asSiteTemp[...], ...)`
//       calls -- these are the real C-library ::sprintf (destination is a
//       plain `char cSiteInfo[32][10]`, not an AnsiString), not
//       AnsiString::sprintf's variadic-template overload (vclcompat/AnsiString.h:142,
//       which type-converts its args via `conv()` before formatting). Passing
//       a vclcompat::AnsiString object (a `std::string`-backed class, NOT
//       BCB6's single-pointer-layout AnsiString) through real C varargs is
//       undefined behaviour -- BCB6's own AnsiString only worked here because
//       of an ABI coincidence vclcompat does not share. Every `asSiteTemp[i][j]`
//       argument to a bare `sprintf(...)` call below is passed as
//       `asSiteTemp[i][j].c_str()` instead; this produces the byte-identical
//       formatted output golden intended, it does not change golden's string
//       content. Same posture as Command.cpp:2675-2680's SL2->Strings[j]
//       materialization (FW3-WA).
//  (S3) ParseHexToBoolArray golden :9316 `StrToIntDef("$" + hexStr, 0)` --
//       port StrToIntDef (vclcompat/SysUtils.cpp:72) is a plain
//       AnsiString::ToIntDef() wrapper, decimal-only; it does not honour
//       BCB6's native '$'-prefix hex parsing the way golden's own StrToIntDef
//       did, so translating this literally would silently return the 0
//       default instead of the intended hex value. Substituted with this
//       tree's own HexStrToInt() (vclcompat/SysUtils.cpp:81), which explicitly
//       documents accepting both "0x" and "$" hex prefixes for exactly this
//       reason. hexStr is length-checked to exactly 8 hex digits immediately
//       above, so HexStrToInt's own -1-on-unparseable-input path is
//       unreachable at this call site.
//  (S4) GetHandlerStatusByDll golden :9628 `fMain->CheckCanChangeRealDummy()==false`
//       -- CheckCanChangeRealDummy is NOT a declared TfMain member anywhere in
//       forms/fMain.h (grep -rn "CheckCanChangeRealDummy" forms/fMain.h -- 0
//       hits, 20260818); MainCalcCore.h:152-165 documents
//       `ComputeCanChangeRealDummy(bPlate1HasIC, bPlate2HasIC, bShuttleHasIC,
//       bIndexHasIC, bInArmSuckHasIC, bOutArmSuckHasIC)` as the deliberate
//       "Portable replacement for TfMain::CheckCanChangeRealDummy()" (its own
//       comment, verbatim), with every one of its six boolean parameters
//       already a confirmed-real, side-effect-free port symbol
//       (MOT[MMPlate1/MMPlate2].HasIC(), ShuttleHasIC(), IndexHasIC(),
//       InArmSuck/OutArmSuck.HasIC()). This is this tree's own established
//       "extract-calc-core" convention (KNOWLEDGE.md), not an invented
//       behavior -- flagged here because it is a substitution, not a literal
//       spelling match, so the integration pass can veto it in favor of a
//       GATE if it disagrees.
//  (S5) GetSiteOnOffByChannel golden :9241 / SetSiteOnOffByChannel golden
//       :9262, both `GetShtModeFlag()` -- not a declared TfMain member either
//       (grep -rn "GetShtModeFlag" forms/fMain.h -- 0 hits, 20260818).
//       MainCalcCore.h:137-150 documents `ComputeShtModeFlag(iShuttleMode,
//       iShuttle_Sel)` as the "Portable replacement for
//       TfMain::GetShtModeFlag()" (its own comment, verbatim), with both
//       parameters resolved from the real, confirmed `TestIF.iShuttleMode`/
//       `TestIF.iShuttle_Sel` (cprod.h SYSTEM_TEST_IF). Same posture as (S4).
//
//  HEADER-CONFLICT GATE (not an absence -- a real facade this TU cannot
//  safely #include)
//  ------------------------------------------------------------------------------
//  GetHandlerStatusByDll golden :9611 `MyMessageBox->fShow` -- MyMessageBox
//  (TMyMessageBoxShim, acatchtray_shims.h) is a real, populated facade, but
//  that header's `NewRecordProcess(AnsiString,AnsiString,AnsiString="")`
//  declaration conflicts with the different 3rd-parameter default
//  (`Debug=" "`) canary_support.h already puts on the same function, already
//  visible in this TU -- a hard "default argument given for parameter 3"
//  compile error the moment both are included together (confirmed by
//  actually trying it this pass). Pre-existing conflict between two
//  already-committed headers, neither of which is one of this wave's two
//  writable files; gated at the call site instead of pulling in the
//  conflicting #include. See that function's own GATE comment for the full
//  detail.
//
//  GOLDEN BUGS / ODDITIES (translated literally, not "fixed")
//  ------------------------------------------------------------------------------
//  (B1) CreateAndOpenMap golden :8316/:8317/:8330/:8331 `memset(CmdData, 0x00,
//       sizeof(CmdData))` -- CmdData is `INFO *`, so sizeof(CmdData) is
//       sizeof(a pointer) (4 or 8 bytes), not sizeof(INFO). This memset only
//       ever zeroes the pointer-sized prefix of the mapped INFO block, never
//       the whole struct. Translated literally (both occurrences).
//  (B2) CreateAndOpenMap golden :8327 `CreateFileMapping(..., sizeof(CmdData), "HandlerMemory")`
//       -- same sizeof(CmdData) mistake sizes the mapping OBJECT itself
//       (requesting only a pointer-sized backing region), while the very next
//       lines' MapViewOfFile calls request a `sizeof(INFO)`-sized VIEW of that
//       undersized mapping. Translated literally; this is a real
//       memory-safety hazard the moment any caller actually invokes
//       CreateAndOpenMap on a real OS (untested here -- ByDLL callers are
//       explicitly out of this wave's scope).
//  (B3) GetSiteMappingByDLL golden :8437-8899's format strings (e.g.
//       `"[01,%s,%02d]"`) each produce exactly 10 printable characters plus a
//       NUL terminator (11 bytes), but `cSiteInfo` is declared
//       `char cSiteInfo[32][10]` (golden :9003, a function LOCAL -- the :693
//       citation this banner first carried was wrong, corrected at
//       integration 20260818) -- 10 bytes per row. Every
//       `sprintf(cSiteInfo[i], ...)` call golden makes overflows its
//       destination row by exactly 1 byte. Translated literally (array
//       dimensions and format strings both copied verbatim); flagged as a
//       real stack-buffer-overflow hazard for the same reason as (B2).
//  (B4) GetSiteMappingByDLL golden :8899-8901 builds a full `cSiteInfo[0..31]`
//       table (one formatted entry per site) but the function's only output
//       write is `strcpy(cSiteMap, cSiteInfo[0]);` -- ONLY site 1's entry is
//       ever copied to the caller's buffer; cSiteInfo[1..31] are computed and
//       discarded. Every caller of this function (WriteSiteOnOff below,
//       golden :9973) therefore reports site 1 only over GPIB for any
//       machine with more than 1 site. Translated literally.
// =============================================================================

/* ---- golden Command.cpp:8311-8358 ---- */
bool TfMain::CreateAndOpenMap()
{
    HANDLE hFileMap=OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "HandlerMemory");            //Open Map
    if(hFileMap!=NULL)
    {
        // GOLDEN BUG (B1)/(B2) -- see FW3-WC GROUP banner above for the full citation.
        CmdData=(INFO *)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(INFO));    //映射到CmdData結構
        memset(CmdData, 0x00, sizeof(CmdData));
        for(int i=0; i<eCommandTotal; i++)
        {
            CmdData->bCommandList[i]=false;
        }
        sprintf(CmdData->cVersion,"%s" , ATPDLLVersion);    //JerryYang 20230721 : dll版本卡控
        return true;
    }
    else
    {
        // GOLDEN BUG (B2) -- see FW3-WC GROUP banner above for the full citation.
        hFileMapping=CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CmdData), "HandlerMemory");

        if(hFileMapping!=NULL)
        {
            if (GetLastError()==ERROR_ALREADY_EXISTS)
            {
                ShowMyMessage("Mapping file already created!");
                CloseHandle(hFileMapping);
            }

            hFileMap=OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "HandlerMemory");            //Open Map
            if (hFileMap!=NULL)
            {
                // GOLDEN BUG (B1) -- see FW3-WC GROUP banner above for the full citation.
                CmdData=(INFO *)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(INFO));    //映射到CmdData結構
                memset(CmdData, 0x00, sizeof(CmdData));
                for(int i=0; i<eCommandTotal; i++)
                {
                    CmdData->bCommandList[i]=false;
                }
                sprintf(CmdData->cVersion,"%s" , ATPDLLVersion);    //JerryYang 20230721 : dll版本卡控
                return true;
            }
            else
            {
                memset(CmdData, 0x00, sizeof(CmdData));
                ShowMyMessage("Open File Mapping Fail!");
            }
        }
    }

    return false;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:8360-8515 ---- */
int TfMain::SetTrayBinByDLL(int iTrayNum, LPSTR asCategories, int iFail)
{
    char cBinTemp[3];
    int i, j, iTrayBin[256],iBin;
    char cTemp[256];
    memset(iTrayBin,'\0', sizeof(iTrayBin));
    memset(cTemp,'\0', sizeof(cTemp));

    strncpy(cTemp, asCategories, sizeof(cTemp));
    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    if(SystemStart==true)   //保護
    {
        return -4;  //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;  //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine() || LastSet.iTester==OFF_LINE)
    {
        return -4;  //Operation not Allowed
    }

    if(iTrayNum<0 || iTrayNum>6)
        return -3;   //Parameter Error

    if(iFail>0 || iFail<-1)
        return -3;   //Parameter Error

    if(IniConfig.bFTBin2RTBin==false)
        return -4;

    for(i=0; i<iTestBinCount; i++)
    {
        if(cTemp[0]!='\0')
        {
            SplitStrByDotSpaceOnly(cTemp, cBinTemp, 4);

            iBin=StrToIntDef(cBinTemp, -1);
            if(iBin>=iTestBinCount || iBin<0)
            {
                // GATE(FW3-WC) golden :8407 `fBinSel->ReadFile(false, false, "");` --
                // fBinSel (golden TfBinSel*) has NO translated home anywhere in the
                // port (grep -rn "class TfBinSel" --include=*.h . -- 0 hits, 20260818;
                // same absence already established by FW3-WA, Command.cpp:2635-2646).
                // The line only refreshes that form's OWN grid; the function's real,
                // observable effect is the `return -3` immediately after it, which
                // stays ACTIVE un-gated.
#if 0
                fBinSel->ReadFile(false, false, "");
#endif
                return -3;   //Parameter Error
            }

            if(cBinTemp[0]=='0' && iFail==0)
            {
#if 0 // GATE(FW3-WC): fBinSel absent -- see golden :8407's GATE above for the full citation
                fBinSel->ReadFile(false, false, "");
#endif
                return -3;
            }

            if(cBinTemp[0]=='0' && iFail==-1)  //比照EPSON的指令, Bin0當作是Error bin
            {
                BinSelect[iTestRunMode].IfErrorT3=iTrayNum-1;
            }
            else
            {
                if(iFail==0 && BinSelect[iTestRunMode].IfErrorT3==iTrayNum-1)
                {
                    BinSelect[iTestRunMode].IfErrorT3=5;
                }
                iTrayBin[i]=atoi(cBinTemp);
            }

            strcpy(cBinTemp,"");
        }
    }

    for(i=0; i<eTrayCount; i++)
    {
        if(i==iTrayNum-1)
        {
            if(iFail==0)    //Pass Tray
            {
                BinSelect[iTestRunMode].iStackDefFailCate[i]=0;
                bSetByDLL=true;
            }
            else if(iFail==-1)   //Fail Tray
            {
                BinSelect[iTestRunMode].iStackDefFailCate[i]=1;
                bSetByDLL=true;
            }
            else
            {
                bSetByDLL=false;
#if 0 // GATE(FW3-WC): fBinSel absent -- see golden :8407's GATE above for the full citation
                fBinSel->ReadFile(false, false, "");
#endif
                return -3;      //Parameter Error
            }
        }
    }

    // GATE(FW3-WC) golden :8437-8483 `fBinSel->sBinTraySetT3Pos[...]->Count / ->Strings[i] /
    // ->spbSaveClick(this)` -- fBinSel (golden TfBinSel*, the whole Bin-Select
    // mapping VCL form) has NO translated home anywhere in the port (see the
    // :8407 GATE above for the grep citation; same absence FW3-WA's own
    // WriteSetBinMap gate already established, Command.cpp:2635-2646). Every
    // fBinSel-> line in this block only ever mutates that form's OWN internal
    // grid state (sBinTraySetT3Pos) and has no other port-visible effect --
    // `iTrayBin[]`'s only reader is inside this same gated block, and the
    // function's real state mutation (BinSelect[iTestRunMode].IfErrorT3 /
    // .iStackDefFailCate) already happened, ACTIVE, in the loops above.
    // `RT`/`FT` below are golden's bare bin-run-mode literals; the port names
    // the same MachineType.h enum `eBinRT`/`eBinFT` (MachineType.h:627-628),
    // exactly as FW3-WA's WriteSetBinMap already spells it (Command.cpp:2648
    // `eBinFT`) -- so even inside this #if 0 the identifiers are respelled to
    // eBinRT/eBinFT rather than left as bare RT/FT (which do not exist in this
    // tree under those names) for anyone who un-gates this block later.
#if 0
    for(i=0; i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count; i++)
    {
        if(fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]==iTrayNum)
        {
            if(iTestRunMode==eBinRT)
            {
                if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
                (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
                {
                    fBinSel->sBinTraySetT3Pos[eBinFT]->Strings[i]=AnsiString(0);
                }
                else
                {
                    fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(0);
                }
            }
            else
            {
                fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(0);
            }
        }
    }

    for(i=0; i<iTestBinCount; i++)
    {
        for(j=0; j<iTestBinCount; j++)
        {
            if(i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count)
            {
                if(i==iTrayBin[j] && iTrayBin[j]!=0)
                {
                    if(iTestRunMode==eBinRT)
                    {
                        if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
                        (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
                        {
                            fBinSel->sBinTraySetT3Pos[eBinFT]->Strings[i]=AnsiString(iTrayNum);
                        }
                        else
                        {
                            fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(iTrayNum);
                        }
                    }
                    else
                    {
                        fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(iTrayNum);
                    }
                }
            }
        }
    }

    fBinSel->spbSaveClick(this);
#endif
    DoStructUnitConvert();
    // GATE(FW3-WC) golden :8513 `fShowBinSelect->ShowBinSel();` -- ShowBinSel is
    // explicitly forms/fShowBinSelect.h's own documented "WAVE B QUEUE" item
    // (golden span :388-757, ~250-widget ctor-population surface; see that
    // header's line 62) -- not yet translated, no stand-in.
#if 0
    fShowBinSelect->ShowBinSel();
#endif
    SetWorkParameter();                                                         //Steven 20120130 : 存檔後要重新load參數
    bSetByDLL=false;
    return 0;
}
//------------------------------------------------------------------------------
int TfMain::GetTrayBinByDLL(int iTrayNum)
{
    AnsiString asBinSetting="";
    asBinSetting=AnsiString(iTrayNum)+":";
    int i;

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    if(iTrayNum<1 || iTrayNum>6)    //tray is not defined
        return -1;

    for(i=1; i<iTestBinCount; i++)  //從Bin1開始, 比照Epson所定義的格式
    {
        if(Prod.iT6PosCate[i]==iTrayNum)
        {
            // AI(W906-FW3-WC) 20260818: substitution (S1) -- see FW3-WC GROUP
            // banner above for the full citation. golden declares
            // `char *cTemp="A";` once before the loop and reassigns it via
            // `cTemp=asBinSetting.AnsiLastChar();` on every iteration before
            // ever dereferencing it -- the initial "A" is dead in golden too.
            // Dropped the now-purposeless pointer declaration entirely and
            // read the last character directly into a fresh local each
            // iteration instead of carrying an unused `char *cTemp`.
            char cLastChar = asBinSetting[asBinSetting.Length()];
            if(cLastChar==':')
            {
                asBinSetting=asBinSetting+AnsiString(i);
            }
            else
            {
                asBinSetting=asBinSetting+","+AnsiString(i);
            }
        }
    }

    if(Prod.iIfErrorT6==iTrayNum) //比照EPSON的指令, Bin0當作是Error bin
    {
        asBinSetting=asBinSetting+",0";
    }
    asBinSetting+=";";

    if(BinSelect[iTestRunMode].iStackDefFailCate[iTrayNum-1]==0)                //QQQ
    {
        asBinSetting+="0";
    }
    else
    {
        asBinSetting+="-1";
    }
    strcpy(CmdData->cGetBinCategories_Cmd3, asBinSetting.c_str());
    return 0;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:8566-8998 ---- */
int TfMain::SetSiteMapByDLL(LPSTR cSiteMap, int iNoOfSites)
{
    int i, j;
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;
//    char *szSiteMap="[01,01,01][02,03,00][03,02,01][04,--,00]";
    AnsiString strTemp[32]; //最大32site
    bool bTempDutOnOff[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    bool bSiteOnOff[32];
    char *cSiteMapSplit[32];
    int iCount=0;
    char cSiteTemp[2];
    int iSiteMap[32];
    int iTemp=0;

    char cTemp[256];
    memset(cTemp, '\0', sizeof(cTemp));
    strncpy(cTemp, cSiteMap, sizeof(cTemp));

    if(SystemStart==true)   //保護
    {
        return -4;  //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;  //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
    {
        return -4;  //Operation not Allowed
    }

    if(iNoOfSites!=iSiteCnt)
    {
        return -3;   //Parameter Error
    }

    char *temp=strtok(cTemp, " []");
    while(temp!=NULL)
    {
        cSiteMapSplit[iCount]=temp;

        temp = strtok(NULL, " []");

        if(iCount>=iNoOfSites)
        {
            //資料異常
            return -3;   //Parameter Error
        }
        iCount++;
    }

    for(i=0; i<iNoOfSites; i++)
    {
        for(j=0; j<3; j++)
        {
            SplitStrByDotSpaceOnly(cSiteMapSplit[i], cSiteTemp, 3);
            if(j==0)        //表示site的位置
            {
                iTemp=atoi(cSiteTemp);
                if(iTemp!=i+1)
                {
                    return -3;
                }

                if(iTemp>iSiteCnt || iTemp<0)
                {
                    return -3;   //Parameter Error
                }
            }
            else if(j==1)   //表示Site map的設定
            {
                if(cSiteTemp[0]=='-' && cSiteTemp[1]=='-')
                {
                    iSiteMap[i]=0;
                }
                else
                {
                    iSiteMap[i]=atoi(cSiteTemp);
                }
            }
            else if(j==2)   //表示開關site
            {
                iTemp=StrToIntDef(cSiteTemp, -1);
                if(iTemp<0 || iTemp>1)
                    return -3;   //Parameter Error

                bSiteOnOff[i]=atoi(cSiteTemp);
                if(iSiteMap[i]==0 && bSiteOnOff[i]==true)
                {
                    return -3;   //Parameter Error
                }
            }
        }
    }

    for(int i=0; i<iSiteCnt; i++)   //JerryYang 20160113 Sitemap防護
    {
        if(iSiteMap[i]>iSiteCnt || iSiteMap[i]<0)
        {
            return -3;  //Parameter Error
        }
        for(int j=i+1; j<iSiteCnt; j++)
        {
            if(iSiteMap[i]==iSiteMap[j] && iSiteMap[i]!=0)
            {
                return -3;  //Parameter Error
            }
        }
    }

    if(TestIF.iTestMode==SingleSite)//*   SingleSite
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
    }
    else if(TestIF.iTestMode==DualSite)//*   DualSite
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
    }
    else if(TestIF_File.iTestMode==TriSite1X3)//
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
        bTempDutOnOff[0][2]=bSiteOnOff[2];
    }
    else if(TestIF.iTestMode==QualSite1X4)//*     QualSite1X4    _8Site1X4
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];
        TestIF_File.iSiteMap[0][3]=iSiteMap[3];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
        bTempDutOnOff[0][2]=bSiteOnOff[2];
        bTempDutOnOff[0][3]=bSiteOnOff[3];
    }
    else if(TestIF_File.iTestMode==DualSite2x1) //*    DualSite2x1
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
    }
    else if(TestIF.iTestMode==QualSite2X2 || //*    QualSite2X2
            TestIF.iTestMode==QualSite2X2N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
    }
    else if(TestIF_File.iTestMode==_6Site2X3 || //*      _6Site2X3
            TestIF_File.iTestMode==_6Site2X3N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||//*    _8Site2X4
            TestIF_File.iTestMode==_8Site2X4N) //Wei 20231211 : 2X4NN Mode
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
    }
    else if(TestIF_File.iTestMode==_12Site2X6)//*    _12Site2X6
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
        bTempDutOnOff[0][5]=bSiteOnOff[10];
        bTempDutOnOff[1][5]=bSiteOnOff[11];
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //*     _16Site2X8
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];
        TestIF_File.iSiteMap[0][6]=iSiteMap[12];
        TestIF_File.iSiteMap[1][6]=iSiteMap[13];
        TestIF_File.iSiteMap[0][7]=iSiteMap[14];
        TestIF_File.iSiteMap[1][7]=iSiteMap[15];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
        bTempDutOnOff[0][5]=bSiteOnOff[10];
        bTempDutOnOff[1][5]=bSiteOnOff[11];
        bTempDutOnOff[1][6]=bSiteOnOff[12];
        bTempDutOnOff[0][6]=bSiteOnOff[13];
        bTempDutOnOff[1][7]=bSiteOnOff[14];
        bTempDutOnOff[0][7]=bSiteOnOff[15];
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[2][0]=bSiteOnOff[2];
        bTempDutOnOff[3][0]=bSiteOnOff[3];
        bTempDutOnOff[0][1]=bSiteOnOff[4];
        bTempDutOnOff[1][1]=bSiteOnOff[5];
        bTempDutOnOff[2][1]=bSiteOnOff[6];
        bTempDutOnOff[3][1]=bSiteOnOff[7];
        bTempDutOnOff[0][2]=bSiteOnOff[8];
        bTempDutOnOff[1][2]=bSiteOnOff[9];
        bTempDutOnOff[2][2]=bSiteOnOff[10];
        bTempDutOnOff[3][2]=bSiteOnOff[11];
        bTempDutOnOff[0][3]=bSiteOnOff[12];
        bTempDutOnOff[1][3]=bSiteOnOff[13];
        bTempDutOnOff[2][3]=bSiteOnOff[14];
        bTempDutOnOff[3][3]=bSiteOnOff[15];
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];
        TestIF_File.iSiteMap[0][4]=iSiteMap[16];
        TestIF_File.iSiteMap[1][4]=iSiteMap[17];
        TestIF_File.iSiteMap[2][4]=iSiteMap[18];
        TestIF_File.iSiteMap[3][4]=iSiteMap[19];
        TestIF_File.iSiteMap[0][5]=iSiteMap[20];
        TestIF_File.iSiteMap[1][5]=iSiteMap[21];
        TestIF_File.iSiteMap[2][5]=iSiteMap[22];
        TestIF_File.iSiteMap[3][5]=iSiteMap[23];
        TestIF_File.iSiteMap[0][6]=iSiteMap[24];
        TestIF_File.iSiteMap[1][6]=iSiteMap[25];
        TestIF_File.iSiteMap[2][6]=iSiteMap[26];
        TestIF_File.iSiteMap[3][6]=iSiteMap[27];
        TestIF_File.iSiteMap[0][7]=iSiteMap[28];
        TestIF_File.iSiteMap[1][7]=iSiteMap[29];
        TestIF_File.iSiteMap[2][7]=iSiteMap[30];
        TestIF_File.iSiteMap[3][7]=iSiteMap[31];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[2][0]=bSiteOnOff[2];
        bTempDutOnOff[3][0]=bSiteOnOff[3];
        bTempDutOnOff[0][1]=bSiteOnOff[4];
        bTempDutOnOff[1][1]=bSiteOnOff[5];
        bTempDutOnOff[2][1]=bSiteOnOff[6];
        bTempDutOnOff[3][1]=bSiteOnOff[7];
        bTempDutOnOff[0][2]=bSiteOnOff[8];
        bTempDutOnOff[1][2]=bSiteOnOff[9];
        bTempDutOnOff[2][2]=bSiteOnOff[10];
        bTempDutOnOff[3][2]=bSiteOnOff[11];
        bTempDutOnOff[0][3]=bSiteOnOff[12];
        bTempDutOnOff[1][3]=bSiteOnOff[13];
        bTempDutOnOff[2][3]=bSiteOnOff[14];
        bTempDutOnOff[3][3]=bSiteOnOff[15];
        bTempDutOnOff[0][4]=bSiteOnOff[16];
        bTempDutOnOff[1][4]=bSiteOnOff[17];
        bTempDutOnOff[2][4]=bSiteOnOff[18];
        bTempDutOnOff[3][4]=bSiteOnOff[19];
        bTempDutOnOff[0][5]=bSiteOnOff[20];
        bTempDutOnOff[1][5]=bSiteOnOff[21];
        bTempDutOnOff[2][5]=bSiteOnOff[22];
        bTempDutOnOff[3][5]=bSiteOnOff[23];
        bTempDutOnOff[0][6]=bSiteOnOff[24];
        bTempDutOnOff[1][6]=bSiteOnOff[25];
        bTempDutOnOff[2][6]=bSiteOnOff[26];
        bTempDutOnOff[3][6]=bSiteOnOff[27];
        bTempDutOnOff[0][7]=bSiteOnOff[28];
        bTempDutOnOff[1][7]=bSiteOnOff[29];
        bTempDutOnOff[2][7]=bSiteOnOff[30];
        bTempDutOnOff[3][7]=bSiteOnOff[31];
    }
    else
    {
        return -3;   //Parameter Error
    }

    // GATE(FW3-WC) golden :8977-8979 `fSetup->ScrollBar1Change(this);
    // fSetup->DoIniDataToForm(); fSetup->sbUpdateClick(this);` -- forms/fSetup.h's
    // TfSetup facade carries exactly ONE member (`bool fShow`, landed by the
    // W7-L2 substrate pass for a single unrelated ckernel.cpp call site,
    // forms/fSetup.h:17-25); none of these three methods exist on it (grep -n
    // "ScrollBar1Change\|DoIniDataToForm\|sbUpdateClick" forms/fSetup.h -- 0
    // hits, 20260818). `fMain->ShowTestHeadComp(true)` right after IS a real
    // TfMain method (forms/fMain.h:156, empty body from an earlier wave) and
    // stays ACTIVE un-gated.
#if 0
    fSetup->ScrollBar1Change(this);
    fSetup->DoIniDataToForm();
    fSetup->sbUpdateClick(this);
#endif
    fMain->ShowTestHeadComp(true);

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
           bTestSiteUse[0][i][j]=bTempDutOnOff[i][j];
           LastSet.bUseTestSocket[0][i][j]=bTempDutOnOff[i][j];
           bTestSiteUse[1][i][j]=bTempDutOnOff[i][j];
           LastSet.bUseTestSocket[1][i][j]=bTempDutOnOff[i][j];
        }
    }

    fMain->ShowTestHeadComp(false);
    DoStructUnitConvert();
    // GATE(FW3-WC) golden :8993 `fTestCategory->AdjFormData();` -- fTestCategory
    // (golden TfTestCategory*, cTestCategory.h) has no port-wide facade;
    // Automation/auto9045.cpp:18 lists it by name among the forms this tree
    // explicitly documents as "untranslated" and builds its own TU-local
    // `W5FA_TfTestCategoryExt W5FA_FTestCategory` stand-in (auto9045.cpp:358-362)
    // used only inside that one file -- not exported as `fTestCategory` here
    // (grep -rn "TfTestCategory \*fTestCategory\|extern.*fTestCategory"
    // --include=*.h . -- 0 hits, 20260818). `fContactCT->ShowFormComp()`
    // immediately after IS a real, ACTIVE translated method (forms/fContactCT.h:43,
    // ":293 void ShowFormComp();") and stays un-gated.
#if 0
    fTestCategory->AdjFormData();
#endif
    fContactCT->ShowFormComp();
    SetWorkParameter();
    return 0;   //Success
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9000-9213 ---- */
// AI(W906-FW3-WC) 20260818: substitution (S2) applied throughout this
// function's body -- see FW3-WC GROUP banner above for the full citation.
// Every `asSiteTemp[i][j]` argument to a bare `sprintf(cSiteInfo[k], "...%s...", ...)`
// call below is spelled `asSiteTemp[i][j].c_str()` (golden has no `.c_str()`
// there because BCB6's own AnsiString could pass through C varargs directly;
// vclcompat's cannot). Also see GOLDEN BUG (B3) (10-byte `cSiteInfo` rows vs.
// 11-byte formatted output) and (B4) (only `cSiteInfo[0]` is ever returned)
// in that same banner -- both reproduced literally below.
int TfMain::GetSiteMappingByDLL(LPSTR cSiteMap)
{
    int i, j;
    char cSiteInfo[32][10];
    AnsiString asSiteTemp[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    for(i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF_File.iSiteMap[i][j]<=0)
            {
                asSiteTemp[i][j].sprintf("--");
            }
            else
            {
                asSiteTemp[i][j].sprintf("%02d", TestIF_File.iSiteMap[i][j]);
            }
        }
    }

    if(TestIF_File.iTestMode==SingleSite)       // Single Site 1x1
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)       // Dual Site 1x2
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)  // Qual Site 1x4
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[0][0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)  // Dual Site 2x1
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)  // Qual Site 2x2
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][0][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)      //ChungHung 20140115 add for 2x3_6
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][1][2]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][0][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)  //Wei 20231211 : 2X4NN Mode
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[6], "[07,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[7], "[08,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[0][0][3]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6], "[07,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7], "[08,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[0][1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        sprintf(cSiteInfo[0],  "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],  "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],  "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],  "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],  "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],  "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],  "[07,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],  "[08,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],  "[09,%s,%02d]", asSiteTemp[0][4].c_str(), bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],  "[10,%s,%02d]", asSiteTemp[1][4].c_str(), bTestSiteUse[0][1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        sprintf(cSiteInfo[0],  "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],  "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],  "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],  "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],  "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],  "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],  "[07,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],  "[08,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],  "[09,%s,%02d]", asSiteTemp[0][4].c_str(), bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],  "[10,%s,%02d]", asSiteTemp[1][4].c_str(), bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[10], "[11,%s,%02d]", asSiteTemp[0][5].c_str(), bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[11], "[12,%s,%02d]", asSiteTemp[1][5].c_str(), bTestSiteUse[0][1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)     //16Site 2x8
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][4].c_str(), bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][4].c_str(), bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[0][5].c_str(), bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[1][5].c_str(), bTestSiteUse[0][1][5]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][6].c_str(), bTestSiteUse[0][0][6]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][6].c_str(), bTestSiteUse[0][1][6]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[0][7].c_str(), bTestSiteUse[0][0][7]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[1][7].c_str(), bTestSiteUse[0][1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[1][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[2][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[3][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[1][1][1]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[2][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[3][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[1][1][2]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[2][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[3][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[1][1][3]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[2][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[3][3].c_str(), bTestSiteUse[0][1][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0].c_str(), bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0].c_str(), bTestSiteUse[1][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[2][0].c_str(), bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[3][0].c_str(), bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][1].c_str(), bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][1].c_str(), bTestSiteUse[1][1][1]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[2][1].c_str(), bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[3][1].c_str(), bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][2].c_str(), bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][2].c_str(), bTestSiteUse[1][1][2]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[2][2].c_str(), bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[3][2].c_str(), bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][3].c_str(), bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][3].c_str(), bTestSiteUse[1][1][3]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[2][3].c_str(), bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[3][3].c_str(), bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[16],  "[17,%s,%02d]", asSiteTemp[0][4].c_str(), bTestSiteUse[1][0][4]);
        sprintf(cSiteInfo[17],  "[18,%s,%02d]", asSiteTemp[1][4].c_str(), bTestSiteUse[1][1][4]);
        sprintf(cSiteInfo[18],  "[19,%s,%02d]", asSiteTemp[2][4].c_str(), bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[19],  "[20,%s,%02d]", asSiteTemp[3][4].c_str(), bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[20],  "[21,%s,%02d]", asSiteTemp[0][5].c_str(), bTestSiteUse[1][0][5]);
        sprintf(cSiteInfo[21],  "[22,%s,%02d]", asSiteTemp[1][5].c_str(), bTestSiteUse[1][1][5]);
        sprintf(cSiteInfo[22],  "[23,%s,%02d]", asSiteTemp[2][5].c_str(), bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[23],  "[24,%s,%02d]", asSiteTemp[3][5].c_str(), bTestSiteUse[0][1][5]);
        sprintf(cSiteInfo[24],  "[25,%s,%02d]", asSiteTemp[0][6].c_str(), bTestSiteUse[1][0][6]);
        sprintf(cSiteInfo[25],  "[26,%s,%02d]", asSiteTemp[1][6].c_str(), bTestSiteUse[1][1][6]);
        sprintf(cSiteInfo[26],  "[27,%s,%02d]", asSiteTemp[2][6].c_str(), bTestSiteUse[0][0][6]);
        sprintf(cSiteInfo[27],  "[28,%s,%02d]", asSiteTemp[3][6].c_str(), bTestSiteUse[0][1][6]);
        sprintf(cSiteInfo[28],  "[29,%s,%02d]", asSiteTemp[0][7].c_str(), bTestSiteUse[1][0][7]);
        sprintf(cSiteInfo[29],  "[30,%s,%02d]", asSiteTemp[1][7].c_str(), bTestSiteUse[1][1][7]);
        sprintf(cSiteInfo[30],  "[31,%s,%02d]", asSiteTemp[2][7].c_str(), bTestSiteUse[0][0][7]);
        sprintf(cSiteInfo[31],  "[32,%s,%02d]", asSiteTemp[3][7].c_str(), bTestSiteUse[0][1][7]);
    }

    strcpy(cSiteMap, cSiteInfo[0]);
    return iSiteCnt;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9215-9236 ---- */
void TfMain::GetSiteMappingForSIGURD(LPSTR cSiteMap)                            //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
{
    if(InitialOK==false)
    {
         return;
    }
    bool bSites[MAX_SOCKET_ROW*MAX_SOCKET_COL];
    ZeroMemory(bSites , sizeof(bSites));
    unsigned long ulvalue=0;
    int iTolCh=0;

    iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
    for(int i=0;i<iTolCh;i++)
    {
        if(GetSiteOnOffByChannel((i+1),TestSocket.iShtRow,TestSocket.iShtCol))
        {
            ulvalue|=(1<<i);
        }
    }
    sprintf(cSiteMap, "%08X", ulvalue);
}
//---------------------------------------------------------------------------
bool TfMain::GetSiteOnOffByChannel(int iCh,int iTolRow,int iTolCol)             //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
{
    // AI(W906-FW3-WC) 20260818: substitution (S5) -- see FW3-WC GROUP banner
    // above for the full citation. `GetShtModeFlag()` is not a declared
    // TfMain member in this port; replaced with MainCalcCore.h's own
    // documented portable replacement, ComputeShtModeFlag(TestIF.iShuttleMode,
    // TestIF.iShuttle_Sel).
    int iFlag=ComputeShtModeFlag(TestIF.iShuttleMode, TestIF.iShuttle_Sel);
    for(int icol=0;icol<iTolCol;icol++)
    {
        for(int irow=0;irow<iTolRow;irow++)
        {
            if(TestIF_File.iSiteMap[irow][icol]==iCh)
            {
                if(iFlag==2)
                {
                    return bTestSiteUse[0][irow][icol];
                }
                else
                 return bTestSiteUse[iFlag][irow][icol];
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfMain::SetSiteOnOffByChannel(int iCh, bool bSwitch)
{
    // AI(W906-FW3-WC) 20260818: substitution (S5) -- see GetSiteOnOffByChannel
    // above / FW3-WC GROUP banner for the full citation.
    int iFlag=ComputeShtModeFlag(TestIF.iShuttleMode, TestIF.iShuttle_Sel);
    for(int icol=0;icol<TestSocket.iShtCol;icol++)
    {
        for(int irow=0;irow<TestSocket.iShtRow;irow++)
        {
            if(TestIF_File.iSiteMap[irow][icol]==iCh)
            {
                if(iFlag==2)
                {
                    bLowYieldCloseSite[0][irow][icol]=bSwitch;
                    bLowYieldCloseSite[1][irow][icol]=bSwitch;
                    return true;
                }
                else
                {
                    bLowYieldCloseSite[iFlag][irow][icol]=bSwitch;
                    return true;
                }
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TfMain::SetSiteOnOff(AnsiString hexStr)
{
    if(InitialOK==false)
    {
         return;
    }
    bool bChSwitch[MAX_SOCKET_ROW*MAX_SOCKET_COL];
    ZeroMemory(bChSwitch, sizeof(bChSwitch));
    if(ParseHexToBoolArray(hexStr, bChSwitch)==true)
    {
        int iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
        for(int i=0; i<iTolCh; i++)
        {
            SetSiteOnOffByChannel((i+1), bChSwitch[i]);
        }
        ShowTestHeadComp(false);
    }
    fYieldMonitoring->bGetGPIBAutoSiteOff=true;
}
//---------------------------------------------------------------------------
bool TfMain::ParseHexToBoolArray(AnsiString hexStr, bool* bArr)
{
    hexStr=hexStr.Trim();
    if(hexStr.Length()!=8)
    {
        return false;                                                           //字串長度必須為 8
    }
    int iTotalChNum=MAX_SOCKET_ROW*MAX_SOCKET_COL;
    // AI(W906-FW3-WC) 20260818: substitution (S3) -- see FW3-WC GROUP banner
    // above for the full citation.
    unsigned int hexValue = static_cast<unsigned int>(HexStrToInt(hexStr));
    for (int i=0; i<iTotalChNum; i++)
    {
        if ((hexValue & (1 << i)) != 0)
        {
            bArr[i] = true;
        }
        else
        {
            bArr[i] = false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void TfMain::HexCharToBits(char hexChar, bool* bArr, int startIndex)
{
    int value;
    if(hexChar>='0' && hexChar<='9')
        value=hexChar-'0';                                                      // 0-9
    else if(hexChar>='A' && hexChar<='F')
        value=hexChar-'A'+10;                                                   // A-F
    else if(hexChar>='a' && hexChar<='f')
        value=hexChar-'a'+10;                                                   // a-f
    else
        return;
    bArr[startIndex]  =(value&0x8)!=0;                                          // 最高位
    bArr[startIndex+1]=(value&0x4)!=0;
    bArr[startIndex+2]=(value&0x2)!=0;
    bArr[startIndex+3]=(value&0x1)!=0;                                          // 最低位
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9344-9425 ---- */
int TfMain::SetTempByDLL(int iTempModeEPSON, double dTempVal)
{
    int ret, iTempMode;

    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;                                                              //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
    {
        return -4;                                                              //Operation not Allowed
    }

    if(iTempModeEPSON>2 || iTempModeEPSON<1)
    {
        return -3;                                                              //Parameter Error
    }

    if(iTempModeEPSON==2 && (dTempVal<dTempMin || dTempVal>dTempMax))
    {
        return -3;                                                              //Parameter Error
    }

    if(iTempModeEPSON==2)                                                       //高溫iTempMode=2, EPSON定義的指令
    {
        iTempMode=Tempture_Hot;
        Temperature.iMachineTempMode=0;
        LastSet.iTemperature=Tempture_Hot;
    }
    else
    {
        iTempMode=Tempture_Ambient;
        Temperature.iMachineTempMode=1;
        LastSet.iTemperature=Tempture_Ambient;
        dTempVal=25;
    }

    edWorkTemperBase->Text=CheckRange(dTempVal, dTempMax, dTempMin);

    ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));

    if(ret==0)
    {
        if(iTempModeEPSON==2)                                                   //高溫iTempMode=2, EPSON定義的指令
        {
            iTempMode=Tempture_Hot;
            Temperature.iMachineTempMode=0;
        }
        else
        {
            iTempMode=Tempture_Ambient;
            Temperature.iMachineTempMode=1;
        }

        // GATE(FW3-WC) golden :9410 `ret=ChangeTempMode(iTempMode, false,
        // bRefreshFunction, true, true);` -- ChangeTempMode is golden
        // main.h:1323, a TfMain MEMBER function, NOT present in forms/fMain.h
        // (grep -rn "ChangeTempMode" forms/*.h -- 0 declarations, 20260818) --
        // same absence FW3-WA already established for WriteSetTempStatus's two
        // ChangeTempMode call sites (Command.cpp:1852-1867/:1883-1888).
        // Faithful offline default: `ret` stays whatever SetTemp() returned
        // above (0 on the offline-success path), driving the same "0 -> return 0"
        // path below exactly as if ChangeTempMode had itself succeeded.
#if 0
        ret=ChangeTempMode(iTempMode, false, bRefreshFunction, true, true);
#endif

        if(ret==0)
        {
            return 0;
        }
        else
        {
            return -1;                                                          //General Error
        }
    }
    else
    {
        return -1;
    }
}
//---------------------------------------------------------------------------
int TfMain::GetTempSettingByDLL()
{
    AnsiString asTemp;
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(LastSet.iTemperature==Tempture_Hot)                                      //配合EPSON的定義,  Temperature Mode (1 = Ambient; 2 = High Temp)
    {
        asTemp="2";
    }
    else
    {
        asTemp="1";
    }

    AnsiString asWorkTemperBase;
    asWorkTemperBase.sprintf("%.1f", Temperature.fWorkTemperBase);
    if(Temperature.fWorkTemperBase>=0)
    {
        asTemp=asTemp+",+"+asWorkTemperBase;
    }
    else
    {
        asTemp=asTemp+",-"+asWorkTemperBase;
    }
    strcpy(CmdData->cGetTempSettings_Cmd5, asTemp.c_str());
    return 0;
}
//----------------------------------------------------------------------------
int TfMain::FTPDownloadByDLL(LPSTR cRecipeName)
{
    AnsiString Msg;
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;                                                              //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine() || LastSet.iTester==OFF_LINE)
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    // GATE(FW3-WC) golden :9481-9491 `fFTPClient->...` (whole remaining body)
    // -- fFTPClient (golden TfFTPClient*, KYECFTP/FTPClient.h) has NO
    // port-wide facade anywhere in this tree; KYECFTP/FTPClient_EventHandlers.h:94
    // says so explicitly in its own words ("No `TfFTPClient` facade exists
    // anywhere yet"), and the only two things that DO exist under that name
    // (FTPClient_Transfer.h's 4 free "session" functions and
    // FTPClient_EventHandlers.h's `bListOk`/`bError` demoted-to-extern globals)
    // are the golden CLASS's members demoted to file-scope, not a `fFTPClient`
    // object with `bControlBySECSGEM`/`aSetUpNameBySECSGEM`/`ShowFTPModal`/
    // `iErrorBySECSGEM` (grep -rn "fFTPClient" --include=*.h . -- every hit is
    // a citation comment, 0 real declarations, 20260818). `fLotInfo->SetLotStart`
    // and `GetCriticalParaAuth()` inside golden's success `else` are BOTH real,
    // translated, ACTIVE symbols (forms/fLotInfo.h:48; cAuthority.h:84) but are
    // unreachable without a real `fFTPClient->iErrorBySECSGEM` to branch on, so
    // they are quoted here inside the same gate rather than split out.
    // ACTIVE DEFAULT: -1 (General Error) -- an FTP download genuinely cannot
    // happen offline with no FTP-client form; this is a translator decision
    // (not a golden-observed value) documented here per pt-wave policy, not a
    // silent invention.
#if 0
    fFTPClient->bControlBySECSGEM=true;
    fFTPClient->aSetUpNameBySECSGEM=cRecipeName;
    fFTPClient->ShowFTPModal(0);
    if(fFTPClient->iErrorBySECSGEM!=0)
    {
        fFTPClient->iErrorBySECSGEM=-1;
    }
    else                                                                        //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        fLotInfo->SetLotStart(__FUNC__, false);
        GetCriticalParaAuth();
    }
    return fFTPClient->iErrorBySECSGEM;
#else
    return -1;
#endif
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9495-9525 ---- */
int TfMain::GetBinCountByDLL(int iCategNum)
{
    int ret=-1;

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(iCategNum>=iTestBinCount || iCategNum<0)
    {
        return -3;                                                              //Parameter Error
    }

    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量

    if(iCategNum>0)
    {
        if(Prod.iT6CatData[iCategNum]<0)                                        //QQQ
        {
            ret=-1;
        }
        else
        {
            ret=TastCategory.iTotalCategory[iCategNum];
        }
    }

    return ret;
}
//---------------------------------------------------------------------------
int TfMain::ClearBinCountByDLL()
{
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    fMain->Clarn_Data(1, "ClearBinCountByDLL");

    return 0;
}
//---------------------------------------------------------------------------
int TfMain::GetSortCountByDLL(int nTrayNum)
{
    int ret=0;
    if(nTrayNum<0 || nTrayNum>6)
    {
        return -1;                                                              //the tray is not defined
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(nTrayNum==0)                                                             //0: total count for all trays
    {
        ret=RunInfo.iUnloadCount;
    }
    else
    {
        ret=LastSet.BinCT[0][iTo3Unload[nTrayNum-1]];
    }

    return ret;
}
//---------------------------------------------------------------------------
int TfMain::ClearSortCountByDLL()
{
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    // GATE(FW3-WC) golden :9578 `fCounterClear->ClearCount(ctTraySortCount);` --
    // `fCounterClear` is NOT a real symbol in Command.cpp's translation unit.
    // csystem.cpp:4852-4854 defines a TU-LOCAL `#define fCounterClear
    // W7C1_fCounterClear` (a `struct W7C1_TfCounterClearSeam { void
    // LowYieldSpecialInitail(){} };` stand-in with no `ClearCount` member at
    // all) inside csystem.cpp itself -- a #define in a .cpp file, not a
    // header, so it is invisible to every other translation unit including
    // this one (grep -rn "fCounterClear" --include=*.h . -- 0 hits, 20260818;
    // "class TfCounterClear" -- 0 hits tree-wide). `ctTraySortCount` itself
    // IS a real enum value (cmydef.h) but has no real ClearCount() to receive
    // it here. ACTIVE DEFAULT: return 0 unconditionally, exactly as golden's
    // own body does immediately after the (gated) clear call -- there is no
    // separate success/failure branch to preserve.
#if 0
    fCounterClear->ClearCount(ctTraySortCount);
#endif

    return 0;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9583-9636 ---- */
int TfMain::GetHandlerStatusByDll()
{
    int ret=6;
//INIT=0        Breaker is ON but Power is OFF
//IDLE=1        Power is ON but handler display status is HALT
//RUNNING=2     Operator or remote control has started the handler
//PAUSE=3       Operator or remote control has paused the handler
//SYSERROR=4    Temporarily paused by an ALARM
//MANUAL=5      Operator has opened a setting window
//UNDEFINED=6  (Undefined)

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(SystemStart)
    {
        ret=2;
    }
    else
    {
        // GATE(FW3-WC) golden :9611 `MyMessageBox->fShow==true` -- MyMessageBox
        // (golden TMyMessageBox*, mymessbox.h) IS a real translated facade
        // (TMyMessageBoxShim, acatchtray_shims.h:319-327, real `bool fShow;`
        // field) -- but that header's own `NewRecordProcess(AnsiString,
        // AnsiString, AnsiString="")` declaration conflicts with the DIFFERENT
        // default argument (`Debug=" "`) canary_support.h already puts on the
        // same function's 3rd parameter, already visible in this TU (Command.cpp:264)
        // -- "default argument given for parameter 3" is a hard C++ error the
        // moment both headers are included in one TU (confirmed by actually
        // trying it, 20260818: `g++ -std=c++17 -fsyntax-only` on this exact
        // file). This is a PRE-EXISTING latent conflict between two already-committed
        // headers, not something this wave introduces or is in scope to fix
        // (acatchtray_shims.h is not one of this wave's two writable files).
        // Gating the term instead of the #include: `iUnLoaderCount==0` is also
        // ANDed in, so the whole first disjunct only ever contributes when a
        // buffer-place pre-alarm dialog AND zero-in-unloader coincide -- same
        // "no such subsystem is live offline" posture as every other GATE in
        // this wave. `fNote->fShow` (the second disjunct) is real and ACTIVE.
#if 0
        if(((MyMessageBox->fShow==true && iUnLoaderCount==0) || fNote->fShow==true) && bAlarmReset==false)
#else
        if((fNote->fShow==true) && bAlarmReset==false)
#endif
        {
            ret=4;
        }
        //----------判斷是否在設定參數
        else if(SettingsIsWindowOpened()==true)
        {
            ret=5;
        }
        else if(Sen[SnMotorPower].IsOff())
        {
            ret=0;
        }
        else
        {
            // AI(W906-FW3-WC) 20260818: substitution (S4) -- see FW3-WC GROUP
            // banner above for the full citation. `fMain->CheckCanChangeRealDummy()`
            // is not a declared TfMain member in this port; replaced with
            // MainCalcCore.h's own documented portable replacement,
            // ComputeCanChangeRealDummy(), fed the same six HasIC() reads
            // golden's own body would have resolved internally.
            if(ComputeCanChangeRealDummy(MOT[MMPlate1].HasIC(), MOT[MMPlate2].HasIC(),
                                          ShuttleHasIC(), IndexHasIC(),
                                          InArmSuck.HasIC(), OutArmSuck.HasIC())==false)
            {
                ret=3;
            }
            else if(fMain->palMainStatus->Caption=="HALT")
            {
                ret=1;
            }
            else
            {
                ret=6;
            }
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
double TfMain::GetAlarmStatusByDll()
{
    //JAMCode Format:
    //xx.yyy
    //xx = Unit Code
    //yyy = Alarm Code
    AnsiString asJamCode, asAlarmCode,asUnit, asResult;
    asJamCode=fNote->edErrorCode->Text;
    double ret=00.000;

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(fNote->fShow==true)
    {
        if(asJamCode.Pos("JAM")>0)
        {
            asAlarmCode=asJamCode.SubString(4, 2);
            asUnit=asJamCode.SubString(6, 3);
            asResult.sprintf("%s.%03d", asAlarmCode, asUnit.ToInt());
            ret=atof(asResult.c_str());
        }
        else
        {
            ret=00.000;
        }
    }
    else
    {
        ret=00.000;
    }
    return ret;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9718-9777 ---- */
int TfMain::GetBinCountPerSiteByDLL(int iCategNum, int iSiteNum)
{
    int iSocketCT=0;
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;
    int iRowCnt, iColCnt;
    int ret=-1;

//        a  b  c  d  e  f  g  h     **        a  b  c  d
//     A  1  3  5  7  9 11 13 15     **     A  1  2  3  4
//     B  2  4  6  8 10 12 14 16     **

    if(iCategNum>15 || iCategNum<0)
    {
        ret=-3;
        return ret;
    }

    if(iSiteNum>iSiteCnt || iSiteNum<0)
    {
        ret=-3;
        return ret;
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量

    if(TestSocket.iShtRow==1)
    {
        iRowCnt=0;
        iColCnt=iSiteNum-1;
    }
    else if(TestSocket.iShtRow==2)
    {
        if(iSiteNum%2==0)     //Row B
        {
            iRowCnt=1;
            iColCnt=(iSiteNum/2)-1;
            if(iColCnt<0)
                iColCnt=0;
        }
        else                  //Row A
        {
            iRowCnt=0;
            iColCnt=iSiteNum/2;
        }
    }
    else
    {
        ret=-3;
        return ret;
    }

    iSocketCT=TastCategory.iCountCategory[0][iRowCnt][iColCnt][iCategNum]+TastCategory.iCountCategory[1][iRowCnt][iColCnt][iCategNum];
    return iSocketCT;
}
//---------------------------------------------------------------------------
int TfMain::GetTempActualByDLL()
{
    AnsiString t;
    AnsiString asSite="NULL";

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    RefreshTempData();

    if(TestIF.iTestMode==SingleSite)
    {
            t.sprintf("1,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0);
    }
    else if(TestIF.iTestMode==DualSite || TestIF.iTestMode==DualSite2x1)       // Dual Site 1x2
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0);
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0);
    }
    else if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==QualSite2X2) // Qual Site 1x4
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0);
    }
    //QQ 20230214 : 2x2N
    else if(TestIF.iTestMode==_6Site2X3)
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR" && asTempArmOrder[0][4]!="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR" && asTempArmOrder[0][5]!="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR" && asTempArmOrder[1][4]!="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR" && asTempArmOrder[1][5]!="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0);
    }
    //QQ 20230214 : 2x3N
    else if(TestIF.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR" && asTempArmOrder[0][4]!="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR" && asTempArmOrder[0][5]!="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR" && asTempArmOrder[0][6]!="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR" && asTempArmOrder[0][7]!="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR" && asTempArmOrder[1][4]!="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR" && asTempArmOrder[1][5]!="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR" && asTempArmOrder[1][6]!="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR" && asTempArmOrder[1][7]!="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0);
    }
    //QQ 20230214 : 2x5
    else if(TestIF.iTestMode==_12Site2X6)   //12 Site                       //wei 20150702
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f,17,%+03.1f,18,%+03.1f,19,%+03.1f,20,%+03.1f,21,%+03.1f,22,%+03.1f,23,%+03.1f,24,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR"  && asTempArmOrder[0][0] !="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR"  && asTempArmOrder[0][1] !="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR"  && asTempArmOrder[0][2] !="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR"  && asTempArmOrder[0][3] !="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR"  && asTempArmOrder[0][4] !="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR"  && asTempArmOrder[0][5] !="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR"  && asTempArmOrder[0][6] !="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR"  && asTempArmOrder[0][7] !="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[0][8]!="ERR"  && asTempArmOrder[0][8] !="NULL")?atof(asTempArmOrder[0][8].c_str()):0.0,
                         (asTempArmOrder[0][9]!="ERR"  && asTempArmOrder[0][9] !="NULL")?atof(asTempArmOrder[0][9].c_str()):0.0,
                         (asTempArmOrder[0][10]!="ERR" && asTempArmOrder[0][10]!="NULL")?atof(asTempArmOrder[0][10].c_str()):0.0,
                         (asTempArmOrder[0][11]!="ERR" && asTempArmOrder[0][11]!="NULL")?atof(asTempArmOrder[0][11].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR"  && asTempArmOrder[1][0] !="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR"  && asTempArmOrder[1][1] !="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR"  && asTempArmOrder[1][2] !="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR"  && asTempArmOrder[1][3] !="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR"  && asTempArmOrder[1][4] !="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR"  && asTempArmOrder[1][5] !="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR"  && asTempArmOrder[1][6] !="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR"  && asTempArmOrder[1][7] !="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0,
                         (asTempArmOrder[1][8]!="ERR"  && asTempArmOrder[1][8] !="NULL")?atof(asTempArmOrder[1][8].c_str()):0.0,
                         (asTempArmOrder[1][9]!="ERR"  && asTempArmOrder[1][9] !="NULL")?atof(asTempArmOrder[1][9].c_str()):0.0,
                         (asTempArmOrder[1][10]!="ERR" && asTempArmOrder[1][10]!="NULL")?atof(asTempArmOrder[1][10].c_str()):0.0,
                         (asTempArmOrder[1][11]!="ERR" && asTempArmOrder[1][11]!="NULL")?atof(asTempArmOrder[1][11].c_str()):0.0);
    }
    else if(TestIF.iTestMode==_16Site2X8)   //16 Site
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f,17,%+03.1f,18,%+03.1f,19,%+03.1f,20,%+03.1f,21,%+03.1f,22,%+03.1f,23,%+03.1f,24,%+03.1f,25,%+03.1f,26,%+03.1f,27,%+03.1f,28,%+03.1f,29,%+03.1f,30,%+03.1f,31,%+03.1f,32,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR"  && asTempArmOrder[0][0] !="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR"  && asTempArmOrder[0][1] !="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR"  && asTempArmOrder[0][2] !="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR"  && asTempArmOrder[0][3] !="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR"  && asTempArmOrder[0][4] !="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR"  && asTempArmOrder[0][5] !="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR"  && asTempArmOrder[0][6] !="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR"  && asTempArmOrder[0][7] !="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[0][8]!="ERR"  && asTempArmOrder[0][8] !="NULL")?atof(asTempArmOrder[0][8].c_str()):0.0,
                         (asTempArmOrder[0][9]!="ERR"  && asTempArmOrder[0][9] !="NULL")?atof(asTempArmOrder[0][9].c_str()):0.0,
                         (asTempArmOrder[0][10]!="ERR" && asTempArmOrder[0][10]!="NULL")?atof(asTempArmOrder[0][10].c_str()):0.0,
                         (asTempArmOrder[0][11]!="ERR" && asTempArmOrder[0][11]!="NULL")?atof(asTempArmOrder[0][11].c_str()):0.0,
                         (asTempArmOrder[0][12]!="ERR" && asTempArmOrder[0][12]!="NULL")?atof(asTempArmOrder[0][12].c_str()):0.0,
                         (asTempArmOrder[0][13]!="ERR" && asTempArmOrder[0][13]!="NULL")?atof(asTempArmOrder[0][13].c_str()):0.0,
                         (asTempArmOrder[0][14]!="ERR" && asTempArmOrder[0][14]!="NULL")?atof(asTempArmOrder[0][14].c_str()):0.0,
                         (asTempArmOrder[0][15]!="ERR" && asTempArmOrder[0][15]!="NULL")?atof(asTempArmOrder[0][15].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR"  && asTempArmOrder[1][0] !="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR"  && asTempArmOrder[1][1] !="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR"  && asTempArmOrder[1][2] !="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR"  && asTempArmOrder[1][3] !="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR"  && asTempArmOrder[1][4] !="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR"  && asTempArmOrder[1][5] !="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR"  && asTempArmOrder[1][6] !="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR"  && asTempArmOrder[1][7] !="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0,
                         (asTempArmOrder[1][8]!="ERR"  && asTempArmOrder[1][8] !="NULL")?atof(asTempArmOrder[1][8].c_str()):0.0,
                         (asTempArmOrder[1][9]!="ERR"  && asTempArmOrder[1][9] !="NULL")?atof(asTempArmOrder[1][9].c_str()):0.0,
                         (asTempArmOrder[1][10]!="ERR" && asTempArmOrder[1][10]!="NULL")?atof(asTempArmOrder[1][10].c_str()):0.0,
                         (asTempArmOrder[1][11]!="ERR" && asTempArmOrder[1][11]!="NULL")?atof(asTempArmOrder[1][11].c_str()):0.0,
                         (asTempArmOrder[1][12]!="ERR" && asTempArmOrder[1][12]!="NULL")?atof(asTempArmOrder[1][12].c_str()):0.0,
                         (asTempArmOrder[1][13]!="ERR" && asTempArmOrder[1][13]!="NULL")?atof(asTempArmOrder[1][13].c_str()):0.0,
                         (asTempArmOrder[1][14]!="ERR" && asTempArmOrder[1][14]!="NULL")?atof(asTempArmOrder[1][14].c_str()):0.0,
                         (asTempArmOrder[1][15]!="ERR" && asTempArmOrder[1][15]!="NULL")?atof(asTempArmOrder[1][15].c_str()):0.0);
    }
    //QQ 20230214 : 4x4
    //QQ 20230214 : 4x8
    else
    {
        return -3;
    }
    strncpy(CmdData->cGetTempActual_Cmd6, t.c_str(), sizeof(CmdData->cGetTempActual_Cmd6));
    return 0;
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9939-9964 ---- */
// GATE(FW3-WC) golden :9939-9964, the whole 16-term OR-chain (33 dereferences
// across 32 distinct golden form pointers) -- grep-verified 20260818, command
// by command:
//   `grep -rn "class Tf(Teach|MotorTest|HotPlate|TrayAssignment|Speed|BinSel|
//    Security|CCLink|Temp_Set|CounterClear|TowerLight|QAMode|CounterSel|
//    Builder|StartCondition|FTPClient)\b" --include=*.h .` -- 0 hits for every
//   one of: fTeach, fMotorTest, fHotPlate, fTrayAssignment, fSpeed, fBinSel,
//   fSecurity, fCCLink, fTemp_Set, fCounterClear, fTowerLight, fQAMode,
//   fCounterSel, fBuilder, fStartCondition, fFTPClient (13 of the 16 terms
//   golden ORs) -- confirmed absent, cross-referenced against
//   docs/RECON_GateA_FormRegistry.md's own "ABSENT" column for the same
//   names, and against per-file absence banners already in this tree
//   (cinitial.cpp:15865-15872 for fTeach; bthermo.cpp:3913-3920 -- GATE
//   W7-UI-G26a -- for fOmron; csystem.cpp:28434-28437 for
//   fTrayAssignment/FTestIF/fLd_ULd/fHotPlate/fSpeed; MyEtherCAT.cpp:37 for
//   fCCLink; ProductionInfo/uPAT_Function.cpp:292-294 for fHotPlate/fTemp_Set;
//   KYECFTP/FTPClient_EventHandlers.h:94 for fFTPClient).
//   The remaining 3 objects DO exist as real facades, but the exact `fShow`/
//   `bShow` field golden reads on each does NOT (read each header's full
//   class body this pass, 20260818): fShuttleMove (forms/fShuttleMove.h:35
//   explicitly documents `bool fShow` as "Notably NOT landed"), fOffSet
//   (forms/fOffSet.h -- 2 methods only, no data members at all), fiosetview
//   (atester_shims.h:348-353 `class TfiosetviewShim` -- only
//   `bIndexSuck[2][4][8]`), fCleaning (forms/fCleaning.h -- 4 members, none
//   named fShow), fYieldMonitoring (forms/fYieldMonitoring.h:205-291 -- ~30
//   members read in full, none named fShow; `bShowSiteYield[32]` is a
//   different, unrelated array), fTrayForm (forms/fTrayForm.h:30-47 -- only
//   `asErrorMsg`/`IsEnableColorSensor()`), FrmRotate (forms/fRotate.h:47-XX --
//   only `bRotateInHome`/`bRotateOutHome` + 2 methods), fBarCode
//   (aHotPlateSubstrate.h:984-1026 `class TfBarCode_Shim` -- no `bShow`
//   member), fLtcSensor (acarry_shims.h:76-90 -- 10 LatchDataCnt* ints + 10
//   LatchDataTable* arrays + 3 methods, no `bShow`). fConfiguration DOES
//   exist under that exact global name but as `W5SckArtRem_ConfigStub`
//   (Automation/SCK_ART_Remainder.h:594-599), a narrow single-purpose stand-in
//   with exactly one member (`mmoN04_IP`) for an unrelated SCK-ART feature --
//   not golden's TfConfiguration settings dialog, so it carries no `fShow`
//   either.
// Of the 33 OR-terms, only 3 read a real, populated field: fHome->fShow
// (forms/fHome.h:101, real `bool fShow;`), fSetup->fShow (forms/fSetup.h:69,
// real `bool fShow;`), and fContact->fShow (atester_shims.h:157, real
// `bool fShow;`). ACTIVE DEFAULT below is the OR of exactly those three --
// every absent term is equivalent to "that subsystem is not live offline"
// (false), the same posture already established throughout this tree for
// every other lone `->fShow` GATE (e.g. PERSITETemperatureStrings' fContact
// gate, Command.cpp:1162-1196).
bool TfMain::SettingsIsWindowOpened()
{
#if 0
    if(fTeach->fShow     || fMotorTest->fShow    || fShuttleMove->fShow      ||
      fHome->fShow       || fLtcSensor->bShow    || fOmron->bShow            ||
      fContact->fShow    || fiosetview->fShow    || fContact->fShow          ||
      fSetup->fShow      || fOffSet->fShow       || fConfiguration->fShow    ||
      fSpeed->fShow      || fDIOFrom->fShow      || fYieldMonitoring->fShow  ||
      fTrayForm->fShow   || fHotPlate->fShow     || fTrayAssignment->fShow   ||
      fTemp_Set->fShow   || FTestIF->fShow       || fCounterClear->fShow     ||
      fLd_ULd->fShow     || fCCLink->bShow       || fTowerLight->fShow       ||
      fCleaning->fShow   || fQAMode->fShow       || fCounterSel->fShow       ||
      FrmRotate->fShow   || fBuilder->fShow      || fStartCondition->fShow   ||
      fBarCode->bShow    || fSecurity->fShow     || fBinSel->bShow           ||
      fFTPClient->bShow)
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    if(fHome->fShow || fSetup->fShow || fContact->fShow)
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
}
//---------------------------------------------------------------------------
//<==
//JerryYang 20181126 (Steven) : support Epson DLL function
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9965-9978 ---- */
void TfMain::WriteSiteOnOff()                                                   //JerryYang 20190627 回傳開關site狀態
{
    char cSiteOnOff[256];
    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
    {
        GetSiteMappingForSIGURD(cSiteOnOff);
    }
    else
    {
        GetSiteMappingByDLL(cSiteOnOff);
    }
    SendMSG_CMD(MSG_CMD_GetSiteOnOff, cSiteOnOff);
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9979-9985 ---- */
void TfMain::AutoSiteOnOff(AnsiString buffer)                                   //JimmyChiu 20250715 : Auto site on/off by GPIB
{
    SetSiteOnOff(buffer);
    AnsiString sRet="OK";
    SendMSG_CMD(MSG_CMD_GetSiteOnOff, sRet);
}
//---------------------------------------------------------------------------

/* ---- golden Command.cpp:9986-9994 ---- */
void TfMain::WriteNumOfSites()                                                  //JerryYang 20190627 回傳site count
{
    AnsiString sNumOfSite="";
    int iSiteCnt=0;
    iSiteCnt=GetSiteCount();
    sNumOfSite.sprintf("%d", iSiteCnt);
    SendMSG_CMD(MSG_CMD_GetNumOfSites, sNumOfSite);
}

// -- FW3-WC APPEND -- end (ByDLL family, golden Command.cpp :8311-9994; RemoteControl :9673-9717 excluded per never-wave) --

// AI(W906-FW3-WD) 20260818: additional #includes for symbols the FW3-WD GROUP
// below needs that no earlier Command.cpp #include (see top of file, or the
// FW3-WB/FW3-WC blocks) pulls in. Placed here rather than sorted into the top
// block because this file's contract is append-only past "-- FW3-WC APPEND --
// end" -- existing lines (including the existing #include blocks) are not to
// be touched. Header-guarded, so safe regardless of what else already
// (transitively) included them.
//   NOTE: bthermo.h (bGetHeaterUsed), aHotPlateSubstrate.h (TestSocket/cSBin),
//   csystem.h (HasICUnderMachine) and Config.h (IniConfig, via cprod.h) are
//   ALREADY visible from the top-of-file / FW3-WB include blocks -- verified
//   this pass (`grep -n "^#include" Command.cpp`, 20260818) -- so none of them
//   is repeated below.

// =============================================================================
//  FW3-WD GROUP -- the SIGURD/GPIB status-string + bin/soak/site-map command
//                  family
//
//  Translation wave: FW-3 Wave D
//  Translator: AI(W906-FW3-WD) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Command.cpp (15,273 lines, cp950)
//
//  ROLE
//  ----
//  29 golden TfMain:: methods, golden Command.cpp :9995-12061, declared in
//  forms/fMain.h's new "FW3-WD ADD" block. These are the SIGURD-customer GPIB
//  status/temperature/site-map/bin-configuration entry points layered on top
//  of the FW3-WA/WB/WC families: Samsung-format temp/map/soak strings, TTL
//  RS232 state, SIGURD SETTEMP_/SETSOAK_/SETSITEMAP_/CHKSTATUS?/SGSETUP_/
//  SetStartMode_/CHECKLIST?/BINPOS_/SGFTP_*/NONDOUBLEBIN_/BINCOUNT_/SGOSBIN_/
//  SGCONTFAIL_, plus the UTAC PPSELECT read-file pair and the Amlogic SBIN
//  setter. Per the task brief: bodies are translated, NO caller (TCP dispatch
//  / GPIB dispatch table) is wired up. golden Command.cpp :12063-12536 (the
//  next 34 methods, SetTesterID..GetSiteState) were ALREADY translated by
//  FW3-WA (confirmed present in this file, `grep -n "TfMain::SetTesterID\|
//  TfMain::GetSiteState" Command.cpp`, 20260818) -- not re-touched here. Golden
//  :12540 onward (the TCP-server family) is excluded per this wave's own
//  scope boundary, not per any never-wave list.
//
//  WAVE SCOPE (every golden method, golden line span, ACTIVE or GATED-partial)
//  ------------------------------------------------------------------------------
//    GetSamSungTmp             :9995-10135  ACTIVE
//    GetSamSungMap              :10137-10303 ACTIVE
//    GetSamSungSoakTime         :10305-10322 ACTIVE
//    GetTTLState                :10324-10500 GATED-partial (Handle/HVisionWnd, GATE REGISTER 1-2)
//    Send_Command_TTL           :10502-10511 ACTIVE (1 vclcompat-surface substitution, S8)
//    WriteSetTempStatus_SIGURD  :10513-10594 GATED-partial (fTemp_Set / edATCAmbientTemper / ChangeTempMode, GATE REGISTER 3-5)
//    WriteSetSoakTimeStatus_SIGURD :10596-10668 GATED-partial (ChangeTempMode / IniData write, GATE REGISTER 6-7)
//    SetSiteMapData_SIGURD      :10670-10728 GATED-partial (ChangeToSiteMap, GATE REGISTER 8) (1 vclcompat-surface substitution, S7)
//    GetTestIFSiteMap           :10730-10954 ACTIVE
//    ChkStatus                  :10956-11013 ACTIVE (1 GOLDEN ODDITY, B5)
//    GetBinCategory             :11015-11024 ACTIVE
//    GetSetUpFileName           :11026-11039 ACTIVE
//    GetHandlerID_Sigurd        :11041-11051 ACTIVE
//    SetSetupFileName           :11053-11102 GATED-partial (fFTPClient, GATE REGISTER 9)
//    ChangeSetupFileName        :11104-11135 GATED-partial (fFTPClient, GATE REGISTER 10)
//    PPSELECTAskFile            :11139-11144 ACTIVE
//    PPSELECTLoadFile           :11146-11191 GATED-partial (ShowMessage, GATE REGISTER 11)
//    SetStartMode               :11195-11271 ACTIVE (1 GOLDEN ODDITY, B5)
//    ChangeHandlerStartMode     :11274-11332 GATED-partial (cbRunStartMode/DoFTRTClick, GATE REGISTER 12) -- SAFETY-RELEVANT, see RISK note below
//    CheckList                  :11334-11357 GATED-partial (fLotInfo, GATE REGISTER 13)
//    SetBinPosChange            :11359-11398 ACTIVE
//    BinPosChange               :11400-11548 GATED-partial (fBinSel / fShowBinSelect, GATE REGISTER 14-15) -- SAFETY-RELEVANT, see RISK note below
//    GetSGFTPSTATUS             :11550-11558 ACTIVE
//    SetSGFTP                   :11560-11609 GATED-partial (fConfiguration / config\ write / fLotInfo, GATE REGISTER 16-18)
//    SetNONDOUBLEBIN            :11611-11696 ACTIVE (1 vclcompat-surface substitution, S6)
//    SetSBinData                :11698-11724 ACTIVE
//    SetBINCOUNT                :11726-11891 GATED-partial (IniData write / fBinSel, GATE REGISTER 19-20) (1 vclcompat-surface substitution, S6) -- SAFETY-RELEVANT, see RISK note below
//    SetSGOSBIN                 :11893-11947 GATED-partial (fLotInfo, GATE REGISTER 21)
//    SetSGCONTFAIL              :11949-12061 GATED-partial (IniData write / fYieldMonitoring / fLotInfo, GATE REGISTER 22-24) (1 GOLDEN BUG, B6)
//  TOTALS: 29 methods, 2,067 raw golden lines extracted; 12 ACTIVE, 17 GATED-partial.
//
//  RISK NOTE -- machine-mode-switching / persistence methods (per project rule:
//  describe risk before verification, not after)
//  ------------------------------------------------------------------------------
//  * ChangeHandlerStartMode: the body is translated FAITHFULLY (per this wave's
//    "本體照翻, 呼叫者不接線" instruction) including its unconditional
//    `return true;` tail. Because `cbRunStartMode`/`DoFTRTClick` are gated
//    (GATE REGISTER 12), the FT/RT arms currently do NOT actually flip
//    `LastSet.iRunStartMode` the way golden's `DoFTRTClick` would -- only the
//    QA arm (`SetRunStartMode(rsmQAMode)`, a real call) does. Since nothing
//    calls `ChangeHandlerStartMode`/`SetStartMode` yet (no caller wired this
//    wave), this is currently inert; it becomes live behavior the moment a
//    GPIB/TCP dispatcher starts invoking `SetStartMode()`.
//  * BinPosChange / SetBINCOUNT: both update the live in-memory `BinSelect[]`
//    array for real (ACTIVE), but the on-screen `fBinSel` mirror AND the
//    setup-file persistence (`fBinSel->spbSaveClick`, which golden's own
//    comments identify as "寫入 FT SetupFile") are gated (GATE REGISTER 14/20
//    -- fBinSel has no port anywhere in this tree). A BINPOS_/BINCOUNT_ command
//    would report OK and change bin routing for the CURRENT run, but the
//    change is lost on the next setup-file reload / restart. Same absence
//    FW3-WC already established for `WriteSetBinMap` (GATE #6).
//
//  GATE REGISTER  (24 #if 0 sites; each states WHY the gate is correct, not
//  just "not found" -- per this wave's own anti-absence-claim-rot instruction.
//  Every grep below was re-run 20260818, from this tree's root, excluding
//  ./tools/ and ./build*/.)
//  ------------------------------------------------------------------------------
//   1. GetTTLState, golden :10334-10335 `HHandler2Gpib.HandlerHwnd=this->Handle;`
//      / `HHandler2Gpib.GpibHwnd=HVisionWnd;` -- neither `Handle` (TForm's own
//      window handle) nor `HVisionWnd` (golden main.h:1215 `HWND HVisionWnd;`,
//      a TfMain data member) is declared anywhere in forms/fMain.h
//      (`grep -n "HVisionWnd" forms/fMain.h` -- 0 hits; bare `Handle` likewise
//      0 hits). Both struct-field assignments are skipped; `HHandler2Gpib.
//      HandlerHwnd`/`.GpibHwnd` simply keep whatever value they last held
//      (VCL zero-inits `HWND` fields to 0, same as this port's own MV ctor).
//   2. GetTTLState, golden :10497 `SendMessage(fMain->HVisionWnd, WM_COPYDATA,
//      (WPARAM) NULL, (LPARAM)pcp);` -- same HVisionWnd absence as item 1; no
//      real Win32 window subsystem exists offline to address, matching this
//      tree's established "no hardware/UI to fail" posture (e.g.
//      WriteHandlerTestArmEP GATE #7, FW3-WA banner). `pcp` is still
//      allocated, populated (`dwData`/`cbData`/`lpData`) and freed exactly as
//      golden does -- no leak -- only the message SEND itself is skipped.
//      Every other line in this function (the whole TTL_CARD_TYPE / DIOCfg
//      bit-length branch tree and the `asBuffer->Add(...)` message-content
//      build) is real computation over real globals and stays ACTIVE.
//   3. WriteSetTempStatus_SIGURD, golden :10530-10531 `dTempMax=fTemp_Set->
//      MaxTempSetting(); dTempMin=fTemp_Set->MinTempSetting();` -- `fTemp_Set`
//      (golden uTemp_Set.h TfTemp_Set) has NO port anywhere in this tree.
//      ESTABLISHED precedent, not a fresh finding: MyTempPanel.cpp's own GATE
//      (W8-3) already gates this EXACT golden idiom for the same reason.
//      `grep -rn --include=*.h --include=*.cpp -E
//       "fTemp_Set[[:space:]]*(;|=)|\*[[:space:]]*fTemp_Set|TfTemp_Set" .`
//      -- only Automation/auto9045.cpp's TU-local stand-in `W5FA_TfTemp_SetExt
//      W5FA_FTemp_Set` (carries `edSoakTime`/`edWorkTemp` only, no
//      MaxTempSetting/MinTempSetting), plus ./build/* generated layout tables.
//      `dTempMax`/`dTempMin` THEMSELVES are real (cmydef.h:3437-3438, default
//      135.0/20.0) -- the ACTIVE arm leaves both at whatever the rest of the
//      system last wrote, same as W8-3's own reasoning.
//   4. WriteSetTempStatus_SIGURD, golden :10559 `edATCAmbientTemper->Text=d;`
//      and :10580 `atof(edATCAmbientTemper->Text.c_str())` -- `edATCAmbientTemper`
//      is not a declared TfMain member anywhere (`grep -rn "edATCAmbientTemper"
//      .` -- 0 hits, any header or source, tree-wide). Since the widget's only
//      role here is to MIRROR `d` (set two lines above the read, in the exact
//      same `if(Temperature.bATCActiveCooling)` arm), the `SetTemp(...)` call
//      substitutes `d` directly in place of the missing mirror -- the
//      identical numeric value golden's own widget would have held, not a
//      fabricated one.
//   5. WriteSetTempStatus_SIGURD, golden :10585 `ret=ChangeTempMode(TempMode,
//      false, bRefreshFunction, true);` -- ChangeTempMode absence, ESTABLISHED
//      precedent (this file's own FW3-WA GATE #2, Command.cpp:1852-1867/
//      1883-1888: golden main.h:1323 TfMain member, not in forms/fMain.h, no
//      facade path). `ret` is left at whatever `SetTemp()` (a real,
//      already-GATED-leaf fMain member) returned above, same posture as GATE #2.
//   6. WriteSetSoakTimeStatus_SIGURD, golden :10651/:10657 `fMain->
//      ChangeTempMode(0, false, true);` / `fMain->ChangeTempMode(1, false,
//      true);` -- same ChangeTempMode absence as item 5.
//   7. WriteSetSoakTimeStatus_SIGURD, golden :10652/:10658 `WriteIniData(szDir,
//      "Mode", "Mode", 1/0);` where `szDir=DataPath+S+"\Temperature.Data"` --
//      SHARED MACHINE CONFIG WRITE (`DataPath` = `D:\HT9045\IniData\Data\`,
//      common.cpp:104). See "SHARED CONFIG WRITE GATES" list below for the
//      full policy citation; not re-derived per-site.
//   8. SetSiteMapData_SIGURD, golden :10715 `ret=ChangeToSiteMap(str);` --
//      `ChangeToSiteMap` (golden main.h:1432, body golden Command.cpp:7511) has
//      NO port anywhere in this tree (`grep -rn "ChangeToSiteMap" --include=*.h
//      --include=*.cpp .` -- 0 hits). It is a genuine golden dependency this
//      wave's 29-method list does NOT include (golden :7511 sits inside golden
//      Command.cpp's earlier :6542-8201 span, already claimed by FW3-WB's
//      GetCZAllMassTemp, and was not itself translated by that wave either --
//      re-verified this pass). `ret` keeps its OWN declared initial value
//      (`bool ret=true;`, golden :10672) rather than being reset to `false` --
//      per this wave's "不可順手修好" rule, golden's own default is preserved
//      even though it means a SETTINGOK reply is sent without the site map
//      having actually changed. The backup bookkeeping
//      (`iBackupDutOnOff`/`iBackupTestMode`) and the CRLF-strip/reply-building
//      logic around it are real and stay ACTIVE.
//   9. SetSetupFileName, golden :11086 `if(fFTPClient->
//      CheckSetupFileNameFromServer(asChangeSetupFileName)==true) ret=true;`
//      -- `fFTPClient` (golden TfFTPClient, KYECFTP's FTP-download dialog) has
//      no facade in this tree. `grep -rn -E "class TfFTPClient|extern.*
//      \bfFTPClient\b"  --include=*.h .` -- 0 hits; the two existing
//      `fFTPClient` name occurrences (AutoRetest.h:28, canary_support.h:163)
//      are prose NOTING its absence for an unrelated unit, not declarations.
//      `ret` stays its declared default `false` for every customer code other
//      than CC_SIGURD_PeiXing (whose own branch, immediately above, is real
//      and un-gated).
//  10. ChangeSetupFileName, golden :11120-11127, the whole `fFTPClient->
//      bControlByGPIB=true; ->iErrorByGPIB=0; ->asSetUpNameByGPIB=...;
//      ->ShowFTPModal(0); ... ->bControlByGPIB=false; if(fFTPClient->
//      iErrorByGPIB==0)` cluster -- same fFTPClient absence as item 9. Faithful
//      default: `return false;` (no download channel exists offline, so the
//      recipe change cannot have succeeded). `bSigurdDownload_Recipe=true;`/
//      `=false;` (cmydef.h:5184, a real global other code may observe) stay
//      ACTIVE either side of the gated cluster, and the two early-return
//      guards (`SystemStart==true`, name-equality check) stay ACTIVE.
//  11. PPSELECTLoadFile, golden :11163 `ShowMessage("Has IC Under Machine");`
//      -- golden's plain VCL Dialogs.hpp `void ShowMessage(const AnsiString&)`,
//      an ESTABLISHED tree-wide absence with a standard TU-local no-op
//      stand-in convention (5 other TUs each define their own: ATC/
//      ATCSystem.cpp:196 `W906_ATCSystem_Gated_ShowMessage`, ProductionInfo/
//      TfFTP.cpp:169 `Gated_ShowMessage`, SECSGEM/SecsSvEcRegistration.cpp:44,
//      PMAlarm/PMAlarmSystem.cpp:176, SECSGEM/uHGemEquipment.cpp:3271, plus
//      asendic_Loader.cpp:280/310's `#define ShowMessage W7L1L_ShowMessage`).
//      Same treatment applied here: a TU-local `Gated_ShowMessage` no-op is
//      defined once, immediately above PPSELECTLoadFile below, and substituted
//      for golden's call.
//  12. ChangeHandlerStartMode, golden :11302-11305 (the FT arm) and
//      :11313-11316 (the RT arm), all four `cbRunStartMode->Enabled=...;` /
//      `fMain->DoFTRTClick(...)` statements -- neither symbol is a declared
//      TfMain member. `grep -n "cbRunStartMode" forms/fMain.h` -- exactly 1
//      hit, a PROSE comment at line 541 ("the iSecsGemSwitchFTRT==0 &&
//      cbRunStartMode->Enabled==false guard"), no declaration anywhere.
//      `grep -n "DoFTRTClick" forms/fMain.h` -- 0 hits. Per this wave's brief
//      ("SetStartMode/ChangeHandlerStartMode/ChangeSetupFileName 這類改機台
//      模式的方法：本體照翻，呼叫者不接線"), the function's own `return true;`
//      tail is left ACTIVE and un-gated -- see the RISK NOTE above for the
//      behavioral consequence this creates.
//  13. CheckList, golden :11341 `fLotInfo->GenerateCheckList(false,
//      sCheckListName, true);`, :11343 `ret=fLotInfo->GenerateCheckList(false,
//      fMain->cbSetupFileName->Text, false);`, and :11354 `fLotInfo->
//      RefreshYieldMonitor();` -- neither `GenerateCheckList` nor
//      `RefreshYieldMonitor` is a declared forms/fLotInfo.h member.
//      `grep -n "GenerateCheckList\|RefreshYieldMonitor" --include=*.h .`
//      (tree-wide) -- 0 hits. `ret` keeps its declared default `AnsiString
//      ret="";` (golden :11336), so the SendMSG_CMD reply naturally resolves
//      to the `t.sprintf("SETTINGNG")` arm -- a faithful "the checklist action
//      did not happen" outcome, not a fabricated "OK".
//  14. BinPosChange, golden :11535-11541, the whole `for(i<TEST_MAX_BIN)
//      { if(i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count)
//      fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=...; }` loop, plus
//      :11542-11543 `fBinSel->ChangeActivePageIndex();`/`->spbSaveClick(this);`
//      -- `fBinSel` (golden TfBinSel) has no port anywhere in this tree.
//      ESTABLISHED precedent: this file's own FW3-WC GATE (Command.cpp:8483/
//      8498/8538/8546/8618) already cites the same absence for
//      `SetTrayBinByDLL`/`WriteSetBinMap`. The in-memory `BinSelect[iTestRunMode]
//      .iCatDataT3Pos[]` update immediately above (golden :11523-11533) stays
//      ACTIVE and un-gated -- see the RISK NOTE above for the persistence
//      consequence.
//  15. BinPosChange, golden :11544 `fShowBinSelect->ShowBinSel();` --
//      `ShowBinSel` (golden cShowBinSelect.cpp :388-757) is explicitly queued
//      for a LATER wave by forms/fShowBinSelect.h's own "WAVE B QUEUE" table
//      (that file's header banner, this pass re-read in full) -- it is not yet
//      a declared TfShowBinSelect member (`grep -n "ShowBinSel\b"
//      forms/fShowBinSelect.h` -- 1 hit, inside that WAVE-B-QUEUE comment
//      list, no declaration).
//  16. SetSGFTP, golden :11589/:11597 `fConfiguration->cbA32->Checked=
//      true/false;` -- the only `fConfiguration` global in this tree is
//      Automation/SCK_ART_Remainder.h's narrow `W5SckArtRem_ConfigStub`
//      (ESTABLISHED absence, this file's own FW3-WA GATE #8: "carries only
//      ->mmoN04_IP, not edN27_6" -- re-verified this pass that it carries no
//      `cbA32` either, `grep -n "cbA32" Automation/SCK_ART_Remainder.h` -- 0
//      hits).
//  17. SetSGFTP, golden :11588/:11595 `WriteIniData(sPath, "Function",
//      "bA32EnableFTPAutomation", 1/0);` where `sPath=AuthPath+"config.ini"`
//      (AuthPath=`D:\HT9045\config\`, common.cpp:102) -- SHARED MACHINE CONFIG
//      WRITE (config\), explicit case named in this wave's brief. See "SHARED
//      CONFIG WRITE GATES" list below.
//  18. SetSGFTP, golden :11605 `fLotInfo->RefreshYieldMonitor();` -- same
//      absence as item 13.
//  19. SetBINCOUNT, golden :11800/:11802/:11807/:11809, the four
//      `WriteIniData(sLastFilePath, ...)` calls (`sLastFilePath=DataPath+
//      asLastFileName+"\Tester.Data"`) -- SHARED MACHINE CONFIG WRITE
//      (IniData\). See "SHARED CONFIG WRITE GATES" list below.
//  20. SetBINCOUNT, golden :11875/:11876 `fBinSel->ChangeActivePageIndex();`/
//      `->spbSaveClick(this);` -- same fBinSel absence as item 14. The
//      in-memory `BinSelect[iTestRunMode].iPersentIgnore[]`/`Prod.
//      iPersentIgnore[]` updates immediately around it stay ACTIVE -- see the
//      RISK NOTE above.
//  21. SetSGOSBIN, golden :11943 `fLotInfo->RefreshYieldMonitor();` -- same
//      absence as item 13.
//  22. SetSGCONTFAIL, golden :12008-12009/:12019-12020 (FT) and
//      :12035-12036/:12046-12047 (RT), eight `WriteIniData(szDir, "Alarm",
//      ...)` calls (`szDir=DataPath+S+"\Tester.Data"`) -- SHARED MACHINE
//      CONFIG WRITE (IniData\). See "SHARED CONFIG WRITE GATES" list below.
//  23. SetSGCONTFAIL, golden :12010-12011/:12021-12024 (FT) and
//      :12037-12038/:12048-12051 (RT), twelve `fYieldMonitoring->
//      edContsFail...->Text=...` / `->rbContsFail...->Checked=...` lines --
//      none of the six exact widget names this function touches
//      (`edContsFailSocketAlarmCT_FT/_RT`, `edContsFailHeadAlarmCT_FT/_RT`,
//      `rbContsFailBySocket_FTOn/FTOff/RTOn/RTOff`,
//      `rbContsFailByHead_FTOn/FTOff/RTOn/RTOff`) is a declared
//      forms/fYieldMonitoring.h member. `grep -n "edContsFailSocketAlarmCT\|
//      edContsFailHeadAlarmCT\|rbContsFailBySocket\|rbContsFailByHead"
//      forms/fYieldMonitoring.h` -- 0 hits. The real
//      `TestIF_File.bContsFailBySocket(_RT)`/`.bContsFailByHead(_RT)`/
//      `.iContsFailSocketAlarmCT(_RT)`/`.iContsFailHeadAlarmCT(_RT)` globals
//      immediately above each gated block stay ACTIVE. See GOLDEN BUG (B6)
//      below for an inert bug preserved inside this GATE.
//  24. SetSGCONTFAIL, golden :12056 `fLotInfo->RefreshYieldMonitor();` -- same
//      absence as item 13.
//
//  SHARED CONFIG WRITE GATES (per this wave's own "共用 config 寫檔一律 gate"
//  rule: any write toward setup.inf / CurrentSetupData.txt / D:\HT9045\system
//  / config / CFG is `#if 0`-gated with the fixed comment "shared machine
//  config write -- queued for redirect-seam design", because unlike the
//  W906_EVENTLOG_ROOT log-path seam, there is NO redirect seam for these paths
//  and they are shared with the production machine.)
//  ------------------------------------------------------------------------------
//  SCOPE NOTE (read before the list): the wave brief's own enumeration named
//  setup.inf / CurrentSetupData.txt / system\ / config\ / CFG\ verbatim, and
//  did not separately spell out `IniData\`. This wave extends the same gate to
//  every `DataPath`-rooted write (`DataPath` = `D:\HT9045\IniData\Data\`,
//  common.cpp:104) on the strength of the wider project posture already
//  written into this agent's own persona brief and CLAUDE.md's write-boundary
//  table, both of which list `IniData\` alongside `system\`/`config\`/`CFG\`
//  as "量產機共用的執行期參數". `D:\HT9045_Log\CheckingList\...` writes
//  (SetNONDOUBLEBIN/SetBINCOUNT/SetSGOSBIN/SetSGCONTFAIL's `sCheckListFilePath`
//  target) are a DIFFERENT category -- a log directory, not shared machine
//  config -- and are left ACTIVE, matching this wave's own "這與 log 寫檔不同"
//  carve-out.
//    * GATE REGISTER item 7  -- WriteSetSoakTimeStatus_SIGURD, `szDir`
//      (`DataPath+S+"\Temperature.Data"`), 2 statements.
//    * GATE REGISTER item 17 -- SetSGFTP, `sPath` (`AuthPath+"config.ini"`),
//      2 statements. (`config\`, the wave brief's own explicit case.)
//    * GATE REGISTER item 19 -- SetBINCOUNT, `sLastFilePath`
//      (`DataPath+asLastFileName+"\Tester.Data"`), 4 statements.
//    * GATE REGISTER item 22 -- SetSGCONTFAIL, `szDir`
//      (`DataPath+S+"\Tester.Data"`), 8 statements (FT+RT, HeadCT/SocketCT/
//      SocketEnable/HeadEnable).
//
//  VCLCOMPAT-SURFACE SUBSTITUTIONS (behavior-preserving, not gates -- flagged
//  per pt-wave policy so the integration pass can review/veto each; numbered
//  continuing from FW3-WC's S1-S5)
//  ------------------------------------------------------------------------------
//  (S6) SetNONDOUBLEBIN golden :11669 `atoi(tNonDoubleBin->Strings[i].c_str())`
//       and SetBINCOUNT golden :11793/:11820/:11826/:11828
//       `atoi(tBinCount->Strings[i].c_str())` / `asCondition=tBinCount->
//       Strings[i].c_str();` / `atoi(tCondition->Strings[0].c_str())` /
//       `atoi(tCondition->Strings[2].c_str())` -- vclcompat's
//       `TStringList::Strings[i]` returns a `StringsProxy`
//       (vclcompat/TStringList.h:90-99), which carries only an implicit
//       `operator AnsiString()` conversion, NOT a `.c_str()` member -- chaining
//       `.c_str()` directly onto `->Strings[i]` needs TWO user-defined
//       conversions in sequence (StringsProxy->AnsiString->const char*), which
//       C++ does not perform implicitly. Every such call site is wrapped with
//       an explicit `AnsiString(...)` first (e.g.
//       `atoi(AnsiString(tNonDoubleBin->Strings[i]).c_str())`), forcing the
//       first conversion explicitly so the second (`.c_str()`, now a plain
//       member call on a real AnsiString) is legal. Byte-identical result to
//       golden's own `Strings[i].c_str()` in every case. Plain comparisons
//       (`tNonDoubleBin->Strings[i]!=""`, `tBinCount->Strings[i]!=""`) and
//       plain assignments to an AnsiString l-value (`asPassFail=tCondition->
//       Strings[1];`, `asSBin=tSBin->Strings[...]` in SetSBinData,
//       `asBin=tNonDoubleBin->Strings[0];`) need only ONE implicit conversion
//       and are left unwrapped, matching golden's own literal shape.
//  (S7) SetSiteMapData_SIGURD golden :10717 `char *asTestIFSiteMap=
//       GetTestIFSiteMap().c_str();` -- vclcompat::AnsiString::c_str()
//       (vclcompat/AnsiString.h:92) returns `const char*`, not `char*`;
//       assigning it to a non-const `char*` is ill-formed in C++ (BCB6's own
//       AnsiString::c_str() has the same const-qualified signature, so this is
//       a straight port-of-the-type-annotation fix, not a behavior change).
//       Retyped `const char *asTestIFSiteMap=...`; the variable is only ever
//       read afterward (two `t.sprintf("...%s...", ..., asTestIFSiteMap)`
//       calls), so the const qualifier costs nothing.
//  (S8) Send_Command_TTL golden :10508 `sprintf(HHandler2Gpib.Message, "%s",
//       asStr);` -- this is the real C-library `::sprintf` (destination
//       `HHandler2Gpib.Message` is `char Message[2048]`, MessageDef.h:327, not
//       an AnsiString), not AnsiString::sprintf's variadic-template overload.
//       Passing a vclcompat::AnsiString object through real C varargs is
//       undefined behaviour, same class of issue as FW3-WC's own (S2).
//       Substituted with `asStr.c_str()`; produces the byte-identical `%s`
//       output golden intended.
//
//  STUB COLLISIONS  (per this wave's task brief -- retire in the NEXT
//  integration pass, NOT this one; this wave is barred from touching
//  forms/fMain.cpp; same treatment FW3-WA already gave ArmStatusStrings/
//  WritePERSITETemperature/PERSITETemperatureStrings)
//  ------------------------------------------------------------------------------
//    GetSamSungMap       -- offline stub at forms/fMain.cpp:282 (`return AnsiString("");`)
//    GetSamSungSoakTime  -- offline stub at forms/fMain.cpp:283 (`return AnsiString("0");`)
//  Re-verified by `grep -n "TfMain::<name>\b" --include=*.cpp .` for all 29
//  names, 20260818 -- these 2 are the ONLY existing bodies anywhere in the
//  port tree; no other collision exists (in particular GetSamSungTmp has
//  none). forms/fMain.h's two pre-existing declarations (golden main.h:1531/
//  1532, its own W5-Automation INTEGRATE ADD block) are left untouched and
//  NOT re-declared in this wave's FW3-WD ADD block -- the two symbols would
//  collide even as bare re-declarations of an identical signature. Once
//  Command.cpp is added to CMakeLists.txt, forms/fMain.cpp:282-283 must be
//  deleted (linker will otherwise report "multiple definition").
//
//  GOLDEN BUG / ODDITY  (continuing from FW3-WC's B1-B4; recorded, NOT fixed,
//  per this wave's "golden 不合理處照翻並註記, 改行為需使用者決定" rule)
//  ------------------------------------------------------------------------------
//  (B5) ChkStatus golden :10958-10959 (`char runStartMode[10]; char
//       MainStatus[10];`) and SetStartMode golden :11205 (`char
//       runStartMode[10];`) -- all three are plain, UNINITIALISED C arrays.
//       Each is populated ONLY by one arm of an if/else-if ladder over
//       `LastSet.iRunStartMode` (ChkStatus) / `fMain->palMainStatus->Caption`
//       (ChkStatus's `MainStatus`) that has NO final `else` branch. If
//       `iRunStartMode`/`Caption` ever holds a value outside every listed
//       case, the array is read uninitialised by the trailing
//       `t.sprintf("...%s...", MainStatus, runStartMode)` (ChkStatus) /
//       `t.sprintf("...%s", runStartMode)` (SetStartMode) call -- and in
//       SetStartMode's case, the matching `else` arm that DOES exist only
//       calls `ShowMyMessage(...)` and does NOT `return`, so execution falls
//       through into the same uninitialised read regardless. Translated
//       verbatim (bare `char[10]`, no zero-init) -- this reproduces the exact
//       same undefined-behaviour class golden already has, not a new one this
//       port introduces.
//  (B6) SetSGCONTFAIL golden :12024 `fYieldMonitoring->rbContsFailByHead_FTOff
//       ->Checked=TestIF_File.bContsFailByHead;` and golden :12051
//       `fYieldMonitoring->rbContsFailByHead_RTOff->Checked=TestIF_File.
//       bContsFailByHead_RT;` -- BOTH lines are missing the `!` negation their
//       sibling `rbContsFailBySocket_FTOff`/`_RTOff` lines correctly apply two
//       lines above each (golden :12022/:12049, `Checked=!TestIF_File.
//       bContsFailBySocket(_RT)`). A consistent FT+RT copy-paste miss (the
//       "On" radio gets the real value, the "Off" radio should get its
//       negation, and does for Socket but not for Head), preserved verbatim
//       inside GATE REGISTER item 23 -- currently inert (the whole
//       `fYieldMonitoring` cluster is gated), but will reproduce golden's own
//       "Head Off checkbox never actually shows Off" defect the moment that
//       facade lands, unless fixed by explicit user decision at that time.
// =============================================================================
AnsiString TfMain::GetSamSungTmp(bool bSend)                                    //Steven 20191112 : 三星格式
{
    //Tester Send: TMP?
    //Handler Reply: Chamber,HP1,HP2,SH1,SH2,
    //If heater off, show NA
    //If no chamber, use the average of index heater.
    AnsiString Msg="", Str1, Str;
    int iCount=0;
    double dTemp=0.0, dResult=0.0, dTemp1=0.0;
    double dBase=25.0;
    if(LastSet.iTemperature!=Tempture_Ambient)
        dBase=Temperature.fWorkTemperBase;

    if(Temperature.iIndexHeatMode==ChamberOnly)
    {
        dTemp1=atof(asGPIBTempShow[tcChamber].c_str());

        if(dTemp1>dBase+10.0 ||
           dTemp1<dBase-10.0)
        {
            Str.sprintf("dTemp:%0.2f, Chamber", dTemp1);
            NewRecordProcess("", "GetSamSungTmp error!", Str);
            dTemp1=dBase;
        }
        Str1.sprintf("%0.1f,", dTemp1);
    }
    else
    {
        for(int i=tcHead1; i<=tcHead4; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        for(int i=tcAa1; i<=tcBd2; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        for(int i=tcAe1; i<=tcBh2; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        if(dTemp==0.0 || iCount==0)
        {
            Str1.sprintf("%0.1f,", dBase);
            Str.sprintf("dTemp:%0.2f, iCount:%d", dTemp, iCount);
            NewRecordProcess("", "GetSamSungTmp error!", Str);
        }
        else
        {
            dResult=(dTemp/(double)iCount);
            if(dResult>dBase+10.0 ||
               dResult<dBase-10.0)
            {
                dResult=dBase;
                Str.sprintf("dTemp:%0.2f, iCount:%d", dTemp, iCount);
                NewRecordProcess("", "GetSamSungTmp error!", Str);
            }
            Str1.sprintf("%0.1f,", dResult);
        }
    }
    Msg=Str1;

    if(bUT150Install[tcHotPlate1])
        Str1=asGPIBTempShow[tcHotPlate1]+",";
    else if(bUT150Install[tcHotPlate2])                                         //Steven 20210108 : Hana 要求關掉的加熱盤也要有溫度
        Str1=asGPIBTempShow[tcHotPlate2]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcHotPlate2])
        Str1=asGPIBTempShow[tcHotPlate2]+",";
    else if(bUT150Install[tcHotPlate1])                                         //Steven 20210108 : Hana 要求關掉的加熱盤也要有溫度
        Str1=asGPIBTempShow[tcHotPlate1]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcShuttle1])
        Str1=asGPIBTempShow[tcShuttle1]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcShuttle2])
        Str1=asGPIBTempShow[tcShuttle2]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Tmp, Msg);
    return Msg;
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSamSungMap(bool bSend)                                    //Steven 20191112 : 三星格式
{
    AnsiString Msg="";

    if(TestIF_File.iTestMode==SingleSite  ||
       TestIF_File.iTestMode==DualSite    ||
       TestIF_File.iTestMode==TriSite1X3  ||
       TestIF_File.iTestMode==QualSite1X4)
    {
        Msg.sprintf("%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1 ||
            TestIF_File.iTestMode==QualSite2X2 ||
            TestIF_File.iTestMode==QualSite2X2N)     //Frank 20200520 2X2NN Mode
    {
        Msg.sprintf("%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3  ||
            TestIF_File.iTestMode==_6Site2X3N ||   //Steven 20220425 : 2X3NN Mode
            TestIF_File.iTestMode==_8Site2X4N ||   //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_8Site2X4)
    {
        Msg.sprintf("%d%d%d%d%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        Msg.sprintf("0%d0%d%d%d%d%d%d%d%d%d%d0%d0",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[0][4],
                        TestIF_File.iSiteMap[1][3],
                        TestIF_File.iSiteMap[1][4],
                        TestIF_File.iSiteMap[0][5],
                        TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        Msg.sprintf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[2][0],
                        TestIF_File.iSiteMap[2][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[2][2],
                        TestIF_File.iSiteMap[2][3],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[3][0],
                        TestIF_File.iSiteMap[3][1],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[1][3],
                        TestIF_File.iSiteMap[3][2],
                        TestIF_File.iSiteMap[3][3]);
    }
    //QQ 20230214 : 4x8
    else if(TestIF_File.iTestMode==_32Site4X8N ||
            TestIF_File.iTestMode==_32Site4X8M)
    {
    }
    else //if(TestIF_File.iTestMode==_16Site2X8)
    {
        if(TestIF_File.iSiteMap[0][0]==0 &&
           TestIF_File.iSiteMap[1][0]==0 &&
           TestIF_File.iSiteMap[0][1]==0 &&
           TestIF_File.iSiteMap[1][1]==0 &&
           TestIF_File.iSiteMap[0][3]==0 &&
           TestIF_File.iSiteMap[1][3]==0 &&
           TestIF_File.iSiteMap[0][5]==0 &&
           TestIF_File.iSiteMap[1][5]==0 &&
           TestIF_File.iSiteMap[0][6]==0 &&
           TestIF_File.iSiteMap[1][6]==0 &&
           TestIF_File.iSiteMap[0][7]==0 &&
           TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d",
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][4]);
        }
        else if(TestIF_File.iSiteMap[0][1]==0 &&
                TestIF_File.iSiteMap[1][1]==0 &&
                TestIF_File.iSiteMap[0][3]==0 &&
                TestIF_File.iSiteMap[1][3]==0 &&
                TestIF_File.iSiteMap[0][5]==0 &&
                TestIF_File.iSiteMap[1][5]==0 &&
                TestIF_File.iSiteMap[0][7]==0 &&
                TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][0],
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[1][0],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][6],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][6]);
        }
        else if(TestIF_File.iSiteMap[0][0]==0 &&
                TestIF_File.iSiteMap[1][0]==0 &&
                TestIF_File.iSiteMap[0][1]==0 &&
                TestIF_File.iSiteMap[1][1]==0 &&
                TestIF_File.iSiteMap[0][6]==0 &&
                TestIF_File.iSiteMap[1][6]==0 &&
                TestIF_File.iSiteMap[0][7]==0 &&
                TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][3],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][3],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][5],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][5]);
        }
        else
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][0],
                            TestIF_File.iSiteMap[0][1],
                            TestIF_File.iSiteMap[1][0],
                            TestIF_File.iSiteMap[1][1],
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][3],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][3],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][5],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][5],
                            TestIF_File.iSiteMap[0][6],
                            TestIF_File.iSiteMap[0][7],
                            TestIF_File.iSiteMap[1][6],
                            TestIF_File.iSiteMap[1][7]);
        }
    }

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Map, Msg);

    return Msg;
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSamSungSoakTime(bool bSend)
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient ||
       LastSet.iTemperature==Tempture_AmbientHot)
    {
        t.sprintf("0");
    }
    else
    {
        t.sprintf("%s", edSoakTime->Text.c_str());
    }

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Soak, t);

    return t;
}
//---------------------------------------------------------------------------
void TfMain::GetTTLState()                                                      //Isaac 20200903 :TTL RS232通訊
{
    TStringList *asBuffer;
    asBuffer=new TStringList();
    AnsiString Str;
    HHandler2Gpib.iSendCommand=MSG_CMD_State_TTL;
    memset(HHandler2Gpib.Message,'\0', sizeof(HHandler2Gpib.Message));//清空陣列

    HHandler2Gpib.bGpibMode=true;   //Isaac 20210922 : 選擇TTL板子是否帶站別
    HHandler2Gpib.bSupport32Bin=false;      //true : use two TTL board  //Isaac 20210309 :TTL RS232兩塊板子(先不使用)
    // GATE(FW3-WD) golden :10334-10335 -- see GATE REGISTER item 1 above.
#if 0
    HHandler2Gpib.HandlerHwnd=this->Handle; //Isaac 20211115 : 修正因檢查handler和RS232視窗是否開啟而自動關閉RS232視窗
    HHandler2Gpib.GpibHwnd=HVisionWnd;      //Isaac 20211115 : 修正因檢查handler和RS232視窗是否開啟而自動關閉RS232視窗
#endif
    if(TTL_CARD_TYPE==3)
    {
        HHandler2Gpib.bGpibMode=true;   //true:要帶站號
        HHandler2Gpib.bSupport32Bin=true;//true : use two TTL board //Isaac 20210309 :TTL RS232兩塊板子
        if(Prod.DIOCfg.iCateBitLength==_3Bit || Prod.DIOCfg.iCateBitLength==_4Bit   ||
           Prod.DIOCfg.iCateBitLength==_5Bit || Prod.DIOCfg.iCateBitLength==_5BitPE ||
           Prod.DIOCfg.iCateBitLength==_5BitPO)
        {
            if(Prod.DIOCfg.iCateDataType==CHOneByOne)
            {
                //5BitBit
                if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
            else    //CHBinary
            {
                //5BitBinary
                if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
        }
        else
        {
            if(Prod.DIOCfg.iCateDataType==CHOneByOne)
            {
                //10BitBit
                if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
            else    //CHBinary
            {
                //10BitBinary
                if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
        }
    }
    else
    {
        HHandler2Gpib.bGpibMode=TTL_CARD_USE_ADDRESS;   //true:要帶站號     //Isaac 20210922 : 選擇TTL板子是否帶站別
        HHandler2Gpib.bSupport32Bin=false;  //false : use one TTL board
    }
//******************************************
//以下順序不能對調!!
//******************************************
    //Data[0] TS+5V
    asBuffer->Add("0");

    //Data[1] Bin Mode
    if(TestIF_File.bTTLUseASEJPMode)    //Frank 20220408 Add TTL ASE_JP Mode
    {
        asBuffer->Add("4");
    }
    else if(Prod.DIOCfg.iCateBitLength==_3Bit || Prod.DIOCfg.iCateBitLength==_4Bit   ||
            Prod.DIOCfg.iCateBitLength==_5Bit || Prod.DIOCfg.iCateBitLength==_5BitPE ||
            Prod.DIOCfg.iCateBitLength==_5BitPO)
    {
        if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        {
            asBuffer->Add("0");     //5BitBit

            if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
        else    //CHBinary
        {
            asBuffer->Add("2");     //5BitBinary

            if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
    }
    else
    {
        if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        {
            asBuffer->Add("1");     //10BitBit

            if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
        else    //CHBinary
        {
            asBuffer->Add("3");     //10BitBinary

            if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
    }

    if(TestIF_File.bTTLUseASEJPMode)    //Frank 20220408 Add TTL ASE_JP Mode
    {
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
    }
    else
    {
        //Data[2-9] SOT Active Logic
        if(Prod.DIOCfg.iSTLogicMode==0) //Positive
            asBuffer->Add("11111111");
        else                            //Negative
            asBuffer->Add("00000000");

        //Data[10-17] Data Active Logic
        if(Prod.DIOCfg.iCateLogicMode==0)     //Positive
            asBuffer->Add("11111111");
        else                            //Negative
            asBuffer->Add("00000000");

        //Data[18-25] EOT Active Logic
        asBuffer->Add("00000000");

        //Data[26-33] DUT Active Logic
        if(Prod.DIOCfg.iDutType==0 ||   //None
           Prod.DIOCfg.iDutType==1 ||   //Positive Level
           Prod.DIOCfg.iDutType==2)     //Positive Pulse
            asBuffer->Add("11111111");
        else                            //Negative Level || Negative Pulse
            asBuffer->Add("00000000");
    }
    //Data[34-37] SOT Width (MS, 最大1000)
    Str.sprintf("%04d", Prod.DIOCfg.iSTPluseWidth);
    asBuffer->Add(Str);

    //Data[38-41] Dut Width (MS, 最大1000)
    Str.sprintf("%04d", 200);
    asBuffer->Add(Str);

    //Data[42-47] BIN Time Out (MS, 最大500000)
    Str.sprintf("%06d", 0);
    asBuffer->Add(Str);
//******************************************
//以上順序不能對調!!
//******************************************

    Str=StringReplace(asBuffer->Text, "\r\n", "", TReplaceFlags()<<rfReplaceAll);

    strncpy(HHandler2Gpib.Message, Str.c_str(), Str.Length());
    COPYDATASTRUCT *pcp=new COPYDATASTRUCT ;
    pcp->dwData=0;
    pcp->cbData=sizeof(HHandler2Gpib);
    pcp->lpData=(unsigned char *)&HHandler2Gpib.iSendCommand;

    // GATE(FW3-WD) golden :10497 -- see GATE REGISTER item 2 above.
#if 0
    SendMessage(fMain->HVisionWnd, WM_COPYDATA,(WPARAM) NULL, (LPARAM)pcp);
#endif
    delete pcp;
    delete asBuffer;
}
//---------------------------------------------------------------------------
void TfMain::Send_Command_TTL(AnsiString asStr)
{
    const int iLength=asStr.Length();
    if(TestIF_File.iTestType==TTL_MODE && (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))       //Isaac 20210309 :TTL RS232兩塊板子
    {
        HHandler2Gpib.iLotStatus=iLength;
        // AI(W906-FW3-WD) 20260818: substitution (S8) -- see FW3-WD GROUP
        // banner above for the full citation.
        sprintf(HHandler2Gpib.Message, "%s", asStr.c_str());
        fMain->SendMSG_CMD(MSG_CMD_Command_TTL);
    }
}
//---------------------------------------------------------------------------
void TfMain::WriteSetTempStatus_SIGURD()                                        //KaiChen 20181129 ：Add GPIB SETTEMP_
{
    int ret=0, TempMode;
    double d=0.0;
    AnsiString t;
    bool bStatusOK=false;
    bool bValueOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    d=atof(HGpib2Handler->cReturn);
    // GATE(FW3-WD) golden :10530-10531 -- see GATE REGISTER item 3 above.
    // dTempMax/dTempMin keep whatever value the rest of the system last wrote.
#if 0
    dTempMax=fTemp_Set->MaxTempSetting();
    dTempMin=fTemp_Set->MinTempSetting();
#endif
    if(d<=dTempMax)                                                             // 檢查是否超出最大值
    {
        bValueOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetTemp, t);
    }
    else if(bValueOK==false)
    {
        t.sprintf("VALUE NG");
        SendMSG_CMD(MSG_CMD_SetTemp, t);
    }
    else
    {
        if(Temperature.bATCActiveCooling)                                       //Sam 20250214 : RMS 新增 ATC 判斷
        {
            if(d>25.0)
            {
                TempMode=1;
            }
            else
            {
                TempMode=0;
            }
            // GATE(FW3-WD) golden :10559 -- see GATE REGISTER item 4 above.
#if 0
            edATCAmbientTemper->Text=d;
#endif
        }
        else
        {
            if(d>25)
            {
                TempMode=1;
                edWorkTemperBase->Text=d;
            }
            else
            {
                TempMode=0;
            }
        }
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        //==>
        t.sprintf("SETTINGOK>SETTEMP_%d>%d", (int)d, (int)d);
        SendMSG_CMD(MSG_CMD_SetTemp, t);
        //<==
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        if(Temperature.bATCActiveCooling)                                       //Sam 20250214 : RMS 新增 ATC 判斷
            // GATE(FW3-WD) golden :10580 -- see GATE REGISTER item 4 above:
            // `edATCAmbientTemper->Text` is skipped; `d` (its would-be mirror,
            // set two lines above in this same arm) is used directly instead.
            ret=SetTemp(false, d, atof(edSoakTime->Text.c_str()));
        else
            ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
        if(ret==0)
        {
            // GATE(FW3-WD) golden :10585 -- see GATE REGISTER item 5 above.
            // `ret` is left at SetTemp()'s own return value.
#if 0
            ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);
#endif
        }

        if(ret!=0)
        {
            t.sprintf("SETTINGNG>SETTEMP_%d>%s Error", (int)d, edWorkTemperBase->Text);
            ShowMyMessage(t, "");
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::WriteSetSoakTimeStatus_SIGURD()                                    //KaiChen 20181129 ：Add GPIB SETSOAK_
{
    int i, ret=0;
    AnsiString t,S="",szDir="";
    bool bStatusOK=false;
    bool bValueOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    i=atoi(HGpib2Handler->cReturn);
    if(i<=1000 && i>=0) //檢查是否超出最大值
    {
        bValueOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
    }
    else if(bValueOK==false)
    {
        t.sprintf("VALUE NG");
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
    }
    else
    {
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        //==>
        edSoakTime->Text=i;
        t.sprintf("SETTINGOK>SETSOAK_%d>%s", i, edSoakTime->Text);
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
        //<==
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut

        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));     //Steven 20120730
        if(ret==0)
        {
            S=GetLastOpenFN();
            szDir.sprintf("%s%s", DataPath, S);
            szDir+="\\Temperature.Data";
            if(i==0)  //常溫
            {
                if(CUSTOMER_CODE==CC_UTAC_TW)   //Sa
                {
                    //Sam 20240606 : 聯測 SoakTime = 0 不要變更溫度模式
                }
                else
                {
                    // GATE(FW3-WD) golden :10651-10652 -- see GATE REGISTER
                    // items 6 and 7 above (ChangeTempMode absence; IniData
                    // shared config write).
#if 0
                    fMain->ChangeTempMode(0, false, true);
                    WriteIniData(szDir, "Mode", "Mode", 1);                     // shared machine config write -- queued for redirect-seam design
#endif
                }
            }
            else      //高溫
            {
                // GATE(FW3-WD) golden :10657-10658 -- see GATE REGISTER items
                // 6 and 7 above (ChangeTempMode absence; IniData\ shared
                // config write).
#if 0
                fMain->ChangeTempMode(1, false, true);
                WriteIniData(szDir, "Mode", "Mode", 0);                         // shared machine config write -- queued for redirect-seam design
#endif
            }
        }

        if(ret!=0)
        {
            t.sprintf("SETTINGNG>SETSOAK_%d>%s", i, edSoakTime->Text);
            ShowMyMessage(t, "");
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSiteMapData_SIGURD()                                            //KaiChen 20181129 ：Add GPIB SETSITEMAP_
{
    bool ret=true;
    char str[256];
    char str_buf[256];
    AnsiString t;
    bool bStatusOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode) &&                                    //Sam 20250214 : RMS 新增 QA 模式
       HasICUnderMachine()==false)
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSiteMapData, t);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        for(int i=0; i<sizeof(str); i++)
        {
            if(str[i]=='\r' || str[i]=='\n')
            {
                str_buf[i]='\0';
            }
            else
            {
                str_buf[i]=str[i];
            }
        }

        for(int i=0; i<4; i++)  //JerryYang 20160519 備份開關site
        {
            for(int j=0; j<8; j++)
            {
                iBackupDutOnOff[i][j]=bTestSiteUse[0][i][j];
            }
        }
        iBackupTestMode=TestIF_File.iTestMode;
        // GATE(FW3-WD) golden :10715 -- see GATE REGISTER item 8 above. `ret`
        // keeps its declared initial value (`true`) rather than being reset.
#if 0
        ret=ChangeToSiteMap(str);
#endif

        // AI(W906-FW3-WD) 20260818: substitution (S7) -- see FW3-WD GROUP
        // banner above for the full citation.
        const char *asTestIFSiteMap=GetTestIFSiteMap().c_str();
        if(ret)
        {
            t.sprintf("SETTINGOK>SETSITEMAP_%s>%s\n", str_buf, asTestIFSiteMap);
        }
        else
        {
            t.sprintf("SETTINGNG>SETSITEMAP_%s>%s\n", str_buf, asTestIFSiteMap);
        }
        SendMSG_CMD(MSG_CMD_SetSiteMapData, t);
    }
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetTestIFSiteMap()   //KaiChen 20181129 ：Add GPIB SETSITEMAP_
{
    AnsiString asReturn;
    int iSiteMap[32]={-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
    int i=0;
    AnsiString sSiteMode;

    if(TestIF_File.iTestMode==SingleSite)//*   SingleSite
    {
        asReturn="SINGLE1X1";                           //Steven 20230214 : 補上GPIB set site map
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
    }
    else if(TestIF_File.iTestMode==DualSite)//*   DualSite
    {
        asReturn="DUAL1X2";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
    }
    else if(TestIF_File.iTestMode==TriSite1X3)//
    {
        asReturn="TRI1X3";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
        iSiteMap[2]=TestIF_File.iSiteMap[0][2];
    }
    else if(TestIF_File.iTestMode==QualSite1X4)//*     QualSite1X4    _8Site1X4
    {
        asReturn="QUAD1X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
        iSiteMap[2]=TestIF_File.iSiteMap[0][2];
        iSiteMap[3]=TestIF_File.iSiteMap[0][3];
    }
    else if(TestIF_File.iTestMode==DualSite2x1)//*    DualSite2x1
    {
        asReturn="DUAL2X1";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
    }
    else if(TestIF_File.iTestMode==QualSite2X2)//*    QualSite2X2
    {
        asReturn="QUAD2X2";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)                                //Steven 20230214 : 補上GPIB set site map
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="QUAD2X2";
        else
            asReturn="QUAD2X2N";

        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
    }
    else if(TestIF_File.iTestMode==_6Site2X3)//*      _6Site2X3
    {
        asReturn="6SITE2X3";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20230214 : 補上GPIB set site map
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="6SITE2X3";
        else
            asReturn="6SITE2X3N";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="8SITE2X4";
        else
            asReturn="8SITE2X4N";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
    }
    else if(TestIF_File.iTestMode==_8Site2X4)//*    _8Site2X4
    {
        asReturn="8SITE2X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        asReturn="10SITE2X5";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
    }
    else if(TestIF_File.iTestMode==_12Site2X6)//*    _12Site2X6
    {
        asReturn="12SITE2X6";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
        iSiteMap[10]=TestIF_File.iSiteMap[0][5];
        iSiteMap[11]=TestIF_File.iSiteMap[1][5];
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //*     _16Site2X8
    {
        asReturn="16SITE2X8";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
        iSiteMap[10]=TestIF_File.iSiteMap[0][5];
        iSiteMap[11]=TestIF_File.iSiteMap[1][5];
        iSiteMap[12]=TestIF_File.iSiteMap[0][6];
        iSiteMap[13]=TestIF_File.iSiteMap[1][6];
        iSiteMap[14]=TestIF_File.iSiteMap[0][7];
        iSiteMap[15]=TestIF_File.iSiteMap[1][7];
    }
    else if(TestIF_File.iTestMode==_16Site4X4)          //Steven 20230214 : 補上GPIB set site map
    {
        asReturn="16Site4X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[2][0];
        iSiteMap[3]=TestIF_File.iSiteMap[3][0];
        iSiteMap[4]=TestIF_File.iSiteMap[0][1];
        iSiteMap[5]=TestIF_File.iSiteMap[1][1];
        iSiteMap[6]=TestIF_File.iSiteMap[2][1];
        iSiteMap[7]=TestIF_File.iSiteMap[3][1];
        iSiteMap[8]=TestIF_File.iSiteMap[0][2];
        iSiteMap[9]=TestIF_File.iSiteMap[1][2];
        iSiteMap[10]=TestIF_File.iSiteMap[2][2];
        iSiteMap[11]=TestIF_File.iSiteMap[3][2];
        iSiteMap[12]=TestIF_File.iSiteMap[0][3];
        iSiteMap[13]=TestIF_File.iSiteMap[1][3];
        iSiteMap[14]=TestIF_File.iSiteMap[2][3];
        iSiteMap[15]=TestIF_File.iSiteMap[3][3];
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)                                 //Steven 20230214 : 補上GPIB set site map
    {
        asReturn="32Site4X8";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[2][0];
        iSiteMap[3]=TestIF_File.iSiteMap[3][0];
        iSiteMap[4]=TestIF_File.iSiteMap[0][1];
        iSiteMap[5]=TestIF_File.iSiteMap[1][1];
        iSiteMap[6]=TestIF_File.iSiteMap[2][1];
        iSiteMap[7]=TestIF_File.iSiteMap[3][1];
        iSiteMap[8]=TestIF_File.iSiteMap[0][2];
        iSiteMap[9]=TestIF_File.iSiteMap[1][2];
        iSiteMap[10]=TestIF_File.iSiteMap[2][2];
        iSiteMap[11]=TestIF_File.iSiteMap[3][2];
        iSiteMap[12]=TestIF_File.iSiteMap[0][3];
        iSiteMap[13]=TestIF_File.iSiteMap[1][3];
        iSiteMap[14]=TestIF_File.iSiteMap[2][3];
        iSiteMap[15]=TestIF_File.iSiteMap[3][3];
        iSiteMap[16]=TestIF_File.iSiteMap[0][4];
        iSiteMap[17]=TestIF_File.iSiteMap[1][4];
        iSiteMap[18]=TestIF_File.iSiteMap[2][4];
        iSiteMap[19]=TestIF_File.iSiteMap[3][4];
        iSiteMap[20]=TestIF_File.iSiteMap[0][5];
        iSiteMap[21]=TestIF_File.iSiteMap[1][5];
        iSiteMap[22]=TestIF_File.iSiteMap[2][5];
        iSiteMap[23]=TestIF_File.iSiteMap[3][5];
        iSiteMap[24]=TestIF_File.iSiteMap[0][6];
        iSiteMap[25]=TestIF_File.iSiteMap[1][6];
        iSiteMap[26]=TestIF_File.iSiteMap[2][6];
        iSiteMap[27]=TestIF_File.iSiteMap[3][6];
        iSiteMap[28]=TestIF_File.iSiteMap[0][7];
        iSiteMap[29]=TestIF_File.iSiteMap[1][7];
        iSiteMap[30]=TestIF_File.iSiteMap[2][7];
        iSiteMap[31]=TestIF_File.iSiteMap[3][7];
    }

    for(i=0; i<32; i++)
    {
        if(iSiteMap[i]>-2)
        {
            asReturn=asReturn+"-"+iSiteMap[i];
        }
    }
    return  asReturn;
}
//---------------------------------------------------------------------------
void TfMain::ChkStatus()                                                        //KaiChen 20180910 ：Add GPIB CHKSTATUS?
{
    // GOLDEN ODDITY (B5) -- see FW3-WD GROUP banner above for the full
    // citation. Both arrays are left genuinely uninitialised, exactly as
    // golden declares them.
    char runStartMode[10];
    char MainStatus[10];
    AnsiString t;

    if(fMain->palMainStatus->Caption=="HALT")
    {
        sprintf(MainStatus, "HALT");
    }
    else if(fMain->palMainStatus->Caption=="PAUSE")
    {
        sprintf(MainStatus, "PAUSE");
    }
    else if(fMain->palMainStatus->Caption=="Running")
    {
        sprintf(MainStatus, "RUNNING");
    }

    if(LastSet.iRunStartMode==rsmContinuStart ||                                //確認機台狀態
       LastSet.iRunStartMode==rsmContinuStart_MRT)                              //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "FT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmInitialStart ||
            LastSet.iRunStartMode==rsmInitial_MRT)                              //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "FT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest)
    {
        sprintf(runStartMode, "RT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
    {
        sprintf(runStartMode, "RT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmRetest_MRT)                               //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "RT");
    }
    else if(LastSet.iRunStartMode==rsmQAMode)                                   //Sam 20240815 : 新增 QA Mode
    {
        sprintf(runStartMode, "QA_INIT");
    }

    if(LastSet.iRunStartMode==rsmContinuStart_MRT ||                            //Sam 20240625 : 新增 MRT Status
       LastSet.iRunStartMode==rsmInitial_MRT      ||
       LastSet.iRunStartMode==rsmRetest_MRT)
    {
        t.sprintf("MRT_%s_%s", MainStatus, runStartMode);
    }
    else
    {
        t.sprintf("%s_%s", MainStatus, runStartMode);
    }
    SendMSG_CMD(MSG_CMD_SIGURD_CHKSTATUS, t);
}
//---------------------------------------------------------------------------
void TfMain::GetBinCategory()   //KaiChen 20180913 ：Add GPIB GETBINCATEGORY?
{
    AnsiString asCategData;
    for(int i=0; i<15; i++)
    {
        asCategData=asCategData+BinSelect[iTestRunMode].iCatDataT3Pos[15-i];   //QQQ
    }
    asCategData=asCategData+(BinSelect[iTestRunMode].IfErrorT3+1);              //Sam 20250221 : 修正矽格 RMS 回傳資料
    SendMSG_CMD(MSG_CMD_GETBINCATEGORY, asCategData);
}
//---------------------------------------------------------------------------
void TfMain::GetSetUpFileName()     //KaiChen 20181022 ：Add GPIB GETSETUPFILENAME?
{
    AnsiString asFileName;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        asFileName=fMain->cbSetupFileName->Text+"\r\n";//Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    }
    else
    {
        asFileName.sprintf("%s", fMain->cbSetupFileName->Text);
    }
    SendMSG_CMD(MSG_CMD_SETUPFILENAME, asFileName);
}
//---------------------------------------------------------------------------
void TfMain::GetHandlerID_Sigurd()                                              //KaiChen 20200507 ：Add GPIB HANDLERID?
{
    AnsiString asHandlerID;

    asHandlerID=IniConfig.asA32_1_HandlerID;

    if(IniConfig.bA32_2ReturnHandlerID2OI==true)                                //KaiChen 20200618 ：矽格，可以選擇是否回傳 HandlerID 給 OI
    {
        SendMSG_CMD(MSG_CMD_SIGURD_HANDLERID, asHandlerID);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSetupFileName()                                                 //KaiChen 20190613 ：Add GPIB SGSETUP_
{
    bool ret=false;
    bool bStatusOK=false;
    char str[256];
    AnsiString t;

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    AnsiString asChangeSetupFileName=AnsiString(str).Trim();

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SGSETUP, t);
    }
    else
    {
        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)    //KaiChen 20191128 ：矽格-北興，SetupFileName 不等於空白，就回傳 OK
        {
            if(asChangeSetupFileName!="")
                ret=true;
        }
        else
        {
            // GATE(FW3-WD) golden :11086 -- see GATE REGISTER item 9 above.
            // `ret` stays its declared default `false`.
#if 0
            if(fFTPClient->CheckSetupFileNameFromServer(asChangeSetupFileName)==true)   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
                ret=true;
#endif
        }

        if(ret==true)
            t.sprintf("SETTINGOK");
        else
            t.sprintf("SETTINGNG");

        SendMSG_CMD(MSG_CMD_SGSETUP, t);

        if(ret==true)
        {
            ChangeSetupFileName(str);
        }
    }
}
//---------------------------------------------------------------------------
bool TfMain::ChangeSetupFileName(char *str)
{
    AnsiString asChangeSetupFileName=AnsiString(str).Trim();
    AnsiString asNowSetupFileName=fMain->cbSetupFileName->Text;

    if(SystemStart==true)
    {
        return false;
    }

    if(asChangeSetupFileName==asNowSetupFileName)   // 檢查是否與當前 Recipe 一樣
    {
        return false;
    }

    bSigurdDownload_Recipe=true;
    // GATE(FW3-WD) golden :11120-11127 -- see GATE REGISTER item 10 above.
    // Faithful default: no download channel exists offline, so the recipe
    // change cannot have succeeded.
#if 0
    fFTPClient->bControlByGPIB=true;        //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    fFTPClient->iErrorByGPIB=0;
    fFTPClient->asSetUpNameByGPIB=asChangeSetupFileName;
    fFTPClient->ShowFTPModal(0);

    bSigurdDownload_Recipe=false;
    fFTPClient->bControlByGPIB=false;
    if(fFTPClient->iErrorByGPIB==0)
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    bSigurdDownload_Recipe=false;
    return false;
#endif
}
//---------------------------------------------------------------------------
//Richard 20220929 :Add for UTAC 讀檔
//==>
void TfMain::PPSELECTAskFile()
{
    AnsiString asFileName="";
    asFileName.sprintf("%s", fMain->cbSetupFileName->Text);
    SendMSG_CMD(MSG_CMD_ASKPPSELECT, asFileName);
}
//==============================================================================
// GATE(FW3-WD) golden :11163 -- see GATE REGISTER item 11 above. TU-local
// no-op stand-in for golden's real VCL Dialogs.hpp `ShowMessage(const
// AnsiString&)`, matching the established tree-wide convention (ATC/
// ATCSystem.cpp, ProductionInfo/TfFTP.cpp, SECSGEM/SecsSvEcRegistration.cpp,
// PMAlarm/PMAlarmSystem.cpp, SECSGEM/uHGemEquipment.cpp each carry their own
// identically-shaped stand-in).
static void Gated_ShowMessage(const AnsiString & /*S*/) {}
void TfMain::PPSELECTLoadFile()
{
    char str[256];

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    AnsiString atemp="";
    AnsiString afiletemp="";
    atemp=str;                                                       //example The Function File Name is "0809_PROD"
//    int itemp=0;                                                   //"PPSELECT 0809_PROD"
//    itemp=str.Pos(PPSELECT);
    afiletemp=(atemp.Trim()).SubString(atemp.Pos("T")+2,atemp.Length());

    bool bHasFile=false;

    if(HasICUnderMachine()==true)
    {
        bHasFile=false;
        Gated_ShowMessage("Has IC Under Machine");
    }
    else
    {
        for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
        {
            if( (fMain->cbSetupFileName->Items->Strings[i])==afiletemp )
            {
                if(fMain->cbSetupFileName->Text!=fMain->cbSetupFileName->Items->Strings[i])
                {
                    fMain->cbSetupFileName->Text=fMain->cbSetupFileName->Items->Strings[i];
                    fMain->cbSetupFileNameChange(fMain);
                }
                bHasFile=true;
            }
        }
    }

    if(bHasFile==true) //如果沒有檔案 報錯
    {
        SendMSG_CMD(MSG_CMD_PPSELECT, "ECHOCODEOK");
        iFileOkPPSELECT=1;
    }
    else
    {
        SendMSG_CMD(MSG_CMD_PPSELECT, "ECHOCODENG");
        iFileOkPPSELECT=2;
    }
}
//<==
//Richard 20220929 :Add for UTAC 讀檔
//---------------------------------------------------------------------------
void TfMain::SetStartMode()     //KaiChen 20180910 ：Add GPIB SetStartMode_
{
    bool ret=true;
    char str[256];
    AnsiString t;
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    ret=ChangeHandlerStartMode(str);

    AnsiString sMultiContactCount=AnsiString(str).Trim();   //用來判別是 FT 還是 RT
    // GOLDEN ODDITY (B5) -- see FW3-WD GROUP banner above for the full
    // citation. Left genuinely uninitialised, exactly as golden declares it.
    char runStartMode[10];

    if(LastSet.iRunStartMode==rsmContinuStart)  //確認機台狀態
    {
        sprintf(runStartMode, "FT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmInitialStart)
    {
        sprintf(runStartMode, "FT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest)
    {
        sprintf(runStartMode, "RT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
    {
        sprintf(runStartMode, "RT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmQAMode) //Sam 20240301 : 新增 QA 模式
    {
        sprintf(runStartMode, "QA");
    }
    else
    {
        ShowMyMessage("SetStartMode command error1","SetStartMode 指令錯誤1");
    }

    if(ret==true)
    {
        if(sMultiContactCount.SubString(1,2)=="FT")
        {
            t.sprintf("SETTINGOK>SetStartMode_FT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="RT")
        {
            t.sprintf("SETTINGOK>SetStartMode_RT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="QA")    //Sam 20240301 : 新增 QA 模式
        {
            t.sprintf("SETTINGOK>SetStartMode_QA>%s", runStartMode);
        }
        else
        {
            ShowMyMessage("SetStartMode command error2","SetStartMode 指令錯誤2");
        }
    }
    else
    {
        if(sMultiContactCount.SubString(1,2)=="FT")
        {
            t.sprintf("SETTINGNG>SetStartMode_FT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="RT")
        {
            t.sprintf("SETTINGNG>SetStartMode_RT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="QA")    //Sam 20240301 : 新增 QA 模式
        {
            t.sprintf("SETTINGNG>SetStartMode_QA>%s", runStartMode);
        }
        else
        {
            ShowMyMessage("SetStartMode command error3","SetStartMode 指令錯誤3");
        }
    }
    SendMSG_CMD(MSG_CMD_SETSTARTMODE, t);
}
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");
//---------------------------------------------------------------------------
bool TfMain::ChangeHandlerStartMode(char *str)                                  //KaiChen 20180910 ：Add GPIB SetStartMode_
{
    static bool bBackupState=false;
    if(SystemStart)
    {
        return false;
    }

    if(LastSet.iRealDummy!=REALLY)
    {
        ShowMyMessage("Please Check Real/Dummy Mode!!!");
        return false;
    }

    if(LastSet.iTester==OFF_LINE)
    {
        ShowMyMessage("Please Check Tester Mode!!!");
        return false;
    }

    AnsiString sMultiContactCount=AnsiString(str).Trim();

    if(sMultiContactCount.SubString(1,2)=="FT")
    {
        if(LastSet.iRunStartMode==rsmContinuRetest)
        {
            return false;
        }
        // GATE(FW3-WD) golden :11302-11305 -- see GATE REGISTER item 12 above
        // and the RISK NOTE at the top of this wave's banner.
#if 0
        bBackupState=cbRunStartMode->Enabled;
        cbRunStartMode->Enabled=true;                                           //Sam 20210715 : 切換模式不要卡權限
        fMain->DoFTRTClick(false, false);                                        //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        cbRunStartMode->Enabled=bBackupState;
#endif
    }
    else if(sMultiContactCount.SubString(1, 2)=="RT")
    {
        if(LastSet.iRunStartMode==rsmContinuStart)
        {
            return false;
        }
        // GATE(FW3-WD) golden :11313-11316 -- see GATE REGISTER item 12 above
        // and the RISK NOTE at the top of this wave's banner.
#if 0
        bBackupState=cbRunStartMode->Enabled;
        cbRunStartMode->Enabled=true;                                           //Sam 20210715 : 切換模式不要卡權限
        fMain->DoFTRTClick(true, false);                                    //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        cbRunStartMode->Enabled=bBackupState;
#endif
    }
    else if(sMultiContactCount.SubString(1, 2)=="QA")                           //Sam 20240301 : 新增 QA 模式
    {
        if(LastSet.iRunStartMode==rsmContinuStart || LastSet.iRunStartMode==rsmContinuRetest)
        {
            return false;
        }
        SetRunStartMode(rsmQAMode);
    }
    else
    {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void TfMain::CheckList()     //KaiChen 20190613 ：Add GPIB CHECKLIST?
{
    AnsiString ret="";
    AnsiString t;
    AnsiString sCheckListName;

    sCheckListName.sprintf("%s-OI%04d%02d%02d", fMain->cbSetupFileName->Text, SystemYear, SystemMonth, SystemDate);
    // GATE(FW3-WD) golden :11341/:11343/:11354 -- see GATE REGISTER item 13
    // above. `ret` keeps its declared default `""`, so the reply below
    // naturally resolves to "SETTINGNG" -- a faithful "did not happen"
    // outcome, not a fabricated success.
#if 0
    fLotInfo->GenerateCheckList(false, sCheckListName, true);

    ret=fLotInfo->GenerateCheckList(false, fMain->cbSetupFileName->Text, false);
#endif

    if(ret=="OK")
    {
        t.sprintf("SETTINGOK");
    }
    else
    {
        t.sprintf("SETTINGNG");
    }

#if 0
    fLotInfo->RefreshYieldMonitor();
#endif

    SendMSG_CMD(MSG_CMD_CHECKLIST, t);
}
//---------------------------------------------------------------------------
void TfMain::SetBinPosChange()                                                  //KaiChen 20190706 ：Add GPIB BINPOS_
{
    AnsiString asRet;
    char str[256];
    AnsiString t;
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
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_BINPOS, t);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asRet=BinPosChange(str);

        if(asRet=="")
        {
            t.sprintf("OK");
        }
        else if(asRet=="NG")
        {
            t.sprintf("NG");
        }
        else
        {
            t.sprintf("NG%s", asRet);
        }
        SendMSG_CMD(MSG_CMD_BINPOS, t);
    }
}
//---------------------------------------------------------------------------
AnsiString TfMain::BinPosChange(char *str) //KaiChen 20190706 ：Add GPIB BINPOS_
{
    AnsiString asBinPos=AnsiString(str).Trim();
    AnsiString asReturn="";
    AnsiString asBuf[16][2];
    int iBinData[16], iTrayData[16];
    int iPosEnd1=0, iPosEnd2=0;
    AnsiString asTemp1="";
    int iFailCurrent, iFailSet, iWhichTrayCurrent;

    RecordProcess(asBinPos);

    for(int i=0; i<16; i++)
    {
        iBinData[i]=0;
        iTrayData[i]=0;
        asBuf[i][0]="";
        asBuf[i][1]="";
    }

    for(int i=0; i<16; i++)
    {
        iPosEnd1=asBinPos.Pos(",");
        asTemp1=asBinPos.SubString(1, iPosEnd1-1);

        iPosEnd2=asTemp1.Pos("_");
        asBuf[i][0]=asTemp1.SubString(1, iPosEnd2-1);
        asBuf[i][1]=asTemp1.SubString(iPosEnd2+1, asTemp1.Length());

        iBinData[i]=atoi(asBuf[i][0].c_str());
//        iTrayData[i]=atoi(asBuf[i][1].c_str());
        if(asBuf[i][1]=="N")    //KaiChen 20191227 ：修改可以將 Bin 設定到 NoUse
        {
            iTrayData[i]=0;
        }
        else
        {
            iTrayData[i]=atoi(asBuf[i][1].c_str());
        }

        asBinPos=asBinPos.SubString(iPosEnd1+1, asBinPos.Length());

        if(iPosEnd1==0 || iPosEnd2==0)
        {
            return "NG";
        }

        if(asBinPos.Length()<=0)
        {
            break;
        }
    }

    for(int i=0; i<16; i++)
    {
        if(iBinData[i]<0 || iBinData[i]>15)
        {
            return "NG";
        }
        //if(iTrayData[i]<0 || iTrayData[i]>6)
        if(iTrayData[i]<0)  //KaiChen 20191227 ：增加回傳異常碼
        {
            return "NG";
        }
    }

    for(int i=0; i<16; i++)
    {
        if(iBinData[i]>0)
        {
            iWhichTrayCurrent   =BinSelect[iTestRunMode].iCatDataT3Pos[iBinData[i]];       //QQQ
            iFailCurrent        =BinSelect[iTestRunMode].iStackDefFailCate[iWhichTrayCurrent-1];
            iFailSet            =BinSelect[iTestRunMode].iStackDefFailCate[iTrayData[i]-1];

            if(iWhichTrayCurrent>0)
            {
                if(iFailCurrent==0 && iFailSet==0)
                {
                }
                else if(iFailCurrent==1 && iFailSet==1)
                {
                }
                else if(iFailCurrent==0 && iFailSet==1 && iTrayData[i]-1!=BinSelect[iTestRunMode].IfErrorT3)  //Good bin -> Fail bin
                {
                    asReturn=asReturn+"_A1";
                }
                else if(iFailCurrent==0 && iFailSet==1 && iTrayData[i]-1==BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_A2";
                }
                else if(iFailCurrent==1 && iFailSet==0 && iWhichTrayCurrent-1!=BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_B1";
                }
                else if(iFailCurrent==1 && iFailSet==0 && iWhichTrayCurrent-1==BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_B2";
                }
                else
                {
                    return "NG";
                }
            }
            else
            {
                if(iTrayData[i]>6)  //KaiChen 20191227 ：增加回傳異常碼
                {
                    asReturn=asReturn+"_D1";
                }
                else
                {
                    asReturn=asReturn+"_C1";
                }
            }
        }
        else
        {
            break;
        }
    }

    if(asReturn=="")
    {
        for(int i=0; i<16; i++)
        {
            if(iBinData[i]>0)
            {
                BinSelect[iTestRunMode].iCatDataT3Pos[iBinData[i]]=iTrayData[i];   //QQQ
            }
            else
            {
                break;
            }
        }

        // GATE(FW3-WD) golden :11535-11543 -- see GATE REGISTER items 14-15
        // above and the RISK NOTE at the top of this wave's banner.
#if 0
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            if(i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count)
            {
                fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=BinSelect[iTestRunMode].iCatDataT3Pos[i];
            }
        }
        fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
        fBinSel->spbSaveClick(this);
        fShowBinSelect->ShowBinSel();
#endif
    }

    return asReturn;
}
//---------------------------------------------------------------------------
void TfMain::GetSGFTPSTATUS()                   //Sam 20210329 : Add GPIB SGFTP_STATUS
{
    AnsiString asRet;
    if(IniConfig.bA32EnableFTPAutomation==true)
        asRet.sprintf("ON");
    else
        asRet.sprintf("OFF");
    SendMSG_CMD(MSG_CMD_GetSGFTP_STATUS, asRet);
}
//---------------------------------------------------------------------------
void TfMain::SetSGFTP()                                                         //Sam 20210329 : Add GPIB SGFTP_ SGFTP_ON/SGFTP_OFF
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
        SendMSG_CMD(MSG_CMD_SetSGFTP, asRet);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        sPath=AuthPath+"config.ini";

        if(asCmd.Pos("ON")==1)
        {
            // GATE(FW3-WD) golden :11588-11589 -- see GATE REGISTER items
            // 16-17 above (config\ shared config write; fConfiguration
            // absence).
#if 0
            WriteIniData(sPath, "Function", "bA32EnableFTPAutomation", 1);      // shared machine config write -- queued for redirect-seam design
            fConfiguration->cbA32->Checked=true;
#endif
            IniConfig.bA32EnableFTPAutomation=true;
            asRet.sprintf("OK");
        }
        else if(asCmd.Pos("OFF")==1)
        {
            // GATE(FW3-WD) golden :11595/:11597 -- see GATE REGISTER items
            // 16-17 above (config\ shared config write; fConfiguration
            // absence).
#if 0
            WriteIniData(sPath, "Function", "bA32EnableFTPAutomation", 0);      // shared machine config write -- queued for redirect-seam design
#endif
            IniConfig.bA32EnableFTPAutomation=false;
#if 0
            fConfiguration->cbA32->Checked=false;
#endif
            asRet.sprintf("OK");
        }
        else
        {
            asRet.sprintf("NG");
        }

        // GATE(FW3-WD) golden :11605 -- see GATE REGISTER item 18 above.
#if 0
        fLotInfo->RefreshYieldMonitor();
#endif

        SendMSG_CMD(MSG_CMD_SetSGFTP, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetNONDOUBLEBIN()                                                  //Sam 20210329 : Add GPIB NONDOUBLEBIN_
{
    AnsiString asCmd,str1,asRet,asBin;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    TStringList *tNonDoubleBin=new TStringList();
    bool bflag[256]={false};
    int i,iBin;
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
        SendMSG_CMD(MSG_CMD_SetNONDOUBLEBIN, asRet);
    }
    else
    {
        for(i=0;i<256;i++)
        {
            bflag[i]=false;
        }

        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);
        tNonDoubleBin->CommaText=str1;

        asRet="OK";

        if(SystemStart==true)   //系統在 Run 的時候不給改
        {
            asRet.sprintf("NG");
        }
        else if(tNonDoubleBin->Count==1)
        {
            asBin=tNonDoubleBin->Strings[0];
            asCmd="NULL";
        }
        else if(tNonDoubleBin->Count>1)
        {
            for(i=0;i<tNonDoubleBin->Count;i++)
            {
                if(tNonDoubleBin->Strings[i]!="")
                {
                    // AI(W906-FW3-WD) 20260818: substitution (S6) -- see
                    // FW3-WD GROUP banner above for the full citation.
                    iBin=atoi(AnsiString(tNonDoubleBin->Strings[i]).c_str());
                    //檢查 Bin 是否有重複設定
                    if(bflag[iBin]==false)
                        bflag[iBin]=true;
                    else
                        asRet="NG";
                }
                else
                {
                    asRet="NG";
                }
            }
        }
        else
        {
            asRet="NG";
        }

        if(asRet=="OK")
            WriteIniData(sCheckListFilePath, "Tester_Control", "NONDOUBLEBIN", asCmd);
        else
            WriteIniData(sCheckListFilePath, "Tester_Control", "NONDOUBLEBIN", "");//有問題要把資料清空

        SendMSG_CMD(MSG_CMD_SetNONDOUBLEBIN, asRet);
    }
    tNonDoubleBin->Clear();
    delete tNonDoubleBin;
}
//---------------------------------------------------------------------------
void TfMain::SetSBinData()                     //Steven 20220421 : Amlogic需要收SBIN
{
    AnsiString asCmd, str2, asSBin;
    int iSBin;
    char str[544];
    strncpy(str, HGpib2Handler->cReturn, HGpib2Handler->GPIBBin);
    asCmd=AnsiString(str).Trim();  //asCmd="30_1|P|20_2|F|3";
    asCmd=asCmd.SubString(0, asCmd.AnsiPos(";")-1);
    asCmd=asCmd.SubString(9, asCmd.Length());
    TStringList *tSBin=new TStringList();
    tSBin->CommaText=asCmd;
    if(tSBin->Count==32)
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
                {
                    asSBin=tSBin->Strings[32-TestIF.iSiteMap[i][j]]; //要反序,所以32-
                    iSBin=HexStrToInt(asSBin);
                    TestSocket.cSBin[i][j]=iSBin;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetBINCOUNT()                                                      //Sam 20210329 : Add GPIB BINCOUNT_
{
    AnsiString asRet,asCmd,str1,asCondition,str2,asCategory,asBin,asPassFail;
    char str[256];
    AnsiString sPath,sCheckListFilePath,sLastFilePath,asLastFileName;
    TStringList *tBinCount=new TStringList();
    TStringList *tCondition=new TStringList();

    bool bflag[TEST_MAX_BIN]={false};
    int i,j,iBin,iCount,iFailCnt;
    bool bCondition;
    bool bStatusOK=false;

    for(i=0; i<TEST_MAX_BIN; i++)
        bflag[i]=false;

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
        SendMSG_CMD(MSG_CMD_SetBINCOUNT, asRet);
    }
    else
    {
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);     //CheckList檔案
        sLastFilePath=DataPath+asLastFileName+"\\Tester.Data";              //SetupFile

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();  //asCmd="30_1|P|20_2|F|3";

        str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);
        tBinCount->CommaText=str1;

        asRet="OK";

        if(asCmd=="NULL")
        {
            for(j=0; j<15; j++)
            {
                asBin.sprintf("IsPassBin%d", j+1);
                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, AnsiString("NULL")); //將所有 Bin Pass/Fail 設為 NULL
                Prod.iPersentIgnore[j]=0;
            }
        }
        else if(tBinCount->Count>1)
        {
            for(j=0; j<15; j++)
            {
                asBin.sprintf("IsPassBin%d", j+1);
                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, AnsiString("NULL")); //將所有 Bin Pass/Fail 設為 NULL
            }
            for(i=0;i<tBinCount->Count;i++)
            {
                if(tBinCount->Strings[i]!="")
                {
                    if(i==0)
                    {
                        // AI(W906-FW3-WD) 20260818: substitution (S6) -- see
                        // FW3-WD GROUP banner above for the full citation.
                        iCount=atoi(AnsiString(tBinCount->Strings[i]).c_str());//LowYield(By Total) after contact count & By Site Compare after contact count
                        if(iCount>1)
                        {
                            WriteIniData(sCheckListFilePath, "FT_Yield", "Preset", iCount);                    //寫入 Check List
                            sLastFilePath=DataPath+asLastFileName+"\\Tester.Data";
                            // GATE(FW3-WD) golden :11800-11809 -- see GATE
                            // REGISTER item 19 above (IniData\ shared config
                            // write). The in-memory
                            // TestIF_File.iLowYieldCountByTotal /
                            // .iFailAlarmSiteYieldCmpCount /
                            // TestIF.iLowYieldCountByTotal_RT /
                            // TestIF_File.iFailAlarmSiteYieldCmpCount_RT
                            // assignments stay ACTIVE.
                            if(iTestRunMode==FT)
                            {
#if 0
                                WriteIniData(sLastFilePath, "Site Yield Alarm", "Site Yield Compare Count",iCount);     // shared machine config write -- queued for redirect-seam design
#endif
                                TestIF_File.iLowYieldCountByTotal=iCount;
#if 0
                                WriteIniData(sLastFilePath, "Low Yield Alarm", "By Total Count",iCount);                // shared machine config write -- queued for redirect-seam design
#endif
                                TestIF_File.iFailAlarmSiteYieldCmpCount=iCount;
                            }
                            else if(iTestRunMode==RT)
                            {
#if 0
                                WriteIniData(sLastFilePath, "Site Yield Alarm", "Site Yield Compare Count RT",iCount);  // shared machine config write -- queued for redirect-seam design
#endif
                                TestIF.iLowYieldCountByTotal_RT=iCount;
#if 0
                                WriteIniData(sLastFilePath, "Low Yield Alarm", "By Total Count RT",iCount);             // shared machine config write -- queued for redirect-seam design
#endif
                                TestIF_File.iFailAlarmSiteYieldCmpCount_RT=iCount;
                            }
                        }
                        else
                        {
                            asRet="NG";
                        }
                    }
                    else
                    {
                        // AI(W906-FW3-WD) 20260818: substitution (S6) -- see
                        // FW3-WD GROUP banner above for the full citation.
                        asCondition=AnsiString(tBinCount->Strings[i]).c_str();//1|P|Y Bin1 Pass, Yield Ignore Cnt 數量為 Y
                        str2=StringReplace(asCondition,"|", ",", TReplaceFlags()<<rfReplaceAll);
                        tCondition->CommaText=str2;
                        if(tCondition->Count==3)
                        {
                            bCondition=true;
                            // AI(W906-FW3-WD) 20260818: substitution (S6) --
                            // see FW3-WD GROUP banner above for the full
                            // citation.
                            iBin=atoi(AnsiString(tCondition->Strings[0]).c_str());
                            asPassFail=tCondition->Strings[1];
                            iFailCnt=atoi(AnsiString(tCondition->Strings[2]).c_str());

                            //檢查 Bin 是否有重複設定
                            if(bflag[iBin]==false)
                                bflag[iBin]=true;
                            else
                                bCondition=false;

                            if(bCondition==true) //Condition 資料都正確才寫入
                            {
                                asCategory.sprintf("Category%d",iBin);
                                WriteIniData(sCheckListFilePath, asCategory, "Fail Percent Ignore", iFailCnt);                    //寫入 Check List

                                asBin.sprintf("IsPassBin%d",iBin);                                                                //寫入 Check List Pass/Fail
                                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, asPassFail);   //P:Pass , F:Fail

                                Prod.iPersentIgnore[iBin]=iFailCnt;//先放到 Prod 等等再一起寫入 速度筆調快
                            }
                            else
                            {
                                asRet="NG";
                            }
                        }
                        else
                        {
                            asRet="NG";
                        }
                    }
                }
                else
                {
                    asRet="NG";
                }
            }
        }
        else
        {
            asRet="NG";
        }

        if(asRet=="OK")
        {
            for(i=0; i<TEST_MAX_BIN; i++)
            {
                BinSelect[iTestRunMode].iPersentIgnore[i]=Prod.iPersentIgnore[i];
            }
            bSetBINCOUNT=true;
            // GATE(FW3-WD) golden :11875-11876 -- see GATE REGISTER item 20
            // above and the RISK NOTE at the top of this wave's banner.
#if 0
            fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
            fBinSel->spbSaveClick(this);                                                 //寫入 FT SetupFile
#endif
            bSetBINCOUNT=false;

            WriteIniData(sCheckListFilePath, "Tester_Control", "BINCOUNT", asCmd);
        }
        else
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "BINCOUNT", "");     //有問題要把資料清空
        }
        SendMSG_CMD(MSG_CMD_SetBINCOUNT, asRet);
    }
    tBinCount->Clear();
    tCondition->Clear();
    delete tBinCount;
    delete tCondition;
}
//---------------------------------------------------------------------------
void TfMain::SetSGOSBIN()                                                       //Sam 20210406 : Add GPIB SGOSBIN_
{
    AnsiString asCmd,asRet;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    int iBin;
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
        SendMSG_CMD(MSG_CMD_SetSGOSBIN, asRet);
    }
    else
    {
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        iBin=atoi(asCmd.c_str());

        asRet="OK";
        if(asCmd=="NULL")
        {
        }
        else if(iBin<=0)
        {
             asRet="NG";
        }

        if(asRet=="OK")
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "SGOSBIN", asCmd);
        }
        else
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "SGOSBIN", "");//有問題要把資料清空
        }

        // GATE(FW3-WD) golden :11943 -- see GATE REGISTER item 21 above.
#if 0
        fLotInfo->RefreshYieldMonitor();
#endif

        SendMSG_CMD(MSG_CMD_SetSGOSBIN, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSGCONTFAIL()                                                    //Sam 20210422 : Add GPIB SGCONTFAIL_
{
    AnsiString asCmd,asRet;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    int iFailCount;
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    int iOn=0;

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
        SendMSG_CMD(MSG_CMD_SetSGCONTFAIL, asRet);
    }
    else
    {
        szDir.sprintf("%s%s\\Tester.Data", DataPath, S);
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        iFailCount=atoi(asCmd.c_str());

        asRet="OK";
        if(asCmd=="NULL")
        {
        }
        else if(iFailCount<=0)
        {
            asRet="NG";
        }

        if(asRet=="OK")
        {
            if(iTestRunMode==FT)
            {
                if(iFailCount>0 || asCmd!="NULL")
                {
                    iOn=1;
                    TestIF_File.bContsFailBySocket=true;
                    TestIF_File.bContsFailByHead=true;
                    TestIF_File.iContsFailHeadAlarmCT=iFailCount;
                    TestIF_File.iContsFailSocketAlarmCT=iFailCount;
                    // GATE(FW3-WD) golden :12008-12011 -- see GATE REGISTER
                    // items 22-23 above (IniData\ shared config write;
                    // fYieldMonitoring absence).
#if 0
                    WriteIniData(szDir, "Alarm", "HeadCT",   iFailCount);       // shared machine config write -- queued for redirect-seam design
                    WriteIniData(szDir, "Alarm", "SocketCT", iFailCount);       // shared machine config write -- queued for redirect-seam design
                    fYieldMonitoring->edContsFailSocketAlarmCT_FT->Text=iFailCount;
                    fYieldMonitoring->edContsFailHeadAlarmCT_FT->Text=iFailCount;
#endif
                }
                else
                {
                    iOn=0;
                    TestIF_File.bContsFailBySocket=false;
                    TestIF_File.bContsFailByHead=false;
                }
                // GATE(FW3-WD) golden :12019-12024 -- see GATE REGISTER items
                // 22-23 above. GOLDEN BUG (B6): golden's own
                // `rbContsFailByHead_FTOff->Checked=TestIF_File.
                // bContsFailByHead;` (no `!`) is preserved verbatim inside
                // this (already-gated) block -- see the banner's B6 citation.
#if 0
                WriteIniData(szDir, "Alarm", "SocketEnable", iOn);              // shared machine config write -- queued for redirect-seam design
                WriteIniData(szDir, "Alarm", "HeadEnable",   iOn);              // shared machine config write -- queued for redirect-seam design
                fYieldMonitoring->rbContsFailBySocket_FTOn->Checked=TestIF_File.bContsFailBySocket;
                fYieldMonitoring->rbContsFailBySocket_FTOff->Checked=!TestIF_File.bContsFailBySocket;
                fYieldMonitoring->rbContsFailByHead_FTOn->Checked=TestIF_File.bContsFailByHead;
                fYieldMonitoring->rbContsFailByHead_FTOff->Checked=TestIF_File.bContsFailByHead;
#endif
            }
            else
            {
                if(iFailCount>0 || asCmd!="NULL")
                {
                    iOn=1;
                    TestIF_File.bContsFailBySocket_RT=true;
                    TestIF_File.bContsFailByHead_RT=true;
                    TestIF_File.iContsFailHeadAlarmCT_RT=iFailCount;
                    TestIF_File.iContsFailSocketAlarmCT_RT=iFailCount;
                    // GATE(FW3-WD) golden :12035-12038 -- see GATE REGISTER
                    // items 22-23 above (IniData\ shared config write;
                    // fYieldMonitoring absence).
#if 0
                    WriteIniData(szDir, "Alarm", "HeadCT RT",   iFailCount);    // shared machine config write -- queued for redirect-seam design
                    WriteIniData(szDir, "Alarm", "SocketCT RT", iFailCount);    // shared machine config write -- queued for redirect-seam design
                    fYieldMonitoring->edContsFailSocketAlarmCT_RT->Text=iFailCount;
                    fYieldMonitoring->edContsFailHeadAlarmCT_RT->Text=iFailCount;
#endif
                }
                else
                {
                    iOn=0;
                    TestIF_File.bContsFailBySocket_RT=false;
                    TestIF_File.bContsFailByHead_RT=false;
                }
                // GATE(FW3-WD) golden :12046-12051 -- see GATE REGISTER items
                // 22-23 above. GOLDEN BUG (B6): golden's own
                // `rbContsFailByHead_RTOff->Checked=TestIF_File.
                // bContsFailByHead_RT;` (no `!`) is preserved verbatim inside
                // this (already-gated) block -- see the banner's B6 citation.
#if 0
                WriteIniData(szDir, "Alarm", "SocketEnable RT", iOn);           // shared machine config write -- queued for redirect-seam design
                WriteIniData(szDir, "Alarm", "HeadEnable RT",   iOn);           // shared machine config write -- queued for redirect-seam design
                fYieldMonitoring->rbContsFailBySocket_RTOn->Checked=TestIF_File.bContsFailBySocket_RT;
                fYieldMonitoring->rbContsFailBySocket_RTOff->Checked=!TestIF_File.bContsFailBySocket_RT;
                fYieldMonitoring->rbContsFailByHead_RTOn->Checked=TestIF_File.bContsFailByHead_RT;
                fYieldMonitoring->rbContsFailByHead_RTOff->Checked=TestIF_File.bContsFailByHead_RT;
#endif
            }
            WriteIniData(sCheckListFilePath, "Alarm", "Socket", iFailCount);
            WriteIniData(sCheckListFilePath, "Alarm", "Head",   iFailCount);

            // GATE(FW3-WD) golden :12056 -- see GATE REGISTER item 24 above.
#if 0
            fLotInfo->RefreshYieldMonitor();
#endif
        }

        SendMSG_CMD(MSG_CMD_SetSGCONTFAIL, asRet);
    }
}

// -- FW3-WD APPEND -- end (SIGURD/GPIB status-string family, golden Command.cpp :9995-12061) --

// AI(W906-FW3-WE) 20260818: additional #include for the FW3-WE GROUP below.
// `TObject`/`TCustomWinSocket`/`TErrorEvent` (region 1's socket-event
// signatures) and `String` (the BCB6 AnsiString alias, region 1's
// send-process parameter type) are ALREADY unqualified-visible through this
// file's existing `#include "forms/fMain.h"` (top of file): forms/fMain.h
// includes forms/FormWidgets.h, which includes vclcompat/vcl_compat.h
// (FormWidgets.h:82), which itself includes vclcompat/ServerSocket.h
// (vcl_compat.h:240) -- and THAT header includes vclcompat/ClientSocket.h
// (ServerSocket.h:210), defines TCustomWinSocket/TErrorEvent inside
// `namespace Scktcomp`, and re-exports them via `using namespace Scktcomp;`
// (ServerSocket.h:332, guarded `#ifndef VCLCOMPAT_NO_GLOBAL_USING` -- that
// macro is never `#define`d anywhere in this tree, `grep -rn "#define
// VCLCOMPAT_NO_GLOBAL_USING" .` -- 0 hits, so the guard is always open).
// `TObject` itself is re-exported the same way (vcl_compat.h:261 `using
// vclcompat::TObject;`) and `String` via vcl_compat.h:340 `typedef
// vclcompat::AnsiString String;`. `fLotInfo` (SetProdModeByDll's ACTIVE
// `fLotInfo->SetLotStart(__FUNC__);` arm) is NOT already visible -- unlike
// every FW3-WD `fLotInfo->` touch, which lived inside a GATED `#if 0` block,
// this is the first ACTIVE `fLotInfo` use in this file, so forms/fLotInfo.h
// is included below (a leaf header, only pulls in forms/FormWidgets.h,
// already present).
#include "forms/fLotInfo.h"           // fLotInfo (TfLotInfo*) -- SetProdModeByDll's ACTIVE SetLotStart() arm

// AI(W906-FW3-WE) 20260818: TU-local no-op shim for golden's
// LogClientSocketExceptionError(TObject*, AnsiString) (Public/
// WinSocketErrorCode.cpp:430, a REAL, defined function -- not merely
// declared). It is not called directly from here: that symbol lives in the
// ht9045_public library, and ht9045_sm (this file's own library) does not
// link it (CMakeLists.txt:2139-2150, ht9045_sm PUBLIC links ht9045_io/
// ht9045_motor/ht9045_globals/vclcompat/ht9045_secsgem/ht9045_forms -- no
// ht9045_public edge, direct or transitive per WinSocketErrorCode.cpp's own
// G-PTk4 gate commentary, which documents ht9045_public as a leaf nothing
// links). Calling it directly would be a NEW link edge, out of this wave's
// CMakeLists-untouched mandate, and would fail as an undefined reference at
// link time. Every other consumer in this tree hit the exact same wall and
// built its OWN minimal-dependency TU-local wrapper rather than add a link
// edge: EJ1N_LogClientSocketExceptionError (EJ1N/uSocketServerClient.cpp
// :556), W906Auto_LogClientSocketExceptionError (Automation/automation.cpp
// :88), Gated_LogClientSocketExceptionError (Interface/TesterTCP_Socket.cpp
// :117), LogClientSocketExceptionError_ (SECSGEM/uHGemEquipment.cpp:3212).
// This file's own wrapper follows the same shape -- per contract rule 4
// ("TU-local `static` no-op shim 可以"), a `static` free function, not a
// namespace-scope object with dynamic initialisation, so it carries no SIOF
// risk.
static void FW3WE_LogClientSocketExceptionError(TObject * /*Sender*/, const AnsiString & /*Context*/)
{
    // Offline: no EventLog "Exception" row is written (same best-effort-
    // diagnostics-only posture the 4 sibling wrappers above already
    // document -- nothing in TeraTCPResultServerClientError's control flow
    // below depends on this having a real body).
}

// =============================================================================
//  FW3-WE GROUP -- the TCP-command-server family + the ByDLL-tail/GPIB-2DID/
//                  FixAICCD/ATC-multizone command family
//
//  Translation wave: FW-3 Wave E
//  Translator: AI(W906-FW3-WE) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Command.cpp (15,273 lines, cp950)
//
//  ROLE
//  ----
//  35 golden TfMain:: methods across two golden byte ranges: REGION 1
//  (:12540-12761, the raw TCustomWinSocket OnConnect/OnDisconnect/OnError/
//  send-process family behind TCPCommandServer/TeraTCPResultServer) and
//  REGION 2 (:14302-15273, the tail of Command.cpp: ProdMode-by-DLL,
//  READYNEXTSHOT/NEXT2DID GPIB 2DID polling, SETAICCD/GETAICCD FixAICCD,
//  SETOSBIN/GETOSBIN/GETDUTCHK/GETFFC/GetTJFunction/GetPowerFollowing,
//  head-contact-count reporting, FTP/direct setup-file-change GPIB commands,
//  socket/TIM counters, and the ATC multi-zone-temp/water-valve/dynamic-PID
//  tail). golden :12762-14301 (TCPCommandServerClientRead, the overflow-
//  defect dispatcher) is EXCLUDED per this wave's own never-wave list --
//  not declared, not translated, no stand-in, no call site anywhere in this
//  tree. Per the task brief: bodies are translated, NO caller (TCP dispatch
//  table / event wiring / GPIB dispatch) is wired up -- every one of these 35
//  methods is presently unreachable from any translated call site, exactly
//  like FW3-WA/WB/WC/WD before it.
//
//  WAVE SCOPE (every golden method, golden line span, ACTIVE or GATED-partial)
//  ------------------------------------------------------------------------------
//  REGION 1 (golden :12540-12761, 9 methods):
//    TCPCommandServerClientConnect       :12540-12548 ACTIVE
//    TCPCommandServerClientDisconnect    :12550-12557 ACTIVE
//    TeraTCPResultServerClientConnect    :12559-12567 ACTIVE
//    TeraTCPResultServerClientDisconnect :12569-12577 ACTIVE
//    TeraTCPResultServerClientError      :12579-12608 GATED-partial (TeraTCPResultServer, GATE REGISTER 1)
//    TCPIPCommunicationLog               :12610-12621 GATED-partial (Memo2, GATE REGISTER 2) (1 substitution, S9)
//    HanderTcpIp                         :12623-12655 GATED-partial (TCPCommandServer/TeraTCPResultServer, GATE REGISTER 3-4) (1 GOLDEN BUG, B7)
//    HandlerTCPIPResultSendProcess       :12657-12723 GATED-partial (TCPCommandServer, whole body, GATE REGISTER 5)
//    HandlerTeraTResultSendProcess       :12725-12760 GATED-partial (TeraTCPResultServer, whole body, GATE REGISTER 6)
//  REGION 2 (golden :14302-15273, 26 methods):
//    GetProdModeByDll        :14302-14317 ACTIVE
//    SetProdModeByDll        :14319-14356 GATED-partial (fLotInfo, GATE REGISTER 7-8) (1 substitution, S10)
//    WriteREADYNEXTSHOT      :14358-14361 ACTIVE
//    GetREADYNEXTSHOT        :14363-14427 GATED-partial (TMyKitSuck::HasNotTestYet, GATE REGISTER 9)
//    WriteNEXT2DID           :14429-14432 ACTIVE
//    GetNEXT2DID             :14434-14459 GATED-partial (TMyKitSuck::HasNotTestYet, GATE REGISTER 10-11)
//    Get2DID_OrderBySites    :14461-14485 ACTIVE
//    SetAICCD                :14487-14532 GATED-partial (fFixAICCD / IniData\ write, GATE REGISTER 12-13)
//    GetAICCD                :14534-14547 ACTIVE
//    IsStackHasLess16Bin     :14549-14565 ACTIVE
//    TransformTcGPIBData     :14567-14669 GATED-partial (ATC_InterfaceForm, GATE REGISTER 14)
//    SetOSBIN                :14671-14683 GATED-partial (fBinSel, GATE REGISTER 15)
//    GetOSBIN                :14685-14690 ACTIVE
//    GetDUTCHK               :14692-14730 ACTIVE
//    GetFFC                  :14732-14740 ACTIVE
//    GetTJFunction           :14742-14750 ACTIVE
//    GetPowerFollowing       :14752-14760 ACTIVE
//    WriteHeadContactCount   :14762-14976 ACTIVE (1 GOLDEN ODDITY, B8)
//    WriteSetSetupFile       :14979-15034 ACTIVE
//    WriteFTPDownSetupFile   :15036-15103 GATED-partial (fFTPClient, GATE REGISTER 16)
//    GetSocketCounter        :15105-15150 ACTIVE
//    GetTIMCounter           :15152-15212 ACTIVE
//    WriteMultiZoneTemp      :15214-15236 ACTIVE
//    WriteMultiZoneEnable    :15238-15249 ACTIVE
//    ReadWaterValve          :15251-15261 GATED-partial (ATC_InterfaceForm, GATE REGISTER 17)
//    ReadDynamicPID          :15263-15272 ACTIVE
//  TOTALS: 35 methods, ~1,194 raw golden lines extracted (region 1: 222
//  lines / region 2: 972 lines, cp950-clean-extracted line counts); 20
//  ACTIVE, 15 GATED-partial (2 of which -- HandlerTCPIPResultSendProcess /
//  HandlerTeraTResultSendProcess -- are gated across their WHOLE body, same
//  "GATED-partial ... whole body" labelling FW3-WA used for UPHStrings/
//  IndexCycleTimeStrings).
//
//  TYPE-SPELLING NOTE (not a gate, not a substitution -- confirmed compiling
//  as-is): golden's BCB6 `String sMessage` parameter (HandlerTCPIPResult
//  SendProcess/HandlerTeraTResultSendProcess) is kept VERBATIM as `String`,
//  not respelled to `AnsiString` -- vcl_compat.h:340 `typedef vclcompat::
//  AnsiString String;` is re-exported globally (see the file-scope note
//  above this banner), so `String` is directly usable with zero new
//  #include. (An earlier pass of this wave's own recon mis-grepped for
//  `typedef AnsiString String` and reported 0 hits -- the real spelling is
//  `typedef vclcompat::AnsiString String;`, which that pattern does not
//  match. Corrected before writing any code; flagged here per this wave's
//  own anti-absence-claim-rot instruction, since this actually happened
//  during this wave's own recon, not a hypothetical.)
//
//  STUB COLLISIONS: NONE. Re-verified this pass, 20260818 --
//  `grep -rn "TfMain::<name>\b" --include=*.cpp .` for all 35 names (run as
//  two combined-alternation Grep passes, region 1's 9 names and region 2's
//  26 names) returns ZERO hits anywhere in the tree (forms/fMain.cpp
//  included) other than THIS file's own new definitions below; a matching
//  pass over forms/fMain.h finds zero pre-existing DECLARATIONS either. This
//  wave's task brief flagged this exact risk (Wave D nearly mis-registered
//  Command.cpp as unregistered in CMakeLists; it has in fact been registered
//  since FW3-WA, CMakeLists.txt:1852) -- the collision-free result here is a
//  measurement, not an assumption: no stub retirement is needed for this
//  wave's 35 names.
//
//  SUBSTITUTIONS (continuing from FW3-WD's S1-S8)
//  ------------------------------------------------------------------------------
//  (S9)  TCPIPCommunicationLog golden :12614 `Now().FormatString("yyyy/mm/dd")`
//        -- vclcompat's TDateTime (vclcompat/TDateTime.h) has NO
//        `.FormatString` member (`grep -rn "FormatString" vclcompat/` -- 0
//        hits); it has the free function `AnsiString FormatDateTime(const
//        AnsiString&, const TDateTime&)` instead (same token set, same
//        semantics -- just a member-call vs. free-function spelling
//        difference between BCB6's convenience method and vclcompat's
//        SysUtils-style API). Substituted with `FormatDateTime("yyyy/mm/dd",
//        Now())` -- identical output.
//  (S10) SetProdModeByDll golden :14335 `fMain->CheckCanChangeRealDummy()` --
//        NOT a declared TfMain member anywhere in forms/fMain.h (`grep -rn
//        "CheckCanChangeRealDummy" forms/fMain.h` -- 0 hits), the SAME
//        absence FW3-WC's (S4) already established for GetHandlerStatusByDll
//        (see that GROUP banner, Command.cpp :8307-8313 and the call site at
//        :9869-9871). Reuses the SAME already-proven-in-this-file
//        replacement Wave C's S4 used, verbatim call shape:
//        `ComputeCanChangeRealDummy(MOT[MMPlate1].HasIC(),
//        MOT[MMPlate2].HasIC(), ShuttleHasIC(), IndexHasIC(),
//        InArmSuck.HasIC(), OutArmSuck.HasIC())` (MainCalcCore.h's own
//        documented "Portable replacement for TfMain::
//        CheckCanChangeRealDummy()"). NOT the alternative zero-arg global
//        free function `CheckCanChangeRealDummy()` (Automation/auto9045.h
//        :201, a SEPARATE, also-real stand-in this wave's own recon first
//        reached for -- corrected before landing, since it would have
//        needed a brand-new `#include "Automation/auto9045.h"` this file
//        does not otherwise need, where the MainCalcCore.h route is already
//        proven compiling in THIS exact file via Wave C's own call).
//
//  GOLDEN BUG / ODDITY (recorded, NOT fixed -- continuing from FW3-WD's B1-B6)
//  ------------------------------------------------------------------------------
//  (B7) HanderTcpIp golden :12625-12626:
//           if(TCPCommandServer->Active==true)
//               TCPCommandServer->Active==false;
//       The second line is a COMPARISON (`==`), not an assignment (`=`) --
//       golden's own idle-server-should-be-closed-first guard is a silent
//       no-op that discards its result. Preserved verbatim inside GATE
//       REGISTER item 3 below (TCPCommandServer itself is also absent from
//       this port, so the line is doubly inert here) -- flagged so a future
//       wave that DOES stand up a real TCPCommandServer member does not
//       inherit this typo unknowingly.
//  (B8) WriteHeadContactCount golden :14778-14781 (the :14690-14691 citation
//       this banner first carried pointed at GetOSBIN's closing brace --
//       corrected at integration 20260818) (`catch(...) {
//       asChangeSetupFileName=""; iSelect=-1; }`) reassigns
//       `asChangeSetupFileName` -- a GLOBAL (cmydef.h:5729) that has nothing
//       to do with head-contact-count reporting; every sibling GPIB command
//       in this same file (SetTesterID, WriteSetSetupFile,
//       WriteFTPDownSetupFile, ...) uses that exact global for the actual
//       "which setup file did GPIB just send" purpose. Reads as a
//       copy-paste artefact (this function's catch block was probably
//       cloned from one of those setup-file functions and the "reset the
//       parsed command" line was never re-targeted to a
//       WriteHeadContactCount-local variable). Translated verbatim -- it is
//       harmless here (nothing in THIS function reads `asChangeSetupFileName`
//       afterwards) but DOES silently blank out whatever the setup-file-
//       change functions were tracking if a WriteHeadContactCount parse
//       exception fires between them and their own next read of it. Not
//       fixed per contract (behavior change needs a user decision).
//
//  GATE REGISTER (17 #if 0 / dropped-term sites; each states WHY the gate is
//  correct, not just "not found" -- per this wave's own anti-absence-claim-rot
//  instruction. Every grep below was re-run 20260818, from this tree's root.)
//  ------------------------------------------------------------------------------
//   1. TeraTCPResultServerClientError, golden :12602 `TeraTCPResultServer->
//      Close();` -- `TeraTCPResultServer` (golden main.h:121 `TServerSocket
//      *TeraTCPResultServer;`) is NOT a member of forms/fMain.h's TfMain
//      (`grep -rn "TeraTCPResultServer" --include=*.h --include=*.cpp .` --
//      0 hits anywhere in the port tree other than this wave's own
//      comments). vclcompat DOES now carry a real TServerSocket
//      (vclcompat/ServerSocket.h, the FW-WB precedent this wave's brief
//      pointed at) -- the TYPE exists, the MEMBER does not, and this wave's
//      contract explicitly forbids adding socket object members on its own
//      authority ("不要自己加成員"). The `try` body is therefore empty at
//      runtime; the `catch` (ACTIVE, calls the TU-local shim above) never
//      fires offline.
//   2. TCPIPCommunicationLog, golden :12615 `fMain->Memo2->Lines->Add(s);` --
//      `Memo2` (golden main.h:499 `TMemo *Memo2;`) is NOT a member of
//      forms/fMain.h's TfMain (`grep -n "Memo2" forms/fMain.h` -- 0 hits).
//      Real `vclcompat::TMemo` Memo2 members DO exist tree-wide, but only on
//      OTHER forms' facades (fLotInfo, fOCR, fTemp_Set, fTeach, fBarCode,
//      fObserver, fRFID -- e.g. OCRInsp.cpp:333/367) -- none of them is
//      fMain's own TCP-log memo. Everything else in this function
//      (GetTimeInfo/SystemHour.../the two MyForceDirectories+WriteDataToFile
//      writes under asTCPIPPath, `D:\HT9045_Log\TCPIP_Log`) stays ACTIVE --
//      that path is a LOG directory under D:\HT9045_Log, explicitly exempt
//      from the IniData\/config\/system\ write gate per this wave's own
//      instructions.
//   3. HanderTcpIp, golden :12625-12626 (see GOLDEN BUG B7 above) --
//      `TCPCommandServer` (golden main.h:122 `TServerSocket
//      *TCPCommandServer;`) absent from forms/fMain.h, same class of absence
//      as item 1 (`grep -rn "TCPCommandServer" --include=*.h --include=*.cpp
//      .` -- 0 hits outside this wave's own comments).
//   4. HanderTcpIp, golden :12635-12646 (the whole `if(CosFunction.
//      bEnableHandlerResultServer==true){...}` body: both servers' `->Close()
//      /->Port=.../->Open()` and the two "Server Listen"
//      TCPIPCommunicationLog lines) -- every statement inside dereferences
//      `fMain->TCPCommandServer` or `fMain->TeraTCPResultServer` (items
//      1/3); logging "[nnnn] Server Listen" when no listen ever happened
//      would be actively misleading, so the whole block -- not just the
//      member-touching lines -- is gated as one unit.
//      `iHandlerCommandServerPort=7016;`/`iHandlerResultServerPort=7017;`
//      (just above, real globals, cmydef.h:4690-4691) and the try/catch
//      shell (including the ACTIVE catch body) stay outside this gate.
//   5. HandlerTCPIPResultSendProcess, golden :12657-12723, WHOLE BODY -- its
//      very first executable line (`int iPort=fMain->TCPCommandServer->
//      Socket->LocalPort;`) already dereferences the absent member (item 3);
//      every later line (`iCount`, the `ReSendData:` retry loop, both
//      `Connections[i]->` reads) is downstream of that same absent
//      `->Socket`. Same "GATED-partial ... whole body" shape FW3-WA already
//      used for UPHStrings/IndexCycleTimeStrings (fShowBinSelect/fObserver
//      absence).
//   6. HandlerTeraTResultSendProcess, golden :12725-12760, WHOLE BODY -- same
//      reasoning as item 5, against `fMain->TeraTCPResultServer->Socket`
//      (item 1).
//   7. SetProdModeByDll, golden :14339 `fLotInfo->SetLotEnd(__FUNC__);` --
//      forms/fLotInfo.h declares `SetLotStart(AnsiString,bool=false)` (:48)
//      but NO `SetLotEnd` (`grep -n "SetLotEnd" forms/fLotInfo.h` -- 0
//      hits). The sibling `fLotInfo->SetLotStart(__FUNC__);` in this SAME
//      function's other branch (golden :14352) IS real and stays ACTIVE.
//   8. SetProdModeByDll, golden :14340 `fLotInfo->edtBarcodeRecipe->Enabled=
//      false;` -- `edtBarcodeRecipe` is not a member of forms/fLotInfo.h's
//      TfLotInfo (`grep -n "edtBarcodeRecipe" forms/fLotInfo.h` -- 0 hits).
//      The `return 0;` immediately following both item-7/item-8 lines stays
//      ACTIVE (golden's own "Sucess" return is unconditional here regardless
//      of what SetLotEnd/edtBarcodeRecipe do).
//   9. GetREADYNEXTSHOT, golden :14384 `FTestSuck.HasNotTestYet()`, :14394
//      `BTestSuck.HasNotTestYet()`, :14414 `FTestSuck.HasNotTestYet()`, :14415
//      `BTestSuck.HasNotTestYet()` (4 occurrences, all inside `||` clauses)
//      -- `HasNotTestYet()` is NOT a member of the TMyKitSuck this file uses
//      (aHotPlateSubstrate.h:365 -- `grep -n "HasNotTestYet"
//      aHotPlateSubstrate.h` -- 0 hits); it exists only on the OTHER
//      same-named-but-different-layout class (mykitsuck.h:404), the
//      established ODR gotcha (KNOWLEDGE.md). SAME precedent already on
//      record in THIS tree: aTester_Rear.cpp:9495-9496 gates the identical
//      golden idiom (`BTestSuck.HasNotTestYet() ||`) under its own GATE G15,
//      dropping that one OR-term and keeping the rest of the boolean
//      expression ACTIVE. All 4 occurrences here get the same treatment: the
//      `HasNotTestYet() ||` term is dropped, the remaining OR-operand
//      (`FLCarryKit.Item[i][j]!=NULL_IC && ...` / `... FLCarryKit.
//      HasRealIC()` / `BLCarryKit.HasRealIC()`) stays ACTIVE. golden
//      :14411-14413's `MOT[MMTrayY].HasIC() || MOT[MMPlate1].HasIC() ||
//      MOT[MMPlate2].HasIC() ||` sibling OR-terms in the SAME expression are
//      NOT gated -- MOT[]/MMTrayY/MMPlate1/MMPlate2 are all real
//      (cmydef.h:2255-2258; Motor/mymotor.h:385 `extern class TTrayMotor
//      MOT[MAX_TRAY_MOTOR];`; already used ACTIVE in this exact file by
//      FW3-WC's ComputeCanChangeRealDummy() call site, :9869) -- an earlier
//      pass of this wave's own recon mis-grepped for `TMyMotor MOT\[` /
//      `HTMotor \*MOT\[` (the WRONG class name) and wrongly reported them
//      absent; corrected before landing, flagged here per this wave's own
//      anti-absence-claim-rot instruction, since -- like the `String` typedef
//      miss noted in the TYPE-SPELLING NOTE above -- this also actually
//      happened during this wave's own recon, not a hypothetical. DELTA (the
//      real, surviving one): a device that is still physically under an
//      untested test head no longer alone triggers "ready"/"ongoing" -- only
//      a device already sitting in the carry-kit grid, or one of the three
//      real MOT[]-tray/plate HasIC() checks, does. Compare
//      aTester_Front.cpp:9568-9569, which appears to leave the SAME
//      HasNotTestYet() term un-gated (worth the next wave cross-checking
//      whether that call site actually compiles against
//      aHotPlateSubstrate.h's TMyKitSuck or a differently-typed local).
//  10. GetNEXT2DID, golden :14442-14447, the WHOLE
//      `if(FTestSuck.HasNotTestYet()) Get2DID_OrderBySites(&FTestSuck,...);
//       else Get2DID_OrderBySites(&FLCarryKit,...);` selector -- same
//      HasNotTestYet() absence as item 9, but here the predicate is the
//      ENTIRE if-condition (not one term of a multi-term `||`), so there is
//      no "drop the term, keep the rest" move available the way item 9 had.
//      Rather than silently hardcoding a guess at which of the two kits
//      golden would have picked (a real behavior decision, not a mechanical
//      gate), BOTH calls are gated together. `tNEXTBarCodeList` therefore
//      keeps its initial all-"0" fill (golden :14436-14439, ACTIVE,
//      unchanged) for the sht1 branch -- a defined, honest "no 2DID data"
//      output rather than an invented one.
//  11. GetNEXT2DID, golden :14449-14454, the sht2-branch mirror of item 10
//      (`BTestSuck.HasNotTestYet()` selecting BTestSuck vs. BLCarryKit) --
//      same reasoning.
//  12. SetAICCD, golden :14513-14518 (`fFixAICCD->chkEnableFix2AICCD->
//      Checked=...` through `->edtOutArmCycleInsp->Text=...`, 6 lines) --
//      forms/fFixAICCD.h's TfFixAICCD carries exactly two methods
//      (OutArmCycleCounterUpdate(), bCheckUnloaderHasAiNG(int)) and NO
//      widget members (`grep -n "chkEnableFix2AICCD\|rgResultShowType\|
//      edtlFix2AICCDStartDelay\|edtlFix2AICCDExposureTimeOut\|
//      edtInspectResultThres\|edtOutArmCycleInsp" forms/fFixAICCD.h` -- 0
//      hits). The REAL data this mirrors -- `TestIF_File.bEnableFix2BGAAICCD`
//      / `.iResultShowType` / `.iFix2BGAAICCDStartDelay` / `.
//      iFix2BGAAICCDExposureTimeOut` / `.iFix2BGAAICCDAutoRetry` / `.
//      dInspectResultThres` / `.iFix2BGAAICCDGetResultTimeOut` / `.
//      iFix2BGAAICCDOutArmCycleInsp` (all confirmed real fields, cprod.h
//      :2297-2308, part of the SYSTEM_TEST_IF struct backing both `TestIF`
//      and `TestIF_File`) -- is set by the ACTIVE lines directly above this
//      gate and IS what GetAICCD (below) reads back; only the on-screen
//      mirror is missing, same "form absent, real data present" shape as
//      FW3-WD's SetSGCONTFAIL (fYieldMonitoring, GATE REGISTER 22-23).
//  13. SetAICCD, golden :14520-14525 (`WriteIniData(szDir, "Configuration",
//      "Fix2 AI ...", ...)`, 6 calls) -- `szDir` is built as `DataPath +
//      GetLastOpenFN() + "\HandlerCondition.Data"`, and `DataPath`
//      (common.cpp:104) is `D:\HT9045\IniData\Data\` -- squarely the
//      IniData\ shared machine-config write surface this wave's
//      instructions name as gate scope (same treatment FW3-WD gave every
//      `WriteIniData(szDir,...)` / `WriteIniData(sPath,...)` call in
//      SetSGCONTFAIL/SetSGFTP/SetBINCOUNT).
//  14. TransformTcGPIBData, golden :14666 `ATC_InterfaceForm->SetOffset(
//      iATC_Use_Heat_Count, dbATC_Offset);` -- `ATC_InterfaceForm` is
//      acarry_shims.h:109's `TATC_InterfaceFormShim`, which carries ONLY
//      `iATC_MODE_TYPE` (no `SetOffset`). This is not a fresh finding: this
//      EXACT golden call (csystem.cpp:22363, the DoBoostFunctionStepCooling
//      offset push) is ALREADY gated in this tree under GATE H3-5
//      (csystem.cpp:28997-29016, `#if 0 ... ATC_InterfaceForm->SetOffset(
//      iATC_Use_Heat_Count, dbATC_Offset); ... #endif // GATE H3-5`) with
//      the identical two-argument call shape. Every other line of this
//      function (the whole per-arm/per-site `dbATC_Offset[]` computation,
//      the `dGPIBATCOffset[]` global writes, the >136/<20 out-of-range
//      `ret++` counting, `bGPIBOffsetCommand=true;`) is real computation
//      over real globals and stays ACTIVE -- same "computed then thrown
//      away" shape GATE H3-5's own commentary already documents for its
//      call site.
//  15. SetOSBIN, golden :14678-14681, the WHOLE `if(fBinSel->SetOSBin(...))
//      asRet="OK"; else asRet="NG";` -- `fBinSel` "has NO facade anywhere"
//      (established citation already on record at forms/fShowBinSelect.h
//      :202, `grep -rn "fBinSel" --include=*.h .`, re-run 20260818: still
//      only that one comment mentions the name, 0 real declarations).
//      Substituted with golden's OWN failure-path text, `asRet="NG";` -- not
//      an invented value, the exact string golden's own `else` arm already
//      produces.
//  16. WriteFTPDownSetupFile, golden :15071-15074 (`fFTPClient->
//      bControlByGPIB=true;` through `bControlByGPIB=false;`, 4 lines) --
//      `fFTPClient` has no port anywhere in this tree, the SAME absence
//      FW3-WD's SetSetupFileName/ChangeSetupFileName/SetSGFTP already
//      recorded (GATE REGISTER items 9/10/16-18 in that wave's OWN banner --
//      a different numbering space than this wave's). DELTA: because
//      `ShowFTPModal` never runs, `fMain->iHasChangeFile` keeps whatever
//      value it last held instead of being set by a real download outcome --
//      the `if(fMain->iHasChangeFile==1){...}else{...}` branch right after
//      this gate (ACTIVE, unchanged) will therefore almost always fall into
//      the `else`/"SETNG" arm offline. This is an inherent, honest
//      consequence of the absent facade, not something this wave papers
//      over.
//  17. ReadWaterValve, golden :15254 `ATC_InterfaceForm->ReadTCWaterValue();`
//      -- same shim limitation as item 14 (acarry_shims.h:109, only
//      `iATC_MODE_TYPE`). `asReadTCWater[]` (cmydef.h:5783, real global) is
//      read immediately after by the ACTIVE loop that builds `asStr` -- it
//      will simply hold whatever value it last held rather than a freshly-
//      read one, same shape as item 4/16's "real array, absent refresh call"
//      pattern.
// =============================================================================
void TfMain::TCPCommandServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bHandlerResultConnect=true;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Connect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void TfMain::TCPCommandServerClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Disconnect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void TfMain::TeraTCPResultServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bHandlerResultConnect=true;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Connect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void TfMain::TeraTCPResultServerClientDisconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    bHandlerResultConnect=false;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Disconnect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void TfMain::TeraTCPResultServerClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    bHandlerResultConnect=false;
    if(ErrorCode==10053)                                                        // 網路線拔除
    {
        ErrorCode=0;
        TCPIPCommunicationLog("Connect fail : 網路線拔除!");
    }
    else if(ErrorCode==10061)                                                   //連線失敗
    {
        ErrorCode=0;
        TCPIPCommunicationLog("Connect fail : 連線失敗!");
    }
    else
    {
        TCPIPCommunicationLog("Connect fail :" + AnsiString(ErrorCode));
        ErrorCode=0;
    }
    //TCPCommandServer->Close();

    try
    {
        // GATE(FW3-WE) golden :12602 -- see GATE REGISTER item 1 above.
#if 0
        TeraTCPResultServer->Close();
#endif
    }
    catch(...)
    {
        FW3WE_LogClientSocketExceptionError(Sender, "TeraTCPResultServerClient Error");
    }
}
//---------------------------------------------------------------------------
void TfMain::TCPIPCommunicationLog(AnsiString Str)
{
    AnsiString Path, s="";
    GetTimeInfo();
    // AI(W906-FW3-WE) 20260818: substitution (S9) -- see FW3-WE GROUP banner
    // above. golden: Now().FormatString("yyyy/mm/dd").
    s.sprintf("%s %02d:%02d:%02d.%03d %s",FormatDateTime("yyyy/mm/dd", Now()), SystemHour, SystemMin, SystemSec, SystemMSec,Str);   //Sam 20230420 : 時間紀錄到小數點
    // GATE(FW3-WE) golden :12615 -- see GATE REGISTER item 2 above.
#if 0
    fMain->Memo2->Lines->Add(s);
#endif

    Path.sprintf("%s\\%04d_%02d_%02d", asTCPIPPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTCPIPPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str(), s);
}
//---------------------------------------------------------------------------
void TfMain::HanderTcpIp()
{
    // GATE(FW3-WE) golden :12625-12626 -- see GATE REGISTER item 3 above.
    // GOLDEN BUG (B7): golden's own second line is `TCPCommandServer->
    // Active==false;` (comparison, not assignment) -- see the banner's B7
    // citation. Preserved verbatim.
#if 0
    if(TCPCommandServer->Active==true)
        TCPCommandServer->Active==false;
#endif

    AnsiString s="";
    iHandlerCommandServerPort=7016;
    iHandlerResultServerPort=7017;
    try
    {
        if(CosFunction.bEnableHandlerResultServer==true)
        {
            // GATE(FW3-WE) golden :12635-12646 -- see GATE REGISTER item 4
            // above.
#if 0
            fMain->TCPCommandServer->Close();
            fMain->TCPCommandServer->Port=iHandlerCommandServerPort;
            fMain->TCPCommandServer->Open();
            //Sam 20230417 : Log 新增 SocketHandle 資料
            s.sprintf("[%4d] Server Listen",iHandlerCommandServerPort);
            TCPIPCommunicationLog(s);
            fMain->TeraTCPResultServer->Close();
            fMain->TeraTCPResultServer->Port=iHandlerResultServerPort;
            fMain->TeraTCPResultServer->Open();
            //Sam 20230417 : Log 新增 SocketHandle 資料
            s.sprintf("[%4d] Server Listen",iHandlerResultServerPort);
            TCPIPCommunicationLog(s);
#endif
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfMain::HanderTcpIp");
        ShowMyMessage("Socket Server Open Error!!");
        TCPIPCommunicationLog("Socket Server Open Error!!");
    }
}
//---------------------------------------------------------------------------
void TfMain::HandlerTCPIPResultSendProcess(String sMessage)
{
    // GATE(FW3-WE) golden :12657-12723 -- see GATE REGISTER item 5 above
    // (WHOLE BODY -- every line is downstream of the absent
    // fMain->TCPCommandServer->Socket).
#if 0
    int iHandle=0,iErr=0,iHandleSendDone[100];
    int iPort=fMain->TCPCommandServer->Socket->LocalPort;
    AnsiString s="";
    char cBuffer[500];          //Sam 20201021 : for contact count 資料需要再加長 100//Sam 20191120 Fix Issue
    AnsiString Path;
    strcpy(cBuffer, sMessage.c_str());
    bool bNeedSend=false;
    ZeroMemory(iHandleSendDone, sizeof(iHandleSendDone));

    ReSendData:
    int iCount=fMain->TCPCommandServer->Socket->ActiveConnections;
    if(iCount>=1)   //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        for(int i=0; i<iCount; i++)
        {
            try
            {
                iHandle=fMain->TCPCommandServer->Socket->Connections[i]->SocketHandle;  //Sam 20230417 : Log 新增 SocketHandle 資料

                bNeedSend=true;      //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                for(int j=0;j<100;j++)
                {
                    if(iHandleSendDone[j]==iHandle)
                    {
                        bNeedSend=false;//有傳送過的 Handle 就不要再送了
                        break;
                    }
                }

                if(bNeedSend)
                {
                    s.sprintf("[%4d][%4d][ #Send#     ] %s",iHandle, iPort, sMessage);
                    TCPIPCommunicationLog(s);
                    fMain->TCPCommandServer->Socket->Connections[i]->SendBuf(cBuffer, sMessage.Length());

                    for(int j=0;j<100;j++)   //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                    {
                        if(iHandleSendDone[j]==0)
                        {
                            iHandleSendDone[j]=iHandle;//紀錄傳送過的 Handle
                            break;
                        }
                    }
                }
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfMain::HandlerTCPIPResultSendProcess");
                s.sprintf("[%4d][%4d][ #Exception#] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);

                iErr++;     //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                if(iErr<3)
                    goto ReSendData;
                else
                    ShowMyMessage("Handler Result Server Socket Error!!");
            }
        }
    }
    else    //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        s.sprintf("[%04d][%4d][ #SendFail# ] %s",iHandle, iPort, sMessage);
        TCPIPCommunicationLog(s);
    }
#endif
}
//---------------------------------------------------------------------------
void TfMain::HandlerTeraTResultSendProcess(String sMessage)
{
    // GATE(FW3-WE) golden :12725-12760 -- see GATE REGISTER item 6 above
    // (WHOLE BODY -- every line is downstream of the absent
    // fMain->TeraTCPResultServer->Socket).
#if 0
    int iHandle=0;
    int iPort=fMain->TeraTCPResultServer->Socket->LocalPort;
    AnsiString s="";
    char cBuffer[500];          //Sam 20201021 : for contact count 資料需要再加長 100//Sam 20191120 Fix Issue
    AnsiString Path;
    strcpy(cBuffer, sMessage.c_str());

    int iCount=fMain->TeraTCPResultServer->Socket->ActiveConnections;
    if(iCount>=1)   //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        for(int i=0; i<iCount; i++)
        {
            try
            {
                iHandle=fMain->TeraTCPResultServer->Socket->Connections[i]->SocketHandle;  //Sam 20230417 : Log 新增 SocketHandle 資料
                s.sprintf("[%4d][%4d][ #Send#     ] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);
                fMain->TeraTCPResultServer->Socket->Connections[i]->SendBuf(cBuffer, sMessage.Length());
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfMain::HandlerTeraTResultSendProcess");
                ShowMyMessage("Handler Result Server Socket Error!!");
                s.sprintf("[%4d][%4d][ #Exception#] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);
            }
        }
    }
    else    //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        s.sprintf("[%04d][%4d][ #SendFail# ] %s",iHandle, iPort, sMessage);
        TCPIPCommunicationLog(s);
    }
#endif
}
//---------------------------------------------------------------------------
int TfMain::GetProdModeByDll()                                                  //JerryYang 20220311 : ATP鎖定Critical parameter
{
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(RunInfo.bLotStart)                                                       //Lot start=Production mode
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//---------------------------------------------------------------------------
int TfMain::SetProdModeByDll(int iProdMode)                                     //bProdMode=true  : enable production(block edit mode)  //JerryYang 20220311 : ATP鎖定Critical parameter
{                                                                               //bProdMode=false : disable production (allow edit)
    AnsiString Msg="";
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(iProdMode==0)                                                            //Allow edit
    {
        if(RunInfo.bLotStart==false)
        {
            return 0;                                                           //Sucess
        }
        else
        {
            // AI(W906-FW3-WE) 20260818: substitution (S10) -- see FW3-WE
            // GROUP banner above. golden: fMain->CheckCanChangeRealDummy().
            if(ComputeCanChangeRealDummy(MOT[MMPlate1].HasIC(), MOT[MMPlate2].HasIC(),
                                          ShuttleHasIC(), IndexHasIC(),
                                          InArmSuck.HasIC(), OutArmSuck.HasIC())==false ||
               HasICUnderMachine())
            {
                return -1;                                                      //fail
            }
            // GATE(FW3-WE) golden :14339 -- see GATE REGISTER item 7 above.
#if 0
            fLotInfo->SetLotEnd(__FUNC__);
#endif
            // GATE(FW3-WE) golden :14340 -- see GATE REGISTER item 8 above.
#if 0
            fLotInfo->edtBarcodeRecipe->Enabled=false;                          //==> Eastsun 20260527 整合#027-2.MR.M1 SECS LotEnd lock BarcodeRecipe :KYEC
#endif
            return 0;                                                           //Sucess
        }
    }
    else                                                                        //Block edit
    {
        if(RunInfo.bLotStart)
        {
            return 0;                                                           //Sucess
        }
        else
        {
            fLotInfo->SetLotStart(__FUNC__);
            return 0;                                                           //Sucess
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::WriteREADYNEXTSHOT()                                               //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    SendMSG_CMD(MSG_CMD_READYNEXTSHOT, GetREADYNEXTSHOT());
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetREADYNEXTSHOT()                                           //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    AnsiString asDeviceInf="";
    AnsiString asSendData="";
    int iHasCode=0;

    if(IndexStatus==Z1Up_Z2Down)                                                //sht1 barcode scan
    {
        iHasCode=-1;
    }
    else if(IndexStatus==Z1Down_Z2Up)
    {
        iHasCode=-2;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)                                     //Steven 20231024 : not support NN mode
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(IndexStatus==Z1Up_Z2Down)                                        //sht1 barcode scan
            {
                // GATE(FW3-WE) golden :14384 -- see GATE REGISTER item 9
                // above (FTestSuck.HasNotTestYet() || dropped).
                if((FLCarryKit.Item[i][j]!=NULL_IC &&
                    FLCarryKit.Item[i][j]!=HAS_NULL_IC &&
                    FLCarryKit.cDeviceInf[i][j]!=""))
                {
                    iHasCode=1;
                }
            }
            else if(IndexStatus==Z1Down_Z2Up)
            {
                // GATE(FW3-WE) golden :14394 -- see GATE REGISTER item 9
                // above (BTestSuck.HasNotTestYet() || dropped).
                if((BLCarryKit.Item[i][j]!=NULL_IC &&
                    BLCarryKit.Item[i][j]!=HAS_NULL_IC &&
                    BLCarryKit.cDeviceInf[i][j]!=""))
                {
                    iHasCode=1;
                }
            }
        }
    }

    if(iHasCode==1)
    {
        asSendData="1";                                                         //1 =>  ready
    }
    else
    {
        // GATE(FW3-WE) golden :14414-14415 -- see GATE REGISTER item 9 above
        // (FTestSuck.HasNotTestYet() || / BTestSuck.HasNotTestYet() || each
        // dropped from their own clause). MOT[MMTrayY/MMPlate1/MMPlate2]
        // .HasIC() (golden :14411-14413) is NOT gated -- MOT[]/MMTrayY/
        // MMPlate1/MMPlate2 are all real (cmydef.h:2255-2258, Motor/mymotor.h
        // :385 `extern class TTrayMotor MOT[MAX_TRAY_MOTOR];`, already used
        // ACTIVE in this exact file by FW3-WC's ComputeCanChangeRealDummy()
        // call, :9869) -- kept ACTIVE, verbatim.
        if(MOT[MMTrayY].HasIC() ||
           MOT[MMPlate1].HasIC() ||
           MOT[MMPlate2].HasIC() ||
           (iHasCode==-1 && (FLCarryKit.HasRealIC())) ||
           (iHasCode==-2 && (BLCarryKit.HasRealIC())))
        {
            asSendData="2";                                                     //2 => Ongoing to read next 2DID
        }
        else
        {
            asSendData="0";
        }
    }

    asSendData="READYNEXTSHOT:"+asSendData;
    return asSendData;
}
//---------------------------------------------------------------------------
void TfMain::WriteNEXT2DID()                                                    //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    SendMSG_CMD(MSG_CMD_NEXT2DID, GetNEXT2DID());
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetNEXT2DID()                                                //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    TStringList *tNEXTBarCodeList=new TStringList;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        tNEXTBarCodeList->Add("0");
    }

    if(IndexStatus==Z1Up_Z2Down)                                                //sht1 barcode scan
    {
        // GATE(FW3-WE) golden :14442-14447 -- see GATE REGISTER item 10
        // above (whole FTestSuck.HasNotTestYet() selector).
#if 0
        if(FTestSuck.HasNotTestYet())
            Get2DID_OrderBySites(&FTestSuck, tNEXTBarCodeList);
        else
            Get2DID_OrderBySites(&FLCarryKit, tNEXTBarCodeList);
#endif
    }
    else                                                                        //sht2 barcode scan
    {
        // GATE(FW3-WE) golden :14449-14454 -- see GATE REGISTER item 11
        // above (whole BTestSuck.HasNotTestYet() selector).
#if 0
        if(BTestSuck.HasNotTestYet())
            Get2DID_OrderBySites(&BTestSuck, tNEXTBarCodeList);
        else
            Get2DID_OrderBySites(&BLCarryKit, tNEXTBarCodeList);
#endif
    }
    AnsiString t="NEXT2DID:"+tNEXTBarCodeList->CommaText;
    delete tNEXTBarCodeList;
    return t;
}
//---------------------------------------------------------------------------
void TfMain::Get2DID_OrderBySites(TMyKitSuck *kit, TStringList *sSourceList)    //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    if((BAR_CODE_INSTALL!=ebctUninstall && TestIF.bEnableBarCode) ||
       (INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction))                      //Ifor 20210407 add: 自製OCR
    {
    }
    else
    {
        return;
    }
    int iSiteNo=0;
    for(int i=0; i<TestSocket.iShtRow; i++)                                     //Steven 20231024 : need to add for NN mode
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iSiteNo=TestIF.iSiteMap[i][j]-1;
            if(iSiteNo>=0 &&
               kit->Item[i][j]!=NULL_IC &&
               kit->Item[i][j]!=HAS_NULL_IC)
            {
                sSourceList->Strings[MAX_SOCKET_TOTAL-1-iSiteNo]=kit->cDeviceInf[i][j];
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetAICCD()                                                         //Sam 20231108 : Add GPIB SETAICCD_
{
    TStringList *tFixAOICmd=new TStringList();
    AnsiString asRet="OK", asCmd="", szDir="",str1="";
    char str[256];

    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);

    tFixAOICmd->CommaText=str1;

    if(tFixAOICmd->Count==8)                                                    //Sam 20240826 : FixAOI 指令修改
    {
        //1_1_1000_15000_0.5_150 共六組參數
        //bEnableFix2BGAAICCD
        TestIF_File.bEnableFix2BGAAICCD             =(tFixAOICmd->Strings[0]=="1")?true:false;
        TestIF_File.iResultShowType                 =atoi(tFixAOICmd->GetString(1).c_str());      //Sam 20240325 : 新增 DamageTrayMapping 功能
        TestIF_File.iFix2BGAAICCDStartDelay         =atoi(tFixAOICmd->GetString(2).c_str());
        TestIF_File.iFix2BGAAICCDExposureTimeOut    =atoi(tFixAOICmd->GetString(3).c_str());
        TestIF_File.iFix2BGAAICCDAutoRetry          =atoi(tFixAOICmd->GetString(4).c_str());      //Sam 20240826 : FixAOI 指令修改
        TestIF_File.dInspectResultThres             =atof(tFixAOICmd->GetString(5).c_str());
        TestIF_File.iFix2BGAAICCDGetResultTimeOut   =atoi(tFixAOICmd->GetString(6).c_str());
        TestIF_File.iFix2BGAAICCDOutArmCycleInsp    =atoi(tFixAOICmd->GetString(7).c_str());

        // GATE(FW3-WE) golden :14513-14518 -- see GATE REGISTER item 12
        // above.
#if 0
        fFixAICCD->chkEnableFix2AICCD->Checked          =TestIF_File.bEnableFix2BGAAICCD;
        fFixAICCD->rgResultShowType->ItemIndex          =TestIF_File.iResultShowType;   //Sam 20240325 : 新增 DamageTrayMapping 功能
        fFixAICCD->edtlFix2AICCDStartDelay->Text        =IntToStr(TestIF_File.iFix2BGAAICCDStartDelay);
        fFixAICCD->edtlFix2AICCDExposureTimeOut->Text   =IntToStr(TestIF_File.iFix2BGAAICCDExposureTimeOut);
        fFixAICCD->edtInspectResultThres->Text          =FloatToStr(TestIF_File.dInspectResultThres);
        fFixAICCD->edtOutArmCycleInsp->Text             =IntToStr(TestIF_File.iFix2BGAAICCDOutArmCycleInsp);
#endif

        // GATE(FW3-WE) golden :14520-14525 -- see GATE REGISTER item 13
        // above (IniData\ shared config write).
#if 0
        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Enable",                  TestIF_File.bEnableFix2BGAAICCD);
        WriteIniData(szDir, "Configuration", "Fix2 AI iResultShowType",             TestIF_File.iResultShowType);   //Sam 20240325 : 新增 DamageTrayMapping 功能
        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Start Delay",             TestIF_File.iFix2BGAAICCDStartDelay);
        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Exposure Time Out",       TestIF_File.iFix2BGAAICCDExposureTimeOut);
        WriteIniData(szDir, "Configuration", "Fix2 AI InspectResultThres",          TestIF_File.dInspectResultThres);
        WriteIniData(szDir, "Configuration", "Fix2 AI OutArmCycleInsp",             TestIF_File.iFix2BGAAICCDOutArmCycleInsp);
#endif
    }
    else
    {
        asRet="NG";
    }
    SendMSG_CMD(MSG_CMD_SETAICCD, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetAICCD()                                                         //Sam 20240826 : Add GPIB GETAICCD?
{
    AnsiString asRet="";
    asRet.sprintf("AICCD_%s_%d_%d_%d_%d_%2.2f_%d_%d",
                  (TestIF_File.bEnableFix2BGAAICCD)?"1":"0",
                   TestIF_File.iResultShowType,
                   TestIF_File.iFix2BGAAICCDStartDelay,
                   TestIF_File.iFix2BGAAICCDExposureTimeOut,
                   TestIF_File.iFix2BGAAICCDAutoRetry,
                   TestIF_File.dInspectResultThres,
                   TestIF_File.iFix2BGAAICCDGetResultTimeOut,
                   TestIF_File.iFix2BGAAICCDOutArmCycleInsp);
    SendMSG_CMD(MSG_CMD_GETAICCD, asRet);
}
//---------------------------------------------------------------------------
bool TfMain::IsStackHasLess16Bin(int iStack)                                    //JerryYang 20231218 : 檢查該Unloader是否有設定bin別  //JerryYang 20250505 : 只檢查16 bin以下
{
    int iBinCntMax=17;
    for(int i=1; i<iBinCntMax; i++)                                             //從Bin1開始, 比照Epson所定義的格式
    {
        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]==iStack+1)
        {
            return true;
        }
    }

    if(BinSelect[iTestRunMode].IfErrorT3==iStack)                               //比照EPSON的指令, Bin0當作是Error bin
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfMain::TransformTcGPIBData(AnsiString asStr)
{
    double dbATC_Offset[ATC_HEAD_COUNT];
    int ret=0;
    ZeroMemory(dbATC_Offset, sizeof(dbATC_Offset));
    AnsiString str;
    TStringList *IsLine=new TStringList();
    int Num=asStr.Pos("_");
    asStr.Delete(1, Num);
    str=StringReplace(asStr,"_", ",", TReplaceFlags()<<rfReplaceAll);
    IsLine->CommaText=str;

    bool bUse1by2Hear=false;
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;

    if((TestIF_File.iTestMode==_12Site2X6   ||
        TestIF_File.iTestMode==_16Site2X8   ||
        TestIF_File.iTestMode==_32Site4X8N) &&
        TestIF_File.bUse32Heater==false)
    {
        iCol=TestSocket.iShtCol/2;
        bUse1by2Hear=true;
    }
    else if(TestIF_File.iTestMode==QualSite2X2N ||
            TestIF_File.iTestMode==_6Site2X3N   ||
            TestIF_File.iTestMode==_8Site2X4N)
    {
        iRow=TestSocket.iShtRow/2;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<iCol; j++)
        {
            //          Arm1                    Arm2
            //00 01 02 03 04 05 06 07   16 17 18 19 20 21 22 23
            //08 09 10 11 12 13 14 15   24 25 26 27 28 29 30 31
            if(iRow==1)
            {                                                                   //20160523 irow = 1 Site Mapping 方向  由左至右資料讀取
                dbATC_Offset[j]=Temperature.dATCTempOffset[j];                                          //Ifor 20160523 Arm1 陣列起始位置00
                dbATC_Offset[j+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[j+16];               //Ifor 20160523 Arm2 陣列起始位置16
            }
            else
            {
                if(bUse1by2Hear==true)
                {
                    dbATC_Offset[j*2+i]=Temperature.dATCTempOffset[j*2+(i*8)];                                //Ifor 20160523 Arm1 陣列起始位置00 +下排起始位置間距8  (00+8=08)
                    dbATC_Offset[j*2+i+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[(j*2+16)+(i*8)];   //Ifor 20160523 Arm2 陣列起始位置16 +下排起始位置間距8  (16+8=24)
                }
                else
                {                                                               //Ifor 20160523 irow = 2 Site Mapping 方向 由上至下 由下往上 位置之資料讀取
                    dbATC_Offset[j*2+i]=Temperature.dATCTempOffset[j+(i*8)];                                //Ifor 20160523 Arm1 陣列起始位置00 +下排起始位置間距8  (00+8=08)
                    dbATC_Offset[j*2+i+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[(j+16)+(i*8)];   //Ifor 20160523 Arm2 陣列起始位置16 +下排起始位置間距8  (16+8=24)
                }
            }
        }
    }

    for(int i=0; i<(iATC_Use_Heat_Count/2); i++)
    {
        if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
        {
            dGPIBATCOffset[i]=atof(IsLine->GetString(i).c_str());
            dbATC_Offset[i]+=atof(IsLine->GetString(i).c_str());
            if(Temperature.fWorkTemperBase+dbATC_Offset[i]>136 ||
               Temperature.fWorkTemperBase+dbATC_Offset[i]<20)
                ret++;
        }
        else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
        {
            dGPIBATCOffset[i+iATC_Use_Heat_Count/2]=atof(IsLine->GetString(i).c_str());
            dbATC_Offset[i+iATC_Use_Heat_Count/2]+=atof(IsLine->GetString(i).c_str());
            if(Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]>136 ||
               Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]<20)
                ret++;
        }
        else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
        {
            if(i<iRow*iCol)
            {
                dGPIBATCOffset[i]=atof(IsLine->GetString(i).c_str());
                dbATC_Offset[i]+=atof(IsLine->GetString(i).c_str());
                if(Temperature.fWorkTemperBase+dbATC_Offset[i]>136 ||
                   Temperature.fWorkTemperBase+dbATC_Offset[i]<20)
                    ret++;

                dGPIBATCOffset[i+iATC_Use_Heat_Count/2]=atof(IsLine->GetString(i).c_str());
                dbATC_Offset[i+iATC_Use_Heat_Count/2]+=atof(IsLine->GetString(i+(iRow*iCol)).c_str());
                if(Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]>136 ||
                   Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]<20)
                    ret++;
            }
        }
    }

    if(ret==0)
    {
        bGPIBOffsetCommand=true;
        // GATE(FW3-WE) golden :14666 -- see GATE REGISTER item 14 above
        // (established precedent GATE H3-5, csystem.cpp:29014).
#if 0
        ATC_InterfaceForm->SetOffset(iATC_Use_Heat_Count, dbATC_Offset);        //TransformTcGPIBData, not using
#endif
    }
    delete IsLine;                                                              //Steven 20160912 : Add delete for save memory
}
//---------------------------------------------------------------------------
void TfMain::SetOSBIN()                                                         //Sam 20250115 : Add GPIB SETOSBIN_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();

    // GATE(FW3-WE) golden :14678-14681 -- see GATE REGISTER item 15 above.
    // Substituted with golden's own failure-path text.
#if 0
    if(fBinSel->SetOSBin(atoi(asCmd.c_str())))
        asRet="OK";
    else
        asRet="NG";
#else
    asRet="NG";
#endif
    SendMSG_CMD(MSG_CMD_SETOSBIN, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetOSBIN()                                                         //Sam 20250115 : Add GPIB GETOSBIN?
{
    AnsiString asRet="";
    asRet.sprintf("OSBIN_%d", TestIF_File.iOpenBin);
    SendMSG_CMD(MSG_CMD_GETOSBIN, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetDUTCHK()                                                        //Steven 20250701 : for DOOSAN TESNA
{
    AnsiString asRet="";
    asRet+=AnsiString((TestIF_File.iSiteMap[0][0]<10)?TestIF_File.iSiteMap[0][0]:TestIF_File.iSiteMap[0][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][0]<10)?TestIF_File.iSiteMap[1][0]:TestIF_File.iSiteMap[1][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][1]<10)?TestIF_File.iSiteMap[0][1]:TestIF_File.iSiteMap[0][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][1]<10)?TestIF_File.iSiteMap[1][1]:TestIF_File.iSiteMap[1][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][2]<10)?TestIF_File.iSiteMap[0][2]:TestIF_File.iSiteMap[0][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][2]<10)?TestIF_File.iSiteMap[1][2]:TestIF_File.iSiteMap[1][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][3]<10)?TestIF_File.iSiteMap[0][3]:TestIF_File.iSiteMap[0][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][3]<10)?TestIF_File.iSiteMap[1][3]:TestIF_File.iSiteMap[1][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][4]<10)?TestIF_File.iSiteMap[0][4]:TestIF_File.iSiteMap[0][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][4]<10)?TestIF_File.iSiteMap[1][4]:TestIF_File.iSiteMap[1][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][5]<10)?TestIF_File.iSiteMap[0][5]:TestIF_File.iSiteMap[0][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][5]<10)?TestIF_File.iSiteMap[1][5]:TestIF_File.iSiteMap[1][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][6]<10)?TestIF_File.iSiteMap[0][6]:TestIF_File.iSiteMap[0][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][6]<10)?TestIF_File.iSiteMap[1][6]:TestIF_File.iSiteMap[1][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][7]<10)?TestIF_File.iSiteMap[0][7]:TestIF_File.iSiteMap[0][7]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][7]<10)?TestIF_File.iSiteMap[1][7]:TestIF_File.iSiteMap[1][7]+'A');

    asRet+=AnsiString((TestIF_File.iSiteMap[2][0]<10)?TestIF_File.iSiteMap[2][0]:TestIF_File.iSiteMap[2][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][0]<10)?TestIF_File.iSiteMap[3][0]:TestIF_File.iSiteMap[3][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][1]<10)?TestIF_File.iSiteMap[2][1]:TestIF_File.iSiteMap[2][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][1]<10)?TestIF_File.iSiteMap[3][1]:TestIF_File.iSiteMap[3][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][2]<10)?TestIF_File.iSiteMap[2][2]:TestIF_File.iSiteMap[2][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][2]<10)?TestIF_File.iSiteMap[3][2]:TestIF_File.iSiteMap[3][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][3]<10)?TestIF_File.iSiteMap[2][3]:TestIF_File.iSiteMap[2][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][3]<10)?TestIF_File.iSiteMap[3][3]:TestIF_File.iSiteMap[3][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][4]<10)?TestIF_File.iSiteMap[2][4]:TestIF_File.iSiteMap[2][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][4]<10)?TestIF_File.iSiteMap[3][4]:TestIF_File.iSiteMap[3][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][5]<10)?TestIF_File.iSiteMap[2][5]:TestIF_File.iSiteMap[2][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][5]<10)?TestIF_File.iSiteMap[3][5]:TestIF_File.iSiteMap[3][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][6]<10)?TestIF_File.iSiteMap[2][6]:TestIF_File.iSiteMap[2][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][6]<10)?TestIF_File.iSiteMap[3][6]:TestIF_File.iSiteMap[3][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][7]<10)?TestIF_File.iSiteMap[2][7]:TestIF_File.iSiteMap[2][7]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][7]<10)?TestIF_File.iSiteMap[3][7]:TestIF_File.iSiteMap[3][7]+'A');

    SendMSG_CMD(MSG_CMD_DUTCHK, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetFFC()                                                           //Steven 20250701 : for Ampere
{
    AnsiString asRet="";
    if(Temperature.bATC_FFCEnable==true)
        asRet.sprintf("GetFFC 1");
    else
        asRet.sprintf("GetFFC 0");
    SendMSG_CMD(MSG_CMD_GetFFC, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetTJFunction()
{
    AnsiString asRet="";
    if(Temperature.bEnableTJFunction==true)
        asRet.sprintf("GetTJFunction 1");
    else
        asRet.sprintf("GetTJFunction 0");
    SendMSG_CMD(MSG_CMD_GetTJFunction, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetPowerFollowing()
{
    AnsiString asRet="";
    if(Temperature.bPowerFollower_Enable==true)
        asRet.sprintf("GetPowerFollowing 1");
    else
        asRet.sprintf("GetPowerFollowing 0");
    SendMSG_CMD(MSG_CMD_GetPowerFollowing, asRet);
}
//---------------------------------------------------------------------------
void TfMain::WriteHeadContactCount()                                 //Ifor 20240510 add:Report Head Contact Count
{
    AnsiString t="";
    AnsiString asCmd="";
    int iSelect=-1;

    char str[256];

    try
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        iSelect=atoi(asCmd.c_str())-1;
    }
    catch(...)
    {
        // GOLDEN ODDITY (B8) -- see FW3-WE GROUP banner above for the full
        // citation. Preserved verbatim (harmless here, cross-purpose
        // global).
        asChangeSetupFileName="";
        iSelect=-1;
    }

    if(iSelect<0 || iSelect>=3)
    {
        t.sprintf("Out_Of_Scope\r");
    }
    else
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            t.sprintf("Arm1_%d_%d_Arm2_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2]);
        }
        else if(TestIF_File.iTestMode==DualSite2x1)
        {
            t.sprintf("Arm1_%d_%d_Arm2_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1]);
        }
        else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==_8Site1X4)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][6]);
        }
        else if(TestIF_File.iTestMode==SingleSite)
        {
            t.sprintf("Arm1_%d_Arm2_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][1][0]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3]);
        }
        else if(TestIF_File.iTestMode==_6Site2X3)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5]);
        }
        else if(TestIF_File.iTestMode==TriSite1X3)
        {
            t.sprintf("Arm1_%d_%d_%d_Arm2_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][4]);
        }
        else if(TestIF_File.iTestMode==_8Site2X4)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7]);
        }
        else if(TestIF_File.iTestMode==_16Site2X8)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][0][10],
                                        IniConfig.HeadContactCount[iSelect][0][11],
                                        IniConfig.HeadContactCount[iSelect][0][12],
                                        IniConfig.HeadContactCount[iSelect][0][13],
                                        IniConfig.HeadContactCount[iSelect][0][14],
                                        IniConfig.HeadContactCount[iSelect][0][15],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9],
                                        IniConfig.HeadContactCount[iSelect][1][10],
                                        IniConfig.HeadContactCount[iSelect][1][11],
                                        IniConfig.HeadContactCount[iSelect][1][12],
                                        IniConfig.HeadContactCount[iSelect][1][13],
                                        IniConfig.HeadContactCount[iSelect][1][14],
                                        IniConfig.HeadContactCount[iSelect][1][15]);
        }
        else if(TestIF_File.iTestMode==_12Site2X6)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][0][10],
                                        IniConfig.HeadContactCount[iSelect][0][11],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9],
                                        IniConfig.HeadContactCount[iSelect][1][10],
                                        IniConfig.HeadContactCount[iSelect][1][11]);
        }
        else if(TestIF_File.iTestMode==_10Site2X5)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9]);
        }
        else
        {
            t.sprintf("UNKNOWN\r");
        }
    }

    SendMSG_CMD(MSG_CMD_GetContactCount, t);
}

//---------------------------------------------------------------------------
void TfMain::WriteSetSetupFile()         //Ifor 20231101 add:FTP Function
{
    AnsiString t="";
    AnsiString asRawSetupFileName="";
    bool bMachineHasIC=false;
    bool bDownloadFail=false;

    try         //kevin 20180320 add 避免資料轉換異常
    {
        asChangeSetupFileName=HGpib2Handler->cReturn;
    }
    catch(...)
    {
        asChangeSetupFileName="";
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
        bMachineHasIC=true;

    if(IniConfig.bEnableFTP==true || bMachineHasIC==true)
    {
        t.sprintf("SETNG_%s", asChangeSetupFileName);
    }
    else
    {
        if(asChangeSetupFileName=="")
        {
            t.sprintf("SETNG_%s", asChangeSetupFileName);
        }
        else
        {
            fMain->iHasChangeFile=9;
            asRawSetupFileName=fMain->cbSetupFileName->Text;
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\r", "", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\n", "", TReplaceFlags()<<rfReplaceAll);
            fMain->cbSetupFileName->Text=asChangeSetupFileName;
            fMain->cbSetupFileNameChange(fMain);

            if(fMain->iHasChangeFile==1)
            {
                t.sprintf("SETOK_%s", fMain->cbSetupFileName->Text);
            }
            else
            {
                t.sprintf("SETNG_%s", asRawSetupFileName);
                bDownloadFail=true;
            }
        }
    }
    fMain->iHasChangeFile=0;
    SendMSG_CMD(MSG_CMD_SetupFileChange, t);
    if(bDownloadFail)
    {
        iChangeFileHasErr=2;
    }
}
//---------------------------------------------------------------------------
void TfMain::WriteFTPDownSetupFile()         //Ifor 20231101 add:FTP Function
{
    AnsiString t="";
    AnsiString asRawSetupFileName="";
    bool bMachineHasIC=false;
    bool bDownloadFail=false;

    try         //kevin 20180320 add 避免資料轉換異常
    {
        asChangeSetupFileName=HGpib2Handler->cReturn;
    }
    catch(...)
    {
        asChangeSetupFileName="";
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
        bMachineHasIC=true;

    if(IniConfig.bEnableFTP==false || bMachineHasIC==true)
    {
        t.sprintf("SETNG_%s", asChangeSetupFileName);
    }
    else
    {
        if(asChangeSetupFileName == "")
        {
            t.sprintf("SETNG_%s", asChangeSetupFileName);
        }
        else
        {
            asRawSetupFileName=fMain->cbSetupFileName->Text;
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\r", "", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\n", "", TReplaceFlags()<<rfReplaceAll);

            // GATE(FW3-WE) golden :15071-15074 -- see GATE REGISTER item 16
            // above.
#if 0
            fFTPClient->bControlByGPIB=true;
            fFTPClient->asSetUpNameByGPIB=asChangeSetupFileName;
            fFTPClient->ShowFTPModal(0);
            fFTPClient->bControlByGPIB=false;
#endif

            if(fMain->iHasChangeFile==1)
            {
                t.sprintf("SETOK_%s", fMain->cbSetupFileName->Text);
            }
            else
            {
                t.sprintf("SETNG_%s", asRawSetupFileName);
                bDownloadFail=true;
            }
        }
    }
    fMain->iHasChangeFile=0;
    SendMSG_CMD(MSG_CMD_FTPDownLoad, t);

    if(bDownloadFail)
    {
        iChangeFileHasErr=1;
//        if(fNote->fShow)      //Ifor 20240314 Mark GPIB 通訊會卡住 報警拉到外面處理
//        {
//            iChangeFileHasErr=1;
//        }
//        else
//        {
//            iChangeFileHasErr=0;
//            ShowErrorMessage("WAR1684", K_SKIP, MMSystem, 0, asChangeSetupFileName);     //下載 %s.zip 失敗
//        }
    }
}
//---------------------------------------------------------------------------
void TfMain::GetSocketCounter()
{
    AnsiString asRet,asCmd;

    if(TestIF_File.iTestMode==DualSite2x1)
    {
        asRet.sprintf("%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N)
    {
        asRet.sprintf("%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)
    {
        asRet.sprintf("%d,%d,%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1],LastSet.iSocketContactCount[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)
    {
        asRet.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[0][3],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1],LastSet.iSocketContactCount[1][2],LastSet.iSocketContactCount[1][3]);
    }
    else
    {
        if(TestIF_File.iTestMode==SingleSite)
        {
            asRet.sprintf("%d",LastSet.iSocketContactCount[0][0]);
        }
        else if(TestIF_File.iTestMode==DualSite)
        {
            asRet.sprintf("%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1]);
        }
        else if(TestIF_File.iTestMode==TriSite1X3)
        {
            asRet.sprintf("%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2]);
        }
        else if(TestIF_File.iTestMode==QualSite1X4)
        {
            asRet.sprintf("%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[0][3]);
        }
        else
        {
            asRet.sprintf("NG");
        }
    }

    SendMSG_CMD(MSG_CMD_GetSocketCounter, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetTIMCounter()
{
    AnsiString asRet,asCmd;
    int iCh=0;
    char str[256];

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
//    iCh=atoi(asCmd);
    iCh=asCmd.ToIntDef(-1)-1;

    if(asCmd!="" && iCh!=-1)
    {
        asRet.sprintf("OK");
        if(TestIF_File.iTestMode==DualSite2x1)
        {
            asRet.sprintf("%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][1]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2)
        {
            asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2N)
        {
            asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][0][0],IniConfig.HeadContactCount[0][0][2],IniConfig.HeadContactCount[0][1][0],IniConfig.HeadContactCount[0][1][2]);   //待驗證
        }
        else if(TestIF_File.iTestMode==_6Site2X3)
        {
            asRet.sprintf("%d,%d,%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3],IniConfig.HeadContactCount[0][iCh][5]);
        }
        else if(TestIF_File.iTestMode==_8Site2X4)
        {
            asRet.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][6],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3],IniConfig.HeadContactCount[0][iCh][5],IniConfig.HeadContactCount[0][iCh][7]);
        }
        else
        {
            if(TestIF_File.iTestMode==SingleSite)
            {
                asRet.sprintf("%d",IniConfig.HeadContactCount[0][iCh][0]);
            }
            else if(TestIF_File.iTestMode==DualSite)
            {
                asRet.sprintf("%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2]);
            }
            else if(TestIF_File.iTestMode==TriSite1X3)
            {
                asRet.sprintf("%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4]);
            }
            else if(TestIF_File.iTestMode==QualSite1X4)
            {
                asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][6]);
            }
            else
            {
                asRet.sprintf("NG");
            }
        }
    }

    SendMSG_CMD(MSG_CMD_GetTIMCounter, asRet);
}
//---------------------------------------------------------------------------
void TfMain::WriteMultiZoneTemp()
{
    AnsiString asMultiZone="";

    if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
    {
        asMultiZone.sprintf("%s_%s_%s_%s;\r\n",
                            asGPIBTempShow[tcAa1],
                            asGPIBTempShow[tcAb1],
                            asGPIBTempShow[tcAc1],
                            asGPIBTempShow[tcAd1]);
    }
    else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
    {
        asMultiZone.sprintf("%s_%s_%s_%s;\r\n",
                            asGPIBTempShow[tcAa2],
                            asGPIBTempShow[tcAb2],
                            asGPIBTempShow[tcAc2],
                            asGPIBTempShow[tcAd2]);
    }

    SendMSG_CMD(MSG_CMD_MultiZoneTemp, asMultiZone.c_str());
}
//---------------------------------------------------------------------------
void TfMain::WriteMultiZoneEnable()
{
    AnsiString asMultiZoneEnable="";

    asMultiZoneEnable.sprintf("%d,%d,%d,%d;\r\n",
                              Temperature.bZoneTempEnable[0],
                              Temperature.bZoneTempEnable[1],
                              Temperature.bZoneTempEnable[2],
                              Temperature.bZoneTempEnable[3]);

    SendMSG_CMD(MSG_CMD_MultiZoneEnable, asMultiZoneEnable.c_str());
}
//---------------------------------------------------------------------------
void TfMain::ReadWaterValve()
{
    AnsiString asStr="";
    // GATE(FW3-WE) golden :15254 -- see GATE REGISTER item 17 above.
#if 0
    ATC_InterfaceForm->ReadTCWaterValue();
#endif
    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        asStr+=asReadTCWater[i];
        asStr+=",";
    }
    SendMSG_CMD(MSG_CMD_READ_WATER_VALVE, asStr);
}
//---------------------------------------------------------------------------
void TfMain::ReadDynamicPID()
{
    AnsiString asStr="";
    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        asStr+=asReadDynamicPID[i];
        asStr+=",";
    }
    SendMSG_CMD(MSG_CMD_READ_DYNAMIC_PID, asStr);
}
//---------------------------------------------------------------------------

// -- FW3-WE APPEND -- end (TCP-server family golden Command.cpp :12540-12761; ByDLL-tail/GPIB-2DID/FixAICCD/ATC-multizone family golden Command.cpp :14302-15273) --

