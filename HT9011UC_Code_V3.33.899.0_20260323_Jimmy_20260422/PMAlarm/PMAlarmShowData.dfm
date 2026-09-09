object fPMAlarm_ShowData: TfPMAlarm_ShowData
  Left = 598
  Top = 208
  Width = 978
  Height = 625
  Caption = 'PM Alarm Show Data'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object sg_PMAlarmShowData: TStringGrid
    Left = 8
    Top = 72
    Width = 945
    Height = 505
    ColCount = 50
    DefaultColWidth = 80
    RowCount = 50
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 3
    ColWidths = (
      80
      267
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80
      80)
  end
  object cb_PMAlarmChoiseFile: TComboBox
    Left = 208
    Top = 32
    Width = 337
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 20
    ParentFont = False
    TabOrder = 2
    Text = 'cb_PMAlarmChoiseFile'
    OnChange = cb_PMAlarmChoiseFileChange
  end
  object pnlPMAlarmBeforeDays: TPanel
    Left = 32
    Top = 32
    Width = 169
    Height = 28
    BevelOuter = bvNone
    Caption = 'Choise Report File'
    Color = 383181
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object pnlPMAlarm_Title: TPanel
    Left = 0
    Top = 0
    Width = 970
    Height = 28
    Align = alTop
    BevelOuter = bvNone
    Caption = 'Report View'
    Color = clInactiveCaption
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
  end
end
