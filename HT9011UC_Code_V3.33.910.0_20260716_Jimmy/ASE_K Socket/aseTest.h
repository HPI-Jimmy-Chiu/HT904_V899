//---------------------------------------------------------------------------

#ifndef aseTestH
#define aseTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
#include <NMUDP.hpp>

#include <vector>
#include <string>
#include <map>

#define ASEStationCount  2

//---------------------------------------------------------------------------
class TASESendMessage : public TForm
{
__published:    // IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *Commucation;
        TTabSheet *Setup;
        TEdit *IPAddress1;
        TLabel *Label1;
        TLabel *Label24;
        TEdit *IPPort1;
        TLabel *Label2;
        TButton *Connect;
        TButton *SaveData;
        TMemo *Memo1;
    TButton *btnTest;
    TEdit *edtTestMsg;
        TButton *Button2;
        TLabel *Label3;
        TEdit *IPPort2;
        TNMUDP *NMUDP1;
    TEdit *edtRecvMsg;
    TButton *btnTestRecv;
        TNMUDP *NMUDP2;
        void __fastcall SaveDataClick(TObject *Sender);
        void __fastcall ConnectClick(TObject *Sender);
        void __fastcall btnTestClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall NMUDP1DataReceived(TComponent *Sender,
          int NumberBytes, AnsiString FromIP, int Port);
        void __fastcall btnTestRecvClick(TObject *Sender);
    void __fastcall IPPort2Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
        __fastcall TASESendMessage(TComponent* Owner);
        void InitialDataToForm();
//        TStringList *SocketReceiveLength;
        bool SendToASEData(AnsiString Data);//送資料給ASE
        bool ReceveToHandleData(AnsiString Data);//接收ASE資料給Handle
        bool initial();
        bool WriteData();
        bool CompileWord(AnsiString sData,AnsiString iObject);
        bool SendFormData(AnsiString Data);
        bool WriteATCDataToASE(); //kevin 20200414 ATC data
        AnsiString ReceviceData[10];
        bool bASE_AutoClean; //kevin 20160722 遠端啟動 autoclean
};
//---------------------------------------------------------------------------
extern PACKAGE TASESendMessage *ASESendMessage;
//---------------------------------------------------------------------------
struct LAST_GENERAL_SET1
{
    AnsiString IP_ADDR[2];
    int  Port[2];
    int  ScanInterval;

    LAST_GENERAL_SET1();
} ;
//===========================================================================
LAST_GENERAL_SET1:: LAST_GENERAL_SET1()
{
//    MachName="";
}
//===========================================================================
typedef struct
{
    char Schedule[50];
    int RevInQty; //數量
    bool RevCleanCount;//清除所有計數

    int SendBinData[6];// 0:Auto1 1:Auto2 2:Auto3  3:Fix1  4:Fix2 5:Fix3
}Handle_ASECommute;
extern Handle_ASECommute ASECommute;
#endif
