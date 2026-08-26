//---------------------------------------------------------------------------

#ifndef ArmOffsetDataH
#define ArmOffsetDataH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include "MachineType.h"
//---------------------------------------------------------------------------
class TfArmOffsetData : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgcArmOffset;
    TTabSheet *tsInArm;
    TTabSheet *tsOutArm;
    TPanel *pnlInArm;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TPanel *pnlOutArm;
    TLabel *Label13;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TPanel *Panel1;
    TLabel *Label8;
    TPanel *palOffsetParts;
    TLabel *Label27;
    TPanel *Panel2;
    TButton *btnSave;
    TButton *btnExit;
    void __fastcall btnOutArmExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    TEdit *SGUse;

    enum eOffsetEdit
    {
        eOfsEditX=0,
        eOfsEditY,
        eOfsEditPlace,
        eOfsEditPick,
        eOfsEditPitchX1,
        eOfsEditPitchX2,
        eOfsEditPitchX3,
        eOfsEditPitchX4,
        eOfsEditPitchY,
        eOfsEditTotal
    };

    TEdit *MyEdit[OneByOneOfsTotal][eOfsEditTotal];
    AnsiString LastFileName;
    void __fastcall EditOffsetXYClick(TObject *Sender);
    void __fastcall EditOffsetZClick(TObject *Sender);
    void __fastcall SaveOffSet(bool ArmType);
public:     // User declarations
    bool fShow;
    __fastcall TfArmOffsetData(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfArmOffsetData *fArmOffsetData;
//---------------------------------------------------------------------------
#endif
