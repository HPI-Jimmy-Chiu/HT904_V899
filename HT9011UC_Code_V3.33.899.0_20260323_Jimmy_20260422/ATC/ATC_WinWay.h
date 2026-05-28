//---------------------------------------------------------------------------

#ifndef ATC_WinWayH
#define ATC_WinWayH
#include "system.hpp"
#include "SPComm.hpp"
//---------------------------------------------------------------------------
class ATC_WinWay
{
    private:
        double  fPresentTemperature;
        double  fSetTemperature;
    protected:
    public:
        bool                    bCommConnect;
        AnsiString              ansiST;
        AnsiString              sWinwayCommName;
        TComm*                  WinwayCOM;
        //function
        ATC_WinWay(TComm* Comm);
        double        GetPT();
        double        GetPT_NoCommand();
        void          SetPT(double _temp);
        void          SetPT(char* _temp);
        double        GetST();
        void          SetST(double _temp);
        bool          OpenCommPort();
        bool          CloseCommPort();
        unsigned int GETCRC16(unsigned char *string, unsigned char length);
        void MySleep(DWORD dwMilliseconds);
        int iWinWaySendCount;
};
#endif
