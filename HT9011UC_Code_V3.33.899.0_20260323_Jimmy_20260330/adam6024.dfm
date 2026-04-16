object fAdam6024: TfAdam6024
  Left = 1611
  Top = 840
  Width = 346
  Height = 106
  Caption = 'ADAM'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 20
  object ClientSocket1: TClientSocket
    Active = False
    Address = '172.16.8.110'
    ClientType = ctNonBlocking
    Port = 10001
    OnConnect = ClientSocket1Connect
    OnError = ClientSocket1Error
    Left = 36
    Top = 16
  end
end
