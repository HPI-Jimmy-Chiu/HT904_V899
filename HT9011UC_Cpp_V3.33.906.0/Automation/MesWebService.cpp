// =============================================================================
//  MesWebService.cpp  --  BCB6 WSDL-Importer-generated SOAP client stub
//                         bodies for the "MesWebService" web service
//                         (GetMesWebServiceSoap factory + the RegTypes
//                         SOAP-RIO type/interface registration routine).
//
//  Faithful translation of golden Automation/MesWebService.cpp (59 lines,
//  BCB6, Big5/cp950 -- no Chinese text, no byte-level corruption).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//  Sibling of Automation/SCK_TUTS.cpp (SAME wave) -- an IDENTICAL
//  WSDL-Importer template around a different service contract. Every
//  structural decision below is the one already made and justified in full
//  in SCK_TUTS.cpp's banner; this banner cross-references rather than
//  re-deriving the same evidence.
//
//  ROLE: two functions, both operating on Borland's SOAP-RIO COM machinery
//  (THTTPRIO / InvRegistry() / __interfaceTypeinfo), with no substrate
//  anywhere in this port tree (see MesWebService.h / SCK_TUTS.h for the full
//  grep evidence). This file's own caller, SCK_WebService.cpp, is out of this
//  wave's scope and unreached from any translated unit as far as this wave
//  can verify.
//
//  WAVE SCOPE -- ACTIVE vs WHOLE-FUNCTION-GATED
//  --------------------------------------------------------------------------
//   GetMesWebServiceSoap(bool,AnsiString)   golden :18-43.  WHOLE-FUNCTION
//     GATED. Same THTTPRIO/QueryInterface shape as SCK_TUTS.cpp's
//     GetSIP1Soap, PLUS two extra UTF-8-header lines with no counterpart in
//     SCK_TUTS.cpp (golden :28-29: `rio->HTTPWebNode->UseUTF8InHeader=true;`
//     and `rio->Converter->Options<<soUTF8InHeader;`, both 15.09.21.01 kirin)
//     -- still entirely inside the gated THTTPRIO construction, so they need
//     no separate treatment. Golden body preserved verbatim inside `#if 0`;
//     ACTIVE arm returns `NULL` (a `_di_MesWebServiceSoap` is a nullable raw
//     pointer per MesWebService.h's typedef) -- the same "no SOAP client can
//     be constructed offline" outcome a real caller would also see if
//     THTTPRIO's own QueryInterface failed (golden's own contract already
//     treats a null return as expected: :40-41 `if(!service) delete rio;`).
//     SAME whole-function-gate idiom as cMyDB.cpp:1421-1443 (TChart-dependent
//     functions) and SCK_TUTS.cpp:GetSIP1Soap.
//     BEHAVIOUR DELTA, STATED PLAINLY: identical shape to SCK_TUTS.cpp's --
//     the MES lot-attribute lookup / manual EOCAP path is unavailable
//     offline; nothing in this wave's scope calls GetMesWebServiceSoap
//     (grepped, see header), so this has no observable effect on anything
//     this wave delivers.
//   RegTypes()                              golden :49-55.  WHOLE-FUNCTION
//     GATED, identical reasoning to SCK_TUTS.cpp's RegTypes: InvRegistry()/
//     __interfaceTypeinfo/ioDocument have no port; golden body preserved
//     verbatim inside `#if 0`; ACTIVE arm is an empty function body.
//     golden :56 `#pragma startup RegTypes 32` DROPPED for the same reason
//     documented in full in SCK_TUTS.cpp's banner (a scheduling directive
//     with real runtime effect in golden, not build-system noise like
//     #pragma hdrstop -- there is nothing analogous to call RegTypes from,
//     and its own body is gated empty anyway, so losing the auto-invocation
//     is behaviour-neutral here).
//
//  GATE REGISTER: 2 whole-function gates, both re-cited above and at their own
//  call site below (same convention SCK_TUTS.cpp uses for a gate large enough
//  to cover an entire function body).
//
//  VCL/Borland conversions: #pragma hdrstop dropped (golden :11); golden has
//  no #pragma package in this .cpp (SOAP-RIO units use #pragma startup
//  instead, handled above). `#if !defined(MesWebServiceH) #include
//  "MesWebService.h" #endif` (golden :13-15) kept verbatim -- golden's own
//  functioning re-inclusion guard, which is what keeps THIS file safe from
//  MesWebService.h's own include-guard placement quirk (so the header is
//  included at most once in this translation unit). No __fastcall /
//  __property / __published survive (none in golden's own bodies here
//  either). No AnsiString-vs-std::string substitution needed: golden's own
//  locals (defWSDL/defURL/defSvc/defPrt) are already plain `const char*`,
//  kept as-is inside the gated arm. SOFT_SIMULTE is not referenced by this
//  unit.
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

#include "MachineDefine.h"

#if !defined(MesWebServiceH)
#include "MesWebService.h"
#endif

namespace NS_MesWebService {
_di_MesWebServiceSoap GetMesWebServiceSoap(bool useWSDL, AnsiString addr)
{
  // GATE (whole-function): golden :18-43 -- see file head. THTTPRIO /
  // QueryInterface / the COM interface-pointer protocol have no port.
#if 0 // TODO(SOAP-RIO): no THTTPRIO / QueryInterface substrate -- see file head.
  static const char* defWSDL= "http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL";
  static const char* defURL = "http://192.168.10.216/vt_mes/MesWebService.asmx";
  static const char* defSvc = "MesWebService";
  static const char* defPrt = "MesWebServiceSoap";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = new THTTPRIO(0);

  rio->HTTPWebNode->UseUTF8InHeader = true;//15.09.21.01 kirin
  rio->Converter->Options<<soUTF8InHeader; //15.09.21.01 kirin

  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_MesWebServiceSoap service;
  rio->QueryInterface(service);
  if (!service)
    delete rio;
  return service;
#else
  (void)useWSDL; (void)addr;
  return NULL;
#endif
}

// ************************************************************************ //
// This routine registers the interfaces and types used by invoke the SOAP
// Service.
// ************************************************************************ //
static void RegTypes()
{
  // GATE (whole-function): golden :49-55 -- see file head. InvRegistry() /
  // __interfaceTypeinfo / ioDocument have no port, and golden's own
  // `#pragma startup RegTypes 32` (the only thing that ever called this
  // function) has no portable equivalent and is dropped, not reproduced.
#if 0 // TODO(SOAP-RIO): no InvRegistry()/__interfaceTypeinfo substrate -- see file head.
  /* MesWebServiceSoap */
  InvRegistry()->RegisterInterface(__interfaceTypeinfo(MesWebServiceSoap), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__interfaceTypeinfo(MesWebServiceSoap), L"http://tempuri.org/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__interfaceTypeinfo(MesWebServiceSoap), ioDocument);//15.09.21.01 kirin
#endif
}
//AI(W906-PT-W2) 20260807: golden ":56 #pragma startup RegTypes 32" dropped -- see file head. RegTypes is therefore unused offline (its own body is gated empty too), same treatment as SCK_TUTS.cpp's RegTypes.
};     // NS_MesWebService
