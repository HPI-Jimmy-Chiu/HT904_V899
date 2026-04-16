//---------------------------------------------------------------------------

#ifndef LoadCCDMapH
#define LoadCCDMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>
#include "aled.hpp"
#include "ALed.hpp"
#include "MyLed.h"

//---------------------------------------------------------------------------
class TLoadCCD : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TEdit *edtPort;
    TServerSocket *ServerSocket1;
    TButton *btnConnect;
    TLabel *Label1;
    TMemo *Memo1;
    TMyLed *LedAConnect;
    TButton *CCD1;
    TButton *CCD2;
    TTimer *Timer1;
    TButton *btnCCDTest;
    void __fastcall btnConnectClick(TObject *Sender);
    void __fastcall ServerSocket1ClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall CCD1Click(TObject *Sender);
    void __fastcall CCD2Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnCCDTestClick(TObject *Sender);
    void __fastcall edtPortClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TLoadCCD(TComponent* Owner);
    void SendData(AnsiString sData);

    char pucInBuff[8192];                   //kevin 20220319 add
    AnsiString CCDCommand[5];               //送給CCD 拍照 命令
    bool InitialLoadCCD();                  //initial
    int LoadCCDData();                      //load CCD Data
    int iTrayMappingDate[35][70];
    bool bReceiv;                           //收到CCD 資料
    int CCDTask;
    int iICCount;                           //CCD IC 有多少顆
    bool bLoadServoSocketConnect;
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadCCD *LoadCCD;
//---------------------------------------------------------------------------
#endif
