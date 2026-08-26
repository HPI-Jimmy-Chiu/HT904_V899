object CCDInterfaceForm: TCCDInterfaceForm
  Left = 400
  Top = 289
  Width = 391
  Height = 333
  Caption = 'MUTInterface'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 131
    Top = 0
    Width = 236
    Height = 193
    Color = 14670284
    TabOrder = 0
    OnDblClick = MemoDblClick
  end
  object Button1: TButton
    Left = 8
    Top = 40
    Width = 113
    Height = 25
    Caption = 'EXISTENCE'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 104
    Width = 113
    Height = 25
    Caption = 'FILE READY'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 8
    Top = 136
    Width = 113
    Height = 25
    Caption = 'IDENTIFICATION'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 8
    Top = 168
    Width = 113
    Height = 25
    Caption = 'END'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Edit1: TEdit
    Left = 8
    Top = 72
    Width = 113
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = 'Test'
  end
  object Button5: TButton
    Left = 8
    Top = 8
    Width = 113
    Height = 25
    Caption = 'Call CCD Progran'
    TabOrder = 6
    OnClick = Button5Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 200
    Width = 361
    Height = 81
    Caption = 'Grab Image'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    object Label1: TLabel
      Left = 96
      Top = 24
      Width = 40
      Height = 16
      Caption = 'Count :'
    end
    object Label2: TLabel
      Left = 8
      Top = 52
      Width = 62
      Height = 16
      Caption = 'Save Path'
    end
    object CountCBox: TComboBox
      Left = 144
      Top = 20
      Width = 41
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 16
      ParentFont = False
      TabOrder = 0
      Text = '1'
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9'
        '10')
    end
    object Button6: TButton
      Left = 200
      Top = 20
      Width = 65
      Height = 25
      Caption = 'GRAB'
      TabOrder = 1
      OnClick = Button6Click
    end
    object edSavePath: TEdit
      Left = 80
      Top = 48
      Width = 185
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'D:\'
    end
    object VideoCBox: TComboBox
      Left = 8
      Top = 20
      Width = 73
      Height = 24
      ItemHeight = 16
      TabOrder = 3
      Text = 'Video 1'
      Items.Strings = (
        'Video 1'
        'Video 2'
        'Video 3'
        'Video 4')
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 200
    Left = 184
    Top = 16
  end
  object Timer2: TTimer
    Interval = 200
    Left = 220
    Top = 16
  end
end
