object CCInput: TCCInput
  Left = 252
  Top = 254
  Width = 584
  Height = 389
  BorderIcons = []
  Caption = 'Input'
  Color = 9534289
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton1: TSpeedButton
    Left = 380
    Top = 65
    Width = 86
    Height = 65
    Caption = '+100'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 380
    Top = 136
    Width = 86
    Height = 65
    Caption = '+10'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton3: TSpeedButton
    Left = 380
    Top = 207
    Width = 86
    Height = 65
    Caption = '+1'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton4: TSpeedButton
    Left = 286
    Top = 207
    Width = 86
    Height = 135
    Caption = 'Abort'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -29
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton4Click
  end
  object SpeedButton5: TSpeedButton
    Left = 473
    Top = 65
    Width = 86
    Height = 65
    Caption = '-100'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton6: TSpeedButton
    Left = 473
    Top = 136
    Width = 86
    Height = 65
    Caption = '-10'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton7: TSpeedButton
    Left = 473
    Top = 207
    Width = 86
    Height = 65
    Caption = '-1'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -32
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 553
    Height = 53
    AutoSize = False
    Caption = '0'
    Color = 12761254
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Button1: TButton
    Tag = 7
    Left = 6
    Top = 65
    Width = 86
    Height = 65
    Caption = '7'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = NumberClick
  end
  object Button2: TButton
    Tag = 4
    Left = 6
    Top = 136
    Width = 86
    Height = 65
    Caption = '4'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = NumberClick
  end
  object Button3: TButton
    Tag = 1
    Left = 6
    Top = 207
    Width = 86
    Height = 65
    Caption = '1'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = NumberClick
  end
  object Button5: TButton
    Tag = 8
    Left = 99
    Top = 65
    Width = 86
    Height = 65
    Caption = '8'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    OnClick = NumberClick
  end
  object Button6: TButton
    Tag = 5
    Left = 99
    Top = 136
    Width = 86
    Height = 65
    Caption = '5'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    OnClick = NumberClick
  end
  object Button8: TButton
    Tag = 9
    Left = 193
    Top = 65
    Width = 86
    Height = 65
    Caption = '9'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    OnClick = NumberClick
  end
  object Button9: TButton
    Tag = 6
    Left = 193
    Top = 136
    Width = 86
    Height = 65
    Caption = '6'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    OnClick = NumberClick
  end
  object Button11: TButton
    Tag = 3
    Left = 193
    Top = 207
    Width = 86
    Height = 65
    Caption = '3'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    OnClick = NumberClick
  end
  object Button12: TButton
    Tag = 2
    Left = 99
    Top = 207
    Width = 86
    Height = 65
    Caption = '2'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    OnClick = NumberClick
  end
  object Button15: TButton
    Left = 380
    Top = 278
    Width = 180
    Height = 64
    Caption = 'Enter'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 9
    OnClick = Button15Click
  end
  object Button17: TButton
    Left = 193
    Top = 278
    Width = 86
    Height = 65
    Caption = '.'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 10
    OnClick = Button17Click
  end
  object Button18: TButton
    Left = 6
    Top = 278
    Width = 179
    Height = 65
    Caption = '0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 11
    OnClick = NumberClick
  end
  object Button13: TButton
    Left = 286
    Top = 136
    Width = 86
    Height = 65
    Caption = 'BS'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -37
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 12
    OnClick = Button13Click
  end
  object Button4: TButton
    Left = 286
    Top = 65
    Width = 86
    Height = 65
    Caption = '-'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Trebuchet MS'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
    OnClick = Button4Click
  end
end
