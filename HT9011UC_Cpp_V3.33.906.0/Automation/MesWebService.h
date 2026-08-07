// =============================================================================
//  MesWebService.h  --  BCB6 WSDL-Importer-generated SOAP client stub for the
//                       "MesWebService" web service (customer MES lot-
//                       attribute lookup / manual EOCAP creation endpoint,
//                       http://192.168.10.216/vt_mes/MesWebService.asmx).
//
//  Faithful translation of golden Automation/MesWebService.h (55 lines, BCB6,
//  Big5/cp950 -- this file has NO Chinese text and NO byte-level corruption;
//  unlike its sibling SCK_TUTS.h, the timestamp banner at golden :7 decodes
//  cleanly).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//  Sibling of Automation/SCK_TUTS.h/.cpp (SAME wave) -- an IDENTICAL
//  WSDL-Importer template around a different service contract. Every
//  structural decision below is the one already made and justified in full
//  in SCK_TUTS.h's banner; this banner cross-references rather than
//  re-deriving the same evidence.
//
//  ROLE: declares the `NS_MesWebService` namespace's SOAP interface surface --
//  MesWebServiceSoap (2 pure RPC methods: GetLotAttributes/CreateManualEOCAP)
//  and the GetMesWebServiceSoap() factory that golden's MesWebService.cpp
//  implements against Borland's SOAP RIO client (THTTPRIO). This header
//  declares ONE type and ONE function; golden line-number citations for both
//  live in MesWebService.cpp's own banner (the .h carries no bodies).
//
//  THE STRUCTURAL CHANGE -- __interface -> abstract base class
//  --------------------------------------------------------------------------
//  golden :38 `__interface INTERFACE_UUID("{...}") MesWebServiceSoap : public
//  IInvokable` -- SAME Borland COM-interface extension SCK_TUTS.h:38 uses, for
//  the SAME reason with the SAME grep evidence (no __interface/IInvokable/
//  SoapHTTPClient/THTTPRIO/DelphiInterface/InvokeRegistry substrate anywhere
//  in this port tree outside a generated layout-tool artifact -- see
//  SCK_TUTS.h's structural-change note for the full grep). This is also, in
//  golden itself, DEAD CODE FROM THE CALLER'S SIDE exactly like SCK_TUTS:
//  grepped the WHOLE golden tree for `MesWebServiceSoap` /
//  `GetMesWebServiceSoap` -- the only occurrences outside this file's own
//  pair are in Automation/SCK_WebService.cpp/.h, OUT OF THIS WAVE'S SCOPE and,
//  as far as this wave can verify, unreached from any translated unit.
//  Translated the SAME way: an abstract base class with the same two
//  pure-virtual methods, same parameter/return types, plain C++ virtual
//  dispatch, no reference counting/GUID identity/QueryInterface.
//  `_di_MesWebServiceSoap` (golden :44) becomes a plain raw pointer typedef,
//  same as SCK_TUTS.h's `_di_SIP1Soap`.
//
//  GOLDEN QUIRK preserved VERBATIM, not fixed (identical shape to SCK_TUTS.h):
//  the `#endif // __MesWebService_h__` at golden :48 closes the `#ifndef
//  MesWebServiceH` include guard (golden :10) *before* the `namespace
//  NS_MesWebService { ... }` block's own closing brace at golden :49 -- the
//  `};` at :49 and `using namespace NS_MesWebService;` at :51-53 sit OUTSIDE
//  the MesWebServiceH include guard, a latent double-inclusion hazard. Same
//  precedent, same reasoning, same "not exercised twice in this TU" mitigation
//  as SCK_TUTS.h (MesWebService.cpp only ever includes this header once,
//  guarded by `#if !defined(MesWebServiceH)`).
//
//  VCL/Borland conversions: golden :13-16 `#include <System.hpp>` /
//  <InvokeRegistry.hpp> / <XSBuiltIns.hpp> / <SoapHTTPClient.hpp> dropped --
//  same reason as SCK_TUTS.h. No __fastcall / __property / __published
//  anywhere in golden's own declarations here.
//
//  Big5: no Chinese text anywhere in this file. Final gate: ZERO U+FFFD.
// =============================================================================
// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2022/11/09 10:53:30 - $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //

#ifndef   MesWebServiceH
#define   MesWebServiceH

#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W2) 20260807: was <System.hpp>/<InvokeRegistry.hpp>/<XSBuiltIns.hpp>/<SoapHTTPClient.hpp> -- see the file head / SCK_TUTS.h for why none of the COM/SOAP machinery survives

namespace NS_MesWebService {
// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Borland types; however, they could also
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"
// !:string          - "http://tempuri.org/"

// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// binding   : MesWebServiceSoap
// service   : MesWebService
// port      : MesWebServiceSoap
// URL       : http://192.168.10.216/vt_mes/MesWebService.asmx
// ************************************************************************ //
//AI(W906-PT-W2) 20260807: golden ":38 __interface INTERFACE_UUID(\"{...}\") MesWebServiceSoap : public IInvokable" -> plain abstract base class, no COM identity/refcounting. See the file head's structural-change note (and SCK_TUTS.h for the full grep evidence).
class MesWebServiceSoap
{
public:
  virtual ~MesWebServiceSoap() {}
  virtual AnsiString      GetLotAttributes(const AnsiString LotNum) = 0;
  virtual AnsiString      CreateManualEOCAP(const AnsiString CustCode, const AnsiString CustPart, const AnsiString CustLot, const AnsiString InternalLot, const AnsiString StepCode, const AnsiString WaferID, const AnsiString RuleName, const AnsiString IssueDesc, const AnsiString SetBy) = 0;
};
typedef MesWebServiceSoap* _di_MesWebServiceSoap;                              //AI(W906-PT-W2) 20260807: golden ":44 typedef DelphiInterface<MesWebServiceSoap> _di_MesWebServiceSoap;" -> raw pointer, no COM refcounting (see file head)

_di_MesWebServiceSoap GetMesWebServiceSoap(bool useWSDL=false, AnsiString addr="");

#endif // __MesWebService_h__ -- AI(W906-PT-W2) 20260807: golden's own mislabelled include-guard #endif, kept exactly where golden put it (see GOLDEN QUIRK above)
};     // NS_MesWebService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_MesWebService;
#endif
