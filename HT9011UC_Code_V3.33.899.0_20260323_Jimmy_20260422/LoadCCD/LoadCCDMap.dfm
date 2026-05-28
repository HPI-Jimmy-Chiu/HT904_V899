object LoadCCD: TLoadCCD
  Left = 354
  Top = 257
  Width = 759
  Height = 603
  Caption = 'LoadCCD'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 751
    Height = 572
    Align = alClient
    Caption = 'Panel1'
    Color = 12761254
    TabOrder = 0
    object Label1: TLabel
      Left = 3
      Top = 24
      Width = 19
      Height = 13
      Caption = 'Port'
    end
    object LedAConnect: TMyLed
      Left = 331
      Top = 13
      Width = 23
      Height = 23
      Hint = 'Connection'
      Interval = 500
      LEDStyle = LEDSqLarge
      ShowHint = True
    end
    object edtPort: TEdit
      Left = 51
      Top = 16
      Width = 121
      Height = 21
      TabOrder = 1
      Text = '5000'
      OnClick = edtPortClick
    end
    object btnConnect: TButton
      Left = 191
      Top = 12
      Width = 75
      Height = 29
      Caption = 'Connect'
      TabOrder = 0
      OnClick = btnConnectClick
    end
    object Memo1: TMemo
      Left = 8
      Top = 56
      Width = 601
      Height = 497
      Color = 14670284
      Lines.Strings = (
        'Memo1')
      TabOrder = 3
    end
    object CCD1: TButton
      Left = 624
      Top = 24
      Width = 75
      Height = 25
      Caption = 'CCD1'
      TabOrder = 2
      OnClick = CCD1Click
    end
    object CCD2: TButton
      Left = 624
      Top = 64
      Width = 75
      Height = 25
      Caption = 'CCD2'
      TabOrder = 4
      OnClick = CCD2Click
    end
    object btnCCDTest: TButton
      Left = 624
      Top = 100
      Width = 75
      Height = 25
      Caption = 'btnCCDTest'
      TabOrder = 5
      OnClick = btnCCDTestClick
    end
  end
  object ServerSocket1: TServerSocket
    Active = False
    Port = 5000
    ServerType = stNonBlocking
    OnClientConnect = ServerSocket1ClientConnect
    OnClientDisconnect = ServerSocket1ClientDisconnect
    OnClientRead = ServerSocket1ClientRead
    Left = 440
    Top = 8
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 632
    Top = 136
  end
end
