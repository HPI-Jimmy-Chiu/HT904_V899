//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ObserveMagazine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfObserveMagazine *fObserveMagazine;
//---------------------------------------------------------------------------
__fastcall TfObserveMagazine::TfObserveMagazine(TComponent* Owner)
        : TForm(Owner)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfObserveMagazine::FormShow(TObject *Sender)
{
    fShow = true;
}
//---------------------------------------------------------------------------
void __fastcall TfObserveMagazine::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
