// ===========================================================================
//  tests/test_wb_crypto.cpp
//  WebBridge crypto + encoding primitives: SHA-1, Base64, JSON writer.
//
//  Every numeric expectation below is a PUBLISHED vector, cited inline with its
//  RFC and section. Nothing here asserts a value that was produced by running
//  this code -- that is the whole point of a spec-vector test.
//
//    SHA-1        RFC 3174 section 7.3 (TEST1..TEST4) + the empty string
//                 (FIPS 180-1 / RFC 3174 appendix, da39a3ee...)
//    Base64       RFC 4648 section 10 ("Test Vectors": "" f fo foo foob fooba
//                 foobar) + RFC 4648 section 4 alphabet/padding rules
//    WS handshake RFC 6455 section 1.3 (the worked dGhlIHNhbXBsZSBub25jZQ==
//                 -> s3pPLMBiTxaQ9kYGzzhZRbK+xOo= example) -- this is the one
//                 vector that proves SHA-1 and Base64 compose correctly.
//    JSON         RFC 8259 section 7 (string escaping) and section 6 (numbers:
//                 "Numeric values that cannot be represented ... such as
//                 Infinity and NaN, are not permitted")
//    null vs ""   web/docs/ARCHITECTURE.md section 4 rule 3 and
//                 web/README.md "Formats"
//
//  Build: no library dependencies beyond the three WebBridge .cpp files.
//  Non-zero exit on any failure.
// ===========================================================================

#include "WebBridge/Sha1.h"
#include "WebBridge/Base64.h"
#include "WebBridge/JsonWriter.h"

#include <cstdio>
#include <cstdlib>
#include <string>

static int g_fail  = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}

static void check_eq_str(const std::string& got, const std::string& want,
                         const char* what, const char* file, int line) {
    ++g_total;
    if (got != want) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n            got  [%s]\n            want [%s]\n",
                    file, line, what, got.c_str(), want.c_str());
    }
}

#define CHECK(cond)              check((cond), #cond, __FILE__, __LINE__)
#define CHECK_STR(got, want)     check_eq_str((got), (want), #got " == " #want, \
                                              __FILE__, __LINE__)

using namespace webbridge;

static std::string Repeat(const std::string& s, int n) {
    std::string out;
    for (int i = 0; i < n; ++i) { out += s; }
    return out;
}

// ---------------------------------------------------------------------------
//  1. SHA-1 -- RFC 3174 section 7.3 test vectors
// ---------------------------------------------------------------------------
static void TestSha1Vectors() {
    // RFC 3174 s7.3 TEST1: "abc" -> A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
    CHECK_STR(Sha1Hex("abc"),
              "a9993e364706816aba3e25717850c26c9cd0d89d");

    // FIPS 180-1 / RFC 3174: SHA-1 of the empty message.
    CHECK_STR(Sha1Hex(""),
              "da39a3ee5e6b4b0d3255bfef95601890afd80709");

    // RFC 3174 s7.3 TEST2: "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
    // -> 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
    CHECK_STR(Sha1Hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
              "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

    // RFC 3174 s7.3 TEST3: one million 'a'
    // -> 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
    CHECK_STR(Sha1Hex(std::string(1000000, 'a')),
              "34aa973cd4c4daa4f61eeb2bdbad27316534016f");

    // RFC 3174 s7.3 TEST4: "0123456701234567..." repeated to 10 blocks of 64
    // ("01234567012345670123456701234567" x 20 = 640 bytes)
    // -> DEA356A2 CDDD90C7 A7ECEDC5 EBB56393 4F460452
    CHECK_STR(Sha1Hex(Repeat("01234567012345670123456701234567", 20)),
              "dea356a2cddd90c7a7ecedc5ebb563934f460452");

    // Digest is 20 raw bytes (RFC 3174 s1: 160-bit output), not 40 hex chars.
    CHECK(Sha1Raw("abc").size() == 20);
    CHECK(kSha1DigestBytes == 20);

    // Incremental Update() must equal single-shot -- the handshake concatenates
    // the client key and the GUID, so this path is the one actually used.
    {
        Sha1 h;
        h.Update("a");
        h.Update("b");
        h.Update("c");
        CHECK_STR(ToHex(h.Final()), "a9993e364706816aba3e25717850c26c9cd0d89d");
        // Final() is documented idempotent.
        CHECK_STR(ToHex(h.Final()), "a9993e364706816aba3e25717850c26c9cd0d89d");
    }
    // Chunking across the 64-byte block boundary (55/56/57/63/64/65 lengths are
    // where the RFC 3174 s4 padding rule bites).
    {
        const std::string msg(64 * 3 + 5, 'x');
        const std::string oneShot = Sha1Hex(msg);
        for (std::size_t split = 1; split < msg.size(); split += 7) {
            Sha1 h;
            h.Update(msg.substr(0, split));
            h.Update(msg.substr(split));
            CHECK(ToHex(h.Final()) == oneShot);
        }
    }
    // Length field must be the message length, not message+padding: 55, 56, 63
    // and 64 'a's are the classic off-by-one padding traps (RFC 3174 s4 -- a
    // 56-byte message needs a whole extra block for the length field).
    // NOT RFC vectors: these four digests were taken from Python 3 hashlib.sha1
    // as an independent oracle, 20260805. Reproduce with:
    //   python -c "import hashlib;print(hashlib.sha1(b'a'*56).hexdigest())"
    CHECK_STR(Sha1Hex(std::string(55, 'a')),
              "c1c8bbdc22796e28c0e15163d20899b65621d65a");
    CHECK_STR(Sha1Hex(std::string(56, 'a')),
              "c2db330f6083854c99d4b5bfb6e8f29f201be699");
    CHECK_STR(Sha1Hex(std::string(63, 'a')),
              "03f09f5b158a7a8cdad920bddc29b81c18a551f5");
    CHECK_STR(Sha1Hex(std::string(64, 'a')),
              "0098ba824b5c16427bd7a1122a5a442a25ec644d");

    // Raw digest must be byte-transparent (NUL-safe): "abc" digest contains none,
    // but the empty-string digest ends in 0x09 and this catches a C-string bug.
    CHECK(Sha1Raw("").size() == 20);
    CHECK(static_cast<unsigned char>(Sha1Raw("")[19]) == 0x09);
}

// ---------------------------------------------------------------------------
//  2. Base64 -- RFC 4648 section 10 test vectors
// ---------------------------------------------------------------------------
static void TestBase64Vectors() {
    // RFC 4648 s10 "Test Vectors" (standard alphabet, s4):
    CHECK_STR(Base64Encode(""),       "");
    CHECK_STR(Base64Encode("f"),      "Zg==");
    CHECK_STR(Base64Encode("fo"),     "Zm8=");
    CHECK_STR(Base64Encode("foo"),    "Zm9v");
    CHECK_STR(Base64Encode("foob"),   "Zm9vYg==");
    CHECK_STR(Base64Encode("fooba"),  "Zm9vYmE=");
    CHECK_STR(Base64Encode("foobar"), "Zm9vYmFy");

    // Same vectors, decode direction.
    static const char* kPlain[] = { "", "f", "fo", "foo", "foob", "fooba", "foobar" };
    static const char* kCoded[] = { "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==",
                                    "Zm9vYmE=", "Zm9vYmFy" };
    for (int i = 0; i < 7; ++i) {
        std::string out;
        CHECK(Base64Decode(kCoded[i], &out));
        CHECK_STR(out, kPlain[i]);
    }

    // RFC 4648 s4: the standard alphabet uses '+' and '/' (index 62, 63).
    // 0xFB 0xFF -> "+/8=" exercises both.
    {
        std::string bytes;
        bytes.push_back(static_cast<char>(0xFB));
        bytes.push_back(static_cast<char>(0xFF));
        CHECK_STR(Base64Encode(bytes), "+/8=");
        std::string back;
        CHECK(Base64Decode("+/8=", &back));
        CHECK(back == bytes);
    }

    // Binary round-trip over every byte value, NUL included.
    {
        std::string all;
        for (int i = 0; i < 256; ++i) {
            all.push_back(static_cast<char>(i));
        }
        std::string back;
        CHECK(Base64Decode(Base64Encode(all), &back));
        CHECK(back == all);
        CHECK(back.size() == 256);
    }

    // Padding is canonical: output length always a multiple of 4 (RFC 4648 s4).
    for (int n = 1; n <= 12; ++n) {
        CHECK((Base64Encode(std::string(n, 'a')).size() % 4) == 0);
    }

    // Strictness -- a malformed Sec-WebSocket-Key must be rejected, not coerced.
    {
        std::string out;
        CHECK(!Base64Decode("Zm9vYmF", &out));    // length not a multiple of 4
        CHECK(!Base64Decode("Zm9v*g==", &out));   // '*' not in the alphabet
        CHECK(!Base64Decode("Zm=9vYg=", &out));   // '=' before the last quantum
        CHECK(!Base64Decode("Z===", &out));       // three pad chars
        CHECK(!Base64Decode("Zm9v-w==", &out));   // URL-safe alphabet is NOT accepted
        CHECK(!Base64Decode("Zm9v_w==", &out));
    }
    // Whitespace is tolerated (header folding / MIME line wrapping).
    {
        std::string out;
        CHECK(Base64Decode("Zm9v\r\nYmFy", &out));
        CHECK_STR(out, "foobar");
        CHECK(Base64Decode("  Zm9vYmFy  ", &out));
        CHECK_STR(out, "foobar");
        CHECK(Base64Decode("   \r\n  ", &out));    // whitespace only -> empty
        CHECK_STR(out, "");
    }
}

// ---------------------------------------------------------------------------
//  3. The composed use: RFC 6455 section 1.3 Sec-WebSocket-Accept
// ---------------------------------------------------------------------------
static void TestWebSocketHandshakeVector() {
    // RFC 6455 s1.3: key "dGhlIHNhbXBsZSBub25jZQ==" plus the GUID
    // 258EAFA5-E914-47DA-95CA-C5AB0DC85B11 must yield
    // "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=".
    const std::string key  = "dGhlIHNhbXBsZSBub25jZQ==";
    const std::string guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    CHECK_STR(Base64Encode(Sha1Raw(key + guid)), "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");

    // RFC 6455 s4.1: the key itself decodes to exactly 16 bytes.
    std::string nonce;
    CHECK(Base64Decode(key, &nonce));
    CHECK(nonce.size() == 16);
}

// ---------------------------------------------------------------------------
//  4. JSON writer -- null vs "", escaping, numbers
// ---------------------------------------------------------------------------
static void TestJsonNullVsEmptyString() {
    // ARCHITECTURE.md s4 rule 3 / README "Formats": null = unknown/not
    // installed ("---"), "" = deliberately blank. Must not collapse.
    CHECK_STR(JsonValue::MakeNull().ToJson(),     "null");
    CHECK_STR(JsonValue::MakeString("").ToJson(), "\"\"");
    CHECK(JsonValue::MakeNull().ToJson() != JsonValue::MakeString("").ToJson());
    CHECK(JsonValue::MakeNull() != JsonValue::MakeString(""));
    CHECK(JsonValue::MakeNull() == JsonValue::MakeNull());
    CHECK(JsonValue::MakeString("") == JsonValue::MakeString(""));
    // Kind-sensitivity in the other pairs a patch differ will meet.
    CHECK(JsonValue::MakeInt(0) != JsonValue::MakeNull());
    CHECK(JsonValue::MakeInt(0) != JsonValue::MakeString("0"));
    CHECK(JsonValue::MakeBool(false) != JsonValue::MakeNull());

    // Through the writer, in the shape of a real patch frame.
    JsonWriter w;
    w.BeginObject();
      w.Key("type").String("patch");
      w.Key("data").BeginObject();
        w.Key("zone.heatgun.1").Null();       // not installed
        w.Key("speed.index.acc").String("");  // deliberately blank
      w.EndObject();
    w.EndObject();
    CHECK(w.Ok());
    CHECK_STR(w.Str(),
              "{\"type\":\"patch\",\"data\":"
              "{\"zone.heatgun.1\":null,\"speed.index.acc\":\"\"}}");
}

static void TestJsonEscaping() {
    // RFC 8259 s7: '"' and '\' MUST be escaped; the two-character forms
    // \b \f \n \r \t are the named ones.
    CHECK_STR(JsonQuote("a\"b"),   "\"a\\\"b\"");
    CHECK_STR(JsonQuote("a\\b"),   "\"a\\\\b\"");
    CHECK_STR(JsonQuote("\b\f\n\r\t"), "\"\\b\\f\\n\\r\\t\"");

    // Other C0 controls as \u00XX (RFC 8259 s7 allows no raw char < 0x20).
    CHECK_STR(JsonQuote(std::string("\x01", 1)),  "\"\\u0001\"");
    CHECK_STR(JsonQuote(std::string("\x1f", 1)),  "\"\\u001f\"");
    CHECK_STR(JsonQuote(std::string("a\0b", 3)),  "\"a\\u0000b\"");   // NUL-safe

    // '/' is legal raw and must NOT be escaped (noise on every tag path).
    CHECK_STR(JsonQuote("a/b"), "\"a/b\"");
    // DEL 0x7F is >= 0x20 and legal raw.
    CHECK_STR(JsonQuote(std::string("\x7f", 1)), "\"\x7f\"");

    // No raw control byte survives anywhere.
    {
        std::string ctrl;
        for (int i = 0; i < 0x20; ++i) { ctrl.push_back(static_cast<char>(i)); }
        const std::string q = JsonQuote(ctrl);
        for (std::size_t i = 0; i < q.size(); ++i) {
            CHECK(static_cast<unsigned char>(q[i]) >= 0x20);
        }
    }
}

static void TestJsonUtf8Policy() {
    // Valid UTF-8 passes through byte-for-byte (RFC 8259 s8.1: JSON text is
    // UTF-8). "測試" = E6 B8 AC E8 A9 A6.
    const std::string utf8Ok = "\xE6\xB8\xAC\xE8\xA9\xA6";
    CHECK(IsValidUtf8(utf8Ok));
    CHECK_STR(JsonQuote(utf8Ok), "\"" + utf8Ok + "\"");

    // Legacy Big5/CP950 input: "測試" = B4 FA B8 D5. Not valid UTF-8, so the
    // documented policy (JsonWriter.h note 2b) transcodes it to the SAME UTF-8
    // bytes as above -- this is the real handler case.
    const std::string big5 = "\xB4\xFA\xB8\xD5";
    CHECK(!IsValidUtf8(big5));
#if defined(_WIN32)
    CHECK_STR(SanitizeToUtf8(big5), utf8Ok);
    CHECK_STR(JsonQuote(big5), "\"" + utf8Ok + "\"");
#endif

    // Whatever the path taken, output is ALWAYS valid UTF-8: a browser drops the
    // entire frame on one bad byte, so this is the invariant that matters.
    {
        // A lone continuation byte 0x80 is neither valid UTF-8 nor valid CP950.
        const std::string junk = std::string("ok\x80", 3) + std::string("\xFF", 1);
        const std::string clean = SanitizeToUtf8(junk);
        CHECK(IsValidUtf8(clean));
        CHECK(clean.find("ok") == 0);          // ASCII survives
        CHECK(clean.find("\xEF\xBF\xBD") != std::string::npos);  // U+FFFD used
        const std::string q = JsonQuote(junk);
        CHECK(IsValidUtf8(q));
    }
    // Every single byte value, alone, must sanitise to valid UTF-8.
    for (int i = 0; i < 256; ++i) {
        const std::string one(1, static_cast<char>(i));
        CHECK(IsValidUtf8(SanitizeToUtf8(one)));
        CHECK(IsValidUtf8(JsonQuote(one)));
    }

    // IsValidUtf8 must reject the classic malformed forms, or invalid bytes
    // would be waved through as "already UTF-8".
    CHECK(!IsValidUtf8("\xC0\xAF"));              // overlong '/'
    CHECK(!IsValidUtf8("\xE0\x80\xAF"));          // overlong, 3-byte
    CHECK(!IsValidUtf8("\xED\xA0\x80"));          // UTF-16 surrogate U+D800
    CHECK(!IsValidUtf8("\xF5\x80\x80\x80"));      // > U+10FFFF
    CHECK(!IsValidUtf8("\xE6\xB8"));              // truncated sequence
    CHECK(!IsValidUtf8("\x80"));                  // lone continuation
    CHECK(IsValidUtf8(""));
    CHECK(IsValidUtf8("plain ascii"));
    CHECK(IsValidUtf8("\xF4\x8F\xBF\xBF"));       // U+10FFFF, the max legal
}

static void TestJsonNumbers() {
    // Integers are exact, including the 64-bit extremes (a tag like a lifetime
    // contact count is int64).
    CHECK_STR(JsonNumber(static_cast<wb_int64>(0)),   "0");
    CHECK_STR(JsonNumber(static_cast<wb_int64>(130)), "130");
    CHECK_STR(JsonNumber(static_cast<wb_int64>(-1)),  "-1");
    CHECK_STR(JsonNumber(static_cast<wb_int64>(9223372036854775807LL)),
              "9223372036854775807");
    CHECK_STR(JsonNumber(static_cast<wb_int64>(-9223372036854775807LL - 1LL)),
              "-9223372036854775808");

    // RFC 8259 s6: Infinity and NaN are NOT permitted in JSON. They must become
    // null (which the web side renders "---"), never "nan"/"inf".
    const double zero = 0.0;
    const double inf  = 1.0 / zero;
    const double nan  = zero / zero;
    CHECK_STR(JsonNumber(inf),  "null");
    CHECK_STR(JsonNumber(-inf), "null");
    CHECK_STR(JsonNumber(nan),  "null");
    CHECK_STR(JsonValue::MakeDouble(nan).ToJson(),  "null");
    CHECK_STR(JsonValue::MakeDouble(inf).ToJson(),  "null");
    CHECK_STR(JsonValue::MakeDouble(-inf).ToJson(), "null");
    {
        JsonWriter w;
        w.BeginObject().Key("temp.pv").Number(nan).EndObject();
        CHECK_STR(w.Str(), "{\"temp.pv\":null}");
        CHECK(w.Str().find("nan") == std::string::npos);
        CHECK(w.Str().find("inf") == std::string::npos);
        CHECK(w.Str().find("NaN") == std::string::npos);
    }

    // Finite doubles: representative temperature / position values, and the
    // requirement that an integral double is still a legal JSON number.
    CHECK_STR(JsonNumber(0.0),      "0");
    CHECK_STR(JsonNumber(130.0),    "130");
    CHECK_STR(JsonNumber(-40.5),    "-40.5");
    CHECK_STR(JsonNumber(11.0),     "11");
    CHECK_STR(JsonNumber(0.125),    "0.125");

    // Round-trip precision: a decimal that is not exactly representable must
    // still come back as the same double. 0.1 and 1/3 are the standard traps.
    {
        static const double kVals[] = {
            0.1, 0.2, 0.3, 1.0 / 3.0, 130.25, -40.125, 1234.5678,
            98.599999999999994, 2.2250738585072014e-308, 1.7976931348623157e308,
            -0.0, 1e-7, 123456789.123456789
        };
        const int n = static_cast<int>(sizeof(kVals) / sizeof(kVals[0]));
        for (int i = 0; i < n; ++i) {
            const std::string text = JsonNumber(kVals[i]);
            CHECK(text.find("nan") == std::string::npos);
            CHECK(text.find("inf") == std::string::npos);
            const double back = std::strtod(text.c_str(), 0);
            if (back != kVals[i]) {
                ++g_fail;
                std::printf("FAIL [%s:%d]  double did not round-trip: %.17g -> [%s] -> %.17g\n",
                            __FILE__, __LINE__, kVals[i], text.c_str(), back);
            }
            ++g_total;
            // Locale-independence: a ',' decimal separator would be invalid JSON.
            CHECK(text.find(',') == std::string::npos);
        }
    }
}

static void TestJsonWriterStructure() {
    // Arrays, nesting, and mixed value kinds -- the alarm frame shape from
    // ARCHITECTURE.md s4.
    JsonWriter w;
    w.BeginObject();
      w.Key("type").String("alarm");
      w.Key("code").String("WAR0152");
      w.Key("ok").Bool(false);
      w.Key("sites").BeginArray();
        w.Number(static_cast<wb_int64>(1));
        w.Number(static_cast<wb_int64>(2));
        w.Null();
        w.String("");
      w.EndArray();
    w.EndObject();
    CHECK(w.Ok());
    CHECK_STR(w.Str(),
              "{\"type\":\"alarm\",\"code\":\"WAR0152\",\"ok\":false,"
              "\"sites\":[1,2,null,\"\"]}");

    // Empty containers.
    { JsonWriter e; e.BeginObject().EndObject(); CHECK(e.Ok()); CHECK_STR(e.Str(), "{}"); }
    { JsonWriter e; e.BeginArray().EndArray();   CHECK(e.Ok()); CHECK_STR(e.Str(), "[]"); }

    // Keys are escaped like any other string (tag names are data too).
    { JsonWriter e; e.BeginObject().Key("a\"b").Null().EndObject();
      CHECK_STR(e.Str(), "{\"a\\\"b\":null}"); }

    // Ok() reports misuse: unclosed container, and a value with no key.
    { JsonWriter e; e.BeginObject(); CHECK(!e.Ok()); }
    { JsonWriter e; e.BeginObject().Null(); CHECK(!e.Ok()); }
    { JsonWriter e; e.EndObject(); CHECK(!e.Ok()); }
    { JsonWriter e; e.BeginObject().Key("k"); CHECK(!e.Ok()); }   // key, no value

    // Clear() resets the error flag and the buffer.
    { JsonWriter e; e.EndObject(); e.Clear();
      e.BeginObject().Key("k").Bool(true).EndObject();
      CHECK(e.Ok()); CHECK_STR(e.Str(), "{\"k\":true}"); }

    // JsonValue::WriteTo agrees with JsonValue::ToJson for every kind.
    {
        JsonValue vals[5];
        vals[0] = JsonValue::MakeNull();
        vals[1] = JsonValue::MakeBool(true);
        vals[2] = JsonValue::MakeInt(static_cast<wb_int64>(-7));
        vals[3] = JsonValue::MakeDouble(1.5);
        vals[4] = JsonValue::MakeString("HALT");
        for (int i = 0; i < 5; ++i) {
            JsonWriter e;
            e.BeginArray();
            vals[i].WriteTo(e);
            e.EndArray();
            CHECK(e.Ok());
            CHECK_STR(e.Str(), "[" + vals[i].ToJson() + "]");
        }
    }
}

int main() {
    TestSha1Vectors();
    TestBase64Vectors();
    TestWebSocketHandshakeVector();
    TestJsonNullVsEmptyString();
    TestJsonEscaping();
    TestJsonUtf8Policy();
    TestJsonNumbers();
    TestJsonWriterStructure();

    std::printf("\ntest_wb_crypto: %d checks, %d failure(s)\n", g_total, g_fail);
    if (g_fail != 0) {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
