//---------------------------------------------------------------------------

#ifndef cAirConH
#define cAirConH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfAirCon : public TForm
{
__published:    // IDE-managed Components
    TComm *CommAirCon;
    TMemo *Memo1;
    TPanel *Panel1;
    TRadioGroup *rgAirConCommand;
    TSpeedButton *sbUpdate;
    TSpeedButton *sbtExit;
    TEdit *edOutputCurrent;
    TLabel *Label1;
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall CommAirConReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edOutputCurrentMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    void SendCommand(AnsiString Str);
    //JimmyChiu 20210922 Avoid overflow
    AnsiString LogMsg;
public:     // User declarations
    __fastcall TfAirCon(TComponent* Owner);

    void AddMessage(AnsiString Str);

    void __fastcall InitialAirConCom(AnsiString    ComPort,
                                     TParity       Parity      =None,
                                     unsigned      Rate        =9600,
                                     TByteSize     ByteSize    =_8,
                                     bool          ParityCheck =false,
                                     TStopBits     StopBits    =_1);

    enum eAirConCMD{eCLS   =0, //致能輸出電壓
                    ePwrOn =1, //開啟輸出電壓
                    ePwrOff=2, //關閉輸出電壓
                    ePwrSts=3, //回傳power supply狀態
                    eSetCur=4, //設定電壓電流值
                    eGetCur=5, //讀取電流
                    eCmdTotal
                   };

    void StartOutput(bool bEnable);     //開啟輸出電壓
    void SetCurrent(double dCurrent);   //設定電流
    void SetCLS();                      //致能輸出電壓
    void GetStatus();                   //讀取Power狀態
    void GetCurrent();                  //讀取電流

    void PowerUp(double dCurrent=0.1);  //啟動電源
    void PowerDown();                   //關閉電源
};
//---------------------------------------------------------------------------
extern PACKAGE TfAirCon *fAirCon;
//---------------------------------------------------------------------------
#endif
