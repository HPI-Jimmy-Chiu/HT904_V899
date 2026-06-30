//---------------------------------------------------------------------------

#ifndef TesterTCPH
#define TesterTCPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>

#include "MachineType.h"
//---------------------------------------------------------------------------
class TfTesterTCP : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgcTcpIp;
    TTabSheet *tsMain;
    TTimer *TimerTCPIPConnect;
    TClientSocket *ClientSocket_TCPIP;
    TTimer *TimerProcessTCPData;
    TMemo *mmTCPIPCommLog;
    TTabSheet *tsSetting;
    TGroupBox *gbTCPIP;
    TLabel *Label23;
    TLabel *Label24;
    TEdit *edTCPIP_Address;
    TEdit *edTCPIP_Port;
    TBitBtn *btTCPIP_Connect;
    TBitBtn *btTCPIP_Disconnect;
    TButton *btTCPIP_Trigger;
    TEdit *edTCPIP_CMD;
    TGroupBox *gbSite;
    TLabel *labOcr01;
    TComboBox *cbSite01;
    TCheckBox *cbSiteOn01;
    TPanel *plSite01;
    TGroupBox *GroupBox1;
    TLabel *labOcr02;
    TComboBox *cbSite02;
    TCheckBox *cbSiteOn02;
    TPanel *plSite02;
    TGroupBox *GroupBox2;
    TLabel *labOcr03;
    TComboBox *cbSite03;
    TCheckBox *cbSiteOn03;
    TPanel *plSite03;
    TGroupBox *GroupBox3;
    TLabel *labOcr06;
    TComboBox *cbSite06;
    TCheckBox *cbSiteOn06;
    TPanel *plSite06;
    TGroupBox *GroupBox4;
    TLabel *labOcr05;
    TComboBox *cbSite05;
    TCheckBox *cbSiteOn05;
    TPanel *plSite05;
    TGroupBox *GroupBox5;
    TLabel *labOcr04;
    TComboBox *cbSite04;
    TCheckBox *cbSiteOn04;
    TPanel *plSite04;
    TGroupBox *GroupBox6;
    TLabel *labOcr08;
    TComboBox *cbSite08;
    TCheckBox *cbSiteOn08;
    TPanel *plSite08;
    TGroupBox *GroupBox7;
    TLabel *labOcr07;
    TComboBox *cbSite07;
    TCheckBox *cbSiteOn07;
    TPanel *plSite07;
    TGroupBox *GroupBox8;
    TLabel *labOcr09;
    TComboBox *cbSite09;
    TCheckBox *cbSiteOn09;
    TPanel *plSite09;
    TGroupBox *GroupBox9;
    TLabel *labOcr10;
    TComboBox *cbSite10;
    TCheckBox *cbSiteOn10;
    TPanel *plSite10;
    TGroupBox *GroupBox10;
    TLabel *labOcr11;
    TComboBox *cbSite11;
    TCheckBox *cbSiteOn11;
    TPanel *plSite11;
    TGroupBox *GroupBox11;
    TLabel *labOcr14;
    TComboBox *cbSite14;
    TCheckBox *cbSiteOn14;
    TPanel *plSite14;
    TGroupBox *GroupBox12;
    TLabel *labOcr13;
    TComboBox *cbSite13;
    TCheckBox *cbSiteOn13;
    TPanel *plSite13;
    TGroupBox *GroupBox13;
    TLabel *labOcr12;
    TComboBox *cbSite12;
    TCheckBox *cbSiteOn12;
    TPanel *plSite12;
    TGroupBox *GroupBox14;
    TLabel *labOcr16;
    TComboBox *cbSite16;
    TCheckBox *cbSiteOn16;
    TPanel *plSite16;
    TGroupBox *GroupBox15;
    TLabel *labOcr15;
    TComboBox *cbSite15;
    TCheckBox *cbSiteOn15;
    TPanel *plSite15;
    TGroupBox *GroupBox16;
    TLabel *labOcr17;
    TComboBox *cbSite17;
    TCheckBox *cbSiteOn17;
    TPanel *plSite17;
    TGroupBox *GroupBox17;
    TLabel *labOcr18;
    TComboBox *cbSite18;
    TCheckBox *cbSiteOn18;
    TPanel *plSite18;
    TGroupBox *GroupBox18;
    TLabel *labOcr19;
    TComboBox *cbSite19;
    TCheckBox *cbSiteOn19;
    TPanel *plSite19;
    TGroupBox *GroupBox19;
    TLabel *labOcr22;
    TComboBox *cbSite22;
    TCheckBox *cbSiteOn22;
    TPanel *plSite22;
    TGroupBox *GroupBox20;
    TLabel *labOcr21;
    TComboBox *cbSite21;
    TCheckBox *cbSiteOn21;
    TPanel *plSite21;
    TGroupBox *GroupBox21;
    TLabel *labOcr20;
    TComboBox *cbSite20;
    TCheckBox *cbSiteOn20;
    TPanel *plSite20;
    TGroupBox *GroupBox22;
    TLabel *labOcr24;
    TComboBox *cbSite24;
    TCheckBox *cbSiteOn24;
    TPanel *plSite24;
    TGroupBox *GroupBox23;
    TLabel *labOcr23;
    TComboBox *cbSite23;
    TCheckBox *cbSiteOn23;
    TPanel *plSite23;
    TGroupBox *GroupBox24;
    TLabel *labOcr25;
    TComboBox *cbSite25;
    TCheckBox *cbSiteOn25;
    TPanel *plSite25;
    TGroupBox *GroupBox25;
    TLabel *labOcr26;
    TComboBox *cbSite26;
    TCheckBox *cbSiteOn26;
    TPanel *plSite26;
    TGroupBox *GroupBox26;
    TLabel *labOcr27;
    TComboBox *cbSite27;
    TCheckBox *cbSiteOn27;
    TPanel *plSite27;
    TGroupBox *GroupBox27;
    TLabel *labOcr30;
    TComboBox *cbSite30;
    TCheckBox *cbSiteOn30;
    TPanel *plSite30;
    TGroupBox *GroupBox28;
    TLabel *labOcr29;
    TComboBox *cbSite29;
    TCheckBox *cbSiteOn29;
    TPanel *plSite29;
    TGroupBox *GroupBox29;
    TLabel *labOcr28;
    TComboBox *cbSite28;
    TCheckBox *cbSiteOn28;
    TPanel *plSite28;
    TGroupBox *GroupBox30;
    TLabel *labOcr32;
    TComboBox *cbSite32;
    TCheckBox *cbSiteOn32;
    TPanel *plSite32;
    TGroupBox *GroupBox31;
    TLabel *labOcr31;
    TComboBox *cbSite31;
    TCheckBox *cbSiteOn31;
    TPanel *plSite31;
    TTabSheet *tsTestResult;
    TPanel *pnlLeft;
    TGroupBox *grpTempData;
    TMemo *SourceMemo;
    TGroupBox *grpSummary;
    TRichEdit *redtSummary;
    TTabSheet *tsTestResult2;
    TScrollBox *scrlbxSummary;
    TRadioGroup *rgUnloader;

    TGroupBox *grpTrayFunc_OS;
    TCheckBox *chkAuto1_OS;
    TLabel *lblTrayFunc_OS;
    TCheckBox *chkAuto2_OS;
    TCheckBox *chkAuto3_OS;
    TButton *btnSave;
    void __fastcall ClientSocket_TCPIPConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_TCPIPDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerTCPIPConnectTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btTCPIP_TriggerClick(TObject *Sender);
    void __fastcall btTCPIP_ConnectClick(TObject *Sender);
    void __fastcall btTCPIP_DisconnectClick(TObject *Sender);
    void __fastcall ClientSocket_TCPIPRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerProcessTCPDataTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ClientSocket_TCPIPError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall rgUnloaderClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
private:    // User declarations
    AnsiString sTCPIPRecevieData;                                               //Ifor 20151224 add CCD Bar Code
    bool bFlag[eTrayCount];                                                     //RogerYang 20260128 : 田揚志畫面修改需求
public:     // User declarations
    __fastcall TfTesterTCP(TComponent* Owner);
    TStringList *SocketTCPIPReceiveList;
    bool bEnableTCPIPChannelConnect;
    bool bConnectOK;
    bool bConnect;
    bool fShow;
    void SendTCPIPCommand(int index, AnsiString Msg, AnsiString Msg2="");
    void AddTCPIPCommunicationLog(int index, AnsiString Str);
    void SimulateBin();
    bool SimulateStart;
    TMemo *RichEdit[eTrayCount];
    TGroupBox *GroupBox[eTrayCount];
    TStringList *SummaryHead;

    TComboBox *cbSimulateBin[32];
    TPanel *plSite[32];
    TCheckBox *cbSiteOn[32];
    TLabel *labOcr[32];
    AnsiString ErrorMessage;
    bool bTCPError;
    void __fastcall CopyOSTestResult(int iSuckRow, int iSuckCol);               //Steven 20250515 : 整合Open Short測試報表
    void __fastcall PlaceOSTestResultToTray(int iSuckRow, int iSuckCol, int iTrayRow, int iTrayCol, int iAuto);
    void __fastcall ProcessOSPrint(bool bViewOnly=false);
    void __fastcall ProcessOSTrayData(bool bViewOnly=false);
    //AI(ht9045-v899) 20260625: 回傳型別 void->bool，配合 N06 recipe 同步成敗回報
    bool __fastcall CopyRecipeToTester(AnsiString FileName);
    bool __fastcall CopyRecipeFromTester(AnsiString FileName);
    AnsiString OSRecipe;                                                        //Steven 20230116 : OS測試機傳送工作檔名
    bool bOutArmVariAuto_OS[3];                                                 //RogerYang 20260210 : 田揚志需求，有設定的才可以一次放全部，OS報表要照Tray盤放料順序顯示
    AnsiString LastFileName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfTesterTCP *fTesterTCP;
//---------------------------------------------------------------------------
#endif
