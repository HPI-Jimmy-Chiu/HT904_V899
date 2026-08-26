//---------------------------------------------------------------------------

#ifndef cBinAoiSelH
#define cBinAoiSelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

class TfBinAOISel : public TForm
{
__published:    // IDE-managed Components
    TLabel *Label1;
    TPanel *Panel2;
    TScrollBox *ScrollBox1;
    TSpeedButton *spbExit;
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);

private:    // User declarations
    void AddRow(AnsiString labelText, AnsiString comboDefault,int bin);
    int RowCount;
    std::vector<int> AOI_ReadBin;                  //Eastsun 20260316 : AOI ­n¤Àbin
public:        // User declarations
    struct TComboItem
    {
        TComboBox* Combo;
        int Bin;
    };
    int MaxBinSearch;
    __fastcall TfBinAOISel(TComponent* Owner);
    std::vector<TComboItem> ComboList;
    void Refresh(int tag);
    int GetTrayID(AnsiString Name);
    AnsiString GetTrayName(int trayID);
    void Save(int tag);
    int GetTrayIDAddOne(AnsiString Name);
};
//---------------------------------------------------------------------------
extern PACKAGE TfBinAOISel *fBinAOISel;
//---------------------------------------------------------------------------
#endif
