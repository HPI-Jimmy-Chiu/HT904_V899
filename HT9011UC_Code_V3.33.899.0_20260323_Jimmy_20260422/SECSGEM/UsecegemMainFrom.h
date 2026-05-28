//---------------------------------------------------------------------------

#ifndef UsecegemMainFromH
#define UsecegemMainFromH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFSECS : public TForm
{
__published:    // IDE-managed Components
    TTimer *TimerTooling;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TPanel *Pal00;
    TSpeedButton *GemSBSetup;
    TLabel *l2DBarCodeFilename;
    TGroupBox *GroupBox7;
    TPanel *SECSConnectionState;
    TGroupBox *GroupBox8;
    TSpeedButton *BtnEnableComm;
    TSpeedButton *BtnDisableComm;
    TPanel *GEMCommunicatingState;
    TRadioGroup *GemEnableOrDisable;
    TGroupBox *GroupBox9;
    TSpeedButton *GemBtnOnlineRequest;
    TSpeedButton *GemBtnOfflineRequest;
    TSpeedButton *GemBtnOnlineRemote;
    TSpeedButton *GemBtnOnlineLocal;
    TPanel *GemPanelControlState;
    TEdit *ed2DBarCodeFilename;
    TPanel *Pal01;
    TSpeedButton *GemBtnSendTerminalMessage;
    TListBox *GemTerminalWindow;
    TEdit *GemTerminalSendEdit;
    TPanel *Pal02;
    TSpeedButton *GemListRemoteFileName;
    TSpeedButton *GemSelectAllRemoteFile;
    TSpeedButton *GemDisSelectAllRemoteFile;
    TSpeedButton *GemDownLoadRemoteFileName;
    TSpeedButton *GemDeleteRemoteFileName;
    TLabel *Label143;
    TSpeedButton *GemRefreshLocalFile;
    TSpeedButton *GemUpLoadLocalFileName;
    TSpeedButton *GemSelectAllLocalFile;
    TSpeedButton *GemDisSelectAllLocalFile;
    TCheckListBox *GemRemoteReceipeList;
    TCheckListBox *GemLocalFileLixtBox;
    TTabSheet *TabSheet5;
    TPanel *Pal03;
    TPageControl *ToolingSystem;
    TTabSheet *TabSheet7;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TSpeedButton *spGetSocketUseCount0;
    TSpeedButton *spInputSocketID1;
    TSpeedButton *spGetSocketUseCount1;
    TSpeedButton *spInputSocketID2;
    TSpeedButton *spGetSocketUseCount2;
    TSpeedButton *spInputSocketID3;
    TSpeedButton *spGetSocketUseCount3;
    TSpeedButton *spInputSocketID4;
    TSpeedButton *spGetSocketUseCount4;
    TSpeedButton *spInputSocketID5;
    TSpeedButton *spGetSocketUseCount5;
    TSpeedButton *spInputSocketID6;
    TSpeedButton *spGetSocketUseCount6;
    TSpeedButton *spInputSocketID7;
    TSpeedButton *spGetSocketUseCount7;
    TSpeedButton *spInputSocketID8;
    TSpeedButton *spGetSocketUseCount8;
    TSpeedButton *spInputSocketID9;
    TSpeedButton *spGetSocketUseCount9;
    TSpeedButton *spInputSocketID10;
    TSpeedButton *spGetSocketUseCount10;
    TSpeedButton *spInputSocketID11;
    TSpeedButton *spGetSocketUseCount11;
    TSpeedButton *spInputSocketID12;
    TSpeedButton *spGetSocketUseCount12;
    TSpeedButton *spInputSocketID13;
    TSpeedButton *spGetSocketUseCount13;
    TSpeedButton *spInputSocketID14;
    TSpeedButton *spGetSocketUseCount14;
    TSpeedButton *spInputSocketID15;
    TSpeedButton *spGetSocketUseCount15;
    TLabel *Label1;
    TSpeedButton *spInputSocketID0;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TSpeedButton *SpeedButton2;
    TEdit *edSocketID0;
    TEdit *edSocketID1;
    TEdit *edSocketID2;
    TEdit *edSocketID3;
    TEdit *edSocketID4;
    TEdit *edSocketID5;
    TEdit *edSocketCount0;
    TEdit *edSocketCount1;
    TEdit *edSocketCount2;
    TEdit *edSocketCount3;
    TEdit *edSocketCount4;
    TEdit *edSocketCount5;
    TButton *Button1;
    TEdit *edSocketID6;
    TEdit *edSocketID7;
    TEdit *edSocketID8;
    TEdit *edSocketID9;
    TEdit *edSocketID10;
    TEdit *edSocketID11;
    TEdit *edSocketCount6;
    TEdit *edSocketCount7;
    TEdit *edSocketCount8;
    TEdit *edSocketCount9;
    TEdit *edSocketCount10;
    TEdit *edSocketCount11;
    TEdit *edSocketID12;
    TEdit *edSocketID13;
    TEdit *edSocketID14;
    TEdit *edSocketID15;
    TEdit *edSocketCount12;
    TEdit *edSocketCount13;
    TEdit *edSocketCount14;
    TEdit *edSocketCount15;
    TEdit *edSocketUseLimit0;
    TEdit *edSocketUseLimit1;
    TEdit *edSocketUseLimit2;
    TEdit *edSocketUseLimit3;
    TEdit *edSocketUseLimit4;
    TEdit *edSocketUseLimit5;
    TEdit *edSocketUseLimit6;
    TEdit *edSocketUseLimit7;
    TEdit *edSocketUseLimit8;
    TEdit *edSocketUseLimit9;
    TEdit *edSocketUseLimit10;
    TEdit *edSocketUseLimit11;
    TEdit *edSocketUseLimit12;
    TEdit *edSocketUseLimit13;
    TEdit *edSocketUseLimit14;
    TEdit *edSocketUseLimit15;
    TCheckBox *chkHandlerBusy;
    TTabSheet *TabSheet8;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TSpeedButton *spInputLoadBoardID0;
    TSpeedButton *spGetLoadBoardUseCount0;
    TSpeedButton *spInputLoadBoardID1;
    TSpeedButton *spGetLoadBoardUseCount1;
    TSpeedButton *spInputLoadBoardID2;
    TSpeedButton *spGetLoadBoardUseCount2;
    TSpeedButton *spInputLoadBoardID3;
    TSpeedButton *spGetLoadBoardUseCount3;
    TSpeedButton *spInputLoadBoardID4;
    TSpeedButton *spGetLoadBoardUseCount4;
    TSpeedButton *spInputLoadBoardID5;
    TSpeedButton *spGetLoadBoardUseCount5;
    TSpeedButton *spInputLoadBoardID6;
    TSpeedButton *spGetLoadBoardUseCount6;
    TSpeedButton *spInputLoadBoardID7;
    TSpeedButton *spGetLoadBoardUseCount7;
    TLabel *Label28;
    TEdit *edLoadBoardID0;
    TEdit *edLoadBoardID1;
    TEdit *edLoadBoardID2;
    TEdit *edLoadBoardID3;
    TEdit *edLoadBoardID4;
    TEdit *edLoadBoardID5;
    TEdit *edLoadBoardCount0;
    TEdit *edLoadBoardCount1;
    TEdit *edLoadBoardCount2;
    TEdit *edLoadBoardCount3;
    TEdit *edLoadBoardCount4;
    TEdit *edLoadBoardCount5;
    TEdit *edLoadBoardID6;
    TEdit *edLoadBoardID7;
    TEdit *edLoadBoardCount6;
    TEdit *edLoadBoardCount7;
    TEdit *edLoadBoardLimitCount0;
    TEdit *edLoadBoardLimitCount1;
    TEdit *edLoadBoardLimitCount2;
    TEdit *edLoadBoardLimitCount3;
    TEdit *edLoadBoardLimitCount4;
    TEdit *edLoadBoardLimitCount5;
    TEdit *edLoadBoardLimitCount6;
    TEdit *edLoadBoardLimitCount7;
    TTabSheet *TabSheet9;
    TLabel *Label48;
    TLabel *Label49;
    TSpeedButton *spInputKitID0;
    TSpeedButton *spGetKitUseCount0;
    TLabel *Label29;
    TEdit *edKitID0;
    TEdit *edKitCount0;
    TEdit *edKitLimitCount0;
    TTabSheet *TabSheet10;
    TTabSheet *TabSheet11;
    TPanel *Pal04;
    TListBox *lbTemp1;
    TListBox *lbTemp2;
    TPanel *Pal06;
    TButton *Button2;
    TButton *Button3;
    TPanel *Pal07;
    TMemo *Memo1;
    TButton *Button4;
    TButton *Button5;
    TMemo *GemCommMemo;
    TTabSheet *tsCCode;
    TPanel *palPPIDCaption;
    TPanel *Panel2;
    TPanel *palCCodeList;
    TStringGrid *sgCCodeList;
    TPanel *Panel1;
    TPanel *palECIDList;
    TStringGrid *sgECIDList;
    TEdit *edAciveCCode;
    TLabel *Label30;
    TSpeedButton *spbNewCCode;
    TEdit *edCCodeAppend;
    TPanel *Panel3;
    TSpeedButton *apbUpdateCCode;
    TTimer *TimerSecsAlarm;
    TGroupBox *GroupBox1;
    TEdit *edSecsGemPassWord;
    TLabel *Label31;
    TSpeedButton *spEnter;
    TButton *btnS7F6;
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall TimerToolingTimer(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall spbNewCCodeClick(TObject *Sender);
    void __fastcall sgCCodeListMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sgECIDListMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall apbUpdateCCodeClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
    void __fastcall TimerSecsAlarmTimer(TObject *Sender);
    void __fastcall spEnterClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnS7F6Click(TObject *Sender);
private:    // User declarations
    void InitialECStringGrid();
    int iCurrentCCodeRow;
public:     // User declarations
    __fastcall TFSECS(TComponent* Owner);
    bool bGemInitial;       //Steven 20220511 : ½T»{¦³«Øºc
    void GemInitialDefault();
    void GemInitial(AnsiString HandlerType, AnsiString SoftwareVersion);
    int  MySFCode();
    bool bLotStartByHost;
    void __fastcall DoToolingMsg();        //13.06.25.01   klutter

    // 2013/05/8 lee start
    void SetToolingData(bool flag);
    TEdit *edSocketUseCount[16];
    TEdit *edLoadBoardUseCount[8];
    TSpeedButton *spGetSocketUseCount[16];
    TEdit *edSocketID[16];
    TEdit *edLoadBoardID[8];
    TSpeedButton *spInputSocketID[16];
    TSpeedButton *spGetLoadBoardUseCount[8];
    TSpeedButton *spInputLoadBoardID[8];
    TEdit *edSocketUseLimit[16];
    TEdit *edLoadBoardUseLimit[8];

//    int DoProcessHT3008ProductParameterReceive();
//    void InitialHT3008ProductParameter();
//    void SetSECSHT3008ProductParameter();
//    void DoSendHT3008ProductParameter();
//    void DoBackupHT3008ProductParameter();
//    void DoSendBackHT3008ProductParameter();
    // 2013/05/8 lee end

    /*
    TPageControl *pPageControl;
    TTabSheet    *pTabSht[10];
    TPanel *palP[10];
    AnsiString sTab[10];
    */
//    AnsiString CurrentDir;
    void __fastcall SECS_SETData();
    bool IsSupportRemoteCommand(AnsiString S);
    unsigned char __fastcall SECS_PublicRemoteCommand(AnsiString Command);
    void AddECInfoFromData(int ECID, TObject *P);
    void __fastcall ReadDefineCCode();
};
//---------------------------------------------------------------------------
extern PACKAGE TFSECS *FSECS;
extern void EventReport(unsigned Ceid);

extern bool SECS_GEM_PPMUSIC_CONTROL_flag;
extern int  iSECS_GEM_PPMUSIC_CONTROL_CLASS;

extern bool SECS_GEM_PPSIGNALTOWER_CONTROL_flag;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_RED;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW;
//---------------------------------------------------------------------------
#endif
