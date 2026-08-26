//---------------------------------------------------------------------------
#ifndef toolsH
#define toolsH
//---------------------------------------------------------------------------

#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
//---------------------------------------------------------------------------
class TFTool : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TEdit *edtPort0;
    TCheckBox *CheckBox1;
    TLabel *Label1;
    TCheckBox *CheckBox2;
    TALed *ALed1;
    TCheckBox *CheckBox3;
    TCheckBox *CheckBox4;
    TALed *ALed2;
    TCheckBox *CheckBox5;
    TCheckBox *CheckBox6;
    TALed *ALed3;
    TCheckBox *CheckBox7;
    TCheckBox *CheckBox8;
    TALed *ALed4;
    TCheckBox *CheckBox9;
    TCheckBox *CheckBox10;
    TALed *ALed5;
    TCheckBox *CheckBox11;
    TCheckBox *CheckBox12;
    TALed *ALed6;
    TCheckBox *CheckBox13;
    TCheckBox *CheckBox14;
    TALed *ALed7;
    TCheckBox *CheckBox15;
    TCheckBox *CheckBox16;
    TALed *ALed8;
    TLabel *Label2;
    TEdit *edtPort2;
    TCheckBox *CheckBox17;
    TCheckBox *CheckBox18;
    TALed *ALed9;
    TALed *ALed10;
    TALed *ALed11;
    TALed *ALed12;
    TALed *ALed13;
    TALed *ALed14;
    TALed *ALed15;
    TALed *ALed16;
    TCheckBox *CheckBox19;
    TCheckBox *CheckBox20;
    TCheckBox *CheckBox21;
    TCheckBox *CheckBox22;
    TCheckBox *CheckBox23;
    TCheckBox *CheckBox24;
    TCheckBox *CheckBox25;
    TCheckBox *CheckBox26;
    TCheckBox *CheckBox27;
    TCheckBox *CheckBox28;
    TCheckBox *CheckBox29;
    TCheckBox *CheckBox30;
    TCheckBox *CheckBox31;
    TCheckBox *CheckBox32;
    TComboBox *ComboBox1;
    TLabel *Label3;
    TTimer *Timer1;
    TLabel *Label4;
    TEdit *edtPort1;
    TEdit *edtPort3;
    TSpeedButton *SpeedButton2;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edtLane0;
    TEdit *edtLane2;
    TEdit *edtLane1;
    TEdit *edtLane3;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *edtIP0;
    TEdit *edtIP2;
    TEdit *edtIP1;
    TEdit *edtIP3;
    TPanel *sbExit;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall edtPort0KeyPress(TObject *Sender, char &Key);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall edtLane0Click(TObject *Sender);
    void __fastcall edtIP0Click(TObject *Sender);
    void __fastcall edtPort0Click(TObject *Sender);
private:    // User declarations
    void __fastcall EnableIOChange(bool bFlag);
public:     // User declarations
    __fastcall TFTool(TComponent* Owner);
    bool fShow;
    int TimeTick;
    int TimeCT;
};
//---------------------------------------------------------------------------
extern PACKAGE TFTool *FTool;
//---------------------------------------------------------------------------
#endif
