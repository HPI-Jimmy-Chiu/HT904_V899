//---------------------------------------------------------------------------

#ifndef cAutoAlignmentH
#define cAutoAlignmentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include "cVisionComm.h"

#include "CAlignmentB.h"
#include <Grids.hpp>

//layout kit Height/Width
#define AlignHP_W       15000.0
#define AlignHP_H       39000.0
#define AlignLoad_W     11000.0
#define AlignLoad_H     33484.0 //33484.0 //35000.0   //33650.0
#define AlignAuto_W     1000.0
#define AlignAuto_H     1000.0
#define AlignFix_W      1000.0
#define AlignFix_H      1000.0

//layout Mark to Ori Teach
#define eInShut1ToShut1X  8000
#define eInShut1ToShut1Y -2500
#define eInShut2ToShut2X  8000
#define eInShut2ToShut2Y -2500

#define eHP_1ToHP2X  2700 //EdgeToTeachX=9000 ; EdgeToTeachY=1000
#define eHP_1ToHP2Y -1500
#define eHP_2ToHP1X -12300
#define eHP_2ToHP1Y -1500
#define eHP_3ToHP1X  2700
#define eHP_3ToHP1Y  37500

#define eLoad_1ToLoadTeachX -11350
#define eLoad_1ToLoadTeachY -2000
#define eLoad_2ToLoadTeachX -350
#define eLoad_2ToLoadTeachY -2000
#define eLoad_3ToLoadTeachX -5100
#define eLoad_3ToLoadTeachY  31350//31650

//Top CCD 移動到 Bottom CCD 上面的位置
#define TopCCDOnBottomCCDX 16175
#define TopCCDOnBottomCCDY 21
//---------------------------------------------------------------------------
class TfAutoAlignment : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TPanel *palBase;
    TPanel *palCCDControl;
    TSpeedButton *sbPasue;
    TSpeedButton *sbStart;
    TLabel *lblCCDAlarm;
    TMemo *mmoAlignFlowLog;
    TEdit *edAlignOffsetAlarm;
    TPageControl *PageControl1;
    TTabSheet *tsTeach;
    TEdit *edInput;
    TPageControl *PageControl2;
    TTabSheet *tsInMonitor;
    TGroupBox *grpInPicker;
    TLabel *lblInX120Pitch;
    TLabel *lblInX40Pitch;
    TLabel *lblInPickerA;
    TLabel *lblInPickerB;
    TLabel *lblInPickerC;
    TLabel *lblInPickerD;
    TLabel *lblInPickerE;
    TLabel *lblInPickerF;
    TLabel *lblInPickerG;
    TLabel *lblInPickerH;
    TLabel *lblInAvg;
    TLabel *lblInPickerOfsY2;
    TLabel *lblInPickerOfsX2;
    TLabel *lblInPickerDOfs;
    TLabel *lblInPickerHOfs;
    TLabel *lblInPickerFOfs;
    TLabel *lblInPickerGOfs;
    TLabel *lblInPickerEOfs;
    TLabel *lblInPickerCOfs;
    TLabel *lblInPickerAOfs;
    TLabel *lblInPickerOfsY1;
    TLabel *lblInPickerOfsX1;
    TLabel *lblInPickerBOfs;
    TEdit *edInX120PickerA;
    TEdit *edInX40PickerA;
    TEdit *edInX40PickerB;
    TEdit *edInX120PickerB;
    TEdit *edInX120PickerC;
    TEdit *edInX40PickerC;
    TEdit *edInX40PickerD;
    TEdit *edInX120PickerD;
    TEdit *edInX120PickerE;
    TEdit *edInX40PickerE;
    TEdit *edInX40PickerF;
    TEdit *edInX120PickerF;
    TEdit *edInX120PickerG;
    TEdit *edInX40PickerG;
    TEdit *edInX40PickerH;
    TEdit *edInX120PickerH;
    TEdit *edInX120Avg;
    TEdit *edInX40Avg;
    TEdit *edInPickerBOfsY;
    TEdit *edInPickerBOfsX;
    TEdit *edInPickerDOfsY;
    TEdit *edInPickerDOfsX;
    TEdit *edInPickerFOfsX;
    TEdit *edInPickerFOfsY;
    TEdit *edInPickerHOfsY;
    TEdit *edInPickerHOfsX;
    TEdit *edInPickerGOfsY;
    TEdit *edInPickerEOfsY;
    TEdit *edInPickerGOfsX;
    TEdit *edInPickerEOfsX;
    TEdit *edInPickerCOfsX;
    TEdit *edInPickerCOfsY;
    TEdit *edInPickerAOfsX;
    TEdit *edInPickerAOfsY;
    TGroupBox *grpInMark;
    TLabel *lblInScaleX;
    TLabel *lblInScaleY;
    TEdit *edInShtScaleY2;
    TEdit *edInShtScaleX2;
    TEdit *edInShtOfsY2;
    TEdit *edInShtOfsX2;
    TEdit *edInShtOfsX1;
    TEdit *edInShtOfsY1;
    TEdit *edInShtScaleX1;
    TEdit *edInShtScaleY1;
    TEdit *edHPScaleY2;
    TEdit *edHPScaleX2;
    TEdit *edHPOfsY2;
    TEdit *edHPOfsX2;
    TEdit *edHPOfsX1;
    TEdit *edHPOfsY1;
    TEdit *edHPScaleX1;
    TEdit *edHPScaleY1;
    TEdit *edLoadScaleY;
    TEdit *edLoadScaleX;
    TEdit *edLoadOfsY;
    TEdit *edLoadOfsX;
    TGroupBox *grpInCCD;
    TLabel *lblInTopCCDToBasePickerOfsY;
    TLabel *lblInTopCCDToBasePickerOfsX;
    TLabel *lblInTopCCDToBasePickerOfs;
    TEdit *edInTopCCDToBasePickerOfsX;
    TEdit *edInTopCCDToBasePickerOfsY;
    TTabSheet *tsOutMonitor;
    TRadioGroup *rgAutoAlignOnOff;
    TClientSocket *clntscktOutBottom;
    TClientSocket *clntscktOutTop;
    TClientSocket *clntscktInTop;
    TClientSocket *clntscktInBottom;
    TTimer *tmrProcessGetData;
    TTimer *tmrCCDInitial;
    TTimer *tmrCCDConnect;
    TTabSheet *tsCCDSetting;
    TGroupBox *grpCCDSetting1;
    TLabel *lblCCDIP1;
    TLabel *lbllCCDPort1;
    TEdit *edCCDIP1;
    TEdit *edCCDPort1;
    TBitBtn *btnCCDConnect1;
    TBitBtn *btnCCDDisconnect1;
    TButton *btnCCDSend1;
    TEdit *edCCDCMD1;
    TMemo *mmoCommLog;
    TGroupBox *grpCCDSetting2;
    TLabel *lbllCCDIP2;
    TLabel *lblCCDPort2;
    TEdit *edCCDIP2;
    TEdit *edCCDPort2;
    TBitBtn *btnCCDConnect2;
    TBitBtn *btnCCDDisconnect2;
    TButton *btnCCDSend2;
    TEdit *edCCDCMD2;
    TGroupBox *grpCCDSetting3;
    TLabel *lbllCCDIP3;
    TLabel *lblCCDPort3;
    TEdit *edCCDIP3;
    TEdit *edCCDPort3;
    TBitBtn *btnCCDConnect3;
    TBitBtn *btnCCDDisconnect3;
    TButton *btnCCDSend3;
    TEdit *edCCDCMD3;
    TGroupBox *grpCCDSetting4;
    TLabel *lbllCCDIP4;
    TLabel *lblCCDPort4;
    TEdit *edCCDIP4;
    TEdit *edCCDPort4;
    TBitBtn *btnCCDConnect4;
    TBitBtn *btnCCDDisconnect4;
    TButton *btnCCDSend4;
    TEdit *edCCDCMD4;
    TLabel *lblLoadOfs;
    TLabel *lblHPOfs1;
    TLabel *lblHPOfs2;
    TLabel *lblInSHTOfs1;
    TLabel *lblInSHTOfs2;
    TLabel *lblInOffsetY;
    TLabel *lblInOffsetX;
    TGroupBox *grpOutMark;
    TLabel *lblAutoOfs3;
    TLabel *lblAutoOfs2;
    TLabel *lblAutoOfs1;
    TLabel *lblOutSHTOfs2;
    TLabel *lblOutSHTOfs1;
    TLabel *lblOutOffsetX;
    TLabel *lblOutOffsetY;
    TLabel *lblOutScaleX;
    TLabel *lbllOutScaleY;
    TEdit *edAutoScaleY3;
    TEdit *edAutoScaleX3;
    TEdit *edAutoOfsY3;
    TEdit *edAutoOfsX3;
    TEdit *edAutoOfsX2;
    TEdit *edAutoOfsY2;
    TEdit *edAutoScaleX2;
    TEdit *edAutoScaleY2;
    TEdit *edAutoScaleY1;
    TEdit *edAutoScaleX1;
    TEdit *edAutoOfsY1;
    TEdit *edAutoOfsX1;
    TEdit *edOutShtOfsX2;
    TEdit *edOutShtOfsY2;
    TEdit *edOutShtScaleX2;
    TEdit *edOutShtScaleY2;
    TEdit *edOutShtScaleY1;
    TEdit *edOutShtScaleX1;
    TEdit *edOutShtOfsY1;
    TEdit *edOutShtOfsX1;
    TEdit *edFixScaleY;
    TEdit *edFixScaleX;
    TEdit *edFixOfsY;
    TEdit *edFixOfsX;
    TLabel *lblFixOfs;
    TGroupBox *grpOutCCD;
    TLabel *lblOutTopCCDToBasePickerOfsY;
    TLabel *lblOutTopCCDToBasePickerOfsX;
    TLabel *lblOutTopCCDToBasePickerOfs;
    TEdit *edOutTopCCDToBasePickerOfsX;
    TEdit *edOutTopCCDToBasePickerOfsY;
    TGroupBox *grpOutPicker;
    TLabel *lblOutX120Pitch;
    TLabel *lblOutX40Pitch;
    TLabel *lblOutPickerA;
    TLabel *lbllOutPickerB;
    TLabel *lbllOutPickerC;
    TLabel *lbllOutPickerD;
    TLabel *lbllOutPickerE;
    TLabel *lbllOutPickerF;
    TLabel *lbllOutPickerG;
    TLabel *lbllOutSuckH;
    TLabel *lbllOutAvg;
    TLabel *lblOutPickerOfsY2;
    TLabel *lblOutPickerOfsX2;
    TLabel *lblOutPickerDOfs;
    TLabel *lblOutPickerHOfs;
    TLabel *lblOutPickerFOfs;
    TLabel *lblOutPickerGOfs;
    TLabel *lblOutPickerEOfs;
    TLabel *lblOutPickerCOfs;
    TLabel *lblOutPickerAOfs;
    TLabel *lblOutPickerOfsY1;
    TLabel *lblOutPickerOfsX1;
    TLabel *lblOutPickerBOfs;
    TEdit *edOutX120PickerA;
    TEdit *edOutX40PickerA;
    TEdit *edOutX40PickerB;
    TEdit *edOutX120PickerB;
    TEdit *edOutX120PickerC;
    TEdit *edOutX40PickerC;
    TEdit *edOutX40PickerD;
    TEdit *edOutX120PickerD;
    TEdit *edOutX120PickerE;
    TEdit *edOutX40PickerE;
    TEdit *edOutX40PickerF;
    TEdit *edOutX120PickerF;
    TEdit *edOutX120PickerG;
    TEdit *edOutX40PickerG;
    TEdit *edOutX40PickerH;
    TEdit *edOutX120PickerH;
    TEdit *edOutX120Avg;
    TEdit *edOutX40Avg;
    TEdit *edOutPickerBOfsY;
    TEdit *edOutPickerBOfsX;
    TEdit *edOutPickerDOfsY;
    TEdit *edOutPickerDOfsX;
    TEdit *edOutPickerFOfsX;
    TEdit *edOutPickerFOfsY;
    TEdit *edOutPickerHOfsY;
    TEdit *edOutPickerHOfsX;
    TEdit *edOutPickerGOfsY;
    TEdit *edOutPickerEOfsY;
    TEdit *edOutPickerGOfsX;
    TEdit *edOutPickerEOfsX;
    TEdit *edOutPickerCOfsX;
    TEdit *edOutPickerCOfsY;
    TEdit *edOutPickerAOfsX;
    TEdit *edOutPickerAOfsY;
    TEdit *edAlignAccuracy;
    TLabel *Label1;
    TEdit *edInTopPixelSizeX;
    TEdit *edInTopPixelSizeY;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TCheckBox *cbEnableInTopPixelSize;
    TCheckBox *cbEnableInBottomPixelSize;
    TLabel *Label5;
    TEdit *edInBottomPixelSizeY;
    TEdit *edInBottomPixelSizeX;
    TLabel *Label6;
    TLabel *Label7;
    TCheckBox *cbEnableOutTopPixelSize;
    TLabel *Label8;
    TEdit *edOutTopPixelSizeY;
    TEdit *edOutTopPixelSizeX;
    TLabel *Label9;
    TLabel *Label10;
    TCheckBox *cbEnableOutBottomPixelSize;
    TLabel *Label11;
    TEdit *edOutBottomPixelSizeY;
    TEdit *edOutBottomPixelSizeX;
    TLabel *Label12;
    TLabel *Label13;

    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall clntscktConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall clntscktAlignDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall clntscktAlignError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall clntscktAlignRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall tmrCCDInitialTimer(TObject *Sender);
    void __fastcall tmrCCDConnectTimer(TObject *Sender);
    void __fastcall tmrProcessGetDataTimer(TObject *Sender);
    void __fastcall sbStartClick(TObject *Sender);
    void __fastcall sbPasueClick(TObject *Sender);
    void __fastcall btnCCDConnectClick(TObject *Sender);
    void __fastcall btnCCDDisconnectClick(TObject *Sender);
    void __fastcall btnCCDSendClick(TObject *Sender);
private:    // User declarations

public:     // User declarations
    __fastcall TfAutoAlignment(TComponent* Owner);

    bool fShow;

    TCAlignmentB *InAlign;
    TCAlignmentB *OutAlign;
    int iMode;              //=0 跑所有流程(Initail Start) ; =1 僅使用 Top CCD 檢查 Teach Mark (Onece Cycle)
    //Data
    bool bEnableVisionSimulate;
    //bool bDoAutoAlignTune;
    bool bRunAutoAlign;
    bool bAutoAlignDone;

    int ReadFile();
    int WriteFile();
    void DoDataToUI();
    void DoUIToData();

    void ShowResult();

    void SendCommand(int iCCD, AnsiString Msg, AnsiString Msg2);
    void SocketRead(int iCCD,AnsiString EthernetBuffer);

    int DoAutoAlign(bool bReset=false);       //Auto Alignment 主流流程
    int iAutoAlignTask;
};
//---------------------------------------------------------------------------
extern PACKAGE TfAutoAlignment *fAutoAlignment;
//---------------------------------------------------------------------------

#endif
