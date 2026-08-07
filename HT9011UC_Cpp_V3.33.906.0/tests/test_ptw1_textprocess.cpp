// =============================================================================
//  tests/test_ptw1_textprocess.cpp -- PT-W1 golden-oracle test for
//  EJ1N/TextProcess.cpp
//
//  AI(W906-PT-W1-Tests) 20260807.
//
//  THE ORACLE IS GOLDEN, NEVER THE PORT.  Every expected value below was derived
//  BY HAND from
//    D:/HT9045/HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/EJ1N/TextProcess.cpp
//  (cited as "golden :NNN" on each assertion) and NOT read off the port.  If an
//  assertion fails, the port is wrong, not the test -- do not weaken it.
//
//  NOT CALLED HERE: GetCOMPortStatus (golden :595-631) opens a real COM port via
//  ::CreateFile.  Hardware, therefore out of scope by construction.
//
//  NOT CALLED HERE: HexStrToInt(AnsiString) (golden :347-356) is GATED OUT of the
//  port (#if 0 at the port's EJ1N/TextProcess.cpp:501-513) because
//  vclcompat::HexStrToInt(const AnsiString&) already occupies that overload; that
//  path is covered by tests/test_vclcompat.cpp:336-353.  The char* overload IS
//  exercised here.
//
//  1-BASED reminder: AnsiString is 1-based in this project, so the FIRST and the
//  LAST character position of every string-indexing function is asserted
//  explicitly, not just a middle one.
// =============================================================================
#include "EJ1N/TextProcess.h"

#include <cstdio>
#include <cstring>
#include <cmath>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { std::printf("  PASS: %s\n", msg); ++g_pass; }                \
        else      { std::printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Reports the actual value on failure -- the point of the exercise is to be able
// to say what the port returned, not merely that it disagreed.
#define CHECK_INT(actual, expected, msg)                                        \
    do {                                                                        \
        const long _a = (long)(actual);                                         \
        const long _e = (long)(expected);                                       \
        if (_a == _e) { std::printf("  PASS: %s\n", msg); ++g_pass; }            \
        else { std::printf("  FAIL: %s  -- golden expects %ld, port returned %ld  (line %d)\n", \
                           msg, _e, _a, __LINE__); ++g_fail; }                  \
    } while (0)

#define CHECK_STR(actual, expected, msg)                                        \
    do {                                                                        \
        const AnsiString _a = (actual);                                         \
        const AnsiString _e = (expected);                                       \
        if (_a == _e) { std::printf("  PASS: %s\n", msg); ++g_pass; }            \
        else { std::printf("  FAIL: %s  -- golden expects \"%s\", port returned \"%s\"  (line %d)\n", \
                           msg, _e.c_str(), _a.c_str(), __LINE__); ++g_fail; }  \
    } while (0)

#define CHECK_DBL(actual, expected, msg)                                        \
    do {                                                                        \
        const double _a = (actual);                                             \
        const double _e = (expected);                                           \
        if (std::fabs(_a - _e) <= 1e-9) { std::printf("  PASS: %s\n", msg); ++g_pass; } \
        else { std::printf("  FAIL: %s  -- golden expects %.10f, port returned %.10f  (line %d)\n", \
                           msg, _e, _a, __LINE__); ++g_fail; }                  \
    } while (0)

// ---------------------------------------------------------------------------
//  [1] the two conversion tables, element by element (golden :8 and :9)
// ---------------------------------------------------------------------------
static void test_tables()
{
    std::printf("\n[1] T_HEX2ASCII / T_ASXII2HEX -- table CONTENTS vs golden initialiser\n");

    // golden :8
    //   const unsigned char T_HEX2ASCII[16]=
    //       {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    static const unsigned char kHex2Ascii[16] =
        {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int i;
    int bad = 0;
    for (i = 0; i < 16; ++i) {
        if (T_HEX2ASCII[i] != kHex2Ascii[i]) {
            std::printf("    T_HEX2ASCII[%d]: golden 0x%02X ('%c'), port 0x%02X\n",
                        i, kHex2Ascii[i], kHex2Ascii[i], T_HEX2ASCII[i]);
            ++bad;
        }
    }
    CHECK(bad == 0, "T_HEX2ASCII: all 16 elements match golden :8 exactly");
    // First and last element called out separately -- an off-by-one in a table
    // copy shows at the ends, not in the middle.
    CHECK_INT(T_HEX2ASCII[0],  '0', "T_HEX2ASCII[0]=='0'  (FIRST element, golden :8)");
    CHECK_INT(T_HEX2ASCII[15], 'F', "T_HEX2ASCII[15]=='F' (LAST element, golden :8)");
    CHECK_INT(T_HEX2ASCII[9],  '9', "T_HEX2ASCII[9]=='9'  (digit/letter seam, golden :8)");
    CHECK_INT(T_HEX2ASCII[10], 'A', "T_HEX2ASCII[10]=='A' (digit/letter seam, golden :8)");

    // golden :9
    //   const unsigned char T_ASXII2HEX[23]=
    //       {0,1,2,3,4,5,6,7,8,9, 0,0,0,0,0,0,0, 10,11,12,13,14,15};
    static const unsigned char kAscii2Hex[23] =
        {0,1,2,3,4,5,6,7,8,9, 0,0,0,0,0,0,0, 10,11,12,13,14,15};
    bad = 0;
    for (i = 0; i < 23; ++i) {
        if (T_ASXII2HEX[i] != kAscii2Hex[i]) {
            std::printf("    T_ASXII2HEX[%d]: golden %u, port %u\n",
                        i, (unsigned)kAscii2Hex[i], (unsigned)T_ASXII2HEX[i]);
            ++bad;
        }
    }
    CHECK(bad == 0, "T_ASXII2HEX: all 23 elements match golden :9 exactly");
    CHECK_INT(T_ASXII2HEX[0],   0, "T_ASXII2HEX[0]==0   (FIRST element, golden :9)");
    CHECK_INT(T_ASXII2HEX[22], 15, "T_ASXII2HEX[22]==15 (LAST element, golden :9)");
    CHECK_INT(T_ASXII2HEX[10],  0, "T_ASXII2HEX[10]==0  (start of the 7 punctuation pad slots, golden :9)");
    CHECK_INT(T_ASXII2HEX[16],  0, "T_ASXII2HEX[16]==0  (end of the 7 punctuation pad slots, golden :9)");
    CHECK_INT(T_ASXII2HEX[17], 10, "T_ASXII2HEX[17]==10 ('A' slot, golden :9)");
}

// ---------------------------------------------------------------------------
//  [2] T_HEX2ASCII_Mac / T_ASXII2HEX_Mac (golden :11-21)
// ---------------------------------------------------------------------------
static void test_mac_lookups()
{
    std::printf("\n[2] T_HEX2ASCII_Mac / T_ASXII2HEX_Mac -- both ends of range + out of range\n");

    // golden :11-14 -- return T_HEX2ASCII[hex2ascii & 0x0f].  The mask means there
    // IS no out-of-range input: everything wraps into the table.
    CHECK_INT(T_HEX2ASCII_Mac(0x00), '0', "T_HEX2ASCII_Mac(0x00)=='0'  (low end, golden :13)");
    CHECK_INT(T_HEX2ASCII_Mac(0x0F), 'F', "T_HEX2ASCII_Mac(0x0F)=='F'  (high end, golden :13)");
    CHECK_INT(T_HEX2ASCII_Mac(0x0A), 'A', "T_HEX2ASCII_Mac(0x0A)=='A'  (golden :13)");
    // "out of range" for this one means: masked, not rejected.
    CHECK_INT(T_HEX2ASCII_Mac(0x10), '0', "T_HEX2ASCII_Mac(0x10)=='0'  (&0x0f wraps, golden :13)");
    CHECK_INT(T_HEX2ASCII_Mac(0xFF), 'F', "T_HEX2ASCII_Mac(0xFF)=='F'  (&0x0f wraps, golden :13)");

    // golden :16-21
    //   if(ascii2hex-'0'>22 || ascii2hex-'0'<0) return 0;
    //   return T_ASXII2HEX[ascii2hex-'0'];
    // '0'==48 -> index 0 ; 'F'==70 -> index 22.
    CHECK_INT(T_ASXII2HEX_Mac('0'),  0, "T_ASXII2HEX_Mac('0')==0   (index 0,  LOW end of range, golden :20)");
    CHECK_INT(T_ASXII2HEX_Mac('9'),  9, "T_ASXII2HEX_Mac('9')==9   (index 9,  golden :20)");
    CHECK_INT(T_ASXII2HEX_Mac('A'), 10, "T_ASXII2HEX_Mac('A')==10  (index 17, golden :20)");
    CHECK_INT(T_ASXII2HEX_Mac('F'), 15, "T_ASXII2HEX_Mac('F')==15  (index 22, HIGH end of range, golden :20)");
    // The 7 padded punctuation slots ':' (58, index 10) .. '@' (64, index 16)
    // are IN range and read 0 out of the table -- golden quirk, not a reject.
    CHECK_INT(T_ASXII2HEX_Mac(':'), 0, "T_ASXII2HEX_Mac(':')==0  (in range, padded slot 10, golden :9/:20)");
    CHECK_INT(T_ASXII2HEX_Mac('@'), 0, "T_ASXII2HEX_Mac('@')==0  (in range, padded slot 16, golden :9/:20)");
    // Genuinely OUT of range, both directions (golden :18-19).
    CHECK_INT(T_ASXII2HEX_Mac('G'),  0, "T_ASXII2HEX_Mac('G')==0  ('G'-'0'==23 > 22, rejected, golden :18)");
    CHECK_INT(T_ASXII2HEX_Mac('/'),  0, "T_ASXII2HEX_Mac('/')==0  ('/'-'0'==-1 < 0,  rejected, golden :18)");
    CHECK_INT(T_ASXII2HEX_Mac(0x00), 0, "T_ASXII2HEX_Mac(0)==0    (0-'0'==-48 < 0,  rejected, golden :18)");
    CHECK_INT(T_ASXII2HEX_Mac('a'),  0, "T_ASXII2HEX_Mac('a')==0  (lower case rejected: 'a'-'0'==49 > 22, golden :18)");
}

// ---------------------------------------------------------------------------
//  [3] MyDeCodeASCII / MyASCIIToDec (golden :23-159 and :161-295)
// ---------------------------------------------------------------------------
static void test_ascii_decimal()
{
    std::printf("\n[3] MyDeCodeASCII / MyASCIIToDec -- decimal <-> ASCII mnemonic\n");

    CHECK_STR(MyDeCodeASCII(0),   "NUL", "MyDeCodeASCII(0)==\"NUL\"    (golden :28, FIRST case)");
    CHECK_STR(MyDeCodeASCII(3),   "ETX", "MyDeCodeASCII(3)==\"ETX\"    (golden :31)");
    CHECK_STR(MyDeCodeASCII(8),   "BS",  "MyDeCodeASCII(8)==\"BS\"     (golden :36 -- 2 chars, the trailing blank is alignment)");
    CHECK_STR(MyDeCodeASCII(32),  " ",   "MyDeCodeASCII(32)==\" \"     (golden :60)");
    CHECK_STR(MyDeCodeASCII(34),  "\"",  "MyDeCodeASCII(34)=='\"'      (golden :62, escaped literal)");
    CHECK_STR(MyDeCodeASCII(63),  "?",   "MyDeCodeASCII(63)==\"?\"     (golden :91, backslash-? in source)");
    CHECK_STR(MyDeCodeASCII(65),  "A",   "MyDeCodeASCII(65)==\"A\"     (golden :93)");
    CHECK_STR(MyDeCodeASCII(92),  "\\",  "MyDeCodeASCII(92)==backslash (golden :120, escaped literal)");
    CHECK_STR(MyDeCodeASCII(122), "z",   "MyDeCodeASCII(122)==\"z\"    (golden :150)");
    CHECK_STR(MyDeCodeASCII(127), "DEL", "MyDeCodeASCII(127)==\"DEL\"  (golden :155, LAST case)");
    // Both sides of the switch's default (golden :156).
    CHECK_STR(MyDeCodeASCII(128), "Err", "MyDeCodeASCII(128)==\"Err\"  (one past the LAST case, golden :156)");
    CHECK_STR(MyDeCodeASCII(-1),  "Err", "MyDeCodeASCII(-1)==\"Err\"   (below the FIRST case, golden :156)");

    CHECK_INT(MyASCIIToDec("NUL"),   0, "MyASCIIToDec(\"NUL\")==0    (golden :165, FIRST branch)");
    CHECK_INT(MyASCIIToDec("BS"),    8, "MyASCIIToDec(\"BS\")==8     (golden :173)");
    CHECK_INT(MyASCIIToDec(" "),    32, "MyASCIIToDec(\" \")==32     (golden :197)");
    CHECK_INT(MyASCIIToDec("A"),    65, "MyASCIIToDec(\"A\")==65     (golden :230)");
    CHECK_INT(MyASCIIToDec("z"),   122, "MyASCIIToDec(\"z\")==122    (golden :287)");
    CHECK_INT(MyASCIIToDec("DEL"), 127, "MyASCIIToDec(\"DEL\")==127  (golden :292, LAST branch)");
    // No match at all -> the initialiser survives (golden :163).
    CHECK_INT(MyASCIIToDec("XYZZY"), 0, "MyASCIIToDec(\"XYZZY\")==0  (no branch taken, initialiser wins, golden :163)");
    CHECK_INT(MyASCIIToDec(""),      0, "MyASCIIToDec(\"\")==0       (no branch taken, initialiser wins, golden :163)");

    // Full 0..127 round trip.  golden :28-155 enumerate every code 0..127 and
    // golden :165-292 enumerate the identical 128 spellings, so the composition
    // must be the identity over the whole range.
    int bad = 0, i;
    for (i = 0; i <= 127; ++i) {
        const AnsiString mnem = MyDeCodeASCII(i);
        const int back = (int)MyASCIIToDec(mnem);
        if (back != i) {
            std::printf("    round trip broken at %d: MyDeCodeASCII -> \"%s\" -> %d\n",
                        i, mnem.c_str(), back);
            ++bad;
        }
    }
    CHECK(bad == 0, "MyASCIIToDec(MyDeCodeASCII(i))==i for every i in 0..127 (golden :28-155 vs :165-292)");
}

// ---------------------------------------------------------------------------
//  [4] CountHexToByte (golden :297-321) -- the 1-BASED SubString loop
// ---------------------------------------------------------------------------
static void test_CountHexToByte()
{
    std::printf("\n[4] CountHexToByte -- hex text -> binary text, 1-based scan\n");

    // golden :300 `for(int iHexNum=1; iHexNum<=asHCode.Length(); iHexNum++)` and
    // :302-317 map each nibble.  An off-by-one at either end drops the first or
    // the last nibble, so both ends carry a distinct pattern here.
    CHECK_STR(CountHexToByte("0"), "0000", "CountHexToByte(\"0\")==\"0000\"  (golden :302)");
    CHECK_STR(CountHexToByte("F"), "1111", "CountHexToByte(\"F\")==\"1111\"  (golden :317)");
    CHECK_STR(CountHexToByte("1A"), "00011010",
              "CountHexToByte(\"1A\")==\"00011010\"  (golden :303 then :312)");
    // 4 nibbles, all different: catches a dropped FIRST (would give 000000001010)
    // and a dropped LAST (would give 111100000000).
    CHECK_STR(CountHexToByte("F00A"), "1111000000001010",
              "CountHexToByte(\"F00A\")==\"1111000000001010\"  (positions 1..Length, golden :300-317)");
    CHECK_STR(CountHexToByte(""), "", "CountHexToByte(\"\")==\"\"  (loop body never runs, golden :299-300)");
    // Unmatched characters append NOTHING (no else clause, golden :302-317).
    CHECK_STR(CountHexToByte("aF"), "1111",
              "CountHexToByte(\"aF\")==\"1111\"  (lower case 'a' matches no branch and is silently skipped, golden :302-317)");
    CHECK_STR(CountHexToByte("Fz"), "1111",
              "CountHexToByte(\"Fz\")==\"1111\"  (unmatched LAST char silently skipped, golden :302-317)");
}

// ---------------------------------------------------------------------------
//  [5] HexStrToInt(char*) (golden :323-345)
// ---------------------------------------------------------------------------
static void test_HexStrToInt_charptr()
{
    std::printf("\n[5] HexStrToInt(char*) -- reverse scan, break on first non-hex\n");

    // NEVER pass a string literal: golden :329 upper-cases the CALLER's buffer in
    // place, so a literal would be a write to read-only memory.  Writable buffers
    // only, exactly as golden's own call sites do.
    char buf[32];

    std::strcpy(buf, "");
    CHECK_INT(HexStrToInt(buf), 0, "HexStrToInt(\"\")==0  (early return, golden :327-328)");

    std::strcpy(buf, "0");
    CHECK_INT(HexStrToInt(buf), 0, "HexStrToInt(\"0\")==0  (single char, golden :332-343)");

    std::strcpy(buf, "F");
    CHECK_INT(HexStrToInt(buf), 15, "HexStrToInt(\"F\")==15  (single char, golden :337-338)");

    // "1a" -> upper-cased to "1A" (golden :329), then scanned RIGHT to LEFT:
    //   i=1 'A' -> 10*1  = 10 , Scale 16
    //   i=0 '1' ->  1*16 = 16 , sum 26
    std::strcpy(buf, "1a");
    CHECK_INT(HexStrToInt(buf), 26, "HexStrToInt(\"1a\")==26  (case folded then 0x1A, golden :329-343)");
    CHECK_STR(AnsiString(buf), "1A",
              "HexStrToInt upper-cases the CALLER's buffer in place: \"1a\" -> \"1A\"  (golden :329)");

    // "FF" -> i=1 15*1 + i=0 15*16 = 255
    std::strcpy(buf, "FF");
    CHECK_INT(HexStrToInt(buf), 255, "HexStrToInt(\"FF\")==255  (golden :332-343)");

    // "ffff" -> 65535
    std::strcpy(buf, "ffff");
    CHECK_INT(HexStrToInt(buf), 65535, "HexStrToInt(\"ffff\")==65535  (golden :329-343)");

    // Reverse scan + break: only the trailing hex run counts.  "G1" -> i=1 '1' =1,
    // then i=0 'G' matches neither range and BREAKS (golden :339-340) -> 1.
    std::strcpy(buf, "G1");
    CHECK_INT(HexStrToInt(buf), 1, "HexStrToInt(\"G1\")==1  (scan is right-to-left and BREAKS at 'G', golden :339-340)");

    // "0x1A" -> upper "0X1A": 'A'=10, '1'=16, then 'X' breaks -> 26.  The 0x
    // prefix is NOT understood by this overload; it merely terminates the scan.
    std::strcpy(buf, "0x1A");
    CHECK_INT(HexStrToInt(buf), 26, "HexStrToInt(\"0x1A\")==26  ('X' breaks the scan, prefix ignored, golden :339-340)");

    // A single non-hex char accumulates nothing.
    std::strcpy(buf, "Z");
    CHECK_INT(HexStrToInt(buf), 0, "HexStrToInt(\"Z\")==0  (break before any accumulation, golden :339-340)");
}

// ---------------------------------------------------------------------------
//  [6] SetBCC (golden :358-371)
// ---------------------------------------------------------------------------
static void test_SetBCC()
{
    std::printf("\n[6] SetBCC -- XOR of every byte, then ^ETX(0x03)\n");

    // golden :362 seeds BCC with ASCII[0] and :364 starts the loop at i=1, so an
    // EMPTY string reads the NUL terminator as the seed: BCC = 0 ^ 0x03 = 3.
    CHECK_INT(SetBCC(""), 3, "SetBCC(\"\")==3  (seed is the NUL terminator, then ^QETX, golden :361-368)");

    // Single character: the loop body never runs.  'A'(0x41) ^ 0x03 = 0x42 = 66.
    CHECK_INT(SetBCC("A"), 66, "SetBCC(\"A\")==66 (0x41^0x03)  (loop never runs, golden :362-368)");

    // "ABC": 0x41^0x42 = 0x03 ; 0x03^0x43 = 0x40 ; 0x40^0x03 = 0x43 = 67.
    CHECK_INT(SetBCC("ABC"), 67, "SetBCC(\"ABC\")==67 (0x41^0x42^0x43^0x03=0x43)  (golden :362-368)");

    // A real frame head, computed by hand:
    //   '@'=0x40 ^ '0'=0x30 -> 0x70
    //   0x70    ^ '0'=0x30 -> 0x40
    //   0x40    ^ 'R'=0x52 -> 0x12
    //   0x12    ^ 'D'=0x44 -> 0x56
    //   0x56    ^ QETX 0x03 -> 0x55 = 85
    CHECK_INT(SetBCC("@00RD"), 85, "SetBCC(\"@00RD\")==85 (0x55, hand-computed)  (golden :362-368)");

    // Any byte XORed with itself cancels: "AA" -> 0x41^0x41=0 -> ^3 = 3.
    CHECK_INT(SetBCC("AA"), 3, "SetBCC(\"AA\")==3  (XOR self-cancels, golden :362-368)");
}

// ---------------------------------------------------------------------------
//  [7] CountByteToInt / CountByteToHex (golden :373-392, :394-413)
// ---------------------------------------------------------------------------
static void test_byte_tables()
{
    std::printf("\n[7] CountByteToInt / CountByteToHex -- 4-bit binary text lookup\n");

    CHECK_INT(CountByteToInt("0000"),  0, "CountByteToInt(\"0000\")==0   (golden :375, FIRST)");
    CHECK_INT(CountByteToInt("0001"),  1, "CountByteToInt(\"0001\")==1   (golden :376)");
    CHECK_INT(CountByteToInt("1010"), 10, "CountByteToInt(\"1010\")==10  (golden :385)");
    CHECK_INT(CountByteToInt("1111"), 15, "CountByteToInt(\"1111\")==15  (golden :390, LAST)");
    CHECK_INT(CountByteToInt(""),      0, "CountByteToInt(\"\")==0       (fallthrough, golden :391)");
    CHECK_INT(CountByteToInt("2222"),  0, "CountByteToInt(\"2222\")==0   (fallthrough, golden :391)");
    // Only EXACTLY 4 characters match; a 3- or 5-char string falls through.
    CHECK_INT(CountByteToInt("111"),   0, "CountByteToInt(\"111\")==0    (no 3-char key exists, golden :391)");
    CHECK_INT(CountByteToInt("11110"), 0, "CountByteToInt(\"11110\")==0  (no 5-char key exists, golden :391)");

    CHECK_STR(CountByteToHex("0000"), "0", "CountByteToHex(\"0000\")==\"0\"  (golden :396, FIRST)");
    CHECK_STR(CountByteToHex("1001"), "9", "CountByteToHex(\"1001\")==\"9\"  (golden :405)");
    CHECK_STR(CountByteToHex("1010"), "A", "CountByteToHex(\"1010\")==\"A\"  (golden :406, digit/letter seam)");
    CHECK_STR(CountByteToHex("1111"), "F", "CountByteToHex(\"1111\")==\"F\"  (golden :411, LAST)");
    CHECK_STR(CountByteToHex(""),     "0", "CountByteToHex(\"\")==\"0\"      (fallthrough, golden :412)");
    CHECK_STR(CountByteToHex("zzzz"), "0", "CountByteToHex(\"zzzz\")==\"0\"  (fallthrough, golden :412)");
}

// ---------------------------------------------------------------------------
//  [8] ConvertByte / Convert2Byte (golden :415-437, :439-461)
// ---------------------------------------------------------------------------
static void test_convert_byte()
{
    std::printf("\n[8] ConvertByte (4 nibbles) / Convert2Byte (2 nibbles)\n");

    // golden :420-429.  Per char: c>64 -> c-7 ; then -48 ; times pow(16,3-i).
    // Only inputs of at most 4 chars are meaningful (i is unsigned, so 3-i wraps)
    // -- longer inputs are golden UB and are deliberately NOT asserted.
    char buf[16];

    std::strcpy(buf, "");
    CHECK_INT(ConvertByte(buf), 0, "ConvertByte(\"\")==0  (loop never runs, golden :417-420)");

    std::strcpy(buf, "0");
    CHECK_INT(ConvertByte(buf), 0, "ConvertByte(\"0\")==0  ((48-48)*16^3, golden :422-428)");

    // "1"  -> (49-48)*16^3 = 4096.  NOTE: the exponent is fixed by POSITION, not
    // by length, so a 1-char input is scaled as if it were the first of four.
    std::strcpy(buf, "1");
    CHECK_INT(ConvertByte(buf), 4096, "ConvertByte(\"1\")==4096  (exponent is 3-i, not length-based, golden :428)");

    // "1A2B": 1*4096 + 10*256 + 2*16 + 11 = 4096+2560+32+11 = 6699 (=0x1A2B)
    std::strcpy(buf, "1A2B");
    CHECK_INT(ConvertByte(buf), 6699, "ConvertByte(\"1A2B\")==6699 (0x1A2B)  (golden :422-428)");

    // "FFFF": 15*(4096+256+16+1) = 15*4369 = 65535
    std::strcpy(buf, "FFFF");
    CHECK_INT(ConvertByte(buf), 65535, "ConvertByte(\"FFFF\")==65535  (golden :422-428)");

    std::strcpy(buf, "0000");
    CHECK_INT(ConvertByte(buf), 0, "ConvertByte(\"0000\")==0  (golden :422-428)");

    // golden :444-453, same shape with pow(16,1-i): meaningful up to 2 chars.
    std::strcpy(buf, "");
    CHECK_INT(Convert2Byte(buf), 0, "Convert2Byte(\"\")==0  (loop never runs, golden :441-444)");

    std::strcpy(buf, "0");
    CHECK_INT(Convert2Byte(buf), 0, "Convert2Byte(\"0\")==0  (golden :446-452)");

    // "8" -> (56-48)*16 = 128
    std::strcpy(buf, "8");
    CHECK_INT(Convert2Byte(buf), 128, "Convert2Byte(\"8\")==128  (exponent is 1-i, golden :452)");

    // "1A" -> 1*16 + 10 = 26
    std::strcpy(buf, "1A");
    CHECK_INT(Convert2Byte(buf), 26, "Convert2Byte(\"1A\")==26 (0x1A)  (golden :446-452)");

    // "FF" -> 15*16 + 15 = 255
    std::strcpy(buf, "FF");
    CHECK_INT(Convert2Byte(buf), 255, "Convert2Byte(\"FF\")==255  (golden :446-452)");

    std::strcpy(buf, "00");
    CHECK_INT(Convert2Byte(buf), 0, "Convert2Byte(\"00\")==0  (golden :446-452)");
}

// ---------------------------------------------------------------------------
//  [9] float2hex (golden :463-525) -- every typecode x every dataformat branch
// ---------------------------------------------------------------------------
static void test_float2hex()
{
    std::printf("\n[9] float2hex -- Omron analogue scaling, all 5 typecodes x 2 dataformats\n");

    // ---- dataformat==0 : the per-typecode scaling switch (golden :504-521) ----

    // 0x30 : clamp [0,20] (golden :469-474), then value*65535/20 (golden :506-508)
    CHECK_INT(float2hex(0x30,  0.0, 0),     0, "float2hex(0x30, 0,   0)==0      (0*65535/20, golden :507)");
    CHECK_INT(float2hex(0x30, 20.0, 0), 65535, "float2hex(0x30, 20,  0)==65535  (20*65535/20, golden :507)");
    // 10*65535/20 = 32767.5 -> truncated on the double->unsigned assignment
    CHECK_INT(float2hex(0x30, 10.0, 0), 32767, "float2hex(0x30, 10,  0)==32767  (32767.5 truncates, golden :507)");
    CHECK_INT(float2hex(0x30, 25.0, 0), 65535, "float2hex(0x30, 25,  0)==65535  (clamped to 20 first, golden :470-471)");
    CHECK_INT(float2hex(0x30, -5.0, 0),     0, "float2hex(0x30, -5,  0)==0      (clamped to 0 first, golden :472-473)");

    // 0x31 : clamp [4,20] (golden :475-480), then (value-4)*65535/16 (golden :509-511)
    CHECK_INT(float2hex(0x31,  4.0, 0),     0, "float2hex(0x31, 4,   0)==0      ((4-4)*65535/16, golden :510)");
    CHECK_INT(float2hex(0x31, 20.0, 0), 65535, "float2hex(0x31, 20,  0)==65535  ((20-4)*65535/16, golden :510)");
    // (12-4)*65535/16 = 8*65535/16 = 32767.5 -> 32767
    CHECK_INT(float2hex(0x31, 12.0, 0), 32767, "float2hex(0x31, 12,  0)==32767  (32767.5 truncates, golden :510)");
    CHECK_INT(float2hex(0x31,  2.0, 0),     0, "float2hex(0x31, 2,   0)==0      (clamped UP to 4 first, golden :478-479)");
    CHECK_INT(float2hex(0x31, 99.0, 0), 65535, "float2hex(0x31, 99,  0)==65535  (clamped to 20 first, golden :476-477)");

    // 0x33 : clamp [-10,10] (golden :481-486), then value*32767/10 (golden :512-514)
    CHECK_INT(float2hex(0x33,  0.0, 0),     0, "float2hex(0x33, 0,   0)==0      (golden :513)");
    CHECK_INT(float2hex(0x33, 10.0, 0), 32767, "float2hex(0x33, 10,  0)==32767  (10*32767/10, golden :513)");
    // 5*32767/10 = 16383.5 -> 16383
    CHECK_INT(float2hex(0x33,  5.0, 0), 16383, "float2hex(0x33, 5,   0)==16383  (16383.5 truncates, golden :513)");
    CHECK_INT(float2hex(0x33, 50.0, 0), 32767, "float2hex(0x33, 50,  0)==32767  (clamped to 10 first, golden :482-483)");

    // 0x35 : clamp [-5,5] (golden :487-492), then value*32767/5 (golden :515-517)
    CHECK_INT(float2hex(0x35,  0.0, 0),     0, "float2hex(0x35, 0,   0)==0      (golden :516)");
    CHECK_INT(float2hex(0x35,  5.0, 0), 32767, "float2hex(0x35, 5,   0)==32767  (5*32767/5, golden :516)");
    // 2.5*32767/5 = 16383.5 -> 16383
    CHECK_INT(float2hex(0x35,  2.5, 0), 16383, "float2hex(0x35, 2.5, 0)==16383  (16383.5 truncates, golden :516)");
    CHECK_INT(float2hex(0x35, 99.0, 0), 32767, "float2hex(0x35, 99,  0)==32767  (clamped to 5 first, golden :488-489)");

    // default typecode : value forced to 0 (golden :493-495) AND hexValue forced
    // to 0 (golden :518-520).  0x32 and 0x34 sit BETWEEN the supported codes, so
    // they are the sharpest probes of the switch.
    CHECK_INT(float2hex(0x32, 7.0, 0), 0, "float2hex(0x32, 7, 0)==0  (unsupported typecode, golden :493-495/:518-520)");
    CHECK_INT(float2hex(0x34, 7.0, 0), 0, "float2hex(0x34, 7, 0)==0  (unsupported typecode, golden :493-495/:518-520)");
    CHECK_INT(float2hex(0x00, 7.0, 0), 0, "float2hex(0x00, 7, 0)==0  (unsupported typecode, golden :493-495/:518-520)");

    // ---- dataformat!=0 : value*1000 for EVERY typecode (golden :498-501) ------
    // The clamp still runs first (it is outside the dataformat test).
    CHECK_INT(float2hex(0x30, 12.5, 1), 12500, "float2hex(0x30, 12.5, 1)==12500  (value*1000, golden :500)");
    CHECK_INT(float2hex(0x30, 25.0, 1), 20000, "float2hex(0x30, 25,   1)==20000  (clamped to 20, THEN *1000, golden :470-471/:500)");
    CHECK_INT(float2hex(0x31,  3.0, 1),  4000, "float2hex(0x31, 3,    1)==4000   (clamped UP to 4, THEN *1000, golden :478-479/:500)");
    CHECK_INT(float2hex(0x31, 20.0, 1), 20000, "float2hex(0x31, 20,   1)==20000  (value*1000, golden :500)");
    CHECK_INT(float2hex(0x33, 10.0, 1), 10000, "float2hex(0x33, 10,   1)==10000  (value*1000, golden :500)");
    CHECK_INT(float2hex(0x35, 99.0, 1),  5000, "float2hex(0x35, 99,   1)==5000   (clamped to 5, THEN *1000, golden :488-489/:500)");
    CHECK_INT(float2hex(0x35,  0.0, 1),     0, "float2hex(0x35, 0,    1)==0      (value*1000, golden :500)");
    // default typecode with dataformat!=0 goes through :494 (value=0) and then
    // :500 (0*1000) -- NOT through the :518-520 default.
    CHECK_INT(float2hex(0x32, 7.0, 1), 0, "float2hex(0x32, 7, 1)==0  (value zeroed at :494, then *1000 at :500)");
    // dataformat is tested for truthiness, not equality to 1 (golden :498).
    CHECK_INT(float2hex(0x33, 10.0,  2), 10000, "float2hex(0x33, 10, 2)==10000   (dataformat is a truth test, golden :498)");
    CHECK_INT(float2hex(0x33, 10.0, -1), 10000, "float2hex(0x33, 10, -1)==10000  (dataformat is a truth test, golden :498)");
    // NOT TESTED ON PURPOSE: a negative value with dataformat!=0 (e.g. 0x33,-10,1)
    // assigns a negative double to `unsigned`, which is undefined behaviour in
    // both toolchains.  Golden has the identical hazard; asserting a value there
    // would be asserting UB, not golden.
}

// ---------------------------------------------------------------------------
//  [10] StringConvert2Hex (golden :527-561)
// ---------------------------------------------------------------------------
static void test_StringConvert2Hex()
{
    std::printf("\n[10] StringConvert2Hex -- two hex digits + blank per byte (S1 is dead)\n");

    // golden :557 `sprintf(s, "%02X ", Target[i]); S2+=s;` and :560 `return S2;`
    // -- the '<' '=' '>' substitution at :537-548 only ever touches S1, which is
    // DEAD.  So control bytes must still come out as plain hex here.
    CHECK_STR(StringConvert2Hex(""), "", "StringConvert2Hex(\"\")==\"\"  (len 0, loop never runs, golden :534-536)");
    CHECK_STR(StringConvert2Hex("A"), "41 ", "StringConvert2Hex(\"A\")==\"41 \"  (single byte, trailing blank, golden :557)");
    CHECK_STR(StringConvert2Hex("AB"), "41 42 ", "StringConvert2Hex(\"AB\")==\"41 42 \"  (golden :557)");
    CHECK_STR(StringConvert2Hex("0"), "30 ", "StringConvert2Hex(\"0\")==\"30 \"  (golden :557)");
    // STX .. ETX frame: the first and last byte are exactly the control codes
    // whose S1 branch is dead, so they prove the function returns S2, not S1.
    CHECK_STR(StringConvert2Hex(AnsiString("\x02") + "OK" + AnsiString("\x03")),
              "02 4F 4B 03 ",
              "StringConvert2Hex(STX OK ETX)==\"02 4F 4B 03 \"  (S2 returned, S1 dead, golden :537-560)");
    CHECK_STR(StringConvert2Hex("~"), "7E ", "StringConvert2Hex(\"~\")==\"7E \"  (high printable, golden :557)");
}

// ---------------------------------------------------------------------------
//  [11] A_Create_LCR (golden :563-580) and A_Create_LRC (golden :582-593)
// ---------------------------------------------------------------------------
static void test_checksums()
{
    std::printf("\n[11] A_Create_LCR (ASCII-pair LRC) / A_Create_LRC (byte LRC)\n");

    // A_Create_LCR consumes TWO ASCII chars per iteration and decrements `length`
    // TWICE per iteration (golden :574 `--length` plus :578 `while(--length)`),
    // with the :575-576 break that golden's own ChungHung 20121030 comment says
    // stops the infinite loop.  Sum the decoded bytes, then two's-complement.

    // "0102", length 4:
    //   iter1: '0','1' -> 0x01 ; Btmp1=0x01 ; length 4->3, then ->2 (loop again)
    //   iter2: '0','2' -> 0x02 ; Btmp1=0x03 ; length 2->1, then ->0 (exit)
    //   return (~3)+1 = 256-3 = 253
    {
        unsigned char frame[] = "0102";
        CHECK_INT(A_Create_LCR(frame, 4), 253,
                  "A_Create_LCR(\"0102\",4)==253  (0x01+0x02=3, two's complement, golden :567-579)");
    }

    // "0A", length 2 -- the SHORTEST well-formed input:
    //   iter1: '0','A' -> 0x0A ; Btmp1=0x0A ; length 2->1, then ->0 (exit)
    //   return 256-10 = 246
    {
        unsigned char frame[] = "0A";
        CHECK_INT(A_Create_LCR(frame, 2), 246,
                  "A_Create_LCR(\"0A\",2)==246  (single pair, golden :567-579)");
    }

    // "FF01", length 4 -- Btmp1 wraps to 0 inside the unsigned char:
    //   0xFF + 0x01 = 0x00 ; return (~0)+1 = 0
    {
        unsigned char frame[] = "FF01";
        CHECK_INT(A_Create_LCR(frame, 4), 0,
                  "A_Create_LCR(\"FF01\",4)==0  (0xFF+0x01 wraps to 0, golden :572/:579)");
    }

    // ODD length quirk: length 3 consumes the SAME two pairs as length 4, because
    // the body decrement takes 3->2 (not zero, so no break) and the while
    // decrement takes 2->1 (still truthy, so a second pass runs).
    //   iter1: '0','1' -> 0x01 ; length 3->2 ; while: ->1 (loop)
    //   iter2: '0','2' -> 0x02 ; length 1->0 ; break (golden :575-576)
    //   return 256-3 = 253 -- identical to length 4.
    {
        unsigned char frame[] = "0102";
        CHECK_INT(A_Create_LCR(frame, 3), 253,
                  "A_Create_LCR(\"0102\",3)==253  (odd length behaves as 4: golden's double decrement, :574-578)");
    }

    // length 1: ONE pair is still consumed (the do-while body runs before any
    // test), then :574 takes length to 0 and :575-576 breaks.
    //   '4','1' -> 0x41 ; return 256-0x41 = 191
    {
        unsigned char frame[] = "41";
        CHECK_INT(A_Create_LCR(frame, 1), 191,
                  "A_Create_LCR(\"41\",1)==191  (do-while body always runs once, golden :567-576)");
    }

    // Non-hex characters decode to 0 through T_ASXII2HEX_Mac (golden :569/:571):
    // "zz" -> 0x00 ; return 0.
    {
        unsigned char frame[] = "zz";
        CHECK_INT(A_Create_LCR(frame, 2), 0,
                  "A_Create_LCR(\"zz\",2)==0  (both nibbles reject to 0, golden :18-19/:569-571)");
    }

    // ---- A_Create_LRC: plain byte sum then two's complement (golden :584-592) --

    // Standard Modbus-ASCII payload 01 03 00 00 00 02 -> sum 6 -> 0xFA = 250
    {
        unsigned char msg[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02};
        CHECK_INT(A_Create_LRC(msg, 6), 250,
                  "A_Create_LRC({01,03,00,00,00,02},6)==250  (sum 6, (0xff-6)+1, golden :586-590)");
    }

    // Single byte -- the shortest meaningful frame.  0x41 -> 256-65 = 191.
    {
        unsigned char msg[] = {0x41};
        CHECK_INT(A_Create_LRC(msg, 1), 191, "A_Create_LRC({0x41},1)==191  (golden :586-590)");
    }

    // EMPTY: the for loop never runs (golden :586), LRC stays 0, and
    // (0xff-0)+1 == 0x100 truncates to 0 in the unsigned char assignment.
    {
        unsigned char msg[] = {0x00};
        CHECK_INT(A_Create_LRC(msg, 0), 0,
                  "A_Create_LRC(anything,0)==0  ((0xff-0)+1 == 0x100 truncates, golden :590)");
    }

    // Sum wraps inside the unsigned char accumulator: 0xFF + 0x02 = 0x01,
    // so the result is 256-1 = 255, NOT 256-257.
    {
        unsigned char msg[] = {0xFF, 0x02};
        CHECK_INT(A_Create_LRC(msg, 2), 255,
                  "A_Create_LRC({0xFF,0x02},2)==255  (accumulator wraps at 8 bits, golden :588)");
    }

    // A frame whose bytes sum to exactly 0x100 gives LRC 0 as well.
    {
        unsigned char msg[] = {0x80, 0x80};
        CHECK_INT(A_Create_LRC(msg, 2), 0, "A_Create_LRC({0x80,0x80},2)==0  (sum wraps to 0, golden :588-590)");
    }
}

// ---------------------------------------------------------------------------
//  [12] MyFormatFloat (golden :633-671)
// ---------------------------------------------------------------------------
static void test_MyFormatFloat()
{
    std::printf("\n[12] MyFormatFloat -- round to iDigi decimals via sprintf+atof\n");

    // golden :636-639 : iDigi==0 -> "%0.0f"
    CHECK_DBL(MyFormatFloat(3.14159, 0), 3.0,  "MyFormatFloat(3.14159, 0)==3      (0 dp, golden :638)");
    CHECK_DBL(MyFormatFloat(3.99,    0), 4.0,  "MyFormatFloat(3.99, 0)==4         (0 dp rounds up, golden :638)");
    // golden :640-643 : iDigi==1
    CHECK_DBL(MyFormatFloat(3.14159, 1), 3.1,  "MyFormatFloat(3.14159, 1)==3.1    (1 dp, golden :642)");
    // golden :644-647 : iDigi==2
    CHECK_DBL(MyFormatFloat(3.14159, 2), 3.14, "MyFormatFloat(3.14159, 2)==3.14   (2 dp, golden :646)");
    CHECK_DBL(MyFormatFloat(-12.3456, 2), -12.35, "MyFormatFloat(-12.3456, 2)==-12.35  (negatives round away from 0, golden :646)");
    // golden :648-651 : iDigi==3
    CHECK_DBL(MyFormatFloat(3.14159, 3), 3.142, "MyFormatFloat(3.14159, 3)==3.142  (3 dp, golden :650)");
    // golden :652-655 : iDigi==4
    CHECK_DBL(MyFormatFloat(123.456789, 4), 123.4568, "MyFormatFloat(123.456789, 4)==123.4568  (4 dp, golden :654)");
    // golden :656-659 : iDigi==5
    CHECK_DBL(MyFormatFloat(1.234567891, 5), 1.23457, "MyFormatFloat(1.234567891, 5)==1.23457  (5 dp, golden :658)");
    // golden :660-663 : iDigi==6
    CHECK_DBL(MyFormatFloat(1.234567891, 6), 1.234568, "MyFormatFloat(1.234567891, 6)==1.234568  (6 dp, golden :662)");
    // golden :664-667 : EVERYTHING else -> 7 dp.  Both directions out of range.
    CHECK_DBL(MyFormatFloat(1.234567891, 7),  1.2345679, "MyFormatFloat(1.234567891, 7)==1.2345679   (else branch, golden :666)");
    CHECK_DBL(MyFormatFloat(1.234567891, 99), 1.2345679, "MyFormatFloat(1.234567891, 99)==1.2345679  (digits>6 fall to 7 dp, golden :664-666)");
    CHECK_DBL(MyFormatFloat(1.234567891, -1), 1.2345679, "MyFormatFloat(1.234567891, -1)==1.2345679  (NEGATIVE digits also fall to 7 dp, golden :664-666)");
    // Exact values must survive untouched.
    CHECK_DBL(MyFormatFloat(0.0, 2), 0.0, "MyFormatFloat(0, 2)==0  (golden :646/:669)");
    CHECK_DBL(MyFormatFloat(1.0, 3), 1.0, "MyFormatFloat(1, 3)==1  (golden :650/:669)");
    CHECK_DBL(MyFormatFloat(-1.5, 1), -1.5, "MyFormatFloat(-1.5, 1)==-1.5  (already exact at 1 dp, golden :642)");
}

// ---------------------------------------------------------------------------
//  main
// ---------------------------------------------------------------------------
int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);
    std::printf("=================================================================\n");
    std::printf("  PT-W1 -- EJ1N/TextProcess.cpp golden-oracle test\n");
    std::printf("  oracle: golden EJ1N/TextProcess.cpp (674 lines, Big5/cp950)\n");
    std::printf("=================================================================\n");

    test_tables();
    test_mac_lookups();
    test_ascii_decimal();
    test_CountHexToByte();
    test_HexStrToInt_charptr();
    test_SetBCC();
    test_byte_tables();
    test_convert_byte();
    test_float2hex();
    test_StringConvert2Hex();
    test_checksums();
    test_MyFormatFloat();

    std::printf("\n-----------------------------------------------------------------\n");
    std::printf("  test_ptw1_textprocess: %d passed, %d failed\n", g_pass, g_fail);
    std::printf("  RESULT: %s\n", g_fail == 0 ? "PASS" : "FAIL");
    std::printf("-----------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
