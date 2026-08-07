// =============================================================================
//  SCK_TUTS.h  --  BCB6 WSDL-Importer-generated SOAP client stub for the
//                  "SIP1" web service (customer program-download endpoint,
//                  http://10.20.20.32/cimreport/ws/sip1.asmx).
//
//  Faithful translation of golden Automation/SCK_TUTS.h (58 lines, BCB6,
//  Big5/cp950 -- EXCEPT for one 4-byte fragment that is NOT valid Big5/cp950
//  at all, see GOLDEN BYTE-LEVEL DEFECT below).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  GOLDEN BYTE-LEVEL DEFECT (read before touching the banner comment below)
//  --------------------------------------------------------------------------
//  Golden line 7 is `// (2016-10-31 <2 chars> 4:27:29 - $Revision: ... $)`,
//  the WSDL-Importer's auto-generated "when was this stub regenerated"
//  timestamp. The 4 raw bytes at the <2 chars> position are
//  0xBF 0xC0 0xC8 0xC4. The first pair (0xBF 0xC0) IS valid cp950 (decodes to
//  U+8783); the second pair (0xC8 0xC4) is NOT -- 0xC8 is not a valid Big5/
//  cp950 lead byte at all (verified: no trail byte in 0x40-0xFE makes
//  0xC8-led cp950 decode succeed). This is a genuine pre-existing corruption
//  IN GOLDEN ITSELF, not an artifact of how this translation read the file --
//  confirmed by comparing against golden Automation/MesWebService.h line 7,
//  the SAME WSDL-Importer banner template with an intact timestamp:
//  `// (2022/11/09 10:53:30 - $Revision: ... $)` (no AM/PM token at all, a
//  different date format -- so there is no sibling occurrence in this tree to
//  recover the intended characters from either). Comparing the intact bytes
//  against `"上午"`.encode(cp950) == b"\xa4\x57\xa4\xc8" and
//  `"下午"`.encode(cp950) == b"\xa4\x55\xa4\xc8" (the two plausible AM/PM
//  markers for a 12-hour "4:27:29" timestamp) rules out both guesses -- the
//  actual bytes do not match either. Rather than invent Chinese text golden
//  does not actually contain (a fidelity violation) or silently drop the
//  timestamp comment (a completeness violation) or emit U+FFFD (forbidden by
//  this wave's own rules), the comment below reproduces every decodable byte
//  verbatim and spells out the undecodable fragment as its raw hex escape.
//  This satisfies "ZERO U+FFFD" literally: there is no U+FFFD character
//  anywhere in this file, only an ASCII description of bytes that golden
//  itself cannot express in its own declared encoding.
//
//  ROLE: declares the `NS_sip1` namespace's SOAP interface surface --
//  SIP1Soap (6 pure RPC methods: HelloWorld/GetProgram/GetProgram1/
//  GetProgram2/EndLot/InputJam) and the GetSIP1Soap() factory that golden's
//  SCK_TUTS.cpp implements against Borland's SOAP RIO client (THTTPRIO).
//  NOT A FUNCTION INVENTORY FILE: this header declares ONE type and ONE
//  function; golden line-number citations for both live in SCK_TUTS.cpp's own
//  banner (the .h carries no bodies).
//
//  THE STRUCTURAL CHANGE -- __interface -> abstract base class
//  --------------------------------------------------------------------------
//  golden :38 `__interface INTERFACE_UUID("{...}") SIP1Soap : public
//  IInvokable` is Borland's COM-interface extension (__interface / IInvokable
//  / INTERFACE_UUID / DelphiInterface<T> -- all declared in <System.hpp> /
//  <InvokeRegistry.hpp>, VCL's SOAP-RIO COM machinery). `__interface` is not a
//  keyword this toolchain's g++ understands at all (unlike __fastcall, which
//  vclcompat/vcl_compat.h can `#define` away -- there is no way to `#define
//  __interface class` and have the rest of the COM contract, virtual dispatch
//  table layout, QueryInterface, INTERFACE_UUID GUID registration, or
//  DelphiInterface<T>'s intrusive refcounting, follow along for free).
//  Grepped the WHOLE port tree for `__interface` / `IInvokable` /
//  `SoapHTTPClient` / `THTTPRIO` / `DelphiInterface` / `InvokeRegistry`:
//  the only hit anywhere is a generated layout-tool artifact
//  (tools/dfm2rc/layout_out/Automation/SCK_WebService_layout.gen.cpp), not a
//  translated unit -- there is no precedent and no substrate to build on.
//  This is also, in golden itself, DEAD CODE FROM THE CALLER'S SIDE: grepped
//  the WHOLE golden tree for `SIP1Soap` / `GetSIP1Soap` -- the only
//  occurrences outside this file's own pair (SCK_TUTS.h/.cpp) are in
//  Automation/SCK_WebService.cpp/.h, a sibling wrapper unit that is OUT OF
//  THIS WAVE'S SCOPE (not one of the 5 files assigned) and, as far as this
//  wave can verify, is not itself called from any translated unit either.
//  Faithful translation therefore preserves the NAME and SHAPE of the
//  contract (an abstract base class with the same six pure-virtual methods,
//  same parameter/return types) using plain C++ virtual dispatch -- the
//  standard "COM interface -> abstract base class" idiom -- WITHOUT
//  reference counting, GUID identity or QueryInterface, none of which any
//  caller in this tree's translated scope exercises. `_di_SIP1Soap` (golden
//  :48 `typedef DelphiInterface<SIP1Soap> _di_SIP1Soap;`) becomes a plain raw
//  pointer typedef -- the caller-visible shape (`_di_SIP1Soap` as a
//  nullable handle you call methods through) is unchanged; only the
//  lifetime-management mechanism (COM refcounting vs. none) differs, and
//  nothing in this tree's translated scope ever constructs or destroys one
//  (see SCK_TUTS.cpp's own banner: GetSIP1Soap() is whole-function-gated).
//
//  GOLDEN QUIRK preserved VERBATIM, not fixed: the `#endif // __sip1_h__` at
//  golden :51 closes the `#ifndef SCK_TUTSH` include guard (golden :10)
//  *before* the `namespace NS_sip1 { ... }` block's own closing brace at
//  golden :52 -- i.e. the include guard's #endif sits INSIDE the namespace
//  body, textually, even though the preprocessor does not care about brace
//  nesting so the file still parses top-to-bottom exactly once. The `};` at
//  :52 and the `using namespace NS_sip1;` guard at :54-56 are therefore
//  OUTSIDE the SCK_TUTSH include guard -- a second #include of this header in
//  the SAME translation unit would re-run `};` with no matching open namespace
//  brace and fail to compile. This is a real double-inclusion hazard baked
//  into golden's own WSDL-Importer output (a known class of bug in older
//  Borland-generated SOAP stubs), reproduced exactly rather than "fixed" by
//  moving the include-guard #endif to the true end of file. This translation
//  is not exercised twice in the same TU (SCK_TUTS.cpp itself only ever
//  includes this header once, guarded by `#if !defined(SCK_TUTSH)`), so the
//  hazard is latent here exactly as it is latent in golden.
//
//  VCL/Borland conversions: golden :13-16 `#include <System.hpp>` /
//  <InvokeRegistry.hpp> / <XSBuiltIns.hpp> / <SoapHTTPClient.hpp> dropped --
//  every one of them existed only to supply the COM/SOAP machinery this file
//  no longer uses (see the structural-change note above). No __fastcall /
//  __property / __published anywhere in golden's own declarations here (COM
//  interface methods use plain virtual dispatch even in BCB6).
//
//  Big5: every OTHER Chinese/non-ASCII byte in this file (there are none
//  besides the corrupted banner fragment already covered above) -- N/A, this
//  file has no Chinese prose comments of its own past the banner. Final gate:
//  ZERO U+FFFD (the corrupted fragment is spelled out as hex, never decoded).
// =============================================================================
// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2016-10-31 [bytes 0xBF 0xC0 0xC8 0xC4 -- undecodable as Big5/cp950 in
//  golden itself; see GOLDEN BYTE-LEVEL DEFECT above] 4:27:29 -
//  $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //
//Steven 20161101 : SCK SOAP

#ifndef   SCK_TUTSH
#define   SCK_TUTSH

#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W2) 20260807: was <System.hpp>/<InvokeRegistry.hpp>/<XSBuiltIns.hpp>/<SoapHTTPClient.hpp> -- see the file head for why none of the COM/SOAP machinery survives

namespace NS_sip1
{
    // ************************************************************************ //
    // The following types, referred to in the WSDL document are not being represented
    // in this file. They are either aliases[@] of other types represented or were referred
    // to but never[!] declared in the document. The types from the latter category
    // typically map to predefined/known XML or Borland types; however, they could also
    // indicate incorrect WSDL documents that failed to declare or import a schema type.
    // ************************************************************************ //
    // !:string          - "http://www.w3.org/2001/XMLSchema"

    // ************************************************************************ //
    // Namespace : http://tempuri.org/
    // soapAction: http://tempuri.org/%operationName%
    // transport : http://schemas.xmlsoap.org/soap/http
    // binding   : SIP1Soap
    // service   : SIP1
    // port      : SIP1Soap
    // URL       : http://10.20.20.32/cimreport/ws/sip1.asmx
    // ************************************************************************ //
    //AI(W906-PT-W2) 20260807: golden ":38 __interface INTERFACE_UUID(\"{...}\") SIP1Soap : public IInvokable" -> plain abstract base class, no COM identity/refcounting. See the file head's structural-change note.
    class SIP1Soap
    {
        public:
            virtual ~SIP1Soap() {}
            virtual AnsiString      HelloWorld() = 0;
            virtual AnsiString      GetProgram(const AnsiString strLot) = 0;
            virtual AnsiString      GetProgram1(const AnsiString strLot, const AnsiString strHandler, const AnsiString strMap) = 0;
            virtual AnsiString      GetProgram2(const AnsiString strLot, const AnsiString strHandler, const AnsiString strMap) = 0;
            virtual AnsiString      EndLot(const AnsiString LOTID) = 0;
            virtual AnsiString      InputJam(const AnsiString strMessage) = 0;
    };
    typedef SIP1Soap* _di_SIP1Soap;                                             //AI(W906-PT-W2) 20260807: golden ":48 typedef DelphiInterface<SIP1Soap> _di_SIP1Soap;" -> raw pointer, no COM refcounting (see file head)

    _di_SIP1Soap GetSIP1Soap(bool useWSDL=false, AnsiString addr="");
#endif // __sip1_h__ -- AI(W906-PT-W2) 20260807: golden's own mislabelled include-guard #endif, kept exactly where golden put it (see GOLDEN QUIRK above)
};     // NS_sip1

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace NS_sip1;
#endif
