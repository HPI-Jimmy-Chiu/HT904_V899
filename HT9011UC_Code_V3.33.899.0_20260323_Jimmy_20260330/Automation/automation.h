//---------------------------------------------------------------------------

#ifndef automationH
#define automationH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <StdCtrls.hpp>
#include <Mask.hpp>
#include <NMUDP.hpp>
//---------------------------------------------------------------------------
typedef class MyProcessBuffer
{
    public:
        AnsiString Command;
        AnsiString SubCommand;
        AnsiString AlarmID;
        AnsiString AlarmMessage;
        AnsiString TimeString;
        int        ErrType;
}ProcessBuffer;
//---------------------------------------------------------------------------
class TfAutomation : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgcOLP;
    TTabSheet *TabSheet20;
    TMemo *Memo;
    TTabSheet *TabSheet6;
    TMemo *MemoChar;
    TTabSheet *TabSheet22;
    TMemo *MemoCode;
    TServerSocket *OLPServer;
    TTimer *tmrOLP;
    TClientSocket *OLPClient;
    TNMUDP *NMUDP1;
    TButton *btUDPSend;
    TGroupBox *gbAutomation;
    TSpeedButton *OnLine;
    TSpeedButton *OnLine2;
    TComboBox *cbbOLPCommand;
    TButton *btEventReport;
    TButton *btAlarmReport;
    TCheckBox *chkViewComm;
    TButton *btnConnect;
    TButton *btnDisconnect;
    TButton *btTestResult;
    TButton *btUDPMap;
    TButton *btUDPTemp;
    TPanel *Panel1;
    TLabel *lbIP;
    TLabel *lbPORT;
    TEdit *edinputIP;
    TEdit *edinputport;
    void __fastcall OLPServerClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall OLPServerClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall OLPServerClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall OLPServerClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall tmrOLPTimer(TObject *Sender);
    void __fastcall btnConnectClick(TObject *Sender);
    void __fastcall btnDisconnectClick(TObject *Sender);
    void __fastcall btEventReportClick(TObject *Sender);
    void __fastcall btAlarmReportClick(TObject *Sender);
    void __fastcall MemoDblClick(TObject *Sender);
    void __fastcall btTestResultClick(TObject *Sender);
    void __fastcall OLPClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall OLPClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall OLPClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall OLPClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall OLPClientConnecting(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall NMUDP1DataReceived(TComponent *Sender, int NumberBytes,
          AnsiString FromIP, int Port);
    void __fastcall btUDPSendClick(TObject *Sender);
    void __fastcall btUDPTempClick(TObject *Sender);
    void __fastcall btUDPMapClick(TObject *Sender);
    void __fastcall edinputportMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    Word aSystemHour, aSystemMin, aSystemSec, aSystemMSec;
    Word aSystemYear, aSystemMonth, aSystemDate;
    bool SendInvalid;
    void __fastcall ShowCharHexATK(bool bRead, char *Message, int Length);
    bool OLPClientConnected;
    void ShowMSG(AnsiString asStr);
    bool brecordmsgLock;
public:     // User declarations
    __fastcall TfAutomation(TComponent* Owner);
    void __fastcall ShowCharHex(AnsiString S);
    void __fastcall ShowRecord(bool bRead, AnsiString S,int iHandle);
    void __fastcall ProcessBuffer1();
    void __fastcall ProcessBuffer(AnsiString Buffer,int iHandle);
    void __fastcall WriteTCPDataToTextFile();
    void __fastcall SendClient(AnsiString S, AnsiString S2,int iHandle);
    void __fastcall SendServer(AnsiString S, AnsiString S2);
    void __fastcall SendReportRequest(AnsiString ReportID,bool Standard=true);
    void __fastcall CommandProcess(AnsiString CMD, AnsiString V_TOTAL, AnsiString *Data , bool Standard=true,int iHandle=0); //被動回傳
    void __fastcall DoCommandBuffer(AnsiString Command, AnsiString SubCommand=NULL, AnsiString AlarmMessage=NULL, int ErrType=0, AnsiString AlarmID=NULL); //主動回傳
    AnsiString  __fastcall GetTimeInfo();
    bool bReceive;
    bool bReceive2;
    int iSocketHandle;  //Sam 20200727 : 修正連線問題
    bool bReceiveHasData;       // 2011.09.05 , Joye , OLP
    TList *CommandBuffer;
    TStringList *CommandClientReplyBuffer;     //Sam 20200813 : Client 收到資料回傳若遇到斷線將資料丟到 Buffer
    int TestMode;

    bool GetEventNum(int &num,AnsiString cmdstr,AnsiString numstr);
    bool SetEventResult(int num,bool def,AnsiString cmdstr,AnsiString numstr);
    bool GetEventResult(int num);
    int GetEventStrResult(int num);
    void CompareEvent(AnsiString str);
    void ClearEvent(int num);
    bool bOneCycle;
    bool bCleanOut;
    int iClientReConnect;   //Sam 20240226 : Client 重連次數
//    MyProcessBuffer *Pbuffer; //Steven 20120102 : 改為全域變數
    MyProcessBuffer *P1;    //2012-10-26 Dell
    char sATKSendMessage[256];  //Steven 20120330 : ATK Site Map Monitorning
    int  sATKSendMessageCount;  //Steven 20120330 : ATK Site Map Monitorning
    void AmkorSendMessage(int Type);    //Steven 20120330 : ATK Site Map Monitorning
    char sATKSendTemp[256];
    char sATKSendMap[256];
    char sATKSendTempAndMap[256];
    void __fastcall SckSendAlarmCode(AnsiString Text);  //ChungHung 20150518 add for SCK Send JamCode By TcpIp
    void __fastcall SaveRecord();       //Sam 20200826 : 改為 Public //Steven 20141229 : OLP的Log要存檔
    void SendOLPClient(AnsiString Text);
    //HTML JSON
    AnsiString GetHTMLJSONDatas();
    AnsiString GetMachineStatus();
    AnsiString GetWorkOrder();
    AnsiString GetMainTemp();
    void GetTemperatures(double temps[10]);
};
//---------------------------------------------------------------------------
extern PACKAGE TfAutomation *fAutomation;
//---------------------------------------------------------------------------
#endif
