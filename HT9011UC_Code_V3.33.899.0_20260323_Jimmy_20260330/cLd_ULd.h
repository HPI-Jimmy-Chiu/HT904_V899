//---------------------------------------------------------------------------

#ifndef cLd_ULdH
#define cLd_ULdH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfLd_ULd : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TSpeedButton *btnDefaultValue;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *tsKnocker;
    TPanel *Panel2;
    TPanel *Panel3;
    TGroupBox *grpLoader;
    TImage *Image1;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label5;
    TLabel *Label7;
    TEdit *edtLD_TrayArrivalDely;
    TEdit *edtLD_FixTrayDely;
    TEdit *edtLD_MiddLockDelay;
    TEdit *edtLD_LiftDownDelay;
    TGroupBox *grpUnloader;
    TImage *Image2;
    TLabel *Label9;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label15;
    TLabel *Label17;
    TEdit *edtULD_TrayArrivalDelay;
    TEdit *edtULD_FixTrayDely;
    TEdit *edtULD_TrayBackDelay;
    TEdit *edtULD_LiftDownDelay;
    TEdit *edtULD_TrayArrivalWait;
    TLabel *lblUnit;
    TPanel *palKnocker;
    TLabel *labP13;
    TLabel *labP14_2;
    TLabel *labP14_1;
    TLabel *labP16;
    TLabel *labP16_2;
    TLabel *labP13_2;
    TLabel *labF40_2;
    TLabel *labF40_3;
    TLabel *labF40_1;
    TCheckBox *cbP13;
    TEdit *edP13_1;
    TCheckBox *cbP14;
    TEdit *edP14_1;
    TEdit *edP14_2;
    TCheckBox *cbP16;
    TEdit *edP16_1;
    TEdit *edP16_2;
    TEdit *edP13_2;
    TCheckBox *cbF23;
    TEdit *edF23;
    TCheckBox *cbF40;
    TEdit *edP40_2;
    TEdit *edP40_3;
    TEdit *edP40_1;
    TRadioButton *rbTemp;
    TLabel *Label2;
    TEdit *edtLD_BeforeDownDelay;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edtLD_TrayArrivalDelyKeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall btnDefaultValueClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
private:    // User declarations
    AnsiString LastFileName;
    void __fastcall LoadImage();

public:     // User declarations
    __fastcall TfLd_ULd(TComponent* Owner);
    void __fastcall ReadFile();
    void __fastcall SaveSetupFile(AnsiString szDir,AnsiString S);               //kevin 20150105
    void __fastcall DoIniDataToForm();                                          //kevin 20181109
    void SetDefaultPos();                                                       //Steven 20240215 : 調整預設位置
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfLd_ULd *fLd_ULd;
//---------------------------------------------------------------------------
#endif
