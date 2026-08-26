//---------------------------------------------------------------------------
//Tray軌道改通訊式步進馬達
//1. 將該CPP加入專案之中
//2. 在void Initial()中加入RS232Init(AnsiString ComPort);
//3. 根據軌道數量設定速度iStepMotorSpeed[10]
//4. 執行StartSetSpeed()寫入速度
//5. 安排一個Memo指向mmoTrayStepMotor,以便顯示通訊Log
//---------------------------------------------------------------------------
#ifndef TrayStepMotorH
#define TrayStepMotorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TdmTrayMotor : public TDataModule
{
__published:    // IDE-managed Components
    TTimer *tmrTrayStepMotor;
    TComm *comTrayStepMotor;
    void __fastcall tmrTrayStepMotorTimer(TObject *Sender);
    void __fastcall comTrayStepMotorReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
    void __fastcall DataModuleDestroy(TObject *Sender);
private:    // User declarations
    void AddTrayStepMotorLog(AnsiString sFunction, AnsiString str);
    //bool bNeedSetStepMotorSpeed;
    bool bReceiveData;
    bool bReceiveVibrateMotData;    //JerryYang 20230814 : add震動馬達通訊調速版本
    TQPF_Timer SetStepMotorDelay;
    TQPF_Timer SetVibrateMotorDelay;        //JerryYang 20230814 : add震動馬達通訊調速版本
    void __fastcall CMD_CheckStepMotorVer(int Axis);  //ASK Ver
    void __fastcall CMD_SetStepMotorSpeed(int Axis, int SpeedPerc);
    void __fastcall CMD_SetStepMotorSecondSpeedCW(int Axis, int SpeedPerc);
    void __fastcall CMD_SetStepMotorSecondSpeedCCW(int Axis, int SpeedPerc);
    void __fastcall CMD_SetStepMotorSecondSpeedAcc(int Axis);
    void __fastcall CMD_SetStepMotorSecondSpeedDec(int Axis);

    void __fastcall CMD_CheckVibrationMotVer(int Axis);  //JerryYang 20230814 : add震動馬達通訊調速版本
    void __fastcall CMD_SetVibrationMotorSpeed(int Axis);

public:        // User declarations
    __fastcall TdmTrayMotor(TComponent* Owner);
    int  iSetStepMotorTask;
    int iVibrationMotorTask;    //JerryYang 20230814 : add震動馬達通訊調速版本

    void RS232Init(AnsiString ComPort);
        //需要在void Initial()中,加入以下初始化程式, 以便開Com
        //    if(LoaderUnload_StepMotor)
        //        dmTrayMotor->RS232Init(HSys.TrayStepMotor_ComPort);

    int iStepMotorSpeed[10];
        //馬達速度的百分比,要加入TfSpeed::ReadWriteFile之中

    void StartSetSpeed();
        //在需要重新設定速度的地方加入本Function
        //例如: SetMotorSpeed() 與 TfSpeed::ReadWriteFile()

    TMemo *mmoTrayStepMotor;
        //如果要在主畫面顯示通訊Log, 需要在主畫面FormShow時, 加入以下Code
        //void __fastcall TfMain::FormShow(TObject *Sender)
        //{
        //    if(dmTrayMotor!=NULL)
        //    {
        //        dmTrayMotor->mmoTrayStepMotor=mmoTrayStepMotor;
        //    }
        //}
        //----------------------------------------------------------------------
        //另外, 在主畫面解構時, 需要釋放記憶體, 避免操作發生異常
        //void __fastcall TfMain::FormDestroy(TObject *Sender)
        //{
        //    if(dmTrayMotor!=NULL)
        //    {
        //        dmTrayMotor->mmoTrayStepMotor=NULL;
        //    }
        //}
         bool bNeedSetStepMotorSpeed;   //kevin 20210604 move
         bool bNeedSetVibrateMotSpeed;   //JerryYang 20230814 : add震動馬達通訊調速版本
    bool bRunTimer;             //KenHsieh 20220107 : 無通訊面板時使用
    void DoTrayStepMotor();     //KenHsieh 20211222 : Pad與步進馬達為同一Comport
};
//---------------------------------------------------------------------------
extern PACKAGE TdmTrayMotor *dmTrayMotor;
//---------------------------------------------------------------------------
#endif
