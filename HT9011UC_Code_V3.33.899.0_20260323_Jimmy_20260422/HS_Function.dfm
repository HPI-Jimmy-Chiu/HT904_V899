object FormHS: TFormHS
  Left = 461
  Top = 265
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'ATC Self Test'
  ClientHeight = 214
  ClientWidth = 190
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object pnlSlfeTestRun: TPanel
    Left = 0
    Top = 0
    Width = 190
    Height = 217
    TabOrder = 0
    object Label1: TLabel
      Left = 5
      Top = 6
      Width = 180
      Height = 22
      AutoSize = False
      Caption = 'SELF TEST RUN STATE'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object pl_ATCSelfTestSatus_01: TPanel
      Tag = 5
      Left = 15
      Top = 30
      Width = 160
      Height = 41
      BevelInner = bvRaised
      BorderWidth = 1
      TabOrder = 0
      object Label2: TLabel
        Tag = 5
        Left = 25
        Top = 11
        Width = 113
        Height = 19
        Caption = 'Temperature(+5)'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object pl_ATCSelfTestSatus_02: TPanel
      Tag = 5
      Left = 15
      Top = 75
      Width = 160
      Height = 41
      BevelInner = bvRaised
      BorderWidth = 1
      TabOrder = 1
      object Label3: TLabel
        Tag = 5
        Left = 25
        Top = 11
        Width = 109
        Height = 19
        Caption = 'Temperature(-5)'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object pl_ATCSelfTestSatus_03: TPanel
      Tag = 5
      Left = 15
      Top = 120
      Width = 160
      Height = 41
      BevelInner = bvRaised
      BorderWidth = 1
      TabOrder = 2
      object Label4: TLabel
        Tag = 5
        Left = 25
        Top = 11
        Width = 113
        Height = 19
        Caption = 'Temperature(+0)'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object pl_ATCSelfTestSatus_99: TPanel
      Tag = 5
      Left = 15
      Top = 165
      Width = 160
      Height = 41
      BevelInner = bvRaised
      BorderWidth = 1
      TabOrder = 3
      object Label5: TLabel
        Tag = 5
        Left = 45
        Top = 11
        Width = 75
        Height = 19
        Caption = 'ATC STOP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
  end
  object TimerAutoBackup: TTimer
    Enabled = False
    OnTimer = TimerAutoBackupTimer
    Top = 160
  end
  object ESDServerSocket: TServerSocket
    Active = False
    Port = 6666
    ServerType = stNonBlocking
    ThreadCacheSize = 1000
    OnClientConnect = ESDServerSocketClientConnect
    OnClientRead = ESDServerSocketClientRead
    OnClientError = ESDServerSocketClientError
    Top = 128
  end
  object RTMServerSocket: TServerSocket
    Active = False
    Port = 6000
    ServerType = stNonBlocking
    OnClientConnect = RTMServerSocketClientConnect
    OnClientDisconnect = RTMServerSocketClientDisconnect
    OnClientRead = RTMServerSocketClientRead
    OnClientError = RTMServerSocketClientError
    Top = 96
  end
  object TimerRTMMsg: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerRTMMsgTimer
    Left = 28
    Top = 160
  end
  object HandlerClientSocket: TClientSocket
    Active = False
    Address = '127.0.0.1'
    ClientType = ctNonBlocking
    Port = 1023
    OnConnect = HandlerClientSocketConnect
    OnRead = HandlerClientSocketRead
    OnError = HandlerClientSocketError
    Left = 168
    Top = 120
  end
end
