object fLtcSensor: TfLtcSensor
  Left = 625
  Top = 206
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'LtcSensor'
  ClientHeight = 480
  ClientWidth = 961
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnClose: TPanel
    Left = 0
    Top = 441
    Width = 961
    Height = 39
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = btnCloseClick
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 961
    Height = 441
    Align = alClient
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object pnlTop: TPanel
      Left = 0
      Top = 0
      Width = 961
      Height = 73
      Align = alTop
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 0
      object ledSht2Servo: TALed
        Left = 364
        Top = 13
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object lblSht1Servo: TLabel
        Left = 188
        Top = 14
        Width = 66
        Height = 20
        Caption = 'Servo On'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object ledSht1Servo: TALed
        Left = 164
        Top = 13
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object lblSht2Servo: TLabel
        Left = 388
        Top = 14
        Width = 66
        Height = 20
        Caption = 'Servo On'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblLtcCount: TLabel
        Left = 164
        Top = 56
        Width = 297
        Height = 13
        AutoSize = False
        Caption = 'lblLtcCount'
      end
      object Label22: TLabel
        Left = 808
        Top = 40
        Width = 38
        Height = 13
        Caption = 'Label22'
        Visible = False
      end
      object Label23: TLabel
        Left = 732
        Top = 12
        Width = 38
        Height = 13
        Caption = 'Label23'
        Visible = False
      end
      object ledSht3Servo: TALed
        Left = 556
        Top = 13
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object lblSht3Servo: TLabel
        Left = 580
        Top = 14
        Width = 66
        Height = 20
        Caption = 'Servo On'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object btSetLtc: TButton
        Left = 84
        Top = 44
        Width = 75
        Height = 25
        Caption = 'Set'
        TabOrder = 5
        OnClick = btSetLtcClick
      end
      object btSh1Servo: TButton
        Left = 84
        Top = 12
        Width = 75
        Height = 25
        Caption = 'Shuttle1 Servo'
        TabOrder = 1
        OnClick = btSh1ServoClick
      end
      object btSh2Servo: TButton
        Left = 280
        Top = 12
        Width = 75
        Height = 25
        Caption = 'Shuttle2 Servo'
        TabOrder = 2
        OnClick = btSh2ServoClick
      end
      object BtnDellTest: TButton
        Left = 772
        Top = 8
        Width = 75
        Height = 25
        Caption = 'BtnDellTest'
        TabOrder = 0
        Visible = False
        OnClick = BtnDellTestClick
      end
      object ScrollBar1: TScrollBar
        Left = 684
        Top = 40
        Width = 121
        Height = 17
        Min = 10
        PageSize = 0
        Position = 10
        TabOrder = 4
        Visible = False
        OnChange = ScrollBar1Change
      end
      object btSh3Servo: TButton
        Left = 472
        Top = 12
        Width = 75
        Height = 25
        Caption = 'Shuttle3 Servo'
        TabOrder = 3
        OnClick = btSh3ServoClick
      end
    end
    object pnlCenter: TPanel
      Left = 0
      Top = 73
      Width = 961
      Height = 368
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      Color = 12761254
      TabOrder = 1
      object gbOutSh1Z: TGroupBox
        Left = 80
        Top = 4
        Width = 142
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 1 Z'
        TabOrder = 1
        object labOut1_Z1_Axis: TLabel
          Left = 12
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut1_Z2_Axis: TLabel
          Left = 80
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut1_Z1_Count: TLabel
          Left = 12
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut1_Z2_Count: TLabel
          Left = 80
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut1_1: TALed
          Left = 26
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object ledOut1_2: TALed
          Left = 94
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object btGetSh1Ltc: TButton
          Left = 8
          Top = 328
          Width = 129
          Height = 25
          Caption = 'Get'
          TabOrder = 2
          OnClick = btGetSh1LtcClick
        end
        object Memo_Out1Z1: TMemo
          Left = 7
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
        object Memo_Out1Z2: TMemo
          Left = 75
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 1
        end
      end
      object gbOutSh2Z: TGroupBox
        Left = 222
        Top = 4
        Width = 142
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 2 Z'
        TabOrder = 2
        object labOut2_Z1_Axis: TLabel
          Left = 12
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut2_Z2_Axis: TLabel
          Left = 80
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut2_Z1_Count: TLabel
          Left = 12
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut2_Z2_Count: TLabel
          Left = 80
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut2_1: TALed
          Left = 26
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object ledOut2_2: TALed
          Left = 94
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object Memo_Out2Z1: TMemo
          Left = 7
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
        object Memo_Out2Z2: TMemo
          Left = 75
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 1
        end
        object btGetSh2Ltc: TButton
          Left = 8
          Top = 328
          Width = 129
          Height = 25
          Caption = 'Get'
          TabOrder = 2
          OnClick = btGetSh2LtcClick
        end
      end
      object gbOutSh1Y: TGroupBox
        Left = 506
        Top = 4
        Width = 90
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 1 Y'
        TabOrder = 4
        object labOut1_Y_Axis: TLabel
          Left = 20
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut1_Y_Count: TLabel
          Left = 20
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut1_Y: TALed
          Left = 34
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object btGetSh1YLtc: TButton
          Left = 16
          Top = 328
          Width = 60
          Height = 25
          Caption = 'Get'
          TabOrder = 1
          OnClick = btGetSh1LtcClick
        end
        object Memo_Out1Y: TMemo
          Left = 15
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
      end
      object gbOutSh2Y: TGroupBox
        Left = 596
        Top = 4
        Width = 90
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 2 Y'
        TabOrder = 5
        object labOut2_Y_Axis: TLabel
          Left = 20
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut2_Y_Count: TLabel
          Left = 20
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut2_Y: TALed
          Left = 34
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object Memo_Out2Y: TMemo
          Left = 15
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
        object btGetSh2YLtc: TButton
          Left = 15
          Top = 328
          Width = 60
          Height = 25
          Caption = 'Get'
          TabOrder = 1
          OnClick = btGetSh2LtcClick
        end
      end
      object gbInSh1Y: TGroupBox
        Left = 776
        Top = 4
        Width = 90
        Height = 360
        Align = alLeft
        Caption = 'In Shuttle 1 Y'
        TabOrder = 7
        object labIn1_Y_Axis: TLabel
          Left = 20
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labIn1_Y_Count: TLabel
          Left = 20
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledIn1_Y: TALed
          Left = 34
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object btGetInSh1YLtc: TButton
          Left = 16
          Top = 328
          Width = 60
          Height = 25
          Caption = 'Get'
          TabOrder = 1
          OnClick = btGetSh1LtcClick
        end
        object Memo_In1Y: TMemo
          Left = 15
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
      end
      object gbInSh2Y: TGroupBox
        Left = 866
        Top = 4
        Width = 90
        Height = 360
        Align = alLeft
        Caption = 'In Shuttle 2 Y'
        TabOrder = 8
        object labIn2_Y_Axis: TLabel
          Left = 20
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labIn2_Y_Count: TLabel
          Left = 20
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledIn2_Y: TALed
          Left = 34
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object Memo_In2Y: TMemo
          Left = 15
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
        object btGetInSh2YLtc: TButton
          Left = 15
          Top = 328
          Width = 60
          Height = 25
          Caption = 'Get'
          TabOrder = 1
          OnClick = btGetSh2LtcClick
        end
      end
      object pnlCaption: TPanel
        Left = 4
        Top = 4
        Width = 76
        Height = 360
        Align = alLeft
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object Label9: TLabel
          Left = 8
          Top = 64
          Width = 39
          Height = 13
          Caption = 'Axis No.'
        end
        object Label10: TLabel
          Left = 8
          Top = 100
          Width = 57
          Height = 13
          Caption = 'Latch count'
        end
        object Label13: TLabel
          Left = 8
          Top = 28
          Width = 59
          Height = 13
          Caption = 'Latch Signal'
        end
      end
      object gbOutSh3Z: TGroupBox
        Left = 364
        Top = 4
        Width = 142
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 3 Z'
        TabOrder = 3
        object labOut3_Z1_Axis: TLabel
          Left = 12
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut3_Z2_Axis: TLabel
          Left = 80
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut3_Z1_Count: TLabel
          Left = 12
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut3_Z2_Count: TLabel
          Left = 80
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut3_1: TALed
          Left = 26
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object ledOut3_2: TALed
          Left = 94
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object Memo_Out3Z1: TMemo
          Left = 7
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
        object Memo_Out3Z2: TMemo
          Left = 75
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 1
        end
        object btGetSh3Ltc: TButton
          Left = 8
          Top = 328
          Width = 129
          Height = 25
          Caption = 'Get'
          TabOrder = 2
          OnClick = btGetSh3LtcClick
        end
      end
      object gbOutSh3Y: TGroupBox
        Left = 686
        Top = 4
        Width = 90
        Height = 360
        Align = alLeft
        Caption = 'Out Shuttle 3 Y'
        TabOrder = 6
        object labOut3_Y_Axis: TLabel
          Left = 20
          Top = 60
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object labOut3_Y_Count: TLabel
          Left = 20
          Top = 96
          Width = 50
          Height = 13
          AutoSize = False
          Caption = 'Counter'
        end
        object ledOut3_Y: TALed
          Left = 34
          Top = 19
          Width = 22
          Height = 22
          LEDStyle = LEDSqLarge
        end
        object btGetSh3YLtc: TButton
          Left = 16
          Top = 328
          Width = 60
          Height = 25
          Caption = 'Get'
          TabOrder = 1
        end
        object Memo_Out3Y: TMemo
          Left = 15
          Top = 136
          Width = 60
          Height = 189
          Color = 14670284
          TabOrder = 0
        end
      end
    end
  end
  object TimerLtcSensor: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerLtcSensorTimer
    Left = 932
  end
  object DellTest: TTimer
    Enabled = False
    Interval = 100
    OnTimer = DellTestTimer
    Left = 904
  end
end
