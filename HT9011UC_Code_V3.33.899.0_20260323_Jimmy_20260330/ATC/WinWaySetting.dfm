object fWinway: TfWinway
  Left = 280
  Top = 264
  Width = 330
  Height = 518
  Caption = 'ATC WinWay Setting'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblWinwayNumber: TLabel
    Left = 16
    Top = 8
    Width = 24
    Height = 20
    Caption = 'No.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object btnUpdate: TSpeedButton
    Left = 16
    Top = 308
    Width = 287
    Height = 41
    Caption = 'Save'
    Font.Charset = ANSI_CHARSET
    Font.Color = 25600
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = btnUpdateClick
  end
  object grpcps: TGroupBox
    Left = 11
    Top = 40
    Width = 289
    Height = 273
    Caption = 'COM Port Setting'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lbl1: TLabel
      Left = 2
      Top = 84
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
    object lbl2: TLabel
      Left = 2
      Top = 131
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
    object lbl3: TLabel
      Left = 2
      Top = 178
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
    object lbl4: TLabel
      Left = 2
      Top = 225
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
    object lbl5: TLabel
      Left = 2
      Top = 37
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
    object cbbBaudRate: TComboBox
      Left = 104
      Top = 81
      Width = 170
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 20
      ParentFont = False
      TabOrder = 0
      Text = 'cbbBaudRate'
      Items.Strings = (
        '4800'
        '7200'
        '9600'
        '14400'
        '19200'
        '57600'
        '115200')
    end
    object cbbByteSize: TComboBox
      Left = 104
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
      TabOrder = 1
      Text = 'cbbByteSize'
      Items.Strings = (
        '5'
        '6'
        '7'
        '8')
    end
    object cbbStopBit: TComboBox
      Left = 104
      Top = 174
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
      Text = 'cbbStopBit'
      Items.Strings = (
        '1'
        '1.5'
        '2')
    end
    object cbbParity: TComboBox
      Left = 104
      Top = 221
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
      Text = 'cbbParity'
      Items.Strings = (
        'None'
        'Odd'
        'Even'
        'Mark'
        'Space')
    end
    object cbbDevice: TComboBox
      Left = 104
      Top = 34
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
        'COM15')
    end
  end
  object cbbWinwayATCIndex: TComboBox
    Left = 48
    Top = 8
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 1
    OnChange = cbbWinwayATCIndexChange
    Items.Strings = (
      'ATC 1'
      'ATC 2'
      'ATC 3'
      'ATC 4')
  end
  object grpcommu: TGroupBox
    Left = 8
    Top = 368
    Width = 289
    Height = 97
    Caption = 'Communication'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object lbl6: TLabel
      Left = 16
      Top = 24
      Width = 115
      Height = 20
      Caption = 'Set Temprature:'
    end
    object lbl7: TLabel
      Left = 16
      Top = 64
      Width = 140
      Height = 20
      Caption = 'Present Tempature:'
    end
    object lblShowPT: TLabel
      Left = 184
      Top = 64
      Width = 9
      Height = 20
      Caption = '0'
    end
    object edtSetTemp: TEdit
      Left = 136
      Top = 24
      Width = 97
      Height = 28
      TabOrder = 0
      OnClick = edtSetTempClick
    end
    object btnSendTemp: TButton
      Left = 248
      Top = 24
      Width = 33
      Height = 33
      Caption = '>>'
      TabOrder = 1
      OnClick = btnSendTempClick
    end
    object btnGetPV: TButton
      Left = 248
      Top = 64
      Width = 33
      Height = 33
      Caption = '<<'
      TabOrder = 2
      OnClick = btnGetPVClick
    end
  end
  object WinWayATCComm1: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
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
    OnReceiveData = WinWayATCComm1ReceiveData
    Left = 184
    Top = 8
  end
  object WinWayATCComm2: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
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
    OnReceiveData = WinWayATCComm1ReceiveData
    Left = 216
    Top = 8
  end
  object WinWayATCComm3: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
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
    OnReceiveData = WinWayATCComm1ReceiveData
    Left = 248
    Top = 8
  end
  object WinWayATCComm4: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
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
    OnReceiveData = WinWayATCComm1ReceiveData
    Left = 280
    Top = 8
  end
end
