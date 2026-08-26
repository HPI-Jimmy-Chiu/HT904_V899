//---------------------------------------------------------------------------

#ifndef uHGemHT9045H
#define uHGemHT9045H
#include "uHGemClass.h"
#include "UsecegemMainFrom.h"
//---------------------------------------------------------------------------

//typedef vector<unsigned int> MyuVec;
//typedef vector<MyuVec >  MyvVec;

struct ETypeStruct
{
    enum
    {                     //CEID   Report ID Remark
        DoStart=1,        // 1      Start
        DoPause,          // 2      Pause
        DoOneCycle,       // 3      One Cycle
        DoCleanOut,       // 4      Clean Out
        DoClearCount,     // 5      Clear Count
        DoLotStart,       // 6      Lot Start
        DoLot,            // 7      Lot
        DoLotEnd,         // 8      Lot End
        SwitchRunMode,    // 9     ち传 Real / Dummy Mode
        SwitchTesterMode, //10     ち传 Tester Online / Offline Mode
        SwitchProduction, //11     ち传 ネ玻/秸诀家Α
        SwitchEngineer,   //12     ち传 /祘家Α
        SwitchTemperature,//13     ち传 Ambient / Temperature
        SwitchStartMode,  //14     ち传 Start Mode
        SwitchSetupFile,  //15     ち传 郎
        SwitchUser,       //16     ち传 User Level
        EnterTool,        //17     Enter Tool Page
        EnterConfig,      //18     Enter Maintenance Page
        EnterOffset,      //19     Enter Offset Page
        EnterSpeed,       //20     Enter Speed Page
        EnterIO,          //21     Enter I/O Page
        EnterMessage,     //22     Enter Message Page
        EnterDebug,       //23     Enter Debug Page
        DoExit,           //24      Exit
        DoHome,           //25      Home
        GetTestResult,    //26     Get Test Result
        RunStatus,        //27     Change Machine State
        DoRetry,          //28      Retry
        DoSkip,           //29      Skip
        DoAlarmReset,     //30      Alarm Reset
        DoTrayEnd,        //31      Tray End
        DoTrayFeed,       //32      Tray Feed
        DoReset,          //33      Reset
        DoAutoClean,      //34     Auto Clean Start
        Auto1Full,        //35     Auto 1 Full
        Auto2Full,        //36     Auto 2 Full
        Auto3Full,        //37     Auto 3 Full
        Fix1Full,         //38     Fix 1 Full
        Fix2Full,         //39     Fix 2 Full
        Fix3Full,         //40     Fix 3 Full
        OneCycleFinish,   //41     One Cycle Finish
        CleanOutFinish,   //42     Clean Out Finish
        DownloadRecipe,   //43     DownLoad Recipe
        SiteOnOff,        //44     秨闽Site
        ArmOnOff,         //45     秨闽Arm
        SwitchTempData,   //46     Change Temp Default and Soak Time
        SwitchSpeed,      //47     Change Handler Speed
        ChangeEC,         //48     Change EC
        TrayFeedFinish,   //49     Tray Feed Finish
        AutoCleanFinish,  //50     Auto Clean Finish
        SiteMappingStart, //51     Site Mapping Start
        SiteMappingEnd,   //52     Site Mapping End
        UPHRecordStart,   //53     UPH Record Start
        UPHRecordEnd,     //54     UPH Record End
        InitialArtStart,  //55     Initial ART Start
        TesterFT,         //56     Change Tester Program to FT
        TesterRT,         //57     Change Tester Program to RT
        ReadyForArt,      //58    Ready for ART

        ArtReceiveTrayOK,       //59    ART Receive Tray OK
        ArtReceiveTraySTART,    //60    ART Receive Tray START
        ArtRTFinish,            //61    RT Finish
        ArtTrayFeedFinish,      //62    ART Finish
        ArtFTFinish,            //63    FT Finish
        DownLoadRecipeByFTPOK,  //64    DownLoad Recipe by FTP OK
        DownLoadRecipeByFTPNG,  //65    DownLoad Recipe by FTP NG
        LoadTrayFinish,         //66    Load Tray Finish
        TrayTestFinish,         //67    Tray Test Finish
        AutoCleanClearCount,    //68    Auto Clean Clear Count
        SiteMappingStop,        //69    Site Mapping Stop
        BarcodeReaderEnter,     //70    Barcode Reader Enter
        OTDLock,                //71    OTD Lock
        OTDUnLock,              //72    OTD UnLock
        MymessboxOK,            //73     Mymessbox OK
        RemoteProgramClose,     //74
        ChangeTesterPrgToEQC,   //75
        DoStartHasIC,           //76     Start 诀ずΤIC //Ifor 20151118
        ReadCurrentESDData,     //77    Read Current ESD Data (Time event, after setting time will sent report automatically)
        JamSkipICCount,         //78    Jam Skip IC Count
        REVERSED79,             //79
        ReadNowHandlerData,     //80    Read Now Handler Data   //Ifor 20160330 KYEC 璶―Even 
        ReadATCTemperature,     //81    Read ATC Temperature
        ReadATCRefTemperature,  //82    Read ATC Ref Temperature
        ReadNowEPPenconder,     //83    Read Now EP Penconder
        RunStatus_FT,           //84      //wei 20170104
        RunStatus_RT,           //85      //wei 20170104
        MapNoArmHasIC,          //86      //wei 20170609
        MapHasICArmRetry,       //87      //wei 20170609
        MapHasICArmSkip,        //88      //wei 20170609
        PreAlarmMessage,        //89      //Ifor 20171024 : add Pre Alarm Position
        GetTestResultAndBarcode,//90
        SECSOffline,            //91
        SECSOnline,             //92
        SECSOnlineRemote,       //93
        TransferBlocked,         //94
        CassetteLoadComplete,    //95
        CassetteIDReadComplete,  //96
        ReadyToProcessComplete,  //97
        ReadyToCarrierOutLot,    //98
        CassetteOutComplete ,    //99
        CassetteUnclamped,       //100
        ReadyToUnload ,          //101
        UnloadComplete,          //102
        ReadyToCarrierOutTray,   //103
        ReadyToCombinePass,      //104
        ReadyToCombineFail,      //105
        MachineNoStart,          //106
        ReadyToCombinePassLotEnd,//107
        DoCSTLotStart,           //108
        DieCountFailMessageClose,//109
        CleanOutTrayFeedFinish,  //110
        MapNoICArmAutoSkip,      //111
        MRRunModeChange,         //112
        AccessModeChange,        //113
        SoftwareBin,             //114
        TrayIDChange,            //115     //wei 20180130
        ReadyToLoadNoLot,        //116
        ReadyToLoadNoTray,       //117
        ReadyToLoadNoCassette,   //118
        ART_SRQKIND2_FTLOTSTART,    //119
        ART_SRQKIND4_RTLOTSTART,    //120
        ART_SRQKIND8_LOTEND,        //121
        ART_SRQKIND10_FINALLOTEND,  //122
        SafeDoorOnOff,              //123
        SaveRecipe,                 //124
        EESUGOffestSelect,          //125
        EESUGOffestModify,          //126
        Backtonormal,               //127
        TestStart,                  //128
        TestFinish,                 //129
        MaterialReceive,            //130
        SlotMapCountOK,             //131
        CHECK_IN,                   //132
        CHECK_OUT,                  //133
        ReadyToCombineFailLotEnd,   //134
        ReadyToOHTLotEnd,           //135
        Auto1Unloadtray,            //136   //Steven 20210716 : Auto 癶絃ㄆン
        Auto2Unloadtray,            //137   //Steven 20210716 : Auto 癶絃ㄆン
        Auto3Unloadtray,            //138   //Steven 20210716 : Auto 癶絃ㄆン
        DoVisualSortLotStart,       //139   //JerryYang 20220927 : for SPIL Visual sorting Lot start
        PreLoadTray,                //140   //KenHsieh 20220923 : add Tray Map Throw IC Function
        GemControlStateChange,      //141   //Ifor 20221018 add:GEM Control State Change Report
        PickerCountWasCleared,      //142
        UploadPickerCount,          //143
        RequestPickerCount,         //144
        Auto4Unloadtray,            //145   //Steven 20230907 : For HT-9011UC
        Auto5Unloadtray,            //146
        Auto6Unloadtray,            //147
        Auto4Full,                  //148
        Auto5Full,                  //149
        Auto6Full,                  //150
        Fix4Full,                   //151
        Fix5Full,                   //152
        Fix6Full,                   //153
        LoadNoTray,                 //154   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Loader 絃ㄆン
        LoadFullTray,               //155   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Loader 骸絃ㄆン
        LoadOnlyOneTray,            //156   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Loader 逞絃ㄆン
        Loader_ReadyToUnload,       //157   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Loader 癶 TRAY ㄆン(秨﹍)
        Loader_FinishUnload,        //158   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Loader 癶 TRAY ㄆン(挡)
        Empty_PreLoadTray,          //159   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty ㄆン
        EmptyOnlyOneTray,           //160   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 逞絃ㄆン
        EmptyNoTray,                //161   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 絃ㄆン
        EmptyFullTray,              //162   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 骸絃ㄆン
        Color_PreLoadTray,          //163   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color ㄆン
        ColorOnlyOneTray,           //164   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 逞絃ㄆン
        ColorNoTray,                //165   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 絃ㄆン
        Empty_PutTrayToAuto1,       //166   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto1 TRAY ㄆン(秨﹍)
        Empty_PutTrayToAuto2,       //167   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto2 TRAY ㄆン(秨﹍)
        Empty_PutTrayToAuto3,       //168   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto3 TRAY ㄆン(秨﹍)
        Empty_PutTrayToAuto4,       //169   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto1 TRAY ㄆン(秨﹍)
        Empty_PutTrayToAuto5,       //170   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto2 TRAY ㄆン(秨﹍)
        Empty_PutTrayToAuto6,       //171   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto3 TRAY ㄆン(秨﹍)
        Empty_PutCoverToAuto1,      //172   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto1 籠ㄆン
        Empty_PutCoverToAuto2,      //173   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto2 籠ㄆン
        Empty_PutCoverToAuto3,      //174   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto3 籠ㄆン
        Empty_PutCoverToAuto4,      //175   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto1 籠ㄆン
        Empty_PutCoverToAuto5,      //176   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto2 籠ㄆン
        Empty_PutCoverToAuto6,      //177   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 干Auto3 籠ㄆン
        Color_PutTrayToAuto1,       //178   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto1 TRAY ㄆン(秨﹍)
        Color_PutTrayToAuto2,       //179   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto2 TRAY ㄆン(秨﹍)
        Color_PutTrayToAuto3,       //180   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto3 TRAY ㄆン(秨﹍)
        Color_PutTrayToAuto4,       //181   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto1 TRAY ㄆン(秨﹍)
        Color_PutTrayToAuto5,       //182   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto2 TRAY ㄆン(秨﹍)
        Color_PutTrayToAuto6,       //183   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto3 TRAY ㄆン(秨﹍)
        Color_PutCoverToAuto1,      //184   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto1 籠ㄆン
        Color_PutCoverToAuto2,      //185   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto2 籠ㄆン
        Color_PutCoverToAuto3,      //186   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto3 籠ㄆン
        Color_PutCoverToAuto4,      //187   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto1 籠ㄆン
        Color_PutCoverToAuto5,      //188   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto2 籠ㄆン
        Color_PutCoverToAuto6,      //189   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 干Auto3 籠ㄆン
        Auto1_LoadTrayFinish,       //190   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 ﹚TRAYㄆン(挡)
        Auto2_LoadTrayFinish,       //191   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 ﹚TRAYㄆン(挡)
        Auto3_LoadTrayFinish,       //192   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 ﹚TRAYㄆン(挡)
        Auto4_LoadTrayFinish,       //193   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 ﹚TRAYㄆン(挡)
        Auto5_LoadTrayFinish,       //194   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 ﹚TRAYㄆン(挡)
        Auto6_LoadTrayFinish,       //195   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 ﹚TRAYㄆン(挡)
        Auto1_ReadyToUnload,        //196   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 癶TRAYㄆン
        Auto2_ReadyToUnload,        //197   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 癶TRAYㄆン
        Auto3_ReadyToUnload,        //198   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 癶TRAYㄆン
        Auto4_ReadyToUnload,        //199   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 癶TRAYㄆン
        Auto5_ReadyToUnload,        //200   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 癶TRAYㄆン
        Auto6_ReadyToUnload,        //201   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 癶TRAYㄆン
        Auto1NoTray,                //202   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 絃ㄆン
        Auto2NoTray,                //203   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 絃ㄆン
        Auto3NoTray,                //204   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 絃ㄆン
        Auto4NoTray,                //205   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto1 絃ㄆン
        Auto5NoTray,                //206   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto2 絃ㄆン
        Auto6NoTray,                //207   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Auto3 絃ㄆン
        ColorFullTray,              //208   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 骸絃ㄆン
        TrayEndFinish,              //209   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Tray End Finish
        Empty_FinishUnload,         //210   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Empty 癶Tray ㄆン(挡)
        Color_FinishUnload,         //211   //KenHsieh 20230918 : ASEKH_K1 & K3 ㄆン穝糤_Color 癶Tray ㄆン(挡)
        PowerSavingStart,           //212
        PowerSavingEnd,             //213
        Reserved_03 ,               //214
        Reserved_04 ,               //215
        Reserved_05 ,               //216
        LoadPortStatusChanged ,     //217
        EmptyPortStatusChanged ,    //218
        ColorPortStatusChanged ,    //219
        Auto1PortStatusChanged ,    //220
        Auto2PortStatusChanged ,    //221
        Auto3PortStatusChanged ,    //222
        Fix1PortStatusChanged ,     //223
        Fix2PortStatusChanged ,     //224
        Fix3PortStatusChanged ,     //225
        Auto4PortStatusChanged ,    //226
        Auto5PortStatusChanged ,    //227
        Auto6PortStatusChanged ,    //228
        Fix4PortStatusChanged ,     //229
        Fix5PortStatusChanged ,     //230
        Fix6PortStatusChanged ,     //231
        Reserved_21 ,               //232
        Reserved_22 ,               //233
        SafetyDoorOpen,             //234
        SafetyDoorClosed,           //235
        LoadPortBundleArrived,      //236
        LoadPortBundleRead,         //237
        RemoteStart,                //238
        UnexpectedBundleIDRead,     //239
        UnexpectedUNITIDRead,       //240
        BundleCompleteProcessed,    //241
        BundleCompleteIDRead,       //242
        NoCoverTray_2DID,           //243
        NoCoverTray_Normal,         //244
        BundleEnd_Auto1,            //245
        BundleEnd_IDREAD_Auto1,     //246
        BundleEnd_Auto2,            //247
        BundleEnd_IDREAD_Auto2,     //248
        ProcessEnd,                 //249
        DoStartAutoHeight,          //250   //Ifor 20240311 add: Contact Test Start
        BundleEnd_Auto3,            //251
        BundleEnd_IDREAD_Auto3,     //252
        BundleEnd_Auto4,            //253
        BundleEnd_IDREAD_Auto4,     //254
        BundleEnd_Auto5,            //255
        BundleEnd_IDREAD_Auto5,     //256
        BundleEnd_Auto6,            //257
        BundleEnd_IDREAD_Auto6,     //258
        BundleEnd_Fix1,             //259
        BundleEnd_IDREAD_Fix1,      //260
        BundleEnd_Fix2,             //261
        BundleEnd_IDREAD_Fix2,      //262
        BundleEnd_Fix3,             //263
        BundleEnd_IDREAD_Fix3,      //264
        BundleEnd_Fix4,             //265
        BundleEnd_IDREAD_Fix4,      //266
        BundleEnd_Fix5,             //267
        BundleEnd_IDREAD_Fix5,      //268
        BundleEnd_Fix6,             //269
        BundleEnd_IDREAD_Fix6,      //270
        LoaderTrayState,            //271
        AGVSupplement,              //272
        AGVLDUnLDStatus,            //273
        SECSGEMConsecutiveFailure,  //274
        Loader_Buffer_HasTray,      //275       //Sam 20250605 : Loader buffer status change event
        Loader_Buffer_NoTray,       //276
        OutputPort1BinCode,         //277
        OutputPort2BinCode,         //278
        OutputPort3BinCode,         //279
        OutputPort4BinCode,         //280
        OutputPort5BinCode,         //281
        OutputPort6BinCode,         //282
        MaterialModeChange,         //283
        PortStateUpdated,           //284
        UnloaderTrayIDReadOK,       //285
        UnloaderTrayIDReadFail,     //286
        LoaderTrayIDReadFail,       //287
        MaximumOutputPortReport,    //288
        RunCheckRequest,            //289 Eastsun 20260515 F018 Runcheck Request
        AGVLDUnLDFinish,            //290 Eastsun 20260512 F009 KYEC AMR LDUnLD Finish
        AGVLdID,                    //291 Eastsun 20260513 F016 KYEC AMR cover-tray ID readback
        DoSecsGemIndexFail=292,     //292 Eastsun 20260515 F018 KYEC Index Fail event

        TotalEvent                              //﹚璶程
    };
};
extern struct ETypeStruct SECS_EVENT;

// ============================================================
// HCACK 诀絋粄絏 (SEMI E5 S2F42)
// ============================================================
// 0 = Acknowledge, command has been performed          (˙磅︽ЧΘ)
// 1 = Command does not exist                            (ぃ)
// 2 = Cannot perform now                                (砞称ヘ玡篈ぃす砛)
// 3 = At least one parameter is invalid                 (把计ぃ猭)
// 4 = Acknowledge, command will be performed (async)   (钡Μ獶˙磅︽尿パ S6F11 硄)
// 5 = Rejected, already in desired condition            (砞称ヘ夹篈┶荡狡)
// 6 = No such object exists                             (тぃ癸莱ン)
// 7-63 = Reserved
// ============================================================

// 獶˙ RCMD 瑈祘 (ㄑ HCACK=4 獶˙磅︽ㄏノ)
struct TAsyncRCMDState
{
    bool       bPending;       // 琌Τ磅︽獶˙
    AnsiString sCommand;       // 嘿 (糶)
    AnsiString sParam;         // 璶把计 (ㄒ PPID)
    TAsyncRCMDState() : bPending(false), sCommand(""), sParam("") {}
};

class HT9045Gem:public HTGem
{
    private:
        TAsyncRCMDState m_AsyncRCMD;  //Ifor 20260423 add:獶˙ RCMD 篈發萝
    protected:
        AnsiString EventDescription[SECS_EVENT.TotalEvent];
    public:
        HT9045Gem(AnsiString Path, THGem *HGemTmp);
//        virtual ~HT9045Gem()                    ;
        virtual void AddSV()                    ;
        virtual void AddEC()                    ;
        virtual void AddAlarmList()             ;
        virtual void AddCEID()                  ;
        virtual void AddReprot()                ;
        virtual int  S2F15_CheckNewEquipmentConstant(); //wei 20170417 (Steven) add S2F15
        virtual int  S2F15_UpdateNewEquipmentConstant();
        virtual int  S2F42_Host_Command_Acknowledge();
        virtual void S5F6_ListAlarmData();
        virtual int  S7F2_ProcessProgramLoadGrant();
        virtual void S7F4_ProcessProgramAcknowledge();
        virtual void S7F6_ProcessProgramData();
        virtual void S7F6_ProcessProgramData(AnsiString FileName);
        virtual int  ProcessS7F23FromatReceipe() ;
        virtual int  S7F24_FormattedProcessProgramSendAcknowledge();
        virtual int  ProcessS7F25FromatReceipe() ;
        virtual int  S7F26_FormattedProcessProgramData();
        virtual void S14F4_Get2DID_BinCode();
        virtual void S110F5_RequestCustomerNameList();
        virtual void S125F4_LevelSettingChangeAcknowledge();    //Steven 20150605 : S125F3 LevelSettingChangeRequest

        virtual void ReloadParameter();
        virtual void LookForFile()              ;
        void CheckAndExecuteAsyncRCMD();// 獶˙ RCMD 磅︽ㄧΑ (HCACK=4 既诀秈 Idle 牟祇)
};

//==============================================================================
// GEM function Start
//==============================================================================
extern char GEM_MachineName[16];

#endif
