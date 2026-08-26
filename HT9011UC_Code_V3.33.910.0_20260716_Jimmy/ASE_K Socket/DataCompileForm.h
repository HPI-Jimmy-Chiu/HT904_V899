//---------------------------------------------------------------------------

#ifndef DataCompileFormH
#define DataCompileFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TDataCompile : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edtWorkTemp;
    TEdit *edtSoakTime;
    TEdit *edtForcePerPin;
    TEdit *edtPinCount;
    TEdit *edtSiteMap;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *edtBinMap;
    TButton *btnCompare;
    TLabel *Label9;
    void __fastcall btnCompareClick(TObject *Sender);
    void __fastcall edtWorkTempClick(TObject *Sender);
    void __fastcall edtForcePerPinClick(TObject *Sender);
    void __fastcall edtPinCountClick(TObject *Sender);
    void __fastcall edtSoakTimeClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TDataCompile(TComponent* Owner);
    int __fastcall GetDataSpecial(AnsiString &SData);

    int iMachType;  //kevin 20180131 ¾÷«¬
};
//---------------------------------------------------------------------------
extern PACKAGE TDataCompile *DataCompile;
//---------------------------------------------------------------------------
#endif
