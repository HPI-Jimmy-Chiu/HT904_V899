object fSetup: TfSetup
  Left = 192
  Top = 133
  Width = 1090
  Height = 987
  Caption = 'Setup'
  Color = 13550775
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object palLeft: TPanel
    Left = 0
    Top = 0
    Width = 1074
    Height = 948
    Align = alClient
    BevelOuter = bvNone
    Color = 13550775
    TabOrder = 0
    object RadioButton6: TRadioButton
      Left = -85
      Top = 22
      Width = 113
      Height = 17
      Caption = 'RadioButton5'
      TabOrder = 3
    end
    object Panel3: TPanel
      Left = 508
      Top = 0
      Width = 294
      Height = 690
      Align = alRight
      BevelOuter = bvNone
      Caption = 'Panel3'
      Color = 13550775
      TabOrder = 1
      object gbShuttleMode: TGroupBox
        Left = 0
        Top = 597
        Width = 294
        Height = 93
        Align = alBottom
        Caption = 'Shuttle Mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Visible = False
        object rgShtModeNormal: TRadioButton
          Left = 16
          Top = 27
          Width = 122
          Height = 18
          Caption = 'Normal'
          TabOrder = 1
          OnClick = rgShtModeNormalClick
          OnMouseDown = rgShtModeNormalMouseDown
        end
        object rgShtModeOneSide: TRadioButton
          Left = 16
          Top = 55
          Width = 122
          Height = 19
          Caption = 'One Side'
          TabOrder = 2
          OnClick = rgShtModeNormalClick
          OnMouseDown = rgShtModeNormalMouseDown
        end
        object grpShuttleSelect: TGroupBox
          Left = 125
          Top = 16
          Width = 139
          Height = 65
          Caption = 'Shuttle Select'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object rgUseSht1: TRadioButton
            Left = 8
            Top = 23
            Width = 121
            Height = 18
            Caption = 'Use Shuttle1'
            TabOrder = 0
          end
          object rgUseSht2: TRadioButton
            Left = 8
            Top = 42
            Width = 121
            Height = 18
            Caption = 'Use Shuttle2'
            TabOrder = 1
          end
        end
      end
      object grpIndexOption: TGroupBox
        Left = 0
        Top = 120
        Width = 294
        Height = 481
        Align = alTop
        TabOrder = 2
        object chkOffCenterkit: TCheckBox
          Left = 13
          Top = 9
          Width = 275
          Height = 19
          Caption = 'NS7000 bias kit ( Off-Centre)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = chkOffCenterkitClick
        end
        object chkNS7000CS: TCheckBox
          Left = 13
          Top = 28
          Width = 275
          Height = 18
          Caption = 'NS7000 change socket'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
        object chkRotateShuttle: TCheckBox
          Left = 13
          Top = 64
          Width = 275
          Height = 19
          Caption = 'Rotate shuttle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          Visible = False
        end
        object cbNS8000H: TCheckBox
          Left = 13
          Top = 102
          Width = 275
          Height = 18
          Caption = 'NS8000H change socket'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
        end
        object chkOctal80: TCheckBox
          Left = 13
          Top = 172
          Width = 275
          Height = 18
          Caption = 'Octal site X pitch 80 mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 9
        end
        object cbOctal16Site: TCheckBox
          Left = 13
          Top = 190
          Width = 275
          Height = 18
          Caption = 'Octal site use 16 site SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 10
        end
        object cbOctal12Site: TCheckBox
          Left = 13
          Top = 208
          Width = 275
          Height = 18
          Caption = 'Octal site use 12 site SLK'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          State = cbChecked
          TabOrder = 11
          OnClick = cbOctal12SiteClick
        end
        object cb16DirectHeater: TCheckBox
          Left = 13
          Top = 225
          Width = 275
          Height = 18
          Caption = 'One by one direct heater SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 12
        end
        object cb12Site10DirectHeater: TCheckBox
          Left = 13
          Top = 243
          Width = 275
          Height = 18
          Caption = '12 site use 10 heater SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 13
        end
        object cbSquareOctalLayout: TCheckBox
          Left = 13
          Top = 119
          Width = 275
          Height = 18
          Caption = '2x2 site use 2x4 site SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
        end
        object cb2CableLayoutKit: TCheckBox
          Left = 13
          Top = 296
          Width = 275
          Height = 18
          Caption = 'Use 2 cable SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 16
        end
        object cbHotechLayoutKit2x2: TCheckBox
          Left = 13
          Top = 45
          Width = 275
          Height = 19
          Caption = 'HonPrec heater SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object cb1CableLayoutKit: TCheckBox
          Left = 13
          Top = 314
          Width = 275
          Height = 18
          Caption = 'Use 1 cable SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 17
        end
        object cbQualSite2X2Shift: TCheckBox
          Left = 13
          Top = 332
          Width = 275
          Height = 18
          Caption = 'SLK shuttle X shift'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 18
          OnClick = cbQualSite2X2ShiftClick
        end
        object cbSingleSiteSingleHeater: TCheckBox
          Left = 13
          Top = 83
          Width = 275
          Height = 19
          Caption = 'Single site single heater'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          Visible = False
        end
        object cbNSKitPress: TCheckBox
          Left = 13
          Top = 349
          Width = 275
          Height = 18
          Caption = 'NS kit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 19
          Visible = False
        end
        object chk2x2Use16siteSLK: TCheckBox
          Left = 13
          Top = 155
          Width = 275
          Height = 18
          Caption = '2x2 site use 16 site SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
        end
        object cb16change12DirectHeater: TCheckBox
          Left = 13
          Top = 261
          Width = 275
          Height = 18
          Caption = '16 site SLK with 12 site shuttle '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 14
          Visible = False
        end
        object cbIndSLK: TCheckBox
          Left = 13
          Top = 367
          Width = 275
          Height = 18
          Caption = 'Individual EP SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 20
          Visible = False
        end
        object cb16change8DirectHeater: TCheckBox
          Left = 13
          Top = 279
          Width = 275
          Height = 18
          Caption = '16 site SLK with 8 site shuttle '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 15
          Visible = False
        end
        object cbUseRotateForHT7000HPKit: TCheckBox
          Left = 13
          Top = 385
          Width = 275
          Height = 21
          Caption = 'Use hot plate rotate kit  '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 21
          Visible = False
        end
        object cb1x2Use1x4siteSLK: TCheckBox
          Left = 13
          Top = 406
          Width = 275
          Height = 18
          Caption = 'Dual site use 1x4 SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 22
        end
        object cbSingleUseOtherSuck: TCheckBox
          Left = 13
          Top = 423
          Width = 280
          Height = 18
          Caption = '1x1 mode use In C / Out E picker'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 23
        end
        object cb6CableLayoutKit: TCheckBox
          Left = 13
          Top = 441
          Width = 279
          Height = 18
          Caption = 'Use 6 cable SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 24
        end
        object chk12SiteUse2x8SLK: TCheckBox
          Left = 13
          Top = 137
          Width = 275
          Height = 18
          Caption = '12 site use 16 site SLK'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          State = cbChecked
          TabOrder = 7
          OnClick = cbOctal12SiteClick
        end
        object cbSingleInArmUseOtherSuck: TCheckBox
          Left = 14
          Top = 458
          Width = 280
          Height = 18
          Caption = '1x1 mode use In C / Out G Picker'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 25
        end
        object cbUse1x3siteSLK: TCheckBox
          Left = 13
          Top = 473
          Width = 275
          Height = 18
          Caption = 'Use 1x3 SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 26
        end
      end
      object rgUseSuckMode: TRadioGroup
        Left = 0
        Top = 60
        Width = 294
        Height = 60
        Align = alTop
        Caption = 'In Arm Use Suck Mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          'Use 4 pick unit'
          'Use 8 pick unit')
        ParentFont = False
        TabOrder = 1
        OnClick = rgUseSuckModeClick
      end
      object rgSelectSearchLast: TRadioGroup
        Left = 0
        Top = 0
        Width = 294
        Height = 60
        Align = alTop
        Caption = 'In && Out Arm Suck or Destroy Mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ItemIndex = 1
        Items.Strings = (
          'Normal Mode'
          'Search Last Mode')
        ParentFont = False
        TabOrder = 0
      end
    end
    object grpHandlingMode: TGroupBox
      Left = 0
      Top = 0
      Width = 508
      Height = 690
      Align = alClient
      Caption = 'Handling Mode'
      Color = 13550775
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object Image1: TImage
        Left = 186
        Top = 71
        Width = 259
        Height = 171
        AutoSize = True
      end
      object lblXPitch: TLabel
        Left = 223
        Top = 257
        Width = 46
        Height = 17
        Caption = 'X Pitch'
      end
      object lblXPMM: TLabel
        Left = 395
        Top = 256
        Width = 26
        Height = 17
        Caption = 'mm'
      end
      object lblYPitch: TLabel
        Left = 39
        Top = 90
        Width = 86
        Height = 17
        Caption = 'Y Pitch (mm)'
      end
      object labYOffset: TLabel
        Left = 36
        Top = 227
        Width = 93
        Height = 17
        Caption = 'Y Offset (mm)'
      end
      object ScrollBar1: TScrollBar
        Left = 468
        Top = 74
        Width = 17
        Height = 167
        Kind = sbVertical
        Max = 8
        PageSize = 0
        TabOrder = 2
        OnChange = ScrollBar1Change
      end
      object Panel1: TPanel
        Left = 188
        Top = 31
        Width = 257
        Height = 37
        BevelInner = bvLowered
        Caption = 'Panel1'
        Color = 13550775
        TabOrder = 1
      end
      object XPitch: TEdit
        Left = 279
        Top = 252
        Width = 113
        Height = 25
        TabOrder = 7
        Text = 'XPitch'
        OnKeyPress = XPitchKeyPress
        OnMouseDown = XPitchMouseDown
      end
      object YPitch: TEdit
        Left = 34
        Top = 110
        Width = 96
        Height = 25
        TabOrder = 3
        OnKeyPress = XPitchKeyPress
        OnMouseDown = XPitchMouseDown
      end
      object rgInOutArmYPitch: TRadioGroup
        Left = 9
        Top = 23
        Width = 121
        Height = 62
        Caption = 'In Arm Y Pitch'
        Color = 13550775
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          '60.0 mm'
          '63.5 mm')
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
      object edYOffset: TEdit
        Left = 34
        Top = 247
        Width = 96
        Height = 25
        TabOrder = 6
        OnKeyPress = XPitchKeyPress
        OnMouseDown = XShiftPitchMouseDown
      end
      object paQualSite2X2Shift: TPanel
        Left = 204
        Top = 280
        Width = 229
        Height = 33
        BevelOuter = bvNone
        Color = 13550775
        TabOrder = 9
        object Label1: TLabel
          Left = 191
          Top = 7
          Width = 26
          Height = 17
          Caption = 'mm'
        end
        object Label2: TLabel
          Left = 1
          Top = 9
          Width = 66
          Height = 17
          Caption = 'Shift Pitch'
        end
        object XShiftPitch: TEdit
          Left = 75
          Top = 4
          Width = 113
          Height = 25
          TabOrder = 0
          Text = 'XShiftPitch'
          OnKeyPress = XPitchKeyPress
          OnMouseDown = XShiftPitchMouseDown
        end
      end
      object btAutoShuttlePitch: TButton
        Left = 8
        Top = 324
        Width = 129
        Height = 25
        Caption = 'Auto Shuttle Pitch'
        TabOrder = 10
        OnClick = btAutoShuttlePitchClick
      end
      object rgYOffset: TRadioGroup
        Left = 24
        Top = 256
        Width = 97
        Height = 57
        Caption = 'Y-Offset'
        ItemIndex = 0
        Items.Strings = (
          '30mm'
          '15mm')
        TabOrder = 8
      end
      object rgYPitchOffsetMode: TRadioGroup
        Left = 9
        Top = 139
        Width = 157
        Height = 82
        Caption = 'Y-Offset'
        ItemIndex = 0
        Items.Strings = (
          'For input arm'
          'For index arm'
          'Y Center Pitch')
        TabOrder = 4
        OnClick = rgYPitchOffsetModeClick
      end
      object Panel6: TPanel
        Left = 2
        Top = 379
        Width = 504
        Height = 309
        Align = alBottom
        BevelOuter = bvNone
        Color = 13550775
        TabOrder = 11
        object Panel7: TPanel
          Left = 0
          Top = 0
          Width = 264
          Height = 309
          Align = alLeft
          BevelOuter = bvNone
          Color = 13550775
          TabOrder = 0
          object grpUseXCenterPitch: TGroupBox
            Left = 0
            Top = 244
            Width = 264
            Height = 65
            Align = alBottom
            Caption = 'Use X Center Pitch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object Label3: TLabel
              Left = 175
              Top = 44
              Width = 26
              Height = 17
              Caption = 'mm'
            end
            object chkUseXCenterPitch: TCheckBox
              Left = 12
              Top = 20
              Width = 201
              Height = 17
              Caption = 'Enable Use X Center Pitch'
              TabOrder = 0
              OnClick = chkOffCenterkitClick
            end
            object edtXCenterPitch: TEdit
              Left = 60
              Top = 36
              Width = 113
              Height = 25
              TabOrder = 1
              OnKeyPress = XPitchKeyPress
              OnMouseDown = XPitchMouseDown
            end
          end
          object cbF18: TCheckBox
            Left = 8
            Top = 4
            Width = 223
            Height = 19
            Caption = '[F18] In shuttle product detect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Visible = False
          end
          object gbPreciser: TGroupBox
            Left = 0
            Top = 129
            Width = 264
            Height = 115
            Align = alBottom
            Caption = 'Preciser'
            TabOrder = 3
            object Label4: TLabel
              Left = 6
              Top = 62
              Width = 172
              Height = 17
              Caption = 'X Pitch :                       mm'
            end
            object Label5: TLabel
              Left = 6
              Top = 86
              Width = 172
              Height = 17
              Caption = 'Y Pitch :                       mm'
            end
            object cbEnablePreciser: TCheckBox
              Left = 6
              Top = 16
              Width = 130
              Height = 17
              Caption = 'Enable Preciser'
              TabOrder = 0
              OnClick = chkOffCenterkitClick
            end
            object edPreciserXPitch: TEdit
              Left = 69
              Top = 56
              Width = 72
              Height = 25
              TabOrder = 2
              OnKeyPress = XPitchKeyPress
              OnMouseDown = XPitchMouseDown
            end
            object edPreciserYPitch: TEdit
              Left = 69
              Top = 84
              Width = 72
              Height = 25
              TabOrder = 3
              OnKeyPress = XPitchKeyPress
              OnMouseDown = XPitchMouseDown
            end
            object cbEnabledPreciserRT: TCheckBox
              Left = 6
              Top = 36
              Width = 203
              Height = 17
              Caption = 'Enable Preciser when RT'
              TabOrder = 1
              OnClick = chkOffCenterkitClick
            end
          end
          object cbUseTesterDry: TCheckBox
            Left = 8
            Top = 26
            Width = 153
            Height = 19
            Caption = 'Use Tester Dry'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            OnClick = cbUseSLKClampClick
          end
          object grpHeadCondition: TGroupBox
            Left = 0
            Top = 44
            Width = 264
            Height = 85
            Align = alBottom
            Caption = 'Head Condition'
            TabOrder = 2
            object cbO12_LifeTimeCount: TCheckBox
              Left = 8
              Top = 17
              Width = 250
              Height = 26
              BiDiMode = bdRightToLeft
              Caption = '[O12] Use Head 1 Life Time Control .'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
            end
            object cbO13_LifeTimeCount: TCheckBox
              Left = 8
              Top = 36
              Width = 250
              Height = 26
              BiDiMode = bdRightToLeft
              Caption = '[O13] Use Head 2 Life Time Control .'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
            end
            object cbO14_LifeTimeCount: TCheckBox
              Left = 8
              Top = 55
              Width = 250
              Height = 26
              BiDiMode = bdRightToLeft
              Caption = '[O14] Use Head 3 Life Time Control .'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 2
            end
          end
        end
        object Panel8: TPanel
          Left = 264
          Top = 0
          Width = 240
          Height = 309
          Align = alClient
          BevelOuter = bvNone
          Color = 13550775
          TabOrder = 1
          object grpAutoSiteMap: TGroupBox
            Left = 0
            Top = 0
            Width = 240
            Height = 81
            Align = alTop
            Caption = 'Auto site mapping'
            TabOrder = 0
            object chkSiteMapOpenSite: TCheckBox
              Left = 20
              Top = 28
              Width = 108
              Height = 17
              Caption = 'Open Site'
              TabOrder = 0
            end
            object chkSiteMapOneCycle: TCheckBox
              Left = 20
              Top = 53
              Width = 115
              Height = 17
              Caption = 'One cycle'
              TabOrder = 1
            end
          end
          object gbOutUseBackRow: TGroupBox
            Left = 0
            Top = 81
            Width = 240
            Height = 42
            Align = alTop
            Caption = 'Out arm module'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object cbOutUseBackRow: TCheckBox
              Left = 8
              Top = 18
              Width = 139
              Height = 19
              Caption = 'Use Back Row'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 0
              OnClick = cbEnableRealTimeCCDClick
            end
          end
          object gbInUseBackRow: TGroupBox
            Left = 0
            Top = 123
            Width = 240
            Height = 42
            Align = alTop
            Caption = 'In arm module'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object cbInUseBackRow: TCheckBox
              Left = 8
              Top = 18
              Width = 139
              Height = 19
              Caption = 'Use Back Row'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 0
              OnClick = cbEnableRealTimeCCDClick
            end
          end
          object gbRTC: TGroupBox
            Left = 0
            Top = 165
            Width = 240
            Height = 141
            Align = alTop
            Caption = 'Real Time CCD'
            TabOrder = 3
            object lbShowMessage: TLabel
              Left = 4
              Top = 20
              Width = 179
              Height = 17
              Caption = '* Turn On need Clean-Out !'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -15
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object cbEnableRealTimeCCD: TCheckBox
              Left = 4
              Top = 42
              Width = 208
              Height = 19
              Caption = 'Enable Real Time CCD'
              Checked = True
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              State = cbChecked
              TabOrder = 0
              OnClick = cbEnableRealTimeCCDClick
            end
            object cbEnableSocketFloat: TCheckBox
              Left = 4
              Top = 64
              Width = 209
              Height = 19
              Caption = 'Enable Socket Float'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
            end
            object edtRTCFileName: TEdit
              Left = 8
              Top = 111
              Width = 237
              Height = 25
              TabOrder = 3
            end
            object cbEnableStepAsideMode: TCheckBox
              Left = 4
              Top = 86
              Width = 229
              Height = 19
              Caption = 'Enable Step Aside Mode'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -17
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
            end
          end
        end
      end
      object palVisibleIndex: TPanel
        Left = 12
        Top = 174
        Width = 152
        Height = 26
        BevelOuter = bvNone
        Color = 13550775
        TabOrder = 5
      end
    end
    object Panel2: TPanel
      Left = 0
      Top = 893
      Width = 1074
      Height = 55
      Align = alBottom
      BevelOuter = bvNone
      Color = 13550775
      TabOrder = 5
      object sbUpdate: TSpeedButton
        Left = 61
        Top = 8
        Width = 323
        Height = 43
        Caption = 'Save'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -29
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
        OnClick = sbUpdateClick
      end
      object sbtExit: TSpeedButton
        Left = 427
        Top = 7
        Width = 323
        Height = 44
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Exit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -29
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
        OnClick = sbtExitClick
      end
      object cbI21: TCheckBox
        Left = 793
        Top = 6
        Width = 267
        Height = 17
        Caption = 'Enable auto site mapping function'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = cbI21Click
      end
      object rbTemp: TRadioButton
        Left = -77
        Top = 4
        Width = 113
        Height = 17
        Caption = 'rbTemp'
        TabOrder = 0
      end
    end
    object grpFunction: TGroupBox
      Left = 802
      Top = 0
      Width = 272
      Height = 690
      Align = alRight
      Caption = 'Additional Functions'
      Color = 13550775
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 2
      object gbArm1PickArm2Test: TGroupBox
        Left = 2
        Top = 19
        Width = 268
        Height = 214
        Align = alTop
        Caption = 'Arm1PickPlaceArm2Test'
        TabOrder = 0
        object Arm1PickArm2Test: TCheckBox
          Left = 9
          Top = 19
          Width = 204
          Height = 22
          Caption = 'Enable Function'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = Arm1PickArm2TestClick
        end
        object cbCheckArm2Vacuum: TCheckBox
          Left = 18
          Top = 44
          Width = 250
          Height = 17
          Caption = 'Check Arm2 Vacuum'
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
        object cbArm1UseHeat: TCheckBox
          Left = 9
          Top = 167
          Width = 204
          Height = 22
          Caption = 'Arm1 Use Heat'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 7
          OnClick = Arm1PickArm2TestClick
        end
        object cbRTCICResidueCheck: TCheckBox
          Left = 34
          Top = 147
          Width = 179
          Height = 17
          Caption = 'RTC IC Residue Check'
          TabOrder = 6
        end
        object cbRTC20OverFlowCheck: TCheckBox
          Left = 34
          Top = 127
          Width = 195
          Height = 17
          Caption = 'RTC IC Placement Check'
          Checked = True
          State = cbChecked
          TabOrder = 5
          Visible = False
        end
        object cbRTC20GiveWayCheck: TCheckBox
          Left = 18
          Top = 107
          Width = 250
          Height = 17
          Caption = 'RTC Give Way Check'
          Checked = True
          State = cbChecked
          TabOrder = 4
        end
        object cbRTC20CheckFunction: TCheckBox
          Left = 18
          Top = 86
          Width = 250
          Height = 17
          Caption = 'RTC Check Standard'
          Checked = True
          State = cbChecked
          TabOrder = 3
        end
        object cbAutoCleanAlso: TCheckBox
          Left = 18
          Top = 65
          Width = 250
          Height = 17
          Caption = 'Auto Clean'
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object cbArm1OnlyPlaceArm2TestAndSuck: TCheckBox
          Left = 9
          Top = 183
          Width = 256
          Height = 22
          Caption = 'Arm1 Only Place'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
          OnClick = Arm1PickArm2TestClick
        end
      end
      object gbOcr: TGroupBox
        Left = 2
        Top = 233
        Width = 268
        Height = 64
        Align = alTop
        Caption = 'OCR'
        TabOrder = 1
        object cbOcrFunction: TCheckBox
          Left = 8
          Top = 18
          Width = 208
          Height = 19
          Caption = 'Enable OCR Function'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = cbEnableRealTimeCCDClick
        end
        object edOcrText: TEdit
          Left = 8
          Top = 36
          Width = 229
          Height = 25
          TabOrder = 1
          OnMouseDown = edOcrTextMouseDown
        end
      end
      object gbSocketClamp: TGroupBox
        Left = 2
        Top = 297
        Width = 268
        Height = 94
        Align = alTop
        Caption = 'SLK type'
        TabOrder = 2
        object cbUseSLKClamp: TCheckBox
          Left = 8
          Top = 18
          Width = 189
          Height = 19
          Caption = 'Use Clamp SLK'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = cbUseSLKClampClick
        end
        object rgseparabilityTest: TRadioGroup
          Left = 4
          Top = 37
          Width = 209
          Height = 53
          Caption = 'Motion'
          Items.Strings = (
            'Normal'
            'Separate')
          TabOrder = 1
        end
      end
      object grpSocketSensor: TGroupBox
        Left = 2
        Top = 391
        Width = 268
        Height = 297
        Align = alClient
        Caption = 'Socket Sensor'
        TabOrder = 3
        object chkCheckFloating: TCheckBox
          Left = 8
          Top = 60
          Width = 197
          Height = 17
          Caption = 'Check Device Floating'
          TabOrder = 1
          Visible = False
        end
        object scrlbxSocketSensor: TScrollBox
          Left = 2
          Top = 69
          Width = 264
          Height = 226
          Align = alClient
          BevelInner = bvNone
          BevelOuter = bvNone
          TabOrder = 2
        end
        object Panel4: TPanel
          Left = 2
          Top = 19
          Width = 264
          Height = 50
          Align = alTop
          BevelOuter = bvNone
          Color = 13550775
          TabOrder = 0
          object cbdisibleinitialcheck: TCheckBox
            Left = 8
            Top = 30
            Width = 208
            Height = 19
            Caption = 'Disable initial check'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = cbEnableRealTimeCCDClick
          end
          object cbSocketSensor: TCheckBox
            Left = 8
            Top = 2
            Width = 149
            Height = 19
            Caption = 'Socket sensor'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            OnClick = cbEnableRealTimeCCDClick
          end
          object CoSocketCombo: TComboBox
            Left = 162
            Top = -1
            Width = 72
            Height = 25
            Style = csDropDownList
            ImeMode = imClose
            ItemHeight = 17
            TabOrder = 0
            OnChange = CoSocketComboChange
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
              '15'
              '16'
              '17'
              '18'
              '19'
              '20'
              '21'
              '22'
              '23'
              '24')
          end
        end
      end
    end
    object Panel5: TPanel
      Left = 0
      Top = 690
      Width = 1074
      Height = 203
      Align = alBottom
      BevelOuter = bvNone
      Color = 13550775
      TabOrder = 4
      object grpSiteMap: TGroupBox
        Left = 0
        Top = 0
        Width = 800
        Height = 203
        Align = alClient
        Caption = 'Test Site Assign'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object labRowA: TLabel
          Left = 9
          Top = 41
          Width = 39
          Height = 17
          Caption = 'RowA'
        end
        object labRowB: TLabel
          Left = 9
          Top = 71
          Width = 40
          Height = 17
          Caption = 'RowB'
        end
        object labColA: TLabel
          Left = 90
          Top = 16
          Width = 8
          Height = 17
          Caption = 'a'
        end
        object labColB: TLabel
          Left = 176
          Top = 16
          Width = 8
          Height = 17
          Caption = 'b'
        end
        object labColC: TLabel
          Left = 262
          Top = 16
          Width = 8
          Height = 17
          Caption = 'c'
        end
        object labColD: TLabel
          Left = 347
          Top = 16
          Width = 8
          Height = 17
          Caption = 'd'
        end
        object labColE: TLabel
          Left = 432
          Top = 16
          Width = 8
          Height = 17
          Caption = 'e'
        end
        object labColF: TLabel
          Left = 517
          Top = 16
          Width = 4
          Height = 17
          Caption = 'f'
        end
        object labColG: TLabel
          Left = 603
          Top = 16
          Width = 8
          Height = 17
          Caption = 'g'
        end
        object labColH: TLabel
          Left = 688
          Top = 16
          Width = 8
          Height = 17
          Caption = 'h'
        end
        object btnLUpToRDownN: TSpeedButton
          Tag = 4
          Left = 491
          Top = 157
          Width = 43
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6000000000000CE
            D3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6000000000000CE
            D3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6000000000000CED3
            D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D60000000000
            00CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D60000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D60000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object btnLUpToRDownZ: TSpeedButton
          Left = 538
          Top = 157
          Width = 44
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000000000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000
            00000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object btnLDownToRUpZ: TSpeedButton
          Tag = 1
          Left = 586
          Top = 157
          Width = 43
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000000000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000000000000000
            00CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D60000000000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D60000000000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object btnRUpToLDownZ: TSpeedButton
          Tag = 2
          Left = 633
          Top = 157
          Width = 43
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000
            00000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000000000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object btnRDownToLUpZ: TSpeedButton
          Tag = 3
          Left = 681
          Top = 157
          Width = 43
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D60000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000000000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D60000
            00000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D60000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object btnRUpToLDownN: TSpeedButton
          Tag = 5
          Left = 728
          Top = 157
          Width = 43
          Height = 38
          Glyph.Data = {
            D60B0000424DD60B00000000000036000000280000001F0000001F0000000100
            180000000000A00B0000C40E0000C40E00000000000000000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6000000000000000000000000000000000000CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600000000000000
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6000000000000CED3D6000000000000CED3D60000000000
            00CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6000000000000000000CED3D6CED3D6000000000000CED3D6CED3D60000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6000000000000CED3D6CED3D6CED3D6000000000000CED3D6CED3D60000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6000000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D60000
            00000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D60000000000
            00CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D60000000000
            00CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000000000CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000000000CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000000000CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D600
            0000000000CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000CED3D6CED3D6
            CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3
            D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CE
            D3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6CED3D6000000}
          OnClick = btnLUpToRDownNClick
        end
        object labRowC: TLabel
          Left = 9
          Top = 102
          Width = 41
          Height = 17
          Caption = 'RowC'
        end
        object labRowD: TLabel
          Left = 9
          Top = 132
          Width = 41
          Height = 17
          Caption = 'RowD'
        end
        object cbAa: TComboBox
          Left = 61
          Top = 37
          Width = 72
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 0
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbAb: TComboBox
          Left = 149
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 1
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbAc: TComboBox
          Left = 235
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 2
          Text = '1'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbAd: TComboBox
          Left = 321
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 2
          TabOrder = 3
          Text = '3'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbAe: TComboBox
          Left = 407
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 4
          TabOrder = 4
          Text = '5'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbAf: TComboBox
          Left = 493
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 6
          TabOrder = 5
          Text = '7'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbAg: TComboBox
          Left = 579
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 6
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbAh: TComboBox
          Left = 666
          Top = 37
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 7
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbBa: TComboBox
          Left = 61
          Top = 67
          Width = 72
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 8
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbBb: TComboBox
          Left = 149
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 9
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbBc: TComboBox
          Left = 235
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 1
          TabOrder = 10
          Text = '2'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbBd: TComboBox
          Left = 321
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 3
          TabOrder = 11
          Text = '4'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbBe: TComboBox
          Left = 407
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 5
          TabOrder = 12
          Text = '6'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbBf: TComboBox
          Left = 493
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 7
          TabOrder = 13
          Text = '8'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object cbBg: TComboBox
          Left = 579
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 14
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbBh: TComboBox
          Left = 666
          Top = 67
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          ItemIndex = 0
          TabOrder = 15
          Text = '---'
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
          Items.Strings = (
            '---')
        end
        object cbCa: TComboBox
          Left = 61
          Top = 98
          Width = 72
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 16
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCb: TComboBox
          Left = 149
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 17
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCc: TComboBox
          Left = 235
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 18
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCd: TComboBox
          Left = 321
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 19
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCe: TComboBox
          Left = 407
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 20
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCf: TComboBox
          Left = 493
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 21
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCg: TComboBox
          Left = 579
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 22
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbCh: TComboBox
          Left = 666
          Top = 98
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 23
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDa: TComboBox
          Left = 61
          Top = 128
          Width = 72
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 24
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDb: TComboBox
          Left = 149
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 25
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDc: TComboBox
          Left = 235
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 26
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDd: TComboBox
          Left = 321
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 27
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDe: TComboBox
          Left = 407
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 28
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDf: TComboBox
          Left = 493
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 29
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDg: TComboBox
          Left = 579
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 30
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
        object cbDh: TComboBox
          Left = 666
          Top = 128
          Width = 71
          Height = 25
          Style = csDropDownList
          ItemHeight = 17
          TabOrder = 31
          OnChange = cbAaChange
          OnDropDown = cbAaDropDown
        end
      end
      object pgcASE: TPageControl
        Left = 800
        Top = 0
        Width = 274
        Height = 203
        ActivePage = tsCheckTorque
        Align = alRight
        TabIndex = 0
        TabOrder = 1
        object tsCheckTorque: TTabSheet
          Caption = 'Check Torque'
          object lblD01_1: TLabel
            Left = 15
            Top = 29
            Width = 227
            Height = 16
            Caption = 'Over torque Range Error                      % '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object lblD01_2: TLabel
            Left = 15
            Top = 51
            Width = 228
            Height = 16
            Caption = 'Delay Read Time                                 sec'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object lblGetValueDelayTime: TLabel
            Left = 15
            Top = 73
            Width = 229
            Height = 16
            Caption = 'Get Value DelayTime                          sec'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object chkReadTorque: TCheckBox
            Left = 8
            Top = 0
            Width = 221
            Height = 29
            Caption = 'Enable read and check torque '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object edDelayTime: TEdit
            Left = 167
            Top = 48
            Width = 46
            Height = 22
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            TabOrder = 2
            Text = '0'
            OnClick = edDelayTimeClick
          end
          object edOverRange: TEdit
            Left = 167
            Top = 26
            Width = 46
            Height = 22
            TabOrder = 1
            Text = '0'
            OnClick = edOverRangeClick
          end
        end
        object tsTrayMap: TTabSheet
          Caption = 'Tray Map'
          ImageIndex = 1
          object Label32: TLabel
            Left = 16
            Top = 89
            Width = 109
            Height = 16
            Caption = 'Auto1 Tray Count :'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label42: TLabel
            Left = 16
            Top = 117
            Width = 109
            Height = 16
            Caption = 'Auto2 Tray Count :'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label43: TLabel
            Left = 16
            Top = 144
            Width = 109
            Height = 16
            Caption = 'Auto3 Tray Count :'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edAuto1Count: TEdit
            Left = 160
            Top = 85
            Width = 100
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = edAuto1CountClick
          end
          object edAuto2Count: TEdit
            Left = 160
            Top = 113
            Width = 100
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = edAuto1CountClick
          end
          object edAuto3Count: TEdit
            Left = 160
            Top = 140
            Width = 100
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            OnClick = edAuto1CountClick
          end
          object bUseTrayMap: TCheckBox
            Left = 8
            Top = 33
            Width = 221
            Height = 14
            Caption = 'Enable Auto 123 Tray Map'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object bUseLoadCCD: TCheckBox
            Left = 8
            Top = 6
            Width = 221
            Height = 19
            TabStop = False
            Caption = 'Enable Load tray  check map'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object bDoubleUnloadTray: TCheckBox
            Left = 8
            Top = 54
            Width = 141
            Height = 14
            Caption = 'Double unload tray'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object edtGetValueDelayTime: TEdit
            Left = 160
            Top = 50
            Width = 100
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnClick = edtGetValueDelayTimeClick
          end
        end
      end
    end
  end
end
