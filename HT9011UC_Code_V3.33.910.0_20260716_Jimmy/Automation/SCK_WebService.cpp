//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "SCK_WebService.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfWebService *fWebService;
//---------------------------------------------------------------------------
__fastcall TfWebService::TfWebService(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
_di_SIP1Soap TfWebService::GetFSIPServiceSoap()                                 //Steven 20161101 : SCK SOAP
{
    if(!FSIPService)
    {
        HTTPRIO1->QueryInterface(FSIPService);
    }
    return FSIPService;
}
//---------------------------------------------------------------------------
_di_MesWebServiceSoap TfWebService::GetFSIPServiceSoap_VTEST()                  //jou 20221104 : VTest CreateManualEOCAP function;
{
    if(!FSIPService_VTEST)
    {
        HTTPRIO_VTEST->QueryInterface(FSIPService_VTEST);
    }
    return FSIPService_VTEST;
}
//---------------------------------------------------------------------------