//---------------------------------------------------------------------------

#ifndef InputFormH
#define InputFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfInputForm : public TForm
{
__published:    // IDE-managed Components
    TLabel *Label1;
    TEdit *Edit1;
    TButton *Button1;
    TEdit *Edit2;
    TEdit *Edit3;
    TImage *Image1;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Edit1Click(TObject *Sender);
    void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
private:    // User declarations
public:     // User declarations
    __fastcall TfInputForm(TComponent* Owner);
    bool bTrayIDclose;
};
//---------------------------------------------------------------------------
extern PACKAGE TfInputForm *fInputForm;
//---------------------------------------------------------------------------

int ShowMyInput(AnsiString aCaption,AnsiString aString);
AnsiString ShowMyInput1(AnsiString aCaption,AnsiString aString);
AnsiString ShowMyInputSkip(AnsiString aCaption,AnsiString aString);
AnsiString ShowMyInputTrayID(AnsiString aCaption,AnsiString aString);
#endif
