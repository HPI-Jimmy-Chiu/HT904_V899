object fShowBinSet: TfShowBinSet
  Left = 1212
  Top = 169
  BorderStyle = bsToolWindow
  Caption = 'BIN Setting Check'
  ClientHeight = 819
  ClientWidth = 959
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 959
    Height = 819
    ActivePage = tsSiteMappingCheck
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object tsBinSettingCheck: TTabSheet
      Caption = 'Bin'
      object pal1: TPanel
        Left = 0
        Top = 0
        Width = 89
        Height = 791
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object lab4: TLabel
          Left = -2
          Top = 630
          Width = 91
          Height = 29
          Caption = 'Bin Box'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -24
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object labAuto1: TLabel
          Left = 0
          Top = 0
          Width = 89
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
        end
        object labAuto2: TLabel
          Left = 0
          Top = 20
          Width = 89
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
        object labAuto3: TLabel
          Left = 0
          Top = 40
          Width = 89
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
        object labFix1: TLabel
          Left = 0
          Top = 120
          Width = 89
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
        object labFix2: TLabel
          Left = 0
          Top = 140
          Width = 89
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
        object labFix3: TLabel
          Left = 0
          Top = 160
          Width = 89
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
        object labFix4: TLabel
          Left = 0
          Top = 180
          Width = 89
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
        object labFix5: TLabel
          Left = 0
          Top = 200
          Width = 89
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
        object labFix6: TLabel
          Left = 0
          Top = 220
          Width = 89
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
        object Label2: TLabel
          Left = 0
          Top = 360
          Width = 89
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
        object labMag1: TLabel
          Left = 0
          Top = 380
          Width = 89
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
        object labMag2: TLabel
          Left = 0
          Top = 400
          Width = 89
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
        object labMag3: TLabel
          Left = 0
          Top = 420
          Width = 89
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
        object labMag4: TLabel
          Left = 0
          Top = 440
          Width = 89
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
        object labMag5: TLabel
          Left = 0
          Top = 460
          Width = 89
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
        object labMag6: TLabel
          Left = 0
          Top = 480
          Width = 89
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
        object labMag7: TLabel
          Left = 0
          Top = 500
          Width = 89
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
        object labMag8: TLabel
          Left = 0
          Top = 520
          Width = 89
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
        object labMag9: TLabel
          Left = 0
          Top = 540
          Width = 89
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
        object labMag10: TLabel
          Left = 0
          Top = 560
          Width = 89
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
        object labMag11: TLabel
          Left = 0
          Top = 580
          Width = 89
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
        object labMag12: TLabel
          Left = 0
          Top = 600
          Width = 89
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
        object labMag13: TLabel
          Left = 0
          Top = 620
          Width = 89
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
        object labMag14: TLabel
          Left = 0
          Top = 640
          Width = 89
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
        object labAuto4: TLabel
          Left = 0
          Top = 60
          Width = 89
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
        object labAuto5: TLabel
          Left = 0
          Top = 80
          Width = 89
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
        object labAuto6: TLabel
          Left = 0
          Top = 100
          Width = 89
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
        object labFix7: TLabel
          Left = 0
          Top = 240
          Width = 89
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
          Width = 89
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
          Width = 89
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
          Width = 89
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
          Width = 89
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
          Width = 89
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
      object ScrollBox1: TScrollBox
        Left = 89
        Top = 0
        Width = 862
        Height = 791
        Align = alClient
        AutoScroll = False
        BorderStyle = bsNone
        Color = clHighlightText
        ParentColor = False
        TabOrder = 1
        object labBinBox: TLabel
          Left = 0
          Top = 628
          Width = 48
          Height = 29
          Caption = 'Fix6'
          Color = clHighlightText
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -24
          Font.Name = 'Batang'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object pnlShowBin: TPanel
          Left = 0
          Top = 0
          Width = 862
          Height = 791
          Align = alClient
          BevelOuter = bvNone
          Caption = 'pnlShowBin'
          Color = clHighlightText
          TabOrder = 0
          object laAuto1: TLabel
            Left = 0
            Top = 0
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
            Width = 862
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
    end
    object tsSiteMappingCheck: TTabSheet
      Caption = 'Site mapping'
      ImageIndex = 1
      object palSiteMap: TPanel
        Left = 0
        Top = 0
        Width = 949
        Height = 629
        Color = clRed
        TabOrder = 0
        object Panel2: TPanel
          Left = 24
          Top = 16
          Width = 905
          Height = 601
          TabOrder = 0
          object Label1: TLabel
            Left = 8
            Top = 12
            Width = 673
            Height = 96
            Caption = 
              'Please check handler Site Mapping.'#13#10'-- Press OK if Site Mapping ' +
              'is Correct to continue running.'#13#10'-- Press Cancel if Not Correct,' +
              ' Please call Maintenance or Supervisor to correct it.'#13#10
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Panel4: TPanel
            Tag = 2
            Left = 243
            Top = 280
            Width = 130
            Height = 33
            BevelWidth = 2
            Caption = 'Cancel'
            Color = 9534289
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = Panel4Click
          end
          object Panel3: TPanel
            Tag = 1
            Left = 23
            Top = 280
            Width = 130
            Height = 33
            BevelWidth = 2
            Caption = 'OK'
            Color = 9534289
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            OnClick = Panel3Click
          end
          object GroupBox4: TGroupBox
            Left = 4
            Top = 96
            Width = 745
            Height = 177
            Align = alCustom
            Caption = 'Test Site Assign'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object labRowA: TLabel
              Left = 9
              Top = 41
              Width = 39
              Height = 17
              Caption = 'RowA'
            end
            object labRowB: TLabel
              Left = 9
              Top = 71
              Width = 40
              Height = 17
              Caption = 'RowB'
            end
            object labColA: TLabel
              Left = 90
              Top = 16
              Width = 8
              Height = 17
              Caption = 'a'
            end
            object labColB: TLabel
              Left = 176
              Top = 16
              Width = 8
              Height = 17
              Caption = 'b'
            end
            object labColC: TLabel
              Left = 262
              Top = 16
              Width = 8
              Height = 17
              Caption = 'c'
            end
            object labColD: TLabel
              Left = 347
              Top = 16
              Width = 8
              Height = 17
              Caption = 'd'
            end
            object labColE: TLabel
              Left = 432
              Top = 16
              Width = 8
              Height = 17
              Caption = 'e'
            end
            object labColF: TLabel
              Left = 517
              Top = 16
              Width = 4
              Height = 17
              Caption = 'f'
            end
            object labColG: TLabel
              Left = 603
              Top = 16
              Width = 8
              Height = 17
              Caption = 'g'
            end
            object labColH: TLabel
              Left = 688
              Top = 16
              Width = 8
              Height = 17
              Caption = 'h'
            end
            object labRowC: TLabel
              Left = 9
              Top = 102
              Width = 41
              Height = 17
              Caption = 'RowC'
            end
            object labRowD: TLabel
              Left = 9
              Top = 132
              Width = 41
              Height = 17
              Caption = 'RowD'
            end
            object palAa: TPanel
              Left = 68
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 0
            end
            object palAb: TPanel
              Left = 152
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 1
            end
            object palAc: TPanel
              Left = 236
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 2
            end
            object palAd: TPanel
              Left = 320
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 3
            end
            object palAg: TPanel
              Left = 572
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 6
            end
            object palAh: TPanel
              Left = 656
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 7
            end
            object palAf: TPanel
              Left = 488
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 5
            end
            object palAe: TPanel
              Left = 404
              Top = 36
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 4
            end
            object palBa: TPanel
              Left = 68
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 8
            end
            object palBb: TPanel
              Left = 152
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 9
            end
            object palBc: TPanel
              Left = 236
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 10
            end
            object palBd: TPanel
              Left = 320
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 11
            end
            object palBg: TPanel
              Left = 572
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 14
            end
            object palBh: TPanel
              Left = 656
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 15
            end
            object palBf: TPanel
              Left = 488
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 13
            end
            object palBe: TPanel
              Left = 404
              Top = 68
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 12
            end
            object palCa: TPanel
              Left = 68
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 16
            end
            object palCb: TPanel
              Left = 152
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 17
            end
            object palCc: TPanel
              Left = 236
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 18
            end
            object palCd: TPanel
              Left = 320
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 19
            end
            object palCg: TPanel
              Left = 572
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 22
            end
            object palCh: TPanel
              Left = 656
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 23
            end
            object palCf: TPanel
              Left = 488
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 21
            end
            object palCe: TPanel
              Left = 404
              Top = 100
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 20
            end
            object palDa: TPanel
              Left = 68
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 24
            end
            object palDb: TPanel
              Left = 152
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 25
            end
            object palDc: TPanel
              Left = 236
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 26
            end
            object palDd: TPanel
              Left = 320
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 27
            end
            object palDg: TPanel
              Left = 572
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 30
            end
            object palDh: TPanel
              Left = 656
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 31
            end
            object palDf: TPanel
              Left = 488
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 29
            end
            object palDe: TPanel
              Left = 404
              Top = 132
              Width = 71
              Height = 25
              Color = clSilver
              TabOrder = 28
            end
          end
        end
      end
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 720
    Top = 8
  end
end
