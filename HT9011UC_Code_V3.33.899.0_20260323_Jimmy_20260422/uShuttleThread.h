//---------------------------------------------------------------------------

#ifndef uShuttleThreadH
#define uShuttleThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TShuttleThread : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    __fastcall TShuttleThread(bool CreateSuspended);
    void __fastcall TShuttleThread::ShuttleThreadProcess(void);
};
extern TShuttleThread *SHThread;
//---------------------------------------------------------------------------
#endif
