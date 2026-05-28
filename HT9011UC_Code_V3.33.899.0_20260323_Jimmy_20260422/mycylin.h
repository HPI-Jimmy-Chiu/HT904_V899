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
        void OnSwitch();                                                        //Steven 20230721 : 参秨闽北
        void OffSwitch();                                                       //Steven 20230721 : 参秨闽北

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
};

extern class TMyCylinder Cylinder[MaxCylinderItem];
void InitialCylinderName();
#endif
