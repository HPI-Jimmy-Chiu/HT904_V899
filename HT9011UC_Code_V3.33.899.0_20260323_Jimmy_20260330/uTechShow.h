//---------------------------------------------------------------------------

#ifndef uTechShowH
#define uTechShowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfTeachShow : public TForm
{
__published:    // IDE-managed Components
    TLabel *Label1;
    TLabel *Label3;
    TButton *Button1;
    TButton *Button2;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfTeachShow(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTeachShow *fTeachShow;
//---------------------------------------------------------------------------
#endif
