object fCounterSel: TfCounterSel
  Left = 279
  Top = 116
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Counter Selection'
  ClientHeight = 559
  ClientWidth = 397
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
  object spbExit: TSpeedButton
    Left = 43
    Top = 504
    Width = 314
    Height = 48
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    Glyph.Data = {
      0E060000424D0E06000000000000360000002800000016000000160000000100
      180000000000D805000001000000010000000000000000000000EFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF7474A4
      4C4C946C6CA4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF6C6CA44C4C947474
      A4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EF2C2C8C5454C45454C4645C9CEFEFEFEFEFEFEFEFEFEFEFEF645C9C3C3CAC6C
      6CDC4C4C94EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEF8C8CAC2424946464D45454C4645C9CEFEFEFEFEFEF645C9C3C3CAC
      6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEF8C8CAC2424946464D45454C4645C9C645C9C3C3C
      AC6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC2424946464D45454C43C
      3CAC6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC242494
      6464D46C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF645C
      9C3C3CAC6C6CDC6464D45454C4645C9CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF64
      5C9C3C3CAC6C6CDC2C2C9C2424946464D45454C4645C9CEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      645C9C3C3CAC6C6CDC2C2C9C8C8CAC8C8CAC2424946464D45454C4645C9CEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EF6C6CA43C3CAC6C6CDC2C2C9C8C8CACEFEFEFEFEFEF8C8CAC2424946464D454
      54C46C6CA4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEF2C2C8C4C4CBC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC
      2424945454C44C4C94EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEF8C8CAC3434948C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEF8C8CAC3434948C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
      EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF}
    ParentFont = False
    OnClick = spbExitClick
  end
  object GroupBox17: TGroupBox
    Left = 20
    Top = 15
    Width = 353
    Height = 462
    Caption = 'Counter Select'
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object gbLoadingCount: TGroupBox
      Left = 11
      Top = 12
      Width = 332
      Height = 59
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object labLoadingCount: TLabel
        Left = 15
        Top = 25
        Width = 78
        Height = 13
        Caption = 'Loading Counter'
      end
      object rbLoadingCount_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 13
        Caption = 'ON'
        TabOrder = 0
      end
      object rbLoadingCount_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 13
        Caption = 'OFF'
        TabOrder = 1
      end
    end
    object gbContactCount: TGroupBox
      Left = 11
      Top = 72
      Width = 332
      Height = 59
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object labContactCount: TLabel
        Left = 16
        Top = 25
        Width = 68
        Height = 13
        Caption = 'Contact Count'
      end
      object rbContactCount_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 18
        Caption = 'ON'
        TabOrder = 0
      end
      object rbContactCount_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 18
        Caption = 'OFF'
        TabOrder = 1
      end
    end
    object gbTestCategory: TGroupBox
      Left = 11
      Top = 134
      Width = 332
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object labTestCategory: TLabel
        Left = 16
        Top = 25
        Width = 75
        Height = 13
        Caption = 'Tester Category'
      end
      object rbTestCategory_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 18
        Caption = 'ON'
        TabOrder = 1
      end
      object rbTestCategory_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 18
        Caption = 'OFF'
        TabOrder = 2
      end
      object rgTestCategory: TRadioGroup
        Left = 242
        Top = 8
        Width = 82
        Height = 45
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -9
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemIndex = 1
        Items.Strings = (
          'Normal'
          'By Arm')
        ParentFont = False
        TabOrder = 0
      end
    end
    object gbScanner: TGroupBox
      Left = 11
      Top = 194
      Width = 332
      Height = 59
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object labScanner: TLabel
        Left = 14
        Top = 25
        Width = 85
        Height = 13
        Caption = 'Scanner Category'
      end
      object rbScanner_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 18
        Caption = 'ON'
        TabOrder = 0
      end
      object rbScanner_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 18
        Caption = 'OFF'
        TabOrder = 1
      end
    end
    object gbTemperature: TGroupBox
      Left = 11
      Top = 255
      Width = 332
      Height = 59
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object labTemperature: TLabel
        Left = 16
        Top = 25
        Width = 60
        Height = 13
        Caption = 'Temperature'
      end
      object rbTemperature_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 18
        Caption = 'ON'
        TabOrder = 0
      end
      object rbTemperature_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 18
        Caption = 'OFF'
        TabOrder = 1
      end
    end
    object gbBinAssign: TGroupBox
      Left = 11
      Top = 316
      Width = 332
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      object labBinAssign: TLabel
        Left = 16
        Top = 25
        Width = 49
        Height = 13
        Caption = 'Bin Assign'
      end
      object rbBinAssign_On: TRadioButton
        Left = 119
        Top = 25
        Width = 60
        Height = 18
        Caption = 'ON'
        TabOrder = 0
      end
      object rbBinAssign_Off: TRadioButton
        Left = 186
        Top = 25
        Width = 60
        Height = 18
        Caption = 'OFF'
        TabOrder = 1
      end
    end
    object gbStatus: TGroupBox
      Left = 11
      Top = 376
      Width = 332
      Height = 74
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      object labStatus: TLabel
        Left = 16
        Top = 32
        Width = 30
        Height = 13
        Caption = 'Status'
      end
      object cbIndexTime: TCheckBox
        Left = 126
        Top = 15
        Width = 90
        Height = 16
        Caption = 'Index Time'
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object cbUPH: TCheckBox
        Left = 126
        Top = 46
        Width = 98
        Height = 15
        Caption = 'UPH'
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object cbCycleTime: TCheckBox
        Left = 230
        Top = 15
        Width = 90
        Height = 16
        Caption = 'Cycle Time'
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object cbContactHeight: TCheckBox
        Left = 230
        Top = 46
        Width = 98
        Height = 15
        Caption = 'Contact Height'
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
    end
  end
  object cbDefaultValue: TCheckBox
    Left = 226
    Top = 483
    Width = 151
    Height = 16
    Caption = 'Default Position Value'
    TabOrder = 1
  end
end
