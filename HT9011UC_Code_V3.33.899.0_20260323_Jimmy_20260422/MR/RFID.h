//---------------------------------------------------------------------------

#ifndef RFIDH
#define RFIDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "SPComm.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#define RFID_COUNT 2
#include "Delta.h"
//---------------------------------------------------------------------------
class TfRFID : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsRFID;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TCheckBox *cbEnableRFID;
    TEdit *edRFIDScanDelayTime;
    TLabel *Label395;
    TComm *RFID_1;
    TComm *RFID_2;
    TGroupBox *gbRFIDTest;
    TGroupBox *gb1;
    TCheckBox *cbCommReader1;
    TComboBox *coCommRFID1;
    TButton *btRFID1On;
    TSpeedButton *spbResetCom;
    TSpeedButton *spbStartCom;
    TGroupBox *GroupBox1;
    TCheckBox *CheckBox1;
    TComboBox *coCommRFID2;
    TButton *btRFID2On;
    TMemo *Memo1;
    TLabel *labRetryCount;
    TEdit *edRFIDRetryCount;
    TCheckBox *cbRFIDSimulate;
    TTabSheet *tsE84;
    TGroupBox *gbE84TimeOut;
    TLabel *labTP1;
    TEdit *edTP1;
    TEdit *edTP2;
    TLabel *labTP2;
    TEdit *edTP3;
    TLabel *labTP3;
    TEdit *edTP4;
    TLabel *labTP4;
    TEdit *edTP5;
    TLabel *labTP5;
    TEdit *edTP6;
    TLabel *labTP6;
    TGroupBox *gbE84Log;
    TMemo *Memo3;
    TTabSheet *tsDelta;
    TEdit *Edit1;
    TLabel *Label1;
    TMemo *Memo2;
    TEdit *ReqData;
    TEdit *ResData;
    TEdit *ActStatus;
    TLabel *Label11;
    TLabel *Label10;
    TLabel *Label9;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit7;
    TEdit *Edit8;
    TEdit *Edit10;
    TEdit *Edit11;
    TEdit *Edit13;
    TEdit *Edit14;
    TEdit *Edit16;
    TEdit *Edit17;
    TEdit *Edit19;
    TEdit *Edit20;
    TEdit *Edit22;
    TEdit *Edit23;
    TEdit *Edit25;
    TEdit *Edit26;
    TEdit *Edit28;
    TEdit *Edit29;
    TEdit *Edit31;
    TEdit *Edit32;
    TEdit *Edit3;
    TEdit *Edit6;
    TEdit *Edit9;
    TEdit *Edit12;
    TEdit *Edit15;
    TEdit *Edit18;
    TEdit *Edit21;
    TEdit *Edit24;
    TEdit *Edit27;
    TEdit *Edit30;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TCheckBox *cbEnableView;
    TEdit *Edit34;
    TEdit *Edit35;
    TEdit *Edit36;
    TTimer *Timer1;
    TGroupBox *GroupBox2;
    TButton *Button1;
    TButton *Button3;
    TButton *Button4;
    TButton *Button2;
    TButton *Button9;
    void __fastcall spbResetComClick(TObject *Sender);
    void __fastcall spbStartComClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btRFID1OnClick(TObject *Sender);
    void __fastcall RFID_1ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall RFID_2ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Button9Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfRFID(TComponent* Owner);
    TComm  *RFIDCOM              [RFID_COUNT];
    void __fastcall InitRFIDRS232();
    bool fShow;
    AnsiString DoRFIDAnalysis(AnsiString str);
    void DoRFIDReader(int Number);
    bool bRFIDReaderOK[2];
    void __fastcall DoView(int iFunction, int channel);
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    void __fastcall DoE84Load();
    void __fastcall DoE84UnLoad();
    void __fastcall DoCheckViewChange();
    DeltaPLC DeltaCortol;
    bool bRun;
};
//---------------------------------------------------------------------------
extern PACKAGE TfRFID *fRFID;
//---------------------------------------------------------------------------
#endif
