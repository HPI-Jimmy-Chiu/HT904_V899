object FormBarcodeReader: TFormBarcodeReader
  Left = 764
  Top = 212
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Barcode Reader'
  ClientHeight = 139
  ClientWidth = 368
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBarcodeReader: TPanel
    Left = 0
    Top = 0
    Width = 368
    Height = 139
    Align = alClient
    BevelInner = bvLowered
    Color = 6682876
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lblInputType: TLabel
      Left = 24
      Top = 17
      Width = 147
      Height = 24
      Caption = 'Input Operator ID:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 11359005
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object edtBarcodeNumber: TEdit
      Left = 24
      Top = 46
      Width = 321
      Height = 32
      PopupMenu = pmBarcode
      TabOrder = 0
      OnKeyDown = edtBarcodeNumberKeyDown
      OnKeyUp = edtBarcodeNumberKeyUp
    end
    object btnEnter: TButton
      Left = 88
      Top = 92
      Width = 193
      Height = 36
      Caption = 'Enter'
      TabOrder = 1
      OnClick = btnEnterClick
    end
  end
  object TimerKeyIn: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerKeyInTimer
    Left = 328
    Top = 104
  end
  object pmBarcode: TPopupMenu
    Left = 8
    Top = 104
    object miClear: TMenuItem
      Caption = 'Clear'
      OnClick = miClearClick
    end
    object miClose: TMenuItem
      Caption = 'Close'
      OnClick = miCloseClick
    end
  end
end
