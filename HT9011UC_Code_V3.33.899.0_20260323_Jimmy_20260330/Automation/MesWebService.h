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

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <SoapHTTPClient.hpp>


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
__interface INTERFACE_UUID("{1AD98610-A035-B772-EC66-B073FABE9254}") MesWebServiceSoap : public IInvokable
{
public:
  virtual AnsiString      GetLotAttributes(const AnsiString LotNum) = 0;
  virtual AnsiString      CreateManualEOCAP(const AnsiString CustCode, const AnsiString CustPart, const AnsiString CustLot, const AnsiString InternalLot, const AnsiString StepCode, const AnsiString WaferID, const AnsiString RuleName, const AnsiString IssueDesc, const AnsiString SetBy) = 0;
};
typedef DelphiInterface<MesWebServiceSoap> _di_MesWebServiceSoap;

_di_MesWebServiceSoap GetMesWebServiceSoap(bool useWSDL=false, AnsiString addr="");



#endif // __MesWebService_h__

};     // NS_MesWebService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_MesWebService;
#endif

