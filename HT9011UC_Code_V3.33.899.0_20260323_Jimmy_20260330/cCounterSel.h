//---------------------------------------------------------------------------

#ifndef cCounterSelH
#define cCounterSelH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfCounterSel : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox17;
    TGroupBox *gbLoadingCount;
    TLabel *labLoadingCount;
    TRadioButton *rbLoadingCount_On;
    TRadioButton *rbLoadingCount_Off;
    TGroupBox *gbContactCount;
    TLabel *labContactCount;
    TRadioButton *rbContactCount_On;
    TRadioButton *rbContactCount_Off;
    TGroupBox *gbTestCategory;
    TLabel *labTestCategory;
    TRadioButton *rbTestCategory_On;
    TRadioButton *rbTestCategory_Off;
    TRadioGroup *rgTestCategory;
    TGroupBox *gbScanner;
    TLabel *labScanner;
    TRadioButton *rbScanner_On;
    TRadioButton *rbScanner_Off;
    TGroupBox *gbTemperature;
    TLabel *labTemperature;
    TRadioButton *rbTemperature_On;
    TRadioButton *rbTemperature_Off;
    TGroupBox *gbBinAssign;
    TLabel *labBinAssign;
    TRadioButton *rbBinAssign_On;
    TRadioButton *rbBinAssign_Off;
    TGroupBox *gbStatus;
    TLabel *labStatus;
    TCheckBox *cbIndexTime;
    TCheckBox *cbUPH;
    TCheckBox *cbDefaultValue;
    TCheckBox *cbCycleTime;
    TCheckBox *cbContactHeight;
    TSpeedButton *spbExit;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbExitClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfCounterSel(TComponent* Owner);
    AnsiString __fastcall CheckFormIni(AnsiString szDir, AnsiString str);
    bool NeedRef;
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfCounterSel *fCounterSel;
//---------------------------------------------------------------------------
#endif
