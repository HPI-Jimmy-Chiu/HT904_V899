//---------------------------------------------------------------------------

#ifndef uHeaterThreadH
#define uHeaterThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class THeaterThread : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    __fastcall THeaterThread(bool CreateSuspended);
    void __fastcall HeaterThreadProcess(void);
    bool bEnd;                                                                  //Steven 20110718 : 為了要可以關閉執行緒
};
extern THeaterThread *HeaterThread;
//---------------------------------------------------------------------------
void StartHeaterThread();
void EndHeaterThread();
void CheckHeater();
extern bool CheckHeaterOK();                                                    //Steven 20250116 : 確認HeaterOK
extern bool bHeatOverTenErrorOK;                                                //kevin 20130226 加熱ok後 溫度過高
extern bool bHeatOKOverError;                                                   //kevin 20130226 加熱ok後 溫度過高
#endif
