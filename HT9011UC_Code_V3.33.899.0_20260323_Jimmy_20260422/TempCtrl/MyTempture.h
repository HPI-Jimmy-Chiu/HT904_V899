//---------------------------------------------------------------------------
#ifndef MyTemptureH
#define MyTemptureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Series.hpp>
#include "SPComm.hpp"
#include "HTimer.h"

#include "MyTempture_MainPanel.h"
#include "MyTempture_Monitor.h"
#include "MyTempture_Offset.h"
#include "MachineDefine.h"
//---------------------------------------------------------------------------
class TDataModule2 : public TDataModule
{
__published:    // IDE-managed Components
    TComm *Tempture;
private:    // User declarations
public:     // User declarations
    __fastcall TDataModule2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule2 *DataModule2;
#define _MAX_RETRY  3
//------------------------------------------------------------------------------
#define MAX_CONTROL_UNIT 128
#define MAX_HISTROY__RECORD_COUNT 30

class TMyTempCtrl
{
    private :

protected:  // User declarations
    int   Addr;
    TTimer *Timer1;
    TComm *Comm2;
    HTimer Com2Delay;

    int   CommRetry[MAX_CONTROL_UNIT];

    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Comm2ReceiveData(TObject *Sender, Pointer Buffer, WORD BufferLength);
    bool  bHasUnitArray[MAX_CONTROL_UNIT];                                      // 放北竟琌Τ杆
    bool  bHasUnit;                                                             // 狦常⊿Τ杆放北竟碞穦=false
    double fTemp[MAX_CONTROL_UNIT];                                             // 纗ヘ玡放北竟放
    double fOffset[MAX_CONTROL_UNIT];                                           // 2010.07.19 , Joye

    bool  bSetTemp[MAX_CONTROL_UNIT];                                           // êㄇ放北竟璶秨﹍砞﹚放
    double fSetTemp[MAX_CONTROL_UNIT];                                          // êㄇ放北竟璶秨﹍砞﹚放
    double fBias[MAX_CONTROL_UNIT];                                             // êㄇ放北竟璶秨﹍砞﹚放熬畉
    double fReadSetTemp[MAX_CONTROL_UNIT];                                      // 纗ヘ玡放北竟┮弄ぇ砞﹚放
    bool  bReadSetTemp[MAX_CONTROL_UNIT];                                       // êㄇ放北竟璶弄ぇ砞﹚放
    bool  bUseBias[MAX_CONTROL_UNIT];                                           // êㄇ放北竟ヘ玡Τㄏノ放熬畉
    bool  bStartSetTemp;                                                        // 秨﹍秈︽砞﹚放

    int   iMaxPeriodCount[MAX_CONTROL_UNIT];                                    // 放北竟砞﹚放跋丁计秖
    double fTargetPointPeriod[MAX_CONTROL_UNIT][20];                            // 癸放北竟癳饼荐
    double fMeasureValuePeriod[MAX_CONTROL_UNIT][20];                           // 代放

    bool  bAutoTuning;                                                          // 秨﹍秈︽笆放Tuning
    bool  bStopAutoTuning;
    bool  bAutoTuningDevice[MAX_CONTROL_UNIT];                                  // êㄇ放北竟璶放Tuning
    int   iStartAutoTuningTask;                                                 // Auto Turning ㄧΑ Task

    bool   bStartReadPIDData;                                                   //2011.03.10 Q_Q V208D Temp PID Add
    bool   bStartWritePIDData;                                                  //2011.03.10 Q_Q V208D Temp PID Add
    double fReadSetPID[MAX_CONTROL_UNIT][3];                                    //2011.03.10 Q_Q V208D Temp PID Add
    bool   bStartSetPID;                                                        //2011.03.10 Q_Q V208D Temp PID Add
    bool   bSetPID[MAX_CONTROL_UNIT];                                           //2011.03.10 Q_Q V208D Temp PID Add  //êêㄇ惠璶砞﹚PID
    int    iSetPID[MAX_CONTROL_UNIT][3];                                        //2011.03.10 Q_Q V208D Temp PID Add  //柑惠璶砞﹚PID

    char SendBuffer[1024];                             // 饼癸放北竟硄癟ぇ戈
    bool Com2ReceiveOK;                                // Τ眖放北竟硄癟ぇ Comm PortΜ掸戈
    char Com2Buffer[1024];                             // Τ眖放北竟硄癟ぇ Comm PortΜ掸戈ぇず甧
    int  ComPort;                                      // 放北竟硄癟ぇ Comm Port
    TParity ComParity;                                 // 放北竟硄癟ぇ Parity
    unsigned char Com2ReceiveByte[1024];
    int Receivelen;

    int iReadCurrentTempTask;
    int iStartReadSetTempTask;
    int iStartSetTempTask;
    int iComProtTask;                                   // 2010.08.31 , Joye
    int iStartReadPIDDataTask;                          //2011.03.10 Q_Q V208D Temp PID Add
    int iStartWritePIDDataTask;                         //2011.03.10 Q_Q V208D Temp PID Add

    bool bFirstRecord[MAX_CONTROL_UNIT];
    double fHistroyTemp[MAX_CONTROL_UNIT][MAX_HISTROY__RECORD_COUNT];           // 纗ヘ玡放北竟Histroy放
    void RecordTempture(int Index, double Temp);
    void ShowTempValue(int index, double value);
    void DrewSeries(int index, double value);
    int iTotalInstalledUnit;

    Word SystemHour,SystemMin,SystemSec,SystemMSec;
    Word SystemYear,SystemMonth,SystemDate;

    // ぃ放北竟祘Α璶э场
    virtual void ReadData(int Addr,int Command) = 0;                            // 癸address=Index放北竟癳饼弄ぇ Registor
    virtual void WriteData(int Addr,int Command, short Value) = 0;              // 癸address=Index放北竟癳饼砞﹚ぇ Registor
    virtual bool DoReadCurrentTemp() = 0;                                       // 苯磞┮Τ放北竟ヘ玡放 纗 fTemp[] い
    virtual bool DoStartSetTemp() = 0;                                          // 癸放北竟竤舱癳饼砞﹚ぇ放
    virtual bool ReadTargetTemp(int Index) = 0;                                 // 秨﹍弄address=Index放北竟箇﹚荐ぇ放
    virtual bool DoAutoTuning() = 0;                                            // 癸放北竟竤舱癳 Auto Turning 笆璶―

    virtual bool DoReadPIDData() = 0;                                           //2011.03.10 Q_Q V208D Temp PID Add  // 癸放北竟竤舱癳 弄PID
    virtual bool DoWritePIDData() = 0;                                          //2011.03.10 Q_Q V208D Temp PID Add  // 癸放北竟竤舱癳 糶PID

    //Steven 20100517 Start: ぃ璶#include "cpublic.h"
    void GetTimeInfo();
    bool GetCOMPortStatus(int i);
    int HexStrToInt(char *str);
    void CutSpaceAtTail(char *S);
    void CutSpaceAtHead(char *S);
    //Steven 20100517 End
public:
    TMyTempCtrl();
    ~TMyTempCtrl();
    void  SetComPort(int port);                          // 砞﹚放北竟竤舱琌ノêCOM PORT
    void  SetComParity(TParity Parity);                  // 砞﹚放北竟竤舱琌ノêParity
    void  InstalledUnit(int Index);                      // 砞﹚address=Index 放北竟琌Τ
    void  OpenUnit(int Index);                           // 砞﹚address=Index 放北竟砆秨币
    void  CloseUnit(int Index);                          // 砞﹚address=Index 放北竟砆闽超
    bool  UnitHasInstall(int Index);
    void  WriteTargetTemp(int Index, double f);          // 盢address=Index放北竟砞﹚箇荐ぇ放f
    double ReadCurrentTemp(int Index);                   // 弄address=Index 放北竟ヘ玡放
    double ReadPIDData(int Index,int PID);               //2011.03.10 Q_Q V208D Temp PID Add
    void   StartReadPIDData();                           //2011.03.10 Q_Q V208D Temp PID Add
    void   StartWritePID();                              //2011.03.10 Q_Q V208D Temp PID Add

    void  ClearPointPeriod(int Index);
    bool  AddPointPeriod(int Index, double fTarget, double fMeasue);
    bool  WriteTargetConvertTemp(int Index, double f, double fNowOffset = 0);  // 盢address=Index放北竟砞﹚砆锣传ぇ箇荐ぇ放f
    bool  WriteTargetTempUseBias(int Index, double f);                          // 盢address=Index放北竟砞﹚砆锣传ぇ箇荐ぇ放f,熬簿
    void  AutoTuning(int Index);                                                // 盢address=Index放北竟砞﹚放f,Auto Tuning
    void  StopAutoTuning();
    void GetTemptureHistroy(int Index, int *RecordCount, double *HistroyTemp);
    int  GetTotalInstalledUnit();
    AnsiString Alias[MAX_CONTROL_UNIT];
    bool bMonitorSelect[MAX_CONTROL_UNIT];
    bool bAmbientMode;
    AnsiString asHistroyTempTime[MAX_CONTROL_UNIT][MAX_HISTROY__RECORD_COUNT];  // 纗ヘ玡放北竟Histroy放魁丁

    void ReStartComPort();                                                      // 2010.08.31 , Joye
    void WriteSetPID(int Index,int P,int I,int D) ;                             //2011.03.10 Q_Q V208D Temp PID Add
    AnsiString GerCom2Buffer() {return Com2Buffer;};
    TMyTempMainPanel *TempMainPal[MAX_CONTROL_UNIT];                            //礶放陪ボ
    TMyTempMonitor   *TempMonitor[MAX_CONTROL_UNIT];                            //菏北礶竤舱
    TMyTempOffset    *TempOffset[MAX_CONTROL_UNIT];                             //放北竟Offset
    bool bStartMonitor;                                                         //秨币菏北礶
    bool bShowChart;
    TLineSeries *Series[MAX_CONTROL_UNIT];
    bool InitialOK;                                                             //耞北狠琌竒非称ЧΘ
};
#endif
