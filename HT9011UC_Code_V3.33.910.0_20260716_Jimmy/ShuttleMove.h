//---------------------------------------------------------------------------

#ifndef ShuttleMoveH
#define ShuttleMoveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "HTray.h"
//---------------------------------------------------------------------------
class TfShuttleMove : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TGroupBox *gbShuttleMove;
    TButton *btShu2Right;
    TButton *btShu1Right;
    TButton *btShu2Left;
    TButton *btShu1Left;
    TSpeedButton *sbShuttleSensor;
    TGroupBox *gbScanOutShuttle;
    TButton *btScanOutShu2;
    TButton *btScanOutShu1;
    TPanel *Panel3;
    TGroupBox *gbInFiberCheck;
    TSpeedButton *btnInSH1Sen7DetectPos;
    TSpeedButton *btnInSH2Sen7DetectPos;
    TEdit *edInSH2Sen7DetectPos;
    TEdit *edInSH1Sen7DetectPos;
    TGroupBox *gbBarCode;
    TSpeedButton *btInSH1BarCodePos;
    TSpeedButton *btInSH2BarCodePos;
    TEdit *edInSH1BarCodePos;
    TEdit *edInSH2BarCodePos;
    TGroupBox *gbOutFiberCheck;
    TButton *btRetry;
    TSpeedButton *btOutSH1ZDetectPos;
    TSpeedButton *btOutSH2ZDetectPos;
    TSpeedButton *btOutSH1OneRowDetectPos;
    TSpeedButton *btOutSH2OneRowDetectPos;
    TEdit *edOutSH1OneRowDetectPos;
    TEdit *edOutSH2OneRowDetectPos;
    TEdit *edOutSH1ZDetectPos;
    TEdit *edOutSH2ZDetectPos;
    TTMyTray *mtedInSHSen7DetectPos;
    TTMyTray *mtInSHBarCodePos;
    TTMyTray *mtOutSHDetectPos;
    TMemo *meShuttleMaintain;
    TPanel *Panel4;
    TSpeedButton *sbUpdate;
    TSpeedButton *sbtExit;
    TRadioButton *rbTemp;
    TPanel *palSh2Encoder;
    TPanel *palSh1Encoder;
    TSpeedButton *sbSensorLatch;
    TButton *btnTStep;
    TGroupBox *gbInFiberCheckShtSnLct;
    TSpeedButton *btnInSH1SenICDetectPos;
    TSpeedButton *btnInSH2SenICDetectPos;
    TSpeedButton *btnInSH1SenICDetectAction;
    TSpeedButton *btnInSH2SenICDetectAction;
    TEdit *edInSH2SenICDetectPos;
    TEdit *edInSH1SenICDetectPos;
    TTMyTray *mtedInSHSenICDetectPos;
    TEdit *edInSH1SenICDetectZPos;
    TEdit *edInSH2SenICDetectZPos;
    TButton *btStart;
    TSpeedButton *sbBarCode;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btRetryClick(TObject *Sender);
    void __fastcall ShuttleMoveClick(TObject *Sender);
    void __fastcall sbShuttleSensorClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbSensorLatchClick(TObject *Sender);
    void __fastcall edInSH2Sen7DetectPosClick(TObject *Sender);
    void __fastcall edInSH1Sen7DetectPosClick(TObject *Sender);
    void __fastcall btnTStepClick(TObject *Sender);
    void __fastcall mtOutSHDetectPosMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edInSH1SenICDetectZPosClick(TObject *Sender);
    void __fastcall edInSH2SenICDetectZPosClick(TObject *Sender);
    void __fastcall btStartClick(TObject *Sender);
private:    // User declarations
    int iShuttleMode;
    int iMoveToTarget;
    DWORD LastClickTime;
    int LastClickX, LastClickY;
public:     // User declarations
    __fastcall TfShuttleMove(TComponent* Owner);
    bool fShow;
    void DoShuttleMove();
    void DoShuttle1MoveToOutSensor(int iUseSensor,int iTarget=-1);
    void DoShuttle2MoveToOutSensor(int iUseSensor,int iTarget=-1);
    void DoScanDeviceOnShuttle1();
    void DoScanDeviceOnShuttle2();
    void DoShuttleMoveToSide(int iShuttle, int iSide);
    void DoShuttleMoveToBarCodePos1(int iShuttle);
    void DoInShuttleCheckByStep(int iShuttle);
    void DoInShuttleChkStackByStep(int iShuttle);
    void DoInShuttleChkStackAction(int iShuttle);                               //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    void ReadData();                                                            //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    int iShuttleMoveTask;
    int iShuttleMoveByStepTask;
    bool bShuttleRetry;
    void ShowShuttleSensorPosition();
    void SaveInOutArmZHeight();
    bool bSetupStep;
    bool VerifyInArmSafetyPosition();
    bool VerifyOutArmSafetyPosition();
    bool VerifyIndexArmSafetyPosition();
    bool VerifySafetyPositionInOutIndex();
    void ShowShuttleMaintainInfo(AnsiString sMsg);
};
//---------------------------------------------------------------------------
extern PACKAGE TfShuttleMove *fShuttleMove;
//---------------------------------------------------------------------------
#endif
