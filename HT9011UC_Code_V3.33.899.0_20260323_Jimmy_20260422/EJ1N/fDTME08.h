//---------------------------------------------------------------------------

#ifndef fDTME08H
#define fDTME08H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "aled.hpp"
#include "MyLed.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ALed.hpp"
#include "uDTME08Control.h"
#include "myTimer.h"
#include "MyOmronPanel.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfrmDTME08 : public TForm
{
    typedef bool (__closure *TPointVoidDoCommand)(bool,int);
__published:    // IDE-managed Components
    TPanel *Panel7;
    TPanel *Panel1;
    TGroupBox *GroupBox2;
    TScrollBox *ScrollBox1;
    TGroupBox *GroupBox;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TEdit *edSV;
    TPanel *Panel9;
    TMemo *meLogs;
    TPageControl *PageControl1;
    TTabSheet *tsAuto;
    TTabSheet *tsManual;
    TButton *btGetModuler;
    TButton *btRunStop;
    TRadioGroup *rgSensorType;
    TButton *btSetThermoType;
    TRadioGroup *rgRunStop;
    TPanel *Panel6;
    TPanel *Panel8;
    TButton *Button1;
    TTabSheet *tsSimple;
    TButton *btSetSV;
    TPanel *Panel10;
    TCheckBox *cbEnableAT;
    TGroupBox *gbAT;
    TButton *btAT;
    TButton *btATOff;
    TButton *btAT1by1;
    TTabSheet *tsRecord;
    TMemo *Memo1;
    TPanel *Panel11;
    TCheckBox *cbGetRecord;
    TButton *btSaveData;
    TGroupBox *grpStatus;
    TMyLed *ledEJ1N1;
    TLabel *Label25;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TCheckBox *cbSimulation;
    TGroupBox *GroupBox3;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edAddress;
    TEdit *edPort;
    TButton *btnSetSave;
    TButton *btnReload;
    TGroupBox *GroupBox1;
    TPanel *btnGetPV;
    TPanel *btnSetAT;
    TPanel *btnGetSV;
    TPanel *btnSetSV;
    TPanel *btnGetStatus;
    TComboBox *cbCh;
    TTimer *TimerUpdate;
    TPanel *btnSetSensor;
    TButton *btnCtrl;
    TButton *btnSetSVFollowCh1;
    TCheckBox *chkHeater;
    void __fastcall btnSetSaveClick(TObject *Sender);
    void __fastcall TimerUpdateTimer(TObject *Sender);
    void __fastcall btnGetPVClick(TObject *Sender);
    void __fastcall btnSetATClick(TObject *Sender);
    void __fastcall btnGetSVClick(TObject *Sender);
    void __fastcall btnSetSVClick(TObject *Sender);
    void __fastcall btnGetStatusClick(TObject *Sender);
    void __fastcall btSetThermoTypeClick(TObject *Sender);
    void __fastcall btnReloadClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btATClick(TObject *Sender);
    void __fastcall btSetSVClick(TObject *Sender);
    void __fastcall btAT1by1Click(TObject *Sender);
    void __fastcall btATOffClick(TObject *Sender);
    void __fastcall btnSetSensorClick(TObject *Sender);
    void __fastcall btnCtrlClick(TObject *Sender);
    void __fastcall btnSetSVFollowCh1Click(TObject *Sender);
    void __fastcall chkHeaterClick(TObject *Sender);
    void __fastcall edPortClick(TObject *Sender);
    void __fastcall edSVClick(TObject *Sender);

private:    // User declarations
    //enum
    enum eDoCommand
    {
        DoNone=0,
        DoGetSV,
        DoGetPV,
        DoSetSV,
        DoSetAT,
        DoGetStatus,
        DoSetSeonsorType,
        DoSetCycleTime,
        DoSetControlActionOut2,
    };
    enum eDoCommandInitial
    {
        DoInitialNone=0,
        DoInitialSetTypeSeonsor,
        DoInitialSetCycleTime,
        DoInitialSetControlActionOut2,
        DoInitialSetSV,
        DoInitialTotal,
    };
    int iDoInitialCommandStatus;
    int iDoCommandStatus;
    int iDoManualCommandStatus;
    int iDoAutoCommandStatus;
    int iNowActiveStation;
    void ClearAllCommandStatus();
    //
    int iTaskCommand;
    uDTME08Control *dtme08;
    bool brecordmsgLock;
    void NewuDTME08Control();
    void AddTempGUI();
    void DrawTempGUI();
    TQPF_Timer DTMTimeout;
    TQPF_Timer DTMReconnectTime;
    void AddSingleView(TScrollBox *ScrollBox1);
    //myPalGroup
    std::map<int,TMyOmronPanel*> myPalGroup;
    void AddTempPanel(TMyOmronPanel* tmop);
    int GetSize(){return myPalGroup.size();}
    //
    void InitialComboList();
    bool bTimerUpdateLock;
    AnsiString asErrorDTME08;
    bool GetErrorMsg();
    int TargetCh;
    int TargetStation;
    int TargetNo;
    int ChannelNumber;
    int iSingleStationTask;
    int iTaskDTME08Cycle;
    int iCycleRunNumber;
    int GetDTMTimeout(){return 2;}
    int GetCycleTime(){return 2;}
    int GetReconnectTime(){return 10;}
    bool bCycleFirstRun;
    bool bCtrlSwitch;
    void SetPanelPos();//Jimmychiu 20230628 : DTME08ª«¥ó±Æ¦C
public:     // User declarations
    __fastcall TfrmDTME08(TComponent* Owner);
    bool bShow;
    void InitialData();
    void SaveData();
    void Reload();
    void ShowMSG(AnsiString asStr);
    void RecordMsg(AnsiString asMsg);
    void RecordErrorMsg(AnsiString asMsg);
    TPointVoidDoCommand tpvDoCommand;
    bool DoSendCommandByManual(bool bIsFirst,int iStation);
    void GetTargetCh(int ich);
    void SetSingleCommand(TPointVoidDoCommand tcdc,int ich=0);
    void GetStationAndNumber(int iCh,int &iStation,int &iNumber);
    bool DoManualProcess();
    void SetChannelNumber(int iNum){ChannelNumber=iNum;}
    int GetChannelNumber(){return ChannelNumber;}
    int GetChannelIndex(){return cbCh->ItemIndex;}
    void SetSP(int iCh,double SV,AnsiString Event);
    void SetPV(int iCh,double PV,AnsiString Event);
    void SetATStatus(int iCh,bool bOn);
    void DoDTME08Cycle();
    bool DoGetAllPV_DTME08();
    void InitialSetting(int iStation);
    void InitialDoSetAllAT(int iStation);
    void InitialDoSetAllSV(int iStation);
    void InitialDoSetSensorType(int iStation);
    void InitialDoSetCycleTime(int iStation);
    void InitialDoSetControlActionOut2(int iStation);
    TMyOmronPanel* GetPalGroup(int index);
    void SetSettingSV(int iCh,double dtemp);
    bool IsNeedSetSV();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDTME08 *frmDTME08;
//---------------------------------------------------------------------------
#endif
