//---------------------------------------------------------------------------

#ifndef ScanBtnThreadH
#define ScanBtnThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class ScanBtn : public TThread
{
private:
    HANDLE eStart;
    HANDLE eFreeArm;

    bool bStart;
    bool bStopArm;
    bool bEndThread;
protected:
   void __fastcall Execute();
public:
   __fastcall ScanBtn(bool CreateSuspended);
   __fastcall ~ScanBtn();
   void __fastcall Stop();
   void __fastcall Start();
   void __fastcall EndThread();
   bool __fastcall GetRunState();
   void __fastcall SetArmState(bool bStop);
   bool __fastcall GetArmState();
   bool bThreadFlag;        //Steven 20120202 : 加入Thread保護
   bool bRealTimeCCDStop;   //Steven 20120505 : 取代Aled
};
//---------------------------------------------------------------------------
#endif
