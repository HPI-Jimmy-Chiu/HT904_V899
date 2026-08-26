//---------------------------------------------------------------------------

#ifndef cTowerLightH
#define cTowerLightH
//---------------------------------------------------------------------------

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
//---------------------------------------------------------------------------
class TfTowerLight : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel3;
    TALed *RGB50;
    TALed *RGB40;
    TALed *RGB30;
    TALed *RGB20;
    TALed *RGB10;
    TALed *RGB00;
    TALed *RGB01;
    TALed *RGB11;
    TALed *RGB21;
    TALed *RGB31;
    TALed *RGB41;
    TALed *RGB51;
    TALed *RGB52;
    TALed *RGB42;
    TALed *RGB32;
    TALed *RGB22;
    TALed *RGB12;
    TALed *RGB02;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label6;
    TBevel *Bevel1;
    TBevel *Bevel2;
    TBevel *Bevel3;
    TBevel *Bevel4;
    TLabel *Label43;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TComboBox *cbRunning;
    TComboBox *cbJam;
    TComboBox *cbPause;
    TComboBox *cbMessage;
    TComboBox *cbHeating;
    TComboBox *cbHome;
    TRadioGroup *rgMusicTest;
    TPanel *Panel2;
    TTimer *Timer1;
    TPanel *Panel1;
    TALed *RGB60;
    TALed *RGB61;
    TALed *RGB62;
    TComboBox *cbOffLine;
    TPanel *spbExit;
    TPanel *Panel11;
    TComboBox *cbART;
    TALed *RGB72;
    TALed *RGB71;
    TALed *RGB70;
    TPanel *palART;
    void __fastcall RGB00Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall rgMusicTestClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
private:    // User declarations
    void __fastcall UpdateTowerLed(int);

public:     // User declarations
    __fastcall TfTowerLight(TComponent* Owner);
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfTowerLight *fTowerLight;
//---------------------------------------------------------------------------
#endif
