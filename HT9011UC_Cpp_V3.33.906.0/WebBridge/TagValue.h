// ===========================================================================
//  WebBridge/TagValue.h
//  One tag value on the browser<->handler seam: Null | Bool | Int | Double |
//  String.
//
//  AI(W906-WebBridge-State) 20260805: new file. NEW-BUILD infrastructure for
//  the web HMI seam (D:\HT9045\web\docs\ARCHITECTURE.md sections 4-5), NOT a
//  translation of any BCB6 golden source.
//
//  WHY A HAND-ROLLED VARIANT AND NOT std::variant
//  ----------------------------------------------
//  std::variant is a C++17 *library* feature and needs libstdc++ >= 7. This
//  tree's MinGW.org GCC 6.3.0 compiles -std=c++17 but ships the GCC 6 library,
//  so <variant> / <optional> are not available. Everything here is C++11-level
//  library usage so the same source builds under both MinGW g++ and MSVC cl.
//
//  Storage is "one field per alternative" rather than a union, deliberately:
//  a std::string alternative in a union needs manual placement-new/destroy,
//  and the payoff would be ~24 bytes on a map of ~234 tags published at 10 Hz.
//  Not worth the class of bug it buys.
//
//  THE Null-vs-EMPTY-STRING DISTINCTION IS LOAD-BEARING
//  ----------------------------------------------------
//  ARCHITECTURE.md section 4, rule 3:
//      null means *unknown / not installed*
//      ""   means *deliberately blank*
//      "They render differently and are not interchangeable."
//  So operator== treats them as DIFFERENT, and the snapshot diff (TagSnapshot)
//  depends on exactly that: a tag flipping Null -> "" or "" -> Null is a real
//  state change the browser must be told about.
//
//  THREADING: TagValue is a plain value type. It owns its bytes, has no
//  shared state, and does no locking. Copying one is safe from any thread;
//  concurrently mutating *the same* instance is not (same rule as std::string).
// ===========================================================================
#ifndef WEBBRIDGE_TAGVALUE_H
#define WEBBRIDGE_TAGVALUE_H

#include <string>
#include <cstdint>

namespace webbridge {

// Which alternative a TagValue currently holds.
enum class TagType {
    Null = 0,   // unknown / not installed        -> JSON null
    Bool,       //                                -> JSON true|false
    Int,        // 64-bit signed                  -> JSON number (integral)
    Double,     //                                -> JSON number
    String      // may be empty ("" = deliberately blank, NOT Null)
};

class TagValue {
public:
    // -------------------------------------------------------------------
    //  Construction
    //
    //  Factories only, no converting constructors. That is on purpose: an
    //  implicit TagValue(bool) sitting next to TagValue(int) and
    //  TagValue(const char*) is the classic overload trap where
    //  TagValue("HALT") quietly becomes Bool(true). Naming the alternative
    //  at every construction site also makes the Null/""/0/false choice
    //  visible in review, which is the distinction section 4 rule 3 cares
    //  about.
    // -------------------------------------------------------------------
    TagValue();                                        // Null
    static TagValue makeNull();
    static TagValue makeBool(bool v);
    static TagValue makeInt(std::int64_t v);           // any integral converts
    static TagValue makeDouble(double v);
    static TagValue makeString(const std::string& v);  // "" stays "" (not Null)

    // -------------------------------------------------------------------
    //  Inspection
    // -------------------------------------------------------------------
    TagType type() const { return type_; }

    bool isNull()   const { return type_ == TagType::Null;   }
    bool isBool()   const { return type_ == TagType::Bool;   }
    bool isInt()    const { return type_ == TagType::Int;    }
    bool isDouble() const { return type_ == TagType::Double; }
    bool isString() const { return type_ == TagType::String; }
    bool isNumber() const { return type_ == TagType::Int || type_ == TagType::Double; }

    // Strict accessors: they do NOT convert between alternatives. Reading the
    // wrong one returns the documented fallback instead of throwing, because
    // this seam runs on a production machine and a bad tag name must not be
    // able to terminate the handler.
    bool          asBool(bool fallback = false) const;              // non-Bool   -> fallback
    std::int64_t  asInt(std::int64_t fallback = 0) const;           // non-Int    -> fallback
    double        asDouble(double fallback = 0.0) const;            // non-Double -> fallback
    // Non-String (including Null) -> empty string. Callers that must tell
    // Null from "" have to ask isNull() / isString(); that is the whole point.
    const std::string& asString() const;

    // Numeric convenience for the common "publish an int, read it as a
    // double" case. Int -> double, Double -> double, anything else fallback.
    double toNumber(double fallback = 0.0) const;

    // -------------------------------------------------------------------
    //  Equality -- this is the diff predicate
    //
    //  Two values are equal only when the TYPE matches and the payload
    //  matches. Consequences worth knowing, all of them intentional:
    //
    //    Null()          != String("")     <-- section 4 rule 3, load-bearing
    //    Bool(true)      != Int(1)
    //    Bool(false)     != Null()
    //    Int(5)          != Double(5.0)    <-- a tag should keep a stable type;
    //                                          a type flip IS a change
    //    String("0")     != Int(0)
    //
    //  Doubles use DIFF-STABILITY equality, not raw IEEE ==:
    //    NaN  == NaN   (raw == would report "changed" on every single tick for
    //                   any tag whose value is NaN -- endless patch traffic)
    //    +0.0 == -0.0  (raw == already agrees; noted so the choice is explicit)
    //  Everything else is plain ==; no epsilon. An epsilon here would silently
    //  hide small real movements of a position/temperature tag.
    // -------------------------------------------------------------------
    bool operator==(const TagValue& o) const;
    bool operator!=(const TagValue& o) const { return !(*this == o); }

    // -------------------------------------------------------------------
    //  Diagnostics
    //
    //  NOT the wire format. Nothing on the WebSocket path may use these --
    //  JSON encoding lives in the serialiser component, which is the only
    //  place that gets to decide quoting/escaping/number formatting.
    // -------------------------------------------------------------------
    const char* typeName() const;
    std::string debugString() const;   // e.g. Null, Bool(true), Str("HALT")

private:
    TagType      type_;
    bool         b_;
    std::int64_t i_;
    double       d_;
    std::string  s_;
};

} // namespace webbridge

#endif // WEBBRIDGE_TAGVALUE_H
