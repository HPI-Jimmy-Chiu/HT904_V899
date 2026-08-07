// =============================================================================
//  myio.h  --  raw ISA/PCI digital-IO port read/write API (BCB6 outportb/
//              inportb front end), plus the OutPortData/InPortData output-echo
//              cache save/restore pair.
//
//  Faithful translation of golden myio.h (21 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (rootleaf group).
//
//  ROLE: the LOW-LEVEL raw-port IO surface -- SAFETY-CRITICAL (this header's
//  own group brief flags it explicitly).  golden IOBitOn/IOBitOff/IOByteOut
//  drive real digital-output lines through a safe-door interlock
//  (IdleCheckSafeDoorByCylinder) before ever touching hardware; see myio.cpp's
//  own banner for the exact gate rationale.
//
//  RELATIONSHIP TO ht9045_io's TLaneIO (MyLaneIo.h) -- READ BEFORE ASSUMING
//  THESE ARE THE SAME THING, THEY ARE NOT MERGED:
//    TLaneIO (MyLaneIo.h/.cpp, already in this tree) is golden's OTHER,
//    NEWER IO facade -- a per-Ring/IP/Port addressed class wrapping a
//    TIOBackend* (Sim by default), with its own OutPortData CACHE (a
//    3-D array keyed [Ring][IP][Port], MyLaneIo.cpp:course) and its own
//    CheckPortRangeErr/SOFT_SIMULTE bookkeeping.  golden itself keeps BOTH
//    APIs alive side by side (grep of the real BCB6 tree shows call sites for
//    each): myio.cpp's free functions are the OLDER, single-flat-port-number
//    ISA/legacy-card API (IOSetOutport/IOSetInport index into 128-slot flat
//    arrays keyed purely by port NUMBER, no Ring/IP addressing at all), while
//    TLaneIO is the newer multi-card/multi-network abstraction added for
//    MotionNet/PCI1203-class hardware. They are two distinct golden call
//    surfaces translated as two distinct port units -- this file does not
//    reuse TLaneIO's cache, and TLaneIO does not call any function declared
//    here (confirmed: MyLaneIo.cpp never calls IOBitOn/IOBitOff/IOByteOut/
//    IOInputBit/IOInputByte/IOSetOutport/IOSetInport). Do not fold them
//    together in a later wave without re-checking that golden itself never
//    unified them either -- it does not, this split is golden's own, not an
//    artifact of translation order.
//
//  WAVE SCOPE: every declaration below is mirrored VERBATIM from golden
//  myio.h. Two of them -- IOInputLongByte and InitialMyIOData -- are declared
//  here but have NO definition anywhere in golden myio.cpp either (confirmed:
//  golden myio.cpp defines InitialMyOutIOData, a DIFFERENTLY-NAMED function,
//  not InitialMyIOData; and IOInputLongByte has no body in golden myio.cpp at
//  all). This is a pre-existing golden discrepancy between the header and this
//  one .cpp, not something this translation introduces or is asked to fix --
//  see myio.cpp's own banner for the citation. The two declarations are kept
//  here for header fidelity; nothing in this port tree calls either symbol
//  (confirmed by whole-tree grep), so the absent bodies are not a link gap
//  today.
//
//  VCL/Borland conversions: the commented-out `//void CheckWindowsIsXP();`
//  (golden :6) is dead text in golden itself (never live code) and is kept as
//  a comment below, unchanged. No __fastcall / __property / #pragma in golden
//  myio.h to strip.
//
//  Big5: golden myio.h carries no Chinese text (verified via the cp950 decode
//  this wave performed) -- 0 characters to preserve, 0 U+FFFD.
// =============================================================================
#ifndef myioH
#define myioH
//---------------------------------------------------------------------------
#include <windows.h>                // byte -- same precedent citation as MyLaneIo.h:27 ("byte / BYTE / DWORD")

//void CheckWindowsIsXP();

int  IOSetOutport(int port);
bool IOSetInport(int port);
void IOBitOn(int port, int bit);
void IOBitOff(int port, int bit);
void IOByteOut(int port, unsigned int Byte);
bool IOOutBitStatus(int port, int bit);
byte IOOutByteStatus(int port);
bool IOInputBit(int port, int bit);
unsigned int IOInputLongByte(int port);   // golden-declared, golden-undefined -- see banner
byte IOInputByte(int port);
void BackUpOutputData();
void RestoreOutputData();
void InitialMyIOData();                   // golden-declared, golden-undefined -- see banner (golden .cpp defines InitialMyOutIOData instead)
#endif
