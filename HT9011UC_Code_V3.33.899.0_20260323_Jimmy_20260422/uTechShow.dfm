object fTeachShow: TfTeachShow
  Left = 245
  Top = 222
  Width = 526
  Height = 224
  BorderIcons = []
  Caption = 'fTeachShow'
  Color = 14670284
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 40
    Width = 423
    Height = 20
    Caption = 'The manual motion motor to treats measures in the position.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 24
    Top = 80
    Width = 49
    Height = 20
    Caption = 'Save ?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Button1: TButton
    Left = 104
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Yes'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 328
    Top = 120
    Width = 75
    Height = 25
    Caption = 'No'
    TabOrder = 1
    OnClick = Button2Click
  end
end
