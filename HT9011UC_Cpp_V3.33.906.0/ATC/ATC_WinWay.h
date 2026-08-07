// =============================================================================
//  ATC/ATC_WinWay.h  --  AMD-M Winway ATC (temperature controller) Modbus-RTU
//                        driver over a serial TComm port.
//
//  Faithful translation of golden ATC/ATC_WinWay.h (33 lines, BCB6, Big5/cp950
//  -- this header itself is pure ASCII, no Chinese comments).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2, group "atc". Sibling of ATC/TCPData.{h,cpp},
//  landed in the SAME wave.
//
//  ROLE: one class, ATC_WinWay, wrapping a single serial port (TComm*) with a
//  hand-rolled Modbus-RTU-flavoured protocol (function code 0x03 read-holding
//  -register poll for present temperature, function code 0x06 write-single-
//  -register for set temperature) plus its own CRC16 (ANSI/Modbus polynomial
//  0xA001). NO state machine, NO switch(Task) cursor -- a small stateful
//  driver object (2260.10.18, Joye, per golden's own comment on GETCRC16).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim): the WHOLE header -- ctor + 9 methods + the 2
//     private double fields + the 1 public int field + the 3 public
//     AnsiString/bool/TComm* fields.
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     AnsiString (vclcompat/AnsiString.h), Spcomm::TComm (vclcompat/Comm.h,
//     already part of the vcl_compat.h umbrella).
//   GATE (#if 0): NONE.
//
//  VCL/Borland conversions: golden's `#include "system.hpp"` + `#include
//  "SPComm.hpp"` are replaced by `#include "vclcompat/vcl_compat.h"` (brings
//  in AnsiString + Spcomm::TComm, global via the umbrella's `using namespace
//  Spcomm;`). No __fastcall / __property / __published / #pragma in this
//  header (golden has none either).
//
//  MINIMAL NECESSARY ADAPTATION (1, flagged inline): golden's OWN .cpp
//  (ATC_WinWay.cpp:87) declares a local `DWord data;` inside GetPT() -- note
//  the lower-case `o`, NOT the standard Win32 `DWORD` golden's own header
//  spells correctly two lines below (golden .h:30 `void MySleep(DWORD
//  dwMilliseconds);`, matching common.h:261's real `DWORD`). Grepped the
//  ENTIRE D:/HT9045 tree (not just golden 906): `DWord` (this exact casing)
//  never appears in ANY typedef/#define, in this golden snapshot or the older
//  899 tree cpublic.cpp counterparts -- it is a bare, undefined-anywhere
//  identifier that BCB6 apparently accepted from some Borland RTL header this
//  project's include chain pulled in (golden ATC_WinWay.cpp only #includes
//  "MachineDefine.h", whose golden contents this port never received --
//  MachineDefine.h is a pure #include hub, see this tree's own MachineDefine.h
//  banner). Since GetPT() is ACTIVE code (not gated), it must compile, so a
//  local `typedef unsigned int DWord;` is added below, guarded, matching the
//  established precedent for exactly this class of gap (Interface/
//  InterfaceSYS.h:34-37's `Byte` typedef, MyPLC/MyPLC_IO_Modbus.h:32's `Byte`
//  typedef). `unsigned int` because GETCRC16's return type IS `unsigned int`
//  and `data` receives it directly (golden .cpp:95) before being masked with
//  `& 0xff` / `& 0xff00` -- any unsigned integer type >=16 bits gives the
//  IDENTICAL result for those two masks, so this is a zero-behaviour-delta
//  choice, not a guess elevated to load-bearing. Golden's own typo (`DWord`,
//  not `DWORD`) is kept verbatim as the identifier spelling in the .cpp --
//  only ITS DEFINITION is supplied here, nothing about the golden call site
//  is changed.
// =============================================================================
#ifndef ATC_WinWayH
#define ATC_WinWayH
#include "vclcompat/vcl_compat.h"   // AnsiString; Spcomm::TComm (Comm.h, global via umbrella)

// See the MINIMAL NECESSARY ADAPTATION note above: golden ATC_WinWay.cpp:87
// uses the bare identifier `DWord` (lower-case `o`), which is undefined
// anywhere in this repo snapshot. Guarded so a later central vclcompat
// addition of the same name does not collide (same guard-idiom precedent as
// Interface/InterfaceSYS.h:34-37).
#ifndef HT9045_VCLCOMPAT_TYPE_DWORD_LOWER_O_DEFINED
#define HT9045_VCLCOMPAT_TYPE_DWORD_LOWER_O_DEFINED
typedef unsigned int DWord;
#endif

//---------------------------------------------------------------------------
class ATC_WinWay
{
    private:
        double  fPresentTemperature;
        double  fSetTemperature;
    protected:
    public:
        bool                    bCommConnect;
        AnsiString              ansiST;
        AnsiString              sWinwayCommName;
        TComm*                  WinwayCOM;
        //function
        ATC_WinWay(TComm* Comm);
        double        GetPT();
        double        GetPT_NoCommand();
        void          SetPT(double _temp);
        void          SetPT(char* _temp);
        double        GetST();
        void          SetST(double _temp);
        bool          OpenCommPort();
        bool          CloseCommPort();
        unsigned int GETCRC16(unsigned char *string, unsigned char length);
        void MySleep(DWORD dwMilliseconds);
        int iWinWaySendCount;
};
#endif
