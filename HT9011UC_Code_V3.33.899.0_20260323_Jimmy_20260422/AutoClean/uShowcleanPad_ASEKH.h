//---------------------------------------------------------------------------

#ifndef uShowcleanPad_ASEKHH
#define uShowcleanPad_ASEKHH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TCleanPadFrom : public TForm
{
__published:    // IDE-managed Components
        TImage *Image1;
        TSpeedButton *SpeedButton1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
        __fastcall TCleanPadFrom(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCleanPadFrom *CleanPadFrom;
//---------------------------------------------------------------------------
#endif
