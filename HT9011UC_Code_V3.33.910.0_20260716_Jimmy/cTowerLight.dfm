object fTowerLight: TfTowerLight
  Left = 762
  Top = 160
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'TowerLight'
  ClientHeight = 530
  ClientWidth = 760
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 33
    Width = 760
    Height = 456
    Align = alClient
    BevelInner = bvLowered
    Color = 12761254
    TabOrder = 1
    object RGB50: TALed
      Tag = 15
      Left = 168
      Top = 260
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB40: TALed
      Tag = 12
      Left = 168
      Top = 219
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB30: TALed
      Tag = 9
      Left = 168
      Top = 177
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB20: TALed
      Tag = 6
      Left = 168
      Top = 136
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB10: TALed
      Tag = 3
      Left = 168
      Top = 95
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB00: TALed
      Left = 168
      Top = 54
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB01: TALed
      Tag = 1
      Left = 224
      Top = 55
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB11: TALed
      Tag = 4
      Left = 224
      Top = 95
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB21: TALed
      Tag = 7
      Left = 224
      Top = 136
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB31: TALed
      Tag = 10
      Left = 224
      Top = 177
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB41: TALed
      Tag = 13
      Left = 224
      Top = 219
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB51: TALed
      Tag = 16
      Left = 224
      Top = 260
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB52: TALed
      Tag = 17
      Left = 280
      Top = 260
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB42: TALed
      Tag = 14
      Left = 279
      Top = 218
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB32: TALed
      Tag = 11
      Left = 280
      Top = 177
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB22: TALed
      Tag = 8
      Left = 280
      Top = 136
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB12: TALed
      Tag = 5
      Left = 280
      Top = 95
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB02: TALed
      Tag = 2
      Left = 280
      Top = 54
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object Label2: TLabel
      Left = 164
      Top = 24
      Width = 29
      Height = 13
      Caption = 'Green'
    end
    object Label3: TLabel
      Left = 219
      Top = 24
      Width = 31
      Height = 13
      Caption = 'Yellow'
    end
    object Label4: TLabel
      Left = 281
      Top = 24
      Width = 20
      Height = 13
      Caption = 'Red'
    end
    object Label6: TLabel
      Left = 344
      Top = 24
      Width = 61
      Height = 13
      Caption = 'Music Select'
    end
    object Bevel1: TBevel
      Left = 177
      Top = 362
      Width = 7
      Height = 50
      Shape = bsLeftLine
    end
    object Bevel2: TBevel
      Left = 233
      Top = 362
      Width = 7
      Height = 50
      Shape = bsLeftLine
    end
    object Bevel3: TBevel
      Left = 289
      Top = 362
      Width = 7
      Height = 50
      Shape = bsLeftLine
    end
    object Bevel4: TBevel
      Left = 177
      Top = 362
      Width = 134
      Height = 51
      Shape = bsBottomLine
    end
    object Label43: TLabel
      Left = 316
      Top = 406
      Width = 74
      Height = 13
      Caption = 'Click to change'
    end
    object RGB60: TALed
      Tag = 18
      Left = 168
      Top = 300
      Width = 22
      Height = 22
      FalseColor = 22272
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB61: TALed
      Tag = 19
      Left = 224
      Top = 300
      Width = 22
      Height = 22
      TrueColor = clYellow
      FalseColor = 881034
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object RGB62: TALed
      Tag = 20
      Left = 280
      Top = 300
      Width = 22
      Height = 22
      TrueColor = clRed
      FalseColor = clMaroon
      Interval = 300
      LEDStyle = LEDSqLarge
      OnClick = RGB00Click
    end
    object Panel4: TPanel
      Left = 40
      Top = 48
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Running'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object Panel5: TPanel
      Left = 40
      Top = 89
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Error/Jam'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object Panel6: TPanel
      Left = 40
      Top = 130
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Pause'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object Panel7: TPanel
      Left = 40
      Top = 171
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Message'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
    end
    object Panel8: TPanel
      Left = 40
      Top = 213
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Heating'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
    end
    object Panel9: TPanel
      Left = 40
      Top = 254
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Homing'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
    end
    object cbRunning: TComboBox
      Left = 344
      Top = 50
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 1
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object cbJam: TComboBox
      Left = 344
      Top = 92
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 4
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object cbPause: TComboBox
      Left = 344
      Top = 133
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 6
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object cbMessage: TComboBox
      Left = 344
      Top = 174
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 8
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object cbHeating: TComboBox
      Left = 344
      Top = 216
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 10
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object cbHome: TComboBox
      Left = 344
      Top = 257
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 12
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object rgMusicTest: TRadioGroup
      Left = 520
      Top = 56
      Width = 185
      Height = 225
      Caption = 'Music Test'
      ItemIndex = 0
      Items.Strings = (
        '[ 0 ] Silent'
        '[ 1 ] Music 1'
        '[ 2 ] Music 2'
        '[ 3 ] Music 3'
        '[ 4 ] Music 4')
      TabOrder = 2
      OnClick = rgMusicTestClick
    end
    object Panel1: TPanel
      Left = 40
      Top = 294
      Width = 121
      Height = 33
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'OffLine Running'
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
    end
    object cbOffLine: TComboBox
      Left = 344
      Top = 297
      Width = 89
      Height = 21
      ImeName = ''
      ItemHeight = 13
      TabOrder = 14
      Text = 'Silent'
      Items.Strings = (
        'Silent'
        'Music 1'
        'Music 2'
        'Music 3'
        'Music 4')
    end
    object Panel11: TPanel
      Left = 40
      Top = 332
      Width = 405
      Height = 33
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 15
      object RGB72: TALed
        Tag = 23
        Left = 240
        Top = 8
        Width = 22
        Height = 22
        TrueColor = clRed
        FalseColor = clMaroon
        Interval = 300
        LEDStyle = LEDSqLarge
        OnClick = RGB00Click
      end
      object RGB71: TALed
        Tag = 22
        Left = 184
        Top = 8
        Width = 22
        Height = 22
        TrueColor = clYellow
        FalseColor = 881034
        Interval = 300
        LEDStyle = LEDSqLarge
        OnClick = RGB00Click
      end
      object RGB70: TALed
        Tag = 21
        Left = 128
        Top = 8
        Width = 22
        Height = 22
        FalseColor = 22272
        Interval = 300
        LEDStyle = LEDSqLarge
        OnClick = RGB00Click
      end
      object cbART: TComboBox
        Left = 304
        Top = 5
        Width = 89
        Height = 21
        ImeName = ''
        ItemHeight = 13
        TabOrder = 1
        Text = 'Silent'
        Items.Strings = (
          'Silent'
          'Music 1'
          'Music 2'
          'Music 3'
          'Music 4')
      end
      object palART: TPanel
        Left = 0
        Top = 2
        Width = 121
        Height = 33
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Caption = 'Auto Retest'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 760
    Height = 33
    Align = alTop
    BevelInner = bvLowered
    Caption = 'Tower light and  music setup'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object spbExit: TPanel
    Left = 0
    Top = 489
    Width = 760
    Height = 41
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = spbExitClick
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 516
    Top = 32
  end
end
