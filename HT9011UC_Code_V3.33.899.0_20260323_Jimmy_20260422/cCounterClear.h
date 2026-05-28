//---------------------------------------------------------------------------

#ifndef cCounterClearH
#define cCounterClearH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfCounterClear : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *gbItems;
    TCheckBox *cbAlarmData;
    TCheckBox *cbTestCategory;
    TCheckBox *cbScanner;
    TCheckBox *cbLoadingCount;
    TCheckBox *cbContactCountCurr;
    TCheckBox *cbContactCountHis;
    TCheckBox *cbSortingCount;
    TCheckBox *cbTimeData;
    TCheckBox *cbSelectAll;
    TSpeedButton *spbExe;
    TSpeedButton *spbExit;
    void __fastcall cbSelectAllMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cbAlarmDataMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall spbExeClick(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    bool bHasFile; //Steven 20110801 : 檢查檔案在不在
public:     // User declarations
    __fastcall TfCounterClear(TComponent* Owner);
    void __fastcall WriteCTInfo();
    void __fastcall ReadCTInfo();
    void __fastcall ClearCount(int ClearType);
    void __fastcall AutoClear();
    bool fShow;
    void __fastcall LowYieldSpecialInitail();   //Sam 20210505 : PTI 要求的兩段 Low Yeild
};
//---------------------------------------------------------------------------
extern PACKAGE TfCounterClear *fCounterClear;
//---------------------------------------------------------------------------
#endif
