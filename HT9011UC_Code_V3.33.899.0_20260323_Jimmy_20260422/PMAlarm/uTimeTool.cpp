//---------------------------------------------------------------------------

#pragma hdrstop

#include "uTimeTool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
uTimeTool::uTimeTool()
{
}
//---------------------------------------------------------------------------
uTimeTool::~uTimeTool()
{
}
//---------------------------------------------------------------------------
double uTimeTool::DiffTimeSec(TDateTime tMinuend, TDateTime tSubtrahend)
{
    return DiffTimeMin(tMinuend, tSubtrahend)*60;
}
//---------------------------------------------------------------------------
double uTimeTool::DiffTimeMin(TDateTime tMinuend, TDateTime tSubtrahend)
{
    return DiffTimeHour(tMinuend, tSubtrahend)*60;
}
//---------------------------------------------------------------------------
double uTimeTool::DiffTimeHour(TDateTime tMinuend, TDateTime tSubtrahend)
{
    return DiffTimeDay(tMinuend, tSubtrahend)*24;
}
//---------------------------------------------------------------------------
double uTimeTool::DiffTimeDay(TDateTime tMinuend, TDateTime tSubtrahend)
{
    return tMinuend-tSubtrahend;
}
//---------------------------------------------------------------------------
AnsiString uTimeTool::TDatetmeToSavestring(TDateTime tDT)
{
    return tDT.FormatString("yyyymmddhhnnss");
}
//---------------------------------------------------------------------------
TDateTime uTimeTool::SavestringToTDatetme(AnsiString sDate)
{
    if(sDate.Length()!=14)
    {
        return 0.0;
    }
    else
    {
        TDateTime tdDate(atoi(sDate.SubString(1, 4).c_str()),
                         atoi(sDate.SubString(5, 2).c_str()),
                         atoi(sDate.SubString(7, 2).c_str()));

        TDateTime tdTime(atoi(sDate.SubString(9, 2).c_str()),
                         atoi(sDate.SubString(11, 2).c_str()),
                         atoi(sDate.SubString(13, 2).c_str()),
                         0);
        return tdDate+tdTime;
    }
}
//---------------------------------------------------------------------------
AnsiString uTimeTool::TDatetmeToAnsiLog(TDateTime tDT)
{
    return tDT.FormatString("yyyymmdd_hhnnss");
}
//---------------------------------------------------------------------------
TDateTime uTimeTool::AnsiLogToTDatetme(AnsiString sDate)
{
    if(sDate.Length()!=15)
    {
        return 0.0;
    }
    TDateTime tdDate(atoi(sDate.SubString(1, 4).c_str()),
                     atoi(sDate.SubString(5, 2).c_str()),
                     atoi(sDate.SubString(7, 2).c_str()));

    TDateTime tdTime(atoi(sDate.SubString(10, 2).c_str()),
                     atoi(sDate.SubString(12, 2).c_str()),
                     atoi(sDate.SubString(14, 2).c_str()),
                     0);
    return tdDate+tdTime;
}
//---------------------------------------------------------------------------


