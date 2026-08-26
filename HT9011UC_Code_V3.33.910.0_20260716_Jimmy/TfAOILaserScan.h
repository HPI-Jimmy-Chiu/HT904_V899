//---------------------------------------------------------------------------

#ifndef TfAOILaserScanH
#define TfAOILaserScanH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp>
#include "uSocketServerClient.h"                                                //JimmyChiu 20220219 : 殘料檢查 From Loader
#include "myTimer.h"
#include "HTray.h"                                                              //JimmyChiu 20220219 : 殘料檢查 From Loader
//---------------------------------------------------------------------------
typedef void (__closure *TPointVoidShowMsg)(AnsiString);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TAOISocket                                                                //Jimmychiu 20240322 : Add for aoi connect class
{
private:    // User declarations
    uSocketClient *socketClient;
    TPointVoidShowMsg RecordMsg;
    int iCommandTask;
    bool bflag;
public:     // User declarations
    TAOISocket();
    ~TAOISocket();
    AnsiString DoSendCommand(bool bIsFirst,AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    void SendCommandSocket(AnsiString asCommand);
    bool DoCommuncation();
    bool IsCommActive();
    void SetRecordMsg(TPointVoidShowMsg t)
    {
        RecordMsg=t;
    }
    AnsiString asReceiveMsg;
    AnsiString GetSocketAddress(){return socketClient->GetSocketAddress();}
    AnsiString GetSocketPort(){return socketClient->GetSocketPort();}
    bool SetCommParameter(AnsiString asAddress,AnsiString asPort){return socketClient->SetCommParameter(asAddress,asPort);}
    void Close(){socketClient->Close();}
};
//---------------------------------------------------------------------------
class TfAOI                                                                     //JimmyChiu 20220312 add AOI base class
{
private:    // User declarations
    bool bflag;
    int iCommandDelayTime;
    AnsiString asTempStr;
    int iMovetrack;
public:     // User declarations
    TfAOI(AnsiString asname);
    TfAOI();
    ~TfAOI();
    enum eAOI_Command
    {
        eAOI_GetVersion=0,
        eAOI_ClearBuffer,
        eAOI_GetFileName,
        eAOI_LoadFile,
        eAOI_Reset,
        eAOI_GrabIC,
        eAOI_GetValueIC,
        eAOI_SetLot,
        eAOI_CreateFile,
        eAOI_SetGoldenImage,
        eAOI_GetStatus,
        eAOITotal
    };
    enum eMoveTrack
    {
        eMT_Loader=0,
        eMT_Empty,
        eMT_Color,
        eMT_Auto1,
        eMT_Auto2,
        eMT_Auto3,
        eMTTotal
    };
    TAOISocket  *socketAOI;
    TStringList *tsAnalysis;
    int iRow,iCol;
    int CommandDelay(){return iActiveDelayTime;}
    int iLD_TrayArrivalDely(){return 4;}
    int LoaderCarEdgePushDelay(){return 1;}
    //common cmd
    AnsiString GetLoadFileName();
    AnsiString GetLotMO();
    AnsiString GetErrorMsg(AnsiString asFunName,AnsiString asErrorType,AnsiString asContent);
    AnsiString GetAOIReturnError(){return "AOI return Error";}
    AnsiString GetReceiveWrongCommand(){return "Receive wrong command";}
    AnsiString GetTrayData();
    TPointVoidShowMsg RecordMsgLaser;
    TPointVoidShowMsg RecordErrorMsgLaser;
    //<==socket Ctrl
    AnsiString asSendMsg;
    AnsiString asReceiveMsg;
    uSocketClient *socketClient;
    AnsiString asName;
    TQPF_Timer AOIDelay;
    TQPF_Timer htDetectICFloatingDelay;
    TQPF_Timer ttLoaderCarEdgePushDelay;
    TQPF_Timer TrayArrivalDelay;
    AnsiString DoSendCommand(bool bIsFirst,AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    void SendCommandSocket(AnsiString asCommand);
    //<==socket Ctrl
    int iTDCTask;
    int iCommandTask;
    bool bSimulationAOICommand;
    AnsiString GetAOI_Command(eAOI_Command eaoi);
    bool DoCommuncation();
    bool IsCommActive();
    AnsiString GetXC_YC();
    virtual TTMyTray* GetTrayIC(){return NULL;}
    virtual bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError){return true;}
    virtual bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError){return true;}
    //auto tuning
    bool DoAutoTuning(bool bIsFirst,AnsiString &asError);
    //single command s
    virtual bool DoAOI_LoadFile(bool bIsFirst){return true;}
    virtual bool DoAOI_Function(bool bIsFirst, int iIndex=0){return true;}        //RogerYang 20250830 : 殘料檢變更格式
    virtual bool DoGetValue(bool bIsFirst){return true;}
    virtual bool DoAOI_ClearBuffer(bool bIsFirst){return true;}
    bool DoSetLot(bool bIsFirst);
    virtual bool DoCreateFile(bool bIsFirst){return true;}
    virtual bool DoGoldenImage(bool bIsFirst, int index=0){return true;}          //RogerYang 20250830 : 殘料檢變更格式
    virtual bool DoGetStatus(bool bIsFirst){return true;}
    //single command e
    virtual bool IsRemainIC(){return true;}
    virtual bool HasNullIC(TTMyTray* mTray){return false;}
    bool DoMoveOut(bool bIsFirst,AnsiString &asError);
    bool DoMoveIn(bool bIsFirst,AnsiString &asError);
    void SetMoveTrack(int iTrack);
    int  GetMoveTrack();
    bool GetInSensor();
    bool GetOutSensor();
    AnsiString GetName(){return asName;}
    bool bNeedCheckInitialStart;
    bool bRealRunCheckInitialStart;
    bool bEnableRetryCount;
    int  iRemainICLaserRetryCount;
    bool bRemainICLaserAutoSkip;
    int iActiveDelayTime;
    int SetAndBackUpLoaderYSpeed(int iLoaderYCWSpeedPerc=80);                   //RogerYang 20250906 : Add for MLoaderY
    void RestoreLoaderYSpeedRaw(int iRawSpeed);                                 //AI(ht9045-v899) 20260505: 還原 raw speed (與 SetAndBackUpLoaderYSpeed 配對)
    virtual void CloneTrayData(TTMyTray* sourceT,TTMyTray* targetT){}
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfTrayDeviceCheckFromLoader : public  TfAOI             //JimmyChiu 20220312 add class
{
private:    // User declarations
    bool bIsRemainIC;
    AnsiString GetFunctionCode(){return "5";}
public:     // User declarations
    TfTrayDeviceCheckFromLoader(AnsiString asname);
    ~TfTrayDeviceCheckFromLoader();
    bool IsRemainIC(){return bIsRemainIC;}
    bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError);
    bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError);
    bool DoAOI_LoadFile(bool bIsFirst);
    bool DoAOI_Function(bool bIsFirst, int iIndex=0);                           //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetValue(bool bIsFirst);
    bool DoAOI_ClearBuffer(bool bIsFirst);
    bool DoGoldenImage(bool bIsFirst, int iIndex);                              //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetStatus(bool bIsFirst);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfTrayMapFromLoader : public  TfAOI             //JimmyChiu 20220312 add class
{
private:    // User declarations
    TTMyTray* trayMapIC;
    int itemp;
    void AnalysisRowCol(AnsiString asInput,int &icow,int &irow);
    AnsiString GetFunctionCode(){return "4";}
public:     // User declarations
    TfTrayMapFromLoader(AnsiString asname);
    ~TfTrayMapFromLoader();
    void AnalysisTrayData(TTMyTray* mTray,AnsiString asRawData);
    TTMyTray* GetTrayIC(){return trayMapIC;}
    bool HasNullIC(TTMyTray* mTray);
    void SetTrayIC(TTMyTray* _value){trayMapIC=_value;}
    bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError);
    bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError);
    bool DoAOI_LoadFile(bool bIsFirst);
    bool DoAOI_Function(bool bIsFirst, int iIndex=0);                           //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetValue(bool bIsFirst);
    bool DoAOI_ClearBuffer(bool bIsFirst);
    bool DoCreateFile(bool bIsFirst);
    bool DoGoldenImage(bool bIsFirst, int iIndex);                              //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetStatus(bool bIsFirst);
};
//---------------------------------------------------------------------------
#endif
