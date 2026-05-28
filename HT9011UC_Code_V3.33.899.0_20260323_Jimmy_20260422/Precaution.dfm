object fPrecaution: TfPrecaution
  Left = 756
  Top = 155
  BorderStyle = bsSingle
  Caption = 'Precaution Record'
  ClientHeight = 473
  ClientWidth = 574
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnPrecautionsRecord: TPanel
    Left = 0
    Top = 0
    Width = 574
    Height = 473
    Align = alClient
    BevelInner = bvLowered
    Color = 9534289
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Courier New'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object pnMaintenanceRecord: TPanel
      Left = 12
      Top = 8
      Width = 549
      Height = 457
      BevelInner = bvLowered
      Color = 12761254
      TabOrder = 0
      object pnMaintenanceRecordTitle: TPanel
        Left = 2
        Top = 2
        Width = 545
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Station Precautions Record'
        Color = 9534289
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Courier New'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object MemoHandlerPrecautionRecord: TMemo
        Left = 5
        Top = 104
        Width = 536
        Height = 225
        Color = 14670284
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -16
        Font.Name = 'Courier New'
        Font.Style = [fsBold]
        ParentFont = False
        ReadOnly = True
        TabOrder = 6
      end
      object pnPrecautionRecordDocumentNoTitle: TPanel
        Left = 5
        Top = 40
        Width = 150
        Height = 26
        BevelInner = bvLowered
        Caption = 'DOCUMENT NO.'#65306
        Color = 6055843
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object Panel91: TPanel
        Left = 310
        Top = 40
        Width = 231
        Height = 26
        BevelInner = bvLowered
        Caption = '('#33509#28961#35215#31684#20381#24490#65292#21063#20197'"N/A"'#34920#31034')'
        Color = 6055843
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object Panel92: TPanel
        Left = 5
        Top = 72
        Width = 150
        Height = 26
        BevelInner = bvLowered
        Caption = #27880#24847#20107#38917#20839#23481#65306
        Color = 6055843
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Panel93: TPanel
        Left = 5
        Top = 338
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #37096#38272#32147#29702#26680#20934#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object Panel94: TPanel
        Left = 290
        Top = 337
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #35069#34920#32773#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object Panel95: TPanel
        Left = 5
        Top = 370
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #38283#22987#26085#26399#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 11
      end
      object pnPrecautionStartTime: TPanel
        Left = 140
        Top = 370
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object Panel96: TPanel
        Left = 290
        Top = 370
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #32080#26696#26085#26399#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object pnPrecautionEndTime: TPanel
        Left = 416
        Top = 370
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
      end
      object pnPrecautionRecordDocumentNo: TPanel
        Left = 159
        Top = 39
        Width = 146
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object pnNoteContents: TPanel
        Left = 159
        Top = 71
        Width = 382
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object pnApprovedManager: TPanel
        Left = 140
        Top = 338
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object pnWatchmakers: TPanel
        Left = 416
        Top = 338
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object Panel1: TPanel
        Left = 5
        Top = 402
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #32080#26696#26041#24335#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object pnCloseType: TPanel
        Left = 140
        Top = 402
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
      object Panel3: TPanel
        Left = 290
        Top = 402
        Width = 115
        Height = 26
        BevelInner = bvLowered
        Caption = #32080#26696#32773#65306
        Color = 6055843
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #27161#26999#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 17
      end
      object pnCloseName: TPanel
        Left = 416
        Top = 402
        Width = 120
        Height = 26
        BevelInner = bvLowered
        Color = clTeal
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
      end
      object pnPRSpecification: TPanel
        Left = 416
        Top = 432
        Width = 120
        Height = 20
        BevelInner = bvLowered
        Color = clSilver
        Font.Charset = ANSI_CHARSET
        Font.Color = clPurple
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
      end
    end
  end
  object tm_CheckEditEmpty: TTimer
    Enabled = False
    Interval = 300
    OnTimer = tm_CheckEditEmptyTimer
    Left = 500
    Top = 144
  end
end
