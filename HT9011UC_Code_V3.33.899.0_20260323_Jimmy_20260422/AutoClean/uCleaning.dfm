object fCleaning: TfCleaning
  Left = 167
  Top = 42
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Cleaning'
  ClientHeight = 847
  ClientWidth = 996
  Color = 14670284
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object grpTrayData: TGroupBox
    Left = 660
    Top = 0
    Width = 336
    Height = 847
    Align = alRight
    Caption = 'Plate of Use  (Unit : mm)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object pcCleanYield: TPageControl
      Left = 2
      Top = 628
      Width = 332
      Height = 217
      ActivePage = tsOffset
      Align = alBottom
      TabIndex = 0
      TabOrder = 2
      object tsOffset: TTabSheet
        Caption = 'Offset'
        object grpBufferKitLT: TGroupBox
          Left = 12
          Top = 6
          Width = 137
          Height = 77
          Caption = 'Buffer Kit Left-Top'
          TabOrder = 0
          object Label30: TLabel
            Left = 8
            Top = 24
            Width = 8
            Height = 16
            Caption = 'X'
          end
          object Label31: TLabel
            Left = 105
            Top = 24
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object Label32: TLabel
            Left = 8
            Top = 52
            Width = 9
            Height = 16
            Caption = 'Y'
          end
          object Label33: TLabel
            Left = 105
            Top = 52
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object edtBufferKitLTX: TEdit
            Left = 20
            Top = 20
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtBufferKitLTY: TEdit
            Left = 20
            Top = 48
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
        end
        object grpBufferKitRT: TGroupBox
          Left = 152
          Top = 6
          Width = 137
          Height = 77
          Caption = 'Buffer Kit Right-Top'
          TabOrder = 1
          object Label26: TLabel
            Left = 8
            Top = 24
            Width = 8
            Height = 16
            Caption = 'X'
          end
          object Label27: TLabel
            Left = 105
            Top = 24
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object Label28: TLabel
            Left = 8
            Top = 52
            Width = 9
            Height = 16
            Caption = 'Y'
          end
          object Label29: TLabel
            Left = 105
            Top = 52
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object edtBufferKitRTX: TEdit
            Left = 20
            Top = 20
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtBufferKitRTY: TEdit
            Left = 20
            Top = 48
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
        end
        object grpBufferKitLD: TGroupBox
          Left = 12
          Top = 90
          Width = 137
          Height = 77
          Caption = 'Buffer Kit Left-Down'
          TabOrder = 2
          object Label22: TLabel
            Left = 8
            Top = 24
            Width = 8
            Height = 16
            Caption = 'X'
          end
          object Label23: TLabel
            Left = 105
            Top = 24
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object Label24: TLabel
            Left = 8
            Top = 52
            Width = 9
            Height = 16
            Caption = 'Y'
          end
          object Label25: TLabel
            Left = 105
            Top = 52
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object edtBufferKitLDX: TEdit
            Left = 20
            Top = 20
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtBufferKitLDY: TEdit
            Left = 20
            Top = 48
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
        end
        object grpBufferKitRD: TGroupBox
          Left = 152
          Top = 90
          Width = 137
          Height = 77
          Caption = 'Buffer Kit Right-Down'
          TabOrder = 3
          object Label34: TLabel
            Left = 8
            Top = 24
            Width = 8
            Height = 16
            Caption = 'X'
          end
          object Label35: TLabel
            Left = 105
            Top = 24
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object Label36: TLabel
            Left = 8
            Top = 52
            Width = 9
            Height = 16
            Caption = 'Y'
          end
          object Label37: TLabel
            Left = 105
            Top = 52
            Width = 22
            Height = 16
            Caption = 'mm'
          end
          object edtBufferKitRDX: TEdit
            Left = 20
            Top = 20
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtBufferKitRDY: TEdit
            Left = 20
            Top = 48
            Width = 81
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
        end
      end
      object tsYield: TTabSheet
        Caption = 'Yield'
        ImageIndex = 1
        object gbYieldAlarm: TGroupBox
          Left = 0
          Top = 0
          Width = 324
          Height = 186
          Align = alClient
          Caption = 'Yield Alarm (%)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label44: TLabel
            Left = 11
            Top = 79
            Width = 142
            Height = 16
            Caption = 'Site Differ Yield% (1min)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label45: TLabel
            Left = 11
            Top = 26
            Width = 116
            Height = 16
            Caption = 'Low Yields% (1min)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label46: TLabel
            Left = 141
            Top = 49
            Width = 99
            Height = 20
            Caption = '%  after count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label47: TLabel
            Left = 141
            Top = 103
            Width = 99
            Height = 20
            Caption = '%  after count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtFailAlarmSiteYieldDifferent: TEdit
            Left = 245
            Top = 99
            Width = 76
            Height = 28
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object cbFailAlarmSiteYieldDifferent: TCheckBox
            Left = 9
            Top = 106
            Width = 80
            Height = 14
            Caption = 'Enable'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object edtFailAlarmSiteYield: TEdit
            Left = 88
            Top = 99
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
            OnClick = edtLowYieldLimitClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object cbFailAlarmLowYield: TCheckBox
            Left = 9
            Top = 52
            Width = 80
            Height = 14
            Caption = 'Enable '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object edtLowYieldLimit: TEdit
            Left = 88
            Top = 45
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
            OnClick = edtLowYieldLimitClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object edtLowYieldCount: TEdit
            Left = 245
            Top = 45
            Width = 76
            Height = 28
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
        end
      end
      object tsFailure: TTabSheet
        Caption = 'Failure'
        ImageIndex = 2
        object gbConsFailureSocket: TGroupBox
          Left = 0
          Top = 0
          Width = 324
          Height = 81
          Align = alTop
          Caption = 'Consecutive Failure(Socket)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label50: TLabel
            Left = 13
            Top = 47
            Width = 44
            Height = 16
            Caption = 'Normal'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label51: TLabel
            Left = 157
            Top = 47
            Width = 39
            Height = 16
            Caption = 'Retest'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtConseFailureCountBySocket_Retest: TEdit
            Left = 221
            Top = 43
            Width = 76
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object edtConseFailureCountBySocket_Normal: TEdit
            Left = 70
            Top = 42
            Width = 76
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object cbConseFailureBySocket_Normal: TCheckBox
            Left = 25
            Top = 20
            Width = 132
            Height = 14
            Caption = 'Enable for FT'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object cbConseFailureBySocket_Retest: TCheckBox
            Left = 173
            Top = 20
            Width = 144
            Height = 14
            Caption = 'Enable for RT'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object gbConsFailureHead: TGroupBox
          Left = 0
          Top = 81
          Width = 324
          Height = 76
          Align = alTop
          Caption = 'Consecutive Failure(Head)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label48: TLabel
            Left = 13
            Top = 47
            Width = 44
            Height = 16
            Caption = 'Normal'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label49: TLabel
            Left = 157
            Top = 47
            Width = 39
            Height = 16
            Caption = 'Retest'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtConseFailureCountByHead_Retest: TEdit
            Left = 221
            Top = 43
            Width = 76
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object edtConseFailureCountByHead_Normal: TEdit
            Left = 69
            Top = 42
            Width = 76
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '100'
            OnClick = edtLowYieldCountClick
            OnKeyPress = edtLowYieldLimitKeyPress
          end
          object cbConseFailureByHead_Normal: TCheckBox
            Left = 25
            Top = 20
            Width = 128
            Height = 14
            Caption = 'Enable For FT'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object cbConseFailureByHead_Retest: TCheckBox
            Left = 169
            Top = 20
            Width = 152
            Height = 14
            Caption = 'Enable for RT'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
      end
      object tsSmart: TTabSheet
        Caption = 'Smart'
        ImageIndex = 3
        object lbl2edAutoCleanParm: TLabel
          Left = 6
          Top = 36
          Width = 143
          Height = 16
          Caption = '2ed  Contact  Parameter'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblContactMode_Smart: TLabel
          Left = 3
          Top = 61
          Width = 83
          Height = 16
          Caption = 'Contact Mode'
        end
        object lblDropOffset1_Smart1: TLabel
          Left = 244
          Top = 61
          Width = 67
          Height = 16
          Caption = 'Drop offSet'
        end
        object lblDropOffset1_Smart2: TLabel
          Left = 302
          Top = 84
          Width = 22
          Height = 16
          Caption = 'mm'
        end
        object lblContactTime_Smart: TLabel
          Left = 3
          Top = 94
          Width = 193
          Height = 16
          Caption = 'Contact Time                              Sec'
        end
        object lblACContactCount_Smart: TLabel
          Left = 3
          Top = 126
          Width = 207
          Height = 16
          Caption = 'Contact Count                             Times'
        end
        object lblAdaptiveInterval: TLabel
          Left = 3
          Top = 156
          Width = 262
          Height = 16
          Caption = 'Adaptive Interval Max                Min               Adj'
        end
        object cbbACSmart_ContactMode: TComboBox
          Left = 87
          Top = 57
          Width = 153
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 16
          ParentFont = False
          TabOrder = 3
          Text = 'Direct Contact Mode'
          Items.Strings = (
            'Direct Contact Mode'
            'Drop Contact Mode')
        end
        object edtACSmart_DropOffset1: TEdit
          Left = 244
          Top = 82
          Width = 53
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 4
        end
        object edtACSmart_ContactTime: TEdit
          Left = 87
          Top = 90
          Width = 81
          Height = 24
          Hint = 'AutoClean_Contact Time'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Text = '0.50'
          OnMouseDown = edtACSmart_ContactTimeMouseDown
        end
        object edtACSmart_ACContactCount: TEdit
          Left = 87
          Top = 122
          Width = 81
          Height = 24
          Hint = 'AutoClean_Contact Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          Text = '3'
          OnClick = edtLowYieldLimitClick
        end
        object btnResetInterval: TButton
          Left = 216
          Top = 120
          Width = 97
          Height = 25
          Caption = 'Reset Interval'
          TabOrder = 6
          OnClick = btnResetIntervalClick
        end
        object edAdaptiveIntervalAdj: TEdit
          Left = 272
          Top = 152
          Width = 40
          Height = 24
          TabOrder = 10
          Text = '30'
          OnMouseDown = edAdaptiveIntervalAdjMouseDown
        end
        object edAdaptiveIntervalMin: TEdit
          Left = 202
          Top = 152
          Width = 40
          Height = 24
          TabOrder = 9
          Text = '30'
          OnMouseDown = edAdaptiveIntervalMinMouseDown
        end
        object edAdaptiveIntervalMax: TEdit
          Left = 136
          Top = 152
          Width = 40
          Height = 24
          TabOrder = 8
          Text = '1000'
          OnMouseDown = edAdaptiveIntervalMaxMouseDown
        end
        object chkACSmart: TCheckBox
          Left = 0
          Top = 8
          Width = 273
          Height = 17
          Caption = 'Enable, Execut clean count                       CTF'
          TabOrder = 2
        end
        object edACSmartCTF: TEdit
          Left = 280
          Top = 5
          Width = 41
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edACSmartCTFMouseDown
        end
        object edtACSmart: TEdit
          Left = 184
          Top = 5
          Width = 41
          Height = 24
          TabOrder = 0
          Text = '0'
          OnMouseDown = edtACSmartMouseDown
        end
      end
      object tsTimeCT: TTabSheet
        Caption = 'Time'
        ImageIndex = 4
        object chkTimeCT: TCheckBox
          Left = 16
          Top = 24
          Width = 305
          Height = 17
          Caption = 'Enabled time count :                                  min'
          TabOrder = 1
        end
        object edTimeCT: TEdit
          Left = 160
          Top = 20
          Width = 89
          Height = 24
          TabOrder = 0
          Text = '10'
          OnClick = edTimeCTClick
        end
      end
    end
    object pnlCleanPadAlign: TPanel
      Left = 2
      Top = 483
      Width = 332
      Height = 145
      Align = alBottom
      BevelOuter = bvNone
      Color = 14670284
      TabOrder = 1
      object tmyAutoClean: TTMyTray
        Left = -3
        Top = 43
        Width = 335
        Height = 102
        XItem = 8
      end
      object cbbSelectTray: TComboBox
        Left = 7
        Top = 9
        Width = 321
        Height = 24
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeMode = imClose
        ItemHeight = 16
        ParentFont = False
        TabOrder = 0
        Text = 'Select from Database...'
        Visible = False
        OnChange = cbbSelectTrayChange
      end
    end
    object pnlKitSetting: TPanel
      Left = 2
      Top = 18
      Width = 332
      Height = 465
      Align = alClient
      BevelOuter = bvNone
      Caption = 'pnlKitSetting'
      Color = 14670284
      TabOrder = 0
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 332
        Height = 31
        Align = alTop
        BevelOuter = bvNone
        Color = 14670284
        TabOrder = 0
        object PageTypeName: TEdit
          Left = 3
          Top = 1
          Width = 316
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'XST1'
          Visible = False
          OnKeyPress = XST1KeyPress
        end
      end
      object rgCleanKitType: TRadioGroup
        Left = 0
        Top = 31
        Width = 332
        Height = 41
        Align = alTop
        Caption = 'Mode'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Kit'
          'Tray')
        TabOrder = 1
        OnClick = rgCleanKitTypeClick
      end
      object pgCleanType: TPageControl
        Left = 0
        Top = 72
        Width = 332
        Height = 393
        ActivePage = tsKit
        Align = alClient
        MultiLine = True
        TabIndex = 0
        TabOrder = 2
        object tsKit: TTabSheet
          Caption = 'Kit'
          object Image3: TImage
            Left = 1
            Top = 46
            Width = 304
            Height = 228
            Transparent = True
          end
          object Label10: TLabel
            Left = 59
            Top = 43
            Width = 39
            Height = 16
            Caption = 'X-Start'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label11: TLabel
            Left = 125
            Top = 43
            Width = 41
            Height = 16
            Caption = 'X-Pitch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label12: TLabel
            Left = 205
            Top = 43
            Width = 60
            Height = 16
            Caption = 'X-Division'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label13: TLabel
            Left = 18
            Top = 114
            Width = 40
            Height = 16
            Caption = 'Y-Start'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label14: TLabel
            Left = 18
            Top = 166
            Width = 42
            Height = 16
            Caption = 'Y-Pitch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label15: TLabel
            Left = 14
            Top = 214
            Width = 61
            Height = 16
            Caption = 'Y-Division'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object XST1: TEdit
            Left = 50
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object XPitch1: TEdit
            Tag = 1
            Left = 116
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object XCT1: TEdit
            Left = 202
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnChange = XCT1Change
            OnClick = XCT1Click
            OnKeyPress = XCT1KeyPress
          end
          object YST1: TEdit
            Left = 12
            Top = 134
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object YPitch1: TEdit
            Left = 12
            Top = 186
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object YCT1: TEdit
            Left = 13
            Top = 234
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            OnChange = YCT1Change
            OnClick = XCT1Click
            OnKeyPress = XCT1KeyPress
          end
          object chkUseNSKit: TCheckBox
            Left = 8
            Top = 12
            Width = 97
            Height = 17
            Caption = 'Use NS KIT'
            TabOrder = 0
          end
          object chkE43: TCheckBox
            Left = 168
            Top = 12
            Width = 133
            Height = 17
            Caption = 'Use Hot Plate'
            TabOrder = 1
          end
        end
        object tabTray: TTabSheet
          Caption = 'Tray'
          ImageIndex = 1
          object Image1: TImage
            Left = 1
            Top = 46
            Width = 304
            Height = 228
            Transparent = True
          end
          object Label38: TLabel
            Left = 59
            Top = 43
            Width = 39
            Height = 16
            Caption = 'X-Start'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label39: TLabel
            Left = 125
            Top = 43
            Width = 41
            Height = 16
            Caption = 'X-Pitch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label40: TLabel
            Left = 205
            Top = 43
            Width = 60
            Height = 16
            Caption = 'X-Division'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label41: TLabel
            Left = 18
            Top = 114
            Width = 40
            Height = 16
            Caption = 'Y-Start'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label42: TLabel
            Left = 18
            Top = 166
            Width = 42
            Height = 16
            Caption = 'Y-Pitch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label43: TLabel
            Left = 14
            Top = 214
            Width = 61
            Height = 16
            Caption = 'Y-Division'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object XPitch2: TEdit
            Tag = 1
            Left = 116
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnChange = XPitch2Change
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object XST2: TEdit
            Left = 50
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object XCT2: TEdit
            Left = 202
            Top = 66
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnChange = XCT1Change
            OnClick = XCT1Click
            OnKeyPress = XCT1KeyPress
          end
          object YST2: TEdit
            Left = 12
            Top = 134
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object YPitch2: TEdit
            Left = 12
            Top = 186
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = XST1Click
            OnKeyPress = XST1KeyPress
          end
          object YCT2: TEdit
            Left = 13
            Top = 234
            Width = 61
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            OnChange = YCT1Change
            OnClick = XCT1Click
            OnKeyPress = XCT1KeyPress
          end
          object btInclude: TButton
            Left = 4
            Top = 8
            Width = 145
            Height = 25
            Caption = 'Include Tray Data'
            TabOrder = 0
            OnClick = btIncludeClick
          end
        end
      end
    end
  end
  object pnlLeft: TPanel
    Left = 0
    Top = 0
    Width = 297
    Height = 847
    Align = alLeft
    BevelOuter = bvNone
    Color = 14670284
    TabOrder = 0
    object grpCleanPara: TGroupBox
      Left = 0
      Top = 0
      Width = 297
      Height = 218
      Align = alTop
      Caption = 'Cleaning Parameter'
      TabOrder = 0
      object lblEnableFunction: TLabel
        Left = 16
        Top = 25
        Width = 50
        Height = 16
        Caption = 'Cleaning'
      end
      object rgAutoCleanOnOff: TRadioGroup
        Left = 152
        Top = 11
        Width = 127
        Height = 41
        BiDiMode = bdLeftToRight
        Columns = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          'OFF'
          'ON')
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 0
        OnClick = rgAutoCleanOnOffClick
      end
      object grpCleanMode: TGroupBox
        Left = 2
        Top = 52
        Width = 293
        Height = 164
        Align = alBottom
        Caption = 'Cleaning Mode'
        TabOrder = 1
        object lblContactCnt: TLabel
          Left = 203
          Top = 138
          Width = 58
          Height = 16
          AutoSize = False
          Caption = '/ Contact'
        end
        object edIntervalContact: TEdit
          Left = 115
          Top = 134
          Width = 81
          Height = 24
          Hint = 'AutoClean_Cleaning Mode Interval Contact Count'
          BiDiMode = bdLeftToRight
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 7
          Text = '20'
          OnClick = edtLowYieldCountClick
        end
        object chkAutoCleanMode1: TCheckBox
          Left = 16
          Top = 21
          Width = 97
          Height = 17
          Caption = 'Initial Start'
          TabOrder = 1
        end
        object chkAutoCleanMode2: TCheckBox
          Left = 16
          Top = 44
          Width = 165
          Height = 17
          Caption = 'Inital Retest Start'
          TabOrder = 2
        end
        object chkAutoCleanMode3: TCheckBox
          Left = 16
          Top = 68
          Width = 97
          Height = 17
          Caption = 'Finish'
          TabOrder = 3
        end
        object chkAutoCleanMode4: TCheckBox
          Left = 16
          Top = 91
          Width = 97
          Height = 17
          Caption = 'Manual'
          TabOrder = 5
        end
        object chkAutoCleanMode5: TCheckBox
          Left = 16
          Top = 115
          Width = 97
          Height = 17
          Caption = 'Socket Alarm'
          TabOrder = 6
        end
        object chkAutoCleanMode6: TCheckBox
          Left = 16
          Top = 138
          Width = 97
          Height = 17
          Caption = 'Interval'
          TabOrder = 8
          OnMouseUp = chkAutoCleanMode6MouseUp
        end
        object btFocusOnly: TButton
          Left = 236
          Top = 16
          Width = 1
          Height = 1
          Caption = 'Only Focus'
          TabOrder = 0
        end
        object btnStartAutoClean: TButton
          Left = 80
          Top = 89
          Width = 48
          Height = 17
          Caption = 'Clean'
          TabOrder = 4
          OnClick = btnStartAutoCleanClick
        end
      end
    end
    object grpCleanDevice: TGroupBox
      Left = 0
      Top = 218
      Width = 297
      Height = 300
      Align = alClient
      Caption = 'Cleaning Deveice'
      TabOrder = 1
      object Label279: TLabel
        Left = 18
        Top = 35
        Width = 101
        Height = 16
        BiDiMode = bdLeftToRight
        Caption = 'Number of pieces'
        ParentBiDiMode = False
      end
      object Label288: TLabel
        Left = 18
        Top = 148
        Width = 47
        Height = 16
        Caption = 'Position'
      end
      object Label289: TLabel
        Left = 18
        Top = 216
        Width = 72
        Height = 16
        Caption = 'Alarm Count'
      end
      object Label290: TLabel
        Left = 18
        Top = 243
        Width = 88
        Height = 16
        Caption = 'Cleaning Count'
      end
      object Label21: TLabel
        Left = 18
        Top = 186
        Width = 83
        Height = 16
        Caption = 'Shuttle Detect'
      end
      object sbTrayAssign: TSpeedButton
        Tag = 1
        Left = 224
        Top = 9
        Width = 66
        Height = 65
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Tray Assign'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2EA
          EAEAEAEAEAF2F2F2FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2F1F1F1EDEDEDD4D4D4C0
          C0C0C0C0C0D4D4D4EDEDEDF1F1F1F2F2F2FBFBFBFFFFFFFFFFFFFAFAFAEFEFEF
          E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
          E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E1E1E1D0D0D0CDCDCDD0D0D08685876B
          69686B6969858588D0D0D0CDCDCDD1D1D1EAEAEAFBFBFBFFFFFFEFEFEFCECECE
          BDBDBDBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
          BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCB8B8B882858A727376B2B2B2686869EB
          EAE9EBE9E968686BB2B2B2737477868584D1D1D1F2F2F2FFFFFFE9E9E9B98725
          B67E0EB47B09B47A07B47A07B47A07B47A07B47A07B47A08B47B08B47A08B47A
          07B47A07B47A07B47A08B57B08B97E0789744C999A9FA9A9A86A6969949393DE
          DCDBDEDCDB8C8B8C69696AAAA9A89E9C9C918F8EF0F0F0FFFFFFE9E9E9B67E0E
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF87888BA5A4A4DDDAD9D3D1CFDAD7D6D8
          D5D4D8D5D5DAD7D6D3D1CFDDDADAAAA9A77F7D7BEBEBEBFBFBFBE9E9E9B47B09
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7C0A6FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFDAC3A8FFFFFFE3E3E26D6A68CDCBCAD2D0D0B4B2B086
          85847F7D7DB4B2B0D2D0D0CECCCB73706EABAAAAD3D3D3F2F2F2E9E9E9B47B08
          FFFFFFD5BCA0D6BCA0D6BDA0D6BDA0D6BDA0D6BDA1D7BFA3D9C1A7D7BFA3D6BD
          A1D6BDA0D6BDA1D8C0A4DFC7AC9A8F85706D6C8E8B89CFCDCBB2B0AF757370C1
          AE9AC1AD9974726EB1AFADCFCDCB918E8C7A7674949290EAEAEAE9E9E9B47A08
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7BEA3FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFE1C8AC707171D4D2D2CDCBCAC9C5C5888786C3AF9BFF
          FFFFFFFFFFE0DEDB878583C8C5C4CDCCCBD7D5D57D7B79EAEAEAE9E9E9B47A07
          FFFFFFFFFEFBFFFDF8FFFDF8FFFDF8FFFDF8FFFEF9FFFFFFD6BC9EFFFFFFFFFE
          F9FFFDF8FFFEF9FFFFFFE0C6A8737373EBE9E9E8E6E5C4C2C08B8887C3B09AFF
          FFFFFFFFFFE0DFD984817FC4C2C0E9E7E6EEECEB807E7CF2F2F2E9E9E9B47A07
          FFFFFFFFFDF7FFFCF5FFFCF5FFFCF5FFFCF5FFFDF6FFFFFBD6BB9CFFFFFBFFFD
          F6FFFCF5FFFDF6FFFFFCDDC2A3AFAEAC7774739D9A9AD1CFCFA8A5A47F7B79E2
          DFDAE2DED9807E7BA8A4A2D1CFCFA09D9C817E7CA7A5A3FBFBFBE9E9E9B47A08
          FFFFFFFFFFF9FFFFF8FFFFF8FFFFF8FFFFF8FFFFF9FFFFFDD6BD9FFFFFFDFFFF
          F9FFFFF8FFFFF9FFFFFED9BFA1FFFFFFFFFEF57C7978B8B5B3CECCCAA6A3A386
          84838A8785A6A3A2CECCCAB9B6B4827F7DCDCDCDF1F1F1FFFFFFE9E9E9B47B08
          FFFFFFD4B998D4BA98D5BA99D5BA99D5BA99D5BA9AD7BD9DD8BFA0D7BD9DD5BA
          9AD5BA99D5BA9AD7BD9DD9C0A0DDC2A18E8982B3B2B1E6E5E4E4E3E3EEECEBBA
          B9B7BAB8B7EEECEBE4E3E3E6E6E5B9B8B6928F8DF2F2F2FFFFFFE9E9E9B47A08
          FFFFFFFFFEF3FFFEF3FFFEF3FFFEF3FFFEF3FFFEF4FFFFF8D6BC9BFFFFF8FFFE
          F4FFFEF3FFFEF4FFFFF8D7BC9BFFFFFCB2AFAAAAA9A8B8B7B5807E7DA4A1A1D7
          D5D3D7D5D3A4A1A0807E7DBAB9B8B2B1B2A9A7A7FBFBFBFFFFFFE9E9E9B47A08
          FFFFFFFFFAEDFEF9ECFEF9ECFEF9ECFEF9ECFFFAEDFFFDF2D5B897FFFDF2FFFA
          EDFEF9ECFFFAEDFFFDF2D6B997FFFFF4FFFFF4B3AEA99A9693FFFFF4838180F1
          F0F0F0F0F083807FFFFFF49E9EA19D865AE9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFFF9EAFEF8E9FEF8E9FEF8E9FEF8E9FFF9EAFFFCEFD5B995FFFCEFFFF9
          EAFEF8E9FFF9EAFFFCEFD5B995FFFDEFFFFBECFFFDEEFFFFF0FFFFF3A79A8D86
          8485858383B5B2ACFFFFF2FFFFFFB87C03E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFFFBEBFFFAEBFFFBECFFFBECFFFBECFFFBEDFFFEF1D6BA96FFFEF1FFFB
          EDFFFBECFFFBEDFFFEF1D6BA96FFFEF1FFFBEDFFFBEDFFFCEEFFFFF2DCBE99FF
          FFF7FFFFF4FFFFF1FFFDEEFFFFFFB57B07E9E9E9FFFFFFFFFFFFE9E9E9B47B08
          FFFFFFD4B48FD4B691D5B692D5B692D5B692D5B692D7B995D8BB98D7B995D5B6
          92D5B692D5B692D7B995D8BB98D7B995D5B692D5B692D5B692D7B995D9BB99D8
          BA96D6B893D6B692D5B590FFFFFFB47B08E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFFF9E6FFFAE6FFFAE7FFFAE7FFFAE7FFFAE8FFFDECD6B993FFFDECFFFA
          E8FFFAE7FFFAE8FFFDECD6B993FFFDECFFFAE8FFFAE7FFFAE8FFFDECD6B993FF
          FDECFFFAE8FFFAE6FFF9E6FFFFFFB47A08E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFFF6E0FEF5E0FFF5E0FFF5E0FFF5E0FFF6E1FFF9E6D5B590FFF9E6FFF6
          E1FFF5E0FFF6E1FFF9E6D5B590FFF9E6FFF6E1FFF5E0FFF6E1FFF9E6D5B590FF
          F9E6FFF6E1FEF5E0FFF6E0FFFFFFB47A08E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFFF5DEFFF3DEFFF4DEFFF4DEFFF4DEFFF5DFFFF8E3D5B68EFFF8E3FFF5
          DFFFF4DEFFF5DFFFF8E3D5B68EFFF8E3FFF5DFFFF4DEFFF5DFFFF8E3D5B68EFF
          F8E3FFF5DFFFF3DEFFF5DEFFFFFFB47A08E9E9E9FFFFFFFFFFFFE9E9E9B47B08
          FFFFFFFFF6DEFFF6DFFFF7E0FFF7E0FFF7E0FFF7E0FFFAE4D5B78EFFFAE4FFF7
          E0FFF7E0FFF7E0FFFAE4D5B78EFFFAE4FFF7E0FFF7E0FFF7E0FFFAE4D5B78EFF
          FAE4FFF7E0FFF6DFFFF6DEFFFFFFB47B08E9E9E9FFFFFFFFFFFFE9E9E9B47B09
          FFFFFFD3B187D3B38AD4B38AD4B38AD4B38AD4B48BD6B68ED7B890D6B68ED4B4
          8BD4B38AD4B48BD6B68ED7B890D6B68ED4B48BD4B38AD4B48BD6B68ED7B890D6
          B68ED4B48BD3B38AD3B187FFFFFFB47B09E9E9E9FFFFFFFFFFFFE9E9E9B47B08
          FFFFFFFFF3D9FFF4DBFFF5DCFFF5DCFFF5DCFFF5DDFFF8E0D5B58CFFF8E0FFF5
          DDFFF5DCFFF5DDFFF8E0D5B58CFFF8E0FFF5DDFFF5DCFFF5DDFFF8E0D5B58CFF
          F8E0FFF5DDFFF4DBFFF3D9FFFFFFB47B08E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFEF1D3FDF0D3FDF1D4FDF1D4FDF1D4FEF2D5FFF5D9D4B387FFF5D9FEF2
          D5FDF1D4FEF2D5FFF5D9D4B387FFF5D9FEF2D5FDF1D4FEF2D5FFF5D9D4B387FF
          F5D9FEF2D5FDF0D3FEF1D3FFFFFFB47A08E9E9E9FFFFFFFFFFFFE9E9E9B47A08
          FFFFFFFEF0D1FDEFD1FDF0D2FDF0D2FDF0D2FEF1D3FFF4D7D4B286FFF4D7FEF1
          D3FDF0D2FEF1D3FFF4D7D4B286FFF4D7FEF1D3FDF0D2FEF1D3FFF4D7D4B286FF
          F4D7FEF1D3FDEFD1FEF0D1FFFFFFB47A08E9E9E9FFFFFFFFFFFFE9E9E9B47B09
          FFFFFFFFF2D1FFF2D2FFF3D4FFF3D4FFF3D4FFF3D4FFF6D8D7B486FFF6D8FFF3
          D4FFF3D4FFF3D4FFF6D8D7B486FFF6D8FFF3D4FFF3D4FFF3D4FFF6D8D7B486FF
          F6D8FFF3D4FFF2D2FFF2D1FFFFFFB47B09E9E9E9FFFFFFFFFFFFE9E9E9B57B09
          FFFFFFDEB27CE3B47EE4B57FE4B57FE4B57FE3B680E1B783E0B887E1B783E3B6
          80E4B57FE3B680E1B783E0B887E1B783E3B680E4B57FE3B680E1B783E0B887E1
          B783E3B680E3B47EDEB27CFFFFFFB57B09E9E9E9FFFFFFFFFFFFE9E9E9B67B08
          FFFFFF4DC5FF56C8FF58C9FF58C9FF58C9FF56C9FF51CAFFEBB97F51CAFF56C9
          FF58C9FF56C9FF51CAFFEBB97F51CAFF56C9FF58C9FF56C9FF51CAFFEBB97F51
          CAFF56C9FF56C8FF4DC5FFFFFFFFB67B08E9E9E9FFFFFFFFFFFFEAEAEAB67C09
          FFFFFF4CC3FF54C5FF56C6FF56C6FF56C6FF55C6FF4FC6FFE9B5794FC6FF55C6
          FF56C6FF55C6FF4FC6FFE9B5794FC6FF55C6FF56C6FF55C6FF4FC6FFE9B5794F
          C6FF55C6FF54C5FF4CC3FFFFFFFFB67C09EAEAEAFFFFFFFFFFFFF2F2F2B67E0E
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFB67E0EF2F2F2FFFFFFFFFFFFFBFBFBCBA65C
          B67E0EB67C09B67B07B67B07B67B07B67B07B67B07B67B08B57B08B67B08B67B
          07B67B07B67B07B67B08B57B08B67B08B67B07B67B07B67B07B67B08B57B08B6
          7B08B67B07B67B07B67C09B67E0ECBA65CFBFBFBFFFFFFFFFFFF}
        Layout = blGlyphTop
        Margin = 4
        ParentFont = False
        OnClick = sbTrayAssignClick
      end
      object labUnit_Thickness: TLabel
        Left = 260
        Top = 100
        Width = 22
        Height = 16
        Caption = 'mm'
      end
      object ImgCleanUnit: TImage
        Left = 2
        Top = 87
        Width = 151
        Height = 36
        Picture.Data = {
          0A544A504547496D616765300D0000FFD8FFE000104A46494600010101006000
          600000FFDB004300020101020101020202020202020203050303030303060404
          0305070607070706070708090B0908080A0807070A0D0A0A0B0C0C0C0C07090E
          0F0D0C0E0B0C0C0CFFDB004301020202030303060303060C0807080C0C0C0C0C
          0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C
          0C0C0C0C0C0C0C0C0C0C0C0C0CFFC0001108002800B403012200021101031101
          FFC4001F0000010501010101010100000000000000000102030405060708090A
          0BFFC400B5100002010303020403050504040000017D01020300041105122131
          410613516107227114328191A1082342B1C11552D1F02433627282090A161718
          191A25262728292A3435363738393A434445464748494A535455565758595A63
          6465666768696A737475767778797A838485868788898A92939495969798999A
          A2A3A4A5A6A7A8A9AAB2B3B4B5B6B7B8B9BAC2C3C4C5C6C7C8C9CAD2D3D4D5D6
          D7D8D9DAE1E2E3E4E5E6E7E8E9EAF1F2F3F4F5F6F7F8F9FAFFC4001F01000301
          01010101010101010000000000000102030405060708090A0BFFC400B5110002
          0102040403040705040400010277000102031104052131061241510761711322
          328108144291A1B1C109233352F0156272D10A162434E125F11718191A262728
          292A35363738393A434445464748494A535455565758595A636465666768696A
          737475767778797A82838485868788898A92939495969798999AA2A3A4A5A6A7
          A8A9AAB2B3B4B5B6B7B8B9BAC2C3C4C5C6C7C8C9CAD2D3D4D5D6D7D8D9DAE2E3
          E4E5E6E7E8E9EAF2F3F4F5F6F7F8F9FAFFDA000C03010002110311003F00FBDA
          CF4AB1FB0DBFFC4BB4BFF529FF002E517F747FB3527F6558FF00D03B4BFF00C0
          28BFF89A759FFC78DB7FD714FF00D0453750D42DB47D3AE6FAF6E21B4B1B185E
          E2E6691C22431A29667663C050A18927B29A003FB2AC7FE81DA5FF00E0145FFC
          4D1FD9563FF40ED2FF00F00A2FFE26BE3493FE0E0BFD95617646F1C6BECC8704
          A7866EB613DF1EDE9ED4DFF8883FF654FF00A1DBC45FF84CDD5007D9BFD9563F
          F40ED2FF00F00A2FFE268FECAB1FFA07697FF80517FF00135F197FC441FF00B2
          A7FD0EDE22FF00C266EA8FF8883FF654FF00A1DBC45FF84CDD5007D9BFD9563F
          F40ED2FF00F00A2FFE268FECAB1FFA07697FF80517FF00135F197FC441FF00B2
          A7FD0EDE22FF00C266EA8FF8883FF654FF00A1DBC45FF84CDD5007D9BFD9563F
          F40ED2FF00F00A2FFE268FECAB1FFA07697FF80517FF00135F2EFC36FF0082CE
          7C08F8D777790782AE7E2278BA6B08D66BA4D23C1B7974D668582A970A323736
          40278E3DEBAB7FF828F78221243F843E3612091B47C3CD41597EB91D7391C7A5
          007BBFF6558FFD03B4BFFC028BFF0089A3FB2AC7FE81DA5FFE0145FF00C4D783
          FF00C3C93C0DFF004277C6EFFC37F7FF00E147FC3C93C0DFF4277C6EFF00C37F
          7FFE1401EF1FD9563FF40ED2FF00F00A2FFE268FECAB1FFA07697FF80517FF00
          135E0FFF000F24F037FD09DF1BBFF0DFDFFF00851FF0F24F037FD09DF1BBFF00
          0DFDFF00F85007BC7F6558FF00D03B4BFF00C028BFF89A3FB2AC7FE81DA5FF00
          E0145FFC4D783FFC3C93C0DFF4277C6EFF00C37F7FFE147FC3C93C0DFF004277
          C6EFFC37F7FF00E1401EF1FD9563FF0040ED2FFF0000A2FF00E268FECAB1FF00
          A07697FF0080517FF135E0FF00F0F24F037FD09DF1BBFF000DFDFF00F851FF00
          0F24F037FD09DF1BBFF0DFDFFF0085007BC7F6558FFD03B4BFFC028BFF0089A3
          FB2AC7FE81DA5FFE0145FF00C4D783FF00C3C93C0DFF004277C6EFFC37F7FF00
          E147FC3C93C0DFF4277C6EFF00C37F7FFE1401EF1FD9563FF40ED2FF00F00A2F
          FE26B88F8D32B68D7FF0EC595BD95B2DFF008D2D6CEE522B38556EE136978CD1
          B1DBC82CB19DBDCA035E7DFF000F24F037FD09DF1BBFF0DFDFFF00857CBBFF00
          0506FDA67C69F133E20F81F51F849A7FC5DB5D3EFDA3D3F5F8AFFC037C8BE1F5
          5172916AF6C08F9E78E3BDB9057A36C8BFBB401F7F78CACECE0D46245D1F4294
          AC582D71A7C5D77B70985FBA071F5068AC9F03FC43D1B5DF879E1DBB8478B2D6
          D24D3E386DA3D534C6B5BE11C24C00CF14A37C72318CB107AE770E1A8A00E94F
          8BB47D32286DAEF58D1ECEF1208D8DBDC5FC30CAA0A295255981190723DABF39
          3FE0E1FF00F8282C1F0C3E06DAFC1BF076B36B77E20F88108B9D6EE2C2E52616
          7A487C2C61D0901AE644C119C88D1811F357C13FF05FC98C1FF0542F1902E0A1
          D27462CB131D8C7FB32DFA7E7F9E6BE305B924812177EF9CB654FAFD7FC68020
          B9BA2F712304450CC4E06703DA99E79F45A5B852F3B9457284F191CE3B533CB6
          FEEB7E5400EF3CFA2D1E79F45A6F96DFDD6FCA8F2DBFBADF95003BCF3E8B47DA
          0FF757F5A6F96DFDD6FCA8F2DBFBADF95007BFFF00C138FF006E4D73F613FDAA
          BC39E36D39EE1F488E6FB2EBBA7C2E55750D3A4C0B88719E4ED01D33D1D171DE
          BFA67F0E7C71F0DF8B3C37A76ABA778D344BBD3754B68EF2CE56D5A08DA58A45
          0EAE559F20907A1AFE476D142CEA5C305EE7156AFEED6E046144D9405472480A
          3A0FC05007F5EB69E207D42D927B6BF3730483292C370248DC7A8652411F4352
          7F6A5CFF00CFCDC7FDF66BE64FF823982FFF0004BFF82833C9D09FA9FF00A7CB
          8AED3F6A5FDBDFE137EC5575A1C1F137C52FE1B97C491CD2E9CA2C66B9370B11
          5590FEED4EDC175EBD73401ECDFDAF3AFCCF772A22FDF2642360FEF7D300F3ED
          5F971F1E3FE0E83D2FE187C5BF12683E1AF8712F8B741D12FE4B3B4D64EBAF6C
          BA92A36C32AA08DB0A595B6F3CA807BD33FE0A93FF0005D5F865AF7EC9BAC786
          7E09F8BAEF59F1778C73A45D5D45613DAB69560E337122B480664751E52EDE47
          98C7B0AFC52910488CAA8E1DF2A81093BFDB1EC38FC6803F5FBFE22C89BFE88C
          1FFC29E4FF00E3547FC459137FD1183FF853C9FF00C6ABF1CFCA6CE3047F4A77
          D925EE8C0E377231C7AFD2803F62BFE22C89BFE88C1FFC29E4FF00E3547FC459
          137FD1183FF853C9FF00C6ABF1D7ECB273F29E28FB24BF31D8D853B49C700FA7
          D6803F62BFE22C89BFE88C1FFC29E4FF00E355D87C01FF008397758FDA3BE337
          877C0BA2FC26D1F4DD63C5178B61653EA9E307B6B4F3D81D88F21870BBDB0833
          FC4CA3BD7E22476D24AC15119989C000649F6AB3A34F2E9B7D15DC6F3432407C
          C8A489B6BA3AF2ACA47421B073DB1401FD42FF00C2E7FDA7C3303F063E1E2953
          8F9BE22B0CFF00E40F5E3F0AF22FDA87FE0A99F15BF63AD6BC1767E3BF85DE04
          B3B9F1B5FF00D82C56D7C7B24DF674DCAAF7571883F756C8CC0339E067EB8E0F
          F634FF0082FF00FC1BF13FECD3E1697E2DF8BDBC3FF112CADFEC3AD45FD953DC
          2DE4B16145DAB46A5409576B11D436FED8AF4FBFF89DF03BFE0A143C39E2CF07
          4F6DE31B2D43C5367E05D72E6E6D26803D9BD8EA53B5A049146158CC59997A9D
          9DD45007BFDBF883C557DE1FD26E3C53A4E95A2788AE2D8BDF58595F9D4A0B56
          F3640AAB71B47983CB08738E338E719A2B4B55D166D12D34CD3F4ABC86C34FD3
          6C62B4823BC2F24BB231B1492339F94019EA704F7A2803F3F3FE0A0FFF000427
          BAFDBBFF006A0D5FE265A7C5DF0A7862DB5AB1B0812C27B4F3DE0305A450B02C
          B201C94C8FAD7897FC42CDA9FF00D17FF04FFE0ADFFF008F5145001FF10B36A7
          FF0045FF00C13FF82B7FFE3D47FC42CDA9FF00D17FF04FFE0ADFFF008F514500
          1FF10B36A7FF0045FF00C13FF82B7FFE3D47FC42CDA9FF00D17FF04FFE0ADFFF
          008F5145001FF10B36A7FF0045FF00C13FF82B7FFE3D47FC42CDA9FF00D17FF0
          4FFE0ADFFF008F5145001FF10B36A7FF0045FF00C13FF82B7FFE3D47FC42CDA9
          FF00D17FF04FFE0ADFFF008F5145007EA27EC61F032DFF00651FD957C0FF000D
          2EBC4BA2EBD378434F6B192FA19E3823BBCCD249B823392BC498C13DABE72FF8
          2B9FFC130B53FF00829578DBE1A5C695F103C2BE12D33C276DAA25EDC5EB25D3
          8699ADDE2DA8AE3219A39176FF0006DC9CEE1451401F17FF00C430BE347B48E5
          FF0085BDE0486E1B497BB9202BBBC9D4176EDB3DDE66181C9FDF8F978E9CD4D7
          DFF06C478BECAC6E4DAFC62F01DC4F059C371046E9E5ADDDD12DE75BEFF33E41
          18DA7CCE8FBF000DA68A28024BEFF835EFC510EA37620F8D1E069E286EEDA2B6
          91A1D9E7DBB6CF3E66064F95A3CCB84392FE571F7861D17FC1B09E286B911CBF
          1A3C0A9036A2D6CCCB01764B258CE2F08F33258BE17C91C8DD9CE0628A28021D
          2BFE0D7EF16CD77A72DF7C67F0259C13CD72B7CEB179C6CE38D88B7700483CDF
          3400768C14CE0E71496DFF0006C478D26B485A5F8C3E0486E5B4C92E24830098
          2F97684B3DDE661C31661E7741B3A73451400973FF0006C178C6CAC6EE4B4F8C
          5E04B99E2B08A7B68987962E2E98B87B72DE67CA11421DFD1B7E31C54BAAFF00
          C1B01E298DEF16D3E34F816ED60BAB68AD6536FE50BB818A2CF2E0C9F218416F
          90E4C98E08CD145002DBFF00C1B05E2A8A57DDF1A3C0CAABA99B65716FB82D88
          4C8BBC799D7CCC2793F7BE6DD9C0AFB83FE0971FF04D71FB0CFC0EBAF0C78ABC
          67A0EBD7E9E383E2CB2934FB98E28C7936EF67123166390F13098A8E572A99C8
          268A2803EA3F17EA9646FA0C6A3A61FDC8FF0097C8BFBCDFED51451401FFD9}
        Stretch = True
      end
      object lbl2: TLabel
        Left = 9
        Top = 65
        Width = 120
        Height = 16
        Hint = '?'#28500'IC'#30340#21402#24230
        Caption = 'Clean Pad Deviation '
      end
      object lbledIndexArmAutoCleanCnt: TLabel
        Left = 18
        Top = 270
        Width = 74
        Height = 16
        Caption = 'Total contact'
      end
      object btnResetCleanCount: TButton
        Left = 212
        Top = 239
        Width = 69
        Height = 25
        Caption = 'Reset'
        TabOrder = 8
        OnClick = btnResetCleanCountClick
      end
      object rgKitPosition: TRadioGroup
        Left = 84
        Top = 125
        Width = 197
        Height = 50
        Columns = 2
        Enabled = False
        ItemIndex = 2
        Items.Strings = (
          'Fix3'
          'Hot Plate 2'
          'Clean Kit'
          'Clean Air')
        TabOrder = 4
        OnClick = rgKitPositionClick
      end
      object edDevicePices: TEdit
        Left = 132
        Top = 32
        Width = 69
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 0
        Text = '0'
        OnClick = edDevicePicesClick
      end
      object udDeviceCT: TUpDown
        Left = 201
        Top = 32
        Width = 16
        Height = 24
        Associate = edDevicePices
        Min = 0
        Position = 0
        TabOrder = 2
        Wrap = False
        OnChangingEx = udDeviceCTChangingEx
        OnClick = udDeviceCTClick
      end
      object edAlarmCount: TEdit
        Left = 132
        Top = 212
        Width = 69
        Height = 24
        Hint = 'AutoClean_Alarm Count'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 6
        Text = '0'
        OnClick = edtLowYieldCountClick
      end
      object rgShuttleCheck: TRadioGroup
        Left = 107
        Top = 176
        Width = 174
        Height = 36
        Columns = 2
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          'OFF'
          'ON')
        ParentFont = False
        TabOrder = 5
      end
      object edCleaningCount: TEdit
        Left = 132
        Top = 239
        Width = 69
        Height = 24
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 7
        Text = '0'
        OnClick = edCleanCountClick
      end
      object edCleanPadDeviation: TEdit
        Left = 166
        Top = 95
        Width = 81
        Height = 24
        Hint = 'AutoClean_Cleaning Pad Deviation'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = 'Chinese (Traditional) - Phonetic'
        ParentFont = False
        TabOrder = 3
        Text = '0'
        OnClick = edDropOffset1Click
      end
      object cbbCleanPadCount: TComboBox
        Left = 132
        Top = 32
        Width = 85
        Height = 24
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 1
        Text = '12'
        OnChange = cbbCleanPadCountChange
        Items.Strings = (
          '12'
          '16'
          '24'
          '32')
      end
      object edIndexArmAutoCleanCnt: TEdit
        Left = 132
        Top = 266
        Width = 69
        Height = 24
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 9
        Text = '0'
        OnClick = edCleanCountClick
      end
    end
    object grpSpeed: TGroupBox
      Left = 0
      Top = 518
      Width = 297
      Height = 171
      Align = alBottom
      Caption = 'Speed'
      TabOrder = 2
      object lblInArm: TLabel
        Left = 8
        Top = 62
        Width = 66
        Height = 16
        Caption = 'Output Arm'
      end
      object lblShuttle: TLabel
        Left = 8
        Top = 89
        Width = 41
        Height = 16
        Caption = 'Shuttle'
      end
      object lblIndex: TLabel
        Left = 8
        Top = 115
        Width = 58
        Height = 16
        Caption = 'Index Arm'
      end
      object lblInArmZ: TLabel
        Left = 8
        Top = 142
        Width = 48
        Height = 16
        Caption = 'In Arm Z'
      end
      object lblSpeed: TLabel
        Left = 80
        Top = 38
        Width = 61
        Height = 16
        Caption = 'Speed (%)'
      end
      object lblVacuum: TLabel
        Left = 162
        Top = 38
        Width = 47
        Height = 16
        Caption = 'Vacuum'
      end
      object lblAirOn: TLabel
        Left = 242
        Top = 38
        Width = 37
        Height = 16
        Caption = 'Air On'
      end
      object lblDelayTime: TLabel
        Left = 176
        Top = 18
        Width = 100
        Height = 16
        Caption = 'Delay Time (Sec)'
      end
      object OutArmSpeed: TEdit
        Left = 80
        Top = 58
        Width = 60
        Height = 24
        Hint = 'AutoClean_OutArm Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '10'
        OnClick = edtLowYieldLimitClick
      end
      object ShuttleSpeed: TEdit
        Left = 80
        Top = 85
        Width = 60
        Height = 24
        Hint = 'AutoClean_Shuttle Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '10'
        OnClick = edtLowYieldLimitClick
      end
      object IndexArmSpeed: TEdit
        Left = 80
        Top = 111
        Width = 60
        Height = 24
        Hint = 'AutoClean_Index Arm Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = '10'
        OnClick = edtLowYieldLimitClick
      end
      object edtInArmZSpeed: TEdit
        Left = 80
        Top = 138
        Width = 60
        Height = 24
        Hint = 'AutoClean_Rotation Stage Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '10'
        OnClick = edtLowYieldLimitClick
      end
      object edtIndexVacuum: TEdit
        Left = 155
        Top = 111
        Width = 60
        Height = 24
        Hint = 'AutoClean_Index Arm Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        Text = '10'
        OnClick = edtIndexVacuumClick
      end
      object edtInArmVacuum: TEdit
        Left = 156
        Top = 138
        Width = 60
        Height = 24
        Hint = 'AutoClean_Rotation Stage Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '10'
        OnClick = edtIndexVacuumClick
      end
      object edtIndexAirOn: TEdit
        Left = 230
        Top = 111
        Width = 60
        Height = 24
        Hint = 'AutoClean_Index Arm Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '10'
        OnClick = edtIndexVacuumClick
      end
      object edtInArmAirOn: TEdit
        Left = 230
        Top = 138
        Width = 60
        Height = 24
        Hint = 'AutoClean_Rotation Stage Speed'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '10'
        OnClick = edtIndexVacuumClick
      end
    end
    object gbCleanKitOffset: TGroupBox
      Left = 0
      Top = 689
      Width = 297
      Height = 158
      Align = alBottom
      Caption = 'Auto Clean Kit Offset (mm)'
      TabOrder = 3
      Visible = False
      object lblInArmKitXOffset: TLabel
        Left = 14
        Top = 20
        Width = 106
        Height = 16
        Caption = 'In Arm Kit X Offset'
      end
      object lblInArmKitYOffset: TLabel
        Left = 14
        Top = 48
        Width = 108
        Height = 16
        Caption = 'In Arm Kit Y Offset'
      end
      object lblInArmKitPickOffset: TLabel
        Left = 14
        Top = 77
        Width = 125
        Height = 16
        Caption = 'In Arm Kit Pick Offset'
      end
      object lblInArmKitPlaceOffset: TLabel
        Left = 14
        Top = 105
        Width = 132
        Height = 16
        Caption = 'In Arm Kit Place Offset'
      end
      object lblInArmKitXPitchOffset: TLabel
        Left = 14
        Top = 133
        Width = 129
        Height = 16
        Caption = 'In Arm Kit Pitch Offset'
      end
      object edtHotplateXOffset: TEdit
        Left = 180
        Top = 16
        Width = 81
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = '0.50'
        OnClick = edtBufferKitLTXClick
      end
      object edtHotplateYOffset: TEdit
        Left = 180
        Top = 44
        Width = 81
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Text = '0.50'
        OnClick = edtBufferKitLTXClick
      end
      object edtHotplatePlaceOffset: TEdit
        Left = 180
        Top = 101
        Width = 81
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = '0.50'
        OnClick = edtHotplatePickOffsetClick
      end
      object edtHotplatePickOffset: TEdit
        Left = 180
        Top = 73
        Width = 81
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        Text = '0.50'
        OnClick = edtHotplatePickOffsetClick
      end
      object edtHotplatePitchOffset: TEdit
        Left = 180
        Top = 129
        Width = 81
        Height = 24
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeMode = imClose
        ParentFont = False
        TabOrder = 4
        Text = '0.50'
        OnClick = edtBufferKitLTXClick
      end
    end
  end
  object pnlCenter: TPanel
    Left = 297
    Top = 0
    Width = 363
    Height = 847
    Align = alClient
    BevelOuter = bvNone
    Color = 14670284
    TabOrder = 1
    object grpContactPara: TGroupBox
      Left = 0
      Top = 0
      Width = 363
      Height = 801
      Align = alClient
      Caption = 'Contact Parameter'
      Color = 14670284
      ParentColor = False
      TabOrder = 0
      object grpContactForce: TGroupBox
        Left = 2
        Top = 121
        Width = 359
        Height = 149
        Align = alTop
        Caption = 'Contact Force'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object lblArmTotalForceKg: TLabel
          Left = 16
          Top = 26
          Width = 113
          Height = 16
          Caption = 'Arm total force (Kg)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object lblForcePerPinN: TLabel
          Left = 16
          Top = 100
          Width = 97
          Height = 16
          BiDiMode = bdLeftToRight
          Caption = 'Force per pin (N)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
        end
        object lblForcePerPinG: TLabel
          Left = 95
          Top = 125
          Width = 18
          Height = 16
          Caption = '(gf)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object lblArmTotalForceN: TLabel
          Left = 112
          Top = 50
          Width = 17
          Height = 16
          Caption = '(N)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object lblPinCount: TLabel
          Left = 16
          Top = 72
          Width = 99
          Height = 16
          Caption = 'Pins(balls) Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object edPinSingleN: TEdit
          Left = 133
          Top = 96
          Width = 81
          Height = 24
          Hint = 'Click to input preasure of auto height '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 4
          Text = '0'
          OnClick = edPinSingleNClick
        end
        object edPinSingleGf: TEdit
          Left = 133
          Top = 121
          Width = 81
          Height = 24
          Hint = 'Click to input preasure of auto height '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 5
          Text = '0'
          OnClick = edPinSingleGfClick
        end
        object edPinsCount: TEdit
          Left = 133
          Top = 71
          Width = 81
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 3
          Text = '0'
          OnClick = edPinsCountClick
        end
        object edAutoCleanAirForce_Kg: TEdit
          Left = 133
          Top = 23
          Width = 81
          Height = 24
          Color = clBtnFace
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          Text = '0'
          OnClick = edAutoCleanAirForce_KgClick
        end
        object edAutoCleanAirForce_N: TEdit
          Left = 133
          Top = 47
          Width = 81
          Height = 24
          Color = clBtnFace
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
          Text = '0'
          OnClick = edAutoCleanAirForce_NClick
        end
        object grpDieForce: TGroupBox
          Left = 220
          Top = 0
          Width = 133
          Height = 149
          Caption = 'Die Force'
          TabOrder = 0
          object edtAutoCleanDieForce: TEdit
            Left = 8
            Top = 23
            Width = 81
            Height = 24
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Text = '0'
            OnClick = edAutoCleanAirForce_KgClick
          end
        end
      end
      object grpHeight: TGroupBox
        Left = 2
        Top = 270
        Width = 359
        Height = 447
        Align = alClient
        Caption = 'Height (mm)'
        TabOrder = 2
        object lblIndexToSocketShiftHigh: TLabel
          Left = 8
          Top = 20
          Width = 116
          Height = 16
          Caption = 'Contact Shift Height'
        end
        object lblIndexToSocketOffset: TLabel
          Left = 8
          Top = 45
          Width = 130
          Height = 16
          Caption = 'Socket Position Offset'
        end
        object lblIndexToShtPickOffset: TLabel
          Left = 8
          Top = 69
          Width = 159
          Height = 16
          Caption = 'Index to Shuttle Pick Offset'
        end
        object lblIndexToShtPlaceOffset: TLabel
          Left = 8
          Top = 94
          Width = 180
          Height = 16
          Caption = 'Index to Shuttle Release Offset'
        end
        object edACContactShiftHeight: TEdit
          Left = 204
          Top = 16
          Width = 60
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '0.50'
          OnClick = edACContactShiftHeightClick
        end
        object edACContactCleanHeight: TEdit
          Left = 204
          Top = 41
          Width = 60
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '0.50'
          OnClick = edACContactCleanHeightClick
        end
        object pnlNotify: TPanel
          Left = 8
          Top = 108
          Width = 345
          Height = 46
          BevelOuter = bvNone
          Caption = 'Use 4 offset for each row and col'
          Color = 14670284
          TabOrder = 4
          Visible = False
        end
        object edIndexPickOffset: TEdit
          Left = 204
          Top = 65
          Width = 60
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '0.50'
          OnClick = edtHotplatePickOffsetClick
        end
        object pnlInArmOffset: TPanel
          Left = 2
          Top = 153
          Width = 355
          Height = 292
          Align = alBottom
          BevelOuter = bvNone
          Color = 14670284
          TabOrder = 5
          Visible = False
          object lblInArmToSht1PickOffset: TLabel
            Left = 6
            Top = 6
            Width = 172
            Height = 16
            Caption = 'In Arm to Shuttle1 Pick Offset'
          end
          object lblInArmToSht1PlaceOffset: TLabel
            Left = 6
            Top = 33
            Width = 179
            Height = 16
            Caption = 'In Arm to Shuttle1 Place Offset'
          end
          object lblInArmToSht1XOffset: TLabel
            Left = 6
            Top = 61
            Width = 153
            Height = 16
            Caption = 'In Arm to Shuttle1 X Offset'
          end
          object lblInArmToSht1YOffset: TLabel
            Left = 6
            Top = 88
            Width = 155
            Height = 16
            Caption = 'In Arm to Shuttle1 Y Offset'
          end
          object lblInArmToSht2PickOffset: TLabel
            Left = 6
            Top = 116
            Width = 172
            Height = 16
            Caption = 'In Arm to Shuttle2 Pick Offset'
          end
          object lblInArmToSht2PlaceOffset: TLabel
            Left = 6
            Top = 143
            Width = 179
            Height = 16
            Caption = 'In Arm to Shuttle2 Place Offset'
          end
          object lblInArmToSht2XOffset: TLabel
            Left = 6
            Top = 170
            Width = 153
            Height = 16
            Caption = 'In Arm to Shuttle2 X Offset'
          end
          object lblInArmToSht2YOffset: TLabel
            Left = 6
            Top = 198
            Width = 155
            Height = 16
            Caption = 'In Arm to Shuttle2 Y Offset'
          end
          object lblInArmToShtPitchOffset: TLabel
            Left = 6
            Top = 225
            Width = 154
            Height = 16
            Caption = 'In Arm Shuttle Pitch Offset'
          end
          object edShuttle1PickOffset: TEdit
            Left = 202
            Top = 2
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0.50'
            OnClick = edtHotplatePickOffsetClick
          end
          object edtShuttle1PlaceOffset: TEdit
            Left = 202
            Top = 29
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0.50'
            OnClick = edtHotplatePickOffsetClick
          end
          object edtShuttle1XOffset: TEdit
            Left = 202
            Top = 57
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtShuttle1YOffset: TEdit
            Left = 202
            Top = 84
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edShuttle2PickOffset: TEdit
            Left = 202
            Top = 112
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '0.50'
            OnClick = edtHotplatePickOffsetClick
          end
          object edtShuttle2PlaceOffset: TEdit
            Left = 202
            Top = 139
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '0.50'
            OnClick = edtHotplatePickOffsetClick
          end
          object edtShuttle2XOffset: TEdit
            Left = 202
            Top = 166
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtShuttle2YOffset: TEdit
            Left = 202
            Top = 194
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
          object edtShuttlePitch: TEdit
            Left = 202
            Top = 221
            Width = 60
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
            Text = '0.50'
            OnClick = edtBufferKitLTXClick
          end
        end
        object edIndexReleaseOffset: TEdit
          Left = 204
          Top = 90
          Width = 60
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '0.50'
          OnClick = edtHotplatePickOffsetClick
        end
      end
      object pnlCleanArm: TPanel
        Left = 2
        Top = 717
        Width = 359
        Height = 82
        Align = alBottom
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 14670284
        TabOrder = 3
        object chkCleanOtherArm: TCheckBox
          Left = 20
          Top = 37
          Width = 141
          Height = 17
          Caption = 'Clean Index Arm 2'
          TabOrder = 2
          Visible = False
        end
        object chkbAutoClean_UseTray: TCheckBox
          Left = 20
          Top = 13
          Width = 141
          Height = 17
          Caption = 'AutoClean_UseTray'
          TabOrder = 1
          Visible = False
        end
        object rgAutoCleanSelectArm: TRadioGroup
          Left = 196
          Top = 5
          Width = 121
          Height = 73
          Caption = 'Contact'
          ItemIndex = 0
          Items.Strings = (
            'Arm1'
            'Arm2'
            'Arm1 & Arm2')
          TabOrder = 0
        end
      end
      object pnlContactMode: TPanel
        Left = 2
        Top = 18
        Width = 359
        Height = 103
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 14670284
        TabOrder = 0
        object lblContactMode: TLabel
          Left = 3
          Top = 13
          Width = 81
          Height = 16
          Caption = 'Contact Mode'
        end
        object lblContactTime: TLabel
          Left = 3
          Top = 46
          Width = 77
          Height = 16
          Caption = 'Contact Time'
        end
        object lblCleanCount: TLabel
          Left = 3
          Top = 78
          Width = 83
          Height = 16
          Caption = 'Contact Count'
        end
        object lblCleanCountTimes: TLabel
          Left = 176
          Top = 78
          Width = 35
          Height = 16
          Caption = 'Times'
        end
        object lblContactTimeSec: TLabel
          Left = 176
          Top = 46
          Width = 23
          Height = 16
          Caption = 'Sec'
        end
        object lblDropOffset: TLabel
          Left = 256
          Top = 13
          Width = 98
          Height = 16
          Caption = 'Drop offSet (mm)'
        end
        object edContactTime: TEdit
          Left = 87
          Top = 42
          Width = 81
          Height = 24
          Hint = 'AutoClean_Contact Time'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '0.50'
          OnClick = edDropOffset1Click
        end
        object edACContactCount: TEdit
          Left = 87
          Top = 74
          Width = 81
          Height = 24
          Hint = 'AutoClean_Contact Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '3'
          OnClick = edtLowYieldLimitClick
        end
        object ContactMode: TComboBox
          Left = 87
          Top = 9
          Width = 153
          Height = 24
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 16
          ParentFont = False
          TabOrder = 0
          Text = 'Direct Contact Mode'
          Items.Strings = (
            'Direct Contact Mode'
            'Drop Contact Mode')
        end
        object edDropOffset1: TEdit
          Left = 256
          Top = 34
          Width = 90
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 1
          OnClick = edDropOffset1Click
        end
        object edACInitalContactCount: TLabeledEdit
          Left = 296
          Top = 74
          Width = 57
          Height = 24
          EditLabel.Width = 65
          EditLabel.Height = 16
          EditLabel.Caption = 'Inital Count'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 4
          OnClick = edACInitalContactCountClick
        end
      end
    end
    object palBtn: TPanel
      Left = 0
      Top = 801
      Width = 363
      Height = 46
      Align = alBottom
      BevelOuter = bvNone
      Color = 14670284
      TabOrder = 1
      object sbCleanSave: TSpeedButton
        Left = 13
        Top = 5
        Width = 150
        Height = 36
        Caption = 'Save'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          660F0000424D660F000000000000360000002800000024000000240000000100
          180000000000300F000001000000010000000000000000000000FAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF9F9F9F9F9F9F8F8F8F8F8F8F8
          F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8
          F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F9F9F9F9F9F9FAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF7F7F7EFEFEFE8
          E8E8E6E6E6E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
          E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
          E5E5E5E5E9E9E9F3F3F3F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAF8F8F8ECECECD6D6D6C8C8C8C5C5C5C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
          C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
          C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4D0D0D0E9E9E9F9F9F9FAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAF0EFEFC0A6A7A55B6CA96270A45E6C934556
          A6A3A3A4A1A1A19C9C9C9797979393948D8D8E88888A8383867F7D837A7A8079
          78807978994759994759994759994759994759994759994759994756C4C4C4E5
          E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAD2C1C1B28181
          AF6D79B8717FB7707DAA6472EFEFF1EEEEF1995D68B8717EB06B78E1E0E0DDDA
          DBD8D5D6D4D1D2CFCCCCCAC6C6DBD8D88F4B59A05565A05565BE848EB9727FB8
          727EB8717E994557C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAAE6A7AAF6D79BA7481B97380B7737FAA6472E9E9EBECEBEF995D
          68B8717EB06B78E1DFE1DCDADDD7D5D6D2D0D1CDC9CAC8C3C3D9D6D68D49589F
          5464A05565BF848EB97380B9737FB9727F9B4457C4C4C4E5E5E5F8F8F8FAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC38D95BA7682BA7481B974
          80AA6472E5E4E7E9E9EB9A5D69B8737FB06B78E5E5E7E2E0E3DDDBDDD8D5D7D3
          D0D1CDCACBDDDADB8B48579D53639F5464BF858FBA7480BA7481BA74819B4558
          C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
          7AC48E96BA7783BA7682B97481AA6472E0DFE1E5E3E69B5F6BB97580B06B78E9
          EAECE6E5E8E2E1E4DDDCDED9D6D8D4D0D2E0DFDF8945549B50619D5363BF8590
          BA7481BA7481BA74819A4759C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAAE6A7AC58F96BB7784BA7784BA7582AA6472DAD9DBE0
          DEE09B5F6BBA7582B06B78EDECEFEAE9EDE6E6E9E2E2E4DEDDDFDAD8DAE5E2E4
          874353994E5F9B5061C18690B97480BA7481BA74819C495CC4C4C4E5E5E5F8F8
          F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC58F98BC7886BB
          7785BB7783AA6472D5D2D4DBD8DA9C606C925864925864EAEAEDEDEDF0EAEAED
          E8E7E9E4E2E5DFDDDFE8E7E8864252974C5D994E5FC08691B9727FB97380B973
          809C4B5CC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAAE6A7AC49098BC7986BD7985BC7784AA6472CFCCCCD4D1D3D9D7D9DFDCDF
          E2E1E4E7E5E8EAEAEDEDECF0EAEBEEE8E7EAE4E2E6ECEAEC8642528642528743
          53C28791B7727FB8727EB9727F9C4E5DC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC79198BD7A87BD7987BC7985BB7785
          AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA64
          72AA6472AA6472AA6472B77A84B76F7DB7707DB7707EB8717E9E4E5FC4C4C4E5
          E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6929A
          BE7B88BE7B87BC7986BC7886BB7785BB7784BA7682B97481B97480B87380B871
          7EB7717EB76F7DB76D7CB76D7BB76D7BB66D7BB56C7AB56C7AB66D7BB76F7CB7
          6F7CB7707D9F4E60C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAAE6A7AC6929ABE7B88BE7B87BC7986BC7886BB7785BB7784BA76
          82B97481B97480B87380B8717EB76F7DB76D7CB76D7CB76C7AB66C7AB66D7BB5
          6C7AB56C7AB56C7AB66C7AB66C7AB76D7C9F4F60C4C4C4E5E5E5F8F8F8FAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6939BBE7E89BC7884A75E
          70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A7
          5E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70B66B7AB66C7B9F5162
          C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
          7AC7949CBF7F8AAF6F7BEBE1E1FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFE
          FEFDFDFEFDFDFDFCFCFCFCFBFCFBFBFBFBFAFAFAF9F9F9F9F9F9F8F8F8E8D1D1
          A75E70B76D7BB66C7A9F5262C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAAE6A7AC8959DC0808BAF6F7BFEFFFEFCFCFBFBFBFAFA
          FBFAFAFAFAF9FAF9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7
          F7F7F7F7F6F6F6F6F6F9F9F9A75E70B96F7DB66B7AA05363C4C4C4E5E5E5F8F8
          F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9959DC1818CAF
          6F7BFEFFFEE1E1E1DFDFDFDDDDDDDBDCDBDADAD9D9D9D9D7D7D6D5D6D5D4D4D3
          D2D3D2D1D1D1D0D0D0CFCFCFCFCFCFCFCFCFCFCFCFF9FAF9A75E70BA707EB66B
          79A15466C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAAE6A7AC9969EC1818DAF6F7BFEFFFEFCFCFCFCFCFBFBFBFBFBFBFAFBFAFA
          FAFAFAF9F9F9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7
          F6FBFBFBA75E70B97480B56B79A25666C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989FC2828EAF6F7BFEFFFEE4E4E4
          E3E3E3E2E2E1E0E0E0DEDDDEDCDCDCDADADAD9D9D9D7D8D8D6D5D5D4D4D4D2D3
          D3D1D2D1D0D0D0CFCFCFCFCFCFFCFCFBA75E70BA7581B56B79A45968C4C4C4E5
          E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989F
          C2838EAF6F7BFEFFFEFCFCFCFCFCFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFA
          F9F9F9F9F9F9F9F9F9F8F8F9F8F8F8F8F8F7F7F7F7F7F7F7F7FCFCFCA75E70BA
          7783B66C7AA55969C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAAE6A7ACA98A0C2838EAF6F7BFEFFFEE6E6E6E6E5E6E4E4E4E3E3
          E3E1E2E2E0E0E0DEDEDEDCDCDCDBDBDBD9D9D9D8D8D8D6D6D6D4D5D5D3D3D3D1
          D1D1D0D0D0FDFDFCA75E70BC7783B76D7BA55D6CC4C4C4E5E5E5F8F8F8FAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB98A0C2838EAF6F7BFEFF
          FEFDFDFDFDFDFCFCFDFCFCFCFCFCFCFBFBFCFBFBFBFBFBFBFAFAFAFAFAFAF9F9
          F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7FDFEFDA75E70BD7985B76F7DA65F6E
          C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
          7ACB9BA1C2838EAF6F7BFEFFFEE6E6E6E6E6E6E6E6E6E5E5E6E4E5E5E4E3E3E2
          E2E2E1E0E0DFDFDFDDDDDDDBDBDCDADAD9D9D8D9D7D6D7D5D5D5D3D4D3FEFEFE
          A75E70BE7A86B7707DA6606EC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFEFDFEFEFDFD
          FDFCFDFDFCFCFDFCFCFCFCFCFCFBFCFCFBFBFBFAFBFBFAFAFAF9F9F9F9F9F9F9
          F9F9F9F9F9F8F8F8F8FEFFFEA75E70B38088B8717FA7606EC4C4C4E5E5E5F8F8
          F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF
          6F7BFEFFFEE6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E5E5E5E4E4E3E2E3E3
          E1E1E1E0DFDFDDDDDEDCDBDCDADADAD9D9D9D7D8D7FEFFFEA75E70996E759E59
          67A9606EC5C5C5E5E5E5F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFFFEFEFEFEFEFEFDFEFEFDFDFDFC
          FDFDFCFCFDFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFAF9FAFAF9F9F9F9F9F9
          F9FEFFFEA75E70B0757F9E5A68AA6471C5C5C5E6E6E6F9F9F9FAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACA969EC2838EAF6F7BFEFFFEFEFFFE
          FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFF
          FEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEA75E70C77F8BC77F8BAA6572D1D1D1EB
          EBEBFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AAE6A7A
          AE6A7AAE6A7AD9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6
          D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6A75E70A7
          5E70A75E70A75E70ECECECF6F6F6FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
          FAFAFAFAFAFAFAFAFAFA}
        ParentFont = False
        OnClick = sbCleanSaveClick
      end
      object sbCleanExit: TSpeedButton
        Left = 177
        Top = 5
        Width = 150
        Height = 36
        AllowAllUp = True
        GroupIndex = 1
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
        OnClick = sbCleanExitClick
      end
    end
  end
end
