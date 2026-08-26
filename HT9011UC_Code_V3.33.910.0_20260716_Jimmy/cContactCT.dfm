object fContactCT: TfContactCT
  Left = 1040
  Top = 211
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'Contact Counter Kinds'
  ClientHeight = 289
  ClientWidth = 361
  Color = clBtnFace
  DragKind = dkDock
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
  object rgYieldType: TRadioGroup
    Left = 0
    Top = 33
    Width = 361
    Height = 88
    Align = alTop
    Columns = 3
    DragKind = dkDock
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ItemIndex = 3
    Items.Strings = (
      'History'
      'Total'
      'Kind'
      'Kind(%)'
      'ByHeadYield'
      'ByBinPass'
      'ByBinPass(%)'
      'ByBinArmPass'
      'ByBinArmPass(%)'
      'ByBinSitePass'
      'ByBinSitePass(%)')
    ParentFont = False
    TabOrder = 1
    OnClick = rgYieldTypeClick
  end
  object sgYield: TStringGrid
    Left = 0
    Top = 121
    Width = 361
    Height = 168
    TabStop = False
    Align = alClient
    ColCount = 4
    DefaultColWidth = 70
    DefaultRowHeight = 16
    DragKind = dkDock
    RowCount = 9
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Batang'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 2
    OnDblClick = sgYieldDblClick
    OnDrawCell = sgYieldDrawCell
    OnMouseDown = sgYieldMouseDown
    ColWidths = (
      70
      69
      70
      70)
  end
  object palClearCnt: TPanel
    Left = 0
    Top = 0
    Width = 361
    Height = 33
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object btClearCount: TButton
      Left = 0
      Top = 0
      Width = 141
      Height = 29
      Caption = 'Count Clear'
      DragKind = dkDock
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = btClearCountClick
    end
    object btYieldChart: TButton
      Left = 144
      Top = 0
      Width = 141
      Height = 29
      Caption = 'Yield Chart'
      DragKind = dkDock
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btYieldChartClick
    end
  end
end
