//---------------------------------------------------------------------------

#ifndef uShowMessageH
#define uShowMessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------
class TfShowMessage : public TForm
{
__published:    // IDE-managed Components
    TStringGrid *sgdSpeedView;
    TLabel *lblIndexCycleTime;
    TLabel *lblTestTime;
    TLabel *lblInitialMessage;
    TProgressBar *ProgressBar1;
    TButton *btnBackToMain;
    TImage *imgTray;
    TImage *imgDevice;
    TImage *imgTrayHere;
    TImage *imgDeviceHere;
//    TLabel *VerShow;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sgdSpeedViewDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
    HDC hDC;
    TCanvas *pCanvas;
    bool bShowTotal;
public:     // User declarations
    __fastcall TfShowMessage(TComponent* Owner);
    void __fastcall ShowSpeed(bool tag);
    bool bShow;
    void __fastcall UpdateForm(int iBar);
    void __fastcall ShowMyMessage();
    void __fastcall SetFormLayer();                                             //把該顯示在最上層的畫面顯示出來
    void __fastcall ShowTrayDeviceDir();
    int iTrayTop;
    int iTrayLeft;
};
//---------------------------------------------------------------------------
extern PACKAGE TfShowMessage *fShowMessage;
//---------------------------------------------------------------------------
#endif
