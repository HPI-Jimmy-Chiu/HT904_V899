//---------------------------------------------------------------------------

#ifndef cDefrostNoteH
#define cDefrostNoteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfDefrostNote : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel11;
    TPanel *pnl_ShowMessage;
    TButton *btn_Start;
    TTimer *Timer1;
    TButton *btn_Interrupt;
    void __fastcall btn_StartClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btn_InterruptClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:    // User declarations
public:        // User declarations
    __fastcall TfDefrostNote(TComponent* Owner);
    bool bStartDefrost;
    bool bInterruptDefrost;
    bool bShow;
    AnsiString sShowMessage;
};
//---------------------------------------------------------------------------
extern PACKAGE TfDefrostNote *fDefrostNote;
//---------------------------------------------------------------------------
#endif
