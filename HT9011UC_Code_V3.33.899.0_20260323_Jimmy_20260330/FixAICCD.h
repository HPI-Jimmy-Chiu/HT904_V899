//---------------------------------------------------------------------------

#ifndef FixAICCDH
#define FixAICCDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TfFixAICCD : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsTraySetting;
    TLabel *lbllFix2AICCDGetResultTimeOut;
    TLabel *lblFix2AICCDStartDelay;
    TLabel *lablFix2AICCDAutoRetry;
    TLabel *lbllFix2AICCDExposureTimeOut;
    TEdit *edtlFix2AICCDGetResultTimeOut;
    TEdit *edtlFix2AICCDStartDelay;
    TEdit *edtlFix2AICCDAutoRetry;
    TEdit *edtlFix2AICCDExposureTimeOut;
    TPanel *Panel4;
    TCheckBox *chkEnableFix2AICCD;
    TTabSheet *tsTrayCCD;
    TPanel *Panel2;
    TGroupBox *GroupBox2;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edFix2AICCD_Address;
    TEdit *edtFix2AICCD_Port;
    TBitBtn *btnFix2AICCD_Connect;
    TBitBtn *btnFix2AICCD_Disconnect;
    TButton *btFix2AICCD_Trigger;
    TEdit *edtFix2AICCD_CMD;
    TMemo *memoFix2AICCDCommLog;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TTimer *tmrProcessFixAICCDData;
    TClientSocket *ClientSocket_FixAICCD;
    TTimer *TimerDownFixAICCDConnect;
    TCheckBox *chkEnableLearningMode;
    TRadioGroup *rgFixAICCD;
    TLabel *lblBGALightValue;
    TPanel *pnlBGALightValue;
    TScrollBar *scrBGALightValue;
    TTimer *tmrLightControl;
    TLabel *lablFix2AICCDInspectCycle;
    TEdit *edtOutArmCycleInsp;
    TLabel *lablFix2AICCDInspectCycleCounter;
    TLabel *lblInspectResultThres;
    TEdit *edtInspectResultThres;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edFix2AICCD_Address2;
    TEdit *edtFix2AICCD_Port2;
    TBitBtn *btnFix2AICCD_Connect2;
    TBitBtn *btnFix2AICCD_Disconnect2;
    TButton *btFix2AICCD_Trigger2;
    TEdit *edtFix2AICCD_CMD2;
    TClientSocket *ClientSocket_FixAICCD2;
    TLabel *Label3;
    TRadioGroup *rgResultShowType;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall tmrProcessFixAICCDDataTimer(TObject *Sender);
    void __fastcall TimerDownFixAICCDConnectTimer(TObject *Sender);
    void __fastcall ClientSocket_FixAICCDConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_FixAICCDDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_FixAICCDError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket_FixAICCDRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall btnFix2AICCD_ConnectClick(TObject *Sender);
    void __fastcall btnFix2AICCD_DisconnectClick(TObject *Sender);
    void __fastcall btFix2AICCD_TriggerClick(TObject *Sender);
    void __fastcall scrBGALightValueChange(TObject *Sender);
    void __fastcall tmrLightControlTimer(TObject *Sender);
    void __fastcall pnlBGALightValueDblClick(TObject *Sender);
    void __fastcall edtFix2AICCD_PortClick(TObject *Sender);
    void __fastcall edtlFix2AICCDStartDelayClick(TObject *Sender);
    void __fastcall edtlFix2AICCDAutoRetryClick(TObject *Sender);
    void __fastcall edtInspectResultThresClick(TObject *Sender);
    void __fastcall edtOutArmCycleInspClick(TObject *Sender);
private:    // User declarations
    AnsiString sProessFixAICCDRecevieData[2];
    bool DoBGAViewFunction();
    bool DoMoveXY_BGAView(int &iRow, int &iCol);
    void InitBGAViewProcess();
    void InitBGAViewFunction();
    bool DoBGAViewFunction_LearningMode(int &iRow, int &iCol);
    void InitBGAViewInsp();
    bool DoBGAViewFunction_Inspection();

    AnsiString asOutArmCycleCounter;
    bool bBGAViewInspResult;

    TClientSocket *clntsckt_FixAOI[eTotalFixAOI];
    TEdit               *edAddress[eTotalFixAOI];
    TEdit                  *edPort[eTotalFixAOI];
    TEdit                   *edCMD[eTotalFixAOI];

    //方北
    bool    bWait;
public:     // User declarations
    __fastcall TfFixAICCD(TComponent* Owner);
    void __fastcall DoIniDataToForm();
    void __fastcall ReadFile();
    void ChangeFix2AICCDSetupFile();
    void SendFix2AICCDCommand(int index, AnsiString Msg, AnsiString Msg2="");
    void AddFix2AICCDCommunicationLog(int index, AnsiString Str);
    TStringList *SocketFixAICCDReceiveList[2];
    bool fShow;
    bool bEnableFix2AICCDChannelConnect[2];

    //OutArm Cycle璸计
    int iOutArmCycleCounter;
    void OutArmCycleCounterUpdate();
    int UnloadAICntNG[eTrayCount];
    void InitialCycleCount();                                                   //Sam 20231108 : FixAOI 穝糤
    //硄癟
    bool bAICCDExposureOK;
    bool bAICCDError;
    bool bAICCDGetResultOK;
    int iAICCDGrabCnt;                                                          //Sam 20240325 : 穝糤 DamageTrayMapping 
    int iAICCDResultCnt;
    void Fix2AICCDFunction();
    bool DoFix2AICCDFunction();
    bool bBGAViewInspRunning;
    bool NeedToGrabImage();

    //方北
    void LightDataReflesh();
    void LightControlUpdate();
    void bCheckUnloaderHasAiNG(int iMot);                                       //Sam 20240325 : 穝糤 DamageTrayMapping 

    int iRetryConnectTimer;                                                     //Sam 20231116 : 穝糤﹚竡把计
};
//---------------------------------------------------------------------------
extern PACKAGE TfFixAICCD *fFixAICCD;
//---------------------------------------------------------------------------
#endif
