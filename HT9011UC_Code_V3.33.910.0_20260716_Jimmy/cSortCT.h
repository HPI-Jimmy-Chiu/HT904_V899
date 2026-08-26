//---------------------------------------------------------------------------

#ifndef cSortCTH
#define cSortCTH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct
{
    TPanel *pnlCount;
    TPanel *pnlYield;
    TLabel *lblName;
    TPanel *pnlCountART;
    TPanel *pnlYieldART;
    TLabel *lblNameART;

    bool bVisible;
    int iTop;
    int iTag;
    void SetObject(TPanel *_pnlCount, TPanel *_pnlYield, TLabel *_lblName, TPanel *_pnlCountART, TPanel *_pnlYieldART, TLabel *_lblNameART, int _iTag);
    void SetTop(int _iTop);
    void SetVisible(bool _bVisible);
}_MyCountPanel;
//---------------------------------------------------------------------------
class TfSortCT : public TForm
{
__published:    // IDE-managed Components
    TTimer *Timer1;
        TPageControl *PageControl1;
        TTabSheet *SortCount;
        TTabSheet *ARTSortCount;
    TPanel *pnlLoadingBG;
    TLabel *lblLoading;
    TPanel *pnlLoader;
    TPanel *pnlTotalBG;
    TLabel *lblTotal;
    TPanel *pnlTotal;
    TPanel *pnlYield;
    TPanel *pnlUnloadBG;
        TGroupBox *gbLotID;
        TSpeedButton *spbClearAllCount;
        TEdit *edLotID;
    TPanel *pnlLoadingARTBG;
        TLabel *Label14;
    TPanel *pnlLoadingART;
    TPanel *pnlTotalARTBG;
        TLabel *Label15;
    TPanel *pnlTotalART;
    TPanel *pnlYieldART;
        TTimer *Timer2;
    TTabSheet *tsICCount;
    TGroupBox *grpInputArea;
    TLabel *lblLoad;
    TPanel *pnlLoad;
    TLabel *lblHP2;
    TPanel *pnlHP1;
    TLabel *lblHP1;
    TPanel *pnlHP2;
    TLabel *lblAuto1;
    TPanel *pnlAuto1Yield;
    TPanel *pnlAuto1;
    TLabel *lblAuto2;
    TPanel *pnlAuto2Yield;
    TPanel *pnlAuto2;
    TLabel *lblAuto3;
    TPanel *pnlAuto3Yield;
    TPanel *pnlAuto3;
    TLabel *lblAuto4;
    TPanel *pnlAuto4Yield;
    TPanel *pnlAuto4;
    TLabel *lblAuto5;
    TPanel *pnlAuto5Yield;
    TPanel *pnlAuto5;
    TLabel *lblAuto6;
    TPanel *pnlAuto6Yield;
    TPanel *pnlAuto6;
    TLabel *lblFix1;
    TPanel *pnlFix1Yield;
    TPanel *pnlFix1;
    TLabel *lblFix2;
    TPanel *pnlFix2Yield;
    TPanel *pnlFix2;
    TLabel *lblFix4;
    TPanel *pnlFix4Yield;
    TPanel *pnlFix4;
    TLabel *lblFix3;
    TPanel *pnlFix3Yield;
    TPanel *pnlFix3;
    TLabel *lblFix5;
    TPanel *pnlFix5Yield;
    TPanel *pnlFix5;
    TLabel *lblFix6;
    TPanel *pnlFix6Yield;
    TPanel *pnlFix6;
    TLabel *lblFix7;
    TPanel *pnlFix7Yield;
    TPanel *pnlFix7;
    TLabel *lblFix8;
    TPanel *pnlFix8Yield;
    TPanel *pnlFix8;
    TLabel *lblFix9;
    TPanel *pnlFix9Yield;
    TPanel *pnlFix9;
    TLabel *lblFix10;
    TPanel *pnlFix10Yield;
    TPanel *pnlFix10;
    TLabel *lblFix11;
    TPanel *pnlFix11Yield;
    TPanel *pnlFix11;
    TLabel *lblFix12;
    TPanel *pnlFix12Yield;
    TPanel *pnlFix12;
    TLabel *lblBinBox;
    TPanel *pnlBinBoxYield;
    TPanel *pnlBinBox;
    TLabel *lblMag1;
    TPanel *pnlMag1Yield;
    TPanel *pnlMag1;
    TLabel *lblMag2;
    TPanel *pnlMag2Yield;
    TPanel *pnlMag2;
    TLabel *lblMag3;
    TPanel *pnlMag3Yield;
    TPanel *pnlMag3;
    TLabel *lblMag4;
    TPanel *pnlMag4Yield;
    TPanel *pnlMag4;
    TLabel *lblMag5;
    TPanel *pnlMag5Yield;
    TPanel *pnlMag5;
    TLabel *lblMag6;
    TPanel *pnlMag6Yield;
    TPanel *pnlMag6;
    TLabel *lblMag7;
    TPanel *pnlMag7Yield;
    TPanel *pnlMag7;
    TLabel *lblMag8;
    TPanel *pnlMag8Yield;
    TPanel *pnlMag8;
    TLabel *lblMag9;
    TPanel *pnlMag9Yield;
    TPanel *pnlMag9;
    TLabel *lblMag10;
    TPanel *pnlMag10Yield;
    TPanel *pnlMag10;
    TLabel *lblMag11;
    TPanel *pnlMag11Yield;
    TPanel *pnlMag11;
    TLabel *lblMag12;
    TPanel *pnlMag12Yield;
    TPanel *pnlMag12;
    TLabel *lblMag13;
    TPanel *pnlMag13Yield;
    TPanel *pnlMag13;
    TLabel *lblMag14;
    TPanel *pnlMag14Yield;
    TPanel *pnlMag14;
    TSpeedButton *btnClearCount;
    TPanel *pnlUnloadingARTBG;
    TLabel *lblARTMag14;
    TPanel *pnARTMag14Yield;
    TPanel *pnARTMag14;
    TLabel *lblARTMag13;
    TPanel *pnARTMag13Yield;
    TPanel *pnARTMag13;
    TLabel *lblARTMag12;
    TPanel *pnARTMag12Yield;
    TPanel *pnARTMag12;
    TLabel *lblARTMag11;
    TPanel *pnARTMag11Yield;
    TPanel *pnARTMag11;
    TLabel *lblARTMag10;
    TPanel *pnARTMag10Yield;
    TPanel *pnARTMag10;
    TLabel *lblARTMag9;
    TPanel *pnARTMag9Yield;
    TPanel *pnARTMag9;
    TLabel *lblARTMag8;
    TPanel *pnARTMag8Yield;
    TPanel *pnARTMag8;
    TPanel *pnARTMag7;
    TPanel *pnARTMag7Yield;
    TLabel *lblARTMag7;
    TLabel *lblARTMag6;
    TPanel *pnARTMag6Yield;
    TPanel *pnARTMag6;
    TLabel *lblARTMag5;
    TPanel *pnARTMag5Yield;
    TPanel *pnARTMag5;
    TLabel *lblARTMag4;
    TPanel *pnARTMag4Yield;
    TPanel *pnARTMag4;
    TLabel *lblARTMag3;
    TPanel *pnARTMag3Yield;
    TPanel *pnARTMag3;
    TLabel *lblARTMag2;
    TPanel *pnARTMag2Yield;
    TPanel *pnARTMag2;
    TLabel *lblARTMag1;
    TPanel *pnARTMag1Yield;
    TPanel *pnARTMag1;
    TLabel *lblARTBinBox;
    TPanel *pnARTBinBoxYield;
    TPanel *pnARTBinBox;
    TLabel *lblARTFix12;
    TPanel *pnARTFix12Yield;
    TPanel *pnARTFix12;
    TLabel *lblARTFix11;
    TPanel *pnARTFix11Yield;
    TPanel *pnARTFix11;
    TLabel *lblARTFix10;
    TPanel *pnARTFix10Yield;
    TPanel *pnARTFix10;
    TLabel *lblARTFix9;
    TPanel *pnARTFix9Yield;
    TPanel *pnARTFix9;
    TLabel *lblARTAuto4;
    TPanel *pnARTAuto4Yield;
    TPanel *pnARTAuto4;
    TLabel *lblARTFix8;
    TPanel *pnARTFix8Yield;
    TPanel *pnARTFix8;
    TLabel *lblARTFix7;
    TPanel *pnARTFix7Yield;
    TPanel *pnARTFix7;
    TLabel *lblARTFix6;
    TPanel *pnARTFix6Yield;
    TPanel *pnARTFix6;
    TLabel *lblARTFix5;
    TPanel *pnARTFix5Yield;
    TPanel *pnARTFix5;
    TLabel *lblARTFix4;
    TPanel *pnARTFix4Yield;
    TPanel *pnARTFix4;
    TLabel *lblARTFix3;
    TPanel *pnARTFix3Yield;
    TPanel *pnARTFix3;
    TLabel *lblARTFix2;
    TPanel *pnARTFix2Yield;
    TPanel *pnARTFix2;
    TLabel *lblARTFix1;
    TPanel *pnARTFix1Yield;
    TPanel *pnARTFix1;
    TLabel *lblARTAuto2;
    TPanel *pnARTAuto2Yield;
    TPanel *pnARTAuto2;
    TLabel *lblARTAuto1;
    TPanel *pnARTAuto1Yield;
    TPanel *pnARTAuto1;
    TLabel *lblARTAuto3;
    TPanel *pnARTAuto3Yield;
    TPanel *pnARTAuto3;
    TLabel *lblARTAuto5;
    TPanel *pnARTAuto5Yield;
    TPanel *pnARTAuto5;
    TLabel *lblARTAuto6;
    TPanel *pnARTAuto6Yield;
    TPanel *pnARTAuto6;
    TPanel *pnlLoadTrayCt;
    TPanel *pnlLoadCID;
    TPanel *pnlAuto1TrayCt;
    TPanel *pnlAuto1CID;
    TPanel *pnlTrayCount;
    TPanel *pnlTrayCID;
    TPanel *pnlAuto2TrayCt;
    TPanel *pnlAuto2CID;
    TPanel *pnlAuto3TrayCt;
    TPanel *pnlAuto3CID;
    TPanel *pnlAuto4TrayCt;
    TPanel *pnlAuto4CID;
    TPanel *pnlAuto5TrayCt;
    TPanel *pnlAuto5CID;
    TPanel *pnlAuto6TrayCt;
    TPanel *pnlAuto6CID;
    TPanel *pnlCoverTrayD;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall pnlLoaderMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlAuto1YieldMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlAuto2YieldMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlAuto3YieldMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlFix1YieldMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlFix2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlFix3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnClearCountClick(TObject *Sender);
    void __fastcall spbClearAllCountClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall pnlHP1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlHP2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlMag1YieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlMag1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall lblTotalClick(TObject *Sender);
    void __fastcall pnlFix4YieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlFix5YieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlFix6YieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlAuto1DblClick(TObject *Sender);
    void __fastcall pnlAuto1CIDDblClick(TObject *Sender);
private:    // User declarations
    TPanel *TrayCTPanel[eTrayCount];                                            //JerryYang 20220909 : 10->eTrayCount
    TPanel *TrayCTART[eTrayCount];                                              //JerryYang 20220909 : 10->eTrayCount
    TPanel *pnlSortCnt[eTrayCount];
    TPanel *pnlSortART[eTrayCount];
public:     // User declarations
    __fastcall TfSortCT(TComponent* Owner);
    bool bShow;
    void __fastcall ShowLoadingIC();
    void __fastcall ShowSortIC();
    void __fastcall UpForm();
    void __fastcall ShowLoadingIC_ART();                                        //kevin 20150615
    void IntervalTotalYieldDifference();                                        //20150604 Mylin Interval Total Yield Difference
    void ClearIntervalTotalYieldDifferenceCount();                              //20150604 Mylin Interval Total Yield Difference
    void Alarm4Yield();                                                         //20150604 Mylin Interval Total Yield Difference
    void Alarm5Yield();                                                         //Sam 20171213 (Steven) : 超豐良率監控
    void CheckTheYieldAfterPlaceAuto();                                         //Sam 20230306 : 修正 Initial Start 誤報警問題
    _MyCountPanel myCountPanel[eTrayCount];

    TPanel *pnlTrayCnt[MAX_AUTO_TRAY];
    TPanel *pnlTrayID[MAX_AUTO_TRAY];
    int iVisibleHeight;
};
//---------------------------------------------------------------------------
extern PACKAGE TfSortCT *fSortCT;
//---------------------------------------------------------------------------
#endif
