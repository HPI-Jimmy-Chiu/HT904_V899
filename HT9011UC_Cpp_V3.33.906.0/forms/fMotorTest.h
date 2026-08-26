// =============================================================================
//  forms/fMotorTest.h  --  non-VCL facade for golden's TfMotorTest
//  (uMotorTest.h/.cpp) -- the manual motor-test / motor-database form
//
//  AI(W906-FW-MOTORTEST-W24) 20260826: new file, FW wave FW-MOTORTEST-W24.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/uMotorTest.h (386 lines) +
//  uMotorTest.cpp (2,377 lines) + uMotorTest.dfm -- all cp950, decoded with
//  `python -c "io.open(p, encoding='cp950').read()"`, 0 U+FFFD, this wave
//  (20260826).
//
//  ROLE
//  ----
//  TfMotorTest is golden's engineering screen for driving ONE motor at a time
//  by hand: pick a motor from the per-motor row list (pnlMotor, one
//  TMotorTestClass row per motor), then Jog +/-, Move +/- by an interval,
//  Home, Loop-Move between two taught positions, edit the motor's
//  speed/accel/soft-limit parameters live, toggle motor power / servo, and
//  (tab 4) edit + save system\Mot_Table.csv itself.
//
//  ============================================================================
//  *** SAFETY POSTURE -- READ THIS BEFORE TOUCHING ANY GATE IN THIS FILE ***
//  ============================================================================
//  This form is the single most motion-dense form in the corpus: of its 93
//  golden method definitions, 52 issue a REAL MACHINE COMMAND -- JogP/JogN,
//  MotorMove, Gali_MotMove, Gali_SingalHome, PCIL132_StopMotor,
//  Gali_Command("ST"), StopAllMotor, SetSpeed, ServoOnOff, DoMotorPowerOn,
//  SW[SwServerON].On(), GaliMotorServoOff, PCIL132_SetPos(0), ResetMNet,
//  InitMotor, InitialMotorParameter -- or WRITE a motor's protection
//  parameters (PSoftLimitP/PSoftLimitN/PJogHighSpeed/PHomeLowSpeed/SetRange/
//  SetAccDataBase/SetDecDataBase).
//
//  Per this project's hard rule (CLAUDE.md "安全關鍵變更"), EVERY one of those
//  52 is OUT OF SCOPE for this wave and is recorded in the GATE REGISTER
//  below as `#if 0`.  They are NOT translated, NOT stubbed, and NOT
//  "translated but commented as unreachable".  A `#if 0` block that a future
//  wave un-gates is a DELIBERATE, REVIEWED decision that must come with an
//  interlock story; a translated-but-inert body is a landmine, because the
//  day someone wires the web write path the motor moves.
//
//  What this wave DOES land is the read-only half: the widget/field model,
//  the per-motor row class, the parameter-DISPLAY path, the pure predicates,
//  the UI-state toggles, the string/grid plumbing, and the Mot_Table.csv
//  LOAD (not the save).  See WAVE SCOPE.
//  ============================================================================
//
//  DENOMINATOR -- MEASURED THIS WAVE, NOT INHERITED
//  --------------------------------------------------------------------------
//  93 distinct method definitions in golden uMotorTest.cpp, measured after
//  stripping `/* */` and `//` comments:
//      python: t = re.sub(r'/\*.*?\*/','',src,flags=re.S)
//              t = re.sub(r'//[^\n]*','',t)
//              re.findall(r'^[^\n]*?\b(TfMotorTest|TMotorTestClass)::(~?\w+)\s*\(', t, re.M)
//      -> 91 `TfMotorTest::` + 2 `TMotorTestClass::` = 93
//  (Two file-scope FREE functions, `SaveAsCSV` golden :1749-1787 and
//  `SaveAsCSV_Kaichen` golden :1982-2027, are correctly NOT in that 93 -- and
//  are not delivered here either, because their only callers are gated.)
//
//  THIS WAVE: 35 of 93 delivered (37.6% of golden method DEFINITIONS in
//  uMotorTest.cpp -- not of lines, and not of the form's behaviour, most of
//  which is the gated motion half).  58 gated.
//
//  THIS IS A NEW CLASS -- NO EXISTING STAND-IN, NO ODR CONFLICT
//  --------------------------------------------------------------------------
//  Verified this wave (20260826 14:00-14:20):
//    * `Grep "TfMotorTest|fMotorTest|TMotorTestClass" --glob *.{h,cpp}` over
//      HT9011UC_Cpp_V3.33.906.0: 0 declarations, and every COMPILED-looking
//      use turns out to be preprocessor-dead:
//        - Command.cpp:10239 (`fMotorTest->fShow`) is inside the `#if 0` that
//          opens at Command.cpp:10238 -- TfMain::SettingsIsWindowOpened's
//          33-term OR is gated wholesale.
//        - Command.cpp:15027 is inside the `#if 0` at Command.cpp:15026.
//        - csystem.cpp:840-847 (`fMotorTest->fShow` / `->bSingleHome` /
//          `->iSingleHomeIndex`) is inside the `#if 0 // GOLDEN VERBATIM`
//          that opens at csystem.cpp:598 and covers golden csystem.cpp:
//          16711-19100.  Confirmed by an #if/#endif depth scan this wave, not
//          by eyeballing -- there are nested `#ifdef DEBUG_TRY_CATCH` pairs in
//          between that make the region boundary non-obvious.
//      Everything else is a `docs/` note or a `tools/dfm2rc` data artifact.
//    * `Grep "TMotorTestClass|TfMotorTestLabel|TfMotorTestEdit|
//       TfMotorTestCheckBox|TfMotorTestPanel|TfMotorTestLed|TfMotorTestTimer|
//       TfMotorTestBevel" --glob *.{h,cpp}`: 0 hits -- every type name this
//      header introduces is unclaimed.
//  So this file UNBLOCKS those three gates; they live outside this wave's
//  write boundary and stay CLOSED.  Both greps were re-run at wave close
//  (20260826 14:38) with the same result -- absence claims go stale under
//  parallel waves (docs/KNOWLEDGE.md), and two sibling waves DID land files
//  in forms/ while this one ran.
//
//  WHAT THIS FILE UNBLOCKS (all outside this wave's write boundary, all left
//  CLOSED -- listed so the next wave does not have to rediscover them):
//    * Command.cpp SettingsIsWindowOpened / :15026 block -- the `fMotorTest->
//      fShow` OR-terms now have a real `bool fShow;` behind them.
//    * csystem.cpp:840-847 -- `fMotorTest->bSingleHome` / `->iSingleHomeIndex`
//      now exist.  ⚠ THAT BLOCK IS A HOMING DRIVER (it calls
//      ProcessSingleMotorHome); un-gating it is a motion decision, not a
//      type-availability one.
//    * forms/fTeach.h:434-435 records `btnMotorTestClick` (golden uteach.cpp
//      :2436-2440) as blocked on `fMotorTest->ShowModal()` "a sibling wave is
//      creating forms/fMotorTest.* right now".  Both `fMotorTest` and
//      `ShowModal()` now exist (ShowModal is the documented offline no-op,
//      DEVIATION D-8), so that dependency is satisfiable.
//
//  FORM CLASS SHAPE: plain non-VCL class, NOT TForm-derived -- same posture
//  as every translated form facade in this tree.  Widget stand-ins store
//  data, they do not render ("logic faithful, rendering = web").
//
//  CTOR / Init() SPLIT (task-mandated; forms/fQwertyKey.h + cSetUp.cpp
//  convention)
//  --------------------------------------------------------------------------
//  `TfMotorTest::TfMotorTest()` stuffs FIELDS ONLY: the BCB6-TObject
//  zero-fill equivalents (golden never initialises fShow / bSingleHome /
//  LoopTask / dwLoopCount / Average / iUseAxis / iSelectRow.. in its ctor and
//  relies on TObject's zero-fill; plain C++ has none) plus the .dfm-sourced
//  grid dimensions.  Golden's ENTIRE ctor body (uMotorTest.cpp:148-389 --
//  the 164 TMotorTestClass push_backs, the row-layout loop, the ColWidths
//  writes) moves VERBATIM into explicit `Init()`.
//    WHY THIS SPLIT IS LOAD-BEARING HERE SPECIFICALLY: golden's ctor body
//    reads ~40 machine-configuration globals (InOutArmPickerUseMotor,
//    LOAD_Z_USE_MOTOR[], USE_ROTATE_KIT, CosFunction.bTrayOCR, ...).  Those
//    are cross-TU objects; running that body from a static initialiser is the
//    exact shape that cost this tree 88 SEGFAULTs out of 134 ctests when
//    fLaserSensor's ctor called InitLaserEdtList() with elLaser still NULL
//    (docs/KNOWLEDGE.md).  Nothing here constructs TfMotorTest at static-init
//    time (`fMotorTest` is a raw pointer, golden's own zero state), so with
//    the body in Init() there is no SIOF exposure at all.
//
//  ============================================================================
//  WAVE SCOPE -- all 93 golden methods listed; ACTIVE ones were each read
//  line-by-line in full before translating.
//  ============================================================================
//   ACTIVE (35)
//    TMotorTestClass ctor        golden :32-128   ACTIVE, GATEs M-Parent/M-OnClick
//    TMotorTestClass::SetPosition golden :130-143 ACTIVE
//    TfMotorTest ctor           (PORT split)      fields only, see above
//    Init()                     golden :148-389  ACTIVE
//    UpdateMotorParameter       golden :655-669  ACTIVE
//    ShowMotorSelect            golden :671-698  ACTIVE
//    UpdatePreasureScreen       golden :700-732  ACTIVE (see G-DeadAvg)
//    btnSetPosPClick            golden :1079-1086 ACTIVE
//    btnSetPosNClick            golden :1088-1095 ACTIVE
//    IsMotorCanRun              golden :1280-1298 ACTIVE (pure EMG predicate)
//    FormClose                  golden :1347-1362 ACTIVE, GATE M-IniWrite
//    btnRangeClick              golden :1451-1456 ACTIVE
//    btnRateClick               golden :1458-1463 ACTIVE
//    SpeedButton4Click          golden :1465-1469 ACTIVE
//    SpeedButton5Click          golden :1471-1475 ACTIVE
//    SpeedButton8Click          golden :1477-1481 ACTIVE
//    SpeedButton6Click          golden :1483-1487 ACTIVE
//    SpeedButton7Click          golden :1489-1493 ACTIVE
//    SpeedButton9Click          golden :1495-1499 ACTIVE
//    ck00Click                  golden :1673-1685 ACTIVE
//    edtSpeedClick              golden :1713-1716 ACTIVE
//    palExitClick               golden :1727-1730 ACTIVE
//    FormDestroy                golden :1732-1747 ACTIVE, GATE M-Destroy
//    edDelayTimeMouseDown       golden :2039-2043 ACTIVE
//    edPitechMouseDown          golden :2045-2049 ACTIVE
//    rgAxisClick                golden :2121-2133 ACTIVE
//    sbtReloadClick             golden :2135-2179 ACTIVE (Mot_Table.csv READ)
//    strngrdMotorDataSelectCell golden :2181-2186 ACTIVE
//    btnAddMotorClick           golden :2188-2197 ACTIVE
//    btnDeleteMotorClick        golden :2199-2212 ACTIVE
//    btnModifyClick             golden :2214-2238 ACTIVE
//    strngrdMotorDataDblClick   golden :2270-2273 ACTIVE
//    edtMotorRateClick          golden :2346-2349 ACTIVE
//    edtMotorRangeClick         golden :2351-2354 ACTIVE
//    LockAllButton              golden :2356-2364 ACTIVE
//    AllBtnUp                   golden :2366-2375 ACTIVE
//
//   GATED -- MOTION / SAFETY-CRITICAL (52).  GATE id (S-nn) per entry; each
//   `#if 0` in the .cpp names the exact golden call that makes it unsafe.
//    (S-01) DoLoopMove                golden :393-624   MotorMove/Gali_MotMove
//    (S-02) UpdateMotorLed            golden :626-653   PCIL132_StopMotor
//    (S-03) lM00Click                 golden :734-760   SetSpeed
//    (S-04) lpA00Click                golden :762-789   SetSpeed
//    (S-05) scrlbrMotorSpeedScroll    golden :791-810   SetSpeed
//    (S-06) sbMotorTest_JogNMouseDown golden :812-848   JogN/Gali_JogN
//    (S-07) SpeedButton26MouseUp      golden :850-860   StopMotor/Gali "ST"
//    (S-08) sbMotorTest_JogPMouseDown golden :862-898   JogP/Gali_JogP
//    (S-09) sbMotorTest_JogPMouseUp   golden :900-910   StopMotor/Gali "ST"
//    (S-10) Timer1Timer               golden :912-983   drives DoLoopMove/DoGaliHome/SetSpeed
//    (S-11) FormShow                  golden :985-1077  DoMotorPowerOn/SwServerON/GaliMotorServoOff
//    (S-12) btnGoSoftPClick           golden :1097-1103 MotorMove to soft limit
//    (S-13) btnGoSoftNClick           golden :1105-1111 MotorMove to soft limit
//    (S-14) btnHomeClick              golden :1113-1174 Home / ResetPos / StopMotor
//    (S-15) strngrdMotorSelectCell    golden :1176-1222 WRITES every motor parameter
//    (S-16) sbMotorTest_MoveNClick    golden :1224-1250 MotorMove
//    (S-17) sbMotorTest_MovePClick    golden :1252-1278 MotorMove
//    (S-18) btnLoopMoveClick          golden :1300-1345 InitMOTParameter/StopMotor
//    (S-19) btnSetRateClick           golden :1364-1372 SetRate + InitMotor
//    (S-20) btnSetRangeClick          golden :1374-1382 SetRange + InitMotor
//    (S-21) BitBtn1Click              golden :1384-1401 copies speeds+soft limits between motors
//    (S-22) btnHighSpeedClick         golden :1403-1409 writes PJogHighSpeed
//    (S-23) btnLowSpeedClick          golden :1411-1417 writes PJogLowSpeed
//    (S-24) btnHomeHighClick          golden :1419-1425 writes PHomeHighSpeed
//    (S-25) btnHomeLowClick           golden :1427-1433 writes PHomeLowSpeed
//    (S-26) btnSoftPPosClick          golden :1435-1441 writes PSoftLimitP (protection!)
//    (S-27) btnSoftNPosClick          golden :1443-1449 writes PSoftLimitN (protection!)
//    (S-28) ScrollBar2Scroll          golden :1501-1506 SetSpeed(MTestZ1)
//    (S-29) ScrollBar3Scroll          golden :1508-1513 SetSpeed(MTestZ2)
//    (S-30) SpeedButton44Click        golden :1515-1519 MotorMove(MTestZ1)
//    (S-31) SpeedButton45Click        golden :1521-1525 MotorMove(MTestZ1)
//    (S-32) SpeedButton50Click        golden :1527-1531 MotorMove(MTestZ2)
//    (S-33) SpeedButton51Click        golden :1533-1537 MotorMove(MTestZ2)
//    (S-34) SpeedButton46MouseDown    golden :1539-1543 Motor->JogN()
//    (S-35) SpeedButton46MouseUp      golden :1545-1549 PCIL132_StopMotor
//    (S-36) SpeedButton47MouseDown    golden :1551-1555 Motor->JogP()
//    (S-37) SpeedButton47MouseUp      golden :1557-1561 PCIL132_StopMotor
//    (S-38) SpeedButton52MouseDown    golden :1563-1567 Motor->JogN()
//    (S-39) SpeedButton52MouseUp      golden :1569-1573 PCIL132_StopMotor
//    (S-40) SpeedButton53MouseDown    golden :1575-1579 Motor->JogP()
//    (S-41) SpeedButton53MouseUp      golden :1581-1585 PCIL132_StopMotor
//    (S-42) edtSpeedChange            golden :1587-1603 SetSpeed
//    (S-43) btnGoClick                golden :1605-1619 MotorMove
//    (S-44) btnMotorPowerClick        golden :1621-1645 motor power relay + servo
//    (S-45) btnStopClick              golden :1647-1659 StopAllMotor
//    (S-46) btnServoOffClick          golden :1661-1671 ServoOnOff
//    (S-47) DoGaliHome                golden :1687-1693 Gali_SingalHome
//    (S-48) btnReloadMotorDataClick   golden :1695-1711 InitialMotorParameter + SetPos(0)
//    (S-49) btResetMNetClick          golden :1718-1725 ResetMNet (motion network power-cycle)
//    (S-50) LightScale                golden :1795-1978 Home + MotorMove sweep
//    (S-51) Timer2Timer               golden :2031-2037 drives LightScale
//    (S-52) BitBtn2Click              golden :2096-2100 arms the LightScale motion loop
//
//   GATED -- WRITES A MACHINE CONFIGURATION FILE (1)
//    (W-01) sbUpdateClick             golden :2240-2268 `sList->SaveToFile(MotTablePath)`
//        i.e. it REWRITES system\Mot_Table.csv, the file cinitial.cpp reads to
//        build every motor's axis mapping/gear ratio.  Out of a read-only
//        wave by definition, and high-risk under CLAUDE.md's runtime-config
//        rule.  The LOAD half (sbtReloadClick) IS delivered.
//
//   GATED -- MISSING PORT-SIDE CAPABILITY (5).  Not motion; simply not
//   translatable today without inventing a cross-file shim (forbidden).
//    (X-01) strngrdMotorDataMouseDown golden :2275-2284 `->MouseToCell(...)`
//    (X-02) strngrdMotorDataDragOver  golden :2286-2300 `->MouseToCell(...)`
//    (X-03) strngrdMotorDataDragDrop  golden :2302-2344 `->MouseToCell(...)`
//        vclcompat::TStringGrid deliberately has NO MouseToCell/BeginDrag
//        ("STILL NO rendering, NO Options/OnDrawCell/OnSelectCell/
//        RowHeights/MouseToCell", vclcompat/StringGrid.h:19-20) -- a
//        pixel->cell hit test has no headless meaning.  Same gate shape and
//        same reason as forms/fTemp_Set.h GATE (G-Grid) (`sgTjMap->
//        MouseToCell` gating the whole of sgTjMapMouseDown).  NOTE the tree
//        also holds the OTHER choice (forms/fContactCT.h:247 declares a
//        `MouseToCell` that returns 0,0 on its own bespoke grid stand-in);
//        that shape is rejected here because it silently makes every drag
//        land on cell (0,0), which for DragDrop means "reorder column 0"
//        rather than "do nothing".
//    (X-04) btnSaveLogLightScaleDataClick golden :2051-2094
//    (X-05) BitBtn3Click              golden :2102-2119
//        Both need `ShowMessage(...)` (VCL Dialogs.hpp).  Grepped this wave:
//        the port tree has NO ShowMessage declaration in any *.h (only
//        `ShowMyMessage`, canary_support.h:80, and the untranslated
//        `ShowMyMessageBox_YES_NO` noted in Automation/AGV_E84.h:58).  They
//        additionally both WRITE .csv exports (D:\LightScale) through the
//        two golden free functions, and their sole data source is the gated
//        LightScale sweep (S-50), so delivering them would produce a
//        write-path entry point whose input is永遠 empty.
//
//  ============================================================================
//  GATE REGISTER -- gates INSIDE otherwise-ACTIVE methods
//  ============================================================================
//  (M-Parent)  `X->Parent = <panel>;` -- TMotorTestClass ctor, golden :49,
//      :62, :71, :80, :90, :102, :110, :120.  The facade has no window
//      hierarchy, so Parent has no offline consumer; and the six that read
//      `fMotorTest->pnlMotor` would additionally dereference the `fMotorTest`
//      GLOBAL, which is a NULL raw pointer until something creates the form.
//      Gating them is what makes TMotorTestClass safe to construct from
//      Init() regardless of whether `fMotorTest` has been assigned yet.
//  (M-OnClick) `X->OnClick = fMotorTest-><handler>;` -- TMotorTestClass ctor,
//      golden :58, :67, :76, :87.  Stock vclcompat widgets carry no OnClick
//      delegate slot (cSetUp.cpp GATE(G-Delegate), the tree-wide idiom).
//      Handler BODIES are translated where they are read-only; wiring is the
//      web write-path layer's job ("event handler 本體翻譯但不接線").
//      NOTE all three targets here (lM00Click / lpA00Click / ck00Click) --
//      only ck00Click is delivered; lM00Click/lpA00Click are S-03/S-04.
//  (M-IniWrite) the `WriteIniData(sPath, ...)` loop -- FormClose, golden
//      :1352-1359.  Writes system\MotorTest.ini.  This wave is read-only
//      direction; and `asSystemPath` on a development box still points at the
//      PRODUCTION machine's system directory (the same coupling that cost
//      this tree system\Gerneral.ini on 20260817).  `fShow=false;` and
//      `PauseUT150Polling=false;` are kept ACTIVE -- they are plain state.
//  (M-Destroy) `MyDBIProcess(...)` + `LogSoftwareOffTime(...)` --
//      FormDestroy, golden :1744/:1746.  Both are ht9045_sm-layer symbols
//      OUTSIDE the ht9045_forms link diet (vclcompat + ht9045_globals +
//      ht9045_core, CMakeLists.txt:714-715); forms/fQwertyKey.h:68-77
//      analysed exactly this and split its FormDestroy into a ROOT file
//      instead.  This wave may only add forms/fMotorTest.{h,cpp}, so the two
//      calls are gated rather than moved.  The vector teardown -- the part
//      that actually matters -- is ACTIVE.  (forms/fLotInfo.cpp:58 takes the
//      other route, a TU-local `extern void MyDBIProcess(...)`; recorded so
//      the next wave can pick either deliberately.)
//
//  ============================================================================
//  GOLDEN NOTES (faithful; recorded for the ledger)
//  ============================================================================
//  (G-VecIndex) LOAD-BEARING INVARIANT, measured this wave.  `MotorTestClass`
//      is built by 164 sequential `push_back`s (golden :151-361) while many
//      call sites index it BY MOTOR ID (`MotorTestClass[ActiveIndex]`, golden
//      :412/:511/:1084/:1093; `MotorTestClass[MTestY1]`.. in ck00Click,
//      golden :1675-1683).  Those two index spaces coincide only because the
//      push_back order happens to be exactly motor id 0..163.  Machine-checked
//      this wave against the ported `const int M*` definitions
//      (cmydef.cpp:2334-2417): 164 push_backs, and `vectorIndex == motorId`
//      for ALL 164 -- zero divergence.  164 also equals TOTAL_MOTOR
//      (cmydef.h:46).  Golden itself does NOT rely on the coincidence in the
//      one place it would be easy to get wrong: the row-layout loop
//      (golden :368-385) SEARCHES for `MotorTestClass[j]->index==i` instead of
//      indexing directly.  Anyone who adds, removes or reorders a push_back
//      silently breaks every by-motor-id site -- including the gated motion
//      ones.  Verbatim, invariant recorded.
//  (G-OwnerInInitList) golden :32
//      `TMotorTestClass::TMotorTestClass(int MotNo, bool bView) : TComponent(Owner)`
//      passes `Owner` to the base ctor, and `Owner` is NOT a parameter of this
//      constructor -- it resolves to TComponent's own `Owner` property, read
//      through a `this` whose base is not yet constructed.  That is undefined
//      behaviour in golden.  The port has no TComponent base at all, so the
//      construct simply disappears; recorded because a reader diffing the two
//      will notice the missing initialiser and should not "restore" it.
//  (G-DeadAvg) UpdatePreasureScreen, golden :713-731: `static double
//      T[2][256];` is DECLARED and READ (`sum0+=T[0][i]`, :723-724) but never
//      WRITTEN anywhere in uMotorTest.cpp -- grepped every `T[` in the file
//      this wave, exactly 3 hits (:713 decl, :723, :724 reads).  So Edit16 and
//      Edit102 always display " 0.000", and the `p` counter that rolls 0..49
//      feeds nothing.  Translated verbatim; the port makes it no deader.
//  (G-ZeroFill) golden's TMotorTestClass leaves labName/edPos1/edPos2/
//      cbUsing/MotorPanel/MotorLabel/MotorLed[]/MotorHomeLed UNASSIGNED when
//      `bView==false` (early `return` at golden :41-42, Steven 20240822
//      "沒用到的馬達不new").  In BCB6 that is safe only because TObject::
//      NewInstance zero-fills the whole instance, so they read NULL.  Plain
//      C++ has no such fill, so this port initialises all of them to NULL
//      explicitly -- reproducing golden's observable state, not changing it.
//      Every golden read of those members is already guarded by `->Visible`.
//  (G-Gali0) UpdatePreasureScreen and the Timer1 read path call
//      `MOT[..].Gali_ReadPos()`.  That is a READ (allowed in this wave), but
//      in THIS TREE its body is the degraded stub `long TMyMotor::
//      Gali_ReadPos() { return 0; }` (Motor/mymotor.cpp:1525) -- the whole
//      Galil layer is TODO(W6-Galil) per Motor/mymotor.h:22-24.  So the
//      Index-Z position readouts show 0 offline.  Pre-existing property of
//      the port's motor layer, not a deviation introduced here.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention
//      (vclcompat/vcl_compat.h neutralises it; forms/fQwertyKey.h does the
//      same).
//  (D-2) Widget ctor OWNER argument dropped: golden `new TLabel(fMotorTest->
//      pnlMotor)` becomes `new TfMotorTestLabel()`.  The stand-ins take no
//      owner, and keeping the argument would re-introduce the `fMotorTest`
//      NULL dereference that (M-Parent) exists to remove.  CONSEQUENCE, stated
//      rather than hidden: in golden the VCL owner frees these widgets when
//      the form dies; here nothing would.  So TMotorTestClass gets a port-only
//      destructor that deletes exactly the widgets it allocated (NULL-safe, so
//      the `bView==false` rows are fine).  That is the faithful equivalent of
//      owner-frees, and it is safe because in this tree those widgets have
//      exactly one owner.  Golden's TMotorTestClass has no destructor.
//  (D-3) `clWindow` (ShowMotorSelect, golden :693-694) has no vclcompat
//      definition -- the shared TColor block (vclcompat/LedCore.h:52-63,
//      shared verbatim with TrayCore.h/BtnPanelCore.h) stops at clBtnFace.
//      Defined in forms/fMotorTest.cpp in an UNNAMED namespace with the real
//      VCL value (Graphics.hpp `clWindow = TColor(0x80000005)`, the same
//      system-colour form as the existing `clBtnFace = TColor(0x8000000F)`).
//      Internal linkage, so it cannot collide with a future
//      `vclcompat::clWindow`, and golden's spelling stays verbatim.
//      ⚠ CROSS-WAVE NOTE, verified 20260826 14:38: the parallel wave
//      forms/fTrayAssignment.h:225-226/:715 GATES three of its own statements
//      on the claim "`clWindow` has ZERO compiled definition tree-wide" (its
//      grep was `--glob *.h` -> 1 prose hit).  That claim is now STALE AS
//      WORDED -- but its GATE is still correct, because the constant below is
//      in an unnamed namespace and is therefore invisible outside
//      forms/fMotorTest.cpp.  It does NOT un-gate fTrayAssignment (T4).  The
//      right consolidation is a shared `vclcompat::clWindow` in the guarded
//      TColor block (vclcompat/LedCore.h:52-63); that file is outside this
//      wave's write boundary, so it is left for the wave that owns vclcompat.
//      Do not "fix" fTrayAssignment by pointing it at this one.
//  (D-4) `TBevel` has no vclcompat type; `Bevel5`/`Bevel6` are decorative and
//      no translated statement touches them, so they are declared as an empty
//      `TfMotorTestBevel : public TControl` purely so the published member
//      list stays complete.
//  (D-5) `TTimer` has no vclcompat type; `Timer1`/`Timer2` become
//      `TfMotorTestTimer { bool Enabled; }` -- the established per-form shape
//      (forms/fLotInfo.h:1167, forms/fShowBinSelect.h:880, forms/fTemp_Set.h
//      :426).  Only `->Enabled` is ever touched, and only from gated methods.
//  (D-6) `TALed` becomes `TfMotorTestLed` (plain UI-state fields), NOT
//      `vclcompat::LedCore`.  LedCore exposes Set*/Get* methods, not the
//      `->Value=`/`->Blink=`/`->LEDStyle=`/`->TrueColor=` PROPERTY spelling
//      golden uses, and wrapping it in assignment proxies is the exact shape
//      that already produced a silent no-op bug in this tree (the
//      proxy-to-proxy `operator=` trap, docs/KNOWLEDGE.md).  FormWidgets.h's
//      `TfLedValue { bool Value; }` is the precedent for the plain-field
//      choice; the enum type `vclcompat::TLEDStyle` IS reused from LedCore.h.
//  (D-7) `.dfm`-sourced initial values that translated code READS are set in
//      the port ctor (fBinSel.h "dims from dfm into ctor" precedent):
//      strngrdMotor ColCount=2 / RowCount=11 / DefaultColWidth=80
//      (uMotorTest.dfm:1873-1877) and strngrdMotorData DefaultColWidth=75 /
//      FixedCols=0 (uMotorTest.dfm:3664-3665).  strngrdMotor's dims are
//      LOAD-BEARING: vclcompat::TStringGrid silently IGNORES out-of-range
//      Cells writes (StringGrid.h:74-80), so at the 5x5 default
//      UpdateMotorParameter's `Cells[1][6..10]` writes would vanish.
//  (D-8) `ShowModal()` / `Close()` are permanent offline no-ops (forms/
//      fTemp_Set.h:575-584 / forms/fQwertyKey.h:105-122 precedent).  Only
//      `Close()` is reached from delivered code (palExitClick).
//  (D-9) TWO golden PARAMETER TYPES HAVE NO PORT AND ARE DROPPED FROM THE
//      SIGNATURE (not faked):
//        * `TCloseAction &Action` (FormClose, golden :1347-1348).  Established
//          decision -- forms/fCounterClear.h:71-72 records "TCloseAction has
//          no port anywhere in this tree", and forms/fHandlerSys.h:343 drops
//          it the same way.  golden's FormClose body never reads Action.
//        * `TDragState State` (strngrdMotorDataDragOver, golden :2286-2287).
//          Grepped this wave: 0 hits for TDragState anywhere in the port tree
//          outside this file.  golden's body never reads State either (it
//          decides purely on Sender + the hit-tested cell), so dropping it
//          loses nothing -- and the method is GATE (X-02) regardless.
//      `TScrollCode` DOES have a port but is NOT in the global namespace
//      (vclcompat/ScrollBar.h:83, deliberately kept namespaced because
//      handlerlog.h:122 owns global `::TScrollBar`), so the two Scroll
//      handlers spell it `vclcompat::TScrollCode`.
//
//  ============================================================================
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt is outside the
//  write boundary).  SUGGESTED landing: forms/fMotorTest.cpp -> the
//  `ht9045_forms` source list (CMakeLists.txt:654-713 block, next to
//  forms/fQwertyKey.cpp).  Everything ACTIVE here fits that target's diet
//  (vclcompat + ht9045_globals + ht9045_core): the motor layer it reads
//  (Motor/mymotor.h MOT[], HTMotor), Sen[]/SW[], cmydef/cprod globals,
//  vclcompat widgets, and `fQwertyKey` itself already live at or below that
//  layer (ATC/ATCInterface.cpp:2301 and cConfiguration.cpp:137 are compiled
//  `fQwertyKey->ShowQwertyKey` call sites, so that edge is proven, not
//  assumed).  The two ht9045_sm symbols that would have broken the diet are
//  GATE (M-Destroy).
//  ⚠ REMEMBER: registering this file makes the linker able to PULL it.  Until
//  something references a symbol in it, none of the 58 `#if 0` blocks can be
//  reached -- but that is a property of the gates, not of the registration.
// =============================================================================
#ifndef FORMS_FMOTORTEST_H
#define FORMS_FMOTORTEST_H

#include "vclcompat/vcl_compat.h"    // AnsiString / TObject / TStringList at global scope
#include "vclcompat/Controls.h"      // TLabel/TPanel/TEdit/TCheckBox/TComboBox/... (global `using`s at :533-551)
#include "vclcompat/StringGrid.h"    // vclcompat::TStringGrid (Cells/RowCount/ColCount/ColWidths)
#include "vclcompat/ScrollBar.h"     // vclcompat::TScrollBar -- MUST stay qualified (handlerlog.h:122 has a global TScrollBar)
#include "vclcompat/LedCore.h"       // vclcompat::TColor / TLEDStyle / LEDHorizontal / cl* consts
#include "vclcompat/BtnPanelCore.h"  // vclcompat::TBevelShim / bvNone (golden MotorPanel->BevelOuter)
#include "vclcompat/ShiftState.h"    // TShiftState / TMouseButton (global `using`s at :128-140)
#include "myTimer.h"                 // TQPF_Timer (golden tLoopMoveTimer/tAvgLoopMoveTimer) + <windows.h> DWORD

#include <vector>

// forms/FormWidgets.h already spells this `using`; duplicate identical
// using-declarations across headers are legal C++, not an ODR conflict (its
// own note, FormWidgets.h:88-98).  Declared locally so this header stays
// independent of every other form's header.
using vclcompat::TStringGrid;

// ---------------------------------------------------------------------------
//  TfMotorTestBevel -- golden `TBevel *Bevel5/*Bevel6` (uMotorTest.h:47/:83).
//  DEVIATION (D-4): decorative only, no translated statement touches it.
// ---------------------------------------------------------------------------
class TfMotorTestBevel : public TControl
{
public:
    virtual ~TfMotorTestBevel() {}
};

// ---------------------------------------------------------------------------
//  TfMotorTestTimer -- golden `TTimer *Timer1/*Timer2` (uMotorTest.h:118/
//  :205).  DEVIATION (D-5): only ->Enabled is ever touched.
// ---------------------------------------------------------------------------
class TfMotorTestTimer : public TObject
{
public:
    bool Enabled;
    TfMotorTestTimer() : Enabled(false) {}
    virtual ~TfMotorTestTimer() {}
};

// ---------------------------------------------------------------------------
//  TfMotorTestLed -- golden `TALed *` (uMotorTest.h:31-32 MotorLed[10]/
//  MotorHomeLed, :162-182 ALed1..ALed10).  DEVIATION (D-6): plain UI-state
//  fields, not vclcompat::LedCore.  Top/Left/Visible/Tag come from TControl.
//  Defaults are the real TALed design-time defaults (aled.pas: Value false,
//  Blink false, LEDStyle LEDSmall, TrueColor clLime) -- golden's ctor
//  overwrites LEDStyle/TrueColor explicitly for every instance it creates.
// ---------------------------------------------------------------------------
class TfMotorTestLed : public TControl
{
public:
    bool                  Value;
    bool                  Blink;
    vclcompat::TLEDStyle  LEDStyle;
    vclcompat::TColor     TrueColor;
    TfMotorTestLed()
        : Value(false), Blink(false),
          LEDStyle(vclcompat::LEDSmall), TrueColor(vclcompat::clLime) {}
    virtual ~TfMotorTestLed() {}
};

// ---------------------------------------------------------------------------
//  TfMotorTestLabel -- golden `TLabel *labName/*MotorLabel` (uMotorTest.h:
//  25/:30).  vclcompat::TLabel carries Caption/Color/Left/Top/Visible/Tag;
//  golden additionally writes ->Height/->Width/->Name and ->Font->{Color,
//  Size,Name}, so those are added here (compose, don't fork vclcompat --
//  forms/fDynamicTemp.h:311 precedent).
// ---------------------------------------------------------------------------
class TfMotorTestLabel : public TLabel
{
public:
    int        Height;
    int        Width;
    AnsiString Name;
    TFont     *Font;
    TfMotorTestLabel() : Height(0), Width(0), Font(new TFont()) {}
    virtual ~TfMotorTestLabel() { delete Font; }
};

// ---------------------------------------------------------------------------
//  TfMotorTestEdit -- golden `TEdit *edPos1/*edPos2` (uMotorTest.h:26-27).
//  Adds Height/Width/Name/Color/Font on top of vclcompat::TEdit's Text.
//  (TEdit has no Color of its own, so this declares rather than shadows.)
// ---------------------------------------------------------------------------
class TfMotorTestEdit : public TEdit
{
public:
    int        Height;
    int        Width;
    int        Color;
    AnsiString Name;
    TFont     *Font;
    TfMotorTestEdit() : Height(0), Width(0), Color(0), Font(new TFont()) {}
    virtual ~TfMotorTestEdit() { delete Font; }
};

// ---------------------------------------------------------------------------
//  TfMotorTestCheckBox -- golden `TCheckBox *cbUsing` (uMotorTest.h:28).
//  Checked/Caption come from vclcompat::TCheckBox.
// ---------------------------------------------------------------------------
class TfMotorTestCheckBox : public TCheckBox
{
public:
    int        Height;
    int        Width;
    AnsiString Name;
    TfMotorTestCheckBox() : Height(0), Width(0) {}
    virtual ~TfMotorTestCheckBox() {}
};

// ---------------------------------------------------------------------------
//  TfMotorTestPanel -- golden `TPanel *MotorPanel` (uMotorTest.h:29).
//  Caption/Color come from vclcompat::TPanel; golden also writes
//  ->Height/->Width/->Name/->BevelOuter.
// ---------------------------------------------------------------------------
class TfMotorTestPanel : public TPanel
{
public:
    int                    Height;
    int                    Width;
    AnsiString             Name;
    vclcompat::TBevelShim  BevelOuter;
    TfMotorTestPanel() : Height(0), Width(0), BevelOuter(vclcompat::bvNone) {}
    virtual ~TfMotorTestPanel() {}
};

class TfMotorTest;   // TMotorTestClass's golden ctor references the form global

// ===========================================================================
//  TMotorTestClass -- golden uMotorTest.h:20-37 (`: public TComponent`,
//  "Stven 20120823 : Home元件改用Vector新增").  One row of the per-motor
//  list: name label, two taught-position edits, an "in the all-motor set"
//  checkbox, and a status-LED strip.
//
//  PORT: no TComponent base (see GOLDEN NOTE G-OwnerInInitList).  Widget
//  pointers are NULL-initialised to reproduce BCB6's TObject zero-fill (see
//  GOLDEN NOTE G-ZeroFill) and freed by a port-only dtor (DEVIATION D-2).
// ===========================================================================
class TMotorTestClass
{
    public:
        int                   index;
        bool                  Visible;
        TfMotorTestLabel     *labName;
        TfMotorTestEdit      *edPos1;
        TfMotorTestEdit      *edPos2;
        TfMotorTestCheckBox  *cbUsing;
        TfMotorTestPanel     *MotorPanel;
        TfMotorTestLabel     *MotorLabel;
        TfMotorTestLed       *MotorLed[10];
        TfMotorTestLed       *MotorHomeLed;

        TMotorTestClass(int MotNo, bool bView=true);   // golden :32-128
        ~TMotorTestClass();                            // PORT-ONLY, DEVIATION D-2
        void        SetPosition(int Top, int Left);    // golden :130-143
        bool        AllMotMoveFlag;

        // PORT-ONLY: the destructor above OWNS the widgets, so an implicit
        // copy would double-free them.  Golden's TComponent base is
        // non-copyable for the same structural reason; vclcompat::TStringGrid
        // (StringGrid.h:143-147) states the identical rationale.  Deleted
        // rather than left implicit so the mistake is a compile error.
        TMotorTestClass(const TMotorTestClass&) = delete;
        TMotorTestClass& operator=(const TMotorTestClass&) = delete;
};

// ===========================================================================
//  TfMotorTest -- non-VCL facade (golden uMotorTest.h:39-380,
//  `class TfMotorTest : public TForm`).
//
//  Widget members below keep golden's ORDER and golden's .dfm leaf NAMES
//  verbatim (uMotorTest.h:42-241), so a reader can diff the two lists
//  line-for-line.  Members are allocated with in-class initialisers -- the
//  established shape in this tree (forms/fTemp_Set.h:1446, forms/
//  fShowBinSelect.h:880) -- which is safe here because nothing constructs
//  TfMotorTest at static-init time (`fMotorTest` is a raw pointer).
// ===========================================================================
class TfMotorTest
{
public:
    // -- golden __published, uMotorTest.h:42-241 -----------------------------
    TPageControl            *PageControl1     = new TPageControl();
    TTabSheet               *TabSheet6        = new TTabSheet();
    TPanel                  *Panel15          = new TPanel();
    TTabSheet               *TabSheet7        = new TTabSheet();
    TPanel                  *Panel24          = new TPanel();
    TfMotorTestBevel        *Bevel5           = new TfMotorTestBevel();
    TSpeedButton            *SpeedButton44    = new TSpeedButton();
    TSpeedButton            *SpeedButton45    = new TSpeedButton();
    TSpeedButton            *SpeedButton46    = new TSpeedButton();
    TSpeedButton            *SpeedButton47    = new TSpeedButton();
    TLabel                  *Label82          = new TLabel();
    TLabel                  *Label83          = new TLabel();
    TLabel                  *Label84          = new TLabel();
    TLabel                  *Label85          = new TLabel();
    TLabel                  *Label87          = new TLabel();
    TLabel                  *Label88          = new TLabel();
    TLabel                  *Label79          = new TLabel();
    TLabel                  *Label80          = new TLabel();
    TLabel                  *Label81          = new TLabel();
    TLabel                  *Label97          = new TLabel();
    TSpeedButton            *SpeedButton4     = new TSpeedButton();
    TSpeedButton            *SpeedButton5     = new TSpeedButton();
    TLabel                  *Label102         = new TLabel();
    TSpeedButton            *SpeedButton8     = new TSpeedButton();
    TLabel                  *Label105         = new TLabel();
    TLabel                  *Label106         = new TLabel();
    TLabel                  *Label107         = new TLabel();
    vclcompat::TScrollBar   *ScrollBar2       = new vclcompat::TScrollBar();
    TComboBox               *ComboBox4        = new TComboBox();
    TEdit                   *Edit14           = new TEdit();
    TEdit                   *Edit16           = new TEdit();
    TEdit                   *Edit18           = new TEdit();
    TEdit                   *edtSpeedIndex1   = new TEdit();
    TEdit                   *Edit3            = new TEdit();
    TEdit                   *Edit4            = new TEdit();
    TEdit                   *Edit5            = new TEdit();
    TEdit                   *Edit6            = new TEdit();
    TEdit                   *Edit12           = new TEdit();
    TEdit                   *Edit13           = new TEdit();
    TPanel                  *Panel25          = new TPanel();
    TPanel                  *Panel26          = new TPanel();
    TfMotorTestBevel        *Bevel6           = new TfMotorTestBevel();
    TSpeedButton            *SpeedButton50    = new TSpeedButton();
    TSpeedButton            *SpeedButton51    = new TSpeedButton();
    TSpeedButton            *SpeedButton52    = new TSpeedButton();
    TSpeedButton            *SpeedButton53    = new TSpeedButton();
    TLabel                  *Label86          = new TLabel();
    TLabel                  *Label89          = new TLabel();
    TLabel                  *Label90          = new TLabel();
    TLabel                  *Label91          = new TLabel();
    TLabel                  *Label92          = new TLabel();
    TLabel                  *Label93          = new TLabel();
    TLabel                  *Label98          = new TLabel();
    TLabel                  *Label99          = new TLabel();
    TLabel                  *Label100         = new TLabel();
    TLabel                  *Label101         = new TLabel();
    TSpeedButton            *SpeedButton6     = new TSpeedButton();
    TSpeedButton            *SpeedButton7     = new TSpeedButton();
    TLabel                  *Label103         = new TLabel();
    TSpeedButton            *SpeedButton9     = new TSpeedButton();
    TLabel                  *Label108         = new TLabel();
    TLabel                  *Label109         = new TLabel();
    TLabel                  *Label110         = new TLabel();
    vclcompat::TScrollBar   *ScrollBar3       = new vclcompat::TScrollBar();
    TComboBox               *ComboBox5        = new TComboBox();
    TEdit                   *Edit24           = new TEdit();
    TEdit                   *Edit102          = new TEdit();
    TEdit                   *Edit103          = new TEdit();
    TEdit                   *edtSpeedIndex2   = new TEdit();
    TEdit                   *Edit7            = new TEdit();
    TEdit                   *Edit8            = new TEdit();
    TEdit                   *Edit10           = new TEdit();
    TEdit                   *Edit11           = new TEdit();
    TEdit                   *Edit15           = new TEdit();
    TEdit                   *Edit17           = new TEdit();
    TPanel                  *Panel27          = new TPanel();
    TfMotorTestTimer        *Timer1           = new TfMotorTestTimer();
    TEdit                   *Edit2            = new TEdit();
    TPanel                  *palExit          = new TPanel();
    TPanel                  *Panel2           = new TPanel();
    TPanel                  *Panel23          = new TPanel();
    TSpeedButton            *btnHighSpeed     = new TSpeedButton();
    TSpeedButton            *btnHomeHigh      = new TSpeedButton();
    TSpeedButton            *btnHomeLow       = new TSpeedButton();
    TSpeedButton            *btnLowSpeed      = new TSpeedButton();
    TSpeedButton            *btnSoftPPos      = new TSpeedButton();
    TSpeedButton            *btnSoftNPos      = new TSpeedButton();
    TSpeedButton            *btnRange         = new TSpeedButton();
    TSpeedButton            *btnRate          = new TSpeedButton();
    TSpeedButton            *btnReloadMotorData = new TSpeedButton();
    TSpeedButton            *btnSetRate       = new TSpeedButton();
    TSpeedButton            *btnSetRange      = new TSpeedButton();
    TEdit                   *edtMotorRate     = new TEdit();
    TEdit                   *edtMotorRange    = new TEdit();
    TButton                 *btResetMNet      = new TButton();
    TPanel                  *Panel20          = new TPanel();
    TLabel                  *Label38          = new TLabel();
    TSpeedButton            *sbMotorTest_JogN = new TSpeedButton();
    TSpeedButton            *sbMotorTest_JogP = new TSpeedButton();
    TLabel                  *Label40          = new TLabel();
    TLabel                  *Label41          = new TLabel();
    TSpeedButton            *sbMotorTest_MoveN = new TSpeedButton();
    TSpeedButton            *sbMotorTest_MoveP = new TSpeedButton();
    TSpeedButton            *btnLoopMove      = new TSpeedButton();
    TSpeedButton            *btnSetPosP       = new TSpeedButton();
    TSpeedButton            *btnSetPosN       = new TSpeedButton();
    TLabel                  *Label39          = new TLabel();
    TSpeedButton            *btnHome          = new TSpeedButton();
    TSpeedButton            *btnGoSoftP       = new TSpeedButton();
    TSpeedButton            *btnGoSoftN       = new TSpeedButton();
    TLabel                  *Label112         = new TLabel();
    TLabel                  *Label114         = new TLabel();
    TLabel                  *Label115         = new TLabel();
    TLabel                  *Label116         = new TLabel();
    TLabel                  *Label117         = new TLabel();
    TLabel                  *Label118         = new TLabel();
    TLabel                  *Label119         = new TLabel();
    TLabel                  *Label120         = new TLabel();
    TLabel                  *Label121         = new TLabel();
    TLabel                  *Label122         = new TLabel();
    TfMotorTestLed          *ALed1            = new TfMotorTestLed();
    TfMotorTestLed          *ALed2            = new TfMotorTestLed();
    TfMotorTestLed          *ALed3            = new TfMotorTestLed();
    TfMotorTestLed          *ALed4            = new TfMotorTestLed();
    TfMotorTestLed          *ALed5            = new TfMotorTestLed();
    TfMotorTestLed          *ALed6            = new TfMotorTestLed();
    TfMotorTestLed          *ALed7            = new TfMotorTestLed();
    TfMotorTestLed          *ALed8            = new TfMotorTestLed();
    TfMotorTestLed          *ALed9            = new TfMotorTestLed();
    TLabel                  *Label123         = new TLabel();
    TLabel                  *Label1           = new TLabel();
    TLabel                  *lblLoopCount     = new TLabel();
    TLabel                  *lblAvgTime       = new TLabel();
    TLabel                  *Label4           = new TLabel();
    TLabel                  *Label5           = new TLabel();
    TSpeedButton            *btnGo            = new TSpeedButton();
    TSpeedButton            *btnServoOff      = new TSpeedButton();
    TSpeedButton            *btnMotorPower    = new TSpeedButton();
    TLabel                  *lblJogPTime      = new TLabel();
    TLabel                  *lblJogNTime      = new TLabel();
    TfMotorTestLed          *ALed10           = new TfMotorTestLed();
    TLabel                  *Label10          = new TLabel();
    TLabel                  *Label12          = new TLabel();
    TPanel                  *pnlMotorAlias    = new TPanel();
    TComboBox               *cbbInterval      = new TComboBox();
    TComboBox               *cbbLoopWaitTime  = new TComboBox();
    vclcompat::TScrollBar   *scrlbrMotorSpeed = new vclcompat::TScrollBar();
    TEdit                   *edtCommandPos    = new TEdit();
    TEdit                   *edtSpeed         = new TEdit();
    TEdit                   *edtHomeOffset    = new TEdit();
    TRadioGroup             *select           = new TRadioGroup();
    TPanel                  *pnlEncoderPos    = new TPanel();
    TPanel                  *pnlMotor         = new TPanel();
    TBitBtn                 *BitBtn1          = new TBitBtn();
    TComboBox               *cbbMotorName     = new TComboBox();
    // DEVIATION (D-7): .dfm ColCount=2 / RowCount=11 (uMotorTest.dfm:
    // 1873/:1877).  LOAD-BEARING -- see the D-7 note.
    TStringGrid             *strngrdMotor     = new TStringGrid(2, 11);
    TCheckBox               *cbUsing          = new TCheckBox();
    TLabel                  *labName          = new TLabel();
    TEdit                   *Edit9            = new TEdit();
    TEdit                   *Edit26           = new TEdit();
    TPanel                  *Panel4           = new TPanel();
    TTabSheet               *TabSheet1        = new TTabSheet();
    TTabSheet               *ts1              = new TTabSheet();
    TfMotorTestTimer        *Timer2           = new TfMotorTestTimer();
    TEdit                   *edDelayTime      = new TEdit();
    TEdit                   *edPitech         = new TEdit();
    TRadioGroup             *rgMoveType       = new TRadioGroup();
    TBitBtn                 *BitBtn2          = new TBitBtn();
    TMemo                   *Memo1            = new TMemo();
    TBitBtn                 *BitBtn3          = new TBitBtn();
    TRadioGroup             *rgAxis           = new TRadioGroup();
    TSpeedButton            *btnSaveLogLightScaleData = new TSpeedButton();
    TLabel                  *InArmX_Go        = new TLabel();
    TLabel                  *lbl1             = new TLabel();
    TLabel                  *lbl2             = new TLabel();
    TLabel                  *lbl3             = new TLabel();
    TLabel                  *lbl4             = new TLabel();
    TLabel                  *lbl5             = new TLabel();
    TLabel                  *lbl6             = new TLabel();
    TLabel                  *lbl7             = new TLabel();
    TMemo                   *mmo1             = new TMemo();
    TMemo                   *mmo2             = new TMemo();
    TMemo                   *mmo3             = new TMemo();
    TMemo                   *mmo4             = new TMemo();
    TMemo                   *mmo5             = new TMemo();
    TMemo                   *mmo6             = new TMemo();
    TMemo                   *mmo7             = new TMemo();
    TMemo                   *mmo8             = new TMemo();
    TLabel                  *lblRealSpeed     = new TLabel();
    TTabSheet               *tsMotorDatabase  = new TTabSheet();
    TPanel                  *Panel1           = new TPanel();
    TSpeedButton            *sbtReload        = new TSpeedButton();
    TSpeedButton            *sbUpdate         = new TSpeedButton();
    TStringGrid             *strngrdMotorData = new TStringGrid();
    TSpeedButton            *btnAddMotor      = new TSpeedButton();
    TSpeedButton            *btnDeleteMotor   = new TSpeedButton();
    TSpeedButton            *btnModify        = new TSpeedButton();
    TLabel                  *lblMotorName     = new TLabel();
    TPanel                  *pnlStop          = new TPanel();
    TSpeedButton            *btnStop          = new TSpeedButton();

    // -- golden event handlers, uMotorTest.h:242-343.  BODIES translated
    //    where read-only; NOT wired to anything (task rule 3).  `TObject
    //    *Sender` / TMouseButton / TShiftState keep golden's signatures --
    //    vclcompat/ShiftState.h supplies the latter two.
    void lM00Click(TObject *Sender);                                            // golden :734-760   GATE (S-03)
    void lpA00Click(TObject *Sender);                                           // golden :762-789   GATE (S-04)
    void scrlbrMotorSpeedScroll(TObject *Sender,
          vclcompat::TScrollCode ScrollCode, int &ScrollPos);                              // golden :791-810   GATE (S-05)
    void sbMotorTest_JogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :812-848   GATE (S-06)
    void SpeedButton26MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :850-860   GATE (S-07)
    void sbMotorTest_JogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :862-898   GATE (S-08)
    void sbMotorTest_JogPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :900-910   GATE (S-09)
    void Timer1Timer(TObject *Sender);                                          // golden :912-983   GATE (S-10)
    void FormShow(TObject *Sender);                                             // golden :985-1077  GATE (S-11)
    void btnSetPosPClick(TObject *Sender);                                      // golden :1079-1086 ACTIVE
    void btnSetPosNClick(TObject *Sender);                                      // golden :1088-1095 ACTIVE
    void btnGoSoftPClick(TObject *Sender);                                      // golden :1097-1103 GATE (S-12)
    void btnGoSoftNClick(TObject *Sender);                                      // golden :1105-1111 GATE (S-13)
    void btnHomeClick(TObject *Sender);                                         // golden :1113-1174 GATE (S-14)
    void strngrdMotorSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);                                           // golden :1176-1222 GATE (S-15)
    void sbMotorTest_MoveNClick(TObject *Sender);                               // golden :1224-1250 GATE (S-16)
    void sbMotorTest_MovePClick(TObject *Sender);                               // golden :1252-1278 GATE (S-17)
    void btnLoopMoveClick(TObject *Sender);                                     // golden :1300-1345 GATE (S-18)
    void FormClose(TObject *Sender);                                            // golden :1347-1362 ACTIVE + GATE (M-IniWrite); TCloseAction dropped, DEVIATION (D-9)
    void btnSetRateClick(TObject *Sender);                                      // golden :1364-1372 GATE (S-19)
    void btnSetRangeClick(TObject *Sender);                                     // golden :1374-1382 GATE (S-20)
    void BitBtn1Click(TObject *Sender);                                         // golden :1384-1401 GATE (S-21)
    void btnHighSpeedClick(TObject *Sender);                                    // golden :1403-1409 GATE (S-22)
    void btnLowSpeedClick(TObject *Sender);                                     // golden :1411-1417 GATE (S-23)
    void btnHomeHighClick(TObject *Sender);                                     // golden :1419-1425 GATE (S-24)
    void btnHomeLowClick(TObject *Sender);                                      // golden :1427-1433 GATE (S-25)
    void btnSoftPPosClick(TObject *Sender);                                     // golden :1435-1441 GATE (S-26)
    void btnSoftNPosClick(TObject *Sender);                                     // golden :1443-1449 GATE (S-27)
    void btnRangeClick(TObject *Sender);                                        // golden :1451-1456 ACTIVE
    void btnRateClick(TObject *Sender);                                         // golden :1458-1463 ACTIVE
    void SpeedButton4Click(TObject *Sender);                                    // golden :1465-1469 ACTIVE
    void SpeedButton5Click(TObject *Sender);                                    // golden :1471-1475 ACTIVE
    void SpeedButton8Click(TObject *Sender);                                    // golden :1477-1481 ACTIVE
    void SpeedButton6Click(TObject *Sender);                                    // golden :1483-1487 ACTIVE
    void SpeedButton7Click(TObject *Sender);                                    // golden :1489-1493 ACTIVE
    void SpeedButton9Click(TObject *Sender);                                    // golden :1495-1499 ACTIVE
    void ScrollBar2Scroll(TObject *Sender,
          vclcompat::TScrollCode ScrollCode, int &ScrollPos);                              // golden :1501-1506 GATE (S-28)
    void ScrollBar3Scroll(TObject *Sender,
          vclcompat::TScrollCode ScrollCode, int &ScrollPos);                              // golden :1508-1513 GATE (S-29)
    void SpeedButton44Click(TObject *Sender);                                   // golden :1515-1519 GATE (S-30)
    void SpeedButton45Click(TObject *Sender);                                   // golden :1521-1525 GATE (S-31)
    void SpeedButton50Click(TObject *Sender);                                   // golden :1527-1531 GATE (S-32)
    void SpeedButton51Click(TObject *Sender);                                   // golden :1533-1537 GATE (S-33)
    void SpeedButton46MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1539-1543 GATE (S-34)
    void SpeedButton46MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1545-1549 GATE (S-35)
    void SpeedButton47MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1551-1555 GATE (S-36)
    void SpeedButton47MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1557-1561 GATE (S-37)
    void SpeedButton52MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1563-1567 GATE (S-38)
    void SpeedButton52MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1569-1573 GATE (S-39)
    void SpeedButton53MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1575-1579 GATE (S-40)
    void SpeedButton53MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :1581-1585 GATE (S-41)
    void edtSpeedChange(TObject *Sender);                                       // golden :1587-1603 GATE (S-42)
    void btnGoClick(TObject *Sender);                                           // golden :1605-1619 GATE (S-43)
    void btnMotorPowerClick(TObject *Sender);                                   // golden :1621-1645 GATE (S-44)
    void btnStopClick(TObject *Sender);                                         // golden :1647-1659 GATE (S-45)
    void btnServoOffClick(TObject *Sender);                                     // golden :1661-1671 GATE (S-46)
    void ck00Click(TObject *Sender);                                            // golden :1673-1685 ACTIVE
    void btnReloadMotorDataClick(TObject *Sender);                              // golden :1695-1711 GATE (S-48)
    void edtSpeedClick(TObject *Sender);                                        // golden :1713-1716 ACTIVE
    void btResetMNetClick(TObject *Sender);                                     // golden :1718-1725 GATE (S-49)
    void palExitClick(TObject *Sender);                                         // golden :1727-1730 ACTIVE
    void FormDestroy(TObject *Sender);                                          // golden :1732-1747 ACTIVE + GATE (M-Destroy)
    void Timer2Timer(TObject *Sender);                                          // golden :2031-2037 GATE (S-51)
    void edDelayTimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :2039-2043 ACTIVE
    void edPitechMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);                                     // golden :2045-2049 ACTIVE
    void btnSaveLogLightScaleDataClick(TObject *Sender);                        // golden :2051-2094 GATE (X-04)
    void BitBtn2Click(TObject *Sender);                                         // golden :2096-2100 GATE (S-52)
    void BitBtn3Click(TObject *Sender);                                         // golden :2102-2119 GATE (X-05)
    void rgAxisClick(TObject *Sender);                                          // golden :2121-2133 ACTIVE
    void sbtReloadClick(TObject *Sender);                                       // golden :2135-2179 ACTIVE
    void strngrdMotorDataSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);                                           // golden :2181-2186 ACTIVE
    void btnAddMotorClick(TObject *Sender);                                     // golden :2188-2197 ACTIVE
    void btnDeleteMotorClick(TObject *Sender);                                  // golden :2199-2212 ACTIVE
    void btnModifyClick(TObject *Sender);                                       // golden :2214-2238 ACTIVE
    void sbUpdateClick(TObject *Sender);                                        // golden :2240-2268 GATE (W-01)
    void strngrdMotorDataDblClick(TObject *Sender);                             // golden :2270-2273 ACTIVE
    void strngrdMotorDataMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);                 // golden :2275-2284 GATE (X-01)
    void strngrdMotorDataDragOver(TObject *Sender,
          TObject *Source, int X, int Y, bool &Accept);                          // golden :2286-2300 GATE (X-02); TDragState dropped, DEVIATION (D-9)
    void strngrdMotorDataDragDrop(TObject *Sender,
          TObject *Source, int X, int Y);                                       // golden :2302-2344 GATE (X-03)
    void edtMotorRateClick(TObject *Sender);                                    // golden :2346-2349 ACTIVE
    void edtMotorRangeClick(TObject *Sender);                                   // golden :2351-2354 ACTIVE

private:    // golden uMotorTest.h:344-350 (User declarations)
    int SelMotSpeed;

    int iSelectRow;
    int iSelectCol;
    int iDragCol;
    int iDragRow;

public:     // golden uMotorTest.h:352-379 (User declarations)
    TfMotorTest();                  // PORT: fields only -- golden ctor body is Init()
    ~TfMotorTest();                 // PORT-ONLY: frees the widget stand-ins (DEVIATION D-2 rationale)
    void Init();                    // golden ctor body :148-389

    // PORT-ONLY: same reason as TMotorTestClass's -- every member above is an
    // OWNING raw pointer freed by the destructor, so an implicit copy would
    // double-free.  Golden's TForm base is non-copyable.
    TfMotorTest(const TfMotorTest&) = delete;
    TfMotorTest& operator=(const TfMotorTest&) = delete;

    int ActiveIndex;
    std::vector<TMotorTestClass *> MotorTestClass;   // Stven 20120831 : MotorTest元件改用Vector新增
    void DoLoopMove();                               // golden :393-624   GATE (S-01)
    void UpdateMotorParameter();                     // golden :655-669   ACTIVE
    int LoopTask;
    bool fShow;
    void UpdatePreasureScreen();                     // golden :700-732   ACTIVE
    void UpdateMotorLed();                           // golden :626-653   GATE (S-02)
    void ShowMotorSelect(int Index, int Attr);       // golden :671-698   ACTIVE
    bool bSingleHome;
    int  iSingleHomeIndex;
//    DWORD dwStartTickCount;                        // golden :365-366, commented out in golden
//    DWORD dwNowTickCount;
    DWORD dwLoopCount;
    bool IsMotorCanRun(bool);                        // golden :1280-1298 ACTIVE
    bool DoGaliHome(int Index);                      // golden :1687-1693 GATE (S-47)

    void LightScale(bool Reset);                     // golden :1795-1978 GATE (S-50)
    int iUseAxis;
    //void ProcessRotater();                         // golden :373 (Steven 20100831 : 多了塊魚)

    TQPF_Timer tLoopMoveTimer;
    TQPF_Timer tAvgLoopMoveTimer;
    DWORD Average;
    void LockAllButton(bool bLock);                  // golden :2356-2364 ACTIVE
    void AllBtnUp();                                 // golden :2366-2375 ACTIVE

    // DEVIATION (D-8): golden TForm::Close()/ShowModal() -- permanent offline
    // no-ops (no window, no modal loop).  Only Close() is reached from
    // delivered code (palExitClick, golden :1729).
    void Close()     {}
    void ShowModal() {}
};

// golden uMotorTest.h:382 -- `extern PACKAGE TfMotorTest *fMotorTest;`
// (PACKAGE glue dropped; raw pointer, NULL until something creates the form,
// which is golden's own pre-CreateForm state).
extern TfMotorTest *fMotorTest;

#endif // FORMS_FMOTORTEST_H
