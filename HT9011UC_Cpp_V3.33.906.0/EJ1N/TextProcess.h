// =============================================================================
//  TextProcess.h  --  EJ1N text / protocol conversion helpers (declarations).
//
//  Faithful translation of golden EJ1N/TextProcess.h (30 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-GA-EJ1N) 20260807
//  Translation wave: W906-GA-EJ1N (pure-translation completion campaign).  First
//  file of the EJ1N/ directory to reach the port tree: before this wave EJ1N/ had
//  NO port at all, which is the blocker cpublic.cpp:281-292 and
//  CCLink/MyCCLink.cpp:77-83 both record against their own gated/undefined work.
//
//  ROLE: declares the 17 EJ1N free functions + the 2 const conversion tables that
//  the Omron EJ1N / Panasonic KT4H / Delta DT4848 serial temperature-controller
//  code and the CCLink board code use to move between ASCII text, decimal, hex,
//  binary text and the BCC / LRC frame checksums.  No state, no class, no VCL
//  form -- pure free functions.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim): all declarations EXCEPT gate [1] below.
//   SATISFIED-BY-SHIM (so it LINKS):  none -- this header declares only symbols
//                                     defined by EJ1N/TextProcess.cpp itself.
//   GATE (#if 0): 1 -- registered below.
//
//  GATE REGISTER
//   [1] extern int HexStrToInt(AnsiString str);        golden EJ1N/TextProcess.h:13
//       (definition golden EJ1N/TextProcess.cpp:347-356)
//       WHY IT CANNOT LAND: this exact function is ALREADY in the port, as
//       vclcompat::HexStrToInt(const AnsiString&) -- vclcompat/SysUtils.h:31,
//       body vclcompat/SysUtils.cpp:81-101, whose own header comment cites
//       "golden ref EJ1N/TextProcess.cpp:347" as its source and reproduces its
//       net behaviour (prepend "0x" if absent, then StrToIntDef(s,-1)).  It is
//       pulled into the GLOBAL namespace by `using vclcompat::HexStrToInt;`
//       (vclcompat/vcl_compat.h:271), which this header includes.  Declaring
//       golden`s by-value `HexStrToInt(AnsiString)` alongside it does NOT produce
//       a duplicate symbol (different mangled name) -- it produces something
//       worse: an AMBIGUOUS CALL at every existing AnsiString call site, because
//       by-value copy-init and const-reference binding are both Exact Match
//       conversion sequences.  That would break, today, database.cpp:2509,
//       EtherCAT/MyNUEC1.cpp:175, MyLaneIo.cpp:723 and tests/test_vclcompat.cpp
//       :341-353.  Verified by grepping every HexStrToInt call site in the tree.
//       DEFAULT IN FORCE: vclcompat::HexStrToInt(const AnsiString&).
//       DELTA, CORRECTED AI(W906-PT-W1-integrate) 20260807 -- this line used to
//       read "same net behaviour, so there is NO observable delta", which was
//       false.  The two AGREE on bare hex digits and on an explicit "0x" prefix
//       (the only forms IO_Table / Mot_Table / the CCLink frames contain).  They
//       DIFFER on '$'-prefixed, internally spaced and signed input: golden
//       prepends "0x" unconditionally and lands on -1, vclcompat trims and
//       accepts '$' and a sign -- see tests/test_vclcompat.cpp:348 (31) and
//       :350 (0x20).  Separately, the gated body itself used to evaluate to -1
//       for EVERY input in this port because StrToIntDef was decimal-only;
//       vclcompat/AnsiString.cpp was fixed to parse '$'/'0x' as BCB6's Val does,
//       so the body now means what golden meant.
//       VERIFIED, not reasoned: hand-ungating the declaration and compiling
//       gives exactly "error: call of overloaded HexStrToInt(AnsiString&) is
//       ambiguous", naming vclcompat/SysUtils.h:31 as the rival candidate.
//       RETIRED BY: a wave that removes HexStrToInt from vclcompat/SysUtils and
//       repoints its 11 call sites at this unit.  Until then, do not ungate.
//
//  NOTE on the sibling overload HexStrToInt(char*) (golden .h:12), which IS
//  active here: it is a genuinely DIFFERENT function (it upper-cases the caller`s
//  buffer in place and stops at the first non-hex char rather than returning -1),
//  and vclcompat has no char* overload.  Every existing call site in the tree
//  passes `something.c_str()`, i.e. `const char*` (AnsiString::c_str() is const in
//  this port, vclcompat/AnsiString.h:92), which cannot bind to `char*` -- so those
//  sites keep resolving to the vclcompat AnsiString overload exactly as they do
//  today, and no ambiguity is possible.  A call passing a MUTABLE char buffer
//  would now bind here instead; no such call site exists in the tree (checked).
//  All four cases were compiled and the selected overload read back out of the
//  object file with nm, not merely reasoned about.
//
//  !! ONE LATENT HAZARD, inherited from golden, stated here so nobody is
//  surprised: `HexStrToInt("FF")` -- a bare STRING LITERAL -- binds to the char*
//  overload, because GCC still accepts the (C++11-removed) literal->char*
//  conversion as a mere -Wwrite-strings warning.  That overload upper-cases its
//  argument IN PLACE, i.e. it writes to read-only .rdata and crashes.  Golden has
//  the identical hazard (BCB6 allowed the same conversion), and there is no such
//  call site in either tree today -- but if you add one, pass AnsiString("FF")
//  or a writable buffer, never a literal.
//
//  VCL/Borland conversions: golden`s header assumed MachineDefine.h had already
//  pulled <vcl.h>; this port includes vclcompat/vcl_compat.h itself so the header
//  is self-contained (AnsiString, and windows.h -> rpcndr.h `byte` for the
//  MyASCIIToDec return type -- the same `byte` cmydef.h:127 and common.cpp:369
//  already rely on; see vcl_compat.h:185-187 on why WIN32_LEAN_AND_MEAN is not
//  defined).  NO __fastcall / __property / __published anywhere.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8, in its
//  original column.  Final gate: ZERO U+FFFD.
// =============================================================================

// BCB6 ORIGINAL header (mirrored as a comment for provenance):
//   //-------------------------------------------------------------------------
//   #ifndef TextProcessH / #define TextProcessH
//   (no #include of its own -- AnsiString and `byte` arrived via MachineDefine.h)
// =============================================================================

//---------------------------------------------------------------------------

#ifndef TextProcessH
#define TextProcessH

#include "vclcompat/vcl_compat.h"   // AnsiString; windows.h -> rpcndr.h `byte`

//---------------------------------------------------------------------------
#define swap16(x) ((((x) & 0xFF)<<8) | (((x)>>8) & 0xFF))
//---------------------------------------------------------------------------
extern AnsiString MyDeCodeASCII(int iInPut);                                    //10進制 轉 ASCII Code
extern byte       MyASCIIToDec(AnsiString asCode);                              //ASCII Code 轉 10進制
extern AnsiString CountHexToByte(AnsiString asHCode);                           //16進制轉2進制
extern int        HexStrToInt(char *str);                                       //16進制轉10進制
//AI(W906-GA-EJ1N) 20260807: GATE [1] -- golden :13 is superseded by
//  vclcompat::HexStrToInt(const AnsiString&) (vclcompat/SysUtils.h:31, already
//  global via vcl_compat.h:271).  Declaring golden`s by-value overload too makes
//  every AnsiString call site AMBIGUOUS (both are Exact Match), which would break
//  database.cpp:2509 / EtherCAT/MyNUEC1.cpp:175 / MyLaneIo.cpp:723 /
//  tests/test_vclcompat.cpp:341.  Same net behaviour either way -- no delta.
//  See the GATE REGISTER in the banner above.
#if 0 // TODO(W906-GA-EJ1N): superseded by vclcompat::HexStrToInt(const AnsiString&)
extern int        HexStrToInt(AnsiString str);                                  //16進制轉10進制
#endif
extern int        SetBCC(AnsiString SendText);                                  //計算BCC
extern int        CountByteToInt(AnsiString asBCode);                           //2進制轉10進制
extern AnsiString CountByteToHex(AnsiString asBCode);                           //2進制轉16進制
extern AnsiString StringConvert2Hex(AnsiString S);                              //Sam 20240604 : 新增 BinDisplay TFT
extern int Convert2Byte(char *sData);
extern int ConvertByte(char *sData);
extern unsigned float2hex(unsigned char typecode, double value, int dataformat);
extern unsigned char A_Create_LCR(unsigned char *Sptr, unsigned char length);
extern unsigned char A_Create_LRC(unsigned char *Sptr, unsigned char length);

extern const unsigned char T_HEX2ASCII[16];
extern const unsigned char T_ASXII2HEX[23];
extern unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
extern unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
extern bool GetCOMPortStatus(AnsiString Com);                                   //Steven 20120217 : Com Port改成可定義
extern double MyFormatFloat(double dValue, int iDigi);                          //Steven 20241014 : 指定小數點位數
#endif
