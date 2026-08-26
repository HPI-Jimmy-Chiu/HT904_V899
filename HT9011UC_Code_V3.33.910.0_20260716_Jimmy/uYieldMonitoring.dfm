object fYieldMonitoring: TfYieldMonitoring
  Left = 193
  Top = 138
  BorderStyle = bsSingle
  Caption = 'Yield Monitoring'
  ClientHeight = 956
  ClientWidth = 1053
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBottom: TPanel
    Left = 0
    Top = 893
    Width = 1053
    Height = 63
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object btnApply: TButton
      Left = 144
      Top = 15
      Width = 200
      Height = 40
      Caption = 'Save'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnApplyClick
    end
    object btnOk: TButton
      Left = 424
      Top = 15
      Width = 200
      Height = 40
      Caption = 'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnOkClick
    end
  end
  object pnlTop: TPanel
    Left = 0
    Top = 0
    Width = 1053
    Height = 893
    Align = alClient
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object pgcMode: TPageControl
      Left = 0
      Top = 0
      Width = 1053
      Height = 893
      ActivePage = tsYield
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabIndex = 5
      TabOrder = 0
      TabWidth = 100
      object tsNormal: TTabSheet
        Caption = 'Normal'
        object pgcBySiteByBinPercentCompare_FT: TPageControl
          Left = 784
          Top = 0
          Width = 261
          Height = 858
          ActivePage = tsSpecBinByArmPerSiteCompare_FT
          Align = alRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabIndex = 2
          TabOrder = 2
          object tsBinAlarm1_FT: TTabSheet
            Caption = 'Bin Alarm1'
            object labByBinFailureCatCT_FT: TLabel
              Left = 55
              Top = 38
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinFailure_FT: TLabel
              Left = 11
              Top = 5
              Width = 105
              Height = 20
              Caption = 'By bin limit FT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByBinFailureCatCT_FT: TEdit
              Left = 165
              Top = 33
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm1_FT: TScrollBox
              Left = 0
              Top = 67
              Width = 253
              Height = 760
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
              object cbByBinFailureCatXX_FT: TCheckBox
                Left = 8
                Top = 10
                Width = 228
                Height = 16
                Caption = 'Category 100                     %'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Visible = False
                OnMouseUp = cbLowYield_FTMouseUp
              end
              object edByBinFailureCatXX_FT: TEdit
                Left = 135
                Top = 4
                Width = 60
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                Visible = False
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
            end
          end
          object tsSpecBinBySiteCompare_FT: TTabSheet
            Caption = 'Bin Alarm2'
            object labByBinSiteGapCatCT_FT: TLabel
              Left = 63
              Top = 33
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinSiteGapCat_FT: TLabel
              Left = 11
              Top = 5
              Width = 134
              Height = 20
              Caption = 'By bin site gap FT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByBinSiteGapCatCT_FT: TEdit
              Left = 173
              Top = 29
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm2_FT: TScrollBox
              Left = 0
              Top = 67
              Width = 253
              Height = 760
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
            end
          end
          object tsSpecBinByArmPerSiteCompare_FT: TTabSheet
            Caption = 'Bin Alarm3'
            ImageIndex = 1
            object labByArmSiteGapCatCT_FT: TLabel
              Left = 63
              Top = 38
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByArmSiteGapCat_FT: TLabel
              Left = 11
              Top = 5
              Width = 185
              Height = 20
              Caption = 'By arm by bin site gap FT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByArmSiteGapCatCT_FT: TEdit
              Left = 173
              Top = 34
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm3_FT: TScrollBox
              Left = 0
              Top = 64
              Width = 253
              Height = 763
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
            end
          end
          object TabSheet1: TTabSheet
            Caption = 'Bin Alarm4'
            ImageIndex = 3
            object Label9: TLabel
              Left = 11
              Top = 566
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinLimitCount: TLabel
              Left = 19
              Top = 4
              Width = 118
              Height = 20
              Caption = 'By bin limit count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbBybinLimitCat0_FT: TCheckBox
              Left = 8
              Top = 42
              Width = 192
              Height = 14
              Caption = 'Category 0                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat0_FT: TEdit
              Left = 123
              Top = 33
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
            end
            object cbBybinLimitCat1_FT: TCheckBox
              Left = 8
              Top = 73
              Width = 192
              Height = 14
              Caption = 'Category 1                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat1_FT: TEdit
              Left = 123
              Top = 64
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 3
              Text = '0'
            end
            object cbBybinLimitCat2_FT: TCheckBox
              Left = 8
              Top = 105
              Width = 192
              Height = 14
              Caption = 'Category 2                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat3_FT: TCheckBox
              Left = 8
              Top = 136
              Width = 192
              Height = 14
              Caption = 'Category 3                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat4_FT: TCheckBox
              Left = 8
              Top = 168
              Width = 192
              Height = 14
              Caption = 'Category 4                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat3_FT: TEdit
              Left = 123
              Top = 127
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 7
              Text = '0'
            end
            object cbBybinLimitCat5_FT: TCheckBox
              Left = 8
              Top = 199
              Width = 192
              Height = 14
              Caption = 'Category 5                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 8
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat6_FT: TCheckBox
              Left = 8
              Top = 231
              Width = 192
              Height = 14
              Caption = 'Category 6                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 9
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat7_FT: TCheckBox
              Left = 8
              Top = 262
              Width = 192
              Height = 14
              Caption = 'Category 7                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 10
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat8_FT: TCheckBox
              Left = 8
              Top = 294
              Width = 192
              Height = 14
              Caption = 'Category 8                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 11
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat9_FT: TCheckBox
              Left = 8
              Top = 325
              Width = 192
              Height = 14
              Caption = 'Category 9                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 12
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat9_FT: TEdit
              Left = 123
              Top = 317
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 13
              Text = '0'
            end
            object cbBybinLimitCat10_FT: TCheckBox
              Left = 8
              Top = 357
              Width = 192
              Height = 14
              Caption = 'Category 10              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 14
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat10_FT: TEdit
              Left = 123
              Top = 349
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 15
              Text = '0'
            end
            object cbBybinLimitCat11_FT: TCheckBox
              Left = 8
              Top = 388
              Width = 192
              Height = 14
              Caption = 'Category 11              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 16
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat11_FT: TEdit
              Left = 123
              Top = 380
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 17
              Text = '0'
            end
            object cbBybinLimitCat12_FT: TCheckBox
              Left = 8
              Top = 420
              Width = 192
              Height = 14
              Caption = 'Category 12              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 18
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat12_FT: TEdit
              Left = 123
              Top = 412
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 19
              Text = '0'
            end
            object cbBybinLimitCat13_FT: TCheckBox
              Left = 8
              Top = 451
              Width = 192
              Height = 14
              Caption = 'Category 13              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 20
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat13_FT: TEdit
              Left = 123
              Top = 444
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 21
              Text = '0'
            end
            object edByBinLimitCountCat2_FT: TEdit
              Left = 123
              Top = 96
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 22
              Text = '0'
            end
            object edByBinLimitCountCat8_FT: TEdit
              Left = 123
              Top = 286
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 23
              Text = '0'
            end
            object edByBinLimitCountCat7_FT: TEdit
              Left = 123
              Top = 254
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 24
              Text = '0'
            end
            object edByBinLimitCountCat6_FT: TEdit
              Left = 123
              Top = 222
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 25
              Text = '0'
            end
            object edByBinLimitCountCat4_FT: TEdit
              Left = 123
              Top = 159
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 26
              Text = '0'
            end
            object cbBybinLimitCat14_FT: TCheckBox
              Left = 8
              Top = 483
              Width = 192
              Height = 14
              Caption = 'Category 14              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 27
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat14_FT: TEdit
              Left = 123
              Top = 475
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 28
              Text = '0'
            end
            object cbBybinLimitCat15_FT: TCheckBox
              Left = 8
              Top = 514
              Width = 192
              Height = 14
              Caption = 'Category 15              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 29
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat15_FT: TEdit
              Left = 123
              Top = 507
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 30
              Text = '0'
            end
            object edByBinLimitCountCatCT_FT: TEdit
              Left = 121
              Top = 561
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 31
              Text = '100'
            end
            object edByBinLimitCountCat5_FT: TEdit
              Left = 123
              Top = 191
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 32
              Text = '0'
            end
          end
        end
        object pnlFT_Left: TPanel
          Left = 0
          Top = 0
          Width = 404
          Height = 858
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          Caption = 'pnlFT_Left'
          Color = 12761254
          TabOrder = 0
          object labLimite: TLabel
            Left = 207
            Top = 749
            Width = 191
            Height = 20
            Caption = '*Count limitation: 1-100000'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object gbPiggyBack: TGroupBox
            Left = 4
            Top = 505
            Width = 396
            Height = 273
            Align = alTop
            Caption = 'Piggy-Back Functions'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object rgPiggyBack_FT: TRadioGroup
              Left = 2
              Top = 22
              Width = 392
              Height = 55
              Align = alTop
              Caption = 'Action'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Alarm'
                'Index Check')
              ParentFont = False
              TabOrder = 0
              OnClick = rgPiggyBack_FTClick
            end
            object palContinuousPass_FT: TPanel
              Left = 2
              Top = 77
              Width = 392
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 1
              object labContinuousPass_FT: TLabel
                Left = 11
                Top = 5
                Width = 210
                Height = 16
                AutoSize = False
                Caption = 'Continual Pass Bin(Total )'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuousPass1_FT: TLabel
                Left = 223
                Top = 25
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuousPass_FT: TCheckBox
                Left = 10
                Top = 29
                Width = 155
                Height = 12
                Caption = 'Enable Pass Bin'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
              end
              object cobContinuousPass_FT: TComboBox
                Left = 170
                Top = 21
                Width = 45
                Height = 28
                ItemHeight = 20
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
                  '10'
                  '11'
                  '12'
                  '13'
                  '14'
                  '15')
              end
              object edContinuousPass_FT: TEdit
                Left = 266
                Top = 21
                Width = 90
                Height = 28
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContinuPassSkt_FT: TPanel
              Left = 2
              Top = 125
              Width = 392
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 2
              object labContinuPassSkt_FT: TLabel
                Left = 11
                Top = 5
                Width = 210
                Height = 20
                AutoSize = False
                Caption = 'Continual Pass Bin( Socket )'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuPassSkt1_FT: TLabel
                Left = 223
                Top = 24
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuPassSkt_FT: TCheckBox
                Left = 10
                Top = 28
                Width = 155
                Height = 12
                Caption = 'Enable Pass Bin'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContinuPassSkt_FT: TEdit
                Left = 266
                Top = 20
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContinuousLoader_FT: TPanel
              Left = 2
              Top = 173
              Width = 392
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 3
              object labContinuousLoader_FT: TLabel
                Left = 11
                Top = 4
                Width = 210
                Height = 16
                AutoSize = False
                Caption = 'Continual Loader'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuousLoader1_FT: TLabel
                Left = 223
                Top = 22
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuousLoad_FT: TCheckBox
                Left = 10
                Top = 26
                Width = 163
                Height = 12
                Caption = 'Enable Loader'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContinuousLoad_FT: TEdit
                Left = 266
                Top = 18
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContactCount_FT: TPanel
              Left = 2
              Top = 221
              Width = 392
              Height = 48
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 4
              object labContactCountFT: TLabel
                Left = 11
                Top = 4
                Width = 210
                Height = 16
                AutoSize = False
                Caption = 'Continual Contact Count'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContactCount1FT: TLabel
                Left = 223
                Top = 20
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContactCountFT: TCheckBox
                Left = 10
                Top = 24
                Width = 191
                Height = 12
                Caption = 'Enable Contact Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContactCountFT: TEdit
                Left = 266
                Top = 16
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
          end
          object gbYieldAlarm: TGroupBox
            Left = 4
            Top = 4
            Width = 396
            Height = 501
            Align = alTop
            Caption = 'Yield Alarm (%)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object pnlChackInterval: TPanel
              Left = 2
              Top = 22
              Width = 392
              Height = 80
              Align = alTop
              Alignment = taRightJustify
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 0
              object lblCheckInteval: TLabel
                Left = 220
                Top = 28
                Width = 163
                Height = 20
                Caption = 'Check Interval : 1 min   '
              end
              object cbYieldAlarmByBin: TCheckBox
                Left = 5
                Top = 0
                Width = 364
                Height = 25
                Caption = 'Enable Yield Alarm by Bin Setting'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clRed
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 0
                OnMouseUp = cbLowYield_FTMouseUp
              end
              object edSlidingWindowSize: TLabeledEdit
                Left = 172
                Top = 52
                Width = 121
                Height = 28
                EditLabel.Width = 129
                EditLabel.Height = 20
                EditLabel.Caption = 'Window Size (ICs)'
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 1
              end
              object cbSlidingWindowYield: TCheckBox
                Left = 21
                Top = 28
                Width = 176
                Height = 24
                Caption = 'Sliding Window Yield'
                TabOrder = 2
                OnMouseUp = cbLowYield_FTMouseUp
              end
            end
            object grpLowYield_FT: TGroupBox
              Left = 2
              Top = 102
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap of Each Site Compare to Total Yield        '#13#10'(Total Yi' +
                'eld - Site Yield[x][y])'
              Align = alTop
              Caption = 'Socket Low Yields'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              object labLowYield1_FT: TLabel
                Left = 141
                Top = 30
                Width = 130
                Height = 20
                Caption = '%  after test count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbLowYield_FT: TCheckBox
                Left = 8
                Top = 33
                Width = 72
                Height = 14
                Caption = 'Enable '
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbLowYield_FTMouseUp
              end
              object edLowYield_FT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object edLowYieldIg_FT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYieldIg_FTMouseDown
              end
            end
            object grpSiteYieldDifferent_FT: TGroupBox
              Left = 2
              Top = 166
              Width = 392
              Height = 88
              Hint = 
                'Yield Gap of Each Site Between 2 Arms                   '#13#10'(Arm1 ' +
                'Site Yield[x][y] - Arm 2 Site Yield[x][y])'
              Align = alTop
              Caption = 'By Arm Per Site Differ Yields'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              object labSiteYieldDifferent1_FT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edSiteYieldDifferentIg_FT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
              object cbSiteYieldDifferent_FT: TCheckBox
                Left = 8
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object edSiteYieldDifferent_FT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object chkAutoSiteOffByArmBySite: TCheckBox
                Left = 23
                Top = 54
                Width = 274
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
            object grpSiteYieldCmp_FT: TGroupBox
              Left = 2
              Top = 254
              Width = 392
              Height = 88
              Hint = 
                'Yield Gap Between Sites                         '#13#10'(Max Site Yiel' +
                'd - Site Yield[x][y])'
              Align = alTop
              Caption = 'By Socket Compare Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              object labSiteYieldCmp1_FT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edSiteYieldCmpIg_FT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
              object edSiteYieldCmp_FT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object cbSiteYieldCmp_FT: TCheckBox
                Left = 8
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object chkAutoSiteOff: TCheckBox
                Left = 23
                Top = 54
                Width = 274
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
            object grpLowYieldByTotal_FT: TGroupBox
              Left = 2
              Top = 342
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap of Total Yield                        '#13#10' (Pass IC Coun' +
                't / Total IC Count)'
              Align = alTop
              Caption = 'Total Low Yields'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              object labLowYieldByTotal1_FT: TLabel
                Left = 141
                Top = 30
                Width = 132
                Height = 20
                Caption = '%  Ignore IC count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbLowYieldByTotal_FT: TCheckBox
                Left = 8
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbLowYieldByTotal_FTMouseUp
              end
              object edLowYieldByTotal_FT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object edLowYieldByTotalIg_FT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object grpLowYieldByPicker_FT: TGroupBox
              Left = 2
              Top = 406
              Width = 392
              Height = 88
              Hint = 
                'Yield Gap Between Pickers                         '#13#10'(Max Picker ' +
                'Yield - Picker Yield[Arm][x][y])'
              Align = alTop
              Caption = 'By Picker Compare Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              object lblLowYieldByPicker_FT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edtLowYieldByPickerIg_FT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
              object edtLowYieldByPicker_FT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object chkLowYieldByPicker_FT: TCheckBox
                Left = 8
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object chkchkAutoSiteOffByPicker: TCheckBox
                Left = 23
                Top = 54
                Width = 274
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
          end
          object grpCreateManualEOCAP: TGroupBox
            Left = 4
            Top = 778
            Width = 396
            Height = 60
            Align = alTop
            Caption = 'Create Manual EOCAP'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object chkCreateManualEOCAP: TCheckBox
              Left = 11
              Top = 34
              Width = 212
              Height = 12
              Caption = 'Enable send jam code'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
          end
        end
        object pnlFT_Center: TPanel
          Left = 404
          Top = 0
          Width = 380
          Height = 858
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 4
          Color = 12761254
          TabOrder = 1
          object gbLoadcell: TGroupBox
            Left = 4
            Top = 568
            Width = 372
            Height = 57
            Align = alTop
            Caption = 'LoadCellCheck'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Visible = False
            object cbLoadCellMeasure: TCheckBox
              Left = 5
              Top = 31
              Width = 163
              Height = 12
              Caption = 'Enable Arm Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object edLoadCellMeasure: TEdit
              Left = 210
              Top = 23
              Width = 90
              Height = 28
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
          end
          object gbAlarm: TGroupBox
            Left = 4
            Top = 4
            Width = 372
            Height = 387
            Align = alTop
            Caption = 'Alarm'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object gbContsFailBySocket_FT: TGroupBox
              Left = 2
              Top = 22
              Width = 368
              Height = 119
              Align = alTop
              Caption = 'Consecutive Failure Alarm ( Socket )'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              object labContsFailBySocket_FT: TLabel
                Left = 9
                Top = 50
                Width = 78
                Height = 16
                Caption = 'Failure Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object rbContsFailBySocket_FTOn: TRadioButton
                Left = 56
                Top = 24
                Width = 81
                Height = 17
                Caption = 'ON'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnMouseUp = rbContsFailBySocket_FTOnMouseUp
              end
              object rbContsFailBySocket_FTOff: TRadioButton
                Left = 184
                Top = 24
                Width = 81
                Height = 17
                Caption = 'Off'
                Checked = True
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                TabStop = True
                OnMouseUp = rbContsFailBySocket_FTOnMouseUp
              end
              object edContsFailSocketAlarmCT_FT: TEdit
                Left = 104
                Top = 45
                Width = 117
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailSocketAlarmCT_FTMouseDown
              end
              object chkAutoSiteOffByContiFail: TCheckBox
                Left = 28
                Top = 69
                Width = 276
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
              object chkSpcBinCntAsContinue_FT: TCheckBox
                Left = 28
                Top = 93
                Width = 276
                Height = 25
                Caption = 'Count special bin continuously'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
              end
            end
            object gbContsFailByHead_FT: TGroupBox
              Left = 2
              Top = 141
              Width = 368
              Height = 120
              Align = alTop
              Caption = 'Consecutive Failure Alarm ( Head)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              object labContsFailByHead_FT: TLabel
                Left = 9
                Top = 58
                Width = 78
                Height = 16
                Caption = 'Failure Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object rbContsFailByHead_FTOn: TRadioButton
                Left = 56
                Top = 24
                Width = 81
                Height = 17
                Caption = 'ON'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnMouseUp = rbContsFailByHead_FTOnMouseUp
              end
              object rbContsFailByHead_FTOff: TRadioButton
                Left = 184
                Top = 24
                Width = 81
                Height = 17
                Caption = 'Off'
                Checked = True
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                TabStop = True
                OnMouseUp = rbContsFailByHead_FTOnMouseUp
              end
              object edContsFailHeadAlarmCT_FT: TEdit
                Left = 104
                Top = 53
                Width = 117
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailHeadAlarmCT_FTMouseDown
              end
              object chkAutoSiteOffByArmContiFail: TCheckBox
                Left = 28
                Top = 83
                Width = 276
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
            object pnlAlarmFT: TPanel
              Left = 2
              Top = 296
              Width = 368
              Height = 89
              Align = alBottom
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 3
              object lblLevel: TLabel
                Left = 6
                Top = 59
                Width = 279
                Height = 16
                AutoSize = False
                Caption = '*. Insufficient privileges , '#27402#38480#19981#36275#28961#27861#20462#25913
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clRed
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object cbAllSiteFail: TCheckBox
                Left = 10
                Top = 29
                Width = 294
                Height = 19
                Caption = 'All Site Fail                                    count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edAllSiteFailCount: TEdit
                Left = 150
                Top = 26
                Width = 90
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailIgnore_FTMouseDown
              end
            end
            object palConsFailIgnore_FT: TPanel
              Left = 2
              Top = 266
              Width = 368
              Height = 30
              Align = alBottom
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 2
              object cbContsFailIgnore_FT: TCheckBox
                Left = 10
                Top = 7
                Width = 264
                Height = 19
                Caption = 'Ignore IC count                           ea'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object edContsFailIgnore_FT: TEdit
                Left = 150
                Top = 2
                Width = 90
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailIgnore_FTMouseDown
              end
            end
          end
          object gbLowYieldAutoClean: TGroupBox
            Left = 4
            Top = 391
            Width = 372
            Height = 177
            Align = alTop
            Caption = 'Auto Yield Monitor'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Visible = False
            object lblAutoCleanLowYieldLimit: TLabel
              Left = 14
              Top = 116
              Width = 286
              Height = 20
              Caption = 'Yield Difference               % Clean Socket'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAutoCleanLowYieldCount: TLabel
              Left = 14
              Top = 85
              Width = 265
              Height = 20
              Caption = 'Refence Yield Setting                Cycles'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblCloseSite: TLabel
              Left = 16
              Top = 142
              Width = 190
              Height = 20
              Caption = 'Close Site                         % '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edAutoCleanLowYieldLimit: TEdit
              Left = 130
              Top = 111
              Width = 53
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edAutoCleanLowYieldLimitMouseDown
            end
            object edAutoCleanLowYieldCount: TEdit
              Left = 181
              Top = 81
              Width = 44
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
            object rgAutoCleanYieldOnOff: TRadioGroup
              Left = 2
              Top = 22
              Width = 368
              Height = 49
              Align = alTop
              Columns = 2
              ItemIndex = 0
              Items.Strings = (
                'OFF'
                'ON')
              TabOrder = 0
              OnClick = edContactCountFTChange
            end
            object eCloseSite: TEdit
              Left = 130
              Top = 142
              Width = 53
              Height = 28
              BiDiMode = bdLeftToRight
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 3
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edAutoCleanLowYieldLimitMouseDown
            end
          end
          object grpSpecialLowYield: TGroupBox
            Left = 4
            Top = 729
            Width = 372
            Height = 126
            Align = alTop
            Caption = 'Special Low Yield'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object lblLowYieldIg_Special1: TLabel
              Left = 13
              Top = 61
              Width = 266
              Height = 20
              Caption = '1 step check interval                         ea '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblLowYieldIg_Special2: TLabel
              Left = 13
              Top = 93
              Width = 266
              Height = 20
              Caption = '2 step check interval                         ea '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbLowYield_Special: TCheckBox
              Left = 10
              Top = 32
              Width = 192
              Height = 14
              Caption = 'Enable               %'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object edLowYield_Special: TEdit
              Left = 88
              Top = 25
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edLowYieldIg_Special1: TEdit
              Left = 169
              Top = 57
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object edLowYieldIg_Special2: TEdit
              Left = 169
              Top = 89
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
          end
          object grpAdaptiveYield_FT: TGroupBox
            Left = 4
            Top = 625
            Width = 372
            Height = 104
            Align = alTop
            Caption = 'Adaptive Low Yield%'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Visible = False
            object lblContsLowerAlarmNor_FT: TLabel
              Left = 14
              Top = 48
              Width = 359
              Height = 20
              Caption = 'continuously lower alarm Normal Area             Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblContsLowerAlarmMin_FT: TLabel
              Left = 215
              Top = 78
              Width = 158
              Height = 20
              Caption = 'Min Area             Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAdaptiveYield_FT: TLabel
              Left = 156
              Top = 17
              Width = 186
              Height = 20
              Caption = 'Max             %  Min           %'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAdaptiveLowYield_FT: TCheckBox
              Left = 13
              Top = 25
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbLowYieldByTotal_FTMouseUp
            end
            object btnResetInterval: TButton
              Left = 16
              Top = 76
              Width = 97
              Height = 25
              Caption = 'Reset Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnClick = btnResetIntervalClick
            end
            object edlContsLowerAlarmNor_FT: TEdit
              Left = 284
              Top = 44
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 3
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edlContsLowerAlarmMin_FT: TEdit
              Left = 284
              Top = 74
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 4
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAdaptiveYieldMax_FT: TEdit
              Left = 188
              Top = 14
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAdaptiveYieldMin_FT: TEdit
              Left = 284
              Top = 14
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
        end
      end
      object tsReTest: TTabSheet
        Caption = 'Re-Test'
        ImageIndex = 1
        object pgcBySiteByBinPercentCompare_RT: TPageControl
          Left = 784
          Top = 0
          Width = 261
          Height = 858
          ActivePage = tsSpecBinByArmPerSiteCompare_RT
          Align = alRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabIndex = 2
          TabOrder = 2
          object tsBinAlarm1_RT: TTabSheet
            Caption = 'Bin Alarm1'
            ImageIndex = 2
            object labByBinFailureCatCT_RT: TLabel
              Left = 55
              Top = 35
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinFailure_RT: TLabel
              Left = 11
              Top = 5
              Width = 107
              Height = 20
              Caption = 'By bin limit RT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByBinFailureCatCT_RT: TEdit
              Left = 165
              Top = 30
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm1_RT: TScrollBox
              Left = 0
              Top = 67
              Width = 253
              Height = 760
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
            end
          end
          object tsSpecBinBySiteCompare_RT: TTabSheet
            Caption = 'Bin Alarm2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            object labByBinSiteGapCatCT_RT: TLabel
              Left = 59
              Top = 35
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinSiteGapCat_RT: TLabel
              Left = 11
              Top = 5
              Width = 136
              Height = 20
              Caption = 'By bin site gap RT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByBinSiteGapCatCT_RT: TEdit
              Left = 169
              Top = 31
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm2_RT: TScrollBox
              Left = 0
              Top = 67
              Width = 253
              Height = 760
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
            end
          end
          object tsSpecBinByArmPerSiteCompare_RT: TTabSheet
            Caption = 'Bin Alarm3'
            ImageIndex = 1
            object labByArmSiteGapCatCT_RT: TLabel
              Left = 59
              Top = 35
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByArmSiteGapCat_RT: TLabel
              Left = 11
              Top = 5
              Width = 187
              Height = 20
              Caption = 'By arm by bin site gap RT :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edByArmSiteGapCatCT_RT: TEdit
              Left = 169
              Top = 31
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYieldIg_FTMouseDown
            end
            object scrlbxBinAlarm3_RT: TScrollBox
              Left = 0
              Top = 67
              Width = 253
              Height = 760
              HorzScrollBar.Visible = False
              Align = alBottom
              TabOrder = 1
            end
          end
          object TabSheet2: TTabSheet
            Caption = 'Bin Alarm4'
            ImageIndex = 3
            object Label11: TLabel
              Left = 11
              Top = 566
              Width = 103
              Height = 20
              Caption = 'Count ignore : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object labByBinLimitCountRT: TLabel
              Left = 11
              Top = 5
              Width = 154
              Height = 20
              Caption = 'By Bin Limit Count RT'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbBybinLimitCat0_RT: TCheckBox
              Left = 8
              Top = 42
              Width = 192
              Height = 14
              Caption = 'Category 0                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat0_RT: TEdit
              Left = 123
              Top = 33
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
            end
            object cbBybinLimitCat1_RT: TCheckBox
              Left = 8
              Top = 73
              Width = 192
              Height = 14
              Caption = 'Category 1                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat1_RT: TEdit
              Left = 123
              Top = 64
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 3
              Text = '0'
            end
            object cbBybinLimitCat2_RT: TCheckBox
              Left = 8
              Top = 105
              Width = 192
              Height = 14
              Caption = 'Category 2                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat3_RT: TCheckBox
              Left = 8
              Top = 136
              Width = 192
              Height = 14
              Caption = 'Category 3                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat4_RT: TCheckBox
              Left = 8
              Top = 168
              Width = 192
              Height = 14
              Caption = 'Category 4                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat3_RT: TEdit
              Left = 123
              Top = 127
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 7
              Text = '0'
            end
            object cbBybinLimitCat5_RT: TCheckBox
              Left = 8
              Top = 199
              Width = 192
              Height = 14
              Caption = 'Category 5                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 8
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat6_RT: TCheckBox
              Left = 8
              Top = 231
              Width = 192
              Height = 14
              Caption = 'Category 6                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 9
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat7_RT: TCheckBox
              Left = 8
              Top = 262
              Width = 192
              Height = 14
              Caption = 'Category 7                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 10
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object cbBybinLimitCat8_RT: TCheckBox
              Left = 8
              Top = 294
              Width = 192
              Height = 14
              Caption = 'Category 8                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 11
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat5_RT: TEdit
              Left = 123
              Top = 191
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 12
              Text = '0'
            end
            object cbBybinLimitCat9_RT: TCheckBox
              Left = 8
              Top = 325
              Width = 192
              Height = 14
              Caption = 'Category 9                ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 13
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat9_RT: TEdit
              Left = 123
              Top = 317
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 14
              Text = '0'
            end
            object cbBybinLimitCat10_RT: TCheckBox
              Left = 8
              Top = 357
              Width = 192
              Height = 14
              Caption = 'Category 10              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 15
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat10_RT: TEdit
              Left = 123
              Top = 349
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 16
              Text = '0'
            end
            object cbBybinLimitCat11_RT: TCheckBox
              Left = 8
              Top = 388
              Width = 192
              Height = 14
              Caption = 'Category 11              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 17
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat11_RT: TEdit
              Left = 123
              Top = 380
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 18
              Text = '0'
            end
            object cbBybinLimitCat12_RT: TCheckBox
              Left = 8
              Top = 420
              Width = 192
              Height = 14
              Caption = 'Category 12              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 19
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat12_RT: TEdit
              Left = 123
              Top = 412
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 20
              Text = '0'
            end
            object cbBybinLimitCat13_RT: TCheckBox
              Left = 8
              Top = 451
              Width = 192
              Height = 14
              Caption = 'Category 13              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 21
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat13_RT: TEdit
              Left = 123
              Top = 444
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 22
              Text = '0'
            end
            object edByBinLimitCountCat2_RT: TEdit
              Left = 123
              Top = 96
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 23
              Text = '0'
            end
            object edByBinLimitCountCat8_RT: TEdit
              Left = 123
              Top = 286
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 24
              Text = '0'
            end
            object edByBinLimitCountCat7_RT: TEdit
              Left = 123
              Top = 254
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 25
              Text = '0'
            end
            object edByBinLimitCountCat6_RT: TEdit
              Left = 123
              Top = 222
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 26
              Text = '0'
            end
            object edByBinLimitCountCat4_RT: TEdit
              Left = 123
              Top = 159
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 27
              Text = '0'
            end
            object cbBybinLimitCat14_RT: TCheckBox
              Left = 8
              Top = 483
              Width = 192
              Height = 14
              Caption = 'Category 14              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 28
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat14_RT: TEdit
              Left = 123
              Top = 475
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 29
              Text = '0'
            end
            object cbBybinLimitCat15_RT: TCheckBox
              Left = 8
              Top = 514
              Width = 192
              Height = 14
              Caption = 'Category 15              ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 30
              OnMouseUp = cbLowYield_FTMouseUp
            end
            object edByBinLimitCountCat15_RT: TEdit
              Left = 123
              Top = 507
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 31
              Text = '0'
            end
            object edByBinLimitCountCatCT_RT: TEdit
              Left = 121
              Top = 561
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 32
              Text = '100'
            end
          end
        end
        object pnlRT_Left: TPanel
          Left = 0
          Top = 0
          Width = 404
          Height = 858
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          Color = 12761254
          TabOrder = 0
          object gbPiggyBackRT: TGroupBox
            Left = 4
            Top = 377
            Width = 396
            Height = 284
            Align = alTop
            Caption = 'Piggy-Back Funtions'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object rgPiggyBack_RT: TRadioGroup
              Left = 2
              Top = 22
              Width = 392
              Height = 52
              Align = alTop
              Caption = 'Action'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Alarm'
                'Index Check')
              ParentFont = False
              TabOrder = 0
              OnClick = rgPiggyBack_FTClick
            end
            object palContinuousPass_RT: TPanel
              Left = 2
              Top = 74
              Width = 392
              Height = 52
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 1
              object labContinuousPass_RT: TLabel
                Left = 11
                Top = 5
                Width = 162
                Height = 16
                AutoSize = False
                Caption = 'Continual Pass Bin(Total )'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuousPass1_RT: TLabel
                Left = 223
                Top = 25
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuousPass_RT: TCheckBox
                Left = 10
                Top = 29
                Width = 155
                Height = 12
                Caption = 'Enable Pass Bin'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
              end
              object cobContinuousPass_RT: TComboBox
                Left = 170
                Top = 21
                Width = 45
                Height = 28
                ItemHeight = 20
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
                  '10'
                  '11'
                  '12'
                  '13'
                  '14'
                  '15')
              end
              object edContinuousPass_RT: TEdit
                Left = 266
                Top = 21
                Width = 90
                Height = 28
                TabOrder = 1
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContinuPassSkt_RT: TPanel
              Left = 2
              Top = 126
              Width = 392
              Height = 52
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 2
              object labContinuPassSkt_RT: TLabel
                Left = 11
                Top = 5
                Width = 174
                Height = 16
                AutoSize = False
                Caption = 'Continual Pass Bin( Socket )'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuPassSkt1_RT: TLabel
                Left = 223
                Top = 24
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuPassSkt_RT: TCheckBox
                Left = 10
                Top = 28
                Width = 159
                Height = 12
                Caption = 'Enable Pass Bin'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContinuPassSkt_RT: TEdit
                Left = 266
                Top = 20
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContinuousLoader_RT: TPanel
              Left = 2
              Top = 178
              Width = 392
              Height = 52
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 3
              object labContinuousLoader_RT: TLabel
                Left = 11
                Top = 4
                Width = 114
                Height = 16
                AutoSize = False
                Caption = 'Continual Loader'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContinuousLoader1_RT: TLabel
                Left = 223
                Top = 22
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContinuousLoad_RT: TCheckBox
                Left = 10
                Top = 26
                Width = 159
                Height = 12
                Caption = 'Enable Loader'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContinuousLoad_RT: TEdit
                Left = 266
                Top = 18
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object palContactCount_RT: TPanel
              Left = 2
              Top = 230
              Width = 392
              Height = 52
              Align = alTop
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 4
              object labContactCountRT: TLabel
                Left = 11
                Top = 4
                Width = 162
                Height = 16
                AutoSize = False
                Caption = 'Continual Contact Count'
                Color = 12761254
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
              end
              object labContactCount1RT: TLabel
                Left = 223
                Top = 20
                Width = 40
                Height = 20
                Caption = 'count'
              end
              object cbContactCountRT: TCheckBox
                Left = 10
                Top = 24
                Width = 187
                Height = 12
                Caption = 'Enable Contact Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object edContactCountRT: TEdit
                Left = 266
                Top = 16
                Width = 90
                Height = 28
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
          end
          object gbYieldAlarmRT: TGroupBox
            Left = 4
            Top = 4
            Width = 396
            Height = 373
            Align = alTop
            Caption = 'Yield Alarm (%)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object grpLowYield_RT: TGroupBox
              Left = 2
              Top = 22
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap of Each Site Compare to Total Yield        '#13#10'(Total Yi' +
                'eld - Site Yield[x][y])'
              Align = alTop
              Caption = 'Socket Low Yields'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              object labLowYield1_RT: TLabel
                Left = 141
                Top = 30
                Width = 130
                Height = 20
                Caption = '%  after test count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbLowYield_RT: TCheckBox
                Left = 6
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable '
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbLowYield_RTMouseUp
              end
              object edLowYield_RT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object edLowYieldIg_RT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYieldIg_FTMouseDown
              end
            end
            object grpSiteYieldDifferent_RT: TGroupBox
              Left = 2
              Top = 86
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap of Each Site Between 2 Arms                   '#13#10'(Arm1 ' +
                'Site Yield[x][y] - Arm 2 Site Yield[x][y])'
              Align = alTop
              Caption = 'By Arm Per Site Differ Yields'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              object labSiteYieldDifferent1_RT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edSiteYieldDifferentIg_RT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
              object cbSiteYieldDifferent_RT: TCheckBox
                Left = 6
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldDifferent_RTMouseUp
              end
              object edSiteYieldDifferent_RT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
            end
            object grpSiteYieldCmp_RT: TGroupBox
              Left = 2
              Top = 150
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap Between Sites                         '#13#10'(Max Site Yiel' +
                'd - Site Yield[x][y])'
              Align = alTop
              Caption = 'By Socket Compare Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              object labSiteYieldCmp1_RT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbSiteYieldCmp_RT: TCheckBox
                Left = 6
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldCmp_RTMouseUp
              end
              object edSiteYieldCmp_RT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object edSiteYieldCmpIg_RT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object grpLowYieldByTotal_RT: TGroupBox
              Left = 2
              Top = 214
              Width = 392
              Height = 64
              Hint = 
                'Yield Gap of Total Yield                        '#13#10' (Pass IC Coun' +
                't / Total IC Count)'
              Align = alTop
              Caption = 'Total Low Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              object labLowYieldByTotal1_RT: TLabel
                Left = 141
                Top = 30
                Width = 132
                Height = 20
                Caption = '%  Ignore IC count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbLowYieldByTotal_RT: TCheckBox
                Left = 6
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbLowYieldByTotal_RTMouseUp
              end
              object edLowYieldByTotal_RT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object edLowYieldByTotalIg_RT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
            end
            object grpLowYieldByPicker_RT: TGroupBox
              Left = 2
              Top = 278
              Width = 392
              Height = 88
              Hint = 
                'Yield Gap Between Pickers                         '#13#10'(Max Picker ' +
                'Yield - Picker Yield[Arm][x][y])'
              Align = alTop
              Caption = 'By Picker Compare Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              object lblLowYieldByPicker_RT: TLabel
                Left = 141
                Top = 30
                Width = 156
                Height = 20
                Caption = '%  after contact count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edtLowYieldByPickerIg_RT: TEdit
                Left = 304
                Top = 24
                Width = 80
                Height = 28
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContactCountFTMouseDown
              end
              object edtLowYieldByPicker_RT: TEdit
                Left = 84
                Top = 24
                Width = 52
                Height = 28
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnChange = edContactCountFTChange
                OnMouseDown = edLowYield_RTMouseDown
              end
              object chkLowYieldByPicker_RT: TCheckBox
                Left = 8
                Top = 30
                Width = 72
                Height = 14
                Caption = 'Enable'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object chkchkAutoSiteOffByPicker_RT: TCheckBox
                Left = 23
                Top = 54
                Width = 274
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
          end
        end
        object pnlRT_Center: TPanel
          Left = 404
          Top = 0
          Width = 380
          Height = 858
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 4
          Color = 12761254
          TabOrder = 1
          object gbAlarmRT: TGroupBox
            Left = 4
            Top = 4
            Width = 372
            Height = 361
            Align = alTop
            Caption = 'Alarm'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object lblAlarmLevelRT: TLabel
              Left = 6
              Top = 316
              Width = 279
              Height = 16
              AutoSize = False
              Caption = '*. Insufficient privileges , '#27402#38480#19981#36275#28961#27861#20462#25913
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object gbContsFailBySocket_RT: TGroupBox
              Left = 2
              Top = 22
              Width = 368
              Height = 115
              Align = alTop
              Caption = 'Consecutive Failure Alarm ( Socket )'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              object labContsFailBySocket_RT: TLabel
                Left = 9
                Top = 58
                Width = 78
                Height = 16
                Caption = 'Failure Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object rbContsFailBySocket_RTOn: TRadioButton
                Left = 56
                Top = 24
                Width = 81
                Height = 17
                Caption = 'ON'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnMouseUp = rbContsFailBySocket_RTOnMouseUp
              end
              object rbContsFailBySocket_RTOff: TRadioButton
                Left = 184
                Top = 24
                Width = 81
                Height = 17
                Caption = 'Off'
                Checked = True
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                TabStop = True
                OnMouseUp = rbContsFailBySocket_RTOnMouseUp
              end
              object edContsFailSocketAlarmCT_RT: TEdit
                Left = 116
                Top = 53
                Width = 117
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailSocketAlarmCT_RTMouseDown
              end
              object chkSpcBinCntAsContinue_RT: TCheckBox
                Left = 28
                Top = 86
                Width = 276
                Height = 25
                Caption = 'Trigger with Auto Site Off Func.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clMaroon
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
            end
            object gbContsFailByHead_RT: TGroupBox
              Left = 2
              Top = 137
              Width = 368
              Height = 88
              Align = alTop
              Caption = 'Consecutive Failure Alarm ( Head)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              object labContsFailByHead_RT: TLabel
                Left = 9
                Top = 58
                Width = 78
                Height = 16
                Caption = 'Failure Count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object rbContsFailByHead_RTOn: TRadioButton
                Left = 56
                Top = 24
                Width = 81
                Height = 17
                Caption = 'ON'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnMouseUp = rbContsFailByHead_RTOnMouseUp
              end
              object rbContsFailByHead_RTOff: TRadioButton
                Left = 184
                Top = 24
                Width = 81
                Height = 17
                Caption = 'Off'
                Checked = True
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                TabStop = True
                OnMouseUp = rbContsFailByHead_RTOnMouseUp
              end
              object edContsFailHeadAlarmCT_RT: TEdit
                Left = 116
                Top = 53
                Width = 117
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailHeadAlarmCT_RTMouseDown
              end
            end
            object palConsFailIgnore_RT: TPanel
              Left = 12
              Top = 236
              Width = 269
              Height = 30
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 2
              object cbContsFailIgnore_RT: TCheckBox
                Left = 5
                Top = 7
                Width = 264
                Height = 19
                Caption = 'Ignore IC count                           ea'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnMouseUp = cbSiteYieldDifferent_FTMouseUp
              end
              object edContsFailIgnore_RT: TEdit
                Left = 141
                Top = 2
                Width = 97
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                Text = '100'
                OnChange = edContactCountFTChange
                OnMouseDown = edContsFailIgnore_FTMouseDown
              end
            end
            object cbAllSiteFail_RT: TCheckBox
              Left = 17
              Top = 291
              Width = 264
              Height = 19
              Caption = 'All Site Fail'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
            end
            object edAllSiteFailCountRT: TEdit
              Left = 150
              Top = 287
              Width = 97
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContsFailIgnore_FTMouseDown
            end
          end
          object grpAdaptiveYield_RT: TGroupBox
            Left = 4
            Top = 365
            Width = 372
            Height = 105
            Align = alTop
            Caption = 'Adaptive Low Yield%'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Visible = False
            object lbllContsLowerAlarmNor_RT: TLabel
              Left = 14
              Top = 49
              Width = 359
              Height = 20
              Caption = 'continuously lower alarm Normal Area             Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbllContsLowerAlarmMin_RT: TLabel
              Left = 215
              Top = 79
              Width = 158
              Height = 20
              Caption = 'Min Area             Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAdaptiveYield_RT: TLabel
              Left = 156
              Top = 17
              Width = 186
              Height = 20
              Caption = 'Max             %  Min           %'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAdaptiveLowYield_RT: TCheckBox
              Left = 13
              Top = 25
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbLowYieldByTotal_FTMouseUp
            end
            object edlContsLowerAlarmNor_RT: TEdit
              Left = 284
              Top = 44
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 3
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edlContsLowerAlarmMin_RT: TEdit
              Left = 284
              Top = 74
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 4
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAdaptiveYieldMin_RT: TEdit
              Left = 284
              Top = 14
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAdaptiveYieldMax_RT: TEdit
              Left = 188
              Top = 14
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
        end
      end
      object tsAutoRetest: TTabSheet
        Caption = 'AutoRetest'
        ImageIndex = 2
        object edInput: TEdit
          Left = 824
          Top = 24
          Width = 121
          Height = 28
          TabOrder = 0
          Text = 'edInput'
        end
      end
      object tsAutoRetest1: TTabSheet
        Caption = 'AutoRetest'
        ImageIndex = 3
        object gbA10: TGroupBox
          Left = 0
          Top = 0
          Width = 385
          Height = 858
          Align = alLeft
          Caption = ' Auto retest parameter (ART)'
          TabOrder = 0
          object lblARTLimit: TLabel
            Left = 12
            Top = 32
            Width = 110
            Height = 20
            Caption = 'Auto retest limit'
          end
          object lblAutoLeastRetestLimitFile: TLabel
            Left = 20
            Top = 204
            Width = 139
            Height = 20
            Caption = 'Auto retest min limit'
          end
          object edAutoRetestLimitFile: TEdit
            Left = 168
            Top = 29
            Width = 73
            Height = 28
            TabOrder = 0
            OnChange = edContactCountFTChange
            OnMouseDown = edAutoRetestLimitFileMouseDown
          end
          object edAutoLeastRetestLimitFile: TEdit
            Left = 168
            Top = 201
            Width = 73
            Height = 28
            TabOrder = 4
            OnChange = edContactCountFTChange
            OnMouseDown = edAutoLeastRetestLimitFileMouseDown
          end
          object ckUseLeastRetestTimes: TCheckBox
            Left = 20
            Top = 168
            Width = 269
            Height = 17
            Caption = 'Use Least Retest Times'
            TabOrder = 3
          end
          object pnlARTFailYiel: TPanel
            Left = 8
            Top = 60
            Width = 269
            Height = 45
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object lblFailYieldRate_ARTFile: TLabel
              Left = 4
              Top = 12
              Width = 146
              Height = 20
              Caption = ' Fail yield rate >= (%)'
            end
            object edFailYieldRate_ARTFile: TEdit
              Left = 160
              Top = 9
              Width = 73
              Height = 28
              TabOrder = 0
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldRate_ARTFTFileMouseDown
            end
          end
          object palART_FT: TPanel
            Left = 8
            Top = 240
            Width = 361
            Height = 153
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 5
            object rgFT_ART: TRadioGroup
              Left = 0
              Top = 0
              Width = 361
              Height = 153
              Align = alClient
              Caption = 'FT'
              ItemIndex = 0
              Items.Strings = (
                'Pass yield rate                                               %'
                'Pass yield rate                     '#65374'                     %')
              TabOrder = 0
              OnClick = rgPiggyBack_FTClick
            end
            object cbUseFTFailYieldModel: TComboBox
              Left = 136
              Top = 40
              Width = 73
              Height = 28
              ItemHeight = 20
              TabOrder = 1
              Items.Strings = (
                '>='
                '>'
                '<'
                '<=')
            end
            object edFailYieldRate_ARTFTFile: TEdit
              Left = 236
              Top = 40
              Width = 73
              Height = 28
              TabOrder = 2
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldRate_ARTFTFileMouseDown
            end
            object edFailYieldMinRate_ARTFTFile: TEdit
              Left = 136
              Top = 96
              Width = 73
              Height = 28
              TabOrder = 3
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldRate_ARTFTFileMouseDown
            end
            object edFailYieldMaxRate_ARTFTFile: TEdit
              Left = 236
              Top = 96
              Width = 73
              Height = 28
              TabOrder = 4
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldMaxRate_ARTFTFileMouseDown
            end
          end
          object palART_RT: TPanel
            Left = 8
            Top = 408
            Width = 361
            Height = 169
            BevelOuter = bvNone
            Caption = 'palART_RT'
            Color = 12761254
            TabOrder = 6
            object rgRT_ART: TRadioGroup
              Left = 0
              Top = 0
              Width = 361
              Height = 169
              Align = alClient
              Caption = 'RT'
              ItemIndex = 0
              Items.Strings = (
                'Pass yield rate                                               %'
                'Pass yield rate                     '#65374'                     %')
              TabOrder = 0
              OnClick = rgPiggyBack_FTClick
            end
            object edFailYieldRate_ARTRTFile: TEdit
              Left = 237
              Top = 44
              Width = 73
              Height = 28
              TabOrder = 2
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldRate_ARTFTFileMouseDown
            end
            object edFailYieldMinRate_ARTRTFile: TEdit
              Left = 135
              Top = 108
              Width = 73
              Height = 28
              TabOrder = 3
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldRate_ARTFTFileMouseDown
            end
            object edFailYieldMaxRate_ARTRTFile: TEdit
              Left = 237
              Top = 108
              Width = 73
              Height = 28
              TabOrder = 4
              OnChange = edContactCountFTChange
              OnMouseDown = edFailYieldMaxRate_ARTRTFileMouseDown
            end
            object cbUseRTFailYieldModel: TComboBox
              Left = 136
              Top = 44
              Width = 73
              Height = 28
              ItemHeight = 20
              TabOrder = 1
              Items.Strings = (
                '>='
                '>'
                '<'
                '<=')
            end
          end
          object ckUseFailNoDistinction: TCheckBox
            Left = 20
            Top = 140
            Width = 269
            Height = 17
            Caption = 'Use Use Fail No Distinction'
            TabOrder = 2
          end
        end
      end
      object tsAlarm4: TTabSheet
        Caption = 'Alarm 4 / 5'
        ImageIndex = 4
        object grpAlarm4: TGroupBox
          Left = 0
          Top = 0
          Width = 404
          Height = 858
          Align = alLeft
          Caption = 'Alarm 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object grpAlarm4_SiteYieldOverAlert: TGroupBox
            Left = 2
            Top = 425
            Width = 400
            Height = 64
            Align = alTop
            Caption = 'Site/Head Yield % Over Alert '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object lblSiteYieldOverAlertCount: TLabel
              Left = 137
              Top = 30
              Width = 132
              Height = 20
              Caption = '%  Ignore IC count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cb_SiteYieldOverAlert: TCheckBox
              Left = 8
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object ed_SiteYieldOverAlert: TEdit
              Left = 84
              Top = 24
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object ed_SiteYieldOverAlertCount: TEdit
              Left = 284
              Top = 24
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
          end
          object grpAlarm4_ContinueType: TGroupBox
            Left = 2
            Top = 22
            Width = 400
            Height = 123
            Align = alTop
            Caption = 'Continue Type Yield Monitorning'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object lbAlarm4IntervalCount: TLabel
              Left = 170
              Top = 60
              Width = 99
              Height = 20
              Alignment = taRightJustify
              Caption = 'Interval Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbAlarm4ContinueCount: TLabel
              Left = 158
              Top = 89
              Width = 111
              Height = 20
              Alignment = taRightJustify
              Caption = 'Continue Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAlarm4ContinueType: TCheckBox
              Left = 8
              Top = 30
              Width = 177
              Height = 17
              Hint = 'Tester_Alarm4 Site To Site Continue Type Select'
              Caption = 'Enable Function'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object edAlarm4IntervalCount: TEdit
              Left = 284
              Top = 54
              Width = 80
              Height = 28
              Hint = 'Tester_Alarm4 Site To Site Interval Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
            object edAlarm4ContinueCount: TEdit
              Left = 284
              Top = 87
              Width = 80
              Height = 28
              Hint = 'Tester_Alarm4 Site To Site Continue Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnChange = edContactCountFTChange
              OnMouseDown = edAlarm4ContinueCountMouseDown
            end
          end
          object grAlarm4_SiteToSiteYieldEnable: TGroupBox
            Left = 2
            Top = 297
            Width = 400
            Height = 64
            Align = alTop
            Caption = 'Site To Site Yield % '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object lblSiteToSiteYieldCount: TLabel
              Left = 137
              Top = 30
              Width = 132
              Height = 20
              Caption = '%  Ignore IC count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cb_SiteToSiteYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object ed_SiteToSiteYield: TEdit
              Left = 84
              Top = 24
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object ed_SiteToSiteYieldCount: TEdit
              Left = 284
              Top = 24
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
          end
          object grpAlarm4_HeadToHeadYieldEnable: TGroupBox
            Left = 2
            Top = 361
            Width = 400
            Height = 64
            Align = alTop
            Caption = 'Head To Head Yield % '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object lblHeadToHeadYieldCount: TLabel
              Left = 137
              Top = 30
              Width = 132
              Height = 20
              Caption = '%  Ignore IC count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cb_HeadToHeadYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object ed_HeadToHeadYield: TEdit
              Left = 84
              Top = 24
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object ed_HeadToHeadYieldCount: TEdit
              Left = 284
              Top = 24
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
          end
          object grpAlarm4_EnableIntervalYield: TGroupBox
            Left = 2
            Top = 145
            Width = 400
            Height = 152
            Align = alTop
            Caption = 'Interval Total Yield Difference'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object lblAlarm4IntervalYieldIntervalCount: TLabel
              Left = 170
              Top = 84
              Width = 99
              Height = 20
              Alignment = taRightJustify
              Caption = 'Interval Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAlarm4IntervalYieldContinueCount: TLabel
              Left = 158
              Top = 116
              Width = 111
              Height = 20
              Alignment = taRightJustify
              Caption = 'Continue Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAlarm4IntervalYieldYield: TLabel
              Left = 234
              Top = 53
              Width = 35
              Height = 20
              Alignment = taRightJustify
              Caption = 'Yield'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cb_Alarm4EnableIntervalYield: TCheckBox
              Left = 8
              Top = 30
              Width = 153
              Height = 17
              Hint = 'Tester_Alarm4 Interval Total Yield Different Select'
              Caption = 'Enable Function'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object ed_Alarm4IntervalYieldIntervalCount: TEdit
              Left = 284
              Top = 80
              Width = 80
              Height = 28
              Hint = 'Tester_Alarm4 Interval Total Yield Different Interval Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
            object ed_Alarm4IntervalYieldContinueCount: TEdit
              Left = 284
              Top = 112
              Width = 80
              Height = 28
              Hint = 'Tester_Alarm4 Interval Total Yield Different Continue Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edAlarm4ContinueCountMouseDown
            end
            object ed_Alarm4IntervalYieldYield: TEdit
              Left = 284
              Top = 49
              Width = 80
              Height = 28
              Hint = 'Tester_Alarm4 Interval Total Yield Different'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
        end
        object pnlAlarm_Center: TPanel
          Left = 404
          Top = 0
          Width = 12
          Height = 858
          Align = alLeft
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 1
        end
        object grpAlarm5: TGroupBox
          Left = 416
          Top = 0
          Width = 629
          Height = 858
          Align = alClient
          Caption = 'Alarm 5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object pnlAlarm5_BySiteIntervalContactCnt: TPanel
            Left = 2
            Top = 22
            Width = 625
            Height = 51
            Align = alTop
            BevelOuter = bvNone
            BorderWidth = 1
            Color = 12761254
            TabOrder = 0
            object lblAlarm5_BySiteIntervalContactCnt: TLabel
              Left = 13
              Top = 10
              Width = 213
              Height = 20
              Caption = 'By Site Interval Contact Count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edAlarm5_BySiteIntervalContactCnt: TEdit
              Left = 241
              Top = 4
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edContactCountFTMouseDown
            end
          end
          object grpAlarm5_BySiteLowYield: TGroupBox
            Left = 2
            Top = 73
            Width = 625
            Height = 64
            Align = alTop
            Caption = 'By Site Low Yield% '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object lblAlarm5_BySiteLowYieldRej: TLabel
              Left = 230
              Top = 30
              Width = 46
              Height = 20
              Caption = 'Reject'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAlarm5_BySiteLowYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 130
              Height = 14
              Caption = 'Enable Alarm'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object edAlarm5_BySiteLowYield: TEdit
              Left = 130
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAlarm5_BySiteLowYieldRej: TEdit
              Left = 280
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
          object grpAlarm5_BySiteCmpYield: TGroupBox
            Left = 2
            Top = 137
            Width = 625
            Height = 64
            Align = alTop
            Caption = 'By Site Compare Yield% '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object lblAlarm5_BySiteCmpYieldRej: TLabel
              Left = 230
              Top = 30
              Width = 46
              Height = 20
              Caption = 'Reject'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAlarm5_BySiteCmpYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 130
              Height = 14
              Caption = 'Enable Alarm'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object edAlarm5_BySiteCmpYield: TEdit
              Left = 130
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAlarm5_BySiteCmpYieldRej: TEdit
              Left = 280
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
          object grpAlarm5_BySiteAlarmYield: TGroupBox
            Left = 2
            Top = 201
            Width = 625
            Height = 64
            Align = alTop
            Caption = 'By Site Alarm Yield O/S %'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object lblAlarm5_OSBin: TLabel
              Left = 374
              Top = 30
              Width = 54
              Height = 20
              Caption = 'O/S Bin'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblAlarm5_BySiteAlarmYieldRej: TLabel
              Left = 230
              Top = 30
              Width = 46
              Height = 20
              Caption = 'Reject'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAlarm5_BySiteAlarmYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 130
              Height = 14
              Caption = 'Enable Alarm'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object edAlarm5_BySiteAlarmYield: TEdit
              Left = 130
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAlarm5_BySiteAlarmYieldRej: TEdit
              Left = 280
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAlarm5_OSBin: TEdit
              Left = 432
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edAlarm5_OSBinMouseDown
            end
          end
          object grpAlarm5_BySitePreCmpYieldEnable: TGroupBox
            Left = 2
            Top = 265
            Width = 625
            Height = 64
            Align = alTop
            Caption = 'By Site Previous Yield Difference%'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object lblAlarm5_BySitePreCmpYieldRej: TLabel
              Left = 230
              Top = 30
              Width = 46
              Height = 20
              Caption = 'Reject'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAlarm5_BySitePreCmpYieldEnable: TCheckBox
              Left = 8
              Top = 30
              Width = 130
              Height = 14
              Caption = 'Enable Alarm'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbSiteYieldDifferent_FTMouseUp
            end
            object edAlarm5_BySitePreCmpYield: TEdit
              Left = 130
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edAlarm5_BySitePreCmpYieldRej: TEdit
              Left = 280
              Top = 24
              Width = 80
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
          end
        end
      end
      object tsYield: TTabSheet
        Caption = 'Yield'
        ImageIndex = 7
        object gbOtherYieldAlarmFT: TGroupBox
          Left = 0
          Top = 0
          Width = 485
          Height = 858
          Align = alLeft
          Caption = 'Yield Alarm FT'
          TabOrder = 0
          object palIntervalLowYeildBySite_FT: TPanel
            Left = 18
            Top = 31
            Width = 401
            Height = 53
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object lblIntervalLowYieldBySite_FTCap: TLabel
              Left = 7
              Top = 3
              Width = 173
              Height = 16
              Caption = 'Interval Low Yields%(By Site)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblIntervalLowYieldBySite_FT: TLabel
              Left = 137
              Top = 27
              Width = 156
              Height = 20
              Caption = '%  after contact count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbIntervalLowYieldBySite_FT: TCheckBox
              Left = 5
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbIntervalLowYieldBySite_FTMouseUp
            end
            object edIntervalLowYieldBySite_FT: TEdit
              Left = 84
              Top = 23
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edIntervalLowYieldBySiteIg_FT: TEdit
              Left = 297
              Top = 23
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edIntervalLowYieldBySiteIg_FTMouseDown
            end
          end
          object palIntervalLowYeildByTotal_FT: TPanel
            Left = 18
            Top = 95
            Width = 401
            Height = 53
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object lblIntervalLowYieldByTotal_FTCap: TLabel
              Left = 7
              Top = 3
              Width = 181
              Height = 16
              Caption = 'Interval Low Yields%(By Total)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblIntervalLowYieldByTotal_FT: TLabel
              Left = 137
              Top = 27
              Width = 130
              Height = 20
              Caption = '%  after test count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbIntervalLowYieldByTotal_FT: TCheckBox
              Left = 5
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbIntervalLowYieldByTotal_FTMouseUp
            end
            object edIntervalLowYieldByTotal_FT: TEdit
              Left = 84
              Top = 23
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edIntervalLowYieldByTotalIg_FT: TEdit
              Left = 297
              Top = 23
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edIntervalLowYieldBySiteIg_FTMouseDown
            end
          end
        end
        object gbOtherYieldAlarmRT: TGroupBox
          Left = 560
          Top = 0
          Width = 485
          Height = 858
          Align = alRight
          Caption = 'Yield Alarm RT'
          TabOrder = 1
          object palIntervalLowYeildBySite_RT: TPanel
            Left = 18
            Top = 31
            Width = 397
            Height = 53
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object lblIntervalLowYieldBySite_RTCap: TLabel
              Left = 7
              Top = 3
              Width = 176
              Height = 16
              Caption = 'Interval Low Yields%(By Sitel)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblIntervalLowYieldBySite_RT: TLabel
              Left = 137
              Top = 27
              Width = 156
              Height = 20
              Caption = '%  after contact count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbIntervalLowYieldBySite_RT: TCheckBox
              Left = 5
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbIntervalLowYieldBySite_RTMouseUp
            end
            object edIntervalLowYieldBySite_RT: TEdit
              Left = 84
              Top = 23
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edIntervalLowYieldBySiteIg_RT: TEdit
              Left = 297
              Top = 23
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edIntervalLowYieldBySiteIg_FTMouseDown
            end
          end
          object palIntervalLowYeildByTotal_RT: TPanel
            Left = 18
            Top = 95
            Width = 397
            Height = 53
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object lblIntervalLowYieldByTotal_RTCap: TLabel
              Left = 7
              Top = 3
              Width = 181
              Height = 16
              Caption = 'Interval Low Yields%(By Total)'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblIntervalLowYieldByTotal_RT: TLabel
              Left = 137
              Top = 27
              Width = 130
              Height = 20
              Caption = '%  after test count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbIntervalLowYieldByTotal_RT: TCheckBox
              Left = 5
              Top = 30
              Width = 80
              Height = 14
              Caption = 'Enable'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnMouseUp = cbIntervalLowYieldByTotal_RTMouseUp
            end
            object edIntervalLowYieldByTotal_RT: TEdit
              Left = 84
              Top = 23
              Width = 44
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edLowYield_RTMouseDown
            end
            object edIntervalLowYieldByTotalIg_RT: TEdit
              Left = 297
              Top = 23
              Width = 80
              Height = 28
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '100'
              OnChange = edContactCountFTChange
              OnMouseDown = edIntervalLowYieldBySiteIg_FTMouseDown
            end
          end
        end
      end
      object tsAutoSiteOff: TTabSheet
        Caption = 'Auto Site Off'
        ImageIndex = 6
        object gbAutoSiteOff: TGroupBox
          Left = 0
          Top = 0
          Width = 493
          Height = 858
          Align = alLeft
          Caption = 'Auto Site Off When Production'
          TabOrder = 0
          object labSiteYieldCmp1SiteCnt_FT: TLabel
            Left = 7
            Top = 29
            Width = 239
            Height = 20
            Caption = 'Alarm if site on count is less than :'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtSiteYieldCmp1SiteCnt_FT: TEdit
            Left = 251
            Top = 25
            Width = 80
            Height = 28
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '2'
            OnMouseDown = edtSiteYieldCmp1SiteCnt_FTMouseDown
          end
          object rgCloseSiteOnHPAction: TRadioGroup
            Left = 2
            Top = 60
            Width = 483
            Height = 125
            Caption = 'Close site device on remained IC'
            Ctl3D = True
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Keep Testing'
              'Don'#39't Testing, place to error bin'
              'Don'#39't Testing, place to setted bin.')
            ParentCtl3D = False
            ParentFont = False
            TabOrder = 1
            OnClick = rgPiggyBack_FTClick
          end
          object edtAutoSiteOffAlarm: TEdit
            Left = 387
            Top = 261
            Width = 80
            Height = 28
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '2'
            OnMouseDown = edtAutoSiteOffAlarmMouseDown
          end
          object chkAutoSiteOffAlarm: TCheckBox
            Left = 11
            Top = 263
            Width = 374
            Height = 25
            Caption = 'Alarm several times and then close the Site'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnMouseUp = cbLowYield_FTMouseUp
          end
          object cbbClosedSiteBin: TComboBox
            Left = 272
            Top = 152
            Width = 105
            Height = 28
            ItemHeight = 20
            ItemIndex = 0
            TabOrder = 2
            Text = '0'
            Items.Strings = (
              '0'
              '1'
              '2'
              '3')
          end
        end
        object pnlAutoSiteOff_Right: TPanel
          Left = 493
          Top = 0
          Width = 552
          Height = 858
          Align = alClient
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 1
          object grpAutoSiteOffWhenRT: TGroupBox
            Left = 0
            Top = 0
            Width = 552
            Height = 129
            Align = alTop
            Caption = 'Auto Site Off When Change to RT'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Visible = False
            object lblAutoSiteOffWhenRT: TLabel
              Left = 16
              Top = 78
              Width = 254
              Height = 20
              Caption = 'Yield difference               % close site'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object rgAutoSiteOffWhenRT: TRadioGroup
              Left = 8
              Top = 16
              Width = 293
              Height = 49
              Columns = 2
              ItemIndex = 0
              Items.Strings = (
                'OFF'
                'ON')
              TabOrder = 0
              OnClick = edContactCountFTChange
            end
            object edtAutoSiteOffWhenRT: TEdit
              Left = 130
              Top = 74
              Width = 53
              Height = 28
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnChange = edContactCountFTChange
              OnMouseDown = edtAutoSiteOffWhenRTMouseDown
            end
          end
          object rgAutoSiteOn: TRadioGroup
            Left = 0
            Top = 129
            Width = 552
            Height = 72
            Align = alTop
            Caption = 'Auto Site On When Change to Initial Start'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'OFF'
              'ON')
            ParentFont = False
            TabOrder = 1
            OnClick = edContactCountFTChange
          end
          object rgAutoSiteOnByGPIB: TRadioGroup
            Left = 0
            Top = 201
            Width = 552
            Height = 72
            Align = alTop
            Caption = 'Auto Site On by GPIB'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'OFF'
              'ON')
            ParentFont = False
            TabOrder = 2
            OnClick = edContactCountFTChange
          end
        end
      end
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 964
    Top = 65535
  end
end
