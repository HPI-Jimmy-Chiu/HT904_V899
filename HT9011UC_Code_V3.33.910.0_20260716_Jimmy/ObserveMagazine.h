//---------------------------------------------------------------------------

#ifndef ObserveMagazineH
#define ObserveMagazineH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "HTray.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfObserveMagazine : public TForm
{
__published:    // IDE-managed Components
        TTMyTray *mtMagazineTray14;
        TTMyTray *mtMagazineTray13;
        TTMyTray *mtMagazineTray12;
        TTMyTray *mtMagazineTray11;
        TTMyTray *mtMagazineTray10;
        TTMyTray *mtMagazineTray9;
        TTMyTray *mtMagazineTray8;
        TTMyTray *mtMagazineTray7;
        TTMyTray *mtMagazineTray6;
        TTMyTray *mtMagazineTray5;
        TTMyTray *mtMagazineTray4;
        TTMyTray *mtMagazineTray3;
        TTMyTray *mtMagazineTray2;
        TTMyTray *mtMagazineTray1;
        TLabel *labSimMagazineTrayNo;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
public:        // User declarations
        bool fShow;
        __fastcall TfObserveMagazine(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfObserveMagazine *fObserveMagazine;
//---------------------------------------------------------------------------
#endif
