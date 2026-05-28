//==============================================================================
//
//   士林電機
//   Shihlin  WT404 系列
//
//==============================================================================
#ifndef MyTemptureWT404H
#define MyTemptureWT404H

#include "MyTempture.h"

class TMyWT404:public TMyTempCtrl
{
    private:
        float ProcessData();
        AnsiString WT404LRC(AnsiString str);
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
    public:
        TMyWT404();
};
#endif