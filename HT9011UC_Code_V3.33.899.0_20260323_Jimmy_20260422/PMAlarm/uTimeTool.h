//---------------------------------------------------------------------------

#ifndef uTimeToolH
#define uTimeToolH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <SysUtils.hpp>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class uTimeTool
{
private:

public:     // User declarations
    uTimeTool();
    ~uTimeTool();
    double DiffTimeSec(TDateTime tMinuend, TDateTime tSubtrahend);
    double DiffTimeMin(TDateTime tMinuend, TDateTime tSubtrahend);
    double DiffTimeHour(TDateTime tMinuend, TDateTime tSubtrahend);
    double DiffTimeDay(TDateTime tMinuend, TDateTime tSubtrahend);
    AnsiString TDatetmeToSavestring(TDateTime tDT);
    TDateTime SavestringToTDatetme(AnsiString sDate);
    bool CheckSavestringLength(AnsiString sdata){return sdata.Length()==14?true:false;}   //確認格式是否相符
    TDateTime GetNow(){return Now();}
    AnsiString TDatetmeToAnsiLog(TDateTime tDT);
    TDateTime AnsiLogToTDatetme(AnsiString sDate);
};
//---------------------------------------------------------------------------
#endif
 