object ATCInterfaceForm: TATCInterfaceForm
  Left = 383
  Top = 192
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'ATC Interface '
  ClientHeight = 719
  ClientWidth = 1006
  Color = 13550775
  DefaultMonitor = dmDesktop
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 673
    Width = 1006
    Height = 46
    Align = alBottom
    BevelOuter = bvNone
    BiDiMode = bdRightToLeftNoAlign
    Color = 13550775
    ParentBiDiMode = False
    TabOrder = 1
    object btnClearAll: TBitBtn
      Left = 609
      Top = 3
      Width = 180
      Height = 38
      Caption = 'Clear All'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clPurple
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnClearAllClick
    end
    object btnExit: TBitBtn
      Left = 796
      Top = 3
      Width = 180
      Height = 38
      Caption = 'EXIT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnExitClick
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1006
    Height = 673
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'ATC 2.0'
      object pnlTitle: TPanel
        Left = 0
        Top = 0
        Width = 998
        Height = 41
        Align = alTop
        BevelInner = bvLowered
        Caption = 'ATC System'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object ScrollBoxATC: TScrollBox
        Left = 0
        Top = 41
        Width = 998
        Height = 604
        Align = alClient
        BorderStyle = bsNone
        TabOrder = 1
        object gbATC: TGroupBox
          Left = 8
          Top = 104
          Width = 956
          Height = 121
          Caption = 'Channel01'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
          Visible = False
          object LedATCConnect: TMyLed
            Left = 19
            Top = 21
            Width = 23
            Height = 23
            Hint = 'Connection'
            Interval = 500
            LEDStyle = LEDSqLarge
            ShowHint = True
          end
          object labState: TLabel
            Left = 10
            Top = 46
            Width = 73
            Height = 16
            Alignment = taCenter
            AutoSize = False
            Caption = 'OFF-Line'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object labAddress: TLabel
            Left = 197
            Top = 30
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object labPort: TLabel
            Left = 331
            Top = 30
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object LedATCActive: TMyLed
            Left = 55
            Top = 21
            Width = 23
            Height = 23
            Hint = 'Active'
            Interval = 500
            LEDStyle = LEDSqLarge
            ShowHint = True
          end
          object edtAddress: TEdit
            Left = 195
            Top = 54
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '127.000.000.100'
          end
          object edtPort: TEdit
            Left = 330
            Top = 54
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '1000'
            OnClick = edtPortClick
          end
          object BitBtnConnect: TBitBtn
            Left = 450
            Top = 16
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object BitBtnDisconnect: TBitBtn
            Left = 450
            Top = 51
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
          end
          object edtSendData: TEdit
            Left = 194
            Top = 89
            Width = 251
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 8404992
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object BitBtnSend: TBitBtn
            Left = 450
            Top = 90
            Width = 120
            Height = 30
            Caption = 'SEND'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
          end
          object MemoATC: TMemo
            Left = 576
            Top = 16
            Width = 374
            Height = 97
            Color = 14670284
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ReadOnly = True
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object BitBtnSet: TBitBtn
            Left = 396
            Top = 54
            Width = 49
            Height = 30
            Caption = 'SET'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
          end
          object pal_PV: TPanel
            Left = 12
            Top = 89
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'PV'
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object gbOffset: TGroupBox
            Left = 88
            Top = 44
            Width = 100
            Height = 70
            Caption = 'Offset'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object edtOffset: TEdit
              Left = 10
              Top = 16
              Width = 80
              Height = 26
              TabOrder = 0
              Text = '0.0'
              OnClick = edtOffsetClick
            end
            object btSetOffset: TButton
              Left = 10
              Top = 44
              Width = 80
              Height = 20
              Caption = 'Set'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnClick = btSetOffsetClick
            end
          end
          object cbEnableSite: TCheckBox
            Left = 88
            Top = 24
            Width = 109
            Height = 17
            Caption = 'Enable Site'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pal_SV: TEdit
            Left = 11
            Top = 63
            Width = 70
            Height = 25
            AutoSize = False
            TabOrder = 8
            Text = '0.0'
          end
        end
        object gbChiller: TGroupBox
          Left = 8
          Top = 0
          Width = 289
          Height = 105
          Caption = 'Chiller'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object ledChiller: TMyLed
            Left = 35
            Top = 29
            Width = 23
            Height = 23
            Interval = 500
            LEDStyle = LEDSqLarge
          end
          object Label1: TLabel
            Left = 10
            Top = 56
            Width = 73
            Height = 16
            Alignment = taCenter
            AutoSize = False
            Caption = 'Chiller Stop'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object palChillerPos: TPanel
            Left = 84
            Top = 20
            Width = 100
            Height = 25
            BevelInner = bvLowered
            Caption = 'Position'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object palChillerCurr: TPanel
            Left = 84
            Top = 70
            Width = 100
            Height = 25
            BevelInner = bvLowered
            Caption = 'Current Set'
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object palChillerSet: TPanel
            Left = 84
            Top = 45
            Width = 100
            Height = 25
            BevelInner = bvLowered
            Caption = 'Set Temp'
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pan_ATCTempChiller: TPanel
            Left = 184
            Top = 20
            Width = 100
            Height = 25
            BevelInner = bvLowered
            Caption = 'Chiller'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object pl_ATCTempChiller: TPanel
            Left = 184
            Top = 70
            Width = 100
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object pl_ATCSetTempChiller: TEdit
            Left = 184
            Top = 45
            Width = 100
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0.0'
            OnClick = pl_ATCSetTempChillerClick
          end
        end
        object btnATCChillerSwitchRun: TButton
          Left = 720
          Top = 45
          Width = 120
          Height = 30
          Caption = 'RUN Chiller'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          OnClick = btnATCChillerSwitchRunClick
        end
        object btnATCChillerSwitchStop: TButton
          Left = 720
          Top = 81
          Width = 120
          Height = 30
          Caption = 'STOP Chiller'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          OnClick = btnATCChillerSwitchStopClick
        end
        object btnATCPower: TButton
          Left = 844
          Top = 10
          Width = 120
          Height = 30
          Caption = 'RUN ATC'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = btnATCPowerClick
        end
        object btOnLine: TButton
          Left = 596
          Top = 10
          Width = 120
          Height = 30
          Caption = 'ATC On Line'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btOnLineClick
        end
        object btOffLine: TButton
          Left = 720
          Top = 10
          Width = 120
          Height = 30
          Caption = 'ATC Off Line'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btOffLineClick
        end
        object btSetChillerTemp: TButton
          Left = 596
          Top = 45
          Width = 120
          Height = 30
          Caption = 'Set Chiller Temp'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btSetChillerTempClick
        end
        object btSave: TButton
          Left = 844
          Top = 45
          Width = 120
          Height = 30
          Caption = 'Save Config'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          OnClick = btSaveClick
        end
        object Button1: TButton
          Left = 844
          Top = 81
          Width = 120
          Height = 30
          Caption = 'Start Timer'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
          OnClick = Button1Click
        end
        object btSetWorkTemp: TButton
          Left = 596
          Top = 81
          Width = 120
          Height = 30
          Caption = 'Set Work Temp'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
          OnClick = btSetWorkTempClick
        end
        object gbL11: TGroupBox
          Left = 304
          Top = 4
          Width = 289
          Height = 65
          TabOrder = 1
          object labChillerProtectedFunction: TLabel
            Left = 88
            Top = 36
            Width = 130
            Height = 18
            Caption = ' Check Time (Min).'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edATCChillerCheckTime: TEdit
            Left = 222
            Top = 33
            Width = 55
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ImeName = 'Chinese (Traditional) - Phonetic'
            MaxLength = 2
            ParentFont = False
            TabOrder = 1
            Text = '20'
            OnClick = edATCChillerCheckTimeClick
          end
          object cbChillerProtectedFunction: TCheckBox
            Left = 7
            Top = 9
            Width = 278
            Height = 24
            Caption = 'Enable Chiller Auto Close Protected.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object Button2: TButton
          Left = 460
          Top = 74
          Width = 120
          Height = 30
          Caption = 'Set Now Arm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          OnClick = Button2Click
        end
        object chkSaveLog: TCheckBox
          Left = 308
          Top = 84
          Width = 97
          Height = 17
          Caption = 'Save Log'
          TabOrder = 12
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'ATC 7.0'
      ImageIndex = 1
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 998
        Height = 41
        Align = alTop
        BevelInner = bvLowered
        Caption = 'ATC 7.0 System'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object ScrollBox1: TScrollBox
        Left = 0
        Top = 41
        Width = 998
        Height = 604
        Align = alClient
        BorderStyle = bsNone
        TabOrder = 1
        object LedATC7Connect: TMyLed
          Left = 8
          Top = 53
          Width = 23
          Height = 23
          Hint = 'Connection'
          Interval = 500
          LEDStyle = LEDSqLarge
          ShowHint = True
        end
        object Label2: TLabel
          Left = 40
          Top = 56
          Width = 64
          Height = 13
          Caption = 'ATC Connect'
        end
        object LedATC7Active: TMyLed
          Left = 8
          Top = 85
          Width = 23
          Height = 23
          Hint = 'Connection'
          Interval = 500
          LEDStyle = LEDSqLarge
          ShowHint = True
        end
        object Label3: TLabel
          Left = 40
          Top = 88
          Width = 54
          Height = 13
          Caption = 'ATC Active'
        end
        object HandlerMemo: TMemo
          Left = 584
          Top = 12
          Width = 536
          Height = 605
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          ScrollBars = ssBoth
          TabOrder = 0
        end
        object GroupBox3: TGroupBox
          Left = 136
          Top = 48
          Width = 105
          Height = 97
          Caption = 'CH 1'
          TabOrder = 1
          object Panel_CH1_Temp: TPanel
            Left = 10
            Top = 23
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel_CH1_TSDTemp: TPanel
            Left = 10
            Top = 56
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object GroupBox2: TGroupBox
          Left = 248
          Top = 48
          Width = 105
          Height = 97
          Caption = 'CH 2'
          TabOrder = 2
          object Panel_CH2_Temp: TPanel
            Left = 10
            Top = 23
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel_CH2_TSDTemp: TPanel
            Left = 10
            Top = 56
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object GroupBox4: TGroupBox
          Left = 8
          Top = 152
          Width = 569
          Height = 361
          Caption = 'Manual Control'
          TabOrder = 5
          object cBoxEnabledCH1: TCheckBox
            Left = 24
            Top = 28
            Width = 121
            Height = 17
            Caption = 'Channel 1 Enabled'
            TabOrder = 1
          end
          object cBoxEnabledCH2: TCheckBox
            Left = 24
            Top = 52
            Width = 121
            Height = 17
            Caption = 'Channel 2 Enabled'
            TabOrder = 2
          end
          object cBoxEnabledTSD: TCheckBox
            Left = 24
            Top = 132
            Width = 97
            Height = 17
            Caption = 'Enabled TSD'
            TabOrder = 6
          end
          object edATC7_Temp: TEdit
            Left = 24
            Top = 168
            Width = 121
            Height = 21
            TabOrder = 7
            Text = '30'
            OnClick = edATC7_TempClick
          end
          object BitBtn1: TBitBtn
            Left = 152
            Top = 168
            Width = 113
            Height = 25
            Caption = 'Set Temptrature'
            TabOrder = 8
            OnClick = BitBtn1Click
          end
          object BitBtn2: TBitBtn
            Left = 24
            Top = 216
            Width = 75
            Height = 49
            Caption = 'Run'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
            OnClick = BitBtn2Click
          end
          object BitBtn3: TBitBtn
            Left = 120
            Top = 216
            Width = 75
            Height = 49
            Caption = 'Stop'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
            OnClick = BitBtn3Click
          end
          object BitBtn4: TBitBtn
            Left = 152
            Top = 128
            Width = 75
            Height = 25
            Caption = 'Set TSD'
            TabOrder = 5
            OnClick = BitBtn4Click
          end
          object BitBtn5: TBitBtn
            Left = 152
            Top = 24
            Width = 75
            Height = 89
            Caption = 'Set Enabled'
            TabOrder = 0
            OnClick = BitBtn5Click
          end
          object cBoxEnabledCH3: TCheckBox
            Left = 24
            Top = 76
            Width = 121
            Height = 17
            Caption = 'Channel 3 Enabled'
            TabOrder = 3
          end
          object cBoxEnabledCH4: TCheckBox
            Left = 24
            Top = 100
            Width = 121
            Height = 17
            Caption = 'Channel 4 Enabled'
            TabOrder = 4
          end
        end
        object GroupBox1: TGroupBox
          Left = 360
          Top = 48
          Width = 105
          Height = 97
          Caption = 'CH 3'
          TabOrder = 3
          object Panel_CH3_Temp: TPanel
            Left = 10
            Top = 23
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel_CH3_TSDTemp: TPanel
            Left = 10
            Top = 56
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object GroupBox5: TGroupBox
          Left = 472
          Top = 48
          Width = 105
          Height = 97
          Caption = 'CH 4'
          TabOrder = 4
          object Panel_CH4_Temp: TPanel
            Left = 10
            Top = 23
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel_CH4_TSDTemp: TPanel
            Left = 10
            Top = 56
            Width = 87
            Height = 34
            BevelInner = bvLowered
            Caption = '0.0'
            Color = clInfoBk
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
      end
    end
  end
  object ATCWatchTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = ATCWatchTimerTimer
    Left = 180
  end
  object TimerChillerStop: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerChillerStopTimer
    Left = 208
  end
  object ATC7_ServerSocket: TServerSocket
    Active = False
    Port = 7000
    ServerType = stNonBlocking
    OnClientConnect = ATC7_ServerSocketClientConnect
    OnClientDisconnect = ATC7_ServerSocketClientDisconnect
    OnClientRead = ATC7_ServerSocketClientRead
    Left = 244
  end
  object TimerATC: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerATCTimer
    Left = 154
  end
end
