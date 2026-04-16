object fSecurity: TfSecurity
  Left = 66
  Top = 40
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Password and Security'
  ClientHeight = 871
  ClientWidth = 874
  Color = 12761254
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
  object Panel13: TPanel
    Left = 0
    Top = 0
    Width = 874
    Height = 31
    Align = alTop
    BevelInner = bvLowered
    Caption = 'Add And Delete User'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 31
    Width = 874
    Height = 53
    Align = alTop
    BevelInner = bvLowered
    Color = 12761254
    TabOrder = 1
    object sbSupervisor: TSpeedButton
      Left = 67
      Top = 11
      Width = 186
      Height = 33
      Caption = 'Supervisor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = sbSupervisorClick
    end
    object sbEngineer: TSpeedButton
      Left = 497
      Top = 11
      Width = 186
      Height = 33
      Caption = 'Engineer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = sbEngineerClick
    end
    object btnHonPrec: TSpeedButton
      Left = 282
      Top = 11
      Width = 186
      Height = 33
      Caption = 'HonPrec'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = btnHonPrecClick
    end
    object btnOperator: TSpeedButton
      Left = 645
      Top = 11
      Width = 186
      Height = 33
      Caption = 'Operator'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = btnOperatorClick
    end
  end
  object Panel17: TPanel
    Left = 0
    Top = 84
    Width = 874
    Height = 31
    Align = alTop
    BevelInner = bvLowered
    Caption = 'Operate level setup'
    Color = 9534289
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object SecurityExit: TPanel
    Left = 0
    Top = 830
    Width = 874
    Height = 41
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = SecurityExitClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 115
    Width = 874
    Height = 715
    ActivePage = tsJamCode
    Align = alClient
    TabIndex = 10
    TabOrder = 3
    TabWidth = 75
    object tsMain: TTabSheet
      Caption = 'Main'
      object sbMain: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 601
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsTools: TTabSheet
      Caption = 'Tools'
      ImageIndex = 1
      object sbTools: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 601
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsConfig: TTabSheet
      Caption = 'Config'
      ImageIndex = 2
      object sbConfig: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 601
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsContact: TTabSheet
      Caption = 'Contact'
      ImageIndex = 3
      object sbContact: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 601
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsSetup: TTabSheet
      Caption = 'Setup'
      ImageIndex = 4
      object sbSetup: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 490
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsTemp: TTabSheet
      Caption = 'Temp. Offset'
      ImageIndex = 5
      object sbTemp: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 490
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsIo: TTabSheet
      Caption = 'IO'
      ImageIndex = 6
      object sbIO: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 490
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsConfiguration: TTabSheet
      Caption = 'Configuration'
      ImageIndex = 7
      object sbConfiguration: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 490
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsYield: TTabSheet
      Caption = 'Yield'
      ImageIndex = 8
      object sbYield: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 490
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsOther: TTabSheet
      Caption = 'Other'
      ImageIndex = 9
      object sbOther: TScrollBox
        Left = 0
        Top = 0
        Width = 841
        Height = 554
        Align = alClient
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsJamCode: TTabSheet
      Caption = 'Jam Code'
      ImageIndex = 10
      object labJamArea: TLabel
        Left = 4
        Top = 0
        Width = 47
        Height = 20
        Caption = 'Area : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object labJamCode: TLabel
        Left = 4
        Top = 36
        Width = 52
        Height = 20
        Caption = 'Code : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 4
        Top = 72
        Width = 50
        Height = 20
        Caption = 'Level : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object labLang: TLabel
        Left = 0
        Top = 189
        Width = 87
        Height = 20
        Caption = 'Language : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
      end
      object spbExport: TSpeedButton
        Left = 687
        Top = 317
        Width = 143
        Height = 55
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Export'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          FA0E0000424DFA0E000000000000360000002800000024000000230000000100
          180000000000C40E000001000000010000000000000000000000EFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEEEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEEEFEFEEEFEFEEEFEFEEEFEFEEEFEFEEEFEFEEEFEFEEEFEFEE
          EBEBEAEBEBEAEBEBEAE6E6E6E6E6E6EBEBEAEFEFEEEFEFEEDDDDDDE6E6E6EBEB
          EAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE6E6E6DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDD6D6D6D6D6D6CCCCCCCCCCCCCCCCCCD6D6D6E6E6
          E600833CE6E6E6D6D6D6DDDDDDEBEBEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEBEBEADDDDDDD6D6D6CCCCCC
          CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC5C5C5BDBDBDB6B6
          B6B6B6B6B6B6B6B6B6B6D6D6D600833C00833CDDDDDDD6D6D6DDDDDDEBEBEAEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEBEBEA
          952E7C93297A93297A93297A8E29758E29758E297585216D85216D85216D8521
          6DA66396A663969D5F8D965D89965D89965D89965D89CCCCCC00833C00BE5800
          833CDDDDDDD6D6D6DDDDDDEBEBEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFE6E6E6DDDDDD9D3083EBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEB
          EAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEA00833C00833C00833C00833C00833C00
          833C00833C00833C00BE5800BE5800833CDDDDDDD6D6D6DDDDDDEFEFEEEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFDDDDDDD6D6D6A33888EBEBEAEBEB
          EAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEA00833C79
          E6AA00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00833C
          DDDDDDD6D6D6E6E6E6EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF952E7C952E
          7C8E2975A33888EBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEB
          EBEAEBEBEAEBEBEA00833C79E6AA00C45A00C45A00C45A00C45A00C45A00C45A
          00C45A00C45A00C45A00C45A00833CE6E6E6EBEBEAEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEF9D3083EBEBEAE6E6E6A33888EBEBEAEBEBEAEBEBEAEBEBEAEB
          EBEAEBEBEAEBEBEAEBEBEAEBEBEAEBEBEAEFEFEE00833C79E6AA00C45A00C45A
          00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00833CF7F7
          F7EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA33888EBEBEAE6E6E6A6398CEF
          EFEEEFEFEEEFEFEEC5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDCCCCCC
          00833C79E6AA00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C4
          5A00C45A008B40FBFBFAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA3
          3888EBEBEAE6E6E6AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2
          F3F2F2F3F2F2F3F2F2F3F2F200833C79E6AA79E6AA79E6AA79E6AA79E6AA79E6
          AA79E6AA00C45A00C45A00C45A008B40F7F7F7EFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFA33888EBEBEAE6E6E6A6398CEFEFEEEFEFEEEFEFEE
          C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDCCCCCC00833C00833C0083
          3C00833C00833C00833C00833C00833C00C45A00C45A008B40FBFBFAEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA6398CEFEFEEEBEBEA
          AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2
          F2F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7AF689DE6E6E600833C00C45A00
          8B40FBFBFAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFAE4193F3F2F2EBEBEAB8489CF3F2F2F3F2F2F3F2F2C5C5C5C5C5C5C5C5
          C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDB6B6B6F3F2F2F3F2F2F3F2F293
          297AE6E6E600833C00833CFBFBFAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA6398CEFEFEEEBEBEABC4C9FF3F2F2F3F2
          F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3
          F2F2F3F2F2F3F2F2F3F2F2952E7CE6E6E600833CFBFBFAEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFAE4193F3F2
          F2EBEBEAC452A7F7F7F7F7F7F7F7F7F7C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5
          C5C5BDBDBDBDBDBDBDBDBDBDBDBDF7F7F7F7F7F7F7F7F79D3083E6E6E6F3F2F2
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFB8489CF3F2F2EBEBEAC956ABF7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7A33888DDDDDDEFEFEEEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFBC4C9FF3F2F2EFEFEECD58B0FB
          FBFAFBFBFAFBFBFAD461B6CD58B0C452A7B8489CAE4193AE4193A338889D3083
          93297A8C2373FBFBFAFBFBFAFBFBFAA6398CDDDDDDEFEFEEEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFC4
          52A7F7F7F7F3F2F2D461B6FBFBFAFBFBFAFBFBFADD6CBEEB89D0EB89D0E982CE
          E87DCDE87DCDE87DCDE87DCDE87DCD9D3083FBFBFAFBFBFAFBFBFAAE4193DDDD
          DDEFEFEEEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFC956ABF7F7F7F3F2F2DC65BDFBFBFAFBFBFAFBFBFA
          E56CC6ED8AD3ED8AD3E982CEE982CEE982CEE87DCDE87DCDE87DCD9D3083FBFB
          FAFBFBFAFBFBFAB8489CDDDDDDEFEFEEEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFCD58B0FBFBFAF3F2F2
          DC65BDFFFFFFFFFFFFFFFFFFE56CC6E56CC6D461B6CD58B0C452A7C452A7B848
          9CAE4193A6398C9D3083FFFFFFFFFFFFFFFFFFBD4DA1DDDDDDEFEFEEEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFD461B6FBFBFAF3F2F2E56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4
          52A7DDDDDDEFEFEEEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFDC65BDFBFBFAF7F7F7E56CC6FFFFFFFFFF
          FFFFFFFFC5C5C5C5C5C5C5C5C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBD
          BDBDFFFFFFFFFFFFFFFFFFC956ABDDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFDC65BDFFFF
          FFFBFBFAE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E6E6E6EFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFD15BB2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFE56CC6E5
          6CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6
          DC65BDDC65BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE5
          6CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E6E6E6EFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15B
          B2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6E56CC6E56CC6
          E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6DC65BDDC65
          BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF}
        Margin = 2
        ParentFont = False
        OnClick = spbExportClick
      end
      object spbImport: TSpeedButton
        Left = 535
        Top = 317
        Width = 141
        Height = 55
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Import'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          FA0E0000424DFA0E000000000000360000002800000024000000230000000100
          180000000000C40E000001000000010000000000000000000000EFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFECECEAECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFECECEAECECEAE5E5E5DDDDDDE5E5E5ECECEAECECEAEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE5E5E5E3E3E1DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD7D7D7CCCCCCD7D7D70083
          3CDDDDDDDDDDDDECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFECECEADDDDDDD7D7D7CCCCCC
          CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC5C5
          C5BDBDBDCCCCCC00833C00833CD7D7D7D7D7D7DDDDDDDDDDDDDDDDDDDDDDDDE3
          E3E1E5E5E5EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFECECEA
          952E7C93297A93297A93297A8E29758E29758E297585216D85216D85216D8521
          6D85216D85216D7C1C64741B5F9A5E8D00833C00BF5900833CD7D7D7DDDDDDDD
          DDDDE3E3E1E3E3E1E3E3E1E3E3E1ECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFE5E5E5DDDDDD9D3083ECECEAECECEAECECEAECECEAECECEAECEC
          EAECECEAECECEAECECEAECECEAECECEAE3E3E1D7D7D7DDDDDD00833C00BF5900
          BF5900833C00833C00833C00833C00833C00833C00833C00833CE5E5E5EFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE3E3E1D7D7D7A33888ECECEAECEC
          EAECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAE3E3E1E3
          E3E100833C00BF5900C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A
          00C45A00833CE5E5E5EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF952E7C952E
          7C8E2975A33888ECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAEC
          ECEAECECEAECECEAECECEA00833C00C45A00C45A00C45A00C45A00C45A00C45A
          00C45A00C45A00C45A00C45A00C45A00833CE5E5E5EFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEF9D3083ECECEAE3E3E1A33888ECECEAECECEAECECEAECECEAEC
          ECEAECECEAECECEAECECEAECECEAECECEAF3F2F200833C79E6AA00C45A00C45A
          00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00833CECEC
          EAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA33888ECECEAE5E5E5A93D8EEF
          EFEFEFEFEFEFEFEFC5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBD
          CCCCCC008B4079E6AA00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C4
          5A00C45A00C45A00833CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA3
          3888ECECEAE5E5E5AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2
          F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2008B4079E6AA00C45A00C45A79E6
          AA79E6AA79E6AA79E6AA79E6AA79E6AA79E6AA00833CF7F7F7EFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFA33888ECECEAE5E5E5A93D8EEFEFEFEFEFEFEFEFEF
          C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDB5B5B5B5B5B5B5B5B5F3F2
          F2008B4079E6AA00C45A00833C00833C00833C00833C00833C00833C00833C00
          833CFBFBFAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA93D8EEFEFEFECECEA
          AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2
          F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2008B4079E6AA00833CECECEAF7F7F7FB
          FBFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFAE4193F3F2F2ECECEAB8489CF3F2F2F3F2F2F3F2F2C5C5C5C5C5C5C5C5
          C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDF3F2F2F3F2F2F3F2F200
          833C00833CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA93D8EEFEFEFECECEABC4C9FF3F2F2F3F2
          F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3
          F2F2F3F2F2F3F2F2F3F2F2B66EA400833CF3F2F2EFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFAE4193F3F2
          F2ECECEAC452A7F7F7F7F7F7F7F7F7F7C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5
          C5C5BDBDBDBDBDBDBDBDBDBDBDBDF7F7F7F7F7F7F7F7F79D3083E5E5E5F3F2F2
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFB8489CF3F2F2ECECEAC956ABF7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7A33888DDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFBC4C9FF3F2F2EFEFEFCD58B0FB
          FBFAFBFBFAFBFBFAD461B6CD58B0C452A7B8489CAE4193AE4193A338889D3083
          93297A8C2373FBFBFAFBFBFAFBFBFAA93D8EDDDDDDEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFC4
          52A7F7F7F7F3F2F2D461B6FBFBFAFBFBFAFBFBFADD6CBEEB89D0EB89D0E982CE
          E87DCDE87DCDE87DCDE87DCDE87DCD9D3083FBFBFAFBFBFAFBFBFAAE4193DDDD
          DDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFC956ABF7F7F7F3F2F2DC65BDFBFBFAFBFBFAFBFBFA
          E56CC6ED8AD3ED8AD3E982CEE982CEE982CEE87DCDE87DCDE87DCD9D3083FBFB
          FAFBFBFAFBFBFAB8489CDDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFCD58B0FBFBFAF3F2F2
          E068C1FFFFFFFFFFFFFFFFFFE56CC6E068C1D461B6CD58B0C452A7C452A7B848
          9CAE4193A6398C9D3083FFFFFFFFFFFFFFFFFFBD4DA1DDDDDDEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFD461B6FBFBFAF3F2F2E068C1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4
          52A7DDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFDC65BDFBFBFAF7F7F7E56CC6FFFFFFFFFF
          FFFFFFFFC5C5C5C5C5C5C5C5C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBD
          BDBDFFFFFFFFFFFFFFFFFFC956ABE3E3E1EFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE068C1FFFF
          FFFBFBFAE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E5E5E5EFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFE068C1FFFFFFFFFFFFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFD15BB2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFE56CC6E5
          6CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E068C1E068C1
          E068C1DC65BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE5
          6CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E5E5E5EFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15B
          B2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6E56CC6E56CC6
          E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E068C1E068C1E068C1DC65
          BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF}
        Margin = 2
        ParentFont = False
        OnClick = spbImportClick
      end
      object labMustCheck_35: TLabel
        Left = 92
        Top = 112
        Width = 354
        Height = 16
        Caption = 'Level must large than [35] Alarm - Trouble Shooting'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
      end
      object rgJamLevel: TRadioGroup
        Left = 92
        Top = 64
        Width = 433
        Height = 45
        Columns = 4
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          'Operator'
          'Engineer'
          'Supervisor'
          'HonPrec')
        ParentFont = False
        TabOrder = 3
      end
      object cbJamArea: TComboBox
        Left = 92
        Top = 0
        Width = 145
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemHeight = 20
        ItemIndex = 0
        ParentFont = False
        TabOrder = 0
        Text = '01 Input Arm'
        OnChange = cbJamAreaChange
        Items.Strings = (
          '01 Input Arm'
          '02 Output Arm'
          '03 Index Unit'
          '04 Input Shuttle'
          '05 Output Shuttle'
          '06 Empty Tray Arm'
          '07 Tester I/F'
          '08 Scanner'
          '09 Tray Loader'
          '10 Empty Tray'
          '11 Tray Unloader 1'
          '12 Tray Unloader 2'
          '13 Tray Unloader 3'
          '14 Color Tray'
          '15 Temp. Controller'
          '16 System'
          '17 Fix Tray 1'
          '18 Fix Tray 2'
          '19 Fix Tray 3'
          '20 ESD System'
          '21 Process Log'
          '22 Motion Log'
          '23 Message'
          '24 Motor'
          '25 Tray Unloader 4'
          '26 Tray Unloader 5'
          '27 Tray Unloader 6'
          '28 Fix Tray 4'
          '29 Fix Tray 5'
          '30 Fix Tray 6'
          '31 Cylinder')
      end
      object cbJamCode: TComboBox
        Left = 92
        Top = 32
        Width = 725
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        TabOrder = 2
        OnChange = cbJamCodeChange
      end
      object cbJamLang: TComboBox
        Left = 88
        Top = 185
        Width = 145
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        TabOrder = 10
        Text = 'English'
        OnChange = cbJamLangChange
        Items.Strings = (
          'English'
          'Chinese'
          'Korean'
          'Singapore')
      end
      object RichEditJamCode: TRichEdit
        Left = 0
        Top = 389
        Width = 866
        Height = 298
        Align = alBottom
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 18
      end
      object cbJamNeedRed: TCheckBox
        Left = 560
        Top = 64
        Width = 217
        Height = 17
        Caption = 'Need Red Background'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object cbSilentMode: TCheckBox
        Left = 560
        Top = 88
        Width = 217
        Height = 17
        Caption = 'Silent Mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object cbUnlockPassWord: TCheckBox
        Left = 560
        Top = 112
        Width = 217
        Height = 17
        Caption = 'Need unlock password'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object rgMachineStatusBit8: TRadioGroup
        Left = 264
        Top = 185
        Width = 207
        Height = 45
        Caption = 'Machine Status:Bit8'
        Columns = 4
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          '0'
          '1')
        ParentFont = False
        TabOrder = 11
      end
      object cbIncludeMTBA: TCheckBox
        Left = 560
        Top = 136
        Width = 217
        Height = 17
        Caption = 'Include MTBA'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object chkCheckContAlarm: TCheckBox
        Left = 560
        Top = 160
        Width = 277
        Height = 17
        Caption = 'Check continuously alarm [O16]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object chkO17: TCheckBox
        Left = 560
        Top = 184
        Width = 277
        Height = 17
        Caption = 'Check continuously alarm [O17]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object cbAddAlarmLog: TCheckBox
        Left = 560
        Top = 208
        Width = 277
        Height = 17
        Caption = 'Add Alarm Log'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object cbN27AddBoard: TCheckBox
        Left = 720
        Top = 232
        Width = 145
        Height = 17
        Caption = 'N27 Add Board'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 14
      end
      object cbN27AlarmSel: TCheckBox
        Left = 560
        Top = 232
        Width = 153
        Height = 17
        Caption = 'N27 Alarm to FTP'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object cbN27AlarmSelByArea: TCheckBox
        Left = 248
        Top = 6
        Width = 217
        Height = 17
        Caption = 'N27 Alarm to FTP By Area'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object chkTCPAlarm: TCheckBox
        Left = 560
        Top = 256
        Width = 113
        Height = 17
        Caption = 'TCP Alarm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object cbContAlarmNotUpload: TCheckBox
        Left = 244
        Top = 304
        Width = 277
        Height = 17
        Caption = 'Continuously alarm not upload'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 17
      end
      object chkAlarmAfterFullTray: TCheckBox
        Left = 560
        Top = 280
        Width = 281
        Height = 17
        Caption = 'Alarm After Full Tray'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
    end
    object tsStatisticsJam: TTabSheet
      Caption = 'Statistics Jam'
      ImageIndex = 11
      object sgStatisticsJam: TStringGrid
        Left = 0
        Top = 0
        Width = 841
        Height = 554
        Align = alClient
        Color = 14670284
        ColCount = 8
        DefaultColWidth = 50
        DefaultRowHeight = 15
        FixedColor = 9534289
        RowCount = 60
        TabOrder = 0
        RowHeights = (
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15
          15)
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'csv'#13#10
    Filter = 'CSV (*.csv)|*.csv'
    InitialDir = 'D:\'
    Left = 776
    Top = 139
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'csv'
    Filter = 'CSV (*.csv)|*.csv'
    InitialDir = 'D:\'
    Left = 804
    Top = 139
  end
end
