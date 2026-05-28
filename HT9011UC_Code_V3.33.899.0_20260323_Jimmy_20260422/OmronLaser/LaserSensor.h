//---------------------------------------------------------------------------

#ifndef LaserSensorH
#define LaserSensorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ALed.hpp"
#include "HTray.h"
#include "MachineType.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------

enum eResponseCodeL{rcl0000=0, rcl1001, rcl1002, rcl1003, rcl1101,
                    rcl1103, rcl1104, rcl2203, rcl2204, rcl2205, rclTotalCount};


class TfLaserSensor : public TForm
{
__published:    // IDE-managed Components
    TComm *CommLaser1;
    TPageControl *pcLaserSensor;
    TTabSheet *tsLaserSensor;
    TTabSheet *tsLaserSet;
    TScrollBox *ScrollBox1;
    TGroupBox *gbLaserPanel_InArm;
    TPanel *palLaserValue_InArm;
    TGroupBox *gbLaserPanel_Shuttle1_2;
    TPanel *palLaserValue_InShuttle1_2;
    TGroupBox *gbLaserPanel_Shuttle1_1;
    TPanel *palLaserValue_InShuttle1_1;
    TGroupBox *gbLaserPanel_Shuttle2_2;
    TPanel *palLaserValue_InShuttle2_2;
    TGroupBox *gbLaserPanel_Shuttle2_1;
    TPanel *palLaserValue_InShuttle2_1;
    TTimer *Timer1;
    TComm *CommLaser2;
    TComm *CommLaserInArm;
    TButton *btConnect;
    TButton *btSetToZero;
    TButton *btSetToZeroSh1;
    TButton *btSetToZeroSh2;
    TButton *btGetValueSht2_1Row;
    TButton *btGetValueSht2_2Row;
    TButton *btGetValueSht1_1Row;
    TButton *btGetValueSht1_2Row;
    TCheckBox *CheckBox1;
    TButton *btGetValueInArm;
    TButton *btSetToZeroInArm;
    TCheckBox *cbStartPooling;
    TCheckBox *cbEnableShuttleLaser;
    TPanel *Panel1;
    TMemo *memoLaserSensor;
    TMemo *MemoStatus;
    TSpeedButton *sbUpdate;
    TSpeedButton *sbtExit;
    TLabel *labICThickness;
    TEdit *edICThickness;
    TLabel *labLaserThreshold;
    TEdit *edLaserShtThreshold;
    TTabSheet *TabSheet1;
    TButton *btSaveLog;
    TTimer *TimerInArm;
    TCheckBox *cbEnableInArmLaser;
    TPageControl *PageControl1;
    TTabSheet *tsplate1;
    TTabSheet *tsPlate2;
    TGroupBox *gpPlate1_Golden;
    TTMyTray *mtPlate1_Golden;
    TGroupBox *gpPlate2_Golden;
    TTMyTray *mtPlate2_Golden;
    TTMyTray *mtPlate1;
    TTMyTray *mtPlate2;
    TMemo *MemoSh2;
    TMemo *MemoSh1;
    TTMyTray *mtShuttle2;
    TGroupBox *gbShuttle2_Golden;
    TTMyTray *mtShuttle2_Golden;
    TGroupBox *gbShuttle1_Golden;
    TTMyTray *mtShuttle1_Golden;
    TTMyTray *mtShuttle1;
    TLabel *Label1;
    TEdit *edLaserHPThreshold;
    TButton *btSetPlate1GoldenToZero;
    TButton *btSetPlate2GoldenToZero;
    TEdit *edTemp;
    TCheckBox *cbLD40L;
    void __fastcall CommLaser1ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btSetToZeroClick(TObject *Sender);
    void __fastcall CommLaser2ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall cbStartPoolingClick(TObject *Sender);
    void __fastcall btGetValueSht1_2RowClick(TObject *Sender);
    void __fastcall btGetValueSht1_1RowClick(TObject *Sender);
    void __fastcall btGetValueSht2_2RowClick(TObject *Sender);
    void __fastcall btGetValueSht2_1RowClick(TObject *Sender);
    void __fastcall btConnectClick(TObject *Sender);
    void __fastcall CommLaserInArmReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
    void __fastcall btGetValueInArmClick(TObject *Sender);
    void __fastcall btSetToZeroSh2Click(TObject *Sender);
    void __fastcall btSetToZeroSh1Click(TObject *Sender);
    void __fastcall btSetToZeroInArmClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall edICThicknessMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall btSaveLogClick(TObject *Sender);
    void __fastcall TimerInArmTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btSetPlate2GoldenToZeroClick(TObject *Sender);
    void __fastcall btSetPlate1GoldenToZeroClick(TObject *Sender);
    void __fastcall mtPlate2_GoldenMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtPlate1_GoldenMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    AnsiString sResponseCode[2][rclTotalCount];
    AnsiString sEndCode[2][ecTotal];
    void ProcessReceiveData(int iCom, AnsiString Data);
    void ProcessReceiveData0101(int iCom, AnsiString Data);

public:     // User declarations
    __fastcall TfLaserSensor(TComponent* Owner);
    void WriteInfoToMemo(AnsiString asInfo);
    int  iMaxChannel;           //最多幾顆
    int  iCurrentChannel;       //目前做到第幾顆
    int  iCurrentChannelSht;

    int  iMaxStep;              //要掃幾次
    int  iCurrentStep;          //目前掃到第幾次

    int  iLaserTask;
    int  iLaser1Task;
    int  iLaser2Task;
    int  iLaserInArmTask;
    bool bRecvData1, bRecvData2, bRecvDataIn;

    bool bSetToZero;

    bool bSetToZeroSh1;
    bool bSetToZeroSh2;
    bool bSetToZeroInArm;

    bool bSh1_2Row;
    bool bSh1_1Row;
    bool bSh2_2Row;
    bool bSh2_1Row;
    bool bInArm;

    AnsiString GetValueMessage[2][2];
    AnsiString GetValMessASCII[2][2];
    AnsiString GetValueMessageInArm;
    AnsiString GetValMessASCIIInArm;

    AnsiString SetToZeroMessage[2][2];
    AnsiString SetToZeroMessASCII[2][2];
    AnsiString SetToZeroMessageInArm;
    AnsiString SetToZeroMessASCIIInArm;

    void __fastcall ReadFile();
    void __fastcall ReadLaserFile();
    void __fastcall SaveShuttleLaserValue(int iSht);
    void __fastcall SaveInArmLaserValue();
    void __fastcall WriteToMemo(TMemo *Memo, AnsiString Str);

    TPanel *MyPanel[2][2];

    bool fShow;

    void __fastcall DoIniDataToForm();
    void InitLaserEdtList();               //Steven 20190614 : 重新整理Edit to 變數的方式
};
//---------------------------------------------------------------------------
extern PACKAGE TfLaserSensor *fLaserSensor;
//---------------------------------------------------------------------------
#endif
