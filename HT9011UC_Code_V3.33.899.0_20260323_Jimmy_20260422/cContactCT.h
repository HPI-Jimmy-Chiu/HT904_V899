//---------------------------------------------------------------------------

#ifndef cContactCTH
#define cContactCTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfContactCT : public TForm
{
__published:    // IDE-managed Components
    TStringGrid *sgYield;
    TRadioGroup *rgYieldType;
    TPanel *palClearCnt;
    TButton *btClearCount;
    TButton *btYieldChart;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgYieldDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall rgYieldTypeClick(TObject *Sender);
    void __fastcall sgYieldDblClick(TObject *Sender);
    void __fastcall sgYieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btClearCountClick(TObject *Sender);
    void __fastcall btYieldChartClick(TObject *Sender);
private:    // User declarations
    HDC hDC;
    TCanvas *pCanvas;
public:     // User declarations
    __fastcall TfContactCT(TComponent* Owner);
    void __fastcall ShowFormComp();
    bool __fastcall TestSite2X2Mode();
    AnsiString __fastcall ReturnSiteData(int Arm, int ARow);
    double ReturnSiteDataArray(bool bType, int i, int j);
    bool bShow;
    void __fastcall ClearData(int iRow, int iCol);

    int ReturnSiteDataArray_AutoClean(bool bType, int i, int j);  //ChungHung 20131225 add
    void __fastcall ClearData_AutoClean(); //ChungHung 20131225 add
    double GetLowYield_AutoClean(int iType);  //iType:0 LowYield ;1 SiteYieldDifferent
    void SaveSiteYield(AnsiString SaveEvent);
    void ACSmartClearData();        //Sam 20230111 : Smart Auto Clean
    void SaveTotalYield(AnsiString SaveEvent);  //Sam 20231106 : ¬ö¿ý Total yield
};
//---------------------------------------------------------------------------
extern PACKAGE TfContactCT *fContactCT;
//---------------------------------------------------------------------------
#endif
