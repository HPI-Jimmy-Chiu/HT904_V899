//---------------------------------------------------------------------------
#ifndef CCINPUTH
#define CCINPUTH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TCCInput : public TForm
{
__published:    // IDE-managed Components
     TButton *Button1;
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
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button17Click(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton4Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
     __fastcall TCCInput(TComponent* Owner);
     int Result;
     double fResult;
     bool bFirstEnter;
     AnsiString Back;
};
//---------------------------------------------------------------------------
extern PACKAGE TCCInput *CCInput;
//---------------------------------------------------------------------------
void MyCCInputBox(TEdit *Ptr);
#endif
