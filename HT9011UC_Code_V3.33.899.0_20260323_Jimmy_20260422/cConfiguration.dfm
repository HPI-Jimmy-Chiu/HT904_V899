object fConfiguration: TfConfiguration
  Left = 443
  Top = 57
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'fConfiguration'
  ClientHeight = 875
  ClientWidth = 937
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 937
    Height = 833
    ActivePage = tsConfig
    Align = alClient
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MultiLine = True
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    TabIndex = 2
    TabOrder = 0
    TabWidth = 130
    OnChange = PageControl1Change
    object tsSoftSimu: TTabSheet
      Caption = 'Soft'
      ImageIndex = 7
      object pnlSoftSpeed: TPanel
        Left = 0
        Top = 41
        Width = 929
        Height = 757
        Align = alClient
        BevelInner = bvLowered
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object btnAdd1000: TButton
          Left = 716
          Top = 472
          Width = 177
          Height = 33
          Caption = '+1000'
          TabOrder = 1
          OnClick = btnAdd1000Click
        end
        object btnDec1000: TButton
          Left = 716
          Top = 513
          Width = 177
          Height = 33
          Caption = '-1000'
          TabOrder = 2
          OnClick = btnDec1000Click
        end
        object btnSetTo1000: TButton
          Left = 716
          Top = 554
          Width = 177
          Height = 33
          Caption = '=1000'
          TabOrder = 3
          OnClick = btnSetTo1000Click
        end
        object btnAdd10000: TButton
          Left = 716
          Top = 430
          Width = 177
          Height = 33
          Caption = '+10000'
          TabOrder = 0
          OnClick = btnAdd10000Click
        end
        object btnSetToTech: TButton
          Left = 716
          Top = 596
          Width = 177
          Height = 33
          Caption = 'Set Offset To Tech'
          TabOrder = 4
          OnClick = btnSetToTechClick
        end
      end
      object pnlSoftSpeed_1: TPanel
        Left = 0
        Top = 0
        Width = 929
        Height = 41
        Align = alTop
        BevelInner = bvLowered
        Caption = 'Soft Simulate Speed'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsTempComm: TTabSheet
      Caption = 'Comm'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImageIndex = 9
      ParentFont = False
      object gbSendTemp: TGroupBox
        Left = 0
        Top = 0
        Width = 309
        Height = 798
        Align = alLeft
        Caption = 'Send'
        TabOrder = 0
        object sbSendTemp: TSpeedButton
          Tag = 1
          Left = 104
          Top = 700
          Width = 197
          Height = 41
          Caption = 'Send Temp Set'
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0040800040
            80004080004080004080004080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF333333333333
            3333333333333333333333333333333333333333333333330099FF0099FF0099
            FF0099FF0099FF33333333333333333333333333333333333333333333333333
            3333333333333333333333333333333333FFFFFFFFFFFFFFFFFFD7D7D7D7D7D7
            D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D780FFFF33FFFF33FF
            FF33FFFF33FFFF0099FFD7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7
            D7D7D7D7D7D7D7D7D7D7D7D7D7D75F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF33CCFF33CC
            FF33CCFF33CCFF33CCFF0099FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFD7D7D7777777333333FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFB2B2B25F5F5F333333FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF333333333333
            3333333333333333333333333333333333333333333333333333333333333333
            3333333333333333333333333333333333333333333333333333333333333333
            3333333333333333333333333333FFFFFFFFFFFFFFFFFFFFFFFF666666666666
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666666666666666666666333333FFFFFFFFFFFFFFFFFFB2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2666666333333FFFFFFFFFFFFFFFFFFB2B2B2818181
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666818181B2B2B2666666333333333333FFFFFFFFFFFFB2B2B2A4A0A0
            FFFFFFF1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1A4A0A0B2B2B2666666333333333333FFFFFFFFFFFFB2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2666666333333333333333333FFFFFFB2B2B2818181
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666666666666666666666666666666666666666666666666666666666
            6666666666818181B2B2B2666666333333333333333333FFFFFFB2B2B2A4A0A0
            F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1A4A0A0B2B2B2666666333333333333333333FFFFFFB2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2666666333333333333333333FFFFFFB2B2B2B2B2B2
            818181A4A0A0818181A4A0A0818181A4A0A0818181A4A0A0818181B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B233993333
            9933339933B2B2B2B2B2B2666666333333333333333333FFFFFFB2B2B2B2B2B2
            A4A0A0F1F1F1A4A0A0F1F1F1A4A0A0F1F1F1A4A0A0F1F1F1A4A0A0B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B266FF3366
            FF33339933B2B2B2B2B2B2666666333333333333333333FFFFFFB2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2
            B2B2B2B2B2B2B2B2B2B2B2666666333333333333333333FFFFFFCBCBCBCBCBCB
            CBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCB
            CBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCB
            CBCBCBCBCBCBCBCBCBCBCBB2B2B2333333333333333333FFFFFFFFFFFFE3E3E3
            E3E3E3E3E3E3E3E3E3DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDE3E3E3666666333333333333FFFFFFFFFFFFFFFFFF
            FFFFFFE3E3E3E3E3E3DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDDDDDDDB2B2B2333333333333FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
            DDDDDDDDDDDDDDDDDDDDDDDDDDDDE3E3E35F5F5F333333FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFF1F1F1FFFFFFF1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
            F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1B2B2B2333333FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
          OnClick = sbSendTempClick
        end
        object Label95: TLabel
          Left = 148
          Top = 673
          Width = 59
          Height = 16
          Caption = 'Set Temp'
        end
        object btHeaterClearSelect: TButton
          Left = 220
          Top = 608
          Width = 81
          Height = 25
          Caption = 'Clear'
          TabOrder = 0
          OnClick = btHeaterClearSelectClick
        end
        object btHeaterSelectAll: TButton
          Left = 220
          Top = 636
          Width = 81
          Height = 25
          Caption = 'All'
          TabOrder = 1
          OnClick = btHeaterSelectAllClick
        end
        object edSetTemp: TEdit
          Left = 220
          Top = 665
          Width = 81
          Height = 24
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          TabOrder = 2
          Text = '0'
          OnClick = edSetTempClick
          OnKeyDown = edSetTempKeyDown
        end
      end
      object Panel2: TPanel
        Left = 309
        Top = 0
        Width = 620
        Height = 798
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object grpCommMsg: TGroupBox
          Left = 0
          Top = 661
          Width = 620
          Height = 137
          Align = alBottom
          Caption = 'Send && Receive Monitor'
          TabOrder = 1
          object listHeaterMonitor: TListBox
            Left = 2
            Top = 18
            Width = 616
            Height = 97
            Align = alTop
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ItemHeight = 16
            TabOrder = 0
          end
          object chkHeater: TCheckBox
            Left = 464
            Top = 116
            Width = 97
            Height = 17
            Caption = 'Heater On'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = chkHeaterClick
          end
        end
        object gbGetTemp: TGroupBox
          Left = 0
          Top = 0
          Width = 620
          Height = 661
          Align = alClient
          Caption = 'Receive'
          TabOrder = 0
          object sbReadTemp: TSpeedButton
            Tag = 2
            Left = 404
            Top = 564
            Width = 197
            Height = 41
            Caption = 'Read Temp'
            Glyph.Data = {
              360C0000424D360C000000000000360000002800000020000000200000000100
              180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FF5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F
              5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5FFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A0
              A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4
              A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A05F5F5F5F5F5F5F5F5FFFFFFF333333
              333333333333333333004080004080004080004080003333333333333333D7D7
              D7C0C0C0B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2D7
              D7D7D7D7D7D7D7D7B2B2B2B2B2B2A4A0A05F5F5F5F5F5F5F5F5FA4A0A0808080
              8080808080800099FF0099FF0099FF0099FF808080808080808080808080D7D7
              D7C0C0C0339933339933B2B2B2B2B2B2B2B2B2B2B2B2B2B2B277777777777766
              66665F5F5F5F5F5F777777777777A4A0A05F5F5F5F5F5F5F5F5FFFFFFFD7D7D7
              D7D7D7D7D7D733FFFF33FFFF33FFFF33FFFF0099FFB2B2B2D7D7D7D7D7D7D7D7
              D7C0C0C066FF33339933C0C0C0B2B2B2B2B2B2B2B2B2B2B2B2B2B2B296969696
              9696969696969696B2B2B2B2B2B2A4A0A05F5F5F5F5F5F5F5F5FFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFF33CCFF33CCFF33CCFF33CCFF0099FFFFFFFFFFFFFFB2B2
              B2D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7C0C0C0C0C0C0C0C0C0A4A0A0A4
              A0A08686868686867777777777777777775F5F5F5F5F5F5F5F5FFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFD7D7D7777777333333FFFFFFFFFFFFFFFFFFFFFF
              FFA4A0A0B2B2B2B2B2B2B2B2B2CCCCCCB2B2B2C0C0C0B2B2B2A4A0A077777777
              7777777777777777808080808080868686868686777777666666FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFD7D7D7777777333333FFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFF969696A4A0A0A4A0A0CCCCCCB2B2B28080805F5F5F5F5F5F5F
              5F5F5F5F5F777777777777777777777777777777777777777777FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFB2B2B25F5F5F333333FFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCCCCCCCCCCCB2B2B28080805F5F5FFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF666666
              666666666666666666666666666666666666666666666666666666666666B2B2
              B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2C0C0C0C0C0C0C0C0C096969696969696
              9696969696969696969696A4A0A0666666FFFFFFFFFFFFFFFFFFA4A0A0A4A0A0
              A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0A4A0A0D7D7
              D7C1C1C1C1C1C1C1C1C1C1C1C1C1C1C1C1C1C1B2B2B2B2B2B2A4A0A0A4A0A0A4
              A0A0A4A0A0A4A0A0A4A0A0A4A0A0666666FFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2D7D7D7D7D7
              D7B2B2B2CC6633CC6633996633CC6633996633CC6633996633CC663399663399
              6633996633996633A4A0A0A4A0A0666666666666FFFFFFFFFFFFD7D7D7B2B2B2
              339933339933B2B2B2B2B2B2B2B2B2B2B2B2B2B2B27777777777775F5F5FD7D7
              D7B2B2B2CC6633FF6633FF6633FF6633CC6633996633CC663399663399663399
              6633996633996633A4A0A0A4A0A0666666666666FFFFFFFFFFFFD7D7D7C0C0C0
              66FF33339933B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2969696A4A0A0D7D7
              D7B2B2B2CC6633FF6633FF6633CC6633FF6633CC6633996633CC663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFD7D7D7D7D7D7
              D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7C0C0C0C0C0C0C0C0C0D7D7
              D7B2B2B2CC6633FF6633FF6633FF6633CC6633CC6633CC663399663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFA4A0A0A4A0A0
              B2B2B2B2B2B2CCCCCCB2B2B2C0C0C0B2B2B2A4A0A0777777808080808080D7D7
              D7B2B2B2CC6633FF6633FF6633CC6633CC6633CC6633996633CC663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFFFFFFFFFFFFF
              A4A0A0A4A0A0B2B2B2CCCCCCB2B2B28080805F5F5F5F5F5F5F5F5F5F5F5FD7D7
              D7B2B2B2CC6633FF6633FF6633FF6633CC6633CC6633CC663399663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFCCCCCCCCCCCCB2B2B27777775F5F5FFFFFFFFFFFFFD7D7
              D7B2B2B2CC6633FF6633FF6633CC6633FF6633CC6633996633CC663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFB2B2B2B2B2B2
              B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2D7D7
              D7B2B2B2CC6633FF6633FF6633FF6633CC6633FF6633CC663399663399663399
              6633996633996633A4A0A0A4A0A06666666666665F5F5FFFFFFFD7D7D7C1C1C1
              C1C1C1C1C1C1C1C1C1C1C1C1C1C1C1C0C0C0C0C0C0C0C0C0C0C0C0B2B2B2D7D7
              D7C1C1C1CC6633CC6633CC6633CC6633FF6633CC6633FF6633CC663399663399
              6633996633996633A4A0A0A4A0A0666666666666777777FFFFFFD7D7D7B2B2B2
              CC6633CC6633996633CC6633996633CC6633996633CC6633996633996633D7D7
              D7C1C1C1C1C1C1C1C1C1C1C1C1C0C0C0C0C0C0B2B2B2B2B2B2B2B2B2B2B2B2B2
              B2B2B2B2B2A4A0A0A4A0A0A4A0A0666666777777FFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633FF6633CC6633996633CC6633996633996633996633E3E3
              E3E3E3E3DDDDDDDDDDDDDDDDDDDDDDDDCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC0
              C0C0C0C0C0C0C0C0C0C0C0C0C0C0777777FFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633CC6633FF6633CC6633996633CC66339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633FF6633CC6633CC6633CC66339966339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633CC6633CC6633CC6633996633CC66339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633FF6633CC6633CC6633CC66339966339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633CC6633FF6633CC6633996633CC66339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633FF6633CC6633FF6633CC66339966339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7B2B2B2
              CC6633FF6633FF6633CC6633FF6633CC6633996633CC66339966339966339966
              33996633B2B2B2B2B2B26666666666665F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7C0C0C0
              CC6633FF6633FF6633FF6633CC6633FF6633CC6633FF66339966339966339966
              33996633B2B2B2B2B2B2666666666666777777FFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7C0C0C0
              C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0B2B2
              B2B2B2B2B2B2B2B2B2B2666666777777FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D7D7D7D7
              D7D7D7DDDDDDDDDDDDDDDDDDCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
              CCC0C0C0C0C0C0C0C0C0777777FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
            OnClick = sbSendTempClick
          end
        end
      end
    end
    object tsConfig: TTabSheet
      Caption = 'Config'
      ImageIndex = 14
      object pcConfig: TPageControl
        Left = 0
        Top = 0
        Width = 929
        Height = 798
        ActivePage = tsN00
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        MultiLine = True
        ParentFont = False
        Style = tsButtons
        TabIndex = 10
        TabOrder = 0
        TabWidth = 123
        OnChange = pcConfigChange
        object tsA00: TTabSheet
          Caption = 'A  [ Function ]'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          object MemoA: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pcA00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = tsA_05
            Align = alClient
            TabIndex = 4
            TabOrder = 0
            object tsA_00: TTabSheet
              Caption = '[A01] - [A10]'
              object pal_A1: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbA03: TCheckBox
                  Left = 5
                  Top = 158
                  Width = 400
                  Height = 17
                  Caption = '[A03] After home, pick and carry IC put to error bin'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbA04: TCheckBox
                  Left = 5
                  Top = 185
                  Width = 400
                  Height = 17
                  Caption = '[A04] Loader magazine, tray split fail can skip'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                end
                object cbA05: TCheckBox
                  Left = 5
                  Top = 212
                  Width = 400
                  Height = 17
                  Caption = '[A05] Use one touch docking (OTD)'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                end
                object cbA06: TCheckBox
                  Left = 5
                  Top = 239
                  Width = 400
                  Height = 17
                  Caption = '[A06] Use fail bin count alarm'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                  Visible = False
                end
                object cbA02: TCheckBox
                  Left = 5
                  Top = 132
                  Width = 400
                  Height = 17
                  Caption = '[A02] Can select Normal or Prime bin data'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbA08: TCheckBox
                  Left = 5
                  Top = 292
                  Width = 400
                  Height = 21
                  Caption = '[A08] Loader no tray clean out and clean out finish check again'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object cbA09: TCheckBox
                  Left = 5
                  Top = 323
                  Width = 400
                  Height = 21
                  Caption = '[A09] Use by arm close site function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                  OnClick = cbA09Click
                end
                object gbA10: TGroupBox
                  Left = 5
                  Top = 411
                  Width = 445
                  Height = 259
                  Caption = '[A10] Auto retest parameter (ART)'
                  TabOrder = 10
                  object labA10_4: TLabel
                    Left = 32
                    Top = 129
                    Width = 206
                    Height = 16
                    Caption = '[A10-4] Tray arm speed when ART'
                  end
                  object palA10_ART: TPanel
                    Left = 2
                    Top = 18
                    Width = 441
                    Height = 65
                    Align = alTop
                    BevelOuter = bvNone
                    Color = 12761254
                    TabOrder = 0
                    object labA10_2: TLabel
                      Left = 12
                      Top = 8
                      Width = 134
                      Height = 16
                      Caption = '[A10-2] Auto retest limit'
                    end
                    object labA10_3: TLabel
                      Left = 12
                      Top = 40
                      Width = 142
                      Height = 16
                      Caption = '[A10-3] Fail yield rate >='
                    end
                    object labA10_3_1: TLabel
                      Left = 244
                      Top = 40
                      Width = 12
                      Height = 16
                      Caption = '%'
                    end
                    object edA10_2: TEdit
                      Left = 168
                      Top = 4
                      Width = 73
                      Height = 24
                      TabOrder = 0
                    end
                    object edA10_3: TEdit
                      Left = 168
                      Top = 36
                      Width = 73
                      Height = 24
                      TabOrder = 1
                    end
                  end
                  object cbA10_3_ARTTestMode: TComboBox
                    Left = 244
                    Top = 94
                    Width = 145
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 6
                    TabOrder = 1
                    Text = '32 Bin GS'
                    Visible = False
                    Items.Strings = (
                      'Advan type1'
                      '256 Bin'
                      '16 Bin'
                      '32 Bin'
                      'SPEA Type'
                      '16 Bin GS'
                      '32 Bin GS')
                  end
                  object cbA10_3: TCheckBox
                    Left = 13
                    Top = 98
                    Width = 225
                    Height = 17
                    Caption = '[A10-3] Lock test mode when ART'
                    TabOrder = 2
                    Visible = False
                  end
                  object edA10_4: TEdit
                    Left = 244
                    Top = 125
                    Width = 73
                    Height = 24
                    TabOrder = 3
                  end
                  object cbA10_5: TCheckBox
                    Left = 13
                    Top = 160
                    Width = 369
                    Height = 17
                    Caption = '[A10-5] Enable auto correction function'
                    TabOrder = 4
                  end
                  object cbA10_6: TCheckBox
                    Left = 13
                    Top = 191
                    Width = 252
                    Height = 17
                    Caption = '[A10-6] Enable HANA test mode'
                    TabOrder = 6
                  end
                  object cbA10_6_HANA_ARTMode: TComboBox
                    Left = 244
                    Top = 185
                    Width = 101
                    Height = 24
                    ItemHeight = 16
                    TabOrder = 5
                    Text = 'General'
                    Visible = False
                    Items.Strings = (
                      'General'
                      'SMILL')
                  end
                  object cbA10_7: TCheckBox
                    Left = 13
                    Top = 222
                    Width = 148
                    Height = 17
                    Caption = '[A10-7] Enable FTCT'
                    TabOrder = 7
                  end
                end
                object GroupBox2: TGroupBox
                  Left = 6
                  Top = 15
                  Width = 445
                  Height = 105
                  Caption = '[A01] Competence'
                  TabOrder = 0
                  object cbA01: TCheckBox
                    Left = 21
                    Top = 28
                    Width = 400
                    Height = 17
                    Caption = 
                      '[A01] Auto switch to operator when idle over              sec.(>' +
                      '10Sec) '
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                  end
                  object edA01: TEdit
                    Left = 300
                    Top = 24
                    Width = 33
                    Height = 24
                    MaxLength = 3
                    TabOrder = 0
                    Text = '60'
                  end
                  object cbA01_1: TCheckBox
                    Left = 21
                    Top = 53
                    Width = 400
                    Height = 17
                    Caption = '[A01_1] Press start auto switch to operator'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object cbA01_2: TCheckBox
                    Left = 21
                    Top = 77
                    Width = 400
                    Height = 17
                    Caption = '[A01_2] Disable Saving Parameters when switch to operator '
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                end
                object cbA10: TCheckBox
                  Left = 5
                  Top = 381
                  Width = 200
                  Height = 21
                  Caption = '[A10-1] Enable ART'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object chkA09_1: TCheckBox
                  Left = 25
                  Top = 351
                  Width = 348
                  Height = 21
                  Caption = '[A09-1] If all site closed, disable arm automatically.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                  OnClick = cbA09Click
                end
              end
            end
            object tsA_01: TTabSheet
              Caption = '[A11] - [A20]'
              ImageIndex = 1
              object pal_A2: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object lblA16: TLabel
                  Left = 64
                  Top = 168
                  Width = 323
                  Height = 16
                  Caption = 'Normal use direct contact mode and vacuum off mode.'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                end
                object BitBtn1: TBitBtn
                  Left = 12
                  Top = 285
                  Width = 241
                  Height = 33
                  Caption = 'Set Vender Password'
                  TabOrder = 8
                  Visible = False
                  OnClick = BitBtn1Click
                  Glyph.Data = {
                    AA040000424DAA04000000000000360000002800000014000000130000000100
                    18000000000074040000C40E0000C40E00000000000000000000FFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
                    FFFF00FFFF00000000000000FFFF00FFFF00000000000000FFFF00FFFF00FFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFF00FFFF0000
                    0000FFFF00FFFF00000000000000FFFF00000000FFFF00000000FFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFF00FFFF
                    00000000C0C0C0FFFFFF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000C0C0C0
                    FFFFFFC0C0C0000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000C0C0C0FFFFFF00000000
                    0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFF000000000000000000C0C0C0FFFFFFC0C0C0FFFF
                    FFC0C0C0000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFF000000000000000000FFFFFFC0C0C0FFFFFFC0C0C0000000
                    000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FF000000000000000000000000C0C0C0FFFFFFC0C0C0FFFFFFC0C0C0000000FF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
                    000000000000C0C0C0FFFFFFC0C0C0FFFFFFC0C0C0FFFFFFC0C0C0000000FFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000
                    0000000000000000FFFFFFC0C0C0FF0000C0C0C0000000FFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000
                    00FFFFFFC0C0C0FFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000000808080C0C0C0
                    FFFFFFC0C0C0FFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFF000000000000000000000000000000000000808080FF
                    FFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000
                    00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFF000000000000000000000000000000000000000000FFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
                end
                object cbA11: TCheckBox
                  Left = 9
                  Top = 14
                  Width = 400
                  Height = 17
                  Caption = '[A11] Barcode reader over                sec.(>10Sec) '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object edA11: TEdit
                  Left = 197
                  Top = 12
                  Width = 33
                  Height = 24
                  MaxLength = 3
                  TabOrder = 0
                  Text = '60'
                end
                object pnlA12: TPanel
                  Left = 9
                  Top = 35
                  Width = 432
                  Height = 57
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 2
                  object lblA12: TLabel
                    Left = 40
                    Top = 33
                    Width = 103
                    Height = 16
                    Caption = 'Tray Arm X Offset'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object cbA12: TCheckBox
                    Left = 1
                    Top = 6
                    Width = 276
                    Height = 21
                    Caption = '[A12] Clear loader tray device         Count'#65306
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                  end
                  object edtA12_1: TEdit
                    Left = 153
                    Top = 26
                    Width = 76
                    Height = 24
                    OEMConvert = True
                    TabOrder = 2
                  end
                  object edA12: TEdit
                    Left = 269
                    Top = 4
                    Width = 80
                    Height = 24
                    MaxLength = 3
                    TabOrder = 0
                    Text = '2'
                  end
                end
                object cbA14: TCheckBox
                  Left = 9
                  Top = 88
                  Width = 400
                  Height = 21
                  Caption = '[A14] Use barcode reader to change work file'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                  OnClick = cbA09Click
                end
                object cbA15: TCheckBox
                  Left = 9
                  Top = 116
                  Width = 400
                  Height = 21
                  Caption = '[A15] Use ESD auto decay function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object chA16: TCheckBox
                  Left = 9
                  Top = 142
                  Width = 400
                  Height = 21
                  Caption = '[A16] Contact test use drop contact and vacuum off mode'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object cbA19: TCheckBox
                  Left = 9
                  Top = 321
                  Width = 400
                  Height = 21
                  Caption = '[A19] Use PM alarm function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object edA15_ESDReportTime: TEdit
                  Left = 253
                  Top = 112
                  Width = 80
                  Height = 24
                  MaxLength = 3
                  TabOrder = 4
                  Text = '2'
                end
                object grpA20: TGroupBox
                  Left = 4
                  Top = 352
                  Width = 449
                  Height = 165
                  Caption = '[A20] Start Check Function'
                  TabOrder = 10
                  Visible = False
                  object cbA20_1: TCheckBox
                    Left = 9
                    Top = 20
                    Width = 400
                    Height = 21
                    Caption = '[A20-1] Check RTC function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object cbA20_2: TCheckBox
                    Left = 9
                    Top = 48
                    Width = 400
                    Height = 21
                    Caption = '[A20-2] Check Tray ID function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                  end
                  object cbA20_3: TCheckBox
                    Left = 9
                    Top = 76
                    Width = 400
                    Height = 21
                    Caption = '[A20-3] Check auto clean function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object cbA20_4: TCheckBox
                    Left = 9
                    Top = 104
                    Width = 400
                    Height = 21
                    Caption = '[A20-4] Check conts fail function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                  object cbA20_5: TCheckBox
                    Left = 9
                    Top = 132
                    Width = 400
                    Height = 21
                    Caption = '[A20-5] Check OCR function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                end
                object grpA17: TGroupBox
                  Left = 4
                  Top = 188
                  Width = 449
                  Height = 73
                  Caption = '[A17] Reset Function'
                  TabOrder = 7
                  object cbA17_1: TCheckBox
                    Left = 9
                    Top = 20
                    Width = 265
                    Height = 21
                    Caption = '[A17-1] Disable RESET button '
                    TabOrder = 0
                  end
                  object cbA17_2: TCheckBox
                    Left = 9
                    Top = 44
                    Width = 392
                    Height = 21
                    Caption = '[A17-2] RESET without testing until cleam out finish'
                    TabOrder = 1
                  end
                end
              end
            end
            object tsA_03: TTabSheet
              Caption = '[A21]-[A30]'
              ImageIndex = 2
              object pal_A3: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object spbA27: TSpeedButton
                  Tag = 1
                  Left = 16
                  Top = 399
                  Width = 226
                  Height = 28
                  Hint = 'N14_sb'
                  Caption = '[A27] Save Standard Config'
                  Visible = False
                  OnClick = spbA27Click
                end
                object cbA21: TCheckBox
                  Left = 5
                  Top = 4
                  Width = 400
                  Height = 21
                  Caption = '[A21] Rotator detect IC floating error, need to shake.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                  Visible = False
                end
                object gbA22: TGroupBox
                  Left = 5
                  Top = 40
                  Width = 400
                  Height = 113
                  Caption = '[A22] Magnetic Scale'
                  TabOrder = 1
                  object labA22_3: TLabel
                    Left = 344
                    Top = 48
                    Width = 22
                    Height = 16
                    Caption = 'mm'
                  end
                  object labA22_4: TLabel
                    Left = 344
                    Top = 80
                    Width = 22
                    Height = 16
                    Caption = 'mm'
                  end
                  object labA22_1: TLabel
                    Left = 28
                    Top = 48
                    Width = 244
                    Height = 16
                    Caption = '[A22-2] Show message and keep running'
                  end
                  object labA22_2: TLabel
                    Left = 28
                    Top = 80
                    Width = 239
                    Height = 16
                    Caption = '[A22-3] Show message and stop running'
                  end
                  object cbA22_1: TCheckBox
                    Left = 21
                    Top = 20
                    Width = 220
                    Height = 21
                    Caption = '[A22-1] Enable magnetic scale'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object edA22_2: TEdit
                    Left = 280
                    Top = 45
                    Width = 59
                    Height = 24
                    TabOrder = 1
                    OnClick = edA22_2Click
                  end
                  object edA22_3: TEdit
                    Left = 280
                    Top = 77
                    Width = 59
                    Height = 24
                    TabOrder = 2
                    OnClick = edA22_3Click
                  end
                end
                object cbA23: TCheckBox
                  Left = 5
                  Top = 163
                  Width = 400
                  Height = 21
                  Caption = '[A23] Check '#39'lot no'#39' in SLT report'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                  Visible = False
                end
                object cbA24: TCheckBox
                  Left = 5
                  Top = 188
                  Width = 400
                  Height = 21
                  Caption = '[A24] Auto backup setup file'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                  Visible = False
                end
                object grpA25: TGroupBox
                  Left = 16
                  Top = 220
                  Width = 437
                  Height = 93
                  Caption = '[A25]  Run Execut File'
                  TabOrder = 4
                  object spbA25_RunExecutFilePathChoice: TSpeedButton
                    Tag = 1
                    Left = 385
                    Top = 19
                    Width = 44
                    Height = 28
                    Hint = 'N14_sb'
                    Glyph.Data = {
                      DE030000424DDE03000000000000360000002800000011000000120000000100
                      180000000000A803000001000000010000000000000000000000FFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFBFFFFFFFFFFFFD4DEDED6D4D3D5CFD4DAD1CDD8D2D3DBD0D2D9D1D2D3CECF
                      D3D1D1D5D6D4D4D5D3E1D9D9DEEFFFFFFFFFFFFFFFFFFFFFFF60B8DC4CB0D228
                      A5CB20A9D717AAD216AAD41DA9D21DABD41EAAD321A8D41FAAD51DA6CD3BB0D7
                      77B8D7FFFFFFFFFFFFF2FFFFFF39B1D488E4FD5EDBF065D8F364DBF068D9F46D
                      D6F16ADAF069DBF26AD7F35EDAF25BD9EB89EBFB32ABD2FFFFFFFFFFFFF3FFFF
                      FF1AA5CC82E3FD32CDE636C6E939C8E72FC8ED37C7EA30C6E437C8E83DC4EA31
                      C7EB38CDE775E3FB11A9D2FFFFFFFFFFFFFCFFFFFF21A8DA89E0FF37CDEB40CF
                      F54ECCF540C8F84CCEF942CBF149CFF74DCBFB39CDF73BD1F077E2FD1BA9D8FF
                      FFFFFFFFFFFFFFFFFF22ABD28BEAFE46DEF642DBFC5AD6FE44D2FB55DAFF47D6
                      F250D8FA58D4FD44D8FC48DCF485E8FC28A2D6FFFFFFFFFFFFF6FFFFFF32A6DD
                      8AF0FC5FE7F964DFF966D9FF5CE8FF65E5FC63E2FB62E4FF5EE2FF57E2FD67E1
                      F997EDFF34ACCFFFFFFFFFFFFFFCFFFFFF22A8D899F6FF61F1F65EF2F864E7FB
                      71E8F771EEFC62EFFE5AE7FA67E7FC6DEEFD71EEF298FFFC1BB1D0FFFFFFFFFF
                      FFEFFFFFFF23A9D3B4F9FF6DF2FA6AF9FE89F2FF9BFDFF9DF9FAA0FEF9A7FFFC
                      AAFDFEA4FFFF9CF8FDB3F8FF19ABCFFFFFFFFFFFFFFDFFFFFF14AAC8BDFDFF82
                      FAFA7DFCF3A7FEFB13A6CE1CAED216AFD01AAED221A7D11DA8D31CA9D42EAAD3
                      31A5CEFFFFFFFFFFFFFAFFFFFF2AB2CCBCF3FFAFFDFFADFFFAC7FFF838B5C97D
                      F0FD85FAFF86F7F990FCFD83FBF788FCF5A4FFFA30A8CBFFFFFFFFFFFFFFFFFF
                      FF1DACC82CAECC19AED01AAED225A9CE5FCDE5A4FAFFB1FAFFB2F9FCB1FFFE9C
                      FCFCA5FFFFC4FEFF29B0C6FFFFFFFFFFFFF7FFFFFF6DC8E32CB0CE1BA7D017AA
                      D81AABD923ACCC26AFCF18A9C91FAED324A9D51DA8D322AED330A8CB7ACDE2FF
                      FFFFFFFFFFF9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFF}
                    Visible = False
                    OnClick = spbA25_RunExecutFilePathChoiceClick
                  end
                  object lbl25_1: TLabel
                    Left = 53
                    Top = 64
                    Width = 77
                    Height = 16
                    Hint = 'N14_lb'
                    Caption = 'Button Name'
                    Visible = False
                  end
                  object edA25_1: TEdit
                    Left = 5
                    Top = 21
                    Width = 364
                    Height = 24
                    Hint = 'N14_ed'
                    TabOrder = 0
                    Visible = False
                  end
                  object edA25_2: TEdit
                    Left = 136
                    Top = 60
                    Width = 185
                    Height = 24
                    Hint = 'N14_ed'
                    TabOrder = 1
                    Visible = False
                  end
                end
                object grpA28: TGroupBox
                  Left = 13
                  Top = 432
                  Width = 433
                  Height = 73
                  Caption = '[A28] Open html'
                  TabOrder = 8
                  object spbA28_2: TSpeedButton
                    Tag = 1
                    Left = 347
                    Top = 29
                    Width = 65
                    Height = 28
                    Hint = 'N14_sb'
                    Caption = 'OPEN'
                    Visible = False
                    OnClick = spbA28_2Click
                  end
                  object edtA28_1: TEdit
                    Left = 23
                    Top = 30
                    Width = 306
                    Height = 24
                    TabOrder = 0
                  end
                end
                object cbA26: TCheckBox
                  Left = 5
                  Top = 324
                  Width = 400
                  Height = 21
                  Caption = '[A26] Motor speed sort display'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object cbA27_1: TCheckBox
                  Left = 25
                  Top = 374
                  Width = 400
                  Height = 17
                  Caption = '[A27-1] Log light scale data'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbA27: TCheckBox
                  Left = 5
                  Top = 349
                  Width = 400
                  Height = 17
                  Caption = '[A27] Enable light scale'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                  OnClick = cbA27Click
                end
                object cbA29: TCheckBox
                  Left = 5
                  Top = 516
                  Width = 400
                  Height = 21
                  Caption = '[A29] Enable auto clean function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object cbA30: TCheckBox
                  Left = 5
                  Top = 544
                  Width = 400
                  Height = 21
                  Caption = '[A30] Setup teach function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                end
              end
            end
            object tsA03: TTabSheet
              Caption = '[A31]-[A40]'
              ImageIndex = 3
              object pal_A4: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object gbA31: TGroupBox
                  Left = 2
                  Top = 2
                  Width = 467
                  Height = 125
                  Align = alTop
                  Caption = '[A31] Auto Clean Ion Fan'
                  TabOrder = 0
                  object cbA31: TCheckBox
                    Left = 13
                    Top = 20
                    Width = 240
                    Height = 17
                    Caption = '[A31] Enable auto clean ION fan'
                    Color = 12761254
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                  end
                  object gbA31_CleanTiming: TGroupBox
                    Left = 8
                    Top = 40
                    Width = 345
                    Height = 73
                    Caption = 'Clean Timing'
                    TabOrder = 1
                    object Label2: TLabel
                      Left = 23
                      Top = 109
                      Width = 55
                      Height = 16
                      Caption = 'Halt time:'
                      Visible = False
                    end
                    object Label6: TLabel
                      Left = 144
                      Top = 116
                      Width = 54
                      Height = 16
                      Caption = 'min / Unit'
                    end
                    object cbA31_Everyonecycle: TCheckBox
                      Left = 9
                      Top = 48
                      Width = 160
                      Height = 17
                      Caption = 'One cycle'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 2
                      Visible = False
                    end
                    object cbA31_1: TCheckBox
                      Left = 9
                      Top = 24
                      Width = 160
                      Height = 17
                      Caption = 'Initia start'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 0
                    end
                    object cbA31_IonFanAlarm: TCheckBox
                      Left = 169
                      Top = 24
                      Width = 160
                      Height = 17
                      Caption = 'ION fan alarm'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 1
                      Visible = False
                    end
                  end
                end
                object grpA32: TGroupBox
                  Left = 2
                  Top = 127
                  Width = 467
                  Height = 257
                  Align = alTop
                  Caption = '[A32] FTP Automation'
                  TabOrder = 1
                  object labA32_1: TLabel
                    Left = 46
                    Top = 58
                    Width = 106
                    Height = 16
                    Caption = '[A32-1]Handler ID'
                  end
                  object Image1: TImage
                    Left = 400
                    Top = 8
                    Width = 41
                    Height = 21
                    Transparent = True
                    OnDblClick = Image1DblClick
                  end
                  object cbA32: TCheckBox
                    Left = 5
                    Top = 27
                    Width = 400
                    Height = 17
                    Caption = '[A32] Enable FTP Automation'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object edA32_1: TEdit
                    Left = 154
                    Top = 55
                    Width = 97
                    Height = 24
                    TabOrder = 1
                    OnClick = edA32_1Click
                  end
                  object cbA32_2: TCheckBox
                    Left = 45
                    Top = 84
                    Width = 212
                    Height = 17
                    Caption = '[A32-2] Return Handler ID To OI'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object grpA32_1: TGroupBox
                    Left = 16
                    Top = 128
                    Width = 409
                    Height = 121
                    Caption = 'Check List Enable'
                    TabOrder = 4
                    Visible = False
                    object cbA32_01: TCheckBox
                      Left = 16
                      Top = 24
                      Width = 97
                      Height = 17
                      Caption = 'Temperature'
                      TabOrder = 0
                    end
                    object cbA32_02: TCheckBox
                      Left = 128
                      Top = 24
                      Width = 97
                      Height = 17
                      Caption = 'Alarm'
                      TabOrder = 1
                    end
                    object cbA32_03: TCheckBox
                      Left = 240
                      Top = 24
                      Width = 97
                      Height = 17
                      Caption = 'FT_Yield'
                      TabOrder = 2
                    end
                    object cbA32_04: TCheckBox
                      Left = 16
                      Top = 48
                      Width = 97
                      Height = 17
                      Caption = 'SiteMapping'
                      TabOrder = 3
                    end
                    object cbA32_05: TCheckBox
                      Left = 128
                      Top = 48
                      Width = 97
                      Height = 17
                      Caption = 'Speed'
                      TabOrder = 4
                    end
                    object cbA32_06: TCheckBox
                      Left = 240
                      Top = 48
                      Width = 97
                      Height = 17
                      Caption = 'Contact'
                      TabOrder = 5
                    end
                    object cbA32_07: TCheckBox
                      Left = 16
                      Top = 72
                      Width = 97
                      Height = 17
                      Caption = 'Category'
                      TabOrder = 6
                    end
                    object cbA32_08: TCheckBox
                      Left = 128
                      Top = 72
                      Width = 97
                      Height = 17
                      Caption = 'BinSetting'
                      TabOrder = 7
                    end
                    object cbA32_09: TCheckBox
                      Left = 240
                      Top = 72
                      Width = 97
                      Height = 17
                      Caption = 'TrayForm'
                      TabOrder = 8
                    end
                    object cbA32_10: TCheckBox
                      Left = 16
                      Top = 96
                      Width = 97
                      Height = 17
                      Caption = 'HotPlate'
                      TabOrder = 9
                    end
                  end
                  object cbA32_3: TCheckBox
                    Left = 45
                    Top = 108
                    Width = 212
                    Height = 17
                    Caption = '[A32-3] For 93K Function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                end
                object cbA33: TCheckBox
                  Left = 5
                  Top = 400
                  Width = 400
                  Height = 17
                  Caption = '[A33] Set machine IC to error bin after out shuttle loss IC'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbA34: TCheckBox
                  Left = 5
                  Top = 432
                  Width = 400
                  Height = 17
                  Caption = '[A34] Lock test socket IC check In contact form'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                  Visible = False
                end
                object cbA35: TCheckBox
                  Left = 5
                  Top = 464
                  Width = 447
                  Height = 21
                  Caption = 
                    '[A35] After out shuttle lose IC/out arm pickup error, set to err' +
                    'or bin.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                  Visible = False
                end
                object cbA36: TCheckBox
                  Left = 5
                  Top = 500
                  Width = 445
                  Height = 21
                  Caption = '[A36] Open door need device to Error bin.(index+output shuttle)'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                  Visible = False
                end
                object cbA37: TCheckBox
                  Left = 5
                  Top = 535
                  Width = 400
                  Height = 21
                  Caption = '[A37] Supported EAP 3.8'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                  Visible = False
                end
                object cbA38: TCheckBox
                  Left = 5
                  Top = 571
                  Width = 400
                  Height = 21
                  Caption = '[A38] SLT summary.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                  Visible = False
                end
                object cbA39: TCheckBox
                  Left = 5
                  Top = 607
                  Width = 400
                  Height = 17
                  Caption = '[A39] Record run state                        sec.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object edA39: TEdit
                  Left = 168
                  Top = 604
                  Width = 57
                  Height = 24
                  MaxLength = 3
                  TabOrder = 8
                  Text = '60'
                end
                object cbA40: TCheckBox
                  Left = 5
                  Top = 636
                  Width = 408
                  Height = 21
                  Caption = '[A40] Don'#39't record 2D data when contact page is opened'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                end
              end
            end
            object tsA_05: TTabSheet
              Caption = '[A51-A70]'
              ImageIndex = 4
              object gbA56: TGroupBox
                Left = 5
                Top = 188
                Width = 452
                Height = 121
                Caption = '[A56] Auto Teach Funciton'
                TabOrder = 3
                object lblA56_2: TLabel
                  Left = 32
                  Top = 58
                  Width = 369
                  Height = 20
                  AutoSize = False
                  Caption = 
                    '[A56-2] Shuttle Pick Up Offset When Auto Teach                  ' +
                    '   um'
                end
                object lblA56_3: TLabel
                  Left = 32
                  Top = 88
                  Width = 401
                  Height = 20
                  AutoSize = False
                  Caption = 
                    '[A56-3] Socket Pick Up Offset When Auto Teach Only              ' +
                    '       um'
                end
                object cbA56_1: TCheckBox
                  Left = 13
                  Top = 20
                  Width = 252
                  Height = 29
                  Caption = '[A56-1] Enable Auto Teach Funciton'
                  TabOrder = 0
                end
                object edA56_3: TEdit
                  Left = 352
                  Top = 86
                  Width = 55
                  Height = 24
                  Hint = 'A41-1_ed'
                  MaxLength = 5
                  TabOrder = 2
                  Text = '500'
                end
                object edA56_2: TEdit
                  Left = 320
                  Top = 55
                  Width = 55
                  Height = 24
                  Hint = 'A41-1_ed'
                  MaxLength = 5
                  TabOrder = 1
                  Text = '500'
                end
              end
              object gbA57: TGroupBox
                Left = 5
                Top = 312
                Width = 452
                Height = 97
                Caption = '[A57] Receipe Save By Machine'
                TabOrder = 4
                object cbA57_1: TCheckBox
                  Left = 13
                  Top = 20
                  Width = 252
                  Height = 29
                  Caption = '[A57-1] Save ArmSpeed By Machine'
                  TabOrder = 0
                end
                object cbA57_2: TCheckBox
                  Left = 13
                  Top = 40
                  Width = 252
                  Height = 29
                  Caption = '[A57-2] Save Temperature By Machine'
                  TabOrder = 1
                end
                object cbA57_3: TCheckBox
                  Left = 13
                  Top = 60
                  Width = 252
                  Height = 29
                  Caption = '[A57-3] Save Offset By Machine'
                  TabOrder = 2
                end
              end
              object cbA51: TCheckBox
                Left = 5
                Top = 43
                Width = 400
                Height = 21
                Caption = '[A51] EQC mode'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
                Visible = False
              end
              object chkA50: TCheckBox
                Left = 5
                Top = 8
                Width = 400
                Height = 21
                Caption = '[A50] 1x4 bias mode can use Y-offset (Default is 30mm)'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 0
                Visible = False
              end
              object grpA55: TGroupBox
                Left = 5
                Top = 96
                Width = 452
                Height = 89
                Caption = '[A55] PM Alarm Update From Server by FTP'
                TabOrder = 2
                object edA55: TEdit
                  Left = 7
                  Top = 52
                  Width = 434
                  Height = 24
                  TabOrder = 1
                end
                object cbA55: TCheckBox
                  Left = 13
                  Top = 20
                  Width = 252
                  Height = 29
                  Caption = '[A55] Enable'
                  TabOrder = 0
                end
              end
              object cbA58: TCheckBox
                Left = 5
                Top = 415
                Width = 400
                Height = 21
                Caption = '[A58] Show Close Sites Alarm'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 5
                Visible = False
              end
              object cbA65: TCheckBox
                Left = 5
                Top = 613
                Width = 400
                Height = 21
                Caption = '[A65] Support Bundle INFO'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 8
                Visible = False
              end
              object cbA66: TCheckBox
                Left = 5
                Top = 631
                Width = 400
                Height = 21
                Caption = '[A66] 2D Sort'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 9
                Visible = False
              end
              object cbA61: TCheckBox
                Left = 5
                Top = 575
                Width = 400
                Height = 21
                Caption = '[A61] Continuous Mode disable  clean MUBA'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 6
                Visible = False
              end
              object cbA62: TCheckBox
                Left = 5
                Top = 595
                Width = 400
                Height = 21
                Caption = '[A62] Use Stop Machine In/Out Arm Need To Home'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 7
              end
              object cbA67: TCheckBox
                Left = 5
                Top = 651
                Width = 400
                Height = 21
                Caption = '[A67] Trigger ONE CYCLE After SKIP specific Alarm.'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 10
                Visible = False
              end
              object cbA68: TCheckBox
                Left = 5
                Top = 675
                Width = 400
                Height = 21
                Caption = '[A68] Enable Automatic Loading and Unloading'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 11
                Visible = False
              end
              object grpA60: TGroupBox
                Left = 5
                Top = 436
                Width = 452
                Height = 137
                Caption = '[A60] AMR'
                TabOrder = 12
                object lblA60_1: TLabel
                  Left = 28
                  Top = 40
                  Width = 103
                  Height = 16
                  Caption = 'Notify Qty Loader'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object lblA60_2: TLabel
                  Left = 28
                  Top = 64
                  Width = 94
                  Height = 16
                  Caption = 'Notify Qty Auto1'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object lblA60_3: TLabel
                  Left = 28
                  Top = 88
                  Width = 94
                  Height = 16
                  Caption = 'Notify Qty Auto2'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object lblA60_4: TLabel
                  Left = 28
                  Top = 112
                  Width = 94
                  Height = 16
                  Caption = 'Notify Qty Auto3'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object lblA60_5: TLabel
                  Left = 228
                  Top = 40
                  Width = 88
                  Height = 16
                  Caption = 'Qty at one time'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object cbA60_1: TCheckBox
                  Left = 13
                  Top = 20
                  Width = 148
                  Height = 21
                  Caption = ' Enable AMR'
                  TabOrder = 0
                end
                object edA60_1: TEdit
                  Left = 144
                  Top = 36
                  Width = 73
                  Height = 24
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  Text = '10'
                end
                object edA60_2: TEdit
                  Left = 144
                  Top = 60
                  Width = 73
                  Height = 24
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  Text = '10'
                end
                object edA60_3: TEdit
                  Left = 144
                  Top = 84
                  Width = 73
                  Height = 24
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  Text = '10'
                end
                object edA60_4: TEdit
                  Left = 144
                  Top = 108
                  Width = 73
                  Height = 24
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 4
                  Text = '10'
                end
                object edA60_5: TEdit
                  Left = 320
                  Top = 36
                  Width = 73
                  Height = 24
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 5
                  Text = '10'
                end
              end
            end
            object tsA06: TTabSheet
              Caption = '[A71-A80]'
              ImageIndex = 5
              object gbA71: TGroupBox
                Left = 0
                Top = 0
                Width = 471
                Height = 73
                Align = alTop
                Caption = '[A71] Backup Now Recipe'
                TabOrder = 0
                object Label61: TLabel
                  Left = 16
                  Top = 45
                  Width = 71
                  Height = 16
                  Caption = 'Folder path:'
                end
                object edA71: TEdit
                  Left = 100
                  Top = 40
                  Width = 250
                  Height = 24
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 0
                  Text = 'D:\\Run'
                end
                object cbA71: TCheckBox
                  Left = 12
                  Top = 20
                  Width = 277
                  Height = 17
                  Caption = 'Enable'
                  TabOrder = 1
                end
                object btnA71Manually: TButton
                  Left = 360
                  Top = 40
                  Width = 75
                  Height = 25
                  Caption = 'Manually'
                  TabOrder = 2
                  OnClick = btnA71ManuallyClick
                end
              end
            end
          end
        end
        object tsb00: TTabSheet
          Caption = 'B [ Report ]'
          ImageIndex = 12
          object pcB00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = tsB_00
            Align = alClient
            TabIndex = 0
            TabOrder = 0
            TabWidth = 75
            object tsB_00: TTabSheet
              Caption = '[B01] - [B10]'
              object pal_B1: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object grpB01: TGroupBox
                  Left = 2
                  Top = 2
                  Width = 467
                  Height = 110
                  Align = alTop
                  Caption = '[B01] Precaution Record Function'
                  TabOrder = 0
                  object lblB01_AutoWakeupPrecautionRecordFormTime: TLabel
                    Left = 10
                    Top = 50
                    Width = 414
                    Height = 16
                    Caption = 
                      '[B01-1] Auto Wake Up Precaution Record Form Time                ' +
                      '          min'
                  end
                  object lblB01_PrecautionRecordSavePath: TLabel
                    Left = 6
                    Top = 76
                    Width = 82
                    Height = 16
                    Caption = 'Save To Path'
                  end
                  object cbB01: TCheckBox
                    Left = 10
                    Top = 23
                    Width = 303
                    Height = 22
                    Hint = 'B11'
                    Caption = '[B01] Use Precaution Record Function'
                    TabOrder = 0
                  end
                  object edB01: TEdit
                    Left = 331
                    Top = 44
                    Width = 65
                    Height = 24
                    Hint = 'B11-1_ed'
                    TabOrder = 1
                  end
                  object edB01_1: TEdit
                    Left = 96
                    Top = 72
                    Width = 349
                    Height = 24
                    Hint = 'B11-1_ed'
                    TabOrder = 2
                  end
                end
                object grpB02: TGroupBox
                  Left = 2
                  Top = 112
                  Width = 467
                  Height = 86
                  Align = alTop
                  Caption = '[B02] Handler Major Maintenance Record Function'
                  TabOrder = 1
                  object lblB02_HanderMajorMaintenanceRecordSavePath: TLabel
                    Left = 6
                    Top = 54
                    Width = 82
                    Height = 16
                    Caption = 'Save To Path'
                  end
                  object edB02: TEdit
                    Left = 96
                    Top = 50
                    Width = 349
                    Height = 24
                    Hint = 'B11-1_ed'
                    TabOrder = 1
                  end
                  object cbB02: TCheckBox
                    Left = 7
                    Top = 23
                    Width = 370
                    Height = 22
                    Hint = 'B12'
                    Caption = '[B02] Use Handler Major Maintenance Record Function'
                    TabOrder = 0
                  end
                end
                object grpB03: TGroupBox
                  Left = 2
                  Top = 198
                  Width = 467
                  Height = 121
                  Align = alTop
                  Caption = '[B03] Tester Report'
                  TabOrder = 2
                  object lblB03: TLabel
                    Left = 248
                    Top = 24
                    Width = 130
                    Height = 16
                    Caption = 'C:\HonPrecTestLogs\'
                  end
                  object lblB03_1: TLabel
                    Left = 22
                    Top = 55
                    Width = 69
                    Height = 16
                    Caption = 'Customer'#65306
                  end
                  object lblB03_2: TLabel
                    Left = 19
                    Top = 87
                    Width = 71
                    Height = 16
                    Caption = 'Device ID'#65306
                  end
                  object cbB03: TCheckBox
                    Left = 8
                    Top = 24
                    Width = 225
                    Height = 17
                    Caption = '[B03] Use tester report function'
                    TabOrder = 0
                  end
                  object edB03_1: TEdit
                    Left = 96
                    Top = 50
                    Width = 193
                    Height = 24
                    Hint = 'B11-1_ed'
                    TabOrder = 1
                  end
                  object edB03_2: TEdit
                    Left = 96
                    Top = 82
                    Width = 193
                    Height = 24
                    Hint = 'B11-1_ed'
                    TabOrder = 2
                  end
                end
                object grpB05: TGroupBox
                  Left = 2
                  Top = 319
                  Width = 467
                  Height = 66
                  Align = alTop
                  Caption = '[B05] O/S Test Report'
                  TabOrder = 3
                  object cbB05: TCheckBox
                    Left = 10
                    Top = 25
                    Width = 199
                    Height = 17
                    Hint = 'B06'
                    Caption = 'By lot save test log to server'
                    TabOrder = 1
                  end
                  object edtB05: TEdit
                    Left = 216
                    Top = 22
                    Width = 233
                    Height = 24
                    Hint = 'B06_ed'
                    TabOrder = 0
                  end
                end
              end
            end
            object tsB11_15: TTabSheet
              Caption = '[B11] - [B15]'
              ImageIndex = 1
              object pal_B11: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 681
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object gbB12: TGroupBox
                  Left = 2
                  Top = 81
                  Width = 467
                  Height = 80
                  Align = alTop
                  Caption = '[B12]   PAT SET UP'
                  TabOrder = 0
                  object Label30: TLabel
                    Left = 22
                    Top = 52
                    Width = 82
                    Height = 16
                    Caption = 'Save To Path'
                  end
                  object cbB12Enable: TCheckBox
                    Left = 10
                    Top = 23
                    Width = 159
                    Height = 22
                    Hint = 'B12'
                    Caption = '[B12] Enable'
                    TabOrder = 0
                  end
                  object edB12Path: TEdit
                    Left = 112
                    Top = 48
                    Width = 349
                    Height = 24
                    TabOrder = 1
                  end
                end
                object gbB14: TGroupBox
                  Left = 2
                  Top = 249
                  Width = 467
                  Height = 72
                  Align = alTop
                  Caption = '[B14]   PAT REPORT'
                  TabOrder = 1
                  object Label31: TLabel
                    Left = 8
                    Top = 20
                    Width = 173
                    Height = 16
                    Caption = 'Interval Time:                      Sec.'
                  end
                  object Label52: TLabel
                    Left = 8
                    Top = 44
                    Width = 60
                    Height = 16
                    Caption = 'RealTime'
                  end
                  object edB14IntervalTime: TEdit
                    Left = 96
                    Top = 16
                    Width = 49
                    Height = 24
                    TabOrder = 0
                  end
                  object edB14ReportRealTime: TEdit
                    Left = 72
                    Top = 40
                    Width = 385
                    Height = 24
                    TabOrder = 1
                  end
                end
                object gbB13: TGroupBox
                  Left = 2
                  Top = 161
                  Width = 467
                  Height = 88
                  Align = alTop
                  Caption = '[B13]   Management PAT Job Parameter'
                  TabOrder = 2
                  object Label55: TLabel
                    Left = 22
                    Top = 28
                    Width = 75
                    Height = 16
                    Caption = 'Upload Path'
                  end
                  object Label56: TLabel
                    Left = 22
                    Top = 60
                    Width = 91
                    Height = 16
                    Caption = 'Download Path'
                  end
                  object edB13UploadPath: TEdit
                    Left = 112
                    Top = 24
                    Width = 349
                    Height = 24
                    TabOrder = 0
                  end
                  object edB13DownloadPath: TEdit
                    Left = 120
                    Top = 56
                    Width = 341
                    Height = 24
                    TabOrder = 1
                  end
                end
                object gbB11: TGroupBox
                  Left = 2
                  Top = 2
                  Width = 467
                  Height = 79
                  Align = alTop
                  Caption = '[B11]    Read PAT Server'
                  TabOrder = 3
                  object Label59: TLabel
                    Left = 22
                    Top = 52
                    Width = 27
                    Height = 16
                    Caption = 'Path'
                  end
                  object cbB11Enable: TCheckBox
                    Left = 10
                    Top = 23
                    Width = 159
                    Height = 22
                    Hint = 'B11'
                    Caption = '[B11] Enable'
                    TabOrder = 0
                  end
                  object edB11PATServerPath: TEdit
                    Left = 56
                    Top = 48
                    Width = 405
                    Height = 24
                    TabOrder = 1
                  end
                end
              end
            end
          end
          object MemoB: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ScrollBars = ssVertical
            TabOrder = 1
          end
        end
        object tsC00: TTabSheet
          Caption = 'C [ Hardware ]'
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImageIndex = 1
          ParentFont = False
          object MemoC: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pgC00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = tsC_10
            Align = alClient
            TabIndex = 1
            TabOrder = 0
            object tsC_00: TTabSheet
              Caption = '[C01] - [C10]'
              object pal_C1: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object lblC09: TLabel
                  Left = 56
                  Top = 614
                  Width = 255
                  Height = 16
                  Caption = 'Will turn off after                                minutes on.'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  Visible = False
                end
                object cbC02: TCheckBox
                  Left = 5
                  Top = 29
                  Width = 400
                  Height = 17
                  Caption = '[C02] Enable CCD'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 1
                  Visible = False
                end
                object cbC03: TCheckBox
                  Left = 5
                  Top = 52
                  Width = 400
                  Height = 17
                  Caption = '[C03] Use catch tray hardware'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 2
                end
                object cbC04: TCheckBox
                  Left = 5
                  Top = 75
                  Width = 352
                  Height = 17
                  Caption = '[C04] Enable test temp  IC'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 3
                end
                object gbC05: TGroupBox
                  Left = 4
                  Top = 100
                  Width = 449
                  Height = 221
                  Caption = '[C05] Use power saving mode'
                  TabOrder = 4
                  object rgC05: TRadioGroup
                    Left = 261
                    Top = 18
                    Width = 186
                    Height = 201
                    Align = alClient
                    Caption = 'Mode'
                    ItemIndex = 0
                    Items.Strings = (
                      'No message'
                      'Show comfirm message')
                    TabOrder = 1
                  end
                  object bgC05_1: TGroupBox
                    Left = 2
                    Top = 18
                    Width = 259
                    Height = 201
                    Align = alLeft
                    Caption = '[C05-1] Module'
                    TabOrder = 0
                    object labC05_1: TLabel
                      Left = 23
                      Top = 34
                      Width = 55
                      Height = 16
                      Caption = 'Halt time:'
                      Visible = False
                    end
                    object labC05_3: TLabel
                      Left = 23
                      Top = 78
                      Width = 55
                      Height = 16
                      Caption = 'Halt time:'
                      Visible = False
                    end
                    object labC05_5: TLabel
                      Left = 23
                      Top = 122
                      Width = 55
                      Height = 16
                      Caption = 'Halt time:'
                      Visible = False
                    end
                    object labC05_7: TLabel
                      Left = 23
                      Top = 166
                      Width = 55
                      Height = 16
                      Caption = 'Halt time:'
                      Visible = False
                    end
                    object labC05_2: TLabel
                      Left = 144
                      Top = 34
                      Width = 106
                      Height = 16
                      Caption = 'Max:200 min / Unit'
                    end
                    object labC05_4: TLabel
                      Left = 144
                      Top = 78
                      Width = 106
                      Height = 16
                      Caption = 'Max:200 min / Unit'
                    end
                    object labC05_6: TLabel
                      Left = 144
                      Top = 122
                      Width = 106
                      Height = 16
                      Caption = 'Max:200 min / Unit'
                    end
                    object labC05_8: TLabel
                      Left = 144
                      Top = 166
                      Width = 106
                      Height = 16
                      Caption = 'Max:200 min / Unit'
                    end
                    object cbC05_Temp: TCheckBox
                      Left = 9
                      Top = 58
                      Width = 160
                      Height = 17
                      Caption = ' Enable temp module'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 2
                    end
                    object cbC05_Motor: TCheckBox
                      Left = 9
                      Top = 14
                      Width = 160
                      Height = 17
                      Caption = ' Enable motor module'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 0
                    end
                    object edC05_Motor: TEdit
                      Left = 88
                      Top = 30
                      Width = 49
                      Height = 24
                      ImeMode = imClose
                      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                      TabOrder = 1
                      Text = '0'
                    end
                    object edC05_Temp: TEdit
                      Left = 88
                      Top = 74
                      Width = 49
                      Height = 24
                      ImeMode = imClose
                      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                      TabOrder = 3
                      Text = '0'
                    end
                    object cbC05_Vacuum: TCheckBox
                      Left = 9
                      Top = 102
                      Width = 160
                      Height = 17
                      Caption = ' Enable vacuum pump'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 4
                    end
                    object edC05_Vacuum: TEdit
                      Left = 88
                      Top = 120
                      Width = 49
                      Height = 24
                      ImeMode = imClose
                      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                      TabOrder = 5
                      Text = '0'
                    end
                    object cbC05_ATC: TCheckBox
                      Left = 9
                      Top = 146
                      Width = 160
                      Height = 17
                      Caption = ' Enable ATC Saving'
                      Color = 12761254
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clBlack
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentColor = False
                      ParentFont = False
                      TabOrder = 6
                    end
                    object edC05_ATC: TEdit
                      Left = 88
                      Top = 164
                      Width = 49
                      Height = 24
                      ImeMode = imClose
                      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                      TabOrder = 7
                      Text = '0'
                    end
                  end
                end
                object gbC06: TGroupBox
                  Left = 8
                  Top = 327
                  Width = 209
                  Height = 169
                  Caption = '[C06] By pass ionizer'
                  TabOrder = 5
                  Visible = False
                  object cbC06_01: TCheckBox
                    Left = 8
                    Top = 24
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 1'
                    TabOrder = 0
                  end
                  object cbC06_02: TCheckBox
                    Left = 8
                    Top = 47
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 2'
                    TabOrder = 2
                  end
                  object cbC06_03: TCheckBox
                    Left = 8
                    Top = 70
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 3'
                    TabOrder = 4
                  end
                  object cbC06_04: TCheckBox
                    Left = 8
                    Top = 94
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 4'
                    TabOrder = 6
                  end
                  object cbC06_05: TCheckBox
                    Left = 8
                    Top = 117
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 5'
                    TabOrder = 8
                  end
                  object cbC06_06: TCheckBox
                    Left = 8
                    Top = 140
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 6'
                    TabOrder = 10
                  end
                  object cbC06_07: TCheckBox
                    Left = 116
                    Top = 24
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 7'
                    TabOrder = 1
                  end
                  object cbC06_08: TCheckBox
                    Left = 116
                    Top = 47
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 8'
                    TabOrder = 3
                  end
                  object cbC06_09: TCheckBox
                    Left = 116
                    Top = 70
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 9'
                    TabOrder = 5
                  end
                  object cbC06_10: TCheckBox
                    Left = 116
                    Top = 94
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 10'
                    TabOrder = 7
                  end
                  object cbC06_11: TCheckBox
                    Left = 116
                    Top = 117
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 11'
                    TabOrder = 9
                  end
                  object cbC06_12: TCheckBox
                    Left = 116
                    Top = 140
                    Width = 80
                    Height = 17
                    Caption = 'Ion Fan 12'
                    TabOrder = 11
                  end
                end
                object cbC07: TCheckBox
                  Left = 5
                  Top = 509
                  Width = 400
                  Height = 17
                  Caption = '[C07] Disable OCR with tester (No save just testing)'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 6
                  Visible = False
                end
                object cbC08: TCheckBox
                  Left = 5
                  Top = 538
                  Width = 400
                  Height = 17
                  Caption = '[C08] Use socket sensor '
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 7
                end
                object edC09: TEdit
                  Left = 153
                  Top = 610
                  Width = 76
                  Height = 24
                  OEMConvert = True
                  TabOrder = 10
                  Visible = False
                end
                object cbC09: TCheckBox
                  Left = 5
                  Top = 586
                  Width = 390
                  Height = 15
                  Caption = '[C09] Active car recorder after jam happened'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                  Visible = False
                end
                object cbC10: TCheckBox
                  Left = 5
                  Top = 638
                  Width = 400
                  Height = 17
                  Caption = '[C10] Enable ESD connect error report function'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 11
                end
                object cbC01: TCheckBox
                  Left = 5
                  Top = 6
                  Width = 400
                  Height = 17
                  Caption = '[C01] Fan Direction'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 0
                end
                object cbC08_1: TCheckBox
                  Left = 41
                  Top = 558
                  Width = 400
                  Height = 17
                  Caption = '[C08_1] Every Start Check Function is ON'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 8
                end
              end
            end
            object tsC_10: TTabSheet
              Caption = '[C11] - [C20]'
              ImageIndex = 1
              object pal_C2: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object Label5: TLabel
                  Left = 52
                  Top = 120
                  Width = 274
                  Height = 16
                  Caption = 'cleaning interval times                                minutes'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  Visible = False
                end
                object cbC11: TCheckBox
                  Left = 5
                  Top = 4
                  Width = 240
                  Height = 17
                  Caption = '[C11] Use Monitor Video(For TCP/IP)'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 0
                end
                object cbC13: TCheckBox
                  Left = 5
                  Top = 49
                  Width = 348
                  Height = 17
                  Caption = '[C13] Need to restart GroundMan  when initial start'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 2
                  OnClick = cbC12Click
                end
                object cbC12: TCheckBox
                  Left = 5
                  Top = 25
                  Width = 240
                  Height = 17
                  Caption = '[C12] Use PE Mode'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 1
                  OnClick = cbC12Click
                end
                object cbC15: TCheckBox
                  Left = 5
                  Top = 97
                  Width = 390
                  Height = 15
                  Caption = '[C15] Alarm for Ion Fan Cleaning'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                  Visible = False
                end
                object cbC16: TCheckBox
                  Left = 5
                  Top = 145
                  Width = 390
                  Height = 15
                  Caption = '[C16] Use barcoder reader change setup file'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                  Visible = False
                end
                object edtC15: TEdit
                  Left = 201
                  Top = 118
                  Width = 76
                  Height = 24
                  OEMConvert = True
                  TabOrder = 5
                  Visible = False
                end
                object chkC14: TCheckBox
                  Left = 5
                  Top = 73
                  Width = 348
                  Height = 17
                  Caption = '[C14] Save communication log of bin display'
                  Color = 12761254
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                  TabOrder = 3
                  OnClick = cbC12Click
                end
                object grpC19: TGroupBox
                  Left = 4
                  Top = 279
                  Width = 450
                  Height = 206
                  Hint = 'C19_gb'
                  Caption = '[C19] Dew point parameter setting'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 7
                  Visible = False
                  object lblC19_3: TLabel
                    Left = 8
                    Top = 85
                    Width = 254
                    Height = 16
                    Hint = 'M02'
                    Caption = '[19-3] Check Temperature Range              '#176'C'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object lblC19_4: TLabel
                    Left = 8
                    Top = 115
                    Width = 322
                    Height = 16
                    Hint = 'M02'
                    Caption = '[19-4] Below               degrees do not use energy saving'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object lblC19_5: TLabel
                    Left = 8
                    Top = 145
                    Width = 343
                    Height = 16
                    Hint = 'M02'
                    Caption = '[19-5] Over                  degree strong opening energy saving'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object cbC19_1: TCheckBox
                    Left = 8
                    Top = 30
                    Width = 357
                    Height = 17
                    Caption = '[19-1] Use Dew Point Meter'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                  end
                  object edC19_3: TEdit
                    Left = 210
                    Top = 79
                    Width = 36
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    Text = '3'
                  end
                  object cbC19_2: TCheckBox
                    Left = 8
                    Top = 60
                    Width = 400
                    Height = 17
                    Caption = '[19-2] Enable Energy - Saving Dry Air'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                  end
                  object cbC19_6: TCheckBox
                    Left = 8
                    Top = 173
                    Width = 405
                    Height = 17
                    Caption = '[19-6]  Index Area Use Dew Point Sensor'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 5
                  end
                  object edC19_4: TEdit
                    Left = 88
                    Top = 109
                    Width = 35
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                    Text = '0'
                  end
                  object edC19_5: TEdit
                    Left = 88
                    Top = 140
                    Width = 35
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                    Text = '131'
                  end
                end
                object GroupBox4: TGroupBox
                  Left = 8
                  Top = 489
                  Width = 450
                  Height = 128
                  Hint = 'C19_gb'
                  Caption = '[C20] Dew point parameter setting'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 8
                  Visible = False
                  object Label22: TLabel
                    Left = 8
                    Top = 46
                    Width = 322
                    Height = 16
                    Hint = 'M02'
                    Caption = '[20-2] Below               degrees do not use energy saving'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object Label23: TLabel
                    Left = 8
                    Top = 76
                    Width = 343
                    Height = 16
                    Hint = 'M02'
                    Caption = '[20-3] Over                  degree strong opening energy saving'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object cbC20_4: TCheckBox
                    Left = 8
                    Top = 104
                    Width = 405
                    Height = 17
                    Caption = '[20-4] Index Area Use Frost Sensor'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 3
                  end
                  object edC20_2: TEdit
                    Left = 88
                    Top = 40
                    Width = 35
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    Text = '0'
                  end
                  object edC20_3: TEdit
                    Left = 88
                    Top = 71
                    Width = 35
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    Text = '131'
                  end
                  object cbC20_1: TCheckBox
                    Left = 8
                    Top = 20
                    Width = 400
                    Height = 17
                    Caption = '[20-1] Enable Energy - Saving Dry Air'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 0
                  end
                end
                object grpC17: TGroupBox
                  Left = 4
                  Top = 165
                  Width = 465
                  Height = 92
                  Caption = '[C17]  Loader Color Sensor'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 9
                  object lbC17DelayTime: TLabel
                    Left = 64
                    Top = 40
                    Width = 176
                    Height = 16
                    Caption = 'Delay times                           Sec'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object cbC17: TCheckBox
                    Left = 13
                    Top = 22
                    Width = 240
                    Height = 17
                    Caption = '[C17] Use Loader Color Sensor'
                    Color = 12761254
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    OnClick = cbC12Click
                  end
                  object edtC17: TEdit
                    Left = 137
                    Top = 40
                    Width = 76
                    Height = 24
                    OEMConvert = True
                    TabOrder = 1
                    Visible = False
                  end
                  object cbC17_1: TCheckBox
                    Left = 13
                    Top = 70
                    Width = 133
                    Height = 17
                    Caption = '[C17-1]Skip Alarm'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                  end
                end
              end
            end
            object tsC21: TTabSheet
              Caption = '[C21]'
              ImageIndex = 2
              object grpC21: TGroupBox
                Left = 0
                Top = 0
                Width = 471
                Height = 297
                Align = alTop
                Caption = '[C21] Vibration motor speed'
                TabOrder = 0
                object PageControl2: TPageControl
                  Left = 2
                  Top = 18
                  Width = 467
                  Height = 277
                  ActivePage = TabSheet3
                  Align = alClient
                  TabIndex = 0
                  TabOrder = 0
                  object TabSheet3: TTabSheet
                    Caption = 'Small(<6mm)'
                    object lblC21_HP1: TLabel
                      Left = 8
                      Top = 17
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 1: '
                    end
                    object lblC21_HP2: TLabel
                      Left = 8
                      Top = 48
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 2: '
                    end
                    object lblC21_SHT1: TLabel
                      Left = 8
                      Top = 79
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 1: '
                    end
                    object lblC21_SHT2: TLabel
                      Left = 8
                      Top = 110
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 2: '
                    end
                    object lblC21_Auto1: TLabel
                      Left = 8
                      Top = 142
                      Width = 40
                      Height = 16
                      Caption = 'Auto1: '
                    end
                    object lblC21_Auto2: TLabel
                      Left = 8
                      Top = 173
                      Width = 40
                      Height = 16
                      Caption = 'Auto2: '
                    end
                    object lblC21_Auto3: TLabel
                      Left = 8
                      Top = 204
                      Width = 40
                      Height = 16
                      Caption = 'Auto3: '
                    end
                    object edtC21_HP1: TEdit
                      Left = 80
                      Top = 16
                      Width = 121
                      Height = 24
                      TabOrder = 0
                    end
                    object edtC21_HP2: TEdit
                      Left = 80
                      Top = 46
                      Width = 121
                      Height = 24
                      TabOrder = 1
                    end
                    object edtC21_SHT1: TEdit
                      Left = 80
                      Top = 77
                      Width = 121
                      Height = 24
                      TabOrder = 2
                    end
                    object edtC21_SHT2: TEdit
                      Left = 80
                      Top = 107
                      Width = 121
                      Height = 24
                      TabOrder = 3
                    end
                    object edtC21_Auto1: TEdit
                      Left = 80
                      Top = 137
                      Width = 121
                      Height = 24
                      TabOrder = 4
                    end
                    object edtC21_Auto2: TEdit
                      Left = 80
                      Top = 168
                      Width = 121
                      Height = 24
                      TabOrder = 5
                    end
                    object edtC21_Auto3: TEdit
                      Left = 80
                      Top = 198
                      Width = 121
                      Height = 24
                      TabOrder = 6
                    end
                  end
                  object TabSheet4: TTabSheet
                    Caption = 'Mid(6~20mm)'
                    ImageIndex = 1
                    object lblC21_HP1_m: TLabel
                      Left = 8
                      Top = 17
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 1: '
                    end
                    object lblC21_HP2_m: TLabel
                      Left = 8
                      Top = 48
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 2: '
                    end
                    object lblC21_SHT1_m: TLabel
                      Left = 8
                      Top = 79
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 1: '
                    end
                    object lblC21_SHT2_m: TLabel
                      Left = 8
                      Top = 110
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 2: '
                    end
                    object lblC21_Auto1_m: TLabel
                      Left = 8
                      Top = 142
                      Width = 40
                      Height = 16
                      Caption = 'Auto1: '
                    end
                    object lblC21_Auto2_m: TLabel
                      Left = 8
                      Top = 173
                      Width = 40
                      Height = 16
                      Caption = 'Auto2: '
                    end
                    object lblC21_Auto3_m: TLabel
                      Left = 8
                      Top = 204
                      Width = 40
                      Height = 16
                      Caption = 'Auto3: '
                    end
                    object edtC21_HP1_m: TEdit
                      Left = 80
                      Top = 16
                      Width = 121
                      Height = 24
                      TabOrder = 0
                    end
                    object edtC21_HP2_m: TEdit
                      Left = 80
                      Top = 46
                      Width = 121
                      Height = 24
                      TabOrder = 1
                    end
                    object edtC21_SHT1_m: TEdit
                      Left = 80
                      Top = 77
                      Width = 121
                      Height = 24
                      TabOrder = 2
                    end
                    object edtC21_SHT2_m: TEdit
                      Left = 80
                      Top = 107
                      Width = 121
                      Height = 24
                      TabOrder = 3
                    end
                    object edtC21_Auto1_m: TEdit
                      Left = 80
                      Top = 137
                      Width = 121
                      Height = 24
                      TabOrder = 4
                    end
                    object edtC21_Auto2_m: TEdit
                      Left = 80
                      Top = 168
                      Width = 121
                      Height = 24
                      TabOrder = 5
                    end
                    object edtC21_Auto3_m: TEdit
                      Left = 80
                      Top = 198
                      Width = 121
                      Height = 24
                      TabOrder = 6
                    end
                  end
                  object TabSheet5: TTabSheet
                    Caption = 'Big(>20mm)'
                    ImageIndex = 2
                    object lblC21_HP1_b: TLabel
                      Left = 8
                      Top = 17
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 1: '
                    end
                    object lblC21_HP2_b: TLabel
                      Left = 8
                      Top = 48
                      Width = 70
                      Height = 16
                      Caption = 'Hot plate 2: '
                    end
                    object lblC21_SHT1_b: TLabel
                      Left = 8
                      Top = 79
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 1: '
                    end
                    object lblC21_SHT2_b: TLabel
                      Left = 8
                      Top = 110
                      Width = 56
                      Height = 16
                      Caption = 'Shuttle 2: '
                    end
                    object lblC21_Auto1_b: TLabel
                      Left = 8
                      Top = 142
                      Width = 40
                      Height = 16
                      Caption = 'Auto1: '
                    end
                    object lblC21_Auto2_b: TLabel
                      Left = 8
                      Top = 173
                      Width = 40
                      Height = 16
                      Caption = 'Auto2: '
                    end
                    object lblC21_Auto3_b: TLabel
                      Left = 8
                      Top = 204
                      Width = 40
                      Height = 16
                      Caption = 'Auto3: '
                    end
                    object edtC21_HP1_b: TEdit
                      Left = 80
                      Top = 16
                      Width = 121
                      Height = 24
                      TabOrder = 0
                    end
                    object edtC21_HP2_b: TEdit
                      Left = 80
                      Top = 46
                      Width = 121
                      Height = 24
                      TabOrder = 1
                    end
                    object edtC21_SHT1_b: TEdit
                      Left = 80
                      Top = 77
                      Width = 121
                      Height = 24
                      TabOrder = 2
                    end
                    object edtC21_SHT2_b: TEdit
                      Left = 80
                      Top = 107
                      Width = 121
                      Height = 24
                      TabOrder = 3
                    end
                    object edtC21_Auto1_b: TEdit
                      Left = 80
                      Top = 137
                      Width = 121
                      Height = 24
                      TabOrder = 4
                    end
                    object edtC21_Auto2_b: TEdit
                      Left = 80
                      Top = 168
                      Width = 121
                      Height = 24
                      TabOrder = 5
                    end
                    object edtC21_Auto3_b: TEdit
                      Left = 80
                      Top = 198
                      Width = 121
                      Height = 24
                      TabOrder = 6
                    end
                  end
                end
              end
            end
          end
        end
        object tsD00: TTabSheet
          Caption = 'D [ Index ]'
          ImageIndex = 4
          object pcD00: TPageControl
            Left = 0
            Top = 0
            Width = 437
            Height = 737
            ActivePage = tsD_80
            Align = alLeft
            TabIndex = 8
            TabOrder = 0
            TabWidth = 60
            object tsD_Preasure: TTabSheet
              Caption = 'Preasure'
              object pal_D1: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labD03_1: TLabel
                  Left = 23
                  Top = 171
                  Width = 186
                  Height = 20
                  AutoSize = False
                  Caption = '[D03] Max pressure limit value'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  Visible = False
                end
                object labD03_2: TLabel
                  Left = 152
                  Top = 32
                  Width = 210
                  Height = 16
                  Caption = 'after contact                       read torque'
                end
                object lblD04_30mm: TLabel
                  Left = 100
                  Top = 284
                  Width = 94
                  Height = 16
                  Caption = '30mm Diameter'
                end
                object lblD04_40mm: TLabel
                  Left = 100
                  Top = 311
                  Width = 94
                  Height = 16
                  Caption = '40mm Diameter'
                end
                object lblD04_60mm: TLabel
                  Left = 100
                  Top = 339
                  Width = 94
                  Height = 16
                  Caption = '60mm Diameter'
                end
                object lblD04_Default: TLabel
                  Left = 100
                  Top = 228
                  Width = 80
                  Height = 16
                  Caption = 'Default Value'
                end
                object lblD04_20mm: TLabel
                  Left = 100
                  Top = 256
                  Width = 94
                  Height = 16
                  Caption = '20mm Diameter'
                end
                object lblD04_80mm: TLabel
                  Left = 100
                  Top = 368
                  Width = 94
                  Height = 16
                  Caption = '80mm Diameter'
                end
                object cbD01: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 232
                  Height = 17
                  Caption = '[D01] Enable read torque  (1/per)'
                  TabOrder = 0
                end
                object cbD02: TCheckBox
                  Left = 5
                  Top = 141
                  Width = 350
                  Height = 17
                  Caption = '[D02] Off read torque function during test'
                  TabOrder = 4
                end
                object edD01: TEdit
                  Left = 232
                  Top = 28
                  Width = 49
                  Height = 24
                  TabOrder = 1
                end
                object coD03: TComboBox
                  Left = 209
                  Top = 167
                  Width = 104
                  Height = 24
                  ItemHeight = 16
                  ItemIndex = 0
                  TabOrder = 5
                  Text = '[1]   70'
                  Visible = False
                  Items.Strings = (
                    '[1]   70'
                    '[2]   80'
                    '[3]   90')
                end
                object cbD04: TCheckBox
                  Left = 5
                  Top = 201
                  Width = 208
                  Height = 17
                  Caption = '[D04] Min force is set by file.'
                  TabOrder = 6
                end
                object palD01: TPanel
                  Left = 12
                  Top = 54
                  Width = 379
                  Height = 63
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 2
                  object lblD01_1: TLabel
                    Left = 15
                    Top = 9
                    Width = 227
                    Height = 16
                    Caption = 'Over torque Range Error                      % '
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object lblD01_2: TLabel
                    Left = 15
                    Top = 33
                    Width = 228
                    Height = 16
                    Caption = 'Delay Read Time                                 sec'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object edD01_Xilinx: TEdit
                    Left = 167
                    Top = 4
                    Width = 46
                    Height = 24
                    TabOrder = 0
                  end
                  object edD01DelayTime_Xilinx: TEdit
                    Left = 167
                    Top = 30
                    Width = 46
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    TabOrder = 1
                  end
                end
                object cbD05: TCheckBox
                  Left = 5
                  Top = 401
                  Width = 208
                  Height = 17
                  Caption = '[D05] Contact Count Alarm'
                  TabOrder = 14
                end
                object edD05: TEdit
                  Left = 216
                  Top = 397
                  Width = 50
                  Height = 24
                  TabOrder = 13
                end
                object cbD01_1: TCheckBox
                  Left = 3
                  Top = 118
                  Width = 284
                  Height = 17
                  Caption = '[D01_1] Enable read and check torque '
                  TabOrder = 3
                end
                object cbD06: TCheckBox
                  Left = 5
                  Top = 529
                  Width = 220
                  Height = 17
                  Caption = '[D06] Contact Offset Default Value'
                  TabOrder = 17
                end
                object edD06: TEdit
                  Left = 230
                  Top = 525
                  Width = 50
                  Height = 24
                  TabOrder = 16
                end
                object edtD04: TEdit
                  Left = 204
                  Top = 224
                  Width = 50
                  Height = 24
                  TabOrder = 7
                end
                object edtD04_40mm: TEdit
                  Left = 204
                  Top = 307
                  Width = 50
                  Height = 24
                  TabOrder = 10
                end
                object edtD04_60mm: TEdit
                  Left = 204
                  Top = 335
                  Width = 50
                  Height = 24
                  TabOrder = 11
                end
                object edtD04_30mm: TEdit
                  Left = 204
                  Top = 280
                  Width = 50
                  Height = 24
                  TabOrder = 9
                end
                object edtD04_20mm: TEdit
                  Left = 204
                  Top = 252
                  Width = 50
                  Height = 24
                  TabOrder = 8
                end
                object edtD04_80mm: TEdit
                  Left = 204
                  Top = 364
                  Width = 50
                  Height = 24
                  TabOrder = 12
                end
                object cbD05_1: TCheckBox
                  Left = 13
                  Top = 433
                  Width = 324
                  Height = 17
                  Caption = '[D05-1] Save socket count by machine'
                  TabOrder = 15
                end
              end
            end
            object tsD_HighCal: TTabSheet
              Caption = 'High Cal'
              ImageIndex = 1
              object pal_D2: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbD10: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 350
                  Height = 17
                  Caption = '[D10] Manual height use Z1, Z2 button to UP/Down'
                  TabOrder = 0
                end
                object cbD11: TCheckBox
                  Left = 5
                  Top = 39
                  Width = 350
                  Height = 17
                  Caption = '[D11] If shuttle no device, no need to do auto height.'
                  TabOrder = 1
                end
                object cbD12: TCheckBox
                  Left = 5
                  Top = 69
                  Width = 350
                  Height = 17
                  Caption = '[D12] Shuttle auto height by setting force value'
                  TabOrder = 2
                end
                object cdD13: TCheckBox
                  Left = 5
                  Top = 98
                  Width = 350
                  Height = 17
                  Caption = '[D13] Check index home sensor after find home.'
                  TabOrder = 3
                end
                object cbD14: TCheckBox
                  Left = 5
                  Top = 126
                  Width = 388
                  Height = 17
                  Caption = '[D14] Auto height use setting torque (For > 300KG Model)'
                  TabOrder = 4
                end
                object edD14: TEdit
                  Left = 268
                  Top = 147
                  Width = 121
                  Height = 24
                  TabOrder = 5
                end
                object cbD15: TCheckBox
                  Left = 5
                  Top = 178
                  Width = 350
                  Height = 17
                  Caption = '[D15] Continuous auto contact test'
                  TabOrder = 6
                end
                object cbD16: TCheckBox
                  Left = 5
                  Top = 204
                  Width = 350
                  Height = 17
                  Caption = '[D16] Step by step contact test'
                  TabOrder = 7
                end
                object chkD18: TCheckBox
                  Left = 5
                  Top = 418
                  Width = 384
                  Height = 17
                  Caption = '[D18] Notice to check contact height when change recipe.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 10
                end
                object rgD17: TRadioGroup
                  Left = 4
                  Top = 260
                  Width = 417
                  Height = 149
                  Caption = '[D17] Auto Height Method'
                  ItemIndex = 2
                  Items.Strings = (
                    'Normal'
                    'Get hardware height with Full EP in high calibration.'
                    'Get contact height without EP in high calibration.'
                    'Check EP have leakage or not')
                  TabOrder = 8
                end
                object edtD17_3: TEdit
                  Left = 276
                  Top = 376
                  Width = 121
                  Height = 24
                  TabOrder = 9
                  Text = '1'
                end
              end
            end
            object tsD_Contact: TTabSheet
              Caption = 'Contact'
              ImageIndex = 2
              object pal_D3: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labD21_2: TLabel
                  Left = 365
                  Top = 8
                  Width = 22
                  Height = 16
                  Caption = 'sec'
                end
                object labD21_1: TLabel
                  Left = 289
                  Top = 8
                  Width = 22
                  Height = 16
                  Caption = 'mm'
                end
                object cbD21: TCheckBox
                  Left = 5
                  Top = 8
                  Width = 228
                  Height = 17
                  Caption = '[D21] Enable finish test up && wait'
                  TabOrder = 2
                end
                object edD21_mm: TEdit
                  Left = 236
                  Top = 4
                  Width = 50
                  Height = 24
                  TabOrder = 0
                  Text = '0'
                end
                object edD21_Sec: TEdit
                  Left = 313
                  Top = 4
                  Width = 50
                  Height = 24
                  TabOrder = 1
                  Text = '0'
                end
                object cbD24: TCheckBox
                  Left = 5
                  Top = 195
                  Width = 244
                  Height = 17
                  Caption = '[D24] Enable EP check function'
                  TabOrder = 7
                  Visible = False
                end
                object cbD26: TCheckBox
                  Left = 9
                  Top = 553
                  Width = 240
                  Height = 17
                  Caption = '[D26] Enable EP encoder range +,- '
                  TabOrder = 11
                end
                object edD26: TEdit
                  Left = 260
                  Top = 549
                  Width = 50
                  Height = 24
                  TabOrder = 9
                  Text = '0'
                end
                object cbD27: TCheckBox
                  Left = 9
                  Top = 619
                  Width = 212
                  Height = 17
                  Caption = '[D27] Enable single site 85 kg'
                  TabOrder = 16
                end
                object cbD29: TCheckBox
                  Left = 9
                  Top = 660
                  Width = 350
                  Height = 17
                  Caption = '[D29] Stable contact mode'
                  TabOrder = 18
                end
                object gbD25: TGroupBox
                  Left = 4
                  Top = 216
                  Width = 385
                  Height = 329
                  Caption = '[D25]EP load rate :'
                  Color = 12761254
                  ParentColor = False
                  TabOrder = 8
                  object labD25_1: TLabel
                    Left = 2
                    Top = 31
                    Width = 45
                    Height = 16
                    Caption = '60mm : '
                  end
                  object labD25_3: TLabel
                    Left = 2
                    Top = 235
                    Width = 45
                    Height = 16
                    Caption = '30mm : '
                  end
                  object labD25_2: TLabel
                    Left = 2
                    Top = 131
                    Width = 45
                    Height = 16
                    Caption = '40mm : '
                  end
                  object labD25_4: TLabel
                    Left = 104
                    Top = 96
                    Width = 168
                    Height = 16
                    Caption = '60mm offset by heater mode'
                  end
                  object labD25_5: TLabel
                    Left = 104
                    Top = 196
                    Width = 168
                    Height = 16
                    Caption = '40mm offset by heater mode'
                  end
                  object labD25_6: TLabel
                    Left = 104
                    Top = 304
                    Width = 168
                    Height = 16
                    Caption = '30mm offset by heater mode'
                  end
                  object labD25_1_NS: TLabel
                    Left = 2
                    Top = 63
                    Width = 82
                    Height = 16
                    Caption = '60mm for NS: '
                  end
                  object labD25_2_NS: TLabel
                    Left = 2
                    Top = 163
                    Width = 82
                    Height = 16
                    Caption = '40mm for NS: '
                  end
                  object labD25_3_NS: TLabel
                    Left = 2
                    Top = 271
                    Width = 82
                    Height = 16
                    Caption = '30mm for NS: '
                  end
                  object tbD25_Index60mm: TTrackBar
                    Left = 108
                    Top = 26
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 0
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index60mmChange
                  end
                  object tbD25_Index30mm: TTrackBar
                    Left = 108
                    Top = 230
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 6
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index30mmChange
                  end
                  object tbD25_Index40mm: TTrackBar
                    Left = 108
                    Top = 126
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 3
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index40mmChange
                  end
                  object edD25_60mm: TEdit
                    Left = 282
                    Top = 92
                    Width = 80
                    Height = 24
                    TabOrder = 2
                    OnClick = edD25_60mmClick
                  end
                  object edD25_40mm: TEdit
                    Left = 282
                    Top = 192
                    Width = 80
                    Height = 24
                    TabOrder = 5
                    OnClick = edD25_60mmClick
                  end
                  object edD25_30mm: TEdit
                    Left = 282
                    Top = 300
                    Width = 80
                    Height = 24
                    TabOrder = 8
                    OnClick = edD25_60mmClick
                  end
                  object tbD25_Index60mm_NS: TTrackBar
                    Left = 108
                    Top = 58
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 1
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index60mm_NSChange
                  end
                  object tbD25_Index40mm_NS: TTrackBar
                    Left = 108
                    Top = 158
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 4
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index40mm_NSChange
                  end
                  object tbD25_Index30mm_NS: TTrackBar
                    Left = 108
                    Top = 266
                    Width = 275
                    Height = 35
                    Max = 115
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 7
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD25_Index30mm_NSChange
                  end
                end
                object cbD23: TCheckBox
                  Left = 5
                  Top = 170
                  Width = 308
                  Height = 17
                  Caption = '[D23] Every device do multi contact before test.'
                  TabOrder = 5
                end
                object cbD26_1: TCheckBox
                  Left = 29
                  Top = 577
                  Width = 164
                  Height = 17
                  Caption = '[D26_1] Enable EP log'
                  TabOrder = 12
                end
                object edD23: TEdit
                  Left = 320
                  Top = 166
                  Width = 57
                  Height = 24
                  TabOrder = 4
                  Text = '0'
                end
                object cbD26_2: TCheckBox
                  Left = 193
                  Top = 577
                  Width = 192
                  Height = 17
                  Caption = '[D26_2] Show EP encoder '
                  TabOrder = 13
                end
                object btnOpenEP: TButton
                  Left = 310
                  Top = 191
                  Width = 75
                  Height = 25
                  Caption = 'OpenEP'
                  TabOrder = 6
                  OnClick = btnOpenEPClick
                end
                object grpD22: TGroupBox
                  Left = 4
                  Top = 32
                  Width = 389
                  Height = 133
                  Caption = '[D22] Double Contact Function'
                  TabOrder = 3
                  object cbD22_1: TCheckBox
                    Left = 9
                    Top = 23
                    Width = 232
                    Height = 17
                    Caption = '[D22-1] Support multi double contact'
                    TabOrder = 1
                  end
                  object coD22: TComboBox
                    Left = 256
                    Top = 20
                    Width = 115
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    ItemHeight = 16
                    TabOrder = 0
                    Text = '2'
                    Items.Strings = (
                      '2'
                      '3'
                      '4'
                      '5'
                      '6'
                      '7'
                      '8'
                      '9'
                      '10')
                  end
                  object cbD22_2: TCheckBox
                    Left = 9
                    Top = 52
                    Width = 276
                    Height = 17
                    Caption = '[D22-2] Double contact no need re-contact'
                    TabOrder = 2
                  end
                  object cbD22_3: TCheckBox
                    Left = 288
                    Top = 52
                    Width = 96
                    Height = 17
                    Caption = 'Verify Mode'
                    TabOrder = 3
                  end
                  object cbD22_3_: TCheckBox
                    Left = 9
                    Top = 80
                    Width = 276
                    Height = 17
                    Caption = '[D22-3] Double contact use diff. SRQ'
                    TabOrder = 5
                  end
                  object coD22_3: TComboBox
                    Left = 256
                    Top = 76
                    Width = 115
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    ItemHeight = 16
                    TabOrder = 4
                    Text = '0x42'
                    Items.Strings = (
                      '0x42'
                      '0x43'
                      '0xC1')
                  end
                  object cbD22_4: TCheckBox
                    Left = 9
                    Top = 108
                    Width = 276
                    Height = 17
                    Caption = '[D22-4] Double contact can set pass bin'
                    TabOrder = 6
                  end
                end
                object cbD28: TCheckBox
                  Left = 9
                  Top = 639
                  Width = 350
                  Height = 17
                  Caption = '[D28] Maximum contact force limitation by diameter.'
                  TabOrder = 17
                end
                object coD26: TComboBox
                  Left = 312
                  Top = 549
                  Width = 89
                  Height = 24
                  ItemHeight = 16
                  TabOrder = 10
                  Text = 'Fix Value'
                  Items.Strings = (
                    'Fix Value'
                    'Percentage')
                end
                object cbD26_3: TCheckBox
                  Left = 9
                  Top = 600
                  Width = 344
                  Height = 17
                  Caption = '[D26_3] Enable Dual EP encoder range +,-                   Kpa'
                  TabOrder = 15
                end
                object edD26_3: TEdit
                  Left = 282
                  Top = 596
                  Width = 50
                  Height = 24
                  TabOrder = 14
                  Text = '0'
                end
              end
            end
            object tsD_Mode: TTabSheet
              Caption = 'Mode'
              ImageIndex = 3
              object pal_D4: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbD30: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 350
                  Height = 17
                  Caption = '[D30] Enable site mode select'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbD31: TCheckBox
                  Left = 5
                  Top = 36
                  Width = 350
                  Height = 17
                  Caption = '[D31] RTC change recipe need re-create RTC model'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbD32: TCheckBox
                  Left = 5
                  Top = 63
                  Width = 388
                  Height = 17
                  Caption = '[D32] Tray pitch > 35mm, the counter air on time must >0.5 Sec.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbD33: TCheckBox
                  Left = 5
                  Top = 89
                  Width = 350
                  Height = 17
                  Caption = '[D33] RTC initial start need verify'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                end
                object cbD34: TCheckBox
                  Left = 5
                  Top = 115
                  Width = 350
                  Height = 17
                  Caption = '[D34] Enable galil protection function.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                end
                object cbD35: TCheckBox
                  Left = 5
                  Top = 142
                  Width = 350
                  Height = 17
                  Caption = '[D35] RTC need check site number'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object cbD36: TCheckBox
                  Left = 5
                  Top = 168
                  Width = 350
                  Height = 17
                  Caption = '[D36] RTC auto model verify'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                  OnClick = cbD36Click
                end
                object cbD37: TCheckBox
                  Left = 5
                  Top = 247
                  Width = 350
                  Height = 17
                  Caption = '[D37] Manual process'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                  OnClick = cbD36Click
                end
                object cbD38: TCheckBox
                  Left = 5
                  Top = 273
                  Width = 350
                  Height = 17
                  Caption = '[D38] Index release device to shuttle no wait motion.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                  OnClick = cbD36Click
                end
                object cbD36_1: TCheckBox
                  Left = 21
                  Top = 194
                  Width = 350
                  Height = 17
                  Caption = '[D36_1] RTC auto model verify auto live show check'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                  OnClick = cbD36Click
                end
                object cbD36_2: TCheckBox
                  Left = 21
                  Top = 220
                  Width = 350
                  Height = 17
                  Caption = '[D36_2] Trigger of RTC auto verification after  one cycle'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                  OnClick = cbD36Click
                end
              end
            end
            object tsD_Other: TTabSheet
              Caption = 'D[40]'
              ImageIndex = 4
              object pal_D5: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labD46: TLabel
                  Left = 23
                  Top = 300
                  Width = 152
                  Height = 16
                  Caption = '[D46] Index destroy delay'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clNavy
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                end
                object lblD44: TLabel
                  Left = 60
                  Top = 228
                  Width = 89
                  Height = 16
                  Caption = 'Place IC height'
                  Visible = False
                end
                object cbD40: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 388
                  Height = 15
                  Caption = '[D40] Index IC lose, need press Z1 to active skip button.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbD42: TCheckBox
                  Left = 5
                  Top = 105
                  Width = 316
                  Height = 17
                  Caption = '[D42] Index && shuttle jam need pause'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object edD44: TEdit
                  Left = 317
                  Top = 201
                  Width = 56
                  Height = 24
                  TabOrder = 6
                end
                object cbD44: TCheckBox
                  Left = 5
                  Top = 205
                  Width = 312
                  Height = 15
                  Caption = '[D44] Check vacuum after test head purge, (sec)'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbD45: TCheckBox
                  Left = 5
                  Top = 276
                  Width = 388
                  Height = 15
                  Caption = '[D45] Out arm Z should wait until Index Z goes up from shuttle'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object udD46: TUpDown
                  Left = 322
                  Top = 296
                  Width = 19
                  Height = 24
                  Min = 5
                  Max = 15
                  Position = 5
                  TabOrder = 11
                  Wrap = False
                  OnClick = udD46Click
                end
                object edD46: TEdit
                  Left = 237
                  Top = 296
                  Width = 85
                  Height = 24
                  Enabled = False
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clNavy
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  ParentFont = False
                  TabOrder = 10
                end
                object cbD48: TCheckBox
                  Left = 5
                  Top = 561
                  Width = 384
                  Height = 15
                  Caption = '[D48] Disable Z1, Z2 function when power off or EMG press'
                  TabOrder = 13
                end
                object cbD43: TCheckBox
                  Left = 5
                  Top = 131
                  Width = 316
                  Height = 17
                  Caption = '[D43] Index && shuttle jam can retry or skip'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                end
                object edD44_Height: TEdit
                  Left = 153
                  Top = 224
                  Width = 56
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  TabOrder = 8
                  Visible = False
                end
                object cbD49: TCheckBox
                  Left = 5
                  Top = 585
                  Width = 384
                  Height = 15
                  Caption = '[D49] After RTC alarm, set index devices to error bin.'
                  TabOrder = 14
                end
                object cbD43_1: TCheckBox
                  Left = 13
                  Top = 155
                  Width = 324
                  Height = 17
                  Caption = '[D43-1] Enable auto retry when index pick up error'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                end
                object cbD43_2: TCheckBox
                  Left = 13
                  Top = 179
                  Width = 380
                  Height = 17
                  Caption = '[D43-2] Check vacuum in socket when index pick up error'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object grpD47: TGroupBox
                  Left = 8
                  Top = 328
                  Width = 389
                  Height = 221
                  Caption = '[D47] Socket clean function'
                  TabOrder = 12
                  object labD47_1: TLabel
                    Left = 28
                    Top = 80
                    Width = 134
                    Height = 16
                    Caption = '[D47-3] Contact Count :'
                  end
                  object labD47_2: TLabel
                    Left = 28
                    Top = 108
                    Width = 151
                    Height = 16
                    Caption = '[D47-4] Purge Time (Sec)'
                  end
                  object labD47_3: TLabel
                    Left = 21
                    Top = 192
                    Width = 107
                    Height = 16
                    Caption = 'Current test count :'
                  end
                  object lblD47_1: TLabel
                    Left = 28
                    Top = 52
                    Width = 122
                    Height = 16
                    Caption = '[D47-2] Clean Type :'
                  end
                  object lblD47_6: TLabel
                    Left = 28
                    Top = 164
                    Width = 160
                    Height = 16
                    Caption = '[D47-6] Shuttle offset (mm) :'
                  end
                  object lblD47_5: TLabel
                    Left = 28
                    Top = 136
                    Width = 165
                    Height = 16
                    Caption = '[D47-5] Contact offset (mm) :'
                  end
                  object cbD47: TCheckBox
                    Left = 9
                    Top = 25
                    Width = 336
                    Height = 15
                    Caption = '[D47-1] Enable socket clean function'
                    TabOrder = 0
                  end
                  object edD47_Count: TEdit
                    Left = 204
                    Top = 76
                    Width = 81
                    Height = 24
                    TabOrder = 2
                  end
                  object edD47_Time: TEdit
                    Left = 204
                    Top = 104
                    Width = 81
                    Height = 24
                    TabOrder = 3
                  end
                  object edD47_3: TEdit
                    Left = 136
                    Top = 188
                    Width = 142
                    Height = 24
                    Enabled = False
                    TabOrder = 7
                  end
                  object btD47: TButton
                    Left = 287
                    Top = 184
                    Width = 94
                    Height = 31
                    Caption = 'Clear'
                    TabOrder = 6
                    OnClick = btD47Click
                  end
                  object cbbD47: TComboBox
                    Left = 156
                    Top = 48
                    Width = 213
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 1
                    TabOrder = 1
                    Text = 'Type 2 Socket & Shuttle'
                    Items.Strings = (
                      'Type 1 Socket'
                      'Type 2 Socket & Shuttle')
                  end
                  object edtD47_5: TEdit
                    Left = 204
                    Top = 132
                    Width = 81
                    Height = 24
                    TabOrder = 4
                  end
                  object edtD47_6: TEdit
                    Left = 204
                    Top = 160
                    Width = 81
                    Height = 24
                    TabOrder = 5
                  end
                end
                object pnlD41: TPanel
                  Left = 16
                  Top = 28
                  Width = 393
                  Height = 77
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 1
                  object labD41: TLabel
                    Left = 343
                    Top = 35
                    Width = 22
                    Height = 16
                    Caption = 'mm'
                  end
                  object labD41_3: TLabel
                    Left = 35
                    Top = 35
                    Width = 106
                    Height = 16
                    Caption = 'Index check offset'
                  end
                  object labD41_2: TLabel
                    Left = 222
                    Top = 35
                    Width = 22
                    Height = 16
                    Caption = 'mm'
                  end
                  object labD41_1: TLabel
                    Left = 7
                    Top = 7
                    Width = 232
                    Height = 16
                    Caption = '[D41] Index check IC position for socket'
                  end
                  object coD41: TComboBox
                    Left = 244
                    Top = 3
                    Width = 104
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    ItemHeight = 16
                    TabOrder = 0
                    Text = 'Inside socket'
                    Items.Strings = (
                      'Inside socket'
                      'Above Socket')
                  end
                  object edD41: TEdit
                    Left = 299
                    Top = 31
                    Width = 41
                    Height = 24
                    TabOrder = 2
                    Text = '1'
                  end
                  object edD41_2: TEdit
                    Left = 159
                    Top = 31
                    Width = 49
                    Height = 24
                    TabOrder = 1
                    Text = '1'
                  end
                  object cbD41: TCheckBox
                    Left = 24
                    Top = 57
                    Width = 161
                    Height = 15
                    Caption = '[D41] Check by Setup'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                end
              end
            end
            object tsD_50: TTabSheet
              Caption = 'D[50]'
              ImageIndex = 5
              object pal_D6: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labD53: TLabel
                  Left = 56
                  Top = 138
                  Width = 266
                  Height = 16
                  Caption = 
                    'or will turn off after                                minutes on' +
                    '.'
                end
                object labD54: TLabel
                  Left = 240
                  Top = 195
                  Width = 64
                  Height = 16
                  Caption = 'Speed (%)'
                end
                object cbD51: TCheckBox
                  Left = 5
                  Top = 37
                  Width = 388
                  Height = 21
                  Caption = '[D51] After one cycle and clean out, test arm go rear position'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbD52: TCheckBox
                  Left = 5
                  Top = 74
                  Width = 384
                  Height = 15
                  Caption = '[D52] Test head goed up then show interface error '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbD53: TCheckBox
                  Left = 5
                  Top = 106
                  Width = 390
                  Height = 15
                  Caption = '[D53] Index light always on '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                end
                object edD53: TEdit
                  Left = 169
                  Top = 134
                  Width = 76
                  Height = 24
                  TabOrder = 4
                end
                object cbD54: TCheckBox
                  Left = 5
                  Top = 170
                  Width = 313
                  Height = 17
                  Caption = '[D54] Index pick && place shuttle need slow down'
                  TabOrder = 5
                end
                object edD54: TEdit
                  Left = 308
                  Top = 190
                  Width = 73
                  Height = 24
                  TabOrder = 6
                end
                object cbD55: TCheckBox
                  Left = 5
                  Top = 228
                  Width = 313
                  Height = 17
                  Caption = '[D55] When RTC enabled, disable index check.'
                  TabOrder = 7
                end
                object cbD56: TCheckBox
                  Left = 5
                  Top = 256
                  Width = 313
                  Height = 17
                  Caption = '[D56] Forced enable Piggy-Back function'
                  TabOrder = 8
                end
                object cbD57: TCheckBox
                  Left = 5
                  Top = 284
                  Width = 313
                  Height = 17
                  Caption = '[D57] Close site need display chanel number '
                  TabOrder = 9
                end
                object cbD58: TCheckBox
                  Left = 6
                  Top = 312
                  Width = 313
                  Height = 17
                  Caption = '[D58] Arm 1 for pick and place,  Arm 2 for testing'
                  TabOrder = 10
                end
                object cbD59: TCheckBox
                  Left = 6
                  Top = 340
                  Width = 313
                  Height = 17
                  Caption = '[D59] 32Site, Pnp devices together'
                  TabOrder = 11
                end
                object cbD50: TCheckBox
                  Left = 5
                  Top = 8
                  Width = 388
                  Height = 21
                  Caption = '[D50] Enable Index Pick Error Skip Need Check Vaccum'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
              end
            end
            object tsD_60: TTabSheet
              Caption = 'D[60]'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImageIndex = 6
              ParentFont = False
              object pal_D7: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labD62: TLabel
                  Left = 163
                  Top = 223
                  Width = 190
                  Height = 16
                  Caption = 'Blow Air Time                            Sec'
                  Visible = False
                end
                object labD68_1: TLabel
                  Left = 3
                  Top = 463
                  Width = 347
                  Height = 32
                  Caption = 
                    '[D68] After complete of the Auto high compared to before,   '#13#10'  ' +
                    '        if the height more than need to Jam                     ' +
                    '     mm'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  Visible = False
                  WordWrap = True
                end
                object gbD60: TGroupBox
                  Left = 4
                  Top = 5
                  Width = 385
                  Height = 125
                  Caption = '[D60] EP load rate :'
                  Color = 12761254
                  ParentColor = False
                  TabOrder = 0
                  object labD60: TLabel
                    Left = 2
                    Top = 31
                    Width = 45
                    Height = 16
                    Caption = '56mm : '
                  end
                  object labD60_1: TLabel
                    Left = 104
                    Top = 96
                    Width = 168
                    Height = 16
                    Caption = '60mm offset by heater mode'
                  end
                  object labD60_NS: TLabel
                    Left = 2
                    Top = 63
                    Width = 82
                    Height = 16
                    Caption = '56mm for NS: '
                  end
                  object tbD60_Index56mm: TTrackBar
                    Left = 108
                    Top = 26
                    Width = 275
                    Height = 35
                    Max = 150
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 0
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD60_Index56mmChange
                  end
                  object edD60_56mm: TEdit
                    Left = 282
                    Top = 92
                    Width = 80
                    Height = 24
                    TabOrder = 2
                    OnClick = edD60_56mmClick
                  end
                  object tbD60_Index56mm_NS: TTrackBar
                    Left = 108
                    Top = 58
                    Width = 275
                    Height = 35
                    Max = 150
                    Min = 80
                    Orientation = trHorizontal
                    Frequency = 1
                    Position = 80
                    SelEnd = 0
                    SelStart = 0
                    TabOrder = 1
                    TickMarks = tmBottomRight
                    TickStyle = tsAuto
                    OnChange = tbD60_Index56mm_NSChange
                  end
                end
                object cbD61: TCheckBox
                  Left = 6
                  Top = 144
                  Width = 391
                  Height = 41
                  Caption = 
                    '[D61] encountered  Vacuum sensor OFF error, must do piggyback ch' +
                    'eck.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -12
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                end
                object cbD62: TCheckBox
                  Left = 6
                  Top = 180
                  Width = 391
                  Height = 41
                  Caption = '[D62] Pick up shuttle error, need to purge one time.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                end
                object pnlD63: TPanel
                  Left = 1
                  Top = 245
                  Width = 396
                  Height = 65
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 4
                  object lblD63: TLabel
                    Left = 324
                    Top = 35
                    Width = 42
                    Height = 16
                    Caption = '(Pulse)'
                  end
                  object edD63: TEdit
                    Left = 248
                    Top = 32
                    Width = 74
                    Height = 24
                    Hint = 'D63_ed'
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    TabOrder = 1
                  end
                  object cbD63: TCheckBox
                    Left = 4
                    Top = 6
                    Width = 387
                    Height = 25
                    Hint = 'D63'
                    Caption = '[D63] Check Index Z Home To Z Phase Distance Over Range'
                    Checked = True
                    State = cbChecked
                    TabOrder = 0
                  end
                end
                object cbD64: TCheckBox
                  Left = 6
                  Top = 324
                  Width = 391
                  Height = 29
                  Caption = '[D64] Pick up shuttle error, only SKIP'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 6
                end
                object cbD65: TCheckBox
                  Left = 6
                  Top = 360
                  Width = 379
                  Height = 25
                  Hint = 'D63'
                  Caption = '[D65] Enable check socket sensor function'
                  TabOrder = 7
                end
                object edD62: TEdit
                  Left = 249
                  Top = 220
                  Width = 74
                  Height = 24
                  Hint = 'D63_ed'
                  ImeName = 'Chinese (Traditional) - Phonetic'
                  TabOrder = 3
                  Visible = False
                end
                object cbD66: TCheckBox
                  Left = 6
                  Top = 388
                  Width = 391
                  Height = 41
                  Caption = '[D66] Initial start auto height'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 8
                end
                object cbD67: TCheckBox
                  Left = 6
                  Top = 432
                  Width = 391
                  Height = 20
                  Caption = '[D67] Load Cell Measure'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 9
                  Visible = False
                end
                object edD68: TEdit
                  Left = 240
                  Top = 476
                  Width = 74
                  Height = 24
                  Hint = 'D63_ed'
                  ImeName = 'Chinese (Traditional) - Phonetic'
                  TabOrder = 10
                  Visible = False
                end
                object rgD69: TRadioGroup
                  Left = 4
                  Top = 516
                  Width = 377
                  Height = 81
                  Caption = '[D69] Index check mode for auto clean'
                  ItemIndex = 0
                  Items.Strings = (
                    'Enable all index check'
                    'Disable index check after auto clean'
                    'Disable both index check when auto clean')
                  TabOrder = 11
                end
                object cbD63_1: TCheckBox
                  Left = 26
                  Top = 298
                  Width = 387
                  Height = 25
                  Hint = 'D63'
                  Caption = '[D63_1] Find Motor Phase Every Go-Home Process'
                  TabOrder = 5
                end
              end
            end
            object tsD_70: TTabSheet
              Caption = 'D[70]'
              ImageIndex = 7
              object pal_D8: TPanel
                Left = 0
                Top = 0
                Width = 429
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbD70: TCheckBox
                  Left = 6
                  Top = 8
                  Width = 391
                  Height = 20
                  Caption = '[D70] Index Cycle Time Record'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 0
                  Visible = False
                end
                object rgD71: TRadioGroup
                  Left = 4
                  Top = 36
                  Width = 413
                  Height = 133
                  Caption = '[D71] Do Index check mode'
                  ItemIndex = 0
                  Items.Strings = (
                    'Lot Strart/Lot End'
                    'After index alarm'
                    'Disable all index check'
                    'Normal(Enable all index check)')
                  TabOrder = 1
                end
                object cbD72: TCheckBox
                  Left = 6
                  Top = 180
                  Width = 391
                  Height = 20
                  Caption = '[D72] Shuttle 1 move after index contact for NN mode.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 2
                  Visible = False
                end
                object cbD73: TCheckBox
                  Left = 6
                  Top = 204
                  Width = 391
                  Height = 20
                  Caption = '[D73] Contact Mode fast'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  Visible = False
                end
                object cbD75: TCheckBox
                  Left = 7
                  Top = 255
                  Width = 418
                  Height = 17
                  Caption = '[D75] One cycle finished, Alaway need to be learning RTC golden.'
                  TabOrder = 5
                end
                object cbD78: TCheckBox
                  Left = 6
                  Top = 524
                  Width = 391
                  Height = 20
                  Caption = '[D78] Index Check Has IC Need Purge'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 6
                end
                object cbD79: TCheckBox
                  Left = 6
                  Top = 548
                  Width = 391
                  Height = 20
                  Caption = '[D79] Index Pick Shuttle Err Need Purge'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 7
                end
                object cbD74: TCheckBox
                  Left = 7
                  Top = 231
                  Width = 178
                  Height = 18
                  Caption = '[D74] RTC Auto Tuning'
                  TabOrder = 4
                end
              end
            end
            object tsD_80: TTabSheet
              Caption = 'D[80]'
              ImageIndex = 8
              object cbD80: TCheckBox
                Left = 4
                Top = 8
                Width = 387
                Height = 25
                Hint = 'D63'
                Caption = '[D80] After Out Shuttle to Right Site Index Check'
                Ctl3D = True
                ParentCtl3D = False
                TabOrder = 0
              end
              object cbD81: TCheckBox
                Left = 4
                Top = 35
                Width = 410
                Height = 20
                Caption = '[D81] Index Check Vacuum on shuttle after input arm device drop.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
              end
              object cbD82: TCheckBox
                Left = 4
                Top = 56
                Width = 410
                Height = 20
                Caption = '[D82] Check Index Arm has IC when indexing the product.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
              end
            end
          end
          object MemoD: TMemo
            Left = 437
            Top = 0
            Width = 484
            Height = 737
            Align = alClient
            Color = 14670284
            ScrollBars = ssVertical
            TabOrder = 1
          end
        end
        object tsE00: TTabSheet
          Caption = 'E [ In/Out Arm ]'
          ImageIndex = 3
          object pcE00: TPageControl
            Left = 0
            Top = 0
            Width = 529
            Height = 737
            ActivePage = tsE70
            Align = alLeft
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabIndex = 3
            TabOrder = 0
            TabWidth = 75
            object tsE_XYScale: TTabSheet
              Caption = 'X/Y Scale'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImageIndex = 1
              ParentFont = False
              object pgcScale: TPageControl
                Left = 0
                Top = 0
                Width = 521
                Height = 706
                ActivePage = tsE30
                Align = alClient
                TabIndex = 0
                TabOrder = 0
                object tsE30: TTabSheet
                  Caption = '[E30] In Arm Scale'
                  object pnlE30: TPanel
                    Left = 0
                    Top = 0
                    Width = 513
                    Height = 120
                    Align = alTop
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 0
                    object cbE30: TCheckBox
                      Left = 6
                      Top = 8
                      Width = 403
                      Height = 17
                      Caption = '[E30] In arm use different scale (Range 0.95'#65374'1.05)'
                      TabOrder = 0
                      OnClick = cbE30Click
                    end
                    object palE30: TPanel
                      Left = 2
                      Top = 25
                      Width = 509
                      Height = 93
                      Align = alBottom
                      BevelOuter = bvNone
                      Color = 12761254
                      TabOrder = 1
                      object gbE30_loader: TGroupBox
                        Left = 8
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Loader'
                        TabOrder = 0
                        object labE30_2: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE30_1: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE30_LodX: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE30_LodY: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object gbE30_HP1: TGroupBox
                        Left = 178
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Hot Plate 1'
                        TabOrder = 1
                        object labE30_3: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE30_4: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE30_HP1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE30_HP1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object gbE30_HP2: TGroupBox
                        Left = 348
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Hot Plate 2'
                        TabOrder = 2
                        object labE30_5: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE30_6: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE30_HP2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE30_HP2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                    end
                  end
                  object pnlE30_Hot: TPanel
                    Left = 1
                    Top = 114
                    Width = 529
                    Height = 120
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 1
                    object cbE30_1: TCheckBox
                      Left = 6
                      Top = 8
                      Width = 447
                      Height = 17
                      Caption = '[E30-1] In arm use different scale (Range 0.95'#65374'1.05) Hot'
                      TabOrder = 0
                    end
                    object gbE30_1_loader: TGroupBox
                      Left = 9
                      Top = 27
                      Width = 160
                      Height = 85
                      Caption = 'Loader'
                      TabOrder = 1
                      object Label39: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label40: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_1_LodX: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                      object edE30_1_LodY: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                    end
                    object gbE30_1_HP1: TGroupBox
                      Left = 179
                      Top = 27
                      Width = 160
                      Height = 85
                      Caption = 'Hot Plate 1'
                      TabOrder = 2
                      object Label41: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label44: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_1_HP1X: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                      object edE30_1_HP1Y: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                    end
                    object gbE30_1_HP2: TGroupBox
                      Left = 349
                      Top = 27
                      Width = 160
                      Height = 85
                      Caption = 'Hot Plate 2'
                      TabOrder = 3
                      object Label45: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label46: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_1_HP2Y: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                      object edE30_1_HP2X: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                    end
                  end
                  object pnlE30_Cold: TPanel
                    Left = 1
                    Top = 234
                    Width = 529
                    Height = 120
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 2
                    object cbE30_2: TCheckBox
                      Left = 10
                      Top = 6
                      Width = 471
                      Height = 17
                      Caption = '[E30-2] In arm use different scale (Range 0.95'#65374'1.05) Cold'
                      TabOrder = 0
                    end
                    object gbE30_2_loader: TGroupBox
                      Left = 10
                      Top = 28
                      Width = 160
                      Height = 85
                      Caption = 'Loader'
                      TabOrder = 1
                      object Label90: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label91: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_2_LodX: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                      object edE30_2_LodY: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                    end
                    object gbE30_2_HP1: TGroupBox
                      Left = 180
                      Top = 28
                      Width = 160
                      Height = 85
                      Caption = 'Hot Plate 1'
                      TabOrder = 2
                      object Label92: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label93: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_2_HP1X: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                      object edE30_2_HP1Y: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                    end
                    object gbE30_2_HP2: TGroupBox
                      Left = 350
                      Top = 28
                      Width = 160
                      Height = 85
                      Caption = 'Hot Plate 2'
                      TabOrder = 3
                      object Label94: TLabel
                        Left = 15
                        Top = 22
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'X'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object Label96: TLabel
                        Left = 15
                        Top = 55
                        Width = 20
                        Height = 20
                        AutoSize = False
                        Caption = 'Y'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                      end
                      object edE30_2_HP2Y: TEdit
                        Left = 30
                        Top = 50
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 1
                      end
                      object edE30_2_HP2X: TEdit
                        Left = 30
                        Top = 18
                        Width = 120
                        Height = 24
                        ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                        TabOrder = 0
                      end
                    end
                  end
                end
                object tsE31: TTabSheet
                  Caption = '[E31] Out Arm Scale'
                  ImageIndex = 2
                  object pgcE31: TPageControl
                    Left = 0
                    Top = 0
                    Width = 513
                    Height = 675
                    ActivePage = tsE31_1
                    Align = alClient
                    TabIndex = 0
                    TabOrder = 0
                    object tsE31_1: TTabSheet
                      Caption = '[E31]'
                      object pnlE31: TPanel
                        Left = 0
                        Top = 0
                        Width = 505
                        Height = 393
                        Align = alTop
                        BevelOuter = bvNone
                        BorderWidth = 2
                        Color = 12761254
                        TabOrder = 0
                        object cbE31: TCheckBox
                          Left = 6
                          Top = 8
                          Width = 403
                          Height = 17
                          Caption = '[E31] Out arm use different scale (Range 0.95'#65374'1.05)'
                          TabOrder = 0
                          OnClick = cbE30Click
                        end
                        object palE31: TPanel
                          Left = 2
                          Top = 34
                          Width = 501
                          Height = 357
                          Align = alBottom
                          BevelOuter = bvNone
                          Color = 12761254
                          TabOrder = 1
                          object gbE31_Auto1: TGroupBox
                            Left = 0
                            Top = -2
                            Width = 160
                            Height = 85
                            Caption = 'Auto 1'
                            TabOrder = 0
                            object labE31_1: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_2: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Auto2: TGroupBox
                            Left = 170
                            Top = -2
                            Width = 160
                            Height = 85
                            Caption = 'Auto 2'
                            TabOrder = 1
                            object labE31_3: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_4: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Auto3: TGroupBox
                            Left = 340
                            Top = -2
                            Width = 160
                            Height = 85
                            Caption = 'Auto 3'
                            TabOrder = 2
                            object labE31_5: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_6: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Fix3: TGroupBox
                            Left = 340
                            Top = 168
                            Width = 160
                            Height = 85
                            Caption = 'Fix 3'
                            TabOrder = 8
                            object labE31_B: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_C: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Fix2: TGroupBox
                            Left = 170
                            Top = 168
                            Width = 160
                            Height = 85
                            Caption = 'Fix 2'
                            TabOrder = 7
                            object labE31_9: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_A: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Fix1: TGroupBox
                            Left = 0
                            Top = 168
                            Width = 160
                            Height = 85
                            Caption = 'Fix 1'
                            TabOrder = 6
                            object labE31_7: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object labE31_8: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Auto4: TGroupBox
                            Left = 0
                            Top = 83
                            Width = 160
                            Height = 85
                            Caption = 'Auto 4'
                            TabOrder = 3
                            object Label10: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label11: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au4Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object gbE31_Auto5: TGroupBox
                            Left = 170
                            Top = 83
                            Width = 160
                            Height = 85
                            Caption = 'Auto 5'
                            TabOrder = 4
                            object Label12: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label13: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object grpE31_Auto6: TGroupBox
                            Left = 340
                            Top = 83
                            Width = 160
                            Height = 85
                            Caption = 'Auto 6'
                            TabOrder = 5
                            object Label14: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label15: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Au6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Au6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object grpE31_Fix4: TGroupBox
                            Left = 0
                            Top = 253
                            Width = 160
                            Height = 85
                            Caption = 'Fix 4'
                            TabOrder = 9
                            object Label16: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label17: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi4Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object grpE31_Fix5: TGroupBox
                            Left = 170
                            Top = 253
                            Width = 160
                            Height = 85
                            Caption = 'Fix 5'
                            TabOrder = 10
                            object Label18: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label19: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object grpE31_Fix6: TGroupBox
                            Left = 340
                            Top = 253
                            Width = 160
                            Height = 85
                            Caption = 'Fix 6'
                            TabOrder = 11
                            object Label20: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label21: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_Fi6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_Fi6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                        end
                      end
                    end
                    object tsE31_Hot: TTabSheet
                      Caption = '[E31] Hot'
                      ImageIndex = 1
                      object pnlE31_Hot: TPanel
                        Left = 0
                        Top = 0
                        Width = 505
                        Height = 393
                        Align = alTop
                        BevelOuter = bvNone
                        BorderWidth = 2
                        Color = 12761254
                        TabOrder = 0
                        object cbE31_1: TCheckBox
                          Left = 6
                          Top = 8
                          Width = 483
                          Height = 17
                          Caption = '[E31-1] Out arm use different scale (Range 0.95'#65374'1.05) Hot'
                          TabOrder = 0
                          OnClick = cbE30Click
                        end
                        object pnlE31_2: TPanel
                          Left = 2
                          Top = 34
                          Width = 501
                          Height = 357
                          Align = alBottom
                          BevelOuter = bvNone
                          Color = 12761254
                          TabOrder = 1
                          object GroupBox8: TGroupBox
                            Left = 1
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 1'
                            TabOrder = 0
                            object Label47: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label48: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox9: TGroupBox
                            Left = 171
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 2'
                            TabOrder = 1
                            object Label49: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label50: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox10: TGroupBox
                            Left = 341
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 3'
                            TabOrder = 2
                            object Label51: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label53: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox11: TGroupBox
                            Left = 341
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 3'
                            TabOrder = 8
                            object Label54: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label57: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox12: TGroupBox
                            Left = 171
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 2'
                            TabOrder = 7
                            object Label58: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label64: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox13: TGroupBox
                            Left = 1
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 1'
                            TabOrder = 6
                            object Label65: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label66: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox14: TGroupBox
                            Left = 1
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 4'
                            TabOrder = 3
                            object Label67: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label68: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au4Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox15: TGroupBox
                            Left = 171
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 5'
                            TabOrder = 4
                            object Label70: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label73: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox18: TGroupBox
                            Left = 341
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 6'
                            TabOrder = 5
                            object Label74: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label75: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Au6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Au6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox19: TGroupBox
                            Left = 1
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 4'
                            TabOrder = 9
                            object Label76: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label77: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi4Y: TEdit
                              Left = 34
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox20: TGroupBox
                            Left = 171
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 5'
                            TabOrder = 10
                            object Label78: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label79: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox21: TGroupBox
                            Left = 341
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 6'
                            TabOrder = 11
                            object Label80: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label81: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_1_Fi6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_1_Fi6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                        end
                      end
                    end
                    object tsE31_Cold: TTabSheet
                      Caption = '[E31] Cold'
                      ImageIndex = 2
                      object pnlE31_Cold: TPanel
                        Left = 0
                        Top = 0
                        Width = 505
                        Height = 393
                        Align = alTop
                        BevelOuter = bvNone
                        BorderWidth = 2
                        Color = 12761254
                        TabOrder = 0
                        object cbE31_2: TCheckBox
                          Left = 6
                          Top = 8
                          Width = 483
                          Height = 17
                          Caption = '[E31-2] Out arm use different scale (Range 0.95'#65374'1.05) Cold'
                          TabOrder = 0
                          OnClick = cbE30Click
                        end
                        object pnlE32_3: TPanel
                          Left = 2
                          Top = 34
                          Width = 501
                          Height = 357
                          Align = alBottom
                          BevelOuter = bvNone
                          Color = 12761254
                          TabOrder = 1
                          object GroupBox29: TGroupBox
                            Left = 0
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 1'
                            TabOrder = 0
                            object Label97: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label98: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox30: TGroupBox
                            Left = 170
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 2'
                            TabOrder = 1
                            object Label99: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label100: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox31: TGroupBox
                            Left = 340
                            Top = -3
                            Width = 160
                            Height = 85
                            Caption = 'Auto 3'
                            TabOrder = 2
                            object Label101: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label102: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox32: TGroupBox
                            Left = 340
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 3'
                            TabOrder = 8
                            object Label103: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label104: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi3X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi3Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox33: TGroupBox
                            Left = 170
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 2'
                            TabOrder = 7
                            object Label105: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label106: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi2X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi2Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox34: TGroupBox
                            Left = 0
                            Top = 167
                            Width = 160
                            Height = 85
                            Caption = 'Fix 1'
                            TabOrder = 6
                            object Label107: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label108: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi1X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi1Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox35: TGroupBox
                            Left = 0
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 4'
                            TabOrder = 3
                            object Label109: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label110: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au4Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox36: TGroupBox
                            Left = 170
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 5'
                            TabOrder = 4
                            object Label111: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label112: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox37: TGroupBox
                            Left = 340
                            Top = 82
                            Width = 160
                            Height = 85
                            Caption = 'Auto 6'
                            TabOrder = 5
                            object Label113: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label114: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Au6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Au6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox38: TGroupBox
                            Left = 0
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 4'
                            TabOrder = 9
                            object Label115: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label116: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi4X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi4Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox39: TGroupBox
                            Left = 170
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 5'
                            TabOrder = 10
                            object Label117: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label118: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi5X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi5Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                          object GroupBox40: TGroupBox
                            Left = 340
                            Top = 252
                            Width = 160
                            Height = 85
                            Caption = 'Fix 6'
                            TabOrder = 11
                            object Label119: TLabel
                              Left = 15
                              Top = 22
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'X'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object Label120: TLabel
                              Left = 15
                              Top = 55
                              Width = 20
                              Height = 20
                              AutoSize = False
                              Caption = 'Y'
                              Font.Charset = DEFAULT_CHARSET
                              Font.Color = clWindowText
                              Font.Height = -13
                              Font.Name = 'MS Sans Serif'
                              Font.Style = []
                              ParentFont = False
                            end
                            object edE31_2_Fi6X: TEdit
                              Left = 30
                              Top = 18
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 0
                              OnClick = edE31_Fi1XClick
                            end
                            object edE31_2_Fi6Y: TEdit
                              Left = 30
                              Top = 50
                              Width = 120
                              Height = 24
                              ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                              TabOrder = 1
                              OnClick = edE31_Fi1XClick
                            end
                          end
                        end
                      end
                    end
                  end
                end
                object tsE32: TTabSheet
                  Caption = '[E32] Shuttle Scale'
                  ImageIndex = 1
                  object pnlE32: TPanel
                    Left = 0
                    Top = 0
                    Width = 513
                    Height = 206
                    Align = alTop
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 0
                    object palE32: TPanel
                      Left = 2
                      Top = 25
                      Width = 509
                      Height = 179
                      Align = alBottom
                      BevelOuter = bvNone
                      Color = 12761254
                      TabOrder = 1
                      object gbE32_InSh1: TGroupBox
                        Left = 8
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 1'
                        TabOrder = 0
                        object labE32_1: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE32_2: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_IS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_IS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object gbE32_InSh2: TGroupBox
                        Left = 178
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 2'
                        TabOrder = 1
                        object labE32_3: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE32_4: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_IS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_IS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object gbE32_OutSh1: TGroupBox
                        Left = 8
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 1'
                        TabOrder = 2
                        object labE32_5: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE32_6: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_OS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_OS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object gbE32_OutSh2: TGroupBox
                        Left = 178
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 2'
                        TabOrder = 3
                        object labE32_7: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object labE32_8: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_OS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_OS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                    end
                    object cbE32: TCheckBox
                      Left = 6
                      Top = 8
                      Width = 403
                      Height = 17
                      Caption = '[E32] Shuttle use different scale (Range 0.95'#65374'1.05)'
                      TabOrder = 0
                      OnClick = cbE30Click
                    end
                  end
                  object pnlE32_Hot: TPanel
                    Left = 0
                    Top = 206
                    Width = 513
                    Height = 206
                    Align = alTop
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 1
                    object cbE32_1: TCheckBox
                      Left = 6
                      Top = 8
                      Width = 467
                      Height = 17
                      Caption = '[E32-1] Shuttle use different scale (Range 0.95'#65374'1.05) Hot'
                      TabOrder = 0
                      OnClick = cbE30Click
                    end
                    object palE32_Hot: TPanel
                      Left = 2
                      Top = 25
                      Width = 509
                      Height = 179
                      Align = alBottom
                      BevelOuter = bvNone
                      Color = 12761254
                      TabOrder = 1
                      object GroupBox22: TGroupBox
                        Left = 8
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 1'
                        TabOrder = 0
                        object Label82: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label83: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_1_IS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_1_IS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox23: TGroupBox
                        Left = 178
                        Top = 2
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 2'
                        TabOrder = 1
                        object Label84: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label85: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_1_IS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_1_IS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox24: TGroupBox
                        Left = 8
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 1'
                        TabOrder = 2
                        object Label86: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label87: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_1_OS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_1_OS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox25: TGroupBox
                        Left = 178
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 2'
                        TabOrder = 3
                        object Label88: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label89: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_1_OS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_1_OS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                    end
                  end
                  object pnlE32_Cold: TPanel
                    Left = 0
                    Top = 412
                    Width = 513
                    Height = 206
                    Align = alTop
                    BevelOuter = bvNone
                    BorderWidth = 2
                    Color = 12761254
                    TabOrder = 2
                    object cbE32_2: TCheckBox
                      Left = 6
                      Top = 8
                      Width = 483
                      Height = 17
                      Caption = '[E32-2] Shuttle use different scale (Range 0.95'#65374'1.05) Cold'
                      TabOrder = 0
                      OnClick = cbE30Click
                    end
                    object palE32_Cold: TPanel
                      Left = 2
                      Top = 24
                      Width = 509
                      Height = 180
                      Align = alBottom
                      BevelOuter = bvNone
                      Color = 12761254
                      TabOrder = 1
                      object GroupBox41: TGroupBox
                        Left = 10
                        Top = 4
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 1'
                        TabOrder = 0
                        object Label121: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label122: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_2_IS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_2_IS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox42: TGroupBox
                        Left = 180
                        Top = 4
                        Width = 160
                        Height = 85
                        Caption = 'Input Shuttle 2'
                        TabOrder = 1
                        object Label123: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label124: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_2_IS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_2_IS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox43: TGroupBox
                        Left = 10
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 1'
                        TabOrder = 2
                        object Label125: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label126: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_2_OS1X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_2_OS1Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                      end
                      object GroupBox44: TGroupBox
                        Left = 180
                        Top = 90
                        Width = 160
                        Height = 85
                        Caption = 'Output Shuttle 2'
                        TabOrder = 3
                        object Label127: TLabel
                          Left = 15
                          Top = 22
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'X'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object Label128: TLabel
                          Left = 15
                          Top = 55
                          Width = 20
                          Height = 20
                          AutoSize = False
                          Caption = 'Y'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object edE32_2_OS2Y: TEdit
                          Left = 30
                          Top = 50
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 1
                          OnClick = edE31_Fi1XClick
                        end
                        object edE32_2_OS2X: TEdit
                          Left = 30
                          Top = 18
                          Width = 120
                          Height = 24
                          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                          TabOrder = 0
                          OnClick = edE31_Fi1XClick
                        end
                      end
                    end
                  end
                end
              end
            end
            object tsE_Other: TTabSheet
              Caption = 'Other'
              ImageIndex = 1
              object pal_E2: TPanel
                Left = 0
                Top = 0
                Width = 521
                Height = 681
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labE36: TLabel
                  Left = 15
                  Top = 85
                  Width = 382
                  Height = 16
                  Caption = 
                    '[E36] In arm Y pitch 60mm offset ( Range:100~-100 , 0.01mm/unit ' +
                    ')'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labE37: TLabel
                  Left = 15
                  Top = 110
                  Width = 392
                  Height = 16
                  Caption = 
                    '[E37] Out arm Y pitch 60mm offset ( Range:100~-100 , 0.01mm/unit' +
                    ' )'
                  ParentShowHint = False
                  ShowHint = True
                end
                object cbE33: TCheckBox
                  Left = 10
                  Top = 6
                  Width = 403
                  Height = 17
                  Caption = '[E33] In && out arm Z using same offset'
                  TabOrder = 0
                  OnClick = cbE30Click
                end
                object cbE34: TCheckBox
                  Left = 10
                  Top = 32
                  Width = 405
                  Height = 17
                  Caption = '[E34] In && out arm pitch && pick/release using same offset'
                  TabOrder = 1
                end
                object cbE35: TCheckBox
                  Left = 10
                  Top = 59
                  Width = 483
                  Height = 17
                  Caption = 
                    '[E35] In && Out arm disable check device drop when picker goes d' +
                    'own.'
                  TabOrder = 2
                end
                object edE36: TEdit
                  Tag = 10
                  Left = 414
                  Top = 82
                  Width = 75
                  Height = 24
                  MaxLength = 4
                  TabOrder = 3
                  Text = '0'
                end
                object edE37: TEdit
                  Tag = 10
                  Left = 414
                  Top = 108
                  Width = 75
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 4
                  Text = '0'
                end
                object cbE38: TCheckBox
                  Left = 10
                  Top = 136
                  Width = 303
                  Height = 14
                  Caption = '[E38] Check hot plate while initial start.'
                  TabOrder = 5
                end
                object cbE39: TCheckBox
                  Left = 10
                  Top = 159
                  Width = 410
                  Height = 13
                  Caption = '[E39] Check hot plate after clean out and before tray feed.'
                  TabOrder = 6
                  OnClick = cbE39Click
                end
                object cbE39_1: TCheckBox
                  Left = 56
                  Top = 181
                  Width = 264
                  Height = 13
                  Caption = '[E39-1] Put the devices to error bin'
                  TabOrder = 7
                end
                object cbE40: TCheckBox
                  Left = 10
                  Top = 204
                  Width = 410
                  Height = 14
                  Caption = '[E40] Clear all hot IC then pick loader IC'
                  TabOrder = 8
                end
                object cbE41: TCheckBox
                  Left = 10
                  Top = 227
                  Width = 392
                  Height = 17
                  Caption = '[E41] Tray pitch > 35mm, in out arm speed must small than 80%.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object cbE42: TCheckBox
                  Left = 10
                  Top = 254
                  Width = 410
                  Height = 17
                  Caption = '[E42] In arm need servo off when out shuttle alarm'
                  TabOrder = 10
                end
                object cbE43: TCheckBox
                  Left = 10
                  Top = 280
                  Width = 410
                  Height = 17
                  Caption = '[E43] Auto clean use hot plate 1'
                  TabOrder = 11
                end
                object cbE44: TCheckBox
                  Left = 10
                  Top = 334
                  Width = 410
                  Height = 17
                  Caption = '[E44] Shuttle need servo off when shuttle lose devices'
                  TabOrder = 13
                end
                object cbE45: TCheckBox
                  Left = 10
                  Top = 361
                  Width = 410
                  Height = 17
                  Caption = '[E45] All setup file use one offset data'
                  TabOrder = 14
                end
                object cbE46: TCheckBox
                  Left = 10
                  Top = 387
                  Width = 410
                  Height = 17
                  Caption = '[E46] Loader use 2 offset for each row'
                  TabOrder = 15
                end
                object cbE47: TCheckBox
                  Left = 10
                  Top = 413
                  Width = 410
                  Height = 17
                  Caption = '[E47] Shuttle use 4 offset for each row and col'
                  TabOrder = 16
                end
                object cbE48: TCheckBox
                  Left = 10
                  Top = 440
                  Width = 410
                  Height = 17
                  Caption = '[E48] Auto clean shuttle use 4 offset for each row and col'
                  TabOrder = 17
                end
                object cbE49: TCheckBox
                  Left = 10
                  Top = 466
                  Width = 410
                  Height = 17
                  Caption = '[E49] Loader pick up error only RETRY and CLEAN OUT'
                  TabOrder = 18
                end
                object chkE43_1: TCheckBox
                  Left = 30
                  Top = 308
                  Width = 410
                  Height = 17
                  Caption = '[E43-1] Auto clean count save to DefineAutoClean folder'
                  TabOrder = 12
                end
              end
            end
            object tsE50: TTabSheet
              Caption = '[E50]'
              ImageIndex = 2
              object pal_E5: TPanel
                Left = 0
                Top = 0
                Width = 521
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cdE52: TCheckBox
                  Left = 6
                  Top = 70
                  Width = 410
                  Height = 17
                  Caption = '[E52] Out arm Z ADC speed'
                  TabOrder = 3
                end
                object cbE51: TCheckBox
                  Left = 6
                  Top = 48
                  Width = 410
                  Height = 17
                  Caption = '[E51] In arm Z ADC speed'
                  TabOrder = 2
                end
                object edE51: TEdit
                  Tag = 10
                  Left = 198
                  Top = 44
                  Width = 75
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 1
                  Text = '100'
                end
                object edE52: TEdit
                  Tag = 10
                  Left = 198
                  Top = 70
                  Width = 75
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 4
                  Text = '100'
                end
                object cbE53: TCheckBox
                  Left = 6
                  Top = 93
                  Width = 325
                  Height = 21
                  Caption = '[E53] When low yield do auto clean and close site.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object cbE54: TCheckBox
                  Left = 6
                  Top = 119
                  Width = 473
                  Height = 21
                  Caption = '[E54] Check close site can not have IC when pick from hot plate'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object cbE55: TCheckBox
                  Left = 6
                  Top = 145
                  Width = 473
                  Height = 21
                  Caption = '[E55] Use Fix3 Full Tray Function'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbE56: TCheckBox
                  Left = 6
                  Top = 171
                  Width = 473
                  Height = 21
                  Caption = '[E56] When loader have pick up error, to retry at same position.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                end
                object cbE57: TCheckBox
                  Left = 6
                  Top = 198
                  Width = 473
                  Height = 21
                  Caption = '[E57] Hot Plate can use another vacuum delay time '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object cbE58: TCheckBox
                  Left = 6
                  Top = 224
                  Width = 473
                  Height = 21
                  Caption = '[E58] In Out Arm Y pitch home check.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                end
                object cbE59: TCheckBox
                  Left = 6
                  Top = 250
                  Width = 473
                  Height = 21
                  Caption = '[E59] Offset file group by [###]'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 11
                end
                object cbE60: TCheckBox
                  Left = 6
                  Top = 277
                  Width = 473
                  Height = 21
                  Caption = '[E60] In arm pick from loader drop error auto skip'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 12
                end
                object cbE61: TCheckBox
                  Left = 6
                  Top = 303
                  Width = 473
                  Height = 21
                  Caption = '[E61] In arm standby postion on loader '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 13
                end
                object cbE62: TCheckBox
                  Left = 6
                  Top = 329
                  Width = 473
                  Height = 21
                  Caption = '[E62] Search last row when auto skip count over limit.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 14
                end
                object cbE63: TCheckBox
                  Left = 6
                  Top = 356
                  Width = 503
                  Height = 21
                  Caption = 
                    '[E63] In arm retry to pick up the loader device before alarm tak' +
                    'eout tray message.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 15
                end
                object cbE64: TCheckBox
                  Left = 6
                  Top = 382
                  Width = 515
                  Height = 21
                  Caption = 
                    '[E64] Tray pitch > 50mm or Tray  X-Division=1 , in out arm speed' +
                    ' must small than 50%.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 16
                end
                object chkE65: TCheckBox
                  Left = 6
                  Top = 408
                  Width = 503
                  Height = 21
                  Caption = '[E65] Out arm destroy error, clear the data on auto tray.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 17
                end
                object chkE66: TCheckBox
                  Left = 6
                  Top = 434
                  Width = 503
                  Height = 21
                  Caption = '[E66] Log Hot Plate Action'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 18
                end
                object chkE67: TCheckBox
                  Left = 6
                  Top = 460
                  Width = 503
                  Height = 21
                  Caption = '[E67] Load pick up error,move wait pos.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 19
                end
                object rgE50: TRadioGroup
                  Left = 8
                  Top = 4
                  Width = 413
                  Height = 37
                  Caption = '[E50] Output arm pickup error can choose'
                  Columns = 3
                  ItemIndex = 1
                  Items.Strings = (
                    'RETRY/SKIP'
                    'RETRY'
                    'SKIP')
                  TabOrder = 0
                end
                object chkE69: TCheckBox
                  Left = 6
                  Top = 510
                  Width = 503
                  Height = 21
                  Caption = '[E69] Pickup Error Placement'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 22
                end
                object cbE68: TCheckBox
                  Left = 6
                  Top = 485
                  Width = 419
                  Height = 21
                  Caption = 
                    '[E68] In/Out Arm IC drop status check several times and then ala' +
                    'rm'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 21
                end
                object edE68: TEdit
                  Tag = 10
                  Left = 427
                  Top = 482
                  Width = 75
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 20
                  Text = '100'
                end
              end
            end
            object tsE70: TTabSheet
              Caption = '[E70]'
              ImageIndex = 3
              object pal_E80: TPanel
                Left = 0
                Top = 0
                Width = 521
                Height = 706
                Align = alClient
                Color = 12761254
                TabOrder = 0
                object gbE85: TGroupBox
                  Left = 9
                  Top = 253
                  Width = 500
                  Height = 80
                  Caption = '[E85] Fill The Tray'
                  TabOrder = 7
                  object cbE85_Enable: TCheckBox
                    Left = 13
                    Top = 20
                    Width = 252
                    Height = 29
                    Caption = '[E85] Enable'
                    TabOrder = 0
                  end
                  object cbE85_Auto3: TCheckBox
                    Left = 101
                    Top = 45
                    Width = 60
                    Height = 29
                    Caption = 'Auto3'
                    TabOrder = 2
                  end
                  object cbE85_Auto1: TCheckBox
                    Left = 29
                    Top = 45
                    Width = 60
                    Height = 29
                    Caption = 'Auto1'
                    TabOrder = 1
                  end
                end
                object chkE70: TCheckBox
                  Left = 6
                  Top = 11
                  Width = 503
                  Height = 21
                  Caption = '[E70] In/Out Arm Use Tray Thick Adjust Z Height'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object chkE71: TCheckBox
                  Left = 6
                  Top = 36
                  Width = 503
                  Height = 21
                  Caption = 
                    '[E71] Tray Pitch < 10mm Lock Loader to Empty and Color to Auto T' +
                    'ray'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object chkE72: TCheckBox
                  Left = 6
                  Top = 61
                  Width = 503
                  Height = 21
                  Caption = '[E72] Inarm pick IC from tray need wait shuttle.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object chkE73: TCheckBox
                  Left = 6
                  Top = 85
                  Width = 503
                  Height = 21
                  Caption = 
                    '[E73] Inarm Z Motor Step Loss Check.                            ' +
                    '      times/min.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                end
                object edE73: TEdit
                  Tag = 10
                  Left = 311
                  Top = 82
                  Width = 30
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 3
                  Text = '5'
                end
                object chkE74: TCheckBox
                  Left = 6
                  Top = 110
                  Width = 503
                  Height = 21
                  Caption = '[E74] Inspect In/Out arm position.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object cbE77: TCheckBox
                  Left = 6
                  Top = 135
                  Width = 503
                  Height = 21
                  Caption = '[E77] Output arm C motion.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object cbkE78: TCheckBox
                  Left = 6
                  Top = 159
                  Width = 503
                  Height = 21
                  Caption = 
                    '[E78] In Arm Suck one by one when a pickup error occurs at the l' +
                    'oader.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                end
              end
            end
          end
          object MemoE: TMemo
            Left = 529
            Top = 0
            Width = 392
            Height = 737
            Align = alClient
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ScrollBars = ssVertical
            TabOrder = 1
          end
        end
        object tsF00: TTabSheet
          Caption = 'F [ Shuttle ]'
          ImageIndex = 7
          object MemoF: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pcF00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = TabSheet2
            Align = alClient
            TabIndex = 1
            TabOrder = 0
            object tsF01: TTabSheet
              Caption = '[F01] - [F10]'
              object pal_F00: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labF05: TLabel
                  Left = 240
                  Top = 102
                  Width = 34
                  Height = 16
                  Caption = 'Count'
                end
                object cbF05: TCheckBox
                  Left = 5
                  Top = 101
                  Width = 236
                  Height = 17
                  Caption = '[F05] Enable shuttlet clean function'
                  TabOrder = 4
                end
                object cbF01: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 350
                  Height = 17
                  Caption = '[F01] Shake shuttle when jam happen.  Speed(%):'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbF03: TCheckBox
                  Left = 5
                  Top = 75
                  Width = 427
                  Height = 17
                  Caption = '[F03] Output shuttle skip detect  IC miss'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object edF05: TEdit
                  Left = 280
                  Top = 98
                  Width = 81
                  Height = 24
                  TabOrder = 3
                end
                object cbF06: TCheckBox
                  Left = 5
                  Top = 128
                  Width = 350
                  Height = 17
                  Caption = '[F06] Enable initial IC check'
                  TabOrder = 5
                end
                object rgF07: TRadioGroup
                  Left = 18
                  Top = 162
                  Width = 265
                  Height = 80
                  Caption = '[F07] Out shuttle sensor detect mode'
                  ItemIndex = 0
                  Items.Strings = (
                    '1. Detect have IC'
                    '2. Detect double device'
                    '3 .Use Y latch sensor check floating')
                  TabOrder = 6
                end
                object cbF09: TCheckBox
                  Left = 5
                  Top = 272
                  Width = 292
                  Height = 17
                  Caption = '[F09] Check IC which first time load'
                  TabOrder = 7
                  Visible = False
                end
                object edF01: TEdit
                  Left = 344
                  Top = 4
                  Width = 61
                  Height = 24
                  TabOrder = 0
                  Text = '100'
                end
              end
            end
            object TabSheet2: TTabSheet
              Caption = '[F11] - [F20]'
              ImageIndex = 1
              object pal_F10: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labF12: TLabel
                  Left = 100
                  Top = 56
                  Width = 152
                  Height = 16
                  Caption = 'Check Delay Time (Sec.):'
                end
                object cbF20Label2: TLabel
                  Left = 68
                  Top = 516
                  Width = 367
                  Height = 16
                  Caption = 
                    'When use this function, [F06] && [F18] function would no functio' +
                    'n.'
                end
                object cbF11: TCheckBox
                  Left = 9
                  Top = 4
                  Width = 400
                  Height = 17
                  Caption = '[F11] Out shuttle use front rear sensor detect superfluous IC'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbF12: TCheckBox
                  Left = 9
                  Top = 32
                  Width = 384
                  Height = 17
                  Caption = '[F12] Rotate shuttle need check if the rotation is done.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object EdF12: TEdit
                  Left = 256
                  Top = 52
                  Width = 81
                  Height = 24
                  TabOrder = 2
                  Text = '0.01'
                end
                object gbF13_RotateShtSpeed: TGroupBox
                  Left = 4
                  Top = 80
                  Width = 389
                  Height = 113
                  Caption = '[F13] Rotate shuttle speed'
                  TabOrder = 3
                  object labF13_iRotateADC: TLabel
                    Left = 20
                    Top = 28
                    Width = 139
                    Height = 16
                    Caption = 'ADC speed (MAX:100) :'
                  end
                  object labF13_InitSpeed: TLabel
                    Left = 20
                    Top = 56
                    Width = 144
                    Height = 16
                    Caption = 'Initial speed (MAX:100) : '
                  end
                  object labF13_JogHighSpeed: TLabel
                    Left = 20
                    Top = 84
                    Width = 169
                    Height = 16
                    Caption = 'Jog high speed (MAX:5000) :'
                  end
                  object edF13_ADC: TEdit
                    Left = 212
                    Top = 24
                    Width = 121
                    Height = 24
                    TabOrder = 0
                    Text = '5'
                  end
                  object edF13_Ini: TEdit
                    Left = 212
                    Top = 52
                    Width = 121
                    Height = 24
                    TabOrder = 1
                    Text = '50'
                  end
                  object edF13_Jog: TEdit
                    Left = 212
                    Top = 80
                    Width = 121
                    Height = 24
                    TabOrder = 2
                    Text = '1500'
                  end
                end
                object cbF15: TCheckBox
                  Left = 9
                  Top = 344
                  Width = 400
                  Height = 17
                  Caption = '[F15] Out shuttle lose IC need input password '
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object cbF16: TCheckBox
                  Left = 9
                  Top = 372
                  Width = 400
                  Height = 17
                  Caption = '[F16] Check input shuttle sensor I/O'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object cbF18: TCheckBox
                  Left = 9
                  Top = 430
                  Width = 260
                  Height = 19
                  Caption = '[F18] In shuttle product detect'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 8
                end
                object cbF19: TCheckBox
                  Left = 9
                  Top = 460
                  Width = 380
                  Height = 19
                  Caption = '[F19] Out shuttle lose IC need do piggyback check'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 9
                end
                object cbF20: TCheckBox
                  Left = 9
                  Top = 490
                  Width = 380
                  Height = 19
                  Caption = '[F20] In shuttle product prominent detect '
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 10
                end
                object cbF17: TCheckBox
                  Left = 9
                  Top = 402
                  Width = 344
                  Height = 19
                  Caption = '[F17] Always shuttle 1 first after one cycle in hot mode'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 7
                end
                object pnlF14: TPanel
                  Left = 4
                  Top = 196
                  Width = 421
                  Height = 145
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 4
                  object labF14: TLabel
                    Left = 248
                    Top = 48
                    Width = 84
                    Height = 16
                    Caption = 'No. of knocks:'
                  end
                  object lab1: TLabel
                    Left = 248
                    Top = 116
                    Width = 84
                    Height = 16
                    Caption = 'No. of knocks:'
                  end
                  object cbF14: TCheckBox
                    Left = 5
                    Top = 12
                    Width = 400
                    Height = 17
                    Caption = '[F14] Knock shuttle when jam happen.  Interval(Sec.) :'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                  end
                  object cbF14_1: TCheckBox
                    Left = 5
                    Top = 80
                    Width = 400
                    Height = 17
                    Caption = 
                      '[F14-1] Knock shuttle first .                         Interval(S' +
                      'ec.) :'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                  object edF14: TEdit
                    Left = 340
                    Top = 8
                    Width = 65
                    Height = 24
                    TabOrder = 0
                    Text = '0.01'
                  end
                  object edF14_No: TEdit
                    Left = 340
                    Top = 44
                    Width = 65
                    Height = 24
                    TabOrder = 2
                    Text = '3'
                  end
                  object edF14_1: TEdit
                    Left = 340
                    Top = 76
                    Width = 65
                    Height = 24
                    TabOrder = 3
                    Text = '0.01'
                  end
                  object edF14_1_No: TEdit
                    Left = 340
                    Top = 112
                    Width = 65
                    Height = 24
                    TabOrder = 5
                    Text = '3'
                  end
                end
              end
            end
            object tsF21: TTabSheet
              Caption = '[F21] - [F30]'
              ImageIndex = 2
              object pal_F20: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbF21: TCheckBox
                  Left = 12
                  Top = 16
                  Width = 440
                  Height = 17
                  Caption = '[F21] IN/OUT Arm Z motor on home sensor ,shuttle move'
                  TabOrder = 0
                end
                object cbF22: TCheckBox
                  Left = 12
                  Top = 40
                  Width = 440
                  Height = 17
                  Caption = '[F22] In shuttle check has device from Index Arm.'
                  TabOrder = 1
                end
                object cbF23: TCheckBox
                  Left = 12
                  Top = 64
                  Width = 440
                  Height = 17
                  Caption = 
                    '[F23] Enable shuttle vibration                   (Unit : 0.1 Sec' +
                    ')'
                  TabOrder = 3
                end
                object edF23: TEdit
                  Left = 208
                  Top = 62
                  Width = 45
                  Height = 24
                  TabOrder = 2
                  Text = '20'
                end
                object cbF24: TCheckBox
                  Left = 11
                  Top = 113
                  Width = 440
                  Height = 17
                  Caption = '[F24] Out shuttle lose IC must open index door and push Z1 '
                  TabOrder = 5
                end
                object cbF25: TCheckBox
                  Left = 12
                  Top = 134
                  Width = 441
                  Height = 24
                  Hint = '[L23]Hot Temp shuttle no add pos'
                  Caption = 
                    '[F25] Vibration function for Output shuttle.                    ' +
                    '(Unit : 0.1 Sec)'
                  TabOrder = 7
                end
                object edF25: TEdit
                  Tag = 30
                  Left = 270
                  Top = 133
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 6
                  Text = '30'
                end
                object rgF26: TRadioGroup
                  Left = 10
                  Top = 160
                  Width = 363
                  Height = 80
                  Caption = '[F26] Out shuttle Jam Select Skip or Retry'
                  ItemIndex = 0
                  Items.Strings = (
                    'Skip/ Retry'
                    'Skip'
                    'Retry')
                  TabOrder = 8
                end
                object chkF27: TCheckBox
                  Left = 11
                  Top = 249
                  Width = 440
                  Height = 17
                  Caption = '[F27] Out shuttle lose IC, out arm need to pick again.'
                  TabOrder = 9
                end
                object cbF28: TCheckBox
                  Left = 11
                  Top = 276
                  Width = 440
                  Height = 17
                  Caption = '[F28] Index Check Shuttle pos for Sensor. '
                  TabOrder = 10
                end
                object cbF29: TCheckBox
                  Left = 11
                  Top = 302
                  Width = 165
                  Height = 24
                  Hint = '[L23]Hot Temp shuttle no add pos'
                  Caption = '[F29] Always Vibration'
                  TabOrder = 11
                end
                object cbF30: TCheckBox
                  Left = 11
                  Top = 331
                  Width = 440
                  Height = 17
                  Caption = 
                    '[F30] In shuttle floating sensor using new rule(use middle senso' +
                    'r)'
                  TabOrder = 12
                end
                object edtF23_1: TLabeledEdit
                  Left = 211
                  Top = 88
                  Width = 121
                  Height = 24
                  EditLabel.Width = 96
                  EditLabel.Height = 16
                  EditLabel.Caption = 'Vibration Count :'
                  LabelPosition = lpLeft
                  LabelSpacing = 3
                  TabOrder = 4
                end
              end
            end
            object tsF31: TTabSheet
              Caption = '[F31] - [F40]'
              ImageIndex = 3
              object pal_F30: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 681
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object grpF31: TGroupBox
                  Left = 2
                  Top = 2
                  Width = 467
                  Height = 229
                  Align = alTop
                  Caption = '[F31]'
                  TabOrder = 0
                  Visible = False
                  object Label24: TLabel
                    Left = 10
                    Top = 45
                    Width = 231
                    Height = 16
                    Caption = '[F31-1]  Shuttle 1 Motor Move Count Set'
                  end
                  object Label25: TLabel
                    Left = 10
                    Top = 75
                    Width = 231
                    Height = 16
                    Caption = '[F31-2]  Shuttle 2 Motor Move Count Set'
                  end
                  object Label26: TLabel
                    Left = 10
                    Top = 105
                    Width = 238
                    Height = 16
                    Caption = '[F31-3]  Shuttle 1 Motor Move Count Now'
                  end
                  object Label27: TLabel
                    Left = 10
                    Top = 135
                    Width = 238
                    Height = 16
                    Caption = '[F31-4]  Shuttle 2 Motor Move Count Now'
                  end
                  object Label28: TLabel
                    Left = 10
                    Top = 165
                    Width = 253
                    Height = 16
                    Caption = '[F31-6]  Shuttle 1 Motor Move Count Histroy'
                  end
                  object Label29: TLabel
                    Left = 10
                    Top = 195
                    Width = 253
                    Height = 16
                    Caption = '[F31-7]  Shuttle 2 Motor Move Count Histroy'
                  end
                  object edF31_1: TEdit
                    Tag = 30
                    Left = 280
                    Top = 40
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 1
                    Text = '1000000'
                  end
                  object edF31_2: TEdit
                    Tag = 30
                    Left = 280
                    Top = 70
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 2
                    Text = '1000000'
                  end
                  object edF31_3: TEdit
                    Tag = 30
                    Left = 280
                    Top = 100
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 3
                    Text = '30'
                  end
                  object edF31_4: TEdit
                    Tag = 30
                    Left = 280
                    Top = 130
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 4
                    Text = '30'
                  end
                  object edF31_5: TEdit
                    Tag = 30
                    Left = 280
                    Top = 160
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 5
                    Text = '30'
                  end
                  object edF31_6: TEdit
                    Tag = 30
                    Left = 280
                    Top = 190
                    Width = 70
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 6
                    Text = '30'
                  end
                  object cbF31: TCheckBox
                    Left = 12
                    Top = 20
                    Width = 420
                    Height = 17
                    Hint = 'F27'
                    Caption = '[F31] Enable Check Shuttle Motor Move Over Times To Jam'
                    TabOrder = 0
                  end
                end
                object cbF33_Check2DHardware: TCheckBox
                  Left = 2
                  Top = 274
                  Width = 440
                  Height = 17
                  Caption = '[F33] Shuttle 2DID Mapping checking function.'
                  TabOrder = 3
                end
                object cbF32: TCheckBox
                  Left = 3
                  Top = 245
                  Width = 441
                  Height = 24
                  Hint = '[L23]Hot Temp shuttle no add pos'
                  Caption = '[F32] Check in shuttle sensor by pass. Set value :'
                  TabOrder = 1
                end
                object edF32: TEdit
                  Tag = 30
                  Left = 314
                  Top = 245
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 2
                  Text = '10'
                end
                object cbF34: TCheckBox
                  Left = 2
                  Top = 300
                  Width = 440
                  Height = 17
                  Caption = 
                    '[F34] Output arm pick up error at shuttle, need trigger reset pr' +
                    'ocess.'
                  TabOrder = 4
                end
                object cbF35: TCheckBox
                  Left = 2
                  Top = 323
                  Width = 440
                  Height = 17
                  Caption = '[F35] Output shuttle lose IC, need trigger reset process.'
                  TabOrder = 5
                end
              end
            end
          end
        end
        object tsG00: TTabSheet
          Caption = 'G [ Visible ]'
          ImageIndex = 8
          object MemoG: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pal_G: TPanel
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            Align = alClient
            BevelInner = bvLowered
            Color = 12761254
            TabOrder = 0
            object cbG01: TCheckBox
              Left = 5
              Top = 10
              Width = 400
              Height = 17
              Caption = '[G01] Show test rate'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
            end
            object cbG04: TCheckBox
              Left = 5
              Top = 35
              Width = 392
              Height = 17
              Caption = '[G04] Show fail alarm count'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
            end
            object cbG05: TCheckBox
              Left = 5
              Top = 59
              Width = 392
              Height = 17
              Caption = '[G05] Show motor speed'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 2
            end
            object cbG06: TCheckBox
              Left = 5
              Top = 84
              Width = 392
              Height = 17
              Caption = '[G06] Home push Z1 start initial'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 3
            end
            object cbG07: TCheckBox
              Left = 5
              Top = 109
              Width = 392
              Height = 17
              Caption = '[G07] Support multi color for error bin'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 4
            end
            object cbG08: TCheckBox
              Left = 5
              Top = 133
              Width = 392
              Height = 17
              Caption = '[G08] Show '#39'Are you sure'#39' message after alarm.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 5
            end
            object cbG09: TCheckBox
              Left = 5
              Top = 158
              Width = 280
              Height = 21
              Caption = '[G09] Need password when edit site map'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 6
            end
            object cbG10: TCheckBox
              Left = 5
              Top = 187
              Width = 280
              Height = 21
              Caption = '[G10] Show immediate UPH'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 7
            end
            object cbG11: TCheckBox
              Left = 5
              Top = 215
              Width = 280
              Height = 21
              Caption = '[G11] ASE Report  record'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 8
            end
            object cbG12: TCheckBox
              Left = 4
              Top = 244
              Width = 397
              Height = 21
              Caption = '[G12] Contract high manual send test message'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 9
            end
            object cbG13: TCheckBox
              Left = 4
              Top = 272
              Width = 397
              Height = 21
              Caption = '[G13] Show Temp. offset on contact page.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 10
            end
            object cbG14: TCheckBox
              Left = 4
              Top = 301
              Width = 397
              Height = 21
              Caption = '[G14] Start-up warning                            sec'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 12
            end
            object cbG15: TCheckBox
              Left = 5
              Top = 330
              Width = 397
              Height = 21
              Caption = '[G15] Load Input Count'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 13
            end
            object chkG16: TCheckBox
              Left = 5
              Top = 358
              Width = 397
              Height = 21
              Caption = '[G16] Bin Display has communication error, need to alarm.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 14
            end
            object edG14: TEdit
              Left = 164
              Top = 300
              Width = 61
              Height = 24
              TabOrder = 11
              Text = '1'
            end
            object cbG17: TCheckBox
              Left = 5
              Top = 387
              Width = 397
              Height = 21
              Caption = '[G17]Load CCD map for Ase-Kh'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 15
            end
            object cbG17_1: TCheckBox
              Left = 24
              Top = 418
              Width = 397
              Height = 21
              Caption = '[G17-1]Load CCD map for tray end Ase-Kh'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 16
            end
            object G18: TGroupBox
              Left = 6
              Top = 444
              Width = 447
              Height = 121
              Caption = '[G18]Unload put empty tray'
              TabOrder = 17
              object Label32: TLabel
                Left = 198
                Top = 43
                Width = 111
                Height = 16
                Caption = 'Auto 1 Tray Count :'
              end
              object Label42: TLabel
                Left = 197
                Top = 68
                Width = 111
                Height = 16
                Caption = 'Auto 2 Tray Count :'
              end
              object Label43: TLabel
                Left = 196
                Top = 94
                Width = 111
                Height = 16
                Caption = 'Auto 3 Tray Count :'
              end
              object cbG18_1: TCheckBox
                Left = 5
                Top = 19
                Width = 397
                Height = 21
                Caption = '[G18-1] Use Tray Map'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 0
              end
              object cbG18_2: TCheckBox
                Left = 5
                Top = 38
                Width = 180
                Height = 21
                Caption = '[G18-2] Double unload tray'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
              end
              object edG18Auto1Count: TEdit
                Left = 312
                Top = 39
                Width = 93
                Height = 24
                TabOrder = 2
              end
              object edG18Auto2Count: TEdit
                Left = 312
                Top = 65
                Width = 93
                Height = 24
                TabOrder = 3
              end
              object edG18Auto3Count: TEdit
                Left = 312
                Top = 91
                Width = 93
                Height = 24
                TabOrder = 4
              end
            end
            object cbG19: TCheckBox
              Left = 5
              Top = 572
              Width = 397
              Height = 21
              Caption = '[G19]Shuttle sensor add Autoclean  Parmameter'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 18
            end
            object cbG20: TCheckBox
              Left = 5
              Top = 596
              Width = 397
              Height = 21
              Caption = '[G20]Fix full tray wait on manual position when using AGV robot'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 19
            end
            object cbG21: TCheckBox
              Left = 5
              Top = 621
              Width = 397
              Height = 21
              Caption = '[G21]Color full tray no alarm (XP,Telix)'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 20
            end
            object cbG22: TCheckBox
              Left = 5
              Top = 647
              Width = 397
              Height = 21
              Caption = '[G22] Notice takeout tray after contact mode.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 21
            end
            object cbG23: TCheckBox
              Left = 5
              Top = 668
              Width = 397
              Height = 21
              Caption = '[G23] Disable Show Function Status.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 22
            end
            object cbG24: TCheckBox
              Left = 5
              Top = 688
              Width = 397
              Height = 21
              Caption = '[G24] Disable Show SECS/GEM Status.'
              ParentShowHint = False
              ShowHint = True
              TabOrder = 23
            end
          end
        end
        object tsI00: TTabSheet
          Caption = 'I [ Tester ]'
          ImageIndex = 12
          object MemoI: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pal_I: TPanel
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            Align = alClient
            BevelInner = bvLowered
            Color = 12761254
            TabOrder = 0
            object pnlI17: TPanel
              Left = 2
              Top = 702
              Width = 475
              Height = 33
              Align = alBottom
              BevelOuter = bvNone
              Color = 12761254
              TabOrder = 1
              object cbI17: TCheckBox
                Left = 13
                Top = 8
                Width = 277
                Height = 17
                Caption = '[I17] Wait for change GPIB program'
                Color = 12761254
                ParentColor = False
                TabOrder = 0
              end
            end
            object pgI00: TPageControl
              Left = 2
              Top = 2
              Width = 475
              Height = 700
              ActivePage = tsI_50
              Align = alClient
              TabIndex = 4
              TabOrder = 0
              object tsI_01: TTabSheet
                Caption = '[I01] - [I22]'
                object pal_I01: TPanel
                  Left = 0
                  Top = 0
                  Width = 467
                  Height = 669
                  Align = alClient
                  BevelInner = bvLowered
                  Color = 12761254
                  TabOrder = 0
                  object cbI01: TCheckBox
                    Left = 5
                    Top = 10
                    Width = 345
                    Height = 17
                    Caption = '[I01] Enable tester finish then homing'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object cbI12: TCheckBox
                    Left = 5
                    Top = 245
                    Width = 401
                    Height = 17
                    Caption = '[I12] Tester time out ,don'#39't send START signal again'
                    Checked = True
                    State = cbChecked
                    TabOrder = 9
                  end
                  object cbI07: TCheckBox
                    Left = 5
                    Top = 168
                    Width = 393
                    Height = 17
                    Caption = '[I07] Reset GPIB after one cycle or clean out'
                    TabOrder = 6
                  end
                  object cbI16: TCheckBox
                    Left = 5
                    Top = 296
                    Width = 264
                    Height = 17
                    Caption = '[I16] TTL setting save to setup file.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 11
                  end
                  object cbI04: TCheckBox
                    Left = 5
                    Top = 87
                    Width = 312
                    Height = 17
                    Caption = '[I04] Enable change bin during pause'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                  object cbI18: TCheckBox
                    Left = 5
                    Top = 321
                    Width = 277
                    Height = 17
                    Caption = '[I18] Can receive ECHOSTOP'
                    TabOrder = 12
                  end
                  object cbI19: TCheckBox
                    Left = 5
                    Top = 367
                    Width = 372
                    Height = 17
                    Caption = '[I19] Auto site map pause change SIMULATE test bin data '
                    TabOrder = 13
                  end
                  object cbI02: TCheckBox
                    Left = 5
                    Top = 36
                    Width = 345
                    Height = 17
                    Caption = '[I02] After home,set socket IC to error bin'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                  end
                  object cbI03: TCheckBox
                    Left = 5
                    Top = 61
                    Width = 345
                    Height = 17
                    Caption = '[I03] Enable temperature control function'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object cbI13: TCheckBox
                    Left = 5
                    Top = 270
                    Width = 411
                    Height = 17
                    Caption = 
                      '[I13] Initial start delay fuinction setting different when FT an' +
                      'd RT.'
                    TabOrder = 10
                  end
                  object cbI08: TCheckBox
                    Left = 5
                    Top = 193
                    Width = 393
                    Height = 17
                    Caption = '[I08] Check 2DID function when initial start.'
                    TabOrder = 7
                  end
                  object cbI05: TCheckBox
                    Left = 5
                    Top = 112
                    Width = 430
                    Height = 21
                    Caption = '[I05] Low yield alarm forced one cycle'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                  object cbI06: TCheckBox
                    Left = 5
                    Top = 142
                    Width = 340
                    Height = 17
                    Caption = '[I06] Turn on the [I01] function after Home is complete'
                    TabOrder = 5
                  end
                  object cbI09: TCheckBox
                    Left = 5
                    Top = 219
                    Width = 393
                    Height = 17
                    Caption = '[I09] Yield alarm no need  clean shuttle.'
                    TabOrder = 8
                  end
                end
              end
              object tsI_20: TTabSheet
                Caption = '[I20] - [I30]'
                ImageIndex = 1
                object pal_I20: TPanel
                  Left = 0
                  Top = 0
                  Width = 467
                  Height = 669
                  Align = alClient
                  BevelInner = bvLowered
                  Color = 12761254
                  TabOrder = 0
                  object labI25: TLabel
                    Left = 28
                    Top = 422
                    Width = 304
                    Height = 16
                    Caption = '[I25] Format for get handler testing arm temperature.'
                  end
                  object labI20: TLabel
                    Left = 28
                    Top = 8
                    Width = 198
                    Height = 16
                    Caption = '[I20] The alphabet of the error bin.'
                  end
                  object LabelI22: TLabel
                    Left = 152
                    Top = 352
                    Width = 200
                    Height = 16
                    Caption = '[I22] Home delay time               sec.'
                  end
                  object cbI23: TCheckBox
                    Left = 8
                    Top = 374
                    Width = 233
                    Height = 17
                    Caption = '[I23] Hot test waiting mode'
                    TabOrder = 5
                  end
                  object cbI24: TCheckBox
                    Left = 8
                    Top = 398
                    Width = 229
                    Height = 17
                    Caption = '[I24] Stop all motor while testing'
                    TabOrder = 6
                  end
                  object coI25: TComboBox
                    Left = 164
                    Top = 442
                    Width = 249
                    Height = 24
                    ImeMode = imClose
                    ItemHeight = 16
                    TabOrder = 7
                    Text = 'With _    (Ex. DUAL_1X2_1_2_)'
                    Items.Strings = (
                      'With _    (Ex. DUAL_1X2_1_2_)'
                      'Without _ (Ex. DUAL1X2_1_2_)')
                  end
                  object cbI26: TCheckBox
                    Left = 8
                    Top = 472
                    Width = 394
                    Height = 17
                    Caption = '[I26] Close site have bin data need manual remove device.'
                    TabOrder = 8
                  end
                  object cbI28: TCheckBox
                    Left = 8
                    Top = 520
                    Width = 394
                    Height = 17
                    Caption = '[I28] On off sites on the fly'
                    TabOrder = 10
                    Visible = False
                  end
                  object cbI27: TCheckBox
                    Left = 8
                    Top = 496
                    Width = 394
                    Height = 17
                    Caption = '[I27] Manual sort mode'
                    TabOrder = 9
                  end
                  object cbI29: TCheckBox
                    Left = 8
                    Top = 548
                    Width = 329
                    Height = 17
                    Caption = '[I29] Enable yield record                         Sec'
                    TabOrder = 12
                  end
                  object edI29: TEdit
                    Tag = 120
                    Left = 176
                    Top = 545
                    Width = 61
                    Height = 24
                    TabOrder = 11
                    Text = '120'
                  end
                  object cbI30: TCheckBox
                    Left = 8
                    Top = 621
                    Width = 240
                    Height = 17
                    Caption = '[I30] Reset bin fail count number over.'
                    TabOrder = 16
                  end
                  object chkI29_1: TCheckBox
                    Left = 44
                    Top = 574
                    Width = 397
                    Height = 17
                    Caption = '[I29-1] Save yield data by socket by bin'
                    TabOrder = 13
                  end
                  object coI20: TComboBox
                    Left = 232
                    Top = 4
                    Width = 145
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 2
                    TabOrder = 0
                    Text = 'E'
                    Items.Strings = (
                      '0'
                      '16'
                      'E'
                      'Err'
                      'Error'
                      'Accroding to Tester')
                  end
                  object gbI21: TGroupBox
                    Left = 8
                    Top = 28
                    Width = 441
                    Height = 245
                    Caption = '[I21] Auto Site Mapping '
                    TabOrder = 1
                    Visible = False
                    object lbI21_6: TLabel
                      Left = 281
                      Top = 135
                      Width = 64
                      Height = 16
                      Caption = 'Error retry :'
                      Visible = False
                    end
                    object lblI21: TLabel
                      Left = 314
                      Top = 24
                      Width = 57
                      Height = 16
                      Caption = 'Fail retry :'
                    end
                    object cbI21_SkipSoakTime: TCheckBox
                      Left = 12
                      Top = 46
                      Width = 300
                      Height = 17
                      Caption = '[I21-2] Skip soak time'
                      TabOrder = 2
                    end
                    object edI21: TEdit
                      Tag = 120
                      Left = 212
                      Top = 63
                      Width = 61
                      Height = 24
                      TabOrder = 3
                      Text = '120'
                    end
                    object cbI21_SameSoakTime: TCheckBox
                      Left = 12
                      Top = 68
                      Width = 201
                      Height = 17
                      Caption = '[I21-3] Use same soak time'
                      TabOrder = 4
                    end
                    object cbI21: TCheckBox
                      Left = 12
                      Top = 24
                      Width = 300
                      Height = 17
                      Caption = '[I21-1] Enable auto site mapping function'
                      TabOrder = 1
                    end
                    object cbI21_CheckOpen: TCheckBox
                      Left = 12
                      Top = 91
                      Width = 300
                      Height = 17
                      Caption = '[I21-4] Check every dut should be open.'
                      TabOrder = 5
                    end
                    object cbI21_5: TCheckBox
                      Left = 12
                      Top = 113
                      Width = 300
                      Height = 17
                      Caption = '[I21-5] Remove loader tray manually.'
                      TabOrder = 6
                    end
                    object cbI21_6: TCheckBox
                      Left = 12
                      Top = 135
                      Width = 300
                      Height = 17
                      Caption = '[I21-6] Run time check'
                      TabOrder = 8
                    end
                    object cbI21_7: TCheckBox
                      Left = 12
                      Top = 158
                      Width = 300
                      Height = 17
                      Caption = '[I21-7] Bin IC combine place to Fix 2'
                      TabOrder = 9
                    end
                    object cbI21_8: TCheckBox
                      Left = 12
                      Top = 180
                      Width = 300
                      Height = 17
                      Caption = '[I21-8] Auto Site Mapping Use Hotplate'
                      TabOrder = 10
                    end
                    object cbI21_9: TCheckBox
                      Left = 12
                      Top = 202
                      Width = 285
                      Height = 17
                      Caption = '[I21-9] Enable Site Mapping Fail Bin setting'
                      TabOrder = 12
                    end
                    object edI21_9: TEdit
                      Tag = 15
                      Left = 300
                      Top = 198
                      Width = 33
                      Height = 24
                      TabOrder = 11
                      Text = '0'
                    end
                    object edI21_6: TEdit
                      Tag = 5
                      Left = 352
                      Top = 131
                      Width = 61
                      Height = 24
                      TabOrder = 7
                      Text = '3'
                    end
                    object cbI21_10: TCheckBox
                      Left = 12
                      Top = 223
                      Width = 285
                      Height = 17
                      Caption = '[I21-10] RT Mode Don'#39't Run Site Mapping'
                      TabOrder = 13
                    end
                    object edI21_1: TEdit
                      Tag = 5
                      Left = 375
                      Top = 20
                      Width = 61
                      Height = 24
                      TabOrder = 0
                      Text = '3'
                    end
                  end
                  object cbI22: TCheckBox
                    Left = 8
                    Top = 277
                    Width = 313
                    Height = 17
                    Caption = '[I22] Enable test time out option:'
                    TabOrder = 2
                  end
                  object rgI22: TRadioGroup
                    Left = 32
                    Top = 296
                    Width = 413
                    Height = 49
                    Caption = '[I22] Test time out can use '
                    Columns = 3
                    ItemIndex = 1
                    Items.Strings = (
                      'SKIP'
                      'RETRY'
                      'RETRY && SKIP'
                      'HOME')
                    TabOrder = 3
                  end
                  object cbI29_3: TCheckBox
                    Left = 8
                    Top = 594
                    Width = 368
                    Height = 26
                    BiDiMode = bdRightToLeft
                    Caption = 
                      '[I29-3] Enable yield record                         ea (By Total' +
                      ' Yield)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentBiDiMode = False
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 14
                  end
                  object edI29_3: TEdit
                    Tag = 1800
                    Left = 192
                    Top = 594
                    Width = 61
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 3
                    TabOrder = 15
                    Text = '100'
                  end
                  object edI22_1: TEdit
                    Tag = 15
                    Left = 287
                    Top = 348
                    Width = 33
                    Height = 24
                    TabOrder = 4
                    Text = '0'
                  end
                end
              end
              object tsI_30: TTabSheet
                Caption = '[I31]-[I40]'
                ImageIndex = 2
                object pal_I30: TPanel
                  Left = 0
                  Top = 0
                  Width = 467
                  Height = 669
                  Align = alClient
                  BevelInner = bvLowered
                  Color = 12761254
                  TabOrder = 0
                  object cbI32: TCheckBox
                    Left = 4
                    Top = 158
                    Width = 430
                    Height = 21
                    Caption = 
                      '[I32] Disable error bin setting. Error devices should take out m' +
                      'anually.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 1
                    Visible = False
                  end
                  object cbI33: TCheckBox
                    Left = 4
                    Top = 189
                    Width = 430
                    Height = 21
                    Caption = 
                      '[I33] Enable error bin box setting. Error devices put to bin box' +
                      '.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object cbI34: TCheckBox
                    Left = 4
                    Top = 220
                    Width = 430
                    Height = 21
                    Caption = 
                      '[I34] In the test relust all site are specific fail bin, show al' +
                      'arm.'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 3
                  end
                  object cbI35: TCheckBox
                    Left = 4
                    Top = 251
                    Width = 317
                    Height = 21
                    Caption = '[I35] Use Third Test Site (Engineer Access)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                  object cbI36: TCheckBox
                    Left = 4
                    Top = 282
                    Width = 430
                    Height = 21
                    Caption = '[I36] Tester timer out, manual take out on arm device.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 5
                  end
                  object grpI37: TGroupBox
                    Left = 4
                    Top = 312
                    Width = 445
                    Height = 141
                    Caption = '[I37] FIFO Mode'
                    TabOrder = 6
                    object imgI37_3: TImage
                      Left = 270
                      Top = 70
                      Width = 61
                      Height = 61
                      AutoSize = True
                      OnClick = imgI37_3Click
                    end
                    object cbI37_1: TCheckBox
                      Left = 8
                      Top = 32
                      Width = 197
                      Height = 17
                      Caption = '[I37-1] Enable  FIFO mode.'
                      TabOrder = 0
                    end
                    object cbI37_2: TCheckBox
                      Left = 8
                      Top = 60
                      Width = 197
                      Height = 17
                      Caption = '[I37-2] Enable site order link. '
                      TabOrder = 1
                    end
                    object cbI37_3: TCheckBox
                      Left = 8
                      Top = 88
                      Width = 221
                      Height = 17
                      Caption = '[I37-3] Lock loader sort direction'
                      TabOrder = 2
                    end
                    object cbI37_4: TCheckBox
                      Left = 8
                      Top = 116
                      Width = 221
                      Height = 17
                      Caption = '[I37-4] One by one test'
                      TabOrder = 3
                      Visible = False
                    end
                  end
                  object cbI39: TCheckBox
                    Left = 4
                    Top = 577
                    Width = 430
                    Height = 21
                    Caption = '[I39] Enabled Spirox Lot End and Full lot end command.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 8
                  end
                  object cbI40: TCheckBox
                    Left = 4
                    Top = 604
                    Width = 430
                    Height = 21
                    Caption = '[I40] Operator mode ->ON line'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 9
                  end
                  object grpI31: TGroupBox
                    Left = 8
                    Top = 4
                    Width = 425
                    Height = 109
                    Caption = '[I31] GPIB command'
                    TabOrder = 0
                    object cbI31_1: TCheckBox
                      Left = 12
                      Top = 23
                      Width = 240
                      Height = 17
                      Caption = '[I31-1] GPIB Lot End Command'
                      TabOrder = 0
                      Visible = False
                    end
                    object cbI31_2: TCheckBox
                      Left = 12
                      Top = 51
                      Width = 240
                      Height = 17
                      Caption = '[I31-2] GPIB Lot Start Command'
                      TabOrder = 1
                      Visible = False
                    end
                    object cbI31_3: TCheckBox
                      Left = 12
                      Top = 79
                      Width = 240
                      Height = 17
                      Caption = '[I31-3] GPIB Reset Command'
                      TabOrder = 2
                      Visible = False
                    end
                  end
                  object rgI38: TRadioGroup
                    Left = 4
                    Top = 484
                    Width = 397
                    Height = 89
                    Caption = '[I38] Format of SETTEMP?'
                    ItemIndex = 0
                    Items.Strings = (
                      'Settemp +XX.X'
                      '+XX.X'
                      'XX')
                    TabOrder = 7
                  end
                end
              end
              object tsI_40: TTabSheet
                Caption = '[I41]-[I50]'
                ImageIndex = 3
                object pal_I40: TPanel
                  Left = 0
                  Top = 0
                  Width = 467
                  Height = 669
                  Align = alClient
                  BevelInner = bvLowered
                  Color = 12761254
                  TabOrder = 0
                  object lblI49: TLabel
                    Left = 56
                    Top = 536
                    Width = 353
                    Height = 16
                    Caption = 
                      'change direct contact auto above  socket                        ' +
                      '     mm'
                  end
                  object grpI41: TGroupBox
                    Left = 2
                    Top = 2
                    Width = 463
                    Height = 275
                    Align = alTop
                    Caption = '[I41] Empty Socket Check Function'
                    TabOrder = 0
                    object lblI41: TLabel
                      Left = 36
                      Top = 232
                      Width = 145
                      Height = 16
                      AutoSize = False
                      Caption = 'Bin of ESC function :'
                    end
                    object cbI41: TCheckBox
                      Left = 11
                      Top = 28
                      Width = 264
                      Height = 17
                      Hint = 'I31'
                      Caption = '[I41] Enable Empty Socket Check'
                      ParentShowHint = False
                      ShowHint = True
                      TabOrder = 0
                    end
                    object cbI41_1: TCheckBox
                      Left = 37
                      Top = 53
                      Width = 105
                      Height = 17
                      Hint = 'I31_cb1'
                      Caption = 'Start of lot'
                      TabOrder = 1
                    end
                    object cbI41_2: TCheckBox
                      Left = 37
                      Top = 83
                      Width = 329
                      Height = 17
                      Hint = 'I31_cb2'
                      Caption = 'After the Chamber Door Open And Close'
                      TabOrder = 2
                    end
                    object cbI41_3: TCheckBox
                      Left = 37
                      Top = 113
                      Width = 217
                      Height = 17
                      Hint = 'I31_cb3'
                      Caption = 'After Contactor teminated'
                      TabOrder = 3
                    end
                    object cbI41_4: TCheckBox
                      Left = 37
                      Top = 143
                      Width = 209
                      Height = 17
                      Hint = 'I31_cb4'
                      Caption = 'After Contactor Jamming'
                      TabOrder = 4
                    end
                    object cbI41_5: TCheckBox
                      Left = 37
                      Top = 173
                      Width = 249
                      Height = 17
                      Hint = 'I31_cb5'
                      Caption = 'Regular execution Cycle (Times)'
                      TabOrder = 6
                    end
                    object edtI41_5: TEdit
                      Left = 285
                      Top = 169
                      Width = 57
                      Height = 24
                      Hint = 'I31_ed'
                      TabOrder = 5
                    end
                    object cbI41_6: TCheckBox
                      Left = 37
                      Top = 203
                      Width = 153
                      Height = 17
                      Hint = 'I31_cb6'
                      Caption = 'Manual Activation'
                      TabOrder = 7
                    end
                    object cbbI41: TComboBox
                      Left = 196
                      Top = 228
                      Width = 145
                      Height = 24
                      ItemHeight = 16
                      ItemIndex = 0
                      TabOrder = 8
                      Text = '0'
                      Items.Strings = (
                        '0'
                        '1'
                        '2'
                        '3'
                        '4'
                        '5'
                        '6'
                        '7'
                        '8'
                        '9'
                        '10'
                        '11'
                        '12'
                        '13'
                        '14'
                        '15'
                        '16')
                    end
                  end
                  object cbI42: TCheckBox
                    Left = 8
                    Top = 289
                    Width = 425
                    Height = 17
                    Caption = '[I42] Enable Barcode Flow Error Check'
                    TabOrder = 1
                  end
                  object cbI43: TCheckBox
                    Left = 8
                    Top = 312
                    Width = 268
                    Height = 21
                    Caption = '[I43] Reset GPIB after tray feed finish.'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                    Visible = False
                  end
                  object chkI44: TCheckBox
                    Left = 8
                    Top = 340
                    Width = 443
                    Height = 17
                    Caption = 
                      '[I44] Low Yield Alarm Interval Time By Setting(1~300)           ' +
                      '                sec'
                    TabOrder = 4
                  end
                  object edI44: TEdit
                    Tag = 10
                    Left = 350
                    Top = 337
                    Width = 61
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 4
                    TabOrder = 3
                    Text = '60'
                  end
                  object cbI45: TCheckBox
                    Left = 8
                    Top = 364
                    Width = 268
                    Height = 21
                    Caption = '[I45] Use 2DID Sorting'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 5
                    Visible = False
                  end
                  object rgI46: TRadioGroup
                    Left = 2
                    Top = 389
                    Width = 463
                    Height = 108
                    Caption = '[I46] Action when GPIB flow error (WAR07327 and WAR07328)'
                    ItemIndex = 1
                    Items.Strings = (
                      'None'
                      'Alarm with Skip then set IC in socket to error bin '
                      'Alarm with Retey to resend SOT.'
                      'Alarm with Skip and Retry.')
                    TabOrder = 6
                  end
                  object gbI50: TGroupBox
                    Left = 2
                    Top = 560
                    Width = 463
                    Height = 107
                    Align = alBottom
                    Caption = '[I50] Auto Site Mapping Trigger'
                    TabOrder = 7
                    object cbI50_StartLot: TCheckBox
                      Left = 45
                      Top = 49
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_startlot'
                      Caption = 'Start of lot'
                      TabOrder = 1
                    end
                    object cbI50_InitialStart: TCheckBox
                      Left = 193
                      Top = 49
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_initialstart'
                      Caption = 'Initial Start'
                      TabOrder = 2
                    end
                    object cbI50_OnyCycle: TCheckBox
                      Left = 341
                      Top = 49
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_onecycle'
                      Caption = 'OneCycle'
                      TabOrder = 3
                    end
                    object cbI50_Pause: TCheckBox
                      Left = 45
                      Top = 74
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_pause'
                      Caption = 'Pause'
                      TabOrder = 6
                    end
                    object cbI50_EnableASM_Trigger: TCheckBox
                      Left = 13
                      Top = 25
                      Width = 284
                      Height = 17
                      Hint = 'cbI50_startlot'
                      Caption = '[I50] Enable Auto Site Mapping Trigger'
                      TabOrder = 0
                    end
                    object cbI50_RT: TCheckBox
                      Left = 193
                      Top = 73
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_initialstart'
                      Caption = 'RT'
                      TabOrder = 4
                    end
                    object cbI50_TrayFeed: TCheckBox
                      Left = 341
                      Top = 73
                      Width = 105
                      Height = 17
                      Hint = 'cbI50_initialstart'
                      Caption = 'TrayFeed'
                      TabOrder = 5
                    end
                  end
                  object cbI49: TCheckBox
                    Left = 9
                    Top = 509
                    Width = 432
                    Height = 17
                    BiDiMode = bdRightToLeft
                    Caption = 
                      '[I49] Enable All unit to error bin for tester clean unit. If dro' +
                      'p contact auto '
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentBiDiMode = False
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 8
                  end
                  object edI49: TEdit
                    Tag = 1800
                    Left = 312
                    Top = 533
                    Width = 61
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 3
                    TabOrder = 9
                    Text = '100'
                  end
                end
              end
              object tsI_50: TTabSheet
                Caption = '[I51]-[I60]'
                ImageIndex = 4
                object cbI51: TCheckBox
                  Left = 4
                  Top = 12
                  Width = 430
                  Height = 21
                  Caption = 
                    '[I51] Reset not set error bin for device on input shuttle and in' +
                    'put arm.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbI52: TCheckBox
                  Left = 4
                  Top = 36
                  Width = 430
                  Height = 21
                  Caption = '[I52] Use AQL Sort Mode'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbI53: TCheckBox
                  Left = 4
                  Top = 64
                  Width = 443
                  Height = 17
                  Caption = 
                    '[I53]When stops for more than                       Sec, execute' +
                    ' Initial Start Delay '
                  TabOrder = 3
                end
                object edI53: TEdit
                  Tag = 10
                  Left = 204
                  Top = 61
                  Width = 60
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 4
                  TabOrder = 2
                  Text = '60'
                  Visible = False
                end
                object gbI54: TGroupBox
                  Left = 0
                  Top = 89
                  Width = 467
                  Height = 107
                  Caption = '[I54] Check the temperature during index arm testing'
                  TabOrder = 4
                  object cbI54_Enable: TCheckBox
                    Left = 13
                    Top = 25
                    Width = 284
                    Height = 17
                    Hint = 'cbI50_startlot'
                    Caption = '[I54] Enable'
                    TabOrder = 0
                  end
                  object cbI54_1: TCheckBox
                    Left = 37
                    Top = 45
                    Width = 420
                    Height = 17
                    Hint = 'I31_cb1'
                    Caption = '[I54-1] All ICs on the arm set error bin when temperature error'
                    TabOrder = 1
                  end
                  object cbI54_2: TCheckBox
                    Left = 37
                    Top = 69
                    Width = 420
                    Height = 17
                    Hint = 'I31_cb1'
                    Caption = '[I54-2] Only abnormal IC set error bin when temperature error'
                    TabOrder = 2
                  end
                end
              end
            end
          end
        end
        object tsL00: TTabSheet
          Caption = 'L [ Temperature ]'
          ImageIndex = 15
          object MemoL: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pcL00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = tsL_10
            Align = alClient
            TabIndex = 1
            TabOrder = 0
            TabWidth = 75
            object tsL_00: TTabSheet
              Caption = '[L01] - [L10]'
              object pal_L1: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labL04: TLabel
                  Left = 15
                  Top = 46
                  Width = 183
                  Height = 16
                  Caption = '[L04] Temperature range(2..10)'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL05: TLabel
                  Left = 15
                  Top = 76
                  Width = 235
                  Height = 16
                  Caption = '[L05] Chamber temperature range(2..30)'
                  ParentShowHint = False
                  ShowHint = True
                end
                object lblL06: TLabel
                  Left = 15
                  Top = 106
                  Width = 229
                  Height = 16
                  Caption = '[L06] Ambient temperature range(0..10)'
                  ParentShowHint = False
                  ShowHint = True
                end
                object cbL08: TLabel
                  Left = 15
                  Top = 160
                  Width = 222
                  Height = 16
                  Caption = '[L08] Socket temperature range(1..30)'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL08_Over: TLabel
                  Left = 272
                  Top = 160
                  Width = 29
                  Height = 16
                  Caption = 'Over'
                end
                object labL08_Under: TLabel
                  Left = 272
                  Top = 188
                  Width = 37
                  Height = 16
                  Caption = 'Under'
                end
                object lab2: TLabel
                  Left = 15
                  Top = 14
                  Width = 265
                  Height = 16
                  Caption = '[L03] Socket Air Cooling contact count trun on'
                  ParentShowHint = False
                  ShowHint = True
                end
                object edL04: TEdit
                  Tag = 10
                  Left = 258
                  Top = 42
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 2
                end
                object edL05: TEdit
                  Tag = 30
                  Left = 258
                  Top = 72
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 3
                end
                object gbL09: TGroupBox
                  Left = 8
                  Top = 212
                  Width = 364
                  Height = 241
                  Caption = '[L09] Tempture Position Shift'
                  TabOrder = 8
                  object labL09_Sh1R: TLabel
                    Left = 229
                    Top = 47
                    Width = 93
                    Height = 16
                    Caption = 'Shuttle 1 Right : '
                  end
                  object labL09_Sh1L: TLabel
                    Left = 13
                    Top = 43
                    Width = 83
                    Height = 16
                    Caption = 'Shuttle 1 Left : '
                  end
                  object labL09_Sh2L: TLabel
                    Left = 12
                    Top = 99
                    Width = 83
                    Height = 16
                    Caption = 'Shuttle 2 Left : '
                  end
                  object labL09_Sh2R: TLabel
                    Left = 228
                    Top = 99
                    Width = 93
                    Height = 16
                    Caption = 'Shuttle 2 Right : '
                  end
                  object labL09: TLabel
                    Left = 16
                    Top = 156
                    Width = 219
                    Height = 16
                    Caption = '* High tempture 130 deg Position shift'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clNavy
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object Label37: TLabel
                    Left = 13
                    Top = 181
                    Width = 142
                    Height = 16
                    Caption = '[L09-1]Hight Temp(165):'
                  end
                  object Label38: TLabel
                    Left = 229
                    Top = 181
                    Width = 132
                    Height = 16
                    Caption = '[L09-2]Low Temp(-45):'
                  end
                  object edL09_Sh1L: TEdit
                    Left = 13
                    Top = 63
                    Width = 121
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    TabOrder = 1
                    Text = '0'
                  end
                  object edL09_Sh1R: TEdit
                    Left = 229
                    Top = 63
                    Width = 121
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    TabOrder = 2
                    Text = '0'
                  end
                  object edL09_Sh2L: TEdit
                    Left = 12
                    Top = 119
                    Width = 121
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    TabOrder = 3
                    Text = '0'
                  end
                  object edL09_Sh2R: TEdit
                    Left = 228
                    Top = 119
                    Width = 121
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    TabOrder = 4
                    Text = '0'
                  end
                  object cbL09: TCheckBox
                    Left = 10
                    Top = 20
                    Width = 335
                    Height = 24
                    Hint = '[L23]Hot Temp shuttle no add pos'
                    Caption = '[L09] Hot Temp shuttle no add offset pos '
                    TabOrder = 0
                  end
                  object edtL09_1: TEdit
                    Left = 13
                    Top = 206
                    Width = 121
                    Height = 24
                    TabOrder = 5
                    Text = '15'
                  end
                  object edtL09_2: TEdit
                    Left = 229
                    Top = 206
                    Width = 121
                    Height = 24
                    TabOrder = 6
                    Text = '-40'
                  end
                end
                object cbL07: TCheckBox
                  Left = 14
                  Top = 133
                  Width = 303
                  Height = 17
                  Caption = '[L07] Use single limit'
                  TabOrder = 5
                end
                object edtL06: TEdit
                  Tag = 10
                  Left = 258
                  Top = 102
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 4
                end
                object edL08_Over: TEdit
                  Tag = 30
                  Left = 322
                  Top = 156
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 6
                end
                object rgL10: TRadioGroup
                  Left = 8
                  Top = 460
                  Width = 365
                  Height = 61
                  Caption = '[L10] Temperature record Interval'
                  Columns = 3
                  ItemIndex = 0
                  Items.Strings = (
                    '5  Sec'
                    '15 Sec'
                    '30 Sec'
                    '1  Min'
                    '5  Min'
                    '10 Min')
                  TabOrder = 9
                end
                object edL08_Under: TEdit
                  Tag = 30
                  Left = 322
                  Top = 184
                  Width = 55
                  Height = 24
                  MaxLength = 2
                  TabOrder = 7
                end
                object edL03: TEdit
                  Tag = 10
                  Left = 310
                  Top = 9
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 2
                  TabOrder = 0
                end
                object cbL03: TCheckBox
                  Left = 15
                  Top = 13
                  Width = 296
                  Height = 17
                  Caption = '[L03] Socket Air Cooling contact count trun on'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbL10: TCheckBox
                  Left = 8
                  Top = 528
                  Width = 369
                  Height = 17
                  Caption = '[L10-1] Index Test log temperature'
                  TabOrder = 10
                end
              end
            end
            object tsL_10: TTabSheet
              Caption = '[L11] - [L20]'
              ImageIndex = 1
              object pal_L2: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbL13: TCheckBox
                  Left = 14
                  Top = 352
                  Width = 407
                  Height = 17
                  Caption = '[L13] Hot plate and shuttle use same temperature offset'
                  TabOrder = 2
                end
                object cbL12: TCheckBox
                  Left = 14
                  Top = 324
                  Width = 303
                  Height = 17
                  Caption = '[L12] Temperature error no close heater power'
                  TabOrder = 1
                end
                object gbL11: TGroupBox
                  Left = 8
                  Top = 4
                  Width = 425
                  Height = 313
                  Caption = '[L11] ATC'
                  TabOrder = 0
                  object labL11_2: TLabel
                    Left = 152
                    Top = 112
                    Width = 104
                    Height = 16
                    Caption = ' Check time (Min).'
                  end
                  object labL11_1: TLabel
                    Left = 152
                    Top = 60
                    Width = 107
                    Height = 16
                    Caption = ' Check time (Sec).'
                  end
                  object labL11_4: TLabel
                    Left = 16
                    Top = 176
                    Width = 205
                    Height = 16
                    Caption = '[L11-4] ATC max temperature limit :'
                  end
                  object edL11_2: TEdit
                    Left = 266
                    Top = 108
                    Width = 55
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 4
                    Text = '20'
                  end
                  object cbL11_2: TCheckBox
                    Left = 11
                    Top = 81
                    Width = 298
                    Height = 24
                    Caption = '[L11-2] Enable Chiller Auto Close Protected.'
                    TabOrder = 3
                  end
                  object edL11_1: TEdit
                    Tag = 30
                    Left = 266
                    Top = 22
                    Width = 55
                    Height = 24
                    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                    MaxLength = 2
                    TabOrder = 0
                  end
                  object cbL11_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 249
                    Height = 17
                    Caption = '[L11-1] ATC temperature range(1..30)'
                    TabOrder = 1
                  end
                  object edL11_1_2: TEdit
                    Left = 266
                    Top = 56
                    Width = 55
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 2
                    Text = '20.0'
                  end
                  object edL11_4: TEdit
                    Tag = 150
                    Left = 266
                    Top = 172
                    Width = 55
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 3
                    TabOrder = 5
                    Text = '130'
                  end
                  object cbL11_6: TCheckBox
                    Left = 11
                    Top = 229
                    Width = 410
                    Height = 24
                    Caption = 
                      '[L11-6] ATC Temperature outside             Continuous          ' +
                      '  (s)Alarm'
                    Enabled = False
                    TabOrder = 8
                  end
                  object edL11_6_Outside: TEdit
                    Tag = 150
                    Left = 234
                    Top = 228
                    Width = 31
                    Height = 24
                    Enabled = False
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 3
                    TabOrder = 6
                    Text = '2'
                  end
                  object edL11_6_Continuous: TEdit
                    Tag = 150
                    Left = 334
                    Top = 228
                    Width = 31
                    Height = 24
                    Enabled = False
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 3
                    TabOrder = 7
                    Text = '3'
                  end
                  object edL11_7_MaxSurge: TEdit
                    Tag = 150
                    Left = 270
                    Top = 256
                    Width = 55
                    Height = 24
                    Enabled = False
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 3
                    TabOrder = 10
                    Text = '7'
                  end
                  object cbL11_7: TCheckBox
                    Left = 11
                    Top = 253
                    Width = 202
                    Height = 24
                    Caption = '[L11-7] ATC max peak alarm:'
                    Enabled = False
                    TabOrder = 9
                  end
                  object cbL11_8: TCheckBox
                    Left = 11
                    Top = 277
                    Width = 350
                    Height = 24
                    Caption = '[L11-8] Use temperature difference over setting alarm'
                    Enabled = False
                    TabOrder = 11
                  end
                end
                object cbL14: TCheckBox
                  Left = 14
                  Top = 380
                  Width = 411
                  Height = 17
                  Caption = 
                    '[L14] DUT temperature error will turn off the power until next s' +
                    'tart.'
                  TabOrder = 3
                end
                object cbL15: TCheckBox
                  Left = 13
                  Top = 410
                  Width = 416
                  Height = 17
                  Caption = '[L15] Chamber mode too low need wait initial wait time'
                  TabOrder = 4
                end
                object cbL17: TCheckBox
                  Left = 14
                  Top = 472
                  Width = 411
                  Height = 17
                  Caption = '[L17] Turn on all head heater when close site.'
                  TabOrder = 5
                end
                object cbL18: TCheckBox
                  Left = 14
                  Top = 500
                  Width = 411
                  Height = 17
                  Caption = '[L18] No full site add temperature offset'
                  TabOrder = 6
                end
                object cbL19: TCheckBox
                  Left = 14
                  Top = 525
                  Width = 439
                  Height = 17
                  Caption = '[L19] Keep heating when chamber door open without chamber heat'
                  TabOrder = 7
                end
                object cbL20: TCheckBox
                  Left = 14
                  Top = 553
                  Width = 411
                  Height = 17
                  Caption = '[L20] Abiemt guard band check'
                  TabOrder = 8
                end
              end
            end
            object tsL_20: TTabSheet
              Caption = '[L21] - [L30]'
              ImageIndex = 2
              object pal_L3: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object labL25_1: TLabel
                  Left = 27
                  Top = 136
                  Width = 236
                  Height = 16
                  Hint = 'LA28-lb1'
                  Caption = 'Ex : WorkFileName + "_" + Temperature'
                  ParentShowHint = False
                  ShowHint = True
                  Visible = False
                end
                object cbL21: TCheckBox
                  Left = 8
                  Top = 10
                  Width = 411
                  Height = 17
                  Caption = '[L21] Power OFF open chamber door, break all temperature power.'
                  TabOrder = 0
                end
                object cbL22: TCheckBox
                  Left = 8
                  Top = 31
                  Width = 441
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = '[L22] Enable 3 Sigma For Temperature Monitoring '
                  TabOrder = 1
                end
                object cbL24: TCheckBox
                  Left = 8
                  Top = 83
                  Width = 313
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = '[L24] Heater stable wait time                       sec'
                  TabOrder = 2
                end
                object edL24: TEdit
                  Tag = 1800
                  Left = 200
                  Top = 83
                  Width = 55
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 3
                  TabOrder = 3
                end
                object gbL30: TGroupBox
                  Left = 16
                  Top = 280
                  Width = 401
                  Height = 81
                  Caption = '[L30] Use 1 Cable Layout Kit By Configuration'
                  TabOrder = 7
                  object cbL30: TCheckBox
                    Left = 12
                    Top = 24
                    Width = 213
                    Height = 17
                    Caption = '[L30] Use by configuration'
                    TabOrder = 0
                  end
                  object cbL30_1: TCheckBox
                    Left = 36
                    Top = 48
                    Width = 213
                    Height = 17
                    Caption = '[L30-1] Use 1 Cable Layout Kit '
                    TabOrder = 1
                  end
                end
                object cbL29: TCheckBox
                  Left = 8
                  Top = 250
                  Width = 441
                  Height = 24
                  Hint = '[L23]Hot Temp shuttle no add pos'
                  Caption = '[L29] Ambient mode does not display temperature'
                  TabOrder = 6
                end
                object cbL25: TCheckBox
                  Left = 8
                  Top = 112
                  Width = 420
                  Height = 24
                  Hint = 'LA28'
                  Caption = '[L25] Change the format of Send Working file name to ATC '
                  TabOrder = 4
                end
                object cbL28: TCheckBox
                  Left = 8
                  Top = 218
                  Width = 361
                  Height = 24
                  Hint = '[L23]Hot Temp shuttle no add pos'
                  Caption = '[L28] Tempearture offset function use ready temp range'
                  TabOrder = 5
                end
              end
            end
            object tsL_30: TTabSheet
              Caption = '[L31] - [L32]'
              ImageIndex = 3
              object grpL31: TGroupBox
                Left = 0
                Top = 0
                Width = 471
                Height = 139
                Align = alTop
                Caption = '[L31]Enable ATC Temperature Over Upper Limit'
                TabOrder = 0
                Visible = False
                object cbL31_1: TCheckBox
                  Left = 4
                  Top = 15
                  Width = 429
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = 
                    '[L31-1] Enable ATC Temperature Over Upper Limit                 ' +
                    ' Alarm.'
                  TabOrder = 1
                end
                object edL31_1: TEdit
                  Tag = 1800
                  Left = 330
                  Top = 13
                  Width = 41
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 3
                  TabOrder = 0
                end
                object cbL31_2: TCheckBox
                  Left = 4
                  Top = 45
                  Width = 429
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = '[L31-2] ATC Temperature Over To Error Bin Only. '
                  TabOrder = 2
                end
                object cbL31_3: TCheckBox
                  Left = 4
                  Top = 75
                  Width = 429
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = 
                    '[L31-3] Enable ATC Temperature Over Lower Limit                 ' +
                    ' Alarm.'
                  TabOrder = 4
                end
                object cbL31_4: TCheckBox
                  Left = 4
                  Top = 105
                  Width = 429
                  Height = 24
                  Hint = '[LA27] Enable 3 Sigma For Temperature Monitoring '
                  Caption = '[L31-4] Enable ATC Temperature Offset Range '
                  TabOrder = 6
                end
                object edL31_3: TEdit
                  Tag = 1800
                  Left = 326
                  Top = 73
                  Width = 41
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 3
                  TabOrder = 3
                end
                object edL31_4: TEdit
                  Tag = 1800
                  Left = 306
                  Top = 101
                  Width = 41
                  Height = 24
                  ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                  MaxLength = 3
                  TabOrder = 5
                end
              end
              object grpL32: TGroupBox
                Left = 0
                Top = 139
                Width = 471
                Height = 338
                Hint = 'LA14-gb'
                Align = alTop
                Caption = '[L32] Defrost Function'
                TabOrder = 1
                Visible = False
                object grpL32_1: TGroupBox
                  Left = 2
                  Top = 18
                  Width = 467
                  Height = 53
                  Hint = 'L13'
                  Align = alTop
                  Caption = 'Manual Defrost'
                  TabOrder = 0
                  object cbL32_1: TCheckBox
                    Left = 10
                    Top = 20
                    Width = 420
                    Height = 24
                    Hint = 'LA12'
                    Caption = '[L32-1] Enable Manual Defrost Function'
                    TabOrder = 0
                  end
                end
                object grpL32_2: TGroupBox
                  Left = 2
                  Top = 71
                  Width = 467
                  Height = 58
                  Hint = 'L13'
                  Align = alTop
                  Caption = 'Automatic Defrost '
                  TabOrder = 1
                  object Label237: TLabel
                    Left = 5
                    Top = 50
                    Width = 3
                    Height = 16
                    Hint = 'M02'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object Label238: TLabel
                    Left = 10
                    Top = 114
                    Width = 357
                    Height = 16
                    Caption = 
                      '[LA14-3]Machine Stop At Low Temperature                   Minute' +
                      's'
                    ParentShowHint = False
                    ShowHint = True
                    Visible = False
                  end
                  object Label239: TLabel
                    Left = 10
                    Top = 144
                    Width = 380
                    Height = 16
                    Hint = 'M02'
                    Caption = 
                      '[LA14-4]Machine Produce At Low Temperature                   Min' +
                      'utes'
                    ParentShowHint = False
                    ShowHint = True
                    Visible = False
                  end
                  object cbL32_2: TCheckBox
                    Left = 10
                    Top = 20
                    Width = 420
                    Height = 24
                    Caption = '[L32-2] Enable Auto Defrost Function'
                    TabOrder = 0
                  end
                  object edtSetMachineStopLowTemperatureTime: TEdit
                    Left = 325
                    Top = 109
                    Width = 62
                    Height = 24
                    Enabled = False
                    TabOrder = 1
                    Text = '300'
                    Visible = False
                  end
                  object edtSetMachineProduceLowTemperatureTime: TEdit
                    Left = 350
                    Top = 139
                    Width = 62
                    Height = 24
                    Enabled = False
                    TabOrder = 2
                    Text = '600'
                    Visible = False
                  end
                end
                object grpL32_3: TGroupBox
                  Left = 2
                  Top = 129
                  Width = 467
                  Height = 172
                  Hint = 'L13'
                  Align = alTop
                  Caption = 'Defrost Parameter Setting'
                  TabOrder = 2
                  object labL32_4: TLabel
                    Left = 10
                    Top = 25
                    Width = 293
                    Height = 16
                    Caption = '[L32-4] Set Defrost Temperature                   Degree'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object labL32_5: TLabel
                    Left = 10
                    Top = 55
                    Width = 294
                    Height = 16
                    Hint = 'M02'
                    Caption = 
                      '[L32-5] Set Defrost Time                                   Minut' +
                      'es'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object labL32_6: TLabel
                    Left = 10
                    Top = 85
                    Width = 299
                    Height = 16
                    Hint = 'M02'
                    Caption = '[L32-6]Set Below Temperature                       Degree.'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object labL32_7: TLabel
                    Left = 10
                    Top = 113
                    Width = 354
                    Height = 16
                    Hint = 'M02'
                    Caption = 
                      '[L32-7]Set Time                                                 ' +
                      '   Hour.                Min.'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object labL32_8: TLabel
                    Left = 10
                    Top = 142
                    Width = 300
                    Height = 16
                    Hint = 'M02'
                    Caption = '[L32-8]Set Air Stream Temp                             Degree.'
                    ParentShowHint = False
                    ShowHint = True
                  end
                  object edL32_4: TEdit
                    Left = 204
                    Top = 20
                    Width = 45
                    Height = 24
                    TabOrder = 0
                    Text = '80'
                  end
                  object edL32_5: TEdit
                    Left = 204
                    Top = 50
                    Width = 45
                    Height = 24
                    TabOrder = 1
                    Text = '20'
                  end
                  object edL32_6: TEdit
                    Left = 204
                    Top = 80
                    Width = 45
                    Height = 24
                    Hint = 'tLowTemperatureOverSetTimeAlarm_SetDegree'
                    ReadOnly = True
                    TabOrder = 2
                    Text = '10'
                  end
                  object edL32_7_1: TEdit
                    Left = 204
                    Top = 108
                    Width = 45
                    Height = 24
                    ReadOnly = True
                    TabOrder = 3
                    Text = '0'
                  end
                  object edL32_7_2: TEdit
                    Left = 292
                    Top = 108
                    Width = 45
                    Height = 24
                    ReadOnly = True
                    TabOrder = 4
                    Text = '1'
                  end
                  object edL32_8: TEdit
                    Left = 204
                    Top = 137
                    Width = 45
                    Height = 24
                    Hint = 'tLowTemperatureOverSetTimeAlarm_SetDegree'
                    ReadOnly = True
                    TabOrder = 5
                    Text = '10'
                  end
                end
              end
            end
            object tsL_30_1: TTabSheet
              Caption = '[L33] - [L35]'
              ImageIndex = 4
              object grpL33: TGroupBox
                Left = 0
                Top = 0
                Width = 471
                Height = 201
                Hint = 'N09_gb'
                Align = alTop
                Caption = '[L33] Check Door Open Time  Over Set Must Be Alarms '
                TabOrder = 0
                Visible = False
                object labL33_3: TLabel
                  Left = 8
                  Top = 83
                  Width = 381
                  Height = 16
                  Hint = 'M02'
                  Caption = 
                    '[L33-3] Set Dig Door Check Time(Below 25 Degree)                ' +
                    ' Sec'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL33_4: TLabel
                  Left = 8
                  Top = 111
                  Width = 455
                  Height = 16
                  Hint = 'M02'
                  Caption = 
                    '[L33-4] Set Hatchway Check Time (Below25 Degree)               S' +
                    'ec(Small Door)'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL33_7: TLabel
                  Left = 8
                  Top = 141
                  Width = 460
                  Height = 16
                  Hint = 'M02'
                  Caption = 
                    '[L33-5] Set Check Door Open Over Time :Cold Temperature         ' +
                    '           Degree'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL33_8: TLabel
                  Left = 8
                  Top = 173
                  Width = 462
                  Height = 16
                  Hint = 'M02'
                  Caption = 
                    '[L33-6] Set Check Door Open Over Time :Hot Temperature          ' +
                    '             Degree'
                  ParentShowHint = False
                  ShowHint = True
                end
                object edL33_3: TEdit
                  Left = 323
                  Top = 78
                  Width = 38
                  Height = 24
                  Enabled = False
                  ReadOnly = True
                  TabOrder = 2
                  Text = '5'
                end
                object cbL33_1: TCheckBox
                  Left = 10
                  Top = 25
                  Width = 450
                  Height = 24
                  Hint = 'LA12'
                  Caption = '[L33-1] Enable Check Function( Only alarm)'
                  TabOrder = 0
                end
                object cbL33_2: TCheckBox
                  Left = 10
                  Top = 51
                  Width = 450
                  Height = 24
                  Hint = 'LA12'
                  Caption = '[L33-2] Enable Automatic Heating and Defrosting'
                  TabOrder = 1
                end
                object edL33_4: TEdit
                  Left = 325
                  Top = 108
                  Width = 35
                  Height = 24
                  Enabled = False
                  ReadOnly = True
                  TabOrder = 3
                  Text = '5'
                end
                object edL33_5: TEdit
                  Left = 370
                  Top = 136
                  Width = 47
                  Height = 24
                  Enabled = False
                  ReadOnly = True
                  TabOrder = 4
                  Text = '25'
                end
                object edL33_6: TEdit
                  Left = 370
                  Top = 168
                  Width = 47
                  Height = 24
                  Enabled = False
                  ReadOnly = True
                  TabOrder = 5
                  Text = '131'
                end
              end
              object grpL34: TGroupBox
                Left = 0
                Top = 201
                Width = 471
                Height = 185
                Hint = 'LA19-gb'
                Align = alTop
                Caption = '[L34] Delay Time After Fix Door Open'
                TabOrder = 1
                object labL34_2: TLabel
                  Left = 10
                  Top = 63
                  Width = 187
                  Height = 16
                  Hint = 'M02'
                  Caption = '[L34-2] Set Time                     Sec '
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL34_3: TLabel
                  Left = 10
                  Top = 95
                  Width = 232
                  Height = 16
                  Hint = 'M02'
                  Caption = '[L34-3] Set DewPoint                     Degree'
                  ParentShowHint = False
                  ShowHint = True
                end
                object labL34_4: TLabel
                  Left = 10
                  Top = 127
                  Width = 331
                  Height = 16
                  Hint = 'M02'
                  Caption = '[L34-4] Set Open Auto3 Track Flood Gate                     Sec'
                  ParentShowHint = False
                  ShowHint = True
                end
                object edL34_2: TEdit
                  Left = 113
                  Top = 58
                  Width = 44
                  Height = 24
                  TabOrder = 1
                  Text = '30'
                end
                object cbL34_1: TCheckBox
                  Left = 10
                  Top = 30
                  Width = 400
                  Height = 24
                  Hint = 'LA12'
                  Caption = '[L34-1] Enable this Function'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 0
                  Visible = False
                end
                object edL34_3: TEdit
                  Left = 141
                  Top = 90
                  Width = 48
                  Height = 24
                  TabOrder = 2
                  Text = '0'
                end
                object edL34_4: TEdit
                  Left = 263
                  Top = 122
                  Width = 44
                  Height = 24
                  TabOrder = 3
                  Text = '0'
                end
                object cbL34_5: TCheckBox
                  Left = 10
                  Top = 155
                  Width = 459
                  Height = 24
                  Hint = 'LA12'
                  Caption = '[L34-5] Open Fix tray area safe door cylinders automatically'
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 4
                end
              end
              object grpL35: TGroupBox
                Left = 0
                Top = 386
                Width = 471
                Height = 95
                Hint = 'LA20-gb'
                Align = alTop
                Caption = '[L35] More Than Set The Temperature To Turn On The Fan'
                TabOrder = 2
                object labL35_2: TLabel
                  Left = 10
                  Top = 65
                  Width = 277
                  Height = 16
                  Hint = 'M02'
                  Caption = '[L35-2] Set Temperature                             Degree'
                  ParentShowHint = False
                  ShowHint = True
                end
                object edL35_2: TEdit
                  Left = 169
                  Top = 60
                  Width = 62
                  Height = 24
                  TabOrder = 1
                  Text = '130'
                end
                object cbL35_1: TCheckBox
                  Left = 10
                  Top = 30
                  Width = 450
                  Height = 24
                  Hint = 'LA12'
                  Caption = '[L35-1] Enable Function'
                  TabOrder = 0
                  Visible = False
                end
                object cbOverSetTempMustOpenFan_UltraTempKitSupportAmbient: TCheckBox
                  Left = 10
                  Top = 200
                  Width = 567
                  Height = 24
                  Hint = 'LA12'
                  Caption = 
                    '[LA20-3] UltraHigh Temperature Kit Support  Ambient Mode(ATC Con' +
                    'trol)'
                  TabOrder = 3
                  Visible = False
                end
                object cbUseUltraHighTemperatureNormalTemperatureKIT: TCheckBox
                  Left = 10
                  Top = 200
                  Width = 559
                  Height = 24
                  Hint = 'LA12'
                  Caption = 
                    '[LA20-4] Use Ultra High Temperature / Normal Temperature Shared ' +
                    'KIT'
                  TabOrder = 2
                end
              end
            end
            object tsL_30_2: TTabSheet
              Caption = '[L36] - [L42]'
              ImageIndex = 5
              object labL40: TLabel
                Left = 4
                Top = 248
                Width = 343
                Height = 16
                Hint = 'LA03'
                Caption = '[L40] Immediate Temperature Exceed Range Show Alarm'
                ParentShowHint = False
                ShowHint = True
              end
              object labL41: TLabel
                Left = 4
                Top = 284
                Width = 421
                Height = 16
                Hint = 'L11'
                Caption = 
                  '[L41] ATC temperature exceed the scope                 seconds s' +
                  'how alarm.'
                ParentShowHint = False
                ShowHint = True
              end
              object grpL37: TGroupBox
                Left = 0
                Top = 0
                Width = 471
                Height = 90
                Hint = 'LA23_gb'
                Align = alTop
                Caption = '[L36]  Handler Control Heating Offset(Prevent frosting area)'
                TabOrder = 0
                object lblL37_3: TLabel
                  Left = 10
                  Top = 26
                  Width = 164
                  Height = 16
                  Hint = 'LA28-lb1'
                  Caption = '[L36-1]Tri Temp ATC Rang '
                  ParentShowHint = False
                  ShowHint = True
                end
                object lblL37_4: TLabel
                  Left = 10
                  Top = 60
                  Width = 178
                  Height = 16
                  Hint = 'LA28-lb1'
                  Caption = '[L36-2]Tri Temp Heater Rang '
                  ParentShowHint = False
                  ShowHint = True
                end
                object edtL36_1: TEdit
                  Left = 220
                  Top = 23
                  Width = 80
                  Height = 24
                  Enabled = False
                  TabOrder = 0
                  Text = '0'
                  Visible = False
                end
                object edtL36_2: TEdit
                  Left = 220
                  Top = 56
                  Width = 80
                  Height = 24
                  Enabled = False
                  TabOrder = 1
                  Text = '0'
                  Visible = False
                end
              end
              object cbL37: TCheckBox
                Left = 2
                Top = 98
                Width = 431
                Height = 24
                Hint = 'LA16'
                Caption = 
                  '[L37] Docking/OTD Area Sensor  Off  Must Stop Air Machine Functi' +
                  'on'
                TabOrder = 1
                Visible = False
              end
              object cbL38: TCheckBox
                Left = 3
                Top = 125
                Width = 420
                Height = 27
                Hint = 'C14'
                Caption = '[L38] Enable Check Air Stream Module Status'
                Color = 12761254
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 2
                Visible = False
              end
              object grpL39: TGroupBox
                Left = -1
                Top = 152
                Width = 472
                Height = 89
                Caption = '[L39 Waiting The Temperature In Range]'
                TabOrder = 3
                Visible = False
                object cbL39_1: TCheckBox
                  Left = 3
                  Top = 25
                  Width = 465
                  Height = 24
                  Hint = 'LA25'
                  Caption = 
                    '[L39-1] Enable operation after waiting for the Temperature In Ra' +
                    'nge'
                  TabOrder = 0
                end
                object cbL39_2: TCheckBox
                  Left = 3
                  Top = 57
                  Width = 465
                  Height = 24
                  Hint = 'LA25'
                  Caption = 
                    '[L39-2] Wait for the Temperature Stabilization Time             ' +
                    '     Sec'
                  TabOrder = 2
                end
                object edtL39_2: TEdit
                  Left = 324
                  Top = 54
                  Width = 45
                  Height = 24
                  Hint = 'Temp_TriTemp Out Shuttle Desoak Time'
                  ImeName = 'Chinese (Traditional) - Phonetic'
                  MaxLength = 2
                  TabOrder = 1
                  Text = '0'
                end
              end
              object edL41: TEdit
                Left = 257
                Top = 280
                Width = 38
                Height = 24
                Hint = 'LA04_ed'
                ImeName = 'Chinese (Traditional) - Phonetic'
                MaxLength = 2
                TabOrder = 5
                Text = '3'
                Visible = False
              end
              object cbL42: TCheckBox
                Left = 4
                Top = 313
                Width = 400
                Height = 24
                Hint = 'Temp_TriTemp Use Out Shuttle Desoak Time'
                Caption = '[L42] Use Out Shuttle Desoak Time                  Sec.'
                TabOrder = 7
                Visible = False
              end
              object edtL42: TEdit
                Left = 236
                Top = 312
                Width = 45
                Height = 24
                Hint = 'Temp_TriTemp Out Shuttle Desoak Time'
                ImeName = 'Chinese (Traditional) - Phonetic'
                MaxLength = 2
                TabOrder = 6
                Text = '0'
                Visible = False
              end
              object edL40: TEdit
                Left = 361
                Top = 244
                Width = 38
                Height = 24
                Hint = 'LA04_ed'
                ImeName = 'Chinese (Traditional) - Phonetic'
                MaxLength = 2
                TabOrder = 4
                Text = '7'
                Visible = False
              end
              object cbL46: TCheckBox
                Left = 4
                Top = 414
                Width = 441
                Height = 27
                Hint = 'C14'
                Caption = '[L46] Enable Air Stream Abnormal The Compressor Need Onecycle'
                Color = 12761254
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 13
                Visible = False
              end
              object cbL44: TCheckBox
                Left = 4
                Top = 360
                Width = 441
                Height = 27
                Hint = 'C14'
                Caption = '[L44] Set Cold Air Switch Temperature'
                Color = 12761254
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 9
                Visible = False
              end
              object cbL45: TCheckBox
                Left = 4
                Top = 387
                Width = 441
                Height = 27
                Hint = 'C14'
                Caption = '[L45] Set Dew Point Offset'
                Color = 12761254
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentColor = False
                ParentFont = False
                TabOrder = 11
                Visible = False
              end
              object edtL44: TEdit
                Left = 252
                Top = 361
                Width = 45
                Height = 24
                Hint = 'Temp_TriTemp Out Shuttle Desoak Time'
                ImeName = 'Chinese (Traditional) - Phonetic'
                MaxLength = 2
                TabOrder = 10
                Text = '0'
              end
              object edtL45: TEdit
                Left = 180
                Top = 387
                Width = 45
                Height = 24
                Hint = 'Temp_TriTemp Out Shuttle Desoak Time'
                ImeName = 'Chinese (Traditional) - Phonetic'
                MaxLength = 2
                TabOrder = 12
                Text = '0'
              end
              object cbL43: TCheckBox
                Left = 4
                Top = 339
                Width = 257
                Height = 17
                Caption = '[L43] Enable Power Follow Function'
                TabOrder = 8
              end
            end
          end
        end
        object tsO00: TTabSheet
          Caption = 'O [ Count ]'
          ImageIndex = 6
          object MemoO: TMemo
            Left = 453
            Top = 0
            Width = 468
            Height = 737
            Align = alClient
            Color = 14670284
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pcO00: TPageControl
            Left = 0
            Top = 0
            Width = 453
            Height = 737
            ActivePage = tsO_11
            Align = alLeft
            TabIndex = 1
            TabOrder = 0
            object tsO_00: TTabSheet
              Caption = '[O01] - [O10]'
              object pal_O1: TPanel
                Left = 0
                Top = 0
                Width = 445
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object lblO07: TLabel
                  Left = 224
                  Top = 490
                  Width = 45
                  Height = 16
                  Caption = 'FT Max'
                  Visible = False
                end
                object cbO02: TCheckBox
                  Left = 5
                  Top = 34
                  Width = 400
                  Height = 17
                  Caption = '[O02] [ RESET ] do not need clear hot plate'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbO05: TCheckBox
                  Left = 5
                  Top = 120
                  Width = 388
                  Height = 17
                  Caption = '[O05] [ RESET ] Need remove all tray on the handler'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbO01: TCheckBox
                  Left = 5
                  Top = 5
                  Width = 388
                  Height = 17
                  Caption = '[O01] [ RESET ] Clear and auto check hot plate matrix'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbO07: TCheckBox
                  Left = 9
                  Top = 489
                  Width = 192
                  Height = 17
                  Caption = '[O07]  [FT ]Can'#39't off continue'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object edO07: TEdit
                  Tag = 35
                  Left = 276
                  Top = 486
                  Width = 57
                  Height = 24
                  TabOrder = 4
                  Text = '0'
                end
                object cbO08: TCheckBox
                  Left = 9
                  Top = 516
                  Width = 388
                  Height = 17
                  Caption = '[O08] Continue fail by socket need PWD'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 6
                end
                object gbO06: TGroupBox
                  Left = 4
                  Top = 140
                  Width = 429
                  Height = 341
                  Caption = '[O06] Auto save event log'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  object labO06_2: TLabel
                    Left = 8
                    Top = 148
                    Width = 200
                    Height = 16
                    AutoSize = False
                    Caption = '[O06-5] Last Record Time'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object labO06_Remote: TLabel
                    Left = 176
                    Top = 184
                    Width = 105
                    Height = 16
                    Caption = 'Remote Directory'
                  end
                  object labO06_Local: TLabel
                    Left = 176
                    Top = 208
                    Width = 90
                    Height = 16
                    Caption = 'Local Directory'
                  end
                  object labO06_7: TLabel
                    Left = 8
                    Top = 228
                    Width = 179
                    Height = 16
                    Caption = '[O06-7] Weekday for save file '
                  end
                  object dtO06_LastDate: TDateTimePicker
                    Left = 201
                    Top = 148
                    Width = 100
                    Height = 24
                    CalAlignment = dtaLeft
                    Date = 40162.5441927662
                    Time = 40162.5441927662
                    DateFormat = dfShort
                    DateMode = dmComboBox
                    Enabled = False
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    Kind = dtkDate
                    ParseInput = False
                    ParentFont = False
                    TabOrder = 8
                  end
                  object cbO06: TCheckBox
                    Left = 8
                    Top = 21
                    Width = 200
                    Height = 26
                    Caption = '[O06-1] Enable Event log'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object edO06_FilePath: TEdit
                    Left = 224
                    Top = 22
                    Width = 200
                    Height = 24
                    TabOrder = 1
                    Text = 'D:\RMS'
                  end
                  object strngrdAutoSaveLog: TStringGrid
                    Left = 12
                    Top = 248
                    Width = 290
                    Height = 53
                    ColCount = 7
                    DefaultColWidth = 40
                    FixedCols = 0
                    RowCount = 2
                    TabOrder = 13
                    OnMouseDown = strngrdAutoSaveLogMouseDown
                    ColWidths = (
                      40
                      40
                      40
                      40
                      40
                      40
                      40)
                  end
                  object btnAutoSaveSetAll: TButton
                    Left = 308
                    Top = 276
                    Width = 65
                    Height = 25
                    Caption = 'Set All'
                    TabOrder = 14
                    OnClick = btnAutoSaveSetAllClick
                  end
                  object dtpO06NextTime: TDateTimePicker
                    Left = 316
                    Top = 148
                    Width = 109
                    Height = 24
                    CalAlignment = dtaLeft
                    Date = 41198.5638512616
                    Time = 41198.5638512616
                    DateFormat = dfShort
                    DateMode = dmComboBox
                    Kind = dtkTime
                    ParseInput = False
                    TabOrder = 9
                  end
                  object chkO06AlarmHistroy: TCheckBox
                    Left = 8
                    Top = 53
                    Width = 200
                    Height = 26
                    Caption = '[O06-2] Enable Alarm Histroy'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object edtO06AlarmHistroy: TEdit
                    Left = 224
                    Top = 54
                    Width = 200
                    Height = 24
                    TabOrder = 3
                    Text = 'D:\RMS'
                  end
                  object chkO06AlarmStatist: TCheckBox
                    Left = 8
                    Top = 85
                    Width = 200
                    Height = 26
                    Caption = '[O06-3] Enable Alarm Statist'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                  object edtO06AlarmStatist: TEdit
                    Left = 224
                    Top = 86
                    Width = 200
                    Height = 24
                    TabOrder = 5
                    Text = 'D:\RMS'
                  end
                  object chkO06UseNetDrive: TCheckBox
                    Left = 8
                    Top = 181
                    Width = 156
                    Height = 26
                    Caption = '[O06-6] Use Net Drive'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 10
                  end
                  object edtO06_Local: TEdit
                    Left = 292
                    Top = 206
                    Width = 131
                    Height = 24
                    TabOrder = 12
                    Text = '\\steven-pc\'#33258#30001#36575#39636
                  end
                  object edtO06_Remote: TEdit
                    Left = 292
                    Top = 182
                    Width = 131
                    Height = 24
                    TabOrder = 11
                    Text = 'Z:'
                  end
                  object chkO06Production: TCheckBox
                    Left = 8
                    Top = 117
                    Width = 217
                    Height = 26
                    Caption = '[O06-4] Enable Production Data'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 6
                  end
                  object edtO06Production: TEdit
                    Left = 224
                    Top = 118
                    Width = 200
                    Height = 24
                    TabOrder = 7
                    Text = 'D:\RMS'
                  end
                  object chkO06TimePeriod: TCheckBox
                    Left = 8
                    Top = 309
                    Width = 401
                    Height = 26
                    Caption = 
                      '[O06-8] Update Production Record every                         m' +
                      'inutes'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 15
                  end
                  object cbO06TimePeriod: TComboBox
                    Left = 277
                    Top = 310
                    Width = 60
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 0
                    TabOrder = 16
                    Text = '10'
                    Items.Strings = (
                      '10'
                      '30')
                  end
                end
                object cbO09: TCheckBox
                  Left = 9
                  Top = 542
                  Width = 388
                  Height = 13
                  Caption = '[O09] Initial start need ask'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbO10_EventLogSaver: TCheckBox
                  Left = 9
                  Top = 565
                  Width = 272
                  Height = 26
                  Caption = '[O10] Use event log saver program'
                  Enabled = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                end
              end
            end
            object tsO_11: TTabSheet
              Caption = '[O11] - [O20]'
              ImageIndex = 1
              object pal_O2: TPanel
                Left = 0
                Top = 0
                Width = 445
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object lblO17: TLabel
                  Left = 197
                  Top = 343
                  Width = 64
                  Height = 16
                  Caption = 'within (min)'
                end
                object btnRecordJamRateByTimeClear: TButton
                  Left = 360
                  Top = 18
                  Width = 65
                  Height = 25
                  Caption = 'Set All'
                  TabOrder = 2
                  OnClick = btnRecordJamRateByTimeClearClick
                end
                object cbO11: TCheckBox
                  Left = 9
                  Top = 17
                  Width = 344
                  Height = 26
                  Caption = '[O11] Record Jam Rate By Time :                       minutes.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object edtO11: TEdit
                  Tag = 35
                  Left = 232
                  Top = 18
                  Width = 57
                  Height = 24
                  TabOrder = 1
                  Text = '0'
                end
                object paLifeTime: TPanel
                  Left = 8
                  Top = 44
                  Width = 429
                  Height = 109
                  BevelOuter = bvNone
                  Caption = 'paLifeTime'
                  Color = 12761254
                  TabOrder = 3
                  Visible = False
                  object cbO12_LifeTimeCount: TCheckBox
                    Left = 1
                    Top = 5
                    Width = 344
                    Height = 26
                    BiDiMode = bdRightToLeft
                    Caption = '[O12] Use Head Condition1  Life Time Control .'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentBiDiMode = False
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object cbO13_LifeTimeCount: TCheckBox
                    Left = 1
                    Top = 41
                    Width = 344
                    Height = 26
                    BiDiMode = bdRightToLeft
                    Caption = '[O13] Use Head Condition1  Life Time Control .'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentBiDiMode = False
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 2
                  end
                  object edHeadCondition1: TEdit
                    Tag = 35
                    Left = 80
                    Top = 6
                    Width = 105
                    Height = 24
                    BiDiMode = bdRightToLeft
                    ParentBiDiMode = False
                    TabOrder = 1
                    OnClick = edA32_1Click
                  end
                  object edHeadCondition2: TEdit
                    Tag = 35
                    Left = 80
                    Top = 42
                    Width = 105
                    Height = 24
                    BiDiMode = bdRightToLeft
                    ParentBiDiMode = False
                    TabOrder = 3
                    OnClick = edA32_1Click
                  end
                  object cbO14_LifeTimeCount: TCheckBox
                    Left = 1
                    Top = 77
                    Width = 344
                    Height = 26
                    BiDiMode = bdRightToLeft
                    Caption = '[O14] Use Head Condition1  Life Time Control .'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clBlack
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentBiDiMode = False
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 4
                  end
                  object edHeadCondition3: TEdit
                    Tag = 35
                    Left = 80
                    Top = 78
                    Width = 105
                    Height = 24
                    BiDiMode = bdRightToLeft
                    ParentBiDiMode = False
                    TabOrder = 5
                    OnClick = edA32_1Click
                  end
                end
                object grpO15: TGroupBox
                  Left = 4
                  Top = 156
                  Width = 437
                  Height = 109
                  Caption = '[O15] Text event log file define'
                  TabOrder = 4
                  object lblO15_1: TLabel
                    Left = 16
                    Top = 28
                    Width = 156
                    Height = 16
                    Caption = '[O15-1] Saving file period :'
                  end
                  object cbbO15_1: TComboBox
                    Left = 188
                    Top = 24
                    Width = 145
                    Height = 24
                    ItemHeight = 16
                    TabOrder = 0
                    Text = 'By Lot'
                    Items.Strings = (
                      'Per hour'
                      'Every 2 hour'
                      'Every 4 hour'
                      'Every 6 hour'
                      'Every 8 hour'
                      'Every 12 hour'
                      'Per day'
                      'By Lot'
                      '')
                  end
                  object chkO15_2: TCheckBox
                    Left = 16
                    Top = 64
                    Width = 365
                    Height = 17
                    Caption = '[O15-2] File name include machine ID'
                    TabOrder = 1
                  end
                  object chkO15_3: TCheckBox
                    Left = 16
                    Top = 88
                    Width = 365
                    Height = 17
                    Caption = '[O15-3] Save same folder  '
                    TabOrder = 2
                  end
                end
                object chkO16: TCheckBox
                  Left = 9
                  Top = 274
                  Width = 316
                  Height = 26
                  Caption = '[O16] Continuously alarm need password. Count :'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 5
                end
                object edtO16: TEdit
                  Tag = 35
                  Left = 324
                  Top = 275
                  Width = 57
                  Height = 24
                  TabOrder = 6
                  Text = '3'
                end
                object chkO17: TCheckBox
                  Left = 9
                  Top = 309
                  Width = 265
                  Height = 21
                  Caption = '[O17] Use level [166] when alarm count '
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                  Visible = False
                end
                object edtO17_Count: TEdit
                  Left = 266
                  Top = 306
                  Width = 121
                  Height = 24
                  TabOrder = 7
                  Text = '5'
                end
                object edtO17_Time: TEdit
                  Left = 266
                  Top = 340
                  Width = 121
                  Height = 24
                  TabOrder = 9
                  Text = '30'
                end
                object cbO18: TCheckBox
                  Left = 9
                  Top = 381
                  Width = 288
                  Height = 26
                  Caption = '[O18] Safe door on/off duration detect. (hr)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                end
                object edtO18: TEdit
                  Tag = 35
                  Left = 300
                  Top = 382
                  Width = 57
                  Height = 24
                  TabOrder = 11
                  Text = '168'
                end
                object grpO16: TGroupBox
                  Left = 4
                  Top = 416
                  Width = 437
                  Height = 169
                  Caption = '[O19] Auto Record Report'
                  TabOrder = 12
                  object lblO19_6: TLabel
                    Left = 27
                    Top = 129
                    Width = 55
                    Height = 16
                    Caption = 'File Path:'
                  end
                  object cbO19_1: TCheckBox
                    Left = 24
                    Top = 32
                    Width = 81
                    Height = 17
                    Caption = 'Everyday'
                    TabOrder = 0
                  end
                  object cbO19_2: TCheckBox
                    Left = 24
                    Top = 64
                    Width = 97
                    Height = 17
                    Caption = 'Every week'
                    TabOrder = 2
                  end
                  object coO19_3: TComboBox
                    Left = 124
                    Top = 59
                    Width = 69
                    Height = 24
                    ItemHeight = 16
                    TabOrder = 1
                    Text = 'Sun.'
                    Items.Strings = (
                      'Sun.'
                      'Mon.'
                      'Tue.'
                      'Wed.'
                      'Thu.'
                      'Fri.'
                      'Sat.')
                  end
                  object cbO19_4: TCheckBox
                    Left = 24
                    Top = 96
                    Width = 97
                    Height = 17
                    Caption = 'Every month'
                    TabOrder = 4
                  end
                  object coO19_5: TComboBox
                    Left = 124
                    Top = 91
                    Width = 69
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 0
                    TabOrder = 3
                    Text = '1'
                    Items.Strings = (
                      '1'
                      '2'
                      '3'
                      '4'
                      '5'
                      '6'
                      '7'
                      '8'
                      '9'
                      '10'
                      '11'
                      '12'
                      '13'
                      '14'
                      '15'
                      '16'
                      '17'
                      '18'
                      '19'
                      '20'
                      '21'
                      '22'
                      '23'
                      '24'
                      '25'
                      '26'
                      '27'
                      '28'
                      '29'
                      '30')
                  end
                  object edtO19_6: TEdit
                    Left = 88
                    Top = 126
                    Width = 329
                    Height = 24
                    TabOrder = 5
                    Text = 'D:\HT9045_Log\OneDayProduct'
                  end
                end
                object cbO20: TCheckBox
                  Left = 5
                  Top = 597
                  Width = 296
                  Height = 26
                  BiDiMode = bdRightToLeft
                  Caption = '[O20] Input/output arm picker life time control.'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentBiDiMode = False
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 13
                end
                object cbO20_1: TCheckBox
                  Left = 37
                  Top = 623
                  Width = 296
                  Height = 26
                  BiDiMode = bdRightToLeft
                  Caption = '[O20-1] Clear data when initial start'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentBiDiMode = False
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 14
                end
              end
            end
            object tsO21: TTabSheet
              Caption = '[O21] - [O30]'
              ImageIndex = 2
              object cbO21: TCheckBox
                Left = 13
                Top = 13
                Width = 296
                Height = 26
                BiDiMode = bdRightToLeft
                Caption = '[O21] FT After tray end clear fail bin count'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 0
              end
              object chkO22: TCheckBox
                Left = 13
                Top = 41
                Width = 296
                Height = 26
                BiDiMode = bdRightToLeft
                Caption = '[O22] Clear tray count when double click.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
              end
              object chkO23: TCheckBox
                Left = 13
                Top = 69
                Width = 296
                Height = 26
                BiDiMode = bdRightToLeft
                Caption = '[O23] Lot ID input by barcode reader.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 2
              end
              object chkO24: TCheckBox
                Left = 13
                Top = 101
                Width = 296
                Height = 26
                BiDiMode = bdRightToLeft
                Caption = '[O24] Save production by Lot ID'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentBiDiMode = False
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 3
              end
            end
          end
        end
        object tsN00: TTabSheet
          Caption = 'N [ Network ]'
          ImageIndex = 10
          object MemoN: TMemo
            Left = 569
            Top = 116
            Width = 352
            Height = 621
            Align = alRight
            Color = 14670284
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 2
          end
          object pal_N: TPanel
            Left = 0
            Top = 116
            Width = 569
            Height = 621
            Align = alClient
            BevelInner = bvLowered
            Color = 12761254
            TabOrder = 1
            object pcN00: TPageControl
              Left = 2
              Top = 2
              Width = 565
              Height = 570
              ActivePage = tsN10
              Align = alClient
              MultiLine = True
              TabIndex = 5
              TabOrder = 0
              TabWidth = 55
              Visible = False
              object tsN05: TTabSheet
                Caption = '[N05]'
                object gbN05: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N05] RMS Setting for Recipe File'
                  TabOrder = 0
                  object labN05_1: TLabel
                    Left = 12
                    Top = 40
                    Width = 77
                    Height = 16
                    Caption = 'Upload path:'
                  end
                  object labN05_AmbTemp: TLabel
                    Left = 12
                    Top = 104
                    Width = 164
                    Height = 16
                    Caption = 'Ambient temperaure define:'
                  end
                  object labN05_DownPath: TLabel
                    Left = 12
                    Top = 64
                    Width = 66
                    Height = 16
                    Caption = 'Down path:'
                  end
                  object cbN05_EnableRMS: TCheckBox
                    Left = 12
                    Top = 20
                    Width = 269
                    Height = 17
                    Caption = 'Enable RMS connection'
                    TabOrder = 0
                  end
                  object edN05_RmsPath: TEdit
                    Left = 112
                    Top = 36
                    Width = 233
                    Height = 24
                    TabOrder = 1
                    Text = 'D:\RMS'
                  end
                  object edN05_AmbTemp: TEdit
                    Left = 188
                    Top = 100
                    Width = 77
                    Height = 24
                    TabOrder = 4
                    Text = '40'
                  end
                  object cbN05_CheckFile: TCheckBox
                    Left = 12
                    Top = 88
                    Width = 177
                    Height = 17
                    Caption = 'Enable check file'
                    TabOrder = 3
                  end
                  object edN05_DownPath: TEdit
                    Left = 112
                    Top = 60
                    Width = 233
                    Height = 24
                    TabOrder = 2
                    Text = 'D:\RMS'
                  end
                  object gbN05_1: TGroupBox
                    Left = 2
                    Top = 353
                    Width = 553
                    Height = 71
                    Align = alBottom
                    Caption = '[N05] ERMS'
                    TabOrder = 10
                    object labN05_2: TLabel
                      Left = 12
                      Top = 44
                      Width = 77
                      Height = 16
                      Caption = 'Upload path:'
                    end
                    object cbN05_1: TCheckBox
                      Left = 12
                      Top = 20
                      Width = 269
                      Height = 17
                      Caption = 'Enable RMS connection'
                      TabOrder = 0
                    end
                    object edN05_1: TEdit
                      Left = 112
                      Top = 40
                      Width = 233
                      Height = 24
                      TabOrder = 1
                      Text = 'D:\RMS'
                    end
                  end
                  object gbN05_WebService: TGroupBox
                    Left = 2
                    Top = 217
                    Width = 553
                    Height = 136
                    Align = alBottom
                    Caption = '[N05] Web Service'
                    TabOrder = 9
                    object labN05_UUID: TLabel
                      Left = 20
                      Top = 48
                      Width = 343
                      Height = 15
                      Caption = 'UUID     = {4A32CEB8-8260-53EA-F953-5D05A880B4F2}'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -12
                      Font.Name = 'Courier New'
                      Font.Style = []
                      ParentFont = False
                    end
                    object labN05_WSDL: TLabel
                      Left = 20
                      Top = 71
                      Width = 399
                      Height = 15
                      Caption = 'WSDL     = http://10.20.20.32/cimreport/ws/sip1.asmx?WSDL'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -12
                      Font.Name = 'Courier New'
                      Font.Style = []
                      ParentFont = False
                    end
                    object labN05_Version: TLabel
                      Left = 20
                      Top = 93
                      Width = 98
                      Height = 15
                      Caption = 'Version  = 1.0'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -12
                      Font.Name = 'Courier New'
                      Font.Style = []
                      ParentFont = False
                    end
                    object labN05_Revision: TLabel
                      Left = 20
                      Top = 116
                      Width = 161
                      Height = 15
                      Caption = 'Revision = 1.0.1.0.1.82'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -12
                      Font.Name = 'Courier New'
                      Font.Style = []
                      ParentFont = False
                    end
                    object cbN05_SCKWebService: TCheckBox
                      Left = 4
                      Top = 28
                      Width = 233
                      Height = 17
                      Caption = 'Enable web service'
                      TabOrder = 0
                    end
                  end
                  object groupbN05_RTC: TGroupBox
                    Left = 2
                    Top = 424
                    Width = 553
                    Height = 71
                    Align = alBottom
                    Caption = '[N05] RTC'
                    TabOrder = 11
                    object lblN05: TLabel
                      Left = 12
                      Top = 44
                      Width = 77
                      Height = 16
                      Caption = 'Upload path:'
                    end
                    object checkbN05_RTC: TCheckBox
                      Left = 12
                      Top = 20
                      Width = 269
                      Height = 17
                      Caption = 'Enable RTC alarm image upload'
                      TabOrder = 0
                    end
                    object editN05_RTC: TEdit
                      Left = 112
                      Top = 40
                      Width = 233
                      Height = 24
                      TabOrder = 1
                      Text = 'D:\RMS'
                    end
                  end
                  object btnMesSystem: TButton
                    Left = 284
                    Top = 100
                    Width = 89
                    Height = 25
                    Caption = 'Mes System'
                    TabOrder = 5
                    Visible = False
                    OnClick = btnMesSystemClick
                  end
                  object chkN05_TrayFeedClear: TCheckBox
                    Left = 12
                    Top = 120
                    Width = 269
                    Height = 17
                    Caption = 'After tray feed, clear device name'
                    TabOrder = 7
                  end
                  object rgUpDlMethod: TRadioGroup
                    Left = 8
                    Top = 144
                    Width = 537
                    Height = 33
                    Caption = 'Upload Download Method'
                    Columns = 2
                    Items.Strings = (
                      'Network'
                      'FTP')
                    TabOrder = 8
                  end
                  object btnUploadAll: TButton
                    Left = 380
                    Top = 100
                    Width = 89
                    Height = 25
                    Caption = 'Upload All'
                    TabOrder = 6
                    OnClick = btnUploadAllClick
                  end
                end
              end
              object tsN06: TTabSheet
                Caption = '[N06]'
                ImageIndex = 1
                object gbN06_FTP: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N06] FTP Setting for Recipe File'
                  TabOrder = 0
                  object labN06_Host: TLabel
                    Left = 16
                    Top = 131
                    Width = 31
                    Height = 16
                    Caption = 'Host:'
                  end
                  object labN06_DownloadPath: TLabel
                    Left = 16
                    Top = 160
                    Width = 94
                    Height = 16
                    Caption = 'Download Path:'
                  end
                  object labN06_UploadPath: TLabel
                    Left = 16
                    Top = 189
                    Width = 78
                    Height = 16
                    Caption = 'Upload Path:'
                  end
                  object labN06_UserName: TLabel
                    Left = 16
                    Top = 73
                    Width = 72
                    Height = 16
                    Caption = 'User Name:'
                  end
                  object labN06_Password: TLabel
                    Left = 16
                    Top = 102
                    Width = 63
                    Height = 16
                    Caption = 'Password:'
                  end
                  object labN06_TesterList: TLabel
                    Left = 16
                    Top = 300
                    Width = 68
                    Height = 16
                    Caption = 'Tester List :'
                  end
                  object labN06_TesterMap: TLabel
                    Left = 16
                    Top = 271
                    Width = 72
                    Height = 16
                    Caption = 'Tester Map:'
                  end
                  object labN06_FileName: TLabel
                    Left = 16
                    Top = 438
                    Width = 319
                    Height = 16
                    Caption = 
                      'FileName :                                                      ' +
                      '      _Date_LotID'
                    Visible = False
                  end
                  object Label1: TLabel
                    Left = 16
                    Top = 214
                    Width = 27
                    Height = 16
                    Caption = 'Port:'
                  end
                  object lblN06_Mode: TLabel
                    Left = 16
                    Top = 245
                    Width = 88
                    Height = 16
                    Caption = 'Transfer Mode'
                  end
                  object edN06_HostName: TEdit
                    Left = 116
                    Top = 129
                    Width = 250
                    Height = 24
                    TabOrder = 6
                  end
                  object edN06_DownPath: TEdit
                    Left = 116
                    Top = 157
                    Width = 250
                    Height = 24
                    TabOrder = 7
                  end
                  object edN06_UpLdPath: TEdit
                    Left = 116
                    Top = 185
                    Width = 250
                    Height = 24
                    TabOrder = 8
                  end
                  object gbN06_HddLevel: TRadioGroup
                    Left = 16
                    Top = 345
                    Width = 357
                    Height = 41
                    Caption = 'HD level setup'
                    Columns = 4
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ItemIndex = 0
                    Items.Strings = (
                      'Operator'
                      'Engineer'
                      'Supervisor'
                      'HonPrec')
                    ParentFont = False
                    TabOrder = 18
                  end
                  object gbN06_ServerLv: TRadioGroup
                    Left = 16
                    Top = 385
                    Width = 357
                    Height = 41
                    Caption = 'Server level setup'
                    Columns = 4
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ItemIndex = 0
                    Items.Strings = (
                      'Operator'
                      'Engineer'
                      'Supervisor'
                      'HonPrec')
                    ParentFont = False
                    TabOrder = 19
                  end
                  object edN06_UserName: TEdit
                    Left = 116
                    Top = 73
                    Width = 250
                    Height = 24
                    TabOrder = 4
                  end
                  object edN06_Password: TEdit
                    Left = 116
                    Top = 101
                    Width = 250
                    Height = 24
                    PasswordChar = '*'
                    TabOrder = 5
                  end
                  object cbN06_EnableFTP: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 141
                    Height = 17
                    Caption = 'Enable FTP'
                    TabOrder = 0
                    OnMouseDown = cbN06_EnableFTPMouseDown
                  end
                  object edN06_TestList: TEdit
                    Left = 116
                    Top = 296
                    Width = 221
                    Height = 24
                    TabOrder = 14
                  end
                  object btN06_TesterList: TButton
                    Left = 340
                    Top = 296
                    Width = 29
                    Height = 25
                    Caption = '...'
                    TabOrder = 15
                    OnClick = btN06_TesterListClick
                  end
                  object btN06_UpdateTesterList: TButton
                    Left = 224
                    Top = 325
                    Width = 149
                    Height = 25
                    Caption = 'Update Tester List'
                    TabOrder = 17
                  end
                  object edN06_TesterMap: TEdit
                    Left = 116
                    Top = 267
                    Width = 221
                    Height = 24
                    TabOrder = 12
                  end
                  object btN06_TesterMap: TButton
                    Left = 340
                    Top = 266
                    Width = 29
                    Height = 25
                    Caption = '...'
                    TabOrder = 11
                    OnClick = btN06_TesterMapClick
                  end
                  object cbN06_UseSystemCall: TCheckBox
                    Left = 16
                    Top = 321
                    Width = 201
                    Height = 17
                    Caption = 'Use System Call to UnZip'
                    TabOrder = 16
                  end
                  object edN06_FileName: TEdit
                    Left = 116
                    Top = 434
                    Width = 141
                    Height = 24
                    TabOrder = 20
                    Visible = False
                  end
                  object edN06_Port: TEdit
                    Left = 116
                    Top = 212
                    Width = 250
                    Height = 24
                    TabOrder = 9
                    OnClick = edN06_PortClick
                  end
                  object cbN06_UseBarcode: TCheckBox
                    Left = 140
                    Top = 28
                    Width = 165
                    Height = 17
                    Caption = 'Use Barcode Reader'
                    TabOrder = 1
                  end
                  object cbN06_1: TCheckBox
                    Left = 140
                    Top = 51
                    Width = 141
                    Height = 17
                    Caption = 'Use Password Path'
                    TabOrder = 3
                  end
                  object edN06_1: TEdit
                    Left = 280
                    Top = 45
                    Width = 161
                    Height = 24
                    TabOrder = 2
                    Text = 'edN06_1'
                  end
                  object cbbN06_Mode: TComboBox
                    Left = 116
                    Top = 239
                    Width = 369
                    Height = 24
                    ItemHeight = 16
                    ItemIndex = 0
                    TabOrder = 10
                    Text = 'ASCII - Sends data as ASCII text'
                    Items.Strings = (
                      'ASCII - Sends data as ASCII text'
                      'IMAGE - raw binary data in 8-bit bytes'
                      'BYTE - raw binary data using variable-length bytes')
                  end
                  object chkN06_Tester: TCheckBox
                    Left = 372
                    Top = 269
                    Width = 181
                    Height = 17
                    Caption = 'Include tester file'
                    TabOrder = 13
                  end
                end
              end
              object tsN07: TTabSheet
                Caption = '[N07]'
                ImageIndex = 2
                object gbN07: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N07] SECS GEM'
                  TabOrder = 0
                  object labN07_5: TLabel
                    Left = 40
                    Top = 189
                    Width = 209
                    Height = 16
                    Caption = 'Start check after                    sec  retry'
                  end
                  object Label33: TLabel
                    Left = 56
                    Top = 297
                    Width = 105
                    Height = 16
                    Caption = 'Delay Time(Sec.)'
                  end
                  object cbN07_EnableSecs: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 277
                    Height = 17
                    Caption = '[N07-1] Enable SECS GEM'
                    TabOrder = 0
                  end
                  object cbN07_EnableHostStart: TCheckBox
                    Left = 20
                    Top = 52
                    Width = 277
                    Height = 17
                    Caption = '[N07-2] Enable host control start'
                    TabOrder = 1
                  end
                  object pnlN07_2: TPanel
                    Left = 36
                    Top = 72
                    Width = 325
                    Height = 37
                    BevelOuter = bvNone
                    Color = 12761254
                    TabOrder = 2
                    object lblN07_2: TLabel
                      Left = 16
                      Top = 9
                      Width = 238
                      Height = 16
                      Caption = 'Start run check after                    sec  alarm'
                    end
                    object edN07_2: TEdit
                      Left = 136
                      Top = 5
                      Width = 45
                      Height = 24
                      TabOrder = 0
                      Text = '60'
                    end
                  end
                  object cbN07_EnableSecsOneCycle: TCheckBox
                    Left = 20
                    Top = 108
                    Width = 361
                    Height = 17
                    Caption = '[N07-3] When SECS GEM disconnect will auto one cycle'
                    TabOrder = 3
                  end
                  object cbN07_EnableSecsLotCheck: TCheckBox
                    Left = 20
                    Top = 136
                    Width = 361
                    Height = 17
                    Caption = '[N07-4] Enable lot check'
                    TabOrder = 4
                    Visible = False
                  end
                  object cbN07_EnableEmployeeCheak: TCheckBox
                    Left = 20
                    Top = 160
                    Width = 361
                    Height = 17
                    Caption = '[N07-5] Enable employee ID check'
                    TabOrder = 5
                    Visible = False
                    OnClick = cbN07_EnableEmployeeCheakClick
                  end
                  object edN07_5: TEdit
                    Left = 140
                    Top = 181
                    Width = 45
                    Height = 24
                    TabOrder = 6
                    Text = '60'
                  end
                  object chkN07_6: TCheckBox
                    Left = 20
                    Top = 220
                    Width = 361
                    Height = 17
                    Caption = '[N07-6] S7F3 / S7F5 include OS Tester Recipe'
                    TabOrder = 7
                    Visible = False
                  end
                  object lbledtN07_6: TLabeledEdit
                    Left = 232
                    Top = 244
                    Width = 121
                    Height = 24
                    EditLabel.Width = 69
                    EditLabel.Height = 16
                    EditLabel.Caption = 'Net Driver : '
                    LabelPosition = lpLeft
                    LabelSpacing = 3
                    TabOrder = 8
                    Text = 'Z:\'
                  end
                  object chkN07_7: TCheckBox
                    Left = 20
                    Top = 272
                    Width = 361
                    Height = 17
                    Caption = '[N07-7] S7F3 / S7F5 Recipe file send as binary.'
                    TabOrder = 10
                    Visible = False
                  end
                  object edtN07_7: TEdit
                    Left = 172
                    Top = 293
                    Width = 97
                    Height = 24
                    TabOrder = 11
                    Text = '60'
                  end
                  object cbN07_6CompressedFile: TCheckBox
                    Left = 364
                    Top = 244
                    Width = 141
                    Height = 17
                    Caption = 'compressed file'
                    TabOrder = 9
                    OnMouseDown = cbN06_EnableFTPMouseDown
                  end
                end
              end
              object tsN08: TTabSheet
                Caption = '[N08]'
                ImageIndex = 3
                object gbN08: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N08] Automation'
                  TabOrder = 0
                  object lbl_N18_OlpIP: TLabel
                    Left = 28
                    Top = 61
                    Width = 12
                    Height = 16
                    Caption = 'IP'
                  end
                  object lbl_N18_OlpPort: TLabel
                    Left = 28
                    Top = 97
                    Width = 24
                    Height = 16
                    Caption = 'Port'
                  end
                  object cbN08_1: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 277
                    Height = 17
                    Caption = '[N08-1] Save communication logs.'
                    TabOrder = 0
                  end
                  object edN08_2: TEdit
                    Left = 56
                    Top = 57
                    Width = 250
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                  end
                  object edN08_3: TEdit
                    Left = 56
                    Top = 93
                    Width = 250
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                    OnClick = edN06_PortClick
                  end
                end
              end
              object tsN09: TTabSheet
                Caption = '[N09]'
                ImageIndex = 12
                object grpN09: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N09] Lot count automation'
                  TabOrder = 0
                  object lblN09_Handler: TLabel
                    Left = 26
                    Top = 324
                    Width = 134
                    Height = 16
                    Caption = '[N09-6] Handler folder '
                  end
                  object lblTSV: TLabel
                    Left = 26
                    Top = 88
                    Width = 106
                    Height = 16
                    Caption = '[N09-3] TSV Port :'
                  end
                  object lblN07_7: TLabel
                    Left = 26
                    Top = 356
                    Width = 257
                    Height = 16
                    Caption = '[N09-7] Skip IP addr start from (CSV format):'
                  end
                  object chkN09: TCheckBox
                    Left = 8
                    Top = 24
                    Width = 409
                    Height = 17
                    Caption = '[N09-1] Enable lot count automation function'
                    TabOrder = 0
                  end
                  object edtN09_Handler: TEdit
                    Left = 180
                    Top = 320
                    Width = 250
                    Height = 24
                    TabOrder = 6
                  end
                  object edtN09_TSV: TEdit
                    Left = 300
                    Top = 80
                    Width = 81
                    Height = 24
                    TabOrder = 3
                    OnClick = edN06_PortClick
                  end
                  object edtN09_SearchTime: TEdit
                    Left = 300
                    Top = 52
                    Width = 82
                    Height = 24
                    TabOrder = 1
                  end
                  object grpN09_5: TGroupBox
                    Left = 4
                    Top = 163
                    Width = 449
                    Height = 149
                    Caption = '[N09-5] FTP Setting'
                    TabOrder = 5
                    object lblN09_5_Name: TLabel
                      Left = 12
                      Top = 21
                      Width = 72
                      Height = 16
                      Caption = 'User Name:'
                    end
                    object lblN09_5_Password: TLabel
                      Left = 12
                      Top = 53
                      Width = 63
                      Height = 16
                      Caption = 'Password:'
                    end
                    object lblN09_5_Host: TLabel
                      Left = 12
                      Top = 85
                      Width = 31
                      Height = 16
                      Caption = 'Host:'
                    end
                    object lblN09_5_Path: TLabel
                      Left = 12
                      Top = 117
                      Width = 78
                      Height = 16
                      Caption = 'Upload Path:'
                    end
                    object edtN09_5Host: TEdit
                      Left = 93
                      Top = 81
                      Width = 284
                      Height = 24
                      TabOrder = 2
                    end
                    object edtN09_5Password: TEdit
                      Left = 93
                      Top = 49
                      Width = 284
                      Height = 24
                      PasswordChar = '*'
                      TabOrder = 1
                    end
                    object edtN09_5User: TEdit
                      Left = 93
                      Top = 17
                      Width = 284
                      Height = 24
                      TabOrder = 0
                    end
                    object edtN09_5Path: TEdit
                      Left = 93
                      Top = 113
                      Width = 284
                      Height = 24
                      TabOrder = 3
                    end
                  end
                  object rgN09_4: TRadioGroup
                    Left = 4
                    Top = 110
                    Width = 449
                    Height = 49
                    Caption = '[N09-4] Upload Method'
                    Columns = 2
                    ItemIndex = 0
                    Items.Strings = (
                      'FTP'
                      'Net Drive')
                    TabOrder = 4
                  end
                  object edtN09_7: TEdit
                    Left = 180
                    Top = 376
                    Width = 250
                    Height = 24
                    TabOrder = 7
                    Text = '172,192'
                  end
                  object chkN09_2: TCheckBox
                    Left = 8
                    Top = 56
                    Width = 289
                    Height = 17
                    Caption = '[N09-2] Waiting TSV reply time out time (Sec)'
                    TabOrder = 2
                  end
                end
              end
              object tsN10: TTabSheet
                Caption = '[N10]'
                ImageIndex = 5
                object pgcN10_1: TPageControl
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  ActivePage = tsN10_11_20
                  Align = alClient
                  TabIndex = 1
                  TabOrder = 0
                  object tsN10_1_10: TTabSheet
                    Caption = '[N1]-[N10]'
                    object grpLogUploadToFTP: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 466
                      Align = alClient
                      Caption = '[N10] Log File Upload to Server'
                      TabOrder = 0
                      object cbN10_1: TCheckBox
                        Left = 8
                        Top = 24
                        Width = 441
                        Height = 17
                        Caption = '[N10-1] Enable Temperature && EP && ESD log upload to server.'
                        Checked = True
                        State = cbChecked
                        TabOrder = 0
                      end
                      object cbN10_2: TCheckBox
                        Left = 8
                        Top = 46
                        Width = 333
                        Height = 17
                        Caption = '[N10-2] Enable upload summary to server'
                        TabOrder = 1
                      end
                      object grpN10_FTP: TGroupBox
                        Left = 2
                        Top = 261
                        Width = 545
                        Height = 203
                        Align = alBottom
                        Caption = '[N10-5] FTP Setting'
                        TabOrder = 6
                        object lblN10UserName: TLabel
                          Left = 10
                          Top = 21
                          Width = 72
                          Height = 16
                          Caption = 'User Name:'
                        end
                        object lblN10Password: TLabel
                          Left = 10
                          Top = 53
                          Width = 63
                          Height = 16
                          Caption = 'Password:'
                        end
                        object lblN10Host: TLabel
                          Left = 10
                          Top = 85
                          Width = 31
                          Height = 16
                          Caption = 'Host:'
                        end
                        object lblN10UploadPath: TLabel
                          Left = 10
                          Top = 117
                          Width = 78
                          Height = 16
                          Caption = 'Upload Path:'
                        end
                        object lblUploadRealPathName: TLabel
                          Left = 10
                          Top = 174
                          Width = 110
                          Height = 16
                          Caption = 'Upload Real Path:'
                        end
                        object lblUploadRealPath: TLabel
                          Left = 122
                          Top = 175
                          Width = 84
                          Height = 16
                          Caption = '____________'
                        end
                        object edN10Host: TEdit
                          Left = 93
                          Top = 81
                          Width = 284
                          Height = 24
                          TabOrder = 2
                        end
                        object edN10Password: TEdit
                          Left = 93
                          Top = 49
                          Width = 284
                          Height = 24
                          PasswordChar = '*'
                          TabOrder = 1
                        end
                        object edN10UserName: TEdit
                          Left = 93
                          Top = 17
                          Width = 284
                          Height = 24
                          TabOrder = 0
                        end
                        object edN10UploadPath: TEdit
                          Left = 93
                          Top = 113
                          Width = 284
                          Height = 24
                          TabOrder = 3
                        end
                        object chkN10_Passive: TCheckBox
                          Left = 15
                          Top = 148
                          Width = 137
                          Height = 17
                          Caption = 'Passive mode'
                          TabOrder = 5
                        end
                        object edtN10_Port: TLabeledEdit
                          Left = 256
                          Top = 144
                          Width = 121
                          Height = 24
                          EditLabel.Width = 30
                          EditLabel.Height = 16
                          EditLabel.Caption = 'Port: '
                          LabelPosition = lpLeft
                          LabelSpacing = 3
                          TabOrder = 4
                          Text = '21'
                        end
                      end
                      object cbN10_3: TCheckBox
                        Left = 8
                        Top = 68
                        Width = 377
                        Height = 17
                        Caption = '[N10-3] Enable daily upload production status to server'
                        Checked = True
                        State = cbChecked
                        TabOrder = 2
                      end
                      object rgN10_4: TRadioGroup
                        Left = 2
                        Top = 221
                        Width = 545
                        Height = 40
                        Align = alBottom
                        Caption = '[N10-4] Upload Method'
                        Columns = 2
                        ItemIndex = 0
                        Items.Strings = (
                          'FTP'
                          'Net Drive')
                        TabOrder = 5
                      end
                      object rgN10_3_1: TRadioGroup
                        Left = 2
                        Top = 173
                        Width = 545
                        Height = 48
                        Align = alBottom
                        Caption = '[N10-3-1] Upload Time Priod Method'
                        Columns = 4
                        ItemIndex = 0
                        Items.Strings = (
                          '00:00~24:00'
                          '08:00~20:00'
                          'Per Hour'
                          'Specified Time')
                        TabOrder = 4
                      end
                      object dtpN10_3_1_SpecifiedTime: TDateTimePicker
                        Left = 396
                        Top = 71
                        Width = 149
                        Height = 27
                        CalAlignment = dtaLeft
                        Date = 40162.5441927662
                        Time = 40162.5441927662
                        ShowCheckbox = True
                        Color = 14670284
                        DateFormat = dfShort
                        DateMode = dmComboBox
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -16
                        Font.Name = 'Calibri'
                        Font.Style = []
                        Kind = dtkTime
                        ParseInput = False
                        ParentFont = False
                        TabOrder = 3
                      end
                    end
                  end
                  object tsN10_11_20: TTabSheet
                    Caption = '[N11]-[N20]'
                    ImageIndex = 1
                    object lblN10_6: TLabel
                      Left = 24
                      Top = 1
                      Width = 313
                      Height = 16
                      Caption = 
                        '[N10-6] Interval time for upload to host                       s' +
                        'ec'
                    end
                    object lblN10DataType: TLabel
                      Left = 24
                      Top = 31
                      Width = 187
                      Height = 16
                      Caption = '[N10-7] Upload date folder type'
                    end
                    object lblN10_8: TLabel
                      Left = 24
                      Top = 60
                      Width = 137
                      Height = 16
                      Caption = '[N10-8] Net derive path'
                    end
                    object edtN10_6: TEdit
                      Left = 253
                      Top = -3
                      Width = 60
                      Height = 24
                      TabOrder = 0
                      Text = '60'
                    end
                    object coN10DataType: TComboBox
                      Left = 224
                      Top = 27
                      Width = 153
                      Height = 24
                      ItemHeight = 16
                      ItemIndex = 0
                      TabOrder = 1
                      Text = 'yyyy_'#24180
                      Items.Strings = (
                        'yyyy_'#24180
                        'yyyymm_'#24180#26376
                        'yyyymmdd_'#24180#26376#26085)
                    end
                    object edtN10_8: TEdit
                      Left = 168
                      Top = 56
                      Width = 200
                      Height = 24
                      TabOrder = 2
                      Text = 'D:\RMS'
                    end
                    object chkN10_9: TCheckBox
                      Left = 24
                      Top = 90
                      Width = 333
                      Height = 17
                      Caption = '[N10-9] Upload unloader tray data to FTP'
                      TabOrder = 3
                    end
                    object cbN10_12: TCheckBox
                      Left = 24
                      Top = 120
                      Width = 441
                      Height = 17
                      Caption = '[N10-12] Enable Upload GPIB'
                      TabOrder = 4
                    end
                    object cbN10_11: TCheckBox
                      Left = 24
                      Top = 151
                      Width = 441
                      Height = 17
                      Caption = '[N10-11] Enable Upload EventLog'
                      TabOrder = 5
                    end
                  end
                end
              end
              object tsN11: TTabSheet
                Caption = '[N11]'
                ImageIndex = 6
                object gbN11: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N11]  eKeeper'
                  TabOrder = 0
                  object cbN11_1: TCheckBox
                    Left = 8
                    Top = 24
                    Width = 409
                    Height = 17
                    Caption = '[N11_1] Remote control clean out and close site.'
                    TabOrder = 0
                  end
                end
              end
              object tsN12: TTabSheet
                Caption = '[N12]'
                ImageIndex = 6
                object gbN12_SocketIdProductDataUpoadToFTP: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N12] Socket ID Product Data Upload To FTP'
                  TabOrder = 0
                  object labN12_UserName: TLabel
                    Left = 16
                    Top = 56
                    Width = 72
                    Height = 16
                    Caption = 'User Name:'
                  end
                  object labN12_Password: TLabel
                    Left = 16
                    Top = 85
                    Width = 63
                    Height = 16
                    Caption = 'Password:'
                  end
                  object labN12_Host: TLabel
                    Left = 16
                    Top = 114
                    Width = 31
                    Height = 16
                    Caption = 'Host:'
                  end
                  object labN12_DownloadPath: TLabel
                    Left = 16
                    Top = 172
                    Width = 78
                    Height = 16
                    Caption = 'Upload Path:'
                  end
                  object chkN12: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 381
                    Height = 17
                    Caption = 'Enable Scoket ID Product Data Upload to FTP'
                    TabOrder = 0
                  end
                  object edN12_UserName: TEdit
                    Left = 116
                    Top = 56
                    Width = 250
                    Height = 24
                    TabOrder = 1
                  end
                  object edN12_Password: TEdit
                    Left = 116
                    Top = 84
                    Width = 250
                    Height = 24
                    PasswordChar = '*'
                    TabOrder = 2
                  end
                  object edN12_HostName: TEdit
                    Left = 116
                    Top = 112
                    Width = 250
                    Height = 24
                    TabOrder = 3
                  end
                  object edN12_UpLdPath: TEdit
                    Left = 116
                    Top = 168
                    Width = 250
                    Height = 24
                    TabOrder = 4
                  end
                end
              end
              object tsN13: TTabSheet
                Caption = '[N13]'
                ImageIndex = 7
                object grpASEMNetworkDrive: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N13] ASEM Network Drive'
                  TabOrder = 0
                  object cbN13_EnableARMSFunction: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 381
                    Height = 17
                    Caption = 'Enable ARMS Function'
                    TabOrder = 0
                  end
                end
              end
              object tsN14: TTabSheet
                Caption = '[N14]'
                ImageIndex = 8
                object grpN14: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Hint = 'N11_gb'
                  Align = alClient
                  Caption = '[N14] Handler OEE Function Setting'
                  TabOrder = 0
                  object pgcN14_1: TPageControl
                    Left = 2
                    Top = 18
                    Width = 553
                    Height = 477
                    ActivePage = tsN14_23
                    Align = alClient
                    TabIndex = 5
                    TabOrder = 0
                    object tsN14_1: TTabSheet
                      Caption = '[N01]-[N06]'
                      object lblN14_4MOFilePath: TLabel
                        Left = 16
                        Top = 261
                        Width = 70
                        Height = 16
                        Caption = 'MO file path'
                      end
                      object lblN14_5: TLabel
                        Left = 4
                        Top = 290
                        Width = 194
                        Height = 16
                        Caption = '[N14-5] Pause interval time (Sec)'
                      end
                      object cbN14_1: TCheckBox
                        Left = 4
                        Top = 8
                        Width = 441
                        Height = 17
                        Caption = 
                          '[N14-1] Use handler OEE function       Record cycle time        ' +
                          '         Sec'
                        TabOrder = 1
                      end
                      object edtN14_1: TEdit
                        Left = 351
                        Top = 2
                        Width = 46
                        Height = 24
                        Hint = 'N11_ed1'
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 0
                      end
                      object grpN14_3: TGroupBox
                        Left = 0
                        Top = 75
                        Width = 446
                        Height = 153
                        Caption = '[N14-3] OEE FTP'
                        TabOrder = 4
                        object lblN14_3HandlerOEEUploadPath: TLabel
                          Left = 16
                          Top = 121
                          Width = 77
                          Height = 16
                          Caption = 'Upload path:'
                        end
                        object lblN14_3HandlerOEEHost: TLabel
                          Left = 56
                          Top = 97
                          Width = 31
                          Height = 16
                          Caption = 'Host:'
                        end
                        object lblN14_3HandlerOEEPassword: TLabel
                          Left = 24
                          Top = 69
                          Width = 63
                          Height = 16
                          Caption = 'Password:'
                        end
                        object lblN14_3HandlerOEEUserName: TLabel
                          Left = 16
                          Top = 45
                          Width = 69
                          Height = 16
                          Caption = 'User name:'
                        end
                        object edtN14_3Path: TEdit
                          Left = 100
                          Top = 118
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 4
                        end
                        object edtN14_3Host: TEdit
                          Left = 100
                          Top = 91
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                        end
                        object edtN14_3Password: TEdit
                          Left = 100
                          Top = 65
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          PasswordChar = '*'
                          TabOrder = 2
                        end
                        object edtN14_3UserName: TEdit
                          Left = 100
                          Top = 40
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                        end
                        object cbN14_3: TCheckBox
                          Left = 12
                          Top = 20
                          Width = 277
                          Height = 17
                          Caption = 'Handler OEE FTP upload'
                          TabOrder = 0
                        end
                      end
                      object cbN14_2: TCheckBox
                        Left = 4
                        Top = 32
                        Width = 257
                        Height = 17
                        Caption = '[N14-2] Save production data to path'
                        TabOrder = 2
                      end
                      object edtN14_2: TEdit
                        Left = 28
                        Top = 49
                        Width = 413
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 3
                      end
                      object cbN14_4: TCheckBox
                        Left = 4
                        Top = 232
                        Width = 277
                        Height = 17
                        Caption = '[N14-4] Handler OEE auto load MO file'
                        TabOrder = 5
                      end
                      object edtN14_4: TEdit
                        Left = 100
                        Top = 256
                        Width = 333
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 6
                      end
                      object edtN14_5: TEdit
                        Left = 218
                        Top = 286
                        Width = 61
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 7
                      end
                      object grpN14_6: TGroupBox
                        Left = 0
                        Top = 370
                        Width = 545
                        Height = 76
                        Align = alBottom
                        Caption = '[N14_6] Yield Control'
                        TabOrder = 8
                        object lblN14_6Alarm5_BySiteIntervalContactCnt: TLabel
                          Left = 8
                          Top = 21
                          Width = 148
                          Height = 16
                          Caption = 'By site interval count (ea)'
                        end
                        object lblN14_6Alarm5_BySiteCmpYield: TLabel
                          Left = 8
                          Top = 52
                          Width = 152
                          Height = 16
                          Caption = 'By site compare yield (%)'
                        end
                        object lblN14_6Alarm5_BySiteAlarmYieldRate: TLabel
                          Left = 262
                          Top = 52
                          Width = 61
                          Height = 16
                          Caption = 'Alarm rate'
                        end
                        object lblN14_6Alarm5_BySiteLowYieldRate: TLabel
                          Left = 262
                          Top = 21
                          Width = 83
                          Height = 16
                          Caption = 'Yield rate  (%)'
                        end
                        object edtN14_6_1: TEdit
                          Left = 174
                          Top = 17
                          Width = 61
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 0
                          Visible = False
                        end
                        object edtN14_6_4: TEdit
                          Left = 362
                          Top = 48
                          Width = 61
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Visible = False
                        end
                        object edtN14_6_2: TEdit
                          Left = 362
                          Top = 17
                          Width = 61
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Visible = False
                        end
                        object edtN14_6_3: TEdit
                          Left = 174
                          Top = 48
                          Width = 61
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Visible = False
                        end
                      end
                    end
                    object tsN14_2: TTabSheet
                      Caption = '[N07]-[N13]'
                      ImageIndex = 1
                      object lblN14_7: TLabel
                        Left = 40
                        Top = 26
                        Width = 87
                        Height = 16
                        Caption = 'Check file path'
                      end
                      object lblN14_8: TLabel
                        Left = 40
                        Top = 75
                        Width = 94
                        Height = 16
                        Caption = 'Upload file path'
                      end
                      object lblN14_9: TLabel
                        Left = 40
                        Top = 125
                        Width = 94
                        Height = 16
                        Caption = 'Upload file path'
                      end
                      object lblN14_12_2: TLabel
                        Left = 395
                        Top = 248
                        Width = 45
                        Height = 16
                        Caption = 'second'
                      end
                      object lblN14_12_1: TLabel
                        Left = 296
                        Top = 248
                        Width = 35
                        Height = 16
                        Caption = 'Every'
                      end
                      object lblN14_12_3: TLabel
                        Left = 24
                        Top = 280
                        Width = 74
                        Height = 16
                        Caption = 'Upload path'
                      end
                      object lblN14_13_2: TLabel
                        Left = 395
                        Top = 316
                        Width = 45
                        Height = 16
                        Caption = 'second'
                      end
                      object lblN14_13_1: TLabel
                        Left = 296
                        Top = 316
                        Width = 35
                        Height = 16
                        Caption = 'Every'
                      end
                      object lblN14_13_3: TLabel
                        Left = 24
                        Top = 344
                        Width = 74
                        Height = 16
                        Caption = 'Upload path'
                      end
                      object cbN14_10: TCheckBox
                        Left = 0
                        Top = 154
                        Width = 281
                        Height = 17
                        Caption = '[N14-10] Auto download setup file by MO'
                        TabOrder = 6
                      end
                      object cbbN14_10_OEEFTPDownloadList: TComboBox
                        Left = 144
                        Top = 175
                        Width = 249
                        Height = 28
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -17
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ItemHeight = 0
                        ParentFont = False
                        TabOrder = 7
                        Visible = False
                      end
                      object cbN14_7: TCheckBox
                        Left = 4
                        Top = 2
                        Width = 277
                        Height = 17
                        Caption = '[N14-7] Check auto motive approve'
                        TabOrder = 0
                      end
                      object edtN14_7: TEdit
                        Left = 144
                        Top = 22
                        Width = 250
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 1
                      end
                      object cbN14_8: TCheckBox
                        Left = 4
                        Top = 51
                        Width = 277
                        Height = 17
                        Caption = '[N14-8] Upload setup condition'
                        TabOrder = 2
                      end
                      object edtN14_8: TEdit
                        Left = 144
                        Top = 71
                        Width = 250
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 3
                      end
                      object cbN14_9: TCheckBox
                        Left = 4
                        Top = 101
                        Width = 317
                        Height = 17
                        Caption = '[N14-9] Upload device quantity compare report'
                        TabOrder = 4
                      end
                      object edtN14_9: TEdit
                        Left = 144
                        Top = 121
                        Width = 250
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 5
                      end
                      object cbN14_11: TCheckBox
                        Left = 4
                        Top = 218
                        Width = 317
                        Height = 17
                        Caption = '[N14-11] Do auto check site map by MO'
                        TabOrder = 8
                      end
                      object cbN14_12: TCheckBox
                        Left = 4
                        Top = 248
                        Width = 277
                        Height = 17
                        Caption = '[N14-12] Temperature log upload to FTP'
                        TabOrder = 10
                      end
                      object edtN14_12: TEdit
                        Left = 335
                        Top = 244
                        Width = 58
                        Height = 24
                        ImeName = 'Chinese (Traditional) - Phonetic'
                        TabOrder = 9
                        Text = '1'
                      end
                      object edtN14_12_Path: TEdit
                        Left = 105
                        Top = 276
                        Width = 300
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 11
                      end
                      object cbN14_13: TCheckBox
                        Left = 4
                        Top = 316
                        Width = 273
                        Height = 17
                        Caption = '[N14-13] Test bin quantity upload to FTP'
                        TabOrder = 13
                      end
                      object edtN14_13: TEdit
                        Left = 336
                        Top = 312
                        Width = 57
                        Height = 24
                        ImeName = 'Chinese (Traditional) - Phonetic'
                        TabOrder = 12
                        Text = '1'
                      end
                      object edtN14_13_Path: TEdit
                        Left = 105
                        Top = 340
                        Width = 300
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 14
                      end
                    end
                    object tsN14_3: TTabSheet
                      Caption = '[N14]-[N19]'
                      ImageIndex = 2
                      object pnlN14_3: TPanel
                        Left = 0
                        Top = 0
                        Width = 545
                        Height = 125
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 0
                        object lblN14_14_2: TLabel
                          Left = 4
                          Top = 63
                          Width = 131
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Message file path'
                        end
                        object lblN14_14_3: TLabel
                          Left = 4
                          Top = 95
                          Width = 131
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Flag file path'
                        end
                        object lblN14_14_1: TLabel
                          Left = 4
                          Top = 31
                          Width = 131
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Execut file path'
                        end
                        object cbN14_14: TCheckBox
                          Left = 6
                          Top = 1
                          Width = 361
                          Height = 29
                          Caption = '[N14-14] Use alarm control machine'
                          TabOrder = 0
                        end
                        object edtN14_14_2: TEdit
                          Left = 137
                          Top = 61
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Text = 'C:\GTK_Control\Message\Alarm\'
                        end
                        object edtN14_14_3: TEdit
                          Left = 137
                          Top = 93
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Text = 'C:\GTK_Control\Flag\'
                        end
                        object edtN14_14_1: TEdit
                          Left = 137
                          Top = 29
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = 'C:\GTK\EMG.exe'
                        end
                      end
                      object pnlN14_15: TPanel
                        Left = 0
                        Top = 125
                        Width = 545
                        Height = 96
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 1
                        object lblN14_15_2: TLabel
                          Left = 4
                          Top = 65
                          Width = 131
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Message file path'
                        end
                        object lblN14_15_1: TLabel
                          Left = 4
                          Top = 33
                          Width = 131
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Execut file path'
                        end
                        object cbN14_15: TCheckBox
                          Left = 6
                          Top = 3
                          Width = 431
                          Height = 29
                          Caption = '[N14-15] Socket life time count control report upload'
                          TabOrder = 0
                        end
                        object edtN14_15_2: TEdit
                          Left = 137
                          Top = 63
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Text = 'C:\GTK_Control\Message\count\'
                        end
                        object edtN14_15_1: TEdit
                          Left = 137
                          Top = 31
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = 'C:\GTK\Lifetime_count.exe'
                        end
                      end
                      object pnlN14_17: TPanel
                        Left = 0
                        Top = 221
                        Width = 545
                        Height = 36
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 2
                        object lblN14_17: TLabel
                          Left = 12
                          Top = 9
                          Width = 262
                          Height = 16
                          Caption = '[N14-17] Ambient temperature upper (Temp)'
                        end
                        object edtN14_17: TEdit
                          Left = 278
                          Top = 4
                          Width = 41
                          Height = 24
                          TabOrder = 0
                          Text = '39'
                        end
                      end
                      object pnlN14_18: TPanel
                        Left = 0
                        Top = 257
                        Width = 545
                        Height = 72
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 3
                        object lblN14_18: TLabel
                          Left = 40
                          Top = 35
                          Width = 90
                          Height = 16
                          Caption = 'Download path'
                        end
                        object cbN14_18: TCheckBox
                          Left = 6
                          Top = 3
                          Width = 323
                          Height = 29
                          Caption = '[N14-18] Temperature Tj offset by  tool database'
                          TabOrder = 0
                        end
                        object edtN14_18: TEdit
                          Left = 137
                          Top = 31
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = '\Handler_data\Taj_offset_Tool\'
                        end
                      end
                      object pnlN14_19: TPanel
                        Left = 0
                        Top = 329
                        Width = 545
                        Height = 72
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 4
                        object lblN14_19: TLabel
                          Left = 40
                          Top = 35
                          Width = 74
                          Height = 16
                          Caption = 'Upload path'
                        end
                        object cbN14_19: TCheckBox
                          Left = 6
                          Top = 3
                          Width = 323
                          Height = 29
                          Caption = '[N14-19] Tray mapping log upload to FTP'
                          TabOrder = 0
                        end
                        object edtN14_19: TEdit
                          Left = 137
                          Top = 31
                          Width = 300
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = '\TrayMapping\'
                        end
                      end
                    end
                    object tsN14_4: TTabSheet
                      Caption = 'IPSC'
                      ImageIndex = 3
                      object grpN14_16: TGroupBox
                        Left = 0
                        Top = 0
                        Width = 545
                        Height = 410
                        Align = alClient
                        Caption = '[N14_16] IPSC control funciton'
                        TabOrder = 0
                        object lblN14_16_2: TLabel
                          Left = 8
                          Top = 88
                          Width = 160
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Flag Ini File Path Name : '
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object lblN14_16_1: TLabel
                          Left = 8
                          Top = 56
                          Width = 160
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Execute File Path Name : '
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object lblN14_16_3: TLabel
                          Left = 8
                          Top = 120
                          Width = 160
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          Caption = 'Product Data File Path : '
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object lblIPSCOut_Qty: TLabel
                          Left = 8
                          Top = 232
                          Width = 46
                          Height = 16
                          Caption = 'Out Qty:'
                        end
                        object lblIPSCIn_Qty: TLabel
                          Left = 8
                          Top = 208
                          Width = 36
                          Height = 16
                          Caption = 'In Qty:'
                        end
                        object lblIPSC_Qty: TLabel
                          Left = 8
                          Top = 184
                          Width = 23
                          Height = 16
                          Caption = 'Qty:'
                        end
                        object lblIPSCClear_Qty: TLabel
                          Left = 8
                          Top = 160
                          Width = 58
                          Height = 16
                          Caption = 'Clear Qty:'
                        end
                        object lblN14_16_4: TLabel
                          Left = 179
                          Top = 159
                          Width = 257
                          Height = 20
                          AutoSize = False
                          Caption = 'Record Cycle Time :                   min'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                        end
                        object cbN14_16: TCheckBox
                          Left = 6
                          Top = 18
                          Width = 279
                          Height = 29
                          Caption = 'Enable IPSC control funciton'
                          TabOrder = 0
                        end
                        object edtN14_16_2: TEdit
                          Left = 170
                          Top = 84
                          Width = 250
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Text = 'C:\GTK_Control\Flag\IPSC.ini'
                        end
                        object edtN14_16_1: TEdit
                          Left = 170
                          Top = 52
                          Width = 250
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = 'C:\GTK\IPSC.exe'
                        end
                        object edtN14_16_3: TEdit
                          Left = 170
                          Top = 116
                          Width = 250
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Text = 'C:\GTK_CONTROL\MESSAGE\'
                        end
                        object cb_DisableAlarm2LowYields: TCheckBox
                          Left = 8
                          Top = 256
                          Width = 305
                          Height = 17
                          Caption = 'Disable Alarm 2 Low Yields'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 8
                          Visible = False
                        end
                        object cb_DisableAlarm3: TCheckBox
                          Left = 8
                          Top = 288
                          Width = 305
                          Height = 17
                          Caption = 'Disable Alarm 3'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 9
                          Visible = False
                        end
                        object cb_DisableAlarm4: TCheckBox
                          Left = 8
                          Top = 320
                          Width = 305
                          Height = 17
                          Caption = 'Disable Alarm 4'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 10
                          Visible = False
                        end
                        object cb_DisableAlarm5: TCheckBox
                          Left = 8
                          Top = 352
                          Width = 305
                          Height = 17
                          Caption = 'Disable Alarm 5'
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 11
                          Visible = False
                        end
                        object btnSetIPSCCleaarQty: TButton
                          Left = 176
                          Top = 224
                          Width = 75
                          Height = 25
                          Caption = 'Set Clear '
                          TabOrder = 7
                          OnClick = btnSetIPSCCleaarQtyClick
                        end
                        object edtSetIPSCQty: TEdit
                          Left = 260
                          Top = 207
                          Width = 41
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 6
                          Text = '50'
                        end
                        object btnSetIPSCQty: TButton
                          Left = 176
                          Top = 192
                          Width = 75
                          Height = 25
                          Caption = 'Set Qty'
                          TabOrder = 5
                          OnClick = btnSetIPSCQtyClick
                        end
                        object edtN14_16_4: TEdit
                          Left = 308
                          Top = 155
                          Width = 41
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 4
                          Text = '2'
                        end
                      end
                    end
                    object tsN14_5: TTabSheet
                      Caption = '[N20]'
                      ImageIndex = 4
                      object Panel3: TPanel
                        Left = 0
                        Top = 0
                        Width = 545
                        Height = 121
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 0
                        object lblN14_20: TLabel
                          Left = 12
                          Top = 35
                          Width = 77
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          BiDiMode = bdLeftToRight
                          Caption = 'Log file path'
                          ParentBiDiMode = False
                        end
                        object cbN14_20: TCheckBox
                          Left = 6
                          Top = 3
                          Width = 431
                          Height = 29
                          Caption = '[N14-20] Default Recipe Chane Log'
                          TabOrder = 0
                        end
                        object edN14_20: TEdit
                          Left = 105
                          Top = 31
                          Width = 320
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = '\DefaultRecipeChangeLog\'
                        end
                        object cbN14_20_1: TCheckBox
                          Left = 30
                          Top = 67
                          Width = 147
                          Height = 30
                          Caption = 'Record Cycle Time : '
                          TabOrder = 3
                        end
                        object rgN14_20_1: TRadioGroup
                          Left = 181
                          Top = 56
                          Width = 140
                          Height = 41
                          Columns = 2
                          ItemIndex = 0
                          Items.Strings = (
                            '30 min'
                            '60 min')
                          TabOrder = 2
                        end
                      end
                      object Panel4: TPanel
                        Left = 0
                        Top = 121
                        Width = 545
                        Height = 152
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 1
                        object Label7: TLabel
                          Left = 12
                          Top = 91
                          Width = 93
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          BiDiMode = bdLeftToRight
                          Caption = 'HotPlateForm'
                          ParentBiDiMode = False
                        end
                        object Label8: TLabel
                          Left = 12
                          Top = 119
                          Width = 93
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          BiDiMode = bdLeftToRight
                          Caption = 'TrayForm'
                          ParentBiDiMode = False
                        end
                        object Label9: TLabel
                          Left = 12
                          Top = 63
                          Width = 93
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          BiDiMode = bdLeftToRight
                          Caption = 'BinCategory'
                          ParentBiDiMode = False
                        end
                        object lbN14_21: TLabel
                          Left = 12
                          Top = 35
                          Width = 93
                          Height = 20
                          Alignment = taRightJustify
                          AutoSize = False
                          BiDiMode = bdLeftToRight
                          Caption = 'SiteMap'
                          ParentBiDiMode = False
                        end
                        object cbN14_21: TCheckBox
                          Left = 6
                          Top = 3
                          Width = 431
                          Height = 29
                          Caption = '[N14-21] Set Up File Download'
                          TabOrder = 0
                        end
                        object edN14_21: TEdit
                          Left = 105
                          Top = 31
                          Width = 320
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = '/handler_data/SETUP_FILE'
                        end
                        object edN14_21_HP: TEdit
                          Left = 105
                          Top = 87
                          Width = 320
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Text = '/handler_data/SETUP_FILE'
                        end
                        object edN14_21_TF: TEdit
                          Left = 105
                          Top = 115
                          Width = 320
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 4
                          Text = '/handler_data/SETUP_FILE'
                        end
                        object edN14_21_BC: TEdit
                          Left = 105
                          Top = 59
                          Width = 320
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Text = '/handler_data/SETUP_FILE'
                        end
                      end
                      object grpN14_22: TGroupBox
                        Left = 0
                        Top = 273
                        Width = 545
                        Height = 119
                        Align = alTop
                        Caption = '[N14-22] Config Update From Server'
                        TabOrder = 2
                        object edtN14_22Exp: TEdit
                          Left = 8
                          Top = 55
                          Width = 297
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                        end
                        object edtN14_22Imp: TEdit
                          Left = 8
                          Top = 84
                          Width = 297
                          Height = 24
                          Hint = 'N11_ed1'
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                        end
                        object btnN14_22Import: TButton
                          Left = 312
                          Top = 84
                          Width = 89
                          Height = 25
                          Caption = 'Import'
                          TabOrder = 4
                          OnClick = btnN14_22ImportClick
                        end
                        object btnN14_22Export: TButton
                          Left = 312
                          Top = 56
                          Width = 89
                          Height = 25
                          Caption = 'Export'
                          TabOrder = 2
                          OnClick = btnN14_22ExportClick
                        end
                        object cbN14_22: TCheckBox
                          Left = 6
                          Top = 19
                          Width = 431
                          Height = 29
                          Caption = '[N14-22] Enable Config Update From Server'
                          TabOrder = 0
                        end
                      end
                    end
                    object tsN14_23: TTabSheet
                      Caption = '[N23]'
                      ImageIndex = 5
                      object Panel5: TPanel
                        Left = 0
                        Top = 0
                        Width = 545
                        Height = 41
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 0
                        object cb14_23: TCheckBox
                          Left = 6
                          Top = 5
                          Width = 431
                          Height = 29
                          Caption = '[N14-23] Read Text File for Password'
                          TabOrder = 0
                        end
                      end
                      object Panel6: TPanel
                        Left = 0
                        Top = 41
                        Width = 545
                        Height = 41
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 1
                        object lbRealContiNum: TLabel
                          Left = 496
                          Top = 16
                          Width = 42
                          Height = 16
                          Caption = '000000'
                        end
                        object Label60: TLabel
                          Left = 403
                          Top = 13
                          Width = 37
                          Height = 16
                          Caption = 'power'
                        end
                        object cb14_24: TCheckBox
                          Left = 6
                          Top = 5
                          Width = 395
                          Height = 29
                          Caption = '[N14-24] Dynamic multiplier for Continual Pass Bin( Socket )'
                          TabOrder = 0
                        end
                        object ed14_24: TEdit
                          Left = 380
                          Top = 9
                          Width = 21
                          Height = 24
                          Font.Charset = DEFAULT_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = '2'
                        end
                      end
                      object plAutoCalSuckZ: TPanel
                        Left = 0
                        Top = 82
                        Width = 545
                        Height = 41
                        Align = alTop
                        BevelInner = bvLowered
                        Color = 12761254
                        TabOrder = 2
                        object btnAutoCalSuckZ: TButton
                          Left = 24
                          Top = 8
                          Width = 257
                          Height = 25
                          Caption = 'Set Auto Calibrate Suck Z height'
                          TabOrder = 0
                        end
                      end
                    end
                  end
                end
              end
              object tsN15: TTabSheet
                Caption = '[N15]'
                ImageIndex = 9
                object grpN15: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Hint = 'N14_gb'
                  Align = alClient
                  Caption = '[N15] User Level By txt And ESD Control Machine '
                  TabOrder = 0
                  object lblN15UserLevelByTxtReadFilePath: TLabel
                    Left = 8
                    Top = 48
                    Width = 52
                    Height = 16
                    Hint = 'N14_lb'
                    Caption = 'File Path'
                  end
                  object sbN15UserLevelByTxtReadFilePath: TSpeedButton
                    Tag = 1
                    Left = 404
                    Top = 37
                    Width = 49
                    Height = 29
                    Hint = 'N14_sb'
                    Glyph.Data = {
                      DE030000424DDE03000000000000360000002800000011000000120000000100
                      180000000000A803000001000000010000000000000000000000FFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFBFFFFFFFFFFFFD4DEDED6D4D3D5CFD4DAD1CDD8D2D3DBD0D2D9D1D2D3CECF
                      D3D1D1D5D6D4D4D5D3E1D9D9DEEFFFFFFFFFFFFFFFFFFFFFFF60B8DC4CB0D228
                      A5CB20A9D717AAD216AAD41DA9D21DABD41EAAD321A8D41FAAD51DA6CD3BB0D7
                      77B8D7FFFFFFFFFFFFF2FFFFFF39B1D488E4FD5EDBF065D8F364DBF068D9F46D
                      D6F16ADAF069DBF26AD7F35EDAF25BD9EB89EBFB32ABD2FFFFFFFFFFFFF3FFFF
                      FF1AA5CC82E3FD32CDE636C6E939C8E72FC8ED37C7EA30C6E437C8E83DC4EA31
                      C7EB38CDE775E3FB11A9D2FFFFFFFFFFFFFCFFFFFF21A8DA89E0FF37CDEB40CF
                      F54ECCF540C8F84CCEF942CBF149CFF74DCBFB39CDF73BD1F077E2FD1BA9D8FF
                      FFFFFFFFFFFFFFFFFF22ABD28BEAFE46DEF642DBFC5AD6FE44D2FB55DAFF47D6
                      F250D8FA58D4FD44D8FC48DCF485E8FC28A2D6FFFFFFFFFFFFF6FFFFFF32A6DD
                      8AF0FC5FE7F964DFF966D9FF5CE8FF65E5FC63E2FB62E4FF5EE2FF57E2FD67E1
                      F997EDFF34ACCFFFFFFFFFFFFFFCFFFFFF22A8D899F6FF61F1F65EF2F864E7FB
                      71E8F771EEFC62EFFE5AE7FA67E7FC6DEEFD71EEF298FFFC1BB1D0FFFFFFFFFF
                      FFEFFFFFFF23A9D3B4F9FF6DF2FA6AF9FE89F2FF9BFDFF9DF9FAA0FEF9A7FFFC
                      AAFDFEA4FFFF9CF8FDB3F8FF19ABCFFFFFFFFFFFFFFDFFFFFF14AAC8BDFDFF82
                      FAFA7DFCF3A7FEFB13A6CE1CAED216AFD01AAED221A7D11DA8D31CA9D42EAAD3
                      31A5CEFFFFFFFFFFFFFAFFFFFF2AB2CCBCF3FFAFFDFFADFFFAC7FFF838B5C97D
                      F0FD85FAFF86F7F990FCFD83FBF788FCF5A4FFFA30A8CBFFFFFFFFFFFFFFFFFF
                      FF1DACC82CAECC19AED01AAED225A9CE5FCDE5A4FAFFB1FAFFB2F9FCB1FFFE9C
                      FCFCA5FFFFC4FEFF29B0C6FFFFFFFFFFFFF7FFFFFF6DC8E32CB0CE1BA7D017AA
                      D81AABD923ACCC26AFCF18A9C91FAED324A9D51DA8D322AED330A8CB7ACDE2FF
                      FFFFFFFFFFF9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                      FFFF}
                    OnClick = sbN15UserLevelByTxtReadFilePathClick
                  end
                  object lblN15UseESDControlMachineFilePath: TLabel
                    Left = 8
                    Top = 120
                    Width = 52
                    Height = 16
                    Hint = 'N14_lb'
                    Caption = 'File Path'
                  end
                  object btnN15ESDForm: TSpeedButton
                    Left = 328
                    Top = 71
                    Width = 121
                    Height = 33
                    Caption = ' ESD Form'
                    OnClick = btnN15ESDFormClick
                  end
                  object lblN15ESDControlUserName: TLabel
                    Left = 20
                    Top = 157
                    Width = 72
                    Height = 16
                    Hint = 'N11_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN15ESDControlPassword: TLabel
                    Left = 28
                    Top = 187
                    Width = 63
                    Height = 16
                    Hint = 'N11_lb2'
                    Caption = 'Password:'
                  end
                  object lblN15ESDControlHost: TLabel
                    Left = 60
                    Top = 217
                    Width = 31
                    Height = 16
                    Hint = 'N11_lb3'
                    Caption = 'Host:'
                  end
                  object lblN15ESDControlMachineSaveRecordFilePath: TLabel
                    Left = 20
                    Top = 248
                    Width = 197
                    Height = 16
                    Hint = 'N14_lb'
                    Caption = 'Save ESD Login Record To Path'
                  end
                  object lblAUTOMOTIVE: TLabel
                    Left = 13
                    Top = 313
                    Width = 144
                    Height = 16
                    Hint = 'N11_lb1'
                    Caption = 'AUTOMOTIVE File Path'
                  end
                  object edtN15_1: TEdit
                    Left = 64
                    Top = 45
                    Width = 329
                    Height = 24
                    Hint = 'N14_ed'
                    TabOrder = 1
                  end
                  object cbN15_1: TCheckBox
                    Left = 16
                    Top = 24
                    Width = 300
                    Height = 17
                    Hint = 'N14'
                    Caption = 'Enable User Level By Txt Function'
                    TabOrder = 0
                  end
                  object cbN15_2: TCheckBox
                    Left = 16
                    Top = 80
                    Width = 300
                    Height = 17
                    Hint = 'N14'
                    Caption = 'Enable ESD Control Machine Function'
                    TabOrder = 2
                  end
                  object edtN15_2: TEdit
                    Left = 63
                    Top = 117
                    Width = 386
                    Height = 24
                    Hint = 'N14_ed'
                    TabOrder = 3
                  end
                  object edN15_Host: TEdit
                    Left = 100
                    Top = 212
                    Width = 250
                    Height = 24
                    Hint = 'N11_ed3'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 6
                  end
                  object edN15_Password: TEdit
                    Left = 100
                    Top = 182
                    Width = 250
                    Height = 24
                    Hint = 'N11_ed2'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    PasswordChar = '*'
                    TabOrder = 5
                  end
                  object edN15_UserName: TEdit
                    Left = 100
                    Top = 152
                    Width = 250
                    Height = 24
                    Hint = 'N11_ed1'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 4
                  end
                  object cbESDFTPList: TComboBox
                    Left = 273
                    Top = 408
                    Width = 279
                    Height = 28
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -17
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ItemHeight = 0
                    ParentFont = False
                    TabOrder = 11
                    Visible = False
                  end
                  object edN15_3: TEdit
                    Left = 15
                    Top = 269
                    Width = 434
                    Height = 24
                    Hint = 'N14_ed'
                    TabOrder = 7
                  end
                  object ComboBox5: TComboBox
                    Left = 75
                    Top = 368
                    Width = 279
                    Height = 28
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -17
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ItemHeight = 0
                    ParentFont = False
                    TabOrder = 9
                    Visible = False
                  end
                  object edtN15_4: TEdit
                    Left = 166
                    Top = 308
                    Width = 250
                    Height = 24
                    Hint = 'N11_ed1'
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 8
                  end
                  object cbOEEAUTOMOACTIVEDownloadList: TComboBox
                    Left = 75
                    Top = 404
                    Width = 145
                    Height = 24
                    ItemHeight = 0
                    TabOrder = 10
                    Visible = False
                  end
                end
              end
              object tsN16: TTabSheet
                Caption = '[N16]'
                ImageIndex = 11
                object grpN16: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N16] Offset FTP'
                  TabOrder = 0
                  object lblN16_Host: TLabel
                    Left = 16
                    Top = 114
                    Width = 31
                    Height = 16
                    Caption = 'Host:'
                  end
                  object lblN16_DownloadPath: TLabel
                    Left = 16
                    Top = 143
                    Width = 94
                    Height = 16
                    Caption = 'Download Path:'
                  end
                  object lblN16_UploadPath: TLabel
                    Left = 16
                    Top = 172
                    Width = 78
                    Height = 16
                    Caption = 'Upload Path:'
                  end
                  object lblN16_UserName: TLabel
                    Left = 16
                    Top = 56
                    Width = 72
                    Height = 16
                    Caption = 'User Name:'
                  end
                  object lblN16_Password: TLabel
                    Left = 16
                    Top = 85
                    Width = 63
                    Height = 16
                    Caption = 'Password:'
                  end
                  object edN16_HostName: TEdit
                    Left = 116
                    Top = 112
                    Width = 250
                    Height = 24
                    TabOrder = 3
                  end
                  object edN16_DownPath: TEdit
                    Left = 116
                    Top = 140
                    Width = 250
                    Height = 24
                    TabOrder = 4
                  end
                  object edN16_UpLdPath: TEdit
                    Left = 116
                    Top = 168
                    Width = 250
                    Height = 24
                    TabOrder = 5
                  end
                  object edN16_UserName: TEdit
                    Left = 116
                    Top = 56
                    Width = 250
                    Height = 24
                    TabOrder = 1
                  end
                  object edN16_Password: TEdit
                    Left = 116
                    Top = 84
                    Width = 250
                    Height = 24
                    PasswordChar = '*'
                    TabOrder = 2
                  end
                  object chkN16: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 277
                    Height = 17
                    Caption = 'Enable Offset FTP'
                    TabOrder = 0
                  end
                end
              end
              object tsN17: TTabSheet
                Caption = '[N17]'
                ImageIndex = 23
                object gbN17LotSummary: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 89
                  Align = alTop
                  Caption = '[N17] Upload Lot summary'
                  TabOrder = 0
                  object labN17_2: TLabel
                    Left = 38
                    Top = 50
                    Width = 137
                    Height = 16
                    Caption = '[N17-2] Net derive path'
                  end
                  object cbN17_1: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 237
                    Height = 17
                    Caption = '[N17-1] Upload lot summary'
                    TabOrder = 0
                  end
                  object edN17_2: TEdit
                    Left = 190
                    Top = 48
                    Width = 200
                    Height = 24
                    TabOrder = 1
                    Text = 'D:\RMS'
                  end
                end
                object gbN17ProdLogUL: TGroupBox
                  Left = 0
                  Top = 89
                  Width = 557
                  Height = 88
                  Align = alTop
                  Caption = '[N17] Upload Production log'
                  TabOrder = 1
                  object labN17_4: TLabel
                    Left = 38
                    Top = 50
                    Width = 137
                    Height = 16
                    Caption = '[N17-4] Net derive path'
                  end
                  object cbN17_3: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 237
                    Height = 17
                    Caption = '[N17-3] Upload production log daily '
                    TabOrder = 0
                  end
                  object edN17_4: TEdit
                    Left = 190
                    Top = 48
                    Width = 200
                    Height = 24
                    TabOrder = 1
                    Text = 'D:\RMS'
                  end
                end
              end
              object tsN21: TTabSheet
                Caption = '[N21]'
                ImageIndex = 24
                object grpN21: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 174
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N21] Handler state change upload server'
                  TabOrder = 0
                  object lblN21_3: TLabel
                    Left = 16
                    Top = 109
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object lblN21_1: TLabel
                    Left = 16
                    Top = 52
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN21_2: TLabel
                    Left = 16
                    Top = 81
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object lblN21_4: TLabel
                    Left = 16
                    Top = 137
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object edN21_3: TEdit
                    Left = 116
                    Top = 104
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 3
                    Text = '127.0.0.1'
                  end
                  object edN21_1: TEdit
                    Left = 116
                    Top = 48
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 1
                    Text = 'UnKnown'
                  end
                  object edN21_2: TEdit
                    Left = 116
                    Top = 76
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 2
                    Text = '1234'
                  end
                  object chkN21_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 133
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Function'
                    TabOrder = 0
                  end
                  object edN21_4: TEdit
                    Left = 116
                    Top = 132
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 4
                    Text = '\\Handler\\InstallForAll\\'
                  end
                end
              end
              object tsN22: TTabSheet
                Caption = '[N22]'
                ImageIndex = 10
                object grpN22: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N22] ASE-CL FTP Function'
                  TabOrder = 0
                  object cbN22_EveltLog: TCheckBox
                    Left = 12
                    Top = 172
                    Width = 201
                    Height = 17
                    Caption = '[N22-2] Enable Event Log'
                    TabOrder = 2
                  end
                  object cbN22: TCheckBox
                    Left = 12
                    Top = 20
                    Width = 257
                    Height = 17
                    Caption = '[N22-1] Enable FTP Function'
                    TabOrder = 0
                  end
                  object pnlN22: TPanel
                    Left = 32
                    Top = 40
                    Width = 417
                    Height = 129
                    BevelInner = bvLowered
                    Color = 12761254
                    TabOrder = 1
                    object lb_N22_UserName: TLabel
                      Left = 12
                      Top = 12
                      Width = 72
                      Height = 16
                      Caption = 'User Name:'
                    end
                    object lb_N22_Password: TLabel
                      Left = 12
                      Top = 40
                      Width = 63
                      Height = 16
                      Caption = 'Password:'
                    end
                    object lb_N22_Host: TLabel
                      Left = 12
                      Top = 69
                      Width = 31
                      Height = 16
                      Hint = 'N11_lb3'
                      Caption = 'Host:'
                    end
                    object lb_N22_DownloadPath: TLabel
                      Left = 12
                      Top = 97
                      Width = 94
                      Height = 16
                      Caption = 'Download Path:'
                    end
                    object ed_N22_DownloadPath: TEdit
                      Left = 152
                      Top = 93
                      Width = 250
                      Height = 24
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 3
                    end
                    object ed_N22_HostAddress: TEdit
                      Left = 152
                      Top = 65
                      Width = 250
                      Height = 24
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 2
                    end
                    object ed_N22_Password: TEdit
                      Left = 152
                      Top = 36
                      Width = 250
                      Height = 24
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentFont = False
                      PasswordChar = '*'
                      TabOrder = 1
                    end
                    object ed_N22_UserName: TEdit
                      Left = 152
                      Top = 8
                      Width = 250
                      Height = 24
                      Font.Charset = ANSI_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = []
                      ParentFont = False
                      TabOrder = 0
                    end
                  end
                end
              end
              object tsN23: TTabSheet
                Caption = '[N23]'
                ImageIndex = 13
                object pgcN23: TPageControl
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  ActivePage = tsN23_0
                  Align = alClient
                  TabIndex = 0
                  TabOrder = 0
                  object tsN23_0: TTabSheet
                    Caption = '[N23]'
                    ImageIndex = 3
                    object rgbN23_2DSorting: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 466
                      Align = alClient
                      Caption = '[N23] 2DID Sorting Mode'
                      TabOrder = 0
                      object lab_N23_3NetDrivePath: TLabel
                        Left = 13
                        Top = 243
                        Width = 137
                        Height = 16
                        Caption = '[N23-3] Net derive path'
                      end
                      object gbN23_FTPSetting: TGroupBox
                        Left = 2
                        Top = 85
                        Width = 545
                        Height = 147
                        Align = alTop
                        Caption = '[N23-2] FTP Setting'
                        TabOrder = 1
                        object lab_N23_UserName: TLabel
                          Left = 12
                          Top = 21
                          Width = 72
                          Height = 16
                          Caption = 'User Name:'
                        end
                        object lab_N23Pwd: TLabel
                          Left = 12
                          Top = 53
                          Width = 63
                          Height = 16
                          Caption = 'Password:'
                        end
                        object lab_N23FTPHost: TLabel
                          Left = 12
                          Top = 85
                          Width = 31
                          Height = 16
                          Caption = 'Host:'
                        end
                        object lab_N23FTPPath: TLabel
                          Left = 12
                          Top = 117
                          Width = 94
                          Height = 16
                          Caption = 'Download Path:'
                        end
                        object edN23_2FTPHost: TEdit
                          Left = 113
                          Top = 81
                          Width = 284
                          Height = 24
                          TabOrder = 2
                        end
                        object edN23_2Pwd: TEdit
                          Left = 113
                          Top = 49
                          Width = 284
                          Height = 24
                          PasswordChar = '*'
                          TabOrder = 1
                        end
                        object edN23_2UserName: TEdit
                          Left = 113
                          Top = 17
                          Width = 284
                          Height = 24
                          TabOrder = 0
                        end
                        object edN23_2FTPPath: TEdit
                          Left = 113
                          Top = 113
                          Width = 284
                          Height = 24
                          TabOrder = 3
                        end
                      end
                      object rgN23_1_2DSorting: TRadioGroup
                        Left = 2
                        Top = 18
                        Width = 545
                        Height = 67
                        Align = alTop
                        Caption = '[N23-1] Upload Method'
                        Columns = 2
                        ItemIndex = 1
                        Items.Strings = (
                          'FTP'
                          'Net Drive'
                          'SECS/GEM'
                          'Manual Select')
                        TabOrder = 0
                      end
                      object edN23_3NetDrivePath: TEdit
                        Left = 165
                        Top = 241
                        Width = 200
                        Height = 24
                        TabOrder = 2
                        Text = 'D:\RMS'
                      end
                      object chkN23_4: TCheckBox
                        Left = 12
                        Top = 281
                        Width = 245
                        Height = 17
                        Caption = '[N23-4] Get lot info from file'
                        TabOrder = 4
                      end
                      object edtN23_4: TEdit
                        Left = 261
                        Top = 277
                        Width = 200
                        Height = 24
                        TabOrder = 3
                        Text = 'D:\RMS'
                      end
                      object chkN23_5: TCheckBox
                        Left = 12
                        Top = 309
                        Width = 321
                        Height = 17
                        Caption = '[N23-5] Summary folder without YY\MM'
                        TabOrder = 5
                      end
                    end
                  end
                  object tsN23_1: TTabSheet
                    Caption = '[N23-1]'
                    object grpN23_1: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N23-1] 2DID comparasion function'
                      TabOrder = 0
                      object lbledtN23_1_URL: TLabeledEdit
                        Left = 44
                        Top = 44
                        Width = 401
                        Height = 24
                        EditLabel.Width = 33
                        EditLabel.Height = 16
                        EditLabel.Caption = 'URL :'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 1
                        Text = 
                          'http://172.16.83.190/sm5200/page/TrafficExecute.jsp?pptid=519135' +
                          '&argument=519135.args'
                      end
                      object chkN23_1: TCheckBox
                        Left = 12
                        Top = 20
                        Width = 257
                        Height = 17
                        Caption = '[N23-1] Enable Function'
                        TabOrder = 0
                      end
                    end
                  end
                  object tsN23_2: TTabSheet
                    Caption = '[N23-2]'
                    ImageIndex = 1
                    object grpN23_2: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 121
                      Align = alTop
                      Caption = '[N23-2] Production setting'
                      TabOrder = 0
                      object edtN23_2_ProcessName: TLabeledEdit
                        Left = 104
                        Top = 56
                        Width = 265
                        Height = 24
                        Color = clWhite
                        EditLabel.Width = 90
                        EditLabel.Height = 16
                        EditLabel.Caption = 'Process Name'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 1
                      end
                      object edtN23_2_LineID: TLabeledEdit
                        Left = 104
                        Top = 24
                        Width = 265
                        Height = 24
                        Color = clWhite
                        EditLabel.Width = 25
                        EditLabel.Height = 16
                        EditLabel.Caption = 'Line'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 0
                      end
                      object edtN23_2_Product: TLabeledEdit
                        Left = 104
                        Top = 88
                        Width = 265
                        Height = 24
                        Color = clWhite
                        EditLabel.Width = 86
                        EditLabel.Height = 16
                        EditLabel.Caption = 'Product Name'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 2
                      end
                    end
                  end
                  object tsN23_3: TTabSheet
                    Caption = '[N23-3]'
                    ImageIndex = 2
                    object grpN23_3: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N23-3] Upload test result'
                      TabOrder = 0
                      object lbledtN23_2_URL: TLabeledEdit
                        Left = 68
                        Top = 44
                        Width = 373
                        Height = 24
                        EditLabel.Width = 33
                        EditLabel.Height = 16
                        EditLabel.Caption = 'URL :'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 1
                        Text = 
                          'http://172.16.83.190/sm5200/page/TrafficExecute.jsp?pptid=519129' +
                          '&argument=519129.args'
                      end
                      object chkN23_3: TCheckBox
                        Left = 12
                        Top = 20
                        Width = 257
                        Height = 17
                        Caption = '[N23-3] Enable Function'
                        TabOrder = 0
                      end
                    end
                    object grpN23_4: TGroupBox
                      Left = 0
                      Top = 80
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N23-4] Download 2DID list path'
                      TabOrder = 1
                      object lbledtN23_4_URL: TLabeledEdit
                        Left = 60
                        Top = 44
                        Width = 393
                        Height = 24
                        EditLabel.Width = 33
                        EditLabel.Height = 16
                        EditLabel.Caption = 'URL :'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 0
                        Text = 'D:\RMS'
                      end
                    end
                    object grpN23_5: TGroupBox
                      Left = 0
                      Top = 160
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N23-5] Upload 2DID white list'
                      TabOrder = 2
                      object lbledtN23_5_UploadPath: TLabeledEdit
                        Left = 60
                        Top = 44
                        Width = 393
                        Height = 24
                        EditLabel.Width = 33
                        EditLabel.Height = 16
                        EditLabel.Caption = 'URL :'
                        LabelPosition = lpLeft
                        LabelSpacing = 3
                        TabOrder = 0
                      end
                    end
                  end
                end
              end
              object tsN24: TTabSheet
                Caption = '[N24]'
                ImageIndex = 14
                object grpN24: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 461
                  Align = alClient
                  Caption = '[N24] RTM control function'
                  TabOrder = 0
                  object chk24: TCheckBox
                    Left = 12
                    Top = 24
                    Width = 257
                    Height = 17
                    Caption = '[N24] Enable function'
                    TabOrder = 0
                  end
                  object lbledtN24: TLabeledEdit
                    Left = 84
                    Top = 44
                    Width = 81
                    Height = 24
                    EditLabel.Width = 33
                    EditLabel.Height = 16
                    EditLabel.Caption = 'Port : '
                    LabelPosition = lpLeft
                    LabelSpacing = 3
                    TabOrder = 1
                    Text = '6000'
                    OnClick = edN06_PortClick
                  end
                end
              end
              object tsN25: TTabSheet
                Caption = '[N25]'
                ImageIndex = 15
                object pcN25: TPageControl
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  ActivePage = tsN25_4
                  Align = alClient
                  TabIndex = 1
                  TabOrder = 0
                  object tsN25_1_3: TTabSheet
                    Caption = '[N25-1]-[N25-3]'
                    object grpN25: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 430
                      Align = alClient
                      Caption = '[N25] Handler automation'
                      TabOrder = 0
                      object grpN25_1: TGroupBox
                        Left = 2
                        Top = 18
                        Width = 545
                        Height = 147
                        Align = alTop
                        Caption = '[N25-1] Auto Start FTP'
                        TabOrder = 0
                        object lblN25_1_Path: TLabel
                          Left = 32
                          Top = 121
                          Width = 55
                          Height = 16
                          Caption = 'File Path:'
                        end
                        object lblN25_1_Host: TLabel
                          Left = 56
                          Top = 97
                          Width = 31
                          Height = 16
                          Caption = 'Host:'
                        end
                        object lblN25_1_Password: TLabel
                          Left = 24
                          Top = 69
                          Width = 63
                          Height = 16
                          Caption = 'Password:'
                        end
                        object lblN25_1_Name: TLabel
                          Left = 16
                          Top = 45
                          Width = 69
                          Height = 16
                          Caption = 'User name:'
                        end
                        object edtN25_1_Host: TEdit
                          Left = 100
                          Top = 90
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Text = '10.20.50.3'
                        end
                        object edtN25_1_Path: TEdit
                          Left = 100
                          Top = 115
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 4
                          Text = '/summary/Handler/TesterStatus/'
                        end
                        object edtN25_1_Password: TEdit
                          Left = 100
                          Top = 65
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          PasswordChar = '*'
                          TabOrder = 2
                          Text = 'handler'
                        end
                        object edtN25_1_Name: TEdit
                          Left = 100
                          Top = 40
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = 'handler'
                        end
                        object chkN25_1: TCheckBox
                          Left = 12
                          Top = 20
                          Width = 277
                          Height = 17
                          Caption = 'Enable auto start function'
                          TabOrder = 0
                        end
                      end
                      object GroupBox1: TGroupBox
                        Left = 2
                        Top = 165
                        Width = 545
                        Height = 172
                        Align = alTop
                        Caption = '[N25-2] Temperature log FTP'
                        TabOrder = 1
                        object lblN25_2_Path: TLabel
                          Left = 32
                          Top = 121
                          Width = 55
                          Height = 16
                          Caption = 'File Path:'
                        end
                        object lblN25_2_Host: TLabel
                          Left = 56
                          Top = 97
                          Width = 31
                          Height = 16
                          Caption = 'Host:'
                        end
                        object lblN25_2_Password: TLabel
                          Left = 24
                          Top = 69
                          Width = 63
                          Height = 16
                          Caption = 'Password:'
                        end
                        object lblN25_2_Name: TLabel
                          Left = 16
                          Top = 45
                          Width = 69
                          Height = 16
                          Caption = 'User name:'
                        end
                        object lblN25_2_Interval: TLabel
                          Left = 16
                          Top = 144
                          Width = 123
                          Height = 16
                          Caption = 'Upload Interval (min)'
                        end
                        object edtN25_2_Host: TEdit
                          Left = 100
                          Top = 90
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 3
                          Text = '10.20.50.3'
                        end
                        object edtN25_2_Path: TEdit
                          Left = 100
                          Top = 115
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 4
                          Text = '/Summary/naslfs2/Handler/'
                        end
                        object edtN25_2_Password: TEdit
                          Left = 100
                          Top = 65
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          PasswordChar = '*'
                          TabOrder = 2
                          Text = 'handler'
                        end
                        object edtN25_2_Name: TEdit
                          Left = 100
                          Top = 40
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 1
                          Text = 'handler'
                        end
                        object chkN25_2: TCheckBox
                          Left = 12
                          Top = 20
                          Width = 277
                          Height = 17
                          Caption = 'Enable temperature log function'
                          TabOrder = 0
                        end
                        object edtN25_2_Interval: TEdit
                          Left = 156
                          Top = 141
                          Width = 73
                          Height = 24
                          TabOrder = 5
                        end
                      end
                      object grpN25_3: TGroupBox
                        Left = 2
                        Top = 337
                        Width = 545
                        Height = 80
                        Align = alTop
                        Caption = '[N25-3] Jam log FTP'
                        TabOrder = 2
                        object lblN25_3: TLabel
                          Left = 32
                          Top = 49
                          Width = 55
                          Height = 16
                          Caption = 'File Path:'
                        end
                        object edtN25_3_LogJamPath: TEdit
                          Left = 100
                          Top = 43
                          Width = 250
                          Height = 24
                          Font.Charset = ANSI_CHARSET
                          Font.Color = clWindowText
                          Font.Height = -13
                          Font.Name = 'MS Sans Serif'
                          Font.Style = []
                          ParentFont = False
                          TabOrder = 2
                          Text = '/Summary/naslfs2/Handler/'
                        end
                        object chkN25_3: TCheckBox
                          Left = 12
                          Top = 20
                          Width = 277
                          Height = 17
                          Caption = 'Enable'
                          TabOrder = 0
                        end
                        object btnN25_3_Manual: TButton
                          Left = 376
                          Top = 40
                          Width = 75
                          Height = 25
                          Caption = 'Manual'
                          TabOrder = 1
                          OnClick = btnN25_3_ManualClick
                        end
                      end
                    end
                  end
                  object tsN25_4: TTabSheet
                    Caption = '[N25-4]'
                    ImageIndex = 1
                    object grpN25_5: TGroupBox
                      Left = 0
                      Top = 80
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N25-5] Upload EventLog'
                      TabOrder = 1
                      object Label35: TLabel
                        Left = 32
                        Top = 49
                        Width = 55
                        Height = 16
                        Caption = 'File Path:'
                      end
                      object edtN25_5_UploadPath: TEdit
                        Left = 100
                        Top = 43
                        Width = 250
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 2
                        Text = '/Summary/naslfs2/Handler/'
                      end
                      object chkN25_5: TCheckBox
                        Left = 12
                        Top = 20
                        Width = 277
                        Height = 17
                        Caption = 'Enable'
                        TabOrder = 0
                      end
                      object btnN25_5_Manual: TButton
                        Left = 376
                        Top = 40
                        Width = 75
                        Height = 25
                        Caption = 'Manual'
                        TabOrder = 1
                        OnClick = btnN25_5_ManualClick
                      end
                    end
                    object grpN25_4: TGroupBox
                      Left = 0
                      Top = 0
                      Width = 549
                      Height = 80
                      Align = alTop
                      Caption = '[N25-4] Summary Count FTP'
                      TabOrder = 0
                      object Label36: TLabel
                        Left = 32
                        Top = 49
                        Width = 55
                        Height = 16
                        Caption = 'File Path:'
                      end
                      object edtN25_4_UploadPath: TEdit
                        Left = 100
                        Top = 43
                        Width = 250
                        Height = 24
                        Font.Charset = ANSI_CHARSET
                        Font.Color = clWindowText
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = []
                        ParentFont = False
                        TabOrder = 2
                        Text = '/Summary/naslfs2/Handler/'
                      end
                      object chkN25_4: TCheckBox
                        Left = 12
                        Top = 20
                        Width = 277
                        Height = 17
                        Caption = 'Enable'
                        TabOrder = 0
                      end
                      object btnN25_4_Manual: TButton
                        Left = 376
                        Top = 40
                        Width = 75
                        Height = 25
                        Caption = 'Manual'
                        TabOrder = 1
                        OnClick = btnN25_4_ManualClick
                      end
                    end
                  end
                end
              end
              object tsN26: TTabSheet
                Caption = '[N26]'
                ImageIndex = 18
                object gbN26: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 249
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N26] FTP Setting JAM RawData Updata'
                  TabOrder = 0
                  object lblN26_4: TLabel
                    Left = 16
                    Top = 171
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object lblN26_5: TLabel
                    Left = 16
                    Top = 205
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object Label3: TLabel
                    Left = 16
                    Top = 107
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN26_3: TLabel
                    Left = 16
                    Top = 139
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object Label4: TLabel
                    Left = 16
                    Top = 80
                    Width = 339
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'To Directory : D:\HT9045_Log\EventLogTxt\SGJamCount'
                  end
                  object edtN26_3: TEdit
                    Left = 182
                    Top = 169
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 4
                    Text = '127.0.0.1'
                  end
                  object edtN26_4: TEdit
                    Left = 182
                    Top = 201
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 5
                    Text = '\\Handler\\temperature\\'
                  end
                  object edtN26_1: TEdit
                    Left = 182
                    Top = 107
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 2
                    Text = 'UnKnown'
                  end
                  object edtN26_2: TEdit
                    Left = 182
                    Top = 138
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 3
                    Text = '1234'
                  end
                  object chkN26_1: TCheckBox
                    Left = 13
                    Top = 54
                    Width = 236
                    Height = 22
                    Hint = 'B05-2'
                    Caption = 'Enable Upload To FTP : '
                    TabOrder = 1
                  end
                  object chkN26_2: TCheckBox
                    Left = 13
                    Top = 30
                    Width = 236
                    Height = 22
                    Hint = 'B05-2'
                    Caption = 'Enable JamRawData Record'
                    TabOrder = 0
                  end
                end
              end
              object tsN27: TTabSheet
                Caption = '[N27]'
                ImageIndex = 17
                object grpN27: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 237
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N27]  Alarm Log Upload FTP'
                  TabOrder = 0
                  object lblN27_3: TLabel
                    Left = 16
                    Top = 106
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object Label288: TLabel
                    Left = 16
                    Top = 136
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object lblN27_1: TLabel
                    Left = 16
                    Top = 48
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN27_2: TLabel
                    Left = 16
                    Top = 77
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object lblN27_6: TLabel
                    Left = 16
                    Top = 166
                    Width = 55
                    Height = 16
                    Hint = 'N16_lb5'
                    Caption = 'Tester ID'
                  end
                  object edN27_3: TEdit
                    Left = 108
                    Top = 104
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 3
                    Text = '127.0.0.1'
                  end
                  object edN27_4: TEdit
                    Left = 108
                    Top = 132
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 4
                    Text = '\\Handler\\EventLogXml\\'
                  end
                  object edN27_1: TEdit
                    Left = 108
                    Top = 48
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 1
                    Text = 'UnKnown'
                  end
                  object edN27_2: TEdit
                    Left = 108
                    Top = 76
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 2
                    Text = '1234'
                  end
                  object cbN27_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 221
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Alarm Log Upload FTP'
                    TabOrder = 0
                  end
                  object edN27_6: TEdit
                    Left = 108
                    Top = 160
                    Width = 209
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 5
                    Text = 'TesterID'
                  end
                end
              end
              object tsN28: TTabSheet
                Caption = '[N28]'
                ImageIndex = 16
                object grpN28: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 497
                  Align = alClient
                  Caption = '[N28] OEE Function'
                  TabOrder = 0
                  object lblN28_1: TLabel
                    Left = 20
                    Top = 61
                    Width = 33
                    Height = 16
                    Caption = 'Path :'
                  end
                  object lblN28_2: TLabel
                    Left = 20
                    Top = 85
                    Width = 18
                    Height = 16
                    Caption = 'IP :'
                  end
                  object chkN28: TCheckBox
                    Left = 20
                    Top = 28
                    Width = 381
                    Height = 17
                    Caption = 'Enable OEE Function'
                    TabOrder = 0
                  end
                  object edtN28_Path: TEdit
                    Left = 68
                    Top = 56
                    Width = 250
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                  end
                  object edtN28_IP: TEdit
                    Left = 68
                    Top = 81
                    Width = 250
                    Height = 24
                    Font.Charset = ANSI_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 2
                  end
                end
              end
              object tsN29: TTabSheet
                Caption = '[N29]'
                ImageIndex = 19
                object lblN29: TLabel
                  Left = 88
                  Top = 40
                  Width = 52
                  Height = 16
                  Caption = 'File Path'
                end
                object edtN29: TEdit
                  Left = 153
                  Top = 36
                  Width = 284
                  Height = 24
                  Enabled = False
                  TabOrder = 1
                end
                object chkN29: TCheckBox
                  Left = 4
                  Top = 8
                  Width = 433
                  Height = 17
                  Caption = '[N29] Enable important  parameter check function'
                  TabOrder = 0
                end
              end
              object tsN30: TTabSheet
                Caption = '[N30]'
                ImageIndex = 20
                object gbN30: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 198
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N30] Ground and ESD Log Upload FTP'
                  TabOrder = 0
                  object lblN30_3: TLabel
                    Left = 16
                    Top = 109
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object lblN30_4: TLabel
                    Left = 16
                    Top = 137
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object lblN30_1: TLabel
                    Left = 16
                    Top = 52
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN30_2: TLabel
                    Left = 16
                    Top = 81
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object lblN30_5: TLabel
                    Left = 16
                    Top = 166
                    Width = 85
                    Height = 16
                    Hint = 'N16_lb5'
                    Caption = 'Upload Path2:'
                  end
                  object edN30_3: TEdit
                    Left = 108
                    Top = 104
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 3
                    Text = '127.0.0.1'
                  end
                  object edN30_4: TEdit
                    Left = 108
                    Top = 132
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 4
                    Text = '\\Handler\\GroundESDLog\\'
                  end
                  object edN30_1: TEdit
                    Left = 108
                    Top = 48
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 1
                    Text = 'UnKnown'
                  end
                  object edN30_2: TEdit
                    Left = 108
                    Top = 76
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 2
                    Text = '1234'
                  end
                  object cbN30_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 221
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Upload FTP'
                    TabOrder = 0
                  end
                  object edN30_5: TEdit
                    Left = 108
                    Top = 161
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed5'
                    TabOrder = 5
                    Text = '\\Handler\\GroundESDLog2\\'
                  end
                end
              end
              object tsN31: TTabSheet
                Caption = '[N31]'
                ImageIndex = 21
                object gbN31: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 293
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N31] Auto temperature offset'
                  TabOrder = 0
                  object labN31_3: TLabel
                    Left = 16
                    Top = 137
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object labN31_4: TLabel
                    Left = 16
                    Top = 165
                    Width = 94
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Download Path:'
                  end
                  object labN31_1: TLabel
                    Left = 16
                    Top = 80
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object labN31_2: TLabel
                    Left = 16
                    Top = 109
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object labN31_5: TLabel
                    Left = 16
                    Top = 194
                    Width = 82
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Contact Count'
                  end
                  object lblN31_7: TLabel
                    Left = 192
                    Top = 226
                    Width = 54
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Max Offs.'
                  end
                  object lblN31_6: TLabel
                    Left = 16
                    Top = 226
                    Width = 50
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Min Offs.'
                  end
                  object edN31_3: TEdit
                    Left = 116
                    Top = 132
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 3
                    Text = '127.0.0.1'
                  end
                  object edN31_4: TEdit
                    Left = 116
                    Top = 160
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 4
                    Text = '\\Handler\\AutoTempOfs\\'
                  end
                  object edN31_1: TEdit
                    Left = 116
                    Top = 76
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 1
                    Text = 'UnKnown'
                  end
                  object edN31_2: TEdit
                    Left = 116
                    Top = 104
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 2
                    Text = '1234'
                  end
                  object edN31_5: TEdit
                    Left = 116
                    Top = 189
                    Width = 45
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 5
                    Text = '10'
                  end
                  object btnN31_Manual: TButton
                    Left = 192
                    Top = 190
                    Width = 121
                    Height = 25
                    Caption = 'Manual'
                    TabOrder = 6
                    OnClick = btnN31_ManualClick
                  end
                  object cbN31_1: TRadioGroup
                    Left = 16
                    Top = 24
                    Width = 509
                    Height = 49
                    Caption = '[N31]'
                    Columns = 3
                    ItemIndex = 0
                    Items.Strings = (
                      'Disable'
                      'by FTP'
                      'by Net Drive')
                    TabOrder = 0
                  end
                  object edN31_MaxOffset: TEdit
                    Left = 252
                    Top = 221
                    Width = 45
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 8
                    Text = '10'
                  end
                  object edN31_MinOffset: TEdit
                    Left = 116
                    Top = 221
                    Width = 45
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 7
                    Text = '10'
                  end
                end
              end
              object tsN32: TTabSheet
                Caption = '[N32]'
                ImageIndex = 22
                object gbN32: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 433
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N32] Download updates HT9045 automatically'
                  TabOrder = 0
                  object labN32_3: TLabel
                    Left = 12
                    Top = 181
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object labN32_4: TLabel
                    Left = 12
                    Top = 241
                    Width = 123
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'FTP Download Path:'
                  end
                  object labN32_1: TLabel
                    Left = 12
                    Top = 124
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object labN32_2: TLabel
                    Left = 12
                    Top = 153
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object labN32_5: TLabel
                    Left = 12
                    Top = 303
                    Width = 153
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Net Drive Download Path:'
                  end
                  object lblN32_6: TLabel
                    Left = 155
                    Top = 204
                    Width = 314
                    Height = 16
                    Caption = '1st check path. Delete after file download is complete'
                  end
                  object lblN32_7: TLabel
                    Left = 155
                    Top = 244
                    Width = 97
                    Height = 16
                    Caption = '2nd check patch'
                  end
                  object edN32_3: TEdit
                    Left = 169
                    Top = 176
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 4
                    Text = '127.0.0.1'
                  end
                  object edN32_4: TEdit
                    Left = 169
                    Top = 220
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 5
                    Text = '\\Handler\\Install\\'
                  end
                  object edN32_1: TEdit
                    Left = 169
                    Top = 120
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 2
                    Text = 'UnKnown'
                  end
                  object edN32_2: TEdit
                    Left = 169
                    Top = 148
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 3
                    Text = '1234'
                  end
                  object cbN32_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 293
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Function'
                    TabOrder = 0
                  end
                  object btnN32: TButton
                    Left = 28
                    Top = 402
                    Width = 121
                    Height = 25
                    Caption = 'Check Manual'
                    TabOrder = 11
                    OnClick = btnN32Click
                  end
                  object cbN32_2: TCheckBox
                    Left = 8
                    Top = 332
                    Width = 213
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Check for updates once a day'
                    TabOrder = 8
                  end
                  object cbN32_3: TCheckBox
                    Left = 8
                    Top = 356
                    Width = 133
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Check at initail start '
                    TabOrder = 9
                  end
                  object cbN32_4: TCheckBox
                    Left = 8
                    Top = 380
                    Width = 173
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Check at tray feed finish '
                    TabOrder = 10
                  end
                  object rgN32: TRadioGroup
                    Left = 12
                    Top = 54
                    Width = 449
                    Height = 49
                    Caption = '[N32] Upload Method'
                    Columns = 2
                    ItemIndex = 0
                    Items.Strings = (
                      'FTP'
                      'Net Drive')
                    TabOrder = 1
                  end
                  object edN32_5: TEdit
                    Left = 169
                    Top = 298
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 7
                    Text = 'D:\\RMS\\'
                  end
                  object edN32_6: TEdit
                    Left = 169
                    Top = 260
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 6
                    Text = '\\Handler\\InstallForAll\\'
                  end
                end
              end
              object tsN33: TTabSheet
                Caption = '[N33]'
                ImageIndex = 25
                object gbN33: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 94
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N33] Upload OCR and Bin log '
                  TabOrder = 0
                  object labN33: TLabel
                    Left = 16
                    Top = 57
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object edN33: TEdit
                    Left = 116
                    Top = 52
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 1
                  end
                  object cbN33: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 125
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Upload'
                    TabOrder = 0
                  end
                end
                object gbN33_1: TGroupBox
                  Left = 0
                  Top = 94
                  Width = 557
                  Height = 56
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N33-1] Net Driver change file and data'
                  TabOrder = 1
                  object cbN33_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 373
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable to change file and data by Net Driver'
                    TabOrder = 0
                  end
                end
              end
              object tsN34: TTabSheet
                Caption = '[N34]'
                ImageIndex = 26
                object gbN34: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 83
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N34] OEE And Failure Report'
                  TabOrder = 0
                  object Label34: TLabel
                    Left = 16
                    Top = 57
                    Width = 30
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Path:'
                  end
                  object edN34: TEdit
                    Left = 116
                    Top = 52
                    Width = 421
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 1
                  end
                  object cbN34: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 125
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable'
                    TabOrder = 0
                  end
                end
              end
              object tsN35: TTabSheet
                Caption = '[N35]'
                ImageIndex = 27
                object grpN35: TGroupBox
                  Left = 0
                  Top = 0
                  Width = 557
                  Height = 230
                  Hint = 'N16_gb'
                  Align = alTop
                  Caption = '[N35]  Record Ground and ESD at intervals and upload'
                  TabOrder = 0
                  object lblN35_3: TLabel
                    Left = 16
                    Top = 109
                    Width = 31
                    Height = 16
                    Hint = 'N16_lb3'
                    Caption = 'Host:'
                  end
                  object lblN35_4: TLabel
                    Left = 16
                    Top = 137
                    Width = 78
                    Height = 16
                    Hint = 'N16_lb4'
                    Caption = 'Upload Path:'
                  end
                  object lblN35_1: TLabel
                    Left = 16
                    Top = 52
                    Width = 72
                    Height = 16
                    Hint = 'N16_lb1'
                    Caption = 'User Name:'
                  end
                  object lblN35_2: TLabel
                    Left = 16
                    Top = 81
                    Width = 63
                    Height = 16
                    Hint = 'N16_lb2'
                    Caption = 'Password:'
                  end
                  object edN35_3: TEdit
                    Left = 108
                    Top = 104
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed3'
                    TabOrder = 3
                    Text = '192.168.119.243'
                  end
                  object edN35_4: TEdit
                    Left = 108
                    Top = 132
                    Width = 270
                    Height = 24
                    Hint = 'N16_ed4'
                    TabOrder = 4
                    Text = '\\HT_JAM\\HT-9046HA-8H\\'
                  end
                  object edN35_1: TEdit
                    Left = 108
                    Top = 48
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed1'
                    TabOrder = 1
                    Text = 'SGPHANDLER'
                  end
                  object edN35_2: TEdit
                    Left = 108
                    Top = 76
                    Width = 155
                    Height = 24
                    Hint = 'N16_ed2'
                    PasswordChar = '*'
                    TabOrder = 2
                    Text = 'CHROMA'
                  end
                  object cbN35_1: TCheckBox
                    Left = 12
                    Top = 28
                    Width = 221
                    Height = 17
                    Hint = 'N16'
                    Caption = 'Enable Upload FTP'
                    TabOrder = 0
                  end
                  object rgN35_1: TRadioGroup
                    Left = 2
                    Top = 168
                    Width = 553
                    Height = 60
                    Align = alBottom
                    Caption = 'Interval '
                    Columns = 3
                    ItemIndex = 0
                    Items.Strings = (
                      '30 min'
                      '60 min'
                      '90 min'
                      '120min')
                    TabOrder = 5
                  end
                  object btnN35_Test: TButton
                    Left = 344
                    Top = 192
                    Width = 75
                    Height = 25
                    Caption = 'Manual'
                    TabOrder = 6
                    OnClick = btnN35_TestClick
                  end
                end
              end
            end
            object grpN20: TGroupBox
              Left = 2
              Top = 572
              Width = 565
              Height = 47
              Align = alBottom
              Caption = '[N20] Check Sum Function'
              TabOrder = 1
              object chkN20: TCheckBox
                Left = 12
                Top = 24
                Width = 317
                Height = 17
                Caption = 'Check MD5 when download setup file from server'
                TabOrder = 0
              end
            end
          end
          object gbN04: TGroupBox
            Left = 0
            Top = 0
            Width = 921
            Height = 116
            Align = alTop
            Caption = '[N04] Machine Info'
            TabOrder = 0
            object labN04_1: TLabel
              Left = 12
              Top = 20
              Width = 92
              Height = 16
              Caption = 'Machine Model'
            end
            object labN04_2: TLabel
              Left = 12
              Top = 45
              Width = 67
              Height = 16
              Caption = 'Machine ID'
            end
            object labATCVer: TLabel
              Left = 13
              Top = 71
              Width = 51
              Height = 16
              Caption = 'ATC Ver'
              Visible = False
            end
            object labGPIBVer: TLabel
              Left = 13
              Top = 96
              Width = 55
              Height = 16
              Caption = 'GPIB Ver'
              Visible = False
            end
            object lblN04_IP: TLabel
              Left = 624
              Top = 20
              Width = 66
              Height = 16
              Caption = 'Machine IP'
            end
            object lblN04_Host: TLabel
              Left = 336
              Top = 20
              Width = 116
              Height = 16
              Caption = 'Machine host name'
            end
            object lblN04_TesterID: TLabel
              Left = 336
              Top = 44
              Width = 55
              Height = 16
              Caption = 'Tester ID'
            end
            object edN04_Model: TEdit
              Left = 133
              Top = 16
              Width = 120
              Height = 24
              TabOrder = 0
              Text = 'HT9045W'
              OnClick = edN04_ModelClick
            end
            object edN04_ID: TEdit
              Left = 133
              Top = 43
              Width = 120
              Height = 24
              TabOrder = 4
              Text = 'HT10'
              OnClick = edN04_ModelClick
            end
            object btResume: TButton
              Left = 177
              Top = 72
              Width = 75
              Height = 25
              Caption = 'Resume'
              TabOrder = 5
              OnClick = btResumeClick
            end
            object edtN04_Host: TEdit
              Left = 461
              Top = 16
              Width = 121
              Height = 24
              Enabled = False
              TabOrder = 1
            end
            object mmoN04_IP: TMemo
              Left = 713
              Top = 18
              Width = 206
              Height = 96
              Align = alRight
              ScrollBars = ssVertical
              TabOrder = 2
            end
            object edtN04_TesterID: TEdit
              Left = 461
              Top = 40
              Width = 121
              Height = 24
              TabOrder = 3
            end
          end
        end
        object tsP00: TTabSheet
          Caption = 'P [ Tray ]'
          ImageIndex = 9
          object MemoP: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pcP00: TPageControl
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            ActivePage = tsP20
            Align = alClient
            TabIndex = 2
            TabOrder = 0
            object tsP_00: TTabSheet
              Caption = '[P01] - [P10]'
              object pal_P0: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object lblP04: TLabel
                  Left = 26
                  Top = 35
                  Width = 307
                  Height = 16
                  Caption = '[P04] must be turned on and [P24] must be turned on'
                  Color = 12761254
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clRed
                  Font.Height = -13
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentColor = False
                  ParentFont = False
                end
                object cbP04: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 400
                  Height = 17
                  Caption = '[P04] Color stage regard as empty tray unloader to use.'
                  TabOrder = 0
                end
                object cbP10: TCheckBox
                  Left = 5
                  Top = 225
                  Width = 404
                  Height = 17
                  Caption = '[P10] Fix tary loads new tary need propose the initial question.'
                  Enabled = False
                  TabOrder = 7
                end
                object cbP05: TCheckBox
                  Left = 5
                  Top = 71
                  Width = 400
                  Height = 17
                  Caption = '[P05] Tray lifter cylinder should pre-on when initial start'
                  TabOrder = 1
                end
                object cbP06: TCheckBox
                  Left = 5
                  Top = 121
                  Width = 404
                  Height = 17
                  Caption = '[P06] Carrier tray can use.'
                  Enabled = False
                  TabOrder = 3
                end
                object cbP07: TCheckBox
                  Left = 5
                  Top = 147
                  Width = 404
                  Height = 17
                  Caption = '[P07] Auto tray feed when loader no tray.'
                  Enabled = False
                  TabOrder = 4
                end
                object cbP08: TCheckBox
                  Left = 5
                  Top = 171
                  Width = 404
                  Height = 17
                  Caption = '[P08] Clear lot ID after tray feed.'
                  Enabled = False
                  TabOrder = 5
                end
                object cbP09: TCheckBox
                  Left = 5
                  Top = 197
                  Width = 404
                  Height = 17
                  Caption = '[P09] Tray end can select receive tray.'
                  Enabled = False
                  TabOrder = 6
                end
                object cbP05_1: TCheckBox
                  Left = 17
                  Top = 95
                  Width = 428
                  Height = 17
                  Caption = 
                    '[P05-1] Unloader tray lifter cylinder should pre-on when swap tr' +
                    'ay.'
                  TabOrder = 2
                end
              end
            end
            object tsP_10: TTabSheet
              Caption = '[P11] - [P20]'
              ImageIndex = 1
              object pal_P1: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbP15: TCheckBox
                  Left = 5
                  Top = 205
                  Width = 400
                  Height = 17
                  Caption = '[P15] Free unload tray cylinder when open door'
                  TabOrder = 2
                end
                object cbP17: TCheckBox
                  Left = 5
                  Top = 315
                  Width = 337
                  Height = 17
                  Caption = '[P17] In arm picker must wait loader tray. (full pick up) '
                  TabOrder = 4
                end
                object cbP18: TCheckBox
                  Left = 5
                  Top = 345
                  Width = 325
                  Height = 14
                  Caption = '[P18] Autotray is fail bin must manual put tray  '
                  TabOrder = 5
                end
                object cbP19: TCheckBox
                  Left = 5
                  Top = 372
                  Width = 384
                  Height = 22
                  Caption = '[P19] Catch tray goes up then check if had catched tray'
                  TabOrder = 6
                end
                object cbP20: TCheckBox
                  Left = 5
                  Top = 403
                  Width = 384
                  Height = 14
                  Caption = '[P20] Must manual clear fix tray before initial start'
                  TabOrder = 7
                end
                object cbP11: TCheckBox
                  Left = 5
                  Top = 10
                  Width = 380
                  Height = 17
                  Caption = '[P11] Record UPH information'
                  Enabled = False
                  TabOrder = 0
                end
                object chkP20_1: TCheckBox
                  Left = 21
                  Top = 431
                  Width = 384
                  Height = 14
                  Caption = '[P20-1] Must manual clear auto tray before initial start'
                  TabOrder = 8
                end
                object pnlP13: TPanel
                  Left = 4
                  Top = 32
                  Width = 425
                  Height = 169
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 1
                  object labP13: TLabel
                    Left = 32
                    Top = 31
                    Width = 282
                    Height = 16
                    Caption = '[P13-1] Loop delay time                   (Unit : 0.1 Sec)'
                  end
                  object labP14_2: TLabel
                    Left = 32
                    Top = 141
                    Width = 329
                    Height = 16
                    Caption = 
                      '[P14-2] Receive loop delay time                  (Unit : 0.1 Sec' +
                      ')'
                  end
                  object labP14_1: TLabel
                    Left = 32
                    Top = 109
                    Width = 236
                    Height = 16
                    Caption = '[P14-1] Delay edge push cylinder count '
                  end
                  object labP13_2: TLabel
                    Left = 33
                    Top = 58
                    Width = 280
                    Height = 16
                    Caption = '[P13-2] On delay time                       (Unit : 0.1 Sec)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object cbP13: TCheckBox
                    Left = 0
                    Top = 4
                    Width = 392
                    Height = 17
                    Caption = '[P13] Enable auto tray edge push cylinder loop function.'
                    TabOrder = 0
                  end
                  object edP13_1: TEdit
                    Left = 177
                    Top = 27
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 1
                    Text = '20'
                  end
                  object cbP14: TCheckBox
                    Left = 0
                    Top = 83
                    Width = 331
                    Height = 17
                    Caption = '[P14] Enable auto tray receive delay function.'
                    TabOrder = 3
                  end
                  object edP14_1: TEdit
                    Left = 275
                    Top = 104
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 4
                    Text = '5'
                  end
                  object edP14_2: TEdit
                    Left = 225
                    Top = 136
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 5
                    Text = '5'
                  end
                  object edP13_2: TEdit
                    Left = 177
                    Top = 57
                    Width = 49
                    Height = 24
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    ParentFont = False
                    TabOrder = 2
                    Text = '1'
                  end
                end
                object pnlP16: TPanel
                  Left = 4
                  Top = 236
                  Width = 409
                  Height = 77
                  BevelOuter = bvNone
                  Color = 12761254
                  TabOrder = 3
                  object labP16: TLabel
                    Left = 33
                    Top = 27
                    Width = 282
                    Height = 16
                    Caption = '[P16-1] Loop delay time                   (Unit : 0.1 Sec)'
                  end
                  object labP16_2: TLabel
                    Left = 33
                    Top = 52
                    Width = 280
                    Height = 16
                    Caption = '[P16-2] On delay time                       (Unit : 0.1 Sec)'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                  end
                  object cbP16: TCheckBox
                    Left = 1
                    Top = 1
                    Width = 392
                    Height = 17
                    Caption = '[P16] Enable hot plate edge push cylinder loop function.'
                    TabOrder = 0
                  end
                  object edP16_1: TEdit
                    Left = 178
                    Top = 23
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 1
                    Text = '1'
                  end
                  object edP16_2: TEdit
                    Left = 177
                    Top = 50
                    Width = 49
                    Height = 24
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    ParentFont = False
                    TabOrder = 2
                    Text = '1'
                  end
                end
                object chkP20_2: TCheckBox
                  Left = 21
                  Top = 455
                  Width = 384
                  Height = 14
                  Caption = '[P20-2] Must manual clear loader tray before initial start'
                  TabOrder = 9
                end
              end
            end
            object tsP20: TTabSheet
              Caption = '[P21] - [P30]'
              ImageIndex = 2
              object pal_P2: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbP21: TCheckBox
                  Left = 5
                  Top = 27
                  Width = 384
                  Height = 14
                  Caption = '[P21] Tray feed finish will notice to take out fix tray'
                  TabOrder = 0
                end
                object cbP22: TCheckBox
                  Left = 5
                  Top = 110
                  Width = 384
                  Height = 14
                  Caption = '[P22] When initial start, firsit tray need alarm '
                  TabOrder = 4
                end
                object coP22: TComboBox
                  Left = 284
                  Top = 105
                  Width = 65
                  Height = 24
                  ItemHeight = 16
                  TabOrder = 3
                  Text = 'Auto1'
                  Items.Strings = (
                    'Auto1'
                    'Auto2'
                    'Auto3')
                end
                object cbP24: TCheckBox
                  Left = 5
                  Top = 254
                  Width = 428
                  Height = 14
                  Caption = '[P24] Skip event happen need remove Empty and Color tray'
                  TabOrder = 6
                end
                object gbP23_OCR: TGroupBox
                  Left = 12
                  Top = 135
                  Width = 385
                  Height = 105
                  Caption = '[P23] OCR conditions'
                  TabOrder = 5
                  object lblP23: TLabel
                    Left = 196
                    Top = 72
                    Width = 48
                    Height = 16
                    Caption = 'devices'
                  end
                  object lblP23_1: TLabel
                    Left = 84
                    Top = 71
                    Width = 57
                    Height = 16
                    Caption = 'Maximum'
                  end
                  object rgP23_OCRByNewTray: TRadioButton
                    Left = 8
                    Top = 24
                    Width = 137
                    Height = 17
                    Caption = 'By new tray interval'
                    Checked = True
                    TabOrder = 1
                    TabStop = True
                  end
                  object edP23_1: TEdit
                    Left = 159
                    Top = 20
                    Width = 41
                    Height = 24
                    BiDiMode = bdLeftToRight
                    ParentBiDiMode = False
                    TabOrder = 0
                    Text = '1'
                  end
                  object edP23_2: TEdit
                    Left = 145
                    Top = 68
                    Width = 41
                    Height = 24
                    TabOrder = 3
                    Text = '10'
                  end
                  object rgP23_OCRByInitialStart: TRadioButton
                    Left = 8
                    Top = 48
                    Width = 105
                    Height = 17
                    Caption = 'By initial start'
                    TabOrder = 2
                  end
                end
                object cbP25: TCheckBox
                  Left = 5
                  Top = 336
                  Width = 428
                  Height = 20
                  Caption = '[P25] Empty or Color tray no supple Auto 1 2 3, no load one tray'
                  TabOrder = 9
                end
                object gbP26_OCRCheck: TGroupBox
                  Left = 16
                  Top = 363
                  Width = 385
                  Height = 97
                  Caption = '[P26] OCR check'
                  TabOrder = 10
                  object lblP26: TLabel
                    Left = 16
                    Top = 41
                    Width = 20
                    Height = 16
                    Caption = 'X'#65306
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    Visible = False
                  end
                  object cbP26: TCheckBox
                    Left = 13
                    Top = 19
                    Width = 244
                    Height = 14
                    Caption = '[P26] OCR check lot'
                    TabOrder = 0
                  end
                  object edtP26: TEdit
                    Left = 48
                    Top = 37
                    Width = 45
                    Height = 24
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -13
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ParentFont = False
                    TabOrder = 1
                    Text = '2'
                    Visible = False
                  end
                end
                object cbP27: TCheckBox
                  Left = 5
                  Top = 472
                  Width = 428
                  Height = 20
                  Caption = '[P27] Auto sorting bin tray by out arm when clean out'
                  TabOrder = 11
                end
                object cbP28: TCheckBox
                  Left = 5
                  Top = 496
                  Width = 428
                  Height = 20
                  Caption = '[P28] Auto 1 only can set bin 1'
                  TabOrder = 12
                end
                object cbP29: TCheckBox
                  Left = 5
                  Top = 520
                  Width = 428
                  Height = 20
                  Caption = '[P29] Always check loader is full. Checking interval (sec):'
                  TabOrder = 14
                end
                object edP29: TEdit
                  Left = 359
                  Top = 519
                  Width = 70
                  Height = 24
                  BiDiMode = bdLeftToRight
                  ParentBiDiMode = False
                  TabOrder = 13
                  Text = '1'
                end
                object cbP30: TCheckBox
                  Left = 5
                  Top = 544
                  Width = 373
                  Height = 20
                  Caption = 
                    '[P30] Fix tray be left over IC continue                         ' +
                    '   pieces'
                  TabOrder = 16
                end
                object edP30: TEdit
                  Left = 239
                  Top = 543
                  Width = 70
                  Height = 24
                  BiDiMode = bdLeftToRight
                  ParentBiDiMode = False
                  TabOrder = 15
                  Text = '1'
                end
                object cbP24_2: TCheckBox
                  Left = 25
                  Top = 277
                  Width = 413
                  Height = 17
                  Caption = '[P24-2] Skip event happen need remove Color tray (for IDT)'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbP24_3: TCheckBox
                  Left = 24
                  Top = 297
                  Width = 369
                  Height = 17
                  Caption = '[P24-3] Two tray  must be manually removed for genernal'
                  TabOrder = 8
                end
                object chkP21_1: TCheckBox
                  Left = 25
                  Top = 51
                  Width = 384
                  Height = 14
                  Caption = '[P21-1] Initial start check fix tray should have tray.'
                  TabOrder = 1
                end
                object chkP21_2: TCheckBox
                  Left = 25
                  Top = 75
                  Width = 384
                  Height = 14
                  Caption = '[P21-2] Tray feed include loader tray.'
                  TabOrder = 2
                end
              end
            end
            object tsP30: TTabSheet
              Caption = '[P31] - [P50]'
              ImageIndex = 3
              object pal_P3: TPanel
                Left = 0
                Top = 0
                Width = 471
                Height = 706
                Align = alClient
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 0
                object cbP31: TCheckBox
                  Left = 5
                  Top = 13
                  Width = 428
                  Height = 20
                  Caption = '[P31] Loader tray last one feed continue run'
                  TabOrder = 0
                end
                object cbP32: TCheckBox
                  Left = 4
                  Top = 38
                  Width = 428
                  Height = 20
                  Caption = '[P32] Empty/Color Tray Pre Alarm'
                  TabOrder = 1
                end
                object cbP33: TCheckBox
                  Left = 4
                  Top = 64
                  Width = 428
                  Height = 20
                  Caption = '[P33] Unloader Stock Full Pre Alarm'
                  TabOrder = 2
                end
                object cbP34: TCheckBox
                  Left = 4
                  Top = 89
                  Width = 428
                  Height = 20
                  Caption = '[P34] Cleanout change initial start'
                  TabOrder = 3
                end
                object cbP35: TCheckBox
                  Left = 5
                  Top = 114
                  Width = 428
                  Height = 20
                  Caption = '[P35] Tray Arm home safe pos'
                  TabOrder = 4
                end
                object cbP36: TCheckBox
                  Left = 5
                  Top = 140
                  Width = 428
                  Height = 20
                  Caption = '[P36] Load && Unload tray buffer tray trace no setup same'
                  TabOrder = 5
                end
                object cbP37: TCheckBox
                  Left = 5
                  Top = 165
                  Width = 428
                  Height = 20
                  Caption = '[P37] Auto 1 2 3 Z cylinder always up.'
                  TabOrder = 6
                end
                object cbP38: TCheckBox
                  Left = 4
                  Top = 191
                  Width = 400
                  Height = 17
                  Caption = '[P38] Use empty full tray put to color'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 7
                end
                object cbP40: TCheckBox
                  Left = 4
                  Top = 235
                  Width = 213
                  Height = 17
                  Caption = '[P40] Tray Y speed by machine'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 9
                end
                object cbP42: TCheckBox
                  Left = 4
                  Top = 280
                  Width = 293
                  Height = 17
                  Caption = '[P42] Alarm when exiting Tray is complete'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 11
                end
                object cbP39: TCheckBox
                  Left = 4
                  Top = 213
                  Width = 445
                  Height = 17
                  Caption = 
                    '[P39] Loader tray-end, skip, clean-out event happen place to emp' +
                    'ty'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 8
                end
                object gbUnloadMode: TGroupBox
                  Left = 4
                  Top = 305
                  Width = 458
                  Height = 88
                  Caption = '[P43] Unloading Tray Mode'
                  TabOrder = 12
                  object chkAutoTrayFeed: TCheckBox
                    Left = 20
                    Top = 17
                    Width = 353
                    Height = 17
                    Caption = 'Feed trays automatically after cleaning out device'
                    TabOrder = 0
                  end
                  object cbManuTakeAutoFailTray_FT: TCheckBox
                    Left = 20
                    Top = 40
                    Width = 325
                    Height = 17
                    Caption = '[FT] Auto tray is fail bin must manual put tray  '
                    Checked = True
                    State = cbChecked
                    TabOrder = 1
                  end
                  object cbManuTakeAutoFailTray_RT: TCheckBox
                    Left = 20
                    Top = 62
                    Width = 325
                    Height = 17
                    Caption = '[RT] Auto tray is fail bin must manual put tray  '
                    Checked = True
                    State = cbChecked
                    TabOrder = 2
                  end
                end
                object cbP45: TCheckBox
                  Left = 4
                  Top = 424
                  Width = 293
                  Height = 17
                  Caption = '[P45] Loader Empty tray No In Side'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 14
                end
                object cbP44: TCheckBox
                  Left = 4
                  Top = 397
                  Width = 457
                  Height = 17
                  Caption = 
                    '[P44] Lock '#39'Loader tray mode'#39' to '#39'None'#39'(Move out loader tray by ' +
                    'tray arm)'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 13
                end
                object cbP46: TCheckBox
                  Left = 4
                  Top = 452
                  Width = 293
                  Height = 17
                  Caption = '[P46] Loader tray mode save by handler.'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 15
                end
                object cbP50: TCheckBox
                  Left = 4
                  Top = 608
                  Width = 293
                  Height = 17
                  Caption = '[P50] Disabled auto track sensor detect'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 18
                end
                object grpP48: TGroupBox
                  Left = 4
                  Top = 481
                  Width = 458
                  Height = 96
                  Caption = '[P48] Edge push and Fixer cylinder loop'
                  TabOrder = 16
                  object lblP48_1: TLabel
                    Left = 29
                    Top = 47
                    Width = 72
                    Height = 16
                    Caption = 'Loop Times'
                  end
                  object lblP48_2: TLabel
                    Left = 29
                    Top = 70
                    Width = 136
                    Height = 16
                    Caption = 'Loop Delay Time(Sec)'
                  end
                  object cbP48: TCheckBox
                    Left = 4
                    Top = 24
                    Width = 453
                    Height = 17
                    Caption = '[P48] Enable Auto1~3 edge push and Fixer cylinder loop'
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                  end
                  object edP48Times: TEdit
                    Left = 182
                    Top = 43
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 1
                    Text = '1'
                  end
                  object edP48DelayTime: TEdit
                    Left = 182
                    Top = 67
                    Width = 49
                    Height = 24
                    ImeName = 'Chinese (Traditional) - Phonetic'
                    MaxLength = 2
                    TabOrder = 2
                    Text = '1'
                  end
                end
                object cbP41: TCheckBox
                  Left = 4
                  Top = 258
                  Width = 213
                  Height = 17
                  Caption = '[P41] Unload tray disable edit'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 10
                end
                object cbP49: TCheckBox
                  Left = 4
                  Top = 585
                  Width = 400
                  Height = 17
                  Caption = '[P49] Use Local Tray Speed'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 17
                end
              end
            end
            object tsP50: TTabSheet
              Caption = '[P51] - [P65]'
              ImageIndex = 4
              object cbP51: TCheckBox
                Left = 4
                Top = 13
                Width = 373
                Height = 17
                Caption = '[P51] Tray Arm wait unload tray finish, then put tray to track'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 0
              end
              object cbP52: TCheckBox
                Left = 4
                Top = 39
                Width = 312
                Height = 17
                Caption = '[P52] Empty/Color Last Tray Check'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
              end
              object cbP53: TCheckBox
                Left = 4
                Top = 62
                Width = 452
                Height = 20
                Caption = 
                  '[P53] Forced scan bin code label before takeout the tray from un' +
                  'loader.'
                TabOrder = 2
              end
              object cbP54: TCheckBox
                Left = 4
                Top = 88
                Width = 293
                Height = 17
                Caption = '[P54] Unloader tary check has error bin IC'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 3
              end
              object cbP55: TCheckBox
                Left = 4
                Top = 112
                Width = 453
                Height = 17
                Caption = 
                  '[P55] Load && Unload Use Empty And Color Tray ( 4 Auto Tray Lane' +
                  ' Only)'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 4
              end
              object chkP56: TCheckBox
                Left = 4
                Top = 136
                Width = 453
                Height = 17
                Caption = '[P56] Tray arm wait at color track'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 5
              end
              object cbP57: TCheckBox
                Left = 4
                Top = 155
                Width = 285
                Height = 25
                Caption = '[P57] Loader auto clean out by input count'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 6
              end
              object rgP58RunModeAfterFT: TRadioGroup
                Left = 6
                Top = 184
                Width = 453
                Height = 33
                Caption = '[P58] Run mode After FT'
                Columns = 2
                Items.Strings = (
                  'FT'
                  'RT')
                TabOrder = 7
              end
              object cbP59: TCheckBox
                Left = 4
                Top = 256
                Width = 293
                Height = 17
                Caption = '[P59] Unloader IC Floatting Alarm after exit'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 8
              end
              object cbP60: TCheckBox
                Left = 4
                Top = 280
                Width = 469
                Height = 17
                Caption = '[P60] Read Clip Code From Unloader(Auto1-3'#12289'Fix1-3)'
                ParentShowHint = False
                ShowHint = True
                TabOrder = 9
              end
              object plP62: TPanel
                Left = 0
                Top = 320
                Width = 465
                Height = 89
                BevelInner = bvLowered
                Color = 12761254
                TabOrder = 10
                object cbP62: TCheckBox
                  Left = 4
                  Top = 8
                  Width = 400
                  Height = 17
                  Caption = '[P62] First Tray Check On Unloader'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
                object cbP62_Auto1: TCheckBox
                  Left = 20
                  Top = 32
                  Width = 117
                  Height = 17
                  Caption = 'Auto1 Position'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                end
                object cbP62_Auto2: TCheckBox
                  Left = 144
                  Top = 32
                  Width = 117
                  Height = 17
                  Caption = 'Auto2 Position'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 2
                end
                object cbP62_Auto3: TCheckBox
                  Left = 268
                  Top = 32
                  Width = 117
                  Height = 17
                  Caption = 'Auto3 Position'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 3
                end
                object cbP62_1: TCheckBox
                  Left = 20
                  Top = 56
                  Width = 400
                  Height = 17
                  Caption = '[P62-1] Function always enabled every time lot start'
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 4
                end
              end
            end
          end
        end
        object tsM00: TTabSheet
          Caption = 'M [ Monitor ]'
          ImageIndex = 11
          object MemoM: TMemo
            Left = 479
            Top = 0
            Width = 442
            Height = 737
            Align = alRight
            Color = 14670284
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 1
          end
          object pal_M: TPanel
            Left = 0
            Top = 0
            Width = 479
            Height = 737
            Align = alClient
            BevelInner = bvLowered
            Color = 12761254
            TabOrder = 0
            object gbM01: TGroupBox
              Left = 4
              Top = 32
              Width = 405
              Height = 377
              Caption = 'Monitor Option'
              TabOrder = 1
              object cbM01_01: TCheckBox
                Left = 16
                Top = 24
                Width = 377
                Height = 17
                Caption = '[M01-1] Contact mode must select different speed'
                TabOrder = 0
                OnClick = cbM01Click
              end
              object cbM01_02: TCheckBox
                Left = 16
                Top = 48
                Width = 377
                Height = 17
                Caption = '[M01-2] Site yield different must on'
                TabOrder = 1
                OnClick = cbM01Click
              end
              object cbM01_03: TCheckBox
                Left = 16
                Top = 72
                Width = 377
                Height = 17
                Caption = '[M01-3] Site yield continue fail by socket must on'
                TabOrder = 2
                OnClick = cbM01Click
              end
              object cbM01_04: TCheckBox
                Left = 16
                Top = 96
                Width = 377
                Height = 17
                Caption = '[M01-4] Site yield continue fail by head must on'
                TabOrder = 3
                OnClick = cbM01Click
              end
              object cbM01_05: TCheckBox
                Left = 16
                Top = 120
                Width = 377
                Height = 17
                Caption = '[M01-5] In/Out Arm device check must on'
                TabOrder = 4
                OnClick = cbM01Click
              end
              object cbM01_06: TCheckBox
                Left = 16
                Top = 144
                Width = 377
                Height = 17
                Caption = '[M01-6] Index destory check must on'
                TabOrder = 5
                OnClick = cbM01Click
              end
              object cbM01_07: TCheckBox
                Left = 16
                Top = 168
                Width = 377
                Height = 17
                Caption = '[M01-7] Auto speed must on'
                TabOrder = 6
                OnClick = cbM01Click
              end
              object cbM01_08: TCheckBox
                Left = 16
                Top = 192
                Width = 377
                Height = 17
                Caption = '[M01-8] Every first device have initial delay time'
                TabOrder = 7
                OnClick = cbM01Click
              end
              object cbM01_09: TCheckBox
                Left = 16
                Top = 216
                Width = 377
                Height = 17
                Caption = '[M01-9] If handler RTC off check piggy back function'
                TabOrder = 8
                OnClick = cbM01Click
              end
              object cbM01_10: TCheckBox
                Left = 16
                Top = 240
                Width = 377
                Height = 17
                Caption = '[M01-10] Disable [I12] function'
                TabOrder = 9
                OnClick = cbM01Click
              end
              object cbM01_11: TCheckBox
                Left = 16
                Top = 264
                Width = 377
                Height = 17
                Caption = '[M01-11] Enable [I22] function'
                TabOrder = 10
                Visible = False
                OnClick = cbM01Click
              end
              object cbM01_12: TCheckBox
                Left = 16
                Top = 288
                Width = 377
                Height = 17
                Caption = '[M01-12] Enable auto clean function'
                TabOrder = 11
                OnClick = cbM01Click
              end
              object cbM01_13: TCheckBox
                Left = 16
                Top = 312
                Width = 377
                Height = 17
                Caption = '[M01-13] Enable 2DID function'
                TabOrder = 12
                OnClick = cbM01Click
              end
              object cbM01_14: TCheckBox
                Left = 16
                Top = 336
                Width = 377
                Height = 17
                Caption = '[M01-14] Enable ATC function'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 13
                OnClick = cbM01Click
              end
            end
            object cbM01: TCheckBox
              Left = 5
              Top = 10
              Width = 324
              Height = 17
              Caption = '[M01] Enable monitor funciton'
              TabOrder = 0
              OnClick = cbM01Click
            end
          end
        end
        object tsSearchFunction: TTabSheet
          Caption = 'Search Function'
          ImageIndex = 13
          object scrlbxSearch: TScrollBox
            Left = 0
            Top = 0
            Width = 921
            Height = 658
            Align = alClient
            TabOrder = 0
          end
          object pnlsearch: TPanel
            Left = 0
            Top = 658
            Width = 921
            Height = 54
            Align = alBottom
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object edtSearchFunction: TLabeledEdit
              Left = 120
              Top = 9
              Width = 185
              Height = 24
              EditLabel.Width = 96
              EditLabel.Height = 16
              EditLabel.Caption = 'Search Function'
              LabelPosition = lpLeft
              LabelSpacing = 3
              TabOrder = 0
              OnChange = edtSearchFunctionChange
            end
          end
        end
      end
    end
    object tsTrayData: TTabSheet
      Caption = 'Tray'
      ImageIndex = 3
      object palTrayDef: TPanel
        Left = 0
        Top = 0
        Width = 929
        Height = 40
        Align = alTop
        BevelInner = bvLowered
        Caption = 'Tray Define'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object strngrdTray: TStringGrid
        Left = 0
        Top = 93
        Width = 929
        Height = 680
        Align = alClient
        Color = 14670284
        TabOrder = 1
        OnDblClick = strngrdTrayDblClick
        OnSelectCell = strngrdTraySelectCell
      end
      object pnlTray: TPanel
        Left = 0
        Top = 40
        Width = 929
        Height = 53
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 2
        object btnAddTray: TSpeedButton
          Left = 190
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Add Tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D296B2A52E9465008A4900
            8949008949008A492E946596B2A5D2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D24B9D7700915200B67900CD9000
            D19400D19400CD9000B6790091524B9D77D2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF4EA07A00965600CB8F00CD8F00C98A00
            C68500C68500C98A00CD8F00CB8F0096564EA07ADFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E09FBBAE00925200C98D00CA8C00C78800C380FF
            FFFFFFFFFF00C38000C78800CA8C00C98D009251A2BEB1F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B833966A00B37500C88A00C58600C38300BE79FF
            FFFFFFFFFF00BE7900C38300C58600C88A00B374329969EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09BA7D09CC7E0900884900C58700C38300BE7C00BC7700B86FFF
            FFFFFFFFFF00B86F00BC7700BE7C00C38300C587008A4AE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF6F8FFFFFDFF00823E17CC9400BD7AFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BD7A1BCD96008949E9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEFEDEFFFF3FB007F3835D29F00BB76FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BB7739D3A2008948EAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EBE7E8FDEEF3007D3368D9B500BB7800B97400B77100B268FF
            FFFFFFFFFF00B26800B77100B97400BB786EDCB9008947F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF3EA97856C19728C89300BA7800B97800B46EFF
            FFFFFFFFFF00B46E00B97800BA782AC89563C7A03BA171F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFC8E6D900894587E0C11CC38A00B77400B36DFF
            FFFFFFFFFF00B36D00B7741DC48B8DE3C60B9457C1DDD0FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF4FB08400915091E1C55BD4AA0ABC7E00
            B06800B0680BBC7E5BD4AB93E3C90E995F61B18CFCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF65BB9400894553BF9490E0C39B
            E7CD9BE7CD90E0C353C09600905152862FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCEEDDF42AE7D00813700
            823900823900813634A171C1E2DECA7F0CE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B1B1B1B1B5B2B3BAB5B8C2B8BDC7BAC0C9
            BBC1C8BBC1C5B9BEEFE5EAFEFEFFBA7E0BE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DEDDE0DFDEE2DFDFE3DFE0E3
            E0E0E3E0E0E2DFDEDFDEDFF6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnAddTrayClick
        end
        object btnDeleteTray: TSpeedButton
          Left = 372
          Top = 9
          Width = 170
          Height = 36
          Caption = 'Delete Tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D29FA2BD4F5BBB2B3BBF2B
            3BBE2B3BBE2B3BBF4F5BBB9FA2BDD2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D2666EBC3344C75164EB6578FF69
            7CFF697CFF6578FF5164EB3344C7666EBCD2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF6972BF3546CA6072FE6174FE6073FD5F
            72FC5F72FC6073FD6174FE6072FE3546CA6972BFDFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E0A7ABC73242C85B6FFC5B6FFC596EFA596DF959
            6DF9596DF9596DF9596EFA5B6FFC5B6FFC3242C7ABAEC9F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B84B5AC4465AE8566BFA5268F85167F75166F751
            66F75166F75166F75167F75268F8566BFA475BE6545FC0EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09B97E07C687001D32CA4C63F84A61F54159F33D55F23C54F23C
            54F23C54F23C54F23D55F24159F34A61F54F64F62D3CBFE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF5F9FFFFFFFF192CC15D71F83B53F2FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF3B53F25F74F82C3BBFE9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEEEEEDFEFCEF1628BC6D81F8334EF0FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF334EF07084F82A3ABFEAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EAE8E6F9F5E91124BA8E9DF6324DED2F4AEC2C47EB2B47EB2B
            47EB2B47EB2B47EB2C47EB2F4AEC334EED93A2F72A3AC0F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF626ED17986E35870F0324DEB3550EB3651EB37
            51EB3751EB3651EB3550EB324DEB5A71F0848FE65A66C9F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFD2D5F22434C1A0ADF44C63ED2D48E9314BE933
            4DE9334DE9314BE92D48E94D64EEA5B2F63947C7CACDE8FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF707BD52E40C5A5B0F57B8DF03E57E925
            42E52542E53E57E97C8DF1A7B3F83B4DCF7882D2FCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF828CDB2334C27280E1A3AEF4AE
            B9F8AEB9F8A3AEF47281E3283CCD6C5A6FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD9DCF86773D71527BF17
            29C01729C01427BF5964CACBD3F7C58701E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B0B1B1B1B3B3B2B9B8B3BEBDB5C3C1B6C4
            C3B6C4C2B6C1C0B4ECEBE2FCFFFFB97F08E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DFDDE1E0DEE2E1DEE2
            E1DEE2E1DEE1E0DDDFDEDDF6FCFFB67D09E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnDeleteTrayClick
        end
        object btnModifyTray: TSpeedButton
          Left = 8
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Modify Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D296B2A52E9465008A4900
            8949008949008A492E946596B2A5D2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D24B9D7700915200B67900CD9000
            D19400D19400CD9000B6790091524B9D77D2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF4EA07A00965600CB8F00CD8F00C98A00
            C68500C68500C98A00CD8F00CB8F0096564EA07ADFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E09FBBAE00925200C98D00CA8C00C78800C380FF
            FFFFFFFFFF00C38000C78800CA8C00C98D009251A2BEB1F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B833966A00B37500C88A00C58600C38300BE79FF
            FFFFFFFFFF00BE7900C38300C58600C88A00B374329969EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09BA7D09CC7E0900884900C58700C38300BE7C00BC7700B86FFF
            FFFFFFFFFF00B86F00BC7700BE7C00C38300C587008A4AE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF6F8FFFFFDFF00823E17CC9400BD7AFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BD7A1BCD96008949E9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEFEDEFFFF3FB007F3835D29F00BB76FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BB7739D3A2008948EAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EBE7E8FDEEF3007D3368D9B500BB7800B97400B77100B268FF
            FFFFFFFFFF00B26800B77100B97400BB786EDCB9008947F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF3EA97856C19728C89300BA7800B97800B46EFF
            FFFFFFFFFF00B46E00B97800BA782AC89563C7A03BA171F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFC8E6D900894587E0C11CC38A00B77400B36DFF
            FFFFFFFFFF00B36D00B7741DC48B8DE3C60B9457C1DDD0FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF4FB08400915091E1C55BD4AA0ABC7E00
            B06800B0680BBC7E5BD4AB93E3C90E995F61B18CFCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF65BB9400894553BF9490E0C39B
            E7CD9BE7CD90E0C353C09600905152862FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCEEDDF42AE7D00813700
            823900823900813634A171C1E2DECA7F0CE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B1B1B1B1B5B2B3BAB5B8C2B8BDC7BAC0C9
            BBC1C8BBC1C5B9BEEFE5EAFEFEFFBA7E0BE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DEDDE0DFDEE2DFDFE3DFE0E3
            E0E0E3E0E0E2DFDEDFDEDFF6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnModifyTrayClick
        end
        object sbUpdateTray: TSpeedButton
          Left = 736
          Top = 9
          Width = 170
          Height = 36
          Caption = 'Save'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            660F0000424D660F000000000000360000002800000024000000240000000100
            180000000000300F000001000000010000000000000000000000FAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF9F9F9F9F9F9F8F8F8F8F8F8F8
            F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8
            F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F9F9F9F9F9F9FAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF7F7F7EFEFEFE8
            E8E8E6E6E6E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
            E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
            E5E5E5E5E9E9E9F3F3F3F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAF8F8F8ECECECD6D6D6C8C8C8C5C5C5C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
            C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
            C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4D0D0D0E9E9E9F9F9F9FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAF0EFEFC0A6A7A55B6CA96270A45E6C934556
            A6A3A3A4A1A1A19C9C9C9797979393948D8D8E88888A8383867F7D837A7A8079
            78807978994759994759994759994759994759994759994759994756C4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAD2C1C1B28181
            AF6D79B8717FB7707DAA6472EFEFF1EEEEF1995D68B8717EB06B78E1E0E0DDDA
            DBD8D5D6D4D1D2CFCCCCCAC6C6DBD8D88F4B59A05565A05565BE848EB9727FB8
            727EB8717E994557C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7AAF6D79BA7481B97380B7737FAA6472E9E9EBECEBEF995D
            68B8717EB06B78E1DFE1DCDADDD7D5D6D2D0D1CDC9CAC8C3C3D9D6D68D49589F
            5464A05565BF848EB97380B9737FB9727F9B4457C4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC38D95BA7682BA7481B974
            80AA6472E5E4E7E9E9EB9A5D69B8737FB06B78E5E5E7E2E0E3DDDBDDD8D5D7D3
            D0D1CDCACBDDDADB8B48579D53639F5464BF858FBA7480BA7481BA74819B4558
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7AC48E96BA7783BA7682B97481AA6472E0DFE1E5E3E69B5F6BB97580B06B78E9
            EAECE6E5E8E2E1E4DDDCDED9D6D8D4D0D2E0DFDF8945549B50619D5363BF8590
            BA7481BA7481BA74819A4759C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7AC58F96BB7784BA7784BA7582AA6472DAD9DBE0
            DEE09B5F6BBA7582B06B78EDECEFEAE9EDE6E6E9E2E2E4DEDDDFDAD8DAE5E2E4
            874353994E5F9B5061C18690B97480BA7481BA74819C495CC4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC58F98BC7886BB
            7785BB7783AA6472D5D2D4DBD8DA9C606C925864925864EAEAEDEDEDF0EAEAED
            E8E7E9E4E2E5DFDDDFE8E7E8864252974C5D994E5FC08691B9727FB97380B973
            809C4B5CC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7AC49098BC7986BD7985BC7784AA6472CFCCCCD4D1D3D9D7D9DFDCDF
            E2E1E4E7E5E8EAEAEDEDECF0EAEBEEE8E7EAE4E2E6ECEAEC8642528642528743
            53C28791B7727FB8727EB9727F9C4E5DC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC79198BD7A87BD7987BC7985BB7785
            AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA64
            72AA6472AA6472AA6472B77A84B76F7DB7707DB7707EB8717E9E4E5FC4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6929A
            BE7B88BE7B87BC7986BC7886BB7785BB7784BA7682B97481B97480B87380B871
            7EB7717EB76F7DB76D7CB76D7BB76D7BB66D7BB56C7AB56C7AB66D7BB76F7CB7
            6F7CB7707D9F4E60C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7AC6929ABE7B88BE7B87BC7986BC7886BB7785BB7784BA76
            82B97481B97480B87380B8717EB76F7DB76D7CB76D7CB76C7AB66C7AB66D7BB5
            6C7AB56C7AB56C7AB66C7AB66C7AB76D7C9F4F60C4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6939BBE7E89BC7884A75E
            70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A7
            5E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70B66B7AB66C7B9F5162
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7AC7949CBF7F8AAF6F7BEBE1E1FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFE
            FEFDFDFEFDFDFDFCFCFCFCFBFCFBFBFBFBFAFAFAF9F9F9F9F9F9F8F8F8E8D1D1
            A75E70B76D7BB66C7A9F5262C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7AC8959DC0808BAF6F7BFEFFFEFCFCFBFBFBFAFA
            FBFAFAFAFAF9FAF9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7
            F7F7F7F7F6F6F6F6F6F9F9F9A75E70B96F7DB66B7AA05363C4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9959DC1818CAF
            6F7BFEFFFEE1E1E1DFDFDFDDDDDDDBDCDBDADAD9D9D9D9D7D7D6D5D6D5D4D4D3
            D2D3D2D1D1D1D0D0D0CFCFCFCFCFCFCFCFCFCFCFCFF9FAF9A75E70BA707EB66B
            79A15466C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7AC9969EC1818DAF6F7BFEFFFEFCFCFCFCFCFBFBFBFBFBFBFAFBFAFA
            FAFAFAF9F9F9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7
            F6FBFBFBA75E70B97480B56B79A25666C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989FC2828EAF6F7BFEFFFEE4E4E4
            E3E3E3E2E2E1E0E0E0DEDDDEDCDCDCDADADAD9D9D9D7D8D8D6D5D5D4D4D4D2D3
            D3D1D2D1D0D0D0CFCFCFCFCFCFFCFCFBA75E70BA7581B56B79A45968C4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989F
            C2838EAF6F7BFEFFFEFCFCFCFCFCFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFA
            F9F9F9F9F9F9F9F9F9F8F8F9F8F8F8F8F8F7F7F7F7F7F7F7F7FCFCFCA75E70BA
            7783B66C7AA55969C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7ACA98A0C2838EAF6F7BFEFFFEE6E6E6E6E5E6E4E4E4E3E3
            E3E1E2E2E0E0E0DEDEDEDCDCDCDBDBDBD9D9D9D8D8D8D6D6D6D4D5D5D3D3D3D1
            D1D1D0D0D0FDFDFCA75E70BC7783B76D7BA55D6CC4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB98A0C2838EAF6F7BFEFF
            FEFDFDFDFDFDFCFCFDFCFCFCFCFCFCFBFBFCFBFBFBFBFBFBFAFAFAFAFAFAF9F9
            F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7FDFEFDA75E70BD7985B76F7DA65F6E
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7ACB9BA1C2838EAF6F7BFEFFFEE6E6E6E6E6E6E6E6E6E5E5E6E4E5E5E4E3E3E2
            E2E2E1E0E0DFDFDFDDDDDDDBDBDCDADAD9D9D8D9D7D6D7D5D5D5D3D4D3FEFEFE
            A75E70BE7A86B7707DA6606EC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFEFDFEFEFDFD
            FDFCFDFDFCFCFDFCFCFCFCFCFCFBFCFCFBFBFBFAFBFBFAFAFAF9F9F9F9F9F9F9
            F9F9F9F9F9F8F8F8F8FEFFFEA75E70B38088B8717FA7606EC4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF
            6F7BFEFFFEE6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E5E5E5E4E4E3E2E3E3
            E1E1E1E0DFDFDDDDDEDCDBDCDADADAD9D9D9D7D8D7FEFFFEA75E70996E759E59
            67A9606EC5C5C5E5E5E5F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFFFEFEFEFEFEFEFDFEFEFDFDFDFC
            FDFDFCFCFDFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFAF9FAFAF9F9F9F9F9F9
            F9FEFFFEA75E70B0757F9E5A68AA6471C5C5C5E6E6E6F9F9F9FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACA969EC2838EAF6F7BFEFFFEFEFFFE
            FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFF
            FEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEA75E70C77F8BC77F8BAA6572D1D1D1EB
            EBEBFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AAE6A7A
            AE6A7AAE6A7AD9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6
            D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6A75E70A7
            5E70A75E70A75E70ECECECF6F6F6FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFA}
          Margin = 20
          ParentFont = False
          OnClick = sbUpdateTrayClick
        end
        object sbtReloadTray: TSpeedButton
          Left = 554
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Load Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2F0F0F0F9F9
            F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEAEAEAD0D0D0D1D1D1EEEE
            EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD38996C008E4EE9E9
            E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD3E986E00C686008B4BD8D8
            D8E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9EBEBEBF1F1F1F8
            F8F8FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD3E986D00BF8200E3A6008848B7B7
            B7BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBDBDBDC1C1C1CCCCCCDB
            DBDBEAEAEAF7F7F7FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFBEBEBEBCDCDCD3E986D00BA8100D8A000D9A10083420087
            46008847008847008847008847008847008847008947008948078B4D3C986D83
            AA98C4C4C4D9D9D9EEEEEEFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFF2F2F2D1D1D13E986D00B88000D4A000D19C00D19C00D9A000DB
            A200DBA200DBA200DBA200DBA200DBA200DBA200D3A200D5A300C18B00AA7000
            8E4D058B4D85AB9ACCCCCCE7E7E7FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFEDEDED3F9C7100B78100D09F00CC9B00CB9A00CB9A00CE9B00CE
            9B00CE9B00CE9B00CE9B00CE9B00CE9B00CF9B00CB9B00CC9C00CD9D00CF9F00
            CD9C00AC750089472E9666C8C8C8E8E8E8FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFF2F2F2008A482CD9B807CBA100C89B00C89B00C89B00C89900C8
            9900C89900C89900C89900C89900C89900C89900C89B00C89B00C89B00C99C00
            CA9E00CDA100C293008E4D139157CECECEEFEFEFFEFEFEFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFB4CAA7E00B28162DFC700C39A00C29900C29800C19800C1
            9900C19900C19900C19900C19900C19900C19900C39800C39800C39900C49A00
            C59B00C69C00C8A000C093008D4D3F9C71DADADAF7F7F7FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFCFCFC51AB7F00AE7F60DCC600BE9794E5D74DE6D151E8
            D452E8D552E8D552E8D551E8D57FE1D37DE1D37CE4D172E1CC49D5B906C59E00
            C29900C39B00C49C00C6A000BE93008A478EB5A3EBEBEBFEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AB7F00AB7F5AD9C482E3D600823B0086
            42008743008743008743008743008743008A49169E6626B1826CD8C175E0CD27
            CBAD00BE9800C09A00C19C00C4A100A975068E4DDDDDDDF9F9F9FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AC7F00A97F7BE3DA008944E9E9
            E9FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEC8E5D8CBE5D969B791008A484BC19D81
            E2D328C8AC00BC9800BE9B00C09D00C09D008E4DA5C1B4F1F1F1FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AC7F00AB87008C4AEEEE
            EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEDEEDE61D92574C
            C09F85E1D30EBFA200B99A00BB9C00BEA100A17043A074EBEBEBFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFB4CAD80008E4DF9F9
            F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEEEC00
            8A4775D8C758D3C100B69800B79A00B99D00AF8C078D4CEBEBEBFBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9EDEDEDF7F7F7FEFEFEFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F961
            B18B2DB18A8FE4DE7BDDD47DDED47EDFD683E3DD008B48F2F2F2F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCC7C7C7E3E3E3F9F9F9FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEC0
            E0D1008B49008944008843008843008944008A474BAC7FFBFBFBEBEBEB3B9D70
            008949008747008747008746008746008A49A0C0B1F1F1F1FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9F0F0F0F2F2F2FBFBFBFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB008949
            11ECAE03E5A400E3A000E29E96FBDF2FBE89509F79E0E0E0F9F9F9FEFEFEFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEED1D1D1D0D0D0EAEAEAFC
            FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1078B4D
            22D39A14E3A600DC9900DB9750EABC68E6BF008B49BEC2C0DFDFDFF1F1F1F9F9
            F9FCFCFCFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFE9E9E9008E4E38996CCDCDCDEB
            EBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F94CA77C
            20B67E35E9B400DB9700DB970CDD9F64EEC538C996168B52A6B5AECECECEDCDC
            DCE2E2E2E7E7E7E9E9E9E9E9E9E9E9E9E9E9E9D8D8D8008B4B00C6863E986ECD
            CDCDEBEBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEBDD8CC
            048F5053E9BF19DEA600D89700D8971ADEA54EEABE2EC691008C4A4C9A7594AE
            A293B0A3BBBBBBBCBCBCBCBCBCBCBCBCBCBCBCB7B7B700884800E3A600BF823E
            986DCDCDCDEBEBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7
            078B4D36BC8D62E8C604D49D00D19800D19813D7A33AE1B632D8AA11B37A0A9D
            610C8E4F00884600884700884700884700884700874600834200D9A100D8A000
            BA813E986DCDCDCDEBEBEBFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
            ADD4C2008A4965DBB966E5C600CD9A00CB9700CC9801CE9B18D5A624D8AD27DB
            AF29E0B100D8A100D9A200D9A200D9A200D9A200D9A200D7A000D19C00D19C00
            D4A000B8803E986DD1D1D1F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FCFCFC50AB811195598AE3CE75E4CD10CCA200C59800C69900C69A00C79A00C7
            9B00CB9A00CB9A00CB9B00CB9B00CB9B00CB9B00CB9B00CB9B00CB9A00CB9A00
            CC9B00D09F00B7813F9C71EDEDEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFAFAFA18945912955978DAC18CE7D843D4B800C19B00C09700C09800C0
            9900C39900C39900C39900C39900C39900C39900C39900C29A00C89A00C89B00
            C89B07CBA12CD9B8008A48F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFB3FA676008C4A44B8908DE2D58DE4D858D6C139CCB31BC4
            A71EC5A800C4A301C4A301C4A301C4A301C4A301C4A300C4A300C29800C29900
            C39A62DFC700B2814CAA7EFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFEFEFEB4DAC9078C4C008D4C3AB1876DCFB898E6E198E5
            E09BE7E04EE3D551E3D552E3D552E3D552E3D551E2D54DE1D293E6D700BF9760
            DCC600AE7F51AB7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEB3DAC853AE82098C4C0088430087
            4200874200874300874300874300874300874300874200823B82E3D65ADAC400
            AB7F51AB7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9E9E90089447BE3DA00A97F51
            AC7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE008C4A00AB8751AC7FFC
            FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9008E4D4CAD80FBFBFBFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = sbtReloadTrayClick
        end
      end
      object edtTemp: TEdit
        Left = 840
        Top = 8
        Width = 65
        Height = 26
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = 'edtTemp'
        Visible = False
      end
    end
    object tsHPData: TTabSheet
      Caption = 'Hot Plate'
      ImageIndex = 4
      object palHPDef: TPanel
        Left = 0
        Top = 0
        Width = 929
        Height = 40
        Align = alTop
        BevelInner = bvLowered
        Caption = 'Hot Plate Define'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object pnlHP: TPanel
        Left = 0
        Top = 40
        Width = 929
        Height = 53
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object btnAddHP: TSpeedButton
          Left = 190
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Add HP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D296B2A52E9465008A4900
            8949008949008A492E946596B2A5D2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D24B9D7700915200B67900CD9000
            D19400D19400CD9000B6790091524B9D77D2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF4EA07A00965600CB8F00CD8F00C98A00
            C68500C68500C98A00CD8F00CB8F0096564EA07ADFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E09FBBAE00925200C98D00CA8C00C78800C380FF
            FFFFFFFFFF00C38000C78800CA8C00C98D009251A2BEB1F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B833966A00B37500C88A00C58600C38300BE79FF
            FFFFFFFFFF00BE7900C38300C58600C88A00B374329969EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09BA7D09CC7E0900884900C58700C38300BE7C00BC7700B86FFF
            FFFFFFFFFF00B86F00BC7700BE7C00C38300C587008A4AE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF6F8FFFFFDFF00823E17CC9400BD7AFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BD7A1BCD96008949E9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEFEDEFFFF3FB007F3835D29F00BB76FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BB7739D3A2008948EAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EBE7E8FDEEF3007D3368D9B500BB7800B97400B77100B268FF
            FFFFFFFFFF00B26800B77100B97400BB786EDCB9008947F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF3EA97856C19728C89300BA7800B97800B46EFF
            FFFFFFFFFF00B46E00B97800BA782AC89563C7A03BA171F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFC8E6D900894587E0C11CC38A00B77400B36DFF
            FFFFFFFFFF00B36D00B7741DC48B8DE3C60B9457C1DDD0FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF4FB08400915091E1C55BD4AA0ABC7E00
            B06800B0680BBC7E5BD4AB93E3C90E995F61B18CFCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF65BB9400894553BF9490E0C39B
            E7CD9BE7CD90E0C353C09600905152862FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCEEDDF42AE7D00813700
            823900823900813634A171C1E2DECA7F0CE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B1B1B1B1B5B2B3BAB5B8C2B8BDC7BAC0C9
            BBC1C8BBC1C5B9BEEFE5EAFEFEFFBA7E0BE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DEDDE0DFDEE2DFDFE3DFE0E3
            E0E0E3E0E0E2DFDEDFDEDFF6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnAddHPClick
        end
        object btnDeleteHP: TSpeedButton
          Left = 372
          Top = 9
          Width = 170
          Height = 36
          Caption = 'Delete HP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D29FA2BD4F5BBB2B3BBF2B
            3BBE2B3BBE2B3BBF4F5BBB9FA2BDD2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D2666EBC3344C75164EB6578FF69
            7CFF697CFF6578FF5164EB3344C7666EBCD2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF6972BF3546CA6072FE6174FE6073FD5F
            72FC5F72FC6073FD6174FE6072FE3546CA6972BFDFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E0A7ABC73242C85B6FFC5B6FFC596EFA596DF959
            6DF9596DF9596DF9596EFA5B6FFC5B6FFC3242C7ABAEC9F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B84B5AC4465AE8566BFA5268F85167F75166F751
            66F75166F75166F75167F75268F8566BFA475BE6545FC0EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09B97E07C687001D32CA4C63F84A61F54159F33D55F23C54F23C
            54F23C54F23C54F23D55F24159F34A61F54F64F62D3CBFE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF5F9FFFFFFFF192CC15D71F83B53F2FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF3B53F25F74F82C3BBFE9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEEEEEDFEFCEF1628BC6D81F8334EF0FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF334EF07084F82A3ABFEAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EAE8E6F9F5E91124BA8E9DF6324DED2F4AEC2C47EB2B47EB2B
            47EB2B47EB2B47EB2C47EB2F4AEC334EED93A2F72A3AC0F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF626ED17986E35870F0324DEB3550EB3651EB37
            51EB3751EB3651EB3550EB324DEB5A71F0848FE65A66C9F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFD2D5F22434C1A0ADF44C63ED2D48E9314BE933
            4DE9334DE9314BE92D48E94D64EEA5B2F63947C7CACDE8FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF707BD52E40C5A5B0F57B8DF03E57E925
            42E52542E53E57E97C8DF1A7B3F83B4DCF7882D2FCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF828CDB2334C27280E1A3AEF4AE
            B9F8AEB9F8A3AEF47281E3283CCD6C5A6FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD9DCF86773D71527BF17
            29C01729C01427BF5964CACBD3F7C58701E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B0B1B1B1B3B3B2B9B8B3BEBDB5C3C1B6C4
            C3B6C4C2B6C1C0B4ECEBE2FCFFFFB97F08E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DFDDE1E0DEE2E1DEE2
            E1DEE2E1DEE1E0DDDFDEDDF6FCFFB67D09E9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnDeleteHPClick
        end
        object btnModifyHP: TSpeedButton
          Left = 8
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Modify Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9F0F0F0EAEAEAE9
            E9E9E9E9E9EAEAEAF0F0F0F9F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF1F1F1DFDFDFCBCBCBBFBFBFBC
            BCBCBCBCBCBFBFBFCBCBCBDFDFDFF1F1F1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEDEDEDD2D2D296B2A52E9465008A4900
            8949008949008A492E946596B2A5D2D2D2EDEDEDFCFCFCFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1D2D2D24B9D7700915200B67900CD9000
            D19400D19400CD9000B6790091524B9D77D2D2D2F1F1F1FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF9F9F9DFDFDF4EA07A00965600CB8F00CD8F00C98A00
            C68500C68500C98A00CD8F00CB8F0096564EA07ADFDFDFF9F9F9FBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
            E9E9E9E9E9E9E9E9E9E9E0E0E09FBBAE00925200C98D00CA8C00C78800C380FF
            FFFFFFFFFF00C38000C78800CA8C00C98D009251A2BEB1F0F0F0F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
            BCBCBCBCBCBCBCBCBCBCB8B8B833966A00B37500C88A00C58600C38300BE79FF
            FFFFFFFFFF00BE7900C38300C58600C88A00B374329969EAEAEAEAEAEABB964C
            B77F0FB67D0AB67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C09B67C
            09B67C09B67C09BA7D09CC7E0900884900C58700C38300BE7C00BC7700B86FFF
            FFFFFFFFFF00B86F00BC7700BE7C00C38300C587008A4AE9E9E9E9E9E9B77F0F
            F7FFFFF3F8FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7FFF2F7
            FFF2F7FFF2F7FFF6F8FFFFFDFF00823E17CC9400BD7AFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BD7A1BCD96008949E9E9E9E9E9E9B67D0A
            F4FBFFECEDF0ECECEDECECEDECECEDECECEDECECEDECECEDECECEDECECEDECEC
            EDEBECEDEBEBEDEFEDEFFFF3FB007F3835D29F00BB76FFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF00BB7739D3A2008948EAEAEAE9E9E9B67C09
            F4F9FFEAE9EBEBE8E8EBE9E9EBE9E9EBE8E9EBE9E9EBE9E9EBE9E9EAE8E8EAE8
            E8E9E6E6E8E6E6EBE7E8FDEEF3007D3368D9B500BB7800B97400B77100B268FF
            FFFFFFFFFF00B26800B77100B97400BB786EDCB9008947F0F0F0E9E9E9B67C09
            F4FAFFE9E9E9EBEAE9EEEDECEEEDEBECEBE9EDECEBEEEDECEDECEBEAE9E8E7E6
            E5FBFBFBFFFFFFFFFFFFFFFFFF3EA97856C19728C89300BA7800B97800B46EFF
            FFFFFFFFFF00B46E00B97800BA782AC89563C7A03BA171F9F9F9E9E9E9B67C09
            F4FAFFE7E7E8ECEBEB4F4E4E919090EEEDED8F8E8F9190908F8E8EE9E8E8E6E5
            E5ECEBEBFFFFFFFFFFFFFFFFFFC8E6D900894587E0C11CC38A00B77400B36DFF
            FFFFFFFFFF00B36D00B7741DC48B8DE3C60B9457C1DDD0FEFEFEE9E9E9B67C09
            F4FAFFE5E5E6E9E8E8F1F0F0F1F0F0EEEDEDEDECECEDECECECEBEBE7E6E6E4E3
            E3DEDDDBFFFFFFFFFFFFFFFFFFFFFFFF4FB08400915091E1C55BD4AA0ABC7E00
            B06800B0680BBC7E5BD4AB93E3C90E995F61B18CFCFCFCFFFFFFE9E9E9B67C09
            F4FAFFE2E2E4E7E6E64D4D4D9190908F8E8E8D8C8CE9E8E8898888E5E4E4E3E2
            E3D1D0CDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF65BB9400894553BF9490E0C39B
            E7CD9BE7CD90E0C353C09600905152862FE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFE1DFE1E4E1E1E7E5E5E8E6E6E7E5E5E6E4E4E4E2E2E4E2E2E3E1E1E3E1
            E1C5C3C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCEEDDF42AE7D00813700
            823900823900813634A171C1E2DECA7F0CE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDEDEDFDFDEDDE0DFDEE0E0DEE0DFDEE0DFDEE0DFDEE0DFDDE0DFDDE1E0
            DFB2B2B3B1B2B2B0B1B1B0B0B0B0B0B1B1B1B1B5B2B3BAB5B8C2B8BDC7BAC0C9
            BBC1C8BBC1C5B9BEEFE5EAFEFEFFBA7E0BE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDCDCDDDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDEDD
            DCDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDDFDEDDE0DEDDE0DFDEE2DFDFE3DFE0E3
            E0E0E3E0E0E2DFDEDFDEDFF6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C09
            F5FBFFDADADADBDAD9DCDBDADCDBDADCDBDADBDBDADCDBDADCDBDADBDAD9DAD9
            D8D9D8D7D8D7D6D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8D7D5D8
            D7D5D8D7D6D9D8D6D9D9D9F5FBFFB67C09E9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD9D8D9DCDAD9E0DEDDE0DEDDDFDDDCDDDBDADFDCDBDEDCDBDCDAD8D8D6
            D5FBFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFBFBFBD7D5D6F5FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67C0A
            F6FBFFD8D6D7DDDBDA4A49498A8988878685DFDDDC868584858583DBD9D8D6D4
            D3E9E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFE9E8E8D5D3D3F6FBFFB67C0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD5D4D6DBD9D9E2E0E0E3E1E1E1DFDFDFDDDDE0DDDEDEDCDCD9D7D7D5D3
            D3D8D7D6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFD7D6D5D3D2D4F6FBFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD3D3D4D9D8D7494848888787868586868585868585838282D7D6D5D4D4
            D2C7C6C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFC7C6C5D2D2D2F6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFD0D0D1D4D3D2D8D7D6D8D7D6D8D7D6D8D7D6D7D7D5D6D5D4D4D3D2D4D3
            D2BAB7B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFB9B7B4D1D1D2F7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCECDCED1CFCED2CFCED2D0CFD2CFCED2CFCED2CFCED1CFCED1CFCED3D0
            CFA3A3A4A2A2A2A0A1A1A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0
            A1A1A2A2A2A2A3A3D0CFCFF7FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0A
            F6FCFFCAC9CBCCCACACCCACACCCACACCCACACCCACACCCACACCCACACCCACACDCB
            CBCECDCCCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
            CDCDCECDCDCDCCCCCBCACCF6FCFFB67D0AE9E9E9FFFFFFFFFFFFE9E9E9B67D0C
            F4FDFFF2F7FFF2F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8FFF3F8
            FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3F9FFF3
            F9FFF3F9FFF3F8FFF2F7FFF4FDFFB67D0CE9E9E9FFFFFFFFFFFFE9E9E9B67F0F
            FAE3C2E3AB52E3AC55E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD
            56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3AD56E3
            AD56E3AD56E3AC55E3AB52FAE3C2B67F0FE9E9E9FFFFFFFFFFFFE9E9E9B68012
            F5DDB8D99E39DAA13FDAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA1
            40DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DAA140DA
            A140DAA140DAA13FD99E39F5DDB8B68012E9E9E9FFFFFFFFFFFFEAEAEAB68114
            F1D6A9D18B17D28E1BD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E
            1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD28E1CD2
            8E1CD28E1CD28E1BD18B17F1D6A9B68114EAEAEAFFFFFFFFFFFFF2F2F2B88217
            EECF9BECCD98ECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE
            9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AECCE9AEC
            CE9AECCE9AECCE9AECCD98EECF9BB88217F2F2F2FFFFFFFFFFFFFBFBFBCBA75E
            B88317B78216B78116B78116B78116B78116B78116B78116B78116B78116B781
            16B78116B78116B78116B78116B78116B78116B78116B78116B78116B78116B7
            8116B78116B78116B78216B88317CBA75EFBFBFBFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = btnModifyHPClick
        end
        object sbUpdateHP: TSpeedButton
          Left = 736
          Top = 9
          Width = 170
          Height = 36
          Caption = 'Save'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            660F0000424D660F000000000000360000002800000024000000240000000100
            180000000000300F000001000000010000000000000000000000FAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF9F9F9F9F9F9F8F8F8F8F8F8F8
            F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8
            F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F9F9F9F9F9F9FAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAF7F7F7EFEFEFE8
            E8E8E6E6E6E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
            E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5E5
            E5E5E5E5E9E9E9F3F3F3F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAF8F8F8ECECECD6D6D6C8C8C8C5C5C5C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
            C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4
            C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4C4D0D0D0E9E9E9F9F9F9FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAF0EFEFC0A6A7A55B6CA96270A45E6C934556
            A6A3A3A4A1A1A19C9C9C9797979393948D8D8E88888A8383867F7D837A7A8079
            78807978994759994759994759994759994759994759994759994756C4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAD2C1C1B28181
            AF6D79B8717FB7707DAA6472EFEFF1EEEEF1995D68B8717EB06B78E1E0E0DDDA
            DBD8D5D6D4D1D2CFCCCCCAC6C6DBD8D88F4B59A05565A05565BE848EB9727FB8
            727EB8717E994557C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7AAF6D79BA7481B97380B7737FAA6472E9E9EBECEBEF995D
            68B8717EB06B78E1DFE1DCDADDD7D5D6D2D0D1CDC9CAC8C3C3D9D6D68D49589F
            5464A05565BF848EB97380B9737FB9727F9B4457C4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC38D95BA7682BA7481B974
            80AA6472E5E4E7E9E9EB9A5D69B8737FB06B78E5E5E7E2E0E3DDDBDDD8D5D7D3
            D0D1CDCACBDDDADB8B48579D53639F5464BF858FBA7480BA7481BA74819B4558
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7AC48E96BA7783BA7682B97481AA6472E0DFE1E5E3E69B5F6BB97580B06B78E9
            EAECE6E5E8E2E1E4DDDCDED9D6D8D4D0D2E0DFDF8945549B50619D5363BF8590
            BA7481BA7481BA74819A4759C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7AC58F96BB7784BA7784BA7582AA6472DAD9DBE0
            DEE09B5F6BBA7582B06B78EDECEFEAE9EDE6E6E9E2E2E4DEDDDFDAD8DAE5E2E4
            874353994E5F9B5061C18690B97480BA7481BA74819C495CC4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC58F98BC7886BB
            7785BB7783AA6472D5D2D4DBD8DA9C606C925864925864EAEAEDEDEDF0EAEAED
            E8E7E9E4E2E5DFDDDFE8E7E8864252974C5D994E5FC08691B9727FB97380B973
            809C4B5CC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7AC49098BC7986BD7985BC7784AA6472CFCCCCD4D1D3D9D7D9DFDCDF
            E2E1E4E7E5E8EAEAEDEDECF0EAEBEEE8E7EAE4E2E6ECEAEC8642528642528743
            53C28791B7727FB8727EB9727F9C4E5DC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC79198BD7A87BD7987BC7985BB7785
            AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA6472AA64
            72AA6472AA6472AA6472B77A84B76F7DB7707DB7707EB8717E9E4E5FC4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6929A
            BE7B88BE7B87BC7986BC7886BB7785BB7784BA7682B97481B97480B87380B871
            7EB7717EB76F7DB76D7CB76D7BB76D7BB66D7BB56C7AB56C7AB66D7BB76F7CB7
            6F7CB7707D9F4E60C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7AC6929ABE7B88BE7B87BC7986BC7886BB7785BB7784BA76
            82B97481B97480B87380B8717EB76F7DB76D7CB76D7CB76C7AB66C7AB66D7BB5
            6C7AB56C7AB56C7AB66C7AB66C7AB76D7C9F4F60C4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC6939BBE7E89BC7884A75E
            70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70A7
            5E70A75E70A75E70A75E70A75E70A75E70A75E70A75E70B66B7AB66C7B9F5162
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7AC7949CBF7F8AAF6F7BEBE1E1FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFE
            FEFDFDFEFDFDFDFCFCFCFCFBFCFBFBFBFBFAFAFAF9F9F9F9F9F9F8F8F8E8D1D1
            A75E70B76D7BB66C7A9F5262C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7AC8959DC0808BAF6F7BFEFFFEFCFCFBFBFBFAFA
            FBFAFAFAFAF9FAF9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7
            F7F7F7F7F6F6F6F6F6F9F9F9A75E70B96F7DB66B7AA05363C4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9959DC1818CAF
            6F7BFEFFFEE1E1E1DFDFDFDDDDDDDBDCDBDADAD9D9D9D9D7D7D6D5D6D5D4D4D3
            D2D3D2D1D1D1D0D0D0CFCFCFCFCFCFCFCFCFCFCFCFF9FAF9A75E70BA707EB66B
            79A15466C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7AC9969EC1818DAF6F7BFEFFFEFCFCFCFCFCFBFBFBFBFBFBFAFBFAFA
            FAFAFAF9F9F9F9F9F9F9F9F9F9F9F8F8F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7
            F6FBFBFBA75E70B97480B56B79A25666C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989FC2828EAF6F7BFEFFFEE4E4E4
            E3E3E3E2E2E1E0E0E0DEDDDEDCDCDCDADADAD9D9D9D7D8D8D6D5D5D4D4D4D2D3
            D3D1D2D1D0D0D0CFCFCFCFCFCFFCFCFBA75E70BA7581B56B79A45968C4C4C4E5
            E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AC9989F
            C2838EAF6F7BFEFFFEFCFCFCFCFCFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFA
            F9F9F9F9F9F9F9F9F9F8F8F9F8F8F8F8F8F7F7F7F7F7F7F7F7FCFCFCA75E70BA
            7783B66C7AA55969C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAAE6A7ACA98A0C2838EAF6F7BFEFFFEE6E6E6E6E5E6E4E4E4E3E3
            E3E1E2E2E0E0E0DEDEDEDCDCDCDBDBDBD9D9D9D8D8D8D6D6D6D4D5D5D3D3D3D1
            D1D1D0D0D0FDFDFCA75E70BC7783B76D7BA55D6CC4C4C4E5E5E5F8F8F8FAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB98A0C2838EAF6F7BFEFF
            FEFDFDFDFDFDFCFCFDFCFCFCFCFCFCFBFBFCFBFBFBFBFBFBFAFAFAFAFAFAF9F9
            F9F9F9F9F9F9F9F9F8F8F8F8F8F8F7F7F7FDFEFDA75E70BD7985B76F7DA65F6E
            C4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A
            7ACB9BA1C2838EAF6F7BFEFFFEE6E6E6E6E6E6E6E6E6E5E5E6E4E5E5E4E3E3E2
            E2E2E1E0E0DFDFDFDDDDDDDBDBDCDADAD9D9D8D9D7D6D7D5D5D5D3D4D3FEFEFE
            A75E70BE7A86B7707DA6606EC4C4C4E5E5E5F8F8F8FAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFEFDFEFEFDFD
            FDFCFDFDFCFCFDFCFCFCFCFCFCFBFCFCFBFBFBFAFBFBFAFAFAF9F9F9F9F9F9F9
            F9F9F9F9F9F8F8F8F8FEFFFEA75E70B38088B8717FA7606EC4C4C4E5E5E5F8F8
            F8FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACB9BA1C2838EAF
            6F7BFEFFFEE6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E6E5E5E5E4E4E3E2E3E3
            E1E1E1E0DFDFDDDDDEDCDBDCDADADAD9D9D9D7D8D7FEFFFEA75E70996E759E59
            67A9606EC5C5C5E5E5E5F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAAE6A7ACB9BA1C2838EAF6F7BFEFFFEFEFFFEFEFEFEFEFEFDFEFEFDFDFDFC
            FDFDFCFCFDFCFCFCFCFCFCFBFBFBFBFBFBFAFAFAFAFAFAF9FAFAF9F9F9F9F9F9
            F9FEFFFEA75E70B0757F9E5A68AA6471C5C5C5E6E6E6F9F9F9FAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7ACA969EC2838EAF6F7BFEFFFEFEFFFE
            FEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEFEFF
            FEFEFFFEFEFFFEFEFFFEFEFFFEFEFFFEA75E70C77F8BC77F8BAA6572D1D1D1EB
            EBEBFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAAE6A7AAE6A7A
            AE6A7AAE6A7AD9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6
            D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6D9D6D6A75E70A7
            5E70A75E70A75E70ECECECF6F6F6FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFA
            FAFAFAFAFAFAFAFAFAFA}
          Margin = 20
          ParentFont = False
          OnClick = sbUpdateHPClick
        end
        object sbtReloadHP: TSpeedButton
          Left = 554
          Top = 9
          Width = 170
          Height = 36
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Load Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2F0F0F0F9F9
            F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEAEAEAD0D0D0D1D1D1EEEE
            EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD38996C008E4EE9E9
            E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD3E986E00C686008B4BD8D8
            D8E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9EBEBEBF1F1F1F8
            F8F8FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFCFCFCEBEBEBCDCDCD3E986D00BF8200E3A6008848B7B7
            B7BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBDBDBDC1C1C1CCCCCCDB
            DBDBEAEAEAF7F7F7FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFBEBEBEBCDCDCD3E986D00BA8100D8A000D9A10083420087
            46008847008847008847008847008847008847008947008948078B4D3C986D83
            AA98C4C4C4D9D9D9EEEEEEFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFF2F2F2D1D1D13E986D00B88000D4A000D19C00D19C00D9A000DB
            A200DBA200DBA200DBA200DBA200DBA200DBA200D3A200D5A300C18B00AA7000
            8E4D058B4D85AB9ACCCCCCE7E7E7FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFEDEDED3F9C7100B78100D09F00CC9B00CB9A00CB9A00CE9B00CE
            9B00CE9B00CE9B00CE9B00CE9B00CE9B00CF9B00CB9B00CC9C00CD9D00CF9F00
            CD9C00AC750089472E9666C8C8C8E8E8E8FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFF2F2F2008A482CD9B807CBA100C89B00C89B00C89B00C89900C8
            9900C89900C89900C89900C89900C89900C89900C89B00C89B00C89B00C99C00
            CA9E00CDA100C293008E4D139157CECECEEFEFEFFEFEFEFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFB4CAA7E00B28162DFC700C39A00C29900C29800C19800C1
            9900C19900C19900C19900C19900C19900C19900C39800C39800C39900C49A00
            C59B00C69C00C8A000C093008D4D3F9C71DADADAF7F7F7FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFCFCFC51AB7F00AE7F60DCC600BE9794E5D74DE6D151E8
            D452E8D552E8D552E8D551E8D57FE1D37DE1D37CE4D172E1CC49D5B906C59E00
            C29900C39B00C49C00C6A000BE93008A478EB5A3EBEBEBFEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AB7F00AB7F5AD9C482E3D600823B0086
            42008743008743008743008743008743008A49169E6626B1826CD8C175E0CD27
            CBAD00BE9800C09A00C19C00C4A100A975068E4DDDDDDDF9F9F9FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AC7F00A97F7BE3DA008944E9E9
            E9FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEC8E5D8CBE5D969B791008A484BC19D81
            E2D328C8AC00BC9800BE9B00C09D00C09D008E4DA5C1B4F1F1F1FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC51AC7F00AB87008C4AEEEE
            EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEDEEDE61D92574C
            C09F85E1D30EBFA200B99A00BB9C00BEA100A17043A074EBEBEBFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFB4CAD80008E4DF9F9
            F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEEEC00
            8A4775D8C758D3C100B69800B79A00B99D00AF8C078D4CEBEBEBFBFBFBF2F2F2
            EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9EDEDEDF7F7F7FEFEFEFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F961
            B18B2DB18A8FE4DE7BDDD47DDED47EDFD683E3DD008B48F2F2F2F2F2F2D4D4D4
            C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCC7C7C7E3E3E3F9F9F9FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEC0
            E0D1008B49008944008843008843008944008A474BAC7FFBFBFBEBEBEB3B9D70
            008949008747008747008746008746008A49A0C0B1F1F1F1FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9F0F0F0F2F2F2FBFBFBFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB008949
            11ECAE03E5A400E3A000E29E96FBDF2FBE89509F79E0E0E0F9F9F9FEFEFEFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEED1D1D1D0D0D0EAEAEAFC
            FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1078B4D
            22D39A14E3A600DC9900DB9750EABC68E6BF008B49BEC2C0DFDFDFF1F1F1F9F9
            F9FCFCFCFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFE9E9E9008E4E38996CCDCDCDEB
            EBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F94CA77C
            20B67E35E9B400DB9700DB970CDD9F64EEC538C996168B52A6B5AECECECEDCDC
            DCE2E2E2E7E7E7E9E9E9E9E9E9E9E9E9E9E9E9D8D8D8008B4B00C6863E986ECD
            CDCDEBEBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEBDD8CC
            048F5053E9BF19DEA600D89700D8971ADEA54EEABE2EC691008C4A4C9A7594AE
            A293B0A3BBBBBBBCBCBCBCBCBCBCBCBCBCBCBCB7B7B700884800E3A600BF823E
            986DCDCDCDEBEBEBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7
            078B4D36BC8D62E8C604D49D00D19800D19813D7A33AE1B632D8AA11B37A0A9D
            610C8E4F00884600884700884700884700884700874600834200D9A100D8A000
            BA813E986DCDCDCDEBEBEBFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
            ADD4C2008A4965DBB966E5C600CD9A00CB9700CC9801CE9B18D5A624D8AD27DB
            AF29E0B100D8A100D9A200D9A200D9A200D9A200D9A200D7A000D19C00D19C00
            D4A000B8803E986DD1D1D1F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FCFCFC50AB811195598AE3CE75E4CD10CCA200C59800C69900C69A00C79A00C7
            9B00CB9A00CB9A00CB9B00CB9B00CB9B00CB9B00CB9B00CB9B00CB9A00CB9A00
            CC9B00D09F00B7813F9C71EDEDEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFAFAFA18945912955978DAC18CE7D843D4B800C19B00C09700C09800C0
            9900C39900C39900C39900C39900C39900C39900C39900C29A00C89A00C89B00
            C89B07CBA12CD9B8008A48F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFBFBFB3FA676008C4A44B8908DE2D58DE4D858D6C139CCB31BC4
            A71EC5A800C4A301C4A301C4A301C4A301C4A301C4A300C4A300C29800C29900
            C39A62DFC700B2814CAA7EFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFEFEFEB4DAC9078C4C008D4C3AB1876DCFB898E6E198E5
            E09BE7E04EE3D551E3D552E3D552E3D552E3D551E2D54DE1D293E6D700BF9760
            DCC600AE7F51AB7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEB3DAC853AE82098C4C0088430087
            4200874200874300874300874300874300874300874200823B82E3D65ADAC400
            AB7F51AB7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9E9E90089447BE3DA00A97F51
            AC7FFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE008C4A00AB8751AC7FFC
            FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9008E4D4CAD80FBFBFBFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
          Margin = 20
          ParentFont = False
          OnClick = sbtReloadHPClick
        end
      end
      object strngrdHP: TStringGrid
        Left = 0
        Top = 93
        Width = 929
        Height = 680
        Align = alClient
        Color = 14670284
        TabOrder = 2
        OnDblClick = strngrdHPDblClick
        OnSelectCell = strngrdHPSelectCell
      end
    end
  end
  object sbExit: TPanel
    Left = 0
    Top = 833
    Width = 937
    Height = 42
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = sbExitClick
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 784
  end
  object OpenDialog1: TOpenDialog
    Filter = '*.csv|*.csv; *.txt'
    Left = 812
  end
  object OpenDialog3: TOpenDialog
    Filter = '*.csv|*.csv; *.txt'
    Left = 868
  end
  object OpenDialog2: TOpenDialog
    Filter = '*.csv|*.csv; *.txt'
    Left = 840
  end
end
