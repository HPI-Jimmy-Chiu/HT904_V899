//---------------------------------------------------------------------------

#ifndef BarcodeReaderH
#define BarcodeReaderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "myTimer.h"

#define KEY_NONE        0
#define KEY_ENTER       1
#define KEY_CANCEL      2

//---------------------------------------------------------------------------
class TFormBarcodeReader : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlBarcodeReader;
    TEdit *edtBarcodeNumber;
    TButton *btnEnter;
    TLabel *lblInputType;
    TTimer *TimerKeyIn;
    TPopupMenu *pmBarcode;
    TMenuItem *miClear;
    TMenuItem *miClose;
    void __fastcall btnEnterClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edtBarcodeNumberKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtBarcodeNumberKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall TimerKeyInTimer(TObject *Sender);
    void __fastcall miClearClick(TObject *Sender);
    void __fastcall miCloseClick(TObject *Sender);
private:    // User declarations
    TQPF_Timer HDelayTime; //RogerYang 20170407 (Steven) KeyUp­p®É
public:     // User declarations
    __fastcall TFormBarcodeReader(TComponent* Owner);

    AnsiString sBarcodeInfo;
    int iKeyType;
    bool bShow;  //Steven 20140508
    AnsiString _sInputType;

};
int Barcode_Reader(int count);    // 20140103 wei KYEC Barcode Reader
//---------------------------------------------------------------------------
extern PACKAGE TFormBarcodeReader *FormBarcodeReader;
//---------------------------------------------------------------------------

AnsiString InputBarcodeNumber(AnsiString sLabelCaption, AnsiString sInputType="");


#endif
