//---------------------------------------------------------------------------

#ifndef PowerSavingModeH
#define PowerSavingModeH

#include "myswitch.h"
//---------------------------------------------------------------------------
class TModule
{
  public:
  __fastcall TModule();
  __fastcall ~TModule();

  bool Enabled;                                          //Module啟用
  TMySwitch *mSW;
  TDateTime AlarmTmr;                                    //設定警報時間
  TDateTime iCountDown;                                 //倒數的時間

  bool TimeOut(TDateTime StartTmr);                      //倒數
  virtual bool Doing(bool Reset=false);                 //時間到,Module進入處理工作
};
class TMtrModule : public TModule
{
   public:
   bool Doing(bool Reset=false);
};
class TVacuumModule : public TModule                    //Steven 20221215 : Power saving for vacuum pump
{
   public:
   bool Doing(bool Reset=false);
};
class THotModule : public TModule
{
   public:
   bool bCheckTempClose;
   bool Doing(bool Reset=false);
   TQPF_Timer tHotModuleTimer;
};
class TPowerSaving
{
   private:
   bool flagStartTmr;                                    //機台閒置進入休眠倒數旗標
   bool flagUseFunc;                                     //休眠功能啟用旗標
   TDateTime StartTmr;
   bool bHighlightOn;                                    //AI(ht9045-v899) 20260811: 狀態列反黃中(避免每秒重複重繪) (CASE-PTI-20260811-001)
   void ShowPowerSaveHighlight(bool bOn);                //AI(ht9045-v899) 20260811: 進入省電後狀態列底色反黃並放大字型 (CASE-PTI-20260811-001)

   public:
   __fastcall TPowerSaving();
   __fastcall ~TPowerSaving();

   TTimer *ScanTmr;
   TMtrModule *MtrModule;                                //控制馬達休眠Moduel
   THotModule *HotModule;                                //控制加熱裝置休眠Moduel
   TVacuumModule *VacuumModule;                          //Steven 20221215 : Power saving for vacuum pump
   TModule *tModule;                                     //處理進入休眠ShowMessage

   void __fastcall OnScanTmr(TObject *Sender);           //Scan Timer
   void SetFunction(bool OnOff);                         //設定啟用的Module
   void SetAlarmTime(TDateTime &Module ,int Minute);     //設定警報時間
   bool CheckSystemRun();                                //檢查是否進入休眠
   void Restart();                                       //通知謷報重新計數
   bool CheckChangeState();                              //檢查是否在進入倒數時,有停止進入省電模式的操作

   bool bRestart;                                        //重新計數
};
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260804: shared by cConfiguration for the [C05] halt-time edit limit (CASE-PTI-20260804-001)
extern int GetPowerSaveMaxMinute();
extern class TPowerSaving *tPowerSaving;
#endif

