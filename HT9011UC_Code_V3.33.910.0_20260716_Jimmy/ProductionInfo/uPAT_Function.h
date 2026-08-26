//---------------------------------------------------------------------------

#ifndef uPAT_FunctionH
#define uPAT_FunctionH
//---------------------------------------------------------------------------
#include "FileInfo.h"
#include <inifiles.hpp>
#include <vector>
//---------------------------------------------------------------------------
#define MAX_SITES 16
#define MAX_BINS 15
//---------------------------------------------------------------------------
class PAT_Function                                                              //Jimmychiu 20241110 : add PAT Class
{
private:
    double dStartLotTime;
    double dEndLotTime;
public:     // User declarations
    PAT_Function();
    ~PAT_Function();
    struct PAT_SetUp_Datas
    {
        AnsiString customerCode;
        AnsiString deviceNo;
        AnsiString cLotNo;
        AnsiString lotNo;
        AnsiString accessoryNo1;
        AnsiString testQty;
        AnsiString waferId;
        AnsiString temp;
        AnsiString employeeNo;
        AnsiString testerNo;
        AnsiString entityNo;
        AnsiString step;
        AnsiString programName;
        AnsiString pgmPath;
        AnsiString recipe;
        AnsiString programVer;
        AnsiString prtProgram;
        AnsiString rtProgram;
        AnsiString gdProgram;
        AnsiString eqcProgram;
        AnsiString cshProgram;
        AnsiString batchFileName;
        AnsiString codeName;
        AnsiString codeVersion;
        AnsiString reloadProgram;
        AnsiString writeReadCodeDevice;
        AnsiString eqcCshProgram;
        AnsiString cshBatchFileExecution;
        AnsiString scriptFileName;
        AnsiString scriptFilePath;
        AnsiString notchDirection;
        AnsiString pretestFileName;
        AnsiString retestFileName;
        AnsiString goldenFileName;
        AnsiString eqcFileName;
        AnsiString dutBoardNo;
        AnsiString cfgServerPath;
        AnsiString cfgLocalPath;
        AnsiString cfgProgram;
        AnsiString siteNumber;
        AnsiString waferType;
        AnsiString cpAutoReTest;
        AnsiString notRtBin;
        AnsiString failSiteNumber;
        AnsiString proberCardCurrentTouchDown;
        AnsiString proberCardLimitTouchDown;
        AnsiString curiVersion;
        AnsiString osVersion;
        AnsiString waferVersion;
        AnsiString igExcelVersion;
        AnsiString downloadPath;
        AnsiString accessoryNo2;
        AnsiString accessoryNo3;
        AnsiString accessoryNo4;
        AnsiString accessoryNo5;
        AnsiString s100OIVersion;
        AnsiString step2;
        AnsiString codeName2;
        AnsiString registerBatExecution;
        AnsiString modelFile;
        AnsiString type;
        AnsiString dateCode;
        AnsiString socketNumber;
        AnsiString auxDataFile;
        AnsiString driverPath;
        AnsiString driverSize1;
        AnsiString driverSize2;
        AnsiString driverNameDisplayPath;
        AnsiString cLotNoForSLT;
        AnsiString passBin;
        AnsiString checkSun;
        AnsiString releaseCode;
        AnsiString pgmVersion;
        AnsiString autoZOSProgram;
        AnsiString accessoryNo6;
        AnsiString pgmPath2;
        AnsiString downloadPath2;
        AnsiString autoFixtureCal;
        AnsiString fdEngineMonitor;
        AnsiString ecnNo;
        AnsiString nvtProcess;
        AnsiString nvtFlowStep;
        void Clear()
        {
            customerCode                = "";
            deviceNo                    = "";
            cLotNo                      = "";
            lotNo                       = "";
            accessoryNo1                = "";
            testQty                     = "";
            waferId                     = "";
            temp                        = "";
            employeeNo                  = "";
            testerNo                    = "";
            entityNo                    = "";
            step                        = "";
            programName                 = "";
            pgmPath                     = "";
            recipe                      = "";
            programVer                  = "";
            prtProgram                  = "";
            rtProgram                   = "";
            gdProgram                   = "";
            eqcProgram                  = "";
            cshProgram                  = "";
            batchFileName               = "";
            codeName                    = "";
            codeVersion                 = "";
            reloadProgram               = "";
            writeReadCodeDevice         = "";
            eqcCshProgram               = "";
            cshBatchFileExecution       = "";
            scriptFileName              = "";
            scriptFilePath              = "";
            notchDirection              = "";
            pretestFileName             = "";
            retestFileName              = "";
            goldenFileName              = "";
            eqcFileName                 = "";
            dutBoardNo                  = "";
            cfgServerPath               = "";
            cfgLocalPath                = "";
            cfgProgram                  = "";
            siteNumber                  = "";
            waferType                   = "";
            cpAutoReTest                = "";
            notRtBin                    = "";
            failSiteNumber              = "";
            proberCardCurrentTouchDown  = "";
            proberCardLimitTouchDown    = "";
            curiVersion                 = "";
            osVersion                   = "";
            waferVersion                = "";
            igExcelVersion              = "";
            downloadPath                = "";
            accessoryNo2                = "";
            accessoryNo3                = "";
            accessoryNo4                = "";
            accessoryNo5                = "";
            s100OIVersion               = "";
            step2                       = "";
            codeName2                   = "";
            registerBatExecution        = "";
            modelFile                   = "";
            type                        = "";
            dateCode                    = "";
            socketNumber                = "";
            auxDataFile                 = "";
            driverPath                  = "";
            driverSize1                 = "";
            driverSize2                 = "";
            driverNameDisplayPath       = "";
            cLotNoForSLT                = "";
            passBin                     = "";
            checkSun                    = "";
            releaseCode                 = "";
            pgmVersion                  = "";
            autoZOSProgram              = "";
            accessoryNo6                = "";
            pgmPath2                    = "";
            downloadPath2               = "";
            autoFixtureCal              = "";
            fdEngineMonitor             = "";
            ecnNo                       = "";
            nvtProcess                  = "";
            nvtFlowStep                 = "";
        }
    };
    struct PathConfig
    {
        int iIntervalTime;
        AnsiString sRealTimePath;
        AnsiString sLoadProduction;
        AnsiString sBackupToServer;
        AnsiString sSockLifeTime;
        AnsiString sCleanSocketPath;
        AnsiString sSpecialOriginPath;
        AnsiString sSpecialSaveToPath;
        AnsiString sJobFileUpDownloadPath;
        AnsiString sRealTimeSubTestPath;
        AnsiString sServerJobFilePath;
        AnsiString sSendJamLogFilePath;
        AnsiString sRunCorrelationIniPath;
        AnsiString sConfirmSiteMapIniPath;
        AnsiString sEngJobFileUploadPath;
        AnsiString sRunInfoTesterPath;
        AnsiString sOneCycleReportPath;
        AnsiString sRunInfoBackupPath;
    };
    struct TimeConfig
    {
        AnsiString A;
        AnsiString B;
        AnsiString C;
        AnsiString D;
        AnsiString E;
        AnsiString F;
        AnsiString G;
        AnsiString H;
    };
    struct TypeConfig
    {
        AnsiString JamType1;
        AnsiString JamType2;
        AnsiString JamType3;
        AnsiString JamType4;
        AnsiString JamType5;
    };
    struct TTrayConfig
    {
        double     PitchX;
        double     PitchY;
        double     StartPosX;
        double     StartPosY;
        int        DivisionX;
        int        DivisionY;
        double     DimemsionX;
        double     DimemsionY;
        double     ZThickness;
        AnsiString UserDefineName;
        TTrayConfig()
        {
            Clear();
        }
        void Clear()
        {
            PitchX         = 0.0;
            PitchY         = 0.0;
            StartPosX      = 0.0;
            StartPosY      = 0.0;
            DivisionX      = 0;
            DivisionY      = 0;
            DimemsionX     = 0.0;
            DimemsionY     = 0.0;
            ZThickness     = 0.0;
            UserDefineName = "";
        }
    };
    struct TPlateConfig
    {
        double PitchX;
        double PitchY;
        double StartPositionX;
        double StartPositionY;
        int    DivisionX;
        int    DivisionY;
        double WidthX;
        double WidthY;
        TPlateConfig()
        {
            Clear();
        }
        void Clear()
        {
            PitchX         = 0.0;
            PitchY         = 0.0;
            StartPositionX = 0.0;
            StartPositionY = 0.0;
            DivisionX      = 0;
            DivisionY      = 0;
            WidthX         = 0.0;
            WidthY         = 0.0;
        }
    };
    struct TTemperatureConfig
    {
        int TemptureMode;
        int DefaultValue;
        int SoakTime;
        TTemperatureConfig()
        {
            Clear();
        }
        void Clear()
        {
            TemptureMode = 0;
            DefaultValue = 0;
            SoakTime     = 0;
        }
    };
    struct TContactConfig
    {
        int    TheNoOfPins;
        double gf;
        TContactConfig()
        {
            Clear();
        }
        void Clear()
        {
            TheNoOfPins = 0;
            gf          = 0.0;
        }
    };
    struct TAutoCleanConfig
    {
        int    UseAutoClean;
        int    InitialStart;
        int    InitialRetestStart;
        int    Finish;
        int    Manual;
        int    SocketAlarmFail;
        int    SocketAlarmFailCount;
        int    Interval;
        int    IntervalCount;
        int    ContinuousFail;
        int    ContinuousFailCount;
        int    NumberPices;
        int    Tray;
        int    AlarmCount;
        int    CleanPadDeviation;
        int    ContactMode;
        int    ContactTime;
        int    ContactCount;
        int    PinCount;
        double PinForceN;
        double PinForceGf;
        int    OutArmSpeed;
        int    ShuttleSpeed;
        int    IndexArmSpeed;
        int    InArmSpeed;
        TAutoCleanConfig()
        {
            Clear();
        }
        void Clear()
        {
            UseAutoClean         = 0;
            InitialStart         = 0;
            InitialRetestStart   = 0;
            Finish               = 0;
            Manual               = 0;
            SocketAlarmFail      = 0;
            SocketAlarmFailCount = 0;
            Interval             = 0;
            IntervalCount        = 0;
            ContinuousFail       = 0;
            ContinuousFailCount  = 0;
            NumberPices          = 0;
            Tray                 = 0;
            AlarmCount           = 0;
            CleanPadDeviation    = 0;
            ContactMode          = 0;
            ContactTime          = 0;
            ContactCount         = 0;
            PinCount             = 0;
            PinForceN            = 0.0;
            PinForceGf           = 0.0;
            OutArmSpeed          = 0;
            ShuttleSpeed         = 0;
            IndexArmSpeed        = 0;
            InArmSpeed           = 0;
        }
    };
    struct TTesterConfig
    {
        int TestMode;
        int Rs232_Interface;
        int AntiSignal;
        int Gpib_Interface;
        int GpibAddress;
        TTesterConfig()
        {
            Clear();
        }
        void Clear()
        {
            TestMode = 0;
            Rs232_Interface = 0;
            AntiSignal = 0;
            Gpib_Interface = 0;
            GpibAddress = 0;
        }
    };
    struct TVacuumConfig
    {
        double Arm1VacuumWait;
        double IndexVacuumWait;
        double OutVacuumWait;
        double TrayVacuumWait;
        double Arm1DestroyWait;
        double IndexDestroyWait;
        double OutDestroyWait;
        double TrayDestroyWait;
        TVacuumConfig()
        {
            Clear();
        }
        void Clear()
        {
            Arm1VacuumWait = 0.0;
            IndexVacuumWait = 0.0;
            OutVacuumWait = 0.0;
            TrayVacuumWait = 0.0;
            Arm1DestroyWait = 0.0;
            IndexDestroyWait = 0.0;
            OutDestroyWait = 0.0;
            TrayDestroyWait = 0.0;
        }
    };
    struct TSpeedConfig
    {
        int InArm2Speed;
        int ShuttleSpeed;
        int IndexSpeed;
        int OutArmSpeed;
        int CatchTraySpeed;
        int TrayLoaderSpeed;
        TSpeedConfig()
        {
            Clear();
        }
        void Clear()
        {
            InArm2Speed    = 0;
            ShuttleSpeed   = 0;
            IndexSpeed     = 0;
            OutArmSpeed    = 0;
            CatchTraySpeed = 0;
            TrayLoaderSpeed= 0;
        }
    };
    struct TAlarm3Config
    {
        int OpenFunction;
        int ViewMode;
        int SiteABinContinuouCount;
        int SiteBBinContinuouCount;
        int SiteCBinContinuouCount;
        int SiteDBinContinuouCount;
        int SiteEBinContinuouCount;
        int SiteFBinContinuouCount;
        int SiteGBinContinuouCount;
        int SiteHBinContinuouCount;
        TAlarm3Config()
        {
            Clear();
        }
        void Clear()
        {
            OpenFunction              = 0;
            ViewMode                  = 0;
            SiteABinContinuouCount    = 0;
            SiteBBinContinuouCount    = 0;
            SiteCBinContinuouCount    = 0;
            SiteDBinContinuouCount    = 0;
            SiteEBinContinuouCount    = 0;
            SiteFBinContinuouCount    = 0;
            SiteGBinContinuouCount    = 0;
            SiteHBinContinuouCount    = 0;
        }
    };
    PAT_SetUp_Datas patSetup;
    PathConfig pathConf;
    TimeConfig timeConf;
    TypeConfig typeConf;
    TTrayConfig trayconfig;
    TPlateConfig plateconfig;
    TTemperatureConfig tempconfig;
    TContactConfig contactconfig;
    TAutoCleanConfig autocleanconfig;
    TTesterConfig testerconfig;
    TVacuumConfig vacuumconfig;
    TSpeedConfig speedConfig;
    TAlarm3Config alarm3Config;
    //
    struct TOutputCount
    {
        int iAuto1;
        int iAuto2;
        int iAuto3;
        int iFix1;
        int iFix2;
        int iFix3;
        int iTotal;
        TOutputCount()
        {
            Clear();
        }
        void Clear()
        {
            iAuto1   = 0;
            iAuto2   = 0;
            iAuto3   = 0;
            iFix1    = 0;
            iFix2    = 0;
            iFix3    = 0;
            iTotal   = 0;
        }
        void CalculateTotal()
        {
            iTotal = iAuto1 + iAuto2 + iAuto3 + iFix1 + iFix2 + iFix3;
        }
    };
    TOutputCount toutputCnt;
    //
    struct TTrayData
    {
        int Bins[MAX_BINS];
        int Total;
        TTrayData()
        {
            Clear();
        }
        void CalculateTotal()
        {
            Total = 0;
            for (int i = 0; i < MAX_BINS; i++)
            {
                Total += Bins[i];
            }
        }
        void Clear()
        {
            for (int i = 0; i < MAX_BINS; i++)
            {
                Bins[i] = 0;
            }
            Total = 0;
        }
    };
    TTrayData Tray[MAX_SITES];
    int iTotalSites;
    //
    AnsiString sPATFtpPwd;
    AnsiString sPATMode;
    int iPATModeInd;
    enum ReportType
    {
        REPORT_REALTIME,
        REPORT_HOURLY,
        REPORT_END_LOT
    };
    enum eLastSetBinCTArea
    {
        elsbAuto1=0,
        elsbAuto2,
        elsbAuto3,
        elsbFix1,
        elsbFix2,
        elsbFix3,
        elsbTotal,
    };
    std::vector<AnsiString> JamLogs;
    void DoIniDataToForm();
    void DoIniDatas();
    bool AnalyzePATSetUpText(const AnsiString& text);
    bool LoadSetupFile(const AnsiString FilePath);
    bool SaveServerINI(const AnsiString FilePath);
    bool LoadServerINI(const AnsiString FilePath);
    void SetPathConfig(TIniFile* iniFile,const PathConfig &pathConfig);
    bool GetPathConfig(TIniFile* iniFile,PathConfig &pathConfig);
    void SetTimeConfig(TIniFile* iniFile,const TimeConfig &timeConfig);
    bool GetTimeConfig(TIniFile* iniFile,TimeConfig &timeConfig);
    void SetTypeConfig(TIniFile* iniFile,const TypeConfig &typeConfig);
    bool GetTypeConfig(TIniFile* iniFile,TypeConfig &typeConfig);
    void SetPasswordConfig(TIniFile* iniFile,const AnsiString &ftppwd);
    bool GetPasswordConfig(TIniFile* iniFile,AnsiString &ftppwd);
    AnsiString ReadINIString(TIniFile* iniFile,const AnsiString& section, const AnsiString& key, const AnsiString& defaultValue = "");
    void WriteINIString(TIniFile* iniFile,const AnsiString& section, const AnsiString& key, const AnsiString& value);
    AnsiString GetRealTimeReport(const int &iReportType);
    AnsiString GetRealTimeRpt_FormatRow(const AnsiString& sLabel, const AnsiString& sValue);
    AnsiString GetRealTimeRpt_OutCnt(const AnsiString& sLabel, const AnsiString& sNum, const AnsiString& sPercent);
    AnsiString GetDoubleToStr(double dValue);
    //<==for report
    int GetTotalContactCount();
    void GetCTbyChannel(int iCh,int iRetDatas[16]);
    void ClearRptStruct();
    AnsiString GetAverageTestTime();
    AnsiString GetAveragePauseTime();
    AnsiString GetOffSiteNumber();
    AnsiString GetJamRate();
    AnsiString GetIndexCycleTime();
    AnsiString GetOutputCount();
    AnsiString GetOutputTrayBinSummer();
    AnsiString GetOutputSiteBinSummer();
    AnsiString GetOutputSiteBinYieldSummer();
    AnsiString GetBasicData(const int &iReportType);
    AnsiString GetTimeFomate(const TDateTime &datetime);
    void SetStartLotTime(const double &dValue);
    void SetEndLotTime(const double &dValue);
    AnsiString GetStartLotTimeStr();
    TDateTime GetEndLotTime(const int &iReportType);
    AnsiString GetEndLotTimeStr(const int &iReportType);
    AnsiString GetTestTimeStr(const int &iReportType);
    void GenerateRealTimeReport();
    void GenerateRealTimeReport(const AnsiString &sPath);
    void GenerateHourlyReport();
    void GenerateHourlyReport(const AnsiString &sPath);
    void GenerateEndLotReport();
    void GenerateEndLotReport(const AnsiString &sPath);
    AnsiString GetPATServerINI();
    AnsiString GetPATJobFileDownload();
    AnsiString GetPATJobFileUpload();
    AnsiString GetPATSetUpFile();
    AnsiString GetDayHourMinSecStr(const double &dtTime);                       //for TDatetime
    bool CheckMachineStationOnStart();
    AnsiString GetClosedSitesName();
    AnsiString GetProgramName(int iTestIdx);
    AnsiString GetSoakTime();
    AnsiString GetJamLogList();
    //
    void AnalysisTempAndTolerance(const AnsiString &str,int &iTemp,int &iTolerance);
    bool SaveJobINI(const AnsiString FilePath);
    bool LoadJobINI(const AnsiString FilePath);
    //
    void SetJobTrayConfig(TIniFile* iniFile, const TTrayConfig &tray);
    bool GetJobTrayConfig(TIniFile* iniFile, TTrayConfig &tray);
    void SetJobTrayToSetting(TTrayConfig &tray);
    void GetJobTrayToSetting(const TTrayConfig &tray);
    //
    void SetJobPlateConfig(TIniFile* iniFile, const TPlateConfig &plate);
    bool GetJobPlateConfig(TIniFile* iniFile, TPlateConfig &plate);
    void SetJobPlateToSetting(TPlateConfig &plate);
    void GetJobPlateToSetting(const TPlateConfig &plate);
    //
    void SetJobTemperatureConfig(TIniFile* iniFile, const TTemperatureConfig &temp);
    bool GetJobTemperatureConfig(TIniFile* iniFile, TTemperatureConfig &temp);
    void SetJobTemperatureToSetting(TTemperatureConfig &temp);
    void GetJobTemperatureToSetting(const TTemperatureConfig &temp);
    //
    void SetJobContactConfig(TIniFile* iniFile, const TContactConfig &contact);
    bool GetJobContactConfig(TIniFile* iniFile, TContactConfig &contact);
    void SetJobContactToSetting(TContactConfig &contact);
    void GetJobContactToSetting(const TContactConfig &contact);
    //
    void SetJobAutoCleanConfig(TIniFile* iniFile, const TAutoCleanConfig &autoClean);
    bool GetJobAutoCleanConfig(TIniFile* iniFile, TAutoCleanConfig &autoClean);
    void SetJobAutoCleanToSetting(TAutoCleanConfig &autoClean);
    void GetJobAutoCleanToSetting(const TAutoCleanConfig &autoClean);
    //
    void SetJobTesterConfig(TIniFile* iniFile, const TTesterConfig &tester);
    bool GetJobTesterConfig(TIniFile* iniFile, TTesterConfig &tester);
    void SetJobTesterToSetting(TTesterConfig &tester);
    void GetJobTesterToSetting(const TTesterConfig &tester);
    //
    void SetJobVacuumConfig(TIniFile* iniFile, const TVacuumConfig &vacuum);
    bool GetJobVacuumConfig(TIniFile* iniFile, TVacuumConfig &vacuum);
    void SetJobVacuumToSetting(TVacuumConfig &vacuum);
    void GetJobVacuumToSetting(const TVacuumConfig &vacuum);
    //
    void SetJobSpeedConfig(TIniFile* iniFile, const TSpeedConfig &speed);
    bool GetJobSpeedConfig(TIniFile* iniFile, TSpeedConfig &speed);
    void SetJobSpeedToSetting(TSpeedConfig &speed);
    void GetJobSpeedToSetting(const TSpeedConfig &speed);
    //
    void SetJobAlarm3Config(TIniFile* iniFile, const TAlarm3Config &alarm3);
    bool GetJobAlarm3Config(TIniFile* iniFile, TAlarm3Config &alarm3);
    void SetJobAlarm3ToSetting(TAlarm3Config &alarm3);
    void GetJobAlarm3ToSetting(const TAlarm3Config &alarm3);
    //
    AnsiString GetPAT_Folder();
    AnsiString GetJobName();
    AnsiString GetDoublePercentStr(const double &dMolecular,const double &dDenominator);
    void SetJamLog(const AnsiString& sJamArea,const AnsiString& sMsg);
    //
    int GetAreaByBin(int iBin);//Bin1=1 ; Bin2=2
    int GetBinByArea(int iArea);//Bin1=1 ; Bin2=2
    TDateTime SecondsToDateTime(int seconds);
};
//---------------------------------------------------------------------------
class TMachineTimeManager
{
private:
    TDateTime startTime;       // Lot 開始時間戳
    TDateTime lastSwitchTime;  // 上次狀態切換時間戳
    TDateTime totalRunning;    // 總運作時間 (TDateTime)
    TDateTime totalPause;      // 總暫停時間 (TDateTime)
    TDateTime totalPauseBeforeTest;      // 總暫停時間 (TDateTime)
    MachineState state;           // 當前狀態
    void UpdateTime();            // 內部方法：更新時間累加
public:
    TMachineTimeManager();
    ~TMachineTimeManager();

    void StartLot();                     // 開始 Lot，重置並進入運作狀態
    void Pause();                        // 暫停，記錄運作時間
    void Resume();                       // 恢復運作，記錄暫停時間
    void EndLot();                       // 結束 Lot，記錄最後時間
    void ReadEdtListIni();
    void WriteEdtListIni();
    AnsiString GetFileName(){return AnsiString("MachineTimeManager.ini");}
    void InitEdtList();
    double GetTotalRunningTime() const;  // 獲取總運作時間 (秒)
    double GetTotalPauseTime() const;    // 獲取總暫停時間 (秒)
    double GetTotalPauseTimeBeforeTest() const;    // 獲取總暫停時間 (秒)
};
//---------------------------------------------------------------------------
#endif
