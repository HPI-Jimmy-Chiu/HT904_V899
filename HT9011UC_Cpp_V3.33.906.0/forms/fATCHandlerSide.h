// =============================================================================
//  forms/fATCHandlerSide.h  --  non-VCL facade for golden's TATC_InterfaceForm
//  (ATC/ATC_Handler_Side.h / ATC_Handler_Side.cpp) -- the handler-side of the
//  Handler <-> ATC (Advanced Temperature Controller) socket protocol: command
//  assembly/dispatch (SendCommand), inbound frame parsing
//  (ProcessReceiveString_ATC), temperature/offset/mode setpoints, self-test
//  and lot-start/end sequencing, refrigerator/air-machine control, PF/TJ/PID
//  tuning, and an ATC-recipe file-transfer side channel (FileSocket).
//
//  AI(W906-FW3-ATC1) 20260827: new file, FW wave FW3-ATC1 (facade wave 1 of
//  ATC_Handler_Side -- read-only direction, no write path; task banner
//  "FW3-ATC1"). Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/
//  ATC/ATC_Handler_Side.cpp (4,097 lines) + ATC_Handler_Side.h (499 lines).
//  Decoded this wave with `io.open(p, encoding='cp950')`, written out UTF-8,
//  0 U+FFFD (checked with `data.count(chr(0xFFFD))` after decode, 20260827).
//
//  ============================================================================
//  *** SAFETY POSTURE -- READ BEFORE TOUCHING ANY GATE IN THIS FILE ***
//  ============================================================================
//  TATC_InterfaceForm is the OUTBOUND-COMMAND-AND-SETPOINT hub for the ATC
//  subsystem: SendCommand assembles and writes every "@<cmd>,...#" frame onto
//  ClientSocket; Run/Stop/StartTesting/TestFinish/SetRunSelfTest/
//  SetManualSelfTest/LotStart/LotEnd/SetAllTemp/SetSingleTemp/SetOffset/
//  SetAirValve/SendSwitchRefrigerator/ChannelFFCTrigger/SetTempPID/
//  ATCCONTROLMODEMode and every other Send*/Set*/Start*/Stop/Change* member
//  either calls SendCommand directly or is itself only reachable by calling
//  one that does. Of golden's 169 method DEFINITIONS (see DENOMINATOR below --
//  169, not census's raw 168; see the DISCREPANCY note), this wave delivers
//  28 and gates 141.
//
//  Every gated method is DECLARED here and DELIBERATELY NOT DEFINED in
//  forms/fATCHandlerSide.cpp (same idiom as forms/fHS.h / forms/fIoSetView.h /
//  forms/fContact.h). The LINKER is the interlock: the moment any future wave
//  -- or the web write path -- tries to CALL one, the build fails with
//  `undefined reference to TATC_InterfaceForm::<method>` naming the exact
//  entry point. No member here is virtual, so an undefined member function
//  costs nothing at link time until somebody calls it.
//
//  UN-GATING PROTOCOL: translate a gated body only in a wave that owns the
//  interlock story for whichever subsystem it touches -- SendCommand itself
//  needs the outbound-socket-write story; every Set*/Send* setpoint method
//  needs the "who is allowed to change ATC temperature/mode/run-state"
//  story; the FileSocket family needs the ATC-recipe-transfer ownership
//  story. Never as a side effect of "finishing the facade".
//
//  ============================================================================
//  DENOMINATOR -- MEASURED THIS WAVE, NOT INHERITED, AND CORRECTED (READ THIS)
//  ============================================================================
//  `python tools/census/wave_preflight.py ATC/ATC_Handler_Side.cpp` (20260827)
//  reports: golden has 168 `TATC_InterfaceForm::` method DEFINITIONS, 0
//  file-scope, 3,713 span lines; span-sanity "census sees 168 bodies /
//  stripped sees 168 / swallowed 0" -- no comment-swallowed-brace risk.
//
//  ⚠ DISCREPANCY, RE-WEIGHED THIS WAVE: census.py's `functions()` keys its
//  result by NAME, so golden's two `ChangeRecipe` OVERLOADS
//  (`ChangeRecipe(AnsiString)` golden :1920-1925, 6 lines, and
//  `ChangeRecipe(AnsiString, double)` golden :1927-1935, 9 lines -- both
//  real, distinct, `//----` -delimited bodies, confirmed by direct read) fold
//  into ONE dict slot, silently dropping the first overload from the count.
//  A raw `grep -cE '^\S.*TATC_InterfaceForm::\w+\('` over the golden .cpp
//  returns 169, and reading golden :1918-1936 confirms both bodies are real
//  and independent. So: THE TRUE DENOMINATOR IS 169 bodies / 3,719 span
//  lines (3,713 + the 6-line first `ChangeRecipe` overload census dropped),
//  not census's 168/3,713. This is the SAME class of undercount the campaign
//  docs already flag for census.py's other two known conservative defects
//  (KNOWLEDGE.md "census.py那兩個「檔名比對缺陷」") -- do not "fix"
//  census.py for this; it is documented here per-wave instead, per
//  established policy.
//
//  Separately: golden's header declares THREE more methods
//  (`GetTempReady`, `SendAllRefrigeratorFullOpen`, `SetReadTemp`) that have
//  NO body anywhere in ATC_Handler_Side.cpp (verified: a name-diff between
//  every `__fastcall <Name>(` in the .h and every `TATC_InterfaceForm::
//  <Name>(` in the .cpp leaves exactly these three on the header-only side).
//  They are outside the 169-body ledger entirely -- not translated, not
//  declared here, not counted as gated (there is nothing to gate).
//
//  THIS WAVE: 28 of 169 delivered (16.6%), 141 gated. BY LINES (golden
//  spans, end-start+1): SetCommandString 137 + ctor 132 (of which only ~35
//  lines are actually translated -- see DEVIATION D-2) + Get_ATC
//  RefrigeratorAtcSetMode 20 + CheckATCSetTemp 14 + GetSetTemp 11 +
//  GetSetOffset 11 + PushBackMessage 9 + ReadTC/ReadTC2/ReadTJ 9 each (27) +
//  HasAlarmMsg 7 + SetATC_ModuleCount 6 + A31_Set_Temp_Reading_ModeClick 6 +
//  A31_Initial_SetClick 6 + SetModuleRunStop 5 + A31_Chiller_StatusClick 5 +
//  A31_Read_TempClick 5 + GetRunState/GetRunSelfTestResult/
//  GetRunSelfTestStatus/GetATCLotStartStatus/GetATCLotEndStatus 4 each (20) +
//  IsConnect/IsConnecting 4 each (8) + Button2Click 4 + ReadPID/SetPID 3
//  each (6) = 443 of golden's 3,719-line ledger (~11.9%). Quote both numbers
//  or neither. (The ctor's 132-line golden span is credited at its GOLDEN
//  size for the denominator table above, per convention -- the ACTUAL
//  translated-line count for the ctor is much smaller; see D-2.)
//
//  ============================================================================
//  *** THE GLOBALS `ATC_InterfaceForm` AND `ATC_Data` ARE ALREADY TAKEN ***
//  ============================================================================
//  Golden ends ATC_Handler_Side.h:483 with
//  `extern PACKAGE TATC_InterfaceForm *ATC_InterfaceForm;` and :496 with
//  `extern TATC_Data ATC_Data;`. This header deliberately declares NEITHER
//  global, because the port tree already has live, registered stand-ins for
//  both exact names:
//
//      acarry_shims.h:109  class TATC_InterfaceFormShim { public: int
//          iATC_MODE_TYPE; ... };
//      acarry_shims.h:115  extern TATC_InterfaceFormShim *ATC_InterfaceForm;
//      acarry_shims.h:117  struct TATC_DataShim { double dChillerSetTemp;
//          ... };
//      acarry_shims.h:122  extern TATC_DataShim ATC_Data;
//
//  Both shims are LIVE: `DoFloodGateClose` reads
//  `ATC_InterfaceForm->iATC_MODE_TYPE` and `ATC_Data.dChillerSetTemp` and
//  depends on the offline default (iATC_MODE_TYPE==0, i.e. !=36) to take the
//  deterministic SAFE branch (no flood-gate close). `python tools/census/
//  wave_preflight.py ATC/ATC_Handler_Side.cpp` section [3](c) confirms 2 hits
//  (acarry_shims.cpp, acarry_shims.h) for the global name and 0 hits for the
//  CLASS name `TATC_InterfaceForm` (section [3](a): "clear") -- only the bare
//  global pointer and the `ATC_Data` global instance are spoken for, not the
//  class this file defines.
//
//  Two direct consequences for THIS wave's translated bodies:
//    1. `ctor` golden :446 (`ATC_Data.dChillerSetTemp=-999.0;`) is DROPPED
//       (DEVIATION D-2) -- writing through the occupied `ATC_Data` global
//       would silently mutate DoFloodGateClose's live safety input from a
//       constructor nobody currently calls; not touched.
//    2. Any golden BODY that dereferences the global `ATC_InterfaceForm`
//       itself (self-referential through the global instead of `this`) is
//       GATED outright, category GLOBAL below: `Check_ATC_Busy_State`
//       (golden :3362-3372, reads `ATC_InterfaceForm->
//       bATC_ModuleState[...]`), `TesterChangeTemp` (golden :3772-3816,
//       calls `ATC_InterfaceForm->SetSingleTemp(...)`), `ChangeTJMode`
//       (golden :3973-3996, reads `ATC_InterfaceForm->asControlMode[i]`),
//       `SendHandler2DID` (golden :3211-3232, reads `ATC_InterfaceForm->
//       iATC_MODE_TYPE`) -- the shim's `iATC_MODE_TYPE` field would
//       silently satisfy the TYPE-CHECK for that one line but the REST of
//       each body needs fields the shim does not have, so these are gated
//       whole, not partially "fixed" to use the shim.
//
//  ============================================================================
//  ⚠ TRAP #5 RE-VERIFIED: `iATCSelfTestStatus` NAME COLLISION, DIFFERENT
//  CLASS, NOT AN OCCUPATION
//  ============================================================================
//  `ATC/ATCInterface.h:296` (the UNRELATED, no-underscore `TATCInterfaceForm`
//  class, already translated, lives in ht9045_sm) ALSO declares
//  `int iATCSelfTestStatus;`. This is two INDEPENDENT class members with the
//  same spelling in two different classes -- no ODR conflict, no occupation
//  of anything this file needs. Re-verified 20260827 by reading ATC/
//  ATCInterface.h:270-300 directly (also has `bATCRunSetting`/
//  `bATCRunStatus`, similar-but-NOT-identical to this file's `bATCRun`).
//  Recorded per task rule: name similarity across the two ATC* classes
//  costs nothing here, but is exactly the shape trap #5 warns about, so it
//  is written down rather than silently assumed safe.
//
//  ============================================================================
//  BATCH CRITERION FOR THIS WAVE (why these 28 and not others)
//  ============================================================================
//  A method is ACTIVE here only if ALL of the following hold, each checked by
//  reading the FULL golden body (never by name):
//    1. It issues no outbound ATC command -- no direct or transitive call to
//       SendCommand (the sole socket-write path in this class).
//    2. It touches no VCL component member (TCheckBox/TPanel/TMemo/
//       TComboBox/TSpeedButton/TClientSocket/TLabel/TButton/TGroupBox/
//       TPageControl/TTabSheet/TShape/TProgressBar) -- this facade declares
//       NO widgets and NO TForm base (DEVIATION D-3), so a body that reads
//       or writes one cannot be translated without inventing UI state that
//       was never asked for.
//    3. It dereferences neither of the two occupied globals
//       (`ATC_InterfaceForm`, `ATC_Data`) nor an unconstructed global
//       (`fQwertyKey`/`fQwertyKey2` -- forms/fQwertyKey.cpp:41-42 declares
//       them as bare `TfQwertyKey *fQwertyKey;` with NO initializer,
//       matching forms/fPassword.h's own documented NULL-GLOBAL-until-wired
//       posture; `frmFileTransfer` likewise unverified this wave).
//    4. It performs no filesystem or persistent-config write (no
//       MyForceDirectories/CreateDir/DeleteDirectory/TIniFile-write) and no
//       live socket I/O.
//    5. Every symbol it needs already exists in the port tree AND is
//       reachable from ht9045_forms's actual link set (vclcompat +
//       ht9045_globals + ht9045_core -- ht9045_core itself links
//       ht9045_public PUBLIC (CMakeLists.txt:420), so ht9045_public is ALSO
//       transitively reachable; ht9045_forms does NOT link ht9045_sm,
//       ht9045_motor, ht9045_io, ht9045_db). Checked 20260827:
//         Total_Compressor -> cmydef.cpp:5679 (`int Total_Compressor=0;`),
//             declared cmydef.h:5546, ht9045_globals (CMakeLists.txt:474).
//       Every other symbol the 28 ACTIVE bodies touch is a member of THIS
//       class itself (no external link edge introduced at all).
//    6. Every symbol's definition line is NOT inside `#if 0` (n/a here --
//       none of the 28 ACTIVE bodies call outside this class except
//       `SetCommandString` calling nothing external and the ctor calling
//       its own `SetCommandString`).
//  Everything left is an outbound command, a widget dependency, an occupied-
//  or unconstructed-global dereference, a persistence/socket call, a golden
//  body with NO implementation anywhere (GetTempReady/
//  SendAllRefrigeratorFullOpen/SetReadTemp -- not declared at all, see
//  DENOMINATOR), or a thin UI handler whose ENTIRE reason to exist is
//  calling one of the above (CALL category below).
//
//  ============================================================================
//  WAVE SCOPE -- ACTIVE (28).  Each was read line-by-line in full first.
//  ============================================================================
//    ctor                        golden :338-469  PORT SPLIT -- fields only,
//                                 see DEVIATION D-2 for exactly which of the
//                                 132 golden lines survive.
//    SetCommandString            golden :200-336  ACTIVE (pure lookup-table
//                                 builder: ~120 `asCommString[<ID>-1000]=
//                                 "..."` literal assignments; zero reads,
//                                 zero I/O, zero widget/global touch).
//    IsConnect                   golden :1910-1913 ACTIVE (`return
//                                 IsConnectFlag;`).  NAME RESERVED.
//    IsConnecting                golden :1915-1918 ACTIVE (`return
//                                 bConnecting;`).
//    PushBackMessage              golden :553-561  ACTIVE (pure de-dup
//                                 insert into own `sAlarmMessageList`
//                                 TStringList + own `LastMsg` field; no
//                                 widget, no SendCommand).
//    HasAlarmMsg                  golden :2479-2485 ACTIVE (`sAlarm
//                                 MessageList->Count<1` check only).
//    GetSetTemp                   golden :2515-2525 ACTIVE (bounds-checked
//                                 read of own `dNowATCSetTemp[]`).
//    GetSetOffset                 golden :2847-2857 ACTIVE (ditto,
//                                 `dNowATCSetOffset[]`).
//    CheckATCSetTemp              golden :2527-2540 ACTIVE (pure comparison
//                                 loop over own `dNowATCSetTemp[]`).
//    GetRunState                  golden :2542-2545 ACTIVE (`return
//                                 bATCRun;`).  NAME RESERVED.
//    GetRunSelfTestResult         golden :2606-2609 ACTIVE (`return flagATC
//                                 SelfTestResult;`).
//    GetRunSelfTestStatus         golden :2611-2614 ACTIVE (`return
//                                 iATCSelfTestStatus;`).
//    GetATCLotStartStatus         golden :2616-2619 ACTIVE (`return bATCLot
//                                 StartStatus;`).
//    GetATCLotEndStatus           golden :2621-2624 ACTIVE (`return bATCLot
//                                 EndStatus;`).
//    SetATC_ModuleCount           golden :2547-2552 ACTIVE (pure bounds-
//                                 clamped setter of own `iATC_ModuleCount`).
//    SetModuleRunStop             golden :2554-2558 ACTIVE (pure copy loop
//                                 into own `ATC_Module[]`, bounded by own
//                                 `iATC_ModuleCount`).
//    Get_ATCRefrigeratorAtcSetMode golden :3267-3286 ACTIVE (pure read/copy
//                                 over own `iATC_RefrigeratorUserMode[][]`
//                                 bounded by `Total_Compressor`; zero
//                                 SendCommand, zero widget).  NAME RESERVED.
//    ReadTC                       golden :2360-2368 ACTIVE (bounds-checked
//                                 copy from own `dTC[]`).  `dTC` NAME
//                                 RESERVED (the FIELD, not this method).
//    ReadTC2                      golden :2370-2378 ACTIVE (ditto, `dTC2[]`,
//                                 also NAME RESERVED).
//    ReadTJ                       golden :2380-2388 ACTIVE (ditto, `dTJ[]`,
//                                 not itself name-reserved but needed by
//                                 this method).
//    ReadPID                      golden :2390-2392 ACTIVE -- golden's ENTIRE
//                                 body is `{}` (empty).  Translated verbatim
//                                 (a faithful no-op, not a degraded stub).
//    SetPID                       golden :2394-2396 ACTIVE -- ditto, golden
//                                 body is `{}`.
//    A31_Read_TempClick           golden :1710-1714 ACTIVE (`double dTemp[
//                                 ATC_MAX_SITE]; ReadTC(ATC_MAX_SITE,
//                                 dTemp);` -- calls this class's own ACTIVE
//                                 ReadTC; touches no widget, `Sender`
//                                 unused matching golden).
//    A31_Set_Temp_Reading_ModeClick golden :959-964  ACTIVE -- golden's
//                                 entire body is THREE COMMENTED-OUT lines
//                                 (`//A31_Set_Temp_Reading_Mode->Font->
//                                 Color=clBlack; //iSendData[0]=...;
//                                 //SendCommand(ATC_SET_TRM);`). The ACTUAL
//                                 behaviour is a no-op; translated as an
//                                 empty function body, which is exactly
//                                 what golden does at runtime (not a
//                                 degraded translation -- see GOLDEN NOTE
//                                 G-1).
//    A31_Initial_SetClick         golden :972-977  ACTIVE -- same shape as
//                                 above, golden body is three commented-out
//                                 lines, translated as empty (GOLDEN NOTE
//                                 G-1).
//    Button2Click                 golden :1871-1874 ACTIVE -- golden body is
//                                 ONE commented-out line
//                                 (`//SendCommand(ATC_CANCEL_MESSAGE);`),
//                                 translated as empty (GOLDEN NOTE G-1).
//    A31_Chiller_StatusClick      golden :1876-1880 ACTIVE -- golden body is
//                                 two commented-out lines, translated as
//                                 empty (GOLDEN NOTE G-1).
//
//  ============================================================================
//  GATED -- CATEGORY NET: CALLS SendCommand (DIRECTLY) -- OUTBOUND ATC
//  COMMAND / SETPOINT / MODE-SWITCH (83 -- corrected from an initial
//  miscount of 67; recounted directly against the enumerated list below,
//  20260827)
//  ============================================================================
//  Every one of these assembles `iSendData[]`/`asSendData[]` and calls
//  `SendCommand(ATC_*)`, which is itself gated (see CATEGORY NET-ROOT) --
//  translating any of these ACTIVE would either fail to link (calling an
//  undefined `SendCommand`) or silently drop the one line that gives the
//  function its purpose. Content-verified by reading each full body, not by
//  name (several -- Get_ATCRefrigeratorAllStatus, SendHandler2DID,
//  SendSwitchRefrigerator, Send_ATC_DewPoint, SetAirValve, SetManualSelfTest,
//  SetOffset, SetRunSelfTest, SetSingleTemp, SiteTesting, StartTesting,
//  Stop, TestFinish, UseTSD_Function -- are on the 37-name-reserved list,
//  which is orthogonal to the active/gate decision).
//    SendCommand golden :624-924 (301L, NET-ROOT -- explicit task-brief
//        exclusion, the sole socket-write path this whole category funnels
//        through)
//    ClearATCMessage :1864-1869; SetChannelCount :1937-1993 (also WIDGET);
//    SetAddressPort :1995-2010 (also SOCK); Run :2012-2019 (also WIDGET,
//        NAME RESERVED); Stop :2021-2046 (also WIDGET, NAME RESERVED);
//    SetAllTemp :2048-2076 (also WIDGET, NAME RESERVED); SetMultiZoneTemp
//        :2078-2108; SetAirValve :2110-2146 (NAME RESERVED); SetTemp
//        :2148-2167 (also WIDGET); SetSingleTemp :2169-2184 (also WIDGET,
//        NAME RESERVED); SetOffset :2186-2220 (also WIDGET, NAME RESERVED);
//    SetFFCOffset :2222-2247; SetSingleOffset :2249-2265 (also WIDGET);
//    EnablesChannel :2267-2300 (also WIDGET); StartTesting :2302-2307 (NAME
//        RESERVED); TestFinish :2309-2314 (NAME RESERVED); SiteTesting
//        :2316-2331 (NAME RESERVED); HandlerArm :2333-2358 (NAME RESERVED);
//    SetMaxTemperature :2452-2456; Set2ndRange_P :2560-2575; Set2ndRange_N
//        :2577-2592; Set2ndTime :2594-2598; Set2ndFunction :2600-2604;
//    SetRunSelfTest :2626-2632 (NAME RESERVED); SetManualSelfTest
//        :2634-2640 (NAME RESERVED); QuerySelfTestStatus :2642-2647;
//        SendHandlerStatus :2649-2653; UseTSD_Function :2684-2692 (NAME
//        RESERVED); UseTJ_MODE :2694-2701; EMG_UpDown :2703-2707;
//    GetSW_Ver :2785-2790; GetFW_Ver :2792-2796; GetNowRecipeFile
//        :2859-2865 (also WIDGET); GetRecipeFileList :2867-2874 (also
//        WIDGET); SetHandlerVersion :2886-2891; SetChillerTemp :2908-2920;
//        SetTempReadMode :2933-2945; SetTJOffset :2947-2964; GetATC_2ND_
//        Status :2971-2975; LotStart :2982-2988 (also WIDGET); LotEnd
//        :2990-2996 (also WIDGET, NAME RESERVED); QuerySelfTestResult
//        :2998-3002; SendAbnormalSite :3010-3018; GetControllerSN
//        :3027-3032;
//    ChannelFFCTrigger :3069-3121 (also WIDGET); FFCOffsetEnable
//        :3123-3136; AskATCDateToHandle :3155-3186 (5x SendCommand);
//        GetChillerStatus :3203-3207; SendHandler2DID :3211-3232 (also
//        GLOBAL, NAME RESERVED); SendSwitchRefrigerator :3238-3248 (NAME
//        RESERVED); Get_ATCRefrigeratorAllStatus :3250-3265 (NAME
//        RESERVED); Send_AtcDefrostStatus :3288-3292; SendAirMachineStatus
//        :3296-3359 (64L, explicit task-brief exclusion); Send_ATC_DewPoint
//        :3374-3380 (NAME RESERVED); SendEnableAtcAtcFreonRecover
//        :3382-3386; Send_ATCSETTJENABLED :3388-3392; Send_ATCSETTJ
//        PARAMETER :3394-3399; Send_HANDLERTESTSITEMAPPING :3401-3413;
//        SetAllTemp_List :3415-3440; Send_ATCWATCHDOG :3442-3455;
//        Set_ASIF_TJ_EFUSED :3457-3481; Get_ASIF_TJ_REQUEST :3483-3486;
//        Get_ASIF_TJ_FB :3488-3491; SetTC2Offset :3493-3513; Send_ATCSetPF
//        Parameter :3515-3523; PFMode :3525-3565; ATCCONTROLMODEMode
//        :3567-3706 (140L, explicit task-brief exclusion); GetPFCPARAMETER
//        :3708-3711; SetPFCPARAMETER :3713-3717; GetSLOPEOFFSET
//        :3719-3723; SetSLOPEOFFSET :3725-3730; GetVOLTAGE :3732-3735;
//        RECORDTJTEMP :3737-3759; QUERYTJTEMP :3761-3765; ATCGETCONTROLMODE
//        :3767-3770; SetTCWaterValve :3902-3919; ReadTCWaterValue
//        :3921-3925 (NAME RESERVED); SetDynamicPID :3927-3964; ReadDynamic
//        PID :3966-3971; SetMultiSensorOffset :3998-4019; EnablesMulti
//        Sensor :4021-4033.
//
//  ============================================================================
//  GATED -- CATEGORY WIDGET: TOUCHES A VCL COMPONENT MEMBER (13, not already
//  listed under NET)
//  ============================================================================
//  This facade declares no TForm base and no `__published` widgets
//  (DEVIATION D-3); every body below reads or writes a real golden
//  component (TSpeedButton/TEdit/TComboBox/TMemo/TLabel/TPageControl/
//  TTabSheet/TCheckBox array/etc.) that has no port on this class.
//    ATC_SetChannelEnabledClick :979-993 (widget + calls gated
//        EnablesChannel); TimerTimer :1692-1708 (Label7 + calls gated
//        SendReadTempComm); ATC_ConnectBtnClick :2398-2412 (widget + calls
//        gated Connect/Disconnet); ShowATC_Page :2414-2442 (widget +
//        TIniFile write, also FS); GetAlarmMsg :2487-2504 (AlarmMsg TMemo);
//        PID_SetClick :2656-2677 (widget + calls gated SendCommand path);
//        CommFlagTimerTimer :2803-2845 (ChTempTC/ChTempTC2/ChTempTJ arrays);
//        AutoCheckList :2893-2906 (Hint/Text on TEdit); A31_TemperatureClick
//        :3188-3191 (also GLOBAL, fQwertyKey); A31_Temp_OFSClick
//        :3193-3196 (also GLOBAL); ed_PClick :3198-3201 (also GLOBAL);
//        btnFFCTriggerClick :3039-3067 (widget + calls gated
//        ChannelFFCTrigger); btnFFCEnableClick :3138-3153 (widget + calls
//        gated FFCOffsetEnable).
//
//  ============================================================================
//  GATED -- CATEGORY GLOBAL: DEREFERENCES AN OCCUPIED OR UNCONSTRUCTED
//  GLOBAL (4, not already listed above)
//  ============================================================================
//    Check_ATC_Busy_State golden :3362-3372 -- reads `ATC_InterfaceForm->
//        bATC_ModuleState[...]`, writes global `bATCBusy` (itself
//        unreferenced anywhere else in the port tree, verified 20260827:
//        `grep -rn bATCBusy` over the tree hits only this file's own golden
//        text).
//    TesterChangeTemp golden :3772-3816 -- calls `ATC_InterfaceForm->
//        SetSingleTemp(...)` (also NET category in spirit, but the global
//        deref is the blocking ground: the shim has no `SetSingleTemp`).
//    ChangeTJMode golden :3973-3996 -- reads `ATC_InterfaceForm->
//        asControlMode[i]` (shim has no such field).
//    ReadATCModeType golden :2444-2450 -- reads `asATCFilePath` via `new
//        TIniFile(asATCFilePath)`; golden's own ctor never initialises
//        `asATCFilePath` (confirmed: absent from the 132-line ctor read in
//        full), so this would open/create an INI file at whatever an
//        uninitialised-then-default-"" AnsiString resolves to -- exactly
//        the "value with no load path drives a real action" shape
//        KNOWLEDGE.md's ungate-checklist warns about. Also touches
//        `TIniFile`, an FS-adjacent type; gated on BOTH grounds.
//
//  ============================================================================
//  GATED -- CATEGORY SOCK: LIVE SOCKET CONNECT/DISCONNECT/ERROR/READ (14)
//  ============================================================================
//  vclcompat DOES port TClientSocket/TCustomWinSocket/TErrorEvent
//  (vclcompat/ClientSocket.h), so these are not blocked on a missing type.
//  Gated because they are LIVE network I/O whose ownership (who is allowed
//  to hold the ATC socket, alongside WebBridge's own sockets) this
//  read-only wave is not positioned to decide -- same reasoning as forms/
//  fHS.h's CATEGORY F.
//    ClientSocketConnect :536-551 (also calls gated SendCommand);
//    ClientSocketDisconnect :563-592; ClientSocketRead :594-622;
//    ClientSocketError :2458-2477; Connect :1882-1898; Disconnet
//    :1900-1908 (also GLOBAL: `ATC_InterfaceForm->Caption`); FileSocket
//    Connect :4035-4044; FileSocketDisconnect :4046-4053; FileSocketError
//    :4055-4063; FileSocketRead :4065-4070; GET_ATC_Recipe :4072-4082 (also
//    FS: DeleteDirectory + CreateDir); ATC_Recipe_FileTransfer :4084-4096;
//    ProcessReceiveString_ATC :995-1690 (696L, explicit task-brief
//    exclusion -- the inbound-frame counterpart of SendCommand);
//    SendReadTempComm :1716-1831 (116L, explicit task-brief exclusion --
//    periodic outbound poll).
//
//  ============================================================================
//  GATED -- CATEGORY FS: FILESYSTEM / PERSISTENT-CONFIG WRITE (2, not
//  already listed above)
//  ============================================================================
//    CheckFolderExist golden :2709-2723 -- MyForceDirectories x4 (mkdir
//        chain under D:\ATC_Recoder).
//    SaveCommLog golden :2725-2778 (54L, explicit task-brief exclusion) --
//        fopen/fputs/fclose against the same D:\ATC_Recoder tree.
//
//  ============================================================================
//  GATED -- CATEGORY CALL: THIN HANDLER, SOLE PURPOSE IS CALLING A GATED
//  SIBLING (20 -- corrected from an initial miscount of 18; recounted
//  directly against the enumerated list below, 20260827)
//  ============================================================================
//  Each of these has an otherwise-inert body whose only observable action is
//  invoking a method that is itself gated above; translating the wrapper
//  ACTIVE while its only callee is undefined would either not link (if
//  called) or be a pointless empty shell that misrepresents what golden's
//  handler does.
//    A31_RunStopsBtnClick :926-940 (-> Run/Stop, also WIDGET); A31_Set_
//        TempClick :942-947 (-> SetAllTemp, also WIDGET); A31_Set_Temp_
//        OFSClick :949-957 (-> SetOffset, also WIDGET); ATC_Recipe_FileClick
//        :966-970 (-> ChangeRecipe, also WIDGET); A31_Handler_ArmClick
//        :1838-1841 (-> HandlerArm); ATC_Set_Single_Temp_OFSClick
//        :1843-1849 (-> SetSingleOffset, also WIDGET); ATC_Set_Single_
//        TempClick :1851-1857 (-> SetSingleTemp, also WIDGET); A31_Test_
//        StartClick :1859-1862 (-> StartTesting); A31_GetAlarmMsgClick
//        :2506-2513 (-> gated GetAlarmMsg); EMG_UP_DOWNClick :2679-2682
//        (-> EMG_UpDown, also WIDGET); cBox_RecordCommClick :2780-2783
//        (-> gated SaveCommLog, also WIDGET); SecondSensorFunctionClick
//        :2798-2801 (-> Set2ndFunction, also WIDGET); btnGetNowRecipeFile
//        Click :2876-2879 (-> GetNowRecipeFile); btnGetRecipeFileListClick
//        :2881-2884 (-> GetRecipeFileList); BtnSetTSDClick :2922-2931
//        (-> UseTSD_Function, also WIDGET); SpeedButton1Click :2966-2969
//        (-> SetRunSelfTest); SpeedButton2Click :2977-2980 (-> Set
//        ManualSelfTest); btnGetSelfTestResultClick :3004-3008 (-> Query
//        SelfTestResult); SpeedButton3Click :3020-3025 (-> SendAbnormal
//        Site, also WIDGET); SpeedButton4Click :3034-3037 (-> Get
//        ControllerSN).
//
//  ============================================================================
//  GATED -- CATEGORY MISC: FormShow / FormDestroy / InitialATCSet /
//  ChangeRecipe overloads (5)
//  ============================================================================
//    FormShow golden :471-478 -- `Left=100; Top=10;` needs a TForm base this
//        facade does not have (D-3); the `#ifdef SOFT_SIMULTE` branch calls
//        the also-gated InitialATCSet.
//    InitialATCSet golden :480-485 -- SetChannelCount (WIDGET) + Set
//        ReadTempInterval (would be safe alone, but 2 of 3 lines are not) +
//        SetAddressPort (SOCK).
//    FormDestroy golden :487-534 -- deletes the five widget-pointer arrays
//        this facade does not declare, plus the established
//        canary_support.cpp `MyDBIProcess`/`LogSoftwareOffTime` edge (same
//        edge forms/fHS.h cites, but reusing it here would be for a body
//        that is 80% widget cleanup this facade has nothing to clean up).
//    ChangeRecipe(AnsiString) golden :1920-1925 -- widget (`ATC_RecipeFile->
//        Text`) + SendCommand.
//    ChangeRecipe(AnsiString, double) golden :1927-1935 -- ditto.
//
//  ============================================================================
//  GOLDEN NOTES (faithful; recorded, NOT "fixed")
//  ============================================================================
//  (G-1) Four ACTIVE handlers -- A31_Set_Temp_Reading_ModeClick, A31_
//      Initial_SetClick, Button2Click, A31_Chiller_StatusClick -- have
//      golden bodies that are ENTIRELY `//`-commented-out statements (a
//      button click that used to do something and was disabled in place,
//      not removed). Translating them as empty functions is the FAITHFUL
//      translation, not a degraded one: golden's own compiled behaviour at
//      that call site is already "do nothing".
//  (G-2) ReadPID/SetPID (golden :2390-2396) are declared with real
//      parameters (`int*, double*, int*` / `int, double, int`) but golden's
//      bodies are `{}` -- never implemented, still declared+called
//      elsewhere presumably as a future stub. Translated verbatim as `{}`.
//  (G-3) golden's own header (ATC_Handler_Side.h) declares three more
//      methods -- `GetTempReady`, `SendAllRefrigeratorFullOpen`,
//      `SetReadTemp` -- with NO body anywhere in ATC_Handler_Side.cpp
//      (confirmed by full name-diff, 20260827). These are not part of this
//      wave's 169-body ledger and are not declared on this facade at all.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) CTOR carries a SUBSET of golden's 132 lines (task rule 5: fields
//      only, plus this file's own stricter "only fields an ACTIVE method
//      touches, or a name-reserved field" criterion -- see BATCH CRITERION).
//      KEPT (golden line -> what): :341 IsConnectFlag=false; :342
//      bConnecting=false; :347 asATC_SW_Ver="" (NAME RESERVED); :349
//      iATC_MODE_TYPE=ATC_TYPE_UNSET (NAME RESERVED); :355 ReadTempInterval
//      =1000; :357 iATC_ModuleCount=1; :368 flagATCSelfTestResult=0 (NAME
//      RESERVED); :390-391 sAlarmMessageList=new TStringList(); ->Clear()
//      (needed by ACTIVE HasAlarmMsg/PushBackMessage -- WITHOUT this the
//      first call to either would deref a null TStringList*, the exact
//      "static-init ctor derefs a null global/member" shape task rule 4
//      warns about); per-site loop :393-432 trimmed to ONLY `ATC_Module[i]
//      =false;` (needed by ACTIVE SetModuleRunStop); :448 LastMsg="" (needed
//      by ACTIVE PushBackMessage); :449 SetCommandString() (calls this
//      class's own ACTIVE method, not an external wire-up); :451-455
//      iATC_RefrigeratorUserMode[][] init loop (NAME RESERVED, needed by
//      ACTIVE Get_ATCRefrigeratorAtcSetMode); :457-463 dATC_
//      RefrigeratorAllStatus[][] init loop (NAME RESERVED field); :464-468
//      dATC_RefrigerantMachineHighLimit/LowLimit/bReadRefrigerantMode_
//      AllStatus/Send/Recv (all NAME RESERVED fields).
//      DROPPED: :339 `: TForm(Owner)` base-init (D-3, no TForm base); :350-
//      354 five TEdit/TPanel geometry/property writes (WIDGET); :343-346,
//      356, 358-367 fields no ACTIVE method touches and that are not name-
//      reserved (bOpenFlag/dCommLogFileSize/asData/asDataTemp/asHandlerVer/
//      iHandlerSiteCount/iATCTempIndex/iATCTempIndexCount/bATC2nd
//      FunctionFlag/bGetATC_ModuleStateFlag/bHandlerOnlineFlag/bGetNow
//      RecipeFlag/bGetRecipeListFlag/bGetATC_SW_VerFlag/iATC_DataCount);
//      :370-373 ATC_WorkFileList[5000]="" loop (field not declared -- not
//      ACTIVE-needed, not reserved); :375-378 asATC_ErrorMsg[8]="" loop
//      (ditto); :380-385 iCommCount[]/bCommFlag[]/asCommString[]="" reset
//      loop (asCommString is about to be fully repopulated by
//      SetCommandString() two lines later regardless, and iCommCount/
//      bCommFlag are not declared -- not ACTIVE-needed, not reserved);
//      :387-389 csReceiveString/slReveiveStringList/sDataList allocations
//      (fields not declared -- only GATED bodies would use them); the per-
//      site loop's five widget-construction blocks (:395-422, WIDGET) and
//      the P_Value/I_Value/D_Value/asATC_FW_Ver/bATC_ModuleState/
//      asControllerSN assignments inside it (:424-431, fields not ACTIVE-
//      needed, not reserved); :436-444 PageControl/TabSheet visibility
//      (WIDGET); :446 `ATC_Data.dChillerSetTemp=-999.0;` (occupied global,
//      see safety-posture banner above); :433-434 bGetControllerSN_Flag/
//      bGetControllerFW_Flag=false (fields not ACTIVE-needed, not
//      reserved).
//  (D-3) `TComponent* Owner` / `: TForm(Owner)` base-init dropped -- no
//      facade-side TForm base (matches forms/fHS.h D-3, forms/fIoSetView.h
//      D-4: no delivered method needs one).  `TATC_InterfaceForm` has NO
//      base class here.
//  (D-4) golden `private:` members (IsConnectFlag/bConnecting/LastMsg/
//      COMM_Log/dCommLogFileSize -- the last two not declared, see D-2)
//      collapsed to `public:` -- established convention (forms/fHS.h D-4,
//      forms/fPassword.h D-4).
//  (D-5) `TObject *Sender` kept on every delivered handler signature that
//      golden gives one (A31_Read_TempClick / the four commented-out-body
//      click handlers), even though unused in the translated body --
//      golden signature, vclcompat::TObject exists, no handler is wired to
//      anything (task rule 5).
//  (D-6) `String` (golden's VCL alias) not used anywhere in the 28 ACTIVE
//      signatures (golden's `LotStart(String)`/`LotEnd(String)` are both
//      GATED); GATED declarations that DO use it (LotStart/LotEnd) spell it
//      `AnsiString` per the tree's dominant convention (matches forms/
//      fHS.h D-6).
//  (D-7) GATED declarations use `TMemo*` / `TObject*` / `TCustomWinSocket*`
//      / `TErrorEvent` exactly as golden declares them (all four are real
//      ports: vclcompat/Controls.h, vclcompat/vcl_compat.h, vclcompat/
//      ClientSocket.h) so a future un-gate's signature matches without
//      re-deriving types.  No GATED signature in this file needs a widget
//      type this port lacks (TCheckBox/TPanel/TComboBox/TSpeedButton/
//      TClientSocket/TLabel/TButton/TGroupBox/TPageControl/TTabSheet/
//      TShape/TProgressBar/TTimer/TCriticalSection are all used only
//      INSIDE golden bodies via member fields, never as a parameter or
//      return type of any of the 169 signatures -- verified by reading
//      every declaration in ATC_Handler_Side.h:152-480).
//  (D-8) Every scalar/array field declared below carries an explicit
//      default-member-initializer (`= false` / `= 0` / `= {}`), REGARDLESS
//      of whether the trimmed ctor (D-2) also assigns it -- established
//      convention (forms/fHS.h: `bool bFTPError = false;` etc.), and the
//      only way to give golden's own genuinely-uninitialised members
//      (dTC/dTC2/dTJ/dNowATCSetTemp/dNowATCSetOffset/iATCSelfTestStatus/
//      bATCLotStartStatus/bATCLotEndStatus/bATCRun/bGetATC_SEND_TEMP_READY
//      -- none of which golden's OWN ctor ever touches either, confirmed by
//      the full ctor read) a defined value instead of indeterminate
//      class-member content. This is a deliberate, disclosed hygiene
//      deviation, not a behaviour claim about golden.
//  (D-9) `struct stcAirMachineInfo` (golden :434-447) reproduced verbatim
//      as a nested type with its two instances `AirMachineInfo`/
//      `AirMachineInfo_Index` -- both NAME RESERVED, neither touched by any
//      ACTIVE method (declared for the reservation only, per the same
//      "declare even if only the name is spoken for" rule as every GATED
//      method).  golden's OTHER embedded struct, `stcSend_Rec_
//      AirMachineStatus` (:413-417, `Send_Rec_AirMachineStatus`/
//      `Send_Rec_AirMachineStatus_Index`), is NOT reserved and NOT ACTIVE-
//      needed -- not declared.
//
//  ============================================================================
//  ABSENCE RE-RUN AT WAVE CLOSE -- 20260827
//  ============================================================================
//  Absence claims go stale (docs/KNOWLEDGE.md) and this repo runs parallel
//  waves. Re-run immediately before writing this banner's final text:
//    global `TATC_InterfaceForm` / class `TATC_InterfaceForm` (outside this
//        file) -- 0 hits (`python tools/census/wave_preflight.py ATC/
//        ATC_Handler_Side.cpp` section [3](a): "clear").
//    global `ATC_InterfaceForm` (outside this file) -- still exactly the
//        `TATC_InterfaceFormShim` owner (acarry_shims.h:109/:115,
//        acarry_shims.cpp), still the LIVE `iATC_MODE_TYPE` call sites
//        (acarry.cpp:8349, aTester_Front.cpp:479 ...).
//    global `ATC_Data` (outside this file) -- still exactly the
//        `TATC_DataShim` owner (acarry_shims.h:117/:122).
//    `bATCBusy` (any definition) -- still 0 hits tree-wide outside this
//        file's own golden text.
//    `fQwertyKey =` (an actual construction, not just the bare declaration)
//        -- forms/fQwertyKey.cpp:41 is still a bare `TfQwertyKey
//        *fQwertyKey;` with no initializer.
//  Method: same grep-based re-check style as forms/fHS.h's own ABSENCE
//  RE-RUN, run against the corpus excluding build*/.git/.svn/.pti_frames/
//  web.
//
//  ============================================================================
//  SYMBOLS THIS TU NEEDS (for the CMakeLists landing decision -- NOT made
//  here; this wave may not touch CMakeLists.txt)
//  ============================================================================
//    vclcompat      -- AnsiString/TObject (vcl_compat.h), TMemo (Controls.h,
//                      appears ONLY in the GATED SaveCommLog signature, no
//                      ACTIVE body needs it), TCustomWinSocket/TErrorEvent
//                      (ClientSocket.h, GATED signatures only), TStringList
//                      (TStringList.h, sAlarmMessageList).
//    ht9045_globals -- Total_Compressor (cmydef.cpp:5679) -- the ONLY
//                      external symbol any of the 28 ACTIVE bodies touch;
//                      every other symbol is a member of this class itself.
//  Expected landing: ht9045_forms (same layer as forms/fHS.cpp, forms/
//  fContact.cpp, forms/fIoSetView.cpp). Introduces NO NEW archive edge --
//  ht9045_forms already links ht9045_globals for every other form in this
//  directory.
//
//  ============================================================================
//  WHAT REMAINS / NOT DONE THIS WAVE (explicit, so scope-narrowing is not
//  silently absorbed -- task rule 9)
//  ============================================================================
//  - ProcessReceiveString_ATC (696L) and SendCommand (301L) were named in
//    the task's own exclusion list before work started; SendCommand was
//    read in full (needed to classify every NET-category caller correctly)
//    but ProcessReceiveString_ATC was only skimmed (~200 of 696 lines,
//    enough to confirm it is inbound-frame parsing with heavy widget
//    touch) -- its own internal structure beyond that is UNCATALOGUED.
//  - ATCCONTROLMODEMode (140L) and SetTempPID (83L) were both read in full
//    (both explicitly excluded by the task banner); SetTempPID's body is
//    worth flagging even though gated either way -- golden's ENTIRE
//    function body (:3819-3899) is a `/*...*/`-commented block, i.e. it is
//    ALREADY a no-op in the shipped binary, the same "commented-out
//    function" shape as the four ACTIVE click handlers (GOLDEN NOTE G-1) --
//    but kept GATED per the task brief's explicit name-exclusion rather
//    than reclassified, since the brief named it before this file was
//    opened and re-litigating an explicit exclusion is out of scope for a
//    single wave.
//  - AskATCDateToHandle (32L, 5 SendCommand calls) and PFMode/
//    ATCCONTROLMODEMode's IndexStatus/iGPIBIndexStatus/TestSocket/TestIF/
//    LastSet.bUseTestSocket dependency surface were read enough to confirm
//    the SendCommand gate but NOT traced further (whether those globals
//    themselves have ports was not checked -- moot, since the gate ground
//    is independently sufficient).
//  - CheckFolderExist/SaveCommLog's `D:\ATC_Recoder` log-file convention
//    was read but not cross-checked against the project-wide `D:\HandlerLog`
//    convention forms/fHS.h documents for a different log family -- both
//    are gated regardless, so the discrepancy (if any) was not pursued.
//  - No build/ctest was run by this file's author beyond `g++ -std=c++17
//    -fsyntax-only` on both forms/fATCHandlerSide.h and forms/
//    fATCHandlerSide.cpp -- NOT a cmake configure/build (this wave's task
//    rules explicitly reserve CMakeLists.txt registration for the main
//    loop).
//  - The golden `__published` widget block (Panel1/PageControl/TabSheet20-
//    70/ClientSocket/Timer/A31_*/ATC_Channel[]/ChPanel[]/ChTempTC[]/
//    ChTempTC2[]/ChTempTJ[]/FileSocket/sheRecipe/labRecipe/probRecipe/etc.)
//    is NOT declared anywhere on this facade -- zero ACTIVE method this
//    wave touches a widget, so none were added (matches D-3/D-9).
// =============================================================================
#ifndef FORMS_FATCHANDLERSIDE_H
#define FORMS_FATCHANDLERSIDE_H

#include "vclcompat/vcl_compat.h"    // AnsiString / TObject
#include "vclcompat/Controls.h"      // TMemo (GATED SaveCommLog signature only)
#include "vclcompat/ClientSocket.h"  // TCustomWinSocket / TErrorEvent (GATED signatures only)
#include "vclcompat/TStringList.h"   // TStringList (sAlarmMessageList)

// ===========================================================================
//  golden ATC_Handler_Side.h:17-33 constants, reproduced verbatim (plain
//  top-level #defines in golden, not TATC_InterfaceForm members, so not
//  "occupied" by anything -- re-verified 20260827, 0 hits outside this
//  file for any of these names).
// ===========================================================================
#define ATC_MAX_SITE        40
#define ATC_TYPE_20         20
#define ATC_TYPE_21         21
#define ATC_TYPE_30         30
#define ATC_TYPE_31         31
#define ATC_TYPE_32         32
#define ATC_TYPE_33         33
#define ATC_TYPE_35         35
#define ATC_TYPE_36         36
#define ATC_TYPE_50         50
#define ATC_TYPE_51         51
#define ATC_TYPE_60         60
#define ATC_TYPE_61         61
#define ATC_TYPE_70         70
#define ATC_TYPE_UNSET      9999
#define iATC_Refrigerator_Num  8
#define ATC_MAX_COMMAND     150

// ===========================================================================
//  TATC_InterfaceForm -- non-VCL facade (golden ATC_Handler_Side.h:37-481,
//  `class TATC_InterfaceForm : public TForm`; NO base class here, see
//  DEVIATION D-3). NOT to be confused with golden's UNRELATED,
//  no-underscore `TATCInterfaceForm` (ATC/ATCInterface.h:301, already
//  translated, lives in ht9045_sm) -- see the TRAP #5 note above.
// ===========================================================================
class TATC_InterfaceForm
{
public:
    TATC_InterfaceForm();

    // -- golden private: fields, collapsed to public (DEVIATION D-4) -------
    bool IsConnectFlag = false;             // golden :210 -- ACTIVE (IsConnect)
    bool bConnecting = false;               // golden :211 -- ACTIVE (IsConnecting)
    AnsiString LastMsg;                     // golden :212 -- ACTIVE (PushBackMessage)

    // -- golden public: fields this wave's ACTIVE methods touch -------------
    AnsiString asCommString[ATC_MAX_COMMAND]; // golden :274 -- ACTIVE (SetCommandString)
    TStringList *sAlarmMessageList = nullptr; // golden :222 -- ACTIVE (HasAlarmMsg/PushBackMessage)

    int iATC_MODE_TYPE = ATC_TYPE_UNSET;    // golden :224 -- NAME RESERVED (ctor-only this wave)
    double dTC[ATC_MAX_SITE] = {};          // golden :244 -- ACTIVE (ReadTC); NAME RESERVED (DEVIATION D-8)
    double dTC2[ATC_MAX_SITE] = {};         // golden :245 -- ACTIVE (ReadTC2); NAME RESERVED (DEVIATION D-8)
    double dTJ[ATC_MAX_SITE] = {};          // golden :246 -- ACTIVE (ReadTJ) (DEVIATION D-8)

    bool bATCRun = false;                   // golden :250 -- ACTIVE (GetRunState) (DEVIATION D-8)

    double dNowATCSetTemp[ATC_MAX_SITE] = {};   // golden :254 -- ACTIVE (GetSetTemp/CheckATCSetTemp) (DEVIATION D-8)
    double dNowATCSetOffset[ATC_MAX_SITE] = {}; // golden :255 -- ACTIVE (GetSetOffset) (DEVIATION D-8)
    int flagATCSelfTestResult = 0;              // golden :256 -- ACTIVE (GetRunSelfTestResult); NAME RESERVED
    int iATCSelfTestStatus = 0;                 // golden :258 -- ACTIVE (GetRunSelfTestStatus) (DEVIATION D-8)

    bool bATCLotStartStatus = false;        // golden :260 -- ACTIVE (GetATCLotStartStatus) (DEVIATION D-8)
    bool bATCLotEndStatus = false;          // golden :261 -- ACTIVE (GetATCLotEndStatus) (DEVIATION D-8)

    int ReadTempInterval = 1000;            // golden :264 -- ACTIVE (SetReadTempInterval, ctor)

    int iATC_ModuleCount = 1;               // golden :227 -- ACTIVE (SetATC_ModuleCount, SetModuleRunStop, ctor)
    bool ATC_Module[ATC_MAX_SITE] = {};     // golden :269 -- ACTIVE (SetModuleRunStop, ctor)

    AnsiString asATC_SW_Ver;                // golden :290 -- NAME RESERVED (ctor-only this wave)
    bool bGetATC_SEND_TEMP_READY = false;   // golden :279 -- NAME RESERVED (DEVIATION D-8, unused this wave)

    int iATC_RefrigeratorUserMode[2][iATC_Refrigerator_Num + 1] = {}; // golden :411 -- ACTIVE
        // (Get_ATCRefrigeratorAtcSetMode, ctor); NAME RESERVED

    double dATC_RefrigeratorAllStatus[iATC_Refrigerator_Num][9] = {}; // golden :424 -- NAME RESERVED (ctor-only)
    double dATC_RefrigerantMachineHighLimit = -9999.0; // golden :425 -- NAME RESERVED (ctor-only)
    double dATC_RefrigerantMachineLowLimit  = -9999.0; // golden :426 -- NAME RESERVED (ctor-only)
    bool bReadRefrigerantMode_AllStatus = false;        // golden :427 -- NAME RESERVED (ctor-only)
    bool bReadRefrigerantMode_Send = false;             // golden :429 -- NAME RESERVED (ctor-only)
    bool bReadRefrigerantMode_Recv = false;             // golden :430 -- NAME RESERVED (ctor-only)

    // -- golden :434-447 nested struct + 2 instances, NAME RESERVED, not
    //    touched by any ACTIVE method this wave (DEVIATION D-9) -----------
    struct stcAirMachineInfo
    {
        AnsiString asATC_AirMachineSetTemp;
        AnsiString asATC_AirMachineNowTemp;
        AnsiString asATC_AirMachineSocketBaseTemp;
        AnsiString asATC_AirMachineStatus;
        AnsiString asATC_AirMachineAlarm;
        AnsiString asATC_AirMachineAirStatus;
        int        iDefrostSec = 0;
        double     iATC_AirMachineSocketChTemp[4] = {};
        int        iATC_AirVolume = 0;
        AnsiString asATC_AirMachineIndexGetTemp_Ch1;
        AnsiString asATC_AirMachineIndexGetTemp_Ch2;
    };
    stcAirMachineInfo AirMachineInfo;        // golden :447 -- NAME RESERVED
    stcAirMachineInfo AirMachineInfo_Index;  // golden :447 -- NAME RESERVED

    // -- ACTIVE this wave (defined in forms/fATCHandlerSide.cpp) -----------
    void SetCommandString();                                                   // golden :200-336
    void A31_Set_Temp_Reading_ModeClick(TObject *Sender);                      // golden :959-964  (GOLDEN NOTE G-1)
    void A31_Initial_SetClick(TObject *Sender);                                // golden :972-977  (GOLDEN NOTE G-1)
    void A31_Read_TempClick(TObject *Sender);                                  // golden :1710-1714
    void SetReadTempInterval(int iInterval);                                   // golden :1833-1836
    void Button2Click(TObject *Sender);                                        // golden :1871-1874 (GOLDEN NOTE G-1)
    void A31_Chiller_StatusClick(TObject *Sender);                             // golden :1876-1880 (GOLDEN NOTE G-1)
    bool IsConnect();                                                          // golden :1910-1913 NAME RESERVED
    bool IsConnecting();                                                       // golden :1915-1918
    bool HasAlarmMsg();                                                        // golden :2479-2485
    double GetSetTemp(int iChannel = 0);                                       // golden :2515-2525
    bool CheckATCSetTemp(double dSettemp);                                     // golden :2527-2540
    bool GetRunState();                                                        // golden :2542-2545 NAME RESERVED
    void SetATC_ModuleCount(int iCount);                                       // golden :2547-2552
    void SetModuleRunStop(bool *Enabled);                                      // golden :2554-2558
    int GetRunSelfTestResult();                                                // golden :2606-2609
    int GetRunSelfTestStatus();                                                // golden :2611-2614
    bool GetATCLotStartStatus();                                               // golden :2616-2619
    bool GetATCLotEndStatus();                                                 // golden :2621-2624
    void PushBackMessage(AnsiString aMsg);                                     // golden :553-561
    double GetSetOffset(int iChannel = 0);                                     // golden :2847-2857
    void ReadTC(int iChCount, double *TempTC);                                 // golden :2360-2368
    void ReadTC2(int iChCount, double *TempTC2);                               // golden :2370-2378
    void ReadTJ(int iChCount, double *TempTJ);                                 // golden :2380-2388
    void ReadPID(int *kP, double *kI, int *kD);                                // golden :2390-2392 (GOLDEN NOTE G-2)
    void SetPID(int kP, double kI, int kD);                                    // golden :2394-2396 (GOLDEN NOTE G-2)
    int Get_ATCRefrigeratorAtcSetMode(int iRefrigeratorNum,
                                       int *iTempRefrigeratorUserMode);         // golden :3267-3286 NAME RESERVED

    // -- GATED this wave -- declared, NOT defined.  Calling one of these is
    //    a LINK ERROR by design (see safety posture banner above). ---------
    void FormShow(TObject *Sender);                                                        // golden :471-478   GATE (MISC)
    void InitialATCSet();                                                                   // golden :480-485   GATE (MISC)
    void FormDestroy(TObject *Sender);                                                      // golden :487-534   GATE (MISC)
    void ClientSocketConnect(TObject *Sender, TCustomWinSocket *Socket);                    // golden :536-551   GATE (SOCK)
    void ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);                 // golden :563-592   GATE (SOCK)
    void ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket);                       // golden :594-622   GATE (SOCK)
    void SendCommand(int iCommIndex);                                                       // golden :624-924   GATE (NET-ROOT)
    void A31_RunStopsBtnClick(TObject *Sender);                                             // golden :926-940   GATE (CALL/WIDGET)
    void A31_Set_TempClick(TObject *Sender);                                                // golden :942-947   GATE (CALL/WIDGET)
    void A31_Set_Temp_OFSClick(TObject *Sender);                                             // golden :949-957   GATE (CALL/WIDGET)
    void ATC_Recipe_FileClick(TObject *Sender);                                             // golden :966-970   GATE (CALL/WIDGET)
    void ATC_SetChannelEnabledClick(TObject *Sender);                                       // golden :979-993   GATE (WIDGET)
    void ProcessReceiveString_ATC();                                                        // golden :995-1690  GATE (SOCK -- explicit exclusion)
    void TimerTimer(TObject *Sender);                                                       // golden :1692-1708 GATE (WIDGET)
    void SendReadTempComm();                                                                // golden :1716-1831 GATE (NET -- explicit exclusion)
    void A31_Handler_ArmClick(TObject *Sender);                                             // golden :1838-1841 GATE (CALL)
    void ATC_Set_Single_Temp_OFSClick(TObject *Sender);                                     // golden :1843-1849 GATE (CALL/WIDGET)
    void ATC_Set_Single_TempClick(TObject *Sender);                                         // golden :1851-1857 GATE (CALL/WIDGET)
    void A31_Test_StartClick(TObject *Sender);                                              // golden :1859-1862 GATE (CALL)
    void ClearATCMessage();                                                                 // golden :1864-1869 GATE (NET)
    void Connect();                                                                         // golden :1882-1898 GATE (SOCK)
    void Disconnet();                                                                       // golden :1900-1908 GATE (SOCK/GLOBAL)
    void ChangeRecipe(AnsiString RecipeName);                                               // golden :1920-1925 GATE (MISC)
    void ChangeRecipe(AnsiString RecipeName, double dTemperature);                          // golden :1927-1935 GATE (MISC)
    void SetChannelCount(int iCount);                                                       // golden :1937-1993 GATE (NET/WIDGET)
    bool SetAddressPort(AnsiString asAddress, int iPost);                                   // golden :1995-2010 GATE (SOCK)
    void Run();                                                                             // golden :2012-2019 GATE (NET/WIDGET) NAME RESERVED
    void Stop();                                                                            // golden :2021-2046 GATE (NET/WIDGET) NAME RESERVED
    void SetAllTemp(double dTemperature);                                                   // golden :2048-2076 GATE (NET/WIDGET) NAME RESERVED
    void SetMultiZoneTemp(double dTemperature);                                             // golden :2078-2108 GATE (NET)
    void SetAirValve(bool bControl, bool bOnOff);                                           // golden :2110-2146 GATE (NET) NAME RESERVED
    void SetTemp(int iChCount, double *dTemp);                                              // golden :2148-2167 GATE (NET/WIDGET)
    void SetSingleTemp(int iChannel, double dTemp);                                         // golden :2169-2184 GATE (NET/WIDGET) NAME RESERVED
    void SetOffset(int iChCount, double *dOffset);                                          // golden :2186-2220 GATE (NET/WIDGET) NAME RESERVED
    void SetFFCOffset(int iChCount, double *dOffset);                                       // golden :2222-2247 GATE (NET)
    void SetSingleOffset(int iChannel, double Offset);                                      // golden :2249-2265 GATE (NET/WIDGET)
    void EnablesChannel(int iChCount, bool *Enabled);                                       // golden :2267-2300 GATE (NET/WIDGET)
    void StartTesting();                                                                    // golden :2302-2307 GATE (NET) NAME RESERVED
    void TestFinish();                                                                      // golden :2309-2314 GATE (NET) NAME RESERVED
    void SiteTesting(int iCount, bool *bTesting);                                           // golden :2316-2331 GATE (NET) NAME RESERVED
    void HandlerArm(int iArm);                                                              // golden :2333-2358 GATE (NET) NAME RESERVED
    void ATC_ConnectBtnClick(TObject *Sender);                                              // golden :2398-2412 GATE (WIDGET)
    void ShowATC_Page(int iType);                                                           // golden :2414-2442 GATE (WIDGET/FS)
    void ReadATCModeType();                                                                 // golden :2444-2450 GATE (GLOBAL/FS)
    void SetMaxTemperature(double dTemperature);                                            // golden :2452-2456 GATE (NET)
    void ClientSocketError(TObject *Sender, TCustomWinSocket *Socket,
                            TErrorEvent ErrorEvent, int &ErrorCode);                          // golden :2458-2477 GATE (SOCK)
    AnsiString GetAlarmMsg();                                                               // golden :2487-2504 GATE (WIDGET)
    void A31_GetAlarmMsgClick(TObject *Sender);                                             // golden :2506-2513 GATE (CALL)
    void Set2ndRange_P(int iCount, double *Range);                                          // golden :2560-2575 GATE (NET)
    void Set2ndRange_N(int iCount, double *Range);                                          // golden :2577-2592 GATE (NET)
    void Set2ndTime(int iTime);                                                             // golden :2594-2598 GATE (NET)
    void Set2ndFunction(bool bEnabled);                                                     // golden :2600-2604 GATE (NET)
    void SetRunSelfTest();                                                                  // golden :2626-2632 GATE (NET) NAME RESERVED
    void SetManualSelfTest();                                                               // golden :2634-2640 GATE (NET) NAME RESERVED
    void QuerySelfTestStatus();                                                             // golden :2642-2647 GATE (NET)
    void SendHandlerStatus();                                                               // golden :2649-2653 GATE (NET)
    void PID_SetClick(TObject *Sender);                                                     // golden :2656-2677 GATE (WIDGET)
    void EMG_UP_DOWNClick(TObject *Sender);                                                 // golden :2679-2682 GATE (CALL/WIDGET)
    void UseTSD_Function(int iCount, int *iSiteOn);                                         // golden :2684-2692 GATE (NET) NAME RESERVED
    void UseTJ_MODE(int iChCount, int *iMode);                                              // golden :2694-2701 GATE (NET)
    void EMG_UpDown(bool bDown);                                                            // golden :2703-2707 GATE (NET)
    void CheckFolderExist();                                                                // golden :2709-2723 GATE (FS)
    void SaveCommLog(bool bCloseFile, TMemo *CommMemo);                                     // golden :2725-2778 GATE (FS -- explicit exclusion)
    void cBox_RecordCommClick(TObject *Sender);                                             // golden :2780-2783 GATE (CALL/WIDGET)
    void GetSW_Ver();                                                                       // golden :2785-2790 GATE (NET)
    void GetFW_Ver();                                                                       // golden :2792-2796 GATE (NET)
    void SecondSensorFunctionClick(TObject *Sender);                                        // golden :2798-2801 GATE (CALL/WIDGET)
    void CommFlagTimerTimer(TObject *Sender);                                               // golden :2803-2845 GATE (WIDGET)
    void GetNowRecipeFile();                                                                // golden :2859-2865 GATE (NET/WIDGET)
    void GetRecipeFileList();                                                               // golden :2867-2874 GATE (NET/WIDGET)
    void btnGetNowRecipeFileClick(TObject *Sender);                                         // golden :2876-2879 GATE (CALL)
    void btnGetRecipeFileListClick(TObject *Sender);                                        // golden :2881-2884 GATE (CALL)
    void SetHandlerVersion(AnsiString asVersion);                                           // golden :2886-2891 GATE (NET)
    void AutoCheckList();                                                                   // golden :2893-2906 GATE (WIDGET)
    void SetChillerTemp(double dTemp);                                                      // golden :2908-2920 GATE (NET)
    void BtnSetTSDClick(TObject *Sender);                                                   // golden :2922-2931 GATE (CALL/WIDGET)
    void SetTempReadMode(int iChCount, int *iMode);                                         // golden :2933-2945 GATE (NET)
    void SetTJOffset(int iChCount, double *dOffset);                                        // golden :2947-2964 GATE (NET)
    void SpeedButton1Click(TObject *Sender);                                                // golden :2966-2969 GATE (CALL)
    void GetATC_2ND_Status();                                                               // golden :2971-2975 GATE (NET)
    void SpeedButton2Click(TObject *Sender);                                                // golden :2977-2980 GATE (CALL)
    void LotStart(AnsiString sLotID);                                                       // golden :2982-2988 GATE (NET/WIDGET)
    void LotEnd(AnsiString sLotID);                                                         // golden :2990-2996 GATE (NET/WIDGET) NAME RESERVED
    void QuerySelfTestResult(int iChCount);                                                 // golden :2998-3002 GATE (NET)
    void btnGetSelfTestResultClick(TObject *Sender);                                        // golden :3004-3008 GATE (CALL)
    void SendAbnormalSite(int iChCount, int *AbnormalSite);                                 // golden :3010-3018 GATE (NET)
    void SpeedButton3Click(TObject *Sender);                                                // golden :3020-3025 GATE (CALL/WIDGET)
    void GetControllerSN(int iChCount);                                                     // golden :3027-3032 GATE (NET)
    void SpeedButton4Click(TObject *Sender);                                                // golden :3034-3037 GATE (CALL)
    void btnFFCTriggerClick(TObject *Sender);                                               // golden :3039-3067 GATE (WIDGET)
    void ChannelFFCTrigger(int iChCount, int Arm, bool Enabled);                            // golden :3069-3121 GATE (NET/WIDGET)
    void FFCOffsetEnable(bool Enabled);                                                     // golden :3123-3136 GATE (NET)
    void btnFFCEnableClick(TObject *Sender);                                                // golden :3138-3153 GATE (WIDGET)
    void AskATCDateToHandle();                                                              // golden :3155-3186 GATE (NET)
    void A31_TemperatureClick(TObject *Sender);                                             // golden :3188-3191 GATE (WIDGET/GLOBAL)
    void A31_Temp_OFSClick(TObject *Sender);                                                // golden :3193-3196 GATE (WIDGET/GLOBAL)
    void ed_PClick(TObject *Sender);                                                        // golden :3198-3201 GATE (WIDGET/GLOBAL)
    void GetChillerStatus();                                                                // golden :3203-3207 GATE (NET)
    void SendHandler2DID(int iArm, bool bStartRecord, AnsiString as2DData = "",
                          int iSiteCount = 0);                                                // golden :3211-3232 GATE (NET/GLOBAL) NAME RESERVED
    void SendSwitchRefrigerator(int iRefrigeratorNo, int iRefrigeratorOnOff);                // golden :3238-3248 GATE (NET) NAME RESERVED
    void Get_ATCRefrigeratorAllStatus(bool bAllStatus);                                     // golden :3250-3265 GATE (NET) NAME RESERVED
    void Send_AtcDefrostStatus(int iOpenClose);                                             // golden :3288-3292 GATE (NET)
    void SendAirMachineStatus(int iEnableAirMachine, double dSetSocketTempature,
                               double dSetIndexTempature);                                    // golden :3296-3359 GATE (NET -- explicit exclusion)
    void Check_ATC_Busy_State();                                                            // golden :3362-3372 GATE (GLOBAL)
    void Send_ATC_DewPoint(double dDewPoint_Index, double dDewPoint_InSht,
                            double dDewPointOutSht);                                          // golden :3374-3380 GATE (NET) NAME RESERVED
    void SendEnableAtcAtcFreonRecover(int iOpenClose);                                       // golden :3382-3386 GATE (NET)
    void Send_ATCSETTJENABLED(int iOpenClose);                                              // golden :3388-3392 GATE (NET)
    void Send_ATCSETTJPARAMETER(AnsiString asSlope, AnsiString asOffset);                   // golden :3394-3399 GATE (NET)
    void Send_HANDLERTESTSITEMAPPING(int iChCount, int *iSiteMap);                          // golden :3401-3413 GATE (NET)
    void SetAllTemp_List(double *dTemperature);                                             // golden :3415-3440 GATE (NET)
    void Send_ATCWATCHDOG(bool bTjWatchdog, double dDelaySOT, double dInputVLow,
                           double dInputVHigh, double dContinueSec);                          // golden :3442-3455 GATE (NET)
    void Set_ASIF_TJ_EFUSED(AnsiString Str);                                                // golden :3457-3481 GATE (NET)
    void Get_ASIF_TJ_REQUEST();                                                             // golden :3483-3486 GATE (NET)
    void Get_ASIF_TJ_FB();                                                                  // golden :3488-3491 GATE (NET)
    void SetTC2Offset(int iChCount, double *dOffset);                                       // golden :3493-3513 GATE (NET)
    void Send_ATCSetPFParameter(int iOnOff, int iFullPower, AnsiString asPFSlope,
                                 AnsiString asWGain, int iMany2one);                           // golden :3515-3523 GATE (NET)
    void PFMode(bool bOnOff);                                                               // golden :3525-3565 GATE (NET)
    void ATCCONTROLMODEMode(AnsiString asSetData);                                          // golden :3567-3706 GATE (NET -- explicit exclusion)
    void GetPFCPARAMETER();                                                                 // golden :3708-3711 GATE (NET)
    void SetPFCPARAMETER(AnsiString asSetData);                                             // golden :3713-3717 GATE (NET)
    void GetSLOPEOFFSET();                                                                  // golden :3719-3723 GATE (NET)
    void SetSLOPEOFFSET(AnsiString asSetData);                                              // golden :3725-3730 GATE (NET)
    void GetVOLTAGE();                                                                      // golden :3732-3735 GATE (NET)
    void RECORDTJTEMP(int iStart);                                                          // golden :3737-3759 GATE (NET)
    void QUERYTJTEMP();                                                                     // golden :3761-3765 GATE (NET)
    void ATCGETCONTROLMODE();                                                               // golden :3767-3770 GATE (NET)
    void TesterChangeTemp(int iTemp);                                                       // golden :3772-3816 GATE (GLOBAL)
    void SetTempPID(double dTemp, bool bChange, double dTemp1 = 0.0);                       // golden :3818-3900 GATE (NET -- explicit exclusion; GOLDEN NOTE, body fully commented)
    void SetTCWaterValve(AnsiString asWaterValueGPIB, int iWaterValue);                     // golden :3902-3919 GATE (NET)
    void ReadTCWaterValue();                                                                // golden :3921-3925 GATE (NET) NAME RESERVED
    void SetDynamicPID(AnsiString asSetData);                                               // golden :3927-3964 GATE (NET)
    void ReadDynamicPID();                                                                  // golden :3966-3971 GATE (NET)
    void ChangeTJMode();                                                                    // golden :3973-3996 GATE (GLOBAL)
    void SetMultiSensorOffset(int iChCount, double *dOffset);                               // golden :3998-4019 GATE (NET)
    void EnablesMultiSensor(bool *Enabled);                                                 // golden :4021-4033 GATE (NET)
    void FileSocketConnect(TObject *Sender, TCustomWinSocket *Socket);                      // golden :4035-4044 GATE (SOCK)
    void FileSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);                   // golden :4046-4053 GATE (SOCK)
    void FileSocketError(TObject *Sender, TCustomWinSocket *Socket,
                          TErrorEvent ErrorEvent, int &ErrorCode);                            // golden :4055-4063 GATE (SOCK)
    void FileSocketRead(TObject *Sender, TCustomWinSocket *Socket);                         // golden :4065-4070 GATE (SOCK)
    void GET_ATC_Recipe();                                                                  // golden :4072-4082 GATE (SOCK/FS)
    void ATC_Recipe_FileTransfer(AnsiString asFileSend);                                    // golden :4084-4096 GATE (SOCK)
};
#endif
