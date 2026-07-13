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
//  GATED (47 of the original 57 methods remain gated, `#if 0` + cited golden
//  range + ACTIVE default stub) -- see each stub's comment for its exact
//  golden line range and (post integrate-wave) an honest note on exactly
//  which missing piece still blocks it.
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
//      call; its 3 siblings stay gated for exactly those reasons).
//
//  Every un-gated method's `HGemPtr->X(...)`/`HGem->X(...)` call became
//  `WireCodec.X(...)` (wire-codec ops) or `SvEcReg.X(...)` (EC/SV lists);
//  `HType`/`HTypeStruct` (shared global, already visible via
//  SecsWireCodec.h) needed no change. Nothing else in these 10 bodies
//  changed vs. golden.
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"
#include "uHGemClass.h"
#include <cstdlib>   // atoi (CheckECValue's Type/PMax_Value/PMin_Value decode)

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
extern void MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2 = "");

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
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
};
//---------------------------------------------------------------------------
HTGem::HTGem(THGem *HGemTmp)
{
    HGemPtr=HGemTmp;
    HandlerPath="";
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
}
//---------------------------------------------------------------------------
HTGem::HTGem(AnsiString Path)
{
    HandlerPath=Path;
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
void HTGem::S1F1_AreYouThereRequest()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem member bS1F2_OnLineData) -- golden SECSGEM/uHGemClass.cpp:89-95
#endif
}
//---------------------------------------------------------------------------
// [S1,F2] OnLineData : Data signifying that the equipment is alive.
//---------------------------------------------------------------------------
void HTGem::S1F2_OnLineData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckSFFormatOnlyHead + THGem members GemMDLN/GemSOFTREV) -- golden SECSGEM/uHGemClass.cpp:102-111
#endif
}
//---------------------------------------------------------------------------
// [S1,F4] Selected Status Reply -- reports the value of each requested SVID.
//---------------------------------------------------------------------------
void HTGem::S1F4_SelectedStatusReply()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::DataItemOutSV) -- golden SECSGEM/uHGemClass.cpp:119-264
#endif
}
//---------------------------------------------------------------------------
// [S1,F12] Status Variable Namelist Reply.
//---------------------------------------------------------------------------
void HTGem::S1F12_StatusVariableNamelistReply()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::DataItemOutSVNameList) -- golden SECSGEM/uHGemClass.cpp:266-324
#endif
}
//---------------------------------------------------------------------------
// [S1,F13] Establish Communications Request.
//---------------------------------------------------------------------------
void HTGem::S1F13_EstablishCommunicationsRequest()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members GemMDLN/GemSOFTREV) -- golden SECSGEM/uHGemClass.cpp:326-334
#endif
}
//---------------------------------------------------------------------------
// [S1,F14] Connect Request Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F14_ConnectRequestAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TCheckBox chkMoreMessageAbortProcess + GetDataItemLenAndTypeAndDelete + THGem members GemMDLN/GemSOFTREV/bReceiveEstablishCommunicationsRequest) -- golden SECSGEM/uHGemClass.cpp:341-368
#endif
}
//---------------------------------------------------------------------------
void HTGem::Process_S1F14_ConnectRequestAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members bWaitEstablishCommunicationsResponse/bWaitEstablishCommunicationsResponseError) -- golden SECSGEM/uHGemClass.cpp:370-403
#endif
}
//---------------------------------------------------------------------------
// [S1,F16] OFFLINE Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F16_OFFLINEAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckSFFormatOnlyHead + THGem::OffLine) -- golden SECSGEM/uHGemClass.cpp:408-417
#endif
}
//---------------------------------------------------------------------------
// [S1,F18] ONLINE Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F18_ONLINEAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckSFFormatOnlyHead + THGem member bOnLine + VCL TCheckBox GemCheckBoxAcceptHostOnlineRequest + THGem::OnLine/GetOnLineMode) -- golden SECSGEM/uHGemClass.cpp:423-447
#endif
}
//---------------------------------------------------------------------------
// [S1,F24] Collection Event Namelist.                          //2014/01/01 lee
//---------------------------------------------------------------------------
void HTGem::S1F24_CollectionEventNamelist()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TStringGrid widgets strGrdCEID/stdGridReportID) -- golden SECSGEM/uHGemClass.cpp:450-587
#endif
}
//---------------------------------------------------------------------------
// [S2,F14] Equipment Constant Data.
//---------------------------------------------------------------------------
void HTGem::S2F14_EquipmentConstanData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::DataItemOutEC) -- golden SECSGEM/uHGemClass.cpp:609-719
#endif
}
//---------------------------------------------------------------------------
// [S2,F16] New Equipment Constant Send Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S2F16_NewEquipmentConstantSendAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem member SReceiveDataBackup + THGem member MoveCheckCallBack (function pointer)) -- golden SECSGEM/uHGemClass.cpp:731-779
#endif
}
//---------------------------------------------------------------------------
// [S2,F18] Date and Time Data.
//---------------------------------------------------------------------------
void HTGem::S2F18_DateandTimeData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckSFFormatOnlyHead + THGem::GetTimeInfo + THGem member GemClock) -- golden SECSGEM/uHGemClass.cpp:786-793
#endif
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
        WireCodec.LocalAcknowledge(2,24,(unsigned char)ret);                     // int->uchar narrowing, matches golden's own implicit conversion
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
    ret=WireCodec.GetDataItemLenAndType(len,Type);
    if(Type==HType.BINARY_TYPE && ret==1)
    {
        Temp=new unsigned char [len+100];
        if(WireCodec.DataItemIn(len, Type, Temp)==1)
        {
            WireCodec.InitLocalHead(2, 26, 0);
            WireCodec.DataItemOut(len, HType.BINARY_TYPE, Temp);
            WireCodec.SendLocalData();
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
void HTGem::S2F30_EquipmentConstantNamelistReply()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::DataItemOutECNameList) -- golden SECSGEM/uHGemClass.cpp:1037-1080
#endif
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
int HTGem::S2F34_DefineReportAcknowledgeSub()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members slTempReportID/lTempReportIDContent + THGem::DeleteAllHostDefineReportID) -- golden SECSGEM/uHGemClass.cpp:1218-1291
#endif
    return 1;                                                                  // conservative default (deny); real DRACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F34_ProcessHostSendReportID()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckReportAlreadyDefine/IsValidSVID/DefineReportAcknowledgeAlreadyDefined/DefineReportAcknowledgeInvalidSVID/DeleteReportID/DeleteReportIDOfCeid/SetReportIDContent/DefineReportAcknowledgeInsufficientSpace/DefineReportAcknowledgeAccept/SaveEventReportData) -- golden SECSGEM/uHGemClass.cpp:1294-1361
#endif
}
//---------------------------------------------------------------------------
void HTGem::S2F34_DefineReportAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::DefineReportAcknowledgeFormatError) -- golden SECSGEM/uHGemClass.cpp:1363-1393
#endif
}
//---------------------------------------------------------------------------
// [S2,F36] Link Event Report -- ack-code sub (LRACK).
//---------------------------------------------------------------------------
int HTGem::S2F36_LinkEventReportAcknowledgeSub()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members slTempCeID/lTempCeIDContent + THGem::DeleteAllHostDefineCeid) -- golden SECSGEM/uHGemClass.cpp:1395-1457
#endif
    return 1;                                                                  // conservative default (deny); real LRACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F36_LinkEventReportAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::LinkReportAcknowledgeFormatError/ProcessHostSendReportLinkID) -- golden SECSGEM/uHGemClass.cpp:1460-1477
#endif
}
//---------------------------------------------------------------------------
void HTGem::S2F38_EnableDisableEventReportAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::CheckCEIDExist/EnableDisableEventReportAcknowledgeCeidNotExist/EnableDisableEventReportAcknowledgeFormatError/EnableDisableEventReport) -- golden SECSGEM/uHGemClass.cpp:1480-1521
#endif
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

    if(WireCodec.DataItemIn(2, HType.LIST_TYPE, NULL)==1)                       // 需要補充多重 Command
    {
        WireCodec.GetDataItemLenAndType(len, Type);
        if(Type==HType.ASCII_TYPE)
        {
            ret=WireCodec.DataItemIn(len, HType.ASCII_TYPE, CommandStr);
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
        WireCodec.InitLocalHead(2, 42, 0);

        if(HCACK==0)
        {
            WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
            WireCodec.DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            WireCodec.DataItemOut(0, HType.LIST_TYPE, NULL);
        }
        else
        {
            WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
            WireCodec.DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            WireCodec.DataItemOut(0, HType.LIST_TYPE, NULL);
        }

        WireCodec.SendLocalData();
        return 1;
    }
    else
    {
        HCACK=3;
        WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
        WireCodec.DataItemOut(1, HType.BINARY_TYPE, &HCACK);
        WireCodec.DataItemOut(0, HType.LIST_TYPE, NULL);
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
    WireCodec.InitLocalHead(2, 44,0);
    WireCodec.DataItemOut( 2,HType.LIST_TYPE, NULL);
    WireCodec.DataItemOut( 1,HType.BINARY_TYPE,&C);
    WireCodec.DataItemOut( 0,HType.LIST_TYPE, NULL);
    WireCodec.SendLocalData();
}
//---------------------------------------------------------------------------
void HTGem::S5F4_EnableDisableAlarmAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::EnableDisableAlarmAll/EnableDisableAlarm) -- golden SECSGEM/uHGemClass.cpp:1593-1624
#endif
}
//---------------------------------------------------------------------------
void HTGem::S5F6_ListAlarmData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TStringGrid strGrdAlarm + THGem::GetAlarmIndex) -- golden SECSGEM/uHGemClass.cpp:1626-1890
#endif
}
//---------------------------------------------------------------------------
void HTGem::S5F8_ListEnableAlarmAcknowledge()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TStringGrid strGrdAlarm) -- golden SECSGEM/uHGemClass.cpp:1892-1923
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F16_EventReportData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckCEIDExist/SendCeid) -- golden SECSGEM/uHGemClass.cpp:1925-1960
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F18_AnnotatedEventReportData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem::CheckCEIDExist/SendAnnotatedCeid) -- golden SECSGEM/uHGemClass.cpp:1962-1997
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F20_IndividualReportData()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TStringGrid stdGridReportID + THGem::DataItemOutSV) -- golden SECSGEM/uHGemClass.cpp:1999-2052
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F24_RequestSpooledDataAcknowledgementSend()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members bSpoolActive/bBeginTransferSpool/GemSpoolPath) -- golden SECSGEM/uHGemClass.cpp:2053-2079
#endif
}
//---------------------------------------------------------------------------
// [S7,F2] Process Program Load Grant (PPGNT).
//---------------------------------------------------------------------------
int HTGem::S7F2_ProcessProgramLoadGrant()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete) -- golden SECSGEM/uHGemClass.cpp:2081-2113
#endif
    return 1;                                                                  // conservative default (not ready); real PPGNT decode gated above
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
    WireCodec.StringOut(S);
    WireCodec.InitLocalHead(9, 1, 0);
    WireCodec.DataItemOut(HType.ASCII_TYPE, S);
    WireCodec.SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F3] Unrecognized Stream Function Type.                    //KenHsieh 20221006
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2232-2238).
//---------------------------------------------------------------------------
void HTGem::S9F3_Unrecognized_Stream_Function_Type(AnsiString S)
{
    WireCodec.StringOut(S);
    WireCodec.InitLocalHead(9, 3, 0);
    WireCodec.DataItemOut(HType.ASCII_TYPE, S);
    WireCodec.SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F5] Unrecognized Function Type.                            //Ifor 20260402
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2242-2248).
//---------------------------------------------------------------------------
void HTGem::S9F5_UnrecognizedFunctionType(AnsiString S)
{
    WireCodec.StringOut(S);
    WireCodec.InitLocalHead(9, 5, 0);
    WireCodec.DataItemOut(HType.ASCII_TYPE, S);
    WireCodec.SendLocalData();
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
    WireCodec.StringOut(S);
    WireCodec.InitLocalHead(9, 7, 0);
    WireCodec.DataItemOut(HType.ASCII_TYPE, S);
    WireCodec.SendLocalData();
}
//---------------------------------------------------------------------------
// [S9,F9] Transaction Timer Timeout.                             //Ifor 20260402
// AI(W906-uHGemClass-Unlock) 20260713 UN-GATED (golden SECSGEM/uHGemClass.cpp:2263-2269).
//---------------------------------------------------------------------------
void HTGem::S9F9_TransactionTimerTimeout(AnsiString S)
{
    WireCodec.StringOut(S);
    WireCodec.InitLocalHead(9, 9, 0);
    WireCodec.DataItemOut(HType.ASCII_TYPE, S);
    WireCodec.SendLocalData();
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
void HTGem::S100F4_ReportAllAlarm()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs VCL TStringGrid strGrdAlarm) -- golden SECSGEM/uHGemClass.cpp:2437-2450
#endif
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
void HTGem::S101F6()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members MoveCheckCallBack/bReceiveS101F5 (calls still-gated S101F6_StoreHostUploadFile)) -- golden SECSGEM/uHGemClass.cpp:2505-2513
#endif
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
void HTGem::S101F8()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs THGem members MoveCheckCallBack/bReceiveS101F7 (calls still-gated S101F8_StoreHostUploadFile)) -- golden SECSGEM/uHGemClass.cpp:2583-2595
#endif
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
// is provably unreachable from ITS OWN callers), SetECValue's caller
// (S2F15_UpdateNewEquipmentConstant, itself still gated below) cannot offer
// the same proof -- SecsSvEcRegistration.cpp's own TObject*-overload
// SetECDataPointer DOES register ECs with EC_VCL_NAME=="1", so IsVCL==1 is a
// real, reachable case in a live system, just not exercised by anything in
// this file's current scope. Gating only the IsVCL==1 sub-branch without
// that same reachability proof would be forcing a partial translation
// through on an unproven assumption -- left fully gated per this project's
// "do not force it through" instruction instead.
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
//---------------------------------------------------------------------------
int HTGem::S2F15_UpdateNewEquipmentConstant()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::SendInvalidDataMessageToHost) -- golden SECSGEM/uHGemClass.cpp:2884-3024
#endif
    return 1;                                                                  // conservative default (deny); real EAC decode gated above
}
//---------------------------------------------------------------------------
// [S2,F15] Check New Equipment Constant.                 //wei 20170417 (Steven)
//---------------------------------------------------------------------------
int HTGem::S2F15_CheckNewEquipmentConstant()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::SendInvalidDataMessageToHost) -- golden SECSGEM/uHGemClass.cpp:3026-3190
#endif
    return 1;                                                                  // conservative default (deny); real EAC decode gated above
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
    WireCodec.StringOut("ECID:"+ECID+" not exist !!");
    return 1;
}
//---------------------------------------------------------------------------
// [S103,F12] Status Variable Namelist Reply (with value).        //Steven 20140911
//---------------------------------------------------------------------------
void HTGem::S103F12_StatusVariableNamelistReply()
{
#if 0 // TODO(W906-uHGemClass-Unlock, needs GetDataItemLenAndTypeAndDelete + THGem::DataItemOutSVNameListWithValue) -- golden SECSGEM/uHGemClass.cpp:3562-3625
#endif
}
