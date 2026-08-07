// =============================================================================
//  PMAlarm/uTimeTool.h  --  TDateTime <-> fixed-width numeric-string helpers
//
//  Faithful translation of golden PMAlarm/uTimeTool.h (31 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (pure-translation completion campaign, group
//    "pmalarm").  FIRST file of golden PMAlarm/ to land; its sibling
//    PMAlarm/PMAlarmSystem.{h,cpp} (this same wave) is the only in-tree consumer
//    (via a `uTimeTool utimetool;` member, golden PMAlarmSystem.h:72).
//
//  ROLE: a tiny leaf value-type helper class -- TDateTime subtraction expressed
//  as seconds/minutes/hours/days, plus two fixed-width numeric date/time string
//  round-trips ("yyyymmddhhnnss" and "yyyymmdd_hhnnss").  NO state machine, NO
//  file I/O, NO globals of its own.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim -- all 10 golden members, whole file):
//     uTimeTool() / ~uTimeTool()                          golden :16-17
//     DiffTimeSec / DiffTimeMin / DiffTimeHour / DiffTimeDay   golden :18-21
//     TDatetmeToSavestring / SavestringToTDatetme          golden :22-23
//     CheckSavestringLength (inline in header)             golden :24
//     GetNow (inline in header)                            golden :25
//     TDatetmeToAnsiLog / AnsiLogToTDatetme                golden :26-27
//   SATISFIED-BY-SUBSTRATE: vclcompat::TDateTime / AnsiString / Now() /
//     EncodeDate / EncodeTime / FormatDateTime (see .cpp banner for the two
//     constructor-shape adaptations this substrate forces).
//   GATE REGISTER: NONE.  Every symbol this header names already exists in the
//     port tree (vclcompat, all made ambient by vclcompat/vcl_compat.h's
//     `using vclcompat::...;` block, vcl_compat.h:244-334).
//
//  VCL/Borland conversions: golden's `#include <System.hpp>` / `<SysUtils.hpp>`
//  (golden :6-7) -- the BCB6 RTL headers that declare AnsiString/TDateTime/Now --
//  replaced by `#include "MachineDefine.h"`, this tree's de-VCL'd stand-in for
//  the whole BCB6 RTL include stack (see MachineDefine.h:11's own banner note).
//  No __fastcall / __property / __published anywhere in golden's uTimeTool --
//  none reproduced here either.
//
//  Big5: the one Chinese comment (golden :24, "確認格式是否相符") decoded via
//  cp950 and preserved as UTF-8, in place.  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <System.hpp> ; #include <SysUtils.hpp> ; using namespace std;
// =============================================================================
#ifndef uTimeToolH
#define uTimeToolH
//---------------------------------------------------------------------------
#include "MachineDefine.h"          // de-VCL'd include hub -> vclcompat umbrella (AnsiString/TDateTime/Now/EncodeDate/EncodeTime/FormatDateTime all ambient)
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
