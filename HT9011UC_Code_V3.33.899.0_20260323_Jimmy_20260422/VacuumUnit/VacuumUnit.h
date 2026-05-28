//---------------------------------------------------------------------------

#ifndef VacuumUnitH
#define VacuumUnitH
//---------------------------------------------------------------------------
#include "MyVacuumPanel.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ALed.hpp"
#include "BtnPanelLane.h"
#include "MyLedLane.h"
#include "VacuumUnit.h"
#include <Buttons.hpp>
#include "MyLed.h"
//---------------------------------------------------------------------------
const int TOTAL_VACUUM_UNIT=8;
const int VACUUM_UNIT_WIDTH=81;
const int VACUUM_UNIT_HEIGHT=177;
class TfVacuumUnit : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgcVacuumUnit;
    TTabSheet *tsInArm;
    TTabSheet *tsIndex;
    TGroupBox *grpInarm;
    TScrollBox *scrlbxInArm;
    TGroupBox *grpIndexArm2;
    TScrollBox *scrlbxIndexArm2;
    TGroupBox *grpIndexArm1;
    TScrollBox *scrlbxIndexArm1;
    TTimer *tmr1;
    TTabSheet *tsManual;
    TGroupBox *GroupBox1;
    TSpeedButton *btnSV;
    TPanel *pnlCurectVal;
    TPanel *pnlThreshold;
    TPanel *pnlEvent;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TCheckBox *CheckBox1;
    TEdit *Edit1;
    TEdit *edSV;
    TMyLed *myld1;
    TBtnPanelLane *bplOn;
    TBtnPanelLane *bplOff;
    TSpeedButton *sbReset;
    TTabSheet *tsOutArm;
    TGroupBox *grpOutarm;
    TScrollBox *scrlbxOutArm;
    TGroupBox *grpSetAllValue;
    TButton *btnSetInArm;
    TEdit *edSetInArm;
    TEdit *edSetIndexArm;
    TButton *btnSetIndexArm;
    TButton *btnSetOutArm;
    TEdit *edSetOutArm;
    TLabel *lblInArm;
    TLabel *lblIndexArm;
    TLabel *lblOutArm;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tmr1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall sbResetClick(TObject *Sender);
    void __fastcall btnSetInArmClick(TObject *Sender);
    void __fastcall edSetInArmClick(TObject *Sender);
private:    // User declarations
    void SetPanelPos(bool bInitial=true);
    AnsiString LastFileName;

public:     // User declarations
    __fastcall TfVacuumUnit(TComponent* Owner);
    TMyVacuumPanel *myPalArm2   [TOTAL_VACUUM_UNIT][2];
    TMyVacuumPanel *myPalArm1   [TOTAL_VACUUM_UNIT][2];
    TMyVacuumPanel *myPalInArm  [TOTAL_VACUUM_UNIT/2][2];
    TMyVacuumPanel *myPalOutArm [TOTAL_VACUUM_UNIT/2][2];

    double dIndexArm1[TOTAL_VACUUM_UNIT][2];
    double dIndexArm2[TOTAL_VACUUM_UNIT][2];

    double dInArm[TOTAL_VACUUM_UNIT/2][2];
    double dOutArm[TOTAL_VACUUM_UNIT/2][2];

    bool fShow;
    int iCount;
    void __fastcall ShowSuckMode(int iSel);
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    void __fastcall SaveSetupFile(AnsiString szDir, AnsiString S);
    //void __fastcall CopyFormDB(class TMyVacuumPanel &Target, class TMySucker &Source);
    void Initial();
    int iIndexColMax;
    int iInOutColMax;
    int iTest1;
    bool bTest1;

    int iTest33;
};
extern PACKAGE TfVacuumUnit *fVacuumUnit;
extern void SetIOTableByECAT_VC8_Sucker();
extern void __fastcall VaccumCopyFormSuck(class TMyVacuumPanel &Target, class TMySucker &Source);
extern void __fastcall VaccumCopyToSuck(class TMyVacuumPanel &Source, class TMySucker &Target);
extern void __fastcall SetSuckISABase(class TMySucker &Source, int iISABase);
//---------------------------------------------------------------------------
#endif
