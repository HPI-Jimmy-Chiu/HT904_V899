//---------------------------------------------------------------------------
#ifndef uruncontrolH
#define uruncontrolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TRunControl : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    __fastcall TRunControl(bool CreateSuspended);
    void __fastcall TRunControl::ThreadProcess(void);
    bool bEnd;   //Steven 20110726 : 為了要可以關閉執行緒
};
extern TRunControl *MyThread;
//---------------------------------------------------------------------------
void StartMainThread();
void EndMainThread();
#endif
