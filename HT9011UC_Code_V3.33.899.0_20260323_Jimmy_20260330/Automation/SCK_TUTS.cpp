// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2016-10-31 ¿ÀÈÄ 4:27:29 - $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //
//Steven 20161101 : SCK SOAP

#include "MachineDefine.h"
#pragma hdrstop

#if !defined(SCK_TUTSH)
#include "SCK_TUTS.h"
#endif

namespace NS_sip1
{
    _di_SIP1Soap GetSIP1Soap(bool useWSDL, AnsiString addr)
    {
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
    }

    // ************************************************************************ //
    // This routine registers the interfaces and types used by invoke the SOAP
    // Service.
    // ************************************************************************ //
    static void RegTypes()
    {
        /* SIP1Soap */
        InvRegistry()->RegisterInterface(__interfaceTypeinfo(SIP1Soap), L"http://tempuri.org/", L"utf-8");
        InvRegistry()->RegisterDefaultSOAPAction(__interfaceTypeinfo(SIP1Soap), L"http://tempuri.org/%operationName%");
        InvRegistry()->RegisterInvokeOptions(__interfaceTypeinfo(SIP1Soap), ioDocument);//15.09.21.01 kirin
    }
    #pragma startup RegTypes 32

};     // NS_sip1

