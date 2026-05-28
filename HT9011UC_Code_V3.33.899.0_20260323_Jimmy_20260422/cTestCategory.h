#ifndef cTestCategoryH
#define cTestCategoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfTestCategory : public TForm
{
__published:    // IDE-managed Components
    TStringGrid *sgArm2;
    TStringGrid *sgArm1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgArm1DrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
    TStringGrid *MyStringGD[2];
    int EdgeWidth;
    int EdgeHeight;
    HDC PtrDC[2];
//    int TestResult[2][4][4];
//    TColor ColorPtr[2][4][4];
    //jou 981226 start : for 9046
    int TestResult[2][4][8];
    TColor ColorPtr[2][4][8];
    //jou 981226 end
    bool bCateByArm;
    TCanvas *pCanvas;
public:     // User declarations
    __fastcall TfTestCategory(TComponent* Owner);
    bool bShow;
    void __fastcall SetTestCateCellINT(int Arm,int X,int Y,int Bin,TColor Color);
    void __fastcall SetTestCateCellAS(int Arm,int X,int Y,AnsiString Bin,TColor Color);
    void __fastcall InitCateCell();
    void __fastcall SetShowCateMode();
    void __fastcall AdjFormData();
    void __fastcall SetTestingCateCell(int Arm,int X,int Y,TColor Color);
    int __fastcall GetTestResult(int Arm,int Row,int Col,TColor *CellColor);
    void __fastcall ShowTestCategory(int iIndex);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTestCategory *fTestCategory;
//---------------------------------------------------------------------------
#endif
