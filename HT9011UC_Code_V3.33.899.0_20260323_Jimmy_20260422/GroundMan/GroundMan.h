//---------------------------------------------------------------------------

#ifndef GroundManH
#define GroundManH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "ALed.hpp"
#include "MyLedLane.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfGroundMan : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsMain;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TSpeedButton *spbStartCom;
    TSpeedButton *spbStopCom;
    TGroupBox *gbBoard0;
    TLabel *labValue_0_0;
    TLabel *labValue_0_1;
    TLabel *labValue_0_2;
    TLabel *labValue_0_3;
    TLabel *labValue_0_4;
    TLabel *labValue_0_5;
    TLabel *labValue_0_6;
    TLabel *labValue_0_7;
    TLabel *labCH_0_0;
    TLabel *labCH_0_1;
    TLabel *labCH_0_2;
    TLabel *labCH_0_3;
    TLabel *labCH_0_4;
    TLabel *labCH_0_5;
    TLabel *labCH_0_6;
    TLabel *labCH_0_7;
    TMyLedLane *led_0_0;
    TMyLedLane *led_0_1;
    TMyLedLane *led_0_2;
    TMyLedLane *led_0_3;
    TMyLedLane *led_0_4;
    TMyLedLane *led_0_5;
    TMyLedLane *led_0_6;
    TMyLedLane *led_0_7;
    TLabel *Label9;
    TComm *comGM;
    TTimer *Timer1;
    TGroupBox *gbBoard1;
    TLabel *labValue_1_0;
    TLabel *labValue_1_1;
    TLabel *labValue_1_2;
    TLabel *labValue_1_3;
    TLabel *labValue_1_4;
    TLabel *labValue_1_5;
    TLabel *labValue_1_6;
    TLabel *labValue_1_7;
    TLabel *labCH_1_0;
    TLabel *labCH_1_1;
    TLabel *labCH_1_2;
    TLabel *labCH_1_3;
    TLabel *labCH_1_4;
    TLabel *labCH_1_5;
    TLabel *labCH_1_6;
    TLabel *labCH_1_7;
    TMyLedLane *led_1_0;
    TMyLedLane *led_1_1;
    TMyLedLane *led_1_2;
    TMyLedLane *led_1_3;
    TMyLedLane *led_1_4;
    TMyLedLane *led_1_5;
    TMyLedLane *led_1_6;
    TMyLedLane *led_1_7;
    TLabel *Label10;
    TGroupBox *gbBoard2;
    TLabel *labValue_2_0;
    TLabel *labValue_2_1;
    TLabel *labValue_2_2;
    TLabel *labValue_2_3;
    TLabel *labValue_2_4;
    TLabel *labValue_2_5;
    TLabel *labValue_2_6;
    TLabel *labValue_2_7;
    TLabel *labCH_2_0;
    TLabel *labCH_2_1;
    TLabel *labCH_2_2;
    TLabel *labCH_2_3;
    TLabel *labCH_2_4;
    TLabel *labCH_2_5;
    TLabel *labCH_2_6;
    TLabel *labCH_2_7;
    TMyLedLane *led_2_0;
    TMyLedLane *led_2_1;
    TMyLedLane *led_2_2;
    TMyLedLane *led_2_3;
    TMyLedLane *led_2_4;
    TMyLedLane *led_2_5;
    TMyLedLane *led_2_6;
    TMyLedLane *led_2_7;
    TLabel *Label11;
    TTabSheet *tsLog;
    TMemo *mmGroundManLog;
    TLabel *labStatus;
    TLabel *labBoardVersion;
    TLabel *labBoardOhrm;
    TLabel *labBoardOhrm2;
    TLabel *labBoardOhrm3;
    TLabel *labBoardVersion2;
    TLabel *labBoardVersion3;
    TLabel *labResetByStart;
    TLabel *labUseOffset;
    TGroupBox *gbBoard3;
    TLabel *labValue_3_0;
    TLabel *labValue_3_1;
    TLabel *labValue_3_2;
    TLabel *labValue_3_3;
    TLabel *labValue_3_4;
    TLabel *labValue_3_5;
    TLabel *labValue_3_6;
    TLabel *labValue_3_7;
    TLabel *labCH_3_0;
    TLabel *labCH_3_1;
    TLabel *labCH_3_2;
    TLabel *labCH_3_3;
    TLabel *labCH_3_4;
    TLabel *labCH_3_5;
    TLabel *labCH_3_6;
    TLabel *labCH_3_7;
    TMyLedLane *led_3_0;
    TMyLedLane *led_3_1;
    TMyLedLane *led_3_2;
    TMyLedLane *led_3_3;
    TMyLedLane *led_3_4;
    TMyLedLane *led_3_5;
    TMyLedLane *led_3_6;
    TMyLedLane *led_3_7;
    TLabel *Label20;
    TLabel *labBoardOhrm4;
    TLabel *labBoardVersion4;
    TLabel *Label38;
    TLabel *Label43;
    TEdit *edContinuous_Time;
    TEdit *edOccurrences;
    TLabel *Label49;
    TLabel *labCount_0_0;
    TLabel *labCount_0_1;
    TLabel *labCount_0_2;
    TLabel *labCount_0_3;
    TLabel *labCount_0_4;
    TLabel *labCount_0_5;
    TLabel *labCount_0_6;
    TLabel *labCount_0_7;
    TLabel *labCount_1_7;
    TLabel *labCount_1_6;
    TLabel *labCount_1_5;
    TLabel *labCount_1_4;
    TLabel *labCount_1_3;
    TLabel *labCount_1_2;
    TLabel *labCount_1_1;
    TLabel *labCount_1_0;
    TLabel *labCount_2_7;
    TLabel *labCount_2_6;
    TLabel *labCount_2_5;
    TLabel *labCount_2_4;
    TLabel *labCount_2_3;
    TLabel *labCount_2_2;
    TLabel *labCount_2_1;
    TLabel *labCount_2_0;
    TLabel *labCount_3_7;
    TLabel *labCount_3_6;
    TLabel *labCount_3_5;
    TLabel *labCount_3_4;
    TLabel *labCount_3_3;
    TLabel *labCount_3_2;
    TLabel *labCount_3_1;
    TLabel *labCount_3_0;
    TSpeedButton *btnMaintenanceMode;
    void __fastcall spbStartComClick(TObject *Sender);
    void __fastcall spbStopComClick(TObject *Sender);
    void __fastcall comGMReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edOccurrencesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edContinuous_TimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    AnsiString calc_crc(unsigned char *buf, int length);
    void __fastcall DoGroundMasterMonitor();
    void ShowGroundManLog(unsigned char ucLog[15],AnsiString sMsg, bool bSend=true);    //Sam 20230906 : 新增 GroundMan Msg Log
    bool bReaderOK[4][6];   //KaiChen 20200916 ：增加 GroundMan 功能 3->6
    AnsiString sCMD[6]; //Sam 20230906 : 新增 GroundMan Msg Log
    AnsiString asRecordValueData[4][8];
    double dRecordValueData_AddOffset[4][8];
    bool bRecordAlarmData_AddOffset[4][8];
    double dOffset[4][8];
    bool bRecordAlarmData[4][8];
    bool bRs232Ok;
    bool bOpenClose[4][8];
    AnsiString asShowName[4][8];
    TQPF_Timer tTimerOutTimer;
    TQPF_Timer tDalayTimer;
    bool bGroundManReset;
    bool bGroundManResetByStart;
    bool bUseOffset;
    void ReadGroundOffset();
    int  iUseGndBoard;
    //JimmyChiu 20210922 Avoid overflow
    AnsiString LogMsg;
    bool bGroundManAlarm_FirstTime[4][8];
    int  iGroundManAlarm_StartTime[4][8];
    int  iGroundManAlarm_HappenCount[4][8];
    bool bInternalOhm;                      //KenHsieh 20220803 : GroundMan流程及功能修改
    bool bVersion;                          //KenHsieh 20220803 : GroundMan流程及功能修改
    int iMachineOhmRetry;                   //Sam 20230908 : 讀取阻抗獨立 Retry 機制
public:     // User declarations
    __fastcall TfGroundMan(TComponent* Owner);
    void __fastcall Init_GM_RS232();
    void __fastcall SetGroundMaster(int iAddress,int iMode);
    int iGroundMasterTask;
    bool fShow;
    void ReStart();       //Sam 20220107 : 矽格北興 Initail Start 要重啟 GroundMan
    AnsiString asGroundVaule;       //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
};
//---------------------------------------------------------------------------
extern PACKAGE TfGroundMan *fGroundMan;
//---------------------------------------------------------------------------
#endif
