// =============================================================================
//  TextProcess.cpp  --  EJ1N text / protocol conversion helpers.
//
//  Faithful translation of golden EJ1N/TextProcess.cpp (674 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-GA-EJ1N) 20260807
//  Translation wave: W906-GA-EJ1N (pure-translation completion campaign).  Sibling
//  of nothing yet -- this is the FIRST file of golden EJ1N/ to land in the port.
//
//  ROLE: the leaf conversion library for every serial temperature-controller /
//  fieldbus text protocol in the machine.  Owns NO state machine and NO cursor.
//  Entry points, all free functions:
//    * ASCII <-> decimal ...... MyDeCodeASCII (129-case switch), MyASCIIToDec
//    * hex <-> binary text .... CountHexToByte, CountByteToInt, CountByteToHex
//    * hex text -> int ........ HexStrToInt(char*)   [AnsiString overload gated]
//    * nibble tables .......... T_HEX2ASCII[16], T_ASXII2HEX[23],
//                               T_HEX2ASCII_Mac, T_ASXII2HEX_Mac
//    * frame checksums ........ SetBCC (XOR + ETX), A_Create_LCR, A_Create_LRC
//    * ModBus-ish field parse . ConvertByte (4 nibbles), Convert2Byte (2 nibbles)
//    * analogue scaling ....... float2hex (Omron typecode 0x30/0x31/0x33/0x35)
//    * misc ................... StringConvert2Hex, GetCOMPortStatus, MyFormatFloat
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim bodies):  16 of golden`s 17 functions, plus both
//     const tables T_HEX2ASCII / T_ASXII2HEX.  Nothing else in this file.
//   SATISFIED-BY-SHIM (so it LINKS):  MyDBIProcess(AnsiString,AnsiString) -- the
//     only outbound call in the whole unit (golden :434 and :458, both inside a
//     catch(...)).  Real definition: aHotPlateSubstrate.cpp:1030, declared
//     aHotPlateSubstrate.h:924.  That is this tree`s standing substitution for
//     golden cMyDB.h`s MyDBIProcess (same substitution recorded at
//     ainarm_SearchPickPlate.cpp:52 and atester_ProcessCount.cpp:111).  NOTE the
//     arity difference: golden cMyDB.h:20 declares MyDBIProcess as 3-arg with a
//     defaulted third; the port`s is 2-arg.  Both golden call sites in THIS file
//     pass exactly 2 arguments, so the call is unchanged.
//     MEASURED: with g++ 6.3 the compiled object does NOT reference MyDBIProcess
//     at all (`nm -g -u` on the .o lists vclcompat + libstdc++ + CRT +
//     CreateFileA/CloseHandle and nothing else).  Both catch(...) handlers are
//     provably unreachable -- everything inside the try (strlen, pow) is declared
//     nothrow -- so GCC drops the landing pad.  The include is therefore needed
//     to COMPILE, not to LINK; this TU adds ZERO cross-archive link edges today.
//     A compiler that keeps the handler (MSVC may) would make MyDBIProcess an
//     undefined ref into ht9045_sm, resolved by the exe-level LINK_GROUP:RESCAN.
//   GATE (#if 0): 1 -- registered below.
//
//  GATE REGISTER
//   [1] int HexStrToInt(AnsiString str)                golden :347-356
//       WHY IT CANNOT LAND: already in the port as
//       vclcompat::HexStrToInt(const AnsiString&) (vclcompat/SysUtils.cpp:81-101,
//       whose own comment cites golden :347 as its source), made global by
//       `using vclcompat::HexStrToInt;` at vclcompat/vcl_compat.h:271.  Landing
//       golden`s by-value overload as well is not a duplicate symbol but an
//       AMBIGUOUS-CALL break at every existing AnsiString call site (by-value
//       copy-init and const-ref binding rank identically): database.cpp:2509,
//       EtherCAT/MyNUEC1.cpp:175, MyLaneIo.cpp:723, tests/test_vclcompat.cpp:341
//       -353.  DEFAULT IN FORCE: the vclcompat function, same net behaviour, so
//       NO observable behaviour delta.  The golden body is kept VERBATIM inside
//       the #if 0 so the retirement wave can lift it out unchanged.
//       Re-cited at its own call site below.
//
//  UNGATED ON PURPOSE (checked, does link -- "gate only what does not LINK"):
//   * MyDBIProcess       -> aHotPlateSubstrate.cpp:1030 (real body).
//   * strlen/strcpy/pow/sprintf/atof/toupper -> MachineDefine.h`s portable
//     <string.h>/<math.h>/<stdio.h>/<stdlib.h> block.
//   * CreateFile/CloseHandle/HANDLE/INVALID_HANDLE_VALUE/GENERIC_*/OPEN_EXISTING/
//     FILE_ATTRIBUTE_NORMAL/FILE_FLAG_OVERLAPPED -> <windows.h> via
//     vclcompat/vcl_compat.h (which deliberately does NOT set
//     WIN32_LEAN_AND_MEAN, vcl_compat.h:185-187).  `CreateFile` expands to
//     CreateFileA; vcl_compat.h undefines DeleteFile/CopyFile/MoveFile/GetObject
//     but NOT CreateFile, so golden`s `::CreateFile(...)` is untouched.
//   * T_HEX2ASCII / T_ASXII2HEX / T_HEX2ASCII_Mac / A_Create_LCR are re-declared
//     by CCLink/MyCCLink.cpp:80-83 and used by cpublic.cpp:307-346 with matching
//     prototypes; this file is the definition home they were both waiting for.
//
//  MINIMAL NECESSARY ADAPTATIONS (2, both flagged inline with //AI at the line):
//   (a) golden :329 `strcpy(str, strupr(str));` -- `strupr` is a Microsoft /
//       OLDNAMES extension that MinGW`s <string.h> hides under -std=c++17
//       (__STRICT_ANSI__ suppresses the `#ifndef _NO_OLDNAMES` block at
//       C:/MinGW/include/string.h:130-146), and this project sets
//       CMAKE_CXX_EXTENSIONS OFF (CMakeLists.txt:6).  Verified by an isolated
//       `g++ -std=c++17 -fsyntax-only` probe: "strupr was not declared".
//       Replaced by the equivalent in-place ASCII upper-case loop.  Byte-
//       identical result; the golden self-copy strcpy(str,str) goes with it.
//   (b) golden :361 `char *ASCII=SendText.c_str();` -- BCB6 AnsiString::c_str()
//       returns `char*`; this port`s returns `const char*` (AnsiString.h:92).
//       Declared `const char *ASCII`.  The pointer is only read, so no delta.
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init) dropped
//  (golden :2 and :5).  AnsiString (1-based) + AnsiString::sprintf via vclcompat,
//  verbatim.  NO __fastcall / __property / __published in this .cpp.
//  SOFT_SIMULTE is NOT defined and golden does not reference it in this unit.
//  Numeric semantics kept EXACT (no int/float rewrites, no unsigned->signed
//  "fixes" -- see the GOLDEN QUIRKS list).
//
//  GOLDEN QUIRKS / BUGS preserved VERBATIM, not fixed:
//   1. ConvertByte (:428) and Convert2Byte (:452) compute `pow(16, 3-i)` /
//      `pow(16, 1-i)` with `unsigned int i`, so `3-i` and `1-i` are UNSIGNED
//      subtractions that wrap to ~4e9 as soon as i exceeds 3 / 1 -- pow() then
//      returns +inf and the `iRes+=` narrows inf to int.  The functions are
//      therefore only meaningful for inputs of at most 4 / 2 characters.  Kept.
//   2. ConvertByte`s exception text is misspelled "ConverByte" and Convert2Byte`s
//      "Conver2Byte" (:434, :458).  Kept as-is -- log strings are behaviour.
//   3. StringConvert2Hex (:527) builds S1 char-by-char and then never uses it:
//      the function returns S2.  Dead work, kept.  It also `strncpy`s into a
//      10 KiB stack buffer with sizeof(Target) as the bound, so an input of
//      >=10240 chars leaves Target unterminated.  Kept.
//   4. A_Create_LCR (:563) decrements `length` TWICE per iteration (`--length`
//      in the body AND `while(--length)`), with an extra `if(length==0) break;`
//      that golden`s own ChungHung 20121030 comment says was added to stop the
//      infinite loop / dead thread the double decrement caused on odd lengths.
//      Reproduced exactly, comment and all.
//   5. GetCOMPortStatus (:595) retries the failed ::CreateFile with byte-for-byte
//      identical arguments (it rebuilds the same CN string first).  The retry can
//      only succeed on a race.  Kept.
//   6. T_ASXII2HEX_Mac (:16) guards the index with `>22 || <0` for a 23-element
//      table, i.e. it maps `0`..`F` and returns 0 for anything else, INCLUDING
//      the 7 punctuation slots `:`..`@` that the table pads with 0.  Kept.
//   7. HexStrToInt(char*) (:323) upper-cases the CALLER`S buffer in place, so
//      `HexStrToInt("literal")` writes to read-only memory and crashes.  Golden
//      has the same hazard and no such call site; neither does the port.  Kept.
//      See the note in EJ1N/TextProcess.h`s banner.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8, in its
//  original column.  Final gate: ZERO U+FFFD.
// =============================================================================

// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "TextProcess.h"   ; #pragma package(smart_init)
// =============================================================================

#include "MachineDefine.h"          // de-VCL`d include hub: vclcompat umbrella +
                                    //   <windows.h>/<string.h>/<stdio.h>/<stdlib.h>/
                                    //   <math.h> + `using namespace std;`

#include "EJ1N/TextProcess.h"       // this unit`s own declarations + swap16
#include "aHotPlateSubstrate.h"     // MyDBIProcess (golden cMyDB.h) -- real body at
                                    //   aHotPlateSubstrate.cpp:1030
#include <ctype.h>                  // toupper -- the strupr stand-in, see (a) above

//------------------------------------------------------------------------------
const unsigned char T_HEX2ASCII[16]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const unsigned char T_ASXII2HEX[23]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
//------------------------------------------------------------------------------
unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii)
{
    return(T_HEX2ASCII[(hex2ascii)&0x0f]);
}
//------------------------------------------------------------------------------
unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex)
{
    if(ascii2hex-'0'>22 || ascii2hex-'0'<0)
        return 0;
    return(T_ASXII2HEX[ascii2hex-'0']);
}
//------------------------------------------------------------------------------
AnsiString MyDeCodeASCII(int iInPut)
{
    AnsiString asReturnASCII="";
    switch (iInPut)
    {
        case   0 : {  asReturnASCII="NUL"; break;}
        case   1 : {  asReturnASCII="SOH"; break;}
        case   2 : {  asReturnASCII="STX"; break;}
        case   3 : {  asReturnASCII="ETX"; break;}
        case   4 : {  asReturnASCII="EOT"; break;}
        case   5 : {  asReturnASCII="ENQ"; break;}
        case   6 : {  asReturnASCII="ACK"; break;}
        case   7 : {  asReturnASCII="BEL"; break;}
        case   8 : {  asReturnASCII="BS" ; break;}
        case   9 : {  asReturnASCII="HT" ; break;}
        case  10 : {  asReturnASCII="LF" ; break;}
        case  11 : {  asReturnASCII="VT" ; break;}
        case  12 : {  asReturnASCII="FF" ; break;}
        case  13 : {  asReturnASCII="CR" ; break;}
        case  14 : {  asReturnASCII="SO" ; break;}
        case  15 : {  asReturnASCII="SI" ; break;}
        case  16 : {  asReturnASCII="DLE"; break;}
        case  17 : {  asReturnASCII="DC1"; break;}
        case  18 : {  asReturnASCII="DC2"; break;}
        case  19 : {  asReturnASCII="DC3"; break;}
        case  20 : {  asReturnASCII="DC4"; break;}
        case  21 : {  asReturnASCII="NAK"; break;}
        case  22 : {  asReturnASCII="SYN"; break;}
        case  23 : {  asReturnASCII="ETB"; break;}
        case  24 : {  asReturnASCII="CAN"; break;}
        case  25 : {  asReturnASCII="EM" ; break;}
        case  26 : {  asReturnASCII="SUB"; break;}
        case  27 : {  asReturnASCII="ESC"; break;}
        case  28 : {  asReturnASCII="FS" ; break;}
        case  29 : {  asReturnASCII="GS" ; break;}
        case  30 : {  asReturnASCII="RS" ; break;}
        case  31 : {  asReturnASCII="US" ; break;}
        case  32 : {  asReturnASCII=" "  ; break;}
        case  33 : {  asReturnASCII="!"  ; break;}
        case  34 : {  asReturnASCII="\"" ; break;}
        case  35 : {  asReturnASCII="#"  ; break;}
        case  36 : {  asReturnASCII="$"  ; break;}
        case  37 : {  asReturnASCII="%"  ; break;}
        case  38 : {  asReturnASCII="&"  ; break;}
        case  39 : {  asReturnASCII="\'" ; break;}
        case  40 : {  asReturnASCII="("  ; break;}
        case  41 : {  asReturnASCII=")"  ; break;}
        case  42 : {  asReturnASCII="*"  ; break;}
        case  43 : {  asReturnASCII="+"  ; break;}
        case  44 : {  asReturnASCII=","  ; break;}
        case  45 : {  asReturnASCII="-"  ; break;}
        case  46 : {  asReturnASCII="."  ; break;}
        case  47 : {  asReturnASCII="/"  ; break;}
        case  48 : {  asReturnASCII="0"  ; break;}
        case  49 : {  asReturnASCII="1"  ; break;}
        case  50 : {  asReturnASCII="2"  ; break;}
        case  51 : {  asReturnASCII="3"  ; break;}
        case  52 : {  asReturnASCII="4"  ; break;}
        case  53 : {  asReturnASCII="5"  ; break;}
        case  54 : {  asReturnASCII="6"  ; break;}
        case  55 : {  asReturnASCII="7"  ; break;}
        case  56 : {  asReturnASCII="8"  ; break;}
        case  57 : {  asReturnASCII="9"  ; break;}
        case  58 : {  asReturnASCII=":"  ; break;}
        case  59 : {  asReturnASCII=";"  ; break;}
        case  60 : {  asReturnASCII="<"  ; break;}
        case  61 : {  asReturnASCII="="  ; break;}
        case  62 : {  asReturnASCII=">"  ; break;}
        case  63 : {  asReturnASCII="\?" ; break;}
        case  64 : {  asReturnASCII="@"  ; break;}
        case  65 : {  asReturnASCII="A"  ; break;}
        case  66 : {  asReturnASCII="B"  ; break;}
        case  67 : {  asReturnASCII="C"  ; break;}
        case  68 : {  asReturnASCII="D"  ; break;}
        case  69 : {  asReturnASCII="E"  ; break;}
        case  70 : {  asReturnASCII="F"  ; break;}
        case  71 : {  asReturnASCII="G"  ; break;}
        case  72 : {  asReturnASCII="H"  ; break;}
        case  73 : {  asReturnASCII="I"  ; break;}
        case  74 : {  asReturnASCII="J"  ; break;}
        case  75 : {  asReturnASCII="K"  ; break;}
        case  76 : {  asReturnASCII="L"  ; break;}
        case  77 : {  asReturnASCII="M"  ; break;}
        case  78 : {  asReturnASCII="N"  ; break;}
        case  79 : {  asReturnASCII="O"  ; break;}
        case  80 : {  asReturnASCII="P"  ; break;}
        case  81 : {  asReturnASCII="Q"  ; break;}
        case  82 : {  asReturnASCII="R"  ; break;}
        case  83 : {  asReturnASCII="S"  ; break;}
        case  84 : {  asReturnASCII="T"  ; break;}
        case  85 : {  asReturnASCII="U"  ; break;}
        case  86 : {  asReturnASCII="V"  ; break;}
        case  87 : {  asReturnASCII="W"  ; break;}
        case  88 : {  asReturnASCII="X"  ; break;}
        case  89 : {  asReturnASCII="Y"  ; break;}
        case  90 : {  asReturnASCII="Z"  ; break;}
        case  91 : {  asReturnASCII="["  ; break;}
        case  92 : {  asReturnASCII="\\" ; break;}
        case  93 : {  asReturnASCII="]"  ; break;}
        case  94 : {  asReturnASCII="^"  ; break;}
        case  95 : {  asReturnASCII="_"  ; break;}
        case  96 : {  asReturnASCII="`"  ; break;}
        case  97 : {  asReturnASCII="a"  ; break;}
        case  98 : {  asReturnASCII="b"  ; break;}
        case  99 : {  asReturnASCII="c"  ; break;}
        case 100 : {  asReturnASCII="d"  ; break;}
        case 101 : {  asReturnASCII="e"  ; break;}
        case 102 : {  asReturnASCII="f"  ; break;}
        case 103 : {  asReturnASCII="g"  ; break;}
        case 104 : {  asReturnASCII="h"  ; break;}
        case 105 : {  asReturnASCII="i"  ; break;}
        case 106 : {  asReturnASCII="j"  ; break;}
        case 107 : {  asReturnASCII="k"  ; break;}
        case 108 : {  asReturnASCII="l"  ; break;}
        case 109 : {  asReturnASCII="m"  ; break;}
        case 110 : {  asReturnASCII="n"  ; break;}
        case 111 : {  asReturnASCII="o"  ; break;}
        case 112 : {  asReturnASCII="p"  ; break;}
        case 113 : {  asReturnASCII="q"  ; break;}
        case 114 : {  asReturnASCII="r"  ; break;}
        case 115 : {  asReturnASCII="s"  ; break;}
        case 116 : {  asReturnASCII="t"  ; break;}
        case 117 : {  asReturnASCII="u"  ; break;}
        case 118 : {  asReturnASCII="v"  ; break;}
        case 119 : {  asReturnASCII="w"  ; break;}
        case 120 : {  asReturnASCII="x"  ; break;}
        case 121 : {  asReturnASCII="y"  ; break;}
        case 122 : {  asReturnASCII="z"  ; break;}
        case 123 : {  asReturnASCII="{"  ; break;}
        case 124 : {  asReturnASCII="|"  ; break;}
        case 125 : {  asReturnASCII="}"  ; break;}
        case 126 : {  asReturnASCII="~"  ; break;}
        case 127 : {  asReturnASCII="DEL"; break;}
        default  : {  asReturnASCII="Err"; break;}
    }
    return asReturnASCII;
}
//------------------------------------------------------------------------------
byte       MyASCIIToDec(AnsiString asCode)
{
    byte cReturnDec=0;

    if     (asCode=="NUL")   cReturnDec=  0;
    else if(asCode=="SOH")   cReturnDec=  1;
    else if(asCode=="STX")   cReturnDec=  2;
    else if(asCode=="ETX")   cReturnDec=  3;
    else if(asCode=="EOT")   cReturnDec=  4;
    else if(asCode=="ENQ")   cReturnDec=  5;
    else if(asCode=="ACK")   cReturnDec=  6;
    else if(asCode=="BEL")   cReturnDec=  7;
    else if(asCode=="BS" )   cReturnDec=  8;
    else if(asCode=="HT" )   cReturnDec=  9;
    else if(asCode=="LF" )   cReturnDec= 10;
    else if(asCode=="VT" )   cReturnDec= 11;
    else if(asCode=="FF" )   cReturnDec= 12;
    else if(asCode=="CR" )   cReturnDec= 13;
    else if(asCode=="SO" )   cReturnDec= 14;
    else if(asCode=="SI" )   cReturnDec= 15;
    else if(asCode=="DLE")   cReturnDec= 16;
    else if(asCode=="DC1")   cReturnDec= 17;
    else if(asCode=="DC2")   cReturnDec= 18;
    else if(asCode=="DC3")   cReturnDec= 19;
    else if(asCode=="DC4")   cReturnDec= 20;
    else if(asCode=="NAK")   cReturnDec= 21;
    else if(asCode=="SYN")   cReturnDec= 22;
    else if(asCode=="ETB")   cReturnDec= 23;
    else if(asCode=="CAN")   cReturnDec= 24;
    else if(asCode=="EM" )   cReturnDec= 25;
    else if(asCode=="SUB")   cReturnDec= 26;
    else if(asCode=="ESC")   cReturnDec= 27;
    else if(asCode=="FS" )   cReturnDec= 28;
    else if(asCode=="GS" )   cReturnDec= 29;
    else if(asCode=="RS" )   cReturnDec= 30;
    else if(asCode=="US" )   cReturnDec= 31;
    else if(asCode==" "  )   cReturnDec= 32;
    else if(asCode=="!"  )   cReturnDec= 33;
    else if(asCode=="\"" )   cReturnDec= 34;
    else if(asCode=="#"  )   cReturnDec= 35;
    else if(asCode=="$"  )   cReturnDec= 36;
    else if(asCode=="%"  )   cReturnDec= 37;
    else if(asCode=="&"  )   cReturnDec= 38;
    else if(asCode=="\'" )   cReturnDec= 39;
    else if(asCode=="("  )   cReturnDec= 40;
    else if(asCode==")"  )   cReturnDec= 41;
    else if(asCode=="*"  )   cReturnDec= 42;
    else if(asCode=="+"  )   cReturnDec= 43;
    else if(asCode==","  )   cReturnDec= 44;
    else if(asCode=="-"  )   cReturnDec= 45;
    else if(asCode=="."  )   cReturnDec= 46;
    else if(asCode=="/"  )   cReturnDec= 47;
    else if(asCode=="0"  )   cReturnDec= 48;
    else if(asCode=="1"  )   cReturnDec= 49;
    else if(asCode=="2"  )   cReturnDec= 50;
    else if(asCode=="3"  )   cReturnDec= 51;
    else if(asCode=="4"  )   cReturnDec= 52;
    else if(asCode=="5"  )   cReturnDec= 53;
    else if(asCode=="6"  )   cReturnDec= 54;
    else if(asCode=="7"  )   cReturnDec= 55;
    else if(asCode=="8"  )   cReturnDec= 56;
    else if(asCode=="9"  )   cReturnDec= 57;
    else if(asCode==":"  )   cReturnDec= 58;
    else if(asCode==";"  )   cReturnDec= 59;
    else if(asCode=="<"  )   cReturnDec= 60;
    else if(asCode=="="  )   cReturnDec= 61;
    else if(asCode==">"  )   cReturnDec= 62;
    else if(asCode=="\?" )   cReturnDec= 63;
    else if(asCode=="@"  )   cReturnDec= 64;
    else if(asCode=="A"  )   cReturnDec= 65;
    else if(asCode=="B"  )   cReturnDec= 66;
    else if(asCode=="C"  )   cReturnDec= 67;
    else if(asCode=="D"  )   cReturnDec= 68;
    else if(asCode=="E"  )   cReturnDec= 69;
    else if(asCode=="F"  )   cReturnDec= 70;
    else if(asCode=="G"  )   cReturnDec= 71;
    else if(asCode=="H"  )   cReturnDec= 72;
    else if(asCode=="I"  )   cReturnDec= 73;
    else if(asCode=="J"  )   cReturnDec= 74;
    else if(asCode=="K"  )   cReturnDec= 75;
    else if(asCode=="L"  )   cReturnDec= 76;
    else if(asCode=="M"  )   cReturnDec= 77;
    else if(asCode=="N"  )   cReturnDec= 78;
    else if(asCode=="O"  )   cReturnDec= 79;
    else if(asCode=="P"  )   cReturnDec= 80;
    else if(asCode=="Q"  )   cReturnDec= 81;
    else if(asCode=="R"  )   cReturnDec= 82;
    else if(asCode=="S"  )   cReturnDec= 83;
    else if(asCode=="T"  )   cReturnDec= 84;
    else if(asCode=="U"  )   cReturnDec= 85;
    else if(asCode=="V"  )   cReturnDec= 86;
    else if(asCode=="W"  )   cReturnDec= 87;
    else if(asCode=="X"  )   cReturnDec= 88;
    else if(asCode=="Y"  )   cReturnDec= 89;
    else if(asCode=="Z"  )   cReturnDec= 90;
    else if(asCode=="["  )   cReturnDec= 91;
    else if(asCode=="\\" )   cReturnDec= 92;
    else if(asCode=="]"  )   cReturnDec= 93;
    else if(asCode=="^"  )   cReturnDec= 94;
    else if(asCode=="_"  )   cReturnDec= 95;
    else if(asCode=="`"  )   cReturnDec= 96;
    else if(asCode=="a"  )   cReturnDec= 97;
    else if(asCode=="b"  )   cReturnDec= 98;
    else if(asCode=="c"  )   cReturnDec= 99;
    else if(asCode=="d"  )   cReturnDec=100;
    else if(asCode=="e"  )   cReturnDec=101;
    else if(asCode=="f"  )   cReturnDec=102;
    else if(asCode=="g"  )   cReturnDec=103;
    else if(asCode=="h"  )   cReturnDec=104;
    else if(asCode=="i"  )   cReturnDec=105;
    else if(asCode=="j"  )   cReturnDec=106;
    else if(asCode=="k"  )   cReturnDec=107;
    else if(asCode=="l"  )   cReturnDec=108;
    else if(asCode=="m"  )   cReturnDec=109;
    else if(asCode=="n"  )   cReturnDec=110;
    else if(asCode=="o"  )   cReturnDec=111;
    else if(asCode=="p"  )   cReturnDec=112;
    else if(asCode=="q"  )   cReturnDec=113;
    else if(asCode=="r"  )   cReturnDec=114;
    else if(asCode=="s"  )   cReturnDec=115;
    else if(asCode=="t"  )   cReturnDec=116;
    else if(asCode=="u"  )   cReturnDec=117;
    else if(asCode=="v"  )   cReturnDec=118;
    else if(asCode=="w"  )   cReturnDec=119;
    else if(asCode=="x"  )   cReturnDec=120;
    else if(asCode=="y"  )   cReturnDec=121;
    else if(asCode=="z"  )   cReturnDec=122;
    else if(asCode=="{"  )   cReturnDec=123;
    else if(asCode=="|"  )   cReturnDec=124;
    else if(asCode=="}"  )   cReturnDec=125;
    else if(asCode=="~"  )   cReturnDec=126;
    else if(asCode=="DEL")   cReturnDec=127;

    return cReturnDec;
}
//------------------------------------------------------------------------------
AnsiString CountHexToByte(AnsiString asHCode)                                   //16進制轉2進制
{
    AnsiString asReturnByte="";
    for(int iHexNum=1; iHexNum<=asHCode.Length(); iHexNum++)
    {
        if     (asHCode.SubString(iHexNum,1)=="0")   asReturnByte+="0000";
        else if(asHCode.SubString(iHexNum,1)=="1")   asReturnByte+="0001";
        else if(asHCode.SubString(iHexNum,1)=="2")   asReturnByte+="0010";
        else if(asHCode.SubString(iHexNum,1)=="3")   asReturnByte+="0011";
        else if(asHCode.SubString(iHexNum,1)=="4")   asReturnByte+="0100";
        else if(asHCode.SubString(iHexNum,1)=="5")   asReturnByte+="0101";
        else if(asHCode.SubString(iHexNum,1)=="6")   asReturnByte+="0110";
        else if(asHCode.SubString(iHexNum,1)=="7")   asReturnByte+="0111";
        else if(asHCode.SubString(iHexNum,1)=="8")   asReturnByte+="1000";
        else if(asHCode.SubString(iHexNum,1)=="9")   asReturnByte+="1001";
        else if(asHCode.SubString(iHexNum,1)=="A")   asReturnByte+="1010";
        else if(asHCode.SubString(iHexNum,1)=="B")   asReturnByte+="1011";
        else if(asHCode.SubString(iHexNum,1)=="C")   asReturnByte+="1100";
        else if(asHCode.SubString(iHexNum,1)=="D")   asReturnByte+="1101";
        else if(asHCode.SubString(iHexNum,1)=="E")   asReturnByte+="1110";
        else if(asHCode.SubString(iHexNum,1)=="F")   asReturnByte+="1111";
    }

    return asReturnByte;
}
//------------------------------------------------------------------------------
int HexStrToInt(char *str)
{
    int Scale=1, j;
    char c;
    if(strlen(str)==0)
        return 0;
    //AI(W906-GA-EJ1N) 20260807: golden :329 was `strcpy(str, strupr(str));`.
    //  `strupr` is a Microsoft/OLDNAMES extension that MinGW`s <string.h> hides
    //  under -std=c++17 (__STRICT_ANSI__ suppresses the `#ifndef _NO_OLDNAMES`
    //  block, C:/MinGW/include/string.h:130-146) and the project builds with
    //  CMAKE_CXX_EXTENSIONS OFF (CMakeLists.txt:6) -- confirmed undeclared by an
    //  isolated g++ -std=c++17 -fsyntax-only probe.  This in-place ASCII upper-
    //  case loop is the exact equivalent for the [0-9A-Fa-f] input this parser
    //  accepts, and it also drops golden`s self-overlapping strcpy(str, str).
    for(char *p=str; *p; ++p)
        *p=(char)toupper((unsigned char)*p);
    int len=strlen(str);
    int sum=0;
    for(int i=(len-1); i>=0; i--)
    {
        c=str[i];
        if((c>='0' && c<='9'))
            j=(int)(c-'0');
        else if((c>='A' && c<='F'))
            j=10+(int)(c-'A');
        else
            break;
        sum+=j*Scale;
        Scale*=16;
    }
    return sum;
}
//------------------------------------------------------------------------------
//AI(W906-GA-EJ1N) 20260807: GATE [1] (banner GATE REGISTER).  golden :347-356.
//  This function is ALREADY in the port as vclcompat::HexStrToInt(const
//  AnsiString&) -- vclcompat/SysUtils.cpp:81-101, whose own comment cites
//  "golden ref EJ1N/TextProcess.cpp:347" and reproduces the same net behaviour --
//  and it is visible unqualified everywhere via `using vclcompat::HexStrToInt;`
//  (vclcompat/vcl_compat.h:271).  Defining golden`s by-value overload here is not
//  a duplicate symbol but an AMBIGUOUS CALL at every AnsiString call site, since
//  by-value copy-init and const-reference binding are both Exact Match: it would
//  break database.cpp:2509, EtherCAT/MyNUEC1.cpp:175, MyLaneIo.cpp:723 and
//  tests/test_vclcompat.cpp:341-353 (every HexStrToInt call site in the tree was
//  grepped).  Body kept VERBATIM so the retirement wave can lift it out unchanged.
//  BEHAVIOUR DELTA, CORRECTED AI(W906-PT-W1-integrate) 20260807 -- the original
//  wording here and in the header said "NO observable delta", and that was wrong
//  twice over:
//   (a) against golden-on-BCB6 the two differ on '$'-prefixed, internally spaced
//       and signed input.  golden PREPENDS "0x" whenever the substring "0x" is
//       absent, so "$1F" becomes "0x$1F" and parses to -1, while vclcompat trims,
//       accepts '$' and accepts a sign -- values this tree's own
//       tests/test_vclcompat.cpp:348 and :350 pin as 31 and 0x20.  They AGREE on
//       the two forms that actually occur in IO_Table / Mot_Table and the CCLink
//       frames: bare hex digits and an explicit "0x" prefix.
//   (b) worse, and now fixed: as this port stood, the body below returned -1 for
//       EVERY input including a plain "FF", because StrToIntDef resolved to a
//       DECIMAL-ONLY AnsiString::ToIntDef.  A wave following the header's
//       "RETIRED BY" instruction would have zeroed every hex Port column to -1.
//       vclcompat/AnsiString.cpp now parses '$' and '0x' the way BCB6's Val does,
//       so this body means what golden meant.  The retirement is safe again; the
//       residual delta in (a) is the only thing to weigh when taking it.
#if 0 // TODO(W906-GA-EJ1N): superseded by vclcompat::HexStrToInt(const AnsiString&)
int HexStrToInt(AnsiString str)
{
    AnsiString S;

    if(str.AnsiPos("0x")==0)
        S="0x"+str;
    else
        S=str;
    return StrToIntDef(S, -1);
}
#endif
//------------------------------------------------------------------------------
#define QETX 0x03
int SetBCC(AnsiString SendText)
{
    //AI(W906-GA-EJ1N) 20260807: golden :361 is `char *ASCII=SendText.c_str();`.
    //  BCB6 AnsiString::c_str() returns char*; this port`s returns const char*
    //  (vclcompat/AnsiString.h:92).  Declared const -- ASCII is only ever read
    //  below, so the XOR result is unchanged.
    const char *ASCII=SendText.c_str();
    int BCC=ASCII[0];

    for(unsigned int i=1; i<strlen(ASCII); i++)
    {
        BCC^=ASCII[i];
    }
    BCC^=QETX;

    return BCC;
}
//------------------------------------------------------------------------------
int CountByteToInt(AnsiString asBCode)                                          //2進制轉10進制
{
    if(asBCode=="0000")  return 0;
    if(asBCode=="0001")  return 1;
    if(asBCode=="0010")  return 2;
    if(asBCode=="0011")  return 3;
    if(asBCode=="0100")  return 4;
    if(asBCode=="0101")  return 5;
    if(asBCode=="0110")  return 6;
    if(asBCode=="0111")  return 7;
    if(asBCode=="1000")  return 8;
    if(asBCode=="1001")  return 9;
    if(asBCode=="1010")  return 10;
    if(asBCode=="1011")  return 11;
    if(asBCode=="1100")  return 12;
    if(asBCode=="1101")  return 13;
    if(asBCode=="1110")  return 14;
    if(asBCode=="1111")  return 15;
    return 0;
}
//------------------------------------------------------------------------------
AnsiString CountByteToHex(AnsiString asBCode)                                   //2進制轉16進制
{
    if(asBCode=="0000")  return "0";
    if(asBCode=="0001")  return "1";
    if(asBCode=="0010")  return "2";
    if(asBCode=="0011")  return "3";
    if(asBCode=="0100")  return "4";
    if(asBCode=="0101")  return "5";
    if(asBCode=="0110")  return "6";
    if(asBCode=="0111")  return "7";
    if(asBCode=="1000")  return "8";
    if(asBCode=="1001")  return "9";
    if(asBCode=="1010")  return "A";
    if(asBCode=="1011")  return "B";
    if(asBCode=="1100")  return "C";
    if(asBCode=="1101")  return "D";
    if(asBCode=="1110")  return "E";
    if(asBCode=="1111")  return "F";
    return "0";
}
//------------------------------------------------------------------------------
int ConvertByte(char *sData)
{
    int iRes=0;
    try
    {
        for(unsigned int i=0; i<strlen(sData); i++)
        {
            int iNum=sData[i];
            if(iNum>64)
            {
               iNum=iNum-7;
            }
            iNum =iNum-48;
            iRes+=iNum*pow(16, 3-i);
        }
    }
    catch(...)
    {
//        ShowDataOnComMemo(com_EMAware->CommName+" Function ConvertByte error!!! ");
        MyDBIProcess("Exception", "ConverByte");
    }
    return iRes;
}
//------------------------------------------------------------------------------
int Convert2Byte(char *sData)
{
    int iRes=0;
    try
    {
        for(unsigned int i=0; i<strlen(sData); i++)
        {
            int iNum=sData[i];
            if(iNum>64)
            {
                iNum=iNum-7;
            }
            iNum =iNum-48;
            iRes+=iNum*pow(16, 1-i);
        }
    }
    catch(...)
    {
//        ShowDataOnComMemo(com_EMAware->CommName+" Function Convert2Byte error!!! ");
        MyDBIProcess("Exception", "Conver2Byte");
    }
    return iRes;
}
//------------------------------------------------------------------------------
unsigned float2hex(unsigned char typecode, double value, int dataformat)
{
    unsigned hexValue=0;

    switch(typecode)
    {
        case 0x30:
            if(value>20)
                value=20;
            else if(value<0)
                value=0;
            break;
        case 0x31:
            if(value>20)
                value=20;
            else if(value<4)
                value=4;
            break;
        case 0x33:
            if(value>10)
                value=10;
            else if(value<-10)
                value=-10;
            break;
        case 0x35:
            if(value>5)
                value=5;
            else if(value<-5)
                value=-5;
            break;
        default:
            value=0;
            break;
    }

    if(dataformat)
    {
        hexValue=value*1000;
    }
    else
    {
        switch(typecode)
        {
            case 0x30:
                hexValue=value*65535/20;
                break;
            case 0x31:
                hexValue=(value-4)*65535/16;
                break;
            case 0x33:
                hexValue=value*32767/10;
                break;
            case 0x35:
                hexValue=value*32767/5;
                break;
            default:
                hexValue=0;
                break;
        }
    }

    return hexValue;
}
//------------------------------------------------------------------------------
AnsiString StringConvert2Hex(AnsiString S)                                      //Sam 20240604 : 新增 BinDisplay TFT
{
    AnsiString S1="", S2="";
    char s[12];
    char Target[10240];
    int len, i;
    strncpy(Target, S.c_str(), sizeof(Target));
    len=strlen(Target);
    for(i=0; i<len; i++)
    {
        if(Target[i]==0x01 || Target[i]==0x02 || Target[i]==0x03)
        {
            if(Target[i]==0x02)
                s[0]='<';
            if(Target[i]==0x01)
                s[0]='=';
            if(Target[i]==0x03)
                s[0]='>';

            s[1]=' ';
            s[2]=0;
        }
        else
        {
            s[0]=Target[i];
            s[1]=' ';
            s[2]=0;
        }
        S1+=s;

        sprintf(s, "%02X ", Target[i]);
        S2+=s;
    }
    return S2;
}
//------------------------------------------------------------------------------
unsigned char A_Create_LCR(unsigned char *Sptr, unsigned char length)
{
    unsigned char Btmp, Btmp1;
    Btmp1=0;
    do
    {
        Btmp=T_ASXII2HEX_Mac(*Sptr);                                            //CONV 0-9 A-F
        Sptr++;
        Btmp=(Btmp<<4)|T_ASXII2HEX_Mac(*Sptr);                                  //CONV 0-9 A-F
        Btmp1+=Btmp;                                                            //MATH LRC
        Sptr++;
        --length;
        if(length==0)                                                           //ChungHung 20121030 修正會造成無窮迴圈 執行緒會死掉
          break;
    }
    while(--length);                                                            //check EVEN
    return ((~Btmp1)+1);                                                        //LRC CODE
}
//------------------------------------------------------------------------------
unsigned char A_Create_LRC(unsigned char *Sptr, unsigned char length)
{
    unsigned char LRC=0;

    for(unsigned char z=0; z<length; z++)
    {
       LRC+=Sptr[z];
    }
    LRC=(0xff-LRC)+1;

    return LRC;
}
//------------------------------------------------------------------------------
bool GetCOMPortStatus(AnsiString Com)                                           //Steven 20120217 : Com Port改成可定義
{
    HANDLE h=INVALID_HANDLE_VALUE;
    AnsiString CN="";
    CN="\\\\.\\"+Com;
    h=::CreateFile(CN.c_str(),
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        0);

    if(h==INVALID_HANDLE_VALUE)
    {
        CN="\\\\.\\"+Com;
        h=::CreateFile(CN.c_str(),
            GENERIC_READ|GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            0);

        if(h==INVALID_HANDLE_VALUE)
        {
            return false;
        }
        else
        {
            CloseHandle(h);
            return true;
        }
    }
    CloseHandle(h);
    return true;
}
//------------------------------------------------------------------------------
double MyFormatFloat(double dValue, int iDigi)
{
    AnsiString Str;
    if(iDigi==0)
    {
        Str.sprintf("%0.0f", dValue);
    }
    else if(iDigi==1)
    {
        Str.sprintf("%0.1f", dValue);
    }
    else if(iDigi==2)
    {
        Str.sprintf("%0.2f", dValue);
    }
    else if(iDigi==3)
    {
        Str.sprintf("%0.3f", dValue);
    }
    else if(iDigi==4)
    {
        Str.sprintf("%0.4f", dValue);
    }
    else if(iDigi==5)
    {
        Str.sprintf("%0.5f", dValue);
    }
    else if(iDigi==6)
    {
        Str.sprintf("%0.6f", dValue);
    }
    else
    {
        Str.sprintf("%0.7f", dValue);
    }

    dValue=atof(Str.c_str());
    return dValue;
}
//------------------------------------------------------------------------------
