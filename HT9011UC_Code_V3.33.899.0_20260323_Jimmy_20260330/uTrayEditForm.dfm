object TrayEditForm: TTrayEditForm
  Left = 640
  Top = 45
  Width = 520
  Height = 699
  Caption = 'TrayEditForm'
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
  object spbUpdate: TSpeedButton
    Left = 224
    Top = 576
    Width = 137
    Height = 33
    Caption = 'Update'
    OnClick = spbUpdateClick
  end
  object SpeedButton2: TSpeedButton
    Left = 368
    Top = 576
    Width = 129
    Height = 33
    Caption = 'Abort'
    OnClick = SpeedButton2Click
  end
  object Label1: TLabel
    Left = 176
    Top = 636
    Width = 7
    Height = 13
    Caption = 'Y'
  end
  object Label2: TLabel
    Left = 24
    Top = 636
    Width = 7
    Height = 13
    Caption = 'X'
  end
  object mtLoaderBuffer: TTMyTray
    Left = 6
    Top = 2
    Width = 491
    Height = 567
    Color = clWhite
    XItem = 5
    YItem = 8
    OnMouseDown = mtLoaderBufferMouseDown
    OnMouseUp = mtLoaderBufferMouseUp
    OnMouseMove = mtLoaderBufferMouseMove
  end
  object edtYPos: TEdit
    Left = 200
    Top = 632
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = edtXPosClick
  end
  object edtXPos: TEdit
    Left = 48
    Top = 632
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = edtXPosClick
  end
  object Button1: TButton
    Left = 336
    Top = 632
    Width = 113
    Height = 25
    Caption = 'Manual Input'
    TabOrder = 7
    OnClick = Button1Click
  end
  object BitBtn1: TBitBtn
    Left = 8
    Top = 568
    Width = 97
    Height = 25
    Caption = 'START'
    TabOrder = 1
    Visible = False
    OnClick = BitBtn1Click
  end
  object Button2: TButton
    Left = 108
    Top = 568
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 2
    Visible = False
    OnClick = Button2Click
  end
  object CheckBox1: TCheckBox
    Left = 384
    Top = 612
    Width = 97
    Height = 17
    Caption = 'No Save Image'
    TabOrder = 4
  end
  object cbBinCount: TComboBox
    Left = 8
    Top = 600
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 3
    Text = '1'
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 96
    Top = 8
  end
end
