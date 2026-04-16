//---------------------------------------------------------------------------
#ifndef uMotorTestH
#define uMotorTestH
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
#include <Forms.hpp>
#include <Vector>

using namespace std;
//---------------------------------------------------------------------------
class TMotorTestClass  : public TComponent                                      //Stven 20120823 : Home元件改用Vector新增
{
    public:
        int         index;
        bool        Visible;
        TLabel      *labName;
        TEdit       *edPos1;
        TEdit       *edPos2;
        TCheckBox   *cbUsing;
        TPanel      *MotorPanel;
        TLabel      *MotorLabel;
        TALed       *MotorLed[10];
        TALed       *MotorHomeLed;

        TMotorTestClass(int MotNo, bool bView=true);
        void        SetPosition(int Top, int Left);
        bool        AllMotMoveFlag;
};
//---------------------------------------------------------------------------
class TfMotorTest : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet6;
    TPanel *Panel15;
    TTabSheet *TabSheet7;
    TPanel *Panel24;
    TBevel *Bevel5;
    TSpeedButton *SpeedButton44;
    TSpeedButton *SpeedButton45;
    TSpeedButton *SpeedButton46;
    TSpeedButton *SpeedButton47;
    TLabel *Label82;
    TLabel *Label83;
    TLabel *Label84;
    TLabel *Label85;
    TLabel *Label87;
    TLabel *Label88;
    TLabel *Label79;
    TLabel *Label80;
    TLabel *Label81;
    TLabel *Label97;
    TSpeedButton *SpeedButton4;
    TSpeedButton *SpeedButton5;
    TLabel *Label102;
    TSpeedButton *SpeedButton8;
    TLabel *Label105;
    TLabel *Label106;
    TLabel *Label107;
    TScrollBar *ScrollBar2;
    TComboBox *ComboBox4;
    TEdit *Edit14;
    TEdit *Edit16;
    TEdit *Edit18;
    TEdit *edtSpeedIndex1;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TEdit *Edit12;
    TEdit *Edit13;
    TPanel *Panel25;
    TPanel *Panel26;
    TBevel *Bevel6;
    TSpeedButton *SpeedButton50;
    TSpeedButton *SpeedButton51;
    TSpeedButton *SpeedButton52;
    TSpeedButton *SpeedButton53;
    TLabel *Label86;
    TLabel *Label89;
    TLabel *Label90;
    TLabel *Label91;
    TLabel *Label92;
    TLabel *Label93;
    TLabel *Label98;
    TLabel *Label99;
    TLabel *Label100;
    TLabel *Label101;
    TSpeedButton *SpeedButton6;
    TSpeedButton *SpeedButton7;
    TLabel *Label103;
    TSpeedButton *SpeedButton9;
    TLabel *Label108;
    TLabel *Label109;
    TLabel *Label110;
    TScrollBar *ScrollBar3;
    TComboBox *ComboBox5;
    TEdit *Edit24;
    TEdit *Edit102;
    TEdit *Edit103;
    TEdit *edtSpeedIndex2;
    TEdit *Edit7;
    TEdit *Edit8;
    TEdit *Edit10;
    TEdit *Edit11;
    TEdit *Edit15;
    TEdit *Edit17;
    TPanel *Panel27;
    TTimer *Timer1;
    TEdit *Edit2;
    TPanel *palExit;
    TPanel *Panel2;
    TPanel *Panel23;
    TSpeedButton *btnHighSpeed;
    TSpeedButton *btnHomeHigh;
    TSpeedButton *btnHomeLow;
    TSpeedButton *btnLowSpeed;
    TSpeedButton *btnSoftPPos;
    TSpeedButton *btnSoftNPos;
    TSpeedButton *btnRange;
    TSpeedButton *btnRate;
    TSpeedButton *btnReloadMotorData;
    TSpeedButton *btnSetRate;
    TSpeedButton *btnSetRange;
    TEdit *edtMotorRate;
    TEdit *edtMotorRange;
    TButton *btResetMNet;
    TPanel *Panel20;
    TLabel *Label38;
    TSpeedButton *sbMotorTest_JogN;
    TSpeedButton *sbMotorTest_JogP;
    TLabel *Label40;
    TLabel *Label41;
    TSpeedButton *sbMotorTest_MoveN;
    TSpeedButton *sbMotorTest_MoveP;
    TSpeedButton *btnLoopMove;
    TSpeedButton *btnSetPosP;
    TSpeedButton *btnSetPosN;
    TLabel *Label39;
    TSpeedButton *btnHome;
    TSpeedButton *btnGoSoftP;
    TSpeedButton *btnGoSoftN;
    TLabel *Label112;
    TLabel *Label114;
    TLabel *Label115;
    TLabel *Label116;
    TLabel *Label117;
    TLabel *Label118;
    TLabel *Label119;
    TLabel *Label120;
    TLabel *Label121;
    TLabel *Label122;
    TALed *ALed1;
    TALed *ALed2;
    TALed *ALed3;
    TALed *ALed4;
    TALed *ALed5;
    TALed *ALed6;
    TALed *ALed7;
    TALed *ALed8;
    TALed *ALed9;
    TLabel *Label123;
    TLabel *Label1;
    TLabel *lblLoopCount;
    TLabel *lblAvgTime;
    TLabel *Label4;
    TLabel *Label5;
    TSpeedButton *btnGo;
    TSpeedButton *btnServoOff;
    TSpeedButton *btnMotorPower;
    TLabel *lblJogPTime;
    TLabel *lblJogNTime;
    TALed *ALed10;
    TLabel *Label10;
    TLabel *Label12;
    TPanel *pnlMotorAlias;
    TComboBox *cbbInterval;
    TComboBox *cbbLoopWaitTime;
    TScrollBar *scrlbrMotorSpeed;
    TEdit *edtCommandPos;
    TEdit *edtSpeed;
    TEdit *edtHomeOffset;
    TRadioGroup *select;
    TPanel *pnlEncoderPos;
    TPanel *pnlMotor;
    TBitBtn *BitBtn1;
    TComboBox *cbbMotorName;
    TStringGrid *strngrdMotor;
    TCheckBox *cbUsing;
    TLabel *labName;
    TEdit *Edit9;
    TEdit *Edit26;
    TPanel *Panel4;
    TTabSheet *TabSheet1;
    TTabSheet *ts1;
    TTimer *Timer2;
    TEdit *edDelayTime;
    TEdit *edPitech;
    TRadioGroup *rgMoveType;
    TBitBtn *BitBtn2;
    TMemo *Memo1;
    TBitBtn *BitBtn3;
    TRadioGroup *rgAxis;
    TSpeedButton *btnSaveLogLightScaleData;
    TLabel *InArmX_Go;
    TLabel *lbl1;
    TLabel *lbl2;
    TLabel *lbl3;
    TLabel *lbl4;
    TLabel *lbl5;
    TLabel *lbl6;
    TLabel *lbl7;
    TMemo *mmo1;
    TMemo *mmo2;
    TMemo *mmo3;
    TMemo *mmo4;
    TMemo *mmo5;
    TMemo *mmo6;
    TMemo *mmo7;
    TMemo *mmo8;
    TLabel *lblRealSpeed;
    TTabSheet *tsMotorDatabase;
    TPanel *Panel1;
    TSpeedButton *sbtReload;
    TSpeedButton *sbUpdate;
    TStringGrid *strngrdMotorData;
    TSpeedButton *btnAddMotor;
    TSpeedButton *btnDeleteMotor;
    TSpeedButton *btnModify;
    TLabel *lblMotorName;
    TPanel *pnlStop;
    TSpeedButton *btnStop;
    void __fastcall lM00Click(TObject *Sender);
    void __fastcall lpA00Click(TObject *Sender);
    void __fastcall scrlbrMotorSpeedScroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
    void __fastcall sbMotorTest_JogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton26MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sbMotorTest_JogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sbMotorTest_JogPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnSetPosPClick(TObject *Sender);
    void __fastcall btnSetPosNClick(TObject *Sender);
    void __fastcall btnGoSoftPClick(TObject *Sender);
    void __fastcall btnGoSoftNClick(TObject *Sender);
    void __fastcall btnHomeClick(TObject *Sender);
    void __fastcall strngrdMotorSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall sbMotorTest_MoveNClick(TObject *Sender);
    void __fastcall sbMotorTest_MovePClick(TObject *Sender);
    void __fastcall btnLoopMoveClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnSetRateClick(TObject *Sender);
    void __fastcall btnSetRangeClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall btnHighSpeedClick(TObject *Sender);
    void __fastcall btnLowSpeedClick(TObject *Sender);
    void __fastcall btnHomeHighClick(TObject *Sender);
    void __fastcall btnHomeLowClick(TObject *Sender);
    void __fastcall btnSoftPPosClick(TObject *Sender);
    void __fastcall btnSoftNPosClick(TObject *Sender);
    void __fastcall btnRangeClick(TObject *Sender);
    void __fastcall btnRateClick(TObject *Sender);
    void __fastcall SpeedButton4Click(TObject *Sender);
    void __fastcall SpeedButton5Click(TObject *Sender);
    void __fastcall SpeedButton8Click(TObject *Sender);
    void __fastcall SpeedButton6Click(TObject *Sender);
    void __fastcall SpeedButton7Click(TObject *Sender);
    void __fastcall SpeedButton9Click(TObject *Sender);
    void __fastcall ScrollBar2Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
    void __fastcall ScrollBar3Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
    void __fastcall SpeedButton44Click(TObject *Sender);
    void __fastcall SpeedButton45Click(TObject *Sender);
    void __fastcall SpeedButton50Click(TObject *Sender);
    void __fastcall SpeedButton51Click(TObject *Sender);
    void __fastcall SpeedButton46MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton46MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton47MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton47MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton52MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton52MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton53MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton53MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtSpeedChange(TObject *Sender);
    void __fastcall btnGoClick(TObject *Sender);
    void __fastcall btnMotorPowerClick(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall btnServoOffClick(TObject *Sender);
    void __fastcall ck00Click(TObject *Sender);
    void __fastcall btnReloadMotorDataClick(TObject *Sender);
    void __fastcall edtSpeedClick(TObject *Sender);
    void __fastcall btResetMNetClick(TObject *Sender);
    void __fastcall palExitClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall edDelayTimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edPitechMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnSaveLogLightScaleDataClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall rgAxisClick(TObject *Sender);
    void __fastcall sbtReloadClick(TObject *Sender);
    void __fastcall strngrdMotorDataSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall btnAddMotorClick(TObject *Sender);
    void __fastcall btnDeleteMotorClick(TObject *Sender);
    void __fastcall btnModifyClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall strngrdMotorDataDblClick(TObject *Sender);
    void __fastcall strngrdMotorDataMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall strngrdMotorDataDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
    void __fastcall strngrdMotorDataDragDrop(TObject *Sender,
          TObject *Source, int X, int Y);
    void __fastcall edtMotorRateClick(TObject *Sender);
    void __fastcall edtMotorRangeClick(TObject *Sender);
private:    // User declarations
    int SelMotSpeed;

    int iSelectRow;
    int iSelectCol;
    int iDragCol;
    int iDragRow;

public:     // User declarations
    __fastcall TfMotorTest(TComponent* Owner);
    int ActiveIndex;
    vector<TMotorTestClass *>MotorTestClass;      //Stven 20120831 : MotorTest元件改用Vector新增
    void __fastcall DoLoopMove();
    void __fastcall UpdateMotorParameter();
    int LoopTask;
    bool fShow;
    void __fastcall UpdatePreasureScreen();
    void __fastcall UpdateMotorLed();
    void __fastcall ShowMotorSelect(int Index, int Attr);
    bool bSingleHome;
    int  iSingleHomeIndex;
//    DWORD dwStartTickCount;
//    DWORD dwNowTickCount;
    DWORD dwLoopCount;
    bool __fastcall IsMotorCanRun(bool);
    bool __fastcall DoGaliHome(int Index);

    void LightScale(bool Reset);
    int iUseAxis;
    //void __fastcall ProcessRotater(); //Steven 20100831 : 多了塊魚

    TQPF_Timer tLoopMoveTimer;
    TQPF_Timer tAvgLoopMoveTimer;
    DWORD Average;
    void LockAllButton(bool bLock);
    void AllBtnUp();
};
//---------------------------------------------------------------------------
extern PACKAGE TfMotorTest *fMotorTest;
//---------------------------------------------------------------------------
#endif

