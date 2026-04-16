// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2016-10-31 ¿ÀÈÄ 4:27:29 - $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //

#ifndef   SCK_TUTSH
#define   SCK_TUTSH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <SoapHTTPClient.hpp>

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
    __interface INTERFACE_UUID("{4A32CEB8-8260-53EA-F953-5D05A880B4F2}") SIP1Soap : public IInvokable
    {
        public:
            virtual AnsiString      HelloWorld() = 0;
            virtual AnsiString      GetProgram(const AnsiString strLot) = 0;
            virtual AnsiString      GetProgram1(const AnsiString strLot, const AnsiString strHandler, const AnsiString strMap) = 0;
            virtual AnsiString      GetProgram2(const AnsiString strLot, const AnsiString strHandler, const AnsiString strMap) = 0;
            virtual AnsiString      EndLot(const AnsiString LOTID) = 0;
            virtual AnsiString      InputJam(const AnsiString strMessage) = 0;
    };
    typedef DelphiInterface<SIP1Soap> _di_SIP1Soap;

    _di_SIP1Soap GetSIP1Soap(bool useWSDL=false, AnsiString addr="");
    #endif // __sip1_h__
};     // NS_sip1

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace NS_sip1;
#endif

