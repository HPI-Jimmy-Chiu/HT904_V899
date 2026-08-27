// =============================================================================
//  forms/fTesterIF.h  --  non-VCL facade for golden's TFTestIF (cTesterIF.h)
//
//  AI(W906-FW3-TIF1) 20260828: new file, FW wave FW3-TIF1 (facade wave 1 of
//  cTesterIF -- read-only direction, no write path). Golden source of truth:
//  HT9011UC_Code_V3.33.906.0_20260618/cTesterIF.h (318 lines) +
//  cTesterIF.cpp (1569 lines). Decoded this wave with
//  `io.open(p, encoding='cp950')` (0 U+FFFD asserted over the whole file
//  before any quoting); every GATED body below is SLICED PROGRAMMATICALLY out
//  of that decode rather than retyped. Written out UTF-8 / bare-LF.
//
//  ============================================================================
//  ⚠ NAME COLLISION -- READ BEFORE TOUCHING ANYTHING IN THIS FILE
//  ============================================================================
//  `TFTestIF` / `FTestIF` (THIS form) and `TestIF` / `TestIF_File` (two plain
//  DATA globals of type `SYSTEM_TEST_IF`, declared cprod.h:2576-2577, defined
//  in cprod.cpp = target ht9045_globals) are COMPLETELY DIFFERENT THINGS that
//  merely look alike. Declaring or reaching one is NOT declaring or reaching
//  the other, and no method of this facade may be written as "operate on
//  TestIF_File" as a substitute for the form. The one ACTIVE body here that
//  touches the data globals -- AntiSignalCBoxClick, golden :1291
//  `TestIF.bAntiSignal = AntiSignalCBox->Checked;` -- is golden's own text
//  doing exactly that, and is annotated at the body. Same class of hazard as
//  `fShowBinSet` vs `fShowBinSelect` (differ by "elect") and
//  `TATC_InterfaceForm` vs `TATCInterfaceForm` (differ by one underscore).
//
//  ROLE
//  ----
//  TFTestIF is golden's "Tester Interface" recipe dialog: it picks the
//  handler<->tester link type (TTL / GP-IB / RS-232C / TCP-IP), the DIO
//  profile, the GPIB address and 2DID format, the RS-232 line parameters, and
//  the whole family of test-timing and initial-delay values (FT / RT / EQC
//  variants), then persists them to `<recipe>\Tester.Data`.
//
//  `python tools/census/wave_preflight.py cTesterIF.cpp` (20260828, re-run at
//  closeout): 38 bodies = 38 `TFTestIF::` members + 0 file-scope, 1488 span
//  lines, 0 brace-swallow short-fall, 0 overload collapse.
//  THIS WAVE: 21 of 38 members ACTIVE (55.3%, one of them PARTIAL) + 16 GATED
//  (bodies translated into `#if 0` in the .cpp) + 1 OMITTED ENTIRELY.
//  BY LINES: the 21 ACTIVE bodies' golden spans total 142 of 1488 lines
//  (9.5%) -- but the ctor is only PARTIALLY live, so the honest live figure is
//  115 of 1488 (7.7%). The 16 GATED bodies carry 1338 of 1488 (89.9%) as
//  transcript; FormShortCut's 8 lines (0.5%) are omitted entirely.
//  ⚠ 21/38 members and 7.7% of lines are BOTH true and they disagree, because
//  every large body in this form is a persistence or a link-boundary body and
//  every small one is a keyboard/mouse handler. Quote both or neither.
//
//  ============================================================================
//  *** THE GLOBALS `TFTestIF` / `FTestIF` ARE FREE -- THIS WAVE CLAIMS THEM ***
//  Measured 20260828 05:14 over the 1,199 git-tracked .cpp/.h/.hpp files
//  (`git ls-files -- '*.cpp' '*.h' '*.hpp'` + per-file regex scan; build_*/ is
//  untracked and therefore excluded):
//    `TFTestIF` -- 9 hits: 8 are banner prose stating that golden's TFTestIF
//      has no port (Automation/SCK_ART_Remainder.cpp:148, .h:192,
//      Command.cpp:168 and :3684, ProductionInfo/uPAT_Function.cpp:296,
//      forms/fTemp_Set.h:148, uTemp_Set.cpp:214 and :3356); 1 is a generated
//      layout row (tools/dfm2rc/layout_out/cTesterIF_layout.gen.cpp:12).
//      0 `class TFTestIF` definitions.
//    `FTestIF` -- same 9 files plus the PRE-COMMITTED `#if 0` sites below.
//      0 global definitions.
//  `.h` declares `extern TFTestIF *FTestIF;`, `.cpp` defines
//  `TFTestIF *FTestIF = new TFTestIF();` -- same idiom as
//  forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24.
//
//
//  CMakeLists.txt LINE NUMBERS WERE RE-MEASURED AFTER REGISTRATION.
//  Every `CMakeLists.txt:N` / `ht9045_xxx:N` citation in this file was first
//  taken at 20260828 05:10, then the integrating wave registered this facade's
//  .cpp into `add_library(ht9045_forms ...)` -- a 53-line insertion (46 lines, plus a later 7-line
//  correction by the integrator) at
//  CMakeLists.txt:924-976 -- which shifted EVERY citation from :935 onward by
//  +53 in total. They were re-measured TWICE -- after the registration, and
//  again after that 7-line correction -- each time READ OFF the file rather
//  than derived by adding an offset, because deriving is how off-by-N gets in.
//  All were
//  corrected. Citations at :272 / :485 / :699 sit ABOVE the insertion point and
//  are unchanged. Adding sources to ht9045_forms moves these numbers again --
//  re-measure, do not trust them blind.
//  ============================================================================
//  PRE-COMMITTED `#if 0` CALL SITES ELSEWHERE (10 names / 14 sites). Each
//  enclosing `#if` was located this wave by walking UP from the call line --
//  NONE is live, which matches wave_preflight's "LIVE 0 member(s)".
//  ============================================================================
//    DoIniDataToForm()          csystem.cpp:29300  (enclosing `#if 0 // GATE
//                               H3-6` at :29291) -- zero arguments.
//    SaveSetupFile(AnsiString,AnsiString)
//                               csystem.cpp:29301  (same GATE H3-6),
//                               ProductionInfo/uPAT_Function.cpp:2069
//                               (GATE [G12] at :2058) -- 2 arguments both.
//    ReadTestIFFile()           uTemp_Set.cpp:3365 (GATE(G-FTestIF) at :3364),
//                               uTemp_Set.cpp:5384 (enclosing
//                               `#if 0 // SAFETY GATE (S1)` at :4853).
//    rgInterfaceType            uPAT_Function.cpp:2048 (read ->ItemIndex),
//                               :2060 (write ->ItemIndex) -- must be a POINTER
//                               to something with ItemIndex.
//    cbGPIBType                 uPAT_Function.cpp:2049, :2062 -- ->ItemIndex.
//    cbRs232Type                uPAT_Function.cpp:2061 -- ->ItemIndex.
//    edGPIBAddress              uPAT_Function.cpp:2050 (StrToIntDef(...->Text)),
//                               :2063 (->Text=IntToStr(...)).
//    edMaxTestTime              Command.cpp:3691 -- ->Text.
//    edInitialMaxTest           Command.cpp:3718 -- ->Text.
//    fShow                      Command.cpp:10245, :15033 -- plain `bool`,
//                               read inside a big `||` chain of form flags.
//  All 14 sites stay `#if 0` after this wave. Declaring these members does NOT
//  un-gate them -- each is its owning file's own separate, deliberate edit and
//  is outside this wave's write boundary.
//
//  ============================================================================
//  GATE REGISTER (17 -- 16 declared-with-transcript + 1 omitted entirely)
//  ============================================================================
//  Reason classes used below:
//     [W] SAFETY POLICY -- the body writes disk / sends a command / moves
//         hardware. Read-only wave => never live, regardless of reachability.
//     [L] LINK BOUNDARY -- the symbol's DEFINING .cpp is in a target
//         ht9045_forms does not link. ht9045_forms links exactly
//         vclcompat + ht9045_globals + ht9045_core (CMakeLists.txt:988-995);
//         it does NOT link ht9045_sm / ht9045_motor / ht9045_io / ht9045_db /
//         ht9045_comms. The FOUR verified forms->sm exceptions are
//         RecordProcess / ShowMyMessage / ShowErrorMessage (canary_support.cpp)
//         and MyDBIProcess (aHotPlateSubstrate.cpp) -- nothing else.
//     [M] MISSING MEMBER/TYPE -- no port exists for the member or type.
//     [T] TRANSITIVE -- calls a gated method.
//
//  (F-1) ctor's two guarded blocks   golden :42 and :46-66 -- gated INSIDE an
//        otherwise-ACTIVE ctor (statement-level gating; same precedent as
//        forms/fMonitor's gated `LoadTCPIPParament()` call inside a live
//        method). (a) golden :42 `InitcbDIOType(false);` is [T] on (F-2) AND
//        a static-init hazard: (F-2) scans a directory and can call
//        `Application->Terminate()`. (b) golden :46-66 is [M]+static-init:
//        `edInitialMaxTest->Hint` / `edMaxTestTime->Hint` /
//        `edStartDelayTime->Hint` -- `Hint` does NOT exist on
//        vclcompat::TControl (Controls.h:213-280, whole member list is
//        Visible/Enabled/hCtl/Tag/Left/Top + SetFocus/Click/BringToFront/
//        Push/Pull) and adding it is a shared-file edit this wave may not
//        make; and both blocks read `CUSTOMER_CODE` (ht9045_globals) from a
//        static-init constructor, which docs/KNOWLEDGE.md forbids after the
//        fLaserSensor incident (88 of 134 ctest binaries SEGFAULTed).
//        `cbGPIBType->Refresh()` (:58) is a second [M].
//  (F-2) InitcbDIOType        golden :69-106 (38L) -- [M]. :97-98
//        `Application->MessageBox(...)` / `Application->Terminate()`.
//        `Application` has NO port anywhere in this tree (scan over the 1,199
//        tracked sources, 20260828: the only hits are banner prose in
//        EJ1N/uDTME08Control.h:130-138 and FormsBootstrap.cpp:11 recording the
//        same absence). Independently, `Application->Terminate()` from a
//        static-init ctor would kill the process before main() -- which is
//        exactly why (F-1)a gates the call as well as this body.
//  (F-3) FormShow             golden :108-334 (227L) -- FIVE independent
//        reasons; any one alone is sufficient.
//        (a) [T] on (F-2) at :117 and on (F-5) ReadTestIFFile at :118 --
//            both unconditional, both before anything else.
//        (b) [L] `fSecurity->Insufficient(19,false)` (:141) and (27,false)
//            (:316): fSecurity's defining TU is cSecurity.cpp, target
//            ht9045_sm (CMakeLists.txt:2168).
//        (c) [L] `SW[SwPurgeAir].Enable` (:325): `SW[]` is
//            `extern class TMySwitch SW[MAX_SWITCH_ITEM]` (myswitch.h:43),
//            defined myswitch.cpp:46, target ht9045_io (CMakeLists.txt:1240).
//        (d) [L] `CheckTTLBoardBitMode()` (:148): body csystem.cpp:30645,
//            target ht9045_sm (CMakeLists.txt:2131).
//        (e) [M] `fMain->CheckCanChangeRealDummy()` (:130) is NOT a member of
//            the ported TfMain (forms/fMain.h -- grep this wave: fMain carries
//            SendMSG_CMD/BackupSetupFile but no CheckCanChangeRealDummy, no
//            CloseGpibProgram, no oldLastiTestMode); `fSCKART->sInfo_Stage`
//            (:266) is not a member of the ported TfSCKART (it lives on
//            Automation/SCK_ART_Remainder's own state struct instead);
//            `PageControl1->ActivePage` (:264/:268/:272) does not exist on
//            vclcompat::TPageControl (Controls.h:497, ActivePageIndex only);
//            `rbTemp->SetFocus()` (:115) IS fine (Controls.h:279).
//  (F-4) SaveSetupFile        golden :336-492 (157L) -- [W]. 60+
//        `WriteIniData(szDir, ...)` calls plus `MyForceDirectories(szDir)`
//        (:338) and `AddSpace(szDir)` (:491). This is the recipe writer; its
//        name is one of the 10 PRE-COMMITTED names, so it is DECLARED with
//        golden's exact `(AnsiString szDir, AnsiString S)` signature.
//  (F-5) ReadTestIFFile       golden :562-977 (416L, the largest body in the
//        file) -- [W] HIDDEN WRITE, x3, plus [L] and [M]. Despite the name
//        "Read", this function WRITES:
//            :576  WriteIniData(szDir, "Mode", "Tester Type", 1)   -- when the
//                  stored index exceeds the radio group's item count
//            :824  WriteIniData(szDir, "DIO", "TypeName", ...)     -- when the
//                  key is absent, after a FindFirstFile directory scan
//            :892  WriteIniData(szDir, "RS-232C", "BaudRate", ...) -- legacy
//                  index -> real baud-rate migration
//        and at :973 calls `ATKRecipeInfo->SaveFile()` (another writer).
//        THIS IS THE CASE THE TASK BRIEF ASKED ME TO CHECK BY READING THE
//        BODY: "ReadTestIFFile 若會回寫或建立檔案" -- it does, three times, so
//        it is NOT eligible to be live. Additional independent gates: [L]
//        `fShowBinSelect->ShowInitialString()` (:931, cShowBinSelect.cpp =
//        ht9045_sm, CMakeLists.txt:2162), [L] `CheckRs232StandardIni()` (:947,
//        (F-6) below), [M] `fTesterTCP->TimerTCPIPConnect` /
//        `->TimerProcessTCPData` / `->ClientSocket_TCPIP` (:584-615) -- this
//        wave's own forms/fTesterTCP facade deliberately does NOT declare
//        those three (see its FIELD LIST: `class TTimer` has no vclcompat
//        port at all), and [M] `fLotInfo->labTCPIPSimulate` (:586/:616) is not
//        a member of the ported TfLotInfo either. `fLotInfo->
//        SettsChipAdvVisible()` (:618) IS real (forms/fLotInfo.cpp:1790).
//  (F-6) CheckRs232StandardIni golden :496-558 (63L) -- [W]+[M]. Writes
//        FOUR keys into `D:\RS232Standard\System\Setup.ini` (:512/:524/:536/
//        :550) -- a SECOND product's config file, outside this tree entirely.
//        Also [M] `fMain->CloseGpibProgram(__FUNC__)` (:556), not a member of
//        the ported TfMain.
//  (F-7) DoIniDataToForm      golden :979-1124 (146L) -- [T] on (F-8). Its
//        LAST statement is `rgInterfaceTypeClick(this);` (:1123),
//        unconditional. Everything before it is a pure global->widget mirror
//        (TestIF_File -> this facade's own TEdit/TComboBox/TCheckBox fields)
//        and would have been safe; the transitive rule still applies, exactly
//        as forms/fHotPlate.h GATE (G-4) applies it to FormClose. This is the
//        single biggest missed opportunity of the wave and is called out as
//        such in the closing report -- un-gating (F-8) would immediately make
//        146 more golden lines live.
//  (F-8) rgInterfaceTypeClick golden :1139-1172 (34L) -- [L]+[T].
//        [L] `Barcode_Reader(bcTesterIF)` (:1142) -- body BarcodeReader.cpp,
//        target ht9045_sm (CMakeLists.txt:2215); same boundary
//        forms/fHotPlate.h GATE (G-5)(a) records for the same function.
//        [T] `ShowPageControl2(...)` (:1171), (F-9).
//  (F-9) ShowPageControl2     golden :1174-1183 -- [M].
//        `PageControl2->ActivePage=tsTemp[Index];` (:1180) --
//        vclcompat::TPageControl has `ActivePageIndex` but NO `ActivePage`
//        (Controls.h:497-503). The neighbouring `->ActivePageIndex=Index`
//        (:1181) and `tsTemp[i]->TabVisible` (:1178-1179) are both fine; the
//        one missing member gates the body, and adding `ActivePage` would be
//        a shared-file (vclcompat/Controls.h) edit this wave may not make.
//  (F-10) FormClose           golden :1197-1217 -- [T]+[M]+[L]. [T] on (F-5)
//        at :1199; [M] `fMain->oldLastiTestMode` (:1206) is not a TfMain
//        member; [L] `CheckTTLBoardBitMode()` (:1211, ht9045_sm) and [M]
//        `fMain->CloseGpibProgram(__FUNC__)` (:1212).
//  (F-11) ShowTTLState        golden :1220-1279 (60L) -- [M]+[L].
//        [M] `fDIOFrom->rgStartLogic/rgStartChannel/cbSignalType/rgBinLogic/
//            rgBinBitLength/rgBinDataType` (:1226-1259): there is NO
//            `TfDIOFrom` class and no `fDIOFrom` global anywhere in this tree
//            (DIOInterFaceCFG.{h,cpp} DO NOT EXIST in this tree at all --
//            verified this wave by direct stat AND by scanning every
//            add_library block in CMakeLists.txt; the only `fDIOFrom` hits
//            in the 1,199 tracked sources are two `#if 0`-gated
//            reads in Command.cpp:10243/:15031 and a generated layout table).
//        [L] `CheckTTLBoardBitMode()` (:1265, ht9045_sm).
//  (F-12) cbDIOTypeChange     golden :1281-1287 -- [M] `fDIOFrom->
//        GetDIOFileName()` / `->LoadData(S)` (same absence as (F-11)) and [T]
//        on (F-11) at :1286.
//  (F-13) spbSaveClick        golden :1302-1356 (55L) -- [W]+[L]+[M]. [W] on
//        (F-4) SaveSetupFile at :1316 and (F-5) ReadTestIFFile at :1326.
//        [L] `fSpeed->SearchRecipeParameter(...)` x6 (:1337-1342, cSpeed.cpp =
//        ht9045_sm :2178), `fYieldMonitoring->...` (:1348, uYieldMonitoring.cpp
//        = ht9045_sm :2101), `SetWorkParameter()` (:1355, cinitial.cpp =
//        ht9045_sm :1643), `EventReport(SECS_EVENT.SaveRecipe)` (:1324,
//        SECSGEM = ht9045_secsgem :1326).
//        [M] `fRPDefault->RP_Name/RP_Default/RP_Value` (:1331-1333, :1351) --
//        RPDefault.{h,cpp} do not exist in this tree at all (direct stat,
//        20260828).
//        `fMain->BackupSetupFile()` (:1354) and `fCleaning->
//        SearchRecipeParameter(...)` (:1343) ARE real and reachable; they are
//        not what gates this.
//  (F-14) sbtExitClick        golden :1358-1366 -- [L]. `CheckTTLBoardBitMode()`
//        (:1362, csystem.cpp:30645 = ht9045_sm). The rest
//        (`rgInterfaceType->ItemIndex`, `sbtExit->Down=false`, `Close()`)
//        would all have been safe -- this is a ONE-LINE gate on an otherwise
//        trivial handler, recorded plainly so a future ht9045_sm-reaching
//        wave can un-gate it cheaply.
//  (F-15) DoSetRPDefault      golden :1418-1442 -- [W]+[M]. [W] `WriteIniData(
//        szDir, aSection, aHintName, PEdit->Text)` (:1439) into
//        `D:\HT9045\IniData\RPDefault.ini`. [M] the whole traversal idiom
//        `TWinControl *PCtrl` / `PCtrl->ControlCount` / `PCtrl->Controls[iP]` /
//        `PEdit->Hint`: none of ControlCount / Controls[] / Hint exists on
//        vclcompat::TControl, and there is no `TWinControl` type.
//  (F-16) DoReplyDefaultToForm golden :1444-1473 -- [M] only (same traversal
//        idiom as (F-15)); it does NOT write. Recorded explicitly because it
//        is the ONE gated body in this file whose gate is purely a missing
//        type -- if a future wave adds a TWinControl traversal shim, this one
//        can go live without any safety argument.
//  (F-17) SearchRecipeParameter golden :1475-1514 (40L) -- [W]+[M]. [W]
//        `CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp)` (:1499)
//        WRITES THE DEFAULT BACK when the key is missing
//        (common.cpp:1567-1569) -- it is NOT the pure-read `ReadIniData`
//        (common.cpp:692); same finding forms/fHotPlate.h GATE (G-1) records.
//        [M] same TWinControl traversal as (F-15), plus `fRPDefault->RP_*`.
//  (F-18) FormShortCut        golden :1521-1528 -- OMITTED ENTIRELY, not even
//        declared. Its signature is `(TWMKey &Msg, bool &Handled)` and
//        `TWMKey` has ZERO port in this tree (scan 20260828: the only hits are
//        banner prose in cBinSel.cpp:4196, cSetUp.cpp:250 and
//        forms/fContact.h:489/:758/:774 recording the same absence). Same
//        disposition as forms/fContact.h GATE (X-27) and forms/fHotPlate.h
//        (G-12) for the same golden method shape.
//
//  ============================================================================
//  ACTIVE (21, one PARTIAL) -- reachability evidence
//  ============================================================================
//  The whole ACTIVE set rests on FOUR reachable dependencies, each verified
//  by locating the DEFINING .cpp and then its enclosing add_library block:
//    fQwertyKey->ShowQwertyKey(TControl*,int,int=0,bool=false,double=0,double=0)
//        forms/fQwertyKey.h:370, body forms/fQwertyKey.cpp -- target
//        ht9045_forms (CMakeLists.txt:699), i.e. the SAME target. 16 of the 21
//        ACTIVE bodies are nothing but one call to it.
//    OnlyNumberInPut / OnlyNumberAndDotInPut   common.h:403-404, body
//        common.cpp -- target ht9045_core (CMakeLists.txt:272), which
//        ht9045_forms links explicitly (CMakeLists.txt:988-995, added by
//        FW-SETUP-C2 for exactly this class of dependency).
//    N_DOUBLE / N_INTEGER / N_PORT             cmydef.h:289-297
//        (`extern const int`), body cmydef.cpp -- ht9045_globals.
//    CUSTOMER_CODE / CC_* / IniConfig / CosFunction / TestIF
//        cmydef.cpp / MachineType.h `#define`s / Config.cpp / CosFunction.cpp /
//        cprod.cpp -- all ht9045_globals.
//  Per-body notes worth having:
//    TFTestIF()                golden :39-67  ACTIVE-PARTIAL. Live = golden
//        :43-44 only (`ioldTestType=-1; ioldDIOType=-1;`), two writes to this
//        object's own ints. See (F-1) for the two gated blocks.
//    edMaxTestTimeKeyPress     golden :1126-1131 -- note it BOTH filters the
//        keypress AND pops the on-screen keypad, unconditionally.
//    AntiSignalCBoxClick       golden :1289-1292 -- the only ACTIVE body that
//        writes a GLOBAL (`TestIF.bAntiSignal`, cprod.cpp/ht9045_globals). In
//        this tree nothing else reads it back through this path, so the write
//        currently has no consumer; it is golden's own text and is kept.
//        See the NAME COLLISION warning at the top before reasoning about it.
//    cbRs232TypeChange         golden :1294-1300 -- own widgets only.
//    btTesterTCPShowClick      golden :1516-1519 -- `fTesterTCP->Show();`.
//        `fTesterTCP` is THIS WAVE'S OWN sibling facade (forms/fTesterTCP.h,
//        same target) and its `Show()` is a port-only no-op (that file's
//        DEVIATION D-2). So this body is live but its EFFECT is nil -- stated
//        plainly rather than counted as a working feature.
//    edInitialMaxTestClick     golden :1380-1403 (24L, the largest ACTIVE
//        body) -- a 5-way CUSTOMER_CODE / IniConfig.bVTESTFunction fan-out
//        choosing the keypad's numeric limits. All five globals are
//        ht9045_globals.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//      Inside the `#if 0` transcripts it is dropped too (the ONLY edit made to
//      the sliced golden text).
//  (D-2) `Key=NULL;` on a `char&` (golden :1129, :1136) -> `Key=0;`.
//      Faithful: golden's NULL is 0 here and assigning a pointer-null macro to
//      a char is a BCB6-ism. Same call as forms/fHotPlate.cpp:94.
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- golden inherits
//      `TForm::Close`; this facade has no TForm base (D-5). Used only by
//      GATED bodies ((F-13) spbSaveClick, (F-14) sbtExitClick), declared
//      anyway so those transcripts stay literal.
//  (D-4) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();`.
//      `TCloseAction` has no port; the body is GATED so the signature is free
//      either way. Matches forms/fPrecaution.h D-5 / forms/fMesSystem.h D-5.
//  (D-5) `TFTestIF` has NO base class.
//  (D-6) `DoSetRPDefault` / `DoReplyDefaultToForm` / `SearchRecipeParameter`
//      take golden's `TWinControl *PCtrl`; there is no `TWinControl` in this
//      port, so they are declared with `TControl *PCtrl` instead. All three
//      are GATED, so nothing binds to the choice yet -- it is recorded so a
//      future wave knows the declaration was ADAPTED, not copied.
//  (D-7) golden's file-scope `static bool bflag=false;` (cTesterIF.cpp:37) is
//      carried INSIDE the `#if 0` region next to its only two users
//      ((F-8) rgInterfaceTypeClick, (F-10) FormClose). Keeping it at live file
//      scope would be a defined-but-unused static (-Wunused-variable under
//      this target's -Wall -Wextra). Flagged because it is a real structural
//      difference from golden, not a formatting choice.
//
//  ============================================================================
//  FIELD LIST (dfm leaf names, per the FW rule "widget 存取改成 UI-state 欄位")
//  ============================================================================
//  DECLARED -- the 5 PRE-COMMITTED widgets, plus what an ACTIVE body touches:
//    rgInterfaceType   (TRadioGroup*) golden :18  -- PRE-COMMITTED x2
//    cbGPIBType        (TComboBox*)   golden :30  -- PRE-COMMITTED x2
//    cbRs232Type       (TComboBox*)   golden :207 -- PRE-COMMITTED, + ACTIVE
//                                     cbRs232TypeChange
//    edGPIBAddress     (TEdit*)       golden :31  -- PRE-COMMITTED x2
//    edMaxTestTime     (TEdit*)       golden :91  -- PRE-COMMITTED, + ACTIVE
//                                     edPowerSwitchDelayClick/edMaxBIOSWaitTimeClick
//    edInitialMaxTest  (TEdit*)       golden :92  -- PRE-COMMITTED
//    edSLTMaxTestTime  (TEdit*)       golden :249 -- ACTIVE edMinTestTimeClick
//    AntiSignalCBox    (TCheckBox*)   golden :25  -- ACTIVE AntiSignalCBoxClick
//    gbRs232BinCount   (TGroupBox*)   golden :44  -- ACTIVE cbRs232TypeChange
//    sbtExit           (TSpeedButton*) golden :35 -- GATED (F-14) only
//    spbSave           (TSpeedButton*) golden :34 -- GATED (F-13) only
//    fShow             (bool)         golden :302 -- PRE-COMMITTED x2
//    bIsResetRs232Standard (bool)     golden :303 -- written by GATED (F-6)
//    ioldTestType / ioldDIOType (int) golden :304-305 -- ACTIVE ctor
//  NOT DECLARED, deliberately: the other ~200 golden __published widgets
//  (cTesterIF.h:17-256). Nothing LIVE reads them; every GATED transcript that
//  names one is inside `#if 0`, so declaring them would be shape without
//  evidence. A wave that un-gates (F-7) DoIniDataToForm must add roughly 70 of
//  them first -- that is the real cost of that un-gate and it is stated here so
//  it is not discovered late.
//
//  ============================================================================
//  ZERO-WRITER FIELDS -- observable consequence, flagged per task rule 7
//  ============================================================================
//  All 6 PRE-COMMITTED widget fields are WRITTEN only by GATED (F-7)
//  DoIniDataToForm and READ only by still-gated sites in uPAT_Function.cpp /
//  Command.cpp. They therefore sit at their `new`-defaults (ItemIndex==0,
//  Text=="") for as long as (F-7) is gated. ItemIndex==0 is NOT a neutral
//  "unset": golden's `rgInterfaceType->ItemIndex==0` means TTL_MODE and is an
//  ACTIVELY MATCHED branch (golden :144, :1154, :1261, :1360). So un-gating
//  any consumer without also supplying (F-7)'s values would silently assert
//  "the tester link is TTL" -- the same value-provenance trap
//  docs/KNOWLEDGE.md "解 gate 前先查值從哪來" describes for tcHotPlate1==0.
//  `TestIF.bAntiSignal` is the mirror case: ACTIVE AntiSignalCBoxClick writes
//  it from a checkbox nothing ever checks, so it can only ever be written
//  false.
//
//  ============================================================================
//  ABSENCE-CLAIM PROVENANCE (trap #2: these expire)
//  ============================================================================
//  Every "0 hits" / "not a member" claim above was measured 20260828 05:14
//  with `git ls-files -- '*.cpp' '*.h' '*.hpp'` (1,199 files) plus a per-file
//  Python regex scan, and the add_library attributions by parsing every
//  add_library block in CMakeLists.txt. Re-run at closeout -- a sibling wave
//  landing a TfMain member or a vclcompat widget property would falsify
//  several of the [M] gates above.
// =============================================================================
#ifndef FORMS_FTESTERIF_H
#define FORMS_FTESTERIF_H

#include "forms/FormWidgets.h"     // AnsiString / TRadioGroup / TComboBox / TEdit / TCheckBox / TGroupBox / TSpeedButton
#include "vclcompat/ShiftState.h"  // TMouseButton / TShiftState (the *MouseDown handlers)

// ===========================================================================
//  TFTestIF -- non-VCL facade (golden cTesterIF.h:14-314,
//  `class TFTestIF : public TForm`; NO base class here, DEVIATION D-5).
//
//  ⚠ NOT the same thing as the `TestIF` / `TestIF_File` DATA globals
//  (cprod.h:2576-2577). See the NAME COLLISION block at the top of this file.
// ===========================================================================
class TFTestIF
{
public:
    TFTestIF();

    // -- golden __published widgets (dfm leaf names) ------------------------
    TRadioGroup  *rgInterfaceType  = new TRadioGroup();   // golden :18,  PRE-COMMITTED uPAT_Function.cpp:2048/:2060
    TComboBox    *cbGPIBType       = new TComboBox();     // golden :30,  PRE-COMMITTED uPAT_Function.cpp:2049/:2062
    TComboBox    *cbRs232Type      = new TComboBox();     // golden :207, PRE-COMMITTED uPAT_Function.cpp:2061
    TEdit        *edGPIBAddress    = new TEdit();         // golden :31,  PRE-COMMITTED uPAT_Function.cpp:2050/:2063
    TEdit        *edMaxTestTime    = new TEdit();         // golden :91,  PRE-COMMITTED Command.cpp:3691
    TEdit        *edInitialMaxTest = new TEdit();         // golden :92,  PRE-COMMITTED Command.cpp:3718
    TEdit        *edSLTMaxTestTime = new TEdit();         // golden :249
    TCheckBox    *AntiSignalCBox   = new TCheckBox();     // golden :25
    TGroupBox    *gbRs232BinCount  = new TGroupBox();     // golden :44
    TSpeedButton *spbSave          = new TSpeedButton();  // golden :34
    TSpeedButton *sbtExit          = new TSpeedButton();  // golden :35

    // -- golden public: User declarations -----------------------------------
    bool fShow                 = false;   // golden :302, PRE-COMMITTED Command.cpp:10245/:15033
    bool bIsResetRs232Standard = false;   // golden :303, Sam 20181219 : Handler 與 Rs232Standard 設定同步
    int  ioldTestType          = 0;       // golden :304, 0:TTL_MODE ; 1: GPIB_MODE ; 2: RS232_MODE
    int  ioldDIOType           = 0;       // golden :305

    // -- ACTIVE this wave (defined in forms/fTesterIF.cpp) -------------------
    void edMaxTestTimeKeyPress(TObject *Sender, char &Key);          // golden :1126-1131 ACTIVE
    void Edit1KeyPress(TObject *Sender, char &Key);                  // golden :1133-1137 ACTIVE
    void edMaxTestTimeMouseDown(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y);    // golden :1185-1189 ACTIVE
    void Edit1MouseDown(TObject *Sender, TMouseButton Button,
                        TShiftState Shift, int X, int Y);            // golden :1191-1195 ACTIVE
    void AntiSignalCBoxClick(TObject *Sender);                       // golden :1289-1292 ACTIVE
    void cbRs232TypeChange(TObject *Sender);                         // golden :1294-1300 ACTIVE
    void edMaxBinCountMouseDown(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y);    // golden :1368-1372 ACTIVE
    void edTempOffsetCountMouseDown(TObject *Sender, TMouseButton Button,
                                    TShiftState Shift, int X, int Y);// golden :1374-1378 ACTIVE
    void edInitialMaxTestClick(TObject *Sender);                     // golden :1380-1403 ACTIVE
    void edtInitialDelay_1Click(TObject *Sender);                    // golden :1405-1408 ACTIVE
    void edtInitStartDelayClick(TObject *Sender);                    // golden :1410-1416 ACTIVE
    void btTesterTCPShowClick(TObject *Sender);                      // golden :1516-1519 ACTIVE
    void edTCPIP_PortClick(TObject *Sender);                         // golden :1530-1533 ACTIVE
    void edPowerSwitchDelayClick(TObject *Sender);                   // golden :1535-1538 ACTIVE
    void edMaxBIOSWaitTimeClick(TObject *Sender);                    // golden :1540-1543 ACTIVE
    void edMinTestTimeClick(TObject *Sender);                        // golden :1545-1548 ACTIVE
    void edtAutoOnecycleHomStartTimeClick(TObject *Sender);          // golden :1550-1553 ACTIVE
    void edGPIBAddressClick(TObject *Sender);                        // golden :1555-1558 ACTIVE
    void edtInitialDecCount1Click(TObject *Sender);                  // golden :1560-1563 ACTIVE
    void edPurgeAirClick(TObject *Sender);                           // golden :1565-1568 ACTIVE
    void Close() {}                                                   // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fTesterIF.cpp inside `#if 0`. Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). ----------------------------------
    void FormShow(TObject *Sender);                                   // golden :108-334   GATE (F-3)
    void SaveSetupFile(AnsiString szDir, AnsiString S);               // golden :336-492   GATE (F-4), PRE-COMMITTED csystem.cpp:29301 / uPAT_Function.cpp:2069
    void CheckRs232StandardIni();                                     // golden :496-558   GATE (F-6)
    void ReadTestIFFile();                                            // golden :562-977   GATE (F-5), PRE-COMMITTED uTemp_Set.cpp:3365/:5384
    void DoIniDataToForm();                                           // golden :979-1124  GATE (F-7), PRE-COMMITTED csystem.cpp:29300
    void rgInterfaceTypeClick(TObject *Sender);                       // golden :1139-1172 GATE (F-8)
    void FormClose();                                                 // golden :1197-1217 GATE (F-10), DEVIATION D-4 no-param
    void cbDIOTypeChange(TObject *Sender);                            // golden :1281-1287 GATE (F-12)
    void spbSaveClick(TObject *Sender);                               // golden :1302-1356 GATE (F-13)
    void sbtExitClick(TObject *Sender);                               // golden :1358-1366 GATE (F-14)
    void DoSetRPDefault(TControl *PCtrl, int iSection);               // golden :1418-1442 GATE (F-15), DEVIATION D-6
    void DoReplyDefaultToForm(TControl *PCtrl, int iSection);         // golden :1444-1473 GATE (F-16), DEVIATION D-6
    void SearchRecipeParameter(TControl *PCtrl, int iSection);        // golden :1475-1514 GATE (F-17), DEVIATION D-6

    // FormShortCut(TWMKey&, bool&) -- golden :1521-1528, OMITTED ENTIRELY.
    // See GATE (F-18): `TWMKey` has zero port in this tree.

private:
    void ShowTTLState();                                              // golden :1220-1279 GATE (F-11), private (matches golden :290)
    void ShowPageControl2(int Index);                                 // golden :1174-1183 GATE (F-9),  private (matches golden :291)
    void InitcbDIOType(bool bAlarm);                                  // golden :69-106    GATE (F-2),  private (matches golden :292)
};

// AI(W906-FW3-TIF1) 20260828: TFTestIF/FTestIF were FREE tree-wide (see
// banner's measured provenance) -- declared here so the 14 pre-committed
// `#if 0` sites have something real to bind to once each owning file's OWN
// gate is separately lifted (NOT done by this wave).
//
// ⚠ `FTestIF` (this form pointer) is NOT `TestIF` / `TestIF_File`
// (cprod.h:2576-2577, SYSTEM_TEST_IF data globals). Read the NAME COLLISION
// block at the top of this file before using either.
extern TFTestIF *FTestIF;

#endif // FORMS_FTESTERIF_H
