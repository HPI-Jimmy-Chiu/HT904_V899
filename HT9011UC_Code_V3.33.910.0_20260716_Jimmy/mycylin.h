//---------------------------------------------------------------------------
#ifndef mycylinH
#define mycylinH
//---------------------------------------------------------------------------

//#include "htimer.h"
#include "myTimer.h"
#define MaxCylinderItem 295

class TMyCylinder{
    private:
        int OnTask;
        int OffTask;
        int OnOff;
        int OnTryTask;
        int OffTryTask;
        int iOnLeft;
        int iOnTop;
        int iOffLeft;
        int iOffTop;
        void OnSwitch();                                                        //Steven 20230721 : 統一開關控制
        void OffSwitch();                                                       //Steven 20230721 : 統一開關控制

    protected:
        TControl *PTempWinCtrl;
    public:
        _fastcall TMyCylinder::TMyCylinder();
        AnsiString CylinderName;
        AnsiString OnSensorName;
        AnsiString OffSensorName;
        TQPF_Timer  TOn;
        TQPF_Timer  TOff;
        TQPF_Timer  TOnDelay;
        TQPF_Timer  TOffDelay;
        bool    NeedFinishOnFunction;
        bool    NeedFinishOffFunction;
        bool    Enable;
        bool    Status;
        bool    Change;
        bool    AlarmEnable;
        int     OnAlarmCode;
        int     OffAlarmCode;
        int     OnAlarmTime;
        int     OffAlarmTime;

        int     OnDelayTime;
        int     OffDelayTime;

        AnsiString OutRingUse;
        int     OutRing;
        int     OutIP;
        int     OutPort;
        int     OutBit;
        int     OutType;                                                        // A or B Type
        int     OutISABase;

        AnsiString OnSenRingUse;
        int     OnSenRing;
        int     OnSenIP;
        int     OnSenPort;
        int     OnSenBit;
        int     OnSenType;
        int     OnSenISABase;
        bool    OnSenEnable;

        AnsiString OffSenRingUse;
        int     OffSenRing;
        int     OffSenIP;
        int     OffSenPort;
        int     OffSenBit;
        int     OffSenType;
        int     OffSenISABase;
        bool    OffSenEnable;

        bool    Push();
        bool    Pop();
        void    On();
        void    Off();

        bool    bCylinderOn;                                                    // ben edit //
        bool    Reset();
        bool    OnStatus();
        bool    OffStatus();
        bool    OnSensor();
        bool    OffSensor();
        bool    GetOutBit();

        bool    FinshFullMotion();
        bool    FinshOnMotion();
        void    SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int simuStart, int simuEnd);
        void    UpdateSimulateCompomentPosition();
        bool FinshOffMotion();
        void ResetOnAlarmTime();
        void ResetOffAlarmTime();
        void ResetOnDelayTime();
        void ResetOffDelayTime();
//        int  iX;
//        int  iY;
        int ISABase;                                                            //Nickliu 20230309 add Cylinder ISABase Type                                                         //Nickliu 20230306 add IO Use ISABase
        bool bCheckSafeDoor;                                                    //Steven 20230703 : Add Cylinder action check SafeDoor
        //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
        //==>
        TQPF_Timer tOnOff;
        bool bfirst;
        void AddOnCount(bool bResetTimer=false);
        void AddOffCount(bool bResetTimer=false);
        void LatchOnffTime();
        bool bOnOffState;
        int iOnOffCount;        //紀錄累積 On + Off 次數
        double dOnTime;         //紀錄目前 On Time 時間，須配合使用 OnSenEnable
        double dOffTime;        //紀錄目前 Off Time 時間，須配合使用 OffSenEnable
        int iTimeOutCount;      //紀錄累積 Push or Pop Time Out 次數
        bool bCylPreAlarmByPassT;

        TStringList *sListOnTime;
        TStringList *sListOffTime;
        void AddOnTime(double dOnTime);
        void AddOffTime(double dOffTime);
        AnsiString GetOnTime();
        AnsiString GetOffTime();
        AnsiString GetOnTimeAvg();
        AnsiString GetOffTimeAvg();
        AnsiString GetOnTimeAlarm();
        AnsiString GetOffTimeAlarm();

        int iOnOffCountAlarm;
        int iOnCountAlarm;
        int iOffCountAlarm;
        double dOnTimeAlarm;
        double dOffTimeAlarm;
        int iTimeOutCountAlarm;
        int iResetCount;
        AnsiString sResetTime;
        //<==
        //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
};

extern class TMyCylinder Cylinder[MaxCylinderItem];
void InitialCylinderName();
#endif
