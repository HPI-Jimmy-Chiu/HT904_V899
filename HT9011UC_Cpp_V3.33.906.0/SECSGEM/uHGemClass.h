//---------------------------------------------------------------------------
//  SECSGEM/uHGemClass.h  --  HTGem abstract base (SECS/GEM S,F-handler layer)
//
//  Translation wave: W5 (SECSGEM base layer)
//  Translator: AI(W5-SECSGEM-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemClass.h (115 lines, cp950)
//
//  WHAT THIS FILE IS
//  ------------------
//  `HTGem` is an abstract base class of ~90 virtual S,F (Stream,Function) SECS/
//  GEM message handlers (S1F1_AreYouThereRequest, S2F14_EquipmentConstanData,
//  ...).  The site-specific subclass (uHGemHT9045.h/.cpp, a ~6200-line override
//  layer, explicitly OUT OF SCOPE for this wave) overrides the ones it needs;
//  the rest keep HTGem's inline no-op/default body (already `{}`/`{return N;}`
//  in golden -- carried over verbatim below, zero gating needed for those).
//
//  INTERFACE-CUT: THGem forward-declared only
//  --------------------------------------------------------------------------
//  Golden includes "uHGemEquipment.h" for the `THGem` type (a ~6200-line VCL
//  TForm -- the actual byte-level SECS engine: InitLocalHead/DataItemIn/
//  DataItemOut/SendLocalData/StringOut, plus VCL widgets like strGrdCEID/
//  chkMoreMessageAbortProcess).  uHGemEquipment.h/.cpp are NOT part of this
//  unit's scope (recon confirmed HTGem itself is 0% VCL, but ~57 of its ~90
//  methods deref `HGemPtr` -- a `THGem*` -- for nearly every statement; see
//  uHGemClass.cpp's file-head note for the discrepancy this created).  `THGem`
//  is therefore only FORWARD-declared here (a bare `class THGem;`), matching
//  the project's established form-pointer interface-cut convention (see
//  KNOWLEDGE.md: "form 指標 extern PACKAGE TfXxx*->前置宣告 class TfXxx;(W7)").
//  A pointer to an incomplete type is a valid class member; only method BODIES
//  that dereference it need the full definition (gated in the .cpp -- see
//  there).
//
//  TRANSLATION RULES
//  ------------------
//    * __fastcall removed (S7F18_DeleteProcessProgramAcknowledge) -- neutralized
//      by vcl_compat.h anyway, but this project's convention is to drop it from
//      new translations and note it here, not rely on the macro silently.
//    * AnsiString / TStringList via vclcompat.
//    * Method list, order, names, signatures, and existing inline `{}` bodies
//      preserved VERBATIM from golden (this header carries zero behavior change
//      -- it is a pure interface transcription).
//---------------------------------------------------------------------------

#ifndef uHGemClassH
#define uHGemClassH
#include "vclcompat/vcl_compat.h"
//---------------------------------------------------------------------------

// AI(W5-SECSGEM-Translate) 20260710: THGem (uHGemEquipment.h) is the SECS
// engine VCL form -- out of scope for this unit (see file-head note above).
// Forward declaration only; HTGem never dereferences it in ACTIVE code (all
// HGemPtr-dereferencing method bodies are gated in uHGemClass.cpp pending a
// dedicated future wave that translates uHGemEquipment.h/.cpp).
class THGem;

extern AnsiString SYS_ECChangeID             ;    //pig 2014.04.23 KYEC SECS
extern AnsiString SYS_ECChangeIDOriginaValue ;    //pig 2014.04.23 KYEC SECS
extern AnsiString SYS_ECChangeIDNewValue     ;    //pig 2014.04.23 KYEC SECS
class HTGem
{
    private:
    protected:
    public:
        AnsiString HandlerPath;
        AnsiString DataPath;
        HTGem();
        HTGem(THGem *HGemTmp);
        HTGem(AnsiString Path);
        THGem *HGemPtr;
        TStringList *SecsAlarmMessage;   //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
        void UpdateDataPath(AnsiString Path);
        virtual ~HTGem()                        ;
        virtual void AddSV()                    {};
        virtual void AddEC()                    {};
        virtual void AddAlarmList()             {};
        virtual void AddCEID()                  {};
        virtual void AddReprot()                {};
        virtual void ReloadParameter()          {};
        virtual void LookForFile()              {};

        virtual void S1F1_AreYouThereRequest();
        virtual void S1F2_OnLineData();
        virtual void S1F4_SelectedStatusReply();
        virtual void S1F12_StatusVariableNamelistReply();
        virtual void S1F13_EstablishCommunicationsRequest();
        virtual void S1F14_ConnectRequestAcknowledge();
        virtual void Process_S1F14_ConnectRequestAcknowledge();
        virtual void S1F16_OFFLINEAcknowledge();
        virtual void S1F18_ONLINEAcknowledge();
        virtual void S1F24_CollectionEventNamelist();         //2014/01/01  lee

        virtual void S2F14_EquipmentConstanData();
        virtual int  S2F15_UpdateNewEquipmentConstant();
        virtual int  S2F15_CheckNewEquipmentConstant();     //wei 20170417 (Steven) add S2F15
        virtual void S2F16_NewEquipmentConstantSendAcknowledge();
        virtual void SetECValue(unsigned ECID, void *PtrSour);
        virtual int  CheckECValue(AnsiString ECID, void *PtrSour);
        virtual void S2F18_DateandTimeData();
        virtual int  S2F24_TraceInitializeAcknowledgeSub();
        virtual void S2F24_TraceInitializeAcknowledge();
        virtual void S2F26_DiagnosticLoopbackData();
        virtual void S2F32_DateAndTimeAcknowledge();
        virtual void S2F30_EquipmentConstantNamelistReply();
        virtual int  S2F34_DefineReportAcknowledgeSub();
        virtual void S2F34_ProcessHostSendReportID();
        virtual void S2F34_DefineReportAcknowledge();
        virtual int  S2F36_LinkEventReportAcknowledgeSub();
        virtual void S2F36_LinkEventReportAcknowledge();
        virtual void S2F38_EnableDisableEventReportAcknowledge();
        virtual int  S2F42_Host_Command_Acknowledge();
        virtual void S2F44_ResetSpoolingAcknowledge();

        virtual void S5F4_EnableDisableAlarmAcknowledge();
        virtual void S5F6_ListAlarmData();
        virtual void S5F8_ListEnableAlarmAcknowledge();

        virtual void S6F16_EventReportData();
        virtual void S6F18_AnnotatedEventReportData();
        virtual void S6F20_IndividualReportData();
        virtual void S6F24_RequestSpooledDataAcknowledgementSend();

        virtual int  S7F2_ProcessProgramLoadGrant();
        virtual void S7F4_ProcessProgramAcknowledge()                           {};
        virtual void S7F6_ProcessProgramData()                                  {};
        virtual void S7F6_ProcessProgramData(AnsiString FileName)               {};
        virtual void S7F18_DeleteProcessProgramAcknowledge();                          // AI(W5-SECSGEM-Translate) 20260710: __fastcall dropped (neutralized anyway; project convention)
        virtual void Process_S7F20_CurrentEPPIDData();
        virtual void S7F20_CurrentEPPDData();
        virtual int S7F24_FormattedProcessProgramSendAcknowledge()              {return 1;};
        virtual int ProcessS7F23FromatReceipe()                                 {return 2;};
        virtual int S7F26_FormattedProcessProgramData()                         {return 1;};
        virtual int ProcessS7F25FromatReceipe()                                 {return 2;};

        virtual void S9F1_UnrecognizedDeviceID(AnsiString S);                         //Ifor 20260402: S9F1 Unrecognized Device ID
        virtual void S9F5_UnrecognizedFunctionType(AnsiString S);                     //Ifor 20260402: S9F5 Unrecognized Function Type
        virtual void S9F7_IllegalData(AnsiString S);
        virtual void S9F9_TransactionTimerTimeout(AnsiString S);                      //Ifor 20260402: S9F9 Transaction Timer Timeout
        virtual void S10F4_TerminalDisplaySingleAcknowledge();
        virtual void S10F6_TerminalDisplayMultiBlockAcknowledge();
        virtual void S100F4_ReportAllAlarm();
        virtual void S101F2_CurrentEPPDData();
        virtual void S101F4_CurrentEPPDData();
        virtual void S101F6_StoreHostUploadFile();
        virtual void S101F6();
        virtual void S101F8_StoreHostUploadFile();
        virtual void S101F8();
        virtual void S103F12_StatusVariableNamelistReply();
        virtual void S14F4_Get2DID_BinCode()                                    {};     //Frank 20221122 : 2DID sorting for ATK
        virtual void S110F5_RequestCustomerNameList()                           {};
        //pig 2014.07.04 ASEM SECS GEM start
        virtual void S110F6_ListCustomerName()                                  {};
        virtual void S110F7_RequestReceipeInformation()                         {};
        virtual void S110F8_ListReceipeInformation()                            {};
        virtual void S120F1_RequestReceipeSetupFile()                           {};
        virtual void S120F2_ListReceipeSetupFile()                              {};
        //pig 2014.07.04 ASEM SECS GEM end
        virtual void S125F2_EnableDisableECDataAcknowledge();
        virtual void S125F4_LevelSettingChangeAcknowledge()                     {};    //Steven 20150605 : S125F3 LevelSettingChangeRequest
        virtual void S9F3_Unrecognized_Stream_Function_Type(AnsiString S);             //KenHsieh 20221006 : ASE_K1要求下錯誤S,F Code時回傳S9F3
        TStringList *FMessageList;                                                     //Ifor 20251018 add:Secs Alarm List
};

#endif
