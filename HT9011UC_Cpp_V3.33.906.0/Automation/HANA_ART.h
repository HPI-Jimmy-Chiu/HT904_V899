//---------------------------------------------------------------------------
//  Automation/HANA_ART.h  --  HANA-ART tester-side SRQ protocol helper
//
//  Translation wave: W5 Wave 1 (parallel translate batch)
//  Translator: AI(W5-HanaART-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/HANA_ART.h
//
//  uHANA_ART implements the HANA-ART SRQ handshake (FT/RT prime+run, LOT
//  start/end, tray-map upload bookkeeping) that main.h's `uHANA_ART*
//  hanaART` member points at (golden main.h:1696, `hanaART=new uHANA_ART();`
//  main.cpp:1500).  This header is a near-verbatim structural mirror of the
//  golden header: only `#include <StdCtrls.hpp>` -> vclcompat, and
//  `__fastcall` (none present in this header) would be neutralized.
//
//  See HANA_ART.cpp's file-header comment for the full list of gated
//  (not-yet-available) cross-module leaves and the FormsFacade / substrate
//  additions this unit needs from the integrate step.
//---------------------------------------------------------------------------
#ifndef HANA_ARTH
#define HANA_ARTH

#include "vclcompat/vcl_compat.h"   // AnsiString (was: #include <StdCtrls.hpp>)

//---------------------------------------------------------------------------
#define TDATAMaxCount 9
#define CDATAMaxCount 9
#define SDATAMaxCount 6
#define JDATAMaxCount 8
//---------------------------------------------------------------------------
class uHANA_ART                                                                //JimmyChiu 20241023 HANA ART Function
{
private:    // User declarations
    //SRQ define
    struct TimeData
    {
        // Handler Time Data (TDATA)
        int TDATA[TDATAMaxCount];
        enum TD_Time
        {
            TD_LOT_START_TO_END=0,      // Time from LOT start to LOT end
            TD_TEST_MODE_TO_END,        // Time from TEST MODE start to MODE END
            TD_HANDLER_STOP_DURATION,   // Time when the HANDLER was stopped between LOT start and LOT END
            TD_RESET_AFTER_JAM_1,       // Time to press RESET after JAM (instant stop wait time 1)
            TD_RESET_AFTER_JAM_2,       // Time to press RESET after non-TD4 JAM (instant stop wait time 2)
            TD_JAM_TO_RESTART,          // Time from JAM to restart (maintenance or action time)
            TD_HANDLER_INDEX_TIME,      // Total HANDLER INDEX TIME
            TD_TOTAL_TEST_TIME,         // Total TEST TIME
            TD_FIRST_SOT_DURATION       // Duration of first SOT after LOT Start
        };
        // HWBIN COUNT (CDATA)
        //int CDATA[CDATAMaxCount];//0 = ERR BIN COUNT , 1 = HBIN1 COUNT ...

        // Contact Tool Motor Z Position & Contact method (SDATA)
        double SDATA[SDATAMaxCount];
        enum P_Parameters
        {
            P_FRONT_CONTACTOR_Z=0,   // Front Contactor Z Position
            P_BACK_CONTACTOR_Z,      // Back Contactor Z Position
            P_HANDLER_MODE,          // Handler mode
            P_CONTACT_FORCE,         // Contact Force
            P_FORCE_PER_DEVICE,      // Force per device
            P_FORCE_PER_PIN          // Input Force per Pin
        };

        // JAM Count by Position (JDATA)
        int JDATA[JDATAMaxCount];
        enum J_Parameters
        {
            J_INPUT_PICKER_JAM=0,          // Input Picker Jam Count
            J_OUTPUT_PICKER_JAM,           // Output Picker Jam Count
            J_FRONT_CONTACTOR_JAM,         // Front Contactor Jam Count
            J_REAR_CONTACTOR_JAM,          // Rear Contactor Jam Count
            J_INPUT_SHUTTLE_JAM_FRONT,     // Input Shuttle Jam Count (Front)
            J_INPUT_SHUTTLE_JAM_BACK,      // Input Shuttle Jam Count (Back)
            J_OUT_SHUTTLE_JAM_FRONT,       // Out Shuttle Jam Count (Front)
            J_OUT_SHUTTLE_JAM_BACK,        // Out Shuttle Jam Count (Back)
            J_TRAY_TRANSFER_JAM,           // Tray Transfer Jam Count
            J_JAM_COUNT_EXCLUDING_FACILITY,// Jam Count excluding facility
            J_INPUT_MOTOR_ERROR,           // Input Motor error
            J_SHUTTLE_MOTOR_ERROR,         // Shuttle Motor error
            J_CONTACTOR_MOTOR_ERROR,       // Contactor Motor error
            J_OUTPUT_MOTOR_ERROR           // Output Motor error
        };

        // Generated JAM NO or Code (ADATA)
        AnsiString ADATA; // Store JAM code, e.g., "ZZ" if no jam
        //Function
        void Clear()
        {
            ZeroMemory(TDATA,      sizeof(TDATA));
//            ZeroMemory(CDATA,      sizeof(CDATA));
            ZeroMemory(SDATA,      sizeof(SDATA));
            ZeroMemory(JDATA,      sizeof(JDATA));
            ADATA="";
        }
    };

    struct HandlerWaitingData
    {
        AnsiString Model;    //HT9046
        AnsiString Model_NO; //HT9046-01
        AnsiString Site;     //HANA
        int HD_Mode;         //0=GENERAL 1=SMILL
        bool bHD_MODE_STATUS;//false:HDMODENG  true:HDMODEOK
        void Clear()
        {
            Model="";
            Model_NO="";
            Site="";
            bHD_MODE_STATUS=false;
        }
        //Run mode
        enum ModeType
        {
            modeGeneral=0,
            modeSMILL,
        };
    };

    struct HandlerStartingData
    {
        int iStatus;
        int iFT_RT_Mode;//0=HALT 1=FT 2=RT
        bool bEnableContact_FT;
        bool bEnableContact_RT;
        bool bDUMMY_TEST_0x42_OK;
        int  iNeedToRT;
        void Clear()
        {
            iStatus=0;
            iFT_RT_Mode=frtHALT;
            bEnableContact_FT=false;
            bEnableContact_RT=false;
            bDUMMY_TEST_0x42_OK=false;
            iNeedToRT=0;
        }
        enum FTRTType
        {
            frtHALT=0,
            frtFT,
            frtFT_LotEnd,
            frtRT,
        };
    };

    struct TesterData
    {
        int iStatus;
        void Clear()
        {
            iStatus=0;
        }
        enum eTestStatus
        {
            etsHALT=0,
            etsSetFT,
            etsSetFT_SMILL,
            etsPrimeFT,
            etsSetRT,
            etsPrimeRT,
            etsTotalStatus,
        };
    };
protected:
    TimeData timeData;
    HandlerWaitingData HdWaitData;
    HandlerStartingData HdStartData;
    TesterData TestData;
public:     // User declarations
    uHANA_ART();
    ~uHANA_ART();
    //Time Data Define
    enum SRQCode
    {
        SETUP_INFORM_REQUEST_SRQ0x55        = 0x55,
        SETUP_INFORM_RECEIVE_OK_SRQ0x56     = 0x56,
        SETUP_INFORM_RECEIVE_FAIL_SRQ0x57   = 0x57,
        LOT_START_SRQ0x63                   = 0x63,
        LOTON_READ_SUCCESS_SRQ0x51          = 0x51,
        LOTON_READ_FAIL_SRQ0x52             = 0x52,
        LOT_END_SRQ0x64                     = 0x64,
        STANDBY_TESTMODE_SRQ0x53            = 0x53,
        PRIME_START_SRQ0x50                 = 0x50,
        PRIME_END_SRQ0x54                   = 0x54,
        NORMAL_START_SRQ0x41                = 0x41,
        RETEST_START_SRQ0x65                = 0x65,
        RETEST_END_SRQ0x66                  = 0x66,
        DUMMYTEST_START_SRQ0x42             = 0x42,
        ERROR_START_SRQ0x67                 = 0x67,
        ERROR_END_SRQ0x68                   = 0x68,
        ERROR_CLEAR_SRQ0x69                 = 0x69,
        AUTO_START_SRQ0x40                  = 0x40,
        INIT_START_SRQ0x62                  = 0x62,
        LOADER_REQ_SRQ0x61                  = 0x61,
        LOT_INFORM_SRQ0x58                  = 0x58,
        FQATEST_START_SRQ0x71               = 0x71,
        FQATEST_END_SRQ0x72                 = 0x72,
        WPSOCKET_REQ_SRQ0x73                = 0x73,
        FBIN_MANUAL_REQ_SRQ0x74             = 0x74,
        REAL_PARA_SEQ_SRQ0x75               = 0x75,
        LOT_INFO_METHOD_SRQ0x76             = 0x76,
        RF_ID_SRQ0x77                       = 0x77,
        SBL_SRQ0x78                         = 0x78,
        AUTODUMPING_SRQ0x79                 = 0x79,
        EQP_MODEL_SRQ0x43                   = 0x43,
        HD_MODE_SRQ0x59                     = 0x59,
        INLINEUPDATE_SRQ0x60                = 0x60,
        CLEAR_START_SRQ0x00                 = 0x0
    };
    //parameter
    bool bSimuTester;
    //Fucntion
    void Clear();
    void SetHandlerWaitingData(AnsiString Model,AnsiString Model_NO, AnsiString Site, int HD_Mode);
    void DoCmdWhenHDStart(AnsiString sTestCmd);
    void DoRunHanaART(bool bRun);
    void SendHanaSRQInt(int iCmd);
    void SendHanaCommand(AnsiString sCmd);
    AnsiString GetHanaSRQStr(unsigned int srq);
//    void ShowErrMessage(AnsiString sMsg);
    void DoSETUP_INFORM();
//    void DoPRIME_END();
    void DoRT_START();
    AnsiString GetNow_FTRT_Mode();
    AnsiString GetHD_ID();
    AnsiString GetHD_MAP();
    AnsiString GetHD_CT();
    AnsiString GetHD_Tempset();
    AnsiString GetHD_Soak();
    AnsiString GetHD_TDATA();
    AnsiString GetHD_CDATA();
    AnsiString GetHD_SDATA();
    AnsiString GetHD_JDATA();
    AnsiString GetHD_ADATA();
    AnsiString LotEndTime;
    bool DoSetUpInfo(AnsiString sCmd);
    //AI(W5-HanaART-Translate) 20260710: golden gap preserved verbatim -- handleError
    //is declared here (golden HANA_ART.h) but has NO body anywhere in golden
    //HANA_ART.cpp and is never called; faithfully left undefined (declared only).
    void handleError(int errorCode);
    void StartPrimeTest();
    void EndPrimeTest();
    void StartReTest();
    void EndReTest();
    void EndOfLot();
    void TestDataNextStep();
    bool IsHanaArtAvailable();
    bool IsContactAvailable();
    bool IsPrimeTest();
    int  NeedToRT();
    bool ParseLOTONStr(const AnsiString input, AnsiString& sLotNo, int& LotSize, AnsiString& sMode);
    void UpdateFileNameList(int iUpdateStatus);
    void AddNewTrayHead(int iAuto);
    int SendTrayMapToFTP();
    void RecoveryARTStatus();
//    void GenerateTrayMapRpt();
};
#endif
