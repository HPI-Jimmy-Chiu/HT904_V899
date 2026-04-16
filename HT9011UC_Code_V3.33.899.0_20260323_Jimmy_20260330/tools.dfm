object FTool: TFTool
  Left = 809
  Top = 101
  BorderStyle = bsSingle
  Caption = 'Tool'
  ClientHeight = 568
  ClientWidth = 684
  Color = 12761254
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 684
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    Caption = 'Tools and Function'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 41
    Width = 684
    Height = 470
    ActivePage = TabSheet1
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'IO SET'
      object Label1: TLabel
        Left = 8
        Top = 98
        Width = 43
        Height = 20
        Caption = 'PORT'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object ALed1: TALed
        Left = 168
        Top = 133
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed2: TALed
        Left = 168
        Top = 165
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed3: TALed
        Left = 168
        Top = 197
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed4: TALed
        Left = 168
        Top = 229
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed5: TALed
        Left = 168
        Top = 261
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed6: TALed
        Left = 168
        Top = 293
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed7: TALed
        Left = 168
        Top = 325
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed8: TALed
        Left = 168
        Top = 357
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object Label2: TLabel
        Left = 232
        Top = 98
        Width = 43
        Height = 20
        Caption = 'PORT'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object ALed9: TALed
        Left = 392
        Top = 133
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed10: TALed
        Left = 392
        Top = 165
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed11: TALed
        Left = 392
        Top = 197
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed12: TALed
        Left = 392
        Top = 229
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed13: TALed
        Left = 392
        Top = 261
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed14: TALed
        Left = 392
        Top = 293
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed15: TALed
        Left = 392
        Top = 325
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object ALed16: TALed
        Left = 392
        Top = 357
        Width = 22
        Height = 22
        LEDStyle = LEDSqLarge
      end
      object Label3: TLabel
        Left = 464
        Top = 56
        Width = 145
        Height = 20
        Caption = 'Loop Output Interval'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 576
        Top = 96
        Width = 33
        Height = 20
        Caption = 'SEC'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object SpeedButton2: TSpeedButton
        Left = 448
        Top = 16
        Width = 161
        Height = 28
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Enable IO Change '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clPurple
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        OnClick = SpeedButton2Click
      end
      object Label5: TLabel
        Left = 8
        Top = 16
        Width = 36
        Height = 20
        Caption = 'Lane'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 232
        Top = 16
        Width = 36
        Height = 20
        Caption = 'Lane'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 8
        Top = 56
        Width = 15
        Height = 20
        Caption = 'IP'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 232
        Top = 56
        Width = 15
        Height = 20
        Caption = 'IP'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edtPort0: TEdit
        Left = 64
        Top = 96
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 0
        OnClick = edtPort0Click
        OnKeyPress = edtPort0KeyPress
      end
      object CheckBox1: TCheckBox
        Left = 8
        Top = 136
        Width = 65
        Height = 17
        Caption = 'Bit 0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object CheckBox2: TCheckBox
        Left = 80
        Top = 136
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object CheckBox3: TCheckBox
        Left = 8
        Top = 168
        Width = 65
        Height = 17
        Caption = 'Bit 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object CheckBox4: TCheckBox
        Left = 80
        Top = 168
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object CheckBox5: TCheckBox
        Left = 8
        Top = 200
        Width = 65
        Height = 17
        Caption = 'Bit 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object CheckBox6: TCheckBox
        Left = 80
        Top = 200
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object CheckBox7: TCheckBox
        Left = 8
        Top = 232
        Width = 65
        Height = 17
        Caption = 'Bit 3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object CheckBox8: TCheckBox
        Left = 80
        Top = 232
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object CheckBox9: TCheckBox
        Left = 8
        Top = 264
        Width = 65
        Height = 17
        Caption = 'Bit 4'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object CheckBox10: TCheckBox
        Left = 80
        Top = 264
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object CheckBox11: TCheckBox
        Left = 8
        Top = 296
        Width = 65
        Height = 17
        Caption = 'Bit 5'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
      end
      object CheckBox12: TCheckBox
        Left = 80
        Top = 296
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object CheckBox13: TCheckBox
        Left = 8
        Top = 328
        Width = 65
        Height = 17
        Caption = 'Bit 6'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object CheckBox14: TCheckBox
        Left = 80
        Top = 328
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
      end
      object CheckBox15: TCheckBox
        Left = 8
        Top = 360
        Width = 65
        Height = 17
        Caption = 'Bit 7'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object CheckBox16: TCheckBox
        Left = 80
        Top = 360
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
      object edtPort2: TEdit
        Left = 288
        Top = 96
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 17
        OnClick = edtPort0Click
        OnKeyPress = edtPort0KeyPress
      end
      object CheckBox17: TCheckBox
        Left = 232
        Top = 136
        Width = 57
        Height = 17
        Caption = 'Bit 0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
      end
      object CheckBox18: TCheckBox
        Left = 304
        Top = 136
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
      end
      object CheckBox19: TCheckBox
        Left = 304
        Top = 360
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
      end
      object CheckBox20: TCheckBox
        Left = 304
        Top = 328
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
      end
      object CheckBox21: TCheckBox
        Left = 304
        Top = 296
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
      end
      object CheckBox22: TCheckBox
        Left = 304
        Top = 264
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
      end
      object CheckBox23: TCheckBox
        Left = 304
        Top = 232
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
      end
      object CheckBox24: TCheckBox
        Left = 304
        Top = 200
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 25
      end
      object CheckBox25: TCheckBox
        Left = 304
        Top = 168
        Width = 57
        Height = 17
        Caption = 'Loop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 26
      end
      object CheckBox26: TCheckBox
        Left = 232
        Top = 168
        Width = 65
        Height = 17
        Caption = 'Bit 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 27
      end
      object CheckBox27: TCheckBox
        Left = 232
        Top = 200
        Width = 65
        Height = 17
        Caption = 'Bit 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 28
      end
      object CheckBox28: TCheckBox
        Left = 232
        Top = 232
        Width = 65
        Height = 17
        Caption = 'Bit 3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 29
      end
      object CheckBox29: TCheckBox
        Left = 232
        Top = 264
        Width = 65
        Height = 17
        Caption = 'Bit 4'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 30
      end
      object CheckBox30: TCheckBox
        Left = 232
        Top = 296
        Width = 65
        Height = 17
        Caption = 'Bit 5'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 31
      end
      object CheckBox31: TCheckBox
        Left = 232
        Top = 328
        Width = 65
        Height = 17
        Caption = 'Bit 6'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 32
      end
      object CheckBox32: TCheckBox
        Left = 232
        Top = 360
        Width = 65
        Height = 17
        Caption = 'Bit 7'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 33
      end
      object ComboBox1: TComboBox
        Left = 464
        Top = 96
        Width = 97
        Height = 24
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #26032#32048#26126#39636
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ItemHeight = 16
        ParentFont = False
        TabOrder = 34
        Text = '1'
        OnChange = ComboBox1Change
        Items.Strings = (
          '0.1'
          '0.3'
          '0.5'
          '0.7'
          '1'
          '2'
          '3')
      end
      object edtPort1: TEdit
        Left = 144
        Top = 96
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 35
        OnClick = edtPort0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtPort3: TEdit
        Left = 368
        Top = 96
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 36
        OnClick = edtPort0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtLane0: TEdit
        Left = 64
        Top = 14
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 37
        OnClick = edtLane0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtLane2: TEdit
        Left = 288
        Top = 14
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 38
        OnClick = edtLane0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtLane1: TEdit
        Left = 144
        Top = 14
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 39
        OnClick = edtLane0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtLane3: TEdit
        Left = 368
        Top = 14
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 40
        OnClick = edtLane0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtIP0: TEdit
        Left = 64
        Top = 54
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 41
        OnClick = edtIP0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtIP2: TEdit
        Left = 288
        Top = 54
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 42
        OnClick = edtIP0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtIP1: TEdit
        Left = 144
        Top = 54
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 43
        OnClick = edtIP0Click
        OnKeyPress = edtPort0KeyPress
      end
      object edtIP3: TEdit
        Left = 368
        Top = 54
        Width = 65
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 44
        OnClick = edtIP0Click
        OnKeyPress = edtPort0KeyPress
      end
    end
  end
  object sbExit: TPanel
    Left = 0
    Top = 511
    Width = 684
    Height = 57
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = sbExitClick
  end
  object Timer1: TTimer
    Interval = 50
    OnTimer = Timer1Timer
    Left = 544
    Top = 8
  end
end
