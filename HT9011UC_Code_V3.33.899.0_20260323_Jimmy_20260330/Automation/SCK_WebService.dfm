object fWebService: TfWebService
  Left = 543
  Top = 403
  Width = 220
  Height = 155
  Caption = 'fWebService'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object HTTPRIO1: THTTPRIO
    WSDLLocation = 'http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL'
    Service = 'SIP1'
    Port = 'SIP1Soap'
    HTTPWebNode.Agent = 'Borland SOAP 1.1'
    HTTPWebNode.UseUTF8InHeader = False
    HTTPWebNode.InvokeOptions = [soIgnoreInvalidCerts]
    Converter.Options = [soSendMultiRefObj, soTryAllSchema, soRootRefNodesToBody]
    Left = 20
    Top = 12
  end
  object HTTPRIO_VTEST: THTTPRIO
    WSDLLocation = 'http://192.168.10.216/vt_mes/MesWebService.asmx?WSDL'
    Service = 'MesWebService'
    Port = 'MesWebServiceSoap'
    HTTPWebNode.Agent = 'Borland SOAP 1.1'
    HTTPWebNode.UseUTF8InHeader = False
    HTTPWebNode.InvokeOptions = [soIgnoreInvalidCerts]
    Converter.Options = [soSendMultiRefObj, soTryAllSchema, soRootRefNodesToBody]
    Left = 84
    Top = 12
  end
end
