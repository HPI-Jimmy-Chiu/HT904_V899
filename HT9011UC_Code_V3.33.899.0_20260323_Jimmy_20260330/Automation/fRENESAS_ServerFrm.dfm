object fServerFrm: TfServerFrm
  Left = 311
  Top = 247
  Width = 508
  Height = 336
  Caption = 'ServerFrm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  inline frameProdInfo1: TframeProdInfo
    Left = 4
    Top = 4
    Width = 173
    Height = 299
    TabOrder = 0
    inherited lbledtLotNo: TLabeledEdit
      Width = 150
    end
    inherited lbledtLotID: TLabeledEdit
      Width = 150
    end
    inherited lbledtHdParameter: TLabeledEdit
      Width = 150
    end
    inherited lbledtProduct: TLabeledEdit
      Width = 150
    end
    inherited lbledtStepCode: TLabeledEdit
      Width = 150
    end
    inherited lbledtQty: TLabeledEdit
      Width = 150
    end
    inherited lbledtDate: TLabeledEdit
      Width = 150
    end
    inherited ShowTimer: TTimer
      OnTimer = frameProdInfo1ShowTimerTimer
      Top = 268
    end
  end
  object grpFTCTStatus: TGroupBox
    Left = 188
    Top = 8
    Width = 177
    Height = 57
    Caption = 'FTCT Status'
    TabOrder = 1
    object pnlOnLine: TPanel
      Left = 9
      Top = 15
      Width = 160
      Height = 30
      Caption = 'pnlOnLine'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object grpTestMode: TGroupBox
    Left = 188
    Top = 72
    Width = 177
    Height = 57
    Caption = 'Test Mode'
    TabOrder = 3
    object pnlTestMode: TPanel
      Left = 9
      Top = 16
      Width = 160
      Height = 30
      Caption = 'pnlTestMode'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object GroupBox1: TGroupBox
    Left = 188
    Top = 132
    Width = 309
    Height = 21
    Caption = 'GroupBox1'
    TabOrder = 5
    Visible = False
    object lbledtHdPass: TLabeledEdit
      Left = 8
      Top = 32
      Width = 73
      Height = 24
      EditLabel.Width = 62
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdPass'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 0
    end
    object lbledtHdFail: TLabeledEdit
      Left = 8
      Top = 72
      Width = 73
      Height = 24
      EditLabel.Width = 55
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdFail'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 3
    end
    object lbledtHdRejectATPass: TLabeledEdit
      Left = 8
      Top = 112
      Width = 73
      Height = 24
      EditLabel.Width = 107
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdRejectATPass'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 6
    end
    object lbledtHdRejectATFail: TLabeledEdit
      Left = 8
      Top = 152
      Width = 73
      Height = 24
      EditLabel.Width = 100
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdRejectATFail'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 9
    end
    object lbledtHdRejectBT: TLabeledEdit
      Left = 8
      Top = 192
      Width = 73
      Height = 24
      EditLabel.Width = 84
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdRejectBT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 12
    end
    object lbledtTesterMatch: TLabeledEdit
      Left = 120
      Top = 32
      Width = 73
      Height = 24
      EditLabel.Width = 85
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtTesterMatch'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 1
    end
    object lbledtTesterUnmatch: TLabeledEdit
      Left = 120
      Top = 72
      Width = 73
      Height = 24
      EditLabel.Width = 98
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtTesterUnmatch'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 4
    end
    object lbledtTesterIgnore: TLabeledEdit
      Left = 120
      Top = 112
      Width = 73
      Height = 24
      EditLabel.Width = 85
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtTesterIgnore'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 7
    end
    object lbledtHdPickUp: TLabeledEdit
      Left = 120
      Top = 152
      Width = 73
      Height = 24
      EditLabel.Width = 74
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdPickUp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 10
    end
    object lbledtHdPass1: TLabeledEdit
      Left = 232
      Top = 32
      Width = 73
      Height = 24
      EditLabel.Width = 68
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdPass1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 2
    end
    object lbledtHdPass2: TLabeledEdit
      Left = 232
      Top = 76
      Width = 73
      Height = 24
      EditLabel.Width = 68
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdPass2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 5
    end
    object lbledtHdRejectATPass1: TLabeledEdit
      Left = 232
      Top = 117
      Width = 73
      Height = 24
      EditLabel.Width = 113
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdRejectATPass1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 8
    end
    object lbledtHdRejectATPass2: TLabeledEdit
      Left = 232
      Top = 157
      Width = 73
      Height = 24
      EditLabel.Width = 113
      EditLabel.Height = 13
      EditLabel.Caption = 'lbledtHdRejectATPass2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 11
    end
  end
  object grpHandlerSeq: TGroupBox
    Left = 372
    Top = 72
    Width = 125
    Height = 57
    Caption = 'Handler Last seq status'
    TabOrder = 4
    object pnlHandlerSeq: TPanel
      Left = 9
      Top = 16
      Width = 84
      Height = 30
      Caption = 'pnlHandlerSeq'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 368
    Top = 8
    Width = 129
    Height = 57
    Caption = 'FTCT Last seq status'
    TabOrder = 2
    object pnlFTCTLastSeqStatus: TPanel
      Left = 9
      Top = 16
      Width = 84
      Height = 30
      Caption = 'pnlFTCTLastSeqStatus'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object grpAfter71RecvTimeOut: TGroupBox
    Left = 188
    Top = 160
    Width = 249
    Height = 73
    Caption = 'grpAfter71RecvTimeOut'
    TabOrder = 6
    Visible = False
    object grpNowCounter: TGroupBox
      Left = 8
      Top = 16
      Width = 113
      Height = 52
      Caption = 'grpNowCounter'
      TabOrder = 0
      object lblNowCounterUnit: TLabel
        Left = 76
        Top = 18
        Width = 33
        Height = 24
        Caption = 'Sec'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object pnlNowCounter: TPanel
        Left = 9
        Top = 16
        Width = 64
        Height = 30
        Caption = 'pnlNowCounter'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
    object grpSetCounter: TGroupBox
      Left = 128
      Top = 16
      Width = 113
      Height = 52
      Caption = 'grpSetCounter'
      TabOrder = 1
      object lblSetCounterUnit: TLabel
        Left = 76
        Top = 17
        Width = 33
        Height = 24
        Caption = 'Sec'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object pnlSetCounter: TPanel
        Left = 9
        Top = 16
        Width = 64
        Height = 30
        Caption = 'pnlFTCTLastSeqStatus'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
  end
  object ProcTimer: TTimer
    Interval = 100
    OnTimer = ProcTimerTimer
    Left = 180
    Top = 272
  end
end
