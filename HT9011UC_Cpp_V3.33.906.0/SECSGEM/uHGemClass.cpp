//---------------------------------------------------------------------------
//  SECSGEM/uHGemClass.cpp  --  HTGem abstract base (SECS/GEM S,F-handler layer)
//
//  Translation wave: W5 (SECSGEM base layer)
//  Translator: AI(W5-SECSGEM-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemClass.cpp (3625 lines, cp950)
//
//  DISCREPANCY FROM THE ASSIGNED UNIT SCOPING -- READ THIS FIRST
//  ---------------------------------------------------------------------------
//  The unit brief characterized uHGemClass.cpp as "confirmed 0% VCL... only
//  needs vclcompat + a couple of default handler bodies that reference
//  csystem.h's predicate API".  On actually reading golden, that is NOT what
//  is here: HTGem itself never touches VCL directly (true, 0% VCL on ITS OWN
//  members), but 57 of its ~90 methods are near-single-purpose thin wrappers
//  that dereference `HGemPtr` (a `THGem*`) for almost every statement --
//  `HGemPtr->InitLocalHead(...)`, `HGemPtr->DataItemOut(...)`,
//  `HGemPtr->SendLocalData()`, and in a handful of cases direct VCL widget
//  reads THROUGH that pointer (`HGemPtr->strGrdCEID->Cells[...]`,
//  `HGemPtr->chkMoreMessageAbortProcess->Checked`).  `THGem` is defined in
//  uHGemEquipment.h/.cpp (~6200 lines combined) -- explicitly OUT OF SCOPE
//  for this unit (it is the actual byte-level SECS engine + VCL form, not the
//  "site-specific override layer" the brief meant to exclude -- that is
//  uHGemHT9045.h/.cpp, a DIFFERENT, also out-of-scope file).  None of the 57
//  methods reference csystem.h predicates at all (grep confirms zero
//  IndexHasIC/ShuttleHasIc/TestSocketHasIC hits in this file).
//
//  Golden reality: this file is a ~90%-by-body-volume adapter onto a
//  not-yet-translated subsystem.  Faithfully hand-transcribing every method
//  body here would require also fabricating THGem's ~101-distinct-member
//  interface from zero first-hand analysis of uHGemEquipment.h/.cpp -- i.e.
//  effectively also translating (or guessing at) that 6200-line VCL form,
//  which is explicitly out of scope and would not be a faithful translation
//  (it would be invented behavior, not golden-derived).
//
//  RESOLUTION (follows this project's established convention -- see
//  atester.cpp's GetTesterResult/ProcessTestResult, csystem.cpp's
//  DoOneCycleFinishCheck gated branches, etc.): every method whose real body
//  needs `HGemPtr`/`THGem` is GATED (`#if 0 // TODO(...) -- golden :L1-L2`,
//  body omitted -- golden already holds the citable source, no need to
//  duplicate 3000+ lines here) with an ACTIVE minimal stub that compiles
//  clean and returns a conservative/no-op default, so:
//    (a) HTGem is a real, instantiable, link-complete abstract base today
//        (every virtual has SOME body, matching its declared signature),
//    (b) the header's already-fully-inline virtuals (AddSV/AddEC/S7F4_.../
//        S7F24_.../S14F4_.../S110F*/S120F*/S125F4_...) carry over verbatim,
//        UNCHANGED, with zero gating (they were already 0%-coupled in
//        golden), and
//    (c) a future dedicated wave that translates uHGemEquipment.h (THGem)
//        can un-gate each block here mechanically, one at a time, citing the
//        golden line range already recorded in the `#if 0` comment.
//
//  ACTIVE in this file (faithful, zero gating -- genuinely 0%-VCL/0%-THGem):
//    * HTGem::HTGem() / HTGem(THGem*) / HTGem(AnsiString) / ~HTGem()
//      (SecsAlarmMessage / FMessageList TStringList lifetime only).
//    * HTGem::UpdateDataPath(AnsiString) (DataPath=Path;  one-liner).
//    * IsCorrectDateFormat(int,int,int) (free helper; pure leap-year check;
//      golden's only caller, S2F32_DateAndTimeAcknowledge, is itself gated --
//      kept active anyway per this project's "keep pure leaves active even
//      if their only caller is gated" precedent).
//    * SYS_ECChangeID / SYS_ECChangeIDOriginaValue / SYS_ECChangeIDNewValue
//      global AnsiString definitions (never read by ACTIVE code in this
//      file, but harmless, cheap, and matches golden 1:1).
//
//  GATED (23 of the original 57 methods remain gated as of INTEGRATE WAVE 4
//  below, `#if 0` + cited golden range + ACTIVE default stub) -- see each
//  stub's comment for its exact golden line range and (post integrate-wave)
//  an honest note on exactly which missing piece still blocks it.
//  AI(W906-SysModWire) 20260720: refreshed from "44" (INTEGRATE WAVE 2's own
//  count) -- INTEGRATE WAVE 3 un-gated 8 more methods (44-8=36, matching that
//  wave's own note below); grep `^#if 0` count re-verified at 36.
//  AI(W906-SvEcDataItem) 20260720: INTEGRATE WAVE 4 un-gated 13 more methods
//  (36-13=23, matching that wave's own note below); grep `^#if 0` count
//  re-verified at 23.
//
//  TRANSLATION RULES
//  ------------------
//    * __fastcall removed from S7F18's out-of-line definition (golden:
//      `void __fastcall HTGem::S7F18_...`) -- neutralized by vcl_compat.h
//      anyway; dropped per project convention (see .h file-head note).
//    * AnsiString / TStringList via vclcompat.
//    * MyDBIProcess: declared `extern` locally below (project convention --
//      see aHotPlateSubstrate.h:576 for the identical prior-wave forward
//      declaration; not `#include`d here to avoid pulling HotPlate-specific
//      globals into a file that otherwise needs none of them).
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8.  ZERO
//      U+FFFD (verified; see translate-agent report).
//---------------------------------------------------------------------------
//
//  INTEGRATE WAVE (AI(W906-uHGemClass-Unlock) 20260713) -- uHGemClass gate-unlock
//  ---------------------------------------------------------------------------
//  Two prerequisite THGem slices now exist as real, standalone, tested
//  classes: `SecsWireCodec` (byte-level SECS-II item codec -- InitLocalHead/
//  DataItemOut/DataItemIn/GetDataItemLenAndType/StringOut/LocalAcknowledge/
//  SendLocalData[gated]/...) and `SecsSvEcRegistration` (SV/EC registration
//  bookkeeping -- EC_ID/EC_TYPE/EC_Ptr_Max/EC_Ptr_Min/EC_Ptr_Max_Value/
//  EC_Ptr_Min_Value/...). HTGem now embeds one instance of EACH (`WireCodec`/
//  `SvEcReg`, see uHGemClass.h's own design-choice comment on those members).
//
//  Every one of the 57 gated methods was re-read against golden IN FULL (not
//  just its cited line range -- also golden uHGemEquipment.h's THGem member
//  list, to identify every field/method it touches) to determine whether its
//  ENTIRE dependency chain now resolves through WireCodec/SvEcReg alone.
//  REAL RESULT: 10 of 57 do (a much smaller fraction than "most" -- the
//  other 47 need at least one thing neither engine models: a StringGrid-
//  backed CEID/Report-ID/Alarm database, a VCL widget type not in vclcompat
//  [TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/TRadioGroup/TCheckListBox],
//  a THGem-only data member with no engine home [bOnLine, GemMDLN/GemSOFTREV/
//  GemClock, TraceData[10]/bTraceData[10]/iTRID[10]/..., SReceiveDataBackup,
//  MoveCheckCallBack, UpLoadPath, SV_70_UNT1_ReceipeStruct, ...], a THGem
//  method with no engine home [CheckSFFormatOnlyHead, OnLine/OffLine/
//  GetOnLineMode, GetTimeInfo, DataItemOutSV/-NameList/-NameListWithValue/
//  DataItemOutEC/-NameList, DefineReportAcknowledge*/LinkReportAcknowledge*/
//  EnableDisable*, IsValidSVID, SetReceipeDirectoryAndGlobalName, ...], the
//  as-yet-untranslated `GetDataItemLenAndTypeAndDelete` (SIBLING of the
//  peek-only `GetDataItemLenAndType` SecsWireCodec already has -- NOT the
//  same method; by far the single most common blocker, hit by ~20 of the 47),
//  or Borland dos.h `settime()`/`setdate()`/`struct time`/`struct date`
//  (S2F32) -- none of which SecsWireCodec/SecsSvEcRegistration were ever
//  scoped to cover, and none of which this wave invents a stand-in for.
//
//  UN-GATED (10, now calling WireCodec./SvEcReg. instead of HGemPtr->/HGem->):
//    S9F1_UnrecognizedDeviceID, S9F3_Unrecognized_Stream_Function_Type,
//    S9F5_UnrecognizedFunctionType, S9F7_IllegalData,
//    S9F9_TransactionTimerTimeout (golden :2222-2269, all 5 an identical
//      4-call StringOut+InitLocalHead+DataItemOut+SendLocalData shape);
//    S2F26_DiagnosticLoopbackData (golden :1006-1032);
//    S2F42_Host_Command_Acknowledge (golden :1523-1574, incl. its own golden
//      quirk -- the error path's DataItemOut calls with no preceding
//      InitLocalHead, preserved verbatim);
//    S2F44_ResetSpoolingAcknowledge (golden :1576-1584);
//    S2F24_TraceInitializeAcknowledge (golden :992-1000 -- a thin wrapper:
//      calls its own still-GATED …Sub() sibling + LocalAcknowledge, needs
//      nothing else, so it un-gates independently of its Sub);
//    CheckECValue (golden :3192-3560 -- the ONLY one of the 4
//      SetECValue/S2F15_Update/S2F15_Check/CheckECValue "EC value" cluster
//      with zero VCL-widget-cast branch and zero GetDataItemLenAndTypeAndDelete
//      call at the time of THIS wave; its 3 siblings stayed gated for exactly
//      those reasons back then -- see INTEGRATE WAVE 2 below for 2 of the 3
//      un-gating once their sole remaining blocker was translated).
//
//  Every un-gated method's `HGemPtr->X(...)`/`HGem->X(...)` call became
//  `WireCodec.X(...)` (wire-codec ops) or `SvEcReg.X(...)` (EC/SV lists);
//  `HType`/`HTypeStruct` (shared global, already visible via
//  SecsWireCodec.h) needed no change. Nothing else in these 10 bodies
//  changed vs. golden.
//---------------------------------------------------------------------------
//
//  INTEGRATE WAVE 2 (AI(W906-uHGemClass-Unlock2) 20260716) -- SecsWireCodec
//  "WAVE 3" unlocks S7F2 + the S2F15 pair
//  ---------------------------------------------------------------------------
//  SecsWireCodec gained 3 more real methods this wave (see SecsWireCodec.h/.cpp
//  own "WAVE 3" file-head notes): `GetDataItemLenAndTypeAndDeleteSub`/
//  `GetDataItemLenAndTypeAndDelete` (the destructive sibling pair of the
//  peek-only `GetDataItemLenAndType(Sub)` this file already had -- confirmed
//  by direct golden read to be pure SReceiveData bookkeeping, zero new THGem
//  member, zero VCL) and `SendInvalidDataMessageToHost` (a 3-call InitLocalHead
//  +DataItemOut+SendLocalData composer, same shape as the already-un-gated
//  S9F7_IllegalData). Re-reading every still-gated method's cited blocker list
//  against this delta found exactly 3 whose ENTIRE remaining dependency chain
//  now resolves through WireCodec (+ same-class virtual calls, which are not
//  an HGemPtr/HGem dependency and were already permitted for CheckECValue in
//  the prior wave):
//
//  UN-GATED (3 more, 13/57 total now):
//    S7F2_ProcessProgramLoadGrant (golden uHGemClass.cpp:2081-2113 -- sole
//      blocker was GetDataItemLenAndTypeAndDelete; error path was already
//      un-gated S9F7_IllegalData);
//    S2F15_UpdateNewEquipmentConstant (golden uHGemClass.cpp:2884-3024 -- both
//      GetDataItemLenAndTypeAndDelete and SendInvalidDataMessageToHost were its
//      only 2 recorded blockers; calls its own still-gated SetECValue, which
//      simply no-ops for now, exactly as CheckECValue's un-gating already
//      established as an acceptable interim state for this cluster);
//    S2F15_CheckNewEquipmentConstant (golden uHGemClass.cpp:3026-3190 -- same
//      2 blockers, same resolution; calls its own already-un-gated
//      CheckECValue).
//
//  STILL GATED, checked and confirmed NOT resolvable by this wave's delta
//  alone (each needs at least one more thing beyond WireCodec/SvEcReg, so
//  landing them would be forcing a partial translation through):
//    SetECValue itself (needs TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/
//      TRadioGroup dynamic_cast targets, none in vclcompat -- see its own
//      comment, unchanged reasoning);
//    S7F18_DeleteProcessProgramAcknowledge / Process_S7F20_CurrentEPPIDData /
//      S10F4/S10F6 TerminalDisplay* (need GetDataItemLenAndTypeAndDelete --
//      now available -- PLUS a VCL widget/THGem-only-member this wave's
//      SendInvalidDataMessageToHost addition does nothing for: UpLoadPath/
//      DeleteDirectory, TCheckListBox GemRemoteReceipeList,
//      TerminalDisplayIndex/TerminalMemoPtr/... respectively);
//    S2F13/.../S2F30/S2F34/S2F36/S2F38 SV/EC-registration-DB family (need
//      DataItemOutSV/-NameList/-EC/-NameList/-NameListWithValue, none of which
//      this wave translates);
//    S125F2_EnableDisableECDataAcknowledge (needs EnableDisableECDataAll/
//      EnableDisableECData, both StringGrid-backed -- sgSECSECData, confirmed
//      by direct golden read this wave (see stretch-goal recon below), out of
//      this file's scope).
//
//  STRETCH-GOAL RECON (AI(W906-uHGemClass-Unlock2) 20260716): the hand-off
//  also flagged 8 THGem methods as "worth a dedicated look" --
//  CheckSFFormatOnlyHead, IsValidSVID, GetTimeInfo,
//  SendInvalidDataMessageToHost, DeleteAllHostDefineReportID,
//  DeleteAllHostDefineCeid, GetAlarmIndex, EnableDisableAlarmAll/
//  EnableDisableAlarm, EnableDisableECDataAll/EnableDisableECData. Each was
//  read against golden uHGemEquipment.cpp in full:
//    SendInvalidDataMessageToHost: CLEAN (translated above into SecsWireCodec,
//      see its own citation).
//    CheckSFFormatOnlyHead: reads `chkMoreMessageAbortProcess->Checked` (VCL
//      TCheckBox, golden uHGemEquipment.cpp:8684) before ever reaching
//      GetDataItemLenAndTypeAndDelete -- a real VCL dependency this wave's
//      scope (SecsWireCodec.h/.cpp + uHGemClass.h/.cpp only) has no home for;
//      stays gated.
//    IsValidSVID: `SV_ID->IndexOf(SVID)` (golden uHGemEquipment.cpp:3114) --
//      SV_ID IS already a real SecsSvEcRegistration member (SvEcReg.SV_ID),
//      but SecsSvEcRegistration.h/.cpp is OUTSIDE this wave's allowed-file
//      list, so this method cannot be placed anywhere from here; moot anyway
//      since its only caller in this file (golden uHGemClass.cpp:810-985) is
//      blocked by 7 OTHER THGem-only members with no engine home regardless.
//    GetTimeInfo: writes THGem-only members (SystemYear/Month/Date/Hour/Min/
//      Sec/MSec, TimeString, GemClock, iTimeFormat, plus disk-space/memory-
//      status globals -- golden uHGemEquipment.cpp:315-346) that belong to
//      neither WireCodec nor SvEcReg's modeled scope and would be new,
//      unrelated state bolted onto one or the other; its only caller in this
//      file is ALSO blocked by CheckSFFormatOnlyHead above regardless --
//      stays gated.
//    DeleteAllHostDefineReportID / DeleteAllHostDefineCeid / GetAlarmIndex /
//      EnableDisableAlarmAll / EnableDisableAlarm / EnableDisableECDataAll /
//      EnableDisableECData: every one of these 7 iterates a live VCL
//      TStringGrid (stdGridReportID / strGrdCEID / strGrdAlarm / sgSECSECData
//      respectively, confirmed by direct golden read) -- exactly the
//      "StringGrid-backed CEID/Report-ID/Alarm database" category the prior
//      wave's note already named as out of WireCodec/SvEcReg's scope; none
//      translated this wave.
//  Net stretch-goal yield: 1 of 8 (SendInvalidDataMessageToHost) was
//  genuinely clean; the other 7 all hit a real, confirmed blocker (6 VCL
//  StringGrid, 1 VCL TCheckBox + THGem-only members) -- reported honestly
//  rather than forced through.
//---------------------------------------------------------------------------
//
//  INTEGRATE WAVE 3 (AI(W906-SysModWire) 20260720) -- SystemModularInitial
//  wiring wave: ActiveWire indirection + 8 more S,F handlers unlocked
//  ---------------------------------------------------------------------------
//  Design D (see design brief): HTGem gained a `SecsWireCodec *ActiveWire`
//  member (uHGemClass.h), defaulting to `&WireCodec` in all three ctors.
//  MECHANICAL RENAME: every occurrence of `WireCodec.` in the (now) 13
//  previously-un-gated ACTIVE method bodies below became `ActiveWire->`
//  (90 call sites; verified none touch `SvEcReg.`, which is untouched --
//  EC/SV registration state is HTGem's own, not a wire-codec concept, so it
//  does not route through the indirection). Standalone callers (this file's
//  own test, test_uHGemClass.cpp) are unaffected: ActiveWire defaults to
//  &WireCodec, so `hgem.WireCodec.*` pokes remain read by the SAME object
//  ActiveWire dereferences. Historical comments elsewhere in this file below
//  that say "now calling WireCodec." describe THAT wave's own before/after
//  (HGemPtr-> -> WireCodec.) and are left as their own historical record --
//  not rewritten to "ActiveWire->" -- since the code they narrate has, in
//  turn, been renamed uniformly by this note's own mechanical pass.
//
//  UN-GATED (8 more, 13->21/57 total now) -- golden SECSGEM/uHGemClass.cpp
//  line ranges cited at each definition below: S1F1_AreYouThereRequest,
//  S1F2_OnLineData, S1F13_EstablishCommunicationsRequest,
//  S1F14_ConnectRequestAcknowledge, Process_S1F14_ConnectRequestAcknowledge,
//  S1F16_OFFLINEAcknowledge, S1F18_ONLINEAcknowledge, S2F18_DateandTimeData.
//  Every one of their `HGemPtr->InitLocalHead/DataItemOut/DataItemIn/
//  SendLocalData/GetDataItemLenAndTypeAndDelete/StringOut` calls became
//  `ActiveWire->...` (wire-codec ops, same rule as the mechanical rename
//  above); `HGemPtr->GemMDLN/GemSOFTREV/bS1F2_OnLineData/bOnLine/
//  bReceiveEstablishCommunicationsRequest/bWaitEstablishCommunications.../
//  GemClock` (real THGem DATA members) and `HGemPtr->CheckSFFormatOnlyHead(...)/
//  OffLine()/OnLine(...)/GetOnLineMode()/GetTimeInfo()` (real THGem OUT-OF-LINE
//  METHODS, now defined in uHGemEquipment.cpp -- this is what makes this file
//  #include "SECSGEM/uHGemEquipment.h" for the FIRST time, see uHGemClass.h's
//  own updated forward-declaration note) all stay `HGemPtr->`, unchanged --
//  real THGem state/behavior, not a wire-codec concept. HGemPtr==NULL at call
//  time is undefined behavior, same as golden's own pre-AddSV construction
//  window (uHGemHT9045_SV.cpp:61 re-points HGemPtr=HGem there) -- not guarded,
//  by design (see design brief risk R8).
//---------------------------------------------------------------------------
//
//  INTEGRATE WAVE 4 (AI(W906-SvEcDataItem) 20260720) -- SECSGEM closing-waves
//  Wave 1 (design doc DESIGN_SECSGEM_closing_waves.md): SvEcReg embed + SV/EC
//  DataItem family unlocks 13 more S,F handlers
//  ---------------------------------------------------------------------------
//  uHGemEquipment.h's THGem now embeds `SecsSvEcRegistration SvEcReg;` (by
//  value, mirroring HTGem's own already-proven WireCodec/SvEcReg precedent)
//  and gained 5 new real methods (DataItemOutSV/DataItemOutSVNameList/
//  DataItemOutSVNameListWithValue/DataItemOutEC/DataItemOutECNameList) +
//  IsValidSVID + the Report/CEID composer family (SendRepoerID/
//  SendAnnotatedRepoerID/SendCeid/SendAnnotatedCeid) + FormCreate (system SV/
//  EC registration). This resolves every remaining blocker for 8 methods in
//  the §2-category-② "small increment" bucket (S1F4, S1F12, S2F14, S2F30,
//  S6F16, S6F18, S6F20, S103F12) PLUS the 5 §2-category-① "no defect"
//  methods that were always resolvable through WireCodec/strGrdCEID/
//  strGrdAlarm/stdGridReportID alone and simply hadn't been visited yet
//  (S1F24, S5F8, S100F4, S101F6, S101F8) -- landed together in this one wave
//  per the design doc's own bucketing (§3 Wave 1 scope item 4).
//
//  UN-GATED (13 more, 21->34/57 total now; 36->23 remaining-gated) -- golden
//  SECSGEM/uHGemClass.cpp line ranges cited at each definition below:
//    S1F4_SelectedStatusReply (:119-264), S1F12_StatusVariableNamelistReply
//    (:266-324), S1F24_CollectionEventNamelist (:450-587),
//    S2F14_EquipmentConstanData (:609-719), S2F30_EquipmentConstantNamelistReply
//    (:1037-1080), S5F8_ListEnableAlarmAcknowledge (:1892-1923),
//    S6F16_EventReportData (:1925-1960), S6F18_AnnotatedEventReportData
//    (:1962-1997), S6F20_IndividualReportData (:1999-2052),
//    S100F4_ReportAllAlarm (:2437-2450), S101F6 (:2505-2513), S101F8
//    (:2583-2595), S103F12_StatusVariableNamelistReply (:3562-3625).
//
//  MECHANICAL RENAME RULE (same golden-derived split as prior waves): every
//  golden `HGemPtr->InitLocalHead/DataItemOut/DataItemIn/GetDataItemLenAndType/
//  GetDataItemLenAndTypeAndDelete/SendLocalData/LocalAcknowledge/
//  bDisableBinaryShow` (wire-codec primitives/state) became `ActiveWire->...`;
//  golden `HGemPtr->SV_ID`/`HGemPtr->EC_ID` (SV/EC bookkeeping, now living
//  one level down inside THGem's embedded SvEcReg rather than as THGem's own
//  direct members) became `HGemPtr->SvEcReg.SV_ID`/`HGemPtr->SvEcReg.EC_ID`
//  (still routes through HGemPtr -- SvEcReg IS THGem's own state, just
//  nested); golden `HGemPtr->DataItemOutSV/DataItemOutSVNameList/
//  DataItemOutSVNameListWithValue/DataItemOutEC/DataItemOutECNameList/
//  SendCeid/SendAnnotatedCeid/CheckCEIDExist/strGrdCEID/strGrdAlarm/
//  stdGridReportID/MoveCheckCallBack/bReceiveS101F5/bReceiveS101F7` (real
//  THGem methods/data members) stayed `HGemPtr->`, unchanged.
//
//  GOLDEN BUG DISCOVERED AND PRESERVED VERBATIM (flag for review, found via
//  direct golden read while translating -- not previously catalogued):
//  S6F18_AnnotatedEventReportData's "CEID not exist" error path
//  (uHGemClass.cpp:1988) sends `InitLocalHead(6, 16, 0)` -- S,F 6,16, NOT
//  6,18 -- an evident copy-paste bug from S6F16_EventReportData's own,
//  textually-identical error path right above it in golden. NOT corrected
//  here, per this project's faithful-translation mandate; see that method's
//  own inline comment.
//
//  STILL GATED (23 remain; unaffected by this wave's delta, each needs at
//  least one thing beyond SvEcReg/WireCodec/StringGrid): S2F24Sub (Trace
//  member arrays), S2F32 (Borland dos.h clock), S2F34 family x5 (temp-list
//  members), S2F36 family x2 (ditto), S2F38 (ditto), S6F24/S7F18/S7F20/
//  Process_S7F20/S101F2/S101F4/S101F6_StoreHostUploadFile/
//  S101F8_StoreHostUploadFile (spool/upload/recipe subsystem members --
//  Wave 3/3b territory), S10F4/S10F6 (Terminal widget stand-ins),
//  S125F2/SetECValue (VCL widget dynamic_cast cluster) -- see §2 DEFERRED
//  in the design doc for the full breakdown.
//---------------------------------------------------------------------------
//
//  INTEGRATE WAVE 5 (AI(W906-AlarmReportAck) 20260721) -- SECSGEM closing-waves
//  Wave 2: THGem's Alarm/Report-Ack family unlocks the 8 remaining S2F34/
//  S2F36/S2F38/S5F4/S5F6 methods
//  ---------------------------------------------------------------------------
//  uHGemEquipment.h's THGem gained: 2 wire-codec forwarders (InitLocalHead/
//  DataItemOut, pointer overload only), 4 new members (slTempReportID/
//  lTempReportIDContent/slTempCeID/lTempCeIDContent), 3 new Alarm methods
//  (EnableDisableAlarm/EnableDisableAlarmAll/GetAlarmIndex), the Report/Link
//  Acknowledge composer family (ProcessHostSendReportLinkID/
//  ReportAcknowledge+5 wrappers/ReportLinkAcknowledgeError+4 wrappers), and a
//  REAL body for EnableDisableEventReportAcknowledgeError (+ its 2 wrappers)
//  -- replacing the earlier no-op stub. This resolves every remaining
//  blocker for the last 8 of the original 57 gated methods.
//
//  UN-GATED (8 more, 34->42/57 total now; 23->15 remaining-gated) -- golden
//  SECSGEM/uHGemClass.cpp line ranges cited at each definition below:
//    S2F34_DefineReportAcknowledgeSub (:1218-1291), S2F34_ProcessHostSendReportID
//    (:1294-1361), S2F34_DefineReportAcknowledge (:1363-1393),
//    S2F36_LinkEventReportAcknowledgeSub (:1395-1457),
//    S2F36_LinkEventReportAcknowledge (:1460-1477),
//    S2F38_EnableDisableEventReportAcknowledge (:1480-1521),
//    S5F4_EnableDisableAlarmAcknowledge (:1593-1624), S5F6_ListAlarmData
//    (:1626-1887).
//
//  MECHANICAL RENAME RULE (same golden-derived split as prior waves): every
//  golden `HGemPtr->DataItemIn/GetDataItemLenAndType/
//  GetDataItemLenAndTypeAndDelete/InitLocalHead/DataItemOut/SendLocalData/
//  LocalAcknowledge` (wire-codec primitives) became `ActiveWire->...`; golden
//  `HGemPtr->slTempReportID/lTempReportIDContent/slTempCeID/
//  lTempCeIDContent/strGrdAlarm/CheckReportAlreadyDefine/IsValidSVID/
//  DefineReportAcknowledge*/DeleteReportID/DeleteReportIDOfCeid/
//  SetReportIDContent/SaveEventReportData/DeleteAllHostDefineReportID/
//  DeleteAllHostDefineCeid/LinkReportAcknowledgeFormatError/
//  ProcessHostSendReportLinkID/CheckCEIDExist/
//  EnableDisableEventReportAcknowledgeCeidNotExist/...FormatError/
//  EnableDisableEventReport/EnableDisableAlarmAll/EnableDisableAlarm/
//  GetAlarmIndex` (real THGem-only state/methods, no wire-codec engine home)
//  stayed `HGemPtr->`, unchanged.
//
//  DEPENDENCY WIDENING (flagged explicitly): S2F34_DefineReportAcknowledgeSub's
//  golden body branches on `CUSTOMER_CODE==CC_ONSEMI_M` (golden :1241-1248,
//  an empty-bodied `if` -- preserved verbatim, not an omission on this port's
//  part). Neither symbol existed in this file before this wave; `cmydef.h`
//  is now `#include`d (below) for `CUSTOMER_CODE`/`CC_ONSEMI_M`, matching the
//  identical widening uHGemEquipment.cpp's own Bucket-B wave already made for
//  the same header -- zero new CMake link edge needed (uHGemClass.cpp and
//  uHGemEquipment.cpp already share the `ht9045_secsgem` target, which
//  already links `ht9045_core`/`ht9045_globals` for exactly this).
//
//  GOLDEN QUIRK PRESERVED VERBATIM (S5F6_ListAlarmData, golden :1751-1788,
//  the "alarm not found" branch): `DataItemOut(0, HType.BINARY_TYPE, &Mode)`
//  passes length 0 -- `Mode`'s value is never actually transmitted on the
//  wire. This is a genuine golden zero-length SECS-II item, not a
//  translation bug -- see that method's own inline comment. Also preserved:
//  golden `_atoi64` -> `strtoll`/`strtoull` (MinGW <cstdlib> substitution,
//  same precedent SecsSvEcRegistration.cpp/uHGemEquipment.cpp already
//  established) and the `(unsigned)strtoull(...)` 32-bit-truncating cast
//  golden itself applies before storing into a 64-bit `uint8SV` (golden
//  :1720 -- an existing golden quirk, not introduced here).
//
//  STILL GATED (15 remain; unaffected by this wave's delta): S2F24Sub (Trace
//  member arrays), S2F32 (Borland dos.h clock), S6F24/S7F18/S7F20/
//  Process_S7F20/S101F2/S101F4/S101F6_StoreHostUploadFile/
//  S101F8_StoreHostUploadFile (spool/upload/recipe subsystem members),
//  S10F4/S10F6 (Terminal widget stand-ins), S125F2/SetECValue (VCL widget
//  dynamic_cast cluster) -- unchanged from INTEGRATE WAVE 4's own list minus
//  the 8 resolved this wave.
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"
#include "uHGemClass.h"
// AI(W906-SysModWire) 20260720: FIRST include of uHGemEquipment.h in this file
// -- see uHGemClass.h's own updated forward-declaration note for why: 8 newly
// un-gated methods below call real out-of-line THGem methods
// (CheckSFFormatOnlyHead/OffLine/OnLine/GetOnLineMode/GetTimeInfo), which
// require THGem's complete type (not just the bare forward declaration the
// header keeps). uHGemClass.h itself is UNCHANGED (still `class THGem;`) --
// this include stays confined to the .cpp, matching this project's "header
// stays minimal, .cpp pulls what its method BODIES need" convention.
#include "SECSGEM/uHGemEquipment.h"
// AI(W906-AlarmReportAck) 20260721: DEPENDENCY WIDENING -- S2F34_
// DefineReportAcknowledgeSub's golden body branches on
// `CUSTOMER_CODE==CC_ONSEMI_M` (golden uHGemClass.cpp:1241-1248). Neither
// symbol existed in this file before this wave; cmydef.h supplies both
// (CUSTOMER_CODE extern int + CC_ONSEMI_M macro, via its own MachineType.h
// include) -- same widening uHGemEquipment.cpp's own Bucket-B wave already
// made for the identical header, zero new CMake link edge needed (this file
// and uHGemEquipment.cpp already share the ht9045_secsgem target, which
// already links ht9045_core/ht9045_globals for exactly this).
#include "cmydef.h"
#include <cstdlib>   // atoi (CheckECValue's Type/PMax_Value/PMin_Value decode); strtoll/strtoull (S5F6_ListAlarmData's golden _atoi64 substitution, same precedent as SecsSvEcRegistration.cpp/uHGemEquipment.cpp)

// AI(W5-SECSGEM-Translate) 20260710: MyDBIProcess's real golden signature is
// `void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S1,
// AnsiString S2="");` (cMyDB.h:20; defined cMyDB.cpp:789, not yet
// translated) -- golden call sites like this file's own
// `MyDBIProcess("Exception", "~HTGem")` rely on the S2="" default.  Declared
// locally here matching that REAL 3-arg signature (not #include-ing cMyDB.h,
// to keep this file's dependency surface to vclcompat only).  NOTE for the
// integrate agent: aHotPlateSubstrate.h:576 forward-declares a DIFFERENT
// (2-arg) `MyDBIProcess(AnsiString S1, AnsiString S2)` from a prior wave --
// that is a distinct overload, not a redeclaration, so both can coexist, but
// it means the prior wave's declaration does not match golden's real
// signature (flagged as a pre-existing discrepancy, not introduced here).
// AI(W906-uHGemEquipment-BucketC) 20260717: added the missing `__fastcall` --
// this declaration previously LACKED it while database.cpp:64 (and the test
// stubs satisfying it) carry it. On i686 MinGW `__fastcall` is a real ABI
// (`__attribute__((fastcall))`, @n-decorated symbol): the mismatch made ld
// "resolve" the plain reference against the decorated definition with only a
// warning, producing a call through the WRONG calling convention --
// empirically reproduced as a segfault by the Bucket C fidelity review
// (dormant only because the affected call sites are exception-path-only).
// Root cause is vcl_compat.h's `#ifndef __fastcall` neutralization never
// firing (`__fastcall` is a compiler-builtin macro on MinGW) -- systemic
// follow-up tracked in MIGRATION_ROADMAP; this line fixes the one live edge.
extern void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2 = "");

//---------------------------------------------------------------------------
AnsiString SYS_ECChangeID             = "";                                     //pig 2014.04.23 KYEC SECS
AnsiString SYS_ECChangeIDOriginaValue = "";                                     //pig 2014.04.23 KYEC SECS
AnsiString SYS_ECChangeIDNewValue     = "";                                     //pig 2014.04.23 KYEC SECS
//------------------------------------------------------------------------------
bool IsCorrectDateFormat(int y,int m,int d)
{
    int   month[]={31,0,31,30,31,30,31,31,30,31,30,31};
    if(y<=2000 || y>9999)
        return   false;
    if(m<1 || m>12)
        return   false;
    if(((y%4)==0 && (y%100)!=0) || (y%400==0))
        month[1]=29;
    else
        month[1]=28;
    if(d>=1 && d<=month[m-1])
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
HTGem::HTGem()
{
    HGemPtr=NULL;
    HandlerPath="";
    ActiveWire=&WireCodec;                                                      //AI(W906-SysModWire) 20260720: design D default -- standalone dispatch target
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
};
//---------------------------------------------------------------------------
HTGem::HTGem(THGem *HGemTmp)
{
    HGemPtr=HGemTmp;
    HandlerPath="";
    ActiveWire=&WireCodec;                                                      //AI(W906-SysModWire) 20260720: design D default -- standalone dispatch target
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
}
//---------------------------------------------------------------------------
HTGem::HTGem(AnsiString Path)
{
    HandlerPath=Path;
    ActiveWire=&WireCodec;                                                      //AI(W906-SysModWire) 20260720: design D default -- standalone dispatch target
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
}
//---------------------------------------------------------------------------
HTGem::~HTGem()                                                                 //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
{
    try
    {
        if(SecsAlarmMessage!=NULL)
        {
            SecsAlarmMessage->Clear();                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete SecsAlarmMessage;
        }
        SecsAlarmMessage=NULL;                                                  //kevin 20180907 add

        if(FMessageList!=NULL)                                                  //Ifor 20251018 add:Secs Alarm List
        {
            FMessageList->Clear();
            delete FMessageList;
        }
        FMessageList = NULL;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HTGem");
    }
}
//---------------------------------------------------------------------------
void HTGem::UpdateDataPath(AnsiString Path)
{
    DataPath=Path;
}
//===========================================================================
//  Below: 57 out-of-line S,F handlers.  Every one of them needs `HGemPtr`
//  (THGem, out of scope -- see file-head note).  Real bodies are GATED with
//  a golden line-range citation; golden itself is the citable source, so the
//  body text is intentionally NOT duplicated here.  Each ACTIVE stub matches
//  its declared signature and returns a conservative/no-op default.
//===========================================================================

//---------------------------------------------------------------------------
// [S1,F1] Are You There Request.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:89-95).
// `HGemPtr->bS1F2_OnLineData` is a real THGem DATA member (unchanged);
// InitLocalHead/StringOut/SendLocalData -> ActiveWire-> (wire-codec ops).
// GOLDEN QUIRK preserved verbatim: InitLocalHead(1,1,0) sets W-Bit=0 even
// though S1,F1 is itself a request (W-Bit would normally be 1) -- not "fixed".
void HTGem::S1F1_AreYouThereRequest()
{
    HGemPtr->bS1F2_OnLineData=false;
    ActiveWire->InitLocalHead(1,1,0);
    ActiveWire->StringOut("[Send]    AreYouThereRequest");
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S1,F2] OnLineData : Data signifying that the equipment is alive.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:102-111).
// CheckSFFormatOnlyHead/GemMDLN/GemSOFTREV are real THGem method/data members
// (HGemPtr->, unchanged); InitLocalHead/DataItemOut/SendLocalData -> ActiveWire->.
void HTGem::S1F2_OnLineData()
{
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F1 Format error !!!")==false)
        return;
    ActiveWire->InitLocalHead(1, 2, 0);
    ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S1,F4] Selected Status Reply -- reports the value of each requested SVID.
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 119-264). Wire primitives (GetDataItemLenAndType/GetDataItemLenAndTypeAndDelete/
// InitLocalHead/DataItemOut/DataItemIn/SendLocalData) -> ActiveWire->;
// DataItemOutSV is THGem's own method (HGemPtr->, unchanged).
void HTGem::S1F4_SelectedStatusReply()
{
    int SVlen, len, i;
    unsigned char Type;
    AnsiString S;

    if(ActiveWire->GetDataItemLenAndType(SVlen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)                                   // new
        {
            ActiveWire->GetDataItemLenAndTypeAndDelete(SVlen, Type);
            if(SVlen==0)                                                        // report all svid
            {
                ActiveWire->InitLocalHead(1, 4, 0);
                len=HGemPtr->SvEcReg.SV_ID->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                {
                    HGemPtr->DataItemOutSV(atoi(HGemPtr->SvEcReg.SV_ID->GetString(i).c_str()));
                }
                ActiveWire->SendLocalData();
            }
            else
            {
                ActiveWire->InitLocalHead(1, 4, 0);
                ActiveWire->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len,Type)==1)
                        if(ActiveWire->DataItemIn(len, Type, S)==1)              // only for ascii,i1,i2,i4,i5 ,u1,u2,u4,u8
                            if(HGemPtr->DataItemOutSV(S)==false)
                                return;
                }
                ActiveWire->SendLocalData();
            }
        }
        else                                                                    // old type
        {
            ActiveWire->InitLocalHead(1, 4, 0);
            ActiveWire->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
            if(Type==HType.UINT_2_TYPE)
            {
                unsigned short *P;
                P=new unsigned short [SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                unsigned *P;
                P=new unsigned[SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned long long *P;                                          // golden unsigned __int64
                P=new unsigned long long[SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        // AI(W906-SvEcDataItem) 20260720: explicit (long long)
                        // cast -- vclcompat::AnsiString has ctors for
                        // int/unsigned int/long/long long/double but none for
                        // unsigned long long, so an implicit conversion here is
                        // AMBIGUOUS (could go via the long long OR the double
                        // ctor). SVIDs are always small positive integers in
                        // practice (well within long long range), so this cast
                        // is value-preserving for every real call; disambiguates
                        // only, no behavior change.
                        if(HGemPtr->DataItemOutSV((long long)P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                short *P;
                P=new short[SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                int *P;
                P=new int [SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                long long *P;                                                   // golden __int64
                P=new long long [SVlen];
                if(ActiveWire->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else
            {
                S9F7_IllegalData("S1,F3 data format error");
                return;
            }
            ActiveWire->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S1,F3 data format error");
    }
}
//---------------------------------------------------------------------------
// [S1,F12] Status Variable Namelist Reply.
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 266-324). Same wire-primitive -> ActiveWire-> / THGem-method -> HGemPtr->
// rule as S1F4 above.
void HTGem::S1F12_StatusVariableNamelistReply()
{
    int SVlen, i, len;
    unsigned char Type;
    AnsiString S;

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(SVlen,Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            ActiveWire->InitLocalHead(1,12,0);
            if(SVlen==0)
            {
                len=HGemPtr->SvEcReg.SV_ID->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutSVNameList(HGemPtr->SvEcReg.SV_ID->GetString(i));
            }
            else
            {
                ActiveWire->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(ActiveWire->DataItemIn(len, Type, S)==1)
                        {
                            if(HGemPtr->DataItemOutSVNameList(S)==false)
                                return;
                        }
                        else
                        {
                            S9F7_IllegalData("S1,F11 data format error");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S1,F11 data format error");
                        return;
                    }
                }
            }
            ActiveWire->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S1,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S1,F11 data format error");
    }
}
//---------------------------------------------------------------------------
// [S1,F13] Establish Communications Request.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:326-334).
// GemMDLN/GemSOFTREV real THGem data members (HGemPtr->, unchanged);
// InitLocalHead/DataItemOut/SendLocalData -> ActiveWire->.
void HTGem::S1F13_EstablishCommunicationsRequest()
{
    AnsiString S;
    ActiveWire->InitLocalHead(1, 13, 0);
    ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S1,F14] Connect Request Acknowledge.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:341-368).
// chkMoreMessageAbortProcess (THGemCheckBox*) + GemMDLN/GemSOFTREV +
// bReceiveEstablishCommunicationsRequest are real THGem members (HGemPtr->,
// unchanged); GetDataItemLenAndTypeAndDelete/InitLocalHead/DataItemOut/
// SendLocalData -> ActiveWire->.
void HTGem::S1F14_ConnectRequestAcknowledge()
{
    unsigned char Command=0,Type;
    AnsiString S, ret;
    int len;

    // < L[0]
    // >.

    if(HGemPtr->chkMoreMessageAbortProcess->Checked)
    {
        ret=ActiveWire->GetDataItemLenAndTypeAndDelete(len, Type);
        if(ret!=1 || len!=0 || Type!=HType.LIST_TYPE)
        {
            S9F7_IllegalData("S1,F13 data format error");
            return;
        }
    }
    ActiveWire->InitLocalHead(1,14,0);

    ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
    ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &Command);
    ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    ActiveWire->SendLocalData();
    HGemPtr->bReceiveEstablishCommunicationsRequest=true;
}
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:370-403).
// bWaitEstablishCommunicationsResponse/...Error are real THGem data members
// (HGemPtr->, unchanged); DataItemIn -> ActiveWire->.
void HTGem::Process_S1F14_ConnectRequestAcknowledge()
{
    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        unsigned char binarydata[1];
        if(ActiveWire->DataItemIn(1, HType.BINARY_TYPE, binarydata)==1)
        {
            if(ActiveWire->DataItemIn(0, HType.LIST_TYPE, NULL)!=1)
            {
                S9F7_IllegalData("S1,F14 data format error");
                return;
            }

            if(binarydata[0]==0x00)
            {
                HGemPtr->bWaitEstablishCommunicationsResponse=true;
                HGemPtr->bWaitEstablishCommunicationsResponseError=false;
                return;
            }
        }
        else
        {
            S9F7_IllegalData("S1,F14 data format error");
            return;
        }
    }
    else
    {
        S9F7_IllegalData("S1,F14 data format error");
        return;
    }
    HGemPtr->bWaitEstablishCommunicationsResponse=true;
    HGemPtr->bWaitEstablishCommunicationsResponseError=true;
}
//---------------------------------------------------------------------------
// [S1,F16] OFFLINE Acknowledge.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:408-417).
// CheckSFFormatOnlyHead/OffLine are real THGem out-of-line methods (HGemPtr->,
// unchanged); InitLocalHead/DataItemOut/SendLocalData -> ActiveWire->.
void HTGem::S1F16_OFFLINEAcknowledge()
{
    unsigned char Command=0;
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F15 Format error !!!")==false)
        return;

    ActiveWire->InitLocalHead(1, 16, 0);
    ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &Command);
    ActiveWire->SendLocalData();
    HGemPtr->OffLine();
}
//---------------------------------------------------------------------------
// [S1,F18] ONLINE Acknowledge.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:423-447).
// CheckSFFormatOnlyHead/bOnLine/GemCheckBoxAcceptHostOnlineRequest/OnLine/
// GetOnLineMode are real THGem members (HGemPtr->, unchanged); InitLocalHead/
// DataItemOut/SendLocalData -> ActiveWire->.
void HTGem::S1F18_ONLINEAcknowledge()
{
    unsigned char Command=0;
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F17 Format error !!!")==false)
        return;

    ActiveWire->InitLocalHead(1, 18, 0);
    if(HGemPtr->bOnLine)
    {
        Command=2;
    }
    else if(HGemPtr->GemCheckBoxAcceptHostOnlineRequest->Checked)
    {
        HGemPtr->bOnLine=true;
        Command=0;
    }
    else
    {
        Command=1;
    }
    ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &Command);
    ActiveWire->SendLocalData();
    if(Command==0)
        HGemPtr->OnLine(HGemPtr->GetOnLineMode());
}
//---------------------------------------------------------------------------
// [S1,F24] Collection Event Namelist.                          //2014/01/01 lee
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 450-587) -- "no defect" method (§2 category ①): strGrdCEID/stdGridReportID
// (THGem's own StringGrid members, HGemPtr->, unchanged) + wire primitives
// (-> ActiveWire->) is the WHOLE dependency chain -- zero new THGem surface.
void HTGem::S1F24_CollectionEventNamelist()
{
    int SVlen, len;
    unsigned char Type;

    int   pos;
    unsigned int   uint4SV;
    bool ret;
    AnsiString S;
    TStringList *CEIDList,*SVIDList,*BackCeid;

    CEIDList=new TStringList;
    SVIDList=new TStringList;
    BackCeid=new TStringList;

    CEIDList->Clear();
    BackCeid->Clear();

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(SVlen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            ActiveWire->InitLocalHead(1, 24, 0);
            if(SVlen==0)                                                        // report all CEID name and SVID
            {
                for(int i=1; i<HGemPtr->strGrdCEID->RowCount; i++)
                {
                    if(HGemPtr->strGrdCEID->Cells[0][i]!="" && HGemPtr->strGrdCEID->Cells[0][i]!=NULL)
                        CEIDList->Add(i);
                }
                len=CEIDList->Count;

                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int j=0; j<len; j++)
                {
                    ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
                    pos=atoi(CEIDList->GetString(j).c_str());
                    uint4SV=(unsigned)atoi(HGemPtr->strGrdCEID->Cells[0][pos].c_str());
                    ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdCEID->Cells[2][pos]);
                    SVIDList->Clear();
                    for(int k=3; k<HGemPtr->strGrdCEID->ColCount; k++)
                    {
                        S=HGemPtr->strGrdCEID->Cells[k][pos];
                        if(S!="")
                        {
                            for(int y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
                            {
                                if(S==HGemPtr->stdGridReportID->Cells[0][y])
                                {
                                    for(int x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                                    {
                                        if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                                            SVIDList->Add(HGemPtr->stdGridReportID->Cells[x][y]);
                                    }
                                }
                            }
                        }
                    }
                    ActiveWire->DataItemOut(SVIDList->Count, HType.LIST_TYPE, NULL);
                    for(int k=0; k<SVIDList->Count; k++)
                    {
                        uint4SV=(unsigned)atoi(SVIDList->GetString(k).c_str());
                        ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                    }
                }
            }
            else
            {
                for(int j=0; j<SVlen; j++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(ActiveWire->DataItemIn(len, Type, S)==1)
                        {
                            ret=false;
                            for(int i=1; i<HGemPtr->strGrdCEID->RowCount; i++)
                            {
                                if(HGemPtr->strGrdCEID->Cells[0][i]==S)
                                {
                                    CEIDList->Add(i);
                                    ret=true;
                                    break;
                                }
                            }
                            BackCeid->Add(S);
                            if(ret==false)
                                CEIDList->Add(0);
                        }
                    }
                }
                len=CEIDList->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int j=0; j<len; j++)
                {
                    ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
                    pos=atoi(CEIDList->GetString(j).c_str());
                    if(pos==0)                                                  // no such CEID
                    {
                        uint4SV=(unsigned)atoi(BackCeid->GetString(j).c_str());
                        ActiveWire->DataItemOut(1, HType.UINT_4_TYPE,&uint4SV);
                        ActiveWire->DataItemOut(HType.ASCII_TYPE, AnsiString(""));
                        ActiveWire->DataItemOut(0, HType.LIST_TYPE, NULL);
                    }
                    else
                    {
                        uint4SV=(unsigned)atoi(HGemPtr->strGrdCEID->Cells[0][pos].c_str());
                        ActiveWire->DataItemOut(1, HType.UINT_4_TYPE,&uint4SV);
                        ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdCEID->Cells[2][pos]);
                        SVIDList->Clear();
                        for(int k=3; k<HGemPtr->strGrdCEID->ColCount; k++)
                        {
                            S=HGemPtr->strGrdCEID->Cells[k][pos];
                            if(S!="")
                            {
                                for(int y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
                                {
                                    if(S==HGemPtr->stdGridReportID->Cells[0][y])
                                    {
                                        for(int x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                                        {
                                            if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                                                SVIDList->Add(HGemPtr->stdGridReportID->Cells[x][y]);
                                        }
                                    }
                                }
                            }
                        }
                        ActiveWire->DataItemOut(SVIDList->Count, HType.LIST_TYPE, NULL);
                        for(int k=0; k<SVIDList->Count; k++)
                        {
                            uint4SV=(unsigned)atoi(SVIDList->GetString(k).c_str());
                            ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                        }
                    }
                }
            }
            ActiveWire->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S1,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S1,F11 data format error");
    }
    CEIDList->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    SVIDList->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    BackCeid->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete CEIDList;
    delete SVIDList;
    delete BackCeid;
}
//---------------------------------------------------------------------------
// [S2,F14] Equipment Constant Data.
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 609-719). EC_ID -> HGemPtr->SvEcReg.EC_ID (SV/EC bookkeeping embed);
// DataItemOutEC is THGem's own method (HGemPtr->, unchanged); wire
// primitives -> ActiveWire->.
void HTGem::S2F14_EquipmentConstanData()
{
    int EClen, len;
    unsigned char Type;
    AnsiString S;

    if(ActiveWire->GetDataItemLenAndType(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || EClen==0)
        {
            ActiveWire->GetDataItemLenAndTypeAndDelete(EClen, Type);
            ActiveWire->InitLocalHead(2, 14, 0);
            if(EClen==0)
            {
                len=HGemPtr->SvEcReg.EC_ID->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int i=0; i<len; i++)
                    HGemPtr->DataItemOutEC(HGemPtr->SvEcReg.EC_ID->GetString(i));
            }
            else
            {
                ActiveWire->DataItemOut(EClen, HType.LIST_TYPE, NULL);
                for(int i=0; i<EClen; i++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len, Type)==1)
                    {
                        if(ActiveWire->DataItemIn(len, Type, S)==1)
                            HGemPtr->DataItemOutEC(S);
                    }
                    else
                    {
                        S9F7_IllegalData("S2,F13 data format error");
                        return;
                    }
                }
            }
            ActiveWire->SendLocalData();
        }
        else
        {
            ActiveWire->InitLocalHead(2, 14, 0);
            ActiveWire->DataItemOut(EClen, HType.LIST_TYPE, NULL);
            if(Type==HType.UINT_2_TYPE)
            {
                unsigned short *P;
                P=new unsigned short [EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                unsigned *P;
                P=new unsigned [EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                short *P;
                P=new short [EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                int *P;
                P=new int[EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20130730 ADD
            {
                long long *P;                                                   // golden __int64
                P=new long long[EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.FT_4_TYPE)                                      //Steven 20130730 ADD
            {
                float *P;
                P=new float[EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.FT_8_TYPE)                                      //Steven 20130730 ADD
            {
                double *P;
                P=new double[EClen];
                if(ActiveWire->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else
            {
                S9F7_IllegalData("S21,F13 data format error");
                return;
            }
            ActiveWire->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S2,F13 data format error");
    }
}
//---------------------------------------------------------------------------
// [S2,F16] New Equipment Constant Send Acknowledge.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: gate comment narrowed -- THGem::MoveCheckCallBack
// (this wave's own new member) is NO LONGER a blocker for this method (golden
// :779-780 is now satisfiable), but golden :734/746 still need
// `THGem::SReceiveDataBackup` (a member no wave has added yet) and the
// csystem predicates `HasICUnderMachine()`/`HasAnyICInMachine()` (golden
// :738/746 -- free functions that would require a NEW ht9045_secsgem ->
// ht9045_sm link edge, deliberately out of this wave's scope). Still gated.
void HTGem::S2F16_NewEquipmentConstantSendAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem member SReceiveDataBackup + csystem predicates HasICUnderMachine()/HasAnyICInMachine() (new ht9045_secsgem->ht9045_sm link edge)) -- golden SECSGEM/uHGemClass.cpp:731-779
#endif
}
//---------------------------------------------------------------------------
// [S2,F18] Date and Time Data.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:786-793).
// CheckSFFormatOnlyHead/GetTimeInfo/GemClock are real THGem method/data
// members (HGemPtr->, unchanged); InitLocalHead/DataItemOut/SendLocalData ->
// ActiveWire->.
void HTGem::S2F18_DateandTimeData()
{
    if(HGemPtr->CheckSFFormatOnlyHead("S2,F17 Format error !!!")==false)
        return;
    HGemPtr->GetTimeInfo();
    ActiveWire->InitLocalHead(2, 18, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemClock);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S2,F24] Trace Initialize -- ack-code sub (called by the void wrapper below).
//---------------------------------------------------------------------------
int HTGem::S2F24_TraceInitializeAcknowledgeSub()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members bTraceData[10]/iTRID[10]/DSPER[10]/iTOTSMP[10]/iREPGSZ[10]/TraceData[10]/TraceDataResponseTask[10] + THGem::IsValidSVID) -- golden SECSGEM/uHGemClass.cpp:810-985
#endif
    return 1;                                                                  // conservative default (deny); real TIACK decode gated above
}
//---------------------------------------------------------------------------
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:992-1000):
// a thin wrapper -- calls its own still-GATED Sub() sibling (compiles/links
// fine either way; a sibling method call needs no THGem) plus LocalAcknowledge
// (now WireCodec.LocalAcknowledge, real). Needs nothing else.
void HTGem::S2F24_TraceInitializeAcknowledge()
{
    int ret;
    ret=S2F24_TraceInitializeAcknowledgeSub();
    if(ret==-1)
        S9F7_IllegalData("S2,F23 Format error !!!");
    else
        ActiveWire->LocalAcknowledge(2,24,(unsigned char)ret);                     // int->uchar narrowing, matches golden's own implicit conversion
}
//---------------------------------------------------------------------------
// [S2,F26] Diagnostic Loopback Data.
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:1006-1032; fixed 20260713 by an independent fidelity review, was mis-cited 1006-1024):
// entirely GetDataItemLenAndType/DataItemIn/InitLocalHead/DataItemOut/
// SendLocalData -- all now WireCodec methods. `Temp` is allocated as
// `unsigned char[]` here (golden: `unsigned char *Temp; ... Temp=new
// char[len+100];` -- a char*->unsigned char* implicit pointer-signedness
// assignment BCB6 accepted and standard C++ rejects without a cast;
// allocating with the variable's own element type instead is the same
// zero-behavior-change accommodation already established elsewhere in this
// file/SecsWireCodec.cpp for analogous char/unsigned char pointer mismatches).
//---------------------------------------------------------------------------
void HTGem::S2F26_DiagnosticLoopbackData()
{
    int len, ret;
    unsigned char Type;
    unsigned char *Temp;
    ret=ActiveWire->GetDataItemLenAndType(len,Type);
    if(Type==HType.BINARY_TYPE && ret==1)
    {
        Temp=new unsigned char [len+100];
        if(ActiveWire->DataItemIn(len, Type, Temp)==1)
        {
            ActiveWire->InitLocalHead(2, 26, 0);
            ActiveWire->DataItemOut(len, HType.BINARY_TYPE, Temp);
            ActiveWire->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S2,F25 Format error !!!");
        }
        delete[] Temp;
        Temp=NULL;                                                              //kevin 20180517
    }
    else
    {
        S9F7_IllegalData("S2,F25 Format error !!!");
    }
}
//---------------------------------------------------------------------------
// [S2,F30] Equipment Constant Namelist Reply.
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1037-1080). Same EC_ID -> HGemPtr->SvEcReg.EC_ID / DataItemOutECNameList ->
// HGemPtr-> / wire -> ActiveWire-> rule as S2F14 above.
void HTGem::S2F30_EquipmentConstantNamelistReply()
{
    int EClen, i, len;                                                          //,ret;
    unsigned char Type;
    AnsiString ECID;

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            ActiveWire->InitLocalHead(2, 30, 0);
            if(EClen==0)
            {
                len=HGemPtr->SvEcReg.EC_ID->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutECNameList(HGemPtr->SvEcReg.EC_ID->GetString(i));
            }
            else
            {
                ActiveWire->DataItemOut(EClen, HType.LIST_TYPE, NULL);
                for(i=0; i<EClen; i++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len, Type)==1)
                    {
                        if(ActiveWire->DataItemIn(len, Type, ECID)==1)
                        {
                            HGemPtr->DataItemOutECNameList(ECID);
                        }
                        else
                        {
                            S9F7_IllegalData("S2,F29 Format error !!!");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S2,F29 Format error !!!");
                        return;
                    }
                }
            }
            ActiveWire->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S2,F29 Format error !!!");
        }
    }
    else
    {
        S9F7_IllegalData("S2,F29 Format error !!!");
    }
}
//---------------------------------------------------------------------------
// [S2,F32] Date And Time Acknowledge (uses IsCorrectDateFormat, kept ACTIVE above).
//---------------------------------------------------------------------------
void HTGem::S2F32_DateAndTimeAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs Borland dos.h settime()/setdate()/struct time/struct date (not in vclcompat)) -- golden SECSGEM/uHGemClass.cpp:1095-1216
#endif
}
//---------------------------------------------------------------------------
// [S2,F34] Define Report -- ack-code sub (DRACK).
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1218-1291). DataItemIn/GetDataItemLenAndType/GetDataItemLenAndTypeAndDelete
// -> ActiveWire-> (wire-codec primitives); slTempReportID/lTempReportIDContent/
// DeleteAllHostDefineReportID stay HGemPtr-> (real THGem state/method, no
// wire-codec engine home). DEPENDENCY WIDENING: the CUSTOMER_CODE==
// CC_ONSEMI_M branch below needs cmydef.h, now included (see this file's own
// file-head note).
int HTGem::S2F34_DefineReportAcknowledgeSub()
{
    int len, svlen, ret, temp;
    unsigned char Type;
    AnsiString DATAID, ReportID, sSVID;
    TStringList *P;

    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)                         // 若上一次的資料尚未清除則要清除
    {
        P=(TStringList *) HGemPtr->lTempReportIDContent->Items[i];
        P->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete P;
    }
    HGemPtr->slTempReportID->Clear();
    HGemPtr->lTempReportIDContent->Clear();

    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
        return -1;

    ActiveWire->GetDataItemLenAndType(len, Type);
    if(ActiveWire->DataItemIn(len, Type, DATAID)!=1)
        return -1;

    if(CUSTOMER_CODE==CC_ONSEMI_M)
    {
    }
    else
    {
        if(DATAID=="")                                                          //JerryYang 20250120 : modify
            return -1;
    }

    ret=ActiveWire->GetDataItemLenAndType(len, Type);                              // 取得多少 report id 要被定義
    if(ret!=1 || Type!=HType.LIST_TYPE)
        return -1;
    ActiveWire->DataItemIn(len, HType.LIST_TYPE, NULL);
    if(len==0)
    {
        HGemPtr->DeleteAllHostDefineReportID();
        return 1;
    }

    for(int i=0; i<len; i++)
    {
        if(ActiveWire->DataItemIn(2, HType.LIST_TYPE,NULL)!=1)
            return -1;
        if(ActiveWire->GetDataItemLenAndType(temp, Type)!=1)
            return -1;
        if(ActiveWire->DataItemIn(temp, Type, ReportID)!=1)
            return -1;
        ret=ActiveWire->GetDataItemLenAndTypeAndDelete(svlen, Type);
        if(ret!=1 || Type!=HType.LIST_TYPE)
            return -1;

        P=new TStringList;
        P->Clear();
        for(int j=0; j<svlen; j++)
        {
            ActiveWire->GetDataItemLenAndType(temp, Type);
            if(ActiveWire->DataItemIn(temp, Type, sSVID)==1)
            {
                P->Add(sSVID);
            }
            else
            {
                P->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete P;
                return -1;
            }
        }
        HGemPtr->slTempReportID->Add(ReportID);
        HGemPtr->lTempReportIDContent->Add(P);
    }
    return 1;
}
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1294-1361). No wire-codec primitives called directly in this method's own
// body -- every HGemPtr-> call stays HGemPtr-> (real THGem state/methods,
// all already real per this wave's own header/A4 additions).
void HTGem::S2F34_ProcessHostSendReportID()
{
    AnsiString ReportID, SVID;
    TStringList *strPtr;

    // 檢查是否已經定義了
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        ReportID=HGemPtr->slTempReportID->Strings[i];
        strPtr  =(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        if(HGemPtr->CheckReportAlreadyDefine(ReportID) && strPtr->Count!=0)
        {
            HGemPtr->DefineReportAcknowledgeAlreadyDefined();
            return;
        }
    }
    // 檢查 SVID 是否有不存在的
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        strPtr=(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        for(int j=0; j<strPtr->Count; j++)
        {
            SVID=strPtr->Strings[j];
            if(HGemPtr->IsValidSVID(SVID)==false)
            {
                HGemPtr->DefineReportAcknowledgeInvalidSVID();
                return;
            }
        }
    }
    unsigned SVIDBuffer[1024];
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        ReportID=HGemPtr->slTempReportID->Strings[i];
        strPtr=(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        if(strPtr->Count==0)
        {
            HGemPtr->DeleteReportID(atoi(ReportID.c_str()), 0);                 //  把 report id 表內地 report id 全部 delete  2013/11/20
            HGemPtr->DeleteReportIDOfCeid(atoi(ReportID.c_str()));              // 把 ceid   id 表內地 report id 全部 delete
        }
        else
        {
            if(strPtr->Count>1024)
            {
                HGemPtr->DefineReportAcknowledgeInsufficientSpace();
                return;
            }

            for(int j=0; j<strPtr->Count; j++)
            {
                SVID=strPtr->Strings[j];
                SVIDBuffer[j]=atoi(SVID.c_str());
            }

            if(HGemPtr->SetReportIDContent(atoi(ReportID.c_str()), strPtr->Count, SVIDBuffer, 0)==false)
            {
                HGemPtr->DefineReportAcknowledgeInsufficientSpace();
                return;
            }
        }
        strPtr->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete strPtr;
    }
    HGemPtr->slTempReportID->Clear();
    HGemPtr->lTempReportIDContent->Clear();
    HGemPtr->DefineReportAcknowledgeAccept();
    HGemPtr->SaveEventReportData();
}
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1363-1393). No wire-codec primitives called directly (calls its own
// Sub() sibling + HGemPtr->DefineReportAcknowledgeFormatError, both real).
void HTGem::S2F34_DefineReportAcknowledge()
{
    int ret;
    ret=S2F34_DefineReportAcknowledgeSub();
    if(ret==-1)
        HGemPtr->DefineReportAcknowledgeFormatError();
    else
        S2F34_ProcessHostSendReportID();
}
//---------------------------------------------------------------------------
// [S2,F36] Link Event Report -- ack-code sub (LRACK).
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1395-1457). Same DataItemIn/GetDataItemLenAndType/
// GetDataItemLenAndTypeAndDelete -> ActiveWire-> rule as
// S2F34_DefineReportAcknowledgeSub above; slTempCeID/lTempCeIDContent/
// DeleteAllHostDefineCeid stay HGemPtr->.
int HTGem::S2F36_LinkEventReportAcknowledgeSub()
{
    int len, svlen, ret, temp;
    unsigned char Type;
    AnsiString DATAID,Ceid,sReportID;
    TStringList *P;

    for(int i=0; i<HGemPtr->slTempCeID->Count; i++)                             // 若上一次遞資料尚未清除記憶體則要清除
    {
        P=(TStringList *) HGemPtr->lTempCeIDContent->Items[i];
        P->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete P;
    }
    HGemPtr->slTempCeID->Clear();
    HGemPtr->lTempCeIDContent->Clear();

    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
        return -1;

    ActiveWire->GetDataItemLenAndType(len, Type);
    if(ActiveWire->DataItemIn(len, Type, DATAID)!=1)
        return -1;
    ret=ActiveWire->GetDataItemLenAndType(len, Type);                              // 取得多少 report id 要被定義
    if(ret!=1 || Type!=HType.LIST_TYPE)
        return -1;

    ActiveWire->DataItemIn(len, HType.LIST_TYPE, NULL);
    if(len==0)
    {
        HGemPtr->DeleteAllHostDefineCeid();
        return 1;
    }

    for(int i=0; i<len; i++)
    {
        if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
            return -1;

        ActiveWire->GetDataItemLenAndType(temp, Type);
        if(ActiveWire->DataItemIn(temp, Type, Ceid)!=1)
            return -1;

        ret=ActiveWire->GetDataItemLenAndTypeAndDelete(svlen, Type);
        if(ret!=1 || Type!=HType.LIST_TYPE)
            return -1;

        P=new TStringList;
        P->Clear();
        for(int j=0; j<svlen; j++)
        {
            ActiveWire->GetDataItemLenAndType(temp, Type);
            if(ActiveWire->DataItemIn(temp, Type, sReportID)!=1)
            {
                P->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete P;
                return -1;
            }
            P->Add(sReportID);
        }
        HGemPtr->slTempCeID->Add(Ceid);
        HGemPtr->lTempCeIDContent->Add(P);
    }
    return 1;
}
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1460-1477). No wire-codec primitives called directly (calls its own
// Sub() sibling + HGemPtr->LinkReportAcknowledgeFormatError/
// ProcessHostSendReportLinkID, both real).
void HTGem::S2F36_LinkEventReportAcknowledge()
{
    int ret;
    ret=S2F36_LinkEventReportAcknowledgeSub();
    if(ret==-1)
        HGemPtr->LinkReportAcknowledgeFormatError();
    else
        HGemPtr->ProcessHostSendReportLinkID();
}
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1480-1521). DataItemIn/GetDataItemLenAndTypeAndDelete/GetDataItemLenAndType
// -> ActiveWire->; CheckCEIDExist/EnableDisableEventReportAcknowledge
// CeidNotExist/...FormatError/EnableDisableEventReport stay HGemPtr-> (real
// THGem methods, all already real per this wave's own header/A4 additions).
void HTGem::S2F38_EnableDisableEventReportAcknowledge()
{
    int len, slen;
    unsigned char Type;
    bool CEED;
    unsigned CEID[1024];
    AnsiString sID;

    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
    {
        if(ActiveWire->DataItemIn(1, HType.BOOLEAN_TYPE, &CEED))
        {
            if(ActiveWire->GetDataItemLenAndTypeAndDelete(slen, Type)==1)
            {
                if(Type==HType.LIST_TYPE)
                {
                    for(int i=0; i<slen; i++)
                    {
                        ActiveWire->GetDataItemLenAndType(len, Type);
                        if(ActiveWire->DataItemIn(len, Type, sID)==1)
                        {
                            CEID[i]=atoi(sID.c_str());
                            if(HGemPtr->CheckCEIDExist(sID)==false)
                            {
                                HGemPtr->EnableDisableEventReportAcknowledgeCeidNotExist();
                                return;
                            }
                        }
                        else
                        {
                            HGemPtr->EnableDisableEventReportAcknowledgeFormatError();
                            return;
                        }
                    }
                    HGemPtr->EnableDisableEventReport(CEED, slen, CEID);
                    return;
                }
            }
        }
    }
    HGemPtr->EnableDisableEventReportAcknowledgeFormatError();
}
//---------------------------------------------------------------------------
// [S2,F42] Host Command Acknowledge (HCACK).
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:1523-1574):
// entirely DataItemIn/GetDataItemLenAndType/InitLocalHead/DataItemOut/
// SendLocalData -- all now WireCodec methods. PRESERVED GOLDEN QUIRK (not
// fixed): the `else` (format-error) path emits DataItemOut calls WITHOUT a
// preceding InitLocalHead -- golden's own asymmetry vs. the `if` branch,
// confirmed by direct read, not a translation gap.
//---------------------------------------------------------------------------
int HTGem::S2F42_Host_Command_Acknowledge()
{
    unsigned char HCACK=1;
    char CommandStr[1024];
    AnsiString PPID;
    int ret,len;
    unsigned char Type;
    AnsiString S, S1;

    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)==1)                       // 需要補充多重 Command
    {
        ActiveWire->GetDataItemLenAndType(len, Type);
        if(Type==HType.ASCII_TYPE)
        {
            ret=ActiveWire->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
            if(ret==-1)
                S="";
        }
        else
        {
            S="";
        }
        S=CommandStr;
        S=S.UpperCase();
        HCACK=1;
        ActiveWire->InitLocalHead(2, 42, 0);

        if(HCACK==0)
        {
            ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
            ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            ActiveWire->DataItemOut(0, HType.LIST_TYPE, NULL);
        }
        else
        {
            ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
            ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            ActiveWire->DataItemOut(0, HType.LIST_TYPE, NULL);
        }

        ActiveWire->SendLocalData();
        return 1;
    }
    else
    {
        HCACK=3;
        ActiveWire->DataItemOut(2, HType.LIST_TYPE, NULL);
        ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
        ActiveWire->DataItemOut(0, HType.LIST_TYPE, NULL);
    }
    return HCACK;
}
//---------------------------------------------------------------------------
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:1576-1584; fixed 20260713 by an independent fidelity review, was mis-cited 1576-1591):
// entirely InitLocalHead/DataItemOut/SendLocalData -- all now WireCodec.
//---------------------------------------------------------------------------
void HTGem::S2F44_ResetSpoolingAcknowledge()
{
    unsigned char C=0;
    ActiveWire->InitLocalHead(2, 44,0);
    ActiveWire->DataItemOut( 2,HType.LIST_TYPE, NULL);
    ActiveWire->DataItemOut( 1,HType.BINARY_TYPE,&C);
    ActiveWire->DataItemOut( 0,HType.LIST_TYPE, NULL);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1593-1624). DataItemIn/GetDataItemLenAndType/LocalAcknowledge ->
// ActiveWire-> (wire-codec primitives); EnableDisableAlarmAll/
// EnableDisableAlarm stay HGemPtr-> (real THGem methods, added this wave).
void HTGem::S5F4_EnableDisableAlarmAcknowledge()
{
    unsigned char ALED, Type;
    int len;
    AnsiString ID;

    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        if(ActiveWire->DataItemIn(1, HType.BINARY_TYPE, &ALED))
        {
            ActiveWire->GetDataItemLenAndType(len, Type);
            if(len==0)
            {
                HGemPtr->EnableDisableAlarmAll(ALED);
                ActiveWire->LocalAcknowledge(5, 4, 0);
            }
            else
            {
                ActiveWire->DataItemIn(len, Type, ID);
                if(HGemPtr->EnableDisableAlarm(ID, ALED))
                    ActiveWire->LocalAcknowledge(5, 4, 0);
                else
                    ActiveWire->LocalAcknowledge(5, 4, 1);
            }
            return;
        }
    }
    S9F7_IllegalData("S5,F3 Data Format error !!!");
}
//---------------------------------------------------------------------------
// V 1.0
// S5,F6 This message contains the alarm data known to the equipment. There are “m” alarms in the list.
//---------------------------------------------------------------------------
// AI(W906-AlarmReportAck) 20260721: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1626-1887). DataItemIn/GetDataItemLenAndType/InitLocalHead/DataItemOut/
// SendLocalData -> ActiveWire-> (wire-codec primitives); strGrdAlarm/
// GetAlarmIndex stay HGemPtr-> (real THGem StringGrid/method, no wire-codec
// engine home). golden `_atoi64` -> `strtoll`/`strtoull` (MinGW <cstdlib>
// substitution, see this file's own file-head note); the `(unsigned)
// strtoull(...)` 32-bit-truncating cast before storing into the 64-bit
// `uint8SV` is an EXISTING golden quirk (golden :1720), not introduced here.
void HTGem::S5F6_ListAlarmData()
{
    int SVlen,i, j,ret;
    unsigned char Type;

    __int64  int8SV;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4SV;
    short int2SV;
    unsigned __int64  uint8SV;                                                  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4SV;
    unsigned short uint2SV;
    AnsiString str;
    AnsiString S;

    if(ActiveWire->GetDataItemLenAndType(SVlen,Type)==1)
    {
        if(SVlen!=0)
        {
            if(Type==HType.UINT_8_TYPE || Type==HType.UINT_4_TYPE || Type==HType.UINT_2_TYPE ||
               Type==HType.INT_8_TYPE  || Type==HType.INT_4_TYPE  || Type==HType.INT_2_TYPE)                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64 *uint8Ptr;
                unsigned *uint4Ptr;
                unsigned short *uint2Ptr;
                __int64     *int8Ptr;
                int      *int4Ptr;
                short    *int2Ptr;
                unsigned char Mode=0x80;

                ret=ActiveWire->GetDataItemLenAndType(SVlen,Type);
                if(ret!=1)
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    return;
                }

                if(Type==HType.UINT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    uint8Ptr=new unsigned __int64 [SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.UINT_8_TYPE,uint8Ptr);
                }
                else if(Type==HType.UINT_4_TYPE)
                {
                    uint4Ptr=new unsigned [SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.UINT_4_TYPE,uint4Ptr);
                }
                else if(Type==HType.UINT_2_TYPE)
                {
                    uint2Ptr=new unsigned short [SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.UINT_2_TYPE,uint2Ptr);
                }
                else if(Type==HType.INT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    int8Ptr=new __int64[SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.INT_8_TYPE,int8Ptr);
                }
                else if(Type==HType.INT_4_TYPE)
                {
                    int4Ptr=new int[SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.INT_4_TYPE,int4Ptr);
                }
                else if(Type==HType.INT_2_TYPE)
                {
                    int2Ptr=new short[SVlen];
                    ret=ActiveWire->DataItemIn(SVlen, HType.INT_2_TYPE,int2Ptr);
                }

                if(ret==1)
                {
                    ActiveWire->InitLocalHead(5,6,0);
                    ActiveWire->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                    for(i=0; i<SVlen; i++)
                    {
                        if(Type==HType.UINT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            // AI(W906-AlarmReportAck) 20260721: cast to (long
                            // long) -- vclcompat AnsiString has no operator=
                            // matching `unsigned long long` exactly (ambiguous
                            // across int/unsigned int/long long/double), same
                            // established precedent as SecsWireCodec.cpp's own
                            // identical accommodation.
                            S=(long long)uint8Ptr[i];
                        else if(Type==HType.UINT_4_TYPE)
                            S=uint4Ptr[i];
                        else if(Type==HType.UINT_2_TYPE)
                            S=uint2Ptr[i];
                        else if(Type==HType.INT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=int8Ptr[i];
                        else if(Type==HType.INT_4_TYPE)
                            S=int4Ptr[i];
                        else if(Type==HType.INT_2_TYPE)
                            S=int2Ptr[i];

                        j=HGemPtr->GetAlarmIndex(S);
                        if(j>=0)
                        {
                            ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
                            Mode=atoi(HGemPtr->strGrdAlarm->Cells[2][j].c_str())+0x80;
                            ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &Mode);  // mode
                            if(Type==HType.UINT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                uint8SV=(unsigned)strtoull(HGemPtr->strGrdAlarm->Cells[1][j].c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution); (unsigned) truncation is an EXISTING golden quirk, see this method's own file-head-adjacent comment
                                ActiveWire->DataItemOut(1, HType.UINT_8_TYPE, &uint8SV);
                            }
                            else if(Type==HType.UINT_4_TYPE)
                            {
                                uint4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                            }
                            else if(Type==HType.UINT_2_TYPE)
                            {
                                uint2SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                ActiveWire->DataItemOut(1, HType.UINT_2_TYPE, &uint2SV);
                            }
                            else if(Type==HType.INT_8_TYPE)                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                int8SV=strtoll(HGemPtr->strGrdAlarm->Cells[1][j].c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution)
                                ActiveWire->DataItemOut(1, HType.INT_8_TYPE, &int8SV);
                            }
                            else if(Type==HType.INT_4_TYPE)
                            {
                                int4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                ActiveWire->DataItemOut(1, HType.INT_4_TYPE, &int4SV);
                            }
                            else if(Type==HType.INT_2_TYPE)
                            {
                                int2SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                ActiveWire->DataItemOut(1, HType.INT_2_TYPE, &int2SV);
                            }

                            ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][j]);                  // message
                        }
                        else
                        {
                            // AI(W906-AlarmReportAck) 20260721: golden quirk,
                            // preserved verbatim -- this DataItemOut call
                            // passes length 0, so `Mode`'s value (0x80 from
                            // its declaration above, never reassigned in THIS
                            // branch) is never actually transmitted. This is a
                            // genuine zero-length SECS-II item in golden
                            // itself (golden uHGemClass.cpp:1754), not a
                            // translation error -- do NOT "fix" it to a
                            // non-zero length.
                            ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
                            ActiveWire->DataItemOut(0, HType.BINARY_TYPE,&Mode);   // mode -- zero length, see comment above
                            if(Type==HType.UINT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                uint8SV=uint8Ptr[i];
                                ActiveWire->DataItemOut(1, HType.UINT_8_TYPE, &uint8SV);
                            }
                            else if(Type==HType.UINT_4_TYPE)
                            {
                                uint4SV=uint4Ptr[i];
                                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                            }
                            else if(Type==HType.UINT_2_TYPE)
                            {
                                uint2SV=uint2Ptr[i];
                                ActiveWire->DataItemOut(1, HType.UINT_2_TYPE, &uint2SV);
                            }
                            else if(Type==HType.INT_8_TYPE)                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                int8SV=int8Ptr[i];
                                ActiveWire->DataItemOut(1, HType.INT_8_TYPE, &int8SV);
                            }
                            else if(Type==HType.INT_4_TYPE)
                            {
                                int4SV=int4Ptr[i];
                                ActiveWire->DataItemOut(1, HType.INT_4_TYPE, &int4SV);
                            }
                            else if(Type==HType.INT_2_TYPE)
                            {
                                int2SV=int2Ptr[i];
                                ActiveWire->DataItemOut(1, HType.INT_2_TYPE, &int2SV);
                            }

                            str="";
                            ActiveWire->DataItemOut(HType.ASCII_TYPE, str);
                        }
                    }
                    ActiveWire->SendLocalData();
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }
                    return;
                }
                else
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }
                    return;
                }
            }
            else
            {
                S9F7_IllegalData("S5,F5 Data Format error !!!");
            }
        }
        else
        {
            ActiveWire->InitLocalHead(5, 6, 0);
            unsigned char Mode=0x80;
            ActiveWire->DataItemOut(HGemPtr->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
            for(i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
            {
                ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
                Mode=atoi(HGemPtr->strGrdAlarm->Cells[2][i].c_str())+0x80;
                ActiveWire->DataItemOut(1, HType.BINARY_TYPE,&Mode);               // mode
                uint4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][i].c_str());
                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);           // ID
                ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
            }

            ActiveWire->SendLocalData();
            return;
        }
    }
    else
    {
        S9F7_IllegalData("S5,F5 Data Format error !!!");
    }
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1892-1923) -- "no defect" method (§2 category ①): strGrdAlarm (THGem's
// own StringGrid, HGemPtr->, unchanged) + wire -> ActiveWire-> is the WHOLE
// dependency chain -- zero new THGem surface.
void HTGem::S5F8_ListEnableAlarmAcknowledge()
{
    int SVlen, len;
    unsigned char Type;
    unsigned int   uint4SV;
    unsigned char ALT=0x80;
    if( ActiveWire->GetDataItemLenAndType(SVlen, Type)!=1)                       //pig 2014.08.01 SECS
    {
        S9F7_IllegalData("S5,F7 Data Format error !!!");
        return;
    }
    ActiveWire->InitLocalHead(5,8,0);
    len=0;
    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
        if(HGemPtr->strGrdAlarm->Cells[7][i]=="1")                              //wei 20180227 (Steven) S5F7 判斷位置錯誤 3-->7
            len++;

    ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);

    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
    {
        if(HGemPtr->strGrdAlarm->Cells[7][i]=="1")                              //wei 20180227 (Steven) S5F7 判斷位置錯誤 3-->7
        {
            ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);
            ActiveWire->DataItemOut(1, HType.BINARY_TYPE, &ALT);
            uint4SV=(unsigned)atoi(HGemPtr->strGrdAlarm->Cells[1][i].c_str());
            ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
            ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
        }
    }
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1925-1960). CheckCEIDExist/SendCeid are THGem's own methods (HGemPtr->,
// unchanged); wire -> ActiveWire->.
void HTGem::S6F16_EventReportData()
{
    int len, ret;
    unsigned char Type;
    AnsiString CEID;
    AnsiString S;
    unsigned iCeid,iDataID=1;

    ret=ActiveWire->GetDataItemLenAndType(len,Type);
    if(ret==1 && ActiveWire->DataItemIn(len, Type, CEID)==1)
    {
        iCeid=(unsigned)atoi(CEID.c_str());
        if(HGemPtr->CheckCEIDExist(CEID))
        {
            //if(IsEnableEvent(1,iCeid))                                        // 若 Host 要求 Equipment 送回某一個 CEID
            {                                                                   // 但此 CEID 並未被 enable 那麼依然回報嗎
                ActiveWire->InitLocalHead(6, 16, 0);                            // 目前先寫一定回報,若要依　enable 來決定
                ActiveWire->DataItemOut(3, HType.LIST_TYPE, NULL);              // 則以下的　mark 打開就好
                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &iDataID);
                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE, &iCeid);
                HGemPtr->SendCeid(iCeid);
                ActiveWire->SendLocalData();
            }
        }
        else
        {
            ActiveWire->InitLocalHead(6, 16, 0);
            ActiveWire->DataItemOut(0, HType.UINT_4_TYPE, &iCeid);
            ActiveWire->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S6,F15 Data Format error !!!");
    }
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1962-1997). Same rule as S6F16 above. GOLDEN BUG preserved verbatim: the
// "CEID not exist" error path below sends `InitLocalHead(6, 16, 0)` -- S,F
// 6,16, NOT 6,18 (a real golden copy-paste bug from S6F16's own error path
// right above it, confirmed by direct golden read, uHGemClass.cpp:1988) --
// not "fixed" to (6,18,0) here, per this project's faithful-translation
// mandate.
void HTGem::S6F18_AnnotatedEventReportData()
{
    int len, ret;
    unsigned char Type;
    AnsiString CEID;
    AnsiString S;
    unsigned iCeid,iDataID=1;

    ret=ActiveWire->GetDataItemLenAndType(len, Type);
    if(ret==1 && ActiveWire->DataItemIn(len, Type, CEID)==1)
    {
        iCeid=(unsigned)atoi(CEID.c_str());
        if(HGemPtr->CheckCEIDExist(CEID))
        {
            //if(IsEnableEvent(1,iCeid))                                        // 若 Host 要求 Equipment 送回某一個 CEID
            {                                                                   // 但此 CEID 並未被 enable 那麼依然回報嗎
                ActiveWire->InitLocalHead(6, 18, 0);                            // 目前先寫一定回報,若要依　enable 來決定
                ActiveWire->DataItemOut(3, HType.LIST_TYPE,NULL);               // 則以下的　mark 打開就好
                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE,&iDataID);
                ActiveWire->DataItemOut(1, HType.UINT_4_TYPE,&iCeid);
                HGemPtr->SendAnnotatedCeid(iCeid);
                ActiveWire->SendLocalData();
            }
        }
        else
        {
            ActiveWire->InitLocalHead(6, 16, 0);                               // golden bug preserved -- see comment above
            ActiveWire->DataItemOut(0, HType.UINT_4_TYPE, &iCeid);
            ActiveWire->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S6,F17 Data Format error !!!");
    }
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 1999-2052). stdGridReportID is THGem's own StringGrid (HGemPtr->,
// unchanged); SV_ID -> HGemPtr->SvEcReg.SV_ID (SV/EC bookkeeping embed);
// DataItemOutSV -> HGemPtr-> (THGem's own method); wire -> ActiveWire->.
void HTGem::S6F20_IndividualReportData()
{
    int ret, x, y, ict, len;
    AnsiString RptID,SVID;
    unsigned char Type;

    ret=ActiveWire->GetDataItemLenAndType(len, Type);

    if(ret==1 && ActiveWire->DataItemIn(len, Type, RptID)==1)
    {
        for(y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
        {
            if(HGemPtr->stdGridReportID->Cells[0][y]==RptID)
            {
                ict=0;
                for(x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                {
                    if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                    {
                        SVID=HGemPtr->stdGridReportID->Cells[x][y];
                        if(HGemPtr->SvEcReg.SV_ID->IndexOf(SVID)>=0)             //Ifor 20260402: fix IndexOf>=0 (was >0, would miss index 0)
                        {
                            ict++;
                        }
                        else
                        {
                            S9F7_IllegalData("S6,F19 Invalid SVID in Report ");
                            return;
                        }
                    }
                }
                ActiveWire->InitLocalHead(6, 20, 0);
                ActiveWire->DataItemOut(ict, HType.LIST_TYPE, NULL);
                for(x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                {
                    if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                    {
                        SVID=HGemPtr->stdGridReportID->Cells[x][y];
                        HGemPtr->DataItemOutSV(SVID);
                    }
                }
                ActiveWire->SendLocalData();
                return;
            }
        }
    }
    S9F7_IllegalData("S6,F19 Data Format error !!!");
}
//---------------------------------------------------------------------------
void HTGem::S6F24_RequestSpooledDataAcknowledgementSend()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members bSpoolActive/bBeginTransferSpool/GemSpoolPath) -- golden SECSGEM/uHGemClass.cpp:2053-2079
#endif
}
//---------------------------------------------------------------------------
// [S7,F2] Process Program Load Grant (PPGNT).
// AI(W906-uHGemClass-Unlock2) 20260716 UN-GATED (golden SECSGEM/uHGemClass.cpp:2081-2113):
// its sole recorded blocker, GetDataItemLenAndTypeAndDelete, is now a real
// WireCodec method (SecsWireCodec.h/.cpp "WAVE 3" addendum); everything else
// in this body was already WireCodec-only (DataItemIn/GetDataItemLenAndType/
// LocalAcknowledge), and its error path (S9F7_IllegalData) was un-gated back
// in the first integrate wave. `HGemPtr->` -> `WireCodec.`, nothing else
// changed vs. golden.
//---------------------------------------------------------------------------
int HTGem::S7F2_ProcessProgramLoadGrant()
{
// L,2
//    1. <PPID>          // ascii
//    2. <LENGTH>        // int ,uint
    int ret,len;
    unsigned char Type;
    AnsiString PPID;
    if(ActiveWire->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        ret=ActiveWire->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            ActiveWire->DataItemIn(len, Type, PPID);
            ret=ActiveWire->GetDataItemLenAndTypeAndDelete(len, Type);
            if(ret==1 && len==1 &&
                 (Type==HType.UINT_1_TYPE || Type==HType.UINT_2_TYPE ||
                  Type==HType.UINT_4_TYPE || Type==HType.UINT_8_TYPE ||
                  Type==HType.INT_1_TYPE  || Type==HType.INT_2_TYPE  ||
                  Type==HType.INT_4_TYPE  || Type==HType.INT_8_TYPE))
            {
                ActiveWire->LocalAcknowledge(7, 2, 0);
                return 1;
            }
        }
    }
    S9F7_IllegalData("S7,F1 Data Format error !!!");
    return 0;
}
//---------------------------------------------------------------------------
void HTGem::S7F18_DeleteProcessProgramAcknowledge()                             // AI(W5-SECSGEM-Translate) 20260710: __fastcall dropped (see .h note)
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::GetLastOpenFN/member UpLoadPath + DeleteDirectory (W3 DEFERRED, not yet translated)) -- golden SECSGEM/uHGemClass.cpp:2115-2166
#endif
}
//---------------------------------------------------------------------------
void HTGem::S7F20_CurrentEPPDData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members SV_70_UNT1_ReceipeStruct/UploadFileString + THGem::SetReceipeDirectoryAndGlobalName) -- golden SECSGEM/uHGemClass.cpp:2168-2189
#endif
}
//---------------------------------------------------------------------------
void HTGem::Process_S7F20_CurrentEPPIDData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TCheckListBox GemRemoteReceipeList + GetDataItemLenAndTypeAndDelete) -- golden SECSGEM/uHGemClass.cpp:2191-2220
#endif
}
//---------------------------------------------------------------------------
// [S9,F1] Unrecognized Device ID.                                //Ifor 20260402
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2222-2228):
// this and its 4 S9Fx siblings below share an identical 4-call shape --
// StringOut+InitLocalHead+DataItemOut(ASCII,S)+SendLocalData -- all now
// WireCodec methods, nothing else needed.
//---------------------------------------------------------------------------
void HTGem::S9F1_UnrecognizedDeviceID(AnsiString S)
{
    ActiveWire->StringOut(S);
    ActiveWire->InitLocalHead(9, 1, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, S);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F3] Unrecognized Stream Function Type.                    //KenHsieh 20221006
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2232-2238).
//---------------------------------------------------------------------------
void HTGem::S9F3_Unrecognized_Stream_Function_Type(AnsiString S)
{
    ActiveWire->StringOut(S);
    ActiveWire->InitLocalHead(9, 3, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, S);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F5] Unrecognized Function Type.                            //Ifor 20260402
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2242-2248).
//---------------------------------------------------------------------------
void HTGem::S9F5_UnrecognizedFunctionType(AnsiString S)
{
    ActiveWire->StringOut(S);
    ActiveWire->InitLocalHead(9, 5, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, S);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F7] Illegal Data.
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2253-2259).
// This is the most-called-internally of the 5 (nearly every other gated
// method in this file calls it on a format-error path) -- un-gating it means
// those still-gated callers now emit a REAL S9F7 reply instead of silently
// swallowing the error, even while the caller's own main body stays gated.
//---------------------------------------------------------------------------
void HTGem::S9F7_IllegalData(AnsiString S)
{
    ActiveWire->StringOut(S);
    ActiveWire->InitLocalHead(9, 7, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, S);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F9] Transaction Timer Timeout.                             //Ifor 20260402
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2263-2269).
//---------------------------------------------------------------------------
void HTGem::S9F9_TransactionTimerTimeout(AnsiString S)
{
    ActiveWire->StringOut(S);
    ActiveWire->InitLocalHead(9, 9, 0);
    ActiveWire->DataItemOut(HType.ASCII_TYPE, S);
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S10,F4] Terminal Display Single Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S10F4_TerminalDisplaySingleAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members/widgets TerminalDisplayIndex/TerminalMemoPtr/TerminalListboxPtr/TerminalEditPtr/TerminalPanelPtr(+2) + GetDataItemLenAndTypeAndDelete) -- golden SECSGEM/uHGemClass.cpp:2274-2382
#endif
}
//---------------------------------------------------------------------------
// [S10,F6] Terminal Display Multi-Block Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S10F6_TerminalDisplayMultiBlockAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members/widgets TerminalDisplayIndex/TerminalMemoPtr/TerminalListboxPtr/TerminalEditPtr/TerminalPanelPtr(+2) + GetDataItemLenAndTypeAndDelete (called twice)) -- golden SECSGEM/uHGemClass.cpp:2387-2431
#endif
}
//---------------------------------------------------------------------------
// [S100,F4] Report All Alarm.
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 2437-2450) -- "no defect" method (§2 category ①): strGrdAlarm (HGemPtr->,
// unchanged) + wire (-> ActiveWire->) is the WHOLE dependency chain.
void HTGem::S100F4_ReportAllAlarm()
{
    bool type;
    ActiveWire->InitLocalHead(100, 4, 0);
    ActiveWire->DataItemOut(HGemPtr->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
    {
        ActiveWire->DataItemOut(3, HType.LIST_TYPE ,NULL);
        ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[1][i]);
        ActiveWire->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
        type=atoi(HGemPtr->strGrdAlarm->Cells[2][i].c_str())!=0;
        ActiveWire->DataItemOut(1, HType.BOOLEAN_TYPE, &type);
    }
    ActiveWire->SendLocalData();
}
//---------------------------------------------------------------------------
// [S101,F2] Current EPPD Data (variant 1).
//---------------------------------------------------------------------------
void HTGem::S101F2_CurrentEPPDData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members SV_70_UNT1_ReceipeStruct/UploadFileString/SV_71_ASCII_FilenameExtened) -- golden SECSGEM/uHGemClass.cpp:2456-2476
#endif
}
//---------------------------------------------------------------------------
// [S101,F4] Current EPPD Data (variant 2).
//---------------------------------------------------------------------------
void HTGem::S101F4_CurrentEPPDData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members SV_70_UNT1_ReceipeStruct/UploadFileString/SV_71_ASCII_FilenameExtened) -- golden SECSGEM/uHGemClass.cpp:2482-2502
#endif
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 2505-2513) -- "no defect" method (§2 category ①): `bDisableBinaryShow`/
// `LocalAcknowledge` are SecsWireCodec members/methods (-> ActiveWire->,
// D1/D2 precedent, NOT THGem's own -- corrects this stub's own prior gate
// comment, which assumed no engine home existed for them); MoveCheckCallBack/
// bReceiveS101F5 are THGem's own members (HGemPtr->, unchanged, added by the
// SysModWire wave). S101F6_StoreHostUploadFile itself STAYS GATED (needs
// UpLoadPath/GemRemoteReceipeList/SV_70_UNT1_ReceipeStruct/
// SV_71_ASCII_FilenameExtened/bFinishDownloadFile -- none in this wave's
// scope, deferred to Wave 3b per design doc) -- calling a gated no-op stub
// is itself unconditionally safe (matches the project's established
// "caller un-gates independently of its gated callee" precedent, e.g.
// S2F24_TraceInitializeAcknowledge/S2F24_TraceInitializeAcknowledgeSub).
void HTGem::S101F6()
{
    ActiveWire->bDisableBinaryShow=true;
    S101F6_StoreHostUploadFile();
    ActiveWire->bDisableBinaryShow=false;
    if(HGemPtr->MoveCheckCallBack!=NULL)
        HGemPtr->MoveCheckCallBack();
    HGemPtr->bReceiveS101F5=true;
    ActiveWire->LocalAcknowledge(101, 6, 0);
}
//---------------------------------------------------------------------------
// [S101,F6] Store Host Upload File.
//---------------------------------------------------------------------------
void HTGem::S101F6_StoreHostUploadFile()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem member UpLoadPath + VCL TCheckListBox GemRemoteReceipeList + THGem members SV_70_UNT1_ReceipeStruct/SV_71_ASCII_FilenameExtened/bFinishDownloadFile) -- golden SECSGEM/uHGemClass.cpp:2519-2581
#endif
}
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 2583-2595) -- same shape/reasoning as S101F6 above. S101F8_StoreHostUploadFile
// itself STAYS GATED (same deferred blocker list, Wave 3b).
void HTGem::S101F8()
{
    ActiveWire->bDisableBinaryShow=true;
    S101F8_StoreHostUploadFile();
    ActiveWire->bDisableBinaryShow=false;
    if(HGemPtr->MoveCheckCallBack!=NULL)
        HGemPtr->MoveCheckCallBack();
    HGemPtr->bReceiveS101F7=true;
    ActiveWire->LocalAcknowledge(101, 8, 0);
}
//---------------------------------------------------------------------------
// [S101,F8] Store Host Upload File (variant).
//---------------------------------------------------------------------------
void HTGem::S101F8_StoreHostUploadFile()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem member CurrentDirectory + VCL TCheckListBox GemRemoteReceipeList) -- golden SECSGEM/uHGemClass.cpp:2597-2641
#endif
}
//---------------------------------------------------------------------------
// [S125,F2] Enable/Disable EC Data Acknowledge.                  //wei 20150630
//---------------------------------------------------------------------------
void HTGem::S125F2_EnableDisableECDataAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::EnableDisableECDataAll/EnableDisableECData) -- golden SECSGEM/uHGemClass.cpp:2643-2680
#endif
}
//---------------------------------------------------------------------------
// SetECValue -- writes an EC value out to *PtrSour by ECID's registered type.
// AI(W906-uHGemClass-Unlock) 20260713 STILL GATED -- checked in full against SvEcReg,
// NOT un-gateable today: the raw-pointer (EC_VCL_NAME=="0") dispatch ladder
// alone would resolve entirely through SvEcReg (EC_ID/EC_TYPE/EC_Ptr), but
// golden's `IsVCL==1` branch (golden :2705-2717) `dynamic_cast`s the stored
// pointer to TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/TRadioGroup/
// TStringList -- none of the first 6 exist in vclcompat today. UNLIKE
// SecsSvEcRegistration.cpp's GetECDataValue (which gates the analogous
// dynamic_cast branch but has a rigorous registration-time proof that path
// is provably unreachable from ITS OWN callers), SetECValue cannot offer the
// same proof -- SecsSvEcRegistration.cpp's own TObject*-overload
// SetECDataPointer DOES register ECs with EC_VCL_NAME=="1", so IsVCL==1 is a
// real, reachable case in a live system. This remains true regardless of
// whether SetECValue's caller is itself gated: AI(W906-uHGemClass-Unlock2)
// 20260716 un-gated S2F15_UpdateNewEquipmentConstant below (a real caller of
// this function now), which does NOT change SetECValue's own reachability
// proof -- the IsVCL==1 branch is selected by whatever ECID happens to be
// registered, not by the caller, so SetECValue stays fully gated here.
// Gating only the IsVCL==1 sub-branch without a reachability proof would be
// forcing a partial translation through on an unproven assumption -- left
// fully gated per this project's "do not force it through" instruction
// instead. (Safe either way: S2F15_UpdateNewEquipmentConstant's calls into
// this still-gated stub simply no-op, exactly like CheckECValue's un-gating
// in the prior wave already established as an acceptable interim state.)
//---------------------------------------------------------------------------
void HTGem::SetECValue(unsigned ECID, void *PtrSour)
{
#if 0 // TODO(future THGem/vclcompat wave, needs TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/TRadioGroup in vclcompat) -- golden SECSGEM/uHGemClass.cpp:2682-2882
#endif
    (void)ECID;
    (void)PtrSour;
}
//---------------------------------------------------------------------------
// [S2,F15] Update New Equipment Constant.                //wei 20170417 (Steven)
// AI(W906-uHGemClass-Unlock2) 20260716 UN-GATED (golden SECSGEM/uHGemClass.cpp:2884-3024):
// both recorded blockers are now real WireCodec methods
// (GetDataItemLenAndTypeAndDelete / SendInvalidDataMessageToHost, SecsWireCodec
// "WAVE 3" addendum); every other call in this body was already
// WireCodec-only (GetDataItemLenAndType/DataItemIn) or a same-class virtual
// call (SetECValue -- still gated above, see its own comment; ReloadParameter
// -- already an inline no-op in the header). `HGem->` -> `WireCodec.`,
// nothing else changed vs. golden.
// PRESERVED GOLDEN QUIRK (confirmed by direct read, not a translation bug):
// the ASCII branch's `Str=new char[len+100]` is never `delete[]`d anywhere in
// golden (uHGemClass.cpp's ASCII branch, ~golden :2955-2960) -- a genuine
// golden per-call memory leak on every ASCII-typed EC update, preserved
// verbatim (not "fixed" into a delete[]).
//---------------------------------------------------------------------------
int HTGem::S2F15_UpdateNewEquipmentConstant()
{
    int EClen, i, len;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64 uint8EC;                                                   //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC, ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
    AnsiString S;

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            for(i=0; i<EClen; i++)
            {
                if(ActiveWire->GetDataItemLenAndTypeAndDelete(len, Type)==1)
                {
                    if(Type!=HType.LIST_TYPE || len!=2)
                        break;
                    // get ECID

                    ActiveWire->GetDataItemLenAndType(len, Type);
                    ActiveWire->DataItemIn(len, Type, S);
                    ECID=(unsigned)atoi(S.c_str());

                    ActiveWire->GetDataItemLenAndType(len, Type);

                    if(Type==HType.UINT_1_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &uint1EC)==1)
                            SetECValue(ECID, &uint1EC);
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &uint2EC)==1)
                            SetECValue(ECID, &uint2EC);
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &uint4EC)==1)
                            SetECValue(ECID, &uint4EC);
                    }
                    else if(Type==HType.UINT_8_TYPE)                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(ActiveWire->DataItemIn(1, Type, &uint8EC)==1)
                            SetECValue(ECID, &uint8EC);
                    }
                    else if(Type==HType.INT_1_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &int2EC)==1)
                            SetECValue(ECID, &int2EC);
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        if(ActiveWire->DataItemIn(1, Type, &int4EC)==1)
                            SetECValue(ECID, &int4EC);
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(ActiveWire->DataItemIn(1, Type, &int8EC)==1)
                            SetECValue(ECID, &int8EC);
                    }
                    else if(Type==HType.ASCII_TYPE)
                    {
                        char *Str;
                        Str=new char [(size_t)len+100];
                        if(ActiveWire->DataItemIn(len, Type, Str)==1)
                            SetECValue(ECID, Str);
                        // NOTE: golden never delete[]s Str here -- see this
                        // function's own "PRESERVED GOLDEN QUIRK" comment above.
                    }
                    else if(Type==HType.BINARY_TYPE)
                    {
                        if(ActiveWire->DataItemIn(len, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.BOOLEAN_TYPE)
                    {
                        if(ActiveWire->DataItemIn(len, Type, &int1EC)==1)
                        {
                            SetECValue(ECID, &int1EC);
                        }
                    }
                    else if(Type==HType.FT_4_TYPE)
                    {
                        if(ActiveWire->DataItemIn(len, Type, &floatEC)==1)
                            SetECValue(ECID, &floatEC);
                    }
                    else if(Type==HType.FT_8_TYPE)
                    {
                        if(ActiveWire->DataItemIn(len, Type, &doubleEC)==1)
                            SetECValue(ECID, &doubleEC);
                    }
                    else                                                        // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
                    {
                        ActiveWire->SendInvalidDataMessageToHost("error format");
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    ReloadParameter();
    return 0;                                                                   //sucess  //JerryYang 20250120 : modify
}
//---------------------------------------------------------------------------
// [S2,F15] Check New Equipment Constant.                 //wei 20170417 (Steven)
// AI(W906-uHGemClass-Unlock2) 20260716 UN-GATED (golden SECSGEM/uHGemClass.cpp:3026-3190):
// same rationale as S2F15_UpdateNewEquipmentConstant immediately above --
// both recorded blockers are now real WireCodec methods; the remaining calls
// are WireCodec-only or a same-class virtual call (CheckECValue -- already
// un-gated in the prior wave). `HGem->` -> `WireCodec.`, nothing else
// changed vs. golden.
//---------------------------------------------------------------------------
int HTGem::S2F15_CheckNewEquipmentConstant()                                    //wei 20170417 (Steven) add S2F15
{
    int EClen, i, len, ret;
    unsigned char Type;
    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64  uint8EC;                                                  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC;
    AnsiString ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;

    /*
        L,n
        1. L,2
        1. <ECID1>
        2. <ECV1>
        2. L,2
        .
        .
        n. L,2
        1. <ECIDn>
        2. <ECVn>
    */

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(EClen, Type)!=1)
        return -1;
    if(Type!=HType.LIST_TYPE)
        return -1;
    if(EClen<1)
        return -1;
    for(i=0; i<EClen; i++)
    {
        if(ActiveWire->GetDataItemLenAndTypeAndDelete(len, Type)!=1)
            return -1;
        if(Type!=HType.LIST_TYPE || len!=2)
            return -1;
        ret=1;
        if(ActiveWire->GetDataItemLenAndType(len, Type)==1)
        {
            if(ActiveWire->DataItemIn(len, Type, ECID)!=1)
                return -1;
        }
        else
        {
            return -1;
        }

        if(ActiveWire->GetDataItemLenAndType(len, Type)==1)
        {
            if(Type==HType.UINT_1_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &uint1EC)==1)
                    ret=CheckECValue(ECID, &uint1EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &uint2EC)==1)
                    ret=CheckECValue(ECID, &uint2EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &uint4EC)==1)
                    ret=CheckECValue(ECID, &uint4EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(ActiveWire->DataItemIn(1, Type, &uint8EC)==1)
                    ret=CheckECValue(ECID, &uint8EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &int2EC)==1)
                    ret=CheckECValue(ECID, &int2EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                if(ActiveWire->DataItemIn(1, Type, &int4EC)==1)
                    ret=CheckECValue(ECID, &int4EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(ActiveWire->DataItemIn(1, Type, &int8EC)==1)
                    ret=CheckECValue(ECID, &int8EC);
                else
                    return -1;
            }
            else if(Type==HType.ASCII_TYPE)
            {
                char *Str;
                Str=new char [(size_t)len+100];
                if(ActiveWire->DataItemIn(len, Type, Str)==1)
                    ret=CheckECValue(ECID, Str);
                else
                    return -1;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                if(ActiveWire->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                if(ActiveWire->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                if(ActiveWire->DataItemIn(len, Type, &floatEC)==1)
                    ret=CheckECValue(ECID, &floatEC);
                else
                    return -1;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                if(ActiveWire->DataItemIn(len, Type, &doubleEC)==1)
                    ret=CheckECValue(ECID, &doubleEC);
                else
                    return -1;
            }
            else                                                                // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
            {
                ActiveWire->SendInvalidDataMessageToHost("error format");
                return -1;
            }

            if(ret!=0)
                return ret;
        }
        else
        {
            ActiveWire->SendInvalidDataMessageToHost("GetDataItemLenAndType Error");
            return -1;
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
// CheckECValue -- validates an ECID/value pair against its registered
// Min/Max bounds, returning 0 (in range / no bound to check) or 3 (out of
// range); golden's own not-found path defaults to `return 1;` (golden
// :3558-3559), matched here.
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:3192-3560):
// the ONLY one of the SetECValue/S2F15_Update/S2F15_Check/CheckECValue "EC
// value" cluster with zero VCL-widget dynamic_cast and zero
// GetDataItemLenAndTypeAndDelete call -- entirely EC_ID/EC_TYPE/EC_Ptr_Max/
// EC_Ptr_Min/EC_Ptr_Max_Value/EC_Ptr_Min_Value (now SvEcReg) + StringOut
// (now WireCodec.StringOut) on the not-found path. Golden itself accesses
// these through the GLOBAL `HGem` (not the member `HGemPtr`) -- both are
// THGem*, out of scope either way; this translation collapses both onto the
// one HTGem instance's own SvEcReg/WireCodec (see uHGemClass.h's own design
// note), a deliberate, harmless simplification since golden only ever has
// one live THGem singleton in practice too.
//---------------------------------------------------------------------------
int HTGem::CheckECValue(AnsiString ECID, void *PtrSour)
{
    int i;
    unsigned char Type;
    void *PMax, *PMin;
    AnsiString PMax_Value, PMin_Value;

    i=SvEcReg.EC_ID->IndexOf(ECID);
    if(i>=0)
    {
        Type=(unsigned char)atoi(SvEcReg.EC_TYPE->GetString(i).c_str());
        PMax=SvEcReg.EC_Ptr_Max->GetItem(i);
        PMin=SvEcReg.EC_Ptr_Min->GetItem(i);

        PMax_Value=SvEcReg.EC_Ptr_Max_Value->GetString(i);
        PMin_Value=SvEcReg.EC_Ptr_Min_Value->GetString(i);

        if(Type==HType.ASCII_TYPE)
        {
            return 0;
        }
        else if(Type==HType.BINARY_TYPE)
        {
            return 0;
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            return 0;
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr2, *PtrMax, *PtrMin;
            char PtrMax_Value, PtrMin_Value;

            PtrMax=(char *)PMax;
            PtrMin=(char *)PMin;

            PtrMax_Value=(char)atoi(PMax_Value.c_str());
            PtrMin_Value=(char)atoi(PMin_Value.c_str());                        //2013/09/17 lee

            Ptr2=(char *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr2, *PtrMax, *PtrMin;
            short PtrMax_Value, PtrMin_Value;

            PtrMax=(short *)PMax;
            PtrMin=(short *)PMin;
            PtrMax_Value=(short)atoi(PMax_Value.c_str());
            PtrMin_Value=(short)atoi(PMin_Value.c_str());                       //2013/09/17 lee
            Ptr2=(short *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return  3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr2, *PtrMax, *PtrMin;
            int PtrMax_Value, PtrMin_Value;
            PtrMax=(int *)PMax;
            PtrMin=(int *)PMin;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());

            Ptr2=(int *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_8_TYPE)                                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            __int64 *Ptr2, *PtrMax, *PtrMin;
            __int64 PtrMax_Value, PtrMin_Value;
            PtrMax=(__int64 *)PMax;
            PtrMin=(__int64 *)PMin;

            PtrMax_Value=strtoll(PMax_Value.c_str(), NULL, 10);
            PtrMin_Value=strtoll(PMin_Value.c_str(), NULL, 10);

            Ptr2=(__int64 *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr2,*PtrMax,*PtrMin;
            unsigned char PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned char *)PMax;
            PtrMin=(unsigned char *)PMin;
            Ptr2=(unsigned char *)PtrSour;
            PtrMax_Value=(unsigned char)atoi(PMax_Value.c_str());
            PtrMin_Value=(unsigned char)atoi(PMin_Value.c_str());               //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>=(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<=(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr2,*PtrMax,*PtrMin;
            unsigned short PtrMax_Value,PtrMin_Value;

            PtrMax=(unsigned short *)PMax;
            PtrMin=(unsigned short *)PMin;
            Ptr2=(unsigned short *)PtrSour;
            PtrMax_Value=(unsigned short)atoi(PMax_Value.c_str());
            PtrMin_Value=(unsigned short)atoi(PMin_Value.c_str());              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>=(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<=(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int  *Ptr2,*PtrMax,*PtrMin;
            unsigned int PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned int *)PMax;
            PtrMin=(unsigned int *)PMin;
            Ptr2=(unsigned int *)PtrSour;
            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_8_TYPE)                                        //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            unsigned __int64 *Ptr2,*PtrMax,*PtrMin;
            unsigned __int64 PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned __int64 *)PMax;
            PtrMin=(unsigned __int64 *)PMin;
            Ptr2=(unsigned __int64 *)PtrSour;
            PtrMax_Value=(unsigned)strtoll(PMax_Value.c_str(), NULL, 10);
            PtrMin_Value=(unsigned)strtoll(PMin_Value.c_str(), NULL, 10);

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr2, *PtrMax, *PtrMin;
            float PtrMax_Value,PtrMin_Value;

            PtrMax=(float *)PMax;
            PtrMin=(float *)PMin;
            Ptr2=(float *)PtrSour;

            PtrMax_Value=(float)atoi(PMax_Value.c_str());
            PtrMin_Value=(float)atoi(PMin_Value.c_str());                       //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        // AI(W906-uHGemClass-Unlock) 20260713 PRESERVED GOLDEN BUG (confirmed by direct
        // cp950-aware read of golden SECSGEM/uHGemClass.cpp -- NOT a
        // translation error): golden has TWO consecutive `else if(Type==
        // HType.FT_4_TYPE)` branches back to back -- this second one uses
        // `double` (clearly meant to be the FT_8_TYPE branch) but tests the
        // SAME `FT_4_TYPE` condition as the branch immediately above, so it
        // is provably unreachable dead code in an if/else-if chain (the
        // first FT_4_TYPE branch above already claims every FT_4_TYPE value).
        // The REAL FT_8_TYPE handling is the NEXT branch below, correctly
        // conditioned on `HType.FT_8_TYPE`. Preserved verbatim, not "fixed"
        // by renaming this branch's condition or deleting it.
        else if(Type==HType.FT_4_TYPE)
        {
            double  *Ptr2,*PtrMax,*PtrMin;
            double PtrMax_Value,PtrMin_Value;
            PtrMax=(double *)PMax;
            PtrMin=(double *)PMin;
            Ptr2=(double *)PtrSour;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.FT_8_TYPE)                                          //Steven 20130730 : FT_4 --> FT_8
        {
            double  *Ptr2,*PtrMax,*PtrMin;
            double PtrMax_Value,PtrMin_Value;
            PtrMax=(double *)PMax;
            PtrMin=(double *)PMin;
            Ptr2=(double *)PtrSour;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        return 4;
    }
    ActiveWire->StringOut("ECID:"+ECID+" not exist !!");
    return 1;
}
//---------------------------------------------------------------------------
// [S103,F12] Status Variable Namelist Reply (with value).        //Steven 20140911
//---------------------------------------------------------------------------
// AI(W906-SvEcDataItem) 20260720: UN-GATED (golden SECSGEM/uHGemClass.cpp:
// 3562-3625). SV_ID -> HGemPtr->SvEcReg.SV_ID; DataItemOutSVNameListWithValue
// -> HGemPtr-> (THGem's own method); wire -> ActiveWire->.
void HTGem::S103F12_StatusVariableNamelistReply()                               //Steven 20140911 : Add S103F11
{
    int SVlen, i, len;
    unsigned char Type;

    AnsiString S;

    if(ActiveWire->GetDataItemLenAndTypeAndDelete(SVlen,Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            ActiveWire->InitLocalHead(103,12,0);
            if(SVlen==0)
            {
                len=HGemPtr->SvEcReg.SV_ID->Count;
                ActiveWire->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutSVNameListWithValue(HGemPtr->SvEcReg.SV_ID->GetString(i));
            }
            else
            {
                ActiveWire->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(ActiveWire->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(ActiveWire->DataItemIn(len, Type, S)==1)
                        {
                            if(HGemPtr->DataItemOutSVNameListWithValue(S)==false)
                                return;
                        }
                        else
                        {
                            S9F7_IllegalData("S103,F11 data format error");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S103,F11 data format error");
                        return;
                    }
                }
            }
            ActiveWire->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S103,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S103,F11 data format error");
    }
}
