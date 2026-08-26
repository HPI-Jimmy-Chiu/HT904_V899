//---------------------------------------------------------------------------

#ifndef uDTME08ControlH
#define uDTME08ControlH
//---------------------------------------------------------------------------
#include "uSocketServerClient.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
typedef void (__closure *TPointVoidShowMsg)(AnsiString);
#define swap16(x) ((((x) & 0xFF)<<8) | (((x)>>8) & 0xFF))
//---------------------------------------------------------------------------
#define SendCommLength 200
//---------------------------------------------------------------------------
class uDTME08Info
{
private:
public: // User declarations
    uDTME08Info()
    {
        InitialDatas();
    }
    void InitialDatas()
    {
        PV=SV=0.0;
        SetSV=SensorType=CycleTime=ControlActionOut2=0;
        RunAutoTunning=RealAutoTunning=Alarm3=Alarm2=IsCelsius=IsFahrenheit=Alarm1=IsRunOutput2=IsRunOutput1=IsRunAutoTuning=false;
        Event="";
    }
    double PV;
    AnsiString Event;
    double SV;
    int SetSV;
    void SetSettingSV(double dvalue){SetSV=(int)(dvalue*10);}
    bool RunAutoTunning;
    bool RealAutoTunning;
    int SensorType;
    int CycleTime;
    void SetCycleTime(double dvalue){CycleTime=dvalue*10;}
    int ControlActionOut2;
    //status
    bool Alarm3;
    bool Alarm2;
    bool IsCelsius;
    bool IsFahrenheit;
    bool Alarm1;
    bool IsRunOutput2;
    bool IsRunOutput1;
    bool IsRunAutoTuning;
};
class uDTME08Control
{
private:    // User declarations
    uSocketClient *socketClient;
    AnsiString GetSettingFilePath(){return ExtractFilePath(Application->ExeName);}
//    AnsiString GetSettingFilePath(){return "D:\\HT9045\\system\\";}
    AnsiString GetSettingFileName(){return "DTME08_Control.ini";}
    AnsiString GetSettingFileNameWithPath(){return GetSettingFilePath()+GetSettingFileName();}
    bool bflag;
    //SendComm
    char strSendUse[SendCommLength];
    int iCommandLen;
    //Receive
    char* cReceive;
    int iReceiveLen;
    AnsiString tempStr;
    int tempInt;
    byte tempByte;
    int GetMinReceiveLen(){return 9+GetChannelNumberPerStation()*2;}
    void GetStatus(int iInput,uDTME08Info *uinfo);
    void ClearClientComm();
    bool bDebug;
public: // User declarations
    //enum
    enum eSetSensorType
    {
        esstKType=0,
        esstPT100=12,
    };
    enum eFunctionCode
    {
        efc_SetSV=0,
        efc_SetAT,
        efc_GetSV,
        efc_GetPV,
        efc_GetStatus,
        efc_SetSensorType,
        efc_SetCycleTime,
        efc_SetControlActionOut2,
    };
    enum eAutoTune
    {
        eatTrunStop=0,
        eatTrunRunning,
    };
    enum eControlAction
    {
        ecaHeating=0,
        ecaCooling,
        ecaDisable,
    };
    //
    uDTME08Control();
    ~uDTME08Control();
    void Initialization();
    TPointVoidShowMsg RecordMsg;
    TPointVoidShowMsg RecordErrorMsg;
    int GetMaxStationNumber(){return 4;}
    int GetChannelNumberPerStation(){return 8;}
    //socket
    void ReceiveData(char* cGet,int iLen);
    bool IsConnected(){return socketClient->IsConnected();}
    bool DoCommuncation(){return socketClient->DoOpenCommuncation();}
    void DoSocketClose(){socketClient->Close();}
    AnsiString GetSocketAddress(){return socketClient->GetSocketAddress();}
    AnsiString GetSocketPort(){return socketClient->GetSocketPort();}
    void SetSocketAddress(AnsiString asValue);
    void SetSocketPort(AnsiString asValue);
    void ReadSettingFile();
    void WriteSettingFile();
    void SendCommandSocket(char* cSet,int iLen);
    void AnalysisChannelStatus(int iInput,bool **arrBool,int iBoolLen);
    //
    int GetStationCode(int istate);
    AnsiString Chararr2Hexstring(char* cstr,int iNum);
    int GetFunctionCode(eFunctionCode fc);
    //
    bool DoGetSV(bool bIsFirst,int iStation);
    bool DoGetPV(bool bIsFirst,int iStation);
    bool DoGetStatus(bool bIsFirst,int iStation);
    //
    bool DoSetValue(eFunctionCode fc,bool bIsFirst,int iStation);
    bool DoSetSV(bool bIsFirst,int iStation);
    bool DoSetAT(bool bIsFirst,int iStation);
    bool DoSetSensorType(bool bIsFirst,int iStation);
    bool DoSetCycleTime(bool bIsFirst,int iStation);
    bool DoSetControlActionOut2(bool bIsFirst,int iStation);
    //
    std::map<int,uDTME08Info> mapDTMInfo;
    int GetSizeDTMEInfo(){return mapDTMInfo.size();}
    void InitialmapDTMInfoList();
    AnsiString ErrorCodeDescription(byte bdata);
    bool ReceiveInitial();
};
#endif
