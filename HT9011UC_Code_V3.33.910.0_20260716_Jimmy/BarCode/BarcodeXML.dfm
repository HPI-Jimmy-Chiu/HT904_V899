object frmXml: TfrmXml
  Left = 313
  Top = 120
  Width = 180
  Height = 83
  Caption = 'Xml'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object IdHTTPMESSystem: TIdHTTP
    Request.Accept = 'text/html, */*'
    Request.ContentLength = 0
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.ProxyPort = 0
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    Left = 36
    Top = 8
  end
  object XMLDocument1: TXMLDocument
    Left = 72
    Top = 8
    DOMVendorDesc = 'Open XML'
  end
end
