//---------------------------------------------------------------------------

#ifndef MonitorInterfaceH
#define MonitorInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ALed.hpp"
#include "MyLed.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
#include "MonitorTCPIP.h"
//---------------------------------------------------------------------------
class TfMonitor : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnMVConnectTitle;
    TLabel *lbMVConnectState;
    TSpeedButton *sbMVConnect;
    TSpeedButton *sbMVDisconnect;
    TPageControl *MVPageControl;
    TTabSheet *tsMVMain;
    TTabSheet *tsMVSettingPage;
    TPanel *pnMVSetupPage;
    TLabel *lbMVAddress;
    TLabel *lbMVPort;
    TEdit *edMVPort;
    TEdit *edMVAddress;
    TTimer *MonitorTimer;
    TClientSocket *MonitorClientSocket;
    TPanel *pnMVButtonTitle;
    TSpeedButton *sbMVUpdate;
    TSpeedButton *sbMVExit;
    TMemo *MonitorMemo;
    TPanel *pnMVMain;
    TEdit *edCommandData;
    TSpeedButton *sbSendCommand;
    TSpeedButton *sbMVLogClear;
    TPanel *pnMVState;
    TSpeedButton *sbMonitor1Open;
    TSpeedButton *sbMonitor2Open;
    TSpeedButton *sbMonitor3Open;
    TSpeedButton *sbMonitor1Stop;
    TSpeedButton *sbMonitor2Stop;
    TSpeedButton *sbMonitor3Stop;
    TMyLed *MyLed1;
    TMemo *Memo1;
    TSpeedButton *sbMonitorGetHDSpace;
    TTabSheet *tsMonitorSpecific;
    TCheckBox *cbWhenHDFullAlarm;
    TEdit *edLowHDSpace;
    TEdit *edWhenHDFullPrompt;
    TLabel *lbWhenHDFullPrompt;
    TCheckBox *cbAfterHandlerTrayFeedMonitor1ClosedVideo;
    TEdit *edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTime;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbMVUpdateClick(TObject *Sender);
    void __fastcall sbMVConnectClick(TObject *Sender);
    void __fastcall sbMVDisconnectClick(TObject *Sender);
    void __fastcall sbMVExitClick(TObject *Sender);
    void __fastcall MonitorTimerTimer(TObject *Sender);
    void __fastcall sbSendCommandClick(TObject *Sender);
    void __fastcall sbMVLogClearClick(TObject *Sender);
    void __fastcall sbMonitor1OpenClick(TObject *Sender);
    void __fastcall sbMonitorGetHDSpaceClick(TObject *Sender);
    void __fastcall edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTimeMouseDown(
          TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
          int Y);
    void __fastcall edWhenHDFullPromptMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edLowHDSpaceMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edMVPortMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
    bool bShow;
public:     // User declarations
    __fastcall TfMonitor(TComponent* Owner);

    MonitorTCPIP *MVCtrl;

    bool bCommandReady;
    AnsiString sADDRESS;
    int iPORT;
    TStringList *sComData;            //pig 2015.11.30 ModifyMonitorIssue
    int iHDSpace;
    int iStatus[4];                                                            //AI(ht9045-v899) 20260422: expand monitor status array for Greatek Ch3

    void LoadTCPIPParament();
    void SaveTCPIPParament();
    void OpenMonitorVedio(int index);
    void StopMonitorVedio(int index);
    void GetMonitorVedioState(int index);
    void GetMonitorHDSpec();
    int iDisconnectCount; //Sam 20180419 : Add Monitor Video Retry Connect when CCD IPC Disconnect
};
//---------------------------------------------------------------------------
extern PACKAGE TfMonitor *fMonitor;
//---------------------------------------------------------------------------
#endif
