object ASESendMessage: TASESendMessage
  Left = 472
  Top = 216
  Width = 923
  Height = 490
  Caption = 'ASESendMessage'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 432
    Height = 451
    ActivePage = Setup
    Align = alLeft
    TabIndex = 1
    TabOrder = 0
    object Commucation: TTabSheet
      Caption = 'Commucation'
      object btnTest: TButton
        Left = 280
        Top = 0
        Width = 75
        Height = 25
        Caption = 'Test'
        TabOrder = 0
        OnClick = btnTestClick
      end
      object edtTestMsg: TEdit
        Left = 4
        Top = 4
        Width = 265
        Height = 21
        TabOrder = 1
        Text = 'edtTestMsg'
      end
      object edtRecvMsg: TEdit
        Left = 4
        Top = 40
        Width = 265
        Height = 21
        TabOrder = 2
        Text = '@e01001Schedule=ABCDEFG,InQty=654'
      end
      object btnTestRecv: TButton
        Left = 280
        Top = 36
        Width = 75
        Height = 25
        Caption = 'btnTestRecv'
        TabOrder = 3
        OnClick = btnTestRecvClick
      end
    end
    object Setup: TTabSheet
      Caption = 'Setup'
      ImageIndex = 1
      object Label1: TLabel
        Left = 35
        Top = 3
        Width = 54
        Height = 13
        Caption = 'IP Address '
      end
      object Label24: TLabel
        Left = 3
        Top = 23
        Width = 27
        Height = 13
        Caption = '1       '
      end
      object Label2: TLabel
        Left = 187
        Top = 3
        Width = 58
        Height = 13
        Caption = 'TCP/IP Port'
      end
      object Label3: TLabel
        Left = 191
        Top = 243
        Width = 58
        Height = 13
        Caption = 'TCP/IP Port'
      end
      object IPAddress1: TEdit
        Left = 35
        Top = 19
        Width = 121
        Height = 21
        ImeMode = imClose
        TabOrder = 1
        Text = '127.0.0.1'
      end
      object IPPort1: TEdit
        Left = 187
        Top = 19
        Width = 121
        Height = 21
        TabOrder = 2
        Text = '9200'
        OnClick = IPPort2Click
      end
      object Connect: TButton
        Left = 320
        Top = 16
        Width = 100
        Height = 25
        Caption = 'Client Connect'
        TabOrder = 0
        OnClick = ConnectClick
      end
      object SaveData: TButton
        Left = 320
        Top = 48
        Width = 100
        Height = 25
        Caption = 'SaveData'
        TabOrder = 3
        OnClick = SaveDataClick
      end
      object Button2: TButton
        Left = 316
        Top = 256
        Width = 100
        Height = 25
        Caption = 'Server Connect'
        TabOrder = 4
        OnClick = Button2Click
      end
      object IPPort2: TEdit
        Left = 191
        Top = 259
        Width = 121
        Height = 21
        ImeMode = imClose
        TabOrder = 5
        Text = '9150'
        OnClick = IPPort2Click
      end
    end
  end
  object Memo1: TMemo
    Left = 432
    Top = 0
    Width = 475
    Height = 451
    Align = alClient
    Color = 14670284
    TabOrder = 1
  end
  object NMUDP1: TNMUDP
    RemoteHost = '127.0.0.1'
    RemotePort = 9001
    LocalPort = 9000
    ReportLevel = 1
    OnDataReceived = NMUDP1DataReceived
    Left = 376
  end
  object NMUDP2: TNMUDP
    RemoteHost = '127.0.0.1'
    RemotePort = 9001
    LocalPort = 9000
    ReportLevel = 1
    OnDataReceived = NMUDP1DataReceived
    Left = 404
  end
end
