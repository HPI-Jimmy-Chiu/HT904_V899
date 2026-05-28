//---------------------------------------------------------------------------

#ifndef BarcodeXMLH
#define BarcodeXMLH
//---------------------------------------------------------------------------
//#include <math.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <oxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
//---------------------------------------------------------------------------
class TfrmXml : public TForm
{
__published:    // IDE-managed Components
    TIdHTTP *IdHTTPMESSystem;
    TXMLDocument *XMLDocument1;
private:    // User declarations
public:     // User declarations
    __fastcall TfrmXml(TComponent* Owner);
    void PeocessXML(int iSht);                                                  //Steven 20200409 : Murata 2DID比對功能
    int SendHTTPRequest(int iSht);
    int SendTestResultToHttp();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmXml *frmXml;
//---------------------------------------------------------------------------
#endif
