//---------------------------------------------------------------------------

#ifndef LtcSensorH
#define LtcSensorH
//---------------------------------------------------------------------------
#include "Type_def.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
//---------------------------------------------------------------------------
class TfLtcSensor : public TForm
{
__published:    // IDE-managed Components
    TPanel *btnClose;
    TPanel *Panel1;
    TTimer *TimerLtcSensor;
    TTimer *DellTest;
    TPanel *pnlTop;
    TALed *ledSht2Servo;
    TLabel *lblSht1Servo;
    TALed *ledSht1Servo;
    TLabel *lblSht2Servo;
    TLabel *lblLtcCount;
    TLabel *Label22;
    TLabel *Label23;
    TButton *btSetLtc;
    TButton *btSh1Servo;
    TButton *btSh2Servo;
    TButton *BtnDellTest;
    TScrollBar *ScrollBar1;
    TPanel *pnlCenter;
    TGroupBox *gbOutSh1Z;
    TLabel *labOut1_Z1_Axis;
    TLabel *labOut1_Z2_Axis;
    TLabel *labOut1_Z1_Count;
    TLabel *labOut1_Z2_Count;
    TALed *ledOut1_1;
    TALed *ledOut1_2;
    TButton *btGetSh1Ltc;
    TMemo *Memo_Out1Z1;
    TMemo *Memo_Out1Z2;
    TGroupBox *gbOutSh2Z;
    TLabel *labOut2_Z1_Axis;
    TLabel *labOut2_Z2_Axis;
    TLabel *labOut2_Z1_Count;
    TLabel *labOut2_Z2_Count;
    TALed *ledOut2_1;
    TALed *ledOut2_2;
    TMemo *Memo_Out2Z1;
    TMemo *Memo_Out2Z2;
    TButton *btGetSh2Ltc;
    TGroupBox *gbOutSh1Y;
    TLabel *labOut1_Y_Axis;
    TLabel *labOut1_Y_Count;
    TALed *ledOut1_Y;
    TButton *btGetSh1YLtc;
    TMemo *Memo_Out1Y;
    TGroupBox *gbOutSh2Y;
    TLabel *labOut2_Y_Axis;
    TLabel *labOut2_Y_Count;
    TALed *ledOut2_Y;
    TMemo *Memo_Out2Y;
    TButton *btGetSh2YLtc;
    TGroupBox *gbInSh1Y;
    TLabel *labIn1_Y_Axis;
    TLabel *labIn1_Y_Count;
    TALed *ledIn1_Y;
    TButton *btGetInSh1YLtc;
    TMemo *Memo_In1Y;
    TGroupBox *gbInSh2Y;
    TLabel *labIn2_Y_Axis;
    TLabel *labIn2_Y_Count;
    TALed *ledIn2_Y;
    TMemo *Memo_In2Y;
    TButton *btGetInSh2YLtc;
    TPanel *pnlCaption;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label13;
    TGroupBox *gbOutSh3Z;
    TLabel *labOut3_Z1_Axis;
    TLabel *labOut3_Z2_Axis;
    TLabel *labOut3_Z1_Count;
    TLabel *labOut3_Z2_Count;
    TALed *ledOut3_1;
    TALed *ledOut3_2;
    TMemo *Memo_Out3Z1;
    TMemo *Memo_Out3Z2;
    TButton *btGetSh3Ltc;
    TGroupBox *gbOutSh3Y;
    TLabel *labOut3_Y_Axis;
    TLabel *labOut3_Y_Count;
    TALed *ledOut3_Y;
    TButton *btGetSh3YLtc;
    TMemo *Memo_Out3Y;
    TButton *btSh3Servo;
    TALed *ledSht3Servo;
    TLabel *lblSht3Servo;
    void __fastcall btSetLtcClick(TObject *Sender);
    void __fastcall btGetSh1LtcClick(TObject *Sender);
    void __fastcall btSh1ServoClick(TObject *Sender);
    void __fastcall btSh2ServoClick(TObject *Sender);
    void __fastcall TimerLtcSensorTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btGetSh2LtcClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall BtnDellTestClick(TObject *Sender);
    void __fastcall DellTestTimer(TObject *Sender);
    void __fastcall ScrollBar1Change(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btSh3ServoClick(TObject *Sender);
    void __fastcall btGetSh3LtcClick(TObject *Sender);
private:    // User declarations
    U16 AxisNumShuttle1;
    U16 AxisNumShuttle2;
    U16 AxisNumShuttle3;
    U16 LatchInputNo0;
    U16 LatchInputNo1;
    U16 LatchInputNo2;
    U16 LatchInputNo3;
    U16 LatchInputNo4;
    U16 LatchInputNo5;
    U16 LatchInputNo6;
    U16 LatchInputNo7;

    U16 length;
    U32 IO_status0;
    U32 IO_status1;
    U32 IO_status2;
    U32 IO_status3;
public:     // User declarations
    __fastcall TfLtcSensor(TComponent* Owner);
    void __fastcall SetLtcSensor(int iSht);                                     //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
    void SetLtcSensor_SYNTEK(int iShuttle);                                     //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
    void SetLtcSensor_SMC(int iShuttle);                                        //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
    void __fastcall GetLtcSensor(int iShuttle);
    void __fastcall ClearLtcSensor(int iShuttle);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
    int GetLtcDataCnt_SMC(int iShuttl);                                         //Sam 20211115 : Latch 清除增加檢查
    void GetLtcSensor_SYNTEK(int iShuttle);
    void GetLtcSensor_SMC(int iShuttle);
    int __fastcall GetLatchTotalLength();
    int GetLatchTotalLength_SYNTEK();
    void ABSLatchDataTable(U16 uLatchDataCnt,I32 *LatchDataTable0);             //Jimmychiu 20221208 abs shuttle1 LatchData
    int GetLatchTotalLength_SMC();
    bool bShow;
    //Shuttle 1
    U16 uGetAxisCounterNo0;      //哪一軸
    U16 uGetAxisCounterNo1;
    I16 iGetAxisCounterNo0;      //哪一軸
    I16 iGetAxisCounterNo1;
    I16 iGetAxisCounterNo2;
    I16 iGetAxisCounterNo3;
    I16 iGetAxisCounterNo4;      //哪一軸
    I16 iGetAxisCounterNo5;
    I16 iGetAxisCounterNo6;
    I16 iGetAxisCounterNo7;
    I16 iGetSortCounter0;
    I16 iGetSortCounter1;
    I16 iGetSortCounter2;
    I16 iGetSortCounter3;    

    U16 LatchDataCnt0;          //計數器
    U16 LatchDataCnt1;
    U16 LatchDataCnt2;
    U16 LatchDataCnt3;
    U16 LatchDataCnt4;
    U16 LatchDataCnt5;
    U16 LatchDataCnt6;
    U16 LatchDataCnt7;
    U16 LatchDataCntSort0;
    U16 LatchDataCntSort1;
    U16 LatchDataCntSort2;
    U16 LatchDataCntSort3;
        
    I32 LatchDataTable0[1024];  //Encoder位置
    I32 LatchDataTable1[1024];
    I32 LatchDataTable2[1024];
    I32 LatchDataTable3[1024];
    I32 LatchDataTable4[1024];
    I32 LatchDataTable5[1024];
    I32 LatchDataTable6[1024];
    I32 LatchDataTable7[1024];
    I32 LatchSortDataTable0[1024];
    I32 LatchSortDataTable1[1024];
    I32 LatchSortDataTable2[1024];
    I32 LatchSortDataTable3[1024];

    short LatchCnt_Pulse0[2];
    short LatchCnt_Pulse1[2];
    short LatchCnt_Pulse2[2];
    short LatchCnt_Pulse3[2];
    short LatchCnt_Pulse4[2];
    short LatchCnt_Pulse5[2];
    short LatchCnt_Pulse6[2];
    short LatchCnt_Pulse7[2];
//---------------------------------

    short LatchDataCnt;
    short AxisCounterNo;

    void DellTestP(bool Reset);
    TStringList *str;

};
//---------------------------------------------------------------------------
extern PACKAGE TfLtcSensor *fLtcSensor;
//---------------------------------------------------------------------------
#endif
