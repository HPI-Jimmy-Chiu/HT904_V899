//---------------------------------------------------------------------------

#ifndef Socket_ASE_KRH
#define Socket_ASE_KRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfSocketCommunication : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TSpeedButton *OnLine;
    TGroupBox *GroupBox3;
    TButton *btnC1N1;
    TButton *btnC1N3;
    TButton *btnC6N1;
    TButton *btnC9N1;
    TButton *btnC9N3;
    TButton *btnC9N5;
    TClientSocket *ClientSocket1;
    TMemo *Memo;
    TTimer *Timer1;
    TCheckBox *cbEnableSocket;
    TGroupBox *gbSetting;
    TLabel *Label1;
    TEdit *edtServerIP_1;
    TLabel *Label2;
    TEdit *edtServerPort;
    TLabel *Label3;
    TEdit *edtSocketTimeout;
    TEdit *edtServerIP_2;
    TEdit *edtServerIP_3;
    TEdit *edtServerIP_4;
    TButton *btnOK;
    TButton *btnCancel;
    TSplitter *Splitter1;
    TButton *Button1;
    TButton *Button2;
    void __fastcall btnC1N1Click(TObject *Sender);
    void __fastcall btnC1N3Click(TObject *Sender);
    void __fastcall btnC6N1Click(TObject *Sender);
    void __fastcall btnC9N1Click(TObject *Sender);
    void __fastcall btnC9N3Click(TObject *Sender);
    void __fastcall btnC9N5Click(TObject *Sender);
    void __fastcall ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ClientSocket1Connecting(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall edtServerPortClick(TObject *Sender);
    void __fastcall edtServerIP_1Click(TObject *Sender);
    void __fastcall edtSocketTimeoutClick(TObject *Sender);
private:    // User declarations
    //TMemoryStream *mReadData; //ChungHung 20130204 delete
    AnsiString ReadData; //ChungHung 20130204 add

    bool bStartCount;
    int iInitial;
    int iBinMap;
    int iEnd;

    bool bConnecting;

    bool bDisconnected;

    //ChungHung 20130204 add
    bool __fastcall GetDataFinish(int &Category,int &Num);
    bool __fastcall CommandProcess();

    //ChungHung 20130204 delete
    //bool __fastcall GetDataFinish(byte &Category,byte &Num);
    //bool __fastcall CommandProcess(byte Category,byte Num);

    bool bWrongHandlerID;
    bool bWrongFormat;
    bool bUnknownMessage;

    bool bDoStopOrOneCycle;

public:     // User declarations
    __fastcall TfSocketCommunication(TComponent* Owner);
    bool __fastcall Socket_ASE_KR_Open(AnsiString Ip,AnsiString Port,bool bshow=false);

    AnsiString aSetHandlerState; //設定的狀態
    AnsiString aCurrentHandlerState; //目前狀態
    bool bStopShowAlarm;
    void __fastcall InitialASEKrSocket();
    bool __fastcall InitialzedHandler();
    void __fastcall InitBinMapReport();
    bool __fastcall BinMapReport();
    void __fastcall InitSendCurrentStatus();
    bool __fastcall SendCurrentStatus();
    void __fastcall SetHandlerState(int state); //0:DOWN 1:RUN 2:IDLE

    bool __fastcall GetCategoryAndNum(int &Category,int &Num);

    bool __fastcall C1N1(); //Handler first time open or socket setting change
    bool __fastcall C1N3();
    bool __fastcall C6N1();
    bool __fastcall C9N1();
    bool __fastcall C9N3();
    bool __fastcall C9N5();
    //ChungHung 20130204 add
    void __fastcall ShowMemo(AnsiString data,bool bState=false);
    //ChungHung 20130204 delete
    //void __fastcall ShowMemo(TMemoryStream* data,bool bState=false);  //false:send true:receive

    //ChungHung 20130204 add
    AnsiString HandlerID;
    AnsiString ReceivedHandler;

    //ChungHung 20130204 delete
    //AnsiString TesterID;

    bool __fastcall IsUnknownMessage(int Category,int Num);
    bool __fastcall IsWrongFormat(int Category,int Num);
    bool __fastcall IsWrongHandlerID();
    void __fastcall GetErrorMSG();
    bool __fastcall GetACK();

    bool bWaitServerRely;
    bool bNeedShow;
    bool bOneCycleFormServerButNoRunJustShow;
    bool bOneCycleFormServer;
    bool bStopFormServer;
    bool bSendBinMapReport;
    //bool  bOneCycleForWhatsCommand;
    AnsiString ErrorMessage;

    int iBackCategory;
    int iBackNum;

    bool bTryToConnect;
};
//---------------------------------------------------------------------------
extern PACKAGE TfSocketCommunication *fSocketCommunication;
//---------------------------------------------------------------------------
#endif
