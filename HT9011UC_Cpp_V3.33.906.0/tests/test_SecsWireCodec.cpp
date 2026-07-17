// tests/test_SecsWireCodec.cpp
// Verification harness for the SECSGEM/SecsWireCodec translation (the pure
// byte-level SECS-II item codec slice of THGem).
//
// Golden reference:
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h:43-85
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//     (StringOut :392-396, GetLengthOfType :917-932, GetLengthByte :941-974,
//      GetSMLLenthByte :1235-1244, DataItemOut :979-1183, DataItemInSub
//      :2225-2432, GetDataItemLenAndTypeSub :2440-2447,
//      GetDataItemLenAndTypeAndDeleteSub :2453-2462 (WAVE 3), DataItemInNew
//      :7114-7122, DataItemIn(void*) :7125-7130, DataItemIn(AnsiString&)
//      :7134-7281, GetDataItemLenAndTypeAndDelete :7099-7106 (WAVE 3),
//      GetDataItemLenAndType :7290-7296, SendInvalidDataMessageToHost
//      :7353-7358 (WAVE 3))
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

    //=====================================================================
    // WAVE 2 -- frame header builder/parser + byte-stream decoder/
    //           pretty-printer (golden uHGemEquipment.cpp, per-function
    //           citations given at each block below).
    //=====================================================================

    // -------------------------------------------------------------------
    // ConvertLocalData (golden :847-854)
    // -------------------------------------------------------------------
    printf("\n-- ConvertLocalData (MSB-first raw byte copy) --\n");
    {
        SecsWireCodec c;
        c.ResetLocalBuffer();   // LocalLength_4 = 4
        unsigned short v = 0x0102;
        c.ConvertLocalData(2, &v);
        check_u("ConvertLocalData 2-byte MSB", c.LocalBuffer[4], 0x01);
        check_u("ConvertLocalData 2-byte LSB", c.LocalBuffer[5], 0x02);
        check_u("ConvertLocalData advances LocalLength_4", c.LocalLength_4, 6);
    }

    // -------------------------------------------------------------------
    // CreateLocalHead (golden :859-889) + InitLocalHead (golden :894-912)
    // -------------------------------------------------------------------
    printf("\n-- CreateLocalHead / InitLocalHead --\n");
    {
        SecsWireCodec c;
        c.InitLocalHead(6, 11, 1);   // S6F11, W-bit=1 (odd FCode -> bumps EquipmentSystemByte)
        check_u("InitLocalHead sets Local.MessageID_S", c.Local.MessageID_S, 6);
        check_u("InitLocalHead sets Local.MessageID_F", c.Local.MessageID_F, 11);
        check_u("InitLocalHead sets Local.W_Bit", c.Local.W_Bit, 1);
        check_u("InitLocalHead odd FCode bumps EquipmentSystemByte to 1", c.EquipmentSystemByte, 1);
        check_u("InitLocalHead odd FCode: Local.SystemByte == EquipmentSystemByte", c.Local.SystemByte, 1);
        check_u("CreateLocalHead: LocalLength_4 == 14 (4 len-header + 10 head)", c.LocalLength_4, 14);
        check_u("CreateLocalHead: LocalLength == 10", c.LocalLength, 10);
        // Wire layout: LocalBuffer[4..13] = DeviceID(2) MessageID_S|Wbit(1) MessageID_F(1) PType(1) SType(1) SystemByte(4)
        check_u("CreateLocalHead: MessageID_S byte has W-bit 0x80 packed in", c.LocalBuffer[6], (unsigned)(6 | 0x80));
        check_u("CreateLocalHead: MessageID_F byte", c.LocalBuffer[7], 11);
        check_u("CreateLocalHead: SystemByte LSB (==1)", c.LocalBuffer[13], 1);
        // 4-byte length header at LocalBuffer[0..3] == LocalLength (10)
        unsigned hdrLen = (unsigned(c.LocalBuffer[0]) << 24) | (unsigned(c.LocalBuffer[1]) << 16) |
                          (unsigned(c.LocalBuffer[2]) << 8) | unsigned(c.LocalBuffer[3]);
        check_u("CreateLocalHead: 4-byte length header == LocalLength", hdrLen, c.LocalLength);

        // Even FCode -> Local.SystemByte echoes Remote.SystemByte instead.
        c.Remote.SystemByte = 777;
        c.InitLocalHead(6, 12, 0);
        check_u("InitLocalHead even FCode: Local.SystemByte echoes Remote.SystemByte", c.Local.SystemByte, 777);
        check_u("InitLocalHead even FCode: EquipmentSystemByte NOT bumped again", c.EquipmentSystemByte, 1);
    }

    // -------------------------------------------------------------------
    // HostConvertToData (golden :2150-2159) + ProcessRemoteHead (golden
    // :2164-2201, GATED widget read #1/4 -- see SecsWireCodec.cpp)
    // -------------------------------------------------------------------
    printf("\n-- HostConvertToData / ProcessRemoteHead --\n");
    {
        SecsWireCodec c;
        unsigned char buf[4] = { 0x00, 0x00, 0x01, 0x00 };   // 256, MSB-first
        unsigned decoded = 0;
        c.Host_HSMS_RunLength = 4;
        unsigned char *next = c.HostConvertToData(buf, 4, &decoded);
        check_u("HostConvertToData decodes MSB-first value", decoded, 256);
        check_i("HostConvertToData advances the returned pointer by len",
                (int)(next - buf), 4);
        check_u("HostConvertToData decrements Host_HSMS_RunLength", c.Host_HSMS_RunLength, 0);
    }
    {
        // A 14-byte wire head, with the length field declaring 14 (10 head
        // bytes + a fictional 4-byte body that this focused head-only test
        // does not itself include): 4-byte length(=14) + DeviceID(2)=0x1234
        // + MessageID_S=0x86 (S=6, W-bit set) + MessageID_F=11 + PType=0 +
        // SType=0 + SystemByte(4)=99.
        SecsWireCodec c;
        unsigned char head[14] = {
            0x00, 0x00, 0x00, 0x0E,             // length = 14 (10 head + 4 fictional body)
            0x12, 0x34,                         // DeviceID = 0x1234
            0x86,                                // MessageID_S=6 | W-bit(0x80)
            0x0B,                                // MessageID_F=11
            0x00,                                // PType
            0x00,                                // SType
            0x00, 0x00, 0x00, 0x63              // SystemByte = 99
        };
        c.ProcessRemoteHead(head);
        check_u("ProcessRemoteHead decodes RemoteLength", c.RemoteLength, 14);
        check_u("ProcessRemoteHead decodes Remote.DeviceID", c.Remote.DeviceID, 0x1234);
        check_u("ProcessRemoteHead unpacks W-bit from MessageID_S top bit", c.Remote.W_Bit, 1);
        check_u("ProcessRemoteHead masks W-bit off MessageID_S", c.Remote.MessageID_S, 6);
        check_u("ProcessRemoteHead decodes MessageID_F", c.Remote.MessageID_F, 11);
        check_u("ProcessRemoteHead decodes SystemByte", c.Remote.SystemByte, 99);
        // Host_HSMS_RunLength is seeded from RemoteLength(14), then
        // decremented by every HostConvertToData call ProcessRemoteHead
        // itself makes while consuming the 10-byte head (2+1+1+1+1+4=10) --
        // leaving 4, the fictional body's byte count still to be processed
        // by a future ProcessSML/DataItemIn* call (out of THIS test's scope).
        check_u("ProcessRemoteHead: Host_HSMS_RunLength == RemoteLength - 10-byte head",
                c.Host_HSMS_RunLength, 4);
        // GATED widget read #1/4: GemCheckBoxShowHeadInformation->Checked
        // always reads false offline -- no trace lines emitted either way.
        check_i("ProcessRemoteHead: gated head-info trace produces no StringOut lines",
                c.WaitShowString->Count, 0);
    }

    // -------------------------------------------------------------------
    // StoreToReceiveString (golden :1442-1447) -- gated by bReceiveData
    // -------------------------------------------------------------------
    printf("\n-- StoreToReceiveString (bReceiveData gate) --\n");
    {
        SecsWireCodec c;
        check_b("bReceiveData defaults false", c.bReceiveData, false);
        c.StoreToReceiveString(AnsiString("ignored"));
        check_i("StoreToReceiveString no-ops while bReceiveData==false", c.SReceiveData->Count, 0);
        c.bReceiveData = true;
        c.StoreToReceiveString(AnsiString("kept"));
        check_i("StoreToReceiveString appends while bReceiveData==true", c.SReceiveData->Count, 1);
        check_s("StoreToReceiveString appended value", c.SReceiveData->GetString(0).str(), "kept");
    }

    // -------------------------------------------------------------------
    // LocalAcknowledge (golden :2207-2214, corrected from the task's
    // "~2207-2224" estimate -- golden's actual body ends at :2214) + its
    // gated SendLocalData prerequisite.
    // -------------------------------------------------------------------
    printf("\n-- LocalAcknowledge (+ gated SendLocalData prerequisite) --\n");
    {
        SecsWireCodec c;
        c.bReceiveData = true;   // SendLocalData's one real side effect: forces this back false.
        c.LocalAcknowledge(6, 2, 0);   // S6F2, Command=0 (Acknowledge)
        check_u("LocalAcknowledge sets Local.MessageID_S via InitLocalHead", c.Local.MessageID_S, 6);
        check_u("LocalAcknowledge sets Local.MessageID_F via InitLocalHead", c.Local.MessageID_F, 2);
        // DataItemOut(1, BINARY_TYPE, &C=0) encodes right after the 14-byte head.
        check_u("LocalAcknowledge BINARY ack byte format", c.LocalBuffer[14],
                (unsigned)(HType.BINARY_TYPE | 1));
        check_u("LocalAcknowledge BINARY ack byte length", c.LocalBuffer[15], 1);
        check_u("LocalAcknowledge BINARY ack payload (Command=0)", c.LocalBuffer[16], 0);
        check_b("LocalAcknowledge's gated SendLocalData() resets bReceiveData to false",
                c.bReceiveData, false);
    }

    // -------------------------------------------------------------------
    // StringBinaryOut (golden :401-404) -- genuinely empty in golden itself.
    // -------------------------------------------------------------------
    printf("\n-- StringBinaryOut (genuinely empty golden body) --\n");
    {
        SecsWireCodec c;
        c.StringBinaryOut(AnsiString("anything"));
        check_i("StringBinaryOut touches neither WaitShowString...", c.WaitShowString->Count, 0);
        check_i("...nor LogDataString (golden body is commented out)", c.LogDataString->Count, 0);
    }

    // -------------------------------------------------------------------
    // MakeSMLSpaceString / ShowSMLSpaceString / ShowSMLSpaceBinaryString
    // (golden :1194-1224; 1 gated checkbox read in ShowSMLSpaceBinaryString)
    // -------------------------------------------------------------------
    printf("\n-- MakeSMLSpaceString / ShowSMLSpaceString / ShowSMLSpaceBinaryString --\n");
    {
        SecsWireCodec c;
        c.iSMLSpace = 0;
        c.MakeSMLSpaceString();
        check_s("iSMLSpace<=0 -> SpaceStr empty", c.SpaceStr.str(), "");
        c.iSMLSpace = 2;
        c.MakeSMLSpaceString();
        check_s("iSMLSpace==2 -> SpaceStr is 4 spaces", c.SpaceStr.str(), "    ");

        c.ShowSMLSpaceString(AnsiString("hello"));
        check_s("ShowSMLSpaceString prefixes SpaceStr", c.WaitShowString->GetString(0).str(), "    hello");

        // GATED widget read #4/4 (2nd call site reading the same conceptual
        // GemCheckBoxShowBinary widget as ShowSMLBinary below): always false
        // offline -> ShowSMLSpaceBinaryString always takes its own early return.
        int before = c.WaitShowString->Count;
        c.ShowSMLSpaceBinaryString(AnsiString("binary trace"));
        check_i("ShowSMLSpaceBinaryString gated no-op leaves WaitShowString untouched",
                c.WaitShowString->Count, before);
    }

    // -------------------------------------------------------------------
    // ShowSFDescription (golden :1411-1436) -- static S,F lookup table.
    // -------------------------------------------------------------------
    printf("\n-- ShowSFDescription (SFCodeAndMean[] lookup) --\n");
    {
        SecsWireCodec c;
        AnsiString sf = c.ShowSFDescription(1, 1);
        check_s("ShowSFDescription(1,1) -> SFCode text", sf.str(), "S1F1");
        check_s("ShowSFDescription(1,1) logs the matched English description",
                c.LogDataString->GetString(0).str(), "[S1F1] Are you There");
    }
    {
        // A Big5-origin (cp950-read, transcribed as UTF-8) Chinese table
        // entry -- exercises the SAME lookup path for a non-ASCII row.
        SecsWireCodec c;
        AnsiString sf = c.ShowSFDescription(101, 1);
        check_s("ShowSFDescription(101,1) -> SFCode text", sf.str(), "S101F1");
        check_i("ShowSFDescription(101,1) logs a matched (Chinese) description",
                c.LogDataString->Count, 1);
    }
    {
        // Unknown S,F code -> falls through to the "i>=arrayCount" branch:
        // StringOut("") and the bare SFCode text is still returned.
        SecsWireCodec c;
        AnsiString sf = c.ShowSFDescription(200, 1);
        check_s("ShowSFDescription(200,1) unknown code -> bare SFCode", sf.str(), "S200F1");
        check_s("ShowSFDescription(200,1) unknown code logs an empty line",
                c.LogDataString->GetString(0).str(), "");
    }

    // -------------------------------------------------------------------
    // ProcessSMLBinary (golden :1250-1329) / ShowSMLBinary (golden
    // :1334-1345, GATED widget read #3/4) -- recursive hex-dump
    // pretty-printer smoke test (hand-crafted, not log-derived).
    // -------------------------------------------------------------------
    printf("\n-- ProcessSMLBinary / ShowSMLBinary (hex dump) --\n");
    {
        SecsWireCodec c;
        c.InitLocalHead(6, 11, 0);   // builds the real 14-byte wire head ShowSMLBinary's fixed RunLength=14 start expects
        int v = 100;
        c.DataItemOut(1, HType.INT_4_TYPE, &v);   // appends a 6-byte item -> LocalLength_4 == 20
        int ret = c.ShowSMLBinary(c.LocalBuffer.data(), (int)c.LocalLength_4);
        check_i("ShowSMLBinary/ProcessSMLBinary decode ret", ret, 0);
        // GATED widget read #3/4 (GemCheckBoxShowBinary->Checked==true gates
        // one trailing StringBinaryOut("") call, itself a no-op regardless --
        // see StringBinaryOut's own comment) -- nothing observable either way.
    }

    //=====================================================================
    // ProcessSML (golden :1452-1863) + ShowSML (golden :1869-1882, GATED
    // widget read #2/4) -- the core inbound byte -> token decoder.
    //=====================================================================

    // ---------------------------------------------------------------------
    // REAL CAPTURED-TRAFFIC ORACLE: D:\SECS_GEM_LOGS\2023_11_08\
    // SECSGEM_TextLog_16.txt lines 56-68, a real [S6F11] Event Report Send
    // captured on 2023-11-08 16:01:39:
    //   [S6F11] Event Report Send
    //   <L[3]
    //     <U4[1] 1>
    //     <U4[1] 92>
    //     <L[1]
    //       <L[2]
    //         <U4[1] 92>
    //         <L[0]
    //         >
    //       >
    //     >
    //   >
    // The log only records ShowSML's DECODED TEXT trace, not the raw wire
    // bytes (no byte-level capture exists in D:\SECS_GEM_LOGS) -- so the 26
    // raw SECS-II bytes below were HAND-ENCODED from this real structure
    // (item-by-item: L[3]{ U4[1]=1, U4[1]=92, L[1]{ L[2]{ U4[1]=92, L[0]{} }
    // } }), then verified by hand against golden's own line-by-line
    // recursion/indentation logic (documented inline below) before being
    // typed in. The ORACLE being checked is the real log's own decoded tree
    // structure/values/indentation, not a byte-exact capture -- reported
    // plainly per the task's own instructions on this distinction.
    // ---------------------------------------------------------------------
    printf("\n-- ProcessSML real-traffic oracle (D:\\SECS_GEM_LOGS S6F11) --\n");
    {
        unsigned char body[] = {
            0x01, 0x03,                          // L[3]
            0xb1, 0x04, 0x00, 0x00, 0x00, 0x01,  //   U4[1] = 1
            0xb1, 0x04, 0x00, 0x00, 0x00, 0x5c,  //   U4[1] = 92
            0x01, 0x01,                          //   L[1]
            0x01, 0x02,                          //     L[2]
            0xb1, 0x04, 0x00, 0x00, 0x00, 0x5c,  //       U4[1] = 92
            0x01, 0x00                           //       L[0]
        };
        const int bodyLen = (int)sizeof(body);

        SecsWireCodec c;
        c.bReceiveData = true;   // so the token trail is captured for the decode-back check below.
        int RunLength = 0;
        int ret = c.ProcessSML(body, bodyLen, RunLength);
        check_i("oracle S6F11: ProcessSML ret", ret, 0);
        check_i("oracle S6F11: fully consumed (RunLength==bodyLen)", RunLength, bodyLen);

        static const char *expectedLines[] = {
            "<L[3]",
            "  <U4[1] 1>",
            "  <U4[1] 92>",
            "  <L[1]",
            "    <L[2]",
            "      <U4[1] 92>",
            "      <L[0]",
            "      >",
            "    >",
            "  >",
            ">",
        };
        const int expectedLineCount = (int)(sizeof(expectedLines) / sizeof(expectedLines[0]));
        check_i("oracle S6F11: trace line count matches real log (11 lines)",
                c.WaitShowString->Count, expectedLineCount);
        for (int li = 0; li < expectedLineCount && li < c.WaitShowString->Count; ++li)
        {
            char name[96];
            sprintf(name, "oracle S6F11: trace line[%d] matches real log", li);
            check_s(name, c.WaitShowString->GetString(li).str(), expectedLines[li]);
        }

        // Decode-back: the SAME token trail ProcessSML just pushed into
        // SReceiveData (bReceiveData==true) round-trips through the
        // already-existing DataItemInSub family back to the original values.
        int v1 = 0, v2 = 0, v3 = 0;
        check_i("oracle S6F11 decode-back: outer L[3] header",
                c.DataItemInSub(3, HType.LIST_TYPE, NULL), 1);
        check_i("oracle S6F11 decode-back: U4[1] value1 ret",
                c.DataItemInSub(1, HType.UINT_4_TYPE, &v1), 1);
        check_i("oracle S6F11 decode-back: U4[1] value1 == 1", v1, 1);
        check_i("oracle S6F11 decode-back: U4[1] value2 ret",
                c.DataItemInSub(1, HType.UINT_4_TYPE, &v2), 1);
        check_i("oracle S6F11 decode-back: U4[1] value2 == 92", v2, 92);
        check_i("oracle S6F11 decode-back: L[1] header",
                c.DataItemInSub(1, HType.LIST_TYPE, NULL), 1);
        check_i("oracle S6F11 decode-back: L[2] header",
                c.DataItemInSub(2, HType.LIST_TYPE, NULL), 1);
        check_i("oracle S6F11 decode-back: nested U4[1] value ret",
                c.DataItemInSub(1, HType.UINT_4_TYPE, &v3), 1);
        check_i("oracle S6F11 decode-back: nested U4[1] value == 92", v3, 92);
        check_i("oracle S6F11 decode-back: L[0] header",
                c.DataItemInSub(0, HType.LIST_TYPE, NULL), 1);
        check_i("oracle S6F11 decode-back: SReceiveData fully consumed (17 tokens)",
                c.SReceiveData->Count, 0);
    }

    // ---------------------------------------------------------------------
    // HAND-CRAFTED round trip: Group A encoders (InitLocalHead/
    // CreateLocalHead/ConvertLocalData) build a message; Group A decoders
    // (ProcessRemoteHead/HostConvertToData) + ProcessSML decode it back --
    // exercising every one of this wave's 15 functions together in one flow.
    // Explicitly NOT a D:\SECS_GEM_LOGS-derived oracle -- said plainly here
    // per the task's own instructions on that distinction.
    // ---------------------------------------------------------------------
    printf("\n-- hand-crafted encode/decode round trip (all Wave-2 functions) --\n");
    {
        SecsWireCodec c;
        c.InitLocalHead(1, 1, 1);          // S1F1, W-bit=1
        int val = 12345;
        c.DataItemOut(1, HType.INT_4_TYPE, &val);

        // "Receive" the very message we just built, as if it arrived from
        // the Host over the wire (ProcessRemoteHead/HostConvertToData decode
        // the identical 4-byte-length + 10-byte-head wire format
        // CreateLocalHead/ConvertLocalData just wrote).
        c.ProcessRemoteHead(c.LocalBuffer.data());
        check_u("round trip: RemoteLength == encoded LocalLength", c.RemoteLength, c.LocalLength);
        check_u("round trip: Remote.MessageID_S == Local.MessageID_S", c.Remote.MessageID_S, c.Local.MessageID_S);
        check_u("round trip: Remote.MessageID_F == Local.MessageID_F", c.Remote.MessageID_F, c.Local.MessageID_F);
        check_u("round trip: Remote.W_Bit == Local.W_Bit", c.Remote.W_Bit, c.Local.W_Bit);
        check_u("round trip: Remote.SystemByte == Local.SystemByte", c.Remote.SystemByte, c.Local.SystemByte);

        c.bReceiveData = true;
        int RunLength = 14;   // body starts right after the 14-byte wire head
        // RemoteLength is "everything after the 4-byte length header itself"
        // (10-byte head + item payload), so the buffer's own total length is
        // 4 + RemoteLength -- NOT 14 + RemoteLength (that would double-count
        // the 10-byte head, already included in RemoteLength).
        int ret = c.ProcessSML(c.LocalBuffer.data(), (int)(4 + c.RemoteLength), RunLength);
        check_i("round trip: ProcessSML decode ret", ret, 0);
        check_i("round trip: ProcessSML consumed exactly the body",
                RunLength, (int)(4 + c.RemoteLength));

        int decodedVal = 0;
        ret = c.DataItemInSub(1, HType.INT_4_TYPE, &decodedVal);
        check_i("round trip: decoded value ret", ret, 1);
        check_i("round trip: decoded value == original (12345)", decodedVal, 12345);
    }

    // ---------------------------------------------------------------------
    // HAND-CRAFTED ProcessSML branch coverage: ASCII / BINARY / BOOLEAN /
    // negative INT_2 -- small standalone scalar items (not wrapped in a
    // LIST), each built by hand from the SECS-II wire format rules, not
    // derived from D:\SECS_GEM_LOGS.
    // ---------------------------------------------------------------------
    printf("\n-- ProcessSML hand-crafted branch coverage --\n");
    {
        unsigned char asciiItem[] = { 0x41, 0x02, 'H', 'I' };   // ASCII_TYPE|1, len=2, "HI"
        SecsWireCodec c;
        int RunLength = 0;
        int ret = c.ProcessSML(asciiItem, (int)sizeof(asciiItem), RunLength);
        check_i("ASCII branch ret", ret, 0);
        check_s("ASCII branch trace line", c.WaitShowString->GetString(0).str(), "<A[2] \"HI\">");
    }
    {
        unsigned char binItem[] = { 0x21, 0x02, 0xAB, 0xCD };   // BINARY_TYPE|1, len=2
        SecsWireCodec c;
        int RunLength = 0;
        int ret = c.ProcessSML(binItem, (int)sizeof(binItem), RunLength);
        check_i("BINARY branch ret", ret, 0);
        check_s("BINARY branch trace line", c.WaitShowString->GetString(0).str(), "<B[2] 0xAB 0xCD>");
    }
    {
        unsigned char boolItem[] = { 0x25, 0x02, 0x01, 0x00 };   // BOOLEAN_TYPE|1, len=2
        SecsWireCodec c;
        int RunLength = 0;
        int ret = c.ProcessSML(boolItem, (int)sizeof(boolItem), RunLength);
        check_i("BOOLEAN branch ret", ret, 0);
        check_s("BOOLEAN branch trace line", c.WaitShowString->GetString(0).str(), "<Boolean[2] 0x01 0x00>");
    }
    {
        // INT_2, value=-5, two's-complement big-endian: 0xFFFB.
        unsigned char negItem[] = { 0x69, 0x02, 0xFF, 0xFB };   // INT_2_TYPE|1, len=2 (1 item * 2 bytes)
        SecsWireCodec c;
        int RunLength = 0;
        int ret = c.ProcessSML(negItem, (int)sizeof(negItem), RunLength);
        check_i("negative INT_2 branch ret", ret, 0);
        check_s("negative INT_2 branch trace line (two's-complement decode)",
                c.WaitShowString->GetString(0).str(), "<I2[1] -5>");
    }

    // -------------------------------------------------------------------
    // ShowSML (golden :1869-1882, GATED widget read #2/4: `DB->Clear()`) --
    // top-level entry point wraps ProcessSML with the 14-byte head offset.
    // -------------------------------------------------------------------
    printf("\n-- ShowSML (top-level entry point) --\n");
    {
        SecsWireCodec c;
        c.InitLocalHead(1, 1, 0);
        int val = 42;
        c.DataItemOut(1, HType.INT_4_TYPE, &val);
        c.ShowSML(c.LocalBuffer.data(), (int)c.LocalLength_4);
        // GATED widget read #2/4 (DB->Clear()) is a no-op offline; the
        // decode/trace work below it in ShowSML's own body is unaffected.
        check_b("ShowSML leaves bOutputBusy false when done", c.bOutputBusy, false);
        check_i("ShowSML produced at least one decoded trace line",
                c.WaitShowString->Count > 0, true);
    }

    //=====================================================================
    // WAVE 3 -- destructive (peek+consume) GetDataItemLenAndType siblings +
    //           SendInvalidDataMessageToHost (golden citations at each block).
    //=====================================================================

    // -------------------------------------------------------------------
    // GetDataItemLenAndTypeAndDeleteSub / GetDataItemLenAndTypeAndDelete
    // (golden :2453-2462 / :7099-7106) -- PEEK-vs-DELETE oracle: same input
    // burst, same decoded (len,Type), but this pair actually shrinks
    // SReceiveData by exactly 2 tokens where GetDataItemLenAndType (above)
    // left all 3 untouched.
    // -------------------------------------------------------------------
    printf("\n-- GetDataItemLenAndTypeAndDelete (peek+consume, oracle vs. the peek-only pair above) --\n");
    {
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 555);
        int len = 0;
        unsigned char type = 0;
        int ret = c.GetDataItemLenAndTypeAndDelete(len, type);
        check_i("delete ret", ret, 1);
        check_i("delete len", len, 1);
        check_u("delete type", type, HType.INT_4_TYPE);
        check_i("delete consumes exactly 2 tokens (Type,len) -- 1 remains (the value)",
                c.SReceiveData->Count, 1);
    }
    {
        // Same 3-token burst through the PEEK-only sibling first, to make the
        // peek-vs-delete contrast explicit within a single test run (not just
        // "trust the two tests above separately").
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        pushToken(c, 555);
        int len = 0;
        unsigned char type = 0;

        int peekRet = c.GetDataItemLenAndType(len, type);
        check_i("peek (GetDataItemLenAndType) ret", peekRet, 1);
        check_i("peek leaves ALL 3 tokens in place", c.SReceiveData->Count, 3);

        int delRet = c.GetDataItemLenAndTypeAndDelete(len, type);
        check_i("delete (GetDataItemLenAndTypeAndDelete) ret on the SAME burst", delRet, 1);
        check_i("delete len matches the peek", len, 1);
        check_u("delete type matches the peek", type, HType.INT_4_TYPE);
        check_i("delete THEN shrinks by exactly 2 (1 remains -- the value token)",
                c.SReceiveData->Count, 1);
    }
    {
        // Fewer than 2 tokens -> -2 (same guard as the peek-only Sub).
        SecsWireCodec c;
        pushToken(c, HType.INT_4_TYPE);
        int len = 0;
        unsigned char type = 0;
        int ret = c.GetDataItemLenAndTypeAndDelete(len, type);
        check_i("delete with <2 tokens -> -2", ret, -2);
        check_i("delete with <2 tokens does NOT delete anything (guard fires first)",
                c.SReceiveData->Count, 1);
    }
    {
        // Sticky iReturnCode wrapper behavior (golden :7099-7106): once
        // latched to a non-1 error, a LATER successful call must not
        // overwrite it (same "初始值=1,...避免錯誤碼被後來的正確碼所取代"
        // convention already exercised for GetDataItemLenAndType above).
        SecsWireCodec c;
        c.iReturnCode = 1;
        pushToken(c, HType.INT_4_TYPE);   // only 1 token -> first call returns -2
        int len = 0;
        unsigned char type = 0;
        int ret1 = c.GetDataItemLenAndTypeAndDelete(len, type);
        check_i("first (underrun) call ret", ret1, -2);
        check_i("iReturnCode latches to the error", c.iReturnCode, -2);

        pushToken(c, HType.INT_4_TYPE);
        pushToken(c, 1);
        int ret2 = c.GetDataItemLenAndTypeAndDelete(len, type);
        check_i("second (now-successful) call ret", ret2, 1);
        check_i("iReturnCode stays latched to the earlier error, NOT overwritten",
                c.iReturnCode, -2);
    }

    // -------------------------------------------------------------------
    // SendInvalidDataMessageToHost (golden uHGemEquipment.cpp:7353-7358) --
    // pure InitLocalHead(9,7,0)+DataItemOut(ASCII)+SendLocalData composition;
    // SendLocalData is itself GATED (see its own comment), so the only
    // observable in-scope effects are the wire header S,F code and the
    // encoded ASCII payload sitting in LocalBuffer afterward.
    // -------------------------------------------------------------------
    printf("\n-- SendInvalidDataMessageToHost (S9F7-shaped composer, no leading StringOut) --\n");
    {
        SecsWireCodec c;
        c.SendInvalidDataMessageToHost("bad format");
        check_u("wire head S-code == 9 (Local.MessageID_S)", c.Local.MessageID_S, 9);
        check_u("wire head F-code == 7 (Local.MessageID_F)", c.Local.MessageID_F, 7);
        check_u("wire head W-bit == 0", c.Local.W_Bit, 0);
        // LocalBuffer layout after InitLocalHead+DataItemOut: [0..3] length
        // header, [4..13] 10-byte HSMS head, [14] Format|LenOfLen byte,
        // [15] length-of-data byte, [16..] the "bad format" ASCII payload
        // (10 chars) -- same layout DataItemOut's own tests already verify
        // byte-by-byte elsewhere in this file, spot-checked here only for
        // the payload itself.
        check_u("encoded item Format byte == ASCII_TYPE|1 (1 length byte)",
                c.LocalBuffer[14], (unsigned)(HType.ASCII_TYPE | 1));
        check_u("encoded item length byte == 10 (\"bad format\")", c.LocalBuffer[15], 10);
        std::string payload(reinterpret_cast<const char*>(&c.LocalBuffer[16]), 10);
        check_s("encoded ASCII payload", payload, "bad format");
        // golden's own SendLocalData (uHGemEquipment.cpp:1985-2107) is GATED
        // here (comms layer out of scope) -- its one preserved in-scope
        // effect, bReceiveData=false, is still faithfully reproduced.
        c.bReceiveData = true;
        c.SendInvalidDataMessageToHost("bad format 2");
        check_b("SendLocalData's gated stub still resets bReceiveData=false",
                c.bReceiveData, false);
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
