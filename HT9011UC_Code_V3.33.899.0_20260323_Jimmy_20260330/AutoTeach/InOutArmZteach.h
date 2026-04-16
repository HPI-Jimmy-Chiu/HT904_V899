//---------------------------------------------------------------------------

#ifndef InOutArmZteachH
#define InOutArmZteachH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <math.h>
//---------------------------------------------------------------------------
class TZteach : public TForm
{
__published:    // IDE-managed Components
        TPanel *Panel1;
        TGroupBox *GroupBox8;
        TLabel *Label46;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label53;
        TLabel *Label58;
        TLabel *Label59;
        TLabel *Label60;
        TLabel *Label61;
        TEdit *AEdit1;
        TEdit *AEdit2;
        TEdit *AEdit3;
        TEdit *AEdit4;
        TEdit *AEdit5;
        TEdit *AEdit6;
        TEdit *AEdit7;
        TEdit *AEdit8;
        TSpeedButton *SpeedButton14;
        TGroupBox *GroupBox9;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label70;
        TLabel *Label85;
        TLabel *Label101;
        TLabel *Label102;
        TEdit *AOutEdit7;
        TEdit *AOutEdit5;
        TEdit *AOutEdit3;
        TEdit *AOutEdit1;
        TEdit *AOutEdit8;
        TEdit *AOutEdit6;
        TEdit *AOutEdit4;
        TEdit *AOutEdit2;
        TTimer *AutoTimer;
        TSpeedButton *SpeedButton11;
        TMemo *Memo1;
        TBitBtn *BitBtn1;
        TRadioGroup *AutoTeachSingle;
        TStringGrid *StringGrid1;
        TButton *SendData;
    TRadioGroup *rgTeachMode;
    TGroupBox *gbInarm;
    TButton *SendInarm;
    TButton *SendOutarm;
    TSpeedButton *sbHotPlate2;
    TSpeedButton *sbHotPlate1;
    TSpeedButton *sbInSht2;
    TSpeedButton *sbInSht1;
    TGroupBox *gbOutArm;
    TSpeedButton *sbFix1;
    TSpeedButton *sbFix2;
    TSpeedButton *sbFix3;
    TSpeedButton *sbAuto3;
    TSpeedButton *sbAuto2;
    TSpeedButton *sbAuto1;
    TSpeedButton *sbOutSht2;
    TSpeedButton *sbOutSht1;
    TSpeedButton *sbInZ_Calibration;
    TSpeedButton *sbOutZ_Calibration;
    TSpeedButton *sbInRotate;
    TSpeedButton *sbOutRotate;
    TSpeedButton *sbLoader;
    TStringGrid *sgThreePointTeach;
    TPanel *PanelTeachPosSelect;
    TCheckBox *cbEnableUseAreaZCalbration;
    TCheckBox *CbEnRecordVacuum;
        TPanel *Panel2;
        TSpeedButton *MotorInArmX;
        TSpeedButton *MotorInArmY;
        TSpeedButton *SpeedButton13;
        TSpeedButton *SpeedButton12;
        TComboBox *ComboBox1;
        TPanel *Panel3;
        TPanel *pnlEncoderPos;
    TEdit *edtPosition;
        TLabel *Label4;
        TSpeedButton *SpeedButton1;
    TButton *Button1;
        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall SendInarmClick(TObject *Sender);
        void __fastcall SendOutarmClick(TObject *Sender);
        void __fastcall SendDataClick(TObject *Sender);
        void __fastcall StringGrid1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall AutoTeachSingleClick(TObject *Sender);
    void __fastcall sbLoaderClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall sgThreePointTeachDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall rgTeachModeClick(TObject *Sender);
    void __fastcall cbEnableUseAreaZCalbrationClick(TObject *Sender);
        void __fastcall MotorInArmXClick(TObject *Sender);
        void __fastcall SpeedButton13Click(TObject *Sender);
        void __fastcall SpeedButton12Click(TObject *Sender);
        void __fastcall AutoTimerTimer(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton14Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall AEdit1Click(TObject *Sender);
    void __fastcall edtPositionClick(TObject *Sender);
private:    // User declarations
        HDC DcSGScale;
        TCanvas *pCanvasScale;
public:     // User declarations
        __fastcall TZteach(TComponent* Owner);
        void __fastcall GetTechPos();
        void __fastcall InitAutoPosTask();
        void __fastcall SetAutoTeachMode();
        void __fastcall DisplayAutoTeachPos(int iInArm, int TeachPos);
        void __fastcall InOutArmZHome(int iInArm);                              //kevin 20171101
        void __fastcall TZteach::UpdateMotorTeachMonitor(int Index);            //kevin 20210227 add X y move
        void __fastcall SaveSetupFile(AnsiString szDir, AnsiString szFilePath, int iSelPartData, int SpecialMode, bool bReset);//kevin 20210312 Save offset pos
        void __fastcall SaveFile(int iSelPartData, int SpecialMode, bool bReset);   //kevin 20210312 Save offset pos

        bool OpenSuckOn(class TMyKitSuck &Ptr, bool CheckSuckOn);               //kevin 20131015 開真空 確認真空是否正常
        bool CheckSuckOn(class TMyKitSuck &Ptr, int X, int Y);                  //kevin 20131015 開真空 確認真空是否正常
        bool AutoTeachZ(bool Reset, int InArm, bool bUseSingleZDoCalibration, int TeachPos);
        bool MoveInArmXYToTeach(int ZNumber, int iInArm);
        bool DoZHome();
        bool MoveInArmXYToTeachPos(int iInArm, int SendXpos, int SendYpos);     //移動到各量測位置
        void AutoTeachPos();
        double __fastcall Angle(double x1, double y1, double x2, double y2, bool bCompareXY);
        int  bVacuumOnOff(int iInArm, int iZMot);                               // 0: inarm 1: out arm 3: Index
        bool MoveInOutArmXYToTeachPos(int TeachPos,int iInArm);

        void Recode();
        bool bInArmZHome[2][4];
        bool bOutArmZHome[2][4];
        bool fShow;
        int  iINArmTeach;                   //Inarm 目前要TEACH 位置
        int  iOutArmTeach;                  //Outarm 目前要TEACH 位置
        int  iTeachPos;                     // 目前要校正哪一個位置
        bool SingleTeach;                   //單一個選項TEACH
        int  AutoTeachiTeachPos;            //目前FULL AUTOTEACH是校正到第幾個
        bool bSendDataToTeach[2][10];       //勾選要送資料
        int  InoutArmPos[2][2][10];         // 0: inarm  1:outarm     0:X 1: Y   0-9 : shuttle1 2
        int  b3PointTeach;                  //JerryYang 20161012 是否使用3點校正Teaching

        bool bEnableUseAreaZCalbration;
        bool bZTeachOffset;                 //kevin 20210204 Z Teach Offset
        int  iTeachMode;                      //kevin 20210208 in out arm teach
        int  ActiveMotorIndex;
        AnsiString sVacuumData;
};
//---------------------------------------------------------------------------
extern PACKAGE TZteach *Zteach;
//---------------------------------------------------------------------------
#endif
