object fShowBinSelect: TfShowBinSelect
  Left = 1848
  Top = 443
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'BinSelect'
  ClientHeight = 834
  ClientWidth = 431
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
  object Label7: TLabel
    Left = 8
    Top = 8
    Width = 68
    Height = 15
    Caption = 'CUSTOMER'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 8
    Top = 40
    Width = 68
    Height = 15
    Caption = 'CUSTOMER'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 8
    Top = 72
    Width = 68
    Height = 15
    Caption = 'CUSTOMER'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label10: TLabel
    Left = 8
    Top = 104
    Width = 68
    Height = 15
    Caption = 'CUSTOMER'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label11: TLabel
    Left = 8
    Top = 136
    Width = 68
    Height = 15
    Caption = 'CUSTOMER'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 431
    Height = 834
    ActivePage = tsIndex
    Align = alClient
    MultiLine = True
    TabIndex = 4
    TabOrder = 0
    OnChange = PageControl1Change
    object tsTestBin: TTabSheet
      Caption = 'Test Bin'
      object ScrollBox1: TScrollBox
        Left = 65
        Top = 0
        Width = 358
        Height = 770
        Align = alClient
        AutoScroll = False
        BorderStyle = bsNone
        Color = clHighlightText
        ParentColor = False
        TabOrder = 1
        object pnlShowBin: TPanel
          Left = 0
          Top = 0
          Width = 360
          Height = 770
          BevelOuter = bvNone
          Caption = 'pnlShowBin'
          Color = clHighlightText
          TabOrder = 0
          object laAuto1: TLabel
            Left = 0
            Top = 0
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laAuto2: TLabel
            Left = 0
            Top = 20
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laAuto3: TLabel
            Left = 0
            Top = 40
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laBinBox: TLabel
            Left = 0
            Top = 360
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Bulk Box'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag1: TLabel
            Left = 0
            Top = 380
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag2: TLabel
            Left = 0
            Top = 400
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag3: TLabel
            Left = 0
            Top = 420
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag4: TLabel
            Left = 0
            Top = 440
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag5: TLabel
            Left = 0
            Top = 460
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag6: TLabel
            Left = 0
            Top = 480
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag7: TLabel
            Left = 0
            Top = 500
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag8: TLabel
            Left = 0
            Top = 520
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag9: TLabel
            Left = 0
            Top = 540
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag10: TLabel
            Left = 0
            Top = 560
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag11: TLabel
            Left = 0
            Top = 580
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag12: TLabel
            Left = 0
            Top = 600
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag13: TLabel
            Left = 0
            Top = 620
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 13'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laMag14: TLabel
            Left = 0
            Top = 640
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 14'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laAuto5: TLabel
            Left = 0
            Top = 80
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laAuto6: TLabel
            Left = 0
            Top = 100
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix4: TLabel
            Left = 0
            Top = 180
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix5: TLabel
            Left = 0
            Top = 200
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix6: TLabel
            Left = 0
            Top = 220
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laAuto4: TLabel
            Left = 0
            Top = 60
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix1: TLabel
            Left = 0
            Top = 120
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix2: TLabel
            Left = 0
            Top = 140
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix3: TLabel
            Left = 0
            Top = 160
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix7: TLabel
            Left = 0
            Top = 240
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix8: TLabel
            Left = 0
            Top = 260
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix9: TLabel
            Left = 0
            Top = 280
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix10: TLabel
            Left = 0
            Top = 300
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix11: TLabel
            Left = 0
            Top = 320
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laFix12: TLabel
            Left = 0
            Top = 340
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
        end
      end
      object palUnloader: TPanel
        Left = 0
        Top = 0
        Width = 65
        Height = 770
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object labAuto1: TLabel
          Left = 0
          Top = 0
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labAuto2: TLabel
          Left = 0
          Top = 20
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labAuto3: TLabel
          Left = 0
          Top = 40
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix1: TLabel
          Left = 0
          Top = 120
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix2: TLabel
          Left = 0
          Top = 140
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix3: TLabel
          Left = 0
          Top = 160
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix4: TLabel
          Left = 0
          Top = 180
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix5: TLabel
          Left = 0
          Top = 200
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix6: TLabel
          Left = 0
          Top = 220
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labBinBox: TLabel
          Left = 0
          Top = 360
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'BulkBox'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag1: TLabel
          Left = 0
          Top = 380
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag2: TLabel
          Left = 0
          Top = 400
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag3: TLabel
          Left = 0
          Top = 420
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag4: TLabel
          Left = 0
          Top = 440
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag5: TLabel
          Left = 0
          Top = 460
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag6: TLabel
          Left = 0
          Top = 480
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag7: TLabel
          Left = 0
          Top = 500
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag8: TLabel
          Left = 0
          Top = 520
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag9: TLabel
          Left = 0
          Top = 540
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag10: TLabel
          Left = 0
          Top = 560
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag11: TLabel
          Left = 0
          Top = 580
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag12: TLabel
          Left = 0
          Top = 600
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag13: TLabel
          Left = 0
          Top = 620
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 13'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labMag14: TLabel
          Left = 0
          Top = 640
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 14'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labAuto4: TLabel
          Left = 0
          Top = 60
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labAuto5: TLabel
          Left = 0
          Top = 80
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labAuto6: TLabel
          Left = 0
          Top = 100
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labFix7: TLabel
          Left = 0
          Top = 240
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labFix8: TLabel
          Left = 0
          Top = 260
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labFix9: TLabel
          Left = 0
          Top = 280
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labFix10: TLabel
          Left = 0
          Top = 300
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labFix11: TLabel
          Left = 0
          Top = 320
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labFix12: TLabel
          Left = 0
          Top = 340
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object btReturn: TButton
          Left = 0
          Top = 738
          Width = 61
          Height = 25
          Caption = 'return'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = #24494#36575#27491#40657#39636
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = btReturnClick
        end
      end
    end
    object tsCategoryInfo: TTabSheet
      Caption = 'Category Info'
      ImageIndex = 1
      object StrGrdCategory: TStringGrid
        Left = 0
        Top = 0
        Width = 423
        Height = 770
        TabStop = False
        Align = alClient
        ColCount = 3
        DefaultColWidth = 76
        DefaultRowHeight = 16
        RowCount = 16
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        RowHeights = (
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16)
      end
    end
    object Tab_UPH: TTabSheet
      Caption = 'UPH Information'
      ImageIndex = 2
      object UPH_StringGrid: TStringGrid
        Left = 0
        Top = 0
        Width = 423
        Height = 770
        TabStop = False
        Align = alClient
        ColCount = 4
        DefaultColWidth = 60
        DefaultRowHeight = 16
        FixedCols = 0
        RowCount = 14
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnDblClick = UPH_StringGridDblClick
      end
    end
    object tsUnloadMap: TTabSheet
      Caption = 'Bin Display Status'
      ImageIndex = 3
      object sbRunStatus: TStatusBar
        Left = 0
        Top = 750
        Width = 423
        Height = 20
        Panels = <
          item
            Width = 50
          end>
        SimplePanel = False
      end
      object pnlFix123: TPanel
        Left = 0
        Top = 58
        Width = 423
        Height = 116
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object gbFix1: TGroupBox
          Left = 4
          Top = 0
          Width = 81
          Height = 58
          Caption = 'Fix1'
          TabOrder = 0
          object lblFix1: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix1: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix1'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix2: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Fix2'
          TabOrder = 1
          object lblFix2: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix2: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix2'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix3: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Fix3'
          TabOrder = 2
          object lblFix3: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix3'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix3: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix3'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbBinBox: TGroupBox
          Left = 256
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Bulkbox'
          TabOrder = 3
          Visible = False
          object lblBinBox: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblBinBox'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlBinBox: TPanel
            Left = 0
            Top = 12
            Width = 79
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'BulkBox'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix4: TGroupBox
          Left = 4
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix4'
          TabOrder = 4
          object lblFix4: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix4: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix4'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix5: TGroupBox
          Left = 88
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix5'
          TabOrder = 5
          object lblFix5: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix5'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix5: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix5'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix6: TGroupBox
          Left = 172
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix6'
          TabOrder = 6
          object lblFix6: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix6'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix6: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix6'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
      object pnlAuto123: TPanel
        Left = 0
        Top = 290
        Width = 423
        Height = 58
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 3
        object gbAuto1: TGroupBox
          Left = 4
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto1'
          TabOrder = 0
          object lblAuto1: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto1: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbAuto2: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto2'
          TabOrder = 1
          object lblAuto2: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto2: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto2'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbAuto3: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto3'
          TabOrder = 2
          object lblAuto3: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto3: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto3'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
      object pnlLoad: TPanel
        Left = 0
        Top = 0
        Width = 423
        Height = 58
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object gbLoader: TGroupBox
          Left = 4
          Top = 0
          Width = 80
          Height = 47
          Caption = 'Loader'
          TabOrder = 0
          object pnlLoader: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'L'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbEmpty: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 47
          Caption = 'Empty'
          TabOrder = 1
          object pnlEmpty: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'E'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbColor: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 47
          Caption = 'Color'
          TabOrder = 2
          object pnlColor: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'C'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
      object pnlMag123: TPanel
        Left = 0
        Top = 408
        Width = 423
        Height = 290
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 5
        object gbMag1: TGroupBox
          Left = 4
          Top = 0
          Width = 81
          Height = 58
          Caption = 'Mag.1'
          TabOrder = 0
          object lblMag1: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag1: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag1'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag2: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Mag.2'
          TabOrder = 1
          object lblMag2: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag2: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag2'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag3: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Mag.3'
          TabOrder = 2
          object lblMag3: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag3'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag3: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag3'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag4: TGroupBox
          Left = 4
          Top = 58
          Width = 81
          Height = 58
          Caption = 'Mag.4'
          TabOrder = 3
          object lblMag4: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag4: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag4'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag5: TGroupBox
          Left = 88
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Mag.5'
          TabOrder = 4
          object lblMag5: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag5'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag5: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag5'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag6: TGroupBox
          Left = 172
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Mag.6'
          TabOrder = 5
          object lblMag6: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag6'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag6: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag6'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag7: TGroupBox
          Left = 4
          Top = 116
          Width = 81
          Height = 58
          Caption = 'Mag.7'
          TabOrder = 6
          object lblMag7: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag7'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag7: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag7'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag8: TGroupBox
          Left = 88
          Top = 116
          Width = 80
          Height = 58
          Caption = 'Mag.8'
          TabOrder = 7
          object lblMag8: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag8'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag8: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag8'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag9: TGroupBox
          Left = 172
          Top = 116
          Width = 80
          Height = 58
          Caption = 'Mag.9'
          TabOrder = 8
          object lblMag9: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag9'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag9: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag9'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag10: TGroupBox
          Left = 4
          Top = 174
          Width = 81
          Height = 58
          Caption = 'Mag.10'
          TabOrder = 9
          object lblMag10: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag10'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag10: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag10'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag11: TGroupBox
          Left = 88
          Top = 174
          Width = 80
          Height = 58
          Caption = 'Mag.11'
          TabOrder = 10
          object lblMag11: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag11'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag11: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag11'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag12: TGroupBox
          Left = 172
          Top = 174
          Width = 80
          Height = 58
          Caption = 'Mag.12'
          TabOrder = 11
          object lblMag12: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag12'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag12: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag12'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag13: TGroupBox
          Left = 4
          Top = 232
          Width = 81
          Height = 58
          Caption = 'Mag.13'
          TabOrder = 12
          object lblMag13: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag13'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag13: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag13'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMag14: TGroupBox
          Left = 88
          Top = 232
          Width = 80
          Height = 58
          Caption = 'Mag.14'
          TabOrder = 13
          object lblMag14: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblMag14'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlMag14: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Mag14'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
      object pnlFix789: TPanel
        Left = 0
        Top = 174
        Width = 423
        Height = 116
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
        object gbFix10: TGroupBox
          Left = 4
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix10'
          TabOrder = 3
          object lblFix10: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix10'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix10: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix10'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix11: TGroupBox
          Left = 88
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix11'
          TabOrder = 4
          object lblFix11: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix11'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix11: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix11'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix12: TGroupBox
          Left = 172
          Top = 58
          Width = 80
          Height = 58
          Caption = 'Fix12'
          TabOrder = 5
          object lblFix12: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix12'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix12: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix12'
            Color = 33023
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix9: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Fix9'
          TabOrder = 2
          object lblFix9: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix9'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix9: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix9'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix8: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Fix8'
          TabOrder = 1
          object lblFix8: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix8'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix8: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix8'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbFix7: TGroupBox
          Left = 4
          Top = 0
          Width = 81
          Height = 58
          Caption = 'Fix7'
          TabOrder = 0
          object lblFix7: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblFix7'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlFix7: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Fix7'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
      object pnlAuto456: TPanel
        Left = 0
        Top = 348
        Width = 423
        Height = 60
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 4
        object gbAuto6: TGroupBox
          Left = 172
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto6'
          TabOrder = 2
          object lblAuto6: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto6: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto6'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbAuto5: TGroupBox
          Left = 88
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto5'
          TabOrder = 1
          object lblAuto5: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto5: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto5'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbAuto4: TGroupBox
          Left = 4
          Top = 0
          Width = 80
          Height = 58
          Caption = 'Auto4'
          TabOrder = 0
          object lblAuto4: TLabel
            Left = 6
            Top = 40
            Width = 65
            Height = 13
            AutoSize = False
            Caption = 'lblAuto4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnlAuto4: TPanel
            Left = 6
            Top = 12
            Width = 65
            Height = 30
            BevelInner = bvLowered
            BevelWidth = 2
            Caption = 'Auto4'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
        end
      end
    end
    object tsIndex: TTabSheet
      Caption = 'Index'
      ImageIndex = 4
      object labIndexIn: TLabel
        Left = 7
        Top = 29
        Width = 56
        Height = 13
        Caption = 'Index  Input'
      end
      object labIndexOut: TLabel
        Left = 7
        Top = 51
        Width = 64
        Height = 13
        Caption = 'Index  Output'
      end
      object labOutArm: TLabel
        Left = 7
        Top = 73
        Width = 59
        Height = 13
        Caption = 'OutArm Pick'
      end
      object IndexInput: TLabel
        Left = 83
        Top = 28
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object IndexOut: TLabel
        Left = 83
        Top = 50
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object OutArm_input: TLabel
        Left = 83
        Top = 71
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labAutoClean: TLabel
        Left = 1
        Top = 234
        Width = 48
        Height = 13
        Caption = 'Autoclean'
        Visible = False
      end
      object AutocleanCount: TLabel
        Left = 168
        Top = 232
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
        Visible = False
      end
      object labInArm: TLabel
        Left = 7
        Top = 7
        Width = 51
        Height = 13
        Caption = 'InArm Pick'
      end
      object labInArm_input: TLabel
        Left = 83
        Top = 7
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labLowYield_1: TLabel
        Left = 127
        Top = 7
        Width = 77
        Height = 13
        Caption = 'Low Yield Count'
      end
      object labLowYield: TLabel
        Left = 219
        Top = 7
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labArmDiff_1: TLabel
        Left = 127
        Top = 23
        Width = 77
        Height = 13
        Caption = 'Arm Differ Count'
      end
      object labArmDiff: TLabel
        Left = 219
        Top = 23
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labSiteDiff_1: TLabel
        Left = 127
        Top = 39
        Width = 77
        Height = 13
        Caption = 'Site Differ Count'
      end
      object labSiteDiff: TLabel
        Left = 219
        Top = 39
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labTotalYield_1: TLabel
        Left = 127
        Top = 71
        Width = 50
        Height = 13
        Caption = 'Total Yeild'
      end
      object labTotalYield: TLabel
        Left = 219
        Top = 71
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labTotalYieldTotal_1: TLabel
        Left = 127
        Top = 55
        Width = 80
        Height = 13
        Caption = 'Total Yield(Total)'
      end
      object labTotalYieldTotal: TLabel
        Left = 219
        Top = 55
        Width = 6
        Height = 13
        Caption = '0'
        Color = clHighlightText
        ParentColor = False
      end
      object labJamrate: TLabel
        Left = 3
        Top = 247
        Width = 62
        Height = 20
        Caption = 'Jam rate'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object Jamrate: TLabel
        Left = 163
        Top = 247
        Width = 9
        Height = 20
        Caption = '0'
        Color = clHighlightText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        Visible = False
      end
      object gbAutoCleanCount: TGroupBox
        Left = 0
        Top = 108
        Width = 249
        Height = 121
        Caption = 'Auto Clean conut'
        TabOrder = 1
        object btnAutoClean: TSpeedButton
          Left = 5
          Top = 73
          Width = 120
          Height = 40
          Caption = 'Auto Clean'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Margin = 22
          ParentFont = False
          Spacing = -1
          OnClick = btnAutoCleanClick
        end
        object btnCleanReset: TSpeedButton
          Left = 129
          Top = 73
          Width = 114
          Height = 40
          Caption = 'Clean Reset'
          Enabled = False
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Margin = 22
          ParentFont = False
          Spacing = -1
          OnClick = btnCleanResetClick
        end
        object AutocleanlifeTime: TLabel
          Left = 7
          Top = 52
          Width = 225
          Height = 16
          Caption = 'AutoClean LifeTime:10000/10000'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object ed_AutoCleanCount: TEdit
          Left = 8
          Top = 16
          Width = 233
          Height = 32
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '0'
          OnClick = ed_AutoCleanCountClick
        end
      end
      object btnClearCount: TButton
        Left = 172
        Top = 88
        Width = 75
        Height = 25
        Caption = 'CLEAR'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = btnClearCountClick
      end
      object Panel1: TPanel
        Left = 0
        Top = 264
        Width = 249
        Height = 41
        TabOrder = 2
        object Label1: TLabel
          Left = 1
          Top = 4
          Width = 155
          Height = 13
          Caption = 'Interval Low Yield Alarm By Total'
        end
        object IntervalByTotalCount: TLabel
          Left = 168
          Top = 4
          Width = 6
          Height = 13
          Caption = '0'
          Color = clHighlightText
          ParentColor = False
        end
        object Label4: TLabel
          Left = 1
          Top = 20
          Width = 202
          Height = 13
          Caption = 'Interval Low Yield Alarm By Total             %'
        end
        object IntervalByTotal: TLabel
          Left = 168
          Top = 20
          Width = 6
          Height = 13
          Caption = '0'
          Color = clHighlightText
          ParentColor = False
        end
      end
      object pnlSpeciallYield: TPanel
        Left = 0
        Top = 305
        Width = 249
        Height = 41
        TabOrder = 3
        object lblSpeciallYieldTotal_1: TLabel
          Left = 7
          Top = 20
          Width = 91
          Height = 13
          Caption = 'Specail Yield(Total)'
        end
        object lblSpeciallYield_1: TLabel
          Left = 7
          Top = 4
          Width = 61
          Height = 13
          Caption = 'Specail Yeild'
        end
        object lblSpeciallYield: TLabel
          Left = 115
          Top = 8
          Width = 6
          Height = 13
          Caption = '0'
          Color = clHighlightText
          ParentColor = False
        end
        object lblSpeciallYieldTotal: TLabel
          Left = 115
          Top = 20
          Width = 6
          Height = 13
          Caption = '0'
          Color = clHighlightText
          ParentColor = False
        end
      end
      object gbTriggerAlm: TGroupBox
        Left = 0
        Top = 352
        Width = 249
        Height = 121
        TabOrder = 4
        object labTriggerAlm: TLabel
          Left = 12
          Top = 72
          Width = 54
          Height = 20
          Caption = 'OPID: '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object btnICMisPlacementAuto1: TSpeedButton
          Left = 1
          Top = 33
          Width = 64
          Height = 24
          BiDiMode = bdLeftToRight
          Caption = 'AUTO1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          Margin = 5
          ParentFont = False
          ParentBiDiMode = False
          Spacing = -1
          OnClick = btnICMisPlacementClick
        end
        object btnICMisPlacementAuto2: TSpeedButton
          Tag = 1
          Left = 92
          Top = 33
          Width = 64
          Height = 24
          BiDiMode = bdLeftToRight
          Caption = 'AUTO2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          Margin = 5
          ParentFont = False
          ParentBiDiMode = False
          Spacing = -1
          OnClick = btnICMisPlacementClick
        end
        object btnICMisPlacementAuto3: TSpeedButton
          Tag = 2
          Left = 182
          Top = 33
          Width = 64
          Height = 24
          BiDiMode = bdLeftToRight
          Caption = 'AUTO3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          Margin = 5
          ParentFont = False
          ParentBiDiMode = False
          Spacing = -1
          OnClick = btnICMisPlacementClick
        end
        object Label2: TLabel
          Left = 5
          Top = 8
          Width = 250
          Height = 19
          Caption = 'Output'#32622#26009#19981#33391'IC Misplacement     '
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object edtTriggerAlm: TEdit
          Left = 68
          Top = 71
          Width = 121
          Height = 21
          TabOrder = 0
        end
      end
      object palAutoDeviceEjection: TPanel
        Left = -1
        Top = 473
        Width = 249
        Height = 89
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial Narrow'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        object btnAutoDeviceEjectionRemove: TSpeedButton
          Left = 9
          Top = 41
          Width = 81
          Height = 40
          Caption = #36864#30436
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Margin = 22
          ParentFont = False
          Spacing = -1
        end
        object Label5: TLabel
          Left = 72
          Top = 12
          Width = 121
          Height = 16
          AutoSize = False
          Caption = #33258#21205#28165#27231#21151#33021
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object btnAutoDeviceEjection: TSpeedButton
          Left = 154
          Top = 40
          Width = 81
          Height = 40
          Caption = #28165#27231
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Margin = 22
          ParentFont = False
          Spacing = -1
        end
      end
      object gbCopyRecipe: TGroupBox
        Left = 0
        Top = 560
        Width = 249
        Height = 65
        Caption = 'Copy Recipe'
        TabOrder = 6
        object sbCopyRecipe: TSpeedButton
          Left = 5
          Top = 17
          Width = 116
          Height = 40
          Caption = 'Copy Recipe'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          Margin = 5
          ParentFont = False
          Spacing = -1
          OnClick = sbCopyRecipeClick
        end
      end
    end
    object tsASE: TTabSheet
      Caption = 'ASE'
      ImageIndex = 5
      object labScheduleNAME_1: TLabel
        Left = 8
        Top = 32
        Width = 48
        Height = 13
        Caption = 'Schedule:'
      end
      object labInQty_1: TLabel
        Left = 8
        Top = 72
        Width = 34
        Height = 13
        Caption = 'InQty : '
      end
      object labScheduleNAME: TLabel
        Left = 72
        Top = 24
        Width = 257
        Height = 29
        AutoSize = False
        Color = clHighlightText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
      end
      object labInQty: TLabel
        Left = 72
        Top = 64
        Width = 257
        Height = 29
        AutoSize = False
        Color = clHighlightText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
      end
      object BulkBox: TPanel
        Left = 2
        Top = 132
        Width = 257
        Height = 81
        TabOrder = 1
        object LabBulkBox: TLabel
          Left = 115
          Top = 59
          Width = 49
          Height = 13
          Caption = 'BulkCount'
        end
        object LabErrorBinNowCount: TLabel
          Left = 185
          Top = 59
          Width = 6
          Height = 13
          Caption = '0'
        end
        object Label3: TLabel
          Left = 116
          Top = 17
          Width = 57
          Height = 13
          Caption = 'Alarm Count'
        end
        object rg_FixBinBox: TRadioGroup
          Left = 1
          Top = -1
          Width = 109
          Height = 80
          Hint = 'Module_FixBinBox Switch'
          Caption = 'Fix BinBox Switch'
          Enabled = False
          ItemIndex = 0
          Items.Strings = (
            '[1] Normal'
            '[2] BinBox')
          TabOrder = 0
        end
        object ed_FixBinBoxAlarmCount: TEdit
          Left = 181
          Top = 13
          Width = 55
          Height = 21
          Hint = 'Module_FixBinBox Alarm Count'
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
      end
      object PLoadInput: TPanel
        Left = 14
        Top = 120
        Width = 259
        Height = 80
        TabOrder = 0
        object LoadKeyIn: TRadioGroup
          Left = -3
          Top = -1
          Width = 185
          Height = 44
          Caption = 'Load input count'
          Columns = 2
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = #27161#26999#39636
          Font.Style = [fsBold]
          ItemIndex = 0
          Items.Strings = (
            '100%'
            'Input')
          ParentFont = False
          TabOrder = 0
        end
        object EdLoadCount: TEdit
          Left = 16
          Top = 48
          Width = 121
          Height = 21
          TabOrder = 1
          Text = '0'
          OnClick = EdLoadCountClick
        end
        object btnSetInpputCnt: TButton
          Left = 160
          Top = 48
          Width = 75
          Height = 25
          Caption = 'Set'
          TabOrder = 2
          OnClick = btnSetInpputCntClick
        end
      end
    end
    object PageControl1_ART: TTabSheet
      Caption = 'Category Info ART'
      ImageIndex = 6
      object StrGrdCategoryART: TStringGrid
        Left = 0
        Top = 0
        Width = 423
        Height = 770
        TabStop = False
        Align = alClient
        ColCount = 3
        DefaultColWidth = 76
        DefaultRowHeight = 16
        RowCount = 16
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        RowHeights = (
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16)
      end
    end
    object Tab_ARTSkipICCount: TTabSheet
      Caption = 'ART Skip IC Count'
      ImageIndex = 7
      object StrARTSkipICCount: TStringGrid
        Left = 0
        Top = 0
        Width = 423
        Height = 770
        TabStop = False
        Align = alClient
        ColCount = 3
        DefaultColWidth = 76
        DefaultRowHeight = 16
        RowCount = 16
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        RowHeights = (
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16)
      end
    end
    object tsARTNormalBin: TTabSheet
      Caption = 'ART normal bin'
      ImageIndex = 9
      object ScrollBox2: TScrollBox
        Left = 65
        Top = 0
        Width = 358
        Height = 770
        Align = alClient
        AutoScroll = False
        BorderStyle = bsNone
        Color = clHighlightText
        ParentColor = False
        TabOrder = 1
        object pnlShowBin_ARTFT: TPanel
          Left = 0
          Top = 0
          Width = 360
          Height = 770
          BevelOuter = bvNone
          Color = clHighlightText
          TabOrder = 0
          object laArtFtAuto1: TLabel
            Left = 0
            Top = 0
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtAuto2: TLabel
            Left = 0
            Top = 20
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtAuto3: TLabel
            Left = 0
            Top = 40
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtBinBox: TLabel
            Left = 0
            Top = 360
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Bulk Box'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag1: TLabel
            Left = 0
            Top = 380
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag2: TLabel
            Left = 0
            Top = 400
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag3: TLabel
            Left = 0
            Top = 420
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag4: TLabel
            Left = 0
            Top = 440
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag5: TLabel
            Left = 0
            Top = 460
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag6: TLabel
            Left = 0
            Top = 480
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag7: TLabel
            Left = 0
            Top = 500
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag8: TLabel
            Left = 0
            Top = 520
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag9: TLabel
            Left = 0
            Top = 540
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag10: TLabel
            Left = 0
            Top = 560
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag11: TLabel
            Left = 0
            Top = 580
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag12: TLabel
            Left = 0
            Top = 600
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag13: TLabel
            Left = 0
            Top = 620
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 13'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtMag14: TLabel
            Left = 0
            Top = 640
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 14'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtAuto5: TLabel
            Left = 0
            Top = 80
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtAuto6: TLabel
            Left = 0
            Top = 100
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix4: TLabel
            Left = 0
            Top = 180
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix5: TLabel
            Left = 0
            Top = 200
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix6: TLabel
            Left = 0
            Top = 220
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtAuto4: TLabel
            Left = 0
            Top = 60
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix1: TLabel
            Left = 0
            Top = 120
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix2: TLabel
            Left = 0
            Top = 140
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix3: TLabel
            Left = 0
            Top = 160
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix7: TLabel
            Left = 0
            Top = 240
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix8: TLabel
            Left = 0
            Top = 260
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix9: TLabel
            Left = 0
            Top = 280
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix10: TLabel
            Left = 0
            Top = 300
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix11: TLabel
            Left = 0
            Top = 320
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtFtFix12: TLabel
            Left = 0
            Top = 340
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
        end
      end
      object palARTNor: TPanel
        Left = 0
        Top = 0
        Width = 65
        Height = 770
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object labArtFtAuto1: TLabel
          Left = 0
          Top = 0
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labArtFtAuto2: TLabel
          Left = 0
          Top = 20
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtAuto3: TLabel
          Left = 0
          Top = 40
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix1: TLabel
          Left = 0
          Top = 120
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix2: TLabel
          Left = 0
          Top = 140
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix3: TLabel
          Left = 0
          Top = 160
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix4: TLabel
          Left = 0
          Top = 180
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix5: TLabel
          Left = 0
          Top = 200
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix6: TLabel
          Left = 0
          Top = 220
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtBinBox: TLabel
          Left = 0
          Top = 360
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'BulkBox'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag1: TLabel
          Left = 0
          Top = 380
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag2: TLabel
          Left = 0
          Top = 400
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag3: TLabel
          Left = 0
          Top = 420
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag4: TLabel
          Left = 0
          Top = 440
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag5: TLabel
          Left = 0
          Top = 460
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag6: TLabel
          Left = 0
          Top = 480
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag7: TLabel
          Left = 0
          Top = 500
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag8: TLabel
          Left = 0
          Top = 520
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag9: TLabel
          Left = 0
          Top = 540
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag10: TLabel
          Left = 0
          Top = 560
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag11: TLabel
          Left = 0
          Top = 580
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag12: TLabel
          Left = 0
          Top = 600
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag13: TLabel
          Left = 0
          Top = 620
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 13'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtMag14: TLabel
          Left = 0
          Top = 640
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 14'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtAuto4: TLabel
          Left = 0
          Top = 60
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtAuto5: TLabel
          Left = 0
          Top = 80
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtAuto6: TLabel
          Left = 0
          Top = 100
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix7: TLabel
          Left = 0
          Top = 240
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix8: TLabel
          Left = 0
          Top = 260
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix9: TLabel
          Left = 0
          Top = 280
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix10: TLabel
          Left = 0
          Top = 300
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix11: TLabel
          Left = 0
          Top = 320
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtFtFix12: TLabel
          Left = 0
          Top = 340
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
      end
    end
    object tsARTRTBin: TTabSheet
      Caption = 'ART RT Bin'
      ImageIndex = 10
      object ScrollBox3: TScrollBox
        Left = 65
        Top = 0
        Width = 358
        Height = 770
        Align = alClient
        AutoScroll = False
        BorderStyle = bsNone
        Color = clHighlightText
        ParentColor = False
        TabOrder = 1
        object pnlShowBin_ARTRT: TPanel
          Left = 0
          Top = 0
          Width = 360
          Height = 770
          BevelOuter = bvNone
          Caption = 'pnlShowBin_ARTRT'
          Color = clHighlightText
          TabOrder = 0
          object laArtRtAuto1: TLabel
            Left = 0
            Top = 0
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtAuto2: TLabel
            Left = 0
            Top = 20
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtAuto3: TLabel
            Left = 0
            Top = 40
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtBinBox: TLabel
            Left = 0
            Top = 360
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Bulk Box'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag1: TLabel
            Left = 0
            Top = 380
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag2: TLabel
            Left = 0
            Top = 400
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag3: TLabel
            Left = 0
            Top = 420
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag4: TLabel
            Left = 0
            Top = 440
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag5: TLabel
            Left = 0
            Top = 460
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag6: TLabel
            Left = 0
            Top = 480
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag7: TLabel
            Left = 0
            Top = 500
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag8: TLabel
            Left = 0
            Top = 520
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag9: TLabel
            Left = 0
            Top = 540
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag10: TLabel
            Left = 0
            Top = 560
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag11: TLabel
            Left = 0
            Top = 580
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag12: TLabel
            Left = 0
            Top = 600
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag13: TLabel
            Left = 0
            Top = 620
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 13'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtMag14: TLabel
            Left = 0
            Top = 640
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Magazine 14'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtAuto5: TLabel
            Left = 0
            Top = 80
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtAuto6: TLabel
            Left = 0
            Top = 100
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix4: TLabel
            Left = 0
            Top = 180
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix5: TLabel
            Left = 0
            Top = 200
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 5'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix6: TLabel
            Left = 0
            Top = 220
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 6'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtAuto4: TLabel
            Left = 0
            Top = 60
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Auto 4'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix1: TLabel
            Left = 0
            Top = 120
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 1'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix2: TLabel
            Left = 0
            Top = 140
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 2'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix3: TLabel
            Left = 0
            Top = 160
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 3'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix7: TLabel
            Left = 0
            Top = 240
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 7'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix8: TLabel
            Left = 0
            Top = 260
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 8'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix9: TLabel
            Left = 0
            Top = 280
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 9'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix10: TLabel
            Left = 0
            Top = 300
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 10'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix11: TLabel
            Left = 0
            Top = 320
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 11'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object laArtRtFix12: TLabel
            Left = 0
            Top = 340
            Width = 360
            Height = 20
            Align = alTop
            AutoSize = False
            Caption = 'Fix 12'
            Color = clHighlightText
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Batang'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
        end
      end
      object palARTRT: TPanel
        Left = 0
        Top = 0
        Width = 65
        Height = 770
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object labArtRtAuto1: TLabel
          Left = 0
          Top = 0
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          OnClick = labAuto1Click
        end
        object labArtRtAuto2: TLabel
          Left = 0
          Top = 20
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtAuto3: TLabel
          Left = 0
          Top = 40
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix1: TLabel
          Left = 0
          Top = 120
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix2: TLabel
          Left = 0
          Top = 140
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix3: TLabel
          Left = 0
          Top = 160
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix4: TLabel
          Left = 0
          Top = 180
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix5: TLabel
          Left = 0
          Top = 200
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix6: TLabel
          Left = 0
          Top = 220
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtBinBox: TLabel
          Left = 0
          Top = 360
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'BulkBox'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag1: TLabel
          Left = 0
          Top = 380
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag2: TLabel
          Left = 0
          Top = 400
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag3: TLabel
          Left = 0
          Top = 420
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag4: TLabel
          Left = 0
          Top = 440
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag5: TLabel
          Left = 0
          Top = 460
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag6: TLabel
          Left = 0
          Top = 480
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag7: TLabel
          Left = 0
          Top = 500
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag8: TLabel
          Left = 0
          Top = 520
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag9: TLabel
          Left = 0
          Top = 540
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag10: TLabel
          Left = 0
          Top = 560
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag11: TLabel
          Left = 0
          Top = 580
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag12: TLabel
          Left = 0
          Top = 600
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag13: TLabel
          Left = 0
          Top = 620
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 13'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtMag14: TLabel
          Left = 0
          Top = 640
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Mag 14'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtAuto4: TLabel
          Left = 0
          Top = 60
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtAuto5: TLabel
          Left = 0
          Top = 80
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtAuto6: TLabel
          Left = 0
          Top = 100
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Auto 6'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix7: TLabel
          Left = 0
          Top = 240
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 7'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix8: TLabel
          Left = 0
          Top = 260
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 8'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix9: TLabel
          Left = 0
          Top = 280
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 9'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix10: TLabel
          Left = 0
          Top = 300
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 10'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix11: TLabel
          Left = 0
          Top = 320
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 11'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
        object labArtRtFix12: TLabel
          Left = 0
          Top = 340
          Width = 65
          Height = 20
          Align = alTop
          AutoSize = False
          Caption = 'Fix 12'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
      end
    end
    object tsFxiAI: TTabSheet
      Caption = 'AI'
      ImageIndex = 9
      object edAiAuto1: TLabeledEdit
        Left = 80
        Top = 8
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 1'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        EditLabel.Layout = tlCenter
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 0
      end
      object edAiAuto2: TLabeledEdit
        Left = 80
        Top = 31
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 2'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 1
      end
      object edAiAuto3: TLabeledEdit
        Left = 80
        Top = 54
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 3'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 2
      end
      object edAiAuto4: TLabeledEdit
        Left = 80
        Top = 77
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 4'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 3
      end
      object edAiAuto5: TLabeledEdit
        Left = 80
        Top = 100
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 5'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 4
      end
      object edAiAuto6: TLabeledEdit
        Left = 80
        Top = 123
        Width = 121
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 20
        EditLabel.Caption = 'Auto 6'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 5
      end
      object edlAiFix1: TLabeledEdit
        Left = 80
        Top = 146
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 1'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 6
      end
      object edlAiFix2: TLabeledEdit
        Left = 80
        Top = 169
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 2'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 7
      end
      object edlAiFix3: TLabeledEdit
        Left = 80
        Top = 192
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 3'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 8
      end
      object edlAiFix4: TLabeledEdit
        Left = 80
        Top = 215
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 4'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 9
      end
      object edlAiFix5: TLabeledEdit
        Left = 80
        Top = 238
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 5'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 10
      end
      object edlAiFix6: TLabeledEdit
        Left = 80
        Top = 261
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 6'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 11
      end
      object edlAiFix7: TLabeledEdit
        Left = 80
        Top = 284
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 7'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 12
      end
      object edlAiFix8: TLabeledEdit
        Left = 80
        Top = 307
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 8'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 13
      end
      object edlAiFix9: TLabeledEdit
        Left = 80
        Top = 330
        Width = 121
        Height = 21
        EditLabel.Width = 33
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 9'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 14
      end
      object edlAiFix10: TLabeledEdit
        Left = 80
        Top = 353
        Width = 121
        Height = 21
        EditLabel.Width = 42
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 10'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 15
      end
      object edlAiFix11: TLabeledEdit
        Left = 80
        Top = 376
        Width = 121
        Height = 21
        EditLabel.Width = 42
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 11'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 16
      end
      object edlAiFix12: TLabeledEdit
        Left = 80
        Top = 399
        Width = 121
        Height = 21
        EditLabel.Width = 42
        EditLabel.Height = 20
        EditLabel.Caption = 'Fix 12'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 17
      end
      object edlAiMag1: TLabeledEdit
        Left = 80
        Top = 445
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 1'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 19
      end
      object edlAiMag2: TLabeledEdit
        Left = 80
        Top = 468
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 2'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 20
      end
      object edlAiMag3: TLabeledEdit
        Left = 80
        Top = 491
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 3'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 21
      end
      object edlAiMag4: TLabeledEdit
        Left = 80
        Top = 514
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 4'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 22
      end
      object edlAiMag5: TLabeledEdit
        Left = 80
        Top = 537
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 5'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 23
      end
      object edlAiMag6: TLabeledEdit
        Left = 80
        Top = 560
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 6'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 24
      end
      object edlAiMag7: TLabeledEdit
        Left = 80
        Top = 583
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 7'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 25
      end
      object edlAiMag8: TLabeledEdit
        Left = 80
        Top = 606
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 8'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 26
      end
      object edlAiMag9: TLabeledEdit
        Left = 80
        Top = 629
        Width = 121
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 9'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 27
      end
      object edlAiMag10: TLabeledEdit
        Left = 80
        Top = 652
        Width = 121
        Height = 21
        EditLabel.Width = 53
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 10'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 28
      end
      object edlAiMag11: TLabeledEdit
        Left = 80
        Top = 675
        Width = 121
        Height = 21
        EditLabel.Width = 53
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 11'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 29
      end
      object edlAiMag12: TLabeledEdit
        Left = 80
        Top = 698
        Width = 121
        Height = 21
        EditLabel.Width = 53
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 12'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 30
      end
      object edlAiMag13: TLabeledEdit
        Left = 80
        Top = 721
        Width = 121
        Height = 21
        EditLabel.Width = 53
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 13'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 31
      end
      object edlAiMag14: TLabeledEdit
        Left = 80
        Top = 744
        Width = 121
        Height = 21
        EditLabel.Width = 53
        EditLabel.Height = 20
        EditLabel.Caption = 'Mag 14'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 32
      end
      object edlAiBinBox: TLabeledEdit
        Left = 80
        Top = 422
        Width = 121
        Height = 21
        EditLabel.Width = 62
        EditLabel.Height = 20
        EditLabel.Caption = 'Bulk Box'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -16
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 18
      end
    end
    object tsCategoryInfoContCT: TTabSheet
      Caption = 'tsCategoryInfoContCT'
      ImageIndex = 12
      object StrGrdCategoryContCT: TStringGrid
        Left = 0
        Top = 0
        Width = 423
        Height = 770
        TabStop = False
        Align = alClient
        ColCount = 3
        DefaultColWidth = 76
        DefaultRowHeight = 16
        RowCount = 16
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        RowHeights = (
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16
          16)
      end
    end
  end
  object TimerAutoCleanCount: TTimer
    Enabled = False
    OnTimer = TimerAutoCleanCountTimer
    Left = 324
    Top = 62
  end
end
