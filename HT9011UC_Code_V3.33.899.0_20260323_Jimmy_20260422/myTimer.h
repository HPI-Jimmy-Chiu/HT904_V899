//---------------------------------------------------------------------------

#ifndef myTimerH
#define myTimerH
//---------------------------------------------------------------------------

class TQPF_Timer
{
private:
    int iCount;
    TLargeInteger rStart;
    TLargeInteger rSetTime;
    TLargeInteger rEnd;
    TLargeInteger rFreq;
    TLargeInteger rStartDelay;
    TLargeInteger rEndDelay;
    TLargeInteger PerformanceCounterOverhead;

    TLargeInteger CalibratePerformanceCounterOverhead();
public:
    __fastcall TQPF_Timer();
    int LatchCycleTime(bool Start=false);
    int LatchCycleTimeUS(bool Start=false);     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
    int LatchCycleTimeSec(bool Start=false);    //Steven 20190724 : 擷取秒
//    void On();
    void On();
    bool Off();
    void SetSec(DWORD Sec);
    void SetMS(DWORD MSec);
    void SetUS(DWORD USec);
//    void ResetSecAndOn(double Sec);
//    void ResetMSAndOn(DWORD MSec);
//    void ResetUSAndOn(DWORD USec);
    void Set0_1SecAndOn(double Sec);
    void SetSecAndOn(double Sec);
    void SetMSAndOn(DWORD MSec);
    void SetUSAndOn(DWORD USec);
};

#endif
