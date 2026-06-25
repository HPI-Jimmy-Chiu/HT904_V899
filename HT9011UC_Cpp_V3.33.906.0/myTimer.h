//---------------------------------------------------------------------------

#ifndef myTimerH
#define myTimerH
#include "vclcompat/vcl_compat.h"   //AI(W0-TAIL) 20260626: neutralize __fastcall (was <vcl.h> via MachineDefine.h)
#include <windows.h>                 //AI(W0-TAIL) 20260626: LARGE_INTEGER/DWORD/QueryPerformanceCounter (was via MachineDefine.h)
typedef LARGE_INTEGER TLargeInteger; //AI(W0-TAIL) 20260626: VCL alias for Win32 LARGE_INTEGER (was via SysUtils.hpp)
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
