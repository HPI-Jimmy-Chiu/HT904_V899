//AI(W0-TAIL) 20260626: de-VCL -- dropped MachineDefine.h (VCL+drivers).
#include "vclcompat/vcl_compat.h"   //AI(W0-TAIL) 20260626: neutralize __fastcall
#include <windows.h>                 //AI(W0-TAIL) 20260626: QueryPerformanceCounter/Frequency
#include "MachineType.h"             //AI(W0-TAIL) 20260626: ChangeToFloatNonPcnt template
//was-MachineDefine.h                                                      //Dell 將.h統一,可加速build
//AI(W0-TAIL) 20260626: dropped #pragma hdrstop

#include "myTimer.h"

//---------------------------------------------------------------------------

//AI(W0-TAIL) 20260626: dropped #pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TQPF_Timer::TQPF_Timer()
{
    PerformanceCounterOverhead = CalibratePerformanceCounterOverhead();
}
//---------------------------------------------------------------------------
TLargeInteger TQPF_Timer::CalibratePerformanceCounterOverhead()
{
    QueryPerformanceCounter(&rStart);
    for (int i=1; i<=1000; i++)
        QueryPerformanceCounter(&rEnd);

    // 取得每次呼叫 QueryPerformanceCounter 函式所花費時間之平均值
    TLargeInteger r;
    r.QuadPart=rEnd.QuadPart-rStart.QuadPart;
    r.QuadPart/=1000;
    return r;
}
//---------------------------------------------------------------------------
void TQPF_Timer::On()
{
    // 進入迴圈前取得開始計數值
    QueryPerformanceCounter(&rStart);
    QueryPerformanceCounter(&rStartDelay);                                      // 取得目前計數值
    rEnd.QuadPart=rSetTime.QuadPart+rStart.QuadPart-PerformanceCounterOverhead.QuadPart-PerformanceCounterOverhead.QuadPart;
}
//---------------------------------------------------------------------------
bool TQPF_Timer::Off()
{
    QueryPerformanceCounter(&rFreq);                                            // 取得目前計數值
    return (rFreq.QuadPart>=rEnd.QuadPart);
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetSec(DWORD Sec)
{
    QueryPerformanceFrequency(&rFreq);
    rSetTime.QuadPart=Sec*rFreq.QuadPart;
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetMS(DWORD MSec)
{
    QueryPerformanceFrequency(&rFreq);
    rSetTime.QuadPart=MSec*rFreq.QuadPart/1000;
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetUS(DWORD USec)
{
    QueryPerformanceFrequency(&rFreq);
    rSetTime.QuadPart=USec*rFreq.QuadPart/1000000;
}
//---------------------------------------------------------------------------
//void TQPF_Timer::ResetSecAndOn(double Sec)
//{
//    SetMS(Sec*1000);
//    On();
//}
//---------------------------------------------------------------------------
//void TQPF_Timer::ResetMSAndOn(DWORD MSec)
//{
//    SetMS(MSec);
//    On();
//}
//---------------------------------------------------------------------------
//void TQPF_Timer::ResetUSAndOn(DWORD USec)
//{
//    SetUS(USec);
//    On();
//}
//---------------------------------------------------------------------------
void TQPF_Timer::Set0_1SecAndOn(double Sec)
{
    SetMS(Sec*100);
    On();
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetSecAndOn(double Sec)
{
    SetMS(Sec*1000);
    On();
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetMSAndOn(DWORD MSec)
{
    SetMS(MSec);
    On();
}
//---------------------------------------------------------------------------
void TQPF_Timer::SetUSAndOn(DWORD USec)
{
    SetUS(USec);
    On();
}
//---------------------------------------------------------------------------
int TQPF_Timer::LatchCycleTimeSec(bool Start)
{
    int itmp=0;
    if(Start)
    {
        QueryPerformanceCounter(&rStartDelay);                                  // 取得目前計數值
    }
    else
    {
        QueryPerformanceCounter(&rEndDelay);                                    // 取得目前計數值
        QueryPerformanceFrequency(&rFreq);
        itmp=ChangeToFloatNonPcnt((double)((rEndDelay.QuadPart-rStartDelay.QuadPart)), (double)(rFreq.QuadPart));       //Steven 20191016 : 修正
    }

    return itmp;
}
//---------------------------------------------------------------------------
int TQPF_Timer::LatchCycleTime(bool Start)
{
    int itmp=0;
    if(Start)
    {
        QueryPerformanceCounter(&rStartDelay);                                  // 取得目前計數值
    }
    else
    {
        QueryPerformanceCounter(&rEndDelay);                                    // 取得目前計數值
        QueryPerformanceFrequency(&rFreq);
        itmp=ChangeToFloatNonPcnt((double)((rEndDelay.QuadPart-rStartDelay.QuadPart)*1000), (double)(rFreq.QuadPart));
    }

    return itmp;
}
//---------------------------------------------------------------------------
int TQPF_Timer::LatchCycleTimeUS(bool Start)                                    //Steven 20180808 (wei) : TTL的時間單位改成microsecond
{
    int itmp=0;
    if(Start)
    {
        QueryPerformanceCounter(&rStartDelay);                                  // 取得目前計數值
    }
    else
    {
        QueryPerformanceCounter(&rEndDelay);                                    // 取得目前計數值
        QueryPerformanceFrequency(&rFreq);
        itmp=ChangeToFloatNonPcnt((double)((rEndDelay.QuadPart-rStartDelay.QuadPart)*1000000), (double)(rFreq.QuadPart));
    }

    return itmp;
}
//---------------------------------------------------------------------------