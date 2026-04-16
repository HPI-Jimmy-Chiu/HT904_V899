//---------------------------------------------------------------------------

#ifndef DIOInterFaceCFGH
#define DIOInterFaceCFGH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfDIOFrom : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox3;
    TRadioGroup *rgBinLogic;
    TRadioGroup *rgBinBitLength;
    TRadioGroup *rgBinDataType;
    TGroupBox *GroupBox4;
    TLabel *Label6;
    TEdit *edTTLModeName;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TSpeedButton *spbSave;
    TSpeedButton *spbLoad;
    TSpeedButton *spbDelete;
    TPanel *spbExit;
    TPanel *Panel1;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *edSignalAfterOff;
    TComboBox *cbSignalType;
    TEdit *edSignalBeforeOn;
    TGroupBox *GroupBox1;
    TLabel *Label52;
    TLabel *Label55;
    TRadioGroup *rgStartLogic;
    TRadioGroup *rgStartChannel;
    TEdit *edPulseWidth;
    TComboBox *cbOneSTChannel;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Edit3KeyPress(TObject *Sender, char &Key);
    void __fastcall edSignalBeforeOnKeyPress(TObject *Sender, char &Key);
    void __fastcall edPulseWidthMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall spbLoadClick(TObject *Sender);
    void __fastcall spbDeleteClick(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    //TStringList *MyList;
public:     // User declarations
    __fastcall TfDIOFrom(TComponent* Owner);
    void __fastcall LoadData(AnsiString S);
    void __fastcall InitData();
    void __fastcall DoIniDataToForm();
    AnsiString GetDIOFileName();                    //Steven 20180626 : TTL設定存到工作檔裡面
    AnsiString DIOFileName;
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfDIOFrom *fDIOFrom;
//---------------------------------------------------------------------------
#endif
