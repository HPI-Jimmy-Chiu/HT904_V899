// AI(W906-WebBridge) 20260805: minimal JSON *writer* for the web-bridge wire
// protocol (web/docs/ARCHITECTURE.md section 4). Parsing is NOT done here --
// inbound frames are parsed with the vendored cJSON (Public/cJSON.h).
//
// Serialises exactly the value kinds the tag contract uses: null, bool, 64-bit
// int, double, string, plus objects and arrays to nest them in.
//
// -------------------------------------------------------------------------
// Two behaviours are load-bearing for the browser. Both are covered by
// tests/test_wb_crypto.cpp; do not "simplify" either one.
// -------------------------------------------------------------------------
//
// 1. null and "" are DIFFERENT values.
//      Null()      ->  null      web side renders "---"  (unknown/not installed)
//      String("")  ->  ""        web side renders blank  (deliberately empty)
//    They are not interchangeable (ARCHITECTURE.md section 4 rule 3, and
//    web/README.md "Formats": Number(null) is 0, so collapsing them prints
//    "0.00" for every uninstalled heater zone).
//
// 2. Every emitted string is well-formed JSON *and* well-formed UTF-8.
//    A browser drops the ENTIRE frame on a single invalid byte, so one bad
//    legacy string would blank the whole screen rather than one field.
//
//    Escaping: the JSON-required escapes (" \ \b \f \n \r \t), all other
//    C0 control characters as \u00XX, and DEL(0x7F) left as-is (legal JSON).
//    '/' is NOT escaped (legal, and escaping it is noise).
//
//    NON-UTF-8 INPUT -- the documented policy, applied in this order:
//      a. If the whole string is already valid UTF-8, it is passed through
//         unchanged (only the escapes above are applied).
//      b. Otherwise the string is assumed to be legacy Big5 / CP950 -- which
//         is what the V899 sources and machine .ini/.csv files actually hold --
//         and is transcoded CP950 -> UTF-8. This is the common real case:
//         handler text such as Big5 B4 FA B8 D5 becomes U+6E2C U+8A66.
//         (Windows only; MultiByteToWideChar with codepage 950.)
//      c. If that transcode also fails (mixed/corrupt bytes, or non-Windows
//         build), each byte that is not valid UTF-8 on its own is replaced with
//         U+FFFD REPLACEMENT CHARACTER. ASCII bytes always survive.
//    The result is that invalid input degrades to visible replacement glyphs in
//    one field, and never to a dropped frame.
//
// 3. Doubles never serialise as "nan"/"inf"/"-inf" (those are NOT valid JSON and
//    would also drop the frame) -- non-finite values are emitted as null, which
//    the web side already renders as "---" i.e. "unknown". Finite values are
//    written with the shortest decimal form that round-trips exactly through
//    strtod, so temperatures and positions survive the trip. Formatting is
//    locale-independent: a locale whose decimal point is ',' is corrected.
//
// Layer rule: WebBridge/ must stay independent of VCL / vclcompat. Standard
// headers only (plus <windows.h> inside the .cpp for the CP950 transcode).
#ifndef WEBBRIDGE_JSONWRITER_H
#define WEBBRIDGE_JSONWRITER_H

#include <string>
#include <vector>
#include <cstddef>

#if defined(_MSC_VER) && _MSC_VER < 1900
typedef __int64 wb_int64;
#else
#include <cstdint>
typedef std::int64_t wb_int64;
#endif

namespace webbridge {

// ---------------------------------------------------------------------------
//  Free functions -- the primitives. Usable without a JsonWriter instance.
// ---------------------------------------------------------------------------

// Returns raw re-encoded as valid UTF-8 per the policy documented above.
// No quoting, no escaping -- this is the encoding stage only.
std::string SanitizeToUtf8(const std::string& raw);

// Returns a complete JSON string token INCLUDING the surrounding quotes.
// JsonQuote("") == "\"\"" -- an empty JSON string, which is not null.
std::string JsonQuote(const std::string& raw);

// Number tokens.
std::string JsonNumber(wb_int64 v);
// Non-finite -> the literal text "null" (see note 3 above).
std::string JsonNumber(double v);

// True when s is well-formed UTF-8 (exposed because callers and tests need it;
// rejects overlong forms, surrogates D800-DFFF, and anything above U+10FFFF).
bool IsValidUtf8(const std::string& s);

// ---------------------------------------------------------------------------
//  JsonWriter -- streaming builder for objects/arrays.
// ---------------------------------------------------------------------------
//
//   JsonWriter w;
//   w.BeginObject();
//     w.Key("type").String("patch");
//     w.Key("data").BeginObject();
//       w.Key("temp.pv").Number((wb_int64)130);
//       w.Key("zone.heatgun.1").Null();
//       w.Key("speed.index.acc").String("");
//     w.EndObject();
//   w.EndObject();
//   const std::string& frame = w.Str();
//
// Every method returns *this so calls can chain. Misuse (EndObject without a
// BeginObject, a value where a key is required) sets a sticky error flag which
// Ok() reports; the buffer is then not trustworthy. Nothing throws.
class JsonWriter {
public:
    JsonWriter();

    JsonWriter& BeginObject();
    JsonWriter& EndObject();
    JsonWriter& BeginArray();
    JsonWriter& EndArray();

    // Object member name. Must be followed by exactly one value.
    JsonWriter& Key(const std::string& name);

    JsonWriter& Null();
    JsonWriter& Bool(bool v);
    JsonWriter& Number(wb_int64 v);
    JsonWriter& Number(double v);
    JsonWriter& String(const std::string& v);

    // Emits a value token verbatim -- for splicing an already-serialised
    // fragment. The caller owns its validity.
    JsonWriter& RawValue(const std::string& jsonText);

    void Clear();

    // False if any misuse was detected, or if a container was left unclosed.
    bool Ok() const;

    const std::string& Str() const { return buf_; }

private:
    void BeforeValue();

    enum Ctx { kCtxObject, kCtxArray };

    std::string       buf_;
    std::vector<char> stack_;      // kCtxObject / kCtxArray
    bool              needComma_;  // a sibling has already been written here
    bool              keyPending_; // Key() was called, a value is required
    bool              ok_;
};

// ---------------------------------------------------------------------------
//  JsonValue -- a tagged value, so a caller can hold null and "" as genuinely
//  distinct entries.
//
//  OVERLAP NOTE (read before using this in new code): WebBridge/TagValue.h is
//  the CANONICAL value type for the snapshot/patch layer, and it makes the same
//  null-vs-"" distinction. Glue code that already has a TagValue should walk it
//  and call JsonWriter directly rather than converting through JsonValue --
//  two value types with two copies of the null rule is exactly how the rule
//  drifts. JsonValue exists so JsonWriter is usable, and testable, with no
//  dependency on the snapshot layer.
// ---------------------------------------------------------------------------
class JsonValue {
public:
    enum Kind { kNull, kBool, kInt, kDouble, kString };

    JsonValue();                                   // null
    static JsonValue MakeNull();
    static JsonValue MakeBool(bool v);
    static JsonValue MakeInt(wb_int64 v);
    static JsonValue MakeDouble(double v);
    static JsonValue MakeString(const std::string& v);

    Kind kind() const { return kind_; }
    bool IsNull() const { return kind_ == kNull; }

    bool     b() const { return b_; }
    wb_int64 i() const { return i_; }
    double   d() const { return d_; }
    const std::string& s() const { return s_; }

    // Equality is kind-sensitive: MakeNull() != MakeString(""), which is the
    // whole point (used by the patch differ so a null->"" change is a change).
    bool operator==(const JsonValue& o) const;
    bool operator!=(const JsonValue& o) const { return !(*this == o); }

    // Serialised token, e.g. "null", "true", "130", "1.5", "\"HALT\"".
    std::string ToJson() const;

    void WriteTo(JsonWriter& w) const;

private:
    Kind        kind_;
    bool        b_;
    wb_int64    i_;
    double      d_;
    std::string s_;
};

} // namespace webbridge

#endif // WEBBRIDGE_JSONWRITER_H
