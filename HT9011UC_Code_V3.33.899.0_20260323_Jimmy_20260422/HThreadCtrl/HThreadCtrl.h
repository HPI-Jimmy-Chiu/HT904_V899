#ifndef HThread_H
#define HThread_H

typedef void (*PF_PROCESS)(LPVOID);

/*
Class Name      : HThreadCtrl
Version         : 1.00
Release Date    : 2008.10.14
*/
class HThreadCtrl
{
private:
    HANDLE      _handle;
    DWORD       _id;
    LPVOID      _parameter;
    PF_PROCESS  _process_func;  // Thread execute function pointer

    bool    _execute;

    int     _sleep_delay_count;

protected:
    static DWORD CALLBACK ThreadProcess( LPVOID lpParameter);

public:
    HThreadCtrl();
    ~HThreadCtrl();

    void Initial( PF_PROCESS Func , LPVOID Param );
    int OpenThread();
    int CloseThread( int iTimeOut = INFINITE );

    void SetSleepDelayCount( int iDelay );

};

#endif
