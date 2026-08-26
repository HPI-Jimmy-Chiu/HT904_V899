//---------------------------------------------------------------------------

#ifndef SCK_WebServiceH
#define SCK_WebServiceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "SCK_TUTS.h"
#include "MesWebService.h"
#include <Rio.hpp>
#include <SOAPHTTPClient.hpp>
//---------------------------------------------------------------------------
class TfWebService : public TForm
{
__published:    // IDE-managed Components
        THTTPRIO *HTTPRIO1;
    THTTPRIO *HTTPRIO_VTEST;
private:    // User declarations
public:     // User declarations
        __fastcall TfWebService(TComponent* Owner);

    _di_SIP1Soap FSIPService;                                                   //15.09.21.02 Mathew s
    _di_SIP1Soap GetFSIPServiceSoap();
    __property _di_SIP1Soap SIPService={read=GetFSIPServiceSoap};               //Steven 20161101 : SCK SOAP

    _di_MesWebServiceSoap FSIPService_VTEST;                                    //15.09.21.02 Mathew s
    _di_MesWebServiceSoap GetFSIPServiceSoap_VTEST();
    __property _di_MesWebServiceSoap SIPServiceSoap_VTEST={read=GetFSIPServiceSoap_VTEST}; //Steven 20161101 : SCK SOAP
};
//---------------------------------------------------------------------------
extern PACKAGE TfWebService *fWebService;
//---------------------------------------------------------------------------
#endif
