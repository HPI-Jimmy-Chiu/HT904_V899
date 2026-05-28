//==============================================================================
//
//   台達電
//   Delta DT4848 系列
//
//==============================================================================
#ifndef MyTemptureDT4848H
#define MyTemptureDT4848H

#include "MyTempture.h"
//---------------------------------------------------------------------------
class TMyDT4848:public TMyTempCtrl
{
    private:
        float ProcessData();
        AnsiString DT4848LRC(AnsiString str);
    protected:
        virtual void ReadData(int Addr, int Command);               // 對address=Index溫控器送出欲讀取之 Registor
        virtual void WriteData(int Addr, int Command, short Value); // 對address=Index溫控器送出欲設定之 Registor
        virtual bool DoReadCurrentTemp();
        virtual bool DoStartSetTemp();
        virtual bool ReadTargetTemp(int Addr);
        virtual bool DoAutoTuning();
        //=================================================
        virtual bool DoReadPIDData();                             //2011.03.10 Q_Q V208D Temp PID Add
        virtual bool DoWritePIDData();                            //2011.03.10 Q_Q V208D Temp PID Add
        //===================================================
        void ReadPIDData(int Addr, int Command);                  //2011.03.10 Q_Q V208D Temp PID Add
        void WritePIDData(int Addr, AnsiString Command, short Value);   //2011.03.10 Q_Q V208D Temp PID Add
};
#endif