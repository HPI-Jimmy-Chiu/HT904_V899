//---------------------------------------------------------------------------

#ifndef cheksocketH
#define cheksocketH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EPICOLOACLLib_OCX.h"
#include "eVision_OCX.h"
#include <OleCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfCCDPin1 : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TEBW8Image *LiveImage;
    TTabSheet *TabSheet2;
    TEBW8Image *SourceImage;
    TTabSheet *TabSheet3;
    TEBW8Image *EBW8Image1;
    TTabSheet *TabSheet4;
    TEBW8Image *EBW8Image2;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton8;
    TLabel *Label3;
    TEBW8ROI *ROI1;
    TEasyMain *EasyMain;
    TButton *Button1;
    TButton *Button2;
    TEBW8ROI *ROI2;
    TMemo *Memo1;
    TECameraPicolo *CCD;
    TScrollBar *ScrollBar1;
    TLabel *Label1;
    TEdit *Edit3;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall CCDSurfaceAvailable(TObject *Sender,
          long SurfaceHandle, long Data, long Flags, long TimeCode,
          long ExtendedStatus);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfCCDPin1(TComponent* Owner);

    bool fShow;
    bool bLive;
    void __fastcall ReDrawAll();
    void __fastcall  SaveImage(char *str);
    void __fastcall  SaveImage();
    bool bHasSourceImage;
    bool bChangeSourceImage;
    bool bGrabOne;
    bool __fastcall Caculate();
    void __fastcall Load();

};
//---------------------------------------------------------------------------
extern PACKAGE TfCCDPin1 *fCCDPin1;
//---------------------------------------------------------------------------
#endif
