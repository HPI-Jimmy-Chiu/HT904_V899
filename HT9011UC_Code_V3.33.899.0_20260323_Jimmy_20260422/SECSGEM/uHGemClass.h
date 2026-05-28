//---------------------------------------------------------------------------

#ifndef uHGemClassH
#define uHGemClassH
#include "uHGemEquipment.h"
//---------------------------------------------------------------------------

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
        virtual void __fastcall S7F18_DeleteProcessProgramAcknowledge();
        virtual void Process_S7F20_CurrentEPPIDData();
        virtual void S7F20_CurrentEPPDData();
        virtual int S7F24_FormattedProcessProgramSendAcknowledge()              {return 1;};
        virtual int ProcessS7F23FromatReceipe()                                 {return 2;};
        virtual int S7F26_FormattedProcessProgramData()                         {return 1;};
        virtual int ProcessS7F25FromatReceipe()                                 {return 2;};

        virtual void S9F7_IllegalData(AnsiString S);
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
