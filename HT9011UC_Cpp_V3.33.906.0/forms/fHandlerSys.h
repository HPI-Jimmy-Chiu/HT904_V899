// =============================================================================
//  forms/fHandlerSys.h  --  non-VCL facade for golden's THandlerSystem
//  (HandlerSys.h/.cpp)
//
//  AI(W906-FW3-HandlerSys-WA) 20260819: new file, batch-5 dual-form
//  display-side wave (cStartCondition + HandlerSys). Golden ref:
//  HT9011UC_Code_V3.33.906.0_20260618/HandlerSys.h (515 lines) + .cpp
//  (1,376 lines, 44 methods; cp950 -- decoded with `python3 -c
//  "open(...,'rb').read().decode('cp950')"`, 0 U+FFFD, this wave) + .dfm.
//  No prior forms/fHandlerSys.h or same-named stand-in existed before this
//  wave (grepped 20260819: only tools/dfm2rc/* generator artifacts + a few
//  passing mentions in csystem.cpp comments, no C++ class/global).
//
//  ROLE
//  ----
//  THandlerSystem is golden's "Handler System Setup" dialog -- the machine-
//  IDENTITY / installed-hardware-options editor (which motion card, which IO
//  card, which optional modules -- OCR/2D-barcode/ATC/RFID/ESD-monitor/
//  vibration-motor/tri-temperature/... -- are physically installed on THIS
//  machine, plus the safe-door sensor enable set). Unlike cStartCondition
//  (mostly write-oriented), this form's dominant bucket IS display: its two
//  biggest methods (LoaderSystemSet, 378 lines; SaveSystemSet, 520 lines)
//  split CLEANLY into a pure-read display refresh and a pure-write persist --
//  no read/write-combo trap like cStartCondition's ReadWriteStartCondition.
//
//  METHOD CLASSIFICATION (all 44 golden methods; the mandate's 4-bucket taxonomy)
//  --------------------------------------------------------------------------
//  (a) DISPLAY/READ-ONLY -- TRANSLATED this wave (3 methods, bodies in
//      HandlerSys.cpp):
//        FormShow          golden :34-77   (44 lines -- calls LoaderSystemSet/
//                            LoaderSafeDoorSet, GATE (H1) on SortItemToMap(),
//                            reads a GPIB-bridge ini for cbHandlerModel/
//                            rgModel)
//        LoaderSystemSet   golden :79-457  (378 lines -- ~150
//                            CheckAndReadIniDataGeneral() calls filling
//                            RadioGroup->ItemIndex/ComboBox->Text/
//                            Edit->Text/CheckBox->Checked from the machine's
//                            installed-hardware config; see MAJOR RISK below)
//        LoaderSafeDoorSet golden :990-1004 (12 lines -- Sen[SnSafeDoorN].Enable
//                            -> SafeDoorN->Checked)
//
//  (b) WRITE PATH -- QUEUED, NOT TRANSLATED, NOT EVEN DECLARED this wave -- 34
//      methods: SaveSystemSet (:459-978, 520 lines, the LoaderSystemSet
//      mirror -- WriteIniDataGeneral/WriteIniData), SaveBtnClick/LoadBtnClick
//      (thin wrappers), SaveSafeDoorSet (:1006-1035, writes Sen[].Enable +
//      SAFE_DOOR_AMOUNT-conditioned overrides -- SAFETY-ADJACENT, see (c)),
//      ExitBtnClick (:1037-1043, calls HSys.ReadGeneralIni()+SaveSafeDoorSet),
//      BtnEnableAllClick (:1045-1052, zeroes all SafeDoorN checkboxes --
//      SAFETY-ADJACENT, see (c)), rgCustomerListClick (:1054-1060, stages
//      edtCustomerCode->Text from the selected list item), ExitBtnMouseDown
//      (:1062-1071, right-click reveals the hidden customer-code tab),
//      FormClose (:1073-1079, TabVisible reset + Do_Log -- pure chrome, no
//      persist of its own; bucketed here rather than as (a) since it has no
//      display VALUE to refresh, matching forms/fSpeed.h's sbtExitClick
//      "nothing to translate" precedent), btnSetATCComClick (:1081-1098,
//      hardcodes 14 COM-port text fields), GetCustomerName (:1100-1130 --
//      SEE DEFERRAL NOTE below, NOT bucketed (b), a data-fidelity deferral),
//      rgRotateKit_TypeClick (:1132-1136, a 2-line Enabled recompute
//      IDENTICAL to FormShow's own already-translated logic, but gated as an
//      edit-adjacent Click handler per convention), edtUserDefMaxContactHeightClick
//      + 20 more ed*Click/ed*MouseDown QwertyKey-opener handlers (same
//      "edit gesture" bucket as forms/fSpeed.h's ~24 MouseDown handlers):
//      edtSearchCodeChange, edtSearchFunctionChange (both GATE (H1)-
//      dependent, see (d)), rgTTLCardClick, edtUserDefineIndexZSafePosClick,
//      edtCustomerCodeClick, edMaxMpaFBClick, edMaxKpaClick, edMinMpaClick,
//      edATCSystemPortClick, edATCSystemUseHeatClick,
//      edHotGunFlow_Gun1_ChannelNoClick, edHotGunFlow_DevNoClick,
//      edtMinYPitchClick, edtMaxYPitchClick, edCognexSystemCCDClick,
//      ed24VMonitorPulseCountClick, edGroundMan_AlarmOhmClick,
//      edIONPulseCountClick, edtTriTempTotalChClick, edtOutShtMaxTempClick,
//      edtTriTemperature_MaxDegreeClick, edtTriTemperature_MinDegreeClick,
//      edtIndexMaxTempClick, edt_Total_CompressorChange, edtMinXPitchClick,
//      edtMaxXPitchClick.
//
//  (c) SAFETY-ADJACENT -- subset of (b), called out separately, NOT
//      translated this wave: SaveSafeDoorSet + BtnEnableAllClick both mutate
//      `Sen[SnSafeDoorN].Enable` -- whether a physical safe-door interlock
//      sensor is even CHECKED by the running handler. Disabling one here
//      (BtnEnableAllClick zeroes ALL of them in one click) does not itself
//      open a door, but it can silence the software-side detection that a
//      door is open. Flagged explicitly, not silently bucketed with the
//      ~150-line generic WriteIniDataGeneral cascade in SaveSystemSet.
//
//  (d) GOLDEN-DEAD or STRUCTURALLY GATED -- 4 methods, NOT declared this wave:
//        SortItemToMap (:1170-1199) / InitItemToMap (:1201-1218) -- GATE (H1)
//          below. edtSearchCodeChange (:1149-1168) / edtSearchFunctionChange
//          (:1220-1245) both transitively depend on TempComp (populated only
//          by SortItemToMap/InitItemToMap), so both are structurally gated
//          alongside them, same as forms/fSpeed.h's GATE (S4)
//          DoReplyDefaultToForm/SearchRecipeParameter both depending on the
//          gated fRPDefault.
//        pcSettingChange (:1144-1147, `edtSearchCode->Visible=(pcSetting->
//          ActivePage==tsCustomerCode);`) -- SAME missing-property class as
//          cStartCondition's GATE (SC6): vclcompat::TPageControl has no
//          pointer-valued ActivePage. Trivial 1-line body, not worth
//          declaring just to #if-0 it whole; omitted per forms/fSpeed.h's
//          "not even a stub declared" convention for structurally-blocked
//          methods.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (H1) FormShow :36 `SortItemToMap();` -- SortItemToMap/InitItemToMap
//      (golden :1170-1218) take/recurse via `TWinControl *PCtrl` +
//      `PCtrl->ControlCount` + `PCtrl->Controls[iP]` -- the IDENTICAL
//      TWinControl-shaped component-tree-walk gap forms/fSpeed.h's GATE (S4)
//      already documents in exhaustive detail (vclcompat::TPanel/TGroupBox/
//      TRadioGroup all derive directly from TControl, none from any
//      TWinControl-shaped ancestor carrying ControlCount/Controls[]; a
//      DIFFERENT, unrelated TWinControl exists at handlerlog.h:104 for
//      TMyLog's own unrelated walk and is not convertible to accept a
//      TPanel*). Per "跨檔缺口 GATE 不自建 shim", the call is `#if 0`-gated
//      in FormShow and SortItemToMap/InitItemToMap themselves are NOT
//      declared here at all. DISCLOSED CONSEQUENCE: this facade's
//      `edtSearchCode`/`edtSearchFunction`/customer-code-search feature is
//      entirely inert offline (TempComp always empty) -- same posture as the
//      GetCustomerName DEFERRAL below, both centred on the same
//      rgCustomerList data this wave declined to hand-transcribe.
//
//  MAJOR RISK -- LoaderSystemSet reads the SHARED PRODUCTION Gerneral.ini
//  --------------------------------------------------------------------------
//  `CheckAndReadIniDataGeneral(Group,Name,Default)` (common.cpp:1444-1525)
//  operates directly on `INIFileGeneral` -- the SAME TIniFile the project's
//  own `common.cpp:89`/`asGeneralPath` gotcha is about
//  (`D:\HT9045\system\Gerneral.ini`, the shared production machine-identity
//  file that `LoadMachineConfig()` back-fills missing keys into). This is
//  NOT a new risk this wave introduces -- CheckAndReadIniDataGeneral is an
//  existing, already-merged, already-relied-on utility (forms/fSpeed.h's own
//  "DISCLOSED, NOT GATED" precedent covers its SISTER function
//  CheckAndReadIniData's identical lazy-seed behaviour) -- but THIS form is
//  categorically the MOST central place in the whole tree for it: golden's
//  own LoaderSystemSet calls CheckAndReadIniDataGeneral roughly 150 times in
//  one function, covering literally every installed-hardware-option flag the
//  machine has. If THandlerSystem is ever wired to a live INIFileGeneral (a
//  future integration wave's job, not this one's -- no `extern THandlerSystem
//  *HandlerSystem;` is declared here, see INTEGRATION-PENDING), calling
//  LoaderSystemSet for real means: (1) any key ABSENT from Gerneral.ini gets
//  SEEDED into it (a real write against the shared production file), and
//  (2) running this against a live production machine's Gerneral.ini without
//  `--dry` is squarely the scenario the project's standing `--dry` policy
//  for `wb_serve`/`wb_publish` exists to prevent. NOT gated here (matches
//  precedent for an already-relied-on utility), but flagged with maximum
//  visibility since this file is the single largest concentration of that
//  risk surface in the tree.
//
//  DEFERRAL -- GetCustomerName / rgCustomerList NOT translated (data-fidelity
//  risk, not a missing-dependency gate)
//  --------------------------------------------------------------------------
//  golden's ctor (:18-24) populates `slCustomerCode` by copying
//  `rgCustomerList->Items` -- a HARDCODED ~200-entry TRadioGroup Items list
//  (HandlerSys.dfm :Items.Strings, one line per customer code "NNN Name
//  Description", many with cp950-decimal-escaped (`#40251#21185` etc.)
//  Chinese/Japanese/Korean company names) that GetCustomerName's entire
//  SubString(1,3)/SubString(5,..) parsing depends on verbatim. Hand-
//  transcribing ~200 lines of decimal-escaped multi-byte text under this
//  wave's time budget is exactly the kind of high-risk, easy-to-silently-
//  corrupt transcription this tree's own "分析前先驗快照歸屬"/customer-
//  identity-mistake lessons warn against -- a single dropped or mis-decoded
//  entry would misreport a REAL customer's name back through GetCustomerName
//  to (eventually) SECS/GEM or on-screen operator-facing text. NEITHER
//  `rgCustomerList` NOR `GetCustomerName` NOR the ctor's `slCustomerCode`
//  wiring is declared/translated this wave; a future wave with a proper
//  automated (not hand-typed) dfm-string extraction pass should do this
//  list, matching the project's own "有維度的 widget 照 golden .dfm 維度
//  hydrate" discipline extended to "text CONTENT hydration", not just
//  numeric dimensions.
//
//
//  ADDENDUM -- RESOLVED (W906-FW3-HandlerSys-WB, 20260820)
//  --------------------------------------------------------------------------
//  The DEFERRAL above is RESOLVED. rgCustomerList/slCustomerCode/
//  GetCustomerName are now declared/translated (this header + HandlerSys.cpp).
//  The ~200-entry text WAS NOT hand-transcribed -- a one-time Python script
//  (session scratchpad, not checked into this tree) parsed HandlerSys.dfm's
//  `Items.Strings = (...)` block in BINARY mode, decoding quoted segments as
//  cp950 and `#NNNNN` escapes as Unicode codepoints (chr(NNNNN), verified
//  NOT cp950 byte pairs -- see the data-table banner in HandlerSys.cpp for
//  the full method). Result: 211 items, 0 tokenizer anomalies (every one of
//  211 physical lines -- including the terminator line, where the block's
//  closing ')' sits glued onto the last item's final token with no
//  separator -- fully round-tripped with zero leftover bytes), item count
//  cross-checked two independent ways (tokenizer count vs. independent
//  regex line count, both 211), and 5 entries spot-checked against both raw
//  dfm bytes and real-world semantic plausibility (own company name +
//  Thailand x2 + Germany + USA + Qualcomm's actual Chinese brand name). See
//  HandlerSys.cpp's data-table banner for the itemized spot-check list.
//
//  DEPENDENCY AUDIT (this wave, 20260819) -- confirmed present with matching
//  field/function names:
//    common.h/.cpp : CheckAndReadIniDataGeneral (4 overloads, common.h:229-232),
//                    CheckAndReadIniData, CheckRange<T>(), MyForceDirectories.
//    mysensor.h    : `extern class TMySensor Sen[MAX_SENSOR_ITEM];` with
//                    `bool Enable` member.
//    cmydef.h      : SnSafeDoor1..10/SnHeaterDoor/SnHeaterDoor2 (sensor-index
//                    enum), SAFE_DOOR_AMOUNT, and the ~150 golden hardware-
//                    config globals LoaderSystemSet reads by name (
//                    SUPPORT_2_EMPTY_EMPTY/LOAD_Z_USE_MOTOR[]/LOAD_Y_USE_MOTOR[]/
//                    LOADUNLOAD_USE_CASSETTE[]/InOutArmPickerUseMotor/
//                    NUMBER_PANEL_TYPE/ION_FAN_TYPE/SHUTTLE_SENSOR_TYPE/
//                    NUEC_TYPE/ENABLE_OUT_SHUTTLE_SENEOR/
//                    ENABLE_OUT_SHUTTLEY_LATCH/Use_AxisY_Sensor_2x3mode/
//                    Bias_Mode_Use_Y_Sensor/WEIGHT_CALIBRATION/bUseAuto2Empty/
//                    USE_AUTO_RETEST/UNLOADER_ART[]/ATC_SYSTEM/
//                    USE_IN_OUT_ARM_Y_PITCH/USE_OUT_ARM_Y_PITCH/iIndEPCnt --
//                    all confirmed present tree-wide via `grep -rl`, this
//                    wave; individual struct-field arity not re-verified one
//                    by one for every one of the ~150 -- see self-check
//                    result for the authoritative pass/fail).
//    MachineType.h : eAuto1..eAuto6, eNewATCSystem, iXPitch60/iXPitch40mm,
//                    tTemp175/155/150/130, e85KG, eht4Heater, KT4H,
//                    eocrUninstal, ebctUninstall, eATCUninstall, eATCNonMix,
//                    Fix3K_Uninstall, eCSMUN_Uninstall, eartUninstall,
//                    Type_None.
//    NOT found      : COMMSPEED_20M (golden Motor/mn200.h #define) -- NO
//                    ported header anywhere in this tree. SAME already-
//                    documented gate as database.cpp:1082's own
//                    `TODO(GA1-B6)` for this identifier (not a new finding
//                    this wave). `#if 0`-gated in LoaderSystemSet's
//                    rgMNetSpeed->ItemIndex assignment; left untouched
//                    rather than substituting a guessed literal.
//    handlerlog.h  : myLog (TMyLog), Do_Log() -- FormShow's own call
//                    GATE'd out (see below), not because it's missing but
//                    because it needs the `Sender` parameter this facade's
//                    FormShow (matching every other translated form in this
//                    tree) intentionally drops.
//  NOT found / gated: TWinControl-shaped base for TPanel/TGroupBox/
//  TRadioGroup (GATE H1), TPageControl::ActivePage (pcSettingChange, not
//  declared).
//
//  INTEGRATION-PENDING -- NO `extern THandlerSystem *HandlerSystem;` here
//  --------------------------------------------------------------------------
//  Same posture as forms/fSpeed.h/forms/fStartCondition.h: no consumer of a
//  `HandlerSystem` global exists anywhere else in this tree yet. Picking the
//  live singleton wiring -- and resolving the MAJOR RISK above by deciding
//  whether that wiring goes through a `--dry`-safe copy of INIFileGeneral --
//  is a later integration wave's call, not this one's.
// =============================================================================
#ifndef FORMS_FHANDLERSYS_H
#define FORMS_FHANDLERSYS_H

#include "vclcompat/Controls.h"   // TEdit/TCheckBox/TRadioGroup/TComboBox/
                                  //   TGroupBox/TPageControl/TTabSheet
                                  //   (brought to global scope by that
                                  //   header's own footer)

// ===========================================================================
//  THandlerSystem -- non-VCL facade (golden HandlerSys.h,
//  THandlerSystem : public TForm)
// ===========================================================================
class THandlerSystem
{
public:
    THandlerSystem();

    // -----------------------------------------------------------------------
    //  Widget members touched by this wave's 3 translated (a) methods only
    //  (FormShow/LoaderSystemSet/LoaderSafeDoorSet) -- golden __published
    //  order preserved. `rgCustomerList`/`slCustomerCode`/`TempComp` NOT
    //  declared -- see DEFERRAL and GATE (H1) notes above. TPopupMenu/
    //  TMenuItem/TScrollBox-typed widgets don't exist in this form anyway
    //  (unlike cStartCondition); the omission here is purely "not touched by
    //  the 3 in-scope methods", same DEVIATION rationale as
    //  forms/fStartCondition.h's leaner-subset note.
    //
    //  UPDATE (W906-FW3-HandlerSys-WB, 20260820): the "`rgCustomerList`/
    //  `slCustomerCode` NOT declared" sentence above is now STALE -- both
    //  ARE declared below (see the DEFERRAL ADDENDUM further up this file).
    //  `TempComp` remains NOT declared (still GATE (H1), unaffected by this
    //  wave's data-fidelity fix).
    // -----------------------------------------------------------------------
    TGroupBox    *GroupBox1  = new TGroupBox();
    TGroupBox    *GroupBox2  = new TGroupBox();
    TPageControl *pcSetting  = new TPageControl();
    TTabSheet    *tsCustomerCode = new TTabSheet();

    TComboBox   *cbComIndex = new TComboBox();
    TComboBox   *cbComTemp = new TComboBox();
    TComboBox   *cbComTempOmron = new TComboBox();
    TComboBox   *cbComTester = new TComboBox();
    TComboBox   *cbComBinDisp = new TComboBox();
    TComboBox   *cbComRTC = new TComboBox();
    TComboBox   *cbComDyTemp = new TComboBox();
    TComboBox   *cbbATC2 = new TComboBox();
    TComboBox   *cbbATC3 = new TComboBox();
    TComboBox   *cbbATC4 = new TComboBox();
    TComboBox   *cbbATC1 = new TComboBox();
    TComboBox   *cbOCR = new TComboBox();
    TComboBox   *cbOCRwithTester = new TComboBox();
    TComboBox   *cbAirCon = new TComboBox();
    TComboBox   *cbEMAwarePort1 = new TComboBox();
    TComboBox   *cbEMAwarePort2 = new TComboBox();
    TComboBox   *cbEMAwarePort3 = new TComboBox();
    TComboBox   *cbEMAwarePort4 = new TComboBox();
    TCheckBox   *cbESDUse4COM = new TCheckBox();
    TComboBox   *cbNovx3360 = new TComboBox();
    TComboBox   *cbComLaser1 = new TComboBox();
    TComboBox   *cbComLaser2 = new TComboBox();
    TComboBox   *cbComLaserInArm = new TComboBox();
    TRadioGroup *rgColorSensor = new TRadioGroup();
    TComboBox   *cb2DReader1 = new TComboBox();
    TComboBox   *cb2DReader2 = new TComboBox();
    TComboBox   *cb2DReader3 = new TComboBox();
    TComboBox   *cb2DReader4 = new TComboBox();
    TRadioGroup *rgRotateKit = new TRadioGroup();
    TRadioGroup *rgRotateKitIn = new TRadioGroup();
    TRadioGroup *rgRotateKitOut = new TRadioGroup();
    TRadioGroup *rgRotateKit_Type = new TRadioGroup();
    TComboBox   *cbbGroundMan = new TComboBox();
    TComboBox   *cbKASUGA_Fan = new TComboBox();
    TComboBox   *cbSocketSenAmpCnt = new TComboBox();
    TComboBox   *cbRotateSenAmpCnt = new TComboBox();
    TComboBox   *cbColorSenAmpCnt = new TComboBox();
    TRadioGroup *rgSocketSen = new TRadioGroup();
    TComboBox   *cbbTrayStepMotor = new TComboBox();
    TCheckBox   *chkUseHPComCard = new TCheckBox();
    TRadioGroup *rgCanBusMethod = new TRadioGroup();
    TRadioGroup *rgTrayVibration = new TRadioGroup();
    TRadioGroup *rgNumberPanelType = new TRadioGroup();
    TRadioGroup *rgInstallFix3 = new TRadioGroup();
    TRadioGroup *rgAutoAlignment = new TRadioGroup();
    TRadioGroup *Auto2SelectCy = new TRadioGroup();
    TRadioGroup *rgLdUldUseStepMotor = new TRadioGroup();
    TRadioGroup *rgCKDFCM = new TRadioGroup();
    TRadioGroup *rgCatchTrayModel = new TRadioGroup();
    TRadioGroup *rgTrayMappingGrabImage = new TRadioGroup();
    TRadioGroup *rgLoaderKeyence = new TRadioGroup();
    TRadioGroup *rgMultileEmptyTrayIDKeyence = new TRadioGroup();
    TRadioGroup *rgEmptyKeyence = new TRadioGroup();
    TRadioGroup *rgFix3FullPlace = new TRadioGroup();
    TRadioGroup *rgRFIDSystem = new TRadioGroup();
    TRadioGroup *rgCatchTrayBlock = new TRadioGroup();
    TRadioGroup *rgTrayRobot = new TRadioGroup();
    TRadioGroup *rgMultileEmpty = new TRadioGroup();
    TEdit       *edtCustomerCode = new TEdit();
    TEdit       *edtSeriaNo = new TEdit();
    TComboBox   *cbHandlerModel = new TComboBox();
    TRadioGroup *rgModel = new TRadioGroup();
    TRadioGroup *rgMotionCard = new TRadioGroup();
    TRadioGroup *rgIOCard = new TRadioGroup();
    TRadioGroup *rgTTLCard = new TRadioGroup();
    TRadioGroup *rgUseSucker_9046_DB = new TRadioGroup();
    TRadioGroup *rgUseSensor_9046_DB = new TRadioGroup();
    TRadioGroup *rgSafeDoorLock = new TRadioGroup();
    TRadioGroup *rgMNetSpeed = new TRadioGroup();
    TRadioGroup *rgIndexMotionCard = new TRadioGroup();
    TCheckBox   *SafeDoor1 = new TCheckBox();
    TCheckBox   *SafeDoor2 = new TCheckBox();
    TCheckBox   *SafeDoor3 = new TCheckBox();
    TCheckBox   *SafeDoor4 = new TCheckBox();
    TCheckBox   *SafeDoor5 = new TCheckBox();
    TCheckBox   *SafeDoor6 = new TCheckBox();
    TCheckBox   *SafeDoor7 = new TCheckBox();
    TCheckBox   *SafeDoor8 = new TCheckBox();
    TCheckBox   *SafeDoor9 = new TCheckBox();
    TCheckBox   *SafeDoor10 = new TCheckBox();
    TCheckBox   *HeaterDoor1 = new TCheckBox();
    TCheckBox   *HeaterDoor2 = new TCheckBox();
    TRadioGroup *rgInOutArmYPitch = new TRadioGroup();
    TEdit       *edtMinYPitch = new TEdit();
    TEdit       *edtMaxYPitch = new TEdit();
    TRadioGroup *rgLaserDistance = new TRadioGroup();
    TRadioGroup *rgOCR = new TRadioGroup();
    TRadioGroup *rgDeviceFlipper = new TRadioGroup();
    TRadioGroup *rgHotPlatePos = new TRadioGroup();
    TRadioGroup *rgHotplateType = new TRadioGroup();
    TRadioGroup *rgHotPlateLimit = new TRadioGroup();
    TRadioGroup *rgMagneticScale = new TRadioGroup();
    TRadioGroup *ArmZAtoZH = new TRadioGroup();
    TRadioGroup *rgPickerCount = new TRadioGroup();
    TRadioGroup *rgPreciser = new TRadioGroup();
    TRadioGroup *rgPreciserPos = new TRadioGroup();
    TRadioGroup *rgDieClean = new TRadioGroup();
    TRadioGroup *rgSafeDoor = new TRadioGroup();
    TRadioGroup *rgAOI = new TRadioGroup();
    TEdit       *ed24VMonitorPulseCount = new TEdit();
    TRadioGroup *rg2DBarcode = new TRadioGroup();
    TRadioGroup *rgShuttleFloating = new TRadioGroup();
    TRadioGroup *rgBottom2DID = new TRadioGroup();
    TEdit       *edCognexSystemCCD = new TEdit();
    TRadioGroup *rgShuttleSensor = new TRadioGroup();
    TRadioGroup *rgShuttleZType = new TRadioGroup();
    TRadioGroup *rgShuttleCrossSensor = new TRadioGroup();
    TRadioGroup *rgAutoShuttleSensor = new TRadioGroup();
    TRadioGroup *rgInShtLastSensor = new TRadioGroup();
    TRadioGroup *rgShuttleVibration = new TRadioGroup();
    TRadioGroup *rgUseOutSht = new TRadioGroup();
    TRadioGroup *rgUse4DUT = new TRadioGroup();
    TRadioGroup *rgOTDInstall = new TRadioGroup();
    TRadioGroup *rgIOChangeToque = new TRadioGroup();
    TRadioGroup *rgIndexMotorType = new TRadioGroup();
    TRadioGroup *rgSocketClamp = new TRadioGroup();
    TRadioGroup *rgRealTimeCCD = new TRadioGroup();
    TRadioGroup *rgRealTimeCCDTempNum = new TRadioGroup();
    TRadioGroup *rgIndexPressType = new TRadioGroup();
    TEdit       *edMaxMpaFB = new TEdit();
    TEdit       *edtMinMpaFB = new TEdit();
    TRadioGroup *rgDoubleEPControl = new TRadioGroup();
    TRadioGroup *rgATC = new TRadioGroup();
    TRadioGroup *rgHeaterType = new TRadioGroup();
    TEdit       *edATCSystemIP = new TEdit();
    TEdit       *edATCSystemPort = new TEdit();
    TEdit       *edATCSystemUseHeat = new TEdit();
    TRadioGroup *rgHighTempLimit = new TRadioGroup();
    TCheckBox   *chkUserDefMaxContactHeight = new TCheckBox();
    TEdit       *edtUserDefMaxContactHeight = new TEdit();
    TEdit       *edHotGunFlow_LineNo = new TEdit();
    TEdit       *edHotGunFlow_DevNo = new TEdit();
    TEdit       *edHotGunFlow_Gun1_ChannelNo = new TEdit();
    TEdit       *edHotGunFlow_Gun2_ChannelNo = new TEdit();
    TRadioGroup *rgHotGunFlow = new TRadioGroup();
    TRadioGroup *rgHeatGun = new TRadioGroup();
    TRadioGroup *rgHeater = new TRadioGroup();
    TRadioGroup *rgESDTemp = new TRadioGroup();
    TRadioGroup *rgAirConditioner = new TRadioGroup();
    TRadioGroup *ElectronPressure = new TRadioGroup();
    TRadioGroup *rgWeightCali = new TRadioGroup();
    TRadioGroup *rgIndexSuckerType = new TRadioGroup();
    TEdit       *edMaxKpa = new TEdit();
    TEdit       *edMinMpa = new TEdit();
    TRadioGroup *rgLBTemp = new TRadioGroup();
    TRadioGroup *rgCleanAir = new TRadioGroup();
    TRadioGroup *rgFinePitch = new TRadioGroup();
    TComboBox   *cbIndEPCnt = new TComboBox();
    TRadioGroup *rgDewpointHW = new TRadioGroup();
    TRadioGroup *rgCCDTemp = new TRadioGroup();
    TRadioGroup *rgIonFanType = new TRadioGroup();
    TRadioGroup *rgUsePulseType = new TRadioGroup();
    TEdit       *edIONPulseCount = new TEdit();
    TRadioGroup *rgGroundMan = new TRadioGroup();
    TRadioGroup *rgGroundMan_ScanPoint = new TRadioGroup();
    TEdit       *edGroundMan_AlarmOhm = new TEdit();
    TRadioGroup *rg_ControlPanelMode = new TRadioGroup();
    TComboBox   *cbComTTLRS232 = new TComboBox();
    TComboBox   *cbComTTLRS232_2 = new TComboBox();
    TRadioGroup *rgTTLUseAddress = new TRadioGroup();
    TRadioGroup *rgFixAICCD = new TRadioGroup();
    TRadioGroup *rgATCHeatGun = new TRadioGroup();
    TComboBox   *cbbRFIDReader = new TComboBox();
    TRadioGroup *rgCCDAutoAlignmentMode = new TRadioGroup();
    TRadioGroup *rgIndexMotorAxis = new TRadioGroup();
    TCheckBox   *chkUser_Define_IndexZ_SafePos = new TCheckBox();
    TEdit       *edtUserDefineIndexZSafePos = new TEdit();
    TRadioGroup *rgBottom2DID_CCD = new TRadioGroup();
    TRadioGroup *rgTrayArmMode = new TRadioGroup();
    TRadioGroup *rgBaseHeaterCount = new TRadioGroup();
    TRadioGroup *rgShuttleFloodgate = new TRadioGroup();
    TRadioGroup *rgNUECType = new TRadioGroup();
    TRadioGroup *rgCOLORSENSOR_MUN = new TRadioGroup();
    TRadioGroup *rgInOutArmXPitch = new TRadioGroup();
    TEdit       *edtMinXPitch = new TEdit();
    TEdit       *edtMaxXPitch = new TEdit();
    TEdit       *edtHPLimit = new TEdit();
    TRadioGroup *rgVibrationCommuncation = new TRadioGroup();
    TComboBox   *cbVibrationCardQty = new TComboBox();
    TRadioGroup *MachineTrack = new TRadioGroup();
    TRadioGroup *rgLoaderHinge = new TRadioGroup();
    TCheckBox   *cbLoaderCassette = new TCheckBox();
    TCheckBox   *cbEmptyCassette = new TCheckBox();
    TCheckBox   *cbColorCassette = new TCheckBox();
    TCheckBox   *cbAuto1Cassette = new TCheckBox();
    TCheckBox   *cbAuto2Cassette = new TCheckBox();
    TCheckBox   *cbAuto3Cassette = new TCheckBox();
    TCheckBox   *cbAuto4Cassette = new TCheckBox();
    TCheckBox   *cbAuto5Cassette = new TCheckBox();
    TCheckBox   *cbAuto6Cassette = new TCheckBox();
    TCheckBox   *chkLoader = new TCheckBox();
    TCheckBox   *chkEmpty = new TCheckBox();
    TCheckBox   *chkColor = new TCheckBox();
    TCheckBox   *chkAuto1 = new TCheckBox();
    TCheckBox   *chkAuto2 = new TCheckBox();
    TCheckBox   *chkAuto3 = new TCheckBox();
    TCheckBox   *chkAuto6 = new TCheckBox();
    TCheckBox   *chkAuto5 = new TCheckBox();
    TCheckBox   *chkAuto4 = new TCheckBox();
    TRadioGroup *rgInstallAutoRestest = new TRadioGroup();
    TRadioGroup *rgAuto1ART = new TRadioGroup();
    TRadioGroup *rgAuto2ART = new TRadioGroup();
    TRadioGroup *rgAuto3ART = new TRadioGroup();
    TRadioGroup *rgAuto6ART = new TRadioGroup();
    TRadioGroup *rgAuto5ART = new TRadioGroup();
    TRadioGroup *rgAuto4ART = new TRadioGroup();
    TComboBox   *cbbComBinDisp2 = new TComboBox();
    TRadioGroup *rgMagBinDispType = new TRadioGroup();
    TRadioGroup *rgTriTempMachine = new TRadioGroup();
    TRadioGroup *rgAirStreamSelect = new TRadioGroup();
    TEdit       *edtTriTemperature_MaxDegree = new TEdit();
    TEdit       *edtTriTemperature_MinDegree = new TEdit();
    TEdit       *edtTriTempTotalCh = new TEdit();
    TEdit       *edtOutShtMaxTemp = new TEdit();
    TEdit       *edtIndexMaxTemp = new TEdit();
    TEdit       *edtBaseMaxTemp = new TEdit();
    TEdit       *edt_Total_Compressor = new TEdit();
    TRadioGroup *rgDoubleBeltMode = new TRadioGroup();
    TRadioGroup *rg_IndexDoorHeater = new TRadioGroup();
    TEdit       *edtOutSortXPitchMin = new TEdit();
    TEdit       *edtOutSortXPitchMax = new TEdit();
    TRadioGroup *rgOutSortArm = new TRadioGroup();
    TRadioGroup *rgVacuUnitType = new TRadioGroup();
    TCheckBox   *cbEnableOutShtSensor = new TCheckBox();
    TCheckBox   *chkOutShtYSensorByLatch = new TCheckBox();
    TCheckBox   *chk2x3modeUseAxisYSensor = new TCheckBox();
    TCheckBox   *chkBiasModeUseYSensor = new TCheckBox();
    TRadioGroup *rgScanner_AOI = new TRadioGroup();
    TRadioGroup *rgTopScanner_AOI = new TRadioGroup();
    TRadioGroup *rgSafePlcIO = new TRadioGroup();
    TComboBox   *coNudn1Macid11 = new TComboBox();
    TComboBox   *coNudn1Macid12 = new TComboBox();
    TComboBox   *coNudn1Macid13 = new TComboBox();
    TComboBox   *coCanBusNudn1 = new TComboBox();
    TRadioGroup *rgLBTemp2 = new TRadioGroup();
    TRadioGroup *rgCustomerList = new TRadioGroup();  // golden :422 -- Items hydrated in ctor
                                                     //   (HandlerSys.cpp), NOT via NSDMI; see
                                                     //   ctor DEVIATION banner there. DEFERRAL
                                                     //   RESOLVED, see ADDENDUM below.
    TComboBox   *coNudn1Macid14 = new TComboBox();
    TComboBox   *cbSocketSenAmpCnt2nd = new TComboBox();
    TRadioGroup *rgInShtAutoLatch = new TRadioGroup();
    TRadioGroup *rgLDCarRotAtm = new TRadioGroup();
    TRadioGroup *rgE84Sensor = new TRadioGroup();
    TRadioGroup *rgLdUldCassetteMode = new TRadioGroup();
    TRadioGroup *rgOutArmYPitch = new TRadioGroup();
    TRadioGroup *rg3M_EM_AWARE_Monitor = new TRadioGroup();
    TRadioGroup *rgNovx3360 = new TRadioGroup();
    TRadioGroup *rgKasuga = new TRadioGroup();
    TRadioGroup *rgKasuga_Fan = new TRadioGroup();
    TRadioGroup *rgHTIonBar = new TRadioGroup();
    TRadioGroup *rgChamberUsePulseType = new TRadioGroup();
    TRadioGroup *rgAutoCleanIonFan = new TRadioGroup();
    TRadioGroup *rgOCRYStepMot = new TRadioGroup();
    TRadioGroup *rgCoverTrayID = new TRadioGroup();
    TRadioGroup *rgRFIDReader = new TRadioGroup();
    TRadioGroup *rg2ndLoader = new TRadioGroup();
    TRadioGroup *rgAuto3Magazine = new TRadioGroup();
    TRadioGroup *rgOHTSystem = new TRadioGroup();
    TRadioGroup *rgMRSystem = new TRadioGroup();
    TRadioGroup *rgTrayMapping = new TRadioGroup();
    TCheckBox   *chkLoaderY = new TCheckBox();
    TCheckBox   *chkEmptyY = new TCheckBox();
    TCheckBox   *chkColorY = new TCheckBox();
    TCheckBox   *chkAuto1Y = new TCheckBox();
    TCheckBox   *chkAuto2Y = new TCheckBox();
    TCheckBox   *chkAuto3Y = new TCheckBox();
    TRadioGroup *rgATCMixMode = new TRadioGroup();
    TComboBox   *cbSocketSenAmpCnt3rd = new TComboBox();

    // -----------------------------------------------------------------------
    //  Wave A translated methods (bodies: HandlerSys.cpp, this wave)
    // -----------------------------------------------------------------------
    void FormShow();          // golden :34-77 (Sender dropped, see below)
    void LoaderSystemSet();   // golden :79-457
    void LoaderSafeDoorSet(); // golden :990-1004

    // -----------------------------------------------------------------------
    //  Wave B translated member/method (body: HandlerSys.cpp, this wave) --
    //  resolves the DEFERRAL note above. See ADDENDUM below and the ctor/
    //  GetCustomerName banners in HandlerSys.cpp for extraction method +
    //  verification detail (211/211 items, 0 tokenizer anomalies).
    // -----------------------------------------------------------------------
    TStringList *slCustomerCode = new TStringList();  // golden :506; populated in
                                                       //   ctor (HandlerSys.cpp)
    AnsiString   GetCustomerName();                    // golden :1100-1130
};

// DEVIATION: FormShow drops its golden TObject* Sender parameter -- matches
// every other translated form facade's identical simplification in this
// tree. CONSEQUENCE: golden's own `myLog.Do_Log(Sender, asUser, asLogPath);`
// tail call (golden :76) is #if 0-gated in the body rather than translated,
// since it needs the parameter this facade intentionally does not carry
// (myLog/Do_Log themselves DO exist, handlerlog.h -- not a missing-
// dependency gate, just a dropped-parameter consequence).

// AI(W906-FW3-HandlerSys-WA) 20260819: integration-pending -- NO
// `extern THandlerSystem *HandlerSystem;` here. See INTEGRATION-PENDING note
// above.

#endif // FORMS_FHANDLERSYS_H

