// ===========================================================================
//  vclcompat/TDateTime.cpp -- BCB6 TDateTime implementation.
//  Day 0 of the serial is 1899-12-30 (OLE epoch), matching BCB6/Delphi.
// ===========================================================================
#include "vclcompat/TDateTime.h"

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
#  include <windows.h>
#endif

namespace vclcompat {

namespace {

// Days from a proleptic-Gregorian civil date to 1970-01-01 (Howard Hinnant's
// days_from_civil). Returns a signed day count; works for any reasonable year.
long long daysFromCivil(int y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153u * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return static_cast<long long>(era) * 146097 + static_cast<long long>(doe) - 719468;
}

// Inverse of daysFromCivil: serial days since 1970-01-01 -> civil date.
void civilFromDays(long long z, int& y, unsigned& m, unsigned& d) {
    z += 719468;
    const long long era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const int yy = static_cast<int>(yoe) + static_cast<int>(era) * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    d = doy - (153 * mp + 2) / 5 + 1;
    m = mp + (mp < 10 ? 3 : -9);
    y = yy + (m <= 2);
}

// OLE epoch 1899-12-30 expressed as days since 1970-01-01 (negative).
const long long kOleEpochDaysFrom1970 = daysFromCivil(1899, 12, 30); // = -25569

// Build the OLE serial double from a broken-down local time.
double serialFromTm(int year, int mon, int day, int hour, int min, int sec, int msec) {
    long long days = daysFromCivil(year, static_cast<unsigned>(mon),
                                   static_cast<unsigned>(day)) - kOleEpochDaysFrom1970;
    double frac = (hour * 3600.0 + min * 60.0 + sec + msec / 1000.0) / 86400.0;
    return static_cast<double>(days) + frac;
}

// Split an OLE serial into date (days since OLE epoch) and time fraction.
// Mirrors BCB6: the time part is the absolute fractional magnitude.
void splitSerial(double serial, long long& dayssince1970, double& dayFraction) {
    double days = std::floor(serial);
    double frac = serial - days;          // [0,1)
    dayssince1970 = static_cast<long long>(days) + kOleEpochDaysFrom1970;
    dayFraction = frac;
}

} // namespace

// ---------------------------------------------------------------------------
//  Factories (local time, like BCB6 Now/Date/Time)
// ---------------------------------------------------------------------------
static double nowSerialWithMs() {
#if defined(_WIN32)
    // Windows (MinGW or MSVC): GetLocalTime gives local time with millisecond
    // precision directly, with no clock_gettime / timespec dependency.
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    return serialFromTm(st.wYear, st.wMonth, st.wDay,
                        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
    // POSIX: second precision (millisecond not needed off-target).
    std::time_t t = std::time(0);
    struct tm lt;
    localtime_r(&t, &lt);
    return serialFromTm(lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
                        lt.tm_hour, lt.tm_min, lt.tm_sec, 0);
#endif
}

TDateTime Now()  { return TDateTime(nowSerialWithMs()); }
TDateTime Date() { return TDateTime(std::floor(nowSerialWithMs())); }
TDateTime Time() { double s = nowSerialWithMs(); return TDateTime(s - std::floor(s)); }

// ---------------------------------------------------------------------------
//  Encode / Decode
// ---------------------------------------------------------------------------
void DecodeDate(const TDateTime& dt, Word& year, Word& month, Word& day) {
    long long d1970; double frac;
    splitSerial(dt.Val(), d1970, frac);
    int y; unsigned m, d;
    civilFromDays(d1970, y, m, d);
    year  = static_cast<Word>(y);
    month = static_cast<Word>(m);
    day   = static_cast<Word>(d);
}

void DecodeTime(const TDateTime& dt, Word& hour, Word& min, Word& sec, Word& msec) {
    long long d1970; double frac;
    splitSerial(dt.Val(), d1970, frac);
    // total milliseconds in the day, rounded to nearest ms.
    long long totalMs = static_cast<long long>(frac * 86400000.0 + 0.5);
    if (totalMs >= 86400000LL) totalMs = 86399999LL;
    msec = static_cast<Word>(totalMs % 1000); totalMs /= 1000;
    sec  = static_cast<Word>(totalMs % 60);   totalMs /= 60;
    min  = static_cast<Word>(totalMs % 60);   totalMs /= 60;
    hour = static_cast<Word>(totalMs % 24);
}

TDateTime EncodeDate(Word year, Word month, Word day) {
    return TDateTime(serialFromTm(year, month, day, 0, 0, 0, 0));
}

TDateTime EncodeTime(Word hour, Word min, Word sec, Word msec) {
    double frac = (hour * 3600.0 + min * 60.0 + sec + msec / 1000.0) / 86400.0;
    return TDateTime(frac);
}

// ---------------------------------------------------------------------------
//  FormatDateTime
// ---------------------------------------------------------------------------
AnsiString FormatDateTime(const AnsiString& fmt, const TDateTime& dt) {
    Word year, month, day, hour, min, sec, msec;
    DecodeDate(dt, year, month, day);
    DecodeTime(dt, hour, min, sec, msec);

    const std::string f = fmt.str();
    std::string out;
    char tmp[16];

    // 12-hour clock support: when an AM/PM marker is present, hours print 1..12.
    bool use12h = false;
    {
        std::string up;
        for (size_t i = 0; i < f.size(); ++i)
            up += static_cast<char>(std::toupper(static_cast<unsigned char>(f[i])));
        use12h = (up.find("AM/PM") != std::string::npos);
    }

    static const char* monthAbbr[13] = {"", "Jan","Feb","Mar","Apr","May","Jun",
                                         "Jul","Aug","Sep","Oct","Nov","Dec"};

    size_t i = 0;
    while (i < f.size()) {
        char c = f[i];

        // '\' escapes the next literal char.
        if (c == '\\') {
            if (i + 1 < f.size()) { out += f[i + 1]; i += 2; }
            else { ++i; }
            continue;
        }

        // AM/PM literal marker (case-insensitive) -> emit AM or PM.
        if ((c == 'A' || c == 'a') && i + 4 < f.size()) {
            std::string seg = f.substr(i, 5);
            std::string segUp;
            for (size_t k = 0; k < seg.size(); ++k)
                segUp += static_cast<char>(std::toupper(static_cast<unsigned char>(seg[k])));
            if (segUp == "AM/PM") {
                out += (hour < 12) ? "AM" : "PM";
                i += 5;
                continue;
            }
        }

        char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (lower == 'y' || lower == 'm' || lower == 'd' ||
            lower == 'h' || lower == 'n' || lower == 's' || lower == 'z') {
            // count run length of identical (case-insensitive) token char
            size_t run = 1;
            while (i + run < f.size() &&
                   std::tolower(static_cast<unsigned char>(f[i + run])) == lower)
                ++run;

            int dispHour = hour;
            if (use12h && (lower == 'h')) {
                dispHour = hour % 12;
                if (dispHour == 0) dispHour = 12;
            }

            if (lower == 'y') {
                if (run >= 4) std::snprintf(tmp, sizeof(tmp), "%04d", year);
                else          std::snprintf(tmp, sizeof(tmp), "%02d", year % 100);
                out += tmp;
            } else if (lower == 'd') {
                if (run >= 2) std::snprintf(tmp, sizeof(tmp), "%02d", day);
                else          std::snprintf(tmp, sizeof(tmp), "%d", day);
                out += tmp;
            } else if (lower == 'h') {
                if (run >= 2) std::snprintf(tmp, sizeof(tmp), "%02d", dispHour);
                else          std::snprintf(tmp, sizeof(tmp), "%d", dispHour);
                out += tmp;
            } else if (lower == 'n') {                 // minute (always)
                if (run >= 2) std::snprintf(tmp, sizeof(tmp), "%02d", min);
                else          std::snprintf(tmp, sizeof(tmp), "%d", min);
                out += tmp;
            } else if (lower == 's') {
                if (run >= 2) std::snprintf(tmp, sizeof(tmp), "%02d", sec);
                else          std::snprintf(tmp, sizeof(tmp), "%d", sec);
                out += tmp;
            } else if (lower == 'z') {                 // millisecond
                if (run >= 3) std::snprintf(tmp, sizeof(tmp), "%03d", msec);
                else          std::snprintf(tmp, sizeof(tmp), "%d", msec);
                out += tmp;
            } else if (lower == 'm') {
                // 'm'/'mm' -> minute if the nearest neighbouring date/time TOKEN
                // (skipping separator chars like ':', ' ', '/') is an hour token
                // before it, or a second token after it; otherwise month.
                // (Delphi/BCB6 rule; e.g. "HH:MM:ss" -> minutes despite the ':'.)
                bool isMinute = false;
                // scan backward to the previous token char (skip separators)
                for (int j = static_cast<int>(i) - 1; j >= 0; --j) {
                    char pc = static_cast<char>(std::tolower(static_cast<unsigned char>(f[static_cast<size_t>(j)])));
                    if (pc == 'h') { isMinute = true; break; }
                    if (pc == 'y' || pc == 'd' || pc == 's' || pc == 'n' ||
                        pc == 'z' || pc == 'm') break;   // hit another token -> not hour
                    // else: separator char, keep scanning
                }
                // scan forward to the next token char (skip separators)
                if (!isMinute) {
                    for (size_t j = i + run; j < f.size(); ++j) {
                        char nc = static_cast<char>(std::tolower(static_cast<unsigned char>(f[j])));
                        if (nc == 's') { isMinute = true; break; }
                        if (nc == 'y' || nc == 'd' || nc == 'h' || nc == 'n' ||
                            nc == 'z' || nc == 'm') break;  // other token -> month
                        // else: separator, keep scanning
                    }
                }
                if (isMinute) {
                    if (run >= 2) std::snprintf(tmp, sizeof(tmp), "%02d", min);
                    else          std::snprintf(tmp, sizeof(tmp), "%d", min);
                    out += tmp;
                } else if (run >= 3) {                 // mmm -> month abbrev
                    out += monthAbbr[(month >= 1 && month <= 12) ? month : 0];
                } else if (run == 2) {
                    std::snprintf(tmp, sizeof(tmp), "%02d", month);
                    out += tmp;
                } else {
                    std::snprintf(tmp, sizeof(tmp), "%d", month);
                    out += tmp;
                }
            }
            i += run;
            continue;
        }

        // Any other char: literal passthrough.
        out += c;
        ++i;
    }
    return AnsiString(out);
}

AnsiString DateTimeToStr(const TDateTime& dt) {
    return FormatDateTime(AnsiString("yyyy/mm/dd hh:nn:ss"), dt);
}

AnsiString TimeToStr(const TDateTime& dt) {
    return FormatDateTime(AnsiString("hh:nn:ss"), dt);
}

// Best-effort parse for the formats this project actually produces:
//   "YYYY/MM/DD HH:NN:SS", "YYYY-MM-DD HH:NN:SS", date-only, time-only.
TDateTime StrToDateTime(const AnsiString& s) {
    int Y = 1899, Mo = 12, D = 30, h = 0, m = 0, sec = 0;
    std::string str = s.Trim().str();
    // normalise separators to spaces for a single sscanf attempt
    int n = std::sscanf(str.c_str(), "%d%*[-/]%d%*[-/]%d %d:%d:%d",
                        &Y, &Mo, &D, &h, &m, &sec);
    if (n >= 3) {
        return TDateTime(serialFromTm(Y, Mo, D, h, m, sec, 0));
    }
    // time only "HH:NN:SS"
    if (std::sscanf(str.c_str(), "%d:%d:%d", &h, &m, &sec) >= 2) {
        return EncodeTime(static_cast<Word>(h), static_cast<Word>(m),
                          static_cast<Word>(sec), 0);
    }
    return TDateTime(0.0);
}

} // namespace vclcompat
