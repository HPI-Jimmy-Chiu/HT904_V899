object ACTForm: TACTForm
  Left = 225
  Top = 95
  Width = 1194
  Height = 853
  Caption = 'Auto Temperature'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 20
  object pcACT: TPageControl
    Left = 0
    Top = 0
    Width = 1186
    Height = 822
    ActivePage = tsTemperature
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object tsTemperature: TTabSheet
      Caption = 'Temperature'
      object pnlData: TPanel
        Left = 0
        Top = 0
        Width = 265
        Height = 585
        Align = alLeft
        BevelInner = bvLowered
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object labTimer: TLabel
          Left = 4
          Top = 124
          Width = 193
          Height = 20
          AutoSize = False
          Caption = 'Auto Offset Timer: 00:00'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 105
          Top = 190
          Width = 16
          Height = 20
          Caption = #65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 197
          Top = 7
          Width = 48
          Height = 20
          Caption = 'Range'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object pnlBase01: TPanel
          Left = 21
          Top = 30
          Width = 89
          Height = 25
          Caption = 'Low Base'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clPurple
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlBase02: TPanel
          Left = 21
          Top = 62
          Width = 89
          Height = 25
          Caption = 'Mid Base'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clPurple
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlBase03: TPanel
          Left = 21
          Top = 94
          Width = 89
          Height = 25
          Caption = 'High Base'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clPurple
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pnlBaseTemp01: TPanel
          Left = 115
          Top = 30
          Width = 81
          Height = 25
          BevelInner = bvLowered
          Caption = '000.00'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlBaseTemp02: TPanel
          Left = 115
          Top = 62
          Width = 81
          Height = 25
          BevelInner = bvLowered
          Caption = '000.00'
          Color = 50688
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlBaseTemp03: TPanel
          Left = 115
          Top = 94
          Width = 81
          Height = 25
          BevelInner = bvLowered
          Caption = '000.00'
          Color = 10921472
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object cbBase01: TCheckBox
          Left = 4
          Top = 34
          Width = 17
          Height = 17
          TabOrder = 3
        end
        object cbBase02: TCheckBox
          Left = 4
          Top = 66
          Width = 17
          Height = 17
          TabOrder = 7
        end
        object cbBase03: TCheckBox
          Left = 4
          Top = 98
          Width = 17
          Height = 17
          TabOrder = 11
        end
        object btnAutoStart: TBitBtn
          Left = 4
          Top = 383
          Width = 192
          Height = 41
          Caption = 'START'
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -24
          Font.Name = 'Courier New'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 17
          OnClick = btnAutoStartClick
        end
        object btnDutOnOff: TBitBtn
          Left = 4
          Top = 339
          Width = 192
          Height = 41
          Caption = 'GET On / Off'
          Font.Charset = ANSI_CHARSET
          Font.Color = clOlive
          Font.Height = -24
          Font.Name = 'Courier New'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 16
          OnClick = btnDutOnOffClick
        end
        object btShowPos: TBitBtn
          Left = 4
          Top = 295
          Width = 192
          Height = 41
          Caption = 'Default Position'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Courier New'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 15
          OnClick = btShowPosClick
        end
        object chkAutoStart: TCheckBox
          Left = 4
          Top = 164
          Width = 189
          Height = 17
          Caption = 'Enable auto start Timer'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
        object edtAutoStartHr: TEdit
          Left = 56
          Top = 184
          Width = 49
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 13
          Text = '00'
          OnMouseDown = edtAutoStartHrMouseDown
        end
        object edtAutoStartMin: TEdit
          Left = 124
          Top = 184
          Width = 49
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 14
          Text = '00'
          OnMouseDown = edtAutoStartMinMouseDown
        end
        object edtRange1: TEdit
          Left = 200
          Top = 28
          Width = 41
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '3'
          OnMouseDown = edtRange1MouseDown
        end
        object edtRange2: TEdit
          Left = 200
          Top = 60
          Width = 41
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '3'
          OnMouseDown = edtRange1MouseDown
        end
        object edtRange3: TEdit
          Left = 200
          Top = 92
          Width = 41
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          Text = '3'
          OnMouseDown = edtRange1MouseDown
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 585
        Width = 1178
        Height = 202
        Align = alBottom
        BevelInner = bvLowered
        Color = 12761254
        TabOrder = 2
        object MemoLog: TMemo
          Left = 2
          Top = 2
          Width = 1174
          Height = 87
          Align = alTop
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          ScrollBars = ssBoth
          TabOrder = 0
        end
        object MemoOffset: TMemo
          Left = 2
          Top = 89
          Width = 989
          Height = 111
          Align = alClient
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          ScrollBars = ssBoth
          TabOrder = 1
        end
        object palSaveLog: TPanel
          Left = 991
          Top = 89
          Width = 185
          Height = 111
          Align = alRight
          Caption = 'Save Log'
          TabOrder = 2
          OnClick = palSaveLogClick
        end
      end
      object ScrollBox1: TScrollBox
        Left = 265
        Top = 0
        Width = 913
        Height = 585
        Align = alClient
        TabOrder = 1
        object gbMeasurePoint: TGroupBox
          Left = 4
          Top = 4
          Width = 280
          Height = 105
          Caption = 'Measure Point'
          TabOrder = 0
          Visible = False
          object ImgOriginalOffset: TImage
            Left = 174
            Top = 16
            Width = 100
            Height = 84
          end
          object pnlTemp: TPanel
            Left = 4
            Top = 21
            Width = 168
            Height = 25
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 10419454
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object cbUse01: TCheckBox
            Left = 4
            Top = 80
            Width = 89
            Height = 17
            Caption = 'Use'
            TabOrder = 2
          end
          object cbMeasurePoint: TComboBox
            Left = 4
            Top = 48
            Width = 168
            Height = 28
            ItemHeight = 20
            TabOrder = 1
            Text = 'Hot Plate 1'
            Items.Strings = (
              'Plate 1'
              'Plate 2'
              'Shuttle 1'
              'Shuttle 2'
              'Head 1/2'
              'Head 3/4'
              'Head 5/6'
              'Head 7/8'
              'Dut'
              'Chamber'
              'CCD'
              'A1'
              'C1'
              'E1'
              'G1'
              'B1'
              'D1'
              'F1'
              'H1'
              'A2'
              'C2'
              'E2'
              'G2'
              'B2'
              'D2'
              'F2'
              'H2'
              'Heat Gun1'
              'Heat Gun2')
          end
        end
      end
    end
    object tsSetup: TTabSheet
      Caption = 'Setup'
      ImageIndex = 1
      object GroupBox1: TGroupBox
        Left = 363
        Top = 21
        Width = 313
        Height = 332
        Caption = 'COM Port Setting'
        TabOrder = 1
        object Label1: TLabel
          Left = 10
          Top = 132
          Width = 104
          Height = 20
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Baud Rate'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 10
          Top = 175
          Width = 104
          Height = 20
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Byte Size'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 10
          Top = 217
          Width = 104
          Height = 20
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Stop Bit'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 10
          Top = 259
          Width = 104
          Height = 20
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Parity'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 10
          Top = 89
          Width = 104
          Height = 20
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Device'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 14
          Top = 23
          Width = 207
          Height = 20
          AutoSize = False
          Caption = 'Thermo Contraller Type'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbBaudRate: TComboBox
          Left = 112
          Top = 128
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 2
          Text = 'cbBaudRate'
          Items.Strings = (
            '4800'
            '7200'
            '9600'
            '14400'
            '19200')
        end
        object cbByteSize: TComboBox
          Left = 112
          Top = 171
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 3
          Text = 'cbByteSize'
          Items.Strings = (
            '5'
            '6'
            '7'
            '8')
        end
        object cbStopBit: TComboBox
          Left = 112
          Top = 213
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 4
          Text = 'cbStopBit'
          Items.Strings = (
            '1'
            '1.5'
            '2')
        end
        object cbParity: TComboBox
          Left = 112
          Top = 255
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 5
          Text = 'cbParity'
          Items.Strings = (
            'None'
            'Odd'
            'Even'
            'Mark'
            'Space')
        end
        object cbDevice: TComboBox
          Left = 112
          Top = 86
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = 'cbCOM'
          Items.Strings = (
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9'
            'COM10'
            'COM11'
            'COM12'
            'COM13'
            'COM14'
            'COM15'
            'COM16'
            'COM17'
            'COM18')
        end
        object cbbThermoCtrlType: TComboBox
          Left = 112
          Top = 43
          Width = 170
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ItemIndex = 1
          ParentFont = False
          TabOrder = 0
          Text = 'Delta DTB4824'
          Items.Strings = (
            'Agilent 34970A'
            'Delta DTB4824')
        end
      end
      object btnUpdate: TButton
        Left = 16
        Top = 500
        Width = 321
        Height = 41
        Caption = 'Update'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = btnUpdateClick
      end
      object gbACTSetting: TGroupBox
        Left = 16
        Top = 21
        Width = 329
        Height = 316
        Caption = 'A.C.T. Setting'
        TabOrder = 0
        object Label6: TLabel
          Left = 20
          Top = 43
          Width = 155
          Height = 20
          Caption = 'Check Interval Time'#65306
        end
        object Label7: TLabel
          Left = 277
          Top = 44
          Width = 29
          Height = 20
          Caption = 'Min.'
        end
        object Label8: TLabel
          Left = 32
          Top = 83
          Width = 143
          Height = 20
          Caption = 'Calibration Range'#65306
          Visible = False
        end
        object Label9: TLabel
          Left = 41
          Top = 123
          Width = 134
          Height = 20
          Caption = 'Read Scan Time'#65306
        end
        object Label10: TLabel
          Left = 277
          Top = 123
          Width = 32
          Height = 20
          Caption = 'Sec.'
        end
        object Label11: TLabel
          Left = 7
          Top = 163
          Width = 168
          Height = 20
          Caption = 'Maximum Offset Limit'#65306
        end
        object Label13: TLabel
          Left = 30
          Top = 203
          Width = 145
          Height = 20
          Caption = 'Single Offset Limit'#65306
        end
        object edtCheckIntervalTime: TEdit
          Left = 180
          Top = 40
          Width = 89
          Height = 28
          TabOrder = 0
          Text = '10'
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
        object edtCalibrationRange: TEdit
          Left = 180
          Top = 80
          Width = 89
          Height = 28
          TabOrder = 1
          Text = '3'
          Visible = False
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
        object edtReadScanTime: TEdit
          Left = 180
          Top = 120
          Width = 89
          Height = 28
          TabOrder = 2
          Text = '5'
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
        object edtOffsetLimit: TEdit
          Left = 180
          Top = 159
          Width = 89
          Height = 28
          TabOrder = 3
          Text = '30'
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
        object edSingleOffset: TEdit
          Left = 180
          Top = 199
          Width = 89
          Height = 28
          TabOrder = 4
          Text = '30'
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
        object rgOffsetMethod: TRadioGroup
          Left = 4
          Top = 236
          Width = 317
          Height = 65
          Caption = 'Offset Method'
          Columns = 2
          ItemIndex = 1
          Items.Strings = (
            'Max - Min'
            'Average')
          TabOrder = 5
        end
      end
      object gbDisplay: TGroupBox
        Left = 16
        Top = 364
        Width = 329
        Height = 65
        Caption = 'Display Setting'
        TabOrder = 2
        object Label12: TLabel
          Left = 12
          Top = 28
          Width = 106
          Height = 20
          Caption = 'Columns No.'#65306
        end
        object edColumns: TEdit
          Left = 164
          Top = 24
          Width = 89
          Height = 28
          TabOrder = 0
          Text = '3'
          OnMouseDown = edtCheckIntervalTimeMouseDown
        end
      end
    end
  end
  object ACTCom: TComm
    CommName = 'COM1'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = False
    Inx_XonXoffFlow = False
    ReplaceWhenParityError = False
    IgnoreNullChar = False
    RtsControl = RtsEnable
    XonLimit = 500
    XoffLimit = 500
    ByteSize = _8
    Parity = None
    StopBits = _1
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = ACTComReceiveData
    Left = 672
  end
  object TimerACT: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerACTTimer
    Left = 645
  end
end
