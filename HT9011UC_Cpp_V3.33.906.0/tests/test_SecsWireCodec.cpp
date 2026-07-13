// tests/test_SecsWireCodec.cpp
// Verification harness for the SECSGEM/SecsWireCodec translation (the pure
// byte-level SECS-II item codec slice of THGem).
//
// Golden reference:
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h:43-85
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//     (StringOut :392-396, GetLengthOfType :917-932, GetLengthByte :941-974,
//      GetSMLLenthByte :1235-1244, DataItemOut :979-1183, DataItemInSub
//      :2225-2432, GetDataItemLenAndTypeSub :2440-2447, DataItemInNew
//      :7114-7122, DataItemIn(void*) :7125-7130, DataItemIn(AnsiString&)
//      :7134-7281, GetDataItemLenAndType :7290-7296)
//
// SCOPE: no THGem/VCL/socket/grid dependency at all -- this test links
// SecsWireCodec.cpp against vclcompat ONLY (see its own trivial link line).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "SECSGEM/SecsWireCodec.h"
#include <cstdio>
#include <cstring>
#include <string>

static int g_pass = 0;
static int g_fail = 0;

static void check_i(const char* name, int got, int expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, got, expected); ++g_fail; }
}

static void check_u(const char* name, unsigned got, unsigned expected)
{
    if (got == expected) { printf("PASS  %-58s got=%u\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=%u exp=%u\n", name, got, expected); ++g_fail; }
}

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { printf("PASS  %-58s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

// Push one decode burst (Type,len,value...) onto SReceiveData as golden's
// decimal-string tokens (ProcessSML -- out of scope -- is what does this from
// a real received byte stream; tests populate it directly).
static void pushToken(SecsWireCodec& c, int v) { c.SReceiveData->Add(AnsiString(v)); }

int main()
{
    printf("=== SecsWireCodec translation verification ===\n");
    printf("(golden HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp; original binary NOT run)\n\n");

    // -------------------------------------------------------------------
    // HType global -- literal values from golden InitHType() (:355-367).
    // -------------------------------------------------------------------
    printf("-- HType global (golden InitHType literal values) --\n");
    check_u("HType.LIST_TYPE",    HType.LIST_TYPE,    0x00);
    check_u("HType.ASCII_TYPE",   HType.ASCII_TYPE,   0x40);
    check_u("HType.JIS_TYPE",     HType.JIS_TYPE,     0x44);
    check_u("HType.BINARY_TYPE",  HType.BINARY_TYPE,  0x20);
    check_u("HType.BOOLEAN_TYPE", HType.BOOLEAN_TYPE, 0x24);
    check_u("HType.INT_1_TYPE",   HType.INT_1_TYPE,   0x64);
    check_u("HType.INT_2_TYPE",   HType.INT_2_TYPE,   0x68);
    check_u("HType.INT_4_TYPE",   HType.INT_4_TYPE,   0x70);
    check_u("HType.INT_8_TYPE",   HType.INT_8_TYPE,   0x60);
    check_u("HType.UINT_1_TYPE",  HType.UINT_1_TYPE,  0xa4);
    check_u("HType.UINT_2_TYPE",  HType.UINT_2_TYPE,  0xa8);
    check_u("HType.UINT_4_TYPE",  HType.UINT_4_TYPE,  0xb0);
    check_u("HType.UINT_8_TYPE",  HType.UINT_8_TYPE,  0xa0);
    check_u("HType.FT_4_TYPE",    HType.FT_4_TYPE,    0x90);
    check_u("HType.FT_8_TYPE",    HType.FT_8_TYPE,    0x80);
    // VCL_TYPE: golden never assigns it in InitHType() -- faithfully unset
    // (value-initialized to 0). See SecsWireCodec.cpp's HTypeInitializer note.
    check_u("HType.VCL_TYPE (golden gap, unset)", HType.VCL_TYPE, 0x00);

    // -------------------------------------------------------------------
    // GetLengthOfType (golden :917-932)
    // -------------------------------------------------------------------
    printf("\n-- GetLengthOfType --\n");
    {
        SecsWireCodec c;
        check_i("LIST_TYPE -> 1",    c.GetLengthOfType(HType.LIST_TYPE), 1);
        check_i("ASCII_TYPE -> 1",   c.GetLengthOfType(HType.ASCII_TYPE), 1);
        check_i("BINARY_TYPE -> 1",  c.GetLengthOfType(HType.BINARY_TYPE), 1);
        check_i("BOOLEAN_TYPE -> 1", c.GetLengthOfType(HType.BOOLEAN_TYPE), 1);
        check_i("UINT_1_TYPE -> 1",  c.GetLengthOfType(HType.UINT_1_TYPE), 1);
        check_i("INT_1_TYPE -> 1",   c.GetLengthOfType(HType.INT_1_TYPE), 1);
        check_i("UINT_2_TYPE -> 2",  c.GetLengthOfType(HType.UINT_2_TYPE), 2);
        check_i("INT_2_TYPE -> 2",   c.GetLengthOfType(HType.INT_2_TYPE), 2);
        check_i("UINT_4_TYPE -> 4",  c.GetLengthOfType(HType.UINT_4_TYPE), 4);
        check_i("INT_4_TYPE -> 4",   c.GetLengthOfType(HType.INT_4_TYPE), 4);
        check_i("FT_4_TYPE -> 4",    c.GetLengthOfType(HType.FT_4_TYPE), 4);
        check_i("UINT_8_TYPE -> 8",  c.GetLengthOfType(HType.UINT_8_TYPE), 8);
        check_i("INT_8_TYPE -> 8",   c.GetLengthOfType(HType.INT_8_TYPE), 8);
        check_i("FT_8_TYPE -> 8",    c.GetLengthOfType(HType.FT_8_TYPE), 8);

        // Unrecognized format byte -> 0 + a StringOut diagnostic line.
        int before = c.LogDataString->Count;
        int r = c.GetLengthOfType((unsigned char)0xFF);
        check_i("unrecognized Type -> 0", r, 0);
        check_b("unrecognized Type logs a StringOut diagnostic",
                c.LogDataString->Count > before, true);
    }

    // -------------------------------------------------------------------
    // GetLengthByte / GetSMLLenthByte (golden :941-974 / :1235-1244)
    // -------------------------------------------------------------------
    printf("\n-- GetLengthByte / GetSMLLenthByte (inverse pair) --\n");
    {
        SecsWireCodec c;
        unsigned char buf[3];

        unsigned char n1 = c.GetLengthByte(5, buf);       // 1-byte length
        check_u("GetLengthByte(5) byte count", n1, 1);
        check_u("GetLengthByte(5) buf[0]", buf[0], 5);

        unsigned char n2 = c.GetLengthByte(300, buf);     // 2-byte length (0x012C), MSB-first
        check_u("GetLengthByte(300) byte count", n2, 2);
        check_u("GetLengthByte(300) buf[0] (MSB)", buf[0], 0x01);
        check_u("GetLengthByte(300) buf[1] (LSB)", buf[1], 0x2C);

        unsigned char n3 = c.GetLengthByte(70000, buf);   // 3-byte length (0x011170), MSB-first
        check_u("GetLengthByte(70000) byte count", n3, 3);
        check_u("GetLengthByte(70000) buf[0] (MSB)", buf[0], 0x01);
        check_u("GetLengthByte(70000) buf[1]", buf[1], 0x11);
        check_u("GetLengthByte(70000) buf[2] (LSB)", buf[2], 0x70);

        // >3-byte length is a hard error: golden returns 0 + StringOut.
        // (This call partially overwrites buf[0..2] before failing -- do the
        // GetSMLLenthByte roundtrip check on freshly-reproduced 300 bytes,
        // not on buf as left behind by this error path.)
        int before = c.LogDataString->Count;
        unsigned char n4 = c.GetLengthByte(0x01000000u, buf); // needs a 4th byte
        check_u("GetLengthByte(over-3-byte) -> 0", n4, 0);
        check_b("over-3-byte length logs a StringOut diagnostic",
                c.LogDataString->Count > before, true);

        // Inverse roundtrip via GetSMLLenthByte: TypeChar's low 2 bits carry
        // the length-of-length-bytes count. Re-derive buf for 300 fresh (it
        // was clobbered by the over-3-byte-error call just above).
        unsigned char n2b = c.GetLengthByte(300, buf);
        check_u("GetLengthByte(300) re-derived byte count", n2b, n2);
        int back = c.GetSMLLenthByte((unsigned char)(HType.INT_4_TYPE | n2b), buf, 0);
        check_i("GetSMLLenthByte inverse of GetLengthByte(300)", back, 300);
    }

    // -------------------------------------------------------------------
    // DataItemOut(int len, Type, void*P) (golden :986-1183)
    // -------------------------------------------------------------------
    printf("\n-- DataItemOut(len,Type,void*P) encode --\n");
    {
        SecsWireCodec c;
        c.ResetLocalBuffer();
        int vals[2] = { 100, 200 };
        c.DataItemOut(2, HType.INT_4_TYPE, vals);

        unsigned totalLen = (unsigned(c.LocalBuffer[0]) << 24) | (unsigned(c.LocalBuffer[1]) << 16) |
                            (unsigned(c.LocalBuffer[2]) << 8)  |  unsigned(c.LocalBuffer[3]);
        check_u("4-byte length header == LocalLength", totalLen, c.LocalLength);
        // LocalLength counts everything AFTER the 4-byte length header itself
        // (format+lenOfLen byte [1] + length-byte [1] + 2*INT_4 payload [8] = 10).
        check_u("LocalLength (1 fmt + 1 lenbyte + 8 payload = 10)", c.LocalLength, 10);
        check_u("Format|LenOfLen byte", c.LocalBuffer[4], (unsigned)(HType.INT_4_TYPE | 1));
        check_u("Length-of-data byte (2*4=8)", c.LocalBuffer[5], 8);
        check_u("val[0] byte0", c.LocalBuffer[6], 0);
        check_u("val[0] byte3 (=100)", c.LocalBuffer[9], 100);
        check_u("val[1] byte3 (=200)", c.LocalBuffer[13], 200);
    }
    {
        // INT_1 negative value -> two's-complement single byte (0xFF for -1).
        SecsWireCodec c;
        c.ResetLocalBuffer();
        char v = -1;
        c.DataItemOut(1, HType.INT_1_TYPE, &v);
        check_u("INT_1(-1) encodes as 0xFF", c.LocalBuffer[6], 0xFF);
    }
    {
        // BOOLEAN_TYPE array.
        SecsWireCodec c;
        c.ResetLocalBuffer();
        bool bv[2] = { true, false };
        c.DataItemOut(2, HType.BOOLEAN_TYPE, bv);
        check_u("BOOLEAN[0]=true -> 1",  c.LocalBuffer[6], 1);
        check_u("BOOLEAN[1]=false -> 0", c.LocalBuffer[7], 0);
    }
    {
        // FT_4 (float) round-trips its raw big-endian IEEE754 bytes.
        SecsWireCodec c;
        c.ResetLocalBuffer();
        float f = 1.0f;   // 0x3F800000
        c.DataItemOut(1, HType.FT_4_TYPE, &f);
        check_u("FT_4(1.0f) byte0 (MSB)", c.LocalBuffer[6], 0x3F);
        check_u("FT_4(1.0f) byte1",       c.LocalBuffer[7], 0x80);
        check_u("FT_4(1.0f) byte2",       c.LocalBuffer[8], 0x00);
        check_u("FT_4(1.0f) byte3 (LSB)", c.LocalBuffer[9], 0x00);
    }

    // -------------------------------------------------------------------
    // DataItemOut(Type, AnsiString S) (golden :979-984)
    // -------------------------------------------------------------------
    printf("\n-- DataItemOut(Type,AnsiString) ASCII encode --\n");
    {
        SecsWireCodec c;
        c.ResetLocalBuffer();
        c.DataItemOut(HType.ASCII_TYPE, AnsiString("HI"));
        check_u("ASCII Format|LenOfLen byte", c.LocalBuffer[4], (unsigned)(HType.ASCII_TYPE | 1));
        check_u("ASCII length byte", c.LocalBuffer[5], 2);
        check_u("ASCII payload[0]='H'", c.LocalBuffer[6], (unsigned char)'H');
        check_u("ASCII payload[1]='I'", c.LocalBuffer[7], (unsigned char)'I');
    }

    // -------------------------------------------------------------------
    // StringOut (golden :392-396) -- fans out to BOTH sinks.
    // -------------------------------------------------------------------
    printf("\n-- StringOut --\n");
    {
        SecsWireCodec c;
        c.StringOut(AnsiString("hello"));
        check_i("WaitShowString gets the line", c.WaitShowString->Count, 1);
        check_i("LogDataString gets the line",   c.LogDataString->Count, 1);
        check_s("WaitShowString content", c.WaitShowString->GetString(0).str(), "hello");
        check_s("LogDataString content",  c.LogDataString->GetString(0).str(), "hello");
    }

    // -------------------------------------------------------------------
    // DataItemInSub (golden :2225-2432)
    // -------------------------------------------------------------------
    printf("\n-- DataItemInSub decode --\n");
    {
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 2);
        pushToken(c, 100);
        pushToken(c, 200);
        int outv[2] = { 0, 0 };
        int ret = c.DataItemInSub(2, HType.INT_4_TYPE, outv);
        check_i("INT_4 array decode ret", ret, 1);
        check_i("INT_4 array decode val[0]", outv[0], 100);
        check_i("INT_4 array decode val[1]", outv[1], 200);
        check_i("SReceiveData fully consumed", c.SReceiveData->Count, 0);
    }
    {
        // Wrong Type token -> -1 (nothing consumed beyond peek).
        SecsWireCodec c;
        pushToken(c, HType.ASCII_TYPE);
        pushToken(c, 1);
        pushToken(c, 42);
        int v = 0;
        int ret = c.DataItemInSub(1, HType.INT_4_TYPE, &v);
        check_i("wrong Type token -> -1", ret, -1);
    }
    {
        // Wrong length token (non-ASCII: len must match exactly) -> -2.
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 3);        // caller asks for len=1, declared len=3
        pushToken(c, 42);
        int v = 0;
        int ret = c.DataItemInSub(1, HType.INT_4_TYPE, &v);
        check_i("length token != caller len -> -2", ret, -2);
    }
    {
        // ASCII: declared len may be <= caller's buffer; > caller's -> -2.
        SecsWireCodec c;
        pushToken(c, HType.ASCII_TYPE);
        pushToken(c, 10);        // declared len=10 > caller's buffer of 4
        char buf[16];
        int ret = c.DataItemInSub(4, HType.ASCII_TYPE, buf);
        check_i("ASCII declared-len > caller buffer -> -2", ret, -2);
    }
    {
        // LIST_TYPE: no payload tokens consumed, immediate success.
        SecsWireCodec c;
        pushToken(c, HType.LIST_TYPE);
        pushToken(c, 3);          // list item count (not consumed as bytes)
        int ret = c.DataItemInSub(3, HType.LIST_TYPE, NULL);
        check_i("LIST_TYPE -> 1 (no payload consumption)", ret, 1);
        check_i("SReceiveData left with 0 tokens (Type+len only)", c.SReceiveData->Count, 0);
    }
    {
        // BOOLEAN_TYPE decode.
        SecsWireCodec c;
        pushToken(c, HType.BOOLEAN_TYPE);
        pushToken(c, 2);
        pushToken(c, 1);
        pushToken(c, 0);
        bool bv[2] = { false, false };
        int ret = c.DataItemInSub(2, HType.BOOLEAN_TYPE, bv);
        check_i("BOOLEAN decode ret", ret, 1);
        check_b("BOOLEAN[0]", bv[0], true);
        check_b("BOOLEAN[1]", bv[1], false);
    }
    {
        // Empty SReceiveData -> -1.
        SecsWireCodec c;
        int v = 0;
        int ret = c.DataItemInSub(1, HType.INT_4_TYPE, &v);
        check_i("empty SReceiveData -> -1", ret, -1);
    }

    // -------------------------------------------------------------------
    // DataItemIn(len,Type,void*P) -- sticky iReturnCode (golden :7125-7130)
    // -------------------------------------------------------------------
    printf("\n-- DataItemIn(void*) sticky iReturnCode --\n");
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 42);
        int v = 0;
        int ret = c.DataItemIn(1, HType.INT_4_TYPE, &v);
        check_i("first ok call ret", ret, 1);
        check_i("first ok call value", v, 42);
        check_i("iReturnCode stays 1 on success", c.iReturnCode, 1);

        // A failing call latches iReturnCode to the error...
        pushToken(c, HType.ASCII_TYPE);   // wrong type on purpose
        pushToken(c, 1);
        pushToken(c, 42);
        ret = c.DataItemIn(1, HType.INT_4_TYPE, &v);
        check_i("wrong-type call ret", ret, -1);
        check_i("iReturnCode latches to -1", c.iReturnCode, -1);
        // FAITHFUL QUIRK (confirmed against golden, uHGemEquipment.cpp:2244
        // `if(t!=Type) return -1;` fires BEFORE the `SReceiveData->Delete(0);`
        // a few lines below it): a Type-mismatch failure does NOT consume
        // ANY of the mismatched burst's tokens -- they are left in place.
        check_i("Type-mismatch leaves its burst unconsumed (golden quirk)",
                c.SReceiveData->Count, 3);
        // A real caller must discard/resync a mismatched message itself
        // (out of this codec's scope, e.g. via S9F7); simulate that here so
        // the NEXT burst below starts clean.
        c.SReceiveData->Clear();

        // ...and a LATER successful call must NOT clear the sticky error
        // (golden's documented rationale: "避免錯誤碼被後來的正確碼所取代").
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 7);
        ret = c.DataItemIn(1, HType.INT_4_TYPE, &v);
        check_i("later ok call still returns 1", ret, 1);
        check_i("later ok call still decodes the value", v, 7);
        check_i("iReturnCode remains -1 (sticky, not cleared)", c.iReturnCode, -1);
    }

    // -------------------------------------------------------------------
    // DataItemIn(len,Type,AnsiString&) (golden :7134-7281)
    // -------------------------------------------------------------------
    printf("\n-- DataItemIn(AnsiString&) numeric/ASCII formatting --\n");
    {
        SecsWireCodec c;
        AnsiString s;
        int ret = c.DataItemIn(0, HType.INT_4_TYPE, s);
        check_i("len<=0 guard ret", ret, 0);
        check_s("len<=0 guard clears Str", s.str(), "");
        check_i("len<=0 guard resets iReturnCode to 0", c.iReturnCode, 0);
    }
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.ASCII_TYPE);
        pushToken(c, 5);
        pushToken(c, 0);   // placeholder int token replaced below with the real string
        c.SReceiveData->Delete(2);
        c.SReceiveData->Add(AnsiString("HELLO"));
        AnsiString s;
        int ret = c.DataItemIn(5, HType.ASCII_TYPE, s);
        check_i("ASCII decode ret", ret, 1);
        check_s("ASCII decode value", s.str(), "HELLO");
    }
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.UINT_2_TYPE);
        pushToken(c, 1);
        pushToken(c, 999);
        AnsiString s;
        int ret = c.DataItemIn(1, HType.UINT_2_TYPE, s);
        check_i("UINT_2 decode ret", ret, 1);
        check_s("UINT_2 decode formatted value", s.str(), "999");
    }
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.INT_2_TYPE);
        pushToken(c, 1);
        pushToken(c, -5);
        AnsiString s;
        int ret = c.DataItemIn(1, HType.INT_2_TYPE, s);
        check_i("INT_2 negative decode ret", ret, 1);
        check_s("INT_2 negative decode formatted value", s.str(), "-5");
    }
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.FT_4_TYPE);
        pushToken(c, 1);
        c.SReceiveData->Add(AnsiString(2.5));   // atof-parsed
        AnsiString s;
        int ret = c.DataItemIn(1, HType.FT_4_TYPE, s);
        check_i("FT_4 decode ret", ret, 1);
        check_s("FT_4 decode formatted value", s.str(), "2.5");
    }
    {
        // len!=1 with a non-ASCII scalar type -> -1 (golden: "if(len!=1) return -1;").
        SecsWireCodec c;
        AnsiString s;
        int ret = c.DataItemIn(3, HType.INT_4_TYPE, s);
        check_i("non-ASCII len!=1 -> -1", ret, -1);
    }

    // -------------------------------------------------------------------
    // GetDataItemLenAndTypeSub / GetDataItemLenAndType -- PEEK, no delete
    // (golden :2440-2447 / :7290-7296)
    // -------------------------------------------------------------------
    printf("\n-- GetDataItemLenAndType (peek, no delete) --\n");
    {
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 555);
        int len = 0;
        unsigned char type = 0;
        int ret = c.GetDataItemLenAndType(len, type);
        check_i("peek ret", ret, 1);
        check_i("peek len", len, 1);
        check_u("peek type", type, HType.INT_4_TYPE);
        check_i("peek does NOT delete (3 tokens remain)", c.SReceiveData->Count, 3);
    }
    {
        // Fewer than 2 tokens -> -2.
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        int len = 0;
        unsigned char type = 0;
        int ret = c.GetDataItemLenAndType(len, type);
        check_i("peek with <2 tokens -> -2", ret, -2);
    }

    // -------------------------------------------------------------------
    // DataItemInNew (golden :7114-7122) -- peek THEN decode.
    //
    // FAITHFUL QUIRK (confirmed against golden, not a translation bug):
    // GetDataItemLenAndType only PEEKS (Type,len) without deleting; the
    // DataItemIn(len,Type,Str) call right after independently RE-READS (and
    // this time deletes) that SAME (Type,len) pair via DataItemInSub, plus
    // the value token. So SReceiveData needs only ONE copy of
    // (Type,len,value) for a DataItemInNew call -- NOT two.
    // -------------------------------------------------------------------
    printf("\n-- DataItemInNew (peek + redundant-but-faithful re-decode) --\n");
    {
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 555);
        AnsiString s;
        int ret = c.DataItemInNew(s);
        check_i("DataItemInNew ret", ret, 1);
        check_s("DataItemInNew decoded value", s.str(), "555");
        check_i("DataItemInNew fully consumes its 1 (Type,len,value) burst",
                c.SReceiveData->Count, 0);
    }

    // -------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
