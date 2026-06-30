object fSortCT: TfSortCT
  Left = 829
  Top = 132
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Sort Count'
  ClientHeight = 1113
  ClientWidth = 487
  Color = clBtnFace
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 487
    Height = 1113
    ActivePage = SortCount
    Align = alClient
    TabHeight = 25
    TabIndex = 0
    TabOrder = 0
    object SortCount: TTabSheet
      Caption = 'SortCount'
      object pnlLoadingBG: TPanel
        Left = 0
        Top = 0
        Width = 479
        Height = 36
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 0
        object lblLoading: TLabel
          Left = 7
          Top = 12
          Width = 46
          Height = 13
          Caption = 'Loading'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnlLoader: TPanel
          Left = 60
          Top = 8
          Width = 176
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnMouseDown = pnlLoaderMouseDown
        end
        object pnlLoadTrayCt: TPanel
          Left = 244
          Top = 8
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlLoadCID: TPanel
          Left = 300
          Top = 8
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnDblClick = pnlAuto1DblClick
          OnMouseDown = pnlAuto1YieldMouseDown
        end
      end
      object pnlTotalBG: TPanel
        Left = 0
        Top = 36
        Width = 479
        Height = 36
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 1
        object lblTotal: TLabel
          Left = 7
          Top = 12
          Width = 30
          Height = 13
          Caption = 'Total'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          OnClick = lblTotalClick
        end
        object pnlTotal: TPanel
          Left = 116
          Top = 8
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlYield: TPanel
          Left = 60
          Top = 8
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlCoverTrayD: TPanel
          Left = 300
          Top = 8
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnDblClick = pnlAuto1DblClick
          OnMouseDown = pnlAuto1YieldMouseDown
        end
      end
      object pnlUnloadBG: TPanel
        Left = 0
        Top = 72
        Width = 479
        Height = 849
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 2
        object lblAuto1: TLabel
          Left = 7
          Top = 8
          Width = 38
          Height = 13
          Caption = 'Auto 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAuto2: TLabel
          Left = 7
          Top = 30
          Width = 38
          Height = 13
          Caption = 'Auto 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAuto3: TLabel
          Left = 7
          Top = 51
          Width = 38
          Height = 13
          Caption = 'Auto 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAuto4: TLabel
          Left = 7
          Top = 74
          Width = 38
          Height = 13
          Caption = 'Auto 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAuto5: TLabel
          Left = 7
          Top = 98
          Width = 38
          Height = 13
          Caption = 'Auto 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAuto6: TLabel
          Left = 7
          Top = 122
          Width = 38
          Height = 13
          Caption = 'Auto 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix1: TLabel
          Left = 7
          Top = 146
          Width = 28
          Height = 13
          Caption = 'Fix 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix2: TLabel
          Left = 7
          Top = 166
          Width = 28
          Height = 13
          Caption = 'Fix 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix4: TLabel
          Left = 7
          Top = 214
          Width = 28
          Height = 13
          Caption = 'Fix 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix3: TLabel
          Left = 7
          Top = 190
          Width = 28
          Height = 13
          Caption = 'Fix 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix5: TLabel
          Left = 7
          Top = 238
          Width = 28
          Height = 13
          Caption = 'Fix 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix6: TLabel
          Left = 7
          Top = 258
          Width = 28
          Height = 13
          Caption = 'Fix 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix7: TLabel
          Left = 7
          Top = 282
          Width = 28
          Height = 13
          Caption = 'Fix 7'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix8: TLabel
          Left = 7
          Top = 306
          Width = 28
          Height = 13
          Caption = 'Fix 8'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix9: TLabel
          Left = 7
          Top = 330
          Width = 28
          Height = 13
          Caption = 'Fix 9'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix10: TLabel
          Left = 7
          Top = 354
          Width = 35
          Height = 13
          Caption = 'Fix 10'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix11: TLabel
          Left = 7
          Top = 378
          Width = 35
          Height = 13
          Caption = 'Fix 11'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblFix12: TLabel
          Left = 7
          Top = 402
          Width = 35
          Height = 13
          Caption = 'Fix 12'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblBinBox: TLabel
          Left = 7
          Top = 426
          Width = 51
          Height = 13
          Caption = 'Bulk Box'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag1: TLabel
          Left = 7
          Top = 450
          Width = 36
          Height = 13
          Caption = 'Mag 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag2: TLabel
          Left = 7
          Top = 474
          Width = 36
          Height = 13
          Caption = 'Mag 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag3: TLabel
          Left = 7
          Top = 498
          Width = 36
          Height = 13
          Caption = 'Mag 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag4: TLabel
          Left = 7
          Top = 522
          Width = 36
          Height = 13
          Caption = 'Mag 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag5: TLabel
          Left = 7
          Top = 546
          Width = 36
          Height = 13
          Caption = 'Mag 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag6: TLabel
          Left = 7
          Top = 570
          Width = 36
          Height = 13
          Caption = 'Mag 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag7: TLabel
          Left = 7
          Top = 594
          Width = 36
          Height = 13
          Caption = 'Mag 7'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag8: TLabel
          Left = 7
          Top = 618
          Width = 36
          Height = 13
          Caption = 'Mag 8'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag9: TLabel
          Left = 7
          Top = 642
          Width = 36
          Height = 13
          Caption = 'Mag 9'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag10: TLabel
          Left = 7
          Top = 666
          Width = 43
          Height = 13
          Caption = 'Mag 10'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag11: TLabel
          Left = 7
          Top = 690
          Width = 43
          Height = 13
          Caption = 'Mag 11'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag12: TLabel
          Left = 7
          Top = 714
          Width = 43
          Height = 13
          Caption = 'Mag 12'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag13: TLabel
          Left = 7
          Top = 738
          Width = 43
          Height = 13
          Caption = 'Mag 13'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblMag14: TLabel
          Left = 7
          Top = 762
          Width = 43
          Height = 13
          Caption = 'Mag 14'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object btnClearCount: TSpeedButton
          Left = 16
          Top = 801
          Width = 220
          Height = 23
          Caption = 'Clear Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Glyph.Data = {
            AA040000424DAA04000000000000360000002800000014000000130000000100
            1800000000007404000074120000741200000000000000000000C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033990033
            990033990033990033990033990033990033990033990033990033990033C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033FFFFFFFFFFFFFFFFFF
            FFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C09900
            33FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033990033FFFFFFFFFFFF99003399
            0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF990033FFFFFF990033990033FFFFFF990033C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFF990033FFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFF990033FFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            990033FFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C099003399003399003399003399003399003399003399
            0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
          ParentFont = False
          OnClick = btnClearCountClick
        end
        object pnlAuto1Yield: TPanel
          Left = 60
          Top = 4
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto1: TPanel
          Left = 116
          Top = 4
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnDblClick = pnlAuto1DblClick
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto2Yield: TPanel
          Left = 60
          Top = 26
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlAuto2: TPanel
          Left = 116
          Top = 26
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlAuto3Yield: TPanel
          Left = 60
          Top = 47
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlAuto3: TPanel
          Left = 116
          Top = 47
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlAuto4Yield: TPanel
          Left = 60
          Top = 70
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlAuto4: TPanel
          Left = 116
          Top = 70
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pnlAuto5Yield: TPanel
          Left = 60
          Top = 94
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pnlAuto5: TPanel
          Left = 116
          Top = 94
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pnlAuto6Yield: TPanel
          Left = 60
          Top = 118
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object pnlAuto6: TPanel
          Left = 116
          Top = 118
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object pnlFix1Yield: TPanel
          Left = 60
          Top = 142
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
          OnMouseDown = pnlFix1YieldMouseDown
        end
        object pnlFix1: TPanel
          Left = 116
          Top = 142
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
        object pnlFix2Yield: TPanel
          Left = 60
          Top = 162
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 14
          OnMouseDown = pnlFix2MouseDown
        end
        object pnlFix2: TPanel
          Left = 116
          Top = 162
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 15
        end
        object pnlFix4Yield: TPanel
          Left = 60
          Top = 210
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 18
          OnMouseDown = pnlFix4YieldMouseDown
        end
        object pnlFix4: TPanel
          Left = 116
          Top = 210
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 19
        end
        object pnlFix3Yield: TPanel
          Left = 60
          Top = 186
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 16
          OnMouseDown = pnlFix3MouseDown
        end
        object pnlFix3: TPanel
          Left = 116
          Top = 186
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 17
        end
        object pnlFix5Yield: TPanel
          Left = 60
          Top = 234
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 20
          OnMouseDown = pnlFix5YieldMouseDown
        end
        object pnlFix5: TPanel
          Left = 116
          Top = 234
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 21
        end
        object pnlFix6Yield: TPanel
          Left = 60
          Top = 254
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 22
          OnMouseDown = pnlFix6YieldMouseDown
        end
        object pnlFix6: TPanel
          Left = 116
          Top = 254
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 23
        end
        object pnlFix7Yield: TPanel
          Left = 60
          Top = 278
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 24
        end
        object pnlFix7: TPanel
          Left = 116
          Top = 278
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 25
        end
        object pnlFix8Yield: TPanel
          Left = 60
          Top = 302
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 26
        end
        object pnlFix8: TPanel
          Left = 116
          Top = 302
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 27
        end
        object pnlFix9Yield: TPanel
          Left = 60
          Top = 326
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 28
        end
        object pnlFix9: TPanel
          Left = 116
          Top = 326
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 29
        end
        object pnlFix10Yield: TPanel
          Left = 60
          Top = 350
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 30
        end
        object pnlFix10: TPanel
          Left = 116
          Top = 350
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 31
        end
        object pnlFix11Yield: TPanel
          Left = 60
          Top = 374
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 32
        end
        object pnlFix11: TPanel
          Left = 116
          Top = 374
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 33
        end
        object pnlFix12Yield: TPanel
          Left = 60
          Top = 398
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 34
        end
        object pnlFix12: TPanel
          Left = 116
          Top = 398
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 35
        end
        object pnlBinBoxYield: TPanel
          Left = 60
          Top = 422
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 36
        end
        object pnlBinBox: TPanel
          Left = 116
          Top = 422
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 37
        end
        object pnlMag1Yield: TPanel
          Left = 60
          Top = 446
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 38
        end
        object pnlMag1: TPanel
          Left = 116
          Top = 446
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 39
        end
        object pnlMag2Yield: TPanel
          Tag = 1
          Left = 60
          Top = 470
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 40
        end
        object pnlMag2: TPanel
          Tag = 1
          Left = 116
          Top = 470
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 41
        end
        object pnlMag3Yield: TPanel
          Tag = 2
          Left = 60
          Top = 494
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 42
        end
        object pnlMag3: TPanel
          Tag = 2
          Left = 116
          Top = 494
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 43
        end
        object pnlMag4Yield: TPanel
          Tag = 3
          Left = 60
          Top = 518
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 44
        end
        object pnlMag4: TPanel
          Tag = 3
          Left = 116
          Top = 518
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 45
        end
        object pnlMag5Yield: TPanel
          Tag = 4
          Left = 60
          Top = 542
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 46
        end
        object pnlMag5: TPanel
          Tag = 4
          Left = 116
          Top = 542
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 47
        end
        object pnlMag6Yield: TPanel
          Tag = 5
          Left = 60
          Top = 566
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 48
        end
        object pnlMag6: TPanel
          Tag = 5
          Left = 116
          Top = 566
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 49
        end
        object pnlMag7Yield: TPanel
          Tag = 6
          Left = 60
          Top = 590
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 50
        end
        object pnlMag7: TPanel
          Tag = 6
          Left = 116
          Top = 590
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 51
        end
        object pnlMag8Yield: TPanel
          Tag = 7
          Left = 60
          Top = 614
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 52
        end
        object pnlMag8: TPanel
          Tag = 7
          Left = 116
          Top = 614
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 53
        end
        object pnlMag9Yield: TPanel
          Tag = 8
          Left = 60
          Top = 638
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 54
        end
        object pnlMag9: TPanel
          Tag = 8
          Left = 116
          Top = 638
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 55
        end
        object pnlMag10Yield: TPanel
          Tag = 9
          Left = 60
          Top = 662
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 56
        end
        object pnlMag10: TPanel
          Tag = 9
          Left = 116
          Top = 662
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 57
        end
        object pnlMag11Yield: TPanel
          Tag = 10
          Left = 60
          Top = 686
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 58
        end
        object pnlMag11: TPanel
          Tag = 10
          Left = 116
          Top = 686
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 59
        end
        object pnlMag12Yield: TPanel
          Tag = 11
          Left = 60
          Top = 710
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 60
        end
        object pnlMag12: TPanel
          Tag = 11
          Left = 116
          Top = 710
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 61
        end
        object pnlMag13Yield: TPanel
          Tag = 12
          Left = 60
          Top = 734
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 62
        end
        object pnlMag13: TPanel
          Tag = 12
          Left = 116
          Top = 734
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 63
        end
        object pnlMag14Yield: TPanel
          Tag = 13
          Left = 60
          Top = 758
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 64
        end
        object pnlMag14: TPanel
          Tag = 13
          Left = 116
          Top = 758
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 65
        end
        object pnlAuto1TrayCt: TPanel
          Left = 244
          Top = 4
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 66
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto1CID: TPanel
          Left = 300
          Top = 4
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 67
          OnDblClick = pnlAuto1CIDDblClick
        end
        object pnlAuto2TrayCt: TPanel
          Left = 244
          Top = 26
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 68
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto2CID: TPanel
          Left = 300
          Top = 26
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 69
          OnDblClick = pnlAuto1CIDDblClick
        end
        object pnlAuto3TrayCt: TPanel
          Left = 244
          Top = 47
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 70
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto3CID: TPanel
          Left = 300
          Top = 47
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 71
          OnDblClick = pnlAuto1CIDDblClick
        end
        object pnlAuto4TrayCt: TPanel
          Left = 244
          Top = 70
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 72
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto4CID: TPanel
          Left = 300
          Top = 70
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 73
          OnDblClick = pnlAuto1CIDDblClick
        end
        object pnlAuto5TrayCt: TPanel
          Left = 244
          Top = 94
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 74
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto5CID: TPanel
          Left = 300
          Top = 94
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 75
          OnDblClick = pnlAuto1CIDDblClick
        end
        object pnlAuto6TrayCt: TPanel
          Left = 244
          Top = 118
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 76
          OnMouseDown = pnlAuto1YieldMouseDown
        end
        object pnlAuto6CID: TPanel
          Left = 300
          Top = 118
          Width = 170
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 77
          OnDblClick = pnlAuto1CIDDblClick
        end
      end
      object gbLotID: TGroupBox
        Left = 0
        Top = 921
        Width = 479
        Height = 86
        Align = alTop
        Caption = 'Lot ID'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Visible = False
        object spbClearAllCount: TSpeedButton
          Left = 12
          Top = 51
          Width = 221
          Height = 26
          Caption = 'Clear All Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Glyph.Data = {
            AA040000424DAA04000000000000360000002800000014000000130000000100
            1800000000007404000074120000741200000000000000000000C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033990033
            990033990033990033990033990033990033990033990033990033990033C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033FFFFFFFFFFFFFFFFFF
            FFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C09900
            33FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033990033FFFFFFFFFFFF99003399
            0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF990033FFFFFF990033990033FFFFFF990033C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFF990033FFFFFFFFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFF990033FFFFFFFFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            990033FFFFFF990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C099003399003399003399003399003399003399003399
            0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
          ParentFont = False
          OnClick = spbClearAllCountClick
        end
        object edLotID: TEdit
          Left = 12
          Top = 20
          Width = 221
          Height = 24
          TabOrder = 0
        end
      end
    end
    object ARTSortCount: TTabSheet
      Caption = 'ARTSortCount'
      ImageIndex = 1
      object pnlLoadingARTBG: TPanel
        Left = 0
        Top = 0
        Width = 479
        Height = 36
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 0
        object Label14: TLabel
          Left = 7
          Top = 12
          Width = 46
          Height = 13
          Caption = 'Loading'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnlLoadingART: TPanel
          Left = 57
          Top = 8
          Width = 176
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnMouseDown = pnlLoaderMouseDown
        end
      end
      object pnlTotalARTBG: TPanel
        Left = 0
        Top = 36
        Width = 479
        Height = 36
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 1
        object Label15: TLabel
          Left = 7
          Top = 12
          Width = 30
          Height = 13
          Caption = 'Total'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnlTotalART: TPanel
          Left = 113
          Top = 8
          Width = 118
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlYieldART: TPanel
          Left = 58
          Top = 8
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object pnlUnloadingARTBG: TPanel
        Left = 0
        Top = 72
        Width = 479
        Height = 801
        Align = alTop
        BevelInner = bvLowered
        TabOrder = 2
        object lblARTMag14: TLabel
          Left = 7
          Top = 776
          Width = 43
          Height = 13
          Caption = 'Mag 14'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag13: TLabel
          Left = 7
          Top = 752
          Width = 43
          Height = 13
          Caption = 'Mag 13'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag12: TLabel
          Left = 7
          Top = 728
          Width = 43
          Height = 13
          Caption = 'Mag 12'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag11: TLabel
          Left = 7
          Top = 704
          Width = 43
          Height = 13
          Caption = 'Mag 11'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag10: TLabel
          Left = 7
          Top = 680
          Width = 43
          Height = 13
          Caption = 'Mag 10'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag9: TLabel
          Left = 7
          Top = 656
          Width = 36
          Height = 13
          Caption = 'Mag 9'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag8: TLabel
          Left = 7
          Top = 632
          Width = 36
          Height = 13
          Caption = 'Mag 8'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag7: TLabel
          Left = 7
          Top = 608
          Width = 36
          Height = 13
          Caption = 'Mag 7'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag6: TLabel
          Left = 7
          Top = 584
          Width = 36
          Height = 13
          Caption = 'Mag 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag5: TLabel
          Left = 7
          Top = 560
          Width = 36
          Height = 13
          Caption = 'Mag 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag4: TLabel
          Left = 7
          Top = 536
          Width = 36
          Height = 13
          Caption = 'Mag 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag3: TLabel
          Left = 7
          Top = 512
          Width = 36
          Height = 13
          Caption = 'Mag 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag2: TLabel
          Left = 7
          Top = 488
          Width = 36
          Height = 13
          Caption = 'Mag 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTMag1: TLabel
          Left = 7
          Top = 464
          Width = 36
          Height = 13
          Caption = 'Mag 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTBinBox: TLabel
          Left = 7
          Top = 440
          Width = 51
          Height = 13
          Caption = 'Bulk Box'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix12: TLabel
          Left = 7
          Top = 416
          Width = 35
          Height = 13
          Caption = 'Fix 12'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix11: TLabel
          Left = 7
          Top = 392
          Width = 35
          Height = 13
          Caption = 'Fix 11'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix10: TLabel
          Left = 7
          Top = 368
          Width = 35
          Height = 13
          Caption = 'Fix 10'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix9: TLabel
          Left = 7
          Top = 344
          Width = 28
          Height = 13
          Caption = 'Fix 9'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto4: TLabel
          Left = 7
          Top = 104
          Width = 38
          Height = 13
          Caption = 'Auto 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix8: TLabel
          Left = 7
          Top = 320
          Width = 28
          Height = 13
          Caption = 'Fix 8'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix7: TLabel
          Left = 7
          Top = 296
          Width = 28
          Height = 13
          Caption = 'Fix 7'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix6: TLabel
          Left = 7
          Top = 272
          Width = 28
          Height = 13
          Caption = 'Fix 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix5: TLabel
          Left = 7
          Top = 248
          Width = 28
          Height = 13
          Caption = 'Fix 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix4: TLabel
          Left = 7
          Top = 224
          Width = 28
          Height = 13
          Caption = 'Fix 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix3: TLabel
          Left = 7
          Top = 200
          Width = 28
          Height = 13
          Caption = 'Fix 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix2: TLabel
          Left = 7
          Top = 176
          Width = 28
          Height = 13
          Caption = 'Fix 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTFix1: TLabel
          Left = 7
          Top = 152
          Width = 28
          Height = 13
          Caption = 'Fix 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto2: TLabel
          Left = 7
          Top = 32
          Width = 38
          Height = 13
          Caption = 'Auto 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto1: TLabel
          Left = 7
          Top = 8
          Width = 38
          Height = 13
          Caption = 'Auto 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto3: TLabel
          Left = 7
          Top = 56
          Width = 38
          Height = 13
          Caption = 'Auto 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto5: TLabel
          Left = 7
          Top = 80
          Width = 38
          Height = 13
          Caption = 'Auto 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblARTAuto6: TLabel
          Left = 7
          Top = 128
          Width = 38
          Height = 13
          Caption = 'Auto 6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnARTMag14Yield: TPanel
          Tag = 13
          Left = 60
          Top = 772
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnARTMag14: TPanel
          Tag = 13
          Left = 116
          Top = 772
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnARTMag13Yield: TPanel
          Tag = 12
          Left = 60
          Top = 748
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnARTMag13: TPanel
          Tag = 12
          Left = 116
          Top = 748
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnARTMag12Yield: TPanel
          Tag = 11
          Left = 60
          Top = 724
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnARTMag12: TPanel
          Tag = 11
          Left = 116
          Top = 724
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnARTMag11Yield: TPanel
          Tag = 10
          Left = 60
          Top = 700
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnARTMag11: TPanel
          Tag = 10
          Left = 116
          Top = 700
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pnARTMag10Yield: TPanel
          Tag = 9
          Left = 60
          Top = 676
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pnARTMag10: TPanel
          Tag = 9
          Left = 116
          Top = 676
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pnARTMag9Yield: TPanel
          Tag = 8
          Left = 60
          Top = 652
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object pnARTMag9: TPanel
          Tag = 8
          Left = 116
          Top = 652
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object pnARTMag8Yield: TPanel
          Tag = 7
          Left = 60
          Top = 628
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
        object pnARTMag8: TPanel
          Tag = 7
          Left = 116
          Top = 628
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
        object pnARTMag7: TPanel
          Tag = 6
          Left = 116
          Top = 604
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 14
        end
        object pnARTMag7Yield: TPanel
          Tag = 6
          Left = 60
          Top = 604
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 15
        end
        object pnARTMag6Yield: TPanel
          Tag = 5
          Left = 60
          Top = 580
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 16
        end
        object pnARTMag6: TPanel
          Tag = 5
          Left = 116
          Top = 580
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 17
        end
        object pnARTMag5Yield: TPanel
          Tag = 4
          Left = 60
          Top = 556
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 18
        end
        object pnARTMag5: TPanel
          Tag = 4
          Left = 116
          Top = 556
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 19
        end
        object pnARTMag4Yield: TPanel
          Tag = 3
          Left = 60
          Top = 532
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 20
        end
        object pnARTMag4: TPanel
          Tag = 3
          Left = 116
          Top = 532
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 21
        end
        object pnARTMag3Yield: TPanel
          Tag = 2
          Left = 60
          Top = 508
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 22
        end
        object pnARTMag3: TPanel
          Tag = 2
          Left = 116
          Top = 508
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 23
        end
        object pnARTMag2Yield: TPanel
          Tag = 1
          Left = 60
          Top = 484
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 24
        end
        object pnARTMag2: TPanel
          Tag = 1
          Left = 116
          Top = 484
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 25
        end
        object pnARTMag1Yield: TPanel
          Left = 60
          Top = 460
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 26
        end
        object pnARTMag1: TPanel
          Left = 116
          Top = 460
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 27
        end
        object pnARTBinBoxYield: TPanel
          Left = 60
          Top = 436
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 28
        end
        object pnARTBinBox: TPanel
          Left = 116
          Top = 436
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 29
        end
        object pnARTFix12Yield: TPanel
          Left = 60
          Top = 412
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 30
        end
        object pnARTFix12: TPanel
          Left = 116
          Top = 412
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 31
        end
        object pnARTFix11Yield: TPanel
          Left = 60
          Top = 388
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 32
        end
        object pnARTFix11: TPanel
          Left = 116
          Top = 388
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 33
        end
        object pnARTFix10Yield: TPanel
          Left = 60
          Top = 364
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 34
        end
        object pnARTFix10: TPanel
          Left = 116
          Top = 364
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 35
        end
        object pnARTFix9Yield: TPanel
          Left = 60
          Top = 340
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 36
        end
        object pnARTFix9: TPanel
          Left = 116
          Top = 340
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 37
        end
        object pnARTAuto4Yield: TPanel
          Left = 60
          Top = 100
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 38
        end
        object pnARTAuto4: TPanel
          Left = 116
          Top = 100
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 39
        end
        object pnARTFix8Yield: TPanel
          Left = 60
          Top = 316
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 40
        end
        object pnARTFix8: TPanel
          Left = 116
          Top = 316
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 41
        end
        object pnARTFix7Yield: TPanel
          Left = 60
          Top = 292
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 42
        end
        object pnARTFix7: TPanel
          Left = 116
          Top = 292
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 43
        end
        object pnARTFix6Yield: TPanel
          Left = 60
          Top = 268
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 44
        end
        object pnARTFix6: TPanel
          Left = 116
          Top = 268
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 45
        end
        object pnARTFix5Yield: TPanel
          Left = 60
          Top = 244
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 46
        end
        object pnARTFix5: TPanel
          Left = 116
          Top = 244
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 47
        end
        object pnARTFix4Yield: TPanel
          Left = 60
          Top = 220
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 48
        end
        object pnARTFix4: TPanel
          Left = 116
          Top = 220
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 49
        end
        object pnARTFix3Yield: TPanel
          Left = 60
          Top = 196
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 50
        end
        object pnARTFix3: TPanel
          Left = 116
          Top = 196
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 51
        end
        object pnARTFix2Yield: TPanel
          Left = 60
          Top = 172
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 52
        end
        object pnARTFix2: TPanel
          Left = 116
          Top = 172
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 53
        end
        object pnARTFix1Yield: TPanel
          Left = 60
          Top = 148
          Width = 49
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 54
        end
        object pnARTFix1: TPanel
          Left = 116
          Top = 148
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 55
        end
        object pnARTAuto2Yield: TPanel
          Left = 60
          Top = 28
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 56
        end
        object pnARTAuto2: TPanel
          Left = 116
          Top = 28
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 57
        end
        object pnARTAuto1Yield: TPanel
          Left = 60
          Top = 4
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 58
        end
        object pnARTAuto1: TPanel
          Left = 116
          Top = 4
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 59
        end
        object pnARTAuto3Yield: TPanel
          Left = 60
          Top = 52
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 60
        end
        object pnARTAuto3: TPanel
          Left = 116
          Top = 52
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 61
        end
        object pnARTAuto5Yield: TPanel
          Left = 60
          Top = 76
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 62
        end
        object pnARTAuto5: TPanel
          Left = 116
          Top = 76
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 63
        end
        object pnARTAuto6Yield: TPanel
          Left = 60
          Top = 124
          Width = 50
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 64
        end
        object pnARTAuto6: TPanel
          Left = 116
          Top = 124
          Width = 120
          Height = 20
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = '100'
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 65
        end
      end
    end
    object tsICCount: TTabSheet
      Caption = 'IC Count'
      ImageIndex = 2
      object grpInputArea: TGroupBox
        Left = 0
        Top = 0
        Width = 476
        Height = 110
        Align = alTop
        Caption = 'Input Area'
        TabOrder = 0
        object lblLoad: TLabel
          Left = 8
          Top = 24
          Width = 40
          Height = 13
          Caption = 'Loader'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblHP2: TLabel
          Left = 8
          Top = 52
          Width = 25
          Height = 13
          Caption = 'HP1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblHP1: TLabel
          Left = 8
          Top = 80
          Width = 25
          Height = 13
          Caption = 'HP2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnlLoad: TPanel
          Left = 57
          Top = 18
          Width = 176
          Height = 25
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnMouseDown = pnlLoaderMouseDown
        end
        object pnlHP1: TPanel
          Left = 57
          Top = 46
          Width = 176
          Height = 25
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnMouseDown = pnlHP1MouseDown
        end
        object pnlHP2: TPanel
          Left = 57
          Top = 74
          Width = 176
          Height = 25
          Alignment = taRightJustify
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Batang'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnMouseDown = pnlHP2MouseDown
        end
      end
    end
  end
  object pnlTrayCount: TPanel
    Left = 228
    Top = 4
    Width = 89
    Height = 25
    BevelOuter = bvNone
    Caption = 'Tray Count'
    TabOrder = 1
  end
  object pnlTrayCID: TPanel
    Left = 313
    Top = 4
    Width = 153
    Height = 25
    BevelOuter = bvNone
    Caption = 'Cover Tray ID'
    TabOrder = 2
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 189
    Top = 765
  end
  object Timer2: TTimer
    Interval = 5000
    OnTimer = Timer2Timer
    Left = 220
    Top = 765
  end
end
