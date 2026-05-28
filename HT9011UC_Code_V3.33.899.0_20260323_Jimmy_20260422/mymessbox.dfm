object MyMessageBox: TMyMessageBox
  Left = 432
  Top = 411
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Message'
  ClientHeight = 241
  ClientWidth = 500
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object labStopTime: TLabel
    Left = 102
    Top = 208
    Width = 244
    Height = 24
    Caption = 'Timer : 0000 days 00:00:00'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -21
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblSecsMsg: TLabel
    Left = 8
    Top = 0
    Width = 441
    Height = 49
    Alignment = taCenter
    AutoSize = False
    Caption = 'SECS GEM Message'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Visible = False
    WordWrap = True
  end
  object pnlPause: TPanel
    Left = 155
    Top = 172
    Width = 130
    Height = 33
    BevelWidth = 2
    Caption = 'Pause'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = pnlPauseClick
  end
  object pnlMain: TPanel
    Left = 24
    Top = 16
    Width = 457
    Height = 153
    BevelInner = bvLowered
    Color = 14670284
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lblSubMsg: TLabel
      Left = 7
      Top = 99
      Width = 441
      Height = 27
      Alignment = taCenter
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblMainMsg: TLabel
      Left = 8
      Top = 8
      Width = 441
      Height = 49
      Alignment = taCenter
      AutoSize = False
      Caption = 'English'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object lblChineseMsg: TLabel
      Left = 8
      Top = 64
      Width = 441
      Height = 62
      Alignment = taCenter
      AutoSize = False
      Caption = #20013#25991
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object moSecsGem: TMemo
      Left = 8
      Top = 8
      Width = 437
      Height = 141
      Color = 14670284
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object pnlNo: TPanel
    Tag = 2
    Left = 247
    Top = 172
    Width = 130
    Height = 33
    BevelWidth = 2
    Caption = 'No'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    Visible = False
    OnClick = pnlYesClick
  end
  object pnlYes: TPanel
    Tag = 1
    Left = 47
    Top = 172
    Width = 130
    Height = 33
    BevelWidth = 2
    Caption = 'Yes'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    Visible = False
    OnClick = pnlYesClick
  end
  object pnlAlarmReset: TPanel
    Left = 347
    Top = 172
    Width = 110
    Height = 33
    BevelWidth = 2
    Caption = 'AlarmReset'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    OnClick = pnlAlarmResetClick
  end
  object palWaitEAP: TPanel
    Left = 552
    Top = 776
    Width = 625
    Height = 49
    Caption = 'Waiting for EAP system judgment'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -27
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    Visible = False
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 416
    Top = 176
  end
end
