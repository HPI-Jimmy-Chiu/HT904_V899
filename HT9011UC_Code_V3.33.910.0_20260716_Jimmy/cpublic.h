#ifndef cpublicH
#define cpublicH

//------------------------------------------------------------------------------
void CutSpaceAtHead(char *S);
char *ConvertSecondToTime(long s);
void GetEveryCode(AnsiString AnsiData);                                         //Steven 20111028 : 改成AnsiString
int Change_Tempture_Value();
void UT100WordWriteNoSucm(int Addr, int Command, int Value);
void UT100WordReadNoSucm(int Addr, int Command);
AnsiString DTK4848_LRC(AnsiString str);                                         //KaiHuang 20190821 : 新增台達 DTK4848溫控器
void DTK4848WordWriteNoSucm(int Addr,int Value);                                //KaiHuang 20190821 : 新增台達 DTK4848溫控器
void DTK4848WordReadNoSucm(int Addr);                                           //KaiHuang 20190821 : 新增台達 DTK4848溫控器
void ConvertGearValue(int *Ref, double GearRatio);
char *GetFloatFormatString(double v, int P1, int P2);
char *ConvertToMMType(int i);
int Get0_01MMType(char *str);
char *ConvertTouMType(int i);
int ConvertDoubleStrToInt(AnsiString Str, double dDigit);
double ConvertIntStrToDouble(AnsiString Str, double dDigit);
void GetTimeInfo();
void GetYesterdayInfo();
bool SplitStrByDotSpaceOnly(char *str, char *dest, int Max);
int GetScreenPos(int NowPos, int ScreenTop, int ScreenBottom, int RealTop, int RealBottom);
AnsiString ConvertSecondToSPC(long s);                                          //Steven 20141111 : 秒轉為時分秒
AnsiString ConvertMSecToSPC(long s);                                            //Steven 20190714 : MS時間轉換成時分秒
AnsiString ConvertMSecToTime(long s);                                           //Steven 20190714 : MS時間轉換成日時分秒
bool IsDoubleEqual(const double a, const double b, const double tolerance=1e-6);
//------------------------------------------------------------------------------

void TMC401WriteTemp(int Addr, int CH, int Temp);
void TMC401ReadTemp(int Addr, int CH);

void E5DCReadTemp(int Addr);                                                    //Frank20141030
void E5DCWriteTemp(int Addr, int Temp);

void OutShuttleLog(bool bFlag=false);                                           //Steven 20110531
void HomeLog(AnsiString Message);                                               //Kevin  20110525
bool RespondASECom(AnsiString S1);                                              //kevin 20150415
void TTLLog(AnsiString Message);                                                //Steven 20151123 : Log for TTL
void HeaterLog(AnsiString Message, bool bOnOff);                                //Steven 20151123 : Log for Heater Relay
void HeaterSVLog(int Addr, double dTemp);                                       //Steven 20200121 : Log for Heater SV
void ProductionLog(AnsiString Message, bool bSaveToFile=false, AnsiString JamCode="");//JerryYang 20151225 Production Log for SPIL蘇州

bool ExecZipCommand(AnsiString Path, AnsiString Param);                         //Steven 20160205 : 存檔時候不要跳DOS視窗
HANDLE ExecZipCommandHandle(AnsiString Path, AnsiString Param);//AI(ht9045-v899) 20260630: 同 ExecZipCommand 但回傳 process handle 供非阻塞輪詢(State Record 確認 1.bat 複製完才壓縮/刪資料夾)

AnsiString GetDateInfoByString(AnsiString asSign="");                           //ChungHung 20151125 modify for KYEC //ChungHung 20150902 add
AnsiString GetOnlyTimeInfoByString(AnsiString asSign="");                       //ChungHung 20151125 modify for KYEC  //ChungHung 20150902 add
void ProductionDataLog();                                                       // kevin 20160724生產資料

int CompareMD5ByFolder(AnsiString FolderName);                                  //Steven 20170927 (wei) : 比對工作檔的檢查碼是否正確 -1:沒有MD5,  1:Pass, 0:Fail
void SetMD5ByFolder(AnsiString FolderName);                                     //Steven 20170927 (wei) : 將工作檔加入檢查碼
bool b_Check_Dir_Exist_And_Creak_Dir(AnsiString asDir);                         //函示說明:檢查本機目錄是否存在，並且重新建立
AnsiString GetSoftwareFileVersion(AnsiString sFilePaych);                       //Sam 20230328 : 自動更新增加版本檢查
double VC8ToKpa(int iVal);                                                      //Sam 20230210 : 新增 VacuumUnit 通訊模組
int KpaToVC8(double dKpa);                                                      //Sam 20230210 : 新增 VacuumUnit 通訊模組
//------------------------------------------------------------------------------
union ByteUnionBit
{
    struct Bit_dev
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    }bit;
    unsigned char byte;

    //Function-----------------
    int Bit(int i);
    int BitCount(bool bOn);                                                     //判斷有幾個是1
    void SetBit(int i, bool Value);
    bool SetByte(int Value);
    AnsiString BitString();
    AnsiString Hex(bool bWith0x=false);
    //Function-----------------
};
//------------------------------------------------------------------------------
union IntUnionBit                                                               //Sam 20221012 : 新增 IntUnionBit
{
    struct Bit_dev
    {
        unsigned int b0:1;
        unsigned int b1:1;
        unsigned int b2:1;
        unsigned int b3:1;
        unsigned int b4:1;
        unsigned int b5:1;
        unsigned int b6:1;
        unsigned int b7:1;
        unsigned int b8:1;
        unsigned int b9:1;
        unsigned int b10:1;
        unsigned int b11:1;
        unsigned int b12:1;
        unsigned int b13:1;
        unsigned int b14:1;
        unsigned int b15:1;
        unsigned int b16:1;
        unsigned int b17:1;
        unsigned int b18:1;
        unsigned int b19:1;
        unsigned int b20:1;
        unsigned int b21:1;
        unsigned int b22:1;
        unsigned int b23:1;
        unsigned int b24:1;
        unsigned int b25:1;
        unsigned int b26:1;
        unsigned int b27:1;
        unsigned int b28:1;
        unsigned int b29:1;
        unsigned int b30:1;
        unsigned int b31:1;
    }bit;
    unsigned int UnsignInt;

    //Function-----------------
    bool Bit(int i)
    {
        if(i==0)        return (bit.b0)?true:false;
        else if(i==1)   return (bit.b1)?true:false;
        else if(i==2)   return (bit.b2)?true:false;
        else if(i==3)   return (bit.b3)?true:false;
        else if(i==4)   return (bit.b4)?true:false;
        else if(i==5)   return (bit.b5)?true:false;
        else if(i==6)   return (bit.b6)?true:false;
        else if(i==7)   return (bit.b7)?true:false;
        else if(i==8)   return (bit.b8)?true:false;
        else if(i==9)   return (bit.b9)?true:false;
        else if(i==10)  return (bit.b10)?true:false;
        else if(i==11)  return (bit.b11)?true:false;
        else if(i==12)  return (bit.b12)?true:false;
        else if(i==13)  return (bit.b13)?true:false;
        else if(i==14)  return (bit.b14)?true:false;
        else if(i==15)  return (bit.b15)?true:false;
        else if(i==16)  return (bit.b16)?true:false;
        else if(i==17)  return (bit.b17)?true:false;
        else if(i==18)  return (bit.b18)?true:false;
        else if(i==19)  return (bit.b19)?true:false;
        else if(i==20)  return (bit.b20)?true:false;
        else if(i==21)  return (bit.b21)?true:false;
        else if(i==22)  return (bit.b22)?true:false;
        else if(i==23)  return (bit.b23)?true:false;
        else if(i==24)  return (bit.b24)?true:false;
        else if(i==25)  return (bit.b25)?true:false;
        else if(i==26)  return (bit.b26)?true:false;
        else if(i==27)  return (bit.b27)?true:false;
        else if(i==28)  return (bit.b28)?true:false;
        else if(i==29)  return (bit.b29)?true:false;
        else if(i==30)  return (bit.b30)?true:false;
        else if(i==31)  return (bit.b31)?true:false;
        else            return false;
    }

    int BitCount(bool bOn)                                                      //判斷有幾個是1
    {
        int iCount=0;
        for(int i=0; i<32; i++)
        {
            if(Bit(i)==bOn)
                iCount++;
        }
        return iCount;
    }

    void SetBit(int i, bool Value)
    {
        if(i==0)        bit.b0=Value?1:0;
        else if(i==1)   bit.b1=Value?1:0;
        else if(i==2)   bit.b2=Value?1:0;
        else if(i==3)   bit.b3=Value?1:0;
        else if(i==4)   bit.b4=Value?1:0;
        else if(i==5)   bit.b5=Value?1:0;
        else if(i==6)   bit.b6=Value?1:0;
        else if(i==7)   bit.b7=Value?1:0;
        else if(i==8)   bit.b8=Value?1:0;
        else if(i==9)   bit.b9=Value?1:0;
        else if(i==10)  bit.b10=Value?1:0;
        else if(i==11)  bit.b11=Value?1:0;
        else if(i==12)  bit.b12=Value?1:0;
        else if(i==13)  bit.b13=Value?1:0;
        else if(i==14)  bit.b14=Value?1:0;
        else if(i==15)  bit.b15=Value?1:0;
        else if(i==16)  bit.b16=Value?1:0;
        else if(i==17)  bit.b17=Value?1:0;
        else if(i==18)  bit.b18=Value?1:0;
        else if(i==19)  bit.b19=Value?1:0;
        else if(i==20)  bit.b20=Value?1:0;
        else if(i==21)  bit.b21=Value?1:0;
        else if(i==22)  bit.b22=Value?1:0;
        else if(i==23)  bit.b23=Value?1:0;
        else if(i==24)  bit.b24=Value?1:0;
        else if(i==25)  bit.b25=Value?1:0;
        else if(i==26)  bit.b26=Value?1:0;
        else if(i==27)  bit.b27=Value?1:0;
        else if(i==28)  bit.b28=Value?1:0;
        else if(i==29)  bit.b29=Value?1:0;
        else if(i==30)  bit.b30=Value?1:0;
        else            bit.b31=Value?1:0;
    }

    bool SetInt( __int64 Value)
    {
        if(Value>4294967295 || Value<0)
            return false;
        else
            UnsignInt=Value;
        return true;
    };
    AnsiString BitString()
    {
        AnsiString Str;
        Str.sprintf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
        bit.b31, bit.b30, bit.b29, bit.b28, bit.b27, bit.b26, bit.b25, bit.b24,
        bit.b23, bit.b22, bit.b21, bit.b20, bit.b19, bit.b18, bit.b17, bit.b16,
        bit.b15, bit.b14, bit.b13, bit.b12, bit.b11, bit.b10, bit.b9,  bit.b8,
        bit.b7,  bit.b6,  bit.b5,  bit.b4,  bit.b3,  bit.b2,  bit.b1,  bit.b0);
        return Str;
    };

    AnsiString Hex(bool bWith0x=false)
    {
        __int64 int64;
        int64=(__int64)UnsignInt;
        if(bWith0x)
            return "0x"+IntToHex(int64, 2);
        else
            return IntToHex(int64, 2);
    }
};
//------------------------------------------------------------------------------
#define MAX_Q_10 500                                                            //Steven 20200825 : Task list改成100筆
#define MAX_Q_100 100
typedef struct TMyQueue10                                                       //Steven 20180808 (wei) : 修改紀錄Task的方式
{
    AnsiString Alias;
    AnsiString DateTime[MAX_Q_10];
    double dData[MAX_Q_10];
    int iData[MAX_Q_10];
    bool bInt;
    int iIndex;
    int iCount;
    int *iTask;

    TMyQueue10();
    void ClearData();
    void Add(int data);
    void Add(double data);
    bool CheckTaskChange();                                                     //跟最新的不一樣就++
    double GetData(int i);
    double GetLastData();
    AnsiString GetDateTime(int i);
    AnsiString ShowCommaText(bool bWithDateTime);
    void SetAliasAndTask(AnsiString sAlias, int *Task);
}TMyQueue10;
//------------------------------------------------------------------------------
typedef struct TMyQueue100
{
    AnsiString Alias;
    AnsiString DateTime[MAX_Q_100];
    double dData[MAX_Q_100];
    int iData[MAX_Q_100];
    bool bInt;
    int iIndex;
    int iCount;

    TMyQueue100();
    void ClearData();
    void Add(int data);
    void Add(double data);
    double GetData(int i);
    AnsiString GetDateTime(int i);
    AnsiString ShowCommaText(bool bWithDateTime);
}TMyQueue100;
//------------------------------------------------------------------------------
typedef struct TMyStrQueue100
{
    AnsiString DateTime[MAX_Q_100];
    AnsiString sData1[MAX_Q_100];
    AnsiString sData2[MAX_Q_100];
    AnsiString sLastFileName;
    int iIndex;
    int iCount;

    TMyStrQueue100();
    void ClearData();
    void Add(AnsiString data1, AnsiString data2="");
    AnsiString GetData(int i);
    AnsiString GetDateTime(int i);
    AnsiString ShowCommaText(bool bWithDateTime);
    void SafeData();
}TMyStrQueue100;
void StartTestTimeStamp(bool bAdd=true);
void EndTestTimeStamp(int iAdd=1);
//------------------------------------------------------------------------------
typedef struct TMyTimerQueue100
{
    AnsiString DateTime[MAX_Q_100];
    AnsiString sStartTime[MAX_Q_100];
    AnsiString sEndTime[MAX_Q_100];
    AnsiString sTimeStr[MAX_Q_100];
    AnsiString sLastFileName;
    int iTime[MAX_Q_100];
    int iIndex;
    int iCount;

    TMyTimerQueue100();
    void ClearData();
    void Add(AnsiString sTime, AnsiString eTime, int iCurrTime);
    int GetTimeData(int i);
    AnsiString GetStartTime(int i);
    AnsiString GetEndTime(int i);
    AnsiString GetTimeString(int i);
    AnsiString GetDateTime(int i);
    AnsiString ShowCommaText(bool bWithDateTime);
    void SafeData();
}TMyTimerQueue100;
//------------------------------------------------------------------------------
extern int FindAndKillProcess(LPCTSTR lpszProcessName);                         //JerryYang 20200430 Kill Eventlogsaver
void LogIndexMaxMinPos(AnsiString str="Unknowed");                              //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，記錄並存檔
void UDPErrorLog(AnsiString aTitle, AnsiString Command);                        //kevin 20211020 UDP error log
void RecordErrorLog(int iSaveToFile,AnsiString FilePth, AnsiString Command);    //kevin 20211022 any error log
void sDataTimelog(AnsiString &Msg);                                             //kevin 20211027 log + DataTime
void SetSocketHandlerID(AnsiString strID);                                      //Jimmychiu 20220805 SocketHandlerID can not be NULL
void ShuttleLog();                                                              //kevin 20220912 add shuttle sensor record

double Round(double x, double point);                                           //ChungHung 20210113 add for Alignment CCD
int Round(double x);                                                            //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
AnsiString GetBundleInfo(int iAuto);                                            //JerryYang 20240318 : add
void RotationCoordinates(double px, double py, double &px1, double &py1, double RadianX, double RadianY, bool bCCD=true);    //ChungHung 20210113 add for Alignment CCD

AnsiString GetErrorMessage(DWORD dwErrorMessageCode);                           //Steven 20240911 : 抓取系統錯誤的訊息

class VerInfo                                                                   //Sam 20230328 : 自動更新增加版本檢查
{
    public:
        VerInfo();
        __property AnsiString   FileName            ={read=m_strFileName,    write=m_SetFileName};   //檔案名稱
        __property AnsiString   FixedFileVersion    ={read=m_strGetFixedFileVersion};                //檔案版本
        __property WORD         LangID              ={read=m_wLangID};                               //語系
        __property WORD         CharsetID           ={read=m_wCharsetID};                            //字元表
        __property AnsiString   ProductName         ={read=m_strProductName};                        //產品名稱
        __property AnsiString   ProductVersion      ={read=m_strProductVersion};                     //產品版本
        __property AnsiString   OriginalFilename    ={read=m_strOriginalFilename };                  //原始檔名
        __property AnsiString   FileDescription     ={read=m_strFileDescription};                    //檔案說明
        __property AnsiString   CompanyName         ={read=m_strCompanyName};                        //公司名稱
        __property AnsiString   LegalCopyright      ={read=m_strLegalCopyright};                     //著作權
        __property AnsiString   LegalTrademarks     ={read=m_strLegalTrademarks};                    //合法商標
        __property AnsiString   InternalName        ={read=m_strInternalName};                       //內部名稱
        __property AnsiString   Comments            ={read=m_strComments};                           //說明
        void GetAppVersion(AnsiString sAppExeName, WORD& major, WORD& minor, WORD& build, WORD& revision);
        AnsiString GetSVNRev();
        AnsiString GetFileVersion();
        AnsiString GetMainVersion();
    private:
        AnsiString m_strFileName;
        //AnsiString m_strGetFileName(void);
        void m_SetFileName(AnsiString);
        VS_FIXEDFILEINFO m_stFileInfo;
        AnsiString m_strGetFixedFileVersion(void);
        AnsiString m_strGetFixedProductVersion(void);
        WORD m_wLangID;
        WORD m_wCharsetID;
        AnsiString m_strProductName;
        AnsiString m_strProductVersion;
        AnsiString m_strOriginalFilename;
        AnsiString m_strFileDescription;
        AnsiString m_strFileVersion;
        AnsiString m_strCompanyName;
        AnsiString m_strLegalCopyright;
        AnsiString m_strLegalTrademarks;
        AnsiString m_strInternalName;
        AnsiString m_strComments;
        DWORD m_dwLastError;
        DWORD m_dwGetLastError(void);
        void m_GetVerInfo(void);
        void m_ClearData(void);
};
//------------------------------------------------------------------------------
#endif
