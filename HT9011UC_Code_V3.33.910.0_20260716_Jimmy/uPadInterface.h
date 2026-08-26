//---------------------------------------------------------------------------

#ifndef uPadInterfaceH
#define uPadInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "SPComm.hpp"
#include <Menus.hpp>
#include "ALed.hpp"
#include "butPa1.h"
#include "MyLed.h"
#include "BtnPanelLane.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
#define PAD_ControlDLC      0x04
#define PAD_FrontControl    0x00
#define PAD_RearControl     0x01
#define PAD_LedLight        0x00
#define PAD_LedBling        0x01
#define PAD_PowerOff        0x000001                 //00000000000000001
#define PAD_PowerOn         0x000002                 //00000000000000010
#define PAD_PannelEnable    0x000004                 //00000000000000100
#define PAD_Reset           0x000008                 //00000000000001000
#define PAD_Pause           0x000010                 //00000000000010000
#define PAD_Home            0x000020                 //00000000000100000
#define PAD_Start           0x000040                 //00000000001000000
#define PAD_OneCycle        0x000080                 //00000000010000000
#define PAD_Retry           0x000100                 //00000000100000000
#define PAD_Skip            0x000200                 //00000001000000000
#define PAD_CleanOut        0x000400                 //00000010000000000
#define PAD_TrayFeed        0x000800                 //00000100000000000
#define PAD_TrayEnd         0x001000                 //00001000000000000
#define PAD_AlarmReset      0x002000                 //00010000000000000
#define PAD_SafeLock        0x004000                 //00100000000000000
#define PAD_Step            0x008000                 //01000000000000000
#define PAD_TStart          0x010000                 //10000000000000000
//===========================================================================
typedef struct
{
    TMyLed          *mlEvent;
    TBtnPanelLane   *btnEvent;
    AnsiString      PadName;
    int             iData;
    AnsiString      InputName;

    void SetItem(TMyLed *_mlEvent, TBtnPanelLane *_btnEvent, AnsiString _PadName, int _iData, AnsiString _InputName);
} PAD_PTR;
//===========================================================================
class TPadRS232Thread : public TThread
{
    private:
    protected:
        void __fastcall Execute();
    public:
        __fastcall TPadRS232Thread(bool CreateSuspended);
        void __fastcall TPadRS232Thread::RS232ThreadProcess(void);
};
extern TPadRS232Thread *MyPad232Thread;
//---------------------------------------------------------------------------
class TfPadInterface : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pc_PadInterface;
    TPanel *pn_PadInterfaceTitle;
    TSpeedButton *sb_PadInterface_Exit;
    TPanel *sb_PadInterface_MainPage;
    TPanel *pn_PadInterface_Front;
    TMyLed *ml_PadInterface_FrontPowerOff;
    TMyLed *ml_PadInterface_FrontPowerOn;
    TMyLed *ml_PadInterface_FrontReset;
    TMyLed *ml_PadInterface_FrontPause;
    TMyLed *ml_PadInterface_FrontHome;
    TMyLed *ml_PadInterface_FrontStart;
    TMyLed *ml_PadInterface_FrontOneCycle;
    TMyLed *ml_PadInterface_FrontRetry;
    TMyLed *ml_PadInterface_FrontSkip;
    TMyLed *ml_PadInterface_FrontCleanOut;
    TMyLed *ml_PadInterface_FrontTrayFeed;
    TMyLed *ml_PadInterface_FrontTrayEnd;
    TMyLed *ml_PadInterface_FrontAlarmReset;
    TPanel *pn_PadInterface_FrontTop;
    TBtnPanelLane *sb_PadInterface_FrontPowerOff;
    TBtnPanelLane *sb_PadInterface_FrontPowerOn;
    TBtnPanelLane *sb_PadInterface_Front;
    TBtnPanelLane *sb_PadInterface_FrontStart;
    TBtnPanelLane *sb_PadInterface_FrontHome;
    TBtnPanelLane *sb_PadInterface_FrontPause;
    TBtnPanelLane *sb_PadInterface_FrontReset;
    TBtnPanelLane *sb_PadInterface_FrontOneCycle;
    TBtnPanelLane *sb_PadInterface_FrontRetry;
    TBtnPanelLane *sb_PadInterface_FrontSkip;
    TBtnPanelLane *sb_PadInterface_FrontCleanOut;
    TBtnPanelLane *sb_PadInterface_FrontAlarmReset;
    TBtnPanelLane *sb_PadInterface_FrontTrayEnd;
    TBtnPanelLane *sb_PadInterface_FrontTrayFeed;
    TPanel *pn_PadInterface_Rear;
    TMyLed *ml_PadInterface_RearPowerOff;
    TMyLed *ml_PadInterface_RearPowerOn;
    TMyLed *ml_PadInterface_RearReset;
    TMyLed *ml_PadInterface_RearPause;
    TMyLed *ml_PadInterface_RearHome;
    TMyLed *ml_PadInterface_RearStart;
    TMyLed *ml_PadInterface_RearOneCycle;
    TMyLed *ml_PadInterface_RearRetry;
    TMyLed *ml_PadInterface_RearSkip;
    TMyLed *ml_PadInterface_RearCleanOut;
    TMyLed *ml_PadInterface_RearTrayFeed;
    TMyLed *ml_PadInterface_RearTrayEnd;
    TMyLed *ml_PadInterface_RearAlarmReset;
    TMyLed *ml_PadInterface_RearStep;
    TMyLed *ml_PadInterface_RearTStart;
    TMyLed *ml_PadInterface_Rear;
    TMyLed *ml_PadInterface_RearSafeLock;
    TPanel *pn_PadInterface_RearTop;
    TBtnPanelLane *sb_PadInterface_RearPowerOff;
    TBtnPanelLane *sb_PadInterface_RearPowerOn;
    TBtnPanelLane *sb_PadInterface_RearPause;
    TBtnPanelLane *sb_PadInterface_RearReset;
    TBtnPanelLane *sb_PadInterface_RearOneCycle;
    TBtnPanelLane *sb_PadInterface_RearRetry;
    TBtnPanelLane *sb_PadInterface_RearSkip;
    TBtnPanelLane *sb_PadInterface_RearHome;
    TBtnPanelLane *sb_PadInterface_RearStart;
    TBtnPanelLane *sb_PadInterface_RearCleanOut;
    TBtnPanelLane *sb_PadInterface_RearAlarmReset;
    TBtnPanelLane *sb_PadInterface_RearTrayEnd;
    TBtnPanelLane *sb_PadInterface_RearTrayFeed;
    TBtnPanelLane *sb_PadInterface_RearStep;
    TBtnPanelLane *sb_PadInterface_RearTStart;
    TBtnPanelLane *sb_PadInterface_RearSafeLock;
    TMemo *Memo_PadInterface;
    TButton *sb_PadInterface_ManualSend;
    TEdit *ed_PadInterface_ManualSend;
    TButton *btnResetCom;
    TCheckBox *cb_PadInterface_PadLedBling;
    TBtnPanelLane *sb_PadInterface_Rear;
    TMyLed *ml_PadInterface_Front;

    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ClearLog1Click(TObject *Sender);
    void __fastcall sb_PadInterface_ManualSendClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sb_PadInterface_ExitClick(TObject *Sender);
    void __fastcall sb_PadInterface_FrontPowerOffMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    AnsiString  aParaPath;
    AnsiString  aBaudRate;
    int         iByteSize;
    int         iStopBit;
    int         iParity;
    TList       *MyPageChangeList;
    TStringList *slPadIniName;
    AnsiString  aReciveData;
    AnsiString  aSendData;

    void PadWriteDataToFile(AnsiString cFilePath, AnsiString cData);
    void SearchChangePageButton(TWinControl *PCtrl);
    void __fastcall FormButtonClick(TObject *Sender);
    void __fastcall PadButtonClick(TObject *Sender);
    enum ePadName                                                               //Jimmychiu 20230718 : Fixed for 三面板控制
    {   epn_SnFKPowerOff=0,     epn_SnFKPowerOn=1,      epn_SnFrontPadActive=2, epn_SnFKReset=3,        epn_SnFKPause=4,
        epn_SnFKHome=5,         epn_SnFKStart=6,        epn_SnFKOneCycle=7,     epn_SnFKRetry=8,        epn_SnFKSkip=9,
        epn_SnFKCleanOut=10,    epn_SnFKTrayFeed=11,    epn_SnFKTrayEnd=12,     epn_SnFKAlarmReset=13,  epn_SnRKPowerOff=14,
        epn_SnRKPowerOn=15,     epn_SnRKReset=16,       epn_SnRKPause=17,       epn_SnRKHome=18,        epn_SnRKStart=19,
        epn_SnRKOneCycle=20,    epn_SnRKRetry=21,       epn_SnRKSkip=22,        epn_SnRKCleanOut=23,    epn_SnRKTrayFeed=24,
        epn_SnRKTrayEnd=25,     epn_SnRKAlarmReset=26,  epn_SnRKSafeLock=27,    epn_SnRKManualStep=28,  epn_SnRKManualTStart=29,
        epn_SnRearPadActive=30, epn_Total
    };
public:     // User declarations
    __fastcall TfPadInterface(TComponent* Owner);

    bool            bRs232Ok;
    TStringList     *CommReceiveList;
    TStringList     *CommReceiveLength;
    vector<String>  SendData;
    vector<String>  RequestData;
    PAD_PTR         PadItem[32];
    int             CheckPadItem;
    bool            bPadStatus[32];
    bool            bShow;
    TStringList     *CommSendList;
    TStringList     *CommSendLength;
    bool            bRequestVer;
    bool            bScanSwitch;        //KenHsieh 20211224 : 新增按鈕狀態掃描

    void InitialVariable();
    void __fastcall ResetComm();
    bool OpenCommPort();
    bool CloseCommPort();
    void SendCommand(AnsiString sData);
    void SendCommand(vector<Byte>& Data);
    void __fastcall ProcessReceiceData();
    void __fastcall DoScanPanelLed(int iAddress,int iKey);
    void __fastcall DoUpdataPadStatus(int iAddress,int iKey);
    void __fastcall Main232();
    void SendSwitchStatus(TBtnPanelLane *bpPtr);
    void SendSwitchStatus(AnsiString aName,bool Type);
    bool IsPadButton(AnsiString aName);
    bool IsPadKey(AnsiString aName);
    bool __fastcall ProcessSendDataNew();
    bool __fastcall ProcessScanKey(AnsiString aSenName);
    void RequestPadVersion();
    bool bSendSwitchStatusing;
    void RecordCommunication(AnsiString aTitle,AnsiString Command);
};
//---------------------------------------------------------------------------
extern PACKAGE TfPadInterface *fPadInterface;
//---------------------------------------------------------------------------
#endif
