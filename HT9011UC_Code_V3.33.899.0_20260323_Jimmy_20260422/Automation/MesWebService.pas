// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL
// Encoding : utf-8
// Version  : 1.0
// (2022/11/09 10:53:30 - $Revision:   1.0.1.0.1.82  $)
// ************************************************************************ //

unit MesWebService;

interface

uses InvokeRegistry, Types, XSBuiltIns;

type

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
  MesWebServiceSoap = interface(IInvokable)
  ['{1AD98610-A035-B772-EC66-B073FABE9254}']
    function  GetLotAttributes(const LotNum: String): String; stdcall;
    function  CreateManualEOCAP(const CustCode: String; const CustPart: String; const CustLot: String; const InternalLot: String; const StepCode: String; const WaferID: String; const RuleName: String; const IssueDesc: String; const SetBy: String): String; stdcall;
  end;

function GetMesWebServiceSoap(UseWSDL: Boolean=System.False; Addr: string=''): MesWebServiceSoap;


implementation
  uses SOAPHTTPClient;

function GetMesWebServiceSoap(UseWSDL: Boolean; Addr: string): MesWebServiceSoap;
const
  defWSDL = 'http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL';
  defURL  = 'http://192.168.10.216/vt_mes/MesWebService.asmx';
  defSvc  = 'MesWebService';
  defPrt  = 'MesWebServiceSoap';
var
  RIO: THTTPRIO;
begin
  Result := nil;
  if (Addr = '') then
  begin
    if UseWSDL then
      Addr := defWSDL
    else
      Addr := defURL;
  end;
  RIO := THTTPRIO.Create(nil);
  try
     if UseWSDL then
    begin
      RIO.WSDLLocation := Addr;
      RIO.Service := defSvc;
      RIO.Port := defPrt;
    end else
      RIO.URL := Addr;
    Result := (RIO as MesWebServiceSoap);
  finally
    if Result = nil then
      RIO.Free;
  end;
end;


initialization
  InvRegistry.RegisterInterface(TypeInfo(MesWebServiceSoap), 'http://tempuri.org/', 'utf-8');
  InvRegistry.RegisterDefaultSOAPAction(TypeInfo(MesWebServiceSoap), 'http://tempuri.org/%operationName%');

end.