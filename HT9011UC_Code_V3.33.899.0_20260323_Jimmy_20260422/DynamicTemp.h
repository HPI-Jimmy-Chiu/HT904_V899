//---------------------------------------------------------------------------

#ifndef DynamicTempH
#define DynamicTempH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Chart.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfDynamicTemp : public TForm
{
__published:    // IDE-managed Components
    TPanel *btExit;
    TPanel *PanelMain7;
    TChart *Chart1;
    TLineSeries *Series1;
    TLineSeries *Series2;
    TLineSeries *Series3;
    TLineSeries *Series4;
    TLineSeries *Series5;
    TLineSeries *Series6;
    TLineSeries *Series7;
    TLineSeries *Series8;
    TLineSeries *Series9;
    TLineSeries *Series10;
    TLineSeries *Series11;
    TLineSeries *Series12;
    TLineSeries *Series13;
    TLineSeries *Series14;
    TLineSeries *Series15;
    TLineSeries *Series16;
    TPanel *Panel1;
    TLabel *Label4;
    TSaveDialog *SaveDialog1;
    TOpenDialog *OpenDialog2;
    TLineSeries *Series17;
    TLineSeries *Series18;
    TEdit *edMax;
    TEdit *edMin;
    TGroupBox *gbA1;
    TCheckBox *CheckBox1;
    TEdit *edtRealTime1;
    TGroupBox *gbB2;
    TEdit *edtRealTime2;
    TCheckBox *CheckBox2;
    TPanel *Panel2;
    TListBox *ListBox1;
    TPanel *btTempICSave;
    TPanel *btTempICLoad;
    TGroupBox *gbF10;
    TGroupBox *gbH16;
    TGroupBox *gbH14;
    TGroupBox *gbF12;
    TGroupBox *gbD8;
    TGroupBox *gbD6;
    TGroupBox *gbB4;
    TGroupBox *gbG15;
    TGroupBox *gbG13;
    TGroupBox *gbE11;
    TGroupBox *gbE9;
    TGroupBox *gbC7;
    TGroupBox *gbC5;
    TGroupBox *gbA3;
    TCheckBox *CheckBox3;
    TEdit *edtRealTime3;
    TEdit *edtRealTime5;
    TEdit *edtRealTime7;
    TEdit *edtRealTime9;
    TEdit *edtRealTime11;
    TEdit *edtRealTime13;
    TEdit *edtRealTime15;
    TCheckBox *CheckBox5;
    TCheckBox *CheckBox7;
    TCheckBox *CheckBox9;
    TCheckBox *CheckBox11;
    TCheckBox *CheckBox13;
    TCheckBox *CheckBox15;
    TEdit *edtRealTime4;
    TEdit *edtRealTime6;
    TEdit *edtRealTime8;
    TEdit *edtRealTime10;
    TEdit *edtRealTime12;
    TEdit *edtRealTime14;
    TEdit *edtRealTime16;
    TCheckBox *CheckBox4;
    TCheckBox *CheckBox6;
    TCheckBox *CheckBox8;
    TCheckBox *CheckBox10;
    TCheckBox *CheckBox12;
    TCheckBox *CheckBox14;
    TCheckBox *CheckBox16;
    TPanel *sbTempICClear;
    TPanel *btShow;
    TPanel *btSelectAll;
    TPanel *btDefaultPos;
    TGroupBox *gbUpper;
    TEdit *edUpper;
    TCheckBox *CheckBox17;
    TGroupBox *gbLower;
    TEdit *edLower;
    TCheckBox *CheckBox18;
    TLabel *gbUpperDbl;
    TLabel *gbLowerDbl;
        TTimer *Timer1;
        TButton *Button1;
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall lblRealTime6Click(TObject *Sender);
    void __fastcall lblRealTime1DblClick(TObject *Sender);
    void __fastcall btTempICLoadClick(TObject *Sender);
    void __fastcall btTempICSaveClick(TObject *Sender);
    void __fastcall sbTempICClearClick(TObject *Sender);
    void __fastcall lblRealTime2Click(TObject *Sender);
    void __fastcall btShowClick(TObject *Sender);
    void __fastcall edMinMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edMaxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall edLowerMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edUpperMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btSelectAllClick(TObject *Sender);
    void __fastcall gbA1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall gbA1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall gbA1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall btSelectAllMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btSelectAllMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btDefaultPosClick(TObject *Sender);
    void __fastcall gbUpperDblClick(TObject *Sender);
    void __fastcall gbLowerDblClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:    // User declarations
    int       iStartX;
    int       iStartY;
public:     // User declarations
    __fastcall TfDynamicTemp(TComponent* Owner);
    TColor TC[18];
    TEdit *SetEdit[18];
    TCheckBox *myCB[18];
};
//---------------------------------------------------------------------------
extern PACKAGE TfDynamicTemp *fDynamicTemp;
//---------------------------------------------------------------------------
#endif
