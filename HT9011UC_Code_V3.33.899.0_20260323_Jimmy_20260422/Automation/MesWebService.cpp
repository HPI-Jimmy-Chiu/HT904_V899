// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2022/11/09 10:53:30 - $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //

#include "MachineDefine.h"
#pragma hdrstop

#if !defined(MesWebServiceH)
#include "MesWebService.h"
#endif



namespace NS_MesWebService {

_di_MesWebServiceSoap GetMesWebServiceSoap(bool useWSDL, AnsiString addr)
{
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
}


// ************************************************************************ //
// This routine registers the interfaces and types used by invoke the SOAP
// Service.
// ************************************************************************ //
static void RegTypes()
{
  /* MesWebServiceSoap */
  InvRegistry()->RegisterInterface(__interfaceTypeinfo(MesWebServiceSoap), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__interfaceTypeinfo(MesWebServiceSoap), L"http://tempuri.org/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__interfaceTypeinfo(MesWebServiceSoap), ioDocument);//15.09.21.01 kirin
}
#pragma startup RegTypes 32

};     // NS_MesWebService

