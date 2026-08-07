// =============================================================================
//  PMAlarm/uTimeTool.cpp  --  TDateTime <-> fixed-width numeric-string helpers
//
//  Faithful translation of golden PMAlarm/uTimeTool.cpp (85 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (pure-translation completion campaign, group
//    "pmalarm").  See this unit's own uTimeTool.h banner for the full role
//    statement and wave scope table (identical golden-line map applies here).
//
//  ROLE: implementation of the 8 out-of-line members declared in uTimeTool.h
//  (the other 2 -- CheckSavestringLength / GetNow -- are inline in the header,
//  verbatim from golden).
//
//  WAVE SCOPE: ACTIVE, faithful, verbatim -- all 8 golden function bodies below.
//  GATE REGISTER: NONE (see header banner).
//
//  MINIMAL NECESSARY ADAPTATIONS (2, both flagged inline with //AI at the line,
//  forced by this tree's vclcompat::TDateTime shape -- see vclcompat/TDateTime.h):
//   (a) golden :39/:64 `tDT.FormatString("...")` -- BCB6 TDateTime member call.
//       vclcompat::TDateTime carries NO FormatString method (it is a thin
//       double wrapper, vclcompat/TDateTime.h:33-53); the tree's equivalent is
//       the free function `FormatDateTime(fmt, dt)` (vclcompat/TDateTime.h:67,
//       made ambient by `using vclcompat::FormatDateTime;`,
//       vclcompat/vcl_compat.h:334). Same picture-string tokens, same output,
//       reordered call syntax only -- this is the exact adaptation already
//       recorded at SECSGEM/uHGemEquipment.cpp:3591-3599 for the same golden
//       idiom ("golden calls tdSaveTime.FormatString(fmt) ... this tree's
//       FormatDateTime(fmt, dt) free function is the equivalent").
//   (b) golden :50-52/:54-57/:73-75/:77-80 `TDateTime tdDate(y,m,d)` /
//       `TDateTime tdTime(h,mi,s,0)` -- BCB6 TDateTime has date-only (3-arg)
//       and time-only (4-arg) CONSTRUCTOR overloads; vclcompat::TDateTime has
//       neither (only a bare `TDateTime(double)`, vclcompat/TDateTime.h:36).
//       The tree's equivalent pair is the free functions `EncodeDate(y,m,d)` /
//       `EncodeTime(h,mi,s,msec)` (vclcompat/TDateTime.h:63-64), summed exactly
//       as golden sums its two constructed objects (`tdDate+tdTime`) -- this is
//       the tree's own established substitution, pinned by
//       tests/test_vclcompat.cpp:304/329-330, tests/test_common.cpp:348-349 and
//       tests/test_IniFiles.cpp:146 (all of the form
//       `EncodeDate(Y,M,D) + EncodeTime(h,mi,s,ms)`).  Net value identical to
//       golden's date-ctor + time-ctor sum (both are OLE-serial "whole days
//       since epoch" + "day fraction" additions).
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :3 and :7).  No __fastcall / __property / __published.
//  Numeric semantics kept EXACT: DiffTimeSec/Min/Hour all cascade through each
//  other with the same *60/*60/*24 chain golden uses (no algebraic collapsing).
//
//  Big5: this file has no Chinese comments (all in the header). Final gate:
//  ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #pragma hdrstop ; #include "uTimeTool.h" ; #pragma package(smart_init)
// =============================================================================
#include "uTimeTool.h"              // this unit's own contract
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
    //AI(W906-PT-W2) 20260807: golden :39 `tDT.FormatString("yyyymmddhhnnss");`
    //  -- see banner adaptation (a). FormatDateTime(fmt,dt) is the tree's
    //  free-function equivalent of BCB6's TDateTime::FormatString(fmt) member.
    return FormatDateTime("yyyymmddhhnnss", tDT);
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
        //AI(W906-PT-W2) 20260807: golden :50-52/:54-57 `TDateTime tdDate(y,m,d)`
        //  / `TDateTime tdTime(h,mi,s,0)` -- see banner adaptation (b).
        //  EncodeDate/EncodeTime are the tree's free-function equivalents of
        //  BCB6's TDateTime date-only/time-only constructor overloads.
        TDateTime tdDate=EncodeDate(atoi(sDate.SubString(1, 4).c_str()),
                         atoi(sDate.SubString(5, 2).c_str()),
                         atoi(sDate.SubString(7, 2).c_str()));

        TDateTime tdTime=EncodeTime(atoi(sDate.SubString(9, 2).c_str()),
                         atoi(sDate.SubString(11, 2).c_str()),
                         atoi(sDate.SubString(13, 2).c_str()),
                         0);
        return tdDate+tdTime;
    }
}
//---------------------------------------------------------------------------
AnsiString uTimeTool::TDatetmeToAnsiLog(TDateTime tDT)
{
    //AI(W906-PT-W2) 20260807: golden :64 `tDT.FormatString("yyyymmdd_hhnnss");`
    //  -- see banner adaptation (a), same as TDatetmeToSavestring above.
    return FormatDateTime("yyyymmdd_hhnnss", tDT);
}
//---------------------------------------------------------------------------
TDateTime uTimeTool::AnsiLogToTDatetme(AnsiString sDate)
{
    if(sDate.Length()!=15)
    {
        return 0.0;
    }
    //AI(W906-PT-W2) 20260807: golden :73-75/:77-80 -- same adaptation (b) as
    //  SavestringToTDatetme above (EncodeDate/EncodeTime replace golden's
    //  TDateTime date-only/time-only constructor overloads).
    TDateTime tdDate=EncodeDate(atoi(sDate.SubString(1, 4).c_str()),
                     atoi(sDate.SubString(5, 2).c_str()),
                     atoi(sDate.SubString(7, 2).c_str()));

    TDateTime tdTime=EncodeTime(atoi(sDate.SubString(10, 2).c_str()),
                     atoi(sDate.SubString(12, 2).c_str()),
                     atoi(sDate.SubString(14, 2).c_str()),
                     0);
    return tdDate+tdTime;
}
//---------------------------------------------------------------------------
