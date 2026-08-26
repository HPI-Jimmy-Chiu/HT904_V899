//---------------------------------------------------------------------------
#ifndef INPUTH
#define INPUTH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfInput : public TForm
{
__published:    // IDE-managed Components
     TButton *Button1;
     TEdit *Edit1;
     TButton *Button2;
     TButton *Button3;
     TButton *Button5;
     TButton *Button6;
     TButton *Button8;
     TButton *Button9;
     TButton *Button11;
     TButton *Button12;
     TButton *Button15;
     TButton *Button17;
     TButton *Button18;
    TButton *Button13;
    TButton *Button4;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TSpeedButton *SpeedButton5;
    TSpeedButton *SpeedButton6;
    TSpeedButton *SpeedButton7;
    TLabel *Label1;
    void __fastcall Button15Click(TObject *Sender);
    void __fastcall NumberClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Button13Click(TObject *Sender);
    void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button17Click(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton4Click(TObject *Sender);
    void __fastcall Button17KeyPress(TObject *Sender, char &Key);
    void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
public:     // User declarations
     __fastcall TfInput(TComponent* Owner);
     int Result;
     double fResult;
     bool bFirstEnter;
     AnsiString Back;
     bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfInput *fInput;
//---------------------------------------------------------------------------
void MyInputBox(TEdit *Ptr);
//void MyInputBox(AnsiString &Str);
void MyFloatInputBox(TEdit *Ptr);
void MyFloatInputBox2(TEdit *Ptr); //wei 20150708 直接顯示小數點後兩位
void MyPassWordInputBox(TEdit *Ptr);
void MyDoubleInputBox(TEdit *Ptr);
void MyTempFloatInputBox(TEdit *Ptr);
void MySuperInputBox(TEdit *Ptr, double base, double Max, double Min);   //Steven 20120209
#endif
