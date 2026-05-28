object MemoryAlarmForm: TMemoryAlarmForm
  Left = 701
  Top = 295
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Memory Alarm'
  ClientHeight = 160
  ClientWidth = 754
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMemoryAlarm: TPanel
    Left = 0
    Top = 0
    Width = 754
    Height = 160
    Align = alClient
    Color = 1384132
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lbMemoryAlarm: TLabel
      Left = 1
      Top = 1
      Width = 752
      Height = 158
      Align = alClient
      AutoSize = False
      Caption = 'Memory Alarm'
      Layout = tlCenter
    end
  end
end
