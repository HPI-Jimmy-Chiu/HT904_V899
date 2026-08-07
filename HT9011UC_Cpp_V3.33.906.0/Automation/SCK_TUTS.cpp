// =============================================================================
//  SCK_TUTS.cpp  --  BCB6 WSDL-Importer-generated SOAP client stub bodies for
//                    the "SIP1" web service (GetSIP1Soap factory + the
//                    RegTypes SOAP-RIO type/interface registration routine).
//
//  Faithful translation of golden Automation/SCK_TUTS.cpp (60 lines, BCB6,
//  Big5/cp950 -- except the same corrupted 4-byte WSDL-Importer timestamp
//  fragment documented in SCK_TUTS.h's banner; golden :7 here is byte-for-
//  byte identical to golden SCK_TUTS.h:7, so it is not re-analysed here --
//  see SCK_TUTS.h's GOLDEN BYTE-LEVEL DEFECT section for the full derivation).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  ROLE: two functions, both operating on Borland's SOAP-RIO COM machinery
//  (THTTPRIO / InvRegistry() / __interfaceTypeinfo), none of which has any
//  substrate anywhere in this port tree (see SCK_TUTS.h's structural-change
//  note for the full grep evidence -- __interface/IInvokable/SoapHTTPClient/
//  THTTPRIO/DelphiInterface/InvokeRegistry occur nowhere else in the tree
//  except a generated layout-tool artifact, and this file's own caller,
//  SCK_WebService.cpp, is out of this wave's scope and unreached from any
//  translated unit as far as this wave can verify).
//
//  WAVE SCOPE -- ACTIVE vs WHOLE-FUNCTION-GATED
//  --------------------------------------------------------------------------
//   GetSIP1Soap(bool,AnsiString)   golden :20-44.  WHOLE-FUNCTION GATED.
//     Constructs a `new THTTPRIO(0)`, wires WSDLLocation/Service/Port or URL,
//     then `rio->QueryInterface(service)` to obtain the `_di_SIP1Soap` COM
//     interface pointer -- every one of THTTPRIO/QueryInterface/the COM
//     interface-pointer protocol is Borland SOAP-RIO machinery with no
//     translated equivalent (see file head). Golden body preserved verbatim
//     inside `#if 0`; ACTIVE arm returns `NULL` (a `_di_SIP1Soap` is a
//     nullable raw pointer per SCK_TUTS.h's typedef) -- the same "no SOAP
//     client can be constructed offline" outcome a real caller would also see
//     if THTTPRIO's own QueryInterface failed (golden itself already handles
//     that: :41-42 `if(!service) delete rio;`, i.e. golden's own contract
//     already treats a null return as an expected, handled outcome, not an
//     exceptional one). SAME whole-function-gate idiom this tree already
//     applies to cMyDB.cpp's TChart-dependent MyDBVUnitEventCount /
//     MyDBVAxleEventCount (cMyDB.cpp:1421-1443) for an analogous "the VCL
//     component this function is built on has no port" situation.
//     BEHAVIOUR DELTA, STATED PLAINLY: on a real machine, when SCK_WebService
//     (or any future caller) calls GetSIP1Soap(), golden either returns a live
//     SOAP client bound to http://10.20.20.32/cimreport/ws/sip1.asmx or NULL
//     if QueryInterface failed; this port always returns NULL, so the SIP1
//     SOAP program-download path is unavailable offline. Nothing in this
//     wave's scope calls GetSIP1Soap (grepped, see file head), so this has no
//     observable effect on anything this wave delivers.
//   RegTypes()                     golden :50-56.  WHOLE-FUNCTION GATED.
//     Registers SIP1Soap's interface/SOAPAction/invoke-options with the
//     process-wide SOAP-RIO `InvRegistry()` singleton so THTTPRIO can later
//     resolve method calls against it via reflection
//     (`__interfaceTypeinfo(SIP1Soap)`). InvRegistry()/__interfaceTypeinfo/
//     ioDocument have no port. Golden body preserved verbatim inside `#if 0`;
//     ACTIVE arm is an empty function body -- there is no registry to
//     populate, and (see below) nothing calls this function offline either,
//     so an empty body changes nothing observable.
//     golden :57 `#pragma startup RegTypes 32` is a BCB6-only directive that
//     schedules a plain (non-class, non-__fastcall) function to run once at
//     module-load time, priority 32, before main() -- there is no portable
//     equivalent and MinGW g++ does not recognise `#pragma startup` (it is
//     silently ignored by the preprocessor as an unknown pragma, per the C++
//     standard's pragma-ignoring rule, so this is not a compile error, but it
//     is also not a "translation" of the directive -- there is nothing
//     analogous to call it from). DROPPED, not reproduced-as-comment-only:
//     unlike `#pragma hdrstop` / `#pragma package(smart_init)` (which this
//     tree's convention already drops from every unit as pure BCB6 build-
//     system noise), `#pragma startup` had REAL RUNTIME EFFECT in golden
//     (it is how RegTypes ever got called AT ALL -- golden has no other call
//     site for it either, grepped the whole golden Automation/ tree). Because
//     RegTypes' own body is gated to empty anyway (nothing to register
//     against), losing the auto-invocation is behaviour-neutral here, but it
//     is flagged explicitly rather than silently folded into the routine
//     "drop hdrstop-class pragmas" treatment, since this one is a different
//     KIND of pragma (scheduling, not build-system metadata).
//
//  GATE REGISTER: 2 whole-function gates, both re-cited above and at their own
//  call site below (this tree's convention for a gate large enough to cover
//  an entire function body rather than one call within a larger function).
//
//  VCL/Borland conversions: #pragma hdrstop dropped (golden :12); golden has
//  no #pragma package in this .cpp (SOAP-RIO units use #pragma startup
//  instead, handled above). `#if !defined(SCK_TUTSH) #include "SCK_TUTS.h"
//  #endif` (golden :14-16) kept verbatim -- it is itself golden's own
//  functioning re-inclusion guard for a header whose OWN internal guard is
//  the one with the placement quirk (see SCK_TUTS.h's GOLDEN QUIRK); this
//  wrapper is what keeps THIS file safe from that quirk (SCK_TUTS.h is
//  therefore included at most once in this translation unit). No __fastcall /
//  __property / __published survive (none in golden's own bodies here either
//  -- COM interface dispatch uses plain virtual calls even in BCB6). No
//  AnsiString-vs-std::string substitution needed: golden's own locals
//  (defWSDL/defURL/defSvc/defPrt) are already plain `const char*`, kept as-is
//  inside the gated arm. SOFT_SIMULTE is not referenced by this unit.
//
//  Big5: no Chinese prose in this file besides the banner timestamp fragment
//  already covered in SCK_TUTS.h's banner (byte-for-byte identical, not
//  re-derived here). Final gate: ZERO U+FFFD.
// =============================================================================
// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2016-10-31 [bytes 0xBF 0xC0 0xC8 0xC4 -- undecodable as Big5/cp950 in
//  golden itself; see SCK_TUTS.h's GOLDEN BYTE-LEVEL DEFECT] 4:27:29 -
//  $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //
//Steven 20161101 : SCK SOAP

#include "MachineDefine.h"

#if !defined(SCK_TUTSH)
#include "SCK_TUTS.h"
#endif

namespace NS_sip1
{
    _di_SIP1Soap GetSIP1Soap(bool useWSDL, AnsiString addr)
    {
        // GATE (whole-function): golden :20-44 -- see file head. THTTPRIO /
        // QueryInterface / the COM interface-pointer protocol have no port.
#if 0 // TODO(SOAP-RIO): no THTTPRIO / QueryInterface substrate -- see file head.
        static const char* defWSDL= "http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL";
        static const char* defURL = "http://10.20.20.32/cimreport/ws/sip1.asmx";
        static const char* defSvc = "SIP1";
        static const char* defPrt = "SIP1Soap";
        if(addr=="")
            addr = useWSDL ? defWSDL : defURL;
        THTTPRIO* rio = new THTTPRIO(0);
        if (useWSDL)
        {
            rio->WSDLLocation = addr;
            rio->Service = defSvc;
            rio->Port = defPrt;
        }
        else
        {
            rio->URL = addr;
        }
        _di_SIP1Soap service;
        rio->QueryInterface(service);
        if(!service)
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
        // GATE (whole-function): golden :50-56 -- see file head. InvRegistry() /
        // __interfaceTypeinfo / ioDocument have no port, and golden's own
        // `#pragma startup RegTypes 32` (the only thing that ever called this
        // function) has no portable equivalent and is dropped, not reproduced.
#if 0 // TODO(SOAP-RIO): no InvRegistry()/__interfaceTypeinfo substrate -- see file head.
        /* SIP1Soap */
        InvRegistry()->RegisterInterface(__interfaceTypeinfo(SIP1Soap), L"http://tempuri.org/", L"utf-8");
        InvRegistry()->RegisterDefaultSOAPAction(__interfaceTypeinfo(SIP1Soap), L"http://tempuri.org/%operationName%");
        InvRegistry()->RegisterInvokeOptions(__interfaceTypeinfo(SIP1Soap), ioDocument);//15.09.21.01 kirin
#endif
    }
    //AI(W906-PT-W2) 20260807: golden ":57 #pragma startup RegTypes 32" dropped -- see file head. RegTypes is therefore unused offline (its own body is gated empty too), same as golden's registration would be a no-op once THTTPRIO/InvRegistry never exist to be registered against.
};     // NS_sip1
