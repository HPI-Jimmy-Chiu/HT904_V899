object fTestCategory: TfTestCategory
  Left = 657
  Top = 299
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Tester Category'
  ClientHeight = 160
  ClientWidth = 258
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
  object sgArm2: TStringGrid
    Left = 0
    Top = 0
    Width = 258
    Height = 80
    TabStop = False
    Align = alClient
    ColCount = 3
    DefaultColWidth = 80
    Enabled = False
    RowCount = 3
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Batang'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 0
    OnDrawCell = sgArm1DrawCell
    RowHeights = (
      24
      24
      24)
  end
  object sgArm1: TStringGrid
    Left = 0
    Top = 80
    Width = 258
    Height = 80
    TabStop = False
    Align = alBottom
    ColCount = 3
    DefaultColWidth = 80
    Enabled = False
    RowCount = 3
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Batang'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 1
    OnDrawCell = sgArm1DrawCell
    ColWidths = (
      80
      80
      80)
  end
end
