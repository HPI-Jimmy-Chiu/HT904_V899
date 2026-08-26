//==============================================================================
//
//   TMC401
//
//==============================================================================
#ifndef MyTemptureTMC401H
#define MyTemptureTMC401H

#include "MyTempture.h"

class TMyTMC401:public TMyTempCtrl
{
    private:
        unsigned char Check_LRC(void);
        unsigned char Create_LCR(unsigned char *Sptr, unsigned char length);
    protected:
        virtual void ReadData(int Addr, int Command);               // 對address=Index溫控器送出欲讀取之 Registor
        virtual void WriteData(int Addr, int Command, short Value); // 對address=Index溫控器送出欲設定之 Registor
        virtual bool DoReadCurrentTemp();
        virtual bool DoStartSetTemp();
        virtual bool ReadTargetTemp(int Addr);
        virtual bool DoAutoTuning();
        //===================================================
        virtual bool DoReadPIDData();        //2011.03.10 Q_Q V208D Temp PID Add
        virtual bool DoWritePIDData();       //2011.03.10 Q_Q V208D Temp PID Add
        //===================================================
        float ProcessData();
};
#endif
