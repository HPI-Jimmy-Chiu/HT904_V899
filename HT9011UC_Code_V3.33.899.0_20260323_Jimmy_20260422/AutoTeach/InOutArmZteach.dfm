object Zteach: TZteach
  Left = 396
  Top = 163
  Width = 1181
  Height = 752
  Caption = 'Auto Teach'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1173
    Height = 718
    Align = alClient
    Color = 12761254
    TabOrder = 0
    object SpeedButton14: TSpeedButton
      Left = 996
      Top = 264
      Width = 100
      Height = 30
      Caption = 'STOP'
      OnClick = SpeedButton14Click
    end
    object SpeedButton11: TSpeedButton
      Left = 996
      Top = 228
      Width = 100
      Height = 30
      Caption = 'Start'
      OnClick = SpeedButton11Click
    end
    object GroupBox8: TGroupBox
      Left = 6
      Top = 3
      Width = 305
      Height = 130
      Caption = 'InArm Z'
      TabOrder = 0
      object Label46: TLabel
        Left = 32
        Top = 16
        Width = 7
        Height = 13
        Caption = 'A'
      end
      object Label47: TLabel
        Left = 32
        Top = 59
        Width = 7
        Height = 13
        Caption = 'B'
      end
      object Label48: TLabel
        Left = 104
        Top = 16
        Width = 7
        Height = 13
        Caption = 'C'
      end
      object Label53: TLabel
        Left = 104
        Top = 59
        Width = 8
        Height = 13
        Caption = 'D'
      end
      object Label58: TLabel
        Left = 175
        Top = 16
        Width = 7
        Height = 13
        Caption = 'E'
      end
      object Label59: TLabel
        Left = 175
        Top = 59
        Width = 6
        Height = 13
        Caption = 'F'
      end
      object Label60: TLabel
        Left = 247
        Top = 16
        Width = 8
        Height = 13
        Caption = 'G'
      end
      object Label61: TLabel
        Left = 247
        Top = 59
        Width = 8
        Height = 13
        Caption = 'H'
      end
      object AEdit1: TEdit
        Left = 11
        Top = 32
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 0
        OnClick = AEdit1Click
      end
      object AEdit2: TEdit
        Left = 9
        Top = 75
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 4
        OnClick = AEdit1Click
      end
      object AEdit3: TEdit
        Left = 84
        Top = 32
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 1
        OnClick = AEdit1Click
      end
      object AEdit4: TEdit
        Left = 82
        Top = 75
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 5
        OnClick = AEdit1Click
      end
      object AEdit5: TEdit
        Left = 156
        Top = 32
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 2
        OnClick = AEdit1Click
      end
      object AEdit6: TEdit
        Left = 156
        Top = 75
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 6
        OnClick = AEdit1Click
      end
      object AEdit7: TEdit
        Left = 229
        Top = 32
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 3
        OnClick = AEdit1Click
      end
      object AEdit8: TEdit
        Left = 229
        Top = 75
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 7
        OnClick = AEdit1Click
      end
      object SendInarm: TButton
        Left = 222
        Top = 101
        Width = 75
        Height = 25
        Caption = 'Send InArm'
        TabOrder = 8
        OnClick = SendInarmClick
      end
    end
    object GroupBox9: TGroupBox
      Left = 322
      Top = 4
      Width = 305
      Height = 130
      Caption = 'OutArm Z'
      TabOrder = 1
      object Label62: TLabel
        Left = 32
        Top = 16
        Width = 7
        Height = 13
        Caption = 'A'
      end
      object Label63: TLabel
        Left = 32
        Top = 59
        Width = 7
        Height = 13
        Caption = 'B'
      end
      object Label64: TLabel
        Left = 104
        Top = 16
        Width = 7
        Height = 13
        Caption = 'C'
      end
      object Label65: TLabel
        Left = 104
        Top = 59
        Width = 8
        Height = 13
        Caption = 'D'
      end
      object Label70: TLabel
        Left = 175
        Top = 16
        Width = 7
        Height = 13
        Caption = 'E'
      end
      object Label85: TLabel
        Left = 175
        Top = 59
        Width = 6
        Height = 13
        Caption = 'F'
      end
      object Label101: TLabel
        Left = 247
        Top = 16
        Width = 8
        Height = 13
        Caption = 'G'
      end
      object Label102: TLabel
        Left = 247
        Top = 59
        Width = 8
        Height = 13
        Caption = 'H'
      end
      object AOutEdit7: TEdit
        Left = 229
        Top = 33
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 3
        OnClick = AEdit1Click
      end
      object AOutEdit5: TEdit
        Left = 156
        Top = 33
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 2
        OnClick = AEdit1Click
      end
      object AOutEdit3: TEdit
        Left = 84
        Top = 33
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 1
        OnClick = AEdit1Click
      end
      object AOutEdit1: TEdit
        Left = 11
        Top = 33
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 0
        OnClick = AEdit1Click
      end
      object AOutEdit8: TEdit
        Left = 229
        Top = 73
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 7
        OnClick = AEdit1Click
      end
      object AOutEdit6: TEdit
        Left = 156
        Top = 73
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 6
        OnClick = AEdit1Click
      end
      object AOutEdit4: TEdit
        Left = 84
        Top = 73
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 5
        OnClick = AEdit1Click
      end
      object AOutEdit2: TEdit
        Left = 11
        Top = 73
        Width = 60
        Height = 21
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        TabOrder = 4
        OnClick = AEdit1Click
      end
      object SendOutarm: TButton
        Left = 218
        Top = 100
        Width = 75
        Height = 25
        Caption = 'Send OutArm'
        TabOrder = 8
        OnClick = SendOutarmClick
      end
    end
    object Memo1: TMemo
      Left = 755
      Top = 161
      Width = 233
      Height = 259
      Color = 14670284
      ImeMode = imClose
      TabOrder = 9
    end
    object BitBtn1: TBitBtn
      Left = 732
      Top = 646
      Width = 145
      Height = 41
      Caption = 'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 15
      OnClick = BitBtn1Click
    end
    object AutoTeachSingle: TRadioGroup
      Left = 820
      Top = 9
      Width = 185
      Height = 105
      Caption = 'AutoTeach'
      Color = clRed
      ItemIndex = 0
      Items.Strings = (
        'Single'
        'Full-AutoTeach')
      ParentColor = False
      TabOrder = 3
      OnClick = AutoTeachSingleClick
    end
    object StringGrid1: TStringGrid
      Left = 10
      Top = 423
      Width = 491
      Height = 214
      Color = 14670284
      ColCount = 13
      DefaultColWidth = 80
      DefaultRowHeight = 20
      FixedColor = 9534289
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
      OnMouseDown = StringGrid1MouseDown
    end
    object SendData: TButton
      Left = 92
      Top = 641
      Width = 85
      Height = 41
      Caption = 'SendData'
      TabOrder = 14
      OnClick = SendDataClick
    end
    object rgTeachMode: TRadioGroup
      Left = 632
      Top = 9
      Width = 185
      Height = 105
      Caption = 'Teaching Mode'
      Color = clAqua
      ItemIndex = 0
      Items.Strings = (
        '1 point teach'
        '3 point teach'
        'Z teach offset')
      ParentColor = False
      TabOrder = 2
      OnClick = rgTeachModeClick
    end
    object gbInarm: TGroupBox
      Left = 8
      Top = 155
      Width = 313
      Height = 265
      Caption = 'Input Arm'
      TabOrder = 7
      object sbHotPlate2: TSpeedButton
        Tag = 5
        Left = 7
        Top = 15
        Width = 97
        Height = 88
        GroupIndex = 1
        Caption = 'Hot Plate 2'
        OnClick = sbLoaderClick
      end
      object sbHotPlate1: TSpeedButton
        Tag = 4
        Left = 7
        Top = 110
        Width = 97
        Height = 88
        GroupIndex = 1
        Caption = 'Hot Plate 1'
        OnClick = sbLoaderClick
      end
      object sbInSht2: TSpeedButton
        Tag = 2
        Left = 112
        Top = 16
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Shuttle 2'
        OnClick = sbLoaderClick
      end
      object sbInSht1: TSpeedButton
        Tag = 1
        Left = 113
        Top = 94
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Shuttle 1'
        OnClick = sbLoaderClick
      end
      object sbInZ_Calibration: TSpeedButton
        Left = 213
        Top = 16
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Z Calibration'
        OnClick = sbLoaderClick
      end
      object sbInRotate: TSpeedButton
        Tag = 6
        Left = 112
        Top = 52
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Rotate'
        OnClick = sbLoaderClick
      end
      object sbLoader: TSpeedButton
        Tag = 3
        Left = 113
        Top = 166
        Width = 97
        Height = 87
        GroupIndex = 1
        Caption = 'Loader'
        OnClick = sbLoaderClick
      end
    end
    object gbOutArm: TGroupBox
      Left = 324
      Top = 155
      Width = 429
      Height = 265
      Caption = 'Output Arm'
      TabOrder = 8
      object sbFix1: TSpeedButton
        Tag = 16
        Left = 116
        Top = 16
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Fix 1'
        OnClick = sbLoaderClick
      end
      object sbFix2: TSpeedButton
        Tag = 17
        Left = 188
        Top = 16
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Fix 2'
        OnClick = sbLoaderClick
      end
      object sbFix3: TSpeedButton
        Tag = 18
        Left = 260
        Top = 16
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Fix 3'
        OnClick = sbLoaderClick
      end
      object sbAuto3: TSpeedButton
        Tag = 15
        Left = 260
        Top = 156
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Auto 3'
        OnClick = sbLoaderClick
      end
      object sbAuto2: TSpeedButton
        Tag = 14
        Left = 188
        Top = 156
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Auto 2'
        OnClick = sbLoaderClick
      end
      object sbAuto1: TSpeedButton
        Tag = 13
        Left = 116
        Top = 156
        Width = 65
        Height = 85
        GroupIndex = 1
        Caption = 'Auto 1'
        OnClick = sbLoaderClick
      end
      object sbOutSht2: TSpeedButton
        Tag = 12
        Left = 8
        Top = 16
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Shuttle 2'
        OnClick = sbLoaderClick
      end
      object sbOutSht1: TSpeedButton
        Tag = 11
        Left = 8
        Top = 94
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Shuttle 1'
        OnClick = sbLoaderClick
      end
      object sbOutZ_Calibration: TSpeedButton
        Tag = 10
        Left = 329
        Top = 17
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Z Calibration'
        OnClick = sbLoaderClick
      end
      object sbOutRotate: TSpeedButton
        Tag = 19
        Left = 8
        Top = 56
        Width = 97
        Height = 33
        GroupIndex = 1
        Caption = 'Rotate'
        OnClick = sbLoaderClick
      end
    end
    object sgThreePointTeach: TStringGrid
      Left = 502
      Top = 423
      Width = 411
      Height = 214
      Color = 14670284
      ColCount = 12
      DefaultColWidth = 80
      DefaultRowHeight = 20
      FixedColor = 9534289
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnDrawCell = sgThreePointTeachDrawCell
      RowHeights = (
        20
        20
        20
        20
        20)
    end
    object PanelTeachPosSelect: TPanel
      Left = 632
      Top = 116
      Width = 313
      Height = 41
      BevelInner = bvLowered
      Color = 9534289
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -23
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
    object cbEnableUseAreaZCalbration: TCheckBox
      Left = 8
      Top = 136
      Width = 177
      Height = 17
      Caption = 'Enable Use Area Z Calibration'
      TabOrder = 5
      OnClick = cbEnableUseAreaZCalbrationClick
    end
    object CbEnRecordVacuum: TCheckBox
      Left = 321
      Top = 137
      Width = 177
      Height = 17
      Caption = 'Record Vacuum Time'
      TabOrder = 6
    end
    object Panel2: TPanel
      Left = 917
      Top = 424
      Width = 232
      Height = 273
      Color = 14670284
      TabOrder = 13
      object MotorInArmX: TSpeedButton
        Left = 4
        Top = 168
        Width = 100
        Height = 30
        AllowAllUp = True
        Caption = 'X'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        OnClick = MotorInArmXClick
      end
      object MotorInArmY: TSpeedButton
        Tag = 1
        Left = 4
        Top = 208
        Width = 100
        Height = 30
        AllowAllUp = True
        Caption = 'Y'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        OnClick = MotorInArmXClick
      end
      object SpeedButton13: TSpeedButton
        Left = 3
        Top = 107
        Width = 110
        Height = 49
        AllowAllUp = True
        Caption = 'Move -'
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #26032#32048#26126#39636
        Font.Style = []
        Glyph.Data = {
          AA040000424DAA04000000000000360000002800000014000000130000000100
          1800000000007404000000000000000000000000000000000000C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FFFFFFFFFFFFC7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7FFFF
          FFFFFFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7FFFFFFFFFFFFFF0707FFFFFF
          C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7FFFFFFFFFFFFFF0707FF0707FF0707FFFFFFC7BFC7C7
          BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          FFFFFFFFFFFFFF0707FF0707FF0707FF0707FF0707FFFFFFC7BFC7C7BFC7C7BF
          C7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7FFFFFFFFFFFFFF0707FF
          0707FF0707FF0707FF0707FF0707FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7
          877F87FF0707FFFFFFC7BFC7C7BFC7FFFFFFFF0707FF0707FF0707FF0707FF07
          07FF0707FF0707FF0707FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF
          0707FFFFFFC7BFC7877F87877F87FF0707FF0707FF0707FF0707FF0707FF0707
          FF0707FF0707FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFF
          FFC7BFC7C7BFC7C7BFC7877F87877F87FF0707FF0707FF0707FF0707FF0707FF
          0707FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7877F87877F87FF0707FF0707FF0707FF0707FF07
          07FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87877F87FF0707FF0707FF0707FFFFFF
          C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87877F87FF0707FFFFFFC7BFC7C7
          BFC7C7BFC7C7BFC7877F87877F87FFFFFFC7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FFFFFFC7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7}
        Layout = blGlyphRight
        ParentFont = False
        OnClick = SpeedButton13Click
      end
      object SpeedButton12: TSpeedButton
        Left = 119
        Top = 107
        Width = 103
        Height = 49
        AllowAllUp = True
        Caption = 'Move +'
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #26032#32048#26126#39636
        Font.Style = []
        Glyph.Data = {
          AA040000424DAA04000000000000360000002800000014000000130000000100
          1800000000007404000000000000000000000000000000000000C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7FFFFFFC7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F
          87FFFFFFFFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFFFFFFFC7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707
          FFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707FF0707FF0707FFFFFFFFFFFFC7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7
          BFC7C7BFC7C7BFC7877F87FF0707FF0707FF0707FF0707FF0707FFFFFFFFFFFF
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BF
          C7C7BFC7877F87FF0707FF0707FF0707FF0707FF0707FF0707FF0707FFFFFFFF
          FFFFC7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7
          877F87FF0707FF0707FF0707FF0707FF0707FF0707FF0707FF0707FF0707FFFF
          FFFFFFFFC7BFC7C7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7877F87FF
          0707FF0707FF0707FF0707FF0707FF0707FF0707FF0707FF0707877F87877F87
          FFFFFFC7BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707FF07
          07FF0707FF0707FF0707FF0707FF0707877F87877F87C7BFC7C7BFC7C7BFC7C7
          BFC7877F87FF0707FFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707FF0707FF0707
          FF0707FF0707877F87877F87C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F
          87FF0707FFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707FF0707FF0707877F8787
          7F87C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707
          FFFFFFC7BFC7C7BFC7C7BFC7877F87FF0707877F87877F87C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87FF0707FFFFFFC7
          BFC7C7BFC7C7BFC7877F87877F87C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7877F87877F87FFFFFFC7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7
          C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BF
          C7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7BFC7C7
          BFC7C7BFC7C7BFC7C7BFC7C7BFC7}
        ParentFont = False
        OnClick = SpeedButton12Click
      end
      object Label4: TLabel
        Left = 131
        Top = 42
        Width = 91
        Height = 20
        Caption = 'Now Position'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object SpeedButton1: TSpeedButton
        Left = 112
        Top = 230
        Width = 109
        Height = 30
        AllowAllUp = True
        Caption = 'Start Auto Z offset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        OnClick = SpeedButton1Click
      end
      object ComboBox1: TComboBox
        Left = 137
        Top = 171
        Width = 81
        Height = 32
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ItemHeight = 24
        ParentFont = False
        TabOrder = 3
        Text = '1'
        Items.Strings = (
          '1'
          '10'
          '100'
          '1000'
          '10000')
      end
      object Panel3: TPanel
        Left = 1
        Top = 4
        Width = 225
        Height = 31
        BevelInner = bvLowered
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -23
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object pnlEncoderPos: TPanel
        Left = 4
        Top = 64
        Width = 89
        Height = 31
        BevelInner = bvLowered
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -23
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object edtPosition: TEdit
        Left = 124
        Top = 69
        Width = 97
        Height = 32
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
        ParentFont = False
        TabOrder = 2
        OnClick = edtPositionClick
      end
    end
    object Button1: TButton
      Left = 996
      Top = 164
      Width = 100
      Height = 30
      Caption = 'Vacuum Check'
      TabOrder = 10
      OnClick = Button1Click
    end
  end
  object AutoTimer: TTimer
    Interval = 30
    OnTimer = AutoTimerTimer
    Left = 991
    Top = 4
  end
end
