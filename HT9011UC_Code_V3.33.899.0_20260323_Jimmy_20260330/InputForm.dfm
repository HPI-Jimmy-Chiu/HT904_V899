object fInputForm: TfInputForm
  Left = 630
  Top = 318
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsSingle
  Caption = 'Input Form'
  ClientHeight = 118
  ClientWidth = 379
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 185
    Height = 20
    Caption = 'Please Keyin Number : '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Image1: TImage
    Left = 8
    Top = 132
    Width = 841
    Height = 741
    Center = True
    Stretch = True
  end
  object Edit1: TEdit
    Left = 40
    Top = 40
    Width = 305
    Height = 21
    TabOrder = 0
    OnClick = Edit1Click
    OnKeyPress = Edit1KeyPress
  end
  object Button1: TButton
    Left = 144
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Edit2: TEdit
    Left = 40
    Top = 44
    Width = 305
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object Edit3: TEdit
    Left = 40
    Top = 44
    Width = 305
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
end
