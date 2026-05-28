object fDefrostNote: TfDefrostNote
  Left = 688
  Top = 196
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'Defrost Note'
  ClientHeight = 198
  ClientWidth = 365
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
  object Panel11: TPanel
    Left = 0
    Top = 0
    Width = 357
    Height = 197
    Color = 12761254
    TabOrder = 0
    object pnl_ShowMessage: TPanel
      Left = 1
      Top = 1
      Width = 355
      Height = 132
      Align = alTop
      Caption = 'Defrost Time Has Arrived'
      Color = clSilver
      Font.Charset = ANSI_CHARSET
      Font.Color = clRed
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object btn_Start: TButton
      Left = 100
      Top = 144
      Width = 85
      Height = 37
      Caption = 'Start'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btn_StartClick
    end
    object btn_Interrupt: TButton
      Left = 192
      Top = 144
      Width = 85
      Height = 37
      Caption = 'Interrupt '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = btn_InterruptClick
    end
  end
  object Timer1: TTimer
    Interval = 500
    OnTimer = Timer1Timer
    Left = 320
    Top = 156
  end
end
