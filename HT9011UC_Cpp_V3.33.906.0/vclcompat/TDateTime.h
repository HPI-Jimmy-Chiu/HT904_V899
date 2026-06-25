// ===========================================================================
//  vclcompat/TDateTime.h
//  BCB6 TDateTime compatibility shim.
//
//  TDateTime is a thin wrapper over a `double` whose integer part is the
//  number of days since 1899-12-30 and whose fractional part is the fraction
//  of a 24h day (the classic OLE Automation date, same as Delphi/BCB6).
//  Implemented to support the dominant idioms:
//    * Now()/Time()/Date() -> TDateTime
//    * elapsed-time math:  (Now() - start) etc.  -> implicit double conversion
//    * FormatDateTime(fmt, dt), DecodeDate/DecodeTime, EncodeTime
//
//  FormatDateTime token set (verified against golden ref usage):
//    yyyy/yy year ; mm/m month or MINUTE (see rule) ; dd/d day ;
//    hh/h or HH/H hour (0-23) ; nn/n minute ; ss/s second ; zzz/z millisecond;
//    AM/PM -> 12h clock with marker ; '\' escapes next char ; other chars
//    are emitted literally.
//  BCB6/Delphi month-vs-minute rule for 'm'/'mm': it is interpreted as a
//  MINUTE when it immediately follows an hour token (h/hh) or immediately
//  precedes a second token (s/ss); otherwise it is a MONTH. (So
//  "HH:MM:ss" -> minutes, "yyyy-mm-dd" -> month, matching real format strings
//  like "yyyy-mm-dd HH:MM:ss" and "MM/DD/YYYY HH:NN:SS AM/PM".)
// ===========================================================================
#ifndef VCLCOMPAT_TDATETIME_H
#define VCLCOMPAT_TDATETIME_H

#include "vclcompat/AnsiString.h"

namespace vclcompat {

typedef unsigned short Word;   // BCB6 SysUtils Word, used by DecodeDate/Time

class TDateTime {
public:
    TDateTime() : value_(0.0) {}
    TDateTime(double v) : value_(v) {}
    TDateTime(const TDateTime& o) : value_(o.value_) {}
    TDateTime& operator=(const TDateTime& o) { value_ = o.value_; return *this; }
    TDateTime& operator=(double v) { value_ = v; return *this; }

    // Implicit conversion to double enables (Now()-start) elapsed math and
    // comparisons used pervasively in the source.
    operator double() const { return value_; }
    double Val() const { return value_; }

    TDateTime operator+(const TDateTime& o) const { return TDateTime(value_ + o.value_); }
    TDateTime operator-(const TDateTime& o) const { return TDateTime(value_ - o.value_); }
    TDateTime& operator+=(const TDateTime& o) { value_ += o.value_; return *this; }
    TDateTime& operator-=(const TDateTime& o) { value_ -= o.value_; return *this; }

private:
    double value_;
};

// ---- factories ------------------------------------------------------------
TDateTime Now();    // current local date+time
TDateTime Date();   // current local date (time = 00:00)
TDateTime Time();   // current local time-of-day (date part = 0)

// ---- encode / decode ------------------------------------------------------
void DecodeDate(const TDateTime& dt, Word& year, Word& month, Word& day);
void DecodeTime(const TDateTime& dt, Word& hour, Word& min, Word& sec, Word& msec);
TDateTime EncodeDate(Word year, Word month, Word day);
TDateTime EncodeTime(Word hour, Word min, Word sec, Word msec);

// ---- format / parse -------------------------------------------------------
AnsiString FormatDateTime(const AnsiString& fmt, const TDateTime& dt);
AnsiString DateTimeToStr(const TDateTime& dt);   // "yyyy/mm/dd hh:nn:ss"-ish default
AnsiString TimeToStr(const TDateTime& dt);       // "hh:nn:ss"
TDateTime  StrToDateTime(const AnsiString& s);   // parses common formats

} // namespace vclcompat

#endif // VCLCOMPAT_TDATETIME_H
