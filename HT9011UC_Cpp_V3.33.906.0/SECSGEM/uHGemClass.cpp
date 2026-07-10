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
//  GATED (57 methods, `#if 0` + cited golden range + ACTIVE default stub):
//  every out-of-line S,F handler this class defines -- see each stub's
//  comment for its exact golden line range.  Grouped by SECS stream for
//  quick lookup: S1F(1,2,4,12,13,14,+Process,16,18,24), S2F(14,15x2,16,18,
//  24x2,26,30,32,34x3,36x2,38,42,44)+Set/CheckECValue, S5F(4,6,8),
//  S6F(16,18,20,24), S7F(2,18,20,+Process), S9F(1,3,5,7,9),
//  S10F(4,6), S100F4, S101F(2,4,6x2,8x2), S103F12, S125F2.
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

#include "vclcompat/vcl_compat.h"
#include "uHGemClass.h"

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
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:89-95
#endif
}
//---------------------------------------------------------------------------
// [S1,F2] OnLineData : Data signifying that the equipment is alive.
//---------------------------------------------------------------------------
void HTGem::S1F2_OnLineData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:102-111
#endif
}
//---------------------------------------------------------------------------
// [S1,F4] Selected Status Reply -- reports the value of each requested SVID.
//---------------------------------------------------------------------------
void HTGem::S1F4_SelectedStatusReply()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:119-264
#endif
}
//---------------------------------------------------------------------------
// [S1,F12] Status Variable Namelist Reply.
//---------------------------------------------------------------------------
void HTGem::S1F12_StatusVariableNamelistReply()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:266-324
#endif
}
//---------------------------------------------------------------------------
// [S1,F13] Establish Communications Request.
//---------------------------------------------------------------------------
void HTGem::S1F13_EstablishCommunicationsRequest()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:326-334
#endif
}
//---------------------------------------------------------------------------
// [S1,F14] Connect Request Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F14_ConnectRequestAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:341-368
#endif
}
//---------------------------------------------------------------------------
void HTGem::Process_S1F14_ConnectRequestAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:370-403
#endif
}
//---------------------------------------------------------------------------
// [S1,F16] OFFLINE Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F16_OFFLINEAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:408-417
#endif
}
//---------------------------------------------------------------------------
// [S1,F18] ONLINE Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S1F18_ONLINEAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:423-447
#endif
}
//---------------------------------------------------------------------------
// [S1,F24] Collection Event Namelist.                          //2014/01/01 lee
//---------------------------------------------------------------------------
void HTGem::S1F24_CollectionEventNamelist()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:450-587
#endif
}
//---------------------------------------------------------------------------
// [S2,F14] Equipment Constant Data.
//---------------------------------------------------------------------------
void HTGem::S2F14_EquipmentConstanData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:609-719
#endif
}
//---------------------------------------------------------------------------
// [S2,F16] New Equipment Constant Send Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S2F16_NewEquipmentConstantSendAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:731-779
#endif
}
//---------------------------------------------------------------------------
// [S2,F18] Date and Time Data.
//---------------------------------------------------------------------------
void HTGem::S2F18_DateandTimeData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:786-793
#endif
}
//---------------------------------------------------------------------------
// [S2,F24] Trace Initialize -- ack-code sub (called by the void wrapper below).
//---------------------------------------------------------------------------
int HTGem::S2F24_TraceInitializeAcknowledgeSub()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:810-985
#endif
    return 1;                                                                  // conservative default (deny); real TIACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F24_TraceInitializeAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:992-1000
#endif
}
//---------------------------------------------------------------------------
// [S2,F26] Diagnostic Loopback Data.
//---------------------------------------------------------------------------
void HTGem::S2F26_DiagnosticLoopbackData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1006-1024
#endif
}
//---------------------------------------------------------------------------
// [S2,F30] Equipment Constant Namelist Reply.
//---------------------------------------------------------------------------
void HTGem::S2F30_EquipmentConstantNamelistReply()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1037-1080
#endif
}
//---------------------------------------------------------------------------
// [S2,F32] Date And Time Acknowledge (uses IsCorrectDateFormat, kept ACTIVE above).
//---------------------------------------------------------------------------
void HTGem::S2F32_DateAndTimeAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1095-1216
#endif
}
//---------------------------------------------------------------------------
// [S2,F34] Define Report -- ack-code sub (DRACK).
//---------------------------------------------------------------------------
int HTGem::S2F34_DefineReportAcknowledgeSub()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1218-1291
#endif
    return 1;                                                                  // conservative default (deny); real DRACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F34_ProcessHostSendReportID()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1294-1361
#endif
}
//---------------------------------------------------------------------------
void HTGem::S2F34_DefineReportAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1363-1393
#endif
}
//---------------------------------------------------------------------------
// [S2,F36] Link Event Report -- ack-code sub (LRACK).
//---------------------------------------------------------------------------
int HTGem::S2F36_LinkEventReportAcknowledgeSub()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1395-1457
#endif
    return 1;                                                                  // conservative default (deny); real LRACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F36_LinkEventReportAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1460-1477
#endif
}
//---------------------------------------------------------------------------
void HTGem::S2F38_EnableDisableEventReportAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1480-1521
#endif
}
//---------------------------------------------------------------------------
// [S2,F42] Host Command Acknowledge (HCACK).
//---------------------------------------------------------------------------
int HTGem::S2F42_Host_Command_Acknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1523-1574
#endif
    return 1;                                                                  // conservative default (command does not exist); real HCACK decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S2F44_ResetSpoolingAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1576-1591
#endif
}
//---------------------------------------------------------------------------
void HTGem::S5F4_EnableDisableAlarmAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1593-1624
#endif
}
//---------------------------------------------------------------------------
void HTGem::S5F6_ListAlarmData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1626-1890
#endif
}
//---------------------------------------------------------------------------
void HTGem::S5F8_ListEnableAlarmAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1892-1923
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F16_EventReportData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1925-1960
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F18_AnnotatedEventReportData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1962-1997
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F20_IndividualReportData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:1999-2052
#endif
}
//---------------------------------------------------------------------------
void HTGem::S6F24_RequestSpooledDataAcknowledgementSend()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2053-2079
#endif
}
//---------------------------------------------------------------------------
// [S7,F2] Process Program Load Grant (PPGNT).
//---------------------------------------------------------------------------
int HTGem::S7F2_ProcessProgramLoadGrant()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2081-2113
#endif
    return 1;                                                                  // conservative default (not ready); real PPGNT decode gated above
}
//---------------------------------------------------------------------------
void HTGem::S7F18_DeleteProcessProgramAcknowledge()                             // AI(W5-SECSGEM-Translate) 20260710: __fastcall dropped (see .h note)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2115-2166
#endif
}
//---------------------------------------------------------------------------
void HTGem::S7F20_CurrentEPPDData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2168-2189
#endif
}
//---------------------------------------------------------------------------
void HTGem::Process_S7F20_CurrentEPPIDData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2191-2220
#endif
}
//---------------------------------------------------------------------------
// [S9,F1] Unrecognized Device ID.                                //Ifor 20260402
//---------------------------------------------------------------------------
void HTGem::S9F1_UnrecognizedDeviceID(AnsiString S)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2222-2228
#endif
    (void)S;
}
//---------------------------------------------------------------------------
// [S9,F3] Unrecognized Stream Function Type.                    //KenHsieh 20221006
//---------------------------------------------------------------------------
void HTGem::S9F3_Unrecognized_Stream_Function_Type(AnsiString S)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2232-2238
#endif
    (void)S;
}
//---------------------------------------------------------------------------
// [S9,F5] Unrecognized Function Type.                            //Ifor 20260402
//---------------------------------------------------------------------------
void HTGem::S9F5_UnrecognizedFunctionType(AnsiString S)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2242-2248
#endif
    (void)S;
}
//---------------------------------------------------------------------------
// [S9,F7] Illegal Data.
//---------------------------------------------------------------------------
void HTGem::S9F7_IllegalData(AnsiString S)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2253-2259
#endif
    (void)S;
}
//---------------------------------------------------------------------------
// [S9,F9] Transaction Timer Timeout.                             //Ifor 20260402
//---------------------------------------------------------------------------
void HTGem::S9F9_TransactionTimerTimeout(AnsiString S)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2263-2269
#endif
    (void)S;
}
//---------------------------------------------------------------------------
// [S10,F4] Terminal Display Single Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S10F4_TerminalDisplaySingleAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2274-2382
#endif
}
//---------------------------------------------------------------------------
// [S10,F6] Terminal Display Multi-Block Acknowledge.
//---------------------------------------------------------------------------
void HTGem::S10F6_TerminalDisplayMultiBlockAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2387-2431
#endif
}
//---------------------------------------------------------------------------
// [S100,F4] Report All Alarm.
//---------------------------------------------------------------------------
void HTGem::S100F4_ReportAllAlarm()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2437-2450
#endif
}
//---------------------------------------------------------------------------
// [S101,F2] Current EPPD Data (variant 1).
//---------------------------------------------------------------------------
void HTGem::S101F2_CurrentEPPDData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2456-2476
#endif
}
//---------------------------------------------------------------------------
// [S101,F4] Current EPPD Data (variant 2).
//---------------------------------------------------------------------------
void HTGem::S101F4_CurrentEPPDData()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2482-2502
#endif
}
//---------------------------------------------------------------------------
void HTGem::S101F6()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2505-2513
#endif
}
//---------------------------------------------------------------------------
// [S101,F6] Store Host Upload File.
//---------------------------------------------------------------------------
void HTGem::S101F6_StoreHostUploadFile()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2519-2581
#endif
}
//---------------------------------------------------------------------------
void HTGem::S101F8()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2583-2595
#endif
}
//---------------------------------------------------------------------------
// [S101,F8] Store Host Upload File (variant).
//---------------------------------------------------------------------------
void HTGem::S101F8_StoreHostUploadFile()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2597-2641
#endif
}
//---------------------------------------------------------------------------
// [S125,F2] Enable/Disable EC Data Acknowledge.                  //wei 20150630
//---------------------------------------------------------------------------
void HTGem::S125F2_EnableDisableECDataAcknowledge()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2643-2680
#endif
}
//---------------------------------------------------------------------------
// SetECValue -- writes an EC value out to *PtrSour by ECID's registered type.
//---------------------------------------------------------------------------
void HTGem::SetECValue(unsigned ECID, void *PtrSour)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2682-2882
#endif
    (void)ECID;
    (void)PtrSour;
}
//---------------------------------------------------------------------------
// [S2,F15] Update New Equipment Constant.                //wei 20170417 (Steven)
//---------------------------------------------------------------------------
int HTGem::S2F15_UpdateNewEquipmentConstant()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:2884-3024
#endif
    return 1;                                                                  // conservative default (deny); real EAC decode gated above
}
//---------------------------------------------------------------------------
// [S2,F15] Check New Equipment Constant.                 //wei 20170417 (Steven)
//---------------------------------------------------------------------------
int HTGem::S2F15_CheckNewEquipmentConstant()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:3026-3190
#endif
    return 1;                                                                  // conservative default (deny); real EAC decode gated above
}
//---------------------------------------------------------------------------
// CheckECValue -- validates an ECID/value pair; golden's own not-found path
// already defaults to `return 1;` (see golden :3558-3559), matched here.
//---------------------------------------------------------------------------
int HTGem::CheckECValue(AnsiString ECID, void *PtrSour)
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:3192-3560
#endif
    (void)ECID;
    (void)PtrSour;
    return 1;                                                                  // matches golden's own not-found default (golden :3559)
}
//---------------------------------------------------------------------------
// [S103,F12] Status Variable Namelist Reply (with value).        //Steven 20140911
//---------------------------------------------------------------------------
void HTGem::S103F12_StatusVariableNamelistReply()
{
#if 0 // TODO(W5-SECSGEM-engine, needs THGem) -- golden SECSGEM/uHGemClass.cpp:3562-3625
#endif
}
