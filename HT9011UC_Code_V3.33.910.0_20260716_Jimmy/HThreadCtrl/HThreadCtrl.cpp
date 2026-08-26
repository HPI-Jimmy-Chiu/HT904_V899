#include "MachineDefine.h"
#pragma hdrstop

#include "HThreadCtrl.h"
#include "common.h"
#include "cmydef.h"
//------------------------------------------------------------------------------
HThreadCtrl::HThreadCtrl()
{
    _handle=NULL;
    _id=0;
    _parameter=NULL;
    _process_func=NULL;
    _sleep_delay_count=0;
    _execute=false;
}
//------------------------------------------------------------------------------
HThreadCtrl::~HThreadCtrl()
{
    try
    {
        CloseThread();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HThreadCtrl");
    }
}
//------------------------------------------------------------------------------
void HThreadCtrl::Initial(PF_PROCESS Func, LPVOID Param)
{
    if(_handle!=NULL)
    {
        CloseThread();
    }

    _process_func   = Func;
    _parameter      = Param;
}
//------------------------------------------------------------------------------
int HThreadCtrl::OpenThread()
{
    if(_handle!=NULL || _execute==true)
    {
        CloseThread();
    }
    _execute=true;
    _handle=CreateThread(0, 0, ThreadProcess, this, 0, (DWORD*)&_id);
    return 0;
}
//------------------------------------------------------------------------------
int HThreadCtrl::CloseThread(int iTimeOut)
{
    if(_handle==NULL && _execute==false)
    {
        return 0;
    }

    LogSoftwareOffTime("FC: Wait HThreadCtrl");
    _execute=false;

    DWORD dWait=WaitForSingleObject(_handle, iTimeOut);

    if(dWait==WAIT_FAILED || dWait==WAIT_TIMEOUT)
    {
        TerminateThread(_handle, 0);
    }

    CloseHandle(_handle);
    _handle=NULL;
    _id=0;

    return dWait;
}
//------------------------------------------------------------------------------
void HThreadCtrl::SetSleepDelayCount(int iDelay)
{
    if(iDelay>=20)
        iDelay=20;

    _sleep_delay_count=iDelay;
}
//------------------------------------------------------------------------------
DWORD CALLBACK HThreadCtrl::ThreadProcess(LPVOID lpParameter)
{
    HThreadCtrl* thread=(HThreadCtrl*) lpParameter;
    static int iSleep=0;
    while(thread->_execute)
    {
        thread->_process_func(thread->_parameter);

        iSleep++;
        if(iSleep>thread->_sleep_delay_count)
        {
            iSleep=0;
            MySleep(1); // Release CPU Resource
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
