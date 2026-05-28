object frameProdInfo: TframeProdInfo
  Left = 0
  Top = 0
  Width = 303
  Height = 299
  TabOrder = 0
  object lbledtLotNo: TLabeledEdit
    Left = 8
    Top = 24
    Width = 289
    Height = 24
    EditLabel.Width = 54
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtLotNo'
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
  object lbledtLotID: TLabeledEdit
    Left = 8
    Top = 64
    Width = 289
    Height = 24
    EditLabel.Width = 51
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtLotID'
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
  object lbledtHdParameter: TLabeledEdit
    Left = 8
    Top = 104
    Width = 289
    Height = 24
    EditLabel.Width = 87
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtHdParameter'
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
  object lbledtProduct: TLabeledEdit
    Left = 8
    Top = 144
    Width = 289
    Height = 24
    EditLabel.Width = 62
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtProduct'
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
  object lbledtStepCode: TLabeledEdit
    Left = 8
    Top = 184
    Width = 289
    Height = 24
    EditLabel.Width = 72
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtStepCode'
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
  object lbledtQty: TLabeledEdit
    Left = 8
    Top = 224
    Width = 289
    Height = 24
    EditLabel.Width = 41
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtQty'
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
  object lbledtDate: TLabeledEdit
    Left = 8
    Top = 264
    Width = 289
    Height = 24
    EditLabel.Width = 48
    EditLabel.Height = 13
    EditLabel.Caption = 'lbledtDate'
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
  object ShowTimer: TTimer
    Interval = 100
    OnTimer = ShowTimerTimer
    Left = 8
    Top = 264
  end
end
