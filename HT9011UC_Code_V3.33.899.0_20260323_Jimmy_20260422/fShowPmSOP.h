//---------------------------------------------------------------------------

#ifndef fShowPmSOPH
#define fShowPmSOPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TfShowPmSOPForm : public TForm
{
__published:    // IDE-managed Components
    TCppWebBrowser *cpwbrwsrPmSOP;
    void __fastcall FormShow(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfShowPmSOPForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfShowPmSOPForm *fShowPmSOPForm;
//---------------------------------------------------------------------------

extern AnsiString sPathPmSOP;   //KaiChen 20171110¡G¶WÂ× Show PM SOP

#endif
