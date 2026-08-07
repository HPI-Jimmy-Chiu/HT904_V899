// =============================================================================
//  Monitor/MonitorTCPIP.h  --  generic TCP-connection monitor/adapter wrapper
//
//  Faithful translation of golden Monitor/MonitorTCPIP.h (45 lines, BCB6,
//  Big5/cp950 -- this header has no Chinese text of its own).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (group "socket").
//
//  ROLE: MonitorTCPIP owns a background HThreadCtrl watcher thread
//  (MV_TCPIP_WATCH) that drains whatever `TCPIP*` connection it is handed
//  (golden's ATC/TCPData.h abstract base) and mirrors traffic into an
//  optional TMemo log. Golden's own consumer is Monitor/MonitorInterface.cpp
//  (constructs a concrete TCPClient/TCPServer and hands it in) -- NEITHER
//  MonitorInterface NOR ATC/TCPData is part of THIS wave's two assigned
//  units, and grepping the whole port tree (excluding build*/) before writing
//  a line found NEITHER golden unit ported anywhere, and NOTHING in the
//  currently-ported tree references `MonitorTCPIP` at all (see GATE REGISTER
//  in MonitorTCPIP.cpp for the full accounting).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim: pure declaration file (no
//  function bodies); every method is DEFINED in MonitorTCPIP.cpp except
//  GetDataString() -- see that method's own note below.
//
//  VCL/Borland conversions:
//    * `TCPIP*`/`DATA`/`TD_ERROR`/`TD_READ`/`TD_SEND`/`TD_STATE` are golden's
//      OWN ATC/TCPData.h public interface (golden :5 `#include "TCPData.h"`).
//      That header/its .cpp are an entire separate, unported golden unit (not
//      one of this wave's two assigned units -- see MonitorTCPIP.cpp's file
//      banner for the full grep-verified accounting). Reproducing them here
//      verbatim-but-forward-only (², below) keeps this header's public
//      surface byte-shape-identical to golden's (same `TCPIP*` member type,
//      same `DATA` parameter/return types on every public method) WITHOUT
//      inventing a competing full `class TCPIP` definition that a future
//      dedicated ATC/TCPData translation wave would collide with:
//        (a) `class TCPIP;` is FORWARD-DECLARED ONLY (never fully defined
//            anywhere in this file or its .cpp) -- sufficient for a pointer
//            member and for pointer-typed ctor parameters/assignments, which
//            is all this header's OWN declarations need. Every place the
//            .cpp would need to actually CALL a method on the pointee is
//            gated (see MonitorTCPIP.cpp's GATE REGISTER) -- the golden call
//            lives inside a `#if 0` arm that the preprocessor strips before
//            the compiler ever needs `TCPIP` to be a complete type, so the
//            forward declaration is enough for this whole translation unit
//            to compile clean.
//        (b) `DATA` (golden `typedef vector<BYTE> DATA;`, ATC/TCPData.h:23)
//            and the four `TD_*` enum-like constants (golden ATC/TCPData.h
//            :18-21) are reproduced HERE, byte-identical to golden's own
//            spelling and values -- they are load-bearing in THIS header's
//            own method signatures (SendCommand(DATA)/AdapterData(int&,DATA&)/
//            ShowData(AnsiString,DATA)), not merely internal-body details, so
//            they cannot be deferred into the .cpp. A `typedef`/`#define` is
//            not a class definition: if/when a real ATC/TCPData.h lands with
//            the IDENTICAL typedef and IDENTICAL macro values (which it must,
//            to stay byte-identical to golden), re-declaring the same
//            typedef and the same macro value is legal C++ (no ODR conflict)
//            -- so this is NOT expected to need edits at that integration
//            point, only for `#include "TCPData.h"` to eventually replace
//            this stand-in block (left for the integrate agent, matching the
//            established `aArmHeader.h` "guard-only shim" precedent for a
//            not-yet-ported golden include).
//    * `_bCommandReady[10]` (golden :21) kept as plain `int[10]`, matching
//      golden (not an enum-backed size -- golden's own ctor loop is `for(int
//      i=0;i<10;i++)`, no named constant).
//    * `vector<int> _error_code` (golden :19) -- plain `std::vector<int>`,
//      `using namespace std` already in scope via MachineDefine.h; NEVER
//      read or written by golden's own 176 lines (grepped -- `_error_code`
//      appears nowhere outside its own declaration), so it carries no
//      GATE of its own.
//    * `GetDataString()` (golden :40, `String GetDataString();`) is declared
//      in golden's OWN header but has NO body anywhere in golden's own
//      Monitor/MonitorTCPIP.cpp (176 lines, read in full) and is never called
//      by any golden .cpp in the whole 906 tree (grepped) -- it is a
//      dead/never-defined declaration in GOLDEN ITSELF, not a gap this
//      translation introduces. Mirrored here as a declaration-only entry
//      with NO definition in the .cpp, matching golden's own (non-)state; if
//      nothing ever calls it (nothing does, tree-wide), this links exactly
//      as cleanly as golden's own equivalent build did.
//    * `String` resolves to `vclcompat::AnsiString` via the umbrella's
//      `typedef vclcompat::AnsiString String;` (vclcompat/vcl_compat.h:340) --
//      used verbatim, not rewritten to `AnsiString`.
//
//  Big5: this header has no Chinese comments to preserve. Final gate: ZERO
//  U+FFFD (vacuously true).
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "TCPData.h"
//   #include "HThreadCtrl.h"
// =============================================================================
#ifndef MonitorTCPIPH
#define MonitorTCPIPH

#include "MachineDefine.h"          // de-VCL'd include hub: vclcompat umbrella (AnsiString/String/TMemo via Controls.h chain) + <vector> + `using namespace std;`
#include "HThreadCtrl/HThreadCtrl.h" // HThreadCtrl (golden bare "HThreadCtrl.h" -- this tree's mirrored path; already real, AI(W906-PT-W2) sibling unit)
#include "vclcompat/Controls.h"     // TMemo (golden ATC/TCPData.h forward chain pulls in VCL StdCtrls -- this tree's TMemo stand-in)

// ---------------------------------------------------------------------------
//  AI(W906-PT-W2-integrate) 20260807: REPLACED a forward declaration + a private
//  copy of TCPData.h's constants.  What stood here was:
//
//      class TCPIP;                        // "forward-declared only"
//      #define BYTE_DATA / STRING_DATA / TD_ERROR / TD_READ / TD_SEND / TD_STATE
//      typedef vector<BYTE> DATA;
//
//  justified by a banner claim that golden ATC/TCPData.h is "out-of-scope,
//  unported".  That claim was true when this file was drafted and FALSE by the
//  time it landed: ATC/TCPData.{h,cpp} were translated in the SAME wave (PT-W2)
//  and went into ht9045_comms.  ATC/TCPData.h:74-99 defines the real abstract
//  TCPIP with exactly the members this unit calls -- Initial/Connect/DisConnect/
//  Send(DATA)/Send(string) pure virtual, IsConnect() and PopBuffer() concrete --
//  and :62-71 already defines every constant that was being duplicated here.
//  Golden's own Monitor/MonitorTCPIP.h:5 is `#include "TCPData.h"`; this is that
//  include, written with the directory prefix this tree uses for cross-directory
//  headers (the include root is the port root).
//
//  The consequence of leaving it: nine call sites in the .cpp were gated to
//  degraded constants (IsConnect()->false, SendCommand()->-1, AdapterData()->
//  iSize 0), so the monitor link would have reported "never connected, every
//  send failed, nothing ever received" on a machine where it was working.
// ---------------------------------------------------------------------------
#include "ATC/TCPData.h"   // golden :5 `#include "TCPData.h"` -- TCPIP, DATA, TD_*

//---------------------------------------------------------------------------

class MonitorTCPIP
{
    private:

    protected:
        TCPIP*          _socket;
        HThreadCtrl     _socket_thread;

        TMemo*          _memo;

        vector< int >   _error_code;

        int    _bCommandReady[10];

    public:
        MonitorTCPIP( TCPIP* pSocket );
        ~MonitorTCPIP();

        void InitialSocket( AnsiString sAddress , int iPort );
        void InitialMemo( TMemo* pMemo );

        void Connect();
        void Disconnect();
        bool IsConnect();

        int SendCommand( DATA SendData );
        int SendCommand( AnsiString SendData );

        int AdapterData( int& iType , DATA& vData );
        void ShowData( AnsiString sType , DATA vData );

        String GetDataString();   // golden :40 -- declared, NEVER defined anywhere in golden itself and never called tree-wide; see banner note. No body in the .cpp.
        int GetReceiveCommand(int index);
};
void MV_TCPIP_WATCH( void* pParameter );

#endif
